#ifndef EVAS_VG_PRIVATE_H_
# define EVAS_VG_PRIVATE_H_

#include <Ector.h>

#include "evas_vg_cache.h"

typedef struct _Efl_VG_Base_Data Efl_VG_Base_Data;
typedef struct _Efl_VG_Container_Data Efl_VG_Container_Data;
typedef struct _Efl_VG_Gradient_Data Efl_VG_Gradient_Data;
typedef struct _Efl_VG_Interpolation Efl_VG_Interpolation;
typedef struct _Evas_VG_Data Evas_VG_Data;

typedef struct _Vg_Cache
{
   Eina_Hash            *vfd_hash;
   Eina_Hash            *vg_entry_hash;
   int                   ref;
} Vg_Cache;

typedef struct _Vg_Cache_Entry
{
   char                 *hash_key;
   const Eina_File      *file;
   Eina_Stringshare     *key;
   int                   w;
   int                   h;
   Efl_VG               *root[3];     //0: default, 1: start frame, 2: end frame
   int                   ref;
   Vg_File_Data         *vfd;

   Eina_Bool             mmap : 1;
} Vg_Cache_Entry;

struct _Evas_VG_Data
{
   void   *engine_data;
   Efl_VG *root;
   Svg_Entry *svg;
   Eina_Rectangle fill;

   unsigned int width, height;

   Eina_Array cleanup;
   Eina_Bool content_changed;
   void *backing_store;

   Vg_Cache_Entry            *vg_entry;
   int                        frame_index;
   Eina_File                 *file;
   Eina_Stringshare          *key;

   int magic;               //private data for safety.
};

void                        evas_cache_vg_init(void);
void                        evas_cache_vg_shutdown(void);
Vg_Cache_Entry*             evas_cache_vg_entry_resize(Vg_Cache_Entry *entry, int w, int h);
void                        evas_cache_vg_entry_del(Vg_Cache_Entry *vg_entry);
Vg_Cache_Entry*             evas_cache_vg_entry_create(const Eina_File *file, const char *key, int w, int h, Eina_Bool mmap);
Efl_VG*                     evas_cache_vg_tree_get(Vg_Cache_Entry *vg_entry, unsigned int frame_num);
double                      evas_cache_vg_anim_duration_get(const Vg_Cache_Entry *vg_entry);
unsigned int                evas_cache_vg_anim_frame_count_get(const Vg_Cache_Entry *vg_entry);
void                        evas_cache_vg_entry_default_size_get(const Vg_Cache_Entry *vg_entry, int *w, int *h);

struct _Efl_VG_Base_Data
{
   const char *name;

   Eina_Matrix3 *m;
   Efl_VG_Interpolation *intp;

   Ector_Renderer *renderer;

   void (*render_pre)(Evas_Object_Protected_Data *vg_pd, Efl_VG *node,
                      Efl_VG_Base_Data *nd, Ector_Surface *surface,
                      Eina_Matrix3 *ptransform, Ector_Buffer *mask, int mask_op, void *data);
   void *data;

   double x, y;
   int r, g, b, a;
   Efl_Gfx_Change_Flag flags;

   Eina_Bool visibility : 1;
   Eina_Bool changed : 1;
};

typedef struct _Eina_Rect
{
   int x, y, w, h;

} Eina_Rect;

typedef struct _Vg_Mask
{
   Evas_Object_Protected_Data *vg_pd;  //Vector Object (for accessing backend engine)
   Ector_Buffer *buffer;               //Mask Ector Buffer
   void *pixels;                       //Mask pixel buffer (actual data)
   Eina_Rect bound;                    //Mask boundary
   Eina_List *target;                  //Mask target
   int option;                         //Mask option
   Eina_Bool dirty : 1;                //Need to update mask image.
} Vg_Mask;

struct _Efl_VG_Container_Data
{
   Eina_List *children;
   Eina_Hash *names;

   //Masking feature.
   Efl_VG_Base *mask_src;         //Mask Source
   Vg_Mask mask;                  //Mask source data
};

struct _Efl_VG_Gradient_Data
{
   // FIXME: Later on we should deduplicate it somehow (Using Ector ?).
   Efl_Gfx_Gradient_Stop *colors;
   unsigned int colors_count;

   Efl_Gfx_Gradient_Spread s;
};

struct _Efl_VG_Interpolation
{
   Eina_Quaternion rotation;
   Eina_Quaternion perspective;
   Eina_Point_3D translation;
   Eina_Point_3D scale;
   Eina_Point_3D skew;
};

static inline Efl_VG_Base_Data *
_evas_vg_render_pre(Evas_Object_Protected_Data *vg_pd, Efl_VG *child, Ector_Surface *surface, Eina_Matrix3 *transform, Ector_Buffer *mask, int mask_op)
{
   if (!child) return NULL;
   Efl_VG_Base_Data *nd = eo_data_scope_get(child, EFL_VG_BASE_CLASS);
   if (nd) nd->render_pre(vg_pd, child, nd, surface, transform, mask, mask_op, nd->data);
   return nd;
}

static inline void
_efl_vg_base_changed(Eo *obj)
{
   eo_do(obj, eo_event_callback_call(EFL_GFX_CHANGED, NULL));
}

static inline void *
_efl_vg_realloc(void *from, unsigned int sz)
{
   void *result;

   result = sz > 0 ? realloc(from, sz) : NULL;
   if (!result) free(from);

   return result;
}

static inline void
_efl_vg_clean_object(Eo **obj)
{
   if (*obj) eo_unref(*obj);
   *obj = NULL;
}

#define EFL_VG_COMPUTE_MATRIX(Current, Parent, Nd)                      \
  Eina_Matrix3 *Current = Nd->m;                                        \
  Eina_Matrix3 _matrix_tmp;                                             \
                                                                        \
  if (Parent)                                                           \
    {                                                                   \
       if (Current)                                                     \
         {                                                              \
            eina_matrix3_compose(Parent, Current, &_matrix_tmp);        \
            Current = &_matrix_tmp;                                     \
         }                                                              \
       else                                                             \
         {                                                              \
            eina_matrix3_identity(&_matrix_tmp);                        \
            eina_matrix3_translate(&_matrix_tmp, -(Nd->x), -(Nd->y));   \
            eina_matrix3_compose(Parent, &_matrix_tmp, &_matrix_tmp);   \
            eina_matrix3_translate(&_matrix_tmp, (Nd->x), (Nd->y));     \
            Current = &_matrix_tmp;                                     \
         }                                                              \
    }


#endif
