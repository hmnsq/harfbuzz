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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-vector-paint.hh"
#అన్తర్భావ "hb-paint.hh"
#అన్తర్భావ "hb-vector-path.hh"

#అన్తర్భావ <math.h>

స్థిర స్థిరరాశి అక్షరం *
hb_సదిశ_svg_extend_బహుళకం_str (hb_paint_extend_t ext)
{
  ఎంపిక (ext)
  {
    స్థితి HB_PAINT_EXTEND_PAD: తిరిగి "pad";
    స్థితి HB_PAINT_EXTEND_REPEAT: తిరిగి "repeat";
    స్థితి HB_PAINT_EXTEND_REFLECT: తిరిగి "reflect";
    అప్రమేయ: తిరిగి "pad";
  }
}

స్థిర శూన్య
hb_సదిశ_svg_emit_color_stops (hb_సదిశ_paint_t *paint,
				hb_సదిశ_buf_t *buf,
				hb_సదిశ_t<hb_color_stop_t> *stops)
{
  కొరకు (గుర్తులేని i = 0; i < stops->నిడివి; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_అంకె (stops->arrayZ[i].స్థానభ్రంశం, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    ఒకవేళ (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_అంకె (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

స్థిర స్థిరరాశి అక్షరం *
hb_సదిశ_svg_సంయుక్త_బహుళకం_str (hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  ఎంపిక (బహుళకం)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_PLUS: తిరిగి "screen";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_XOR: తిరిగి "difference";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP:
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP:
      తిరిగి శూన్యసూచిక;
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER: తిరిగి "normal";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SCREEN: తిరిగి "screen";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY: తిరిగి "overlay";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DARKEN: తిరిగి "darken";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN: తిరిగి "lighten";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE: తిరిగి "color-dodge";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN: తిరిగి "color-burn";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT: తిరిగి "hard-light";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT: తిరిగి "soft-light";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE: తిరిగి "difference";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION: తిరిగి "exclusion";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY: తిరిగి "multiply";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE: తిరిగి "hue";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION: తిరిగి "saturation";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR: తిరిగి "color";
    స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY: తిరిగి "luminosity";
    అప్రమేయ: తిరిగి శూన్యసూచిక;
  }
}

నిర్మాణం hb_సదిశ_svg_బిందువు_t { దశాంశం x, y; };
నిర్మాణం hb_సదిశ_svg_rgba_t { దశాంశం r, g, b, a; };

స్థిర అంతఃవరుస దశాంశం
hb_సదిశ_svg_lerp (దశాంశం a, దశాంశం b, దశాంశం t)
{ తిరిగి a + (b - a) * t; }

స్థిర అంతఃవరుస దశాంశం
hb_సదిశ_svg_clamp01 (దశాంశం v)
{
  ఒకవేళ (v < 0.f) తిరిగి 0.f;
  ఒకవేళ (v > 1.f) తిరిగి 1.f;
  తిరిగి v;
}

స్థిర అంతఃవరుస hb_సదిశ_svg_rgba_t
hb_సదిశ_svg_rgba_from_hb_color (hb_color_t c)
{
  తిరిగి {(దశాంశం) hb_color_get_red (c) / 255.f,
          (దశాంశం) hb_color_get_green (c) / 255.f,
          (దశాంశం) hb_color_get_blue (c) / 255.f,
          (దశాంశం) hb_color_get_alpha (c) / 255.f};
}

స్థిర అంతఃవరుస hb_color_t
hb_సదిశ_svg_hb_color_from_rgba (స్థిరరాశి hb_సదిశ_svg_rgba_t &c)
{
  గుర్తులేని r = (గుర్తులేని) roundf (hb_సదిశ_svg_clamp01 (c.r) * 255.f);
  గుర్తులేని g = (గుర్తులేని) roundf (hb_సదిశ_svg_clamp01 (c.g) * 255.f);
  గుర్తులేని b = (గుర్తులేని) roundf (hb_సదిశ_svg_clamp01 (c.b) * 255.f);
  గుర్తులేని a = (గుర్తులేని) roundf (hb_సదిశ_svg_clamp01 (c.a) * 255.f);
  తిరిగి HB_COLOR (b, g, r, a);
}

స్థిర అంతఃవరుస hb_సదిశ_svg_rgba_t
hb_సదిశ_svg_lerp_rgba (స్థిరరాశి hb_సదిశ_svg_rgba_t &c0,
			 స్థిరరాశి hb_సదిశ_svg_rgba_t &c1,
			 దశాంశం t)
{
  తిరిగి {hb_సదిశ_svg_lerp (c0.r, c1.r, t),
          hb_సదిశ_svg_lerp (c0.g, c1.g, t),
          hb_సదిశ_svg_lerp (c0.b, c1.b, t),
          hb_సదిశ_svg_lerp (c0.a, c1.a, t)};
}

స్థిర అంతఃవరుస దశాంశం hb_సదిశ_svg_dot (స్థిరరాశి hb_సదిశ_svg_బిందువు_t &p, స్థిరరాశి hb_సదిశ_svg_బిందువు_t &q) { తిరిగి p.x * q.x + p.y * q.y; }
స్థిర అంతఃవరుస hb_సదిశ_svg_బిందువు_t hb_సదిశ_svg_కూడు (స్థిరరాశి hb_సదిశ_svg_బిందువు_t &p, స్థిరరాశి hb_సదిశ_svg_బిందువు_t &q) { తిరిగి {p.x + q.x, p.y + q.y}; }
స్థిర అంతఃవరుస hb_సదిశ_svg_బిందువు_t hb_సదిశ_svg_sub (స్థిరరాశి hb_సదిశ_svg_బిందువు_t &p, స్థిరరాశి hb_సదిశ_svg_బిందువు_t &q) { తిరిగి {p.x - q.x, p.y - q.y}; }
స్థిర అంతఃవరుస hb_సదిశ_svg_బిందువు_t hb_సదిశ_svg_scale (స్థిరరాశి hb_సదిశ_svg_బిందువు_t &p, దశాంశం f) { తిరిగి {p.x * f, p.y * f}; }

స్థిర అంతఃవరుస hb_సదిశ_svg_బిందువు_t
hb_సదిశ_svg_normalize (స్థిరరాశి hb_సదిశ_svg_బిందువు_t &p)
{
  దశాంశం len = sqrtf (hb_సదిశ_svg_dot (p, p));
  ఒకవేళ (len == 0.f) తిరిగి {0.f, 0.f};
  తిరిగి hb_సదిశ_svg_scale (p, 1.f / len);
}

స్థిర శూన్య
hb_సదిశ_svg_కూడు_sweep_patch (hb_సదిశ_buf_t *body,
			       గుర్తులేని precision,
			       దశాంశం cx, దశాంశం cy, దశాంశం radius,
			       దశాంశం a0, స్థిరరాశి hb_సదిశ_svg_rgba_t &c0_లోపల,
			       దశాంశం a1, స్థిరరాశి hb_సదిశ_svg_rgba_t &c1_లోపల)
{
  స్థిర స్థిరరాశి దశాంశం max_angle = HB_PI / 16.f;
  hb_సదిశ_svg_బిందువు_t center = {cx, cy};
  పూర్ణాంకం అంకె_splits = (పూర్ణాంకం) ceilf (fabsf (a1 - a0) / max_angle);
  ఒకవేళ (అంకె_splits < 1) అంకె_splits = 1;

  hb_సదిశ_svg_బిందువు_t p0 = {cosf (a0), sinf (a0)};
  hb_సదిశ_svg_rgba_t color0 = c0_లోపల;

  కొరకు (పూర్ణాంకం a = 0; a < అంకె_splits; a++)
  {
    దశాంశం k = (a + 1.f) / అంకె_splits;
    దశాంశం angle1 = hb_సదిశ_svg_lerp (a0, a1, k);
    hb_సదిశ_svg_rgba_t color1 = hb_సదిశ_svg_lerp_rgba (c0_లోపల, c1_లోపల, k);

    hb_సదిశ_svg_బిందువు_t p1 = {cosf (angle1), sinf (angle1)};
    hb_సదిశ_svg_బిందువు_t sp0 = hb_సదిశ_svg_కూడు (center, hb_సదిశ_svg_scale (p0, radius));
    hb_సదిశ_svg_బిందువు_t sp1 = hb_సదిశ_svg_కూడు (center, hb_సదిశ_svg_scale (p1, radius));

    hb_సదిశ_svg_బిందువు_t A = hb_సదిశ_svg_normalize (hb_సదిశ_svg_కూడు (p0, p1));
    hb_సదిశ_svg_బిందువు_t U = {-A.y, A.x};
    దశాంశం up0 = hb_సదిశ_svg_dot (U, p0);
    దశాంశం up1 = hb_సదిశ_svg_dot (U, p1);
    ఒకవేళ (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      కొనసాగు;
    }
    hb_సదిశ_svg_బిందువు_t C0 = hb_సదిశ_svg_కూడు (A, hb_సదిశ_svg_scale (U, hb_సదిశ_svg_dot (hb_సదిశ_svg_sub (p0, A), p0) / up0));
    hb_సదిశ_svg_బిందువు_t C1 = hb_సదిశ_svg_కూడు (A, hb_సదిశ_svg_scale (U, hb_సదిశ_svg_dot (hb_సదిశ_svg_sub (p1, A), p1) / up1));

    hb_సదిశ_svg_బిందువు_t sc0 = hb_సదిశ_svg_కూడు (center, hb_సదిశ_svg_scale (hb_సదిశ_svg_కూడు (C0, hb_సదిశ_svg_scale (hb_సదిశ_svg_sub (C0, p0), 0.33333f)), radius));
    hb_సదిశ_svg_బిందువు_t sc1 = hb_సదిశ_svg_కూడు (center, hb_సదిశ_svg_scale (hb_సదిశ_svg_కూడు (C1, hb_సదిశ_svg_scale (hb_సదిశ_svg_sub (C1, p1), 0.33333f)), radius));

    hb_సదిశ_svg_rgba_t mid_color = hb_సదిశ_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_సదిశ_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_అంకె (center.x, precision);
    body->append_c (',');
    body->append_అంకె (center.y, precision);
    body->append_str ("L");
    body->append_అంకె (sp0.x, precision);
    body->append_c (',');
    body->append_అంకె (sp0.y, precision);
    body->append_str ("C");
    body->append_అంకె (sc0.x, precision);
    body->append_c (',');
    body->append_అంకె (sc0.y, precision);
    body->append_c (' ');
    body->append_అంకె (sc1.x, precision);
    body->append_c (',');
    body->append_అంకె (sc1.y, precision);
    body->append_c (' ');
    body->append_అంకె (sp1.x, precision);
    body->append_c (',');
    body->append_అంకె (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, నిజం);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
నిర్మాణం hb_సదిశ_svg_sweep_ctx_t
{
  hb_సదిశ_buf_t *body;
  గుర్తులేని precision;
  దశాంశం cx, cy, radius;
};

స్థిర శూన్య
hb_సదిశ_svg_sweep_emit_patch (దశాంశం a0, hb_color_t c0,
				దశాంశం a1, hb_color_t c1,
				శూన్య *user_దత్తాంశం)
{
  స్వయంచాలక *ctx = (hb_సదిశ_svg_sweep_ctx_t *) user_దత్తాంశం;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  ఒకవేళ (unlikely (ctx->body->లోపల_error ()))
    తిరిగి;
  hb_సదిశ_svg_కూడు_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_సదిశ_svg_rgba_from_hb_color (c0),
				 a1, hb_సదిశ_svg_rgba_from_hb_color (c1));
}


స్థిర శూన్య hb_సదిశ_paint_నెట్టు_transform (hb_paint_funcs_t *, శూన్య *,
                                            దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం,
                                            శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_తీయి_transform (hb_paint_funcs_t *, శూన్య *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_fill_glyph (hb_paint_funcs_t *, శూన్య *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_నెట్టు_clip_glyph (hb_paint_funcs_t *, శూన్య *, hb_codepoint_t, hb_font_t *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_నెట్టు_clip_rectangle (hb_paint_funcs_t *, శూన్య *, దశాంశం, దశాంశం, దశాంశం, దశాంశం, శూన్య *);
స్థిర hb_draw_funcs_t * hb_సదిశ_paint_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *, శూన్య *, శూన్య **, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *, శూన్య *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_తీయి_clip (hb_paint_funcs_t *, శూన్య *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_color (hb_paint_funcs_t *, శూన్య *, hb_bool_t, hb_color_t, శూన్య *);
స్థిర hb_bool_t hb_సదిశ_paint_image (hb_paint_funcs_t *, శూన్య *, hb_blob_t *, గుర్తులేని, గుర్తులేని, hb_ట్యాగ్_t, దశాంశం, hb_glyph_extents_t *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_రేఖీయ_gradient (hb_paint_funcs_t *, శూన్య *, hb_color_పంక్తి_t *, దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_radial_gradient (hb_paint_funcs_t *, శూన్య *, hb_color_పంక్తి_t *, దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం, దశాంశం, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_sweep_gradient (hb_paint_funcs_t *, శూన్య *, hb_color_పంక్తి_t *, దశాంశం, దశాంశం, దశాంశం, దశాంశం, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_నెట్టు_సమూహం (hb_paint_funcs_t *, శూన్య *, శూన్య *);
స్థిర శూన్య hb_సదిశ_paint_తీయి_సమూహం (hb_paint_funcs_t *, శూన్య *, hb_paint_సంయుక్త_బహుళకం_t, శూన్య *);
స్థిర hb_bool_t hb_సదిశ_paint_color_glyph (hb_paint_funcs_t *, శూన్య *, hb_codepoint_t, hb_font_t *, శూన్య *);
స్థిర hb_bool_t
hb_సదిశ_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_వాడని,
				      శూన్య *paint_దత్తాంశం,
				      గుర్తులేని color_సూచీ,
				      hb_color_t *color,
				      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_సదిశ_paint_t *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (!color)
    తిరిగి అబద్ధం;

  hb_color_t *value = శూన్యసూచిక;
  ఒకవేళ (!paint->custom_palette_colors.has (color_సూచీ, &value) || !value)
    తిరిగి అబద్ధం;
  *color = *value;
  తిరిగి నిజం;
}

స్థిర అంతఃవరుస శూన్య free_static_సదిశ_paint_funcs ();
స్థిర నిర్మాణం hb_సదిశ_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_సదిశ_paint_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_సమితి_నెట్టు_transform_func (funcs, (hb_paint_నెట్టు_transform_func_t) hb_సదిశ_paint_నెట్టు_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func (funcs, (hb_paint_తీయి_transform_func_t) hb_సదిశ_paint_తీయి_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_సదిశ_paint_fill_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func (funcs, (hb_paint_నెట్టు_clip_glyph_func_t) hb_సదిశ_paint_నెట్టు_clip_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_rectangle_func (funcs, (hb_paint_నెట్టు_clip_rectangle_func_t) hb_సదిశ_paint_నెట్టు_clip_rectangle, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_ప్రారంభం_func (funcs, (hb_paint_నెట్టు_clip_మార్గం_ప్రారంభం_func_t) hb_సదిశ_paint_నెట్టు_clip_మార్గం_ప్రారంభం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_చివర_func (funcs, (hb_paint_నెట్టు_clip_మార్గం_చివర_func_t) hb_సదిశ_paint_నెట్టు_clip_మార్గం_చివర, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func (funcs, (hb_paint_తీయి_clip_func_t) hb_సదిశ_paint_తీయి_clip, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func (funcs, (hb_paint_color_func_t) hb_సదిశ_paint_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_image_func (funcs, (hb_paint_image_func_t) hb_సదిశ_paint_image, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func (funcs, (hb_paint_రేఖీయ_gradient_func_t) hb_సదిశ_paint_రేఖీయ_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_సదిశ_paint_radial_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_సదిశ_paint_sweep_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func (funcs, (hb_paint_నెట్టు_సమూహం_func_t) hb_సదిశ_paint_నెట్టు_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func (funcs, (hb_paint_తీయి_సమూహం_func_t) hb_సదిశ_paint_తీయి_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_సదిశ_paint_color_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_సదిశ_paint_custom_palette_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_సదిశ_paint_సమితి_budget_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_సదిశ_paint_get_budget_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_paint_get_budget_మిగిలిన_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_సదిశ_paint_funcs);
    తిరిగి funcs;
  }
} static_సదిశ_paint_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_సదిశ_paint_funcs ()
{
  static_సదిశ_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_సదిశ_paint_svg_funcs_get ()
{
  తిరిగి static_సదిశ_paint_funcs.get_unconst ();
}


స్థిర శూన్య
hb_సదిశ_paint_నెట్టు_transform (hb_paint_funcs_t *,
                                శూన్య *paint_దత్తాంశం,
                                దశాంశం xx, దశాంశం yx,
                                దశాంశం xy, దశాంశం yy,
                                దశాంశం dx, దశాంశం dy,
                                శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  ఒకవేళ (unlikely (paint->transform_సమూహం_అధికప్రవాహం_లోతు))
  {
    paint->transform_సమూహం_అధికప్రవాహం_లోతు++;
    తిరిగి;
  }
  ఒకవేళ (unlikely (paint->transform_సమూహం_లోతు >= 64))
  {
    paint->transform_సమూహం_అధికప్రవాహం_లోతు = 1;
    తిరిగి;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_సమూహం_open_ముసుగు = (paint->transform_సమూహం_open_ముసుగు << 1) | (opened ? 1ull : 0ull);
  paint->transform_సమూహం_లోతు++;

  ఒకవేళ (!opened)
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  గుర్తులేని sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_అంకె (xx, sprec);
  body.append_c (',');
  body.append_అంకె (yx, sprec);
  body.append_c (',');
  body.append_అంకె (xy, sprec);
  body.append_c (',');
  body.append_అంకె (yy, sprec);
  body.append_c (',');
  body.append_అంకె (paint->sx (dx));
  body.append_c (',');
  body.append_అంకె (paint->sy (dy));
  body.append_str (")\">\n");
}

స్థిర శూన్య
hb_సదిశ_paint_తీయి_transform (hb_paint_funcs_t *,
                               శూన్య *paint_దత్తాంశం,
                               శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  ఒకవేళ (unlikely (paint->transform_సమూహం_అధికప్రవాహం_లోతు))
  {
    paint->transform_సమూహం_అధికప్రవాహం_లోతు--;
    తిరిగి;
  }
  ఒకవేళ (!paint->transform_సమూహం_లోతు)
    తిరిగి;
  paint->transform_సమూహం_లోతు--;
  hb_bool_t opened = !!(paint->transform_సమూహం_open_ముసుగు & 1ull);
  paint->transform_సమూహం_open_ముసుగు >>= 1;
  ఒకవేళ (opened)
    paint->current_body ().append_str ("</g>\n");
}

స్థిర శూన్య
hb_సదిశ_paint_fill_glyph (hb_paint_funcs_t *,
                            శూన్య *paint_దత్తాంశం,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  paint->మార్గం.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  ఒకవేళ (likely (paint->budget_మిగిలిన >= 0))
  {
    hb_సదిశ_మార్గం_sink_t sink = {&paint->మార్గం, paint->get_precision (),
				 paint->x_scale_గుణకం, paint->y_scale_గుణకం,
				 &paint->budget_మిగిలిన};
    hb_font_draw_glyph (font, glyph, hb_సదిశ_svg_మార్గం_draw_funcs_get (), &sink);
  }

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->మార్గం.arrayZ, paint->మార్గం.నిడివి);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, నిజం);
  body.append_str ("\"/>\n");
}

స్థిర శూన్య
hb_సదిశ_paint_నెట్టు_clip_glyph (hb_paint_funcs_t *,
                                 శూన్య *paint_దత్తాంశం,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్థిరరాశి అక్షరం *pfx = paint->id_ఉపసర్గ;
  గుర్తులేని pfx_len = paint->id_ఉపసర్గ_నిడివి;

  paint->మార్గం.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  ఒకవేళ (likely (paint->budget_మిగిలిన >= 0))
  {
    hb_సదిశ_మార్గం_sink_t sink = {&paint->మార్గం, paint->get_precision (),
				 paint->x_scale_గుణకం, paint->y_scale_గుణకం,
				 &paint->budget_మిగిలిన};
    hb_font_draw_glyph (font, glyph, hb_సదిశ_svg_మార్గం_draw_funcs_get (), &sink);
  }

  గుర్తులేని def_id = paint->మార్గం_def_లెక్క++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->మార్గం.arrayZ, paint->మార్గం.నిడివి);
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

స్థిర శూన్య
hb_సదిశ_paint_నెట్టు_clip_rectangle (hb_paint_funcs_t *,
                                     శూన్య *paint_దత్తాంశం,
                                     దశాంశం xmin, దశాంశం ymin,
                                     దశాంశం xmax, దశాంశం ymax,
                                     శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్థిరరాశి అక్షరం *pfx = paint->id_ఉపసర్గ;
  గుర్తులేని pfx_len = paint->id_ఉపసర్గ_నిడివి;
  గుర్తులేని clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_అంకె (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_అంకె (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_అంకె (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_అంకె (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

స్థిర hb_draw_funcs_t *
hb_సదిశ_paint_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *,
                                      శూన్య *paint_దత్తాంశం,
                                      శూన్య **draw_దత్తాంశం,
                                      శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_దత్తాంశం = శూన్యసూచిక;
    తిరిగి శూన్యసూచిక;
  }

  paint->మార్గం.clear ();
  paint->clip_మార్గం_sink = {&paint->మార్గం, paint->get_precision (),
			   paint->x_scale_గుణకం,
			   paint->y_scale_గుణకం,
			   &paint->budget_మిగిలిన};
  *draw_దత్తాంశం = &paint->clip_మార్గం_sink;
  తిరిగి hb_సదిశ_svg_మార్గం_draw_funcs_get ();
}

స్థిర శూన్య
hb_సదిశ_paint_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *,
                                    శూన్య *paint_దత్తాంశం,
                                    శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్థిరరాశి అక్షరం *pfx = paint->id_ఉపసర్గ;
  గుర్తులేని pfx_len = paint->id_ఉపసర్గ_నిడివి;
  గుర్తులేని clip_id = paint->clip_మార్గం_counter++;

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
  paint->defs.append_len (paint->మార్గం.arrayZ, paint->మార్గం.నిడివి);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

స్థిర శూన్య
hb_సదిశ_paint_తీయి_clip (hb_paint_funcs_t *,
                          శూన్య *paint_దత్తాంశం,
                          శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("</g>\n");
}

స్థిర శూన్య
hb_సదిశ_paint_color (hb_paint_funcs_t *,
                       శూన్య *paint_దత్తాంశం,
                       hb_bool_t,
                       hb_color_t color,
                       శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, నిజం);
  body.append_str ("\"/>\n");
}

స్థిర hb_bool_t
hb_సదిశ_paint_image (hb_paint_funcs_t *,
                       శూన్య *paint_దత్తాంశం,
                       hb_blob_t *image,
                       గుర్తులేని వెడల్పు,
                       గుర్తులేని ఎత్తు,
                       hb_ట్యాగ్_t format,
                       దశాంశం slant HB_వాడని,
                       hb_glyph_extents_t *extents,
                       శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి అబద్ధం;

  స్వయంచాలక &body = paint->current_body ();
  ఒకవేళ (format == HB_ట్యాగ్ ('p','n','g',' '))
  {
    ఒకవేళ (!extents || !వెడల్పు || !ఎత్తు)
      తిరిగి అబద్ధం;

    గుర్తులేని len = 0;
    స్థిరరాశి అక్షరం *png_దత్తాంశం = hb_blob_get_దత్తాంశం (image, &len);
    ఒకవేళ (!png_దత్తాంశం || !len)
      తిరిగి అబద్ధం;

    body.append_str ("<g transform=\"translate(");
    body.append_అంకె (paint->sx ((దశాంశం) extents->x_bearing));
    body.append_c (',');
    body.append_అంకె (paint->sy ((దశాంశం) extents->y_bearing));
    body.append_str (") scale(");
    body.append_అంకె (paint->sx ((దశాంశం) extents->వెడల్పు) / వెడల్పు);
    body.append_c (',');
    body.append_అంకె (paint->sy ((దశాంశం) extents->ఎత్తు) / ఎత్తు);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((స్థిరరాశి uint8_t *) png_దత్తాంశం, len);
    body.append_str ("\" width=\"");
    body.append_అంకె ((దశాంశం) వెడల్పు);
    body.append_str ("\" height=\"");
    body.append_అంకె ((దశాంశం) ఎత్తు);
    body.append_str ("\"/>\n</g>\n");

    తిరిగి నిజం;
  }

  తిరిగి అబద్ధం;
}

స్థిర శూన్య
hb_సదిశ_paint_రేఖీయ_gradient (hb_paint_funcs_t *,
                                 శూన్య *paint_దత్తాంశం,
                                 hb_color_పంక్తి_t *color_పంక్తి,
                                 దశాంశం x0, దశాంశం y0,
                                 దశాంశం x1, దశాంశం y1,
                                 దశాంశం x2, దశాంశం y2,
                                 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops.arrayZ, stops.నిడివి, &mn, &mx);

  స్థిరరాశి అక్షరం *pfx = paint->id_ఉపసర్గ;
  గుర్తులేని pfx_len = paint->id_ఉపసర్గ_నిడివి;
  గుర్తులేని grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  దశాంశం lx0, ly0, lx1, ly1;
  hb_paint_reduce_రేఖీయ_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  దశాంశం gx0 = lx0 + mn * (lx1 - lx0);
  దశాంశం gy0 = ly0 + mn * (ly1 - ly0);
  దశాంశం gx1 = lx0 + mx * (lx1 - lx0);
  దశాంశం gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_అంకె (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_అంకె (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_అంకె (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_అంకె (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_సదిశ_svg_extend_బహుళకం_str (hb_color_పంక్తి_get_extend (color_పంక్తి)));
  paint->defs.append_str ("\">\n");
  hb_సదిశ_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

స్థిర శూన్య
hb_సదిశ_paint_radial_gradient (hb_paint_funcs_t *,
                                 శూన్య *paint_దత్తాంశం,
                                 hb_color_పంక్తి_t *color_పంక్తి,
                                 దశాంశం x0, దశాంశం y0, దశాంశం r0,
                                 దశాంశం x1, దశాంశం y1, దశాంశం r1,
                                 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops.arrayZ, stops.నిడివి, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  దశాంశం gx0 = x0 + mn * (x1 - x0);
  దశాంశం gy0 = y0 + mn * (y1 - y0);
  దశాంశం gr0 = r0 + mn * (r1 - r0);
  దశాంశం gx1 = x0 + mx * (x1 - x0);
  దశాంశం gy1 = y0 + mx * (y1 - y0);
  దశాంశం gr1 = r0 + mx * (r1 - r0);

  స్థిరరాశి అక్షరం *pfx = paint->id_ఉపసర్గ;
  గుర్తులేని pfx_len = paint->id_ఉపసర్గ_నిడివి;
  గుర్తులేని grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_అంకె (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_అంకె (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_అంకె (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_అంకె (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_అంకె (paint->sy (gy0));
  ఒకవేళ (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_అంకె (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_సదిశ_svg_extend_బహుళకం_str (hb_color_పంక్తి_get_extend (color_పంక్తి)));
  paint->defs.append_str ("\">\n");
  hb_సదిశ_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

స్థిర శూన్య
hb_సదిశ_paint_sweep_gradient (hb_paint_funcs_t *,
                                శూన్య *paint_దత్తాంశం,
                                hb_color_పంక్తి_t *color_పంక్తి,
                                దశాంశం cx, దశాంశం cy,
                                దశాంశం ప్రారంభం_angle, దశాంశం చివర_angle,
                                శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops.arrayZ, stops.నిడివి, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  దశాంశం ga0 = ప్రారంభం_angle + mn * (చివర_angle - ప్రారంభం_angle);
  దశాంశం ga1 = ప్రారంభం_angle + mx * (చివర_angle - ప్రారంభం_angle);

  hb_సదిశ_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.నిడివి,
				 hb_color_పంక్తి_get_extend (color_పంక్తి),
				 ga0, ga1,
				 hb_సదిశ_svg_sweep_emit_patch,
				 &ctx);
}

స్థిర శూన్య
hb_సదిశ_paint_నెట్టు_సమూహం (hb_paint_funcs_t *,
                            శూన్య *paint_దత్తాంశం,
                            శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  ఒకవేళ (unlikely (!paint->సమూహం_దొంతర.నెట్టు_or_fail (hb_సదిశ_buf_t {})))
    తిరిగి;
}

స్థిర శూన్య
hb_సదిశ_paint_తీయి_సమూహం (hb_paint_funcs_t *,
                           శూన్య *paint_దత్తాంశం,
                           hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
                           శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  ఒకవేళ (paint->సమూహం_దొంతర.నిడివి < 2)
    తిరిగి;

  hb_సదిశ_buf_t సమూహం = paint->సమూహం_దొంతర.తీయి ();
  స్వయంచాలక &body = paint->current_body ();

  స్థిరరాశి అక్షరం *blend = hb_సదిశ_svg_సంయుక్త_బహుళకం_str (బహుళకం);
  ఒకవేళ (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (సమూహం.arrayZ, సమూహం.నిడివి);
    body.append_str ("</g>\n");
  }
  లేకపోతే
    body.append_len (సమూహం.arrayZ, సమూహం.నిడివి);
}

స్థిర hb_bool_t
hb_సదిశ_paint_color_glyph (hb_paint_funcs_t *,
                             శూన్య *paint_దత్తాంశం,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (paint->color_glyph_లోతు >= HB_MAX_NESTING_LEVEL))
    తిరిగి అబద్ధం;
  ఒకవేళ (unlikely (hb_సమితి_has (paint->చురుకైన_color_glyphs, glyph)))
    తిరిగి అబద్ధం;

  paint->color_glyph_లోతు++;
  hb_సమితి_కూడు (paint->చురుకైన_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_సదిశ_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_సమితి_del (paint->చురుకైన_color_glyphs, glyph);
  paint->color_glyph_లోతు--;
  తిరిగి నిజం;
}




hb_blob_t *
hb_సదిశ_paint_render_svg (hb_సదిశ_paint_t *paint)
{
  ఒకవేళ (!paint->has_extents)
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి శూన్యసూచిక;

  hb_సదిశ_buf_t బహిర్గతం;
  hb_buf_recover_recycled (paint->recycled_blob, &బహిర్గతం);
  గుర్తులేని estimated = paint->defs.నిడివి +
		       paint->సమూహం_దొంతర.arrayZ[0].నిడివి +
		       320;
  బహిర్గతం.alloc (estimated);
  దశాంశం vb_x = paint->extents.x;
  దశాంశం vb_y = -(paint->extents.y + paint->extents.ఎత్తు);
  దశాంశం vb_w = paint->extents.వెడల్పు;
  దశాంశం vb_h = paint->extents.ఎత్తు;
  బహిర్గతం.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  బహిర్గతం.append_అంకె (vb_x);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_y);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_w);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (vb_h);
  బహిర్గతం.append_str ("\" width=\"");
  బహిర్గతం.append_అంకె (vb_w);
  బహిర్గతం.append_str ("\" height=\"");
  బహిర్గతం.append_అంకె (vb_h);
  బహిర్గతం.append_str ("\">\n");

  ఒకవేళ (paint->defs.నిడివి)
  {
    బహిర్గతం.append_str ("<defs>\n");
    బహిర్గతం.append_len (paint->defs.arrayZ, paint->defs.నిడివి);
    బహిర్గతం.append_str ("</defs>\n");
  }

  ఒకవేళ (hb_color_get_alpha (paint->background))
  {
    బహిర్గతం.append_str ("<rect x=\"");
    బహిర్గతం.append_అంకె (vb_x);
    బహిర్గతం.append_str ("\" y=\"");
    బహిర్గతం.append_అంకె (vb_y);
    బహిర్గతం.append_str ("\" width=\"");
    బహిర్గతం.append_అంకె (vb_w);
    బహిర్గతం.append_str ("\" height=\"");
    బహిర్గతం.append_అంకె (vb_h);
    బహిర్గతం.append_str ("\" fill=\"rgb(");
    బహిర్గతం.append_unsigned (hb_color_get_red (paint->background));
    బహిర్గతం.append_c (',');
    బహిర్గతం.append_unsigned (hb_color_get_green (paint->background));
    బహిర్గతం.append_c (',');
    బహిర్గతం.append_unsigned (hb_color_get_blue (paint->background));
    బహిర్గతం.append_str (")\"");
    ఒకవేళ (hb_color_get_alpha (paint->background) < 255)
    {
      బహిర్గతం.append_str (" fill-opacity=\"");
      బహిర్గతం.append_అంకె (hb_color_get_alpha (paint->background) / 255.f, 4);
      బహిర్గతం.append_c ('"');
    }
    బహిర్గతం.append_str ("/>\n");
  }

  బహిర్గతం.append_str ("<g transform=\"scale(1,-1)\">\n");
  బహిర్గతం.append_len (paint->సమూహం_దొంతర.arrayZ[0].arrayZ, paint->సమూహం_దొంతర.arrayZ[0].నిడివి);
  బహిర్గతం.append_str ("</g>\n");

  బహిర్గతం.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &బహిర్గతం);

  తిరిగి blob;
}
