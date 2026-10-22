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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-path.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
hb_ਸਦਿਸ਼_svg_extend_ਬਹੁਲਕ_str (hb_paint_extend_t ext)
{
  ਚੋਣ (ext)
  {
    ਹਾਲਤ HB_PAINT_EXTEND_PAD: ਵਾਪਸ "pad";
    ਹਾਲਤ HB_PAINT_EXTEND_REPEAT: ਵਾਪਸ "repeat";
    ਹਾਲਤ HB_PAINT_EXTEND_REFLECT: ਵਾਪਸ "reflect";
    ਮੂਲ: ਵਾਪਸ "pad";
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_emit_color_stops (hb_ਸਦਿਸ਼_paint_t *paint,
				hb_ਸਦਿਸ਼_buf_t *buf,
				hb_ਸਦਿਸ਼_t<hb_color_stop_t> *stops)
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < stops->ਲੰਮਾਈ; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_ਅੰਕ (stops->arrayZ[i].ਵਿੱਥ, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    ਜੇ (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_ਅੰਕ (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
hb_ਸਦਿਸ਼_svg_ਸੰਯੁਕਤ_ਬਹੁਲਕ_str (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  ਚੋਣ (ਬਹੁਲਕ)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS: ਵਾਪਸ "screen";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR: ਵਾਪਸ "difference";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP:
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP:
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER: ਵਾਪਸ "normal";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN: ਵਾਪਸ "screen";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY: ਵਾਪਸ "overlay";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN: ਵਾਪਸ "darken";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN: ਵਾਪਸ "lighten";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE: ਵਾਪਸ "color-dodge";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN: ਵਾਪਸ "color-burn";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT: ਵਾਪਸ "hard-light";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT: ਵਾਪਸ "soft-light";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE: ਵਾਪਸ "difference";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION: ਵਾਪਸ "exclusion";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY: ਵਾਪਸ "multiply";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE: ਵਾਪਸ "hue";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION: ਵਾਪਸ "saturation";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR: ਵਾਪਸ "color";
    ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY: ਵਾਪਸ "luminosity";
    ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}

ਬਣਤਰ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t { ਦਸ਼ਮਲਵ x, y; };
ਬਣਤਰ hb_ਸਦਿਸ਼_svg_rgba_t { ਦਸ਼ਮਲਵ r, g, b, a; };

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hb_ਸਦਿਸ਼_svg_lerp (ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b, ਦਸ਼ਮਲਵ t)
{ ਵਾਪਸ a + (b - a) * t; }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ
hb_ਸਦਿਸ਼_svg_clamp01 (ਦਸ਼ਮਲਵ v)
{
  ਜੇ (v < 0.f) ਵਾਪਸ 0.f;
  ਜੇ (v > 1.f) ਵਾਪਸ 1.f;
  ਵਾਪਸ v;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_rgba_t
hb_ਸਦਿਸ਼_svg_rgba_from_hb_color (hb_color_t c)
{
  ਵਾਪਸ {(ਦਸ਼ਮਲਵ) hb_color_get_red (c) / 255.f,
          (ਦਸ਼ਮਲਵ) hb_color_get_green (c) / 255.f,
          (ਦਸ਼ਮਲਵ) hb_color_get_blue (c) / 255.f,
          (ਦਸ਼ਮਲਵ) hb_color_get_alpha (c) / 255.f};
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_color_t
hb_ਸਦਿਸ਼_svg_hb_color_from_rgba (ਅਚਲ hb_ਸਦਿਸ਼_svg_rgba_t &c)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ r = (ਬਿਨਾਨਿਸ਼ਾਨੀ) roundf (hb_ਸਦਿਸ਼_svg_clamp01 (c.r) * 255.f);
  ਬਿਨਾਨਿਸ਼ਾਨੀ g = (ਬਿਨਾਨਿਸ਼ਾਨੀ) roundf (hb_ਸਦਿਸ਼_svg_clamp01 (c.g) * 255.f);
  ਬਿਨਾਨਿਸ਼ਾਨੀ b = (ਬਿਨਾਨਿਸ਼ਾਨੀ) roundf (hb_ਸਦਿਸ਼_svg_clamp01 (c.b) * 255.f);
  ਬਿਨਾਨਿਸ਼ਾਨੀ a = (ਬਿਨਾਨਿਸ਼ਾਨੀ) roundf (hb_ਸਦਿਸ਼_svg_clamp01 (c.a) * 255.f);
  ਵਾਪਸ HB_COLOR (b, g, r, a);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_rgba_t
hb_ਸਦਿਸ਼_svg_lerp_rgba (ਅਚਲ hb_ਸਦਿਸ਼_svg_rgba_t &c0,
			 ਅਚਲ hb_ਸਦਿਸ਼_svg_rgba_t &c1,
			 ਦਸ਼ਮਲਵ t)
{
  ਵਾਪਸ {hb_ਸਦਿਸ਼_svg_lerp (c0.r, c1.r, t),
          hb_ਸਦਿਸ਼_svg_lerp (c0.g, c1.g, t),
          hb_ਸਦਿਸ਼_svg_lerp (c0.b, c1.b, t),
          hb_ਸਦਿਸ਼_svg_lerp (c0.a, c1.a, t)};
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦਸ਼ਮਲਵ hb_ਸਦਿਸ਼_svg_dot (ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &p, ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &q) { ਵਾਪਸ p.x * q.x + p.y * q.y; }
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &p, ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &q) { ਵਾਪਸ {p.x + q.x, p.y + q.y}; }
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t hb_ਸਦਿਸ਼_svg_sub (ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &p, ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &q) { ਵਾਪਸ {p.x - q.x, p.y - q.y}; }
ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t hb_ਸਦਿਸ਼_svg_scale (ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &p, ਦਸ਼ਮਲਵ f) { ਵਾਪਸ {p.x * f, p.y * f}; }

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t
hb_ਸਦਿਸ਼_svg_normalize (ਅਚਲ hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t &p)
{
  ਦਸ਼ਮਲਵ len = sqrtf (hb_ਸਦਿਸ਼_svg_dot (p, p));
  ਜੇ (len == 0.f) ਵਾਪਸ {0.f, 0.f};
  ਵਾਪਸ hb_ਸਦਿਸ਼_svg_scale (p, 1.f / len);
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_ਜੋੜੋ_sweep_patch (hb_ਸਦਿਸ਼_buf_t *body,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ precision,
			       ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy, ਦਸ਼ਮਲਵ radius,
			       ਦਸ਼ਮਲਵ a0, ਅਚਲ hb_ਸਦਿਸ਼_svg_rgba_t &c0_ਵਿੱਚ,
			       ਦਸ਼ਮਲਵ a1, ਅਚਲ hb_ਸਦਿਸ਼_svg_rgba_t &c1_ਵਿੱਚ)
{
  ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ max_angle = HB_PI / 16.f;
  hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t center = {cx, cy};
  ਪੂਰਨ ਅੰਕ_splits = (ਪੂਰਨ) ceilf (fabsf (a1 - a0) / max_angle);
  ਜੇ (ਅੰਕ_splits < 1) ਅੰਕ_splits = 1;

  hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t p0 = {cosf (a0), sinf (a0)};
  hb_ਸਦਿਸ਼_svg_rgba_t color0 = c0_ਵਿੱਚ;

  ਲਈ (ਪੂਰਨ a = 0; a < ਅੰਕ_splits; a++)
  {
    ਦਸ਼ਮਲਵ k = (a + 1.f) / ਅੰਕ_splits;
    ਦਸ਼ਮਲਵ angle1 = hb_ਸਦਿਸ਼_svg_lerp (a0, a1, k);
    hb_ਸਦਿਸ਼_svg_rgba_t color1 = hb_ਸਦਿਸ਼_svg_lerp_rgba (c0_ਵਿੱਚ, c1_ਵਿੱਚ, k);

    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t p1 = {cosf (angle1), sinf (angle1)};
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t sp0 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (center, hb_ਸਦਿਸ਼_svg_scale (p0, radius));
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t sp1 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (center, hb_ਸਦਿਸ਼_svg_scale (p1, radius));

    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t A = hb_ਸਦਿਸ਼_svg_normalize (hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (p0, p1));
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t U = {-A.y, A.x};
    ਦਸ਼ਮਲਵ up0 = hb_ਸਦਿਸ਼_svg_dot (U, p0);
    ਦਸ਼ਮਲਵ up1 = hb_ਸਦਿਸ਼_svg_dot (U, p1);
    ਜੇ (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      ਜਾਰੀ;
    }
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t C0 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (A, hb_ਸਦਿਸ਼_svg_scale (U, hb_ਸਦਿਸ਼_svg_dot (hb_ਸਦਿਸ਼_svg_sub (p0, A), p0) / up0));
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t C1 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (A, hb_ਸਦਿਸ਼_svg_scale (U, hb_ਸਦਿਸ਼_svg_dot (hb_ਸਦਿਸ਼_svg_sub (p1, A), p1) / up1));

    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t sc0 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (center, hb_ਸਦਿਸ਼_svg_scale (hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (C0, hb_ਸਦਿਸ਼_svg_scale (hb_ਸਦਿਸ਼_svg_sub (C0, p0), 0.33333f)), radius));
    hb_ਸਦਿਸ਼_svg_ਬਿੰਦੂ_t sc1 = hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (center, hb_ਸਦਿਸ਼_svg_scale (hb_ਸਦਿਸ਼_svg_ਜੋੜੋ (C1, hb_ਸਦਿਸ਼_svg_scale (hb_ਸਦਿਸ਼_svg_sub (C1, p1), 0.33333f)), radius));

    hb_ਸਦਿਸ਼_svg_rgba_t mid_color = hb_ਸਦਿਸ਼_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_ਸਦਿਸ਼_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_ਅੰਕ (center.x, precision);
    body->append_c (',');
    body->append_ਅੰਕ (center.y, precision);
    body->append_str ("L");
    body->append_ਅੰਕ (sp0.x, precision);
    body->append_c (',');
    body->append_ਅੰਕ (sp0.y, precision);
    body->append_str ("C");
    body->append_ਅੰਕ (sc0.x, precision);
    body->append_c (',');
    body->append_ਅੰਕ (sc0.y, precision);
    body->append_c (' ');
    body->append_ਅੰਕ (sc1.x, precision);
    body->append_c (',');
    body->append_ਅੰਕ (sc1.y, precision);
    body->append_c (' ');
    body->append_ਅੰਕ (sp1.x, precision);
    body->append_c (',');
    body->append_ਅੰਕ (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, ਸੱਚ);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ਬਣਤਰ hb_ਸਦਿਸ਼_svg_sweep_ctx_t
{
  hb_ਸਦਿਸ਼_buf_t *body;
  ਬਿਨਾਨਿਸ਼ਾਨੀ precision;
  ਦਸ਼ਮਲਵ cx, cy, radius;
};

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_svg_sweep_emit_patch (ਦਸ਼ਮਲਵ a0, hb_color_t c0,
				ਦਸ਼ਮਲਵ a1, hb_color_t c1,
				ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *ctx = (hb_ਸਦਿਸ਼_svg_sweep_ctx_t *) user_ਅੰਕੜੇ;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  ਜੇ (unlikely (ctx->body->ਵਿੱਚ_error ()))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_svg_ਜੋੜੋ_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_ਸਦਿਸ਼_svg_rgba_from_hb_color (c0),
				 a1, hb_ਸਦਿਸ਼_svg_rgba_from_hb_color (c1));
}


ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *, ਖਾਲੀ *,
                                            ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ,
                                            ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *, ਖਾਲੀ *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_fill_glyph (hb_paint_funcs_t *, ਖਾਲੀ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *, ਖਾਲੀ *, hb_codepoint_t, hb_font_t *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *, ਖਾਲੀ *, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਖਾਲੀ *);
ਸਥਿਰ hb_draw_funcs_t * hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *, ਖਾਲੀ *, ਖਾਲੀ **, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *, ਖਾਲੀ *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *, ਖਾਲੀ *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_color (hb_paint_funcs_t *, ਖਾਲੀ *, hb_bool_t, hb_color_t, ਖਾਲੀ *);
ਸਥਿਰ hb_bool_t hb_ਸਦਿਸ਼_paint_image (hb_paint_funcs_t *, ਖਾਲੀ *, hb_blob_t *, ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_ਟੈਗ_t, ਦਸ਼ਮਲਵ, hb_glyph_extents_t *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *, ਖਾਲੀ *, hb_color_ਸਤਰ_t *, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_radial_gradient (hb_paint_funcs_t *, ਖਾਲੀ *, hb_color_ਸਤਰ_t *, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_sweep_gradient (hb_paint_funcs_t *, ਖਾਲੀ *, hb_color_ਸਤਰ_t *, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਦਸ਼ਮਲਵ, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *, ਖਾਲੀ *, ਖਾਲੀ *);
ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *, ਖਾਲੀ *, hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t, ਖਾਲੀ *);
ਸਥਿਰ hb_bool_t hb_ਸਦਿਸ਼_paint_color_glyph (hb_paint_funcs_t *, ਖਾਲੀ *, hb_codepoint_t, hb_font_t *, ਖਾਲੀ *);
ਸਥਿਰ hb_bool_t
hb_ਸਦਿਸ਼_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_ਅਣਵਰਤਿਆ,
				      ਖਾਲੀ *paint_ਅੰਕੜੇ,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ color_ਸੂਚਕਾਂਕ,
				      hb_color_t *color,
				      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_ਸਦਿਸ਼_paint_t *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (!color)
    ਵਾਪਸ ਝੂਠ;

  hb_color_t *value = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!paint->custom_palette_colors.has (color_ਸੂਚਕਾਂਕ, &value) || !value)
    ਵਾਪਸ ਝੂਠ;
  *color = *value;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_paint_funcs ();
ਸਥਿਰ ਬਣਤਰ hb_ਸਦਿਸ਼_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_ਸਦਿਸ਼_paint_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func (funcs, (hb_paint_ਧੱਕੋ_transform_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func (funcs, (hb_paint_ਕੱਢੋ_transform_func_t) hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_ਸਦਿਸ਼_paint_fill_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, (hb_paint_ਧੱਕੋ_clip_glyph_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, (hb_paint_ਧੱਕੋ_clip_rectangle_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func (funcs, (hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func (funcs, (hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, (hb_paint_ਕੱਢੋ_clip_func_t) hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, (hb_paint_color_func_t) hb_ਸਦਿਸ਼_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, (hb_paint_image_func_t) hb_ਸਦਿਸ਼_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, (hb_paint_ਰੇਖਿਕ_gradient_func_t) hb_ਸਦਿਸ਼_paint_ਰੇਖਿਕ_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_ਸਦਿਸ਼_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_ਸਦਿਸ਼_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, (hb_paint_ਧੱਕੋ_ਸਮੂਹ_func_t) hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, (hb_paint_ਕੱਢੋ_ਸਮੂਹ_func_t) hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_ਸਦਿਸ਼_paint_color_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_ਸਦਿਸ਼_paint_custom_palette_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_budget_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_ਸਦਿਸ਼_paint_get_budget_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_paint_get_budget_ਬਚਿਆ_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_ਸਦਿਸ਼_paint_funcs);
    ਵਾਪਸ funcs;
  }
} static_ਸਦਿਸ਼_paint_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_ਸਦਿਸ਼_paint_funcs ()
{
  static_ਸਦਿਸ਼_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_ਸਦਿਸ਼_paint_svg_funcs_get ()
{
  ਵਾਪਸ static_ਸਦਿਸ਼_paint_funcs.get_unconst ();
}


ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *,
                                ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                                ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                                ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
                                ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਜੇ (unlikely (paint->transform_ਸਮੂਹ_ਉਛਾਲ_ਡੂੰਘਾਈ))
  {
    paint->transform_ਸਮੂਹ_ਉਛਾਲ_ਡੂੰਘਾਈ++;
    ਵਾਪਸ;
  }
  ਜੇ (unlikely (paint->transform_ਸਮੂਹ_ਡੂੰਘਾਈ >= 64))
  {
    paint->transform_ਸਮੂਹ_ਉਛਾਲ_ਡੂੰਘਾਈ = 1;
    ਵਾਪਸ;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_ਸਮੂਹ_open_ਨਕਾਬ = (paint->transform_ਸਮੂਹ_open_ਨਕਾਬ << 1) | (opened ? 1ull : 0ull);
  paint->transform_ਸਮੂਹ_ਡੂੰਘਾਈ++;

  ਜੇ (!opened)
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_ਅੰਕ (xx, sprec);
  body.append_c (',');
  body.append_ਅੰਕ (yx, sprec);
  body.append_c (',');
  body.append_ਅੰਕ (xy, sprec);
  body.append_c (',');
  body.append_ਅੰਕ (yy, sprec);
  body.append_c (',');
  body.append_ਅੰਕ (paint->sx (dx));
  body.append_c (',');
  body.append_ਅੰਕ (paint->sy (dy));
  body.append_str (")\">\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *,
                               ਖਾਲੀ *paint_ਅੰਕੜੇ,
                               ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਜੇ (unlikely (paint->transform_ਸਮੂਹ_ਉਛਾਲ_ਡੂੰਘਾਈ))
  {
    paint->transform_ਸਮੂਹ_ਉਛਾਲ_ਡੂੰਘਾਈ--;
    ਵਾਪਸ;
  }
  ਜੇ (!paint->transform_ਸਮੂਹ_ਡੂੰਘਾਈ)
    ਵਾਪਸ;
  paint->transform_ਸਮੂਹ_ਡੂੰਘਾਈ--;
  hb_bool_t opened = !!(paint->transform_ਸਮੂਹ_open_ਨਕਾਬ & 1ull);
  paint->transform_ਸਮੂਹ_open_ਨਕਾਬ >>= 1;
  ਜੇ (opened)
    paint->current_body ().append_str ("</g>\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_fill_glyph (hb_paint_funcs_t *,
                            ਖਾਲੀ *paint_ਅੰਕੜੇ,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  paint->ਰਸਤਾ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  ਜੇ (likely (paint->budget_ਬਚਿਆ >= 0))
  {
    hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t sink = {&paint->ਰਸਤਾ, paint->get_precision (),
				 paint->x_scale_ਗੁਣਕ, paint->y_scale_ਗੁਣਕ,
				 &paint->budget_ਬਚਿਆ};
    hb_font_draw_glyph (font, glyph, hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get (), &sink);
  }

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->ਰਸਤਾ.arrayZ, paint->ਰਸਤਾ.ਲੰਮਾਈ);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, ਸੱਚ);
  body.append_str ("\"/>\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *,
                                 ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਅਚਲ ਅੱਖਰ *pfx = paint->id_ਅਗੇਤਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pfx_len = paint->id_ਅਗੇਤਰ_ਲੰਮਾਈ;

  paint->ਰਸਤਾ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  ਜੇ (likely (paint->budget_ਬਚਿਆ >= 0))
  {
    hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t sink = {&paint->ਰਸਤਾ, paint->get_precision (),
				 paint->x_scale_ਗੁਣਕ, paint->y_scale_ਗੁਣਕ,
				 &paint->budget_ਬਚਿਆ};
    hb_font_draw_glyph (font, glyph, hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get (), &sink);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ def_id = paint->ਰਸਤਾ_def_ਗਿਣਤੀ++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->ਰਸਤਾ.arrayZ, paint->ਰਸਤਾ.ਲੰਮਾਈ);
  paint->defs.append_str ("\"/>\n");
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("clip-p");
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\"><use href=\"#");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("clip-p");
  paint->current_body ().append_unsigned (def_id);
  paint->current_body ().append_str (")\">\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *,
                                     ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                     ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin,
                                     ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
                                     ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਅਚਲ ਅੱਖਰ *pfx = paint->id_ਅਗੇਤਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pfx_len = paint->id_ਅਗੇਤਰ_ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_ਅੰਕ (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_ਅੰਕ (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_ਅੰਕ (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_ਅੰਕ (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ਸਥਿਰ hb_draw_funcs_t *
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *,
                                      ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                      ਖਾਲੀ **draw_ਅੰਕੜੇ,
                                      ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  paint->ਰਸਤਾ.clear ();
  paint->clip_ਰਸਤਾ_sink = {&paint->ਰਸਤਾ, paint->get_precision (),
			   paint->x_scale_ਗੁਣਕ,
			   paint->y_scale_ਗੁਣਕ,
			   &paint->budget_ਬਚਿਆ};
  *draw_ਅੰਕੜੇ = &paint->clip_ਰਸਤਾ_sink;
  ਵਾਪਸ hb_ਸਦਿਸ਼_svg_ਰਸਤਾ_draw_funcs_get ();
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *,
                                    ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                    ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਅਚਲ ਅੱਖਰ *pfx = paint->id_ਅਗੇਤਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pfx_len = paint->id_ਅਗੇਤਰ_ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ clip_id = paint->clip_ਰਸਤਾ_counter++;

  /* The accumulated path is in font Y-up coords (the
   * convention used inside per-glyph <use scale(_,-sy)>
   * wrappers); this clip is emitted at base body level
   * (free-form between glyphs), so flip its geometry inside
   * the clipPath via the path's own transform attribute,
   * keeping the body's <g clip-path> at body Y-down. */
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("cp");
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><path transform=\"scale(1,-1)\" d=\"");
  paint->defs.append_len (paint->ਰਸਤਾ.arrayZ, paint->ਰਸਤਾ.ਲੰਮਾਈ);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *,
                          ਖਾਲੀ *paint_ਅੰਕੜੇ,
                          ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("</g>\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_color (hb_paint_funcs_t *,
                       ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       hb_bool_t,
                       hb_color_t color,
                       ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, ਸੱਚ);
  body.append_str ("\"/>\n");
}

ਸਥਿਰ hb_bool_t
hb_ਸਦਿਸ਼_paint_image (hb_paint_funcs_t *,
                       ਖਾਲੀ *paint_ਅੰਕੜੇ,
                       hb_blob_t *image,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
                       hb_ਟੈਗ_t format,
                       ਦਸ਼ਮਲਵ slant HB_ਅਣਵਰਤਿਆ,
                       hb_glyph_extents_t *extents,
                       ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ ਝੂਠ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  ਜੇ (format == HB_ਟੈਗ ('p','n','g',' '))
  {
    ਜੇ (!extents || !ਚੌੜਾਈ || !ਉਚਾਈ)
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
    ਅਚਲ ਅੱਖਰ *png_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (image, &len);
    ਜੇ (!png_ਅੰਕੜੇ || !len)
      ਵਾਪਸ ਝੂਠ;

    body.append_str ("<g transform=\"translate(");
    body.append_ਅੰਕ (paint->sx ((ਦਸ਼ਮਲਵ) extents->x_bearing));
    body.append_c (',');
    body.append_ਅੰਕ (paint->sy ((ਦਸ਼ਮਲਵ) extents->y_bearing));
    body.append_str (") scale(");
    body.append_ਅੰਕ (paint->sx ((ਦਸ਼ਮਲਵ) extents->ਚੌੜਾਈ) / ਚੌੜਾਈ);
    body.append_c (',');
    body.append_ਅੰਕ (paint->sy ((ਦਸ਼ਮਲਵ) extents->ਉਚਾਈ) / ਉਚਾਈ);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ਅਚਲ uint8_t *) png_ਅੰਕੜੇ, len);
    body.append_str ("\" width=\"");
    body.append_ਅੰਕ ((ਦਸ਼ਮਲਵ) ਚੌੜਾਈ);
    body.append_str ("\" height=\"");
    body.append_ਅੰਕ ((ਦਸ਼ਮਲਵ) ਉਚਾਈ);
    body.append_str ("\"/>\n</g>\n");

    ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *,
                                 ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                 hb_color_ਸਤਰ_t *color_ਸਤਰ,
                                 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
                                 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
                                 ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
                                 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops.arrayZ, stops.ਲੰਮਾਈ, &mn, &mx);

  ਅਚਲ ਅੱਖਰ *pfx = paint->id_ਅਗੇਤਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pfx_len = paint->id_ਅਗੇਤਰ_ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  ਦਸ਼ਮਲਵ lx0, ly0, lx1, ly1;
  hb_paint_reduce_ਰੇਖਿਕ_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  ਦਸ਼ਮਲਵ gx0 = lx0 + mn * (lx1 - lx0);
  ਦਸ਼ਮਲਵ gy0 = ly0 + mn * (ly1 - ly0);
  ਦਸ਼ਮਲਵ gx1 = lx0 + mx * (lx1 - lx0);
  ਦਸ਼ਮਲਵ gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_ਅੰਕ (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_ਅੰਕ (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_ਅੰਕ (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_ਅੰਕ (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_ਸਦਿਸ਼_svg_extend_ਬਹੁਲਕ_str (hb_color_ਸਤਰ_get_extend (color_ਸਤਰ)));
  paint->defs.append_str ("\">\n");
  hb_ਸਦਿਸ਼_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_radial_gradient (hb_paint_funcs_t *,
                                 ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                 hb_color_ਸਤਰ_t *color_ਸਤਰ,
                                 ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
                                 ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
                                 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops.arrayZ, stops.ਲੰਮਾਈ, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  ਦਸ਼ਮਲਵ gx0 = x0 + mn * (x1 - x0);
  ਦਸ਼ਮਲਵ gy0 = y0 + mn * (y1 - y0);
  ਦਸ਼ਮਲਵ gr0 = r0 + mn * (r1 - r0);
  ਦਸ਼ਮਲਵ gx1 = x0 + mx * (x1 - x0);
  ਦਸ਼ਮਲਵ gy1 = y0 + mx * (y1 - y0);
  ਦਸ਼ਮਲਵ gr1 = r0 + mx * (r1 - r0);

  ਅਚਲ ਅੱਖਰ *pfx = paint->id_ਅਗੇਤਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ pfx_len = paint->id_ਅਗੇਤਰ_ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_ਅੰਕ (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_ਅੰਕ (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_ਅੰਕ (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_ਅੰਕ (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_ਅੰਕ (paint->sy (gy0));
  ਜੇ (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_ਅੰਕ (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_ਸਦਿਸ਼_svg_extend_ਬਹੁਲਕ_str (hb_color_ਸਤਰ_get_extend (color_ਸਤਰ)));
  paint->defs.append_str ("\">\n");
  hb_ਸਦਿਸ਼_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_sweep_gradient (hb_paint_funcs_t *,
                                ਖਾਲੀ *paint_ਅੰਕੜੇ,
                                hb_color_ਸਤਰ_t *color_ਸਤਰ,
                                ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
                                ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle, ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
                                ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops.arrayZ, stops.ਲੰਮਾਈ, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  ਦਸ਼ਮਲਵ ga0 = ਸ਼ੁਰੂ_angle + mn * (ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle);
  ਦਸ਼ਮਲਵ ga1 = ਸ਼ੁਰੂ_angle + mx * (ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle);

  hb_ਸਦਿਸ਼_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.ਲੰਮਾਈ,
				 hb_color_ਸਤਰ_get_extend (color_ਸਤਰ),
				 ga0, ga1,
				 hb_ਸਦਿਸ਼_svg_sweep_emit_patch,
				 &ctx);
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *,
                            ਖਾਲੀ *paint_ਅੰਕੜੇ,
                            ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਜੇ (unlikely (!paint->ਸਮੂਹ_ਢੇਰ.ਧੱਕੋ_or_fail (hb_ਸਦਿਸ਼_buf_t {})))
    ਵਾਪਸ;
}

ਸਥਿਰ ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *,
                           ਖਾਲੀ *paint_ਅੰਕੜੇ,
                           hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
                           ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਜੇ (paint->ਸਮੂਹ_ਢੇਰ.ਲੰਮਾਈ < 2)
    ਵਾਪਸ;

  hb_ਸਦਿਸ਼_buf_t ਸਮੂਹ = paint->ਸਮੂਹ_ਢੇਰ.ਕੱਢੋ ();
  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਅਚਲ ਅੱਖਰ *blend = hb_ਸਦਿਸ਼_svg_ਸੰਯੁਕਤ_ਬਹੁਲਕ_str (ਬਹੁਲਕ);
  ਜੇ (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (ਸਮੂਹ.arrayZ, ਸਮੂਹ.ਲੰਮਾਈ);
    body.append_str ("</g>\n");
  }
  ਨਹੀਂਤਾਂ
    body.append_len (ਸਮੂਹ.arrayZ, ਸਮੂਹ.ਲੰਮਾਈ);
}

ਸਥਿਰ hb_bool_t
hb_ਸਦਿਸ਼_paint_color_glyph (hb_paint_funcs_t *,
                             ਖਾਲੀ *paint_ਅੰਕੜੇ,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (paint->color_glyph_ਡੂੰਘਾਈ >= HB_MAX_NESTING_LEVEL))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (unlikely (hb_ਸੈੱਟ_has (paint->ਸਰਗਰਮ_color_glyphs, glyph)))
    ਵਾਪਸ ਝੂਠ;

  paint->color_glyph_ਡੂੰਘਾਈ++;
  hb_ਸੈੱਟ_ਜੋੜੋ (paint->ਸਰਗਰਮ_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_ਸਦਿਸ਼_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_ਸੈੱਟ_del (paint->ਸਰਗਰਮ_color_glyphs, glyph);
  paint->color_glyph_ਡੂੰਘਾਈ--;
  ਵਾਪਸ ਸੱਚ;
}




hb_blob_t *
hb_ਸਦਿਸ਼_paint_render_svg (hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਜੇ (!paint->has_extents)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_ਸਦਿਸ਼_buf_t ਬਾਹਰਲਾ;
  hb_buf_recover_recycled (paint->recycled_blob, &ਬਾਹਰਲਾ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ estimated = paint->defs.ਲੰਮਾਈ +
		       paint->ਸਮੂਹ_ਢੇਰ.arrayZ[0].ਲੰਮਾਈ +
		       320;
  ਬਾਹਰਲਾ.alloc (estimated);
  ਦਸ਼ਮਲਵ vb_x = paint->extents.x;
  ਦਸ਼ਮਲਵ vb_y = -(paint->extents.y + paint->extents.ਉਚਾਈ);
  ਦਸ਼ਮਲਵ vb_w = paint->extents.ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ vb_h = paint->extents.ਉਚਾਈ;
  ਬਾਹਰਲਾ.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_x);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_y);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
  ਬਾਹਰਲਾ.append_str ("\" width=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
  ਬਾਹਰਲਾ.append_str ("\" height=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
  ਬਾਹਰਲਾ.append_str ("\">\n");

  ਜੇ (paint->defs.ਲੰਮਾਈ)
  {
    ਬਾਹਰਲਾ.append_str ("<defs>\n");
    ਬਾਹਰਲਾ.append_len (paint->defs.arrayZ, paint->defs.ਲੰਮਾਈ);
    ਬਾਹਰਲਾ.append_str ("</defs>\n");
  }

  ਜੇ (hb_color_get_alpha (paint->background))
  {
    ਬਾਹਰਲਾ.append_str ("<rect x=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_x);
    ਬਾਹਰਲਾ.append_str ("\" y=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_y);
    ਬਾਹਰਲਾ.append_str ("\" width=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
    ਬਾਹਰਲਾ.append_str ("\" height=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
    ਬਾਹਰਲਾ.append_str ("\" fill=\"rgb(");
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_red (paint->background));
    ਬਾਹਰਲਾ.append_c (',');
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_green (paint->background));
    ਬਾਹਰਲਾ.append_c (',');
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_blue (paint->background));
    ਬਾਹਰਲਾ.append_str (")\"");
    ਜੇ (hb_color_get_alpha (paint->background) < 255)
    {
      ਬਾਹਰਲਾ.append_str (" fill-opacity=\"");
      ਬਾਹਰਲਾ.append_ਅੰਕ (hb_color_get_alpha (paint->background) / 255.f, 4);
      ਬਾਹਰਲਾ.append_c ('"');
    }
    ਬਾਹਰਲਾ.append_str ("/>\n");
  }

  ਬਾਹਰਲਾ.append_str ("<g transform=\"scale(1,-1)\">\n");
  ਬਾਹਰਲਾ.append_len (paint->ਸਮੂਹ_ਢੇਰ.arrayZ[0].arrayZ, paint->ਸਮੂਹ_ਢੇਰ.arrayZ[0].ਲੰਮਾਈ);
  ਬਾਹਰਲਾ.append_str ("</g>\n");

  ਬਾਹਰਲਾ.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &ਬਾਹਰਲਾ);

  ਵਾਪਸ blob;
}
