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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_CORETEXT

#অন্তর্ভাব "hb-coretext.hh"
#অন্তর্ভাব "hb-aat-layout-trak-table.hh"

#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-machinery.hh"

#যদি (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#endif

#define MAX_GLYPHS 256u

স্থির hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       শূন্য *font_data HB_UNUSED,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  unsigned গুনতি = 0;

  যদি (unicode <= 0xFFFF)
  {
    ch[গুনতি++] = unicode;
  }
  অন্যথা যদি (unicode <= 0x10FFFF)
  {
    ch[গুনতি++] = (unicode >> 10) + 0xD7C0;
    ch[গুনতি++] = (unicode & 0x3FF) + 0xDC00;
  }
  অন্যথা
    ch[গুনতি++] = 0xFFFD;

  যদি (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, গুনতি))
  {
    *glyph = cg_glyph[0];
    প্রত্যাগম সত্য;
  }
  প্রত্যাগম অসত্য;
}

স্থির unsigned পূর্ণাঙ্ক
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				শূন্য *font_data,
				unsigned পূর্ণাঙ্ক গুনতি,
				ধ্রুব hb_codepoint_t *first_unicode,
				unsigned পূর্ণাঙ্ক unicode_stride,
				hb_codepoint_t *first_glyph,
				unsigned পূর্ণাঙ্ক glyph_stride,
				শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  তার্কিক slow_path = অসত্য;
  auto *unicode = first_unicode;
  প্রতি (unsigned i = 0; i < গুনতি; i++)
  {
    যদি (*unicode > 0xFFFF)
    {
      slow_path = সত্য;
      ভঙ্গ;
    }
    unicode = &StructAtOffset<ধ্রুব hb_codepoint_t> (unicode, unicode_stride);
  }

  যদি (unlikely (slow_path))
  {
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      যদি (!hb_coretext_get_nominal_glyph (font, font_data, *first_unicode, first_glyph, nullptr))
	প্রত্যাগম i;
      first_unicode = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_unicode, unicode_stride);
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
    প্রত্যাগম গুনতি;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  প্রতি (unsigned i = 0; i < গুনতি; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((পূর্ণাঙ্ক) MAX_GLYPHS, (পূর্ণাঙ্ক) গুনতি - (পূর্ণাঙ্ক) i);
    প্রতি (unsigned j = 0; j < c; j++)
    {
      ch[j] = *first_unicode;
      first_unicode = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_unicode, unicode_stride);
    }
    যদি (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      প্রতি (unsigned j = 0; j < c; j++)
      {
	যদি (!hb_coretext_get_nominal_glyph (font, font_data, ch[j], first_glyph, nullptr))
	  প্রত্যাগম i + j;
	first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
      }
    }
    প্রতি (unsigned j = 0; j < c; j++)
    {
      *first_glyph = cg_glyph[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }
  }

  প্রত্যাগম গুনতি;
}

স্থির hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 শূন্য *font_data HB_UNUSED,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  unsigned গুনতি = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  যদি (unicode <= 0xFFFF)
    ch[গুনতি++] = unicode;
  অন্যথা যদি (unicode <= 0x10FFFF)
  {
    ch[গুনতি++] = (unicode >> 10) + 0xD7C0;
    ch[গুনতি++] = (unicode & 0x3FF) + 0xDC00;
  }
  অন্যথা
    ch[গুনতি++] = 0xFFFD;

  যদি (variation_selector <= 0xFFFF)
    ch[গুনতি++] = variation_selector;
  অন্যথা যদি (variation_selector <= 0x10FFFF)
  {
    ch[গুনতি++] = (variation_selector >> 10) + 0xD7C0;
    ch[গুনতি++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  অন্যথা
    ch[গুনতি++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, গুনতি);

  // All except for first should be zero if we succeeded
  প্রতি (unsigned i = 1; i < গুনতি; i++)
    যদি (cg_glyph[i])
      প্রত্যাগম অসত্য;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  প্রত্যাগম সত্য;
}

স্থির শূন্য
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  শূন্য* font_data HB_UNUSED,
				  unsigned গুনতি,
				  ধ্রুব hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  প্রতি (unsigned i = 0; i < গুনতি; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((পূর্ণাঙ্ক) MAX_GLYPHS, (পূর্ণাঙ্ক) গুনতি - (পূর্ণাঙ্ক) i);
    প্রতি (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    প্রতি (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * x_mult) - (দ্বিগুণ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

#ifndef HB_NO_VERTICAL
স্থির শূন্য
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  শূন্য* font_data HB_UNUSED,
				  unsigned গুনতি,
				  ধ্রুব hb_codepoint_t *first_glyph,
				  unsigned glyph_stride,
				  hb_position_t *first_advance,
				  unsigned advance_stride,
				  শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;
  hb_position_t tracking = font->face->table.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  প্রতি (unsigned i = 0; i < গুনতি; i += MAX_GLYPHS)
  {
    unsigned c = (unsigned) hb_min ((পূর্ণাঙ্ক) MAX_GLYPHS, (পূর্ণাঙ্ক) গুনতি - (পূর্ণাঙ্ক) i);
    প্রতি (unsigned j = 0; j < c; j++)
    {
      cg_glyph[j] = *first_glyph;
      first_glyph = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    প্রতি (unsigned j = 0; j < c; j++)
    {
      *first_advance = hb_clamp_to<hb_position_t> (
	  round (advances[j].width * y_mult) - (দ্বিগুণ) tracking);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }
  }
}

স্থির hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				শূন্য *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_size;

  ধ্রুব CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_to<hb_position_t> (round (x_mult * origin.width));
  *y = hb_clamp_to<hb_position_t> (round (y_mult * origin.height));

  প্রত্যাগম সত্য;
}
#endif

স্থির hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       শূন্য *font_data HB_UNUSED,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, রিক্ত, 1);

  দ্বিগুণ y_bearing = round ((bounds.origin.y + bounds.size.height) * y_mult);

  extents->x_bearing = hb_clamp_to<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
  extents->width = hb_clamp_to<hb_position_t> (round (bounds.size.width * x_mult));
  extents->height = hb_clamp_to<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  প্রত্যাগম সত্য;
}

স্থির hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				শূন্য *font_data HB_UNUSED,
				hb_font_extents_t *metrics,
				শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  metrics->ascender = hb_clamp_to<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_to<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->সারি_gap = hb_clamp_to<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  প্রত্যাগম সত্য;
}

#ifndef HB_NO_DRAW

স্থির শূন্য
ct_apply_func (শূন্য *info, ধ্রুব CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  বিকল্প (element->type)
  {
  পক্ষ kCGPathElementMoveToPoint:
    draws->move_to (element->points[0].x, element->points[0].y);
    ভঙ্গ;
  পক্ষ k_CGPath_Element_Add_সারি_To_Point:
    draws->সারি_to (element->points[0].x, element->points[0].y);
    ভঙ্গ;
  পক্ষ kCGPathElementAddQuadCurveToPoint:
    draws->quadratic_to (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    ভঙ্গ;
  পক্ষ kCGPathElementAddCurveToPoint:
    draws->cubic_to (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    ভঙ্গ;
  পক্ষ kCGPathElementCloseSubpath:
    draws->close_path ();
    ভঙ্গ;
  }
}

স্থির hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				শূন্য *font_data HB_UNUSED,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, শূন্য *draw_data,
				শূন্য *user_data)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  CGAffineTransform transform = CGAffineTransformIdentity;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> path (CTFontCreatePathForGlyph (ct_font, glyph, &transform));
  যদি (!path)
    প্রত্যাগম অসত্য;

  hb_draw_session_t drawing {draw_funcs, draw_data};

  CGPathApply (path, &drawing, ct_apply_func);

  প্রত্যাগম সত্য;
}
#endif

স্থির hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    শূন্য *font_data HB_UNUSED,
			    hb_codepoint_t glyph,
			    বর্ণ *name, unsigned পূর্ণাঙ্ক size,
			    শূন্য *user_data HB_UNUSED)
{
  CGFontRef cg_font = (CGFontRef) (ধ্রুব শূন্য *) font->face->data.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  যদি (!cf_name)
    প্রত্যাগম অসত্য;

  যদি (!size)
    প্রত্যাগম সত্য;

  CFIndex len = CFStringGetLength (cf_name);
  যদি (len > (CFIndex)size - 1)
    len = (CFIndex)size - 1;

  CFStringGetBytes (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, অসত্য,
		    (UInt8 *) name, size, &len);

  name[len] = '\0';

  প্রত্যাগম সত্য;
}

স্থির hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 শূন্য *font_data HB_UNUSED,
				 ধ্রুব বর্ণ *name, পূর্ণাঙ্ক len,
				 hb_codepoint_t *glyph,
				 শূন্য *user_data HB_UNUSED)
{
  CTFontRef ct_font = (CTFontRef) (ধ্রুব শূন্য *) font->data.coretext;

  যদি (len == -1)
    len = strlen (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFStringCreateWithBytes (kCFAllocatorDefault,
							     (ধ্রুব UInt8 *) name, len,
							     kCFStringEncodingUTF8, অসত্য));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  প্রত্যাগম cg_glyph != 0;
}


স্থির অন্তঃস্থ শূন্য free_static_coretext_funcs ();

স্থির সংরচনা hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  স্থির hb_font_funcs_t *create ()
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

    প্রত্যাগম funcs;
  }
} static_coretext_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

স্থির hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  প্রত্যাগম static_coretext_funcs.get_unconst ();
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
শূন্য
hb_coretext_font_set_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  যদি (unlikely (!ct_font))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    প্রত্যাগম;
  }

  hb_font_set_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
