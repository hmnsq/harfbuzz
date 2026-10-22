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

#समावेश "hb.hh"

#समावेश "hb-font.hh"
#समावेश "hb-draw.hh"
#समावेश "hb-paint.hh"
#समावेश "hb-machinery.hh"

#समावेश "hb-ot.h"

#समावेश "hb-ot-var-avar-table.hh"
#समावेश "hb-ot-var-fvar-table.hh"

#अगर_अघोषित HB_NO_OT_FONT
#समावेश "hb-ot.h"
#पूर्ण_अगर
#अगर_घोषित HAVE_FREETYPE
#समावेश "hb-ft.h"
#पूर्ण_अगर
#अगर_घोषित HAVE_FONTATIONS
#समावेश "hb-fontations.h"
#पूर्ण_अगर
#अगर_घोषित HAVE_CORETEXT
#समावेश "hb-coretext.h"
#पूर्ण_अगर
#अगर_घोषित HAVE_DIRECTWRITE
#समावेश "hb-directwrite.h"
#पूर्ण_अगर


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

अटल hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_UNUSED,
				व्योम              *font_तथ्य HB_UNUSED,
				hb_font_extents_t *extents,
				व्योम              *user_तथ्य HB_UNUSED)
{
  hb_memset (extents, 0, माप (*extents));
  वापस false;
}

अटल hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    व्योम              *font_तथ्य HB_UNUSED,
				    hb_font_extents_t *extents,
				    व्योम              *user_तथ्य HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_h_extents (extents, false);
  अगर (ret) {
    extents->ascender = font->parent_scale_y_distance (extents->ascender);
    extents->descender = font->parent_scale_y_distance (extents->descender);
    extents->पंक्ति_gap = font->parent_scale_y_distance (extents->पंक्ति_gap);
  }
  वापस ret;
}

अटल hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_UNUSED,
				व्योम              *font_तथ्य HB_UNUSED,
				hb_font_extents_t *extents,
				व्योम              *user_तथ्य HB_UNUSED)
{
  hb_memset (extents, 0, माप (*extents));
  वापस false;
}

अटल hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    व्योम              *font_तथ्य HB_UNUSED,
				    hb_font_extents_t *extents,
				    व्योम              *user_तथ्य HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_font_v_extents (extents, false);
  अगर (ret) {
    extents->ascender = font->parent_scale_x_distance (extents->ascender);
    extents->descender = font->parent_scale_x_distance (extents->descender);
    extents->पंक्ति_gap = font->parent_scale_x_distance (extents->पंक्ति_gap);
  }
  वापस ret;
}

अटल hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_UNUSED,
			       व्योम           *font_तथ्य HB_UNUSED,
			       hb_codepoint_t  unicode HB_UNUSED,
			       hb_codepoint_t *glyph,
			       व्योम           *user_तथ्य HB_UNUSED)
{
  *glyph = 0;
  वापस false;
}

अटल hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   व्योम           *font_तथ्य HB_UNUSED,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (font->has_nominal_glyphs_func_set ())
  {
    वापस font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  वापस font->parent->get_nominal_glyph (unicode, glyph);
}

#घोषणा hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

अटल अचिन्हित पूर्णांक
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    व्योम                 *font_तथ्य HB_UNUSED,
				    अचिन्हित पूर्णांक          गिनती,
				    स्थिर hb_codepoint_t *first_unicode,
				    अचिन्हित पूर्णांक          unicode_stride,
				    hb_codepoint_t       *first_glyph,
				    अचिन्हित पूर्णांक          glyph_stride,
				    व्योम                 *user_तथ्य HB_UNUSED)
{
  अगर (font->has_nominal_glyph_func_set ())
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (!font->get_nominal_glyph (*first_unicode, first_glyph))
	वापस i;

      first_unicode = &StructAtOffsetUnaligned<hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    वापस गिनती;
  }

  वापस font->parent->get_nominal_glyphs (गिनती,
					   first_unicode, unicode_stride,
					   first_glyph, glyph_stride);
}

अटल hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_UNUSED,
				 व्योम           *font_तथ्य HB_UNUSED,
				 hb_codepoint_t  unicode HB_UNUSED,
				 hb_codepoint_t  variation_selector HB_UNUSED,
				 hb_codepoint_t *glyph,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  *glyph = 0;
  वापस false;
}

अटल hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->parent->get_variation_glyph (unicode, variation_selector, glyph);
}


अटल hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 व्योम           *font_तथ्य HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->x_scale;
}

अटल hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_h_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, false);
    वापस ret;
  }
  वापस font->parent_scale_x_distance (font->parent->get_glyph_h_advance (glyph, false));
}

अटल hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 व्योम           *font_तथ्य HB_UNUSED,
				 hb_codepoint_t  glyph HB_UNUSED,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  वापस -font->y_scale;
}

अटल hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_v_advances_func_set ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, false);
    वापस ret;
  }
  वापस font->parent_scale_y_distance (font->parent->get_glyph_v_advance (glyph, false));
}

#घोषणा hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

अटल व्योम
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      व्योम*                 font_तथ्य HB_UNUSED,
				      अचिन्हित पूर्णांक          गिनती,
				      स्थिर hb_codepoint_t *first_glyph,
				      अचिन्हित पूर्णांक          glyph_stride,
				      hb_position_t        *first_advance,
				      अचिन्हित पूर्णांक          advance_stride,
				      व्योम                 *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_h_advance_func_set ())
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = font->get_glyph_h_advance (*first_glyph, false);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

  font->parent->get_glyph_h_advances (गिनती,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      false);
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    *first_advance = font->parent_scale_x_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

#घोषणा hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
अटल व्योम
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      व्योम*                 font_तथ्य HB_UNUSED,
				      अचिन्हित पूर्णांक          गिनती,
				      स्थिर hb_codepoint_t *first_glyph,
				      अचिन्हित पूर्णांक          glyph_stride,
				      hb_position_t        *first_advance,
				      अचिन्हित पूर्णांक          advance_stride,
				      व्योम                 *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_v_advance_func_set ())
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      *first_advance = font->get_glyph_v_advance (*first_glyph, false);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
    }
    वापस;
  }

  font->parent->get_glyph_v_advances (गिनती,
				      first_glyph, glyph_stride,
				      first_advance, advance_stride,
				      false);
  क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
  {
    *first_advance = font->parent_scale_y_distance (*first_advance);
    first_advance = &StructAtOffsetUnaligned<hb_position_t> (first_advance, advance_stride);
  }
}

अटल hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_UNUSED,
				व्योम           *font_तथ्य HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				व्योम           *user_तथ्य HB_UNUSED)
{
  *x = *y = 0;
  वापस true;
}

अटल hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    व्योम           *font_तथ्य HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_h_origins_func_set ())
  {
    वापस font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, false);
  }
  hb_bool_t ret = font->parent->get_glyph_h_origin (glyph, x, y);
  अगर (ret)
    font->parent_scale_position (x, y);
  वापस ret;
}

अटल hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_UNUSED,
				व्योम           *font_तथ्य HB_UNUSED,
				hb_codepoint_t  glyph HB_UNUSED,
				hb_position_t  *x,
				hb_position_t  *y,
				व्योम           *user_तथ्य HB_UNUSED)
{
  वापस false;
}

अटल hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    व्योम           *font_तथ्य HB_UNUSED,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_v_origins_func_set ())
  {
    वापस font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, false);
  }
  hb_bool_t ret = font->parent->get_glyph_v_origin (glyph, x, y);
  अगर (ret)
    font->parent_scale_position (x, y);
  वापस ret;
}

#घोषणा hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

अटल hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_UNUSED,
				     व्योम *font_तथ्य HB_UNUSED,
				     अचिन्हित पूर्णांक गिनती,
				     स्थिर hb_codepoint_t *first_glyph HB_UNUSED,
				     अचिन्हित glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     अचिन्हित x_stride,
				     hb_position_t *first_y,
				     अचिन्हित y_stride,
				     व्योम *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_h_origin_func_set ())
  {
    hb_bool_t ret = true;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      ret &= font->get_glyph_h_origin (*first_glyph, first_x, first_y, false);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    वापस ret;
  }

  hb_bool_t ret = font->parent->get_glyph_h_origins (गिनती,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  अगर (ret)
  {
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  वापस ret;
}

#घोषणा hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

अटल hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_UNUSED,
				     व्योम *font_तथ्य HB_UNUSED,
				     अचिन्हित पूर्णांक गिनती,
				     स्थिर hb_codepoint_t *first_glyph HB_UNUSED,
				     अचिन्हित glyph_stride HB_UNUSED,
				     hb_position_t *first_x,
				     अचिन्हित x_stride,
				     hb_position_t *first_y,
				     अचिन्हित y_stride,
				     व्योम *user_तथ्य HB_UNUSED)
{
  अगर (font->has_glyph_v_origin_func_set ())
  {
    hb_bool_t ret = true;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      ret &= font->get_glyph_v_origin (*first_glyph, first_x, first_y, false);
      first_glyph = &StructAtOffsetUnaligned<hb_codepoint_t> (first_glyph, glyph_stride);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
    वापस ret;
  }

  hb_bool_t ret = font->parent->get_glyph_v_origins (गिनती,
						     first_glyph, glyph_stride,
						     first_x, x_stride,
						     first_y, y_stride);
  अगर (ret)
  {
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      font->parent_scale_position (first_x, first_y);
      first_x = &StructAtOffsetUnaligned<hb_position_t> (first_x, x_stride);
      first_y = &StructAtOffsetUnaligned<hb_position_t> (first_y, y_stride);
    }
  }
  वापस ret;
}

अटल hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_UNUSED,
				 व्योम           *font_तथ्य HB_UNUSED,
				 hb_codepoint_t  बायाँ_glyph HB_UNUSED,
				 hb_codepoint_t  दायाँ_glyph HB_UNUSED,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  वापस 0;
}

अटल hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  बायाँ_glyph,
				     hb_codepoint_t  दायाँ_glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->parent_scale_x_distance (font->parent->get_glyph_h_kerning (बायाँ_glyph, दायाँ_glyph));
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
अटल hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_UNUSED,
				 व्योम           *font_तथ्य HB_UNUSED,
				 hb_codepoint_t  शीर्ष_glyph HB_UNUSED,
				 hb_codepoint_t  bottom_glyph HB_UNUSED,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  वापस 0;
}

अटल hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  शीर्ष_glyph,
				     hb_codepoint_t  bottom_glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->parent_scale_y_distance (font->parent->get_glyph_v_kerning (शीर्ष_glyph, bottom_glyph));
}
#पूर्ण_अगर

अटल hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_UNUSED,
			       व्योम               *font_तथ्य HB_UNUSED,
			       hb_codepoint_t      glyph HB_UNUSED,
			       hb_glyph_extents_t *extents,
			       व्योम               *user_तथ्य HB_UNUSED)
{
  hb_memset (extents, 0, माप (*extents));
  वापस false;
}

अटल hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   व्योम               *font_तथ्य HB_UNUSED,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   व्योम               *user_तथ्य HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_extents (glyph, extents, false);
  अगर (ret) {
    font->parent_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->parent_scale_distance (&extents->width, &extents->height);
  }
  वापस ret;
}

अटल hb_bool_t
hb_font_get_glyph_contour_point_nil (hb_font_t      *font HB_UNUSED,
				     व्योम           *font_तथ्य HB_UNUSED,
				     hb_codepoint_t  glyph HB_UNUSED,
				     अचिन्हित पूर्णांक    point_सूचकांक HB_UNUSED,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  *x = *y = 0;
  वापस false;
}

अटल hb_bool_t
hb_font_get_glyph_contour_point_default (hb_font_t      *font,
					 व्योम           *font_तथ्य HB_UNUSED,
					 hb_codepoint_t  glyph,
					 अचिन्हित पूर्णांक    point_सूचकांक,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 व्योम           *user_तथ्य HB_UNUSED)
{
  hb_bool_t ret = font->parent->get_glyph_contour_point (glyph, point_सूचकांक, x, y, false);
  अगर (ret)
    font->parent_scale_position (x, y);
  वापस ret;
}

अटल hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_UNUSED,
			    व्योम           *font_तथ्य HB_UNUSED,
			    hb_codepoint_t  glyph HB_UNUSED,
			    अक्षर           *name,
			    अचिन्हित पूर्णांक    आकार,
			    व्योम           *user_तथ्य HB_UNUSED)
{
  अगर (आकार) *name = '\0';
  वापस false;
}

अटल hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				व्योम           *font_तथ्य HB_UNUSED,
				hb_codepoint_t  glyph,
				अक्षर           *name,
				अचिन्हित पूर्णांक    आकार,
				व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->parent->get_glyph_name (glyph, name, आकार);
}

अटल hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_UNUSED,
				 व्योम           *font_तथ्य HB_UNUSED,
				 स्थिर अक्षर     *name HB_UNUSED,
				 पूर्णांक             len HB_UNUSED, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 व्योम           *user_तथ्य HB_UNUSED)
{
  *glyph = 0;
  वापस false;
}

अटल hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     व्योम           *font_तथ्य HB_UNUSED,
				     स्थिर अक्षर     *name,
				     पूर्णांक             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     व्योम           *user_तथ्य HB_UNUSED)
{
  वापस font->parent->get_glyph_from_name (name, len, glyph);
}

अटल hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_UNUSED,
				व्योम            *font_तथ्य HB_UNUSED,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				व्योम            *draw_तथ्य,
				व्योम            *user_तथ्य HB_UNUSED)
{
  वापस false;
}

अटल hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_UNUSED,
				 व्योम *font_तथ्य HB_UNUSED,
				 hb_codepoint_t glyph HB_UNUSED,
				 hb_paint_funcs_t *paint_funcs HB_UNUSED,
				 व्योम *paint_तथ्य HB_UNUSED,
				 अचिन्हित पूर्णांक palette HB_UNUSED,
				 hb_color_t foreground HB_UNUSED,
				 व्योम *user_तथ्य HB_UNUSED)
{
  वापस false;
}

प्रकार काष्ठा hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  व्योम		  *draw_तथ्य;
  भग्न		   x_scale;
  भग्न		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

अटल व्योम
hb_draw_move_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED,
			 व्योम *draw_तथ्य,
			 hb_draw_state_t *st,
			 भग्न to_x, भग्न to_y,
			 व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  भग्न x_scale = adaptor->x_scale;
  भग्न y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_to (adaptor->draw_तथ्य, *st,
				     x_scale * to_x, y_scale * to_y);
}

अटल व्योम
hb_draw_पंक्ति_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			 hb_draw_state_t *st,
			 भग्न to_x, भग्न to_y,
			 व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  भग्न x_scale = adaptor->x_scale;
  भग्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_पंक्ति_to (adaptor->draw_तथ्य, *st,
				     x_scale * to_x, y_scale * to_y);
}

अटल व्योम
hb_draw_quadratic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			      hb_draw_state_t *st,
			      भग्न control_x, भग्न control_y,
			      भग्न to_x, भग्न to_y,
			      व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  भग्न x_scale = adaptor->x_scale;
  भग्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_to (adaptor->draw_तथ्य, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * to_x, y_scale * to_y);
}

अटल व्योम
hb_draw_cubic_to_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			  hb_draw_state_t *st,
			  भग्न control1_x, भग्न control1_y,
			  भग्न control2_x, भग्न control2_y,
			  भग्न to_x, भग्न to_y,
			  व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  भग्न x_scale = adaptor->x_scale;
  भग्न y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_to (adaptor->draw_तथ्य, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * to_x, y_scale * to_y);
}

अटल व्योम
hb_draw_close_path_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			    hb_draw_state_t *st,
			    व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;

  adaptor->draw_funcs->emit_close_path (adaptor->draw_तथ्य, *st);
}

अटल hb_bool_t
hb_draw_set_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			    int64_t budget,
			    व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  वापस adaptor->draw_funcs->set_budget (adaptor->draw_तथ्य, budget);
}

अटल int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
			    व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  वापस adaptor->draw_funcs->get_budget (adaptor->draw_तथ्य);
}

अटल int64_t *
hb_draw_get_budget_remaining_default (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *draw_तथ्य,
				      व्योम *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_तथ्य;
  वापस adaptor->draw_funcs->get_budget_remaining_ptr (adaptor->draw_तथ्य);
}

अटल स्थिर hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  {
#घोषणा HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_DRAW_FUNC_IMPLEMENT
  }
};

अटल hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    व्योम            *font_तथ्य HB_UNUSED,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    व्योम            *draw_तथ्य,
				    व्योम            *user_तथ्य HB_UNUSED)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_तथ्य,
    font->parent->x_scale ? (भग्न) font->x_scale / (भग्न) font->parent->x_scale : 0.f,
    font->parent->y_scale ? (भग्न) font->y_scale / (भग्न) font->parent->y_scale : 0.f
  };

  वापस font->parent->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   false);
}

अटल hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     व्योम *font_तथ्य,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     व्योम *paint_तथ्य,
				     अचिन्हित पूर्णांक palette,
				     hb_color_t foreground,
				     व्योम *user_तथ्य)
{
  paint_funcs->धकेलो_transform (paint_तथ्य,
    font->parent->x_scale ? (भग्न) font->x_scale / (भग्न) font->parent->x_scale : 0, 0,
    0, font->parent->y_scale ? (भग्न) font->y_scale / (भग्न) font->parent->y_scale : 0,
    0, 0);

  bool ret = font->parent->paint_glyph_or_fail (glyph, paint_funcs, paint_तथ्य, palette, foreground);

  paint_funcs->निकालो_transform (paint_तथ्य);

  वापस ret;
}

DEFINE_NULL_INSTANCE (hb_font_funcs_t) =
{
  HB_OBJECT_HEADER_STATIC,

  nullptr,
  nullptr,
  {
    {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_nil,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
    }
  }
};

अटल स्थिर hb_font_funcs_t _hb_font_funcs_default = {
  HB_OBJECT_HEADER_STATIC,

  nullptr,
  nullptr,
  {
    {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_default,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
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

  अगर (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    वापस hb_font_funcs_get_empty ();

  ffuncs->लो = _hb_font_funcs_default.लो;

  वापस ffuncs;
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
  वापस const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
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
  वापस hb_object_reference (ffuncs);
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
व्योम
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  अगर (!hb_object_destroy (ffuncs)) वापस;

  अगर (ffuncs->destroy)
  {
#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) अगर (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_तथ्य ? nullptr : ffuncs->user_तथ्य->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_तथ्य);

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
hb_font_funcs_set_user_तथ्य (hb_font_funcs_t    *ffuncs,
			     hb_user_तथ्य_कुंजी_t *key,
			     व्योम *              तथ्य,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  वापस hb_object_set_user_तथ्य (ffuncs, key, तथ्य, destroy, replace);
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
व्योम *
hb_font_funcs_get_user_तथ्य (स्थिर hb_font_funcs_t *ffuncs,
			     hb_user_तथ्य_कुंजी_t    *key)
{
  वापस hb_object_get_user_तथ्य (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
व्योम
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  अगर (hb_object_is_immutable (ffuncs))
    वापस;

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
  वापस hb_object_is_immutable (ffuncs);
}


अटल bool
_hb_font_funcs_set_preamble (hb_font_funcs_t    *ffuncs,
			     bool                func_is_null,
			     व्योम              **user_तथ्य,
			     hb_destroy_func_t  *destroy)
{
  अगर (hb_object_is_immutable (ffuncs))
  {
    अगर (*destroy)
      (*destroy) (*user_तथ्य);
    वापस false;
  }

  अगर (func_is_null)
  {
    अगर (*destroy)
      (*destroy) (*user_तथ्य);
    *destroy = nullptr;
    *user_तथ्य = nullptr;
  }

  वापस true;
}

अटल bool
hb_font_funcs_set_मध्य (hb_font_funcs_t   *ffuncs,
			   व्योम              *user_तथ्य,
			   hb_destroy_func_t  destroy)
{
  स्वतः destroy_guard = hb_make_scope_guard ([&]() {
    अगर (destroy) destroy (user_तथ्य);
  });

  अगर (user_तथ्य && !ffuncs->user_तथ्य)
  {
    ffuncs->user_तथ्य = (decltype (ffuncs->user_तथ्य)) hb_calloc (1, माप (*ffuncs->user_तथ्य));
    अगर (unlikely (!ffuncs->user_तथ्य))
      वापस false;
  }
  अगर (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, माप (*ffuncs->destroy));
    अगर (unlikely (!ffuncs->destroy))
      वापस false;
  }

  destroy_guard.release ();
  वापस true;
}

#घोषणा HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
व्योम                                                                     \
hb_font_funcs_set_##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 व्योम                        *user_तथ्य, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  अगर (!_hb_font_funcs_set_preamble (ffuncs, !func, &user_तथ्य, &destroy))\
      वापस;                                                            \
									 \
  अगर (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_तथ्य ? nullptr : ffuncs->user_तथ्य->name); \
                                                                         \
  अगर (!hb_font_funcs_set_मध्य (ffuncs, user_तथ्य, destroy))           \
      वापस;                                                            \
									 \
  अगर (func)                                                              \
    ffuncs->लो.f.name = func;                                           \
  अन्यथा                                                                   \
    ffuncs->लो.f.name = hb_font_##get_##name##_default;                   \
									 \
  अगर (ffuncs->user_तथ्य)                                                 \
    ffuncs->user_तथ्य->name = user_तथ्य;                                 \
  अगर (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#अघोषित HB_FONT_FUNC_IMPLEMENT

bool
hb_font_t::has_func_set (अचिन्हित पूर्णांक i)
{
  वापस यह->klass->लो.array[i] != _hb_font_funcs_default.लो.array[i];
}

bool
hb_font_t::has_func (अचिन्हित पूर्णांक i)
{
  वापस has_func_set (i) ||
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
  वापस font->get_font_h_extents (extents);
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
  वापस font->get_font_v_extents (extents);
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
  अगर (unlikely (variation_selector))
    वापस font->get_variation_glyph (unicode, variation_selector, glyph);
  वापस font->get_nominal_glyph (unicode, glyph);
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
  वापस font->get_nominal_glyph (unicode, glyph);
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
अचिन्हित पूर्णांक
hb_font_get_nominal_glyphs (hb_font_t *font,
			    अचिन्हित पूर्णांक गिनती,
			    स्थिर hb_codepoint_t *first_unicode,
			    अचिन्हित पूर्णांक unicode_stride,
			    hb_codepoint_t *first_glyph,
			    अचिन्हित पूर्णांक glyph_stride)
{
  वापस font->get_nominal_glyphs (गिनती,
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
  वापस font->get_variation_glyph (unicode, variation_selector, glyph);
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
  वापस font->get_glyph_h_advance (glyph);
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
  वापस font->get_glyph_v_advance (glyph);
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
व्योम
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      अचिन्हित पूर्णांक          गिनती,
			      स्थिर hb_codepoint_t *first_glyph,
			      अचिन्हित              glyph_stride,
			      hb_position_t        *first_advance,
			      अचिन्हित              advance_stride)
{
  font->get_glyph_h_advances (गिनती, first_glyph, glyph_stride, first_advance, advance_stride);
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
व्योम
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      अचिन्हित पूर्णांक          गिनती,
			      स्थिर hb_codepoint_t *first_glyph,
			      अचिन्हित              glyph_stride,
			      hb_position_t        *first_advance,
			      अचिन्हित              advance_stride)
{
  font->get_glyph_v_advances (गिनती, first_glyph, glyph_stride, first_advance, advance_stride);
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
  वापस font->get_glyph_h_origin (glyph, x, y);
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
  वापस font->get_glyph_v_origin (glyph, x, y);
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
			     अचिन्हित पूर्णांक    गिनती,
			     स्थिर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णांक    glyph_stride,
			     hb_position_t  *first_x,
			     अचिन्हित पूर्णांक    x_stride,
			     hb_position_t  *first_y,
			     अचिन्हित पूर्णांक    y_stride)

{
  वापस font->get_glyph_h_origins (गिनती,
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
			     अचिन्हित पूर्णांक    गिनती,
			     स्थिर hb_codepoint_t *first_glyph,
			     अचिन्हित पूर्णांक    glyph_stride,
			     hb_position_t  *first_x,
			     अचिन्हित पूर्णांक    x_stride,
			     hb_position_t  *first_y,
			     अचिन्हित पूर्णांक    y_stride)

{
  वापस font->get_glyph_v_origins (गिनती,
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
			     hb_codepoint_t  बायाँ_glyph,
			     hb_codepoint_t  दायाँ_glyph)
{
  वापस font->get_glyph_h_kerning (बायाँ_glyph, दायाँ_glyph);
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
			     hb_codepoint_t  शीर्ष_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  वापस font->get_glyph_v_kerning (शीर्ष_glyph, bottom_glyph);
}
#पूर्ण_अगर

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
  वापस font->get_glyph_extents (glyph, extents);
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
				 अचिन्हित पूर्णांक    point_सूचकांक,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  वापस font->get_glyph_contour_point (glyph, point_सूचकांक, x, y);
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
			अक्षर           *name,
			अचिन्हित पूर्णांक    आकार)
{
  वापस font->get_glyph_name (glyph, name, आकार);
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
			     स्थिर अक्षर     *name,
			     पूर्णांक             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  वापस font->get_glyph_from_name (name, len, glyph);
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
व्योम
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_तथ्य);
}
#पूर्ण_अगर

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
			    hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य)
{
  वापस font->draw_glyph_or_fail (glyph, dfuncs, draw_तथ्य);
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
			     hb_paint_funcs_t *pfuncs, व्योम *paint_तथ्य,
			     अचिन्हित पूर्णांक palette_सूचकांक,
			     hb_color_t foreground)
{
  वापस font->paint_glyph_or_fail (glyph, pfuncs, paint_तथ्य, palette_सूचकांक, foreground);
}

/* A bit higher-level, and with fallback */

व्योम
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
			अचिन्हित पूर्णांक palette,
			hb_color_t foreground)
{
  अगर (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_तथ्य,
			   palette, foreground))
    वापस;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_तथ्य, glyph, यह, true, foreground);
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
व्योम
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, व्योम *draw_तथ्य)
{
  (व्योम) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_तथ्य);
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
व्योम
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, व्योम *paint_तथ्य,
                     अचिन्हित पूर्णांक palette_सूचकांक,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_तथ्य, palette_सूचकांक, foreground);
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
व्योम
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
व्योम
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
HB_EXTERN व्योम
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  अचिन्हित पूर्णांक          गिनती,
					  स्थिर hb_codepoint_t *first_glyph,
					  अचिन्हित              glyph_stride,
					  hb_position_t        *first_advance,
					  अचिन्हित              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, गिनती, first_glyph, glyph_stride, first_advance, advance_stride);
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
व्योम
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  वापस font->get_glyph_origin_for_direction (glyph, direction, x, y);
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
व्योम
hb_font_add_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  वापस font->add_glyph_origin_for_direction (glyph, direction, x, y);
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
व्योम
hb_font_subtract_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  वापस font->subtract_glyph_origin_for_direction (glyph, direction, x, y);
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
व्योम
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  first_glyph,
					 hb_codepoint_t  second_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  वापस font->get_glyph_kerning_for_direction (first_glyph, second_glyph, direction, x, y);
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
  वापस font->get_glyph_extents_for_origin (glyph, direction, extents);
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
					    अचिन्हित पूर्णांक    point_सूचकांक,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  वापस font->get_glyph_contour_point_for_origin (glyph, point_सूचकांक, direction, x, y);
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
व्योम
hb_font_glyph_to_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 अक्षर           *s,
			 अचिन्हित पूर्णांक    आकार)
{
  font->glyph_to_string (glyph, s, आकार);
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
			   स्थिर अक्षर     *s,
			   पूर्णांक             len,
			   hb_codepoint_t *glyph)
{
  वापस font->glyph_from_string (s, len, glyph);
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
  false, /* is_synthetic */
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
  false, /* has_nonzero_coords */
  0, /* num_coords */
  nullptr, /* coords */
  nullptr, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


अटल hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  अगर (unlikely (!face))
    face = hb_face_get_empty ();

  अगर (!(font = hb_object_create<hb_font_t> ()))
    वापस hb_font_get_empty ();

  hb_face_make_immutable (face);
  font->parent = hb_font_get_empty ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_empty ();
  font->तथ्य.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_in_place = true;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_सूचकांक = HB_FONT_NO_VAR_NAMED_INSTANCE;

  वापस font;
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

#अगर_अघोषित HB_NO_VAR
  // Initialize variations.
  अगर (likely (face))
  {
    अगर (face->सूचकांक_2 >> 16)
      hb_font_set_var_named_instance (font, (face->सूचकांक_2 >> 16) - 1);
    अन्यथा
      hb_font_set_variations (font, nullptr, 0);
  }
#पूर्ण_अगर

  वापस font;
}

अटल व्योम
_hb_font_adopt_var_coords (hb_font_t *font,
			   पूर्णांक *coords, /* 2.14 normalized */
			   भग्न *design_coords,
			   अचिन्हित पूर्णांक coords_लंबाई)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->अंक_coords = coords_लंबाई;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_लंबाई));

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
  अगर (unlikely (!parent))
    parent = hb_font_get_empty ();

  hb_font_t *font = _hb_font_create (parent->face);

  अगर (unlikely (hb_object_is_immutable (font)))
    वापस font;

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

  अचिन्हित पूर्णांक अंक_coords = parent->अंक_coords;
  अगर (अंक_coords)
  {
    पूर्णांक *coords = (पूर्णांक *) hb_calloc (अंक_coords, माप (parent->coords[0]));
    भग्न *design_coords = (भग्न *) hb_calloc (अंक_coords, माप (parent->design_coords[0]));
    अगर (likely (coords && design_coords))
    {
      hb_memcpy (coords, parent->coords, अंक_coords * माप (parent->coords[0]));
      hb_memcpy (design_coords, parent->design_coords, अंक_coords * माप (parent->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, अंक_coords);
    }
    अन्यथा
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->changed ();
  font->serial_coords = font->serial;

  वापस font;
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
  वापस const_cast<hb_font_t *> (&Null (hb_font_t));
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
  वापस hb_object_reference (font);
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
व्योम
hb_font_destroy (hb_font_t *font)
{
  अगर (!hb_object_destroy (font)) वापस;

  font->तथ्य.fini ();

  अगर (font->destroy)
    font->destroy (font->user_तथ्य);

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
hb_font_set_user_तथ्य (hb_font_t          *font,
		       hb_user_तथ्य_कुंजी_t *key,
		       व्योम *              तथ्य,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  अगर (!hb_object_is_immutable (font))
    font->changed ();

  वापस hb_object_set_user_तथ्य (font, key, तथ्य, destroy, replace);
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
व्योम *
hb_font_get_user_तथ्य (स्थिर hb_font_t    *font,
		       hb_user_तथ्य_कुंजी_t *key)
{
  वापस hb_object_get_user_तथ्य (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
व्योम
hb_font_make_immutable (hb_font_t *font)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->parent)
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
  वापस hb_object_is_immutable (font);
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
अचिन्हित पूर्णांक
hb_font_get_serial (hb_font_t *font)
{
  वापस font->serial.get_acquire ();
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
व्योम
hb_font_changed (hb_font_t *font)
{
  अगर (hb_object_is_immutable (font))
    वापस;

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
व्योम
hb_font_set_parent (hb_font_t *font,
		    hb_font_t *parent)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (parent == font->parent)
    वापस;

  अगर (!parent)
    parent = hb_font_get_empty ();

  क्रम (hb_font_t *p = parent; p && p != hb_font_get_empty(); p = p->parent)
    अगर (p == font)
      वापस; /* Would create a cycle - reject */

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
  वापस font->parent;
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
व्योम
hb_font_set_face (hb_font_t *font,
		  hb_face_t *face)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (face == font->face)
    वापस;

  अगर (unlikely (!face))
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
#पूर्ण_अगर

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
  वापस font->face;
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
व्योम
hb_font_set_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   व्योम              *font_तथ्य,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  अगर (hb_object_is_immutable (font))
  {
    अगर (destroy)
      destroy (font_तथ्य);
    वापस;
  }

  अगर (font->destroy)
    font->destroy (font->user_तथ्य);

  अगर (!klass)
    klass = hb_font_funcs_get_empty ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_तथ्य = font_तथ्य;
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
व्योम
hb_font_set_funcs_तथ्य (hb_font_t         *font,
		        व्योम              *font_तथ्य,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  अगर (hb_object_is_immutable (font))
  {
    अगर (destroy)
      destroy (font_तथ्य);
    वापस;
  }

  अगर (font->destroy)
    font->destroy (font->user_तथ्य);

  font->user_तथ्य = font_तथ्य;
  font->destroy = destroy;

  font->changed ();
}

अटल स्थिर काष्ठा supported_font_funcs_t {
	अक्षर name[16];
	व्योम (*func) (hb_font_t *);
} supported_font_funcs[] =
{
#अगर_अघोषित HB_NO_OT_FONT
  {"ot",	hb_ot_font_set_funcs},
#पूर्ण_अगर
#अगर_घोषित HAVE_FREETYPE
  {"ft",	hb_ft_font_set_funcs},
#पूर्ण_अगर
#अगर_घोषित HAVE_FONTATIONS
  {"fontations",hb_fontations_font_set_funcs},
#पूर्ण_अगर
#अगर_घोषित HAVE_CORETEXT
  {"coretext",	hb_coretext_font_set_funcs},
#पूर्ण_अगर
#अगर_घोषित HAVE_DIRECTWRITE
  {"directwrite",hb_directwrite_font_set_funcs},
#पूर्ण_अगर
};

अटल स्थिर अक्षर *get_default_funcs_name ()
{
  अटल hb_atomic_t<स्थिर अक्षर *> static_funcs_name;
  स्थिर अक्षर *name = static_funcs_name.get_acquire ();
  अगर (!name)
  {
    name = दो_पर्या ("HB_FONT_FUNCS");
    अगर (!name)
      name = "";
    अगर (!static_funcs_name.cmpexch (nullptr, name))
      name = static_funcs_name.get_acquire ();
  }
  वापस name;
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
			 स्थिर अक्षर *name)
{
  अगर (unlikely (hb_object_is_immutable (font)))
    वापस false;

  bool retry = false;

  अगर (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = true;
  }
  अगर (name && !*name) name = nullptr;

retry:
  क्रम (अचिन्हित i = 0; i < ARRAY_लंबाई (supported_font_funcs); i++)
    अगर (!name || म_भेद (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      अगर (name || font->klass != hb_font_funcs_get_empty ())
	वापस true;
    }

  अगर (retry)
  {
    retry = false;
    name = nullptr;
    जाओ retry;
  }

  वापस false;
}

अटल अंतरभूत व्योम free_static_font_funcs_list ();

अटल स्थिर अक्षर * स्थिर nil_font_funcs_list[] = {nullptr};

अटल काष्ठा hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<स्थिर अक्षर *,
								  hb_font_funcs_list_lazy_loader_t>
{
  अटल स्थिर अक्षर ** create ()
  {
    स्थिर अक्षर **font_funcs_list = (स्थिर अक्षर **) hb_calloc (1 + ARRAY_लंबाई (supported_font_funcs), माप (स्थिर अक्षर *));
    अगर (unlikely (!font_funcs_list))
      वापस nullptr;

    अचिन्हित i;
    क्रम (i = 0; i < ARRAY_लंबाई (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = nullptr;

    hb_atexit (free_static_font_funcs_list);

    वापस font_funcs_list;
  }
  अटल व्योम destroy (स्थिर अक्षर **l)
  { hb_free (l); }
  अटल स्थिर अक्षर * स्थिर * get_null ()
  { वापस nil_font_funcs_list; }
} static_font_funcs_list;

अटल अंतरभूत
व्योम free_static_font_funcs_list ()
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
स्थिर अक्षर **
hb_font_list_funcs ()
{
  वापस static_font_funcs_list.get_unconst ();
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
व्योम
hb_font_set_scale (hb_font_t *font,
		   पूर्णांक        x_scale,
		   पूर्णांक        y_scale)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->x_scale == x_scale && font->y_scale == y_scale)
    वापस;

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
व्योम
hb_font_get_scale (hb_font_t *font,
		   पूर्णांक       *x_scale,
		   पूर्णांक       *y_scale)
{
  अगर (x_scale) *x_scale = font->x_scale;
  अगर (y_scale) *y_scale = font->y_scale;
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
व्योम
hb_font_set_ppem (hb_font_t    *font,
		  अचिन्हित पूर्णांक  x_ppem,
		  अचिन्हित पूर्णांक  y_ppem)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    वापस;

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
व्योम
hb_font_get_ppem (hb_font_t    *font,
		  अचिन्हित पूर्णांक *x_ppem,
		  अचिन्हित पूर्णांक *y_ppem)
{
  अगर (x_ppem) *x_ppem = font->x_ppem;
  अगर (y_ppem) *y_ppem = font->y_ppem;
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
व्योम
hb_font_set_ptem (hb_font_t *font,
		  भग्न      ptem)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->ptem == ptem)
    वापस;

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
भग्न
hb_font_get_ptem (hb_font_t *font)
{
  वापस font->ptem;
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
  वापस font->is_synthetic;
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
व्योम
hb_font_set_synthetic_bold (hb_font_t *font,
			    भग्न x_embolden,
			    भग्न y_embolden,
			    hb_bool_t in_place)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_in_place == (bool) in_place)
    वापस;

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
व्योम
hb_font_get_synthetic_bold (hb_font_t *font,
			    भग्न *x_embolden,
			    भग्न *y_embolden,
			    hb_bool_t *in_place)
{
  अगर (x_embolden) *x_embolden = font->x_embolden;
  अगर (y_embolden) *y_embolden = font->y_embolden;
  अगर (in_place) *in_place = font->embolden_in_place;
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
HB_EXTERN व्योम
hb_font_set_synthetic_slant (hb_font_t *font, भग्न slant)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->slant == slant)
    वापस;

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
HB_EXTERN भग्न
hb_font_get_synthetic_slant (hb_font_t *font)
{
  वापस font->slant;
}

#अगर_अघोषित HB_NO_VAR
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
व्योम
hb_font_set_variations (hb_font_t            *font,
			स्थिर hb_variation_t *variations,
			अचिन्हित पूर्णांक          variations_लंबाई)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  स्थिर OT::fvar &fvar = *font->face->table.fvar;
  स्वतः axes = fvar.get_axes ();
  स्थिर अचिन्हित coords_लंबाई = axes.लंबाई;

  पूर्णांक *normalized = coords_लंबाई ? (पूर्णांक *) hb_calloc (coords_लंबाई, माप (पूर्णांक)) : nullptr;
  भग्न *design_coords = coords_लंबाई ? (भग्न *) hb_calloc (coords_लंबाई, माप (भग्न)) : nullptr;

  अगर (unlikely (coords_लंबाई && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    वापस;
  }

  /* Initialize design coords. */
  क्रम (अचिन्हित पूर्णांक i = 0; i < coords_लंबाई; i++)
    design_coords[i] = axes[i].get_default ();
  अगर (font->instance_सूचकांक != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    अचिन्हित गिनती = coords_लंबाई;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_सूचकांक,
						&गिनती, design_coords);
  }

  क्रम (अचिन्हित पूर्णांक i = 0; i < variations_लंबाई; i++)
  {
    स्थिर स्वतः tag = variations[i].tag;
    स्थिर स्वतः v = variations[i].value;
    क्रम (अचिन्हित axis_सूचकांक = 0; axis_सूचकांक < coords_लंबाई; axis_सूचकांक++)
      अगर (axes[axis_सूचकांक].axisTag == tag)
	design_coords[axis_सूचकांक] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_लंबाई, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_लंबाई);
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
व्योम
hb_font_set_variation (hb_font_t *font,
		       hb_tag_t tag,
		       भग्न    value)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  // TODO Share some of this code with set_variations()

  स्थिर OT::fvar &fvar = *font->face->table.fvar;
  स्वतः axes = fvar.get_axes ();
  स्थिर अचिन्हित coords_लंबाई = axes.लंबाई;

  पूर्णांक *normalized = coords_लंबाई ? (पूर्णांक *) hb_calloc (coords_लंबाई, माप (पूर्णांक)) : nullptr;
  भग्न *design_coords = coords_लंबाई ? (भग्न *) hb_calloc (coords_लंबाई, माप (भग्न)) : nullptr;

  अगर (unlikely (coords_लंबाई && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    वापस;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  अगर (font->design_coords && font->अंक_coords == coords_लंबाई)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < coords_लंबाई; i++)
      design_coords[i] = font->design_coords[i];
  }
  अन्यथा
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < coords_लंबाई; i++)
      design_coords[i] = axes[i].get_default ();
    अगर (font->instance_सूचकांक != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      अचिन्हित गिनती = coords_लंबाई;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_सूचकांक,
						  &गिनती, design_coords);
    }
  }

  क्रम (अचिन्हित axis_सूचकांक = 0; axis_सूचकांक < coords_लंबाई; axis_सूचकांक++)
    अगर (axes[axis_सूचकांक].axisTag == tag)
      design_coords[axis_सूचकांक] = value;

  hb_ot_var_normalize_coords (font->face, coords_लंबाई, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_लंबाई);
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
व्योम
hb_font_set_var_coords_design (hb_font_t    *font,
			       स्थिर भग्न  *coords,
			       अचिन्हित पूर्णांक  निवेश_coords_लंबाई)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  स्थिर OT::fvar &fvar = *font->face->table.fvar;
  स्वतः axes = fvar.get_axes ();
  स्थिर अचिन्हित coords_लंबाई = axes.लंबाई;

  निवेश_coords_लंबाई = hb_min (निवेश_coords_लंबाई, coords_लंबाई);
  पूर्णांक *normalized = coords_लंबाई ? (पूर्णांक *) hb_calloc (coords_लंबाई, माप (पूर्णांक)) : nullptr;
  भग्न *design_coords = coords_लंबाई ? (भग्न *) hb_calloc (coords_लंबाई, माप (भग्न)) : nullptr;

  अगर (unlikely (coords_लंबाई && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    वापस;
  }

  अगर (निवेश_coords_लंबाई)
    hb_memcpy (design_coords, coords, निवेश_coords_लंबाई * माप (font->design_coords[0]));
  // Fill in the rest with default values
  क्रम (अचिन्हित पूर्णांक i = निवेश_coords_लंबाई; i < coords_लंबाई; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_लंबाई, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_लंबाई);
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
व्योम
hb_font_set_var_named_instance (hb_font_t *font,
				अचिन्हित पूर्णांक instance_सूचकांक)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  अगर (font->instance_सूचकांक == instance_सूचकांक)
    वापस;

  font->instance_सूचकांक = instance_सूचकांक;
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
अचिन्हित पूर्णांक
hb_font_get_var_named_instance (hb_font_t *font)
{
  वापस font->instance_सूचकांक;
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
व्योम
hb_font_set_var_coords_normalized (hb_font_t    *font,
				   स्थिर पूर्णांक    *coords, /* 2.14 normalized */
				   अचिन्हित पूर्णांक  निवेश_coords_लंबाई)
{
  अगर (hb_object_is_immutable (font))
    वापस;

  स्थिर OT::fvar &fvar = *font->face->table.fvar;
  स्वतः axes = fvar.get_axes ();
  अचिन्हित coords_लंबाई = axes.लंबाई;

  निवेश_coords_लंबाई = hb_min (निवेश_coords_लंबाई, coords_लंबाई);
  पूर्णांक *copy = coords_लंबाई ? (पूर्णांक *) hb_calloc (coords_लंबाई, माप (coords[0])) : nullptr;
  भग्न *design_coords = coords_लंबाई ? (भग्न *) hb_calloc (coords_लंबाई, माप (design_coords[0])) : nullptr;

  अगर (unlikely (coords_लंबाई && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    वापस;
  }

  अगर (निवेश_coords_लंबाई)
    hb_memcpy (copy, coords, निवेश_coords_लंबाई * माप (coords[0]));

  क्रम (अचिन्हित पूर्णांक i = 0; i < coords_लंबाई; ++i)
    design_coords[i] = न_अंक;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_लंबाई);
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
स्थिर पूर्णांक *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   अचिन्हित पूर्णांक *लंबाई)
{
  अगर (लंबाई)
    *लंबाई = font->अंक_coords;

  वापस font->coords;
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
स्थिर भग्न *
hb_font_get_var_coords_design (hb_font_t *font,
			       अचिन्हित पूर्णांक *लंबाई)
{
  अगर (लंबाई)
    *लंबाई = font->अंक_coords;

  वापस font->design_coords;
}
#पूर्ण_अगर

#अगर_अघोषित HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

काष्ठा hb_trampoline_closure_t
{
  व्योम *user_तथ्य;
  hb_destroy_func_t destroy;
  अचिन्हित पूर्णांक ref_गिनती_2;
};

ढाँचा <typename FuncType>
काष्ठा hb_trampoline_t
{
  hb_trampoline_closure_t closure; /* Must be first. */
  FuncType func;
};

ढाँचा <typename FuncType>
अटल hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   व्योम              *user_तथ्य,
		   hb_destroy_func_t  destroy)
{
  प्रकार hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, माप (trampoline_t));

  अगर (unlikely (!trampoline))
    वापस nullptr;

  trampoline->closure.user_तथ्य = user_तथ्य;
  trampoline->closure.destroy = destroy;
  trampoline->closure.ref_गिनती_2 = 1;
  trampoline->func = func;

  वापस trampoline;
}

अटल व्योम
trampoline_reference (hb_trampoline_closure_t *closure)
{
  closure->ref_गिनती_2++;
}

अटल व्योम
trampoline_destroy (व्योम *user_तथ्य)
{
  hb_trampoline_closure_t *closure = (hb_trampoline_closure_t *) user_तथ्य;

  अगर (--closure->ref_गिनती_2)
    वापस;

  अगर (closure->destroy)
    closure->destroy (closure->user_तथ्य);
  hb_free (closure);
}

प्रकार hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

अटल hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      व्योम           *font_तथ्य,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      व्योम           *user_तथ्य)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_तथ्य;
  वापस trampoline->func (font, font_तथ्य, unicode, 0, glyph, trampoline->closure.user_तथ्य);
}

अटल hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					व्योम           *font_तथ्य,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					व्योम           *user_तथ्य)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_तथ्य;
  वापस trampoline->func (font, font_तथ्य, unicode, variation_selector, glyph, trampoline->closure.user_तथ्य);
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
व्योम
hb_font_funcs_set_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      व्योम                     *user_तथ्य,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  अगर (hb_object_is_immutable (ffuncs))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_तथ्य, destroy);
  अगर (unlikely (!trampoline))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
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
#पूर्ण_अगर


#अगर_अघोषित HB_DISABLE_DEPRECATED

काष्ठा hb_draw_glyph_closure_t
{
  hb_font_draw_glyph_func_t func;
  व्योम *user_तथ्य;
  hb_destroy_func_t destroy;
};
अटल hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       व्योम            *font_तथ्य,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       व्योम            *draw_तथ्य,
			       व्योम            *user_तथ्य)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_तथ्य;
  closure->func (font, font_तथ्य, glyph, draw_funcs, draw_तथ्य, closure->user_तथ्य);
  वापस true;
}
अटल व्योम
hb_font_draw_glyph_closure_destroy (व्योम *user_तथ्य)
{
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) user_तथ्य;

  अगर (closure->destroy)
    closure->destroy (closure->user_तथ्य);
  hb_free (closure);
}
अटल व्योम
_hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    व्योम                      *user_तथ्य,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  अगर (hb_object_is_immutable (ffuncs))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
  }
  hb_draw_glyph_closure_t *closure = (hb_draw_glyph_closure_t *) hb_calloc (1, माप (hb_draw_glyph_closure_t));
  अगर (unlikely (!closure))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
  }
  closure->func = func;
  closure->user_तथ्य = user_तथ्य;
  closure->destroy = destroy;

  hb_font_funcs_set_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     closure,
					     hb_font_draw_glyph_closure_destroy);
}
व्योम
hb_font_funcs_set_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   व्योम                      *user_तथ्य,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_तथ्य, destroy);
}
व्योम
hb_font_funcs_set_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   व्योम                           *user_तथ्य,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  _hb_font_funcs_set_draw_glyph_func (ffuncs, func, user_तथ्य, destroy);
}

काष्ठा hb_paint_glyph_closure_t
{
  hb_font_paint_glyph_func_t func;
  व्योम *user_तथ्य;
  hb_destroy_func_t destroy;
};
अटल hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				व्योम *font_तथ्य,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				व्योम *paint_तथ्य,
				अचिन्हित पूर्णांक palette,
				hb_color_t foreground,
				व्योम *user_तथ्य)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_तथ्य;
  closure->func (font, font_तथ्य, glyph, paint_funcs, paint_तथ्य, palette, foreground, closure->user_तथ्य);
  वापस true;
}
अटल व्योम
hb_font_paint_glyph_closure_destroy (व्योम *user_तथ्य)
{
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) user_तथ्य;

  अगर (closure->destroy)
    closure->destroy (closure->user_तथ्य);
  hb_free (closure);
}
व्योम
hb_font_funcs_set_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    व्योम                      *user_तथ्य,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  अगर (hb_object_is_immutable (ffuncs))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
  }
  hb_paint_glyph_closure_t *closure = (hb_paint_glyph_closure_t *) hb_calloc (1, माप (hb_paint_glyph_closure_t));
  अगर (unlikely (!closure))
  {
    अगर (destroy)
      destroy (user_तथ्य);
    वापस;
  }
  closure->func = func;
  closure->user_तथ्य = user_तथ्य;
  closure->destroy = destroy;

  hb_font_funcs_set_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      closure,
					      hb_font_paint_glyph_closure_destroy);
}
#पूर्ण_अगर
