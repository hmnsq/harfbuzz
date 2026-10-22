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

#اگر_غیر_تعریف_شدہ HB_SUBSET_INSTANCER_SOLVER_HH
#تعریف_کرو HB_SUBSET_INSTANCER_SOLVER_HH

#شامل "hb.hh"

/* pre-normalized distances */
ساخت TripleDistances
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

ساخت Triple
{
  Triple () = طےشدہ;

  Triple (اعشاری minimum_, اعشاری درمیان_2, اعشاری زیادہ_سے_زیادہ_2) :
    minimum (minimum_), درمیان (درمیان_2), زیادہ_سے_زیادہ (زیادہ_سے_زیادہ_2) {}

  منطقی عامل == (ثابت Triple &o) ثابت
  {
    واپس minimum == o.minimum &&
	   درمیان  == o.درمیان  &&
	   زیادہ_سے_زیادہ == o.زیادہ_سے_زیادہ;
  }

  منطقی عامل != (ثابت Triple o) ثابت
  { واپس !(*یہ == o); }

  منطقی is_نقطہ () ثابت
  { واپس minimum == درمیان && درمیان == زیادہ_سے_زیادہ; }

  منطقی شامل_ہے (اعشاری نقطہ) ثابت
  { واپس minimum <= نقطہ && نقطہ <= زیادہ_سے_زیادہ; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ثابت
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (درمیان);
    current = current * 16777619;

    current = current ^ hb_hash (زیادہ_سے_زیادہ);
    current = current * 16777619;
    واپس current;
  }

  اعشاری minimum = 0;
  اعشاری درمیان = 0;
  اعشاری زیادہ_سے_زیادہ = 0;
};

استعمال rebase_tent_نتیجہ_شے_t = hb_pair_t<اعشاری, Triple>;
استعمال rebase_tent_نتیجہ_t = hb_سمتیہ_t<rebase_tent_نتیجہ_شے_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_اندرونی اعشاری renormalize_قدر (اعشاری v, ثابت Triple &triple,
                                    ثابت TripleDistances &triple_distances,
                                    منطقی extrapolate = سچ);
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
HB_اندرونی خالی rebase_tent (Triple tent,
			      Triple axis_حد_بندی_2,
			      TripleDistances axis_triple_distances,
			      rebase_tent_نتیجہ_t &برآمد,
			      rebase_tent_نتیجہ_t &scratch);

#اگر_ختم /* HB_SUBSET_INSTANCER_SOLVER_HH */
