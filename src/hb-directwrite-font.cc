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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#અન્તર્ભાવ "hb-directwrite.h"

#અન્તર્ભાવ <d2d1.h>

#અન્તર્ભાવ "hb-draw.hh"
#અન્તર્ભાવ "hb-font.hh"
#અન્તર્ભાવ "hb-machinery.hh"

#define MAX_GLYPHS 256u

સ્થિર hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  જો (value >= INT16_MIN && value <= INT16_MAX)
    પાછા font->em_scale_x ((int16_t) value);
  પાછા font->em_scalef_x ((દશાંશ) value);
}

સ્થિર hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  જો (value >= INT16_MIN && value <= INT16_MAX)
    પાછા font->em_scale_y ((int16_t) value);
  પાછા font->em_scalef_y ((દશાંશ) value);
}

સ્થિર unsigned પૂર્ણાંક
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   શૂન્ય *font_data HB_UNUSED,
				   unsigned પૂર્ણાંક ગણતરી,
				   અચલ hb_codepoint_t *first_unicode,
				   unsigned પૂર્ણાંક unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned પૂર્ણાંક glyph_stride,
				   શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  માટે (unsigned i = 0; i < ગણતરી;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ગણતરી - i);

    માટે (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<અચલ hb_codepoint_t> (first_unicode, unicode_stride);
    }

    જો (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      પાછા i;

    માટે (unsigned j = 0; j < n; j++)
    {
      જો (!gids[j])
        પાછા i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  પાછા ગણતરી;
}

સ્થિર hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   શૂન્ય *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->લીટી_gap = hb_directwrite_scale_y (font, dw_metrics.લીટી_Gap);

  પાછા સાચું;
}

સ્થિર શૂન્ય
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     શૂન્ય* font_data HB_UNUSED,
				     unsigned ગણતરી,
				     અચલ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (શૂન્ય**)&dw_face1);
  પ્રતિજ્ઞા (dw_face1);

  unsigned પૂર્ણાંક અંક_glyphs = font->face->get_અંક_glyphs ();

  માટે (unsigned i = 0; i < ગણતરી;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ગણતરી - i);

    માટે (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<અચલ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ખોટું);
    માટે (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < અંક_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

સ્થિર શૂન્ય
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     શૂન્ય* font_data HB_UNUSED,
				     unsigned ગણતરી,
				     અચલ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (શૂન્ય**)&dw_face1);
  પ્રતિજ્ઞા (dw_face1);

  માટે (unsigned i = 0; i < ગણતરી;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ગણતરી - i);

    માટે (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<અચલ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, સાચું);
    માટે (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

સ્થિર hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   શૂન્ય *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  જો (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    પાછા ખોટું;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  પાછા સાચું;
}
#endif

સ્થિર hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  શૂન્ય *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  શૂન્ય *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  જો (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    પાછા ખોટું;

  int64_t જમણું = (int64_t) metrics.advanceWidth - metrics.જમણું_Side_Bearing;
  int64_t ટોચ_2 = (int64_t) metrics.verticalOriginY - metrics.ટોચ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ડાબું_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, ટોચ_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, જમણું), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  પાછા સાચું;
}


#ifndef HB_NO_DRAW

વર્ગ GeometrySink : જાહેર IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

જાહેર:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       શૂન્ય *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { પાછા S_OK; }
  શૂન્ય STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  શૂન્ય STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, શૂન્ય **) override { પાછા E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { પાછા 1; }
  IFACEMETHOD_(ULONG, Release)() override { પાછા 1; }

  શૂન્ય STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(દશાંશ) font->em_scalef_y (startPoint.y));
  }

  શૂન્ય STDMETHODCALLTYPE AddBeziers(અચલ D2D1_BEZIER_SEGMENT *beziers, UINT beziers_ગણતરી) override
  {
    માટે (unsigned i = 0; i < beziers_ગણતરી; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(દશાંશ) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(દશાંશ) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(દશાંશ) font->em_scalef_y (beziers[i].point3.y));
  }

  શૂન્ય STDMETHODCALLTYPE Add_લીટીઓ(અચલ D2D1_POINT_2F *points, UINT points_ગણતરી) override
  {
    માટે (unsigned i = 0; i < points_ગણતરી; ++i)
      drawing.લીટી_to (font->em_scalef_x (points[i].x), -(દશાંશ) font->em_scalef_y (points[i].y));
  }

  શૂન્ય STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

સ્થિર hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   શૂન્ય *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, શૂન્ય *draw_data,
				   શૂન્ય *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  પાછા S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      ખોટું, ખોટું,
					      &sink);
}

#endif

સ્થિર અંતઃરેખ શૂન્ય free_static_directwrite_funcs ();

સ્થિર સંરચના hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  સ્થિર hb_font_funcs_t *create ()
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

    પાછા funcs;
  }
} static_directwrite_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

સ્થિર hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  પાછા static_directwrite_funcs.get_unconst ();
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
શૂન્ય
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (અચલ શૂન્ય *) font->data.directwrite;
  જો (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    પાછા;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
