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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_PAINT

#અન્તર્ભાવ "hb-paint-extents.hh"

#અન્તર્ભાવ "hb-draw.hh"

#અન્તર્ભાવ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

સ્થિર શૂન્ય
hb_paint_extents_ધકેલો_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 શૂન્ય *paint_data,
				 દશાંશ xx, દશાંશ yx,
				 દશાંશ xy, દશાંશ yy,
				 દશાંશ dx, દશાંશ dy,
				 શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ધકેલો_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

સ્થિર શૂન્ય
hb_paint_extents_કાઢો_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        શૂન્ય *paint_data,
				શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->કાઢો_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
સંરચના hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  તાર્કિક consume_segment ()
  {
    પાછા hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

સ્થિર શૂન્ય
hb_paint_extents_ગણતરી_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, શૂન્ય *data,
				hb_draw_state_t *st HB_UNUSED,
				દશાંશ to_x, દશાંશ to_y,
				શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  જો (unlikely (!sink->consume_segment ())) પાછા;
  sink->extents.add_point (to_x, to_y);
}

સ્થિર શૂન્ય
hb_paint_extents_ગણતરી_લીટી_to (hb_draw_funcs_t *dfuncs HB_UNUSED, શૂન્ય *data,
				hb_draw_state_t *st HB_UNUSED,
				દશાંશ to_x, દશાંશ to_y,
				શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  જો (unlikely (!sink->consume_segment ())) પાછા;
  sink->extents.add_point (to_x, to_y);
}

સ્થિર શૂન્ય
hb_paint_extents_ગણતરી_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, શૂન્ય *data,
				     hb_draw_state_t *st HB_UNUSED,
				     દશાંશ control_x, દશાંશ control_y,
				     દશાંશ to_x, દશાંશ to_y,
				     શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  જો (unlikely (!sink->consume_segment ())) પાછા;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

સ્થિર શૂન્ય
hb_paint_extents_ગણતરી_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, શૂન્ય *data,
				 hb_draw_state_t *st HB_UNUSED,
				 દશાંશ control1_x, દશાંશ control1_y,
				 દશાંશ control2_x, દશાંશ control2_y,
				 દશાંશ to_x, દશાંશ to_y,
				 શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  જો (unlikely (!sink->consume_segment ())) પાછા;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

સ્થિર int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, શૂન્ય *draw_data, શૂન્ય *)
{
  પાછા ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

સ્થિર અંતઃરેખ શૂન્ય free_static_paint_extents_draw_funcs ();

સ્થિર સંરચના hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  સ્થિર hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_ગણતરી_move_to, nullptr, nullptr);
    hb_draw_funcs_set_લીટી_to_func (funcs, hb_paint_extents_ગણતરી_લીટી_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_ગણતરી_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_ગણતરી_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    પાછા funcs;
  }
} static_paint_extents_draw_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

સ્થિર શૂન્ય
hb_paint_extents_ધકેલો_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  શૂન્ય *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  જો (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ધકેલો_clip (sink.extents);
}

સ્થિર શૂન્ય
hb_paint_extents_ધકેલો_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      શૂન્ય *paint_data,
				      દશાંશ xmin, દશાંશ ymin, દશાંશ xmax, દશાંશ ymax,
				      શૂન્ય *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ધકેલો_clip (extents);
}

સ્થિર શૂન્ય
hb_paint_extents_કાઢો_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   શૂન્ય *paint_data,
			   શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->કાઢો_clip ();
}

સ્થિર શૂન્ય
hb_paint_extents_ધકેલો_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     શૂન્ય *paint_data,
			     શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->ધકેલો_group ();
}

સ્થિર શૂન્ય
hb_paint_extents_કાઢો_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    શૂન્ય *paint_data,
			    hb_paint_composite_mode_t mode,
			    શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->કાઢો_group (mode);
}

સ્થિર hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      શૂન્ય *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned પૂર્ણાંક width HB_UNUSED,
			      unsigned પૂર્ણાંક height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      દશાંશ slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  જો (!glyph_extents)
    પાછા ખોટું; // Happens with SVG images.

  hb_extents_t<> extents = {(દશાંશ) glyph_extents->x_bearing,
			    (દશાંશ) glyph_extents->y_bearing + glyph_extents->height,
			    (દશાંશ) glyph_extents->x_bearing + glyph_extents->width,
			    (દશાંશ) glyph_extents->y_bearing};
  c->ધકેલો_clip (extents);
  c->paint ();
  c->કાઢો_clip ();

  પાછા સાચું;
}

સ્થિર શૂન્ય
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      શૂન્ય *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        શૂન્ય *paint_data,
				        hb_color_લીટી_t *color_લીટી HB_UNUSED,
				        દશાંશ x0 HB_UNUSED, દશાંશ y0 HB_UNUSED,
				        દશાંશ x1 HB_UNUSED, દશાંશ y1 HB_UNUSED,
				        દશાંશ x2 HB_UNUSED, દશાંશ y2 HB_UNUSED,
				        શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        શૂન્ય *paint_data,
				        hb_color_લીટી_t *color_લીટી HB_UNUSED,
				        દશાંશ x0 HB_UNUSED, દશાંશ y0 HB_UNUSED, દશાંશ r0 HB_UNUSED,
				        દશાંશ x1 HB_UNUSED, દશાંશ y1 HB_UNUSED, દશાંશ r1 HB_UNUSED,
				        શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

સ્થિર શૂન્ય
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       શૂન્ય *paint_data,
				       hb_color_લીટી_t *color_લીટી HB_UNUSED,
				       દશાંશ cx HB_UNUSED, દશાંશ cy HB_UNUSED,
				       દશાંશ start_angle HB_UNUSED,
				       દશાંશ end_angle HB_UNUSED,
				       શૂન્ય *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

સ્થિર hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, શૂન્ય *paint_data,
			     int64_t budget, શૂન્ય *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = સાચું;
  context->recharge_budget ();
  પાછા સાચું;
}

સ્થિર int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, શૂન્ય *paint_data, શૂન્ય *)
{
  પાછા ((hb_paint_extents_context_t *) paint_data)->budget;
}

સ્થિર int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, શૂન્ય *paint_data, શૂન્ય *)
{
  પાછા &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

સ્થિર અંતઃરેખ શૂન્ય free_static_paint_extents_funcs ();

સ્થિર સંરચના hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  સ્થિર hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ધકેલો_transform_func (funcs, hb_paint_extents_ધકેલો_transform, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_transform_func (funcs, hb_paint_extents_કાઢો_transform, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_glyph_func (funcs, hb_paint_extents_ધકેલો_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_clip_rectangle_func (funcs, hb_paint_extents_ધકેલો_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_clip_func (funcs, hb_paint_extents_કાઢો_clip, nullptr, nullptr);
    hb_paint_funcs_set_ધકેલો_group_func (funcs, hb_paint_extents_ધકેલો_group, nullptr, nullptr);
    hb_paint_funcs_set_કાઢો_group_func (funcs, hb_paint_extents_કાઢો_group, nullptr, nullptr);
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

    પાછા funcs;
  }
} static_paint_extents_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  પાછા static_paint_extents_funcs.get_unconst ();
}


#endif
