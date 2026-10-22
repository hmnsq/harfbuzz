/*
 * Copyright © 2026  Behdad Esfahbod
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
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint-extents.hh"


ਬਣਤਰ budget_probe_t
{
  int64_t observed = -1;
};

ਸਥਿਰ hb_bool_t
draw_budget_probe (hb_font_t *, ਖਾਲੀ *font_ਅੰਕੜੇ, hb_codepoint_t,
		   hb_draw_funcs_t *draw_funcs, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *probe = (budget_probe_t *) font_ਅੰਕੜੇ;
  probe->observed = hb_draw_get_budget_ਬਚਿਆ (draw_funcs, draw_ਅੰਕੜੇ);

  hb_draw_state_t state = HB_DRAW_STATE_DEFAULT;
  hb_draw_move_ਮੰਜ਼ਿਲ (draw_funcs, draw_ਅੰਕੜੇ, &state, 10.f, 20.f);
  hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (draw_funcs, draw_ਅੰਕੜੇ, &state, 30.f, 40.f);
  ਵਾਪਸ ਸੱਚ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc HB_ਅਣਵਰਤਿਆ, ਅੱਖਰ **argv HB_ਅਣਵਰਤਿਆ)
{
  hb_paint_funcs_t *paint_funcs = hb_paint_extents_get_funcs ();

  /* COLR caches this context in zero-allocated scratch storage. */
  ਸਵੈਚਾਲਿਤ *zeroed = (hb_paint_extents_context_t *)
		 hb_calloc (1, ਆਕਾਰ (hb_paint_extents_context_t));
  hb_always_assert (zeroed);
  zeroed->clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, zeroed) == HB_BUDGET_DEFAULT);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, zeroed) > 0);
  zeroed->~hb_paint_extents_context_t ();
  hb_free (zeroed);

  hb_paint_extents_context_t context;

  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == HB_BUDGET_DEFAULT);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) > 0);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) < HB_BUDGET_UNLIMITED);

  budget_probe_t probe;
  hb_font_funcs_t *font_funcs = hb_font_funcs_create ();
  hb_font_funcs_ਸੈੱਟ_draw_glyph_or_fail_func (font_funcs, draw_budget_probe, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_font_t *font = hb_font_create (hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ());
  hb_font_ਸੈੱਟ_funcs (font, font_funcs, &probe, ਖਾਲੀ_ਸੂਚਕ);
  hb_font_funcs_destroy (font_funcs);

  hb_always_assert (hb_paint_ਸੈੱਟ_budget (paint_funcs, &context, 1));
  hb_paint_ਧੱਕੋ_clip_glyph (paint_funcs, &context, 0, font);
  hb_always_assert (probe.observed == 1);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) < 0);

  context.clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == 1);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) == 1);

  hb_always_assert (hb_paint_ਸੈੱਟ_budget (paint_funcs, &context, HB_BUDGET_UNLIMITED));
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) == HB_BUDGET_UNLIMITED);

  hb_always_assert (hb_paint_ਸੈੱਟ_budget (paint_funcs, &context, 0));
  context.clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == 0);
  hb_always_assert (hb_paint_get_budget_ਬਚਿਆ (paint_funcs, &context) == 0);

  hb_font_destroy (font);
  ਵਾਪਸ 0;
}
