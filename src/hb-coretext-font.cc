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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_CORETEXT

#ಅನ್ತರ್ಭಾವ "hb-coretext.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-layout-trak-table.hh"

#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

#ಯದಿ (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       ಶೂನ್ಯ *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  unsigned ಎಣಿಕೆ = 0;

  ಯದಿ (unicode <= 0xFFFF)
  {
    ch[ಎಣಿಕೆ++] = unicode;
  }
  ಅನ್ಯಥಾ ಯದಿ (unicode <= 0x10FFFF)
  {
    ch[ಎಣಿಕೆ++] = (unicode >> 10) + 0xD7C0;
    ch[ಎಣಿಕೆ++] = (unicode & 0x3FF) + 0xDC00;
  }
  ಅನ್ಯಥಾ
    ch[ಎಣಿಕೆ++] = 0xFFFD;

  ಯದಿ (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ಎಣಿಕೆ))
  {
    *glyph = cg_glyph[0];
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				ಶೂನ್ಯ *font_data,
				unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				ಧ್ರುವ hb_codepoint_t *first_unicode,
				unsigned ಪೂರ್ಣಾಙ್ಕ unicode_stride,
				hb_codepoint_t *first_glyph,
				unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  ತಾರ್ಕಿಕ slow_path = ಅಸತ್ಯ;
  auto *unicode = first_unicode;
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
  {
    ಯದಿ (*unicode > 0xFFFF)
    {
      slow_path = ಸತ್ಯ;
      ಭಙ್ಗ;
    }
    unicode = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (unicode, unicode_stride);
  }

  ಯದಿ (unlikely (slow_path))
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಯದಿ (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	ಪ್ರತ್ಯಾಗಮ i;
      first_unicode = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ಪೂರ್ಣಾಙ್ಕ) MAX_GLYPHS, (ಪೂರ್ಣಾಙ್ಕ) ಎಣಿಕೆ - (ಪೂರ್ಣಾಙ್ಕ) i);
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_unicode, unicode_stride);
    }
    ಯದಿ (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      ಪ್ರತಿ (unsigned j = 0; j < c; j++)
      {
	ಯದಿ (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  ಪ್ರತ್ಯಾಗಮ i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 ಶೂನ್ಯ *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  unsigned ಎಣಿಕೆ = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  ಯದಿ (unicode <= 0xFFFF)
    ch[ಎಣಿಕೆ++] = unicode;
  ಅನ್ಯಥಾ ಯದಿ (unicode <= 0x10FFFF)
  {
    ch[ಎಣಿಕೆ++] = (unicode >> 10) + 0xD7C0;
    ch[ಎಣಿಕೆ++] = (unicode & 0x3FF) + 0xDC00;
  }
  ಅನ್ಯಥಾ
    ch[ಎಣಿಕೆ++] = 0xFFFD;

  ಯದಿ (variation_selector <= 0xFFFF)
    ch[ಎಣಿಕೆ++] = variation_selector;
  ಅನ್ಯಥಾ ಯದಿ (variation_selector <= 0x10FFFF)
  {
    ch[ಎಣಿಕೆ++] = (variation_selector >> 10) + 0xD7C0;
    ch[ಎಣಿಕೆ++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  ಅನ್ಯಥಾ
    ch[ಎಣಿಕೆ++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ಎಣಿಕೆ);

  // All except for first should be zero if we succeeded
  ಪ್ರತಿ (unsigned i = 1; i < ಎಣಿಕೆ; i++)
    ಯದಿ (cg_glyph[i])
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  ಶೂನ್ಯ* font_data HB_UNUSED,
				  unsigned ಎಣಿಕೆ,
				  ಧ್ರುವ hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ಪೂರ್ಣಾಙ್ಕ) MAX_GLYPHS, (ಪೂರ್ಣಾಙ್ಕ) ಎಣಿಕೆ - (ಪೂರ್ಣಾಙ್ಕ) i);
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (ದ್ವಿಗುಣ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
ಸ್ಥಿರ ಶೂನ್ಯ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  ಶೂನ್ಯ* font_data HB_UNUSED,
				  unsigned ಎಣಿಕೆ,
				  ಧ್ರುವ hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ಪೂರ್ಣಾಙ್ಕ) MAX_GLYPHS, (ಪೂರ್ಣಾಙ್ಕ) ಎಣಿಕೆ - (ಪೂರ್ಣಾಙ್ಕ) i);
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    ಪ್ರತಿ (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (ದ್ವಿಗುಣ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				ಶೂನ್ಯ *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  ಧ್ರುವ CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       ಶೂನ್ಯ *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, ರಿಕ್ತ, 1);

  ದ್ವಿಗುಣ y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				ಶೂನ್ಯ *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->ಸಾಲು_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

#ifndef HB_NO_DRAW

ಸ್ಥಿರ ಶೂನ್ಯ
ct_apply_func (ಶೂನ್ಯ *info, ಧ್ರುವ CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  ವಿಕಲ್ಪ (element->type)
  {
  ಪಕ್ಷ kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    ಭಙ್ಗ;
  ಪಕ್ಷ k_CGPath_Element_Add_ಸಾಲು_To_Point:
    draws->ಸಾಲು_to (element->points[0].x, element->points[0].y);
    ಭಙ್ಗ;
  ಪಕ್ಷ kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    ಭಙ್ಗ;
  ಪಕ್ಷ kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    ಭಙ್ಗ;
  ಪಕ್ಷ kCGPathElementCloseSubpath:
    draws->close_path ();
    ಭಙ್ಗ;
  }
}

ಸ್ಥಿರ hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				ಶೂನ್ಯ *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, ಶೂನ್ಯ *draw_data,
				ಶೂನ್ಯ *user_data)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  ಯದಿ (!path)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    ಶೂನ್ಯ *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    ವರ್ಣ *name, unsigned ಪೂರ್ಣಾಙ್ಕ size,
			    ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  ಯದಿ (!cf_name)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (!size)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  CFIndex len = CFStringGetLength (cf_name);
  ಯದಿ (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, ಅಸತ್ಯ,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 ಶೂನ್ಯ *font_data HB_UNUSED,
				 ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ len,
				 hb_codepoint_t *glyph,
				 ಶೂನ್ಯ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.coretext;

  ಯದಿ (len == -1)
    len = strlen (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (ಧ್ರುವ UInt8 *) name, len,
							     kCFStringEncodingUTF8, ಅಸತ್ಯ));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  ಪ್ರತ್ಯಾಗಮ cg_glyph != 0;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_coretext_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, nullptr, nullptr);
    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, nullptr, nullptr);
    hb_font_funcs_set_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, nullptr, nullptr);

#ifndef HB_NO_VERTICAL
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, nullptr, nullptr);
#endif

#ifndef HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, nullptr, nullptr);
#endif

    hb_font_funcs_set_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, nullptr, nullptr);

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
    hb_font_funcs_set_glyph_name_func (funcs, hb_coretext_get_glyph_name, nullptr, nullptr);
    hb_font_funcs_set_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, nullptr, nullptr);
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_coretext_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

ಸ್ಥಿರ hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_coretext_funcs.get_unconst ();
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
ಶೂನ್ಯ
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  ಯದಿ (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    ಪ್ರತ್ಯಾಗಮ;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
