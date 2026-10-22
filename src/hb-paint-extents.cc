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

#شامل "hb.hh"

#ifndef HB_NO_PAINT

#شامل "hb-paint-extents.hh"

#شامل "hb-draw.hh"

#شامل "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ایستا تهی
hb_paint_extents_افزودن_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 تهی *paint_data,
				 اعشاری xx, اعشاری yx,
				 اعشاری xy, اعشاری yy,
				 اعشاری dx, اعشاری dy,
				 تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->افزودن_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ایستا تهی
hb_paint_extents_برداشت_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        تهی *paint_data,
				تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->برداشت_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
ساختار hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  منطقی consume_segment ()
  {
    بازگشت hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

ایستا تهی
hb_paint_extents_شمار_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, تهی *data,
				hb_draw_state_t *st HB_UNUSED,
				اعشاری to_x, اعشاری to_y,
				تهی *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  اگر (unlikely (!sink->consume_segment ())) بازگشت;
  sink->extents.add_point (to_x, to_y);
}

ایستا تهی
hb_paint_extents_شمار_سطر_to (hb_draw_funcs_t *dfuncs HB_UNUSED, تهی *data,
				hb_draw_state_t *st HB_UNUSED,
				اعشاری to_x, اعشاری to_y,
				تهی *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  اگر (unlikely (!sink->consume_segment ())) بازگشت;
  sink->extents.add_point (to_x, to_y);
}

ایستا تهی
hb_paint_extents_شمار_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, تهی *data,
				     hb_draw_state_t *st HB_UNUSED,
				     اعشاری control_x, اعشاری control_y,
				     اعشاری to_x, اعشاری to_y,
				     تهی *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  اگر (unlikely (!sink->consume_segment ())) بازگشت;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

ایستا تهی
hb_paint_extents_شمار_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, تهی *data,
				 hb_draw_state_t *st HB_UNUSED,
				 اعشاری control1_x, اعشاری control1_y,
				 اعشاری control2_x, اعشاری control2_y,
				 اعشاری to_x, اعشاری to_y,
				 تهی *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  اگر (unlikely (!sink->consume_segment ())) بازگشت;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

ایستا int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, تهی *draw_data, تهی *)
{
  بازگشت ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

ایستا درونخطی تهی free_static_paint_extents_draw_funcs ();

ایستا ساختار hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ایستا hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_شمار_move_to, nullptr, nullptr);
    hb_draw_funcs_set_سطر_to_func (funcs, hb_paint_extents_شمار_سطر_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_شمار_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_شمار_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    بازگشت funcs;
  }
} static_paint_extents_draw_funcs;

ایستا درونخطی
تهی free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ایستا تهی
hb_paint_extents_افزودن_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  تهی *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  اگر (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->افزودن_clip (sink.extents);
}

ایستا تهی
hb_paint_extents_افزودن_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      تهی *paint_data,
				      اعشاری xmin, اعشاری ymin, اعشاری xmax, اعشاری ymax,
				      تهی *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->افزودن_clip (extents);
}

ایستا تهی
hb_paint_extents_برداشت_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   تهی *paint_data,
			   تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->برداشت_clip ();
}

ایستا تهی
hb_paint_extents_افزودن_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     تهی *paint_data,
			     تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->افزودن_group ();
}

ایستا تهی
hb_paint_extents_برداشت_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    تهی *paint_data,
			    hb_paint_composite_mode_t mode,
			    تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->برداشت_group (mode);
}

ایستا hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      تهی *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned صحیح width HB_UNUSED,
			      unsigned صحیح height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      اعشاری slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  اگر (!glyph_extents)
    بازگشت نادرست; // Happens with SVG images.

  hb_extents_t<> extents = {(اعشاری) glyph_extents->x_bearing,
			    (اعشاری) glyph_extents->y_bearing + glyph_extents->height,
			    (اعشاری) glyph_extents->x_bearing + glyph_extents->width,
			    (اعشاری) glyph_extents->y_bearing};
  c->افزودن_clip (extents);
  c->paint ();
  c->برداشت_clip ();

  بازگشت درست;
}

ایستا تهی
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      تهی *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        تهی *paint_data,
				        hb_color_سطر_t *color_سطر HB_UNUSED,
				        اعشاری x0 HB_UNUSED, اعشاری y0 HB_UNUSED,
				        اعشاری x1 HB_UNUSED, اعشاری y1 HB_UNUSED,
				        اعشاری x2 HB_UNUSED, اعشاری y2 HB_UNUSED,
				        تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        تهی *paint_data,
				        hb_color_سطر_t *color_سطر HB_UNUSED,
				        اعشاری x0 HB_UNUSED, اعشاری y0 HB_UNUSED, اعشاری r0 HB_UNUSED,
				        اعشاری x1 HB_UNUSED, اعشاری y1 HB_UNUSED, اعشاری r1 HB_UNUSED,
				        تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ایستا تهی
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       تهی *paint_data,
				       hb_color_سطر_t *color_سطر HB_UNUSED,
				       اعشاری cx HB_UNUSED, اعشاری cy HB_UNUSED,
				       اعشاری start_angle HB_UNUSED,
				       اعشاری end_angle HB_UNUSED,
				       تهی *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ایستا hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, تهی *paint_data,
			     int64_t budget, تهی *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = درست;
  context->recharge_budget ();
  بازگشت درست;
}

ایستا int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, تهی *paint_data, تهی *)
{
  بازگشت ((hb_paint_extents_context_t *) paint_data)->budget;
}

ایستا int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, تهی *paint_data, تهی *)
{
  بازگشت &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

ایستا درونخطی تهی free_static_paint_extents_funcs ();

ایستا ساختار hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ایستا hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_افزودن_transform_func (funcs, hb_paint_extents_افزودن_transform, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_transform_func (funcs, hb_paint_extents_برداشت_transform, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_glyph_func (funcs, hb_paint_extents_افزودن_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_clip_rectangle_func (funcs, hb_paint_extents_افزودن_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_clip_func (funcs, hb_paint_extents_برداشت_clip, nullptr, nullptr);
    hb_paint_funcs_set_افزودن_group_func (funcs, hb_paint_extents_افزودن_group, nullptr, nullptr);
    hb_paint_funcs_set_برداشت_group_func (funcs, hb_paint_extents_برداشت_group, nullptr, nullptr);
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

    بازگشت funcs;
  }
} static_paint_extents_funcs;

ایستا درونخطی
تهی free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  بازگشت static_paint_extents_funcs.get_unconst ();
}


#endif
