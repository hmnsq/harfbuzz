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

#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <cstdlib>

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-number.hh"

ಸ್ಥಿರ ಶೂನ್ಯ
test_parse_int ()
{
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + 3;

    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "-123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + 4;

    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "   123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "\t123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "\n123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = " -123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "007";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "2147483647";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-2147483648";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "   ";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "++1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "--1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+-1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-+1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    ಧ್ರುವ ವರ್ಣ str[] = " 123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999";   /* 10 digits, > INT_MAX */
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "2147483647";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-2147483648";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "2147483648";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-2147483649";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999999999999999999999999999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "- 123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-   ";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
test_parse_uint ()
{
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + strlen (str);

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "12F";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + 3;

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "12Fq";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + 4;

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str);

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "123\0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str);

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "123V";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str);

    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    ಧ್ರುವ ವರ್ಣ str[] = " 123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "\t123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "007";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "000";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "4294967295";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1010";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "11111111";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "12";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "777";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "010";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "FF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "ff";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "Ff";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0xFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0XFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "Z";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "z";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "0XFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0xff";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0x0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0x";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0xG";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-0xFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0xFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "4294967295";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "4294967296";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "99999999999999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999999999999999999999999999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0xFFFFFFFF";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0x100000000";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "9999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-4294967296";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-2147483648";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "Az";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "- 1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0X10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಸತ್ಯ, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0x0x10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ಅಸತ್ಯ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "12\0F";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
almost_equal (ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b)
{
  ಪ್ರತ್ಯಾಗಮ fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

ಸ್ಥಿರ ಶೂನ್ಯ
test_parse_double (ಶೂನ್ಯ)
{
  {
    ಧ್ರುವ ವರ್ಣ str[] = ".123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str);

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "0.123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "0.123e0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "123e-3";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = ".000123e+3";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "-.000000123e6";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1.23E-1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;

    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((ಪೂರ್ಣಾಙ್ಕ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e512";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e600";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-512";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-600";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "0.9999999999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0.499999999999999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e+";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = ".";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1e";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-.";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = " \t 123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-0.0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5E10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5E+10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e308";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e309";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-309";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "5e-324";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e+5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1E5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1.5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == -1.5);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5E-10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-.5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == -0.5);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "500";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-7.5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123 ";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = " 123";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 123.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 1.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+1.5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 1.5);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "--1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "+-1";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0e5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 0.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5e";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5e+";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e9999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1e9999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-9999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-1e-9999999";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-400";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "007";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 7.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "0.0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "NaN";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "inf";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1..5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.5.5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "   ";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = " abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.7976931348623157e308";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "99999999999999999999"; // 20 nines
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1.e5";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e005";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e-005";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "-0";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "12345678901234567890e-10";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "123abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, ಸತ್ಯ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e5abc";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1e+5x";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    ಧ್ರುವ ವರ್ಣ str[] = "1ex";
    ಧ್ರುವ ವರ್ಣ *pp = str;
    ಧ್ರುವ ವರ್ಣ *end = str + ARRAY_LENGTH (str) - 1;
    ದ್ವಿಗುಣ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  ಪ್ರತ್ಯಾಗಮ 0;
}
