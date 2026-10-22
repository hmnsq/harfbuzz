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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#ଅନ୍ତର୍ଭାଵ "hb-directwrite.h"

#ଅନ୍ତର୍ଭାଵ <d2d1.h>

#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

#define MAX_GLYPHS 256u

ସ୍ଥିର hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  ଯଦି (value >= INT16_MIN && value <= INT16_MAX)
    ଫେରନ୍ତୁ font->em_scale_x ((int16_t) value);
  ଫେରନ୍ତୁ font->em_scalef_x ((ଦଶମିକ) value);
}

ସ୍ଥିର hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  ଯଦି (value >= INT16_MIN && value <= INT16_MAX)
    ଫେରନ୍ତୁ font->em_scale_y ((int16_t) value);
  ଫେରନ୍ତୁ font->em_scalef_y ((ଦଶମିକ) value);
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   ଶୂନ୍ୟ *font_data HB_UNUSED,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				   ଅଚଳ hb_codepoint_t *first_unicode,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_stride,
				   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ଗଣନା_2 - i);

    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_unicode, unicode_stride);
    }

    ଯଦି (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      ଫେରନ୍ତୁ i;

    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      ଯଦି (!gids[j])
        ଫେରନ୍ତୁ i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  ଫେରନ୍ତୁ ଗଣନା_2;
}

ସ୍ଥିର hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   ଶୂନ୍ୟ *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->ଧାଡ଼ି_gap = hb_directwrite_scale_y (font, dw_metrics.ଧାଡ଼ି_Gap);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     ଶୂନ୍ୟ* font_data HB_UNUSED,
				     unsigned ଗଣନା_2,
				     ଅଚଳ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ଶୂନ୍ୟ**)&dw_face1);
  ପ୍ରତିଜ୍ଞା (dw_face1);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs = font->face->get_ଅଙ୍କ_glyphs ();

  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ଗଣନା_2 - i);

    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ମିଥ୍ୟା);
    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < ଅଙ୍କ_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

ସ୍ଥିର ଶୂନ୍ୟ
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     ଶୂନ୍ୟ* font_data HB_UNUSED,
				     unsigned ଗଣନା_2,
				     ଅଚଳ hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (ଶୂନ୍ୟ**)&dw_face1);
  ପ୍ରତିଜ୍ଞା (dw_face1);

  ପାଇଁ (unsigned i = 0; i < ଗଣନା_2;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, ଗଣନା_2 - i);

    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ଅଚଳ hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, ସତ୍ୟ);
    ପାଇଁ (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

ସ୍ଥିର hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   ଶୂନ୍ୟ *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ଯଦି (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ସ୍ଥିର hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  ଶୂନ୍ୟ *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ଯଦି (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  int64_t ଡାହାଣ = (int64_t) metrics.advanceWidth - metrics.ଡାହାଣ_Side_Bearing;
  int64_t ଶୀର୍ଷ_2 = (int64_t) metrics.verticalOriginY - metrics.ଶୀର୍ଷ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ବାମ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, ଶୀର୍ଷ_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, ଡାହାଣ), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  ଫେରନ୍ତୁ ସତ୍ୟ;
}


#ifndef HB_NO_DRAW

ଶ୍ରେଣୀ GeometrySink : ସର୍ବସାଧାରଣ IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

ସର୍ବସାଧାରଣ:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       ଶୂନ୍ୟ *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { ଫେରନ୍ତୁ S_OK; }
  ଶୂନ୍ୟ STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  ଶୂନ୍ୟ STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, ଶୂନ୍ୟ **) override { ଫେରନ୍ତୁ E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { ଫେରନ୍ତୁ 1; }
  IFACEMETHOD_(ULONG, Release)() override { ଫେରନ୍ତୁ 1; }

  ଶୂନ୍ୟ STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(ଦଶମିକ) font->em_scalef_y (startPoint.y));
  }

  ଶୂନ୍ୟ STDMETHODCALLTYPE AddBeziers(ଅଚଳ D2D1_BEZIER_SEGMENT *beziers, UINT beziers_ଗଣନା) override
  {
    ପାଇଁ (unsigned i = 0; i < beziers_ଗଣନା; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(ଦଶମିକ) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(ଦଶମିକ) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(ଦଶମିକ) font->em_scalef_y (beziers[i].point3.y));
  }

  ଶୂନ୍ୟ STDMETHODCALLTYPE Add_ଧାଡ଼ିଗୁଡ଼ିକ(ଅଚଳ D2D1_POINT_2F *points, UINT points_ଗଣନା) override
  {
    ପାଇଁ (unsigned i = 0; i < points_ଗଣନା; ++i)
      drawing.ଧାଡ଼ି_to (font->em_scalef_x (points[i].x), -(ଦଶମିକ) font->em_scalef_y (points[i].y));
  }

  ଶୂନ୍ୟ STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

ସ୍ଥିର hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   ଶୂନ୍ୟ *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, ଶୂନ୍ୟ *draw_data,
				   ଶୂନ୍ୟ *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  ଫେରନ୍ତୁ S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      ମିଥ୍ୟା, ମିଥ୍ୟା,
					      &sink);
}

#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_directwrite_funcs ();

ସ୍ଥିର ସଂରଚନା hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_font_funcs_t *create ()
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

    ଫେରନ୍ତୁ funcs;
  }
} static_directwrite_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

ସ୍ଥିର hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  ଫେରନ୍ତୁ static_directwrite_funcs.get_unconst ();
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
ଶୂନ୍ୟ
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ଅଚଳ ଶୂନ୍ୟ *) font->data.directwrite;
  ଯଦି (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    ଫେରନ୍ତୁ;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
