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

#నిర్వచితం_కాకపోతే HB_SUBSET_INSTANCER_SOLVER_HH
#నిర్వచించు HB_SUBSET_INSTANCER_SOLVER_HH

#అన్తర్భావ "hb.hh"

/* pre-normalized distances */
నిర్మాణం TripleDistances
{
  TripleDistances (): negative (1.f), positive (1.f) {}
  TripleDistances (దశాంశం neg_, దశాంశం pos_): negative (neg_), positive (pos_) {}
  TripleDistances (దశాంశం min, దశాంశం default_, దశాంశం max)
  {
    negative = default_ - min;
    positive = max - default_;
  }

  దశాంశం negative;
  దశాంశం positive;
};

నిర్మాణం Triple
{
  Triple () = అప్రమేయ;

  Triple (దశాంశం minimum_, దశాంశం మధ్య_2, దశాంశం గరిష్ఠ_2) :
    minimum (minimum_), మధ్య (మధ్య_2), గరిష్ఠ (గరిష్ఠ_2) {}

  తార్కిక క్రియాకారకం == (స్థిరరాశి Triple &o) స్థిరరాశి
  {
    తిరిగి minimum == o.minimum &&
	   మధ్య  == o.మధ్య  &&
	   గరిష్ఠ == o.గరిష్ఠ;
  }

  తార్కిక క్రియాకారకం != (స్థిరరాశి Triple o) స్థిరరాశి
  { తిరిగి !(*ఇది == o); }

  తార్కిక is_బిందువు () స్థిరరాశి
  { తిరిగి minimum == మధ్య && మధ్య == గరిష్ఠ; }

  తార్కిక కలిగియుంది (దశాంశం బిందువు) స్థిరరాశి
  { తిరిగి minimum <= బిందువు && బిందువు <= గరిష్ఠ; }

  /* from hb_array_t hash ()*/
  uint32_t hash () స్థిరరాశి
  {
    uint32_t current = /*cbf29ce4*/0x84222325;
    current = current ^ hb_hash (minimum);
    current = current * 16777619;

    current = current ^ hb_hash (మధ్య);
    current = current * 16777619;

    current = current ^ hb_hash (గరిష్ఠ);
    current = current * 16777619;
    తిరిగి current;
  }

  దశాంశం minimum = 0;
  దశాంశం మధ్య = 0;
  దశాంశం గరిష్ఠ = 0;
};

ఉపయోగించి rebase_tent_ఫలితం_అంశం_t = hb_pair_t<దశాంశం, Triple>;
ఉపయోగించి rebase_tent_ఫలితం_t = hb_సదిశ_t<rebase_tent_ఫలితం_అంశం_t>;

/* renormalize a normalized value v to the range of an axis,
 * considering the prenormalized distances as well as the new axis limits.
 * Ported from fonttools */
HB_అంతర్గత దశాంశం renormalize_విలువ (దశాంశం v, స్థిరరాశి Triple &triple,
                                    స్థిరరాశి TripleDistances &triple_distances,
                                    తార్కిక extrapolate = నిజం);
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
HB_అంతర్గత శూన్య rebase_tent (Triple tent,
			      Triple axis_హద్దు_2,
			      TripleDistances axis_triple_distances,
			      rebase_tent_ఫలితం_t &బహిర్గతం,
			      rebase_tent_ఫలితం_t &scratch);

#అయితే_ముగింపు /* HB_SUBSET_INSTANCER_SOLVER_HH */
