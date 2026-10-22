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

#جے_بنا_تعریف HB_سمتیہ_BUF_HH
#تعریف_کرو HB_سمتیہ_BUF_HH

#ضمن "hb.hh"
#ضمن "hb-limits.hh"
#ضمن "hb-vector.hh"
#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <string.h>

HB_اندرونی ثابت حرف *
hb_سمتیہ_decimal_نقطہ_get (فراغ);

ساكن ثابت مزدوج hb_سمتیہ_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

بنية hb_سمتیہ_buf_t : hb_سمتیہ_t<حرف>
{
  بنانشانی precision = 2;

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
  منطقي وچ_error () ثابت
  { أرجع hb_سمتیہ_t<حرف>::وچ_error () || overflowed; }

  فراغ clear ()
  {
    overflowed = خطأ;
    hb_سمتیہ_t<حرف>::clear ();
  }

  بنانشانی scale_precision () ثابت
  { أرجع precision < 7 ? 7 : precision; }

  منطقي would_حد_توں_ودھ (بنانشانی جوڑو)
  {
    إذا (unlikely (وچ_error ()))
      أرجع سچ;
    بنانشانی new_len;
    إذا (unlikely (hb_unsigned_جوڑو_overflows (لمائی, جوڑو, &new_len) ||
		  new_len > HB_سمتیہ_MAX_DOCUMENT_ناپ))
    {
      overflowed = سچ;
      أرجع سچ;
    }
    أرجع خطأ;
  }

  منطقي append_len (ثابت حرف *s, بنانشانی l)
  {
    إذا (unlikely (would_حد_توں_ودھ (l)))
      أرجع خطأ;
    بنانشانی old_len = لمائی;
    بنانشانی new_len;
    إذا (unlikely (hb_unsigned_جوڑو_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;
    hb_memcpy (arrayZ + old_len, s, l);
    أرجع سچ;
  }

  منطقي append_c (حرف ch)
  {
    إذا (unlikely (would_حد_توں_ودھ (1)))
      أرجع خطأ;
    أرجع ڈالو_or_fail (ch);
  }

  منطقي append_str (ثابت حرف *s)
  { أرجع append_len (s, (بنانشانی) لڑی_لمبائی (s)); }

  منطقي append_unsigned (بنانشانی v)
  {
    حرف tmp[16];
    snprintf (tmp, حجم (tmp), "%u", v);
    أرجع append_len (tmp, (بنانشانی) لڑی_لمبائی (tmp));
  }

  منطقي append_hex_byte (بنانشانی v)
  {
    حرف tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    أرجع append_len (tmp, 2);
  }

  فراغ append_عدد (عائم v)
  { append_عدد (v, precision); }

  فراغ append_عدد (عائم v, بنانشانی p)
  {
    إذا (p > 12) p = 12;

    عائم rounded_zero_حد = 0.5f;
    لكل (بنانشانی i = 0; i < p; i++)
      rounded_zero_حد *= 0.1f;
    إذا (fabsf (v) < rounded_zero_حد)
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
    مزدوج scaled = (مزدوج) v * hb_سمتیہ_buf_pow10[p];
    إذا (likely (fabs (scaled) < 9.0e18))
    {
      مزدوج fl = floor (scaled);
      مزدوج diff = scaled - fl;
      int64_t n = (int64_t) fl;
      إذا (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      بنانشانی frac = p;
      طالما (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      حرف tmp[24];
      حرف *سرا = tmp + حجم (tmp);
      حرف *s = سرا;
      بنانشانی d = 0;
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

      append_len (s, (بنانشانی) (سرا - s));
      أرجع;
    }

    حرف fmt[6];
    snprintf (fmt, حجم (fmt), "%%.%uf", p);
    حرف باہرلا[128];
    snprintf (باہرلا, حجم (باہرلا), fmt, (مزدوج) v);

    ثابت حرف *decimal_point = hb_سمتیہ_decimal_نقطہ_get ();

    إذا (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      حرف *dp = لڑی_کھوج (باہرلا, decimal_point);
      إذا (dp)
      {
	بنانشانی dp_len = (بنانشانی) لڑی_لمبائی (decimal_point);
	بنانشانی پوچھ_len = (بنانشانی) لڑی_لمبائی (dp + dp_len);
	memmove (dp + 1, dp + dp_len, پوچھ_len + 1);
	*dp = '.';
      }
    }

    حرف *dot = strchr (باہرلا, '.');
    إذا (dot)
    {
      حرف *سرا = باہرلا + لڑی_لمبائی (باہرلا) - 1;
      طالما (سرا > dot && *سرا == '0')
	*سرا-- = '\0';
      إذا (سرا == dot)
	*سرا = '\0';
    }

    append_len (باہرلا, (بنانشانی) لڑی_لمبائی (باہرلا));
  }

  فراغ append_svg_color (hb_color_t color, منطقي with_alpha)
  {
    بنانشانی r = hb_color_get_red (color);
    بنانشانی g = hb_color_get_green (color);
    بنانشانی b = hb_color_get_blue (color);
    بنانشانی a = hb_color_get_alpha (color);
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

  منطقي append_base64 (ثابت uint8_t *انکڑے, بنانشانی len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    بنانشانی ngroups = len / 3 + (len % 3 != 0);
    بنانشانی باہرلا_len;
    بنانشانی old_len = لمائی;
    بنانشانی new_len;
    إذا (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &باہرلا_len) ||
		  would_حد_توں_ودھ (باہرلا_len) ||
		  hb_unsigned_جوڑو_overflows (old_len, باہرلا_len, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      أرجع خطأ;

    حرف *dst = arrayZ + old_len;
    بنانشانی di = 0;
    بنانشانی i = 0;
    طالما (i + 2 < len)
    {
      بنانشانی v = ((بنانشانی) انکڑے[i] << 16) |
		   ((بنانشانی) انکڑے[i + 1] << 8) |
		   ((بنانشانی) انکڑے[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    إذا (i < len)
    {
      بنانشانی v = (بنانشانی) انکڑے[i] << 16;
      إذا (i + 1 < len)
	v |= (بنانشانی) انکڑے[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    أرجع سچ;
  }
};

#جے_انت /* HB_VECTOR_BUF_HH */
