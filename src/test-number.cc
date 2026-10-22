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

#અન્તર્ભાવ <cmath>
#અન્તર્ભાવ <cstdlib>

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-number.hh"

સ્થિર શૂન્ય
test_parse_int ()
{
  {
    અચલ અક્ષર str[] = "123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + 3;

    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    અચલ અક્ષર str[] = "-123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + 4;

    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    અચલ અક્ષર str[] = "   123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "\t123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "\n123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = " -123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    અચલ અક્ષર str[] = "+123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "+0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    અચલ અક્ષર str[] = "007";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    અચલ અક્ષર str[] = "-0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    અચલ અક્ષર str[] = "2147483647";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "-2147483648";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    અચલ અક્ષર str[] = "   ";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    અચલ અક્ષર str[] = "+";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "-";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    અચલ અક્ષર str[] = "++1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "--1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "+-1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "-+1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    અચલ અક્ષર str[] = " 123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, સાચું));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    અચલ અક્ષર str[] = "9999999999";   /* 10 digits, > INT_MAX */
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    અચલ અક્ષર str[] = "2147483647";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "-2147483648";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    અચલ અક્ષર str[] = "2147483648";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    અચલ અક્ષર str[] = "-2147483649";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    અચલ અક્ષર str[] = "9999999999999999999999999999999999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    અચલ અક્ષર str[] = "9999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    અચલ અક્ષર str[] = "- 123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    અચલ અક્ષર str[] = "+0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    અચલ અક્ષર str[] = "-   ";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

સ્થિર શૂન્ય
test_parse_uint ()
{
  {
    અચલ અક્ષર str[] = "123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + strlen (str);

    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    અચલ અક્ષર str[] = "12F";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + 3;

    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    અચલ અક્ષર str[] = "12Fq";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + 4;

    unsigned પૂર્ણાંક pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    અચલ અક્ષર str[] = "123";
    અચલ અક્ષર *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str);

    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    અચલ અક્ષર str[] = "123\0";
    અચલ અક્ષર *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str);

    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    અચલ અક્ષર str[] = "123V";
    અચલ અક્ષર *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str);

    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, સાચું));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    અચલ અક્ષર str[] = " 123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "\t123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    અચલ અક્ષર str[] = "007";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "000";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    અચલ અક્ષર str[] = "4294967295";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    અચલ અક્ષર str[] = "-1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "-0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    અચલ અક્ષર str[] = "1010";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "11111111";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "12";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    અચલ અક્ષર str[] = "777";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "010";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    અચલ અક્ષર str[] = "FF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "ff";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "Ff";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "0xFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "0XFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    અચલ અક્ષર str[] = "Z";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "z";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    અચલ અક્ષર str[] = "abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, ખોટું, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    અચલ અક્ષર str[] = "+";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "-";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    અચલ અક્ષર str[] = "0XFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "0xff";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "0x0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    અચલ અક્ષર str[] = "0x";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    અચલ અક્ષર str[] = "0xG";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    અચલ અક્ષર str[] = "-0xFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    અચલ અક્ષર str[] = "0xFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    અચલ અક્ષર str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    અચલ અક્ષર str[] = "4294967295";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    અચલ અક્ષર str[] = "4294967296";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    અચલ અક્ષર str[] = "99999999999999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    અચલ અક્ષર str[] = "9999999999999999999999999999999999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    અચલ અક્ષર str[] = "0xFFFFFFFF";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    અચલ અક્ષર str[] = "0x100000000";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    અચલ અક્ષર str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, સાચું, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    અચલ અક્ષર str[] = "9999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    અચલ અક્ષર str[] = "-4294967296";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    અચલ અક્ષર str[] = "-2147483648";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    અચલ અક્ષર str[] = "Az";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    અચલ અક્ષર str[] = "- 1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    અચલ અક્ષર str[] = "0X10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, સાચું, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    અચલ અક્ષર str[] = "0x0x10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, ખોટું, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    અચલ અક્ષર str[] = "12\0F";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    unsigned પૂર્ણાંક pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
સ્થિર અંતઃરેખ તાર્કિક
almost_equal (દ્વિગુણ a, દ્વિગુણ b)
{
  પાછા fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

સ્થિર શૂન્ય
test_parse_double (શૂન્ય)
{
  {
    અચલ અક્ષર str[] = ".123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str);

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    અચલ અક્ષર str[] = "0.123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    અચલ અક્ષર str[] = "0.123e0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    અચલ અક્ષર str[] = "123e-3";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    અચલ અક્ષર str[] = ".000123e+3";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    અચલ અક્ષર str[] = "-.000000123e6";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    અચલ અક્ષર str[] = "-1.23E-1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;

    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((પૂર્ણાંક) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    અચલ અક્ષર str[] = "1e512";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    અચલ અક્ષર str[] = "1e600";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    અચલ અક્ષર str[] = "1e-512";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    અચલ અક્ષર str[] = "1e-600";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    અચલ અક્ષર str[] = "0.9999999999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    અચલ અક્ષર str[] = "0.499999999999999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    અચલ અક્ષર str[] = "1e";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    અચલ અક્ષર str[] = "1e+";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    અચલ અક્ષર str[] = "-";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    અચલ અક્ષર str[] = ".";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    અચલ અક્ષર str[] = "1e-";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    અચલ અક્ષર str[] = "-1e";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    અચલ અક્ષર str[] = "+";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    અચલ અક્ષર str[] = "-.";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    અચલ અક્ષર str[] = "1.";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "1.abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    અચલ અક્ષર str[] = "abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = "";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = " \t 123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "-0.0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    અચલ અક્ષર str[] = "1.5E10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 1.5e10);
  }
  {
    અચલ અક્ષર str[] = "1.5E+10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 1.5e10);
  }
  {
    અચલ અક્ષર str[] = "1e308";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    અચલ અક્ષર str[] = "1e309";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    અચલ અક્ષર str[] = "1e-309";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    અચલ અક્ષર str[] = "5e-324";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    અચલ અક્ષર str[] = "1e+5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    અચલ અક્ષર str[] = "1E5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, સાચું));
  }
  {
    અચલ અક્ષર str[] = "-1.5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == -1.5);
  }
  {
    અચલ અક્ષર str[] = "1.5E-10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    અચલ અક્ષર str[] = "-.5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == -0.5);
  }
  {
    અચલ અક્ષર str[] = "500";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    અચલ અક્ષર str[] = "-7.5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    અચલ અક્ષર str[] = "123 ";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, સાચું));
  }
  {
    અચલ અક્ષર str[] = " 123";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 123.0);
  }
  {
    અચલ અક્ષર str[] = "+1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 1.0);
  }
  {
    અચલ અક્ષર str[] = "+1.5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 1.5);
  }
  {
    અચલ અક્ષર str[] = "--1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = "+-1";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = "0e5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 0.0);
  }
  {
    અચલ અક્ષર str[] = "1.5e";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    અચલ અક્ષર str[] = "1.5e+";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    અચલ અક્ષર str[] = "1e9999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    અચલ અક્ષર str[] = "-1e9999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    અચલ અક્ષર str[] = "1e-9999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    અચલ અક્ષર str[] = "-1e-9999999";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    અચલ અક્ષર str[] = "1e-400";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    અચલ અક્ષર str[] = "007";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 7.0);
  }
  {
    અચલ અક્ષર str[] = "0.0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    અચલ અક્ષર str[] = "NaN";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = "inf";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = "1..5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    અચલ અક્ષર str[] = "1.5.5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    અચલ અક્ષર str[] = "   ";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    અચલ અક્ષર str[] = " abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    અચલ અક્ષર str[] = "1.7976931348623157e308";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "99999999999999999999"; // 20 nines
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "1.e5";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    અચલ અક્ષર str[] = "1e005";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    અચલ અક્ષર str[] = "1e-005";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    અચલ અક્ષર str[] = "-0";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    અચલ અક્ષર str[] = "12345678901234567890e-10";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    અચલ અક્ષર str[] = "123abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, સાચું));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    અચલ અક્ષર str[] = "abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    અચલ અક્ષર str[] = "1e5abc";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    અચલ અક્ષર str[] = "1e+5x";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    અચલ અક્ષર str[] = "1ex";
    અચલ અક્ષર *pp = str;
    અચલ અક્ષર *end = str + ARRAY_LENGTH (str) - 1;
    દ્વિગુણ pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  પાછા 0;
}
