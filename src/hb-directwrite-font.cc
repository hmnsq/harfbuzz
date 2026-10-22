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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_DIRECTWRITE

#అన్తర్భావ "hb-directwrite.h"

#అన్తర్భావ <d2d1.h>

#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-machinery.hh"

#నిర్వచించు MAX_GLYPHS 256u

స్థిర hb_position_t
hb_directwrite_scale_x (hb_font_t *font, int64_t value)
{
  ఒకవేళ (value >= INT16_MIN && value <= INT16_MAX)
    తిరిగి font->em_scale_x ((int16_t) value);
  తిరిగి font->em_scalef_x ((దశాంశం) value);
}

స్థిర hb_position_t
hb_directwrite_scale_y (hb_font_t *font, int64_t value)
{
  ఒకవేళ (value >= INT16_MIN && value <= INT16_MAX)
    తిరిగి font->em_scale_y ((int16_t) value);
  తిరిగి font->em_scalef_y ((దశాంశం) value);
}

స్థిర గుర్తులేని పూర్ణాంకం
hb_directwrite_get_nominal_glyphs (hb_font_t *font,
				   శూన్య *font_దత్తాంశం HB_వాడని,
				   గుర్తులేని పూర్ణాంకం లెక్క,
				   స్థిరరాశి hb_codepoint_t *మొదటి_unicode,
				   గుర్తులేని పూర్ణాంకం unicode_stride,
				   hb_codepoint_t *మొదటి_glyph,
				   గుర్తులేని పూర్ణాంకం glyph_stride,
				   శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  కొరకు (గుర్తులేని i = 0; i < లెక్క;)
  {
    UINT32 unicodes[MAX_GLYPHS];
    UINT16 gids[MAX_GLYPHS];

    గుర్తులేని n = hb_min (MAX_GLYPHS, లెక్క - i);

    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      unicodes[j] = *మొదటి_unicode;
      మొదటి_unicode = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_unicode, unicode_stride);
    }

    ఒకవేళ (!SUCCEEDED (dw_face->GetGlyphIndices (unicodes, n, gids)))
      తిరిగి i;

    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      ఒకవేళ (!gids[j])
        తిరిగి i + j;
      *మొదటి_glyph = gids[j];
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }

    i += n;
  }

  తిరిగి లెక్క;
}

స్థిర hb_bool_t
hb_directwrite_get_font_h_extents (hb_font_t *font,
				   శూన్య *font_దత్తాంశం HB_వాడని,
				   hb_font_extents_t *metrics,
				   శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  DWRITE_FONT_METRICS dw_metrics;
  dw_face->GetMetrics (&dw_metrics);

  metrics->ascender = hb_directwrite_scale_y (font, dw_metrics.ascent);
  metrics->descender = hb_saturate_neg (hb_directwrite_scale_y (font, dw_metrics.descent));
  metrics->పంక్తి_gap = hb_directwrite_scale_y (font, dw_metrics.పంక్తి_Gap);

  తిరిగి నిజం;
}

స్థిర శూన్య
hb_directwrite_get_glyph_h_advances (hb_font_t* font,
				     శూన్య* font_దత్తాంశం HB_వాడని,
				     గుర్తులేని లెక్క,
				     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				     గుర్తులేని glyph_stride,
				     hb_position_t *మొదటి_advance,
				     గుర్తులేని advance_stride,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  IDWriteFontFace1 *dw_face1 = శూన్యసూచిక;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (శూన్య**)&dw_face1);
  ప్రతిజ్ఞా (dw_face1);

  గుర్తులేని పూర్ణాంకం అంకె_glyphs = font->face->get_అంకె_glyphs ();

  కొరకు (గుర్తులేని i = 0; i < లెక్క;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    గుర్తులేని n = hb_min (MAX_GLYPHS, లెక్క - i);

    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      gids[j] = *మొదటి_glyph;
      advances[j] = 0;
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, అబద్ధం);
    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      // https://github.com/harfbuzz/harfbuzz/issues/5319
      స్వయంచాలక advance = gids[j] < అంకె_glyphs ? advances[j] : 0;
      *మొదటి_advance = hb_directwrite_scale_x (font, advance);
      మొదటి_advance = &Struct_At_స్థానభ్రంశం<hb_position_t> (మొదటి_advance, advance_stride);
    }

    i += n;
  }
}

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL

స్థిర శూన్య
hb_directwrite_get_glyph_v_advances (hb_font_t* font,
				     శూన్య* font_దత్తాంశం HB_వాడని,
				     గుర్తులేని లెక్క,
				     స్థిరరాశి hb_codepoint_t *మొదటి_glyph,
				     గుర్తులేని glyph_stride,
				     hb_position_t *మొదటి_advance,
				     గుర్తులేని advance_stride,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  IDWriteFontFace1 *dw_face1 = శూన్యసూచిక;
  dw_face->QueryInterface (__uuidof(IDWriteFontFace1), (శూన్య**)&dw_face1);
  ప్రతిజ్ఞా (dw_face1);

  కొరకు (గుర్తులేని i = 0; i < లెక్క;)
  {
    UINT16 gids[MAX_GLYPHS];
    INT32 advances[MAX_GLYPHS];

    గుర్తులేని n = hb_min (MAX_GLYPHS, లెక్క - i);

    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      gids[j] = *మొదటి_glyph;
      advances[j] = 0;
      మొదటి_glyph = &Struct_At_స్థానభ్రంశం<స్థిరరాశి hb_codepoint_t> (మొదటి_glyph, glyph_stride);
    }
    dw_face1->GetDesignGlyphAdvances (n, gids, advances, నిజం);
    కొరకు (గుర్తులేని j = 0; j < n; j++)
    {
      *మొదటి_advance = hb_saturate_neg (hb_directwrite_scale_y (font, advances[j]));
      మొదటి_advance = &Struct_At_స్థానభ్రంశం<hb_position_t> (మొదటి_advance, advance_stride);
    }

    i += n;
  }
}

స్థిర hb_bool_t
hb_directwrite_get_glyph_v_origin (hb_font_t *font,
				   శూన్య *font_దత్తాంశం HB_వాడని,
				   hb_codepoint_t glyph,
				   hb_position_t *x,
				   hb_position_t *y,
				   శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ఒకవేళ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    తిరిగి అబద్ధం;

  *x = hb_directwrite_scale_x (font, metrics.advance_వెడల్పు / 2);
  *y = hb_directwrite_scale_y (font, metrics.verticalOriginY); // Untested

  తిరిగి నిజం;
}
#అయితే_ముగింపు

స్థిర hb_bool_t
hb_directwrite_get_glyph_extents (hb_font_t *font,
				  శూన్య *font_దత్తాంశం HB_వాడని,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  UINT16 gid = glyph;
  DWRITE_GLYPH_METRICS metrics;

  ఒకవేళ (FAILED (dw_face->GetDesignGlyphMetrics (&gid, 1, &metrics)))
    తిరిగి అబద్ధం;

  int64_t కుడి = (int64_t) metrics.advance_వెడల్పు - metrics.కుడి_Side_Bearing;
  int64_t శిఖరం_2 = (int64_t) metrics.verticalOriginY - metrics.శిఖరం_Side_Bearing;
  int64_t bottom = (int64_t) metrics.verticalOriginY - metrics.advance_ఎత్తు + metrics.bottomSideBearing;

  extents->x_bearing = hb_directwrite_scale_x (font, metrics.ఎడమ_Side_Bearing);
  extents->y_bearing = hb_directwrite_scale_y (font, శిఖరం_2);
  extents->వెడల్పు = hb_saturate_sub (hb_directwrite_scale_x (font, కుడి), extents->x_bearing);
  extents->ఎత్తు = hb_saturate_sub (hb_directwrite_scale_y (font, bottom),
				     extents->y_bearing); // Magic

  తిరిగి నిజం;
}


#నిర్వచితం_కాకపోతే HB_NO_DRAW

తరగతి GeometrySink : బహిరంగ IDWriteGeometrySink
{
  hb_font_t *font;
  hb_draw_session_t drawing;

బహిరంగ:
  GeometrySink(hb_font_t *font,
	       hb_draw_funcs_t *draw_funcs,
	       శూన్య *draw_దత్తాంశం)
    : font (font), drawing ({draw_funcs, draw_దత్తాంశం}) {}

  HRESULT STDMETHODCALLTYPE Close() override { తిరిగి S_OK; }
  శూన్య STDMETHODCALLTYPE సమితి_Fill_బహుళకం(D2_D1_FILL_బహుళకం) override {}
  శూన్య STDMETHODCALLTYPE సమితి_Segment_Flags(D2_D1_మార్గం_SEGMENT) override {}

  IFACEMETHOD(QueryInterface)(REFIID, శూన్య **) override { తిరిగి E_NOINTERFACE; }
  IFACEMETHOD_(ULONG, కూడు_Ref)() override { తిరిగి 1; }
  IFACEMETHOD_(ULONG, Release)() override { తిరిగి 1; }

  శూన్య STDMETHODCALLTYPE BeginFigure(D2_D1_బిందువు_2_F ప్రారంభం_బిందువు, D2D1_FIGURE_BEGIN) override
  {
    drawing.move_గమ్యం (font->em_scalef_x (ప్రారంభం_బిందువు.x), -(దశాంశం) font->em_scalef_y (ప్రారంభం_బిందువు.y));
  }

  శూన్య STDMETHODCALLTYPE కూడు_Beziers(స్థిరరాశి D2D1_BEZIER_SEGMENT *beziers, UINT beziers_లెక్క) override
  {
    కొరకు (గుర్తులేని i = 0; i < beziers_లెక్క; ++i)
      drawing.cubic_గమ్యం (font->em_scalef_x (beziers[i].point1.x), -(దశాంశం) font->em_scalef_y (beziers[i].point1.y),
			font->em_scalef_x (beziers[i].point2.x), -(దశాంశం) font->em_scalef_y (beziers[i].point2.y),
			font->em_scalef_x (beziers[i].point3.x), -(దశాంశం) font->em_scalef_y (beziers[i].point3.y));
  }

  శూన్య STDMETHODCALLTYPE కూడు_పంక్తులు(స్థిరరాశి D2_D1_బిందువు_2_F *points, UINT points_లెక్క) override
  {
    కొరకు (గుర్తులేని i = 0; i < points_లెక్క; ++i)
      drawing.పంక్తి_గమ్యం (font->em_scalef_x (points[i].x), -(దశాంశం) font->em_scalef_y (points[i].y));
  }

  శూన్య STDMETHODCALLTYPE చివర_Figure(D2_D1_FIGURE_చివర) override
  {
    drawing.close_మార్గం ();
  }
};

స్థిర hb_bool_t
hb_directwrite_draw_glyph_or_fail (hb_font_t *font,
				   శూన్య *font_దత్తాంశం HB_వాడని,
				   hb_codepoint_t glyph,
				   hb_draw_funcs_t *draw_funcs, శూన్య *draw_దత్తాంశం,
				   శూన్య *user_దత్తాంశం)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;

  GeometrySink sink (font, draw_funcs, draw_దత్తాంశం);
  UINT16 gid = static_cast<UINT16>(glyph);
  గుర్తులేని upem = font->face->get_upem();

  తిరిగి S_OK == dw_face->Get_Glyph_నిరంతరం_Outline (upem,
					      &gid, శూన్యసూచిక, శూన్యసూచిక,
					      1,
					      అబద్ధం, అబద్ధం,
					      &sink);
}

#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య free_static_directwrite_funcs ();

స్థిర నిర్మాణం hb_directwrite_font_funcs_lazy_loader_t : hb_font_funcs_lazy_loader_t<hb_directwrite_font_funcs_lazy_loader_t>
{
  స్థిర hb_font_funcs_t *create ()
  {
    hb_font_funcs_t *funcs = hb_font_funcs_create ();

    hb_font_funcs_సమితి_nominal_glyphs_func (funcs, hb_directwrite_get_nominal_glyphs, శూన్యసూచిక, శూన్యసూచిక);
    //hb_font_funcs_set_variation_glyph_func (funcs, hb_directwrite_get_variation_glyph, nullptr, nullptr);

    hb_font_funcs_సమితి_font_h_extents_func (funcs, hb_directwrite_get_font_h_extents, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_h_advances_func (funcs, hb_directwrite_get_glyph_h_advances, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
    hb_font_funcs_సమితి_glyph_v_advances_func (funcs, hb_directwrite_get_glyph_v_advances, శూన్యసూచిక, శూన్యసూచిక);
    hb_font_funcs_సమితి_glyph_v_origin_func (funcs, hb_directwrite_get_glyph_v_origin, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_NO_DRAW
    hb_font_funcs_సమితి_draw_glyph_or_fail_func (funcs, hb_directwrite_draw_glyph_or_fail, శూన్యసూచిక, శూన్యసూచిక);
#అయితే_ముగింపు

    hb_font_funcs_సమితి_glyph_extents_func (funcs, hb_directwrite_get_glyph_extents, శూన్యసూచిక, శూన్యసూచిక);

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_GLYPH_పేర్లు
    //hb_font_funcs_set_glyph_name_func (funcs, hb_directwrite_get_glyph_name, nullptr, nullptr);
    //hb_font_funcs_set_glyph_from_name_func (funcs, hb_directwrite_get_glyph_from_name, nullptr, nullptr);
#అయితే_ముగింపు

    hb_font_funcs_make_immutable (funcs);

    hb_atexit (free_static_directwrite_funcs);

    తిరిగి funcs;
  }
} static_directwrite_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_directwrite_funcs ()
{
  static_directwrite_funcs.free_instance ();
}

స్థిర hb_font_funcs_t *
_hb_directwrite_get_font_funcs ()
{
  తిరిగి static_directwrite_funcs.get_unconst ();
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
శూన్య
hb_directwrite_font_సమితి_funcs (hb_font_t *font)
{
  IDWriteFontFace *dw_face = (IDWriteFontFace *) (స్థిరరాశి శూన్య *) font->దత్తాంశం.directwrite;
  ఒకవేళ (unlikely (!dw_face))
  {
    hb_font_సమితి_funcs (font,
		       hb_font_funcs_get_ఖాళీలు (),
		       శూన్యసూచిక, శూన్యసూచిక);
    తిరిగి;
  }

  dw_face->కూడు_Ref ();
  hb_font_సమితి_funcs (font,
		     _hb_directwrite_get_font_funcs (),
		     శూన్యసూచిక, శూన్యసూచిక);
}

#నిర్వచనం_రద్దు MAX_GLYPHS

#అయితే_ముగింపు
