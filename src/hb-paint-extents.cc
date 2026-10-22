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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_PAINT

#ଅନ୍ତର୍ଭାଵ "hb-paint-extents.hh"

#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"

#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଠେଲ_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 ଶୂନ୍ୟ *paint_data,
				 ଦଶମିକ xx, ଦଶମିକ yx,
				 ଦଶମିକ xy, ଦଶମିକ yy,
				 ଦଶମିକ dx, ଦଶମିକ dy,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ଠେଲ_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_କାଢ଼_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        ଶୂନ୍ୟ *paint_data,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->କାଢ଼_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
ସଂରଚନା hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  ତାର୍କିକ consume_segment ()
  {
    ଫେରନ୍ତୁ hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଗଣନା_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ଶୂନ୍ୟ *data,
				hb_draw_state_t *st HB_UNUSED,
				ଦଶମିକ to_x, ଦଶମିକ to_y,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  ଯଦି (unlikely (!sink->consume_segment ())) ଫେରନ୍ତୁ;
  sink->extents.add_point (to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଗଣନା_ଧାଡ଼ି_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ଶୂନ୍ୟ *data,
				hb_draw_state_t *st HB_UNUSED,
				ଦଶମିକ to_x, ଦଶମିକ to_y,
				ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  ଯଦି (unlikely (!sink->consume_segment ())) ଫେରନ୍ତୁ;
  sink->extents.add_point (to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଗଣନା_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ଶୂନ୍ୟ *data,
				     hb_draw_state_t *st HB_UNUSED,
				     ଦଶମିକ control_x, ଦଶମିକ control_y,
				     ଦଶମିକ to_x, ଦଶମିକ to_y,
				     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  ଯଦି (unlikely (!sink->consume_segment ())) ଫେରନ୍ତୁ;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଗଣନା_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ଶୂନ୍ୟ *data,
				 hb_draw_state_t *st HB_UNUSED,
				 ଦଶମିକ control1_x, ଦଶମିକ control1_y,
				 ଦଶମିକ control2_x, ଦଶମିକ control2_y,
				 ଦଶମିକ to_x, ଦଶମିକ to_y,
				 ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  ଯଦି (unlikely (!sink->consume_segment ())) ଫେରନ୍ତୁ;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

ସ୍ଥିର int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, ଶୂନ୍ୟ *draw_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_paint_extents_draw_funcs ();

ସ୍ଥିର ସଂରଚନା hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_ଗଣନା_move_to, nullptr, nullptr);
    hb_draw_funcs_set_ଧାଡ଼ି_to_func (funcs, hb_paint_extents_ଗଣନା_ଧାଡ଼ି_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_ଗଣନା_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_ଗଣନା_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_paint_extents_draw_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଠେଲ_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  ଶୂନ୍ୟ *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  ଯଦି (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ଠେଲ_clip (sink.extents);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଠେଲ_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      ଶୂନ୍ୟ *paint_data,
				      ଦଶମିକ xmin, ଦଶମିକ ymin, ଦଶମିକ xmax, ଦଶମିକ ymax,
				      ଶୂନ୍ୟ *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ଠେଲ_clip (extents);
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_କାଢ଼_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   ଶୂନ୍ୟ *paint_data,
			   ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->କାଢ଼_clip ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_ଠେଲ_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     ଶୂନ୍ୟ *paint_data,
			     ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ଠେଲ_group ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_କାଢ଼_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    ଶୂନ୍ୟ *paint_data,
			    hb_paint_composite_mode_t mode,
			    ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->କାଢ଼_group (mode);
}

ସ୍ଥିର hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      ଶୂନ୍ୟ *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା width HB_UNUSED,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ଦଶମିକ slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  ଯଦି (!glyph_extents)
    ଫେରନ୍ତୁ ମିଥ୍ୟା; // Happens with SVG images.

  hb_extents_t<> extents = {(ଦଶମିକ) glyph_extents->x_bearing,
			    (ଦଶମିକ) glyph_extents->y_bearing + glyph_extents->height,
			    (ଦଶମିକ) glyph_extents->x_bearing + glyph_extents->width,
			    (ଦଶମିକ) glyph_extents->y_bearing};
  c->ଠେଲ_clip (extents);
  c->paint ();
  c->କାଢ଼_clip ();

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      ଶୂନ୍ୟ *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ଶୂନ୍ୟ *paint_data,
				        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				        ଦଶମିକ x0 HB_UNUSED, ଦଶମିକ y0 HB_UNUSED,
				        ଦଶମିକ x1 HB_UNUSED, ଦଶମିକ y1 HB_UNUSED,
				        ଦଶମିକ x2 HB_UNUSED, ଦଶମିକ y2 HB_UNUSED,
				        ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ଶୂନ୍ୟ *paint_data,
				        hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				        ଦଶମିକ x0 HB_UNUSED, ଦଶମିକ y0 HB_UNUSED, ଦଶମିକ r0 HB_UNUSED,
				        ଦଶମିକ x1 HB_UNUSED, ଦଶମିକ y1 HB_UNUSED, ଦଶମିକ r1 HB_UNUSED,
				        ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର ଶୂନ୍ୟ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       ଶୂନ୍ୟ *paint_data,
				       hb_color_ଧାଡ଼ି_t *color_ଧାଡ଼ି HB_UNUSED,
				       ଦଶମିକ cx HB_UNUSED, ଦଶମିକ cy HB_UNUSED,
				       ଦଶମିକ start_angle HB_UNUSED,
				       ଦଶମିକ end_angle HB_UNUSED,
				       ଶୂନ୍ୟ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ସ୍ଥିର hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data,
			     int64_t budget, ଶୂନ୍ୟ *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = ସତ୍ୟ;
  context->recharge_budget ();
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ ((hb_paint_extents_context_t *) paint_data)->budget;
}

ସ୍ଥିର int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, ଶୂନ୍ୟ *paint_data, ଶୂନ୍ୟ *)
{
  ଫେରନ୍ତୁ &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_paint_extents_funcs ();

ସ୍ଥିର ସଂରଚନା hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ଠେଲ_transform_func (funcs, hb_paint_extents_ଠେଲ_transform, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_transform_func (funcs, hb_paint_extents_କାଢ଼_transform, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_glyph_func (funcs, hb_paint_extents_ଠେଲ_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_clip_rectangle_func (funcs, hb_paint_extents_ଠେଲ_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_clip_func (funcs, hb_paint_extents_କାଢ଼_clip, nullptr, nullptr);
    hb_paint_funcs_set_ଠେଲ_group_func (funcs, hb_paint_extents_ଠେଲ_group, nullptr, nullptr);
    hb_paint_funcs_set_କାଢ଼_group_func (funcs, hb_paint_extents_କାଢ଼_group, nullptr, nullptr);
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

    ଫେରନ୍ତୁ funcs;
  }
} static_paint_extents_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  ଫେରନ୍ତୁ static_paint_extents_funcs.get_unconst ();
}


#endif
