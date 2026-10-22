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

#ifndef HB_GPU_DRAW_HH
#define HB_GPU_DRAW_HH

#شامل "hb.hh"
#شامل "hb-gpu.h"
#شامل "hb-object.hh"
#شامل "hb-vector.hh"

#شامل <cmath>


#define HB_GPU_UNITS_PER_EM 4


ساختار hb_gpu_curve_t
{
  دوگانه p1x, p1y;
  دوگانه p2x, p2y;
  دوگانه p3x, p3y;
  منطقی contour_start;
};

ساختار hb_gpu_encode_curve_info_t
{
  دوگانه min_x, max_x, min_y, max_y;
  منطقی is_horizontal;
  منطقی is_vertical;
  صحیح hband_lo, hband_hi;
  صحیح vband_lo, vband_hi;
};

ساختار hb_gpu_encode_scratch_t
{
  hb_vector_t<hb_gpu_encode_curve_info_t> curve_infos;
  hb_vector_t<unsigned> hband_curve_counts;
  hb_vector_t<unsigned> vband_curve_counts;
  hb_vector_t<unsigned> hband_offsets;
  hb_vector_t<unsigned> vband_offsets;
  hb_vector_t<unsigned> hband_curves;
  hb_vector_t<unsigned> hband_curves_asc;
  hb_vector_t<unsigned> vband_curves;
  hb_vector_t<unsigned> vband_curves_asc;
  hb_vector_t<unsigned> hband_cursors;
  hb_vector_t<unsigned> vband_cursors;
  hb_vector_t<unsigned> curve_texel_offset;

  تهی clear ()
  {
    curve_infos.clear ();
    hband_curve_counts.clear ();
    vband_curve_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    curve_texel_offset.clear ();
  }
};

ساختار hb_gpu_draw_t
{
  hb_object_header_t header;

  /* Accumulator state */
  دوگانه start_x = 0, start_y = 0;
  دوگانه current_x = 0, current_y = 0;
  منطقی need_moveto = درست;
  unsigned رقم_curves = 0;
  منطقی success = درست;

  hb_vector_t<hb_gpu_curve_t> curves;

  /* Extents tracking (updated during accumulation) */
  دوگانه ext_min_x =  HUGE_VAL;
  دوگانه ext_min_y =  HUGE_VAL;
  دوگانه ext_max_x = -HUGE_VAL;
  دوگانه ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  صحیح x_scale = 0;
  صحیح y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_remaining = HB_BUDGET_GLYPH;

  تهی recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_remaining ()
  { بازگشت &budget_remaining; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = nullptr;

  /* Internal accumulation methods */
  HB_INTERNAL تهی acc_move_to (دوگانه x, دوگانه y);
  HB_INTERNAL تهی acc_سطر_to (دوگانه x, دوگانه y);
  HB_INTERNAL تهی acc_conic_to (دوگانه cx, دوگانه cy, دوگانه x, دوگانه y);
  HB_INTERNAL تهی acc_cubic_to (دوگانه c1x, دوگانه c1y,
				 دوگانه c2x, دوگانه c2y,
				 دوگانه x, دوگانه y);
  HB_INTERNAL تهی acc_close_path ();
};


#endif /* HB_GPU_DRAW_HH */
