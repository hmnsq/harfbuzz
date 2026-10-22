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

#ifndef HB_NUMBER_PARSER_HH
#define HB_NUMBER_PARSER_HH

#ضمن "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ساكن مضمن مزدوج
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  אם (unlikely (exponent > 308)) החזר HUGE_VAL;

  ساكن ثابت مزدوج _powers_of_10[] =
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
  مزدوج result = 1;
  עבור (ثابت مزدوج *power = _powers_of_10; mask; ++power, mask >>= 1)
    אם (exponent & mask) result *= *power;
  החזר result;
}

بنية _hb_fraction
{
  فراغ add (صحيح digit)
  {
    אם (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++count;
    }
    אחרת אם (drop < 0) drop = digit;
    אחרת אם (digit != 0) sticky = אמת;
  }

  مزدوج get_value () ثابت
  {
    مزدوج result = fraction;
    // Round-to-nearest-even using the recorded tail
    אם (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    החזר result / _pow10 (count);
  }

  منطقي is_empty () ثابت { החזר !count; }

خاص:
  unsigned count = 0;
  مزدوج fraction = 0;
  صحيح drop = -1; // drop is the first digit we couldn't accumulate
  منطقي sticky = שקר; // sticky is set when any later digit was nonzero.
  ساكن constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
ساكن مضمن مزدوج
hb_strtod (ثابت حرف *p, ثابت حرف **end_ptr /* IN/OUT */)
{
  ثابت حرف *p_original = p;
  ثابت حرف *pe = *end_ptr;

  // Skip leading whitespace
  כלעוד (p < pe && ISSPACE (*p))
    p++;

  // Sign
  منطقي neg = שקר;
  אם (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  مزدوج value = 0;

  // Integer and fraction
  {
    ثابت حرف *int_start = p;
    כלעוד (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    منطقي has_int = p > int_start;

    _hb_fraction frac;
    אם (p < pe && *p == '.')
    {
      p++;
      כלעוד (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    אם (!frac.is_empty ()) value += frac.get_value ();
    אחרת אם (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      החזר .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  منطقي exp_neg = שקר, exp_overflow = שקר;
  אם (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ثابت حرف *before_exponent = p;
    p++;
    אם (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    אם (p < pe && '0' <= *p && *p <= '9')
      עשה
      {
	אם (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	אחרת
	  exp_overflow = אמת;
	p++;
      } כלעוד (p < pe && '0' <= *p && *p <= '9');
    אחרת
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  אם (neg) value *= -1.;

  אם (unlikely (exp_overflow))
  {
    אם (value == 0) החזר value;
    אם (exp_neg)    החזר neg ? -.0 : .0;
    אחרת            החזר neg ? -DBL_MAX : DBL_MAX;
  }

  אם (exp)
  {
    אם (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      אם (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	אם (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    אחרת
      value *= _pow10 (exp);
  }

  החזר value;
}

#endif /* HB_NUMBER_PARSER_HH */
