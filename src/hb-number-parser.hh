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

#ifndef HB_عدد_PARSER_HH
#define HB_عدد_PARSER_HH

#شامل "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ایستا درونخطی دوگانه
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  اگر (unlikely (exponent > 308)) بازگشت HUGE_VAL;

  ایستا ثابت دوگانه _powers_of_10[] =
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
  دوگانه result = 1;
  برای (ثابت دوگانه *power = _powers_of_10; mask; ++power, mask >>= 1)
    اگر (exponent & mask) result *= *power;
  بازگشت result;
}

ساختار _hb_fraction
{
  تهی add (صحیح digit)
  {
    اگر (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++شمار;
    }
    وگرنه اگر (drop < 0) drop = digit;
    وگرنه اگر (digit != 0) sticky = درست;
  }

  دوگانه get_مقدار () ثابت
  {
    دوگانه result = fraction;
    // Round-to-nearest-even using the recorded tail
    اگر (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    بازگشت result / _pow10 (شمار);
  }

  منطقی is_empty () ثابت { بازگشت !شمار; }

خصوصی:
  unsigned شمار = 0;
  دوگانه fraction = 0;
  صحیح drop = -1; // drop is the first digit we couldn't accumulate
  منطقی sticky = نادرست; // sticky is set when any later digit was nonzero.
  ایستا constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
ایستا درونخطی دوگانه
hb_strtod (ثابت نویسه *p, ثابت نویسه **end_ptr /* IN/OUT */)
{
  ثابت نویسه *p_original = p;
  ثابت نویسه *pe = *end_ptr;

  // Skip leading whitespace
  وقتی (p < pe && ISSPACE (*p))
    p++;

  // Sign
  منطقی neg = نادرست;
  اگر (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  دوگانه value = 0;

  // Integer and fraction
  {
    ثابت نویسه *int_start = p;
    وقتی (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    منطقی has_int = p > int_start;

    _hb_fraction frac;
    اگر (p < pe && *p == '.')
    {
      p++;
      وقتی (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    اگر (!frac.is_empty ()) value += frac.get_مقدار ();
    وگرنه اگر (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      بازگشت .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  منطقی exp_neg = نادرست, exp_overflow = نادرست;
  اگر (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ثابت نویسه *before_exponent = p;
    p++;
    اگر (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    اگر (p < pe && '0' <= *p && *p <= '9')
      انجامبده
      {
	اگر (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	وگرنه
	  exp_overflow = درست;
	p++;
      } وقتی (p < pe && '0' <= *p && *p <= '9');
    وگرنه
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  اگر (neg) value *= -1.;

  اگر (unlikely (exp_overflow))
  {
    اگر (value == 0) بازگشت value;
    اگر (exp_neg)    بازگشت neg ? -.0 : .0;
    وگرنه            بازگشت neg ? -DBL_MAX : DBL_MAX;
  }

  اگر (exp)
  {
    اگر (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      اگر (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	اگر (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    وگرنه
      value *= _pow10 (exp);
  }

  بازگشت value;
}

#endif /* HB_NUMBER_PARSER_HH */
