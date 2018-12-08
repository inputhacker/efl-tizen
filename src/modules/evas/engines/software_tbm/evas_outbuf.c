#include "evas_engine.h"

#include <tbm_surface.h>
#include <tbm_surface_queue.h>
#include <tbm_surface_internal.h>

static const int key_evas_buffer;
#define KEY_EVAS_BUFFER ((unsigned long)&key_evas_buffer)

typedef struct {
   RGBA_Image *im;
   unsigned int age;
   tbm_surface_h tbm_surface;
}evas_buffer_info;

//#define ENG_DEBUG

static void
_tbm_surface_evas_buffer_info_free(void *data)
{
   evas_buffer_info* ebuf_info = data;
   evas_cache_image_drop(&ebuf_info->im->cache_entry);

   free(data);
}

static evas_buffer_info *
_tbm_surface_evas_buffer_info_get(tbm_surface_h surface, Eina_Bool *is_first)
{
   tbm_surface_info_s tbm_info;
   evas_buffer_info *ebuf_info = NULL;
   RGBA_Image *img = NULL;

   if(!tbm_surface_internal_get_user_data(surface, KEY_EVAS_BUFFER, (void**)&ebuf_info))
     {
        tbm_surface_internal_add_user_data(surface, KEY_EVAS_BUFFER,
                                       _tbm_surface_evas_buffer_info_free);
        tbm_surface_get_info(surface, &tbm_info);

        img = (RGBA_Image *)
           evas_cache_image_data(evas_common_image_cache_get(),
                                 tbm_info.planes[0].stride/4, tbm_info.height,
                                 (DATA32*)tbm_info.planes[0].ptr,
                                 1,
                                 EVAS_COLORSPACE_ARGB8888);

        if (!img)
          ERR("fail to evas_cache_image_data()");

        ebuf_info = calloc(1, sizeof(evas_buffer_info));
        if (!ebuf_info) return NULL;
        ebuf_info->im = img;
        ebuf_info->tbm_surface = surface;
        tbm_surface_internal_set_user_data(surface, KEY_EVAS_BUFFER, ebuf_info);

        if (is_first)
           *is_first = EINA_TRUE;
     }

   return ebuf_info;
}

static void
_tbm_surface_queue_reset_cb(tbm_surface_queue_h surface_queue EINA_UNUSED, void *data)
{
   Outbuf *ob = data;
   ob->priv.queue_reset = EINA_TRUE;
}

Outbuf *
evas_outbuf_setup(Evas_Engine_Info_Software_Tbm *info, int w, int h)
{
   Outbuf *ob;

   /* try to allocate space for outbuf */
   if (!(ob = calloc(1, sizeof(Outbuf)))) return NULL;

#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng setup %dx%d(%p) => %dx%d(%p)\n",
      ob->w, ob->h, ob->priv.tbm_queue,
      w,h,info->info.tbm_queue);
#endif

   /* set properties of outbuf */
   ob->w = w;
   ob->h = h;

   ob->info = info;
   ob->depth = info->info.depth;
   ob->rotation = info->info.rotation;

   if (!info->info.tbm_queue)
     {
        ob->priv.tbm_queue = tbm_surface_queue_create(3,
                                                      ob->w, ob->h,
                                                      TBM_FORMAT_ARGB8888,
                                                      TBM_BO_DEFAULT);
        if (!ob->priv.tbm_queue)
           ERR("Cannot create tbm_queue! w:%d, h%d", ob->w, ob->h);
        ob->priv.queue_need_destroy = EINA_TRUE;
        info->info.tbm_queue = ob->priv.tbm_queue;
     }
   else
     {
        ob->priv.tbm_queue = info->info.tbm_queue;
     }

   tbm_surface_queue_add_reset_cb(ob->priv.tbm_queue, _tbm_surface_queue_reset_cb, ob);

   return ob;
}

void
evas_outbuf_free(Outbuf *ob)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng free tbm_queue:%p need_destroy:%d\n",
            ob->priv.tbm_queue, ob->priv.queue_need_destroy);
#endif

   /* free allocate space for outbuf */
   if (ob->priv.tbm_queue && ob->priv.queue_need_destroy)
     {
        tbm_surface_queue_destroy(ob->priv.tbm_queue);
        ob->priv.tbm_queue = NULL;
        ob->priv.queue_need_destroy = EINA_FALSE;
     }

   free(ob);
}

void
evas_outbuf_reconfigure(Outbuf *ob, int w, int h, int rot EINA_UNUSED, Outbuf_Depth depth EINA_UNUSED)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng reconfig %dx%d(%p) => %dx%d\n",
      ob->w, ob->h, ob->priv.tbm_queue,
      w,h);
#endif

   if ((ob->w != w) || (ob->h != h) || (ob->rotation != rot))
     {
        ob->w = w;
        ob->h = h;
        ob->rotation = rot;
        //tbm_surface_queue_reset(ob->priv.tbm_queue, w, h, TBM_FORMAT_ARGB8888);
     }
}

Render_Output_Swap_Mode
evas_outbuf_swap_mode_get(Outbuf *ob)
{
#define USE_BUFFER_AGE   0

   tbm_surface_h surface = NULL;
   evas_buffer_info *ebuf_info;
   Eina_Bool is_first = EINA_FALSE;
   Render_Output_Swap_Mode mode = MODE_FULL;
   tbm_surface_info_s tbm_info;

   tbm_surface_queue_dequeue(ob->priv.tbm_queue, &surface);
   if (!surface)
      ERR("Fail to dequeue_buffer");

   ebuf_info = _tbm_surface_evas_buffer_info_get(surface, &is_first);
   if (!ebuf_info)
      ERR("Fail to get evas_buffer_info");

   ob->priv.ebuf_info = ebuf_info;
#if USE_BUFFER_AGE
   if (!ob->priv.frame_age || is_first == EINA_TRUE)
      mode = MODE_FULL;
   else
     {
        if (ebuf_info->age < ob->priv.frame_age)
          {
             unsigned int diff;
             diff = ob->priv.frame_age - ebuf_info->age;
             switch(diff)
             {
             case 0:
               mode = MODE_COPY;
               break;
             case 1:
               mode = MODE_DOUBLE;
               break;
             case 2:
               mode = MODE_TRIPLE;
               break;
             case 3:
               mode = MODE_QUADRUPLE;
               break;
             default:
               mode = MODE_FULL;
             }
          }
     }
#endif

   tbm_surface_map(surface, TBM_SURF_OPTION_READ|TBM_SURF_OPTION_WRITE, &tbm_info);
   tbm_surface_internal_ref(surface);

#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng swap_mod(%d) frm_age:%d buf_age:%d tbm_surf:%p\n",
      mode,
      ob->priv.frame_age, ebuf_info->age,
      surface);
#endif
   return mode;
}

Eina_Bool
evas_outbuf_update_region_first_rect(Outbuf *ob)
{
   Eina_Bool reset = ob->priv.queue_reset;

   ob->priv.queue_reset = EINA_FALSE;

#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng first_rect(%d)\n", reset);
#endif

   return reset;
}

void *
evas_outbuf_update_region_new(Outbuf *ob, int x, int y, int w, int h, int *cx, int *cy, int *cw, int *ch)
{
   evas_buffer_info *ebuf_info;

   if ((w <= 0) || (h <= 0)) return NULL;

   RECTS_CLIP_TO_RECT(x, y, w, h, 0, 0, ob->w, ob->h);

   ebuf_info = ob->priv.ebuf_info;
   if (!ebuf_info)
     {
        ERR("evas_outbuf_update_region_new ebuf_info is NULL");
        return NULL;
     }

   if (cx) *cx = x;
   if (cy) *cy = y;
   if (cw) *cw = w;
   if (ch) *ch = h;

#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng region_new %dx%d+%d+%d cx:%dx%d+%d+%d img:%p\n",
         x,y,w,h,
         *cx, *cy, *cw, *ch,
         ebuf_info->im);
#endif

   return (void*)ebuf_info->im;
}

void
evas_outbuf_update_region_push(Outbuf *ob EINA_UNUSED, RGBA_Image *update EINA_UNUSED, int x EINA_UNUSED, int y EINA_UNUSED, int w EINA_UNUSED, int h EINA_UNUSED)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng region_push img:%p %dx%d+%d+%d\n",
         update,
         x, y, w, h);
#endif
}

void
evas_outbuf_update_region_free(Outbuf *ob EINA_UNUSED, RGBA_Image *update EINA_UNUSED)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng region_free img:%p\n",
         update);
#endif
}

void
evas_outbuf_flush(Outbuf *ob, Tilebuf_Rect *surface_damage EINA_UNUSED, Tilebuf_Rect *buffer_damage EINA_UNUSED, Evas_Render_Mode render_mode EINA_UNUSED)
{
   evas_buffer_info *ebuf_info = ob->priv.ebuf_info;

   ob->priv.frame_age++;
   ebuf_info->age = ob->priv.frame_age;
   ob->priv.ebuf_info = NULL;

#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng flush frm_age:%d buf_age:%d tbm_surf:%p\n",
         ob->priv.frame_age, ebuf_info->age, ebuf_info->tbm_surface);
#endif

   tbm_surface_unmap(ebuf_info->tbm_surface);
   tbm_surface_queue_enqueue(ob->priv.tbm_queue, ebuf_info->tbm_surface);
   tbm_surface_internal_unref(ebuf_info->tbm_surface);
}

void
evas_output_idle_flush(Outbuf *ob EINA_UNUSED)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng idle_flush\n");
#endif
}

int
evas_outbuf_rot_get(Outbuf *ob)
{
#ifdef ENG_DEBUG
   fprintf(stderr,"[evas:sw_tbm] eng rot_get: %d\n", ob->rotation);
#endif

   return ob->rotation;
}
