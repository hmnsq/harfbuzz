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

#অন্তর্ভাব "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr স্থির দশমিক EPSILON = 1.f / (1 << 14);
constexpr স্থির দশমিক MAX_F2DOT14 = দশমিক (0x7FFF) / (1 << 14);

স্থির অন্তঃরেখ Triple _reverse_negate(ধ্রুবক Triple &v)
{ ফেরত {-v.maximum, -v.মধ্য, -v.minimum}; }


স্থির অন্তঃরেখ দশমিক supportScalar (দশমিক coord, ধ্রুবক Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  দশমিক start = tent.minimum, peak = tent.মধ্য, end = tent.maximum;

  যদি (unlikely (start > peak || peak > end))
    ফেরত 1.f;
  যদি (unlikely (start < 0 && end > 0 && peak != 0))
    ফেরত 1.f;

  যদি (peak == 0 || coord == peak)
    ফেরত 1.f;

  যদি (coord <= start || end <= coord)
    ফেরত 0.f;

  /* Interpolate */
  যদি (coord < peak)
    ফেরত (coord - start) / (peak - start);
  অন্যথা
    ফেরত  (end - coord) / (end - peak);
}

স্থির অন্তঃরেখ শূন্য
_solve (Triple tent, Triple axisLimit, rebase_tent_ফলাফল_t &out, যৌক্তিক negative = মিথ্যা)
{
  out.reset();
  দশমিক axisMin = axisLimit.minimum;
  দশমিক axisDef = axisLimit.মধ্য;
  দশমিক axisMax = axisLimit.maximum;
  দশমিক lower = tent.minimum;
  দশমিক peak  = tent.মধ্য;
  দশমিক upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  যদি (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    জন্য (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    ফেরত;
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
  যদি (axisMax <= lower && axisMax < peak)
      ফেরত;  // No overlap

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
  যদি (axisMax < peak)
  {
    দশমিক mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    জন্য (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    ফেরত;
  }

  // lower <= axisDef <= peak <= axisMax

  দশমিক gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  দশমিক outGain = supportScalar (axisMax, tent);

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
  যদি (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    দশমিক crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    দশমিক scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    যদি (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      দশমিক scalar = outGain;

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
    অন্যথা
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      যদি (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      দশমিক scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      দশমিক scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  অন্যথা
  {
    // Special-case if peak is at axisMax.
    যদি (axisMax == peak)
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
    দশমিক newUpper = peak + (1 - gain) * (upper - peak);
    প্রতিজ্ঞা (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    যদি (মিথ্যা && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      যদি (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	প্রতিজ্ঞা (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      দশমিক scalar = 1.f;

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
    অন্যথা
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      দশমিক scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      দশমিক scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      যদি (peak < axisMax)
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
  যদি (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    দশমিক scalar = supportScalar (axisMin, tent);

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
  অন্যথা
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    যদি (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    দশমিক scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    দশমিক scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

স্থির অন্তঃরেখ TripleDistances _reverse_triple_distances (ধ্রুবক TripleDistances &v)
{ ফেরত TripleDistances (v.positive, v.negative); }

দশমিক renormalize_মান (দশমিক v, ধ্রুবক Triple &triple,
                         ধ্রুবক TripleDistances &triple_distances, যৌক্তিক extrapolate)
{
  দশমিক lower = triple.minimum, def = triple.মধ্য, upper = triple.maximum;
  যদি (unlikely (!(lower <= def && def <= upper)))
    ফেরত hb_clamp (v, -1.f, +1.f);

  যদি (!extrapolate)
    v = hb_clamp (v, lower, upper);

  যদি (v == def)
    ফেরত 0.f;

  যদি (def < 0.f)
    ফেরত -renormalize_মান (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  যদি (v > def)
  {
    দশমিক positive_range = upper - def;
    যদি (unlikely (!positive_range))
      ফেরত +1.f;
    ফেরত (v - def) / positive_range;
  }

  /* v < def */
  যদি (lower >= 0.f)
  {
    দশমিক negative_range = def - lower;
    যদি (unlikely (!negative_range))
      ফেরত -1.f;
    ফেরত (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  দশমিক যোগ_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  যদি (unlikely (!যোগ_distance))
    ফেরত 0.f;

  দশমিক v_distance;
  যদি (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  অন্যথা
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  ফেরত (-v_distance) /যোগ_distance;
}

শূন্য
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_ফলাফল_t &out,
	     rebase_tent_ফলাফল_t &scratch)
{
  যদি (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.মধ্য &&
                  axisLimit.মধ্য <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.মধ্য &&
                  tent.মধ্য <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.মধ্য == 0.f))
  {
    out.reset ();
    ফেরত;
  }

  rebase_tent_ফলাফল_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (দশমিক v) { ফেরত renormalize_মান (v, axisLimit, axis_triple_distances); };

  out.reset();
  জন্য (auto &p : sols)
  {
    যদি (!p.first) চালু;
    যদি (p.second == Triple{})
    {
      out.push (p);
      চালু;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.মধ্য), n (t.maximum)}));
  }
}
