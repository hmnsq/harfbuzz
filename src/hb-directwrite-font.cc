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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ಅನ್ತರ್ಭಾವ "hb-directwrite.h"

#ಅನ್ತರ್ಭಾವ <d2d1.h>

#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

#define MAX_GLYPHS 256u

ಸ್ಥಿರ hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  ಯದಿ (value >= INT16_MIN && value <= INT16_MAX)
    ಪ್ರತ್ಯಾಗಮ font->em_scale_x ((int16_t) value);
  ಪ್ರತ್ಯಾಗಮ font->em_scalef_x ((ಭಿನ್ನ) value);
}

ಸ್ಥಿರ hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  ಯದಿ (value >= INT16_MIN && value <= INT16_MAX)
    ಪ್ರತ್ಯಾಗಮ font->em_scale_y ((int16_t) value);
  ಪ್ರತ್ಯಾಗಮ font->em_scalef_y ((ಭಿನ್ನ) value);
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   ಶೂನ್ಯ *font_data HB_UNUSED,
				   unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				   ಧ್ರುವ hb_codepoint_t *first_unicode,
				   unsigned ಪೂರ್ಣಾಙ್ಕ unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ಪೂರ್ಣಾಙ್ಕ glyph_stride,
				   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ಎಣಿಕೆ - i);

    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_unicode, unicode_stride);
    }

    ಯದಿ (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      ಪ್ರತ್ಯಾಗಮ i;

    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      ಯದಿ (!gids[j])
        ಪ್ರತ್ಯಾಗಮ i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸ್ಥಿರ hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   ಶೂನ್ಯ *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->ಸಾಲು_gap = hb_directwrite_scale_y (font, dw_metrics.ಸಾಲು_Gap);

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     ಶೂನ್ಯ* font_data HB_UNUSED,
				     unsigned ಎಣಿಕೆ,
				     ಧ್ರುವ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ಶೂನ್ಯ**)&dw_face1);
  ಪ್ರತಿಜ್ಞಾ (dw_face1);

  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs = font->face->get_ಅಂಕೆ_glyphs ();

  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ಎಣಿಕೆ - i);

    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ಅಸತ್ಯ);
    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < ಅಂಕೆ_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

ಸ್ಥಿರ ಶೂನ್ಯ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     ಶೂನ್ಯ* font_data HB_UNUSED,
				     unsigned ಎಣಿಕೆ,
				     ಧ್ರುವ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ಶೂನ್ಯ**)&dw_face1);
  ಪ್ರತಿಜ್ಞಾ (dw_face1);

  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ಎಣಿಕೆ - i);

    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ಧ್ರುವ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ಸತ್ಯ);
    ಪ್ರತಿ (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

ಸ್ಥಿರ hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   ಶೂನ್ಯ *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ಯದಿ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಸ್ಥಿರ hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  ಶೂನ್ಯ *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ಯದಿ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  int64_t ಬಲ = (int64_t) metrics.advanceWidth - metrics.ಬಲ_Side_Bearing;
  int64_t ತುದಿ_2 = (int64_t) metrics.verticalOriginY - metrics.ತುದಿ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ಎಡ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, ತುದಿ_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, ಬಲ), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


#ifndef HB_NO_DRAW

ವರ್ಗ GeometrySink : ಸಾರ್ವಜನಿಕ IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

ಸಾರ್ವಜನಿಕ:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       ಶೂನ್ಯ *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { ಪ್ರತ್ಯಾಗಮ S_OK; }
  ಶೂನ್ಯ STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  ಶೂನ್ಯ STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, ಶೂನ್ಯ **) override { ಪ್ರತ್ಯಾಗಮ E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { ಪ್ರತ್ಯಾಗಮ 1; }
  IFACEMETHOD_(ULONG, Release)() override { ಪ್ರತ್ಯಾಗಮ 1; }

  ಶೂನ್ಯ STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(ಭಿನ್ನ) font->em_scalef_y (startPoint.y));
  }

  ಶೂನ್ಯ STDMETHODCALLTYPE AddBeziers(ಧ್ರುವ D2D1_BEZIER_SEGMENT *beziers, UINT beziers_ಎಣಿಕೆ) override
  {
    ಪ್ರತಿ (unsigned i = 0; i < beziers_ಎಣಿಕೆ; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(ಭಿನ್ನ) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(ಭಿನ್ನ) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(ಭಿನ್ನ) font->em_scalef_y (beziers[i].point3.y));
  }

  ಶೂನ್ಯ STDMETHODCALLTYPE Add_ಸಾಲುಗಳು(ಧ್ರುವ D2D1_POINT_2F *points, UINT points_ಎಣಿಕೆ) override
  {
    ಪ್ರತಿ (unsigned i = 0; i < points_ಎಣಿಕೆ; ++i)
      drawing.ಸಾಲು_to (font->em_scalef_x (points[i].x), -(ಭಿನ್ನ) font->em_scalef_y (points[i].y));
  }

  ಶೂನ್ಯ STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

ಸ್ಥಿರ hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   ಶೂನ್ಯ *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, ಶೂನ್ಯ *draw_data,
				   ಶೂನ್ಯ *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  ಪ್ರತ್ಯಾಗಮ S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      ಅಸತ್ಯ, ಅಸತ್ಯ,
					      &sink);
}

#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_directwrite_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_font_funcs_t *create ()
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

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_directwrite_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

ಸ್ಥಿರ hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_directwrite_funcs.get_unconst ();
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
ಶೂನ್ಯ
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ಧ್ರುವ ಶೂನ್ಯ *) font->data.directwrite;
  ಯದಿ (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    ಪ್ರತ್ಯಾಗಮ;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
