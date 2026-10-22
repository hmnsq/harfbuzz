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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTWRITE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-directwrite.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <d2d1.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_GLYPHS 256u

ਸਥਿਰ hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  ਜੇ (value >= INT16_MIN && value <= INT16_MAX)
    ਵਾਪਸ font->em_scale_x ((int16_t) value);
  ਵਾਪਸ font->em_scalef_x ((ਦਸ਼ਮਲਵ) value);
}

ਸਥਿਰ hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  ਜੇ (value >= INT16_MIN && value <= INT16_MAX)
    ਵਾਪਸ font->em_scale_y ((int16_t) value);
  ਵਾਪਸ font->em_scalef_y ((ਦਸ਼ਮਲਵ) value);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2,
				   ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_unicode,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unicode_stride,
				   hb_codepoint_t *ਪਹਿਲਾ_glyph,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_stride,
				   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    ਬਿਨਾਨਿਸ਼ਾਨੀ n = hb_min (MAX_GLYPHS, ਗਿਣਤੀ_2 - i);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      unicodes[j] = *ਪਹਿਲਾ_unicode;
      ਪਹਿਲਾ_unicode = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_unicode, unicode_stride);
    }

    ਜੇ (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      ਵਾਪਸ i;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      ਜੇ (!gids[j])
        ਵਾਪਸ i + j;
      *ਪਹਿਲਾ_glyph = gids[j];
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }

    i += n;
  }

  ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਸਥਿਰ hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   hb_font_extents_t *metrics,
				   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->ਸਤਰ_gap = hb_directwrite_scale_y (font, dw_metrics.ਸਤਰ_Gap);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     ਖਾਲੀ* font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
				     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
				     hb_position_t *ਪਹਿਲਾ_advance,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  IDWriteFontFace1 *dw_face1 = ਖਾਲੀ_ਸੂਚਕ;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ਖਾਲੀ**)&dw_face1);
  ਪ੍ਰਤਿਜ੍ਞਾ (dw_face1);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = font->face->get_ਅੰਕ_glyphs ();

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    ਬਿਨਾਨਿਸ਼ਾਨੀ n = hb_min (MAX_GLYPHS, ਗਿਣਤੀ_2 - i);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      gids[j] = *ਪਹਿਲਾ_glyph;
      advances[j] = 0;
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ਝੂਠ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      ਸਵੈਚਾਲਿਤ advance = gids[j] < ਅੰਕ_glyphs ? advances[j] : 0;
      *ਪਹਿਲਾ_advance = hb_directwrite_scale_x (font, advance);
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    i += n;
  }
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL

ਸਥਿਰ ਖਾਲੀ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     ਖਾਲੀ* font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
				     ਅਚਲ hb_codepoint_t *ਪਹਿਲਾ_glyph,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_stride,
				     hb_position_t *ਪਹਿਲਾ_advance,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ advance_stride,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  IDWriteFontFace1 *dw_face1 = ਖਾਲੀ_ਸੂਚਕ;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ਖਾਲੀ**)&dw_face1);
  ਪ੍ਰਤਿਜ੍ਞਾ (dw_face1);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    ਬਿਨਾਨਿਸ਼ਾਨੀ n = hb_min (MAX_GLYPHS, ਗਿਣਤੀ_2 - i);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      gids[j] = *ਪਹਿਲਾ_glyph;
      advances[j] = 0;
      ਪਹਿਲਾ_glyph = &Struct_At_ਵਿੱਥ<ਅਚਲ hb_codepoint_t> (ਪਹਿਲਾ_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ਸੱਚ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < n; j++)
    {
      *ਪਹਿਲਾ_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      ਪਹਿਲਾ_advance = &Struct_At_ਵਿੱਥ<hb_position_t> (ਪਹਿਲਾ_advance, advance_stride);
    }

    i += n;
  }
}

ਸਥਿਰ hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ਜੇ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ਵਾਪਸ ਝੂਠ;

  *x = hb_directwrite_scale_x (font, metrics.advance_ਚੌੜਾਈ / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ਜੇ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ਵਾਪਸ ਝੂਠ;

  int64_t ਸੱਜੇ = (int64_t) metrics.advance_ਚੌੜਾਈ - metrics.ਸੱਜਾ_Side_Bearing;
  int64_t ਸਿਖਰ_2 = (int64_t) metrics.verticalOriginY - metrics.ਸਿਖਰ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_ਉਚਾਈ + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ਖੱਬਾ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, ਸਿਖਰ_2);
  extents->ਚੌੜਾਈ = hb_saturate_sub (hb_directwrite_scale_x (font, ਸੱਜੇ), extents->x_bearing);
  extents->ਉਚਾਈ = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  ਵਾਪਸ ਸੱਚ;
}


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW

ਵਰਗ GeometrySink : ਜਨਤਕ IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

ਜਨਤਕ:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       ਖਾਲੀ *draw_ਅੰਕੜੇ)
    : font (font), drawing ({draw_funcs, draw_ਅੰਕੜੇ}) {}

  HRESULT STDMETHODCALLTYPE Close() override { ਵਾਪਸ S_OK; }
  ਖਾਲੀ STDMETHODCALLTYPE ਸੈੱਟ_Fill_ਬਹੁਲਕ(D2_D1_FILL_ਬਹੁਲਕ) override {}
  ਖਾਲੀ STDMETHODCALLTYPE ਸੈੱਟ_Segment_Flags(D2_D1_ਰਸਤਾ_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, ਖਾਲੀ **) override { ਵਾਪਸ E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, ਜੋੜੋ_Ref)() override { ਵਾਪਸ 1; }
  IFACEMETHOD_(ULONG, Release)() override { ਵਾਪਸ 1; }

  ਖਾਲੀ STDMETHODCALLTYPE BeginFigure(D2_D1_ਬਿੰਦੂ_2_F ਸ਼ੁਰੂ_ਬਿੰਦੂ, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_ਮੰਜ਼ਿਲ (font->em_scalef_x (ਸ਼ੁਰੂ_ਬਿੰਦੂ.x), -(ਦਸ਼ਮਲਵ) font->em_scalef_y (ਸ਼ੁਰੂ_ਬਿੰਦੂ.y));
  }

  ਖਾਲੀ STDMETHODCALLTYPE ਜੋੜੋ_Beziers(ਅਚਲ D2D1_BEZIER_SEGMENT *beziers, UINT beziers_ਗਿਣਤੀ) override
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < beziers_ਗਿਣਤੀ; ++i)
      drawing.cubic_ਮੰਜ਼ਿਲ (font->em_scalef_x (beziers[i].point1.x), -(ਦਸ਼ਮਲਵ) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(ਦਸ਼ਮਲਵ) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(ਦਸ਼ਮਲਵ) font->em_scalef_y (beziers[i].point3.y));
  }

  ਖਾਲੀ STDMETHODCALLTYPE ਜੋੜੋ_ਸਤਰਾਂ(ਅਚਲ D2_D1_ਬਿੰਦੂ_2_F *points, UINT points_ਗਿਣਤੀ) override
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < points_ਗਿਣਤੀ; ++i)
      drawing.ਸਤਰ_ਮੰਜ਼ਿਲ (font->em_scalef_x (points[i].x), -(ਦਸ਼ਮਲਵ) font->em_scalef_y (points[i].y));
  }

  ਖਾਲੀ STDMETHODCALLTYPE ਸਿਰਾ_Figure(D2_D1_FIGURE_ਸਿਰਾ) override
  {
    drawing.close_ਰਸਤਾ ();
  }
};

ਸਥਿਰ hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   ਖਾਲੀ *font_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ,
				   ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;

  GeometrySink sink (font, draw_funcs, draw_ਅੰਕੜੇ);
  UINT16 gid = static_cast<UINT16>(glyph);
  ਬਿਨਾਨਿਸ਼ਾਨੀ upem = font->face->get_upem();

  ਵਾਪਸ S_OK == dw_face->Get_Glyph_ਲੜੀਵਾਰ_Outline (upem,
					      &gid, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ,
					      1,
					      ਝੂਠ, ਝੂਠ,
					      &sink);
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_directwrite_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_ਸੈੱਟ_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_ਸੈੱਟ_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
    hb_font_funcs_ਸੈੱਟ_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_font_funcs_ਸੈੱਟ_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_DRAW
    hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
#ਜੇ_ਅੰਤ

    hb_font_funcs_ਸੈੱਟ_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_GLYPH_ਨਾਂ
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#ਜੇ_ਅੰਤ

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    ਵਾਪਸ funcs;
  }
} static_directwrite_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

ਸਥਿਰ hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  ਵਾਪਸ static_directwrite_funcs.get_unconst ();
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
ਖਾਲੀ
hb_directwrite_font_ਸੈੱਟ_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.directwrite;
  ਜੇ (unlikely (!dw_face))
  {
    hb_font_ਸੈੱਟ_funcs (font,
		       hb_font_funcs_get_ਖਾਲੀ_ਥਾਵਾਂ (),
		       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    ਵਾਪਸ;
  }

  dw_face->ਜੋੜੋ_Ref ();
  hb_font_ਸੈੱਟ_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MAX_GLYPHS

#ਜੇ_ਅੰਤ
