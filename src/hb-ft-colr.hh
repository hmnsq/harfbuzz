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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-decycler.hh"
#അന്തര്ഭാവ "hb-paint-bounded.hh"

#അന്തര്ഭാവ FT_COLOR_H


സ്ഥിതം hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  തിരഞ്ഞെടുപ്പ് (mode)
  {
    അവസ്ഥ FT_COLR_COMPOSITE_CLEAR:          തിരികെ HB_PAINT_COMPOSITE_MODE_CLEAR;
    അവസ്ഥ FT_COLR_COMPOSITE_SRC:            തിരികെ HB_PAINT_COMPOSITE_MODE_SRC;
    അവസ്ഥ FT_COLR_COMPOSITE_DEST:           തിരികെ HB_PAINT_COMPOSITE_MODE_DEST;
    അവസ്ഥ FT_COLR_COMPOSITE_SRC_OVER:       തിരികെ HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    അവസ്ഥ FT_COLR_COMPOSITE_DEST_OVER:      തിരികെ HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    അവസ്ഥ FT_COLR_COMPOSITE_SRC_IN:         തിരികെ HB_PAINT_COMPOSITE_MODE_SRC_IN;
    അവസ്ഥ FT_COLR_COMPOSITE_DEST_IN:        തിരികെ HB_PAINT_COMPOSITE_MODE_DEST_IN;
    അവസ്ഥ FT_COLR_COMPOSITE_SRC_OUT:        തിരികെ HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    അവസ്ഥ FT_COLR_COMPOSITE_DEST_OUT:       തിരികെ HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    അവസ്ഥ FT_COLR_COMPOSITE_SRC_ATOP:       തിരികെ HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    അവസ്ഥ FT_COLR_COMPOSITE_DEST_ATOP:      തിരികെ HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    അവസ്ഥ FT_COLR_COMPOSITE_XOR:            തിരികെ HB_PAINT_COMPOSITE_MODE_XOR;
    അവസ്ഥ FT_COLR_COMPOSITE_PLUS:           തിരികെ HB_PAINT_COMPOSITE_MODE_PLUS;
    അവസ്ഥ FT_COLR_COMPOSITE_SCREEN:         തിരികെ HB_PAINT_COMPOSITE_MODE_SCREEN;
    അവസ്ഥ FT_COLR_COMPOSITE_OVERLAY:        തിരികെ HB_PAINT_COMPOSITE_MODE_OVERLAY;
    അവസ്ഥ FT_COLR_COMPOSITE_DARKEN:         തിരികെ HB_PAINT_COMPOSITE_MODE_DARKEN;
    അവസ്ഥ FT_COLR_COMPOSITE_LIGHTEN:        തിരികെ HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    അവസ്ഥ FT_COLR_COMPOSITE_COLOR_DODGE:    തിരികെ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    അവസ്ഥ FT_COLR_COMPOSITE_COLOR_BURN:     തിരികെ HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    അവസ്ഥ FT_COLR_COMPOSITE_HARD_LIGHT:     തിരികെ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    അവസ്ഥ FT_COLR_COMPOSITE_SOFT_LIGHT:     തിരികെ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    അവസ്ഥ FT_COLR_COMPOSITE_DIFFERENCE:     തിരികെ HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    അവസ്ഥ FT_COLR_COMPOSITE_EXCLUSION:      തിരികെ HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    അവസ്ഥ FT_COLR_COMPOSITE_MULTIPLY:       തിരികെ HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    അവസ്ഥ FT_COLR_COMPOSITE_HSL_HUE:        തിരികെ HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    അവസ്ഥ FT_COLR_COMPOSITE_HSL_SATURATION: തിരികെ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    അവസ്ഥ FT_COLR_COMPOSITE_HSL_COLOR:      തിരികെ HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    അവസ്ഥ FT_COLR_COMPOSITE_HSL_LUMINOSITY: തിരികെ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    അവസ്ഥ FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    സ്ഥിരസ്ഥിതി:                               തിരികെ HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

തരനിർവചനം ഘടന hb_ft_paint_context_t hb_ft_paint_context_t;

സ്ഥിതം ശൂന്യം
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

സ്ഥിതം ശൂന്യം
_hb_ft_unscale_clip_box (hb_font_t *font,
			 സ്ഥിരം FT_ClipBox *clip_box,
			 ദശാംശം *xmin, ദശാംശം *ymin,
			 ദശാംശം *xmax, ദശാംശം *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  ദശാംശം upem = font->face->get_upem ();
  ദശാംശം xscale = upem / (font->x_scale ? font->x_scale : upem);
  ദശാംശം yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ഇടത്.x * xscale;
  *ymin = clip_box->bottom_ഇടത്.y * yscale;
  *xmax = clip_box->മുകൾ_വലത്.x * xscale;
  *ymax = clip_box->മുകൾ_വലത്.y * yscale;
}

സ്ഥിതം unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  തിരികെ (alpha * alpha_mult + (1 << 13)) >> 14;
}

ഘടന hb_ft_paint_context_t
{
  hb_ft_paint_context_t (സ്ഥിരം hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, ശൂന്യം *paint_data,
			 hb_array_t<സ്ഥിരം FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    എങ്കിൽ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, സത്യം);
      hb_font_set_synthetic_slant (font, 0);
    }
    അല്ലെങ്കിൽ
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ശൂന്യം recurse (FT_OpaquePaint paint)
  {
    എങ്കിൽ (unlikely (depth_ഇടത് <= 0 || edge_എണ്ണം <= 0)) തിരികെ;
    depth_ഇടത്--;
    edge_എണ്ണം--;
    _hb_ft_paint (ഇത്, paint);
    depth_ഇടത്++;
  }

  സ്ഥിരം hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  ശൂന്യം *data;
  hb_array_t<സ്ഥിരം FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  പൂർണ്ണസംഖ്യ depth_ഇടത് = HB_MAX_NESTING_LEVEL;
  പൂർണ്ണസംഖ്യ edge_എണ്ണം = HB_MAX_GRAPH_EDGE_എണ്ണം;
};

സ്ഥിതം unsigned
hb_ft_color_വരി_get_color_stops (hb_color_വരി_t *color_വരി,
				   ശൂന്യം *color_വരി_data,
				   unsigned പൂർണ്ണസംഖ്യ start,
				   unsigned പൂർണ്ണസംഖ്യ *എണ്ണം,
				   hb_color_stop_t *color_stops,
				   ശൂന്യം *user_data)
{
  FT_Color_വരി *cl = (FT_Color_വരി *) color_വരി_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  എങ്കിൽ (എണ്ണം && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    എങ്കിൽ (start >= cl->color_stop_iterator.അക്കം_color_stops)
    {
      *എണ്ണം = 0;
      തിരികെ cl->color_stop_iterator.അക്കം_color_stops;
    }

    ആകുമ്പോൾ (cl->color_stop_iterator.current_color_stop < start)
    {
      എങ്കിൽ (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        തിരികെ cl->color_stop_iterator.അക്കം_color_stops;
      }
    }

    ആകുമ്പോൾ (wrote < *എണ്ണം &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      എങ്കിൽ (വലുപ്പം stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      അല്ലെങ്കിൽ
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      എങ്കിൽ (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      അല്ലെങ്കിൽ
      {
	hb_color_t color;
        എങ്കിൽ (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	അല്ലെങ്കിൽ എങ്കിൽ (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	അല്ലെങ്കിൽ
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *എണ്ണം = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  തിരികെ cl->color_stop_iterator.അക്കം_color_stops;
}

സ്ഥിതം hb_paint_extend_t
hb_ft_color_വരി_get_extend (hb_color_വരി_t *color_വരി,
			      ശൂന്യം *color_വരി_data,
			      ശൂന്യം *user_data)
{
  FT_Color_വരി *c = (FT_Color_വരി *) color_വരി_data;
  തിരഞ്ഞെടുപ്പ് (c->extend)
  {
    സ്ഥിരസ്ഥിതി:
    അവസ്ഥ FT_COLR_PAINT_EXTEND_PAD:     തിരികെ HB_PAINT_EXTEND_PAD;
    അവസ്ഥ FT_COLR_PAINT_EXTEND_REPEAT:  തിരികെ HB_PAINT_EXTEND_REPEAT;
    അവസ്ഥ FT_COLR_PAINT_EXTEND_REFLECT: തിരികെ HB_PAINT_EXTEND_REFLECT;
  }
}

സ്ഥിതം ശൂന്യം
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			സ്ഥിരം FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  എങ്കിൽ (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    തിരികെ;
  }

  എങ്കിൽ (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    തിരികെ;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

ശൂന്യം
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  എങ്കിൽ (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    തിരികെ;

  തിരഞ്ഞെടുപ്പ് (paint.format)
  {
    അവസ്ഥ FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_കണ്ണി_t node (c->layers_decycler);
      ആകുമ്പോൾ (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	എങ്കിൽ (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  തുടരുക;

	c->recurse (other_paint);
      }
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_വരി_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_വരി_get_color_stops, c,
	hb_ft_color_വരി_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_വരി_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_വരി_get_color_stops, c,
	hb_ft_color_വരി_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_വരി_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_വരി_get_color_stops, c,
	hb_ft_color_വരി_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      എങ്കിൽ (likely (c->depth_ഇടത് > 0 && c->edge_എണ്ണം > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_എണ്ണം--;
	c->funcs->തള്ളുക_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->എടുക്കുക_transform (c->data);
	നിർത്തുക;
      }

      c->funcs->തള്ളുക_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->തള്ളുക_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->തള്ളുക_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->എടുക്കുക_transform (c->data);
      c->funcs->എടുക്കുക_clip (c->data);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_കണ്ണി_t node (c->glyphs_decycler);
      എങ്കിൽ (unlikely (!node.visit (gid)))
	തിരികെ;

      c->funcs->തള്ളുക_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      എങ്കിൽ (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->എടുക്കുക_transform (c->data);
	തിരികെ;
      }
      c->ft_font->lock.lock ();
      c->funcs->എടുക്കുക_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      എങ്കിൽ (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_വേര്_TRANSFORM,
				    &other_paint))
      {
        തർക്കം has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        എങ്കിൽ (has_clip_box)
	{
	  ദശാംശം xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->തള്ളുക_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        എങ്കിൽ (has_clip_box)
          c->funcs->എടുക്കുക_clip (c->data);
      }
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->തള്ളുക_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      ദശാംശം dx = paint.u.translate.dx / 65536.f;
      ദശാംശം dy = paint.u.translate.dy / 65536.f;

      c->funcs->തള്ളുക_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_SCALE:
    {
      ദശാംശം dx = paint.u.scale.center_x / 65536.f;
      ദശാംശം dy = paint.u.scale.center_y / 65536.f;
      ദശാംശം sx = paint.u.scale.scale_x / 65536.f;
      ദശാംശം sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->തള്ളുക_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_ROTATE:
    {
      ദശാംശം dx = paint.u.rotate.center_x / 65536.f;
      ദശാംശം dy = paint.u.rotate.center_y / 65536.f;
      ദശാംശം a = paint.u.rotate.angle / 65536.f;

      c->funcs->തള്ളുക_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_SKEW:
    {
      ദശാംശം dx = paint.u.skew.center_x / 65536.f;
      ദശാംശം dy = paint.u.skew.center_y / 65536.f;
      ദശാംശം sx = paint.u.skew.x_skew_angle / 65536.f;
      ദശാംശം sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->തള്ളുക_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->എടുക്കുക_transform (c->data);
    }
    നിർത്തുക;
    അവസ്ഥ FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->തള്ളുക_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->തള്ളുക_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->എടുക്കുക_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->എടുക്കുക_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    നിർത്തുക;

    അവസ്ഥ FT_COLR_PAINT_FORMAT_MAX: നിർത്തുക;
    സ്ഥിരസ്ഥിതി: HB_FALLTHROUGH;
    അവസ്ഥ FT_COLR_PAINTFORMAT_UNSUPPORTED: നിർത്തുക;
  }
}


സ്ഥിതം തർക്കം
hb_ft_paint_glyph_colr (hb_font_t *font,
			ശൂന്യം *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, ശൂന്യം *paint_data,
			unsigned പൂർണ്ണസംഖ്യ palette_index,
			hb_color_t foreground,
			ശൂന്യം *user_data)
{
  സ്ഥിരം hb_ft_font_t *ft_font = (സ്ഥിരം hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = ശൂന്യ;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (ശൂന്യം) FT_Palette_Data_Get(ft_face, &palette_data);
  (ശൂന്യം) FT_Palette_Select(ft_face, palette_index, &palette);
  എങ്കിൽ (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (ശൂന്യം) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((സ്ഥിരം FT_Color *) palette,
				 palette ? palette_data.അക്കം_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  എങ്കിൽ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_വേര്_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_കണ്ണി_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    തർക്കം clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    തർക്കം is_bounded = clip;
    എങ്കിൽ (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_കണ്ണി_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->തള്ളുക_font_transform (c.data, font);

    എങ്കിൽ (clip)
    {
      ദശാംശം xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->തള്ളുക_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    എങ്കിൽ (is_bounded)
      c.recurse (paint);

    എങ്കിൽ (clip)
      c.funcs->എടുക്കുക_clip (c.data);

    c.funcs->എടുക്കുക_transform (c.data);

    തിരികെ സത്യം;
  }

  /* COLRv0 */
  iterator.p  = ശൂന്യ;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  എങ്കിൽ (palette && have_layers)
  {
    ചെയ്യുക
    {
      hb_bool_t is_foreground = സത്യം;
      hb_color_t color = foreground;

      എങ്കിൽ ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = അസത്യം;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } ആകുമ്പോൾ (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    തിരികെ സത്യം;
  }

  തിരികെ അസത്യം;
}


#endif /* HB_FT_COLR_HH */
