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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-limits.hh"
#અન્તર્ભાવ "hb-vector.hh"
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>

HB_INTERNAL અચલ અક્ષર *
hb_vector_decimal_point_get (શૂન્ય);

સ્થિર અચલ દ્વિગુણ _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

સંરચના hb_vector_buf_t : hb_vector_t<અક્ષર>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  તાર્કિક overflowed = ખોટું;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  તાર્કિક in_error () અચલ
  { પાછા hb_vector_t<અક્ષર>::in_error () || overflowed; }

  શૂન્ય clear ()
  {
    overflowed = ખોટું;
    hb_vector_t<અક્ષર>::clear ();
  }

  unsigned scale_precision () અચલ
  { પાછા precision < 7 ? 7 : precision; }

  તાર્કિક would_overflow (unsigned add)
  {
    જો (unlikely (in_error ()))
      પાછા સાચું;
    unsigned new_len;
    જો (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = સાચું;
      પાછા સાચું;
    }
    પાછા ખોટું;
  }

  તાર્કિક append_len (અચલ અક્ષર *s, unsigned l)
  {
    જો (unlikely (would_overflow (l)))
      પાછા ખોટું;
    unsigned old_len = length;
    unsigned new_len;
    જો (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((પૂર્ણાંક) new_len)))
      પાછા ખોટું;
    hb_memcpy (arrayZ + old_len, s, l);
    પાછા સાચું;
  }

  તાર્કિક append_c (અક્ષર ch)
  {
    જો (unlikely (would_overflow (1)))
      પાછા ખોટું;
    પાછા ધકેલો_or_fail (ch);
  }

  તાર્કિક append_str (અચલ અક્ષર *s)
  { પાછા append_len (s, (unsigned) strlen (s)); }

  તાર્કિક append_unsigned (unsigned v)
  {
    અક્ષર tmp[16];
    snprintf (tmp, કદ (tmp), "%u", v);
    પાછા append_len (tmp, (unsigned) strlen (tmp));
  }

  તાર્કિક append_hex_byte (unsigned v)
  {
    અક્ષર tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    પાછા append_len (tmp, 2);
  }

  શૂન્ય append_અંક (દશાંશ v)
  { append_અંક (v, precision); }

  શૂન્ય append_અંક (દશાંશ v, unsigned p)
  {
    જો (p > 12) p = 12;

    દશાંશ rounded_zero_threshold = 0.5f;
    માટે (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    જો (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    જો (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      પાછા;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    દ્વિગુણ scaled = (દ્વિગુણ) v * _hb_vector_buf_pow10[p];
    જો (likely (fabs (scaled) < 9.0e18))
    {
      દ્વિગુણ fl = floor (scaled);
      દ્વિગુણ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      જો (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      જ્યારે (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      અક્ષર tmp[24];
      અક્ષર *end = tmp + કદ (tmp);
      અક્ષર *s = end;
      unsigned d = 0;
      કરો
      {
	*--s = (અક્ષર) ('0' + (અક્ષર) (u % 10));
	u /= 10;
	જો (++d == frac)
	  *--s = '.';
      }
      જ્યારે (u || d < frac + 1);
      જો (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      પાછા;
    }

    અક્ષર fmt[6];
    snprintf (fmt, કદ (fmt), "%%.%uf", p);
    અક્ષર out[128];
    snprintf (out, કદ (out), fmt, (દ્વિગુણ) v);

    અચલ અક્ષર *decimal_point = hb_vector_decimal_point_get ();

    જો (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      અક્ષર *dp = strstr (out, decimal_point);
      જો (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    અક્ષર *dot = strchr (out, '.');
    જો (dot)
    {
      અક્ષર *end = out + strlen (out) - 1;
      જ્યારે (end > dot && *end == '0')
	*end-- = '\0';
      જો (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  શૂન્ય append_svg_color (hb_color_t color, તાર્કિક with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    જો (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    નહીંતર
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    જો (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_અંક (a / 255.f, 4);
    }
  }

  તાર્કિક append_base64 (અચલ uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    જો (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((પૂર્ણાંક) new_len)))
      પાછા ખોટું;

    અક્ષર *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    જ્યારે (i + 2 < len)
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

    જો (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      જો (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    પાછા સાચું;
  }
};

#endif /* HB_VECTOR_BUF_HH */
