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

#شامل "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ایستا اعشاری EPSILON = 1.f / (1 << 14);
constexpr ایستا اعشاری MAX_F2DOT14 = اعشاری (0x7FFF) / (1 << 14);

ایستا درونخطی Triple _reverse_negate(ثابت Triple &v)
{ بازگشت {-v.maximum, -v.میانه, -v.minimum}; }


ایستا درونخطی اعشاری supportScalar (اعشاری coord, ثابت Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  اعشاری start = tent.minimum, peak = tent.میانه, end = tent.maximum;

  اگر (unlikely (start > peak || peak > end))
    بازگشت 1.f;
  اگر (unlikely (start < 0 && end > 0 && peak != 0))
    بازگشت 1.f;

  اگر (peak == 0 || coord == peak)
    بازگشت 1.f;

  اگر (coord <= start || end <= coord)
    بازگشت 0.f;

  /* Interpolate */
  اگر (coord < peak)
    بازگشت (coord - start) / (peak - start);
  وگرنه
    بازگشت  (end - coord) / (end - peak);
}

ایستا درونخطی تهی
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, منطقی negative = نادرست)
{
  out.reset();
  اعشاری axisMin = axisLimit.minimum;
  اعشاری axisDef = axisLimit.میانه;
  اعشاری axisMax = axisLimit.maximum;
  اعشاری lower = tent.minimum;
  اعشاری peak  = tent.میانه;
  اعشاری upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  اگر (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    برای (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    بازگشت;
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
  اگر (axisMax <= lower && axisMax < peak)
      بازگشت;  // No overlap

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
  اگر (axisMax < peak)
  {
    اعشاری mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    برای (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    بازگشت;
  }

  // lower <= axisDef <= peak <= axisMax

  اعشاری gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  اعشاری outGain = supportScalar (axisMax, tent);

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
  اگر (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    اعشاری crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    اعشاری scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    اگر (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      اعشاری scalar = outGain;

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
    وگرنه
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      اگر (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      اعشاری scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      اعشاری scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  وگرنه
  {
    // Special-case if peak is at axisMax.
    اگر (axisMax == peak)
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
    اعشاری newUpper = peak + (1 - gain) * (upper - peak);
    أكد (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    اگر (نادرست && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      اگر (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	أكد (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      اعشاری scalar = 1.f;

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
    وگرنه
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      اعشاری scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      اعشاری scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      اگر (peak < axisMax)
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
  اگر (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    اعشاری scalar = supportScalar (axisMin, tent);

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
  وگرنه
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    اگر (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    اعشاری scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    اعشاری scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

ایستا درونخطی TripleDistances _reverse_triple_distances (ثابت TripleDistances &v)
{ بازگشت TripleDistances (v.positive, v.negative); }

اعشاری renormalize_مقدار (اعشاری v, ثابت Triple &triple,
                         ثابت TripleDistances &triple_distances, منطقی extrapolate)
{
  اعشاری lower = triple.minimum, def = triple.میانه, upper = triple.maximum;
  اگر (unlikely (!(lower <= def && def <= upper)))
    بازگشت hb_clamp (v, -1.f, +1.f);

  اگر (!extrapolate)
    v = hb_clamp (v, lower, upper);

  اگر (v == def)
    بازگشت 0.f;

  اگر (def < 0.f)
    بازگشت -renormalize_مقدار (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  اگر (v > def)
  {
    اعشاری positive_range = upper - def;
    اگر (unlikely (!positive_range))
      بازگشت +1.f;
    بازگشت (v - def) / positive_range;
  }

  /* v < def */
  اگر (lower >= 0.f)
  {
    اعشاری negative_range = def - lower;
    اگر (unlikely (!negative_range))
      بازگشت -1.f;
    بازگشت (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  اعشاری total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  اگر (unlikely (!total_distance))
    بازگشت 0.f;

  اعشاری v_distance;
  اگر (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  وگرنه
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  بازگشت (-v_distance) /total_distance;
}

تهی
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  اگر (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.میانه &&
                  axisLimit.میانه <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.میانه &&
                  tent.میانه <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.میانه == 0.f))
  {
    out.reset ();
    بازگشت;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (اعشاری v) { بازگشت renormalize_مقدار (v, axisLimit, axis_triple_distances); };

  out.reset();
  برای (auto &p : sols)
  {
    اگر (!p.first) ادامه;
    اگر (p.second == Triple{})
    {
      out.push (p);
      ادامه;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.میانه), n (t.maximum)}));
  }
}
