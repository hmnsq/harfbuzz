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

#شامل "hb-subset-instancer-solver.hh"

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/f73220816264fc383b8a75f2146e8d69e455d398/Lib/fontTools/varLib/instancer/solver.py
 *
 * Where that file returns None for a triple, we return Triple{}.
 * This should be safe.
 */

constexpr ساکن اعشاری EPSILON = 1.f / (1 << 14);
constexpr ساکن اعشاری MAX_F2DOT14 = اعشاری (0x7FFF) / (1 << 14);

ساکن اندرونی Triple الٹ_negate(ثابت Triple &v)
{ واپس {-v.زیادہ_سے_زیادہ, -v.درمیان, -v.minimum}; }


ساکن اندرونی اعشاری supportScalar (اعشاری coord, ثابت Triple &tent)
{
  /* Copied from VarRegionAxis::evaluate() */
  اعشاری آغاز = tent.minimum, peak = tent.درمیان, انتہا = tent.زیادہ_سے_زیادہ;

  اگر (unlikely (آغاز > peak || peak > انتہا))
    واپس 1.f;
  اگر (unlikely (آغاز < 0 && انتہا > 0 && peak != 0))
    واپس 1.f;

  اگر (peak == 0 || coord == peak)
    واپس 1.f;

  اگر (coord <= آغاز || انتہا <= coord)
    واپس 0.f;

  /* Interpolate */
  اگر (coord < peak)
    واپس (coord - آغاز) / (peak - آغاز);
  ورنہ
    واپس  (انتہا - coord) / (انتہا - peak);
}

ساکن اندرونی خالی
_solve (Triple tent, Triple axis_حد_بندی_2, rebase_tent_نتیجہ_t &برآمد, منطقی negative = جھوٹ)
{
  برآمد.reset();
  اعشاری axisMin = axis_حد_بندی_2.minimum;
  اعشاری axisDef = axis_حد_بندی_2.درمیان;
  اعشاری axisMax = axis_حد_بندی_2.زیادہ_سے_زیادہ;
  اعشاری زیریں = tent.minimum;
  اعشاری peak  = tent.درمیان;
  اعشاری بالائی = tent.زیادہ_سے_زیادہ;

  // Mirror the problem such that axisDef <= peak
  اگر (axisDef > peak)
  {
    _solve (الٹ_negate (tent), الٹ_negate (axis_حد_بندی_2), برآمد, !negative);

    برائے (خودکار &p : برآمد)
      p = hb_pair (p.پہلا, الٹ_negate (p.second));

    واپس;
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
  اگر (axisMax <= زیریں && axisMax < peak)
      واپس;  // No overlap

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
  اگر (axisMax < peak)
  {
    اعشاری mult = supportScalar (axisMax, tent);
    tent = Triple{زیریں, axisMax, axisMax};

    _solve (tent, axis_حد_بندی_2, برآمد);

    برائے (خودکار &p : برآمد)
      p = hb_pair (p.پہلا * mult, p.second);

    واپس;
  }

  // lower <= axisDef <= peak <= axisMax

  اعشاری gain = supportScalar (axisDef, tent);
  برآمد.push(hb_pair (gain, Triple{}));

  // First, the positive side

  // outGain is the scalar of axisMax at the tent.
  اعشاری برآمد_Gain = supportScalar (axisMax, tent);

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
  اگر (gain >= برآمد_Gain)
  {
    // Note that this is the branch taken if both gain and outGain are 0.

    // Crossing point on the axis.
    اعشاری پار = peak + (1 - gain) * (بالائی - peak);

    Triple loc{hb_max (زیریں, axisDef), peak, پار};
    اعشاری scalar = 1.f;

    // The part before the crossing point.
    برآمد.push (hb_pair (scalar - gain, loc));

    /* The part after the crossing point may use one or two tents,
     * depending on whether upper is before axisMax or not, in one
     * case we need to keep it down to eternity.
     *
     * Case 3a1, similar to case 1neg; just one tent needed, as in
     * the drawing above.
     */
    اگر (بالائی >= axisMax)
    {
      Triple loc {پار, axisMax, axisMax};
      اعشاری scalar = برآمد_Gain;

      برآمد.push (hb_pair (scalar - gain, loc));
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
    ورنہ
    {
      // A tent's peak cannot fall on axis default. Nudge it.
      اگر (بالائی == axisDef)
	بالائی += EPSILON;

      // Downslope.
      Triple loc1 {پار, بالائی, axisMax};
      اعشاری scalar1 = 0.f;

      // Eternity justify.
      Triple loc2 {بالائی, axisMax, axisMax};
      اعشاری scalar2 = 0.f;

      برآمد.push (hb_pair (scalar1 - gain, loc1));
      برآمد.push (hb_pair (scalar2 - gain, loc2));
    }
  }

  ورنہ
  {
    // Special-case if peak is at axisMax.
    اگر (axisMax == peak)
	بالائی = peak;

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
    اعشاری new_بالائی = peak + (1 - gain) * (بالائی - peak);
    أكد (axisMax <= new_بالائی);  // Because outGain > gain
    /* Disabled because ots doesn't like us:
     * https://github.com/fonttools/fonttools/issues/3350 */

    اگر (جھوٹ && (new_بالائی <= axisDef + (axisMax - axisDef) * 2))
    {
      بالائی = new_بالائی;
      اگر (!negative && axisDef + (axisMax - axisDef) * MAX_F2DOT14 < بالائی)
      {
	// we clamp +2.0 to the max F2Dot14 (~1.99994) for convenience
	بالائی = axisDef + (axisMax - axisDef) * MAX_F2DOT14;
	أكد (peak < بالائی);
      }

      Triple loc {hb_max (axisDef, زیریں), peak, بالائی};
      اعشاری scalar = 1.f;

      برآمد.push (hb_pair (scalar - gain, loc));
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
    ورنہ
    {
      Triple loc1 {hb_max (axisDef, زیریں), peak, axisMax};
      اعشاری scalar1 = 1.f;

      Triple loc2 {peak, axisMax, axisMax};
      اعشاری scalar2 = برآمد_Gain;

      برآمد.push (hb_pair (scalar1 - gain, loc1));
      // Don't add a dirac delta!
      اگر (peak < axisMax)
	برآمد.push (hb_pair (scalar2 - gain, loc2));
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
  اگر (زیریں <= axisMin)
  {
    Triple loc {axisMin, axisMin, axisDef};
    اعشاری scalar = supportScalar (axisMin, tent);

    برآمد.push (hb_pair (scalar - gain, loc));
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
  ورنہ
  {
    // A tent's peak cannot fall on axis default. Nudge it.
    اگر (زیریں == axisDef)
      زیریں -= EPSILON;

    // Downslope.
    Triple loc1 {axisMin, زیریں, axisDef};
    اعشاری scalar1 = 0.f;

    // Eternity justify.
    Triple loc2 {axisMin, axisMin, زیریں};
    اعشاری scalar2 = 0.f;

    برآمد.push (hb_pair (scalar1 - gain, loc1));
    برآمد.push (hb_pair (scalar2 - gain, loc2));
  }
}

ساکن اندرونی TripleDistances الٹ_triple_distances (ثابت TripleDistances &v)
{ واپس TripleDistances (v.positive, v.negative); }

اعشاری renormalize_قدر (اعشاری v, ثابت Triple &triple,
                         ثابت TripleDistances &triple_distances, منطقی extrapolate)
{
  اعشاری زیریں = triple.minimum, def = triple.درمیان, بالائی = triple.زیادہ_سے_زیادہ;
  اگر (unlikely (!(زیریں <= def && def <= بالائی)))
    واپس hb_clamp (v, -1.f, +1.f);

  اگر (!extrapolate)
    v = hb_clamp (v, زیریں, بالائی);

  اگر (v == def)
    واپس 0.f;

  اگر (def < 0.f)
    واپس -renormalize_قدر (-v, الٹ_negate (triple),
                              الٹ_triple_distances (triple_distances), extrapolate);

  /* default >= 0 and v != default */
  اگر (v > def)
  {
    اعشاری مثبت_range = بالائی - def;
    اگر (unlikely (!مثبت_range))
      واپس +1.f;
    واپس (v - def) / مثبت_range;
  }

  /* v < def */
  اگر (زیریں >= 0.f)
  {
    اعشاری negative_range = def - زیریں;
    اگر (unlikely (!negative_range))
      واپس -1.f;
    واپس (v - def) / negative_range;
  }

  /* lower < 0 and v < default */
  اعشاری کل_فاصلہ = triple_distances.negative * (-زیریں) + triple_distances.positive * def;
  اگر (unlikely (!کل_فاصلہ))
    واپس 0.f;

  اعشاری v_فاصلہ;
  اگر (v >= 0.f)
    v_فاصلہ = (def - v) * triple_distances.positive;
  ورنہ
    v_فاصلہ = (-v) * triple_distances.negative + triple_distances.positive * def;

  واپس (-v_فاصلہ) /کل_فاصلہ;
}

خالی
rebase_tent (Triple tent, Triple axis_حد_بندی_2, TripleDistances axis_triple_distances,
	     rebase_tent_نتیجہ_t &برآمد,
	     rebase_tent_نتیجہ_t &scratch)
{
  اگر (unlikely (!(-1.f <= axis_حد_بندی_2.minimum &&
                  axis_حد_بندی_2.minimum <= axis_حد_بندی_2.درمیان &&
                  axis_حد_بندی_2.درمیان <= axis_حد_بندی_2.زیادہ_سے_زیادہ &&
                  axis_حد_بندی_2.زیادہ_سے_زیادہ <= +1.f) ||
                !(-2.f <= tent.minimum &&
                  tent.minimum <= tent.درمیان &&
                  tent.درمیان <= tent.زیادہ_سے_زیادہ &&
                  tent.زیادہ_سے_زیادہ <= +2.f) ||
                tent.درمیان == 0.f))
  {
    برآمد.reset ();
    واپس;
  }

  rebase_tent_نتیجہ_t &sols = scratch;
  _solve (tent, axis_حد_بندی_2, sols);

  خودکار n = [&axis_حد_بندی_2, &axis_triple_distances] (اعشاری v) { واپس renormalize_قدر (v, axis_حد_بندی_2, axis_triple_distances); };

  برآمد.reset();
  برائے (خودکار &p : sols)
  {
    اگر (!p.پہلا) جاری;
    اگر (p.second == Triple{})
    {
      برآمد.push (p);
      جاری;
    }
    Triple t = p.second;
    برآمد.push (hb_pair (p.پہلا,
		       Triple{n (t.minimum), n (t.درمیان), n (t.زیادہ_سے_زیادہ)}));
  }
}
