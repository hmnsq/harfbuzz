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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-limits.hh"
#অন্তর্ভাব "hb-vector.hh"
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>

HB_INTERNAL ধ্রুবক অক্ষর *
hb_vector_decimal_point_get (শূন্য);

স্থির ধ্রুবক দ্বিগুণ _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

কাঠামো hb_vector_buf_t : hb_vector_t<অক্ষর>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  যৌক্তিক overflowed = মিথ্যা;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  যৌক্তিক in_error () ধ্রুবক
  { ফেরত hb_vector_t<অক্ষর>::in_error () || overflowed; }

  শূন্য clear ()
  {
    overflowed = মিথ্যা;
    hb_vector_t<অক্ষর>::clear ();
  }

  unsigned scale_precision () ধ্রুবক
  { ফেরত precision < 7 ? 7 : precision; }

  যৌক্তিক would_overflow (unsigned add)
  {
    যদি (unlikely (in_error ()))
      ফেরত সত্য;
    unsigned new_len;
    যদি (unlikely (hb_unsigned_add_overflows (দৈর্ঘ্য, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_আকার))
    {
      overflowed = সত্য;
      ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }

  যৌক্তিক append_len (ধ্রুবক অক্ষর *s, unsigned l)
  {
    যদি (unlikely (would_overflow (l)))
      ফেরত মিথ্যা;
    unsigned old_len = দৈর্ঘ্য;
    unsigned new_len;
    যদি (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((পূর্ণসংখ্যা) new_len)))
      ফেরত মিথ্যা;
    hb_memcpy (arrayZ + old_len, s, l);
    ফেরত সত্য;
  }

  যৌক্তিক append_c (অক্ষর ch)
  {
    যদি (unlikely (would_overflow (1)))
      ফেরত মিথ্যা;
    ফেরত ঠেলো_or_fail (ch);
  }

  যৌক্তিক append_str (ধ্রুবক অক্ষর *s)
  { ফেরত append_len (s, (unsigned) strlen (s)); }

  যৌক্তিক append_unsigned (unsigned v)
  {
    অক্ষর tmp[16];
    snprintf (tmp, আকার (tmp), "%u", v);
    ফেরত append_len (tmp, (unsigned) strlen (tmp));
  }

  যৌক্তিক append_hex_byte (unsigned v)
  {
    অক্ষর tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    ফেরত append_len (tmp, 2);
  }

  শূন্য append_অঙ্ক (দশমিক v)
  { append_অঙ্ক (v, precision); }

  শূন্য append_অঙ্ক (দশমিক v, unsigned p)
  {
    যদি (p > 12) p = 12;

    দশমিক rounded_zero_threshold = 0.5f;
    জন্য (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    যদি (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    যদি (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      ফেরত;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    দ্বিগুণ scaled = (দ্বিগুণ) v * _hb_vector_buf_pow10[p];
    যদি (likely (fabs (scaled) < 9.0e18))
    {
      দ্বিগুণ fl = floor (scaled);
      দ্বিগুণ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      যদি (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      যখন (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      অক্ষর tmp[24];
      অক্ষর *end = tmp + আকার (tmp);
      অক্ষর *s = end;
      unsigned d = 0;
      করো
      {
	*--s = (অক্ষর) ('0' + (অক্ষর) (u % 10));
	u /= 10;
	যদি (++d == frac)
	  *--s = '.';
      }
      যখন (u || d < frac + 1);
      যদি (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      ফেরত;
    }

    অক্ষর fmt[6];
    snprintf (fmt, আকার (fmt), "%%.%uf", p);
    অক্ষর out[128];
    snprintf (out, আকার (out), fmt, (দ্বিগুণ) v);

    ধ্রুবক অক্ষর *decimal_point = hb_vector_decimal_point_get ();

    যদি (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      অক্ষর *dp = strstr (out, decimal_point);
      যদি (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned লেজ_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, লেজ_len + 1);
	*dp = '.';
      }
    }

    অক্ষর *dot = strchr (out, '.');
    যদি (dot)
    {
      অক্ষর *end = out + strlen (out) - 1;
      যখন (end > dot && *end == '0')
	*end-- = '\0';
      যদি (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  শূন্য append_svg_color (hb_color_t color, যৌক্তিক with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    যদি (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    অন্যথা
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    যদি (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_অঙ্ক (a / 255.f, 4);
    }
  }

  যৌক্তিক append_base64 (ধ্রুবক uint8_t *তথ্য, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = দৈর্ঘ্য;
    unsigned new_len;
    যদি (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((পূর্ণসংখ্যা) new_len)))
      ফেরত মিথ্যা;

    অক্ষর *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    যখন (i + 2 < len)
    {
      unsigned v = ((unsigned) তথ্য[i] << 16) |
		   ((unsigned) তথ্য[i + 1] << 8) |
		   ((unsigned) তথ্য[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    যদি (i < len)
    {
      unsigned v = (unsigned) তথ্য[i] << 16;
      যদি (i + 1 < len)
	v |= (unsigned) তথ্য[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    ফেরত সত্য;
  }
};

#endif /* HB_VECTOR_BUF_HH */
