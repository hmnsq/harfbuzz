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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_CORETEXT

#ଅନ୍ତର୍ଭାଵ "hb-coretext.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-layout-trak-table.hh"

#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

#ଯଦି (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

ସ୍ଥିର hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       ଶୂନ୍ୟ *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  unsigned ଗଣନା_2 = 0;

  ଯଦି (unicode <= 0xFFFF)
  {
    ch[ଗଣନା_2++] = unicode;
  }
  ଅନ୍ୟଥା ଯଦି (unicode <= 0x10FFFF)
  {
    ch[ଗଣନା_2++] = (unicode >> 10) + 0xD7C0;
    ch[ଗଣନା_2++] = (unicode & 0x3FF) + 0xDC00;
  }
  ଅନ୍ୟଥା
    ch[ଗଣନା_2++] = 0xFFFD;

  ଯଦି (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ଗଣନା_2))
  {
    *glyph = cg_glyph[0];
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				ଶୂନ୍ୟ *font_data,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				ଅଚଳ hb_codepoint_t *first_unicode,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
				hb_codepoint_t *first_glyph,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  ତାର୍କିକ slow_path = ମିଥ୍ୟା;
  auto *unicode = first_unicode;
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
  {
    ଯଦି (*unicode > 0xFFFF)
    {
      slow_path = ସତ୍ୟ;
      ଭାଙ୍ଗ;
    }
    unicode = &StructAtOffset<ଅଚଳ hb_codepoint_t> (unicode, unicode_stride);
  }

  ଯଦି (unlikely (slow_path))
  {
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଯଦି (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	ଫେରନ୍ତୁ i;
      first_unicode = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) MAX_GLYPHS, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଗଣନା_2 - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i);
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_unicode, unicode_stride);
    }
    ଯଦି (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      ପାଇଁ (unsigned j = 0; j < c; j++)
      {
	ଯଦି (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  ଫେରନ୍ତୁ i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  ଫେରନ୍ତୁ ଗଣନା_2;
}

ସ୍ଥିର hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 ଶୂନ୍ୟ *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  unsigned ଗଣନା_2 = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  ଯଦି (unicode <= 0xFFFF)
    ch[ଗଣନା_2++] = unicode;
  ଅନ୍ୟଥା ଯଦି (unicode <= 0x10FFFF)
  {
    ch[ଗଣନା_2++] = (unicode >> 10) + 0xD7C0;
    ch[ଗଣନା_2++] = (unicode & 0x3FF) + 0xDC00;
  }
  ଅନ୍ୟଥା
    ch[ଗଣନା_2++] = 0xFFFD;

  ଯଦି (variation_selector <= 0xFFFF)
    ch[ଗଣନା_2++] = variation_selector;
  ଅନ୍ୟଥା ଯଦି (variation_selector <= 0x10FFFF)
  {
    ch[ଗଣନା_2++] = (variation_selector >> 10) + 0xD7C0;
    ch[ଗଣନା_2++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  ଅନ୍ୟଥା
    ch[ଗଣନା_2++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ଗଣନା_2);

  // All except for first should be zero if we succeeded
  ପାଇଁ (unsigned i = 1; i < ଗଣନା_2; i++)
    ଯଦି (cg_glyph[i])
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  ଶୂନ୍ୟ* font_data HB_UNUSED,
				  unsigned ଗଣନା_2,
				  ଅଚଳ hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) MAX_GLYPHS, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଗଣନା_2 - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i);
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (ଦ୍ୱିଗୁଣ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
ସ୍ଥିର ଶୂନ୍ୟ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  ଶୂନ୍ୟ* font_data HB_UNUSED,
				  unsigned ଗଣନା_2,
				  ଅଚଳ hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) MAX_GLYPHS, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଗଣନା_2 - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i);
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    ପାଇଁ (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (ଦ୍ୱିଗୁଣ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

ସ୍ଥିର hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				ଶୂନ୍ୟ *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  ଅଚଳ CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ସ୍ଥିର hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       ଶୂନ୍ୟ *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, ଖାଲି, 1);

  ଦ୍ୱିଗୁଣ y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				ଶୂନ୍ୟ *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->ଧାଡ଼ି_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

#ifndef HB_NO_DRAW

ସ୍ଥିର ଶୂନ୍ୟ
ct_apply_func (ଶୂନ୍ୟ *info, ଅଚଳ CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  ବାଛ (element->type)
  {
  ସ୍ଥିତି kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି k_CGPath_Element_Add_ଧାଡ଼ି_To_Point:
    draws->ଧାଡ଼ି_to (element->points[0].x, element->points[0].y);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    ଭାଙ୍ଗ;
  ସ୍ଥିତି kCGPathElementCloseSubpath:
    draws->close_path ();
    ଭାଙ୍ଗ;
  }
}

ସ୍ଥିର hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				ଶୂନ୍ୟ *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, ଶୂନ୍ୟ *draw_data,
				ଶୂନ୍ୟ *user_data)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  ଯଦି (!path)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ସ୍ଥିର hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    ଶୂନ୍ୟ *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    ଅକ୍ଷର *name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  ଯଦି (!cf_name)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (!size)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  CFIndex len = CFStringGetLength (cf_name);
  ଯଦି (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, ମିଥ୍ୟା,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 ଶୂନ୍ୟ *font_data HB_UNUSED,
				 ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
				 hb_codepoint_t *glyph,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ଅଚଳ ଶୂନ୍ୟ *) font->data.coretext;

  ଯଦି (len == -1)
    len = strlen (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (ଅଚଳ UInt8 *) name, len,
							     kCFStringEncodingUTF8, ମିଥ୍ୟା));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  ଫେରନ୍ତୁ cg_glyph != 0;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_coretext_funcs ();

ସ୍ଥିର ସଂରଚନା hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_font_funcs_t *create ()
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

    ଫେରନ୍ତୁ funcs;
  }
} static_coretext_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

ସ୍ଥିର hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  ଫେରନ୍ତୁ static_coretext_funcs.get_unconst ();
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
ଶୂନ୍ୟ
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  ଯଦି (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    ଫେରନ୍ତୁ;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
