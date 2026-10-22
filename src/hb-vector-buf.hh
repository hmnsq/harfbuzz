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

#నిర్వచితం_కాకపోతే HB_సదిశ_BUF_HH
#నిర్వచించు HB_సదిశ_BUF_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-limits.hh"
#అన్తర్భావ "hb-vector.hh"
#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>

HB_అంతర్గత స్థిరరాశి అక్షరం *
hb_సదిశ_decimal_బిందువు_get (శూన్య);

స్థిర స్థిరరాశి ద్విగుణ hb_సదిశ_buf_pow10[13] = {1., 1e1, 1e2, 1e3, 1e4, 1e5, 1e6,
						1e7, 1e8, 1e9, 1e10, 1e11, 1e12};

నిర్మాణం hb_సదిశ_buf_t : hb_సదిశ_t<అక్షరం>
{
  గుర్తులేని precision = 2;

  /* Sticky flag set once the serialized document would exceed
   * HB_VECTOR_MAX_DOCUMENT_SIZE.  Folded into in_error() so every emission
   * site that already checks it -- path commands, gradient meshes -- stops
   * cleanly.  Bounds output that the outline work budget cannot see, most
   * importantly sweep-gradient meshes and embedded bitmaps. */
  తార్కిక overflowed = అబద్ధం;

  /* Hides (does not override -- the base method is not virtual) the base
   * in_error(); every emission site uses hb_vector_buf_t directly, so the
   * cap is seen everywhere.  clear() resets the sticky flag so a context
   * reused across renders recovers after one over-cap glyph. */
  తార్కిక లోపల_error () స్థిరరాశి
  { తిరిగి hb_సదిశ_t<అక్షరం>::లోపల_error () || overflowed; }

  శూన్య clear ()
  {
    overflowed = అబద్ధం;
    hb_సదిశ_t<అక్షరం>::clear ();
  }

  గుర్తులేని scale_precision () స్థిరరాశి
  { తిరిగి precision < 7 ? 7 : precision; }

  తార్కిక would_అధికప్రవాహం (గుర్తులేని కూడు)
  {
    ఒకవేళ (unlikely (లోపల_error ()))
      తిరిగి నిజం;
    గుర్తులేని new_len;
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (నిడివి, కూడు, &new_len) ||
		  new_len > HB_సదిశ_MAX_DOCUMENT_కొలత))
    {
      overflowed = నిజం;
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  తార్కిక append_len (స్థిరరాశి అక్షరం *s, గుర్తులేని l)
  {
    ఒకవేళ (unlikely (would_అధికప్రవాహం (l)))
      తిరిగి అబద్ధం;
    గుర్తులేని old_len = నిడివి;
    గుర్తులేని new_len;
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (old_len, l, &new_len) ||
		  !resize_dirty ((పూర్ణాంకం) new_len)))
      తిరిగి అబద్ధం;
    hb_memcpy (arrayZ + old_len, s, l);
    తిరిగి నిజం;
  }

  తార్కిక append_c (అక్షరం ch)
  {
    ఒకవేళ (unlikely (would_అధికప్రవాహం (1)))
      తిరిగి అబద్ధం;
    తిరిగి నెట్టు_or_fail (ch);
  }

  తార్కిక append_str (స్థిరరాశి అక్షరం *s)
  { తిరిగి append_len (s, (గుర్తులేని) అక్షరశ్రేణి_పొడవు (s)); }

  తార్కిక append_unsigned (గుర్తులేని v)
  {
    అక్షరం tmp[16];
    snprintf (tmp, పరిమాణం (tmp), "%u", v);
    తిరిగి append_len (tmp, (గుర్తులేని) అక్షరశ్రేణి_పొడవు (tmp));
  }

  తార్కిక append_hex_byte (గుర్తులేని v)
  {
    అక్షరం tmp[2] = {"0123456789ABCDEF"[(v >> 4) & 15],
		   "0123456789ABCDEF"[v & 15]};
    తిరిగి append_len (tmp, 2);
  }

  శూన్య append_అంకె (దశాంశం v)
  { append_అంకె (v, precision); }

  శూన్య append_అంకె (దశాంశం v, గుర్తులేని p)
  {
    ఒకవేళ (p > 12) p = 12;

    దశాంశం rounded_zero_పరిమితి = 0.5f;
    కొరకు (గుర్తులేని i = 0; i < p; i++)
      rounded_zero_పరిమితి *= 0.1f;
    ఒకవేళ (fabsf (v) < rounded_zero_పరిమితి)
      v = 0.f;

    ఒకవేళ (!(v == v) || !std::isfinite (v))
    {
      append_c ('0');
      తిరిగి;
    }

    /* Fast path: fixed-point decimal via integer math.  @v has a
     * 24-bit significand and 5^12 has 28 bits, so v*10^p is exact in
     * a double for p <= 12; rounding it half-to-even therefore
     * matches snprintf "%.pf" output digit-for-digit, while skipping
     * the (much slower) printf floating-point formatting and any
     * locale decimal-point fix-up. */
    ద్విగుణ scaled = (ద్విగుణ) v * hb_సదిశ_buf_pow10[p];
    ఒకవేళ (likely (fabs (scaled) < 9.0e18))
    {
      ద్విగుణ fl = floor (scaled);
      ద్విగుణ diff = scaled - fl;
      int64_t n = (int64_t) fl;
      ఒకవేళ (diff > 0.5 || (diff == 0.5 && (n & 1)))
	n++;

      uint64_t u = (uint64_t) (n < 0 ? -n : n);
      గుర్తులేని frac = p;
      అయితే (frac && !(u % 10))
      {
	u /= 10;
	frac--;
      }

      అక్షరం tmp[24];
      అక్షరం *చివర = tmp + పరిమాణం (tmp);
      అక్షరం *s = చివర;
      గుర్తులేని d = 0;
      చేయి
      {
	*--s = (అక్షరం) ('0' + (అక్షరం) (u % 10));
	u /= 10;
	ఒకవేళ (++d == frac)
	  *--s = '.';
      }
      అయితే (u || d < frac + 1);
      ఒకవేళ (v < 0.f)
	*--s = '-';

      append_len (s, (గుర్తులేని) (చివర - s));
      తిరిగి;
    }

    అక్షరం fmt[6];
    snprintf (fmt, పరిమాణం (fmt), "%%.%uf", p);
    అక్షరం బహిర్గతం[128];
    snprintf (బహిర్గతం, పరిమాణం (బహిర్గతం), fmt, (ద్విగుణ) v);

    స్థిరరాశి అక్షరం *decimal_point = hb_సదిశ_decimal_బిందువు_get ();

    ఒకవేళ (decimal_point[0] != '.' || decimal_point[1] != '\0')
    {
      అక్షరం *dp = అక్షరశ్రేణి_శోధన (బహిర్గతం, decimal_point);
      ఒకవేళ (dp)
      {
	గుర్తులేని dp_len = (గుర్తులేని) అక్షరశ్రేణి_పొడవు (decimal_point);
	గుర్తులేని తోక_len = (గుర్తులేని) అక్షరశ్రేణి_పొడవు (dp + dp_len);
	memmove (dp + 1, dp + dp_len, తోక_len + 1);
	*dp = '.';
      }
    }

    అక్షరం *dot = strchr (బహిర్గతం, '.');
    ఒకవేళ (dot)
    {
      అక్షరం *చివర = బహిర్గతం + అక్షరశ్రేణి_పొడవు (బహిర్గతం) - 1;
      అయితే (చివర > dot && *చివర == '0')
	*చివర-- = '\0';
      ఒకవేళ (చివర == dot)
	*చివర = '\0';
    }

    append_len (బహిర్గతం, (గుర్తులేని) అక్షరశ్రేణి_పొడవు (బహిర్గతం));
  }

  శూన్య append_svg_color (hb_color_t color, తార్కిక with_alpha)
  {
    గుర్తులేని r = hb_color_get_red (color);
    గుర్తులేని g = hb_color_get_green (color);
    గుర్తులేని b = hb_color_get_blue (color);
    గుర్తులేని a = hb_color_get_alpha (color);
    append_c ('#');
    ఒకవేళ (((r >> 4) == (r & 0xF)) &&
	((g >> 4) == (g & 0xF)) &&
	((b >> 4) == (b & 0xF)))
    {
      append_c ("0123456789ABCDEF"[r & 0xF]);
      append_c ("0123456789ABCDEF"[g & 0xF]);
      append_c ("0123456789ABCDEF"[b & 0xF]);
    }
    లేకపోతే
    {
      append_hex_byte (r);
      append_hex_byte (g);
      append_hex_byte (b);
    }
    ఒకవేళ (with_alpha && a != 255)
    {
      append_str ("\" fill-opacity=\"");
      append_అంకె (a / 255.f, 4);
    }
  }

  తార్కిక append_base64 (స్థిరరాశి uint8_t *దత్తాంశం, గుర్తులేని len)
  {
    /* Output is 4 bytes per 3 input bytes; compute without overflowing. */
    గుర్తులేని ngroups = len / 3 + (len % 3 != 0);
    గుర్తులేని బహిర్గతం_len;
    గుర్తులేని old_len = నిడివి;
    గుర్తులేని new_len;
    ఒకవేళ (unlikely (hb_unsigned_mul_overflows (ngroups, 4, &బహిర్గతం_len) ||
		  would_అధికప్రవాహం (బహిర్గతం_len) ||
		  hb_unsigned_కూడు_overflows (old_len, బహిర్గతం_len, &new_len) ||
		  !resize_dirty ((పూర్ణాంకం) new_len)))
      తిరిగి అబద్ధం;

    అక్షరం *dst = arrayZ + old_len;
    గుర్తులేని di = 0;
    గుర్తులేని i = 0;
    అయితే (i + 2 < len)
    {
      గుర్తులేని v = ((గుర్తులేని) దత్తాంశం[i] << 16) |
		   ((గుర్తులేని) దత్తాంశం[i + 1] << 8) |
		   ((గుర్తులేని) దత్తాంశం[i + 2]);
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[v & 63];
      i += 3;
    }

    ఒకవేళ (i < len)
    {
      గుర్తులేని v = (గుర్తులేని) దత్తాంశం[i] << 16;
      ఒకవేళ (i + 1 < len)
	v |= (గుర్తులేని) దత్తాంశం[i + 1] << 8;
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 18) & 63];
      dst[di++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 12) & 63];
      dst[di++] = (i + 1 < len) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(v >> 6) & 63] : '=';
      dst[di++] = '=';
    }

    తిరిగి నిజం;
  }
};

#అయితే_ముగింపు /* HB_VECTOR_BUF_HH */
