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

#अगर_अघोषित HB_SUBSET_INSTANCER_SOLVER_HH
#घोषणा HB_SUBSET_INSTANCER_SOLVER_HH

#समावेश "hb.hh"

/* pre-normalized distances */
काष्ठा TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (भग्न neg_, भग्न pos_): negative (neg_), positive (pos_) {}
  TripleDistances (भग्न min, भग्न default_, भग्न max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  भग्न negative;
  भग्न positive;
};

काष्ठा Triple
{
  Triple () = शेष;

  Triple (भग्न minimum_, भग्न मध्य_2, भग्न maximum_) :
    minimum (minimum_), मध्य (मध्य_2), maximum (maximum_) {}

  bool चालक == (स्थिर Triple &o) स्थिर
  {
    वापस minimum == o.minimum &&
	   मध्य  == o.मध्य  &&
	   maximum == o.maximum;
  }

  bool चालक != (स्थिर Triple o) स्थिर
  { वापस !(*यह == o); }

  bool is_point () स्थिर
  { वापस minimum == मध्य && मध्य == maximum; }

  bool contains (भग्न point) स्थिर
  { वापस minimum <= point && point <= maximum; }

  /* from hb_array_t hash ()*/
  uint32_t hash () स्थिर
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (मध्य);
    current = current * 16777619;

    current = current ^ hb_hash (maximum);
    current = current * 16777619;
    वापस current;
  }

  भग्न minimum = 0;
  भग्न मध्य = 0;
  भग्न maximum = 0;
};

using rebase_tent_परिणाम_item_t = hb_pair_t<भग्न, Triple>;
using rebase_tent_परिणाम_t = hb_vector_t<rebase_tent_परिणाम_item_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_INTERNAL भग्न renormalize_मूल्य (भग्न v, स्थिर Triple &triple,
                                    स्थिर TripleDistances &triple_distances,
                                    bool extrapolate = true);
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
HB_INTERNAL व्योम rebase_tent (Triple tent,
			      Triple axisLimit,
			      TripleDistances axis_triple_distances,
			      rebase_tent_परिणाम_t &out,
			      rebase_tent_परिणाम_t &scratch);

#पूर्ण_अगर /* HB_SUBSET_INSTANCER_SOLVER_HH */
