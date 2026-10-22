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

#اگر_تعریف_نشده HB_وکتور_BUF_HH
#تعریف_کن HB_وکتور_BUF_HH

#ضمن "hb.hh"
#ضمن "hb-limits.hh"
#ضمن "hb-vector.hh"
#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <string.h>

HB_داخلی ثابت حرف *
hb_وکتور_decimal_نقطه_get (فراغ);

ساكن ثابت مزدوج hb_وکتور_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

بنية hb_وکتور_buf_t : hb_وکتور_t<حرف>
{
  بیعلامت precision = 2;

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
  منطقي درون_error () ثابت
  { أرجع hb_وکتور_t<حرف>::درون_error () || overflowed; }

  فراغ clear ()
  {
    overflowed = خطأ;
    hb_وکتور_t<حرف>::clear ();
  }

  بیعلامت scale_precision () ثابت
  { أرجع precision < 7 ? 7 : precision; }

  منطقي would_سرریز (بیعلامت جمع_کن)
  {
    إذا (unlikely (درون_error ()))
      أرجع درست;
    بیعلامت new_len;
    إذا (unlikely (hb_unsigned_جمع_کن_overflows (درازی, جمع_کن, &new_len) ||
		  new_len > HB_وکتور_MAX_DOCUMENT_اندازه))
    {
      overflowed = درست;
      أرجع درست;
    }
    أرجع خطأ;
  }

  منطقي append_len (ثابت حرف *s, بیعلامت l)
  {
    إذا (unlikely (would_سرریز (l)))
      أرجع خطأ;
    بیعلامت old_len = درازی;
    بیعلامت new_len;
    إذا (unlikely (hb_unsigned_جمع_کن_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;
    hb_memcpy (arrayZ + old_len, s, l);
    أرجع درست;
  }

  منطقي append_c (حرف ch)
  {
    إذا (unlikely (would_سرریز (1)))
      أرجع خطأ;
    أرجع افزودن_or_fail (ch);
  }

  منطقي append_str (ثابت حرف *s)
  { أرجع append_len (s, (بیعلامت) طول_رشته (s)); }

  منطقي append_unsigned (بیعلامت v)
  {
    حرف tmp[16];
    snprintf (tmp, حجم (tmp), "%u", v);
    أرجع append_len (tmp, (بیعلامت) طول_رشته (tmp));
  }

  منطقي append_hex_byte (بیعلامت v)
  {
    حرف tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    أرجع append_len (tmp, 2);
  }

  فراغ append_رقم (عائم v)
  { append_رقم (v, precision); }

  فراغ append_رقم (عائم v, بیعلامت p)
  {
    إذا (p > 12) p = 12;

    عائم rounded_zero_آستانه = 0.5f;
    لكل (بیعلامت i = 0; i < p; i++)
      rounded_zero_آستانه *= 0.1f;
    إذا (fabsf (v) < rounded_zero_آستانه)
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
    مزدوج scaled = (مزدوج) v * hb_وکتور_buf_pow10[p];
    إذا (likely (fabs (scaled) < 9.0e18))
    {
      مزدوج fl = floor (scaled);
      مزدوج diff = scaled - fl;
      int64_t n = (int64_t) fl;
      إذا (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      بیعلامت frac = p;
      طالما (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      حرف tmp[24];
      حرف *انتها = tmp + حجم (tmp);
      حرف *s = انتها;
      بیعلامت d = 0;
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

      append_len (s, (بیعلامت) (انتها - s));
      أرجع;
    }

    حرف fmt[6];
    snprintf (fmt, حجم (fmt), "%%.%uf", p);
    حرف بیرون[128];
    snprintf (بیرون, حجم (بیرون), fmt, (مزدوج) v);

    ثابت حرف *decimal_point = hb_وکتور_decimal_نقطه_get ();

    إذا (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      حرف *dp = جستجوی_رشته (بیرون, decimal_point);
      إذا (dp)
      {
	بیعلامت dp_len = (بیعلامت) طول_رشته (decimal_point);
	بیعلامت دم_len = (بیعلامت) طول_رشته (dp + dp_len);
	memmove (dp + 1, dp + dp_len, دم_len + 1);
	*dp = '.';
      }
    }

    حرف *dot = strchr (بیرون, '.');
    إذا (dot)
    {
      حرف *انتها = بیرون + طول_رشته (بیرون) - 1;
      طالما (انتها > dot && *انتها == '0')
	*انتها-- = '\0';
      إذا (انتها == dot)
	*انتها = '\0';
    }

    append_len (بیرون, (بیعلامت) طول_رشته (بیرون));
  }

  فراغ append_svg_color (hb_color_t color, منطقي with_alpha)
  {
    بیعلامت r = hb_color_get_red (color);
    بیعلامت g = hb_color_get_green (color);
    بیعلامت b = hb_color_get_blue (color);
    بیعلامت a = hb_color_get_alpha (color);
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
      append_رقم (a / 255.f, 4);
    }
  }

  منطقي append_base64 (ثابت uint8_t *داده, بیعلامت len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    بیعلامت ngroups = len / 3 + (len % 3 != 0);
    بیعلامت بیرون_len;
    بیعلامت old_len = درازی;
    بیعلامت new_len;
    إذا (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &بیرون_len) ||
		  would_سرریز (بیرون_len) ||
		  hb_unsigned_جمع_کن_overflows (old_len, بیرون_len, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;

    حرف *dst = arrayZ + old_len;
    بیعلامت di = 0;
    بیعلامت i = 0;
    طالما (i + 2 < len)
    {
      بیعلامت v = ((بیعلامت) داده[i] << 16) |
		   ((بیعلامت) داده[i + 1] << 8) |
		   ((بیعلامت) داده[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    إذا (i < len)
    {
      بیعلامت v = (بیعلامت) داده[i] << 16;
      إذا (i + 1 < len)
	v |= (بیعلامت) داده[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    أرجع درست;
  }
};

#پایان_اگر /* HB_VECTOR_BUF_HH */
