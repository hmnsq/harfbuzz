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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-decycler.hh"
#அந்தர்பாவ "hb-paint-bounded.hh"

#அந்தர்பாவ FT_COLOR_H


நிலையான hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  தேர்வு (mode)
  {
    நிலை FT_COLR_COMPOSITE_CLEAR:          திரும்பு HB_PAINT_COMPOSITE_MODE_CLEAR;
    நிலை FT_COLR_COMPOSITE_SRC:            திரும்பு HB_PAINT_COMPOSITE_MODE_SRC;
    நிலை FT_COLR_COMPOSITE_DEST:           திரும்பு HB_PAINT_COMPOSITE_MODE_DEST;
    நிலை FT_COLR_COMPOSITE_SRC_OVER:       திரும்பு HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    நிலை FT_COLR_COMPOSITE_DEST_OVER:      திரும்பு HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    நிலை FT_COLR_COMPOSITE_SRC_IN:         திரும்பு HB_PAINT_COMPOSITE_MODE_SRC_IN;
    நிலை FT_COLR_COMPOSITE_DEST_IN:        திரும்பு HB_PAINT_COMPOSITE_MODE_DEST_IN;
    நிலை FT_COLR_COMPOSITE_SRC_OUT:        திரும்பு HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    நிலை FT_COLR_COMPOSITE_DEST_OUT:       திரும்பு HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    நிலை FT_COLR_COMPOSITE_SRC_ATOP:       திரும்பு HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    நிலை FT_COLR_COMPOSITE_DEST_ATOP:      திரும்பு HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    நிலை FT_COLR_COMPOSITE_XOR:            திரும்பு HB_PAINT_COMPOSITE_MODE_XOR;
    நிலை FT_COLR_COMPOSITE_PLUS:           திரும்பு HB_PAINT_COMPOSITE_MODE_PLUS;
    நிலை FT_COLR_COMPOSITE_SCREEN:         திரும்பு HB_PAINT_COMPOSITE_MODE_SCREEN;
    நிலை FT_COLR_COMPOSITE_OVERLAY:        திரும்பு HB_PAINT_COMPOSITE_MODE_OVERLAY;
    நிலை FT_COLR_COMPOSITE_DARKEN:         திரும்பு HB_PAINT_COMPOSITE_MODE_DARKEN;
    நிலை FT_COLR_COMPOSITE_LIGHTEN:        திரும்பு HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    நிலை FT_COLR_COMPOSITE_COLOR_DODGE:    திரும்பு HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    நிலை FT_COLR_COMPOSITE_COLOR_BURN:     திரும்பு HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    நிலை FT_COLR_COMPOSITE_HARD_LIGHT:     திரும்பு HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    நிலை FT_COLR_COMPOSITE_SOFT_LIGHT:     திரும்பு HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    நிலை FT_COLR_COMPOSITE_DIFFERENCE:     திரும்பு HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    நிலை FT_COLR_COMPOSITE_EXCLUSION:      திரும்பு HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    நிலை FT_COLR_COMPOSITE_MULTIPLY:       திரும்பு HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    நிலை FT_COLR_COMPOSITE_HSL_HUE:        திரும்பு HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    நிலை FT_COLR_COMPOSITE_HSL_SATURATION: திரும்பு HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    நிலை FT_COLR_COMPOSITE_HSL_COLOR:      திரும்பு HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    நிலை FT_COLR_COMPOSITE_HSL_LUMINOSITY: திரும்பு HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    நிலை FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    இயல்பு:                               திரும்பு HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

வகைவரை கட்டமைப்பு hb_ft_paint_context_t hb_ft_paint_context_t;

நிலையான வெறுமை
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

நிலையான வெறுமை
_hb_ft_unscale_clip_box (hb_font_t *font,
			 மாறிலி FT_ClipBox *clip_box,
			 தசம *xmin, தசம *ymin,
			 தசம *xmax, தசம *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  தசம upem = font->face->get_upem ();
  தசம xscale = upem / (font->x_scale ? font->x_scale : upem);
  தசம yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_இடது.x * xscale;
  *ymin = clip_box->bottom_இடது.y * yscale;
  *xmax = clip_box->உச்சி_வலது.x * xscale;
  *ymax = clip_box->உச்சி_வலது.y * yscale;
}

நிலையான unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  திரும்பு (alpha * alpha_mult + (1 << 13)) >> 14;
}

கட்டமைப்பு hb_ft_paint_context_t
{
  hb_ft_paint_context_t (மாறிலி hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
			 hb_array_t<மாறிலி FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    எனில் (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, உண்மை);
      hb_font_set_synthetic_slant (font, 0);
    }
    இல்லையெனில்
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  வெறுமை recurse (FT_OpaquePaint paint)
  {
    எனில் (unlikely (depth_இடது <= 0 || edge_மொத்தம் <= 0)) திரும்பு;
    depth_இடது--;
    edge_மொத்தம்--;
    _hb_ft_paint (இது, paint);
    depth_இடது++;
  }

  மாறிலி hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  வெறுமை *data;
  hb_array_t<மாறிலி FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  முழு depth_இடது = HB_MAX_NESTING_LEVEL;
  முழு edge_மொத்தம் = HB_MAX_GRAPH_EDGE_மொத்தம்;
};

நிலையான unsigned
hb_ft_color_வரி_get_color_stops (hb_color_வரி_t *color_வரி,
				   வெறுமை *color_வரி_data,
				   unsigned முழு start,
				   unsigned முழு *மொத்தம்,
				   hb_color_stop_t *color_stops,
				   வெறுமை *user_data)
{
  FT_Color_வரி *cl = (FT_Color_வரி *) color_வரி_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  எனில் (மொத்தம் && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    எனில் (start >= cl->color_stop_iterator.இலக்கம்_color_stops)
    {
      *மொத்தம் = 0;
      திரும்பு cl->color_stop_iterator.இலக்கம்_color_stops;
    }

    போது (cl->color_stop_iterator.current_color_stop < start)
    {
      எனில் (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        திரும்பு cl->color_stop_iterator.இலக்கம்_color_stops;
      }
    }

    போது (wrote < *மொத்தம் &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      எனில் (அளவு stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      இல்லையெனில்
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      எனில் (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      இல்லையெனில்
      {
	hb_color_t color;
        எனில் (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	இல்லையெனில் எனில் (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	இல்லையெனில்
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *மொத்தம் = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  திரும்பு cl->color_stop_iterator.இலக்கம்_color_stops;
}

நிலையான hb_paint_extend_t
hb_ft_color_வரி_get_extend (hb_color_வரி_t *color_வரி,
			      வெறுமை *color_வரி_data,
			      வெறுமை *user_data)
{
  FT_Color_வரி *c = (FT_Color_வரி *) color_வரி_data;
  தேர்வு (c->extend)
  {
    இயல்பு:
    நிலை FT_COLR_PAINT_EXTEND_PAD:     திரும்பு HB_PAINT_EXTEND_PAD;
    நிலை FT_COLR_PAINT_EXTEND_REPEAT:  திரும்பு HB_PAINT_EXTEND_REPEAT;
    நிலை FT_COLR_PAINT_EXTEND_REFLECT: திரும்பு HB_PAINT_EXTEND_REFLECT;
  }
}

நிலையான வெறுமை
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			மாறிலி FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  எனில் (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    திரும்பு;
  }

  எனில் (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    திரும்பு;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

வெறுமை
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  எனில் (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    திரும்பு;

  தேர்வு (paint.format)
  {
    நிலை FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_முனை_t node (c->layers_decycler);
      போது (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	எனில் (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  தொடர்;

	c->recurse (other_paint);
      }
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_வரி_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_வரி_get_color_stops, c,
	hb_ft_color_வரி_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_வரி_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_வரி_get_color_stops, c,
	hb_ft_color_வரி_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_வரி_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_வரி_get_color_stops, c,
	hb_ft_color_வரி_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      எனில் (likely (c->depth_இடது > 0 && c->edge_மொத்தம் > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_மொத்தம்--;
	c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->எடு_transform (c->data);
	நிறுத்து;
      }

      c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->தள்ளு_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->தள்ளு_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->எடு_transform (c->data);
      c->funcs->எடு_clip (c->data);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_முனை_t node (c->glyphs_decycler);
      எனில் (unlikely (!node.visit (gid)))
	திரும்பு;

      c->funcs->தள்ளு_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      எனில் (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->எடு_transform (c->data);
	திரும்பு;
      }
      c->ft_font->lock.lock ();
      c->funcs->எடு_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      எனில் (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_வேர்_TRANSFORM,
				    &other_paint))
      {
        தருக்க has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        எனில் (has_clip_box)
	{
	  தசம xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->தள்ளு_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        எனில் (has_clip_box)
          c->funcs->எடு_clip (c->data);
      }
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->தள்ளு_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      தசம dx = paint.u.translate.dx / 65536.f;
      தசம dy = paint.u.translate.dy / 65536.f;

      c->funcs->தள்ளு_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_SCALE:
    {
      தசம dx = paint.u.scale.center_x / 65536.f;
      தசம dy = paint.u.scale.center_y / 65536.f;
      தசம sx = paint.u.scale.scale_x / 65536.f;
      தசம sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->தள்ளு_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_ROTATE:
    {
      தசம dx = paint.u.rotate.center_x / 65536.f;
      தசம dy = paint.u.rotate.center_y / 65536.f;
      தசம a = paint.u.rotate.angle / 65536.f;

      c->funcs->தள்ளு_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_SKEW:
    {
      தசம dx = paint.u.skew.center_x / 65536.f;
      தசம dy = paint.u.skew.center_y / 65536.f;
      தசம sx = paint.u.skew.x_skew_angle / 65536.f;
      தசம sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->தள்ளு_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->எடு_transform (c->data);
    }
    நிறுத்து;
    நிலை FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->தள்ளு_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->தள்ளு_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->எடு_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->எடு_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    நிறுத்து;

    நிலை FT_COLR_PAINT_FORMAT_MAX: நிறுத்து;
    இயல்பு: HB_FALLTHROUGH;
    நிலை FT_COLR_PAINTFORMAT_UNSUPPORTED: நிறுத்து;
  }
}


நிலையான தருக்க
hb_ft_paint_glyph_colr (hb_font_t *font,
			வெறுமை *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, வெறுமை *paint_data,
			unsigned முழு palette_index,
			hb_color_t foreground,
			வெறுமை *user_data)
{
  மாறிலி hb_ft_font_t *ft_font = (மாறிலி hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = வெற்று;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (வெறுமை) FT_Palette_Data_Get(ft_face, &palette_data);
  (வெறுமை) FT_Palette_Select(ft_face, palette_index, &palette);
  எனில் (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (வெறுமை) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((மாறிலி FT_Color *) palette,
				 palette ? palette_data.இலக்கம்_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  எனில் (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_வேர்_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_முனை_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    தருக்க clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    தருக்க is_bounded = clip;
    எனில் (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_முனை_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->தள்ளு_font_transform (c.data, font);

    எனில் (clip)
    {
      தசம xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->தள்ளு_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    எனில் (is_bounded)
      c.recurse (paint);

    எனில் (clip)
      c.funcs->எடு_clip (c.data);

    c.funcs->எடு_transform (c.data);

    திரும்பு உண்மை;
  }

  /* COLRv0 */
  iterator.p  = வெற்று;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  எனில் (palette && have_layers)
  {
    செய்
    {
      hb_bool_t is_foreground = உண்மை;
      hb_color_t color = foreground;

      எனில் ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = பொய்;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } போது (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    திரும்பு உண்மை;
  }

  திரும்பு பொய்;
}


#endif /* HB_FT_COLR_HH */
