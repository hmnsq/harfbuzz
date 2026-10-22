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

#अन्तर्भाव <cmath>
#अन्तर्भाव <cstdlib>

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-number.hh"

स्थिर शून्य
test_parse_int ()
{
  {
    स्थिरांक अक्षर str[] = "123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + 3;

    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    स्थिरांक अक्षर str[] = "-123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + 4;

    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    स्थिरांक अक्षर str[] = "   123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "\t123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "\n123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = " -123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    स्थिरांक अक्षर str[] = "+123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "+0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    स्थिरांक अक्षर str[] = "007";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    स्थिरांक अक्षर str[] = "-0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    स्थिरांक अक्षर str[] = "2147483647";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "-2147483648";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    स्थिरांक अक्षर str[] = "   ";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    स्थिरांक अक्षर str[] = "+";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "-";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    स्थिरांक अक्षर str[] = "++1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "--1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "+-1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "-+1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    स्थिरांक अक्षर str[] = " 123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, सत्य));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    स्थिरांक अक्षर str[] = "9999999999";   /* 10 digits, > INT_MAX */
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    स्थिरांक अक्षर str[] = "2147483647";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "-2147483648";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    स्थिरांक अक्षर str[] = "2147483648";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    स्थिरांक अक्षर str[] = "-2147483649";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    स्थिरांक अक्षर str[] = "9999999999999999999999999999999999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    स्थिरांक अक्षर str[] = "9999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    स्थिरांक अक्षर str[] = "- 123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    स्थिरांक अक्षर str[] = "+0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    स्थिरांक अक्षर str[] = "-   ";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

स्थिर शून्य
test_parse_uint ()
{
  {
    स्थिरांक अक्षर str[] = "123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + म_माप (str);

    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    स्थिरांक अक्षर str[] = "12F";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + 3;

    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    स्थिरांक अक्षर str[] = "12Fq";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + 4;

    अचिन्हित पूर्णांक pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    स्थिरांक अक्षर str[] = "123";
    स्थिरांक अक्षर *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str);

    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    स्थिरांक अक्षर str[] = "123\0";
    स्थिरांक अक्षर *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str);

    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    स्थिरांक अक्षर str[] = "123V";
    स्थिरांक अक्षर *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str);

    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, सत्य));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    स्थिरांक अक्षर str[] = " 123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "\t123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    स्थिरांक अक्षर str[] = "007";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "000";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    स्थिरांक अक्षर str[] = "4294967295";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    स्थिरांक अक्षर str[] = "-1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "-0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    स्थिरांक अक्षर str[] = "1010";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "11111111";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "12";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    स्थिरांक अक्षर str[] = "777";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "010";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    स्थिरांक अक्षर str[] = "FF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "ff";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "Ff";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "0xFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "0XFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    स्थिरांक अक्षर str[] = "Z";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "z";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    स्थिरांक अक्षर str[] = "abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, असत्य, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    स्थिरांक अक्षर str[] = "+";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "-";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    स्थिरांक अक्षर str[] = "0XFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "0xff";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "0x0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    स्थिरांक अक्षर str[] = "0x";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    स्थिरांक अक्षर str[] = "0xG";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    स्थिरांक अक्षर str[] = "-0xFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == (अचिन्हित) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    स्थिरांक अक्षर str[] = "0xFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    स्थिरांक अक्षर str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    स्थिरांक अक्षर str[] = "4294967295";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    स्थिरांक अक्षर str[] = "4294967296";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    स्थिरांक अक्षर str[] = "99999999999999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    स्थिरांक अक्षर str[] = "9999999999999999999999999999999999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    स्थिरांक अक्षर str[] = "0xFFFFFFFF";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    स्थिरांक अक्षर str[] = "0x100000000";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    स्थिरांक अक्षर str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, सत्य, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    स्थिरांक अक्षर str[] = "9999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    स्थिरांक अक्षर str[] = "-4294967296";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    स्थिरांक अक्षर str[] = "-2147483648";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    स्थिरांक अक्षर str[] = "Az";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    स्थिरांक अक्षर str[] = "- 1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    स्थिरांक अक्षर str[] = "0X10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, सत्य, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    स्थिरांक अक्षर str[] = "0x0x10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, असत्य, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    स्थिरांक अक्षर str[] = "12\0F";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    अचिन्हित पूर्णांक pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
स्थिर ओळीत तार्किक
almost_equal (द्विगुण a, द्विगुण b)
{
  परत भ_असल (a - b) <= भ_असल (a) * 2 * द्विग_अंतर;
}

स्थिर शून्य
test_parse_double (शून्य)
{
  {
    स्थिरांक अक्षर str[] = ".123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str);

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    स्थिरांक अक्षर str[] = "0.123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    स्थिरांक अक्षर str[] = "0.123e0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    स्थिरांक अक्षर str[] = "123e-3";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    स्थिरांक अक्षर str[] = ".000123e+3";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    स्थिरांक अक्षर str[] = "-.000000123e6";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    स्थिरांक अक्षर str[] = "-1.23E-1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;

    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((पूर्णांक) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    स्थिरांक अक्षर str[] = "1e512";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    स्थिरांक अक्षर str[] = "1e600";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    स्थिरांक अक्षर str[] = "1e-512";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    स्थिरांक अक्षर str[] = "1e-600";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    स्थिरांक अक्षर str[] = "0.9999999999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    स्थिरांक अक्षर str[] = "0.499999999999999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    स्थिरांक अक्षर str[] = "1e";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    स्थिरांक अक्षर str[] = "1e+";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    स्थिरांक अक्षर str[] = "-";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    स्थिरांक अक्षर str[] = ".";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    स्थिरांक अक्षर str[] = "1e-";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    स्थिरांक अक्षर str[] = "-1e";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    स्थिरांक अक्षर str[] = "+";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    स्थिरांक अक्षर str[] = "-.";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    स्थिरांक अक्षर str[] = "1.";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "1.abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    स्थिरांक अक्षर str[] = "abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = "";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = " \t 123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "-0.0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    स्थिरांक अक्षर str[] = "1.5E10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 1.5e10);
  }
  {
    स्थिरांक अक्षर str[] = "1.5E+10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 1.5e10);
  }
  {
    स्थिरांक अक्षर str[] = "1e308";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= द्विग_उच्च);
  }
  {
    स्थिरांक अक्षर str[] = "1e309";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    स्थिरांक अक्षर str[] = "1e-309";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < द्विग_न्यून);
    hb_always_assert (pv == म_से_भग्न (str, nullptr));
  }
  {
    स्थिरांक अक्षर str[] = "5e-324";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv > 0.0 && pv < द्विग_न्यून);
    hb_always_assert (pv == म_से_भग्न (str, nullptr));
  }
  {
    स्थिरांक अक्षर str[] = "1e+5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    स्थिरांक अक्षर str[] = "1E5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, सत्य));
  }
  {
    स्थिरांक अक्षर str[] = "-1.5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == -1.5);
  }
  {
    स्थिरांक अक्षर str[] = "1.5E-10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    स्थिरांक अक्षर str[] = "-.5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == -0.5);
  }
  {
    स्थिरांक अक्षर str[] = "500";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    स्थिरांक अक्षर str[] = "-7.5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    स्थिरांक अक्षर str[] = "123 ";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, सत्य));
  }
  {
    स्थिरांक अक्षर str[] = " 123";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 123.0);
  }
  {
    स्थिरांक अक्षर str[] = "+1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 1.0);
  }
  {
    स्थिरांक अक्षर str[] = "+1.5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 1.5);
  }
  {
    स्थिरांक अक्षर str[] = "--1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = "+-1";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = "0e5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 0.0);
  }
  {
    स्थिरांक अक्षर str[] = "1.5e";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    स्थिरांक अक्षर str[] = "1.5e+";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    स्थिरांक अक्षर str[] = "1e9999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == द्विग_उच्च);
  }
  {
    स्थिरांक अक्षर str[] = "-1e9999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -द्विग_उच्च);
  }
  {
    स्थिरांक अक्षर str[] = "1e-9999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == म_से_भग्न (str, nullptr));
  }
  {
    स्थिरांक अक्षर str[] = "-1e-9999999";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -म_से_भग्न (str, nullptr));
  }
  {
    स्थिरांक अक्षर str[] = "1e-400";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    स्थिरांक अक्षर str[] = "007";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 7.0);
  }
  {
    स्थिरांक अक्षर str[] = "0.0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    स्थिरांक अक्षर str[] = "NaN";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = "inf";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = "1..5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    स्थिरांक अक्षर str[] = "1.5.5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    स्थिरांक अक्षर str[] = "   ";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    स्थिरांक अक्षर str[] = " abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    स्थिरांक अक्षर str[] = "1.7976931348623157e308";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv >= द्विग_उच्च || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "99999999999999999999"; // 20 nines
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "1.e5";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    स्थिरांक अक्षर str[] = "1e005";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    स्थिरांक अक्षर str[] = "1e-005";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    स्थिरांक अक्षर str[] = "-0";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    स्थिरांक अक्षर str[] = "12345678901234567890e-10";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    स्थिरांक अक्षर str[] = "123abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, सत्य));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    स्थिरांक अक्षर str[] = "abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    स्थिरांक अक्षर str[] = "1e5abc";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    स्थिरांक अक्षर str[] = "1e+5x";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    स्थिरांक अक्षर str[] = "1ex";
    स्थिरांक अक्षर *pp = str;
    स्थिरांक अक्षर *end = str + ARRAY_LENGTH (str) - 1;
    द्विगुण pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  परत 0;
}
