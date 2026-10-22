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

#समावेश "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr अटल भग्न EPSILON = 1.f / (1 << 14);
constexpr अटल भग्न MAX_F2DOT14 = भग्न (0x7FFF) / (1 << 14);

अटल अंतरभूत Triple _reverse_negate(स्थिर Triple &v)
{ वापस {-v.maximum, -v.मध्य, -v.minimum}; }


अटल अंतरभूत भग्न supportScalar (भग्न coord, स्थिर Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  भग्न start = tent.minimum, peak = tent.मध्य, end = tent.maximum;

  अगर (unlikely (start > peak || peak > end))
    वापस 1.f;
  अगर (unlikely (start < 0 && end > 0 && peak != 0))
    वापस 1.f;

  अगर (peak == 0 || coord == peak)
    वापस 1.f;

  अगर (coord <= start || end <= coord)
    वापस 0.f;

  /* Interpolate */
  अगर (coord < peak)
    वापस (coord - start) / (peak - start);
  अन्यथा
    वापस  (end - coord) / (end - peak);
}

अटल अंतरभूत व्योम
_solve (Triple tent, Triple axisLimit, rebase_tent_परिणाम_t &out, bool negative = false)
{
  out.reset();
  भग्न axisMin = axisLimit.minimum;
  भग्न axisDef = axisLimit.मध्य;
  भग्न axisMax = axisLimit.maximum;
  भग्न lower = tent.minimum;
  भग्न peak  = tent.मध्य;
  भग्न upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  अगर (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    क्रम (स्वतः &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    वापस;
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
  अगर (axisMax <= lower && axisMax < peak)
      वापस;  // No overlap

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
  अगर (axisMax < peak)
  {
    भग्न mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    क्रम (स्वतः &p : out)
      p = hb_pair (p.first * mult, p.second);

    वापस;
  }

  // lower <= axisDef <= peak <= axisMax

  भग्न gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  भग्न outGain = supportScalar (axisMax, tent);

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
  अगर (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    भग्न crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    भग्न scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    अगर (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      भग्न scalar = outGain;

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
      अगर (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      भग्न scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      भग्न scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  अन्यथा
  {
    // Special-case if peak is at axisMax.
    अगर (axisMax == peak)
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
    भग्न newUpper = peak + (1 - gain) * (upper - peak);
    निश्चित (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    अगर (false && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      अगर (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	निश्चित (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      भग्न scalar = 1.f;

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
      भग्न scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      भग्न scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      अगर (peak < axisMax)
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
  अगर (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    भग्न scalar = supportScalar (axisMin, tent);

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
    अगर (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    भग्न scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    भग्न scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

अटल अंतरभूत TripleDistances _reverse_triple_distances (स्थिर TripleDistances &v)
{ वापस TripleDistances (v.positive, v.negative); }

भग्न renormalize_मूल्य (भग्न v, स्थिर Triple &triple,
                         स्थिर TripleDistances &triple_distances, bool extrapolate)
{
  भग्न lower = triple.minimum, def = triple.मध्य, upper = triple.maximum;
  अगर (unlikely (!(lower <= def && def <= upper)))
    वापस hb_clamp (v, -1.f, +1.f);

  अगर (!extrapolate)
    v = hb_clamp (v, lower, upper);

  अगर (v == def)
    वापस 0.f;

  अगर (def < 0.f)
    वापस -renormalize_मूल्य (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  अगर (v > def)
  {
    भग्न positive_range = upper - def;
    अगर (unlikely (!positive_range))
      वापस +1.f;
    वापस (v - def) / positive_range;
  }

  /* v < def */
  अगर (lower >= 0.f)
  {
    भग्न negative_range = def - lower;
    अगर (unlikely (!negative_range))
      वापस -1.f;
    वापस (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  भग्न योग_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  अगर (unlikely (!योग_distance))
    वापस 0.f;

  भग्न v_distance;
  अगर (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  अन्यथा
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  वापस (-v_distance) /योग_distance;
}

व्योम
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_परिणाम_t &out,
	     rebase_tent_परिणाम_t &scratch)
{
  अगर (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.मध्य &&
                  axisLimit.मध्य <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.मध्य &&
                  tent.मध्य <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.मध्य == 0.f))
  {
    out.reset ();
    वापस;
  }

  rebase_tent_परिणाम_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  स्वतः n = [&axisLimit, &axis_triple_distances] (भग्न v) { वापस renormalize_मूल्य (v, axisLimit, axis_triple_distances); };

  out.reset();
  क्रम (स्वतः &p : sols)
  {
    अगर (!p.first) जारी;
    अगर (p.second == Triple{})
    {
      out.push (p);
      जारी;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.मध्य), n (t.maximum)}));
  }
}
