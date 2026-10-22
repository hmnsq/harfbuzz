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

#అన్తర్భావ <cmath>
#అన్తర్భావ <cstdlib>

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-number.hh"

స్థిర శూన్య
test_parse_int ()
{
  {
    స్థిరరాశి అక్షరం str[] = "123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + 3;

    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 0);
    hb_always_assert (!*చివర);
  }

  {
    స్థిరరాశి అక్షరం str[] = "-123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + 4;

    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (చివర - pp == 0);
    hb_always_assert (!*చివర);
  }

  /* Leading whitespace */
  {
    స్థిరరాశి అక్షరం str[] = "   123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "\t123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "\n123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = " -123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == -123);
    hb_always_assert (pp == చివర);
  }

  /* Leading '+' sign */
  {
    స్థిరరాశి అక్షరం str[] = "+123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }

  /* Leading zeros are decimal, not octal (base is passed as 10) */
  {
    స్థిరరాశి అక్షరం str[] = "007";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == చివర);
  }

  /* Negative zero */
  {
    స్థిరరాశి అక్షరం str[] = "-0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }

  /* Partial parse: stops at first non-digit */
  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == str + 3);
  }

  /* INT_MAX / INT_MIN (fits in long on every supported platform) */
  {
    స్థిరరాశి అక్షరం str[] = "2147483647";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-2147483648";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == -2147483647 - 1);
    hb_always_assert (pp == చివర);
  }

  /* Failures: no digits */
  {
    స్థిరరాశి అక్షరం str[] = "   ";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }

  /* Multiple signs fail */
  {
    స్థిరరాశి అక్షరం str[] = "++1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "--1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "+-1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-+1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
  }

  /* whole_buffer: leading whitespace ok, trailing content rejected,
   * and *pp / *pv are left untouched on failure */
  {
    స్థిరరాశి అక్షరం str[] = " 123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv, నిజం));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    /* 10 digits > INT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    స్థిరరాశి అక్షరం str[] = "9999999999";   /* 10 digits, > INT_MAX */
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MAX / INT_MIN - exactly representable */
  {
    స్థిరరాశి అక్షరం str[] = "2147483647";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 2147483647);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-2147483648";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == -2147483647 - 1);   /* not -2147483648, that's UB as an int literal */
    hb_always_assert (pp == చివర);
  }

  /* One past INT_MAX - must fail */
  {
    స్థిరరాశి అక్షరం str[] = "2147483648";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* One past INT_MIN - must fail */
  {
    స్థిరరాశి అక్షరం str[] = "-2147483649";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Same for signed */
  {
    స్థిరరాశి అక్షరం str[] = "9999999999999999999999999999999999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    /* Verify *pv and *pp are untouched when a range check rejects. */
    స్థిరరాశి అక్షరం str[] = "9999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 42;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    స్థిరరాశి అక్షరం str[] = "- 123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Plus zero, for symmetry with the -0 case */
  {
    స్థిరరాశి అక్షరం str[] = "+0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }

  /* Sign alone plus trailing whitespace still fails */
  {
    స్థిరరాశి అక్షరం str[] = "-   ";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_int (&pp, చివర, &pv));
    hb_always_assert (pv == 99);
  }
}

స్థిర శూన్య
test_parse_uint ()
{
  {
    స్థిరరాశి అక్షరం str[] = "123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + అక్షరశ్రేణి_పొడవు (str);

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 0);
    hb_always_assert (!*చివర);
  }

  {
    స్థిరరాశి అక్షరం str[] = "12F";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + 3;

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 0);
    hb_always_assert (!*చివర);
  }

  {
    స్థిరరాశి అక్షరం str[] = "12Fq";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + 4;

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 16));
    hb_always_assert (pv == 0x12F);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 1);
    hb_always_assert (!*చివర);
  }

  {
    స్థిరరాశి అక్షరం str[] = "123";
    స్థిరరాశి అక్షరం *pp = str;
    hb_always_assert (ARRAY_నిడివి (str) == 4);
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str);

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 1);
  }

  {
    స్థిరరాశి అక్షరం str[] = "123\0";
    స్థిరరాశి అక్షరం *pp = str;
    hb_always_assert (ARRAY_నిడివి (str) == 5);
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str);

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 2);
  }

  {
    స్థిరరాశి అక్షరం str[] = "123V";
    స్థిరరాశి అక్షరం *pp = str;
    hb_always_assert (ARRAY_నిడివి (str) == 5);
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str);

    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp - str == 3);
    hb_always_assert (చివర - pp == 2);
  }

  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv, నిజం));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* Leading whitespace */
  {
    స్థిరరాశి అక్షరం str[] = " 123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "\t123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 123);
    hb_always_assert (pp == చివర);
  }

  /* Leading zeros (still decimal when base is 10) */
  {
    స్థిరరాశి అక్షరం str[] = "007";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 7);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "000";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }

  /* UINT_MAX */
  {
    స్థిరరాశి అక్షరం str[] = "4294967295";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == చివర);
  }

  /* Negative wraps in two's complement (matches strtoul on unsigned) */
  {
    స్థిరరాశి అక్షరం str[] = "-1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }

  /* Base 2 */
  {
    స్థిరరాశి అక్షరం str[] = "1010";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 2));
    hb_always_assert (pv == 10);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "11111111";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 2));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "12";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    /* '2' is not a base-2 digit; parse stops at index 1 */
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 2));
    hb_always_assert (pv == 1);
    hb_always_assert (pp == str + 1);
  }

  /* Base 8 */
  {
    స్థిరరాశి అక్షరం str[] = "777";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 8));
    hb_always_assert (pv == 511);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "010";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 8));
    hb_always_assert (pv == 8);
    hb_always_assert (pp == చివర);
  }

  /* Base 16, with and without 0x prefix, mixed case */
  {
    స్థిరరాశి అక్షరం str[] = "FF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "ff";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "Ff";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0xFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0XFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }

  /* Base 36 */
  {
    స్థిరరాశి అక్షరం str[] = "Z";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "z";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 36));
    hb_always_assert (pv == 35);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 36));
    hb_always_assert (pv == 36);
    hb_always_assert (pp == చివర);
  }

  /* Failures */
  {
    స్థిరరాశి అక్షరం str[] = "abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    /* 'a' isn't a decimal digit; whole parse fails */
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 10));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
  }

  {
    స్థిరరాశి అక్షరం str[] = "0XFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0xff";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 255);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0x0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == చివర);
  }
  /* "0x" alone: consume only the 0, leave the x */
  {
    స్థిరరాశి అక్షరం str[] = "0x";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* "0xG": 0 is consumed, xG left */
  {
    స్థిరరాశి అక్షరం str[] = "0xG";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  /* Negative with prefix */
  {
    స్థిరరాశి అక్షరం str[] = "-0xFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == (గుర్తులేని) -255);
    hb_always_assert (pp == చివర);
  }
  /* Prefix rule doesn't fire for base 10 */
  {
    స్థిరరాశి అక్షరం str[] = "0xFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 10));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 1);
  }
  {
    /* 10 digits > UINT_MAX.  The old strtol-based implementation silently
     * truncated on 64-bit platforms (unsigned long is 64-bit) but rejected on
     * 32-bit; the new implementation rejects uniformly. */
    స్థిరరాశి అక్షరం str[] = "9999999999";   /* 10 digits, > UINT_MAX */
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* UINT_MAX - exactly representable */
  {
    స్థిరరాశి అక్షరం str[] = "4294967295";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == చివర);
  }
  /* One past UINT_MAX - must fail (currently truncates to 0) */
  {
    స్థిరరాశి అక్షరం str[] = "4294967296";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 20 digits, way past UINT_MAX */
  {
    స్థిరరాశి అక్షరం str[] = "99999999999999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }
  /* 40 digits, past the old 32-byte stack buffer */
  {
    స్థిరరాశి అక్షరం str[] = "9999999999999999999999999999999999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 0xFFFFFFFF = UINT_MAX */
  {
    స్థిరరాశి అక్షరం str[] = "0xFFFFFFFF";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 4294967295u);
    hb_always_assert (pp == చివర);
  }
  /* 0x100000000 = UINT_MAX + 1 */
  {
    స్థిరరాశి అక్షరం str[] = "0x100000000";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* 13 'Z's = 36^13 - 1, comfortably > UINT64_MAX would need ~12 digits */
  {
    స్థిరరాశి అక్షరం str[] = "ZZZZZZZZZZZZZZ";   /* 14 Z's */
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv, నిజం, 36));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  {
    స్థిరరాశి అక్షరం str[] = "9999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 42;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42);
  }

  /* Negative values whose magnitude exceeds UINT_MAX must fail, not wrap */
  {
    స్థిరరాశి అక్షరం str[] = "-4294967296";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99);
  }

  /* INT_MIN magnitude as unsigned: wraps to 0x80000000 */
  {
    స్థిరరాశి అక్షరం str[] = "-2147483648";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 2147483648u);
    hb_always_assert (pp == చివర);
  }

  /* Mixed case in base 36 */
  {
    స్థిరరాశి అక్షరం str[] = "Az";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 36));
    hb_always_assert (pv == 10 * 36 + 35);
    hb_always_assert (pp == చివర);
  }

  /* Whitespace between sign and digits is not allowed */
  {
    స్థిరరాశి అక్షరం str[] = "- 1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv = 99;
    hb_always_assert (!hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 99);
  }

  /* "0x" prefix is case-insensitive but only for base 16 */
  {
    స్థిరరాశి అక్షరం str[] = "0X10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, నిజం, 16));
    hb_always_assert (pv == 16);
    hb_always_assert (pp == చివర);
  }

  /* A second "0x" is not consumed as a prefix */
  {
    స్థిరరాశి అక్షరం str[] = "0x0x10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv, అబద్ధం, 16));
    hb_always_assert (pv == 0);
    hb_always_assert (pp == str + 3);
  }

  /* Null byte stops the scan (end passed at NUL) */
  {
    స్థిరరాశి అక్షరం str[] = "12\0F";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    గుర్తులేని పూర్ణాంకం pv;
    hb_always_assert (hb_parse_uint (&pp, చివర, &pv));
    hb_always_assert (pv == 12);
    hb_always_assert (pp == str + 2);
  }
}

/* Tolerate 1 ULP difference. Needed because 32-bit x86 (x87) computes
 * at 80-bit precision and rounds to 64-bit on store, which can differ
 * from a directly-rounded decimal literal by one ULP. */
స్థిర అంతఃవరుస తార్కిక
almost_equal (ద్విగుణ a, ద్విగుణ b)
{
  తిరిగి fabs (a - b) <= fabs (a) * 2 * DBL_EPSILON;
}

స్థిర శూన్య
test_parse_double (శూన్య)
{
  {
    స్థిరరాశి అక్షరం str[] = ".123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str);

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 4);
    hb_always_assert (చివర - pp == 1);
  }

  {
    స్థిరరాశి అక్షరం str[] = "0.123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 5);
    hb_always_assert (చివర - pp == 0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "0.123e0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 7);
    hb_always_assert (చివర - pp == 0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "123e-3";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 6);
    hb_always_assert (చివర - pp == 0);
  }

  {
    స్థిరరాశి అక్షరం str[] = ".000123e+3";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == 123);
    hb_always_assert (pp - str == 10);
    hb_always_assert (చివర - pp == 0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "-.000000123e6";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 13);
    hb_always_assert (చివర - pp == 0);

  }

  {
    స్థిరరాశి అక్షరం str[] = "-1.23E-1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;

    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert ((పూర్ణాంకం) roundf (pv * 1000.) == -123);
    hb_always_assert (pp - str == 8);
    hb_always_assert (చివర - pp == 0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "1e512";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e600";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-512";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-600";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 0.0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "0.9999999999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0.499999999999999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 0.5);
  }

  {
    స్థిరరాశి అక్షరం str[] = "1e";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e+";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99);
  }
  {
    స్థిరరాశి అక్షరం str[] = ".";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-1e";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == -1.0);
    hb_always_assert (pp == str + 2);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-.";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
  }

  // But be careful with fractional parts
  {
    స్థిరరాశి అక్షరం str[] = "1.";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // at least "1." was consumed
  }

  {
    స్థిరరాశి అక్షరం str[] = "abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = " \t 123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-0.0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5E10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 1.5e10);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5E+10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 1.5e10);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e308";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv > 0 && pv <= DBL_MAX);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e309";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (std::isinf (pv) && pv > 0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-309";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, శూన్యసూచిక));
  }
  {
    స్థిరరాశి అక్షరం str[] = "5e-324";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv > 0.0 && pv < DBL_MIN);
    hb_always_assert (pv == strtod (str, శూన్యసూచిక));
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e+5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1E5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 100000.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 123.0);
    hb_always_assert (pp == str + 3);
  }
  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv, నిజం));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-1.5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == -1.5);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5E-10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (almost_equal (pv, 1.5e-10));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-.5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == -0.5);
  }
  {
    స్థిరరాశి అక్షరం str[] = "500";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 500.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-7.5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == -7.5);
  }
  {
    స్థిరరాశి అక్షరం str[] = "123 ";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv, నిజం));
  }
  {
    స్థిరరాశి అక్షరం str[] = " 123";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 123.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 1.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+1.5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 1.5);
  }
  {
    స్థిరరాశి అక్షరం str[] = "--1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "+-1";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0e5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 0.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5e";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5e+";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e9999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == DBL_MAX);
  }
  {
    స్థిరరాశి అక్షరం str[] = "-1e9999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == -DBL_MAX);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-9999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == .0);
    hb_always_assert (pv == strtod (str, శూన్యసూచిక));
  }
  {
    స్థిరరాశి అక్షరం str[] = "-1e-9999999";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == -.0);
    hb_always_assert (pv == -strtod (str, శూన్యసూచిక));
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-400";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 0.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "007";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 7.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "0.0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 0.0 && !std::signbit (pv));
  }
  {
    స్థిరరాశి అక్షరం str[] = "NaN";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "inf";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1..5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 2); // Don't consume ending ".5"
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.5.5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.5);
    hb_always_assert (pp == str + 3); // Don't consume ending ".5"
  }

  {
    స్థిరరాశి అక్షరం str[] = "   ";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 99.0);
  }
  {
    స్థిరరాశి అక్షరం str[] = " abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 99.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 99.0);
  }

  {
    స్థిరరాశి అక్షరం str[] = "1.7976931348623157e308";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv >= DBL_MAX || std::isinf (pv));
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "99999999999999999999"; // 20 nines
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv >= 1e19 && pv <= 1.1e20);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1.e5";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == చివర);
  }

  /* Exponent with leading zeros */
  {
    స్థిరరాశి అక్షరం str[] = "1e005";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == చివర);
  }
  {
    స్థిరరాశి అక్షరం str[] = "1e-005";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (almost_equal (pv, 1e-5));
    hb_always_assert (pp == చివర);
  }

  /* Bare -0 (no decimal) preserves the sign, matching strtod */
  {
    స్థిరరాశి అక్షరం str[] = "-0";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv == 0.0 && std::signbit (pv));
    hb_always_assert (pp == చివర);
  }

  /* Long mantissa followed by an exponent */
  {
    స్థిరరాశి అక్షరం str[] = "12345678901234567890e-10";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pv > 1.234e9 && pv < 1.235e9);
    hb_always_assert (pp == చివర);
  }

  /* whole_buffer failure must not touch *pv */
  {
    స్థిరరాశి అక్షరం str[] = "123abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv, నిజం));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Same for no-match failure */
  {
    స్థిరరాశి అక్షరం str[] = "abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv = 42.0;
    hb_always_assert (!hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pp == str);
    hb_always_assert (pv == 42.0);
  }

  /* Exponent followed by trailing content */
  {
    స్థిరరాశి అక్షరం str[] = "1e5abc";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 3);
  }
  /* Same with a sign in the exponent */
  {
    స్థిరరాశి అక్షరం str[] = "1e+5x";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 100000.0);
    hb_always_assert (pp == str + 4);
  }
  /* Malformed exponent followed by content: should stop at the 'e' */
  {
    స్థిరరాశి అక్షరం str[] = "1ex";
    స్థిరరాశి అక్షరం *pp = str;
    స్థిరరాశి అక్షరం *చివర = str + ARRAY_నిడివి (str) - 1;
    ద్విగుణ pv;
    hb_always_assert (hb_parse_double (&pp, చివర, &pv));
    hb_always_assert (pv == 1.0);
    hb_always_assert (pp == str + 1);
  }
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_parse_double ();
  test_parse_int ();
  test_parse_uint ();

  తిరిగి 0;
}
