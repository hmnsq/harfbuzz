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

#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_INSTANCER_SOLVER_HH
#وصف_ڪريو HB_SUBSET_INSTANCER_SOLVER_HH

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

  Triple (عائم minimum_, عائم وچ_2, عائم وڌ_کان_وڌ_2) :
    minimum (minimum_), وچ (وچ_2), وڌ_کان_وڌ (وڌ_کان_وڌ_2) {}

  منطقي عامل == (ثابت Triple &o) ثابت
  {
    أرجع minimum == o.minimum &&
	   وچ  == o.وچ  &&
	   وڌ_کان_وڌ == o.وڌ_کان_وڌ;
  }

  منطقي عامل != (ثابت Triple o) ثابت
  { أرجع !(*هذا == o); }

  منطقي is_نقطو () ثابت
  { أرجع minimum == وچ && وچ == وڌ_کان_وڌ; }

  منطقي شامل_آهي (عائم نقطو) ثابت
  { أرجع minimum <= نقطو && نقطو <= وڌ_کان_وڌ; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ثابت
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (وچ);
    current = current * 16777619;

    current = current ^ hb_hash (وڌ_کان_وڌ);
    current = current * 16777619;
    أرجع current;
  }

  عائم minimum = 0;
  عائم وچ = 0;
  عائم وڌ_کان_وڌ = 0;
};

استعمال rebase_tent_نتيجو_شي_t = hb_pair_t<عائم, Triple>;
استعمال rebase_tent_نتيجو_t = hb_ويڪٽر_t<rebase_tent_نتيجو_شي_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_اندروني عائم renormalize_قدر (عائم v, ثابت Triple &triple,
                                    ثابت TripleDistances &triple_distances,
                                    منطقي extrapolate = سچ);
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
HB_اندروني فراغ rebase_tent (Triple tent,
			      Triple axis_حد_بندي_2,
			      TripleDistances axis_triple_distances,
			      rebase_tent_نتيجو_t &ٻاهريون,
			      rebase_tent_نتيجو_t &scratch);

#جيڪڏهن_پورو /* HB_SUBSET_INSTANCER_SOLVER_HH */
