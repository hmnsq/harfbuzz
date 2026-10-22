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

#جيڪڏهن_وصف_نه_ٿيل HB_عدد_PARSER_HH
#وصف_ڪريو HB_عدد_PARSER_HH

#ضمن "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ساكن مضمن مزدوج
_pow10 (بنانشان exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  إذا (unlikely (exponent > 308)) أرجع HUGE_VAL;

  ساكن ثابت مزدوج powers_جو_10[] =
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
  بنانشان نقاب = 1 << (ARRAY_ڊيگهائي (powers_جو_10) - 1);
  مزدوج نتيجو = 1;
  لكل (ثابت مزدوج *power = powers_جو_10; نقاب; ++power, نقاب >>= 1)
    إذا (exponent & نقاب) نتيجو *= *power;
  أرجع نتيجو;
}

بنية _hb_fraction
{
  فراغ جوڙيو (صحيح digit)
  {
    إذا (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++ڳڻپ;
    }
    وإلا إذا (drop < 0) drop = digit;
    وإلا إذا (digit != 0) sticky = سچ;
  }

  مزدوج get_قدر () ثابت
  {
    مزدوج نتيجو = fraction;
    // Round-to-nearest-even using the recorded tail
    إذا (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      نتيجو += 1;
    أرجع نتيجو / _pow10 (ڳڻپ);
  }

  منطقي is_empty () ثابت { أرجع !ڳڻپ; }

خاص:
  بنانشان ڳڻپ = 0;
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
    ثابت حرف *int_شروعات = p;
    طالما (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    منطقي has_int = p > int_شروعات;

    _hb_fraction frac;
    إذا (p < pe && *p == '.')
    {
      p++;
      طالما (p < pe && '0' <= *p && *p <= '9')
      {
	frac.جوڙيو (*p - '0');
	p++;
      }
    }

    إذا (!frac.is_empty ()) value += frac.get_قدر ();
    وإلا إذا (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      أرجع .0;
    }
  }

  // Exponent
  بنانشان قوت_نما = 0;
  منطقي exp_neg = خطأ, exp_حد_کان_مٿي = خطأ;
  إذا (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr بنانشان MAX_EXP = 0x7FFu; /* 2^11-1 */
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
	إذا (likely (قوت_نما * 10 + (*p - '0') <= MAX_EXP))
	  قوت_نما = قوت_نما * 10 + (*p - '0');
	وإلا
	  exp_حد_کان_مٿي = سچ;
	p++;
      } طالما (p < pe && '0' <= *p && *p <= '9');
    وإلا
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  إذا (neg) value *= -1.;

  إذا (unlikely (exp_حد_کان_مٿي))
  {
    إذا (value == 0) أرجع value;
    إذا (exp_neg)    أرجع neg ? -.0 : .0;
    وإلا            أرجع neg ? -DBL_MAX : DBL_MAX;
  }

  إذا (قوت_نما)
  {
    إذا (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      إذا (unlikely (قوت_نما > 308))
      {
	value /= 1e308;
	قوت_نما -= 308;
	إذا (unlikely (قوت_نما > 308))
	{
	  value /= 1e308;
	  قوت_نما -= 308;
	}
      }
      value /= _pow10 (قوت_نما);
    }
    وإلا
      value *= _pow10 (قوت_نما);
  }

  أرجع value;
}

#جيڪڏهن_پورو /* HB_NUMBER_PARSER_HH */
