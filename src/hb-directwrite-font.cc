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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_DIRECTWRITE

#அந்தர்பாவ "hb-directwrite.h"

#அந்தர்பாவ <d2d1.h>

#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-font.hh"
#அந்தர்பாவ "hb-machinery.hh"

#define MAX_GLYPHS 256u

நிலையான hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  எனில் (value >= INT16_MIN && value <= INT16_MAX)
    திரும்பு font->em_scale_x ((int16_t) value);
  திரும்பு font->em_scalef_x ((தசம) value);
}

நிலையான hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  எனில் (value >= INT16_MIN && value <= INT16_MAX)
    திரும்பு font->em_scale_y ((int16_t) value);
  திரும்பு font->em_scalef_y ((தசம) value);
}

நிலையான unsigned முழு
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   வெறுமை *font_data HB_UNUSED,
				   unsigned முழு மொத்தம்,
				   மாறிலி hb_codepoint_t *first_unicode,
				   unsigned முழு unicode_stride,
				   hb_codepoint_t *first_glyph,
				   unsigned முழு glyph_stride,
				   வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  க்கு (unsigned i = 0; i < மொத்தம்;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, மொத்தம் - i);

    க்கு (unsigned j = 0; j < n; j++)
    {
      unicodes[j] = *first_unicode;
      first_unicode = &StructAtOffset<மாறிலி hb_codepoint_t> (first_unicode, unicode_stride);
    }

    எனில் (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      திரும்பு i;

    க்கு (unsigned j = 0; j < n; j++)
    {
      எனில் (!gids[j])
        திரும்பு i + j;
      *first_glyph = gids[j];
      first_glyph = &StructAtOffset<hb_codepoint_t> (first_glyph, glyph_stride);
    }

    i += n;
  }

  திரும்பு மொத்தம்;
}

நிலையான hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   வெறுமை *font_data HB_UNUSED,
				   hb_font_extents_t *metrics,
				   வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->வரி_gap = hb_directwrite_scale_y (font, dw_metrics.வரி_Gap);

  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     வெறுமை* font_data HB_UNUSED,
				     unsigned மொத்தம்,
				     மாறிலி hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (வெறுமை**)&dw_face1);
  ப்ரதிஜ்ஞா (dw_face1);

  unsigned முழு இலக்கம்_glyphs = font->face->get_இலக்கம்_glyphs ();

  க்கு (unsigned i = 0; i < மொத்தம்;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, மொத்தம் - i);

    க்கு (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<மாறிலி hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, பொய்);
    க்கு (unsigned j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      auto advance = gids[j] < இலக்கம்_glyphs ? advances[j] : 0;
      *first_advance = hb_directwrite_scale_x (font, advance);
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

#ifndef HB_NO_VERTICAL

நிலையான வெறுமை
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     வெறுமை* font_data HB_UNUSED,
				     unsigned மொத்தம்,
				     மாறிலி hb_codepoint_t *first_glyph,
				     unsigned glyph_stride,
				     hb_position_t *first_advance,
				     unsigned advance_stride,
				     வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  IDWriteFontFace1 *dw_face1 = nullptr;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (வெறுமை**)&dw_face1);
  ப்ரதிஜ்ஞா (dw_face1);

  க்கு (unsigned i = 0; i < மொத்தம்;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    unsigned n = hb_min (MAX_GLYPHS, மொத்தம் - i);

    க்கு (unsigned j = 0; j < n; j++)
    {
      gids[j] = *first_glyph;
      advances[j] = 0;
      first_glyph = &StructAtOffset<மாறிலி hb_codepoint_t> (first_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, உண்மை);
    க்கு (unsigned j = 0; j < n; j++)
    {
      *first_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      first_advance = &StructAtOffset<hb_position_t> (first_advance, advance_stride);
    }

    i += n;
  }
}

நிலையான hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   வெறுமை *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  எனில் (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    திரும்பு பொய்;

  *x = hb_directwrite_scale_x (font, metrics.advanceWidth / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  திரும்பு உண்மை;
}
#endif

நிலையான hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  வெறுமை *font_data HB_UNUSED,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  வெறுமை *user_data HB_UNUSED)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  எனில் (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    திரும்பு பொய்;

  int64_t வலது = (int64_t) metrics.advanceWidth - metrics.வலது_Side_Bearing;
  int64_t உச்சி_2 = (int64_t) metrics.verticalOriginY - metrics.உச்சி_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advanceHeight + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.இடது_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, உச்சி_2);
  extents->width = hb_saturate_sub (hb_directwrite_scale_x (font, வலது), extents->x_bearing);
  extents->height = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  திரும்பு உண்மை;
}


#ifndef HB_NO_DRAW

வகுப்பு GeometrySink : பொது IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

பொது:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       வெறுமை *draw_data)
    : font (font), drawing ({draw_funcs, draw_data}) {}

  HRESULT STDMETHODCALLTYPE Close() override { திரும்பு S_OK; }
  வெறுமை STDMETHODCALLTYPE SetFillMode(D2D1_FILL_MODE) override {}
  வெறுமை STDMETHODCALLTYPE SetSegmentFlags(D2D1_PATH_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, வெறுமை **) override { திரும்பு E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, AddRef)() override { திரும்பு 1; }
  IFACEMETHOD_(ULONG, Release)() override { திரும்பு 1; }

  வெறுமை STDMETHODCALLTYPE BeginFigure(D2D1_POINT_2F startPoint, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_to (font->em_scalef_x (startPoint.x), -(தசம) font->em_scalef_y (startPoint.y));
  }

  வெறுமை STDMETHODCALLTYPE AddBeziers(மாறிலி D2D1_BEZIER_SEGMENT *beziers, UINT beziers_மொத்தம்) override
  {
    க்கு (unsigned i = 0; i < beziers_மொத்தம்; ++i)
      drawing.cubic_to (font->em_scalef_x (beziers[i].point1.x), -(தசம) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(தசம) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(தசம) font->em_scalef_y (beziers[i].point3.y));
  }

  வெறுமை STDMETHODCALLTYPE Add_வரிகள்(மாறிலி D2D1_POINT_2F *points, UINT points_மொத்தம்) override
  {
    க்கு (unsigned i = 0; i < points_மொத்தம்; ++i)
      drawing.வரி_to (font->em_scalef_x (points[i].x), -(தசம) font->em_scalef_y (points[i].y));
  }

  வெறுமை STDMETHODCALLTYPE EndFigure(D2D1_FIGURE_END) override
  {
    drawing.close_path ();
  }
};

நிலையான hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   வெறுமை *font_data HB_UNUSED,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, வெறுமை *draw_data,
				   வெறுமை *user_data)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;

  GeometrySink sink (font, draw_funcs, draw_data);
  UINT16 gid = static_cast<UINT16>(glyph);
  unsigned upem = font->face->get_upem();

  திரும்பு S_OK == dw_face->GetGlyphRunOutline (upem,
					      &gid, nullptr, nullptr,
					      1,
					      பொய், பொய்,
					      &sink);
}

#endif

நிலையான உள்வரி வெறுமை free_static_directwrite_funcs ();

நிலையான கட்டமைப்பு hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  நிலையான hb_font_funcs_t *create ()
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

    திரும்பு funcs;
  }
} static_directwrite_funcs;

நிலையான உள்வரி
வெறுமை free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

நிலையான hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  திரும்பு static_directwrite_funcs.get_unconst ();
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
வெறுமை
hb_directwrite_font_set_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (மாறிலி வெறுமை *) font->data.directwrite;
  எனில் (unlikely (!dw_face))
  {
    hb_font_set_funcs (font,
		       hb_font_funcs_get_empty (),
		       nullptr, nullptr);
    திரும்பு;
  }

  dw_face->AddRef ();
  hb_font_set_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     nullptr, nullptr);
}

#undef MAX_GLYPHS

#endif
