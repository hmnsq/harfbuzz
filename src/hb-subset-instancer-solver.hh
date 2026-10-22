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

#அந்தர்பாவ "hb.hh"

/* pre-normalized distances */
கட்டமைப்பு TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (தசம neg_, தசம pos_): negative (neg_), positive (pos_) {}
  TripleDistances (தசம min, தசம default_, தசம max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  தசம negative;
  தசம positive;
};

கட்டமைப்பு Triple
{
  Triple () = இயல்பு;

  Triple (தசம minimum_, தசம நடு_2, தசம maximum_) :
    minimum (minimum_), நடு (நடு_2), maximum (maximum_) {}

  தருக்க operator == (மாறிலி Triple &o) மாறிலி
  {
    திரும்பு minimum == o.minimum &&
	   நடு  == o.நடு  &&
	   maximum == o.maximum;
  }

  தருக்க operator != (மாறிலி Triple o) மாறிலி
  { திரும்பு !(*இது == o); }

  தருக்க is_point () மாறிலி
  { திரும்பு minimum == நடு && நடு == maximum; }

  தருக்க contains (தசம point) மாறிலி
  { திரும்பு minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () மாறிலி
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (நடு);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    திரும்பு current;
  }

  தசம minimum = 0;
  தசம நடு = 0;
  தசம maximum = 0;
};

using rebase_tent_result_item_t = hb_pair_t<தசம, Triple>;
using rebase_tent_result_t = hb_vector_t<rebase_tent_result_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL தசம renormalize_மதிப்பு (தசம v, மாறிலி Triple &triple,
                                    மாறிலி TripleDistances &triple_distances,
                                    தருக்க extrapolate = உண்மை);
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
HB_INTERNAL வெறுமை rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_result_t &out,
			      rebase_tent_result_t &scratch);

#endif /* HB_SUBSET_INSTANCER_SOLVER_HH */
