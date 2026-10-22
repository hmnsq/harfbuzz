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

#که_تعریف_شوی HAVE_DIRECTWRITE

#ضمن "hb-directwrite.h"

#ضمن <d2d1.h>

#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"

#تعریف_کړه MAX_GLYPHS 256u

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

ساكن بینښه صحيح
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   فراغ *font_معلومات HB_ناکارول_شوی,
				   بینښه صحيح شمېر,
				   ثابت hb_codepoint_t *لومړی_unicode,
				   بینښه صحيح unicode_stride,
				   hb_codepoint_t *لومړی_glyph,
				   بینښه صحيح glyph_stride,
				   فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  لكل (بینښه i = 0; i < شمېر;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    بینښه n = hb_min (MAX_GLYPHS, شمېر - i);

    لكل (بینښه j = 0; j < n; j++)
    {
      unicodes[j] = *لومړی_unicode;
      لومړی_unicode = &Struct_At_دننه_واټن<ثابت hb_codepoint_t> (لومړی_unicode, unicode_stride);
    }

    إذا (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      أرجع i;

    لكل (بینښه j = 0; j < n; j++)
    {
      إذا (!gids[j])
        أرجع i + j;
      *لومړی_glyph = gids[j];
      لومړی_glyph = &Struct_At_دننه_واټن<hb_codepoint_t> (لومړی_glyph, glyph_stride);
    }

    i += n;
  }

  أرجع شمېر;
}

ساكن hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   فراغ *font_معلومات HB_ناکارول_شوی,
				   hb_font_extents_t *metrics,
				   فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->کرښه_gap = hb_directwrite_scale_y (font, dw_metrics.کرښه_Gap);

  أرجع ریښتیا;
}

ساكن فراغ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     فراغ* font_معلومات HB_ناکارول_شوی,
				     بینښه شمېر,
				     ثابت hb_codepoint_t *لومړی_glyph,
				     بینښه glyph_stride,
				     hb_position_t *لومړی_advance,
				     بینښه advance_stride,
				     فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  IDWriteFontFace1 *dw_face1 = تش_اشاره;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  بینښه صحيح عدد_glyphs = font->face->get_عدد_glyphs ();

  لكل (بینښه i = 0; i < شمېر;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بینښه n = hb_min (MAX_GLYPHS, شمېر - i);

    لكل (بینښه j = 0; j < n; j++)
    {
      gids[j] = *لومړی_glyph;
      advances[j] = 0;
      لومړی_glyph = &Struct_At_دننه_واټن<ثابت hb_codepoint_t> (لومړی_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, خطأ);
    لكل (بینښه j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      خپلکاری advance = gids[j] < عدد_glyphs ? advances[j] : 0;
      *لومړی_advance = hb_directwrite_scale_x (font, advance);
      لومړی_advance = &Struct_At_دننه_واټن<hb_position_t> (لومړی_advance, advance_stride);
    }

    i += n;
  }
}

#که_نه_تعریف_شوی HB_NO_VERTICAL

ساكن فراغ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     فراغ* font_معلومات HB_ناکارول_شوی,
				     بینښه شمېر,
				     ثابت hb_codepoint_t *لومړی_glyph,
				     بینښه glyph_stride,
				     hb_position_t *لومړی_advance,
				     بینښه advance_stride,
				     فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  IDWriteFontFace1 *dw_face1 = تش_اشاره;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  لكل (بینښه i = 0; i < شمېر;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بینښه n = hb_min (MAX_GLYPHS, شمېر - i);

    لكل (بینښه j = 0; j < n; j++)
    {
      gids[j] = *لومړی_glyph;
      advances[j] = 0;
      لومړی_glyph = &Struct_At_دننه_واټن<ثابت hb_codepoint_t> (لومړی_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ریښتیا);
    لكل (بینښه j = 0; j < n; j++)
    {
      *لومړی_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      لومړی_advance = &Struct_At_دننه_واټن<hb_position_t> (لومړی_advance, advance_stride);
    }

    i += n;
  }
}

ساكن hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   فراغ *font_معلومات HB_ناکارول_شوی,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  *x = hb_directwrite_scale_x (font, metrics.advance_پلنوالی / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  أرجع ریښتیا;
}
#د_که_پای

ساكن hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  فراغ *font_معلومات HB_ناکارول_شوی,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  فراغ *user_معلومات HB_ناکارول_شوی)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  int64_t ښی = (int64_t) metrics.advance_پلنوالی - metrics.ښی_Side_Bearing;
  int64_t سر_4 = (int64_t) metrics.verticalOriginY - metrics.سر_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_لوړوالی + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.کیڼ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, سر_4);
  extents->پلنوالی = hb_saturate_sub (hb_directwrite_scale_x (font, ښی), extents->x_bearing);
  extents->لوړوالی = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  أرجع ریښتیا;
}


#که_نه_تعریف_شوی HB_NO_DRAW

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
  فراغ STDMETHODCALLTYPE سېټ_Fill_ډېرپېښ(D2_D1_FILL_ډېرپېښ) override {}
  فراغ STDMETHODCALLTYPE سېټ_Segment_Flags(D2_D1_لار_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, فراغ **) override { أرجع E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, جمع_کړه_Ref)() override { أرجع 1; }
  IFACEMETHOD_(ULONG, Release)() override { أرجع 1; }

  فراغ STDMETHODCALLTYPE BeginFigure(D2_D1_ټکی_2_F پیل_ټکی, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_منزل (font->em_scalef_x (پیل_ټکی.x), -(عائم) font->em_scalef_y (پیل_ټکی.y));
  }

  فراغ STDMETHODCALLTYPE جمع_کړه_Beziers(ثابت D2D1_BEZIER_SEGMENT *beziers, UINT beziers_شمېر) override
  {
    لكل (بینښه i = 0; i < beziers_شمېر; ++i)
      drawing.cubic_منزل (font->em_scalef_x (beziers[i].point1.x), -(عائم) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(عائم) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(عائم) font->em_scalef_y (beziers[i].point3.y));
  }

  فراغ STDMETHODCALLTYPE جمع_کړه_کرښې(ثابت D2_D1_ټکی_2_F *points, UINT points_شمېر) override
  {
    لكل (بینښه i = 0; i < points_شمېر; ++i)
      drawing.کرښه_منزل (font->em_scalef_x (points[i].x), -(عائم) font->em_scalef_y (points[i].y));
  }

  فراغ STDMETHODCALLTYPE پای_ټکی_Figure(D2_D1_FIGURE_پای_ټکی) override
  {
    drawing.close_لار ();
  }
};

ساكن hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   فراغ *font_معلومات HB_ناکارول_شوی,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, فراغ *draw_معلومات,
				   فراغ *user_معلومات)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;

  GeometrySink sink (font, draw_funcs, draw_معلومات);
  UINT16 gid = static_cast<UINT16>(glyph);
  بینښه upem = font->face->get_upem();

  أرجع S_OK == dw_face->Get_Glyph_پرلپسې_Outline (upem,
					      &gid, تش_اشاره, تش_اشاره,
					      1,
					      خطأ, خطأ,
					      &sink);
}

#د_که_پای

ساكن مضمن فراغ free_static_directwrite_funcs ();

ساكن بنية hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سېټ_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, تش_اشاره, تش_اشاره);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_سېټ_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, تش_اشاره, تش_اشاره);

#که_نه_تعریف_شوی HB_NO_VERTICAL
    hb_font_funcs_سېټ_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, تش_اشاره, تش_اشاره);
    hb_font_funcs_سېټ_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, تش_اشاره, تش_اشاره);
#د_که_پای

#که_نه_تعریف_شوی HB_NO_DRAW
    hb_font_funcs_سېټ_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, تش_اشاره, تش_اشاره);
#د_که_پای

    hb_font_funcs_سېټ_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, تش_اشاره, تش_اشاره);

#که_نه_تعریف_شوی HB_NO_OT_FONT_GLYPH_نومونه
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#د_که_پای

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
hb_directwrite_font_سېټ_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->معلومات.directwrite;
  إذا (unlikely (!dw_face))
  {
    hb_font_سېټ_funcs (font,
		       hb_font_funcs_get_تشه (),
		       تش_اشاره, تش_اشاره);
    أرجع;
  }

  dw_face->جمع_کړه_Ref ();
  hb_font_سېټ_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     تش_اشاره, تش_اشاره);
}

#تعریف_لغوه MAX_GLYPHS

#د_که_پای
