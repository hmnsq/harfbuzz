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

#شامل <cmath>
#شامل <cstdlib>

#شامل "hb.hh"
#شامل "hb-number.hh"

ایستا تهی
test_parse_int ()
{
  {
    ثابت نویسه str[] = "123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + 3;

    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ثابت نویسه str[] = "-123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + 4;

    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    ثابت نویسه str[] = "   123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "\t123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "\n123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = " -123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    ثابت نویسه str[] = "+123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "+0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    ثابت نویسه str[] = "007";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    ثابت نویسه str[] = "-0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    ثابت نویسه str[] = "2147483647";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "-2147483648";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    ثابت نویسه str[] = "   ";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ثابت نویسه str[] = "+";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "-";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    ثابت نویسه str[] = "++1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "--1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "+-1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "-+1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    ثابت نویسه str[] = " 123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, درست));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, درست));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ثابت نویسه str[] = "9999999999";   /* 10 digits, > INT_MAX */
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    ثابت نویسه str[] = "2147483647";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "-2147483648";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    ثابت نویسه str[] = "2147483648";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    ثابت نویسه str[] = "-2147483649";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    ثابت نویسه str[] = "9999999999999999999999999999999999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    ثابت نویسه str[] = "9999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ثابت نویسه str[] = "- 123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    ثابت نویسه str[] = "+0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    ثابت نویسه str[] = "-   ";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    صحیح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

ایستا تهی
test_parse_uint ()
{
  {
    ثابت نویسه str[] = "123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + strlen (str);

    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ثابت نویسه str[] = "12F";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + 3;

    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ثابت نویسه str[] = "12Fq";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + 4;

    unsigned صحیح pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    ثابت نویسه str[] = "123";
    ثابت نویسه *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    ثابت نویسه *end = str + ARRAY_LENGTH (str);

    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    ثابت نویسه str[] = "123\0";
    ثابت نویسه *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ثابت نویسه *end = str + ARRAY_LENGTH (str);

    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ثابت نویسه str[] = "123V";
    ثابت نویسه *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ثابت نویسه *end = str + ARRAY_LENGTH (str);

    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, درست));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    ثابت نویسه str[] = " 123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "\t123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    ثابت نویسه str[] = "007";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "000";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    ثابت نویسه str[] = "4294967295";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    ثابت نویسه str[] = "-1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "-0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    ثابت نویسه str[] = "1010";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "11111111";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "12";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    ثابت نویسه str[] = "777";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "010";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    ثابت نویسه str[] = "FF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "ff";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "Ff";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "0xFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "0XFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    ثابت نویسه str[] = "Z";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "z";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    ثابت نویسه str[] = "abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, نادرست, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ثابت نویسه str[] = "+";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "-";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    ثابت نویسه str[] = "0XFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "0xff";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "0x0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    ثابت نویسه str[] = "0x";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    ثابت نویسه str[] = "0xG";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    ثابت نویسه str[] = "-0xFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    ثابت نویسه str[] = "0xFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ثابت نویسه str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    ثابت نویسه str[] = "4294967295";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    ثابت نویسه str[] = "4294967296";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    ثابت نویسه str[] = "99999999999999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    ثابت نویسه str[] = "9999999999999999999999999999999999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    ثابت نویسه str[] = "0xFFFFFFFF";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    ثابت نویسه str[] = "0x100000000";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    ثابت نویسه str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, درست, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    ثابت نویسه str[] = "9999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    ثابت نویسه str[] = "-4294967296";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    ثابت نویسه str[] = "-2147483648";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    ثابت نویسه str[] = "Az";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ثابت نویسه str[] = "- 1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    ثابت نویسه str[] = "0X10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, درست, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    ثابت نویسه str[] = "0x0x10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, نادرست, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    ثابت نویسه str[] = "12\0F";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    unsigned صحیح pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
ایستا درونخطی منطقی
almost_equal (دوگانه a, دوگانه b)
{
  بازگشت fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

ایستا تهی
test_parse_double (تهی)
{
  {
    ثابت نویسه str[] = ".123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str);

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    ثابت نویسه str[] = "0.123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    ثابت نویسه str[] = "0.123e0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    ثابت نویسه str[] = "123e-3";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    ثابت نویسه str[] = ".000123e+3";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    ثابت نویسه str[] = "-.000000123e6";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    ثابت نویسه str[] = "-1.23E-1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;

    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((صحیح) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    ثابت نویسه str[] = "1e512";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت نویسه str[] = "1e600";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت نویسه str[] = "1e-512";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت نویسه str[] = "1e-600";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    ثابت نویسه str[] = "0.9999999999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    ثابت نویسه str[] = "0.499999999999999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    ثابت نویسه str[] = "1e";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت نویسه str[] = "1e+";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت نویسه str[] = "-";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ثابت نویسه str[] = ".";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ثابت نویسه str[] = "1e-";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت نویسه str[] = "-1e";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    ثابت نویسه str[] = "+";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    ثابت نویسه str[] = "-.";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    ثابت نویسه str[] = "1.";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "1.abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    ثابت نویسه str[] = "abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = "";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = " \t 123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "-0.0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    ثابت نویسه str[] = "1.5E10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ثابت نویسه str[] = "1.5E+10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ثابت نویسه str[] = "1e308";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    ثابت نویسه str[] = "1e309";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت نویسه str[] = "1e-309";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ثابت نویسه str[] = "5e-324";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ثابت نویسه str[] = "1e+5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ثابت نویسه str[] = "1E5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, درست));
  }
  {
    ثابت نویسه str[] = "-1.5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == -1.5);
  }
  {
    ثابت نویسه str[] = "1.5E-10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    ثابت نویسه str[] = "-.5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == -0.5);
  }
  {
    ثابت نویسه str[] = "500";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    ثابت نویسه str[] = "-7.5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    ثابت نویسه str[] = "123 ";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, درست));
  }
  {
    ثابت نویسه str[] = " 123";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 123.0);
  }
  {
    ثابت نویسه str[] = "+1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 1.0);
  }
  {
    ثابت نویسه str[] = "+1.5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 1.5);
  }
  {
    ثابت نویسه str[] = "--1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = "+-1";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = "0e5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت نویسه str[] = "1.5e";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت نویسه str[] = "1.5e+";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت نویسه str[] = "1e9999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    ثابت نویسه str[] = "-1e9999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    ثابت نویسه str[] = "1e-9999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ثابت نویسه str[] = "-1e-9999999";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    ثابت نویسه str[] = "1e-400";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت نویسه str[] = "007";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 7.0);
  }
  {
    ثابت نویسه str[] = "0.0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    ثابت نویسه str[] = "NaN";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = "inf";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = "1..5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    ثابت نویسه str[] = "1.5.5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    ثابت نویسه str[] = "   ";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت نویسه str[] = " abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    ثابت نویسه str[] = "1.7976931348623157e308";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "99999999999999999999"; // 20 nines
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "1.e5";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    ثابت نویسه str[] = "1e005";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    ثابت نویسه str[] = "1e-005";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    ثابت نویسه str[] = "-0";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    ثابت نویسه str[] = "12345678901234567890e-10";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    ثابت نویسه str[] = "123abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, درست));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    ثابت نویسه str[] = "abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    ثابت نویسه str[] = "1e5abc";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    ثابت نویسه str[] = "1e+5x";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    ثابت نویسه str[] = "1ex";
    ثابت نویسه *pp = str;
    ثابت نویسه *end = str + ARRAY_LENGTH (str) - 1;
    دوگانه pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  بازگشت 0;
}
