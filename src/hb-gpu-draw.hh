/*
 * Copyright (C) 2026  Behdad Esfahbod
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
 *
 * Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GPU_DRAW_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_DRAW_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-object.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_UNITS_ਪ੍ਰਤੀ_EM 4


ਬਣਤਰ hb_gpu_ਵਕਰ_t
{
  ਦੁੱਗਣਾ p1x, p1y;
  ਦੁੱਗਣਾ p2x, p2y;
  ਦੁੱਗਣਾ p3x, p3y;
  ਤਾਰਕਿਕ contour_ਸ਼ੁਰੂ;
};

ਬਣਤਰ hb_gpu_encode_ਵਕਰ_info_t
{
  ਦੁੱਗਣਾ min_x, max_x, min_y, max_y;
  ਤਾਰਕਿਕ is_horizontal;
  ਤਾਰਕਿਕ is_vertical;
  ਪੂਰਨ hband_lo, hband_hi;
  ਪੂਰਨ vband_lo, vband_hi;
};

ਬਣਤਰ hb_gpu_encode_scratch_t
{
  hb_ਸਦਿਸ਼_t<hb_gpu_encode_ਵਕਰ_info_t> ਵਕਰ_infos;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> hband_ਵਕਰ_counts;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vband_ਵਕਰ_counts;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> hband_offsets;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vband_offsets;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> hband_curves;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> hband_curves_asc;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vband_curves;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vband_curves_asc;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> hband_cursors;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vband_cursors;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵਕਰ_texel_ਵਿੱਥ;

  ਖਾਲੀ clear ()
  {
    ਵਕਰ_infos.clear ();
    hband_ਵਕਰ_counts.clear ();
    vband_ਵਕਰ_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    ਵਕਰ_texel_ਵਿੱਥ.clear ();
  }
};

ਬਣਤਰ hb_gpu_draw_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  /* Accumulator state */
  ਦੁੱਗਣਾ ਸ਼ੁਰੂ_x = 0, ਸ਼ੁਰੂ_y = 0;
  ਦੁੱਗਣਾ current_x = 0, current_y = 0;
  ਤਾਰਕਿਕ need_moveto = ਸੱਚ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_curves = 0;
  ਤਾਰਕਿਕ success = ਸੱਚ;

  hb_ਸਦਿਸ਼_t<hb_gpu_ਵਕਰ_t> curves;

  /* Extents tracking (updated during accumulation) */
  ਦੁੱਗਣਾ ext_min_x =  HUGE_VAL;
  ਦੁੱਗਣਾ ext_min_y =  HUGE_VAL;
  ਦੁੱਗਣਾ ext_max_x = -HUGE_VAL;
  ਦੁੱਗਣਾ ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  ਪੂਰਨ x_scale = 0;
  ਪੂਰਨ y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_ਬਚਿਆ = HB_BUDGET_GLYPH;

  ਖਾਲੀ recharge_budget ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_ਬਚਿਆ ()
  { ਵਾਪਸ &budget_ਬਚਿਆ; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = ਖਾਲੀ_ਸੂਚਕ;

  /* Internal accumulation methods */
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ acc_move_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ acc_ਸਤਰ_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ acc_conic_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ cx, ਦੁੱਗਣਾ cy, ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ acc_cubic_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ c1x, ਦੁੱਗਣਾ c1y,
				 ਦੁੱਗਣਾ c2x, ਦੁੱਗਣਾ c2y,
				 ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y);
  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ acc_close_ਰਸਤਾ ();
};


#ਜੇ_ਅੰਤ /* HB_GPU_DRAW_HH */
