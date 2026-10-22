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

#ضمن "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ساكن عائم EPSILON = 1.f / (1 << 14);
constexpr ساكن عائم MAX_F2DOT14 = عائم (0x7FFF) / (1 << 14);

ساكن مضمن Triple _reverse_negate(ثابت Triple &v)
{ ܦܢܐ {-v.maximum, -v.middle, -v.minimum}; }


ساكن مضمن عائم supportScalar (عائم coord, ثابت Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  عائم start = tent.minimum, peak = tent.middle, end = tent.maximum;

  ܐܢ (unlikely (start > peak || peak > end))
    ܦܢܐ 1.f;
  ܐܢ (unlikely (start < 0 && end > 0 && peak != 0))
    ܦܢܐ 1.f;

  ܐܢ (peak == 0 || coord == peak)
    ܦܢܐ 1.f;

  ܐܢ (coord <= start || end <= coord)
    ܦܢܐ 0.f;

  /* Interpolate */
  ܐܢ (coord < peak)
    ܦܢܐ (coord - start) / (peak - start);
  ܐܚܪܢܐ
    ܦܢܐ  (end - coord) / (end - peak);
}

ساكن مضمن فراغ
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, منطقي negative = ܕܓܠܐ)
{
  out.reset();
  عائم axisMin = axisLimit.minimum;
  عائم axisDef = axisLimit.middle;
  عائم axisMax = axisLimit.maximum;
  عائم lower = tent.minimum;
  عائم peak  = tent.middle;
  عائم upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  ܐܢ (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    لكل (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    ܦܢܐ;
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
  ܐܢ (axisMax <= lower && axisMax < peak)
      ܦܢܐ;  // No overlap

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
  ܐܢ (axisMax < peak)
  {
    عائم mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    لكل (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    ܦܢܐ;
  }

  // lower <= axisDef <= peak <= axisMax

  عائم gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  عائم outGain = supportScalar (axisMax, tent);

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
  ܐܢ (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    عائم crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    عائم scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ܐܢ (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      عائم scalar = outGain;

      out.push (hb_pair (scalar - gain, loc));
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
    ܐܚܪܢܐ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ܐܢ (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      عائم scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      عائم scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ܐܚܪܢܐ
  {
    // Special-case if peak is at axisMax.
    ܐܢ (axisMax == peak)
	upper = peak;

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
    عائم newUpper = peak + (1 - gain) * (upper - peak);
    أكد (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ܐܢ (ܕܓܠܐ && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      ܐܢ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	أكد (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      عائم scalar = 1.f;

      out.push (hb_pair (scalar - gain, loc));
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
    ܐܚܪܢܐ
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      عائم scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      عائم scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ܐܢ (peak < axisMax)
	out.push (hb_pair (scalar2 - gain, loc2));
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
  ܐܢ (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    عائم scalar = supportScalar (axisMin, tent);

    out.push (hb_pair (scalar - gain, loc));
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
  ܐܚܪܢܐ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ܐܢ (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    عائم scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    عائم scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

ساكن مضمن TripleDistances _reverse_triple_distances (ثابت TripleDistances &v)
{ ܦܢܐ TripleDistances (v.positive, v.negative); }

عائم renormalizeValue (عائم v, ثابت Triple &triple,
                         ثابت TripleDistances &triple_distances, منطقي extrapolate)
{
  عائم lower = triple.minimum, def = triple.middle, upper = triple.maximum;
  ܐܢ (unlikely (!(lower <= def && def <= upper)))
    ܦܢܐ hb_clamp (v, -1.f, +1.f);

  ܐܢ (!extrapolate)
    v = hb_clamp (v, lower, upper);

  ܐܢ (v == def)
    ܦܢܐ 0.f;

  ܐܢ (def < 0.f)
    ܦܢܐ -renormalizeValue (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ܐܢ (v > def)
  {
    عائم positive_range = upper - def;
    ܐܢ (unlikely (!positive_range))
      ܦܢܐ +1.f;
    ܦܢܐ (v - def) / positive_range;
  }

  /* v < def */
  ܐܢ (lower >= 0.f)
  {
    عائم negative_range = def - lower;
    ܐܢ (unlikely (!negative_range))
      ܦܢܐ -1.f;
    ܦܢܐ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  عائم total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  ܐܢ (unlikely (!total_distance))
    ܦܢܐ 0.f;

  عائم v_distance;
  ܐܢ (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  ܐܚܪܢܐ
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  ܦܢܐ (-v_distance) /total_distance;
}

فراغ
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  ܐܢ (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.middle &&
                  axisLimit.middle <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.middle &&
                  tent.middle <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.middle == 0.f))
  {
    out.reset ();
    ܦܢܐ;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (عائم v) { ܦܢܐ renormalizeValue (v, axisLimit, axis_triple_distances); };

  out.reset();
  لكل (auto &p : sols)
  {
    ܐܢ (!p.first) استمر;
    ܐܢ (p.second == Triple{})
    {
      out.push (p);
      استمر;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.middle), n (t.maximum)}));
  }
}
