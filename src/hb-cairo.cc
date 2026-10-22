/*
 * Copyright © 2022  Red Hat, Inc.
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
 *
 * Red Hat Author(s): Matthias Clasen
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cairo.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cairo-utils.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-utf.hh"


/**
 * SECTION:hb-cairo
 * @title: hb-cairo
 * @short_description: Cairo integration
 * @include: hb-cairo.h
 *
 * Functions for using HarfBuzz with the cairo library.
 *
 * HarfBuzz supports using cairo for rendering.
 **/

ਸਥਿਰ ਖਾਲੀ
hb_cairo_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		  ਖਾਲੀ *draw_ਅੰਕੜੇ,
		  hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->spend (HB_BUDGET_1)))
    ਵਾਪਸ;

  cairo_move_ਮੰਜ਼ਿਲ (c->cr, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		  ਖਾਲੀ *draw_ਅੰਕੜੇ,
		  hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->spend (HB_BUDGET_1)))
    ਵਾਪਸ;

  cairo_ਸਤਰ_ਮੰਜ਼ਿਲ (c->cr, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		   ਖਾਲੀ *draw_ਅੰਕੜੇ,
		   hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		   ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
		   ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
		   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->spend (HB_BUDGET_1)))
    ਵਾਪਸ;

  cairo_ਵਕਰ_ਮੰਜ਼ਿਲ (c->cr,
                  (ਦੁੱਗਣਾ) control1_x, (ਦੁੱਗਣਾ) control1_y,
                  (ਦੁੱਗਣਾ) control2_x, (ਦੁੱਗਣਾ) control2_y,
                  (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *draw_ਅੰਕੜੇ,
		     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->spend (HB_BUDGET_1)))
    ਵਾਪਸ;

  cairo_close_ਰਸਤਾ (c->cr);
}

ਸਥਿਰ hb_bool_t
hb_cairo_draw_ਸੈੱਟ_budget (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  int64_t budget,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) draw_ਅੰਕੜੇ)->ਸੈੱਟ_budget (budget);
}

ਸਥਿਰ int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *draw_ਅੰਕੜੇ,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) draw_ਅੰਕੜੇ)->get_budget ();
}

ਸਥਿਰ int64_t *
hb_cairo_draw_get_budget_ਬਚਿਆ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
				    ਖਾਲੀ *draw_ਅੰਕੜੇ,
				    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) draw_ਅੰਕੜੇ)->get_budget_ਬਚਿਆ ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_cairo_draw_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, hb_cairo_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, hb_cairo_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, hb_cairo_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, hb_cairo_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_cairo_draw_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_cairo_draw_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_cairo_draw_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    ਵਾਪਸ funcs;
  }
} static_cairo_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

ਸਥਿਰ hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  ਵਾਪਸ static_cairo_draw_funcs.get_unconst ();
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_FONT_FACE_ਸੈੱਟ_RENDER_COLOR_GLYPH_FUNC

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਧੱਕੋ_transform (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ *paint_ਅੰਕੜੇ,
			 ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
			 ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
			 ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
			 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_ਮੈਟ੍ਰਿਕਸ_t m;

  cairo_save (cr);
  cairo_ਮੈਟ੍ਰਿਕਸ_init (&m, (ਦੁੱਗਣਾ) xx, (ਦੁੱਗਣਾ) yx,
                         (ਦੁੱਗਣਾ) xy, (ਦੁੱਗਣਾ) yy,
                         (ਦੁੱਗਣਾ) dx, (ਦੁੱਗਣਾ) dy);
  cairo_transform (cr, &m);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਕੱਢੋ_transform (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		        ਖਾਲੀ *paint_ਅੰਕੜੇ,
		        ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *paint_ਅੰਕੜੇ,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_ਰਸਤਾ (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_ਰਸਤਾ (cr);
  hb_cairo_ਸੈੱਟ_ਸਰੋਤ_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

ਸਥਿਰ hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  ਜੇ (unlikely (!c->spend (HB_BUDGET_1)))
    ਵਾਪਸ ਸੱਚ;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_ਸੈੱਟ_scaled_font (cr, c->scaled_font);
  cairo_ਸੈੱਟ_font_ਮਾਪ (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *paint_ਅੰਕੜੇ,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_ਰਸਤਾ (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_ਰਸਤਾ (cr);
  cairo_clip (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (ਦੁੱਗਣਾ) xmin, (ਦੁੱਗਣਾ) ymin,
                   (ਦੁੱਗਣਾ) (xmax - xmin), (ਦੁੱਗਣਾ) (ymax - ymin));
  cairo_clip (cr);
}

ਸਥਿਰ hb_draw_funcs_t *
hb_cairo_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ *paint_ਅੰਕੜੇ,
			       ਖਾਲੀ **draw_ਅੰਕੜੇ,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_ਰਸਤਾ (cr);
  *draw_ਅੰਕੜੇ = c;
  ਵਾਪਸ hb_cairo_draw_get_funcs ();
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_close_ਰਸਤਾ (cr);
  cairo_clip (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਕੱਢੋ_clip (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		   ਖਾਲੀ *paint_ਅੰਕੜੇ,
		   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *paint_ਅੰਕੜੇ,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_ਧੱਕੋ_ਸਮੂਹ (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		    ਖਾਲੀ *paint_ਅੰਕੜੇ,
		    hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
		    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  cairo_ਕੱਢੋ_ਸਮੂਹ_ਮੰਜ਼ਿਲ_ਸਰੋਤ (cr);
  cairo_ਸੈੱਟ_operator (cr, hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_ਮੰਜ਼ਿਲ_cairo (ਬਹੁਲਕ));
  cairo_paint (cr);

  cairo_restore (cr);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *paint_ਅੰਕੜੇ,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

  hb_cairo_ਸੈੱਟ_ਸਰੋਤ_color (c, use_foreground, color);
  cairo_paint (cr);
}

ਸਥਿਰ hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *paint_ਅੰਕੜੇ,
		      hb_blob_t *blob,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
		      hb_ਟੈਗ_t format,
		      ਦਸ਼ਮਲਵ slant,
		      hb_glyph_extents_t *extents,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;

  ਵਾਪਸ _hb_cairo_paint_glyph_image (c, blob, ਚੌੜਾਈ, ਉਚਾਈ, format, slant, extents);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_paint_ਰੇਖਿਕ_gradient_2 (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *paint_ਅੰਕੜੇ,
				hb_color_ਸਤਰ_t *color_ਸਤਰ,
				ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
				ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
				ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;

  hb_cairo_paint_ਰੇਖਿਕ_gradient (c, color_ਸਤਰ, x0, y0, x1, y1, x2, y2);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				ਖਾਲੀ *paint_ਅੰਕੜੇ,
				hb_color_ਸਤਰ_t *color_ਸਤਰ,
				ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
				ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;

  _hb_cairo_paint_radial_gradient (c, color_ਸਤਰ, x0, y0, r0, x1, y1, r1);
}

ਸਥਿਰ ਖਾਲੀ
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ *paint_ਅੰਕੜੇ,
			       hb_color_ਸਤਰ_t *color_ਸਤਰ,
			       ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			       ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle, ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;

  _hb_cairo_paint_sweep_gradient (c, color_ਸਤਰ, x0, y0, ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle);
}

ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t color_cache_ਕੁੰਜੀ = {0};

ਸਥਿਰ ਖਾਲੀ
_hb_cairo_destroy_map (ਖਾਲੀ *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

ਸਥਿਰ hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ color_ਸੂਚਕਾਂਕ,
                                     hb_color_t *color,
                                     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ਅੰਕੜੇ;
  cairo_t *cr = c->cr;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEADBEEF HB_ਟੈਗ(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  ਜੇ (likely (color_cache && color_cache->has (color_ਸੂਚਕਾਂਕ, &v)))
  {
    ਜੇ (*v == HB_DEADBEEF)
      ਵਾਪਸ ਝੂਠ;
    *color = *v;
    ਵਾਪਸ ਸੱਚ;
  }

  cairo_font_options_t *options;
  ਦੁੱਗਣਾ red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  ਜੇ (CAIRO_ਸਥਿਤੀ_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_ਸੂਚਕਾਂਕ,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    ਜੇ (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->ਸੈੱਟ (color_ਸੂਚਕਾਂਕ, *color);

    ਵਾਪਸ ਸੱਚ;
  }
  cairo_font_options_destroy (options);

  ਜੇ (likely (color_cache))
    color_cache->ਸੈੱਟ (color_ਸੂਚਕਾਂਕ, HB_DEADBEEF);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_DEADBEEF

#ਜੇ_ਅੰਤ

  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_cairo_paint_ਸੈੱਟ_budget (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   int64_t budget,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) paint_ਅੰਕੜੇ)->ਸੈੱਟ_budget (budget);
}

ਸਥਿਰ int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) paint_ਅੰਕੜੇ)->get_budget ();
}

ਸਥਿਰ int64_t *
hb_cairo_paint_get_budget_ਬਚਿਆ (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				     ਖਾਲੀ *paint_ਅੰਕੜੇ,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ ((hb_cairo_context_t *) paint_ਅੰਕੜੇ)->get_budget_ਬਚਿਆ ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_cairo_paint_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func (funcs, hb_cairo_ਧੱਕੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func (funcs, hb_cairo_ਕੱਢੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_fill_glyph_func (funcs, hb_cairo_fill_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_glyph_func (funcs, hb_cairo_paint_color_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, hb_cairo_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, hb_cairo_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func (funcs, hb_cairo_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func (funcs, hb_cairo_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, hb_cairo_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, hb_cairo_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, hb_cairo_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, hb_cairo_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, hb_cairo_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, hb_cairo_paint_ਰੇਖਿਕ_gradient_2, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_cairo_paint_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_cairo_paint_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_cairo_paint_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_paint_funcs);

    ਵਾਪਸ funcs;
  }
} static_cairo_paint_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

ਸਥਿਰ hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  ਵਾਪਸ static_cairo_paint_funcs.get_unconst ();
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_face_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};
ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};
ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_font_init_func_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};
ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_font_init_user_ਅੰਕੜੇ_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};
ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_scale_ਗੁਣਕ_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};
ਸਥਿਰ ਅਚਲ cairo_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_cairo_budget_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {0};

ਸਥਿਰ ਖਾਲੀ hb_cairo_face_destroy (ਖਾਲੀ *p) { hb_face_destroy ((hb_face_t *) p); }
ਸਥਿਰ ਖਾਲੀ hb_cairo_font_destroy (ਖਾਲੀ *p) { hb_font_destroy ((hb_font_t *) p); }
ਸਥਿਰ ਖਾਲੀ
hb_cairo_budget_destroy (ਖਾਲੀ *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

ਸਥਿਰ cairo_ਸਥਿਤੀ_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_ਅਣਵਰਤਿਆ,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
								 &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);

  ਜੇ (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
								   &hb_cairo_face_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
    font = hb_font_create (face);

#ਜੇ !defined(HB_NO_VAR) && CAIRO_ਸੰਸਕਰਣ >= CAIRO_ਸੰਸਕਰਣ_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    ਅਚਲ ਅੱਖਰ *variations = cairo_font_options_get_variations (font_options);
    hb_ਸਦਿਸ਼_t<hb_variation_t> vars;
    ਅਚਲ ਅੱਖਰ *p = variations;
    ਜਦੋਂ (p && *p)
    {
      ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = strpbrk ((ਅੱਖਰ *) p, ", ");
      hb_variation_t var;
      ਜੇ (hb_variation_from_string (p, ਸਿਰਾ ? ਸਿਰਾ - p : -1, &var))
	vars.push (var);
      p = ਸਿਰਾ ? ਸਿਰਾ + 1 : ਖਾਲੀ_ਸੂਚਕ;
    }
    hb_font_ਸੈੱਟ_variations (font, &vars[0], vars.ਲੰਮਾਈ);

    cairo_font_options_destroy (font_options);
#ਜੇ_ਅੰਤ

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    ਬਿਨਾਨਿਸ਼ਾਨੀ scale_ਗੁਣਕ = hb_cairo_font_face_get_scale_ਗੁਣਕ (font_face);
    ਜੇ (scale_ਗੁਣਕ)
    {
      cairo_ਮੈਟ੍ਰਿਕਸ_t font_ਮੈਟ੍ਰਿਕਸ;
      cairo_scaled_font_get_scale_ਮੈਟ੍ਰਿਕਸ (scaled_font, &font_ਮੈਟ੍ਰਿਕਸ);
      hb_font_ਸੈੱਟ_scale (font,
			 hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (font_ਮੈਟ੍ਰਿਕਸ.xx * scale_ਗੁਣਕ)),
			 hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (font_ਮੈਟ੍ਰਿਕਸ.yy * scale_ਗੁਣਕ)));
    }

    ਸਵੈਚਾਲਿਤ *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
						     &hb_cairo_font_init_func_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
    ਜੇ (init_func)
    {
      ਖਾਲੀ *user_ਅੰਕੜੇ = cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
						       &hb_cairo_font_init_user_ਅੰਕੜੇ_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
      font = init_func (font, scaled_font, user_ਅੰਕੜੇ);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_ਸੈੱਟ_user_ਅੰਕੜੇ (scaled_font,
				   &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
				   (ਖਾਲੀ *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (ਆਕਾਰ (hb_cairo_budget_t));
  ਜੇ (unlikely (!budget))
    ਵਾਪਸ CAIRO_ਸਥਿਤੀ_NO_ਸਮ੍ਰਿਤੀ;
  ਨਵਾਂ (budget) hb_cairo_budget_t ();
  cairo_ਸਥਿਤੀ_t ਸਥਿਤੀ = cairo_scaled_font_ਸੈੱਟ_user_ਅੰਕੜੇ (scaled_font,
						   &hb_cairo_budget_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
						   budget,
						   hb_cairo_budget_destroy);
  ਜੇ (unlikely (ਸਥਿਤੀ != CAIRO_ਸਥਿਤੀ_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    ਵਾਪਸ ਸਥਿਤੀ;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (ਦੁੱਗਣਾ)  hb_extents.ascender  / y_scale;
  extents->descent = -(ਦੁੱਗਣਾ) hb_extents.descender / y_scale;
  extents->ਉਚਾਈ  = extents->ascent + extents->descent;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_FONT_FACE_ਸੈੱਟ_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  ਜੇ (unlikely (CAIRO_ਸਥਿਤੀ_SUCCESS != cairo_scaled_font_ਸੈੱਟ_user_ਅੰਕੜੇ (scaled_font,
									 &color_cache_ਕੁੰਜੀ,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#ਜੇ_ਅੰਤ

  ਵਾਪਸ CAIRO_ਸਥਿਤੀ_SUCCESS;
}

ਸਥਿਰ cairo_ਸਥਿਤੀ_t
hb_cairo_ਪਾਠ_ਮੰਜ਼ਿਲ_glyphs (cairo_scaled_font_t        *scaled_font,
			 ਅਚਲ ਅੱਖਰ	            *utf8,
			 ਪੂਰਨ		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 ਪੂਰਨ		            *ਅੰਕ_glyphs,
			 cairo_ਪਾਠ_cluster_t      **clusters,
			 ਪੂਰਨ		            *ਅੰਕ_clusters,
			 cairo_ਪਾਠ_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font,
								   &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);

  hb_ਬਫਰ_t *ਬਫਰ = hb_ਬਫਰ_create ();
  hb_ਬਫਰ_ਜੋੜੋ_utf8 (ਬਫਰ, utf8, utf8_len, 0, utf8_len);
  hb_ਬਫਰ_guess_segment_properties (ਬਫਰ);
  hb_shape (font, ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ, 0);

  ਪੂਰਨ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_ਬਫਰ (ਬਫਰ,
			       ਸੱਚ,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (ਬਿਨਾਨਿਸ਼ਾਨੀ *) ਅੰਕ_glyphs,
			       clusters, (ਬਿਨਾਨਿਸ਼ਾਨੀ *) ਅੰਕ_clusters,
			       cluster_flags);

  hb_ਬਫਰ_destroy (ਬਫਰ);

  ਵਾਪਸ CAIRO_ਸਥਿਤੀ_SUCCESS;
}

ਸਥਿਰ cairo_ਸਥਿਤੀ_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ         glyph,
		       cairo_t              *cr,
		       cairo_ਪਾਠ_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font,
								   &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font,
									     &hb_cairo_budget_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
  ਜੇ (unlikely (!budget))
    ਵਾਪਸ CAIRO_ਸਥਿਤੀ_NO_ਸਮ੍ਰਿਤੀ;
  hb_cairo_context_t c {scaled_font, cr, ਖਾਲੀ_ਸੂਚਕ, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  ਜੇ (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  ਵਾਪਸ CAIRO_ਸਥਿਤੀ_SUCCESS;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_FONT_FACE_ਸੈੱਟ_RENDER_COLOR_GLYPH_FUNC

ਸਥਿਰ cairo_ਸਥਿਤੀ_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ         glyph,
			     cairo_t              *cr,
			     cairo_ਪਾਠ_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font,
								   &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font,
									     &hb_cairo_budget_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
  ਜੇ (unlikely (!budget))
    ਵਾਪਸ CAIRO_ਸਥਿਤੀ_NO_ਸਮ੍ਰਿਤੀ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ palette = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#ਜੇ_ਅੰਤ

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font, &color_cache_ਕੁੰਜੀ),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  ਵਾਪਸ CAIRO_ਸਥਿਤੀ_SUCCESS;
}

#ਜੇ_ਅੰਤ

ਸਥਿਰ cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_ਸੈੱਟ_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_ਸੈੱਟ_ਪਾਠ_ਮੰਜ਼ਿਲ_glyphs_func (cairo_face, hb_cairo_ਪਾਠ_ਮੰਜ਼ਿਲ_glyphs);
  cairo_user_font_face_ਸੈੱਟ_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAIRO_USER_FONT_FACE_ਸੈੱਟ_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_ਸੈੱਟ_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#ਜੇ_ਅੰਤ

  ਜੇ (unlikely (CAIRO_ਸਥਿਤੀ_SUCCESS != cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (cairo_face,
								       &hb_cairo_face_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
								       (ਖਾਲੀ *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  ਵਾਪਸ cairo_face;
}

/**
 * hb_cairo_font_face_create_for_font:
 * @font: a #hb_font_t
 *
 * Creates a #cairo_font_face_t for rendering text according
 * to @font.
 *
 * Note that the scale of @font does not affect the rendering,
 * but the variations and slant that are set on @font do.
 *
 * Returns: (transfer full): a newly created #cairo_font_face_t
 *
 * Since: 7.0.0
 */
cairo_font_face_t *
hb_cairo_font_face_create_for_font (hb_font_t *font)
{
  hb_font_make_immutable (font);

  ਸਵੈਚਾਲਿਤ *hb_face = hb_font_get_face (font);
  ਸਵੈਚਾਲਿਤ *cairo_face =  user_font_face_create (hb_face);

  ਜੇ (unlikely (CAIRO_ਸਥਿਤੀ_SUCCESS != cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (cairo_face,
								       &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
								       (ਖਾਲੀ *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  ਵਾਪਸ cairo_face;
}

/**
 * hb_cairo_font_face_get_font:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the #hb_font_t that @font_face was created from.
 *
 * Returns: (nullable) (transfer none): the #hb_font_t that @font_face was created from
 *
 * Since: 7.0.0
 */
hb_font_t *
hb_cairo_font_face_get_font (cairo_font_face_t *font_face)
{
  ਵਾਪਸ (hb_font_t *) cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
						      &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
}

/**
 * hb_cairo_font_face_create_for_face:
 * @face: a #hb_face_t
 *
 * Creates a #cairo_font_face_t for rendering text according
 * to @face.
 *
 * Returns: (transfer full): a newly created #cairo_font_face_t
 *
 * Since: 7.0.0
 */
cairo_font_face_t *
hb_cairo_font_face_create_for_face (hb_face_t *face)
{
  hb_face_make_immutable (face);

  ਵਾਪਸ user_font_face_create (face);
}

/**
 * hb_cairo_font_face_get_face:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the #hb_face_t associated with @font_face.
 *
 * Returns: (nullable) (transfer none): the #hb_face_t associated with @font_face
 *
 * Since: 7.0.0
 */
hb_face_t *
hb_cairo_font_face_get_face (cairo_font_face_t *font_face)
{
  ਵਾਪਸ (hb_face_t *) cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
						      &hb_cairo_face_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
}

/**
 * hb_cairo_font_face_set_font_init_func:
 * @font_face: a #cairo_font_face_t
 * @func: The virtual method to use
 * @user_data: user data accompanying the method
 * @destroy: function to call when @user_data is not needed anymore
 *
 * Set the virtual method to be called when a cairo
 * face created using hb_cairo_font_face_create_for_face()
 * creates an #hb_font_t for a #cairo_scaled_font_t.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_cairo_font_face_ਸੈੱਟ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       ਖਾਲੀ *user_ਅੰਕੜੇ,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (font_face,
				 &hb_cairo_font_init_func_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
				 (ਖਾਲੀ *) func,
				 ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (CAIRO_ਸਥਿਤੀ_SUCCESS != cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (font_face,
								       &hb_cairo_font_init_user_ਅੰਕੜੇ_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
								       (ਖਾਲੀ *) user_ਅੰਕੜੇ,
								       destroy)) && destroy)
  {
    destroy (user_ਅੰਕੜੇ);
    cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (font_face,
				   &hb_cairo_font_init_func_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
				   ਖਾਲੀ_ਸੂਚਕ,
				   ਖਾਲੀ_ਸੂਚਕ);
  }
}

/**
 * hb_cairo_scaled_font_get_font:
 * @scaled_font: a #cairo_scaled_font_t
 *
 * Gets the #hb_font_t associated with @scaled_font.
 *
 * Returns: (nullable) (transfer none): the #hb_font_t associated with @scaled_font
 *
 * Since: 7.0.0
 */
hb_font_t *
hb_cairo_scaled_font_get_font (cairo_scaled_font_t *scaled_font)
{
  ਵਾਪਸ (hb_font_t *) cairo_scaled_font_get_user_ਅੰਕੜੇ (scaled_font, &hb_cairo_font_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
}


/**
 * hb_cairo_font_face_set_scale_factor:
 * @scale_factor: The scale factor to use. See below
 * @font_face: a #cairo_font_face_t
 *
 * Sets the scale factor of the @font_face. Default scale
 * factor is zero.
 *
 * When a #cairo_font_face_t is created from a #hb_face_t using
 * hb_cairo_font_face_create_for_face(), such face will create
 * #hb_font_t objects during scaled-font creation.  The scale
 * factor defines how the scale set on such #hb_font_t objects
 * relates to the font-matrix (as such font size) of the cairo
 * scaled-font.
 *
 * If the scale-factor is zero (default), then the scale of the
 * #hb_font_t object will be left at default, which is the UPEM
 * value of the respective #hb_face_t.
 *
 * If the scale-factor is set to non-zero, then the X and Y scale
 * of the #hb_font_t object will be respectively set to the
 * @scale_factor times the xx and yy elements of the scale-matrix
 * of the cairo scaled-font being created.
 *
 * When using the hb_cairo_glyphs_from_buffer() API to convert the
 * HarfBuzz glyph buffer that resulted from shaping with such a #hb_font_t,
 * if the scale-factor was non-zero, you can pass it directly to
 * that API as both X and Y scale factors.
 *
 * If the scale-factor was zero however, or the cairo face was
 * created using the alternative constructor
 * hb_cairo_font_face_create_for_font(), you need to calculate the
 * correct X/Y scale-factors to pass to hb_cairo_glyphs_from_buffer()
 * by dividing the #hb_font_t X/Y scale-factors by the
 * cairo scaled-font's scale-matrix XX/YY components respectively
 * and use those values.  Or if you know that relationship offhand
 * (because you set the scale of the #hb_font_t yourself), use
 * the conversion rate involved.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_cairo_font_face_ਸੈੱਟ_scale_ਗੁਣਕ (cairo_font_face_t *font_face,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scale_ਗੁਣਕ)
{
  cairo_font_face_ਸੈੱਟ_user_ਅੰਕੜੇ (font_face,
				 &hb_cairo_scale_ਗੁਣਕ_user_ਅੰਕੜੇ_ਕੁੰਜੀ,
				 (ਖਾਲੀ *) (uintptr_t) scale_ਗੁਣਕ,
				 ਖਾਲੀ_ਸੂਚਕ);
}

/**
 * hb_cairo_font_face_get_scale_factor:
 * @font_face: a #cairo_font_face_t
 *
 * Gets the scale factor set on the @font_face. Defaults to zero.
 * See hb_cairo_font_face_set_scale_factor() for details.
 *
 * Returns: the scale factor of @font_face
 *
 * Since: 7.0.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_cairo_font_face_get_scale_ਗੁਣਕ (cairo_font_face_t *font_face)
{
  ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (uintptr_t)
	 cairo_font_face_get_user_ਅੰਕੜੇ (font_face,
					&hb_cairo_scale_ਗੁਣਕ_user_ਅੰਕੜੇ_ਕੁੰਜੀ);
}

/**
 * hb_cairo_glyphs_from_buffer:
 * @buffer: a #hb_buffer_t containing glyphs
 * @utf8_clusters: `true` if @buffer clusters are in bytes, instead of characters
 * @x_scale_factor: scale factor to divide #hb_position_t Y values by
 * @y_scale_factor: scale factor to divide #hb_position_t X values by
 * @x: X position to place first glyph
 * @y: Y position to place first glyph
 * @utf8: (nullable): the text that was shaped in @buffer
 * @utf8_len: the length of @utf8 in bytes
 * @glyphs: (out): return location for an array of #cairo_glyph_t
 * @num_glyphs: (inout): return location for the length of @glyphs
 * @clusters: (out) (nullable): return location for an array of cluster positions
 * @num_clusters: (inout) (nullable): return location for the length of @clusters
 * @cluster_flags: (out) (nullable): return location for cluster flags
 *
 * Extracts information from @buffer in a form that can be
 * passed to cairo_show_text_glyphs() or cairo_show_glyphs().
 * This API is modeled after cairo_scaled_font_text_to_glyphs() and
 * cairo_user_scaled_font_text_to_glyphs_func_t.
 *
 * The @num_glyphs argument should be preset to the number of glyph entries available
 * in the @glyphs buffer. If the @glyphs buffer is `NULL`, the value of
 * @num_glyphs must be zero.  If the provided glyph array is too short for
 * the conversion (or for convenience), a new glyph array may be allocated
 * using cairo_glyph_allocate() and placed in @glyphs.  Upon return,
 * @num_glyphs should contain the number of generated glyphs.  If the value
 * @glyphs points at has changed after the call, the caller will free the
 * allocated glyph array using cairo_glyph_free().  The caller will also free
 * the original value of @glyphs, so this function shouldn't do so.
 *
 * If @clusters is not `NULL`, then @num_clusters and @cluster_flags
 * should not be either, and @utf8 must be provided, and cluster
 * mapping will be computed. The semantics of how
 * cluster array allocation works is similar to the glyph array.  That is,
 * if @clusters initially points to a non-`NULL` value, that array may be used
 * as a cluster buffer, and @num_clusters points to the number of cluster
 * entries available there.  If the provided cluster array is too short for
 * the conversion (or for convenience), a new cluster array may be allocated
 * using cairo_text_cluster_allocate() and placed in @clusters.  In this case,
 * the original value of @clusters will still be freed by the caller.  Upon
 * return, @num_clusters will contain the number of generated clusters.
 * If the value @clusters points at has changed after the call, the caller
 * will free the allocated cluster array using cairo_text_cluster_free().
 *
 * See hb_cairo_font_face_set_scale_factor() for the details of
 * the @scale_factor argument.
 *
 * The returned @glyphs vector actually has `@num_glyphs + 1` entries in
 * it and the x,y values of the extra entry at the end add up the advance
 * x,y of all the glyphs in the @buffer.
 *
 * Since: 7.0.0
 */
ਖਾਲੀ
hb_cairo_glyphs_from_ਬਫਰ (hb_ਬਫਰ_t *ਬਫਰ,
			     hb_bool_t utf8_clusters,
			     ਦੁੱਗਣਾ x_scale_ਗੁਣਕ,
			     ਦੁੱਗਣਾ y_scale_ਗੁਣਕ,
			     ਦੁੱਗਣਾ x,
			     ਦੁੱਗਣਾ y,
			     ਅਚਲ ਅੱਖਰ *utf8,
			     ਪੂਰਨ utf8_len,
			     cairo_glyph_t **glyphs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_glyphs,
			     cairo_ਪਾਠ_cluster_t **clusters,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_clusters,
			     cairo_ਪਾਠ_cluster_flags_t *cluster_flags)
{
  ਜੇ (utf8 && utf8_len < 0)
    utf8_len = ਲੜੀ_ਲੰਬਾਈ (utf8);

  ਬਿਨਾਨਿਸ਼ਾਨੀ orig_ਅੰਕ_glyphs = *ਅੰਕ_glyphs;
  *ਅੰਕ_glyphs = hb_ਬਫਰ_get_ਲੰਮਾਈ (ਬਫਰ);
  hb_glyph_info_t *hb_glyph = hb_ਬਫਰ_get_glyph_infos (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  hb_glyph_position_t *hb_position = hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (orig_ਅੰਕ_glyphs < *ਅੰਕ_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*ਅੰਕ_glyphs + 1);

  ਜੇ (clusters && utf8)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ orig_ਅੰਕ_clusters = *ਅੰਕ_clusters;
    *ਅੰਕ_clusters = *ਅੰਕ_glyphs ? 1 : 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < *ਅੰਕ_glyphs; i++)
      ਜੇ (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*ਅੰਕ_clusters)++;
    ਜੇ (orig_ਅੰਕ_clusters < *ਅੰਕ_clusters)
      *clusters = cairo_ਪਾਠ_cluster_allocate (*ਅੰਕ_clusters);
  }

  ਦੁੱਗਣਾ x_scale = x_scale_ਗੁਣਕ ? 1. / x_scale_ਗੁਣਕ : 0.;
  ਦੁੱਗਣਾ y_scale = y_scale_ਗੁਣਕ ? 1. / y_scale_ਗੁਣਕ : 0.;
  ਦੁੱਗਣਾ hx = 0, hy = 0;
  ਪੂਰਨ i;
  ਲਈ (i = 0; i < (ਪੂਰਨ) *ਅੰਕ_glyphs; i++)
  {
    (*glyphs)[i].ਸੂਚਕਾਂਕ_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((ਦੁੱਗਣਾ) hb_position->x_ਵਿੱਥ_2 + hx) * x_scale;
    (*glyphs)[i].y = y + (-(ਦੁੱਗਣਾ) hb_position->y_ਵਿੱਥ_2 + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].ਸੂਚਕਾਂਕ_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  ਜੇ (clusters && *ਅੰਕ_clusters && utf8)
  {
    hb_memset ((ਖਾਲੀ *) *clusters, 0, *ਅੰਕ_clusters * ਆਕਾਰ ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_ਬਫਰ_get_direction (ਬਫਰ));
    *cluster_flags = backward ? CAIRO_ਪਾਠ_CLUSTER_FLAG_BACKWARD : (cairo_ਪਾਠ_cluster_flags_t) 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = 0;
    ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ = utf8, *ਸਿਰਾ;
    (*clusters)[cluster].ਅੰਕ_glyphs++;
    ਜੇ (backward)
    {
      ਲਈ (i = *ਅੰਕ_glyphs - 2; i >= 0; i--)
      {
	ਜੇ (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  ਪ੍ਰਤਿਜ੍ਞਾ (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  ਜੇ (utf8_clusters)
	    ਸਿਰਾ = ਸ਼ੁਰੂ + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  ਨਹੀਂਤਾਂ
	    ਸਿਰਾ = (ਅਚਲ ਅੱਖਰ *) hb_utf_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕ<hb_utf8_t> ((ਅਚਲ uint8_t *) ਸ਼ੁਰੂ,
								      (ਅਚਲ uint8_t *) utf8, utf8_len,
								      (ਨਿਸ਼ਾਨੀਵਾਲਾ) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].ਅੰਕ_ਬਾਈਟਾਂ = ਸਿਰਾ - ਸ਼ੁਰੂ;
	  ਸ਼ੁਰੂ = ਸਿਰਾ;
	  cluster++;
	}
	(*clusters)[cluster].ਅੰਕ_glyphs++;
      }
      (*clusters)[cluster].ਅੰਕ_ਬਾਈਟਾਂ = utf8 + utf8_len - ਸ਼ੁਰੂ;
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (i = 1; i < (ਪੂਰਨ) *ਅੰਕ_glyphs; i++)
      {
	ਜੇ (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  ਪ੍ਰਤਿਜ੍ਞਾ (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  ਜੇ (utf8_clusters)
	    ਸਿਰਾ = ਸ਼ੁਰੂ + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  ਨਹੀਂਤਾਂ
	    ਸਿਰਾ = (ਅਚਲ ਅੱਖਰ *) hb_utf_ਵਿੱਥ_ਮੰਜ਼ਿਲ_ਸੂਚਕ<hb_utf8_t> ((ਅਚਲ uint8_t *) ਸ਼ੁਰੂ,
								      (ਅਚਲ uint8_t *) utf8, utf8_len,
								      (ਨਿਸ਼ਾਨੀਵਾਲਾ) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].ਅੰਕ_ਬਾਈਟਾਂ = ਸਿਰਾ - ਸ਼ੁਰੂ;
	  ਸ਼ੁਰੂ = ਸਿਰਾ;
	  cluster++;
	}
	(*clusters)[cluster].ਅੰਕ_glyphs++;
      }
      (*clusters)[cluster].ਅੰਕ_ਬਾਈਟਾਂ = utf8 + utf8_len - ਸ਼ੁਰੂ;
    }
  }
  ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_clusters)
    *ਅੰਕ_clusters = 0;
}

#ਜੇ_ਅੰਤ
