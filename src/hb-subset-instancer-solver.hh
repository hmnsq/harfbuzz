/*
 * Copyright © 2023  Behdad Esfahbod
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

#ifndef HB_SUBSET_INSTANCER_SOLVER_HH
#define HB_SUBSET_INSTANCER_SOLVER_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"

/* pre-normalized distances */
ಸಂರಚನಾ TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (ಭಿನ್ನ neg_, ಭಿನ್ನ pos_): negative (neg_), positive (pos_) {}
  TripleDistances (ಭಿನ್ನ min, ಭಿನ್ನ default_, ಭಿನ್ನ max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  ಭಿನ್ನ negative;
  ಭಿನ್ನ positive;
};

ಸಂರಚನಾ Triple
{
  Triple () = ಸ್ವಭಾವ;

  Triple (ಭಿನ್ನ minimum_, ಭಿನ್ನ ಮಧ್ಯ_2, ಭಿನ್ನ maximum_) :
    minimum (minimum_), ಮಧ್ಯ (ಮಧ್ಯ_2), maximum (maximum_) {}

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ Triple &o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ minimum == o.minimum &&
	   ಮಧ್ಯ  == o.ಮಧ್ಯ  &&
	   maximum == o.maximum;
  }

  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ Triple o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }

  ತಾರ್ಕಿಕ is_point () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ minimum == ಮಧ್ಯ && ಮಧ್ಯ == maximum; }

  ತಾರ್ಕಿಕ contains (ಭಿನ್ನ point) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ಧ್ರುವ
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (ಮಧ್ಯ);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    ಪ್ರತ್ಯಾಗಮ current;
  }

  ಭಿನ್ನ minimum = 0;
  ಭಿನ್ನ ಮಧ್ಯ = 0;
  ಭಿನ್ನ maximum = 0;
};

using rebase_tent_result_item_t = hb_pair_t<ಭಿನ್ನ, Triple>;
using rebase_tent_result_t = hb_vector_t<rebase_tent_result_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL ಭಿನ್ನ renormalize_ಮೌಲ್ಯ (ಭಿನ್ನ v, ಧ್ರುವ Triple &triple,
                                    ಧ್ರುವ TripleDistances &triple_distances,
                                    ತಾರ್ಕಿಕ extrapolate = ಸತ್ಯ);
/* Given a tuple (lower,peak,upper) "tent" and new axis limits
 * (axisMin,axisDefault,axisMax), solves how to represent the tent
 * under the new axis configuration.  All values are in normalized
 * -1,0,+1 coordinate system. Tent values can be outside this range.
 *
 * Return value: a list of tuples. Each tuple is of the form
 * (scalar,tent), where scalar is a multipler to multiply any
 * delta-sets by, and tent is a new tent for that output delta-set.
 * If tent value is Triple{}, that is a special deltaset that should
 * be always-enabled (called "gain").
 */
HB_INTERNAL ಶೂನ್ಯ rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_result_t &out,
			      rebase_tent_result_t &scratch);

#endif /* HB_SUBSET_INSTANCER_SOLVER_HH */
