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

#অন্তর্ভাৱ <cmath>
#অন্তর্ভাৱ <cstdlib>

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-number.hh"

স্থির শূন্য
test_parse_int ()
{
  {
    ধ্রুৱ ৱর্ণ str[] = "123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + 3;

    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "-123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + 4;

    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    ধ্রুৱ ৱর্ণ str[] = "   123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "\t123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "\n123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = " -123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    ধ্রুৱ ৱর্ণ str[] = "+123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    ধ্রুৱ ৱর্ণ str[] = "007";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    ধ্রুৱ ৱর্ণ str[] = "-0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    ধ্রুৱ ৱর্ণ str[] = "2147483647";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-2147483648";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    ধ্রুৱ ৱর্ণ str[] = "   ";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    ধ্রুৱ ৱর্ণ str[] = "++1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "--1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+-1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-+1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    ধ্রুৱ ৱর্ণ str[] = " 123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, সঁচা));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ধ্রুৱ ৱর্ণ str[] = "9999999999";   /* 10 digits, > INT_MAX */
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    ধ্রুৱ ৱর্ণ str[] = "2147483647";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-2147483648";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    ধ্রুৱ ৱর্ণ str[] = "2147483648";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    ধ্রুৱ ৱর্ণ str[] = "-2147483649";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    ধ্রুৱ ৱর্ণ str[] = "9999999999999999999999999999999999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    ধ্রুৱ ৱর্ণ str[] = "9999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ধ্রুৱ ৱর্ণ str[] = "- 123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    ধ্রুৱ ৱর্ণ str[] = "+0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    ধ্রুৱ ৱর্ণ str[] = "-   ";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

স্থির শূন্য
test_parse_uint ()
{
  {
    ধ্রুৱ ৱর্ণ str[] = "123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + strlen (str);

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "12F";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + 3;

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "12Fq";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + 4;

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "123";
    ধ্রুৱ ৱর্ণ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str);

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "123\0";
    ধ্রুৱ ৱর্ণ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str);

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "123V";
    ধ্রুৱ ৱর্ণ *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str);

    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, সঁচা));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    ধ্রুৱ ৱর্ণ str[] = " 123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "\t123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    ধ্রুৱ ৱর্ণ str[] = "007";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "000";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    ধ্রুৱ ৱর্ণ str[] = "4294967295";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    ধ্রুৱ ৱর্ণ str[] = "-1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    ধ্রুৱ ৱর্ণ str[] = "1010";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "11111111";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "12";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    ধ্রুৱ ৱর্ণ str[] = "777";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "010";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    ধ্রুৱ ৱর্ণ str[] = "FF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "ff";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "Ff";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0xFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0XFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    ধ্রুৱ ৱর্ণ str[] = "Z";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "z";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    ধ্রুৱ ৱর্ণ str[] = "abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, মিছা, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "0XFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0xff";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0x0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    ধ্রুৱ ৱর্ণ str[] = "0x";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    ধ্রুৱ ৱর্ণ str[] = "0xG";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    ধ্রুৱ ৱর্ণ str[] = "-0xFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    ধ্রুৱ ৱর্ণ str[] = "0xFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ধ্রুৱ ৱর্ণ str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    ধ্রুৱ ৱর্ণ str[] = "4294967295";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    ধ্রুৱ ৱর্ণ str[] = "4294967296";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    ধ্রুৱ ৱর্ণ str[] = "99999999999999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    ধ্রুৱ ৱর্ণ str[] = "9999999999999999999999999999999999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    ধ্রুৱ ৱর্ণ str[] = "0xFFFFFFFF";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    ধ্রুৱ ৱর্ণ str[] = "0x100000000";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    ধ্রুৱ ৱর্ণ str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, সঁচা, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "9999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    ধ্রুৱ ৱর্ণ str[] = "-4294967296";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    ধ্রুৱ ৱর্ণ str[] = "-2147483648";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    ধ্রুৱ ৱর্ণ str[] = "Az";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ধ্রুৱ ৱর্ণ str[] = "- 1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    ধ্রুৱ ৱর্ণ str[] = "0X10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, সঁচা, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    ধ্রুৱ ৱর্ণ str[] = "0x0x10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, মিছা, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    ধ্রুৱ ৱর্ণ str[] = "12\0F";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    unsigned পূর্ণাঙ্ক pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
স্থির অন্তঃস্থ তার্কিক
almost_equal (দ্ৱিগুণ a, দ্ৱিগুণ b)
{
  ঘূৰাই fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

স্থির শূন্য
test_parse_double (শূন্য)
{
  {
    ধ্রুৱ ৱর্ণ str[] = ".123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str);

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "0.123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "0.123e0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "123e-3";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = ".000123e+3";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "-.000000123e6";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    ধ্রুৱ ৱর্ণ str[] = "-1.23E-1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;

    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((পূর্ণাঙ্ক) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "1e512";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e600";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-512";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-600";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "0.9999999999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0.499999999999999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "1e";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e+";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = ".";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-1e";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-.";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    ধ্রুৱ ৱর্ণ str[] = "1.";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = " \t 123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-0.0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5E10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5E+10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e308";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e309";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-309";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "5e-324";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e+5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1E5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, সঁচা));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-1.5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == -1.5);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5E-10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-.5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == -0.5);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "500";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-7.5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "123 ";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, সঁচা));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = " 123";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 123.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 1.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+1.5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 1.5);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "--1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "+-1";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0e5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 0.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5e";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5e+";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e9999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-1e9999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-9999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "-1e-9999999";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-400";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "007";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 7.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "0.0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "NaN";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "inf";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1..5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.5.5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "   ";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = " abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    ধ্রুৱ ৱর্ণ str[] = "1.7976931348623157e308";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "99999999999999999999"; // 20 nines
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1.e5";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    ধ্রুৱ ৱর্ণ str[] = "1e005";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    ধ্রুৱ ৱর্ণ str[] = "1e-005";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    ধ্রুৱ ৱর্ণ str[] = "-0";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    ধ্রুৱ ৱর্ণ str[] = "12345678901234567890e-10";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    ধ্রুৱ ৱর্ণ str[] = "123abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, সঁচা));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    ধ্রুৱ ৱর্ণ str[] = "abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    ধ্রুৱ ৱর্ণ str[] = "1e5abc";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    ধ্রুৱ ৱর্ণ str[] = "1e+5x";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    ধ্রুৱ ৱর্ণ str[] = "1ex";
    ধ্রুৱ ৱর্ণ *pp = str;
    ধ্রুৱ ৱর্ণ *end = str + ARRAY_LENGTH (str) - 1;
    দ্ৱিগুণ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  ঘূৰাই 0;
}
