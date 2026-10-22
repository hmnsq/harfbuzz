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

#ضمن "hb.hh"

#ifndef HB_NO_PAINT

#ضمن "hb-paint-extents.hh"

#ضمن "hb-draw.hh"

#ضمن "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ساكن فراغ
hb_paint_extents_پاڵنان_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 فراغ *paint_data,
				 عائم xx, عائم yx,
				 عائم xy, عائم yy,
				 عائم dx, عائم dy,
				 فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->پاڵنان_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ساكن فراغ
hb_paint_extents_دەرهێنان_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        فراغ *paint_data,
				فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->دەرهێنان_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
بنية hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  منطقي consume_segment ()
  {
    أرجع hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

ساكن فراغ
hb_paint_extents_ژماردن_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *data,
				hb_draw_state_t *st HB_UNUSED,
				عائم to_x, عائم to_y,
				فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.add_point (to_x, to_y);
}

ساكن فراغ
hb_paint_extents_ژماردن_هێڵ_to (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *data,
				hb_draw_state_t *st HB_UNUSED,
				عائم to_x, عائم to_y,
				فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.add_point (to_x, to_y);
}

ساكن فراغ
hb_paint_extents_ژماردن_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *data,
				     hb_draw_state_t *st HB_UNUSED,
				     عائم control_x, عائم control_y,
				     عائم to_x, عائم to_y,
				     فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

ساكن فراغ
hb_paint_extents_ژماردن_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, فراغ *data,
				 hb_draw_state_t *st HB_UNUSED,
				 عائم control1_x, عائم control1_y,
				 عائم control2_x, عائم control2_y,
				 عائم to_x, عائم to_y,
				 فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  إذا (unlikely (!sink->consume_segment ())) أرجع;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

ساكن int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, فراغ *draw_data, فراغ *)
{
  أرجع ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

ساكن مضمن فراغ free_static_paint_extents_draw_funcs ();

ساكن بنية hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_ژماردن_move_to, nullptr, nullptr);
    hb_draw_funcs_set_هێڵ_to_func (funcs, hb_paint_extents_ژماردن_هێڵ_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_ژماردن_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_ژماردن_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    أرجع funcs;
  }
} static_paint_extents_draw_funcs;

ساكن مضمن
فراغ free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ساكن فراغ
hb_paint_extents_پاڵنان_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  فراغ *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  إذا (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->پاڵنان_clip (sink.extents);
}

ساكن فراغ
hb_paint_extents_پاڵنان_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      فراغ *paint_data,
				      عائم xmin, عائم ymin, عائم xmax, عائم ymax,
				      فراغ *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->پاڵنان_clip (extents);
}

ساكن فراغ
hb_paint_extents_دەرهێنان_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   فراغ *paint_data,
			   فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->دەرهێنان_clip ();
}

ساكن فراغ
hb_paint_extents_پاڵنان_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     فراغ *paint_data,
			     فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->پاڵنان_group ();
}

ساكن فراغ
hb_paint_extents_دەرهێنان_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    فراغ *paint_data,
			    hb_paint_composite_mode_t mode,
			    فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->دەرهێنان_group (mode);
}

ساكن hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      فراغ *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned صحيح width HB_UNUSED,
			      unsigned صحيح height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      عائم slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  إذا (!glyph_extents)
    أرجع خطأ; // Happens with SVG images.

  hb_extents_t<> extents = {(عائم) glyph_extents->x_bearing,
			    (عائم) glyph_extents->y_bearing + glyph_extents->height,
			    (عائم) glyph_extents->x_bearing + glyph_extents->width,
			    (عائم) glyph_extents->y_bearing};
  c->پاڵنان_clip (extents);
  c->paint ();
  c->دەرهێنان_clip ();

  أرجع true;
}

ساكن فراغ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      فراغ *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        فراغ *paint_data,
				        hb_color_هێڵ_t *color_هێڵ HB_UNUSED,
				        عائم x0 HB_UNUSED, عائم y0 HB_UNUSED,
				        عائم x1 HB_UNUSED, عائم y1 HB_UNUSED,
				        عائم x2 HB_UNUSED, عائم y2 HB_UNUSED,
				        فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        فراغ *paint_data,
				        hb_color_هێڵ_t *color_هێڵ HB_UNUSED,
				        عائم x0 HB_UNUSED, عائم y0 HB_UNUSED, عائم r0 HB_UNUSED,
				        عائم x1 HB_UNUSED, عائم y1 HB_UNUSED, عائم r1 HB_UNUSED,
				        فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ساكن فراغ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       فراغ *paint_data,
				       hb_color_هێڵ_t *color_هێڵ HB_UNUSED,
				       عائم cx HB_UNUSED, عائم cy HB_UNUSED,
				       عائم start_angle HB_UNUSED,
				       عائم end_angle HB_UNUSED,
				       فراغ *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

ساكن hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, فراغ *paint_data,
			     int64_t budget, فراغ *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = true;
  context->recharge_budget ();
  أرجع true;
}

ساكن int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, فراغ *paint_data, فراغ *)
{
  أرجع ((hb_paint_extents_context_t *) paint_data)->budget;
}

ساكن int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, فراغ *paint_data, فراغ *)
{
  أرجع &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

ساكن مضمن فراغ free_static_paint_extents_funcs ();

ساكن بنية hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_پاڵنان_transform_func (funcs, hb_paint_extents_پاڵنان_transform, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_transform_func (funcs, hb_paint_extents_دەرهێنان_transform, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_glyph_func (funcs, hb_paint_extents_پاڵنان_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_clip_rectangle_func (funcs, hb_paint_extents_پاڵنان_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_clip_func (funcs, hb_paint_extents_دەرهێنان_clip, nullptr, nullptr);
    hb_paint_funcs_set_پاڵنان_group_func (funcs, hb_paint_extents_پاڵنان_group, nullptr, nullptr);
    hb_paint_funcs_set_دەرهێنان_group_func (funcs, hb_paint_extents_دەرهێنان_group, nullptr, nullptr);
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

    أرجع funcs;
  }
} static_paint_extents_funcs;

ساكن مضمن
فراغ free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  أرجع static_paint_extents_funcs.get_unconst ();
}


#endif
