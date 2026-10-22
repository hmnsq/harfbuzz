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

#అన్తర్భావ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr స్థిర దశాంశం EPSILON = 1.f / (1 << 14);
constexpr స్థిర దశాంశం MAX_F2DOT14 = దశాంశం (0x7FFF) / (1 << 14);

స్థిర అంతఃవరుస Triple తిరగేయి_negate(స్థిరరాశి Triple &v)
{ తిరిగి {-v.గరిష్ఠ, -v.మధ్య, -v.minimum}; }


స్థిర అంతఃవరుస దశాంశం supportScalar (దశాంశం coord, స్థిరరాశి Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  దశాంశం ప్రారంభం = tent.minimum, peak = tent.మధ్య, చివర = tent.గరిష్ఠ;

  ఒకవేళ (unlikely (ప్రారంభం > peak || peak > చివర))
    తిరిగి 1.f;
  ఒకవేళ (unlikely (ప్రారంభం < 0 && చివర > 0 && peak != 0))
    తిరిగి 1.f;

  ఒకవేళ (peak == 0 || coord == peak)
    తిరిగి 1.f;

  ఒకవేళ (coord <= ప్రారంభం || చివర <= coord)
    తిరిగి 0.f;

  /* Interpolate */
  ఒకవేళ (coord < peak)
    తిరిగి (coord - ప్రారంభం) / (peak - ప్రారంభం);
  లేకపోతే
    తిరిగి  (చివర - coord) / (చివర - peak);
}

స్థిర అంతఃవరుస శూన్య
_solve (Triple tent, Triple axis_హద్దు_2, rebase_tent_ఫలితం_t &బహిర్గతం, తార్కిక negative = అబద్ధం)
{
  బహిర్గతం.reset();
  దశాంశం axisMin = axis_హద్దు_2.minimum;
  దశాంశం axisDef = axis_హద్దు_2.మధ్య;
  దశాంశం axisMax = axis_హద్దు_2.గరిష్ఠ;
  దశాంశం క్రింది = tent.minimum;
  దశాంశం peak  = tent.మధ్య;
  దశాంశం పై = tent.గరిష్ఠ;

  // Mirror the problem such that axisDef <= peak
  ఒకవేళ (axisDef > peak)
  {
    _solve (తిరగేయి_negate (tent), తిరగేయి_negate (axis_హద్దు_2), బహిర్గతం, !negative);

    కొరకు (స్వయంచాలక &p : బహిర్గతం)
      p = hb_pair (p.మొదటి, తిరగేయి_negate (p.second));

    తిరిగి;
  }
  // axisDef <= peak

  /* case 1: The whole deltaset falls outside the new limit; we can drop it
   *
   *                                          peak
   *  1.........................................o..........
   *                                           / \
   *                                          /   \
   *                                         /     \
   *                                        /       \
   *  0---|-----------|----------|-------- o         o----1
   *    axisMin     axisDef    axisMax   lower     upper
   */
  ఒకవేళ (axisMax <= క్రింది && axisMax < peak)
      తిరిగి;  // No overlap

  /* case 2: Only the peak and outermost bound fall outside the new limit;
   * we keep the deltaset, update peak and outermost bound and scale deltas
   * by the scalar value for the restricted axis at the new limit, and solve
   * recursively.
   *
   *                                  |peak
   *  1...............................|.o..........
   *                                  |/ \
   *                                  /   \
   *                                 /|    \
   *                                / |     \
   *  0--------------------------- o  |      o----1
   *                           lower  |      upper
   *                                  |
   *                                axisMax
   *
   * Convert to:
   *
   *  1............................................
   *                                  |
   *                                  o peak
   *                                 /|
   *                                /x|
   *  0--------------------------- o  o upper ----1
   *                           lower  |
   *                                  |
   *                                axisMax
   */
  ఒకవేళ (axisMax < peak)
  {
    దశాంశం mult = supportScalar (axisMax, tent);
    tent = Triple{క్రింది, axisMax, axisMax};

    _solve (tent, axis_హద్దు_2, బహిర్గతం);

    కొరకు (స్వయంచాలక &p : బహిర్గతం)
      p = hb_pair (p.మొదటి * mult, p.second);

    తిరిగి;
  }

  // lower <= axisDef <= peak <= axisMax

  దశాంశం gain = supportScalar (axisDef, tent);
  బహిర్గతం.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  దశాంశం బహిర్గతం_Gain = supportScalar (axisMax, tent);

  /* Case 3a: Gain is more than outGain. The tent down-slope crosses
   * the axis into negative. We have to split it into multiples.
   *
   *                      | peak  |
   *  1...................|.o.....|..............
   *                      |/x\_   |
   *  gain................+....+_.|..............
   *                     /|    |y\|
   *  ................../.|....|..+_......outGain
   *                   /  |    |  | \
   *  0---|-----------o   |    |  |  o----------1
   *    axisMin    lower  |    |  |   upper
   *                      |    |  |
   *                axisDef    |  axisMax
   *                           |
   *                      crossing
   */
  ఒకవేళ (gain >= బహిర్గతం_Gain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    దశాంశం దాటడం = peak + (1 - gain) * (పై - peak);

    Triple loc{hb_max (క్రింది, axisDef), peak, దాటడం};
    దశాంశం scalar = 1.f;

    // The part before the crossing point.
    బహిర్గతం.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ఒకవేళ (పై >= axisMax)
    {
      Triple loc {దాటడం, axisMax, axisMax};
      దశాంశం scalar = బహిర్గతం_Gain;

      బహిర్గతం.push (hb_pair (scalar - gain, loc));
    }

    /* Case 3a2: Similar to case 2neg; two tents needed, to keep
     * down to eternity.
     *
     *                      | peak             |
     *  1...................|.o................|...
     *                      |/ \_              |
     *  gain................+....+_............|...
     *                     /|    | \xxxxxxxxxxy|
     *                    / |    |  \_xxxxxyyyy|
     *                   /  |    |    \xxyyyyyy|
     *  0---|-----------o   |    |     o-------|--1
     *    axisMin    lower  |    |      upper  |
     *                      |    |             |
     *                axisDef    |             axisMax
     *                           |
     *                      crossing
     */
    లేకపోతే
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ఒకవేళ (పై == axisDef)
	పై += EPSILON;

      // Downslope.
      Triple loc1 {దాటడం, పై, axisMax};
      దశాంశం scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {పై, axisMax, axisMax};
      దశాంశం scalar2 = 0.f;

      బహిర్గతం.push (hb_pair (scalar1 - gain, loc1));
      బహిర్గతం.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  లేకపోతే
  {
    // Special-case if peak is at axisMax.
    ఒకవేళ (axisMax == peak)
	పై = peak;

    /* Case 3:
     * we keep deltas as is and only scale the axis upper to achieve
     * the desired new tent if feasible.
     *
     *                        peak
     *  1.....................o....................
     *                       / \_|
     *  ..................../....+_.........outGain
     *                     /     | \
     *  gain..............+......|..+_.............
     *                   /|      |  | \
     *  0---|-----------o |      |  |  o----------1
     *    axisMin    lower|      |  |   upper
     *                    |      |  newUpper
     *              axisDef      axisMax
     */
    దశాంశం new_పై = peak + (1 - gain) * (పై - peak);
    ప్రతిజ్ఞా (axisMax <= new_పై);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ఒకవేళ (అబద్ధం && (new_పై <= axisDef + (axisMax - axisDef) * 2))
    {
      పై = new_పై;
      ఒకవేళ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < పై)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	పై = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	ప్రతిజ్ఞా (peak < పై);
      }

      Triple loc {hb_max (axisDef, క్రింది), peak, పై};
      దశాంశం scalar = 1.f;

      బహిర్గతం.push (hb_pair (scalar - gain, loc));
    }

    /* Case 4: New limit doesn't fit; we need to chop into two tents,
     * because the shape of a triangle with part of one side cut off
     * cannot be represented as a triangle itself.
     *
     *            |   peak |
     *  1.........|......o.|....................
     *  ..........|...../x\|.............outGain
     *            |    |xxy|\_
     *            |   /xxxy|  \_
     *            |  |xxxxy|    \_
     *            |  /xxxxy|      \_
     *  0---|-----|-oxxxxxx|        o----------1
     *    axisMin | lower  |        upper
     *            |        |
     *          axisDef  axisMax
     */
    లేకపోతే
    {
      Triple loc1 {hb_max (axisDef, క్రింది), peak, axisMax};
      దశాంశం scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      దశాంశం scalar2 = బహిర్గతం_Gain;

      బహిర్గతం.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ఒకవేళ (peak < axisMax)
	బహిర్గతం.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  /* Now, the negative side
   *
   * Case 1neg: Lower extends beyond axisMin: we chop. Simple.
   *
   *                     |   |peak
   *  1..................|...|.o.................
   *                     |   |/ \
   *  gain...............|...+...\...............
   *                     |x_/|    \
   *                     |/  |     \
   *                   _/|   |      \
   *  0---------------o  |   |       o----------1
   *              lower  |   |       upper
   *                     |   |
   *               axisMin   axisDef
   */
  ఒకవేళ (క్రింది <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    దశాంశం scalar = supportScalar (axisMin, tent);

    బహిర్గతం.push (hb_pair (scalar - gain, loc));
  }

  /* Case 2neg: Lower is betwen axisMin and axisDef: we add two
   * tents to keep it down all the way to eternity.
   *
   *      |               |peak
   *  1...|...............|.o.................
   *      |               |/ \
   *  gain|...............+...\...............
   *      |yxxxxxxxxxxxxx/|    \
   *      |yyyyyyxxxxxxx/ |     \
   *      |yyyyyyyyyyyx/  |      \
   *  0---|-----------o   |       o----------1
   *    axisMin    lower  |       upper
   *                      |
   *                    axisDef
   */
  లేకపోతే
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ఒకవేళ (క్రింది == axisDef)
      క్రింది -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, క్రింది, axisDef};
    దశాంశం scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, క్రింది};
    దశాంశం scalar2 = 0.f;

    బహిర్గతం.push (hb_pair (scalar1 - gain, loc1));
    బహిర్గతం.push (hb_pair (scalar2 - gain, loc2));
  }
}

స్థిర అంతఃవరుస TripleDistances తిరగేయి_triple_distances (స్థిరరాశి TripleDistances &v)
{ తిరిగి TripleDistances (v.positive, v.negative); }

దశాంశం renormalize_విలువ (దశాంశం v, స్థిరరాశి Triple &triple,
                         స్థిరరాశి TripleDistances &triple_distances, తార్కిక extrapolate)
{
  దశాంశం క్రింది = triple.minimum, def = triple.మధ్య, పై = triple.గరిష్ఠ;
  ఒకవేళ (unlikely (!(క్రింది <= def && def <= పై)))
    తిరిగి hb_clamp (v, -1.f, +1.f);

  ఒకవేళ (!extrapolate)
    v = hb_clamp (v, క్రింది, పై);

  ఒకవేళ (v == def)
    తిరిగి 0.f;

  ఒకవేళ (def < 0.f)
    తిరిగి -renormalize_విలువ (-v, తిరగేయి_negate (triple),
                              తిరగేయి_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ఒకవేళ (v > def)
  {
    దశాంశం ధనాత్మక_range = పై - def;
    ఒకవేళ (unlikely (!ధనాత్మక_range))
      తిరిగి +1.f;
    తిరిగి (v - def) / ధనాత్మక_range;
  }

  /* v < def */
  ఒకవేళ (క్రింది >= 0.f)
  {
    దశాంశం negative_range = def - క్రింది;
    ఒకవేళ (unlikely (!negative_range))
      తిరిగి -1.f;
    తిరిగి (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  దశాంశం మొత్తం_దూరం = triple_distances.negative * (-క్రింది) + triple_distances.positive * def;
  ఒకవేళ (unlikely (!మొత్తం_దూరం))
    తిరిగి 0.f;

  దశాంశం v_దూరం;
  ఒకవేళ (v >= 0.f)
    v_దూరం = (def - v) * triple_distances.positive;
  లేకపోతే
    v_దూరం = (-v) * triple_distances.negative + triple_distances.positive * def;

  తిరిగి (-v_దూరం) /మొత్తం_దూరం;
}

శూన్య
rebase_tent (Triple tent, Triple axis_హద్దు_2, TripleDistances axis_triple_distances,
	     rebase_tent_ఫలితం_t &బహిర్గతం,
	     rebase_tent_ఫలితం_t &scratch)
{
  ఒకవేళ (unlikely (!(-1.f <= axis_హద్దు_2.minimum &&
                  axis_హద్దు_2.minimum <= axis_హద్దు_2.మధ్య &&
                  axis_హద్దు_2.మధ్య <= axis_హద్దు_2.గరిష్ఠ &&
                  axis_హద్దు_2.గరిష్ఠ <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.మధ్య &&
                  tent.మధ్య <= tent.గరిష్ఠ &&
                  tent.గరిష్ఠ <= +2.f) ||
                tent.మధ్య == 0.f))
  {
    బహిర్గతం.reset ();
    తిరిగి;
  }

  rebase_tent_ఫలితం_t &sols = scratch;
  _solve (tent, axis_హద్దు_2, sols);

  స్వయంచాలక n = [&axis_హద్దు_2, &axis_triple_distances] (దశాంశం v) { తిరిగి renormalize_విలువ (v, axis_హద్దు_2, axis_triple_distances); };

  బహిర్గతం.reset();
  కొరకు (స్వయంచాలక &p : sols)
  {
    ఒకవేళ (!p.మొదటి) కొనసాగు;
    ఒకవేళ (p.second == Triple{})
    {
      బహిర్గతం.push (p);
      కొనసాగు;
    }
    Triple t = p.second;
    బహిర్గతం.push (hb_pair (p.మొదటి,
		       Triple{n (t.minimum), n (t.మధ్య), n (t.గరిష్ఠ)}));
  }
}
