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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-decycler.hh"
#અન્તર્ભાવ "hb-paint-bounded.hh"

#અન્તર્ભાવ FT_COLOR_H


સ્થિર hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  પસંદગી (mode)
  {
    સ્થિતિ FT_COLR_COMPOSITE_CLEAR:          પાછા HB_PAINT_COMPOSITE_MODE_CLEAR;
    સ્થિતિ FT_COLR_COMPOSITE_SRC:            પાછા HB_PAINT_COMPOSITE_MODE_SRC;
    સ્થિતિ FT_COLR_COMPOSITE_DEST:           પાછા HB_PAINT_COMPOSITE_MODE_DEST;
    સ્થિતિ FT_COLR_COMPOSITE_SRC_OVER:       પાછા HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    સ્થિતિ FT_COLR_COMPOSITE_DEST_OVER:      પાછા HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    સ્થિતિ FT_COLR_COMPOSITE_SRC_IN:         પાછા HB_PAINT_COMPOSITE_MODE_SRC_IN;
    સ્થિતિ FT_COLR_COMPOSITE_DEST_IN:        પાછા HB_PAINT_COMPOSITE_MODE_DEST_IN;
    સ્થિતિ FT_COLR_COMPOSITE_SRC_OUT:        પાછા HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    સ્થિતિ FT_COLR_COMPOSITE_DEST_OUT:       પાછા HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    સ્થિતિ FT_COLR_COMPOSITE_SRC_ATOP:       પાછા HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    સ્થિતિ FT_COLR_COMPOSITE_DEST_ATOP:      પાછા HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    સ્થિતિ FT_COLR_COMPOSITE_XOR:            પાછા HB_PAINT_COMPOSITE_MODE_XOR;
    સ્થિતિ FT_COLR_COMPOSITE_PLUS:           પાછા HB_PAINT_COMPOSITE_MODE_PLUS;
    સ્થિતિ FT_COLR_COMPOSITE_SCREEN:         પાછા HB_PAINT_COMPOSITE_MODE_SCREEN;
    સ્થિતિ FT_COLR_COMPOSITE_OVERLAY:        પાછા HB_PAINT_COMPOSITE_MODE_OVERLAY;
    સ્થિતિ FT_COLR_COMPOSITE_DARKEN:         પાછા HB_PAINT_COMPOSITE_MODE_DARKEN;
    સ્થિતિ FT_COLR_COMPOSITE_LIGHTEN:        પાછા HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    સ્થિતિ FT_COLR_COMPOSITE_COLOR_DODGE:    પાછા HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    સ્થિતિ FT_COLR_COMPOSITE_COLOR_BURN:     પાછા HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    સ્થિતિ FT_COLR_COMPOSITE_HARD_LIGHT:     પાછા HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    સ્થિતિ FT_COLR_COMPOSITE_SOFT_LIGHT:     પાછા HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    સ્થિતિ FT_COLR_COMPOSITE_DIFFERENCE:     પાછા HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    સ્થિતિ FT_COLR_COMPOSITE_EXCLUSION:      પાછા HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    સ્થિતિ FT_COLR_COMPOSITE_MULTIPLY:       પાછા HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    સ્થિતિ FT_COLR_COMPOSITE_HSL_HUE:        પાછા HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    સ્થિતિ FT_COLR_COMPOSITE_HSL_SATURATION: પાછા HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    સ્થિતિ FT_COLR_COMPOSITE_HSL_COLOR:      પાછા HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    સ્થિતિ FT_COLR_COMPOSITE_HSL_LUMINOSITY: પાછા HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    સ્થિતિ FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    મૂળભૂત:                               પાછા HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

પ્રકારવ્યાખ્યા સંરચના hb_ft_paint_context_t hb_ft_paint_context_t;

સ્થિર શૂન્ય
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

સ્થિર શૂન્ય
_hb_ft_unscale_clip_box (hb_font_t *font,
			 અચલ FT_ClipBox *clip_box,
			 દશાંશ *xmin, દશાંશ *ymin,
			 દશાંશ *xmax, દશાંશ *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  દશાંશ upem = font->face->get_upem ();
  દશાંશ xscale = upem / (font->x_scale ? font->x_scale : upem);
  દશાંશ yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ડાબું.x * xscale;
  *ymin = clip_box->bottom_ડાબું.y * yscale;
  *xmax = clip_box->ટોચ_જમણું.x * xscale;
  *ymax = clip_box->ટોચ_જમણું.y * yscale;
}

સ્થિર unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  પાછા (alpha * alpha_mult + (1 << 13)) >> 14;
}

સંરચના hb_ft_paint_context_t
{
  hb_ft_paint_context_t (અચલ hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, શૂન્ય *paint_data,
			 hb_array_t<અચલ FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    જો (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, સાચું);
      hb_font_set_synthetic_slant (font, 0);
    }
    નહીંતર
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  શૂન્ય recurse (FT_OpaquePaint paint)
  {
    જો (unlikely (depth_ડાબું <= 0 || edge_ગણતરી <= 0)) પાછા;
    depth_ડાબું--;
    edge_ગણતરી--;
    _hb_ft_paint (આ, paint);
    depth_ડાબું++;
  }

  અચલ hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  શૂન્ય *data;
  hb_array_t<અચલ FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  પૂર્ણાંક depth_ડાબું = HB_MAX_NESTING_LEVEL;
  પૂર્ણાંક edge_ગણતરી = HB_MAX_GRAPH_EDGE_ગણતરી;
};

સ્થિર unsigned
hb_ft_color_લીટી_get_color_stops (hb_color_લીટી_t *color_લીટી,
				   શૂન્ય *color_લીટી_data,
				   unsigned પૂર્ણાંક start,
				   unsigned પૂર્ણાંક *ગણતરી,
				   hb_color_stop_t *color_stops,
				   શૂન્ય *user_data)
{
  FT_Color_લીટી *cl = (FT_Color_લીટી *) color_લીટી_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  જો (ગણતરી && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    જો (start >= cl->color_stop_iterator.અંક_color_stops)
    {
      *ગણતરી = 0;
      પાછા cl->color_stop_iterator.અંક_color_stops;
    }

    જ્યારે (cl->color_stop_iterator.current_color_stop < start)
    {
      જો (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        પાછા cl->color_stop_iterator.અંક_color_stops;
      }
    }

    જ્યારે (wrote < *ગણતરી &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      જો (કદ stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      નહીંતર
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      જો (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      નહીંતર
      {
	hb_color_t color;
        જો (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	નહીંતર જો (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	નહીંતર
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *ગણતરી = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  પાછા cl->color_stop_iterator.અંક_color_stops;
}

સ્થિર hb_paint_extend_t
hb_ft_color_લીટી_get_extend (hb_color_લીટી_t *color_લીટી,
			      શૂન્ય *color_લીટી_data,
			      શૂન્ય *user_data)
{
  FT_Color_લીટી *c = (FT_Color_લીટી *) color_લીટી_data;
  પસંદગી (c->extend)
  {
    મૂળભૂત:
    સ્થિતિ FT_COLR_PAINT_EXTEND_PAD:     પાછા HB_PAINT_EXTEND_PAD;
    સ્થિતિ FT_COLR_PAINT_EXTEND_REPEAT:  પાછા HB_PAINT_EXTEND_REPEAT;
    સ્થિતિ FT_COLR_PAINT_EXTEND_REFLECT: પાછા HB_PAINT_EXTEND_REFLECT;
  }
}

સ્થિર શૂન્ય
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			અચલ FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  જો (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    પાછા;
  }

  જો (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    પાછા;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

શૂન્ય
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  જો (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    પાછા;

  પસંદગી (paint.format)
  {
    સ્થિતિ FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_ગાંઠ_t node (c->layers_decycler);
      જ્યારે (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	જો (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  ચાલુ;

	c->recurse (other_paint);
      }
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_લીટી_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_લીટી_get_color_stops, c,
	hb_ft_color_લીટી_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_લીટી_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_લીટી_get_color_stops, c,
	hb_ft_color_લીટી_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_લીટી_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_લીટી_get_color_stops, c,
	hb_ft_color_લીટી_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      જો (likely (c->depth_ડાબું > 0 && c->edge_ગણતરી > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_ગણતરી--;
	c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->કાઢો_transform (c->data);
	તોડો;
      }

      c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ધકેલો_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ધકેલો_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->કાઢો_transform (c->data);
      c->funcs->કાઢો_clip (c->data);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_ગાંઠ_t node (c->glyphs_decycler);
      જો (unlikely (!node.visit (gid)))
	પાછા;

      c->funcs->ધકેલો_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      જો (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->કાઢો_transform (c->data);
	પાછા;
      }
      c->ft_font->lock.lock ();
      c->funcs->કાઢો_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      જો (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_મૂળ_TRANSFORM,
				    &other_paint))
      {
        તાર્કિક has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        જો (has_clip_box)
	{
	  દશાંશ xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ધકેલો_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        જો (has_clip_box)
          c->funcs->કાઢો_clip (c->data);
      }
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ધકેલો_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      દશાંશ dx = paint.u.translate.dx / 65536.f;
      દશાંશ dy = paint.u.translate.dy / 65536.f;

      c->funcs->ધકેલો_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_SCALE:
    {
      દશાંશ dx = paint.u.scale.center_x / 65536.f;
      દશાંશ dy = paint.u.scale.center_y / 65536.f;
      દશાંશ sx = paint.u.scale.scale_x / 65536.f;
      દશાંશ sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ધકેલો_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_ROTATE:
    {
      દશાંશ dx = paint.u.rotate.center_x / 65536.f;
      દશાંશ dy = paint.u.rotate.center_y / 65536.f;
      દશાંશ a = paint.u.rotate.angle / 65536.f;

      c->funcs->ધકેલો_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_SKEW:
    {
      દશાંશ dx = paint.u.skew.center_x / 65536.f;
      દશાંશ dy = paint.u.skew.center_y / 65536.f;
      દશાંશ sx = paint.u.skew.x_skew_angle / 65536.f;
      દશાંશ sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ધકેલો_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->કાઢો_transform (c->data);
    }
    તોડો;
    સ્થિતિ FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ધકેલો_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ધકેલો_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->કાઢો_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->કાઢો_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    તોડો;

    સ્થિતિ FT_COLR_PAINT_FORMAT_MAX: તોડો;
    મૂળભૂત: HB_FALLTHROUGH;
    સ્થિતિ FT_COLR_PAINTFORMAT_UNSUPPORTED: તોડો;
  }
}


સ્થિર તાર્કિક
hb_ft_paint_glyph_colr (hb_font_t *font,
			શૂન્ય *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, શૂન્ય *paint_data,
			unsigned પૂર્ણાંક palette_index,
			hb_color_t foreground,
			શૂન્ય *user_data)
{
  અચલ hb_ft_font_t *ft_font = (અચલ hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = ખાલી;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (શૂન્ય) FT_Palette_Data_Get(ft_face, &palette_data);
  (શૂન્ય) FT_Palette_Select(ft_face, palette_index, &palette);
  જો (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (શૂન્ય) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((અચલ FT_Color *) palette,
				 palette ? palette_data.અંક_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  જો (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_મૂળ_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_ગાંઠ_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    તાર્કિક clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    તાર્કિક is_bounded = clip;
    જો (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_ગાંઠ_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->ધકેલો_font_transform (c.data, font);

    જો (clip)
    {
      દશાંશ xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ધકેલો_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    જો (is_bounded)
      c.recurse (paint);

    જો (clip)
      c.funcs->કાઢો_clip (c.data);

    c.funcs->કાઢો_transform (c.data);

    પાછા સાચું;
  }

  /* COLRv0 */
  iterator.p  = ખાલી;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  જો (palette && have_layers)
  {
    કરો
    {
      hb_bool_t is_foreground = સાચું;
      hb_color_t color = foreground;

      જો ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = ખોટું;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } જ્યારે (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    પાછા સાચું;
  }

  પાછા ખોટું;
}


#endif /* HB_FT_COLR_HH */
