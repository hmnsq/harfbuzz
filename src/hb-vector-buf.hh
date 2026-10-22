/*
 * Copyright © 2026  Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#که_نه_تعریف_شوی HB_ویکټور_BUF_HH
#تعریف_کړه HB_ویکټور_BUF_HH

#ضمن "hb.hh"
#ضمن "hb-limits.hh"
#ضمن "hb-vector.hh"
#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <string.h>

HB_دننی ثابت حرف *
hb_ویکټور_decimal_ټکی_get (فراغ);

ساكن ثابت مزدوج hb_ویکټور_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

بنية hb_ویکټور_buf_t : hb_ویکټور_t<حرف>
{
  بینښه precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  منطقي overflowed = خطأ;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  منطقي دننه_error () ثابت
  { أرجع hb_ویکټور_t<حرف>::دننه_error () || overflowed; }

  فراغ clear ()
  {
    overflowed = خطأ;
    hb_ویکټور_t<حرف>::clear ();
  }

  بینښه scale_precision () ثابت
  { أرجع precision < 7 ? 7 : precision; }

  منطقي would_ډکېدنه (بینښه جمع_کړه)
  {
    إذا (unlikely (دننه_error ()))
      أرجع ریښتیا;
    بینښه new_len;
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (اوږدتیا, جمع_کړه, &new_len) ||
		  new_len > HB_ویکټور_MAX_DOCUMENT_اندازه))
    {
      overflowed = ریښتیا;
      أرجع ریښتیا;
    }
    أرجع خطأ;
  }

  منطقي append_len (ثابت حرف *s, بینښه l)
  {
    إذا (unlikely (would_ډکېدنه (l)))
      أرجع خطأ;
    بینښه old_len = اوږدتیا;
    بینښه new_len;
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;
    hb_memcpy (arrayZ + old_len, s, l);
    أرجع ریښتیا;
  }

  منطقي append_c (حرف ch)
  {
    إذا (unlikely (would_ډکېدنه (1)))
      أرجع خطأ;
    أرجع ټېل_or_fail (ch);
  }

  منطقي append_str (ثابت حرف *s)
  { أرجع append_len (s, (بینښه) مزي_اوږدوالی (s)); }

  منطقي append_unsigned (بینښه v)
  {
    حرف tmp[16];
    snprintf (tmp, حجم (tmp), "%u", v);
    أرجع append_len (tmp, (بینښه) مزي_اوږدوالی (tmp));
  }

  منطقي append_hex_byte (بینښه v)
  {
    حرف tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    أرجع append_len (tmp, 2);
  }

  فراغ append_عدد (عائم v)
  { append_عدد (v, precision); }

  فراغ append_عدد (عائم v, بینښه p)
  {
    إذا (p > 12) p = 12;

    عائم rounded_zero_پوله = 0.5f;
    لكل (بینښه i = 0; i < p; i++)
      rounded_zero_پوله *= 0.1f;
    إذا (fabsf (v) < rounded_zero_پوله)
      v = 0.f;

    إذا (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      أرجع;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    مزدوج scaled = (مزدوج) v * hb_ویکټور_buf_pow10[p];
    إذا (likely (fabs (scaled) < 9.0e18))
    {
      مزدوج fl = floor (scaled);
      مزدوج diff = scaled - fl;
      int64_t n = (int64_t) fl;
      إذا (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      بینښه frac = p;
      طالما (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      حرف tmp[24];
      حرف *پای_ټکی = tmp + حجم (tmp);
      حرف *s = پای_ټکی;
      بینښه d = 0;
      افعل
      {
	*--s = (حرف) ('0' + (حرف) (u % 10));
	u /= 10;
	إذا (++d == frac)
	  *--s = '.';
      }
      طالما (u || d < frac + 1);
      إذا (v < 0.f)
	*--s = '-';

      append_len (s, (بینښه) (پای_ټکی - s));
      أرجع;
    }

    حرف fmt[6];
    snprintf (fmt, حجم (fmt), "%%.%uf", p);
    حرف بهر[128];
    snprintf (بهر, حجم (بهر), fmt, (مزدوج) v);

    ثابت حرف *decimal_point = hb_ویکټور_decimal_ټکی_get ();

    إذا (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      حرف *dp = مزي_پلټنه (بهر, decimal_point);
      إذا (dp)
      {
	بینښه dp_len = (بینښه) مزي_اوږدوالی (decimal_point);
	بینښه لکۍ_len = (بینښه) مزي_اوږدوالی (dp + dp_len);
	memmove (dp + 1, dp + dp_len, لکۍ_len + 1);
	*dp = '.';
      }
    }

    حرف *dot = strchr (بهر, '.');
    إذا (dot)
    {
      حرف *پای_ټکی = بهر + مزي_اوږدوالی (بهر) - 1;
      طالما (پای_ټکی > dot && *پای_ټکی == '0')
	*پای_ټکی-- = '\0';
      إذا (پای_ټکی == dot)
	*پای_ټکی = '\0';
    }

    append_len (بهر, (بینښه) مزي_اوږدوالی (بهر));
  }

  فراغ append_svg_color (hb_color_t color, منطقي with_alpha)
  {
    بینښه r = hb_color_get_red (color);
    بینښه g = hb_color_get_green (color);
    بینښه b = hb_color_get_blue (color);
    بینښه a = hb_color_get_alpha (color);
    append_c ('#');
    إذا (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    وإلا
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    إذا (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_عدد (a / 255.f, 4);
    }
  }

  منطقي append_base64 (ثابت uint8_t *معلومات, بینښه len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    بینښه ngroups = len / 3 + (len % 3 != 0);
    بینښه بهر_len;
    بینښه old_len = اوږدتیا;
    بینښه new_len;
    إذا (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &بهر_len) ||
		  would_ډکېدنه (بهر_len) ||
		  hb_unsigned_جمع_کړه_overflows (old_len, بهر_len, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;

    حرف *dst = arrayZ + old_len;
    بینښه di = 0;
    بینښه i = 0;
    طالما (i + 2 < len)
    {
      بینښه v = ((بینښه) معلومات[i] << 16) |
		   ((بینښه) معلومات[i + 1] << 8) |
		   ((بینښه) معلومات[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    إذا (i < len)
    {
      بینښه v = (بینښه) معلومات[i] << 16;
      إذا (i + 1 < len)
	v |= (بینښه) معلومات[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    أرجع ریښتیا;
  }
};

#د_که_پای /* HB_VECTOR_BUF_HH */
