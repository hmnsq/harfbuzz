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

#ضمن "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ساكن عائم EPSILON = 1.f / (1 << 14);
constexpr ساكن عائم MAX_F2DOT14 = عائم (0x7FFF) / (1 << 14);

ساكن مضمن Triple ابتو_negate(ثابت Triple &v)
{ أرجع {-v.وڌ_کان_وڌ, -v.وچ, -v.minimum}; }


ساكن مضمن عائم supportScalar (عائم coord, ثابت Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  عائم شروعات = tent.minimum, peak = tent.وچ, پڇاڙي = tent.وڌ_کان_وڌ;

  إذا (unlikely (شروعات > peak || peak > پڇاڙي))
    أرجع 1.f;
  إذا (unlikely (شروعات < 0 && پڇاڙي > 0 && peak != 0))
    أرجع 1.f;

  إذا (peak == 0 || coord == peak)
    أرجع 1.f;

  إذا (coord <= شروعات || پڇاڙي <= coord)
    أرجع 0.f;

  /* Interpolate */
  إذا (coord < peak)
    أرجع (coord - شروعات) / (peak - شروعات);
  وإلا
    أرجع  (پڇاڙي - coord) / (پڇاڙي - peak);
}

ساكن مضمن فراغ
_solve (Triple tent, Triple axis_حد_بندي_2, rebase_tent_نتيجو_t &ٻاهريون, منطقي negative = خطأ)
{
  ٻاهريون.reset();
  عائم axisMin = axis_حد_بندي_2.minimum;
  عائم axisDef = axis_حد_بندي_2.وچ;
  عائم axisMax = axis_حد_بندي_2.وڌ_کان_وڌ;
  عائم هيٺاهون = tent.minimum;
  عائم peak  = tent.وچ;
  عائم مٿانهون = tent.وڌ_کان_وڌ;

  // Mirror the problem such that axisDef <= peak
  إذا (axisDef > peak)
  {
    _solve (ابتو_negate (tent), ابتو_negate (axis_حد_بندي_2), ٻاهريون, !negative);

    لكل (خودڪار &p : ٻاهريون)
      p = hb_pair (p.پهريون, ابتو_negate (p.second));

    أرجع;
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
  إذا (axisMax <= هيٺاهون && axisMax < peak)
      أرجع;  // No overlap

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
  إذا (axisMax < peak)
  {
    عائم mult = supportScalar (axisMax, tent);
    tent = Triple{هيٺاهون, axisMax, axisMax};

    _solve (tent, axis_حد_بندي_2, ٻاهريون);

    لكل (خودڪار &p : ٻاهريون)
      p = hb_pair (p.پهريون * mult, p.second);

    أرجع;
  }

  // lower <= axisDef <= peak <= axisMax

  عائم gain = supportScalar (axisDef, tent);
  ٻاهريون.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  عائم ٻاهريون_Gain = supportScalar (axisMax, tent);

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
  إذا (gain >= ٻاهريون_Gain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    عائم پار = peak + (1 - gain) * (مٿانهون - peak);

    Triple loc{hb_max (هيٺاهون, axisDef), peak, پار};
    عائم scalar = 1.f;

    // The part before the crossing point.
    ٻاهريون.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    إذا (مٿانهون >= axisMax)
    {
      Triple loc {پار, axisMax, axisMax};
      عائم scalar = ٻاهريون_Gain;

      ٻاهريون.push (hb_pair (scalar - gain, loc));
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
    وإلا
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      إذا (مٿانهون == axisDef)
	مٿانهون += EPSILON;

      // Downslope.
      Triple loc1 {پار, مٿانهون, axisMax};
      عائم scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {مٿانهون, axisMax, axisMax};
      عائم scalar2 = 0.f;

      ٻاهريون.push (hb_pair (scalar1 - gain, loc1));
      ٻاهريون.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  وإلا
  {
    // Special-case if peak is at axisMax.
    إذا (axisMax == peak)
	مٿانهون = peak;

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
    عائم new_مٿانهون = peak + (1 - gain) * (مٿانهون - peak);
    أكد (axisMax <= new_مٿانهون);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    إذا (خطأ && (new_مٿانهون <= axisDef + (axisMax - axisDef) * 2))
    {
      مٿانهون = new_مٿانهون;
      إذا (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < مٿانهون)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	مٿانهون = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	أكد (peak < مٿانهون);
      }

      Triple loc {hb_max (axisDef, هيٺاهون), peak, مٿانهون};
      عائم scalar = 1.f;

      ٻاهريون.push (hb_pair (scalar - gain, loc));
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
    وإلا
    {
      Triple loc1 {hb_max (axisDef, هيٺاهون), peak, axisMax};
      عائم scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      عائم scalar2 = ٻاهريون_Gain;

      ٻاهريون.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      إذا (peak < axisMax)
	ٻاهريون.push (hb_pair (scalar2 - gain, loc2));
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
  إذا (هيٺاهون <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    عائم scalar = supportScalar (axisMin, tent);

    ٻاهريون.push (hb_pair (scalar - gain, loc));
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
  وإلا
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    إذا (هيٺاهون == axisDef)
      هيٺاهون -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, هيٺاهون, axisDef};
    عائم scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, هيٺاهون};
    عائم scalar2 = 0.f;

    ٻاهريون.push (hb_pair (scalar1 - gain, loc1));
    ٻاهريون.push (hb_pair (scalar2 - gain, loc2));
  }
}

ساكن مضمن TripleDistances ابتو_triple_distances (ثابت TripleDistances &v)
{ أرجع TripleDistances (v.positive, v.negative); }

عائم renormalize_قدر (عائم v, ثابت Triple &triple,
                         ثابت TripleDistances &triple_distances, منطقي extrapolate)
{
  عائم هيٺاهون = triple.minimum, def = triple.وچ, مٿانهون = triple.وڌ_کان_وڌ;
  إذا (unlikely (!(هيٺاهون <= def && def <= مٿانهون)))
    أرجع hb_clamp (v, -1.f, +1.f);

  إذا (!extrapolate)
    v = hb_clamp (v, هيٺاهون, مٿانهون);

  إذا (v == def)
    أرجع 0.f;

  إذا (def < 0.f)
    أرجع -renormalize_قدر (-v, ابتو_negate (triple),
                              ابتو_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  إذا (v > def)
  {
    عائم مثبت_range = مٿانهون - def;
    إذا (unlikely (!مثبت_range))
      أرجع +1.f;
    أرجع (v - def) / مثبت_range;
  }

  /* v < def */
  إذا (هيٺاهون >= 0.f)
  {
    عائم negative_range = def - هيٺاهون;
    إذا (unlikely (!negative_range))
      أرجع -1.f;
    أرجع (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  عائم ڪل_مفاصلو = triple_distances.negative * (-هيٺاهون) + triple_distances.positive * def;
  إذا (unlikely (!ڪل_مفاصلو))
    أرجع 0.f;

  عائم v_مفاصلو;
  إذا (v >= 0.f)
    v_مفاصلو = (def - v) * triple_distances.positive;
  وإلا
    v_مفاصلو = (-v) * triple_distances.negative + triple_distances.positive * def;

  أرجع (-v_مفاصلو) /ڪل_مفاصلو;
}

فراغ
rebase_tent (Triple tent, Triple axis_حد_بندي_2, TripleDistances axis_triple_distances,
	     rebase_tent_نتيجو_t &ٻاهريون,
	     rebase_tent_نتيجو_t &scratch)
{
  إذا (unlikely (!(-1.f <= axis_حد_بندي_2.minimum &&
                  axis_حد_بندي_2.minimum <= axis_حد_بندي_2.وچ &&
                  axis_حد_بندي_2.وچ <= axis_حد_بندي_2.وڌ_کان_وڌ &&
                  axis_حد_بندي_2.وڌ_کان_وڌ <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.وچ &&
                  tent.وچ <= tent.وڌ_کان_وڌ &&
                  tent.وڌ_کان_وڌ <= +2.f) ||
                tent.وچ == 0.f))
  {
    ٻاهريون.reset ();
    أرجع;
  }

  rebase_tent_نتيجو_t &sols = scratch;
  _solve (tent, axis_حد_بندي_2, sols);

  خودڪار n = [&axis_حد_بندي_2, &axis_triple_distances] (عائم v) { أرجع renormalize_قدر (v, axis_حد_بندي_2, axis_triple_distances); };

  ٻاهريون.reset();
  لكل (خودڪار &p : sols)
  {
    إذا (!p.پهريون) استمر;
    إذا (p.second == Triple{})
    {
      ٻاهريون.push (p);
      استمر;
    }
    Triple t = p.second;
    ٻاهريون.push (hb_pair (p.پهريون,
		       Triple{n (t.minimum), n (t.وچ), n (t.وڌ_کان_وڌ)}));
  }
}
