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

#اگر_غیر_تعریف_شدہ HB_نمبر_PARSER_HH
#تعریف_کرو HB_نمبر_PARSER_HH

#شامل "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
ساکن اندرونی دوہرا
_pow10 (غیرعلامتی exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  اگر (unlikely (exponent > 308)) واپس HUGE_VAL;

  ساکن ثابت دوہرا powers_کا_10[] =
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
  غیرعلامتی نقاب = 1 << (ARRAY_لمبائی (powers_کا_10) - 1);
  دوہرا نتیجہ = 1;
  برائے (ثابت دوہرا *power = powers_کا_10; نقاب; ++power, نقاب >>= 1)
    اگر (exponent & نقاب) نتیجہ *= *power;
  واپس نتیجہ;
}

ساخت _hb_fraction
{
  خالی جوڑو (صحیح digit)
  {
    اگر (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++گنتی;
    }
    ورنہ اگر (drop < 0) drop = digit;
    ورنہ اگر (digit != 0) sticky = سچ;
  }

  دوہرا get_قدر () ثابت
  {
    دوہرا نتیجہ = fraction;
    // Round-to-nearest-even using the recorded tail
    اگر (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      نتیجہ += 1;
    واپس نتیجہ / _pow10 (گنتی);
  }

  منطقی is_empty () ثابت { واپس !گنتی; }

نجی:
  غیرعلامتی گنتی = 0;
  دوہرا fraction = 0;
  صحیح drop = -1; // drop is the first digit we couldn't accumulate
  منطقی sticky = جھوٹ; // sticky is set when any later digit was nonzero.
  ساکن constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
ساکن اندرونی دوہرا
hb_strtod (ثابت حرف *p, ثابت حرف **end_ptr /* IN/OUT */)
{
  ثابت حرف *p_original = p;
  ثابت حرف *pe = *end_ptr;

  // Skip leading whitespace
  جب (p < pe && ISSPACE (*p))
    p++;

  // Sign
  منطقی neg = جھوٹ;
  اگر (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  دوہرا value = 0;

  // Integer and fraction
  {
    ثابت حرف *int_آغاز = p;
    جب (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    منطقی has_int = p > int_آغاز;

    _hb_fraction frac;
    اگر (p < pe && *p == '.')
    {
      p++;
      جب (p < pe && '0' <= *p && *p <= '9')
      {
	frac.جوڑو (*p - '0');
	p++;
      }
    }

    اگر (!frac.is_empty ()) value += frac.get_قدر ();
    ورنہ اگر (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      واپس .0;
    }
  }

  // Exponent
  غیرعلامتی قوت_نما = 0;
  منطقی exp_neg = جھوٹ, exp_حد_سے_تجاوز = جھوٹ;
  اگر (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr غیرعلامتی MAX_EXP = 0x7FFu; /* 2^11-1 */
    ثابت حرف *before_exponent = p;
    p++;
    اگر (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    اگر (p < pe && '0' <= *p && *p <= '9')
      کرو
      {
	اگر (likely (قوت_نما * 10 + (*p - '0') <= MAX_EXP))
	  قوت_نما = قوت_نما * 10 + (*p - '0');
	ورنہ
	  exp_حد_سے_تجاوز = سچ;
	p++;
      } جب (p < pe && '0' <= *p && *p <= '9');
    ورنہ
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  اگر (neg) value *= -1.;

  اگر (unlikely (exp_حد_سے_تجاوز))
  {
    اگر (value == 0) واپس value;
    اگر (exp_neg)    واپس neg ? -.0 : .0;
    ورنہ            واپس neg ? -DBL_MAX : DBL_MAX;
  }

  اگر (قوت_نما)
  {
    اگر (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      اگر (unlikely (قوت_نما > 308))
      {
	value /= 1e308;
	قوت_نما -= 308;
	اگر (unlikely (قوت_نما > 308))
	{
	  value /= 1e308;
	  قوت_نما -= 308;
	}
      }
      value /= _pow10 (قوت_نما);
    }
    ورنہ
      value *= _pow10 (قوت_نما);
  }

  واپس value;
}

#اگر_ختم /* HB_NUMBER_PARSER_HH */
