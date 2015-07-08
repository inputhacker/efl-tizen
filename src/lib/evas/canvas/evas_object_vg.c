#include "evas_common_private.h"
#include "evas_private.h"

#include "evas_vg_private.h"
#include "evas_vg_root_node.eo.h"

#define MY_CLASS EVAS_VG_CLASS

/* private magic number for rectangle objects */
static const char o_type[] = "vectors";

const char *o_vg_type = o_type;

/* private struct for rectangle object internal data */
typedef struct _Evas_VG_Data      Evas_VG_Data;

struct _Evas_VG_Data
{
   void         *engine_data;
   Evas_VG_Node *root;

   /* Opening an SVG file (could actually be inside an eet section */
   Eina_File    *f;
   const char   *key;

   Eina_Rectangle fill;

   unsigned int width, height;
};

static void evas_object_vg_render(Evas_Object *eo_obj,
                                  Evas_Object_Protected_Data *obj,
                                  void *type_private_data,
                                  void *output, void *context, void *surface,
                                  int x, int y, Eina_Bool do_async);
static void evas_object_vg_render_pre(Evas_Object *eo_obj,
                                      Evas_Object_Protected_Data *obj,
                                      void *type_private_data);
static void evas_object_vg_render_post(Evas_Object *eo_obj,
                                       Evas_Object_Protected_Data *obj,
                                       void *type_private_data);
static unsigned int evas_object_vg_id_get(Evas_Object *eo_obj);
static unsigned int evas_object_vg_visual_id_get(Evas_Object *eo_obj);
static void *evas_object_vg_engine_data_get(Evas_Object *eo_obj);
static int evas_object_vg_is_opaque(Evas_Object *eo_obj,
                                    Evas_Object_Protected_Data *obj,
                                    void *type_private_data);
static int evas_object_vg_was_opaque(Evas_Object *eo_obj,
                                     Evas_Object_Protected_Data *obj,
                                     void *type_private_data);

static const Evas_Object_Func object_func =
{
   /* methods (compulsory) */
   NULL,
     evas_object_vg_render,
     evas_object_vg_render_pre,
     evas_object_vg_render_post,
     evas_object_vg_id_get,
     evas_object_vg_visual_id_get,
     evas_object_vg_engine_data_get,
   /* these are optional. NULL = nothing */
     NULL,
     NULL,
     NULL,
     NULL,
     evas_object_vg_is_opaque,
     evas_object_vg_was_opaque,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL
};

/* the actual api call to add a vector graphic object */
EAPI Evas_Object *
evas_object_vg_add(Evas *e)
{
   MAGIC_CHECK(e, Evas, MAGIC_EVAS);
   return NULL;
   MAGIC_CHECK_END();
   Evas_Object *eo_obj = eo_add(MY_CLASS, e);

   // Ask backend to return the main Ector_Surface

   return eo_obj;
}

Evas_VG_Node *
_evas_vg_root_node_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd)
{
   return pd->root;
}

void
_evas_vg_eo_base_destructor(Eo *eo_obj, Evas_VG_Data *pd)
{
   eo_unref(pd->root);
   eo_do_super(eo_obj, MY_CLASS, eo_destructor());
}

void
_evas_vg_eo_base_constructor(Eo *eo_obj, Evas_VG_Data *pd)
{
   Evas_Object_Protected_Data *obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);
   Eo *parent = NULL;

   eo_do_super(eo_obj, MY_CLASS, eo_constructor());

   /* set up methods (compulsory) */
   obj->func = &object_func;
   obj->private_data = eo_data_ref(eo_obj, MY_CLASS);
   obj->type = o_type;

   /* root node */
   pd->root = eo_add(EVAS_VG_ROOT_NODE_CLASS, eo_obj);
   eo_ref(pd->root);

   eo_do(eo_obj, parent = eo_parent_get());
   evas_object_inject(eo_obj, obj, evas_object_evas_get(parent));
}

static void
_evas_vg_render(Evas_Object_Protected_Data *obj,
                void *output, void *context, void *surface, Evas_VG_Node *node,
                Eina_Array *clips, int x, int y, Eina_Bool do_async)
{
    // FIXME if the class is not container ,
    // some times its returning garbage container data.
    if (eo_isa(node, EVAS_VG_CONTAINER_CLASS))
     {
        Evas_VG_Container_Data *cd = eo_data_scope_get(node,
                                                       EVAS_VG_CONTAINER_CLASS);
        Evas_VG_Node *child;
        Eina_List *l;

        EINA_LIST_FOREACH(cd->children, l, child)
           _evas_vg_render(obj, output, context, surface, child, clips, x, y,
                           do_async);
     }
   else
     {
        Evas_VG_Node_Data *nd = eo_data_scope_get(node, EVAS_VG_NODE_CLASS);
        obj->layer->evas->engine.func->ector_draw(output, context, surface,
                                                  nd->renderer, clips, x, y,
                                                  do_async);
     }
}

static void
evas_object_vg_render(Evas_Object *eo_obj EINA_UNUSED,
                      Evas_Object_Protected_Data *obj,
                      void *type_private_data,
                      void *output, void *context, void *surface,
                      int x, int y, Eina_Bool do_async)
{
   Evas_VG_Data *vd = type_private_data;

   // FIXME: Set context (that should affect Ector_Surface) and
   // then call Ector_Renderer render from bottom to top. Get the
   // Ector_Surface that match the output from Evas engine API.
   // It is a requirement that you can reparent an Ector_Renderer
   // to another Ector_Surface as long as that Ector_Surface is a
   // child of the main Ector_Surface (necessary for Evas_Map).

   /* render object to surface with context, and offxet by x,y */
   obj->layer->evas->engine.func->context_color_set(output,
                                                    context,
                                                    obj->cur->cache.clip.r,
                                                    obj->cur->cache.clip.g,
                                                    obj->cur->cache.clip.b,
                                                    obj->cur->cache.clip.a);
   obj->layer->evas->engine.func->context_anti_alias_set(output, context,
                                                         obj->cur->anti_alias);
   obj->layer->evas->engine.func->context_multiplier_unset(output,
                                                           context);
   obj->layer->evas->engine.func->context_render_op_set(output, context,
                                                        obj->cur->render_op);
   _evas_vg_render(obj, output, context, surface, vd->root, NULL,
                   obj->cur->geometry.x + x, obj->cur->geometry.y + y,
                   do_async);
}

static void
evas_object_vg_render_pre(Evas_Object *eo_obj,
                          Evas_Object_Protected_Data *obj,
                          void *type_private_data)
{
   Evas_VG_Data *vd = type_private_data;
   Evas_Public_Data *e = obj->layer->evas;
   int is_v, was_v;
   Ector_Surface *surface;
   Eina_Bool change = EINA_FALSE;

   /* dont pre-render the obj twice! */
   if (obj->pre_render_done) return;
   obj->pre_render_done = EINA_TRUE;
   /* pre-render phase. this does anything an object needs to do just before */
   /* rendering. this could mean loading the image data, retrieving it from */
   /* elsewhere, decoding video etc. */
   /* then when this is done the object needs to figure if it changed and */
   /* if so what and where and add the appropriate redraw rectangles */
   /* if someone is clipping this obj - go calculate the clipper */
   if (obj->cur->clipper)
     {
        if (obj->cur->cache.clip.dirty)
          evas_object_clip_recalc(obj->cur->clipper);
        obj->cur->clipper->func->render_pre(obj->cur->clipper->object,
                                            obj->cur->clipper,
                                            obj->cur->clipper->private_data);
     }
   /* now figure what changed and add draw rects */
   /* if it just became visible or invisible */
   is_v = evas_object_is_visible(eo_obj, obj);
   was_v = evas_object_was_visible(eo_obj,obj);
   if (!(is_v | was_v)) goto done;

   // FIXME: handle damage only on changed renderer.
   surface = e->engine.func->ector_get(e->engine.data.output);
   if (surface)
     change = _evas_vg_render_pre(vd->root, surface, NULL);

   if (change)
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes,
                                            eo_obj, obj);
        goto done;
     }

   if (is_v != was_v)
     {
        evas_object_render_pre_visible_change(&obj->layer->evas->clip_changes,
                                              eo_obj, is_v, was_v);
        goto done;
     }

   if (obj->changed_map || obj->changed_src_visible)
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes,
                                            eo_obj, obj);
        goto done;
     }
   /* it's not visible - we accounted for it appearing or not so just abort */
   if (!is_v) goto done;
   /* clipper changed this is in addition to anything else for obj */
   evas_object_render_pre_clipper_change(&obj->layer->evas->clip_changes,
                                         eo_obj);
   /* if we restacked (layer or just within a layer) and don't clip anyone */
   if ((obj->restack) && (!obj->clip.clipees))
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes,
                                            eo_obj, obj);
        goto done;
     }
   /* if it changed render op */
   if (obj->cur->render_op != obj->prev->render_op)
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes,
                                            eo_obj, obj);
        goto done;
     }
   /* if it changed color */
   if ((obj->cur->color.r != obj->prev->color.r) ||
       (obj->cur->color.g != obj->prev->color.g) ||
       (obj->cur->color.b != obj->prev->color.b) ||
       (obj->cur->color.a != obj->prev->color.a))
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes,
                                            eo_obj, obj);
        goto done;
     }
   /* if it changed geometry - and obviously not visibility or color */
   /* calculate differences since we have a constant color fill */
   /* we really only need to update the differences */
   if ((obj->cur->geometry.x != obj->prev->geometry.x) ||
       (obj->cur->geometry.y != obj->prev->geometry.y) ||
       (obj->cur->geometry.w != obj->prev->geometry.w) ||
       (obj->cur->geometry.h != obj->prev->geometry.h))
     {
        evas_rects_return_difference_rects(&obj->layer->evas->clip_changes,
                                           obj->cur->geometry.x,
                                           obj->cur->geometry.y,
                                           obj->cur->geometry.w,
                                           obj->cur->geometry.h,
                                           obj->prev->geometry.x,
                                           obj->prev->geometry.y,
                                           obj->prev->geometry.w,
                                           obj->prev->geometry.h);
        goto done;
     }
   /* it obviously didn't change - add a NO obscure - this "unupdates"  this */
   /* area so if there were updates for it they get wiped. don't do it if we */
   /* arent fully opaque and we are visible */
   if (evas_object_is_visible(eo_obj, obj) &&
       evas_object_is_opaque(eo_obj, obj) &&
       (!obj->clip.clipees))
     {
        Evas_Coord x, y, w, h;

        x = obj->cur->cache.clip.x;
        y = obj->cur->cache.clip.y;
        w = obj->cur->cache.clip.w;
        h = obj->cur->cache.clip.h;
        if (obj->cur->clipper)
          {
             RECTS_CLIP_TO_RECT(x, y, w, h,
                                obj->cur->clipper->cur->cache.clip.x,
                                obj->cur->clipper->cur->cache.clip.y,
                                obj->cur->clipper->cur->cache.clip.w,
                                obj->cur->clipper->cur->cache.clip.h);
          }
        obj->layer->evas->engine.func->output_redraws_rect_del
        (obj->layer->evas->engine.data.output,
         x + obj->layer->evas->framespace.x,
         y + obj->layer->evas->framespace.y,
         w, h);
     }
done:
   evas_object_render_pre_effect_updates(&obj->layer->evas->clip_changes,
                                         eo_obj, is_v, was_v);
}

static void
evas_object_vg_render_post(Evas_Object *eo_obj,
                           Evas_Object_Protected_Data *obj EINA_UNUSED,
                           void *type_private_data EINA_UNUSED)
{
   /* this moves the current data to the previous state parts of the object */
   /* in whatever way is safest for the object. also if we don't need object */
   /* data anymore we can free it if the object deems this is a good idea */
   /* remove those pesky changes */
   evas_object_clip_changes_clean(eo_obj);
   /* move cur to prev safely for object data */
   evas_object_cur_prev(eo_obj);
}

static unsigned int
evas_object_vg_id_get(Evas_Object *eo_obj)
{
   Evas_VG_Data *o = eo_data_scope_get(eo_obj, MY_CLASS);
   if (!o) return 0;
   return MAGIC_OBJ_VG;
}

static unsigned int
evas_object_vg_visual_id_get(Evas_Object *eo_obj)
{
   Evas_VG_Data *o = eo_data_scope_get(eo_obj, MY_CLASS);
   if (!o) return 0;
   return MAGIC_OBJ_SHAPE;
}

static void *
evas_object_vg_engine_data_get(Evas_Object *eo_obj)
{
   Evas_VG_Data *o = eo_data_scope_get(eo_obj, MY_CLASS);
   return o->engine_data;
}

static int
evas_object_vg_is_opaque(Evas_Object *eo_obj EINA_UNUSED,
                         Evas_Object_Protected_Data *obj EINA_UNUSED,
                         void *type_private_data EINA_UNUSED)
{
   return 0;
}

static int
evas_object_vg_was_opaque(Evas_Object *eo_obj EINA_UNUSED,
                          Evas_Object_Protected_Data *obj EINA_UNUSED,
                          void *type_private_data EINA_UNUSED)
{
   return 0;
}


static Eina_Bool
_evas_vg_mmap_set(Eo *obj, Evas_VG_Data *pd,
                  const Eina_File *f, const char *key)
{
   Eina_File *tmp;

   if (f == pd->f &&
       ((key == NULL && pd->key == NULL) ||
        (key != NULL && pd->key != NULL && !strcmp(key, pd->key))))
     return EINA_TRUE;

   tmp = f ? eina_file_dup(f) : NULL;

   if (tmp)
     {
        if (!evas_vg_loader_svg(obj, tmp, NULL))
          {
             eina_file_close(tmp);
             return EINA_FALSE;
          }
     }

   // it succeeded.
   if (pd->f) eina_file_close(pd->f);
   pd->f = tmp;
   eina_stringshare_replace(&pd->key, key);

   return EINA_TRUE;
}

static void
_evas_vg_mmap_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                  const Eina_File **f, const char **key)
{
   if (f) *f = pd->f;
   if (key) *key = pd->key;
}

Eina_Bool
_evas_vg_efl_file_file_set(Eo *obj, Evas_VG_Data *pd EINA_UNUSED,
                           const char *file, const char *key)
{
   Eina_File *f;
   Eina_Bool r = EINA_FALSE;

   f = eina_file_open(file, EINA_FALSE);
   if (!f) return EINA_FALSE;

   eo_do(obj, r = evas_obj_vg_mmap_set(f, key));

   eina_file_close(f);
   return r;
}

void
_evas_vg_efl_file_file_get(Eo *obj, Evas_VG_Data *pd EINA_UNUSED,
                           const char **file, const char **key)
{
   const Eina_File *f = NULL;

   eo_do(obj, evas_obj_vg_mmap_get(&f, key));

   if (file) *file = eina_file_filename_get(f);
}

void
_evas_vg_size_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                  unsigned int *w, unsigned int *h)
{
   if (w) *w = pd->width;
   if (h) *h = pd->height;
}

void
_evas_vg_size_set(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                  unsigned int w, unsigned int h)
{
   pd->width = w;
   pd->height = h;
}

void
_evas_vg_fill_set(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                  Evas_Coord x, Evas_Coord y, Evas_Coord w, Evas_Coord h)
{
   pd->fill.x = x;
   pd->fill.y = y;
   pd->fill.w = w;
   pd->fill.h = h;
}

void
_evas_vg_fill_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                  Evas_Coord *x, Evas_Coord *y, Evas_Coord *w, Evas_Coord *h)
{
   if (x) *x = pd->fill.x;
   if (y) *y = pd->fill.y;
   if (w) *w = pd->fill.w;
   if (h) *h = pd->fill.h;
}

#include "evas_vg.eo.c"
