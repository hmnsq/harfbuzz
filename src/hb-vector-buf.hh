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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-limits.hh"
#அந்தர்பாவ "hb-vector.hh"
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>

HB_INTERNAL மாறிலி எழுத்து *
hb_vector_decimal_point_get (வெறுமை);

நிலையான மாறிலி இரட்டை _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

கட்டமைப்பு hb_vector_buf_t : hb_vector_t<எழுத்து>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  தருக்க overflowed = பொய்;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  தருக்க in_error () மாறிலி
  { திரும்பு hb_vector_t<எழுத்து>::in_error () || overflowed; }

  வெறுமை clear ()
  {
    overflowed = பொய்;
    hb_vector_t<எழுத்து>::clear ();
  }

  unsigned scale_precision () மாறிலி
  { திரும்பு precision < 7 ? 7 : precision; }

  தருக்க would_overflow (unsigned add)
  {
    எனில் (unlikely (in_error ()))
      திரும்பு உண்மை;
    unsigned new_len;
    எனில் (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = உண்மை;
      திரும்பு உண்மை;
    }
    திரும்பு பொய்;
  }

  தருக்க append_len (மாறிலி எழுத்து *s, unsigned l)
  {
    எனில் (unlikely (would_overflow (l)))
      திரும்பு பொய்;
    unsigned old_len = length;
    unsigned new_len;
    எனில் (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((முழு) new_len)))
      திரும்பு பொய்;
    hb_memcpy (arrayZ + old_len, s, l);
    திரும்பு உண்மை;
  }

  தருக்க append_c (எழுத்து ch)
  {
    எனில் (unlikely (would_overflow (1)))
      திரும்பு பொய்;
    திரும்பு தள்ளு_or_fail (ch);
  }

  தருக்க append_str (மாறிலி எழுத்து *s)
  { திரும்பு append_len (s, (unsigned) strlen (s)); }

  தருக்க append_unsigned (unsigned v)
  {
    எழுத்து tmp[16];
    snprintf (tmp, அளவு (tmp), "%u", v);
    திரும்பு append_len (tmp, (unsigned) strlen (tmp));
  }

  தருக்க append_hex_byte (unsigned v)
  {
    எழுத்து tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    திரும்பு append_len (tmp, 2);
  }

  வெறுமை append_இலக்கம் (தசம v)
  { append_இலக்கம் (v, precision); }

  வெறுமை append_இலக்கம் (தசம v, unsigned p)
  {
    எனில் (p > 12) p = 12;

    தசம rounded_zero_threshold = 0.5f;
    க்கு (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    எனில் (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    எனில் (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      திரும்பு;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    இரட்டை scaled = (இரட்டை) v * _hb_vector_buf_pow10[p];
    எனில் (likely (fabs (scaled) < 9.0e18))
    {
      இரட்டை fl = floor (scaled);
      இரட்டை diff = scaled - fl;
      int64_t n = (int64_t) fl;
      எனில் (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      போது (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      எழுத்து tmp[24];
      எழுத்து *end = tmp + அளவு (tmp);
      எழுத்து *s = end;
      unsigned d = 0;
      செய்
      {
	*--s = (எழுத்து) ('0' + (எழுத்து) (u % 10));
	u /= 10;
	எனில் (++d == frac)
	  *--s = '.';
      }
      போது (u || d < frac + 1);
      எனில் (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      திரும்பு;
    }

    எழுத்து fmt[6];
    snprintf (fmt, அளவு (fmt), "%%.%uf", p);
    எழுத்து out[128];
    snprintf (out, அளவு (out), fmt, (இரட்டை) v);

    மாறிலி எழுத்து *decimal_point = hb_vector_decimal_point_get ();

    எனில் (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      எழுத்து *dp = strstr (out, decimal_point);
      எனில் (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    எழுத்து *dot = strchr (out, '.');
    எனில் (dot)
    {
      எழுத்து *end = out + strlen (out) - 1;
      போது (end > dot && *end == '0')
	*end-- = '\0';
      எனில் (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  வெறுமை append_svg_color (hb_color_t color, தருக்க with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    எனில் (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    இல்லையெனில்
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    எனில் (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_இலக்கம் (a / 255.f, 4);
    }
  }

  தருக்க append_base64 (மாறிலி uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    எனில் (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((முழு) new_len)))
      திரும்பு பொய்;

    எழுத்து *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    போது (i + 2 < len)
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

    எனில் (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      எனில் (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    திரும்பு உண்மை;
  }
};

#endif /* HB_VECTOR_BUF_HH */
