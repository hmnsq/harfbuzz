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

#ifndef HB_FT_COLR_HH
#define HB_FT_COLR_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-decycler.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint-bounded.hh"

#ಅನ್ತರ್ಭಾವ FT_COLOR_H


ಸ್ಥಿರ hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  ವಿಕಲ್ಪ (mode)
  {
    ಪಕ್ಷ FT_COLR_COMPOSITE_CLEAR:          ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_CLEAR;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SRC:            ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SRC;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DEST:           ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DEST;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SRC_OVER:       ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DEST_OVER:      ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SRC_IN:         ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SRC_IN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DEST_IN:        ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DEST_IN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SRC_OUT:        ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DEST_OUT:       ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SRC_ATOP:       ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DEST_ATOP:      ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    ಪಕ್ಷ FT_COLR_COMPOSITE_XOR:            ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_XOR;
    ಪಕ್ಷ FT_COLR_COMPOSITE_PLUS:           ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_PLUS;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SCREEN:         ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SCREEN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_OVERLAY:        ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_OVERLAY;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DARKEN:         ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DARKEN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_LIGHTEN:        ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_COLOR_DODGE:    ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    ಪಕ್ಷ FT_COLR_COMPOSITE_COLOR_BURN:     ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    ಪಕ್ಷ FT_COLR_COMPOSITE_HARD_LIGHT:     ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    ಪಕ್ಷ FT_COLR_COMPOSITE_SOFT_LIGHT:     ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    ಪಕ್ಷ FT_COLR_COMPOSITE_DIFFERENCE:     ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    ಪಕ್ಷ FT_COLR_COMPOSITE_EXCLUSION:      ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    ಪಕ್ಷ FT_COLR_COMPOSITE_MULTIPLY:       ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    ಪಕ್ಷ FT_COLR_COMPOSITE_HSL_HUE:        ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    ಪಕ್ಷ FT_COLR_COMPOSITE_HSL_SATURATION: ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    ಪಕ್ಷ FT_COLR_COMPOSITE_HSL_COLOR:      ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    ಪಕ್ಷ FT_COLR_COMPOSITE_HSL_LUMINOSITY: ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    ಪಕ್ಷ FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    ಸ್ವಭಾವ:                               ಪ್ರತ್ಯಾಗಮ HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_ft_paint_context_t hb_ft_paint_context_t;

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ಧ್ರುವ FT_ClipBox *clip_box,
			 ಭಿನ್ನ *xmin, ಭಿನ್ನ *ymin,
			 ಭಿನ್ನ *xmax, ಭಿನ್ನ *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  ಭಿನ್ನ upem = font->face->get_upem ();
  ಭಿನ್ನ xscale = upem / (font->x_scale ? font->x_scale : upem);
  ಭಿನ್ನ yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ಎಡ.x * xscale;
  *ymin = clip_box->bottom_ಎಡ.y * yscale;
  *xmax = clip_box->ತುದಿ_ಬಲ.x * xscale;
  *ymax = clip_box->ತುದಿ_ಬಲ.y * yscale;
}

ಸ್ಥಿರ unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  ಪ್ರತ್ಯಾಗಮ (alpha * alpha_mult + (1 << 13)) >> 14;
}

ಸಂರಚನಾ hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ಧ್ರುವ hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, ಶೂನ್ಯ *paint_data,
			 hb_array_t<ಧ್ರುವ FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    ಯದಿ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, ಸತ್ಯ);
      hb_font_set_synthetic_slant (font, 0);
    }
    ಅನ್ಯಥಾ
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ಶೂನ್ಯ recurse (FT_OpaquePaint paint)
  {
    ಯದಿ (unlikely (depth_ಎಡ <= 0 || edge_ಎಣಿಕೆ <= 0)) ಪ್ರತ್ಯಾಗಮ;
    depth_ಎಡ--;
    edge_ಎಣಿಕೆ--;
    _hb_ft_paint (ಏತತ್, paint);
    depth_ಎಡ++;
  }

  ಧ್ರುವ hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  ಶೂನ್ಯ *data;
  hb_array_t<ಧ್ರುವ FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ಪೂರ್ಣಾಙ್ಕ depth_ಎಡ = HB_MAX_NESTING_LEVEL;
  ಪೂರ್ಣಾಙ್ಕ edge_ಎಣಿಕೆ = HB_MAX_GRAPH_EDGE_ಎಣಿಕೆ;
};

ಸ್ಥಿರ unsigned
hb_ft_color_ಸಾಲು_get_color_stops (hb_color_ಸಾಲು_t *color_ಸಾಲು,
				   ಶೂನ್ಯ *color_ಸಾಲು_data,
				   unsigned ಪೂರ್ಣಾಙ್ಕ start,
				   unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ,
				   hb_color_stop_t *color_stops,
				   ಶೂನ್ಯ *user_data)
{
  FT_Color_ಸಾಲು *cl = (FT_Color_ಸಾಲು *) color_ಸಾಲು_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  ಯದಿ (ಎಣಿಕೆ && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    ಯದಿ (start >= cl->color_stop_iterator.ಅಂಕೆ_color_stops)
    {
      *ಎಣಿಕೆ = 0;
      ಪ್ರತ್ಯಾಗಮ cl->color_stop_iterator.ಅಂಕೆ_color_stops;
    }

    ಯಾವತ್ (cl->color_stop_iterator.current_color_stop < start)
    {
      ಯದಿ (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        ಪ್ರತ್ಯಾಗಮ cl->color_stop_iterator.ಅಂಕೆ_color_stops;
      }
    }

    ಯಾವತ್ (wrote < *ಎಣಿಕೆ &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      ಯದಿ (ಪರಿಮಾಣ stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      ಅನ್ಯಥಾ
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      ಯದಿ (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      ಅನ್ಯಥಾ
      {
	hb_color_t color;
        ಯದಿ (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	ಅನ್ಯಥಾ ಯದಿ (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	ಅನ್ಯಥಾ
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *ಎಣಿಕೆ = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  ಪ್ರತ್ಯಾಗಮ cl->color_stop_iterator.ಅಂಕೆ_color_stops;
}

ಸ್ಥಿರ hb_paint_extend_t
hb_ft_color_ಸಾಲು_get_extend (hb_color_ಸಾಲು_t *color_ಸಾಲು,
			      ಶೂನ್ಯ *color_ಸಾಲು_data,
			      ಶೂನ್ಯ *user_data)
{
  FT_Color_ಸಾಲು *c = (FT_Color_ಸಾಲು *) color_ಸಾಲು_data;
  ವಿಕಲ್ಪ (c->extend)
  {
    ಸ್ವಭಾವ:
    ಪಕ್ಷ FT_COLR_PAINT_EXTEND_PAD:     ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_PAD;
    ಪಕ್ಷ FT_COLR_PAINT_EXTEND_REPEAT:  ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_REPEAT;
    ಪಕ್ಷ FT_COLR_PAINT_EXTEND_REFLECT: ಪ್ರತ್ಯಾಗಮ HB_PAINT_EXTEND_REFLECT;
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ಧ್ರುವ FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  ಯದಿ (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    ಪ್ರತ್ಯಾಗಮ;
  }

  ಯದಿ (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    ಪ್ರತ್ಯಾಗಮ;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

ಶೂನ್ಯ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  ಯದಿ (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    ಪ್ರತ್ಯಾಗಮ;

  ವಿಕಲ್ಪ (paint.format)
  {
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_ಗಂಟು_t node (c->layers_decycler);
      ಯಾವತ್ (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	ಯದಿ (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  ಅನುವರ್ತ;

	c->recurse (other_paint);
      }
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_ಸಾಲು_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ಸಾಲು_get_color_stops, c,
	hb_ft_color_ಸಾಲು_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_ಸಾಲು_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ಸಾಲು_get_color_stops, c,
	hb_ft_color_ಸಾಲು_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_ಸಾಲು_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ಸಾಲು_get_color_stops, c,
	hb_ft_color_ಸಾಲು_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      ಯದಿ (likely (c->depth_ಎಡ > 0 && c->edge_ಎಣಿಕೆ > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_ಎಣಿಕೆ--;
	c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->ತೆಗೆ_transform (c->data);
	ಭಙ್ಗ;
      }

      c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ತಳ್ಳು_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ತಳ್ಳು_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
      c->funcs->ತೆಗೆ_clip (c->data);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_ಗಂಟು_t node (c->glyphs_decycler);
      ಯದಿ (unlikely (!node.visit (gid)))
	ಪ್ರತ್ಯಾಗಮ;

      c->funcs->ತಳ್ಳು_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      ಯದಿ (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->ತೆಗೆ_transform (c->data);
	ಪ್ರತ್ಯಾಗಮ;
      }
      c->ft_font->lock.lock ();
      c->funcs->ತೆಗೆ_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      ಯದಿ (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_ಬೇರು_TRANSFORM,
				    &other_paint))
      {
        ತಾರ್ಕಿಕ has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        ಯದಿ (has_clip_box)
	{
	  ಭಿನ್ನ xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ತಳ್ಳು_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        ಯದಿ (has_clip_box)
          c->funcs->ತೆಗೆ_clip (c->data);
      }
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ತಳ್ಳು_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      ಭಿನ್ನ dx = paint.u.translate.dx / 65536.f;
      ಭಿನ್ನ dy = paint.u.translate.dy / 65536.f;

      c->funcs->ತಳ್ಳು_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_SCALE:
    {
      ಭಿನ್ನ dx = paint.u.scale.center_x / 65536.f;
      ಭಿನ್ನ dy = paint.u.scale.center_y / 65536.f;
      ಭಿನ್ನ sx = paint.u.scale.scale_x / 65536.f;
      ಭಿನ್ನ sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ತಳ್ಳು_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_ROTATE:
    {
      ಭಿನ್ನ dx = paint.u.rotate.center_x / 65536.f;
      ಭಿನ್ನ dy = paint.u.rotate.center_y / 65536.f;
      ಭಿನ್ನ a = paint.u.rotate.angle / 65536.f;

      c->funcs->ತಳ್ಳು_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_SKEW:
    {
      ಭಿನ್ನ dx = paint.u.skew.center_x / 65536.f;
      ಭಿನ್ನ dy = paint.u.skew.center_y / 65536.f;
      ಭಿನ್ನ sx = paint.u.skew.x_skew_angle / 65536.f;
      ಭಿನ್ನ sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ತಳ್ಳು_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->ತೆಗೆ_transform (c->data);
    }
    ಭಙ್ಗ;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ತಳ್ಳು_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ತಳ್ಳು_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->ತೆಗೆ_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->ತೆಗೆ_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    ಭಙ್ಗ;

    ಪಕ್ಷ FT_COLR_PAINT_FORMAT_MAX: ಭಙ್ಗ;
    ಸ್ವಭಾವ: HB_FALLTHROUGH;
    ಪಕ್ಷ FT_COLR_PAINTFORMAT_UNSUPPORTED: ಭಙ್ಗ;
  }
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ
hb_ft_paint_glyph_colr (hb_font_t *font,
			ಶೂನ್ಯ *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, ಶೂನ್ಯ *paint_data,
			unsigned ಪೂರ್ಣಾಙ್ಕ palette_index,
			hb_color_t foreground,
			ಶೂನ್ಯ *user_data)
{
  ಧ್ರುವ hb_ft_font_t *ft_font = (ಧ್ರುವ hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = ರಿಕ್ತ;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (ಶೂನ್ಯ) FT_Palette_Data_Get(ft_face, &palette_data);
  (ಶೂನ್ಯ) FT_Palette_Select(ft_face, palette_index, &palette);
  ಯದಿ (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (ಶೂನ್ಯ) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ಧ್ರುವ FT_Color *) palette,
				 palette ? palette_data.ಅಂಕೆ_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ಯದಿ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ಬೇರು_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_ಗಂಟು_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    ತಾರ್ಕಿಕ clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    ತಾರ್ಕಿಕ is_bounded = clip;
    ಯದಿ (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_ಗಂಟು_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->ತಳ್ಳು_font_transform (c.data, font);

    ಯದಿ (clip)
    {
      ಭಿನ್ನ xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ತಳ್ಳು_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    ಯದಿ (is_bounded)
      c.recurse (paint);

    ಯದಿ (clip)
      c.funcs->ತೆಗೆ_clip (c.data);

    c.funcs->ತೆಗೆ_transform (c.data);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* COLRv0 */
  iterator.p  = ರಿಕ್ತ;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  ಯದಿ (palette && have_layers)
  {
    ಕುರು
    {
      hb_bool_t is_foreground = ಸತ್ಯ;
      hb_color_t color = foreground;

      ಯದಿ ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = ಅಸತ್ಯ;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } ಯಾವತ್ (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}


#endif /* HB_FT_COLR_HH */
