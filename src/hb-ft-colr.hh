/*
 * Copyright © 2022  Behdad Esfahbod
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
 */

#నిర్వచితం_కాకపోతే HB_FT_COLR_HH
#నిర్వచించు HB_FT_COLR_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-decycler.hh"
#అన్తర్భావ "hb-paint-bounded.hh"

#అన్తర్భావ FT_COLOR_H


స్థిర hb_paint_సంయుక్త_బహుళకం_t
hb_ft_paint_సంయుక్త_బహుళకం (FT_సంయుక్త_బహుళకం బహుళకం)
{
  ఎంపిక (బహుళకం)
  {
    స్థితి FT_COLR_సంయుక్త_CLEAR:          తిరిగి HB_PAINT_సంయుక్త_బహుళకం_CLEAR;
    స్థితి FT_COLR_సంయుక్త_SRC:            తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SRC;
    స్థితి FT_COLR_సంయుక్త_DEST:           తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DEST;
    స్థితి FT_COLR_సంయుక్త_SRC_OVER:       తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER;
    స్థితి FT_COLR_సంయుక్త_DEST_OVER:      తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER;
    స్థితి FT_COLR_సంయుక్త_SRC_లోపల:         తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల;
    స్థితి FT_COLR_సంయుక్త_DEST_లోపల:        తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల;
    స్థితి FT_COLR_సంయుక్త_SRC_బహిర్గతం:        తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం;
    స్థితి FT_COLR_సంయుక్త_DEST_బహిర్గతం:       తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం;
    స్థితి FT_COLR_సంయుక్త_SRC_ATOP:       తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP;
    స్థితి FT_COLR_సంయుక్త_DEST_ATOP:      తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP;
    స్థితి FT_COLR_సంయుక్త_XOR:            తిరిగి HB_PAINT_సంయుక్త_బహుళకం_XOR;
    స్థితి FT_COLR_సంయుక్త_PLUS:           తిరిగి HB_PAINT_సంయుక్త_బహుళకం_PLUS;
    స్థితి FT_COLR_సంయుక్త_SCREEN:         తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SCREEN;
    స్థితి FT_COLR_సంయుక్త_OVERLAY:        తిరిగి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY;
    స్థితి FT_COLR_సంయుక్త_DARKEN:         తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DARKEN;
    స్థితి FT_COLR_సంయుక్త_LIGHTEN:        తిరిగి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN;
    స్థితి FT_COLR_సంయుక్త_COLOR_DODGE:    తిరిగి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE;
    స్థితి FT_COLR_సంయుక్త_COLOR_BURN:     తిరిగి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN;
    స్థితి FT_COLR_సంయుక్త_HARD_LIGHT:     తిరిగి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT;
    స్థితి FT_COLR_సంయుక్త_SOFT_LIGHT:     తిరిగి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT;
    స్థితి FT_COLR_సంయుక్త_DIFFERENCE:     తిరిగి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE;
    స్థితి FT_COLR_సంయుక్త_EXCLUSION:      తిరిగి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION;
    స్థితి FT_COLR_సంయుక్త_MULTIPLY:       తిరిగి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY;
    స్థితి FT_COLR_సంయుక్త_HSL_HUE:        తిరిగి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE;
    స్థితి FT_COLR_సంయుక్త_HSL_SATURATION: తిరిగి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION;
    స్థితి FT_COLR_సంయుక్త_HSL_COLOR:      తిరిగి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR;
    స్థితి FT_COLR_సంయుక్త_HSL_LUMINOSITY: తిరిగి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY;

    స్థితి FT_COLR_సంయుక్త_MAX:            HB_FALLTHROUGH;
    అప్రమేయ:                               తిరిగి HB_PAINT_సంయుక్త_బహుళకం_CLEAR;
  }
}

రకనిర్వచనం నిర్మాణం hb_ft_paint_context_t hb_ft_paint_context_t;

స్థిర శూన్య
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

స్థిర శూన్య
_hb_ft_unscale_clip_box (hb_font_t *font,
			 స్థిరరాశి FT_ClipBox *clip_box,
			 దశాంశం *xmin, దశాంశం *ymin,
			 దశాంశం *xmax, దశాంశం *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  దశాంశం upem = font->face->get_upem ();
  దశాంశం xscale = upem / (font->x_scale ? font->x_scale : upem);
  దశాంశం yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ఎడమ.x * xscale;
  *ymin = clip_box->bottom_ఎడమ.y * yscale;
  *xmax = clip_box->శిఖరం_కుడి.x * xscale;
  *ymax = clip_box->శిఖరం_కుడి.y * yscale;
}

స్థిర గుర్తులేని
_hb_ft_color_alpha (గుర్తులేని alpha, గుర్తులేని alpha_mult)
{
  తిరిగి (alpha * alpha_mult + (1 << 13)) >> 14;
}

నిర్మాణం hb_ft_paint_context_t
{
  hb_ft_paint_context_t (స్థిరరాశి hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			 hb_array_t<స్థిరరాశి FT_Color> palette,
			 గుర్తులేని palette_సూచీ,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), దత్తాంశం (paint_దత్తాంశం),
    palette (palette), palette_సూచీ (palette_సూచీ), foreground (foreground)
  {
    ఒకవేళ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_సమితి_synthetic_bold (font, 0, 0, నిజం);
      hb_font_సమితి_synthetic_slant (font, 0);
    }
    లేకపోతే
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  శూన్య recurse (FT_OpaquePaint paint)
  {
    ఒకవేళ (unlikely (లోతు_ఎడమ <= 0 || అంచు_లెక్క <= 0)) తిరిగి;
    లోతు_ఎడమ--;
    అంచు_లెక్క--;
    _hb_ft_paint (ఇది, paint);
    లోతు_ఎడమ++;
  }

  స్థిరరాశి hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  శూన్య *దత్తాంశం;
  hb_array_t<స్థిరరాశి FT_Color> palette;
  గుర్తులేని palette_సూచీ;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  పూర్ణాంకం లోతు_ఎడమ = HB_MAX_NESTING_LEVEL;
  పూర్ణాంకం అంచు_లెక్క = HB_MAX_లేఖాచిత్రం_అంచు_లెక్క;
};

స్థిర గుర్తులేని
hb_ft_color_పంక్తి_get_color_stops (hb_color_పంక్తి_t *color_పంక్తి,
				   శూన్య *color_పంక్తి_దత్తాంశం,
				   గుర్తులేని పూర్ణాంకం ప్రారంభం,
				   గుర్తులేని పూర్ణాంకం *లెక్క,
				   hb_color_stop_t *color_stops,
				   శూన్య *user_దత్తాంశం)
{
  FT_Color_పంక్తి *cl = (FT_Color_పంక్తి *) color_పంక్తి_దత్తాంశం;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_దత్తాంశం;

  ఒకవేళ (లెక్క && color_stops)
  {
    FT_ColorStop stop;
    గుర్తులేని wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    ఒకవేళ (ప్రారంభం >= cl->color_stop_iterator.అంకె_color_stops)
    {
      *లెక్క = 0;
      తిరిగి cl->color_stop_iterator.అంకె_color_stops;
    }

    అయితే (cl->color_stop_iterator.current_color_stop < ప్రారంభం)
    {
      ఒకవేళ (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        తిరిగి cl->color_stop_iterator.అంకె_color_stops;
      }
    }

    అయితే (wrote < *లెక్క &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      ఒకవేళ (పరిమాణం stop.stop_స్థానభ్రంశం_2 == 2)
	color_stops->స్థానభ్రంశం = stop.stop_స్థానభ్రంశం_2 / 16384.f;
      లేకపోతే
	color_stops->స్థానభ్రంశం = stop.stop_స్థానభ్రంశం_2 / 65536.f;

      color_stops->is_foreground = stop.color.palette_సూచీ == 0xFFFF;
      ఒకవేళ (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      లేకపోతే
      {
	hb_color_t color;
        ఒకవేళ (c->funcs->custom_palette_color (c->దత్తాంశం, stop.color.palette_సూచీ, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	లేకపోతే ఒకవేళ (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_సూచీ];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	లేకపోతే
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *లెక్క = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  తిరిగి cl->color_stop_iterator.అంకె_color_stops;
}

స్థిర hb_paint_extend_t
hb_ft_color_పంక్తి_get_extend (hb_color_పంక్తి_t *color_పంక్తి,
			      శూన్య *color_పంక్తి_దత్తాంశం,
			      శూన్య *user_దత్తాంశం)
{
  FT_Color_పంక్తి *c = (FT_Color_పంక్తి *) color_పంక్తి_దత్తాంశం;
  ఎంపిక (c->extend)
  {
    అప్రమేయ:
    స్థితి FT_COLR_PAINT_EXTEND_PAD:     తిరిగి HB_PAINT_EXTEND_PAD;
    స్థితి FT_COLR_PAINT_EXTEND_REPEAT:  తిరిగి HB_PAINT_EXTEND_REPEAT;
    స్థితి FT_COLR_PAINT_EXTEND_REFLECT: తిరిగి HB_PAINT_EXTEND_REFLECT;
  }
}

స్థిర శూన్య
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			స్థిరరాశి FT_Color_సూచీ &color_సూచీ,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_సూచీ.palette_సూచీ == 0xFFFF;

  ఒకవేళ (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_సూచీ.alpha));
    తిరిగి;
  }

  ఒకవేళ (c->funcs->custom_palette_color (c->దత్తాంశం, color_సూచీ.palette_సూచీ, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_సూచీ.alpha));
    తిరిగి;
  }

  FT_Color ft_color = c->palette[color_సూచీ.palette_సూచీ];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_సూచీ.alpha));
}

శూన్య
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  ఒకవేళ (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    తిరిగి;

  ఎంపిక (paint.format)
  {
    స్థితి FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_కణుపు_t node (c->layers_decycler);
      అయితే (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	ఒకవేళ (unlikely (!node.సందర్శించు ((uintptr_t) other_paint.p)))
	  కొనసాగు;

	c->recurse (other_paint);
      }
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->దత్తాంశం, is_foreground, color);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_రేఖీయ_GRADIENT:
    {
      hb_color_పంక్తి_t cl = {
	&paint.u.రేఖీయ_gradient.colorline,
	hb_ft_color_పంక్తి_get_color_stops, c,
	hb_ft_color_పంక్తి_get_extend, శూన్యసూచిక
      };

      c->funcs->రేఖీయ_gradient (c->దత్తాంశం, &cl,
				 paint.u.రేఖీయ_gradient.p0.x / 65536.f,
				 paint.u.రేఖీయ_gradient.p0.y / 65536.f,
				 paint.u.రేఖీయ_gradient.p1.x / 65536.f,
				 paint.u.రేఖీయ_gradient.p1.y / 65536.f,
				 paint.u.రేఖీయ_gradient.p2.x / 65536.f,
				 paint.u.రేఖీయ_gradient.p2.y / 65536.f);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_పంక్తి_t cl = {
	&paint.u.రేఖీయ_gradient.colorline,
	hb_ft_color_పంక్తి_get_color_stops, c,
	hb_ft_color_పంక్తి_get_extend, శూన్యసూచిక
      };

      c->funcs->radial_gradient (c->దత్తాంశం, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_పంక్తి_t cl = {
	&paint.u.రేఖీయ_gradient.colorline,
	hb_ft_color_పంక్తి_get_color_stops, c,
	hb_ft_color_పంక్తి_get_extend, శూన్యసూచిక
      };

      c->funcs->sweep_gradient (c->దత్తాంశం, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.ప్రారంభం_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.చివర_angle / 65536.f + 1) * HB_PI);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      ఒకవేళ (likely (c->లోతు_ఎడమ > 0 && c->అంచు_లెక్క > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->అంచు_లెక్క--;
	c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->దత్తాంశం, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->తీయి_transform (c->దత్తాంశం);
	ఆపు;
      }

      c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->నెట్టు_clip_glyph (c->దత్తాంశం, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->నెట్టు_font_transform (c->దత్తాంశం, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
      c->funcs->తీయి_clip (c->దత్తాంశం);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_కణుపు_t node (c->glyphs_decycler);
      ఒకవేళ (unlikely (!node.సందర్శించు (gid)))
	తిరిగి;

      c->funcs->నెట్టు_inverse_font_transform (c->దత్తాంశం, c->font);
      c->ft_font->lock.unlock ();
      ఒకవేళ (c->funcs->color_glyph (c->దత్తాంశం, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->తీయి_transform (c->దత్తాంశం);
	తిరిగి;
      }
      c->ft_font->lock.lock ();
      c->funcs->తీయి_transform (c->దత్తాంశం);

      FT_OpaquePaint other_paint = {0};
      ఒకవేళ (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_మూలం_TRANSFORM,
				    &other_paint))
      {
        తార్కిక has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        ఒకవేళ (has_clip_box)
	{
	  దశాంశం xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->నెట్టు_clip_rectangle (c->దత్తాంశం, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        ఒకవేళ (has_clip_box)
          c->funcs->తీయి_clip (c->దత్తాంశం);
      }
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->నెట్టు_transform (c->దత్తాంశం,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      దశాంశం dx = paint.u.translate.dx / 65536.f;
      దశాంశం dy = paint.u.translate.dy / 65536.f;

      c->funcs->నెట్టు_translate (c->దత్తాంశం, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_SCALE:
    {
      దశాంశం dx = paint.u.scale.center_x / 65536.f;
      దశాంశం dy = paint.u.scale.center_y / 65536.f;
      దశాంశం sx = paint.u.scale.scale_x / 65536.f;
      దశాంశం sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->నెట్టు_scale_around_center (c->దత్తాంశం, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_ROTATE:
    {
      దశాంశం dx = paint.u.rotate.center_x / 65536.f;
      దశాంశం dy = paint.u.rotate.center_y / 65536.f;
      దశాంశం a = paint.u.rotate.angle / 65536.f;

      c->funcs->నెట్టు_rotate_around_center (c->దత్తాంశం, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_SKEW:
    {
      దశాంశం dx = paint.u.skew.center_x / 65536.f;
      దశాంశం dy = paint.u.skew.center_y / 65536.f;
      దశాంశం sx = paint.u.skew.x_skew_angle / 65536.f;
      దశాంశం sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->నెట్టు_skew_around_center (c->దత్తాంశం, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->తీయి_transform (c->దత్తాంశం);
    }
    ఆపు;
    స్థితి FT_COLR_PAINTFORMAT_సంయుక్త:
    {
      c->funcs->నెట్టు_సమూహం (c->దత్తాంశం);
      c->recurse (paint.u.సంయుక్త_2.backdrop_paint);
      c->funcs->నెట్టు_సమూహం (c->దత్తాంశం);
      c->recurse (paint.u.సంయుక్త_2.ఆధారం_paint);
      c->funcs->తీయి_సమూహం (c->దత్తాంశం, hb_ft_paint_సంయుక్త_బహుళకం (paint.u.సంయుక్త_2.సంయుక్త_బహుళకం_2));
      c->funcs->తీయి_సమూహం (c->దత్తాంశం, HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER);
    }
    ఆపు;

    స్థితి FT_COLR_PAINT_FORMAT_MAX: ఆపు;
    అప్రమేయ: HB_FALLTHROUGH;
    స్థితి FT_COLR_PAINTFORMAT_UNSUPPORTED: ఆపు;
  }
}


స్థిర తార్కిక
hb_ft_paint_glyph_colr (hb_font_t *font,
			శూన్య *font_దత్తాంశం,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, శూన్య *paint_దత్తాంశం,
			గుర్తులేని పూర్ణాంకం palette_సూచీ,
			hb_color_t foreground,
			శూన్య *user_దత్తాంశం)
{
  స్థిరరాశి hb_ft_font_t *ft_font = (స్థిరరాశి hb_ft_font_t *) font_దత్తాంశం;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_దత్తాంశం   palette_దత్తాంశం = {};
  FT_Color*         palette = ఖాళీ;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_సూచీ;
  FT_UInt  layer_color_సూచీ;

  (శూన్య) FT_Palette_దత్తాంశం_Get(ft_face, &palette_దత్తాంశం);
  (శూన్య) FT_Palette_Select(ft_face, palette_సూచీ, &palette);
  ఒకవేళ (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (శూన్య) FT_Palette_Select(ft_face, 0, &palette);
  }

  స్వయంచాలక palette_array = hb_array ((స్థిరరాశి FT_Color *) palette,
				 palette ? palette_దత్తాంశం.అంకె_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ఒకవేళ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_మూలం_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_దత్తాంశం,
			     palette_array, palette_సూచీ, foreground);
    hb_decycler_కణుపు_t node (c.glyphs_decycler);
    node.సందర్శించు (gid);

    FT_ClipBox clip_box;
    తార్కిక clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    తార్కిక is_bounded = clip;
    ఒకవేళ (!is_bounded)
    {
      స్వయంచాలక *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_దత్తాంశం;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_దత్తాంశం,
			        palette_array, palette_సూచీ, foreground);
      hb_decycler_కణుపు_t node2 (ce.glyphs_decycler);
      node2.సందర్శించు (gid);
      ce.recurse (paint);
      is_bounded = bounded_దత్తాంశం.is_bounded ();
    }

    c.funcs->నెట్టు_font_transform (c.దత్తాంశం, font);

    ఒకవేళ (clip)
    {
      దశాంశం xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->నెట్టు_clip_rectangle (c.దత్తాంశం, xmin, ymin, xmax, ymax);
    }

    ఒకవేళ (is_bounded)
      c.recurse (paint);

    ఒకవేళ (clip)
      c.funcs->తీయి_clip (c.దత్తాంశం);

    c.funcs->తీయి_transform (c.దత్తాంశం);

    తిరిగి నిజం;
  }

  /* COLRv0 */
  iterator.p  = ఖాళీ;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_సూచీ,
					 &layer_color_సూచీ,
					 &iterator);

  ఒకవేళ (palette && have_layers)
  {
    చేయి
    {
      hb_bool_t is_foreground = నిజం;
      hb_color_t color = foreground;

      ఒకవేళ ( layer_color_సూచీ != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_సూచీ];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = అబద్ధం;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_దత్తాంశం, layer_glyph_సూచీ, font, is_foreground, color);
      ft_font->lock.lock ();

    } అయితే (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_సూచీ,
				      &layer_color_సూచీ,
				      &iterator));
    తిరిగి నిజం;
  }

  తిరిగి అబద్ధం;
}


#అయితే_ముగింపు /* HB_FT_COLR_HH */
