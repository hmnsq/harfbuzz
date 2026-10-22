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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_CAIRO

#شامل "hb-cairo.h"

#شامل "hb-cairo-utils.hh"

#شامل "hb-machinery.hh"
#شامل "hb-utf.hh"


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

ساکن خالی
hb_cairo_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		  خالی *draw_ڈیٹا,
		  hb_draw_state_t *st HB_غیر_مستعمل,
		  اعشاری مقصد_x, اعشاری مقصد_y,
		  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ڈیٹا;
  اگر (unlikely (!c->spend (HB_BUDGET_1)))
    واپس;

  cairo_move_مقصد (c->cr, (دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_cairo_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		  خالی *draw_ڈیٹا,
		  hb_draw_state_t *st HB_غیر_مستعمل,
		  اعشاری مقصد_x, اعشاری مقصد_y,
		  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ڈیٹا;
  اگر (unlikely (!c->spend (HB_BUDGET_1)))
    واپس;

  cairo_سطر_مقصد (c->cr, (دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_cairo_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		   خالی *draw_ڈیٹا,
		   hb_draw_state_t *st HB_غیر_مستعمل,
		   اعشاری control1_x, اعشاری control1_y,
		   اعشاری control2_x, اعشاری control2_y,
		   اعشاری مقصد_x, اعشاری مقصد_y,
		   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ڈیٹا;
  اگر (unlikely (!c->spend (HB_BUDGET_1)))
    واپس;

  cairo_خم_مقصد (c->cr,
                  (دوہرا) control1_x, (دوہرا) control1_y,
                  (دوہرا) control2_x, (دوہرا) control2_y,
                  (دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_cairo_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		     خالی *draw_ڈیٹا,
		     hb_draw_state_t *st HB_غیر_مستعمل,
		     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_ڈیٹا;
  اگر (unlikely (!c->spend (HB_BUDGET_1)))
    واپس;

  cairo_close_راستہ (c->cr);
}

ساکن hb_bool_t
hb_cairo_draw_سیٹ_budget (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			  خالی *draw_ڈیٹا,
			  int64_t budget,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) draw_ڈیٹا)->سیٹ_budget (budget);
}

ساکن int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			  خالی *draw_ڈیٹا,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) draw_ڈیٹا)->get_budget ();
}

ساکن int64_t *
hb_cairo_draw_get_budget_بقایا (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
				    خالی *draw_ڈیٹا,
				    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) draw_ڈیٹا)->get_budget_بقایا ();
}

ساکن اندرونی خالی free_static_cairo_draw_funcs ();

ساکن ساخت hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func (funcs, hb_cairo_move_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func (funcs, hb_cairo_سطر_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func (funcs, hb_cairo_cubic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func (funcs, hb_cairo_close_راستہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_cairo_draw_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_cairo_draw_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_cairo_draw_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    واپس funcs;
  }
} static_cairo_draw_funcs;

ساکن اندرونی
خالی free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

ساکن hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  واپس static_cairo_draw_funcs.get_unconst ();
}


#اگر_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC

ساکن خالی
hb_cairo_ڈالو_transform (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			 خالی *paint_ڈیٹا,
			 اعشاری xx, اعشاری yx,
			 اعشاری xy, اعشاری yy,
			 اعشاری dx, اعشاری dy,
			 خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_میٹرکس_t m;

  cairo_save (cr);
  cairo_میٹرکس_init (&m, (دوہرا) xx, (دوہرا) yx,
                         (دوہرا) xy, (دوہرا) yy,
                         (دوہرا) dx, (دوہرا) dy);
  cairo_transform (cr, &m);
}

ساکن خالی
hb_cairo_نکالو_transform (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		        خالی *paint_ڈیٹا,
		        خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساکن خالی
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		     خالی *paint_ڈیٹا,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_راستہ (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_راستہ (cr);
  hb_cairo_سیٹ_ماخذ_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

ساکن hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			    خالی *paint_ڈیٹا,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  اگر (unlikely (!c->spend (HB_BUDGET_1)))
    واپس سچ;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_سیٹ_scaled_font (cr, c->scaled_font);
  cairo_سیٹ_font_سائز (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  واپس سچ;
}

ساکن خالی
hb_cairo_ڈالو_clip_glyph (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			  خالی *paint_ڈیٹا,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_راستہ (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_راستہ (cr);
  cairo_clip (cr);
}

ساکن خالی
hb_cairo_ڈالو_clip_rectangle (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			      خالی *paint_ڈیٹا,
			      اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax,
			      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (دوہرا) xmin, (دوہرا) ymin,
                   (دوہرا) (xmax - xmin), (دوہرا) (ymax - ymin));
  cairo_clip (cr);
}

ساکن hb_draw_funcs_t *
hb_cairo_ڈالو_clip_راستہ_آغاز (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			       خالی *paint_ڈیٹا,
			       خالی **draw_ڈیٹا,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_راستہ (cr);
  *draw_ڈیٹا = c;
  واپس hb_cairo_draw_get_funcs ();
}

ساکن خالی
hb_cairo_ڈالو_clip_راستہ_انتہا (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			     خالی *paint_ڈیٹا,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_close_راستہ (cr);
  cairo_clip (cr);
}

ساکن خالی
hb_cairo_نکالو_clip (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		   خالی *paint_ڈیٹا,
		   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساکن خالی
hb_cairo_ڈالو_گروہ (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		     خالی *paint_ڈیٹا,
		     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_ڈالو_گروہ (cr);
}

ساکن خالی
hb_cairo_نکالو_گروہ (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		    خالی *paint_ڈیٹا,
		    hb_paint_مرکب_منوال_t منوال,
		    خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  cairo_نکالو_گروہ_مقصد_ماخذ (cr);
  cairo_سیٹ_operator (cr, hb_paint_مرکب_منوال_مقصد_cairo (منوال));
  cairo_paint (cr);

  cairo_restore (cr);
}

ساکن خالی
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		      خالی *paint_ڈیٹا,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

  hb_cairo_سیٹ_ماخذ_color (c, use_foreground, color);
  cairo_paint (cr);
}

ساکن hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
		      خالی *paint_ڈیٹا,
		      hb_blob_t *blob,
		      غیرعلامتی چوڑائی,
		      غیرعلامتی اونچائی,
		      hb_لیبل_t format,
		      اعشاری slant,
		      hb_glyph_extents_t *extents,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;

  واپس _hb_cairo_paint_glyph_image (c, blob, چوڑائی, اونچائی, format, slant, extents);
}

ساکن خالی
hb_cairo_paint_خطی_gradient_2 (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
				خالی *paint_ڈیٹا,
				hb_color_سطر_t *color_سطر,
				اعشاری x0, اعشاری y0,
				اعشاری x1, اعشاری y1,
				اعشاری x2, اعشاری y2,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;

  hb_cairo_paint_خطی_gradient (c, color_سطر, x0, y0, x1, y1, x2, y2);
}

ساکن خالی
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
				خالی *paint_ڈیٹا,
				hb_color_سطر_t *color_سطر,
				اعشاری x0, اعشاری y0, اعشاری r0,
				اعشاری x1, اعشاری y1, اعشاری r1,
				خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;

  _hb_cairo_paint_radial_gradient (c, color_سطر, x0, y0, r0, x1, y1, r1);
}

ساکن خالی
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			       خالی *paint_ڈیٹا,
			       hb_color_سطر_t *color_سطر,
			       اعشاری x0, اعشاری y0,
			       اعشاری آغاز_angle, اعشاری انتہا_angle,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;

  _hb_cairo_paint_sweep_gradient (c, color_سطر, x0, y0, آغاز_angle, انتہا_angle);
}

ساکن ثابت cairo_user_ڈیٹا_کلید_t color_cache_کلید = {0};

ساکن خالی
_hb_cairo_destroy_map (خالی *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

ساکن hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     خالی *paint_ڈیٹا,
                                     غیرعلامتی صحیح color_اشاریہ,
                                     hb_color_t *color,
                                     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
#اگر_تعریف_شدہ HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_ڈیٹا;
  cairo_t *cr = c->cr;

#تعریف_کرو HB_DEADBEEF HB_لیبل(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  اگر (likely (color_cache && color_cache->has (color_اشاریہ, &v)))
  {
    اگر (*v == HB_DEADBEEF)
      واپس جھوٹ;
    *color = *v;
    واپس سچ;
  }

  cairo_font_options_t *options;
  دوہرا red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  اگر (CAIRO_حالت_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_اشاریہ,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    اگر (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->سیٹ (color_اشاریہ, *color);

    واپس سچ;
  }
  cairo_font_options_destroy (options);

  اگر (likely (color_cache))
    color_cache->سیٹ (color_اشاریہ, HB_DEADBEEF);

#تعریف_منسوخ HB_DEADBEEF

#اگر_ختم

  واپس جھوٹ;
}

ساکن hb_bool_t
hb_cairo_paint_سیٹ_budget (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			   خالی *paint_ڈیٹا,
			   int64_t budget,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) paint_ڈیٹا)->سیٹ_budget (budget);
}

ساکن int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
			   خالی *paint_ڈیٹا,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) paint_ڈیٹا)->get_budget ();
}

ساکن int64_t *
hb_cairo_paint_get_budget_بقایا (hb_paint_funcs_t *pfuncs HB_غیر_مستعمل,
				     خالی *paint_ڈیٹا,
				     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس ((hb_cairo_context_t *) paint_ڈیٹا)->get_budget_بقایا ();
}

ساکن اندرونی خالی free_static_cairo_paint_funcs ();

ساکن ساخت hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, hb_cairo_ڈالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, hb_cairo_نکالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, hb_cairo_fill_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_glyph_func (funcs, hb_cairo_paint_color_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_cairo_ڈالو_clip_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_cairo_ڈالو_clip_rectangle, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_آغاز_func (funcs, hb_cairo_ڈالو_clip_راستہ_آغاز, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_انتہا_func (funcs, hb_cairo_ڈالو_clip_راستہ_انتہا, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_cairo_نکالو_clip, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_cairo_ڈالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_cairo_نکالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_cairo_paint_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_cairo_paint_image, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_cairo_paint_خطی_gradient_2, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_cairo_paint_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_cairo_paint_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_cairo_paint_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_paint_funcs);

    واپس funcs;
  }
} static_cairo_paint_funcs;

ساکن اندرونی
خالی free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

ساکن hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  واپس static_cairo_paint_funcs.get_unconst ();
}
#اگر_ختم

ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_face_user_ڈیٹا_کلید = {0};
ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_font_user_ڈیٹا_کلید = {0};
ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_font_init_func_user_ڈیٹا_کلید = {0};
ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_font_init_user_ڈیٹا_user_ڈیٹا_کلید = {0};
ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_scale_ضربی_عدد_user_ڈیٹا_کلید = {0};
ساکن ثابت cairo_user_ڈیٹا_کلید_t hb_cairo_budget_user_ڈیٹا_کلید = {0};

ساکن خالی hb_cairo_face_destroy (خالی *p) { hb_face_destroy ((hb_face_t *) p); }
ساکن خالی hb_cairo_font_destroy (خالی *p) { hb_font_destroy ((hb_font_t *) p); }
ساکن خالی
hb_cairo_budget_destroy (خالی *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

ساکن cairo_حالت_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_غیر_مستعمل,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_ڈیٹا (font_face,
								 &hb_cairo_font_user_ڈیٹا_کلید);

  اگر (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_ڈیٹا (font_face,
								   &hb_cairo_face_user_ڈیٹا_کلید);
    font = hb_font_create (face);

#اگر !defined(HB_NO_VAR) && CAIRO_نسخہ >= CAIRO_نسخہ_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    ثابت حرف *variations = cairo_font_options_get_variations (font_options);
    hb_سمتیہ_t<hb_variation_t> vars;
    ثابت حرف *p = variations;
    جب (p && *p)
    {
      ثابت حرف *انتہا = strpbrk ((حرف *) p, ", ");
      hb_variation_t var;
      اگر (hb_variation_from_string (p, انتہا ? انتہا - p : -1, &var))
	vars.push (var);
      p = انتہا ? انتہا + 1 : خالی_اشاریہ;
    }
    hb_font_سیٹ_variations (font, &vars[0], vars.لمبائی);

    cairo_font_options_destroy (font_options);
#اگر_ختم

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    غیرعلامتی scale_ضربی_عدد = hb_cairo_font_face_get_scale_ضربی_عدد (font_face);
    اگر (scale_ضربی_عدد)
    {
      cairo_میٹرکس_t font_میٹرکس;
      cairo_scaled_font_get_scale_میٹرکس (scaled_font, &font_میٹرکس);
      hb_font_سیٹ_scale (font,
			 hb_clamp_مقصد<int32_t> (round (font_میٹرکس.xx * scale_ضربی_عدد)),
			 hb_clamp_مقصد<int32_t> (round (font_میٹرکس.yy * scale_ضربی_عدد)));
    }

    خودکار *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_ڈیٹا (font_face,
						     &hb_cairo_font_init_func_user_ڈیٹا_کلید);
    اگر (init_func)
    {
      خالی *user_ڈیٹا = cairo_font_face_get_user_ڈیٹا (font_face,
						       &hb_cairo_font_init_user_ڈیٹا_user_ڈیٹا_کلید);
      font = init_func (font, scaled_font, user_ڈیٹا);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_سیٹ_user_ڈیٹا (scaled_font,
				   &hb_cairo_font_user_ڈیٹا_کلید,
				   (خالی *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (حجم (hb_cairo_budget_t));
  اگر (unlikely (!budget))
    واپس CAIRO_حالت_NO_حافظہ;
  نیا (budget) hb_cairo_budget_t ();
  cairo_حالت_t حالت_2 = cairo_scaled_font_سیٹ_user_ڈیٹا (scaled_font,
						   &hb_cairo_budget_user_ڈیٹا_کلید,
						   budget,
						   hb_cairo_budget_destroy);
  اگر (unlikely (حالت_2 != CAIRO_حالت_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    واپس حالت_2;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (دوہرا)  hb_extents.ascender  / y_scale;
  extents->descent = -(دوہرا) hb_extents.descender / y_scale;
  extents->اونچائی  = extents->ascent + extents->descent;

#اگر_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  اگر (unlikely (CAIRO_حالت_SUCCESS != cairo_scaled_font_سیٹ_user_ڈیٹا (scaled_font,
									 &color_cache_کلید,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#اگر_ختم

  واپس CAIRO_حالت_SUCCESS;
}

ساکن cairo_حالت_t
hb_cairo_متن_مقصد_glyphs (cairo_scaled_font_t        *scaled_font,
			 ثابت حرف	            *utf8,
			 صحیح		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 صحیح		            *عدد_glyphs,
			 cairo_متن_cluster_t      **clusters,
			 صحیح		            *عدد_clusters,
			 cairo_متن_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font,
								   &hb_cairo_font_user_ڈیٹا_کلید);

  hb_بفر_t *بفر = hb_بفر_create ();
  hb_بفر_جوڑو_utf8 (بفر, utf8, utf8_len, 0, utf8_len);
  hb_بفر_guess_segment_properties (بفر);
  hb_shape (font, بفر, خالی_اشاریہ, 0);

  صحیح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_بفر (بفر,
			       سچ,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (غیرعلامتی *) عدد_glyphs,
			       clusters, (غیرعلامتی *) عدد_clusters,
			       cluster_flags);

  hb_بفر_destroy (بفر);

  واپس CAIRO_حالت_SUCCESS;
}

ساکن cairo_حالت_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       غیرعلامتی طویل         glyph,
		       cairo_t              *cr,
		       cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font,
								   &hb_cairo_font_user_ڈیٹا_کلید);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font,
									     &hb_cairo_budget_user_ڈیٹا_کلید);
  اگر (unlikely (!budget))
    واپس CAIRO_حالت_NO_حافظہ;
  hb_cairo_context_t c {scaled_font, cr, خالی_اشاریہ, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  اگر (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  واپس CAIRO_حالت_SUCCESS;
}

#اگر_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC

ساکن cairo_حالت_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     غیرعلامتی طویل         glyph,
			     cairo_t              *cr,
			     cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font,
								   &hb_cairo_font_user_ڈیٹا_کلید);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font,
									     &hb_cairo_budget_user_ڈیٹا_کلید);
  اگر (unlikely (!budget))
    واپس CAIRO_حالت_NO_حافظہ;

  غیرعلامتی صحیح palette = 0;
#اگر_تعریف_شدہ CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#اگر_ختم

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font, &color_cache_کلید),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  واپس CAIRO_حالت_SUCCESS;
}

#اگر_ختم

ساکن cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_سیٹ_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_سیٹ_متن_مقصد_glyphs_func (cairo_face, hb_cairo_متن_مقصد_glyphs);
  cairo_user_font_face_سیٹ_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#اگر_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_سیٹ_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#اگر_ختم

  اگر (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_ڈیٹا (cairo_face,
								       &hb_cairo_face_user_ڈیٹا_کلید,
								       (خالی *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  واپس cairo_face;
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

  خودکار *hb_face = hb_font_get_face (font);
  خودکار *cairo_face =  user_font_face_create (hb_face);

  اگر (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_ڈیٹا (cairo_face,
								       &hb_cairo_font_user_ڈیٹا_کلید,
								       (خالی *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  واپس cairo_face;
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
  واپس (hb_font_t *) cairo_font_face_get_user_ڈیٹا (font_face,
						      &hb_cairo_font_user_ڈیٹا_کلید);
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

  واپس user_font_face_create (face);
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
  واپس (hb_face_t *) cairo_font_face_get_user_ڈیٹا (font_face,
						      &hb_cairo_face_user_ڈیٹا_کلید);
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
خالی
hb_cairo_font_face_سیٹ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       خالی *user_ڈیٹا,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_سیٹ_user_ڈیٹا (font_face,
				 &hb_cairo_font_init_func_user_ڈیٹا_کلید,
				 (خالی *) func,
				 خالی_اشاریہ);
  اگر (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_ڈیٹا (font_face,
								       &hb_cairo_font_init_user_ڈیٹا_user_ڈیٹا_کلید,
								       (خالی *) user_ڈیٹا,
								       destroy)) && destroy)
  {
    destroy (user_ڈیٹا);
    cairo_font_face_سیٹ_user_ڈیٹا (font_face,
				   &hb_cairo_font_init_func_user_ڈیٹا_کلید,
				   خالی_اشاریہ,
				   خالی_اشاریہ);
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
  واپس (hb_font_t *) cairo_scaled_font_get_user_ڈیٹا (scaled_font, &hb_cairo_font_user_ڈیٹا_کلید);
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
خالی
hb_cairo_font_face_سیٹ_scale_ضربی_عدد (cairo_font_face_t *font_face,
				     غیرعلامتی صحیح scale_ضربی_عدد)
{
  cairo_font_face_سیٹ_user_ڈیٹا (font_face,
				 &hb_cairo_scale_ضربی_عدد_user_ڈیٹا_کلید,
				 (خالی *) (uintptr_t) scale_ضربی_عدد,
				 خالی_اشاریہ);
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
غیرعلامتی صحیح
hb_cairo_font_face_get_scale_ضربی_عدد (cairo_font_face_t *font_face)
{
  واپس (غیرعلامتی صحیح) (uintptr_t)
	 cairo_font_face_get_user_ڈیٹا (font_face,
					&hb_cairo_scale_ضربی_عدد_user_ڈیٹا_کلید);
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
خالی
hb_cairo_glyphs_from_بفر (hb_بفر_t *بفر,
			     hb_bool_t utf8_clusters,
			     دوہرا x_scale_ضربی_عدد,
			     دوہرا y_scale_ضربی_عدد,
			     دوہرا x,
			     دوہرا y,
			     ثابت حرف *utf8,
			     صحیح utf8_len,
			     cairo_glyph_t **glyphs,
			     غیرعلامتی صحیح *عدد_glyphs,
			     cairo_متن_cluster_t **clusters,
			     غیرعلامتی صحیح *عدد_clusters,
			     cairo_متن_cluster_flags_t *cluster_flags)
{
  اگر (utf8 && utf8_len < 0)
    utf8_len = لڑی_لمبائی (utf8);

  غیرعلامتی orig_عدد_glyphs = *عدد_glyphs;
  *عدد_glyphs = hb_بفر_get_لمبائی (بفر);
  hb_glyph_info_t *hb_glyph = hb_بفر_get_glyph_infos (بفر, خالی_اشاریہ);
  hb_glyph_position_t *hb_position = hb_بفر_get_glyph_positions (بفر, خالی_اشاریہ);
  اگر (orig_عدد_glyphs < *عدد_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*عدد_glyphs + 1);

  اگر (clusters && utf8)
  {
    غیرعلامتی orig_عدد_clusters = *عدد_clusters;
    *عدد_clusters = *عدد_glyphs ? 1 : 0;
    برائے (غیرعلامتی صحیح i = 1; i < *عدد_glyphs; i++)
      اگر (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*عدد_clusters)++;
    اگر (orig_عدد_clusters < *عدد_clusters)
      *clusters = cairo_متن_cluster_allocate (*عدد_clusters);
  }

  دوہرا x_scale = x_scale_ضربی_عدد ? 1. / x_scale_ضربی_عدد : 0.;
  دوہرا y_scale = y_scale_ضربی_عدد ? 1. / y_scale_ضربی_عدد : 0.;
  دوہرا hx = 0, hy = 0;
  صحیح i;
  برائے (i = 0; i < (صحیح) *عدد_glyphs; i++)
  {
    (*glyphs)[i].اشاریہ_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((دوہرا) hb_position->x_انحراف_2 + hx) * x_scale;
    (*glyphs)[i].y = y + (-(دوہرا) hb_position->y_انحراف_2 + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].اشاریہ_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  اگر (clusters && *عدد_clusters && utf8)
  {
    hb_memset ((خالی *) *clusters, 0, *عدد_clusters * حجم ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_بفر_get_direction (بفر));
    *cluster_flags = backward ? CAIRO_متن_CLUSTER_FLAG_BACKWARD : (cairo_متن_cluster_flags_t) 0;
    غیرعلامتی صحیح cluster = 0;
    ثابت حرف *آغاز = utf8, *انتہا;
    (*clusters)[cluster].عدد_glyphs++;
    اگر (backward)
    {
      برائے (i = *عدد_glyphs - 2; i >= 0; i--)
      {
	اگر (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  اگر (utf8_clusters)
	    انتہا = آغاز + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  ورنہ
	    انتہا = (ثابت حرف *) hb_utf_انحراف_مقصد_اشارہ_گر<hb_utf8_t> ((ثابت uint8_t *) آغاز,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (علامتی) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].عدد_بائٹیں = انتہا - آغاز;
	  آغاز = انتہا;
	  cluster++;
	}
	(*clusters)[cluster].عدد_glyphs++;
      }
      (*clusters)[cluster].عدد_بائٹیں = utf8 + utf8_len - آغاز;
    }
    ورنہ
    {
      برائے (i = 1; i < (صحیح) *عدد_glyphs; i++)
      {
	اگر (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  اگر (utf8_clusters)
	    انتہا = آغاز + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  ورنہ
	    انتہا = (ثابت حرف *) hb_utf_انحراف_مقصد_اشارہ_گر<hb_utf8_t> ((ثابت uint8_t *) آغاز,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (علامتی) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].عدد_بائٹیں = انتہا - آغاز;
	  آغاز = انتہا;
	  cluster++;
	}
	(*clusters)[cluster].عدد_glyphs++;
      }
      (*clusters)[cluster].عدد_بائٹیں = utf8 + utf8_len - آغاز;
    }
  }
  ورنہ اگر (عدد_clusters)
    *عدد_clusters = 0;
}

#اگر_ختم
