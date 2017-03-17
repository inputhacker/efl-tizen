#ifndef _ECORE_EVAS_WAYLAND_PRIVATE_H_
#define _ECORE_EVAS_WAYLAND_PRIVATE_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

//#define LOGFNS 1
#ifdef LOGFNS
# include <stdio.h>
# define LOGFN(fl, ln, fn) \
   printf("-ECORE_EVAS-WL: %25s: %5i - %s\n", fl, ln, fn);
#else
# define LOGFN(fl, ln, fn)
#endif

#include <Eina.h>
#include <Ecore.h>
#include "ecore_private.h"
#include <Ecore_Input.h>
#include <Ecore_Input_Evas.h>
#include <Ecore_Wayland.h>

#include <Ecore_Evas.h>
#include "ecore_evas_private.h"
#include "ecore_evas_wayland.h"

typedef struct _Ecore_Evas_Engine_Wl_Data Ecore_Evas_Engine_Wl_Data;

struct _Ecore_Evas_Engine_Wl_Data 
{
   Ecore_Wl_Window *parent, *win;
   Evas_Object *frame;
   int fx, fy, fw, fh;
#ifdef BUILD_ECORE_EVAS_WAYLAND_EGL
   struct wl_egl_window *egl_win;
#endif
   struct
     {
        unsigned char supported: 1;
        unsigned char request : 1;
        unsigned char done : 1;
        Ecore_Job    *manual_mode_job;
     } wm_rot;
   struct
     {
        unsigned char above : 1;
        unsigned char below : 1;
     } state;
   struct wl_callback *anim_callback;

   Ecore_Event_Handler *output_transform_hdl;
   short output_rotation;
};

Ecore_Evas_Interface_Wayland *_ecore_evas_wl_interface_new(void);

int  _ecore_evas_wl_common_init(void);
int  _ecore_evas_wl_common_shutdown(void);
void _ecore_evas_wl_common_pre_free(Ecore_Evas *ee);
void _ecore_evas_wl_common_free(Ecore_Evas *ee);
void _ecore_evas_wl_common_callback_resize_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_move_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_delete_request_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_focus_in_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_focus_out_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_mouse_in_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_callback_mouse_out_set(Ecore_Evas *ee, void (*func)(Ecore_Evas *ee));
void _ecore_evas_wl_common_move(Ecore_Evas *ee, int x, int y);
void _ecore_evas_wl_common_resize(Ecore_Evas *ee, int w, int h);
void _ecore_evas_wl_common_raise(Ecore_Evas *ee);
void _ecore_evas_wl_common_lower(Ecore_Evas *ee);
void _ecore_evas_wl_common_activate(Ecore_Evas *ee);
void _ecore_evas_wl_common_title_set(Ecore_Evas *ee, const char *title);
void _ecore_evas_wl_common_name_class_set(Ecore_Evas *ee, const char *n, const char *c);
void _ecore_evas_wl_common_size_min_set(Ecore_Evas *ee, int w, int h);
void _ecore_evas_wl_common_size_max_set(Ecore_Evas *ee, int w, int h);
void _ecore_evas_wl_common_size_base_set(Ecore_Evas *ee, int w, int h);
void _ecore_evas_wl_common_size_step_set(Ecore_Evas *ee, int w, int h);
void _ecore_evas_wl_common_aspect_set(Ecore_Evas *ee, double aspect);
void _ecore_evas_wl_common_object_cursor_set(Ecore_Evas *ee, Evas_Object *obj, int layer, int hot_x, int hot_y);
void _ecore_evas_wl_common_object_cursor_unset(Ecore_Evas *ee);
void _ecore_evas_wl_common_layer_set(Ecore_Evas *ee, int layer);
void _ecore_evas_wl_common_iconified_set(Ecore_Evas *ee, Eina_Bool on);
void _ecore_evas_wl_common_maximized_set(Ecore_Evas *ee, Eina_Bool on);
void _ecore_evas_wl_common_fullscreen_set(Ecore_Evas *ee, Eina_Bool on);
void _ecore_evas_wl_common_ignore_events_set(Ecore_Evas *ee, int ignore);
void _ecore_evas_wl_common_focus_skip_set(Ecore_Evas *ee, Eina_Bool on);
int  _ecore_evas_wl_common_pre_render(Ecore_Evas *ee);
/* int  _ecore_evas_wl_common_render_updates(Ecore_Evas *ee); */
void _ecore_evas_wl_common_post_render(Ecore_Evas *ee);
int  _ecore_evas_wl_common_render(Ecore_Evas *ee);
void _ecore_evas_wl_common_screen_geometry_get(const Ecore_Evas *ee, int *x, int *y, int *w, int *h);
void _ecore_evas_wl_common_screen_dpi_get(const Ecore_Evas *ee, int *xdpi, int *ydpi);
void _ecore_evas_wl_common_output_transform_register(Ecore_Evas *ee);
void _ecore_evas_wl_common_render_flush_pre(void *data, Evas *evas EINA_UNUSED, void *event);
void _ecore_evas_wl_common_render_flush_post(void *data, Evas *evas EINA_UNUSED, void *event);
void _ecore_evas_wl_common_render_updates(void *data, Evas *evas, void *event);
void _ecore_evas_wl_common_rotation_set(Ecore_Evas *ee, int rotation, int resize);
void _ecore_evas_wl_common_borderless_set(Ecore_Evas *ee, Eina_Bool on);
void _ecore_evas_wl_common_withdrawn_set(Ecore_Evas *ee, Eina_Bool on);

void _ecore_evas_wl_common_frame_callback_clean(Ecore_Evas *ee);

Evas_Object * _ecore_evas_wl_common_frame_add(Evas *evas);
void _ecore_evas_wl_common_frame_border_size_set(Evas_Object *obj, int fx, int fy, int fw, int fh);

void _ecore_evas_wl_common_pointer_xy_get(const Ecore_Evas *ee, Evas_Coord *x, Evas_Coord *y);
Eina_Bool _ecore_evas_wl_common_pointer_warp(const Ecore_Evas *ee, Evas_Coord x, Evas_Coord y);

void _ecore_evas_wl_common_wm_rot_preferred_rotation_set(Ecore_Evas *ee, int rot);
void _ecore_evas_wl_common_wm_rot_available_rotations_set(Ecore_Evas *ee, const int *rots, unsigned int count);
void _ecore_evas_wl_common_wm_rot_manual_rotation_done_set(Ecore_Evas *ee, Eina_Bool set);
void _ecore_evas_wl_common_wm_rot_manual_rotation_done(Ecore_Evas *ee);
void _ecore_evas_wl_common_wm_rot_cb_angle_changed(Ecore_Wl_Window *win, int rot, Eina_Bool resize, int w, int h, void *data);

int _ecore_evas_common_init_count_get(void);

#ifdef BUILD_ECORE_EVAS_WAYLAND_SHM
void _ecore_evas_wayland_shm_resize(Ecore_Evas *ee, int location);
void _ecore_evas_wayland_shm_resize_edge_set(Ecore_Evas *ee, int edge);
void _ecore_evas_wayland_shm_transparent_do(Ecore_Evas *ee, int transparent);
void _ecore_evas_wayland_shm_alpha_do(Ecore_Evas *ee, int transparent);
void _ecore_evas_wayland_shm_window_rotate(Ecore_Evas *ee, int rotation, int resize);
#endif

#ifdef BUILD_ECORE_EVAS_WAYLAND_EGL
void _ecore_evas_wayland_egl_resize(Ecore_Evas *ee, int location);
void _ecore_evas_wayland_egl_resize_edge_set(Ecore_Evas *ee, int edge);
void _ecore_evas_wayland_egl_window_rotate(Ecore_Evas *ee, int rotation, int resize);
#endif

#endif /* _ECORE_EVAS_WAYLAND_PRIVATE_H_ */
