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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸੰਖਿਆ_PARSER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੰਖਿਆ_PARSER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
_pow10 (ਬਿਨਾਨਿਸ਼ਾਨੀ exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  ਜੇ (unlikely (exponent > 308)) ਵਾਪਸ HUGE_VAL;

  ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ powers_ਦਾ_10[] =
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
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = 1 << (ARRAY_ਲੰਮਾਈ (powers_ਦਾ_10) - 1);
  ਦੁੱਗਣਾ ਨਤੀਜਾ = 1;
  ਲਈ (ਅਚਲ ਦੁੱਗਣਾ *power = powers_ਦਾ_10; ਨਕਾਬ; ++power, ਨਕਾਬ >>= 1)
    ਜੇ (exponent & ਨਕਾਬ) ਨਤੀਜਾ *= *power;
  ਵਾਪਸ ਨਤੀਜਾ;
}

ਬਣਤਰ _hb_fraction
{
  ਖਾਲੀ ਜੋੜੋ (ਪੂਰਨ digit)
  {
    ਜੇ (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++ਗਿਣਤੀ_2;
    }
    ਨਹੀਂਤਾਂ ਜੇ (drop < 0) drop = digit;
    ਨਹੀਂਤਾਂ ਜੇ (digit != 0) sticky = ਸੱਚ;
  }

  ਦੁੱਗਣਾ get_ਮੁੱਲ () ਅਚਲ
  {
    ਦੁੱਗਣਾ ਨਤੀਜਾ = fraction;
    // Round-to-nearest-even using the recorded tail
    ਜੇ (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      ਨਤੀਜਾ += 1;
    ਵਾਪਸ ਨਤੀਜਾ / _pow10 (ਗਿਣਤੀ_2);
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ !ਗਿਣਤੀ_2; }

ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = 0;
  ਦੁੱਗਣਾ fraction = 0;
  ਪੂਰਨ drop = -1; // drop is the first digit we couldn't accumulate
  ਤਾਰਕਿਕ sticky = ਝੂਠ; // sticky is set when any later digit was nonzero.
  ਸਥਿਰ constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
hb_strtod (ਅਚਲ ਅੱਖਰ *p, ਅਚਲ ਅੱਖਰ **end_ptr /* IN/OUT */)
{
  ਅਚਲ ਅੱਖਰ *p_original = p;
  ਅਚਲ ਅੱਖਰ *pe = *end_ptr;

  // Skip leading whitespace
  ਜਦੋਂ (p < pe && ISSPACE (*p))
    p++;

  // Sign
  ਤਾਰਕਿਕ neg = ਝੂਠ;
  ਜੇ (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  ਦੁੱਗਣਾ value = 0;

  // Integer and fraction
  {
    ਅਚਲ ਅੱਖਰ *int_ਸ਼ੁਰੂ = p;
    ਜਦੋਂ (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    ਤਾਰਕਿਕ has_int = p > int_ਸ਼ੁਰੂ;

    _hb_fraction frac;
    ਜੇ (p < pe && *p == '.')
    {
      p++;
      ਜਦੋਂ (p < pe && '0' <= *p && *p <= '9')
      {
	frac.ਜੋੜੋ (*p - '0');
	p++;
      }
    }

    ਜੇ (!frac.is_empty ()) value += frac.get_ਮੁੱਲ ();
    ਨਹੀਂਤਾਂ ਜੇ (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      ਵਾਪਸ .0;
    }
  }

  // Exponent
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਘਾਤਾਂਕ = 0;
  ਤਾਰਕਿਕ exp_neg = ਝੂਠ, exp_ਉਛਾਲ = ਝੂਠ;
  ਜੇ (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_EXP = 0x7FFu; /* 2^11-1 */
    ਅਚਲ ਅੱਖਰ *before_exponent = p;
    p++;
    ਜੇ (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    ਜੇ (p < pe && '0' <= *p && *p <= '9')
      ਕਰੋ
      {
	ਜੇ (likely (ਘਾਤਾਂਕ * 10 + (*p - '0') <= MAX_EXP))
	  ਘਾਤਾਂਕ = ਘਾਤਾਂਕ * 10 + (*p - '0');
	ਨਹੀਂਤਾਂ
	  exp_ਉਛਾਲ = ਸੱਚ;
	p++;
      } ਜਦੋਂ (p < pe && '0' <= *p && *p <= '9');
    ਨਹੀਂਤਾਂ
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  ਜੇ (neg) value *= -1.;

  ਜੇ (unlikely (exp_ਉਛਾਲ))
  {
    ਜੇ (value == 0) ਵਾਪਸ value;
    ਜੇ (exp_neg)    ਵਾਪਸ neg ? -.0 : .0;
    ਨਹੀਂਤਾਂ            ਵਾਪਸ neg ? -DBL_MAX : DBL_MAX;
  }

  ਜੇ (ਘਾਤਾਂਕ)
  {
    ਜੇ (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      ਜੇ (unlikely (ਘਾਤਾਂਕ > 308))
      {
	value /= 1e308;
	ਘਾਤਾਂਕ -= 308;
	ਜੇ (unlikely (ਘਾਤਾਂਕ > 308))
	{
	  value /= 1e308;
	  ਘਾਤਾਂਕ -= 308;
	}
      }
      value /= _pow10 (ਘਾਤਾਂਕ);
    }
    ਨਹੀਂਤਾਂ
      value *= _pow10 (ਘਾਤਾਂਕ);
  }

  ਵਾਪਸ value;
}

#ਜੇ_ਅੰਤ /* HB_NUMBER_PARSER_HH */
