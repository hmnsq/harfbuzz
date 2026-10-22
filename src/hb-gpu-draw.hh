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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-gpu.h"
#அந்தர்பாவ "hb-object.hh"
#அந்தர்பாவ "hb-vector.hh"

#அந்தர்பாவ <cmath>


#define HB_GPU_UNITS_PER_EM 4


கட்டமைப்பு hb_gpu_curve_t
{
  இரட்டை p1x, p1y;
  இரட்டை p2x, p2y;
  இரட்டை p3x, p3y;
  தருக்க contour_start;
};

கட்டமைப்பு hb_gpu_encode_curve_info_t
{
  இரட்டை min_x, max_x, min_y, max_y;
  தருக்க is_horizontal;
  தருக்க is_vertical;
  முழு hband_lo, hband_hi;
  முழு vband_lo, vband_hi;
};

கட்டமைப்பு hb_gpu_encode_scratch_t
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

  வெறுமை clear ()
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

கட்டமைப்பு hb_gpu_draw_t
{
  hb_object_header_t header;

  /* Accumulator state */
  இரட்டை start_x = 0, start_y = 0;
  இரட்டை current_x = 0, current_y = 0;
  தருக்க need_moveto = உண்மை;
  unsigned இலக்கம்_curves = 0;
  தருக்க success = உண்மை;

  hb_vector_t<hb_gpu_curve_t> curves;

  /* Extents tracking (updated during accumulation) */
  இரட்டை ext_min_x =  HUGE_VAL;
  இரட்டை ext_min_y =  HUGE_VAL;
  இரட்டை ext_max_x = -HUGE_VAL;
  இரட்டை ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  முழு x_scale = 0;
  முழு y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_remaining = HB_BUDGET_GLYPH;

  வெறுமை recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_remaining ()
  { திரும்பு &budget_remaining; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = nullptr;

  /* Internal accumulation methods */
  HB_INTERNAL வெறுமை acc_move_to (இரட்டை x, இரட்டை y);
  HB_INTERNAL வெறுமை acc_வரி_to (இரட்டை x, இரட்டை y);
  HB_INTERNAL வெறுமை acc_conic_to (இரட்டை cx, இரட்டை cy, இரட்டை x, இரட்டை y);
  HB_INTERNAL வெறுமை acc_cubic_to (இரட்டை c1x, இரட்டை c1y,
				 இரட்டை c2x, இரட்டை c2y,
				 இரட்டை x, இரட்டை y);
  HB_INTERNAL வெறுமை acc_close_path ();
};


#endif /* HB_GPU_DRAW_HH */
