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

#ضمن "hb.hh"

#جے_تعریف_شدہ HAVE_CAIRO

#ضمن "hb-cairo.h"

#ضمن "hb-cairo-utils.hh"

#ضمن "hb-machinery.hh"
#ضمن "hb-utf.hh"


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

ساكن فراغ
hb_cairo_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		  فراغ *draw_انکڑے,
		  hb_draw_state_t *st HB_بن_ورتیا,
		  عائم منزل_x, عائم منزل_y,
		  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_انکڑے;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_move_منزل (c->cr, (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		  فراغ *draw_انکڑے,
		  hb_draw_state_t *st HB_بن_ورتیا,
		  عائم منزل_x, عائم منزل_y,
		  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_انکڑے;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_سطر_منزل (c->cr, (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		   فراغ *draw_انکڑے,
		   hb_draw_state_t *st HB_بن_ورتیا,
		   عائم control1_x, عائم control1_y,
		   عائم control2_x, عائم control2_y,
		   عائم منزل_x, عائم منزل_y,
		   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_انکڑے;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_ونگ_منزل (c->cr,
                  (مزدوج) control1_x, (مزدوج) control1_y,
                  (مزدوج) control2_x, (مزدوج) control2_y,
                  (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_close_رستہ (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
		     فراغ *draw_انکڑے,
		     hb_draw_state_t *st HB_بن_ورتیا,
		     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_انکڑے;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_close_رستہ (c->cr);
}

ساكن hb_bool_t
hb_cairo_draw_سیٹ_budget (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			  فراغ *draw_انکڑے,
			  int64_t budget,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) draw_انکڑے)->سیٹ_budget (budget);
}

ساكن int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			  فراغ *draw_انکڑے,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) draw_انکڑے)->get_budget ();
}

ساكن int64_t *
hb_cairo_draw_get_budget_بچیا (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
				    فراغ *draw_انکڑے,
				    فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) draw_انکڑے)->get_budget_بچیا ();
}

ساكن مضمن فراغ free_static_cairo_draw_funcs ();

ساكن بنية hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_منزل_func (funcs, hb_cairo_move_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func (funcs, hb_cairo_سطر_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func (funcs, hb_cairo_cubic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func (funcs, hb_cairo_close_رستہ, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_cairo_draw_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_cairo_draw_get_budget, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_cairo_draw_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    أرجع funcs;
  }
} static_cairo_draw_funcs;

ساكن مضمن
فراغ free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

ساكن hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  أرجع static_cairo_draw_funcs.get_unconst ();
}


#جے_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC

ساكن فراغ
hb_cairo_ڈالو_transform (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			 فراغ *paint_انکڑے,
			 عائم xx, عائم yx,
			 عائم xy, عائم yy,
			 عائم dx, عائم dy,
			 فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_میٹرکس_t m;

  cairo_save (cr);
  cairo_میٹرکس_init (&m, (مزدوج) xx, (مزدوج) yx,
                         (مزدوج) xy, (مزدوج) yy,
                         (مزدوج) dx, (مزدوج) dy);
  cairo_transform (cr, &m);
}

ساكن فراغ
hb_cairo_نکالو_transform (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		        فراغ *paint_انکڑے,
		        فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		     فراغ *paint_انکڑے,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_رستہ (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_رستہ (cr);
  hb_cairo_سیٹ_سروت_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

ساكن hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			    فراغ *paint_انکڑے,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع سچ;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_سیٹ_scaled_font (cr, c->scaled_font);
  cairo_سیٹ_font_ناپ (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  أرجع سچ;
}

ساكن فراغ
hb_cairo_ڈالو_clip_glyph (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			  فراغ *paint_انکڑے,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_رستہ (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_رستہ (cr);
  cairo_clip (cr);
}

ساكن فراغ
hb_cairo_ڈالو_clip_rectangle (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			      فراغ *paint_انکڑے,
			      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
			      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (مزدوج) xmin, (مزدوج) ymin,
                   (مزدوج) (xmax - xmin), (مزدوج) (ymax - ymin));
  cairo_clip (cr);
}

ساكن hb_draw_funcs_t *
hb_cairo_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			       فراغ *paint_انکڑے,
			       فراغ **draw_انکڑے,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_رستہ (cr);
  *draw_انکڑے = c;
  أرجع hb_cairo_draw_get_funcs ();
}

ساكن فراغ
hb_cairo_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			     فراغ *paint_انکڑے,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_close_رستہ (cr);
  cairo_clip (cr);
}

ساكن فراغ
hb_cairo_نکالو_clip (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		   فراغ *paint_انکڑے,
		   فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_ڈالو_گروہ (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		     فراغ *paint_انکڑے,
		     فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_ڈالو_گروہ (cr);
}

ساكن فراغ
hb_cairo_نکالو_گروہ (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		    فراغ *paint_انکڑے,
		    hb_paint_مرکب_منوال_t منوال,
		    فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  cairo_نکالو_گروہ_منزل_سروت (cr);
  cairo_سیٹ_operator (cr, hb_paint_مرکب_منوال_منزل_cairo (منوال));
  cairo_paint (cr);

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		      فراغ *paint_انکڑے,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

  hb_cairo_سیٹ_سروت_color (c, use_foreground, color);
  cairo_paint (cr);
}

ساكن hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
		      فراغ *paint_انکڑے,
		      hb_blob_t *blob,
		      بنانشانی چوڑائی,
		      بنانشانی اچائی,
		      hb_ٹیگ_t format,
		      عائم slant,
		      hb_glyph_extents_t *extents,
		      فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;

  أرجع _hb_cairo_paint_glyph_image (c, blob, چوڑائی, اچائی, format, slant, extents);
}

ساكن فراغ
hb_cairo_paint_خطی_gradient_2 (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				فراغ *paint_انکڑے,
				hb_color_سطر_t *color_سطر,
				عائم x0, عائم y0,
				عائم x1, عائم y1,
				عائم x2, عائم y2,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;

  hb_cairo_paint_خطی_gradient (c, color_سطر, x0, y0, x1, y1, x2, y2);
}

ساكن فراغ
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				فراغ *paint_انکڑے,
				hb_color_سطر_t *color_سطر,
				عائم x0, عائم y0, عائم r0,
				عائم x1, عائم y1, عائم r1,
				فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;

  _hb_cairo_paint_radial_gradient (c, color_سطر, x0, y0, r0, x1, y1, r1);
}

ساكن فراغ
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			       فراغ *paint_انکڑے,
			       hb_color_سطر_t *color_سطر,
			       عائم x0, عائم y0,
			       عائم شروع_angle, عائم سرا_angle,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;

  _hb_cairo_paint_sweep_gradient (c, color_سطر, x0, y0, شروع_angle, سرا_angle);
}

ساكن ثابت cairo_user_انکڑے_کلید_t color_cache_کلید = {0};

ساكن فراغ
_hb_cairo_destroy_map (فراغ *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

ساكن hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     فراغ *paint_انکڑے,
                                     بنانشانی صحيح color_اشاریہ,
                                     hb_color_t *color,
                                     فراغ *user_انکڑے HB_بن_ورتیا)
{
#جے_تعریف_شدہ HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_انکڑے;
  cairo_t *cr = c->cr;

#تعریف_کرو HB_DEADBEEF HB_ٹیگ(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  إذا (likely (color_cache && color_cache->has (color_اشاریہ, &v)))
  {
    إذا (*v == HB_DEADBEEF)
      أرجع خطأ;
    *color = *v;
    أرجع سچ;
  }

  cairo_font_options_t *options;
  مزدوج red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  إذا (CAIRO_حالت_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_اشاریہ,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    إذا (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->سیٹ (color_اشاریہ, *color);

    أرجع سچ;
  }
  cairo_font_options_destroy (options);

  إذا (likely (color_cache))
    color_cache->سیٹ (color_اشاریہ, HB_DEADBEEF);

#تعریف_رد HB_DEADBEEF

#جے_انت

  أرجع خطأ;
}

ساكن hb_bool_t
hb_cairo_paint_سیٹ_budget (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			   فراغ *paint_انکڑے,
			   int64_t budget,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) paint_انکڑے)->سیٹ_budget (budget);
}

ساكن int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
			   فراغ *paint_انکڑے,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) paint_انکڑے)->get_budget ();
}

ساكن int64_t *
hb_cairo_paint_get_budget_بچیا (hb_paint_funcs_t *pfuncs HB_بن_ورتیا,
				     فراغ *paint_انکڑے,
				     فراغ *user_انکڑے HB_بن_ورتیا)
{
  أرجع ((hb_cairo_context_t *) paint_انکڑے)->get_budget_بچیا ();
}

ساكن مضمن فراغ free_static_cairo_paint_funcs ();

ساكن بنية hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, hb_cairo_ڈالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, hb_cairo_نکالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, hb_cairo_fill_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_glyph_func (funcs, hb_cairo_paint_color_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, hb_cairo_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, hb_cairo_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_شروع_func (funcs, hb_cairo_ڈالو_clip_رستہ_شروع, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_سرا_func (funcs, hb_cairo_ڈالو_clip_رستہ_سرا, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, hb_cairo_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, hb_cairo_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, hb_cairo_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, hb_cairo_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, hb_cairo_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, hb_cairo_paint_خطی_gradient_2, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_cairo_paint_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_cairo_paint_get_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_cairo_paint_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_paint_funcs);

    أرجع funcs;
  }
} static_cairo_paint_funcs;

ساكن مضمن
فراغ free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

ساكن hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  أرجع static_cairo_paint_funcs.get_unconst ();
}
#جے_انت

ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_face_user_انکڑے_کلید = {0};
ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_font_user_انکڑے_کلید = {0};
ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_font_init_func_user_انکڑے_کلید = {0};
ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_font_init_user_انکڑے_user_انکڑے_کلید = {0};
ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_scale_ضارب_user_انکڑے_کلید = {0};
ساكن ثابت cairo_user_انکڑے_کلید_t hb_cairo_budget_user_انکڑے_کلید = {0};

ساكن فراغ hb_cairo_face_destroy (فراغ *p) { hb_face_destroy ((hb_face_t *) p); }
ساكن فراغ hb_cairo_font_destroy (فراغ *p) { hb_font_destroy ((hb_font_t *) p); }
ساكن فراغ
hb_cairo_budget_destroy (فراغ *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

ساكن cairo_حالت_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_بن_ورتیا,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_انکڑے (font_face,
								 &hb_cairo_font_user_انکڑے_کلید);

  إذا (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_انکڑے (font_face,
								   &hb_cairo_face_user_انکڑے_کلید);
    font = hb_font_create (face);

#إذا !defined(HB_NO_VAR) && CAIRO_نسخہ >= CAIRO_نسخہ_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    ثابت حرف *variations = cairo_font_options_get_variations (font_options);
    hb_سمتیہ_t<hb_variation_t> vars;
    ثابت حرف *p = variations;
    طالما (p && *p)
    {
      ثابت حرف *سرا = strpbrk ((حرف *) p, ", ");
      hb_variation_t var;
      إذا (hb_variation_from_string (p, سرا ? سرا - p : -1, &var))
	vars.push (var);
      p = سرا ? سرا + 1 : خالی_سوچک;
    }
    hb_font_سیٹ_variations (font, &vars[0], vars.لمائی);

    cairo_font_options_destroy (font_options);
#جے_انت

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    بنانشانی scale_ضارب = hb_cairo_font_face_get_scale_ضارب (font_face);
    إذا (scale_ضارب)
    {
      cairo_میٹرکس_t font_میٹرکس;
      cairo_scaled_font_get_scale_میٹرکس (scaled_font, &font_میٹرکس);
      hb_font_سیٹ_scale (font,
			 hb_clamp_منزل<int32_t> (round (font_میٹرکس.xx * scale_ضارب)),
			 hb_clamp_منزل<int32_t> (round (font_میٹرکس.yy * scale_ضارب)));
    }

    خودکار *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_انکڑے (font_face,
						     &hb_cairo_font_init_func_user_انکڑے_کلید);
    إذا (init_func)
    {
      فراغ *user_انکڑے = cairo_font_face_get_user_انکڑے (font_face,
						       &hb_cairo_font_init_user_انکڑے_user_انکڑے_کلید);
      font = init_func (font, scaled_font, user_انکڑے);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_سیٹ_user_انکڑے (scaled_font,
				   &hb_cairo_font_user_انکڑے_کلید,
				   (فراغ *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (حجم (hb_cairo_budget_t));
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_حافظہ;
  جديد (budget) hb_cairo_budget_t ();
  cairo_حالت_t حالت = cairo_scaled_font_سیٹ_user_انکڑے (scaled_font,
						   &hb_cairo_budget_user_انکڑے_کلید,
						   budget,
						   hb_cairo_budget_destroy);
  إذا (unlikely (حالت != CAIRO_حالت_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    أرجع حالت;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (مزدوج)  hb_extents.ascender  / y_scale;
  extents->descent = -(مزدوج) hb_extents.descender / y_scale;
  extents->اچائی  = extents->ascent + extents->descent;

#جے_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_scaled_font_سیٹ_user_انکڑے (scaled_font,
									 &color_cache_کلید,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#جے_انت

  أرجع CAIRO_حالت_SUCCESS;
}

ساكن cairo_حالت_t
hb_cairo_متن_منزل_glyphs (cairo_scaled_font_t        *scaled_font,
			 ثابت حرف	            *utf8,
			 صحيح		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 صحيح		            *عدد_glyphs,
			 cairo_متن_cluster_t      **clusters,
			 صحيح		            *عدد_clusters,
			 cairo_متن_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_انکڑے (scaled_font,
								   &hb_cairo_font_user_انکڑے_کلید);

  hb_بفر_t *بفر = hb_بفر_create ();
  hb_بفر_جوڑو_utf8 (بفر, utf8, utf8_len, 0, utf8_len);
  hb_بفر_guess_segment_properties (بفر);
  hb_shape (font, بفر, خالی_سوچک, 0);

  صحيح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_بفر (بفر,
			       سچ,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (بنانشانی *) عدد_glyphs,
			       clusters, (بنانشانی *) عدد_clusters,
			       cluster_flags);

  hb_بفر_destroy (بفر);

  أرجع CAIRO_حالت_SUCCESS;
}

ساكن cairo_حالت_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       بنانشانی لما         glyph,
		       cairo_t              *cr,
		       cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_انکڑے (scaled_font,
								   &hb_cairo_font_user_انکڑے_کلید);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_انکڑے (scaled_font,
									     &hb_cairo_budget_user_انکڑے_کلید);
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_حافظہ;
  hb_cairo_context_t c {scaled_font, cr, خالی_سوچک, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  إذا (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  أرجع CAIRO_حالت_SUCCESS;
}

#جے_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC

ساكن cairo_حالت_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     بنانشانی لما         glyph,
			     cairo_t              *cr,
			     cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_انکڑے (scaled_font,
								   &hb_cairo_font_user_انکڑے_کلید);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_انکڑے (scaled_font,
									     &hb_cairo_budget_user_انکڑے_کلید);
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_حافظہ;

  بنانشانی صحيح palette = 0;
#جے_تعریف_شدہ CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#جے_انت

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_انکڑے (scaled_font, &color_cache_کلید),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  أرجع CAIRO_حالت_SUCCESS;
}

#جے_انت

ساكن cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_سیٹ_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_سیٹ_متن_منزل_glyphs_func (cairo_face, hb_cairo_متن_منزل_glyphs);
  cairo_user_font_face_سیٹ_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#جے_تعریف_شدہ HAVE_CAIRO_USER_FONT_FACE_سیٹ_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_سیٹ_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#جے_انت

  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_انکڑے (cairo_face,
								       &hb_cairo_face_user_انکڑے_کلید,
								       (فراغ *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  أرجع cairo_face;
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

  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_انکڑے (cairo_face,
								       &hb_cairo_font_user_انکڑے_کلید,
								       (فراغ *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  أرجع cairo_face;
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
  أرجع (hb_font_t *) cairo_font_face_get_user_انکڑے (font_face,
						      &hb_cairo_font_user_انکڑے_کلید);
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

  أرجع user_font_face_create (face);
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
  أرجع (hb_face_t *) cairo_font_face_get_user_انکڑے (font_face,
						      &hb_cairo_face_user_انکڑے_کلید);
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
فراغ
hb_cairo_font_face_سیٹ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       فراغ *user_انکڑے,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_سیٹ_user_انکڑے (font_face,
				 &hb_cairo_font_init_func_user_انکڑے_کلید,
				 (فراغ *) func,
				 خالی_سوچک);
  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سیٹ_user_انکڑے (font_face,
								       &hb_cairo_font_init_user_انکڑے_user_انکڑے_کلید,
								       (فراغ *) user_انکڑے,
								       destroy)) && destroy)
  {
    destroy (user_انکڑے);
    cairo_font_face_سیٹ_user_انکڑے (font_face,
				   &hb_cairo_font_init_func_user_انکڑے_کلید,
				   خالی_سوچک,
				   خالی_سوچک);
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
  أرجع (hb_font_t *) cairo_scaled_font_get_user_انکڑے (scaled_font, &hb_cairo_font_user_انکڑے_کلید);
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
فراغ
hb_cairo_font_face_سیٹ_scale_ضارب (cairo_font_face_t *font_face,
				     بنانشانی صحيح scale_ضارب)
{
  cairo_font_face_سیٹ_user_انکڑے (font_face,
				 &hb_cairo_scale_ضارب_user_انکڑے_کلید,
				 (فراغ *) (uintptr_t) scale_ضارب,
				 خالی_سوچک);
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
بنانشانی صحيح
hb_cairo_font_face_get_scale_ضارب (cairo_font_face_t *font_face)
{
  أرجع (بنانشانی صحيح) (uintptr_t)
	 cairo_font_face_get_user_انکڑے (font_face,
					&hb_cairo_scale_ضارب_user_انکڑے_کلید);
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
فراغ
hb_cairo_glyphs_from_بفر (hb_بفر_t *بفر,
			     hb_bool_t utf8_clusters,
			     مزدوج x_scale_ضارب,
			     مزدوج y_scale_ضارب,
			     مزدوج x,
			     مزدوج y,
			     ثابت حرف *utf8,
			     صحيح utf8_len,
			     cairo_glyph_t **glyphs,
			     بنانشانی صحيح *عدد_glyphs,
			     cairo_متن_cluster_t **clusters,
			     بنانشانی صحيح *عدد_clusters,
			     cairo_متن_cluster_flags_t *cluster_flags)
{
  إذا (utf8 && utf8_len < 0)
    utf8_len = لڑی_لمبائی (utf8);

  بنانشانی orig_عدد_glyphs = *عدد_glyphs;
  *عدد_glyphs = hb_بفر_get_لمائی (بفر);
  hb_glyph_info_t *hb_glyph = hb_بفر_get_glyph_infos (بفر, خالی_سوچک);
  hb_glyph_position_t *hb_position = hb_بفر_get_glyph_positions (بفر, خالی_سوچک);
  إذا (orig_عدد_glyphs < *عدد_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*عدد_glyphs + 1);

  إذا (clusters && utf8)
  {
    بنانشانی orig_عدد_clusters = *عدد_clusters;
    *عدد_clusters = *عدد_glyphs ? 1 : 0;
    لكل (بنانشانی صحيح i = 1; i < *عدد_glyphs; i++)
      إذا (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*عدد_clusters)++;
    إذا (orig_عدد_clusters < *عدد_clusters)
      *clusters = cairo_متن_cluster_allocate (*عدد_clusters);
  }

  مزدوج x_scale = x_scale_ضارب ? 1. / x_scale_ضارب : 0.;
  مزدوج y_scale = y_scale_ضارب ? 1. / y_scale_ضارب : 0.;
  مزدوج hx = 0, hy = 0;
  صحيح i;
  لكل (i = 0; i < (صحيح) *عدد_glyphs; i++)
  {
    (*glyphs)[i].اشاریہ_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((مزدوج) hb_position->x_وتھ_2 + hx) * x_scale;
    (*glyphs)[i].y = y + (-(مزدوج) hb_position->y_وتھ_2 + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].اشاریہ_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  إذا (clusters && *عدد_clusters && utf8)
  {
    hb_memset ((فراغ *) *clusters, 0, *عدد_clusters * حجم ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_بفر_get_direction (بفر));
    *cluster_flags = backward ? CAIRO_متن_CLUSTER_FLAG_BACKWARD : (cairo_متن_cluster_flags_t) 0;
    بنانشانی صحيح cluster = 0;
    ثابت حرف *شروع = utf8, *سرا;
    (*clusters)[cluster].عدد_glyphs++;
    إذا (backward)
    {
      لكل (i = *عدد_glyphs - 2; i >= 0; i--)
      {
	إذا (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  إذا (utf8_clusters)
	    سرا = شروع + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  وإلا
	    سرا = (ثابت حرف *) hb_utf_وتھ_منزل_اشارہ_گر<hb_utf8_t> ((ثابت uint8_t *) شروع,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (نشانیوالا) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].عدد_بائٹاں = سرا - شروع;
	  شروع = سرا;
	  cluster++;
	}
	(*clusters)[cluster].عدد_glyphs++;
      }
      (*clusters)[cluster].عدد_بائٹاں = utf8 + utf8_len - شروع;
    }
    وإلا
    {
      لكل (i = 1; i < (صحيح) *عدد_glyphs; i++)
      {
	إذا (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  إذا (utf8_clusters)
	    سرا = شروع + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  وإلا
	    سرا = (ثابت حرف *) hb_utf_وتھ_منزل_اشارہ_گر<hb_utf8_t> ((ثابت uint8_t *) شروع,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (نشانیوالا) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].عدد_بائٹاں = سرا - شروع;
	  شروع = سرا;
	  cluster++;
	}
	(*clusters)[cluster].عدد_glyphs++;
      }
      (*clusters)[cluster].عدد_بائٹاں = utf8 + utf8_len - شروع;
    }
  }
  وإلا إذا (عدد_clusters)
    *عدد_clusters = 0;
}

#جے_انت
