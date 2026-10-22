/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ضمن "hb.hh"

#ضمن "hb-font.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"
#ضمن "hb-machinery.hh"

#ضمن "hb-ot.h"

#ضمن "hb-ot-var-avar-table.hh"
#ضمن "hb-ot-var-fvar-table.hh"

#ifndef HB_NO_OT_FONT
#ضمن "hb-ot.h"
#endif
#ifdef HAVE_FREETYPE
#ضمن "hb-ft.h"
#endif
#ifdef HAVE_FONTATIONS
#ضمن "hb-fontations.h"
#endif
#ifdef HAVE_CORETEXT
#ضمن "hb-coretext.h"
#endif
#ifdef HAVE_DIRECTWRITE
#ضمن "hb-directwrite.h"
#endif


/**
 * SECTION:hb-font
 * @title: hb-font
 * @short_description: Font objects
 * @include: hb.h
 *
 * Functions for working with font objects.
 *
 * A font object represents a font face at a specific size and with
 * certain other parameters (pixels-per-em, points-per-em, variation
 * settings) specified. Font objects are created from font face
 * objects, and are used as input to hb_shape(), among other things.
 *
 * Client programs can optionally pass in their own functions that
 * implement the basic, lower-level queries of font objects. This set
 * of font functions is defined by the virtual methods in
 * #hb_font_funcs_t.
 *
 * HarfBuzz provides a built-in set of lightweight default
 * functions for each method in #hb_font_funcs_t.
 *
 * The default font functions are implemented in terms of the
 * #hb_font_funcs_t methods of the parent font object.  This allows
 * client programs to override only the methods they need to, and
 * otherwise inherit the parent font's implementation, if any.
 **/


/*
 * hb_font_funcs_t
 */

ساكن hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_UNUSED,
				فراغ              *font_data HB_UNUSED,
				hb_font_extents_t *extents,
				فراغ              *user_data HB_UNUSED)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    فراغ              *font_data HB_UNUSED,
				    hb_font_extents_t *extents,
				    فراغ              *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_h_extents (extents, خطأ);
  إذا (ret) {
    extents->ascender = font->parent_scale_y_distance (extents->ascender);
    extents->descender = font->parent_scale_y_distance (extents->descender);
    extents->باريس_gap = font->parent_scale_y_distance (extents->باريس_gap);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_UNUSED,
				فراغ              *font_data HB_UNUSED,
				hb_font_extents_t *extents,
				فراغ              *user_data HB_UNUSED)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    فراغ              *font_data HB_UNUSED,
				    hb_font_extents_t *extents,
				    فراغ              *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_v_extents (extents, خطأ);
  إذا (ret) {
    extents->ascender = font->parent_scale_x_distance (extents->ascender);
    extents->descender = font->parent_scale_x_distance (extents->descender);
    extents->باريس_gap = font->parent_scale_x_distance (extents->باريس_gap);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_UNUSED,
			       فراغ           *font_data HB_UNUSED,
			       hb_codepoint_t  unicode HB_UNUSED,
			       hb_codepoint_t *glyph,
			       فراغ           *user_data HB_UNUSED)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   فراغ           *font_data HB_UNUSED,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   فراغ           *user_data HB_UNUSED)
{
  إذا (font->has_nominal_glyphs_func_set ())
  {
    أرجع font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  أرجع font->parent->get_nominal_glyph (unicode, glyph);
}

#define hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

ساكن unsigned صحيح
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    فراغ                 *font_data HB_UNUSED,
				    unsigned صحيح          کيراءن,
				    ثابت hb_codepoint_t *first_unicode,
				    unsigned صحيح          unicode_stride,
				    hb_codepoint_t       *first_glyph,
				    unsigned صحيح          glyph_stride,
				    فراغ                 *user_data HB_UNUSED)
{
  إذا (font->has_nominal_glyph_func_set ())
  {
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      إذا (!font->get_nominal_glyph (*first_unicode, first_glyph))
	أرجع i;

      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    أرجع کيراءن;
  }

  أرجع font->parent->get_nominal_glyphs (کيراءن,
					   first_unicode, unicode_stride,
					   first_glyph, glyph_stride);
}

ساكن hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_UNUSED,
				 فراغ           *font_data HB_UNUSED,
				 hb_codepoint_t  unicode HB_UNUSED,
				 hb_codepoint_t  variation_selector HB_UNUSED,
				 hb_codepoint_t *glyph,
				 فراغ           *user_data HB_UNUSED)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     فراغ           *user_data HB_UNUSED)
{
  أرجع font->parent->get_variation_glyph (unicode, variation_selector, glyph);
}


ساكن hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 فراغ           *font_data HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 فراغ           *user_data HB_UNUSED)
{
  أرجع font->x_scale;
}

ساكن hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph,
				     فراغ           *user_data HB_UNUSED)
{
  إذا (font->has_glyph_h_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, خطأ);
    أرجع ret;
  }
  أرجع font->parent_scale_x_distance (font->parent->get_glyph_h_advance (glyph, خطأ));
}

ساكن hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 فراغ           *font_data HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 فراغ           *user_data HB_UNUSED)
{
  أرجع -font->y_scale;
}

ساكن hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph,
				     فراغ           *user_data HB_UNUSED)
{
  إذا (font->has_glyph_v_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, خطأ);
    أرجع ret;
  }
  أرجع font->parent_scale_y_distance (font->parent->get_glyph_v_advance (glyph, خطأ));
}

#define hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

ساكن فراغ
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      فراغ*                 font_data HB_UNUSED,
				      unsigned صحيح          کيراءن,
				      ثابت hb_codepoint_t *first_glyph,
				      unsigned صحيح          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned صحيح          advance_stride,
				      فراغ                 *user_data HB_UNUSED)
{
  إذا (font->has_glyph_h_advance_func_set ())
  {
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      *first_advance = font->get_glyph_h_advance (*first_glyph, خطأ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    أرجع;
  }

  font->parent->get_glyph_h_advances (کيراءن,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      خطأ);
  لكل (unsigned صحيح i = 0; i < کيراءن; i++)
  {
    *first_advance = font->parent_scale_x_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#define hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
ساكن فراغ
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      فراغ*                 font_data HB_UNUSED,
				      unsigned صحيح          کيراءن,
				      ثابت hb_codepoint_t *first_glyph,
				      unsigned صحيح          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned صحيح          advance_stride,
				      فراغ                 *user_data HB_UNUSED)
{
  إذا (font->has_glyph_v_advance_func_set ())
  {
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      *first_advance = font->get_glyph_v_advance (*first_glyph, خطأ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    أرجع;
  }

  font->parent->get_glyph_v_advances (کيراءن,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      خطأ);
  لكل (unsigned صحيح i = 0; i < کيراءن; i++)
  {
    *first_advance = font->parent_scale_y_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

ساكن hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_UNUSED,
				فراغ           *font_data HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				فراغ           *user_data HB_UNUSED)
{
  *x = *y = 0;
  أرجع true;
}

ساكن hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    فراغ           *font_data HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    فراغ           *user_data HB_UNUSED)
{
  إذا (font->has_glyph_h_origins_func_set ())
  {
    أرجع font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, خطأ);
  }
  hb_bool_t ret = font->parent->get_glyph_h_origin (glyph, x, y);
  إذا (ret)
    font->parent_scale_position (x, y);
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_UNUSED,
				فراغ           *font_data HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				فراغ           *user_data HB_UNUSED)
{
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    فراغ           *font_data HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    فراغ           *user_data HB_UNUSED)
{
  إذا (font->has_glyph_v_origins_func_set ())
  {
    أرجع font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, خطأ);
  }
  hb_bool_t ret = font->parent->get_glyph_v_origin (glyph, x, y);
  إذا (ret)
    font->parent_scale_position (x, y);
  أرجع ret;
}

#define hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

ساكن hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_UNUSED,
				     فراغ *font_data HB_UNUSED,
				     unsigned صحيح کيراءن,
				     ثابت hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     فراغ *user_data HB_UNUSED)
{
  إذا (font->has_glyph_h_origin_func_set ())
  {
    hb_bool_t ret = true;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      ret &= font->get_glyph_h_origin (*first_glyph, first_x, first_y, خطأ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    أرجع ret;
  }

  hb_bool_t ret = font->parent->get_glyph_h_origins (کيراءن,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  إذا (ret)
  {
    لكل (unsigned i = 0; i < کيراءن; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  أرجع ret;
}

#define hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

ساكن hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_UNUSED,
				     فراغ *font_data HB_UNUSED,
				     unsigned صحيح کيراءن,
				     ثابت hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     فراغ *user_data HB_UNUSED)
{
  إذا (font->has_glyph_v_origin_func_set ())
  {
    hb_bool_t ret = true;
    لكل (unsigned صحيح i = 0; i < کيراءن; i++)
    {
      ret &= font->get_glyph_v_origin (*first_glyph, first_x, first_y, خطأ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    أرجع ret;
  }

  hb_bool_t ret = font->parent->get_glyph_v_origins (کيراءن,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  إذا (ret)
  {
    لكل (unsigned i = 0; i < کيراءن; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  أرجع ret;
}

ساكن hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_UNUSED,
				 فراغ           *font_data HB_UNUSED,
				 hb_codepoint_t  کيري_glyph HB_UNUSED,
				 hb_codepoint_t  کانن_glyph HB_UNUSED,
				 فراغ           *user_data HB_UNUSED)
{
  أرجع 0;
}

ساكن hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  کيري_glyph,
				     hb_codepoint_t  کانن_glyph,
				     فراغ           *user_data HB_UNUSED)
{
  أرجع font->parent_scale_x_distance (font->parent->get_glyph_h_kerning (کيري_glyph, کانن_glyph));
}

#ifndef HB_DISABLE_DEPRECATED
ساكن hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_UNUSED,
				 فراغ           *font_data HB_UNUSED,
				 hb_codepoint_t  اتس_glyph HB_UNUSED,
				 hb_codepoint_t  bottom_glyph HB_UNUSED,
				 فراغ           *user_data HB_UNUSED)
{
  أرجع 0;
}

ساكن hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  اتس_glyph,
				     hb_codepoint_t  bottom_glyph,
				     فراغ           *user_data HB_UNUSED)
{
  أرجع font->parent_scale_y_distance (font->parent->get_glyph_v_kerning (اتس_glyph, bottom_glyph));
}
#endif

ساكن hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_UNUSED,
			       فراغ               *font_data HB_UNUSED,
			       hb_codepoint_t      glyph HB_UNUSED,
			       hb_glyph_extents_t *extents,
			       فراغ               *user_data HB_UNUSED)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   فراغ               *font_data HB_UNUSED,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   فراغ               *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_extents (glyph, extents, خطأ);
  إذا (ret) {
    font->parent_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->parent_scale_distance (&extents->width, &extents->height);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_contour_point_nil (hb_font_t      *font HB_UNUSED,
				     فراغ           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph HB_UNUSED,
				     unsigned صحيح    point_index HB_UNUSED,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     فراغ           *user_data HB_UNUSED)
{
  *x = *y = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_contour_point_default (hb_font_t      *font,
					 فراغ           *font_data HB_UNUSED,
					 hb_codepoint_t  glyph,
					 unsigned صحيح    point_index,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 فراغ           *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_contour_point (glyph, point_index, x, y, خطأ);
  إذا (ret)
    font->parent_scale_position (x, y);
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_UNUSED,
			    فراغ           *font_data HB_UNUSED,
			    hb_codepoint_t  glyph HB_UNUSED,
			    حرف           *name,
			    unsigned صحيح    size,
			    فراغ           *user_data HB_UNUSED)
{
  إذا (size) *name = '\0';
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				فراغ           *font_data HB_UNUSED,
				hb_codepoint_t  glyph,
				حرف           *name,
				unsigned صحيح    size,
				فراغ           *user_data HB_UNUSED)
{
  أرجع font->parent->get_glyph_name (glyph, name, size);
}

ساكن hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_UNUSED,
				 فراغ           *font_data HB_UNUSED,
				 ثابت حرف     *name HB_UNUSED,
				 صحيح             len HB_UNUSED, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 فراغ           *user_data HB_UNUSED)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     فراغ           *font_data HB_UNUSED,
				     ثابت حرف     *name,
				     صحيح             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     فراغ           *user_data HB_UNUSED)
{
  أرجع font->parent->get_glyph_from_name (name, len, glyph);
}

ساكن hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_UNUSED,
				فراغ            *font_data HB_UNUSED,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				فراغ            *draw_data,
				فراغ            *user_data HB_UNUSED)
{
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_UNUSED,
				 فراغ *font_data HB_UNUSED,
				 hb_codepoint_t glyph HB_UNUSED,
				 hb_paint_funcs_t *paint_funcs HB_UNUSED,
				 فراغ *paint_data HB_UNUSED,
				 unsigned صحيح palette HB_UNUSED,
				 hb_color_t foreground HB_UNUSED,
				 فراغ *user_data HB_UNUSED)
{
  أرجع خطأ;
}

تعريفنوع بنية hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  فراغ		  *draw_data;
  عائم		   x_scale;
  عائم		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

ساكن فراغ
hb_draw_move_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 فراغ *draw_data,
			 hb_draw_state_t *st,
			 عائم to_x, عائم to_y,
			 فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_to (adaptor->draw_data, *st,
				     x_scale * to_x, y_scale * to_y);
}

ساكن فراغ
hb_draw_باريس_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			 hb_draw_state_t *st,
			 عائم to_x, عائم to_y,
			 فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_باريس_to (adaptor->draw_data, *st,
				     x_scale * to_x, y_scale * to_y);
}

ساكن فراغ
hb_draw_quadratic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			      hb_draw_state_t *st,
			      عائم control_x, عائم control_y,
			      عائم to_x, عائم to_y,
			      فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_to (adaptor->draw_data, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * to_x, y_scale * to_y);
}

ساكن فراغ
hb_draw_cubic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			  hb_draw_state_t *st,
			  عائم control1_x, عائم control1_y,
			  عائم control2_x, عائم control2_y,
			  عائم to_x, عائم to_y,
			  فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_to (adaptor->draw_data, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * to_x, y_scale * to_y);
}

ساكن فراغ
hb_draw_close_path_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			    hb_draw_state_t *st,
			    فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;

  adaptor->draw_funcs->emit_close_path (adaptor->draw_data, *st);
}

ساكن hb_bool_t
hb_draw_set_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			    int64_t budget,
			    فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  أرجع adaptor->draw_funcs->set_budget (adaptor->draw_data, budget);
}

ساكن int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
			    فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  أرجع adaptor->draw_funcs->get_budget (adaptor->draw_data);
}

ساكن int64_t *
hb_draw_get_budget_remaining_default (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *draw_data,
				      فراغ *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  أرجع adaptor->draw_funcs->get_budget_remaining_ptr (adaptor->draw_data);
}

ساكن ثابت hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  }
};

ساكن hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    فراغ            *font_data HB_UNUSED,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    فراغ            *draw_data,
				    فراغ            *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_data,
    font->parent->x_scale ? (عائم) font->x_scale / (عائم) font->parent->x_scale : 0.f,
    font->parent->y_scale ? (عائم) font->y_scale / (عائم) font->parent->y_scale : 0.f
  };

  أرجع font->parent->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   خطأ);
}

ساكن hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     فراغ *font_data,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     فراغ *paint_data,
				     unsigned صحيح palette,
				     hb_color_t foreground,
				     فراغ *user_data)
{
  paint_funcs->تولق_transform (paint_data,
    font->parent->x_scale ? (عائم) font->x_scale / (عائم) font->parent->x_scale : 0, 0,
    0, font->parent->y_scale ? (عائم) font->y_scale / (عائم) font->parent->y_scale : 0,
    0, 0);

  منطقي ret = font->parent->paint_glyph_or_fail (glyph, paint_funcs, paint_data, palette, foreground);

  paint_funcs->کلوار_transform (paint_data);

  أرجع ret;
}

DEFINE_NULL_INSTANCE (hb_font_funcs_t) =
{
  HB_OBJECT_HEADER_STATIC,

  nullptr,
  nullptr,
  {
    {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_nil,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    }
  }
};

ساكن ثابت hb_font_funcs_t _hb_font_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  nullptr,
  nullptr,
  {
    {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_default,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
    }
  }
};


/**
 * hb_font_funcs_create:
 *
 * Creates a new #hb_font_funcs_t structure of font functions.
 *
 * Return value: (transfer full): The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_create ()
{
  hb_font_funcs_t *ffuncs;

  إذا (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    أرجع hb_font_funcs_get_empty ();

  ffuncs->get = _hb_font_funcs_default.get;

  أرجع ffuncs;
}

/**
 * hb_font_funcs_get_empty:
 *
 * Fetches an empty font-functions structure.
 *
 * Return value: (transfer full): The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_get_empty ()
{
  أرجع const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
}

/**
 * hb_font_funcs_reference: (skip)
 * @ffuncs: The font-functions structure
 *
 * Increases the reference count on a font-functions structure.
 *
 * Return value: The font-functions structure
 *
 * Since: 0.9.2
 **/
hb_font_funcs_t *
hb_font_funcs_reference (hb_font_funcs_t *ffuncs)
{
  أرجع hb_object_reference (ffuncs);
}

/**
 * hb_font_funcs_destroy: (skip)
 * @ffuncs: The font-functions structure
 *
 * Decreases the reference count on a font-functions structure. When
 * the reference count reaches zero, the font-functions structure is
 * destroyed, freeing all memory.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  إذا (!hb_object_destroy (ffuncs)) أرجع;

  إذا (ffuncs->destroy)
  {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) إذا (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_data ? nullptr : ffuncs->user_data->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_data);

  hb_free (ffuncs);
}

/**
 * hb_font_funcs_set_user_data: (skip)
 * @ffuncs: The font-functions structure
 * @key: The user-data key to set
 * @data: A pointer to the user data set
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified font-functions structure.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_funcs_set_user_data (hb_font_funcs_t    *ffuncs,
			     hb_user_data_کونچي_t *key,
			     فراغ *              data,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  أرجع hb_object_set_user_data (ffuncs, key, data, destroy, replace);
}

/**
 * hb_font_funcs_get_user_data: (skip)
 * @ffuncs: The font-functions structure
 * @key: The user-data key to query
 *
 * Fetches the user data associated with the specified key,
 * attached to the specified font-functions structure.
 *
 * Return value: (transfer none): A pointer to the user data
 *
 * Since: 0.9.2
 **/
فراغ *
hb_font_funcs_get_user_data (ثابت hb_font_funcs_t *ffuncs,
			     hb_user_data_کونچي_t    *key)
{
  أرجع hb_object_get_user_data (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  إذا (hb_object_is_immutable (ffuncs))
    أرجع;

  hb_object_make_immutable (ffuncs);
}

/**
 * hb_font_funcs_is_immutable:
 * @ffuncs: The font-functions structure
 *
 * Tests whether a font-functions structure is immutable.
 *
 * Return value: `true` if @ffuncs is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_funcs_is_immutable (hb_font_funcs_t *ffuncs)
{
  أرجع hb_object_is_immutable (ffuncs);
}


ساكن منطقي
_hb_font_funcs_set_preamble (hb_font_funcs_t    *ffuncs,
			     منطقي                func_is_null,
			     فراغ              **user_data,
			     hb_destroy_func_t  *destroy)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (*destroy)
      (*destroy) (*user_data);
    أرجع خطأ;
  }

  إذا (func_is_null)
  {
    إذا (*destroy)
      (*destroy) (*user_data);
    *destroy = nullptr;
    *user_data = nullptr;
  }

  أرجع true;
}

ساكن منطقي
hb_font_funcs_set_تڠه (hb_font_funcs_t   *ffuncs,
			   فراغ              *user_data,
			   hb_destroy_func_t  destroy)
{
  auto destroy_guard = hb_make_scope_guard ([&]() {
    إذا (destroy) destroy (user_data);
  });

  إذا (user_data && !ffuncs->user_data)
  {
    ffuncs->user_data = (decltype (ffuncs->user_data)) hb_calloc (1, حجم (*ffuncs->user_data));
    إذا (unlikely (!ffuncs->user_data))
      أرجع خطأ;
  }
  إذا (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, حجم (*ffuncs->destroy));
    إذا (unlikely (!ffuncs->destroy))
      أرجع خطأ;
  }

  destroy_guard.release ();
  أرجع true;
}

#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
فراغ                                                                     \
hb_font_funcs_set_##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 فراغ                        *user_data, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  إذا (!_hb_font_funcs_set_preamble (ffuncs, !func, &user_data, &destroy))\
      أرجع;                                                            \
									 \
  إذا (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_data ? nullptr : ffuncs->user_data->name); \
                                                                         \
  إذا (!hb_font_funcs_set_تڠه (ffuncs, user_data, destroy))           \
      أرجع;                                                            \
									 \
  إذا (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  وإلا                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  إذا (ffuncs->user_data)                                                 \
    ffuncs->user_data->name = user_data;                                 \
  إذا (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

منطقي
hb_font_t::has_func_set (unsigned صحيح i)
{
  أرجع هذا->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

منطقي
hb_font_t::has_func (unsigned صحيح i)
{
  أرجع has_func_set (i) ||
	 (parent && parent != &_hb_Null_hb_font_t && parent->has_func (i));
}

/* Public getters */

/**
 * hb_font_get_h_extents:
 * @font: #hb_font_t to work upon
 * @extents: (out): The font extents retrieved
 *
 * Fetches the extents for a specified font, for horizontal
 * text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.1.3
 **/
hb_bool_t
hb_font_get_h_extents (hb_font_t         *font,
		       hb_font_extents_t *extents)
{
  أرجع font->get_font_h_extents (extents);
}

/**
 * hb_font_get_v_extents:
 * @font: #hb_font_t to work upon
 * @extents: (out): The font extents retrieved
 *
 * Fetches the extents for a specified font, for vertical
 * text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.1.3
 **/
hb_bool_t
hb_font_get_v_extents (hb_font_t         *font,
		       hb_font_extents_t *extents)
{
  أرجع font->get_font_v_extents (extents);
}

/**
 * hb_font_get_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @variation_selector: A variation-selector code point
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID for a Unicode code point in the specified
 * font, with an optional variation selector.
 *
 * If @variation_selector is 0, calls hb_font_get_nominal_glyph();
 * otherwise calls hb_font_get_variation_glyph().
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph (hb_font_t      *font,
		   hb_codepoint_t  unicode,
		   hb_codepoint_t  variation_selector,
		   hb_codepoint_t *glyph)
{
  إذا (unlikely (variation_selector))
    أرجع font->get_variation_glyph (unicode, variation_selector, glyph);
  أرجع font->get_nominal_glyph (unicode, glyph);
}

/**
 * hb_font_get_nominal_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the nominal glyph ID for a Unicode code point in the
 * specified font.
 *
 * This version of the function should not be used to fetch glyph IDs
 * for code points modified by variation selectors. For variation-selector
 * support, user hb_font_get_variation_glyph() or use hb_font_get_glyph().
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.2.3
 **/
hb_bool_t
hb_font_get_nominal_glyph (hb_font_t      *font,
			   hb_codepoint_t  unicode,
			   hb_codepoint_t *glyph)
{
  أرجع font->get_nominal_glyph (unicode, glyph);
}

/**
 * hb_font_get_nominal_glyphs:
 * @font: #hb_font_t to work upon
 * @count: number of code points to query
 * @first_unicode: The first Unicode code point to query
 * @unicode_stride: The stride between successive code points
 * @first_glyph: (out): The first glyph ID retrieved
 * @glyph_stride: The stride between successive glyph IDs
 *
 * Fetches the nominal glyph IDs for a sequence of Unicode code points. Glyph
 * IDs must be returned in a #hb_codepoint_t output parameter. Stops at the
 * first unsupported glyph ID.
 *
 * Return value: the number of code points processed
 *
 * Since: 2.6.3
 **/
unsigned صحيح
hb_font_get_nominal_glyphs (hb_font_t *font,
			    unsigned صحيح کيراءن,
			    ثابت hb_codepoint_t *first_unicode,
			    unsigned صحيح unicode_stride,
			    hb_codepoint_t *first_glyph,
			    unsigned صحيح glyph_stride)
{
  أرجع font->get_nominal_glyphs (کيراءن,
				   first_unicode, unicode_stride,
				   first_glyph, glyph_stride);
}

/**
 * hb_font_get_variation_glyph:
 * @font: #hb_font_t to work upon
 * @unicode: The Unicode code point to query
 * @variation_selector: The  variation-selector code point to query
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID for a Unicode code point when followed by
 * by the specified variation-selector code point, in the specified
 * font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 1.2.3
 **/
hb_bool_t
hb_font_get_variation_glyph (hb_font_t      *font,
			     hb_codepoint_t  unicode,
			     hb_codepoint_t  variation_selector,
			     hb_codepoint_t *glyph)
{
  أرجع font->get_variation_glyph (unicode, variation_selector, glyph);
}

/**
 * hb_font_get_glyph_h_advance:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 *
 * Fetches the advance for a glyph ID in the specified font,
 * for horizontal text segments.
 *
 * Return value: The advance of @glyph within @font
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_h_advance (hb_font_t      *font,
			     hb_codepoint_t  glyph)
{
  أرجع font->get_glyph_h_advance (glyph);
}

/**
 * hb_font_get_glyph_v_advance:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 *
 * Fetches the advance for a glyph ID in the specified font,
 * for vertical text segments.
 *
 * Return value: The advance of @glyph within @font
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_v_advance (hb_font_t      *font,
			     hb_codepoint_t  glyph)
{
  أرجع font->get_glyph_v_advance (glyph);
}

/**
 * hb_font_get_glyph_h_advances:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, for horizontal text segments.
 *
 * Since: 1.8.6
 **/
فراغ
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      unsigned صحيح          کيراءن,
			      ثابت hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_h_advances (کيراءن, first_glyph, glyph_stride, first_advance, advance_stride);
}
/**
 * hb_font_get_glyph_v_advances:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: (out): The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, for vertical text segments.
 *
 * Since: 1.8.6
 **/
فراغ
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      unsigned صحيح          کيراءن,
			      ثابت hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_v_advances (کيراءن, first_glyph, glyph_stride, first_advance, advance_stride);
}

/**
 * hb_font_get_glyph_h_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @x: (out): The X coordinate of the origin
 * @y: (out): The Y coordinate of the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph ID
 * in the specified font, for horizontal text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_h_origin (hb_font_t      *font,
			    hb_codepoint_t  glyph,
			    hb_position_t  *x,
			    hb_position_t  *y)
{
  أرجع font->get_glyph_h_origin (glyph, x, y);
}

/**
 * hb_font_get_glyph_v_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @x: (out): The X coordinate of the origin
 * @y: (out): The Y coordinate of the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph ID
 * in the specified font, for vertical text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_v_origin (hb_font_t      *font,
			    hb_codepoint_t  glyph,
			    hb_position_t  *x,
			    hb_position_t  *y)
{
  أرجع font->get_glyph_v_origin (glyph, x, y);
}

/**
 * hb_font_get_glyph_h_origins:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_x: (out): The first X coordinate of the origin retrieved
 * @x_stride: The stride between successive X coordinates
 * @first_y: (out): The first Y coordinate of the origin retrieved
 * @y_stride: The stride between successive Y coordinates
 *
 * Fetches the (X,Y) coordinates of the origin for requested glyph IDs
 * in the specified font, for horizontal text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 11.3.0
 **/
hb_bool_t
hb_font_get_glyph_h_origins (hb_font_t      *font,
			     unsigned صحيح    کيراءن,
			     ثابت hb_codepoint_t *first_glyph,
			     unsigned صحيح    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned صحيح    x_stride,
			     hb_position_t  *first_y,
			     unsigned صحيح    y_stride)

{
  أرجع font->get_glyph_h_origins (کيراءن,
				    first_glyph, glyph_stride,
				    first_x, x_stride,
				    first_y, y_stride);
}

/**
 * hb_font_get_glyph_v_origins:
 * @font: #hb_font_t to work upon
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_x: (out): The first X coordinate of the origin retrieved
 * @x_stride: The stride between successive X coordinates
 * @first_y: (out): The first Y coordinate of the origin retrieved
 * @y_stride: The stride between successive Y coordinates
 *
 * Fetches the (X,Y) coordinates of the origin for requested glyph IDs
 * in the specified font, for vertical text segments.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 11.3.0
 **/
hb_bool_t
hb_font_get_glyph_v_origins (hb_font_t      *font,
			     unsigned صحيح    کيراءن,
			     ثابت hb_codepoint_t *first_glyph,
			     unsigned صحيح    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned صحيح    x_stride,
			     hb_position_t  *first_y,
			     unsigned صحيح    y_stride)

{
  أرجع font->get_glyph_v_origins (کيراءن,
				    first_glyph, glyph_stride,
				    first_x, x_stride,
				    first_y, y_stride);
}


/**
 * hb_font_get_glyph_h_kerning:
 * @font: #hb_font_t to work upon
 * @left_glyph: The glyph ID of the left glyph in the glyph pair
 * @right_glyph: The glyph ID of the right glyph in the glyph pair
 *
 * Fetches the kerning-adjustment value for a glyph-pair in
 * the specified font, for horizontal text segments.
 *
 * <note>It handles legacy kerning only (as returned by the corresponding
 * #hb_font_funcs_t function).</note>
 *
 * Return value: The kerning adjustment value
 *
 * Since: 0.9.2
 **/
hb_position_t
hb_font_get_glyph_h_kerning (hb_font_t      *font,
			     hb_codepoint_t  کيري_glyph,
			     hb_codepoint_t  کانن_glyph)
{
  أرجع font->get_glyph_h_kerning (کيري_glyph, کانن_glyph);
}

#ifndef HB_DISABLE_DEPRECATED
/**
 * hb_font_get_glyph_v_kerning:
 * @font: #hb_font_t to work upon
 * @top_glyph: The glyph ID of the top glyph in the glyph pair
 * @bottom_glyph: The glyph ID of the bottom glyph in the glyph pair
 *
 * Fetches the kerning-adjustment value for a glyph-pair in
 * the specified font, for vertical text segments.
 *
 * <note>It handles legacy kerning only (as returned by the corresponding
 * #hb_font_funcs_t function).</note>
 *
 * Return value: The kerning adjustment value
 *
 * Since: 0.9.2
 * Deprecated: 2.0.0
 **/
hb_position_t
hb_font_get_glyph_v_kerning (hb_font_t      *font,
			     hb_codepoint_t  اتس_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  أرجع font->get_glyph_v_kerning (اتس_glyph, bottom_glyph);
}
#endif

/**
 * hb_font_get_glyph_extents:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @extents: (out): The #hb_glyph_extents_t retrieved
 *
 * Fetches the #hb_glyph_extents_t data for a glyph ID
 * in the specified font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_extents (hb_font_t          *font,
			   hb_codepoint_t      glyph,
			   hb_glyph_extents_t *extents)
{
  أرجع font->get_glyph_extents (glyph, extents);
}

/**
 * hb_font_get_glyph_contour_point:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @point_index: The contour-point index to query
 * @x: (out): The X value retrieved for the contour point
 * @y: (out): The Y value retrieved for the contour point
 *
 * Fetches the (x,y) coordinates of a specified contour-point index
 * in the specified glyph, within the specified font.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_contour_point (hb_font_t      *font,
				 hb_codepoint_t  glyph,
				 unsigned صحيح    point_index,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  أرجع font->get_glyph_contour_point (glyph, point_index, x, y);
}

/**
 * hb_font_get_glyph_name:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @name: (out) (array length=size): Name string retrieved for the glyph ID
 * @size: Length of the glyph-name string retrieved
 *
 * Fetches the glyph-name string for a glyph ID in the specified @font.
 *
 * According to the OpenType specification, glyph names are limited to 63
 * characters and can only contain (a subset of) ASCII.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_name (hb_font_t      *font,
			hb_codepoint_t  glyph,
			حرف           *name,
			unsigned صحيح    size)
{
  أرجع font->get_glyph_name (glyph, name, size);
}

/**
 * hb_font_get_glyph_from_name:
 * @font: #hb_font_t to work upon
 * @name: (array length=len): The name string to query
 * @len: The length of the name queried
 * @glyph: (out): The glyph ID retrieved
 *
 * Fetches the glyph ID that corresponds to a name string in the specified @font.
 *
 * <note>Note: @len == -1 means the name string is null-terminated.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_from_name (hb_font_t      *font,
			     ثابت حرف     *name,
			     صحيح             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  أرجع font->get_glyph_from_name (name, len, glyph);
}

#ifndef HB_DISABLE_DEPRECATED
/**
 * hb_font_get_glyph_shape:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Fetches the glyph shape that corresponds to a glyph in the specified @font.
 * The shape is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Since: 4.0.0
 * Deprecated: 7.0.0: Use hb_font_draw_glyph() instead
 */
فراغ
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, فراغ *draw_data)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_data);
}
#endif

/**
 * hb_font_draw_glyph_or_fail:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Draws the outline that corresponds to a glyph in the specified @font.
 *
 * This is a newer name for hb_font_draw_glyph(), that returns `false`
 * if the font has no outlines for the glyph.
 *
 * The outline is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Return value: `true` if glyph was drawn, `false` otherwise
 *
 * Since: 11.2.0
 **/
hb_bool_t
hb_font_draw_glyph_or_fail (hb_font_t *font,
			    hb_codepoint_t glyph,
			    hb_draw_funcs_t *dfuncs, فراغ *draw_data)
{
  أرجع font->draw_glyph_or_fail (glyph, dfuncs, draw_data);
}

/**
 * hb_font_paint_glyph_or_fail:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @pfuncs: #hb_paint_funcs_t to paint with
 * @paint_data: User data to pass to paint callbacks
 * @palette_index: The index of the font's color palette to use
 * @foreground: The foreground color, unpremultipled
 *
 * Paints a color glyph.
 *
 * Succeeds if @glyph has color paint layers (COLRv0),
 * a color paint graph (COLRv1), or a bitmap image that the
 * font's callbacks render successfully.  Returns `false` if
 * the font has no color data for @glyph; the client can then
 * fall back to hb_font_draw_glyph_or_fail() for the monochrome
 * outline.
 *
 * The painting instructions are returned by way of calls to
 * the callbacks of the @funcs object, with @paint_data passed
 * to them.
 *
 * If the font has color palettes (see hb_ot_color_has_palettes()),
 * then @palette_index selects the palette to use. If the font only
 * has one palette, this will be 0.
 *
 * Return value: `true` if glyph was painted, `false` otherwise
 *
 * Since: 11.2.0
 */
hb_bool_t
hb_font_paint_glyph_or_fail (hb_font_t *font,
			     hb_codepoint_t glyph,
			     hb_paint_funcs_t *pfuncs, فراغ *paint_data,
			     unsigned صحيح palette_index,
			     hb_color_t foreground)
{
  أرجع font->paint_glyph_or_fail (glyph, pfuncs, paint_data, palette_index, foreground);
}

/* A bit higher-level, and with fallback */

فراغ
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, فراغ *paint_data,
			unsigned صحيح palette,
			hb_color_t foreground)
{
  إذا (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_data,
			   palette, foreground))
    أرجع;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_data, glyph, هذا, true, foreground);
}


/**
 * hb_font_draw_glyph:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @dfuncs: #hb_draw_funcs_t to draw to
 * @draw_data: User data to pass to draw callbacks
 *
 * Draws the outline that corresponds to a glyph in the specified @font.
 *
 * This is an older name for hb_font_draw_glyph_or_fail(), with no
 * return value.
 *
 * The outline is returned by way of calls to the callbacks of the @dfuncs
 * objects, with @draw_data passed to them.
 *
 * Since: 7.0.0
 **/
فراغ
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, فراغ *draw_data)
{
  (فراغ) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_data);
}

/**
 * hb_font_paint_glyph:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID
 * @pfuncs: #hb_paint_funcs_t to paint with
 * @paint_data: User data to pass to paint callbacks
 * @palette_index: The index of the font's color palette to use
 * @foreground: The foreground color, unpremultipled
 *
 * Paints the glyph. This function is similar to
 * hb_font_paint_glyph_or_fail(), but if painting a color glyph
 * failed, it will fall back to painting an outline monochrome
 * glyph.
 *
 * The painting instructions are returned by way of calls to
 * the callbacks of the @funcs object, with @paint_data passed
 * to them.
 *
 * If the font has color palettes (see hb_ot_color_has_palettes()),
 * then @palette_index selects the palette to use. If the font only
 * has one palette, this will be 0.
 *
 * Since: 7.0.0
 */
فراغ
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, فراغ *paint_data,
                     unsigned صحيح palette_index,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_data, palette_index, foreground);
}

/**
 * hb_font_get_extents_for_direction:
 * @font: #hb_font_t to work upon
 * @direction: The direction of the text segment
 * @extents: (out): The #hb_font_extents_t retrieved
 *
 * Fetches the extents for a font in a text segment of the
 * specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 1.1.3
 **/
فراغ
hb_font_get_extents_for_direction (hb_font_t         *font,
				   hb_direction_t     direction,
				   hb_font_extents_t *extents)
{
  font->get_extents_for_direction (direction, extents);
}
/**
 * hb_font_get_glyph_advance_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (out): The horizontal advance retrieved
 * @y: (out):  The vertical advance retrieved
 *
 * Fetches the advance for a glyph ID from the specified font,
 * in a text segment of the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_get_glyph_advance_for_direction (hb_font_t      *font,
					 hb_codepoint_t  glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  font->get_glyph_advance_for_direction (glyph, direction, x, y);
}
/**
 * hb_font_get_glyph_advances_for_direction:
 * @font: #hb_font_t to work upon
 * @direction: The direction of the text segment
 * @count: The number of glyph IDs in the sequence queried
 * @first_glyph: The first glyph ID to query
 * @glyph_stride: The stride between successive glyph IDs
 * @first_advance: (out): The first advance retrieved
 * @advance_stride: (out): The stride between successive advances
 *
 * Fetches the advances for a sequence of glyph IDs in the specified
 * font, in a text segment of the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 1.8.6
 **/
HB_EXTERN فراغ
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  unsigned صحيح          کيراءن,
					  ثابت hb_codepoint_t *first_glyph,
					  unsigned              glyph_stride,
					  hb_position_t        *first_advance,
					  unsigned              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, کيراءن, first_glyph, glyph_stride, first_advance, advance_stride);
}

/**
 * hb_font_get_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (out): The X coordinate retrieved for the origin
 * @y: (out): The Y coordinate retrieved for the origin
 *
 * Fetches the (X,Y) coordinates of the origin for a glyph in
 * the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  أرجع font->get_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_add_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (inout): Input = The original X coordinate
 *     Output = The X coordinate plus the X-coordinate of the origin
 * @y: (inout): Input = The original Y coordinate
 *     Output = The Y coordinate plus the Y-coordinate of the origin
 *
 * Adds the origin coordinates to an (X,Y) point coordinate, in
 * the specified glyph ID in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_add_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  أرجع font->add_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_subtract_glyph_origin_for_direction:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @x: (inout): Input = The original X coordinate
 *     Output = The X coordinate minus the X-coordinate of the origin
 * @y: (inout): Input = The original Y coordinate
 *     Output = The Y coordinate minus the Y-coordinate of the origin
 *
 * Subtracts the origin coordinates from an (X,Y) point coordinate,
 * in the specified glyph ID in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_subtract_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  أرجع font->subtract_glyph_origin_for_direction (glyph, direction, x, y);
}

/**
 * hb_font_get_glyph_kerning_for_direction:
 * @font: #hb_font_t to work upon
 * @first_glyph: The glyph ID of the first glyph in the glyph pair to query
 * @second_glyph: The glyph ID of the second glyph in the glyph pair to query
 * @direction: The direction of the text segment
 * @x: (out): The horizontal kerning-adjustment value retrieved
 * @y: (out): The vertical kerning-adjustment value retrieved
 *
 * Fetches the kerning-adjustment value for a glyph-pair in the specified font.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  first_glyph,
					 hb_codepoint_t  second_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  أرجع font->get_glyph_kerning_for_direction (first_glyph, second_glyph, direction, x, y);
}

/**
 * hb_font_get_glyph_extents_for_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @direction: The direction of the text segment
 * @extents: (out): The #hb_glyph_extents_t retrieved
 *
 * Fetches the #hb_glyph_extents_t data for a glyph ID
 * in the specified font, with respect to the origin in
 * a text segment in the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_extents_for_origin (hb_font_t          *font,
				      hb_codepoint_t      glyph,
				      hb_direction_t      direction,
				      hb_glyph_extents_t *extents)
{
  أرجع font->get_glyph_extents_for_origin (glyph, direction, extents);
}

/**
 * hb_font_get_glyph_contour_point_for_origin:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @point_index: The contour-point index to query
 * @direction: The direction of the text segment
 * @x: (out): The X value retrieved for the contour point
 * @y: (out): The Y value retrieved for the contour point
 *
 * Fetches the (X,Y) coordinates of a specified contour-point index
 * in the specified glyph ID in the specified font, with respect
 * to the origin in a text segment in the specified direction.
 *
 * Calls the appropriate direction-specific variant (horizontal
 * or vertical) depending on the value of @direction.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_get_glyph_contour_point_for_origin (hb_font_t      *font,
					    hb_codepoint_t  glyph,
					    unsigned صحيح    point_index,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  أرجع font->get_glyph_contour_point_for_origin (glyph, point_index, direction, x, y);
}

/**
 * hb_font_glyph_to_string:
 * @font: #hb_font_t to work upon
 * @glyph: The glyph ID to query
 * @s: (out) (array length=size): The string containing the glyph name
 * @size: Length of string @s
 *
 * Fetches the name of the specified glyph ID in @font and returns
 * it in string @s.
 *
 * If the glyph ID has no name in @font, a string of the form `gidDDD` is
 * generated, with `DDD` being the glyph ID.
 *
 * According to the OpenType specification, glyph names are limited to 63
 * characters and can only contain (a subset of) ASCII.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_glyph_to_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 حرف           *s,
			 unsigned صحيح    size)
{
  font->glyph_to_string (glyph, s, size);
}

/**
 * hb_font_glyph_from_string:
 * @font: #hb_font_t to work upon
 * @s: (array length=len) (element-type uint8_t): string to query
 * @len: The length of the string @s
 * @glyph: (out): The glyph ID corresponding to the string requested
 *
 * Fetches the glyph ID from @font that matches the specified string.
 * Strings of the format `gidDDD` or `uniUUUU` are parsed automatically.
 *
 * <note>Note: @len == -1 means the string is null-terminated.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_glyph_from_string (hb_font_t      *font,
			   ثابت حرف     *s,
			   صحيح             len,
			   hb_codepoint_t *glyph)
{
  أرجع font->glyph_from_string (s, len, glyph);
}


/*
 * hb_font_t
 */

DEFINE_NULL_INSTANCE (hb_font_t) =
{
  HB_OBJECT_HEADER_STATIC,

  0, /* serial */
  0, /* serial_coords */

  nullptr, /* parent */
  const_cast<hb_face_t *> (&_hb_Null_hb_face_t),

  1000, /* x_scale */
  1000, /* y_scale */
  خطأ, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  true, /* embolden_in_place */
  0, /* x_strength */
  0, /* y_strength */
  0.f, /* slant */
  0.f, /* slant_xy; */
  1.f, /* x_multf */
  1.f, /* y_multf */
  1<<16, /* x_mult */
  1<<16, /* y_mult */

  0, /* x_ppem */
  0, /* y_ppem */
  0, /* ptem */

  HB_FONT_NO_VAR_NAMED_INSTANCE, /* instance_index */
  خطأ, /* has_nonzero_coords */
  0, /* num_coords */
  nullptr, /* coords */
  nullptr, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


ساكن hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  إذا (unlikely (!face))
    face = hb_face_get_empty ();

  إذا (!(font = hb_object_create<hb_font_t> ()))
    أرجع hb_font_get_empty ();

  hb_face_make_immutable (face);
  font->parent = hb_font_get_empty ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_empty ();
  font->data.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_in_place = true;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_index = HB_FONT_NO_VAR_NAMED_INSTANCE;

  أرجع font;
}

/**
 * hb_font_create:
 * @face: a face.
 *
 * Constructs a new font object from the specified face.
 *
 * <note>Note: If @face's index value (as passed to hb_face_create()
 * has non-zero top 16-bits, those bits minus one are passed to
 * hb_font_set_var_named_instance(), effectively loading a named-instance
 * of a variable font, instead of the default-instance.  This allows
 * specifying which named-instance to load by default when creating the
 * face.</note>
 *
 * Return value: (transfer full): The new font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_create (hb_face_t *face)
{
  hb_font_t *font = _hb_font_create (face);

  hb_font_set_funcs_using (font, nullptr);

#ifndef HB_NO_VAR
  // Initialize variations.
  إذا (likely (face))
  {
    إذا (face->index >> 16)
      hb_font_set_var_named_instance (font, (face->index >> 16) - 1);
    وإلا
      hb_font_set_variations (font, nullptr, 0);
  }
#endif

  أرجع font;
}

ساكن فراغ
_hb_font_adopt_var_coords (hb_font_t *font,
			   صحيح *coords, /* 2.14 normalized */
			   عائم *design_coords,
			   unsigned صحيح coords_length)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->اڠک_coords = coords_length;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_length));

  font->changed ();
  font->serial_coords = font->serial;
}

/**
 * hb_font_create_sub_font:
 * @parent: The parent font object
 *
 * Constructs a sub-font font object from the specified @parent font,
 * replicating the parent's properties.
 *
 * Return value: (transfer full): The new sub-font font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_create_sub_font (hb_font_t *parent)
{
  إذا (unlikely (!parent))
    parent = hb_font_get_empty ();

  hb_font_t *font = _hb_font_create (parent->face);

  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع font;

  font->parent = hb_font_reference (parent);

  font->x_scale = parent->x_scale;
  font->y_scale = parent->y_scale;
  font->x_embolden = parent->x_embolden;
  font->y_embolden = parent->y_embolden;
  font->embolden_in_place = parent->embolden_in_place;
  font->slant = parent->slant;
  font->x_ppem = parent->x_ppem;
  font->y_ppem = parent->y_ppem;
  font->ptem = parent->ptem;

  unsigned صحيح اڠک_coords = parent->اڠک_coords;
  إذا (اڠک_coords)
  {
    صحيح *coords = (صحيح *) hb_calloc (اڠک_coords, حجم (parent->coords[0]));
    عائم *design_coords = (عائم *) hb_calloc (اڠک_coords, حجم (parent->design_coords[0]));
    إذا (likely (coords && design_coords))
    {
      hb_memcpy (coords, parent->coords, اڠک_coords * حجم (parent->coords[0]));
      hb_memcpy (design_coords, parent->design_coords, اڠک_coords * حجم (parent->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, اڠک_coords);
    }
    وإلا
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->changed ();
  font->serial_coords = font->serial;

  أرجع font;
}

/**
 * hb_font_get_empty:
 *
 * Fetches the empty font object.
 *
 * Return value: (transfer full): The empty font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_get_empty ()
{
  أرجع const_cast<hb_font_t *> (&Null (hb_font_t));
}

/**
 * hb_font_reference: (skip)
 * @font: #hb_font_t to work upon
 *
 * Increases the reference count on the given font object.
 *
 * Return value: (transfer full): The @font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_reference (hb_font_t *font)
{
  أرجع hb_object_reference (font);
}

/**
 * hb_font_destroy: (skip)
 * @font: #hb_font_t to work upon
 *
 * Decreases the reference count on the given font object. When the
 * reference count reaches zero, the font is destroyed,
 * freeing all memory.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_destroy (hb_font_t *font)
{
  إذا (!hb_object_destroy (font)) أرجع;

  font->data.fini ();

  إذا (font->destroy)
    font->destroy (font->user_data);

  hb_font_destroy (font->parent);
  hb_face_destroy (font->face);
  hb_font_funcs_destroy (font->klass);

  hb_free (font->coords);
  hb_free (font->design_coords);

  hb_free (font);
}

/**
 * hb_font_set_user_data: (skip)
 * @font: #hb_font_t to work upon
 * @key: The user-data key
 * @data: A pointer to the user data
 * @destroy: (nullable): A callback to call when @data is not needed anymore
 * @replace: Whether to replace an existing data with the same key
 *
 * Attaches a user-data key/data pair to the specified font object.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_set_user_data (hb_font_t          *font,
		       hb_user_data_کونچي_t *key,
		       فراغ *              data,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  إذا (!hb_object_is_immutable (font))
    font->changed ();

  أرجع hb_object_set_user_data (font, key, data, destroy, replace);
}

/**
 * hb_font_get_user_data: (skip)
 * @font: #hb_font_t to work upon
 * @key: The user-data key to query
 *
 * Fetches the user-data object associated with the specified key,
 * attached to the specified font object.
 *
 * Return value: (transfer none): Pointer to the user data
 *
 * Since: 0.9.2
 **/
فراغ *
hb_font_get_user_data (ثابت hb_font_t    *font,
		       hb_user_data_کونچي_t *key)
{
  أرجع hb_object_get_user_data (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_make_immutable (hb_font_t *font)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->parent)
    hb_font_make_immutable (font->parent);

  hb_object_make_immutable (font);
}

/**
 * hb_font_is_immutable:
 * @font: #hb_font_t to work upon
 *
 * Tests whether a font object is immutable.
 *
 * Return value: `true` if @font is immutable, `false` otherwise
 *
 * Since: 0.9.2
 **/
hb_bool_t
hb_font_is_immutable (hb_font_t *font)
{
  أرجع hb_object_is_immutable (font);
}

/**
 * hb_font_get_serial:
 * @font: #hb_font_t to work upon
 *
 * Returns the internal serial number of the font. The serial
 * number is increased every time a setting on the font is
 * changed, using a setter function.
 *
 * Return value: serial number
 *
 * Since: 4.4.0
 **/
unsigned صحيح
hb_font_get_serial (hb_font_t *font)
{
  أرجع font->serial.get_acquire ();
}

/**
 * hb_font_changed:
 * @font: #hb_font_t to work upon
 *
 * Notifies the @font that underlying font data has changed.
 * This has the effect of increasing the serial as returned
 * by hb_font_get_serial(), which invalidates internal caches.
 *
 * Since: 4.4.0
 **/
فراغ
hb_font_changed (hb_font_t *font)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  font->changed ();
}

/**
 * hb_font_set_parent:
 * @font: #hb_font_t to work upon
 * @parent: The parent font object to assign
 *
 * Sets the parent font of @font.
 *
 * Since: 1.0.5
 **/
فراغ
hb_font_set_parent (hb_font_t *font,
		    hb_font_t *parent)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (parent == font->parent)
    أرجع;

  إذا (!parent)
    parent = hb_font_get_empty ();

  لكل (hb_font_t *p = parent; p && p != hb_font_get_empty(); p = p->parent)
    إذا (p == font)
      أرجع; /* Would create a cycle - reject */

  hb_font_t *old = font->parent;

  font->parent = hb_font_reference (parent);

  hb_font_destroy (old);

  font->changed ();
}

/**
 * hb_font_get_parent:
 * @font: #hb_font_t to work upon
 *
 * Fetches the parent font of @font.
 *
 * Return value: (transfer none): The parent font object
 *
 * Since: 0.9.2
 **/
hb_font_t *
hb_font_get_parent (hb_font_t *font)
{
  أرجع font->parent;
}

#ifndef HB_DISABLE_DEPRECATED
/**
 * hb_font_set_face:
 * @font: #hb_font_t to work upon
 * @face: The #hb_face_t to assign
 *
 * Sets @face as the font-face value of @font.
 *
 * Font functions and their data may hold pointers into the old face,
 * so the font functions of @font are released and replaced with the
 * default ones for @face, as hb_font_create() would set up.  Other
 * font properties, such as scale and variations, are kept.
 *
 * Since: 1.4.3
 * XDeprecated: REPLACEME: Use hb_font_create() instead.
 **/
فراغ
hb_font_set_face (hb_font_t *font,
		  hb_face_t *face)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (face == font->face)
    أرجع;

  إذا (unlikely (!face))
    face = hb_face_get_empty ();

  hb_face_t *old = font->face;

  hb_face_make_immutable (face);
  font->face = hb_face_reference (face);
  font->changed ();

  /* Release the old font funcs while the old face is still alive. */
  hb_font_set_funcs (font, nullptr, nullptr, nullptr);
  hb_font_set_funcs_using (font, nullptr);

  hb_face_destroy (old);

  font->changed ();
  font->serial_coords = font->serial;
}
#endif

/**
 * hb_font_get_face:
 * @font: #hb_font_t to work upon
 *
 * Fetches the face associated with the specified font object.
 *
 * Return value: (transfer none): The #hb_face_t value
 *
 * Since: 0.9.2
 **/
hb_face_t *
hb_font_get_face (hb_font_t *font)
{
  أرجع font->face;
}


/**
 * hb_font_set_funcs:
 * @font: #hb_font_t to work upon
 * @klass: The font-functions structure.
 * @font_data: Data to attach to @font
 * @destroy: (nullable) (scope async) (closure font_data): The function to call when @font_data is not needed anymore
 *
 * Replaces the font-functions structure attached to a font, updating
 * the font's user-data with @font-data and the @destroy callback.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_set_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   فراغ              *font_data,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (font))
  {
    إذا (destroy)
      destroy (font_data);
    أرجع;
  }

  إذا (font->destroy)
    font->destroy (font->user_data);

  إذا (!klass)
    klass = hb_font_funcs_get_empty ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_data = font_data;
  font->destroy = destroy;

  font->changed ();
}

/**
 * hb_font_set_funcs_data:
 * @font: #hb_font_t to work upon
 * @font_data: Data to attach to @font
 * @destroy: (nullable) (scope async) (closure font_data): The function to call when @font_data is not needed anymore
 *
 * Replaces the user data attached to a font, updating the font's
 * @destroy callback.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_set_funcs_data (hb_font_t         *font,
		        فراغ              *font_data,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  إذا (hb_object_is_immutable (font))
  {
    إذا (destroy)
      destroy (font_data);
    أرجع;
  }

  إذا (font->destroy)
    font->destroy (font->user_data);

  font->user_data = font_data;
  font->destroy = destroy;

  font->changed ();
}

ساكن ثابت بنية supported_font_funcs_t {
	حرف name[16];
	فراغ (*func) (hb_font_t *);
} supported_font_funcs[] =
{
#ifndef HB_NO_OT_FONT
  {"ot",	hb_ot_font_set_funcs},
#endif
#ifdef HAVE_FREETYPE
  {"ft",	hb_ft_font_set_funcs},
#endif
#ifdef HAVE_FONTATIONS
  {"fontations",hb_fontations_font_set_funcs},
#endif
#ifdef HAVE_CORETEXT
  {"coretext",	hb_coretext_font_set_funcs},
#endif
#ifdef HAVE_DIRECTWRITE
  {"directwrite",hb_directwrite_font_set_funcs},
#endif
};

ساكن ثابت حرف *get_default_funcs_name ()
{
  ساكن hb_atomic_t<ثابت حرف *> static_funcs_name;
  ثابت حرف *name = static_funcs_name.get_acquire ();
  إذا (!name)
  {
    name = getenv ("HB_FONT_FUNCS");
    إذا (!name)
      name = "";
    إذا (!static_funcs_name.cmpexch (nullptr, name))
      name = static_funcs_name.get_acquire ();
  }
  أرجع name;
}

/**
 * hb_font_set_funcs_using:
 * @font: #hb_font_t to work upon
 * @name: The name of the font-functions structure to use, or `NULL`
 *
 * Sets the font-functions structure to use for a font, based on the
 * specified name.
 *
 * If @name is `NULL` or the empty string, the default (first) functioning font-functions
 * are used.  This default can be changed by setting the `HB_FONT_FUNCS` environment
 * variable to the name of the desired font-functions.
 *
 * Return value: `true` if the font-functions was found and set, `false` otherwise
 *
 * Since: 11.0.0
 **/
hb_bool_t
hb_font_set_funcs_using (hb_font_t  *font,
			 ثابت حرف *name)
{
  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع خطأ;

  منطقي retry = خطأ;

  إذا (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = true;
  }
  إذا (name && !*name) name = nullptr;

retry:
  لكل (unsigned i = 0; i < ARRAY_LENGTH (supported_font_funcs); i++)
    إذا (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      إذا (name || font->klass != hb_font_funcs_get_empty ())
	أرجع true;
    }

  إذا (retry)
  {
    retry = خطأ;
    name = nullptr;
    اذهب retry;
  }

  أرجع خطأ;
}

ساكن مضمن فراغ free_static_font_funcs_list ();

ساكن ثابت حرف * ثابت nil_font_funcs_list[] = {nullptr};

ساكن بنية hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
								  hb_font_funcs_list_lazy_loader_t>
{
  ساكن ثابت حرف ** create ()
  {
    ثابت حرف **font_funcs_list = (ثابت حرف **) hb_calloc (1 + ARRAY_LENGTH (supported_font_funcs), حجم (ثابت حرف *));
    إذا (unlikely (!font_funcs_list))
      أرجع nullptr;

    unsigned i;
    لكل (i = 0; i < ARRAY_LENGTH (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = nullptr;

    hb_atexit (free_static_font_funcs_list);

    أرجع font_funcs_list;
  }
  ساكن فراغ destroy (ثابت حرف **l)
  { hb_free (l); }
  ساكن ثابت حرف * ثابت * get_null ()
  { أرجع nil_font_funcs_list; }
} static_font_funcs_list;

ساكن مضمن
فراغ free_static_font_funcs_list ()
{
  static_font_funcs_list.free_instance ();
}

/**
 * hb_font_list_funcs:
 *
 * Retrieves the list of font functions supported by HarfBuzz.
 *
 * Return value: (transfer none) (array zero-terminated=1): a
 *    `NULL`-terminated array of supported font functions
 *    constant strings. The returned array is owned by HarfBuzz
 *    and should not be modified or freed.
 *
 * Since: 11.0.0
 **/
ثابت حرف **
hb_font_list_funcs ()
{
  أرجع static_font_funcs_list.get_unconst ();
}

/**
 * hb_font_set_scale:
 * @font: #hb_font_t to work upon
 * @x_scale: Horizontal scale value to assign
 * @y_scale: Vertical scale value to assign
 *
 * Sets the horizontal and vertical scale of a font.
 *
 * The font scale is a number related to, but not the same as,
 * font size. Typically the client establishes a scale factor
 * to be used between the two. For example, 64, or 256, which
 * would be the fractional-precision part of the font scale.
 * This is necessary because #hb_position_t values are integer
 * types and you need to leave room for fractional values
 * in there.
 *
 * For example, to set the font size to 20, with 64
 * levels of fractional precision you would call
 * `hb_font_set_scale(font, 20 * 64, 20 * 64)`.
 *
 * In the example above, even what font size 20 means is up to
 * you. It might be 20 pixels, or 20 points, or 20 millimeters.
 * HarfBuzz does not care about that.  You can set the point
 * size of the font using hb_font_set_ptem(), and the pixel
 * size using hb_font_set_ppem().
 *
 * The choice of scale is yours but needs to be consistent between
 * what you set here, and what you expect out of #hb_position_t
 * as well has draw / paint API output values.
 *
 * Fonts default to a scale equal to the UPEM value of their face.
 * A font with this setting is sometimes called an "unscaled" font.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_set_scale (hb_font_t *font,
		   صحيح        x_scale,
		   صحيح        y_scale)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_scale == x_scale && font->y_scale == y_scale)
    أرجع;

  font->x_scale = x_scale;
  font->y_scale = y_scale;

  font->changed ();
}

/**
 * hb_font_get_scale:
 * @font: #hb_font_t to work upon
 * @x_scale: (out): Horizontal scale value
 * @y_scale: (out): Vertical scale value
 *
 * Fetches the horizontal and vertical scale of a font.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_get_scale (hb_font_t *font,
		   صحيح       *x_scale,
		   صحيح       *y_scale)
{
  إذا (x_scale) *x_scale = font->x_scale;
  إذا (y_scale) *y_scale = font->y_scale;
}

/**
 * hb_font_set_ppem:
 * @font: #hb_font_t to work upon
 * @x_ppem: Horizontal ppem value to assign
 * @y_ppem: Vertical ppem value to assign
 *
 * Sets the horizontal and vertical pixels-per-em (PPEM) of a font.
 *
 * These values are used for pixel-size-specific adjustment to
 * shaping and draw results, though for the most part they are
 * unused and can be left unset.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_set_ppem (hb_font_t    *font,
		  unsigned صحيح  x_ppem,
		  unsigned صحيح  y_ppem)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    أرجع;

  font->x_ppem = x_ppem;
  font->y_ppem = y_ppem;

  font->changed ();
}

/**
 * hb_font_get_ppem:
 * @font: #hb_font_t to work upon
 * @x_ppem: (out): Horizontal ppem value
 * @y_ppem: (out): Vertical ppem value
 *
 * Fetches the horizontal and vertical points-per-em (ppem) of a font.
 *
 * Since: 0.9.2
 **/
فراغ
hb_font_get_ppem (hb_font_t    *font,
		  unsigned صحيح *x_ppem,
		  unsigned صحيح *y_ppem)
{
  إذا (x_ppem) *x_ppem = font->x_ppem;
  إذا (y_ppem) *y_ppem = font->y_ppem;
}

/**
 * hb_font_set_ptem:
 * @font: #hb_font_t to work upon
 * @ptem: font size in points.
 *
 * Sets the "point size" of a font. Set to zero to unset.
 * Used in CoreText to implement optical sizing.
 *
 * <note>Note: There are 72 points in an inch.</note>
 *
 * Since: 1.6.0
 **/
فراغ
hb_font_set_ptem (hb_font_t *font,
		  عائم      ptem)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->ptem == ptem)
    أرجع;

  font->ptem = ptem;

  font->changed ();
}

/**
 * hb_font_get_ptem:
 * @font: #hb_font_t to work upon
 *
 * Fetches the "point size" of a font. Used in CoreText to
 * implement optical sizing.
 *
 * Return value: Point size.  A value of zero means "not set."
 *
 * Since: 1.6.0
 **/
عائم
hb_font_get_ptem (hb_font_t *font)
{
  أرجع font->ptem;
}

/**
 * hb_font_is_synthetic:
 * @font: #hb_font_t to work upon
 *
 * Tests whether a font is synthetic. A synthetic font is one
 * that has either synthetic slant or synthetic bold set on it.
 *
 * Return value: `true` if the font is synthetic, `false` otherwise.
 *
 * Since: 11.2.0
 */
hb_bool_t
hb_font_is_synthetic (hb_font_t *font)
{
  أرجع font->is_synthetic;
}

/**
 * hb_font_set_synthetic_bold:
 * @font: #hb_font_t to work upon
 * @x_embolden: the amount to embolden horizontally
 * @y_embolden: the amount to embolden vertically
 * @in_place: whether to embolden glyphs in-place
 *
 * Sets the "synthetic boldness" of a font.
 *
 * Positive values for @x_embolden / @y_embolden make a font
 * bolder, negative values thinner. Typical values are in the
 * 0.01 to 0.05 range. The default value is zero.
 *
 * Synthetic boldness is applied by offsetting the contour
 * points of the glyph shape.
 *
 * Synthetic boldness is applied when rendering a glyph via
 * hb_font_draw_glyph_or_fail().
 *
 * If @in_place is `false`, then glyph advance-widths are also
 * adjusted, otherwise they are not.  The in-place mode is
 * useful for simulating [font grading](https://fonts.google.com/knowledge/glossary/grade).
 *
 *
 * Since: 7.0.0
 **/
فراغ
hb_font_set_synthetic_bold (hb_font_t *font,
			    عائم x_embolden,
			    عائم y_embolden,
			    hb_bool_t in_place)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_in_place == (منطقي) in_place)
    أرجع;

  font->x_embolden = x_embolden;
  font->y_embolden = y_embolden;
  font->embolden_in_place = in_place;

  font->changed ();
}

/**
 * hb_font_get_synthetic_bold:
 * @font: #hb_font_t to work upon
 * @x_embolden: (out): return location for horizontal value
 * @y_embolden: (out): return location for vertical value
 * @in_place: (out): return location for in-place value
 *
 * Fetches the "synthetic boldness" parameters of a font.
 *
 * Since: 7.0.0
 **/
فراغ
hb_font_get_synthetic_bold (hb_font_t *font,
			    عائم *x_embolden,
			    عائم *y_embolden,
			    hb_bool_t *in_place)
{
  إذا (x_embolden) *x_embolden = font->x_embolden;
  إذا (y_embolden) *y_embolden = font->y_embolden;
  إذا (in_place) *in_place = font->embolden_in_place;
}

/**
 * hb_font_set_synthetic_slant:
 * @font: #hb_font_t to work upon
 * @slant: synthetic slant value.
 *
 * Sets the "synthetic slant" of a font.  By default is zero.
 * Synthetic slant is the graphical skew applied to the font
 * at rendering time.
 *
 * HarfBuzz needs to know this value to adjust shaping results,
 * metrics, and style values to match the slanted rendering.
 *
 * <note>Note: The glyph shape fetched via the hb_font_draw_glyph_or_fail()
 * function is slanted to reflect this value as well.</note>
 *
 * <note>Note: The slant value is a ratio.  For example, a
 * 20% slant would be represented as a 0.2 value.</note>
 *
 * Since: 3.3.0
 **/
HB_EXTERN فراغ
hb_font_set_synthetic_slant (hb_font_t *font, عائم slant)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->slant == slant)
    أرجع;

  font->slant = slant;

  font->changed ();
}

/**
 * hb_font_get_synthetic_slant:
 * @font: #hb_font_t to work upon
 *
 * Fetches the "synthetic slant" of a font.
 *
 * Return value: Synthetic slant.  By default is zero.
 *
 * Since: 3.3.0
 **/
HB_EXTERN عائم
hb_font_get_synthetic_slant (hb_font_t *font)
{
  أرجع font->slant;
}

#ifndef HB_NO_VAR
/*
 * Variations
 */

/**
 * hb_font_set_variations:
 * @font: #hb_font_t to work upon
 * @variations: (array length=variations_length): Array of variation settings to apply
 * @variations_length: Number of variations to apply
 *
 * Applies a list of font-variation settings to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @variations will be effectively set to their
 * default values.
 *
 * Since: 1.4.2
 */
فراغ
hb_font_set_variations (hb_font_t            *font,
			ثابت hb_variation_t *variations,
			unsigned صحيح          variations_length)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ثابت unsigned coords_length = axes.length;

  صحيح *normalized = coords_length ? (صحيح *) hb_calloc (coords_length, حجم (صحيح)) : nullptr;
  عائم *design_coords = coords_length ? (عائم *) hb_calloc (coords_length, حجم (عائم)) : nullptr;

  إذا (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  /* Initialize design coords. */
  لكل (unsigned صحيح i = 0; i < coords_length; i++)
    design_coords[i] = axes[i].get_default ();
  إذا (font->instance_index != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    unsigned کيراءن = coords_length;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_index,
						&کيراءن, design_coords);
  }

  لكل (unsigned صحيح i = 0; i < variations_length; i++)
  {
    ثابت auto tag = variations[i].tag;
    ثابت auto v = variations[i].value;
    لكل (unsigned axis_index = 0; axis_index < coords_length; axis_index++)
      إذا (axes[axis_index].axisTag == tag)
	design_coords[axis_index] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_length, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_length);
}

/**
 * hb_font_set_variation:
 * @font: #hb_font_t to work upon
 * @tag: The #hb_tag_t tag of the variation-axis name
 * @value: The value of the variation axis
 *
 * Change the value of one variation axis on the font.
 *
 * Note: This function is expensive to be called repeatedly.
 *   If you want to set multiple variation axes at the same time,
 *   use hb_font_set_variations() instead.
 *
 * Since: 7.1.0
 */
فراغ
hb_font_set_variation (hb_font_t *font,
		       hb_tag_t tag,
		       عائم    value)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  // TODO Share some of this code with set_variations()

  ثابت OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ثابت unsigned coords_length = axes.length;

  صحيح *normalized = coords_length ? (صحيح *) hb_calloc (coords_length, حجم (صحيح)) : nullptr;
  عائم *design_coords = coords_length ? (عائم *) hb_calloc (coords_length, حجم (عائم)) : nullptr;

  إذا (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  إذا (font->design_coords && font->اڠک_coords == coords_length)
  {
    لكل (unsigned صحيح i = 0; i < coords_length; i++)
      design_coords[i] = font->design_coords[i];
  }
  وإلا
  {
    لكل (unsigned صحيح i = 0; i < coords_length; i++)
      design_coords[i] = axes[i].get_default ();
    إذا (font->instance_index != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      unsigned کيراءن = coords_length;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_index,
						  &کيراءن, design_coords);
    }
  }

  لكل (unsigned axis_index = 0; axis_index < coords_length; axis_index++)
    إذا (axes[axis_index].axisTag == tag)
      design_coords[axis_index] = value;

  hb_ot_var_normalize_coords (font->face, coords_length, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_length);
}

/**
 * hb_font_set_var_coords_design:
 * @font: #hb_font_t to work upon
 * @coords: (array length=coords_length): Array of variation coordinates to apply
 * @coords_length: Number of coordinates to apply
 *
 * Applies a list of variation coordinates (in design-space units)
 * to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @coords will be effectively set to their
 * default values.
 *
 * Since: 1.4.2
 */
فراغ
hb_font_set_var_coords_design (hb_font_t    *font,
			       ثابت عائم  *coords,
			       unsigned صحيح  ماسوقن_coords_length)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ثابت unsigned coords_length = axes.length;

  ماسوقن_coords_length = hb_min (ماسوقن_coords_length, coords_length);
  صحيح *normalized = coords_length ? (صحيح *) hb_calloc (coords_length, حجم (صحيح)) : nullptr;
  عائم *design_coords = coords_length ? (عائم *) hb_calloc (coords_length, حجم (عائم)) : nullptr;

  إذا (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  إذا (ماسوقن_coords_length)
    hb_memcpy (design_coords, coords, ماسوقن_coords_length * حجم (font->design_coords[0]));
  // Fill in the rest with default values
  لكل (unsigned صحيح i = ماسوقن_coords_length; i < coords_length; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_length, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_length);
}

/**
 * hb_font_set_var_named_instance:
 * @font: a font.
 * @instance_index: named instance index.
 *
 * Sets design coords of a font from a named-instance index.
 *
 * Since: 2.6.0
 */
فراغ
hb_font_set_var_named_instance (hb_font_t *font,
				unsigned صحيح instance_index)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->instance_index == instance_index)
    أرجع;

  font->instance_index = instance_index;
  hb_font_set_variations (font, nullptr, 0);
}

/**
 * hb_font_get_var_named_instance:
 * @font: a font.
 *
 * Returns the currently-set named-instance index of the font.
 *
 * Return value: Named-instance index or %HB_FONT_NO_VAR_NAMED_INSTANCE.
 *
 * Since: 7.0.0
 **/
unsigned صحيح
hb_font_get_var_named_instance (hb_font_t *font)
{
  أرجع font->instance_index;
}

/**
 * hb_font_set_var_coords_normalized:
 * @font: #hb_font_t to work upon
 * @coords: (array length=coords_length): Array of variation coordinates to apply
 * @coords_length: Number of coordinates to apply
 *
 * Applies a list of variation coordinates (in normalized units)
 * to a font.
 *
 * Note that this overrides all existing variations set on @font.
 * Axes not included in @coords will be effectively set to their
 * default values.
 *
 * <note>Note: Coordinates should be normalized to 2.14.</note>
 *
 * Since: 1.4.2
 */
فراغ
hb_font_set_var_coords_normalized (hb_font_t    *font,
				   ثابت صحيح    *coords, /* 2.14 normalized */
				   unsigned صحيح  ماسوقن_coords_length)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  unsigned coords_length = axes.length;

  ماسوقن_coords_length = hb_min (ماسوقن_coords_length, coords_length);
  صحيح *copy = coords_length ? (صحيح *) hb_calloc (coords_length, حجم (coords[0])) : nullptr;
  عائم *design_coords = coords_length ? (عائم *) hb_calloc (coords_length, حجم (design_coords[0])) : nullptr;

  إذا (unlikely (coords_length && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    أرجع;
  }

  إذا (ماسوقن_coords_length)
    hb_memcpy (copy, coords, ماسوقن_coords_length * حجم (coords[0]));

  لكل (unsigned صحيح i = 0; i < coords_length; ++i)
    design_coords[i] = NAN;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_length);
}

/**
 * hb_font_get_var_coords_normalized:
 * @font: #hb_font_t to work upon
 * @length: (out): Number of coordinates retrieved
 *
 * Fetches the list of normalized variation coordinates currently
 * set on a font.
 *
 * <note>Note that if no variation coordinates are set, this function may
 * return %NULL.</note>
 *
 * Return value is valid as long as variation coordinates of the font
 * are not modified.
 *
 * Return value: coordinates array
 *
 * Since: 1.4.2
 */
ثابت صحيح *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   unsigned صحيح *length)
{
  إذا (length)
    *length = font->اڠک_coords;

  أرجع font->coords;
}

/**
 * hb_font_get_var_coords_design:
 * @font: #hb_font_t to work upon
 * @length: (out): Number of coordinates retrieved
 *
 * Fetches the list of variation coordinates (in design-space units) currently
 * set on a font.
 *
 * <note>Note that if no variation coordinates are set, this function may
 * return %NULL.</note>
 *
 * <note>If variations have been set on the font using normalized coordinates
 * (i.e. via hb_font_set_var_coords_normalized()), the design coordinates will
 * have NaN (Not a Number) values.</note>
 *
 * Return value is valid as long as variation coordinates of the font
 * are not modified.
 *
 * Return value: coordinates array
 *
 * Since: 3.3.0
 */
ثابت عائم *
hb_font_get_var_coords_design (hb_font_t *font,
			       unsigned صحيح *length)
{
  إذا (length)
    *length = font->اڠک_coords;

  أرجع font->design_coords;
}
#endif

#ifndef HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

بنية hb_trampoline_closure_t
{
  فراغ *user_data;
  hb_destroy_func_t destroy;
  unsigned صحيح ref_کيراءن_2;
};

template <typename FuncType>
بنية hb_trampoline_t
{
  hb_trampoline_closure_t closure; /* Must be first. */
  FuncType func;
};

template <typename FuncType>
ساكن hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   فراغ              *user_data,
		   hb_destroy_func_t  destroy)
{
  تعريفنوع hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, حجم (trampoline_t));

  إذا (unlikely (!trampoline))
    أرجع nullptr;

  trampoline->closure.user_data = user_data;
  trampoline->closure.destroy = destroy;
  trampoline->closure.ref_کيراءن_2 = 1;
  trampoline->func = func;

  أرجع trampoline;
}

ساكن فراغ
trampoline_reference (hb_trampoline_closure_t *closure)
{
  closure->ref_کيراءن_2++;
}

ساكن فراغ
trampoline_destroy (فراغ *user_data)
{
  hb_trampoline_closure_t *closure = (hb_trampoline_closure_t *) user_data;

  إذا (--closure->ref_کيراءن_2)
    أرجع;

  إذا (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}

تعريفنوع hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

ساكن hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      فراغ           *font_data,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      فراغ           *user_data)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_data;
  أرجع trampoline->func (font, font_data, unicode, 0, glyph, trampoline->closure.user_data);
}

ساكن hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					فراغ           *font_data,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					فراغ           *user_data)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_data;
  أرجع trampoline->func (font, font_data, unicode, variation_selector, glyph, trampoline->closure.user_data);
}

/**
 * hb_font_funcs_set_glyph_func:
 * @ffuncs: The font-functions structure
 * @func: (closure user_data) (destroy destroy) (scope notified): callback function
 * @user_data: data to pass to @func
 * @destroy: (nullable): function to call when @user_data is not needed anymore
 *
 * Deprecated.  Use hb_font_funcs_set_nominal_glyph_func() and
 * hb_font_funcs_set_variation_glyph_func() instead.
 *
 * Since: 0.9.2
 * Deprecated: 1.2.3
 **/
فراغ
hb_font_funcs_set_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      فراغ                     *user_data,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_data, destroy);
  إذا (unlikely (!trampoline))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }

  /* Since we pass it to two destroying functions. */
  trampoline_reference (&trampoline->closure);

  hb_font_funcs_set_nominal_glyph_func (ffuncs,
					hb_font_get_nominal_glyph_trampoline,
					trampoline,
					trampoline_destroy);

  hb_font_funcs_set_variation_glyph_func (ffuncs,
					  hb_font_get_variation_glyph_trampoline,
					  trampoline,
					  trampoline_destroy);
}
#endif


#ifndef HB_DISABLE_DEPRECATED

بنية hb_draw_glyph_closure_t
{
  hb_font_draw_glyph_func_t func;
  فراغ *user_data;
  hb_destroy_func_t destroy;
};
ساكن hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       فراغ            *font_data,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       فراغ            *draw_data,
			       فراغ            *user_data)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_data;
  closure->func (font, font_data, glyph, draw_funcs, draw_data, closure->user_data);
  أرجع true;
}
ساكن فراغ
hb_font_draw_glyph_closure_destroy (فراغ *user_data)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_data;

  إذا (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}
ساكن فراغ
_hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    فراغ                      *user_data,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) hb_calloc (1, حجم (hb_draw_glyph_closure_t));
  إذا (unlikely (!closure))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }
  closure->func = func;
  closure->user_data = user_data;
  closure->destroy = destroy;

  hb_font_funcs_set_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     closure,
					     hb_font_draw_glyph_closure_destroy);
}
فراغ
hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   فراغ                      *user_data,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_data, destroy);
}
فراغ
hb_font_funcs_set_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   فراغ                           *user_data,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_data, destroy);
}

بنية hb_paint_glyph_closure_t
{
  hb_font_paint_glyph_func_t func;
  فراغ *user_data;
  hb_destroy_func_t destroy;
};
ساكن hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				فراغ *font_data,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				فراغ *paint_data,
				unsigned صحيح palette,
				hb_color_t foreground,
				فراغ *user_data)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_data;
  closure->func (font, font_data, glyph, paint_funcs, paint_data, palette, foreground, closure->user_data);
  أرجع true;
}
ساكن فراغ
hb_font_paint_glyph_closure_destroy (فراغ *user_data)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_data;

  إذا (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}
فراغ
hb_font_funcs_set_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    فراغ                      *user_data,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) hb_calloc (1, حجم (hb_paint_glyph_closure_t));
  إذا (unlikely (!closure))
  {
    إذا (destroy)
      destroy (user_data);
    أرجع;
  }
  closure->func = func;
  closure->user_data = user_data;
  closure->destroy = destroy;

  hb_font_funcs_set_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      closure,
					      hb_font_paint_glyph_closure_destroy);
}
#endif
