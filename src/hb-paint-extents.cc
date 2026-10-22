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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_PAINT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint-extents.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


/*
 * This file implements bounds-extraction computation of COLRv1 fonts as described in:
 *
 * https://learn.microsoft.com/en-us/typography/opentype/spec/colr#glyph-metrics-and-boundedness
 */

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਧੱਕੋ_transform (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
				 ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
				 ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_transform (hb_transform_t<> {xx, yx, xy, yy, dx, dy});
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਕੱਢੋ_transform (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			        ਖਾਲੀ *paint_ਅੰਕੜੇ,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->ਕੱਢੋ_transform ();
}

/* Like the hb_draw_extents_get_funcs() sink, but tied to the paint
 * session's live work budget. */
ਬਣਤਰ hb_paint_extents_sink_t
{
  hb_extents_t<> extents;
  int64_t *budget_ਬਚਿਆ;

  ਤਾਰਕਿਕ consume_segment ()
  {
    ਵਾਪਸ hb_budget_spend (*budget_ਬਚਿਆ, HB_BUDGET_1);
  }
};

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਗਿਣਤੀ_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ,
				hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
				ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ਅੰਕੜੇ;
  ਜੇ (unlikely (!sink->consume_segment ())) ਵਾਪਸ;
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਗਿਣਤੀ_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ,
				hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
				ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ਅੰਕੜੇ;
  ਜੇ (unlikely (!sink->consume_segment ())) ਵਾਪਸ;
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਗਿਣਤੀ_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ,
				     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
				     ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
				     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ਅੰਕੜੇ;
  ਜੇ (unlikely (!sink->consume_segment ())) ਵਾਪਸ;
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (control_x, control_y);
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਗਿਣਤੀ_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ, ਖਾਲੀ *ਅੰਕੜੇ,
				 hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
				 ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
				 ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
				 ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
				 ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_sink_t *sink = (hb_paint_extents_sink_t *) ਅੰਕੜੇ;
  ਜੇ (unlikely (!sink->consume_segment ())) ਵਾਪਸ;
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (control1_x, control1_y);
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (control2_x, control2_y);
  sink->extents.ਜੋੜੋ_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ int64_t *
hb_paint_extents_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_paint_extents_sink_t *) draw_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_paint_extents_draw_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_paint_extents_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_paint_extents_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, hb_paint_extents_ਗਿਣਤੀ_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, hb_paint_extents_ਗਿਣਤੀ_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_paint_extents_ਗਿਣਤੀ_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, hb_paint_extents_ਗਿਣਤੀ_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_paint_extents_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_draw_funcs);

    ਵਾਪਸ funcs;
  }
} static_paint_extents_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_paint_extents_draw_funcs ()
{
  static_paint_extents_draw_funcs.free_instance ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				  ਖਾਲੀ *paint_ਅੰਕੜੇ,
				  hb_codepoint_t glyph,
				  hb_font_t *font,
				  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  hb_paint_extents_sink_t sink {{}, &c->budget_ਬਚਿਆ};
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip clips everything out. */
  ਜੇ (likely (c->budget_ਬਚਿਆ >= 0))
    hb_font_draw_glyph (font, glyph, static_paint_extents_draw_funcs.get_unconst (), &sink);
  c->ਧੱਕੋ_clip (sink.extents);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *paint_ਅੰਕੜੇ,
				      ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin, ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
				      ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  hb_extents_t<> extents = {xmin, ymin, xmax, ymax};
  c->ਧੱਕੋ_clip (extents);
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਕੱਢੋ_clip (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *paint_ਅੰਕੜੇ,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->ਕੱਢੋ_clip ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->ਧੱਕੋ_ਸਮੂਹ ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
			    ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->ਕੱਢੋ_ਸਮੂਹ (ਬਹੁਲਕ);
}

ਸਥਿਰ hb_bool_t
hb_paint_extents_paint_image (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_blob_t *blob HB_ਅਣਵਰਤਿਆ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ HB_ਅਣਵਰਤਿਆ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਚਾਈ HB_ਅਣਵਰਤਿਆ,
			      hb_ਟੈਗ_t format HB_ਅਣਵਰਤਿਆ,
			      ਦਸ਼ਮਲਵ slant HB_ਅਣਵਰਤਿਆ,
			      hb_glyph_extents_t *glyph_extents,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  ਜੇ (!glyph_extents)
    ਵਾਪਸ ਝੂਠ; // Happens with SVG images.

  hb_extents_t<> extents = {(ਦਸ਼ਮਲਵ) glyph_extents->x_bearing,
			    (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing + glyph_extents->ਉਚਾਈ,
			    (ਦਸ਼ਮਲਵ) glyph_extents->x_bearing + glyph_extents->ਚੌੜਾਈ,
			    (ਦਸ਼ਮਲਵ) glyph_extents->y_bearing};
  c->ਧੱਕੋ_clip (extents);
  c->paint ();
  c->ਕੱਢੋ_clip ();

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_paint_color (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_bool_t use_foreground HB_ਅਣਵਰਤਿਆ,
			      hb_color_t color HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *paint_ਅੰਕੜੇ,
				        hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y0 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y1 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x2 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y2 HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *paint_ਅੰਕੜੇ,
				        hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y0 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ r0 HB_ਅਣਵਰਤਿਆ,
				        ਦਸ਼ਮਲਵ x1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y1 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ r1 HB_ਅਣਵਰਤਿਆ,
				        ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ ਖਾਲੀ
hb_paint_extents_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *paint_ਅੰਕੜੇ,
				       hb_color_ਸਤਰ_t *color_ਸਤਰ HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ cx HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ cy HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle HB_ਅਣਵਰਤਿਆ,
				       ਦਸ਼ਮਲਵ ਸਿਰਾ_angle HB_ਅਣਵਰਤਿਆ,
				       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_extents_context_t *c = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;

  c->paint ();
}

ਸਥਿਰ hb_bool_t
hb_paint_extents_ਸੈੱਟ_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *context = (hb_paint_extents_context_t *) paint_ਅੰਕੜੇ;
  context->budget = budget;
  context->budget_initialized = ਸੱਚ;
  context->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_paint_extents_get_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_paint_extents_context_t *) paint_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_paint_extents_get_budget_ਬਚਿਆ (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ &((hb_paint_extents_context_t *) paint_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_paint_extents_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_paint_extents_funcs_lazy_loader_t : hb_paint_funcs_lazy_loader_t<hb_paint_extents_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func (funcs, hb_paint_extents_ਧੱਕੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func (funcs, hb_paint_extents_ਕੱਢੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, hb_paint_extents_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, hb_paint_extents_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, hb_paint_extents_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, hb_paint_extents_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, hb_paint_extents_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, hb_paint_extents_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, hb_paint_extents_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, hb_paint_extents_paint_ਰੇਖਿਕ_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, hb_paint_extents_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, hb_paint_extents_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_paint_extents_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_paint_extents_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_paint_extents_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_paint_extents_funcs);

    ਵਾਪਸ funcs;
  }
} static_paint_extents_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_paint_extents_funcs ()
{
  static_paint_extents_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_paint_extents_get_funcs ()
{
  ਵਾਪਸ static_paint_extents_funcs.get_unconst ();
}


#ਜੇ_ਅੰਤ
