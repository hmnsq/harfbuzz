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

#અન્તર્ભાવ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr સ્થિર દશાંશ EPSILON = 1.f / (1 << 14);
constexpr સ્થિર દશાંશ MAX_F2DOT14 = દશાંશ (0x7FFF) / (1 << 14);

સ્થિર અંતઃરેખ Triple _reverse_negate(અચલ Triple &v)
{ પાછા {-v.maximum, -v.મધ્ય, -v.minimum}; }


સ્થિર અંતઃરેખ દશાંશ supportScalar (દશાંશ coord, અચલ Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  દશાંશ start = tent.minimum, peak = tent.મધ્ય, end = tent.maximum;

  જો (unlikely (start > peak || peak > end))
    પાછા 1.f;
  જો (unlikely (start < 0 && end > 0 && peak != 0))
    પાછા 1.f;

  જો (peak == 0 || coord == peak)
    પાછા 1.f;

  જો (coord <= start || end <= coord)
    પાછા 0.f;

  /* Interpolate */
  જો (coord < peak)
    પાછા (coord - start) / (peak - start);
  નહીંતર
    પાછા  (end - coord) / (end - peak);
}

સ્થિર અંતઃરેખ શૂન્ય
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, તાર્કિક negative = ખોટું)
{
  out.reset();
  દશાંશ axisMin = axisLimit.minimum;
  દશાંશ axisDef = axisLimit.મધ્ય;
  દશાંશ axisMax = axisLimit.maximum;
  દશાંશ lower = tent.minimum;
  દશાંશ peak  = tent.મધ્ય;
  દશાંશ upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  જો (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    માટે (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    પાછા;
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
  જો (axisMax <= lower && axisMax < peak)
      પાછા;  // No overlap

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
  જો (axisMax < peak)
  {
    દશાંશ mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    માટે (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    પાછા;
  }

  // lower <= axisDef <= peak <= axisMax

  દશાંશ gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  દશાંશ outGain = supportScalar (axisMax, tent);

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
  જો (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    દશાંશ crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    દશાંશ scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    જો (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      દશાંશ scalar = outGain;

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
    નહીંતર
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      જો (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      દશાંશ scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      દશાંશ scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  નહીંતર
  {
    // Special-case if peak is at axisMax.
    જો (axisMax == peak)
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
    દશાંશ newUpper = peak + (1 - gain) * (upper - peak);
    પ્રતિજ્ઞા (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    જો (ખોટું && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      જો (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	પ્રતિજ્ઞા (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      દશાંશ scalar = 1.f;

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
    નહીંતર
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      દશાંશ scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      દશાંશ scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      જો (peak < axisMax)
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
  જો (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    દશાંશ scalar = supportScalar (axisMin, tent);

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
  નહીંતર
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    જો (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    દશાંશ scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    દશાંશ scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

સ્થિર અંતઃરેખ TripleDistances _reverse_triple_distances (અચલ TripleDistances &v)
{ પાછા TripleDistances (v.positive, v.negative); }

દશાંશ renormalize_કિંમત (દશાંશ v, અચલ Triple &triple,
                         અચલ TripleDistances &triple_distances, તાર્કિક extrapolate)
{
  દશાંશ lower = triple.minimum, def = triple.મધ્ય, upper = triple.maximum;
  જો (unlikely (!(lower <= def && def <= upper)))
    પાછા hb_clamp (v, -1.f, +1.f);

  જો (!extrapolate)
    v = hb_clamp (v, lower, upper);

  જો (v == def)
    પાછા 0.f;

  જો (def < 0.f)
    પાછા -renormalize_કિંમત (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  જો (v > def)
  {
    દશાંશ positive_range = upper - def;
    જો (unlikely (!positive_range))
      પાછા +1.f;
    પાછા (v - def) / positive_range;
  }

  /* v < def */
  જો (lower >= 0.f)
  {
    દશાંશ negative_range = def - lower;
    જો (unlikely (!negative_range))
      પાછા -1.f;
    પાછા (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  દશાંશ total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  જો (unlikely (!total_distance))
    પાછા 0.f;

  દશાંશ v_distance;
  જો (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  નહીંતર
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  પાછા (-v_distance) /total_distance;
}

શૂન્ય
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  જો (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.મધ્ય &&
                  axisLimit.મધ્ય <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.મધ્ય &&
                  tent.મધ્ય <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.મધ્ય == 0.f))
  {
    out.reset ();
    પાછા;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (દશાંશ v) { પાછા renormalize_કિંમત (v, axisLimit, axis_triple_distances); };

  out.reset();
  માટે (auto &p : sols)
  {
    જો (!p.first) ચાલુ;
    જો (p.second == Triple{})
    {
      out.push (p);
      ચાલુ;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.મધ્ય), n (t.maximum)}));
  }
}
