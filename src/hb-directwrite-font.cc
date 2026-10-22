/*
 * Copyright © 2025  Google, Inc.
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

#اگر_تعریف_شده HAVE_DIRECTWRITE

#ضمن "hb-directwrite.h"

#ضمن <d2d1.h>

#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"

#تعریف_کن MAX_GLYPHS 256u

ساكن hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  إذا (value >= INT16_MIN && value <= INT16_MAX)
    أرجع font->em_scale_x ((int16_t) value);
  أرجع font->em_scalef_x ((عائم) value);
}

ساكن hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  إذا (value >= INT16_MIN && value <= INT16_MAX)
    أرجع font->em_scale_y ((int16_t) value);
  أرجع font->em_scalef_y ((عائم) value);
}

ساكن بیعلامت صحيح
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   فراغ *font_داده HB_استفادهنشده,
				   بیعلامت صحيح شمار,
				   ثابت hb_codepoint_t *اولی_unicode,
				   بیعلامت صحيح unicode_stride,
				   hb_codepoint_t *اولی_glyph,
				   بیعلامت صحيح glyph_stride,
				   فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  لكل (بیعلامت i = 0; i < شمار;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    بیعلامت n = hb_min (MAX_GLYPHS, شمار - i);

    لكل (بیعلامت j = 0; j < n; j++)
    {
      unicodes[j] = *اولی_unicode;
      اولی_unicode = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_unicode, unicode_stride);
    }

    إذا (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      أرجع i;

    لكل (بیعلامت j = 0; j < n; j++)
    {
      إذا (!gids[j])
        أرجع i + j;
      *اولی_glyph = gids[j];
      اولی_glyph = &Struct_At_جابه_جایی<hb_codepoint_t> (اولی_glyph, glyph_stride);
    }

    i += n;
  }

  أرجع شمار;
}

ساكن hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   فراغ *font_داده HB_استفادهنشده,
				   hb_font_extents_t *metrics,
				   فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->سطر_gap = hb_directwrite_scale_y (font, dw_metrics.سطر_Gap);

  أرجع درست;
}

ساكن فراغ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     فراغ* font_داده HB_استفادهنشده,
				     بیعلامت شمار,
				     ثابت hb_codepoint_t *اولی_glyph,
				     بیعلامت glyph_stride,
				     hb_position_t *اولی_advance,
				     بیعلامت advance_stride,
				     فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  IDWriteFontFace1 *dw_face1 = اشاره_تهی;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  بیعلامت صحيح رقم_glyphs = font->face->get_رقم_glyphs ();

  لكل (بیعلامت i = 0; i < شمار;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بیعلامت n = hb_min (MAX_GLYPHS, شمار - i);

    لكل (بیعلامت j = 0; j < n; j++)
    {
      gids[j] = *اولی_glyph;
      advances[j] = 0;
      اولی_glyph = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, خطأ);
    لكل (بیعلامت j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      خودکار advance = gids[j] < رقم_glyphs ? advances[j] : 0;
      *اولی_advance = hb_directwrite_scale_x (font, advance);
      اولی_advance = &Struct_At_جابه_جایی<hb_position_t> (اولی_advance, advance_stride);
    }

    i += n;
  }
}

#اگر_تعریف_نشده HB_NO_VERTICAL

ساكن فراغ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     فراغ* font_داده HB_استفادهنشده,
				     بیعلامت شمار,
				     ثابت hb_codepoint_t *اولی_glyph,
				     بیعلامت glyph_stride,
				     hb_position_t *اولی_advance,
				     بیعلامت advance_stride,
				     فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  IDWriteFontFace1 *dw_face1 = اشاره_تهی;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  لكل (بیعلامت i = 0; i < شمار;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بیعلامت n = hb_min (MAX_GLYPHS, شمار - i);

    لكل (بیعلامت j = 0; j < n; j++)
    {
      gids[j] = *اولی_glyph;
      advances[j] = 0;
      اولی_glyph = &Struct_At_جابه_جایی<ثابت hb_codepoint_t> (اولی_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, درست);
    لكل (بیعلامت j = 0; j < n; j++)
    {
      *اولی_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      اولی_advance = &Struct_At_جابه_جایی<hb_position_t> (اولی_advance, advance_stride);
    }

    i += n;
  }
}

ساكن hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   فراغ *font_داده HB_استفادهنشده,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  *x = hb_directwrite_scale_x (font, metrics.advance_عرض / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  أرجع درست;
}
#پایان_اگر

ساكن hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  فراغ *font_داده HB_استفادهنشده,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  فراغ *user_داده HB_استفادهنشده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  int64_t راست = (int64_t) metrics.advance_عرض - metrics.راست_Side_Bearing;
  int64_t بالاترین_2 = (int64_t) metrics.verticalOriginY - metrics.بالاترین_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_ارتفاع + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.چپ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, بالاترین_2);
  extents->عرض = hb_saturate_sub (hb_directwrite_scale_x (font, راست), extents->x_bearing);
  extents->ارتفاع = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  أرجع درست;
}


#اگر_تعریف_نشده HB_NO_DRAW

صنف GeometrySink : عام IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

عام:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       فراغ *draw_داده)
    : font (font), drawing ({draw_funcs, draw_داده}) {}

  HRESULT STDMETHODCALLTYPE Close() override { أرجع S_OK; }
  فراغ STDMETHODCALLTYPE مجموعه_Fill_نما(D2_D1_FILL_نما) override {}
  فراغ STDMETHODCALLTYPE مجموعه_Segment_Flags(D2_D1_مسیر_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, فراغ **) override { أرجع E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, جمع_کن_Ref)() override { أرجع 1; }
  IFACEMETHOD_(ULONG, Release)() override { أرجع 1; }

  فراغ STDMETHODCALLTYPE BeginFigure(D2_D1_نقطه_2_F شروع_نقطه, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_مقصد (font->em_scalef_x (شروع_نقطه.x), -(عائم) font->em_scalef_y (شروع_نقطه.y));
  }

  فراغ STDMETHODCALLTYPE جمع_کن_Beziers(ثابت D2D1_BEZIER_SEGMENT *beziers, UINT beziers_شمار) override
  {
    لكل (بیعلامت i = 0; i < beziers_شمار; ++i)
      drawing.cubic_مقصد (font->em_scalef_x (beziers[i].point1.x), -(عائم) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(عائم) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(عائم) font->em_scalef_y (beziers[i].point3.y));
  }

  فراغ STDMETHODCALLTYPE جمع_کن_سطرها(ثابت D2_D1_نقطه_2_F *points, UINT points_شمار) override
  {
    لكل (بیعلامت i = 0; i < points_شمار; ++i)
      drawing.سطر_مقصد (font->em_scalef_x (points[i].x), -(عائم) font->em_scalef_y (points[i].y));
  }

  فراغ STDMETHODCALLTYPE انتها_Figure(D2_D1_FIGURE_انتها) override
  {
    drawing.close_مسیر ();
  }
};

ساكن hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   فراغ *font_داده HB_استفادهنشده,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, فراغ *draw_داده,
				   فراغ *user_داده)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;

  GeometrySink sink (font, draw_funcs, draw_داده);
  UINT16 gid = static_cast<UINT16>(glyph);
  بیعلامت upem = font->face->get_upem();

  أرجع S_OK == dw_face->Get_Glyph_پیاپی_Outline (upem,
					      &gid, اشاره_تهی, اشاره_تهی,
					      1,
					      خطأ, خطأ,
					      &sink);
}

#پایان_اگر

ساكن مضمن فراغ free_static_directwrite_funcs ();

ساكن بنية hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_مجموعه_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, اشاره_تهی, اشاره_تهی);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_مجموعه_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_VERTICAL
    hb_font_funcs_مجموعه_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, اشاره_تهی, اشاره_تهی);
    hb_font_funcs_مجموعه_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, اشاره_تهی, اشاره_تهی);
#پایان_اگر

#اگر_تعریف_نشده HB_NO_DRAW
    hb_font_funcs_مجموعه_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, اشاره_تهی, اشاره_تهی);
#پایان_اگر

    hb_font_funcs_مجموعه_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, اشاره_تهی, اشاره_تهی);

#اگر_تعریف_نشده HB_NO_OT_FONT_GLYPH_نامها
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#پایان_اگر

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    أرجع funcs;
  }
} static_directwrite_funcs;

ساكن مضمن
فراغ free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

ساكن hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  أرجع static_directwrite_funcs.get_unconst ();
}

/**
 * hb_directwrite_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Configures the font-functions structure of the specified
 * #hb_font_t font object to use DirectWrite font functions.
 *
 * In particular, you can use this function to configure an
 * existing #hb_face_t face object for use with DirectWrite font
 * functions even if that #hb_face_t face object was initially
 * created with hb_face_create(), and therefore was not
 * initially configured to use DirectWrite font functions.
 *
 * <note>Note: Internally, this function creates a DirectWrite font.
 * </note>
 *
 * Since: 11.0.0
 **/
فراغ
hb_directwrite_font_مجموعه_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->داده.directwrite;
  إذا (unlikely (!dw_face))
  {
    hb_font_مجموعه_funcs (font,
		       hb_font_funcs_get_خالی (),
		       اشاره_تهی, اشاره_تهی);
    أرجع;
  }

  dw_face->جمع_کن_Ref ();
  hb_font_مجموعه_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     اشاره_تهی, اشاره_تهی);
}

#لغو_تعریف MAX_GLYPHS

#پایان_اگر
