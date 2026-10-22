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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_CAIRO

#ଅନ୍ତର୍ଭାଵ "hb-cairo.h"

#ଅନ୍ତର୍ଭାଵ "hb-cairo-utils.hh"

#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"
#ଅନ୍ତର୍ଭାଵ "hb-utf.hh"


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

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  ଶୂନ୍ୟ *draw_data,
		  hb_draw_state_t *st HB_UNUSED,
		  ଦଶମିକ to_x, ଦଶମିକ to_y,
		  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  ଯଦି (unlikely (!c->spend (HB_BUDGET_1)))
    ଫେରନ୍ତୁ;

  cairo_move_to (c->cr, (ଦ୍ୱିଗୁଣ) to_x, (ଦ୍ୱିଗୁଣ) to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଧାଡ଼ି_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  ଶୂନ୍ୟ *draw_data,
		  hb_draw_state_t *st HB_UNUSED,
		  ଦଶମିକ to_x, ଦଶମିକ to_y,
		  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  ଯଦି (unlikely (!c->spend (HB_BUDGET_1)))
    ଫେରନ୍ତୁ;

  cairo_ଧାଡ଼ି_to (c->cr, (ଦ୍ୱିଗୁଣ) to_x, (ଦ୍ୱିଗୁଣ) to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		   ଶୂନ୍ୟ *draw_data,
		   hb_draw_state_t *st HB_UNUSED,
		   ଦଶମିକ control1_x, ଦଶମିକ control1_y,
		   ଦଶମିକ control2_x, ଦଶମିକ control2_y,
		   ଦଶମିକ to_x, ଦଶମିକ to_y,
		   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  ଯଦି (unlikely (!c->spend (HB_BUDGET_1)))
    ଫେରନ୍ତୁ;

  cairo_curve_to (c->cr,
                  (ଦ୍ୱିଗୁଣ) control1_x, (ଦ୍ୱିଗୁଣ) control1_y,
                  (ଦ୍ୱିଗୁଣ) control2_x, (ଦ୍ୱିଗୁଣ) control2_y,
                  (ଦ୍ୱିଗୁଣ) to_x, (ଦ୍ୱିଗୁଣ) to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     ଶୂନ୍ୟ *draw_data,
		     hb_draw_state_t *st HB_UNUSED,
		     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  ଯଦି (unlikely (!c->spend (HB_BUDGET_1)))
    ଫେରନ୍ତୁ;

  cairo_close_path (c->cr);
}

ସ୍ଥିର hb_bool_t
hb_cairo_draw_set_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  ଶୂନ୍ୟ *draw_data,
			  int64_t budget,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) draw_data)->set_budget (budget);
}

ସ୍ଥିର int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  ଶୂନ୍ୟ *draw_data,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) draw_data)->get_budget ();
}

ସ୍ଥିର int64_t *
hb_cairo_draw_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
				    ଶୂନ୍ୟ *draw_data,
				    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) draw_data)->get_budget_remaining ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_cairo_draw_funcs ();

ସ୍ଥିର ସଂରଚନା hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_cairo_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ଧାଡ଼ି_to_func (funcs, hb_cairo_ଧାଡ଼ି_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_cairo_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_cairo_close_path, nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_cairo_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_cairo_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_cairo_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_cairo_draw_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

ସ୍ଥିର hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  ଫେରନ୍ତୁ static_cairo_draw_funcs.get_unconst ();
}


#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଠେଲ_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			 ଶୂନ୍ୟ *paint_data,
			 ଦଶମିକ xx, ଦଶମିକ yx,
			 ଦଶମିକ xy, ଦଶମିକ yy,
			 ଦଶମିକ dx, ଦଶମିକ dy,
			 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_matrix_t m;

  cairo_save (cr);
  cairo_matrix_init (&m, (ଦ୍ୱିଗୁଣ) xx, (ଦ୍ୱିଗୁଣ) yx,
                         (ଦ୍ୱିଗୁଣ) xy, (ଦ୍ୱିଗୁଣ) yy,
                         (ଦ୍ୱିଗୁଣ) dx, (ଦ୍ୱିଗୁଣ) dy);
  cairo_transform (cr, &m);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_କାଢ଼_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
		        ଶୂନ୍ୟ *paint_data,
		        ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     ଶୂନ୍ୟ *paint_data,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_path (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_path (cr);
  _hb_cairo_set_source_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

ସ୍ଥିର hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    ଶୂନ୍ୟ *paint_data,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  ଯଦି (unlikely (!c->spend (HB_BUDGET_1)))
    ଫେରନ୍ତୁ ସତ୍ୟ;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_set_scaled_font (cr, c->scaled_font);
  cairo_set_font_size (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଠେଲ_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  ଶୂନ୍ୟ *paint_data,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_path (cr);
  cairo_clip (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଠେଲ_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
			      ଶୂନ୍ୟ *paint_data,
			      ଦଶମିକ xmin, ଦଶମିକ ymin, ଦଶମିକ xmax, ଦଶମିକ ymax,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (ଦ୍ୱିଗୁଣ) xmin, (ଦ୍ୱିଗୁଣ) ymin,
                   (ଦ୍ୱିଗୁଣ) (xmax - xmin), (ଦ୍ୱିଗୁଣ) (ymax - ymin));
  cairo_clip (cr);
}

ସ୍ଥିର hb_draw_funcs_t *
hb_cairo_ଠେଲ_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       ଶୂନ୍ୟ *paint_data,
			       ଶୂନ୍ୟ **draw_data,
			       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);
  *draw_data = c;
  ଫେରନ୍ତୁ hb_cairo_draw_get_funcs ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଠେଲ_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     ଶୂନ୍ୟ *paint_data,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_close_path (cr);
  cairo_clip (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_କାଢ଼_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
		   ଶୂନ୍ୟ *paint_data,
		   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_ଠେଲ_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     ଶୂନ୍ୟ *paint_data,
		     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_ଠେଲ_group (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_କାଢ଼_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		    ଶୂନ୍ୟ *paint_data,
		    hb_paint_composite_mode_t mode,
		    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_କାଢ଼_group_to_source (cr);
  cairo_set_operator (cr, _hb_paint_composite_mode_to_cairo (mode));
  cairo_paint (cr);

  cairo_restore (cr);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      ଶୂନ୍ୟ *paint_data,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  _hb_cairo_set_source_color (c, use_foreground, color);
  cairo_paint (cr);
}

ସ୍ଥିର hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      ଶୂନ୍ୟ *paint_data,
		      hb_blob_t *blob,
		      unsigned width,
		      unsigned height,
		      hb_tag_t format,
		      ଦଶମିକ slant,
		      hb_glyph_extents_t *extents,
		      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  ଫେରନ୍ତୁ _hb_cairo_paint_glyph_image (c, blob, width, height, format, slant, extents);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ଶୂନ୍ୟ *paint_data,
				hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				ଦଶମିକ x0, ଦଶମିକ y0,
				ଦଶମିକ x1, ଦଶମିକ y1,
				ଦଶମିକ x2, ଦଶମିକ y2,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_linear_gradient (c, color_ଧାଡ଼ି, x0, y0, x1, y1, x2, y2);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				ଶୂନ୍ୟ *paint_data,
				hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
				ଦଶମିକ x0, ଦଶମିକ y0, ଦଶମିକ r0,
				ଦଶମିକ x1, ଦଶମିକ y1, ଦଶମିକ r1,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_radial_gradient (c, color_ଧାଡ଼ି, x0, y0, r0, x1, y1, r1);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       ଶୂନ୍ୟ *paint_data,
			       hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି,
			       ଦଶମିକ x0, ଦଶମିକ y0,
			       ଦଶମିକ start_angle, ଦଶମିକ end_angle,
			       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_sweep_gradient (c, color_ଧାଡ଼ି, x0, y0, start_angle, end_angle);
}

ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t color_cache_ଚାବି = {0};

ସ୍ଥିର ଶୂନ୍ୟ
_hb_cairo_destroy_map (ଶୂନ୍ୟ *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

ସ୍ଥିର hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     ଶୂନ୍ୟ *paint_data,
                                     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା color_index,
                                     hb_color_t *color,
                                     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
#ifdef HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

#define HB_DEADBEEF HB_TAG(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  ଯଦି (likely (color_cache && color_cache->has (color_index, &v)))
  {
    ଯଦି (*v == HB_DEADBEEF)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *color = *v;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  cairo_font_options_t *options;
  ଦ୍ୱିଗୁଣ red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  ଯଦି (CAIRO_STATUS_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_index,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    ଯଦି (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->set (color_index, *color);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  cairo_font_options_destroy (options);

  ଯଦି (likely (color_cache))
    color_cache->set (color_index, HB_DEADBEEF);

#undef HB_DEADBEEF

#endif

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସ୍ଥିର hb_bool_t
hb_cairo_paint_set_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   ଶୂନ୍ୟ *paint_data,
			   int64_t budget,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) paint_data)->set_budget (budget);
}

ସ୍ଥିର int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   ଶୂନ୍ୟ *paint_data,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) paint_data)->get_budget ();
}

ସ୍ଥିର int64_t *
hb_cairo_paint_get_budget_remaining (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     ଶୂନ୍ୟ *paint_data,
				     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ ((hb_cairo_context_t *) paint_data)->get_budget_remaining ();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_cairo_paint_funcs ();

ସ୍ଥିର ସଂରଚନା hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ଠେଲ_transform_func (funcs, hb_cairo_ଠେଲ_transform, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_transform_func (funcs, hb_cairo_କାଢ଼_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_cairo_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_cairo_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_glyph_func (funcs, hb_cairo_ଠେଲ_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_rectangle_func (funcs, hb_cairo_ଠେଲ_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_start_func (funcs, hb_cairo_ଠେଲ_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_path_end_func (funcs, hb_cairo_ଠେଲ_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_clip_func (funcs, hb_cairo_କାଢ଼_clip, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_group_func (funcs, hb_cairo_ଠେଲ_group, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_group_func (funcs, hb_cairo_କାଢ଼_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_cairo_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_cairo_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_cairo_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_cairo_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_cairo_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, hb_cairo_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_cairo_paint_set_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_cairo_paint_get_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_cairo_paint_get_budget_remaining, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_paint_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_cairo_paint_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

ସ୍ଥିର hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  ଫେରନ୍ତୁ static_cairo_paint_funcs.get_unconst ();
}
#endif

ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_face_user_data_ଚାବି = {0};
ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_font_user_data_ଚାବି = {0};
ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_font_init_func_user_data_ଚାବି = {0};
ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_font_init_user_data_user_data_ଚାବି = {0};
ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_scale_factor_user_data_ଚାବି = {0};
ସ୍ଥିର ଅଚଳ cairo_user_data_ଚାବି_t hb_cairo_budget_user_data_ଚାବି = {0};

ସ୍ଥିର ଶୂନ୍ୟ hb_cairo_face_destroy (ଶୂନ୍ୟ *p) { hb_face_destroy ((hb_face_t *) p); }
ସ୍ଥିର ଶୂନ୍ୟ hb_cairo_font_destroy (ଶୂନ୍ୟ *p) { hb_font_destroy ((hb_font_t *) p); }
ସ୍ଥିର ଶୂନ୍ୟ
hb_cairo_budget_destroy (ଶୂନ୍ୟ *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

ସ୍ଥିର cairo_status_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_UNUSED,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_data (font_face,
								 &hb_cairo_font_user_data_ଚାବି);

  ଯଦି (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_data (font_face,
								   &hb_cairo_face_user_data_ଚାବି);
    font = hb_font_create (face);

#ଯଦି !defined(HB_NO_VAR) && CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    ଅଚଳ ଅକ୍ଷର *variations = cairo_font_options_get_variations (font_options);
    hb_vector_t<hb_variation_t> vars;
    ଅଚଳ ଅକ୍ଷର *p = variations;
    ଯେତେବେଳେ (p && *p)
    {
      ଅଚଳ ଅକ୍ଷର *end = strpbrk ((ଅକ୍ଷର *) p, ", ");
      hb_variation_t var;
      ଯଦି (hb_variation_from_string (p, end ? end - p : -1, &var))
	vars.push (var);
      p = end ? end + 1 : nullptr;
    }
    hb_font_set_variations (font, &vars[0], vars.length);

    cairo_font_options_destroy (font_options);
#endif

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    unsigned scale_factor = hb_cairo_font_face_get_scale_factor (font_face);
    ଯଦି (scale_factor)
    {
      cairo_matrix_t font_matrix;
      cairo_scaled_font_get_scale_matrix (scaled_font, &font_matrix);
      hb_font_set_scale (font,
			 hb_clamp_to<int32_t> (round (font_matrix.xx * scale_factor)),
			 hb_clamp_to<int32_t> (round (font_matrix.yy * scale_factor)));
    }

    auto *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_data (font_face,
						     &hb_cairo_font_init_func_user_data_ଚାବି);
    ଯଦି (init_func)
    {
      ଶୂନ୍ୟ *user_data = cairo_font_face_get_user_data (font_face,
						       &hb_cairo_font_init_user_data_user_data_ଚାବି);
      font = init_func (font, scaled_font, user_data);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_set_user_data (scaled_font,
				   &hb_cairo_font_user_data_ଚାବି,
				   (ଶୂନ୍ୟ *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (ଆକାର (hb_cairo_budget_t));
  ଯଦି (unlikely (!budget))
    ଫେରନ୍ତୁ CAIRO_STATUS_NO_MEMORY;
  ନୂତନ (budget) hb_cairo_budget_t ();
  cairo_status_t status = cairo_scaled_font_set_user_data (scaled_font,
						   &hb_cairo_budget_user_data_ଚାବି,
						   budget,
						   hb_cairo_budget_destroy);
  ଯଦି (unlikely (status != CAIRO_STATUS_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    ଫେରନ୍ତୁ status;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (ଦ୍ୱିଗୁଣ)  hb_extents.ascender  / y_scale;
  extents->descent = -(ଦ୍ୱିଗୁଣ) hb_extents.descender / y_scale;
  extents->height  = extents->ascent + extents->descent;

#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  ଯଦି (unlikely (CAIRO_STATUS_SUCCESS != cairo_scaled_font_set_user_data (scaled_font,
									 &color_cache_ଚାବି,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#endif

  ଫେରନ୍ତୁ CAIRO_STATUS_SUCCESS;
}

ସ୍ଥିର cairo_status_t
hb_cairo_text_to_glyphs (cairo_scaled_font_t        *scaled_font,
			 ଅଚଳ ଅକ୍ଷର	            *utf8,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା		            *ଅଙ୍କ_glyphs,
			 cairo_text_cluster_t      **clusters,
			 ପୂର୍ଣ୍ଣସଂଖ୍ୟା		            *ଅଙ୍କ_clusters,
			 cairo_text_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_ଚାବି);

  hb_buffer_t *buffer = hb_buffer_create ();
  hb_buffer_add_utf8 (buffer, utf8, utf8_len, 0, utf8_len);
  hb_buffer_guess_segment_properties (buffer);
  hb_shape (font, buffer, nullptr, 0);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_buffer (buffer,
			       ସତ୍ୟ,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (unsigned *) ଅଙ୍କ_glyphs,
			       clusters, (unsigned *) ଅଙ୍କ_clusters,
			       cluster_flags);

  hb_buffer_destroy (buffer);

  ଫେରନ୍ତୁ CAIRO_STATUS_SUCCESS;
}

ସ୍ଥିର cairo_status_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       unsigned long         glyph,
		       cairo_t              *cr,
		       cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_ଚାବି);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_data (scaled_font,
									     &hb_cairo_budget_user_data_ଚାବି);
  ଯଦି (unlikely (!budget))
    ଫେରନ୍ତୁ CAIRO_STATUS_NO_MEMORY;
  hb_cairo_context_t c {scaled_font, cr, nullptr, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  ଯଦି (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  ଫେରନ୍ତୁ CAIRO_STATUS_SUCCESS;
}

#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

ସ୍ଥିର cairo_status_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     unsigned long         glyph,
			     cairo_t              *cr,
			     cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_ଚାବି);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_data (scaled_font,
									     &hb_cairo_budget_user_data_ଚାବି);
  ଯଦି (unlikely (!budget))
    ଫେରନ୍ତୁ CAIRO_STATUS_NO_MEMORY;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା palette = 0;
#ifdef CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#endif

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_data (scaled_font, &color_cache_ଚାବି),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  ଫେରନ୍ତୁ CAIRO_STATUS_SUCCESS;
}

#endif

ସ୍ଥିର cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_set_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_set_text_to_glyphs_func (cairo_face, hb_cairo_text_to_glyphs);
  cairo_user_font_face_set_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_set_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#endif

  ଯଦି (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (cairo_face,
								       &hb_cairo_face_user_data_ଚାବି,
								       (ଶୂନ୍ୟ *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  ଫେରନ୍ତୁ cairo_face;
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

  auto *hb_face = hb_font_get_face (font);
  auto *cairo_face =  user_font_face_create (hb_face);

  ଯଦି (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (cairo_face,
								       &hb_cairo_font_user_data_ଚାବି,
								       (ଶୂନ୍ୟ *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  ଫେରନ୍ତୁ cairo_face;
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
  ଫେରନ୍ତୁ (hb_font_t *) cairo_font_face_get_user_data (font_face,
						      &hb_cairo_font_user_data_ଚାବି);
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

  ଫେରନ୍ତୁ user_font_face_create (face);
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
  ଫେରନ୍ତୁ (hb_face_t *) cairo_font_face_get_user_data (font_face,
						      &hb_cairo_face_user_data_ଚାବି);
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
ଶୂନ୍ୟ
hb_cairo_font_face_set_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       ଶୂନ୍ୟ *user_data,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_set_user_data (font_face,
				 &hb_cairo_font_init_func_user_data_ଚାବି,
				 (ଶୂନ୍ୟ *) func,
				 nullptr);
  ଯଦି (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (font_face,
								       &hb_cairo_font_init_user_data_user_data_ଚାବି,
								       (ଶୂନ୍ୟ *) user_data,
								       destroy)) && destroy)
  {
    destroy (user_data);
    cairo_font_face_set_user_data (font_face,
				   &hb_cairo_font_init_func_user_data_ଚାବି,
				   nullptr,
				   nullptr);
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
  ଫେରନ୍ତୁ (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font, &hb_cairo_font_user_data_ଚାବି);
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
ଶୂନ୍ୟ
hb_cairo_font_face_set_scale_factor (cairo_font_face_t *font_face,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale_factor)
{
  cairo_font_face_set_user_data (font_face,
				 &hb_cairo_scale_factor_user_data_ଚାବି,
				 (ଶୂନ୍ୟ *) (uintptr_t) scale_factor,
				 nullptr);
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
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hb_cairo_font_face_get_scale_factor (cairo_font_face_t *font_face)
{
  ଫେରନ୍ତୁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (uintptr_t)
	 cairo_font_face_get_user_data (font_face,
					&hb_cairo_scale_factor_user_data_ଚାବି);
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
ଶୂନ୍ୟ
hb_cairo_glyphs_from_buffer (hb_buffer_t *buffer,
			     hb_bool_t utf8_clusters,
			     ଦ୍ୱିଗୁଣ x_scale_factor,
			     ଦ୍ୱିଗୁଣ y_scale_factor,
			     ଦ୍ୱିଗୁଣ x,
			     ଦ୍ୱିଗୁଣ y,
			     ଅଚଳ ଅକ୍ଷର *utf8,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8_len,
			     cairo_glyph_t **glyphs,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ_glyphs,
			     cairo_text_cluster_t **clusters,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ_clusters,
			     cairo_text_cluster_flags_t *cluster_flags)
{
  ଯଦି (utf8 && utf8_len < 0)
    utf8_len = strlen (utf8);

  unsigned orig_ଅଙ୍କ_glyphs = *ଅଙ୍କ_glyphs;
  *ଅଙ୍କ_glyphs = hb_buffer_get_length (buffer);
  hb_glyph_info_t *hb_glyph = hb_buffer_get_glyph_infos (buffer, nullptr);
  hb_glyph_position_t *hb_position = hb_buffer_get_glyph_positions (buffer, nullptr);
  ଯଦି (orig_ଅଙ୍କ_glyphs < *ଅଙ୍କ_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*ଅଙ୍କ_glyphs + 1);

  ଯଦି (clusters && utf8)
  {
    unsigned orig_ଅଙ୍କ_clusters = *ଅଙ୍କ_clusters;
    *ଅଙ୍କ_clusters = *ଅଙ୍କ_glyphs ? 1 : 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < *ଅଙ୍କ_glyphs; i++)
      ଯଦି (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*ଅଙ୍କ_clusters)++;
    ଯଦି (orig_ଅଙ୍କ_clusters < *ଅଙ୍କ_clusters)
      *clusters = cairo_text_cluster_allocate (*ଅଙ୍କ_clusters);
  }

  ଦ୍ୱିଗୁଣ x_scale = x_scale_factor ? 1. / x_scale_factor : 0.;
  ଦ୍ୱିଗୁଣ y_scale = y_scale_factor ? 1. / y_scale_factor : 0.;
  ଦ୍ୱିଗୁଣ hx = 0, hy = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ପାଇଁ (i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *ଅଙ୍କ_glyphs; i++)
  {
    (*glyphs)[i].index = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((ଦ୍ୱିଗୁଣ) hb_position->x_offset + hx) * x_scale;
    (*glyphs)[i].y = y + (-(ଦ୍ୱିଗୁଣ) hb_position->y_offset + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].index = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  ଯଦି (clusters && *ଅଙ୍କ_clusters && utf8)
  {
    hb_memset ((ଶୂନ୍ୟ *) *clusters, 0, *ଅଙ୍କ_clusters * ଆକାର ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_buffer_get_direction (buffer));
    *cluster_flags = backward ? CAIRO_TEXT_CLUSTER_FLAG_BACKWARD : (cairo_text_cluster_flags_t) 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster = 0;
    ଅଚଳ ଅକ୍ଷର *start = utf8, *end;
    (*clusters)[cluster].ଅଙ୍କ_glyphs++;
    ଯଦି (backward)
    {
      ପାଇଁ (i = *ଅଙ୍କ_glyphs - 2; i >= 0; i--)
      {
	ଯଦି (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  ପ୍ରତିଜ୍ଞା (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  ଯଦି (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  ଅନ୍ୟଥା
	    end = (ଅଚଳ ଅକ୍ଷର *) hb_utf_offset_to_pointer<hb_utf8_t> ((ଅଚଳ uint8_t *) start,
								      (ଅଚଳ uint8_t *) utf8, utf8_len,
								      (signed) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].ଅଙ୍କ_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].ଅଙ୍କ_glyphs++;
      }
      (*clusters)[cluster].ଅଙ୍କ_bytes = utf8 + utf8_len - start;
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (i = 1; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) *ଅଙ୍କ_glyphs; i++)
      {
	ଯଦି (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  ପ୍ରତିଜ୍ଞା (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  ଯଦି (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  ଅନ୍ୟଥା
	    end = (ଅଚଳ ଅକ୍ଷର *) hb_utf_offset_to_pointer<hb_utf8_t> ((ଅଚଳ uint8_t *) start,
								      (ଅଚଳ uint8_t *) utf8, utf8_len,
								      (signed) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].ଅଙ୍କ_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].ଅଙ୍କ_glyphs++;
      }
      (*clusters)[cluster].ଅଙ୍କ_bytes = utf8 + utf8_len - start;
    }
  }
  ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_clusters)
    *ଅଙ୍କ_clusters = 0;
}

#endif
