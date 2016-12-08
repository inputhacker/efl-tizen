#ifndef ECTOR_UTIL_H
# define ECTOR_UTIL_H

typedef unsigned char DATA8;
typedef unsigned short DATA16;

#ifndef WORDS_BIGENDIAN
/* x86 */
#define A_VAL(p) (((DATA8 *)(p))[3])
#define R_VAL(p) (((DATA8 *)(p))[2])
#define G_VAL(p) (((DATA8 *)(p))[1])
#define B_VAL(p) (((DATA8 *)(p))[0])
#define AR_VAL(p) ((DATA16 *)(p)[1])
#define GB_VAL(p) ((DATA16 *)(p)[0])
#else
/* ppc */
#define A_VAL(p) (((DATA8 *)(p))[0])
#define R_VAL(p) (((DATA8 *)(p))[1])
#define G_VAL(p) (((DATA8 *)(p))[2])
#define B_VAL(p) (((DATA8 *)(p))[3])
#define AR_VAL(p) ((DATA16 *)(p)[0])
#define GB_VAL(p) ((DATA16 *)(p)[1])
#endif

#define RGB_JOIN(r,g,b) \
  (((r) << 16) + ((g) << 8) + (b))

#define ARGB_JOIN(a,r,g,b) \
  (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))

#define ECTOR_ARGB_JOIN(a,r,g,b) \
        (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))

#define ECTOR_MUL4_SYM(x, y) \
 ( ((((((x) >> 16) & 0xff00) * (((y) >> 16) & 0xff00)) + 0xff0000) & 0xff000000) + \
   ((((((x) >> 8) & 0xff00) * (((y) >> 16) & 0xff)) + 0xff00) & 0xff0000) + \
   ((((((x) & 0xff00) * ((y) & 0xff00)) + 0xff0000) >> 16) & 0xff00) + \
   (((((x) & 0xff) * ((y) & 0xff)) + 0xff) >> 8) )


#ifndef MIN
#define MIN( a, b )  ( (a) < (b) ? (a) : (b) )
#endif

#ifndef MAX
#define MAX( a, b )  ( (a) > (b) ? (a) : (b) )
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#define BYTE_MUL(c, a) \
 ( (((((c) >> 8) & 0x00ff00ff) * (a)) & 0xff00ff00) + \
   (((((c) & 0x00ff00ff) * (a)) >> 8) & 0x00ff00ff) )

#define LOOP_ALIGNED_U1_A4(DEST, LENGTH, UOP, A4OP) \
   { \
      while((uintptr_t)DEST & 0xF && LENGTH) UOP \
   \
      while(LENGTH) { \
         switch(LENGTH) { \
            case 3: \
            case 2: \
            case 1: \
               UOP \
               break; \
            default: \
               A4OP \
               break; \
         } \
      } \
   }

static inline int
alpha_inverse(int color)
{
   color = ~color;
   return A_VAL(&color);
}

static inline void
_ector_memfill(uint *dest, int length, uint value)
{
   int n;
   if (!length)
     return;

   n = (length + 7) / 8;
   switch (length & 0x07)
     {
        case 0: do { *dest++ = value;
        case 7:      *dest++ = value;
        case 6:      *dest++ = value;
        case 5:      *dest++ = value;
        case 4:      *dest++ = value;
        case 3:      *dest++ = value;
        case 2:      *dest++ = value;
        case 1:      *dest++ = value;
        } while (--n > 0);
     }
}

static inline uint
INTERPOLATE_PIXEL_256(uint x, uint a, uint y, uint b)
{
   uint t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
   t >>= 8;
   t &= 0xff00ff;
   x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
   x &= 0xff00ff00;
   x |= t;
   return x;
}


static inline void
ector_color_argb_premul(int a, int *r, int *g, int *b)
{
   a++;
   if (r) { *r = (a * *r) >> 8; }
   if (g) { *g = (a * *g) >> 8; }
   if (b) { *b = (a * *b) >> 8; }
}

static inline void
ector_color_argb_unpremul(int a, int *r, int *g, int *b)
{
   if (!a) return;
   if (r) { *r = (255 * *r) / a; }
   if (g) { *g = (255 * *g) / a; }
   if (b) { *b = (255 * *b) / a; }
}


static inline unsigned int
ector_color_multiply(unsigned int c1, unsigned int c2)
{
   return ( ((((((c1) >> 16) & 0xff00) * (((c2) >> 16) & 0xff00)) + 0xff0000) & 0xff000000) +
            ((((((c1) >> 8) & 0xff00) * (((c2) >> 16) & 0xff)) + 0xff00) & 0xff0000) +
            ((((((c1) & 0xff00) * ((c2) & 0xff00)) + 0xff0000) >> 16) & 0xff00) +
            (((((c1) & 0xff) * ((c2) & 0xff)) + 0xff) >> 8) );
}

typedef double (*BLEND_FUNC)(double progress);

static double
_ease_linear(double t)
{
   return t;
}

static inline Eina_Bool
_generate_gradient_color_table(Efl_Gfx_Gradient_Stop *gradient_stops, int stop_count, uint *color_table, int size)
{
   int dist, idist, pos = 0, i;
   Eina_Bool alpha = EINA_FALSE;
   Efl_Gfx_Gradient_Stop *curr, *next;
   uint current_color, next_color;
   double delta, t, incr, fpos;

   curr = gradient_stops;
   if (curr->a != 255) alpha = EINA_TRUE;
   current_color = ECTOR_ARGB_JOIN(curr->a, curr->r, curr->g, curr->b);
   incr = 1.0 / (double)size;
   fpos = 1.5 * incr;

   color_table[pos++] = current_color;

   while (fpos <= curr->offset)
     {
        color_table[pos] = color_table[pos - 1];
        pos++;
        fpos += incr;
     }

   for (i = 0; i < stop_count - 1; ++i)
     {
        curr = (gradient_stops + i);
        next = (gradient_stops + i + 1);
        delta = 1/(next->offset - curr->offset);
        if (next->a != 255) alpha = EINA_TRUE;
        next_color = ECTOR_ARGB_JOIN(next->a, next->r, next->g, next->b);
        BLEND_FUNC func = &_ease_linear;
        while (fpos < next->offset && pos < size)
          {
             t = func((fpos - curr->offset) * delta);
             dist = (int)(256 * t);
             idist = 256 - dist;
             color_table[pos] = INTERPOLATE_PIXEL_256(current_color, idist, next_color, dist);
             ++pos;
             fpos += incr;
          }
        current_color = next_color;
     }

   for (;pos < size; ++pos)
     color_table[pos] = current_color;

   // Make sure the last color stop is represented at the end of the table
   color_table[size-1] = current_color;
   return alpha;
}

#endif
