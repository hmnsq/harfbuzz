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

#جيڪڏهن_وصف_ٿيل HAVE_CAIRO

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
hb_cairo_move_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
		  فراغ *draw_معلومات,
		  hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
		  عائم منزل_x, عائم منزل_y,
		  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_معلومات;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_move_منزل (c->cr, (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_سٽ_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
		  فراغ *draw_معلومات,
		  hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
		  عائم منزل_x, عائم منزل_y,
		  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_معلومات;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_سٽ_منزل (c->cr, (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_cubic_منزل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
		   فراغ *draw_معلومات,
		   hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
		   عائم control1_x, عائم control1_y,
		   عائم control2_x, عائم control2_y,
		   عائم منزل_x, عائم منزل_y,
		   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_معلومات;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_وڪر_منزل (c->cr,
                  (مزدوج) control1_x, (مزدوج) control1_y,
                  (مزدوج) control2_x, (مزدوج) control2_y,
                  (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_cairo_close_رستو (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
		     فراغ *draw_معلومات,
		     hb_draw_state_t *st HB_اڻ_استعمال_ٿيل,
		     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_معلومات;
  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع;

  cairo_close_رستو (c->cr);
}

ساكن hb_bool_t
hb_cairo_draw_سيٽ_budget (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
			  فراغ *draw_معلومات,
			  int64_t budget,
			  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) draw_معلومات)->سيٽ_budget (budget);
}

ساكن int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
			  فراغ *draw_معلومات,
			  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) draw_معلومات)->get_budget ();
}

ساكن int64_t *
hb_cairo_draw_get_budget_بچيل (hb_draw_funcs_t *dfuncs HB_اڻ_استعمال_ٿيل,
				    فراغ *draw_معلومات,
				    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) draw_معلومات)->get_budget_بچيل ();
}

ساكن مضمن فراغ free_static_cairo_draw_funcs ();

ساكن بنية hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سيٽ_move_منزل_func (funcs, hb_cairo_move_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سٽ_منزل_func (funcs, hb_cairo_سٽ_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_cubic_منزل_func (funcs, hb_cairo_cubic_منزل, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_close_رستو_func (funcs, hb_cairo_close_رستو, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_سيٽ_budget_func (funcs, hb_cairo_draw_سيٽ_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_func (funcs, hb_cairo_draw_get_budget, خالي_اشارو, خالي_اشارو);
    hb_draw_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_cairo_draw_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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


#جيڪڏهن_وصف_ٿيل HAVE_CAIRO_USER_FONT_FACE_سيٽ_RENDER_COLOR_GLYPH_FUNC

ساكن فراغ
hb_cairo_ڌڪو_transform (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			 فراغ *paint_معلومات,
			 عائم xx, عائم yx,
			 عائم xy, عائم yy,
			 عائم dx, عائم dy,
			 فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_ميٽرڪس_t m;

  cairo_save (cr);
  cairo_ميٽرڪس_init (&m, (مزدوج) xx, (مزدوج) yx,
                         (مزدوج) xy, (مزدوج) yy,
                         (مزدوج) dx, (مزدوج) dy);
  cairo_transform (cr, &m);
}

ساكن فراغ
hb_cairo_ڪڍو_transform (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		        فراغ *paint_معلومات,
		        فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		     فراغ *paint_معلومات,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_رستو (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_رستو (cr);
  hb_cairo_سيٽ_ذريعو_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

ساكن hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			    فراغ *paint_معلومات,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  إذا (unlikely (!c->spend (HB_BUDGET_1)))
    أرجع سچ;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_سيٽ_scaled_font (cr, c->scaled_font);
  cairo_سيٽ_font_سائيز (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  أرجع سچ;
}

ساكن فراغ
hb_cairo_ڌڪو_clip_glyph (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			  فراغ *paint_معلومات,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_رستو (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_رستو (cr);
  cairo_clip (cr);
}

ساكن فراغ
hb_cairo_ڌڪو_clip_rectangle (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			      فراغ *paint_معلومات,
			      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
			      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (مزدوج) xmin, (مزدوج) ymin,
                   (مزدوج) (xmax - xmin), (مزدوج) (ymax - ymin));
  cairo_clip (cr);
}

ساكن hb_draw_funcs_t *
hb_cairo_ڌڪو_clip_رستو_شروعات (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			       فراغ *paint_معلومات,
			       فراغ **draw_معلومات,
			       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_رستو (cr);
  *draw_معلومات = c;
  أرجع hb_cairo_draw_get_funcs ();
}

ساكن فراغ
hb_cairo_ڌڪو_clip_رستو_پڇاڙي (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			     فراغ *paint_معلومات,
			     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_close_رستو (cr);
  cairo_clip (cr);
}

ساكن فراغ
hb_cairo_ڪڍو_clip (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		   فراغ *paint_معلومات,
		   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_ڌڪو_گروهه (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		     فراغ *paint_معلومات,
		     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_ڌڪو_گروهه (cr);
}

ساكن فراغ
hb_cairo_ڪڍو_گروهه (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		    فراغ *paint_معلومات,
		    hb_paint_مرکب_گهڻائي_t گهڻائي,
		    فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  cairo_ڪڍو_گروهه_منزل_ذريعو (cr);
  cairo_سيٽ_operator (cr, hb_paint_مرکب_گهڻائي_منزل_cairo (گهڻائي));
  cairo_paint (cr);

  cairo_restore (cr);
}

ساكن فراغ
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		      فراغ *paint_معلومات,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

  hb_cairo_سيٽ_ذريعو_color (c, use_foreground, color);
  cairo_paint (cr);
}

ساكن hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
		      فراغ *paint_معلومات,
		      hb_blob_t *blob,
		      بنانشان ويڪر,
		      بنانشان اوچائي,
		      hb_ٽيگ_t format,
		      عائم slant,
		      hb_glyph_extents_t *extents,
		      فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;

  أرجع _hb_cairo_paint_glyph_image (c, blob, ويڪر, اوچائي, format, slant, extents);
}

ساكن فراغ
hb_cairo_paint_لڪيري_gradient_2 (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
				فراغ *paint_معلومات,
				hb_color_سٽ_t *color_سٽ,
				عائم x0, عائم y0,
				عائم x1, عائم y1,
				عائم x2, عائم y2,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;

  hb_cairo_paint_لڪيري_gradient (c, color_سٽ, x0, y0, x1, y1, x2, y2);
}

ساكن فراغ
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
				فراغ *paint_معلومات,
				hb_color_سٽ_t *color_سٽ,
				عائم x0, عائم y0, عائم r0,
				عائم x1, عائم y1, عائم r1,
				فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;

  _hb_cairo_paint_radial_gradient (c, color_سٽ, x0, y0, r0, x1, y1, r1);
}

ساكن فراغ
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			       فراغ *paint_معلومات,
			       hb_color_سٽ_t *color_سٽ,
			       عائم x0, عائم y0,
			       عائم شروعات_angle, عائم پڇاڙي_angle,
			       فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;

  _hb_cairo_paint_sweep_gradient (c, color_سٽ, x0, y0, شروعات_angle, پڇاڙي_angle);
}

ساكن ثابت cairo_user_معلومات_ڪنجي_t color_cache_ڪنجي = {0};

ساكن فراغ
_hb_cairo_destroy_map (فراغ *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

ساكن hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     فراغ *paint_معلومات,
                                     بنانشان صحيح color_اشاريو,
                                     hb_color_t *color,
                                     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
#جيڪڏهن_وصف_ٿيل HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_معلومات;
  cairo_t *cr = c->cr;

#وصف_ڪريو HB_DEADBEEF HB_ٽيگ(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  إذا (likely (color_cache && color_cache->has (color_اشاريو, &v)))
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
      cairo_font_options_get_custom_palette_color (options, color_اشاريو,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    إذا (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->سيٽ (color_اشاريو, *color);

    أرجع سچ;
  }
  cairo_font_options_destroy (options);

  إذا (likely (color_cache))
    color_cache->سيٽ (color_اشاريو, HB_DEADBEEF);

#وصف_رد HB_DEADBEEF

#جيڪڏهن_پورو

  أرجع خطأ;
}

ساكن hb_bool_t
hb_cairo_paint_سيٽ_budget (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			   فراغ *paint_معلومات,
			   int64_t budget,
			   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) paint_معلومات)->سيٽ_budget (budget);
}

ساكن int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
			   فراغ *paint_معلومات,
			   فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) paint_معلومات)->get_budget ();
}

ساكن int64_t *
hb_cairo_paint_get_budget_بچيل (hb_paint_funcs_t *pfuncs HB_اڻ_استعمال_ٿيل,
				     فراغ *paint_معلومات,
				     فراغ *user_معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع ((hb_cairo_context_t *) paint_معلومات)->get_budget_بچيل ();
}

ساكن مضمن فراغ free_static_cairo_paint_funcs ();

ساكن بنية hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سيٽ_ڌڪو_transform_func (funcs, hb_cairo_ڌڪو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_transform_func (funcs, hb_cairo_ڪڍو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_fill_glyph_func (funcs, hb_cairo_fill_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_glyph_func (funcs, hb_cairo_paint_color_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_glyph_func (funcs, hb_cairo_ڌڪو_clip_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_rectangle_func (funcs, hb_cairo_ڌڪو_clip_rectangle, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_شروعات_func (funcs, hb_cairo_ڌڪو_clip_رستو_شروعات, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_پڇاڙي_func (funcs, hb_cairo_ڌڪو_clip_رستو_پڇاڙي, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_clip_func (funcs, hb_cairo_ڪڍو_clip, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_func (funcs, hb_cairo_ڌڪو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_گروهه_func (funcs, hb_cairo_ڪڍو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_func (funcs, hb_cairo_paint_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_image_func (funcs, hb_cairo_paint_image, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_لڪيري_gradient_func (funcs, hb_cairo_paint_لڪيري_gradient_2, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_سيٽ_budget_func (funcs, hb_cairo_paint_سيٽ_budget, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_func (funcs, hb_cairo_paint_get_budget, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_cairo_paint_get_budget_بچيل, خالي_اشارو, خالي_اشارو);

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
#جيڪڏهن_پورو

ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_face_user_معلومات_ڪنجي = {0};
ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_font_user_معلومات_ڪنجي = {0};
ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_font_init_func_user_معلومات_ڪنجي = {0};
ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_font_init_user_معلومات_user_معلومات_ڪنجي = {0};
ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_scale_ضرب_ڪندڙ_user_معلومات_ڪنجي = {0};
ساكن ثابت cairo_user_معلومات_ڪنجي_t hb_cairo_budget_user_معلومات_ڪنجي = {0};

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
			   cairo_t              *cr HB_اڻ_استعمال_ٿيل,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_معلومات (font_face,
								 &hb_cairo_font_user_معلومات_ڪنجي);

  إذا (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_معلومات (font_face,
								   &hb_cairo_face_user_معلومات_ڪنجي);
    font = hb_font_create (face);

#إذا !defined(HB_NO_VAR) && CAIRO_نسخو >= CAIRO_نسخو_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    ثابت حرف *variations = cairo_font_options_get_variations (font_options);
    hb_ويڪٽر_t<hb_variation_t> vars;
    ثابت حرف *p = variations;
    طالما (p && *p)
    {
      ثابت حرف *پڇاڙي = strpbrk ((حرف *) p, ", ");
      hb_variation_t var;
      إذا (hb_variation_from_string (p, پڇاڙي ? پڇاڙي - p : -1, &var))
	vars.push (var);
      p = پڇاڙي ? پڇاڙي + 1 : خالي_اشارو;
    }
    hb_font_سيٽ_variations (font, &vars[0], vars.ڊيگهائي);

    cairo_font_options_destroy (font_options);
#جيڪڏهن_پورو

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    بنانشان scale_ضرب_ڪندڙ = hb_cairo_font_face_get_scale_ضرب_ڪندڙ (font_face);
    إذا (scale_ضرب_ڪندڙ)
    {
      cairo_ميٽرڪس_t font_ميٽرڪس;
      cairo_scaled_font_get_scale_ميٽرڪس (scaled_font, &font_ميٽرڪس);
      hb_font_سيٽ_scale (font,
			 hb_clamp_منزل<int32_t> (round (font_ميٽرڪس.xx * scale_ضرب_ڪندڙ)),
			 hb_clamp_منزل<int32_t> (round (font_ميٽرڪس.yy * scale_ضرب_ڪندڙ)));
    }

    خودڪار *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_معلومات (font_face,
						     &hb_cairo_font_init_func_user_معلومات_ڪنجي);
    إذا (init_func)
    {
      فراغ *user_معلومات = cairo_font_face_get_user_معلومات (font_face,
						       &hb_cairo_font_init_user_معلومات_user_معلومات_ڪنجي);
      font = init_func (font, scaled_font, user_معلومات);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_سيٽ_user_معلومات (scaled_font,
				   &hb_cairo_font_user_معلومات_ڪنجي,
				   (فراغ *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (حجم (hb_cairo_budget_t));
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_يادگيري;
  جديد (budget) hb_cairo_budget_t ();
  cairo_حالت_t حالت = cairo_scaled_font_سيٽ_user_معلومات (scaled_font,
						   &hb_cairo_budget_user_معلومات_ڪنجي,
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
  extents->اوچائي  = extents->ascent + extents->descent;

#جيڪڏهن_وصف_ٿيل HAVE_CAIRO_USER_FONT_FACE_سيٽ_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_scaled_font_سيٽ_user_معلومات (scaled_font,
									 &color_cache_ڪنجي,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#جيڪڏهن_پورو

  أرجع CAIRO_حالت_SUCCESS;
}

ساكن cairo_حالت_t
hb_cairo_متن_منزل_glyphs (cairo_scaled_font_t        *scaled_font,
			 ثابت حرف	            *utf8,
			 صحيح		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 صحيح		            *انگ_glyphs,
			 cairo_متن_cluster_t      **clusters,
			 صحيح		            *انگ_clusters,
			 cairo_متن_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_معلومات (scaled_font,
								   &hb_cairo_font_user_معلومات_ڪنجي);

  hb_بفر_t *بفر = hb_بفر_create ();
  hb_بفر_جوڙيو_utf8 (بفر, utf8, utf8_len, 0, utf8_len);
  hb_بفر_guess_segment_properties (بفر);
  hb_shape (font, بفر, خالي_اشارو, 0);

  صحيح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_بفر (بفر,
			       سچ,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (بنانشان *) انگ_glyphs,
			       clusters, (بنانشان *) انگ_clusters,
			       cluster_flags);

  hb_بفر_destroy (بفر);

  أرجع CAIRO_حالت_SUCCESS;
}

ساكن cairo_حالت_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       بنانشان ڊگهو         glyph,
		       cairo_t              *cr,
		       cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_معلومات (scaled_font,
								   &hb_cairo_font_user_معلومات_ڪنجي);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_معلومات (scaled_font,
									     &hb_cairo_budget_user_معلومات_ڪنجي);
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_يادگيري;
  hb_cairo_context_t c {scaled_font, cr, خالي_اشارو, budget};

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

#جيڪڏهن_وصف_ٿيل HAVE_CAIRO_USER_FONT_FACE_سيٽ_RENDER_COLOR_GLYPH_FUNC

ساكن cairo_حالت_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     بنانشان ڊگهو         glyph,
			     cairo_t              *cr,
			     cairo_متن_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_معلومات (scaled_font,
								   &hb_cairo_font_user_معلومات_ڪنجي);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_معلومات (scaled_font,
									     &hb_cairo_budget_user_معلومات_ڪنجي);
  إذا (unlikely (!budget))
    أرجع CAIRO_حالت_NO_يادگيري;

  بنانشان صحيح palette = 0;
#جيڪڏهن_وصف_ٿيل CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#جيڪڏهن_پورو

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_معلومات (scaled_font, &color_cache_ڪنجي),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  أرجع CAIRO_حالت_SUCCESS;
}

#جيڪڏهن_پورو

ساكن cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_سيٽ_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_سيٽ_متن_منزل_glyphs_func (cairo_face, hb_cairo_متن_منزل_glyphs);
  cairo_user_font_face_سيٽ_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#جيڪڏهن_وصف_ٿيل HAVE_CAIRO_USER_FONT_FACE_سيٽ_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_سيٽ_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#جيڪڏهن_پورو

  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سيٽ_user_معلومات (cairo_face,
								       &hb_cairo_face_user_معلومات_ڪنجي,
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

  خودڪار *hb_face = hb_font_get_face (font);
  خودڪار *cairo_face =  user_font_face_create (hb_face);

  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سيٽ_user_معلومات (cairo_face,
								       &hb_cairo_font_user_معلومات_ڪنجي,
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
  أرجع (hb_font_t *) cairo_font_face_get_user_معلومات (font_face,
						      &hb_cairo_font_user_معلومات_ڪنجي);
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
  أرجع (hb_face_t *) cairo_font_face_get_user_معلومات (font_face,
						      &hb_cairo_face_user_معلومات_ڪنجي);
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
hb_cairo_font_face_سيٽ_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       فراغ *user_معلومات,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_سيٽ_user_معلومات (font_face,
				 &hb_cairo_font_init_func_user_معلومات_ڪنجي,
				 (فراغ *) func,
				 خالي_اشارو);
  إذا (unlikely (CAIRO_حالت_SUCCESS != cairo_font_face_سيٽ_user_معلومات (font_face,
								       &hb_cairo_font_init_user_معلومات_user_معلومات_ڪنجي,
								       (فراغ *) user_معلومات,
								       destroy)) && destroy)
  {
    destroy (user_معلومات);
    cairo_font_face_سيٽ_user_معلومات (font_face,
				   &hb_cairo_font_init_func_user_معلومات_ڪنجي,
				   خالي_اشارو,
				   خالي_اشارو);
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
  أرجع (hb_font_t *) cairo_scaled_font_get_user_معلومات (scaled_font, &hb_cairo_font_user_معلومات_ڪنجي);
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
hb_cairo_font_face_سيٽ_scale_ضرب_ڪندڙ (cairo_font_face_t *font_face,
				     بنانشان صحيح scale_ضرب_ڪندڙ)
{
  cairo_font_face_سيٽ_user_معلومات (font_face,
				 &hb_cairo_scale_ضرب_ڪندڙ_user_معلومات_ڪنجي,
				 (فراغ *) (uintptr_t) scale_ضرب_ڪندڙ,
				 خالي_اشارو);
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
بنانشان صحيح
hb_cairo_font_face_get_scale_ضرب_ڪندڙ (cairo_font_face_t *font_face)
{
  أرجع (بنانشان صحيح) (uintptr_t)
	 cairo_font_face_get_user_معلومات (font_face,
					&hb_cairo_scale_ضرب_ڪندڙ_user_معلومات_ڪنجي);
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
			     مزدوج x_scale_ضرب_ڪندڙ,
			     مزدوج y_scale_ضرب_ڪندڙ,
			     مزدوج x,
			     مزدوج y,
			     ثابت حرف *utf8,
			     صحيح utf8_len,
			     cairo_glyph_t **glyphs,
			     بنانشان صحيح *انگ_glyphs,
			     cairo_متن_cluster_t **clusters,
			     بنانشان صحيح *انگ_clusters,
			     cairo_متن_cluster_flags_t *cluster_flags)
{
  إذا (utf8 && utf8_len < 0)
    utf8_len = لڙي_ڊيگهه (utf8);

  بنانشان orig_انگ_glyphs = *انگ_glyphs;
  *انگ_glyphs = hb_بفر_get_ڊيگهائي (بفر);
  hb_glyph_info_t *hb_glyph = hb_بفر_get_glyph_infos (بفر, خالي_اشارو);
  hb_glyph_position_t *hb_position = hb_بفر_get_glyph_positions (بفر, خالي_اشارو);
  إذا (orig_انگ_glyphs < *انگ_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*انگ_glyphs + 1);

  إذا (clusters && utf8)
  {
    بنانشان orig_انگ_clusters = *انگ_clusters;
    *انگ_clusters = *انگ_glyphs ? 1 : 0;
    لكل (بنانشان صحيح i = 1; i < *انگ_glyphs; i++)
      إذا (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*انگ_clusters)++;
    إذا (orig_انگ_clusters < *انگ_clusters)
      *clusters = cairo_متن_cluster_allocate (*انگ_clusters);
  }

  مزدوج x_scale = x_scale_ضرب_ڪندڙ ? 1. / x_scale_ضرب_ڪندڙ : 0.;
  مزدوج y_scale = y_scale_ضرب_ڪندڙ ? 1. / y_scale_ضرب_ڪندڙ : 0.;
  مزدوج hx = 0, hy = 0;
  صحيح i;
  لكل (i = 0; i < (صحيح) *انگ_glyphs; i++)
  {
    (*glyphs)[i].اشاريو_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((مزدوج) hb_position->x_وٿي_2 + hx) * x_scale;
    (*glyphs)[i].y = y + (-(مزدوج) hb_position->y_وٿي_2 + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].اشاريو_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  إذا (clusters && *انگ_clusters && utf8)
  {
    hb_memset ((فراغ *) *clusters, 0, *انگ_clusters * حجم ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_بفر_get_direction (بفر));
    *cluster_flags = backward ? CAIRO_متن_CLUSTER_FLAG_BACKWARD : (cairo_متن_cluster_flags_t) 0;
    بنانشان صحيح cluster = 0;
    ثابت حرف *شروعات = utf8, *پڇاڙي;
    (*clusters)[cluster].انگ_glyphs++;
    إذا (backward)
    {
      لكل (i = *انگ_glyphs - 2; i >= 0; i--)
      {
	إذا (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  إذا (utf8_clusters)
	    پڇاڙي = شروعات + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  وإلا
	    پڇاڙي = (ثابت حرف *) hb_utf_وٿي_منزل_اشارو<hb_utf8_t> ((ثابت uint8_t *) شروعات,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (نشانوارو) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].انگ_بائيٽون = پڇاڙي - شروعات;
	  شروعات = پڇاڙي;
	  cluster++;
	}
	(*clusters)[cluster].انگ_glyphs++;
      }
      (*clusters)[cluster].انگ_بائيٽون = utf8 + utf8_len - شروعات;
    }
    وإلا
    {
      لكل (i = 1; i < (صحيح) *انگ_glyphs; i++)
      {
	إذا (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  أكد (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  إذا (utf8_clusters)
	    پڇاڙي = شروعات + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  وإلا
	    پڇاڙي = (ثابت حرف *) hb_utf_وٿي_منزل_اشارو<hb_utf8_t> ((ثابت uint8_t *) شروعات,
								      (ثابت uint8_t *) utf8, utf8_len,
								      (نشانوارو) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].انگ_بائيٽون = پڇاڙي - شروعات;
	  شروعات = پڇاڙي;
	  cluster++;
	}
	(*clusters)[cluster].انگ_glyphs++;
      }
      (*clusters)[cluster].انگ_بائيٽون = utf8 + utf8_len - شروعات;
    }
  }
  وإلا إذا (انگ_clusters)
    *انگ_clusters = 0;
}

#جيڪڏهن_پورو
