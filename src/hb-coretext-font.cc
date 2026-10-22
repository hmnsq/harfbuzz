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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_CORETEXT

#شامل "hb-coretext.hh"
#شامل "hb-aat-layout-trak-table.hh"

#شامل "hb-draw.hh"
#شامل "hb-font.hh"
#شامل "hb-machinery.hh"

#اگر (defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1080) \
    || (defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 60000) \
    || (defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__) && __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 90000)
#  define kCTFontOrientationDefault kCTFontDefaultOrientation
#  define kCTFontOrientationHorizontal kCTFontHorizontalOrientation
#  define kCTFontOrientationVertical kCTFontVerticalOrientation
#اگر_ختم

#تعریف_کرو MAX_GLYPHS 256u

ساکن hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       خالی *font_ڈیٹا HB_غیر_مستعمل,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  غیرعلامتی گنتی = 0;

  اگر (unicode <= 0xFFFF)
  {
    ch[گنتی++] = unicode;
  }
  ورنہ اگر (unicode <= 0x10FFFF)
  {
    ch[گنتی++] = (unicode >> 10) + 0xD7C0;
    ch[گنتی++] = (unicode & 0x3FF) + 0xDC00;
  }
  ورنہ
    ch[گنتی++] = 0xFFFD;

  اگر (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, گنتی))
  {
    *glyph = cg_glyph[0];
    واپس سچ;
  }
  واپس جھوٹ;
}

ساکن غیرعلامتی صحیح
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				خالی *font_ڈیٹا,
				غیرعلامتی صحیح گنتی,
				ثابت hb_codepoint_t *پہلا_unicode,
				غیرعلامتی صحیح unicode_stride,
				hb_codepoint_t *پہلا_glyph,
				غیرعلامتی صحیح glyph_stride,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  منطقی slow_راستہ = جھوٹ;
  خودکار *unicode = پہلا_unicode;
  برائے (غیرعلامتی i = 0; i < گنتی; i++)
  {
    اگر (*unicode > 0xFFFF)
    {
      slow_راستہ = سچ;
      توڑو;
    }
    unicode = &Struct_At_انحراف<ثابت hb_codepoint_t> (unicode, unicode_stride);
  }

  اگر (unlikely (slow_راستہ))
  {
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اگر (!hb_coretext_get_nominal_glyph (font, font_ڈیٹا, *پہلا_unicode, پہلا_glyph, خالی_اشاریہ))
	واپس i;
      پہلا_unicode = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
      پہلا_glyph = &Struct_At_انحراف<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    واپس گنتی;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  برائے (غیرعلامتی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    غیرعلامتی c = (غیرعلامتی) hb_min ((صحیح) MAX_GLYPHS, (صحیح) گنتی - (صحیح) i);
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      ch[j] = *پہلا_unicode;
      پہلا_unicode = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
    }
    اگر (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      برائے (غیرعلامتی j = 0; j < c; j++)
      {
	اگر (!hb_coretext_get_nominal_glyph (font, font_ڈیٹا, ch[j], پہلا_glyph, خالی_اشاریہ))
	  واپس i + j;
	پہلا_glyph = &Struct_At_انحراف<hb_codepoint_t> (پہلا_glyph, glyph_stride);
      }
    }
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      *پہلا_glyph = cg_glyph[j];
      پہلا_glyph = &Struct_At_انحراف<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
  }

  واپس گنتی;
}

ساکن hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 خالی *font_ڈیٹا HB_غیر_مستعمل,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  غیرعلامتی گنتی = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  اگر (unicode <= 0xFFFF)
    ch[گنتی++] = unicode;
  ورنہ اگر (unicode <= 0x10FFFF)
  {
    ch[گنتی++] = (unicode >> 10) + 0xD7C0;
    ch[گنتی++] = (unicode & 0x3FF) + 0xDC00;
  }
  ورنہ
    ch[گنتی++] = 0xFFFD;

  اگر (variation_selector <= 0xFFFF)
    ch[گنتی++] = variation_selector;
  ورنہ اگر (variation_selector <= 0x10FFFF)
  {
    ch[گنتی++] = (variation_selector >> 10) + 0xD7C0;
    ch[گنتی++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  ورنہ
    ch[گنتی++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, گنتی);

  // All except for first should be zero if we succeeded
  برائے (غیرعلامتی i = 1; i < گنتی; i++)
    اگر (cg_glyph[i])
      واپس جھوٹ;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  واپس سچ;
}

ساکن خالی
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  خالی* font_ڈیٹا HB_غیر_مستعمل,
				  غیرعلامتی گنتی,
				  ثابت hb_codepoint_t *پہلا_glyph,
				  غیرعلامتی glyph_stride,
				  hb_position_t *پہلا_advance,
				  غیرعلامتی advance_stride,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائز;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  برائے (غیرعلامتی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    غیرعلامتی c = (غیرعلامتی) hb_min ((صحیح) MAX_GLYPHS, (صحیح) گنتی - (صحیح) i);
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      cg_glyph[j] = *پہلا_glyph;
      پہلا_glyph = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      *پہلا_advance = hb_clamp_مقصد<hb_position_t> (
	  round (advances[j].چوڑائی * x_mult) - (دوہرا) tracking);
      پہلا_advance = &Struct_At_انحراف<hb_position_t> (پہلا_advance, advance_stride);
    }
  }
}

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
ساکن خالی
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  خالی* font_ڈیٹا HB_غیر_مستعمل,
				  غیرعلامتی گنتی,
				  ثابت hb_codepoint_t *پہلا_glyph,
				  غیرعلامتی glyph_stride,
				  hb_position_t *پہلا_advance,
				  غیرعلامتی advance_stride,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_سائز;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  برائے (غیرعلامتی i = 0; i < گنتی; i += MAX_GLYPHS)
  {
    غیرعلامتی c = (غیرعلامتی) hb_min ((صحیح) MAX_GLYPHS, (صحیح) گنتی - (صحیح) i);
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      cg_glyph[j] = *پہلا_glyph;
      پہلا_glyph = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    برائے (غیرعلامتی j = 0; j < c; j++)
    {
      *پہلا_advance = hb_clamp_مقصد<hb_position_t> (
	  round (advances[j].چوڑائی * y_mult) - (دوہرا) tracking);
      پہلا_advance = &Struct_At_انحراف<hb_position_t> (پہلا_advance, advance_stride);
    }
  }
}

ساکن hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				خالی *font_ڈیٹا HB_غیر_مستعمل,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_سائز;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_سائز;

  ثابت CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_مقصد<hb_position_t> (round (x_mult * origin.چوڑائی));
  *y = hb_clamp_مقصد<hb_position_t> (round (y_mult * origin.اونچائی));

  واپس سچ;
}
#اگر_ختم

ساکن hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       خالی *font_ڈیٹا HB_غیر_مستعمل,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائز;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, تہی, 1);

  دوہرا y_bearing = round ((bounds.origin.y + bounds.سائز.اونچائی) * y_mult);

  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
  extents->چوڑائی = hb_clamp_مقصد<hb_position_t> (round (bounds.سائز.چوڑائی * x_mult));
  extents->اونچائی = hb_clamp_مقصد<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  واپس سچ;
}

ساکن hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				خالی *font_ڈیٹا HB_غیر_مستعمل,
				hb_font_extents_t *metrics,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائز;

  metrics->ascender = hb_clamp_مقصد<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_مقصد<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  واپس سچ;
}

#اگر_غیر_تعریف_شدہ HB_NO_DRAW

ساکن خالی
ct_apply_func (خالی *info, ثابت CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  انتخاب (element->type)
  {
  حالت k_CGPath_Element_Move_مقصد_نقطہ:
    draws->move_مقصد (element->points[0].x, element->points[0].y);
    توڑو;
  حالت k_CGPath_Element_جوڑو_سطر_مقصد_نقطہ:
    draws->سطر_مقصد (element->points[0].x, element->points[0].y);
    توڑو;
  حالت k_CGPath_Element_جوڑو_Quad_خم_مقصد_نقطہ:
    draws->quadratic_مقصد (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    توڑو;
  حالت k_CGPath_Element_جوڑو_خم_مقصد_نقطہ:
    draws->cubic_مقصد (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    توڑو;
  حالت kCGPathElementCloseSubpath:
    draws->close_راستہ ();
    توڑو;
  }
}

ساکن hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				خالی *font_ڈیٹا HB_غیر_مستعمل,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, خالی *draw_ڈیٹا,
				خالی *user_ڈیٹا)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائز;

  CGAffineTransform transform = CGAffine_Transform_شناختی_عنصر;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> راستہ (CTFont_Create_راستہ_For_Glyph (ct_font, glyph, &transform));
  اگر (!راستہ)
    واپس جھوٹ;

  hb_draw_session_t drawing {draw_funcs, draw_ڈیٹا};

  CGPathApply (راستہ, &drawing, ct_apply_func);

  واپس سچ;
}
#اگر_ختم

ساکن hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    خالی *font_ڈیٹا HB_غیر_مستعمل,
			    hb_codepoint_t glyph,
			    حرف *name, غیرعلامتی صحیح سائز,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CGFontRef cg_font = (CGFontRef) (ثابت خالی *) font->face->ڈیٹا.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  اگر (!cf_name)
    واپس جھوٹ;

  اگر (!سائز)
    واپس سچ;

  CFIndex len = CFString_Get_لمبائی (cf_name);
  اگر (len > (CFIndex)سائز - 1)
    len = (CFIndex)سائز - 1;

  CFString_Get_بائٹیں (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, جھوٹ,
		    (UInt8 *) name, سائز, &len);

  name[len] = '\0';

  واپس سچ;
}

ساکن hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 خالی *font_ڈیٹا HB_غیر_مستعمل,
				 ثابت حرف *name, صحیح len,
				 hb_codepoint_t *glyph,
				 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  اگر (len == -1)
    len = لڑی_لمبائی (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_بائٹیں (kCFAllocatorDefault,
							     (ثابت UInt8 *) name, len,
							     kCFStringEncodingUTF8, جھوٹ));
  CGGlyph cg_glyph = CTFontGetGlyphWithName (ct_font, cf_name);
  *glyph = cg_glyph;

  // TODO Return true for .notdef; hb-ft does that.

  واپس cg_glyph != 0;
}


ساکن اندرونی خالی free_static_coretext_funcs ();

ساکن ساخت hb_coretext_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_coretext_font_funcs_lazy_loader_t>
{
  ساکن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, خالی_اشاریہ, خالی_اشاریہ);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_GLYPH_نام
    hb_font_funcs_سیٹ_glyph_name_func (funcs, hb_coretext_get_glyph_name, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_coretext_funcs);

    واپس funcs;
  }
} static_coretext_funcs;

ساکن اندرونی
خالی free_static_coretext_funcs ()
{
  static_coretext_funcs.free_instance ();
}

ساکن hb_font_funcs_t *
_hb_coretext_get_font_funcs ()
{
  واپس static_coretext_funcs.get_unconst ();
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
خالی
hb_coretext_font_سیٹ_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  اگر (unlikely (!ct_font))
  {
    hb_font_سیٹ_funcs (font,
		       hb_font_funcs_get_خالی_خانے (),
		       خالی_اشاریہ, خالی_اشاریہ);
    واپس;
  }

  hb_font_سیٹ_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     خالی_اشاریہ, خالی_اشاریہ);
}

#تعریف_منسوخ MAX_GLYPHS

#اگر_ختم
