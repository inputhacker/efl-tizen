#ifndef _EVAS_TEXT_UTILS_H
# define _EVAS_TEXT_UTILS_H

typedef struct _Evas_Text_Props      Evas_Text_Props;
// special case props
typedef struct _Evas_Text_Props_One  Evas_Text_Props_One;

typedef struct _Evas_Text_Props_Info Evas_Text_Props_Info;
typedef struct _Evas_Font_Array_Data Evas_Font_Array_Data;
typedef struct _Evas_Font_Array      Evas_Font_Array;
typedef struct _Evas_Font_Glyph_Info Evas_Font_Glyph_Info;

typedef enum
{
   EVAS_TEXT_PROPS_MODE_NONE = 0,
   EVAS_TEXT_PROPS_MODE_SHAPE
} Evas_Text_Props_Mode;

# include "evas_font_ot.h"
# include "language/evas_language_utils.h"

/* Used for showing "malformed" or missing chars */
/* TIZEN_ONLY(20161122): Replace the replacement character to 0x00A0 which is empty character.
#define REPLACEMENT_CHAR 0xFFFD
 */
#define REPLACEMENT_CHAR 0x00A0
/* END */

/*******************************************************************
 * TIZEN_ONLY(20150127): Add evas_textblock_cursor_cluster_* APIs. *
 *******************************************************************/
#define CHECK_LANGUAGE_CLUSTER_AVAILABLE(script) \
   (((script == EVAS_SCRIPT_THAI) || \
     (script == EVAS_SCRIPT_ARABIC) || \
     (script == EVAS_SCRIPT_DEVANAGARI) || \
     (script == EVAS_SCRIPT_BENGALI) || \
     (script == EVAS_SCRIPT_GUJARATI) || \
     (script == EVAS_SCRIPT_TELUGU) || \
     (script == EVAS_SCRIPT_KHMER) || \
     (script == EVAS_SCRIPT_SINHALA) || \
     (script == EVAS_SCRIPT_KANNADA) || \
     (script == EVAS_SCRIPT_MALAYALAM) || \
     (script == EVAS_SCRIPT_GURMUKHI) || \
     (script == EVAS_SCRIPT_ORIYA) || \
     (script == EVAS_SCRIPT_TAMIL)) ? \
    EINA_TRUE : EINA_FALSE)

#define CHECK_CLUSTER_EXCEPTION_CHAR(x) \
   (0x0E33 == (x))
/*******
 * END *
 *******/


typedef struct _Evas_Glyph Evas_Glyph;
typedef struct _Evas_Glyph_Array Evas_Glyph_Array;

struct _Evas_Glyph_Array
{
   Eina_Inarray *array;
   void *fi;
   unsigned int refcount;
};

struct _Evas_Text_Props
{
   Evas_Text_Props_Info *info; // 8/4
   Evas_Glyph_Array *glyphs; // 8/4
   void *font_instance; // 8/4
   /* Start and len represent the start offset and the length in the
    * glyphs_info and ot_data fields, they are both internal */
   // i only wonder if generation needs 32bits... :)
   int generation; // 4
//   Evas_BiDi_Props bidi; // 4 // bidi.dir == enum
//   Evas_Script_Type script; // 4 // enum
//   Eina_Bool changed : 1; // 1
//   Eina_Bool prepare : 1;
//   // +3 pad
// ** the below saves 8 bytes (72 -> 64 on 64bit)
   /* TIZEN_ONLY(20170216): add EVAS_BIDI_DIRECTION_ANY_RTL for evas_object_paragraph_direction_set API
   Evas_BiDi_Direction bidi_dir : 2; // 2 (enough for values)
    */
   Evas_BiDi_Direction bidi_dir : 3; // 3 (enough for values)
   /* END */
   Evas_Script_Type script : 7; // cont (enough for values)
   Eina_Bool changed : 1; // cont (bool)
   Eina_Bool prepare : 1; // cont (bool)
   // we have space here for at LEAST 5 bits (round up to 2 bytes) but due
   // to common padding we actually can add 5 + 16 (21) more bits for free
   Eina_Bool szlen_mode : 5; // use 5 of he 21 bits
// this can go here as the above is nicely aligned....
   // this is really big... 32 bytes. MOSt of the time the following...
   // start == text_offset == 0 AND len == text_len == smallish value (8 or
   // 16 bit is enough to store it most of the time).
   size_t start; // 8/4
   size_t len; // 8/4
   size_t text_offset; // 8/4 /* The text offset from the start of the info */
   size_t text_len; // 8/4 /* The length of the original text */
};

#define EVAS_TP_SZLEN_FULL 0
#define EVAS_TP_SZLEN_ONE 1

// special case textprop for ONE char! should use this for textgrid but
// we have to modify every bit of code that usea a textprop to go through a
// getter or setter etc. etc.
struct _Evas_Text_Props_One
{
   Evas_Text_Props_Info *info; // 8/4
   Evas_Glyph_Array *glyphs; // 8/4
   void *font_instance; // 8/4
   /* Start and len represent the start offset and the length in the
    * glyphs_info and ot_data fields, they are both internal */
   // i only wonder if generation needs 32bits... :)
   int generation; // 4
//   Evas_BiDi_Props bidi; // 4 // bidi.dir == enum
//   Evas_Script_Type script; // 4 // enum
//   Eina_Bool changed : 1; // 1
//   Eina_Bool prepare : 1;
//   // +3 pad
// ** the below saves 8 bytes (72 -> 64 on 64bit)
   /* TIZEN_ONLY(20170216): add EVAS_BIDI_DIRECTION_ANY_RTL for evas_object_paragraph_direction_set API
   Evas_BiDi_Direction bidi_dir : 2; // 2 (enough for values)
    */
   Evas_BiDi_Direction bidi_dir : 3; // 3 (enough for values)
   /* END */
   Evas_Script_Type script : 7; // cont (enough for values)
   Eina_Bool changed : 1; // cont (bool)
   Eina_Bool prepare : 1; // cont (bool)
   // we have space here for at LEAST 5 bits (round up to 2 bytes) but due
   // to common padding we actually can add 5 + 16 (21) more bits for free
   Eina_Bool szlen_mode : 5; // use 5 of he 21 bits
};

struct _Evas_Text_Props_Info
{
   Evas_Font_Glyph_Info *glyph; // 8/4
   Evas_Font_OT_Info *ot; // 8/4
   unsigned int refcount; // 4
};

struct _Evas_Font_Array_Data
{
   struct {
      unsigned char r, g, b, a;
   } color;
   int x;
   Evas_Glyph_Array *glyphs;
};

struct _Evas_Font_Array
{
   Eina_Inarray *array;
   unsigned int refcount;
};

/* Sorted in visual order when created */
struct _Evas_Font_Glyph_Info
{
   unsigned int index; // 4 /* Should conform to FT */
#if 1
   // done with shorts to save space... if we need 32k or bigger glyphs and
   // relative layout info... worry then.
   Evas_Coord pen_after; // 4
   short x_bear, y_bear, width; // 6
#else
   Evas_Coord x_bear; // 4
   /* This one is rarely used, only in draw, in which we already get the glyph
    * so it doesn't really save time. Leaving it here just so no one will
    * add it thinking it was accidentally skipped */
   Evas_Coord y_bear; // 4
   Evas_Coord width; // 4
   Evas_Coord pen_after; // 4
#endif
};

void
evas_common_font_glyphs_ref(Evas_Glyph_Array *array);
void
evas_common_font_glyphs_unref(Evas_Glyph_Array *array);

void
evas_common_font_fonts_ref(Evas_Font_Array *array);
void
evas_common_font_fonts_unref(Evas_Font_Array *array);

void
evas_common_text_props_bidi_set(Evas_Text_Props *props,
      Evas_BiDi_Paragraph_Props *bidi_par_props, size_t start);

void
evas_common_text_props_script_set(Evas_Text_Props *props, Evas_Script_Type scr);

EAPI Eina_Bool
evas_common_text_props_content_create(void *_fi, const Eina_Unicode *text,
      Evas_Text_Props *text_props, const Evas_BiDi_Paragraph_Props *par_props,
      size_t par_pos, int len, Evas_Text_Props_Mode mode, const char *lang);

void
evas_common_text_props_content_copy_and_ref(Evas_Text_Props *dst,
      const Evas_Text_Props *src);

void
evas_common_text_props_content_ref(Evas_Text_Props *props);

void
evas_common_text_props_content_nofree_unref(Evas_Text_Props *props);

void
evas_common_text_props_content_unref(Evas_Text_Props *props);

EAPI int
evas_common_text_props_cluster_next(const Evas_Text_Props *props, int pos);

EAPI int
evas_common_text_props_cluster_prev(const Evas_Text_Props *props, int pos);

EAPI int
evas_common_text_props_index_find(const Evas_Text_Props *props, int _cutoff);

EAPI Eina_Bool
evas_common_text_props_split(Evas_Text_Props *base, Evas_Text_Props *ext,
      int cutoff);
EAPI void
evas_common_text_props_merge(Evas_Text_Props *item1, const Evas_Text_Props *item2);

/* Common to Textblock and Filters */
Eina_Bool evas_common_format_color_parse(const char *str, int slen, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a);

#endif
