#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "vg_common.h"
#include <Evas.h>

#ifdef BUILD_VG_LOADER_JSON
static void
_construct_drawable_nodes(Efl_VG *root, const LOTLayerNode *layer, int depth)
{
   Efl_VG *parent = root;
   if (!parent) return;

   for (unsigned int i = 0; i < layer->mNodeList.size; i++)
     {
        const LOTNode *node = layer->mNodeList.ptr[i];
        if (!node) continue;

        const float *data = node->mPath.ptPtr;
        if (!data) continue;

        Efl_VG *shape = eo_add(EFL_VG_SHAPE_CLASS, parent);
        if (!shape) continue;
#if 0
        for (int i = 0; i < depth; i++) printf("    ");
        printf("%s (%p)\n", efl_class_name_get(efl_class_get(shape)), shape);
#endif
        //0: Path
        eo_do(shape, efl_gfx_shape_reserve(node->mPath.elmCount, node->mPath.ptCount));

        for (int i = 0; i < node->mPath.elmCount; i++)
          {
             switch (node->mPath.elmPtr[i])
               {
                case 0:
                   eo_do(shape, efl_gfx_shape_append_move_to(data[0], data[1]));
                   data += 2;
                   break;
                case 1:
                   eo_do(shape, efl_gfx_shape_append_line_to(data[0], data[1]));
                   data += 2;
                   break;
                case 2:
                   eo_do(shape, efl_gfx_shape_append_cubic_to(data[0], data[1], data[2], data[3], data[4], data[5]));
                   data += 6;
                   break;
                case 3:
                   eo_do(shape, efl_gfx_shape_append_close());
                   break;
                default:
                   ERR("No reserved path type = %d", node->mPath.elmPtr[i]);
               }
          }

        //1: Stroke
        if (node->mStroke.enable)
          {
             //Stroke Width
             eo_do(shape, efl_gfx_shape_stroke_width_set(node->mStroke.width));

             //Stroke Cap
             Efl_Gfx_Cap cap;
             switch (node->mStroke.cap)
               {
                case CapFlat: cap = EFL_GFX_CAP_BUTT; break;
                case CapSquare: cap = EFL_GFX_CAP_SQUARE; break;
                case CapRound: cap = EFL_GFX_CAP_ROUND; break;
                default: cap = EFL_GFX_CAP_BUTT; break;
               }
             eo_do(shape, efl_gfx_shape_stroke_cap_set(cap));

             //Stroke Join
             Efl_Gfx_Join join;
             switch (node->mStroke.join)
               {
                case JoinMiter: join = EFL_GFX_JOIN_MITER; break;
                case JoinBevel: join = EFL_GFX_JOIN_BEVEL; break;
                case JoinRound: join = EFL_GFX_JOIN_ROUND; break;
                default: join = EFL_GFX_JOIN_MITER; break;
               }
             eo_do(shape, efl_gfx_shape_stroke_join_set(join));

             //Stroke Dash
             if (node->mStroke.dashArraySize > 0)
               {
                  int size = (node->mStroke.dashArraySize / 2);
                  Efl_Gfx_Dash *dash = malloc(sizeof(Efl_Gfx_Dash) * size);
                  if (dash)
                    {
                       for (int i = 0; i <= size; i+=2)
                         {
                            dash[i].length = node->mStroke.dashArray[i];
                            dash[i].gap = node->mStroke.dashArray[i + 1];
                         }
                       eo_do(shape, efl_gfx_shape_stroke_dash_set(dash, size));
                       free(dash);
                    }
               }
          }

        //2: Fill Method
        switch (node->mBrushType)
          {
           case BrushSolid:
             {
                float pa = ((float)node->mColor.a) / 255;
                int r = (int)(((float) node->mColor.r) * pa);
                int g = (int)(((float) node->mColor.g) * pa);
                int b = (int)(((float) node->mColor.b) * pa);
                int a = node->mColor.a;

                if (node->mStroke.enable)
                  eo_do(shape, efl_gfx_shape_stroke_color_set(r, g, b, a));
                else
                  eo_do(shape, efl_gfx_color_set(r, g, b, a));
             }
             break;
           case BrushGradient:
             {
                Efl_VG* grad = NULL;

                if (node->mGradient.type == GradientLinear)
                  {
                     grad = eo_add(EFL_VG_GRADIENT_LINEAR_CLASS, parent);
                     eo_do(grad, efl_gfx_gradient_linear_start_set(node->mGradient.start.x, node->mGradient.start.y));
                     eo_do(grad, efl_gfx_gradient_linear_end_set(node->mGradient.end.x, node->mGradient.end.y));
                  }
                else if (node->mGradient.type == GradientRadial)
                  {
                     grad = eo_add(EFL_VG_GRADIENT_RADIAL_CLASS, parent);
                     eo_do(grad, efl_gfx_gradient_radial_center_set(node->mGradient.center.x, node->mGradient.center.y));
                     eo_do(grad, efl_gfx_gradient_radial_focal_set(node->mGradient.focal.x, node->mGradient.focal.y));
                     eo_do(grad, efl_gfx_gradient_radial_radius_set(node->mGradient.cradius));
                  }
                else
                  ERR("No reserved gradient type = %d", node->mGradient.type);

                if (grad)
                  {
                     //Gradient Stop
                     Efl_Gfx_Gradient_Stop* stops = malloc(sizeof(Efl_Gfx_Gradient_Stop) * node->mGradient.stopCount);
                     if (stops)
                       {
                          for (unsigned int i = 0; i < node->mGradient.stopCount; i++)
                            {
                               stops[i].offset = node->mGradient.stopPtr[i].pos;
                               float pa = ((float)node->mGradient.stopPtr[i].a) / 255;
                               stops[i].r = (int)(((float)node->mGradient.stopPtr[i].r) * pa);
                               stops[i].g = (int)(((float)node->mGradient.stopPtr[i].g) * pa);
                               stops[i].b = (int)(((float)node->mGradient.stopPtr[i].b) * pa);
                               stops[i].a = node->mGradient.stopPtr[i].a;
                            }
                          eo_do(grad, efl_gfx_gradient_stop_set(stops, node->mGradient.stopCount));
                          free(stops);
                       }
                     if (node->mStroke.enable)
                       evas_vg_shape_stroke_fill_set(shape, grad);
                     else
                       evas_vg_shape_fill_set(shape, grad);
                  }
             }
             break;
           default:
              ERR("No reserved brush type = %d", node->mBrushType);
          }

        //3: Fill Rule
        if (node->mFillRule == FillEvenOdd)
          eo_do(shape, efl_gfx_shape_fill_rule_set(EFL_GFX_FILL_RULE_ODD_EVEN));
        else if (node->mFillRule == FillWinding)
          eo_do(shape, efl_gfx_shape_fill_rule_set(EFL_GFX_FILL_RULE_WINDING));
     }
}

static void
_update_vg_tree(Efl_VG *root, const LOTLayerNode *layer, int depth)
{
   Efl_VG *ptree = NULL;
   Efl_VG *ctree = NULL;

   //Note: We assume that if matte is valid, next layer must be a matte source.
   LOTMatteType matte = MatteNone;
   Eina_Bool skip = EINA_FALSE;

   //Is this layer a container layer?
   for (unsigned int i = 0; i < layer->mLayerList.size; i++)
     {
        if (!layer->mLayerList.ptr[i]->mVisible || skip)
          {
             //Next layer must be a dummy. so skip it.
             if (layer->mLayerList.ptr[i]->mMatte != MatteNone)
               skip = EINA_TRUE;
             else
               skip = EINA_FALSE;
             continue;
          }

        ctree = eo_add(EFL_VG_CONTAINER_CLASS, root);
#if 0
        for (int i = 0; i < depth; i++) printf("    ");
        printf("%s (%p) matte:%d => %p\n", efl_class_name_get(efl_class_get(ctree)), ctree, matte, ptree);
#endif
        _update_vg_tree(ctree, layer->mLayerList.ptr[i], depth+1);

        //TODO: Only valid for MatteAlphaInverse?
        //TODO: Set this blending option to efl_canvas_vg_node...
        if (matte != MatteNone)
           evas_vg_node_mask_set(ptree, ctree, matte);

        matte = layer->mLayerList.ptr[i]->mMatte;
        ptree = ctree;

        //Debug Matte Info
        switch (matte)
          {
           case MatteNone:
           case MatteAlphaInv:
           case MatteAlpha:
              break;
           case MatteLuma:
              ERR("TODO: MatteLuma");
              break;
           case MatteLumaInv:
              ERR("TODO: MatteLumaInv");
              break;
           default:
              ERR("No reserved Matte type = %d", matte);
          }
     }

   //Construct drawable nodes.
   if (layer->mNodeList.size > 0)
     _construct_drawable_nodes(root, layer, depth);
}
#endif

Eina_Bool
vg_common_json_create_vg_node(Vg_File_Data *vfd)
{
#ifdef BUILD_VG_LOADER_JSON
   Lottie_Animation *lot_anim = (Lottie_Animation *) vfd->loader_data;
   if (!lot_anim) return EINA_FALSE;

   //Root node
   if (vfd->root) eo_unref(vfd->root);
   vfd->root = eo_add(EFL_VG_CONTAINER_CLASS, NULL);
   Efl_VG *root = vfd->root;
   if (!root) return EINA_FALSE;
   unsigned int frame_num = (vfd->anim_data) ? vfd->anim_data->frame_num : 0;
   const LOTLayerNode *tree =
      lottie_animation_render_tree(lot_anim, frame_num,
                                   vfd->view_box.w, vfd->view_box.h);
#if 0
   printf("%s (%p)\n", efl_class_name_get(efl_class_get(vfd->root)), vfd->root);
#endif
   _update_vg_tree(root, tree, 1);

#else
   return EINA_FALSE;
#endif
   return EINA_TRUE;
}
