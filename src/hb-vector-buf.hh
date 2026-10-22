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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸਦਿਸ਼_BUF_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸਦਿਸ਼_BUF_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-limits.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

HB_ਅੰਦਰੂਨੀ ਅਚਲ ਅੱਖਰ *
hb_ਸਦਿਸ਼_decimal_ਬਿੰਦੂ_get (ਖਾਲੀ);

ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ hb_ਸਦਿਸ਼_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

ਬਣਤਰ hb_ਸਦਿਸ਼_buf_t : hb_ਸਦਿਸ਼_t<ਅੱਖਰ>
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  ਤਾਰਕਿਕ overflowed = ਝੂਠ;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  { ਵਾਪਸ hb_ਸਦਿਸ਼_t<ਅੱਖਰ>::ਵਿੱਚ_error () || overflowed; }

  ਖਾਲੀ clear ()
  {
    overflowed = ਝੂਠ;
    hb_ਸਦਿਸ਼_t<ਅੱਖਰ>::clear ();
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ scale_precision () ਅਚਲ
  { ਵਾਪਸ precision < 7 ? 7 : precision; }

  ਤਾਰਕਿਕ would_ਉਛਾਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ)
  {
    ਜੇ (unlikely (ਵਿੱਚ_error ()))
      ਵਾਪਸ ਸੱਚ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_len;
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਲੰਮਾਈ, ਜੋੜੋ, &new_len) ||
		  new_len > HB_ਸਦਿਸ਼_MAX_DOCUMENT_ਮਾਪ))
    {
      overflowed = ਸੱਚ;
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਤਾਰਕਿਕ append_len (ਅਚਲ ਅੱਖਰ *s, ਬਿਨਾਨਿਸ਼ਾਨੀ l)
  {
    ਜੇ (unlikely (would_ਉਛਾਲ (l)))
      ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_len = ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_len;
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((ਪੂਰਨ) new_len)))
      ਵਾਪਸ ਝੂਠ;
    hb_memcpy (arrayZ + old_len, s, l);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ append_c (ਅੱਖਰ ch)
  {
    ਜੇ (unlikely (would_ਉਛਾਲ (1)))
      ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ਧੱਕੋ_or_fail (ch);
  }

  ਤਾਰਕਿਕ append_str (ਅਚਲ ਅੱਖਰ *s)
  { ਵਾਪਸ append_len (s, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (s)); }

  ਤਾਰਕਿਕ append_unsigned (ਬਿਨਾਨਿਸ਼ਾਨੀ v)
  {
    ਅੱਖਰ tmp[16];
    snprintf (tmp, ਆਕਾਰ (tmp), "%u", v);
    ਵਾਪਸ append_len (tmp, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (tmp));
  }

  ਤਾਰਕਿਕ append_hex_byte (ਬਿਨਾਨਿਸ਼ਾਨੀ v)
  {
    ਅੱਖਰ tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    ਵਾਪਸ append_len (tmp, 2);
  }

  ਖਾਲੀ append_ਅੰਕ (ਦਸ਼ਮਲਵ v)
  { append_ਅੰਕ (v, precision); }

  ਖਾਲੀ append_ਅੰਕ (ਦਸ਼ਮਲਵ v, ਬਿਨਾਨਿਸ਼ਾਨੀ p)
  {
    ਜੇ (p > 12) p = 12;

    ਦਸ਼ਮਲਵ rounded_zero_ਹੱਦ = 0.5f;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < p; i++)
      rounded_zero_ਹੱਦ *= 0.1f;
    ਜੇ (fabsf (v) < rounded_zero_ਹੱਦ)
      v = 0.f;

    ਜੇ (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      ਵਾਪਸ;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    ਦੁੱਗਣਾ scaled = (ਦੁੱਗਣਾ) v * hb_ਸਦਿਸ਼_buf_pow10[p];
    ਜੇ (likely (fabs (scaled) < 9.0e18))
    {
      ਦੁੱਗਣਾ fl = floor (scaled);
      ਦੁੱਗਣਾ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      ਜੇ (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      ਬਿਨਾਨਿਸ਼ਾਨੀ frac = p;
      ਜਦੋਂ (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      ਅੱਖਰ tmp[24];
      ਅੱਖਰ *ਸਿਰਾ = tmp + ਆਕਾਰ (tmp);
      ਅੱਖਰ *s = ਸਿਰਾ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ d = 0;
      ਕਰੋ
      {
	*--s = (ਅੱਖਰ) ('0' + (ਅੱਖਰ) (u % 10));
	u /= 10;
	ਜੇ (++d == frac)
	  *--s = '.';
      }
      ਜਦੋਂ (u || d < frac + 1);
      ਜੇ (v < 0.f)
	*--s = '-';

      append_len (s, (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਸਿਰਾ - s));
      ਵਾਪਸ;
    }

    ਅੱਖਰ fmt[6];
    snprintf (fmt, ਆਕਾਰ (fmt), "%%.%uf", p);
    ਅੱਖਰ ਬਾਹਰਲਾ[128];
    snprintf (ਬਾਹਰਲਾ, ਆਕਾਰ (ਬਾਹਰਲਾ), fmt, (ਦੁੱਗਣਾ) v);

    ਅਚਲ ਅੱਖਰ *decimal_point = hb_ਸਦਿਸ਼_decimal_ਬਿੰਦੂ_get ();

    ਜੇ (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      ਅੱਖਰ *dp = ਲੜੀ_ਖੋਜ (ਬਾਹਰਲਾ, decimal_point);
      ਜੇ (dp)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ dp_len = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (decimal_point);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਛ_len = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (dp + dp_len);
	memmove (dp + 1, dp + dp_len, ਪੂਛ_len + 1);
	*dp = '.';
      }
    }

    ਅੱਖਰ *dot = strchr (ਬਾਹਰਲਾ, '.');
    ਜੇ (dot)
    {
      ਅੱਖਰ *ਸਿਰਾ = ਬਾਹਰਲਾ + ਲੜੀ_ਲੰਬਾਈ (ਬਾਹਰਲਾ) - 1;
      ਜਦੋਂ (ਸਿਰਾ > dot && *ਸਿਰਾ == '0')
	*ਸਿਰਾ-- = '\0';
      ਜੇ (ਸਿਰਾ == dot)
	*ਸਿਰਾ = '\0';
    }

    append_len (ਬਾਹਰਲਾ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (ਬਾਹਰਲਾ));
  }

  ਖਾਲੀ append_svg_color (hb_color_t color, ਤਾਰਕਿਕ with_alpha)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ r = hb_color_get_red (color);
    ਬਿਨਾਨਿਸ਼ਾਨੀ g = hb_color_get_green (color);
    ਬਿਨਾਨਿਸ਼ਾਨੀ b = hb_color_get_blue (color);
    ਬਿਨਾਨਿਸ਼ਾਨੀ a = hb_color_get_alpha (color);
    append_c ('#');
    ਜੇ (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    ਨਹੀਂਤਾਂ
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    ਜੇ (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_ਅੰਕ (a / 255.f, 4);
    }
  }

  ਤਾਰਕਿਕ append_base64 (ਅਚਲ uint8_t *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ngroups = len / 3 + (len % 3 != 0);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਾਹਰਲਾ_len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_len = ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_len;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &ਬਾਹਰਲਾ_len) ||
		  would_ਉਛਾਲ (ਬਾਹਰਲਾ_len) ||
		  hb_unsigned_ਜੋੜੋ_overflows (old_len, ਬਾਹਰਲਾ_len, &new_len) ||
		  !resize_dirty ((ਪੂਰਨ) new_len)))
      ਵਾਪਸ ਝੂਠ;

    ਅੱਖਰ *dst = arrayZ + old_len;
    ਬਿਨਾਨਿਸ਼ਾਨੀ di = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਜਦੋਂ (i + 2 < len)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ v = ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ[i] << 16) |
		   ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ[i + 1] << 8) |
		   ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    ਜੇ (i < len)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ v = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ[i] << 16;
      ਜੇ (i + 1 < len)
	v |= (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕੜੇ[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    ਵਾਪਸ ਸੱਚ;
  }
};

#ਜੇ_ਅੰਤ /* HB_VECTOR_BUF_HH */
