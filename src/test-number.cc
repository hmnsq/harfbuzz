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

#அந்தர்பாவ <cmath>
#அந்தர்பாவ <cstdlib>

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-number.hh"

நிலையான வெறுமை
test_parse_int ()
{
  {
    மாறிலி எழுத்து str[] = "123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + 3;

    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    மாறிலி எழுத்து str[] = "-123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + 4;

    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    மாறிலி எழுத்து str[] = "   123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "\t123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "\n123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = " -123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    மாறிலி எழுத்து str[] = "+123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "+0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    மாறிலி எழுத்து str[] = "007";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    மாறிலி எழுத்து str[] = "-0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    மாறிலி எழுத்து str[] = "2147483647";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "-2147483648";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    மாறிலி எழுத்து str[] = "   ";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    மாறிலி எழுத்து str[] = "+";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "-";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    மாறிலி எழுத்து str[] = "++1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "--1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "+-1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "-+1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    மாறிலி எழுத்து str[] = " 123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, உண்மை));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    மாறிலி எழுத்து str[] = "9999999999";   /* 10 digits, > INT_MAX */
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    மாறிலி எழுத்து str[] = "2147483647";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "-2147483648";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    மாறிலி எழுத்து str[] = "2147483648";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    மாறிலி எழுத்து str[] = "-2147483649";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    மாறிலி எழுத்து str[] = "9999999999999999999999999999999999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    மாறிலி எழுத்து str[] = "9999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    மாறிலி எழுத்து str[] = "- 123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    மாறிலி எழுத்து str[] = "+0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    மாறிலி எழுத்து str[] = "-   ";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    முழு pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

நிலையான வெறுமை
test_parse_uint ()
{
  {
    மாறிலி எழுத்து str[] = "123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + strlen (str);

    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    மாறிலி எழுத்து str[] = "12F";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + 3;

    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    மாறிலி எழுத்து str[] = "12Fq";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + 4;

    unsigned முழு pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    மாறிலி எழுத்து str[] = "123";
    மாறிலி எழுத்து *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str);

    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    மாறிலி எழுத்து str[] = "123\0";
    மாறிலி எழுத்து *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str);

    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    மாறிலி எழுத்து str[] = "123V";
    மாறிலி எழுத்து *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str);

    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, உண்மை));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    மாறிலி எழுத்து str[] = " 123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "\t123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    மாறிலி எழுத்து str[] = "007";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "000";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    மாறிலி எழுத்து str[] = "4294967295";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    மாறிலி எழுத்து str[] = "-1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "-0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    மாறிலி எழுத்து str[] = "1010";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "11111111";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "12";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    மாறிலி எழுத்து str[] = "777";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "010";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    மாறிலி எழுத்து str[] = "FF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "ff";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "Ff";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "0xFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "0XFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    மாறிலி எழுத்து str[] = "Z";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "z";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    மாறிலி எழுத்து str[] = "abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, பொய், 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    மாறிலி எழுத்து str[] = "+";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "-";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    மாறிலி எழுத்து str[] = "0XFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "0xff";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "0x0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    மாறிலி எழுத்து str[] = "0x";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    மாறிலி எழுத்து str[] = "0xG";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    மாறிலி எழுத்து str[] = "-0xFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    மாறிலி எழுத்து str[] = "0xFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    மாறிலி எழுத்து str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    மாறிலி எழுத்து str[] = "4294967295";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    மாறிலி எழுத்து str[] = "4294967296";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    மாறிலி எழுத்து str[] = "99999999999999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    மாறிலி எழுத்து str[] = "9999999999999999999999999999999999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    மாறிலி எழுத்து str[] = "0xFFFFFFFF";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    மாறிலி எழுத்து str[] = "0x100000000";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    மாறிலி எழுத்து str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, உண்மை, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    மாறிலி எழுத்து str[] = "9999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    மாறிலி எழுத்து str[] = "-4294967296";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    மாறிலி எழுத்து str[] = "-2147483648";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    மாறிலி எழுத்து str[] = "Az";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    மாறிலி எழுத்து str[] = "- 1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    மாறிலி எழுத்து str[] = "0X10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, உண்மை, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    மாறிலி எழுத்து str[] = "0x0x10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, பொய், 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    மாறிலி எழுத்து str[] = "12\0F";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    unsigned முழு pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
நிலையான உள்வரி தருக்க
almost_equal (இரட்டை a, இரட்டை b)
{
  திரும்பு fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

நிலையான வெறுமை
test_parse_double (வெறுமை)
{
  {
    மாறிலி எழுத்து str[] = ".123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str);

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    மாறிலி எழுத்து str[] = "0.123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    மாறிலி எழுத்து str[] = "0.123e0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    மாறிலி எழுத்து str[] = "123e-3";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    மாறிலி எழுத்து str[] = ".000123e+3";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    மாறிலி எழுத்து str[] = "-.000000123e6";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    மாறிலி எழுத்து str[] = "-1.23E-1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;

    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((முழு) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    மாறிலி எழுத்து str[] = "1e512";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    மாறிலி எழுத்து str[] = "1e600";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    மாறிலி எழுத்து str[] = "1e-512";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    மாறிலி எழுத்து str[] = "1e-600";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    மாறிலி எழுத்து str[] = "0.9999999999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    மாறிலி எழுத்து str[] = "0.499999999999999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    மாறிலி எழுத்து str[] = "1e";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    மாறிலி எழுத்து str[] = "1e+";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    மாறிலி எழுத்து str[] = "-";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    மாறிலி எழுத்து str[] = ".";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    மாறிலி எழுத்து str[] = "1e-";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    மாறிலி எழுத்து str[] = "-1e";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    மாறிலி எழுத்து str[] = "+";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    மாறிலி எழுத்து str[] = "-.";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    மாறிலி எழுத்து str[] = "1.";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "1.abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    மாறிலி எழுத்து str[] = "abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = "";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = " \t 123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "-0.0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    மாறிலி எழுத்து str[] = "1.5E10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 1.5e10);
  }
  {
    மாறிலி எழுத்து str[] = "1.5E+10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 1.5e10);
  }
  {
    மாறிலி எழுத்து str[] = "1e308";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    மாறிலி எழுத்து str[] = "1e309";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    மாறிலி எழுத்து str[] = "1e-309";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    மாறிலி எழுத்து str[] = "5e-324";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    மாறிலி எழுத்து str[] = "1e+5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    மாறிலி எழுத்து str[] = "1E5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, உண்மை));
  }
  {
    மாறிலி எழுத்து str[] = "-1.5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == -1.5);
  }
  {
    மாறிலி எழுத்து str[] = "1.5E-10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    மாறிலி எழுத்து str[] = "-.5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == -0.5);
  }
  {
    மாறிலி எழுத்து str[] = "500";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    மாறிலி எழுத்து str[] = "-7.5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    மாறிலி எழுத்து str[] = "123 ";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, உண்மை));
  }
  {
    மாறிலி எழுத்து str[] = " 123";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 123.0);
  }
  {
    மாறிலி எழுத்து str[] = "+1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 1.0);
  }
  {
    மாறிலி எழுத்து str[] = "+1.5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 1.5);
  }
  {
    மாறிலி எழுத்து str[] = "--1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = "+-1";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = "0e5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 0.0);
  }
  {
    மாறிலி எழுத்து str[] = "1.5e";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    மாறிலி எழுத்து str[] = "1.5e+";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    மாறிலி எழுத்து str[] = "1e9999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    மாறிலி எழுத்து str[] = "-1e9999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    மாறிலி எழுத்து str[] = "1e-9999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    மாறிலி எழுத்து str[] = "-1e-9999999";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    மாறிலி எழுத்து str[] = "1e-400";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    மாறிலி எழுத்து str[] = "007";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 7.0);
  }
  {
    மாறிலி எழுத்து str[] = "0.0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    மாறிலி எழுத்து str[] = "NaN";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = "inf";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = "1..5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    மாறிலி எழுத்து str[] = "1.5.5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    மாறிலி எழுத்து str[] = "   ";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    மாறிலி எழுத்து str[] = " abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    மாறிலி எழுத்து str[] = "1.7976931348623157e308";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "99999999999999999999"; // 20 nines
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "1.e5";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    மாறிலி எழுத்து str[] = "1e005";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    மாறிலி எழுத்து str[] = "1e-005";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    மாறிலி எழுத்து str[] = "-0";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    மாறிலி எழுத்து str[] = "12345678901234567890e-10";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    மாறிலி எழுத்து str[] = "123abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, உண்மை));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    மாறிலி எழுத்து str[] = "abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    மாறிலி எழுத்து str[] = "1e5abc";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    மாறிலி எழுத்து str[] = "1e+5x";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    மாறிலி எழுத்து str[] = "1ex";
    மாறிலி எழுத்து *pp = str;
    மாறிலி எழுத்து *end = str + ARRAY_LENGTH (str) - 1;
    இரட்டை pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  திரும்பு 0;
}
