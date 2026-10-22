/*
 * Copyright © 2024  Google, Inc.
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
 * Author(s): Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-coretext.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-trak-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਜੇ (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_GLYPHS 256u

ਸਥਿਰ hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;

  ਜੇ (unicode <= 0xFFFF)
  {
    ch[ਗਿਣਤੀ_2++] = unicode;
  }
  ਨਹੀਂਤਾਂ ਜੇ (unicode <= 0x10FFFF)
  {
    ch[ਗਿਣਤੀ_2++] = (unicode >> 10) + 0xD7C0;
    ch[ਗਿਣਤੀ_2++] = (unicode & 0x3FF) + 0xDC00;
  }
  ਨਹੀਂਤਾਂ
    ch[ਗਿਣਤੀ_2++] = 0xFFFD;

  ਜੇ (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ਗਿਣਤੀ_2))
  {
    *glyph = cg_glyph[0];
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				ਖਾਲੀ *font_ਅੰਕੜੇ,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
				hb_codepoint_t *ਪਹਿਲਾ_glyph,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  ਤਾਰਕਿਕ slow_ਰਸਤਾ = ਝੂਠ;
  ਸਵੈਚਾਲਿਤ *unicode = ਪਹਿਲਾ_unicode;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (*unicode > 0xFFFF)
    {
      slow_ਰਸਤਾ = ਸੱਚ;
      ਰੋਕੋ;
    }
    unicode = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (unicode, unicode_stride);
  }

  ਜੇ (unlikely (slow_ਰਸਤਾ))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਜੇ (!hb_coretext_get_nominal_glyph (font, font_ਅੰਕੜੇ, *ਪਹਿਲਾ_unicode, ਪਹਿਲਾ_glyph, ਖਾਲੀ_ਸੂਚਕ))
	ਵਾਪਸ i;
      ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i += MAX_GLYPHS)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((ਪੂਰਨ) MAX_GLYPHS, (ਪੂਰਨ) ਗਿਣਤੀ_2 - (ਪੂਰਨ) i);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      ch[j] = *ਪਹਿਲਾ_unicode;
      ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
    }
    ਜੇ (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
      {
	ਜੇ (!hb_coretext_get_nominal_glyph (font, font_ਅੰਕੜੇ, ch[j], ਪਹਿਲਾ_glyph, ਖਾਲੀ_ਸੂਚਕ))
	  ਵਾਪਸ i + j;
	ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
      }
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      *ਪਹਿਲਾ_glyph = cg_glyph[j];
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
  }

  ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਸਥਿਰ hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  ਜੇ (unicode <= 0xFFFF)
    ch[ਗਿਣਤੀ_2++] = unicode;
  ਨਹੀਂਤਾਂ ਜੇ (unicode <= 0x10FFFF)
  {
    ch[ਗਿਣਤੀ_2++] = (unicode >> 10) + 0xD7C0;
    ch[ਗਿਣਤੀ_2++] = (unicode & 0x3FF) + 0xDC00;
  }
  ਨਹੀਂਤਾਂ
    ch[ਗਿਣਤੀ_2++] = 0xFFFD;

  ਜੇ (variation_selector <= 0xFFFF)
    ch[ਗਿਣਤੀ_2++] = variation_selector;
  ਨਹੀਂਤਾਂ ਜੇ (variation_selector <= 0x10FFFF)
  {
    ch[ਗਿਣਤੀ_2++] = (variation_selector >> 10) + 0xD7C0;
    ch[ਗਿਣਤੀ_2++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  ਨਹੀਂਤਾਂ
    ch[ਗਿਣਤੀ_2++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ਗਿਣਤੀ_2);

  // All except for first should be zero if we succeeded
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < ਗਿਣਤੀ_2; i++)
    ਜੇ (cg_glyph[i])
      ਵਾਪਸ ਝੂਠ;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  ਖਾਲੀ* font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
				  ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
				  hb_position_t *ਪਹਿਲਾ_advance,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ਮਾਪ;
  hb_position_t tracking = font->face->ਸਾਰਣੀ.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i += MAX_GLYPHS)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((ਪੂਰਨ) MAX_GLYPHS, (ਪੂਰਨ) ਗਿਣਤੀ_2 - (ਪੂਰਨ) i);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      cg_glyph[j] = *ਪਹਿਲਾ_glyph;
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      *ਪਹਿਲਾ_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
	  round (advances[j].ਚੌੜਾਈ * x_mult) - (ਦੁੱਗਣਾ) tracking);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
  }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
ਸਥਿਰ ਖਾਲੀ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  ਖਾਲੀ* font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
				  ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
				  hb_position_t *ਪਹਿਲਾ_advance,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_ਮਾਪ;
  hb_position_t tracking = font->face->ਸਾਰਣੀ.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i += MAX_GLYPHS)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ c = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_min ((ਪੂਰਨ) MAX_GLYPHS, (ਪੂਰਨ) ਗਿਣਤੀ_2 - (ਪੂਰਨ) i);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      cg_glyph[j] = *ਪਹਿਲਾ_glyph;
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < c; j++)
    {
      *ਪਹਿਲਾ_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
	  round (advances[j].ਚੌੜਾਈ * y_mult) - (ਦੁੱਗਣਾ) tracking);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }
  }
}

ਸਥਿਰ hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_ਮਾਪ;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_ਮਾਪ;

  ਅਚਲ CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (x_mult * origin.ਚੌੜਾਈ));
  *y = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (y_mult * origin.ਉਚਾਈ));

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ਮਾਪ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ਮਾਪ;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, ਨਿਰਾ, 1);

  ਦੁੱਗਣਾ y_bearing = round ((bounds.origin.y + bounds.ਮਾਪ.ਉਚਾਈ) * y_mult);

  extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (y_bearing);
  extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (bounds.ਮਾਪ.ਚੌੜਾਈ * x_mult));
  extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_font_extents_t *metrics,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ਮਾਪ;

  metrics->ascender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->ਸਤਰ_gap = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW

ਸਥਿਰ ਖਾਲੀ
ct_apply_func (ਖਾਲੀ *info, ਅਚਲ CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  ਚੋਣ (element->type)
  {
  ਹਾਲਤ k_CGPath_Element_Move_ਮੰਜ਼ਿਲ_ਬਿੰਦੂ:
    draws->move_ਮੰਜ਼ਿਲ (element->points[0].x, element->points[0].y);
    ਰੋਕੋ;
  ਹਾਲਤ k_CGPath_Element_ਜੋੜੋ_ਸਤਰ_ਮੰਜ਼ਿਲ_ਬਿੰਦੂ:
    draws->ਸਤਰ_ਮੰਜ਼ਿਲ (element->points[0].x, element->points[0].y);
    ਰੋਕੋ;
  ਹਾਲਤ k_CGPath_Element_ਜੋੜੋ_Quad_ਵਕਰ_ਮੰਜ਼ਿਲ_ਬਿੰਦੂ:
    draws->quadratic_ਮੰਜ਼ਿਲ (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    ਰੋਕੋ;
  ਹਾਲਤ k_CGPath_Element_ਜੋੜੋ_ਵਕਰ_ਮੰਜ਼ਿਲ_ਬਿੰਦੂ:
    draws->cubic_ਮੰਜ਼ਿਲ (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    ਰੋਕੋ;
  ਹਾਲਤ kCGPathElementCloseSubpath:
    draws->close_ਰਸਤਾ ();
    ਰੋਕੋ;
  }
}

ਸਥਿਰ hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
				ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ਮਾਪ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ਮਾਪ;

  CGAffineTransform transform = CGAffine_Transform_ਪਛਾਣ_ਤੱਤ;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> ਰਸਤਾ (CTFont_Create_ਰਸਤਾ_For_Glyph (ct_font, glyph, &transform));
  ਜੇ (!ਰਸਤਾ)
    ਵਾਪਸ ਝੂਠ;

  hb_draw_session_t drawing {draw_funcs, draw_ਅੰਕੜੇ};

  CGPathApply (ਰਸਤਾ, &drawing, ct_apply_func);

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
			    hb_codepoint_t glyph,
			    ਅੱਖਰ *name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CGFontRef cg_font = (CGFontRef) (ਅਚਲ ਖਾਲੀ *) font->face->ਅੰਕੜੇ.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  ਜੇ (!cf_name)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (!ਮਾਪ)
    ਵਾਪਸ ਸੱਚ;

  CFIndex len = CFString_Get_ਲੰਮਾਈ (cf_name);
  ਜੇ (len > (CFIndex)ਮਾਪ - 1)
    len = (CFIndex)ਮਾਪ - 1;

  CFString_Get_ਬਾਈਟਾਂ (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, ਝੂਠ,
		    (UInt8 *) name, ਮਾਪ, &len);

  name[len] = '\0';

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				 ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
				 hb_codepoint_t *glyph,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  ਜੇ (len == -1)
    len = ਲੜੀ_ਲੰਬਾਈ (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_ਬਾਈਟਾਂ (kCFAllocatorDefault,
							     (ਅਚਲ UInt8 *) name, len,
							     kCFStringEncodingUTF8, ਝੂਠ));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  ਵਾਪਸ cg_glyph != 0;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_coretext_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_ਸੈੱਟ_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_font_funcs_ਸੈੱਟ_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    hb_font_funcs_ਸੈੱਟ_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
    hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

    hb_font_funcs_ਸੈੱਟ_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_GLYPH_ਨਾਂ
    hb_font_funcs_ਸੈੱਟ_glyph_name_func (funcs, hb_coretext_get_glyph_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    ਵਾਪਸ funcs;
  }
} static_coretext_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

ਸਥਿਰ hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  ਵਾਪਸ static_coretext_funcs.get_unconst ();
}


/**
 * hb_coretext_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Configures the font-functions structure of the specified
 * #hb_font_t font object to use CoreText font functions.
 *
 * In particular, you can use this function to configure an
 * existing #hb_face_t face object for use with CoreText font
 * functions even if that #hb_face_t face object was initially
 * created with hb_face_create(), and therefore was not
 * initially configured to use CoreText font functions.
 *
 * <note>Note: Internally, this function creates a CTFont.
* </note>
 *
 * Since: 10.1.0
 **/
ਖਾਲੀ
hb_coretext_font_ਸੈੱਟ_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  ਜੇ (unlikely (!ct_font))
  {
    hb_font_ਸੈੱਟ_funcs (font,
		       hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ (),
		       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ;
  }

  hb_font_ਸੈੱਟ_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MAX_GLYPHS

#ਜੇ_ਅੰਤ
