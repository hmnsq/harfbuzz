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

#include "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr static float EPSILON = 1.f / (1 << 14);
constexpr static float MAX_F2DOT14 = float (0x7FFF) / (1 << 14);

static anta_hstha Triple _reverse_negate(const Triple &v)
{ ꦮꦭꦶ {-v.maximum, -v.middle, -v.minimum}; }


static anta_hstha float supportScalar (float coord, const Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  float start = tent.minimum, peak = tent.middle, end = tent.maximum;

  ꦪꦼꦤ꧀ (unlikely (start > peak || peak > end))
    ꦮꦭꦶ 1.f;
  ꦪꦼꦤ꧀ (unlikely (start < 0 && end > 0 && peak != 0))
    ꦮꦭꦶ 1.f;

  ꦪꦼꦤ꧀ (peak == 0 || coord == peak)
    ꦮꦭꦶ 1.f;

  ꦪꦼꦤ꧀ (coord <= start || end <= coord)
    ꦮꦭꦶ 0.f;

  /* Interpolate */
  ꦪꦼꦤ꧀ (coord < peak)
    ꦮꦭꦶ (coord - start) / (peak - start);
  ꦭꦶꦪ
    ꦮꦭꦶ  (end - coord) / (end - peak);
}

static anta_hstha void
_solve (Triple tent, Triple axisLimit, rebase_tent_result_t &out, bool negative = false)
{
  out.reset();
  float axisMin = axisLimit.minimum;
  float axisDef = axisLimit.middle;
  float axisMax = axisLimit.maximum;
  float lower = tent.minimum;
  float peak  = tent.middle;
  float upper = tent.maximum;

  // Mirror the problem such that axisDef <= peak
  ꦪꦼꦤ꧀ (axisDef > peak)
  {
    _solve (_reverse_negate (tent), _reverse_negate (axisLimit), out, !negative);

    ꦏꦁꦒꦺꦴ (auto &p : out)
      p = hb_pair (p.first, _reverse_negate (p.second));

    ꦮꦭꦶ;
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
  ꦪꦼꦤ꧀ (axisMax <= lower && axisMax < peak)
      ꦮꦭꦶ;  // No overlap

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
  ꦪꦼꦤ꧀ (axisMax < peak)
  {
    float mult = supportScalar (axisMax, tent);
    tent = Triple{lower, axisMax, axisMax};

    _solve (tent, axisLimit, out);

    ꦏꦁꦒꦺꦴ (auto &p : out)
      p = hb_pair (p.first * mult, p.second);

    ꦮꦭꦶ;
  }

  // lower <= axisDef <= peak <= axisMax

  float gain = supportScalar (axisDef, tent);
  out.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  float outGain = supportScalar (axisMax, tent);

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
  ꦪꦼꦤ꧀ (gain >= outGain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    float crossing = peak + (1 - gain) * (upper - peak);

    Triple loc{hb_max (lower, axisDef), peak, crossing};
    float scalar = 1.f;

    // The part before the crossing point.
    out.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    ꦪꦼꦤ꧀ (upper >= axisMax)
    {
      Triple loc {crossing, axisMax, axisMax};
      float scalar = outGain;

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
    ꦭꦶꦪ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      ꦪꦼꦤ꧀ (upper == axisDef)
	upper += EPSILON;

      // Downslope.
      Triple loc1 {crossing, upper, axisMax};
      float scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {upper, axisMax, axisMax};
      float scalar2 = 0.f;

      out.push (hb_pair (scalar1 - gain, loc1));
      out.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ꦭꦶꦪ
  {
    // Special-case if peak is at axisMax.
    ꦪꦼꦤ꧀ (axisMax == peak)
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
    float newUpper = peak + (1 - gain) * (upper - peak);
    assert (axisMax <= newUpper);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    ꦪꦼꦤ꧀ (false && (newUpper <= axisDef + (axisMax - axisDef) * 2))
    {
      upper = newUpper;
      ꦪꦼꦤ꧀ (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < upper)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	upper = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	assert (peak < upper);
      }

      Triple loc {hb_max (axisDef, lower), peak, upper};
      float scalar = 1.f;

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
    ꦭꦶꦪ
    {
      Triple loc1 {hb_max (axisDef, lower), peak, axisMax};
      float scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      float scalar2 = outGain;

      out.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      ꦪꦼꦤ꧀ (peak < axisMax)
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
  ꦪꦼꦤ꧀ (lower <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    float scalar = supportScalar (axisMin, tent);

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
  ꦭꦶꦪ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    ꦪꦼꦤ꧀ (lower == axisDef)
      lower -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, lower, axisDef};
    float scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, lower};
    float scalar2 = 0.f;

    out.push (hb_pair (scalar1 - gain, loc1));
    out.push (hb_pair (scalar2 - gain, loc2));
  }
}

static anta_hstha TripleDistances _reverse_triple_distances (const TripleDistances &v)
{ ꦮꦭꦶ TripleDistances (v.positive, v.negative); }

float renormalizeValue (float v, const Triple &triple,
                         const TripleDistances &triple_distances, bool extrapolate)
{
  float lower = triple.minimum, def = triple.middle, upper = triple.maximum;
  ꦪꦼꦤ꧀ (unlikely (!(lower <= def && def <= upper)))
    ꦮꦭꦶ hb_clamp (v, -1.f, +1.f);

  ꦪꦼꦤ꧀ (!extrapolate)
    v = hb_clamp (v, lower, upper);

  ꦪꦼꦤ꧀ (v == def)
    ꦮꦭꦶ 0.f;

  ꦪꦼꦤ꧀ (def < 0.f)
    ꦮꦭꦶ -renormalizeValue (-v, _reverse_negate (triple),
                              _reverse_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  ꦪꦼꦤ꧀ (v > def)
  {
    float positive_range = upper - def;
    ꦪꦼꦤ꧀ (unlikely (!positive_range))
      ꦮꦭꦶ +1.f;
    ꦮꦭꦶ (v - def) / positive_range;
  }

  /* v < def */
  ꦪꦼꦤ꧀ (lower >= 0.f)
  {
    float negative_range = def - lower;
    ꦪꦼꦤ꧀ (unlikely (!negative_range))
      ꦮꦭꦶ -1.f;
    ꦮꦭꦶ (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  float total_distance = triple_distances.negative * (-lower) + triple_distances.positive * def;
  ꦪꦼꦤ꧀ (unlikely (!total_distance))
    ꦮꦭꦶ 0.f;

  float v_distance;
  ꦪꦼꦤ꧀ (v >= 0.f)
    v_distance = (def - v) * triple_distances.positive;
  ꦭꦶꦪ
    v_distance = (-v) * triple_distances.negative + triple_distances.positive * def;

  ꦮꦭꦶ (-v_distance) /total_distance;
}

void
rebase_tent (Triple tent, Triple axisLimit, TripleDistances axis_triple_distances,
	     rebase_tent_result_t &out,
	     rebase_tent_result_t &scratch)
{
  ꦪꦼꦤ꧀ (unlikely (!(-1.f <= axisLimit.minimum &&
                  axisLimit.minimum <= axisLimit.middle &&
                  axisLimit.middle <= axisLimit.maximum &&
                  axisLimit.maximum <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.middle &&
                  tent.middle <= tent.maximum &&
                  tent.maximum <= +2.f) ||
                tent.middle == 0.f))
  {
    out.reset ();
    ꦮꦭꦶ;
  }

  rebase_tent_result_t &sols = scratch;
  _solve (tent, axisLimit, sols);

  auto n = [&axisLimit, &axis_triple_distances] (float v) { ꦮꦭꦶ renormalizeValue (v, axisLimit, axis_triple_distances); };

  out.reset();
  ꦏꦁꦒꦺꦴ (auto &p : sols)
  {
    ꦪꦼꦤ꧀ (!p.first) continue;
    ꦪꦼꦤ꧀ (p.second == Triple{})
    {
      out.push (p);
      continue;
    }
    Triple t = p.second;
    out.push (hb_pair (p.first,
		       Triple{n (t.minimum), n (t.middle), n (t.maximum)}));
  }
}
