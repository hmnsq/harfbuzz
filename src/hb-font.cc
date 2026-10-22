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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-paint.hh"
#अन्तर्भाव "hb-machinery.hh"

#अन्तर्भाव "hb-ot.h"

#अन्तर्भाव "hb-ot-var-avar-table.hh"
#अन्तर्भाव "hb-ot-var-fvar-table.hh"

#ifndef HB_NO_OT_FONT
#अन्तर्भाव "hb-ot.h"
#endif
#ifdef HAVE_FREETYPE
#अन्तर्भाव "hb-ft.h"
#endif
#ifdef HAVE_FONTATIONS
#अन्तर्भाव "hb-fontations.h"
#endif
#ifdef HAVE_CORETEXT
#अन्तर्भाव "hb-coretext.h"
#endif
#ifdef HAVE_DIRECTWRITE
#अन्तर्भाव "hb-directwrite.h"
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

स्थिर hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_UNUSED,
				शून्य              *font_data HB_UNUSED,
				hb_font_extents_t *extents,
				शून्य              *user_data HB_UNUSED)
{
  hb_memset (extents, 0, परिमाण (*extents));
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    शून्य              *font_data HB_UNUSED,
				    hb_font_extents_t *extents,
				    शून्य              *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_h_extents (extents, मिच्छ);
  जइ (ret) {
    extents->ascender = font->parent_scale_y_distance (extents->ascender);
    extents->descender = font->parent_scale_y_distance (extents->descender);
    extents->पङ्क्तिः_gap = font->parent_scale_y_distance (extents->पङ्क्तिः_gap);
  }
  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_UNUSED,
				शून्य              *font_data HB_UNUSED,
				hb_font_extents_t *extents,
				शून्य              *user_data HB_UNUSED)
{
  hb_memset (extents, 0, परिमाण (*extents));
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    शून्य              *font_data HB_UNUSED,
				    hb_font_extents_t *extents,
				    शून्य              *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_v_extents (extents, मिच्छ);
  जइ (ret) {
    extents->ascender = font->parent_scale_x_distance (extents->ascender);
    extents->descender = font->parent_scale_x_distance (extents->descender);
    extents->पङ्क्तिः_gap = font->parent_scale_x_distance (extents->पङ्क्तिः_gap);
  }
  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_UNUSED,
			       शून्य           *font_data HB_UNUSED,
			       hb_codepoint_t  unicode HB_UNUSED,
			       hb_codepoint_t *glyph,
			       शून्य           *user_data HB_UNUSED)
{
  *glyph = 0;
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   शून्य           *font_data HB_UNUSED,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   शून्य           *user_data HB_UNUSED)
{
  जइ (font->has_nominal_glyphs_func_set ())
  {
    प्रत्यागम font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  प्रत्यागम font->parent->get_nominal_glyph (unicode, glyph);
}

#define hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

स्थिर unsigned पूर्णाङ्क
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    शून्य                 *font_data HB_UNUSED,
				    unsigned पूर्णाङ्क          गणनम्,
				    ध्रुव hb_codepoint_t *first_unicode,
				    unsigned पूर्णाङ्क          unicode_stride,
				    hb_codepoint_t       *first_glyph,
				    unsigned पूर्णाङ्क          glyph_stride,
				    शून्य                 *user_data HB_UNUSED)
{
  जइ (font->has_nominal_glyph_func_set ())
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      जइ (!font->get_nominal_glyph (*first_unicode, first_glyph))
	प्रत्यागम i;

      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    प्रत्यागम गणनम्;
  }

  प्रत्यागम font->parent->get_nominal_glyphs (गणनम्,
					   first_unicode, unicode_stride,
					   first_glyph, glyph_stride);
}

स्थिर hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_UNUSED,
				 शून्य           *font_data HB_UNUSED,
				 hb_codepoint_t  unicode HB_UNUSED,
				 hb_codepoint_t  variation_selector HB_UNUSED,
				 hb_codepoint_t *glyph,
				 शून्य           *user_data HB_UNUSED)
{
  *glyph = 0;
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->parent->get_variation_glyph (unicode, variation_selector, glyph);
}


स्थिर hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 शून्य           *font_data HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->x_scale;
}

स्थिर hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph,
				     शून्य           *user_data HB_UNUSED)
{
  जइ (font->has_glyph_h_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, मिच्छ);
    प्रत्यागम ret;
  }
  प्रत्यागम font->parent_scale_x_distance (font->parent->get_glyph_h_advance (glyph, मिच्छ));
}

स्थिर hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 शून्य           *font_data HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम -font->y_scale;
}

स्थिर hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph,
				     शून्य           *user_data HB_UNUSED)
{
  जइ (font->has_glyph_v_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, मिच्छ);
    प्रत्यागम ret;
  }
  प्रत्यागम font->parent_scale_y_distance (font->parent->get_glyph_v_advance (glyph, मिच्छ));
}

#define hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

स्थिर शून्य
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      शून्य*                 font_data HB_UNUSED,
				      unsigned पूर्णाङ्क          गणनम्,
				      ध्रुव hb_codepoint_t *first_glyph,
				      unsigned पूर्णाङ्क          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned पूर्णाङ्क          advance_stride,
				      शून्य                 *user_data HB_UNUSED)
{
  जइ (font->has_glyph_h_advance_func_set ())
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      *first_advance = font->get_glyph_h_advance (*first_glyph, मिच्छ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    प्रत्यागम;
  }

  font->parent->get_glyph_h_advances (गणनम्,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      मिच्छ);
  प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
  {
    *first_advance = font->parent_scale_x_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#define hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
स्थिर शून्य
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      शून्य*                 font_data HB_UNUSED,
				      unsigned पूर्णाङ्क          गणनम्,
				      ध्रुव hb_codepoint_t *first_glyph,
				      unsigned पूर्णाङ्क          glyph_stride,
				      hb_position_t        *first_advance,
				      unsigned पूर्णाङ्क          advance_stride,
				      शून्य                 *user_data HB_UNUSED)
{
  जइ (font->has_glyph_v_advance_func_set ())
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      *first_advance = font->get_glyph_v_advance (*first_glyph, मिच्छ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    प्रत्यागम;
  }

  font->parent->get_glyph_v_advances (गणनम्,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      मिच्छ);
  प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
  {
    *first_advance = font->parent_scale_y_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

स्थिर hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_UNUSED,
				शून्य           *font_data HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				शून्य           *user_data HB_UNUSED)
{
  *x = *y = 0;
  प्रत्यागम सच्च;
}

स्थिर hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    शून्य           *font_data HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    शून्य           *user_data HB_UNUSED)
{
  जइ (font->has_glyph_h_origins_func_set ())
  {
    प्रत्यागम font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, मिच्छ);
  }
  hb_bool_t ret = font->parent->get_glyph_h_origin (glyph, x, y);
  जइ (ret)
    font->parent_scale_position (x, y);
  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_UNUSED,
				शून्य           *font_data HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    शून्य           *font_data HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    शून्य           *user_data HB_UNUSED)
{
  जइ (font->has_glyph_v_origins_func_set ())
  {
    प्रत्यागम font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, मिच्छ);
  }
  hb_bool_t ret = font->parent->get_glyph_v_origin (glyph, x, y);
  जइ (ret)
    font->parent_scale_position (x, y);
  प्रत्यागम ret;
}

#define hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

स्थिर hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_UNUSED,
				     शून्य *font_data HB_UNUSED,
				     unsigned पूर्णाङ्क गणनम्,
				     ध्रुव hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     शून्य *user_data HB_UNUSED)
{
  जइ (font->has_glyph_h_origin_func_set ())
  {
    hb_bool_t ret = सच्च;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      ret &= font->get_glyph_h_origin (*first_glyph, first_x, first_y, मिच्छ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    प्रत्यागम ret;
  }

  hb_bool_t ret = font->parent->get_glyph_h_origins (गणनम्,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  जइ (ret)
  {
    प्रति (unsigned i = 0; i < गणनम्; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  प्रत्यागम ret;
}

#define hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

स्थिर hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_UNUSED,
				     शून्य *font_data HB_UNUSED,
				     unsigned पूर्णाङ्क गणनम्,
				     ध्रुव hb_codepoint_t *first_glyph HB_UNUSED,
				     unsigned glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     unsigned x_stride,
				     hb_position_t *first_y,
				     unsigned y_stride,
				     शून्य *user_data HB_UNUSED)
{
  जइ (font->has_glyph_v_origin_func_set ())
  {
    hb_bool_t ret = सच्च;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      ret &= font->get_glyph_v_origin (*first_glyph, first_x, first_y, मिच्छ);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    प्रत्यागम ret;
  }

  hb_bool_t ret = font->parent->get_glyph_v_origins (गणनम्,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  जइ (ret)
  {
    प्रति (unsigned i = 0; i < गणनम्; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  प्रत्यागम ret;
}

स्थिर hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_UNUSED,
				 शून्य           *font_data HB_UNUSED,
				 hb_codepoint_t  वामः_glyph HB_UNUSED,
				 hb_codepoint_t  दक्षिणः_glyph HB_UNUSED,
				 शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम 0;
}

स्थिर hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  वामः_glyph,
				     hb_codepoint_t  दक्षिणः_glyph,
				     शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->parent_scale_x_distance (font->parent->get_glyph_h_kerning (वामः_glyph, दक्षिणः_glyph));
}

#ifndef HB_DISABLE_DEPRECATED
स्थिर hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_UNUSED,
				 शून्य           *font_data HB_UNUSED,
				 hb_codepoint_t  शिखरम्_glyph HB_UNUSED,
				 hb_codepoint_t  bottom_glyph HB_UNUSED,
				 शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम 0;
}

स्थिर hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  शिखरम्_glyph,
				     hb_codepoint_t  bottom_glyph,
				     शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->parent_scale_y_distance (font->parent->get_glyph_v_kerning (शिखरम्_glyph, bottom_glyph));
}
#endif

स्थिर hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_UNUSED,
			       शून्य               *font_data HB_UNUSED,
			       hb_codepoint_t      glyph HB_UNUSED,
			       hb_glyph_extents_t *extents,
			       शून्य               *user_data HB_UNUSED)
{
  hb_memset (extents, 0, परिमाण (*extents));
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   शून्य               *font_data HB_UNUSED,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   शून्य               *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_extents (glyph, extents, मिच्छ);
  जइ (ret) {
    font->parent_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->parent_scale_distance (&extents->width, &extents->height);
  }
  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_font_get_glyph_contour_point_nil (hb_font_t      *font HB_UNUSED,
				     शून्य           *font_data HB_UNUSED,
				     hb_codepoint_t  glyph HB_UNUSED,
				     unsigned पूर्णाङ्क    point_index HB_UNUSED,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     शून्य           *user_data HB_UNUSED)
{
  *x = *y = 0;
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_glyph_contour_point_default (hb_font_t      *font,
					 शून्य           *font_data HB_UNUSED,
					 hb_codepoint_t  glyph,
					 unsigned पूर्णाङ्क    point_index,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 शून्य           *user_data HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_contour_point (glyph, point_index, x, y, मिच्छ);
  जइ (ret)
    font->parent_scale_position (x, y);
  प्रत्यागम ret;
}

स्थिर hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_UNUSED,
			    शून्य           *font_data HB_UNUSED,
			    hb_codepoint_t  glyph HB_UNUSED,
			    वर्ण           *name,
			    unsigned पूर्णाङ्क    size,
			    शून्य           *user_data HB_UNUSED)
{
  जइ (size) *name = '\0';
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				शून्य           *font_data HB_UNUSED,
				hb_codepoint_t  glyph,
				वर्ण           *name,
				unsigned पूर्णाङ्क    size,
				शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->parent->get_glyph_name (glyph, name, size);
}

स्थिर hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_UNUSED,
				 शून्य           *font_data HB_UNUSED,
				 ध्रुव वर्ण     *name HB_UNUSED,
				 पूर्णाङ्क             len HB_UNUSED, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 शून्य           *user_data HB_UNUSED)
{
  *glyph = 0;
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     शून्य           *font_data HB_UNUSED,
				     ध्रुव वर्ण     *name,
				     पूर्णाङ्क             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     शून्य           *user_data HB_UNUSED)
{
  प्रत्यागम font->parent->get_glyph_from_name (name, len, glyph);
}

स्थिर hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_UNUSED,
				शून्य            *font_data HB_UNUSED,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				शून्य            *draw_data,
				शून्य            *user_data HB_UNUSED)
{
  प्रत्यागम मिच्छ;
}

स्थिर hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_UNUSED,
				 शून्य *font_data HB_UNUSED,
				 hb_codepoint_t glyph HB_UNUSED,
				 hb_paint_funcs_t *paint_funcs HB_UNUSED,
				 शून्य *paint_data HB_UNUSED,
				 unsigned पूर्णाङ्क palette HB_UNUSED,
				 hb_color_t foreground HB_UNUSED,
				 शून्य *user_data HB_UNUSED)
{
  प्रत्यागम मिच्छ;
}

प्रकारनिर्देश संरचना hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  शून्य		  *draw_data;
  भिन्न		   x_scale;
  भिन्न		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

स्थिर शून्य
hb_draw_move_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 शून्य *draw_data,
			 hb_draw_state_t *st,
			 भिन्न to_x, भिन्न to_y,
			 शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  भिन्न x_scale = adaptor->x_scale;
  भिन्न y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_to (adaptor->draw_data, *st,
				     x_scale * to_x, y_scale * to_y);
}

स्थिर शून्य
hb_draw_पङ्क्तिः_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			 hb_draw_state_t *st,
			 भिन्न to_x, भिन्न to_y,
			 शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  भिन्न x_scale = adaptor->x_scale;
  भिन्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_पङ्क्तिः_to (adaptor->draw_data, *st,
				     x_scale * to_x, y_scale * to_y);
}

स्थिर शून्य
hb_draw_quadratic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			      hb_draw_state_t *st,
			      भिन्न control_x, भिन्न control_y,
			      भिन्न to_x, भिन्न to_y,
			      शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  भिन्न x_scale = adaptor->x_scale;
  भिन्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_to (adaptor->draw_data, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * to_x, y_scale * to_y);
}

स्थिर शून्य
hb_draw_cubic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			  hb_draw_state_t *st,
			  भिन्न control1_x, भिन्न control1_y,
			  भिन्न control2_x, भिन्न control2_y,
			  भिन्न to_x, भिन्न to_y,
			  शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  भिन्न x_scale = adaptor->x_scale;
  भिन्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_to (adaptor->draw_data, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * to_x, y_scale * to_y);
}

स्थिर शून्य
hb_draw_close_path_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			    hb_draw_state_t *st,
			    शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;

  adaptor->draw_funcs->emit_close_path (adaptor->draw_data, *st);
}

स्थिर hb_bool_t
hb_draw_set_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			    int64_t budget,
			    शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  प्रत्यागम adaptor->draw_funcs->set_budget (adaptor->draw_data, budget);
}

स्थिर int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
			    शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  प्रत्यागम adaptor->draw_funcs->get_budget (adaptor->draw_data);
}

स्थिर int64_t *
hb_draw_get_budget_remaining_default (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *draw_data,
				      शून्य *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_data;
  प्रत्यागम adaptor->draw_funcs->get_budget_remaining_ptr (adaptor->draw_data);
}

स्थिर ध्रुव hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  {
#define HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_DRAW_FUNC_IMPLEMENT
  }
};

स्थिर hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    शून्य            *font_data HB_UNUSED,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    शून्य            *draw_data,
				    शून्य            *user_data HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_data,
    font->parent->x_scale ? (भिन्न) font->x_scale / (भिन्न) font->parent->x_scale : 0.f,
    font->parent->y_scale ? (भिन्न) font->y_scale / (भिन्न) font->parent->y_scale : 0.f
  };

  प्रत्यागम font->parent->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   मिच्छ);
}

स्थिर hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     शून्य *font_data,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     शून्य *paint_data,
				     unsigned पूर्णाङ्क palette,
				     hb_color_t foreground,
				     शून्य *user_data)
{
  paint_funcs->प्रक्षिप_transform (paint_data,
    font->parent->x_scale ? (भिन्न) font->x_scale / (भिन्न) font->parent->x_scale : 0, 0,
    0, font->parent->y_scale ? (भिन्न) font->y_scale / (भिन्न) font->parent->y_scale : 0,
    0, 0);

  तार्किक ret = font->parent->paint_glyph_or_fail (glyph, paint_funcs, paint_data, palette, foreground);

  paint_funcs->निष्कासय_transform (paint_data);

  प्रत्यागम ret;
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

स्थिर ध्रुव hb_font_funcs_t _hb_font_funcs_default = {
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

  जइ (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    प्रत्यागम hb_font_funcs_get_empty ();

  ffuncs->get = _hb_font_funcs_default.get;

  प्रत्यागम ffuncs;
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
  प्रत्यागम const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
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
  प्रत्यागम hb_object_reference (ffuncs);
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
शून्य
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  जइ (!hb_object_destroy (ffuncs)) प्रत्यागम;

  जइ (ffuncs->destroy)
  {
#define HB_FONT_FUNC_IMPLEMENT(get_,name) जइ (ffuncs->destroy->name) \
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
			     hb_user_data_कुञ्चिका_t *key,
			     शून्य *              data,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  प्रत्यागम hb_object_set_user_data (ffuncs, key, data, destroy, replace);
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
शून्य *
hb_font_funcs_get_user_data (ध्रुव hb_font_funcs_t *ffuncs,
			     hb_user_data_कुञ्चिका_t    *key)
{
  प्रत्यागम hb_object_get_user_data (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
शून्य
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  जइ (hb_object_is_immutable (ffuncs))
    प्रत्यागम;

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
  प्रत्यागम hb_object_is_immutable (ffuncs);
}


स्थिर तार्किक
_hb_font_funcs_set_preamble (hb_font_funcs_t    *ffuncs,
			     तार्किक                func_is_null,
			     शून्य              **user_data,
			     hb_destroy_func_t  *destroy)
{
  जइ (hb_object_is_immutable (ffuncs))
  {
    जइ (*destroy)
      (*destroy) (*user_data);
    प्रत्यागम मिच्छ;
  }

  जइ (func_is_null)
  {
    जइ (*destroy)
      (*destroy) (*user_data);
    *destroy = nullptr;
    *user_data = nullptr;
  }

  प्रत्यागम सच्च;
}

स्थिर तार्किक
hb_font_funcs_set_मध्यः (hb_font_funcs_t   *ffuncs,
			   शून्य              *user_data,
			   hb_destroy_func_t  destroy)
{
  auto destroy_guard = hb_make_scope_guard ([&]() {
    जइ (destroy) destroy (user_data);
  });

  जइ (user_data && !ffuncs->user_data)
  {
    ffuncs->user_data = (decltype (ffuncs->user_data)) hb_calloc (1, परिमाण (*ffuncs->user_data));
    जइ (unlikely (!ffuncs->user_data))
      प्रत्यागम मिच्छ;
  }
  जइ (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, परिमाण (*ffuncs->destroy));
    जइ (unlikely (!ffuncs->destroy))
      प्रत्यागम मिच्छ;
  }

  destroy_guard.release ();
  प्रत्यागम सच्च;
}

#define HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
शून्य                                                                     \
hb_font_funcs_set_##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 शून्य                        *user_data, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  जइ (!_hb_font_funcs_set_preamble (ffuncs, !func, &user_data, &destroy))\
      प्रत्यागम;                                                            \
									 \
  जइ (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_data ? nullptr : ffuncs->user_data->name); \
                                                                         \
  जइ (!hb_font_funcs_set_मध्यः (ffuncs, user_data, destroy))           \
      प्रत्यागम;                                                            \
									 \
  जइ (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  अण्णहा                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  जइ (ffuncs->user_data)                                                 \
    ffuncs->user_data->name = user_data;                                 \
  जइ (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_FONT_FUNC_IMPLEMENT

तार्किक
hb_font_t::has_func_set (unsigned पूर्णाङ्क i)
{
  प्रत्यागम एतत्->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

तार्किक
hb_font_t::has_func (unsigned पूर्णाङ्क i)
{
  प्रत्यागम has_func_set (i) ||
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
  प्रत्यागम font->get_font_h_extents (extents);
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
  प्रत्यागम font->get_font_v_extents (extents);
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
  जइ (unlikely (variation_selector))
    प्रत्यागम font->get_variation_glyph (unicode, variation_selector, glyph);
  प्रत्यागम font->get_nominal_glyph (unicode, glyph);
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
  प्रत्यागम font->get_nominal_glyph (unicode, glyph);
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
unsigned पूर्णाङ्क
hb_font_get_nominal_glyphs (hb_font_t *font,
			    unsigned पूर्णाङ्क गणनम्,
			    ध्रुव hb_codepoint_t *first_unicode,
			    unsigned पूर्णाङ्क unicode_stride,
			    hb_codepoint_t *first_glyph,
			    unsigned पूर्णाङ्क glyph_stride)
{
  प्रत्यागम font->get_nominal_glyphs (गणनम्,
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
  प्रत्यागम font->get_variation_glyph (unicode, variation_selector, glyph);
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
  प्रत्यागम font->get_glyph_h_advance (glyph);
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
  प्रत्यागम font->get_glyph_v_advance (glyph);
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
शून्य
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      unsigned पूर्णाङ्क          गणनम्,
			      ध्रुव hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_h_advances (गणनम्, first_glyph, glyph_stride, first_advance, advance_stride);
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
शून्य
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      unsigned पूर्णाङ्क          गणनम्,
			      ध्रुव hb_codepoint_t *first_glyph,
			      unsigned              glyph_stride,
			      hb_position_t        *first_advance,
			      unsigned              advance_stride)
{
  font->get_glyph_v_advances (गणनम्, first_glyph, glyph_stride, first_advance, advance_stride);
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
  प्रत्यागम font->get_glyph_h_origin (glyph, x, y);
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
  प्रत्यागम font->get_glyph_v_origin (glyph, x, y);
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
			     unsigned पूर्णाङ्क    गणनम्,
			     ध्रुव hb_codepoint_t *first_glyph,
			     unsigned पूर्णाङ्क    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned पूर्णाङ्क    x_stride,
			     hb_position_t  *first_y,
			     unsigned पूर्णाङ्क    y_stride)

{
  प्रत्यागम font->get_glyph_h_origins (गणनम्,
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
			     unsigned पूर्णाङ्क    गणनम्,
			     ध्रुव hb_codepoint_t *first_glyph,
			     unsigned पूर्णाङ्क    glyph_stride,
			     hb_position_t  *first_x,
			     unsigned पूर्णाङ्क    x_stride,
			     hb_position_t  *first_y,
			     unsigned पूर्णाङ्क    y_stride)

{
  प्रत्यागम font->get_glyph_v_origins (गणनम्,
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
			     hb_codepoint_t  वामः_glyph,
			     hb_codepoint_t  दक्षिणः_glyph)
{
  प्रत्यागम font->get_glyph_h_kerning (वामः_glyph, दक्षिणः_glyph);
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
			     hb_codepoint_t  शिखरम्_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  प्रत्यागम font->get_glyph_v_kerning (शिखरम्_glyph, bottom_glyph);
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
  प्रत्यागम font->get_glyph_extents (glyph, extents);
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
				 unsigned पूर्णाङ्क    point_index,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  प्रत्यागम font->get_glyph_contour_point (glyph, point_index, x, y);
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
			वर्ण           *name,
			unsigned पूर्णाङ्क    size)
{
  प्रत्यागम font->get_glyph_name (glyph, name, size);
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
			     ध्रुव वर्ण     *name,
			     पूर्णाङ्क             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  प्रत्यागम font->get_glyph_from_name (name, len, glyph);
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
शून्य
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, शून्य *draw_data)
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
			    hb_draw_funcs_t *dfuncs, शून्य *draw_data)
{
  प्रत्यागम font->draw_glyph_or_fail (glyph, dfuncs, draw_data);
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
			     hb_paint_funcs_t *pfuncs, शून्य *paint_data,
			     unsigned पूर्णाङ्क palette_index,
			     hb_color_t foreground)
{
  प्रत्यागम font->paint_glyph_or_fail (glyph, pfuncs, paint_data, palette_index, foreground);
}

/* A bit higher-level, and with fallback */

शून्य
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			unsigned पूर्णाङ्क palette,
			hb_color_t foreground)
{
  जइ (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_data,
			   palette, foreground))
    प्रत्यागम;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_data, glyph, एतत्, सच्च, foreground);
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
शून्य
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, शून्य *draw_data)
{
  (शून्य) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_data);
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
शून्य
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, शून्य *paint_data,
                     unsigned पूर्णाङ्क palette_index,
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
शून्य
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
शून्य
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
HB_EXTERN शून्य
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  unsigned पूर्णाङ्क          गणनम्,
					  ध्रुव hb_codepoint_t *first_glyph,
					  unsigned              glyph_stride,
					  hb_position_t        *first_advance,
					  unsigned              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, गणनम्, first_glyph, glyph_stride, first_advance, advance_stride);
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
शून्य
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  प्रत्यागम font->get_glyph_origin_for_direction (glyph, direction, x, y);
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
शून्य
hb_font_add_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  प्रत्यागम font->add_glyph_origin_for_direction (glyph, direction, x, y);
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
शून्य
hb_font_subtract_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  प्रत्यागम font->subtract_glyph_origin_for_direction (glyph, direction, x, y);
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
शून्य
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  first_glyph,
					 hb_codepoint_t  second_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  प्रत्यागम font->get_glyph_kerning_for_direction (first_glyph, second_glyph, direction, x, y);
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
  प्रत्यागम font->get_glyph_extents_for_origin (glyph, direction, extents);
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
					    unsigned पूर्णाङ्क    point_index,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  प्रत्यागम font->get_glyph_contour_point_for_origin (glyph, point_index, direction, x, y);
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
शून्य
hb_font_glyph_to_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 वर्ण           *s,
			 unsigned पूर्णाङ्क    size)
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
			   ध्रुव वर्ण     *s,
			   पूर्णाङ्क             len,
			   hb_codepoint_t *glyph)
{
  प्रत्यागम font->glyph_from_string (s, len, glyph);
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
  मिच्छ, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  सच्च, /* embolden_in_place */
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
  मिच्छ, /* has_nonzero_coords */
  0, /* num_coords */
  nullptr, /* coords */
  nullptr, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


स्थिर hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  जइ (unlikely (!face))
    face = hb_face_get_empty ();

  जइ (!(font = hb_object_create<hb_font_t> ()))
    प्रत्यागम hb_font_get_empty ();

  hb_face_make_immutable (face);
  font->parent = hb_font_get_empty ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_empty ();
  font->data.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_in_place = सच्च;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_index = HB_FONT_NO_VAR_NAMED_INSTANCE;

  प्रत्यागम font;
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
  जइ (likely (face))
  {
    जइ (face->index >> 16)
      hb_font_set_var_named_instance (font, (face->index >> 16) - 1);
    अण्णहा
      hb_font_set_variations (font, nullptr, 0);
  }
#endif

  प्रत्यागम font;
}

स्थिर शून्य
_hb_font_adopt_var_coords (hb_font_t *font,
			   पूर्णाङ्क *coords, /* 2.14 normalized */
			   भिन्न *design_coords,
			   unsigned पूर्णाङ्क coords_length)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->अङ्कः_coords = coords_length;
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
  जइ (unlikely (!parent))
    parent = hb_font_get_empty ();

  hb_font_t *font = _hb_font_create (parent->face);

  जइ (unlikely (hb_object_is_immutable (font)))
    प्रत्यागम font;

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

  unsigned पूर्णाङ्क अङ्कः_coords = parent->अङ्कः_coords;
  जइ (अङ्कः_coords)
  {
    पूर्णाङ्क *coords = (पूर्णाङ्क *) hb_calloc (अङ्कः_coords, परिमाण (parent->coords[0]));
    भिन्न *design_coords = (भिन्न *) hb_calloc (अङ्कः_coords, परिमाण (parent->design_coords[0]));
    जइ (likely (coords && design_coords))
    {
      hb_memcpy (coords, parent->coords, अङ्कः_coords * परिमाण (parent->coords[0]));
      hb_memcpy (design_coords, parent->design_coords, अङ्कः_coords * परिमाण (parent->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, अङ्कः_coords);
    }
    अण्णहा
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->changed ();
  font->serial_coords = font->serial;

  प्रत्यागम font;
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
  प्रत्यागम const_cast<hb_font_t *> (&Null (hb_font_t));
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
  प्रत्यागम hb_object_reference (font);
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
शून्य
hb_font_destroy (hb_font_t *font)
{
  जइ (!hb_object_destroy (font)) प्रत्यागम;

  font->data.fini ();

  जइ (font->destroy)
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
		       hb_user_data_कुञ्चिका_t *key,
		       शून्य *              data,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  जइ (!hb_object_is_immutable (font))
    font->changed ();

  प्रत्यागम hb_object_set_user_data (font, key, data, destroy, replace);
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
शून्य *
hb_font_get_user_data (ध्रुव hb_font_t    *font,
		       hb_user_data_कुञ्चिका_t *key)
{
  प्रत्यागम hb_object_get_user_data (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
शून्य
hb_font_make_immutable (hb_font_t *font)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->parent)
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
  प्रत्यागम hb_object_is_immutable (font);
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
unsigned पूर्णाङ्क
hb_font_get_serial (hb_font_t *font)
{
  प्रत्यागम font->serial.get_acquire ();
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
शून्य
hb_font_changed (hb_font_t *font)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

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
शून्य
hb_font_set_parent (hb_font_t *font,
		    hb_font_t *parent)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (parent == font->parent)
    प्रत्यागम;

  जइ (!parent)
    parent = hb_font_get_empty ();

  प्रति (hb_font_t *p = parent; p && p != hb_font_get_empty(); p = p->parent)
    जइ (p == font)
      प्रत्यागम; /* Would create a cycle - reject */

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
  प्रत्यागम font->parent;
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
शून्य
hb_font_set_face (hb_font_t *font,
		  hb_face_t *face)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (face == font->face)
    प्रत्यागम;

  जइ (unlikely (!face))
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
  प्रत्यागम font->face;
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
शून्य
hb_font_set_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   शून्य              *font_data,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  जइ (hb_object_is_immutable (font))
  {
    जइ (destroy)
      destroy (font_data);
    प्रत्यागम;
  }

  जइ (font->destroy)
    font->destroy (font->user_data);

  जइ (!klass)
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
शून्य
hb_font_set_funcs_data (hb_font_t         *font,
		        शून्य              *font_data,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  जइ (hb_object_is_immutable (font))
  {
    जइ (destroy)
      destroy (font_data);
    प्रत्यागम;
  }

  जइ (font->destroy)
    font->destroy (font->user_data);

  font->user_data = font_data;
  font->destroy = destroy;

  font->changed ();
}

स्थिर ध्रुव संरचना supported_font_funcs_t {
	वर्ण name[16];
	शून्य (*func) (hb_font_t *);
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

स्थिर ध्रुव वर्ण *get_default_funcs_name ()
{
  स्थिर hb_atomic_t<ध्रुव वर्ण *> static_funcs_name;
  ध्रुव वर्ण *name = static_funcs_name.get_acquire ();
  जइ (!name)
  {
    name = getenv ("HB_FONT_FUNCS");
    जइ (!name)
      name = "";
    जइ (!static_funcs_name.cmpexch (nullptr, name))
      name = static_funcs_name.get_acquire ();
  }
  प्रत्यागम name;
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
			 ध्रुव वर्ण *name)
{
  जइ (unlikely (hb_object_is_immutable (font)))
    प्रत्यागम मिच्छ;

  तार्किक retry = मिच्छ;

  जइ (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = सच्च;
  }
  जइ (name && !*name) name = nullptr;

retry:
  प्रति (unsigned i = 0; i < ARRAY_LENGTH (supported_font_funcs); i++)
    जइ (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      जइ (name || font->klass != hb_font_funcs_get_empty ())
	प्रत्यागम सच्च;
    }

  जइ (retry)
  {
    retry = मिच्छ;
    name = nullptr;
    गच्छ retry;
  }

  प्रत्यागम मिच्छ;
}

स्थिर अन्तःस्थ शून्य free_static_font_funcs_list ();

स्थिर ध्रुव वर्ण * ध्रुव nil_font_funcs_list[] = {nullptr};

स्थिर संरचना hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<ध्रुव वर्ण *,
								  hb_font_funcs_list_lazy_loader_t>
{
  स्थिर ध्रुव वर्ण ** create ()
  {
    ध्रुव वर्ण **font_funcs_list = (ध्रुव वर्ण **) hb_calloc (1 + ARRAY_LENGTH (supported_font_funcs), परिमाण (ध्रुव वर्ण *));
    जइ (unlikely (!font_funcs_list))
      प्रत्यागम nullptr;

    unsigned i;
    प्रति (i = 0; i < ARRAY_LENGTH (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = nullptr;

    hb_atexit (free_static_font_funcs_list);

    प्रत्यागम font_funcs_list;
  }
  स्थिर शून्य destroy (ध्रुव वर्ण **l)
  { hb_free (l); }
  स्थिर ध्रुव वर्ण * ध्रुव * get_null ()
  { प्रत्यागम nil_font_funcs_list; }
} static_font_funcs_list;

स्थिर अन्तःस्थ
शून्य free_static_font_funcs_list ()
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
ध्रुव वर्ण **
hb_font_list_funcs ()
{
  प्रत्यागम static_font_funcs_list.get_unconst ();
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
शून्य
hb_font_set_scale (hb_font_t *font,
		   पूर्णाङ्क        x_scale,
		   पूर्णाङ्क        y_scale)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->x_scale == x_scale && font->y_scale == y_scale)
    प्रत्यागम;

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
शून्य
hb_font_get_scale (hb_font_t *font,
		   पूर्णाङ्क       *x_scale,
		   पूर्णाङ्क       *y_scale)
{
  जइ (x_scale) *x_scale = font->x_scale;
  जइ (y_scale) *y_scale = font->y_scale;
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
शून्य
hb_font_set_ppem (hb_font_t    *font,
		  unsigned पूर्णाङ्क  x_ppem,
		  unsigned पूर्णाङ्क  y_ppem)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    प्रत्यागम;

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
शून्य
hb_font_get_ppem (hb_font_t    *font,
		  unsigned पूर्णाङ्क *x_ppem,
		  unsigned पूर्णाङ्क *y_ppem)
{
  जइ (x_ppem) *x_ppem = font->x_ppem;
  जइ (y_ppem) *y_ppem = font->y_ppem;
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
शून्य
hb_font_set_ptem (hb_font_t *font,
		  भिन्न      ptem)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->ptem == ptem)
    प्रत्यागम;

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
भिन्न
hb_font_get_ptem (hb_font_t *font)
{
  प्रत्यागम font->ptem;
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
  प्रत्यागम font->is_synthetic;
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
शून्य
hb_font_set_synthetic_bold (hb_font_t *font,
			    भिन्न x_embolden,
			    भिन्न y_embolden,
			    hb_bool_t in_place)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_in_place == (तार्किक) in_place)
    प्रत्यागम;

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
शून्य
hb_font_get_synthetic_bold (hb_font_t *font,
			    भिन्न *x_embolden,
			    भिन्न *y_embolden,
			    hb_bool_t *in_place)
{
  जइ (x_embolden) *x_embolden = font->x_embolden;
  जइ (y_embolden) *y_embolden = font->y_embolden;
  जइ (in_place) *in_place = font->embolden_in_place;
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
HB_EXTERN शून्य
hb_font_set_synthetic_slant (hb_font_t *font, भिन्न slant)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->slant == slant)
    प्रत्यागम;

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
HB_EXTERN भिन्न
hb_font_get_synthetic_slant (hb_font_t *font)
{
  प्रत्यागम font->slant;
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
शून्य
hb_font_set_variations (hb_font_t            *font,
			ध्रुव hb_variation_t *variations,
			unsigned पूर्णाङ्क          variations_length)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  ध्रुव OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ध्रुव unsigned coords_length = axes.length;

  पूर्णाङ्क *normalized = coords_length ? (पूर्णाङ्क *) hb_calloc (coords_length, परिमाण (पूर्णाङ्क)) : nullptr;
  भिन्न *design_coords = coords_length ? (भिन्न *) hb_calloc (coords_length, परिमाण (भिन्न)) : nullptr;

  जइ (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    प्रत्यागम;
  }

  /* Initialize design coords. */
  प्रति (unsigned पूर्णाङ्क i = 0; i < coords_length; i++)
    design_coords[i] = axes[i].get_default ();
  जइ (font->instance_index != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    unsigned गणनम् = coords_length;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_index,
						&गणनम्, design_coords);
  }

  प्रति (unsigned पूर्णाङ्क i = 0; i < variations_length; i++)
  {
    ध्रुव auto tag = variations[i].tag;
    ध्रुव auto v = variations[i].value;
    प्रति (unsigned axis_index = 0; axis_index < coords_length; axis_index++)
      जइ (axes[axis_index].axisTag == tag)
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
शून्य
hb_font_set_variation (hb_font_t *font,
		       hb_tag_t tag,
		       भिन्न    value)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  // TODO Share some of this code with set_variations()

  ध्रुव OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ध्रुव unsigned coords_length = axes.length;

  पूर्णाङ्क *normalized = coords_length ? (पूर्णाङ्क *) hb_calloc (coords_length, परिमाण (पूर्णाङ्क)) : nullptr;
  भिन्न *design_coords = coords_length ? (भिन्न *) hb_calloc (coords_length, परिमाण (भिन्न)) : nullptr;

  जइ (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    प्रत्यागम;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  जइ (font->design_coords && font->अङ्कः_coords == coords_length)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < coords_length; i++)
      design_coords[i] = font->design_coords[i];
  }
  अण्णहा
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < coords_length; i++)
      design_coords[i] = axes[i].get_default ();
    जइ (font->instance_index != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      unsigned गणनम् = coords_length;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_index,
						  &गणनम्, design_coords);
    }
  }

  प्रति (unsigned axis_index = 0; axis_index < coords_length; axis_index++)
    जइ (axes[axis_index].axisTag == tag)
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
शून्य
hb_font_set_var_coords_design (hb_font_t    *font,
			       ध्रुव भिन्न  *coords,
			       unsigned पूर्णाङ्क  निवेशः_coords_length)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  ध्रुव OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  ध्रुव unsigned coords_length = axes.length;

  निवेशः_coords_length = hb_min (निवेशः_coords_length, coords_length);
  पूर्णाङ्क *normalized = coords_length ? (पूर्णाङ्क *) hb_calloc (coords_length, परिमाण (पूर्णाङ्क)) : nullptr;
  भिन्न *design_coords = coords_length ? (भिन्न *) hb_calloc (coords_length, परिमाण (भिन्न)) : nullptr;

  जइ (unlikely (coords_length && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    प्रत्यागम;
  }

  जइ (निवेशः_coords_length)
    hb_memcpy (design_coords, coords, निवेशः_coords_length * परिमाण (font->design_coords[0]));
  // Fill in the rest with default values
  प्रति (unsigned पूर्णाङ्क i = निवेशः_coords_length; i < coords_length; i++)
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
शून्य
hb_font_set_var_named_instance (hb_font_t *font,
				unsigned पूर्णाङ्क instance_index)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  जइ (font->instance_index == instance_index)
    प्रत्यागम;

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
unsigned पूर्णाङ्क
hb_font_get_var_named_instance (hb_font_t *font)
{
  प्रत्यागम font->instance_index;
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
शून्य
hb_font_set_var_coords_normalized (hb_font_t    *font,
				   ध्रुव पूर्णाङ्क    *coords, /* 2.14 normalized */
				   unsigned पूर्णाङ्क  निवेशः_coords_length)
{
  जइ (hb_object_is_immutable (font))
    प्रत्यागम;

  ध्रुव OT::fvar &fvar = *font->face->table.fvar;
  auto axes = fvar.get_axes ();
  unsigned coords_length = axes.length;

  निवेशः_coords_length = hb_min (निवेशः_coords_length, coords_length);
  पूर्णाङ्क *copy = coords_length ? (पूर्णाङ्क *) hb_calloc (coords_length, परिमाण (coords[0])) : nullptr;
  भिन्न *design_coords = coords_length ? (भिन्न *) hb_calloc (coords_length, परिमाण (design_coords[0])) : nullptr;

  जइ (unlikely (coords_length && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    प्रत्यागम;
  }

  जइ (निवेशः_coords_length)
    hb_memcpy (copy, coords, निवेशः_coords_length * परिमाण (coords[0]));

  प्रति (unsigned पूर्णाङ्क i = 0; i < coords_length; ++i)
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
ध्रुव पूर्णाङ्क *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   unsigned पूर्णाङ्क *length)
{
  जइ (length)
    *length = font->अङ्कः_coords;

  प्रत्यागम font->coords;
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
ध्रुव भिन्न *
hb_font_get_var_coords_design (hb_font_t *font,
			       unsigned पूर्णाङ्क *length)
{
  जइ (length)
    *length = font->अङ्कः_coords;

  प्रत्यागम font->design_coords;
}
#endif

#ifndef HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

संरचना hb_trampoline_closure_t
{
  शून्य *user_data;
  hb_destroy_func_t destroy;
  unsigned पूर्णाङ्क ref_गणनम्_2;
};

template <typename FuncType>
संरचना hb_trampoline_t
{
  hb_trampoline_closure_t closure; /* Must be first. */
  FuncType func;
};

template <typename FuncType>
स्थिर hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   शून्य              *user_data,
		   hb_destroy_func_t  destroy)
{
  प्रकारनिर्देश hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, परिमाण (trampoline_t));

  जइ (unlikely (!trampoline))
    प्रत्यागम nullptr;

  trampoline->closure.user_data = user_data;
  trampoline->closure.destroy = destroy;
  trampoline->closure.ref_गणनम्_2 = 1;
  trampoline->func = func;

  प्रत्यागम trampoline;
}

स्थिर शून्य
trampoline_reference (hb_trampoline_closure_t *closure)
{
  closure->ref_गणनम्_2++;
}

स्थिर शून्य
trampoline_destroy (शून्य *user_data)
{
  hb_trampoline_closure_t *closure = (hb_trampoline_closure_t *) user_data;

  जइ (--closure->ref_गणनम्_2)
    प्रत्यागम;

  जइ (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}

प्रकारनिर्देश hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

स्थिर hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      शून्य           *font_data,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      शून्य           *user_data)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_data;
  प्रत्यागम trampoline->func (font, font_data, unicode, 0, glyph, trampoline->closure.user_data);
}

स्थिर hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					शून्य           *font_data,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					शून्य           *user_data)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_data;
  प्रत्यागम trampoline->func (font, font_data, unicode, variation_selector, glyph, trampoline->closure.user_data);
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
शून्य
hb_font_funcs_set_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      शून्य                     *user_data,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  जइ (hb_object_is_immutable (ffuncs))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_data, destroy);
  जइ (unlikely (!trampoline))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
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

संरचना hb_draw_glyph_closure_t
{
  hb_font_draw_glyph_func_t func;
  शून्य *user_data;
  hb_destroy_func_t destroy;
};
स्थिर hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       शून्य            *font_data,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       शून्य            *draw_data,
			       शून्य            *user_data)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_data;
  closure->func (font, font_data, glyph, draw_funcs, draw_data, closure->user_data);
  प्रत्यागम सच्च;
}
स्थिर शून्य
hb_font_draw_glyph_closure_destroy (शून्य *user_data)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_data;

  जइ (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}
स्थिर शून्य
_hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    शून्य                      *user_data,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  जइ (hb_object_is_immutable (ffuncs))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
  }
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) hb_calloc (1, परिमाण (hb_draw_glyph_closure_t));
  जइ (unlikely (!closure))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
  }
  closure->func = func;
  closure->user_data = user_data;
  closure->destroy = destroy;

  hb_font_funcs_set_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     closure,
					     hb_font_draw_glyph_closure_destroy);
}
शून्य
hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   शून्य                      *user_data,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_data, destroy);
}
शून्य
hb_font_funcs_set_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   शून्य                           *user_data,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_data, destroy);
}

संरचना hb_paint_glyph_closure_t
{
  hb_font_paint_glyph_func_t func;
  शून्य *user_data;
  hb_destroy_func_t destroy;
};
स्थिर hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				शून्य *font_data,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				शून्य *paint_data,
				unsigned पूर्णाङ्क palette,
				hb_color_t foreground,
				शून्य *user_data)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_data;
  closure->func (font, font_data, glyph, paint_funcs, paint_data, palette, foreground, closure->user_data);
  प्रत्यागम सच्च;
}
स्थिर शून्य
hb_font_paint_glyph_closure_destroy (शून्य *user_data)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_data;

  जइ (closure->destroy)
    closure->destroy (closure->user_data);
  hb_free (closure);
}
शून्य
hb_font_funcs_set_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    शून्य                      *user_data,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  जइ (hb_object_is_immutable (ffuncs))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
  }
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) hb_calloc (1, परिमाण (hb_paint_glyph_closure_t));
  जइ (unlikely (!closure))
  {
    जइ (destroy)
      destroy (user_data);
    प्रत्यागम;
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
