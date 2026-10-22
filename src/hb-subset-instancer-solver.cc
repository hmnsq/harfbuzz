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

#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ସ୍ଥିର ଦଶମିକ EPSILON = 1.f / (1 << 14);
constexpr ସ୍ଥିର ଦଶମିକ MAX_F2DOT14 = ଦଶମିକ (0x7FFF) / (1 << 14);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ Triple _reverse_negate(ଅଚଳ Triple &v)
{ ଫେରନ୍ତୁ {-v.maximum, -v.ମଧ୍ୟ, -v.minimum}; }


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଦଶମିକ supportScalar (ଦଶମିକ coord, ଅଚଳ Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  ଦଶମିକ start = tent.minimum, peak = tent.ମଧ୍ୟ, end = tent.maximum;

  ଯଦି (unlikely (start > peak || peak > end))
    ଫେରନ୍ତୁ 1.f;
  ଯଦି (unlikely (start < 0 && end > 0 && peak != 0))
    ଫେରନ୍ତୁ 1.f;

  ଯଦି (peak == 0 || coord == peak)
    ଫେରନ୍ତୁ 1.f;

  ଯଦି (coord <= start || end <= coord)
    ଫେରନ୍ତୁ 0.f;

  /* Interpolate */
  ଯଦି (coord < peak)
    ଫେରନ୍ତୁ (coord - start) / (peak - start);
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ  (end - coord) / (end - peak);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, ତାର୍କିକ negative = ମିଥ୍ୟା)
{
  out.reset();
  ଦଶମିକ axisMin = axisLimit.minimum;
  ଦଶମିକ axisDef = axisLimit.ମଧ୍ୟ;
  ଦଶମିକ axisMax = axisLimit.maximum;
  ଦଶମିକ lower = tent.minimum;
  ଦଶମିକ peak  = tent.ମଧ୍ୟ;
  ଦଶମିକ upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  ଯଦି (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    ପାଇଁ (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    ଫେରନ୍ତୁ;
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
  ଯଦି (axisMax <= lower && axisMax < peak)
      ଫେରନ୍ତୁ;  // No overlap

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
  ଯଦି (axisMax < peak)
  {
    ଦଶମିକ mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    ପାଇଁ (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    ଫେରନ୍ତୁ;
  }

  // lower <= axisDef <= peak <= axisMax

  ଦଶମିକ gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  ଦଶମିକ outGain = supportScalar (axisMax, tent);

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
  ଯଦି (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    ଦଶମିକ crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    ଦଶମିକ scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ଯଦି (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      ଦଶମିକ scalar = outGain;

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
    ଅନ୍ୟଥା
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ଯଦି (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      ଦଶମିକ scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      ଦଶମିକ scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ଅନ୍ୟଥା
  {
    // Special-case if peak is at axisMax.
    ଯଦି (axisMax == peak)
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
    ଦଶମିକ newUpper = peak + (1 - gain) * (upper - peak);
    ପ୍ରତିଜ୍ଞା (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ଯଦି (ମିଥ୍ୟା && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      ଯଦି (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	ପ୍ରତିଜ୍ଞା (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      ଦଶମିକ scalar = 1.f;

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
    ଅନ୍ୟଥା
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      ଦଶମିକ scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      ଦଶମିକ scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ଯଦି (peak < axisMax)
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
  ଯଦି (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    ଦଶମିକ scalar = supportScalar (axisMin, tent);

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
  ଅନ୍ୟଥା
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ଯଦି (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    ଦଶମିକ scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    ଦଶମିକ scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ TripleDistances _reverse_triple_distances (ଅଚଳ TripleDistances &v)
{ ଫେରନ୍ତୁ TripleDistances (v.positive, v.negative); }

ଦଶମିକ renormalize_ମୂଲ୍ୟ (ଦଶମିକ v, ଅଚଳ Triple &triple,
                         ଅଚଳ TripleDistances &triple_distances, ତାର୍କିକ extrapolate)
{
  ଦଶମିକ lower = triple.minimum, def = triple.ମଧ୍ୟ, upper = triple.maximum;
  ଯଦି (unlikely (!(lower <= def && def <= upper)))
    ଫେରନ୍ତୁ hb_clamp (v, -1.f, +1.f);

  ଯଦି (!extrapolate)
    v = hb_clamp (v, lower, upper);

  ଯଦି (v == def)
    ଫେରନ୍ତୁ 0.f;

  ଯଦି (def < 0.f)
    ଫେରନ୍ତୁ -renormalize_ମୂଲ୍ୟ (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ଯଦି (v > def)
  {
    ଦଶମିକ positive_range = upper - def;
    ଯଦି (unlikely (!positive_range))
      ଫେରନ୍ତୁ +1.f;
    ଫେରନ୍ତୁ (v - def) / positive_range;
  }

  /* v < def */
  ଯଦି (lower >= 0.f)
  {
    ଦଶମିକ negative_range = def - lower;
    ଯଦି (unlikely (!negative_range))
      ଫେରନ୍ତୁ -1.f;
    ଫେରନ୍ତୁ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  ଦଶମିକ total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  ଯଦି (unlikely (!total_distance))
    ଫେରନ୍ତୁ 0.f;

  ଦଶମିକ v_distance;
  ଯଦି (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  ଅନ୍ୟଥା
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  ଫେରନ୍ତୁ (-v_distance) /total_distance;
}

ଶୂନ୍ୟ
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  ଯଦି (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.ମଧ୍ୟ &&
                  axisLimit.ମଧ୍ୟ <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.ମଧ୍ୟ &&
                  tent.ମଧ୍ୟ <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.ମଧ୍ୟ == 0.f))
  {
    out.reset ();
    ଫେରନ୍ତୁ;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (ଦଶମିକ v) { ଫେରନ୍ତୁ renormalize_ମୂଲ୍ୟ (v, axisLimit, axis_triple_distances); };

  out.reset();
  ପାଇଁ (auto &p : sols)
  {
    ଯଦି (!p.first) ଜାରି;
    ଯଦି (p.second == Triple{})
    {
      out.push (p);
      ଜାରି;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.ମଧ୍ୟ), n (t.maximum)}));
  }
}
