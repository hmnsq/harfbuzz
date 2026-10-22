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

#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ಸ್ಥಿರ ಭಿನ್ನ EPSILON = 1.f / (1 << 14);
constexpr ಸ್ಥಿರ ಭಿನ್ನ MAX_F2DOT14 = ಭಿನ್ನ (0x7FFF) / (1 << 14);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Triple _reverse_negate(ಧ್ರುವ Triple &v)
{ ಪ್ರತ್ಯಾಗಮ {-v.maximum, -v.ಮಧ್ಯ, -v.minimum}; }


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಭಿನ್ನ supportScalar (ಭಿನ್ನ coord, ಧ್ರುವ Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  ಭಿನ್ನ start = tent.minimum, peak = tent.ಮಧ್ಯ, end = tent.maximum;

  ಯದಿ (unlikely (start > peak || peak > end))
    ಪ್ರತ್ಯಾಗಮ 1.f;
  ಯದಿ (unlikely (start < 0 && end > 0 && peak != 0))
    ಪ್ರತ್ಯಾಗಮ 1.f;

  ಯದಿ (peak == 0 || coord == peak)
    ಪ್ರತ್ಯಾಗಮ 1.f;

  ಯದಿ (coord <= start || end <= coord)
    ಪ್ರತ್ಯಾಗಮ 0.f;

  /* Interpolate */
  ಯದಿ (coord < peak)
    ಪ್ರತ್ಯಾಗಮ (coord - start) / (peak - start);
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ  (end - coord) / (end - peak);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, ತಾರ್ಕಿಕ negative = ಅಸತ್ಯ)
{
  out.reset();
  ಭಿನ್ನ axisMin = axisLimit.minimum;
  ಭಿನ್ನ axisDef = axisLimit.ಮಧ್ಯ;
  ಭಿನ್ನ axisMax = axisLimit.maximum;
  ಭಿನ್ನ lower = tent.minimum;
  ಭಿನ್ನ peak  = tent.ಮಧ್ಯ;
  ಭಿನ್ನ upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  ಯದಿ (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    ಪ್ರತಿ (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    ಪ್ರತ್ಯಾಗಮ;
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
  ಯದಿ (axisMax <= lower && axisMax < peak)
      ಪ್ರತ್ಯಾಗಮ;  // No overlap

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
  ಯದಿ (axisMax < peak)
  {
    ಭಿನ್ನ mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    ಪ್ರತಿ (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    ಪ್ರತ್ಯಾಗಮ;
  }

  // lower <= axisDef <= peak <= axisMax

  ಭಿನ್ನ gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  ಭಿನ್ನ outGain = supportScalar (axisMax, tent);

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
  ಯದಿ (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    ಭಿನ್ನ crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    ಭಿನ್ನ scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ಯದಿ (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      ಭಿನ್ನ scalar = outGain;

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
    ಅನ್ಯಥಾ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ಯದಿ (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      ಭಿನ್ನ scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      ಭಿನ್ನ scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ಅನ್ಯಥಾ
  {
    // Special-case if peak is at axisMax.
    ಯದಿ (axisMax == peak)
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
    ಭಿನ್ನ newUpper = peak + (1 - gain) * (upper - peak);
    ಪ್ರತಿಜ್ಞಾ (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ಯದಿ (ಅಸತ್ಯ && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      ಯದಿ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	ಪ್ರತಿಜ್ಞಾ (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      ಭಿನ್ನ scalar = 1.f;

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
    ಅನ್ಯಥಾ
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      ಭಿನ್ನ scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      ಭಿನ್ನ scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ಯದಿ (peak < axisMax)
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
  ಯದಿ (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    ಭಿನ್ನ scalar = supportScalar (axisMin, tent);

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
  ಅನ್ಯಥಾ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ಯದಿ (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    ಭಿನ್ನ scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    ಭಿನ್ನ scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ TripleDistances _reverse_triple_distances (ಧ್ರುವ TripleDistances &v)
{ ಪ್ರತ್ಯಾಗಮ TripleDistances (v.positive, v.negative); }

ಭಿನ್ನ renormalize_ಮೌಲ್ಯ (ಭಿನ್ನ v, ಧ್ರುವ Triple &triple,
                         ಧ್ರುವ TripleDistances &triple_distances, ತಾರ್ಕಿಕ extrapolate)
{
  ಭಿನ್ನ lower = triple.minimum, def = triple.ಮಧ್ಯ, upper = triple.maximum;
  ಯದಿ (unlikely (!(lower <= def && def <= upper)))
    ಪ್ರತ್ಯಾಗಮ hb_clamp (v, -1.f, +1.f);

  ಯದಿ (!extrapolate)
    v = hb_clamp (v, lower, upper);

  ಯದಿ (v == def)
    ಪ್ರತ್ಯಾಗಮ 0.f;

  ಯದಿ (def < 0.f)
    ಪ್ರತ್ಯಾಗಮ -renormalize_ಮೌಲ್ಯ (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ಯದಿ (v > def)
  {
    ಭಿನ್ನ positive_range = upper - def;
    ಯದಿ (unlikely (!positive_range))
      ಪ್ರತ್ಯಾಗಮ +1.f;
    ಪ್ರತ್ಯಾಗಮ (v - def) / positive_range;
  }

  /* v < def */
  ಯದಿ (lower >= 0.f)
  {
    ಭಿನ್ನ negative_range = def - lower;
    ಯದಿ (unlikely (!negative_range))
      ಪ್ರತ್ಯಾಗಮ -1.f;
    ಪ್ರತ್ಯಾಗಮ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  ಭಿನ್ನ total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  ಯದಿ (unlikely (!total_distance))
    ಪ್ರತ್ಯಾಗಮ 0.f;

  ಭಿನ್ನ v_distance;
  ಯದಿ (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  ಅನ್ಯಥಾ
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  ಪ್ರತ್ಯಾಗಮ (-v_distance) /total_distance;
}

ಶೂನ್ಯ
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  ಯದಿ (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.ಮಧ್ಯ &&
                  axisLimit.ಮಧ್ಯ <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.ಮಧ್ಯ &&
                  tent.ಮಧ್ಯ <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.ಮಧ್ಯ == 0.f))
  {
    out.reset ();
    ಪ್ರತ್ಯಾಗಮ;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (ಭಿನ್ನ v) { ಪ್ರತ್ಯಾಗಮ renormalize_ಮೌಲ್ಯ (v, axisLimit, axis_triple_distances); };

  out.reset();
  ಪ್ರತಿ (auto &p : sols)
  {
    ಯದಿ (!p.first) ಅನುವರ್ತ;
    ಯದಿ (p.second == Triple{})
    {
      out.push (p);
      ಅನುವರ್ತ;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.ಮಧ್ಯ), n (t.maximum)}));
  }
}
