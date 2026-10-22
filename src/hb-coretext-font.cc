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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_CORETEXT

#அந்தர்பாவ "hb-coretext.hh"
#அந்தர்பாவ "hb-aat-layout-trak-table.hh"

#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-machinery.hh"

#எனில் (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

நிலையான hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       வெறுமை *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  unsigned மொத்தம் = 0;

  எனில் (unicode <= 0xFFFF)
  {
    ch[மொத்தம்++] = unicode;
  }
  இல்லையெனில் எனில் (unicode <= 0x10FFFF)
  {
    ch[மொத்தம்++] = (unicode >> 10) + 0xD7C0;
    ch[மொத்தம்++] = (unicode & 0x3FF) + 0xDC00;
  }
  இல்லையெனில்
    ch[மொத்தம்++] = 0xFFFD;

  எனில் (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, மொத்தம்))
  {
    *glyph = cg_glyph[0];
    திரும்பு உண்மை;
  }
  திரும்பு பொய்;
}

நிலையான unsigned முழு
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				வெறுமை *font_data,
				unsigned முழு மொத்தம்,
				மாறிலி hb_codepoint_t *first_unicode,
				unsigned முழு unicode_stride,
				hb_codepoint_t *first_glyph,
				unsigned முழு glyph_stride,
				வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  தருக்க slow_path = பொய்;
  auto *unicode = first_unicode;
  க்கு (unsigned i = 0; i < மொத்தம்; i++)
  {
    எனில் (*unicode > 0xFFFF)
    {
      slow_path = உண்மை;
      நிறுத்து;
    }
    unicode = &StructAtOffset<மாறிலி hb_codepoint_t> (unicode, unicode_stride);
  }

  எனில் (unlikely (slow_path))
  {
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
    {
      எனில் (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	திரும்பு i;
      first_unicode = &StructAtOffset<மாறிலி hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    திரும்பு மொத்தம்;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  க்கு (unsigned i = 0; i < மொத்தம்; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((முழு) MAX_GLYPHS, (முழு) மொத்தம் - (முழு) i);
    க்கு (unsigned j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<மாறிலி hb_codepoint_t> (first_unicode, unicode_stride);
    }
    எனில் (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      க்கு (unsigned j = 0; j < c; j++)
      {
	எனில் (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  திரும்பு i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    க்கு (unsigned j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  திரும்பு மொத்தம்;
}

நிலையான hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 வெறுமை *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  unsigned மொத்தம் = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  எனில் (unicode <= 0xFFFF)
    ch[மொத்தம்++] = unicode;
  இல்லையெனில் எனில் (unicode <= 0x10FFFF)
  {
    ch[மொத்தம்++] = (unicode >> 10) + 0xD7C0;
    ch[மொத்தம்++] = (unicode & 0x3FF) + 0xDC00;
  }
  இல்லையெனில்
    ch[மொத்தம்++] = 0xFFFD;

  எனில் (variation_selector <= 0xFFFF)
    ch[மொத்தம்++] = variation_selector;
  இல்லையெனில் எனில் (variation_selector <= 0x10FFFF)
  {
    ch[மொத்தம்++] = (variation_selector >> 10) + 0xD7C0;
    ch[மொத்தம்++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  இல்லையெனில்
    ch[மொத்தம்++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, மொத்தம்);

  // All except for first should be zero if we succeeded
  க்கு (unsigned i = 1; i < மொத்தம்; i++)
    எனில் (cg_glyph[i])
      திரும்பு பொய்;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  வெறுமை* font_data HB_UNUSED,
				  unsigned மொத்தம்,
				  மாறிலி hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  க்கு (unsigned i = 0; i < மொத்தம்; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((முழு) MAX_GLYPHS, (முழு) மொத்தம் - (முழு) i);
    க்கு (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<மாறிலி hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    க்கு (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (இரட்டை) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
நிலையான வெறுமை
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  வெறுமை* font_data HB_UNUSED,
				  unsigned மொத்தம்,
				  மாறிலி hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  க்கு (unsigned i = 0; i < மொத்தம்; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((முழு) MAX_GLYPHS, (முழு) மொத்தம் - (முழு) i);
    க்கு (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<மாறிலி hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    க்கு (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (இரட்டை) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

நிலையான hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				வெறுமை *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  மாறிலி CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  திரும்பு உண்மை;
}
#endif

நிலையான hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       வெறுமை *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, வெற்று, 1);

  இரட்டை y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  திரும்பு உண்மை;
}

நிலையான hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				வெறுமை *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->வரி_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  திரும்பு உண்மை;
}

#ifndef HB_NO_DRAW

நிலையான வெறுமை
ct_apply_func (வெறுமை *info, மாறிலி CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  தேர்வு (element->type)
  {
  நிலை kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    நிறுத்து;
  நிலை k_CGPath_Element_Add_வரி_To_Point:
    draws->வரி_to (element->points[0].x, element->points[0].y);
    நிறுத்து;
  நிலை kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    நிறுத்து;
  நிலை kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    நிறுத்து;
  நிலை kCGPathElementCloseSubpath:
    draws->close_path ();
    நிறுத்து;
  }
}

நிலையான hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				வெறுமை *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, வெறுமை *draw_data,
				வெறுமை *user_data)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  எனில் (!path)
    திரும்பு பொய்;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  திரும்பு உண்மை;
}
#endif

நிலையான hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    வெறுமை *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    எழுத்து *name, unsigned முழு size,
			    வெறுமை *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (மாறிலி வெறுமை *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  எனில் (!cf_name)
    திரும்பு பொய்;

  எனில் (!size)
    திரும்பு உண்மை;

  CFIndex len = CFStringGetLength (cf_name);
  எனில் (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, பொய்,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  திரும்பு உண்மை;
}

நிலையான hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 வெறுமை *font_data HB_UNUSED,
				 மாறிலி எழுத்து *name, முழு len,
				 hb_codepoint_t *glyph,
				 வெறுமை *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext;

  எனில் (len == -1)
    len = strlen (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (மாறிலி UInt8 *) name, len,
							     kCFStringEncodingUTF8, பொய்));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  திரும்பு cg_glyph != 0;
}


நிலையான உள்வரி வெறுமை free_static_coretext_funcs ();

நிலையான கட்டமைப்பு hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  நிலையான hb_font_funcs_t *create ()
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

    திரும்பு funcs;
  }
} static_coretext_funcs;

நிலையான உள்வரி
வெறுமை free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

நிலையான hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  திரும்பு static_coretext_funcs.get_unconst ();
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
வெறுமை
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  எனில் (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    திரும்பு;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
