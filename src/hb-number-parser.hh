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

#ifndef HB_எண்_PARSER_HH
#define HB_எண்_PARSER_HH

#அந்தர்பாவ "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
நிலையான உள்வரி இரட்டை
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  எனில் (unlikely (exponent > 308)) திரும்பு HUGE_VAL;

  நிலையான மாறிலி இரட்டை _powers_of_10[] =
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
  இரட்டை result = 1;
  க்கு (மாறிலி இரட்டை *power = _powers_of_10; mask; ++power, mask >>= 1)
    எனில் (exponent & mask) result *= *power;
  திரும்பு result;
}

கட்டமைப்பு _hb_fraction
{
  வெறுமை add (முழு digit)
  {
    எனில் (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++மொத்தம்;
    }
    இல்லையெனில் எனில் (drop < 0) drop = digit;
    இல்லையெனில் எனில் (digit != 0) sticky = உண்மை;
  }

  இரட்டை get_மதிப்பு () மாறிலி
  {
    இரட்டை result = fraction;
    // Round-to-nearest-even using the recorded tail
    எனில் (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    திரும்பு result / _pow10 (மொத்தம்);
  }

  தருக்க is_empty () மாறிலி { திரும்பு !மொத்தம்; }

தனிப்பட்ட:
  unsigned மொத்தம் = 0;
  இரட்டை fraction = 0;
  முழு drop = -1; // drop is the first digit we couldn't accumulate
  தருக்க sticky = பொய்; // sticky is set when any later digit was nonzero.
  நிலையான constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
நிலையான உள்வரி இரட்டை
hb_strtod (மாறிலி எழுத்து *p, மாறிலி எழுத்து **end_ptr /* IN/OUT */)
{
  மாறிலி எழுத்து *p_original = p;
  மாறிலி எழுத்து *pe = *end_ptr;

  // Skip leading whitespace
  போது (p < pe && ISSPACE (*p))
    p++;

  // Sign
  தருக்க neg = பொய்;
  எனில் (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  இரட்டை value = 0;

  // Integer and fraction
  {
    மாறிலி எழுத்து *int_start = p;
    போது (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    தருக்க has_int = p > int_start;

    _hb_fraction frac;
    எனில் (p < pe && *p == '.')
    {
      p++;
      போது (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    எனில் (!frac.is_empty ()) value += frac.get_மதிப்பு ();
    இல்லையெனில் எனில் (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      திரும்பு .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  தருக்க exp_neg = பொய், exp_overflow = பொய்;
  எனில் (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    மாறிலி எழுத்து *before_exponent = p;
    p++;
    எனில் (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    எனில் (p < pe && '0' <= *p && *p <= '9')
      செய்
      {
	எனில் (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	இல்லையெனில்
	  exp_overflow = உண்மை;
	p++;
      } போது (p < pe && '0' <= *p && *p <= '9');
    இல்லையெனில்
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  எனில் (neg) value *= -1.;

  எனில் (unlikely (exp_overflow))
  {
    எனில் (value == 0) திரும்பு value;
    எனில் (exp_neg)    திரும்பு neg ? -.0 : .0;
    இல்லையெனில்            திரும்பு neg ? -DBL_MAX : DBL_MAX;
  }

  எனில் (exp)
  {
    எனில் (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      எனில் (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	எனில் (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    இல்லையெனில்
      value *= _pow10 (exp);
  }

  திரும்பு value;
}

#endif /* HB_NUMBER_PARSER_HH */
