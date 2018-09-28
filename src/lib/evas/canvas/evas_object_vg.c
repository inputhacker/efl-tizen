#include "evas_common_private.h"
#include "evas_private.h"

#include "evas_vg_private.h"
#include "efl_vg_root_node.eo.h"

#include "evas_vg_cache.h"

#define MY_CLASS EVAS_VG_CLASS

/* private magic number for rectangle objects */
static const char o_type[] = "vectors";

const char *o_vg_type = o_type;

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

EAPI int
evas_object_vg_animated_frame_get(const Evas_Object *obj)
{
   if (!obj) return 0;
   Evas_VG_Data *pd = eo_data_scope_get(obj, MY_CLASS);
   if (!pd) return 0;

   return pd->frame_index;
}

EAPI double
evas_object_vg_animated_frame_duration_get(const Evas_Object *obj, int start_frame, int frame_num)
{
   if (!obj) return 0;
   Evas_VG_Data *pd = eo_data_scope_get(obj, MY_CLASS);
   if (!pd) return 0;

   if (!pd->vg_entry) return 0;
   return evas_cache_vg_anim_duration_get(pd->vg_entry);
}

EAPI int
evas_object_vg_animated_frame_count_get(const Evas_Object *obj)
{
   if (!obj) return 0;
   Evas_VG_Data *pd = eo_data_scope_get(obj, MY_CLASS);
   if (!pd) return 0;

   if (!pd->vg_entry) return 0;
   return evas_cache_vg_anim_frame_count_get(pd->vg_entry);
}

EAPI Eina_Bool
evas_object_vg_animated_frame_set(Evas_Object *obj, int frame_index)
{
   if (!obj) return 0;
   Evas_VG_Data *pd = eo_data_scope_get(obj, MY_CLASS);
   if (!pd) return 0;

   //TODO: Validate frame_index range

   if (pd->frame_index == frame_index) return EINA_TRUE;

   pd->frame_index = frame_index;
   evas_object_change(obj, eo_data_scope_get(obj, EVAS_OBJECT_CLASS));

   return EINA_TRUE;
}

static Eina_Bool
_vg_file_mmap_set(Eo *eo_obj, Evas_VG_Data *pd, const Eina_File *file, const char *key, Eina_Bool mmap)
{
   Vg_Cache_Entry *old_entry;
   Evas_Object_Protected_Data *obj;

   obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);

   old_entry = pd->vg_entry;

   if (file)
     pd->vg_entry = evas_cache_vg_entry_create(file, key,
                                               obj->cur->geometry.w,
                                               obj->cur->geometry.h,
                                               mmap);
   else
     pd->vg_entry = NULL;

   evas_object_change(eo_obj, obj);
   evas_cache_vg_entry_del(old_entry);

   return EINA_TRUE;
}

EAPI Eina_Bool
evas_object_vg_mmap_set(Evas_Object *eo_obj, const Eina_File *file, const char *key)
{
   Evas_VG_Data *pd = eo_data_scope_get(eo_obj, MY_CLASS);

   Eina_File *pf = pd->file;
   Eina_Bool ret;

   if (!file)
     {
        Evas_Object_Protected_Data *obj;
        obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);
        evas_cache_vg_entry_del(pd->vg_entry);
        evas_object_change(eo_obj, obj);
        eina_stringshare_del(pd->key);
        pd->vg_entry = NULL;
        pd->file = NULL;
        pd->key = NULL;
        return EINA_TRUE;
     }
   if (pd->file == file)
     {
        if (!pd->key && !key) return EINA_FALSE;
        else if (pd->key && key)
          {
             if (!strcmp(pd->key, key)) return EINA_FALSE;
          }
        pf = NULL;
     }
   if (pd->file != file)
     pd->file = eina_file_dup(file);
   ret = _vg_file_mmap_set(eo_obj, pd, file, key, EINA_TRUE);

   //Close previous file after deleting ex-cache entry.
   if (pf) eina_file_close(pf);

   return ret;
}

EAPI Eina_Bool
evas_object_vg_file_set(Evas_Object *eo_obj, const char *file, const char *key)
{
   Evas_VG_Data *pd = eo_data_scope_get(eo_obj, MY_CLASS);

   Eina_File *pf = pd->file;
   Eina_Bool ret;

   if (!file)
     {
        Evas_Object_Protected_Data *obj;
        obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);
        evas_cache_vg_entry_del(pd->vg_entry);
        evas_object_change(eo_obj, obj);
        eina_stringshare_del(pd->key);
        pd->vg_entry = NULL;
        pd->file = NULL;
        pd->key = NULL;
        return EINA_TRUE;
     }
   if (!pd->file)
     {
        pd->file = eina_file_open(file, EINA_FALSE);
        if (!pd->file) return EINA_FALSE;
     }
   else
     {
        const char *filename = eina_file_filename_get(pd->file);
        if (filename)
          {
             if (strcmp(filename, file))
               pd->file = eina_file_open(file, EINA_FALSE);
             else
               {
                  if (!pd->key && !key) return EINA_FALSE;
                  else if (pd->key && key)
                    {
                       if (!strcmp(pd->key, key)) return EINA_FALSE;
                    }
                  pf = NULL;
               }
          }
     }

   ret = _vg_file_mmap_set(eo_obj, pd, pd->file, key, EINA_FALSE);

   //Close previous file after deleting ex-cache entry.
   if (pf) eina_file_close(pf);

   return ret;
}

EAPI void
evas_object_vg_path_set(Eo *obj, const char *path, int src_vg,
                        int dest_vg, float pos)
{
   int w, h;
   Evas_VG_Data *pd;
   Svg_Entry *entry;

   if (!obj) return ;

   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   pd = eo_data_scope_get(obj, MY_CLASS);
   entry = evas_cache_svg_find(path, src_vg, dest_vg, pos, w, h);
   if (entry != pd->svg)
     {
        if (pd->svg)
          {
             evas_cache_svg_entry_del(pd->svg);
          }
        pd->svg = entry;
     }
   evas_object_change(obj, eo_data_scope_get(obj, EVAS_OBJECT_CLASS));
}

Efl_VG *
_evas_vg_root_node_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd)
{
   return pd->root;
}

static Eina_Bool
_cleanup_reference(void *data,
                   Eo *obj EINA_UNUSED,
                   const Eo_Event_Description *desc EINA_UNUSED,
                   void *event_info EINA_UNUSED)
{
   Evas_VG_Data *pd = data;
   Eo *renderer;

   /* unref all renderer and may also destroy them async */
   while ((renderer = eina_array_pop(&pd->cleanup)))
     eo_unref(renderer);

   return EO_CALLBACK_CONTINUE;
}

void
_evas_vg_eo_base_destructor(Eo *eo_obj, Evas_VG_Data *pd)
{
   if (pd->backing_store) {
      Evas_Object_Protected_Data *obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);
      obj->layer->evas->engine.func->image_free(obj->layer->evas->engine.data.output,
                                                  pd->backing_store);
   }
   Evas *e = evas_object_evas_get(eo_obj);

   eo_do(e, eo_event_callback_del(EVAS_CANVAS_EVENT_RENDER_POST, _cleanup_reference, pd));

   eo_unref(pd->root);
   pd->root = NULL;

   evas_cache_vg_entry_del(pd->vg_entry);

   eo_do_super(eo_obj, MY_CLASS, eo_destructor());
}

Eo *
_evas_vg_eo_base_constructor(Eo *eo_obj, Evas_VG_Data *pd)
{
   Evas_Object_Protected_Data *obj = eo_data_scope_get(eo_obj, EVAS_OBJECT_CLASS);

   eo_obj = eo_do_super_ret(eo_obj, MY_CLASS, eo_obj, eo_constructor());

   /* set up methods (compulsory) */
   obj->func = &object_func;
   obj->private_data = eo_data_ref(eo_obj, MY_CLASS);
   obj->type = o_type;

   /* root node */
   pd->root = eo_add(EFL_VG_ROOT_NODE_CLASS, eo_obj);
   eo_ref(pd->root);

   eina_array_step_set(&pd->cleanup, sizeof(pd->cleanup), 8);

   return eo_obj;
}

static Eo_Base *
_evas_vg_eo_base_finalize(Eo *obj, Evas_VG_Data *pd)
{
   Evas *e = evas_object_evas_get(obj);

   // TODO: If we start to have to many Evas_Object_VG per canvas, it may be nice
   // to actually have one event per canvas and one array per canvas to.
   eo_do(e, eo_event_callback_add(EVAS_CANVAS_EVENT_RENDER_POST, _cleanup_reference, pd));

   return obj;
}

static void
_evas_vg_render(Evas_Object_Protected_Data *obj, Evas_VG_Data *vd,
                void *output, void *context, void *surface, Efl_VG *n,
                Eina_Array *clips, Eina_Bool do_async)
{
   if (eo_isa(n, EFL_VG_CONTAINER_CLASS))
     {
        Efl_VG_Container_Data *vc;
        Efl_VG *child;
        Eina_List *l;

        vc = eo_data_scope_get(n, EFL_VG_CONTAINER_CLASS);

        EINA_LIST_FOREACH(vc->children, l, child)
          _evas_vg_render(obj, vd,
                          output, context, surface, child,
                          clips, do_async);
     }
   else
     {
        Efl_VG_Base_Data *nd;

        nd = eo_data_scope_get(n, EFL_VG_BASE_CLASS);

        obj->layer->evas->engine.func->ector_renderer_draw(output, context, surface, nd->renderer, clips, do_async);

        if (do_async)
          eina_array_push(&vd->cleanup, eo_ref(nd->renderer));
     }
}

// renders a vg_tree to an offscreen buffer
// and push it to the cache.
static void *
_render_to_buffer(Evas_Object_Protected_Data *obj, Evas_VG_Data *pd,
                  void *output, void *surface EINA_UNUSED,
                  Efl_VG *root, int w, int h, void *key,
                  void *buffer, Eina_Bool do_async)
{
   Ector_Surface *ector;
   RGBA_Draw_Context *context;
   int error = 0;
   Eina_Bool buffer_created = EINA_FALSE;

   ector = evas_ector_get(obj->layer->evas);
   if (!ector) return NULL;

   if (!buffer)
     {
        // 2. create a buffer
        buffer = obj->layer->evas->engine.func->ector_surface_create(output,
                                                                     NULL,
                                                                     w, h,
                                                                     EINA_TRUE, &error);
        if (error) return NULL; // surface creation error
        buffer_created = EINA_TRUE;
     }

   //1. render pre
   _evas_vg_render_pre(root, ector, NULL);

   //3. draw into the buffer
   context = evas_common_draw_context_new();
   evas_common_draw_context_set_render_op(context, _EVAS_RENDER_COPY);
   evas_common_draw_context_set_color(context, 255, 255, 255, 255);
   obj->layer->evas->engine.func->ector_begin(output, context, ector, buffer, EINA_TRUE,
                                              0, 0, w, h,
                                              do_async);
   _evas_vg_render(obj, pd,
                   output, context, buffer,
                   root, NULL,
                   do_async);

   obj->layer->evas->engine.func->image_dirty_region(output, buffer, 0, 0, w, h);
   obj->layer->evas->engine.func->ector_end(output, context,
                                            ector, buffer,
                                            do_async);

   evas_common_draw_context_free(context);

   if (buffer_created)
     obj->layer->evas->engine.func->ector_surface_cache_set(output, key, buffer);

   return buffer;
}

static void
_render_buffer_to_screen(Evas_Object_Protected_Data *obj,
                         void *output, void *context, void *surface,
                         void *buffer,
                         int x, int y, int w, int h,
                         Eina_Bool do_async)
{
   Eina_Bool async_unref;

   // draw the buffer as image to canvas
   async_unref = obj->layer->evas->engine.func->image_draw(output, context, surface,
                                                           buffer,
                                                           0, 0, w, h,
                                                           x, y, w, h,
                                                           EINA_TRUE, do_async);
   if (do_async && async_unref)
     {
        evas_cache_image_ref((Image_Entry *)buffer);
        evas_unref_queue_image_put(obj->layer->evas, buffer);
     }
}

static void
_cache_vg_entry_render(Evas_Object_Protected_Data *obj,
                       Evas_VG_Data *pd,
                       void *output, void *context, void *surface,
                       int x, int y, int w, int h, Eina_Bool do_async)
{
   Vg_Cache_Entry *vg_entry = pd->vg_entry;
   Efl_VG *root;
   void *buffer;

   // if the size changed in between path set and the draw call;
   if ((vg_entry->w != w) ||
       (vg_entry->h != h))
     {
         vg_entry = evas_cache_vg_entry_resize(vg_entry, w, h);
         evas_cache_vg_entry_del(pd->vg_entry);
         pd->vg_entry = vg_entry;
     }
   root = evas_cache_vg_tree_get(vg_entry, pd->frame_index);
   if (!root) return;
   buffer = obj->layer->evas->engine.func->ector_surface_cache_get(output, root);

   // if the buffer is not created yet
   if (!buffer)
     {
        // render to the buffer
        buffer = _render_to_buffer(obj, pd,
                                   output, surface,
                                   root,
                                   w, h,
                                   root,
                                   buffer,
                                   do_async);
     }
//will be flushed if cache is full.
//   else
//     obj->layer->evas->engine.func->ector_surface_cache_drop(output, root);

   _render_buffer_to_screen(obj,
                            output, context, surface,
                            buffer,
                            x, y, w, h,
                            do_async);
}

static void
_svg_data_render(Evas_Object_Protected_Data *obj,
                 Evas_VG_Data *vd,
                 void *output, void *context, void *surface,
                 int x, int y, Eina_Bool do_async)
{
   Svg_Entry *svg = vd->svg;
   Efl_VG *root, *dupe_root;
   void *buffer;
   Ector_Surface *ector;
   RGBA_Draw_Context *ct;
   Eina_Bool async_unref;
   Eina_Bool created = EINA_FALSE;
   int error = 0;

   // if the size changed in between path set and the draw call;
   if (!(svg->w == obj->cur->geometry.w &&
         svg->h == obj->cur->geometry.h))
     {
         evas_cache_svg_entry_del(svg);
         svg = evas_cache_svg_find(svg->file, svg->src_vg, svg->dest_vg,
                                   svg->key_frame, obj->cur->geometry.w, obj->cur->geometry.h);
         vd->svg = svg;
     }
   // if the buffer is not created yet
   root = evas_cache_svg_vg_tree_get(svg);
   if (!root) return;
   buffer = obj->layer->evas->engine.func->ector_surface_cache_get(output, root);
   if (!buffer)
     {
        // manual render the vg tree
        ector = evas_ector_get(obj->layer->evas);
        if (!ector) return;
        dupe_root = evas_vg_container_add(NULL);
        evas_vg_node_dup(dupe_root, root);
        //1. render pre
        _evas_vg_render_pre(dupe_root, ector, NULL);
        // 2. create surface
        buffer = obj->layer->evas->engine.func->ector_surface_create(output,
                                                                     NULL,
                                                                     svg->w,
                                                                     svg->h,
                                                                     EINA_TRUE, &error);
        if (error)
          return; // surface creation error

        //3. draw into the buffer
        ct = evas_common_draw_context_new();
        evas_common_draw_context_set_render_op(ct, _EVAS_RENDER_COPY);
        evas_common_draw_context_set_color(ct, 255, 255, 255, 255);
        obj->layer->evas->engine.func->ector_begin(output, ct,
                                                   ector, buffer, EINA_TRUE,
                                                   0, 0,
                                                   obj->cur->geometry.w, obj->cur->geometry.h,
                                                   do_async);
        _evas_vg_render(obj, vd,
                        output, ct, buffer,
                        dupe_root, NULL,
                        do_async);
        obj->layer->evas->engine.func->image_dirty_region(output, buffer, 0, 0, 0, 0);
        obj->layer->evas->engine.func->ector_end(output, ct, ector, buffer, do_async);

        evas_common_draw_context_free(ct);
        eo_del(dupe_root);
        created = EINA_TRUE;
     }
   // draw the buffer as image to canvas

   async_unref = obj->layer->evas->engine.func->image_draw(output, context, surface,
                                             buffer, 0, 0,
                                             obj->cur->geometry.w, obj->cur->geometry.h, obj->cur->geometry.x + x,
                                             obj->cur->geometry.y + y, obj->cur->geometry.w, obj->cur->geometry.h,
                                             EINA_TRUE, do_async);
   if (do_async && async_unref)
     {
        evas_cache_image_ref((Image_Entry *)buffer);
        evas_unref_queue_image_put(obj->layer->evas, buffer);
     }

   if (created)
     obj->layer->evas->engine.func->ector_surface_cache_set(output, root, buffer);
}

static void
evas_object_vg_render(Evas_Object *eo_obj EINA_UNUSED,
                      Evas_Object_Protected_Data *obj,
                      void *type_private_data,
                      void *output, void *context, void *surface,
                      int x, int y, Eina_Bool do_async)
{
   Evas_VG_Data *vd = type_private_data;
   Ector_Surface *ector = evas_ector_get(obj->layer->evas);
   int error = 0;

   // FIXME: Set context (that should affect Ector_Surface) and
   // then call Ector_Renderer render from bottom to top. Get the
   // Ector_Surface that match the output from Evas engine API.
   // It is a requirement that you can reparent an Ector_Renderer
   // to another Ector_Surface as long as that Ector_Surface is a
   // child of the main Ector_Surface (necessary for Evas_Map).

   /* render object to surface with context, and offxet by x,y */
   obj->layer->evas->engine.func->context_color_set(output,
                                                    context,
                                                    255,
                                                    255,
                                                    255,
                                                    255);
   obj->layer->evas->engine.func->context_multiplier_set(output,
                                                         context,
                                                         obj->cur->cache.clip.r,
                                                         obj->cur->cache.clip.g,
                                                         obj->cur->cache.clip.b,
                                                         obj->cur->cache.clip.a);
   obj->layer->evas->engine.func->context_anti_alias_set(output, context,
                                                         obj->cur->anti_alias);
   obj->layer->evas->engine.func->context_render_op_set(output, context,
                                                        obj->cur->render_op);

   if (vd->vg_entry)
     {
        _cache_vg_entry_render(obj, vd,
                               output, context, surface,
                               obj->cur->geometry.x + x, obj->cur->geometry.y + y,
                               obj->cur->geometry.w, obj->cur->geometry.h, do_async);
        return;
     }

   if (vd->svg)
     {
        _svg_data_render(obj, vd, output,
                         context, surface,
                         x, y, do_async);
        return;
     }

   if (vd->content_changed || !vd->backing_store)
     vd->backing_store = obj->layer->evas->engine.func->ector_surface_create(output,
                                                                             vd->backing_store,
                                                                             obj->cur->geometry.w,
                                                                             obj->cur->geometry.h,
                                                                             EINA_FALSE, &error);
   if (error)
     return; // surface creation error

   if (!vd->backing_store)
     {
        obj->layer->evas->engine.func->ector_begin(output, context, ector, surface, EINA_FALSE,
                                                   obj->cur->geometry.x + x, obj->cur->geometry.y + y,
                                                   obj->cur->geometry.w, obj->cur->geometry.h,
                                                   do_async);
        _evas_vg_render(obj, vd, output, context, surface, vd->root, NULL, do_async);
        obj->layer->evas->engine.func->ector_end(output, context, ector, surface, do_async);
     }
   else
     {
        if (vd->content_changed)
          {
             obj->layer->evas->engine.func->ector_begin(output, context, ector, vd->backing_store, EINA_TRUE,
                                                        0, 0,
                                                        obj->cur->geometry.w, obj->cur->geometry.h,
                                                        do_async);
             _evas_vg_render(obj, vd, output, context, vd->backing_store, vd->root, NULL,do_async);
             obj->layer->evas->engine.func->image_dirty_region(obj->layer->evas->engine.data.output, vd->backing_store,
                                                               0, 0, 0, 0);
             obj->layer->evas->engine.func->ector_end(output, context, ector, vd->backing_store, do_async);
          }
        obj->layer->evas->engine.func->image_draw(output, context, surface,
                                                  vd->backing_store, 0, 0,
                                                  obj->cur->geometry.w, obj->cur->geometry.h, obj->cur->geometry.x + x,
                                                  obj->cur->geometry.y + y, obj->cur->geometry.w, obj->cur->geometry.h,
                                                  EINA_TRUE, do_async);
     }
   // reset the content change flag
   // don't move this to render_pre as there is no guarentee that
   // each render_pre() will be followed by a render() call.
   vd->content_changed = EINA_FALSE;
}

static void
evas_object_vg_render_pre(Evas_Object *eo_obj,
                          Evas_Object_Protected_Data *obj,
                          void *type_private_data)
{
   Evas_VG_Data *vd = type_private_data;
   Efl_VG_Base_Data *rnd;
   int is_v = 0, was_v = 0;
   Ector_Surface *s;

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

   // handle the vg root node tree.
   if (vd->root)
     {
        // FIXME: handle damage only on changed renderer.
        s = evas_ector_get(obj->layer->evas);
        if (s)
          _evas_vg_render_pre(vd->root, s, NULL);

        // FIXME: for now the walking Evas_VG_Node tree doesn't trigger any damage
        // So just forcing it here if necessary
        rnd = eo_data_scope_get(vd->root, EFL_VG_BASE_CLASS);

        //FIXME find the reason for NULL Base Class in some case?
        if (!rnd) return;

        if (rnd->changed)
          {
             vd->content_changed = EINA_TRUE;
             rnd->changed = EINA_FALSE;
             evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
             goto done;
          }
         else
           {
              // if size got changed , force a redraw.
              if ((obj->cur->geometry.w != obj->prev->geometry.w) ||
                  (obj->cur->geometry.h != obj->prev->geometry.h))
                vd->content_changed = EINA_TRUE;
           }
     }

   /* now figure what changed and add draw rects */
   /* if it just became visible or invisible */
   is_v = evas_object_is_visible(eo_obj, obj);
   was_v = evas_object_was_visible(eo_obj,obj);
   if (!(is_v | was_v)) goto done;

   if (is_v != was_v)
     {
        evas_object_render_pre_visible_change(&obj->layer->evas->clip_changes, eo_obj, is_v, was_v);
        goto done;
     }
   if (obj->changed_map || obj->changed_src_visible)
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
        goto done;
     }
   /* it's not visible - we accounted for it appearing or not so just abort */
   if (!is_v) goto done;
   /* clipper changed this is in addition to anything else for obj */
   evas_object_render_pre_clipper_change(&obj->layer->evas->clip_changes, eo_obj);
   /* if we restacked (layer or just within a layer) and don't clip anyone */
   if ((obj->restack) && (!obj->clip.clipees))
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
        goto done;
     }
   /* if it changed render op */
   if (obj->cur->render_op != obj->prev->render_op)
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
        goto done;
     }
   /* if it changed color */
   if ((obj->cur->color.r != obj->prev->color.r) ||
       (obj->cur->color.g != obj->prev->color.g) ||
       (obj->cur->color.b != obj->prev->color.b) ||
       (obj->cur->color.a != obj->prev->color.a))
     {
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
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
        evas_object_render_pre_prev_cur_add(&obj->layer->evas->clip_changes, eo_obj, obj);
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
   evas_object_render_pre_effect_updates(&obj->layer->evas->clip_changes, eo_obj, is_v, was_v);
}

static void
evas_object_vg_render_post(Evas_Object *eo_obj EINA_UNUSED,
                           Evas_Object_Protected_Data *obj ,
                           void *type_private_data EINA_UNUSED)
{
   /* this moves the current data to the previous state parts of the object */
   /* in whatever way is safest for the object. also if we don't need object */
   /* data anymore we can free it if the object deems this is a good idea */
   /* remove those pesky changes */
   evas_object_clip_changes_clean(obj);
   /* move cur to prev safely for object data */
   evas_object_cur_prev(obj);
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

void
_evas_vg_efl_gfx_view_size_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                               int *w, int *h)
{
   if (w) *w = pd->width;
   if (h) *h = pd->height;
}

void
_evas_vg_efl_gfx_view_size_set(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                               int w, int h)
{
   pd->width = w;
   pd->height = h;
}

void
_evas_vg_efl_gfx_fill_fill_set(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                               int x, int y, int w, int h)
{
   pd->fill.x = x;
   pd->fill.y = y;
   pd->fill.w = w;
   pd->fill.h = h;
}

void
_evas_vg_efl_gfx_fill_fill_get(Eo *obj EINA_UNUSED, Evas_VG_Data *pd,
                               int *x, int *y, int *w, int *h)
{
   if (x) *x = pd->fill.x;
   if (y) *y = pd->fill.y;
   if (w) *w = pd->fill.w;
   if (h) *h = pd->fill.h;
}

#include "evas_vg.eo.c"
