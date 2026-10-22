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

#അന്തര്ഭാവ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr സ്ഥിതം ദശാംശം EPSILON = 1.f / (1 << 14);
constexpr സ്ഥിതം ദശാംശം MAX_F2DOT14 = ദശാംശം (0x7FFF) / (1 << 14);

സ്ഥിതം വരിയിൽ Triple _reverse_negate(സ്ഥിരം Triple &v)
{ തിരികെ {-v.maximum, -v.നടുവ്, -v.minimum}; }


സ്ഥിതം വരിയിൽ ദശാംശം supportScalar (ദശാംശം coord, സ്ഥിരം Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  ദശാംശം start = tent.minimum, peak = tent.നടുവ്, end = tent.maximum;

  എങ്കിൽ (unlikely (start > peak || peak > end))
    തിരികെ 1.f;
  എങ്കിൽ (unlikely (start < 0 && end > 0 && peak != 0))
    തിരികെ 1.f;

  എങ്കിൽ (peak == 0 || coord == peak)
    തിരികെ 1.f;

  എങ്കിൽ (coord <= start || end <= coord)
    തിരികെ 0.f;

  /* Interpolate */
  എങ്കിൽ (coord < peak)
    തിരികെ (coord - start) / (peak - start);
  അല്ലെങ്കിൽ
    തിരികെ  (end - coord) / (end - peak);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, തർക്കം negative = അസത്യം)
{
  out.reset();
  ദശാംശം axisMin = axisLimit.minimum;
  ദശാംശം axisDef = axisLimit.നടുവ്;
  ദശാംശം axisMax = axisLimit.maximum;
  ദശാംശം lower = tent.minimum;
  ദശാംശം peak  = tent.നടുവ്;
  ദശാംശം upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  എങ്കിൽ (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    വേണ്ടി (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    തിരികെ;
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
  എങ്കിൽ (axisMax <= lower && axisMax < peak)
      തിരികെ;  // No overlap

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
  എങ്കിൽ (axisMax < peak)
  {
    ദശാംശം mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    വേണ്ടി (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    തിരികെ;
  }

  // lower <= axisDef <= peak <= axisMax

  ദശാംശം gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  ദശാംശം outGain = supportScalar (axisMax, tent);

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
  എങ്കിൽ (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    ദശാംശം crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    ദശാംശം scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    എങ്കിൽ (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      ദശാംശം scalar = outGain;

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
    അല്ലെങ്കിൽ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      എങ്കിൽ (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      ദശാംശം scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      ദശാംശം scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  അല്ലെങ്കിൽ
  {
    // Special-case if peak is at axisMax.
    എങ്കിൽ (axisMax == peak)
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
    ദശാംശം newUpper = peak + (1 - gain) * (upper - peak);
    പ്രതിജ്ഞാ (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    എങ്കിൽ (അസത്യം && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      എങ്കിൽ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	പ്രതിജ്ഞാ (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      ദശാംശം scalar = 1.f;

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
    അല്ലെങ്കിൽ
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      ദശാംശം scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      ദശാംശം scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      എങ്കിൽ (peak < axisMax)
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
  എങ്കിൽ (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    ദശാംശം scalar = supportScalar (axisMin, tent);

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
  അല്ലെങ്കിൽ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    എങ്കിൽ (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    ദശാംശം scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    ദശാംശം scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

സ്ഥിതം വരിയിൽ TripleDistances _reverse_triple_distances (സ്ഥിരം TripleDistances &v)
{ തിരികെ TripleDistances (v.positive, v.negative); }

ദശാംശം renormalize_മൂല്യം (ദശാംശം v, സ്ഥിരം Triple &triple,
                         സ്ഥിരം TripleDistances &triple_distances, തർക്കം extrapolate)
{
  ദശാംശം lower = triple.minimum, def = triple.നടുവ്, upper = triple.maximum;
  എങ്കിൽ (unlikely (!(lower <= def && def <= upper)))
    തിരികെ hb_clamp (v, -1.f, +1.f);

  എങ്കിൽ (!extrapolate)
    v = hb_clamp (v, lower, upper);

  എങ്കിൽ (v == def)
    തിരികെ 0.f;

  എങ്കിൽ (def < 0.f)
    തിരികെ -renormalize_മൂല്യം (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  എങ്കിൽ (v > def)
  {
    ദശാംശം positive_range = upper - def;
    എങ്കിൽ (unlikely (!positive_range))
      തിരികെ +1.f;
    തിരികെ (v - def) / positive_range;
  }

  /* v < def */
  എങ്കിൽ (lower >= 0.f)
  {
    ദശാംശം negative_range = def - lower;
    എങ്കിൽ (unlikely (!negative_range))
      തിരികെ -1.f;
    തിരികെ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  ദശാംശം total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  എങ്കിൽ (unlikely (!total_distance))
    തിരികെ 0.f;

  ദശാംശം v_distance;
  എങ്കിൽ (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  അല്ലെങ്കിൽ
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  തിരികെ (-v_distance) /total_distance;
}

ശൂന്യം
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  എങ്കിൽ (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.നടുവ് &&
                  axisLimit.നടുവ് <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.നടുവ് &&
                  tent.നടുവ് <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.നടുവ് == 0.f))
  {
    out.reset ();
    തിരികെ;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (ദശാംശം v) { തിരികെ renormalize_മൂല്യം (v, axisLimit, axis_triple_distances); };

  out.reset();
  വേണ്ടി (auto &p : sols)
  {
    എങ്കിൽ (!p.first) തുടരുക;
    എങ്കിൽ (p.second == Triple{})
    {
      out.push (p);
      തുടരുക;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.നടുവ്), n (t.maximum)}));
  }
}
