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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-limits.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

HB_INTERNAL ଅଚଳ ଅକ୍ଷର *
hb_vector_decimal_point_get (ଶୂନ୍ୟ);

ସ୍ଥିର ଅଚଳ ଦ୍ୱିଗୁଣ _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

ସଂରଚନା hb_vector_buf_t : hb_vector_t<ଅକ୍ଷର>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  ତାର୍କିକ overflowed = ମିଥ୍ୟା;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  ତାର୍କିକ in_error () ଅଚଳ
  { ଫେରନ୍ତୁ hb_vector_t<ଅକ୍ଷର>::in_error () || overflowed; }

  ଶୂନ୍ୟ clear ()
  {
    overflowed = ମିଥ୍ୟା;
    hb_vector_t<ଅକ୍ଷର>::clear ();
  }

  unsigned scale_precision () ଅଚଳ
  { ଫେରନ୍ତୁ precision < 7 ? 7 : precision; }

  ତାର୍କିକ would_overflow (unsigned add)
  {
    ଯଦି (unlikely (in_error ()))
      ଫେରନ୍ତୁ ସତ୍ୟ;
    unsigned new_len;
    ଯଦି (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = ସତ୍ୟ;
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ତାର୍କିକ append_len (ଅଚଳ ଅକ୍ଷର *s, unsigned l)
  {
    ଯଦି (unlikely (would_overflow (l)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned old_len = length;
    unsigned new_len;
    ଯଦି (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) new_len)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_memcpy (arrayZ + old_len, s, l);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ append_c (ଅକ୍ଷର ch)
  {
    ଯଦି (unlikely (would_overflow (1)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ଠେଲ_or_fail (ch);
  }

  ତାର୍କିକ append_str (ଅଚଳ ଅକ୍ଷର *s)
  { ଫେରନ୍ତୁ append_len (s, (unsigned) strlen (s)); }

  ତାର୍କିକ append_unsigned (unsigned v)
  {
    ଅକ୍ଷର tmp[16];
    snprintf (tmp, ଆକାର (tmp), "%u", v);
    ଫେରନ୍ତୁ append_len (tmp, (unsigned) strlen (tmp));
  }

  ତାର୍କିକ append_hex_byte (unsigned v)
  {
    ଅକ୍ଷର tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    ଫେରନ୍ତୁ append_len (tmp, 2);
  }

  ଶୂନ୍ୟ append_ଅଙ୍କ (ଦଶମିକ v)
  { append_ଅଙ୍କ (v, precision); }

  ଶୂନ୍ୟ append_ଅଙ୍କ (ଦଶମିକ v, unsigned p)
  {
    ଯଦି (p > 12) p = 12;

    ଦଶମିକ rounded_zero_threshold = 0.5f;
    ପାଇଁ (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    ଯଦି (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    ଯଦି (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      ଫେରନ୍ତୁ;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    ଦ୍ୱିଗୁଣ scaled = (ଦ୍ୱିଗୁଣ) v * _hb_vector_buf_pow10[p];
    ଯଦି (likely (fabs (scaled) < 9.0e18))
    {
      ଦ୍ୱିଗୁଣ fl = floor (scaled);
      ଦ୍ୱିଗୁଣ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      ଯଦି (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      ଯେତେବେଳେ (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      ଅକ୍ଷର tmp[24];
      ଅକ୍ଷର *end = tmp + ଆକାର (tmp);
      ଅକ୍ଷର *s = end;
      unsigned d = 0;
      କର
      {
	*--s = (ଅକ୍ଷର) ('0' + (ଅକ୍ଷର) (u % 10));
	u /= 10;
	ଯଦି (++d == frac)
	  *--s = '.';
      }
      ଯେତେବେଳେ (u || d < frac + 1);
      ଯଦି (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      ଫେରନ୍ତୁ;
    }

    ଅକ୍ଷର fmt[6];
    snprintf (fmt, ଆକାର (fmt), "%%.%uf", p);
    ଅକ୍ଷର out[128];
    snprintf (out, ଆକାର (out), fmt, (ଦ୍ୱିଗୁଣ) v);

    ଅଚଳ ଅକ୍ଷର *decimal_point = hb_vector_decimal_point_get ();

    ଯଦି (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      ଅକ୍ଷର *dp = strstr (out, decimal_point);
      ଯଦି (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    ଅକ୍ଷର *dot = strchr (out, '.');
    ଯଦି (dot)
    {
      ଅକ୍ଷର *end = out + strlen (out) - 1;
      ଯେତେବେଳେ (end > dot && *end == '0')
	*end-- = '\0';
      ଯଦି (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  ଶୂନ୍ୟ append_svg_color (hb_color_t color, ତାର୍କିକ with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    ଯଦି (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    ଅନ୍ୟଥା
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    ଯଦି (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_ଅଙ୍କ (a / 255.f, 4);
    }
  }

  ତାର୍କିକ append_base64 (ଅଚଳ uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) new_len)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅକ୍ଷର *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    ଯେତେବେଳେ (i + 2 < len)
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

    ଯଦି (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      ଯଦି (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};

#endif /* HB_VECTOR_BUF_HH */
