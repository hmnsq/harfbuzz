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

#اگر_غیر_تعریف_شدہ HB_FT_COLR_HH
#تعریف_کرو HB_FT_COLR_HH

#شامل "hb.hh"

#شامل "hb-decycler.hh"
#شامل "hb-paint-bounded.hh"

#شامل FT_COLOR_H


ساکن hb_paint_مرکب_منوال_t
hb_ft_paint_مرکب_منوال (FT_مرکب_منوال منوال)
{
  انتخاب (منوال)
  {
    حالت FT_COLR_مرکب_CLEAR:          واپس HB_PAINT_مرکب_منوال_CLEAR;
    حالت FT_COLR_مرکب_SRC:            واپس HB_PAINT_مرکب_منوال_SRC;
    حالت FT_COLR_مرکب_DEST:           واپس HB_PAINT_مرکب_منوال_DEST;
    حالت FT_COLR_مرکب_SRC_OVER:       واپس HB_PAINT_مرکب_منوال_SRC_OVER;
    حالت FT_COLR_مرکب_DEST_OVER:      واپس HB_PAINT_مرکب_منوال_DEST_OVER;
    حالت FT_COLR_مرکب_SRC_اندر:         واپس HB_PAINT_مرکب_منوال_SRC_اندر;
    حالت FT_COLR_مرکب_DEST_اندر:        واپس HB_PAINT_مرکب_منوال_DEST_اندر;
    حالت FT_COLR_مرکب_SRC_برآمد:        واپس HB_PAINT_مرکب_منوال_SRC_برآمد;
    حالت FT_COLR_مرکب_DEST_برآمد:       واپس HB_PAINT_مرکب_منوال_DEST_برآمد;
    حالت FT_COLR_مرکب_SRC_ATOP:       واپس HB_PAINT_مرکب_منوال_SRC_ATOP;
    حالت FT_COLR_مرکب_DEST_ATOP:      واپس HB_PAINT_مرکب_منوال_DEST_ATOP;
    حالت FT_COLR_مرکب_XOR:            واپس HB_PAINT_مرکب_منوال_XOR;
    حالت FT_COLR_مرکب_PLUS:           واپس HB_PAINT_مرکب_منوال_PLUS;
    حالت FT_COLR_مرکب_SCREEN:         واپس HB_PAINT_مرکب_منوال_SCREEN;
    حالت FT_COLR_مرکب_OVERLAY:        واپس HB_PAINT_مرکب_منوال_OVERLAY;
    حالت FT_COLR_مرکب_DARKEN:         واپس HB_PAINT_مرکب_منوال_DARKEN;
    حالت FT_COLR_مرکب_LIGHTEN:        واپس HB_PAINT_مرکب_منوال_LIGHTEN;
    حالت FT_COLR_مرکب_COLOR_DODGE:    واپس HB_PAINT_مرکب_منوال_COLOR_DODGE;
    حالت FT_COLR_مرکب_COLOR_BURN:     واپس HB_PAINT_مرکب_منوال_COLOR_BURN;
    حالت FT_COLR_مرکب_HARD_LIGHT:     واپس HB_PAINT_مرکب_منوال_HARD_LIGHT;
    حالت FT_COLR_مرکب_SOFT_LIGHT:     واپس HB_PAINT_مرکب_منوال_SOFT_LIGHT;
    حالت FT_COLR_مرکب_DIFFERENCE:     واپس HB_PAINT_مرکب_منوال_DIFFERENCE;
    حالت FT_COLR_مرکب_EXCLUSION:      واپس HB_PAINT_مرکب_منوال_EXCLUSION;
    حالت FT_COLR_مرکب_MULTIPLY:       واپس HB_PAINT_مرکب_منوال_MULTIPLY;
    حالت FT_COLR_مرکب_HSL_HUE:        واپس HB_PAINT_مرکب_منوال_HSL_HUE;
    حالت FT_COLR_مرکب_HSL_SATURATION: واپس HB_PAINT_مرکب_منوال_HSL_SATURATION;
    حالت FT_COLR_مرکب_HSL_COLOR:      واپس HB_PAINT_مرکب_منوال_HSL_COLOR;
    حالت FT_COLR_مرکب_HSL_LUMINOSITY: واپس HB_PAINT_مرکب_منوال_HSL_LUMINOSITY;

    حالت FT_COLR_مرکب_MAX:            HB_FALLTHROUGH;
    طےشدہ:                               واپس HB_PAINT_مرکب_منوال_CLEAR;
  }
}

قسمتعریف ساخت hb_ft_paint_context_t hb_ft_paint_context_t;

ساکن خالی
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ساکن خالی
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

  *xmin = clip_box->bottom_بایاں.x * xscale;
  *ymin = clip_box->bottom_بایاں.y * yscale;
  *xmax = clip_box->چوٹی_دایاں.x * xscale;
  *ymax = clip_box->چوٹی_دایاں.y * yscale;
}

ساکن غیرعلامتی
_hb_ft_color_alpha (غیرعلامتی alpha, غیرعلامتی alpha_mult)
{
  واپس (alpha * alpha_mult + (1 << 13)) >> 14;
}

ساخت hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ثابت hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
			 hb_array_t<ثابت FT_Color> palette,
			 غیرعلامتی palette_اشاریہ,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), ڈیٹا (paint_ڈیٹا),
    palette (palette), palette_اشاریہ (palette_اشاریہ), foreground (foreground)
  {
    اگر (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_سیٹ_synthetic_bold (font, 0, 0, سچ);
      hb_font_سیٹ_synthetic_slant (font, 0);
    }
    ورنہ
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  خالی recurse (FT_OpaquePaint paint)
  {
    اگر (unlikely (گہرائی_بایاں <= 0 || کنارہ_گنتی <= 0)) واپس;
    گہرائی_بایاں--;
    کنارہ_گنتی--;
    _hb_ft_paint (یہ, paint);
    گہرائی_بایاں++;
  }

  ثابت hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  خالی *ڈیٹا;
  hb_array_t<ثابت FT_Color> palette;
  غیرعلامتی palette_اشاریہ;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  صحیح گہرائی_بایاں = HB_MAX_NESTING_LEVEL;
  صحیح کنارہ_گنتی = HB_MAX_گراف_کنارہ_گنتی;
};

ساکن غیرعلامتی
hb_ft_color_سطر_get_color_stops (hb_color_سطر_t *color_سطر,
				   خالی *color_سطر_ڈیٹا,
				   غیرعلامتی صحیح آغاز,
				   غیرعلامتی صحیح *گنتی,
				   hb_color_stop_t *color_stops,
				   خالی *user_ڈیٹا)
{
  FT_Color_سطر *cl = (FT_Color_سطر *) color_سطر_ڈیٹا;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_ڈیٹا;

  اگر (گنتی && color_stops)
  {
    FT_ColorStop stop;
    غیرعلامتی wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    اگر (آغاز >= cl->color_stop_iterator.عدد_color_stops)
    {
      *گنتی = 0;
      واپس cl->color_stop_iterator.عدد_color_stops;
    }

    جب (cl->color_stop_iterator.current_color_stop < آغاز)
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
        واپس cl->color_stop_iterator.عدد_color_stops;
      }
    }

    جب (wrote < *گنتی &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      اگر (حجم stop.stop_انحراف_2 == 2)
	color_stops->انحراف = stop.stop_انحراف_2 / 16384.f;
      ورنہ
	color_stops->انحراف = stop.stop_انحراف_2 / 65536.f;

      color_stops->is_foreground = stop.color.palette_اشاریہ == 0xFFFF;
      اگر (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      ورنہ
      {
	hb_color_t color;
        اگر (c->funcs->custom_palette_color (c->ڈیٹا, stop.color.palette_اشاریہ, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	ورنہ اگر (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_اشاریہ];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	ورنہ
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *گنتی = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  واپس cl->color_stop_iterator.عدد_color_stops;
}

ساکن hb_paint_extend_t
hb_ft_color_سطر_get_extend (hb_color_سطر_t *color_سطر,
			      خالی *color_سطر_ڈیٹا,
			      خالی *user_ڈیٹا)
{
  FT_Color_سطر *c = (FT_Color_سطر *) color_سطر_ڈیٹا;
  انتخاب (c->extend)
  {
    طےشدہ:
    حالت FT_COLR_PAINT_EXTEND_PAD:     واپس HB_PAINT_EXTEND_PAD;
    حالت FT_COLR_PAINT_EXTEND_REPEAT:  واپس HB_PAINT_EXTEND_REPEAT;
    حالت FT_COLR_PAINT_EXTEND_REFLECT: واپس HB_PAINT_EXTEND_REFLECT;
  }
}

ساکن خالی
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ثابت FT_Color_اشاریہ &color_اشاریہ,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_اشاریہ.palette_اشاریہ == 0xFFFF;

  اگر (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_اشاریہ.alpha));
    واپس;
  }

  اگر (c->funcs->custom_palette_color (c->ڈیٹا, color_اشاریہ.palette_اشاریہ, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_اشاریہ.alpha));
    واپس;
  }

  FT_Color ft_color = c->palette[color_اشاریہ.palette_اشاریہ];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_اشاریہ.alpha));
}

خالی
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  اگر (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    واپس;

  انتخاب (paint.format)
  {
    حالت FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_گرہ_t node (c->layers_decycler);
      جب (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	اگر (unlikely (!node.دورہ ((uintptr_t) other_paint.p)))
	  جاری;

	c->recurse (other_paint);
      }
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->ڈیٹا, is_foreground, color);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_خطی_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.خطی_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, خالی_اشاریہ
      };

      c->funcs->خطی_gradient (c->ڈیٹا, &cl,
				 paint.u.خطی_gradient.p0.x / 65536.f,
				 paint.u.خطی_gradient.p0.y / 65536.f,
				 paint.u.خطی_gradient.p1.x / 65536.f,
				 paint.u.خطی_gradient.p1.y / 65536.f,
				 paint.u.خطی_gradient.p2.x / 65536.f,
				 paint.u.خطی_gradient.p2.y / 65536.f);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.خطی_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, خالی_اشاریہ
      };

      c->funcs->radial_gradient (c->ڈیٹا, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_سطر_t cl = {
	&paint.u.خطی_gradient.colorline,
	hb_ft_color_سطر_get_color_stops, c,
	hb_ft_color_سطر_get_extend, خالی_اشاریہ
      };

      c->funcs->sweep_gradient (c->ڈیٹا, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.آغاز_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.انتہا_angle / 65536.f + 1) * HB_PI);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      اگر (likely (c->گہرائی_بایاں > 0 && c->کنارہ_گنتی > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->کنارہ_گنتی--;
	c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->ڈیٹا, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->نکالو_transform (c->ڈیٹا);
	توڑو;
      }

      c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ڈالو_clip_glyph (c->ڈیٹا, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ڈالو_font_transform (c->ڈیٹا, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
      c->funcs->نکالو_clip (c->ڈیٹا);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_گرہ_t node (c->glyphs_decycler);
      اگر (unlikely (!node.دورہ (gid)))
	واپس;

      c->funcs->ڈالو_inverse_font_transform (c->ڈیٹا, c->font);
      c->ft_font->lock.unlock ();
      اگر (c->funcs->color_glyph (c->ڈیٹا, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->نکالو_transform (c->ڈیٹا);
	واپس;
      }
      c->ft_font->lock.lock ();
      c->funcs->نکالو_transform (c->ڈیٹا);

      FT_OpaquePaint other_paint = {0};
      اگر (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_جڑ_TRANSFORM,
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
	  c->funcs->ڈالو_clip_rectangle (c->ڈیٹا, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        اگر (has_clip_box)
          c->funcs->نکالو_clip (c->ڈیٹا);
      }
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ڈالو_transform (c->ڈیٹا,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      اعشاری dx = paint.u.translate.dx / 65536.f;
      اعشاری dy = paint.u.translate.dy / 65536.f;

      c->funcs->ڈالو_translate (c->ڈیٹا, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_SCALE:
    {
      اعشاری dx = paint.u.scale.center_x / 65536.f;
      اعشاری dy = paint.u.scale.center_y / 65536.f;
      اعشاری sx = paint.u.scale.scale_x / 65536.f;
      اعشاری sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ڈالو_scale_around_center (c->ڈیٹا, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_ROTATE:
    {
      اعشاری dx = paint.u.rotate.center_x / 65536.f;
      اعشاری dy = paint.u.rotate.center_y / 65536.f;
      اعشاری a = paint.u.rotate.angle / 65536.f;

      c->funcs->ڈالو_rotate_around_center (c->ڈیٹا, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_SKEW:
    {
      اعشاری dx = paint.u.skew.center_x / 65536.f;
      اعشاری dy = paint.u.skew.center_y / 65536.f;
      اعشاری sx = paint.u.skew.x_skew_angle / 65536.f;
      اعشاری sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ڈالو_skew_around_center (c->ڈیٹا, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->نکالو_transform (c->ڈیٹا);
    }
    توڑو;
    حالت FT_COLR_PAINTFORMAT_مرکب:
    {
      c->funcs->ڈالو_گروہ (c->ڈیٹا);
      c->recurse (paint.u.مرکب_2.backdrop_paint);
      c->funcs->ڈالو_گروہ (c->ڈیٹا);
      c->recurse (paint.u.مرکب_2.ماخذ_paint);
      c->funcs->نکالو_گروہ (c->ڈیٹا, hb_ft_paint_مرکب_منوال (paint.u.مرکب_2.مرکب_منوال_2));
      c->funcs->نکالو_گروہ (c->ڈیٹا, HB_PAINT_مرکب_منوال_SRC_OVER);
    }
    توڑو;

    حالت FT_COLR_PAINT_FORMAT_MAX: توڑو;
    طےشدہ: HB_FALLTHROUGH;
    حالت FT_COLR_PAINTFORMAT_UNSUPPORTED: توڑو;
  }
}


ساکن منطقی
hb_ft_paint_glyph_colr (hb_font_t *font,
			خالی *font_ڈیٹا,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, خالی *paint_ڈیٹا,
			غیرعلامتی صحیح palette_اشاریہ,
			hb_color_t foreground,
			خالی *user_ڈیٹا)
{
  ثابت hb_ft_font_t *ft_font = (ثابت hb_ft_font_t *) font_ڈیٹا;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_ڈیٹا   palette_ڈیٹا = {};
  FT_Color*         palette = تہی;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_اشاریہ;
  FT_UInt  layer_color_اشاریہ;

  (خالی) FT_Palette_ڈیٹا_Get(ft_face, &palette_ڈیٹا);
  (خالی) FT_Palette_Select(ft_face, palette_اشاریہ, &palette);
  اگر (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (خالی) FT_Palette_Select(ft_face, 0, &palette);
  }

  خودکار palette_array = hb_array ((ثابت FT_Color *) palette,
				 palette ? palette_ڈیٹا.عدد_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  اگر (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_جڑ_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_ڈیٹا,
			     palette_array, palette_اشاریہ, foreground);
    hb_decycler_گرہ_t node (c.glyphs_decycler);
    node.دورہ (gid);

    FT_ClipBox clip_box;
    منطقی clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    منطقی is_bounded = clip;
    اگر (!is_bounded)
    {
      خودکار *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_ڈیٹا;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_ڈیٹا,
			        palette_array, palette_اشاریہ, foreground);
      hb_decycler_گرہ_t node2 (ce.glyphs_decycler);
      node2.دورہ (gid);
      ce.recurse (paint);
      is_bounded = bounded_ڈیٹا.is_bounded ();
    }

    c.funcs->ڈالو_font_transform (c.ڈیٹا, font);

    اگر (clip)
    {
      اعشاری xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ڈالو_clip_rectangle (c.ڈیٹا, xmin, ymin, xmax, ymax);
    }

    اگر (is_bounded)
      c.recurse (paint);

    اگر (clip)
      c.funcs->نکالو_clip (c.ڈیٹا);

    c.funcs->نکالو_transform (c.ڈیٹا);

    واپس سچ;
  }

  /* COLRv0 */
  iterator.p  = تہی;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_اشاریہ,
					 &layer_color_اشاریہ,
					 &iterator);

  اگر (palette && have_layers)
  {
    کرو
    {
      hb_bool_t is_foreground = سچ;
      hb_color_t color = foreground;

      اگر ( layer_color_اشاریہ != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_اشاریہ];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = جھوٹ;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_ڈیٹا, layer_glyph_اشاریہ, font, is_foreground, color);
      ft_font->lock.lock ();

    } جب (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_اشاریہ,
				      &layer_color_اشاریہ,
				      &iterator));
    واپس سچ;
  }

  واپس جھوٹ;
}


#اگر_ختم /* HB_FT_COLR_HH */
