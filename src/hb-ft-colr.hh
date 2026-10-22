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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_FT_COLR_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_FT_COLR_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-decycler.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint-bounded.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ FT_COLOR_H


ਸਥਿਰ hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t
hb_ft_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ (FT_ਸੰਯੁਕਤ_ਬਹੁਲਕ ਬਹੁਲਕ)
{
  ਚੋਣ (ਬਹੁਲਕ)
  {
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_CLEAR:          ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SRC:            ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DEST:           ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SRC_OVER:       ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DEST_OVER:      ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SRC_ਵਿੱਚ:         ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DEST_ਵਿੱਚ:        ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SRC_ਬਾਹਰਲਾ:        ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DEST_ਬਾਹਰਲਾ:       ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SRC_ATOP:       ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DEST_ATOP:      ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_XOR:            ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_PLUS:           ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SCREEN:         ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_OVERLAY:        ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DARKEN:         ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_LIGHTEN:        ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_COLOR_DODGE:    ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_COLOR_BURN:     ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_HARD_LIGHT:     ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_SOFT_LIGHT:     ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_DIFFERENCE:     ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_EXCLUSION:      ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_MULTIPLY:       ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_HSL_HUE:        ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_HSL_SATURATION: ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_HSL_COLOR:      ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR;
    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_HSL_LUMINOSITY: ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY;

    ਹਾਲਤ FT_COLR_ਸੰਯੁਕਤ_MAX:            HB_FALLTHROUGH;
    ਮੂਲ:                               ਵਾਪਸ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR;
  }
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ft_paint_context_t hb_ft_paint_context_t;

ਸਥਿਰ ਖਾਲੀ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint);

ਸਥਿਰ ਖਾਲੀ
_hb_ft_unscale_clip_box (hb_font_t *font,
			 ਅਚਲ FT_ClipBox *clip_box,
			 ਦਸ਼ਮਲਵ *xmin, ਦਸ਼ਮਲਵ *ymin,
			 ਦਸ਼ਮਲਵ *xmax, ਦਸ਼ਮਲਵ *ymax)
{
  /* The FreeType ClipBox is in scaled coordinates. Convert it back to
   * font units before pushing it under the font transform.
   */
  ਦਸ਼ਮਲਵ upem = font->face->get_upem ();
  ਦਸ਼ਮਲਵ xscale = upem / (font->x_scale ? font->x_scale : upem);
  ਦਸ਼ਮਲਵ yscale = upem / (font->y_scale ? font->y_scale : upem);

  *xmin = clip_box->bottom_ਖੱਬਾ.x * xscale;
  *ymin = clip_box->bottom_ਖੱਬਾ.y * yscale;
  *xmax = clip_box->ਸਿਖਰ_ਸੱਜਾ.x * xscale;
  *ymax = clip_box->ਸਿਖਰ_ਸੱਜਾ.y * yscale;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
_hb_ft_color_alpha (ਬਿਨਾਨਿਸ਼ਾਨੀ alpha, ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_mult)
{
  ਵਾਪਸ (alpha * alpha_mult + (1 << 13)) >> 14;
}

ਬਣਤਰ hb_ft_paint_context_t
{
  hb_ft_paint_context_t (ਅਚਲ hb_ft_font_t *ft_font_,
			 hb_font_t *font_,
			 hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			 hb_array_t<ਅਚਲ FT_Color> palette,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਸੂਚਕਾਂਕ,
			 hb_color_t foreground) :
    ft_font (ft_font_), font (font_),
    funcs (paint_funcs), ਅੰਕੜੇ (paint_ਅੰਕੜੇ),
    palette (palette), palette_ਸੂਚਕਾਂਕ (palette_ਸੂਚਕਾਂਕ), foreground (foreground)
  {
    ਜੇ (font->is_synthetic)
    {
      font = hb_font_create_sub_font (font);
      hb_font_ਸੈੱਟ_synthetic_bold (font, 0, 0, ਸੱਚ);
      hb_font_ਸੈੱਟ_synthetic_slant (font, 0);
    }
    ਨਹੀਂਤਾਂ
      hb_font_reference (font);
  }

  ~hb_ft_paint_context_t ()
  {
    hb_font_destroy (font);
  }

  ਖਾਲੀ recurse (FT_OpaquePaint paint)
  {
    ਜੇ (unlikely (ਡੂੰਘਾਈ_ਖੱਬਾ <= 0 || ਕਿਨਾਰਾ_ਗਿਣਤੀ <= 0)) ਵਾਪਸ;
    ਡੂੰਘਾਈ_ਖੱਬਾ--;
    ਕਿਨਾਰਾ_ਗਿਣਤੀ--;
    _hb_ft_paint (ਇਹ, paint);
    ਡੂੰਘਾਈ_ਖੱਬਾ++;
  }

  ਅਚਲ hb_ft_font_t *ft_font;
  hb_font_t *font;
  hb_paint_funcs_t *funcs;
  ਖਾਲੀ *ਅੰਕੜੇ;
  hb_array_t<ਅਚਲ FT_Color> palette;
  ਬਿਨਾਨਿਸ਼ਾਨੀ palette_ਸੂਚਕਾਂਕ;
  hb_color_t foreground;
  hb_decycler_t glyphs_decycler;
  hb_decycler_t layers_decycler;
  ਪੂਰਨ ਡੂੰਘਾਈ_ਖੱਬਾ = HB_MAX_NESTING_LEVEL;
  ਪੂਰਨ ਕਿਨਾਰਾ_ਗਿਣਤੀ = HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ;
};

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ft_color_ਸਤਰ_get_color_stops (hb_color_ਸਤਰ_t *color_ਸਤਰ,
				   ਖਾਲੀ *color_ਸਤਰ_ਅੰਕੜੇ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2,
				   hb_color_stop_t *color_stops,
				   ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  FT_Color_ਸਤਰ *cl = (FT_Color_ਸਤਰ *) color_ਸਤਰ_ਅੰਕੜੇ;
  hb_ft_paint_context_t *c = (hb_ft_paint_context_t *) user_ਅੰਕੜੇ;

  ਜੇ (ਗਿਣਤੀ_2 && color_stops)
  {
    FT_ColorStop stop;
    ਬਿਨਾਨਿਸ਼ਾਨੀ wrote = 0;
    FT_ColorStopIterator iter = cl->color_stop_iterator;

    ਜੇ (ਸ਼ੁਰੂ >= cl->color_stop_iterator.ਅੰਕ_color_stops)
    {
      *ਗਿਣਤੀ_2 = 0;
      ਵਾਪਸ cl->color_stop_iterator.ਅੰਕ_color_stops;
    }

    ਜਦੋਂ (cl->color_stop_iterator.current_color_stop < ਸ਼ੁਰੂ)
    {
      ਜੇ (!FT_Get_Colorline_Stops(c->ft_font->ft_face,
			     &stop,
			     &cl->color_stop_iterator))
      {
         // FT_Get_Colorline_Stops can in some cases return 0, and
         // does not advance the iterator. So stop iteration to prevent
         // infinite loop here.
         //
         // reset the iterator for next time
        cl->color_stop_iterator = iter;
        ਵਾਪਸ cl->color_stop_iterator.ਅੰਕ_color_stops;
      }
    }

    ਜਦੋਂ (wrote < *ਗਿਣਤੀ_2 &&
	   FT_Get_Colorline_Stops(c->ft_font->ft_face,
				  &stop,
				  &cl->color_stop_iterator))
    {
      // https://github.com/harfbuzz/harfbuzz/issues/4013
      ਜੇ (ਆਕਾਰ stop.stop_ਵਿੱਥ_2 == 2)
	color_stops->ਵਿੱਥ = stop.stop_ਵਿੱਥ_2 / 16384.f;
      ਨਹੀਂਤਾਂ
	color_stops->ਵਿੱਥ = stop.stop_ਵਿੱਥ_2 / 65536.f;

      color_stops->is_foreground = stop.color.palette_ਸੂਚਕਾਂਕ == 0xFFFF;
      ਜੇ (color_stops->is_foreground)
	color_stops->color = HB_COLOR (hb_color_get_blue (c->foreground),
				       hb_color_get_green (c->foreground),
				       hb_color_get_red (c->foreground),
				       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
							   stop.color.alpha));
      ਨਹੀਂਤਾਂ
      {
	hb_color_t color;
        ਜੇ (c->funcs->custom_palette_color (c->ਅੰਕੜੇ, stop.color.palette_ਸੂਚਕਾਂਕ, &color))
	{
	  color_stops->color = HB_COLOR (hb_color_get_blue (color),
					 hb_color_get_green (color),
					 hb_color_get_red (color),
					 _hb_ft_color_alpha (hb_color_get_alpha (color),
							     stop.color.alpha));
	}
	ਨਹੀਂਤਾਂ ਜੇ (c->palette)
	{
	  FT_Color ft_color = c->palette[stop.color.palette_ਸੂਚਕਾਂਕ];
	  color_stops->color = HB_COLOR (ft_color.blue,
					 ft_color.green,
					 ft_color.red,
					 _hb_ft_color_alpha (ft_color.alpha,
							     stop.color.alpha));
	}
	ਨਹੀਂਤਾਂ
	  color_stops->color = HB_COLOR (0, 0, 0, 0);
      }

      color_stops++;
      wrote++;
    }

    *ਗਿਣਤੀ_2 = wrote;
    // reset the iterator for next time
    cl->color_stop_iterator = iter;
  }

  ਵਾਪਸ cl->color_stop_iterator.ਅੰਕ_color_stops;
}

ਸਥਿਰ hb_paint_extend_t
hb_ft_color_ਸਤਰ_get_extend (hb_color_ਸਤਰ_t *color_ਸਤਰ,
			      ਖਾਲੀ *color_ਸਤਰ_ਅੰਕੜੇ,
			      ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  FT_Color_ਸਤਰ *c = (FT_Color_ਸਤਰ *) color_ਸਤਰ_ਅੰਕੜੇ;
  ਚੋਣ (c->extend)
  {
    ਮੂਲ:
    ਹਾਲਤ FT_COLR_PAINT_EXTEND_PAD:     ਵਾਪਸ HB_PAINT_EXTEND_PAD;
    ਹਾਲਤ FT_COLR_PAINT_EXTEND_REPEAT:  ਵਾਪਸ HB_PAINT_EXTEND_REPEAT;
    ਹਾਲਤ FT_COLR_PAINT_EXTEND_REFLECT: ਵਾਪਸ HB_PAINT_EXTEND_REFLECT;
  }
}

ਸਥਿਰ ਖਾਲੀ
_hb_ft_get_solid_color (hb_ft_paint_context_t *c,
			ਅਚਲ FT_Color_ਸੂਚਕਾਂਕ &color_ਸੂਚਕਾਂਕ,
			hb_bool_t *is_foreground,
			hb_color_t *color)
{
  *is_foreground = color_ਸੂਚਕਾਂਕ.palette_ਸੂਚਕਾਂਕ == 0xFFFF;

  ਜੇ (*is_foreground)
  {
    *color = HB_COLOR (hb_color_get_blue (c->foreground),
		       hb_color_get_green (c->foreground),
		       hb_color_get_red (c->foreground),
		       _hb_ft_color_alpha (hb_color_get_alpha (c->foreground),
					   color_ਸੂਚਕਾਂਕ.alpha));
    ਵਾਪਸ;
  }

  ਜੇ (c->funcs->custom_palette_color (c->ਅੰਕੜੇ, color_ਸੂਚਕਾਂਕ.palette_ਸੂਚਕਾਂਕ, color))
  {
    *color = HB_COLOR (hb_color_get_blue (*color),
		       hb_color_get_green (*color),
		       hb_color_get_red (*color),
		       _hb_ft_color_alpha (hb_color_get_alpha (*color),
					   color_ਸੂਚਕਾਂਕ.alpha));
    ਵਾਪਸ;
  }

  FT_Color ft_color = c->palette[color_ਸੂਚਕਾਂਕ.palette_ਸੂਚਕਾਂਕ];
  *color = HB_COLOR (ft_color.blue,
		     ft_color.green,
		     ft_color.red,
		     _hb_ft_color_alpha (ft_color.alpha,
					 color_ਸੂਚਕਾਂਕ.alpha));
}

ਖਾਲੀ
_hb_ft_paint (hb_ft_paint_context_t *c,
	      FT_OpaquePaint opaque_paint)
{
  FT_Face ft_face = c->ft_font->ft_face;
  FT_COLR_Paint paint;
  ਜੇ (!FT_Get_Paint (ft_face, opaque_paint, &paint))
    ਵਾਪਸ;

  ਚੋਣ (paint.format)
  {
    ਹਾਲਤ FT_COLR_PAINTFORMAT_COLR_LAYERS:
    {
      FT_OpaquePaint other_paint = {0};
      hb_decycler_ਗੰਢ_t node (c->layers_decycler);
      ਜਦੋਂ (FT_Get_Paint_Layers (ft_face,
				  &paint.u.colr_layers.layer_iterator,
				  &other_paint))
      {
	// FreeType doesn't provide a way to get the layer index, so we use the pointer
	// for cycle detection.
	ਜੇ (unlikely (!node.ਦੇਖੋ ((uintptr_t) other_paint.p)))
	  ਜਾਰੀ;

	c->recurse (other_paint);
      }
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_SOLID:
    {
      hb_bool_t is_foreground;
      hb_color_t color;
      _hb_ft_get_solid_color (c, paint.u.solid.color, &is_foreground, &color);
      c->funcs->color (c->ਅੰਕੜੇ, is_foreground, color);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_ਰੇਖਿਕ_GRADIENT:
    {
      hb_color_ਸਤਰ_t cl = {
	&paint.u.ਰੇਖਿਕ_gradient.colorline,
	hb_ft_color_ਸਤਰ_get_color_stops, c,
	hb_ft_color_ਸਤਰ_get_extend, ਖਾਲੀ_ਸੂਚਕ
      };

      c->funcs->ਰੇਖਿਕ_gradient (c->ਅੰਕੜੇ, &cl,
				 paint.u.ਰੇਖਿਕ_gradient.p0.x / 65536.f,
				 paint.u.ਰੇਖਿਕ_gradient.p0.y / 65536.f,
				 paint.u.ਰੇਖਿਕ_gradient.p1.x / 65536.f,
				 paint.u.ਰੇਖਿਕ_gradient.p1.y / 65536.f,
				 paint.u.ਰੇਖਿਕ_gradient.p2.x / 65536.f,
				 paint.u.ਰੇਖਿਕ_gradient.p2.y / 65536.f);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_RADIAL_GRADIENT:
    {
      hb_color_ਸਤਰ_t cl = {
	&paint.u.ਰੇਖਿਕ_gradient.colorline,
	hb_ft_color_ਸਤਰ_get_color_stops, c,
	hb_ft_color_ਸਤਰ_get_extend, ਖਾਲੀ_ਸੂਚਕ
      };

      c->funcs->radial_gradient (c->ਅੰਕੜੇ, &cl,
				 paint.u.radial_gradient.c0.x / 65536.f,
				 paint.u.radial_gradient.c0.y / 65536.f,
				 paint.u.radial_gradient.r0 / 65536.f,
				 paint.u.radial_gradient.c1.x / 65536.f,
				 paint.u.radial_gradient.c1.y / 65536.f,
				 paint.u.radial_gradient.r1 / 65536.f);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_SWEEP_GRADIENT:
    {
      hb_color_ਸਤਰ_t cl = {
	&paint.u.ਰੇਖਿਕ_gradient.colorline,
	hb_ft_color_ਸਤਰ_get_color_stops, c,
	hb_ft_color_ਸਤਰ_get_extend, ਖਾਲੀ_ਸੂਚਕ
      };

      c->funcs->sweep_gradient (c->ਅੰਕੜੇ, &cl,
				paint.u.sweep_gradient.center.x / 65536.f,
				paint.u.sweep_gradient.center.y / 65536.f,
				(paint.u.sweep_gradient.ਸ਼ੁਰੂ_angle / 65536.f + 1) * HB_PI,
				(paint.u.sweep_gradient.ਸਿਰਾ_angle / 65536.f + 1) * HB_PI);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_GLYPH:
    {
      /* Optimize cases that can use a simple fill-glyph operation. */
      FT_COLR_Paint fill;
      ਜੇ (likely (c->ਡੂੰਘਾਈ_ਖੱਬਾ > 0 && c->ਕਿਨਾਰਾ_ਗਿਣਤੀ > 0) &&
	  FT_Get_Paint (ft_face, paint.u.glyph.paint, &fill) &&
	  fill.format == FT_COLR_PAINTFORMAT_SOLID)
      {
	hb_bool_t is_foreground;
	hb_color_t color;
	_hb_ft_get_solid_color (c, fill.u.solid.color, &is_foreground, &color);

	c->ਕਿਨਾਰਾ_ਗਿਣਤੀ--;
	c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
	c->ft_font->lock.unlock ();
	c->funcs->fill_glyph (c->ਅੰਕੜੇ, paint.u.glyph.glyphID, c->font,
			      is_foreground, color);
	c->ft_font->lock.lock ();
	c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
	ਰੋਕੋ;
      }

      c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
      c->ft_font->lock.unlock ();
      c->funcs->ਧੱਕੋ_clip_glyph (c->ਅੰਕੜੇ, paint.u.glyph.glyphID, c->font);
      c->ft_font->lock.lock ();
      c->funcs->ਧੱਕੋ_font_transform (c->ਅੰਕੜੇ, c->font);
      c->recurse (paint.u.glyph.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
      c->funcs->ਕੱਢੋ_clip (c->ਅੰਕੜੇ);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_COLR_GLYPH:
    {
      hb_codepoint_t gid = paint.u.colr_glyph.glyphID;

      hb_decycler_ਗੰਢ_t node (c->glyphs_decycler);
      ਜੇ (unlikely (!node.ਦੇਖੋ (gid)))
	ਵਾਪਸ;

      c->funcs->ਧੱਕੋ_inverse_font_transform (c->ਅੰਕੜੇ, c->font);
      c->ft_font->lock.unlock ();
      ਜੇ (c->funcs->color_glyph (c->ਅੰਕੜੇ, gid, c->font))
      {
	c->ft_font->lock.lock ();
	c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
	ਵਾਪਸ;
      }
      c->ft_font->lock.lock ();
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);

      FT_OpaquePaint other_paint = {0};
      ਜੇ (FT_Get_Color_Glyph_Paint (ft_face, gid,
				    FT_COLOR_NO_ਜੜ੍ਹ_TRANSFORM,
				    &other_paint))
      {
        ਤਾਰਕਿਕ has_clip_box;
        FT_ClipBox clip_box;
        has_clip_box = FT_Get_Color_Glyph_ClipBox (ft_face, paint.u.colr_glyph.glyphID, &clip_box);

        ਜੇ (has_clip_box)
	{
	  ਦਸ਼ਮਲਵ xmin, ymin, xmax, ymax;
	  _hb_ft_unscale_clip_box (c->font, &clip_box,
				   &xmin, &ymin, &xmax, &ymax);
	  c->funcs->ਧੱਕੋ_clip_rectangle (c->ਅੰਕੜੇ, xmin, ymin, xmax, ymax);
	}

	c->recurse (other_paint);

        ਜੇ (has_clip_box)
          c->funcs->ਕੱਢੋ_clip (c->ਅੰਕੜੇ);
      }
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_TRANSFORM:
    {
      c->funcs->ਧੱਕੋ_transform (c->ਅੰਕੜੇ,
				paint.u.transform.affine.xx / 65536.f,
				paint.u.transform.affine.yx / 65536.f,
				paint.u.transform.affine.xy / 65536.f,
				paint.u.transform.affine.yy / 65536.f,
				paint.u.transform.affine.dx / 65536.f,
				paint.u.transform.affine.dy / 65536.f);
      c->recurse (paint.u.transform.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_TRANSLATE:
    {
      ਦਸ਼ਮਲਵ dx = paint.u.translate.dx / 65536.f;
      ਦਸ਼ਮਲਵ dy = paint.u.translate.dy / 65536.f;

      c->funcs->ਧੱਕੋ_translate (c->ਅੰਕੜੇ, dx, dy);
      c->recurse (paint.u.translate.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_SCALE:
    {
      ਦਸ਼ਮਲਵ dx = paint.u.scale.center_x / 65536.f;
      ਦਸ਼ਮਲਵ dy = paint.u.scale.center_y / 65536.f;
      ਦਸ਼ਮਲਵ sx = paint.u.scale.scale_x / 65536.f;
      ਦਸ਼ਮਲਵ sy = paint.u.scale.scale_y / 65536.f;

      c->funcs->ਧੱਕੋ_scale_around_center (c->ਅੰਕੜੇ, sx, sy, dx, dy);
      c->recurse (paint.u.scale.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_ROTATE:
    {
      ਦਸ਼ਮਲਵ dx = paint.u.rotate.center_x / 65536.f;
      ਦਸ਼ਮਲਵ dy = paint.u.rotate.center_y / 65536.f;
      ਦਸ਼ਮਲਵ a = paint.u.rotate.angle / 65536.f;

      c->funcs->ਧੱਕੋ_rotate_around_center (c->ਅੰਕੜੇ, a, dx, dy);
      c->recurse (paint.u.rotate.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_SKEW:
    {
      ਦਸ਼ਮਲਵ dx = paint.u.skew.center_x / 65536.f;
      ਦਸ਼ਮਲਵ dy = paint.u.skew.center_y / 65536.f;
      ਦਸ਼ਮਲਵ sx = paint.u.skew.x_skew_angle / 65536.f;
      ਦਸ਼ਮਲਵ sy = paint.u.skew.y_skew_angle / 65536.f;

      c->funcs->ਧੱਕੋ_skew_around_center (c->ਅੰਕੜੇ, sx, sy, dx, dy);
      c->recurse (paint.u.skew.paint);
      c->funcs->ਕੱਢੋ_transform (c->ਅੰਕੜੇ);
    }
    ਰੋਕੋ;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_ਸੰਯੁਕਤ:
    {
      c->funcs->ਧੱਕੋ_ਸਮੂਹ (c->ਅੰਕੜੇ);
      c->recurse (paint.u.ਸੰਯੁਕਤ_2.backdrop_paint);
      c->funcs->ਧੱਕੋ_ਸਮੂਹ (c->ਅੰਕੜੇ);
      c->recurse (paint.u.ਸੰਯੁਕਤ_2.ਸਰੋਤ_paint);
      c->funcs->ਕੱਢੋ_ਸਮੂਹ (c->ਅੰਕੜੇ, hb_ft_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ (paint.u.ਸੰਯੁਕਤ_2.ਸੰਯੁਕਤ_ਬਹੁਲਕ_2));
      c->funcs->ਕੱਢੋ_ਸਮੂਹ (c->ਅੰਕੜੇ, HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER);
    }
    ਰੋਕੋ;

    ਹਾਲਤ FT_COLR_PAINT_FORMAT_MAX: ਰੋਕੋ;
    ਮੂਲ: HB_FALLTHROUGH;
    ਹਾਲਤ FT_COLR_PAINTFORMAT_UNSUPPORTED: ਰੋਕੋ;
  }
}


ਸਥਿਰ ਤਾਰਕਿਕ
hb_ft_paint_glyph_colr (hb_font_t *font,
			ਖਾਲੀ *font_ਅੰਕੜੇ,
			hb_codepoint_t gid,
			hb_paint_funcs_t *paint_funcs, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette_ਸੂਚਕਾਂਕ,
			hb_color_t foreground,
			ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਅਚਲ hb_ft_font_t *ft_font = (ਅਚਲ hb_ft_font_t *) font_ਅੰਕੜੇ;
  FT_Face ft_face = ft_font->ft_face;

  /* Face is locked. */

  FT_Palette_ਅੰਕੜੇ   palette_ਅੰਕੜੇ = {};
  FT_Color*         palette = ਨਿਰਾ;
  FT_LayerIterator  iterator;

  FT_Bool  have_layers;
  FT_UInt  layer_glyph_ਸੂਚਕਾਂਕ;
  FT_UInt  layer_color_ਸੂਚਕਾਂਕ;

  (ਖਾਲੀ) FT_Palette_ਅੰਕੜੇ_Get(ft_face, &palette_ਅੰਕੜੇ);
  (ਖਾਲੀ) FT_Palette_Select(ft_face, palette_ਸੂਚਕਾਂਕ, &palette);
  ਜੇ (!palette)
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5116
    (ਖਾਲੀ) FT_Palette_Select(ft_face, 0, &palette);
  }

  ਸਵੈਚਾਲਿਤ palette_array = hb_array ((ਅਚਲ FT_Color *) palette,
				 palette ? palette_ਅੰਕੜੇ.ਅੰਕ_palette_entries : 0);

  /* COLRv1 */
  FT_OpaquePaint paint = {0};
  ਜੇ (FT_Get_Color_Glyph_Paint (ft_face, gid,
			        FT_COLOR_NO_ਜੜ੍ਹ_TRANSFORM,
			        &paint))
  {
    hb_ft_paint_context_t c (ft_font, font,
			     paint_funcs, paint_ਅੰਕੜੇ,
			     palette_array, palette_ਸੂਚਕਾਂਕ, foreground);
    hb_decycler_ਗੰਢ_t node (c.glyphs_decycler);
    node.ਦੇਖੋ (gid);

    FT_ClipBox clip_box;
    ਤਾਰਕਿਕ clip = FT_Get_Color_Glyph_ClipBox (ft_face, gid, &clip_box);
    ਤਾਰਕਿਕ is_bounded = clip;
    ਜੇ (!is_bounded)
    {
      ਸਵੈਚਾਲਿਤ *bounded_funcs = hb_paint_bounded_get_funcs ();
      hb_paint_bounded_context_t bounded_ਅੰਕੜੇ;
      hb_ft_paint_context_t ce (ft_font, font,
			        bounded_funcs, &bounded_ਅੰਕੜੇ,
			        palette_array, palette_ਸੂਚਕਾਂਕ, foreground);
      hb_decycler_ਗੰਢ_t node2 (ce.glyphs_decycler);
      node2.ਦੇਖੋ (gid);
      ce.recurse (paint);
      is_bounded = bounded_ਅੰਕੜੇ.is_bounded ();
    }

    c.funcs->ਧੱਕੋ_font_transform (c.ਅੰਕੜੇ, font);

    ਜੇ (clip)
    {
      ਦਸ਼ਮਲਵ xmin, ymin, xmax, ymax;
      _hb_ft_unscale_clip_box (font, &clip_box,
			       &xmin, &ymin, &xmax, &ymax);
      c.funcs->ਧੱਕੋ_clip_rectangle (c.ਅੰਕੜੇ, xmin, ymin, xmax, ymax);
    }

    ਜੇ (is_bounded)
      c.recurse (paint);

    ਜੇ (clip)
      c.funcs->ਕੱਢੋ_clip (c.ਅੰਕੜੇ);

    c.funcs->ਕੱਢੋ_transform (c.ਅੰਕੜੇ);

    ਵਾਪਸ ਸੱਚ;
  }

  /* COLRv0 */
  iterator.p  = ਨਿਰਾ;
  have_layers = FT_Get_Color_Glyph_Layer(ft_face,
					 gid,
					 &layer_glyph_ਸੂਚਕਾਂਕ,
					 &layer_color_ਸੂਚਕਾਂਕ,
					 &iterator);

  ਜੇ (palette && have_layers)
  {
    ਕਰੋ
    {
      hb_bool_t is_foreground = ਸੱਚ;
      hb_color_t color = foreground;

      ਜੇ ( layer_color_ਸੂਚਕਾਂਕ != 0xFFFF )
      {
	FT_Color layer_color = palette[layer_color_ਸੂਚਕਾਂਕ];
	color = HB_COLOR (layer_color.blue,
			  layer_color.green,
			  layer_color.red,
			  layer_color.alpha);
	is_foreground = ਝੂਠ;
      }

      ft_font->lock.unlock ();
      paint_funcs->fill_glyph (paint_ਅੰਕੜੇ, layer_glyph_ਸੂਚਕਾਂਕ, font, is_foreground, color);
      ft_font->lock.lock ();

    } ਜਦੋਂ (FT_Get_Color_Glyph_Layer(ft_face,
				      gid,
				      &layer_glyph_ਸੂਚਕਾਂਕ,
				      &layer_color_ਸੂਚਕਾਂਕ,
				      &iterator));
    ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ ਝੂਠ;
}


#ਜੇ_ਅੰਤ /* HB_FT_COLR_HH */
