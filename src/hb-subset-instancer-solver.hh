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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_INSTANCER_SOLVER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_INSTANCER_SOLVER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/* pre-normalized distances */
ਬਣਤਰ TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (ਦਸ਼ਮਲਵ neg_, ਦਸ਼ਮਲਵ pos_): negative (neg_), positive (pos_) {}
  TripleDistances (ਦਸ਼ਮਲਵ min, ਦਸ਼ਮਲਵ default_, ਦਸ਼ਮਲਵ max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  ਦਸ਼ਮਲਵ negative;
  ਦਸ਼ਮਲਵ positive;
};

ਬਣਤਰ Triple
{
  Triple () = ਮੂਲ;

  Triple (ਦਸ਼ਮਲਵ minimum_, ਦਸ਼ਮਲਵ ਵਿਚਕਾਰ_2, ਦਸ਼ਮਲਵ ਵੱਧ_ਤੋਂ_ਵੱਧ_2) :
    minimum (minimum_), ਵਿਚਕਾਰ (ਵਿਚਕਾਰ_2), ਵੱਧ_ਤੋਂ_ਵੱਧ (ਵੱਧ_ਤੋਂ_ਵੱਧ_2) {}

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ Triple &o) ਅਚਲ
  {
    ਵਾਪਸ minimum == o.minimum &&
	   ਵਿਚਕਾਰ  == o.ਵਿਚਕਾਰ  &&
	   ਵੱਧ_ਤੋਂ_ਵੱਧ == o.ਵੱਧ_ਤੋਂ_ਵੱਧ;
  }

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ Triple o) ਅਚਲ
  { ਵਾਪਸ !(*ਇਹ == o); }

  ਤਾਰਕਿਕ is_ਬਿੰਦੂ () ਅਚਲ
  { ਵਾਪਸ minimum == ਵਿਚਕਾਰ && ਵਿਚਕਾਰ == ਵੱਧ_ਤੋਂ_ਵੱਧ; }

  ਤਾਰਕਿਕ ਸ਼ਾਮਲ_ਹੈ (ਦਸ਼ਮਲਵ ਬਿੰਦੂ) ਅਚਲ
  { ਵਾਪਸ minimum <= ਬਿੰਦੂ && ਬਿੰਦੂ <= ਵੱਧ_ਤੋਂ_ਵੱਧ; }

  /* from hb_array_t hash ()*/
  uint32_t hash () ਅਚਲ
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (ਵਿਚਕਾਰ);
    current = current * 16777619;

    current = current ^ hb_hash (ਵੱਧ_ਤੋਂ_ਵੱਧ);
    current = current * 16777619;
    ਵਾਪਸ current;
  }

  ਦਸ਼ਮਲਵ minimum = 0;
  ਦਸ਼ਮਲਵ ਵਿਚਕਾਰ = 0;
  ਦਸ਼ਮਲਵ ਵੱਧ_ਤੋਂ_ਵੱਧ = 0;
};

ਵਰਤੋਂ rebase_tent_ਨਤੀਜਾ_ਵਸਤੂ_t = hb_pair_t<ਦਸ਼ਮਲਵ, Triple>;
ਵਰਤੋਂ rebase_tent_ਨਤੀਜਾ_t = hb_ਸਦਿਸ਼_t<rebase_tent_ਨਤੀਜਾ_ਵਸਤੂ_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_ਅੰਦਰੂਨੀ ਦਸ਼ਮਲਵ renormalize_ਮੁੱਲ (ਦਸ਼ਮਲਵ v, ਅਚਲ Triple &triple,
                                    ਅਚਲ TripleDistances &triple_distances,
                                    ਤਾਰਕਿਕ extrapolate = ਸੱਚ);
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
HB_ਅੰਦਰੂਨੀ ਖਾਲੀ rebase_tent (Triple tent,
			      Triple axis_ਹੱਦਬੰਦੀ_2,
			      TripleDistances axis_triple_distances,
			      rebase_tent_ਨਤੀਜਾ_t &ਬਾਹਰਲਾ,
			      rebase_tent_ਨਤੀਜਾ_t &scratch);

#ਜੇ_ਅੰਤ /* HB_SUBSET_INSTANCER_SOLVER_HH */
