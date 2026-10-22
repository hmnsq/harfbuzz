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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_DIRECTWRITE

#অন্তর্ভাব "hb-directwrite.h"

#অন্তর্ভাব <d2d1.h>

#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-font.hh"
#অন্তর্ভাব "hb-machinery.hh"

#define MAX_GLYPHS 256u

স্থির hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  যদি (value >= INT16_MIN && value <= INT16_MAX)
    প্রত্যাগম font->em_scale_x ((int16_t) value);
  প্রত্যাগম font->em_scalef_x ((ভিন্ন) value);
}

স্থির hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  যদি (value >= INT16_MIN && value <= INT16_MAX)
    প্রত্যাগম font->em_scale_y ((int16_t) value);
  প্রত্যাগম font->em_scalef_y ((ভিন্ন) value);
}

স্থির unsigned পূর্ণাঙ্ক
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   শূন্য *font_data HB_UNUSED,
				   unsigned পূর্ণাঙ্ক গুনতি,
				   ধ্রুব hb_codepoint_t *first_unicode,
				   unsigned পূর্ণাঙ্ক unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned পূর্ণাঙ্ক glyph_stride,
				   শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  প্রতি (unsigned i = 0; i < গুনতি;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, গুনতি - i);

    প্রতি (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_unicode, unicode_stride);
    }

    যদি (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      প্রত্যাগম i;

    প্রতি (unsigned j = 0; j < n; j++)
    {
      যদি (!gids[j])
        প্রত্যাগম i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  প্রত্যাগম গুনতি;
}

স্থির hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   শূন্য *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->সারি_gap = hb_directwrite_scale_y (font, dw_metrics.সারি_Gap);

  প্রত্যাগম সত্য;
}

স্থির শূন্য
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     শূন্য* font_data HB_UNUSED,
				     unsigned গুনতি,
				     ধ্রুব hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (শূন্য**)&dw_face1);
  প্রতিজ্ঞা (dw_face1);

  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs = font->face->get_অঙ্ক_glyphs ();

  প্রতি (unsigned i = 0; i < গুনতি;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, গুনতি - i);

    প্রতি (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, অসত্য);
    প্রতি (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < অঙ্ক_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

স্থির শূন্য
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     শূন্য* font_data HB_UNUSED,
				     unsigned গুনতি,
				     ধ্রুব hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (শূন্য**)&dw_face1);
  প্রতিজ্ঞা (dw_face1);

  প্রতি (unsigned i = 0; i < গুনতি;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, গুনতি - i);

    প্রতি (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ধ্রুব hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, সত্য);
    প্রতি (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

স্থির hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   শূন্য *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  যদি (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    প্রত্যাগম অসত্য;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  প্রত্যাগম সত্য;
}
#endif

স্থির hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  শূন্য *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  শূন্য *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  যদি (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    প্রত্যাগম অসত্য;

  int64_t ডান = (int64_t) metrics.advanceWidth - metrics.ডান_Side_Bearing;
  int64_t শীর্ষ_2 = (int64_t) metrics.verticalOriginY - metrics.শীর্ষ_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.বাম_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, শীর্ষ_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, ডান), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  প্রত্যাগম সত্য;
}


#ifndef HB_NO_DRAW

বর্গ GeometrySink : সার্বজনিক IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

সার্বজনিক:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       শূন্য *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { প্রত্যাগম S_OK; }
  শূন্য STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  শূন্য STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, শূন্য **) override { প্রত্যাগম E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { প্রত্যাগম 1; }
  IFACEMETHOD_(ULONG, Release)() override { প্রত্যাগম 1; }

  শূন্য STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(ভিন্ন) font->em_scalef_y (startPoint.y));
  }

  শূন্য STDMETHODCALLTYPE AddBeziers(ধ্রুব D2D1_BEZIER_SEGMENT *beziers, UINT beziers_গুনতি) override
  {
    প্রতি (unsigned i = 0; i < beziers_গুনতি; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(ভিন্ন) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(ভিন্ন) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(ভিন্ন) font->em_scalef_y (beziers[i].point3.y));
  }

  শূন্য STDMETHODCALLTYPE Add_সারিগুলি(ধ্রুব D2D1_POINT_2F *points, UINT points_গুনতি) override
  {
    প্রতি (unsigned i = 0; i < points_গুনতি; ++i)
      drawing.সারি_to (font->em_scalef_x (points[i].x), -(ভিন্ন) font->em_scalef_y (points[i].y));
  }

  শূন্য STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

স্থির hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   শূন্য *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, শূন্য *draw_data,
				   শূন্য *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  প্রত্যাগম S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      অসত্য, অসত্য,
					      &sink);
}

#endif

স্থির অন্তঃস্থ শূন্য free_static_directwrite_funcs ();

স্থির সংরচনা hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  স্থির hb_font_funcs_t *create ()
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

    প্রত্যাগম funcs;
  }
} static_directwrite_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

স্থির hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  প্রত্যাগম static_directwrite_funcs.get_unconst ();
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
শূন্য
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ধ্রুব শূন্য *) font->data.directwrite;
  যদি (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    প্রত্যাগম;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
