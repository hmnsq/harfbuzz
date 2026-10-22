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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-decycler.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint-bounded.hh"

#ଅନ୍ତର୍ଭାଵ FT_COLOR_H


ସ୍ଥିର hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  ବାଛ (mode)
  {
    ସ୍ଥିତି FT_COLR_COMPOSITE_CLEAR:          ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_CLEAR;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SRC:            ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SRC;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DEST:           ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DEST;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SRC_OVER:       ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DEST_OVER:      ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SRC_IN:         ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SRC_IN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DEST_IN:        ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DEST_IN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SRC_OUT:        ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DEST_OUT:       ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SRC_ATOP:       ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DEST_ATOP:      ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    ସ୍ଥିତି FT_COLR_COMPOSITE_XOR:            ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_XOR;
    ସ୍ଥିତି FT_COLR_COMPOSITE_PLUS:           ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_PLUS;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SCREEN:         ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SCREEN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_OVERLAY:        ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_OVERLAY;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DARKEN:         ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DARKEN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_LIGHTEN:        ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_COLOR_DODGE:    ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    ସ୍ଥିତି FT_COLR_COMPOSITE_COLOR_BURN:     ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    ସ୍ଥିତି FT_COLR_COMPOSITE_HARD_LIGHT:     ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    ସ୍ଥିତି FT_COLR_COMPOSITE_SOFT_LIGHT:     ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    ସ୍ଥିତି FT_COLR_COMPOSITE_DIFFERENCE:     ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    ସ୍ଥିତି FT_COLR_COMPOSITE_EXCLUSION:      ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    ସ୍ଥିତି FT_COLR_COMPOSITE_MULTIPLY:       ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    ସ୍ଥିତି FT_COLR_COMPOSITE_HSL_HUE:        ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    ସ୍ଥିତି FT_COLR_COMPOSITE_HSL_SATURATION: ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    ସ୍ଥିତି FT_COLR_COMPOSITE_HSL_COLOR:      ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    ସ୍ଥିତି FT_COLR_COMPOSITE_HSL_LUMINOSITY: ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    ସ୍ଥିତି FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:                               ଫେରନ୍ତୁ HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା hb_ft_paint_context_t hb_ft_paint_context_t;

ସ୍ଥିର ଶୂନ୍ୟ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ସ୍ଥିର ଶୂନ୍ୟ
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ଅଚଳ FT_ClipBox *clip_box,
			 ଦଶମିକ *xmin, ଦଶମିକ *ymin,
			 ଦଶମିକ *xmax, ଦଶମିକ *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  ଦଶମିକ upem = font->face->get_upem ();
  ଦଶମିକ xscale = upem / (font->x_scale ? font->x_scale : upem);
  ଦଶମିକ yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ବାମ.x * xscale;
  *ymin = clip_box->bottom_ବାମ.y * yscale;
  *xmax = clip_box->ଶୀର୍ଷ_ଡାହାଣ.x * xscale;
  *ymax = clip_box->ଶୀର୍ଷ_ଡାହାଣ.y * yscale;
}

ସ୍ଥିର unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  ଫେରନ୍ତୁ (alpha * alpha_mult + (1 << 13)) >> 14;
}

ସଂରଚନା hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ଅଚଳ hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, ଶୂନ୍ୟ *paint_data,
			 hb_array_t<ଅଚଳ FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    ଯଦି (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, ସତ୍ୟ);
      hb_font_set_synthetic_slant (font, 0);
    }
    ଅନ୍ୟଥା
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ଶୂନ୍ୟ recurse (FT_OpaquePaint paint)
  {
    ଯଦି (unlikely (depth_ବାମ <= 0 || edge_ଗଣନା <= 0)) ଫେରନ୍ତୁ;
    depth_ବାମ--;
    edge_ଗଣନା--;
    _hb_ft_paint (ଏହି, paint);
    depth_ବାମ++;
  }

  ଅଚଳ hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  ଶୂନ୍ୟ *data;
  hb_array_t<ଅଚଳ FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth_ବାମ = HB_MAX_NESTING_LEVEL;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା edge_ଗଣନା = HB_MAX_GRAPH_EDGE_ଗଣନା;
};

ସ୍ଥିର unsigned
hb_ft_color_ଧାଡ଼ି_get_color_stops (hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				   ଶୂନ୍ୟ *color_ଧାଡ଼ି_data,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2,
				   hb_color_stop_t *color_stops,
				   ଶୂନ୍ୟ *user_data)
{
  FT_Color_ଧାଡ଼ି *cl = (FT_Color_ଧାଡ଼ି *) color_ଧାଡ଼ି_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  ଯଦି (ଗଣନା_2 && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    ଯଦି (start >= cl->color_stop_iterator.ଅଙ୍କ_color_stops)
    {
      *ଗଣନା_2 = 0;
      ଫେରନ୍ତୁ cl->color_stop_iterator.ଅଙ୍କ_color_stops;
    }

    ଯେତେବେଳେ (cl->color_stop_iterator.current_color_stop < start)
    {
      ଯଦି (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        ଫେରନ୍ତୁ cl->color_stop_iterator.ଅଙ୍କ_color_stops;
      }
    }

    ଯେତେବେଳେ (wrote < *ଗଣନା_2 &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      ଯଦି (ଆକାର stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      ଅନ୍ୟଥା
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      ଯଦି (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      ଅନ୍ୟଥା
      {
	hb_color_t color;
        ଯଦି (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	ଅନ୍ୟଥା ଯଦି (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	ଅନ୍ୟଥା
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *ଗଣନା_2 = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  ଫେରନ୍ତୁ cl->color_stop_iterator.ଅଙ୍କ_color_stops;
}

ସ୍ଥିର hb_paint_extend_t
hb_ft_color_ଧାଡ଼ି_get_extend (hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
			      ଶୂନ୍ୟ *color_ଧାଡ଼ି_data,
			      ଶୂନ୍ୟ *user_data)
{
  FT_Color_ଧାଡ଼ି *c = (FT_Color_ଧାଡ଼ି *) color_ଧାଡ଼ି_data;
  ବାଛ (c->extend)
  {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ସ୍ଥିତି FT_COLR_PAINT_EXTEND_PAD:     ଫେରନ୍ତୁ HB_PAINT_EXTEND_PAD;
    ସ୍ଥିତି FT_COLR_PAINT_EXTEND_REPEAT:  ଫେରନ୍ତୁ HB_PAINT_EXTEND_REPEAT;
    ସ୍ଥିତି FT_COLR_PAINT_EXTEND_REFLECT: ଫେରନ୍ତୁ HB_PAINT_EXTEND_REFLECT;
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ଅଚଳ FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  ଯଦି (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    ଫେରନ୍ତୁ;
  }

  ଯଦି (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    ଫେରନ୍ତୁ;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

ଶୂନ୍ୟ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  ଯଦି (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    ଫେରନ୍ତୁ;

  ବାଛ (paint.format)
  {
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_ଗଣ୍ଠି_t node (c->layers_decycler);
      ଯେତେବେଳେ (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	ଯଦି (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  ଜାରି;

	c->recurse (other_paint);
      }
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_ଧାଡ଼ି_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ଧାଡ଼ି_get_color_stops, c,
	hb_ft_color_ଧାଡ଼ି_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_ଧାଡ଼ି_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ଧାଡ଼ି_get_color_stops, c,
	hb_ft_color_ଧାଡ଼ି_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_ଧାଡ଼ି_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_ଧାଡ଼ି_get_color_stops, c,
	hb_ft_color_ଧାଡ଼ି_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      ଯଦି (likely (c->depth_ବାମ > 0 && c->edge_ଗଣନା > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_ଗଣନା--;
	c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->କାଢ଼_transform (c->data);
	ଭାଙ୍ଗ;
      }

      c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ଠେଲ_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ଠେଲ_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->କାଢ଼_transform (c->data);
      c->funcs->କାଢ଼_clip (c->data);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_ଗଣ୍ଠି_t node (c->glyphs_decycler);
      ଯଦି (unlikely (!node.visit (gid)))
	ଫେରନ୍ତୁ;

      c->funcs->ଠେଲ_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      ଯଦି (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->କାଢ଼_transform (c->data);
	ଫେରନ୍ତୁ;
      }
      c->ft_font->lock.lock ();
      c->funcs->କାଢ଼_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      ଯଦି (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_ମୂଳ_TRANSFORM,
				    &other_paint))
      {
        ତାର୍କିକ has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        ଯଦି (has_clip_box)
	{
	  ଦଶମିକ xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ଠେଲ_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        ଯଦି (has_clip_box)
          c->funcs->କାଢ଼_clip (c->data);
      }
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ଠେଲ_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      ଦଶମିକ dx = paint.u.translate.dx / 65536.f;
      ଦଶମିକ dy = paint.u.translate.dy / 65536.f;

      c->funcs->ଠେଲ_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_SCALE:
    {
      ଦଶମିକ dx = paint.u.scale.center_x / 65536.f;
      ଦଶମିକ dy = paint.u.scale.center_y / 65536.f;
      ଦଶମିକ sx = paint.u.scale.scale_x / 65536.f;
      ଦଶମିକ sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ଠେଲ_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_ROTATE:
    {
      ଦଶମିକ dx = paint.u.rotate.center_x / 65536.f;
      ଦଶମିକ dy = paint.u.rotate.center_y / 65536.f;
      ଦଶମିକ a = paint.u.rotate.angle / 65536.f;

      c->funcs->ଠେଲ_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_SKEW:
    {
      ଦଶମିକ dx = paint.u.skew.center_x / 65536.f;
      ଦଶମିକ dy = paint.u.skew.center_y / 65536.f;
      ଦଶମିକ sx = paint.u.skew.x_skew_angle / 65536.f;
      ଦଶମିକ sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ଠେଲ_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->କାଢ଼_transform (c->data);
    }
    ଭାଙ୍ଗ;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ଠେଲ_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ଠେଲ_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->କାଢ଼_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->କାଢ଼_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    ଭାଙ୍ଗ;

    ସ୍ଥିତି FT_COLR_PAINT_FORMAT_MAX: ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: HB_FALLTHROUGH;
    ସ୍ଥିତି FT_COLR_PAINTFORMAT_UNSUPPORTED: ଭାଙ୍ଗ;
  }
}


ସ୍ଥିର ତାର୍କିକ
hb_ft_paint_glyph_colr (hb_font_t *font,
			ଶୂନ୍ୟ *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, ଶୂନ୍ୟ *paint_data,
			unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette_index,
			hb_color_t foreground,
			ଶୂନ୍ୟ *user_data)
{
  ଅଚଳ hb_ft_font_t *ft_font = (ଅଚଳ hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = ଖାଲି;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (ଶୂନ୍ୟ) FT_Palette_Data_Get(ft_face, &palette_data);
  (ଶୂନ୍ୟ) FT_Palette_Select(ft_face, palette_index, &palette);
  ଯଦି (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (ଶୂନ୍ୟ) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ଅଚଳ FT_Color *) palette,
				 palette ? palette_data.ଅଙ୍କ_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ଯଦି (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ମୂଳ_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_ଗଣ୍ଠି_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    ତାର୍କିକ clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    ତାର୍କିକ is_bounded = clip;
    ଯଦି (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_ଗଣ୍ଠି_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->ଠେଲ_font_transform (c.data, font);

    ଯଦି (clip)
    {
      ଦଶମିକ xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ଠେଲ_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    ଯଦି (is_bounded)
      c.recurse (paint);

    ଯଦି (clip)
      c.funcs->କାଢ଼_clip (c.data);

    c.funcs->କାଢ଼_transform (c.data);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* COLRv0 */
  iterator.p  = ଖାଲି;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  ଯଦି (palette && have_layers)
  {
    କର
    {
      hb_bool_t is_foreground = ସତ୍ୟ;
      hb_color_t color = foreground;

      ଯଦି ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = ମିଥ୍ୟା;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } ଯେତେବେଳେ (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}


#endif /* HB_FT_COLR_HH */
