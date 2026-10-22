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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_CAIRO

#அந்தர்பாவ "hb-cairo.h"

#அந்தர்பாவ "hb-cairo-utils.hh"

#அந்தர்பாவ "hb-machinery.hh"
#அந்தர்பாவ "hb-utf.hh"


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

நிலையான வெறுமை
hb_cairo_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  வெறுமை *draw_data,
		  hb_draw_state_t *st HB_UNUSED,
		  தசம to_x, தசம to_y,
		  வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  எனில் (unlikely (!c->spend (HB_BUDGET_1)))
    திரும்பு;

  cairo_move_to (c->cr, (இரட்டை) to_x, (இரட்டை) to_y);
}

நிலையான வெறுமை
hb_cairo_வரி_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  வெறுமை *draw_data,
		  hb_draw_state_t *st HB_UNUSED,
		  தசம to_x, தசம to_y,
		  வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  எனில் (unlikely (!c->spend (HB_BUDGET_1)))
    திரும்பு;

  cairo_வரி_to (c->cr, (இரட்டை) to_x, (இரட்டை) to_y);
}

நிலையான வெறுமை
hb_cairo_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		   வெறுமை *draw_data,
		   hb_draw_state_t *st HB_UNUSED,
		   தசம control1_x, தசம control1_y,
		   தசம control2_x, தசம control2_y,
		   தசம to_x, தசம to_y,
		   வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  எனில் (unlikely (!c->spend (HB_BUDGET_1)))
    திரும்பு;

  cairo_curve_to (c->cr,
                  (இரட்டை) control1_x, (இரட்டை) control1_y,
                  (இரட்டை) control2_x, (இரட்டை) control2_y,
                  (இரட்டை) to_x, (இரட்டை) to_y);
}

நிலையான வெறுமை
hb_cairo_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     வெறுமை *draw_data,
		     hb_draw_state_t *st HB_UNUSED,
		     வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_data;
  எனில் (unlikely (!c->spend (HB_BUDGET_1)))
    திரும்பு;

  cairo_close_path (c->cr);
}

நிலையான hb_bool_t
hb_cairo_draw_set_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  வெறுமை *draw_data,
			  int64_t budget,
			  வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) draw_data)->set_budget (budget);
}

நிலையான int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  வெறுமை *draw_data,
			  வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) draw_data)->get_budget ();
}

நிலையான int64_t *
hb_cairo_draw_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
				    வெறுமை *draw_data,
				    வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) draw_data)->get_budget_remaining ();
}

நிலையான உள்வரி வெறுமை free_static_cairo_draw_funcs ();

நிலையான கட்டமைப்பு hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  நிலையான hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_cairo_move_to, nullptr, nullptr);
    hb_draw_funcs_set_வரி_to_func (funcs, hb_cairo_வரி_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_cairo_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_cairo_close_path, nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_cairo_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_cairo_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_cairo_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    திரும்பு funcs;
  }
} static_cairo_draw_funcs;

நிலையான உள்வரி
வெறுமை free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

நிலையான hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  திரும்பு static_cairo_draw_funcs.get_unconst ();
}


#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

நிலையான வெறுமை
hb_cairo_தள்ளு_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			 வெறுமை *paint_data,
			 தசம xx, தசம yx,
			 தசம xy, தசம yy,
			 தசம dx, தசம dy,
			 வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_matrix_t m;

  cairo_save (cr);
  cairo_matrix_init (&m, (இரட்டை) xx, (இரட்டை) yx,
                         (இரட்டை) xy, (இரட்டை) yy,
                         (இரட்டை) dx, (இரட்டை) dy);
  cairo_transform (cr, &m);
}

நிலையான வெறுமை
hb_cairo_எடு_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
		        வெறுமை *paint_data,
		        வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

நிலையான வெறுமை
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     வெறுமை *paint_data,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     வெறுமை *user_data HB_UNUSED)
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

நிலையான hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    வெறுமை *paint_data,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  எனில் (unlikely (!c->spend (HB_BUDGET_1)))
    திரும்பு உண்மை;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_set_scaled_font (cr, c->scaled_font);
  cairo_set_font_size (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_cairo_தள்ளு_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  வெறுமை *paint_data,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_path (cr);
  cairo_clip (cr);
}

நிலையான வெறுமை
hb_cairo_தள்ளு_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
			      வெறுமை *paint_data,
			      தசம xmin, தசம ymin, தசம xmax, தசம ymax,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (இரட்டை) xmin, (இரட்டை) ymin,
                   (இரட்டை) (xmax - xmin), (இரட்டை) (ymax - ymin));
  cairo_clip (cr);
}

நிலையான hb_draw_funcs_t *
hb_cairo_தள்ளு_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       வெறுமை *paint_data,
			       வெறுமை **draw_data,
			       வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);
  *draw_data = c;
  திரும்பு hb_cairo_draw_get_funcs ();
}

நிலையான வெறுமை
hb_cairo_தள்ளு_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     வெறுமை *paint_data,
			     வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_close_path (cr);
  cairo_clip (cr);
}

நிலையான வெறுமை
hb_cairo_எடு_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
		   வெறுமை *paint_data,
		   வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

நிலையான வெறுமை
hb_cairo_தள்ளு_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     வெறுமை *paint_data,
		     வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_தள்ளு_group (cr);
}

நிலையான வெறுமை
hb_cairo_எடு_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		    வெறுமை *paint_data,
		    hb_paint_composite_mode_t mode,
		    வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  cairo_எடு_group_to_source (cr);
  cairo_set_operator (cr, _hb_paint_composite_mode_to_cairo (mode));
  cairo_paint (cr);

  cairo_restore (cr);
}

நிலையான வெறுமை
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      வெறுமை *paint_data,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

  _hb_cairo_set_source_color (c, use_foreground, color);
  cairo_paint (cr);
}

நிலையான hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      வெறுமை *paint_data,
		      hb_blob_t *blob,
		      unsigned width,
		      unsigned height,
		      hb_tag_t format,
		      தசம slant,
		      hb_glyph_extents_t *extents,
		      வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  திரும்பு _hb_cairo_paint_glyph_image (c, blob, width, height, format, slant, extents);
}

நிலையான வெறுமை
hb_cairo_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				வெறுமை *paint_data,
				hb_color_வரி_t *color_வரி,
				தசம x0, தசம y0,
				தசம x1, தசம y1,
				தசம x2, தசம y2,
				வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_linear_gradient (c, color_வரி, x0, y0, x1, y1, x2, y2);
}

நிலையான வெறுமை
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				வெறுமை *paint_data,
				hb_color_வரி_t *color_வரி,
				தசம x0, தசம y0, தசம r0,
				தசம x1, தசம y1, தசம r1,
				வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_radial_gradient (c, color_வரி, x0, y0, r0, x1, y1, r1);
}

நிலையான வெறுமை
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       வெறுமை *paint_data,
			       hb_color_வரி_t *color_வரி,
			       தசம x0, தசம y0,
			       தசம start_angle, தசம end_angle,
			       வெறுமை *user_data HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;

  _hb_cairo_paint_sweep_gradient (c, color_வரி, x0, y0, start_angle, end_angle);
}

நிலையான மாறிலி cairo_user_data_சாவி_t color_cache_சாவி = {0};

நிலையான வெறுமை
_hb_cairo_destroy_map (வெறுமை *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

நிலையான hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     வெறுமை *paint_data,
                                     unsigned முழு color_index,
                                     hb_color_t *color,
                                     வெறுமை *user_data HB_UNUSED)
{
#ifdef HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_data;
  cairo_t *cr = c->cr;

#define HB_DEADBEEF HB_TAG(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  எனில் (likely (color_cache && color_cache->has (color_index, &v)))
  {
    எனில் (*v == HB_DEADBEEF)
      திரும்பு பொய்;
    *color = *v;
    திரும்பு உண்மை;
  }

  cairo_font_options_t *options;
  இரட்டை red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  எனில் (CAIRO_STATUS_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_index,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    எனில் (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->set (color_index, *color);

    திரும்பு உண்மை;
  }
  cairo_font_options_destroy (options);

  எனில் (likely (color_cache))
    color_cache->set (color_index, HB_DEADBEEF);

#undef HB_DEADBEEF

#endif

  திரும்பு பொய்;
}

நிலையான hb_bool_t
hb_cairo_paint_set_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   வெறுமை *paint_data,
			   int64_t budget,
			   வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) paint_data)->set_budget (budget);
}

நிலையான int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   வெறுமை *paint_data,
			   வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) paint_data)->get_budget ();
}

நிலையான int64_t *
hb_cairo_paint_get_budget_remaining (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     வெறுமை *paint_data,
				     வெறுமை *user_data HB_UNUSED)
{
  திரும்பு ((hb_cairo_context_t *) paint_data)->get_budget_remaining ();
}

நிலையான உள்வரி வெறுமை free_static_cairo_paint_funcs ();

நிலையான கட்டமைப்பு hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  நிலையான hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_தள்ளு_transform_func (funcs, hb_cairo_தள்ளு_transform, nullptr, nullptr);
    hb_paint_funcs_set_எடு_transform_func (funcs, hb_cairo_எடு_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_cairo_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_cairo_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_glyph_func (funcs, hb_cairo_தள்ளு_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_rectangle_func (funcs, hb_cairo_தள்ளு_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_path_start_func (funcs, hb_cairo_தள்ளு_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_path_end_func (funcs, hb_cairo_தள்ளு_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_எடு_clip_func (funcs, hb_cairo_எடு_clip, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_group_func (funcs, hb_cairo_தள்ளு_group, nullptr, nullptr);
    hb_paint_funcs_set_எடு_group_func (funcs, hb_cairo_எடு_group, nullptr, nullptr);
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

    திரும்பு funcs;
  }
} static_cairo_paint_funcs;

நிலையான உள்வரி
வெறுமை free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

நிலையான hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  திரும்பு static_cairo_paint_funcs.get_unconst ();
}
#endif

நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_face_user_data_சாவி = {0};
நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_font_user_data_சாவி = {0};
நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_font_init_func_user_data_சாவி = {0};
நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_font_init_user_data_user_data_சாவி = {0};
நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_scale_factor_user_data_சாவி = {0};
நிலையான மாறிலி cairo_user_data_சாவி_t hb_cairo_budget_user_data_சாவி = {0};

நிலையான வெறுமை hb_cairo_face_destroy (வெறுமை *p) { hb_face_destroy ((hb_face_t *) p); }
நிலையான வெறுமை hb_cairo_font_destroy (வெறுமை *p) { hb_font_destroy ((hb_font_t *) p); }
நிலையான வெறுமை
hb_cairo_budget_destroy (வெறுமை *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

நிலையான cairo_status_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_UNUSED,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_data (font_face,
								 &hb_cairo_font_user_data_சாவி);

  எனில் (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_data (font_face,
								   &hb_cairo_face_user_data_சாவி);
    font = hb_font_create (face);

#எனில் !defined(HB_NO_VAR) && CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    மாறிலி எழுத்து *variations = cairo_font_options_get_variations (font_options);
    hb_vector_t<hb_variation_t> vars;
    மாறிலி எழுத்து *p = variations;
    போது (p && *p)
    {
      மாறிலி எழுத்து *end = strpbrk ((எழுத்து *) p, ", ");
      hb_variation_t var;
      எனில் (hb_variation_from_string (p, end ? end - p : -1, &var))
	vars.push (var);
      p = end ? end + 1 : nullptr;
    }
    hb_font_set_variations (font, &vars[0], vars.length);

    cairo_font_options_destroy (font_options);
#endif

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    unsigned scale_factor = hb_cairo_font_face_get_scale_factor (font_face);
    எனில் (scale_factor)
    {
      cairo_matrix_t font_matrix;
      cairo_scaled_font_get_scale_matrix (scaled_font, &font_matrix);
      hb_font_set_scale (font,
			 hb_clamp_to<int32_t> (round (font_matrix.xx * scale_factor)),
			 hb_clamp_to<int32_t> (round (font_matrix.yy * scale_factor)));
    }

    auto *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_data (font_face,
						     &hb_cairo_font_init_func_user_data_சாவி);
    எனில் (init_func)
    {
      வெறுமை *user_data = cairo_font_face_get_user_data (font_face,
						       &hb_cairo_font_init_user_data_user_data_சாவி);
      font = init_func (font, scaled_font, user_data);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_set_user_data (scaled_font,
				   &hb_cairo_font_user_data_சாவி,
				   (வெறுமை *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (அளவு (hb_cairo_budget_t));
  எனில் (unlikely (!budget))
    திரும்பு CAIRO_STATUS_NO_MEMORY;
  புதிய (budget) hb_cairo_budget_t ();
  cairo_status_t status = cairo_scaled_font_set_user_data (scaled_font,
						   &hb_cairo_budget_user_data_சாவி,
						   budget,
						   hb_cairo_budget_destroy);
  எனில் (unlikely (status != CAIRO_STATUS_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    திரும்பு status;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (இரட்டை)  hb_extents.ascender  / y_scale;
  extents->descent = -(இரட்டை) hb_extents.descender / y_scale;
  extents->height  = extents->ascent + extents->descent;

#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  எனில் (unlikely (CAIRO_STATUS_SUCCESS != cairo_scaled_font_set_user_data (scaled_font,
									 &color_cache_சாவி,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#endif

  திரும்பு CAIRO_STATUS_SUCCESS;
}

நிலையான cairo_status_t
hb_cairo_text_to_glyphs (cairo_scaled_font_t        *scaled_font,
			 மாறிலி எழுத்து	            *utf8,
			 முழு		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 முழு		            *இலக்கம்_glyphs,
			 cairo_text_cluster_t      **clusters,
			 முழு		            *இலக்கம்_clusters,
			 cairo_text_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_சாவி);

  hb_buffer_t *buffer = hb_buffer_create ();
  hb_buffer_add_utf8 (buffer, utf8, utf8_len, 0, utf8_len);
  hb_buffer_guess_segment_properties (buffer);
  hb_shape (font, buffer, nullptr, 0);

  முழு x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_buffer (buffer,
			       உண்மை,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (unsigned *) இலக்கம்_glyphs,
			       clusters, (unsigned *) இலக்கம்_clusters,
			       cluster_flags);

  hb_buffer_destroy (buffer);

  திரும்பு CAIRO_STATUS_SUCCESS;
}

நிலையான cairo_status_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       unsigned long         glyph,
		       cairo_t              *cr,
		       cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_சாவி);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_data (scaled_font,
									     &hb_cairo_budget_user_data_சாவி);
  எனில் (unlikely (!budget))
    திரும்பு CAIRO_STATUS_NO_MEMORY;
  hb_cairo_context_t c {scaled_font, cr, nullptr, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  எனில் (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  திரும்பு CAIRO_STATUS_SUCCESS;
}

#ifdef HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

நிலையான cairo_status_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     unsigned long         glyph,
			     cairo_t              *cr,
			     cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font,
								   &hb_cairo_font_user_data_சாவி);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_data (scaled_font,
									     &hb_cairo_budget_user_data_சாவி);
  எனில் (unlikely (!budget))
    திரும்பு CAIRO_STATUS_NO_MEMORY;

  unsigned முழு palette = 0;
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
			(hb_map_t *) cairo_scaled_font_get_user_data (scaled_font, &color_cache_சாவி),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  திரும்பு CAIRO_STATUS_SUCCESS;
}

#endif

நிலையான cairo_font_face_t *
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

  எனில் (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (cairo_face,
								       &hb_cairo_face_user_data_சாவி,
								       (வெறுமை *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  திரும்பு cairo_face;
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

  எனில் (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (cairo_face,
								       &hb_cairo_font_user_data_சாவி,
								       (வெறுமை *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  திரும்பு cairo_face;
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
  திரும்பு (hb_font_t *) cairo_font_face_get_user_data (font_face,
						      &hb_cairo_font_user_data_சாவி);
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

  திரும்பு user_font_face_create (face);
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
  திரும்பு (hb_face_t *) cairo_font_face_get_user_data (font_face,
						      &hb_cairo_face_user_data_சாவி);
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
வெறுமை
hb_cairo_font_face_set_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       வெறுமை *user_data,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_set_user_data (font_face,
				 &hb_cairo_font_init_func_user_data_சாவி,
				 (வெறுமை *) func,
				 nullptr);
  எனில் (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_data (font_face,
								       &hb_cairo_font_init_user_data_user_data_சாவி,
								       (வெறுமை *) user_data,
								       destroy)) && destroy)
  {
    destroy (user_data);
    cairo_font_face_set_user_data (font_face,
				   &hb_cairo_font_init_func_user_data_சாவி,
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
  திரும்பு (hb_font_t *) cairo_scaled_font_get_user_data (scaled_font, &hb_cairo_font_user_data_சாவி);
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
வெறுமை
hb_cairo_font_face_set_scale_factor (cairo_font_face_t *font_face,
				     unsigned முழு scale_factor)
{
  cairo_font_face_set_user_data (font_face,
				 &hb_cairo_scale_factor_user_data_சாவி,
				 (வெறுமை *) (uintptr_t) scale_factor,
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
unsigned முழு
hb_cairo_font_face_get_scale_factor (cairo_font_face_t *font_face)
{
  திரும்பு (unsigned முழு) (uintptr_t)
	 cairo_font_face_get_user_data (font_face,
					&hb_cairo_scale_factor_user_data_சாவி);
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
வெறுமை
hb_cairo_glyphs_from_buffer (hb_buffer_t *buffer,
			     hb_bool_t utf8_clusters,
			     இரட்டை x_scale_factor,
			     இரட்டை y_scale_factor,
			     இரட்டை x,
			     இரட்டை y,
			     மாறிலி எழுத்து *utf8,
			     முழு utf8_len,
			     cairo_glyph_t **glyphs,
			     unsigned முழு *இலக்கம்_glyphs,
			     cairo_text_cluster_t **clusters,
			     unsigned முழு *இலக்கம்_clusters,
			     cairo_text_cluster_flags_t *cluster_flags)
{
  எனில் (utf8 && utf8_len < 0)
    utf8_len = strlen (utf8);

  unsigned orig_இலக்கம்_glyphs = *இலக்கம்_glyphs;
  *இலக்கம்_glyphs = hb_buffer_get_length (buffer);
  hb_glyph_info_t *hb_glyph = hb_buffer_get_glyph_infos (buffer, nullptr);
  hb_glyph_position_t *hb_position = hb_buffer_get_glyph_positions (buffer, nullptr);
  எனில் (orig_இலக்கம்_glyphs < *இலக்கம்_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*இலக்கம்_glyphs + 1);

  எனில் (clusters && utf8)
  {
    unsigned orig_இலக்கம்_clusters = *இலக்கம்_clusters;
    *இலக்கம்_clusters = *இலக்கம்_glyphs ? 1 : 0;
    க்கு (unsigned முழு i = 1; i < *இலக்கம்_glyphs; i++)
      எனில் (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*இலக்கம்_clusters)++;
    எனில் (orig_இலக்கம்_clusters < *இலக்கம்_clusters)
      *clusters = cairo_text_cluster_allocate (*இலக்கம்_clusters);
  }

  இரட்டை x_scale = x_scale_factor ? 1. / x_scale_factor : 0.;
  இரட்டை y_scale = y_scale_factor ? 1. / y_scale_factor : 0.;
  இரட்டை hx = 0, hy = 0;
  முழு i;
  க்கு (i = 0; i < (முழு) *இலக்கம்_glyphs; i++)
  {
    (*glyphs)[i].index = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((இரட்டை) hb_position->x_offset + hx) * x_scale;
    (*glyphs)[i].y = y + (-(இரட்டை) hb_position->y_offset + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].index = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  எனில் (clusters && *இலக்கம்_clusters && utf8)
  {
    hb_memset ((வெறுமை *) *clusters, 0, *இலக்கம்_clusters * அளவு ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_buffer_get_direction (buffer));
    *cluster_flags = backward ? CAIRO_TEXT_CLUSTER_FLAG_BACKWARD : (cairo_text_cluster_flags_t) 0;
    unsigned முழு cluster = 0;
    மாறிலி எழுத்து *start = utf8, *end;
    (*clusters)[cluster].இலக்கம்_glyphs++;
    எனில் (backward)
    {
      க்கு (i = *இலக்கம்_glyphs - 2; i >= 0; i--)
      {
	எனில் (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  ப்ரதிஜ்ஞா (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  எனில் (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  இல்லையெனில்
	    end = (மாறிலி எழுத்து *) hb_utf_offset_to_pointer<hb_utf8_t> ((மாறிலி uint8_t *) start,
								      (மாறிலி uint8_t *) utf8, utf8_len,
								      (signed) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].இலக்கம்_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].இலக்கம்_glyphs++;
      }
      (*clusters)[cluster].இலக்கம்_bytes = utf8 + utf8_len - start;
    }
    இல்லையெனில்
    {
      க்கு (i = 1; i < (முழு) *இலக்கம்_glyphs; i++)
      {
	எனில் (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  ப்ரதிஜ்ஞா (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  எனில் (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  இல்லையெனில்
	    end = (மாறிலி எழுத்து *) hb_utf_offset_to_pointer<hb_utf8_t> ((மாறிலி uint8_t *) start,
								      (மாறிலி uint8_t *) utf8, utf8_len,
								      (signed) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].இலக்கம்_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].இலக்கம்_glyphs++;
      }
      (*clusters)[cluster].இலக்கம்_bytes = utf8 + utf8_len - start;
    }
  }
  இல்லையெனில் எனில் (இலக்கம்_clusters)
    *இலக்கம்_clusters = 0;
}

#endif
