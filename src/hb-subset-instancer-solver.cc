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

constexpr स्थिर दशांश EPSILON = 1.f / (1 << 14);
constexpr स्थिर दशांश MAX_F2DOT14 = दशांश (0x7FFF) / (1 << 14);

स्थिर ओळीत Triple _reverse_negate(स्थिरांक Triple &v)
{ परत {-v.maximum, -v.मधला, -v.minimum}; }


स्थिर ओळीत दशांश supportScalar (दशांश coord, स्थिरांक Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  दशांश start = tent.minimum, peak = tent.मधला, end = tent.maximum;

  जर (unlikely (start > peak || peak > end))
    परत 1.f;
  जर (unlikely (start < 0 && end > 0 && peak != 0))
    परत 1.f;

  जर (peak == 0 || coord == peak)
    परत 1.f;

  जर (coord <= start || end <= coord)
    परत 0.f;

  /* Interpolate */
  जर (coord < peak)
    परत (coord - start) / (peak - start);
  अन्यथा
    परत  (end - coord) / (end - peak);
}

स्थिर ओळीत शून्य
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, तार्किक negative = असत्य)
{
  out.reset();
  दशांश axisMin = axisLimit.minimum;
  दशांश axisDef = axisLimit.मधला;
  दशांश axisMax = axisLimit.maximum;
  दशांश lower = tent.minimum;
  दशांश peak  = tent.मधला;
  दशांश upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  जर (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    साठी (स्वतः &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    परत;
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
  जर (axisMax <= lower && axisMax < peak)
      परत;  // No overlap

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
  जर (axisMax < peak)
  {
    दशांश mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    साठी (स्वतः &p : out)
      p = hb_pair (p.first * mult, p.second);

    परत;
  }

  // lower <= axisDef <= peak <= axisMax

  दशांश gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  दशांश outGain = supportScalar (axisMax, tent);

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
  जर (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    दशांश crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    दशांश scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    जर (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      दशांश scalar = outGain;

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
      जर (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      दशांश scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      दशांश scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  अन्यथा
  {
    // Special-case if peak is at axisMax.
    जर (axisMax == peak)
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
    दशांश newUpper = peak + (1 - gain) * (upper - peak);
    प्रतिज्ञा (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    जर (असत्य && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      जर (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	प्रतिज्ञा (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      दशांश scalar = 1.f;

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
      दशांश scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      दशांश scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      जर (peak < axisMax)
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
  जर (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    दशांश scalar = supportScalar (axisMin, tent);

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
    जर (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    दशांश scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    दशांश scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

स्थिर ओळीत TripleDistances _reverse_triple_distances (स्थिरांक TripleDistances &v)
{ परत TripleDistances (v.positive, v.negative); }

दशांश renormalize_मूल्य (दशांश v, स्थिरांक Triple &triple,
                         स्थिरांक TripleDistances &triple_distances, तार्किक extrapolate)
{
  दशांश lower = triple.minimum, def = triple.मधला, upper = triple.maximum;
  जर (unlikely (!(lower <= def && def <= upper)))
    परत hb_clamp (v, -1.f, +1.f);

  जर (!extrapolate)
    v = hb_clamp (v, lower, upper);

  जर (v == def)
    परत 0.f;

  जर (def < 0.f)
    परत -renormalize_मूल्य (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  जर (v > def)
  {
    दशांश positive_range = upper - def;
    जर (unlikely (!positive_range))
      परत +1.f;
    परत (v - def) / positive_range;
  }

  /* v < def */
  जर (lower >= 0.f)
  {
    दशांश negative_range = def - lower;
    जर (unlikely (!negative_range))
      परत -1.f;
    परत (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  दशांश total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  जर (unlikely (!total_distance))
    परत 0.f;

  दशांश v_distance;
  जर (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  अन्यथा
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  परत (-v_distance) /total_distance;
}

शून्य
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  जर (unlikely (!(-1.f <= axisLimit.minimum &&
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
    परत;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  स्वतः n = [&axisLimit, &axis_triple_distances] (दशांश v) { परत renormalize_मूल्य (v, axisLimit, axis_triple_distances); };

  out.reset();
  साठी (स्वतः &p : sols)
  {
    जर (!p.first) सुरूठेवा;
    जर (p.second == Triple{})
    {
      out.push (p);
      सुरूठेवा;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.मधला), n (t.maximum)}));
  }
}
