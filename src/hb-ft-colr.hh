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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-decycler.hh"
#अन्तर्भाव "hb-paint-bounded.hh"

#अन्तर्भाव FT_COLOR_H


स्थिर hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  विकल्प (mode)
  {
    पक्ष FT_COLR_COMPOSITE_CLEAR:          पटिनिवत्त HB_PAINT_COMPOSITE_MODE_CLEAR;
    पक्ष FT_COLR_COMPOSITE_SRC:            पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SRC;
    पक्ष FT_COLR_COMPOSITE_DEST:           पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DEST;
    पक्ष FT_COLR_COMPOSITE_SRC_OVER:       पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    पक्ष FT_COLR_COMPOSITE_DEST_OVER:      पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    पक्ष FT_COLR_COMPOSITE_SRC_IN:         पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SRC_IN;
    पक्ष FT_COLR_COMPOSITE_DEST_IN:        पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DEST_IN;
    पक्ष FT_COLR_COMPOSITE_SRC_OUT:        पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    पक्ष FT_COLR_COMPOSITE_DEST_OUT:       पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    पक्ष FT_COLR_COMPOSITE_SRC_ATOP:       पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    पक्ष FT_COLR_COMPOSITE_DEST_ATOP:      पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    पक्ष FT_COLR_COMPOSITE_XOR:            पटिनिवत्त HB_PAINT_COMPOSITE_MODE_XOR;
    पक्ष FT_COLR_COMPOSITE_PLUS:           पटिनिवत्त HB_PAINT_COMPOSITE_MODE_PLUS;
    पक्ष FT_COLR_COMPOSITE_SCREEN:         पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SCREEN;
    पक्ष FT_COLR_COMPOSITE_OVERLAY:        पटिनिवत्त HB_PAINT_COMPOSITE_MODE_OVERLAY;
    पक्ष FT_COLR_COMPOSITE_DARKEN:         पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DARKEN;
    पक्ष FT_COLR_COMPOSITE_LIGHTEN:        पटिनिवत्त HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    पक्ष FT_COLR_COMPOSITE_COLOR_DODGE:    पटिनिवत्त HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    पक्ष FT_COLR_COMPOSITE_COLOR_BURN:     पटिनिवत्त HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    पक्ष FT_COLR_COMPOSITE_HARD_LIGHT:     पटिनिवत्त HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    पक्ष FT_COLR_COMPOSITE_SOFT_LIGHT:     पटिनिवत्त HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    पक्ष FT_COLR_COMPOSITE_DIFFERENCE:     पटिनिवत्त HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    पक्ष FT_COLR_COMPOSITE_EXCLUSION:      पटिनिवत्त HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    पक्ष FT_COLR_COMPOSITE_MULTIPLY:       पटिनिवत्त HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    पक्ष FT_COLR_COMPOSITE_HSL_HUE:        पटिनिवत्त HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    पक्ष FT_COLR_COMPOSITE_HSL_SATURATION: पटिनिवत्त HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    पक्ष FT_COLR_COMPOSITE_HSL_COLOR:      पटिनिवत्त HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    पक्ष FT_COLR_COMPOSITE_HSL_LUMINOSITY: पटिनिवत्त HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    पक्ष FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    स्वभाव:                               पटिनिवत्त HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

प्रकारनिर्देश संरचना hb_ft_paint_context_t hb_ft_paint_context_t;

स्थिर शून्य
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

स्थिर शून्य
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ध्रुव FT_ClipBox *clip_box,
			 भिन्न *xmin, भिन्न *ymin,
			 भिन्न *xmax, भिन्न *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  भिन्न upem = font->face->get_upem ();
  भिन्न xscale = upem / (font->x_scale ? font->x_scale : upem);
  भिन्न yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_वामः.x * xscale;
  *ymin = clip_box->bottom_वामः.y * yscale;
  *xmax = clip_box->शिखरम्_दक्षिणः.x * xscale;
  *ymax = clip_box->शिखरम्_दक्षिणः.y * yscale;
}

स्थिर unsigned
_hb_ft_color_alpha (unsigned alpha, unsigned alpha_mult)
{
  पटिनिवत्त (alpha * alpha_mult + (1 << 13)) >> 14;
}

संरचना hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ध्रुव hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			 hb_array_t<ध्रुव FT_Color> palette,
			 unsigned palette_index,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), data (paint_data),
    palette (palette), palette_index (palette_index), foreground (foreground)
  {
    सचे (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, सच्च);
      hb_font_set_synthetic_slant (font, 0);
    }
    अञ्ञथा
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  शून्य recurse (FT_OpaquePaint paint)
  {
    सचे (unlikely (depth_वामः <= 0 || edge_गणनम् <= 0)) पटिनिवत्त;
    depth_वामः--;
    edge_गणनम्--;
    _hb_ft_paint (एतत्, paint);
    depth_वामः++;
  }

  ध्रुव hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  शून्य *data;
  hb_array_t<ध्रुव FT_Color> palette;
  unsigned palette_index;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  पूर्णाङ्क depth_वामः = HB_MAX_NESTING_LEVEL;
  पूर्णाङ्क edge_गणनम् = HB_MAX_GRAPH_EDGE_गणनम्;
};

स्थिर unsigned
hb_ft_color_पङ्क्तिः_get_color_stops (hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
				   शून्य *color_पङ्क्तिः_data,
				   unsigned पूर्णाङ्क start,
				   unsigned पूर्णाङ्क *गणनम्,
				   hb_color_stop_t *color_stops,
				   शून्य *user_data)
{
  FT_Color_पङ्क्तिः *cl = (FT_Color_पङ्क्तिः *) color_पङ्क्तिः_data;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_data;

  सचे (गणनम् && color_stops)
  {
    FT_ColorStop stop;
    unsigned wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    सचे (start >= cl->color_stop_iterator.अङ्कः_color_stops)
    {
      *गणनम् = 0;
      पटिनिवत्त cl->color_stop_iterator.अङ्कः_color_stops;
    }

    याव (cl->color_stop_iterator.current_color_stop < start)
    {
      सचे (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        पटिनिवत्त cl->color_stop_iterator.अङ्कः_color_stops;
      }
    }

    याव (wrote < *गणनम् &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      सचे (परिमाण stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      अञ्ञथा
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_index == 0xFFFF;
      सचे (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      अञ्ञथा
      {
	hb_color_t color;
        सचे (c->funcs->custom_palette_color (c->data, stop.color.palette_index, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	अञ्ञथा सचे (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_index];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	अञ्ञथा
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *गणनम् = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  पटिनिवत्त cl->color_stop_iterator.अङ्कः_color_stops;
}

स्थिर hb_paint_extend_t
hb_ft_color_पङ्क्तिः_get_extend (hb_color_पङ्क्तिः_t *color_पङ्क्तिः,
			      शून्य *color_पङ्क्तिः_data,
			      शून्य *user_data)
{
  FT_Color_पङ्क्तिः *c = (FT_Color_पङ्क्तिः *) color_पङ्क्तिः_data;
  विकल्प (c->extend)
  {
    स्वभाव:
    पक्ष FT_COLR_PAINT_EXTEND_PAD:     पटिनिवत्त HB_PAINT_EXTEND_PAD;
    पक्ष FT_COLR_PAINT_EXTEND_REPEAT:  पटिनिवत्त HB_PAINT_EXTEND_REPEAT;
    पक्ष FT_COLR_PAINT_EXTEND_REFLECT: पटिनिवत्त HB_PAINT_EXTEND_REFLECT;
  }
}

स्थिर शून्य
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ध्रुव FT_ColorIndex &color_index,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_index.palette_index == 0xFFFF;

  सचे (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_index.alpha));
    पटिनिवत्त;
  }

  सचे (c->funcs->custom_palette_color (c->data, color_index.palette_index, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_index.alpha));
    पटिनिवत्त;
  }

  FT_Color ft_color = c->palette[color_index.palette_index];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_index.alpha));
}

शून्य
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  सचे (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    पटिनिवत्त;

  विकल्प (paint.format)
  {
    पक्ष FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_ग्रन्थिः_t node (c->layers_decycler);
      याव (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	सचे (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  अनुवर्त;

	c->recurse (other_paint);
      }
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->data, is_foreground, color);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_पङ्क्तिः_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पङ्क्तिः_get_color_stops, c,
	hb_ft_color_पङ्क्तिः_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->data, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_पङ्क्तिः_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पङ्क्तिः_get_color_stops, c,
	hb_ft_color_पङ्क्तिः_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->data, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_पङ्क्तिः_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पङ्क्तिः_get_color_stops, c,
	hb_ft_color_पङ्क्तिः_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->data, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      सचे (likely (c->depth_वामः > 0 && c->edge_गणनम् > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_गणनम्--;
	c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->data, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->निष्कासय_transform (c->data);
	भङ्ग;
      }

      c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->प्रक्षिप_clip_glyph (c->data, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->प्रक्षिप_font_transform (c->data, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->निष्कासय_transform (c->data);
      c->funcs->निष्कासय_clip (c->data);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_ग्रन्थिः_t node (c->glyphs_decycler);
      सचे (unlikely (!node.visit (gid)))
	पटिनिवत्त;

      c->funcs->प्रक्षिप_inverse_font_transform (c->data, c->font);
      c->ft_font->lock.unlock ();
      सचे (c->funcs->color_glyph (c->data, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->निष्कासय_transform (c->data);
	पटिनिवत्त;
      }
      c->ft_font->lock.lock ();
      c->funcs->निष्कासय_transform (c->data);

      FT_OpaquePaint other_paint = {0};
      सचे (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_मूलम्_TRANSFORM,
				    &other_paint))
      {
        तार्किक has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        सचे (has_clip_box)
	{
	  भिन्न xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->प्रक्षिप_clip_rectangle (c->data, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        सचे (has_clip_box)
          c->funcs->निष्कासय_clip (c->data);
      }
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->प्रक्षिप_transform (c->data,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      भिन्न dx = paint.u.translate.dx / 65536.f;
      भिन्न dy = paint.u.translate.dy / 65536.f;

      c->funcs->प्रक्षिप_translate (c->data, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_SCALE:
    {
      भिन्न dx = paint.u.scale.center_x / 65536.f;
      भिन्न dy = paint.u.scale.center_y / 65536.f;
      भिन्न sx = paint.u.scale.scale_x / 65536.f;
      भिन्न sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->प्रक्षिप_scale_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_ROTATE:
    {
      भिन्न dx = paint.u.rotate.center_x / 65536.f;
      भिन्न dy = paint.u.rotate.center_y / 65536.f;
      भिन्न a = paint.u.rotate.angle / 65536.f;

      c->funcs->प्रक्षिप_rotate_around_center (c->data, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_SKEW:
    {
      भिन्न dx = paint.u.skew.center_x / 65536.f;
      भिन्न dy = paint.u.skew.center_y / 65536.f;
      भिन्न sx = paint.u.skew.x_skew_angle / 65536.f;
      भिन्न sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->प्रक्षिप_skew_around_center (c->data, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->निष्कासय_transform (c->data);
    }
    भङ्ग;
    पक्ष FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->प्रक्षिप_group (c->data);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->प्रक्षिप_group (c->data);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->निष्कासय_group (c->data, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->निष्कासय_group (c->data, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    भङ्ग;

    पक्ष FT_COLR_PAINT_FORMAT_MAX: भङ्ग;
    स्वभाव: HB_FALLTHROUGH;
    पक्ष FT_COLR_PAINTFORMAT_UNSUPPORTED: भङ्ग;
  }
}


स्थिर तार्किक
hb_ft_paint_glyph_colr (hb_font_t *font,
			शून्य *font_data,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, शून्य *paint_data,
			unsigned पूर्णाङ्क palette_index,
			hb_color_t foreground,
			शून्य *user_data)
{
  ध्रुव hb_ft_font_t *ft_font = (ध्रुव hb_ft_font_t *) font_data;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_Data   palette_data = {};
  FT_Color*         palette = रिक्त;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_index;
  FT_UInt  layer_color_index;

  (शून्य) FT_Palette_Data_Get(ft_face, &palette_data);
  (शून्य) FT_Palette_Select(ft_face, palette_index, &palette);
  सचे (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (शून्य) FT_Palette_Select(ft_face, 0, &palette);
  }

  auto palette_array = hb_array ((ध्रुव FT_Color *) palette,
				 palette ? palette_data.अङ्कः_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  सचे (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_मूलम्_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_data,
			     palette_array, palette_index, foreground);
    hb_decycler_ग्रन्थिः_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    तार्किक clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    तार्किक is_bounded = clip;
    सचे (!is_bounded)
    {
      auto *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_data;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_data,
			        palette_array, palette_index, foreground);
      hb_decycler_ग्रन्थिः_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_data.is_bounded ();
    }

    c.funcs->प्रक्षिप_font_transform (c.data, font);

    सचे (clip)
    {
      भिन्न xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->प्रक्षिप_clip_rectangle (c.data, xmin, ymin, xmax, ymax);
    }

    सचे (is_bounded)
      c.recurse (paint);

    सचे (clip)
      c.funcs->निष्कासय_clip (c.data);

    c.funcs->निष्कासय_transform (c.data);

    पटिनिवत्त सच्च;
  }

  /* COLRv0 */
  iterator.p  = रिक्त;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_index,
					 &layer_color_index,
					 &iterator);

  सचे (palette && have_layers)
  {
    करोहि
    {
      hb_bool_t is_foreground = सच्च;
      hb_color_t color = foreground;

      सचे ( layer_color_index != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_index];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = मिच्छा;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_data, layer_glyph_index, font, is_foreground, color);
      ft_font->lock.lock ();

    } याव (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_index,
				      &layer_color_index,
				      &iterator));
    पटिनिवत्त सच्च;
  }

  पटिनिवत्त मिच्छा;
}


#endif /* HB_FT_COLR_HH */
