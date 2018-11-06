#ifndef _ECORE_DRM_PRIVATE_H
# define _ECORE_DRM_PRIVATE_H

# ifdef HAVE_CONFIG_H
#  include "config.h"
# endif

# include "Ecore.h"
# include "ecore_private.h"
# include "Ecore_Input.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

# include <linux/vt.h>
# include <linux/kd.h>
# include <linux/major.h>
# include <linux/input.h>
# include <libinput.h>

# ifdef HAVE_SYSTEMD
#  include <systemd/sd-login.h>
# endif

# include <Eldbus.h>
# include <Ecore_Drm.h>

# undef TRACE_EFL_BEGIN
# undef TRACE_EFL_END
# undef TRACE_INPUT_BEGIN
# undef TRACE_INPUT_END
# ifdef ENABLE_TTRACE
#  include <ttrace.h>
#  define TRACE_EFL_BEGIN(NAME) traceBegin(TTRACE_TAG_EFL, "EFL:ECORE_DRM:"#NAME)
#  define TRACE_EFL_END() traceEnd(TTRACE_TAG_EFL)
#  define TRACE_INPUT_BEGIN(NAME) traceBegin(TTRACE_TAG_INPUT, "INPUT:ECORE_DRM:"#NAME)
#  define TRACE_INPUT_END() traceEnd(TTRACE_TAG_INPUT)
# else
#  define TRACE_EFL_BEGIN(NAME)
#  define TRACE_EFL_END()
#  define TRACE_INPUT_BEGIN(NAME)
#  define TRACE_INPUT_END()
# endif

# define NUM_FRAME_BUFFERS 2

# ifndef DRM_MAJOR
#  define DRM_MAJOR 226
# endif

# ifndef DRM_CAP_TIMESTAMP_MONOTONIC
#  define DRM_CAP_TIMESTAMP_MONOTONIC 0x6
# endif

# ifdef ECORE_DRM_DEFAULT_LOG_COLOR
#  undef ECORE_DRM_DEFAULT_LOG_COLOR
# endif
# define ECORE_DRM_DEFAULT_LOG_COLOR EINA_COLOR_BLUE

# define EVDEV_SEAT_POINTER (1 << 0)
# define EVDEV_SEAT_KEYBOARD (1 << 1)
# define EVDEV_SEAT_TOUCH (1 << 2)

# define ECORE_DRM_ENV_LIBINPUT_LOG_DISABLE "ECORE_DRM_LIBINPUT_LOG_DISABLE"
# define ECORE_DRM_ENV_LIBINPUT_LOG_EINA_LOG "ECORE_DRM_LIBINPUT_LOG_EINA_LOG"

# ifdef ERR
#  undef ERR
# endif
# ifdef DBG
#  undef DBG
# endif
# ifdef INF
#  undef INF
# endif
# ifdef WRN
#  undef WRN
# endif
# ifdef CRIT
#  undef CRIT
# endif

extern int _ecore_drm_log_dom;
struct xkb_keymap *cached_keymap;
struct xkb_context *cached_context;

# define EVDEV_MAX_SLOTS 32

# define ERR(...) EINA_LOG_DOM_ERR(_ecore_drm_log_dom, __VA_ARGS__)
# define DBG(...) EINA_LOG_DOM_DBG(_ecore_drm_log_dom, __VA_ARGS__)
# define INF(...) EINA_LOG_DOM_INFO(_ecore_drm_log_dom, __VA_ARGS__)
# define WRN(...) EINA_LOG_DOM_WARN(_ecore_drm_log_dom, __VA_ARGS__)
# define CRIT(...) EINA_LOG_DOM_CRIT(_ecore_drm_log_dom, __VA_ARGS__)

# define ALEN(array) (sizeof(array) / sizeof(array)[0])

typedef struct _Ecore_Drm_Pageflip_Callback
{
   Ecore_Drm_Device *dev;
   Ecore_Drm_Pageflip_Cb func;
   void *data;
   int count;
} Ecore_Drm_Pageflip_Callback;

typedef struct _Ecore_Drm_VBlank_Callback
{
   Ecore_Drm_Output *output;
   Ecore_Drm_VBlank_Cb func;
   void *data;
} Ecore_Drm_VBlank_Callback;

typedef enum _Ecore_Drm_Backlight_Type
{
   ECORE_DRM_BACKLIGHT_RAW,
   ECORE_DRM_BACKLIGHT_PLATFORM,
   ECORE_DRM_BACKLIGHT_FIRMWARE
} Ecore_Drm_Backlight_Type;

typedef struct _Ecore_Drm_Backlight
{
   const char *device;
   double brightness_max;
   double brightness_actual;
   double brightness;

   Ecore_Drm_Backlight_Type type;
} Ecore_Drm_Backlight;

struct _Ecore_Drm_Output
{
   Ecore_Drm_Device *dev;
   unsigned int crtc_id;
   unsigned int conn_id;
   unsigned int conn_type;
   drmModeCrtcPtr crtc;
   drmModePropertyPtr dpms;

   int x, y, phys_width, phys_height;

   int pipe;
   const char *make, *model, *name;
   unsigned int subpixel;
   uint16_t gamma;

   Ecore_Drm_Output_Mode *current_mode;
   Eina_List *modes;
   int dpms_level;

   unsigned char *edid_blob;

   struct
     {
        char eisa[13];
        char monitor[13];
        char pnp[5];
        char serial[13];
     } edid;

   Ecore_Drm_Backlight *backlight;   

   Eina_Bool primary : 1;
   Eina_Bool connected : 1;
   Eina_Bool enabled : 1;
   Eina_Bool cloned : 1;
   Eina_Bool need_repaint : 1;
   Eina_Bool repaint_scheduled : 1;
   Eina_Bool pending_destroy : 1;
   Eina_Bool pending_flip : 1;
   Eina_Bool pending_vblank : 1;

   /* this is ugly */
   unsigned int curr_fb_handle;
   int curr_fb_w;
   int curr_fb_h;
   int curr_fb_format;

   void *hal_output;

   int rotation;
};

struct _Ecore_Drm_Seat
{
//   struct libinput_seat *seat;
   const char *name;
   Ecore_Drm_Input *input;
   Eina_List *devices;
   struct
     {
        int ix, iy;
        double dx, dy;
        Eina_Bool swap;
        Eina_Bool invert_x;
        Eina_Bool invert_y;
     } ptr;
};

struct _Ecore_Drm_Input
{
   int fd;
   Ecore_Drm_Device *dev;
   struct libinput *libinput;

   Ecore_Fd_Handler *hdlr;

   Eina_Bool enabled : 1;
   Eina_Bool suspended : 1;
};

struct _Ecore_Drm_Evdev
{
   Ecore_Drm_Seat *seat;
   struct libinput_device *device;

   const char *path;
   int fd;

   int mt_slot;

   Ecore_Drm_Output *output;

   /* struct  */
   /*   { */
   /*      int min_x, min_y; */
   /*      int max_x, max_y; */
   /*      double rel_w, rel_h; */
   /*      struct */
   /*        { */
   /*          int x[2]; */
   /*          int y[2]; */
   /*          Eina_Bool down : 1; */
   /*        } pt[EVDEV_MAX_SLOTS]; */
   /*   } abs; */

   struct 
     {
        int minx, miny, maxw, maxh;
        double dx, dy;
        unsigned int last, prev;
        uint32_t threshold;
        Eina_Bool did_double : 1;
        Eina_Bool did_triple : 1;
        uint32_t prev_button, last_button;
     } mouse;

   struct 
     {
        struct xkb_keymap *keymap;
        struct xkb_state *state;
        xkb_mod_mask_t ctrl_mask;
        xkb_mod_mask_t alt_mask;
        xkb_mod_mask_t shift_mask;
        xkb_mod_mask_t win_mask;
        xkb_mod_mask_t scroll_mask;
        xkb_mod_mask_t num_mask;
        xkb_mod_mask_t caps_mask;
        xkb_mod_mask_t altgr_mask;
        unsigned int modifiers;
        unsigned int depressed, latched, locked, group;
     } xkb;

     Eina_Hash *key_remap_hash;
     Eina_Bool key_remap_enabled;

   /* Ecore_Drm_Evdev_Capabilities caps; */
   Ecore_Drm_Seat_Capabilities seat_caps;

   struct
     {
        struct
          {
             int rotation;
             int x, y;
             int w, h;
          } transform;
        struct
          {
             int x;
             int y;
          } coords[EVDEV_MAX_SLOTS];
        unsigned int pressed;
     } touch;
};

struct _Ecore_Drm_Sprite
{
   Ecore_Drm_Fb *current_fb, *next_fb;
   Ecore_Drm_Output *output;

   int drm_fd;

   unsigned int crtcs;
   unsigned int plane_id;

   struct 
     {
        int x, y;
        unsigned int w, h;
     } src, dest;

   unsigned int num_formats;
   unsigned int formats[];
};

typedef void (*Ecore_Drm_Open_Cb)(void *data, int fd, Eina_Bool b);

void _ecore_drm_event_activate_send(Eina_Bool active);

Eina_Bool _ecore_drm_launcher_device_open(const char *device, Ecore_Drm_Open_Cb callback, void *data, int flags);
int _ecore_drm_launcher_device_open_no_pending(const char *device, int flags);
void _ecore_drm_launcher_device_close(const char *device, int fd);
int _ecore_drm_launcher_device_flags_set(int fd, int flags);

Eina_Bool _ecore_drm_tty_switch(Ecore_Drm_Device *dev, int activate_vt);

#ifdef HAVE_TDM
void _ecore_drm_output_event_send(const Ecore_Drm_Output *output, Eina_Bool plug);

Eina_Bool _ecore_drm_display_init(Ecore_Drm_Device *dev);
void _ecore_drm_display_destroy(Ecore_Drm_Device *dev);
int _ecore_drm_display_get_fd(Ecore_Drm_Device *dev);
Ecore_Drm_Fb* _ecore_drm_display_fb_find(void *hal_buffer);
Ecore_Drm_Fb* _ecore_drm_display_fb_create(Ecore_Drm_Device *dev, int width, int height);
void _ecore_drm_display_fb_destroy(Ecore_Drm_Fb *fb);
void _ecore_drm_display_fb_set(Ecore_Drm_Device *dev, Ecore_Drm_Fb *fb);
void _ecore_drm_display_fb_send(Ecore_Drm_Device *dev, Ecore_Drm_Fb *fb, Ecore_Drm_Pageflip_Cb func, void *data);
Eina_Bool _ecore_drm_display_outputs_create(Ecore_Drm_Device *dev);
void _ecore_drm_display_output_free(Ecore_Drm_Output *output);
void _ecore_drm_display_outputs_update(Ecore_Drm_Device *dev);
void _ecore_drm_display_output_render_enable(Ecore_Drm_Output *output);
void _ecore_drm_display_output_render_disable(Ecore_Drm_Output *output);
void _ecore_drm_display_output_dpms_set(Ecore_Drm_Output *output, int level);
unsigned int _ecore_drm_display_output_crtc_buffer_get(Ecore_Drm_Output *output);
void _ecore_drm_display_output_size_get(Ecore_Drm_Device *dev, int id, int *w, int *h);
void _ecore_drm_display_output_crtc_size_get(Ecore_Drm_Output *output, int *width, int *height);
Eina_Bool _ecore_drm_display_output_mode_set(Ecore_Drm_Output *output, Ecore_Drm_Output_Mode *mode, int x, int y);
Eina_Bool _ecore_drm_display_output_possible_crtc_get(Ecore_Drm_Output *output, unsigned int crtc);
Eina_Bool _ecore_drm_display_output_wait_vblank(Ecore_Drm_Output *output, int interval, Ecore_Drm_VBlank_Cb func, void *data);
void* _ecore_drm_display_output_hal_private_get(Ecore_Drm_Output *output);
#endif

Ecore_Drm_Evdev *_ecore_drm_evdev_device_create(Ecore_Drm_Seat *seat, struct libinput_device *device);
void _ecore_drm_evdev_device_destroy(Ecore_Drm_Evdev *evdev);
Eina_Bool _ecore_drm_evdev_event_process(struct libinput_event *event);
void _ecore_drm_pointer_motion_post(Ecore_Drm_Evdev *evdev);

Ecore_Drm_Fb *_ecore_drm_fb_create(Ecore_Drm_Device *dev, int width, int height);
void _ecore_drm_fb_destroy(Ecore_Drm_Fb *fb);

void _ecore_drm_output_fb_release(Ecore_Drm_Output *output, Ecore_Drm_Fb *fb);
void _ecore_drm_output_repaint_start(Ecore_Drm_Output *output);
void _ecore_drm_output_frame_finish(Ecore_Drm_Output *output);
void _ecore_drm_outputs_update(Ecore_Drm_Device *dev);
void _ecore_drm_output_render_enable(Ecore_Drm_Output *output);
void _ecore_drm_output_render_disable(Ecore_Drm_Output *output);

Eina_Bool _ecore_drm_logind_connect(Ecore_Drm_Device *dev);
void _ecore_drm_logind_disconnect(Ecore_Drm_Device *dev);
void _ecore_drm_logind_restore(Ecore_Drm_Device *dev);
Eina_Bool _ecore_drm_logind_device_open(const char *device, Ecore_Drm_Open_Cb callback, void *data);
int _ecore_drm_logind_device_open_no_pending(const char *device);
void _ecore_drm_logind_device_close(const char *device);

int _ecore_drm_dbus_init(Ecore_Drm_Device *dev);
int _ecore_drm_dbus_shutdown(void);
int _ecore_drm_dbus_device_take(uint32_t major, uint32_t minor, Ecore_Drm_Open_Cb callback, void *data);
int _ecore_drm_dbus_device_take_no_pending(uint32_t major, uint32_t minor, Eina_Bool *paused_out, double timeout);
void _ecore_drm_dbus_device_release(uint32_t major, uint32_t minor);
Eina_Bool _ecore_drm_dbus_session_take(void);
Eina_Bool _ecore_drm_dbus_session_release(void);

void _ecore_drm_device_add(unsigned int window, Ecore_Drm_Evdev *edev);

void _ecore_drm_inputs_init(void);
void _ecore_drm_inputs_shutdown(void);

struct xkb_context *_ecore_drm_device_cached_context_get(enum xkb_context_flags flags);
struct xkb_keymap *_ecore_drm_device_cached_keymap_get(struct xkb_context *ctx,
                       const struct xkb_rule_names *names,
                       enum xkb_keymap_compile_flags flags);
#endif
