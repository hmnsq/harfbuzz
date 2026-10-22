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

#اگر_تعریف_نشده HB_SUBSET_INSTANCER_SOLVER_HH
#تعریف_کن HB_SUBSET_INSTANCER_SOLVER_HH

#ضمن "hb.hh"

/* pre-normalized distances */
بنية TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (عائم neg_, عائم pos_): negative (neg_), positive (pos_) {}
  TripleDistances (عائم min, عائم default_, عائم max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  عائم negative;
  عائم positive;
};

بنية Triple
{
  Triple () = افتراضي;

  Triple (عائم minimum_, عائم میانه_2, عائم بیشینه_2) :
    minimum (minimum_), میانه (میانه_2), بیشینه (بیشینه_2) {}

  منطقي عملگر == (ثابت Triple &o) ثابت
  {
    أرجع minimum == o.minimum &&
	   میانه  == o.میانه  &&
	   بیشینه == o.بیشینه;
  }

  منطقي عملگر != (ثابت Triple o) ثابت
  { أرجع !(*هذا == o); }

  منطقي is_نقطه () ثابت
  { أرجع minimum == میانه && میانه == بیشینه; }

  منطقي شامل_است (عائم نقطه) ثابت
  { أرجع minimum <= نقطه && نقطه <= بیشینه; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ثابت
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (میانه);
    current = current * 16777619;

    current = current ^ hb_hash (بیشینه);
    current = current * 16777619;
    أرجع current;
  }

  عائم minimum = 0;
  عائم میانه = 0;
  عائم بیشینه = 0;
};

استفاده rebase_tent_نتیجه_قلم_t = hb_pair_t<عائم, Triple>;
استفاده rebase_tent_نتیجه_t = hb_وکتور_t<rebase_tent_نتیجه_قلم_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_داخلی عائم renormalize_مقدار (عائم v, ثابت Triple &triple,
                                    ثابت TripleDistances &triple_distances,
                                    منطقي extrapolate = درست);
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
HB_داخلی فراغ rebase_tent (Triple tent,
			      Triple axis_حد_2,
			      TripleDistances axis_triple_distances,
			      rebase_tent_نتیجه_t &بیرون,
			      rebase_tent_نتیجه_t &scratch);

#پایان_اگر /* HB_SUBSET_INSTANCER_SOLVER_HH */
