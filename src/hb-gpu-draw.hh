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

#اگر_تعریف_نشده HB_GPU_DRAW_HH
#تعریف_کن HB_GPU_DRAW_HH

#ضمن "hb.hh"
#ضمن "hb-gpu.h"
#ضمن "hb-object.hh"
#ضمن "hb-vector.hh"

#ضمن <cmath>


#تعریف_کن HB_GPU_UNITS_در_هر_EM 4


بنية hb_gpu_منحنی_t
{
  مزدوج p1x, p1y;
  مزدوج p2x, p2y;
  مزدوج p3x, p3y;
  منطقي contour_شروع;
};

بنية hb_gpu_encode_منحنی_info_t
{
  مزدوج min_x, max_x, min_y, max_y;
  منطقي is_horizontal;
  منطقي is_vertical;
  صحيح hband_lo, hband_hi;
  صحيح vband_lo, vband_hi;
};

بنية hb_gpu_encode_scratch_t
{
  hb_وکتور_t<hb_gpu_encode_منحنی_info_t> منحنی_infos;
  hb_وکتور_t<بیعلامت> hband_منحنی_counts;
  hb_وکتور_t<بیعلامت> vband_منحنی_counts;
  hb_وکتور_t<بیعلامت> hband_offsets;
  hb_وکتور_t<بیعلامت> vband_offsets;
  hb_وکتور_t<بیعلامت> hband_curves;
  hb_وکتور_t<بیعلامت> hband_curves_asc;
  hb_وکتور_t<بیعلامت> vband_curves;
  hb_وکتور_t<بیعلامت> vband_curves_asc;
  hb_وکتور_t<بیعلامت> hband_cursors;
  hb_وکتور_t<بیعلامت> vband_cursors;
  hb_وکتور_t<بیعلامت> منحنی_texel_جابه_جایی;

  فراغ clear ()
  {
    منحنی_infos.clear ();
    hband_منحنی_counts.clear ();
    vband_منحنی_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    منحنی_texel_جابه_جایی.clear ();
  }
};

بنية hb_gpu_draw_t
{
  hb_object_سرآیند_t سرآیند;

  /* Accumulator state */
  مزدوج شروع_x = 0, شروع_y = 0;
  مزدوج current_x = 0, current_y = 0;
  منطقي need_moveto = درست;
  بیعلامت رقم_curves = 0;
  منطقي success = درست;

  hb_وکتور_t<hb_gpu_منحنی_t> curves;

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
  int64_t  budget_مانده = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_مانده = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_مانده ()
  { أرجع &budget_مانده; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = اشاره_تهی;

  /* Internal accumulation methods */
  HB_داخلی فراغ acc_move_مقصد (مزدوج x, مزدوج y);
  HB_داخلی فراغ acc_سطر_مقصد (مزدوج x, مزدوج y);
  HB_داخلی فراغ acc_conic_مقصد (مزدوج cx, مزدوج cy, مزدوج x, مزدوج y);
  HB_داخلی فراغ acc_cubic_مقصد (مزدوج c1x, مزدوج c1y,
				 مزدوج c2x, مزدوج c2y,
				 مزدوج x, مزدوج y);
  HB_داخلی فراغ acc_close_مسیر ();
};


#پایان_اگر /* HB_GPU_DRAW_HH */
