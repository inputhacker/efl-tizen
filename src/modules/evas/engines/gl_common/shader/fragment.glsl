/* General-purpose fragment shader for all operations in Evas.
 * This file can either be used directly by evas at runtime to
 * generate its shaders with the appropriate #defines, or passed
 * through cpp first (in which case the precision must be manually added).
 */

FRAGMENT_SHADER

#ifndef SHD_NOMUL
varying vec4 col;
#endif

#ifdef SHD_EXTERNAL
uniform SAMPLER_EXTERNAL_OES tex;
varying vec2 tex_c;
#elif defined(SHD_TEX)
uniform sampler2D tex;
varying vec2 tex_c;
#endif

#if defined(SHD_NV12) || defined(SHD_YUY2)
uniform sampler2D texuv;
varying vec2 tex_c2;
#endif

#if defined(SHD_YUV)
uniform sampler2D texu;
uniform sampler2D texv;
varying vec2 tex_c2;
varying vec2 tex_c3;
#endif

#ifdef SHD_TEXA
uniform sampler2D texa;
varying vec2 tex_a;
#endif

#if defined(SHD_SAM12) || defined(SHD_SAM21) || defined(SHD_SAM22)
varying vec4 div_s;
# if defined(SHD_SAM12) || defined(SHD_SAM21)
varying vec2 tex_s[2];
# else
varying vec2 tex_s[4];
# endif
#endif

#ifdef SHD_MASK
uniform sampler2D texm;
varying vec2 tex_m;
# if defined(SHD_MASKSAM12) || defined(SHD_MASKSAM21)
varying float maskdiv_s;
varying vec2 masktex_s[2];
# elif defined(SHD_MASKSAM22)
varying float maskdiv_s;
varying vec2 masktex_s[4];
# endif
#endif

#ifdef SHD_PALETTE
uniform float xDerivative;
uniform float yDerivative;
uniform sampler2D paletteSampler;
vec4 texture2DPaletteResize(sampler2D sampler, highp vec2 texC)
{
   float a = texture2D(sampler, texC).a;
   float a1 = texture2D(sampler, texC + vec2(xDerivative, 0.)).a;
   float a2 = texture2D(sampler, texC + vec2(0., yDerivative)).a;
   float a3 = texture2D(sampler, texC + vec2(xDerivative, yDerivative)).a;
   vec4 color = texture2D(paletteSampler, vec2(a, .5));
   vec4 color1 = texture2D(paletteSampler, vec2(a1, .5));
   vec4 color2 = texture2D(paletteSampler, vec2(a2, .5));
   vec4 color3 = texture2D(paletteSampler, vec2(a3, .5));
   vec2 f = fract(texC.xy / vec2(xDerivative, yDerivative));
   return mix(mix(color, color1, f.x), mix(color2, color3, f.x), f.y);
}
vec4 texture2DPalette(sampler2D sampler, highp vec2 texC)
{
   float a = texture2D(sampler, texC).a;
   return texture2D(paletteSampler, vec2(a, .5));
}
#endif

#ifdef SHD_ALPHA
# define SWZ aaaa
#else
# ifndef SHD_BGRA
#  if defined(SHD_IMG) && defined(SHD_BIGENDIAN)
#   define SWZ gbar
#  else
#   define SWZ bgra
#endif
# else
#  if defined(SHD_IMG) && defined(SHD_BIGENDIAN)
#   define SWZ grab
#  else
#   define SWZ rgba
#  endif
# endif
#endif

void main()
{
   vec4 c;

#if defined(SHD_YUV) || defined(SHD_NV12) || defined(SHD_YUY2)
   float r, g, b, y, u, v, vmu;
# if defined(SHD_YUV)
   y = texture2D(tex, tex_c).r;
   u = texture2D(texu, tex_c2).r;
   v = texture2D(texv, tex_c3).r;
# elif defined(SHD_NV12) || defined(SHD_YUY2)
   y = texture2D(tex, tex_c).g;
   u = texture2D(texuv, tex_c2).g;
   v = texture2D(texuv, tex_c2).a;
# endif
// center u and v around 0 for uv and y (with 128/255 for u + v, 16/255 for y)
   u = u - 0.5;
   v = v - 0.5;

# if defined (SHD_YUV_709)
// 709 yuv colorspace for hd content
   y = (y - 0.062) * 1.164;
   vmu = (v * 0.534) + (u * 0.213);
   v = v * 1.793;
   u = u * 2.115;
# else
// 601 colorspace constants (older yuv content)
   y = (y - 0.062) * 1.164;
   vmu = (v * 0.813) + (u * 0.391);
   v = v * 1.596;
   u = u * 2.018;
# endif
// common yuv
   r = y + v;
   g = y - vmu;
   b = y + u;
   c = vec4(r, g, b, 1.0);

#elif defined(SHD_PALETTE)
#  if defined(SHD_PALETTE_RESIZE)
    c = texture2DPaletteResize(tex, tex_c).SWZ;
#  else
    c = texture2DPalette(tex, tex_c).SWZ;
#  endif
#elif defined(SHD_EXTERNAL)
   c = texture2D(tex, tex_c);

#elif defined(SHD_SAM12) || defined(SHD_SAM21)
   vec4 col00 = texture2D(tex, tex_c + tex_s[0]).SWZ;
   vec4 col01 = texture2D(tex, tex_c + tex_s[1]).SWZ;
   c = (col00 + col01) / div_s;

#elif defined(SHD_SAM22)
   vec4 col00 = texture2D(tex, tex_c + tex_s[0]).SWZ;
   vec4 col01 = texture2D(tex, tex_c + tex_s[1]).SWZ;
   vec4 col10 = texture2D(tex, tex_c + tex_s[2]).SWZ;
   vec4 col11 = texture2D(tex, tex_c + tex_s[3]).SWZ;
   c = (col00 + col01 + col10 + col11) / div_s;

#elif defined(SHD_TEX)
   c = texture2D(tex, tex_c).SWZ;

#else
   c = vec4(1, 1, 1, 1);
#endif

#ifdef SHD_MASK
   float ma;
# if defined(SHD_MASKSAM12) || defined(SHD_MASKSAM21)
   float ma00 = texture2D(texm, tex_m + masktex_s[0]).a;
   float ma01 = texture2D(texm, tex_m + masktex_s[1]).a;
   ma = (ma00 + ma01) / maskdiv_s;
# elif defined(SHD_MASKSAM22)
   float ma00 = texture2D(texm, tex_m + masktex_s[0]).a;
   float ma01 = texture2D(texm, tex_m + masktex_s[1]).a;
   float ma10 = texture2D(texm, tex_m + masktex_s[2]).a;
   float ma11 = texture2D(texm, tex_m + masktex_s[3]).a;
   ma = (ma00 + ma01 + ma10 + ma11) / maskdiv_s;
# else
   ma = texture2D(texm, tex_m).a;
# endif
#endif

#ifdef SHD_AFILL
   c.a = 1.0;
#endif

   gl_FragColor =
       c
#ifndef SHD_NOMUL
     * col
#endif
#ifdef SHD_MASK
     * ma
#endif
#ifdef SHD_TEXA
     * texture2D(texa, tex_a).r
#endif
   ;
}

