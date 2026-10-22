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

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdlib>

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-number.hh"

ਸਥਿਰ ਖਾਲੀ
test_parse_int ()
{
  {
    ਅਚਲ ਅੱਖਰ str[] = "123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + 3;

    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 0);
    hb_always_assert (!*ਸਿਰਾ);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "-123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + 4;

    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (ਸਿਰਾ - pp == 0);
    hb_always_assert (!*ਸਿਰਾ);
  }

  /* Leading whitespace */
  {
    ਅਚਲ ਅੱਖਰ str[] = "   123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "\t123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "\n123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = " -123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Leading '+' sign */
  {
    ਅਚਲ ਅੱਖਰ str[] = "+123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "007";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Negative zero */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Partial parse: stops at first non-digit */
  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "2147483647";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-2147483648";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Failures: no digits */
  {
    ਅਚਲ ਅੱਖਰ str[] = "   ";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }

  /* Multiple signs fail */
  {
    ਅਚਲ ਅੱਖਰ str[] = "++1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "--1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+-1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-+1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    ਅਚਲ ਅੱਖਰ str[] = " 123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ਅਚਲ ਅੱਖਰ str[] = "9999999999";   /* 10 digits, > INT_MAX */
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    ਅਚਲ ਅੱਖਰ str[] = "2147483647";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-2147483648";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* One past INT_MAX - must fail */
  {
    ਅਚਲ ਅੱਖਰ str[] = "2147483648";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-2147483649";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    ਅਚਲ ਅੱਖਰ str[] = "9999999999999999999999999999999999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    ਅਚਲ ਅੱਖਰ str[] = "9999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 42;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ਅਚਲ ਅੱਖਰ str[] = "- 123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    ਅਚਲ ਅੱਖਰ str[] = "+0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-   ";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_int (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99);
  }
}

ਸਥਿਰ ਖਾਲੀ
test_parse_uint ()
{
  {
    ਅਚਲ ਅੱਖਰ str[] = "123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ਲੜੀ_ਲੰਬਾਈ (str);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 0);
    hb_always_assert (!*ਸਿਰਾ);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "12F";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + 3;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 0);
    hb_always_assert (!*ਸਿਰਾ);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "12Fq";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + 4;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 1);
    hb_always_assert (!*ਸਿਰਾ);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    hb_always_assert (ARRAY_ਲੰਮਾਈ (str) == 4);
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 1);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "123\0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    hb_always_assert (ARRAY_ਲੰਮਾਈ (str) == 5);
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 2);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "123V";
    ਅਚਲ ਅੱਖਰ *pp = str;
    hb_always_assert (ARRAY_ਲੰਮਾਈ (str) == 5);
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (ਸਿਰਾ - pp == 2);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    ਅਚਲ ਅੱਖਰ str[] = " 123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "\t123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "007";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "000";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* UINT_MAX */
  {
    ਅਚਲ ਅੱਖਰ str[] = "4294967295";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Base 2 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "1010";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "11111111";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "12";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "777";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "010";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    ਅਚਲ ਅੱਖਰ str[] = "FF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "ff";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "Ff";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0xFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0XFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Base 36 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "Z";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "z";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Failures */
  {
    ਅਚਲ ਅੱਖਰ str[] = "abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "0XFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0xff";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0x0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0x";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0xG";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-0xFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == (ਬਿਨਾਨਿਸ਼ਾਨੀ) -255);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0xFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    ਅਚਲ ਅੱਖਰ str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    ਅਚਲ ਅੱਖਰ str[] = "4294967295";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "4294967296";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    ਅਚਲ ਅੱਖਰ str[] = "99999999999999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    ਅਚਲ ਅੱਖਰ str[] = "9999999999999999999999999999999999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0xFFFFFFFF";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0x100000000";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    ਅਚਲ ਅੱਖਰ str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "9999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-4294967296";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-2147483648";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Mixed case in base 36 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "Az";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    ਅਚਲ ਅੱਖਰ str[] = "- 1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0X10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਸੱਚ, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    ਅਚਲ ਅੱਖਰ str[] = "0x0x10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv, ਝੂਠ, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    ਅਚਲ ਅੱਖਰ str[] = "12\0F";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pv;
    hb_always_assert (hb_parse_uint (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
almost_equal (ਦੁੱਗਣਾ a, ਦੁੱਗਣਾ b)
{
  ਵਾਪਸ fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

ਸਥਿਰ ਖਾਲੀ
test_parse_double (ਖਾਲੀ)
{
  {
    ਅਚਲ ਅੱਖਰ str[] = ".123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str);

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (ਸਿਰਾ - pp == 1);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "0.123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (ਸਿਰਾ - pp == 0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "0.123e0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (ਸਿਰਾ - pp == 0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "123e-3";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (ਸਿਰਾ - pp == 0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = ".000123e+3";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (ਸਿਰਾ - pp == 0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "-.000000123e6";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (ਸਿਰਾ - pp == 0);

  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "-1.23E-1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;

    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert ((ਪੂਰਨ) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (ਸਿਰਾ - pp == 0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "1e512";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e600";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-512";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-600";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "0.9999999999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0.499999999999999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "1e";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e+";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = ".";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-1e";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-.";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
  }

  // But be careful with fractional parts
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = " \t 123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-0.0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5E10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5E+10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 1.5e10);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e308";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e309";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-309";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, ਖਾਲੀ_ਸੂਚਕ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "5e-324";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, ਖਾਲੀ_ਸੂਚਕ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e+5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1E5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-1.5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == -1.5);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5E-10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-.5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == -0.5);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "500";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-7.5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "123 ";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = " 123";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 123.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 1.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+1.5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 1.5);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "--1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "+-1";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0e5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 0.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5e";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5e+";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e9999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-1e9999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-9999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, ਖਾਲੀ_ਸੂਚਕ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "-1e-9999999";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, ਖਾਲੀ_ਸੂਚਕ));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-400";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "007";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 7.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "0.0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "NaN";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "inf";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1..5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.5.5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "   ";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = " abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    ਅਚਲ ਅੱਖਰ str[] = "1.7976931348623157e308";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "99999999999999999999"; // 20 nines
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1.e5";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Exponent with leading zeros */
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e005";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == ਸਿਰਾ);
  }
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e-005";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    ਅਚਲ ਅੱਖਰ str[] = "-0";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* Long mantissa followed by an exponent */
  {
    ਅਚਲ ਅੱਖਰ str[] = "12345678901234567890e-10";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == ਸਿਰਾ);
  }

  /* whole_buffer failure must not touch *pv */
  {
    ਅਚਲ ਅੱਖਰ str[] = "123abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv, ਸੱਚ));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    ਅਚਲ ਅੱਖਰ str[] = "abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e5abc";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    ਅਚਲ ਅੱਖਰ str[] = "1e+5x";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    ਅਚਲ ਅੱਖਰ str[] = "1ex";
    ਅਚਲ ਅੱਖਰ *pp = str;
    ਅਚਲ ਅੱਖਰ *ਸਿਰਾ = str + ARRAY_ਲੰਮਾਈ (str) - 1;
    ਦੁੱਗਣਾ pv;
    hb_always_assert (hb_parse_double (&pp, ਸਿਰਾ, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  ਵਾਪਸ 0;
}
