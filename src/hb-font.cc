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

#جے_بنا_تعریف HB_NO_OT_FONT
#ضمن "hb-ot.h"
#جے_انت
#جے_تعریف_شدہ HAVE_FREETYPE
#ضمن "hb-ft.h"
#جے_انت
#جے_تعریف_شدہ HAVE_FONTATIONS
#ضمن "hb-fontations.h"
#جے_انت
#جے_تعریف_شدہ HAVE_CORETEXT
#ضمن "hb-coretext.h"
#جے_انت
#جے_تعریف_شدہ HAVE_DIRECTWRITE
#ضمن "hb-directwrite.h"
#جے_انت


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
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_بن_ورتیا,
				فراغ              *font_انکڑے HB_بن_ورتیا,
				hb_font_extents_t *extents,
				فراغ              *user_انکڑے HB_بن_ورتیا)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    فراغ              *font_انکڑے HB_بن_ورتیا,
				    hb_font_extents_t *extents,
				    فراغ              *user_انکڑے HB_بن_ورتیا)
{
  hb_bool_t ret = font->ماپا->get_font_h_extents (extents, خطأ);
  إذا (ret) {
    extents->ascender = font->ماپا_scale_y_فاصلہ (extents->ascender);
    extents->descender = font->ماپا_scale_y_فاصلہ (extents->descender);
    extents->سطر_gap = font->ماپا_scale_y_فاصلہ (extents->سطر_gap);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_بن_ورتیا,
				فراغ              *font_انکڑے HB_بن_ورتیا,
				hb_font_extents_t *extents,
				فراغ              *user_انکڑے HB_بن_ورتیا)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    فراغ              *font_انکڑے HB_بن_ورتیا,
				    hb_font_extents_t *extents,
				    فراغ              *user_انکڑے HB_بن_ورتیا)
{
  hb_bool_t ret = font->ماپا->get_font_v_extents (extents, خطأ);
  إذا (ret) {
    extents->ascender = font->ماپا_scale_x_فاصلہ (extents->ascender);
    extents->descender = font->ماپا_scale_x_فاصلہ (extents->descender);
    extents->سطر_gap = font->ماپا_scale_x_فاصلہ (extents->سطر_gap);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_بن_ورتیا,
			       فراغ           *font_انکڑے HB_بن_ورتیا,
			       hb_codepoint_t  unicode HB_بن_ورتیا,
			       hb_codepoint_t *glyph,
			       فراغ           *user_انکڑے HB_بن_ورتیا)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   فراغ           *font_انکڑے HB_بن_ورتیا,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_nominal_glyphs_func_سیٹ ())
  {
    أرجع font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  أرجع font->ماپا->get_nominal_glyph (unicode, glyph);
}

#تعریف_کرو hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

ساكن بنانشانی صحيح
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    فراغ                 *font_انکڑے HB_بن_ورتیا,
				    بنانشانی صحيح          گنتی,
				    ثابت hb_codepoint_t *پہلا_unicode,
				    بنانشانی صحيح          unicode_stride,
				    hb_codepoint_t       *پہلا_glyph,
				    بنانشانی صحيح          glyph_stride,
				    فراغ                 *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_nominal_glyph_func_سیٹ ())
  {
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (!font->get_nominal_glyph (*پہلا_unicode, پہلا_glyph))
	أرجع i;

      پہلا_unicode = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_unicode, unicode_stride);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    أرجع گنتی;
  }

  أرجع font->ماپا->get_nominal_glyphs (گنتی,
					   پہلا_unicode, unicode_stride,
					   پہلا_glyph, glyph_stride);
}

ساكن hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_بن_ورتیا,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t  unicode HB_بن_ورتیا,
				 hb_codepoint_t  variation_selector HB_بن_ورتیا,
				 hb_codepoint_t *glyph,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->ماپا->get_variation_glyph (unicode, variation_selector, glyph);
}


ساكن hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t  glyph HB_بن_ورتیا,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->x_scale;
}

ساكن hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_h_advances_func_سیٹ ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, خطأ);
    أرجع ret;
  }
  أرجع font->ماپا_scale_x_فاصلہ (font->ماپا->get_glyph_h_advance (glyph, خطأ));
}

ساكن hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t  glyph HB_بن_ورتیا,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع -font->y_scale;
}

ساكن hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_v_advances_func_سیٹ ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, خطأ);
    أرجع ret;
  }
  أرجع font->ماپا_scale_y_فاصلہ (font->ماپا->get_glyph_v_advance (glyph, خطأ));
}

#تعریف_کرو hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

ساكن فراغ
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      فراغ*                 font_انکڑے HB_بن_ورتیا,
				      بنانشانی صحيح          گنتی,
				      ثابت hb_codepoint_t *پہلا_glyph,
				      بنانشانی صحيح          glyph_stride,
				      hb_position_t        *پہلا_advance,
				      بنانشانی صحيح          advance_stride,
				      فراغ                 *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_h_advance_func_سیٹ ())
  {
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->get_glyph_h_advance (*پہلا_glyph, خطأ);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

  font->ماپا->get_glyph_h_advances (گنتی,
				      پہلا_glyph, glyph_stride,
				      پہلا_advance, advance_stride,
				      خطأ);
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    *پہلا_advance = font->ماپا_scale_x_فاصلہ (*پہلا_advance);
    پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
  }
}

#تعریف_کرو hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
ساكن فراغ
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      فراغ*                 font_انکڑے HB_بن_ورتیا,
				      بنانشانی صحيح          گنتی,
				      ثابت hb_codepoint_t *پہلا_glyph,
				      بنانشانی صحيح          glyph_stride,
				      hb_position_t        *پہلا_advance,
				      بنانشانی صحيح          advance_stride,
				      فراغ                 *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_v_advance_func_سیٹ ())
  {
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      *پہلا_advance = font->get_glyph_v_advance (*پہلا_glyph, خطأ);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
    }
    أرجع;
  }

  font->ماپا->get_glyph_v_advances (گنتی,
				      پہلا_glyph, glyph_stride,
				      پہلا_advance, advance_stride,
				      خطأ);
  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    *پہلا_advance = font->ماپا_scale_y_فاصلہ (*پہلا_advance);
    پہلا_advance = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_advance, advance_stride);
  }
}

ساكن hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_بن_ورتیا,
				فراغ           *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t  glyph HB_بن_ورتیا,
				hb_position_t  *x,
				hb_position_t  *y,
				فراغ           *user_انکڑے HB_بن_ورتیا)
{
  *x = *y = 0;
  أرجع سچ;
}

ساكن hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    فراغ           *font_انکڑے HB_بن_ورتیا,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_h_origins_func_سیٹ ())
  {
    أرجع font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, خطأ);
  }
  hb_bool_t ret = font->ماپا->get_glyph_h_origin (glyph, x, y);
  إذا (ret)
    font->ماپا_scale_position (x, y);
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_بن_ورتیا,
				فراغ           *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t  glyph HB_بن_ورتیا,
				hb_position_t  *x,
				hb_position_t  *y,
				فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    فراغ           *font_انکڑے HB_بن_ورتیا,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_v_origins_func_سیٹ ())
  {
    أرجع font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, خطأ);
  }
  hb_bool_t ret = font->ماپا->get_glyph_v_origin (glyph, x, y);
  إذا (ret)
    font->ماپا_scale_position (x, y);
  أرجع ret;
}

#تعریف_کرو hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

ساكن hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_بن_ورتیا,
				     فراغ *font_انکڑے HB_بن_ورتیا,
				     بنانشانی صحيح گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph HB_بن_ورتیا,
				     بنانشانی glyph_stride HB_بن_ورتیا,
				     hb_position_t *پہلا_x,
				     بنانشانی x_stride,
				     hb_position_t *پہلا_y,
				     بنانشانی y_stride,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_h_origin_func_سیٹ ())
  {
    hb_bool_t ret = سچ;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ret &= font->get_glyph_h_origin (*پہلا_glyph, پہلا_x, پہلا_y, خطأ);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
    أرجع ret;
  }

  hb_bool_t ret = font->ماپا->get_glyph_h_origins (گنتی,
						     پہلا_glyph, glyph_stride,
						     پہلا_x, x_stride,
						     پہلا_y, y_stride);
  إذا (ret)
  {
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      font->ماپا_scale_position (پہلا_x, پہلا_y);
      پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
  }
  أرجع ret;
}

#تعریف_کرو hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

ساكن hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_بن_ورتیا,
				     فراغ *font_انکڑے HB_بن_ورتیا,
				     بنانشانی صحيح گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph HB_بن_ورتیا,
				     بنانشانی glyph_stride HB_بن_ورتیا,
				     hb_position_t *پہلا_x,
				     بنانشانی x_stride,
				     hb_position_t *پہلا_y,
				     بنانشانی y_stride,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  إذا (font->has_glyph_v_origin_func_سیٹ ())
  {
    hb_bool_t ret = سچ;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ret &= font->get_glyph_v_origin (*پہلا_glyph, پہلا_x, پہلا_y, خطأ);
      پہلا_glyph = &Struct_At_وتھ_Unaligned<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
    أرجع ret;
  }

  hb_bool_t ret = font->ماپا->get_glyph_v_origins (گنتی,
						     پہلا_glyph, glyph_stride,
						     پہلا_x, x_stride,
						     پہلا_y, y_stride);
  إذا (ret)
  {
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      font->ماپا_scale_position (پہلا_x, پہلا_y);
      پہلا_x = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_x, x_stride);
      پہلا_y = &Struct_At_وتھ_Unaligned<hb_position_t> (پہلا_y, y_stride);
    }
  }
  أرجع ret;
}

ساكن hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_بن_ورتیا,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t  بایاں_glyph HB_بن_ورتیا,
				 hb_codepoint_t  دایاں_glyph HB_بن_ورتیا,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع 0;
}

ساكن hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  بایاں_glyph,
				     hb_codepoint_t  دایاں_glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->ماپا_scale_x_فاصلہ (font->ماپا->get_glyph_h_kerning (بایاں_glyph, دایاں_glyph));
}

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
ساكن hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_بن_ورتیا,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t  چوٹی_glyph HB_بن_ورتیا,
				 hb_codepoint_t  bottom_glyph HB_بن_ورتیا,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع 0;
}

ساكن hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  چوٹی_glyph,
				     hb_codepoint_t  bottom_glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->ماپا_scale_y_فاصلہ (font->ماپا->get_glyph_v_kerning (چوٹی_glyph, bottom_glyph));
}
#جے_انت

ساكن hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_بن_ورتیا,
			       فراغ               *font_انکڑے HB_بن_ورتیا,
			       hb_codepoint_t      glyph HB_بن_ورتیا,
			       hb_glyph_extents_t *extents,
			       فراغ               *user_انکڑے HB_بن_ورتیا)
{
  hb_memset (extents, 0, حجم (*extents));
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   فراغ               *font_انکڑے HB_بن_ورتیا,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   فراغ               *user_انکڑے HB_بن_ورتیا)
{
  hb_bool_t ret = font->ماپا->get_glyph_extents (glyph, extents, خطأ);
  إذا (ret) {
    font->ماپا_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->ماپا_scale_فاصلہ (&extents->چوڑائی, &extents->اچائی);
  }
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_contour_نقطہ_nil (hb_font_t      *font HB_بن_ورتیا,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     hb_codepoint_t  glyph HB_بن_ورتیا,
				     بنانشانی صحيح    نقطہ_اشاریہ HB_بن_ورتیا,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  *x = *y = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_contour_نقطہ_default (hb_font_t      *font,
					 فراغ           *font_انکڑے HB_بن_ورتیا,
					 hb_codepoint_t  glyph,
					 بنانشانی صحيح    نقطہ_اشاریہ,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  hb_bool_t ret = font->ماپا->get_glyph_contour_نقطہ (glyph, نقطہ_اشاریہ, x, y, خطأ);
  إذا (ret)
    font->ماپا_scale_position (x, y);
  أرجع ret;
}

ساكن hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_بن_ورتیا,
			    فراغ           *font_انکڑے HB_بن_ورتیا,
			    hb_codepoint_t  glyph HB_بن_ورتیا,
			    حرف           *name,
			    بنانشانی صحيح    ناپ,
			    فراغ           *user_انکڑے HB_بن_ورتیا)
{
  إذا (ناپ) *name = '\0';
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				فراغ           *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t  glyph,
				حرف           *name,
				بنانشانی صحيح    ناپ,
				فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->ماپا->get_glyph_name (glyph, name, ناپ);
}

ساكن hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_بن_ورتیا,
				 فراغ           *font_انکڑے HB_بن_ورتیا,
				 ثابت حرف     *name HB_بن_ورتیا,
				 صحيح             len HB_بن_ورتیا, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 فراغ           *user_انکڑے HB_بن_ورتیا)
{
  *glyph = 0;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     فراغ           *font_انکڑے HB_بن_ورتیا,
				     ثابت حرف     *name,
				     صحيح             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     فراغ           *user_انکڑے HB_بن_ورتیا)
{
  أرجع font->ماپا->get_glyph_from_name (name, len, glyph);
}

ساكن hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_بن_ورتیا,
				فراغ            *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				فراغ            *draw_انکڑے,
				فراغ            *user_انکڑے HB_بن_ورتیا)
{
  أرجع خطأ;
}

ساكن hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_بن_ورتیا,
				 فراغ *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t glyph HB_بن_ورتیا,
				 hb_paint_funcs_t *paint_funcs HB_بن_ورتیا,
				 فراغ *paint_انکڑے HB_بن_ورتیا,
				 بنانشانی صحيح palette HB_بن_ورتیا,
				 hb_color_t foreground HB_بن_ورتیا,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع خطأ;
}

تعريفنوع بنية hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  فراغ		  *draw_انکڑے;
  عائم		   x_scale;
  عائم		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

ساكن فراغ
hb_draw_move_منزل_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			 فراغ *draw_انکڑے,
			 hb_draw_state_t *st,
			 عائم منزل_x, عائم منزل_y,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_منزل (adaptor->draw_انکڑے, *st,
				     x_scale * منزل_x, y_scale * منزل_y);
}

ساكن فراغ
hb_draw_سطر_منزل_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			 hb_draw_state_t *st,
			 عائم منزل_x, عائم منزل_y,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_سطر_منزل (adaptor->draw_انکڑے, *st,
				     x_scale * منزل_x, y_scale * منزل_y);
}

ساكن فراغ
hb_draw_quadratic_منزل_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			      hb_draw_state_t *st,
			      عائم control_x, عائم control_y,
			      عائم منزل_x, عائم منزل_y,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_منزل (adaptor->draw_انکڑے, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * منزل_x, y_scale * منزل_y);
}

ساكن فراغ
hb_draw_cubic_منزل_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			  hb_draw_state_t *st,
			  عائم control1_x, عائم control1_y,
			  عائم control2_x, عائم control2_y,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  عائم x_scale = adaptor->x_scale;
  عائم y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_منزل (adaptor->draw_انکڑے, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * منزل_x, y_scale * منزل_y);
}

ساكن فراغ
hb_draw_close_رستہ_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			    hb_draw_state_t *st,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;

  adaptor->draw_funcs->emit_close_رستہ (adaptor->draw_انکڑے, *st);
}

ساكن hb_bool_t
hb_draw_سیٹ_budget_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			    int64_t budget,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  أرجع adaptor->draw_funcs->سیٹ_budget (adaptor->draw_انکڑے, budget);
}

ساكن int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  أرجع adaptor->draw_funcs->get_budget (adaptor->draw_انکڑے);
}

ساكن int64_t *
hb_draw_get_budget_بچیا_default (hb_draw_funcs_t *dfuncs HB_بن_ورتیا, فراغ *draw_انکڑے,
				      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_انکڑے;
  أرجع adaptor->draw_funcs->get_budget_بچیا_ptr (adaptor->draw_انکڑے);
}

ساكن ثابت hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_سرخی_STATIC,

  {
#تعریف_کرو HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_DRAW_FUNC_IMPLEMENT
  }
};

ساكن hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    فراغ            *font_انکڑے HB_بن_ورتیا,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    فراغ            *draw_انکڑے,
				    فراغ            *user_انکڑے HB_بن_ورتیا)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_انکڑے,
    font->ماپا->x_scale ? (عائم) font->x_scale / (عائم) font->ماپا->x_scale : 0.f,
    font->ماپا->y_scale ? (عائم) font->y_scale / (عائم) font->ماپا->y_scale : 0.f
  };

  أرجع font->ماپا->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   خطأ);
}

ساكن hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     فراغ *font_انکڑے,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     فراغ *paint_انکڑے,
				     بنانشانی صحيح palette,
				     hb_color_t foreground,
				     فراغ *user_انکڑے)
{
  paint_funcs->ڈالو_transform (paint_انکڑے,
    font->ماپا->x_scale ? (عائم) font->x_scale / (عائم) font->ماپا->x_scale : 0, 0,
    0, font->ماپا->y_scale ? (عائم) font->y_scale / (عائم) font->ماپا->y_scale : 0,
    0, 0);

  منطقي ret = font->ماپا->paint_glyph_or_fail (glyph, paint_funcs, paint_انکڑے, palette, foreground);

  paint_funcs->نکالو_transform (paint_انکڑے);

  أرجع ret;
}

DEFINE_NULL_INSTANCE (hb_font_funcs_t) =
{
  HB_OBJECT_سرخی_STATIC,

  خالی_سوچک,
  خالی_سوچک,
  {
    {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_nil,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
    }
  }
};

ساكن ثابت hb_font_funcs_t _hb_font_funcs_default = {
  HB_OBJECT_سرخی_STATIC,

  خالی_سوچک,
  خالی_سوچک,
  {
    {
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_default,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
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
    أرجع hb_font_funcs_get_خالی_تھاواں ();

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
hb_font_funcs_get_خالی_تھاواں ()
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
#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) إذا (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_انکڑے ? خالی_سوچک : ffuncs->user_انکڑے->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_انکڑے);

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
hb_font_funcs_سیٹ_user_انکڑے (hb_font_funcs_t    *ffuncs,
			     hb_user_انکڑے_کلید_t *key,
			     فراغ *              انکڑے,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (ffuncs, key, انکڑے, destroy, replace);
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
hb_font_funcs_get_user_انکڑے (ثابت hb_font_funcs_t *ffuncs,
			     hb_user_انکڑے_کلید_t    *key)
{
  أرجع hb_object_get_user_انکڑے (ffuncs, key);
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
hb_font_funcs_سیٹ_preamble (hb_font_funcs_t    *ffuncs,
			     منطقي                func_is_null,
			     فراغ              **user_انکڑے,
			     hb_destroy_func_t  *destroy)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (*destroy)
      (*destroy) (*user_انکڑے);
    أرجع خطأ;
  }

  إذا (func_is_null)
  {
    إذا (*destroy)
      (*destroy) (*user_انکڑے);
    *destroy = خالی_سوچک;
    *user_انکڑے = خالی_سوچک;
  }

  أرجع سچ;
}

ساكن منطقي
hb_font_funcs_سیٹ_درمیان (hb_font_funcs_t   *ffuncs,
			   فراغ              *user_انکڑے,
			   hb_destroy_func_t  destroy)
{
  خودکار destroy_guard = hb_make_scope_guard ([&]() {
    إذا (destroy) destroy (user_انکڑے);
  });

  إذا (user_انکڑے && !ffuncs->user_انکڑے)
  {
    ffuncs->user_انکڑے = (decltype (ffuncs->user_انکڑے)) hb_calloc (1, حجم (*ffuncs->user_انکڑے));
    إذا (unlikely (!ffuncs->user_انکڑے))
      أرجع خطأ;
  }
  إذا (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, حجم (*ffuncs->destroy));
    إذا (unlikely (!ffuncs->destroy))
      أرجع خطأ;
  }

  destroy_guard.release ();
  أرجع سچ;
}

#تعریف_کرو HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
فراغ                                                                     \
hb_font_funcs_سیٹ##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 فراغ                        *user_انکڑے, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  إذا (!hb_font_funcs_سیٹ_preamble (ffuncs, !func, &user_انکڑے, &destroy))\
      أرجع;                                                            \
									 \
  إذا (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_انکڑے ? خالی_سوچک : ffuncs->user_انکڑے->name); \
                                                                         \
  إذا (!hb_font_funcs_سیٹ_درمیان (ffuncs, user_انکڑے, destroy))           \
      أرجع;                                                            \
									 \
  إذا (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  وإلا                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  إذا (ffuncs->user_انکڑے)                                                 \
    ffuncs->user_انکڑے->name = user_انکڑے;                                 \
  إذا (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#تعریف_رد HB_FONT_FUNC_IMPLEMENT

منطقي
hb_font_t::has_func_سیٹ (بنانشانی صحيح i)
{
  أرجع هذا->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

منطقي
hb_font_t::has_func (بنانشانی صحيح i)
{
  أرجع has_func_سیٹ (i) ||
	 (ماپا && ماپا != &_hb_Null_hb_font_t && ماپا->has_func (i));
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
بنانشانی صحيح
hb_font_get_nominal_glyphs (hb_font_t *font,
			    بنانشانی صحيح گنتی,
			    ثابت hb_codepoint_t *پہلا_unicode,
			    بنانشانی صحيح unicode_stride,
			    hb_codepoint_t *پہلا_glyph,
			    بنانشانی صحيح glyph_stride)
{
  أرجع font->get_nominal_glyphs (گنتی,
				   پہلا_unicode, unicode_stride,
				   پہلا_glyph, glyph_stride);
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
			      بنانشانی صحيح          گنتی,
			      ثابت hb_codepoint_t *پہلا_glyph,
			      بنانشانی              glyph_stride,
			      hb_position_t        *پہلا_advance,
			      بنانشانی              advance_stride)
{
  font->get_glyph_h_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
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
			      بنانشانی صحيح          گنتی,
			      ثابت hb_codepoint_t *پہلا_glyph,
			      بنانشانی              glyph_stride,
			      hb_position_t        *پہلا_advance,
			      بنانشانی              advance_stride)
{
  font->get_glyph_v_advances (گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
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
			     بنانشانی صحيح    گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     بنانشانی صحيح    glyph_stride,
			     hb_position_t  *پہلا_x,
			     بنانشانی صحيح    x_stride,
			     hb_position_t  *پہلا_y,
			     بنانشانی صحيح    y_stride)

{
  أرجع font->get_glyph_h_origins (گنتی,
				    پہلا_glyph, glyph_stride,
				    پہلا_x, x_stride,
				    پہلا_y, y_stride);
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
			     بنانشانی صحيح    گنتی,
			     ثابت hb_codepoint_t *پہلا_glyph,
			     بنانشانی صحيح    glyph_stride,
			     hb_position_t  *پہلا_x,
			     بنانشانی صحيح    x_stride,
			     hb_position_t  *پہلا_y,
			     بنانشانی صحيح    y_stride)

{
  أرجع font->get_glyph_v_origins (گنتی,
				    پہلا_glyph, glyph_stride,
				    پہلا_x, x_stride,
				    پہلا_y, y_stride);
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
			     hb_codepoint_t  بایاں_glyph,
			     hb_codepoint_t  دایاں_glyph)
{
  أرجع font->get_glyph_h_kerning (بایاں_glyph, دایاں_glyph);
}

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
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
			     hb_codepoint_t  چوٹی_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  أرجع font->get_glyph_v_kerning (چوٹی_glyph, bottom_glyph);
}
#جے_انت

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
hb_font_get_glyph_contour_نقطہ (hb_font_t      *font,
				 hb_codepoint_t  glyph,
				 بنانشانی صحيح    نقطہ_اشاریہ,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  أرجع font->get_glyph_contour_نقطہ (glyph, نقطہ_اشاریہ, x, y);
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
			بنانشانی صحيح    ناپ)
{
  أرجع font->get_glyph_name (glyph, name, ناپ);
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

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
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
		         hb_draw_funcs_t *dfuncs, فراغ *draw_انکڑے)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_انکڑے);
}
#جے_انت

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
			    hb_draw_funcs_t *dfuncs, فراغ *draw_انکڑے)
{
  أرجع font->draw_glyph_or_fail (glyph, dfuncs, draw_انکڑے);
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
			     hb_paint_funcs_t *pfuncs, فراغ *paint_انکڑے,
			     بنانشانی صحيح palette_اشاریہ,
			     hb_color_t foreground)
{
  أرجع font->paint_glyph_or_fail (glyph, pfuncs, paint_انکڑے, palette_اشاریہ, foreground);
}

/* A bit higher-level, and with fallback */

فراغ
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, فراغ *paint_انکڑے,
			بنانشانی صحيح palette,
			hb_color_t foreground)
{
  إذا (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_انکڑے,
			   palette, foreground))
    أرجع;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_انکڑے, glyph, هذا, سچ, foreground);
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
		    hb_draw_funcs_t *dfuncs, فراغ *draw_انکڑے)
{
  (فراغ) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_انکڑے);
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
                     hb_paint_funcs_t *pfuncs, فراغ *paint_انکڑے,
                     بنانشانی صحيح palette_اشاریہ,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_انکڑے, palette_اشاریہ, foreground);
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
					  بنانشانی صحيح          گنتی,
					  ثابت hb_codepoint_t *پہلا_glyph,
					  بنانشانی              glyph_stride,
					  hb_position_t        *پہلا_advance,
					  بنانشانی              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, گنتی, پہلا_glyph, glyph_stride, پہلا_advance, advance_stride);
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
hb_font_جوڑو_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  أرجع font->جوڑو_glyph_origin_for_direction (glyph, direction, x, y);
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
hb_font_گھٹاؤ_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  أرجع font->گھٹاؤ_glyph_origin_for_direction (glyph, direction, x, y);
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
					 hb_codepoint_t  پہلا_glyph,
					 hb_codepoint_t  دوجا_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  أرجع font->get_glyph_kerning_for_direction (پہلا_glyph, دوجا_glyph, direction, x, y);
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
hb_font_get_glyph_contour_نقطہ_for_origin (hb_font_t      *font,
					    hb_codepoint_t  glyph,
					    بنانشانی صحيح    نقطہ_اشاریہ,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  أرجع font->get_glyph_contour_نقطہ_for_origin (glyph, نقطہ_اشاریہ, direction, x, y);
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
hb_font_glyph_منزل_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 حرف           *s,
			 بنانشانی صحيح    ناپ)
{
  font->glyph_منزل_string (glyph, s, ناپ);
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
  HB_OBJECT_سرخی_STATIC,

  0, /* serial */
  0, /* serial_coords */

  خالی_سوچک, /*ماپا*/
  const_cast<hb_face_t *> (&_hb_Null_hb_face_t),

  1000, /* x_scale */
  1000, /* y_scale */
  خطأ, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  سچ, /* embolden_in_place */
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
  خالی_سوچک, /* coords */
  خالی_سوچک, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


ساكن hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  إذا (unlikely (!face))
    face = hb_face_get_خالی_تھاواں ();

  إذا (!(font = hb_object_create<hb_font_t> ()))
    أرجع hb_font_get_خالی_تھاواں ();

  hb_face_make_immutable (face);
  font->ماپا = hb_font_get_خالی_تھاواں ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_خالی_تھاواں ();
  font->انکڑے.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_وچ_رکھو = سچ;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_اشاریہ = HB_FONT_NO_VAR_NAMED_INSTANCE;

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

  hb_font_سیٹ_funcs_using (font, خالی_سوچک);

#جے_بنا_تعریف HB_NO_VAR
  // Initialize variations.
  إذا (likely (face))
  {
    إذا (face->اشاریہ_2 >> 16)
      hb_font_سیٹ_var_named_instance (font, (face->اشاریہ_2 >> 16) - 1);
    وإلا
      hb_font_سیٹ_variations (font, خالی_سوچک, 0);
  }
#جے_انت

  أرجع font;
}

ساكن فراغ
_hb_font_adopt_var_coords (hb_font_t *font,
			   صحيح *coords, /* 2.14 normalized */
			   عائم *design_coords,
			   بنانشانی صحيح coords_لمائی)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->عدد_coords = coords_لمائی;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_لمائی));

  font->بدلیا ();
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
hb_font_create_sub_font (hb_font_t *ماپا)
{
  إذا (unlikely (!ماپا))
    ماپا = hb_font_get_خالی_تھاواں ();

  hb_font_t *font = _hb_font_create (ماپا->face);

  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع font;

  font->ماپا = hb_font_reference (ماپا);

  font->x_scale = ماپا->x_scale;
  font->y_scale = ماپا->y_scale;
  font->x_embolden = ماپا->x_embolden;
  font->y_embolden = ماپا->y_embolden;
  font->embolden_وچ_رکھو = ماپا->embolden_وچ_رکھو;
  font->slant = ماپا->slant;
  font->x_ppem = ماپا->x_ppem;
  font->y_ppem = ماپا->y_ppem;
  font->ptem = ماپا->ptem;

  بنانشانی صحيح عدد_coords = ماپا->عدد_coords;
  إذا (عدد_coords)
  {
    صحيح *coords = (صحيح *) hb_calloc (عدد_coords, حجم (ماپا->coords[0]));
    عائم *design_coords = (عائم *) hb_calloc (عدد_coords, حجم (ماپا->design_coords[0]));
    إذا (likely (coords && design_coords))
    {
      hb_memcpy (coords, ماپا->coords, عدد_coords * حجم (ماپا->coords[0]));
      hb_memcpy (design_coords, ماپا->design_coords, عدد_coords * حجم (ماپا->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, عدد_coords);
    }
    وإلا
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->بدلیا ();
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
hb_font_get_خالی_تھاواں ()
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

  font->انکڑے.fini ();

  إذا (font->destroy)
    font->destroy (font->user_انکڑے);

  hb_font_destroy (font->ماپا);
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
hb_font_سیٹ_user_انکڑے (hb_font_t          *font,
		       hb_user_انکڑے_کلید_t *key,
		       فراغ *              انکڑے,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  إذا (!hb_object_is_immutable (font))
    font->بدلیا ();

  أرجع hb_object_سیٹ_user_انکڑے (font, key, انکڑے, destroy, replace);
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
hb_font_get_user_انکڑے (ثابت hb_font_t    *font,
		       hb_user_انکڑے_کلید_t *key)
{
  أرجع hb_object_get_user_انکڑے (font, key);
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

  إذا (font->ماپا)
    hb_font_make_immutable (font->ماپا);

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
بنانشانی صحيح
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
hb_font_بدلیا (hb_font_t *font)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  font->بدلیا ();
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
hb_font_سیٹ_ماپا (hb_font_t *font,
		    hb_font_t *ماپا)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (ماپا == font->ماپا)
    أرجع;

  إذا (!ماپا)
    ماپا = hb_font_get_خالی_تھاواں ();

  لكل (hb_font_t *p = ماپا; p && p != hb_font_get_خالی_تھاواں(); p = p->ماپا)
    إذا (p == font)
      أرجع; /* Would create a cycle - reject */

  hb_font_t *old = font->ماپا;

  font->ماپا = hb_font_reference (ماپا);

  hb_font_destroy (old);

  font->بدلیا ();
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
hb_font_get_ماپا (hb_font_t *font)
{
  أرجع font->ماپا;
}

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
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
hb_font_سیٹ_face (hb_font_t *font,
		  hb_face_t *face)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (face == font->face)
    أرجع;

  إذا (unlikely (!face))
    face = hb_face_get_خالی_تھاواں ();

  hb_face_t *old = font->face;

  hb_face_make_immutable (face);
  font->face = hb_face_reference (face);
  font->بدلیا ();

  /* Release the old font funcs while the old face is still alive. */
  hb_font_سیٹ_funcs (font, خالی_سوچک, خالی_سوچک, خالی_سوچک);
  hb_font_سیٹ_funcs_using (font, خالی_سوچک);

  hb_face_destroy (old);

  font->بدلیا ();
  font->serial_coords = font->serial;
}
#جے_انت

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
hb_font_سیٹ_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   فراغ              *font_انکڑے,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (font))
  {
    إذا (destroy)
      destroy (font_انکڑے);
    أرجع;
  }

  إذا (font->destroy)
    font->destroy (font->user_انکڑے);

  إذا (!klass)
    klass = hb_font_funcs_get_خالی_تھاواں ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_انکڑے = font_انکڑے;
  font->destroy = destroy;

  font->بدلیا ();
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
hb_font_سیٹ_funcs_انکڑے (hb_font_t         *font,
		        فراغ              *font_انکڑے,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  إذا (hb_object_is_immutable (font))
  {
    إذا (destroy)
      destroy (font_انکڑے);
    أرجع;
  }

  إذا (font->destroy)
    font->destroy (font->user_انکڑے);

  font->user_انکڑے = font_انکڑے;
  font->destroy = destroy;

  font->بدلیا ();
}

ساكن ثابت بنية supported_font_funcs_t {
	حرف name[16];
	فراغ (*func) (hb_font_t *);
} supported_font_funcs[] =
{
#جے_بنا_تعریف HB_NO_OT_FONT
  {"ot",	hb_ot_font_سیٹ_funcs},
#جے_انت
#جے_تعریف_شدہ HAVE_FREETYPE
  {"ft",	hb_ft_font_سیٹ_funcs},
#جے_انت
#جے_تعریف_شدہ HAVE_FONTATIONS
  {"fontations",hb_fontations_font_سیٹ_funcs},
#جے_انت
#جے_تعریف_شدہ HAVE_CORETEXT
  {"coretext",	hb_coretext_font_سیٹ_funcs},
#جے_انت
#جے_تعریف_شدہ HAVE_DIRECTWRITE
  {"directwrite",hb_directwrite_font_سیٹ_funcs},
#جے_انت
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
    إذا (!static_funcs_name.cmpexch (خالی_سوچک, name))
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
hb_font_سیٹ_funcs_using (hb_font_t  *font,
			 ثابت حرف *name)
{
  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع خطأ;

  منطقي retry = خطأ;

  إذا (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = سچ;
  }
  إذا (name && !*name) name = خالی_سوچک;

retry:
  لكل (بنانشانی i = 0; i < ARRAY_لمائی (supported_font_funcs); i++)
    إذا (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      إذا (name || font->klass != hb_font_funcs_get_خالی_تھاواں ())
	أرجع سچ;
    }

  إذا (retry)
  {
    retry = خطأ;
    name = خالی_سوچک;
    اذهب retry;
  }

  أرجع خطأ;
}

ساكن مضمن فراغ free_static_font_funcs_list ();

ساكن ثابت حرف * ثابت nil_font_funcs_list[] = {خالی_سوچک};

ساكن بنية hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<ثابت حرف *,
								  hb_font_funcs_list_lazy_loader_t>
{
  ساكن ثابت حرف ** create ()
  {
    ثابت حرف **font_funcs_list = (ثابت حرف **) hb_calloc (1 + ARRAY_لمائی (supported_font_funcs), حجم (ثابت حرف *));
    إذا (unlikely (!font_funcs_list))
      أرجع خالی_سوچک;

    بنانشانی i;
    لكل (i = 0; i < ARRAY_لمائی (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = خالی_سوچک;

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
hb_font_سیٹ_scale (hb_font_t *font,
		   صحيح        x_scale,
		   صحيح        y_scale)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_scale == x_scale && font->y_scale == y_scale)
    أرجع;

  font->x_scale = x_scale;
  font->y_scale = y_scale;

  font->بدلیا ();
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
hb_font_سیٹ_ppem (hb_font_t    *font,
		  بنانشانی صحيح  x_ppem,
		  بنانشانی صحيح  y_ppem)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    أرجع;

  font->x_ppem = x_ppem;
  font->y_ppem = y_ppem;

  font->بدلیا ();
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
		  بنانشانی صحيح *x_ppem,
		  بنانشانی صحيح *y_ppem)
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
hb_font_سیٹ_ptem (hb_font_t *font,
		  عائم      ptem)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->ptem == ptem)
    أرجع;

  font->ptem = ptem;

  font->بدلیا ();
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
hb_font_سیٹ_synthetic_bold (hb_font_t *font,
			    عائم x_embolden,
			    عائم y_embolden,
			    hb_bool_t in_place)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_وچ_رکھو == (منطقي) in_place)
    أرجع;

  font->x_embolden = x_embolden;
  font->y_embolden = y_embolden;
  font->embolden_وچ_رکھو = in_place;

  font->بدلیا ();
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
  إذا (in_place) *in_place = font->embolden_وچ_رکھو;
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
hb_font_سیٹ_synthetic_slant (hb_font_t *font, عائم slant)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->slant == slant)
    أرجع;

  font->slant = slant;

  font->بدلیا ();
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

#جے_بنا_تعریف HB_NO_VAR
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
hb_font_سیٹ_variations (hb_font_t            *font,
			ثابت hb_variation_t *variations,
			بنانشانی صحيح          variations_لمائی)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->جدول.fvar;
  خودکار axes = fvar.get_axes ();
  ثابت بنانشانی coords_لمائی = axes.لمائی;

  صحيح *normalized = coords_لمائی ? (صحيح *) hb_calloc (coords_لمائی, حجم (صحيح)) : خالی_سوچک;
  عائم *design_coords = coords_لمائی ? (عائم *) hb_calloc (coords_لمائی, حجم (عائم)) : خالی_سوچک;

  إذا (unlikely (coords_لمائی && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  /* Initialize design coords. */
  لكل (بنانشانی صحيح i = 0; i < coords_لمائی; i++)
    design_coords[i] = axes[i].get_default ();
  إذا (font->instance_اشاریہ != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    بنانشانی گنتی = coords_لمائی;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_اشاریہ,
						&گنتی, design_coords);
  }

  لكل (بنانشانی صحيح i = 0; i < variations_لمائی; i++)
  {
    ثابت خودکار ٹیگ_2 = variations[i].ٹیگ_2;
    ثابت خودکار v = variations[i].value;
    لكل (بنانشانی axis_اشاریہ = 0; axis_اشاریہ < coords_لمائی; axis_اشاریہ++)
      إذا (axes[axis_اشاریہ].axis_ٹیگ == ٹیگ_2)
	design_coords[axis_اشاریہ] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_لمائی, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_لمائی);
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
hb_font_سیٹ_variation (hb_font_t *font,
		       hb_ٹیگ_t ٹیگ_2,
		       عائم    value)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  // TODO Share some of this code with set_variations()

  ثابت OT::fvar &fvar = *font->face->جدول.fvar;
  خودکار axes = fvar.get_axes ();
  ثابت بنانشانی coords_لمائی = axes.لمائی;

  صحيح *normalized = coords_لمائی ? (صحيح *) hb_calloc (coords_لمائی, حجم (صحيح)) : خالی_سوچک;
  عائم *design_coords = coords_لمائی ? (عائم *) hb_calloc (coords_لمائی, حجم (عائم)) : خالی_سوچک;

  إذا (unlikely (coords_لمائی && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  إذا (font->design_coords && font->عدد_coords == coords_لمائی)
  {
    لكل (بنانشانی صحيح i = 0; i < coords_لمائی; i++)
      design_coords[i] = font->design_coords[i];
  }
  وإلا
  {
    لكل (بنانشانی صحيح i = 0; i < coords_لمائی; i++)
      design_coords[i] = axes[i].get_default ();
    إذا (font->instance_اشاریہ != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      بنانشانی گنتی = coords_لمائی;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_اشاریہ,
						  &گنتی, design_coords);
    }
  }

  لكل (بنانشانی axis_اشاریہ = 0; axis_اشاریہ < coords_لمائی; axis_اشاریہ++)
    إذا (axes[axis_اشاریہ].axis_ٹیگ == ٹیگ_2)
      design_coords[axis_اشاریہ] = value;

  hb_ot_var_normalize_coords (font->face, coords_لمائی, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_لمائی);
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
hb_font_سیٹ_var_coords_design (hb_font_t    *font,
			       ثابت عائم  *coords,
			       بنانشانی صحيح  داخلہ_coords_لمائی)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->جدول.fvar;
  خودکار axes = fvar.get_axes ();
  ثابت بنانشانی coords_لمائی = axes.لمائی;

  داخلہ_coords_لمائی = hb_min (داخلہ_coords_لمائی, coords_لمائی);
  صحيح *normalized = coords_لمائی ? (صحيح *) hb_calloc (coords_لمائی, حجم (صحيح)) : خالی_سوچک;
  عائم *design_coords = coords_لمائی ? (عائم *) hb_calloc (coords_لمائی, حجم (عائم)) : خالی_سوچک;

  إذا (unlikely (coords_لمائی && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    أرجع;
  }

  إذا (داخلہ_coords_لمائی)
    hb_memcpy (design_coords, coords, داخلہ_coords_لمائی * حجم (font->design_coords[0]));
  // Fill in the rest with default values
  لكل (بنانشانی صحيح i = داخلہ_coords_لمائی; i < coords_لمائی; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_لمائی, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_لمائی);
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
hb_font_سیٹ_var_named_instance (hb_font_t *font,
				بنانشانی صحيح instance_اشاریہ)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  إذا (font->instance_اشاریہ == instance_اشاریہ)
    أرجع;

  font->instance_اشاریہ = instance_اشاریہ;
  hb_font_سیٹ_variations (font, خالی_سوچک, 0);
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
بنانشانی صحيح
hb_font_get_var_named_instance (hb_font_t *font)
{
  أرجع font->instance_اشاریہ;
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
hb_font_سیٹ_var_coords_normalized (hb_font_t    *font,
				   ثابت صحيح    *coords, /* 2.14 normalized */
				   بنانشانی صحيح  داخلہ_coords_لمائی)
{
  إذا (hb_object_is_immutable (font))
    أرجع;

  ثابت OT::fvar &fvar = *font->face->جدول.fvar;
  خودکار axes = fvar.get_axes ();
  بنانشانی coords_لمائی = axes.لمائی;

  داخلہ_coords_لمائی = hb_min (داخلہ_coords_لمائی, coords_لمائی);
  صحيح *copy = coords_لمائی ? (صحيح *) hb_calloc (coords_لمائی, حجم (coords[0])) : خالی_سوچک;
  عائم *design_coords = coords_لمائی ? (عائم *) hb_calloc (coords_لمائی, حجم (design_coords[0])) : خالی_سوچک;

  إذا (unlikely (coords_لمائی && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    أرجع;
  }

  إذا (داخلہ_coords_لمائی)
    hb_memcpy (copy, coords, داخلہ_coords_لمائی * حجم (coords[0]));

  لكل (بنانشانی صحيح i = 0; i < coords_لمائی; ++i)
    design_coords[i] = NAN;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_لمائی);
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
				   بنانشانی صحيح *لمائی)
{
  إذا (لمائی)
    *لمائی = font->عدد_coords;

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
			       بنانشانی صحيح *لمائی)
{
  إذا (لمائی)
    *لمائی = font->عدد_coords;

  أرجع font->design_coords;
}
#جے_انت

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

بنية hb_trampoline_گھیرا_t
{
  فراغ *user_انکڑے;
  hb_destroy_func_t destroy;
  بنانشانی صحيح ref_گنتی_2;
};

سانچا <قسم_ناں FuncType>
بنية hb_trampoline_t
{
  hb_trampoline_گھیرا_t گھیرا; /* Must be first. */
  FuncType func;
};

سانچا <قسم_ناں FuncType>
ساكن hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   فراغ              *user_انکڑے,
		   hb_destroy_func_t  destroy)
{
  تعريفنوع hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, حجم (trampoline_t));

  إذا (unlikely (!trampoline))
    أرجع خالی_سوچک;

  trampoline->گھیرا.user_انکڑے = user_انکڑے;
  trampoline->گھیرا.destroy = destroy;
  trampoline->گھیرا.ref_گنتی_2 = 1;
  trampoline->func = func;

  أرجع trampoline;
}

ساكن فراغ
trampoline_reference (hb_trampoline_گھیرا_t *گھیرا)
{
  گھیرا->ref_گنتی_2++;
}

ساكن فراغ
trampoline_destroy (فراغ *user_انکڑے)
{
  hb_trampoline_گھیرا_t *گھیرا = (hb_trampoline_گھیرا_t *) user_انکڑے;

  إذا (--گھیرا->ref_گنتی_2)
    أرجع;

  إذا (گھیرا->destroy)
    گھیرا->destroy (گھیرا->user_انکڑے);
  hb_free (گھیرا);
}

تعريفنوع hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

ساكن hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      فراغ           *font_انکڑے,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      فراغ           *user_انکڑے)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_انکڑے;
  أرجع trampoline->func (font, font_انکڑے, unicode, 0, glyph, trampoline->گھیرا.user_انکڑے);
}

ساكن hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					فراغ           *font_انکڑے,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					فراغ           *user_انکڑے)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_انکڑے;
  أرجع trampoline->func (font, font_انکڑے, unicode, variation_selector, glyph, trampoline->گھیرا.user_انکڑے);
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
hb_font_funcs_سیٹ_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      فراغ                     *user_انکڑے,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_انکڑے, destroy);
  إذا (unlikely (!trampoline))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }

  /* Since we pass it to two destroying functions. */
  trampoline_reference (&trampoline->گھیرا);

  hb_font_funcs_سیٹ_nominal_glyph_func (ffuncs,
					hb_font_get_nominal_glyph_trampoline,
					trampoline,
					trampoline_destroy);

  hb_font_funcs_سیٹ_variation_glyph_func (ffuncs,
					  hb_font_get_variation_glyph_trampoline,
					  trampoline,
					  trampoline_destroy);
}
#جے_انت


#جے_بنا_تعریف HB_DISABLE_DEPRECATED

بنية hb_draw_glyph_گھیرا_t
{
  hb_font_draw_glyph_func_t func;
  فراغ *user_انکڑے;
  hb_destroy_func_t destroy;
};
ساكن hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       فراغ            *font_انکڑے,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       فراغ            *draw_انکڑے,
			       فراغ            *user_انکڑے)
{
  hb_draw_glyph_گھیرا_t *گھیرا = (hb_draw_glyph_گھیرا_t *) user_انکڑے;
  گھیرا->func (font, font_انکڑے, glyph, draw_funcs, draw_انکڑے, گھیرا->user_انکڑے);
  أرجع سچ;
}
ساكن فراغ
hb_font_draw_glyph_گھیرا_destroy (فراغ *user_انکڑے)
{
  hb_draw_glyph_گھیرا_t *گھیرا = (hb_draw_glyph_گھیرا_t *) user_انکڑے;

  إذا (گھیرا->destroy)
    گھیرا->destroy (گھیرا->user_انکڑے);
  hb_free (گھیرا);
}
ساكن فراغ
hb_font_funcs_سیٹ_draw_glyph_func_2 (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    فراغ                      *user_انکڑے,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }
  hb_draw_glyph_گھیرا_t *گھیرا = (hb_draw_glyph_گھیرا_t *) hb_calloc (1, حجم (hb_draw_glyph_گھیرا_t));
  إذا (unlikely (!گھیرا))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }
  گھیرا->func = func;
  گھیرا->user_انکڑے = user_انکڑے;
  گھیرا->destroy = destroy;

  hb_font_funcs_سیٹ_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     گھیرا,
					     hb_font_draw_glyph_گھیرا_destroy);
}
فراغ
hb_font_funcs_سیٹ_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   فراغ                      *user_انکڑے,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  hb_font_funcs_سیٹ_draw_glyph_func_2 (ffuncs, func, user_انکڑے, destroy);
}
فراغ
hb_font_funcs_سیٹ_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   فراغ                           *user_انکڑے,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  hb_font_funcs_سیٹ_draw_glyph_func_2 (ffuncs, func, user_انکڑے, destroy);
}

بنية hb_paint_glyph_گھیرا_t
{
  hb_font_paint_glyph_func_t func;
  فراغ *user_انکڑے;
  hb_destroy_func_t destroy;
};
ساكن hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				فراغ *font_انکڑے,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				فراغ *paint_انکڑے,
				بنانشانی صحيح palette,
				hb_color_t foreground,
				فراغ *user_انکڑے)
{
  hb_paint_glyph_گھیرا_t *گھیرا = (hb_paint_glyph_گھیرا_t *) user_انکڑے;
  گھیرا->func (font, font_انکڑے, glyph, paint_funcs, paint_انکڑے, palette, foreground, گھیرا->user_انکڑے);
  أرجع سچ;
}
ساكن فراغ
hb_font_paint_glyph_گھیرا_destroy (فراغ *user_انکڑے)
{
  hb_paint_glyph_گھیرا_t *گھیرا = (hb_paint_glyph_گھیرا_t *) user_انکڑے;

  إذا (گھیرا->destroy)
    گھیرا->destroy (گھیرا->user_انکڑے);
  hb_free (گھیرا);
}
فراغ
hb_font_funcs_سیٹ_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    فراغ                      *user_انکڑے,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  إذا (hb_object_is_immutable (ffuncs))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }
  hb_paint_glyph_گھیرا_t *گھیرا = (hb_paint_glyph_گھیرا_t *) hb_calloc (1, حجم (hb_paint_glyph_گھیرا_t));
  إذا (unlikely (!گھیرا))
  {
    إذا (destroy)
      destroy (user_انکڑے);
    أرجع;
  }
  گھیرا->func = func;
  گھیرا->user_انکڑے = user_انکڑے;
  گھیرا->destroy = destroy;

  hb_font_funcs_سیٹ_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      گھیرا,
					      hb_font_paint_glyph_گھیرا_destroy);
}
#جے_انت
