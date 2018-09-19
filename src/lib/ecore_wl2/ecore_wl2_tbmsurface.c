#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "ecore_wl2_private.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <wayland-client.h>
#include <wayland-tbm-client.h>
#include <tbm_surface_internal.h>

#define KEY_WINDOW (unsigned long)(&key_window)
#define KEY_WL_BUFFER (unsigned long)(&key_wl_buffer)
#define KEY_SURFACE_INFO (unsigned long)(&key_surface_info)

static int key_window;
static int key_wl_buffer;
static int key_surface_info;

typedef struct {
  unsigned int age;
  unsigned int num_surface;
} tbm_buffer_info;

typedef struct _Ecore_Wl2_Tbmbuf_Private
{
   Ecore_Wl2_Buffer *current;
   Eina_List *buffers;
} Ecore_Wl2_Tbmbuf_Private;

static int   tbm_queue_ref = 0;
static Eina_Bool need_to_tbm_queue = EINA_TRUE;
static void *tbm_queue;

typedef enum
{
  MODE_FULL,
  MODE_COPY,
  MODE_DOUBLE,
  MODE_TRIPLE,
  MODE_QUADRUPLE,
  MODE_AUTO
} Render_Output_Swap_Mode;


static void
_evas_tbmbuf_buffer_unmap(Ecore_Wl2_Buffer *surf)
{
  tbm_surface_unmap(surf->tbm_surface);
}


static void
_evas_tbmbuf_surface_reconfigure(Ecore_Wl2_Surface *surface, void *priv_data, int w, int h, uint32_t flags , Eina_Bool force )
{
  Ecore_Wl2_Tbmbuf_Private *p = priv_data;

  Ecore_Wl2_Buffer *surf;

  if (!surface) return;
  if (!p) return;

  surf = p->current;

  if (!surf) return;

  /* TIZEN_ONLY(20171204) : temporary patch */
  if (w < 1) w = 1;
  if (h < 1) h = 1;

  if ((w >= surf->w) && (w <= surf->stride / 4) && (h == surf->h))
    {
      surf->w = w;
      return;
    }
  surf->resize = EINA_TRUE;
  surf->w = w;
  surf->h = h;
}

static void
_wait_free_buffer(Ecore_Wl2_Buffer *surf)
{
  if (!surf) return;

  int ret;
  ret = wl_display_dispatch_pending(surf->wl_display);
  if (ret < 0)
  {
     ERR("Wayland Display Dispatch Pending Failed");
     return;
  }

  if ( tbm_surface_queue_can_dequeue(surf->tbm_queue, 0))
    return;

  DBG("WAIT free buffer");

  while (! tbm_surface_queue_can_dequeue(surf->tbm_queue, 0)) {
      wl_display_dispatch(surf->wl_display);
  }
}

static void
buffer_release(void *data, struct wl_buffer *buffer EINA_UNUSED)
{
  if (!data) return;
  void *tbm_queue = NULL;
  tbm_surface_h tbm_surface = data;
  tbm_surface_queue_error_e tsq_err = TBM_SURFACE_QUEUE_ERROR_NONE;

  tbm_surface_internal_get_user_data(tbm_surface, KEY_WINDOW, (void **)&tbm_queue);
  if (tbm_queue)
    {
      tsq_err = tbm_surface_queue_release(tbm_queue, tbm_surface);
      if (tsq_err != TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST)
        tbm_surface_internal_unref(tbm_surface);
    }
  else
    tbm_surface_internal_unref(tbm_surface);
}

static void
buffer_destroy(void *data)
{
  if (!data) return;
  struct wl_buffer* buffer = data;
  wl_buffer_destroy(buffer);
}

static const struct wl_buffer_listener buffer_listener = {
    buffer_release
};


static int
_evas_tbmbuf_surface_assign(Ecore_Wl2_Surface *surface, void *priv_data)
{
  Ecore_Wl2_Buffer *surf;
  Ecore_Wl2_Tbmbuf_Private *p = priv_data;

  Render_Output_Swap_Mode mode = MODE_FULL;
  tbm_buffer_info *tbuf_info = NULL;
  if (!surface) return 0;
  if (!p) return 0;

  surf = p->current;
  if (!surf)
    {
      ERR("surface is NULL");
      return 0;
    }

  if (surf->resize)
    {
      // reset tbm_surface
      int width, height, format;
      width = surf->w;
      height = surf->h;
      format =  tbm_surface_queue_get_format(surf->tbm_queue);
      tbm_surface_queue_reset(surf->tbm_queue, width, height, format);
      surf->resize = EINA_FALSE;
    }

  surf->tbm_surface = NULL;
  tbm_surface_queue_error_e ret = TBM_SURFACE_QUEUE_ERROR_NONE;
  struct wl_buffer *buffer;

  _wait_free_buffer(surf);

  // check num of tbm surface
  int num_surface, i;
  tbm_surface_h surfaces[5];
  tbm_surface_queue_get_surfaces(surf->tbm_queue, surfaces, &num_surface);

  ret =  tbm_surface_queue_dequeue(surf->tbm_queue, &surf->tbm_surface);

  if (ret != TBM_SURFACE_QUEUE_ERROR_NONE ||
      surf->tbm_surface == NULL) {
      ERR("dequeue:%p from queue:%p err:%d\n", surf->tbm_surface, surf->tbm_queue, ret);
      surf->wait_release = 1;
      return 0;
  }

  tbm_surface_internal_ref(surf->tbm_surface);

  surf->wait_release = 0;
  if(! tbm_surface_internal_get_user_data(surf->tbm_surface, KEY_WL_BUFFER, (void **)&buffer)) {
      buffer =  wayland_tbm_client_create_buffer(surf->tbm_client, surf->tbm_surface);
      if (!buffer)
        {
          ERR("wayland_tbm_client_create_buffer is NULL");
          tbm_surface_internal_unref(surf->tbm_surface);
          tbm_surface_queue_release(surf->tbm_queue, surf->tbm_surface);
          return 0;
        }

      wl_proxy_set_queue((struct wl_proxy *)buffer, NULL);

      tbm_surface_internal_add_user_data(surf->tbm_surface, KEY_WL_BUFFER, (tbm_data_free)buffer_destroy);
      tbm_surface_internal_set_user_data(surf->tbm_surface, KEY_WL_BUFFER, buffer);

      tbm_surface_internal_add_user_data(surf->tbm_surface, KEY_WINDOW, NULL);
      tbm_surface_internal_set_user_data(surf->tbm_surface, KEY_WINDOW, surf->tbm_queue);

      wl_buffer_add_listener(buffer, &buffer_listener, surf->tbm_surface);
  }


  tbm_surface_info_s info;
  tbm_surface_map(surf->tbm_surface, TBM_SURF_OPTION_READ|TBM_SURF_OPTION_WRITE, &info);
  surf->tbm_info = info;

  // check buffer age
  if (! tbm_surface_internal_get_user_data(surf->tbm_surface, KEY_SURFACE_INFO, (void**)&tbuf_info)) {
      tbuf_info = calloc(1, sizeof(tbm_buffer_info));
      if (tbuf_info) {
          tbuf_info->age = 0;
          tbuf_info->num_surface = num_surface;
      }
      tbm_surface_internal_add_user_data(surf->tbm_surface, KEY_SURFACE_INFO, free);
      tbm_surface_internal_set_user_data(surf->tbm_surface, KEY_SURFACE_INFO, tbuf_info);
  }

  //reset
  if (tbuf_info && (unsigned int)num_surface != tbuf_info->num_surface)
    {
      surface->frame_age = 0;

      for(i=0; i<num_surface; i++) {
          tbm_surface_internal_get_user_data(surfaces[i], KEY_SURFACE_INFO, (void **)&tbuf_info);
          if (tbuf_info) {
              tbuf_info->age = 0;
              tbuf_info->num_surface = num_surface;
          }
      }
    }

  if (!tbuf_info || !tbuf_info->age)
    return MODE_FULL;

  if (tbuf_info && tbuf_info->age < surface->frame_age)
    {
      unsigned int diff;
      diff = surface->frame_age - tbuf_info->age + 1;
      if (diff > (unsigned int)num_surface) return MODE_FULL;
      switch(diff)
      {
        case 1:
          mode = MODE_COPY;
          break;
        case 2:
          mode = MODE_DOUBLE;
          break;
        case 3:
          mode = MODE_TRIPLE;
          break;
        default:
          mode = MODE_FULL;
      }
    }

  return mode;
}


static void *
_evas_tbmbuf_surface_data_get(Ecore_Wl2_Surface *surface, void *priv_data, int *w, int *h)
{
  Ecore_Wl2_Buffer *surf;
  void *image;
  Ecore_Wl2_Tbmbuf_Private *p = priv_data;

  if (!p) return 0;

  surf = p->current;
  if (!surf)
    {
      ERR("surface is NULL");
      return 0;
    }

  image = surf->tbm_info.planes[0].ptr;

  surf->stride = surf->tbm_info.planes[0].stride;
  if (w) *w = surf->stride / 4;
  if (h) *h = surf->tbm_info.height;

  return image;
}

void
_evas_surface_damage(struct wl_surface *s, int compositor_version, int w, int h, Eina_Rectangle *rects, unsigned int count)
{
  void (*damage)(struct wl_surface *, int32_t, int32_t, int32_t, int32_t);
  unsigned int k;

  if (compositor_version >= WL_SURFACE_DAMAGE_BUFFER_SINCE_VERSION)
    damage = wl_surface_damage_buffer;
  else
    damage = wl_surface_damage;

  if ((rects) && (count > 0))
    for (k = 0; k < count; k++)
      damage(s, rects[k].x, rects[k].y, rects[k].w, rects[k].h);
  else
    damage(s, 0, 0, w, h);
}


static void
_evas_tbmbuf_surface_post(Ecore_Wl2_Surface *surface, void *priv_data, Eina_Rectangle *rects, unsigned int count)
{
  Ecore_Wl2_Buffer *surf;
  tbm_buffer_info *tbuf_info = NULL;
  Ecore_Wl2_Tbmbuf_Private *p = priv_data;
  
  if (!surface) return;
  if (!p) return;

  surf = p->current;
  if (!surf->wl_surface) return;

  struct wl_buffer *buffer = NULL;

  _evas_tbmbuf_buffer_unmap(surf);

  tbm_surface_internal_get_user_data(surf->tbm_surface, KEY_WL_BUFFER, (void **)&buffer);
  if (!buffer) {
      ERR("Enqueue:%p from queue:%p", surf->tbm_surface, surf->tbm_queue);
      return;
  }

  ++surface->frame_age;
  tbm_surface_internal_get_user_data(surf->tbm_surface, KEY_SURFACE_INFO, (void **)&tbuf_info);
  if (tbuf_info)
    tbuf_info->age = surface->frame_age;

  ecore_wl2_window_buffer_attach(surface->wl2_win, buffer, 0, 0, EINA_FALSE);

  _evas_surface_damage(surf->wl_surface, surf->compositor_version,
                       surf->w, surf->h, rects, count);

  tbm_surface_internal_ref(surf->tbm_surface);
  ecore_wl2_window_commit(surface->wl2_win, EINA_TRUE);

  tbm_surface_internal_unref(surf->tbm_surface);
  tbm_surface_queue_enqueue(surf->tbm_queue, surf->tbm_surface);

  tbm_surface_queue_acquire(surf->tbm_queue, &surf->tbm_surface);

  return;
}

static void
_evas_tbmbuf_surface_destroy(Ecore_Wl2_Surface *surface, void *priv_data)
{
  Ecore_Wl2_Buffer *surf = NULL;
  Ecore_Wl2_Tbmbuf_Private *p = priv_data;
  if (!surface) return;
  if (!p) return;

  surf = p->current;
  if (surf)
    {
      if (surf->tbm_queue && tbm_queue_ref == 0)
        {
          if (surf->tbm_surface)
            tbm_surface_internal_set_user_data(surf->tbm_surface, KEY_WINDOW, NULL);
          tbm_surface_queue_destroy(surf->tbm_queue);
          surf->tbm_queue = NULL;
        }
      if (tbm_queue_ref)
        --tbm_queue_ref;
      free(surf);
      p->current = NULL;
    }
}

static void
_evas_tbmbuf_surface_flush(Ecore_Wl2_Surface *surface EINA_UNUSED,  void *priv_data EINA_UNUSED)
{
}

static Ecore_Wl2_Surface_Interface tbmbuf_smanager =
{
//   .check = _evas_tbmbuf_surface_check,
   .destroy = _evas_tbmbuf_surface_destroy,
   .reconfigure = _evas_tbmbuf_surface_reconfigure,
   .data_get = _evas_tbmbuf_surface_data_get,
   .assign = _evas_tbmbuf_surface_assign,
   .post = _evas_tbmbuf_surface_post,
   .flush = _evas_tbmbuf_surface_flush
};

EAPI void *
ecore_wl2_tbmbuf_surface_tbm_queue_get(Ecore_Wl2_Surface *surface)
{
  Ecore_Wl2_Buffer *surf = NULL;
  if (!surface) return NULL;
  Ecore_Wl2_Tbmbuf_Private *p = surface->private_data;
  if (!p) return NULL;

  surf = p->current;
  if (surf)
    {
      return surf->tbm_queue;
    }
  return NULL;
}

EAPI Ecore_Wl2_Surface *
ecore_wl2_tbmbuf_surface_create(Ecore_Wl2_Window *win, Eina_Bool alpha,
                                struct wayland_tbm_client *tbm_client, void *info_tbm_queue, int w, int h, int num_buff)
{
  Ecore_Wl2_Buffer *surf = NULL;

  Ecore_Wl2_Surface *out;
  Ecore_Wl2_Tbmbuf_Private *p = NULL;
//  Ecore_Wl2_Surface_Interface *intf;

  out = calloc(1, sizeof(*out));
  if (!out) return NULL;

  out->private_data = calloc(1, sizeof(Ecore_Wl2_Tbmbuf_Private));
  if (!out->private_data) goto err;
 
  if (!(surf = calloc(1, sizeof(Ecore_Wl2_Buffer)))) goto err;

  out->wl2_win = win;
  p = (Ecore_Wl2_Tbmbuf_Private *)out->private_data;
  p->current = surf;
  /* TIZEN_ONLY(20171204) : temporary patch */
  if (w < 1) w = 1;
  if (h < 1) h = 1;

  surf->w = w;
  surf->h = h;
  Ecore_Wl2_Display *wl2_display = ecore_wl2_window_display_get(win);
  surf->wl_display = ecore_wl2_display_get(wl2_display);
  surf->wl_surface = ecore_wl2_window_surface_get(win);
  if (!surf->wl_surface)  goto err;
  surf->alpha = alpha;
  surf->compositor_version = 3;

  /* get tbm_client */
  surf->tbm_client = tbm_client;
  if (surf->tbm_client == NULL) {
      ERR("No wayland_tbm global");
      goto err;
  }

  /* check num_buff, not yet support single buffer */
  if (num_buff == 1) num_buff = 2;

  /* create surface buffers */
  if (!info_tbm_queue)
    {
      info_tbm_queue =  wayland_tbm_client_create_surface_queue(surf->tbm_client,
                                                           surf->wl_surface,
                                                           num_buff,
                                                           w, h,
                                                           TBM_FORMAT_ARGB8888);
      surf->tbm_queue = info_tbm_queue;
    }
  else
    {
      /* reuse tbm_queue */
      surf->tbm_queue = info_tbm_queue;
      tbm_surface_queue_reset(surf->tbm_queue, w, h, TBM_FORMAT_ARGB8888);

      tbm_queue_ref++;
    }
  if (surf->tbm_queue == NULL)
    {
      ERR("surf->tbm_queue NULL");
      goto err;
    }
  surf->tbm_surface = NULL;
  out->funcs = &tbmbuf_smanager;

  win->wl2_surface = out;
  /* Since we have loadable modules, we need to make sure this
   * surface keeps ecore_wl2 from de-initting and dlclose()ing
   * things until after it's destroyed
   */
  ecore_wl2_init();

  return out;

  err:
  if (surf)
    {
      free(surf);
    }
  if (out)
    free(out);
  return NULL;
}
