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

#ifndef HB_SUBSET_INSTANCER_IUP_HH
#define HB_SUBSET_INSTANCER_IUP_HH

#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"

ସଂରଚନା iup_scratch_t
{
  hb_vector_t<unsigned> end_points;
  hb_vector_t<ଦ୍ୱିଗୁଣ> interp_x_deltas;
  hb_vector_t<ଦ୍ୱିଗୁଣ> interp_y_deltas;
  hb_vector_t<unsigned> costs;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> chain;
  hb_vector_t<ତାର୍କିକ> rot_indices;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rot_x_deltas;
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> rot_y_deltas;
  contour_point_vector_t rot_points;
};

/* given contour points and deltas, optimize a set of referenced points within error
 * tolerance. Returns optimized referenced point indices */
HB_INTERNAL ତାର୍କିକ iup_delta_optimize (ଅଚଳ contour_point_vector_t& contour_points,
                                     ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& x_deltas,
                                     ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& y_deltas,
                                     hb_vector_t<ତାର୍କିକ>& opt_indices, /* OUT */
				     iup_scratch_t &scratch,
                                     ଦ୍ୱିଗୁଣ tolerance = 0.0);

#endif /* HB_SUBSET_INSTANCER_IUP_HH */
