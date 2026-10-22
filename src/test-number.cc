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

#ضمن <cmath>
#ضمن <cstdlib>

#ضمن "hb.hh"
#ضمن "hb-number.hh"

ساكن فراغ
test_parse_int ()
{
  {
    ثابت حرف str[] = "123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + 3;

    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 0);
    hb_always_assert (!*سرا);
  }

  {
    ثابت حرف str[] = "-123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + 4;

    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (سرا - pp == 0);
    hb_always_assert (!*سرا);
  }

  /* Leading whitespace */
  {
    ثابت حرف str[] = "   123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "\t123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "\n123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = " -123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == سرا);
  }

  /* Leading '+' sign */
  {
    ثابت حرف str[] = "+123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "+0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    ثابت حرف str[] = "007";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == سرا);
  }

  /* Negative zero */
  {
    ثابت حرف str[] = "-0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }

  /* Partial parse: stops at first non-digit */
  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    ثابت حرف str[] = "2147483647";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "-2147483648";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == سرا);
  }

  /* Failures: no digits */
  {
    ثابت حرف str[] = "   ";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ثابت حرف str[] = "+";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "-";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }

  /* Multiple signs fail */
  {
    ثابت حرف str[] = "++1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "--1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "+-1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "-+1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    ثابت حرف str[] = " 123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv, سچ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ثابت حرف str[] = "9999999999";   /* 10 digits, > INT_MAX */
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    ثابت حرف str[] = "2147483647";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "-2147483648";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == سرا);
  }

  /* One past INT_MAX - must fail */
  {
    ثابت حرف str[] = "2147483648";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    ثابت حرف str[] = "-2147483649";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    ثابت حرف str[] = "9999999999999999999999999999999999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    ثابت حرف str[] = "9999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 42;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ثابت حرف str[] = "- 123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    ثابت حرف str[] = "+0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    ثابت حرف str[] = "-   ";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    صحيح pv = 99;
    hb_always_assert (!hb_parse_int (&pp, سرا, &pv));
    hb_always_assert (pv == 99);
  }
}

ساكن فراغ
test_parse_uint ()
{
  {
    ثابت حرف str[] = "123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + لڑی_لمبائی (str);

    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 0);
    hb_always_assert (!*سرا);
  }

  {
    ثابت حرف str[] = "12F";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + 3;

    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 0);
    hb_always_assert (!*سرا);
  }

  {
    ثابت حرف str[] = "12Fq";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + 4;

    بنانشانی صحيح pv;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 1);
    hb_always_assert (!*سرا);
  }

  {
    ثابت حرف str[] = "123";
    ثابت حرف *pp = str;
    hb_always_assert (ARRAY_لمائی (str) == 4);
    ثابت حرف *سرا = str + ARRAY_لمائی (str);

    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 1);
  }

  {
    ثابت حرف str[] = "123\0";
    ثابت حرف *pp = str;
    hb_always_assert (ARRAY_لمائی (str) == 5);
    ثابت حرف *سرا = str + ARRAY_لمائی (str);

    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 2);
  }

  {
    ثابت حرف str[] = "123V";
    ثابت حرف *pp = str;
    hb_always_assert (ARRAY_لمائی (str) == 5);
    ثابت حرف *سرا = str + ARRAY_لمائی (str);

    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (سرا - pp == 2);
  }

  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv, سچ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    ثابت حرف str[] = " 123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "\t123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == سرا);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    ثابت حرف str[] = "007";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "000";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }

  /* UINT_MAX */
  {
    ثابت حرف str[] = "4294967295";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == سرا);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    ثابت حرف str[] = "-1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "-0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }

  /* Base 2 */
  {
    ثابت حرف str[] = "1010";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "11111111";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "12";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    ثابت حرف str[] = "777";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "010";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == سرا);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    ثابت حرف str[] = "FF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "ff";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "Ff";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "0xFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "0XFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }

  /* Base 36 */
  {
    ثابت حرف str[] = "Z";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "z";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == سرا);
  }

  /* Failures */
  {
    ثابت حرف str[] = "abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv, خطأ, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ثابت حرف str[] = "+";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "-";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
  }

  {
    ثابت حرف str[] = "0XFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "0xff";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "0x0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == سرا);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    ثابت حرف str[] = "0x";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    ثابت حرف str[] = "0xG";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    ثابت حرف str[] = "-0xFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == (بنانشانی) -255);
    hb_always_assert (pp == سرا);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    ثابت حرف str[] = "0xFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ثابت حرف str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    ثابت حرف str[] = "4294967295";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == سرا);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    ثابت حرف str[] = "4294967296";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    ثابت حرف str[] = "99999999999999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    ثابت حرف str[] = "9999999999999999999999999999999999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    ثابت حرف str[] = "0xFFFFFFFF";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == سرا);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    ثابت حرف str[] = "0x100000000";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    ثابت حرف str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv, سچ, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    ثابت حرف str[] = "9999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    ثابت حرف str[] = "-4294967296";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    ثابت حرف str[] = "-2147483648";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == سرا);
  }

  /* Mixed case in base 36 */
  {
    ثابت حرف str[] = "Az";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == سرا);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ثابت حرف str[] = "- 1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    ثابت حرف str[] = "0X10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, سچ, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == سرا);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    ثابت حرف str[] = "0x0x10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv, خطأ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    ثابت حرف str[] = "12\0F";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    بنانشانی صحيح pv;
    hb_always_assert (hb_parse_uint (&pp, سرا, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
ساكن مضمن منطقي
almost_equal (مزدوج a, مزدوج b)
{
  أرجع fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

ساكن فراغ
test_parse_double (فراغ)
{
  {
    ثابت حرف str[] = ".123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str);

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (سرا - pp == 1);
  }

  {
    ثابت حرف str[] = "0.123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (سرا - pp == 0);
  }

  {
    ثابت حرف str[] = "0.123e0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (سرا - pp == 0);
  }

  {
    ثابت حرف str[] = "123e-3";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (سرا - pp == 0);
  }

  {
    ثابت حرف str[] = ".000123e+3";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (سرا - pp == 0);
  }

  {
    ثابت حرف str[] = "-.000000123e6";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (سرا - pp == 0);

  }

  {
    ثابت حرف str[] = "-1.23E-1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;

    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert ((صحيح) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (سرا - pp == 0);
  }

  {
    ثابت حرف str[] = "1e512";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت حرف str[] = "1e600";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت حرف str[] = "1e-512";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت حرف str[] = "1e-600";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    ثابت حرف str[] = "0.9999999999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    ثابت حرف str[] = "0.499999999999999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    ثابت حرف str[] = "1e";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت حرف str[] = "1e+";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت حرف str[] = "-";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ثابت حرف str[] = ".";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ثابت حرف str[] = "1e-";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ثابت حرف str[] = "-1e";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    ثابت حرف str[] = "+";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
  }
  {
    ثابت حرف str[] = "-.";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
  }

  // But be careful with fractional parts
  {
    ثابت حرف str[] = "1.";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "1.abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    ثابت حرف str[] = "abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = "";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = " \t 123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "-0.0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    ثابت حرف str[] = "1.5E10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ثابت حرف str[] = "1.5E+10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ثابت حرف str[] = "1e308";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    ثابت حرف str[] = "1e309";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ثابت حرف str[] = "1e-309";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, خالی_سوچک));
  }
  {
    ثابت حرف str[] = "5e-324";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, خالی_سوچک));
  }
  {
    ثابت حرف str[] = "1e+5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ثابت حرف str[] = "1E5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv, سچ));
  }
  {
    ثابت حرف str[] = "-1.5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == -1.5);
  }
  {
    ثابت حرف str[] = "1.5E-10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    ثابت حرف str[] = "-.5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == -0.5);
  }
  {
    ثابت حرف str[] = "500";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    ثابت حرف str[] = "-7.5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    ثابت حرف str[] = "123 ";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv, سچ));
  }
  {
    ثابت حرف str[] = " 123";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 123.0);
  }
  {
    ثابت حرف str[] = "+1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 1.0);
  }
  {
    ثابت حرف str[] = "+1.5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 1.5);
  }
  {
    ثابت حرف str[] = "--1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = "+-1";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = "0e5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت حرف str[] = "1.5e";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت حرف str[] = "1.5e+";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ثابت حرف str[] = "1e9999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    ثابت حرف str[] = "-1e9999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    ثابت حرف str[] = "1e-9999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, خالی_سوچک));
  }
  {
    ثابت حرف str[] = "-1e-9999999";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, خالی_سوچک));
  }
  {
    ثابت حرف str[] = "1e-400";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ثابت حرف str[] = "007";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 7.0);
  }
  {
    ثابت حرف str[] = "0.0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    ثابت حرف str[] = "NaN";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = "inf";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = "1..5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    ثابت حرف str[] = "1.5.5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    ثابت حرف str[] = "   ";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ثابت حرف str[] = " abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    ثابت حرف str[] = "1.7976931348623157e308";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "99999999999999999999"; // 20 nines
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "1.e5";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == سرا);
  }

  /* Exponent with leading zeros */
  {
    ثابت حرف str[] = "1e005";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == سرا);
  }
  {
    ثابت حرف str[] = "1e-005";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == سرا);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    ثابت حرف str[] = "-0";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == سرا);
  }

  /* Long mantissa followed by an exponent */
  {
    ثابت حرف str[] = "12345678901234567890e-10";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == سرا);
  }

  /* whole_buffer failure must not touch *pv */
  {
    ثابت حرف str[] = "123abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv, سچ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    ثابت حرف str[] = "abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    ثابت حرف str[] = "1e5abc";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    ثابت حرف str[] = "1e+5x";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    ثابت حرف str[] = "1ex";
    ثابت حرف *pp = str;
    ثابت حرف *سرا = str + ARRAY_لمائی (str) - 1;
    مزدوج pv;
    hb_always_assert (hb_parse_double (&pp, سرا, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  أرجع 0;
}
