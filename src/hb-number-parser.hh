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

#अगर_अघोषित HB_संख्या_PARSER_HH
#घोषणा HB_संख्या_PARSER_HH

#समावेश "hb.hh"

// Handles 0..308; anything larger overflows double and returns HUGE_VAL.
अटल अंतरभूत द्विगुन
_pow10 (अचिन्हित exponent)
{
  // DBL_MAX is between 10^308 and 10^309
  अगर (unlikely (exponent > 308)) वापस विशाल_मान;

  अटल स्थिर द्विगुन _powers_of_10[] =
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
  अचिन्हित mask = 1 << (ARRAY_लंबाई (_powers_of_10) - 1);
  द्विगुन परिणाम = 1;
  क्रम (स्थिर द्विगुन *power = _powers_of_10; mask; ++power, mask >>= 1)
    अगर (exponent & mask) परिणाम *= *power;
  वापस परिणाम;
}

काष्ठा _hb_fraction
{
  व्योम add (पूर्णांक digit)
  {
    अगर (likely (fraction <= MAX_FRACT / 10))
    {
      fraction = fraction * 10. + digit;
      ++गिनती;
    }
    अन्यथा अगर (drop < 0) drop = digit;
    अन्यथा अगर (digit != 0) sticky = true;
  }

  द्विगुन get_मूल्य () स्थिर
  {
    द्विगुन परिणाम = fraction;
    // Round-to-nearest-even using the recorded tail
    अगर (drop > 5 || (drop == 5 && ((((uint64_t) fraction) & 1) || sticky)))
      परिणाम += 1;
    वापस परिणाम / _pow10 (गिनती);
  }

  bool is_empty () स्थिर { वापस !गिनती; }

निजी:
  अचिन्हित गिनती = 0;
  द्विगुन fraction = 0;
  पूर्णांक drop = -1; // drop is the first digit we couldn't accumulate
  bool sticky = false; // sticky is set when any later digit was nonzero.
  अटल constexpr uint64_t MAX_FRACT = 0xFFFFFFFFFFFFFull; /* 2^52-1 */
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
अटल अंतरभूत द्विगुन
hb_strtod (स्थिर अक्षर *p, स्थिर अक्षर **end_ptr /* IN/OUT */)
{
  स्थिर अक्षर *p_original = p;
  स्थिर अक्षर *pe = *end_ptr;

  // Skip leading whitespace
  जबतक (p < pe && ISSPACE (*p))
    p++;

  // Sign
  bool neg = false;
  अगर (p < pe && (*p == '+' || *p == '-'))
  {
    neg = (*p == '-');
    p++;
  }

  द्विगुन value = 0;

  // Integer and fraction
  {
    स्थिर अक्षर *int_start = p;
    जबतक (p < pe && '0' <= *p && *p <= '9')
    {
      value = value * 10. + (*p - '0');
      p++;
    }
    bool has_int = p > int_start;

    _hb_fraction frac;
    अगर (p < pe && *p == '.')
    {
      p++;
      जबतक (p < pe && '0' <= *p && *p <= '9')
      {
	frac.add (*p - '0');
	p++;
      }
    }

    अगर (!frac.is_empty ()) value += frac.get_मूल्य ();
    अन्यथा अगर (!has_int)
    {
      // Nothing could be parsed
      *end_ptr = p_original;
      वापस .0;
    }
  }

  // Exponent
  अचिन्हित exp = 0;
  bool exp_neg = false, exp_overflow = false;
  अगर (p < pe && (*p == 'e' || *p == 'E'))
  {
    constexpr अचिन्हित MAX_EXP = 0x7FFu; /* 2^11-1 */
    स्थिर अक्षर *before_exponent = p;
    p++;
    अगर (p < pe && (*p == '+' || *p == '-'))
    {
      exp_neg = (*p == '-');
      p++;
    }

    अगर (p < pe && '0' <= *p && *p <= '9')
      करो
      {
	अगर (likely (exp * 10 + (*p - '0') <= MAX_EXP))
	  exp = exp * 10 + (*p - '0');
	अन्यथा
	  exp_overflow = true;
	p++;
      } जबतक (p < pe && '0' <= *p && *p <= '9');
    अन्यथा
      p = before_exponent; // rewind
  }
  *end_ptr = p;

  अगर (neg) value *= -1.;

  अगर (unlikely (exp_overflow))
  {
    अगर (value == 0) वापस value;
    अगर (exp_neg)    वापस neg ? -.0 : .0;
    अन्यथा            वापस neg ? -द्विग_उच्च : द्विग_उच्च;
  }

  अगर (exp)
  {
    अगर (exp_neg)
    {
      // 10^308 is the largest finite power of ten, so two reductions of 308 suffice.
      अगर (unlikely (exp > 308))
      {
	value /= 1e308;
	exp -= 308;
	अगर (unlikely (exp > 308))
	{
	  value /= 1e308;
	  exp -= 308;
	}
      }
      value /= _pow10 (exp);
    }
    अन्यथा
      value *= _pow10 (exp);
  }

  वापस value;
}

#पूर्ण_अगर /* HB_NUMBER_PARSER_HH */
