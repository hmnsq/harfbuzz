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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ਸਥਿਰ ਦਸ਼ਮਲਵ EPSILON = 1.f / (1 << 14);
constexpr ਸਥਿਰ ਦਸ਼ਮਲਵ MAX_F2DOT14 = ਦਸ਼ਮਲਵ (0x7FFF) / (1 << 14);

ਸਥਿਰ ਸਤਰਅੰਦਰ Triple ਉਲਟ_negate(ਅਚਲ Triple &v)
{ ਵਾਪਸ {-v.ਵੱਧ_ਤੋਂ_ਵੱਧ, -v.ਵਿਚਕਾਰ, -v.minimum}; }


ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ supportScalar (ਦਸ਼ਮਲਵ coord, ਅਚਲ Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ = tent.minimum, peak = tent.ਵਿਚਕਾਰ, ਸਿਰਾ = tent.ਵੱਧ_ਤੋਂ_ਵੱਧ;

  ਜੇ (unlikely (ਸ਼ੁਰੂ > peak || peak > ਸਿਰਾ))
    ਵਾਪਸ 1.f;
  ਜੇ (unlikely (ਸ਼ੁਰੂ < 0 && ਸਿਰਾ > 0 && peak != 0))
    ਵਾਪਸ 1.f;

  ਜੇ (peak == 0 || coord == peak)
    ਵਾਪਸ 1.f;

  ਜੇ (coord <= ਸ਼ੁਰੂ || ਸਿਰਾ <= coord)
    ਵਾਪਸ 0.f;

  /* Interpolate */
  ਜੇ (coord < peak)
    ਵਾਪਸ (coord - ਸ਼ੁਰੂ) / (peak - ਸ਼ੁਰੂ);
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ  (ਸਿਰਾ - coord) / (ਸਿਰਾ - peak);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_solve (Triple tent, Triple axis_ਹੱਦਬੰਦੀ_2, rebase_tent_ਨਤੀਜਾ_t &ਬਾਹਰਲਾ, ਤਾਰਕਿਕ negative = ਝੂਠ)
{
  ਬਾਹਰਲਾ.reset();
  ਦਸ਼ਮਲਵ axisMin = axis_ਹੱਦਬੰਦੀ_2.minimum;
  ਦਸ਼ਮਲਵ axisDef = axis_ਹੱਦਬੰਦੀ_2.ਵਿਚਕਾਰ;
  ਦਸ਼ਮਲਵ axisMax = axis_ਹੱਦਬੰਦੀ_2.ਵੱਧ_ਤੋਂ_ਵੱਧ;
  ਦਸ਼ਮਲਵ ਹੇਠਲਾ = tent.minimum;
  ਦਸ਼ਮਲਵ peak  = tent.ਵਿਚਕਾਰ;
  ਦਸ਼ਮਲਵ ਉੱਤਲਾ = tent.ਵੱਧ_ਤੋਂ_ਵੱਧ;

  // Mirror the problem such that axisDef <= peak
  ਜੇ (axisDef > peak)
  {
    _solve (ਉਲਟ_negate (tent), ਉਲਟ_negate (axis_ਹੱਦਬੰਦੀ_2), ਬਾਹਰਲਾ, !negative);

    ਲਈ (ਸਵੈਚਾਲਿਤ &p : ਬਾਹਰਲਾ)
      p = hb_pair (p.ਪਹਿਲਾ, ਉਲਟ_negate (p.second));

    ਵਾਪਸ;
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
  ਜੇ (axisMax <= ਹੇਠਲਾ && axisMax < peak)
      ਵਾਪਸ;  // No overlap

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
  ਜੇ (axisMax < peak)
  {
    ਦਸ਼ਮਲਵ mult = supportScalar (axisMax, tent);
    tent = Triple{ਹੇਠਲਾ, axisMax, axisMax};

    _solve (tent, axis_ਹੱਦਬੰਦੀ_2, ਬਾਹਰਲਾ);

    ਲਈ (ਸਵੈਚਾਲਿਤ &p : ਬਾਹਰਲਾ)
      p = hb_pair (p.ਪਹਿਲਾ * mult, p.second);

    ਵਾਪਸ;
  }

  // lower <= axisDef <= peak <= axisMax

  ਦਸ਼ਮਲਵ gain = supportScalar (axisDef, tent);
  ਬਾਹਰਲਾ.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  ਦਸ਼ਮਲਵ ਬਾਹਰਲਾ_Gain = supportScalar (axisMax, tent);

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
  ਜੇ (gain >= ਬਾਹਰਲਾ_Gain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    ਦਸ਼ਮਲਵ ਪਾਰ = peak + (1 - gain) * (ਉੱਤਲਾ - peak);

    Triple loc{hb_max (ਹੇਠਲਾ, axisDef), peak, ਪਾਰ};
    ਦਸ਼ਮਲਵ scalar = 1.f;

    // The part before the crossing point.
    ਬਾਹਰਲਾ.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ਜੇ (ਉੱਤਲਾ >= axisMax)
    {
      Triple loc {ਪਾਰ, axisMax, axisMax};
      ਦਸ਼ਮਲਵ scalar = ਬਾਹਰਲਾ_Gain;

      ਬਾਹਰਲਾ.push (hb_pair (scalar - gain, loc));
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
    ਨਹੀਂਤਾਂ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ਜੇ (ਉੱਤਲਾ == axisDef)
	ਉੱਤਲਾ += EPSILON;

      // Downslope.
      Triple loc1 {ਪਾਰ, ਉੱਤਲਾ, axisMax};
      ਦਸ਼ਮਲਵ scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {ਉੱਤਲਾ, axisMax, axisMax};
      ਦਸ਼ਮਲਵ scalar2 = 0.f;

      ਬਾਹਰਲਾ.push (hb_pair (scalar1 - gain, loc1));
      ਬਾਹਰਲਾ.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ਨਹੀਂਤਾਂ
  {
    // Special-case if peak is at axisMax.
    ਜੇ (axisMax == peak)
	ਉੱਤਲਾ = peak;

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
    ਦਸ਼ਮਲਵ new_ਉੱਤਲਾ = peak + (1 - gain) * (ਉੱਤਲਾ - peak);
    ਪ੍ਰਤਿਜ੍ਞਾ (axisMax <= new_ਉੱਤਲਾ);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ਜੇ (ਝੂਠ && (new_ਉੱਤਲਾ <= axisDef + (axisMax - axisDef) * 2))
    {
      ਉੱਤਲਾ = new_ਉੱਤਲਾ;
      ਜੇ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < ਉੱਤਲਾ)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	ਉੱਤਲਾ = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	ਪ੍ਰਤਿਜ੍ਞਾ (peak < ਉੱਤਲਾ);
      }

      Triple loc {hb_max (axisDef, ਹੇਠਲਾ), peak, ਉੱਤਲਾ};
      ਦਸ਼ਮਲਵ scalar = 1.f;

      ਬਾਹਰਲਾ.push (hb_pair (scalar - gain, loc));
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
    ਨਹੀਂਤਾਂ
    {
      Triple loc1 {hb_max (axisDef, ਹੇਠਲਾ), peak, axisMax};
      ਦਸ਼ਮਲਵ scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      ਦਸ਼ਮਲਵ scalar2 = ਬਾਹਰਲਾ_Gain;

      ਬਾਹਰਲਾ.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ਜੇ (peak < axisMax)
	ਬਾਹਰਲਾ.push (hb_pair (scalar2 - gain, loc2));
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
  ਜੇ (ਹੇਠਲਾ <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    ਦਸ਼ਮਲਵ scalar = supportScalar (axisMin, tent);

    ਬਾਹਰਲਾ.push (hb_pair (scalar - gain, loc));
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
  ਨਹੀਂਤਾਂ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ਜੇ (ਹੇਠਲਾ == axisDef)
      ਹੇਠਲਾ -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, ਹੇਠਲਾ, axisDef};
    ਦਸ਼ਮਲਵ scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, ਹੇਠਲਾ};
    ਦਸ਼ਮਲਵ scalar2 = 0.f;

    ਬਾਹਰਲਾ.push (hb_pair (scalar1 - gain, loc1));
    ਬਾਹਰਲਾ.push (hb_pair (scalar2 - gain, loc2));
  }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ TripleDistances ਉਲਟ_triple_distances (ਅਚਲ TripleDistances &v)
{ ਵਾਪਸ TripleDistances (v.positive, v.negative); }

ਦਸ਼ਮਲਵ renormalize_ਮੁੱਲ (ਦਸ਼ਮਲਵ v, ਅਚਲ Triple &triple,
                         ਅਚਲ TripleDistances &triple_distances, ਤਾਰਕਿਕ extrapolate)
{
  ਦਸ਼ਮਲਵ ਹੇਠਲਾ = triple.minimum, def = triple.ਵਿਚਕਾਰ, ਉੱਤਲਾ = triple.ਵੱਧ_ਤੋਂ_ਵੱਧ;
  ਜੇ (unlikely (!(ਹੇਠਲਾ <= def && def <= ਉੱਤਲਾ)))
    ਵਾਪਸ hb_clamp (v, -1.f, +1.f);

  ਜੇ (!extrapolate)
    v = hb_clamp (v, ਹੇਠਲਾ, ਉੱਤਲਾ);

  ਜੇ (v == def)
    ਵਾਪਸ 0.f;

  ਜੇ (def < 0.f)
    ਵਾਪਸ -renormalize_ਮੁੱਲ (-v, ਉਲਟ_negate (triple),
                              ਉਲਟ_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ਜੇ (v > def)
  {
    ਦਸ਼ਮਲਵ ਸਕਾਰਾਤਮਕ_range = ਉੱਤਲਾ - def;
    ਜੇ (unlikely (!ਸਕਾਰਾਤਮਕ_range))
      ਵਾਪਸ +1.f;
    ਵਾਪਸ (v - def) / ਸਕਾਰਾਤਮਕ_range;
  }

  /* v < def */
  ਜੇ (ਹੇਠਲਾ >= 0.f)
  {
    ਦਸ਼ਮਲਵ negative_range = def - ਹੇਠਲਾ;
    ਜੇ (unlikely (!negative_range))
      ਵਾਪਸ -1.f;
    ਵਾਪਸ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  ਦਸ਼ਮਲਵ ਕੁੱਲ_ਦੂਰੀ = triple_distances.negative * (-ਹੇਠਲਾ) + triple_distances.positive * def;
  ਜੇ (unlikely (!ਕੁੱਲ_ਦੂਰੀ))
    ਵਾਪਸ 0.f;

  ਦਸ਼ਮਲਵ v_ਦੂਰੀ;
  ਜੇ (v >= 0.f)
    v_ਦੂਰੀ = (def - v) * triple_distances.positive;
  ਨਹੀਂਤਾਂ
    v_ਦੂਰੀ = (-v) * triple_distances.negative + triple_distances.positive * def;

  ਵਾਪਸ (-v_ਦੂਰੀ) /ਕੁੱਲ_ਦੂਰੀ;
}

ਖਾਲੀ
rebase_tent (Triple tent, Triple axis_ਹੱਦਬੰਦੀ_2, TripleDistances axis_triple_distances,
	     rebase_tent_ਨਤੀਜਾ_t &ਬਾਹਰਲਾ,
	     rebase_tent_ਨਤੀਜਾ_t &scratch)
{
  ਜੇ (unlikely (!(-1.f <= axis_ਹੱਦਬੰਦੀ_2.minimum &&
                  axis_ਹੱਦਬੰਦੀ_2.minimum <= axis_ਹੱਦਬੰਦੀ_2.ਵਿਚਕਾਰ &&
                  axis_ਹੱਦਬੰਦੀ_2.ਵਿਚਕਾਰ <= axis_ਹੱਦਬੰਦੀ_2.ਵੱਧ_ਤੋਂ_ਵੱਧ &&
                  axis_ਹੱਦਬੰਦੀ_2.ਵੱਧ_ਤੋਂ_ਵੱਧ <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.ਵਿਚਕਾਰ &&
                  tent.ਵਿਚਕਾਰ <= tent.ਵੱਧ_ਤੋਂ_ਵੱਧ &&
                  tent.ਵੱਧ_ਤੋਂ_ਵੱਧ <= +2.f) ||
                tent.ਵਿਚਕਾਰ == 0.f))
  {
    ਬਾਹਰਲਾ.reset ();
    ਵਾਪਸ;
  }

  rebase_tent_ਨਤੀਜਾ_t &sols = scratch;
  _solve (tent, axis_ਹੱਦਬੰਦੀ_2, sols);

  ਸਵੈਚਾਲਿਤ n = [&axis_ਹੱਦਬੰਦੀ_2, &axis_triple_distances] (ਦਸ਼ਮਲਵ v) { ਵਾਪਸ renormalize_ਮੁੱਲ (v, axis_ਹੱਦਬੰਦੀ_2, axis_triple_distances); };

  ਬਾਹਰਲਾ.reset();
  ਲਈ (ਸਵੈਚਾਲਿਤ &p : sols)
  {
    ਜੇ (!p.ਪਹਿਲਾ) ਜਾਰੀ;
    ਜੇ (p.second == Triple{})
    {
      ਬਾਹਰਲਾ.push (p);
      ਜਾਰੀ;
    }
    Triple t = p.second;
    ਬਾਹਰਲਾ.push (hb_pair (p.ਪਹਿਲਾ,
		       Triple{n (t.minimum), n (t.ਵਿਚਕਾਰ), n (t.ਵੱਧ_ਤੋਂ_ਵੱਧ)}));
  }
}
