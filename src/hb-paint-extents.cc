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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_PAINT

#समावेश "hb-paint-extents.hh"

#समावेश "hb-draw.hh"

#समावेश "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

अटल व्योम
hb_paint_extents_धकेलो_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 व्योम *paint_तथ्य,
				 भग्न xx, भग्न yx,
				 भग्न xy, भग्न yy,
				 भग्न dx, भग्न dy,
				 व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->धकेलो_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

अटल व्योम
hb_paint_extents_निकालो_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        व्योम *paint_तथ्य,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->निकालो_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
काष्ठा hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  bool consume_segment ()
  {
    वापस hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

अटल व्योम
hb_paint_extents_गिनती_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *तथ्य,
				hb_draw_state_t *st HB_UNUSED,
				भग्न to_x, भग्न to_y,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) तथ्य;
  अगर (unlikely (!sink->consume_segment ())) वापस;
  sink->extents.add_point (to_x, to_y);
}

अटल व्योम
hb_paint_extents_गिनती_पंक्ति_to (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *तथ्य,
				hb_draw_state_t *st HB_UNUSED,
				भग्न to_x, भग्न to_y,
				व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) तथ्य;
  अगर (unlikely (!sink->consume_segment ())) वापस;
  sink->extents.add_point (to_x, to_y);
}

अटल व्योम
hb_paint_extents_गिनती_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *तथ्य,
				     hb_draw_state_t *st HB_UNUSED,
				     भग्न control_x, भग्न control_y,
				     भग्न to_x, भग्न to_y,
				     व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) तथ्य;
  अगर (unlikely (!sink->consume_segment ())) वापस;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

अटल व्योम
hb_paint_extents_गिनती_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, व्योम *तथ्य,
				 hb_draw_state_t *st HB_UNUSED,
				 भग्न control1_x, भग्न control1_y,
				 भग्न control2_x, भग्न control2_y,
				 भग्न to_x, भग्न to_y,
				 व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) तथ्य;
  अगर (unlikely (!sink->consume_segment ())) वापस;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

अटल int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, व्योम *draw_तथ्य, व्योम *)
{
  वापस ((hb_paint_extents_sink_t *) draw_तथ्य)->budget_remaining;
}

अटल अंतरभूत व्योम free_static_paint_extents_draw_funcs ();

अटल काष्ठा hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  अटल hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_गिनती_move_to, nullptr, nullptr);
    hb_draw_funcs_set_पंक्ति_to_func (funcs, hb_paint_extents_गिनती_पंक्ति_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_गिनती_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_गिनती_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    वापस funcs;
  }
} static_paint_extents_draw_funcs;

अटल अंतरभूत
व्योम free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

अटल व्योम
hb_paint_extents_धकेलो_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  व्योम *paint_तथ्य,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  अगर (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->धकेलो_clip (sink.extents);
}

अटल व्योम
hb_paint_extents_धकेलो_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      व्योम *paint_तथ्य,
				      भग्न xmin, भग्न ymin, भग्न xmax, भग्न ymax,
				      व्योम *user_तथ्य)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->धकेलो_clip (extents);
}

अटल व्योम
hb_paint_extents_निकालो_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   व्योम *paint_तथ्य,
			   व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->निकालो_clip ();
}

अटल व्योम
hb_paint_extents_धकेलो_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     व्योम *paint_तथ्य,
			     व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->धकेलो_group ();
}

अटल व्योम
hb_paint_extents_निकालो_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    व्योम *paint_तथ्य,
			    hb_paint_composite_mode_t mode,
			    व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->निकालो_group (mode);
}

अटल hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      व्योम *paint_तथ्य,
			      hb_blob_t *blob HB_UNUSED,
			      अचिन्हित पूर्णांक width HB_UNUSED,
			      अचिन्हित पूर्णांक height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      भग्न slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  अगर (!glyph_extents)
    वापस false; // Happens with SVG images.

  hb_extents_t<> extents = {(भग्न) glyph_extents->x_bearing,
			    (भग्न) glyph_extents->y_bearing + glyph_extents->height,
			    (भग्न) glyph_extents->x_bearing + glyph_extents->width,
			    (भग्न) glyph_extents->y_bearing};
  c->धकेलो_clip (extents);
  c->paint ();
  c->निकालो_clip ();

  वापस true;
}

अटल व्योम
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      व्योम *paint_तथ्य,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->paint ();
}

अटल व्योम
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        व्योम *paint_तथ्य,
				        hb_color_पंक्ति_t *color_पंक्ति HB_UNUSED,
				        भग्न x0 HB_UNUSED, भग्न y0 HB_UNUSED,
				        भग्न x1 HB_UNUSED, भग्न y1 HB_UNUSED,
				        भग्न x2 HB_UNUSED, भग्न y2 HB_UNUSED,
				        व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->paint ();
}

अटल व्योम
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        व्योम *paint_तथ्य,
				        hb_color_पंक्ति_t *color_पंक्ति HB_UNUSED,
				        भग्न x0 HB_UNUSED, भग्न y0 HB_UNUSED, भग्न r0 HB_UNUSED,
				        भग्न x1 HB_UNUSED, भग्न y1 HB_UNUSED, भग्न r1 HB_UNUSED,
				        व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->paint ();
}

अटल व्योम
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       व्योम *paint_तथ्य,
				       hb_color_पंक्ति_t *color_पंक्ति HB_UNUSED,
				       भग्न cx HB_UNUSED, भग्न cy HB_UNUSED,
				       भग्न start_angle HB_UNUSED,
				       भग्न end_angle HB_UNUSED,
				       व्योम *user_तथ्य HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_तथ्य;

  c->paint ();
}

अटल hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, व्योम *paint_तथ्य,
			     int64_t budget, व्योम *)
{
  स्वतः *context = (hb_paint_extents_context_t *) paint_तथ्य;
  context->budget = budget;
  context->budget_initialized = true;
  context->recharge_budget ();
  वापस true;
}

अटल int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, व्योम *paint_तथ्य, व्योम *)
{
  वापस ((hb_paint_extents_context_t *) paint_तथ्य)->budget;
}

अटल int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, व्योम *paint_तथ्य, व्योम *)
{
  वापस &((hb_paint_extents_context_t *) paint_तथ्य)->budget_remaining;
}

अटल अंतरभूत व्योम free_static_paint_extents_funcs ();

अटल काष्ठा hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  अटल hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_धकेलो_transform_func (funcs, hb_paint_extents_धकेलो_transform, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_transform_func (funcs, hb_paint_extents_निकालो_transform, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_glyph_func (funcs, hb_paint_extents_धकेलो_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_rectangle_func (funcs, hb_paint_extents_धकेलो_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_clip_func (funcs, hb_paint_extents_निकालो_clip, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_group_func (funcs, hb_paint_extents_धकेलो_group, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_group_func (funcs, hb_paint_extents_निकालो_group, nullptr, nullptr);
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

    वापस funcs;
  }
} static_paint_extents_funcs;

अटल अंतरभूत
व्योम free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  वापस static_paint_extents_funcs.get_unconst ();
}


#पूर्ण_अगर
