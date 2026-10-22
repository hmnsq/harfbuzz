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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-paint.hh"
#অন্তর্ভাব "hb-machinery.hh"

#অন্তর্ভাব "hb-ot.h"

#অন্তর্ভাব "hb-ot-var-avar-table.hh"
#অন্তর্ভাব "hb-ot-var-fvar-table.hh"

#ifndef HB_NO_OT_FONT
#অন্তর্ভাব "hb-ot.h"
#endif
#ifdef HAVE_FREETYPE
#অন্তর্ভাব "hb-ft.h"
#endif
#ifdef HAVE_FONTATIONS
#অন্তর্ভাব "hb-fontations.h"
#endif
#ifdef HAVE_CORETEXT
#অন্তর্ভাব "hb-coretext.h"
#endif
#ifdef HAVE_DIRECTWRITE
#অন্তর্ভাব "hb-directwrite.h"
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

স্থির hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_UNUSED,
				শূন্য              *font_তথ্য HB_UNUSED,
				hb_font_extents_t *extents,
				শূন্য              *user_তথ্য HB_UNUSED)
{
  hb_memset (extents, 0, আকার (*extents));
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    শূন্য              *font_তথ্য HB_UNUSED,
				    hb_font_extents_t *extents,
				    শূন্য              *user_তথ্য HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_h_extents (extents, মিথ্যা);
  যদি (ret) {
    extents->ascender = font->parent_scale_y_distance (extents->ascender);
    extents->descender = font->parent_scale_y_distance (extents->descender);
    extents->সারি_gap = font->parent_scale_y_distance (extents->সারি_gap);
  }
  ফেরত ret;
}

স্থির hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_UNUSED,
				শূন্য              *font_তথ্য HB_UNUSED,
				hb_font_extents_t *extents,
				শূন্য              *user_তথ্য HB_UNUSED)
{
  hb_memset (extents, 0, আকার (*extents));
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    শূন্য              *font_তথ্য HB_UNUSED,
				    hb_font_extents_t *extents,
				    শূন্য              *user_তথ্য HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_v_extents (extents, মিথ্যা);
  যদি (ret) {
    extents->ascender = font->parent_scale_x_distance (extents->ascender);
    extents->descender = font->parent_scale_x_distance (extents->descender);
    extents->সারি_gap = font->parent_scale_x_distance (extents->সারি_gap);
  }
  ফেরত ret;
}

স্থির hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_UNUSED,
			       শূন্য           *font_তথ্য HB_UNUSED,
			       hb_codepoint_t  unicode HB_UNUSED,
			       hb_codepoint_t *glyph,
			       শূন্য           *user_তথ্য HB_UNUSED)
{
  *glyph = 0;
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   শূন্য           *font_তথ্য HB_UNUSED,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (font->has_nominal_glyphs_func_set ())
  {
    ফেরত font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  ফেরত font->parent->get_nominal_glyph (unicode, glyph);
}

#define hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

স্থির unsigned পূর্ণসংখ্যা
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    শূন্য                 *font_তথ্য HB_UNUSED,
				    unsigned পূর্ণসংখ্যা          গুনতি,
				    ধ্রুবক hb_codepoint_t *first_unicode,
				    unsigned পূর্ণসংখ্যা          unicode_stride,
				    hb_codepoint_t       *first_glyph,
				    unsigned পূর্ণসংখ্যা          glyph_stride,
				    শূন্য                 *user_তথ্য HB_UNUSED)
{
  যদি (font->has_nominal_glyph_func_set ())
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যদি (!font->get_nominal_glyph (*first_unicode, first_glyph))
	ফেরত i;

      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ফেরত গুনতি;
  }

  ফেরত font->parent->get_nominal_glyphs (গুনতি,
					   first_unicode, unicode_stride,
					   first_glyph, glyph_stride);
}

স্থির hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_UNUSED,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 hb_codepoint_t  unicode HB_UNUSED,
				 hb_codepoint_t  variation_selector HB_UNUSED,
				 hb_codepoint_t *glyph,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  *glyph = 0;
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->parent->get_variation_glyph (unicode, variation_selector, glyph);
}


স্থির hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->x_scale;
}

স্থির hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_h_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, মিথ্যা);
    ফেরত ret;
  }
  ফেরত font->parent_scale_x_distance (font->parent->get_glyph_h_advance (glyph, মিথ্যা));
}

স্থির hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত -font->y_scale;
}

স্থির hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_v_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, মিথ্যা);
    ফেরত ret;
  }
  ফেরত font->parent_scale_y_distance (font->parent->get_glyph_v_advance (glyph, মিথ্যা));
}

#define hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

স্থির শূন্য
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      শূন্য*                 font_তথ্য HB_UNUSED,
				      unsigned পূর্ণসংখ্যা          গুনতি,
				      ধ্রুবক hb_codepoint_t *first_glyph,
				      unsigned পূর্ণসংখ্যা          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned পূর্ণসংখ্যা          advance_stride,
				      শূন্য                 *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_h_advance_func_set ())
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      *first_advance = font->get_glyph_h_advance (*first_glyph, মিথ্যা);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ফেরত;
  }

  font->parent->get_glyph_h_advances (গুনতি,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      মিথ্যা);
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    *first_advance = font->parent_scale_x_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#define hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
স্থির শূন্য
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      শূন্য*                 font_তথ্য HB_UNUSED,
				      unsigned পূর্ণসংখ্যা          গুনতি,
				      ধ্রুবক hb_codepoint_t *first_glyph,
				      unsigned পূর্ণসংখ্যা          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned পূর্ণসংখ্যা          advance_stride,
				      শূন্য                 *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_v_advance_func_set ())
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      *first_advance = font->get_glyph_v_advance (*first_glyph, মিথ্যা);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    ফেরত;
  }

  font->parent->get_glyph_v_advances (গুনতি,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      মিথ্যা);
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    *first_advance = font->parent_scale_y_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

স্থির hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_UNUSED,
				শূন্য           *font_তথ্য HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				শূন্য           *user_তথ্য HB_UNUSED)
{
  *x = *y = 0;
  ফেরত সত্য;
}

স্থির hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    শূন্য           *font_তথ্য HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_h_origins_func_set ())
  {
    ফেরত font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, মিথ্যা);
  }
  hb_bool_t ret = font->parent->get_glyph_h_origin (glyph, x, y);
  যদি (ret)
    font->parent_scale_position (x, y);
  ফেরত ret;
}

স্থির hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_UNUSED,
				শূন্য           *font_তথ্য HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    শূন্য           *font_তথ্য HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_v_origins_func_set ())
  {
    ফেরত font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, মিথ্যা);
  }
  hb_bool_t ret = font->parent->get_glyph_v_origin (glyph, x, y);
  যদি (ret)
    font->parent_scale_position (x, y);
  ফেরত ret;
}

#define hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

স্থির hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_UNUSED,
				     শূন্য *font_তথ্য HB_UNUSED,
				     unsigned পূর্ণসংখ্যা গুনতি,
				     ধ্রুবক hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     শূন্য *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_h_origin_func_set ())
  {
    hb_bool_t ret = সত্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ret &= font->get_glyph_h_origin (*first_glyph, first_x, first_y, মিথ্যা);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    ফেরত ret;
  }

  hb_bool_t ret = font->parent->get_glyph_h_origins (গুনতি,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  যদি (ret)
  {
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  ফেরত ret;
}

#define hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

স্থির hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_UNUSED,
				     শূন্য *font_তথ্য HB_UNUSED,
				     unsigned পূর্ণসংখ্যা গুনতি,
				     ধ্রুবক hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     শূন্য *user_তথ্য HB_UNUSED)
{
  যদি (font->has_glyph_v_origin_func_set ())
  {
    hb_bool_t ret = সত্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      ret &= font->get_glyph_v_origin (*first_glyph, first_x, first_y, মিথ্যা);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    ফেরত ret;
  }

  hb_bool_t ret = font->parent->get_glyph_v_origins (গুনতি,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  যদি (ret)
  {
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  ফেরত ret;
}

স্থির hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_UNUSED,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 hb_codepoint_t  বাম_glyph HB_UNUSED,
				 hb_codepoint_t  ডান_glyph HB_UNUSED,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত 0;
}

স্থির hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  বাম_glyph,
				     hb_codepoint_t  ডান_glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->parent_scale_x_distance (font->parent->get_glyph_h_kerning (বাম_glyph, ডান_glyph));
}

#ifndef HB_DISABLE_DEPRECATED
স্থির hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_UNUSED,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 hb_codepoint_t  শীর্ষ_glyph HB_UNUSED,
				 hb_codepoint_t  bottom_glyph HB_UNUSED,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত 0;
}

স্থির hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  শীর্ষ_glyph,
				     hb_codepoint_t  bottom_glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->parent_scale_y_distance (font->parent->get_glyph_v_kerning (শীর্ষ_glyph, bottom_glyph));
}
#endif

স্থির hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_UNUSED,
			       শূন্য               *font_তথ্য HB_UNUSED,
			       hb_codepoint_t      glyph HB_UNUSED,
			       hb_glyph_extents_t *extents,
			       শূন্য               *user_তথ্য HB_UNUSED)
{
  hb_memset (extents, 0, আকার (*extents));
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   শূন্য               *font_তথ্য HB_UNUSED,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   শূন্য               *user_তথ্য HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_extents (glyph, extents, মিথ্যা);
  যদি (ret) {
    font->parent_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->parent_scale_distance (&extents->width, &extents->height);
  }
  ফেরত ret;
}

স্থির hb_bool_t
hb_font_get_glyph_contour_point_nil (hb_font_t      *font HB_UNUSED,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     hb_codepoint_t  glyph HB_UNUSED,
				     unsigned পূর্ণসংখ্যা    point_সূচক HB_UNUSED,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  *x = *y = 0;
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_glyph_contour_point_default (hb_font_t      *font,
					 শূন্য           *font_তথ্য HB_UNUSED,
					 hb_codepoint_t  glyph,
					 unsigned পূর্ণসংখ্যা    point_সূচক,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 শূন্য           *user_তথ্য HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_contour_point (glyph, point_সূচক, x, y, মিথ্যা);
  যদি (ret)
    font->parent_scale_position (x, y);
  ফেরত ret;
}

স্থির hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_UNUSED,
			    শূন্য           *font_তথ্য HB_UNUSED,
			    hb_codepoint_t  glyph HB_UNUSED,
			    অক্ষর           *name,
			    unsigned পূর্ণসংখ্যা    আকার_2,
			    শূন্য           *user_তথ্য HB_UNUSED)
{
  যদি (আকার_2) *name = '\0';
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				শূন্য           *font_তথ্য HB_UNUSED,
				hb_codepoint_t  glyph,
				অক্ষর           *name,
				unsigned পূর্ণসংখ্যা    আকার_2,
				শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->parent->get_glyph_name (glyph, name, আকার_2);
}

স্থির hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_UNUSED,
				 শূন্য           *font_তথ্য HB_UNUSED,
				 ধ্রুবক অক্ষর     *name HB_UNUSED,
				 পূর্ণসংখ্যা             len HB_UNUSED, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 শূন্য           *user_তথ্য HB_UNUSED)
{
  *glyph = 0;
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     শূন্য           *font_তথ্য HB_UNUSED,
				     ধ্রুবক অক্ষর     *name,
				     পূর্ণসংখ্যা             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     শূন্য           *user_তথ্য HB_UNUSED)
{
  ফেরত font->parent->get_glyph_from_name (name, len, glyph);
}

স্থির hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_UNUSED,
				শূন্য            *font_তথ্য HB_UNUSED,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				শূন্য            *draw_তথ্য,
				শূন্য            *user_তথ্য HB_UNUSED)
{
  ফেরত মিথ্যা;
}

স্থির hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_UNUSED,
				 শূন্য *font_তথ্য HB_UNUSED,
				 hb_codepoint_t glyph HB_UNUSED,
				 hb_paint_funcs_t *paint_funcs HB_UNUSED,
				 শূন্য *paint_তথ্য HB_UNUSED,
				 unsigned পূর্ণসংখ্যা palette HB_UNUSED,
				 hb_color_t foreground HB_UNUSED,
				 শূন্য *user_তথ্য HB_UNUSED)
{
  ফেরত মিথ্যা;
}

প্রকারলেখ কাঠামো hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  শূন্য		  *draw_তথ্য;
  দশমিক		   x_scale;
  দশমিক		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

স্থির শূন্য
hb_draw_move_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 শূন্য *draw_তথ্য,
			 hb_draw_state_t *st,
			 দশমিক to_x, দশমিক to_y,
			 শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  দশমিক x_scale = adaptor->x_scale;
  দশমিক y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_to (adaptor->draw_তথ্য, *st,
				     x_scale * to_x, y_scale * to_y);
}

স্থির শূন্য
hb_draw_সারি_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			 hb_draw_state_t *st,
			 দশমিক to_x, দশমিক to_y,
			 শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  দশমিক x_scale = adaptor->x_scale;
  দশমিক y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_সারি_to (adaptor->draw_তথ্য, *st,
				     x_scale * to_x, y_scale * to_y);
}

স্থির শূন্য
hb_draw_quadratic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			      hb_draw_state_t *st,
			      দশমিক control_x, দশমিক control_y,
			      দশমিক to_x, দশমিক to_y,
			      শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  দশমিক x_scale = adaptor->x_scale;
  দশমিক y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_to (adaptor->draw_তথ্য, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * to_x, y_scale * to_y);
}

স্থির শূন্য
hb_draw_cubic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			  hb_draw_state_t *st,
			  দশমিক control1_x, দশমিক control1_y,
			  দশমিক control2_x, দশমিক control2_y,
			  দশমিক to_x, দশমিক to_y,
			  শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  দশমিক x_scale = adaptor->x_scale;
  দশমিক y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_to (adaptor->draw_তথ্য, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * to_x, y_scale * to_y);
}

স্থির শূন্য
hb_draw_close_path_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			    hb_draw_state_t *st,
			    শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;

  adaptor->draw_funcs->emit_close_path (adaptor->draw_তথ্য, *st);
}

স্থির hb_bool_t
hb_draw_set_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			    int64_t budget,
			    শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  ফেরত adaptor->draw_funcs->set_budget (adaptor->draw_তথ্য, budget);
}

স্থির int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
			    শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  ফেরত adaptor->draw_funcs->get_budget (adaptor->draw_তথ্য);
}

স্থির int64_t *
hb_draw_get_budget_remaining_default (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *draw_তথ্য,
				      শূন্য *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_তথ্য;
  ফেরত adaptor->draw_funcs->get_budget_remaining_ptr (adaptor->draw_তথ্য);
}

স্থির ধ্রুবক hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  }
};

স্থির hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    শূন্য            *font_তথ্য HB_UNUSED,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    শূন্য            *draw_তথ্য,
				    শূন্য            *user_তথ্য HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_তথ্য,
    font->parent->x_scale ? (দশমিক) font->x_scale / (দশমিক) font->parent->x_scale : 0.f,
    font->parent->y_scale ? (দশমিক) font->y_scale / (দশমিক) font->parent->y_scale : 0.f
  };

  ফেরত font->parent->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   মিথ্যা);
}

স্থির hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     শূন্য *font_তথ্য,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     শূন্য *paint_তথ্য,
				     unsigned পূর্ণসংখ্যা palette,
				     hb_color_t foreground,
				     শূন্য *user_তথ্য)
{
  paint_funcs->ঠেলো_transform (paint_তথ্য,
    font->parent->x_scale ? (দশমিক) font->x_scale / (দশমিক) font->parent->x_scale : 0, 0,
    0, font->parent->y_scale ? (দশমিক) font->y_scale / (দশমিক) font->parent->y_scale : 0,
    0, 0);

  যৌক্তিক ret = font->parent->paint_glyph_or_fail (glyph, paint_funcs, paint_তথ্য, palette, foreground);

  paint_funcs->বের_করো_transform (paint_তথ্য);

  ফেরত ret;
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

স্থির ধ্রুবক hb_font_funcs_t _hb_font_funcs_default = {
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

  যদি (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    ফেরত hb_font_funcs_get_empty ();

  ffuncs->get = _hb_font_funcs_default.get;

  ফেরত ffuncs;
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
  ফেরত const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
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
  ফেরত hb_object_reference (ffuncs);
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
শূন্য
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  যদি (!hb_object_destroy (ffuncs)) ফেরত;

  যদি (ffuncs->destroy)
  {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) যদি (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_তথ্য ? nullptr : ffuncs->user_তথ্য->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_তথ্য);

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
hb_font_funcs_set_user_তথ্য (hb_font_funcs_t    *ffuncs,
			     hb_user_তথ্য_চাবি_t *key,
			     শূন্য *              তথ্য,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  ফেরত hb_object_set_user_তথ্য (ffuncs, key, তথ্য, destroy, replace);
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
শূন্য *
hb_font_funcs_get_user_তথ্য (ধ্রুবক hb_font_funcs_t *ffuncs,
			     hb_user_তথ্য_চাবি_t    *key)
{
  ফেরত hb_object_get_user_তথ্য (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
শূন্য
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  যদি (hb_object_is_immutable (ffuncs))
    ফেরত;

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
  ফেরত hb_object_is_immutable (ffuncs);
}


স্থির যৌক্তিক
_hb_font_funcs_set_preamble (hb_font_funcs_t    *ffuncs,
			     যৌক্তিক                func_is_null,
			     শূন্য              **user_তথ্য,
			     hb_destroy_func_t  *destroy)
{
  যদি (hb_object_is_immutable (ffuncs))
  {
    যদি (*destroy)
      (*destroy) (*user_তথ্য);
    ফেরত মিথ্যা;
  }

  যদি (func_is_null)
  {
    যদি (*destroy)
      (*destroy) (*user_তথ্য);
    *destroy = nullptr;
    *user_তথ্য = nullptr;
  }

  ফেরত সত্য;
}

স্থির যৌক্তিক
hb_font_funcs_set_মধ্য (hb_font_funcs_t   *ffuncs,
			   শূন্য              *user_তথ্য,
			   hb_destroy_func_t  destroy)
{
  auto destroy_guard = hb_make_scope_guard ([&]() {
    যদি (destroy) destroy (user_তথ্য);
  });

  যদি (user_তথ্য && !ffuncs->user_তথ্য)
  {
    ffuncs->user_তথ্য = (decltype (ffuncs->user_তথ্য)) hb_calloc (1, আকার (*ffuncs->user_তথ্য));
    যদি (unlikely (!ffuncs->user_তথ্য))
      ফেরত মিথ্যা;
  }
  যদি (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, আকার (*ffuncs->destroy));
    যদি (unlikely (!ffuncs->destroy))
      ফেরত মিথ্যা;
  }

  destroy_guard.release ();
  ফেরত সত্য;
}

#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
শূন্য                                                                     \
hb_font_funcs_set_##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 শূন্য                        *user_তথ্য, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  যদি (!_hb_font_funcs_set_preamble (ffuncs, !func, &user_তথ্য, &destroy))\
      ফেরত;                                                            \
									 \
  যদি (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_তথ্য ? nullptr : ffuncs->user_তথ্য->name); \
                                                                         \
  যদি (!hb_font_funcs_set_মধ্য (ffuncs, user_তথ্য, destroy))           \
      ফেরত;                                                            \
									 \
  যদি (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  অন্যথা                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  যদি (ffuncs->user_তথ্য)                                                 \
    ffuncs->user_তথ্য->name = user_তথ্য;                                 \
  যদি (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

যৌক্তিক
hb_font_t::has_func_set (unsigned পূর্ণসংখ্যা i)
{
  ফেরত এই->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

যৌক্তিক
hb_font_t::has_func (unsigned পূর্ণসংখ্যা i)
{
  ফেরত has_func_set (i) ||
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
  ফেরত font->get_font_h_extents (extents);
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
  ফেরত font->get_font_v_extents (extents);
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
  যদি (unlikely (variation_selector))
    ফেরত font->get_variation_glyph (unicode, variation_selector, glyph);
  ফেরত font->get_nominal_glyph (unicode, glyph);
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
  ফেরত font->get_nominal_glyph (unicode, glyph);
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
unsigned পূর্ণসংখ্যা
hb_font_get_nominal_glyphs (hb_font_t *font,
			    unsigned পূর্ণসংখ্যা গুনতি,
			    ধ্রুবক hb_codepoint_t *first_unicode,
			    unsigned পূর্ণসংখ্যা unicode_stride,
			    hb_codepoint_t *first_glyph,
			    unsigned পূর্ণসংখ্যা glyph_stride)
{
  ফেরত font->get_nominal_glyphs (গুনতি,
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
  ফেরত font->get_variation_glyph (unicode, variation_selector, glyph);
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
  ফেরত font->get_glyph_h_advance (glyph);
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
  ফেরত font->get_glyph_v_advance (glyph);
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
শূন্য
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      unsigned পূর্ণসংখ্যা          গুনতি,
			      ধ্রুবক hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_h_advances (গুনতি, first_glyph, glyph_stride, first_advance, advance_stride);
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
শূন্য
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      unsigned পূর্ণসংখ্যা          গুনতি,
			      ধ্রুবক hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_v_advances (গুনতি, first_glyph, glyph_stride, first_advance, advance_stride);
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
  ফেরত font->get_glyph_h_origin (glyph, x, y);
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
  ফেরত font->get_glyph_v_origin (glyph, x, y);
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
			     unsigned পূর্ণসংখ্যা    গুনতি,
			     ধ্রুবক hb_codepoint_t *first_glyph,
			     unsigned পূর্ণসংখ্যা    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned পূর্ণসংখ্যা    x_stride,
			     hb_position_t  *first_y,
			     unsigned পূর্ণসংখ্যা    y_stride)

{
  ফেরত font->get_glyph_h_origins (গুনতি,
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
			     unsigned পূর্ণসংখ্যা    গুনতি,
			     ধ্রুবক hb_codepoint_t *first_glyph,
			     unsigned পূর্ণসংখ্যা    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned পূর্ণসংখ্যা    x_stride,
			     hb_position_t  *first_y,
			     unsigned পূর্ণসংখ্যা    y_stride)

{
  ফেরত font->get_glyph_v_origins (গুনতি,
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
			     hb_codepoint_t  বাম_glyph,
			     hb_codepoint_t  ডান_glyph)
{
  ফেরত font->get_glyph_h_kerning (বাম_glyph, ডান_glyph);
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
			     hb_codepoint_t  শীর্ষ_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  ফেরত font->get_glyph_v_kerning (শীর্ষ_glyph, bottom_glyph);
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
  ফেরত font->get_glyph_extents (glyph, extents);
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
				 unsigned পূর্ণসংখ্যা    point_সূচক,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  ফেরত font->get_glyph_contour_point (glyph, point_সূচক, x, y);
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
			অক্ষর           *name,
			unsigned পূর্ণসংখ্যা    আকার_2)
{
  ফেরত font->get_glyph_name (glyph, name, আকার_2);
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
			     ধ্রুবক অক্ষর     *name,
			     পূর্ণসংখ্যা             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  ফেরত font->get_glyph_from_name (name, len, glyph);
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
শূন্য
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, শূন্য *draw_তথ্য)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_তথ্য);
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
			    hb_draw_funcs_t *dfuncs, শূন্য *draw_তথ্য)
{
  ফেরত font->draw_glyph_or_fail (glyph, dfuncs, draw_তথ্য);
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
			     hb_paint_funcs_t *pfuncs, শূন্য *paint_তথ্য,
			     unsigned পূর্ণসংখ্যা palette_সূচক,
			     hb_color_t foreground)
{
  ফেরত font->paint_glyph_or_fail (glyph, pfuncs, paint_তথ্য, palette_সূচক, foreground);
}

/* A bit higher-level, and with fallback */

শূন্য
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
			unsigned পূর্ণসংখ্যা palette,
			hb_color_t foreground)
{
  যদি (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_তথ্য,
			   palette, foreground))
    ফেরত;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_তথ্য, glyph, এই, সত্য, foreground);
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
শূন্য
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, শূন্য *draw_তথ্য)
{
  (শূন্য) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_তথ্য);
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
শূন্য
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, শূন্য *paint_তথ্য,
                     unsigned পূর্ণসংখ্যা palette_সূচক,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_তথ্য, palette_সূচক, foreground);
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
শূন্য
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
শূন্য
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
HB_EXTERN শূন্য
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  unsigned পূর্ণসংখ্যা          গুনতি,
					  ধ্রুবক hb_codepoint_t *first_glyph,
					  unsigned              glyph_stride,
					  hb_position_t        *first_advance,
					  unsigned              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, গুনতি, first_glyph, glyph_stride, first_advance, advance_stride);
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
শূন্য
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  ফেরত font->get_glyph_origin_for_direction (glyph, direction, x, y);
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
শূন্য
hb_font_add_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  ফেরত font->add_glyph_origin_for_direction (glyph, direction, x, y);
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
শূন্য
hb_font_subtract_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  ফেরত font->subtract_glyph_origin_for_direction (glyph, direction, x, y);
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
শূন্য
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  first_glyph,
					 hb_codepoint_t  second_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  ফেরত font->get_glyph_kerning_for_direction (first_glyph, second_glyph, direction, x, y);
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
  ফেরত font->get_glyph_extents_for_origin (glyph, direction, extents);
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
					    unsigned পূর্ণসংখ্যা    point_সূচক,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  ফেরত font->get_glyph_contour_point_for_origin (glyph, point_সূচক, direction, x, y);
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
শূন্য
hb_font_glyph_to_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 অক্ষর           *s,
			 unsigned পূর্ণসংখ্যা    আকার_2)
{
  font->glyph_to_string (glyph, s, আকার_2);
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
			   ধ্রুবক অক্ষর     *s,
			   পূর্ণসংখ্যা             len,
			   hb_codepoint_t *glyph)
{
  ফেরত font->glyph_from_string (s, len, glyph);
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
  মিথ্যা, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  সত্য, /* embolden_in_place */
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
  মিথ্যা, /* has_nonzero_coords */
  0, /* num_coords */
  nullptr, /* coords */
  nullptr, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


স্থির hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  যদি (unlikely (!face))
    face = hb_face_get_empty ();

  যদি (!(font = hb_object_create<hb_font_t> ()))
    ফেরত hb_font_get_empty ();

  hb_face_make_immutable (face);
  font->parent = hb_font_get_empty ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_empty ();
  font->তথ্য.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_in_place = সত্য;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_সূচক = HB_FONT_NO_VAR_NAMED_INSTANCE;

  ফেরত font;
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
  যদি (likely (face))
  {
    যদি (face->সূচক_2 >> 16)
      hb_font_set_var_named_instance (font, (face->সূচক_2 >> 16) - 1);
    অন্যথা
      hb_font_set_variations (font, nullptr, 0);
  }
#endif

  ফেরত font;
}

স্থির শূন্য
_hb_font_adopt_var_coords (hb_font_t *font,
			   পূর্ণসংখ্যা *coords, /* 2.14 normalized */
			   দশমিক *design_coords,
			   unsigned পূর্ণসংখ্যা coords_দৈর্ঘ্য)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->অঙ্ক_coords = coords_দৈর্ঘ্য;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_দৈর্ঘ্য));

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
  যদি (unlikely (!parent))
    parent = hb_font_get_empty ();

  hb_font_t *font = _hb_font_create (parent->face);

  যদি (unlikely (hb_object_is_immutable (font)))
    ফেরত font;

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

  unsigned পূর্ণসংখ্যা অঙ্ক_coords = parent->অঙ্ক_coords;
  যদি (অঙ্ক_coords)
  {
    পূর্ণসংখ্যা *coords = (পূর্ণসংখ্যা *) hb_calloc (অঙ্ক_coords, আকার (parent->coords[0]));
    দশমিক *design_coords = (দশমিক *) hb_calloc (অঙ্ক_coords, আকার (parent->design_coords[0]));
    যদি (likely (coords && design_coords))
    {
      hb_memcpy (coords, parent->coords, অঙ্ক_coords * আকার (parent->coords[0]));
      hb_memcpy (design_coords, parent->design_coords, অঙ্ক_coords * আকার (parent->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, অঙ্ক_coords);
    }
    অন্যথা
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->changed ();
  font->serial_coords = font->serial;

  ফেরত font;
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
  ফেরত const_cast<hb_font_t *> (&Null (hb_font_t));
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
  ফেরত hb_object_reference (font);
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
শূন্য
hb_font_destroy (hb_font_t *font)
{
  যদি (!hb_object_destroy (font)) ফেরত;

  font->তথ্য.fini ();

  যদি (font->destroy)
    font->destroy (font->user_তথ্য);

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
hb_font_set_user_তথ্য (hb_font_t          *font,
		       hb_user_তথ্য_চাবি_t *key,
		       শূন্য *              তথ্য,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  যদি (!hb_object_is_immutable (font))
    font->changed ();

  ফেরত hb_object_set_user_তথ্য (font, key, তথ্য, destroy, replace);
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
শূন্য *
hb_font_get_user_তথ্য (ধ্রুবক hb_font_t    *font,
		       hb_user_তথ্য_চাবি_t *key)
{
  ফেরত hb_object_get_user_তথ্য (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
শূন্য
hb_font_make_immutable (hb_font_t *font)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->parent)
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
  ফেরত hb_object_is_immutable (font);
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
unsigned পূর্ণসংখ্যা
hb_font_get_serial (hb_font_t *font)
{
  ফেরত font->serial.get_acquire ();
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
শূন্য
hb_font_changed (hb_font_t *font)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

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
শূন্য
hb_font_set_parent (hb_font_t *font,
		    hb_font_t *parent)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (parent == font->parent)
    ফেরত;

  যদি (!parent)
    parent = hb_font_get_empty ();

  জন্য (hb_font_t *p = parent; p && p != hb_font_get_empty(); p = p->parent)
    যদি (p == font)
      ফেরত; /* Would create a cycle - reject */

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
  ফেরত font->parent;
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
শূন্য
hb_font_set_face (hb_font_t *font,
		  hb_face_t *face)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (face == font->face)
    ফেরত;

  যদি (unlikely (!face))
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
  ফেরত font->face;
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
শূন্য
hb_font_set_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   শূন্য              *font_তথ্য,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  যদি (hb_object_is_immutable (font))
  {
    যদি (destroy)
      destroy (font_তথ্য);
    ফেরত;
  }

  যদি (font->destroy)
    font->destroy (font->user_তথ্য);

  যদি (!klass)
    klass = hb_font_funcs_get_empty ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_তথ্য = font_তথ্য;
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
শূন্য
hb_font_set_funcs_তথ্য (hb_font_t         *font,
		        শূন্য              *font_তথ্য,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  যদি (hb_object_is_immutable (font))
  {
    যদি (destroy)
      destroy (font_তথ্য);
    ফেরত;
  }

  যদি (font->destroy)
    font->destroy (font->user_তথ্য);

  font->user_তথ্য = font_তথ্য;
  font->destroy = destroy;

  font->changed ();
}

স্থির ধ্রুবক কাঠামো supported_font_funcs_t {
	অক্ষর name[16];
	শূন্য (*func) (hb_font_t *);
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

স্থির ধ্রুবক অক্ষর *get_default_funcs_name ()
{
  স্থির hb_atomic_t<ধ্রুবক অক্ষর *> static_funcs_name;
  ধ্রুবক অক্ষর *name = static_funcs_name.get_acquire ();
  যদি (!name)
  {
    name = getenv ("HB_FONT_FUNCS");
    যদি (!name)
      name = "";
    যদি (!static_funcs_name.cmpexch (nullptr, name))
      name = static_funcs_name.get_acquire ();
  }
  ফেরত name;
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
			 ধ্রুবক অক্ষর *name)
{
  যদি (unlikely (hb_object_is_immutable (font)))
    ফেরত মিথ্যা;

  যৌক্তিক retry = মিথ্যা;

  যদি (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = সত্য;
  }
  যদি (name && !*name) name = nullptr;

retry:
  জন্য (unsigned i = 0; i < ARRAY_দৈর্ঘ্য (supported_font_funcs); i++)
    যদি (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      যদি (name || font->klass != hb_font_funcs_get_empty ())
	ফেরত সত্য;
    }

  যদি (retry)
  {
    retry = মিথ্যা;
    name = nullptr;
    যাও retry;
  }

  ফেরত মিথ্যা;
}

স্থির অন্তঃরেখ শূন্য free_static_font_funcs_list ();

স্থির ধ্রুবক অক্ষর * ধ্রুবক nil_font_funcs_list[] = {nullptr};

স্থির কাঠামো hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<ধ্রুবক অক্ষর *,
								  hb_font_funcs_list_lazy_loader_t>
{
  স্থির ধ্রুবক অক্ষর ** create ()
  {
    ধ্রুবক অক্ষর **font_funcs_list = (ধ্রুবক অক্ষর **) hb_calloc (1 + ARRAY_দৈর্ঘ্য (supported_font_funcs), আকার (ধ্রুবক অক্ষর *));
    যদি (unlikely (!font_funcs_list))
      ফেরত nullptr;

    unsigned i;
    জন্য (i = 0; i < ARRAY_দৈর্ঘ্য (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = nullptr;

    hb_atexit (free_static_font_funcs_list);

    ফেরত font_funcs_list;
  }
  স্থির শূন্য destroy (ধ্রুবক অক্ষর **l)
  { hb_free (l); }
  স্থির ধ্রুবক অক্ষর * ধ্রুবক * get_null ()
  { ফেরত nil_font_funcs_list; }
} static_font_funcs_list;

স্থির অন্তঃরেখ
শূন্য free_static_font_funcs_list ()
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
ধ্রুবক অক্ষর **
hb_font_list_funcs ()
{
  ফেরত static_font_funcs_list.get_unconst ();
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
শূন্য
hb_font_set_scale (hb_font_t *font,
		   পূর্ণসংখ্যা        x_scale,
		   পূর্ণসংখ্যা        y_scale)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->x_scale == x_scale && font->y_scale == y_scale)
    ফেরত;

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
শূন্য
hb_font_get_scale (hb_font_t *font,
		   পূর্ণসংখ্যা       *x_scale,
		   পূর্ণসংখ্যা       *y_scale)
{
  যদি (x_scale) *x_scale = font->x_scale;
  যদি (y_scale) *y_scale = font->y_scale;
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
শূন্য
hb_font_set_ppem (hb_font_t    *font,
		  unsigned পূর্ণসংখ্যা  x_ppem,
		  unsigned পূর্ণসংখ্যা  y_ppem)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    ফেরত;

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
শূন্য
hb_font_get_ppem (hb_font_t    *font,
		  unsigned পূর্ণসংখ্যা *x_ppem,
		  unsigned পূর্ণসংখ্যা *y_ppem)
{
  যদি (x_ppem) *x_ppem = font->x_ppem;
  যদি (y_ppem) *y_ppem = font->y_ppem;
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
শূন্য
hb_font_set_ptem (hb_font_t *font,
		  দশমিক      ptem)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->ptem == ptem)
    ফেরত;

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
দশমিক
hb_font_get_ptem (hb_font_t *font)
{
  ফেরত font->ptem;
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
  ফেরত font->is_synthetic;
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
শূন্য
hb_font_set_synthetic_bold (hb_font_t *font,
			    দশমিক x_embolden,
			    দশমিক y_embolden,
			    hb_bool_t in_place)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_in_place == (যৌক্তিক) in_place)
    ফেরত;

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
শূন্য
hb_font_get_synthetic_bold (hb_font_t *font,
			    দশমিক *x_embolden,
			    দশমিক *y_embolden,
			    hb_bool_t *in_place)
{
  যদি (x_embolden) *x_embolden = font->x_embolden;
  যদি (y_embolden) *y_embolden = font->y_embolden;
  যদি (in_place) *in_place = font->embolden_in_place;
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
HB_EXTERN শূন্য
hb_font_set_synthetic_slant (hb_font_t *font, দশমিক slant)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->slant == slant)
    ফেরত;

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
HB_EXTERN দশমিক
hb_font_get_synthetic_slant (hb_font_t *font)
{
  ফেরত font->slant;
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
শূন্য
hb_font_set_variations (hb_font_t            *font,
			ধ্রুবক hb_variation_t *variations,
			unsigned পূর্ণসংখ্যা          variations_দৈর্ঘ্য)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  ধ্রুবক OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ধ্রুবক unsigned coords_দৈর্ঘ্য = axes.দৈর্ঘ্য;

  পূর্ণসংখ্যা *normalized = coords_দৈর্ঘ্য ? (পূর্ণসংখ্যা *) hb_calloc (coords_দৈর্ঘ্য, আকার (পূর্ণসংখ্যা)) : nullptr;
  দশমিক *design_coords = coords_দৈর্ঘ্য ? (দশমিক *) hb_calloc (coords_দৈর্ঘ্য, আকার (দশমিক)) : nullptr;

  যদি (unlikely (coords_দৈর্ঘ্য && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ফেরত;
  }

  /* Initialize design coords. */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < coords_দৈর্ঘ্য; i++)
    design_coords[i] = axes[i].get_default ();
  যদি (font->instance_সূচক != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    unsigned গুনতি = coords_দৈর্ঘ্য;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_সূচক,
						&গুনতি, design_coords);
  }

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < variations_দৈর্ঘ্য; i++)
  {
    ধ্রুবক auto tag = variations[i].tag;
    ধ্রুবক auto v = variations[i].value;
    জন্য (unsigned axis_সূচক = 0; axis_সূচক < coords_দৈর্ঘ্য; axis_সূচক++)
      যদি (axes[axis_সূচক].axisTag == tag)
	design_coords[axis_সূচক] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_দৈর্ঘ্য, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_দৈর্ঘ্য);
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
শূন্য
hb_font_set_variation (hb_font_t *font,
		       hb_tag_t tag,
		       দশমিক    value)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  // TODO Share some of this code with set_variations()

  ধ্রুবক OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ধ্রুবক unsigned coords_দৈর্ঘ্য = axes.দৈর্ঘ্য;

  পূর্ণসংখ্যা *normalized = coords_দৈর্ঘ্য ? (পূর্ণসংখ্যা *) hb_calloc (coords_দৈর্ঘ্য, আকার (পূর্ণসংখ্যা)) : nullptr;
  দশমিক *design_coords = coords_দৈর্ঘ্য ? (দশমিক *) hb_calloc (coords_দৈর্ঘ্য, আকার (দশমিক)) : nullptr;

  যদি (unlikely (coords_দৈর্ঘ্য && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ফেরত;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  যদি (font->design_coords && font->অঙ্ক_coords == coords_দৈর্ঘ্য)
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < coords_দৈর্ঘ্য; i++)
      design_coords[i] = font->design_coords[i];
  }
  অন্যথা
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < coords_দৈর্ঘ্য; i++)
      design_coords[i] = axes[i].get_default ();
    যদি (font->instance_সূচক != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      unsigned গুনতি = coords_দৈর্ঘ্য;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_সূচক,
						  &গুনতি, design_coords);
    }
  }

  জন্য (unsigned axis_সূচক = 0; axis_সূচক < coords_দৈর্ঘ্য; axis_সূচক++)
    যদি (axes[axis_সূচক].axisTag == tag)
      design_coords[axis_সূচক] = value;

  hb_ot_var_normalize_coords (font->face, coords_দৈর্ঘ্য, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_দৈর্ঘ্য);
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
শূন্য
hb_font_set_var_coords_design (hb_font_t    *font,
			       ধ্রুবক দশমিক  *coords,
			       unsigned পূর্ণসংখ্যা  প্রবেশ_coords_দৈর্ঘ্য)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  ধ্রুবক OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ধ্রুবক unsigned coords_দৈর্ঘ্য = axes.দৈর্ঘ্য;

  প্রবেশ_coords_দৈর্ঘ্য = hb_min (প্রবেশ_coords_দৈর্ঘ্য, coords_দৈর্ঘ্য);
  পূর্ণসংখ্যা *normalized = coords_দৈর্ঘ্য ? (পূর্ণসংখ্যা *) hb_calloc (coords_দৈর্ঘ্য, আকার (পূর্ণসংখ্যা)) : nullptr;
  দশমিক *design_coords = coords_দৈর্ঘ্য ? (দশমিক *) hb_calloc (coords_দৈর্ঘ্য, আকার (দশমিক)) : nullptr;

  যদি (unlikely (coords_দৈর্ঘ্য && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ফেরত;
  }

  যদি (প্রবেশ_coords_দৈর্ঘ্য)
    hb_memcpy (design_coords, coords, প্রবেশ_coords_দৈর্ঘ্য * আকার (font->design_coords[0]));
  // Fill in the rest with default values
  জন্য (unsigned পূর্ণসংখ্যা i = প্রবেশ_coords_দৈর্ঘ্য; i < coords_দৈর্ঘ্য; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_দৈর্ঘ্য, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_দৈর্ঘ্য);
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
শূন্য
hb_font_set_var_named_instance (hb_font_t *font,
				unsigned পূর্ণসংখ্যা instance_সূচক)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  যদি (font->instance_সূচক == instance_সূচক)
    ফেরত;

  font->instance_সূচক = instance_সূচক;
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
unsigned পূর্ণসংখ্যা
hb_font_get_var_named_instance (hb_font_t *font)
{
  ফেরত font->instance_সূচক;
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
শূন্য
hb_font_set_var_coords_normalized (hb_font_t    *font,
				   ধ্রুবক পূর্ণসংখ্যা    *coords, /* 2.14 normalized */
				   unsigned পূর্ণসংখ্যা  প্রবেশ_coords_দৈর্ঘ্য)
{
  যদি (hb_object_is_immutable (font))
    ফেরত;

  ধ্রুবক OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  unsigned coords_দৈর্ঘ্য = axes.দৈর্ঘ্য;

  প্রবেশ_coords_দৈর্ঘ্য = hb_min (প্রবেশ_coords_দৈর্ঘ্য, coords_দৈর্ঘ্য);
  পূর্ণসংখ্যা *copy = coords_দৈর্ঘ্য ? (পূর্ণসংখ্যা *) hb_calloc (coords_দৈর্ঘ্য, আকার (coords[0])) : nullptr;
  দশমিক *design_coords = coords_দৈর্ঘ্য ? (দশমিক *) hb_calloc (coords_দৈর্ঘ্য, আকার (design_coords[0])) : nullptr;

  যদি (unlikely (coords_দৈর্ঘ্য && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    ফেরত;
  }

  যদি (প্রবেশ_coords_দৈর্ঘ্য)
    hb_memcpy (copy, coords, প্রবেশ_coords_দৈর্ঘ্য * আকার (coords[0]));

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < coords_দৈর্ঘ্য; ++i)
    design_coords[i] = NAN;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_দৈর্ঘ্য);
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
ধ্রুবক পূর্ণসংখ্যা *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   unsigned পূর্ণসংখ্যা *দৈর্ঘ্য)
{
  যদি (দৈর্ঘ্য)
    *দৈর্ঘ্য = font->অঙ্ক_coords;

  ফেরত font->coords;
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
ধ্রুবক দশমিক *
hb_font_get_var_coords_design (hb_font_t *font,
			       unsigned পূর্ণসংখ্যা *দৈর্ঘ্য)
{
  যদি (দৈর্ঘ্য)
    *দৈর্ঘ্য = font->অঙ্ক_coords;

  ফেরত font->design_coords;
}
#endif

#ifndef HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

কাঠামো hb_trampoline_closure_t
{
  শূন্য *user_তথ্য;
  hb_destroy_func_t destroy;
  unsigned পূর্ণসংখ্যা ref_গুনতি_2;
};

template <typename FuncType>
কাঠামো hb_trampoline_t
{
  hb_trampoline_closure_t closure; /* Must be first. */
  FuncType func;
};

template <typename FuncType>
স্থির hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   শূন্য              *user_তথ্য,
		   hb_destroy_func_t  destroy)
{
  প্রকারলেখ hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, আকার (trampoline_t));

  যদি (unlikely (!trampoline))
    ফেরত nullptr;

  trampoline->closure.user_তথ্য = user_তথ্য;
  trampoline->closure.destroy = destroy;
  trampoline->closure.ref_গুনতি_2 = 1;
  trampoline->func = func;

  ফেরত trampoline;
}

স্থির শূন্য
trampoline_reference (hb_trampoline_closure_t *closure)
{
  closure->ref_গুনতি_2++;
}

স্থির শূন্য
trampoline_destroy (শূন্য *user_তথ্য)
{
  hb_trampoline_closure_t *closure = (hb_trampoline_closure_t *) user_তথ্য;

  যদি (--closure->ref_গুনতি_2)
    ফেরত;

  যদি (closure->destroy)
    closure->destroy (closure->user_তথ্য);
  hb_free (closure);
}

প্রকারলেখ hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

স্থির hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      শূন্য           *font_তথ্য,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      শূন্য           *user_তথ্য)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_তথ্য;
  ফেরত trampoline->func (font, font_তথ্য, unicode, 0, glyph, trampoline->closure.user_তথ্য);
}

স্থির hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					শূন্য           *font_তথ্য,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					শূন্য           *user_তথ্য)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_তথ্য;
  ফেরত trampoline->func (font, font_তথ্য, unicode, variation_selector, glyph, trampoline->closure.user_তথ্য);
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
শূন্য
hb_font_funcs_set_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      শূন্য                     *user_তথ্য,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  যদি (hb_object_is_immutable (ffuncs))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_তথ্য, destroy);
  যদি (unlikely (!trampoline))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
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

কাঠামো hb_draw_glyph_closure_t
{
  hb_font_draw_glyph_func_t func;
  শূন্য *user_তথ্য;
  hb_destroy_func_t destroy;
};
স্থির hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       শূন্য            *font_তথ্য,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       শূন্য            *draw_তথ্য,
			       শূন্য            *user_তথ্য)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_তথ্য;
  closure->func (font, font_তথ্য, glyph, draw_funcs, draw_তথ্য, closure->user_তথ্য);
  ফেরত সত্য;
}
স্থির শূন্য
hb_font_draw_glyph_closure_destroy (শূন্য *user_তথ্য)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_তথ্য;

  যদি (closure->destroy)
    closure->destroy (closure->user_তথ্য);
  hb_free (closure);
}
স্থির শূন্য
_hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    শূন্য                      *user_তথ্য,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  যদি (hb_object_is_immutable (ffuncs))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) hb_calloc (1, আকার (hb_draw_glyph_closure_t));
  যদি (unlikely (!closure))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }
  closure->func = func;
  closure->user_তথ্য = user_তথ্য;
  closure->destroy = destroy;

  hb_font_funcs_set_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     closure,
					     hb_font_draw_glyph_closure_destroy);
}
শূন্য
hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   শূন্য                      *user_তথ্য,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_তথ্য, destroy);
}
শূন্য
hb_font_funcs_set_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   শূন্য                           *user_তথ্য,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_তথ্য, destroy);
}

কাঠামো hb_paint_glyph_closure_t
{
  hb_font_paint_glyph_func_t func;
  শূন্য *user_তথ্য;
  hb_destroy_func_t destroy;
};
স্থির hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				শূন্য *font_তথ্য,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				শূন্য *paint_তথ্য,
				unsigned পূর্ণসংখ্যা palette,
				hb_color_t foreground,
				শূন্য *user_তথ্য)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_তথ্য;
  closure->func (font, font_তথ্য, glyph, paint_funcs, paint_তথ্য, palette, foreground, closure->user_তথ্য);
  ফেরত সত্য;
}
স্থির শূন্য
hb_font_paint_glyph_closure_destroy (শূন্য *user_তথ্য)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_তথ্য;

  যদি (closure->destroy)
    closure->destroy (closure->user_তথ্য);
  hb_free (closure);
}
শূন্য
hb_font_funcs_set_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    শূন্য                      *user_তথ্য,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  যদি (hb_object_is_immutable (ffuncs))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) hb_calloc (1, আকার (hb_paint_glyph_closure_t));
  যদি (unlikely (!closure))
  {
    যদি (destroy)
      destroy (user_তথ্য);
    ফেরত;
  }
  closure->func = func;
  closure->user_তথ্য = user_তথ্য;
  closure->destroy = destroy;

  hb_font_funcs_set_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      closure,
					      hb_font_paint_glyph_closure_destroy);
}
#endif
