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

#جے_تعریف_شدہ HAVE_CORETEXT

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
#جے_انت

#تعریف_کرو MAX_GLYPHS 256u

ساكن hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       فراغ *font_انکڑے HB_بن_ورتیا,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  بنانشانی گنتی = 0;

  إذا (unicode <= 0xFFFF)
  {
    ch[گنتی++] = unicode;
  }
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[گنتی++] = (unicode >> 10) + 0xD7C0;
    ch[گنتی++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[گنتی++] = 0xFFFD;

  إذا (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, گنتی))
  {
    *glyph = cg_glyph[0];
    أرجع سچ;
  }
  أرجع خطأ;
}

ساكن بنانشانی صحيح
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				فراغ *font_انکڑے,
				بنانشانی صحيح گنتی,
				ثابت hb_codepoint_t *پہلا_unicode,
				بنانشانی صحيح unicode_stride,
				hb_codepoint_t *پہلا_glyph,
				بنانشانی صحيح glyph_stride,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  منطقي slow_رستہ = خطأ;
  خودکار *unicode = پہلا_unicode;
  لكل (بنانشانی i = 0; i < گنتی; i++)
  {
    إذا (*unicode > 0xFFFF)
    {
      slow_رستہ = سچ;
      اكسر;
    }
    unicode = &Struct_At_وتھ<ثابت hb_codepoint_t> (unicode, unicode_stride);
  }

  إذا (unlikely (slow_رستہ))
  {
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      إذا (!hb_coretext_get_nominal_glyph (font, font_انکڑے, *پہلا_unicode, پہلا_glyph, خالی_سوچک))
	أرجع i;
      پہلا_unicode = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
      پہلا_glyph = &Struct_At_وتھ<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    أرجع گنتی;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  لكل (بنانشانی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    بنانشانی c = (بنانشانی) hb_min ((صحيح) MAX_GLYPHS, (صحيح) گنتی - (صحيح) i);
    لكل (بنانشانی j = 0; j < c; j++)
    {
      ch[j] = *پہلا_unicode;
      پہلا_unicode = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
    }
    إذا (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      لكل (بنانشانی j = 0; j < c; j++)
      {
	إذا (!hb_coretext_get_nominal_glyph (font, font_انکڑے, ch[j], پہلا_glyph, خالی_سوچک))
	  أرجع i + j;
	پہلا_glyph = &Struct_At_وتھ<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      }
    }
    لكل (بنانشانی j = 0; j < c; j++)
    {
      *پہلا_glyph = cg_glyph[j];
      پہلا_glyph = &Struct_At_وتھ<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
  }

  أرجع گنتی;
}

ساكن hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 فراغ *font_انکڑے HB_بن_ورتیا,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  بنانشانی گنتی = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  إذا (unicode <= 0xFFFF)
    ch[گنتی++] = unicode;
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[گنتی++] = (unicode >> 10) + 0xD7C0;
    ch[گنتی++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[گنتی++] = 0xFFFD;

  إذا (variation_selector <= 0xFFFF)
    ch[گنتی++] = variation_selector;
  وإلا إذا (variation_selector <= 0x10FFFF)
  {
    ch[گنتی++] = (variation_selector >> 10) + 0xD7C0;
    ch[گنتی++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[گنتی++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, گنتی);

  // All except for first should be zero if we succeeded
  لكل (بنانشانی i = 1; i < گنتی; i++)
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
				  فراغ* font_انکڑے HB_بن_ورتیا,
				  بنانشانی گنتی,
				  ثابت hb_codepoint_t *پہلا_glyph,
				  بنانشانی glyph_stride,
				  hb_position_t *پہلا_advance,
				  بنانشانی advance_stride,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ناپ;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بنانشانی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    بنانشانی c = (بنانشانی) hb_min ((صحيح) MAX_GLYPHS, (صحيح) گنتی - (صحيح) i);
    لكل (بنانشانی j = 0; j < c; j++)
    {
      cg_glyph[j] = *پہلا_glyph;
      پہلا_glyph = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    لكل (بنانشانی j = 0; j < c; j++)
    {
      *پہلا_advance = hb_clamp_منزل<hb_position_t> (
	  round (advances[j].چوڑائی * x_mult) - (مزدوج) tracking);
      پہلا_advance = &Struct_At_وتھ<hb_position_t> (پہلا_advance, advance_stride);
    }
  }
}

#جے_بنا_تعریف HB_NO_VERTICAL
ساكن فراغ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  فراغ* font_انکڑے HB_بن_ورتیا,
				  بنانشانی گنتی,
				  ثابت hb_codepoint_t *پہلا_glyph,
				  بنانشانی glyph_stride,
				  hb_position_t *پہلا_advance,
				  بنانشانی advance_stride,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_ناپ;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بنانشانی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    بنانشانی c = (بنانشانی) hb_min ((صحيح) MAX_GLYPHS, (صحيح) گنتی - (صحيح) i);
    لكل (بنانشانی j = 0; j < c; j++)
    {
      cg_glyph[j] = *پہلا_glyph;
      پہلا_glyph = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    لكل (بنانشانی j = 0; j < c; j++)
    {
      *پہلا_advance = hb_clamp_منزل<hb_position_t> (
	  round (advances[j].چوڑائی * y_mult) - (مزدوج) tracking);
      پہلا_advance = &Struct_At_وتھ<hb_position_t> (پہلا_advance, advance_stride);
    }
  }
}

ساكن hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				فراغ *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_ناپ;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_ناپ;

  ثابت CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_منزل<hb_position_t> (round (x_mult * origin.چوڑائی));
  *y = hb_clamp_منزل<hb_position_t> (round (y_mult * origin.اچائی));

  أرجع سچ;
}
#جے_انت

ساكن hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       فراغ *font_انکڑے HB_بن_ورتیا,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ناپ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ناپ;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, عدم, 1);

  مزدوج y_bearing = round ((bounds.origin.y + bounds.ناپ.اچائی) * y_mult);

  extents->x_bearing = hb_clamp_منزل<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
  extents->چوڑائی = hb_clamp_منزل<hb_position_t> (round (bounds.ناپ.چوڑائی * x_mult));
  extents->اچائی = hb_clamp_منزل<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  أرجع سچ;
}

ساكن hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				فراغ *font_انکڑے HB_بن_ورتیا,
				hb_font_extents_t *metrics,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ناپ;

  metrics->ascender = hb_clamp_منزل<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_منزل<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->سطر_gap = hb_clamp_منزل<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  أرجع سچ;
}

#جے_بنا_تعریف HB_NO_DRAW

ساكن فراغ
ct_apply_func (فراغ *info, ثابت CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  بدل (element->type)
  {
  حالة k_CGPath_Element_Move_منزل_نقطہ:
    draws->move_منزل (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جوڑو_سطر_منزل_نقطہ:
    draws->سطر_منزل (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جوڑو_Quad_ونگ_منزل_نقطہ:
    draws->quadratic_منزل (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    اكسر;
  حالة k_CGPath_Element_جوڑو_ونگ_منزل_نقطہ:
    draws->cubic_منزل (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    اكسر;
  حالة kCGPathElementCloseSubpath:
    draws->close_رستہ ();
    اكسر;
  }
}

ساكن hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				فراغ *font_انکڑے HB_بن_ورتیا,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, فراغ *draw_انکڑے,
				فراغ *user_انکڑے)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ناپ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ناپ;

  CGAffineTransform transform = CGAffine_Transform_پچھان_تت;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> رستہ (CTFont_Create_رستہ_For_Glyph (ct_font, glyph, &transform));
  إذا (!رستہ)
    أرجع خطأ;

  hb_draw_session_t drawing {draw_funcs, draw_انکڑے};

  CGPathApply (رستہ, &drawing, ct_apply_func);

  أرجع سچ;
}
#جے_انت

ساكن hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    فراغ *font_انکڑے HB_بن_ورتیا,
			    hb_codepoint_t glyph,
			    حرف *name, بنانشانی صحيح ناپ,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) font->face->انکڑے.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  إذا (!cf_name)
    أرجع خطأ;

  إذا (!ناپ)
    أرجع سچ;

  CFIndex len = CFString_Get_لمائی (cf_name);
  إذا (len > (CFIndex)ناپ - 1)
    len = (CFIndex)ناپ - 1;

  CFString_Get_بائٹاں (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, خطأ,
		    (UInt8 *) name, ناپ, &len);

  name[len] = '\0';

  أرجع سچ;
}

ساكن hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 فراغ *font_انکڑے HB_بن_ورتیا,
				 ثابت حرف *name, صحيح len,
				 hb_codepoint_t *glyph,
				 فراغ *user_انکڑے HB_بن_ورتیا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  إذا (len == -1)
    len = لڑی_لمبائی (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_بائٹاں (kCFAllocatorDefault,
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

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, خالی_سوچک, خالی_سوچک);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_OT_FONT_GLYPH_ناں
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_coretext_get_glyph_name, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, خالی_سوچک, خالی_سوچک);
#جے_انت

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
hb_coretext_font_سیٹ_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  إذا (unlikely (!ct_font))
  {
    hb_font_سیٹ_funcs (font,
		       hb_font_funcs_get_خالی_تھاواں (),
		       خالی_سوچک, خالی_سوچک);
    أرجع;
  }

  hb_font_سیٹ_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     خالی_سوچک, خالی_سوچک);
}

#تعریف_رد MAX_GLYPHS

#جے_انت
