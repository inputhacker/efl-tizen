/* DO NOT MODIFY THIS FILE AS IT IS AUTO-GENERATED */
/* IF IT IS CHANGED PLEASE COMMIT THE CHANGES */

static const char fragment_glsl[] =
   "/* General-purpose fragment shader for all operations in Evas.\n"
   " * This file can either be used directly by evas at runtime to\n"
   " * generate its shaders with the appropriate #defines, or passed\n"
   " * through cpp first (in which case the precision must be manually added).\n"
   " */\n"
   "#ifdef GL_ES\n"
   "# ifdef GL_FRAGMENT_PRECISION_HIGH\n"
   "precision highp float;\n"
   "# else\n"
   "precision mediump float;\n"
   "# endif\n"
   "# ifdef SHD_EXTERNAL\n"
   "extension GL_OES_EGL_image_external : require\n"
   "#  define SAMPLER_EXTERNAL_OES samplerExternalOES\n"
   "# endif\n"
   "#else\n"
   "# define SAMPLER_EXTERNAL_OES sampler2D\n"
   "#endif\n"
   "#ifndef SHD_NOMUL\n"
   "varying vec4 col;\n"
   "#endif\n"
   "#ifdef SHD_EXTERNAL\n"
   "uniform SAMPLER_EXTERNAL_OES tex;\n"
   "varying vec2 tex_c;\n"
   "#elif defined(SHD_TEX)\n"
   "uniform sampler2D tex;\n"
   "varying vec2 tex_c;\n"
   "#endif\n"
   "#if defined(SHD_NV12) || defined(SHD_YUY2)\n"
   "uniform sampler2D texuv;\n"
   "varying vec2 tex_c2;\n"
   "#endif\n"
   "#if defined(SHD_YUV)\n"
   "uniform sampler2D texu;\n"
   "uniform sampler2D texv;\n"
   "varying vec2 tex_c2;\n"
   "varying vec2 tex_c3;\n"
   "#endif\n"
   "#ifdef SHD_TEXA\n"
   "uniform sampler2D texa;\n"
   "varying vec2 tex_a;\n"
   "#endif\n"
   "#if defined(SHD_SAM12) || defined(SHD_SAM21) || defined(SHD_SAM22)\n"
   "varying vec4 div_s;\n"
   "# if defined(SHD_SAM12) || defined(SHD_SAM21)\n"
   "varying vec2 tex_s[2];\n"
   "# else\n"
   "varying vec2 tex_s[4];\n"
   "# endif\n"
   "#endif\n"
   "#ifdef SHD_MASK\n"
   "uniform sampler2D texm;\n"
   "varying vec2 tex_m;\n"
   "# if defined(SHD_MASKSAM12) || defined(SHD_MASKSAM21)\n"
   "varying float maskdiv_s;\n"
   "varying vec2 masktex_s[2];\n"
   "# elif defined(SHD_MASKSAM22)\n"
   "varying float maskdiv_s;\n"
   "varying vec2 masktex_s[4];\n"
   "# endif\n"
   "#endif\n"
   "#ifdef SHD_ALPHA\n"
   "# define SWZ aaaa\n"
   "#else\n"
   "# ifndef SHD_BGRA\n"
   "#  if defined(SHD_IMG) && defined(SHD_BIGENDIAN)\n"
   "#   define SWZ gbar\n"
   "#  else\n"
   "#   define SWZ bgra\n"
   "#endif\n"
   "# else\n"
   "#  if defined(SHD_IMG) && defined(SHD_BIGENDIAN)\n"
   "#   define SWZ grab\n"
   "#  else\n"
   "#   define SWZ rgba\n"
   "#  endif\n"
   "# endif\n"
   "#endif\n"
   "void main()\n"
   "{\n"
   "   vec4 c;\n"
   "#if defined(SHD_YUV) || defined(SHD_NV12) || defined(SHD_YUY2)\n"
   "   float r, g, b, y, u, v, vmu;\n"
   "# if defined(SHD_YUV)\n"
   "   y = texture2D(tex, tex_c).r;\n"
   "   u = texture2D(texu, tex_c2).r;\n"
   "   v = texture2D(texv, tex_c3).r;\n"
   "# elif defined(SHD_NV12) || defined(SHD_YUY2)\n"
   "   y = texture2D(tex, tex_c).g;\n"
   "   u = texture2D(texuv, tex_c2).g;\n"
   "   v = texture2D(texuv, tex_c2).a;\n"
   "# endif\n"
   "// center u and v around 0 for uv and y (with 128/255 for u + v, 16/255 for y)\n"
   "   u = u - 0.5;\n"
   "   v = v - 0.5;\n"
   "# if defined (SHD_YUV_709)\n"
   "// 709 yuv colorspace for hd content\n"
   "   y = (y - 0.062) * 1.164;\n"
   "   vmu = (v * 0.534) + (u * 0.213);\n"
   "   v = v * 1.793;\n"
   "   u = u * 2.115;\n"
   "# else\n"
   "// 601 colorspace constants (older yuv content)\n"
   "   y = (y - 0.062) * 1.164;\n"
   "   vmu = (v * 0.813) + (u * 0.391);\n"
   "   v = v * 1.596;\n"
   "   u = u * 2.018;\n"
   "# endif\n"
   "// common yuv\n"
   "   r = y + v;\n"
   "   g = y - vmu;\n"
   "   b = y + u;\n"
   "   c = vec4(r, g, b, 1.0);\n"
   "#elif defined(SHD_SAM12) || defined(SHD_SAM21)\n"
   "   vec4 col00 = texture2D(tex, tex_c + tex_s[0]).SWZ;\n"
   "   vec4 col01 = texture2D(tex, tex_c + tex_s[1]).SWZ;\n"
   "   c = (col00 + col01) / div_s;\n"
   "#elif defined(SHD_SAM22)\n"
   "   vec4 col00 = texture2D(tex, tex_c + tex_s[0]).SWZ;\n"
   "   vec4 col01 = texture2D(tex, tex_c + tex_s[1]).SWZ;\n"
   "   vec4 col10 = texture2D(tex, tex_c + tex_s[2]).SWZ;\n"
   "   vec4 col11 = texture2D(tex, tex_c + tex_s[3]).SWZ;\n"
   "   c = (col00 + col01 + col10 + col11) / div_s;\n"
   "#elif defined(SHD_TEX) || defined(SHD_EXTERNAL)\n"
   "   c = texture2D(tex, tex_c).SWZ;\n"
   "#else\n"
   "   c = vec4(1, 1, 1, 1);\n"
   "#endif\n"
   "#ifdef SHD_MASK\n"
   "   float ma;\n"
   "# if defined(SHD_MASKSAM12) || defined(SHD_MASKSAM21)\n"
   "   float ma00 = texture2D(texm, tex_m + masktex_s[0]).a;\n"
   "   float ma01 = texture2D(texm, tex_m + masktex_s[1]).a;\n"
   "   ma = (ma00 + ma01) / maskdiv_s;\n"
   "# elif defined(SHD_MASKSAM22)\n"
   "   float ma00 = texture2D(texm, tex_m + masktex_s[0]).a;\n"
   "   float ma01 = texture2D(texm, tex_m + masktex_s[1]).a;\n"
   "   float ma10 = texture2D(texm, tex_m + masktex_s[2]).a;\n"
   "   float ma11 = texture2D(texm, tex_m + masktex_s[3]).a;\n"
   "   ma = (ma00 + ma01 + ma10 + ma11) / maskdiv_s;\n"
   "# else\n"
   "   ma = texture2D(texm, tex_m).a;\n"
   "# endif\n"
   "#endif\n"
   "   gl_FragColor =\n"
   "       c\n"
   "#ifndef SHD_NOMUL\n"
   "     * col\n"
   "#endif\n"
   "#ifdef SHD_MASK\n"
   "	 * ma\n"
   "#endif\n"
   "#ifdef SHD_TEXA\n"
   "     * texture2D(texa, tex_a).r\n"
   "#endif\n"
   "   ;\n"
   "#ifdef SHD_AFILL\n"
   "   gl_FragColor.a = 1.0;\n"
   "#endif\n"
   "}\n";

static const char vertex_glsl[] =
   "/* General-purpose vertex shader for all operations in Evas.\n"
   " * This file can either be used directly by evas at runtime to\n"
   " * generate its shaders with the appropriate #defines, or passed\n"
   " * through cpp first (in which case the precision must be manually added).\n"
   " */\n"
   "#ifdef GL_ES\n"
   "precision highp float;\n"
   "#endif\n"
   "attribute vec4 vertex;\n"
   "uniform mat4 mvp;\n"
   "/* All except nomul */\n"
   "#ifndef SHD_NOMUL\n"
   "attribute vec4 color;\n"
   "varying vec4 col;\n"
   "#endif\n"
   "/* All images & fonts */\n"
   "#if defined(SHD_TEX) || defined(SHD_EXTERNAL)\n"
   "attribute vec2 tex_coord;\n"
   "varying vec2 tex_c;\n"
   "#endif\n"
   "/* NV12, YUY2 */\n"
   "#if defined(SHD_NV12) || defined(SHD_YUY2) || defined(SHD_YUV)\n"
   "attribute vec2 tex_coord2;\n"
   "varying vec2 tex_c2;\n"
   "#endif\n"
   "/* YUV */\n"
   "#ifdef SHD_YUV\n"
   "attribute vec2 tex_coord3;\n"
   "varying vec2 tex_c3;\n"
   "#endif\n"
   "/* RGB+A */\n"
   "#ifdef SHD_TEXA\n"
   "attribute vec2 tex_coorda;\n"
   "varying vec2 tex_a;\n"
   "#endif\n"
   "/* Sampling */\n"
   "#if defined(SHD_SAM12) || defined(SHD_SAM21) || defined(SHD_SAM22)\n"
   "attribute vec2 tex_sample;\n"
   "varying vec4 div_s;\n"
   "# if defined(SHD_SAM12) || defined(SHD_SAM21)\n"
   "varying vec2 tex_s[2];\n"
   "# else\n"
   "varying vec2 tex_s[4];\n"
   "# endif\n"
   "#endif\n"
   "/* Masking */\n"
   "#ifdef SHD_MASK\n"
   "attribute vec4 mask_coord;\n"
   "varying vec2 tex_m;\n"
   "# if defined(SHD_MASKSAM12) || defined(SHD_MASKSAM21)\n"
   "attribute vec2 tex_masksample;\n"
   "varying float maskdiv_s;\n"
   "varying vec2 masktex_s[2];\n"
   "# elif defined(SHD_MASKSAM22)\n"
   "attribute vec2 tex_masksample;\n"
   "varying float maskdiv_s;\n"
   "varying vec2 masktex_s[4];\n"
   "# endif\n"
   "#endif\n"
   "void main()\n"
   "{\n"
   "   gl_Position = mvp * vertex;\n"
   "#ifndef SHD_NOMUL\n"
   "   col = color;\n"
   "#endif\n"
   "#if defined(SHD_TEX) || defined(SHD_EXTERNAL)\n"
   "   tex_c = tex_coord;\n"
   "#endif\n"
   "#ifdef SHD_NV12\n"
   "   tex_c2 = tex_coord2 * 0.5;\n"
   "#endif\n"
   "#ifdef SHD_YUY2\n"
   "   tex_c2 = vec2(tex_coord2.x * 0.5, tex_coord2.y);\n"
   "#endif\n"
   "#ifdef SHD_YUV\n"
   "   tex_c2 = tex_coord2;\n"
   "   tex_c3 = tex_coord3;\n"
   "#endif\n"
   "#ifdef SHD_TEXA\n"
   "   tex_a = tex_coorda;\n"
   "#endif\n"
   "#if defined(SHD_SAM12)\n"
   "   tex_s[0] = vec2(0, -tex_sample.y);\n"
   "   tex_s[1] = vec2(0,  tex_sample.y);\n"
   "   div_s = vec4(2, 2, 2, 2);\n"
   "#elif defined(SHD_SAM21)\n"
   "   tex_s[0] = vec2(-tex_sample.x, 0);\n"
   "   tex_s[1] = vec2( tex_sample.x, 0);\n"
   "   div_s = vec4(2, 2, 2, 2);\n"
   "#elif defined(SHD_SAM22)\n"
   "   tex_s[0] = vec2(-tex_sample.x, -tex_sample.y);\n"
   "   tex_s[1] = vec2( tex_sample.x, -tex_sample.y);\n"
   "   tex_s[2] = vec2( tex_sample.x,  tex_sample.y);\n"
   "   tex_s[3] = vec2(-tex_sample.x,  tex_sample.y);\n"
   "   div_s = vec4(4, 4, 4, 4);\n"
   "#endif\n"
   "#if defined(SHD_MASKSAM12)\n"
   "   masktex_s[0] = vec2(0, -tex_masksample.y);\n"
   "   masktex_s[1] = vec2(0,  tex_masksample.y);\n"
   "   maskdiv_s = 2.0;\n"
   "#elif defined(SHD_MASKSAM21)\n"
   "   masktex_s[0] = vec2(-tex_masksample.x, 0);\n"
   "   masktex_s[1] = vec2( tex_masksample.x, 0);\n"
   "   maskdiv_s = 2.0;\n"
   "#elif defined(SHD_MASKSAM22)\n"
   "   masktex_s[0] = vec2(-tex_masksample.x, -tex_masksample.y);\n"
   "   masktex_s[1] = vec2( tex_masksample.x, -tex_masksample.y);\n"
   "   masktex_s[2] = vec2( tex_masksample.x,  tex_masksample.y);\n"
   "   masktex_s[3] = vec2(-tex_masksample.x,  tex_masksample.y);\n"
   "   maskdiv_s = 4.0;\n"
   "#endif\n"
   "#ifdef SHD_MASK\n"
   "   // mask_coord.w contains the Y-invert flag\n"
   "   // position on screen in [0..1] range of current pixel\n"
   "   vec4 mask_Position = mvp * vertex * vec4(0.5, sign(mask_coord.w) * 0.5, 0.5, 0.5) + vec4(0.5, 0.5, 0, 0);\n"
   "   tex_m = mask_Position.xy * abs(mask_coord.zw) + mask_coord.xy;\n"
   "#endif\n"
   "}\n";

