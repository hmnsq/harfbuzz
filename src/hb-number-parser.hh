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

#ifndef HB_نومبور_PARSER_HH
#define HB_نومبور_PARSER_HH

#ضمن "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ساكن مضمن مزدوج
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  إذا (unlikely (exponent > 308)) أرجع HUGE_VAL;

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
  لكل (ثابت مزدوج *power = _powers_of_10; mask; ++power, mask >>= 1)
    إذا (exponent & mask) result *= *power;
  أرجع result;
}

بنية _hb_fraction
{
  فراغ add (صحيح digit)
  {
    إذا (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++کيراءن;
    }
    وإلا إذا (drop < 0) drop = digit;
    وإلا إذا (digit != 0) sticky = true;
  }

  مزدوج get_نيلاي () ثابت
  {
    مزدوج result = fraction;
    // Round-to-nearest-even using the recorded tail
    إذا (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    أرجع result / _pow10 (کيراءن);
  }

  منطقي is_empty () ثابت { أرجع !کيراءن; }

خاص:
  unsigned کيراءن = 0;
  مزدوج fraction = 0;
  صحيح drop = -1; // drop is the first digit we couldn't accumulate
  منطقي sticky = خطأ; // sticky is set when any later digit was nonzero.
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
  طالما (p < pe && ISSPACE (*p))
    p++;

  // Sign
  منطقي neg = خطأ;
  إذا (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  مزدوج value = 0;

  // Integer and fraction
  {
    ثابت حرف *int_start = p;
    طالما (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    منطقي has_int = p > int_start;

    _hb_fraction frac;
    إذا (p < pe && *p == '.')
    {
      p++;
      طالما (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    إذا (!frac.is_empty ()) value += frac.get_نيلاي ();
    وإلا إذا (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      أرجع .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  منطقي exp_neg = خطأ, exp_overflow = خطأ;
  إذا (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ثابت حرف *before_exponent = p;
    p++;
    إذا (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    إذا (p < pe && '0' <= *p && *p <= '9')
      افعل
      {
	إذا (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	وإلا
	  exp_overflow = true;
	p++;
      } طالما (p < pe && '0' <= *p && *p <= '9');
    وإلا
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  إذا (neg) value *= -1.;

  إذا (unlikely (exp_overflow))
  {
    إذا (value == 0) أرجع value;
    إذا (exp_neg)    أرجع neg ? -.0 : .0;
    وإلا            أرجع neg ? -DBL_MAX : DBL_MAX;
  }

  إذا (exp)
  {
    إذا (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      إذا (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	إذا (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    وإلا
      value *= _pow10 (exp);
  }

  أرجع value;
}

#endif /* HB_NUMBER_PARSER_HH */
