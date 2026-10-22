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

#اگر_تعریف_شده HAVE_CORETEXT

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
#پایان_اگر

#تعریف_کن MAX_GLYPHS 256u

ساكن hb_bool_t
hb_coretext_get_nominal_glyph (hb_font_t *font,
			       فراغ *font_داده HB_استفادهنشده,
			       hb_codepoint_t unicode,
			       hb_codepoint_t *glyph,
			       فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  UniChar ch[2];
  CGGlyph cg_glyph[2];
  بیعلامت شمار = 0;

  إذا (unicode <= 0xFFFF)
  {
    ch[شمار++] = unicode;
  }
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[شمار++] = (unicode >> 10) + 0xD7C0;
    ch[شمار++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[شمار++] = 0xFFFD;

  إذا (CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, شمار))
  {
    *glyph = cg_glyph[0];
    أرجع درست;
  }
  أرجع خطأ;
}

ساكن بیعلامت صحيح
hb_coretext_get_nominal_glyphs (hb_font_t *font,
				فراغ *font_داده,
				بیعلامت صحيح شمار,
				ثابت hb_codepoint_t *اولی_unicode,
				بیعلامت صحيح unicode_stride,
				hb_codepoint_t *اولی_glyph,
				بیعلامت صحيح glyph_stride,
				فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  // If any non-BMP codepoint is requested, use the slow path.
  منطقي slow_مسیر = خطأ;
  خودکار *unicode = اولی_unicode;
  لكل (بیعلامت i = 0; i < شمار; i++)
  {
    إذا (*unicode > 0xFFFF)
    {
      slow_مسیر = درست;
      اكسر;
    }
    unicode = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (unicode, unicode_stride);
  }

  إذا (unlikely (slow_مسیر))
  {
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      إذا (!hb_coretext_get_nominal_glyph (font, font_داده, *اولی_unicode, اولی_glyph, اشاره_تهی))
	أرجع i;
      اولی_unicode = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_unicode, unicode_stride);
      اولی_glyph = &Struct_At_جابه_جایی<hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    أرجع شمار;
  }

  UniChar ch[MAX_GLYPHS];
  CGGlyph cg_glyph[MAX_GLYPHS];
  لكل (بیعلامت i = 0; i < شمار; i += MAX_GLYPHS)
  {
    بیعلامت c = (بیعلامت) hb_min ((صحيح) MAX_GLYPHS, (صحيح) شمار - (صحيح) i);
    لكل (بیعلامت j = 0; j < c; j++)
    {
      ch[j] = *اولی_unicode;
      اولی_unicode = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_unicode, unicode_stride);
    }
    إذا (unlikely (!CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, c)))
    {
      // Use slow path partially and return at first failure.
      لكل (بیعلامت j = 0; j < c; j++)
      {
	إذا (!hb_coretext_get_nominal_glyph (font, font_داده, ch[j], اولی_glyph, اشاره_تهی))
	  أرجع i + j;
	اولی_glyph = &Struct_At_جابه_جایی<hb_codepoint_t> (اولی_glyph, glyph_stride);
      }
    }
    لكل (بیعلامت j = 0; j < c; j++)
    {
      *اولی_glyph = cg_glyph[j];
      اولی_glyph = &Struct_At_جابه_جایی<hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
  }

  أرجع شمار;
}

ساكن hb_bool_t
hb_coretext_get_variation_glyph (hb_font_t *font,
				 فراغ *font_داده HB_استفادهنشده,
				 hb_codepoint_t unicode,
				 hb_codepoint_t variation_selector,
				 hb_codepoint_t *glyph,
				 فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  UniChar ch[4];
  CGGlyph cg_glyph[4];
  بیعلامت شمار = 0;

  // Add Unicode, then variation selector. Ugly, but works.
  //
  إذا (unicode <= 0xFFFF)
    ch[شمار++] = unicode;
  وإلا إذا (unicode <= 0x10FFFF)
  {
    ch[شمار++] = (unicode >> 10) + 0xD7C0;
    ch[شمار++] = (unicode & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[شمار++] = 0xFFFD;

  إذا (variation_selector <= 0xFFFF)
    ch[شمار++] = variation_selector;
  وإلا إذا (variation_selector <= 0x10FFFF)
  {
    ch[شمار++] = (variation_selector >> 10) + 0xD7C0;
    ch[شمار++] = (variation_selector & 0x3FF) + 0xDC00;
  }
  وإلا
    ch[شمار++] = 0xFFFD;

  CTFontGetGlyphsForCharacters (ct_font, ch, cg_glyph, شمار);

  // All except for first should be zero if we succeeded
  لكل (بیعلامت i = 1; i < شمار; i++)
    إذا (cg_glyph[i])
      أرجع خطأ;

  // Humm. CoreText falls back to the default glyph if the variation selector
  // is not supported.  We cannot truly detect that case. So, in essence,
  // we are always returning true here...

  *glyph = cg_glyph[0];
  أرجع درست;
}

ساكن فراغ
hb_coretext_get_glyph_h_advances (hb_font_t* font,
				  فراغ* font_داده HB_استفادهنشده,
				  بیعلامت شمار,
				  ثابت hb_codepoint_t *اولی_glyph,
				  بیعلامت glyph_stride,
				  hb_position_t *اولی_advance,
				  بیعلامت advance_stride,
				  فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_اندازه;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_LTR, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بیعلامت i = 0; i < شمار; i += MAX_GLYPHS)
  {
    بیعلامت c = (بیعلامت) hb_min ((صحيح) MAX_GLYPHS, (صحيح) شمار - (صحيح) i);
    لكل (بیعلامت j = 0; j < c; j++)
    {
      cg_glyph[j] = *اولی_glyph;
      اولی_glyph = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationHorizontal, cg_glyph, advances, c);
    لكل (بیعلامت j = 0; j < c; j++)
    {
      *اولی_advance = hb_clamp_مقصد<hb_position_t> (
	  round (advances[j].عرض * x_mult) - (مزدوج) tracking);
      اولی_advance = &Struct_At_جابه_جایی<hb_position_t> (اولی_advance, advance_stride);
    }
  }
}

#اگر_تعریف_نشده HB_NO_VERTICAL
ساكن فراغ
hb_coretext_get_glyph_v_advances (hb_font_t* font,
				  فراغ* font_داده HB_استفادهنشده,
				  بیعلامت شمار,
				  ثابت hb_codepoint_t *اولی_glyph,
				  بیعلامت glyph_stride,
				  hb_position_t *اولی_advance,
				  بیعلامت advance_stride,
				  فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_اندازه;
  hb_position_t tracking = font->face->جدول.trak->get_tracking (font, HB_DIRECTION_TTB, 0.f);

  CGGlyph cg_glyph[MAX_GLYPHS];
  CGSize advances[MAX_GLYPHS];
  لكل (بیعلامت i = 0; i < شمار; i += MAX_GLYPHS)
  {
    بیعلامت c = (بیعلامت) hb_min ((صحيح) MAX_GLYPHS, (صحيح) شمار - (صحيح) i);
    لكل (بیعلامت j = 0; j < c; j++)
    {
      cg_glyph[j] = *اولی_glyph;
      اولی_glyph = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    CTFontGetAdvancesForGlyphs (ct_font, kCTFontOrientationVertical, cg_glyph, advances, c);
    لكل (بیعلامت j = 0; j < c; j++)
    {
      *اولی_advance = hb_clamp_مقصد<hb_position_t> (
	  round (advances[j].عرض * y_mult) - (مزدوج) tracking);
      اولی_advance = &Struct_At_جابه_جایی<hb_position_t> (اولی_advance, advance_stride);
    }
  }
}

ساكن hb_bool_t
hb_coretext_get_glyph_v_origin (hb_font_t *font,
				فراغ *font_داده HB_استفادهنشده,
				hb_codepoint_t glyph,
				hb_position_t *x,
				hb_position_t *y,
				فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat x_mult = -(CGFloat) font->x_scale / ct_font_اندازه;
  CGFloat y_mult = -(CGFloat) font->y_scale / ct_font_اندازه;

  ثابت CGGlyph glyphs = glyph;
  CGSize origin;
  CTFontGetVerticalTranslationsForGlyphs (ct_font, &glyphs, &origin, 1);

  *x = hb_clamp_مقصد<hb_position_t> (round (x_mult * origin.عرض));
  *y = hb_clamp_مقصد<hb_position_t> (round (y_mult * origin.ارتفاع));

  أرجع درست;
}
#پایان_اگر

ساكن hb_bool_t
hb_coretext_get_glyph_extents (hb_font_t *font,
			       فراغ *font_داده HB_استفادهنشده,
			       hb_codepoint_t glyph,
			       hb_glyph_extents_t *extents,
			       فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_اندازه;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_اندازه;

  CGGlyph glyphs[1] = { glyph };
  CGRect bounds = ::CTFontGetBoundingRectsForGlyphs(ct_font,
						    kCTFontOrientationDefault, glyphs, عدم, 1);

  مزدوج y_bearing = round ((bounds.origin.y + bounds.اندازه.ارتفاع) * y_mult);

  extents->x_bearing = hb_clamp_مقصد<hb_position_t> (round (bounds.origin.x * x_mult));
  extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
  extents->عرض = hb_clamp_مقصد<hb_position_t> (round (bounds.اندازه.عرض * x_mult));
  extents->ارتفاع = hb_clamp_مقصد<hb_position_t> (round (bounds.origin.y * y_mult) - y_bearing);

  أرجع درست;
}

ساكن hb_bool_t
hb_coretext_get_font_h_extents (hb_font_t *font,
				فراغ *font_داده HB_استفادهنشده,
				hb_font_extents_t *metrics,
				فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_اندازه;

  metrics->ascender = hb_clamp_مقصد<hb_position_t> (round (CTFontGetAscent (ct_font) * y_mult));
  metrics->descender = hb_clamp_مقصد<hb_position_t> (-round (CTFontGetDescent (ct_font) * y_mult));
  metrics->سطر_gap = hb_clamp_مقصد<hb_position_t> (round (CTFontGetLeading (ct_font) * y_mult));

  أرجع درست;
}

#اگر_تعریف_نشده HB_NO_DRAW

ساكن فراغ
ct_apply_func (فراغ *info, ثابت CGPathElement *element)
{
  hb_draw_session_t *draws = (hb_draw_session_t *) info;

  بدل (element->type)
  {
  حالة k_CGPath_Element_Move_مقصد_نقطه:
    draws->move_مقصد (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جمع_کن_سطر_مقصد_نقطه:
    draws->سطر_مقصد (element->points[0].x, element->points[0].y);
    اكسر;
  حالة k_CGPath_Element_جمع_کن_Quad_منحنی_مقصد_نقطه:
    draws->quadratic_مقصد (element->points[0].x, element->points[0].y,
			 element->points[1].x, element->points[1].y);
    اكسر;
  حالة k_CGPath_Element_جمع_کن_منحنی_مقصد_نقطه:
    draws->cubic_مقصد (element->points[0].x, element->points[0].y,
		     element->points[1].x, element->points[1].y,
		     element->points[2].x, element->points[2].y);
    اكسر;
  حالة kCGPathElementCloseSubpath:
    draws->close_مسیر ();
    اكسر;
  }
}

ساكن hb_bool_t
hb_coretext_draw_glyph_or_fail (hb_font_t *font,
				فراغ *font_داده HB_استفادهنشده,
				hb_codepoint_t glyph,
				hb_draw_funcs_t *draw_funcs, فراغ *draw_داده,
				فراغ *user_داده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_اندازه;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_اندازه;

  CGAffineTransform transform = CGAffine_Transform_عنصر_خنثی;
  transform = CGAffineTransformScale (transform, x_mult, y_mult);

  hb_cf_ptr_t<CGPathRef> مسیر (CTFont_Create_مسیر_For_Glyph (ct_font, glyph, &transform));
  إذا (!مسیر)
    أرجع خطأ;

  hb_draw_session_t drawing {draw_funcs, draw_داده};

  CGPathApply (مسیر, &drawing, ct_apply_func);

  أرجع درست;
}
#پایان_اگر

ساكن hb_bool_t
hb_coretext_get_glyph_name (hb_font_t *font,
			    فراغ *font_داده HB_استفادهنشده,
			    hb_codepoint_t glyph,
			    حرف *name, بیعلامت صحيح اندازه,
			    فراغ *user_داده HB_استفادهنشده)
{
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) font->face->داده.coretext;

  CGGlyph cg_glyph = glyph;
  hb_cf_ptr_t<CFStringRef> cf_name (CGFontCopyGlyphNameForGlyph (cg_font, cg_glyph));
  إذا (!cf_name)
    أرجع خطأ;

  إذا (!اندازه)
    أرجع درست;

  CFIndex len = CFString_Get_درازی (cf_name);
  إذا (len > (CFIndex)اندازه - 1)
    len = (CFIndex)اندازه - 1;

  CFString_Get_بایتها (cf_name, CFRangeMake (0, len),
		    kCFStringEncodingUTF8, 0, خطأ,
		    (UInt8 *) name, اندازه, &len);

  name[len] = '\0';

  أرجع درست;
}

ساكن hb_bool_t
hb_coretext_get_glyph_from_name (hb_font_t *font,
				 فراغ *font_داده HB_استفادهنشده,
				 ثابت حرف *name, صحيح len,
				 hb_codepoint_t *glyph,
				 فراغ *user_داده HB_استفادهنشده)
{
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  إذا (len == -1)
    len = طول_رشته (name);

  hb_cf_ptr_t<CFStringRef> cf_name (CFString_Create_With_بایتها (kCFAllocatorDefault,
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

    hb_font_funcs_مجموعه_nominal_glyph_func (funcs, hb_coretext_get_nominal_glyph, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_nominal_glyphs_func (funcs, hb_coretext_get_nominal_glyphs, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_variation_glyph_func (funcs, hb_coretext_get_variation_glyph, اشاره_تهی, اشاره_تهی);

    hb_font_funcs_مجموعه_font_h_extents_func (funcs, hb_coretext_get_font_h_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_h_advances_func (funcs, hb_coretext_get_glyph_h_advances, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_VERTICAL
    hb_font_funcs_مجموعه_glyph_v_advances_func (funcs, hb_coretext_get_glyph_v_advances, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_v_origin_func (funcs, hb_coretext_get_glyph_v_origin, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_DRAW
    hb_font_funcs_مجموعه_draw_glyph_or_fail_func (funcs, hb_coretext_draw_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر

    hb_font_funcs_مجموعه_glyph_extents_func (funcs, hb_coretext_get_glyph_extents, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_OT_FONT_GLYPH_نامها
    hb_font_funcs_مجموعه_glyph_name_func (funcs, hb_coretext_get_glyph_name, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_from_name_func (funcs, hb_coretext_get_glyph_from_name, اشاره_تهی, اشاره_تهی);
#پایان_اگر

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
hb_coretext_font_مجموعه_funcs (hb_font_t *font)
{
  CTFontRef ct_font = hb_coretext_font_get_ct_font (font);
  إذا (unlikely (!ct_font))
  {
    hb_font_مجموعه_funcs (font,
		       hb_font_funcs_get_خالی (),
		       اشاره_تهی, اشاره_تهی);
    أرجع;
  }

  hb_font_مجموعه_funcs (font,
		     _hb_coretext_get_font_funcs (),
		     اشاره_تهی, اشاره_تهی);
}

#لغو_تعریف MAX_GLYPHS

#پایان_اگر
