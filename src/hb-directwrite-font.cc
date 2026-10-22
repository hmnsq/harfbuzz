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

#समावेश "hb.hh"

#अगर_घोषित HAVE_DIRECTWRITE

#समावेश "hb-directwrite.h"

#समावेश <d2d1.h>

#समावेश "hb-draw.hh"
#समावेश "hb-font.hh"
#समावेश "hb-machinery.hh"

#घोषणा MAX_GLYPHS 256u

अटल hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  अगर (value >= INT16_MIN && value <= INT16_MAX)
    वापस font->em_scale_x ((int16_t) value);
  वापस font->em_scalef_x ((भग्न) value);
}

अटल hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  अगर (value >= INT16_MIN && value <= INT16_MAX)
    वापस font->em_scale_y ((int16_t) value);
  वापस font->em_scalef_y ((भग्न) value);
}

अटल अचिन्हित पूर्णांक
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   व्योम *font_तथ्य HB_UNUSED,
				   अचिन्हित पूर्णांक गिनती,
				   स्थिर hb_codepoint_t *first_unicode,
				   अचिन्हित पूर्णांक unicode_stride,
				   hb_codepoint_t *first_glyph,
				   अचिन्हित पूर्णांक glyph_stride,
				   व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  क्रम (अचिन्हित i = 0; i < गिनती;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    अचिन्हित n = hb_min (MAX_GLYPHS, गिनती - i);

    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<स्थिर hb_codepoint_t> (first_unicode, unicode_stride);
    }

    अगर (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      वापस i;

    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      अगर (!gids[j])
        वापस i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  वापस गिनती;
}

अटल hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   व्योम *font_तथ्य HB_UNUSED,
				   hb_font_extents_t *metrics,
				   व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->पंक्ति_gap = hb_directwrite_scale_y (font, dw_metrics.पंक्ति_Gap);

  वापस true;
}

अटल व्योम
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     व्योम* font_तथ्य HB_UNUSED,
				     अचिन्हित गिनती,
				     स्थिर hb_codepoint_t *first_glyph,
				     अचिन्हित glyph_stride,
				     hb_position_t *first_advance,
				     अचिन्हित advance_stride,
				     व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (व्योम**)&dw_face1);
  निश्चित (dw_face1);

  अचिन्हित पूर्णांक अंक_glyphs = font->face->get_अंक_glyphs ();

  क्रम (अचिन्हित i = 0; i < गिनती;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    अचिन्हित n = hb_min (MAX_GLYPHS, गिनती - i);

    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<स्थिर hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, false);
    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      स्वतः advance = gids[j] < अंक_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#अगर_अघोषित HB_NO_VERTICAL

अटल व्योम
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     व्योम* font_तथ्य HB_UNUSED,
				     अचिन्हित गिनती,
				     स्थिर hb_codepoint_t *first_glyph,
				     अचिन्हित glyph_stride,
				     hb_position_t *first_advance,
				     अचिन्हित advance_stride,
				     व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (व्योम**)&dw_face1);
  निश्चित (dw_face1);

  क्रम (अचिन्हित i = 0; i < गिनती;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    अचिन्हित n = hb_min (MAX_GLYPHS, गिनती - i);

    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<स्थिर hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, true);
    क्रम (अचिन्हित j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

अटल hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   व्योम *font_तथ्य HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  अगर (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    वापस false;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  वापस true;
}
#पूर्ण_अगर

अटल hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  व्योम *font_तथ्य HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  व्योम *user_तथ्य HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  अगर (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    वापस false;

  int64_t दाँए = (int64_t) metrics.advanceWidth - metrics.दायाँ_Side_Bearing;
  int64_t शीर्ष_4 = (int64_t) metrics.verticalOriginY - metrics.शीर्ष_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.बायाँ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, शीर्ष_4);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, दाँए), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  वापस true;
}


#अगर_अघोषित HB_NO_DRAW

श्रेणी GeometrySink : खुला IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

खुला:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       व्योम *draw_तथ्य)
    : font (font), drawing ({draw_funcs, draw_तथ्य}) {}

  HRESULT STDMETHODCALLTYPE Close() override { वापस S_OK; }
  व्योम STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  व्योम STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, व्योम **) override { वापस E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { वापस 1; }
  IFACEMETHOD_(ULONG, Release)() override { वापस 1; }

  व्योम STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(भग्न) font->em_scalef_y (startPoint.y));
  }

  व्योम STDMETHODCALLTYPE AddBeziers(स्थिर D2D1_BEZIER_SEGMENT *beziers, UINT beziers_गिनती) override
  {
    क्रम (अचिन्हित i = 0; i < beziers_गिनती; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(भग्न) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(भग्न) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(भग्न) font->em_scalef_y (beziers[i].point3.y));
  }

  व्योम STDMETHODCALLTYPE Add_पंक्तियाँ(स्थिर D2D1_POINT_2F *points, UINT points_गिनती) override
  {
    क्रम (अचिन्हित i = 0; i < points_गिनती; ++i)
      drawing.पंक्ति_to (font->em_scalef_x (points[i].x), -(भग्न) font->em_scalef_y (points[i].y));
  }

  व्योम STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

अटल hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   व्योम *font_तथ्य HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, व्योम *draw_तथ्य,
				   व्योम *user_तथ्य)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;

  GeometrySink sink (font, draw_funcs, draw_तथ्य);
  UINT16 gid = static_cast<UINT16>(glyph);
  अचिन्हित upem = font->face->get_upem();

  वापस S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      false, false,
					      &sink);
}

#पूर्ण_अगर

अटल अंतरभूत व्योम free_static_directwrite_funcs ();

अटल काष्ठा hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  अटल hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_set_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, nullptr, nullptr);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_set_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, nullptr, nullptr);
    hb_font_funcs_set_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, nullptr, nullptr);

#अगर_अघोषित HB_NO_VERTICAL
    hb_font_funcs_set_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, nullptr, nullptr);
    hb_font_funcs_set_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, nullptr, nullptr);
#पूर्ण_अगर

#अगर_अघोषित HB_NO_DRAW
    hb_font_funcs_set_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_set_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, nullptr, nullptr);

#अगर_अघोषित HB_NO_OT_FONT_GLYPH_NAMES
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#पूर्ण_अगर

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    वापस funcs;
  }
} static_directwrite_funcs;

अटल अंतरभूत
व्योम free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

अटल hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  वापस static_directwrite_funcs.get_unconst ();
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
व्योम
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (स्थिर व्योम *) font->तथ्य.directwrite;
  अगर (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    वापस;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#अघोषित MAX_GLYPHS

#पूर्ण_अगर
