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

#اگر_غیر_تعریف_شدہ HB_سمتیہ_BUF_HH
#تعریف_کرو HB_سمتیہ_BUF_HH

#شامل "hb.hh"
#شامل "hb-limits.hh"
#شامل "hb-vector.hh"
#شامل <math.h>
#شامل <stdio.h>
#شامل <string.h>

HB_اندرونی ثابت حرف *
hb_سمتیہ_decimal_نقطہ_get (خالی);

ساکن ثابت دوہرا hb_سمتیہ_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

ساخت hb_سمتیہ_buf_t : hb_سمتیہ_t<حرف>
{
  غیرعلامتی precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  منطقی overflowed = جھوٹ;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  منطقی اندر_error () ثابت
  { واپس hb_سمتیہ_t<حرف>::اندر_error () || overflowed; }

  خالی clear ()
  {
    overflowed = جھوٹ;
    hb_سمتیہ_t<حرف>::clear ();
  }

  غیرعلامتی scale_precision () ثابت
  { واپس precision < 7 ? 7 : precision; }

  منطقی would_حد_سے_تجاوز (غیرعلامتی جوڑو)
  {
    اگر (unlikely (اندر_error ()))
      واپس سچ;
    غیرعلامتی new_len;
    اگر (unlikely (hb_unsigned_جوڑو_overflows (لمبائی, جوڑو, &new_len) ||
		  new_len > HB_سمتیہ_MAX_DOCUMENT_سائز))
    {
      overflowed = سچ;
      واپس سچ;
    }
    واپس جھوٹ;
  }

  منطقی append_len (ثابت حرف *s, غیرعلامتی l)
  {
    اگر (unlikely (would_حد_سے_تجاوز (l)))
      واپس جھوٹ;
    غیرعلامتی old_len = لمبائی;
    غیرعلامتی new_len;
    اگر (unlikely (hb_unsigned_جوڑو_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((صحیح) new_len)))
      واپس جھوٹ;
    hb_memcpy (arrayZ + old_len, s, l);
    واپس سچ;
  }

  منطقی append_c (حرف ch)
  {
    اگر (unlikely (would_حد_سے_تجاوز (1)))
      واپس جھوٹ;
    واپس ڈالو_or_fail (ch);
  }

  منطقی append_str (ثابت حرف *s)
  { واپس append_len (s, (غیرعلامتی) لڑی_لمبائی (s)); }

  منطقی append_unsigned (غیرعلامتی v)
  {
    حرف tmp[16];
    snprintf (tmp, حجم (tmp), "%u", v);
    واپس append_len (tmp, (غیرعلامتی) لڑی_لمبائی (tmp));
  }

  منطقی append_hex_byte (غیرعلامتی v)
  {
    حرف tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    واپس append_len (tmp, 2);
  }

  خالی append_عدد (اعشاری v)
  { append_عدد (v, precision); }

  خالی append_عدد (اعشاری v, غیرعلامتی p)
  {
    اگر (p > 12) p = 12;

    اعشاری rounded_zero_حد = 0.5f;
    برائے (غیرعلامتی i = 0; i < p; i++)
      rounded_zero_حد *= 0.1f;
    اگر (fabsf (v) < rounded_zero_حد)
      v = 0.f;

    اگر (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      واپس;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    دوہرا scaled = (دوہرا) v * hb_سمتیہ_buf_pow10[p];
    اگر (likely (fabs (scaled) < 9.0e18))
    {
      دوہرا fl = floor (scaled);
      دوہرا diff = scaled - fl;
      int64_t n = (int64_t) fl;
      اگر (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      غیرعلامتی frac = p;
      جب (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      حرف tmp[24];
      حرف *انتہا = tmp + حجم (tmp);
      حرف *s = انتہا;
      غیرعلامتی d = 0;
      کرو
      {
	*--s = (حرف) ('0' + (حرف) (u % 10));
	u /= 10;
	اگر (++d == frac)
	  *--s = '.';
      }
      جب (u || d < frac + 1);
      اگر (v < 0.f)
	*--s = '-';

      append_len (s, (غیرعلامتی) (انتہا - s));
      واپس;
    }

    حرف fmt[6];
    snprintf (fmt, حجم (fmt), "%%.%uf", p);
    حرف برآمد[128];
    snprintf (برآمد, حجم (برآمد), fmt, (دوہرا) v);

    ثابت حرف *decimal_point = hb_سمتیہ_decimal_نقطہ_get ();

    اگر (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      حرف *dp = لڑی_تلاش (برآمد, decimal_point);
      اگر (dp)
      {
	غیرعلامتی dp_len = (غیرعلامتی) لڑی_لمبائی (decimal_point);
	غیرعلامتی دم_len = (غیرعلامتی) لڑی_لمبائی (dp + dp_len);
	memmove (dp + 1, dp + dp_len, دم_len + 1);
	*dp = '.';
      }
    }

    حرف *dot = strchr (برآمد, '.');
    اگر (dot)
    {
      حرف *انتہا = برآمد + لڑی_لمبائی (برآمد) - 1;
      جب (انتہا > dot && *انتہا == '0')
	*انتہا-- = '\0';
      اگر (انتہا == dot)
	*انتہا = '\0';
    }

    append_len (برآمد, (غیرعلامتی) لڑی_لمبائی (برآمد));
  }

  خالی append_svg_color (hb_color_t color, منطقی with_alpha)
  {
    غیرعلامتی r = hb_color_get_red (color);
    غیرعلامتی g = hb_color_get_green (color);
    غیرعلامتی b = hb_color_get_blue (color);
    غیرعلامتی a = hb_color_get_alpha (color);
    append_c ('#');
    اگر (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    ورنہ
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    اگر (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_عدد (a / 255.f, 4);
    }
  }

  منطقی append_base64 (ثابت uint8_t *ڈیٹا, غیرعلامتی len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    غیرعلامتی ngroups = len / 3 + (len % 3 != 0);
    غیرعلامتی برآمد_len;
    غیرعلامتی old_len = لمبائی;
    غیرعلامتی new_len;
    اگر (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &برآمد_len) ||
		  would_حد_سے_تجاوز (برآمد_len) ||
		  hb_unsigned_جوڑو_overflows (old_len, برآمد_len, &new_len) ||
		  !resize_dirty ((صحیح) new_len)))
      واپس جھوٹ;

    حرف *dst = arrayZ + old_len;
    غیرعلامتی di = 0;
    غیرعلامتی i = 0;
    جب (i + 2 < len)
    {
      غیرعلامتی v = ((غیرعلامتی) ڈیٹا[i] << 16) |
		   ((غیرعلامتی) ڈیٹا[i + 1] << 8) |
		   ((غیرعلامتی) ڈیٹا[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    اگر (i < len)
    {
      غیرعلامتی v = (غیرعلامتی) ڈیٹا[i] << 16;
      اگر (i + 1 < len)
	v |= (غیرعلامتی) ڈیٹا[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    واپس سچ;
  }
};

#اگر_ختم /* HB_VECTOR_BUF_HH */
