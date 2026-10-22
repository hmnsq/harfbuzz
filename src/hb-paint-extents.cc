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

#অন্তর্ভাৱ "hb.hh"

#ifndef HB_NO_PAINT

#অন্তর্ভাৱ "hb-paint-extents.hh"

#অন্তর্ভাৱ "hb-draw.hh"

#অন্তর্ভাৱ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

স্থির শূন্য
hb_paint_extents_ঠেলো_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 শূন্য *paint_data,
				 ভিন্ন xx, ভিন্ন yx,
				 ভিন্ন xy, ভিন্ন yy,
				 ভিন্ন dx, ভিন্ন dy,
				 শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ঠেলো_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

স্থির শূন্য
hb_paint_extents_বেৰ_কৰো_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        শূন্য *paint_data,
				শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->বেৰ_কৰো_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
সংরচনা hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  তার্কিক consume_segment ()
  {
    ঘূৰাই hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

স্থির শূন্য
hb_paint_extents_গুনতি_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *data,
				hb_draw_state_t *st HB_UNUSED,
				ভিন্ন to_x, ভিন্ন to_y,
				শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  যদি (unlikely (!sink->consume_segment ())) ঘূৰাই;
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_সাৰি_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *data,
				hb_draw_state_t *st HB_UNUSED,
				ভিন্ন to_x, ভিন্ন to_y,
				শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  যদি (unlikely (!sink->consume_segment ())) ঘূৰাই;
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *data,
				     hb_draw_state_t *st HB_UNUSED,
				     ভিন্ন control_x, ভিন্ন control_y,
				     ভিন্ন to_x, ভিন্ন to_y,
				     শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  যদি (unlikely (!sink->consume_segment ())) ঘূৰাই;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *data,
				 hb_draw_state_t *st HB_UNUSED,
				 ভিন্ন control1_x, ভিন্ন control1_y,
				 ভিন্ন control2_x, ভিন্ন control2_y,
				 ভিন্ন to_x, ভিন্ন to_y,
				 শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  যদি (unlikely (!sink->consume_segment ())) ঘূৰাই;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

স্থির int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, শূন্য *draw_data, শূন্য *)
{
  ঘূৰাই ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

স্থির অন্তঃস্থ শূন্য free_static_paint_extents_draw_funcs ();

স্থির সংরচনা hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  স্থির hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_গুনতি_move_to, nullptr, nullptr);
    hb_draw_funcs_set_সাৰি_to_func (funcs, hb_paint_extents_গুনতি_সাৰি_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_গুনতি_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_গুনতি_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    ঘূৰাই funcs;
  }
} static_paint_extents_draw_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

স্থির শূন্য
hb_paint_extents_ঠেলো_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  শূন্য *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  যদি (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ঠেলো_clip (sink.extents);
}

স্থির শূন্য
hb_paint_extents_ঠেলো_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      শূন্য *paint_data,
				      ভিন্ন xmin, ভিন্ন ymin, ভিন্ন xmax, ভিন্ন ymax,
				      শূন্য *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ঠেলো_clip (extents);
}

স্থির শূন্য
hb_paint_extents_বেৰ_কৰো_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   শূন্য *paint_data,
			   শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->বেৰ_কৰো_clip ();
}

স্থির শূন্য
hb_paint_extents_ঠেলো_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     শূন্য *paint_data,
			     শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ঠেলো_group ();
}

স্থির শূন্য
hb_paint_extents_বেৰ_কৰো_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    শূন্য *paint_data,
			    hb_paint_composite_mode_t mode,
			    শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->বেৰ_কৰো_group (mode);
}

স্থির hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      শূন্য *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned পূর্ণাঙ্ক width HB_UNUSED,
			      unsigned পূর্ণাঙ্ক height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ভিন্ন slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  যদি (!glyph_extents)
    ঘূৰাই মিছা; // Happens with SVG images.

  hb_extents_t<> extents = {(ভিন্ন) glyph_extents->x_bearing,
			    (ভিন্ন) glyph_extents->y_bearing + glyph_extents->height,
			    (ভিন্ন) glyph_extents->x_bearing + glyph_extents->width,
			    (ভিন্ন) glyph_extents->y_bearing};
  c->ঠেলো_clip (extents);
  c->paint ();
  c->বেৰ_কৰো_clip ();

  ঘূৰাই সঁচা;
}

স্থির শূন্য
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      শূন্য *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        শূন্য *paint_data,
				        hb_color_সাৰি_t *color_সাৰি HB_UNUSED,
				        ভিন্ন x0 HB_UNUSED, ভিন্ন y0 HB_UNUSED,
				        ভিন্ন x1 HB_UNUSED, ভিন্ন y1 HB_UNUSED,
				        ভিন্ন x2 HB_UNUSED, ভিন্ন y2 HB_UNUSED,
				        শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        শূন্য *paint_data,
				        hb_color_সাৰি_t *color_সাৰি HB_UNUSED,
				        ভিন্ন x0 HB_UNUSED, ভিন্ন y0 HB_UNUSED, ভিন্ন r0 HB_UNUSED,
				        ভিন্ন x1 HB_UNUSED, ভিন্ন y1 HB_UNUSED, ভিন্ন r1 HB_UNUSED,
				        শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       শূন্য *paint_data,
				       hb_color_সাৰি_t *color_সাৰি HB_UNUSED,
				       ভিন্ন cx HB_UNUSED, ভিন্ন cy HB_UNUSED,
				       ভিন্ন start_angle HB_UNUSED,
				       ভিন্ন end_angle HB_UNUSED,
				       শূন্য *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

স্থির hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, শূন্য *paint_data,
			     int64_t budget, শূন্য *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = সঁচা;
  context->recharge_budget ();
  ঘূৰাই সঁচা;
}

স্থির int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, শূন্য *paint_data, শূন্য *)
{
  ঘূৰাই ((hb_paint_extents_context_t *) paint_data)->budget;
}

স্থির int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, শূন্য *paint_data, শূন্য *)
{
  ঘূৰাই &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

স্থির অন্তঃস্থ শূন্য free_static_paint_extents_funcs ();

স্থির সংরচনা hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  স্থির hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ঠেলো_transform_func (funcs, hb_paint_extents_ঠেলো_transform, nullptr, nullptr);
    hb_paint_funcs_set_বেৰ_কৰো_transform_func (funcs, hb_paint_extents_বেৰ_কৰো_transform, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_glyph_func (funcs, hb_paint_extents_ঠেলো_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_rectangle_func (funcs, hb_paint_extents_ঠেলো_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_বেৰ_কৰো_clip_func (funcs, hb_paint_extents_বেৰ_কৰো_clip, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_group_func (funcs, hb_paint_extents_ঠেলো_group, nullptr, nullptr);
    hb_paint_funcs_set_বেৰ_কৰো_group_func (funcs, hb_paint_extents_বেৰ_কৰো_group, nullptr, nullptr);
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

    ঘূৰাই funcs;
  }
} static_paint_extents_funcs;

স্থির অন্তঃস্থ
শূন্য free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  ঘূৰাই static_paint_extents_funcs.get_unconst ();
}


#endif
