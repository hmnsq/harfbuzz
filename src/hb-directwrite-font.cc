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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#അന്തര്ഭാവ "hb-directwrite.h"

#അന്തര്ഭാവ <d2d1.h>

#അന്തര്ഭാവ "hb-draw.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-machinery.hh"

#define MAX_GLYPHS 256u

സ്ഥിതം hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  എങ്കിൽ (value >= INT16_MIN && value <= INT16_MAX)
    തിരികെ font->em_scale_x ((int16_t) value);
  തിരികെ font->em_scalef_x ((ദശാംശം) value);
}

സ്ഥിതം hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  എങ്കിൽ (value >= INT16_MIN && value <= INT16_MAX)
    തിരികെ font->em_scale_y ((int16_t) value);
  തിരികെ font->em_scalef_y ((ദശാംശം) value);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   ശൂന്യം *font_data HB_UNUSED,
				   unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				   സ്ഥിരം hb_codepoint_t *first_unicode,
				   unsigned പൂർണ്ണസംഖ്യ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned പൂർണ്ണസംഖ്യ glyph_stride,
				   ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  വേണ്ടി (unsigned i = 0; i < എണ്ണം;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, എണ്ണം - i);

    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_unicode, unicode_stride);
    }

    എങ്കിൽ (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      തിരികെ i;

    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      എങ്കിൽ (!gids[j])
        തിരികെ i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  തിരികെ എണ്ണം;
}

സ്ഥിതം hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   ശൂന്യം *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->വരി_gap = hb_directwrite_scale_y (font, dw_metrics.വരി_Gap);

  തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     ശൂന്യം* font_data HB_UNUSED,
				     unsigned എണ്ണം,
				     സ്ഥിരം hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ശൂന്യം**)&dw_face1);
  പ്രതിജ്ഞാ (dw_face1);

  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = font->face->get_അക്കം_glyphs ();

  വേണ്ടി (unsigned i = 0; i < എണ്ണം;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, എണ്ണം - i);

    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, അസത്യം);
    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < അക്കം_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

സ്ഥിതം ശൂന്യം
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     ശൂന്യം* font_data HB_UNUSED,
				     unsigned എണ്ണം,
				     സ്ഥിരം hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ശൂന്യം**)&dw_face1);
  പ്രതിജ്ഞാ (dw_face1);

  വേണ്ടി (unsigned i = 0; i < എണ്ണം;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, എണ്ണം - i);

    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<സ്ഥിരം hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, സത്യം);
    വേണ്ടി (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

സ്ഥിതം hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   ശൂന്യം *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  എങ്കിൽ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    തിരികെ അസത്യം;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  തിരികെ സത്യം;
}
#endif

സ്ഥിതം hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  ശൂന്യം *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  ശൂന്യം *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  എങ്കിൽ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    തിരികെ അസത്യം;

  int64_t വലത് = (int64_t) metrics.advanceWidth - metrics.വലത്_Side_Bearing;
  int64_t മുകൾ_2 = (int64_t) metrics.verticalOriginY - metrics.മുകൾ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ഇടത്_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, മുകൾ_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, വലത്), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  തിരികെ സത്യം;
}


#ifndef HB_NO_DRAW

വർഗ്ഗം GeometrySink : പൊതു IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

പൊതു:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       ശൂന്യം *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { തിരികെ S_OK; }
  ശൂന്യം STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  ശൂന്യം STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, ശൂന്യം **) override { തിരികെ E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { തിരികെ 1; }
  IFACEMETHOD_(ULONG, Release)() override { തിരികെ 1; }

  ശൂന്യം STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(ദശാംശം) font->em_scalef_y (startPoint.y));
  }

  ശൂന്യം STDMETHODCALLTYPE AddBeziers(സ്ഥിരം D2D1_BEZIER_SEGMENT *beziers, UINT beziers_എണ്ണം) override
  {
    വേണ്ടി (unsigned i = 0; i < beziers_എണ്ണം; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(ദശാംശം) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(ദശാംശം) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(ദശാംശം) font->em_scalef_y (beziers[i].point3.y));
  }

  ശൂന്യം STDMETHODCALLTYPE Add_വരികൾ(സ്ഥിരം D2D1_POINT_2F *points, UINT points_എണ്ണം) override
  {
    വേണ്ടി (unsigned i = 0; i < points_എണ്ണം; ++i)
      drawing.വരി_to (font->em_scalef_x (points[i].x), -(ദശാംശം) font->em_scalef_y (points[i].y));
  }

  ശൂന്യം STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

സ്ഥിതം hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   ശൂന്യം *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, ശൂന്യം *draw_data,
				   ശൂന്യം *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  തിരികെ S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      അസത്യം, അസത്യം,
					      &sink);
}

#endif

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_directwrite_funcs ();

സ്ഥിതം ഘടന hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, nullptr, nullptr);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, nullptr, nullptr);

#ifndef HB_NO_VERTICAL
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, nullptr, nullptr);
#endif

#ifndef HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, nullptr, nullptr);
#endif

    hb_font_funcs_set_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, nullptr, nullptr);

#ifndef HB_NO_OT_FONT_GLYPH_NAMES
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#endif

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    തിരികെ funcs;
  }
} static_directwrite_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

സ്ഥിതം hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  തിരികെ static_directwrite_funcs.get_unconst ();
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
ശൂന്യം
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (സ്ഥിരം ശൂന്യം *) font->data.directwrite;
  എങ്കിൽ (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    തിരികെ;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
