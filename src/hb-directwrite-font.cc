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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_DIRECTWRITE

#شامل "hb-directwrite.h"

#شامل <d2d1.h>

#شامل "hb-draw.hh"
#شامل "hb-font.hh"
#شامل "hb-machinery.hh"

#تعریف_کرو MAX_GLYPHS 256u

ساکن hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  اگر (value >= INT16_MIN && value <= INT16_MAX)
    واپس font->em_scale_x ((int16_t) value);
  واپس font->em_scalef_x ((اعشاری) value);
}

ساکن hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  اگر (value >= INT16_MIN && value <= INT16_MAX)
    واپس font->em_scale_y ((int16_t) value);
  واپس font->em_scalef_y ((اعشاری) value);
}

ساکن غیرعلامتی صحیح
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   خالی *font_ڈیٹا HB_غیر_مستعمل,
				   غیرعلامتی صحیح گنتی,
				   ثابت hb_codepoint_t *پہلا_unicode,
				   غیرعلامتی صحیح unicode_stride,
				   hb_codepoint_t *پہلا_glyph,
				   غیرعلامتی صحیح glyph_stride,
				   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  برائے (غیرعلامتی i = 0; i < گنتی;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    غیرعلامتی n = hb_min (MAX_GLYPHS, گنتی - i);

    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      unicodes[j] = *پہلا_unicode;
      پہلا_unicode = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_unicode, unicode_stride);
    }

    اگر (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      واپس i;

    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      اگر (!gids[j])
        واپس i + j;
      *پہلا_glyph = gids[j];
      پہلا_glyph = &Struct_At_انحراف<hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }

    i += n;
  }

  واپس گنتی;
}

ساکن hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   خالی *font_ڈیٹا HB_غیر_مستعمل,
				   hb_font_extents_t *metrics,
				   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->سطر_gap = hb_directwrite_scale_y (font, dw_metrics.سطر_Gap);

  واپس سچ;
}

ساکن خالی
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     خالی* font_ڈیٹا HB_غیر_مستعمل,
				     غیرعلامتی گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph,
				     غیرعلامتی glyph_stride,
				     hb_position_t *پہلا_advance,
				     غیرعلامتی advance_stride,
				     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  IDWriteFontFace1 *dw_face1 = خالی_اشاریہ;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (خالی**)&dw_face1);
  أكد (dw_face1);

  غیرعلامتی صحیح عدد_glyphs = font->face->get_عدد_glyphs ();

  برائے (غیرعلامتی i = 0; i < گنتی;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    غیرعلامتی n = hb_min (MAX_GLYPHS, گنتی - i);

    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      gids[j] = *پہلا_glyph;
      advances[j] = 0;
      پہلا_glyph = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, جھوٹ);
    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      خودکار advance = gids[j] < عدد_glyphs ? advances[j] : 0;
      *پہلا_advance = hb_directwrite_scale_x (font, advance);
      پہلا_advance = &Struct_At_انحراف<hb_position_t> (پہلا_advance, advance_stride);
    }

    i += n;
  }
}

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL

ساکن خالی
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     خالی* font_ڈیٹا HB_غیر_مستعمل,
				     غیرعلامتی گنتی,
				     ثابت hb_codepoint_t *پہلا_glyph,
				     غیرعلامتی glyph_stride,
				     hb_position_t *پہلا_advance,
				     غیرعلامتی advance_stride,
				     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  IDWriteFontFace1 *dw_face1 = خالی_اشاریہ;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (خالی**)&dw_face1);
  أكد (dw_face1);

  برائے (غیرعلامتی i = 0; i < گنتی;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    غیرعلامتی n = hb_min (MAX_GLYPHS, گنتی - i);

    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      gids[j] = *پہلا_glyph;
      advances[j] = 0;
      پہلا_glyph = &Struct_At_انحراف<ثابت hb_codepoint_t> (پہلا_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, سچ);
    برائے (غیرعلامتی j = 0; j < n; j++)
    {
      *پہلا_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      پہلا_advance = &Struct_At_انحراف<hb_position_t> (پہلا_advance, advance_stride);
    }

    i += n;
  }
}

ساکن hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   خالی *font_ڈیٹا HB_غیر_مستعمل,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  اگر (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    واپس جھوٹ;

  *x = hb_directwrite_scale_x (font, metrics.advance_چوڑائی / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  واپس سچ;
}
#اگر_ختم

ساکن hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  خالی *font_ڈیٹا HB_غیر_مستعمل,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  اگر (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    واپس جھوٹ;

  int64_t دائیں = (int64_t) metrics.advance_چوڑائی - metrics.دایاں_Side_Bearing;
  int64_t چوٹی_2 = (int64_t) metrics.verticalOriginY - metrics.چوٹی_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_اونچائی + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.بایاں_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, چوٹی_2);
  extents->چوڑائی = hb_saturate_sub (hb_directwrite_scale_x (font, دائیں), extents->x_bearing);
  extents->اونچائی = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  واپس سچ;
}


#اگر_غیر_تعریف_شدہ HB_NO_DRAW

جماعت GeometrySink : عوامی IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

عوامی:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       خالی *draw_ڈیٹا)
    : font (font), drawing ({draw_funcs, draw_ڈیٹا}) {}

  HRESULT STDMETHODCALLTYPE Close() override { واپس S_OK; }
  خالی STDMETHODCALLTYPE سیٹ_Fill_منوال(D2_D1_FILL_منوال) override {}
  خالی STDMETHODCALLTYPE سیٹ_Segment_Flags(D2_D1_راستہ_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, خالی **) override { واپس E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, جوڑو_Ref)() override { واپس 1; }
  IFACEMETHOD_(ULONG, Release)() override { واپس 1; }

  خالی STDMETHODCALLTYPE BeginFigure(D2_D1_نقطہ_2_F آغاز_نقطہ, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_مقصد (font->em_scalef_x (آغاز_نقطہ.x), -(اعشاری) font->em_scalef_y (آغاز_نقطہ.y));
  }

  خالی STDMETHODCALLTYPE جوڑو_Beziers(ثابت D2D1_BEZIER_SEGMENT *beziers, UINT beziers_گنتی) override
  {
    برائے (غیرعلامتی i = 0; i < beziers_گنتی; ++i)
      drawing.cubic_مقصد (font->em_scalef_x (beziers[i].point1.x), -(اعشاری) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(اعشاری) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(اعشاری) font->em_scalef_y (beziers[i].point3.y));
  }

  خالی STDMETHODCALLTYPE جوڑو_سطریں(ثابت D2_D1_نقطہ_2_F *points, UINT points_گنتی) override
  {
    برائے (غیرعلامتی i = 0; i < points_گنتی; ++i)
      drawing.سطر_مقصد (font->em_scalef_x (points[i].x), -(اعشاری) font->em_scalef_y (points[i].y));
  }

  خالی STDMETHODCALLTYPE انتہا_Figure(D2_D1_FIGURE_انتہا) override
  {
    drawing.close_راستہ ();
  }
};

ساکن hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   خالی *font_ڈیٹا HB_غیر_مستعمل,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, خالی *draw_ڈیٹا,
				   خالی *user_ڈیٹا)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;

  GeometrySink sink (font, draw_funcs, draw_ڈیٹا);
  UINT16 gid = static_cast<UINT16>(glyph);
  غیرعلامتی upem = font->face->get_upem();

  واپس S_OK == dw_face->Get_Glyph_تسلسل_Outline (upem,
					      &gid, خالی_اشاریہ, خالی_اشاریہ,
					      1,
					      جھوٹ, جھوٹ,
					      &sink);
}

#اگر_ختم

ساکن اندرونی خالی free_static_directwrite_funcs ();

ساکن ساخت hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ساکن hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_سیٹ_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, خالی_اشاریہ, خالی_اشاریہ);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_سیٹ_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
    hb_font_funcs_سیٹ_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, خالی_اشاریہ, خالی_اشاریہ);
    hb_font_funcs_سیٹ_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_DRAW
    hb_font_funcs_سیٹ_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, خالی_اشاریہ, خالی_اشاریہ);
#اگر_ختم

    hb_font_funcs_سیٹ_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, خالی_اشاریہ, خالی_اشاریہ);

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_GLYPH_نام
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#اگر_ختم

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    واپس funcs;
  }
} static_directwrite_funcs;

ساکن اندرونی
خالی free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

ساکن hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  واپس static_directwrite_funcs.get_unconst ();
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
خالی
hb_directwrite_font_سیٹ_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ثابت خالی *) font->ڈیٹا.directwrite;
  اگر (unlikely (!dw_face))
  {
    hb_font_سیٹ_funcs (font,
		       hb_font_funcs_get_خالی_خانے (),
		       خالی_اشاریہ, خالی_اشاریہ);
    واپس;
  }

  dw_face->جوڑو_Ref ();
  hb_font_سیٹ_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     خالی_اشاریہ, خالی_اشاریہ);
}

#تعریف_منسوخ MAX_GLYPHS

#اگر_ختم
