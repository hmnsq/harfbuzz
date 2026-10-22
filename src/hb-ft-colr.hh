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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-decycler.hh"
#অন্তর্ভাব "hb-paint-bounded.hh"

#অন্তর্ভাব FT_COLOR_H


স্থির hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  বিকল্প (mode)
  {
    পক্ষ FT_COLR_COMPOSITE_CLEAR:          প্রত্যাগম HB_PAINT_COMPOSITE_MODE_CLEAR;
    পক্ষ FT_COLR_COMPOSITE_SRC:            প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SRC;
    পক্ষ FT_COLR_COMPOSITE_DEST:           প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DEST;
    পক্ষ FT_COLR_COMPOSITE_SRC_OVER:       প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    পক্ষ FT_COLR_COMPOSITE_DEST_OVER:      প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    পক্ষ FT_COLR_COMPOSITE_SRC_IN:         প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SRC_IN;
    পক্ষ FT_COLR_COMPOSITE_DEST_IN:        প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DEST_IN;
    পক্ষ FT_COLR_COMPOSITE_SRC_OUT:        প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    পক্ষ FT_COLR_COMPOSITE_DEST_OUT:       প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    পক্ষ FT_COLR_COMPOSITE_SRC_ATOP:       প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    পক্ষ FT_COLR_COMPOSITE_DEST_ATOP:      প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    পক্ষ FT_COLR_COMPOSITE_XOR:            প্রত্যাগম HB_PAINT_COMPOSITE_MODE_XOR;
    পক্ষ FT_COLR_COMPOSITE_PLUS:           প্রত্যাগম HB_PAINT_COMPOSITE_MODE_PLUS;
    পক্ষ FT_COLR_COMPOSITE_SCREEN:         প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SCREEN;
    পক্ষ FT_COLR_COMPOSITE_OVERLAY:        প্রত্যাগম HB_PAINT_COMPOSITE_MODE_OVERLAY;
    পক্ষ FT_COLR_COMPOSITE_DARKEN:         প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DARKEN;
    পক্ষ FT_COLR_COMPOSITE_LIGHTEN:        প্রত্যাগম HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    পক্ষ FT_COLR_COMPOSITE_COLOR_DODGE:    প্রত্যাগম HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    পক্ষ FT_COLR_COMPOSITE_COLOR_BURN:     প্রত্যাগম HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    পক্ষ FT_COLR_COMPOSITE_HARD_LIGHT:     প্রত্যাগম HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    পক্ষ FT_COLR_COMPOSITE_SOFT_LIGHT:     প্রত্যাগম HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    পক্ষ FT_COLR_COMPOSITE_DIFFERENCE:     প্রত্যাগম HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    পক্ষ FT_COLR_COMPOSITE_EXCLUSION:      প্রত্যাগম HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    পক্ষ FT_COLR_COMPOSITE_MULTIPLY:       প্রত্যাগম HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    পক্ষ FT_COLR_COMPOSITE_HSL_HUE:        প্রত্যাগম HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    পক্ষ FT_COLR_COMPOSITE_HSL_SATURATION: প্রত্যাগম HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    পক্ষ FT_COLR_COMPOSITE_HSL_COLOR:      প্রত্যাগম HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    পক্ষ FT_COLR_COMPOSITE_HSL_LUMINOSITY: প্রত্যাগম HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    পক্ষ FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    স্বভাব:                               প্রত্যাগম HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

প্রকারনির্দেশ সংরচনা hb_ft_paint_context_t hb_ft_paint_context_t;

স্থির শূন্য
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

স্থির শূন্য
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ধ্রুব FT_ClipBox *clip_box,
			 ভিন্ন *xmin, ভিন্ন *ymin,
			 ভিন্ন *xmax, ভিন্ন *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  ভিন্ন upem = font->face->get_upem ();
  ভিন্ন xscale = upem / (font->x_scale ? font->x_scale : upem);
  ভিন্ন yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_বাম.x * xscale;
  *ymin = clip_box->bottom_বাম.y * yscale;
  *xmax = clip_box->শীর্ষ_ডান.x * xscale;
  *ymax = clip_box->শীর্ষ_ডান.y * yscale;
}

স্থির unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  প্রত্যাগম (alpha * alpha_mult + (1 << 13)) >> 14;
}

সংরচনা hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ধ্রুব hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, শূন্য *paint_data,
			 hb_array_t<ধ্রুব FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    যদি (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, সত্য);
      hb_font_set_synthetic_slant (font, 0);
    }
    অন্যথা
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  শূন্য recurse (FT_OpaquePaint paint)
  {
    যদি (unlikely (depth_বাম <= 0 || edge_গুনতি <= 0)) প্রত্যাগম;
    depth_বাম--;
    edge_গুনতি--;
    _hb_ft_paint (এতত্, paint);
    depth_বাম++;
  }

  ধ্রুব hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  শূন্য *data;
  hb_array_t<ধ্রুব FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  পূর্ণাঙ্ক depth_বাম = HB_MAX_NESTING_LEVEL;
  পূর্ণাঙ্ক edge_গুনতি = HB_MAX_GRAPH_EDGE_গুনতি;
};

স্থির unsigned
hb_ft_color_সারি_get_color_stops (hb_color_সারি_t *color_সারি,
				   শূন্য *color_সারি_data,
				   unsigned পূর্ণাঙ্ক start,
				   unsigned পূর্ণাঙ্ক *গুনতি,
				   hb_color_stop_t *color_stops,
				   শূন্য *user_data)
{
  FT_Color_সারি *cl = (FT_Color_সারি *) color_সারি_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  যদি (গুনতি && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    যদি (start >= cl->color_stop_iterator.অঙ্ক_color_stops)
    {
      *গুনতি = 0;
      প্রত্যাগম cl->color_stop_iterator.অঙ্ক_color_stops;
    }

    যাবত্ (cl->color_stop_iterator.current_color_stop < start)
    {
      যদি (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        প্রত্যাগম cl->color_stop_iterator.অঙ্ক_color_stops;
      }
    }

    যাবত্ (wrote < *গুনতি &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      যদি (পরিমাণ stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      অন্যথা
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      যদি (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      অন্যথা
      {
	hb_color_t color;
        যদি (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	অন্যথা যদি (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	অন্যথা
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *গুনতি = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  প্রত্যাগম cl->color_stop_iterator.অঙ্ক_color_stops;
}

স্থির hb_paint_extend_t
hb_ft_color_সারি_get_extend (hb_color_সারি_t *color_সারি,
			      শূন্য *color_সারি_data,
			      শূন্য *user_data)
{
  FT_Color_সারি *c = (FT_Color_সারি *) color_সারি_data;
  বিকল্প (c->extend)
  {
    স্বভাব:
    পক্ষ FT_COLR_PAINT_EXTEND_PAD:     প্রত্যাগম HB_PAINT_EXTEND_PAD;
    পক্ষ FT_COLR_PAINT_EXTEND_REPEAT:  প্রত্যাগম HB_PAINT_EXTEND_REPEAT;
    পক্ষ FT_COLR_PAINT_EXTEND_REFLECT: প্রত্যাগম HB_PAINT_EXTEND_REFLECT;
  }
}

স্থির শূন্য
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ধ্রুব FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  যদি (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    প্রত্যাগম;
  }

  যদি (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    প্রত্যাগম;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

শূন্য
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  যদি (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    প্রত্যাগম;

  বিকল্প (paint.format)
  {
    পক্ষ FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_গ্রন্থি_t node (c->layers_decycler);
      যাবত্ (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	যদি (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  অনুবর্ত;

	c->recurse (other_paint);
      }
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      যদি (likely (c->depth_বাম > 0 && c->edge_গুনতি > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_গুনতি--;
	c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->বের_করো_transform (c->data);
	ভঙ্গ;
      }

      c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ঠেলো_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ঠেলো_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->বের_করো_transform (c->data);
      c->funcs->বের_করো_clip (c->data);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_গ্রন্থি_t node (c->glyphs_decycler);
      যদি (unlikely (!node.visit (gid)))
	প্রত্যাগম;

      c->funcs->ঠেলো_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      যদি (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->বের_করো_transform (c->data);
	প্রত্যাগম;
      }
      c->ft_font->lock.lock ();
      c->funcs->বের_করো_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      যদি (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_মূল_TRANSFORM,
				    &other_paint))
      {
        তার্কিক has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        যদি (has_clip_box)
	{
	  ভিন্ন xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ঠেলো_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        যদি (has_clip_box)
          c->funcs->বের_করো_clip (c->data);
      }
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ঠেলো_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      ভিন্ন dx = paint.u.translate.dx / 65536.f;
      ভিন্ন dy = paint.u.translate.dy / 65536.f;

      c->funcs->ঠেলো_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_SCALE:
    {
      ভিন্ন dx = paint.u.scale.center_x / 65536.f;
      ভিন্ন dy = paint.u.scale.center_y / 65536.f;
      ভিন্ন sx = paint.u.scale.scale_x / 65536.f;
      ভিন্ন sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ঠেলো_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_ROTATE:
    {
      ভিন্ন dx = paint.u.rotate.center_x / 65536.f;
      ভিন্ন dy = paint.u.rotate.center_y / 65536.f;
      ভিন্ন a = paint.u.rotate.angle / 65536.f;

      c->funcs->ঠেলো_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_SKEW:
    {
      ভিন্ন dx = paint.u.skew.center_x / 65536.f;
      ভিন্ন dy = paint.u.skew.center_y / 65536.f;
      ভিন্ন sx = paint.u.skew.x_skew_angle / 65536.f;
      ভিন্ন sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ঠেলো_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->বের_করো_transform (c->data);
    }
    ভঙ্গ;
    পক্ষ FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ঠেলো_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ঠেলো_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->বের_করো_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->বের_করো_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    ভঙ্গ;

    পক্ষ FT_COLR_PAINT_FORMAT_MAX: ভঙ্গ;
    স্বভাব: HB_FALLTHROUGH;
    পক্ষ FT_COLR_PAINTFORMAT_UNSUPPORTED: ভঙ্গ;
  }
}


স্থির তার্কিক
hb_ft_paint_glyph_colr (hb_font_t *font,
			শূন্য *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, শূন্য *paint_data,
			unsigned পূর্ণাঙ্ক palette_index,
			hb_color_t foreground,
			শূন্য *user_data)
{
  ধ্রুব hb_ft_font_t *ft_font = (ধ্রুব hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = রিক্ত;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (শূন্য) FT_Palette_Data_Get(ft_face, &palette_data);
  (শূন্য) FT_Palette_Select(ft_face, palette_index, &palette);
  যদি (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (শূন্য) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ধ্রুব FT_Color *) palette,
				 palette ? palette_data.অঙ্ক_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  যদি (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_মূল_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_গ্রন্থি_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    তার্কিক clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    তার্কিক is_bounded = clip;
    যদি (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_গ্রন্থি_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->ঠেলো_font_transform (c.data, font);

    যদি (clip)
    {
      ভিন্ন xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ঠেলো_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    যদি (is_bounded)
      c.recurse (paint);

    যদি (clip)
      c.funcs->বের_করো_clip (c.data);

    c.funcs->বের_করো_transform (c.data);

    প্রত্যাগম সত্য;
  }

  /* COLRv0 */
  iterator.p  = রিক্ত;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  যদি (palette && have_layers)
  {
    কুরু
    {
      hb_bool_t is_foreground = সত্য;
      hb_color_t color = foreground;

      যদি ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = অসত্য;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } যাবত্ (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    প্রত্যাগম সত্য;
  }

  প্রত্যাগম অসত্য;
}


#endif /* HB_FT_COLR_HH */
