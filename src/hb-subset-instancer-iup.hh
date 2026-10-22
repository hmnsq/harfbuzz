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

#నిర్వచితం_కాకపోతే HB_SUBSET_INSTANCER_IUP_HH
#నిర్వచించు HB_SUBSET_INSTANCER_IUP_HH

#అన్తర్భావ "hb-subset-plan.hh"

నిర్మాణం iup_scratch_t
{
  hb_సదిశ_t<గుర్తులేని> చివర_points;
  hb_సదిశ_t<ద్విగుణ> interp_x_deltas;
  hb_సదిశ_t<ద్విగుణ> interp_y_deltas;
  hb_సదిశ_t<గుర్తులేని> costs;
  hb_సదిశ_t<పూర్ణాంకం> chain;
  hb_సదిశ_t<తార్కిక> rot_indices;
  hb_సదిశ_t<పూర్ణాంకం> rot_x_deltas;
  hb_సదిశ_t<పూర్ణాంకం> rot_y_deltas;
  contour_బిందువు_సదిశ_t rot_points;
};

/* given contour points and deltas, optimize a set of referenced points within error
 * tolerance. Returns optimized referenced point indices */
HB_అంతర్గత తార్కిక iup_delta_optimize (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
                                     స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& x_deltas,
                                     స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& y_deltas,
                                     hb_సదిశ_t<తార్కిక>& opt_indices, /*బహిర్గతం*/
				     iup_scratch_t &scratch,
                                     ద్విగుణ tolerance = 0.0);

#అయితే_ముగింపు /* HB_SUBSET_INSTANCER_IUP_HH */
