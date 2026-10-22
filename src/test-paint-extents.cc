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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-paint-extents.hh"


ഘടന budget_probe_t
{
  int64_t observed = -1;
};

സ്ഥിതം hb_bool_t
draw_budget_probe (hb_font_t *, ശൂന്യം *font_data, hb_codepoint_t,
		   hb_draw_funcs_t *draw_funcs, ശൂന്യം *draw_data, ശൂന്യം *)
{
  auto *probe = (budget_probe_t *) font_data;
  probe->observed = hb_draw_get_budget_remaining (draw_funcs, draw_data);

  hb_draw_state_t state = HB_DRAW_STATE_DEFAULT;
  hb_draw_move_to (draw_funcs, draw_data, &state, 10.f, 20.f);
  hb_draw_വരി_to (draw_funcs, draw_data, &state, 30.f, 40.f);
  തിരികെ സത്യം;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc HB_UNUSED, അക്ഷരം **argv HB_UNUSED)
{
  hb_paint_funcs_t *paint_funcs = hb_paint_extents_get_funcs ();

  /* COLR caches this context in zero-allocated scratch storage. */
  auto *zeroed = (hb_paint_extents_context_t *)
		 hb_calloc (1, വലുപ്പം (hb_paint_extents_context_t));
  hb_always_assert (zeroed);
  zeroed->clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, zeroed) == HB_BUDGET_DEFAULT);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, zeroed) > 0);
  zeroed->~hb_paint_extents_context_t ();
  hb_free (zeroed);

  hb_paint_extents_context_t context;

  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == HB_BUDGET_DEFAULT);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) > 0);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) < HB_BUDGET_UNLIMITED);

  budget_probe_t probe;
  hb_font_funcs_t *font_funcs = hb_font_funcs_create ();
  hb_font_funcs_set_draw_glyph_or_fail_func (font_funcs, draw_budget_probe, nullptr, nullptr);
  hb_font_t *font = hb_font_create (hb_face_get_empty ());
  hb_font_set_funcs (font, font_funcs, &probe, nullptr);
  hb_font_funcs_destroy (font_funcs);

  hb_always_assert (hb_paint_set_budget (paint_funcs, &context, 1));
  hb_paint_തള്ളുക_clip_glyph (paint_funcs, &context, 0, font);
  hb_always_assert (probe.observed == 1);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) < 0);

  context.clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == 1);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) == 1);

  hb_always_assert (hb_paint_set_budget (paint_funcs, &context, HB_BUDGET_UNLIMITED));
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) == HB_BUDGET_UNLIMITED);

  hb_always_assert (hb_paint_set_budget (paint_funcs, &context, 0));
  context.clear ();
  hb_always_assert (hb_paint_get_budget (paint_funcs, &context) == 0);
  hb_always_assert (hb_paint_get_budget_remaining (paint_funcs, &context) == 0);

  hb_font_destroy (font);
  തിരികെ 0;
}
