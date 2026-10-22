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

#ifndef HB_ಸಂಖ್ಯೆ_PARSER_HH
#define HB_ಸಂಖ್ಯೆ_PARSER_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  ಯದಿ (unlikely (exponent > 308)) ಪ್ರತ್ಯಾಗಮ HUGE_VAL;

  ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ _powers_of_10[] =
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
  ದ್ವಿಗುಣ result = 1;
  ಪ್ರತಿ (ಧ್ರುವ ದ್ವಿಗುಣ *power = _powers_of_10; mask; ++power, mask >>= 1)
    ಯದಿ (exponent & mask) result *= *power;
  ಪ್ರತ್ಯಾಗಮ result;
}

ಸಂರಚನಾ _hb_fraction
{
  ಶೂನ್ಯ add (ಪೂರ್ಣಾಙ್ಕ digit)
  {
    ಯದಿ (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++ಎಣಿಕೆ;
    }
    ಅನ್ಯಥಾ ಯದಿ (drop < 0) drop = digit;
    ಅನ್ಯಥಾ ಯದಿ (digit != 0) sticky = ಸತ್ಯ;
  }

  ದ್ವಿಗುಣ get_ಮೌಲ್ಯ () ಧ್ರುವ
  {
    ದ್ವಿಗುಣ result = fraction;
    // Round-to-nearest-even using the recorded tail
    ಯದಿ (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    ಪ್ರತ್ಯಾಗಮ result / _pow10 (ಎಣಿಕೆ);
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !ಎಣಿಕೆ; }

ಗುಪ್ತ:
  unsigned ಎಣಿಕೆ = 0;
  ದ್ವಿಗುಣ fraction = 0;
  ಪೂರ್ಣಾಙ್ಕ drop = -1; // drop is the first digit we couldn't accumulate
  ತಾರ್ಕಿಕ sticky = ಅಸತ್ಯ; // sticky is set when any later digit was nonzero.
  ಸ್ಥಿರ constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ
hb_strtod (ಧ್ರುವ ವರ್ಣ *p, ಧ್ರುವ ವರ್ಣ **end_ptr /* IN/OUT */)
{
  ಧ್ರುವ ವರ್ಣ *p_original = p;
  ಧ್ರುವ ವರ್ಣ *pe = *end_ptr;

  // Skip leading whitespace
  ಯಾವತ್ (p < pe && ISSPACE (*p))
    p++;

  // Sign
  ತಾರ್ಕಿಕ neg = ಅಸತ್ಯ;
  ಯದಿ (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  ದ್ವಿಗುಣ value = 0;

  // Integer and fraction
  {
    ಧ್ರುವ ವರ್ಣ *int_start = p;
    ಯಾವತ್ (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    ತಾರ್ಕಿಕ has_int = p > int_start;

    _hb_fraction frac;
    ಯದಿ (p < pe && *p == '.')
    {
      p++;
      ಯಾವತ್ (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    ಯದಿ (!frac.is_empty ()) value += frac.get_ಮೌಲ್ಯ ();
    ಅನ್ಯಥಾ ಯದಿ (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      ಪ್ರತ್ಯಾಗಮ .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  ತಾರ್ಕಿಕ exp_neg = ಅಸತ್ಯ, exp_overflow = ಅಸತ್ಯ;
  ಯದಿ (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ಧ್ರುವ ವರ್ಣ *before_exponent = p;
    p++;
    ಯದಿ (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    ಯದಿ (p < pe && '0' <= *p && *p <= '9')
      ಕುರು
      {
	ಯದಿ (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	ಅನ್ಯಥಾ
	  exp_overflow = ಸತ್ಯ;
	p++;
      } ಯಾವತ್ (p < pe && '0' <= *p && *p <= '9');
    ಅನ್ಯಥಾ
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  ಯದಿ (neg) value *= -1.;

  ಯದಿ (unlikely (exp_overflow))
  {
    ಯದಿ (value == 0) ಪ್ರತ್ಯಾಗಮ value;
    ಯದಿ (exp_neg)    ಪ್ರತ್ಯಾಗಮ neg ? -.0 : .0;
    ಅನ್ಯಥಾ            ಪ್ರತ್ಯಾಗಮ neg ? -DBL_MAX : DBL_MAX;
  }

  ಯದಿ (exp)
  {
    ಯದಿ (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      ಯದಿ (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	ಯದಿ (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    ಅನ್ಯಥಾ
      value *= _pow10 (exp);
  }

  ಪ್ರತ್ಯಾಗಮ value;
}

#endif /* HB_NUMBER_PARSER_HH */
