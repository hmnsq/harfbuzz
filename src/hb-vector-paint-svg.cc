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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-vector-paint.hh"
#அந்தர்பாவ "hb-paint.hh"
#அந்தர்பாவ "hb-vector-path.hh"

#அந்தர்பாவ <math.h>

நிலையான மாறிலி எழுத்து *
hb_vector_svg_extend_mode_str (hb_paint_extend_t ext)
{
  தேர்வு (ext)
  {
    நிலை HB_PAINT_EXTEND_PAD: திரும்பு "pad";
    நிலை HB_PAINT_EXTEND_REPEAT: திரும்பு "repeat";
    நிலை HB_PAINT_EXTEND_REFLECT: திரும்பு "reflect";
    இயல்பு: திரும்பு "pad";
  }
}

நிலையான வெறுமை
hb_vector_svg_emit_color_stops (hb_vector_paint_t *paint,
				hb_vector_buf_t *buf,
				hb_vector_t<hb_color_stop_t> *stops)
{
  க்கு (unsigned i = 0; i < stops->length; i++)
  {
    hb_color_t c = stops->arrayZ[i].color;
    buf->append_str ("<stop offset=\"");
    buf->append_இலக்கம் (stops->arrayZ[i].offset, 4);
    buf->append_str ("\" stop-color=\"rgb(");
    buf->append_unsigned (hb_color_get_red (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_green (c));
    buf->append_c (',');
    buf->append_unsigned (hb_color_get_blue (c));
    buf->append_str (")\"");
    எனில் (hb_color_get_alpha (c) != 255)
    {
      buf->append_str (" stop-opacity=\"");
      buf->append_இலக்கம் (hb_color_get_alpha (c) / 255.f, 4);
      buf->append_c ('"');
    }
    buf->append_str ("/>\n");
  }
}

நிலையான மாறிலி எழுத்து *
hb_vector_svg_composite_mode_str (hb_paint_composite_mode_t mode)
{
  தேர்வு (mode)
  {
    /* Porter-Duff modes have no SVG mix-blend-mode equivalent;
     * approximate the two that have a plausible color-blend analog,
     * and let the rest fall through to normal (SRC_OVER). */
    நிலை HB_PAINT_COMPOSITE_MODE_PLUS: திரும்பு "screen";
    நிலை HB_PAINT_COMPOSITE_MODE_XOR: திரும்பு "difference";
    நிலை HB_PAINT_COMPOSITE_MODE_CLEAR:
    நிலை HB_PAINT_COMPOSITE_MODE_SRC:
    நிலை HB_PAINT_COMPOSITE_MODE_DEST:
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_OVER:
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_IN:
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_IN:
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_OUT:
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_OUT:
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
    நிலை HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
      திரும்பு nullptr;
    நிலை HB_PAINT_COMPOSITE_MODE_SRC_OVER: திரும்பு "normal";
    நிலை HB_PAINT_COMPOSITE_MODE_SCREEN: திரும்பு "screen";
    நிலை HB_PAINT_COMPOSITE_MODE_OVERLAY: திரும்பு "overlay";
    நிலை HB_PAINT_COMPOSITE_MODE_DARKEN: திரும்பு "darken";
    நிலை HB_PAINT_COMPOSITE_MODE_LIGHTEN: திரும்பு "lighten";
    நிலை HB_PAINT_COMPOSITE_MODE_COLOR_DODGE: திரும்பு "color-dodge";
    நிலை HB_PAINT_COMPOSITE_MODE_COLOR_BURN: திரும்பு "color-burn";
    நிலை HB_PAINT_COMPOSITE_MODE_HARD_LIGHT: திரும்பு "hard-light";
    நிலை HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT: திரும்பு "soft-light";
    நிலை HB_PAINT_COMPOSITE_MODE_DIFFERENCE: திரும்பு "difference";
    நிலை HB_PAINT_COMPOSITE_MODE_EXCLUSION: திரும்பு "exclusion";
    நிலை HB_PAINT_COMPOSITE_MODE_MULTIPLY: திரும்பு "multiply";
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_HUE: திரும்பு "hue";
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: திரும்பு "saturation";
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_COLOR: திரும்பு "color";
    நிலை HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: திரும்பு "luminosity";
    இயல்பு: திரும்பு nullptr;
  }
}

கட்டமைப்பு hb_vector_svg_point_t { தசம x, y; };
கட்டமைப்பு hb_vector_svg_rgba_t { தசம r, g, b, a; };

நிலையான உள்வரி தசம
hb_vector_svg_lerp (தசம a, தசம b, தசம t)
{ திரும்பு a + (b - a) * t; }

நிலையான உள்வரி தசம
hb_vector_svg_clamp01 (தசம v)
{
  எனில் (v < 0.f) திரும்பு 0.f;
  எனில் (v > 1.f) திரும்பு 1.f;
  திரும்பு v;
}

நிலையான உள்வரி hb_vector_svg_rgba_t
hb_vector_svg_rgba_from_hb_color (hb_color_t c)
{
  திரும்பு {(தசம) hb_color_get_red (c) / 255.f,
          (தசம) hb_color_get_green (c) / 255.f,
          (தசம) hb_color_get_blue (c) / 255.f,
          (தசம) hb_color_get_alpha (c) / 255.f};
}

நிலையான உள்வரி hb_color_t
hb_vector_svg_hb_color_from_rgba (மாறிலி hb_vector_svg_rgba_t &c)
{
  unsigned r = (unsigned) roundf (hb_vector_svg_clamp01 (c.r) * 255.f);
  unsigned g = (unsigned) roundf (hb_vector_svg_clamp01 (c.g) * 255.f);
  unsigned b = (unsigned) roundf (hb_vector_svg_clamp01 (c.b) * 255.f);
  unsigned a = (unsigned) roundf (hb_vector_svg_clamp01 (c.a) * 255.f);
  திரும்பு HB_COLOR (b, g, r, a);
}

நிலையான உள்வரி hb_vector_svg_rgba_t
hb_vector_svg_lerp_rgba (மாறிலி hb_vector_svg_rgba_t &c0,
			 மாறிலி hb_vector_svg_rgba_t &c1,
			 தசம t)
{
  திரும்பு {hb_vector_svg_lerp (c0.r, c1.r, t),
          hb_vector_svg_lerp (c0.g, c1.g, t),
          hb_vector_svg_lerp (c0.b, c1.b, t),
          hb_vector_svg_lerp (c0.a, c1.a, t)};
}

நிலையான உள்வரி தசம hb_vector_svg_dot (மாறிலி hb_vector_svg_point_t &p, மாறிலி hb_vector_svg_point_t &q) { திரும்பு p.x * q.x + p.y * q.y; }
நிலையான உள்வரி hb_vector_svg_point_t hb_vector_svg_add (மாறிலி hb_vector_svg_point_t &p, மாறிலி hb_vector_svg_point_t &q) { திரும்பு {p.x + q.x, p.y + q.y}; }
நிலையான உள்வரி hb_vector_svg_point_t hb_vector_svg_sub (மாறிலி hb_vector_svg_point_t &p, மாறிலி hb_vector_svg_point_t &q) { திரும்பு {p.x - q.x, p.y - q.y}; }
நிலையான உள்வரி hb_vector_svg_point_t hb_vector_svg_scale (மாறிலி hb_vector_svg_point_t &p, தசம f) { திரும்பு {p.x * f, p.y * f}; }

நிலையான உள்வரி hb_vector_svg_point_t
hb_vector_svg_normalize (மாறிலி hb_vector_svg_point_t &p)
{
  தசம len = sqrtf (hb_vector_svg_dot (p, p));
  எனில் (len == 0.f) திரும்பு {0.f, 0.f};
  திரும்பு hb_vector_svg_scale (p, 1.f / len);
}

நிலையான வெறுமை
hb_vector_svg_add_sweep_patch (hb_vector_buf_t *body,
			       unsigned precision,
			       தசம cx, தசம cy, தசம radius,
			       தசம a0, மாறிலி hb_vector_svg_rgba_t &c0_in,
			       தசம a1, மாறிலி hb_vector_svg_rgba_t &c1_in)
{
  நிலையான மாறிலி தசம max_angle = HB_PI / 16.f;
  hb_vector_svg_point_t center = {cx, cy};
  முழு இலக்கம்_splits = (முழு) ceilf (fabsf (a1 - a0) / max_angle);
  எனில் (இலக்கம்_splits < 1) இலக்கம்_splits = 1;

  hb_vector_svg_point_t p0 = {cosf (a0), sinf (a0)};
  hb_vector_svg_rgba_t color0 = c0_in;

  க்கு (முழு a = 0; a < இலக்கம்_splits; a++)
  {
    தசம k = (a + 1.f) / இலக்கம்_splits;
    தசம angle1 = hb_vector_svg_lerp (a0, a1, k);
    hb_vector_svg_rgba_t color1 = hb_vector_svg_lerp_rgba (c0_in, c1_in, k);

    hb_vector_svg_point_t p1 = {cosf (angle1), sinf (angle1)};
    hb_vector_svg_point_t sp0 = hb_vector_svg_add (center, hb_vector_svg_scale (p0, radius));
    hb_vector_svg_point_t sp1 = hb_vector_svg_add (center, hb_vector_svg_scale (p1, radius));

    hb_vector_svg_point_t A = hb_vector_svg_normalize (hb_vector_svg_add (p0, p1));
    hb_vector_svg_point_t U = {-A.y, A.x};
    தசம up0 = hb_vector_svg_dot (U, p0);
    தசம up1 = hb_vector_svg_dot (U, p1);
    எனில் (fabsf (up0) < 1e-6f || fabsf (up1) < 1e-6f)
    {
      p0 = p1;
      color0 = color1;
      தொடர்;
    }
    hb_vector_svg_point_t C0 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p0, A), p0) / up0));
    hb_vector_svg_point_t C1 = hb_vector_svg_add (A, hb_vector_svg_scale (U, hb_vector_svg_dot (hb_vector_svg_sub (p1, A), p1) / up1));

    hb_vector_svg_point_t sc0 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C0, hb_vector_svg_scale (hb_vector_svg_sub (C0, p0), 0.33333f)), radius));
    hb_vector_svg_point_t sc1 = hb_vector_svg_add (center, hb_vector_svg_scale (hb_vector_svg_add (C1, hb_vector_svg_scale (hb_vector_svg_sub (C1, p1), 0.33333f)), radius));

    hb_vector_svg_rgba_t mid_color = hb_vector_svg_lerp_rgba (color0, color1, 0.5f);
    hb_color_t mid = hb_vector_svg_hb_color_from_rgba (mid_color);

    body->append_str ("<path d=\"M");
    body->append_இலக்கம் (center.x, precision);
    body->append_c (',');
    body->append_இலக்கம் (center.y, precision);
    body->append_str ("L");
    body->append_இலக்கம் (sp0.x, precision);
    body->append_c (',');
    body->append_இலக்கம் (sp0.y, precision);
    body->append_str ("C");
    body->append_இலக்கம் (sc0.x, precision);
    body->append_c (',');
    body->append_இலக்கம் (sc0.y, precision);
    body->append_c (' ');
    body->append_இலக்கம் (sc1.x, precision);
    body->append_c (',');
    body->append_இலக்கம் (sc1.y, precision);
    body->append_c (' ');
    body->append_இலக்கம் (sp1.x, precision);
    body->append_c (',');
    body->append_இலக்கம் (sp1.y, precision);
    body->append_str ("Z\" fill=\"");
    body->append_svg_color (mid, உண்மை);
    body->append_str ("\"/>\n");

    p0 = p1;
    color0 = color1;
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
கட்டமைப்பு hb_vector_svg_sweep_ctx_t
{
  hb_vector_buf_t *body;
  unsigned precision;
  தசம cx, cy, radius;
};

நிலையான வெறுமை
hb_vector_svg_sweep_emit_patch (தசம a0, hb_color_t c0,
				தசம a1, hb_color_t c1,
				வெறுமை *user_data)
{
  auto *ctx = (hb_vector_svg_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once
   * the buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  எனில் (unlikely (ctx->body->in_error ()))
    திரும்பு;
  hb_vector_svg_add_sweep_patch (ctx->body, ctx->precision,
				 ctx->cx, ctx->cy, ctx->radius,
				 a0, hb_vector_svg_rgba_from_hb_color (c0),
				 a1, hb_vector_svg_rgba_from_hb_color (c1));
}


நிலையான வெறுமை hb_vector_paint_தள்ளு_transform (hb_paint_funcs_t *, வெறுமை *,
                                            தசம, தசம, தசம, தசம, தசம, தசம,
                                            வெறுமை *);
நிலையான வெறுமை hb_vector_paint_எடு_transform (hb_paint_funcs_t *, வெறுமை *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_fill_glyph (hb_paint_funcs_t *, வெறுமை *, hb_codepoint_t, hb_font_t *, hb_bool_t, hb_color_t, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_தள்ளு_clip_glyph (hb_paint_funcs_t *, வெறுமை *, hb_codepoint_t, hb_font_t *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_தள்ளு_clip_rectangle (hb_paint_funcs_t *, வெறுமை *, தசம, தசம, தசம, தசம, வெறுமை *);
நிலையான hb_draw_funcs_t * hb_vector_paint_தள்ளு_clip_path_start (hb_paint_funcs_t *, வெறுமை *, வெறுமை **, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_தள்ளு_clip_path_end (hb_paint_funcs_t *, வெறுமை *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_எடு_clip (hb_paint_funcs_t *, வெறுமை *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_color (hb_paint_funcs_t *, வெறுமை *, hb_bool_t, hb_color_t, வெறுமை *);
நிலையான hb_bool_t hb_vector_paint_image (hb_paint_funcs_t *, வெறுமை *, hb_blob_t *, unsigned, unsigned, hb_tag_t, தசம, hb_glyph_extents_t *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_linear_gradient (hb_paint_funcs_t *, வெறுமை *, hb_color_வரி_t *, தசம, தசம, தசம, தசம, தசம, தசம, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_radial_gradient (hb_paint_funcs_t *, வெறுமை *, hb_color_வரி_t *, தசம, தசம, தசம, தசம, தசம, தசம, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_sweep_gradient (hb_paint_funcs_t *, வெறுமை *, hb_color_வரி_t *, தசம, தசம, தசம, தசம, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_தள்ளு_group (hb_paint_funcs_t *, வெறுமை *, வெறுமை *);
நிலையான வெறுமை hb_vector_paint_எடு_group (hb_paint_funcs_t *, வெறுமை *, hb_paint_composite_mode_t, வெறுமை *);
நிலையான hb_bool_t hb_vector_paint_color_glyph (hb_paint_funcs_t *, வெறுமை *, hb_codepoint_t, hb_font_t *, வெறுமை *);
நிலையான hb_bool_t
hb_vector_paint_custom_palette_color (hb_paint_funcs_t *pfuncs HB_UNUSED,
				      வெறுமை *paint_data,
				      unsigned color_index,
				      hb_color_t *color,
				      வெறுமை *user_data HB_UNUSED)
{
  hb_vector_paint_t *paint = (hb_vector_paint_t *) paint_data;
  எனில் (!color)
    திரும்பு பொய்;

  hb_color_t *value = nullptr;
  எனில் (!paint->custom_palette_colors.has (color_index, &value) || !value)
    திரும்பு பொய்;
  *color = *value;
  திரும்பு உண்மை;
}

நிலையான உள்வரி வெறுமை free_static_vector_paint_funcs ();
நிலையான கட்டமைப்பு hb_vector_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_vector_paint_funcs_lazy_loader_t>
{
  நிலையான hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_தள்ளு_transform_func (funcs, (hb_paint_தள்ளு_transform_func_t) hb_vector_paint_தள்ளு_transform, nullptr, nullptr);
    hb_paint_funcs_set_எடு_transform_func (funcs, (hb_paint_எடு_transform_func_t) hb_vector_paint_எடு_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_vector_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_glyph_func (funcs, (hb_paint_தள்ளு_clip_glyph_func_t) hb_vector_paint_தள்ளு_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_rectangle_func (funcs, (hb_paint_தள்ளு_clip_rectangle_func_t) hb_vector_paint_தள்ளு_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_path_start_func (funcs, (hb_paint_தள்ளு_clip_path_start_func_t) hb_vector_paint_தள்ளு_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_clip_path_end_func (funcs, (hb_paint_தள்ளு_clip_path_end_func_t) hb_vector_paint_தள்ளு_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_எடு_clip_func (funcs, (hb_paint_எடு_clip_func_t) hb_vector_paint_எடு_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_vector_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_vector_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_vector_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_vector_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_vector_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_தள்ளு_group_func (funcs, (hb_paint_தள்ளு_group_func_t) hb_vector_paint_தள்ளு_group, nullptr, nullptr);
    hb_paint_funcs_set_எடு_group_func (funcs, (hb_paint_எடு_group_func_t) hb_vector_paint_எடு_group, nullptr, nullptr);
    hb_paint_funcs_set_color_glyph_func (funcs, (hb_paint_color_glyph_func_t) hb_vector_paint_color_glyph, nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func (funcs, (hb_paint_custom_palette_color_func_t) hb_vector_paint_custom_palette_color, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_vector_paint_funcs);
    திரும்பு funcs;
  }
} static_vector_paint_funcs;

நிலையான உள்வரி வெறுமை
free_static_vector_paint_funcs ()
{
  static_vector_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_svg_funcs_get ()
{
  திரும்பு static_vector_paint_funcs.get_unconst ();
}


நிலையான வெறுமை
hb_vector_paint_தள்ளு_transform (hb_paint_funcs_t *,
                                வெறுமை *paint_data,
                                தசம xx, தசம yx,
                                தசம xy, தசம yy,
                                தசம dx, தசம dy,
                                வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  எனில் (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth++;
    திரும்பு;
  }
  எனில் (unlikely (paint->transform_group_depth >= 64))
  {
    paint->transform_group_overflow_depth = 1;
    திரும்பு;
  }

  hb_bool_t opened =
    !(fabsf (xx - 1.f) < 1e-6f && fabsf (yx) < 1e-6f &&
      fabsf (xy) < 1e-6f && fabsf (yy - 1.f) < 1e-6f &&
      fabsf (paint->sx (dx)) < 1e-6f && fabsf (paint->sy (dy)) < 1e-6f);
  paint->transform_group_open_mask = (paint->transform_group_open_mask << 1) | (opened ? 1ull : 0ull);
  paint->transform_group_depth++;

  எனில் (!opened)
    திரும்பு;

  auto &body = paint->current_body ();
  unsigned sprec = paint->defs.scale_precision ();
  body.append_str ("<g transform=\"matrix(");
  body.append_இலக்கம் (xx, sprec);
  body.append_c (',');
  body.append_இலக்கம் (yx, sprec);
  body.append_c (',');
  body.append_இலக்கம் (xy, sprec);
  body.append_c (',');
  body.append_இலக்கம் (yy, sprec);
  body.append_c (',');
  body.append_இலக்கம் (paint->sx (dx));
  body.append_c (',');
  body.append_இலக்கம் (paint->sy (dy));
  body.append_str (")\">\n");
}

நிலையான வெறுமை
hb_vector_paint_எடு_transform (hb_paint_funcs_t *,
                               வெறுமை *paint_data,
                               வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;
  எனில் (unlikely (paint->transform_group_overflow_depth))
  {
    paint->transform_group_overflow_depth--;
    திரும்பு;
  }
  எனில் (!paint->transform_group_depth)
    திரும்பு;
  paint->transform_group_depth--;
  hb_bool_t opened = !!(paint->transform_group_open_mask & 1ull);
  paint->transform_group_open_mask >>= 1;
  எனில் (opened)
    paint->current_body ().append_str ("</g>\n");
}

நிலையான வெறுமை
hb_vector_paint_fill_glyph (hb_paint_funcs_t *,
                            வெறுமை *paint_data,
                            hb_codepoint_t glyph,
                            hb_font_t *font,
                            hb_bool_t,
                            hb_color_t color,
                            வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  எனில் (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  auto &body = paint->current_body ();
  body.append_str ("<path d=\"");
  body.append_len (paint->path.arrayZ, paint->path.length);
  body.append_str ("\" fill=\"");
  body.append_svg_color (color, உண்மை);
  body.append_str ("\"/>\n");
}

நிலையான வெறுமை
hb_vector_paint_தள்ளு_clip_glyph (hb_paint_funcs_t *,
                                 வெறுமை *paint_data,
                                 hb_codepoint_t glyph,
                                 hb_font_t *font,
                                 வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  மாறிலி எழுத்து *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;

  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty clip path clips everything out. */
  எனில் (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph, hb_vector_svg_path_draw_funcs_get (), &sink);
  }

  unsigned def_id = paint->path_def_மொத்தம்++;
  paint->defs.append_str ("<path id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('p');
  paint->defs.append_unsigned (def_id);
  paint->defs.append_str ("\" d=\"");
  paint->defs.append_len (paint->path.arrayZ, paint->path.length);
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

நிலையான வெறுமை
hb_vector_paint_தள்ளு_clip_rectangle (hb_paint_funcs_t *,
                                     வெறுமை *paint_data,
                                     தசம xmin, தசம ymin,
                                     தசம xmax, தசம ymax,
                                     வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  மாறிலி எழுத்து *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_rect_counter++;
  paint->defs.append_str ("<clipPath id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_c ('c');
  paint->defs.append_unsigned (clip_id);
  paint->defs.append_str ("\"><rect x=\"");
  paint->defs.append_இலக்கம் (paint->sx (xmin));
  paint->defs.append_str ("\" y=\"");
  paint->defs.append_இலக்கம் (paint->sy (ymin));
  paint->defs.append_str ("\" width=\"");
  paint->defs.append_இலக்கம் (paint->sx (xmax - xmin));
  paint->defs.append_str ("\" height=\"");
  paint->defs.append_இலக்கம் (paint->sy (ymax - ymin));
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_c ('c');
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

நிலையான hb_draw_funcs_t *
hb_vector_paint_தள்ளு_clip_path_start (hb_paint_funcs_t *,
                                      வெறுமை *paint_data,
                                      வெறுமை **draw_data,
                                      வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    திரும்பு nullptr;
  }

  paint->path.clear ();
  paint->clip_path_sink = {&paint->path, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  திரும்பு hb_vector_svg_path_draw_funcs_get ();
}

நிலையான வெறுமை
hb_vector_paint_தள்ளு_clip_path_end (hb_paint_funcs_t *,
                                    வெறுமை *paint_data,
                                    வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  மாறிலி எழுத்து *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned clip_id = paint->clip_path_counter++;

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
  paint->defs.append_len (paint->path.arrayZ, paint->path.length);
  paint->defs.append_str ("\"/></clipPath>\n");

  paint->current_body ().append_str ("<g clip-path=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("cp");
  paint->current_body ().append_unsigned (clip_id);
  paint->current_body ().append_str (")\">\n");
}

நிலையான வெறுமை
hb_vector_paint_எடு_clip (hb_paint_funcs_t *,
                          வெறுமை *paint_data,
                          வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;
  paint->current_body ().append_str ("</g>\n");
}

நிலையான வெறுமை
hb_vector_paint_color (hb_paint_funcs_t *,
                       வெறுமை *paint_data,
                       hb_bool_t,
                       hb_color_t color,
                       வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  auto &body = paint->current_body ();
  body.append_str ("<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"");
  body.append_svg_color (color, உண்மை);
  body.append_str ("\"/>\n");
}

நிலையான hb_bool_t
hb_vector_paint_image (hb_paint_funcs_t *,
                       வெறுமை *paint_data,
                       hb_blob_t *image,
                       unsigned width,
                       unsigned height,
                       hb_tag_t format,
                       தசம slant HB_UNUSED,
                       hb_glyph_extents_t *extents,
                       வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு பொய்;

  auto &body = paint->current_body ();
  எனில் (format == HB_TAG ('p','n','g',' '))
  {
    எனில் (!extents || !width || !height)
      திரும்பு பொய்;

    unsigned len = 0;
    மாறிலி எழுத்து *png_data = hb_blob_get_data (image, &len);
    எனில் (!png_data || !len)
      திரும்பு பொய்;

    body.append_str ("<g transform=\"translate(");
    body.append_இலக்கம் (paint->sx ((தசம) extents->x_bearing));
    body.append_c (',');
    body.append_இலக்கம் (paint->sy ((தசம) extents->y_bearing));
    body.append_str (") scale(");
    body.append_இலக்கம் (paint->sx ((தசம) extents->width) / width);
    body.append_c (',');
    body.append_இலக்கம் (paint->sy ((தசம) extents->height) / height);
    body.append_str (")\">\n");

    body.append_str ("<image href=\"data:image/png;base64,");
    body.append_base64 ((மாறிலி uint8_t *) png_data, len);
    body.append_str ("\" width=\"");
    body.append_இலக்கம் ((தசம) width);
    body.append_str ("\" height=\"");
    body.append_இலக்கம் ((தசம) height);
    body.append_str ("\"/>\n</g>\n");

    திரும்பு உண்மை;
  }

  திரும்பு பொய்;
}

நிலையான வெறுமை
hb_vector_paint_linear_gradient (hb_paint_funcs_t *,
                                 வெறுமை *paint_data,
                                 hb_color_வரி_t *color_வரி,
                                 தசம x0, தசம y0,
                                 தசம x1, தசம y1,
                                 தசம x2, தசம y2,
                                 வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  எனில் (!paint->fetch_color_stops (color_வரி))
    திரும்பு;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  /* Sort + rescale stops to [0, 1]; shift the gradient axis
   * by the original (mn, mx) so the visible gradient stays
   * put.  SVG <stop offset="..."> requires offsets in [0,1];
   * out-of-range stops would otherwise be silently clamped. */
  தசம mn, mx;
  hb_paint_normalize_color_வரி (stops.arrayZ, stops.length, &mn, &mx);

  மாறிலி எழுத்து *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  /* Reduce COLR's 3-anchor (P0, P1, P2) to SVG's 2-point
   * (start, end) gradient. */
  தசம lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2,
				  &lx0, &ly0, &lx1, &ly1);
  தசம gx0 = lx0 + mn * (lx1 - lx0);
  தசம gy0 = ly0 + mn * (ly1 - ly0);
  தசம gx1 = lx0 + mx * (lx1 - lx0);
  தசம gy1 = ly0 + mx * (ly1 - ly0);

  paint->defs.append_str ("<linearGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" x1=\"");
  paint->defs.append_இலக்கம் (paint->sx (gx0));
  paint->defs.append_str ("\" y1=\"");
  paint->defs.append_இலக்கம் (paint->sy (gy0));
  paint->defs.append_str ("\" x2=\"");
  paint->defs.append_இலக்கம் (paint->sx (gx1));
  paint->defs.append_str ("\" y2=\"");
  paint->defs.append_இலக்கம் (paint->sy (gy1));
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_வரி_get_extend (color_வரி)));
  paint->defs.append_str ("\">\n");
  hb_vector_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</linearGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

நிலையான வெறுமை
hb_vector_paint_radial_gradient (hb_paint_funcs_t *,
                                 வெறுமை *paint_data,
                                 hb_color_வரி_t *color_வரி,
                                 தசம x0, தசம y0, தசம r0,
                                 தசம x1, தசம y1, தசம r1,
                                 வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  எனில் (!paint->fetch_color_stops (color_வரி))
    திரும்பு;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  தசம mn, mx;
  hb_paint_normalize_color_வரி (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift centers + radii by (mn, mx) along the gradient axis
   * to compensate for rescaling stops to [0, 1]. */
  தசம gx0 = x0 + mn * (x1 - x0);
  தசம gy0 = y0 + mn * (y1 - y0);
  தசம gr0 = r0 + mn * (r1 - r0);
  தசம gx1 = x0 + mx * (x1 - x0);
  தசம gy1 = y0 + mx * (y1 - y0);
  தசம gr1 = r0 + mx * (r1 - r0);

  மாறிலி எழுத்து *pfx = paint->id_prefix;
  unsigned pfx_len = paint->id_prefix_length;
  unsigned grad_id = paint->gradient_counter++;

  paint->defs.append_str ("<radialGradient id=\"");
  paint->defs.append_len (pfx, pfx_len);
  paint->defs.append_str ("gr");
  paint->defs.append_unsigned (grad_id);
  paint->defs.append_str ("\" gradientUnits=\"userSpaceOnUse\" cx=\"");
  paint->defs.append_இலக்கம் (paint->sx (gx1));
  paint->defs.append_str ("\" cy=\"");
  paint->defs.append_இலக்கம் (paint->sy (gy1));
  paint->defs.append_str ("\" r=\"");
  paint->defs.append_இலக்கம் (paint->sx (gr1));
  paint->defs.append_str ("\" fx=\"");
  paint->defs.append_இலக்கம் (paint->sx (gx0));
  paint->defs.append_str ("\" fy=\"");
  paint->defs.append_இலக்கம் (paint->sy (gy0));
  எனில் (gr0 > 0)
  {
    paint->defs.append_str ("\" fr=\"");
    paint->defs.append_இலக்கம் (paint->sx (gr0));
  }
  paint->defs.append_str ("\" spreadMethod=\"");
  paint->defs.append_str (hb_vector_svg_extend_mode_str (hb_color_வரி_get_extend (color_வரி)));
  paint->defs.append_str ("\">\n");
  hb_vector_svg_emit_color_stops (paint, &paint->defs, &stops);
  paint->defs.append_str ("</radialGradient>\n");

  paint->current_body ().append_str (
                     "<rect x=\"-32767\" y=\"-32767\" width=\"65534\" height=\"65534\" fill=\"url(#");
  paint->current_body ().append_len (pfx, pfx_len);
  paint->current_body ().append_str ("gr");
  paint->current_body ().append_unsigned (grad_id);
  paint->current_body ().append_str (")\"/>\n");
}

நிலையான வெறுமை
hb_vector_paint_sweep_gradient (hb_paint_funcs_t *,
                                வெறுமை *paint_data,
                                hb_color_வரி_t *color_வரி,
                                தசம cx, தசம cy,
                                தசம start_angle, தசம end_angle,
                                வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;

  எனில் (!paint->fetch_color_stops (color_வரி))
    திரும்பு;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  தசம mn, mx;
  hb_paint_normalize_color_வரி (stops.arrayZ, stops.length, &mn, &mx);

  /* Shift the angle range to compensate for rescaling stops
   * to [0, 1]. */
  தசம ga0 = start_angle + mn * (end_angle - start_angle);
  தசம ga1 = start_angle + mx * (end_angle - start_angle);

  hb_vector_svg_sweep_ctx_t ctx {
    &paint->current_body (), paint->get_precision (), paint->sx (cx), paint->sy (cy), 32767.f
  };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length,
				 hb_color_வரி_get_extend (color_வரி),
				 ga0, ga1,
				 hb_vector_svg_sweep_emit_patch,
				 &ctx);
}

நிலையான வெறுமை
hb_vector_paint_தள்ளு_group (hb_paint_funcs_t *,
                            வெறுமை *paint_data,
                            வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;
  எனில் (unlikely (!paint->group_அடுக்கு.தள்ளு_or_fail (hb_vector_buf_t {})))
    திரும்பு;
}

நிலையான வெறுமை
hb_vector_paint_எடு_group (hb_paint_funcs_t *,
                           வெறுமை *paint_data,
                           hb_paint_composite_mode_t mode,
                           வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு;
  எனில் (paint->group_அடுக்கு.length < 2)
    திரும்பு;

  hb_vector_buf_t group = paint->group_அடுக்கு.எடு ();
  auto &body = paint->current_body ();

  மாறிலி எழுத்து *blend = hb_vector_svg_composite_mode_str (mode);
  எனில் (blend)
  {
    body.append_str ("<g style=\"mix-blend-mode:");
    body.append_str (blend);
    body.append_str ("\">\n");
    body.append_len (group.arrayZ, group.length);
    body.append_str ("</g>\n");
  }
  இல்லையெனில்
    body.append_len (group.arrayZ, group.length);
}

நிலையான hb_bool_t
hb_vector_paint_color_glyph (hb_paint_funcs_t *,
                             வெறுமை *paint_data,
                             hb_codepoint_t glyph,
                             hb_font_t *font,
                             வெறுமை *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு பொய்;
  எனில் (unlikely (paint->color_glyph_depth >= HB_MAX_NESTING_LEVEL))
    திரும்பு பொய்;
  எனில் (unlikely (hb_set_has (paint->active_color_glyphs, glyph)))
    திரும்பு பொய்;

  paint->color_glyph_depth++;
  hb_set_add (paint->active_color_glyphs, glyph);

  hb_font_paint_glyph (font, glyph,
		       hb_vector_paint_svg_funcs_get (),
		       paint,
		       paint->palette,
		       paint->foreground);
  hb_set_del (paint->active_color_glyphs, glyph);
  paint->color_glyph_depth--;
  திரும்பு உண்மை;
}




hb_blob_t *
hb_vector_paint_render_svg (hb_vector_paint_t *paint)
{
  எனில் (!paint->has_extents)
    திரும்பு nullptr;

  எனில் (unlikely (!paint->ensure_initialized ()))
    திரும்பு nullptr;

  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  unsigned estimated = paint->defs.length +
		       paint->group_அடுக்கு.arrayZ[0].length +
		       320;
  out.alloc (estimated);
  தசம vb_x = paint->extents.x;
  தசம vb_y = -(paint->extents.y + paint->extents.height);
  தசம vb_w = paint->extents.width;
  தசம vb_h = paint->extents.height;
  out.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"");
  out.append_இலக்கம் (vb_x);
  out.append_c (' ');
  out.append_இலக்கம் (vb_y);
  out.append_c (' ');
  out.append_இலக்கம் (vb_w);
  out.append_c (' ');
  out.append_இலக்கம் (vb_h);
  out.append_str ("\" width=\"");
  out.append_இலக்கம் (vb_w);
  out.append_str ("\" height=\"");
  out.append_இலக்கம் (vb_h);
  out.append_str ("\">\n");

  எனில் (paint->defs.length)
  {
    out.append_str ("<defs>\n");
    out.append_len (paint->defs.arrayZ, paint->defs.length);
    out.append_str ("</defs>\n");
  }

  எனில் (hb_color_get_alpha (paint->background))
  {
    out.append_str ("<rect x=\"");
    out.append_இலக்கம் (vb_x);
    out.append_str ("\" y=\"");
    out.append_இலக்கம் (vb_y);
    out.append_str ("\" width=\"");
    out.append_இலக்கம் (vb_w);
    out.append_str ("\" height=\"");
    out.append_இலக்கம் (vb_h);
    out.append_str ("\" fill=\"rgb(");
    out.append_unsigned (hb_color_get_red (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_green (paint->background));
    out.append_c (',');
    out.append_unsigned (hb_color_get_blue (paint->background));
    out.append_str (")\"");
    எனில் (hb_color_get_alpha (paint->background) < 255)
    {
      out.append_str (" fill-opacity=\"");
      out.append_இலக்கம் (hb_color_get_alpha (paint->background) / 255.f, 4);
      out.append_c ('"');
    }
    out.append_str ("/>\n");
  }

  out.append_str ("<g transform=\"scale(1,-1)\">\n");
  out.append_len (paint->group_அடுக்கு.arrayZ[0].arrayZ, paint->group_அடுக்கு.arrayZ[0].length);
  out.append_str ("</g>\n");

  out.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  திரும்பு blob;
}
