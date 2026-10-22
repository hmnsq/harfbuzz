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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_PAINT

#அந்தர்பாவ "hb-paint-extents.hh"

#அந்தர்பாவ "hb-draw.hh"

#அந்தர்பாவ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

நிலையான வெறுமை
hb_paint_extents_தள்ளு_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 வெறுமை *paint_data,
				 தசம xx, தசம yx,
				 தசம xy, தசம yy,
				 தசம dx, தசம dy,
				 வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->தள்ளு_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

நிலையான வெறுமை
hb_paint_extents_எடு_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        வெறுமை *paint_data,
				வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->எடு_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
கட்டமைப்பு hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  தருக்க consume_segment ()
  {
    திரும்பு hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

நிலையான வெறுமை
hb_paint_extents_மொத்தம்_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, வெறுமை *data,
				hb_draw_state_t *st HB_UNUSED,
				தசம to_x, தசம to_y,
				வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  எனில் (unlikely (!sink->consume_segment ())) திரும்பு;
  sink->extents.add_point (to_x, to_y);
}

நிலையான வெறுமை
hb_paint_extents_மொத்தம்_வரி_to (hb_draw_funcs_t *dfuncs HB_UNUSED, வெறுமை *data,
				hb_draw_state_t *st HB_UNUSED,
				தசம to_x, தசம to_y,
				வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  எனில் (unlikely (!sink->consume_segment ())) திரும்பு;
  sink->extents.add_point (to_x, to_y);
}

நிலையான வெறுமை
hb_paint_extents_மொத்தம்_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, வெறுமை *data,
				     hb_draw_state_t *st HB_UNUSED,
				     தசம control_x, தசம control_y,
				     தசம to_x, தசம to_y,
				     வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  எனில் (unlikely (!sink->consume_segment ())) திரும்பு;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

நிலையான வெறுமை
hb_paint_extents_மொத்தம்_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, வெறுமை *data,
				 hb_draw_state_t *st HB_UNUSED,
				 தசம control1_x, தசம control1_y,
				 தசம control2_x, தசம control2_y,
				 தசம to_x, தசம to_y,
				 வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  எனில் (unlikely (!sink->consume_segment ())) திரும்பு;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

நிலையான int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, வெறுமை *draw_data, வெறுமை *)
{
  திரும்பு ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

நிலையான உள்வரி வெறுமை free_static_paint_extents_draw_funcs ();

நிலையான கட்டமைப்பு hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  நிலையான hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_மொத்தம்_move_to, nullptr, nullptr);
    hb_draw_funcs_set_வரி_to_func (funcs, hb_paint_extents_மொத்தம்_வரி_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_மொத்தம்_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_மொத்தம்_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    திரும்பு funcs;
  }
} static_paint_extents_draw_funcs;

நிலையான உள்வரி
வெறுமை free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

நிலையான வெறுமை
hb_paint_extents_தள்ளு_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  வெறுமை *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  எனில் (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->தள்ளு_clip (sink.extents);
}

நிலையான வெறுமை
hb_paint_extents_தள்ளு_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      வெறுமை *paint_data,
				      தசம xmin, தசம ymin, தசம xmax, தசம ymax,
				      வெறுமை *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->தள்ளு_clip (extents);
}

நிலையான வெறுமை
hb_paint_extents_எடு_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   வெறுமை *paint_data,
			   வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->எடு_clip ();
}

நிலையான வெறுமை
hb_paint_extents_தள்ளு_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     வெறுமை *paint_data,
			     வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->தள்ளு_group ();
}

நிலையான வெறுமை
hb_paint_extents_எடு_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    வெறுமை *paint_data,
			    hb_paint_composite_mode_t mode,
			    வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->எடு_group (mode);
}

நிலையான hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      வெறுமை *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned முழு width HB_UNUSED,
			      unsigned முழு height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      தசம slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  எனில் (!glyph_extents)
    திரும்பு பொய்; // Happens with SVG images.

  hb_extents_t<> extents = {(தசம) glyph_extents->x_bearing,
			    (தசம) glyph_extents->y_bearing + glyph_extents->height,
			    (தசம) glyph_extents->x_bearing + glyph_extents->width,
			    (தசம) glyph_extents->y_bearing};
  c->தள்ளு_clip (extents);
  c->paint ();
  c->எடு_clip ();

  திரும்பு உண்மை;
}

நிலையான வெறுமை
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      வெறுமை *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        வெறுமை *paint_data,
				        hb_color_வரி_t *color_வரி HB_UNUSED,
				        தசம x0 HB_UNUSED, தசம y0 HB_UNUSED,
				        தசம x1 HB_UNUSED, தசம y1 HB_UNUSED,
				        தசம x2 HB_UNUSED, தசம y2 HB_UNUSED,
				        வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        வெறுமை *paint_data,
				        hb_color_வரி_t *color_வரி HB_UNUSED,
				        தசம x0 HB_UNUSED, தசம y0 HB_UNUSED, தசம r0 HB_UNUSED,
				        தசம x1 HB_UNUSED, தசம y1 HB_UNUSED, தசம r1 HB_UNUSED,
				        வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

நிலையான வெறுமை
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       வெறுமை *paint_data,
				       hb_color_வரி_t *color_வரி HB_UNUSED,
				       தசம cx HB_UNUSED, தசம cy HB_UNUSED,
				       தசம start_angle HB_UNUSED,
				       தசம end_angle HB_UNUSED,
				       வெறுமை *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

நிலையான hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, வெறுமை *paint_data,
			     int64_t budget, வெறுமை *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = உண்மை;
  context->recharge_budget ();
  திரும்பு உண்மை;
}

நிலையான int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, வெறுமை *paint_data, வெறுமை *)
{
  திரும்பு ((hb_paint_extents_context_t *) paint_data)->budget;
}

நிலையான int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, வெறுமை *paint_data, வெறுமை *)
{
  திரும்பு &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

நிலையான உள்வரி வெறுமை free_static_paint_extents_funcs ();

நிலையான கட்டமைப்பு hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  நிலையான hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_தள்ளு_transform_func (funcs, hb_paint_extents_தள்ளு_transform, nullptr, nullptr);
    hb_paint_funcs_set_எடு_transform_func (funcs, hb_paint_extents_எடு_transform, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_glyph_func (funcs, hb_paint_extents_தள்ளு_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_rectangle_func (funcs, hb_paint_extents_தள்ளு_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_எடு_clip_func (funcs, hb_paint_extents_எடு_clip, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_group_func (funcs, hb_paint_extents_தள்ளு_group, nullptr, nullptr);
    hb_paint_funcs_set_எடு_group_func (funcs, hb_paint_extents_எடு_group, nullptr, nullptr);
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

    திரும்பு funcs;
  }
} static_paint_extents_funcs;

நிலையான உள்வரி
வெறுமை free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  திரும்பு static_paint_extents_funcs.get_unconst ();
}


#endif
