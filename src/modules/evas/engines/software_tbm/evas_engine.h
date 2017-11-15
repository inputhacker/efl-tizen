#ifndef EVAS_ENGINE_H
# define EVAS_ENGINE_H

# include "evas_common_private.h"
# include "evas_macros.h"
# include "evas_private.h"
# include "Evas.h"
# include "Evas_Engine_Software_Tbm.h"
# include "Evas_Engine_Software_Generic.h"

# ifdef LOGFNS
#  include <stdio.h>
#  define LOGFN(fl, ln, fn) printf("-EVAS-TBM: %25s: %5i - %s\n", fl, ln, fn);
# else
#  define LOGFN(fl, ln, fn)
# endif

extern int _evas_engine_software_tbm_log_dom;

# ifdef ERR
#  undef ERR
# endif
# define ERR(...) EINA_LOG_DOM_ERR(_evas_engine_software_tbm_log_dom, __VA_ARGS__)

# ifdef DBG
#  undef DBG
# endif
# define DBG(...) EINA_LOG_DOM_DBG(_evas_engine_software_tbm_log_dom, __VA_ARGS__)

# ifdef INF
#  undef INF
# endif
# define INF(...) EINA_LOG_DOM_INFO(_evas_engine_software_tbm_log_dom, __VA_ARGS__)

# ifdef WRN
#  undef WRN
# endif
# define WRN(...) EINA_LOG_DOM_WARN(_evas_engine_software_tbm_log_dom, __VA_ARGS__)

# ifdef CRI
#  undef CRI
# endif
# define CRI(...) EINA_LOG_DOM_CRIT(_evas_engine_software_tbm_log_dom, __VA_ARGS__)

struct _Outbuf
{
   int w, h;
   int rotation, alpha;
   Outbuf_Depth depth;

   Evas *evas;
   Evas_Engine_Info_Software_Tbm *info;

   void *tbm_queue;
   Eina_Bool ext_tbm_queue;

   void* surface;

   struct
   {
      /* one big buffer for updates. flushed on idle_flush */
      RGBA_Image *onebuf;
      Eina_Array onebuf_regions;

      /* a list of pending regions to write out */
      Eina_List *pending_writes;

      /* list of previous frame pending regions to write out */
      Eina_List *prev_pending_writes;

      /* Eina_Bool redraw : 1; */
      Eina_Bool destination_alpha : 1;

      /*New Struct*/
      void *tbm_queue;
      Eina_Bool queue_need_destroy;
      Eina_Bool queue_reset;
      unsigned int frame_age;
      void *ebuf_info;
   } priv;
};

#if 0
/*OLD*/
Outbuf *_evas_software_tbm_outbuf_setup(int w, int h, int rot, Outbuf_Depth depth, Eina_Bool alpha, void *tbm_queue);
void _evas_software_tbm_outbuf_free(Outbuf *ob);
void _evas_software_tbm_outbuf_flush(Outbuf *ob, Tilebuf_Rect *rects, Evas_Render_Mode render_mode);
void _evas_software_tbm_outbuf_idle_flush(Outbuf *ob);

Render_Output_Swap_Mode _evas_software_tbm_outbuf_swap_mode_get(Outbuf *ob);
int _evas_software_tbm_outbuf_rotation_get(Outbuf *ob);
void _evas_software_tbm_outbuf_reconfigure(Outbuf *ob, int x, int y, int w, int h, int rot, Outbuf_Depth depth, Eina_Bool alpha, Eina_Bool resize);
void *_evas_software_tbm_outbuf_update_region_new(Outbuf *ob, int x, int y, int w, int h, int *cx, int *cy, int *cw, int *ch);
void _evas_software_tbm_outbuf_update_region_push(Outbuf *ob, RGBA_Image *update, int x, int y, int w, int h);
void _evas_software_tbm_outbuf_update_region_free(Outbuf *ob, RGBA_Image *update);
#endif

/*New*/
Outbuf *evas_outbuf_setup(Evas_Engine_Info_Software_Tbm *info, int w, int h);
void evas_outbuf_free(Outbuf *ob);
void evas_outbuf_reconfigure(Outbuf *ob, int w, int h, int rot, Outbuf_Depth depth);
Render_Output_Swap_Mode evas_outbuf_swap_mode_get(Outbuf *ob);
int evas_outbuf_rot_get(Outbuf *ob);
Eina_Bool evas_outbuf_update_region_first_rect(Outbuf *ob);
void *evas_outbuf_update_region_new(Outbuf *ob, int x, int y, int w, int h, int *cx, int *cy, int *cw, int *ch);
void evas_outbuf_update_region_push(Outbuf *ob, RGBA_Image *update, int x, int y, int w, int h);
void evas_outbuf_update_region_free(Outbuf *ob, RGBA_Image *update);
void evas_outbuf_flush(Outbuf *ob, Tilebuf_Rect *surface_damage, Tilebuf_Rect *buffer_damage, Evas_Render_Mode render_mode);
void evas_output_idle_flush(Outbuf *ob);

#endif
