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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_DIRECTWRITE

#अन्तर्भाव "hb-directwrite.h"

#अन्तर्भाव <d2d1.h>

#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-machinery.hh"

#define MAX_GLYPHS 256u

स्थिर hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  यदि (value >= INT16_MIN && value <= INT16_MAX)
    प्रत्यागम font->em_scale_x ((int16_t) value);
  प्रत्यागम font->em_scalef_x ((भिन्न) value);
}

स्थिर hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  यदि (value >= INT16_MIN && value <= INT16_MAX)
    प्रत्यागम font->em_scale_y ((int16_t) value);
  प्रत्यागम font->em_scalef_y ((भिन्न) value);
}

स्थिर unsigned पूर्णाङ्क
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   शून्य *font_data HB_UNUSED,
				   unsigned पूर्णाङ्क मोजणी,
				   ध्रुव hb_codepoint_t *first_unicode,
				   unsigned पूर्णाङ्क unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned पूर्णाङ्क glyph_stride,
				   शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  प्रति (unsigned i = 0; i < मोजणी;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, मोजणी - i);

    प्रति (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<ध्रुव hb_codepoint_t> (first_unicode, unicode_stride);
    }

    यदि (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      प्रत्यागम i;

    प्रति (unsigned j = 0; j < n; j++)
    {
      यदि (!gids[j])
        प्रत्यागम i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  प्रत्यागम मोजणी;
}

स्थिर hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   शून्य *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->ओळ_gap = hb_directwrite_scale_y (font, dw_metrics.ओळ_Gap);

  प्रत्यागम सत्य;
}

स्थिर शून्य
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     शून्य* font_data HB_UNUSED,
				     unsigned मोजणी,
				     ध्रुव hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (शून्य**)&dw_face1);
  प्रतिज्ञा (dw_face1);

  unsigned पूर्णाङ्क अंक_glyphs = font->face->get_अंक_glyphs ();

  प्रति (unsigned i = 0; i < मोजणी;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, मोजणी - i);

    प्रति (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ध्रुव hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, असत्य);
    प्रति (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < अंक_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

स्थिर शून्य
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     शून्य* font_data HB_UNUSED,
				     unsigned मोजणी,
				     ध्रुव hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (शून्य**)&dw_face1);
  प्रतिज्ञा (dw_face1);

  प्रति (unsigned i = 0; i < मोजणी;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, मोजणी - i);

    प्रति (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<ध्रुव hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, सत्य);
    प्रति (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

स्थिर hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   शून्य *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  यदि (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    प्रत्यागम असत्य;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  प्रत्यागम सत्य;
}
#endif

स्थिर hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  शून्य *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  शून्य *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  यदि (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    प्रत्यागम असत्य;

  int64_t उजवा = (int64_t) metrics.advanceWidth - metrics.उजवा_Side_Bearing;
  int64_t शीर्ष_2 = (int64_t) metrics.verticalOriginY - metrics.शीर्ष_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.डावा_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, शीर्ष_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, उजवा), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  प्रत्यागम सत्य;
}


#ifndef HB_NO_DRAW

वर्ग GeometrySink : सार्वजनिक IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

सार्वजनिक:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       शून्य *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { प्रत्यागम S_OK; }
  शून्य STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  शून्य STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, शून्य **) override { प्रत्यागम E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { प्रत्यागम 1; }
  IFACEMETHOD_(ULONG, Release)() override { प्रत्यागम 1; }

  शून्य STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(भिन्न) font->em_scalef_y (startPoint.y));
  }

  शून्य STDMETHODCALLTYPE AddBeziers(ध्रुव D2D1_BEZIER_SEGMENT *beziers, UINT beziers_मोजणी) override
  {
    प्रति (unsigned i = 0; i < beziers_मोजणी; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(भिन्न) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(भिन्न) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(भिन्न) font->em_scalef_y (beziers[i].point3.y));
  }

  शून्य STDMETHODCALLTYPE Add_ओळी(ध्रुव D2D1_POINT_2F *points, UINT points_मोजणी) override
  {
    प्रति (unsigned i = 0; i < points_मोजणी; ++i)
      drawing.ओळ_to (font->em_scalef_x (points[i].x), -(भिन्न) font->em_scalef_y (points[i].y));
  }

  शून्य STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

स्थिर hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   शून्य *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, शून्य *draw_data,
				   शून्य *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  प्रत्यागम S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      असत्य, असत्य,
					      &sink);
}

#endif

स्थिर अन्तःस्थ शून्य free_static_directwrite_funcs ();

स्थिर संरचना hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  स्थिर hb_font_funcs_t *create ()
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

    प्रत्यागम funcs;
  }
} static_directwrite_funcs;

स्थिर अन्तःस्थ
शून्य free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

स्थिर hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  प्रत्यागम static_directwrite_funcs.get_unconst ();
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
शून्य
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (ध्रुव शून्य *) font->data.directwrite;
  यदि (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    प्रत्यागम;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
