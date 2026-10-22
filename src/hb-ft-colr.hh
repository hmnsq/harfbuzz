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

#अगर_अघोषित HB_FT_COLR_HH
#घोषणा HB_FT_COLR_HH

#समावेश "hb.hh"

#समावेश "hb-decycler.hh"
#समावेश "hb-paint-bounded.hh"

#समावेश FT_COLOR_H


अटल hb_paint_composite_mode_t
_hb_ft_paint_composite_mode (FT_Composite_Mode mode)
{
  चयन (mode)
  {
    हाल FT_COLR_COMPOSITE_CLEAR:          वापस HB_PAINT_COMPOSITE_MODE_CLEAR;
    हाल FT_COLR_COMPOSITE_SRC:            वापस HB_PAINT_COMPOSITE_MODE_SRC;
    हाल FT_COLR_COMPOSITE_DEST:           वापस HB_PAINT_COMPOSITE_MODE_DEST;
    हाल FT_COLR_COMPOSITE_SRC_OVER:       वापस HB_PAINT_COMPOSITE_MODE_SRC_OVER;
    हाल FT_COLR_COMPOSITE_DEST_OVER:      वापस HB_PAINT_COMPOSITE_MODE_DEST_OVER;
    हाल FT_COLR_COMPOSITE_SRC_IN:         वापस HB_PAINT_COMPOSITE_MODE_SRC_IN;
    हाल FT_COLR_COMPOSITE_DEST_IN:        वापस HB_PAINT_COMPOSITE_MODE_DEST_IN;
    हाल FT_COLR_COMPOSITE_SRC_OUT:        वापस HB_PAINT_COMPOSITE_MODE_SRC_OUT;
    हाल FT_COLR_COMPOSITE_DEST_OUT:       वापस HB_PAINT_COMPOSITE_MODE_DEST_OUT;
    हाल FT_COLR_COMPOSITE_SRC_ATOP:       वापस HB_PAINT_COMPOSITE_MODE_SRC_ATOP;
    हाल FT_COLR_COMPOSITE_DEST_ATOP:      वापस HB_PAINT_COMPOSITE_MODE_DEST_ATOP;
    हाल FT_COLR_COMPOSITE_XOR:            वापस HB_PAINT_COMPOSITE_MODE_XOR;
    हाल FT_COLR_COMPOSITE_PLUS:           वापस HB_PAINT_COMPOSITE_MODE_PLUS;
    हाल FT_COLR_COMPOSITE_SCREEN:         वापस HB_PAINT_COMPOSITE_MODE_SCREEN;
    हाल FT_COLR_COMPOSITE_OVERLAY:        वापस HB_PAINT_COMPOSITE_MODE_OVERLAY;
    हाल FT_COLR_COMPOSITE_DARKEN:         वापस HB_PAINT_COMPOSITE_MODE_DARKEN;
    हाल FT_COLR_COMPOSITE_LIGHTEN:        वापस HB_PAINT_COMPOSITE_MODE_LIGHTEN;
    हाल FT_COLR_COMPOSITE_COLOR_DODGE:    वापस HB_PAINT_COMPOSITE_MODE_COLOR_DODGE;
    हाल FT_COLR_COMPOSITE_COLOR_BURN:     वापस HB_PAINT_COMPOSITE_MODE_COLOR_BURN;
    हाल FT_COLR_COMPOSITE_HARD_LIGHT:     वापस HB_PAINT_COMPOSITE_MODE_HARD_LIGHT;
    हाल FT_COLR_COMPOSITE_SOFT_LIGHT:     वापस HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT;
    हाल FT_COLR_COMPOSITE_DIFFERENCE:     वापस HB_PAINT_COMPOSITE_MODE_DIFFERENCE;
    हाल FT_COLR_COMPOSITE_EXCLUSION:      वापस HB_PAINT_COMPOSITE_MODE_EXCLUSION;
    हाल FT_COLR_COMPOSITE_MULTIPLY:       वापस HB_PAINT_COMPOSITE_MODE_MULTIPLY;
    हाल FT_COLR_COMPOSITE_HSL_HUE:        वापस HB_PAINT_COMPOSITE_MODE_HSL_HUE;
    हाल FT_COLR_COMPOSITE_HSL_SATURATION: वापस HB_PAINT_COMPOSITE_MODE_HSL_SATURATION;
    हाल FT_COLR_COMPOSITE_HSL_COLOR:      वापस HB_PAINT_COMPOSITE_MODE_HSL_COLOR;
    हाल FT_COLR_COMPOSITE_HSL_LUMINOSITY: वापस HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY;

    हाल FT_COLR_COMPOSITE_MAX:            HB_FALLTHROUGH;
    शेष:                               वापस HB_PAINT_COMPOSITE_MODE_CLEAR;
  }
}

प्रकार काष्ठा hb_ft_paint_context_t hb_ft_paint_context_t;

अटल व्योम
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

अटल व्योम
_hb_ft_unscale_clip_box (hb_font_t *font,
			 स्थिर FT_ClipBox *clip_box,
			 भग्न *xmin, भग्न *ymin,
			 भग्न *xmax, भग्न *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  भग्न upem = font->face->get_upem ();
  भग्न xscale = upem / (font->x_scale ? font->x_scale : upem);
  भग्न yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_बायाँ.x * xscale;
  *ymin = clip_box->bottom_बायाँ.y * yscale;
  *xmax = clip_box->शीर्ष_दायाँ.x * xscale;
  *ymax = clip_box->शीर्ष_दायाँ.y * yscale;
}

अटल अचिन्हित
_hb_ft_color_alpha (अचिन्हित alpha, अचिन्हित alpha_mult)
{
  वापस (alpha * alpha_mult + (1 << 13)) >> 14;
}

काष्ठा hb_ft_paint_context_t
{
  hb_ft_paint_context_t (स्थिर hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
			 hb_array_t<स्थिर FT_Color> palette,
			 अचिन्हित palette_सूचकांक,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), तथ्य (paint_तथ्य),
    palette (palette), palette_सूचकांक (palette_सूचकांक), foreground (foreground)
  {
    अगर (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_set_synthetic_bold (font, 0, 0, true);
      hb_font_set_synthetic_slant (font, 0);
    }
    अन्यथा
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  व्योम recurse (FT_OpaquePaint paint)
  {
    अगर (unlikely (depth_बायाँ <= 0 || edge_गिनती <= 0)) वापस;
    depth_बायाँ--;
    edge_गिनती--;
    _hb_ft_paint (यह, paint);
    depth_बायाँ++;
  }

  स्थिर hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  व्योम *तथ्य;
  hb_array_t<स्थिर FT_Color> palette;
  अचिन्हित palette_सूचकांक;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  पूर्णांक depth_बायाँ = HB_MAX_NESTING_LEVEL;
  पूर्णांक edge_गिनती = HB_MAX_GRAPH_EDGE_गिनती;
};

अटल अचिन्हित
hb_ft_color_पंक्ति_get_color_stops (hb_color_पंक्ति_t *color_पंक्ति,
				   व्योम *color_पंक्ति_तथ्य,
				   अचिन्हित पूर्णांक start,
				   अचिन्हित पूर्णांक *गिनती,
				   hb_color_stop_t *color_stops,
				   व्योम *user_तथ्य)
{
  FT_Color_पंक्ति *cl = (FT_Color_पंक्ति *) color_पंक्ति_तथ्य;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_तथ्य;

  अगर (गिनती && color_stops)
  {
    FT_ColorStop stop;
    अचिन्हित wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    अगर (start >= cl->color_stop_iterator.अंक_color_stops)
    {
      *गिनती = 0;
      वापस cl->color_stop_iterator.अंक_color_stops;
    }

    जबतक (cl->color_stop_iterator.current_color_stop < start)
    {
      अगर (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        वापस cl->color_stop_iterator.अंक_color_stops;
      }
    }

    जबतक (wrote < *गिनती &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      अगर (माप stop.stop_offset == 2)
	color_stops->offset = stop.stop_offset / 16384.f;
      अन्यथा
	color_stops->offset = stop.stop_offset / 65536.f;

      color_stops->is_foreground = stop.color.palette_सूचकांक == 0xFFFF;
      अगर (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      अन्यथा
      {
	hb_color_t color;
        अगर (c->funcs->custom_palette_color (c->तथ्य, stop.color.palette_सूचकांक, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	अन्यथा अगर (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_सूचकांक];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	अन्यथा
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *गिनती = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  वापस cl->color_stop_iterator.अंक_color_stops;
}

अटल hb_paint_extend_t
hb_ft_color_पंक्ति_get_extend (hb_color_पंक्ति_t *color_पंक्ति,
			      व्योम *color_पंक्ति_तथ्य,
			      व्योम *user_तथ्य)
{
  FT_Color_पंक्ति *c = (FT_Color_पंक्ति *) color_पंक्ति_तथ्य;
  चयन (c->extend)
  {
    शेष:
    हाल FT_COLR_PAINT_EXTEND_PAD:     वापस HB_PAINT_EXTEND_PAD;
    हाल FT_COLR_PAINT_EXTEND_REPEAT:  वापस HB_PAINT_EXTEND_REPEAT;
    हाल FT_COLR_PAINT_EXTEND_REFLECT: वापस HB_PAINT_EXTEND_REFLECT;
  }
}

अटल व्योम
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			स्थिर FT_Color_सूचकांक &color_सूचकांक,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_सूचकांक.palette_सूचकांक == 0xFFFF;

  अगर (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_सूचकांक.alpha));
    वापस;
  }

  अगर (c->funcs->custom_palette_color (c->तथ्य, color_सूचकांक.palette_सूचकांक, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_सूचकांक.alpha));
    वापस;
  }

  FT_Color ft_color = c->palette[color_सूचकांक.palette_सूचकांक];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_सूचकांक.alpha));
}

व्योम
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  अगर (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    वापस;

  चयन (paint.format)
  {
    हाल FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_गाँठ_t node (c->layers_decycler);
      जबतक (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	अगर (unlikely (!node.visit ((uintptr_t) other_paint.p)))
	  जारी;

	c->recurse (other_paint);
      }
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->तथ्य, is_foreground, color);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_LINEAR_GRADIENT:
    {
      hb_color_पंक्ति_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पंक्ति_get_color_stops, c,
	hb_ft_color_पंक्ति_get_extend, nullptr
      };

      c->funcs->linear_gradient (c->तथ्य, &cl,
				 paint.u.linear_gradient.p0.x / 65536.f,
				 paint.u.linear_gradient.p0.y / 65536.f,
				 paint.u.linear_gradient.p1.x / 65536.f,
				 paint.u.linear_gradient.p1.y / 65536.f,
				 paint.u.linear_gradient.p2.x / 65536.f,
				 paint.u.linear_gradient.p2.y / 65536.f);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_पंक्ति_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पंक्ति_get_color_stops, c,
	hb_ft_color_पंक्ति_get_extend, nullptr
      };

      c->funcs->radial_gradient (c->तथ्य, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_पंक्ति_t cl = {
	&paint.u.linear_gradient.colorline,
	hb_ft_color_पंक्ति_get_color_stops, c,
	hb_ft_color_पंक्ति_get_extend, nullptr
      };

      c->funcs->sweep_gradient (c->तथ्य, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.start_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.end_angle / 65536.f + 1) * HB_PI);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      अगर (likely (c->depth_बायाँ > 0 && c->edge_गिनती > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->edge_गिनती--;
	c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->तथ्य, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->निकालो_transform (c->तथ्य);
	अवरोध;
      }

      c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->धकेलो_clip_glyph (c->तथ्य, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->धकेलो_font_transform (c->तथ्य, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->निकालो_transform (c->तथ्य);
      c->funcs->निकालो_clip (c->तथ्य);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_गाँठ_t node (c->glyphs_decycler);
      अगर (unlikely (!node.visit (gid)))
	वापस;

      c->funcs->धकेलो_inverse_font_transform (c->तथ्य, c->font);
      c->ft_font->lock.unlock ();
      अगर (c->funcs->color_glyph (c->तथ्य, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->निकालो_transform (c->तथ्य);
	वापस;
      }
      c->ft_font->lock.lock ();
      c->funcs->निकालो_transform (c->तथ्य);

      FT_OpaquePaint other_paint = {0};
      अगर (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_मूल_TRANSFORM,
				    &other_paint))
      {
        bool has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        अगर (has_clip_box)
	{
	  भग्न xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->धकेलो_clip_rectangle (c->तथ्य, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        अगर (has_clip_box)
          c->funcs->निकालो_clip (c->तथ्य);
      }
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->धकेलो_transform (c->तथ्य,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      भग्न dx = paint.u.translate.dx / 65536.f;
      भग्न dy = paint.u.translate.dy / 65536.f;

      c->funcs->धकेलो_translate (c->तथ्य, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_SCALE:
    {
      भग्न dx = paint.u.scale.center_x / 65536.f;
      भग्न dy = paint.u.scale.center_y / 65536.f;
      भग्न sx = paint.u.scale.scale_x / 65536.f;
      भग्न sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->धकेलो_scale_around_center (c->तथ्य, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_ROTATE:
    {
      भग्न dx = paint.u.rotate.center_x / 65536.f;
      भग्न dy = paint.u.rotate.center_y / 65536.f;
      भग्न a = paint.u.rotate.angle / 65536.f;

      c->funcs->धकेलो_rotate_around_center (c->तथ्य, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_SKEW:
    {
      भग्न dx = paint.u.skew.center_x / 65536.f;
      भग्न dy = paint.u.skew.center_y / 65536.f;
      भग्न sx = paint.u.skew.x_skew_angle / 65536.f;
      भग्न sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->धकेलो_skew_around_center (c->तथ्य, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->निकालो_transform (c->तथ्य);
    }
    अवरोध;
    हाल FT_COLR_PAINTFORMAT_COMPOSITE:
    {
      c->funcs->धकेलो_group (c->तथ्य);
      c->recurse (paint.u.composite.backdrop_paint);
      c->funcs->धकेलो_group (c->तथ्य);
      c->recurse (paint.u.composite.source_paint);
      c->funcs->निकालो_group (c->तथ्य, _hb_ft_paint_composite_mode (paint.u.composite.composite_mode));
      c->funcs->निकालो_group (c->तथ्य, HB_PAINT_COMPOSITE_MODE_SRC_OVER);
    }
    अवरोध;

    हाल FT_COLR_PAINT_FORMAT_MAX: अवरोध;
    शेष: HB_FALLTHROUGH;
    हाल FT_COLR_PAINTFORMAT_UNSUPPORTED: अवरोध;
  }
}


अटल bool
hb_ft_paint_glyph_colr (hb_font_t *font,
			व्योम *font_तथ्य,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, व्योम *paint_तथ्य,
			अचिन्हित पूर्णांक palette_सूचकांक,
			hb_color_t foreground,
			व्योम *user_तथ्य)
{
  स्थिर hb_ft_font_t *ft_font = (स्थिर hb_ft_font_t *) font_तथ्य;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_तथ्य   palette_तथ्य = {};
  FT_Color*         palette = शून्य;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_सूचकांक;
  FT_UInt  layer_color_सूचकांक;

  (व्योम) FT_Palette_तथ्य_Get(ft_face, &palette_तथ्य);
  (व्योम) FT_Palette_Select(ft_face, palette_सूचकांक, &palette);
  अगर (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (व्योम) FT_Palette_Select(ft_face, 0, &palette);
  }

  स्वतः palette_array = hb_array ((स्थिर FT_Color *) palette,
				 palette ? palette_तथ्य.अंक_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  अगर (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_मूल_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_तथ्य,
			     palette_array, palette_सूचकांक, foreground);
    hb_decycler_गाँठ_t node (c.glyphs_decycler);
    node.visit (gid);

    FT_ClipBox clip_box;
    bool clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    bool is_bounded = clip;
    अगर (!is_bounded)
    {
      स्वतः *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_तथ्य;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_तथ्य,
			        palette_array, palette_सूचकांक, foreground);
      hb_decycler_गाँठ_t node2 (ce.glyphs_decycler);
      node2.visit (gid);
      ce.recurse (paint);
      is_bounded = bounded_तथ्य.is_bounded ();
    }

    c.funcs->धकेलो_font_transform (c.तथ्य, font);

    अगर (clip)
    {
      भग्न xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->धकेलो_clip_rectangle (c.तथ्य, xmin, ymin, xmax, ymax);
    }

    अगर (is_bounded)
      c.recurse (paint);

    अगर (clip)
      c.funcs->निकालो_clip (c.तथ्य);

    c.funcs->निकालो_transform (c.तथ्य);

    वापस true;
  }

  /* COLRv0 */
  iterator.p  = शून्य;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_सूचकांक,
					 &layer_color_सूचकांक,
					 &iterator);

  अगर (palette && have_layers)
  {
    करो
    {
      hb_bool_t is_foreground = true;
      hb_color_t color = foreground;

      अगर ( layer_color_सूचकांक != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_सूचकांक];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = false;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_तथ्य, layer_glyph_सूचकांक, font, is_foreground, color);
      ft_font->lock.lock ();

    } जबतक (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_सूचकांक,
				      &layer_color_सूचकांक,
				      &iterator));
    वापस true;
  }

  वापस false;
}


#पूर्ण_अगर /* HB_FT_COLR_HH */
