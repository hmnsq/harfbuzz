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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_INSTANCER_IUP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_INSTANCER_IUP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"

ਬਣਤਰ iup_scratch_t
{
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਸਿਰਾ_points;
  hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> interp_x_deltas;
  hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> interp_y_deltas;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> costs;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> chain;
  hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> rot_indices;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rot_x_deltas;
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> rot_y_deltas;
  contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t rot_points;
};

/* given contour points and deltas, optimize a set of referenced points within error
 * tolerance. Returns optimized referenced point indices */
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ iup_delta_optimize (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
                                     ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& x_deltas,
                                     ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& y_deltas,
                                     hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>& opt_indices, /*ਬਾਹਰਲਾ*/
				     iup_scratch_t &scratch,
                                     ਦੁੱਗਣਾ tolerance = 0.0);

#ਜੇ_ਅੰਤ /* HB_SUBSET_INSTANCER_IUP_HH */
