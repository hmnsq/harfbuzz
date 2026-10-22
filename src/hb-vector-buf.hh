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

#ifndef HB_VECTOR_BUF_HH
#define HB_VECTOR_BUF_HH

#ضمن "hb.hh"
#ضمن "hb-limits.hh"
#ضمن "hb-vector.hh"
#ضمن <math.h>
#ضمن <stdio.h>
#ضمن <string.h>

HB_INTERNAL ثابت حرف *
hb_vector_decimal_point_get (فراغ);

ساكن ثابت مزدوج _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

بنية hb_vector_buf_t : hb_vector_t<حرف>
{
  unsigned precision = 2;

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
  منطقي in_error () ثابت
  { إرجاع hb_vector_t<حرف>::in_error () || overflowed; }

  فراغ clear ()
  {
    overflowed = خطأ;
    hb_vector_t<حرف>::clear ();
  }

  unsigned scale_precision () ثابت
  { إرجاع precision < 7 ? 7 : precision; }

  منطقي would_overflow (unsigned add)
  {
    إذا (unlikely (in_error ()))
      إرجاع true;
    unsigned new_len;
    إذا (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = true;
      إرجاع true;
    }
    إرجاع خطأ;
  }

  منطقي append_len (ثابت حرف *s, unsigned l)
  {
    إذا (unlikely (would_overflow (l)))
      إرجاع خطأ;
    unsigned old_len = length;
    unsigned new_len;
    إذا (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      إرجاع خطأ;
    hb_memcpy (arrayZ + old_len, s, l);
    إرجاع true;
  }

  منطقي append_c (حرف ch)
  {
    إذا (unlikely (would_overflow (1)))
      إرجاع خطأ;
    إرجاع ادفع_or_fail (ch);
  }

  منطقي append_str (ثابت حرف *s)
  { إرجاع append_len (s, (unsigned) strlen (s)); }

  منطقي append_unsigned (unsigned v)
  {
    حرف tmp[16];
    snprintf (tmp, حجم (tmp), "%u", v);
    إرجاع append_len (tmp, (unsigned) strlen (tmp));
  }

  منطقي append_hex_byte (unsigned v)
  {
    حرف tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    إرجاع append_len (tmp, 2);
  }

  فراغ append_رقم (عائم v)
  { append_رقم (v, precision); }

  فراغ append_رقم (عائم v, unsigned p)
  {
    إذا (p > 12) p = 12;

    عائم rounded_zero_threshold = 0.5f;
    لكل (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    إذا (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    إذا (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      إرجاع;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    مزدوج scaled = (مزدوج) v * _hb_vector_buf_pow10[p];
    إذا (likely (fabs (scaled) < 9.0e18))
    {
      مزدوج fl = floor (scaled);
      مزدوج diff = scaled - fl;
      int64_t n = (int64_t) fl;
      إذا (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      بينما (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      حرف tmp[24];
      حرف *end = tmp + حجم (tmp);
      حرف *s = end;
      unsigned d = 0;
      افعل
      {
	*--s = (حرف) ('0' + (حرف) (u % 10));
	u /= 10;
	إذا (++d == frac)
	  *--s = '.';
      }
      بينما (u || d < frac + 1);
      إذا (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      إرجاع;
    }

    حرف fmt[6];
    snprintf (fmt, حجم (fmt), "%%.%uf", p);
    حرف out[128];
    snprintf (out, حجم (out), fmt, (مزدوج) v);

    ثابت حرف *decimal_point = hb_vector_decimal_point_get ();

    إذا (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      حرف *dp = strstr (out, decimal_point);
      إذا (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    حرف *dot = strchr (out, '.');
    إذا (dot)
    {
      حرف *end = out + strlen (out) - 1;
      بينما (end > dot && *end == '0')
	*end-- = '\0';
      إذا (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  فراغ append_svg_color (hb_color_t color, منطقي with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
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

  منطقي append_base64 (ثابت uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    إذا (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((صحيح) new_len)))
      إرجاع خطأ;

    حرف *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    بينما (i + 2 < len)
    {
      unsigned v = ((unsigned) data[i] << 16) |
		   ((unsigned) data[i + 1] << 8) |
		   ((unsigned) data[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    إذا (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      إذا (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    إرجاع true;
  }
};

#endif /* HB_VECTOR_BUF_HH */
