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

#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <cstdlib>

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-number.hh"

സ്ഥിതം ശൂന്യം
test_parse_int ()
{
  {
    സ്ഥിരം അക്ഷരം str[] = "123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + 3;

    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "-123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + 4;

    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  /* Leading whitespace */
  {
    സ്ഥിരം അക്ഷരം str[] = "   123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "\t123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "\n123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = " -123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == end);
  }

  /* Leading '+' sign */
  {
    സ്ഥിരം അക്ഷരം str[] = "+123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    സ്ഥിരം അക്ഷരം str[] = "007";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }

  /* Negative zero */
  {
    സ്ഥിരം അക്ഷരം str[] = "-0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Partial parse: stops at first non-digit */
  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    സ്ഥിരം അക്ഷരം str[] = "2147483647";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-2147483648";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == end);
  }

  /* Failures: no digits */
  {
    സ്ഥിരം അക്ഷരം str[] = "   ";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* Multiple signs fail */
  {
    സ്ഥിരം അക്ഷരം str[] = "++1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "--1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+-1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-+1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    സ്ഥിരം അക്ഷരം str[] = " 123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv, സത്യം));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    സ്ഥിരം അക്ഷരം str[] = "9999999999";   /* 10 digits, > INT_MAX */
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    സ്ഥിരം അക്ഷരം str[] = "2147483647";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-2147483648";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == end);
  }

  /* One past INT_MAX - must fail */
  {
    സ്ഥിരം അക്ഷരം str[] = "2147483648";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    സ്ഥിരം അക്ഷരം str[] = "-2147483649";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    സ്ഥിരം അക്ഷരം str[] = "9999999999999999999999999999999999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    സ്ഥിരം അക്ഷരം str[] = "9999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 42;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    സ്ഥിരം അക്ഷരം str[] = "- 123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    സ്ഥിരം അക്ഷരം str[] = "+0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    സ്ഥിരം അക്ഷരം str[] = "-   ";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
}

സ്ഥിതം ശൂന്യം
test_parse_uint ()
{
  {
    സ്ഥിരം അക്ഷരം str[] = "123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + strlen (str);

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "12F";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + 3;

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 0);
    hb_always_assert (!*end);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "12Fq";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + 4;

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
    hb_always_assert (!*end);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "123";
    സ്ഥിരം അക്ഷരം *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 4);
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str);

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 1);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "123\0";
    സ്ഥിരം അക്ഷരം *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str);

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "123V";
    സ്ഥിരം അക്ഷരം *pp = str;
    hb_always_assert (ARRAY_LENGTH (str) == 5);
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str);

    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (end - pp == 2);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, സത്യം));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    സ്ഥിരം അക്ഷരം str[] = " 123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "\t123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == end);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    സ്ഥിരം അക്ഷരം str[] = "007";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "000";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* UINT_MAX */
  {
    സ്ഥിരം അക്ഷരം str[] = "4294967295";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    സ്ഥിരം അക്ഷരം str[] = "-1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }

  /* Base 2 */
  {
    സ്ഥിരം അക്ഷരം str[] = "1010";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "11111111";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "12";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    സ്ഥിരം അക്ഷരം str[] = "777";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "010";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == end);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    സ്ഥിരം അക്ഷരം str[] = "FF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "ff";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "Ff";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0xFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0XFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }

  /* Base 36 */
  {
    സ്ഥിരം അക്ഷരം str[] = "Z";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "z";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == end);
  }

  /* Failures */
  {
    സ്ഥിരം അക്ഷരം str[] = "abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, അസത്യം, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "0XFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0xff";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0x0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == end);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    സ്ഥിരം അക്ഷരം str[] = "0x";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    സ്ഥിരം അക്ഷരം str[] = "0xG";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    സ്ഥിരം അക്ഷരം str[] = "-0xFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == (unsigned) -255);
    hb_always_assert (pp == end);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    സ്ഥിരം അക്ഷരം str[] = "0xFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    സ്ഥിരം അക്ഷരം str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    സ്ഥിരം അക്ഷരം str[] = "4294967295";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    സ്ഥിരം അക്ഷരം str[] = "4294967296";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    സ്ഥിരം അക്ഷരം str[] = "99999999999999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    സ്ഥിരം അക്ഷരം str[] = "9999999999999999999999999999999999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    സ്ഥിരം അക്ഷരം str[] = "0xFFFFFFFF";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == end);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    സ്ഥിരം അക്ഷരം str[] = "0x100000000";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    സ്ഥിരം അക്ഷരം str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv, സത്യം, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "9999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    സ്ഥിരം അക്ഷരം str[] = "-4294967296";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    സ്ഥിരം അക്ഷരം str[] = "-2147483648";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == end);
  }

  /* Mixed case in base 36 */
  {
    സ്ഥിരം അക്ഷരം str[] = "Az";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == end);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    സ്ഥിരം അക്ഷരം str[] = "- 1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    സ്ഥിരം അക്ഷരം str[] = "0X10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, സത്യം, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == end);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    സ്ഥിരം അക്ഷരം str[] = "0x0x10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv, അസത്യം, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    സ്ഥിരം അക്ഷരം str[] = "12\0F";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    unsigned പൂർണ്ണസംഖ്യ pv;
    hb_always_assert (hb_parse_uint (&pp, end, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
സ്ഥിതം വരിയിൽ തർക്കം
almost_equal (ഇരട്ട a, ഇരട്ട b)
{
  തിരികെ fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

സ്ഥിതം ശൂന്യം
test_parse_double (ശൂന്യം)
{
  {
    സ്ഥിരം അക്ഷരം str[] = ".123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str);

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (end - pp == 1);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "0.123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (end - pp == 0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "0.123e0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (end - pp == 0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "123e-3";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (end - pp == 0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = ".000123e+3";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (end - pp == 0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "-.000000123e6";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (end - pp == 0);

  }

  {
    സ്ഥിരം അക്ഷരം str[] = "-1.23E-1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;

    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert ((പൂർണ്ണസംഖ്യ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (end - pp == 0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "1e512";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e600";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-512";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-600";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "0.9999999999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0.499999999999999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "1e";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e+";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = ".";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-1e";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-.";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
  }

  // But be careful with fractional parts
  {
    സ്ഥിരം അക്ഷരം str[] = "1.";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = " \t 123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-0.0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5E10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 1.5e10);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5E+10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 1.5e10);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e308";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e309";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-309";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "5e-324";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e+5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1E5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, സത്യം));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-1.5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == -1.5);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5E-10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-.5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == -0.5);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "500";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-7.5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "123 ";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, സത്യം));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = " 123";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 123.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 1.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+1.5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 1.5);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "--1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "+-1";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0e5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 0.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5e";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5e+";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e9999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-1e9999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-9999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, nullptr));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "-1e-9999999";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, nullptr));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-400";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "007";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 7.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "0.0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "NaN";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "inf";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1..5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.5.5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "   ";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = " abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    സ്ഥിരം അക്ഷരം str[] = "1.7976931348623157e308";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "99999999999999999999"; // 20 nines
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1.e5";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }

  /* Exponent with leading zeros */
  {
    സ്ഥിരം അക്ഷരം str[] = "1e005";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == end);
  }
  {
    സ്ഥിരം അക്ഷരം str[] = "1e-005";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == end);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    സ്ഥിരം അക്ഷരം str[] = "-0";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == end);
  }

  /* Long mantissa followed by an exponent */
  {
    സ്ഥിരം അക്ഷരം str[] = "12345678901234567890e-10";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == end);
  }

  /* whole_buffer failure must not touch *pv */
  {
    സ്ഥിരം അക്ഷരം str[] = "123abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv, സത്യം));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    സ്ഥിരം അക്ഷരം str[] = "abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, end, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    സ്ഥിരം അക്ഷരം str[] = "1e5abc";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    സ്ഥിരം അക്ഷരം str[] = "1e+5x";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    സ്ഥിരം അക്ഷരം str[] = "1ex";
    സ്ഥിരം അക്ഷരം *pp = str;
    സ്ഥിരം അക്ഷരം *end = str + ARRAY_LENGTH (str) - 1;
    ഇരട്ട pv;
    hb_always_assert (hb_parse_double (&pp, end, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  തിരികെ 0;
}
