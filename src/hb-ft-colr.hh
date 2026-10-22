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

#ضمن "hb.hh"

#ضمن "hb-decycler.hh"
#ضمن "hb-paint-bounded.hh"

#ضمن FT_COLOR_H


ساكن hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  بدل (mode)
  {
    حالة FT_COLR_COMPOSITE_CLEAR:          أرجع HB_PAINT_COMPOSITE_MODE_CLEAR;
    حالة FT_COLR_COMPOSITE_SRC:            أرجع HB_PAINT_COMPOSITE_MODE_SRC;
    حالة FT_COLR_COMPOSITE_DEST:           أرجع HB_PAINT_COMPOSITE_MODE_DEST;
    حالة FT_COLR_COMPOSITE_SRC_OVER:       أرجع HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    حالة FT_COLR_COMPOSITE_DEST_OVER:      أرجع HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    حالة FT_COLR_COMPOSITE_SRC_IN:         أرجع HB_PAINT_COMPOSITE_MODE_SRC_IN;
    حالة FT_COLR_COMPOSITE_DEST_IN:        أرجع HB_PAINT_COMPOSITE_MODE_DEST_IN;
    حالة FT_COLR_COMPOSITE_SRC_OUT:        أرجع HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    حالة FT_COLR_COMPOSITE_DEST_OUT:       أرجع HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    حالة FT_COLR_COMPOSITE_SRC_ATOP:       أرجع HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    حالة FT_COLR_COMPOSITE_DEST_ATOP:      أرجع HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    حالة FT_COLR_COMPOSITE_XOR:            أرجع HB_PAINT_COMPOSITE_MODE_XOR;
    حالة FT_COLR_COMPOSITE_PLUS:           أرجع HB_PAINT_COMPOSITE_MODE_PLUS;
    حالة FT_COLR_COMPOSITE_SCREEN:         أرجع HB_PAINT_COMPOSITE_MODE_SCREEN;
    حالة FT_COLR_COMPOSITE_OVERLAY:        أرجع HB_PAINT_COMPOSITE_MODE_OVERLAY;
    حالة FT_COLR_COMPOSITE_DARKEN:         أرجع HB_PAINT_COMPOSITE_MODE_DARKEN;
    حالة FT_COLR_COMPOSITE_LIGHTEN:        أرجع HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    حالة FT_COLR_COMPOSITE_COLOR_DODGE:    أرجع HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    حالة FT_COLR_COMPOSITE_COLOR_BURN:     أرجع HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    حالة FT_COLR_COMPOSITE_HARD_LIGHT:     أرجع HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    حالة FT_COLR_COMPOSITE_SOFT_LIGHT:     أرجع HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    حالة FT_COLR_COMPOSITE_DIFFERENCE:     أرجع HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    حالة FT_COLR_COMPOSITE_EXCLUSION:      أرجع HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    حالة FT_COLR_COMPOSITE_MULTIPLY:       أرجع HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    حالة FT_COLR_COMPOSITE_HSL_HUE:        أرجع HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    حالة FT_COLR_COMPOSITE_HSL_SATURATION: أرجع HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    حالة FT_COLR_COMPOSITE_HSL_COLOR:      أرجع HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    حالة FT_COLR_COMPOSITE_HSL_LUMINOSITY: أرجع HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    حالة FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    افتراضي:                               أرجع HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

تعريفنوع بنية hb_ft_paint_context_t hb_ft_paint_context_t;

ساكن فراغ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ساكن فراغ
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ثابت FT_ClipBox *clip_box,
			 عائم *xmin, عائم *ymin,
			 عائم *xmax, عائم *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  عائم upem = font->face->get_upem ();
  عائم xscale = upem / (font->x_scale ? font->x_scale : upem);
  عائم yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_بایاں.x * xscale;
  *ymin = clip_box->bottom_بایاں.y * yscale;
  *xmax = clip_box->چوٹی_دایاں.x * xscale;
  *ymax = clip_box->چوٹی_دایاں.y * yscale;
}

ساكن unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  أرجع (alpha * alpha_mult + (1 << 13)) >> 14;
}

بنية hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ثابت hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, فراغ *paint_data,
			 hb_array_t<ثابت FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    إذا (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, true);
      hb_font_set_synthetic_slant (font, 0);
    }
    وإلا
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  فراغ recurse (FT_OpaquePaint paint)
  {
    إذا (unlikely (depth_بایاں <= 0 || edge_گنتی <= 0)) أرجع;
    depth_بایاں--;
    edge_گنتی--;
    _hb_ft_paint (هذا, paint);
    depth_بایاں++;
  }

  ثابت hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  فراغ *data;
  hb_array_t<ثابت FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحيح depth_بایاں = HB_MAX_NESTING_LEVEL;
  صحيح edge_گنتی = HB_MAX_GRAPH_EDGE_گنتی;
};

ساكن unsigned
hb_ft_color_سطر_get_color_stops (hb_color_سطر_t *color_سطر,
				   فراغ *color_سطر_data,
				   unsigned صحيح start,
				   unsigned صحيح *گنتی,
				   hb_color_stop_t *color_stops,
				   فراغ *user_data)
{
  FT_Color_سطر *cl = (FT_Color_سطر *) color_سطر_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  إذا (گنتی && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    إذا (start >= cl->color_stop_iterator.عدد_color_stops)
    {
      *گنتی = 0;
      أرجع cl->color_stop_iterator.عدد_color_stops;
    }

    طالما (cl->color_stop_iterator.current_color_stop < start)
    {
      إذا (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        أرجع cl->color_stop_iterator.عدد_color_stops;
      }
    }

    طالما (wrote < *گنتی &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      إذا (حجم stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      وإلا
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      إذا (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      وإلا
      {
	hb_color_t color;
        إذا (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	وإلا إذا (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	وإلا
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *گنتی = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  أرجع cl->color_stop_iterator.عدد_color_stops;
}

ساكن hb_paint_extend_t
hb_ft_color_سطر_get_extend (hb_color_سطر_t *color_سطر,
			      فراغ *color_سطر_data,
			      فراغ *user_data)
{
  FT_Color_سطر *c = (FT_Color_سطر *) color_سطر_data;
  بدل (c->extend)
  {
    افتراضي:
    حالة FT_COLR_PAINT_EXTEND_PAD:     أرجع HB_PAINT_EXTEND_PAD;
    حالة FT_COLR_PAINT_EXTEND_REPEAT:  أرجع HB_PAINT_EXTEND_REPEAT;
    حالة FT_COLR_PAINT_EXTEND_REFLECT: أرجع HB_PAINT_EXTEND_REFLECT;
  }
}

ساكن فراغ
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ثابت FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  إذا (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    أرجع;
  }

  إذا (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    أرجع;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

فراغ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  إذا (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    أرجع;

  بدل (paint.format)
  {
    حالة FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_گرہ_t node (c->layers_decycler);
      طالما (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	إذا (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  استمر;

	c->recurse (other_paint);
      }
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      إذا (likely (c->depth_بایاں > 0 && c->edge_گنتی > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_گنتی--;
	c->funcs->ڈالو_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->نکالو_transform (c->data);
	اكسر;
      }

      c->funcs->ڈالو_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ڈالو_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ڈالو_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->نکالو_transform (c->data);
      c->funcs->نکالو_clip (c->data);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_گرہ_t node (c->glyphs_decycler);
      إذا (unlikely (!node.visit (gid)))
	أرجع;

      c->funcs->ڈالو_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      إذا (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->نکالو_transform (c->data);
	أرجع;
      }
      c->ft_font->lock.lock ();
      c->funcs->نکالو_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      إذا (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_جڑ_TRANSFORM,
				    &other_paint))
      {
        منطقي has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        إذا (has_clip_box)
	{
	  عائم xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ڈالو_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        إذا (has_clip_box)
          c->funcs->نکالو_clip (c->data);
      }
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ڈالو_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      عائم dx = paint.u.translate.dx / 65536.f;
      عائم dy = paint.u.translate.dy / 65536.f;

      c->funcs->ڈالو_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_SCALE:
    {
      عائم dx = paint.u.scale.center_x / 65536.f;
      عائم dy = paint.u.scale.center_y / 65536.f;
      عائم sx = paint.u.scale.scale_x / 65536.f;
      عائم sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ڈالو_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_ROTATE:
    {
      عائم dx = paint.u.rotate.center_x / 65536.f;
      عائم dy = paint.u.rotate.center_y / 65536.f;
      عائم a = paint.u.rotate.angle / 65536.f;

      c->funcs->ڈالو_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_SKEW:
    {
      عائم dx = paint.u.skew.center_x / 65536.f;
      عائم dy = paint.u.skew.center_y / 65536.f;
      عائم sx = paint.u.skew.x_skew_angle / 65536.f;
      عائم sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ڈالو_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->نکالو_transform (c->data);
    }
    اكسر;
    حالة FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ڈالو_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ڈالو_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->نکالو_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->نکالو_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    اكسر;

    حالة FT_COLR_PAINT_FORMAT_MAX: اكسر;
    افتراضي: HB_FALLTHROUGH;
    حالة FT_COLR_PAINTFORMAT_UNSUPPORTED: اكسر;
  }
}


ساكن منطقي
hb_ft_paint_glyph_colr (hb_font_t *font,
			فراغ *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, فراغ *paint_data,
			unsigned صحيح palette_index,
			hb_color_t foreground,
			فراغ *user_data)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = عدم;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (فراغ) FT_Palette_Data_Get(ft_face, &palette_data);
  (فراغ) FT_Palette_Select(ft_face, palette_index, &palette);
  إذا (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (فراغ) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ثابت FT_Color *) palette,
				 palette ? palette_data.عدد_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  إذا (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_جڑ_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_گرہ_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    منطقي clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    منطقي is_bounded = clip;
    إذا (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_گرہ_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->ڈالو_font_transform (c.data, font);

    إذا (clip)
    {
      عائم xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ڈالو_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    إذا (is_bounded)
      c.recurse (paint);

    إذا (clip)
      c.funcs->نکالو_clip (c.data);

    c.funcs->نکالو_transform (c.data);

    أرجع true;
  }

  /* COLRv0 */
  iterator.p  = عدم;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  إذا (palette && have_layers)
  {
    افعل
    {
      hb_bool_t is_foreground = true;
      hb_color_t color = foreground;

      إذا ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = خطأ;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } طالما (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    أرجع true;
  }

  أرجع خطأ;
}


#endif /* HB_FT_COLR_HH */
