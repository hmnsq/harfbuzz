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

#ifndef HB_सङ्ख्या_PARSER_HH
#define HB_सङ्ख्या_PARSER_HH

#अन्तर्भाव "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
स्थिर अन्तःस्थ द्विगुण
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  जइ (unlikely (exponent > 308)) प्रत्यागम HUGE_VAL;

  स्थिर ध्रुव द्विगुण _powers_of_10[] =
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
  unsigned mask = 1 << (ARRAY_LENGTH (_powers_of_10) - 1);
  द्विगुण result = 1;
  प्रति (ध्रुव द्विगुण *power = _powers_of_10; mask; ++power, mask >>= 1)
    जइ (exponent & mask) result *= *power;
  प्रत्यागम result;
}

संरचना _hb_fraction
{
  शून्य add (पूर्णाङ्क digit)
  {
    जइ (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++गणनम्;
    }
    अण्णहा जइ (drop < 0) drop = digit;
    अण्णहा जइ (digit != 0) sticky = सच्च;
  }

  द्विगुण get_मानम् () ध्रुव
  {
    द्विगुण result = fraction;
    // Round-to-nearest-even using the recorded tail
    जइ (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    प्रत्यागम result / _pow10 (गणनम्);
  }

  तार्किक is_empty () ध्रुव { प्रत्यागम !गणनम्; }

गुप्त:
  unsigned गणनम् = 0;
  द्विगुण fraction = 0;
  पूर्णाङ्क drop = -1; // drop is the first digit we couldn't accumulate
  तार्किक sticky = मिच्छ; // sticky is set when any later digit was nonzero.
  स्थिर constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
स्थिर अन्तःस्थ द्विगुण
hb_strtod (ध्रुव वर्ण *p, ध्रुव वर्ण **end_ptr /* IN/OUT */)
{
  ध्रुव वर्ण *p_original = p;
  ध्रुव वर्ण *pe = *end_ptr;

  // Skip leading whitespace
  जाव (p < pe && ISSPACE (*p))
    p++;

  // Sign
  तार्किक neg = मिच्छ;
  जइ (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  द्विगुण value = 0;

  // Integer and fraction
  {
    ध्रुव वर्ण *int_start = p;
    जाव (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    तार्किक has_int = p > int_start;

    _hb_fraction frac;
    जइ (p < pe && *p == '.')
    {
      p++;
      जाव (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    जइ (!frac.is_empty ()) value += frac.get_मानम् ();
    अण्णहा जइ (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      प्रत्यागम .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  तार्किक exp_neg = मिच्छ, exp_overflow = मिच्छ;
  जइ (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ध्रुव वर्ण *before_exponent = p;
    p++;
    जइ (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    जइ (p < pe && '0' <= *p && *p <= '9')
      करेहि
      {
	जइ (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	अण्णहा
	  exp_overflow = सच्च;
	p++;
      } जाव (p < pe && '0' <= *p && *p <= '9');
    अण्णहा
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  जइ (neg) value *= -1.;

  जइ (unlikely (exp_overflow))
  {
    जइ (value == 0) प्रत्यागम value;
    जइ (exp_neg)    प्रत्यागम neg ? -.0 : .0;
    अण्णहा            प्रत्यागम neg ? -DBL_MAX : DBL_MAX;
  }

  जइ (exp)
  {
    जइ (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      जइ (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	जइ (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    अण्णहा
      value *= _pow10 (exp);
  }

  प्रत्यागम value;
}

#endif /* HB_NUMBER_PARSER_HH */
