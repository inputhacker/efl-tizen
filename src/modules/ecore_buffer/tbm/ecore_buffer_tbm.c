#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Eina.h>
#include <Ecore.h>

#include <tbm_bufmgr.h>
#include <tbm_surface.h>
#include <tbm_surface_internal.h>

#include <Ecore_Buffer.h>
#include "ecore_buffer_private.h"

typedef struct _Ecore_Buffer_Module_Tbm_Data Ecore_Buffer_Module_Tbm_Data;
typedef struct _Ecore_Buffer_Tbm_Data Ecore_Buffer_Tbm_Data;

struct _Ecore_Buffer_Module_Tbm_Data {
     tbm_bufmgr tbm_mgr;
};

struct _Ecore_Buffer_Tbm_Data {
     void *tbm_surface;
     int w;
     int h;
     int stride;
     unsigned int flags;
     Ecore_Buffer_Format format;
     Eina_Bool is_imported;
};

static int
_buf_get_num_planes(Ecore_Buffer_Format format)
{
   int num_planes = 0;

   switch (format)
     {
      case ECORE_BUFFER_FORMAT_C8:
      case ECORE_BUFFER_FORMAT_RGB332:
      case ECORE_BUFFER_FORMAT_BGR233:
      case ECORE_BUFFER_FORMAT_XRGB4444:
      case ECORE_BUFFER_FORMAT_XBGR4444:
      case ECORE_BUFFER_FORMAT_RGBX4444:
      case ECORE_BUFFER_FORMAT_BGRX4444:
      case ECORE_BUFFER_FORMAT_ARGB4444:
      case ECORE_BUFFER_FORMAT_ABGR4444:
      case ECORE_BUFFER_FORMAT_RGBA4444:
      case ECORE_BUFFER_FORMAT_BGRA4444:
      case ECORE_BUFFER_FORMAT_XRGB1555:
      case ECORE_BUFFER_FORMAT_XBGR1555:
      case ECORE_BUFFER_FORMAT_RGBX5551:
      case ECORE_BUFFER_FORMAT_BGRX5551:
      case ECORE_BUFFER_FORMAT_ARGB1555:
      case ECORE_BUFFER_FORMAT_ABGR1555:
      case ECORE_BUFFER_FORMAT_RGBA5551:
      case ECORE_BUFFER_FORMAT_BGRA5551:
      case ECORE_BUFFER_FORMAT_RGB565:
      case ECORE_BUFFER_FORMAT_BGR565:
      case ECORE_BUFFER_FORMAT_RGB888:
      case ECORE_BUFFER_FORMAT_BGR888:
      case ECORE_BUFFER_FORMAT_XRGB8888:
      case ECORE_BUFFER_FORMAT_XBGR8888:
      case ECORE_BUFFER_FORMAT_RGBX8888:
      case ECORE_BUFFER_FORMAT_BGRX8888:
      case ECORE_BUFFER_FORMAT_ARGB8888:
      case ECORE_BUFFER_FORMAT_ABGR8888:
      case ECORE_BUFFER_FORMAT_RGBA8888:
      case ECORE_BUFFER_FORMAT_BGRA8888:
      case ECORE_BUFFER_FORMAT_XRGB2101010:
      case ECORE_BUFFER_FORMAT_XBGR2101010:
      case ECORE_BUFFER_FORMAT_RGBX1010102:
      case ECORE_BUFFER_FORMAT_BGRX1010102:
      case ECORE_BUFFER_FORMAT_ARGB2101010:
      case ECORE_BUFFER_FORMAT_ABGR2101010:
      case ECORE_BUFFER_FORMAT_RGBA1010102:
      case ECORE_BUFFER_FORMAT_BGRA1010102:
      case ECORE_BUFFER_FORMAT_YUYV:
      case ECORE_BUFFER_FORMAT_YVYU:
      case ECORE_BUFFER_FORMAT_UYVY:
      case ECORE_BUFFER_FORMAT_VYUY:
      case ECORE_BUFFER_FORMAT_AYUV:
         num_planes = 1;
         break;
      case ECORE_BUFFER_FORMAT_NV12:
      case ECORE_BUFFER_FORMAT_NV21:
      case ECORE_BUFFER_FORMAT_NV16:
      case ECORE_BUFFER_FORMAT_NV61:
         num_planes = 2;
         break;
      case ECORE_BUFFER_FORMAT_YUV410:
      case ECORE_BUFFER_FORMAT_YVU410:
      case ECORE_BUFFER_FORMAT_YUV411:
      case ECORE_BUFFER_FORMAT_YVU411:
      case ECORE_BUFFER_FORMAT_YUV420:
      case ECORE_BUFFER_FORMAT_YVU420:
      case ECORE_BUFFER_FORMAT_YUV422:
      case ECORE_BUFFER_FORMAT_YVU422:
      case ECORE_BUFFER_FORMAT_YUV444:
      case ECORE_BUFFER_FORMAT_YVU444:
         num_planes = 3;
         break;

      default:
         break;
     }

   return num_planes;
}

static int
_buf_get_bpp(Ecore_Buffer_Format format)
{
   int bpp = 0;

   switch (format)
     {
      case ECORE_BUFFER_FORMAT_C8:
      case ECORE_BUFFER_FORMAT_RGB332:
      case ECORE_BUFFER_FORMAT_BGR233:
         bpp = 8;
         break;
      case ECORE_BUFFER_FORMAT_XRGB4444:
      case ECORE_BUFFER_FORMAT_XBGR4444:
      case ECORE_BUFFER_FORMAT_RGBX4444:
      case ECORE_BUFFER_FORMAT_BGRX4444:
      case ECORE_BUFFER_FORMAT_ARGB4444:
      case ECORE_BUFFER_FORMAT_ABGR4444:
      case ECORE_BUFFER_FORMAT_RGBA4444:
      case ECORE_BUFFER_FORMAT_BGRA4444:
      case ECORE_BUFFER_FORMAT_XRGB1555:
      case ECORE_BUFFER_FORMAT_XBGR1555:
      case ECORE_BUFFER_FORMAT_RGBX5551:
      case ECORE_BUFFER_FORMAT_BGRX5551:
      case ECORE_BUFFER_FORMAT_ARGB1555:
      case ECORE_BUFFER_FORMAT_ABGR1555:
      case ECORE_BUFFER_FORMAT_RGBA5551:
      case ECORE_BUFFER_FORMAT_BGRA5551:
      case ECORE_BUFFER_FORMAT_RGB565:
      case ECORE_BUFFER_FORMAT_BGR565:
         bpp = 16;
         break;
      case ECORE_BUFFER_FORMAT_RGB888:
      case ECORE_BUFFER_FORMAT_BGR888:
         bpp = 24;
         break;
      case ECORE_BUFFER_FORMAT_XRGB8888:
      case ECORE_BUFFER_FORMAT_XBGR8888:
      case ECORE_BUFFER_FORMAT_RGBX8888:
      case ECORE_BUFFER_FORMAT_BGRX8888:
      case ECORE_BUFFER_FORMAT_ARGB8888:
      case ECORE_BUFFER_FORMAT_ABGR8888:
      case ECORE_BUFFER_FORMAT_RGBA8888:
      case ECORE_BUFFER_FORMAT_BGRA8888:
      case ECORE_BUFFER_FORMAT_XRGB2101010:
      case ECORE_BUFFER_FORMAT_XBGR2101010:
      case ECORE_BUFFER_FORMAT_RGBX1010102:
      case ECORE_BUFFER_FORMAT_BGRX1010102:
      case ECORE_BUFFER_FORMAT_ARGB2101010:
      case ECORE_BUFFER_FORMAT_ABGR2101010:
      case ECORE_BUFFER_FORMAT_RGBA1010102:
      case ECORE_BUFFER_FORMAT_BGRA1010102:
      case ECORE_BUFFER_FORMAT_YUYV:
      case ECORE_BUFFER_FORMAT_YVYU:
      case ECORE_BUFFER_FORMAT_UYVY:
      case ECORE_BUFFER_FORMAT_VYUY:
      case ECORE_BUFFER_FORMAT_AYUV:
         bpp = 32;
         break;
      case ECORE_BUFFER_FORMAT_NV12:
      case ECORE_BUFFER_FORMAT_NV21:
         bpp = 12;
         break;
      case ECORE_BUFFER_FORMAT_NV16:
      case ECORE_BUFFER_FORMAT_NV61:
         bpp = 16;
         break;
      case ECORE_BUFFER_FORMAT_YUV410:
      case ECORE_BUFFER_FORMAT_YVU410:
         bpp = 9;
         break;
      case ECORE_BUFFER_FORMAT_YUV411:
      case ECORE_BUFFER_FORMAT_YVU411:
      case ECORE_BUFFER_FORMAT_YUV420:
      case ECORE_BUFFER_FORMAT_YVU420:
         bpp = 12;
         break;
      case ECORE_BUFFER_FORMAT_YUV422:
      case ECORE_BUFFER_FORMAT_YVU422:
         bpp = 16;
         break;
      case ECORE_BUFFER_FORMAT_YUV444:
      case ECORE_BUFFER_FORMAT_YVU444:
         bpp = 24;
         break;
      default :
         break;
     }

   return bpp;
}

static Ecore_Buffer_Module_Data
_ecore_buffer_tbm_init(const char *context EINA_UNUSED, const char *options EINA_UNUSED)
{
   Ecore_Buffer_Module_Tbm_Data *mdata = NULL;

   mdata = calloc(sizeof(Ecore_Buffer_Module_Tbm_Data), 1);
   EINA_SAFETY_ON_NULL_RETURN_VAL(mdata, NULL);

   mdata->tbm_mgr = tbm_bufmgr_init(-1);
   if (!mdata->tbm_mgr)
     {
        free(mdata);
        return NULL;
     }

   return mdata;
}

static void
_ecore_buffer_tbm_shutdown(Ecore_Buffer_Module_Data bmdata)
{
   Ecore_Buffer_Module_Tbm_Data *bm = bmdata;

   if (!bm) return;

   if (bm->tbm_mgr)
     tbm_bufmgr_deinit(bm->tbm_mgr);

   free(bm);
}

static Ecore_Buffer_Data
_ecore_buffer_tbm_buffer_alloc(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, int width, int height, Ecore_Buffer_Format format, unsigned int flags)
{
   Ecore_Buffer_Tbm_Data *buf;

   /* invalid size */
   if ((width < 1) || (height < 1))
     return NULL;

   buf = calloc(1, sizeof(Ecore_Buffer_Tbm_Data));
   if (!buf)
     return NULL;

   buf->w = width;
   buf->h = height;
   buf->flags = flags;
   buf->format = format;
   buf->is_imported = EINA_FALSE;
   buf->tbm_surface = tbm_surface_create(width, height, (tbm_format)format);
   if (!buf->tbm_surface)
     {
        free(buf);
        return NULL;
     }

   return buf;
}

static Ecore_Buffer_Data
_ecore_buffer_tbm_buffer_alloc_with_tbm_surface(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, void *tbm_surface, int *ret_w, int *ret_h, Ecore_Buffer_Format *ret_format, unsigned int flags)
{
   Ecore_Buffer_Tbm_Data *buf;

   buf = calloc(1, sizeof(Ecore_Buffer_Tbm_Data));
   if (!buf)
     return NULL;

   buf->w = tbm_surface_get_width(tbm_surface);
   buf->h = tbm_surface_get_height(tbm_surface);
   buf->format = tbm_surface_get_format(tbm_surface);
   buf->flags = flags;
   buf->is_imported = EINA_FALSE;
   buf->tbm_surface = tbm_surface;

   if (ret_w) *ret_w = buf->w;
   if (ret_h) *ret_h = buf->h;
   if (ret_format) *ret_format = buf->format;

   return buf;
}

static Eina_Bool
_ecore_buffer_tbm_buffer_info_get(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, Ecore_Buffer_Data bdata, Ecore_Buffer_Info *info)
{
   Ecore_Buffer_Tbm_Data *buf = bdata;
   tbm_surface_info_s tinfo;
   int i, res;

   if (!buf->tbm_surface)
     return EINA_FALSE;

   res = tbm_surface_get_info(buf->tbm_surface, &tinfo);
   if (res != TBM_SURFACE_ERROR_NONE)
     return EINA_FALSE;

   if (info)
     {
        info->width = tinfo.width;
        info->height = tinfo.height;
        info->format = tinfo.format;
        info->bpp = tinfo.bpp;
        info->size = tinfo.size;
        info->num_planes = tinfo.num_planes;
        info->pixmap = 0;

        for (i = 0; i < (int)tinfo.num_planes; i++)
          {
             info->planes[i].size = tinfo.planes[i].size;
             info->planes[i].offset = tinfo.planes[i].offset;
             info->planes[i].stride = tinfo.planes[i].stride;
          }
     }

   return EINA_TRUE;
}

static void
_ecore_buffer_tbm_buffer_free(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, Ecore_Buffer_Data bdata)
{
   Ecore_Buffer_Tbm_Data *buf = bdata;

   if (!buf) return;

   if (buf->tbm_surface)
     {
        tbm_surface_destroy(buf->tbm_surface);
        buf->tbm_surface = NULL;
     }

   free(buf);
}

static Ecore_Export_Type
_ecore_buffer_tbm_buffer_export(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, Ecore_Buffer_Data bdata, int *id)
{
   Ecore_Buffer_Tbm_Data *buf = bdata;
   tbm_bo bo;

   if (!buf) return EXPORT_TYPE_INVALID;

   if (tbm_surface_internal_get_num_bos(buf->tbm_surface) != 1)
     return EXPORT_TYPE_INVALID;

   bo = tbm_surface_internal_get_bo(buf->tbm_surface, 0);

   if (id) *id = tbm_bo_export_fd(bo);

   return EXPORT_TYPE_FD;
}

static Ecore_Buffer_Data
_ecore_buffer_tbm_buffer_import(Ecore_Buffer_Module_Data bmdata, Ecore_Buffer_Info *einfo, Ecore_Export_Type type, int export_id, unsigned int flags)
{
   Ecore_Buffer_Module_Tbm_Data *bm = bmdata;
   Ecore_Buffer_Tbm_Data *buf;
   tbm_bo bo;
   tbm_surface_info_s tinfo;
   int i;

   if (type != EXPORT_TYPE_FD) return NULL;
   if (export_id < 1) return NULL;

   buf = calloc(1, sizeof(Ecore_Buffer_Tbm_Data));
   if (!buf)
     return NULL;

   buf->w = einfo->width;
   buf->h = einfo->height;
   buf->stride = einfo->planes[0].stride;
   buf->format = einfo->format;
   buf->flags = flags;
   buf->is_imported = EINA_TRUE;

   bo = tbm_bo_import_fd(bm->tbm_mgr, export_id);
   if (!bo)
     {
        free(buf);
        return NULL;
     }

   tinfo.width = einfo->width;
   tinfo.height = einfo->height;
   tinfo.format = einfo->format;
   tinfo.bpp = _buf_get_bpp(einfo->format);
   tinfo.size = einfo->height * einfo->planes[0].stride;
   tinfo.num_planes = (uint32_t)_buf_get_num_planes(einfo->format);
   for ( i = 0 ; i < (int)tinfo.num_planes ; i++)
   {
      tinfo.planes[i].size = einfo->height * einfo->planes[i].stride;
      tinfo.planes[i].stride = einfo->planes[i].stride;
      tinfo.planes[i].offset = einfo->planes[i].offset;
   }

   buf->tbm_surface = tbm_surface_internal_create_with_bos(&tinfo, &bo, 1);
   if (!buf->tbm_surface)
     {
        tbm_bo_unref(bo);
        free(buf);
        return NULL;
     }

   tbm_bo_unref(bo);

   return buf;
}

static void *
_ecore_buffer_tbm_tbm_bo_get(Ecore_Buffer_Module_Data bmdata EINA_UNUSED, Ecore_Buffer_Data bdata)
{
   Ecore_Buffer_Tbm_Data *buf = bdata;

   if (!buf) return NULL;

   return buf->tbm_surface;
}

static Ecore_Buffer_Backend _ecore_buffer_tbm_backend = {
     "tbm",
     &_ecore_buffer_tbm_init,
     &_ecore_buffer_tbm_shutdown,
     &_ecore_buffer_tbm_buffer_alloc,
     &_ecore_buffer_tbm_buffer_alloc_with_tbm_surface,
     &_ecore_buffer_tbm_buffer_info_get,
     &_ecore_buffer_tbm_buffer_free,
     &_ecore_buffer_tbm_buffer_export,
     &_ecore_buffer_tbm_buffer_import,
     NULL,
     &_ecore_buffer_tbm_tbm_bo_get,
};

Eina_Bool tbm_init(void)
{
   return ecore_buffer_register(&_ecore_buffer_tbm_backend);
}

void tbm_shutdown(void)
{
   ecore_buffer_unregister(&_ecore_buffer_tbm_backend);
}

EINA_MODULE_INIT(tbm_init);
EINA_MODULE_SHUTDOWN(tbm_shutdown);
