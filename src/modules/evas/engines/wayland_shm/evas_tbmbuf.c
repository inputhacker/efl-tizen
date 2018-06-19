#include "evas_common_private.h"
#include "evas_private.h"
#include "evas_engine.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <wayland-client.h>

#include "tizen-surface-client.h"

#define KEY_WINDOW (unsigned long)(&key_window)
#define KEY_WL_BUFFER (unsigned long)(&key_wl_buffer)
#define KEY_SURFACE_INFO (unsigned long)(&key_surface_info)


static int key_window;
static int key_wl_buffer;
static int key_surface_info;



#define __tbm_fourcc_code(a,b,c,d) ((uint32_t)(a) | ((uint32_t)(b) << 8) | \
      ((uint32_t)(c) << 16) | ((uint32_t)(d) << 24))

#define TBM_FORMAT_XRGB8888 __tbm_fourcc_code('X', 'R', '2', '4')
#define TBM_FORMAT_ARGB8888 __tbm_fourcc_code('A', 'R', '2', '4')
#define TBM_SURF_PLANE_MAX 4 /**< maximum number of the planes  */
/* option to map the tbm_surface */
#define TBM_SURF_OPTION_READ      (1 << 0) /**< access option to read  */
#define TBM_SURF_OPTION_WRITE     (1 << 1) /**< access option to write */

typedef struct _tbm_surface * tbm_surface_h;
//typedef struct _tbm_surface_queue *tbm_surface_queue_h;
typedef uint32_t tbm_format;
typedef void (*tbm_data_free) (void *user_data);

typedef struct _tbm_surface_plane
{
   unsigned char *ptr;   /**< Plane pointer */
   uint32_t size;        /**< Plane size */
   uint32_t offset;      /**< Plane offset */
   uint32_t stride;      /**< Plane stride */

   void *reserved1;      /**< Reserved pointer1 */
   void *reserved2;      /**< Reserved pointer2 */
   void *reserved3;      /**< Reserved pointer3 */
} tbm_surface_plane_s;

typedef struct _tbm_surface_info
{
   uint32_t width;      /**< TBM surface width */
   uint32_t height;     /**< TBM surface height */
   tbm_format format;   /**< TBM surface format*/
   uint32_t bpp;        /**< TBM surface bbp */
   uint32_t size;       /**< TBM surface size */

   uint32_t num_planes;                            /**< The number of planes */
   tbm_surface_plane_s planes[TBM_SURF_PLANE_MAX]; /**< Array of planes */

   void *reserved4;   /**< Reserved pointer4 */
   void *reserved5;   /**< Reserved pointer5 */
   void *reserved6;   /**< Reserved pointer6 */
} tbm_surface_info_s;


typedef enum _queue_node_type {
   QUEUE_NODE_TYPE_NONE,
   QUEUE_NODE_TYPE_DEQUEUE,
   QUEUE_NODE_TYPE_ENQUEUE,
   QUEUE_NODE_TYPE_ACQUIRE,
   QUEUE_NODE_TYPE_RELEASE
} Queue_Node_Type;

typedef enum {
   TBM_SURFACE_QUEUE_ERROR_NONE = 0,                             /**< Successful */
   TBM_SURFACE_QUEUE_ERROR_INVALID_SURFACE = -1,
   TBM_SURFACE_QUEUE_ERROR_INVALID_QUEUE = -2,
   TBM_SURFACE_QUEUE_ERROR_EMPTY = -3,
   TBM_SURFACE_QUEUE_ERROR_INVALID_PARAMETER = -4,
   TBM_SURFACE_QUEUE_ERROR_SURFACE_ALLOC_FAILED = -5,
   TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST = -6,
   TBM_SURFACE_QUEUE_ERROR_UNKNOWN_SURFACE = -7,
} tbm_surface_queue_error_e;

typedef struct _Tbmbuf_Surface Tbmbuf_Surface;

struct _Tbmbuf_Surface
{
   struct wl_display *wl_display;
   struct wl_surface *wl_surface;
   struct wl_buffer *wl_buffer;
   struct wl_registry *registry;
   struct wayland_tbm_client *tbm_client;
   tbm_surface_h tbm_surface;
   tbm_surface_info_s tbm_info;
   void *tbm_queue;
   int wait_release;
   Eina_Bool resize : 1;

   int compositor_version;

   int w, h;
   int dx, dy;
   int stride;
   int frame_age;
   Eina_Bool alpha : 1;

   struct tizen_surface_shm_flusher *tzsurf_flusher;
};


typedef struct {
   unsigned int age;
   unsigned int num_surface;
} tbm_buffer_info;


static void *tbm_lib = NULL;
static void *tbm_client_lib = NULL;
static int   tbm_ref = 0;
static int   tbm_queue_ref = 0;
static struct tizen_surface_shm *tzsurf;

static int (*sym_tbm_surface_map) (tbm_surface_h surface, int opt, tbm_surface_info_s *info) = NULL;
static int (*sym_tbm_surface_unmap) (tbm_surface_h surface) = NULL;
static int (*sym_tbm_surface_queue_can_dequeue) (void *tbm_queue, int value) = NULL;
static int (*sym_tbm_surface_queue_dequeue) (void *tbm_queue, tbm_surface_h *surface) = NULL;
static int (*sym_tbm_surface_queue_enqueue) (void *tbm_queue, tbm_surface_h surface) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_acquire) (void *tbm_queue, tbm_surface_h *surface) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_get_surfaces) (void *surface_queue,
                                                                        tbm_surface_h *surfaces, int *num) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_release) (void *surface_queue, tbm_surface_h surface) = NULL;
static void (*sym_tbm_surface_queue_destroy) (void *surface_queue) = NULL;
static void (*sym_tbm_surface_internal_unref) (tbm_surface_h surface) = NULL;
static void (*sym_tbm_surface_internal_ref) (tbm_surface_h surface) = NULL;
static int (*sym_tbm_surface_internal_get_user_data) (tbm_surface_h surface, unsigned long key, void **data) = NULL;
static int (*sym_tbm_surface_internal_add_user_data) (tbm_surface_h surface, unsigned long key, tbm_data_free data_free_func) = NULL;
static int (*sym_tbm_surface_internal_set_user_data) (tbm_surface_h surface, unsigned long key, void *data) = NULL;
static int (*sym_tbm_surface_queue_get_format) (void *surface_queue) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_reset) (void *surface_queue, int width, int height, int format) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_flush) (void* surface_queue) = NULL;
static tbm_surface_queue_error_e (*sym_tbm_surface_queue_free_flush) (void* surface_queue) = NULL;

static struct wl_buffer * (*sym_wayland_tbm_client_create_buffer) (struct wayland_tbm_client *tbm_client, tbm_surface_h surface) = NULL;
static struct wl_tbm * (*sym_wayland_tbm_client_get_wl_tbm) (struct wayland_tbm_client *tbm_client) = NULL;
static void *(*sym_wayland_tbm_client_create_surface_queue) (struct wayland_tbm_client *tbm_client,
                                                             struct wl_surface *surface,
                                                             int queue_size,
                                                             int width, int height, tbm_format format) = NULL;

static Eina_Bool
tbm_init(void)
{
   if (tbm_lib)
      {
         tbm_ref++;
         return EINA_TRUE;
      }

   const char *tbm_libs[] =
         {
               LIBDIR"/libtbm.so.1",
               LIBDIR"/libtbm.so.0",
               NULL,
         };
   const char *tbm_clients[] =
         {
               LIBDIR"/libwayland-tbm-client.so.0",
               NULL,
         };
   int i, fail;
#define SYM(lib, xx)                            \
      do {                                          \
            sym_ ## xx = dlsym(lib, #xx);            \
            if (!(sym_ ## xx)) {                     \
                  ERR("%s", dlerror());               \
                  fail = 1;                           \
            }                                      \
      } while (0)

   for (i = 0; tbm_libs[i]; i++)
      {
         tbm_lib = dlopen(tbm_libs[i], RTLD_LOCAL | RTLD_LAZY);
         if (tbm_lib)
            {
               fail = 0;
               SYM(tbm_lib, tbm_surface_map);
               SYM(tbm_lib, tbm_surface_unmap);
               SYM(tbm_lib, tbm_surface_queue_can_dequeue);
               SYM(tbm_lib, tbm_surface_queue_dequeue);
               SYM(tbm_lib, tbm_surface_queue_enqueue);
               SYM(tbm_lib, tbm_surface_queue_acquire);
               SYM(tbm_lib, tbm_surface_queue_get_surfaces);
               SYM(tbm_lib, tbm_surface_queue_release);
               SYM(tbm_lib, tbm_surface_queue_destroy);
               SYM(tbm_lib, tbm_surface_internal_ref);
               SYM(tbm_lib, tbm_surface_internal_unref);
               SYM(tbm_lib, tbm_surface_internal_get_user_data);
               SYM(tbm_lib, tbm_surface_internal_add_user_data);
               SYM(tbm_lib, tbm_surface_internal_set_user_data);
               SYM(tbm_lib, tbm_surface_queue_get_format);
               SYM(tbm_lib, tbm_surface_queue_reset);
               SYM(tbm_lib, tbm_surface_queue_flush);
               SYM(tbm_lib, tbm_surface_queue_free_flush);
               if (fail)
                  {
                     dlclose(tbm_lib);
                     tbm_lib = NULL;
                  }
               else break;
            }
      }
   if (!tbm_lib) return EINA_FALSE;

   for (i = 0; tbm_clients[i]; i++)
      {
         tbm_client_lib = dlopen(tbm_clients[i], RTLD_LOCAL | RTLD_LAZY);
         if (tbm_client_lib)
            {
               fail = 0;
               SYM(tbm_client_lib, wayland_tbm_client_get_wl_tbm);
               SYM(tbm_client_lib, wayland_tbm_client_create_buffer);
               SYM(tbm_client_lib, wayland_tbm_client_create_surface_queue);
               if (fail)
                  {
                     dlclose(tbm_client_lib);
                     tbm_client_lib = NULL;
                  }
               else break;
            }
      }
   if (!tbm_client_lib) return EINA_FALSE;

   tbm_ref++;
   return EINA_TRUE;
}

static void
tbm_shutdown(void)
{
   if (tbm_ref > 0)
      {
         tbm_ref--;

         if (tbm_ref == 0)
            {
               if (tbm_lib)
                  {
                     dlclose(tbm_lib);
                     tbm_lib = NULL;
                  }
               if (tbm_client_lib)
                  {
                     dlclose(tbm_client_lib);
                     tbm_client_lib = NULL;
                  }

               if (tzsurf)
                  {
                     tizen_surface_shm_destroy(tzsurf);
                     tzsurf = NULL;
                  }
            }
      }
}


static void
_evas_tbmbuf_buffer_unmap(Tbmbuf_Surface *surface)
{
   sym_tbm_surface_unmap(surface->tbm_surface);
}


static void
_evas_tbmbuf_surface_reconfigure(Surface *s, int dx EINA_UNUSED, int dy EINA_UNUSED, int w, int h, uint32_t flags EINA_UNUSED)
{

   Tbmbuf_Surface *surface;

   if (!s) return;

   surface = s->surf.tbm;

   if (!surface) return;

   if ((w >= surface->w) && (w <= surface->stride / 4) && (h == surface->h))
      {
         surface->w = w;
         return;
      }
   surface->resize = EINA_TRUE;
   surface->w = w;
   surface->h = h;
}

static void
_wait_free_buffer(Tbmbuf_Surface *surface)
{
   if (!surface) return;

   wl_display_dispatch_pending(surface->wl_display);
   if (sym_tbm_surface_queue_can_dequeue(surface->tbm_queue, 0))
      return;

   DBG("WAIT free buffer");

   while (!sym_tbm_surface_queue_can_dequeue(surface->tbm_queue, 0)) {
         wl_display_dispatch(surface->wl_display);
   }
}

static void
buffer_release(void *data, struct wl_buffer *buffer EINA_UNUSED)
{
   if (!data) return;
   void *tbm_queue = NULL;
   tbm_surface_h tbm_surface = data;
   tbm_surface_queue_error_e tsq_err = TBM_SURFACE_QUEUE_ERROR_NONE;

   sym_tbm_surface_internal_get_user_data(tbm_surface, KEY_WINDOW, (void **)&tbm_queue);
   if (tbm_queue)
     {
        tsq_err = sym_tbm_surface_queue_release(tbm_queue, tbm_surface);
        if (tsq_err != TBM_SURFACE_QUEUE_ERROR_ALREADY_EXIST)
          sym_tbm_surface_internal_unref(tbm_surface);
     }
   else
     sym_tbm_surface_internal_unref(tbm_surface);
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
_evas_tbmbuf_surface_assign(Surface *s)
{
   Tbmbuf_Surface *surface;
   Render_Engine_Swap_Mode mode = MODE_FULL;
   tbm_buffer_info *tbuf_info = NULL;
   if (!s) return 0;
   surface = s->surf.tbm;
   if (!surface)
      {
         ERR("surface is NULL");
         return 0;
      }

   if (surface->resize)
      {
         // reset tbm_surface
         int width, height, format;
         width = surface->w;
         height = surface->h;
         format = sym_tbm_surface_queue_get_format(surface->tbm_queue);
         sym_tbm_surface_queue_reset(surface->tbm_queue, width, height, format);
         surface->resize = EINA_FALSE;
      }

   surface->tbm_surface = NULL;
   tbm_surface_queue_error_e ret = TBM_SURFACE_QUEUE_ERROR_NONE;
   struct wl_buffer *buffer;

   _wait_free_buffer(surface);

   // check num of tbm surface
   int num_surface, i;
   tbm_surface_h surfaces[5];
   sym_tbm_surface_queue_get_surfaces(surface->tbm_queue, surfaces, &num_surface);

   ret = sym_tbm_surface_queue_dequeue(surface->tbm_queue, &surface->tbm_surface);

   if (ret != TBM_SURFACE_QUEUE_ERROR_NONE ||
         surface->tbm_surface == NULL) {
         ERR("dequeue:%p from queue:%p err:%d\n", surface->tbm_surface, surface->tbm_queue, ret);
         surface->wait_release = 1;
         return 0;
   }

   sym_tbm_surface_internal_ref(surface->tbm_surface);

   surface->wait_release = 0;
   if(!sym_tbm_surface_internal_get_user_data(surface->tbm_surface, KEY_WL_BUFFER, (void **)&buffer)) {
         buffer = sym_wayland_tbm_client_create_buffer(surface->tbm_client, surface->tbm_surface);
         if (!buffer)
           {
             ERR("wayland_tbm_client_create_buffer is NULL");
             sym_tbm_surface_internal_unref(surface->tbm_surface);
             sym_tbm_surface_queue_release(surface->tbm_queue, surface->tbm_surface);
             return 0;
           }

         wl_proxy_set_queue((struct wl_proxy *)buffer, NULL);

         sym_tbm_surface_internal_add_user_data(surface->tbm_surface, KEY_WL_BUFFER, (tbm_data_free)buffer_destroy);
         sym_tbm_surface_internal_set_user_data(surface->tbm_surface, KEY_WL_BUFFER, buffer);

         sym_tbm_surface_internal_add_user_data(surface->tbm_surface, KEY_WINDOW, NULL);
         sym_tbm_surface_internal_set_user_data(surface->tbm_surface, KEY_WINDOW, surface->tbm_queue);

         wl_buffer_add_listener(buffer, &buffer_listener, surface->tbm_surface);
   }


   tbm_surface_info_s info;
   sym_tbm_surface_map(surface->tbm_surface, TBM_SURF_OPTION_READ|TBM_SURF_OPTION_WRITE, &info);
   surface->tbm_info = info;

   // check buffer age
   if (!sym_tbm_surface_internal_get_user_data(surface->tbm_surface, KEY_SURFACE_INFO, (void**)&tbuf_info)) {
       tbuf_info = calloc(1, sizeof(tbm_buffer_info));
       if (tbuf_info) {
           tbuf_info->age = 0;
           tbuf_info->num_surface = num_surface;
         }
       sym_tbm_surface_internal_add_user_data(surface->tbm_surface, KEY_SURFACE_INFO, free);
       sym_tbm_surface_internal_set_user_data(surface->tbm_surface, KEY_SURFACE_INFO, tbuf_info);
   }

   //reset
   if (tbuf_info && (unsigned int)num_surface != tbuf_info->num_surface)
     {
       s->frame_age = 0;

       for(i=0; i<num_surface; i++) {
           sym_tbm_surface_internal_get_user_data(surfaces[i], KEY_SURFACE_INFO, (void **)&tbuf_info);
           if (tbuf_info) {
               tbuf_info->age = 0;
               tbuf_info->num_surface = num_surface;
           }
       }
     }

   if (!tbuf_info || !tbuf_info->age)
     return MODE_FULL;

   if (tbuf_info && tbuf_info->age < s->frame_age)
     {
       unsigned int diff;
       diff = s->frame_age - tbuf_info->age + 1;
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
_evas_tbmbuf_surface_data_get(Surface *s, int *w, int *h)
{
   Tbmbuf_Surface *surface;
   void *image;

   surface = s->surf.tbm;
   if (!surface)
      {
         ERR("surface is NULL");
         return 0;
      }

   image = surface->tbm_info.planes[0].ptr;

   surface->stride = surface->tbm_info.planes[0].stride;
   if (w) *w = surface->stride / 4;
   if (h) *h = surface->tbm_info.height;

   return image;
}

static void
_shm_wl_registry_global(void *data EINA_UNUSED, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version EINA_UNUSED)
{
   if (!strcmp(interface, "tizen_surface_shm"))
     tzsurf = wl_registry_bind(registry, name, &tizen_surface_shm_interface, 2);
}

static void
_shm_wl_registry_global_remove(void *data EINA_UNUSED, struct wl_registry *registry EINA_UNUSED, uint32_t name EINA_UNUSED)
{
}

static void
_shm_tzsurf_flusher_cb_flush(void *data, struct tizen_surface_shm_flusher *flusher EINA_UNUSED)
{
   Tbmbuf_Surface *surf = data;

   if (surf->tbm_queue)
     sym_tbm_surface_queue_flush(surf->tbm_queue);
}

static void
_shm_tzsurf_flusher_cb_free_flush(void *data, struct tizen_surface_shm_flusher *flusher EINA_UNUSED)
{
   Tbmbuf_Surface *surf = data;

   if (surf->tbm_queue)
     sym_tbm_surface_queue_free_flush(surf->tbm_queue);
}

static const struct tizen_surface_shm_flusher_listener _tzsurf_flusher_listener =
{
      _shm_tzsurf_flusher_cb_flush,
      _shm_tzsurf_flusher_cb_free_flush
};

static const struct wl_registry_listener _shm_wl_registry_listener =
{
   _shm_wl_registry_global,
   _shm_wl_registry_global_remove
};

static void
_shm_tzsurf_init(struct wl_display *disp)
{
   struct wl_event_queue *queue = NULL;
   struct wl_registry *registry = NULL;

   queue = wl_display_create_queue(disp);
   EINA_SAFETY_ON_NULL_GOTO(queue, err_registry);
   registry = wl_display_get_registry(disp);
   EINA_SAFETY_ON_NULL_GOTO(queue, err_registry);

   wl_proxy_set_queue((struct wl_proxy *)registry, queue);
   wl_registry_add_listener(registry, &_shm_wl_registry_listener, NULL);
   if ((wl_display_roundtrip_queue(disp, queue) < 0) || (!tzsurf))
     goto err_registry;

   /* use default queue */
   wl_proxy_set_queue((struct wl_proxy *)tzsurf, NULL);

err_registry:
   if (registry)
     wl_registry_destroy(registry);
   if (queue)
     wl_event_queue_destroy(queue);
   return;
}


static void
__cb_client_frame_callback(void *data EINA_UNUSED, struct wl_callback *callback,
                           uint32_t time EINA_UNUSED)
{
   wl_callback_destroy(callback);
}

static const struct wl_callback_listener frame_listener = {
      __cb_client_frame_callback
};


static void
_evas_tbmbuf_surface_post(Surface *s, Eina_Rectangle *rects, unsigned int count)
{
   Tbmbuf_Surface *surface;
   struct wl_callback *frame_callback = NULL;
   tbm_buffer_info *tbuf_info = NULL;
   if (!s) return;

   surface = s->surf.tbm;
   if (!surface->wl_surface) return;

   struct wl_buffer *buffer = NULL;

   _evas_tbmbuf_buffer_unmap(surface);

   sym_tbm_surface_internal_get_user_data(surface->tbm_surface, KEY_WL_BUFFER, (void **)&buffer);
   if (!buffer) {
         ERR("Enqueue:%p from queue:%p", surface->tbm_surface, surface->tbm_queue);
         return;
   }

   ++s->frame_age;
   sym_tbm_surface_internal_get_user_data(surface->tbm_surface, KEY_SURFACE_INFO, (void **)&tbuf_info);
   if (tbuf_info)
     tbuf_info->age = s->frame_age;


   wl_surface_attach(surface->wl_surface, buffer, 0, 0);
   _evas_surface_damage(surface->wl_surface, surface->compositor_version,
                        surface->w, surface->h, rects, count);

   frame_callback = wl_surface_frame(surface->wl_surface);
   wl_callback_add_listener(frame_callback, &frame_listener, surface->tbm_surface);

   sym_tbm_surface_internal_ref(surface->tbm_surface);
   wl_surface_commit(surface->wl_surface);

   sym_tbm_surface_internal_unref(surface->tbm_surface);
   sym_tbm_surface_queue_enqueue(surface->tbm_queue, surface->tbm_surface);

   sym_tbm_surface_queue_acquire(surface->tbm_queue, &surface->tbm_surface);

   return;
}

static void
_evas_tbmbuf_surface_destroy(Surface *s)
{
   Tbmbuf_Surface *surf = NULL;
   if (!s) return;

   surf = s->surf.tbm;
   if (surf)
      {
         if (surf->tbm_queue && tbm_queue_ref == 0)
            {
               if (surf->tzsurf_flusher)
                 {
                    tizen_surface_shm_flusher_destroy(surf->tzsurf_flusher);
                    surf->tzsurf_flusher = NULL;
                 }
               if (s->info) s->info->info.tzsurf_flusher = NULL;

               if (surf->tbm_surface)
                  sym_tbm_surface_internal_set_user_data(surf->tbm_surface, KEY_WINDOW, NULL);
               sym_tbm_surface_queue_destroy(surf->tbm_queue);
               if (s->info) s->info->info.tbm_queue = NULL;
            }
         if (tbm_queue_ref)
            --tbm_queue_ref;
         free(surf);
         s->surf.tbm = NULL;
      }
   tbm_shutdown();
}

Eina_Bool
_evas_tbmbuf_surface_create(Surface *s, int w, int h, int num_buff)
{
   Tbmbuf_Surface *surf = NULL;

   if (!tbm_init())
      {
         ERR("Could not initialize TBM!");
         goto err;
      }

   if (!(s->surf.tbm = calloc(1, sizeof(Tbmbuf_Surface)))) goto err;
   surf = s->surf.tbm;

   surf->dx = 0;
   surf->dy = 0;
   surf->w = w;
   surf->h = h;
   surf->wl_display = s->info->info.wl_disp;
   surf->wl_surface = s->info->info.wl_surface;
   if (!surf->wl_surface)  goto err;
   surf->alpha = s->info->info.destination_alpha;
   surf->compositor_version = 3;

   /* get tbm_client */
   surf->tbm_client = s->info->info.tbm_client;
   if (surf->tbm_client == NULL) {
         ERR("No wayland_tbm global");
         goto err;
   }

   /* check num_buff, not yet support single buffer */
   if (num_buff == 1) num_buff = 2;

   if (!tzsurf)
      _shm_tzsurf_init(surf->wl_display);


   /* create surface buffers */
   if (!s->info->info.tbm_queue)
      {
         s->info->info.tbm_queue = sym_wayland_tbm_client_create_surface_queue(surf->tbm_client,
                                                                       surf->wl_surface,
                                                                       num_buff,
                                                                       w, h,
                                                                       TBM_FORMAT_ARGB8888);
         surf->tbm_queue = s->info->info.tbm_queue;

         if (tzsurf && !s->info->info.tzsurf_flusher)
           {
             s->info->info.tzsurf_flusher = tizen_surface_shm_get_flusher(tzsurf, surf->wl_surface);
             surf->tzsurf_flusher = s->info->info.tzsurf_flusher;
             tizen_surface_shm_flusher_add_listener(surf->tzsurf_flusher, &_tzsurf_flusher_listener, surf);
           }

      }
   else
      {
         /* reuse tbm_queue */
         surf->tbm_queue = s->info->info.tbm_queue;
         sym_tbm_surface_queue_reset(surf->tbm_queue, w, h, TBM_FORMAT_ARGB8888);

         /* reuse tzsurf_flusher */
         surf->tzsurf_flusher = s->info->info.tzsurf_flusher;

         tbm_queue_ref++;
      }
   if (surf->tbm_queue == NULL) {
         ERR("surf->tbm_queue NULL");
         goto err;
   }
   surf->tbm_surface = NULL;

   s->type = SURFACE_TBM;
   s->funcs.destroy = _evas_tbmbuf_surface_destroy;
   s->funcs.reconfigure = _evas_tbmbuf_surface_reconfigure;
   s->funcs.data_get = _evas_tbmbuf_surface_data_get;
   s->funcs.assign = _evas_tbmbuf_surface_assign;
   s->funcs.post = _evas_tbmbuf_surface_post;



   return EINA_TRUE;

err:
   if (surf)
      {
         free(surf);
         s->surf.tbm = NULL;
      }
   return EINA_FALSE;
}
