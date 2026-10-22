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

#جيڪڏهن_وصف_ٿيل HAVE_DIRECTWRITE

#ضمن "hb-directwrite.h"

#ضمن <d2d1.h>

#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"

#وصف_ڪريو MAX_GLYPHS 256u

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

ساكن بنانشان صحيح
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				   بنانشان صحيح ڳڻپ,
				   ثابت hb_codepoint_t *پهريون_unicode,
				   بنانشان صحيح unicode_stride,
				   hb_codepoint_t *پهريون_glyph,
				   بنانشان صحيح glyph_stride,
				   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  لكل (بنانشان i = 0; i < ڳڻپ;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    بنانشان n = hb_min (MAX_GLYPHS, ڳڻپ - i);

    لكل (بنانشان j = 0; j < n; j++)
    {
      unicodes[j] = *پهريون_unicode;
      پهريون_unicode = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_unicode, unicode_stride);
    }

    إذا (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      أرجع i;

    لكل (بنانشان j = 0; j < n; j++)
    {
      إذا (!gids[j])
        أرجع i + j;
      *پهريون_glyph = gids[j];
      پهريون_glyph = &Struct_At_وٿي<hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }

    i += n;
  }

  أرجع ڳڻپ;
}

ساكن hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				   hb_font_extents_t *metrics,
				   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->سٽ_gap = hb_directwrite_scale_y (font, dw_metrics.سٽ_Gap);

  أرجع سچ;
}

ساكن فراغ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     فراغ* font_معلومات HB_اڻ_استعمال_ٿيل,
				     بنانشان ڳڻپ,
				     ثابت hb_codepoint_t *پهريون_glyph,
				     بنانشان glyph_stride,
				     hb_position_t *پهريون_advance,
				     بنانشان advance_stride,
				     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  IDWriteFontFace1 *dw_face1 = خالي_اشارو;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  بنانشان صحيح انگ_glyphs = font->face->get_انگ_glyphs ();

  لكل (بنانشان i = 0; i < ڳڻپ;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بنانشان n = hb_min (MAX_GLYPHS, ڳڻپ - i);

    لكل (بنانشان j = 0; j < n; j++)
    {
      gids[j] = *پهريون_glyph;
      advances[j] = 0;
      پهريون_glyph = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, خطأ);
    لكل (بنانشان j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      خودڪار advance = gids[j] < انگ_glyphs ? advances[j] : 0;
      *پهريون_advance = hb_directwrite_scale_x (font, advance);
      پهريون_advance = &Struct_At_وٿي<hb_position_t> (پهريون_advance, advance_stride);
    }

    i += n;
  }
}

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL

ساكن فراغ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     فراغ* font_معلومات HB_اڻ_استعمال_ٿيل,
				     بنانشان ڳڻپ,
				     ثابت hb_codepoint_t *پهريون_glyph,
				     بنانشان glyph_stride,
				     hb_position_t *پهريون_advance,
				     بنانشان advance_stride,
				     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  IDWriteFontFace1 *dw_face1 = خالي_اشارو;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  لكل (بنانشان i = 0; i < ڳڻپ;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بنانشان n = hb_min (MAX_GLYPHS, ڳڻپ - i);

    لكل (بنانشان j = 0; j < n; j++)
    {
      gids[j] = *پهريون_glyph;
      advances[j] = 0;
      پهريون_glyph = &Struct_At_وٿي<ثابت hb_codepoint_t> (پهريون_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, سچ);
    لكل (بنانشان j = 0; j < n; j++)
    {
      *پهريون_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      پهريون_advance = &Struct_At_وٿي<hb_position_t> (پهريون_advance, advance_stride);
    }

    i += n;
  }
}

ساكن hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  *x = hb_directwrite_scale_x (font, metrics.advance_ويڪر / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  أرجع سچ;
}
#جيڪڏهن_پورو

ساكن hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  int64_t ساڄي = (int64_t) metrics.advance_ويڪر - metrics.ساڄو_Side_Bearing;
  int64_t چوٽي_2 = (int64_t) metrics.verticalOriginY - metrics.چوٽي_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_اوچائي + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.کاٻو_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, چوٽي_2);
  extents->ويڪر = hb_saturate_sub (hb_directwrite_scale_x (font, ساڄي), extents->x_bearing);
  extents->اوچائي = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  أرجع سچ;
}


#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW

صنف GeometrySink : عام IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

عام:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       فراغ *draw_معلومات)
    : font (font), drawing ({draw_funcs, draw_معلومات}) {}

  HRESULT STDMETHODCALLTYPE Close() override { أرجع S_OK; }
  فراغ STDMETHODCALLTYPE سيٽ_Fill_گهڻائي(D2_D1_FILL_گهڻائي) override {}
  فراغ STDMETHODCALLTYPE سيٽ_Segment_Flags(D2_D1_رستو_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, فراغ **) override { أرجع E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, جوڙيو_Ref)() override { أرجع 1; }
  IFACEMETHOD_(ULONG, Release)() override { أرجع 1; }

  فراغ STDMETHODCALLTYPE BeginFigure(D2_D1_نقطو_2_F شروعات_نقطو, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_منزل (font->em_scalef_x (شروعات_نقطو.x), -(عائم) font->em_scalef_y (شروعات_نقطو.y));
  }

  فراغ STDMETHODCALLTYPE جوڙيو_Beziers(ثابت D2D1_BEZIER_SEGMENT *beziers, UINT beziers_ڳڻپ) override
  {
    لكل (بنانشان i = 0; i < beziers_ڳڻپ; ++i)
      drawing.cubic_منزل (font->em_scalef_x (beziers[i].point1.x), -(عائم) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(عائم) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(عائم) font->em_scalef_y (beziers[i].point3.y));
  }

  فراغ STDMETHODCALLTYPE جوڙيو_سٽون(ثابت D2_D1_نقطو_2_F *points, UINT points_ڳڻپ) override
  {
    لكل (بنانشان i = 0; i < points_ڳڻپ; ++i)
      drawing.سٽ_منزل (font->em_scalef_x (points[i].x), -(عائم) font->em_scalef_y (points[i].y));
  }

  فراغ STDMETHODCALLTYPE پڇاڙي_Figure(D2_D1_FIGURE_پڇاڙي) override
  {
    drawing.close_رستو ();
  }
};

ساكن hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   فراغ *font_معلومات HB_اڻ_استعمال_ٿيل,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, فراغ *draw_معلومات,
				   فراغ *user_معلومات)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  GeometrySink sink (font, draw_funcs, draw_معلومات);
  UINT16 gid = static_cast<UINT16>(glyph);
  بنانشان upem = font->face->get_upem();

  أرجع S_OK == dw_face->Get_Glyph_لاڳيتو_Outline (upem,
					      &gid, خالي_اشارو, خالي_اشارو,
					      1,
					      خطأ, خطأ,
					      &sink);
}

#جيڪڏهن_پورو

ساكن مضمن فراغ free_static_directwrite_funcs ();

ساكن بنية hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سيٽ_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, خالي_اشارو, خالي_اشارو);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_سيٽ_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, خالي_اشارو, خالي_اشارو);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VERTICAL
    hb_font_funcs_سيٽ_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, خالي_اشارو, خالي_اشارو);
    hb_font_funcs_سيٽ_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, خالي_اشارو, خالي_اشارو);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_DRAW
    hb_font_funcs_سيٽ_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, خالي_اشارو, خالي_اشارو);
#جيڪڏهن_پورو

    hb_font_funcs_سيٽ_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, خالي_اشارو, خالي_اشارو);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_FONT_GLYPH_نالا
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#جيڪڏهن_پورو

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
hb_directwrite_font_سيٽ_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;
  إذا (unlikely (!dw_face))
  {
    hb_font_سيٽ_funcs (font,
		       hb_font_funcs_get_خالي_جايون (),
		       خالي_اشارو, خالي_اشارو);
    أرجع;
  }

  dw_face->جوڙيو_Ref ();
  hb_font_سيٽ_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     خالي_اشارو, خالي_اشارو);
}

#وصف_رد MAX_GLYPHS

#جيڪڏهن_پورو
