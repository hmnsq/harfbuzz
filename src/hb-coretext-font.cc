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

#ضمن "hb.hh"

#جيڪڏهن_وصف_ٿيل HAVE_CORETEXT

#ضمن "hb-coretext.hh"
#ضمن "hb-aat-layout-trak-table.hh"

#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"

#إذا (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#جيڪڏهن_پورو

#وصف_ڪريو MAX_GLYPHS 256u

ساكن hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  بنانشان ڳڻپ = 0;

  إذا (unicode <= 0xFFFF)
  {
    ch[ڳڻپ++] = unicode;
  }
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[ڳڻپ++] = (unicode >> 10) + 0xD7C0;
    ch[ڳڻپ++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[ڳڻپ++] = 0xFFFD;

  إذا (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ڳڻپ))
  {
    *glyph = cg_glyph[0];
    أرجع سچ;
  }
  أرجع خطأ;
}

ساكن بنانشان صحيح
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				فراغ *font_معلومات,
				بنانشان صحيح ڳڻپ,
				ثابت hb_codepoint_t *پهريون_unicode,
				بنانشان صحيح unicode_stride,
				hb_codepoint_t *پهريون_glyph,
				بنانشان صحيح glyph_stride,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  منطقي slow_رستو = خطأ;
  خودڪار *unicode = پهريون_unicode;
  لكل (بنانشان i = 0; i < ڳڻپ; i++)
  {
    إذا (*unicode > 0xFFFF)
    {
      slow_رستو = سچ;
      اكسر;
    }
    unicode = &Struct_At_وٿي<ثابت hb_codepoint_t> (unicode, unicode_stride);
  }

  إذا (unlikely (slow_رستو))
  {
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      إذا (!hb_coretext_get_nominal_glyph (font, font_معلومات, *پهريون_unicode, پهريون_glyph, خالي_اشارو))
	أرجع i;
      پهريون_unicode = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_unicode, unicode_stride);
      پهريون_glyph = &Struct_At_وٿي<hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    أرجع ڳڻپ;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  لكل (بنانشان i = 0; i < ڳڻپ; i += MAX_GLYPHS)
  {
    بنانشان c = (بنانشان) hb_min ((صحيح) MAX_GLYPHS, (صحيح) ڳڻپ - (صحيح) i);
    لكل (بنانشان j = 0; j < c; j++)
    {
      ch[j] = *پهريون_unicode;
      پهريون_unicode = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_unicode, unicode_stride);
    }
    إذا (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      لكل (بنانشان j = 0; j < c; j++)
      {
	إذا (!hb_coretext_get_nominal_glyph (font, font_معلومات, ch[j], پهريون_glyph, خالي_اشارو))
	  أرجع i + j;
	پهريون_glyph = &Struct_At_وٿي<hb_codepoint_t> (پهريون_glyph, glyph_stride);
      }
    }
    لكل (بنانشان j = 0; j < c; j++)
    {
      *پهريون_glyph = cg_glyph[j];
      پهريون_glyph = &Struct_At_وٿي<hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
  }

  أرجع ڳڻپ;
}

ساكن hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  بنانشان ڳڻپ = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  إذا (unicode <= 0xFFFF)
    ch[ڳڻپ++] = unicode;
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[ڳڻپ++] = (unicode >> 10) + 0xD7C0;
    ch[ڳڻپ++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[ڳڻپ++] = 0xFFFD;

  إذا (variation_selector <= 0xFFFF)
    ch[ڳڻپ++] = variation_selector;
  وإلا إذا (variation_selector <= 0x10FFFF)
  {
    ch[ڳڻپ++] = (variation_selector >> 10) + 0xD7C0;
    ch[ڳڻپ++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[ڳڻپ++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, ڳڻپ);

  // All except for first should be zero if we succeeded
  لكل (بنانشان i = 1; i < ڳڻپ; i++)
    إذا (cg_glyph[i])
      أرجع خطأ;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  أرجع سچ;
}

ساكن فراغ
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  فراغ* font_معلومات HB_اڻ_استعمال_ٿيل,
				  بنانشان ڳڻپ,
				  ثابت hb_codepoint_t *پهريون_glyph,
				  بنانشان glyph_stride,
				  hb_position_t *پهريون_advance,
				  بنانشان advance_stride,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائيز;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بنانشان i = 0; i < ڳڻپ; i += MAX_GLYPHS)
  {
    بنانشان c = (بنانشان) hb_min ((صحيح) MAX_GLYPHS, (صحيح) ڳڻپ - (صحيح) i);
    لكل (بنانشان j = 0; j < c; j++)
    {
      cg_glyph[j] = *پهريون_glyph;
      پهريون_glyph = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    لكل (بنانشان j = 0; j < c; j++)
    {
      *پهريون_advance = hb_clamp_منزل<hb_position_t> (
	  round (advances[j].ويڪر * x_mult) - (مزدوج) tracking);
      پهريون_advance = &Struct_At_وٿي<hb_position_t> (پهريون_advance, advance_stride);
    }
  }
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
ساكن فراغ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  فراغ* font_معلومات HB_اڻ_استعمال_ٿيل,
				  بنانشان ڳڻپ,
				  ثابت hb_codepoint_t *پهريون_glyph,
				  بنانشان glyph_stride,
				  hb_position_t *پهريون_advance,
				  بنانشان advance_stride,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_سائيز;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بنانشان i = 0; i < ڳڻپ; i += MAX_GLYPHS)
  {
    بنانشان c = (بنانشان) hb_min ((صحيح) MAX_GLYPHS, (صحيح) ڳڻپ - (صحيح) i);
    لكل (بنانشان j = 0; j < c; j++)
    {
      cg_glyph[j] = *پهريون_glyph;
      پهريون_glyph = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    لكل (بنانشان j = 0; j < c; j++)
    {
      *پهريون_advance = hb_clamp_منزل<hb_position_t> (
	  round (advances[j].ويڪر * y_mult) - (مزدوج) tracking);
      پهريون_advance = &Struct_At_وٿي<hb_position_t> (پهريون_advance, advance_stride);
    }
  }
}

ساكن hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_سائيز;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_سائيز;

  ثابت CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_منزل<hb_position_t> (round (x_mult * origin.ويڪر));
  *y = hb_clamp_منزل<hb_position_t> (round (y_mult * origin.اوچائي));

  أرجع سچ;
}
#جيڪڏهن_پورو

ساكن hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائيز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائيز;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, عدم, 1);

  مزدوج y_bearing = round ((bounds.origin.y + bounds.سائيز.اوچائي) * y_mult);

  extents->x_bearing = hb_clamp_منزل<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
  extents->ويڪر = hb_clamp_منزل<hb_position_t> (round (bounds.سائيز.ويڪر * x_mult));
  extents->اوچائي = hb_clamp_منزل<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  أرجع سچ;
}

ساكن hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				hb_font_extents_t *metrics,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائيز;

  metrics->ascender = hb_clamp_منزل<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_منزل<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->سٽ_gap = hb_clamp_منزل<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  أرجع سچ;
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW

ساكن فراغ
ct_apply_func (فراغ *info, ثابت CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  بدل (element->type)
  {
  حالة k_CGPath_Element_Move_منزل_نقطو:
    draws->move_منزل (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جوڙيو_سٽ_منزل_نقطو:
    draws->سٽ_منزل (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جوڙيو_Quad_وڪر_منزل_نقطو:
    draws->quadratic_منزل (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    اكسر;
  حالة k_CGPath_Element_جوڙيو_وڪر_منزل_نقطو:
    draws->cubic_منزل (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    اكسر;
  حالة kCGPathElementCloseSubpath:
    draws->close_رستو ();
    اكسر;
  }
}

ساكن hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, فراغ *draw_معلومات,
				فراغ *user_معلومات)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائيز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائيز;

  CGAffineTransform transform = CGAffine_Transform_سڃاڻپ_عنصر;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> رستو (CTFont_Create_رستو_For_Glyph (ct_font, glyph, &transform));
  إذا (!رستو)
    أرجع خطأ;

  hb_draw_session_t drawing {draw_funcs, draw_معلومات};

  CGPathApply (رستو, &drawing, ct_apply_func);

  أرجع سچ;
}
#جيڪڏهن_پورو

ساكن hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
			    hb_codepoint_t glyph,
			    حرف *name, بنانشان صحيح سائيز,
			    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) font->face->معلومات.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  إذا (!cf_name)
    أرجع خطأ;

  إذا (!سائيز)
    أرجع سچ;

  CFIndex len = CFString_Get_ڊيگهائي (cf_name);
  إذا (len > (CFIndex)سائيز - 1)
    len = (CFIndex)سائيز - 1;

  CFString_Get_بائيٽون (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, خطأ,
		    (UInt8 *) name, سائيز, &len);

  name[len] = '\0';

  أرجع سچ;
}

ساكن hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				 ثابت حرف *name, صحيح len,
				 hb_codepoint_t *glyph,
				 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  إذا (len == -1)
    len = لڙي_ڊيگهه (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_بائيٽون (kCFAllocatorDefault,
							     (ثابت UInt8 *) name, len,
							     kCFStringEncodingUTF8, خطأ));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  أرجع cg_glyph != 0;
}


ساكن مضمن فراغ free_static_coretext_funcs ();

ساكن بنية hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سيٽ_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, خالي_اشارو, خالي_اشارو);

    hb_font_funcs_سيٽ_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, خالي_اشارو, خالي_اشارو);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
    hb_font_funcs_سيٽ_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, خالي_اشارو, خالي_اشارو);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW
    hb_font_funcs_سيٽ_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, خالي_اشارو, خالي_اشارو);
#جيڪڏهن_پورو

    hb_font_funcs_سيٽ_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, خالي_اشارو, خالي_اشارو);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_GLYPH_نالا
    hb_font_funcs_سيٽ_glyph_name_func (funcs, hb_coretext_get_glyph_name, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, خالي_اشارو, خالي_اشارو);
#جيڪڏهن_پورو

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    أرجع funcs;
  }
} static_coretext_funcs;

ساكن مضمن
فراغ free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

ساكن hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  أرجع static_coretext_funcs.get_unconst ();
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
فراغ
hb_coretext_font_سيٽ_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  إذا (unlikely (!ct_font))
  {
    hb_font_سيٽ_funcs (font,
		       hb_font_funcs_get_خالي_جايون (),
		       خالي_اشارو, خالي_اشارو);
    أرجع;
  }

  hb_font_سيٽ_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     خالي_اشارو, خالي_اشارو);
}

#وصف_رد MAX_GLYPHS

#جيڪڏهن_پورو
