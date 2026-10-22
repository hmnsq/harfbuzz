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

#अन्तर्भाव "hb.hh"

/* pre-normalized distances */
संरचना TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (भिन्न neg_, भिन्न pos_): negative (neg_), positive (pos_) {}
  TripleDistances (भिन्न min, भिन्न default_, भिन्न max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  भिन्न negative;
  भिन्न positive;
};

संरचना Triple
{
  Triple () = स्वभाव;

  Triple (भिन्न minimum_, भिन्न बीच_2, भिन्न maximum_) :
    minimum (minimum_), बीच (बीच_2), maximum (maximum_) {}

  तार्किक चालक == (अचर Triple &o) अचर
  {
    फर्काऊ minimum == o.minimum &&
	   बीच  == o.बीच  &&
	   maximum == o.maximum;
  }

  तार्किक चालक != (अचर Triple o) अचर
  { फर्काऊ !(*एतत् == o); }

  तार्किक is_point () अचर
  { फर्काऊ minimum == बीच && बीच == maximum; }

  तार्किक contains (भिन्न point) अचर
  { फर्काऊ minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () अचर
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (बीच);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    फर्काऊ current;
  }

  भिन्न minimum = 0;
  भिन्न बीच = 0;
  भिन्न maximum = 0;
};

using rebase_tent_result_item_t = hb_pair_t<भिन्न, Triple>;
using rebase_tent_result_t = hb_vector_t<rebase_tent_result_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL भिन्न renormalize_मान (भिन्न v, अचर Triple &triple,
                                    अचर TripleDistances &triple_distances,
                                    तार्किक extrapolate = सत्य);
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
HB_INTERNAL शून्य rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_result_t &out,
			      rebase_tent_result_t &scratch);

#endif /* HB_SUBSET_INSTANCER_SOLVER_HH */
