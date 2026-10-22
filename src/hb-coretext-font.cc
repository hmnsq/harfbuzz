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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_CORETEXT

#അന്തര്ഭാവ "hb-coretext.hh"
#അന്തര്ഭാവ "hb-aat-layout-trak-table.hh"

#അന്തര്ഭാവ "hb-draw.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-machinery.hh"

#എങ്കിൽ (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

സ്ഥിതം hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       ശൂന്യം *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  unsigned എണ്ണം = 0;

  എങ്കിൽ (unicode <= 0xFFFF)
  {
    ch[എണ്ണം++] = unicode;
  }
  അല്ലെങ്കിൽ എങ്കിൽ (unicode <= 0x10FFFF)
  {
    ch[എണ്ണം++] = (unicode >> 10) + 0xD7C0;
    ch[എണ്ണം++] = (unicode & 0x3FF) + 0xDC00;
  }
  അല്ലെങ്കിൽ
    ch[എണ്ണം++] = 0xFFFD;

  എങ്കിൽ (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, എണ്ണം))
  {
    *glyph = cg_glyph[0];
    തിരികെ സത്യം;
  }
  തിരികെ അസത്യം;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				ശൂന്യം *font_data,
				unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				സ്ഥിരം hb_codepoint_t *first_unicode,
				unsigned പൂർണ്ണസംഖ്യ unicode_stride,
				hb_codepoint_t *first_glyph,
				unsigned പൂർണ്ണസംഖ്യ glyph_stride,
				ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  തർക്കം slow_path = അസത്യം;
  auto *unicode = first_unicode;
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
  {
    എങ്കിൽ (*unicode > 0xFFFF)
    {
      slow_path = സത്യം;
      നിർത്തുക;
    }
    unicode = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (unicode, unicode_stride);
  }

  എങ്കിൽ (unlikely (slow_path))
  {
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      എങ്കിൽ (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	തിരികെ i;
      first_unicode = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    തിരികെ എണ്ണം;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((പൂർണ്ണസംഖ്യ) MAX_GLYPHS, (പൂർണ്ണസംഖ്യ) എണ്ണം - (പൂർണ്ണസംഖ്യ) i);
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_unicode, unicode_stride);
    }
    എങ്കിൽ (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      വേണ്ടി (unsigned j = 0; j < c; j++)
      {
	എങ്കിൽ (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  തിരികെ i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  തിരികെ എണ്ണം;
}

സ്ഥിതം hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 ശൂന്യം *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  unsigned എണ്ണം = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  എങ്കിൽ (unicode <= 0xFFFF)
    ch[എണ്ണം++] = unicode;
  അല്ലെങ്കിൽ എങ്കിൽ (unicode <= 0x10FFFF)
  {
    ch[എണ്ണം++] = (unicode >> 10) + 0xD7C0;
    ch[എണ്ണം++] = (unicode & 0x3FF) + 0xDC00;
  }
  അല്ലെങ്കിൽ
    ch[എണ്ണം++] = 0xFFFD;

  എങ്കിൽ (variation_selector <= 0xFFFF)
    ch[എണ്ണം++] = variation_selector;
  അല്ലെങ്കിൽ എങ്കിൽ (variation_selector <= 0x10FFFF)
  {
    ch[എണ്ണം++] = (variation_selector >> 10) + 0xD7C0;
    ch[എണ്ണം++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  അല്ലെങ്കിൽ
    ch[എണ്ണം++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, എണ്ണം);

  // All except for first should be zero if we succeeded
  വേണ്ടി (unsigned i = 1; i < എണ്ണം; i++)
    എങ്കിൽ (cg_glyph[i])
      തിരികെ അസത്യം;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  ശൂന്യം* font_data HB_UNUSED,
				  unsigned എണ്ണം,
				  സ്ഥിരം hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((പൂർണ്ണസംഖ്യ) MAX_GLYPHS, (പൂർണ്ണസംഖ്യ) എണ്ണം - (പൂർണ്ണസംഖ്യ) i);
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (ഇരട്ട) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
സ്ഥിതം ശൂന്യം
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  ശൂന്യം* font_data HB_UNUSED,
				  unsigned എണ്ണം,
				  സ്ഥിരം hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  വേണ്ടി (unsigned i = 0; i < എണ്ണം; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((പൂർണ്ണസംഖ്യ) MAX_GLYPHS, (പൂർണ്ണസംഖ്യ) എണ്ണം - (പൂർണ്ണസംഖ്യ) i);
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    വേണ്ടി (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (ഇരട്ട) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

സ്ഥിതം hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				ശൂന്യം *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  സ്ഥിരം CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  തിരികെ സത്യം;
}
#endif

സ്ഥിതം hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       ശൂന്യം *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, ശൂന്യ, 1);

  ഇരട്ട y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  തിരികെ സത്യം;
}

സ്ഥിതം hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				ശൂന്യം *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->വരി_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  തിരികെ സത്യം;
}

#ifndef HB_NO_DRAW

സ്ഥിതം ശൂന്യം
ct_apply_func (ശൂന്യം *info, സ്ഥിരം CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  തിരഞ്ഞെടുപ്പ് (element->type)
  {
  അവസ്ഥ kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    നിർത്തുക;
  അവസ്ഥ k_CGPath_Element_Add_വരി_To_Point:
    draws->വരി_to (element->points[0].x, element->points[0].y);
    നിർത്തുക;
  അവസ്ഥ kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    നിർത്തുക;
  അവസ്ഥ kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    നിർത്തുക;
  അവസ്ഥ kCGPathElementCloseSubpath:
    draws->close_path ();
    നിർത്തുക;
  }
}

സ്ഥിതം hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				ശൂന്യം *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, ശൂന്യം *draw_data,
				ശൂന്യം *user_data)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  എങ്കിൽ (!path)
    തിരികെ അസത്യം;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  തിരികെ സത്യം;
}
#endif

സ്ഥിതം hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    ശൂന്യം *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    അക്ഷരം *name, unsigned പൂർണ്ണസംഖ്യ size,
			    ശൂന്യം *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (സ്ഥിരം ശൂന്യം *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  എങ്കിൽ (!cf_name)
    തിരികെ അസത്യം;

  എങ്കിൽ (!size)
    തിരികെ സത്യം;

  CFIndex len = CFStringGetLength (cf_name);
  എങ്കിൽ (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, അസത്യം,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  തിരികെ സത്യം;
}

സ്ഥിതം hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 ശൂന്യം *font_data HB_UNUSED,
				 സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ len,
				 hb_codepoint_t *glyph,
				 ശൂന്യം *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  എങ്കിൽ (len == -1)
    len = strlen (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (സ്ഥിരം UInt8 *) name, len,
							     kCFStringEncodingUTF8, അസത്യം));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  തിരികെ cg_glyph != 0;
}


സ്ഥിതം വരിയിൽ ശൂന്യം free_static_coretext_funcs ();

സ്ഥിതം ഘടന hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_font_funcs_t *create ()
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

    തിരികെ funcs;
  }
} static_coretext_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

സ്ഥിതം hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  തിരികെ static_coretext_funcs.get_unconst ();
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
ശൂന്യം
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  എങ്കിൽ (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    തിരികെ;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
