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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-avar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-fvar-table.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ft.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FONTATIONS
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-fontations.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-coretext.h"
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.h"
#ਜੇ_ਅੰਤ


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

ਸਥਿਰ hb_bool_t
hb_font_get_font_h_extents_nil (hb_font_t         *font HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ              *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_font_extents_t *extents,
				ਖਾਲੀ              *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_memset (extents, 0, ਆਕਾਰ (*extents));
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_font_h_extents_default (hb_font_t         *font,
				    ਖਾਲੀ              *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    hb_font_extents_t *extents,
				    ਖਾਲੀ              *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_bool_t ret = font->ਮਾਪਾ->get_font_h_extents (extents, ਝੂਠ);
  ਜੇ (ret) {
    extents->ascender = font->ਮਾਪਾ_scale_y_ਦੂਰੀ (extents->ascender);
    extents->descender = font->ਮਾਪਾ_scale_y_ਦੂਰੀ (extents->descender);
    extents->ਸਤਰ_gap = font->ਮਾਪਾ_scale_y_ਦੂਰੀ (extents->ਸਤਰ_gap);
  }
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_font_get_font_v_extents_nil (hb_font_t         *font HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ              *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_font_extents_t *extents,
				ਖਾਲੀ              *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_memset (extents, 0, ਆਕਾਰ (*extents));
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_font_v_extents_default (hb_font_t         *font,
				    ਖਾਲੀ              *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    hb_font_extents_t *extents,
				    ਖਾਲੀ              *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_bool_t ret = font->ਮਾਪਾ->get_font_v_extents (extents, ਝੂਠ);
  ਜੇ (ret) {
    extents->ascender = font->ਮਾਪਾ_scale_x_ਦੂਰੀ (extents->ascender);
    extents->descender = font->ਮਾਪਾ_scale_x_ਦੂਰੀ (extents->descender);
    extents->ਸਤਰ_gap = font->ਮਾਪਾ_scale_x_ਦੂਰੀ (extents->ਸਤਰ_gap);
  }
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_font_get_nominal_glyph_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			       hb_codepoint_t  unicode HB_ਅਣਵਰਤਿਆ,
			       hb_codepoint_t *glyph,
			       ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  *glyph = 0;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_nominal_glyph_default (hb_font_t      *font,
				   ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   hb_codepoint_t  unicode,
				   hb_codepoint_t *glyph,
				   ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_nominal_glyphs_func_ਸੈੱਟ ())
  {
    ਵਾਪਸ font->get_nominal_glyphs (1, &unicode, 0, glyph, 0);
  }
  ਵਾਪਸ font->ਮਾਪਾ->get_nominal_glyph (unicode, glyph);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_font_get_nominal_glyphs_nil hb_font_get_nominal_glyphs_default

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_font_get_nominal_glyphs_default (hb_font_t            *font,
				    ਖਾਲੀ                 *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
				    ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          unicode_stride,
				    hb_codepoint_t       *ਪਹਿਲਾ_glyph,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyph_stride,
				    ਖਾਲੀ                 *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_nominal_glyph_func_ਸੈੱਟ ())
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!font->get_nominal_glyph (*ਪਹਿਲਾ_unicode, ਪਹਿਲਾ_glyph))
	ਵਾਪਸ i;

      ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

  ਵਾਪਸ font->ਮਾਪਾ->get_nominal_glyphs (ਗਿਣਤੀ_2,
					   ਪਹਿਲਾ_unicode, unicode_stride,
					   ਪਹਿਲਾ_glyph, glyph_stride);
}

ਸਥਿਰ hb_bool_t
hb_font_get_variation_glyph_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  unicode HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  variation_selector HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t *glyph,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  *glyph = 0;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_variation_glyph_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  unicode,
				     hb_codepoint_t  variation_selector,
				     hb_codepoint_t *glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->ਮਾਪਾ->get_variation_glyph (unicode, variation_selector, glyph);
}


ਸਥਿਰ hb_position_t
hb_font_get_glyph_h_advance_nil (hb_font_t      *font,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->x_scale;
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_h_advance_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_h_advances_func_ਸੈੱਟ ())
  {
    hb_position_t ret;
    font->get_glyph_h_advances (1, &glyph, 0, &ret, 0, ਝੂਠ);
    ਵਾਪਸ ret;
  }
  ਵਾਪਸ font->ਮਾਪਾ_scale_x_ਦੂਰੀ (font->ਮਾਪਾ->get_glyph_h_advance (glyph, ਝੂਠ));
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_v_advance_nil (hb_font_t      *font,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ -font->y_scale;
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_v_advance_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_v_advances_func_ਸੈੱਟ ())
  {
    hb_position_t ret;
    font->get_glyph_v_advances (1, &glyph, 0, &ret, 0, ਝੂਠ);
    ਵਾਪਸ ret;
  }
  ਵਾਪਸ font->ਮਾਪਾ_scale_y_ਦੂਰੀ (font->ਮਾਪਾ->get_glyph_v_advance (glyph, ਝੂਠ));
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_font_get_glyph_h_advances_nil hb_font_get_glyph_h_advances_default

ਸਥਿਰ ਖਾਲੀ
hb_font_get_glyph_h_advances_default (hb_font_t*            font,
				      ਖਾਲੀ*                 font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
				      ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyph_stride,
				      hb_position_t        *ਪਹਿਲਾ_advance,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          advance_stride,
				      ਖਾਲੀ                 *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_h_advance_func_ਸੈੱਟ ())
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = font->get_glyph_h_advance (*ਪਹਿਲਾ_glyph, ਝੂਠ);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

  font->ਮਾਪਾ->get_glyph_h_advances (ਗਿਣਤੀ_2,
				      ਪਹਿਲਾ_glyph, glyph_stride,
				      ਪਹਿਲਾ_advance, advance_stride,
				      ਝੂਠ);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    *ਪਹਿਲਾ_advance = font->ਮਾਪਾ_scale_x_ਦੂਰੀ (*ਪਹਿਲਾ_advance);
    ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
  }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_font_get_glyph_v_advances_nil hb_font_get_glyph_v_advances_default
ਸਥਿਰ ਖਾਲੀ
hb_font_get_glyph_v_advances_default (hb_font_t*            font,
				      ਖਾਲੀ*                 font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
				      ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          glyph_stride,
				      hb_position_t        *ਪਹਿਲਾ_advance,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          advance_stride,
				      ਖਾਲੀ                 *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_v_advance_func_ਸੈੱਟ ())
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      *ਪਹਿਲਾ_advance = font->get_glyph_v_advance (*ਪਹਿਲਾ_glyph, ਝੂਠ);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
    ਵਾਪਸ;
  }

  font->ਮਾਪਾ->get_glyph_v_advances (ਗਿਣਤੀ_2,
				      ਪਹਿਲਾ_glyph, glyph_stride,
				      ਪਹਿਲਾ_advance, advance_stride,
				      ਝੂਠ);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    *ਪਹਿਲਾ_advance = font->ਮਾਪਾ_scale_y_ਦੂਰੀ (*ਪਹਿਲਾ_advance);
    ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
  }
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_h_origin_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
				hb_position_t  *x,
				hb_position_t  *y,
				ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  *x = *y = 0;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_h_origin_default (hb_font_t      *font,
				    ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_h_origins_func_ਸੈੱਟ ())
  {
    ਵਾਪਸ font->get_glyph_h_origins (1, &glyph, 0, x, 0, y, 0, ਝੂਠ);
  }
  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_h_origin (glyph, x, y);
  ਜੇ (ret)
    font->ਮਾਪਾ_scale_position (x, y);
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_v_origin_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
				hb_position_t  *x,
				hb_position_t  *y,
				ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_v_origin_default (hb_font_t      *font,
				    ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    hb_codepoint_t  glyph,
				    hb_position_t  *x,
				    hb_position_t  *y,
				    ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_v_origins_func_ਸੈੱਟ ())
  {
    ਵਾਪਸ font->get_glyph_v_origins (1, &glyph, 0, x, 0, y, 0, ਝੂਠ);
  }
  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_v_origin (glyph, x, y);
  ਜੇ (ret)
    font->ਮਾਪਾ_scale_position (x, y);
  ਵਾਪਸ ret;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_font_get_glyph_h_origins_nil hb_font_get_glyph_h_origins_default

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_h_origins_default (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride HB_ਅਣਵਰਤਿਆ,
				     hb_position_t *ਪਹਿਲਾ_x,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ x_stride,
				     hb_position_t *ਪਹਿਲਾ_y,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ y_stride,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_h_origin_func_ਸੈੱਟ ())
  {
    hb_bool_t ret = ਸੱਚ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ret &= font->get_glyph_h_origin (*ਪਹਿਲਾ_glyph, ਪਹਿਲਾ_x, ਪਹਿਲਾ_y, ਝੂਠ);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }
    ਵਾਪਸ ret;
  }

  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_h_origins (ਗਿਣਤੀ_2,
						     ਪਹਿਲਾ_glyph, glyph_stride,
						     ਪਹਿਲਾ_x, x_stride,
						     ਪਹਿਲਾ_y, y_stride);
  ਜੇ (ret)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      font->ਮਾਪਾ_scale_position (ਪਹਿਲਾ_x, ਪਹਿਲਾ_y);
      ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }
  }
  ਵਾਪਸ ret;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_font_get_glyph_v_origins_nil hb_font_get_glyph_v_origins_default

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_v_origins_default (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride HB_ਅਣਵਰਤਿਆ,
				     hb_position_t *ਪਹਿਲਾ_x,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ x_stride,
				     hb_position_t *ਪਹਿਲਾ_y,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ y_stride,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (font->has_glyph_v_origin_func_ਸੈੱਟ ())
  {
    hb_bool_t ret = ਸੱਚ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ret &= font->get_glyph_v_origin (*ਪਹਿਲਾ_glyph, ਪਹਿਲਾ_x, ਪਹਿਲਾ_y, ਝੂਠ);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ_Unaligned<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }
    ਵਾਪਸ ret;
  }

  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_v_origins (ਗਿਣਤੀ_2,
						     ਪਹਿਲਾ_glyph, glyph_stride,
						     ਪਹਿਲਾ_x, x_stride,
						     ਪਹਿਲਾ_y, y_stride);
  ਜੇ (ret)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      font->ਮਾਪਾ_scale_position (ਪਹਿਲਾ_x, ਪਹਿਲਾ_y);
      ਪਹਿਲਾ_x = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_x, x_stride);
      ਪਹਿਲਾ_y = &Struct_At_ਵਿੱਥ_Unaligned<hb_position_t> (ਪਹਿਲਾ_y, y_stride);
    }
  }
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_h_kerning_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  ਖੱਬਾ_glyph HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  ਸੱਜਾ_glyph HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ 0;
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_h_kerning_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  ਖੱਬਾ_glyph,
				     hb_codepoint_t  ਸੱਜਾ_glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->ਮਾਪਾ_scale_x_ਦੂਰੀ (font->ਮਾਪਾ->get_glyph_h_kerning (ਖੱਬਾ_glyph, ਸੱਜਾ_glyph));
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
ਸਥਿਰ hb_position_t
hb_font_get_glyph_v_kerning_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  ਸਿਖਰ_glyph HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t  bottom_glyph HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ 0;
}

ਸਥਿਰ hb_position_t
hb_font_get_glyph_v_kerning_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  ਸਿਖਰ_glyph,
				     hb_codepoint_t  bottom_glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->ਮਾਪਾ_scale_y_ਦੂਰੀ (font->ਮਾਪਾ->get_glyph_v_kerning (ਸਿਖਰ_glyph, bottom_glyph));
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_extents_nil (hb_font_t          *font HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ               *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			       hb_codepoint_t      glyph HB_ਅਣਵਰਤਿਆ,
			       hb_glyph_extents_t *extents,
			       ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_memset (extents, 0, ਆਕਾਰ (*extents));
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_extents_default (hb_font_t          *font,
				   ਖਾਲੀ               *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   hb_codepoint_t      glyph,
				   hb_glyph_extents_t *extents,
				   ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_extents (glyph, extents, ਝੂਠ);
  ਜੇ (ret) {
    font->ਮਾਪਾ_scale_position (&extents->x_bearing, &extents->y_bearing);
    font->ਮਾਪਾ_scale_ਦੂਰੀ (&extents->ਚੌੜਾਈ, &extents->ਉਚਾਈ);
  }
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_contour_ਬਿੰਦੂ_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਬਿੰਦੂ_ਸੂਚਕਾਂਕ HB_ਅਣਵਰਤਿਆ,
				     hb_position_t  *x,
				     hb_position_t  *y,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  *x = *y = 0;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_contour_ਬਿੰਦੂ_default (hb_font_t      *font,
					 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
					 hb_codepoint_t  glyph,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
					 hb_position_t  *x,
					 hb_position_t  *y,
					 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_bool_t ret = font->ਮਾਪਾ->get_glyph_contour_ਬਿੰਦੂ (glyph, ਬਿੰਦੂ_ਸੂਚਕਾਂਕ, x, y, ਝੂਠ);
  ਜੇ (ret)
    font->ਮਾਪਾ_scale_position (x, y);
  ਵਾਪਸ ret;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_name_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			    hb_codepoint_t  glyph HB_ਅਣਵਰਤਿਆ,
			    ਅੱਖਰ           *name,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ,
			    ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਜੇ (ਮਾਪ) *name = '\0';
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_name_default (hb_font_t      *font,
				ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t  glyph,
				ਅੱਖਰ           *name,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ,
				ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->ਮਾਪਾ->get_glyph_name (glyph, name, ਮਾਪ);
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_from_name_nil (hb_font_t      *font HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 ਅਚਲ ਅੱਖਰ     *name HB_ਅਣਵਰਤਿਆ,
				 ਪੂਰਨ             len HB_ਅਣਵਰਤਿਆ, /* -1 means nul-terminated */
				 hb_codepoint_t *glyph,
				 ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  *glyph = 0;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_get_glyph_from_name_default (hb_font_t      *font,
				     ਖਾਲੀ           *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     ਅਚਲ ਅੱਖਰ     *name,
				     ਪੂਰਨ             len, /* -1 means nul-terminated */
				     hb_codepoint_t *glyph,
				     ਖਾਲੀ           *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ font->ਮਾਪਾ->get_glyph_from_name (name, len, glyph);
}

ਸਥਿਰ hb_bool_t
hb_font_draw_glyph_or_fail_nil (hb_font_t       *font HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ            *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t   glyph,
				hb_draw_funcs_t *draw_funcs,
				ਖਾਲੀ            *draw_ਅੰਕੜੇ,
				ਖਾਲੀ            *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_font_paint_glyph_or_fail_nil (hb_font_t *font HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t glyph HB_ਅਣਵਰਤਿਆ,
				 hb_paint_funcs_t *paint_funcs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette HB_ਅਣਵਰਤਿਆ,
				 hb_color_t foreground HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ਝੂਠ;
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_font_draw_glyph_default_adaptor_t {
  hb_draw_funcs_t *draw_funcs;
  ਖਾਲੀ		  *draw_ਅੰਕੜੇ;
  ਦਸ਼ਮਲਵ		   x_scale;
  ਦਸ਼ਮਲਵ		   y_scale;
} hb_font_draw_glyph_default_adaptor_t;

ਸਥਿਰ ਖਾਲੀ
hb_draw_move_ਮੰਜ਼ਿਲ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *draw_ਅੰਕੜੇ,
			 hb_draw_state_t *st,
			 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਦਸ਼ਮਲਵ x_scale = adaptor->x_scale;
  ਦਸ਼ਮਲਵ y_scale = adaptor->y_scale;

  adaptor->draw_funcs->emit_move_ਮੰਜ਼ਿਲ (adaptor->draw_ਅੰਕੜੇ, *st,
				     x_scale * ਮੰਜ਼ਿਲ_x, y_scale * ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			 hb_draw_state_t *st,
			 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਦਸ਼ਮਲਵ x_scale = adaptor->x_scale;
  ਦਸ਼ਮਲਵ y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_ਸਤਰ_ਮੰਜ਼ਿਲ (adaptor->draw_ਅੰਕੜੇ, *st,
				     x_scale * ਮੰਜ਼ਿਲ_x, y_scale * ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_quadratic_ਮੰਜ਼ਿਲ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			      hb_draw_state_t *st,
			      ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਦਸ਼ਮਲਵ x_scale = adaptor->x_scale;
  ਦਸ਼ਮਲਵ y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_quadratic_ਮੰਜ਼ਿਲ (adaptor->draw_ਅੰਕੜੇ, *st,
					  x_scale * control_x, y_scale * control_y,
					  x_scale * ਮੰਜ਼ਿਲ_x, y_scale * ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_cubic_ਮੰਜ਼ਿਲ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  hb_draw_state_t *st,
			  ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
			  ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਦਸ਼ਮਲਵ x_scale = adaptor->x_scale;
  ਦਸ਼ਮਲਵ y_scale = adaptor->y_scale;

  st->current_x = st->current_x * x_scale;
  st->current_y = st->current_y * y_scale;

  adaptor->draw_funcs->emit_cubic_ਮੰਜ਼ਿਲ (adaptor->draw_ਅੰਕੜੇ, *st,
				      x_scale * control1_x, y_scale * control1_y,
				      x_scale * control2_x, y_scale * control2_y,
				      x_scale * ਮੰਜ਼ਿਲ_x, y_scale * ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_draw_close_ਰਸਤਾ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			    hb_draw_state_t *st,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;

  adaptor->draw_funcs->emit_close_ਰਸਤਾ (adaptor->draw_ਅੰਕੜੇ, *st);
}

ਸਥਿਰ hb_bool_t
hb_draw_ਸੈੱਟ_budget_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			    int64_t budget,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਵਾਪਸ adaptor->draw_funcs->ਸੈੱਟ_budget (adaptor->draw_ਅੰਕੜੇ, budget);
}

ਸਥਿਰ int64_t
hb_draw_get_budget_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਵਾਪਸ adaptor->draw_funcs->get_budget (adaptor->draw_ਅੰਕੜੇ);
}

ਸਥਿਰ int64_t *
hb_draw_get_budget_ਬਚਿਆ_default (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *draw_ਅੰਕੜੇ,
				      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t *adaptor = (hb_font_draw_glyph_default_adaptor_t *) draw_ਅੰਕੜੇ;
  ਵਾਪਸ adaptor->draw_funcs->get_budget_ਬਚਿਆ_ptr (adaptor->draw_ਅੰਕੜੇ);
}

ਸਥਿਰ ਅਚਲ hb_draw_funcs_t _hb_draw_funcs_default = {
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DRAW_FUNC_IMPLEMENT(name) hb_draw_##name##_default,
    HB_DRAW_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DRAW_FUNC_IMPLEMENT
  }
};

ਸਥਿਰ hb_bool_t
hb_font_draw_glyph_or_fail_default (hb_font_t       *font,
				    ਖਾਲੀ            *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				    hb_codepoint_t   glyph,
				    hb_draw_funcs_t *draw_funcs,
				    ਖਾਲੀ            *draw_ਅੰਕੜੇ,
				    ਖਾਲੀ            *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_font_draw_glyph_default_adaptor_t adaptor = {
    draw_funcs,
    draw_ਅੰਕੜੇ,
    font->ਮਾਪਾ->x_scale ? (ਦਸ਼ਮਲਵ) font->x_scale / (ਦਸ਼ਮਲਵ) font->ਮਾਪਾ->x_scale : 0.f,
    font->ਮਾਪਾ->y_scale ? (ਦਸ਼ਮਲਵ) font->y_scale / (ਦਸ਼ਮਲਵ) font->ਮਾਪਾ->y_scale : 0.f
  };

  ਵਾਪਸ font->ਮਾਪਾ->draw_glyph_or_fail (glyph,
					   const_cast<hb_draw_funcs_t *> (&_hb_draw_funcs_default),
					   &adaptor,
					   ਝੂਠ);
}

ਸਥਿਰ hb_bool_t
hb_font_paint_glyph_or_fail_default (hb_font_t *font,
				     ਖਾਲੀ *font_ਅੰਕੜੇ,
				     hb_codepoint_t glyph,
				     hb_paint_funcs_t *paint_funcs,
				     ਖਾਲੀ *paint_ਅੰਕੜੇ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
				     hb_color_t foreground,
				     ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  paint_funcs->ਧੱਕੋ_transform (paint_ਅੰਕੜੇ,
    font->ਮਾਪਾ->x_scale ? (ਦਸ਼ਮਲਵ) font->x_scale / (ਦਸ਼ਮਲਵ) font->ਮਾਪਾ->x_scale : 0, 0,
    0, font->ਮਾਪਾ->y_scale ? (ਦਸ਼ਮਲਵ) font->y_scale / (ਦਸ਼ਮਲਵ) font->ਮਾਪਾ->y_scale : 0,
    0, 0);

  ਤਾਰਕਿਕ ret = font->ਮਾਪਾ->paint_glyph_or_fail (glyph, paint_funcs, paint_ਅੰਕੜੇ, palette, foreground);

  paint_funcs->ਕੱਢੋ_transform (paint_ਅੰਕੜੇ);

  ਵਾਪਸ ret;
}

DEFINE_NULL_INSTANCE (hb_font_funcs_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  ਖਾਲੀ_ਸੂਚਕ,
  ਖਾਲੀ_ਸੂਚਕ,
  {
    {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_nil,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
    }
  }
};

ਸਥਿਰ ਅਚਲ hb_font_funcs_t _hb_font_funcs_default = {
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  ਖਾਲੀ_ਸੂਚਕ,
  ਖਾਲੀ_ਸੂਚਕ,
  {
    {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) hb_font_##get_##name##_default,
      HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
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

  ਜੇ (!(ffuncs = hb_object_create<hb_font_funcs_t> ()))
    ਵਾਪਸ hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ffuncs->get = _hb_font_funcs_default.get;

  ਵਾਪਸ ffuncs;
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
hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_font_funcs_t *> (&_hb_font_funcs_default);
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
  ਵਾਪਸ hb_object_reference (ffuncs);
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
ਖਾਲੀ
hb_font_funcs_destroy (hb_font_funcs_t *ffuncs)
{
  ਜੇ (!hb_object_destroy (ffuncs)) ਵਾਪਸ;

  ਜੇ (ffuncs->destroy)
  {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) ਜੇ (ffuncs->destroy->name) \
    ffuncs->destroy->name (!ffuncs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : ffuncs->user_ਅੰਕੜੇ->name);
    HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT
  }

  hb_free (ffuncs->destroy);
  hb_free (ffuncs->user_ਅੰਕੜੇ);

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
hb_font_funcs_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_font_funcs_t    *ffuncs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			     ਖਾਲੀ *              ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy /* May be NULL. */,
			     hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (ffuncs, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_font_funcs_get_user_ਅੰਕੜੇ (ਅਚਲ hb_font_funcs_t *ffuncs,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t    *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (ffuncs, key);
}


/**
 * hb_font_funcs_make_immutable:
 * @ffuncs: The font-functions structure
 *
 * Makes a font-functions structure immutable.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_font_funcs_make_immutable (hb_font_funcs_t *ffuncs)
{
  ਜੇ (hb_object_is_immutable (ffuncs))
    ਵਾਪਸ;

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
  ਵਾਪਸ hb_object_is_immutable (ffuncs);
}


ਸਥਿਰ ਤਾਰਕਿਕ
hb_font_funcs_ਸੈੱਟ_preamble (hb_font_funcs_t    *ffuncs,
			     ਤਾਰਕਿਕ                func_is_null,
			     ਖਾਲੀ              **user_ਅੰਕੜੇ,
			     hb_destroy_func_t  *destroy)
{
  ਜੇ (hb_object_is_immutable (ffuncs))
  {
    ਜੇ (*destroy)
      (*destroy) (*user_ਅੰਕੜੇ);
    ਵਾਪਸ ਝੂਠ;
  }

  ਜੇ (func_is_null)
  {
    ਜੇ (*destroy)
      (*destroy) (*user_ਅੰਕੜੇ);
    *destroy = ਖਾਲੀ_ਸੂਚਕ;
    *user_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
hb_font_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (hb_font_funcs_t   *ffuncs,
			   ਖਾਲੀ              *user_ਅੰਕੜੇ,
			   hb_destroy_func_t  destroy)
{
  ਸਵੈਚਾਲਿਤ destroy_guard = hb_make_scope_guard ([&]() {
    ਜੇ (destroy) destroy (user_ਅੰਕੜੇ);
  });

  ਜੇ (user_ਅੰਕੜੇ && !ffuncs->user_ਅੰਕੜੇ)
  {
    ffuncs->user_ਅੰਕੜੇ = (decltype (ffuncs->user_ਅੰਕੜੇ)) hb_calloc (1, ਆਕਾਰ (*ffuncs->user_ਅੰਕੜੇ));
    ਜੇ (unlikely (!ffuncs->user_ਅੰਕੜੇ))
      ਵਾਪਸ ਝੂਠ;
  }
  ਜੇ (destroy && !ffuncs->destroy)
  {
    ffuncs->destroy = (decltype (ffuncs->destroy)) hb_calloc (1, ਆਕਾਰ (*ffuncs->destroy));
    ਜੇ (unlikely (!ffuncs->destroy))
      ਵਾਪਸ ਝੂਠ;
  }

  destroy_guard.release ();
  ਵਾਪਸ ਸੱਚ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FONT_FUNC_IMPLEMENT(get_,name) \
									 \
ਖਾਲੀ                                                                     \
hb_font_funcs_ਸੈੱਟ##name##_func (hb_font_funcs_t             *ffuncs,    \
				 hb_font_##get_##name##_func_t func,     \
				 ਖਾਲੀ                        *user_ਅੰਕੜੇ, \
				 hb_destroy_func_t            destroy)   \
{                                                                        \
  ਜੇ (!hb_font_funcs_ਸੈੱਟ_preamble (ffuncs, !func, &user_ਅੰਕੜੇ, &destroy))\
      ਵਾਪਸ;                                                            \
									 \
  ਜੇ (ffuncs->destroy && ffuncs->destroy->name)                          \
    ffuncs->destroy->name (!ffuncs->user_ਅੰਕੜੇ ? ਖਾਲੀ_ਸੂਚਕ : ffuncs->user_ਅੰਕੜੇ->name); \
                                                                         \
  ਜੇ (!hb_font_funcs_ਸੈੱਟ_ਵਿਚਕਾਰ (ffuncs, user_ਅੰਕੜੇ, destroy))           \
      ਵਾਪਸ;                                                            \
									 \
  ਜੇ (func)                                                              \
    ffuncs->get.f.name = func;                                           \
  ਨਹੀਂਤਾਂ                                                                   \
    ffuncs->get.f.name = hb_font_##get_##name##_default;                   \
									 \
  ਜੇ (ffuncs->user_ਅੰਕੜੇ)                                                 \
    ffuncs->user_ਅੰਕੜੇ->name = user_ਅੰਕੜੇ;                                 \
  ਜੇ (ffuncs->destroy)                                                   \
    ffuncs->destroy->name = destroy;                                     \
}

HB_FONT_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_FONT_FUNC_IMPLEMENT

ਤਾਰਕਿਕ
hb_font_t::has_func_ਸੈੱਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
{
  ਵਾਪਸ ਇਹ->klass->get.array[i] != _hb_font_funcs_default.get.array[i];
}

ਤਾਰਕਿਕ
hb_font_t::has_func (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i)
{
  ਵਾਪਸ has_func_ਸੈੱਟ (i) ||
	 (ਮਾਪਾ && ਮਾਪਾ != &_hb_Null_hb_font_t && ਮਾਪਾ->has_func (i));
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
  ਵਾਪਸ font->get_font_h_extents (extents);
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
  ਵਾਪਸ font->get_font_v_extents (extents);
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
  ਜੇ (unlikely (variation_selector))
    ਵਾਪਸ font->get_variation_glyph (unicode, variation_selector, glyph);
  ਵਾਪਸ font->get_nominal_glyph (unicode, glyph);
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
  ਵਾਪਸ font->get_nominal_glyph (unicode, glyph);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_font_get_nominal_glyphs (hb_font_t *font,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
			    ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
			    hb_codepoint_t *ਪਹਿਲਾ_glyph,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride)
{
  ਵਾਪਸ font->get_nominal_glyphs (ਗਿਣਤੀ_2,
				   ਪਹਿਲਾ_unicode, unicode_stride,
				   ਪਹਿਲਾ_glyph, glyph_stride);
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
  ਵਾਪਸ font->get_variation_glyph (unicode, variation_selector, glyph);
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
  ਵਾਪਸ font->get_glyph_h_advance (glyph);
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
  ਵਾਪਸ font->get_glyph_v_advance (glyph);
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
ਖਾਲੀ
hb_font_get_glyph_h_advances (hb_font_t*            font,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
			      ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ              glyph_stride,
			      hb_position_t        *ਪਹਿਲਾ_advance,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ              advance_stride)
{
  font->get_glyph_h_advances (ਗਿਣਤੀ_2, ਪਹਿਲਾ_glyph, glyph_stride, ਪਹਿਲਾ_advance, advance_stride);
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
ਖਾਲੀ
hb_font_get_glyph_v_advances (hb_font_t*            font,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
			      ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ              glyph_stride,
			      hb_position_t        *ਪਹਿਲਾ_advance,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ              advance_stride)
{
  font->get_glyph_v_advances (ਗਿਣਤੀ_2, ਪਹਿਲਾ_glyph, glyph_stride, ਪਹਿਲਾ_advance, advance_stride);
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
  ਵਾਪਸ font->get_glyph_h_origin (glyph, x, y);
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
  ਵਾਪਸ font->get_glyph_v_origin (glyph, x, y);
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
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਗਿਣਤੀ_2,
			     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    glyph_stride,
			     hb_position_t  *ਪਹਿਲਾ_x,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    x_stride,
			     hb_position_t  *ਪਹਿਲਾ_y,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    y_stride)

{
  ਵਾਪਸ font->get_glyph_h_origins (ਗਿਣਤੀ_2,
				    ਪਹਿਲਾ_glyph, glyph_stride,
				    ਪਹਿਲਾ_x, x_stride,
				    ਪਹਿਲਾ_y, y_stride);
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
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਗਿਣਤੀ_2,
			     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    glyph_stride,
			     hb_position_t  *ਪਹਿਲਾ_x,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    x_stride,
			     hb_position_t  *ਪਹਿਲਾ_y,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    y_stride)

{
  ਵਾਪਸ font->get_glyph_v_origins (ਗਿਣਤੀ_2,
				    ਪਹਿਲਾ_glyph, glyph_stride,
				    ਪਹਿਲਾ_x, x_stride,
				    ਪਹਿਲਾ_y, y_stride);
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
			     hb_codepoint_t  ਖੱਬਾ_glyph,
			     hb_codepoint_t  ਸੱਜਾ_glyph)
{
  ਵਾਪਸ font->get_glyph_h_kerning (ਖੱਬਾ_glyph, ਸੱਜਾ_glyph);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
			     hb_codepoint_t  ਸਿਖਰ_glyph,
			     hb_codepoint_t  bottom_glyph)
{
  ਵਾਪਸ font->get_glyph_v_kerning (ਸਿਖਰ_glyph, bottom_glyph);
}
#ਜੇ_ਅੰਤ

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
  ਵਾਪਸ font->get_glyph_extents (glyph, extents);
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
hb_font_get_glyph_contour_ਬਿੰਦੂ (hb_font_t      *font,
				 hb_codepoint_t  glyph,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
				 hb_position_t  *x,
				 hb_position_t  *y)
{
  ਵਾਪਸ font->get_glyph_contour_ਬਿੰਦੂ (glyph, ਬਿੰਦੂ_ਸੂਚਕਾਂਕ, x, y);
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
			ਅੱਖਰ           *name,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ)
{
  ਵਾਪਸ font->get_glyph_name (glyph, name, ਮਾਪ);
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
			     ਅਚਲ ਅੱਖਰ     *name,
			     ਪੂਰਨ             len, /* -1 means nul-terminated */
			     hb_codepoint_t *glyph)
{
  ਵਾਪਸ font->get_glyph_from_name (name, len, glyph);
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
ਖਾਲੀ
hb_font_get_glyph_shape (hb_font_t *font,
		         hb_codepoint_t glyph,
		         hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ)
{
  hb_font_draw_glyph (font, glyph, dfuncs, draw_ਅੰਕੜੇ);
}
#ਜੇ_ਅੰਤ

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
			    hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ)
{
  ਵਾਪਸ font->draw_glyph_or_fail (glyph, dfuncs, draw_ਅੰਕੜੇ);
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
			     hb_paint_funcs_t *pfuncs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
			     hb_color_t foreground)
{
  ਵਾਪਸ font->paint_glyph_or_fail (glyph, pfuncs, paint_ਅੰਕੜੇ, palette_ਸੂਚਕਾਂਕ, foreground);
}

/* A bit higher-level, and with fallback */

ਖਾਲੀ
hb_font_t::paint_glyph (hb_codepoint_t glyph,
			hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
			hb_color_t foreground)
{
  ਜੇ (paint_glyph_or_fail (glyph,
			   paint_funcs, paint_ਅੰਕੜੇ,
			   palette, foreground))
    ਵਾਪਸ;

  /* Fallback for outline glyph. */
  paint_funcs->fill_glyph (paint_ਅੰਕੜੇ, glyph, ਇਹ, ਸੱਚ, foreground);
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
ਖਾਲੀ
hb_font_draw_glyph (hb_font_t *font,
		    hb_codepoint_t glyph,
		    hb_draw_funcs_t *dfuncs, ਖਾਲੀ *draw_ਅੰਕੜੇ)
{
  (ਖਾਲੀ) hb_font_draw_glyph_or_fail (font, glyph, dfuncs, draw_ਅੰਕੜੇ);
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
ਖਾਲੀ
hb_font_paint_glyph (hb_font_t *font,
                     hb_codepoint_t glyph,
                     hb_paint_funcs_t *pfuncs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
                     hb_color_t foreground)
{
  font->paint_glyph (glyph, pfuncs, paint_ਅੰਕੜੇ, palette_ਸੂਚਕਾਂਕ, foreground);
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
ਖਾਲੀ
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
ਖਾਲੀ
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
HB_EXTERN ਖਾਲੀ
hb_font_get_glyph_advances_for_direction (hb_font_t*            font,
					  hb_direction_t        direction,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          ਗਿਣਤੀ_2,
					  ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ              glyph_stride,
					  hb_position_t        *ਪਹਿਲਾ_advance,
					  ਬਿਨਾਨਿਸ਼ਾਨੀ              advance_stride)
{
  font->get_glyph_advances_for_direction (direction, ਗਿਣਤੀ_2, ਪਹਿਲਾ_glyph, glyph_stride, ਪਹਿਲਾ_advance, advance_stride);
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
ਖਾਲੀ
hb_font_get_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  ਵਾਪਸ font->get_glyph_origin_for_direction (glyph, direction, x, y);
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
ਖਾਲੀ
hb_font_ਜੋੜੋ_glyph_origin_for_direction (hb_font_t      *font,
					hb_codepoint_t  glyph,
					hb_direction_t  direction,
					hb_position_t  *x,
					hb_position_t  *y)
{
  ਵਾਪਸ font->ਜੋੜੋ_glyph_origin_for_direction (glyph, direction, x, y);
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
ਖਾਲੀ
hb_font_ਘਟਾਓ_glyph_origin_for_direction (hb_font_t      *font,
					     hb_codepoint_t  glyph,
					     hb_direction_t  direction,
					     hb_position_t  *x,
					     hb_position_t  *y)
{
  ਵਾਪਸ font->ਘਟਾਓ_glyph_origin_for_direction (glyph, direction, x, y);
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
ਖਾਲੀ
hb_font_get_glyph_kerning_for_direction (hb_font_t      *font,
					 hb_codepoint_t  ਪਹਿਲਾ_glyph,
					 hb_codepoint_t  ਦੂਜਾ_glyph,
					 hb_direction_t  direction,
					 hb_position_t  *x,
					 hb_position_t  *y)
{
  ਵਾਪਸ font->get_glyph_kerning_for_direction (ਪਹਿਲਾ_glyph, ਦੂਜਾ_glyph, direction, x, y);
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
  ਵਾਪਸ font->get_glyph_extents_for_origin (glyph, direction, extents);
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
hb_font_get_glyph_contour_ਬਿੰਦੂ_for_origin (hb_font_t      *font,
					    hb_codepoint_t  glyph,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਬਿੰਦੂ_ਸੂਚਕਾਂਕ,
					    hb_direction_t  direction,
					    hb_position_t  *x,
					    hb_position_t  *y)
{
  ਵਾਪਸ font->get_glyph_contour_ਬਿੰਦੂ_for_origin (glyph, ਬਿੰਦੂ_ਸੂਚਕਾਂਕ, direction, x, y);
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
ਖਾਲੀ
hb_font_glyph_ਮੰਜ਼ਿਲ_string (hb_font_t      *font,
			 hb_codepoint_t  glyph,
			 ਅੱਖਰ           *s,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਮਾਪ)
{
  font->glyph_ਮੰਜ਼ਿਲ_string (glyph, s, ਮਾਪ);
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
			   ਅਚਲ ਅੱਖਰ     *s,
			   ਪੂਰਨ             len,
			   hb_codepoint_t *glyph)
{
  ਵਾਪਸ font->glyph_from_string (s, len, glyph);
}


/*
 * hb_font_t
 */

DEFINE_NULL_INSTANCE (hb_font_t) =
{
  HB_OBJECT_ਸਿਰਲੇਖ_STATIC,

  0, /* serial */
  0, /* serial_coords */

  ਖਾਲੀ_ਸੂਚਕ, /*ਮਾਪਾ*/
  const_cast<hb_face_t *> (&_hb_Null_hb_face_t),

  1000, /* x_scale */
  1000, /* y_scale */
  ਝੂਠ, /* is_synthetic */
  0.f, /* x_embolden */
  0.f, /* y_embolden */
  ਸੱਚ, /* embolden_in_place */
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
  ਝੂਠ, /* has_nonzero_coords */
  0, /* num_coords */
  ਖਾਲੀ_ਸੂਚਕ, /* coords */
  ਖਾਲੀ_ਸੂਚਕ, /* design_coords */

  const_cast<hb_font_funcs_t *> (&_hb_Null_hb_font_funcs_t),

  /* Zero for the rest is fine. */
};


ਸਥਿਰ hb_font_t *
_hb_font_create (hb_face_t *face)
{
  hb_font_t *font;

  ਜੇ (unlikely (!face))
    face = hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਜੇ (!(font = hb_object_create<hb_font_t> ()))
    ਵਾਪਸ hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_face_make_immutable (face);
  font->ਮਾਪਾ = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  font->face = hb_face_reference (face);
  font->klass = hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ();
  font->ਅੰਕੜੇ.init0 (font);
  font->x_scale = font->y_scale = face->get_upem ();
  font->embolden_ਵਿੱਚ_ਰੱਖੋ = ਸੱਚ;
  font->x_multf = font->y_multf = 1.f;
  font->x_mult = font->y_mult = 1 << 16;
  font->instance_ਸੂਚਕਾਂਕ = HB_FONT_NO_VAR_NAMED_INSTANCE;

  ਵਾਪਸ font;
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

  hb_font_ਸੈੱਟ_funcs_using (font, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
  // Initialize variations.
  ਜੇ (likely (face))
  {
    ਜੇ (face->ਸੂਚਕਾਂਕ_2 >> 16)
      hb_font_ਸੈੱਟ_var_named_instance (font, (face->ਸੂਚਕਾਂਕ_2 >> 16) - 1);
    ਨਹੀਂਤਾਂ
      hb_font_ਸੈੱਟ_variations (font, ਖਾਲੀ_ਸੂਚਕ, 0);
  }
#ਜੇ_ਅੰਤ

  ਵਾਪਸ font;
}

ਸਥਿਰ ਖਾਲੀ
_hb_font_adopt_var_coords (hb_font_t *font,
			   ਪੂਰਨ *coords, /* 2.14 normalized */
			   ਦਸ਼ਮਲਵ *design_coords,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coords_ਲੰਮਾਈ)
{
  hb_free (font->coords);
  hb_free (font->design_coords);

  font->coords = coords;
  font->design_coords = design_coords;
  font->ਅੰਕ_coords = coords_ਲੰਮਾਈ;
  font->has_nonzero_coords = hb_any (hb_array (coords, coords_ਲੰਮਾਈ));

  font->ਬਦਲਿਆ ();
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
hb_font_create_sub_font (hb_font_t *ਮਾਪਾ)
{
  ਜੇ (unlikely (!ਮਾਪਾ))
    ਮਾਪਾ = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_font_t *font = _hb_font_create (ਮਾਪਾ->face);

  ਜੇ (unlikely (hb_object_is_immutable (font)))
    ਵਾਪਸ font;

  font->ਮਾਪਾ = hb_font_reference (ਮਾਪਾ);

  font->x_scale = ਮਾਪਾ->x_scale;
  font->y_scale = ਮਾਪਾ->y_scale;
  font->x_embolden = ਮਾਪਾ->x_embolden;
  font->y_embolden = ਮਾਪਾ->y_embolden;
  font->embolden_ਵਿੱਚ_ਰੱਖੋ = ਮਾਪਾ->embolden_ਵਿੱਚ_ਰੱਖੋ;
  font->slant = ਮਾਪਾ->slant;
  font->x_ppem = ਮਾਪਾ->x_ppem;
  font->y_ppem = ਮਾਪਾ->y_ppem;
  font->ptem = ਮਾਪਾ->ptem;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords = ਮਾਪਾ->ਅੰਕ_coords;
  ਜੇ (ਅੰਕ_coords)
  {
    ਪੂਰਨ *coords = (ਪੂਰਨ *) hb_calloc (ਅੰਕ_coords, ਆਕਾਰ (ਮਾਪਾ->coords[0]));
    ਦਸ਼ਮਲਵ *design_coords = (ਦਸ਼ਮਲਵ *) hb_calloc (ਅੰਕ_coords, ਆਕਾਰ (ਮਾਪਾ->design_coords[0]));
    ਜੇ (likely (coords && design_coords))
    {
      hb_memcpy (coords, ਮਾਪਾ->coords, ਅੰਕ_coords * ਆਕਾਰ (ਮਾਪਾ->coords[0]));
      hb_memcpy (design_coords, ਮਾਪਾ->design_coords, ਅੰਕ_coords * ਆਕਾਰ (ਮਾਪਾ->design_coords[0]));
      _hb_font_adopt_var_coords (font, coords, design_coords, ਅੰਕ_coords);
    }
    ਨਹੀਂਤਾਂ
    {
      hb_free (coords);
      hb_free (design_coords);
    }
  }

  font->ਬਦਲਿਆ ();
  font->serial_coords = font->serial;

  ਵਾਪਸ font;
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
hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ()
{
  ਵਾਪਸ const_cast<hb_font_t *> (&Null (hb_font_t));
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
  ਵਾਪਸ hb_object_reference (font);
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
ਖਾਲੀ
hb_font_destroy (hb_font_t *font)
{
  ਜੇ (!hb_object_destroy (font)) ਵਾਪਸ;

  font->ਅੰਕੜੇ.fini ();

  ਜੇ (font->destroy)
    font->destroy (font->user_ਅੰਕੜੇ);

  hb_font_destroy (font->ਮਾਪਾ);
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
hb_font_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_font_t          *font,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
		       ਖਾਲੀ *              ਅੰਕੜੇ,
		       hb_destroy_func_t   destroy /* May be NULL. */,
		       hb_bool_t           replace)
{
  ਜੇ (!hb_object_is_immutable (font))
    font->ਬਦਲਿਆ ();

  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (font, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_font_get_user_ਅੰਕੜੇ (ਅਚਲ hb_font_t    *font,
		       hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (font, key);
}

/**
 * hb_font_make_immutable:
 * @font: #hb_font_t to work upon
 *
 * Makes @font immutable.
 *
 * Since: 0.9.2
 **/
ਖਾਲੀ
hb_font_make_immutable (hb_font_t *font)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->ਮਾਪਾ)
    hb_font_make_immutable (font->ਮਾਪਾ);

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
  ਵਾਪਸ hb_object_is_immutable (font);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_font_get_serial (hb_font_t *font)
{
  ਵਾਪਸ font->serial.get_acquire ();
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
ਖਾਲੀ
hb_font_ਬਦਲਿਆ (hb_font_t *font)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  font->ਬਦਲਿਆ ();
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_ਮਾਪਾ (hb_font_t *font,
		    hb_font_t *ਮਾਪਾ)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (ਮਾਪਾ == font->ਮਾਪਾ)
    ਵਾਪਸ;

  ਜੇ (!ਮਾਪਾ)
    ਮਾਪਾ = hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  ਲਈ (hb_font_t *p = ਮਾਪਾ; p && p != hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ(); p = p->ਮਾਪਾ)
    ਜੇ (p == font)
      ਵਾਪਸ; /* Would create a cycle - reject */

  hb_font_t *old = font->ਮਾਪਾ;

  font->ਮਾਪਾ = hb_font_reference (ਮਾਪਾ);

  hb_font_destroy (old);

  font->ਬਦਲਿਆ ();
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
hb_font_get_ਮਾਪਾ (hb_font_t *font)
{
  ਵਾਪਸ font->ਮਾਪਾ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_face (hb_font_t *font,
		  hb_face_t *face)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (face == font->face)
    ਵਾਪਸ;

  ਜੇ (unlikely (!face))
    face = hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_face_t *old = font->face;

  hb_face_make_immutable (face);
  font->face = hb_face_reference (face);
  font->ਬਦਲਿਆ ();

  /* Release the old font funcs while the old face is still alive. */
  hb_font_ਸੈੱਟ_funcs (font, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_font_ਸੈੱਟ_funcs_using (font, ਖਾਲੀ_ਸੂਚਕ);

  hb_face_destroy (old);

  font->ਬਦਲਿਆ ();
  font->serial_coords = font->serial;
}
#ਜੇ_ਅੰਤ

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
  ਵਾਪਸ font->face;
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_funcs (hb_font_t         *font,
		   hb_font_funcs_t   *klass,
		   ਖਾਲੀ              *font_ਅੰਕੜੇ,
		   hb_destroy_func_t  destroy /* May be NULL. */)
{
  ਜੇ (hb_object_is_immutable (font))
  {
    ਜੇ (destroy)
      destroy (font_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  ਜੇ (font->destroy)
    font->destroy (font->user_ਅੰਕੜੇ);

  ਜੇ (!klass)
    klass = hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_font_funcs_reference (klass);
  hb_font_funcs_destroy (font->klass);
  font->klass = klass;
  font->user_ਅੰਕੜੇ = font_ਅੰਕੜੇ;
  font->destroy = destroy;

  font->ਬਦਲਿਆ ();
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_funcs_ਅੰਕੜੇ (hb_font_t         *font,
		        ਖਾਲੀ              *font_ਅੰਕੜੇ,
		        hb_destroy_func_t  destroy /* May be NULL. */)
{
  /* Destroy user_data? */
  ਜੇ (hb_object_is_immutable (font))
  {
    ਜੇ (destroy)
      destroy (font_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  ਜੇ (font->destroy)
    font->destroy (font->user_ਅੰਕੜੇ);

  font->user_ਅੰਕੜੇ = font_ਅੰਕੜੇ;
  font->destroy = destroy;

  font->ਬਦਲਿਆ ();
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ supported_font_funcs_t {
	ਅੱਖਰ name[16];
	ਖਾਲੀ (*func) (hb_font_t *);
} supported_font_funcs[] =
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT
  {"ot",	hb_ot_font_ਸੈੱਟ_funcs},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FREETYPE
  {"ft",	hb_ft_font_ਸੈੱਟ_funcs},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_FONTATIONS
  {"fontations",hb_fontations_font_ਸੈੱਟ_funcs},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT
  {"coretext",	hb_coretext_font_ਸੈੱਟ_funcs},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE
  {"directwrite",hb_directwrite_font_ਸੈੱਟ_funcs},
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *get_default_funcs_name ()
{
  ਸਥਿਰ hb_atomic_t<ਅਚਲ ਅੱਖਰ *> static_funcs_name;
  ਅਚਲ ਅੱਖਰ *name = static_funcs_name.get_acquire ();
  ਜੇ (!name)
  {
    name = getenv ("HB_FONT_FUNCS");
    ਜੇ (!name)
      name = "";
    ਜੇ (!static_funcs_name.cmpexch (ਖਾਲੀ_ਸੂਚਕ, name))
      name = static_funcs_name.get_acquire ();
  }
  ਵਾਪਸ name;
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
hb_font_ਸੈੱਟ_funcs_using (hb_font_t  *font,
			 ਅਚਲ ਅੱਖਰ *name)
{
  ਜੇ (unlikely (hb_object_is_immutable (font)))
    ਵਾਪਸ ਝੂਠ;

  ਤਾਰਕਿਕ retry = ਝੂਠ;

  ਜੇ (!name || !*name)
  {
    name = get_default_funcs_name ();
    retry = ਸੱਚ;
  }
  ਜੇ (name && !*name) name = ਖਾਲੀ_ਸੂਚਕ;

retry:
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ARRAY_ਲੰਮਾਈ (supported_font_funcs); i++)
    ਜੇ (!name || strcmp (supported_font_funcs[i].name, name) == 0)
    {
      supported_font_funcs[i].func (font);
      ਜੇ (name || font->klass != hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ ())
	ਵਾਪਸ ਸੱਚ;
    }

  ਜੇ (retry)
  {
    retry = ਝੂਠ;
    name = ਖਾਲੀ_ਸੂਚਕ;
    ਜਾਓ retry;
  }

  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_font_funcs_list ();

ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ nil_font_funcs_list[] = {ਖਾਲੀ_ਸੂਚਕ};

ਸਥਿਰ ਬਣਤਰ hb_font_funcs_list_lazy_loader_t : hb_lazy_loader_t<ਅਚਲ ਅੱਖਰ *,
								  hb_font_funcs_list_lazy_loader_t>
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ** create ()
  {
    ਅਚਲ ਅੱਖਰ **font_funcs_list = (ਅਚਲ ਅੱਖਰ **) hb_calloc (1 + ARRAY_ਲੰਮਾਈ (supported_font_funcs), ਆਕਾਰ (ਅਚਲ ਅੱਖਰ *));
    ਜੇ (unlikely (!font_funcs_list))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਲਈ (i = 0; i < ARRAY_ਲੰਮਾਈ (supported_font_funcs); i++)
      font_funcs_list[i] = supported_font_funcs[i].name;
    font_funcs_list[i] = ਖਾਲੀ_ਸੂਚਕ;

    hb_atexit (free_static_font_funcs_list);

    ਵਾਪਸ font_funcs_list;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (ਅਚਲ ਅੱਖਰ **l)
  { hb_free (l); }
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ * ਅਚਲ * get_null ()
  { ਵਾਪਸ nil_font_funcs_list; }
} static_font_funcs_list;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_font_funcs_list ()
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
ਅਚਲ ਅੱਖਰ **
hb_font_list_funcs ()
{
  ਵਾਪਸ static_font_funcs_list.get_unconst ();
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_scale (hb_font_t *font,
		   ਪੂਰਨ        x_scale,
		   ਪੂਰਨ        y_scale)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->x_scale == x_scale && font->y_scale == y_scale)
    ਵਾਪਸ;

  font->x_scale = x_scale;
  font->y_scale = y_scale;

  font->ਬਦਲਿਆ ();
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
ਖਾਲੀ
hb_font_get_scale (hb_font_t *font,
		   ਪੂਰਨ       *x_scale,
		   ਪੂਰਨ       *y_scale)
{
  ਜੇ (x_scale) *x_scale = font->x_scale;
  ਜੇ (y_scale) *y_scale = font->y_scale;
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_ppem (hb_font_t    *font,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  x_ppem,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  y_ppem)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->x_ppem == x_ppem && font->y_ppem == y_ppem)
    ਵਾਪਸ;

  font->x_ppem = x_ppem;
  font->y_ppem = y_ppem;

  font->ਬਦਲਿਆ ();
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
ਖਾਲੀ
hb_font_get_ppem (hb_font_t    *font,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *x_ppem,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *y_ppem)
{
  ਜੇ (x_ppem) *x_ppem = font->x_ppem;
  ਜੇ (y_ppem) *y_ppem = font->y_ppem;
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_ptem (hb_font_t *font,
		  ਦਸ਼ਮਲਵ      ptem)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->ptem == ptem)
    ਵਾਪਸ;

  font->ptem = ptem;

  font->ਬਦਲਿਆ ();
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
ਦਸ਼ਮਲਵ
hb_font_get_ptem (hb_font_t *font)
{
  ਵਾਪਸ font->ptem;
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
  ਵਾਪਸ font->is_synthetic;
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_synthetic_bold (hb_font_t *font,
			    ਦਸ਼ਮਲਵ x_embolden,
			    ਦਸ਼ਮਲਵ y_embolden,
			    hb_bool_t in_place)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->x_embolden == x_embolden &&
      font->y_embolden == y_embolden &&
      font->embolden_ਵਿੱਚ_ਰੱਖੋ == (ਤਾਰਕਿਕ) in_place)
    ਵਾਪਸ;

  font->x_embolden = x_embolden;
  font->y_embolden = y_embolden;
  font->embolden_ਵਿੱਚ_ਰੱਖੋ = in_place;

  font->ਬਦਲਿਆ ();
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
ਖਾਲੀ
hb_font_get_synthetic_bold (hb_font_t *font,
			    ਦਸ਼ਮਲਵ *x_embolden,
			    ਦਸ਼ਮਲਵ *y_embolden,
			    hb_bool_t *in_place)
{
  ਜੇ (x_embolden) *x_embolden = font->x_embolden;
  ਜੇ (y_embolden) *y_embolden = font->y_embolden;
  ਜੇ (in_place) *in_place = font->embolden_ਵਿੱਚ_ਰੱਖੋ;
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
HB_EXTERN ਖਾਲੀ
hb_font_ਸੈੱਟ_synthetic_slant (hb_font_t *font, ਦਸ਼ਮਲਵ slant)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->slant == slant)
    ਵਾਪਸ;

  font->slant = slant;

  font->ਬਦਲਿਆ ();
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
HB_EXTERN ਦਸ਼ਮਲਵ
hb_font_get_synthetic_slant (hb_font_t *font)
{
  ਵਾਪਸ font->slant;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_variations (hb_font_t            *font,
			ਅਚਲ hb_variation_t *variations,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ          variations_ਲੰਮਾਈ)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਅਚਲ OT::fvar &fvar = *font->face->ਸਾਰਣੀ.fvar;
  ਸਵੈਚਾਲਿਤ axes = fvar.get_axes ();
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ coords_ਲੰਮਾਈ = axes.ਲੰਮਾਈ;

  ਪੂਰਨ *normalized = coords_ਲੰਮਾਈ ? (ਪੂਰਨ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਪੂਰਨ)) : ਖਾਲੀ_ਸੂਚਕ;
  ਦਸ਼ਮਲਵ *design_coords = coords_ਲੰਮਾਈ ? (ਦਸ਼ਮਲਵ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਦਸ਼ਮਲਵ)) : ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (coords_ਲੰਮਾਈ && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ਵਾਪਸ;
  }

  /* Initialize design coords. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < coords_ਲੰਮਾਈ; i++)
    design_coords[i] = axes[i].get_default ();
  ਜੇ (font->instance_ਸੂਚਕਾਂਕ != HB_FONT_NO_VAR_NAMED_INSTANCE)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = coords_ਲੰਮਾਈ;
    /* This may fail if index is out-of-range;
     * That's why we initialize design_coords from fvar above
     * unconditionally. */
    hb_ot_var_named_instance_get_design_coords (font->face, font->instance_ਸੂਚਕਾਂਕ,
						&ਗਿਣਤੀ_2, design_coords);
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < variations_ਲੰਮਾਈ; i++)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ ਟੈਗ_2 = variations[i].ਟੈਗ_2;
    ਅਚਲ ਸਵੈਚਾਲਿਤ v = variations[i].value;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਸੂਚਕਾਂਕ = 0; axis_ਸੂਚਕਾਂਕ < coords_ਲੰਮਾਈ; axis_ਸੂਚਕਾਂਕ++)
      ਜੇ (axes[axis_ਸੂਚਕਾਂਕ].axis_ਟੈਗ == ਟੈਗ_2)
	design_coords[axis_ਸੂਚਕਾਂਕ] = v;
  }

  hb_ot_var_normalize_coords (font->face, coords_ਲੰਮਾਈ, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_ਲੰਮਾਈ);
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_variation (hb_font_t *font,
		       hb_ਟੈਗ_t ਟੈਗ_2,
		       ਦਸ਼ਮਲਵ    value)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  // TODO Share some of this code with set_variations()

  ਅਚਲ OT::fvar &fvar = *font->face->ਸਾਰਣੀ.fvar;
  ਸਵੈਚਾਲਿਤ axes = fvar.get_axes ();
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ coords_ਲੰਮਾਈ = axes.ਲੰਮਾਈ;

  ਪੂਰਨ *normalized = coords_ਲੰਮਾਈ ? (ਪੂਰਨ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਪੂਰਨ)) : ਖਾਲੀ_ਸੂਚਕ;
  ਦਸ਼ਮਲਵ *design_coords = coords_ਲੰਮਾਈ ? (ਦਸ਼ਮਲਵ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਦਸ਼ਮਲਵ)) : ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (coords_ਲੰਮਾਈ && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ਵਾਪਸ;
  }

  /* Initialize design coords.
   * The font's coords may belong to another face after hb_font_set_face(). */
  ਜੇ (font->design_coords && font->ਅੰਕ_coords == coords_ਲੰਮਾਈ)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < coords_ਲੰਮਾਈ; i++)
      design_coords[i] = font->design_coords[i];
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < coords_ਲੰਮਾਈ; i++)
      design_coords[i] = axes[i].get_default ();
    ਜੇ (font->instance_ਸੂਚਕਾਂਕ != HB_FONT_NO_VAR_NAMED_INSTANCE)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = coords_ਲੰਮਾਈ;
      /* This may fail if index is out-of-range;
       * That's why we initialize design_coords from fvar above
       * unconditionally. */
      hb_ot_var_named_instance_get_design_coords (font->face, font->instance_ਸੂਚਕਾਂਕ,
						  &ਗਿਣਤੀ_2, design_coords);
    }
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ axis_ਸੂਚਕਾਂਕ = 0; axis_ਸੂਚਕਾਂਕ < coords_ਲੰਮਾਈ; axis_ਸੂਚਕਾਂਕ++)
    ਜੇ (axes[axis_ਸੂਚਕਾਂਕ].axis_ਟੈਗ == ਟੈਗ_2)
      design_coords[axis_ਸੂਚਕਾਂਕ] = value;

  hb_ot_var_normalize_coords (font->face, coords_ਲੰਮਾਈ, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_ਲੰਮਾਈ);
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_var_coords_design (hb_font_t    *font,
			       ਅਚਲ ਦਸ਼ਮਲਵ  *coords,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਅਚਲ OT::fvar &fvar = *font->face->ਸਾਰਣੀ.fvar;
  ਸਵੈਚਾਲਿਤ axes = fvar.get_axes ();
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ coords_ਲੰਮਾਈ = axes.ਲੰਮਾਈ;

  ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ = hb_min (ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ, coords_ਲੰਮਾਈ);
  ਪੂਰਨ *normalized = coords_ਲੰਮਾਈ ? (ਪੂਰਨ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਪੂਰਨ)) : ਖਾਲੀ_ਸੂਚਕ;
  ਦਸ਼ਮਲਵ *design_coords = coords_ਲੰਮਾਈ ? (ਦਸ਼ਮਲਵ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (ਦਸ਼ਮਲਵ)) : ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (coords_ਲੰਮਾਈ && !(normalized && design_coords)))
  {
    hb_free (normalized);
    hb_free (design_coords);
    ਵਾਪਸ;
  }

  ਜੇ (ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ)
    hb_memcpy (design_coords, coords, ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ * ਆਕਾਰ (font->design_coords[0]));
  // Fill in the rest with default values
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ; i < coords_ਲੰਮਾਈ; i++)
    design_coords[i] = axes[i].get_default ();

  hb_ot_var_normalize_coords (font->face, coords_ਲੰਮਾਈ, design_coords, normalized);
  _hb_font_adopt_var_coords (font, normalized, design_coords, coords_ਲੰਮਾਈ);
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_var_named_instance (hb_font_t *font,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instance_ਸੂਚਕਾਂਕ)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਜੇ (font->instance_ਸੂਚਕਾਂਕ == instance_ਸੂਚਕਾਂਕ)
    ਵਾਪਸ;

  font->instance_ਸੂਚਕਾਂਕ = instance_ਸੂਚਕਾਂਕ;
  hb_font_ਸੈੱਟ_variations (font, ਖਾਲੀ_ਸੂਚਕ, 0);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_font_get_var_named_instance (hb_font_t *font)
{
  ਵਾਪਸ font->instance_ਸੂਚਕਾਂਕ;
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
ਖਾਲੀ
hb_font_ਸੈੱਟ_var_coords_normalized (hb_font_t    *font,
				   ਅਚਲ ਪੂਰਨ    *coords, /* 2.14 normalized */
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ)
{
  ਜੇ (hb_object_is_immutable (font))
    ਵਾਪਸ;

  ਅਚਲ OT::fvar &fvar = *font->face->ਸਾਰਣੀ.fvar;
  ਸਵੈਚਾਲਿਤ axes = fvar.get_axes ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ coords_ਲੰਮਾਈ = axes.ਲੰਮਾਈ;

  ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ = hb_min (ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ, coords_ਲੰਮਾਈ);
  ਪੂਰਨ *copy = coords_ਲੰਮਾਈ ? (ਪੂਰਨ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (coords[0])) : ਖਾਲੀ_ਸੂਚਕ;
  ਦਸ਼ਮਲਵ *design_coords = coords_ਲੰਮਾਈ ? (ਦਸ਼ਮਲਵ *) hb_calloc (coords_ਲੰਮਾਈ, ਆਕਾਰ (design_coords[0])) : ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (coords_ਲੰਮਾਈ && !(copy && design_coords)))
  {
    hb_free (copy);
    hb_free (design_coords);
    ਵਾਪਸ;
  }

  ਜੇ (ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ)
    hb_memcpy (copy, coords, ਨਿਵੇਸ਼_coords_ਲੰਮਾਈ * ਆਕਾਰ (coords[0]));

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < coords_ਲੰਮਾਈ; ++i)
    design_coords[i] = NAN;

  _hb_font_adopt_var_coords (font, copy, design_coords, coords_ਲੰਮਾਈ);
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
ਅਚਲ ਪੂਰਨ *
hb_font_get_var_coords_normalized (hb_font_t    *font,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (ਲੰਮਾਈ)
    *ਲੰਮਾਈ = font->ਅੰਕ_coords;

  ਵਾਪਸ font->coords;
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
ਅਚਲ ਦਸ਼ਮਲਵ *
hb_font_get_var_coords_design (hb_font_t *font,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਲੰਮਾਈ)
{
  ਜੇ (ਲੰਮਾਈ)
    *ਲੰਮਾਈ = font->ਅੰਕ_coords;

  ਵਾਪਸ font->design_coords;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
/*
 * Deprecated get_glyph_func():
 */

ਬਣਤਰ hb_trampoline_ਘੇਰਾ_t
{
  ਖਾਲੀ *user_ਅੰਕੜੇ;
  hb_destroy_func_t destroy;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ref_ਗਿਣਤੀ_2;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FuncType>
ਬਣਤਰ hb_trampoline_t
{
  hb_trampoline_ਘੇਰਾ_t ਘੇਰਾ; /* Must be first. */
  FuncType func;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ FuncType>
ਸਥਿਰ hb_trampoline_t<FuncType> *
trampoline_create (FuncType           func,
		   ਖਾਲੀ              *user_ਅੰਕੜੇ,
		   hb_destroy_func_t  destroy)
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_trampoline_t<FuncType> trampoline_t;

  trampoline_t *trampoline = (trampoline_t *) hb_calloc (1, ਆਕਾਰ (trampoline_t));

  ਜੇ (unlikely (!trampoline))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  trampoline->ਘੇਰਾ.user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  trampoline->ਘੇਰਾ.destroy = destroy;
  trampoline->ਘੇਰਾ.ref_ਗਿਣਤੀ_2 = 1;
  trampoline->func = func;

  ਵਾਪਸ trampoline;
}

ਸਥਿਰ ਖਾਲੀ
trampoline_reference (hb_trampoline_ਘੇਰਾ_t *ਘੇਰਾ)
{
  ਘੇਰਾ->ref_ਗਿਣਤੀ_2++;
}

ਸਥਿਰ ਖਾਲੀ
trampoline_destroy (ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_trampoline_ਘੇਰਾ_t *ਘੇਰਾ = (hb_trampoline_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;

  ਜੇ (--ਘੇਰਾ->ref_ਗਿਣਤੀ_2)
    ਵਾਪਸ;

  ਜੇ (ਘੇਰਾ->destroy)
    ਘੇਰਾ->destroy (ਘੇਰਾ->user_ਅੰਕੜੇ);
  hb_free (ਘੇਰਾ);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_trampoline_t<hb_font_get_glyph_func_t> hb_font_get_glyph_trampoline_t;

ਸਥਿਰ hb_bool_t
hb_font_get_nominal_glyph_trampoline (hb_font_t      *font,
				      ਖਾਲੀ           *font_ਅੰਕੜੇ,
				      hb_codepoint_t  unicode,
				      hb_codepoint_t *glyph,
				      ਖਾਲੀ           *user_ਅੰਕੜੇ)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_ਅੰਕੜੇ;
  ਵਾਪਸ trampoline->func (font, font_ਅੰਕੜੇ, unicode, 0, glyph, trampoline->ਘੇਰਾ.user_ਅੰਕੜੇ);
}

ਸਥਿਰ hb_bool_t
hb_font_get_variation_glyph_trampoline (hb_font_t      *font,
					ਖਾਲੀ           *font_ਅੰਕੜੇ,
					hb_codepoint_t  unicode,
					hb_codepoint_t  variation_selector,
					hb_codepoint_t *glyph,
					ਖਾਲੀ           *user_ਅੰਕੜੇ)
{
  hb_font_get_glyph_trampoline_t *trampoline = (hb_font_get_glyph_trampoline_t *) user_ਅੰਕੜੇ;
  ਵਾਪਸ trampoline->func (font, font_ਅੰਕੜੇ, unicode, variation_selector, glyph, trampoline->ਘੇਰਾ.user_ਅੰਕੜੇ);
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
ਖਾਲੀ
hb_font_funcs_ਸੈੱਟ_glyph_func (hb_font_funcs_t          *ffuncs,
			      hb_font_get_glyph_func_t  func,
			      ਖਾਲੀ                     *user_ਅੰਕੜੇ,
			      hb_destroy_func_t         destroy /* May be NULL. */)
{
  ਜੇ (hb_object_is_immutable (ffuncs))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  hb_font_get_glyph_trampoline_t *trampoline;

  trampoline = trampoline_create (func, user_ਅੰਕੜੇ, destroy);
  ਜੇ (unlikely (!trampoline))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }

  /* Since we pass it to two destroying functions. */
  trampoline_reference (&trampoline->ਘੇਰਾ);

  hb_font_funcs_ਸੈੱਟ_nominal_glyph_func (ffuncs,
					hb_font_get_nominal_glyph_trampoline,
					trampoline,
					trampoline_destroy);

  hb_font_funcs_ਸੈੱਟ_variation_glyph_func (ffuncs,
					  hb_font_get_variation_glyph_trampoline,
					  trampoline,
					  trampoline_destroy);
}
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED

ਬਣਤਰ hb_draw_glyph_ਘੇਰਾ_t
{
  hb_font_draw_glyph_func_t func;
  ਖਾਲੀ *user_ਅੰਕੜੇ;
  hb_destroy_func_t destroy;
};
ਸਥਿਰ hb_bool_t
hb_font_draw_glyph_trampoline (hb_font_t       *font,
			       ਖਾਲੀ            *font_ਅੰਕੜੇ,
			       hb_codepoint_t   glyph,
			       hb_draw_funcs_t *draw_funcs,
			       ਖਾਲੀ            *draw_ਅੰਕੜੇ,
			       ਖਾਲੀ            *user_ਅੰਕੜੇ)
{
  hb_draw_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_draw_glyph_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;
  ਘੇਰਾ->func (font, font_ਅੰਕੜੇ, glyph, draw_funcs, draw_ਅੰਕੜੇ, ਘੇਰਾ->user_ਅੰਕੜੇ);
  ਵਾਪਸ ਸੱਚ;
}
ਸਥਿਰ ਖਾਲੀ
hb_font_draw_glyph_ਘੇਰਾ_destroy (ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_draw_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_draw_glyph_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;

  ਜੇ (ਘੇਰਾ->destroy)
    ਘੇਰਾ->destroy (ਘੇਰਾ->user_ਅੰਕੜੇ);
  hb_free (ਘੇਰਾ);
}
ਸਥਿਰ ਖਾਲੀ
hb_font_funcs_ਸੈੱਟ_draw_glyph_func_2 (hb_font_funcs_t           *ffuncs,
				    hb_font_draw_glyph_func_t  func,
				    ਖਾਲੀ                      *user_ਅੰਕੜੇ,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  ਜੇ (hb_object_is_immutable (ffuncs))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }
  hb_draw_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_draw_glyph_ਘੇਰਾ_t *) hb_calloc (1, ਆਕਾਰ (hb_draw_glyph_ਘੇਰਾ_t));
  ਜੇ (unlikely (!ਘੇਰਾ))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }
  ਘੇਰਾ->func = func;
  ਘੇਰਾ->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  ਘੇਰਾ->destroy = destroy;

  hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (ffuncs,
					     hb_font_draw_glyph_trampoline,
					     ਘੇਰਾ,
					     hb_font_draw_glyph_ਘੇਰਾ_destroy);
}
ਖਾਲੀ
hb_font_funcs_ਸੈੱਟ_draw_glyph_func (hb_font_funcs_t           *ffuncs,
                                   hb_font_draw_glyph_func_t  func,
                                   ਖਾਲੀ                      *user_ਅੰਕੜੇ,
                                   hb_destroy_func_t          destroy /* May be NULL. */)
{
  hb_font_funcs_ਸੈੱਟ_draw_glyph_func_2 (ffuncs, func, user_ਅੰਕੜੇ, destroy);
}
ਖਾਲੀ
hb_font_funcs_ਸੈੱਟ_glyph_shape_func (hb_font_funcs_t               *ffuncs,
                                   hb_font_get_glyph_shape_func_t  func,
                                   ਖਾਲੀ                           *user_ਅੰਕੜੇ,
                                   hb_destroy_func_t               destroy /* May be NULL. */)
{
  hb_font_funcs_ਸੈੱਟ_draw_glyph_func_2 (ffuncs, func, user_ਅੰਕੜੇ, destroy);
}

ਬਣਤਰ hb_paint_glyph_ਘੇਰਾ_t
{
  hb_font_paint_glyph_func_t func;
  ਖਾਲੀ *user_ਅੰਕੜੇ;
  hb_destroy_func_t destroy;
};
ਸਥਿਰ hb_bool_t
hb_font_paint_glyph_trampoline (hb_font_t        *font,
				ਖਾਲੀ *font_ਅੰਕੜੇ,
				hb_codepoint_t glyph,
				hb_paint_funcs_t *paint_funcs,
				ਖਾਲੀ *paint_ਅੰਕੜੇ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette,
				hb_color_t foreground,
				ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_paint_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_paint_glyph_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;
  ਘੇਰਾ->func (font, font_ਅੰਕੜੇ, glyph, paint_funcs, paint_ਅੰਕੜੇ, palette, foreground, ਘੇਰਾ->user_ਅੰਕੜੇ);
  ਵਾਪਸ ਸੱਚ;
}
ਸਥਿਰ ਖਾਲੀ
hb_font_paint_glyph_ਘੇਰਾ_destroy (ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_paint_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_paint_glyph_ਘੇਰਾ_t *) user_ਅੰਕੜੇ;

  ਜੇ (ਘੇਰਾ->destroy)
    ਘੇਰਾ->destroy (ਘੇਰਾ->user_ਅੰਕੜੇ);
  hb_free (ਘੇਰਾ);
}
ਖਾਲੀ
hb_font_funcs_ਸੈੱਟ_paint_glyph_func (hb_font_funcs_t           *ffuncs,
				    hb_font_paint_glyph_func_t  func,
				    ਖਾਲੀ                      *user_ਅੰਕੜੇ,
				    hb_destroy_func_t          destroy /* May be NULL. */)
{
  ਜੇ (hb_object_is_immutable (ffuncs))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }
  hb_paint_glyph_ਘੇਰਾ_t *ਘੇਰਾ = (hb_paint_glyph_ਘੇਰਾ_t *) hb_calloc (1, ਆਕਾਰ (hb_paint_glyph_ਘੇਰਾ_t));
  ਜੇ (unlikely (!ਘੇਰਾ))
  {
    ਜੇ (destroy)
      destroy (user_ਅੰਕੜੇ);
    ਵਾਪਸ;
  }
  ਘੇਰਾ->func = func;
  ਘੇਰਾ->user_ਅੰਕੜੇ = user_ਅੰਕੜੇ;
  ਘੇਰਾ->destroy = destroy;

  hb_font_funcs_ਸੈੱਟ_paint_glyph_or_fail_func (ffuncs,
					      hb_font_paint_glyph_trampoline,
					      ਘੇਰਾ,
					      hb_font_paint_glyph_ਘੇਰਾ_destroy);
}
#ਜੇ_ਅੰਤ
