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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-limits.hh"
#അന്തര്ഭാവ "hb-vector.hh"
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>

HB_INTERNAL സ്ഥിരം അക്ഷരം *
hb_vector_decimal_point_get (ശൂന്യം);

സ്ഥിതം സ്ഥിരം ഇരട്ട _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

ഘടന hb_vector_buf_t : hb_vector_t<അക്ഷരം>
{
  unsigned precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  തർക്കം overflowed = അസത്യം;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  തർക്കം in_error () സ്ഥിരം
  { തിരികെ hb_vector_t<അക്ഷരം>::in_error () || overflowed; }

  ശൂന്യം clear ()
  {
    overflowed = അസത്യം;
    hb_vector_t<അക്ഷരം>::clear ();
  }

  unsigned scale_precision () സ്ഥിരം
  { തിരികെ precision < 7 ? 7 : precision; }

  തർക്കം would_overflow (unsigned add)
  {
    എങ്കിൽ (unlikely (in_error ()))
      തിരികെ സത്യം;
    unsigned new_len;
    എങ്കിൽ (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = സത്യം;
      തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
  }

  തർക്കം append_len (സ്ഥിരം അക്ഷരം *s, unsigned l)
  {
    എങ്കിൽ (unlikely (would_overflow (l)))
      തിരികെ അസത്യം;
    unsigned old_len = length;
    unsigned new_len;
    എങ്കിൽ (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((പൂർണ്ണസംഖ്യ) new_len)))
      തിരികെ അസത്യം;
    hb_memcpy (arrayZ + old_len, s, l);
    തിരികെ സത്യം;
  }

  തർക്കം append_c (അക്ഷരം ch)
  {
    എങ്കിൽ (unlikely (would_overflow (1)))
      തിരികെ അസത്യം;
    തിരികെ തള്ളുക_or_fail (ch);
  }

  തർക്കം append_str (സ്ഥിരം അക്ഷരം *s)
  { തിരികെ append_len (s, (unsigned) strlen (s)); }

  തർക്കം append_unsigned (unsigned v)
  {
    അക്ഷരം tmp[16];
    snprintf (tmp, വലുപ്പം (tmp), "%u", v);
    തിരികെ append_len (tmp, (unsigned) strlen (tmp));
  }

  തർക്കം append_hex_byte (unsigned v)
  {
    അക്ഷരം tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    തിരികെ append_len (tmp, 2);
  }

  ശൂന്യം append_അക്കം (ദശാംശം v)
  { append_അക്കം (v, precision); }

  ശൂന്യം append_അക്കം (ദശാംശം v, unsigned p)
  {
    എങ്കിൽ (p > 12) p = 12;

    ദശാംശം rounded_zero_threshold = 0.5f;
    വേണ്ടി (unsigned i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    എങ്കിൽ (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    എങ്കിൽ (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      തിരികെ;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    ഇരട്ട scaled = (ഇരട്ട) v * _hb_vector_buf_pow10[p];
    എങ്കിൽ (likely (fabs (scaled) < 9.0e18))
    {
      ഇരട്ട fl = floor (scaled);
      ഇരട്ട diff = scaled - fl;
      int64_t n = (int64_t) fl;
      എങ്കിൽ (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      unsigned frac = p;
      ആകുമ്പോൾ (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      അക്ഷരം tmp[24];
      അക്ഷരം *end = tmp + വലുപ്പം (tmp);
      അക്ഷരം *s = end;
      unsigned d = 0;
      ചെയ്യുക
      {
	*--s = (അക്ഷരം) ('0' + (അക്ഷരം) (u % 10));
	u /= 10;
	എങ്കിൽ (++d == frac)
	  *--s = '.';
      }
      ആകുമ്പോൾ (u || d < frac + 1);
      എങ്കിൽ (v < 0.f)
	*--s = '-';

      append_len (s, (unsigned) (end - s));
      തിരികെ;
    }

    അക്ഷരം fmt[6];
    snprintf (fmt, വലുപ്പം (fmt), "%%.%uf", p);
    അക്ഷരം out[128];
    snprintf (out, വലുപ്പം (out), fmt, (ഇരട്ട) v);

    സ്ഥിരം അക്ഷരം *decimal_point = hb_vector_decimal_point_get ();

    എങ്കിൽ (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      അക്ഷരം *dp = strstr (out, decimal_point);
      എങ്കിൽ (dp)
      {
	unsigned dp_len = (unsigned) strlen (decimal_point);
	unsigned tail_len = (unsigned) strlen (dp + dp_len);
	memmove (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    അക്ഷരം *dot = strchr (out, '.');
    എങ്കിൽ (dot)
    {
      അക്ഷരം *end = out + strlen (out) - 1;
      ആകുമ്പോൾ (end > dot && *end == '0')
	*end-- = '\0';
      എങ്കിൽ (end == dot)
	*end = '\0';
    }

    append_len (out, (unsigned) strlen (out));
  }

  ശൂന്യം append_svg_color (hb_color_t color, തർക്കം with_alpha)
  {
    unsigned r = hb_color_get_red (color);
    unsigned g = hb_color_get_green (color);
    unsigned b = hb_color_get_blue (color);
    unsigned a = hb_color_get_alpha (color);
    append_c ('#');
    എങ്കിൽ (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    അല്ലെങ്കിൽ
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    എങ്കിൽ (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_അക്കം (a / 255.f, 4);
    }
  }

  തർക്കം append_base64 (സ്ഥിരം uint8_t *data, unsigned len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    unsigned ngroups = len / 3 + (len % 3 != 0);
    unsigned out_len;
    unsigned old_len = length;
    unsigned new_len;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((പൂർണ്ണസംഖ്യ) new_len)))
      തിരികെ അസത്യം;

    അക്ഷരം *dst = arrayZ + old_len;
    unsigned di = 0;
    unsigned i = 0;
    ആകുമ്പോൾ (i + 2 < len)
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

    എങ്കിൽ (i < len)
    {
      unsigned v = (unsigned) data[i] << 16;
      എങ്കിൽ (i + 1 < len)
	v |= (unsigned) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    തിരികെ സത്യം;
  }
};

#endif /* HB_VECTOR_BUF_HH */
