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

#include "hb.hh"

#include "hb-decycler.hh"
#include "hb-paint-bounded.hh"

#include FT_COLOR_H


static hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  switch (mode)
  {
    case FT_COLR_COMPOSITE_CLEAR:          вернуть HB_PAINT_COMPOSITE_MODE_CLEAR;
    case FT_COLR_COMPOSITE_SRC:            вернуть HB_PAINT_COMPOSITE_MODE_SRC;
    case FT_COLR_COMPOSITE_DEST:           вернуть HB_PAINT_COMPOSITE_MODE_DEST;
    case FT_COLR_COMPOSITE_SRC_OVER:       вернуть HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    case FT_COLR_COMPOSITE_DEST_OVER:      вернуть HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    case FT_COLR_COMPOSITE_SRC_IN:         вернуть HB_PAINT_COMPOSITE_MODE_SRC_IN;
    case FT_COLR_COMPOSITE_DEST_IN:        вернуть HB_PAINT_COMPOSITE_MODE_DEST_IN;
    case FT_COLR_COMPOSITE_SRC_OUT:        вернуть HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    case FT_COLR_COMPOSITE_DEST_OUT:       вернуть HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    case FT_COLR_COMPOSITE_SRC_ATOP:       вернуть HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    case FT_COLR_COMPOSITE_DEST_ATOP:      вернуть HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    case FT_COLR_COMPOSITE_XOR:            вернуть HB_PAINT_COMPOSITE_MODE_XOR;
    case FT_COLR_COMPOSITE_PLUS:           вернуть HB_PAINT_COMPOSITE_MODE_PLUS;
    case FT_COLR_COMPOSITE_SCREEN:         вернуть HB_PAINT_COMPOSITE_MODE_SCREEN;
    case FT_COLR_COMPOSITE_OVERLAY:        вернуть HB_PAINT_COMPOSITE_MODE_OVERLAY;
    case FT_COLR_COMPOSITE_DARKEN:         вернуть HB_PAINT_COMPOSITE_MODE_DARKEN;
    case FT_COLR_COMPOSITE_LIGHTEN:        вернуть HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    case FT_COLR_COMPOSITE_COLOR_DODGE:    вернуть HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    case FT_COLR_COMPOSITE_COLOR_BURN:     вернуть HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    case FT_COLR_COMPOSITE_HARD_LIGHT:     вернуть HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    case FT_COLR_COMPOSITE_SOFT_LIGHT:     вернуть HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    case FT_COLR_COMPOSITE_DIFFERENCE:     вернуть HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    case FT_COLR_COMPOSITE_EXCLUSION:      вернуть HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    case FT_COLR_COMPOSITE_MULTIPLY:       вернуть HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    case FT_COLR_COMPOSITE_HSL_HUE:        вернуть HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    case FT_COLR_COMPOSITE_HSL_SATURATION: вернуть HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    case FT_COLR_COMPOSITE_HSL_COLOR:      вернуть HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    case FT_COLR_COMPOSITE_HSL_LUMINOSITY: вернуть HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    case FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    default:                               вернуть HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

typedef struct hb_ft_paint_context_t hb_ft_paint_context_t;

static void
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

static void
_hb_ft_unscale_clip_box (hb_font_t *font,
			 const FT_ClipBox *clip_box,
			 float *xmin, float *ymin,
			 float *xmax, float *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  float upem = font->face->get_upem ();
  float xscale = upem / (font->x_scale ? font->x_scale : upem);
  float yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_left.x * xscale;
  *ymin = clip_box->bottom_left.y * yscale;
  *xmax = clip_box->top_right.x * xscale;
  *ymax = clip_box->top_right.y * yscale;
}

static unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  вернуть (alpha * alpha_mult + (1 << 13)) >> 14;
}

struct hb_ft_paint_context_t
{
  hb_ft_paint_context_t (const hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, void *paint_data,
			 hb_array_t<const FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    если (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, true);
      hb_font_set_synthetic_slant (font, 0);
    }
    иначе
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  void recurse (FT_OpaquePaint paint)
  {
    если (unlikely (depth_left <= 0 || edge_count <= 0)) вернуть;
    depth_left--;
    edge_count--;
    _hb_ft_paint (this, paint);
    depth_left++;
  }

  const hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  void *data;
  hb_array_t<const FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  целое depth_left = HB_MAX_NESTING_LEVEL;
  целое edge_count = HB_MAX_GRAPH_EDGE_COUNT;
};

static unsigned
_hb_ft_color_line_get_color_stops (hb_color_line_t *color_line,
				   void *color_line_data,
				   unsigned целое start,
				   unsigned целое *count,
				   hb_color_stop_t *color_stops,
				   void *user_data)
{
  FT_ColorLine *cl = (FT_ColorLine *) color_line_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  если (count && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    если (start >= cl->color_stop_iterator.num_color_stops)
    {
      *count = 0;
      вернуть cl->color_stop_iterator.num_color_stops;
    }

    пока (cl->color_stop_iterator.current_color_stop < start)
    {
      если (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        вернуть cl->color_stop_iterator.num_color_stops;
      }
    }

    пока (wrote < *count &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      если (sizeof stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      иначе
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      если (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      иначе
      {
	hb_color_t color;
        если (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	иначе если (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	иначе
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *count = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  вернуть cl->color_stop_iterator.num_color_stops;
}

static hb_paint_extend_t
_hb_ft_color_line_get_extend (hb_color_line_t *color_line,
			      void *color_line_data,
			      void *user_data)
{
  FT_ColorLine *c = (FT_ColorLine *) color_line_data;
  switch (c->extend)
  {
    default:
    case FT_COLR_PAINT_EXTEND_PAD:     вернуть HB_PAINT_EXTEND_PAD;
    case FT_COLR_PAINT_EXTEND_REPEAT:  вернуть HB_PAINT_EXTEND_REPEAT;
    case FT_COLR_PAINT_EXTEND_REFLECT: вернуть HB_PAINT_EXTEND_REFLECT;
  }
}

static void
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			const FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  если (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    вернуть;
  }

  если (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    вернуть;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

void
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  если (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    вернуть;

  switch (paint.format)
  {
    case FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_node_t node (c->layers_decycler);
      пока (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	если (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  continue;

	c->recurse (other_paint);
      }
    }
    break;
    case FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    break;
    case FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_line_t cl = {
	&paint.u.linear_gradient.colorline,
	_hb_ft_color_line_get_color_stops, c,
	_hb_ft_color_line_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    break;
    case FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_line_t cl = {
	&paint.u.linear_gradient.colorline,
	_hb_ft_color_line_get_color_stops, c,
	_hb_ft_color_line_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    break;
    case FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_line_t cl = {
	&paint.u.linear_gradient.colorline,
	_hb_ft_color_line_get_color_stops, c,
	_hb_ft_color_line_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    break;
    case FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      если (likely (c->depth_left > 0 && c->edge_count > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_count--;
	c->funcs->push_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->pop_transform (c->data);
	break;
      }

      c->funcs->push_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->push_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->push_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->pop_transform (c->data);
      c->funcs->pop_clip (c->data);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_node_t node (c->glyphs_decycler);
      если (unlikely (!node.visit (gid)))
	вернуть;

      c->funcs->push_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      если (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->pop_transform (c->data);
	вернуть;
      }
      c->ft_font->lock.lock ();
      c->funcs->pop_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      если (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_ROOT_TRANSFORM,
				    &other_paint))
      {
        bool has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        если (has_clip_box)
	{
	  float xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->push_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        если (has_clip_box)
          c->funcs->pop_clip (c->data);
      }
    }
    break;
    case FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->push_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      float dx = paint.u.translate.dx / 65536.f;
      float dy = paint.u.translate.dy / 65536.f;

      c->funcs->push_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_SCALE:
    {
      float dx = paint.u.scale.center_x / 65536.f;
      float dy = paint.u.scale.center_y / 65536.f;
      float sx = paint.u.scale.scale_x / 65536.f;
      float sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->push_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_ROTATE:
    {
      float dx = paint.u.rotate.center_x / 65536.f;
      float dy = paint.u.rotate.center_y / 65536.f;
      float a = paint.u.rotate.angle / 65536.f;

      c->funcs->push_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_SKEW:
    {
      float dx = paint.u.skew.center_x / 65536.f;
      float dy = paint.u.skew.center_y / 65536.f;
      float sx = paint.u.skew.x_skew_angle / 65536.f;
      float sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->push_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->pop_transform (c->data);
    }
    break;
    case FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->push_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->push_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->pop_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->pop_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    break;

    case FT_COLR_PAINT_FORMAT_MAX: break;
    default: HB_FALLTHROUGH;
    case FT_COLR_PAINTFORMAT_UNSUPPORTED: break;
  }
}


static bool
hb_ft_paint_glyph_colr (hb_font_t *font,
			void *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, void *paint_data,
			unsigned целое palette_index,
			hb_color_t foreground,
			void *user_data)
{
  const hb_ft_font_t *ft_font = (const hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = NULL;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (void) FT_Palette_Data_Get(ft_face, &palette_data);
  (void) FT_Palette_Select(ft_face, palette_index, &palette);
  если (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (void) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((const FT_Color *) palette,
				 palette ? palette_data.num_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  если (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ROOT_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_node_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    bool clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    bool is_bounded = clip;
    если (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_node_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->push_font_transform (c.data, font);

    если (clip)
    {
      float xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->push_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    если (is_bounded)
      c.recurse (paint);

    если (clip)
      c.funcs->pop_clip (c.data);

    c.funcs->pop_transform (c.data);

    вернуть true;
  }

  /* COLRv0 */
  iterator.p  = NULL;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  если (palette && have_layers)
  {
    do
    {
      hb_bool_t is_foreground = true;
      hb_color_t color = foreground;

      если ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = false;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } пока (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    вернуть true;
  }

  вернуть false;
}


#endif /* HB_FT_COLR_HH */
