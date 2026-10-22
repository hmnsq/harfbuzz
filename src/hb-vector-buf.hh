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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-limits.hh"
#अन्तर्भाव "hb-vector.hh"
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>

HB_INTERNAL अचर वर्ण *
hb_vector_decimal_point_get (शून्य);

स्थिर अचर द्विगुण _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

संरचना hb_vector_buf_t : hb_vector_t<वर्ण>
{
  अचिन्हित precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  तार्किक overflowed = असत्य;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  तार्किक in_error () अचर
  { फर्काऊ hb_vector_t<वर्ण>::in_error () || overflowed; }

  शून्य clear ()
  {
    overflowed = असत्य;
    hb_vector_t<वर्ण>::clear ();
  }

  अचिन्हित scale_precision () अचर
  { फर्काऊ precision < 7 ? 7 : precision; }

  तार्किक would_overflow (अचिन्हित add)
  {
    यदि (unlikely (in_error ()))
      फर्काऊ सत्य;
    अचिन्हित new_len;
    यदि (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = सत्य;
      फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
  }

  तार्किक append_len (अचर वर्ण *s, अचिन्हित l)
  {
    यदि (unlikely (would_overflow (l)))
      फर्काऊ असत्य;
    अचिन्हित old_len = length;
    अचिन्हित new_len;
    यदि (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((पूर्णाङ्क) new_len)))
      फर्काऊ असत्य;
    hb_memcpy (arrayZ + old_len, s, l);
    फर्काऊ सत्य;
  }

  तार्किक append_c (वर्ण ch)
  {
    यदि (unlikely (would_overflow (1)))
      फर्काऊ असत्य;
    फर्काऊ थप_or_fail (ch);
  }

  तार्किक append_str (अचर वर्ण *s)
  { फर्काऊ append_len (s, (अचिन्हित) म_माप (s)); }

  तार्किक append_unsigned (अचिन्हित v)
  {
    वर्ण tmp[16];
    snprintf (tmp, परिमाण (tmp), "%u", v);
    फर्काऊ append_len (tmp, (अचिन्हित) म_माप (tmp));
  }

  तार्किक append_hex_byte (अचिन्हित v)
  {
    वर्ण tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    फर्काऊ append_len (tmp, 2);
  }

  शून्य append_अङ्क (भिन्न v)
  { append_अङ्क (v, precision); }

  शून्य append_अङ्क (भिन्न v, अचिन्हित p)
  {
    यदि (p > 12) p = 12;

    भिन्न rounded_zero_threshold = 0.5f;
    लागि (अचिन्हित i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    यदि (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    यदि (!(v == v) || !std::है_सीमित (v))
    {
      append_c ('0');
      फर्काऊ;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    द्विगुण scaled = (द्विगुण) v * _hb_vector_buf_pow10[p];
    यदि (likely (भ_असल (scaled) < 9.0e18))
    {
      द्विगुण fl = न्यूनमान (scaled);
      द्विगुण diff = scaled - fl;
      int64_t n = (int64_t) fl;
      यदि (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      अचिन्हित frac = p;
      जबसम्म (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      वर्ण tmp[24];
      वर्ण *end = tmp + परिमाण (tmp);
      वर्ण *s = end;
      अचिन्हित d = 0;
      गर
      {
	*--s = (वर्ण) ('0' + (वर्ण) (u % 10));
	u /= 10;
	यदि (++d == frac)
	  *--s = '.';
      }
      जबसम्म (u || d < frac + 1);
      यदि (v < 0.f)
	*--s = '-';

      append_len (s, (अचिन्हित) (end - s));
      फर्काऊ;
    }

    वर्ण fmt[6];
    snprintf (fmt, परिमाण (fmt), "%%.%uf", p);
    वर्ण out[128];
    snprintf (out, परिमाण (out), fmt, (द्विगुण) v);

    अचर वर्ण *decimal_point = hb_vector_decimal_point_get ();

    यदि (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      वर्ण *dp = म_माला (out, decimal_point);
      यदि (dp)
      {
	अचिन्हित dp_len = (अचिन्हित) म_माप (decimal_point);
	अचिन्हित tail_len = (अचिन्हित) म_माप (dp + dp_len);
	स_हटाओ (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    वर्ण *dot = म_अक्षर (out, '.');
    यदि (dot)
    {
      वर्ण *end = out + म_माप (out) - 1;
      जबसम्म (end > dot && *end == '0')
	*end-- = '\0';
      यदि (end == dot)
	*end = '\0';
    }

    append_len (out, (अचिन्हित) म_माप (out));
  }

  शून्य append_svg_color (hb_color_t color, तार्किक with_alpha)
  {
    अचिन्हित r = hb_color_get_red (color);
    अचिन्हित g = hb_color_get_green (color);
    अचिन्हित b = hb_color_get_blue (color);
    अचिन्हित a = hb_color_get_alpha (color);
    append_c ('#');
    यदि (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    अन्यथा
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    यदि (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_अङ्क (a / 255.f, 4);
    }
  }

  तार्किक append_base64 (अचर uint8_t *data, अचिन्हित len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    अचिन्हित ngroups = len / 3 + (len % 3 != 0);
    अचिन्हित out_len;
    अचिन्हित old_len = length;
    अचिन्हित new_len;
    यदि (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((पूर्णाङ्क) new_len)))
      फर्काऊ असत्य;

    वर्ण *dst = arrayZ + old_len;
    अचिन्हित di = 0;
    अचिन्हित i = 0;
    जबसम्म (i + 2 < len)
    {
      अचिन्हित v = ((अचिन्हित) data[i] << 16) |
		   ((अचिन्हित) data[i + 1] << 8) |
		   ((अचिन्हित) data[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    यदि (i < len)
    {
      अचिन्हित v = (अचिन्हित) data[i] << 16;
      यदि (i + 1 < len)
	v |= (अचिन्हित) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    फर्काऊ सत्य;
  }
};

#endif /* HB_VECTOR_BUF_HH */
