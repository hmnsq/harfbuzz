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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_PAINT

#অন্তর্ভাব "hb-paint-extents.hh"

#অন্তর্ভাব "hb-draw.hh"

#অন্তর্ভাব "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

স্থির শূন্য
hb_paint_extents_ঠেলো_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 শূন্য *paint_তথ্য,
				 দশমিক xx, দশমিক yx,
				 দশমিক xy, দশমিক yy,
				 দশমিক dx, দশমিক dy,
				 শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->ঠেলো_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

স্থির শূন্য
hb_paint_extents_বের_করো_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        শূন্য *paint_তথ্য,
				শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->বের_করো_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
কাঠামো hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  যৌক্তিক consume_segment ()
  {
    ফেরত hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

স্থির শূন্য
hb_paint_extents_গুনতি_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *তথ্য,
				hb_draw_state_t *st HB_UNUSED,
				দশমিক to_x, দশমিক to_y,
				শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) তথ্য;
  যদি (unlikely (!sink->consume_segment ())) ফেরত;
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_সারি_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *তথ্য,
				hb_draw_state_t *st HB_UNUSED,
				দশমিক to_x, দশমিক to_y,
				শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) তথ্য;
  যদি (unlikely (!sink->consume_segment ())) ফেরত;
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *তথ্য,
				     hb_draw_state_t *st HB_UNUSED,
				     দশমিক control_x, দশমিক control_y,
				     দশমিক to_x, দশমিক to_y,
				     শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) তথ্য;
  যদি (unlikely (!sink->consume_segment ())) ফেরত;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

স্থির শূন্য
hb_paint_extents_গুনতি_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, শূন্য *তথ্য,
				 hb_draw_state_t *st HB_UNUSED,
				 দশমিক control1_x, দশমিক control1_y,
				 দশমিক control2_x, দশমিক control2_y,
				 দশমিক to_x, দশমিক to_y,
				 শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) তথ্য;
  যদি (unlikely (!sink->consume_segment ())) ফেরত;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

স্থির int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, শূন্য *draw_তথ্য, শূন্য *)
{
  ফেরত ((hb_paint_extents_sink_t *) draw_তথ্য)->budget_remaining;
}

স্থির অন্তঃরেখ শূন্য free_static_paint_extents_draw_funcs ();

স্থির কাঠামো hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  স্থির hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_গুনতি_move_to, nullptr, nullptr);
    hb_draw_funcs_set_সারি_to_func (funcs, hb_paint_extents_গুনতি_সারি_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_গুনতি_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_গুনতি_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    ফেরত funcs;
  }
} static_paint_extents_draw_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

স্থির শূন্য
hb_paint_extents_ঠেলো_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  শূন্য *paint_তথ্য,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  যদি (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ঠেলো_clip (sink.extents);
}

স্থির শূন্য
hb_paint_extents_ঠেলো_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      শূন্য *paint_তথ্য,
				      দশমিক xmin, দশমিক ymin, দশমিক xmax, দশমিক ymax,
				      শূন্য *user_তথ্য)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ঠেলো_clip (extents);
}

স্থির শূন্য
hb_paint_extents_বের_করো_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   শূন্য *paint_তথ্য,
			   শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->বের_করো_clip ();
}

স্থির শূন্য
hb_paint_extents_ঠেলো_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     শূন্য *paint_তথ্য,
			     শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->ঠেলো_group ();
}

স্থির শূন্য
hb_paint_extents_বের_করো_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    শূন্য *paint_তথ্য,
			    hb_paint_composite_mode_t mode,
			    শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->বের_করো_group (mode);
}

স্থির hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      শূন্য *paint_তথ্য,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned পূর্ণসংখ্যা width HB_UNUSED,
			      unsigned পূর্ণসংখ্যা height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      দশমিক slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  যদি (!glyph_extents)
    ফেরত মিথ্যা; // Happens with SVG images.

  hb_extents_t<> extents = {(দশমিক) glyph_extents->x_bearing,
			    (দশমিক) glyph_extents->y_bearing + glyph_extents->height,
			    (দশমিক) glyph_extents->x_bearing + glyph_extents->width,
			    (দশমিক) glyph_extents->y_bearing};
  c->ঠেলো_clip (extents);
  c->paint ();
  c->বের_করো_clip ();

  ফেরত সত্য;
}

স্থির শূন্য
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      শূন্য *paint_তথ্য,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        শূন্য *paint_তথ্য,
				        hb_color_সারি_t *color_সারি HB_UNUSED,
				        দশমিক x0 HB_UNUSED, দশমিক y0 HB_UNUSED,
				        দশমিক x1 HB_UNUSED, দশমিক y1 HB_UNUSED,
				        দশমিক x2 HB_UNUSED, দশমিক y2 HB_UNUSED,
				        শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        শূন্য *paint_তথ্য,
				        hb_color_সারি_t *color_সারি HB_UNUSED,
				        দশমিক x0 HB_UNUSED, দশমিক y0 HB_UNUSED, দশমিক r0 HB_UNUSED,
				        দশমিক x1 HB_UNUSED, দশমিক y1 HB_UNUSED, দশমিক r1 HB_UNUSED,
				        শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->paint ();
}

স্থির শূন্য
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       শূন্য *paint_তথ্য,
				       hb_color_সারি_t *color_সারি HB_UNUSED,
				       দশমিক cx HB_UNUSED, দশমিক cy HB_UNUSED,
				       দশমিক start_angle HB_UNUSED,
				       দশমিক end_angle HB_UNUSED,
				       শূন্য *user_তথ্য HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_তথ্য;

  c->paint ();
}

স্থির hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, শূন্য *paint_তথ্য,
			     int64_t budget, শূন্য *)
{
  auto *context = (hb_paint_extents_context_t *) paint_তথ্য;
  context->budget = budget;
  context->budget_initialized = সত্য;
  context->recharge_budget ();
  ফেরত সত্য;
}

স্থির int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, শূন্য *paint_তথ্য, শূন্য *)
{
  ফেরত ((hb_paint_extents_context_t *) paint_তথ্য)->budget;
}

স্থির int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, শূন্য *paint_তথ্য, শূন্য *)
{
  ফেরত &((hb_paint_extents_context_t *) paint_তথ্য)->budget_remaining;
}

স্থির অন্তঃরেখ শূন্য free_static_paint_extents_funcs ();

স্থির কাঠামো hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  স্থির hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ঠেলো_transform_func (funcs, hb_paint_extents_ঠেলো_transform, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_transform_func (funcs, hb_paint_extents_বের_করো_transform, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_glyph_func (funcs, hb_paint_extents_ঠেলো_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_rectangle_func (funcs, hb_paint_extents_ঠেলো_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_clip_func (funcs, hb_paint_extents_বের_করো_clip, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_group_func (funcs, hb_paint_extents_ঠেলো_group, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_group_func (funcs, hb_paint_extents_বের_করো_group, nullptr, nullptr);
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

    ফেরত funcs;
  }
} static_paint_extents_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  ফেরত static_paint_extents_funcs.get_unconst ();
}


#endif
