#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

//@Tizen Feature

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <Ecore_Evas.h>
#include <Evas.h>
#include <Ecore.h>
#include "ecore_private.h"
#include <Ecore_Input.h>
#include <tbm_bufmgr.h>
#include <tbm_surface_queue.h>
#include <tbm_surface_internal.h>

#include "ecore_evas_private.h"
#include "ecore_evas_tbm.h"

#include <Evas_Engine_Software_Tbm.h>
#include <Evas_Engine_GL_Tbm.h>
#include <dlfcn.h>

#include "../../../evas/engines/gl_generic/Evas_Engine_GL_Shared.h"

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef DLL_EXPORT
#  define EAPI __declspec(dllexport)
# else
#  define EAPI
# endif /* ! DLL_EXPORT */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

typedef struct _Ecore_Evas_Engine_Tbm_Data Ecore_Evas_Engine_Tbm_Data;

struct _Ecore_Evas_Engine_Tbm_Data {
   void* tbm_queue;
   tbm_surface_h tbm_surf;
   Eina_Bool ext_tbm_queue;
   void  (*free_func) (void *data, void *tbm_queue);
   void *(*alloc_func) (void *data, int w, int h);
   Evas_Object *image;
   void *data;
};

Ecore_Event_Handler *_ecore_evas_tbm_ecore_cb_handlers[2] = {0, };

static Eina_Bool
_ecore_evas_tbm_strcmp(const char *dst, const char *src)
{
   int dst_len, src_len, str_len;

   dst_len = strlen(dst);
   src_len = strlen(src);

   if (src_len > dst_len) str_len = src_len;
   else str_len = dst_len;

   if (!strncmp(dst, src, str_len))
     return EINA_TRUE;
   else
     return EINA_FALSE;
}

static Eina_Bool
_ecore_evas_tbm_evas_device_find(Evas *evas, const char *identifier)
{
   Eina_List *list, *l;
   Evas_Device *device;
   char *evas_device_description;

   EINA_SAFETY_ON_NULL_RETURN_VAL(evas, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(identifier, EINA_FALSE);

   list = (Eina_List *)evas_device_list(evas, NULL);
   EINA_LIST_FOREACH(list, l, device)
     {
        evas_device_description = (char *)evas_device_description_get(device);
        if (!evas_device_description) continue;

        if (_ecore_evas_tbm_strcmp(evas_device_description, identifier))
          {
             return EINA_TRUE;
          }
     }
   return EINA_FALSE;
}

static Evas_Device *
_ecore_evas_tbm_default_seat_get(Evas *evas)
{
   Eina_List *list, *l;
   Evas_Device *device;
   char *evas_device_description;

   EINA_SAFETY_ON_NULL_RETURN_VAL(evas, EINA_FALSE);

   list = (Eina_List *)evas_device_list(evas, NULL);
   EINA_LIST_FOREACH(list, l, device)
     {
        evas_device_description = evas_device_description_get(device);
        if (!evas_device_description) continue;
        if ((evas_device_class_get(device) == EVAS_DEVICE_CLASS_SEAT) &&
            !strncmp(evas_device_description, "Enlightenment seat", sizeof("Enlightenment seat")))
          {
             return device;
          }
     }
   return NULL;
}

static Eina_Bool
_ecore_evas_tbm_cb_ecore_device_add(void *data, int type EINA_UNUSED, void *event)
{
   Ecore_Event_Device_Info *ev;
   Ecore_Evas *ee = (Ecore_Evas *)data;
   Evas_Device *seat;

   ev = event;

   if (ee->prop.window != ev->window) return ECORE_CALLBACK_PASS_ON;
   if (_ecore_evas_tbm_evas_device_find(ee->evas, ev->identifier)) return ECORE_CALLBACK_PASS_ON;

   seat = _ecore_evas_tbm_default_seat_get(ee->evas);

   evas_device_add_full(ee->evas, ev->name,
                        ev->identifier,
                        seat, NULL,
                        ev->clas,
                        ev->subclas);

   return ECORE_CALLBACK_PASS_ON;
}


static Eina_Bool
_ecore_evas_tbm_cb_ecore_device_del(void *data, int type EINA_UNUSED, void *event)
{
   Ecore_Event_Device_Info *ev;
   Ecore_Evas *ee = (Ecore_Evas *)data;
   Eina_List *list, *l, *l_next;
   Evas_Device *device;
   char *evas_device_description;

   ev = event;

   if (ee->prop.window != ev->window) return ECORE_CALLBACK_PASS_ON;

   list = (Eina_List *)evas_device_list(ee->evas, NULL);
   EINA_LIST_FOREACH_SAFE(list, l, l_next, device)
     {
        evas_device_description = evas_device_description_get(device);
        if (!evas_device_description) continue;
        if (!strncmp(evas_device_description, ev->identifier, strlen(ev->identifier)))
          {
             evas_device_del(device);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static void
_ecore_evas_tbm_event_register(Ecore_Evas *ee)
{
   if (!ecore_event_init()) return;

   _ecore_evas_tbm_ecore_cb_handlers[0] = ecore_event_handler_add(ECORE_EVENT_DEVICE_ADD,
                                          _ecore_evas_tbm_cb_ecore_device_add, ee);
   _ecore_evas_tbm_ecore_cb_handlers[1] = ecore_event_handler_add(ECORE_EVENT_DEVICE_DEL,
                                          _ecore_evas_tbm_cb_ecore_device_del, ee);
}

static void
_ecore_evas_tbm_event_unregister(void)
{
   int i;

   if (_ecore_evas_tbm_ecore_cb_handlers[0]) ecore_event_shutdown();

   for (i = 0; i < 2; i++)
     {
        ecore_event_handler_del(_ecore_evas_tbm_ecore_cb_handlers[i]);
        _ecore_evas_tbm_ecore_cb_handlers[i] = NULL;
     }
}

static void
_ecore_evas_tbm_free(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data = ee->engine.data;

   if (tbm_data->tbm_queue && tbm_data->free_func)
     tbm_data->free_func(tbm_data->data,tbm_data->tbm_queue);
   free(tbm_data);

   _ecore_evas_tbm_event_unregister();
}

static void
_ecore_evas_tbm_callback_resize_set(Ecore_Evas *ee, Ecore_Evas_Event_Cb func)
{
   ee->func.fn_resize = func;
}

static void
_ecore_evas_tbm_callback_move_set(Ecore_Evas *ee, Ecore_Evas_Event_Cb func)
{
   ee->func.fn_move = func;
}

static void
_ecore_evas_tbm_move(Ecore_Evas *ee, int x, int y)
{
   ee->req.x = x;
   ee->req.y = y;
   if ((ee->x == x) && (ee->y == y)) return;
   ee->x = x;
   ee->y = y;
   if (ee->func.fn_move) ee->func.fn_move(ee);
}

static void
_ecore_evas_tbm_resize(Ecore_Evas *ee, int w, int h)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data = ee->engine.data;

   if (w < 1) w = 1;
   if (h < 1) h = 1;
   ee->req.w = w;
   ee->req.h = h;
   if ((w == ee->w) && (h == ee->h)) return;
   ee->w = w;
   ee->h = h;
   evas_output_size_set(ee->evas, ee->w, ee->h);
   evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
   evas_damage_rectangle_add(ee->evas, 0, 0, ee->w, ee->h);

   if (tbm_data->tbm_queue)
     tbm_data->free_func(tbm_data->data,tbm_data->tbm_queue);

   tbm_data->tbm_queue = tbm_data->alloc_func(tbm_data->data, ee->w, ee->h);
   if (ee->func.fn_resize) ee->func.fn_resize(ee);
}

static void
_ecore_evas_tbm_move_resize(Ecore_Evas *ee, int x, int y, int w, int h)
{
   if ((ee->x != x) || (ee->y != y))
     _ecore_evas_tbm_move(ee, x, y);
   if ((ee->w != w) || (ee->h != h))
     _ecore_evas_tbm_resize(ee, w, h);
}

static void
_mouse_move_dispatch(Ecore_Evas *ee)
{
   Ecore_Evas_Cursor *cursor;
   Eina_Iterator *itr = eina_hash_iterator_data_new(ee->prop.cursors);

   EINA_SAFETY_ON_NULL_RETURN(itr);

   EINA_ITERATOR_FOREACH(itr, cursor)
     _ecore_evas_mouse_move_process(ee, cursor->pos_x, cursor->pos_y,
                                    ecore_loop_time_get());
   eina_iterator_free(itr);
}

static void
_ecore_evas_tbm_rotation_set(Ecore_Evas *ee, int rotation, int resize)
{
   int rot_dif;

   if (ee->rotation == rotation) return;

   /* calculate difference in rotation */
   rot_dif = ee->rotation - rotation;
   if (rot_dif < 0) rot_dif = -rot_dif;

   /* check if rotation is just a flip */
   if (rot_dif % 180)
     {
        int minw, minh, maxw, maxh;
        int basew, baseh, stepw, steph;

        /* check if we are rotating with resize */
        if (!resize)
          {
             int fw, fh;
             int ww, hh;

             /* grab framespace width & height */
             evas_output_framespace_get(ee->evas, NULL, NULL, &fw, &fh);

             /* check for fullscreen */
             if (!ee->prop.fullscreen && !ee->prop.maximized)
               {

               }
             else
               {
                  /* resize the canvas based on rotation */
                  if ((rotation == 0) || (rotation == 180))
                    {
                       /* resize the canvas */
                       evas_output_size_set(ee->evas, ee->req.w, ee->req.h);
                       evas_output_viewport_set(ee->evas, 0, 0,
                                                ee->req.w, ee->req.h);
                    }
                  else
                    {
                       /* resize the canvas */
                       evas_output_size_set(ee->evas, ee->req.h, ee->req.w);
                       evas_output_viewport_set(ee->evas, 0, 0,
                                                ee->req.h, ee->req.w);
                    }
               }

             /* add canvas damage */
             if (ECORE_EVAS_PORTRAIT(ee))
               evas_damage_rectangle_add(ee->evas, 0, 0, ee->req.w, ee->req.h);
             else
               evas_damage_rectangle_add(ee->evas, 0, 0, ee->req.h, ee->req.w);
             ww = ee->h;
             hh = ee->w;
             ee->w = ww;
             ee->h = hh;
             ee->req.w = ww;
             ee->req.h = hh;
          }
        else
          {
             /* resize the canvas based on rotation */
             if ((rotation == 0) || (rotation == 180))
               {
                  evas_output_size_set(ee->evas, ee->w, ee->h);
                  evas_output_viewport_set(ee->evas, 0, 0, ee->w, ee->h);
               }
             else
               {
                  evas_output_size_set(ee->evas, ee->h, ee->w);
                  evas_output_viewport_set(ee->evas, 0, 0, ee->h, ee->w);
               }

             /* call the ecore_evas' resize function */
             if (ee->func.fn_resize) ee->func.fn_resize(ee);

             /* add canvas damage */
             if (ECORE_EVAS_PORTRAIT(ee))
               evas_damage_rectangle_add(ee->evas, 0, 0, ee->w, ee->h);
             else
               evas_damage_rectangle_add(ee->evas, 0, 0, ee->h, ee->w);
          }

        /* get min, max, base, & step sizes */
        ecore_evas_size_min_get(ee, &minw, &minh);
        ecore_evas_size_max_get(ee, &maxw, &maxh);
        ecore_evas_size_base_get(ee, &basew, &baseh);
        ecore_evas_size_step_get(ee, &stepw, &steph);

        /* record the current rotation of the ecore_evas */
        ee->rotation = rotation;

        /* reset min, max, base, & step sizes */
        ecore_evas_size_min_set(ee, minh, minw);
        ecore_evas_size_max_set(ee, maxh, maxw);
        ecore_evas_size_base_set(ee, baseh, basew);
        ecore_evas_size_step_set(ee, steph, stepw);

        /* send a mouse_move process
         *
         * NB: Is This Really Needed ?
         * Yes, it's required to update the mouse position, relatively to
         * widgets. After a rotation change, e.g., the mouse might not be over
         * a button anymore. */
        _mouse_move_dispatch(ee);
     }
   else
     {
        /* record the current rotation of the ecore_evas */
        ee->rotation = rotation;

        /* send a mouse_move process
         *
         * NB: Is This Really Needed ? Yes, it's required to update the mouse
         * position, relatively to widgets. */
        _mouse_move_dispatch(ee);

        /* call the ecore_evas' resize function */
        if (ee->func.fn_resize) ee->func.fn_resize(ee);

        /* add canvas damage */
        if (ECORE_EVAS_PORTRAIT(ee))
          evas_damage_rectangle_add(ee->evas, 0, 0, ee->w, ee->h);
        else
          evas_damage_rectangle_add(ee->evas, 0, 0, ee->h, ee->w);
     }

   if (!strcmp(ee->driver, "software_tbm"))
     {
        Evas_Engine_Info_Software_Tbm *einfo;
        einfo = (Evas_Engine_Info_Software_Tbm *)evas_engine_info_get(ee->evas);
        if (!einfo) return;
        einfo->info.rotation = rotation;
        if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
          ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
     }
   else if (!strcmp(ee->driver, "gl_tbm"))
     {
        Evas_Engine_Info_GL_Tbm *einfo;
        einfo = (Evas_Engine_Info_GL_Tbm *)evas_engine_info_get(ee->evas);
        if (!einfo) return;
        einfo->info.rotation = rotation;
        einfo->callback.evas = ee->evas;
        if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
          ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
     }

   if (ee->func.fn_state_change) ee->func.fn_state_change(ee);
}

static void
_ecore_evas_tbm_show(Ecore_Evas *ee)
{
   if (ee->prop.focused) return;
   ee->prop.focused = EINA_TRUE;
   ee->prop.withdrawn = EINA_FALSE;
   if (ee->func.fn_state_change) ee->func.fn_state_change(ee);
   evas_focus_in(ee->evas);
   if (ee->func.fn_focus_in) ee->func.fn_focus_in(ee);
}

static int
_ecore_evas_tbm_render(Ecore_Evas *ee)
{
   Eina_List *updates = NULL, *ll;
   Ecore_Evas_Engine_Tbm_Data *tbm_data;
   Ecore_Evas *ee2;
   int rend = 0;

   tbm_data = ee->engine.data;
   EINA_LIST_FOREACH(ee->sub_ecore_evas, ll, ee2)
     {
        if (ee2->func.fn_pre_render) ee2->func.fn_pre_render(ee2);
        if (ee2->engine.func->fn_render)
           rend |= ee2->engine.func->fn_render(ee2);
        if (ee2->func.fn_post_render) ee2->func.fn_post_render(ee2);
     }
   if (tbm_data->tbm_queue)
     {
        updates = evas_render_updates(ee->evas);
     }
   if (updates)
     {
        evas_render_updates_free(updates);
        _ecore_evas_idle_timeout_update(ee);
     }

   if (ee->func.fn_post_render) ee->func.fn_post_render(ee);
   return updates ? 1 : rend;
}

EAPI int
ecore_evas_tbm_render(Ecore_Evas *ee)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, 0);
   return _ecore_evas_tbm_render(ee);
}

#if 0
/* TODO : */
// NOTE: if you fix this, consider fixing ecore_evas_ews.c as it is similar!
static void
_ecore_evas_tbm_coord_translate(Ecore_Evas *ee, Evas_Coord *x, Evas_Coord *y)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data = ee->engine.data;
   Evas_Coord xx, yy, ww, hh, fx, fy, fw, fh;

   evas_object_geometry_get(tbm_data->image, &xx, &yy, &ww, &hh);
   evas_object_image_fill_get(tbm_data->image, &fx, &fy, &fw, &fh);

   if (fw < 1) fw = 1;
   if (fh < 1) fh = 1;

   if (evas_object_map_get(tbm_data->image) &&
       evas_object_map_enable_get(tbm_data->image))
     {
        fx = 0; fy = 0;
        fw = ee->w; fh = ee->h;
        ww = ee->w; hh = ee->h;
     }

   if ((fx == 0) && (fy == 0) && (fw == ww) && (fh == hh))
     {
        *x = (ee->w * (*x - xx)) / fw;
        *y = (ee->h * (*y - yy)) / fh;
     }
   else
     {
        xx = (*x - xx) - fx;
        while (xx < 0) xx += fw;
        while (xx > fw) xx -= fw;
        *x = (ee->w * xx) / fw;

        yy = (*y - yy) - fy;
        while (yy < 0) yy += fh;
        while (yy > fh) yy -= fh;
        *y = (ee->h * yy) / fh;
     }
}

static void
_ecore_evas_tbm_transfer_modifiers_locks(Evas *e, Evas *e2)
{
   const char *mods[] =
     { "Shift", "Control", "Alt", "Meta", "Hyper", "Super", NULL };
   const char *locks[] =
     { "Scroll_Lock", "Num_Lock", "Caps_Lock", NULL };
   int i;

   for (i = 0; mods[i]; i++)
     {
        if (evas_key_modifier_is_set(evas_key_modifier_get(e), mods[i]))
          evas_key_modifier_on(e2, mods[i]);
        else
          evas_key_modifier_off(e2, mods[i]);
     }
   for (i = 0; locks[i]; i++)
     {
        if (evas_key_lock_is_set(evas_key_lock_get(e), locks[i]))
          evas_key_lock_on(e2, locks[i]);
        else
          evas_key_lock_off(e2, locks[i]);
     }
}

static void
_ecore_evas_tbm_cb_mouse_in(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_In *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_mouse_in(ee->evas, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_mouse_out(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_Out *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_mouse_out(ee->evas, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_mouse_down(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_Down *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_mouse_down(ee->evas, ev->button, ev->flags, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_mouse_up(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_Up *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_mouse_up(ee->evas, ev->button, ev->flags, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_mouse_move(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_Move *ev;
   Evas_Coord x, y;

   ee = data;
   ev = event_info;
   x = ev->cur.canvas.x;
   y = ev->cur.canvas.y;
   _ecore_evas_tbm_coord_translate(ee, &x, &y);
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   _ecore_evas_mouse_move_process(ee, x, y, ev->timestamp);
}

static void
_ecore_evas_tbm_cb_mouse_wheel(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Mouse_Wheel *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_mouse_wheel(ee->evas, ev->direction, ev->z, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_multi_down(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Multi_Down *ev;
   Evas_Coord x, y, xx, yy;
   double xf, yf;

   ee = data;
   ev = event_info;
   x = ev->canvas.x;
   y = ev->canvas.y;
   xx = x;
   yy = y;
   _ecore_evas_tbm_coord_translate(ee, &x, &y);
   xf = (ev->canvas.xsub - (double)xx) + (double)x;
   yf = (ev->canvas.ysub - (double)yy) + (double)y;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_multi_down(ee->evas, ev->device, x, y, ev->radius, ev->radius_x, ev->radius_y, ev->pressure, ev->angle, xf, yf, ev->flags, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_multi_up(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Multi_Up *ev;
   Evas_Coord x, y, xx, yy;
   double xf, yf;

   ee = data;
   ev = event_info;
   x = ev->canvas.x;
   y = ev->canvas.y;
   xx = x;
   yy = y;
   _ecore_evas_tbm_coord_translate(ee, &x, &y);
   xf = (ev->canvas.xsub - (double)xx) + (double)x;
   yf = (ev->canvas.ysub - (double)yy) + (double)y;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_multi_up(ee->evas, ev->device, x, y, ev->radius, ev->radius_x, ev->radius_y, ev->pressure, ev->angle, xf, yf, ev->flags, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_multi_move(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Multi_Move *ev;
   Evas_Coord x, y, xx, yy;
   double xf, yf;

   ee = data;
   ev = event_info;
   x = ev->cur.canvas.x;
   y = ev->cur.canvas.y;
   xx = x;
   yy = y;
   _ecore_evas_tbm_coord_translate(ee, &x, &y);
   xf = (ev->cur.canvas.xsub - (double)xx) + (double)x;
   yf = (ev->cur.canvas.ysub - (double)yy) + (double)y;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_multi_move(ee->evas, ev->device, x, y, ev->radius, ev->radius_x, ev->radius_y, ev->pressure, ev->angle, xf, yf, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_free(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;

   ee = data;
   if (ee->driver) _ecore_evas_free(ee);
}

static void
_ecore_evas_tbm_cb_key_down(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Key_Down *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_key_down(ee->evas, ev->keyname, ev->key, ev->string, ev->compose, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_key_up(void *data, Evas *e, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Ecore_Evas *ee;
   Evas_Event_Key_Up *ev;

   ee = data;
   ev = event_info;
   _ecore_evas_tbm_transfer_modifiers_locks(e, ee->evas);
   evas_event_feed_key_up(ee->evas, ev->keyname, ev->key, ev->string, ev->compose, ev->timestamp, NULL);
}

static void
_ecore_evas_tbm_cb_focus_in(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;

   ee = data;
   ee->prop.focused = EINA_TRUE;
   evas_focus_in(ee->evas);
   if (ee->func.fn_focus_in) ee->func.fn_focus_in(ee);
}

static void
_ecore_evas_tbm_cb_focus_out(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;

   ee = data;
   ee->prop.focused = EINA_FALSE;
   evas_focus_out(ee->evas);
   if (ee->func.fn_focus_out) ee->func.fn_focus_out(ee);
}

static void
_ecore_evas_tbm_cb_show(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;

   ee = data;
   ee->prop.withdrawn = EINA_FALSE;
   if (ee->func.fn_state_change) ee->func.fn_state_change(ee);
   ee->visible = 1;
   if (ee->func.fn_show) ee->func.fn_show(ee);
}

static void
_ecore_evas_tbm_cb_hide(void *data, Evas *e EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   Ecore_Evas *ee;

   ee = data;
   ee->prop.withdrawn = EINA_TRUE;
   if (ee->func.fn_state_change) ee->func.fn_state_change(ee);
   ee->visible = 0;
   if (ee->func.fn_hide) ee->func.fn_hide(ee);
}
#endif

static void
_ecore_evas_tbm_alpha_set(Ecore_Evas *ee, int alpha)
{
   if (((ee->alpha) && (alpha)) || ((!ee->alpha) && (!alpha))) return;
   ee->alpha = alpha;
}

static void
_ecore_evas_tbm_profile_set(Ecore_Evas *ee, const char *profile)
{
   _ecore_evas_window_profile_free(ee);
   ee->prop.profile.name = NULL;

   if (profile)
     {
        ee->prop.profile.name = (char *)eina_stringshare_add(profile);

        /* just change ee's state.*/
        if (ee->func.fn_state_change)
          ee->func.fn_state_change(ee);
     }
}

static void
_ecore_evas_tbm_msg_parent_send(Ecore_Evas *ee, int msg_domain, int msg_id, void *data, int size)
{
   Ecore_Evas *parent_ee = NULL;
   parent_ee = ecore_evas_data_get(ee, "parent");

   if (parent_ee)
     {
        if (parent_ee->func.fn_msg_parent_handle)
          parent_ee ->func.fn_msg_parent_handle(parent_ee, msg_domain, msg_id, data, size);
     }
   else
     {
        if (ee->func.fn_msg_parent_handle)
          ee ->func.fn_msg_parent_handle(ee, msg_domain, msg_id, data, size);
     }
}

static void
_ecore_evas_tbm_msg_send(Ecore_Evas *ee, int msg_domain, int msg_id, void *data, int size)
{
   Ecore_Evas *child_ee = NULL;
   child_ee = ecore_evas_data_get(ee, "child");

   if (child_ee)
     {
        if (child_ee->func.fn_msg_handle)
          child_ee->func.fn_msg_handle(child_ee, msg_domain, msg_id, data, size);
     }
   else
     {
        if (ee->func.fn_msg_handle)
          ee->func.fn_msg_handle(ee, msg_domain, msg_id, data, size);
     }
}

static Ecore_Evas_Interface_Tbm *
_ecore_evas_tbm_interface_new(void)
{
   Ecore_Evas_Interface_Tbm *iface;

   iface = calloc(1, sizeof(Ecore_Evas_Interface_Tbm));
   if (!iface) return NULL;

   iface->base.name = "tbm";
   iface->base.version = 1;

   return iface;
}

static Ecore_Evas_Engine_Func _ecore_tbm_engine_func =
{
     _ecore_evas_tbm_free,
     _ecore_evas_tbm_callback_resize_set,
     _ecore_evas_tbm_callback_move_set,
     NULL, // fn_callback_show_set
     NULL, // fn_callback_hide_set
     NULL, // fn_callback_delete_request_set
     NULL, // fn_callback_destroy_set
     NULL, // fn_callback_focus_in_set
     NULL, // fn_callback_focus_out_set
     NULL, // fn_callback_mouse_in_set
     NULL, // fn_callback_mouse_out_set
     NULL, // fn_callback_sticky_set
     NULL, // fn_callback_unsticky_set
     NULL, // fn_callback_pre_render_set
     NULL, // fn_callback_post_render_set
     _ecore_evas_tbm_move,
     NULL, // fn_managed_move
     _ecore_evas_tbm_resize,
     _ecore_evas_tbm_move_resize,
     _ecore_evas_tbm_rotation_set,
     NULL, // fn_shaped_set
     _ecore_evas_tbm_show,
     NULL, // fn_hide
     NULL, // fn_raise
     NULL, // fn_lower
     NULL, // fn_activate
     NULL, // fn_title_set
     NULL, // fn_name_class_set
     NULL, // fn_size_min_set
     NULL, // fn_size_max_set
     NULL, // fn_size_base_set
     NULL, // fn_size_step_set
     NULL, // fn_object_cursor_set
     NULL, // fn_object_cursor_unset
     NULL, // fn_layer_set
     NULL, // fn_focus_set
     NULL, // fn_iconified_set
     NULL, // fn_borderless_set
     NULL, // fn_override_set
     NULL, // fn_maximized_set
     NULL, // fn_fullscreen_set
     NULL, // fn_avoid_damage_set
     NULL, // fn_withdrawn_set
     NULL, // fn_sticky_set
     NULL, // fn_ignore_events_set
     _ecore_evas_tbm_alpha_set,
     NULL, //transparent
     NULL, // profiles_set
     _ecore_evas_tbm_profile_set,

     NULL, // fn_window_group_set
     NULL, // fn_aspect_set
     NULL, // fn_urgent_set
     NULL, // fn_modal_set
     NULL, // fn_demands_attention_set
     NULL, // fn_focus_skip_set

     _ecore_evas_tbm_render,
     NULL, // screen_geometry_get
     NULL,  // screen_dpi_get
     _ecore_evas_tbm_msg_parent_send,
     _ecore_evas_tbm_msg_send,

     NULL, // pointer_xy_get
     NULL, // pointer_warp

     NULL, // wm_rot_preferred_rotation_set
     NULL, // wm_rot_available_rotations_set
     NULL, // wm_rot_manual_rotation_done_set
     NULL, // wm_rot_manual_rotation_done

     NULL, // aux_hints_set

     NULL, // fn_animator_register
     NULL, // fn_animator_unregister

     NULL, // fn_evas_changed
     NULL, //fn_focus_device_set
     NULL, //fn_callback_focus_device_in_set
     NULL, //fn_callback_focus_device_out_set
     NULL, //fn_callback_device_mouse_in_set
     NULL, //fn_callback_device_mouse_out_set
     NULL, //fn_pointer_device_xy_get
     NULL, //fn_prepare
     NULL, //fn_last_tick_get
     //TIZEN_ONLY(20171218) : Add to free evas engine rsc before free evas
     NULL, //fn_evas_engine_rsc_free
     //TIZEN_ONLY(20180817) : Add to get current serial number
     NULL, //fn_serial_get
};

static void *
_ecore_evas_tbm_queue_alloc(void *data EINA_UNUSED, int w, int h)
{
   return (void *)tbm_surface_queue_create(3, w, h, TBM_FORMAT_ARGB8888, TBM_BO_DEFAULT);
}

static void
_ecore_evas_tbm_queue_free(void *data EINA_UNUSED, void *tbm_queue)
{
    tbm_surface_queue_destroy(tbm_queue);
}

EAPI Ecore_Evas *
ecore_evas_tbm_ext_new_internal(const char *engine, void *tbm_surf_queue, void* data)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;
   Ecore_Evas_Interface_Tbm *iface;
   Ecore_Evas *ee;
   const char *driver_name;
   int rmethod;
   int w, h;

    EINA_SAFETY_ON_NULL_RETURN_VAL(tbm_surf_queue, NULL);

   if (!strcmp(engine, "gl_tbm"))
      {
         driver_name = "gl_tbm";
      }
   else if (!strcmp(engine, "software_tbm"))
      {
         driver_name = "software_tbm";
      }
   else
      {
         ERR("engine name is NULL!!");
         return NULL;
      }

   rmethod = evas_render_method_lookup(driver_name);
    EINA_SAFETY_ON_TRUE_RETURN_VAL(rmethod == 0, NULL);

    ee = calloc(1, sizeof(Ecore_Evas));
    EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

    tbm_data = calloc(1, sizeof(Ecore_Evas_Engine_Tbm_Data));
    if (!tbm_data)
      {
        free(ee);
        return NULL;
      }


    ECORE_MAGIC_SET(ee, ECORE_MAGIC_EVAS);

    ee->engine.func = (Ecore_Evas_Engine_Func *)&_ecore_tbm_engine_func;
    ee->engine.data = tbm_data;
    tbm_data->alloc_func = NULL;
    tbm_data->free_func = NULL;
    tbm_data->data = (void *)data;
    tbm_data->tbm_queue = tbm_surf_queue;
    tbm_data->ext_tbm_queue = EINA_TRUE;

    ee->driver = driver_name;

    iface = _ecore_evas_tbm_interface_new();
    ee->engine.ifaces = eina_list_append(ee->engine.ifaces, iface);

    w = tbm_surface_queue_get_width(tbm_data->tbm_queue);
    h = tbm_surface_queue_get_height(tbm_data->tbm_queue);

    if (w < 1) w = 1;
    if (h < 1) h = 1;
    ee->rotation = 0;
    ee->visible = 1;
    ee->w = w;
    ee->h = h;
    ee->req.w = ee->w;
    ee->req.h = ee->h;
    ee->profile_supported = 1;

    ee->prop.max.w = 0;
    ee->prop.max.h = 0;
    ee->prop.layer = 0;
    ee->prop.focused = EINA_TRUE;
    ee->prop.borderless = EINA_TRUE;
    ee->prop.override = EINA_TRUE;
    ee->prop.maximized = EINA_TRUE;
    ee->prop.fullscreen = EINA_FALSE;
    ee->prop.withdrawn = EINA_FALSE;
    ee->prop.sticky = EINA_FALSE;
    ee->prop.window = (Ecore_Window)tbm_data->tbm_queue;

    /* init evas here */
    ee->evas = evas_new();
    evas_data_attach_set(ee->evas, ee);
    evas_output_method_set(ee->evas, rmethod);
    evas_output_size_set(ee->evas, w, h);
    evas_output_viewport_set(ee->evas, 0, 0, w, h);

    if (!strcmp(driver_name, "gl_tbm"))
      {
         Evas_Engine_Info_GL_Tbm *einfo = (Evas_Engine_Info_GL_Tbm *)evas_engine_info_get(ee->evas);
         if (einfo)
            {
               einfo->info.tbm_queue = tbm_data->tbm_queue;
               einfo->info.destination_alpha = EINA_TRUE;
               einfo->info.ext_tbm_queue = EINA_FALSE;
               einfo->info.rotation = 0;
               einfo->info.depth = 32;
               einfo->callback.evas = ee->evas;
               if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
                  {
                     ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
                     ecore_evas_free(ee);
                     return NULL;
                  }
            }
         else
            {
               ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
               ecore_evas_free(ee);
               return NULL;
            }
      }
   else
      {
         Evas_Engine_Info_Software_Tbm *einfo = (Evas_Engine_Info_Software_Tbm *)evas_engine_info_get(ee->evas);
         if (einfo)
            {
               einfo->info.tbm_queue = tbm_data->tbm_queue;
               einfo->info.destination_alpha = EINA_TRUE;
               einfo->info.ext_tbm_queue = EINA_FALSE;
               einfo->info.rotation = 0;
               einfo->info.depth = 32;
               if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
                  {
                     ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
                     ecore_evas_free(ee);
                     return NULL;
                  }
            }
         else
            {
               ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
               ecore_evas_free(ee);
               return NULL;
            }
      }

    evas_key_modifier_add(ee->evas, "Shift");
    evas_key_modifier_add(ee->evas, "Control");
    evas_key_modifier_add(ee->evas, "Alt");
    evas_key_modifier_add(ee->evas, "Meta");
    evas_key_modifier_add(ee->evas, "Hyper");
    evas_key_modifier_add(ee->evas, "Super");
    evas_key_lock_add(ee->evas, "Caps_Lock");
    evas_key_lock_add(ee->evas, "Num_Lock");
    evas_key_lock_add(ee->evas, "Scroll_Lock");

    _ecore_evas_register(ee);

    _ecore_evas_tbm_event_register(ee);

    return ee;
}

EAPI Ecore_Evas *
ecore_evas_tbm_allocfunc_new_internal(const char *engine, int w, int h,
                             void *(*alloc_func) (void *data, int w, int h),
                             void (*free_func) (void *data, void *tbm_queue),
                             const void *data)
{

   Ecore_Evas_Engine_Tbm_Data *tbm_data;
   Ecore_Evas_Interface_Tbm *iface;
   Ecore_Evas *ee;
   int rmethod;
   const char *driver_name;

   EINA_SAFETY_ON_NULL_RETURN_VAL(alloc_func, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(free_func, NULL);

   if (!strcmp(engine, "gl_tbm"))
     {
        driver_name = "gl_tbm";
     }
   else if (!strcmp(engine, "software_tbm"))
     {
        driver_name = "software_tbm";
     }
   else
     {
        ERR("engine name is NULL!!");
        return NULL;
     }

   rmethod = evas_render_method_lookup(driver_name);
   EINA_SAFETY_ON_TRUE_RETURN_VAL(rmethod == 0, NULL);

   ee = calloc(1, sizeof(Ecore_Evas));
   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

   tbm_data = calloc(1, sizeof(Ecore_Evas_Engine_Tbm_Data));
   if (!tbm_data)
     {
       free(ee);
       return NULL;
     }

   ECORE_MAGIC_SET(ee, ECORE_MAGIC_EVAS);

   ee->engine.func = (Ecore_Evas_Engine_Func *)&_ecore_tbm_engine_func;
   ee->engine.data = tbm_data;
   tbm_data->alloc_func = alloc_func;
   tbm_data->free_func = free_func;
   tbm_data->data = (void *)data;
   tbm_data->ext_tbm_queue = EINA_FALSE;

   ee->driver = driver_name;

   iface = _ecore_evas_tbm_interface_new();
   ee->engine.ifaces = eina_list_append(ee->engine.ifaces, iface);

   if (w < 1) w = 1;
   if (h < 1) h = 1;
   ee->rotation = 0;
   ee->visible = 1;
   ee->w = w;
   ee->h = h;
   ee->req.w = ee->w;
   ee->req.h = ee->h;
   ee->profile_supported = 1;

   ee->prop.max.w = 0;
   ee->prop.max.h = 0;
   ee->prop.layer = 0;
   ee->prop.focused = EINA_TRUE;
   ee->prop.borderless = EINA_TRUE;
   ee->prop.override = EINA_TRUE;
   ee->prop.maximized = EINA_TRUE;
   ee->prop.fullscreen = EINA_FALSE;
   ee->prop.withdrawn = EINA_FALSE;
   ee->prop.sticky = EINA_FALSE;

   /* init evas here */
   ee->evas = evas_new();
   evas_data_attach_set(ee->evas, ee);
   evas_output_method_set(ee->evas, rmethod);
   evas_output_size_set(ee->evas, w, h);
   evas_output_viewport_set(ee->evas, 0, 0, w, h);
   tbm_data->tbm_queue = tbm_data->alloc_func(tbm_data->data, w, h);

   if (!strcmp(driver_name, "gl_tbm"))
      {
         Evas_Engine_Info_GL_Tbm *einfo = (Evas_Engine_Info_GL_Tbm *)evas_engine_info_get(ee->evas);
         if (einfo)
           {
              einfo->info.tbm_queue = tbm_data->tbm_queue;
              einfo->info.destination_alpha = EINA_TRUE;
              einfo->info.ext_tbm_queue = EINA_FALSE;
              einfo->info.rotation = 0;
              einfo->info.depth = 32;
              einfo->callback.evas = ee->evas;
              if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
                {
                   ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
                   ecore_evas_free(ee);
                   return NULL;
                }
           }
         else
           {
              ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
              ecore_evas_free(ee);
              return NULL;
           }
      }
   else
      {
         Evas_Engine_Info_Software_Tbm *einfo = (Evas_Engine_Info_Software_Tbm *)evas_engine_info_get(ee->evas);
         if (einfo)
           {
              einfo->info.tbm_queue = tbm_data->tbm_queue;
              einfo->info.destination_alpha = EINA_TRUE;
              einfo->info.ext_tbm_queue = EINA_FALSE;
              einfo->info.rotation = 0;
              einfo->info.depth = 32;
              if (!evas_engine_info_set(ee->evas, (Evas_Engine_Info *)einfo))
                {
                   ERR("evas_engine_info_set() for engine '%s' failed.", ee->driver);
                   ecore_evas_free(ee);
                   return NULL;
                }
           }
         else
           {
              ERR("evas_engine_info_set() init engine '%s' failed.", ee->driver);
              ecore_evas_free(ee);
              return NULL;
           }
      }

   evas_key_modifier_add(ee->evas, "Shift");
   evas_key_modifier_add(ee->evas, "Control");
   evas_key_modifier_add(ee->evas, "Alt");
   evas_key_modifier_add(ee->evas, "Meta");
   evas_key_modifier_add(ee->evas, "Hyper");
   evas_key_modifier_add(ee->evas, "Super");
   evas_key_lock_add(ee->evas, "Caps_Lock");
   evas_key_lock_add(ee->evas, "Num_Lock");
   evas_key_lock_add(ee->evas, "Scroll_Lock");

   evas_event_feed_mouse_in(ee->evas, 0, NULL);

   _ecore_evas_register(ee);

   evas_event_feed_mouse_in(ee->evas, (unsigned int)((unsigned long long)(ecore_time_get() * 1000.0) & 0xffffffff), NULL);

   _ecore_evas_tbm_event_register(ee);

   return ee;

}

EAPI Ecore_Evas *
ecore_evas_gl_tbm_new_internal(int w, int h)
{
    return ecore_evas_tbm_allocfunc_new_internal
     ("gl_tbm", w, h,_ecore_evas_tbm_queue_alloc, _ecore_evas_tbm_queue_free, NULL);
}

EAPI Ecore_Evas *
ecore_evas_software_tbm_new_internal(int w, int h)
{
    return ecore_evas_tbm_allocfunc_new_internal
     ("software_tbm", w, h,_ecore_evas_tbm_queue_alloc, _ecore_evas_tbm_queue_free, NULL);
}

EAPI Ecore_Evas *
ecore_evas_tbm_ecore_evas_parent_get(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

   tbm_data = ee->engine.data;
   return evas_object_data_get(tbm_data->image, "Ecore_Evas_Parent");
}

EAPI const void *
ecore_evas_tbm_pixels_acquire_internal(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;
   tbm_surface_info_s surf_info;
   void *pixels=NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

   tbm_data = ee->engine.data;
   if (tbm_surface_queue_can_acquire(tbm_data->tbm_queue, 1)) {
      tbm_surface_queue_acquire(tbm_data->tbm_queue, &(tbm_data->tbm_surf));
      tbm_surface_internal_ref(tbm_data->tbm_surf);
      tbm_surface_map(tbm_data->tbm_surf, TBM_SURF_OPTION_READ|TBM_SURF_OPTION_WRITE, &surf_info);
      pixels = surf_info.planes[0].ptr;
   }
   ee->engine.data = tbm_data;
   return pixels;
}

EAPI void
ecore_evas_tbm_pixels_release_internal(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;

   EINA_SAFETY_ON_NULL_RETURN(ee);

   tbm_data = ee->engine.data;
   if (tbm_data->tbm_surf) {
      tbm_surface_unmap(tbm_data->tbm_surf);
      tbm_surface_internal_unref(tbm_data->tbm_surf);
      tbm_surface_queue_release(tbm_data->tbm_queue,tbm_data->tbm_surf);
      tbm_data->tbm_surf= NULL;
   }
   ee->engine.data = tbm_data;
}

EAPI void *
ecore_evas_tbm_surface_get(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

   tbm_data = ee->engine.data;

   return &tbm_data->tbm_surf;
}

EAPI int
ecore_evas_tbm_stride_get(Ecore_Evas *ee)
{
   Ecore_Evas_Engine_Tbm_Data *tbm_data;
   tbm_surface_info_s info;

   EINA_SAFETY_ON_NULL_RETURN_VAL(ee, NULL);

   tbm_data = ee->engine.data;
   if (tbm_data->tbm_surf)
   {
     tbm_surface_get_info(tbm_data->tbm_surf, &info);
     return info.planes[0].stride;
   }
   return 0;
}
