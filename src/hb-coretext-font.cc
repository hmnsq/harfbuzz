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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_CORETEXT

#अन्तर्भाव "hb-coretext.hh"
#अन्तर्भाव "hb-aat-layout-trak-table.hh"

#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-machinery.hh"

#जर (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

स्थिर hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       शून्य *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  अचिन्हित मोजणी = 0;

  जर (unicode <= 0xFFFF)
  {
    ch[मोजणी++] = unicode;
  }
  अन्यथा जर (unicode <= 0x10FFFF)
  {
    ch[मोजणी++] = (unicode >> 10) + 0xD7C0;
    ch[मोजणी++] = (unicode & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[मोजणी++] = 0xFFFD;

  जर (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, मोजणी))
  {
    *glyph = cg_glyph[0];
    परत सत्य;
  }
  परत असत्य;
}

स्थिर अचिन्हित पूर्णांक
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				शून्य *font_data,
				अचिन्हित पूर्णांक मोजणी,
				स्थिरांक hb_codepoint_t *first_unicode,
				अचिन्हित पूर्णांक unicode_stride,
				hb_codepoint_t *first_glyph,
				अचिन्हित पूर्णांक glyph_stride,
				शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  तार्किक slow_path = असत्य;
  स्वतः *unicode = first_unicode;
  साठी (अचिन्हित i = 0; i < मोजणी; i++)
  {
    जर (*unicode > 0xFFFF)
    {
      slow_path = सत्य;
      थांबा;
    }
    unicode = &StructAtOffset<स्थिरांक hb_codepoint_t> (unicode, unicode_stride);
  }

  जर (unlikely (slow_path))
  {
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      जर (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	परत i;
      first_unicode = &StructAtOffset<स्थिरांक hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    परत मोजणी;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  साठी (अचिन्हित i = 0; i < मोजणी; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) मोजणी - (पूर्णांक) i);
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<स्थिरांक hb_codepoint_t> (first_unicode, unicode_stride);
    }
    जर (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      साठी (अचिन्हित j = 0; j < c; j++)
      {
	जर (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  परत i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  परत मोजणी;
}

स्थिर hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 शून्य *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  अचिन्हित मोजणी = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  जर (unicode <= 0xFFFF)
    ch[मोजणी++] = unicode;
  अन्यथा जर (unicode <= 0x10FFFF)
  {
    ch[मोजणी++] = (unicode >> 10) + 0xD7C0;
    ch[मोजणी++] = (unicode & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[मोजणी++] = 0xFFFD;

  जर (variation_selector <= 0xFFFF)
    ch[मोजणी++] = variation_selector;
  अन्यथा जर (variation_selector <= 0x10FFFF)
  {
    ch[मोजणी++] = (variation_selector >> 10) + 0xD7C0;
    ch[मोजणी++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  अन्यथा
    ch[मोजणी++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, मोजणी);

  // All except for first should be zero if we succeeded
  साठी (अचिन्हित i = 1; i < मोजणी; i++)
    जर (cg_glyph[i])
      परत असत्य;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  परत सत्य;
}

स्थिर शून्य
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  शून्य* font_data HB_UNUSED,
				  अचिन्हित मोजणी,
				  स्थिरांक hb_codepoint_t *first_glyph,
				  अचिन्हित glyph_stride,
				  hb_position_t *first_advance,
				  अचिन्हित advance_stride,
				  शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  साठी (अचिन्हित i = 0; i < मोजणी; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) मोजणी - (पूर्णांक) i);
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<स्थिरांक hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (द्विगुण) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
स्थिर शून्य
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  शून्य* font_data HB_UNUSED,
				  अचिन्हित मोजणी,
				  स्थिरांक hb_codepoint_t *first_glyph,
				  अचिन्हित glyph_stride,
				  hb_position_t *first_advance,
				  अचिन्हित advance_stride,
				  शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  साठी (अचिन्हित i = 0; i < मोजणी; i += MAX_GLYPHS)
  {
    अचिन्हित c = (अचिन्हित) hb_min ((पूर्णांक) MAX_GLYPHS, (पूर्णांक) मोजणी - (पूर्णांक) i);
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<स्थिरांक hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    साठी (अचिन्हित j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (द्विगुण) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

स्थिर hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				शून्य *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  स्थिरांक CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  परत सत्य;
}
#endif

स्थिर hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       शून्य *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, रिक्त, 1);

  द्विगुण y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  परत सत्य;
}

स्थिर hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				शून्य *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->ओळ_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  परत सत्य;
}

#ifndef HB_NO_DRAW

स्थिर शून्य
ct_apply_func (शून्य *info, स्थिरांक CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  निवड (element->type)
  {
  स्थिती kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    थांबा;
  स्थिती k_CGPath_Element_Add_ओळ_To_Point:
    draws->ओळ_to (element->points[0].x, element->points[0].y);
    थांबा;
  स्थिती kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    थांबा;
  स्थिती kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    थांबा;
  स्थिती kCGPathElementCloseSubpath:
    draws->close_path ();
    थांबा;
  }
}

स्थिर hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				शून्य *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, शून्य *draw_data,
				शून्य *user_data)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  जर (!path)
    परत असत्य;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  परत सत्य;
}
#endif

स्थिर hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    शून्य *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    अक्षर *name, अचिन्हित पूर्णांक size,
			    शून्य *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (स्थिरांक शून्य *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  जर (!cf_name)
    परत असत्य;

  जर (!size)
    परत सत्य;

  CFIndex len = CFStringGetLength (cf_name);
  जर (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, असत्य,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  परत सत्य;
}

स्थिर hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 शून्य *font_data HB_UNUSED,
				 स्थिरांक अक्षर *name, पूर्णांक len,
				 hb_codepoint_t *glyph,
				 शून्य *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (स्थिरांक शून्य *) font->data.coretext;

  जर (len == -1)
    len = म_माप (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (स्थिरांक UInt8 *) name, len,
							     kCFStringEncodingUTF8, असत्य));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  परत cg_glyph != 0;
}


स्थिर ओळीत शून्य free_static_coretext_funcs ();

स्थिर संरचना hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  स्थिर hb_font_funcs_t *create ()
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

    परत funcs;
  }
} static_coretext_funcs;

स्थिर ओळीत
शून्य free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

स्थिर hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  परत static_coretext_funcs.get_unconst ();
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
शून्य
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  जर (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    परत;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
