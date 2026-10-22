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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-gpu.h"
#અન્તર્ભાવ "hb-object.hh"
#અન્તર્ભાવ "hb-vector.hh"

#અન્તર્ભાવ <cmath>


#define HB_GPU_UNITS_PER_EM 4


સંરચના hb_gpu_curve_t
{
  દ્વિગુણ p1x, p1y;
  દ્વિગુણ p2x, p2y;
  દ્વિગુણ p3x, p3y;
  તાર્કિક contour_start;
};

સંરચના hb_gpu_encode_curve_info_t
{
  દ્વિગુણ min_x, max_x, min_y, max_y;
  તાર્કિક is_horizontal;
  તાર્કિક is_vertical;
  પૂર્ણાંક hband_lo, hband_hi;
  પૂર્ણાંક vband_lo, vband_hi;
};

સંરચના hb_gpu_encode_scratch_t
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

  શૂન્ય clear ()
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

સંરચના hb_gpu_draw_t
{
  hb_object_header_t header;

  /* Accumulator state */
  દ્વિગુણ start_x = 0, start_y = 0;
  દ્વિગુણ current_x = 0, current_y = 0;
  તાર્કિક need_moveto = સાચું;
  unsigned અંક_curves = 0;
  તાર્કિક success = સાચું;

  hb_vector_t<hb_gpu_curve_t> curves;

  /* Extents tracking (updated during accumulation) */
  દ્વિગુણ ext_min_x =  HUGE_VAL;
  દ્વિગુણ ext_min_y =  HUGE_VAL;
  દ્વિગુણ ext_max_x = -HUGE_VAL;
  દ્વિગુણ ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  પૂર્ણાંક x_scale = 0;
  પૂર્ણાંક y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_remaining = HB_BUDGET_GLYPH;

  શૂન્ય recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_remaining ()
  { પાછા &budget_remaining; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = nullptr;

  /* Internal accumulation methods */
  HB_INTERNAL શૂન્ય acc_move_to (દ્વિગુણ x, દ્વિગુણ y);
  HB_INTERNAL શૂન્ય acc_લીટી_to (દ્વિગુણ x, દ્વિગુણ y);
  HB_INTERNAL શૂન્ય acc_conic_to (દ્વિગુણ cx, દ્વિગુણ cy, દ્વિગુણ x, દ્વિગુણ y);
  HB_INTERNAL શૂન્ય acc_cubic_to (દ્વિગુણ c1x, દ્વિગુણ c1y,
				 દ્વિગુણ c2x, દ્વિગુણ c2y,
				 દ્વિગુણ x, દ્વિગુણ y);
  HB_INTERNAL શૂન્ય acc_close_path ();
};


#endif /* HB_GPU_DRAW_HH */
