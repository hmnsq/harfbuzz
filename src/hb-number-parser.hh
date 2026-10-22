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

#ifndef HB_সংখ্যা_PARSER_HH
#define HB_সংখ্যা_PARSER_HH

#অন্তর্ভাব "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
স্থির অন্তঃস্থ দ্বিগুণ
_pow10 (unsigned exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  যদি (unlikely (exponent > 308)) প্রত্যাগম HUGE_VAL;

  স্থির ধ্রুব দ্বিগুণ _powers_of_10[] =
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
  দ্বিগুণ result = 1;
  প্রতি (ধ্রুব দ্বিগুণ *power = _powers_of_10; mask; ++power, mask >>= 1)
    যদি (exponent & mask) result *= *power;
  প্রত্যাগম result;
}

সংরচনা _hb_fraction
{
  শূন্য add (পূর্ণাঙ্ক digit)
  {
    যদি (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++গুনতি;
    }
    অন্যথা যদি (drop < 0) drop = digit;
    অন্যথা যদি (digit != 0) sticky = সত্য;
  }

  দ্বিগুণ get_মান () ধ্রুব
  {
    দ্বিগুণ result = fraction;
    // Round-to-nearest-even using the recorded tail
    যদি (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      result += 1;
    প্রত্যাগম result / _pow10 (গুনতি);
  }

  তার্কিক is_empty () ধ্রুব { প্রত্যাগম !গুনতি; }

গুপ্ত:
  unsigned গুনতি = 0;
  দ্বিগুণ fraction = 0;
  পূর্ণাঙ্ক drop = -1; // drop is the first digit we couldn't accumulate
  তার্কিক sticky = অসত্য; // sticky is set when any later digit was nonzero.
  স্থির constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
স্থির অন্তঃস্থ দ্বিগুণ
hb_strtod (ধ্রুব বর্ণ *p, ধ্রুব বর্ণ **end_ptr /* IN/OUT */)
{
  ধ্রুব বর্ণ *p_original = p;
  ধ্রুব বর্ণ *pe = *end_ptr;

  // Skip leading whitespace
  যাবত্ (p < pe && ISSPACE (*p))
    p++;

  // Sign
  তার্কিক neg = অসত্য;
  যদি (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  দ্বিগুণ value = 0;

  // Integer and fraction
  {
    ধ্রুব বর্ণ *int_start = p;
    যাবত্ (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    তার্কিক has_int = p > int_start;

    _hb_fraction frac;
    যদি (p < pe && *p == '.')
    {
      p++;
      যাবত্ (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    যদি (!frac.is_empty ()) value += frac.get_মান ();
    অন্যথা যদি (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      প্রত্যাগম .0;
    }
  }

  // Exponent
  unsigned exp = 0;
  তার্কিক exp_neg = অসত্য, exp_overflow = অসত্য;
  যদি (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr unsigned MAX_EXP = 0x7FFu; /* 2^11-1 */
    ধ্রুব বর্ণ *before_exponent = p;
    p++;
    যদি (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    যদি (p < pe && '0' <= *p && *p <= '9')
      কুরু
      {
	যদি (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	অন্যথা
	  exp_overflow = সত্য;
	p++;
      } যাবত্ (p < pe && '0' <= *p && *p <= '9');
    অন্যথা
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  যদি (neg) value *= -1.;

  যদি (unlikely (exp_overflow))
  {
    যদি (value == 0) প্রত্যাগম value;
    যদি (exp_neg)    প্রত্যাগম neg ? -.0 : .0;
    অন্যথা            প্রত্যাগম neg ? -DBL_MAX : DBL_MAX;
  }

  যদি (exp)
  {
    যদি (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      যদি (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	যদি (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    অন্যথা
      value *= _pow10 (exp);
  }

  প্রত্যাগম value;
}

#endif /* HB_NUMBER_PARSER_HH */
