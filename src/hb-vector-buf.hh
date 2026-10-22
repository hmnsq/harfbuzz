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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-limits.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector.hh"
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>

HB_INTERNAL ಧ್ರುವ ವರ್ಣ *
hb_vector_decimal_point_get (ಶೂನ್ಯ);

ಸ್ಥಿರ ಧ್ರುವ ದ್ವಿಗುಣ _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

ಸಂರಚನಾ hb_vector_buf_t : hb_vector_t<ವರ್ಣ>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  ತಾರ್ಕಿಕ overflowed = ಅಸತ್ಯ;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_vector_t<ವರ್ಣ>::in_error () || overflowed; }

  ಶೂನ್ಯ clear ()
  {
    overflowed = ಅಸತ್ಯ;
    hb_vector_t<ವರ್ಣ>::clear ();
  }

  unsigned scale_precision () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ precision < 7 ? 7 : precision; }

  ತಾರ್ಕಿಕ would_overflow (unsigned add)
  {
    ಯದಿ (unlikely (in_error ()))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    unsigned new_len;
    ಯದಿ (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ append_len (ಧ್ರುವ ವರ್ಣ *s, unsigned l)
  {
    ಯದಿ (unlikely (would_overflow (l)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned old_len = length;
    unsigned new_len;
    ಯದಿ (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((ಪೂರ್ಣಾಙ್ಕ) new_len)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_memcpy (arrayZ + old_len, s, l);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ append_c (ವರ್ಣ ch)
  {
    ಯದಿ (unlikely (would_overflow (1)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ತಳ್ಳು_or_fail (ch);
  }

  ತಾರ್ಕಿಕ append_str (ಧ್ರುವ ವರ್ಣ *s)
  { ಪ್ರತ್ಯಾಗಮ append_len (s, (unsigned) strlen (s)); }

  ತಾರ್ಕಿಕ append_unsigned (unsigned v)
  {
    ವರ್ಣ tmp[16];
    snprintf (tmp, ಪರಿಮಾಣ (tmp), "%u", v);
    ಪ್ರತ್ಯಾಗಮ append_len (tmp, (unsigned) strlen (tmp));
  }

  ತಾರ್ಕಿಕ append_hex_byte (unsigned v)
  {
    ವರ್ಣ tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    ಪ್ರತ್ಯಾಗಮ append_len (tmp, 2);
  }

  ಶೂನ್ಯ append_ಅಂಕೆ (ಭಿನ್ನ v)
  { append_ಅಂಕೆ (v, precision); }

  ಶೂನ್ಯ append_ಅಂಕೆ (ಭಿನ್ನ v, unsigned p)
  {
    ಯದಿ (p > 12) p = 12;

    ಭಿನ್ನ rounded_zero_threshold = 0.5f;
    ಪ್ರತಿ (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    ಯದಿ (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    ಯದಿ (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    ದ್ವಿಗುಣ scaled = (ದ್ವಿಗುಣ) v * _hb_vector_buf_pow10[p];
    ಯದಿ (likely (fabs (scaled) < 9.0e18))
    {
      ದ್ವಿಗುಣ fl = floor (scaled);
      ದ್ವಿಗುಣ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      ಯದಿ (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      ಯಾವತ್ (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      ವರ್ಣ tmp[24];
      ವರ್ಣ *end = tmp + ಪರಿಮಾಣ (tmp);
      ವರ್ಣ *s = end;
      unsigned d = 0;
      ಕುರು
      {
	*--s = (ವರ್ಣ) ('0' + (ವರ್ಣ) (u % 10));
	u /= 10;
	ಯದಿ (++d == frac)
	  *--s = '.';
      }
      ಯಾವತ್ (u || d < frac + 1);
      ಯದಿ (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      ಪ್ರತ್ಯಾಗಮ;
    }

    ವರ್ಣ fmt[6];
    snprintf (fmt, ಪರಿಮಾಣ (fmt), "%%.%uf", p);
    ವರ್ಣ out[128];
    snprintf (out, ಪರಿಮಾಣ (out), fmt, (ದ್ವಿಗುಣ) v);

    ಧ್ರುವ ವರ್ಣ *decimal_point = hb_vector_decimal_point_get ();

    ಯದಿ (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      ವರ್ಣ *dp = strstr (out, decimal_point);
      ಯದಿ (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    ವರ್ಣ *dot = strchr (out, '.');
    ಯದಿ (dot)
    {
      ವರ್ಣ *end = out + strlen (out) - 1;
      ಯಾವತ್ (end > dot && *end == '0')
	*end-- = '\0';
      ಯದಿ (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  ಶೂನ್ಯ append_svg_color (hb_color_t color, ತಾರ್ಕಿಕ with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    ಯದಿ (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    ಅನ್ಯಥಾ
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    ಯದಿ (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_ಅಂಕೆ (a / 255.f, 4);
    }
  }

  ತಾರ್ಕಿಕ append_base64 (ಧ್ರುವ uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((ಪೂರ್ಣಾಙ್ಕ) new_len)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ವರ್ಣ *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    ಯಾವತ್ (i + 2 < len)
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

    ಯದಿ (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      ಯದಿ (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};

#endif /* HB_VECTOR_BUF_HH */
