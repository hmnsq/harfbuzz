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

#अन्तर्भाव "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr स्थिर भिन्न EPSILON = 1.f / (1 << 14);
constexpr स्थिर भिन्न MAX_F2DOT14 = भिन्न (0x7FFF) / (1 << 14);

स्थिर अन्तःस्थ Triple _reverse_negate(ध्रुव Triple &v)
{ प्रत्यागम {-v.maximum, -v.मधला, -v.minimum}; }


स्थिर अन्तःस्थ भिन्न supportScalar (भिन्न coord, ध्रुव Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  भिन्न start = tent.minimum, peak = tent.मधला, end = tent.maximum;

  यदि (unlikely (start > peak || peak > end))
    प्रत्यागम 1.f;
  यदि (unlikely (start < 0 && end > 0 && peak != 0))
    प्रत्यागम 1.f;

  यदि (peak == 0 || coord == peak)
    प्रत्यागम 1.f;

  यदि (coord <= start || end <= coord)
    प्रत्यागम 0.f;

  /* Interpolate */
  यदि (coord < peak)
    प्रत्यागम (coord - start) / (peak - start);
  अन्यथा
    प्रत्यागम  (end - coord) / (end - peak);
}

स्थिर अन्तःस्थ शून्य
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, तार्किक negative = असत्य)
{
  out.reset();
  भिन्न axisMin = axisLimit.minimum;
  भिन्न axisDef = axisLimit.मधला;
  भिन्न axisMax = axisLimit.maximum;
  भिन्न lower = tent.minimum;
  भिन्न peak  = tent.मधला;
  भिन्न upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  यदि (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    प्रति (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    प्रत्यागम;
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
  यदि (axisMax <= lower && axisMax < peak)
      प्रत्यागम;  // No overlap

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
  यदि (axisMax < peak)
  {
    भिन्न mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    प्रति (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    प्रत्यागम;
  }

  // lower <= axisDef <= peak <= axisMax

  भिन्न gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  भिन्न outGain = supportScalar (axisMax, tent);

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
  यदि (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    भिन्न crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    भिन्न scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    यदि (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      भिन्न scalar = outGain;

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
    अन्यथा
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      यदि (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      भिन्न scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      भिन्न scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  अन्यथा
  {
    // Special-case if peak is at axisMax.
    यदि (axisMax == peak)
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
    भिन्न newUpper = peak + (1 - gain) * (upper - peak);
    प्रतिज्ञा (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    यदि (असत्य && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      यदि (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	प्रतिज्ञा (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      भिन्न scalar = 1.f;

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
    अन्यथा
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      भिन्न scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      भिन्न scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      यदि (peak < axisMax)
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
  यदि (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    भिन्न scalar = supportScalar (axisMin, tent);

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
  अन्यथा
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    यदि (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    भिन्न scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    भिन्न scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

स्थिर अन्तःस्थ TripleDistances _reverse_triple_distances (ध्रुव TripleDistances &v)
{ प्रत्यागम TripleDistances (v.positive, v.negative); }

भिन्न renormalize_मूल्य (भिन्न v, ध्रुव Triple &triple,
                         ध्रुव TripleDistances &triple_distances, तार्किक extrapolate)
{
  भिन्न lower = triple.minimum, def = triple.मधला, upper = triple.maximum;
  यदि (unlikely (!(lower <= def && def <= upper)))
    प्रत्यागम hb_clamp (v, -1.f, +1.f);

  यदि (!extrapolate)
    v = hb_clamp (v, lower, upper);

  यदि (v == def)
    प्रत्यागम 0.f;

  यदि (def < 0.f)
    प्रत्यागम -renormalize_मूल्य (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  यदि (v > def)
  {
    भिन्न positive_range = upper - def;
    यदि (unlikely (!positive_range))
      प्रत्यागम +1.f;
    प्रत्यागम (v - def) / positive_range;
  }

  /* v < def */
  यदि (lower >= 0.f)
  {
    भिन्न negative_range = def - lower;
    यदि (unlikely (!negative_range))
      प्रत्यागम -1.f;
    प्रत्यागम (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  भिन्न total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  यदि (unlikely (!total_distance))
    प्रत्यागम 0.f;

  भिन्न v_distance;
  यदि (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  अन्यथा
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  प्रत्यागम (-v_distance) /total_distance;
}

शून्य
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  यदि (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.मधला &&
                  axisLimit.मधला <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.मधला &&
                  tent.मधला <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.मधला == 0.f))
  {
    out.reset ();
    प्रत्यागम;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (भिन्न v) { प्रत्यागम renormalize_मूल्य (v, axisLimit, axis_triple_distances); };

  out.reset();
  प्रति (auto &p : sols)
  {
    यदि (!p.first) अनुवर्त;
    यदि (p.second == Triple{})
    {
      out.push (p);
      अनुवर्त;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.मधला), n (t.maximum)}));
  }
}
