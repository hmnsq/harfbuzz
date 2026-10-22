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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_PAINT

#അന്തര്ഭാവ "hb-paint-extents.hh"

#അന്തര്ഭാവ "hb-draw.hh"

#അന്തര്ഭാവ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

സ്ഥിതം ശൂന്യം
hb_paint_extents_തള്ളുക_transform (hb_paint_funcs_t *funcs HB_UNUSED,
				 ശൂന്യം *paint_data,
				 ദശാംശം xx, ദശാംശം yx,
				 ദശാംശം xy, ദശാംശം yy,
				 ദശാംശം dx, ദശാംശം dy,
				 ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->തള്ളുക_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എടുക്കുക_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			        ശൂന്യം *paint_data,
				ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->എടുക്കുക_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
ഘടന hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_remaining;

  തർക്കം consume_segment ()
  {
    തിരികെ hb_budget_spend (*budget_remaining, HB_BUDGET_1);
  }
};

സ്ഥിതം ശൂന്യം
hb_paint_extents_എണ്ണം_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ശൂന്യം *data,
				hb_draw_state_t *st HB_UNUSED,
				ദശാംശം to_x, ദശാംശം to_y,
				ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  എങ്കിൽ (unlikely (!sink->consume_segment ())) തിരികെ;
  sink->extents.add_point (to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എണ്ണം_വരി_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ശൂന്യം *data,
				hb_draw_state_t *st HB_UNUSED,
				ദശാംശം to_x, ദശാംശം to_y,
				ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  എങ്കിൽ (unlikely (!sink->consume_segment ())) തിരികെ;
  sink->extents.add_point (to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എണ്ണം_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ശൂന്യം *data,
				     hb_draw_state_t *st HB_UNUSED,
				     ദശാംശം control_x, ദശാംശം control_y,
				     ദശാംശം to_x, ദശാംശം to_y,
				     ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  എങ്കിൽ (unlikely (!sink->consume_segment ())) തിരികെ;
  sink->extents.add_point (control_x, control_y);
  sink->extents.add_point (to_x, to_y);
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എണ്ണം_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED, ശൂന്യം *data,
				 hb_draw_state_t *st HB_UNUSED,
				 ദശാംശം control1_x, ദശാംശം control1_y,
				 ദശാംശം control2_x, ദശാംശം control2_y,
				 ദശാംശം to_x, ദശാംശം to_y,
				 ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) data;
  എങ്കിൽ (unlikely (!sink->consume_segment ())) തിരികെ;
  sink->extents.add_point (control1_x, control1_y);
  sink->extents.add_point (control2_x, control2_y);
  sink->extents.add_point (to_x, to_y);
}

സ്ഥിതം int64_t *
hb_paint_extents_get_budget_remaining (hb_draw_funcs_t *, ശൂന്യം *draw_data, ശൂന്യം *)
{
  തിരികെ ((hb_paint_extents_sink_t *) draw_data)->budget_remaining;
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_paint_extents_draw_funcs ();

സ്ഥിതം ഘടന hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func (funcs, hb_paint_extents_എണ്ണം_move_to, nullptr, nullptr);
    hb_draw_funcs_set_വരി_to_func (funcs, hb_paint_extents_എണ്ണം_വരി_to, nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_paint_extents_എണ്ണം_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func (funcs, hb_paint_extents_എണ്ണം_cubic_to, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_paint_extents_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    തിരികെ funcs;
  }
} static_paint_extents_draw_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_തള്ളുക_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
				  ശൂന്യം *paint_data,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_paint_extents_sink_t sink {{}, &c->budget_remaining};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  എങ്കിൽ (likely (c->budget_remaining >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->തള്ളുക_clip (sink.extents);
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_തള്ളുക_clip_rectangle (hb_paint_funcs_t *funcs HB_UNUSED,
				      ശൂന്യം *paint_data,
				      ദശാംശം xmin, ദശാംശം ymin, ദശാംശം xmax, ദശാംശം ymax,
				      ശൂന്യം *user_data)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->തള്ളുക_clip (extents);
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എടുക്കുക_clip (hb_paint_funcs_t *funcs HB_UNUSED,
			   ശൂന്യം *paint_data,
			   ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->എടുക്കുക_clip ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_തള്ളുക_group (hb_paint_funcs_t *funcs HB_UNUSED,
			     ശൂന്യം *paint_data,
			     ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->തള്ളുക_group ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_എടുക്കുക_group (hb_paint_funcs_t *funcs HB_UNUSED,
			    ശൂന്യം *paint_data,
			    hb_paint_composite_mode_t mode,
			    ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->എടുക്കുക_group (mode);
}

സ്ഥിതം hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_UNUSED,
			      ശൂന്യം *paint_data,
			      hb_blob_t *blob HB_UNUSED,
			      unsigned പൂർണ്ണസംഖ്യ width HB_UNUSED,
			      unsigned പൂർണ്ണസംഖ്യ height HB_UNUSED,
			      hb_tag_t format HB_UNUSED,
			      ദശാംശം slant HB_UNUSED,
			      hb_glyph_extents_t *glyph_extents,
			      ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  എങ്കിൽ (!glyph_extents)
    തിരികെ അസത്യം; // Happens with SVG images.

  hb_extents_t<> extents = {(ദശാംശം) glyph_extents->x_bearing,
			    (ദശാംശം) glyph_extents->y_bearing + glyph_extents->height,
			    (ദശാംശം) glyph_extents->x_bearing + glyph_extents->width,
			    (ദശാംശം) glyph_extents->y_bearing};
  c->തള്ളുക_clip (extents);
  c->paint ();
  c->എടുക്കുക_clip ();

  തിരികെ സത്യം;
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
			      ശൂന്യം *paint_data,
			      hb_bool_t use_foreground HB_UNUSED,
			      hb_color_t color HB_UNUSED,
			      ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ശൂന്യം *paint_data,
				        hb_color_വരി_t *color_വരി HB_UNUSED,
				        ദശാംശം x0 HB_UNUSED, ദശാംശം y0 HB_UNUSED,
				        ദശാംശം x1 HB_UNUSED, ദശാംശം y1 HB_UNUSED,
				        ദശാംശം x2 HB_UNUSED, ദശാംശം y2 HB_UNUSED,
				        ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				        ശൂന്യം *paint_data,
				        hb_color_വരി_t *color_വരി HB_UNUSED,
				        ദശാംശം x0 HB_UNUSED, ദശാംശം y0 HB_UNUSED, ദശാംശം r0 HB_UNUSED,
				        ദശാംശം x1 HB_UNUSED, ദശാംശം y1 HB_UNUSED, ദശാംശം r1 HB_UNUSED,
				        ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം ശൂന്യം
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
				       ശൂന്യം *paint_data,
				       hb_color_വരി_t *color_വരി HB_UNUSED,
				       ദശാംശം cx HB_UNUSED, ദശാംശം cy HB_UNUSED,
				       ദശാംശം start_angle HB_UNUSED,
				       ദശാംശം end_angle HB_UNUSED,
				       ശൂന്യം *user_data HB_UNUSED)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_data;

  c->paint ();
}

സ്ഥിതം hb_bool_t
hb_paint_extents_set_budget (hb_paint_funcs_t *, ശൂന്യം *paint_data,
			     int64_t budget, ശൂന്യം *)
{
  auto *context = (hb_paint_extents_context_t *) paint_data;
  context->budget = budget;
  context->budget_initialized = സത്യം;
  context->recharge_budget ();
  തിരികെ സത്യം;
}

സ്ഥിതം int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, ശൂന്യം *paint_data, ശൂന്യം *)
{
  തിരികെ ((hb_paint_extents_context_t *) paint_data)->budget;
}

സ്ഥിതം int64_t *
hb_paint_extents_get_budget_remaining (hb_paint_funcs_t *, ശൂന്യം *paint_data, ശൂന്യം *)
{
  തിരികെ &((hb_paint_extents_context_t *) paint_data)->budget_remaining;
}

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_paint_extents_funcs ();

സ്ഥിതം ഘടന hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_തള്ളുക_transform_func (funcs, hb_paint_extents_തള്ളുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_transform_func (funcs, hb_paint_extents_എടുക്കുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_glyph_func (funcs, hb_paint_extents_തള്ളുക_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_rectangle_func (funcs, hb_paint_extents_തള്ളുക_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_clip_func (funcs, hb_paint_extents_എടുക്കുക_clip, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_group_func (funcs, hb_paint_extents_തള്ളുക_group, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_group_func (funcs, hb_paint_extents_എടുക്കുക_group, nullptr, nullptr);
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

    തിരികെ funcs;
  }
} static_paint_extents_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  തിരികെ static_paint_extents_funcs.get_unconst ();
}


#endif
