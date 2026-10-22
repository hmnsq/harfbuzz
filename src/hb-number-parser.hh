/*
 * Copyright © 2019-2026  Ebrahim Byagowi
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
 *
 */

#నిర్వచితం_కాకపోతే HB_సంఖ్య_PARSER_HH
#నిర్వచించు HB_సంఖ్య_PARSER_HH

#అన్తర్భావ "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
స్థిర అంతఃవరుస ద్విగుణ
_pow10 (గుర్తులేని exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  ఒకవేళ (unlikely (exponent > 308)) తిరిగి HUGE_VAL;

  స్థిర స్థిరరాశి ద్విగుణ powers_యొక్క_10[] =
  {
    1.0e+256,
    1.0e+128,
    1.0e+64,
    1.0e+32,
    1.0e+16,
    1.0e+8,
    10000.,
    100.,
    10.
  };
  గుర్తులేని ముసుగు = 1 << (ARRAY_నిడివి (powers_యొక్క_10) - 1);
  ద్విగుణ ఫలితం = 1;
  కొరకు (స్థిరరాశి ద్విగుణ *power = powers_యొక్క_10; ముసుగు; ++power, ముసుగు >>= 1)
    ఒకవేళ (exponent & ముసుగు) ఫలితం *= *power;
  తిరిగి ఫలితం;
}

నిర్మాణం _hb_fraction
{
  శూన్య కూడు (పూర్ణాంకం digit)
  {
    ఒకవేళ (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++లెక్క;
    }
    లేకపోతే ఒకవేళ (drop < 0) drop = digit;
    లేకపోతే ఒకవేళ (digit != 0) sticky = నిజం;
  }

  ద్విగుణ get_విలువ () స్థిరరాశి
  {
    ద్విగుణ ఫలితం = fraction;
    // Round-to-nearest-even using the recorded tail
    ఒకవేళ (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      ఫలితం += 1;
    తిరిగి ఫలితం / _pow10 (లెక్క);
  }

  తార్కిక is_empty () స్థిరరాశి { తిరిగి !లెక్క; }

అంతరంగ:
  గుర్తులేని లెక్క = 0;
  ద్విగుణ fraction = 0;
  పూర్ణాంకం drop = -1; // drop is the first digit we couldn't accumulate
  తార్కిక sticky = అబద్ధం; // sticky is set when any later digit was nonzero.
  స్థిర constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
};

// A variant of strtod that also gets end of buffer in its second argument.
//
// Grammar (with backtracking on partial exponents):
//
//   number  := sign? mantissa exponent?
//   sign    := '+' | '-'
//   mantissa:= digits ('.' digits?)?
//	    | '.' digits
//   exponent:= ('e'|'E') sign? digits
//
// On success, *end_ptr is set just past the longest well-formed prefix.
// On no match, *end_ptr is left at the original p (matching strtod's
// "no conversion performed" behavior, so the caller's pointer is unchanged).
స్థిర అంతఃవరుస ద్విగుణ
hb_strtod (స్థిరరాశి అక్షరం *p, స్థిరరాశి అక్షరం **end_ptr /* IN/OUT */)
{
  స్థిరరాశి అక్షరం *p_original = p;
  స్థిరరాశి అక్షరం *pe = *end_ptr;

  // Skip leading whitespace
  అయితే (p < pe && ISSPACE (*p))
    p++;

  // Sign
  తార్కిక neg = అబద్ధం;
  ఒకవేళ (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  ద్విగుణ value = 0;

  // Integer and fraction
  {
    స్థిరరాశి అక్షరం *int_ప్రారంభం = p;
    అయితే (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    తార్కిక has_int = p > int_ప్రారంభం;

    _hb_fraction frac;
    ఒకవేళ (p < pe && *p == '.')
    {
      p++;
      అయితే (p < pe && '0' <= *p && *p <= '9')
      {
	frac.కూడు (*p - '0');
	p++;
      }
    }

    ఒకవేళ (!frac.is_empty ()) value += frac.get_విలువ ();
    లేకపోతే ఒకవేళ (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      తిరిగి .0;
    }
  }

  // Exponent
  గుర్తులేని ఘాతాంకం = 0;
  తార్కిక exp_neg = అబద్ధం, exp_అధికప్రవాహం = అబద్ధం;
  ఒకవేళ (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr గుర్తులేని MAX_EXP = 0x7FFu; /* 2^11-1 */
    స్థిరరాశి అక్షరం *before_exponent = p;
    p++;
    ఒకవేళ (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    ఒకవేళ (p < pe && '0' <= *p && *p <= '9')
      చేయి
      {
	ఒకవేళ (likely (ఘాతాంకం * 10 + (*p - '0') <= MAX_EXP))
	  ఘాతాంకం = ఘాతాంకం * 10 + (*p - '0');
	లేకపోతే
	  exp_అధికప్రవాహం = నిజం;
	p++;
      } అయితే (p < pe && '0' <= *p && *p <= '9');
    లేకపోతే
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  ఒకవేళ (neg) value *= -1.;

  ఒకవేళ (unlikely (exp_అధికప్రవాహం))
  {
    ఒకవేళ (value == 0) తిరిగి value;
    ఒకవేళ (exp_neg)    తిరిగి neg ? -.0 : .0;
    లేకపోతే            తిరిగి neg ? -DBL_MAX : DBL_MAX;
  }

  ఒకవేళ (ఘాతాంకం)
  {
    ఒకవేళ (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      ఒకవేళ (unlikely (ఘాతాంకం > 308))
      {
	value /= 1e308;
	ఘాతాంకం -= 308;
	ఒకవేళ (unlikely (ఘాతాంకం > 308))
	{
	  value /= 1e308;
	  ఘాతాంకం -= 308;
	}
      }
      value /= _pow10 (ఘాతాంకం);
    }
    లేకపోతే
      value *= _pow10 (ఘాతాంకం);
  }

  తిరిగి value;
}

#అయితే_ముగింపు /* HB_NUMBER_PARSER_HH */
