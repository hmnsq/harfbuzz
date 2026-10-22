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

#অন্তর্ভাব "hb.hh"

/* pre-normalized distances */
কাঠামো TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (দশমিক neg_, দশমিক pos_): negative (neg_), positive (pos_) {}
  TripleDistances (দশমিক min, দশমিক default_, দশমিক max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  দশমিক negative;
  দশমিক positive;
};

কাঠামো Triple
{
  Triple () = পূর্বনির্ধারিত;

  Triple (দশমিক minimum_, দশমিক মধ্য_2, দশমিক maximum_) :
    minimum (minimum_), মধ্য (মধ্য_2), maximum (maximum_) {}

  যৌক্তিক operator == (ধ্রুবক Triple &o) ধ্রুবক
  {
    ফেরত minimum == o.minimum &&
	   মধ্য  == o.মধ্য  &&
	   maximum == o.maximum;
  }

  যৌক্তিক operator != (ধ্রুবক Triple o) ধ্রুবক
  { ফেরত !(*এই == o); }

  যৌক্তিক is_point () ধ্রুবক
  { ফেরত minimum == মধ্য && মধ্য == maximum; }

  যৌক্তিক contains (দশমিক point) ধ্রুবক
  { ফেরত minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ধ্রুবক
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (মধ্য);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    ফেরত current;
  }

  দশমিক minimum = 0;
  দশমিক মধ্য = 0;
  দশমিক maximum = 0;
};

using rebase_tent_ফলাফল_item_t = hb_pair_t<দশমিক, Triple>;
using rebase_tent_ফলাফল_t = hb_vector_t<rebase_tent_ফলাফল_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL দশমিক renormalize_মান (দশমিক v, ধ্রুবক Triple &triple,
                                    ধ্রুবক TripleDistances &triple_distances,
                                    যৌক্তিক extrapolate = সত্য);
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
HB_INTERNAL শূন্য rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_ফলাফল_t &out,
			      rebase_tent_ফলাফল_t &scratch);

#endif /* HB_SUBSET_INSTANCER_SOLVER_HH */
