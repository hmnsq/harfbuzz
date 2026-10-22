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

#اگر_غیر_تعریف_شدہ HB_GPU_DRAW_HH
#تعریف_کرو HB_GPU_DRAW_HH

#شامل "hb.hh"
#شامل "hb-gpu.h"
#شامل "hb-object.hh"
#شامل "hb-vector.hh"

#شامل <cmath>


#تعریف_کرو HB_GPU_UNITS_فی_EM 4


ساخت hb_gpu_خم_t
{
  دوہرا p1x, p1y;
  دوہرا p2x, p2y;
  دوہرا p3x, p3y;
  منطقی contour_آغاز;
};

ساخت hb_gpu_encode_خم_info_t
{
  دوہرا min_x, max_x, min_y, max_y;
  منطقی is_horizontal;
  منطقی is_vertical;
  صحیح hband_lo, hband_hi;
  صحیح vband_lo, vband_hi;
};

ساخت hb_gpu_encode_scratch_t
{
  hb_سمتیہ_t<hb_gpu_encode_خم_info_t> خم_infos;
  hb_سمتیہ_t<غیرعلامتی> hband_خم_counts;
  hb_سمتیہ_t<غیرعلامتی> vband_خم_counts;
  hb_سمتیہ_t<غیرعلامتی> hband_offsets;
  hb_سمتیہ_t<غیرعلامتی> vband_offsets;
  hb_سمتیہ_t<غیرعلامتی> hband_curves;
  hb_سمتیہ_t<غیرعلامتی> hband_curves_asc;
  hb_سمتیہ_t<غیرعلامتی> vband_curves;
  hb_سمتیہ_t<غیرعلامتی> vband_curves_asc;
  hb_سمتیہ_t<غیرعلامتی> hband_cursors;
  hb_سمتیہ_t<غیرعلامتی> vband_cursors;
  hb_سمتیہ_t<غیرعلامتی> خم_texel_انحراف;

  خالی clear ()
  {
    خم_infos.clear ();
    hband_خم_counts.clear ();
    vband_خم_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    خم_texel_انحراف.clear ();
  }
};

ساخت hb_gpu_draw_t
{
  hb_object_سرخی_t سرخی;

  /* Accumulator state */
  دوہرا آغاز_x = 0, آغاز_y = 0;
  دوہرا current_x = 0, current_y = 0;
  منطقی need_moveto = سچ;
  غیرعلامتی عدد_curves = 0;
  منطقی success = سچ;

  hb_سمتیہ_t<hb_gpu_خم_t> curves;

  /* Extents tracking (updated during accumulation) */
  دوہرا ext_min_x =  HUGE_VAL;
  دوہرا ext_min_y =  HUGE_VAL;
  دوہرا ext_max_x = -HUGE_VAL;
  دوہرا ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  صحیح x_scale = 0;
  صحیح y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_بقایا = HB_BUDGET_GLYPH;

  خالی recharge_budget ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_بقایا ()
  { واپس &budget_بقایا; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = خالی_اشاریہ;

  /* Internal accumulation methods */
  HB_اندرونی خالی acc_move_مقصد (دوہرا x, دوہرا y);
  HB_اندرونی خالی acc_سطر_مقصد (دوہرا x, دوہرا y);
  HB_اندرونی خالی acc_conic_مقصد (دوہرا cx, دوہرا cy, دوہرا x, دوہرا y);
  HB_اندرونی خالی acc_cubic_مقصد (دوہرا c1x, دوہرا c1y,
				 دوہرا c2x, دوہرا c2y,
				 دوہرا x, دوہرا y);
  HB_اندرونی خالی acc_close_راستہ ();
};


#اگر_ختم /* HB_GPU_DRAW_HH */
