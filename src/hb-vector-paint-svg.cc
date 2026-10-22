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

#ضمن "hb.hh"

#ضمن "hb-vector-paint.hh"
#ضمن "hb-paint.hh"
#ضمن "hb-vector-path.hh"

#ضمن <math.h>

ساكن ثابت حرف *
hb_وکتور_svg_extend_نما_str (hb_paint_extend_t ext)
{
  بدل (ext)
  {
    حالة HB_PAINT_EXTEND_PAD: أرجع "pad";
    حالة HB_PAINT_EXTEND_REPEAT: أرجع "repeat";
    حالة HB_PAINT_EXTEND_REFLECT: أرجع "reflect";
    افتراضي: أرجع "pad";
  }
}

ساكن فراغ
hb_وکتور_svg_emit_color_stops (hb_وکتور_paint_t *paint,
				hb_وکتور_buf_t *buf,
				hb_وکتور_t<hb_color_stop_t> *stops)
{
  لكل (بیعلامت i = 0; i < stops->درازی; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_رقم (stops->arrayZ[i].جابه_جایی, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    إذا (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_رقم (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

ساكن ثابت حرف *
hb_وکتور_svg_مرکب_نما_str (hb_paint_مرکب_نما_t نما)
{
  بدل (نما)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    حالة HB_PAINT_مرکب_نما_PLUS: أرجع "screen";
    حالة HB_PAINT_مرکب_نما_XOR: أرجع "difference";
    حالة HB_PAINT_مرکب_نما_CLEAR:
    حالة HB_PAINT_مرکب_نما_SRC:
    حالة HB_PAINT_مرکب_نما_DEST:
    حالة HB_PAINT_مرکب_نما_DEST_OVER:
    حالة HB_PAINT_مرکب_نما_SRC_درون:
    حالة HB_PAINT_مرکب_نما_DEST_درون:
    حالة HB_PAINT_مرکب_نما_SRC_بیرون:
    حالة HB_PAINT_مرکب_نما_DEST_بیرون:
    حالة HB_PAINT_مرکب_نما_SRC_ATOP:
    حالة HB_PAINT_مرکب_نما_DEST_ATOP:
      أرجع اشاره_تهی;
    حالة HB_PAINT_مرکب_نما_SRC_OVER: أرجع "normal";
    حالة HB_PAINT_مرکب_نما_SCREEN: أرجع "screen";
    حالة HB_PAINT_مرکب_نما_OVERLAY: أرجع "overlay";
    حالة HB_PAINT_مرکب_نما_DARKEN: أرجع "darken";
    حالة HB_PAINT_مرکب_نما_LIGHTEN: أرجع "lighten";
    حالة HB_PAINT_مرکب_نما_COLOR_DODGE: أرجع "color-dodge";
    حالة HB_PAINT_مرکب_نما_COLOR_BURN: أرجع "color-burn";
    حالة HB_PAINT_مرکب_نما_HARD_LIGHT: أرجع "hard-light";
    حالة HB_PAINT_مرکب_نما_SOFT_LIGHT: أرجع "soft-light";
    حالة HB_PAINT_مرکب_نما_DIFFERENCE: أرجع "difference";
    حالة HB_PAINT_مرکب_نما_EXCLUSION: أرجع "exclusion";
    حالة HB_PAINT_مرکب_نما_MULTIPLY: أرجع "multiply";
    حالة HB_PAINT_مرکب_نما_HSL_HUE: أرجع "hue";
    حالة HB_PAINT_مرکب_نما_HSL_SATURATION: أرجع "saturation";
    حالة HB_PAINT_مرکب_نما_HSL_COLOR: أرجع "color";
    حالة HB_PAINT_مرکب_نما_HSL_LUMINOSITY: أرجع "luminosity";
    افتراضي: أرجع اشاره_تهی;
  }
}

بنية hb_وکتور_svg_نقطه_t { عائم x, y; };
بنية hb_وکتور_svg_rgba_t { عائم r, g, b, a; };

ساكن مضمن عائم
hb_وکتور_svg_lerp (عائم a, عائم b, عائم t)
{ أرجع a + (b - a) * t; }

ساكن مضمن عائم
hb_وکتور_svg_clamp01 (عائم v)
{
  إذا (v < 0.f) أرجع 0.f;
  إذا (v > 1.f) أرجع 1.f;
  أرجع v;
}

ساكن مضمن hb_وکتور_svg_rgba_t
hb_وکتور_svg_rgba_from_hb_color (hb_color_t c)
{
  أرجع {(عائم) hb_color_get_red (c) / 255.f,
          (عائم) hb_color_get_green (c) / 255.f,
          (عائم) hb_color_get_blue (c) / 255.f,
          (عائم) hb_color_get_alpha (c) / 255.f};
}

ساكن مضمن hb_color_t
hb_وکتور_svg_hb_color_from_rgba (ثابت hb_وکتور_svg_rgba_t &c)
{
  بیعلامت r = (بیعلامت) roundf (hb_وکتور_svg_clamp01 (c.r) * 255.f);
  بیعلامت g = (بیعلامت) roundf (hb_وکتور_svg_clamp01 (c.g) * 255.f);
  بیعلامت b = (بیعلامت) roundf (hb_وکتور_svg_clamp01 (c.b) * 255.f);
  بیعلامت a = (بیعلامت) roundf (hb_وکتور_svg_clamp01 (c.a) * 255.f);
  أرجع HB_COLOR (b, g, r, a);
}

ساكن مضمن hb_وکتور_svg_rgba_t
hb_وکتور_svg_lerp_rgba (ثابت hb_وکتور_svg_rgba_t &c0,
			 ثابت hb_وکتور_svg_rgba_t &c1,
			 عائم t)
{
  أرجع {hb_وکتور_svg_lerp (c0.r, c1.r, t),
          hb_وکتور_svg_lerp (c0.g, c1.g, t),
          hb_وکتور_svg_lerp (c0.b, c1.b, t),
          hb_وکتور_svg_lerp (c0.a, c1.a, t)};
}

ساكن مضمن عائم hb_وکتور_svg_dot (ثابت hb_وکتور_svg_نقطه_t &p, ثابت hb_وکتور_svg_نقطه_t &q) { أرجع p.x * q.x + p.y * q.y; }
ساكن مضمن hb_وکتور_svg_نقطه_t hb_وکتور_svg_جمع_کن (ثابت hb_وکتور_svg_نقطه_t &p, ثابت hb_وکتور_svg_نقطه_t &q) { أرجع {p.x + q.x, p.y + q.y}; }
ساكن مضمن hb_وکتور_svg_نقطه_t hb_وکتور_svg_sub (ثابت hb_وکتور_svg_نقطه_t &p, ثابت hb_وکتور_svg_نقطه_t &q) { أرجع {p.x - q.x, p.y - q.y}; }
ساكن مضمن hb_وکتور_svg_نقطه_t hb_وکتور_svg_scale (ثابت hb_وکتور_svg_نقطه_t &p, عائم f) { أرجع {p.x * f, p.y * f}; }

ساكن مضمن hb_وکتور_svg_نقطه_t
hb_وکتور_svg_normalize (ثابت hb_وکتور_svg_نقطه_t &p)
{
  عائم len = sqrtf (hb_وکتور_svg_dot (p, p));
  إذا (len == 0.f) أرجع {0.f, 0.f};
  أرجع hb_وکتور_svg_scale (p, 1.f / len);
}

ساكن فراغ
hb_وکتور_svg_جمع_کن_sweep_patch (hb_وکتور_buf_t *body,
			       بیعلامت precision,
			       عائم cx, عائم cy, عائم radius,
			       عائم a0, ثابت hb_وکتور_svg_rgba_t &c0_درون,
			       عائم a1, ثابت hb_وکتور_svg_rgba_t &c1_درون)
{
  ساكن ثابت عائم max_angle = HB_PI / 16.f;
  hb_وکتور_svg_نقطه_t center = {cx, cy};
  صحيح رقم_splits = (صحيح) ceilf (fabsf (a1 - a0) / max_angle);
  إذا (رقم_splits < 1) رقم_splits = 1;

  hb_وکتور_svg_نقطه_t p0 = {cosf (a0), sinf (a0)};
  hb_وکتور_svg_rgba_t color0 = c0_درون;

  لكل (صحيح a = 0; a < رقم_splits; a++)
  {
    عائم k = (a + 1.f) / رقم_splits;
    عائم angle1 = hb_وکتور_svg_lerp (a0, a1, k);
    hb_وکتور_svg_rgba_t color1 = hb_وکتور_svg_lerp_rgba (c0_درون, c1_درون, k);

    hb_وکتور_svg_نقطه_t p1 = {cosf (angle1), sinf (angle1)};
    hb_وکتور_svg_نقطه_t sp0 = hb_وکتور_svg_جمع_کن (center, hb_وکتور_svg_scale (p0, radius));
    hb_وکتور_svg_نقطه_t sp1 = hb_وکتور_svg_جمع_کن (center, hb_وکتور_svg_scale (p1, radius));

    hb_وکتور_svg_نقطه_t A = hb_وکتور_svg_normalize (hb_وکتور_svg_جمع_کن (p0, p1));
    hb_وکتور_svg_نقطه_t U = {-A.y, A.x};
    عائم up0 = hb_وکتور_svg_dot (U, p0);
    عائم up1 = hb_وکتور_svg_dot (U, p1);
    إذا (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      استمر;
    }
    hb_وکتور_svg_نقطه_t C0 = hb_وکتور_svg_جمع_کن (A, hb_وکتور_svg_scale (U, hb_وکتور_svg_dot (hb_وکتور_svg_sub (p0, A), p0) / up0));
    hb_وکتور_svg_نقطه_t C1 = hb_وکتور_svg_جمع_کن (A, hb_وکتور_svg_scale (U, hb_وکتور_svg_dot (hb_وکتور_svg_sub (p1, A), p1) / up1));

    hb_وکتور_svg_نقطه_t sc0 = hb_وکتور_svg_جمع_کن (center, hb_وکتور_svg_scale (hb_وکتور_svg_جمع_کن (C0, hb_وکتور_svg_scale (hb_وکتور_svg_sub (C0, p0), 0.33333f)), radius));
    hb_وکتور_svg_نقطه_t sc1 = hb_وکتور_svg_جمع_کن (center, hb_وکتور_svg_scale (hb_وکتور_svg_جمع_کن (C1, hb_وکتور_svg_scale (hb_وکتور_svg_sub (C1, p1), 0.33333f)), radius));

    hb_وکتور_svg_rgba_t mid_color = hb_وکتور_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_وکتور_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_رقم (center.x, precision);
    body->append_c (',');
    body->append_رقم (center.y, precision);
    body->append_str ("L");
    body->append_رقم (sp0.x, precision);
    body->append_c (',');
    body->append_رقم (sp0.y, precision);
    body->append_str ("C");
    body->append_رقم (sc0.x, precision);
    body->append_c (',');
    body->append_رقم (sc0.y, precision);
    body->append_c (' ');
    body->append_رقم (sc1.x, precision);
    body->append_c (',');
    body->append_رقم (sc1.y, precision);
    body->append_c (' ');
    body->append_رقم (sp1.x, precision);
    body->append_c (',');
    body->append_رقم (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, درست);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_وکتور_svg_sweep_ctx_t
{
  hb_وکتور_buf_t *body;
  بیعلامت precision;
  عائم cx, cy, radius;
};

ساكن فراغ
hb_وکتور_svg_sweep_emit_patch (عائم a0, hb_color_t c0,
				عائم a1, hb_color_t c1,
				فراغ *user_داده)
{
  خودکار *ctx = (hb_وکتور_svg_sweep_ctx_t *) user_داده;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->body->درون_error ()))
    أرجع;
  hb_وکتور_svg_جمع_کن_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_وکتور_svg_rgba_from_hb_color (c0),
				 a1, hb_وکتور_svg_rgba_from_hb_color (c1));
}


ساكن فراغ hb_وکتور_paint_افزودن_transform (hb_paint_funcs_t *, فراغ *,
                                            عائم, عائم, عائم, عائم, عائم, عائم,
                                            فراغ *);
ساكن فراغ hb_وکتور_paint_برداشت_transform (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_وکتور_paint_fill_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, فراغ *);
ساكن فراغ hb_وکتور_paint_افزودن_clip_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن فراغ hb_وکتور_paint_افزودن_clip_rectangle (hb_paint_funcs_t *, فراغ *, عائم, عائم, عائم, عائم, فراغ *);
ساكن hb_draw_funcs_t * hb_وکتور_paint_افزودن_clip_مسیر_شروع (hb_paint_funcs_t *, فراغ *, فراغ **, فراغ *);
ساكن فراغ hb_وکتور_paint_افزودن_clip_مسیر_انتها (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_وکتور_paint_برداشت_clip (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_وکتور_paint_color (hb_paint_funcs_t *, فراغ *, hb_bool_t, hb_color_t, فراغ *);
ساكن hb_bool_t hb_وکتور_paint_image (hb_paint_funcs_t *, فراغ *, hb_blob_t *, بیعلامت, بیعلامت, hb_برچسب_t, عائم, hb_glyph_extents_t *, فراغ *);
ساكن فراغ hb_وکتور_paint_خطی_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_وکتور_paint_radial_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_وکتور_paint_sweep_gradient (hb_paint_funcs_t *, فراغ *, hb_color_سطر_t *, عائم, عائم, عائم, عائم, فراغ *);
ساكن فراغ hb_وکتور_paint_افزودن_گروه (hb_paint_funcs_t *, فراغ *, فراغ *);
ساكن فراغ hb_وکتور_paint_برداشت_گروه (hb_paint_funcs_t *, فراغ *, hb_paint_مرکب_نما_t, فراغ *);
ساكن hb_bool_t hb_وکتور_paint_color_glyph (hb_paint_funcs_t *, فراغ *, hb_codepoint_t, hb_font_t *, فراغ *);
ساكن hb_bool_t
hb_وکتور_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_استفادهنشده,
				      فراغ *paint_داده,
				      بیعلامت color_شاخص,
				      hb_color_t *color,
				      فراغ *user_داده HB_استفادهنشده)
{
  hb_وکتور_paint_t *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (!color)
    أرجع خطأ;

  hb_color_t *value = اشاره_تهی;
  إذا (!paint->custom_palette_colors.has (color_شاخص, &value) || !value)
    أرجع خطأ;
  *color = *value;
  أرجع درست;
}

ساكن مضمن فراغ free_static_وکتور_paint_funcs ();
ساكن بنية hb_وکتور_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_وکتور_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_مجموعه_افزودن_transform_func (funcs, (hb_paint_افزودن_transform_func_t) hb_وکتور_paint_افزودن_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_transform_func (funcs, (hb_paint_برداشت_transform_func_t) hb_وکتور_paint_برداشت_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_وکتور_paint_fill_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_glyph_func (funcs, (hb_paint_افزودن_clip_glyph_func_t) hb_وکتور_paint_افزودن_clip_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_rectangle_func (funcs, (hb_paint_افزودن_clip_rectangle_func_t) hb_وکتور_paint_افزودن_clip_rectangle, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_مسیر_شروع_func (funcs, (hb_paint_افزودن_clip_مسیر_شروع_func_t) hb_وکتور_paint_افزودن_clip_مسیر_شروع, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_مسیر_انتها_func (funcs, (hb_paint_افزودن_clip_مسیر_انتها_func_t) hb_وکتور_paint_افزودن_clip_مسیر_انتها, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_clip_func (funcs, (hb_paint_برداشت_clip_func_t) hb_وکتور_paint_برداشت_clip, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_color_func (funcs, (hb_paint_color_func_t) hb_وکتور_paint_color, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_image_func (funcs, (hb_paint_image_func_t) hb_وکتور_paint_image, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_وکتور_paint_خطی_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_وکتور_paint_radial_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_وکتور_paint_sweep_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_گروه_func (funcs, (hb_paint_افزودن_گروه_func_t) hb_وکتور_paint_افزودن_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_گروه_func (funcs, (hb_paint_برداشت_گروه_func_t) hb_وکتور_paint_برداشت_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_وکتور_paint_color_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_وکتور_paint_custom_palette_color, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_مجموعه_budget_func (funcs, hb_وکتور_paint_مجموعه_budget_callback, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_func (funcs, hb_وکتور_paint_get_budget_callback, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_paint_get_budget_مانده_callback, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_وکتور_paint_funcs);
    أرجع funcs;
  }
} static_وکتور_paint_funcs;

ساكن مضمن فراغ
free_static_وکتور_paint_funcs ()
{
  static_وکتور_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_وکتور_paint_svg_funcs_get ()
{
  أرجع static_وکتور_paint_funcs.get_unconst ();
}


ساكن فراغ
hb_وکتور_paint_افزودن_transform (hb_paint_funcs_t *,
                                فراغ *paint_داده,
                                عائم xx, عائم yx,
                                عائم xy, عائم yy,
                                عائم dx, عائم dy,
                                فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (unlikely (paint->transform_گروه_سرریز_عمق))
  {
    paint->transform_گروه_سرریز_عمق++;
    أرجع;
  }
  إذا (unlikely (paint->transform_گروه_عمق >= 64))
  {
    paint->transform_گروه_سرریز_عمق = 1;
    أرجع;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_گروه_open_ماسک = (paint->transform_گروه_open_ماسک << 1) | (opened ? 1ull : 0ull);
  paint->transform_گروه_عمق++;

  إذا (!opened)
    أرجع;

  خودکار &body = paint->current_body ();
  بیعلامت sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_رقم (xx, sprec);
  body.append_c (',');
  body.append_رقم (yx, sprec);
  body.append_c (',');
  body.append_رقم (xy, sprec);
  body.append_c (',');
  body.append_رقم (yy, sprec);
  body.append_c (',');
  body.append_رقم (paint->sx (dx));
  body.append_c (',');
  body.append_رقم (paint->sy (dy));
  body.append_str (")\">\n");
}

ساكن فراغ
hb_وکتور_paint_برداشت_transform (hb_paint_funcs_t *,
                               فراغ *paint_داده,
                               فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (paint->transform_گروه_سرریز_عمق))
  {
    paint->transform_گروه_سرریز_عمق--;
    أرجع;
  }
  إذا (!paint->transform_گروه_عمق)
    أرجع;
  paint->transform_گروه_عمق--;
  hb_bool_t opened = !!(paint->transform_گروه_open_ماسک & 1ull);
  paint->transform_گروه_open_ماسک >>= 1;
  إذا (opened)
    paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_وکتور_paint_fill_glyph (hb_paint_funcs_t *,
                            فراغ *paint_داده,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  paint->مسیر.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_مانده >= 0))
  {
    hb_وکتور_مسیر_sink_t sink = {&paint->مسیر, paint->get_precision (),
				 paint->x_scale_ضریب, paint->y_scale_ضریب,
				 &paint->budget_مانده};
    hb_font_draw_glyph (font, glyph, hb_وکتور_svg_مسیر_draw_funcs_get (), &sink);
  }

  خودکار &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->مسیر.arrayZ, paint->مسیر.درازی);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, درست);
  body.append_str ("\"/>\n");
}

ساكن فراغ
hb_وکتور_paint_افزودن_clip_glyph (hb_paint_funcs_t *,
                                 فراغ *paint_داده,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_پیشوند;
  بیعلامت pfx_len = paint->id_پیشوند_درازی;

  paint->مسیر.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  إذا (likely (paint->budget_مانده >= 0))
  {
    hb_وکتور_مسیر_sink_t sink = {&paint->مسیر, paint->get_precision (),
				 paint->x_scale_ضریب, paint->y_scale_ضریب,
				 &paint->budget_مانده};
    hb_font_draw_glyph (font, glyph, hb_وکتور_svg_مسیر_draw_funcs_get (), &sink);
  }

  بیعلامت def_id = paint->مسیر_def_شمار++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->مسیر.arrayZ, paint->مسیر.درازی);
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

ساكن فراغ
hb_وکتور_paint_افزودن_clip_rectangle (hb_paint_funcs_t *,
                                     فراغ *paint_داده,
                                     عائم xmin, عائم ymin,
                                     عائم xmax, عائم ymax,
                                     فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_پیشوند;
  بیعلامت pfx_len = paint->id_پیشوند_درازی;
  بیعلامت clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_رقم (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_رقم (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_رقم (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_رقم (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن hb_draw_funcs_t *
hb_وکتور_paint_افزودن_clip_مسیر_شروع (hb_paint_funcs_t *,
                                      فراغ *paint_داده,
                                      فراغ **draw_داده,
                                      فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_داده = اشاره_تهی;
    أرجع اشاره_تهی;
  }

  paint->مسیر.clear ();
  paint->clip_مسیر_sink = {&paint->مسیر, paint->get_precision (),
			   paint->x_scale_ضریب,
			   paint->y_scale_ضریب,
			   &paint->budget_مانده};
  *draw_داده = &paint->clip_مسیر_sink;
  أرجع hb_وکتور_svg_مسیر_draw_funcs_get ();
}

ساكن فراغ
hb_وکتور_paint_افزودن_clip_مسیر_انتها (hb_paint_funcs_t *,
                                    فراغ *paint_داده,
                                    فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  ثابت حرف *pfx = paint->id_پیشوند;
  بیعلامت pfx_len = paint->id_پیشوند_درازی;
  بیعلامت clip_id = paint->clip_مسیر_counter++;

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
  paint->defs.append_len (paint->مسیر.arrayZ, paint->مسیر.درازی);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

ساكن فراغ
hb_وکتور_paint_برداشت_clip (hb_paint_funcs_t *,
                          فراغ *paint_داده,
                          فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("</g>\n");
}

ساكن فراغ
hb_وکتور_paint_color (hb_paint_funcs_t *,
                       فراغ *paint_داده,
                       hb_bool_t,
                       hb_color_t color,
                       فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, درست);
  body.append_str ("\"/>\n");
}

ساكن hb_bool_t
hb_وکتور_paint_image (hb_paint_funcs_t *,
                       فراغ *paint_داده,
                       hb_blob_t *image,
                       بیعلامت عرض,
                       بیعلامت ارتفاع,
                       hb_برچسب_t format,
                       عائم slant HB_استفادهنشده,
                       hb_glyph_extents_t *extents,
                       فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;

  خودکار &body = paint->current_body ();
  إذا (format == HB_برچسب ('p','n','g',' '))
  {
    إذا (!extents || !عرض || !ارتفاع)
      أرجع خطأ;

    بیعلامت len = 0;
    ثابت حرف *png_داده = hb_blob_get_داده (image, &len);
    إذا (!png_داده || !len)
      أرجع خطأ;

    body.append_str ("<g transform=\"translate(");
    body.append_رقم (paint->sx ((عائم) extents->x_bearing));
    body.append_c (',');
    body.append_رقم (paint->sy ((عائم) extents->y_bearing));
    body.append_str (") scale(");
    body.append_رقم (paint->sx ((عائم) extents->عرض) / عرض);
    body.append_c (',');
    body.append_رقم (paint->sy ((عائم) extents->ارتفاع) / ارتفاع);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((ثابت uint8_t *) png_داده, len);
    body.append_str ("\" width=\"");
    body.append_رقم ((عائم) عرض);
    body.append_str ("\" height=\"");
    body.append_رقم ((عائم) ارتفاع);
    body.append_str ("\"/>\n</g>\n");

    أرجع درست;
  }

  أرجع خطأ;
}

ساكن فراغ
hb_وکتور_paint_خطی_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_داده,
                                 hb_color_سطر_t *color_سطر,
                                 عائم x0, عائم y0,
                                 عائم x1, عائم y1,
                                 عائم x2, عائم y2,
                                 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.درازی, &mn, &mx);

  ثابت حرف *pfx = paint->id_پیشوند;
  بیعلامت pfx_len = paint->id_پیشوند_درازی;
  بیعلامت grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  عائم gx0 = lx0 + mn * (lx1 - lx0);
  عائم gy0 = ly0 + mn * (ly1 - ly0);
  عائم gx1 = lx0 + mx * (lx1 - lx0);
  عائم gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_رقم (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_رقم (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_رقم (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_رقم (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_وکتور_svg_extend_نما_str (hb_color_سطر_get_extend (color_سطر)));
  paint->defs.append_str ("\">\n");
  hb_وکتور_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_وکتور_paint_radial_gradient (hb_paint_funcs_t *,
                                 فراغ *paint_داده,
                                 hb_color_سطر_t *color_سطر,
                                 عائم x0, عائم y0, عائم r0,
                                 عائم x1, عائم y1, عائم r1,
                                 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.درازی, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  ثابت حرف *pfx = paint->id_پیشوند;
  بیعلامت pfx_len = paint->id_پیشوند_درازی;
  بیعلامت grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_رقم (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_رقم (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_رقم (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_رقم (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_رقم (paint->sy (gy0));
  إذا (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_رقم (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_وکتور_svg_extend_نما_str (hb_color_سطر_get_extend (color_سطر)));
  paint->defs.append_str ("\">\n");
  hb_وکتور_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

ساكن فراغ
hb_وکتور_paint_sweep_gradient (hb_paint_funcs_t *,
                                فراغ *paint_داده,
                                hb_color_سطر_t *color_سطر,
                                عائم cx, عائم cy,
                                عائم شروع_angle, عائم انتها_angle,
                                فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.درازی, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  عائم ga0 = شروع_angle + mn * (انتها_angle - شروع_angle);
  عائم ga1 = شروع_angle + mx * (انتها_angle - شروع_angle);

  hb_وکتور_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.درازی,
				 hb_color_سطر_get_extend (color_سطر),
				 ga0, ga1,
				 hb_وکتور_svg_sweep_emit_patch,
				 &ctx);
}

ساكن فراغ
hb_وکتور_paint_افزودن_گروه (hb_paint_funcs_t *,
                            فراغ *paint_داده,
                            فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (unlikely (!paint->گروه_پشته.افزودن_or_fail (hb_وکتور_buf_t {})))
    أرجع;
}

ساكن فراغ
hb_وکتور_paint_برداشت_گروه (hb_paint_funcs_t *,
                           فراغ *paint_داده,
                           hb_paint_مرکب_نما_t نما,
                           فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  إذا (paint->گروه_پشته.درازی < 2)
    أرجع;

  hb_وکتور_buf_t گروه = paint->گروه_پشته.برداشت ();
  خودکار &body = paint->current_body ();

  ثابت حرف *blend = hb_وکتور_svg_مرکب_نما_str (نما);
  إذا (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (گروه.arrayZ, گروه.درازی);
    body.append_str ("</g>\n");
  }
  وإلا
    body.append_len (گروه.arrayZ, گروه.درازی);
}

ساكن hb_bool_t
hb_وکتور_paint_color_glyph (hb_paint_funcs_t *,
                             فراغ *paint_داده,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  إذا (unlikely (paint->color_glyph_عمق >= HB_MAX_NESTING_LEVEL))
    أرجع خطأ;
  إذا (unlikely (hb_مجموعه_has (paint->فعال_color_glyphs, glyph)))
    أرجع خطأ;

  paint->color_glyph_عمق++;
  hb_مجموعه_جمع_کن (paint->فعال_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_وکتور_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_مجموعه_del (paint->فعال_color_glyphs, glyph);
  paint->color_glyph_عمق--;
  أرجع درست;
}




hb_blob_t *
hb_وکتور_paint_render_svg (hb_وکتور_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع اشاره_تهی;

  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع اشاره_تهی;

  hb_وکتور_buf_t بیرون;
  hb_buf_recover_recycled (paint->recycled_blob, &بیرون);
  بیعلامت estimated = paint->defs.درازی +
		       paint->گروه_پشته.arrayZ[0].درازی +
		       320;
  بیرون.alloc (estimated);
  عائم vb_x = paint->extents.x;
  عائم vb_y = -(paint->extents.y + paint->extents.ارتفاع);
  عائم vb_w = paint->extents.عرض;
  عائم vb_h = paint->extents.ارتفاع;
  بیرون.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  بیرون.append_رقم (vb_x);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_y);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_w);
  بیرون.append_c (' ');
  بیرون.append_رقم (vb_h);
  بیرون.append_str ("\" width=\"");
  بیرون.append_رقم (vb_w);
  بیرون.append_str ("\" height=\"");
  بیرون.append_رقم (vb_h);
  بیرون.append_str ("\">\n");

  إذا (paint->defs.درازی)
  {
    بیرون.append_str ("<defs>\n");
    بیرون.append_len (paint->defs.arrayZ, paint->defs.درازی);
    بیرون.append_str ("</defs>\n");
  }

  إذا (hb_color_get_alpha (paint->background))
  {
    بیرون.append_str ("<rect x=\"");
    بیرون.append_رقم (vb_x);
    بیرون.append_str ("\" y=\"");
    بیرون.append_رقم (vb_y);
    بیرون.append_str ("\" width=\"");
    بیرون.append_رقم (vb_w);
    بیرون.append_str ("\" height=\"");
    بیرون.append_رقم (vb_h);
    بیرون.append_str ("\" fill=\"rgb(");
    بیرون.append_unsigned (hb_color_get_red (paint->background));
    بیرون.append_c (',');
    بیرون.append_unsigned (hb_color_get_green (paint->background));
    بیرون.append_c (',');
    بیرون.append_unsigned (hb_color_get_blue (paint->background));
    بیرون.append_str (")\"");
    إذا (hb_color_get_alpha (paint->background) < 255)
    {
      بیرون.append_str (" fill-opacity=\"");
      بیرون.append_رقم (hb_color_get_alpha (paint->background) / 255.f, 4);
      بیرون.append_c ('"');
    }
    بیرون.append_str ("/>\n");
  }

  بیرون.append_str ("<g transform=\"scale(1,-1)\">\n");
  بیرون.append_len (paint->گروه_پشته.arrayZ[0].arrayZ, paint->گروه_پشته.arrayZ[0].درازی);
  بیرون.append_str ("</g>\n");

  بیرون.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &بیرون);

  أرجع blob;
}
