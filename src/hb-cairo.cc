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

#समावेश "hb.hh"

#अगर_घोषित HAVE_CAIRO

#समावेश "hb-cairo.h"

#समावेश "hb-cairo-utils.hh"

#समावेश "hb-machinery.hh"
#समावेश "hb-utf.hh"


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

अटल व्योम
hb_cairo_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  व्योम *draw_तथ्य,
		  hb_draw_state_t *st HB_UNUSED,
		  भग्न to_x, भग्न to_y,
		  व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_तथ्य;
  अगर (unlikely (!c->spend (HB_BUDGET_1)))
    वापस;

  cairo_move_to (c->cr, (द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_cairo_पंक्ति_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		  व्योम *draw_तथ्य,
		  hb_draw_state_t *st HB_UNUSED,
		  भग्न to_x, भग्न to_y,
		  व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_तथ्य;
  अगर (unlikely (!c->spend (HB_BUDGET_1)))
    वापस;

  cairo_पंक्ति_to (c->cr, (द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_cairo_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		   व्योम *draw_तथ्य,
		   hb_draw_state_t *st HB_UNUSED,
		   भग्न control1_x, भग्न control1_y,
		   भग्न control2_x, भग्न control2_y,
		   भग्न to_x, भग्न to_y,
		   व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_तथ्य;
  अगर (unlikely (!c->spend (HB_BUDGET_1)))
    वापस;

  cairo_curve_to (c->cr,
                  (द्विगुन) control1_x, (द्विगुन) control1_y,
                  (द्विगुन) control2_x, (द्विगुन) control2_y,
                  (द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_cairo_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     व्योम *draw_तथ्य,
		     hb_draw_state_t *st HB_UNUSED,
		     व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) draw_तथ्य;
  अगर (unlikely (!c->spend (HB_BUDGET_1)))
    वापस;

  cairo_close_path (c->cr);
}

अटल hb_bool_t
hb_cairo_draw_set_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  व्योम *draw_तथ्य,
			  int64_t budget,
			  व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) draw_तथ्य)->set_budget (budget);
}

अटल int64_t
hb_cairo_draw_get_budget (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  व्योम *draw_तथ्य,
			  व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) draw_तथ्य)->get_budget ();
}

अटल int64_t *
hb_cairo_draw_get_budget_remaining (hb_draw_funcs_t *dfuncs HB_UNUSED,
				    व्योम *draw_तथ्य,
				    व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) draw_तथ्य)->get_budget_remaining ();
}

अटल अंतरभूत व्योम free_static_cairo_draw_funcs ();

अटल काष्ठा hb_cairo_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_cairo_draw_funcs_lazy_loader_t>
{
  अटल hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_cairo_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पंक्ति_to_func (funcs, hb_cairo_पंक्ति_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_cairo_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_close_path_func (funcs, hb_cairo_close_path, nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_cairo_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_cairo_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_cairo_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_cairo_draw_funcs);

    वापस funcs;
  }
} static_cairo_draw_funcs;

अटल अंतरभूत
व्योम free_static_cairo_draw_funcs ()
{
  static_cairo_draw_funcs.free_instance ();
}

अटल hb_draw_funcs_t *
hb_cairo_draw_get_funcs ()
{
  वापस static_cairo_draw_funcs.get_unconst ();
}


#अगर_घोषित HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

अटल व्योम
hb_cairo_धकेलो_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
			 व्योम *paint_तथ्य,
			 भग्न xx, भग्न yx,
			 भग्न xy, भग्न yy,
			 भग्न dx, भग्न dy,
			 व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_matrix_t m;

  cairo_save (cr);
  cairo_matrix_init (&m, (द्विगुन) xx, (द्विगुन) yx,
                         (द्विगुन) xy, (द्विगुन) yy,
                         (द्विगुन) dx, (द्विगुन) dy);
  cairo_transform (cr, &m);
}

अटल व्योम
hb_cairo_निकालो_transform (hb_paint_funcs_t *pfuncs HB_UNUSED,
		        व्योम *paint_तथ्य,
		        व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

अटल व्योम
hb_cairo_fill_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     व्योम *paint_तथ्य,
		     hb_codepoint_t glyph,
		     hb_font_t *font,
		     hb_bool_t use_foreground,
		     hb_color_t color,
		     व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_save (cr);

  cairo_new_path (cr);
  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);
  cairo_close_path (cr);
  _hb_cairo_set_source_color (c, use_foreground, color);
  cairo_fill (cr);

  cairo_restore (cr);
}

अटल hb_bool_t
hb_cairo_paint_color_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			    व्योम *paint_तथ्य,
			    hb_codepoint_t glyph,
			    hb_font_t *font,
			    व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  अगर (unlikely (!c->spend (HB_BUDGET_1)))
    वापस true;

  cairo_save (cr);

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr, x_scale, -y_scale);

  cairo_glyph_t cairo_glyph = { glyph, 0, 0 };
  cairo_set_scaled_font (cr, c->scaled_font);
  cairo_set_font_आकार (cr, 1);
  cairo_show_glyphs (cr, &cairo_glyph, 1);

  cairo_restore (cr);

  वापस true;
}

अटल व्योम
hb_cairo_धकेलो_clip_glyph (hb_paint_funcs_t *pfuncs HB_UNUSED,
			  व्योम *paint_तथ्य,
			  hb_codepoint_t glyph,
			  hb_font_t *font,
			  व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);

  hb_font_draw_glyph (font, glyph, hb_cairo_draw_get_funcs (), c);

  cairo_close_path (cr);
  cairo_clip (cr);
}

अटल व्योम
hb_cairo_धकेलो_clip_rectangle (hb_paint_funcs_t *pfuncs HB_UNUSED,
			      व्योम *paint_तथ्य,
			      भग्न xmin, भग्न ymin, भग्न xmax, भग्न ymax,
			      व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_rectangle (cr,
                   (द्विगुन) xmin, (द्विगुन) ymin,
                   (द्विगुन) (xmax - xmin), (द्विगुन) (ymax - ymin));
  cairo_clip (cr);
}

अटल hb_draw_funcs_t *
hb_cairo_धकेलो_clip_path_start (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       व्योम *paint_तथ्य,
			       व्योम **draw_तथ्य,
			       व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_new_path (cr);
  *draw_तथ्य = c;
  वापस hb_cairo_draw_get_funcs ();
}

अटल व्योम
hb_cairo_धकेलो_clip_path_end (hb_paint_funcs_t *pfuncs HB_UNUSED,
			     व्योम *paint_तथ्य,
			     व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_close_path (cr);
  cairo_clip (cr);
}

अटल व्योम
hb_cairo_निकालो_clip (hb_paint_funcs_t *pfuncs HB_UNUSED,
		   व्योम *paint_तथ्य,
		   व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_restore (cr);
}

अटल व्योम
hb_cairo_धकेलो_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		     व्योम *paint_तथ्य,
		     व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_save (cr);
  cairo_धकेलो_group (cr);
}

अटल व्योम
hb_cairo_निकालो_group (hb_paint_funcs_t *pfuncs HB_UNUSED,
		    व्योम *paint_तथ्य,
		    hb_paint_composite_mode_t mode,
		    व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  cairo_निकालो_group_to_source (cr);
  cairo_set_operator (cr, _hb_paint_composite_mode_to_cairo (mode));
  cairo_paint (cr);

  cairo_restore (cr);
}

अटल व्योम
hb_cairo_paint_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      व्योम *paint_तथ्य,
		      hb_bool_t use_foreground,
		      hb_color_t color,
		      व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

  _hb_cairo_set_source_color (c, use_foreground, color);
  cairo_paint (cr);
}

अटल hb_bool_t
hb_cairo_paint_image (hb_paint_funcs_t *pfuncs HB_UNUSED,
		      व्योम *paint_तथ्य,
		      hb_blob_t *blob,
		      अचिन्हित width,
		      अचिन्हित height,
		      hb_tag_t format,
		      भग्न slant,
		      hb_glyph_extents_t *extents,
		      व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;

  वापस _hb_cairo_paint_glyph_image (c, blob, width, height, format, slant, extents);
}

अटल व्योम
hb_cairo_paint_linear_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				व्योम *paint_तथ्य,
				hb_color_पंक्ति_t *color_पंक्ति,
				भग्न x0, भग्न y0,
				भग्न x1, भग्न y1,
				भग्न x2, भग्न y2,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;

  _hb_cairo_paint_linear_gradient (c, color_पंक्ति, x0, y0, x1, y1, x2, y2);
}

अटल व्योम
hb_cairo_paint_radial_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
				व्योम *paint_तथ्य,
				hb_color_पंक्ति_t *color_पंक्ति,
				भग्न x0, भग्न y0, भग्न r0,
				भग्न x1, भग्न y1, भग्न r1,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;

  _hb_cairo_paint_radial_gradient (c, color_पंक्ति, x0, y0, r0, x1, y1, r1);
}

अटल व्योम
hb_cairo_paint_sweep_gradient (hb_paint_funcs_t *pfuncs HB_UNUSED,
			       व्योम *paint_तथ्य,
			       hb_color_पंक्ति_t *color_पंक्ति,
			       भग्न x0, भग्न y0,
			       भग्न start_angle, भग्न end_angle,
			       व्योम *user_तथ्य HB_UNUSED)
{
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;

  _hb_cairo_paint_sweep_gradient (c, color_पंक्ति, x0, y0, start_angle, end_angle);
}

अटल स्थिर cairo_user_तथ्य_कुंजी_t color_cache_कुंजी = {0};

अटल व्योम
_hb_cairo_destroy_map (व्योम *p)
{
  hb_map_destroy ((hb_map_t *) p);
}

अटल hb_bool_t
hb_cairo_paint_custom_palette_color (hb_paint_funcs_t *funcs,
                                     व्योम *paint_तथ्य,
                                     अचिन्हित पूर्णांक color_सूचकांक,
                                     hb_color_t *color,
                                     व्योम *user_तथ्य HB_UNUSED)
{
#अगर_घोषित HAVE_CAIRO_FONT_OPTIONS_GET_CUSTOM_PALETTE_COLOR
  hb_cairo_context_t *c = (hb_cairo_context_t *) paint_तथ्य;
  cairo_t *cr = c->cr;

#घोषणा HB_DEADBEEF HB_TAG(0xDE,0xAD,0xBE,0xEF)

  hb_map_t *color_cache = c->color_cache;
  hb_codepoint_t *v;
  अगर (likely (color_cache && color_cache->has (color_सूचकांक, &v)))
  {
    अगर (*v == HB_DEADBEEF)
      वापस false;
    *color = *v;
    वापस true;
  }

  cairo_font_options_t *options;
  द्विगुन red, green, blue, alpha;

  options = cairo_font_options_create ();
  cairo_get_font_options (cr, options);
  अगर (CAIRO_STATUS_SUCCESS ==
      cairo_font_options_get_custom_palette_color (options, color_सूचकांक,
                                                   &red, &green, &blue, &alpha))
  {
    cairo_font_options_destroy (options);
    *color = HB_COLOR (round (255 * blue),
		       round (255 * green),
		       round (255 * red),
		       round (255 * alpha));

    अगर (likely (color_cache && *color != HB_DEADBEEF))
      color_cache->set (color_सूचकांक, *color);

    वापस true;
  }
  cairo_font_options_destroy (options);

  अगर (likely (color_cache))
    color_cache->set (color_सूचकांक, HB_DEADBEEF);

#अघोषित HB_DEADBEEF

#पूर्ण_अगर

  वापस false;
}

अटल hb_bool_t
hb_cairo_paint_set_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   व्योम *paint_तथ्य,
			   int64_t budget,
			   व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) paint_तथ्य)->set_budget (budget);
}

अटल int64_t
hb_cairo_paint_get_budget (hb_paint_funcs_t *pfuncs HB_UNUSED,
			   व्योम *paint_तथ्य,
			   व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) paint_तथ्य)->get_budget ();
}

अटल int64_t *
hb_cairo_paint_get_budget_remaining (hb_paint_funcs_t *pfuncs HB_UNUSED,
				     व्योम *paint_तथ्य,
				     व्योम *user_तथ्य HB_UNUSED)
{
  वापस ((hb_cairo_context_t *) paint_तथ्य)->get_budget_remaining ();
}

अटल अंतरभूत व्योम free_static_cairo_paint_funcs ();

अटल काष्ठा hb_cairo_paint_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_cairo_paint_funcs_lazy_loader_t>
{
  अटल hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_धकेलो_transform_func (funcs, hb_cairo_धकेलो_transform, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_transform_func (funcs, hb_cairo_निकालो_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, hb_cairo_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, hb_cairo_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_glyph_func (funcs, hb_cairo_धकेलो_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_rectangle_func (funcs, hb_cairo_धकेलो_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_start_func (funcs, hb_cairo_धकेलो_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_end_func (funcs, hb_cairo_धकेलो_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_clip_func (funcs, hb_cairo_निकालो_clip, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_group_func (funcs, hb_cairo_धकेलो_group, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_group_func (funcs, hb_cairo_निकालो_group, nullptr, nullptr);
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

    वापस funcs;
  }
} static_cairo_paint_funcs;

अटल अंतरभूत
व्योम free_static_cairo_paint_funcs ()
{
  static_cairo_paint_funcs.free_instance ();
}

अटल hb_paint_funcs_t *
hb_cairo_paint_get_funcs ()
{
  वापस static_cairo_paint_funcs.get_unconst ();
}
#पूर्ण_अगर

अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_face_user_तथ्य_कुंजी = {0};
अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_font_user_तथ्य_कुंजी = {0};
अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_font_init_func_user_तथ्य_कुंजी = {0};
अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_font_init_user_तथ्य_user_तथ्य_कुंजी = {0};
अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_scale_factor_user_तथ्य_कुंजी = {0};
अटल स्थिर cairo_user_तथ्य_कुंजी_t hb_cairo_budget_user_तथ्य_कुंजी = {0};

अटल व्योम hb_cairo_face_destroy (व्योम *p) { hb_face_destroy ((hb_face_t *) p); }
अटल व्योम hb_cairo_font_destroy (व्योम *p) { hb_font_destroy ((hb_font_t *) p); }
अटल व्योम
hb_cairo_budget_destroy (व्योम *p)
{
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) p;
  budget->~hb_cairo_budget_t ();
  hb_free (budget);
}

अटल cairo_status_t
hb_cairo_init_scaled_font (cairo_scaled_font_t  *scaled_font,
			   cairo_t              *cr HB_UNUSED,
			   cairo_font_extents_t *extents)
{
  cairo_font_face_t *font_face = cairo_scaled_font_get_font_face (scaled_font);

  hb_font_t *font = (hb_font_t *) cairo_font_face_get_user_तथ्य (font_face,
								 &hb_cairo_font_user_तथ्य_कुंजी);

  अगर (!font)
  {
    hb_face_t *face = (hb_face_t *) cairo_font_face_get_user_तथ्य (font_face,
								   &hb_cairo_face_user_तथ्य_कुंजी);
    font = hb_font_create (face);

#अगर !defined(HB_NO_VAR) && CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1,16,0)
    cairo_font_options_t *font_options = cairo_font_options_create ();

    // Set variations
    cairo_scaled_font_get_font_options (scaled_font, font_options);
    स्थिर अक्षर *variations = cairo_font_options_get_variations (font_options);
    hb_vector_t<hb_variation_t> vars;
    स्थिर अक्षर *p = variations;
    जबतक (p && *p)
    {
      स्थिर अक्षर *end = strpbrk ((अक्षर *) p, ", ");
      hb_variation_t var;
      अगर (hb_variation_from_string (p, end ? end - p : -1, &var))
	vars.push (var);
      p = end ? end + 1 : nullptr;
    }
    hb_font_set_variations (font, &vars[0], vars.लंबाई);

    cairo_font_options_destroy (font_options);
#पूर्ण_अगर

    // Set scale; Note: should NOT set slant, or we'll double-slant.
    अचिन्हित scale_factor = hb_cairo_font_face_get_scale_factor (font_face);
    अगर (scale_factor)
    {
      cairo_matrix_t font_matrix;
      cairo_scaled_font_get_scale_matrix (scaled_font, &font_matrix);
      hb_font_set_scale (font,
			 hb_clamp_to<int32_t> (round (font_matrix.xx * scale_factor)),
			 hb_clamp_to<int32_t> (round (font_matrix.yy * scale_factor)));
    }

    स्वतः *init_func = (hb_cairo_font_init_func_t)
		      cairo_font_face_get_user_तथ्य (font_face,
						     &hb_cairo_font_init_func_user_तथ्य_कुंजी);
    अगर (init_func)
    {
      व्योम *user_तथ्य = cairo_font_face_get_user_तथ्य (font_face,
						       &hb_cairo_font_init_user_तथ्य_user_तथ्य_कुंजी);
      font = init_func (font, scaled_font, user_तथ्य);
    }

    hb_font_make_immutable (font);
  }

  cairo_scaled_font_set_user_तथ्य (scaled_font,
				   &hb_cairo_font_user_तथ्य_कुंजी,
				   (व्योम *) hb_font_reference (font),
				   hb_cairo_font_destroy);

  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) hb_malloc (माप (hb_cairo_budget_t));
  अगर (unlikely (!budget))
    वापस CAIRO_STATUS_NO_MEMORY;
  नया (budget) hb_cairo_budget_t ();
  cairo_status_t status = cairo_scaled_font_set_user_तथ्य (scaled_font,
						   &hb_cairo_budget_user_तथ्य_कुंजी,
						   budget,
						   hb_cairo_budget_destroy);
  अगर (unlikely (status != CAIRO_STATUS_SUCCESS))
  {
    hb_cairo_budget_destroy (budget);
    वापस status;
  }

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_font_extents_t hb_extents;
  hb_font_get_h_extents (font, &hb_extents);

  extents->ascent  = (द्विगुन)  hb_extents.ascender  / y_scale;
  extents->descent = -(द्विगुन) hb_extents.descender / y_scale;
  extents->height  = extents->ascent + extents->descent;

#अगर_घोषित HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC
  hb_map_t *color_cache = hb_map_create ();
  अगर (unlikely (CAIRO_STATUS_SUCCESS != cairo_scaled_font_set_user_तथ्य (scaled_font,
									 &color_cache_कुंजी,
									 color_cache,
									 _hb_cairo_destroy_map)))
    hb_map_destroy (color_cache);
#पूर्ण_अगर

  वापस CAIRO_STATUS_SUCCESS;
}

अटल cairo_status_t
hb_cairo_text_to_glyphs (cairo_scaled_font_t        *scaled_font,
			 स्थिर अक्षर	            *utf8,
			 पूर्णांक		             utf8_len,
			 cairo_glyph_t	           **glyphs,
			 पूर्णांक		            *अंक_glyphs,
			 cairo_text_cluster_t      **clusters,
			 पूर्णांक		            *अंक_clusters,
			 cairo_text_cluster_flags_t *cluster_flags)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_तथ्य (scaled_font,
								   &hb_cairo_font_user_तथ्य_कुंजी);

  hb_बफर_t *बफर = hb_बफर_create ();
  hb_बफर_add_utf8 (बफर, utf8, utf8_len, 0, utf8_len);
  hb_बफर_guess_segment_properties (बफर);
  hb_shape (font, बफर, nullptr, 0);

  पूर्णांक x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  hb_cairo_glyphs_from_बफर (बफर,
			       true,
			       x_scale, y_scale,
			       0., 0.,
			       utf8, utf8_len,
			       glyphs, (अचिन्हित *) अंक_glyphs,
			       clusters, (अचिन्हित *) अंक_clusters,
			       cluster_flags);

  hb_बफर_destroy (बफर);

  वापस CAIRO_STATUS_SUCCESS;
}

अटल cairo_status_t
hb_cairo_render_glyph (cairo_scaled_font_t  *scaled_font,
		       अचिन्हित दीर्घ         glyph,
		       cairo_t              *cr,
		       cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_तथ्य (scaled_font,
								   &hb_cairo_font_user_तथ्य_कुंजी);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_तथ्य (scaled_font,
									     &hb_cairo_budget_user_तथ्य_कुंजी);
  अगर (unlikely (!budget))
    वापस CAIRO_STATUS_NO_MEMORY;
  hb_cairo_context_t c {scaled_font, cr, nullptr, budget};

  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);

  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));
  अगर (hb_font_draw_glyph_or_fail (font, glyph, hb_cairo_draw_get_funcs (), &c))
    cairo_fill (cr);

  // If draw fails, we still return SUCCESS, as we want empty drawing, not
  // setting the cairo object into error.
  वापस CAIRO_STATUS_SUCCESS;
}

#अगर_घोषित HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC

अटल cairo_status_t
hb_cairo_render_color_glyph (cairo_scaled_font_t  *scaled_font,
			     अचिन्हित दीर्घ         glyph,
			     cairo_t              *cr,
			     cairo_text_extents_t *extents)
{
  hb_font_t *font = (hb_font_t *) cairo_scaled_font_get_user_तथ्य (scaled_font,
								   &hb_cairo_font_user_तथ्य_कुंजी);
  hb_cairo_budget_t *budget = (hb_cairo_budget_t *) cairo_scaled_font_get_user_तथ्य (scaled_font,
									     &hb_cairo_budget_user_तथ्य_कुंजी);
  अगर (unlikely (!budget))
    वापस CAIRO_STATUS_NO_MEMORY;

  अचिन्हित पूर्णांक palette = 0;
#अगर_घोषित CAIRO_COLOR_PALETTE_DEFAULT
  cairo_font_options_t *options = cairo_font_options_create ();
  cairo_scaled_font_get_font_options (scaled_font, options);
  palette = cairo_font_options_get_color_palette (options);
  cairo_font_options_destroy (options);
#पूर्ण_अगर

  hb_color_t color = HB_COLOR (0, 0, 0, 255);
  hb_position_t x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  cairo_scale (cr,
	       +1. / (x_scale ? x_scale : 1),
	       -1. / (y_scale ? y_scale : 1));

  hb_cairo_context_t c {scaled_font,
			cr,
			(hb_map_t *) cairo_scaled_font_get_user_तथ्य (scaled_font, &color_cache_कुंजी),
			budget};

  /* Synthesizing variant: mono glyphs render here too via the
   * fill_glyph foreground fallback inside hb_font_paint_glyph.
   * Callers that want the cheaper outline path for mono fonts set
   * CAIRO_COLOR_MODE_NO_COLOR on the font options instead. */
  hb_font_paint_glyph (font, glyph, hb_cairo_paint_get_funcs (), &c, palette, color);

  वापस CAIRO_STATUS_SUCCESS;
}

#पूर्ण_अगर

अटल cairo_font_face_t *
user_font_face_create (hb_face_t *face)
{
  cairo_font_face_t *cairo_face;

  cairo_face = cairo_user_font_face_create ();
  cairo_user_font_face_set_init_func (cairo_face, hb_cairo_init_scaled_font);
  cairo_user_font_face_set_text_to_glyphs_func (cairo_face, hb_cairo_text_to_glyphs);
  cairo_user_font_face_set_render_glyph_func (cairo_face, hb_cairo_render_glyph);
#अगर_घोषित HAVE_CAIRO_USER_FONT_FACE_SET_RENDER_COLOR_GLYPH_FUNC
  cairo_user_font_face_set_render_color_glyph_func (cairo_face, hb_cairo_render_color_glyph);
#पूर्ण_अगर

  अगर (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_तथ्य (cairo_face,
								       &hb_cairo_face_user_तथ्य_कुंजी,
								       (व्योम *) hb_face_reference (face),
								       hb_cairo_face_destroy)))
    hb_face_destroy (face);

  वापस cairo_face;
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

  स्वतः *hb_face = hb_font_get_face (font);
  स्वतः *cairo_face =  user_font_face_create (hb_face);

  अगर (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_तथ्य (cairo_face,
								       &hb_cairo_font_user_तथ्य_कुंजी,
								       (व्योम *) hb_font_reference (font),
								       hb_cairo_font_destroy)))
    hb_font_destroy (font);

  वापस cairo_face;
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
  वापस (hb_font_t *) cairo_font_face_get_user_तथ्य (font_face,
						      &hb_cairo_font_user_तथ्य_कुंजी);
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

  वापस user_font_face_create (face);
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
  वापस (hb_face_t *) cairo_font_face_get_user_तथ्य (font_face,
						      &hb_cairo_face_user_तथ्य_कुंजी);
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
व्योम
hb_cairo_font_face_set_font_init_func (cairo_font_face_t *font_face,
				       hb_cairo_font_init_func_t func,
				       व्योम *user_तथ्य,
				       hb_destroy_func_t destroy)
{
  cairo_font_face_set_user_तथ्य (font_face,
				 &hb_cairo_font_init_func_user_तथ्य_कुंजी,
				 (व्योम *) func,
				 nullptr);
  अगर (unlikely (CAIRO_STATUS_SUCCESS != cairo_font_face_set_user_तथ्य (font_face,
								       &hb_cairo_font_init_user_तथ्य_user_तथ्य_कुंजी,
								       (व्योम *) user_तथ्य,
								       destroy)) && destroy)
  {
    destroy (user_तथ्य);
    cairo_font_face_set_user_तथ्य (font_face,
				   &hb_cairo_font_init_func_user_तथ्य_कुंजी,
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
  वापस (hb_font_t *) cairo_scaled_font_get_user_तथ्य (scaled_font, &hb_cairo_font_user_तथ्य_कुंजी);
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
व्योम
hb_cairo_font_face_set_scale_factor (cairo_font_face_t *font_face,
				     अचिन्हित पूर्णांक scale_factor)
{
  cairo_font_face_set_user_तथ्य (font_face,
				 &hb_cairo_scale_factor_user_तथ्य_कुंजी,
				 (व्योम *) (uintptr_t) scale_factor,
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
अचिन्हित पूर्णांक
hb_cairo_font_face_get_scale_factor (cairo_font_face_t *font_face)
{
  वापस (अचिन्हित पूर्णांक) (uintptr_t)
	 cairo_font_face_get_user_तथ्य (font_face,
					&hb_cairo_scale_factor_user_तथ्य_कुंजी);
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
व्योम
hb_cairo_glyphs_from_बफर (hb_बफर_t *बफर,
			     hb_bool_t utf8_clusters,
			     द्विगुन x_scale_factor,
			     द्विगुन y_scale_factor,
			     द्विगुन x,
			     द्विगुन y,
			     स्थिर अक्षर *utf8,
			     पूर्णांक utf8_len,
			     cairo_glyph_t **glyphs,
			     अचिन्हित पूर्णांक *अंक_glyphs,
			     cairo_text_cluster_t **clusters,
			     अचिन्हित पूर्णांक *अंक_clusters,
			     cairo_text_cluster_flags_t *cluster_flags)
{
  अगर (utf8 && utf8_len < 0)
    utf8_len = म_माप (utf8);

  अचिन्हित orig_अंक_glyphs = *अंक_glyphs;
  *अंक_glyphs = hb_बफर_get_लंबाई (बफर);
  hb_glyph_info_t *hb_glyph = hb_बफर_get_glyph_infos (बफर, nullptr);
  hb_glyph_position_t *hb_position = hb_बफर_get_glyph_positions (बफर, nullptr);
  अगर (orig_अंक_glyphs < *अंक_glyphs + 1)
    *glyphs = cairo_glyph_allocate (*अंक_glyphs + 1);

  अगर (clusters && utf8)
  {
    अचिन्हित orig_अंक_clusters = *अंक_clusters;
    *अंक_clusters = *अंक_glyphs ? 1 : 0;
    क्रम (अचिन्हित पूर्णांक i = 1; i < *अंक_glyphs; i++)
      अगर (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	(*अंक_clusters)++;
    अगर (orig_अंक_clusters < *अंक_clusters)
      *clusters = cairo_text_cluster_allocate (*अंक_clusters);
  }

  द्विगुन x_scale = x_scale_factor ? 1. / x_scale_factor : 0.;
  द्विगुन y_scale = y_scale_factor ? 1. / y_scale_factor : 0.;
  द्विगुन hx = 0, hy = 0;
  पूर्णांक i;
  क्रम (i = 0; i < (पूर्णांक) *अंक_glyphs; i++)
  {
    (*glyphs)[i].सूचकांक_2 = hb_glyph[i].codepoint;
    (*glyphs)[i].x = x + ((द्विगुन) hb_position->x_offset + hx) * x_scale;
    (*glyphs)[i].y = y + (-(द्विगुन) hb_position->y_offset + hy) * y_scale;
    hx +=  hb_position->x_advance;
    hy -= hb_position->y_advance;

    hb_position++;
  }
  (*glyphs)[i].सूचकांक_2 = -1;
  (*glyphs)[i].x = round (hx * x_scale);
  (*glyphs)[i].y = round (hy * y_scale);

  अगर (clusters && *अंक_clusters && utf8)
  {
    hb_memset ((व्योम *) *clusters, 0, *अंक_clusters * माप ((*clusters)[0]));
    hb_bool_t backward = HB_DIRECTION_IS_BACKWARD (hb_बफर_get_direction (बफर));
    *cluster_flags = backward ? CAIRO_TEXT_CLUSTER_FLAG_BACKWARD : (cairo_text_cluster_flags_t) 0;
    अचिन्हित पूर्णांक cluster = 0;
    स्थिर अक्षर *start = utf8, *end;
    (*clusters)[cluster].अंक_glyphs++;
    अगर (backward)
    {
      क्रम (i = *अंक_glyphs - 2; i >= 0; i--)
      {
	अगर (hb_glyph[i].cluster != hb_glyph[i+1].cluster)
	{
	  निश्चित (hb_glyph[i].cluster > hb_glyph[i+1].cluster);
	  अगर (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i+1].cluster;
	  अन्यथा
	    end = (स्थिर अक्षर *) hb_utf_offset_to_pointer<hb_utf8_t> ((स्थिर uint8_t *) start,
								      (स्थिर uint8_t *) utf8, utf8_len,
								      (चिन्हित) (hb_glyph[i].cluster - hb_glyph[i+1].cluster));
	  (*clusters)[cluster].अंक_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].अंक_glyphs++;
      }
      (*clusters)[cluster].अंक_bytes = utf8 + utf8_len - start;
    }
    अन्यथा
    {
      क्रम (i = 1; i < (पूर्णांक) *अंक_glyphs; i++)
      {
	अगर (hb_glyph[i].cluster != hb_glyph[i-1].cluster)
	{
	  निश्चित (hb_glyph[i].cluster > hb_glyph[i-1].cluster);
	  अगर (utf8_clusters)
	    end = start + hb_glyph[i].cluster - hb_glyph[i-1].cluster;
	  अन्यथा
	    end = (स्थिर अक्षर *) hb_utf_offset_to_pointer<hb_utf8_t> ((स्थिर uint8_t *) start,
								      (स्थिर uint8_t *) utf8, utf8_len,
								      (चिन्हित) (hb_glyph[i].cluster - hb_glyph[i-1].cluster));
	  (*clusters)[cluster].अंक_bytes = end - start;
	  start = end;
	  cluster++;
	}
	(*clusters)[cluster].अंक_glyphs++;
      }
      (*clusters)[cluster].अंक_bytes = utf8 + utf8_len - start;
    }
  }
  अन्यथा अगर (अंक_clusters)
    *अंक_clusters = 0;
}

#पूर्ण_अगर
