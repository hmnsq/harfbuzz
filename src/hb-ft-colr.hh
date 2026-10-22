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
  নির্বাচন (mode)
  {
    ক্ষেত্র FT_COLR_COMPOSITE_CLEAR:          ফেরত HB_PAINT_COMPOSITE_MODE_CLEAR;
    ক্ষেত্র FT_COLR_COMPOSITE_SRC:            ফেরত HB_PAINT_COMPOSITE_MODE_SRC;
    ক্ষেত্র FT_COLR_COMPOSITE_DEST:           ফেরত HB_PAINT_COMPOSITE_MODE_DEST;
    ক্ষেত্র FT_COLR_COMPOSITE_SRC_OVER:       ফেরত HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    ক্ষেত্র FT_COLR_COMPOSITE_DEST_OVER:      ফেরত HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    ক্ষেত্র FT_COLR_COMPOSITE_SRC_IN:         ফেরত HB_PAINT_COMPOSITE_MODE_SRC_IN;
    ক্ষেত্র FT_COLR_COMPOSITE_DEST_IN:        ফেরত HB_PAINT_COMPOSITE_MODE_DEST_IN;
    ক্ষেত্র FT_COLR_COMPOSITE_SRC_OUT:        ফেরত HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    ক্ষেত্র FT_COLR_COMPOSITE_DEST_OUT:       ফেরত HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    ক্ষেত্র FT_COLR_COMPOSITE_SRC_ATOP:       ফেরত HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    ক্ষেত্র FT_COLR_COMPOSITE_DEST_ATOP:      ফেরত HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    ক্ষেত্র FT_COLR_COMPOSITE_XOR:            ফেরত HB_PAINT_COMPOSITE_MODE_XOR;
    ক্ষেত্র FT_COLR_COMPOSITE_PLUS:           ফেরত HB_PAINT_COMPOSITE_MODE_PLUS;
    ক্ষেত্র FT_COLR_COMPOSITE_SCREEN:         ফেরত HB_PAINT_COMPOSITE_MODE_SCREEN;
    ক্ষেত্র FT_COLR_COMPOSITE_OVERLAY:        ফেরত HB_PAINT_COMPOSITE_MODE_OVERLAY;
    ক্ষেত্র FT_COLR_COMPOSITE_DARKEN:         ফেরত HB_PAINT_COMPOSITE_MODE_DARKEN;
    ক্ষেত্র FT_COLR_COMPOSITE_LIGHTEN:        ফেরত HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    ক্ষেত্র FT_COLR_COMPOSITE_COLOR_DODGE:    ফেরত HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    ক্ষেত্র FT_COLR_COMPOSITE_COLOR_BURN:     ফেরত HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    ক্ষেত্র FT_COLR_COMPOSITE_HARD_LIGHT:     ফেরত HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    ক্ষেত্র FT_COLR_COMPOSITE_SOFT_LIGHT:     ফেরত HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    ক্ষেত্র FT_COLR_COMPOSITE_DIFFERENCE:     ফেরত HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    ক্ষেত্র FT_COLR_COMPOSITE_EXCLUSION:      ফেরত HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    ক্ষেত্র FT_COLR_COMPOSITE_MULTIPLY:       ফেরত HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    ক্ষেত্র FT_COLR_COMPOSITE_HSL_HUE:        ফেরত HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    ক্ষেত্র FT_COLR_COMPOSITE_HSL_SATURATION: ফেরত HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    ক্ষেত্র FT_COLR_COMPOSITE_HSL_COLOR:      ফেরত HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    ক্ষেত্র FT_COLR_COMPOSITE_HSL_LUMINOSITY: ফেরত HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    ক্ষেত্র FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    পূর্বনির্ধারিত:                               ফেরত HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

প্রকারলেখ কাঠামো hb_ft_paint_context_t hb_ft_paint_context_t;

স্থির শূন্য
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

স্থির শূন্য
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ধ্রুবক FT_ClipBox *clip_box,
			 দশমিক *xmin, দশমিক *ymin,
			 দশমিক *xmax, দশমিক *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  দশমিক upem = font->face->get_upem ();
  দশমিক xscale = upem / (font->x_scale ? font->x_scale : upem);
  দশমিক yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_বাম.x * xscale;
  *ymin = clip_box->bottom_বাম.y * yscale;
  *xmax = clip_box->শীর্ষ_ডান.x * xscale;
  *ymax = clip_box->শীর্ষ_ডান.y * yscale;
}

স্থির unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  ফেরত (alpha * alpha_mult + (1 << 13)) >> 14;
}

কাঠামো hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ধ্রুবক hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
			 hb_array_t<ধ্রুবক FT_Color> palette,
			 unsigned palette_সূচক,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), তথ্য (paint_তথ্য),
    palette (palette), palette_সূচক (palette_সূচক), foreground (foreground)
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
    যদি (unlikely (depth_বাম <= 0 || edge_গুনতি <= 0)) ফেরত;
    depth_বাম--;
    edge_গুনতি--;
    _hb_ft_paint (এই, paint);
    depth_বাম++;
  }

  ধ্রুবক hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  শূন্য *তথ্য;
  hb_array_t<ধ্রুবক FT_Color> palette;
  unsigned palette_সূচক;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  পূর্ণসংখ্যা depth_বাম = HB_MAX_NESTING_LEVEL;
  পূর্ণসংখ্যা edge_গুনতি = HB_MAX_GRAPH_EDGE_গুনতি;
};

স্থির unsigned
hb_ft_color_সারি_get_color_stops (hb_color_সারি_t *color_সারি,
				   শূন্য *color_সারি_তথ্য,
				   unsigned পূর্ণসংখ্যা start,
				   unsigned পূর্ণসংখ্যা *গুনতি,
				   hb_color_stop_t *color_stops,
				   শূন্য *user_তথ্য)
{
  FT_Color_সারি *cl = (FT_Color_সারি *) color_সারি_তথ্য;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_তথ্য;

  যদি (গুনতি && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    যদি (start >= cl->color_stop_iterator.অঙ্ক_color_stops)
    {
      *গুনতি = 0;
      ফেরত cl->color_stop_iterator.অঙ্ক_color_stops;
    }

    যখন (cl->color_stop_iterator.current_color_stop < start)
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
        ফেরত cl->color_stop_iterator.অঙ্ক_color_stops;
      }
    }

    যখন (wrote < *গুনতি &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      যদি (আকার stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      অন্যথা
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_সূচক == 0xFFFF;
      যদি (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      অন্যথা
      {
	hb_color_t color;
        যদি (c->funcs->custom_palette_color (c->তথ্য, stop.color.palette_সূচক, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	অন্যথা যদি (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_সূচক];
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

  ফেরত cl->color_stop_iterator.অঙ্ক_color_stops;
}

স্থির hb_paint_extend_t
hb_ft_color_সারি_get_extend (hb_color_সারি_t *color_সারি,
			      শূন্য *color_সারি_তথ্য,
			      শূন্য *user_তথ্য)
{
  FT_Color_সারি *c = (FT_Color_সারি *) color_সারি_তথ্য;
  নির্বাচন (c->extend)
  {
    পূর্বনির্ধারিত:
    ক্ষেত্র FT_COLR_PAINT_EXTEND_PAD:     ফেরত HB_PAINT_EXTEND_PAD;
    ক্ষেত্র FT_COLR_PAINT_EXTEND_REPEAT:  ফেরত HB_PAINT_EXTEND_REPEAT;
    ক্ষেত্র FT_COLR_PAINT_EXTEND_REFLECT: ফেরত HB_PAINT_EXTEND_REFLECT;
  }
}

স্থির শূন্য
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ধ্রুবক FT_Color_সূচক &color_সূচক,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_সূচক.palette_সূচক == 0xFFFF;

  যদি (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_সূচক.alpha));
    ফেরত;
  }

  যদি (c->funcs->custom_palette_color (c->তথ্য, color_সূচক.palette_সূচক, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_সূচক.alpha));
    ফেরত;
  }

  FT_Color ft_color = c->palette[color_সূচক.palette_সূচক];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_সূচক.alpha));
}

শূন্য
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  যদি (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    ফেরত;

  নির্বাচন (paint.format)
  {
    ক্ষেত্র FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_গ্রন্থি_t node (c->layers_decycler);
      যখন (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	যদি (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  চালু;

	c->recurse (other_paint);
      }
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->তথ্য, is_foreground, color);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->তথ্য, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->তথ্য, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_সারি_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_সারি_get_color_stops, c,
	hb_ft_color_সারি_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->তথ্য, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_GLYPH:
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
	c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->তথ্য, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->বের_করো_transform (c->তথ্য);
	বিরতি;
      }

      c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ঠেলো_clip_glyph (c->তথ্য, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ঠেলো_font_transform (c->তথ্য, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
      c->funcs->বের_করো_clip (c->তথ্য);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_গ্রন্থি_t node (c->glyphs_decycler);
      যদি (unlikely (!node.visit (gid)))
	ফেরত;

      c->funcs->ঠেলো_inverse_font_transform (c->তথ্য, c->font);
      c->ft_font->lock.unlock ();
      যদি (c->funcs->color_glyph (c->তথ্য, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->বের_করো_transform (c->তথ্য);
	ফেরত;
      }
      c->ft_font->lock.lock ();
      c->funcs->বের_করো_transform (c->তথ্য);

      FT_OpaquePaint other_paint = {0};
      যদি (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_মূল_TRANSFORM,
				    &other_paint))
      {
        যৌক্তিক has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        যদি (has_clip_box)
	{
	  দশমিক xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ঠেলো_clip_rectangle (c->তথ্য, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        যদি (has_clip_box)
          c->funcs->বের_করো_clip (c->তথ্য);
      }
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ঠেলো_transform (c->তথ্য,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      দশমিক dx = paint.u.translate.dx / 65536.f;
      দশমিক dy = paint.u.translate.dy / 65536.f;

      c->funcs->ঠেলো_translate (c->তথ্য, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_SCALE:
    {
      দশমিক dx = paint.u.scale.center_x / 65536.f;
      দশমিক dy = paint.u.scale.center_y / 65536.f;
      দশমিক sx = paint.u.scale.scale_x / 65536.f;
      দশমিক sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ঠেলো_scale_around_center (c->তথ্য, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_ROTATE:
    {
      দশমিক dx = paint.u.rotate.center_x / 65536.f;
      দশমিক dy = paint.u.rotate.center_y / 65536.f;
      দশমিক a = paint.u.rotate.angle / 65536.f;

      c->funcs->ঠেলো_rotate_around_center (c->তথ্য, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_SKEW:
    {
      দশমিক dx = paint.u.skew.center_x / 65536.f;
      দশমিক dy = paint.u.skew.center_y / 65536.f;
      দশমিক sx = paint.u.skew.x_skew_angle / 65536.f;
      দশমিক sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ঠেলো_skew_around_center (c->তথ্য, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->বের_করো_transform (c->তথ্য);
    }
    বিরতি;
    ক্ষেত্র FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->ঠেলো_group (c->তথ্য);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->ঠেলো_group (c->তথ্য);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->বের_করো_group (c->তথ্য, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->বের_করো_group (c->তথ্য, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    বিরতি;

    ক্ষেত্র FT_COLR_PAINT_FORMAT_MAX: বিরতি;
    পূর্বনির্ধারিত: HB_FALLTHROUGH;
    ক্ষেত্র FT_COLR_PAINTFORMAT_UNSUPPORTED: বিরতি;
  }
}


স্থির যৌক্তিক
hb_ft_paint_glyph_colr (hb_font_t *font,
			শূন্য *font_তথ্য,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, শূন্য *paint_তথ্য,
			unsigned পূর্ণসংখ্যা palette_সূচক,
			hb_color_t foreground,
			শূন্য *user_তথ্য)
{
  ধ্রুবক hb_ft_font_t *ft_font = (ধ্রুবক hb_ft_font_t *) font_তথ্য;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_তথ্য   palette_তথ্য = {};
  FT_Color*         palette = ফাঁকা;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_সূচক;
  FT_UInt  layer_color_সূচক;

  (শূন্য) FT_Palette_তথ্য_Get(ft_face, &palette_তথ্য);
  (শূন্য) FT_Palette_Select(ft_face, palette_সূচক, &palette);
  যদি (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (শূন্য) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ধ্রুবক FT_Color *) palette,
				 palette ? palette_তথ্য.অঙ্ক_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  যদি (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_মূল_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_তথ্য,
			     palette_array, palette_সূচক, foreground);
    hb_decycler_গ্রন্থি_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    যৌক্তিক clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    যৌক্তিক is_bounded = clip;
    যদি (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_তথ্য;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_তথ্য,
			        palette_array, palette_সূচক, foreground);
      hb_decycler_গ্রন্থি_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_তথ্য.is_bounded ();
    }

    c.funcs->ঠেলো_font_transform (c.তথ্য, font);

    যদি (clip)
    {
      দশমিক xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ঠেলো_clip_rectangle (c.তথ্য, xmin, ymin, xmax, ymax);
    }

    যদি (is_bounded)
      c.recurse (paint);

    যদি (clip)
      c.funcs->বের_করো_clip (c.তথ্য);

    c.funcs->বের_করো_transform (c.তথ্য);

    ফেরত সত্য;
  }

  /* COLRv0 */
  iterator.p  = ফাঁকা;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_সূচক,
					 &layer_color_সূচক,
					 &iterator);

  যদি (palette && have_layers)
  {
    করো
    {
      hb_bool_t is_foreground = সত্য;
      hb_color_t color = foreground;

      যদি ( layer_color_সূচক != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_সূচক];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = মিথ্যা;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_তথ্য, layer_glyph_সূচক, font, is_foreground, color);
      ft_font->lock.lock ();

    } যখন (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_সূচক,
				      &layer_color_সূচক,
				      &iterator));
    ফেরত সত্য;
  }

  ফেরত মিথ্যা;
}


#endif /* HB_FT_COLR_HH */
