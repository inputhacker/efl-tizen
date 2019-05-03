#include "evas_common_private.h"
#include "evas_private.h"

#include "evas_vg_private.h"

#define MY_CLASS EFL_VG_CONTAINER_CLASS

//FIXME: This enum add temporarily to help understanding of additional code
//related to masking in prepare_mask.
//This needs to be formally declared through the eo class.
//This is a list of blending supported via efl_canvas_vg_node_mask_set().
typedef enum _EFL_CANVAS_VG_NODE_BLEND_TYPE
{
   EFL_CANVAS_VG_NODE_BLEND_TYPE_NONE = 0,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_ALPHA,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_ALPHA_INV,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_ADD,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_SUBSTRACT,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_INTERSECT,
   EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_DIFFERENCE
}EFL_CANVAS_VG_NODE_BLEND_TYPE;
//

static void
_draw_mask(Evas_Object_Protected_Data *obj, Efl_VG_Base *node,
           void *output, void *context, void *buffer)
{
   Eina_Bool visible = EINA_FALSE;
   eo_do(node, visible = efl_gfx_visible_get());

   if (!visible) return;

   if (eo_isa(node, EFL_VG_CONTAINER_CLASS))
     {
        Efl_VG_Container_Data *cd =
           eo_data_scope_get(node, EFL_VG_CONTAINER_CLASS);

        //Draw Mask Image.
        Efl_VG_Base *child;
        Eina_List *l;
        EINA_LIST_FOREACH(cd->children, l, child)
          _draw_mask(obj, child, output, context, buffer);
     }
   else
     {
        Efl_VG_Base_Data *nd = eo_data_scope_get(node, EFL_VG_BASE_CLASS);
        obj->layer->evas->engine.func->ector_renderer_draw(output, context, buffer, nd->renderer, NULL, EINA_FALSE);

     }
}

static Ector_Buffer *
_prepare_mask(Evas_Object_Protected_Data *obj,     //vector object
              Efl_VG_Base* mask_obj,
              void *output, void *context, void *buffer,
              Ector_Surface *surface,
              Eina_Matrix3 *ptransform,
              Eina_Matrix3 *ctransform,
              Ector_Buffer *mask,
              int mask_op)
{
   Efl_VG_Container_Data *pd = eo_data_scope_get(mask_obj, MY_CLASS);
   Efl_VG_Base_Data *nd =
         eo_data_scope_get(mask_obj, EFL_VG_BASE_CLASS);
   if (nd->flags == EFL_GFX_CHANGE_FLAG_NONE) return pd->mask.buffer;
   uint32_t init_buffer = 0x0;

   //1. Mask Size
   Eina_Rect mbound;
   mbound.x = 0;
   mbound.y = 0;
   mbound.w = obj->cur->geometry.w;
   mbound.h = obj->cur->geometry.h;

   //FIXME: If mask typs is SUBSTRACT or INTERSECT, buffer fills in white color(Full alpha color).
   if (pd->mask.option == EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_SUBSTRACT || pd->mask.option == EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_INTERSECT)
     init_buffer = 0xFFFFFFFF;

   //2. Reusable ector buffer?
   if (!pd->mask.buffer || (pd->mask.bound.w != mbound.w) ||
         (pd->mask.bound.h != mbound.h))
     {
        if (pd->mask.pixels) free(pd->mask.pixels);
        if (pd->mask.buffer) free(pd->mask.buffer);
        pd->mask.pixels = malloc(sizeof(uint32_t) * (mbound.w * mbound.h));
        memset(pd->mask.pixels, init_buffer, sizeof(uint32_t) * (mbound.w * mbound.h));
        pd->mask.buffer = (Ector_Buffer*) malloc(sizeof(Ector_Buffer));
        pd->mask.buffer->buffer = pd->mask.pixels;
        pd->mask.buffer->w = mbound.w;
        pd->mask.buffer->h = mbound.h;
        pd->mask.bound.w = mbound.w;
        pd->mask.bound.h = mbound.h;
        pd->mask.vg_pd = obj;
     }
   else
     {
        if (pd->mask.pixels)
          memset(pd->mask.pixels, init_buffer, sizeof(uint32_t) * mbound.w * mbound.h);
     }

   pd->mask.bound.x = mbound.x;
   pd->mask.bound.y = mbound.y;

   if (!pd->mask.buffer) ERR("Mask Buffer is invalid");

   //FIXME: This code means that there is another masking container.
   if (pd->mask.option >= EFL_CANVAS_VG_NODE_BLEND_TYPE_MASK_ADD)
     {
        Efl_VG_Container_Data *src_pd = pd;
        mask = pd->mask.buffer;
        for (Efl_VG *mask_src = pd->mask_src; mask_src; mask_src = src_pd->mask_src)
          {
             Efl_VG_Container_Data *target_pd = NULL;
             src_pd = eo_data_scope_get(mask_src, MY_CLASS);
             target_pd = eo_data_scope_get(eina_list_nth(src_pd->mask.target, 0), MY_CLASS);
             _evas_vg_render_pre(obj, mask_src,
                                 output, context, buffer, surface,
                                 ctransform, mask, target_pd->mask.option);
          }
     }

   //3. Prepare Drawing shapes...
   _evas_vg_render_pre(obj, mask_obj,
                       output, context, buffer,
                       surface,
                       ptransform, mask, mask_op);

   //4. Generating Mask Image.
    eo_do(surface,
       ector_software_surface_set(pd->mask.pixels, pd->mask.bound.w, pd->mask.bound.h, pd->mask.bound.w * 4),
       ector_surface_reference_point_set(-pd->mask.bound.x, -pd->mask.bound.y));
   _draw_mask(obj, mask_obj, output, context, buffer);

   return pd->mask.buffer;
}

static void
_efl_vg_container_render_pre(Evas_Object_Protected_Data *vg_pd,
                             Efl_VG *obj EINA_UNUSED,
                             Efl_VG_Base_Data *nd,
                             void *output,
                             void *context,
                             void *buffer,
                             Ector_Surface *surface,
                             Eina_Matrix3 *ptransform,
                             Ector_Buffer *mask,
                             int mask_op,
                             void *data)
{
   Efl_VG_Container_Data *pd = data;
   Eina_List *l;
   Efl_VG *child;
   Efl_Gfx_Change_Flag flag;

   if (nd->flags == EFL_GFX_CHANGE_FLAG_NONE) return;

   flag = nd->flags;
   nd->flags = EFL_GFX_CHANGE_FLAG_NONE;

   EFL_VG_COMPUTE_MATRIX(ctransform, ptransform, nd);

   //Container may have mask source.
   //FIXME : _prepare_mask() should only work in cases with matte or main mask.
   // This condition is valid because the main mask use same type as matte alpha.
   if (pd->mask_src &&
       (pd->mask.option == EFL_CANVAS_VG_NODE_BLEND_TYPE_ALPHA ||
        pd->mask.option == EFL_CANVAS_VG_NODE_BLEND_TYPE_ALPHA_INV))
     {
        mask_op = pd->mask.option;
        mask = _prepare_mask(vg_pd, pd->mask_src,
                             output, context, buffer, surface,
                             ptransform, ctransform, mask, mask_op);
     }

   EINA_LIST_FOREACH(pd->children, l, child)
     {
        //Don't need to update mask nodes.
        if (eo_isa(child, MY_CLASS))
          {
             Efl_VG_Container_Data *child_cd =
                eo_data_scope_get(child, MY_CLASS);
             if (child_cd->mask.target) continue;
          }

        //Skip Gradients. they will be updated by Shape.
        if (eo_isa(child, EFL_VG_GRADIENT_CLASS))
          continue;

        Efl_VG_Base_Data *child_nd =
           eo_data_scope_get(child, EFL_VG_BASE_CLASS);

        if (flag & EFL_GFX_CHANGE_FLAG_MATRIX)
          child_nd->flags |= EFL_GFX_CHANGE_FLAG_MATRIX;

        _evas_vg_render_pre(vg_pd, child, output, context, buffer, surface, ctransform, mask, mask_op);
     }
}

static Eo *
_efl_vg_container_eo_base_constructor(Eo *obj,
                                      Efl_VG_Container_Data *pd)
{
   Efl_VG_Base_Data *nd;

   pd->names = eina_hash_stringshared_new(NULL);

   obj = eo_do_super_ret(obj, MY_CLASS, obj, eo_constructor());

   nd = eo_data_scope_get(obj, EFL_VG_BASE_CLASS);
   nd->render_pre = _efl_vg_container_render_pre;
   nd->data = pd;
   nd->flags = EFL_GFX_CHANGE_FLAG_ALL;

   return obj;
}

static void
_efl_vg_container_eo_base_destructor(Eo *obj,
                                     Efl_VG_Container_Data *pd EINA_UNUSED)
{
   //Destroy mask surface
   if (pd->mask.buffer) free(pd->mask.buffer);
   if (pd->mask.pixels) free(pd->mask.pixels);

   eo_unref(pd->mask_src);
   eina_list_free(pd->mask.target);
   eina_hash_free(pd->names);
   pd->names = NULL;

   eo_do_super(obj, MY_CLASS, eo_destructor());

}

static void
_efl_vg_container_efl_vg_base_bounds_get(Eo *obj EINA_UNUSED,
                                        Efl_VG_Container_Data *pd,
                                        Eina_Rectangle *r)
{
   Eina_Rectangle s;
   Eina_Bool first = EINA_TRUE;
   Eina_List *l;
   Eo *child;

   EINA_RECTANGLE_SET(&s, -1, -1, 0, 0);

   EINA_LIST_FOREACH(pd->children, l, child)
     {
        if (first)
          {
             eo_do(child, efl_vg_bounds_get(r));
             first = EINA_FALSE;
          }
        else
          {
             eo_do(child, efl_vg_bounds_get(&s));
             eina_rectangle_union(r, &s);
          }
     }
}

static Efl_VG_Base *
_efl_vg_container_child_get(Eo *obj EINA_UNUSED, Efl_VG_Container_Data *pd, const char *name)
{
   const char *tmp = eina_stringshare_add(name);
   Efl_VG_Base *r;

   r = eina_hash_find(pd->names, tmp);
   eina_stringshare_del(tmp);

   return r;
}

static Eina_Iterator *
_efl_vg_container_children_get(Eo *obj EINA_UNUSED, Efl_VG_Container_Data *pd)
{
   return eina_list_iterator_new(pd->children);
}

static Eina_Bool
_efl_vg_container_efl_vg_base_interpolate(Eo *obj,
                                          Efl_VG_Container_Data *pd,
                                          const Efl_VG_Base *from, const Efl_VG_Base *to,
                                          double pos_map)
{
   Eina_Iterator *from_it, *to_it;
   Eina_List *l;
   Eina_Bool r, res = EINA_TRUE;
   Eo *from_child, *to_child, *child;

   //1. check if both the object are containers
   if (!(eo_isa(from, EFL_VG_CONTAINER_CLASS) &&
         eo_isa(to, EFL_VG_CONTAINER_CLASS)))
     return EINA_FALSE;

   eo_do_super(obj, EFL_VG_CONTAINER_CLASS, r = efl_vg_interpolate(from, to, pos_map));

   if (!r) return EINA_FALSE;

   eo_do(from, from_it = efl_vg_container_children_get());
   eo_do(to, to_it = efl_vg_container_children_get());
   EINA_LIST_FOREACH (pd->children, l, child)
     {
        res &= eina_iterator_next(from_it, (void **)&from_child);
        res &= eina_iterator_next(to_it, (void **)&to_child);
        if (!res && (eo_class_get(from_child) != eo_class_get(to_child) ||
            (eo_class_get(child) != eo_class_get(from_child))))
          {
             r = EINA_FALSE;
             break;
          }
        eo_do(child, r &= efl_vg_interpolate(from_child, to_child, pos_map));
        if (!r)
          break;
     }
   eina_iterator_free(from_it);
   eina_iterator_free(to_it);

   return r;
}

static void
_efl_vg_container_efl_vg_base_dup(Eo *obj,
                                  Efl_VG_Container_Data *pd,
                                  const Efl_VG_Base *from)
{
   Efl_VG_Container_Data *fromd;
   Eina_List *l;
   Eo *child;

   eo_do_super(obj, EFL_VG_CONTAINER_CLASS, efl_vg_dup(from));

   fromd = eo_data_scope_get(from, EFL_VG_CONTAINER_CLASS);

   //Copy Mask
   if (pd->mask_src)
     {
        Eo * mask_src = eo_add(eo_class_get(pd->mask_src), obj, efl_vg_dup(pd->mask_src));
        evas_vg_node_mask_set(obj, mask_src, pd->mask.option);
     }

   EINA_LIST_FREE(pd->children, child)
     eo_unref(child);

   EINA_LIST_FOREACH(fromd->children, l, child)
     {
        // By setting parent, we automatically reference
        // this new object as a child of obj. Magic at work !
        (void) eo_add(eo_class_get(child), obj, efl_vg_dup(child));
     }
}

static void
_efl_vg_container_efl_vg_base_mask_set(Eo *obj,
                                       Efl_VG_Container_Data *pd,
                                       Efl_VG_Base *mask,
                                       int op)
{
   if (pd->mask_src == mask) return;

   EINA_SAFETY_ON_FALSE_RETURN(eo_isa(mask, MY_CLASS));

   if (pd->mask_src)
     {
        Efl_VG_Container_Data *pd2 =
              eo_data_scope_get(pd->mask_src, MY_CLASS);
        pd2->mask.target = eina_list_remove(pd2->mask.target, obj);
     }

   if (mask)
     {
        Efl_VG_Container_Data *pd2 = eo_data_scope_get(mask, MY_CLASS);
        pd2->mask.target = eina_list_append(pd2->mask.target, obj);
     }

   pd->mask.option = op;
   if (pd->mask_src) eo_unref(pd->mask_src);
   if (mask) eo_ref(mask);
   pd->mask_src = mask;
   efl_canvas_vg_node_change(obj);
}

static void
_efl_vg_container_eo_base_parent_set(Eo *obj,
                                     Efl_VG_Container_Data *cd EINA_UNUSED,
                                     Eo *parent)
{
   eo_do_super(obj, MY_CLASS, eo_parent_set(parent));

   Efl_VG_Base_Data *nd = eo_data_scope_get(obj, EFL_VG_BASE_CLASS);
   efl_canvas_vg_container_vg_obj_update(obj, nd);
}

void
efl_canvas_vg_container_vg_obj_update(Efl_VG *obj, Efl_VG_Base_Data *nd)
{
   if (!obj) return;

   Efl_VG_Container_Data *cd = eo_data_scope_get(obj, MY_CLASS);
   if (!cd) return;

   Eina_List *l;
   Efl_VG* child;

   EINA_LIST_FOREACH(cd->children, l, child)
     {
        Efl_VG_Base_Data *child_nd =
           eo_data_scope_get(child, EFL_VG_BASE_CLASS);

        if (child_nd->vg_obj == nd->vg_obj) continue;

        child_nd->vg_obj = nd->vg_obj;
        child_nd->vd = nd->vd;

        if (eo_isa(child, MY_CLASS))
          efl_canvas_vg_container_vg_obj_update(child, child_nd);
     }
}

EAPI Efl_VG*
evas_vg_container_add(Efl_VG *parent)
{
   return eo_add(EFL_VG_CONTAINER_CLASS, parent);
}

#include "efl_vg_container.eo.c"
