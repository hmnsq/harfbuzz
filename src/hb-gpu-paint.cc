/*
 * Copyright (C) 2026  Behdad Esfahbod
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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"


/* ---- Paint callbacks ---- */

/* Op-type tags.  Must match the layout documented in
 * hb-gpu-paint.hh. */
ਗਿਣਤੀ {
  HB_GPU_PAINT_OP_LAYER_SOLID    = 0,
  HB_GPU_PAINT_OP_LAYER_GRADIENT = 1,
  HB_GPU_PAINT_OP_ਧੱਕੋ_ਸਮੂਹ     = 2,
  HB_GPU_PAINT_OP_ਕੱਢੋ_ਸਮੂਹ      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_MAX_ਸਮੂਹ_ਡੂੰਘਾਈ 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ  3      /* shader can intersect up to 3 clip outlines per layer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

ਸਥਿਰ hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				   ਖਾਲੀ             *paint_ਅੰਕੜੇ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ          color_ਸੂਚਕਾਂਕ,
				   hb_color_t       *color,
				   ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_ਸੂਚਕਾਂਕ)))
  {
    *color = hb_map_get (c->custom_palette, color_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;

  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ >= HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ))
  {
    c->unsupported = ਸੱਚ;
    c->clip_ਡੂੰਘਾਈ++;
    ਵਾਪਸ;
  }
  c->clip_ਢੇਰ[c->clip_ਡੂੰਘਾਈ] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_ਡੂੰਘਾਈ++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
		       ਖਾਲੀ             *paint_ਅੰਕੜੇ,
		       ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (likely (c->clip_ਡੂੰਘਾਈ > 0)) c->clip_ਡੂੰਘਾਈ--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
ਸਥਿਰ hb_draw_funcs_t *
hb_gpu_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				   ਖਾਲੀ             *paint_ਅੰਕੜੇ,
				   ਖਾਲੀ            **draw_ਅੰਕੜੇ,
				   ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;

  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ >= HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ))
  {
    c->unsupported = ਸੱਚ;
    *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ਜੇ (unlikely (!c->scratch_draw))
    {
      c->unsupported = ਸੱਚ;
      *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_ਸੈੱਟ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_ਬਚਿਆ);
  hb_gpu_draw_ਸੈੱਟ_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_ਰਸਤਾ_transform = c->cur_transform;
  c->pending_clip_ਰਸਤਾ = ਸੱਚ;

  *draw_ਅੰਕੜੇ = c->scratch_draw;
  ਵਾਪਸ hb_gpu_draw_get_funcs (c->scratch_draw);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
				 ਖਾਲੀ             *paint_ਅੰਕੜੇ,
				 ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;

  ਜੇ (unlikely (!c->pending_clip_ਰਸਤਾ))
    ਵਾਪਸ;
  c->pending_clip_ਰਸਤਾ = ਝੂਠ;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_ਬਚਿਆ = hb_draw_get_budget_ਬਚਿਆ (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ਜੇ (unlikely (!blob || !c->ਧੱਕੋ_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  ਪੂਰਨ idx = (ਪੂਰਨ) (c->sub_blobs.ਲੰਮਾਈ - 1);

  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ >= HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ))
  {
    c->unsupported = ਸੱਚ;
    c->clip_ਡੂੰਘਾਈ++;
    ਵਾਪਸ;
  }

  ਪੂਰਨ x0 = ext.x_bearing;
  ਪੂਰਨ x1 = hb_saturate_ਜੋੜੋ (ext.x_bearing, ext.ਚੌੜਾਈ);
  ਪੂਰਨ y0 = ext.y_bearing;
  ਪੂਰਨ y1 = hb_saturate_ਜੋੜੋ (ext.y_bearing, ext.ਉਚਾਈ);

  c->clip_ਢੇਰ[c->clip_ਡੂੰਘਾਈ] = {
    HB_CODEPOINT_INVALID, ਖਾਲੀ_ਸੂਚਕ,
    c->pending_clip_ਰਸਤਾ_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_ਡੂੰਘਾਈ++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			     ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
			     ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
			     ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
			     ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->transform_ਢੇਰ.ਧੱਕੋ_or_fail (c->cur_transform)))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  hb_transform_t<ਦਸ਼ਮਲਵ> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			    ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			    ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!c->transform_ਢੇਰ.ਲੰਮਾਈ))
    ਵਾਪਸ;
  c->cur_transform = c->transform_ਢੇਰ.arrayZ[c->transform_ਢੇਰ.ਲੰਮਾਈ - 1];
  c->transform_ਢੇਰ.shrink (c->transform_ਢੇਰ.ਲੰਮਾਈ - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
ਸਥਿਰ ਖਾਲੀ
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  ਜੇ (unlikely (c->ਅੰਕ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  ਜੇ (unlikely (!c->ops.resize (c->ops.ਲੰਮਾਈ + 4)))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.ਲੰਮਾਈ - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->ਅੰਕ_ops++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			 ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			 ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  c->ਸਮੂਹ_ਡੂੰਘਾਈ++;
  ਜੇ (unlikely (c->ਸਮੂਹ_ਡੂੰਘਾਈ > HB_GPU_PAINT_MAX_ਸਮੂਹ_ਡੂੰਘਾਈ))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_ਧੱਕੋ_ਸਮੂਹ, 0);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t    *funcs HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ                *paint_ਅੰਕੜੇ,
			hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
			ਖਾਲੀ                *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  ਤਾਰਕਿਕ was_ਵਿੱਚ_range = c->ਸਮੂਹ_ਡੂੰਘਾਈ <= HB_GPU_PAINT_MAX_ਸਮੂਹ_ਡੂੰਘਾਈ;
  ਜੇ (likely (c->ਸਮੂਹ_ਡੂੰਘਾਈ > 0)) c->ਸਮੂਹ_ਡੂੰਘਾਈ--;
  ਜੇ (was_ਵਿੱਚ_range)
    emit_control_op (c, HB_GPU_PAINT_OP_ਕੱਢੋ_ਸਮੂਹ, (int16_t) ਬਹੁਲਕ);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
color_ਮੰਜ਼ਿਲ_q15 (ਬਿਨਾਨਿਸ਼ਾਨੀ byte_ਮੁੱਲ)
{
  ਵਾਪਸ (int16_t) ((byte_ਮੁੱਲ * 32767u + 127u) / 255u);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
color_ਮੰਜ਼ਿਲ_q15_rgba (hb_color_t color, int16_t ਬਾਹਰਲਾ[4])
{
  ਬਾਹਰਲਾ[0] = color_ਮੰਜ਼ਿਲ_q15 (hb_color_get_red   (color));
  ਬਾਹਰਲਾ[1] = color_ਮੰਜ਼ਿਲ_q15 (hb_color_get_green (color));
  ਬਾਹਰਲਾ[2] = color_ਮੰਜ਼ਿਲ_q15 (hb_color_get_blue  (color));
  ਬਾਹਰਲਾ[3] = color_ਮੰਜ਼ਿਲ_q15 (hb_color_get_alpha (color));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
clamp_i16 (ਦਸ਼ਮਲਵ v)
{
  ਜੇ (v <= -32768.f) ਵਾਪਸ -32768;
  ਜੇ (v >=  32767.f) ਵਾਪਸ  32767;
  ਜੇ (unlikely (std::isnan (v))) ਵਾਪਸ 0;
  ਵਾਪਸ (int16_t) v;
}

/* Compute the 2x2 inverse of the transform's linear part, quantize
 * each entry to i16 Q10 (multiplier 1024) and write row-major into
 * @out: (m00, m01, m10, m11) such that applying it gives
 *   (M^-1 * v).x = m00 * v.x + m01 * v.y
 *   (M^-1 * v).y = m10 * v.x + m11 * v.y
 * Range [-32, 32), precision ~0.001 -- covers typical COLRv1
 * PaintTransform values with headroom.  Callers who need to skip
 * transforms for degenerate cases can check for is_identity before
 * doing the extra work in-shader, but we always emit so the format
 * stays uniform. */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
linv_q10 (ਅਚਲ hb_transform_t<ਦਸ਼ਮਲਵ> &t, int16_t ਬਾਹਰਲਾ[4])
{
  ਦਸ਼ਮਲਵ det = t.xx * t.yy - t.xy * t.yx;
  ਦਸ਼ਮਲਵ inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  ਬਾਹਰਲਾ[0] = clamp_i16 ( t.yy * inv * 1024.f);
  ਬਾਹਰਲਾ[1] = clamp_i16 (-t.xy * inv * 1024.f);
  ਬਾਹਰਲਾ[2] = clamp_i16 (-t.yx * inv * 1024.f);
  ਬਾਹਰਲਾ[3] = clamp_i16 ( t.xx * inv * 1024.f);
}

/* Transforming pen: applies an affine transform to each outline
 * point before forwarding to a downstream hb_draw_funcs_t.  The
 * downstream's inline draw-state bookkeeping runs against a
 * separate state (down_st) so transformed-space updates don't
 * pollute the caller's pre-transform state. */
ਬਣਤਰ hb_gpu_paint_pen_t
{
  hb_transform_t<ਦਸ਼ਮਲਵ> transform;
  hb_draw_funcs_t  *dfuncs;
  ਖਾਲੀ             *ਅੰਕੜੇ;
  hb_draw_state_t   ਥੱਲੇ_st;
  int64_t          *budget_ਬਚਿਆ;
};

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_pen_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *ਅੰਕੜੇ, hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਸਵੈਚਾਲਿਤ *c = (hb_gpu_paint_pen_t *) ਅੰਕੜੇ;
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  c->dfuncs->move_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, c->ਥੱਲੇ_st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_pen_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *ਅੰਕੜੇ, hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਸਵੈਚਾਲਿਤ *c = (hb_gpu_paint_pen_t *) ਅੰਕੜੇ;
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  c->dfuncs->ਸਤਰ_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, c->ਥੱਲੇ_st, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_pen_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			       ਖਾਲੀ *ਅੰਕੜੇ, hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			       ਦਸ਼ਮਲਵ c_x, ਦਸ਼ਮਲਵ c_y,
			       ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			       ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਸਵੈਚਾਲਿਤ *c = (hb_gpu_paint_pen_t *) ਅੰਕੜੇ;
  c->transform.transform_ਬਿੰਦੂ (c_x, c_y);
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  c->dfuncs->quadratic_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, c->ਥੱਲੇ_st, c_x, c_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_pen_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			   ਖਾਲੀ *ਅੰਕੜੇ, hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			   ਦਸ਼ਮਲਵ c1_x, ਦਸ਼ਮਲਵ c1_y,
			   ਦਸ਼ਮਲਵ c2_x, ਦਸ਼ਮਲਵ c2_y,
			   ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			   ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਸਵੈਚਾਲਿਤ *c = (hb_gpu_paint_pen_t *) ਅੰਕੜੇ;
  c->transform.transform_ਬਿੰਦੂ (c1_x, c1_y);
  c->transform.transform_ਬਿੰਦੂ (c2_x, c2_y);
  c->transform.transform_ਬਿੰਦੂ (ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
  c->dfuncs->cubic_ਮੰਜ਼ਿਲ (c->ਅੰਕੜੇ, c->ਥੱਲੇ_st, c1_x, c1_y, c2_x, c2_y, ਮੰਜ਼ਿਲ_x, ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_pen_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *ਅੰਕੜੇ, hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਸਵੈਚਾਲਿਤ *c = (hb_gpu_paint_pen_t *) ਅੰਕੜੇ;
  c->dfuncs->close_ਰਸਤਾ (c->ਅੰਕੜੇ, c->ਥੱਲੇ_st);
}

ਸਥਿਰ int64_t *
hb_gpu_paint_pen_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_gpu_paint_pen_t *) draw_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_gpu_paint_pen_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func      (funcs, hb_gpu_paint_pen_move_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func      (funcs, hb_gpu_paint_pen_ਸਤਰ_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_gpu_paint_pen_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func     (funcs, hb_gpu_paint_pen_cubic_ਮੰਜ਼ਿਲ,     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func   (funcs, hb_gpu_paint_pen_close_ਰਸਤਾ,   ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_gpu_paint_pen_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_gpu_paint_pen_funcs);
    ਵਾਪਸ funcs;
  }
} static_gpu_paint_pen_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_gpu_paint_pen_funcs ()
{
  static_gpu_paint_pen_funcs.free_instance ();
}

/* Rasterize @clip's glyph outline into a new sub-blob and
 * accumulate its extents.  Returns the sub_blob index (>= 0) on
 * success, or -1 if the outline is empty / the layer should be
 * skipped.  Sets c->unsupported on hard failure.  On the first
 * successful glyph-clip encode, memoizes the sub-blob index and
 * extents back into @clip; subsequent calls hit the cache fast
 * path.  This matters for adversarial COLR trees that emit many
 * paint layers under the same clip stack -- without memoization
 * each layer re-rasterizes every clip glyph. */
ਸਥਿਰ ਪੂਰਨ
emit_clip_sub_blob (hb_gpu_paint_t *c,
		    hb_gpu_paint_t::pending_clip_t &clip)
{
  /* Path clips were already encoded into sub_blobs at
   * push_clip_path_end time; glyph clips are cached here on first
   * use.  Either way: just accumulate extents and hand back the
   * recorded index. */
  ਜੇ (clip.sub_blob_ਸੂਚਕਾਂਕ >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    ਵਾਪਸ clip.sub_blob_ਸੂਚਕਾਂਕ;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  ਜੇ (unlikely (c->sub_blobs.ਲੰਮਾਈ >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ਜੇ (unlikely (c->budget_ਬਚਿਆ < 0))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ -1;
  }

  ਜੇ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ਜੇ (unlikely (!c->scratch_draw))
    {
      c->unsupported = ਸੱਚ;
      ਵਾਪਸ -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_ਸੈੱਟ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_ਬਚਿਆ);

  ਤਾਰਕਿਕ ok;
  ਜੇ (clip.transform.is_ਪਛਾਣ_ਤੱਤ ())
  {
    /* Fast path: feed the glyph outline straight into the draw
     * encoder with no adapter. */
    ok = hb_gpu_draw_glyph_or_fail (c->scratch_draw, clip.font, clip.glyph);
  }
  ਨਹੀਂਤਾਂ
  {
    /* Transform each outline point by the transform that was
     * current at push_clip_glyph time -- NOT the innermost
     * cur_transform, which may have additional PaintTransform
     * wrappers from the paint child underneath. */
    ਪੂਰਨ x_scale, y_scale;
    hb_font_get_scale (clip.font, &x_scale, &y_scale);
    hb_gpu_draw_ਸੈੱਟ_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.ਅੰਕੜੇ      = c->scratch_draw;
    pen.ਥੱਲੇ_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_ਬਚਿਆ = &c->scratch_draw->budget_ਬਚਿਆ;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_ਰਸਤਾ (pen.ਅੰਕੜੇ, pen.ਥੱਲੇ_st);
  }
  c->budget_ਬਚਿਆ = hb_draw_get_budget_ਬਚਿਆ (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  ਜੇ (!ok)
    ਵਾਪਸ -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ਜੇ (unlikely (!blob || !c->ਧੱਕੋ_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  ਪੂਰਨ x0 = ext.x_bearing;
  ਪੂਰਨ x1 = hb_saturate_ਜੋੜੋ (ext.x_bearing, ext.ਚੌੜਾਈ);
  ਪੂਰਨ y0 = ext.y_bearing;
  ਪੂਰਨ y1 = hb_saturate_ਜੋੜੋ (ext.y_bearing, ext.ਉਚਾਈ);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  ਪੂਰਨ idx = (ਪੂਰਨ) (c->sub_blobs.ਲੰਮਾਈ - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_ਸੂਚਕਾਂਕ = idx;
  clip.ext_x0 = hb_min (x0, x1);
  clip.ext_y0 = hb_min (y0, y1);
  clip.ext_x1 = hb_max (x0, x1);
  clip.ext_y1 = hb_max (y0, y1);

  ਵਾਪਸ idx;
}

/* Emit a sub-blob for every clip currently on the stack and return
 * their indices in @out (set to -1 for unused slots).  Returns
 * false if any clip's outline is empty (caller should skip the
 * layer entirely) or hb_gpu_draw_encode failed. */
ਸਥਿਰ ਤਾਰਕਿਕ
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, ਪੂਰਨ ਬਾਹਰਲਾ[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ])
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ; i++)
    ਬਾਹਰਲਾ[i] = -1;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < hb_min (c->clip_ਡੂੰਘਾਈ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ); i++)
  {
    ਬਾਹਰਲਾ[i] = emit_clip_sub_blob (c, c->clip_ਢੇਰ[i]);
    ਜੇ (ਬਾਹਰਲਾ[i] < 0)
      ਵਾਪਸ ਝੂਠ;
  }
  ਵਾਪਸ ਸੱਚ;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
ਸਥਿਰ int16_t *
emit_gradient_op_ਸਿਰਲੇਖ (hb_gpu_paint_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ subtype,
			 ਅਚਲ ਪੂਰਨ clip_idx[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ])
{
  ਜੇ (unlikely (!c->ops.resize (c->ops.ਲੰਮਾਈ + 12)))
  { c->unsupported = ਸੱਚ; ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; }
  int16_t *o = &c->ops.arrayZ[c->ops.ਲੰਮਾਈ - 12];
  int16_t flags = (int16_t) subtype;
  ਜੇ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ਜੇ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_GRADIENT;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  ਵਾਪਸ o + 8;  /* texel 2: gradient meta -- caller fills */
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_emit_solid (hb_gpu_paint_t *c,
			 hb_bool_t       is_foreground,
			 hb_color_t      color)
{
  ਜੇ (unlikely (c->unsupported))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ == 0))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->ਅੰਕ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }

  ਪੂਰਨ clip_idx[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ];
  ਜੇ (!emit_all_clip_sub_blobs (c, clip_idx))
    ਵਾਪਸ;

  /* Emit LAYER_SOLID op: 3 texels (12 int16s).
   *   texel 0: op_type, flags, clip1_hi, clip1_lo
   *   texel 1: clip2_hi, clip2_lo, 0, 0  (only meaningful if HAS_CLIP2)
   *   texel 2: r_q15, g_q15, b_q15, a_q15
   * Payloads hold sub_blob indices; hb_gpu_paint_encode() patches
   * them into texel offsets at serialize time.
   * flags bit 0: use shader foreground uniform instead of the
   * baked color (so runtime foreground-color changes still work).
   * flags bit 8: a second clip outline is present in texel 1. */
  ਜੇ (unlikely (!c->ops.resize (c->ops.ਲੰਮਾਈ + 12)))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.ਲੰਮਾਈ - 12];
  int16_t flags = (int16_t) (is_foreground ? HB_GPU_PAINT_FLAG_IS_FOREGROUND : 0);
  ਜੇ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ਜੇ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_SOLID;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  color_ਮੰਜ਼ਿਲ_q15_rgba (color, o + 8);
  c->ਅੰਕ_ops++;
}

/* Append a color line (@count stops) to @grad_data in the encoded
 * stop format used by gradient ops: 2 texels per stop.
 *   texel a: (offset_q15, flags, _, _) with flags bit 0 = is_foreground
 *   texel b: (r_q15, g_q15, b_q15, a_q15)
 * Returns false on allocation failure. */
ਸਥਿਰ ਤਾਰਕਿਕ
append_color_stops (hb_ਸਦਿਸ਼_t<int16_t> &grad_ਅੰਕੜੇ,
		    ਅਚਲ hb_color_stop_t *stops,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਧਾਰ = grad_ਅੰਕੜੇ.ਲੰਮਾਈ;
  ਜੇ (unlikely (!grad_ਅੰਕੜੇ.resize (ਅਧਾਰ + ਗਿਣਤੀ_2 * 8)))
    ਵਾਪਸ ਝੂਠ;
  int16_t *p = grad_ਅੰਕੜੇ.arrayZ + ਅਧਾਰ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
  {
    ਦਸ਼ਮਲਵ off = stops[i].ਵਿੱਥ;
    ਜੇ (off < -1.f) off = -1.f;
    ਨਹੀਂਤਾਂ ਜੇ (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_ਮੰਜ਼ਿਲ_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_emit_ਰੇਖਿਕ (hb_gpu_paint_t  *c,
			  hb_color_ਸਤਰ_t *color_ਸਤਰ,
			  ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			  ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			  ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2)
{
  ਜੇ (unlikely (c->unsupported))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ == 0))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->ਅੰਕ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  ਜੇ (unlikely (!c->fetch_color_stops (color_ਸਤਰ)))
    ਵਾਪਸ;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ got = c->color_stops_scratch.ਲੰਮਾਈ;

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);

  /* Emit clip sub-blob(s) first. */
  ਪੂਰਨ clip_idx[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ];
  ਜੇ (!emit_all_clip_sub_blobs (c, clip_idx)) ਵਾਪਸ;

  /* Build gradient params sub-blob.
   *   texel 0: (p0_rendered_x, p0_rendered_y, d_canonical_x, d_canonical_y)
   *     i16 font units; d = p1 - p0 stored in untransformed space
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * The shader computes p_canonical = L^-1 * (renderCoord - p0_rendered)
   * and evaluates t = dot(p_canonical, d) / dot(d, d) in
   * untransformed space, so affine distortions of the axis are
   * handled exactly rather than approximated. */
  hb_ਸਦਿਸ਼_t<int16_t> grad_ਅੰਕੜੇ;
  ਜੇ (unlikely (!grad_ਅੰਕੜੇ.resize (8)))
  { c->unsupported = ਸੱਚ; ਵਾਪਸ; }

  ਦਸ਼ਮਲਵ lx0, ly0, lx1, ly1;
  hb_paint_reduce_ਰੇਖਿਕ_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, got, &mn, &mx);
  ਦਸ਼ਮਲਵ dx = lx1 - lx0, dy = ly1 - ly0;
  ਦਸ਼ਮਲਵ ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  ਦਸ਼ਮਲਵ ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  ਦਸ਼ਮਲਵ p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_ਬਿੰਦੂ (p0x_r, p0y_r);
  grad_ਅੰਕੜੇ[0] = clamp_i16 (p0x_r);
  grad_ਅੰਕੜੇ[1] = clamp_i16 (p0y_r);
  grad_ਅੰਕੜੇ[2] = clamp_i16 (ax1 - ax0);
  grad_ਅੰਕੜੇ[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ਅੰਕੜੇ[4] = Linv[0];
  grad_ਅੰਕੜੇ[5] = Linv[1];
  grad_ਅੰਕੜੇ[6] = Linv[2];
  grad_ਅੰਕੜੇ[7] = Linv[3];

  ਜੇ (unlikely (!append_color_stops (grad_ਅੰਕੜੇ, stops, got)))
  { c->unsupported = ਸੱਚ; ਵਾਪਸ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_ਬਾਈਟਾਂ = grad_ਅੰਕੜੇ.ਲੰਮਾਈ * ਆਕਾਰ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) grad_ਅੰਕੜੇ.arrayZ,
					 grad_ਬਾਈਟਾਂ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
					 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!grad_blob || !c->ਧੱਕੋ_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_idx = c->sub_blobs.ਲੰਮਾਈ - 1;

  int16_t *meta = emit_gradient_op_ਸਿਰਲੇਖ (c, 0 /*ਰੇਖਿਕ*/, clip_idx);
  ਜੇ (unlikely (!meta)) ਵਾਪਸ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ਅੰਕ_ops++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_ਸਤਰ_t *color_ਸਤਰ,
			  ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
			  ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1)
{
  ਜੇ (unlikely (c->unsupported))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ == 0))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->ਅੰਕ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }

  ਜੇ (unlikely (!c->fetch_color_stops (color_ਸਤਰ)))
    ਵਾਪਸ;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ got = c->color_stops_scratch.ਲੰਮਾਈ;

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);

  ਪੂਰਨ clip_idx[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ];
  ਜੇ (!emit_all_clip_sub_blobs (c, clip_idx)) ਵਾਪਸ;

  /* Build gradient params sub-blob.
   *   texel 0: (c0_rendered_x, c0_rendered_y, d_canonical_x, d_canonical_y)
   *     where d = c1 - c0 in untransformed space
   *   texel 1: (r0, r1, _, _) in untransformed font units (i16)
   *   texel 2: L^-1 as 4 i16 Q10 (row-major)
   *   texels 3..N: color stops (2 texels each)
   * Shader computes p_canonical = L^-1 * (renderCoord - c0_rendered)
   * then solves |p - t*d|^2 = (r0 + t*(r1-r0))^2 in the
   * untransformed frame, so non-uniform scale / shear (which would
   * turn the circle into an ellipse in rendered space) is handled
   * exactly.
   */
  hb_ਸਦਿਸ਼_t<int16_t> grad_ਅੰਕੜੇ;
  ਜੇ (unlikely (!grad_ਅੰਕੜੇ.resize (12)))
  { c->unsupported = ਸੱਚ; ਵਾਪਸ; }
  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, got, &mn, &mx);
  ਦਸ਼ਮਲਵ dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  ਦਸ਼ਮਲਵ cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  ਦਸ਼ਮਲਵ cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  ਦਸ਼ਮਲਵ cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_ਬਿੰਦੂ (cx0_r, cy0_r);
  grad_ਅੰਕੜੇ[0] = clamp_i16 (cx0_r);
  grad_ਅੰਕੜੇ[1] = clamp_i16 (cy0_r);
  grad_ਅੰਕੜੇ[2] = clamp_i16 (cx1 - cx0);
  grad_ਅੰਕੜੇ[3] = clamp_i16 (cy1 - cy0);
  grad_ਅੰਕੜੇ[4] = clamp_i16 (cr0);
  grad_ਅੰਕੜੇ[5] = clamp_i16 (cr1);
  grad_ਅੰਕੜੇ[6] = 0;
  grad_ਅੰਕੜੇ[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ਅੰਕੜੇ[8]  = Linv[0];
  grad_ਅੰਕੜੇ[9]  = Linv[1];
  grad_ਅੰਕੜੇ[10] = Linv[2];
  grad_ਅੰਕੜੇ[11] = Linv[3];

  ਜੇ (unlikely (!append_color_stops (grad_ਅੰਕੜੇ, stops, got)))
  { c->unsupported = ਸੱਚ; ਵਾਪਸ; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_ਬਾਈਟਾਂ = grad_ਅੰਕੜੇ.ਲੰਮਾਈ * ਆਕਾਰ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) grad_ਅੰਕੜੇ.arrayZ,
					 grad_ਬਾਈਟਾਂ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
					 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!grad_blob || !c->ਧੱਕੋ_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_idx = c->sub_blobs.ਲੰਮਾਈ - 1;

  int16_t *meta = emit_gradient_op_ਸਿਰਲੇਖ (c, 1 /* radial */, clip_idx);
  ਜੇ (unlikely (!meta)) ਵਾਪਸ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ਅੰਕ_ops++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
		    ਖਾਲੀ             *paint_ਅੰਕੜੇ,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ, is_foreground, color);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			      hb_color_ਸਤਰ_t  *color_ਸਤਰ,
			      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			      ਦਸ਼ਮਲਵ x2, ਦਸ਼ਮਲਵ y2,
			      ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_emit_ਰੇਖਿਕ ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ, color_ਸਤਰ,
			    x0, y0, x1, y1, x2, y2);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			      hb_color_ਸਤਰ_t  *color_ਸਤਰ,
			      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
			      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
			      ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ, color_ਸਤਰ,
			    x0, y0, r0, x1, y1, r1);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_ਸਤਰ_t *color_ਸਤਰ,
			 ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
			 ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle, ਦਸ਼ਮਲਵ ਸਿਰਾ_angle)
{
  ਜੇ (unlikely (c->unsupported))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->clip_ਡੂੰਘਾਈ == 0))
    ਵਾਪਸ;
  ਜੇ (unlikely (c->ਅੰਕ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!ਗਿਣਤੀ_2))
    ਵਾਪਸ;
  hb_color_stop_t ਢੇਰ_stops[16];
  hb_color_stop_t *stops = ਢੇਰ_stops;
  hb_color_stop_t *ਢੇਰ_stops_2 = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (ਗਿਣਤੀ_2 > 16)
  {
    ਢੇਰ_stops_2 = (hb_color_stop_t *) hb_malloc2 (ਗਿਣਤੀ_2, ਆਕਾਰ (hb_color_stop_t));
    ਜੇ (unlikely (!ਢੇਰ_stops_2)) { c->unsupported = ਸੱਚ; ਵਾਪਸ; }
    stops = ਢੇਰ_stops_2;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ got = ਗਿਣਤੀ_2;
  hb_color_ਸਤਰ_get_color_stops (color_ਸਤਰ, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);

  ਪੂਰਨ clip_idx[HB_GPU_PAINT_MAX_CLIP_ਡੂੰਘਾਈ];
  ਜੇ (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (ਢੇਰ_stops_2); ਵਾਪਸ; }

  /* Build gradient params sub-blob.
   *   texel 0: (center_rendered_x, center_rendered_y,
   *             start_q14, end_q14)
   *     angles in radians, stored as Q14 fractions of pi; start/end
   *     stay in untransformed space (no rotation baked in).
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * Shader does p = L^-1 * (renderCoord - center_rendered) and
   * atan2 on p, so non-uniform scale / shear angular distortion is
   * handled exactly.
   */
  hb_ਸਦਿਸ਼_t<int16_t> grad_ਅੰਕੜੇ;
  ਜੇ (unlikely (!grad_ਅੰਕੜੇ.resize (8)))
  { c->unsupported = ਸੱਚ; hb_free (ਢੇਰ_stops_2); ਵਾਪਸ; }

  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops, got, &mn, &mx);
  ਦਸ਼ਮਲਵ a_ਵਿਸਤਾਰ = ਸਿਰਾ_angle - ਸ਼ੁਰੂ_angle;
  ਦਸ਼ਮਲਵ a_lo = ਸ਼ੁਰੂ_angle + mn * a_ਵਿਸਤਾਰ;
  ਦਸ਼ਮਲਵ a_hi = ਸ਼ੁਰੂ_angle + mx * a_ਵਿਸਤਾਰ;
  ਸਵੈਚਾਲਿਤ rad_ਮੰਜ਼ਿਲ_q14_pi = [] (ਦਸ਼ਮਲਵ rad) -> int16_t {
    ਦਸ਼ਮਲਵ v = rad * (ਦਸ਼ਮਲਵ) (1.0 / M_PI);  /* fraction of pi */
    ਜੇ (v <= -2.f) ਵਾਪਸ -32768;
    ਜੇ (v >=  2.f) ਵਾਪਸ  32767;
    ਵਾਪਸ (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  ਦਸ਼ਮਲਵ tcx = cx, tcy = cy;
  c->cur_transform.transform_ਬਿੰਦੂ (tcx, tcy);
  grad_ਅੰਕੜੇ[0] = clamp_i16 (tcx);
  grad_ਅੰਕੜੇ[1] = clamp_i16 (tcy);
  grad_ਅੰਕੜੇ[2] = rad_ਮੰਜ਼ਿਲ_q14_pi (a_lo);
  grad_ਅੰਕੜੇ[3] = rad_ਮੰਜ਼ਿਲ_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ਅੰਕੜੇ[4] = Linv[0];
  grad_ਅੰਕੜੇ[5] = Linv[1];
  grad_ਅੰਕੜੇ[6] = Linv[2];
  grad_ਅੰਕੜੇ[7] = Linv[3];

  ਜੇ (unlikely (!append_color_stops (grad_ਅੰਕੜੇ, stops, got)))
  { c->unsupported = ਸੱਚ; hb_free (ਢੇਰ_stops_2); ਵਾਪਸ; }
  hb_free (ਢੇਰ_stops_2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_ਬਾਈਟਾਂ = grad_ਅੰਕੜੇ.ਲੰਮਾਈ * ਆਕਾਰ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ਅਚਲ ਅੱਖਰ *) grad_ਅੰਕੜੇ.arrayZ,
					 grad_ਬਾਈਟਾਂ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_DUPLICATE,
					 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (unlikely (!grad_blob || !c->ਧੱਕੋ_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ਸੱਚ;
    ਵਾਪਸ;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ grad_idx = c->sub_blobs.ਲੰਮਾਈ - 1;

  int16_t *meta = emit_gradient_op_ਸਿਰਲੇਖ (c, 2 /* sweep */, clip_idx);
  ਜੇ (unlikely (!meta)) ਵਾਪਸ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ਅੰਕ_ops++;
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ਅਣਵਰਤਿਆ,
			     ਖਾਲੀ             *paint_ਅੰਕੜੇ,
			     hb_color_ਸਤਰ_t  *color_ਸਤਰ,
			     ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
			     ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
			     ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
			     ਖਾਲੀ             *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ, color_ਸਤਰ,
			   cx, cy, ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
ਸਥਿਰ hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_ਅਣਵਰਤਿਆ,
		    ਖਾਲੀ               *paint_ਅੰਕੜੇ,
		    hb_blob_t          *image   HB_ਅਣਵਰਤਿਆ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਚੌੜਾਈ   HB_ਅਣਵਰਤਿਆ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਉਚਾਈ  HB_ਅਣਵਰਤਿਆ,
		    hb_ਟੈਗ_t            format  HB_ਅਣਵਰਤਿਆ,
		    ਦਸ਼ਮਲਵ               slant   HB_ਅਣਵਰਤਿਆ,
		    hb_glyph_extents_t *extents HB_ਅਣਵਰਤਿਆ,
		    ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ)->unsupported = ਸੱਚ;
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ hb_bool_t
hb_gpu_paint_ਸੈੱਟ_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ,
			 int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_gpu_paint_t *) paint_ਅੰਕੜੇ;
  paint->budget = budget;
  paint->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_gpu_paint_t *) paint_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_gpu_paint_get_budget_ਬਚਿਆ (hb_paint_funcs_t *, ਖਾਲੀ *paint_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ &((hb_gpu_paint_t *) paint_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_gpu_paint_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func        (funcs, hb_gpu_paint_ਧੱਕੋ_transform,        ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func         (funcs, hb_gpu_paint_ਕੱਢੋ_transform,         ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func       (funcs, hb_gpu_paint_ਧੱਕੋ_clip_glyph,       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func  (funcs, hb_gpu_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ,  ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func    (funcs, hb_gpu_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ,    ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func              (funcs, hb_gpu_paint_ਕੱਢੋ_clip,              ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func            (funcs, hb_gpu_paint_ਧੱਕੋ_ਸਮੂਹ,            ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func             (funcs, hb_gpu_paint_ਕੱਢੋ_ਸਮੂਹ,             ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func                 (funcs, hb_gpu_paint_color,                 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func       (funcs, hb_gpu_paint_ਰੇਖਿਕ_gradient,       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_ਸੈੱਟ_image_func                 (funcs, hb_gpu_paint_image,                 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_gpu_paint_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_gpu_paint_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_gpu_paint_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_paint_funcs);

    ਵਾਪਸ funcs;
  }
} static_gpu_paint_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_gpu_paint_funcs ()
{
  static_gpu_paint_funcs.free_instance ();
}


/* ---- Public API ---- */

/**
 * hb_gpu_paint_create_or_fail:
 *
 * Creates a new GPU color-glyph paint encoder.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_gpu_paint_t, or `NULL` on allocation
 * failure.
 *
 * Since: 14.2.0
 **/
hb_gpu_paint_t *
hb_gpu_paint_create_or_fail (ਖਾਲੀ)
{
  ਵਾਪਸ hb_object_create<hb_gpu_paint_t> ();
}

/**
 * hb_gpu_paint_reference: (skip)
 * @paint: a GPU color-glyph paint encoder
 *
 * Increases the reference count on @paint by one.
 *
 * Return value: (transfer full):
 * The referenced #hb_gpu_paint_t.
 *
 * Since: 14.2.0
 **/
hb_gpu_paint_t *
hb_gpu_paint_reference (hb_gpu_paint_t *paint)
{
  ਵਾਪਸ hb_object_reference (paint);
}

/**
 * hb_gpu_paint_destroy: (skip)
 * @paint: a GPU color-glyph paint encoder
 *
 * Decreases the reference count on @paint by one.  When the
 * reference count reaches zero, the encoder is freed.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_destroy (hb_gpu_paint_t *paint)
{
  ਜੇ (!hb_object_should_destroy (paint))
    ਵਾਪਸ;

  hb_map_destroy (paint->custom_palette);
  ਲਈ (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  hb_gpu_draw_destroy (paint->scratch_draw);
  hb_blob_destroy (paint->recycled_blob);

  hb_object_actually_destroy (paint);
  hb_free (paint);
}

/**
 * hb_gpu_paint_set_user_data: (skip)
 * @paint: a GPU color-glyph paint encoder
 * @key: the user-data key
 * @data: a pointer to the user data
 * @destroy: (nullable): a callback to call when @data is not needed anymore
 * @replace: whether to replace an existing data with the same key
 *
 * Attaches user data to @paint.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_gpu_paint_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_gpu_paint_t     *paint,
			    hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			    ਖਾਲੀ               *ਅੰਕੜੇ,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (paint, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_gpu_paint_get_user_data: (skip)
 * @paint: a GPU color-glyph paint encoder
 * @key: the user-data key
 *
 * Fetches the user data associated with the specified key.
 *
 * Return value: (transfer none):
 * A pointer to the user data.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ *
hb_gpu_paint_get_user_ਅੰਕੜੇ (ਅਚਲ hb_gpu_paint_t *paint,
			    hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t   *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (paint, key);
}

/**
 * hb_gpu_paint_get_funcs:
 * @paint: a GPU paint context.
 *
 * Fetches the #hb_paint_funcs_t that feeds paint data into
 * @paint.  Pass @paint as the @paint_data argument when
 * calling the paint functions.
 *
 * Return value: (transfer none):
 * The GPU paint functions
 *
 * Since: 14.2.0
 **/
hb_paint_funcs_t *
hb_gpu_paint_get_funcs (ਅਚਲ hb_gpu_paint_t *paint HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ static_gpu_paint_funcs.get_unconst ();
}

/**
 * hb_gpu_paint_set_palette:
 * @paint: a GPU color-glyph paint encoder
 * @palette: palette index
 *
 * Selects which font palette is used when paint callbacks look up
 * indexed colors.  Default is palette 0.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_ਸੈੱਟ_palette (hb_gpu_paint_t *paint,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ        palette)
{
  paint->palette = palette;
}

/**
 * hb_gpu_paint_get_palette:
 * @paint: a GPU color-glyph paint encoder
 *
 * Returns the palette index previously set on @paint, or 0 if none
 * was set.
 *
 * Return value: the palette index.
 *
 * Since: 14.2.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_gpu_paint_get_palette (ਅਚਲ hb_gpu_paint_t *paint)
{
  ਵਾਪਸ paint->palette;
}

/**
 * hb_gpu_paint_clear_custom_palette_colors:
 * @paint: a GPU color-glyph paint encoder
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_clear_custom_palette_colors (hb_gpu_paint_t *paint)
{
  ਜੇ (paint->custom_palette)
    hb_map_clear (paint->custom_palette);
}

/**
 * hb_gpu_paint_set_custom_palette_color:
 * @paint: a GPU color-glyph paint encoder
 * @color_index: palette index to override
 * @color: replacement color
 *
 * Overrides one font palette color entry.  Overrides are keyed by
 * @color_index and persist on @paint until cleared (or replaced for
 * the same index).
 *
 * Return value: `true` if the override was set; `false` on allocation failure.
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_gpu_paint_ਸੈੱਟ_custom_palette_color (hb_gpu_paint_t *paint,
				       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    color_ਸੂਚਕਾਂਕ,
				       hb_color_t      color)
{
  ਜੇ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ਜੇ (unlikely (!paint->custom_palette))
      ਵਾਪਸ ਝੂਠ;
  }
  hb_map_ਸੈੱਟ (paint->custom_palette, color_ਸੂਚਕਾਂਕ, color);
  ਵਾਪਸ hb_map_ਵੰਡ_successful (paint->custom_palette);
}

/**
 * hb_gpu_paint_set_scale:
 * @paint: a GPU color-glyph paint encoder
 * @x_scale: horizontal scale (typically from hb_font_get_scale())
 * @y_scale: vertical scale
 *
 * Sets the font scale used to dimension clip-glyph slug outlines
 * inside the encoded blob.  Called automatically by
 * hb_gpu_paint_glyph().
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_ਸੈੱਟ_scale (hb_gpu_paint_t *paint,
			ਪੂਰਨ             x_scale,
			ਪੂਰਨ             y_scale)
{
  paint->x_scale = x_scale;
  paint->y_scale = y_scale;
}

/**
 * hb_gpu_paint_get_scale:
 * @paint: a GPU color-glyph paint encoder
 * @x_scale: (out): horizontal scale
 * @y_scale: (out): vertical scale
 *
 * Fetches the font scale previously set on @paint.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_get_scale (ਅਚਲ hb_gpu_paint_t *paint,
			ਪੂਰਨ                  *x_scale,
			ਪੂਰਨ                  *y_scale)
{
  ਜੇ (x_scale) *x_scale = paint->x_scale;
  ਜੇ (y_scale) *y_scale = paint->y_scale;
}

/**
 * hb_gpu_paint_glyph_or_fail:
 * @paint: a GPU color-glyph paint encoder
 * @font: font to paint from
 * @glyph: glyph ID to paint
 *
 * Convenience to feed @glyph's paint tree into the encoder.
 * Equivalent to:
 *
 * |[<!-- language="plain" -->
 * hb_gpu_paint_set_scale (paint, x_scale, y_scale);
 * hb_font_paint_glyph_or_fail (font, glyph,
 *   hb_gpu_paint_get_funcs (paint), paint,
 *   palette, HB_COLOR (0, 0, 0, 0xff));
 * ]|
 *
 * Encoder-level limitations (unsupported paint ops, group-stack
 * overflow) do NOT fail here -- they surface later from
 * hb_gpu_paint_encode() which returns `NULL`.
 *
 * Return value: `true` if the font had paint data for @glyph,
 * `false` otherwise.
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_gpu_paint_glyph_or_fail (hb_gpu_paint_t *paint,
			    hb_font_t      *font,
			    hb_codepoint_t  glyph)
{
  ਪੂਰਨ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_ਸੈੱਟ_scale (paint, x_scale, y_scale);
  ਵਾਪਸ hb_font_paint_glyph_or_fail (font, glyph,
				      hb_gpu_paint_get_funcs (paint), paint,
				      paint->palette,
				      /* Foreground RGB is not read from
				       * the encoded blob -- the
				       * is_foreground flag routes to the
				       * shader's foreground uniform.
				       * Alpha must be opaque so the
				       * baked color carries only the
				       * paint-tree alpha (which the
				       * shader applies on top of the
				       * uniform). */
				      HB_COLOR (0, 0, 0, 0xff));
}

/**
 * hb_gpu_paint_glyph:
 * @paint: a GPU color-glyph paint encoder
 * @font: font to paint from
 * @glyph: glyph ID to paint
 *
 * Feeds @glyph into the encoder.  Unlike
 * hb_gpu_paint_glyph_or_fail(), non-color glyphs are handled
 * transparently via a synthesized foreground-colored layer,
 * so any glyph with an outline produces output.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_glyph (hb_gpu_paint_t *paint,
		    hb_font_t      *font,
		    hb_codepoint_t  glyph)
{
  ਪੂਰਨ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_ਸੈੱਟ_scale (paint, x_scale, y_scale);
  hb_font_paint_glyph (font, glyph,
		       hb_gpu_paint_get_funcs (paint), paint,
		       paint->palette,
		       /* Foreground RGB is not read from
		        * the encoded blob -- the
		        * is_foreground flag routes to the
		        * shader's foreground uniform.
		        * Alpha must be opaque so the
		        * baked color carries only the
		        * paint-tree alpha (which the
		        * shader applies on top of the
		        * uniform). */
		       HB_COLOR (0, 0, 0, 0xff));
}

/**
 * hb_gpu_paint_encode:
 * @paint: a GPU color-glyph paint encoder
 * @extents: (out): receives the glyph's ink extents in font units
 *
 * Encodes the accumulated paint state into a GPU-renderable blob
 * and writes the glyph's ink extents to @extents.
 *
 * On return @paint is auto-cleared so it can be reused for the
 * next glyph; user configuration (palette, foreground, custom
 * palette overrides) is preserved.
 *
 * Return value: (transfer full):
 * A newly allocated blob, or `NULL` if the paint walk hit an
 * unsupported feature (see hb_gpu_paint_glyph()'s return value) or
 * encoding failed (allocation failure).  When the paint accumulated
 * no ink (e.g. a space glyph) returns the empty-blob singleton, so
 * callers can distinguish "nothing to render" (length 0) from a
 * real failure (`NULL`).
 *
 * Since: 14.2.0
 **/
hb_blob_t *
hb_gpu_paint_encode (hb_gpu_paint_t     *paint,
		     hb_glyph_extents_t *extents)
{
  HB_SCOPE_GUARD (hb_gpu_paint_clear (paint));

  ਜੇ (unlikely (paint->unsupported))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  ਜੇ (paint->ਅੰਕ_ops == 0)
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_texels = 3;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ texel_ਬਾਈਟਾਂ   = 8;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ops_texels = paint->ops.ਲੰਮਾਈ / 4;
  ਬਿਨਾਨਿਸ਼ਾਨੀ sub_ਬਾਈਟਾਂ = 0;
  ਲਈ (hb_blob_t *b : paint->sub_blobs)
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (sub_ਬਾਈਟਾਂ,
					     hb_blob_get_ਲੰਮਾਈ (b),
					     &sub_ਬਾਈਟਾਂ)))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  ਜੇ (unlikely (sub_ਬਾਈਟਾਂ % texel_ਬਾਈਟਾਂ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਬਾਈਟਾਂ = ਸਿਰਲੇਖ_texels * texel_ਬਾਈਟਾਂ
			+ paint->ops.ਲੰਮਾਈ * 2;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_ਬਾਈਟਾਂ, sub_ਬਾਈਟਾਂ,
					   &ਕੁੱਲ_ਬਾਈਟਾਂ)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ buf_ਸਮਰੱਥਾ = 0;
  ਅੱਖਰ *replaced_recycled_buf = ਖਾਲੀ_ਸੂਚਕ;
  ਅੱਖਰ *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, ਕੁੱਲ_ਬਾਈਟਾਂ,
					&buf_ਸਮਰੱਥਾ, &replaced_recycled_buf);
  ਜੇ (unlikely (!buf_raw))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  int16_t *buf = (int16_t *) (ਖਾਲੀ *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ sub_offsets_ਗਿਣਤੀ = paint->sub_blobs.ਲੰਮਾਈ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> sub_offsets;
  ਜੇ (unlikely (!sub_offsets.resize (sub_offsets_ਗਿਣਤੀ)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ cursor = ਸਿਰਲੇਖ_texels + ops_texels;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sub_offsets_ਗਿਣਤੀ; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_ਲੰਮਾਈ (paint->sub_blobs[i]) / texel_ਬਾਈਟਾਂ;
  }

  /* Header. */
  buf[0] = (int16_t) paint->ਅੰਕ_ops;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  buf[4] = (int16_t) hb_clamp (paint->ext_min_x, -0x7fff, 0x7fff);
  buf[5] = (int16_t) hb_clamp (paint->ext_min_y, -0x7fff, 0x7fff);
  buf[6] = (int16_t) hb_clamp (paint->ext_max_x, -0x7fff, 0x7fff);
  buf[7] = (int16_t) hb_clamp (paint->ext_max_y, -0x7fff, 0x7fff);
  buf[8] = (int16_t) ਸਿਰਲੇਖ_texels;
  buf[9]  = 0;
  buf[10] = 0;
  buf[11] = 0;

  /* Op stream: copy raw, then patch payload slots to texel
   * offsets where op_type indicates a sub_blob reference. */
  int16_t *ops_ਬਾਹਰਲਾ = buf + ਸਿਰਲੇਖ_texels * 4;
  hb_memcpy (ops_ਬਾਹਰਲਾ, paint->ops.arrayZ, paint->ops.ਲੰਮਾਈ * 2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
  ਜਦੋਂ (i < paint->ops.ਲੰਮਾਈ)
  {
    int16_t op_type = ops_ਬਾਹਰਲਾ[i];
    ਚੋਣ (op_type)
    {
      ਹਾਲਤ HB_GPU_PAINT_OP_LAYER_SOLID:
      ਹਾਲਤ HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_ਬਾਹਰਲਾ[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	ਬਿਨਾਨਿਸ਼ਾਨੀ slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ k = 0; k < 4; k++)
	{
	  ਜੇ (!slots[k]) ਜਾਰੀ;
	  ਬਿਨਾਨਿਸ਼ਾਨੀ p = i + slots[k];
	  ਬਿਨਾਨਿਸ਼ਾਨੀ idx = ((uint16_t) ops_ਬਾਹਰਲਾ[p] << 16)
		       |  (uint16_t) ops_ਬਾਹਰਲਾ[p + 1];
	  ਜੇ (idx < sub_offsets_ਗਿਣਤੀ)
	  {
	    ਬਿਨਾਨਿਸ਼ਾਨੀ off = sub_offsets[idx];
	    ops_ਬਾਹਰਲਾ[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_ਬਾਹਰਲਾ[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	ਰੋਕੋ;
      }
      ਹਾਲਤ HB_GPU_PAINT_OP_ਧੱਕੋ_ਸਮੂਹ:
      ਹਾਲਤ HB_GPU_PAINT_OP_ਕੱਢੋ_ਸਮੂਹ:
	i += 4;  /* 1 texel */
	ਰੋਕੋ;
      ਮੂਲ:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  ਅੱਖਰ *dst = (ਅੱਖਰ *) buf + ਸਿਰਲੇਖ_texels * texel_ਬਾਈਟਾਂ
			+ paint->ops.ਲੰਮਾਈ * 2;
  ਲਈ (hb_blob_t *b : paint->sub_blobs)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = hb_blob_get_ਲੰਮਾਈ (b);
    hb_memcpy (dst, hb_blob_get_ਅੰਕੜੇ (b, ਖਾਲੀ_ਸੂਚਕ), len);
    dst += len;
  }

  ਜੇ (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->ਚੌੜਾਈ     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->ਉਚਾਈ    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = ਖਾਲੀ_ਸੂਚਕ;
  ਵਾਪਸ hb_blob_t::recycle_finalize (buf_raw, buf_ਸਮਰੱਥਾ, ਕੁੱਲ_ਬਾਈਟਾਂ,
				recycled, replaced_recycled_buf);
}

/**
 * hb_gpu_paint_clear:
 * @paint: a GPU color-glyph paint encoder
 *
 * Discards accumulated paint state so @paint can be reused for
 * another encode.  User configuration (font scale) is preserved.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_clear (hb_gpu_paint_t *paint)
{
  /* Use reset() not resize(0): if an earlier push_or_fail() set
   * the vector's error state (common under failing-malloc),
   * resize(0) short-circuits without clearing length.  A
   * subsequent clear would double-destroy the blob pointers left
   * in the array, or keep stale ops across encodes. */
  paint->ops.reset ();
  paint->ਅੰਕ_ops = 0;
  ਲਈ (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  paint->sub_blobs.reset ();
  paint->sub_ਬਾਈਟਾਂ = 0;
  paint->clip_ਡੂੰਘਾਈ = 0;
  paint->pending_clip_ਰਸਤਾ = ਝੂਠ;
  paint->unsupported = ਝੂਠ;
  paint->recharge_budget ();
  paint->cur_transform = {1, 0, 0, 1, 0, 0};
  paint->transform_ਢੇਰ.reset ();
  paint->ext_min_x =  0x7fffffff;
  paint->ext_min_y =  0x7fffffff;
  paint->ext_max_x = -0x7fffffff;
  paint->ext_max_y = -0x7fffffff;
}

/**
 * hb_gpu_paint_reset:
 * @paint: a GPU color-glyph paint encoder
 *
 * Resets @paint, discarding accumulated state and user
 * configuration.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = ਖਾਲੀ_ਸੂਚਕ;
  hb_gpu_paint_clear (paint);
}

/**
 * hb_gpu_paint_recycle_blob:
 * @paint: a GPU color-glyph paint encoder
 * @blob: (transfer full): a blob previously returned by hb_gpu_paint_encode()
 *
 * Returns a blob to the encoder for potential reuse.  The caller
 * transfers ownership of @blob.
 *
 * If @blob came from hb_gpu_paint_encode() its underlying buffer
 * will be reused by the next call to hb_gpu_paint_encode(),
 * avoiding a malloc / blob allocation per glyph.
 *
 * Since: 14.2.0
 **/
ਖਾਲੀ
hb_gpu_paint_recycle_blob (hb_gpu_paint_t *paint,
			   hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&paint->recycled_blob, blob);
}


#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-paint-fragment-glsl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-paint-fragment-msl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-paint-fragment-wgsl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-paint-fragment-hlsl.hh"

/**
 * hb_gpu_paint_shader_source:
 * @stage: pipeline stage (vertex or fragment)
 * @lang: shader language variant
 *
 * Returns the paint-renderer-specific shader source for the
 * specified stage and language.  The returned string is static
 * and must not be freed.
 *
 * This source assumes both the shared helpers
 * (hb_gpu_shader_source()) and the draw-renderer helpers
 * (hb_gpu_draw_shader_source()) are concatenated ahead of it --
 * the paint interpreter invokes hb_gpu_draw() to compute
 * clip-glyph coverage.  Full assembly:
 *
 *   [#version] + hb_gpu_shader_source ()
 *             + hb_gpu_draw_shader_source ()
 *             + hb_gpu_paint_shader_source ()
 *             + caller's main ()
 *
 * Return value: (transfer none):
 * A shader source string, or `NULL` if @stage or @lang is
 * unsupported.
 *
 * Since: 14.2.0
 **/
ਅਚਲ ਅੱਖਰ *
hb_gpu_paint_shader_ਸਰੋਤ (hb_gpu_shader_stage_t stage,
			    hb_gpu_shader_lang_t  lang)
{
  ਚੋਣ (stage) {
  ਹਾਲਤ HB_GPU_SHADER_STAGE_FRAGMENT:
    ਚੋਣ (lang) {
    ਹਾਲਤ HB_GPU_SHADER_LANG_GLSL: ਵਾਪਸ hb_gpu_paint_fragment_glsl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_MSL:  ਵਾਪਸ hb_gpu_paint_fragment_msl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_WGSL: ਵਾਪਸ hb_gpu_paint_fragment_wgsl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_HLSL: ਵਾਪਸ hb_gpu_paint_fragment_hlsl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_INVALID:
    ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  ਹਾਲਤ HB_GPU_SHADER_STAGE_ਸ਼ੀਰਸ਼:
    ਚੋਣ (lang) {
    ਹਾਲਤ HB_GPU_SHADER_LANG_GLSL:
    ਹਾਲਤ HB_GPU_SHADER_LANG_MSL:
    ਹਾਲਤ HB_GPU_SHADER_LANG_WGSL:
    ਹਾਲਤ HB_GPU_SHADER_LANG_HLSL: ਵਾਪਸ "";
    ਹਾਲਤ HB_GPU_SHADER_LANG_INVALID:
    ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
  ਮੂਲ:
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}
