#ifndef EVAS_ENGINE_GL_GENERIC_H__
# define EVAS_ENGINE_GL_GENERIC_H__

#include "../software_generic/Evas_Engine_Software_Generic.h"
#include "Evas_Engine_GL_Shared.h"

#include "../gl_common/evas_gl_common.h"
#include "../gl_common/evas_gl_core.h"
#include "../gl_common/evas_gl_core_private.h"

typedef struct _Render_Engine_GL_Generic Render_Engine_GL_Generic;
typedef struct _Render_Output_GL_Generic Render_Output_GL_Generic;
typedef struct _Context_3D Context_3D;

typedef void (*Window_Use)(Outbuf *ob);
typedef Evas_Engine_GL_Context *(*Window_GL_Context_Get)(Outbuf *ob);
typedef void *(*Window_EGL_Display_Get)(Outbuf *ob);
typedef Context_3D *(*Window_GL_Context_New)(Outbuf *ob);
typedef void (*Window_GL_Context_Use)(Context_3D *ctx);

struct _Render_Engine_GL_Generic
{
   Render_Engine_Software_Generic software;

   Render_Output_GL_Generic *current;

   struct {
      Evas_Object_Image_Pixels_Get_Cb  get_pixels;
      void                            *get_pixels_data;
      Evas_Object                     *obj;
   } func;

   Eina_Bool evgl_initted : 1;
};

struct _Render_Output_GL_Generic
{
   Render_Output_Software_Generic software;

   Window_Use window_use;
   Window_GL_Context_Get window_gl_context_get;
   Window_EGL_Display_Get window_egl_display_get;
   Window_GL_Context_New window_gl_context_new;
   Window_GL_Context_Use window_gl_context_use;

   Context_3D           *context_3d;
   E3D_Renderer         *renderer_3d;
   const EVGL_Interface *evgl_funcs;
};

static inline Eina_Bool
evas_render_engine_gl_generic_init(Render_Engine_Software_Generic *engine,
                                   Render_Output_GL_Generic *re,
                                   Outbuf *ob,
                                   Outbuf_Swap_Mode_Get outbuf_swap_mode_get,
                                   Outbuf_Get_Rot outbuf_get_rot,
                                   Outbuf_Reconfigure outbuf_reconfigure,
                                   Outbuf_Region_First_Rect outbuf_region_first_rect,
                                   Outbuf_Damage_Region_Set outbuf_damage_region_set,
                                   Outbuf_New_Region_For_Update outbuf_new_region_for_update,
                                   Outbuf_Push_Updated_Region outbuf_push_updated_region,
                                   Outbuf_Free_Region_For_Update outbuf_free_region_for_update,
                                   Outbuf_Idle_Flush outbuf_idle_flush,
                                   Outbuf_Flush outbuf_flush,
                                   Outbuf_Redraws_Clear outbuf_redraws_clear,
                                   Outbuf_Free outbuf_free,
                                   Window_Use window_use,
                                   Window_GL_Context_Get window_gl_context_get,
                                   Window_EGL_Display_Get window_egl_display_get,
                                   Window_GL_Context_New window_gl_context_new,
                                   Window_GL_Context_Use window_gl_context_use,
                                   const EVGL_Interface *evgl_funcs,
                                   int w, int h)
{
   if (!evas_render_engine_software_generic_init(engine, &re->software, ob,
                                                 outbuf_swap_mode_get,
                                                 outbuf_get_rot,
                                                 outbuf_reconfigure,
                                                 outbuf_region_first_rect,
                                                 outbuf_damage_region_set,
                                                 outbuf_new_region_for_update,
                                                 outbuf_push_updated_region,
                                                 outbuf_free_region_for_update,
                                                 outbuf_idle_flush,
                                                 outbuf_flush,
                                                 outbuf_redraws_clear,
                                                 outbuf_free,
                                                 w, h))
     return EINA_FALSE;

   re->window_use = window_use;
   re->window_gl_context_get = window_gl_context_get;
   re->window_egl_display_get = window_egl_display_get;
   re->window_gl_context_new = window_gl_context_new;
   re->window_gl_context_use = window_gl_context_use;

   re->context_3d = NULL;
   re->renderer_3d = NULL;
   re->evgl_funcs = evgl_funcs;

   evas_render_engine_software_generic_tile_strict_set(&re->software, EINA_TRUE);

   return EINA_TRUE;
}

static inline Evas_Engine_GL_Context *
gl_generic_context_get(Render_Output_GL_Generic *output, Eina_Bool use)
{
   if (!output->software.ob) return NULL;
   if (use) output->window_use(output->software.ob);
   return output->window_gl_context_get(output->software.ob);

}

static inline void
gl_generic_window_use(void *engine)
{
   Render_Output_GL_Generic *re = engine;

   re->window_use(re->software.ob);
}

static inline Evas_Engine_GL_Context *
gl_generic_context_find(Render_Engine_GL_Generic *engine, Eina_Bool use)
{
   Render_Output_GL_Generic *output;
   Evas_Engine_GL_Context *r = NULL;
   Eina_List *l;

   EINA_LIST_FOREACH(engine->software.outputs, l, output)
     {
        r = gl_generic_context_get(output, use);
        if (r) return r;
     }

   return r;
}

static inline void
gl_generic_window_find(Render_Engine_GL_Generic *engine)
{
   Render_Output_GL_Generic *output;
   Eina_List *l;

   EINA_LIST_FOREACH(engine->software.outputs, l, output)
     {
        if (!output->software.ob) continue;
        gl_generic_window_use(output);
        break;
     }
}

static inline void *
gl_generic_any_output_get(Render_Engine_GL_Generic *engine)
{
   Render_Output_GL_Generic *output;
   Eina_List *l;

   EINA_LIST_FOREACH(engine->software.outputs, l, output)
     {
        if (!output->software.ob) continue;
        return output->software.ob;
     }

   return NULL;
}

static inline Render_Output_GL_Generic *
gl_generic_output_find(Render_Engine_GL_Generic *engine)
{
   Render_Output_GL_Generic *output;
   Eina_List *l;

   EINA_LIST_FOREACH(engine->software.outputs, l, output)
     {
        if (!output->software.ob) continue;
        return output;
     }

   return NULL;
}


#endif
