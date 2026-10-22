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

#جے_بنا_تعریف HB_GPU_DRAW_HH
#تعریف_کرو HB_GPU_DRAW_HH

#ضمن "hb.hh"
#ضمن "hb-gpu.h"
#ضمن "hb-object.hh"
#ضمن "hb-vector.hh"

#ضمن <cmath>


#تعریف_کرو HB_GPU_UNITS_فی_EM 4


بنية hb_gpu_ونگ_t
{
  مزدوج p1x, p1y;
  مزدوج p2x, p2y;
  مزدوج p3x, p3y;
  منطقي contour_شروع;
};

بنية hb_gpu_encode_ونگ_info_t
{
  مزدوج min_x, max_x, min_y, max_y;
  منطقي is_horizontal;
  منطقي is_vertical;
  صحيح hband_lo, hband_hi;
  صحيح vband_lo, vband_hi;
};

بنية hb_gpu_encode_scratch_t
{
  hb_سمتیہ_t<hb_gpu_encode_ونگ_info_t> ونگ_infos;
  hb_سمتیہ_t<بنانشانی> hband_ونگ_counts;
  hb_سمتیہ_t<بنانشانی> vband_ونگ_counts;
  hb_سمتیہ_t<بنانشانی> hband_offsets;
  hb_سمتیہ_t<بنانشانی> vband_offsets;
  hb_سمتیہ_t<بنانشانی> hband_curves;
  hb_سمتیہ_t<بنانشانی> hband_curves_asc;
  hb_سمتیہ_t<بنانشانی> vband_curves;
  hb_سمتیہ_t<بنانشانی> vband_curves_asc;
  hb_سمتیہ_t<بنانشانی> hband_cursors;
  hb_سمتیہ_t<بنانشانی> vband_cursors;
  hb_سمتیہ_t<بنانشانی> ونگ_texel_وتھ;

  فراغ clear ()
  {
    ونگ_infos.clear ();
    hband_ونگ_counts.clear ();
    vband_ونگ_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    ونگ_texel_وتھ.clear ();
  }
};

بنية hb_gpu_draw_t
{
  hb_object_سرخی_t سرخی;

  /* Accumulator state */
  مزدوج شروع_x = 0, شروع_y = 0;
  مزدوج current_x = 0, current_y = 0;
  منطقي need_moveto = سچ;
  بنانشانی عدد_curves = 0;
  منطقي success = سچ;

  hb_سمتیہ_t<hb_gpu_ونگ_t> curves;

  /* Extents tracking (updated during accumulation) */
  مزدوج ext_min_x =  HUGE_VAL;
  مزدوج ext_min_y =  HUGE_VAL;
  مزدوج ext_max_x = -HUGE_VAL;
  مزدوج ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  صحيح x_scale = 0;
  صحيح y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_بچیا = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچیا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_بچیا ()
  { أرجع &budget_بچیا; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = خالی_سوچک;

  /* Internal accumulation methods */
  HB_اندرونی فراغ acc_move_منزل (مزدوج x, مزدوج y);
  HB_اندرونی فراغ acc_سطر_منزل (مزدوج x, مزدوج y);
  HB_اندرونی فراغ acc_conic_منزل (مزدوج cx, مزدوج cy, مزدوج x, مزدوج y);
  HB_اندرونی فراغ acc_cubic_منزل (مزدوج c1x, مزدوج c1y,
				 مزدوج c2x, مزدوج c2y,
				 مزدوج x, مزدوج y);
  HB_اندرونی فراغ acc_close_رستہ ();
};


#جے_انت /* HB_GPU_DRAW_HH */
