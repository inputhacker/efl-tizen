#ifndef _EFL_H
#define _EFL_H

#if defined ( __cplusplus )
extern "C"
{
#endif

#include <Eo.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EFL_BUILD */
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif /* ! _WIN32 */

#ifdef EFL_BETA_API_SUPPORT

/* Interfaces */
#include "interfaces/efl_control.eo.h"
#include "interfaces/efl_file.eo.h"
#include "interfaces/efl_image.eo.h"
#include "interfaces/efl_player.eo.h"
#include "interfaces/efl_text.eo.h"
#include "interfaces/efl_text_properties.eo.h"

#endif
/**
 * Path command enum.
 *
 * @since 1.13
 * @ingroup Efl_Graphics_Shape
 */
typedef enum _Efl_Graphics_Path_Command
{
  EFL_GRAPHICS_PATH_COMMAND_TYPE_END = 0, /**< End of the stream of command */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_MOVE_TO, /**< A move command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_LINE_TO, /**< A line command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_QUADRATIC_TO, /**< A quadratic command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_SQUADRATIC_TO, /**< A smooth quadratic command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_CUBIC_TO, /**< A cubic command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_SCUBIC_TO, /**< A smooth cubic command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_ARC_TO, /**< An arc command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_CLOSE, /**< A close command type */
  EFL_GRAPHICS_PATH_COMMAND_TYPE_LAST, /**< Not a valid command, but last one according to this version header */
} Efl_Graphics_Path_Command;

/**
 * Type describing dash
 * @since 1.13
 */
typedef struct _Efl_Graphics_Dash Efl_Graphics_Dash;
struct _Efl_Graphics_Dash
{
   double length;
   double gap;
};

/**
 * Type defining how a line end.
 * @since 1.13
 */
typedef enum _Efl_Graphics_Cap
{
  EFL_GRAPHICS_CAP_BUTT,
  EFL_GRAPHICS_CAP_ROUND,
  EFL_GRAPHICS_CAP_SQUARE,
  EFL_GRAPHICS_CAP_LAST
} Efl_Graphics_Cap;

/**
 * Type defining how join between path are drawn.
 * @since 1.13
 */
typedef enum _Efl_Graphics_Join
{
  EFL_GRAPHICS_JOIN_MITER,
  EFL_GRAPHICS_JOIN_ROUND,
  EFL_GRAPHICS_JOIN_BEVEL,
  EFL_GRAPHICS_JOIN_LAST
} Efl_Graphics_Join;

/**
 * Type defining gradient stop.
 * @since 1.13
 */
typedef struct _Efl_Graphics_Gradient_Stop Efl_Graphics_Gradient_Stop;
struct _Efl_Graphics_Gradient_Stop
{
   double offset;
   int r;
   int g;
   int b;
   int a;
};

/**
 * Type defining how the gradient spread after its limit.
 * @since 1.13
 */
typedef enum _Efl_Graphics_Gradient_Spread
{
  EFL_GRAPHICS_GRADIENT_SPREAD_PAD,
  EFL_GRAPHICS_GRADIENT_SPREAD_REFLECT,
  EFL_GRAPHICS_GRADIENT_SPREAD_REPEAT,
  EFL_GRAPHICS_GRADIENT_SPREAD_LAST
} Efl_Graphics_Gradient_Spread;

#include "interfaces/efl_graphics_shape.eo.h"
#include "interfaces/efl_graphics_gradient.eo.h"
#include "interfaces/efl_graphics_gradient_linear.eo.h"

#if defined ( __cplusplus )
}
#endif

#endif
