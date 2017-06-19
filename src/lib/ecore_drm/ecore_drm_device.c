#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "ecore_drm_private.h"
#include <dlfcn.h>

#ifdef HAVE_TDM
#include <tdm_helper.h>
#endif

#define INSIDE(x, y, xx, yy, ww, hh) \
   (((x) < ((xx) + (ww))) && ((y) < ((yy) + (hh))) && \
       ((x) >= (xx)) && ((y) >= (yy)))

static Eina_List *drm_devices;

#ifndef HAVE_TDM
static int flip_count = 0;

static void 
_ecore_drm_device_cb_page_flip(int fd EINA_UNUSED, unsigned int frame EINA_UNUSED, unsigned int sec EINA_UNUSED, unsigned int usec EINA_UNUSED, void *data)
{
   Ecore_Drm_Pageflip_Callback *cb;

   /* DBG("Drm Page Flip Event"); */

   TRACE_EFL_BEGIN(DEVICE PAGEFLIP CB);

   if (!(cb = data))
     {
        TRACE_EFL_END();
        return;
     }

   flip_count++;
   if (flip_count < cb->count)
     {
        TRACE_EFL_END();
        return;
     }

   cb->dev->current = cb->dev->next;
   cb->dev->next = NULL;

   flip_count = 0;
   if (cb->func) cb->func(cb->data);
   free(cb);

   TRACE_EFL_END();

   /* Ecore_Drm_Output *output; */

   /* DBG("Drm Page Flip Event"); */

   /* if (!(output = data)) return; */

   /* if (output->pending_flip) */
   /*   { */
   /*      if (output->dev->current) */
   /*        ecore_drm_output_fb_release(output, output->dev->current); */
   /*      output->dev->current = output->dev->next; */
   /*      output->dev->next = NULL; */
   /*   } */

   /* output->pending_flip = EINA_FALSE; */
   /* if (output->pending_destroy) */
   /*   { */
   /*      output->pending_destroy = EINA_FALSE; */
   /*      ecore_drm_output_free(output); */
   /*   } */
   /* else if (!output->pending_vblank) */
   /*   ecore_drm_output_repaint(output); */
}

static void 
_ecore_drm_device_cb_vblank(int fd EINA_UNUSED, unsigned int frame EINA_UNUSED, unsigned int sec EINA_UNUSED, unsigned int usec EINA_UNUSED, void *data)
{
   Ecore_Drm_VBlank_Callback *cb;

   /* DBG("Drm VBlank Event"); */

   if (!(cb = data)) return;
   if (cb->func) cb->func(cb->data);
   free(cb);
}

static Eina_Bool
_ecore_drm_device_cb_event(void *data, Ecore_Fd_Handler *hdlr EINA_UNUSED)
{
   Ecore_Drm_Device *dev;
   int ret;

   if (!(dev = data)) return ECORE_CALLBACK_RENEW;

   ret = drmHandleEvent(dev->drm.fd, &dev->drm_ctx);
   if (ret < 0)
     ERR("drmHandleEvent Failed: %m");

   return ECORE_CALLBACK_RENEW;
}
#endif

#if 0
static Eina_Bool 
_ecore_drm_device_cb_idle(void *data)
{
   Ecore_Drm_Device *dev;
   Ecore_Drm_Output *output;
   Eina_List *l;

   if (!(dev = data)) return ECORE_CALLBACK_CANCEL;

   if (!dev->active) return ECORE_CALLBACK_RENEW;

   EINA_LIST_FOREACH(dev->outputs, l, output)
     {
        if ((!output->enabled) || (!output->need_repaint)) continue;
        if (output->repaint_scheduled) continue;
        _ecore_drm_output_repaint_start(output);
     }

   return ECORE_CALLBACK_RENEW;
}
#endif
#ifndef HAVE_TDM
static void
_ecore_drm_device_cb_output_event(const char *device EINA_UNUSED, Eeze_Udev_Event event EINA_UNUSED, void *data, Eeze_Udev_Watch *watch EINA_UNUSED)
{
   Ecore_Drm_Device *dev;

   if (!(dev = data)) return;
   _ecore_drm_outputs_update(dev);
}
#endif
struct xkb_context *
_ecore_drm_device_cached_context_get(enum xkb_context_flags flags)
{
   if (!cached_context)
     return xkb_context_new(flags);
   else
     return xkb_context_ref(cached_context);
}

struct xkb_keymap *
_ecore_drm_device_cached_keymap_get(struct xkb_context *ctx,
                       const struct xkb_rule_names *names,
                       enum xkb_keymap_compile_flags flags)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(ctx, NULL);

   if (!cached_keymap)
     return xkb_map_new_from_names(ctx, names, flags);
   else
     return xkb_map_ref(cached_keymap);
}

void
_ecore_drm_device_cached_context_update(struct xkb_context *ctx)
{
   Eina_List *l;
   Ecore_Drm_Device *dev;

   EINA_LIST_FOREACH(drm_devices, l, dev)
     {
        xkb_context_unref(dev->xkb_ctx);
        dev->xkb_ctx = xkb_context_ref(ctx);
     }
}

void
_ecore_drm_device_cached_keymap_update(struct xkb_keymap *map)
{
   Eina_List *l, *l2, *l3;
   Ecore_Drm_Device *dev;
   Ecore_Drm_Seat *seat;
   Ecore_Drm_Evdev *edev;

   EINA_LIST_FOREACH(drm_devices, l, dev)
     EINA_LIST_FOREACH(dev->seats, l2, seat)
       EINA_LIST_FOREACH(ecore_drm_seat_evdev_list_get(seat), l3, edev)
         {
            xkb_keymap_unref(edev->xkb.keymap);
            edev->xkb.keymap = xkb_keymap_ref(map);
            xkb_state_unref(edev->xkb.state);
            edev->xkb.state = xkb_state_new(map);
         }
}

/**
 * @defgroup Ecore_Drm_Device_Group Device manipulation functions
 * 
 * Functions that deal with finding, opening, closing, and otherwise using 
 * the DRM device itself.
 */

EAPI Ecore_Drm_Device *
ecore_drm_device_find(const char *name, const char *seat)
{
   Ecore_Drm_Device *dev = NULL;
   Eina_Bool found = EINA_FALSE;
   Eina_Bool platform = EINA_FALSE;
   Eina_List *devs, *l;
   const char *device;

   /* try to get a list of drm devics */
   if (!(devs = eeze_udev_find_by_type(EEZE_UDEV_TYPE_DRM, name)))
#ifdef HAVE_TDM
     {
        if (!getenv("ECORE_DRM_DEVICE_DUMMY"))
          return NULL;

        if ((dev = calloc(1, sizeof(Ecore_Drm_Device))))
          {
             dev->drm.name = eina_stringshare_add("dummy");
             dev->drm.path = eina_stringshare_add("dummy");
             dev->id = 0;
             dev->seat = eina_stringshare_add("seat0");
             dev->vt = 0;
             dev->format = 0;
             dev->use_hw_accel = EINA_FALSE;
             dev->session = NULL;
             dev->window = -1;

             ERR("Using Drm Device: %s", dev->drm.name);

             drm_devices = eina_list_append(drm_devices, dev);
          }

        return dev;
     }
#else
     return NULL;
#endif

   DBG("Find Drm Device: %s", name);

   EINA_LIST_FOREACH(devs, l, device)
     {
        const char *devpath;
        const char *devseat;
        const char *devparent;

        if (!(devpath = eeze_udev_syspath_get_devpath(device)))
          continue;

        DBG("Found Drm Device");
        DBG("\tDevice: %s", device);
        DBG("\tDevpath: %s", devpath);

        if ((name) && (strcmp(name, devpath))) goto cont;

        if (!(devseat = eeze_udev_syspath_get_property(device, "ID_SEAT")))
          devseat = eina_stringshare_add("seat0");

        if ((seat) && (strcmp(seat, devseat))) 
          goto cont;
        else if (strcmp(devseat, "seat0"))
          goto cont;

        devparent = eeze_udev_syspath_get_parent_filtered(device, "pci", NULL);
        if (!devparent)
          {
             devparent = 
               eeze_udev_syspath_get_parent_filtered(device, "platform", NULL);
             platform = EINA_TRUE;
          }

        if (devparent)
          {
             if (!platform)
               {
                  const char *id;

                  if ((id = eeze_udev_syspath_get_sysattr(devparent, "boot_vga")))
                    {
                       if (!strcmp(id, "1")) found = EINA_TRUE;
                       eina_stringshare_del(id);
                    }
               }
             else
               found = EINA_TRUE;

             eina_stringshare_del(devparent);
          }

cont:
        eina_stringshare_del(devpath);
        if (found) break;
     }

   if (!found) goto out;

   if ((dev = calloc(1, sizeof(Ecore_Drm_Device))))
     {
        dev->drm.name = eeze_udev_syspath_get_devpath(device);
        dev->drm.path = eina_stringshare_add(device);

        dev->id = eeze_udev_syspath_get_sysnum(device);

        dev->seat = eeze_udev_syspath_get_property(device, "ID_SEAT");
        if (!dev->seat) dev->seat = eina_stringshare_add("seat0");

        dev->vt = 0;
        dev->format = 0;
        dev->use_hw_accel = EINA_FALSE;
        dev->session = NULL;
        dev->window = -1;

        DBG("Using Drm Device: %s", dev->drm.name);

        drm_devices = eina_list_append(drm_devices, dev);
     }

out:
   EINA_LIST_FREE(devs, device)
     eina_stringshare_del(device);

   return dev;
}

EAPI void 
ecore_drm_device_free(Ecore_Drm_Device *dev)
{
   Ecore_Drm_Output *output;
   unsigned int i = 0;

   /* check for valid device */
   if (!dev) return;

   for (; i < ALEN(dev->dumb); i++)
     {
        if (dev->dumb[i]) ecore_drm_fb_destroy(dev->dumb[i]);
        dev->dumb[i] = NULL;
     }

   ecore_drm_inputs_destroy(dev);

   /* free outputs */
   EINA_LIST_FREE(dev->outputs, output)
     ecore_drm_output_free(output);

   /* free crtcs */
   if (dev->crtcs) free(dev->crtcs);

   /* free device name */
   if (dev->drm.name) eina_stringshare_del(dev->drm.name);

   /* free device path */
   if (dev->drm.path) eina_stringshare_del(dev->drm.path);

   /* free device seat */
   if (dev->seat) eina_stringshare_del(dev->seat);

   /* free session */
   free(dev->session);

   drm_devices = eina_list_remove(drm_devices, dev);

   /* free structure */
   free(dev);
}

EAPI Eina_Bool 
ecore_drm_device_open(Ecore_Drm_Device *dev)
{
#ifdef HAVE_TDM
   /*int events = 0;*/

   /* check for valid device */
   if ((!dev) || (!dev->drm.name)) return EINA_FALSE;

   TRACE_EFL_BEGIN(Device_Open);

   dev->drm.fd = -1;
   if (!_ecore_drm_display_init(dev))
     {
        TRACE_EFL_END();
        return EINA_FALSE;
     }

   /* TDM backend will set a drm master fd to TDM_DRM_MASTER_FD. */
   dev->drm.fd = tdm_helper_get_fd("TDM_DRM_MASTER_FD");
   if (dev->drm.fd < 0)
     {
        ERR("failed: no drm master fd");

        /* If not set, use a tdm display fd instead of a drm master fd. */
        dev->drm.fd = _ecore_drm_display_get_fd(dev);
     }
   else
     {
        char *hal_device_name = drmGetDeviceNameFromFd(dev->drm.fd);

        if (hal_device_name)
          {
             if (dev->drm.name)
               eina_stringshare_del(dev->drm.name);

             dev->drm.name = eina_stringshare_add(hal_device_name);
             free(hal_device_name);
          }
     }

   /* try to create xkb context */
   if (!(dev->xkb_ctx = _ecore_drm_device_cached_context_get(0)))
     {
        ERR("Failed to create xkb context: %m");
        TRACE_EFL_END();
        return EINA_FALSE;
     }
#if 0
   events = (EEZE_UDEV_EVENT_ADD | EEZE_UDEV_EVENT_REMOVE |
             EEZE_UDEV_EVENT_CHANGE);

   dev->watch =
     eeze_udev_watch_add(EEZE_UDEV_TYPE_DRM, events,
                         _ecore_drm_device_cb_output_event, dev);
#endif
   TRACE_EFL_END();

   return EINA_TRUE;
#else
   uint64_t caps;
   int events = 0;
   drmVersionPtr ver;

   /* check for valid device */
   if ((!dev) || (!dev->drm.name)) return EINA_FALSE;

   /* DRM device node is needed immediately to keep going. */
   dev->drm.fd =
     _ecore_drm_launcher_device_open_no_pending(dev->drm.name, O_RDWR);
   if (dev->drm.fd < 0) return EINA_FALSE;

   DBG("Opened Device %s : %d", dev->drm.name, dev->drm.fd);

   ver = drmGetVersion(dev->drm.fd);
   if (ver)
     {
        DBG("\tDriver Name: %s", ver->name);
        DBG("\tDriver Date: %s", ver->date);
        DBG("\tDriver Description: %s", ver->desc);
        DBG("\tDriver Version: %d.%d.%d",
            ver->version_major, ver->version_minor,
            ver->version_patchlevel);
        drmFreeVersion(ver);
     }

   /* set client capabilities to 'universal planes' so drm core will expose
    * the full universal plane list (including primary & cursor planes) */
   drmSetClientCap(dev->drm.fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

   if (!drmGetCap(dev->drm.fd, DRM_CAP_TIMESTAMP_MONOTONIC, &caps))
     {
        if (caps == 1)
          dev->drm.clock = CLOCK_MONOTONIC;
        else
          dev->drm.clock = CLOCK_REALTIME;
     }
   else
     {
        ERR("Could not get TIMESTAMP_MONOTONIC device capabilities: %m");
     }

   /* Without DUMB_BUFFER we can't do software rendering on DRM. Fail without it
    * until we have rock solid hardware accelerated DRM on all drivers */
   if (drmGetCap(dev->drm.fd, DRM_CAP_DUMB_BUFFER, &caps) < 0 || !caps)
     {
        ERR("Could not get DUMB_BUFFER device capabilities: %m");
        return EINA_FALSE;
     }

   /* try to create xkb context */
   if (!(dev->xkb_ctx = _ecore_drm_device_cached_context_get(0)))
     {
        ERR("Failed to create xkb context: %m");
        return EINA_FALSE;
     }

   memset(&dev->drm_ctx, 0, sizeof(dev->drm_ctx));
   dev->drm_ctx.version = DRM_EVENT_CONTEXT_VERSION;
   dev->drm_ctx.page_flip_handler = _ecore_drm_device_cb_page_flip;
   dev->drm_ctx.vblank_handler = _ecore_drm_device_cb_vblank;

   events = (EEZE_UDEV_EVENT_ADD | EEZE_UDEV_EVENT_REMOVE |
             EEZE_UDEV_EVENT_CHANGE);

   dev->watch =
     eeze_udev_watch_add(EEZE_UDEV_TYPE_DRM, events,
                         _ecore_drm_device_cb_output_event, dev);

   dev->drm.hdlr =
     ecore_main_fd_handler_add(dev->drm.fd, ECORE_FD_READ,
                               _ecore_drm_device_cb_event, dev, NULL, NULL);

   /* dev->drm.idler =  */
   /*   ecore_idle_enterer_add(_ecore_drm_device_cb_idle, dev); */

   return EINA_TRUE;
#endif
}

EAPI Eina_Bool 
ecore_drm_device_close(Ecore_Drm_Device *dev)
{
   /* check for valid device */
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);

   /* delete udev watch */
   if (dev->watch) eeze_udev_watch_del(dev->watch);

   /* close xkb context */
   if (dev->xkb_ctx) xkb_context_unref(dev->xkb_ctx);

   if (dev->drm.hdlr) ecore_main_fd_handler_del(dev->drm.hdlr);
   dev->drm.hdlr = NULL;

   _ecore_drm_launcher_device_close(dev->drm.name, dev->drm.fd);

   _ecore_drm_display_destroy(dev);

   /* reset device fd */
   dev->drm.fd = -1;

   return EINA_TRUE;
}

EAPI const Eina_List *
ecore_drm_devices_get(void)
{
   return drm_devices;
}

EAPI Eina_Bool 
ecore_drm_device_master_get(Ecore_Drm_Device *dev)
{
   drm_magic_t mag;

#ifdef HAVE_TDM
   drmVersionPtr ver = drmGetVersion(ecore_drm_device_fd_get(dev));
   if (ver) drmFreeVersion(ver);
   else return EINA_TRUE;
#endif

   /* check for valid device */
   if ((!dev) || (dev->drm.fd < 0)) return EINA_FALSE;

   /* get if we are master or not */
   if ((drmGetMagic(dev->drm.fd, &mag) == 0) && 
       (drmAuthMagic(dev->drm.fd, mag) == 0))
     return EINA_TRUE;

   return EINA_FALSE;
}

EAPI Eina_Bool 
ecore_drm_device_master_set(Ecore_Drm_Device *dev)
{
#ifdef HAVE_TDM
   drmVersionPtr ver = drmGetVersion(ecore_drm_device_fd_get(dev));
   if (ver) drmFreeVersion(ver);
   else return EINA_TRUE;
#endif

   /* check for valid device */
   if ((!dev) || (dev->drm.fd < 0)) return EINA_FALSE;

   DBG("Set Master On Fd: %d", dev->drm.fd);

   drmSetMaster(dev->drm.fd);

   return EINA_TRUE;
}

EAPI Eina_Bool 
ecore_drm_device_master_drop(Ecore_Drm_Device *dev)
{
#ifdef HAVE_TDM
   drmVersionPtr ver = drmGetVersion(ecore_drm_device_fd_get(dev));
   if (ver) drmFreeVersion(ver);
   else return EINA_TRUE;
#endif

   /* check for valid device */
   if ((!dev) || (dev->drm.fd < 0)) return EINA_FALSE;

   DBG("Drop Master On Fd: %d", dev->drm.fd);

   drmDropMaster(dev->drm.fd);

   return EINA_TRUE;
}

EAPI int 
ecore_drm_device_fd_get(Ecore_Drm_Device *dev)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, -1);

   return dev->drm.fd;
}

EAPI void 
ecore_drm_device_window_set(Ecore_Drm_Device *dev, unsigned int window)
{
   Eina_List *l, *ll;
   Ecore_Drm_Seat *seat = NULL;
   Ecore_Drm_Evdev *edev = NULL;

   /* check for valid device */
   EINA_SAFETY_ON_TRUE_RETURN((!dev) || (dev->drm.fd < 0));

   dev->window = window;

   EINA_LIST_FOREACH(dev->seats, l , seat)
     {
        EINA_LIST_FOREACH(seat->devices, ll, edev)
          _ecore_drm_device_add(window, edev);
     }
}

EAPI const char *
ecore_drm_device_name_get(Ecore_Drm_Device *dev)
{
   /* check for valid device */
   EINA_SAFETY_ON_TRUE_RETURN_VAL((!dev) || (dev->drm.fd < 0), NULL);

#ifdef HAVE_TDM
   /* Do we have to get drm.name from TDM? drm.path also? possible?
    * I guess no.
    */
#endif

   return dev->drm.name;
}

EAPI void
ecore_drm_device_pointer_xy_get(Ecore_Drm_Device *dev, int *x, int *y)
{
   Ecore_Drm_Seat *seat;
   Ecore_Drm_Evdev *edev;
   Eina_List *l, *ll;

   if (x) *x = 0;
   if (y) *y = 0;

   /* check for valid device */
   EINA_SAFETY_ON_TRUE_RETURN((!dev) || (dev->drm.fd < 0));

   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        EINA_LIST_FOREACH(seat->devices, ll, edev)
          {
             if (!libinput_device_has_capability(edev->device,
                                                 LIBINPUT_DEVICE_CAP_POINTER))
               continue;

             if (x) *x = seat->ptr.dx;
             if (y) *y = seat->ptr.dy;

             return;
          }
     }
}

EAPI void
ecore_drm_device_pointer_warp(Ecore_Drm_Device *dev, int x, int y)
{
   Ecore_Drm_Seat *seat;
   Ecore_Drm_Evdev *edev;
   Eina_List *l, *ll;

   /* check for valid device */
   EINA_SAFETY_ON_TRUE_RETURN((!dev) || (dev->drm.fd < 0));
   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        EINA_LIST_FOREACH(seat->devices, ll, edev)
          {
             if (!libinput_device_has_capability(edev->device,
                                                 LIBINPUT_DEVICE_CAP_POINTER))
               continue;

             seat->ptr.dx = seat->ptr.ix = x;
             seat->ptr.dy = seat->ptr.iy = y;
             _ecore_drm_pointer_motion_post(edev);
          }
     }
}

#ifdef HAVE_TDM
EAPI Eina_Bool
ecore_drm_device_software_setup(Ecore_Drm_Device *dev EINA_UNUSED)
{
	return EINA_TRUE;
}
#else
EAPI Eina_Bool
ecore_drm_device_software_setup(Ecore_Drm_Device *dev)
{
   unsigned int i = 0;
   int w = 0, h = 0;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);

   /* destroy any old buffers */
   for (; i < ALEN(dev->dumb); i++)
     {
        if (dev->dumb[i]) ecore_drm_fb_destroy(dev->dumb[i]);
        dev->dumb[i] = NULL;
     }

   /* get screen size */
   ecore_drm_outputs_geometry_get(dev, NULL, NULL, &w, &h);

   /* create new buffers */
   for (i = 0; i < ALEN(dev->dumb); i++)
     {
        if (!(dev->dumb[i] = ecore_drm_fb_create(dev, w, h)))
          {
             ERR("Could not create dumb framebuffer: %m");
             goto err;
          }

        DBG("Ecore_Drm_Device Created Dumb Buffer");
        DBG("\tFb: %d", dev->dumb[i]->id);
        DBG("\tHandle: %d", dev->dumb[i]->hdl);
        DBG("\tStride: %d", dev->dumb[i]->stride);
        DBG("\tSize: %d", dev->dumb[i]->size);
        DBG("\tW: %d\tH: %d", dev->dumb[i]->w, dev->dumb[i]->h);
     }

   return EINA_TRUE;

err:
   for (i = 0; i < ALEN(dev->dumb); i++)
     {
        if (dev->dumb[i]) ecore_drm_fb_destroy(dev->dumb[i]);
        dev->dumb[i] = NULL;
     }
   return EINA_FALSE;
}
#endif

EAPI Ecore_Drm_Output *
ecore_drm_device_output_find(Ecore_Drm_Device *dev, int x, int y)
{
   Ecore_Drm_Output *output;
   Eina_List *l;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, NULL);
   EINA_SAFETY_ON_TRUE_RETURN_VAL((x < 0) || (y < 0), NULL);

   EINA_LIST_FOREACH(dev->outputs, l, output)
     {
        int ox = 0, oy = 0;
        int ow = 0, oh = 0;

        if (!output->cloned)
          {
             ox = output->x;
             oy = output->y;
          }

        ow = output->current_mode->width;
        oh = output->current_mode->height;

        if (INSIDE(x, y, ox, oy, ow, oh))
          return output;
     }

   return NULL;
}

EAPI void
ecore_drm_screen_size_range_get(Ecore_Drm_Device *dev, int *minw, int *minh, int *maxw, int *maxh)
{
   EINA_SAFETY_ON_NULL_RETURN(dev);

   if (minw) *minw = dev->min_width;
   if (minh) *minh = dev->min_height;
   if (maxw) *maxw = dev->max_width;
   if (maxh) *maxh = dev->max_height;
}

EAPI Ecore_Drm_Output *
ecore_drm_device_output_name_find(Ecore_Drm_Device *dev, const char *name)
{
   Ecore_Drm_Output *output;
   Eina_List *l;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   EINA_LIST_FOREACH(dev->outputs, l, output)
     if ((output->name) && (!strcmp(name, output->name)))
       return output;

   return NULL;
}

EAPI Eina_Bool
ecore_drm_device_pointer_left_handed_set(Ecore_Drm_Device *dev, Eina_Bool left_handed)
{
   Ecore_Drm_Seat *seat = NULL;
   Ecore_Drm_Evdev *edev = NULL;
   Eina_List *l = NULL, *l2 = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev->seats, EINA_FALSE);

   if (dev->left_handed == left_handed)
     return EINA_TRUE;
   dev->left_handed = left_handed;

   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        EINA_LIST_FOREACH(ecore_drm_seat_evdev_list_get(seat), l2, edev)
          {
             if (libinput_device_has_capability(edev->device,
                                                LIBINPUT_DEVICE_CAP_POINTER))
               {
                  if (libinput_device_config_left_handed_set(edev->device, (int)left_handed) !=
                      LIBINPUT_CONFIG_STATUS_SUCCESS)
                    {
                       WRN("Failed to set left hand mode about device: %s\n",
                           libinput_device_get_name(edev->device));
                       continue;
                    }
               }
          }
     }
   return EINA_TRUE;
}

EAPI void
ecore_drm_device_keyboard_cached_context_set(struct xkb_context *ctx)
{
   EINA_SAFETY_ON_NULL_RETURN(ctx);

   if (cached_context == ctx) return;

   if (cached_context)
     _ecore_drm_device_cached_context_update(ctx);

   cached_context = ctx;
}

EAPI void
ecore_drm_device_keyboard_cached_keymap_set(struct xkb_keymap *map)
{
   EINA_SAFETY_ON_NULL_RETURN(map);

   if (cached_keymap == map) return;

   if (cached_keymap)
      _ecore_drm_device_cached_keymap_update(map);

   cached_keymap = map;
}

EAPI Eina_Bool
ecore_drm_device_pointer_rotation_set(Ecore_Drm_Device *dev, int rotation)
{
   Ecore_Drm_Seat *seat = NULL;
   Eina_List *l = NULL;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev->seats, EINA_FALSE);

   if ((rotation % 90 != 0) || (rotation / 90 > 3) || (rotation < 0)) return EINA_FALSE;

   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        switch (rotation)
          {
           case 90:
              seat->ptr.swap = EINA_TRUE;
              seat->ptr.invert_x = EINA_FALSE;
              seat->ptr.invert_y = EINA_TRUE;
              break;
           case 180:
              seat->ptr.swap = EINA_FALSE;
              seat->ptr.invert_x = EINA_TRUE;
              seat->ptr.invert_y = EINA_TRUE;
              break;
           case 270:
              seat->ptr.swap = EINA_TRUE;
              seat->ptr.invert_x = EINA_TRUE;
              seat->ptr.invert_y = EINA_FALSE;
		break;
           case 0:
              seat->ptr.swap = EINA_FALSE;
              seat->ptr.invert_x = EINA_FALSE;
              seat->ptr.invert_y = EINA_FALSE;
              break;
           default:
              break;
          }
     }
   return EINA_TRUE;
}

static void
_ecore_drm_device_touch_matrix_identify(float result[6])
{
   result[0] = 1.0;
   result[1] = 0.0;
   result[2] = 0.0;
   result[3] = 0.0;
   result[4] = 1.0;
   result[5] = 0.0;
}

static void
_ecore_drm_device_touch_matrix_mulifly(float result[6], float m1[6], float m2[6])
{
   result[0] = m1[0] * m2 [0] + m1[1] * m2[3];
   result[1] = m1[0] * m2 [1] + m1[1] * m2[4];
   result[2] = m1[0] * m2 [2] + m1[1] * m2[5] + m1[2];
   result[3] = m1[3] * m2 [0] + m1[4] * m2[3];
   result[4] = m1[3] * m2 [1] + m1[4] * m2[4];
   result[5] = m1[3] * m2 [2] + m1[4] * m2[5] + m1[5];
}

static void
_ecore_drm_device_touch_matrix_rotation_get(float result[6], int degree, float w, float h)
{
   if (w == 0.0) w = 1.0;
   if (h == 0.0) h = 1.0;

   switch (degree)
     {
        case 90:
          result[0] = 0.0;
          result[1] = -h/w;
          result[2] = h/w;
          result[3] = w/h;
          result[4] = 0.0;
          result[5] = 0.0;
          break;
        case 180:
          result[0] = -1.0;
          result[1] = 0.0;
          result[2] = 1.0;
          result[3] = 0.0;
          result[4] = -1.0;
          result[5] = 1.0;
          break;
        case 270:
          result[0] = 0.0;
          result[1] = h/w;
          result[2] = 0.0;
          result[3] = -w/h;
          result[4] = 0.0;
          result[5] = w/h;
          break;
        case 0:
          _ecore_drm_device_touch_matrix_identify(result);
          break;
        default:
          WRN("Please input valid angle(%d)\n", degree);
     }
}

static void
_ecore_drm_device_touch_matrix_translate_get(float result[6], float x, float y, float w, float h, float default_w, float default_h)
{
   if (default_w == 0.0) default_w = 1.0;
   if (default_h == 0.0) default_h = 1.0;

   result[0] = w / default_w;
   result[4] = h / default_h;
   result[2] = x / default_w;
   result[5] = y / default_h;
}

EAPI Eina_Bool
ecore_drm_device_touch_rotation_set(Ecore_Drm_Device *dev, unsigned int rotation)
{
   Ecore_Drm_Seat *seat = NULL;
   Ecore_Drm_Evdev *edev = NULL;
   Eina_List *l = NULL, *l2 = NULL;
   float mat_translate[6] = {0.0, }, mat_rotation[6] = {0.0, }, result[6] = {0.0, };
   float default_w = 0.0, default_h = 0.0;
   Eina_Bool res = EINA_TRUE;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev->seats, EINA_FALSE);

   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        EINA_LIST_FOREACH(ecore_drm_seat_evdev_list_get(seat), l2, edev)
          {
             if (edev->seat_caps & EVDEV_SEAT_TOUCH)
               {
                  if (edev->output && edev->output->current_mode)
                    {
                       default_w = (float)edev->output->current_mode->width;
                       default_h = (float)edev->output->current_mode->height;
                    }
                  else
                    {
                       default_w = 0.0;
                       default_h = 0.0;
                    }

                  _ecore_drm_device_touch_matrix_identify(mat_translate);
                  _ecore_drm_device_touch_matrix_identify(mat_rotation);
                  _ecore_drm_device_touch_matrix_identify(result);

                  if (edev->touch.transform.x || edev->touch.transform.y ||
                      edev->touch.transform.w || edev->touch.transform.h)
                    {
                       _ecore_drm_device_touch_matrix_translate_get(mat_translate,
                                                                    (float)edev->touch.transform.x,
                                                                    (float)edev->touch.transform.y,
                                                                    (float)edev->touch.transform.w,
                                                                    (float)edev->touch.transform.h,
                                                                    default_w, default_h);

                    }

                  _ecore_drm_device_touch_matrix_rotation_get(mat_rotation, rotation, default_w, default_h);

                  _ecore_drm_device_touch_matrix_mulifly(result, mat_translate, mat_rotation);

                  if (!ecore_drm_evdev_touch_calibration_set(edev, result))
                    {
                       res = EINA_FALSE;
                       continue;
                    }
                  else
                    {
                       edev->touch.transform.rotation = rotation;
                    }
               }
          }
     }

   return res;
}

EAPI Eina_Bool
ecore_drm_device_touch_transformation_set(Ecore_Drm_Device *dev, int offset_x, int offset_y, int w, int h)
{
   Ecore_Drm_Seat *seat = NULL;
   Ecore_Drm_Evdev *edev = NULL;
   Eina_List *l = NULL, *l2 = NULL;
   float mat_translate[6] = {0.0, }, mat_rotation[6] = {0.0 }, result[6] = {0.0, };
   float default_w, default_h;
   Eina_Bool res = EINA_TRUE;

   EINA_SAFETY_ON_NULL_RETURN_VAL(dev, EINA_FALSE);
   EINA_SAFETY_ON_NULL_RETURN_VAL(dev->seats, EINA_FALSE);
   EINA_SAFETY_ON_TRUE_RETURN_VAL((w == 0) || (h == 0), EINA_FALSE);

   EINA_LIST_FOREACH(dev->seats, l, seat)
     {
        EINA_LIST_FOREACH(ecore_drm_seat_evdev_list_get(seat), l2, edev)
          {
             if (edev->seat_caps & EVDEV_SEAT_TOUCH)
               {
                  if (edev->output && edev->output->current_mode)
                    {
                       default_w = (float)edev->output->current_mode->width;
                       default_h = (float)edev->output->current_mode->height;
                    }
                  else
                    {
                       default_w = 0.0;
                       default_h = 0.0;
                    }

                  _ecore_drm_device_touch_matrix_identify(mat_translate);
                  _ecore_drm_device_touch_matrix_identify(mat_rotation);
                  _ecore_drm_device_touch_matrix_identify(result);

                  _ecore_drm_device_touch_matrix_translate_get(mat_translate,
                                                               (float)offset_x, (float)offset_y,
                                                               (float)w, (float)h, default_w, default_h);

                  if (edev->touch.transform.rotation)
                    {
                       _ecore_drm_device_touch_matrix_rotation_get(mat_rotation,
                                                                   edev->touch.transform.rotation,
                                                                   default_w, default_h);
                    }

                  _ecore_drm_device_touch_matrix_mulifly(result, mat_translate, mat_rotation);

                  if (!ecore_drm_evdev_touch_calibration_set(edev, result))
                    {
                       res = EINA_FALSE;
                       continue;
                    }
                  else
                    {
                       edev->touch.transform.x = offset_x;
                       edev->touch.transform.y = offset_y;
                       edev->touch.transform.w = w;
                       edev->touch.transform.h = h;
                    }
               }
          }
     }
   return res;
}
