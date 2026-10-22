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

#شامل "hb.hh"

/* pre-normalized distances */
ساختار TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (اعشاری neg_, اعشاری pos_): negative (neg_), positive (pos_) {}
  TripleDistances (اعشاری min, اعشاری default_, اعشاری max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  اعشاری negative;
  اعشاری positive;
};

ساختار Triple
{
  Triple () = پیشفرض;

  Triple (اعشاری minimum_, اعشاری میانه_2, اعشاری maximum_) :
    minimum (minimum_), میانه (میانه_2), maximum (maximum_) {}

  منطقی operator == (ثابت Triple &o) ثابت
  {
    بازگشت minimum == o.minimum &&
	   میانه  == o.میانه  &&
	   maximum == o.maximum;
  }

  منطقی operator != (ثابت Triple o) ثابت
  { بازگشت !(*این == o); }

  منطقی is_point () ثابت
  { بازگشت minimum == میانه && میانه == maximum; }

  منطقی contains (اعشاری point) ثابت
  { بازگشت minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ثابت
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (میانه);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    بازگشت current;
  }

  اعشاری minimum = 0;
  اعشاری میانه = 0;
  اعشاری maximum = 0;
};

using rebase_tent_result_item_t = hb_pair_t<اعشاری, Triple>;
using rebase_tent_result_t = hb_vector_t<rebase_tent_result_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL اعشاری renormalize_مقدار (اعشاری v, ثابت Triple &triple,
                                    ثابت TripleDistances &triple_distances,
                                    منطقی extrapolate = درست);
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
HB_INTERNAL تهی rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_result_t &out,
			      rebase_tent_result_t &scratch);

#endif /* HB_SUBSET_INSTANCER_SOLVER_HH */
