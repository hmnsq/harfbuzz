/*
 * Copyright © 2024  Google, Inc.
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

#جے_بنا_تعریف HB_SUBSET_INSTANCER_IUP_HH
#تعریف_کرو HB_SUBSET_INSTANCER_IUP_HH

#ضمن "hb-subset-plan.hh"

بنية iup_scratch_t
{
  hb_سمتیہ_t<بنانشانی> سرا_points;
  hb_سمتیہ_t<مزدوج> interp_x_deltas;
  hb_سمتیہ_t<مزدوج> interp_y_deltas;
  hb_سمتیہ_t<بنانشانی> costs;
  hb_سمتیہ_t<صحيح> chain;
  hb_سمتیہ_t<منطقي> rot_indices;
  hb_سمتیہ_t<صحيح> rot_x_deltas;
  hb_سمتیہ_t<صحيح> rot_y_deltas;
  contour_نقطہ_سمتیہ_t rot_points;
};

/* given contour points and deltas, optimize a set of referenced points within error
 * tolerance. Returns optimized referenced point indices */
HB_اندرونی منطقي iup_delta_optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                                     ثابت hb_سمتیہ_t<صحيح>& x_deltas,
                                     ثابت hb_سمتیہ_t<صحيح>& y_deltas,
                                     hb_سمتیہ_t<منطقي>& opt_indices, /*باہرلا*/
				     iup_scratch_t &scratch,
                                     مزدوج tolerance = 0.0);

#جے_انت /* HB_SUBSET_INSTANCER_IUP_HH */
