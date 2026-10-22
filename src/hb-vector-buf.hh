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

HB_INTERNAL स्थिरांक अक्षर *
hb_vector_decimal_point_get (शून्य);

स्थिर स्थिरांक द्विगुण _hb_vector_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

संरचना hb_vector_buf_t : hb_vector_t<अक्षर>
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
  तार्किक in_error () स्थिरांक
  { परत hb_vector_t<अक्षर>::in_error () || overflowed; }

  शून्य clear ()
  {
    overflowed = असत्य;
    hb_vector_t<अक्षर>::clear ();
  }

  अचिन्हित scale_precision () स्थिरांक
  { परत precision < 7 ? 7 : precision; }

  तार्किक would_overflow (अचिन्हित add)
  {
    जर (unlikely (in_error ()))
      परत सत्य;
    अचिन्हित new_len;
    जर (unlikely (hb_unsigned_add_overflows (length, add, &new_len) ||
		  new_len > HB_VECTOR_MAX_DOCUMENT_SIZE))
    {
      overflowed = सत्य;
      परत सत्य;
    }
    परत असत्य;
  }

  तार्किक append_len (स्थिरांक अक्षर *s, अचिन्हित l)
  {
    जर (unlikely (would_overflow (l)))
      परत असत्य;
    अचिन्हित old_len = length;
    अचिन्हित new_len;
    जर (unlikely (hb_unsigned_add_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((पूर्णांक) new_len)))
      परत असत्य;
    hb_memcpy (arrayZ + old_len, s, l);
    परत सत्य;
  }

  तार्किक append_c (अक्षर ch)
  {
    जर (unlikely (would_overflow (1)))
      परत असत्य;
    परत ढकला_or_fail (ch);
  }

  तार्किक append_str (स्थिरांक अक्षर *s)
  { परत append_len (s, (अचिन्हित) म_माप (s)); }

  तार्किक append_unsigned (अचिन्हित v)
  {
    अक्षर tmp[16];
    snprintf (tmp, आकार (tmp), "%u", v);
    परत append_len (tmp, (अचिन्हित) म_माप (tmp));
  }

  तार्किक append_hex_byte (अचिन्हित v)
  {
    अक्षर tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    परत append_len (tmp, 2);
  }

  शून्य append_अंक (दशांश v)
  { append_अंक (v, precision); }

  शून्य append_अंक (दशांश v, अचिन्हित p)
  {
    जर (p > 12) p = 12;

    दशांश rounded_zero_threshold = 0.5f;
    साठी (अचिन्हित i = 0; i < p; i++)
      rounded_zero_threshold *= 0.1f;
    जर (fabsf (v) < rounded_zero_threshold)
      v = 0.f;

    जर (!(v == v) || !std::है_सीमित (v))
    {
      append_c ('0');
      परत;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    द्विगुण scaled = (द्विगुण) v * _hb_vector_buf_pow10[p];
    जर (likely (भ_असल (scaled) < 9.0e18))
    {
      द्विगुण fl = न्यूनमान (scaled);
      द्विगुण diff = scaled - fl;
      int64_t n = (int64_t) fl;
      जर (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      अचिन्हित frac = p;
      पर्यंत (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      अक्षर tmp[24];
      अक्षर *end = tmp + आकार (tmp);
      अक्षर *s = end;
      अचिन्हित d = 0;
      करा
      {
	*--s = (अक्षर) ('0' + (अक्षर) (u % 10));
	u /= 10;
	जर (++d == frac)
	  *--s = '.';
      }
      पर्यंत (u || d < frac + 1);
      जर (v < 0.f)
	*--s = '-';

      append_len (s, (अचिन्हित) (end - s));
      परत;
    }

    अक्षर fmt[6];
    snprintf (fmt, आकार (fmt), "%%.%uf", p);
    अक्षर out[128];
    snprintf (out, आकार (out), fmt, (द्विगुण) v);

    स्थिरांक अक्षर *decimal_point = hb_vector_decimal_point_get ();

    जर (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      अक्षर *dp = म_माला (out, decimal_point);
      जर (dp)
      {
	अचिन्हित dp_len = (अचिन्हित) म_माप (decimal_point);
	अचिन्हित tail_len = (अचिन्हित) म_माप (dp + dp_len);
	स_हटाओ (dp + 1, dp + dp_len, tail_len + 1);
	*dp = '.';
      }
    }

    अक्षर *dot = म_अक्षर (out, '.');
    जर (dot)
    {
      अक्षर *end = out + म_माप (out) - 1;
      पर्यंत (end > dot && *end == '0')
	*end-- = '\0';
      जर (end == dot)
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
    जर (((r >> 4) == (r & 0xF)) &&
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
    जर (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_अंक (a / 255.f, 4);
    }
  }

  तार्किक append_base64 (स्थिरांक uint8_t *data, अचिन्हित len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    अचिन्हित ngroups = len / 3 + (len % 3 != 0);
    अचिन्हित out_len;
    अचिन्हित old_len = length;
    अचिन्हित new_len;
    जर (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &out_len) ||
		  would_overflow (out_len) ||
		  hb_unsigned_add_overflows (old_len, out_len, &new_len) ||
		  !resize_dirty ((पूर्णांक) new_len)))
      परत असत्य;

    अक्षर *dst = arrayZ + old_len;
    अचिन्हित di = 0;
    अचिन्हित i = 0;
    पर्यंत (i + 2 < len)
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

    जर (i < len)
    {
      अचिन्हित v = (अचिन्हित) data[i] << 16;
      जर (i + 1 < len)
	v |= (अचिन्हित) data[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    परत सत्य;
  }
};

#endif /* HB_VECTOR_BUF_HH */
