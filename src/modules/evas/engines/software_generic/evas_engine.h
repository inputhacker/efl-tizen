#ifndef _EVAS_ENGINE_H
# define _EVAS_ENGINE_H

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_EVAS_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif /* ! DLL_EXPORT */
# else
#  define EAPI __declspec(dllimport)
# endif /* ! EFL_EVAS_BUILD */
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

// TIZEN_ONLY(20171114) : image orient
EAPI void _evas_image_flip_horizontal(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_flip_vertical(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_rotate_180(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_rotate_90(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_rotate_270(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_flip_transpose(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);
EAPI void _evas_image_flip_transverse(DATA32 *pixels_out, const DATA32 *pixels_in, int iw, int ih);

#endif //_EVAS_ENGINE_H
