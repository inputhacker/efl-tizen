#ifndef ECTOR_DRAWHELPER_PRIVATE_H
#define ECTOR_DRAWHELPER_PRIVATE_H

#include "ector_private.h"

typedef void (*RGBA_Comp_Func)(uint *dest, const uint *src, int length, uint mul_col, uint const_alpha);
typedef void (*RGBA_Comp_Func_Solid)(uint *dest, int length, uint color, uint const_alpha);

extern RGBA_Comp_Func_Solid func_for_mode_solid[ECTOR_ROP_LAST];
extern RGBA_Comp_Func func_for_mode[ECTOR_ROP_LAST];

void init_drawhelper_gradient();
void init_draw_helper_sse2();
void init_draw_helper_neon();

void init_draw_helper();

RGBA_Comp_Func_Solid ector_comp_func_solid_span_get(Ector_Rop op, uint color);
RGBA_Comp_Func ector_comp_func_span_get(Ector_Rop op, uint color, Eina_Bool src_alpha);

#endif
