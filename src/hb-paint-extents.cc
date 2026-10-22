/*
 * Copyright © 2022 Behdad Esfahbod
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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_PAINT

#अन्तर्भाव "hb-paint-extents.hh"

#अन्तर्भाव "hb-draw.hh"

#अन्तर्भाव "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

स्थिर शून्य
hb_paint_extents_प्रक्षिप_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 शून्य *paint_data,
				 भिन्न xx, भिन्न yx,
				 भिन्न xy, भिन्न yy,
				 भिन्न dx, भिन्न dy,
				 शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->प्रक्षिप_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

स्थिर शून्य
hb_paint_extents_निष्कासय_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        शून्य *paint_data,
				शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->निष्कासय_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
संरचना hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  तार्किक consume_segment ()
  {
    पटिनिवत्त hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

स्थिर शून्य
hb_paint_extents_गणनम्_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *data,
				hb_draw_state_t *st HB_UNUSED,
				भिन्न to_x, भिन्न to_y,
				शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  सचे (unlikely (!sink->consume_segment ())) पटिनिवत्त;
  sink->extents.add_point (to_x, to_y);
}

स्थिर शून्य
hb_paint_extents_गणनम्_पङ्क्तिः_to (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *data,
				hb_draw_state_t *st HB_UNUSED,
				भिन्न to_x, भिन्न to_y,
				शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  सचे (unlikely (!sink->consume_segment ())) पटिनिवत्त;
  sink->extents.add_point (to_x, to_y);
}

स्थिर शून्य
hb_paint_extents_गणनम्_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *data,
				     hb_draw_state_t *st HB_UNUSED,
				     भिन्न control_x, भिन्न control_y,
				     भिन्न to_x, भिन्न to_y,
				     शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  सचे (unlikely (!sink->consume_segment ())) पटिनिवत्त;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

स्थिर शून्य
hb_paint_extents_गणनम्_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, शून्य *data,
				 hb_draw_state_t *st HB_UNUSED,
				 भिन्न control1_x, भिन्न control1_y,
				 भिन्न control2_x, भिन्न control2_y,
				 भिन्न to_x, भिन्न to_y,
				 शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  सचे (unlikely (!sink->consume_segment ())) पटिनिवत्त;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

स्थिर int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, शून्य *draw_data, शून्य *)
{
  पटिनिवत्त ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

स्थिर अन्तःस्थ शून्य free_static_paint_extents_draw_funcs ();

स्थिर संरचना hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_गणनम्_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पङ्क्तिः_to_func (funcs, hb_paint_extents_गणनम्_पङ्क्तिः_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_गणनम्_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_गणनम्_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    पटिनिवत्त funcs;
  }
} static_paint_extents_draw_funcs;

स्थिर अन्तःस्थ
शून्य free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

स्थिर शून्य
hb_paint_extents_प्रक्षिप_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  शून्य *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  सचे (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->प्रक्षिप_clip (sink.extents);
}

स्थिर शून्य
hb_paint_extents_प्रक्षिप_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      शून्य *paint_data,
				      भिन्न xmin, भिन्न ymin, भिन्न xmax, भिन्न ymax,
				      शून्य *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->प्रक्षिप_clip (extents);
}

स्थिर शून्य
hb_paint_extents_निष्कासय_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   शून्य *paint_data,
			   शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->निष्कासय_clip ();
}

स्थिर शून्य
hb_paint_extents_प्रक्षिप_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     शून्य *paint_data,
			     शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->प्रक्षिप_group ();
}

स्थिर शून्य
hb_paint_extents_निष्कासय_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    शून्य *paint_data,
			    hb_paint_composite_mode_t mode,
			    शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->निष्कासय_group (mode);
}

स्थिर hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      शून्य *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned पूर्णाङ्क width HB_UNUSED,
			      unsigned पूर्णाङ्क height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      भिन्न slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  सचे (!glyph_extents)
    पटिनिवत्त मिच्छा; // Happens with SVG images.

  hb_extents_t<> extents = {(भिन्न) glyph_extents->x_bearing,
			    (भिन्न) glyph_extents->y_bearing + glyph_extents->height,
			    (भिन्न) glyph_extents->x_bearing + glyph_extents->width,
			    (भिन्न) glyph_extents->y_bearing};
  c->प्रक्षिप_clip (extents);
  c->paint ();
  c->निष्कासय_clip ();

  पटिनिवत्त सच्च;
}

स्थिर शून्य
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      शून्य *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        शून्य *paint_data,
				        hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				        भिन्न x0 HB_UNUSED, भिन्न y0 HB_UNUSED,
				        भिन्न x1 HB_UNUSED, भिन्न y1 HB_UNUSED,
				        भिन्न x2 HB_UNUSED, भिन्न y2 HB_UNUSED,
				        शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        शून्य *paint_data,
				        hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				        भिन्न x0 HB_UNUSED, भिन्न y0 HB_UNUSED, भिन्न r0 HB_UNUSED,
				        भिन्न x1 HB_UNUSED, भिन्न y1 HB_UNUSED, भिन्न r1 HB_UNUSED,
				        शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

स्थिर शून्य
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       शून्य *paint_data,
				       hb_color_पङ्क्तिः_t *color_पङ्क्तिः HB_UNUSED,
				       भिन्न cx HB_UNUSED, भिन्न cy HB_UNUSED,
				       भिन्न start_angle HB_UNUSED,
				       भिन्न end_angle HB_UNUSED,
				       शून्य *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

स्थिर hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, शून्य *paint_data,
			     int64_t budget, शून्य *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = सच्च;
  context->recharge_budget ();
  पटिनिवत्त सच्च;
}

स्थिर int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, शून्य *paint_data, शून्य *)
{
  पटिनिवत्त ((hb_paint_extents_context_t *) paint_data)->budget;
}

स्थिर int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, शून्य *paint_data, शून्य *)
{
  पटिनिवत्त &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

स्थिर अन्तःस्थ शून्य free_static_paint_extents_funcs ();

स्थिर संरचना hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_प्रक्षिप_transform_func (funcs, hb_paint_extents_प्रक्षिप_transform, nullptr, nullptr);
    hb_paint_funcs_set_निष्कासय_transform_func (funcs, hb_paint_extents_निष्कासय_transform, nullptr, nullptr);
    hb_paint_funcs_set_प्रक्षिप_clip_glyph_func (funcs, hb_paint_extents_प्रक्षिप_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_प्रक्षिप_clip_rectangle_func (funcs, hb_paint_extents_प्रक्षिप_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_निष्कासय_clip_func (funcs, hb_paint_extents_निष्कासय_clip, nullptr, nullptr);
    hb_paint_funcs_set_प्रक्षिप_group_func (funcs, hb_paint_extents_प्रक्षिप_group, nullptr, nullptr);
    hb_paint_funcs_set_निष्कासय_group_func (funcs, hb_paint_extents_निष्कासय_group, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, hb_paint_extents_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, hb_paint_extents_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, hb_paint_extents_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_paint_extents_set_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_paint_extents_get_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_funcs);

    पटिनिवत्त funcs;
  }
} static_paint_extents_funcs;

स्थिर अन्तःस्थ
शून्य free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  पटिनिवत्त static_paint_extents_funcs.get_unconst ();
}


#endif
