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

#شامل "hb.hh"

#شامل "hb-decycler.hh"
#شامل "hb-paint-bounded.hh"

#شامل FT_COLOR_H


ایستا hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  انتخاب (mode)
  {
    حالت FT_COLR_COMPOSITE_CLEAR:          بازگشت HB_PAINT_COMPOSITE_MODE_CLEAR;
    حالت FT_COLR_COMPOSITE_SRC:            بازگشت HB_PAINT_COMPOSITE_MODE_SRC;
    حالت FT_COLR_COMPOSITE_DEST:           بازگشت HB_PAINT_COMPOSITE_MODE_DEST;
    حالت FT_COLR_COMPOSITE_SRC_OVER:       بازگشت HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    حالت FT_COLR_COMPOSITE_DEST_OVER:      بازگشت HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    حالت FT_COLR_COMPOSITE_SRC_IN:         بازگشت HB_PAINT_COMPOSITE_MODE_SRC_IN;
    حالت FT_COLR_COMPOSITE_DEST_IN:        بازگشت HB_PAINT_COMPOSITE_MODE_DEST_IN;
    حالت FT_COLR_COMPOSITE_SRC_OUT:        بازگشت HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    حالت FT_COLR_COMPOSITE_DEST_OUT:       بازگشت HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    حالت FT_COLR_COMPOSITE_SRC_ATOP:       بازگشت HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    حالت FT_COLR_COMPOSITE_DEST_ATOP:      بازگشت HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    حالت FT_COLR_COMPOSITE_XOR:            بازگشت HB_PAINT_COMPOSITE_MODE_XOR;
    حالت FT_COLR_COMPOSITE_PLUS:           بازگشت HB_PAINT_COMPOSITE_MODE_PLUS;
    حالت FT_COLR_COMPOSITE_SCREEN:         بازگشت HB_PAINT_COMPOSITE_MODE_SCREEN;
    حالت FT_COLR_COMPOSITE_OVERLAY:        بازگشت HB_PAINT_COMPOSITE_MODE_OVERLAY;
    حالت FT_COLR_COMPOSITE_DARKEN:         بازگشت HB_PAINT_COMPOSITE_MODE_DARKEN;
    حالت FT_COLR_COMPOSITE_LIGHTEN:        بازگشت HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    حالت FT_COLR_COMPOSITE_COLOR_DODGE:    بازگشت HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    حالت FT_COLR_COMPOSITE_COLOR_BURN:     بازگشت HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    حالت FT_COLR_COMPOSITE_HARD_LIGHT:     بازگشت HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    حالت FT_COLR_COMPOSITE_SOFT_LIGHT:     بازگشت HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    حالت FT_COLR_COMPOSITE_DIFFERENCE:     بازگشت HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    حالت FT_COLR_COMPOSITE_EXCLUSION:      بازگشت HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    حالت FT_COLR_COMPOSITE_MULTIPLY:       بازگشت HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    حالت FT_COLR_COMPOSITE_HSL_HUE:        بازگشت HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    حالت FT_COLR_COMPOSITE_HSL_SATURATION: بازگشت HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    حالت FT_COLR_COMPOSITE_HSL_COLOR:      بازگشت HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    حالت FT_COLR_COMPOSITE_HSL_LUMINOSITY: بازگشت HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    حالت FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    پیشفرض:                               بازگشت HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

تعریفنوع ساختار hb_ft_paint_context_t hb_ft_paint_context_t;

ایستا تهی
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ایستا تهی
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ثابت FT_ClipBox *clip_box,
			 اعشاری *xmin, اعشاری *ymin,
			 اعشاری *xmax, اعشاری *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  اعشاری upem = font->face->get_upem ();
  اعشاری xscale = upem / (font->x_scale ? font->x_scale : upem);
  اعشاری yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_چپ.x * xscale;
  *ymin = clip_box->bottom_چپ.y * yscale;
  *xmax = clip_box->بالاترین_راست.x * xscale;
  *ymax = clip_box->بالاترین_راست.y * yscale;
}

ایستا unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  بازگشت (alpha * alpha_mult + (1 << 13)) >> 14;
}

ساختار hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ثابت hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, تهی *paint_data,
			 hb_array_t<ثابت FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    اگر (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, درست);
      hb_font_set_synthetic_slant (font, 0);
    }
    وگرنه
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  تهی recurse (FT_OpaquePaint paint)
  {
    اگر (unlikely (depth_چپ <= 0 || edge_شمار <= 0)) بازگشت;
    depth_چپ--;
    edge_شمار--;
    _hb_ft_paint (این, paint);
    depth_چپ++;
  }

  ثابت hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  تهی *data;
  hb_array_t<ثابت FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحیح depth_چپ = HB_MAX_NESTING_LEVEL;
  صحیح edge_شمار = HB_MAX_GRAPH_EDGE_شمار;
};

ایستا unsigned
hb_ft_color_سطر_get_color_stops (hb_color_سطر_t *color_سطر,
				   تهی *color_سطر_data,
				   unsigned صحیح start,
				   unsigned صحیح *شمار,
				   hb_color_stop_t *color_stops,
				   تهی *user_data)
{
  FT_Color_سطر *cl = (FT_Color_سطر *) color_سطر_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  اگر (شمار && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    اگر (start >= cl->color_stop_iterator.رقم_color_stops)
    {
      *شمار = 0;
      بازگشت cl->color_stop_iterator.رقم_color_stops;
    }

    وقتی (cl->color_stop_iterator.current_color_stop < start)
    {
      اگر (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        بازگشت cl->color_stop_iterator.رقم_color_stops;
      }
    }

    وقتی (wrote < *شمار &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      اگر (اندازه stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      وگرنه
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      اگر (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      وگرنه
      {
	hb_color_t color;
        اگر (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	وگرنه اگر (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	وگرنه
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *شمار = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  بازگشت cl->color_stop_iterator.رقم_color_stops;
}

ایستا hb_paint_extend_t
hb_ft_color_سطر_get_extend (hb_color_سطر_t *color_سطر,
			      تهی *color_سطر_data,
			      تهی *user_data)
{
  FT_Color_سطر *c = (FT_Color_سطر *) color_سطر_data;
  انتخاب (c->extend)
  {
    پیشفرض:
    حالت FT_COLR_PAINT_EXTEND_PAD:     بازگشت HB_PAINT_EXTEND_PAD;
    حالت FT_COLR_PAINT_EXTEND_REPEAT:  بازگشت HB_PAINT_EXTEND_REPEAT;
    حالت FT_COLR_PAINT_EXTEND_REFLECT: بازگشت HB_PAINT_EXTEND_REFLECT;
  }
}

ایستا تهی
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ثابت FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  اگر (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    بازگشت;
  }

  اگر (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    بازگشت;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

تهی
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  اگر (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    بازگشت;

  انتخاب (paint.format)
  {
    حالت FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_گره_t node (c->layers_decycler);
      وقتی (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	اگر (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  ادامه;

	c->recurse (other_paint);
      }
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
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
    بشکن;
    حالت FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
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
    بشکن;
    حالت FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
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
    بشکن;
    حالت FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      اگر (likely (c->depth_چپ > 0 && c->edge_شمار > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_شمار--;
	c->funcs->افزودن_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->برداشت_transform (c->data);
	بشکن;
      }

      c->funcs->افزودن_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->افزودن_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->افزودن_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->برداشت_transform (c->data);
      c->funcs->برداشت_clip (c->data);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_گره_t node (c->glyphs_decycler);
      اگر (unlikely (!node.visit (gid)))
	بازگشت;

      c->funcs->افزودن_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      اگر (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->برداشت_transform (c->data);
	بازگشت;
      }
      c->ft_font->lock.lock ();
      c->funcs->برداشت_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      اگر (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_ریشه_TRANSFORM,
				    &other_paint))
      {
        منطقی has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        اگر (has_clip_box)
	{
	  اعشاری xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->افزودن_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        اگر (has_clip_box)
          c->funcs->برداشت_clip (c->data);
      }
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->افزودن_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      اعشاری dx = paint.u.translate.dx / 65536.f;
      اعشاری dy = paint.u.translate.dy / 65536.f;

      c->funcs->افزودن_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_SCALE:
    {
      اعشاری dx = paint.u.scale.center_x / 65536.f;
      اعشاری dy = paint.u.scale.center_y / 65536.f;
      اعشاری sx = paint.u.scale.scale_x / 65536.f;
      اعشاری sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->افزودن_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_ROTATE:
    {
      اعشاری dx = paint.u.rotate.center_x / 65536.f;
      اعشاری dy = paint.u.rotate.center_y / 65536.f;
      اعشاری a = paint.u.rotate.angle / 65536.f;

      c->funcs->افزودن_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_SKEW:
    {
      اعشاری dx = paint.u.skew.center_x / 65536.f;
      اعشاری dy = paint.u.skew.center_y / 65536.f;
      اعشاری sx = paint.u.skew.x_skew_angle / 65536.f;
      اعشاری sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->افزودن_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->برداشت_transform (c->data);
    }
    بشکن;
    حالت FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->افزودن_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->افزودن_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->برداشت_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->برداشت_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    بشکن;

    حالت FT_COLR_PAINT_FORMAT_MAX: بشکن;
    پیشفرض: HB_FALLTHROUGH;
    حالت FT_COLR_PAINTFORMAT_UNSUPPORTED: بشکن;
  }
}


ایستا منطقی
hb_ft_paint_glyph_colr (hb_font_t *font,
			تهی *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, تهی *paint_data,
			unsigned صحیح palette_index,
			hb_color_t foreground,
			تهی *user_data)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = پوچ;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (تهی) FT_Palette_Data_Get(ft_face, &palette_data);
  (تهی) FT_Palette_Select(ft_face, palette_index, &palette);
  اگر (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (تهی) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ثابت FT_Color *) palette,
				 palette ? palette_data.رقم_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  اگر (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ریشه_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_گره_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    منطقی clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    منطقی is_bounded = clip;
    اگر (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_گره_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->افزودن_font_transform (c.data, font);

    اگر (clip)
    {
      اعشاری xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->افزودن_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    اگر (is_bounded)
      c.recurse (paint);

    اگر (clip)
      c.funcs->برداشت_clip (c.data);

    c.funcs->برداشت_transform (c.data);

    بازگشت درست;
  }

  /* COLRv0 */
  iterator.p  = پوچ;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  اگر (palette && have_layers)
  {
    انجامبده
    {
      hb_bool_t is_foreground = درست;
      hb_color_t color = foreground;

      اگر ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = نادرست;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } وقتی (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    بازگشت درست;
  }

  بازگشت نادرست;
}


#endif /* HB_FT_COLR_HH */
