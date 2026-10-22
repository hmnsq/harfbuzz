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

#جے_تعریف_شدہ HAVE_DIRECTWRITE

#ضمن "hb-directwrite.h"

#ضمن <d2d1.h>

#ضمن "hb-draw.hh"
#ضمن "hb-font.hh"
#ضمن "hb-machinery.hh"

#تعریف_کرو MAX_GLYPHS 256u

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

ساكن بنانشانی صحيح
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   فراغ *font_انکڑے HB_بن_ورتیا,
				   بنانشانی صحيح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   بنانشانی صحيح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   بنانشانی صحيح glyph_stride,
				   فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  لكل (بنانشانی i = 0; i < گنتی;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    بنانشانی n = hb_min (MAX_GLYPHS, گنتی - i);

    لكل (بنانشانی j = 0; j < n; j++)
    {
      unicodes[j] = *پہلا_unicode;
      پہلا_unicode = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
    }

    إذا (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      أرجع i;

    لكل (بنانشانی j = 0; j < n; j++)
    {
      إذا (!gids[j])
        أرجع i + j;
      *پہلا_glyph = gids[j];
      پہلا_glyph = &Struct_At_وتھ<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }

    i += n;
  }

  أرجع گنتی;
}

ساكن hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   فراغ *font_انکڑے HB_بن_ورتیا,
				   hb_font_extents_t *metrics,
				   فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->سطر_gap = hb_directwrite_scale_y (font, dw_metrics.سطر_Gap);

  أرجع سچ;
}

ساكن فراغ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     فراغ* font_انکڑے HB_بن_ورتیا,
				     بنانشانی گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph,
				     بنانشانی glyph_stride,
				     hb_position_t *پہلا_advance,
				     بنانشانی advance_stride,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  IDWriteFontFace1 *dw_face1 = خالی_سوچک;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  بنانشانی صحيح عدد_glyphs = font->face->get_عدد_glyphs ();

  لكل (بنانشانی i = 0; i < گنتی;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بنانشانی n = hb_min (MAX_GLYPHS, گنتی - i);

    لكل (بنانشانی j = 0; j < n; j++)
    {
      gids[j] = *پہلا_glyph;
      advances[j] = 0;
      پہلا_glyph = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, خطأ);
    لكل (بنانشانی j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      خودکار advance = gids[j] < عدد_glyphs ? advances[j] : 0;
      *پہلا_advance = hb_directwrite_scale_x (font, advance);
      پہلا_advance = &Struct_At_وتھ<hb_position_t> (پہلا_advance, advance_stride);
    }

    i += n;
  }
}

#جے_بنا_تعریف HB_NO_VERTICAL

ساكن فراغ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     فراغ* font_انکڑے HB_بن_ورتیا,
				     بنانشانی گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph,
				     بنانشانی glyph_stride,
				     hb_position_t *پہلا_advance,
				     بنانشانی advance_stride,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  IDWriteFontFace1 *dw_face1 = خالی_سوچک;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (فراغ**)&dw_face1);
  أكد (dw_face1);

  لكل (بنانشانی i = 0; i < گنتی;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    بنانشانی n = hb_min (MAX_GLYPHS, گنتی - i);

    لكل (بنانشانی j = 0; j < n; j++)
    {
      gids[j] = *پہلا_glyph;
      advances[j] = 0;
      پہلا_glyph = &Struct_At_وتھ<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, سچ);
    لكل (بنانشانی j = 0; j < n; j++)
    {
      *پہلا_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      پہلا_advance = &Struct_At_وتھ<hb_position_t> (پہلا_advance, advance_stride);
    }

    i += n;
  }
}

ساكن hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   فراغ *font_انکڑے HB_بن_ورتیا,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  *x = hb_directwrite_scale_x (font, metrics.advance_چوڑائی / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  أرجع سچ;
}
#جے_انت

ساكن hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  فراغ *font_انکڑے HB_بن_ورتیا,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  فراغ *user_انکڑے HB_بن_ورتیا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  إذا (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    أرجع خطأ;

  int64_t سجے = (int64_t) metrics.advance_چوڑائی - metrics.دایاں_Side_Bearing;
  int64_t چوٹی_2 = (int64_t) metrics.verticalOriginY - metrics.چوٹی_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_اچائی + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.بایاں_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, چوٹی_2);
  extents->چوڑائی = hb_saturate_sub (hb_directwrite_scale_x (font, سجے), extents->x_bearing);
  extents->اچائی = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  أرجع سچ;
}


#جے_بنا_تعریف HB_NO_DRAW

صنف GeometrySink : عام IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

عام:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       فراغ *draw_انکڑے)
    : font (font), drawing ({draw_funcs, draw_انکڑے}) {}

  HRESULT STDMETHODCALLTYPE Close() override { أرجع S_OK; }
  فراغ STDMETHODCALLTYPE سیٹ_Fill_منوال(D2_D1_FILL_منوال) override {}
  فراغ STDMETHODCALLTYPE سیٹ_Segment_Flags(D2_D1_رستہ_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, فراغ **) override { أرجع E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, جوڑو_Ref)() override { أرجع 1; }
  IFACEMETHOD_(ULONG, Release)() override { أرجع 1; }

  فراغ STDMETHODCALLTYPE BeginFigure(D2_D1_نقطہ_2_F شروع_نقطہ, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_منزل (font->em_scalef_x (شروع_نقطہ.x), -(عائم) font->em_scalef_y (شروع_نقطہ.y));
  }

  فراغ STDMETHODCALLTYPE جوڑو_Beziers(ثابت D2D1_BEZIER_SEGMENT *beziers, UINT beziers_گنتی) override
  {
    لكل (بنانشانی i = 0; i < beziers_گنتی; ++i)
      drawing.cubic_منزل (font->em_scalef_x (beziers[i].point1.x), -(عائم) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(عائم) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(عائم) font->em_scalef_y (beziers[i].point3.y));
  }

  فراغ STDMETHODCALLTYPE جوڑو_سطریں(ثابت D2_D1_نقطہ_2_F *points, UINT points_گنتی) override
  {
    لكل (بنانشانی i = 0; i < points_گنتی; ++i)
      drawing.سطر_منزل (font->em_scalef_x (points[i].x), -(عائم) font->em_scalef_y (points[i].y));
  }

  فراغ STDMETHODCALLTYPE سرا_Figure(D2_D1_FIGURE_سرا) override
  {
    drawing.close_رستہ ();
  }
};

ساكن hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   فراغ *font_انکڑے HB_بن_ورتیا,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, فراغ *draw_انکڑے,
				   فراغ *user_انکڑے)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;

  GeometrySink sink (font, draw_funcs, draw_انکڑے);
  UINT16 gid = static_cast<UINT16>(glyph);
  بنانشانی upem = font->face->get_upem();

  أرجع S_OK == dw_face->Get_Glyph_لگاتار_Outline (upem,
					      &gid, خالی_سوچک, خالی_سوچک,
					      1,
					      خطأ, خطأ,
					      &sink);
}

#جے_انت

ساكن مضمن فراغ free_static_directwrite_funcs ();

ساكن بنية hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ساكن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, خالی_سوچک, خالی_سوچک);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, خالی_سوچک, خالی_سوچک);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, خالی_سوچک, خالی_سوچک);
#جے_انت

#جے_بنا_تعریف HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, خالی_سوچک, خالی_سوچک);
#جے_انت

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, خالی_سوچک, خالی_سوچک);

#جے_بنا_تعریف HB_NO_OT_FONT_GLYPH_ناں
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#جے_انت

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
hb_directwrite_font_سیٹ_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت فراغ *) font->انکڑے.directwrite;
  إذا (unlikely (!dw_face))
  {
    hb_font_سیٹ_funcs (font,
		       hb_font_funcs_get_خالی_تھاواں (),
		       خالی_سوچک, خالی_سوچک);
    أرجع;
  }

  dw_face->جوڑو_Ref ();
  hb_font_سیٹ_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     خالی_سوچک, خالی_سوچک);
}

#تعریف_رد MAX_GLYPHS

#جے_انت
