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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-gpu.h"
#ಅನ್ತರ್ಭಾವ "hb-gpu-paint.hh"
#ಅನ್ತರ್ಭಾವ "hb-gpu-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.hh"


/* ---- Paint callbacks ---- */

/* Op-type tags.  Must match the layout documented in
 * hb-gpu-paint.hh. */
ಗಣನಾ {
  HB_GPU_PAINT_OP_LAYER_SOLID    = 0,
  HB_GPU_PAINT_OP_LAYER_GRADIENT = 1,
  HB_GPU_PAINT_OP_ತಳ್ಳು_GROUP     = 2,
  HB_GPU_PAINT_OP_ತೆಗೆ_GROUP      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#define HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#define HB_GPU_PAINT_MAX_GROUP_DEPTH 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#define HB_GPU_PAINT_MAX_CLIP_DEPTH  3      /* shader can intersect up to 3 clip outlines per layer */
#define HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#define HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#define HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#define HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

ಸ್ಥಿರ hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_UNUSED,
				   ಶೂನ್ಯ             *paint_data,
				   unsigned          color_index,
				   hb_color_t       *color,
				   ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  ಯದಿ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_index)))
  {
    *color = hb_map_get (c->custom_palette, color_index);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತಳ್ಳು_clip_glyph (hb_paint_funcs_t *funcs HB_UNUSED,
			      ಶೂನ್ಯ             *paint_data,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;

  ಯದಿ (unlikely (c->clip_depth >= HB_GPU_PAINT_MAX_CLIP_DEPTH))
  {
    c->unsupported = ಸತ್ಯ;
    c->clip_depth++;
    ಪ್ರತ್ಯಾಗಮ;
  }
  c->clip_ರಾಶಿ[c->clip_depth] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_depth++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತೆಗೆ_clip (hb_paint_funcs_t *funcs HB_UNUSED,
		       ಶೂನ್ಯ             *paint_data,
		       ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  ಯದಿ (likely (c->clip_depth > 0)) c->clip_depth--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
ಸ್ಥಿರ hb_draw_funcs_t *
hb_gpu_paint_ತಳ್ಳು_clip_path_start (hb_paint_funcs_t *funcs HB_UNUSED,
				   ಶೂನ್ಯ             *paint_data,
				   ಶೂನ್ಯ            **draw_data,
				   ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;

  ಯದಿ (unlikely (c->clip_depth >= HB_GPU_PAINT_MAX_CLIP_DEPTH))
  {
    c->unsupported = ಸತ್ಯ;
    *draw_data = nullptr;
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ಯದಿ (unlikely (!c->scratch_draw))
    {
      c->unsupported = ಸತ್ಯ;
      *draw_data = nullptr;
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_set_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_remaining);
  hb_gpu_draw_set_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_path_transform = c->cur_transform;
  c->pending_clip_path = ಸತ್ಯ;

  *draw_data = c->scratch_draw;
  ಪ್ರತ್ಯಾಗಮ hb_gpu_draw_get_funcs (c->scratch_draw);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತಳ್ಳು_clip_path_end (hb_paint_funcs_t *funcs HB_UNUSED,
				 ಶೂನ್ಯ             *paint_data,
				 ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;

  ಯದಿ (unlikely (!c->pending_clip_path))
    ಪ್ರತ್ಯಾಗಮ;
  c->pending_clip_path = ಅಸತ್ಯ;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_remaining = hb_draw_get_budget_remaining (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ಯದಿ (unlikely (!blob || !c->ತಳ್ಳು_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  ಪೂರ್ಣಾಙ್ಕ idx = (ಪೂರ್ಣಾಙ್ಕ) (c->sub_blobs.length - 1);

  ಯದಿ (unlikely (c->clip_depth >= HB_GPU_PAINT_MAX_CLIP_DEPTH))
  {
    c->unsupported = ಸತ್ಯ;
    c->clip_depth++;
    ಪ್ರತ್ಯಾಗಮ;
  }

  ಪೂರ್ಣಾಙ್ಕ x0 = ext.x_bearing;
  ಪೂರ್ಣಾಙ್ಕ x1 = hb_saturate_add (ext.x_bearing, ext.width);
  ಪೂರ್ಣಾಙ್ಕ y0 = ext.y_bearing;
  ಪೂರ್ಣಾಙ್ಕ y1 = hb_saturate_add (ext.y_bearing, ext.height);

  c->clip_ರಾಶಿ[c->clip_depth] = {
    HB_CODEPOINT_INVALID, nullptr,
    c->pending_clip_path_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_depth++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತಳ್ಳು_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			     ಶೂನ್ಯ             *paint_data,
			     ಭಿನ್ನ xx, ಭಿನ್ನ yx,
			     ಭಿನ್ನ xy, ಭಿನ್ನ yy,
			     ಭಿನ್ನ dx, ಭಿನ್ನ dy,
			     ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  ಯದಿ (unlikely (!c->transform_ರಾಶಿ.ತಳ್ಳು_or_fail (c->cur_transform)))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  hb_transform_t<ಭಿನ್ನ> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತೆಗೆ_transform (hb_paint_funcs_t *funcs HB_UNUSED,
			    ಶೂನ್ಯ             *paint_data,
			    ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  ಯದಿ (unlikely (!c->transform_ರಾಶಿ.length))
    ಪ್ರತ್ಯಾಗಮ;
  c->cur_transform = c->transform_ರಾಶಿ.arrayZ[c->transform_ರಾಶಿ.length - 1];
  c->transform_ರಾಶಿ.shrink (c->transform_ರಾಶಿ.length - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
ಸ್ಥಿರ ಶೂನ್ಯ
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  ಯದಿ (unlikely (c->ಅಂಕೆ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  ಯದಿ (unlikely (!c->ops.resize (c->ops.length + 4)))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.length - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->ಅಂಕೆ_ops++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತಳ್ಳು_group (hb_paint_funcs_t *funcs HB_UNUSED,
			 ಶೂನ್ಯ             *paint_data,
			 ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  c->group_depth++;
  ಯದಿ (unlikely (c->group_depth > HB_GPU_PAINT_MAX_GROUP_DEPTH))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_ತಳ್ಳು_GROUP, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_ತೆಗೆ_group (hb_paint_funcs_t    *funcs HB_UNUSED,
			ಶೂನ್ಯ                *paint_data,
			hb_paint_composite_mode_t mode,
			ಶೂನ್ಯ                *user_data HB_UNUSED)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_data;
  ತಾರ್ಕಿಕ was_in_range = c->group_depth <= HB_GPU_PAINT_MAX_GROUP_DEPTH;
  ಯದಿ (likely (c->group_depth > 0)) c->group_depth--;
  ಯದಿ (was_in_range)
    emit_control_op (c, HB_GPU_PAINT_OP_ತೆಗೆ_GROUP, (int16_t) mode);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int16_t
color_to_q15 (unsigned byte_ಮೌಲ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ (int16_t) ((byte_ಮೌಲ್ಯ * 32767u + 127u) / 255u);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
color_to_q15_rgba (hb_color_t color, int16_t out[4])
{
  out[0] = color_to_q15 (hb_color_get_red   (color));
  out[1] = color_to_q15 (hb_color_get_green (color));
  out[2] = color_to_q15 (hb_color_get_blue  (color));
  out[3] = color_to_q15 (hb_color_get_alpha (color));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ int16_t
clamp_i16 (ಭಿನ್ನ v)
{
  ಯದಿ (v <= -32768.f) ಪ್ರತ್ಯಾಗಮ -32768;
  ಯದಿ (v >=  32767.f) ಪ್ರತ್ಯಾಗಮ  32767;
  ಯದಿ (unlikely (std::isnan (v))) ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ (int16_t) v;
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
linv_q10 (ಧ್ರುವ hb_transform_t<ಭಿನ್ನ> &t, int16_t out[4])
{
  ಭಿನ್ನ det = t.xx * t.yy - t.xy * t.yx;
  ಭಿನ್ನ inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  out[0] = clamp_i16 ( t.yy * inv * 1024.f);
  out[1] = clamp_i16 (-t.xy * inv * 1024.f);
  out[2] = clamp_i16 (-t.yx * inv * 1024.f);
  out[3] = clamp_i16 ( t.xx * inv * 1024.f);
}

/* Transforming pen: applies an affine transform to each outline
 * point before forwarding to a downstream hb_draw_funcs_t.  The
 * downstream's inline draw-state bookkeeping runs against a
 * separate state (down_st) so transformed-space updates don't
 * pollute the caller's pre-transform state. */
ಸಂರಚನಾ hb_gpu_paint_pen_t
{
  hb_transform_t<ಭಿನ್ನ> transform;
  hb_draw_funcs_t  *dfuncs;
  ಶೂನ್ಯ             *data;
  hb_draw_state_t   down_st;
  int64_t          *budget_remaining;
};

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_pen_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  ಶೂನ್ಯ *data, hb_draw_state_t *st HB_UNUSED,
			  ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  auto *c = (hb_gpu_paint_pen_t *) data;
  c->transform.transform_point (to_x, to_y);
  c->dfuncs->move_to (c->data, c->down_st, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_pen_ಸಾಲು_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  ಶೂನ್ಯ *data, hb_draw_state_t *st HB_UNUSED,
			  ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			  ಶೂನ್ಯ *user_data HB_UNUSED)
{
  auto *c = (hb_gpu_paint_pen_t *) data;
  c->transform.transform_point (to_x, to_y);
  c->dfuncs->ಸಾಲು_to (c->data, c->down_st, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_pen_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			       ಶೂನ್ಯ *data, hb_draw_state_t *st HB_UNUSED,
			       ಭಿನ್ನ c_x, ಭಿನ್ನ c_y,
			       ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			       ಶೂನ್ಯ *user_data HB_UNUSED)
{
  auto *c = (hb_gpu_paint_pen_t *) data;
  c->transform.transform_point (c_x, c_y);
  c->transform.transform_point (to_x, to_y);
  c->dfuncs->quadratic_to (c->data, c->down_st, c_x, c_y, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_pen_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			   ಶೂನ್ಯ *data, hb_draw_state_t *st HB_UNUSED,
			   ಭಿನ್ನ c1_x, ಭಿನ್ನ c1_y,
			   ಭಿನ್ನ c2_x, ಭಿನ್ನ c2_y,
			   ಭಿನ್ನ to_x, ಭಿನ್ನ to_y,
			   ಶೂನ್ಯ *user_data HB_UNUSED)
{
  auto *c = (hb_gpu_paint_pen_t *) data;
  c->transform.transform_point (c1_x, c1_y);
  c->transform.transform_point (c2_x, c2_y);
  c->transform.transform_point (to_x, to_y);
  c->dfuncs->cubic_to (c->data, c->down_st, c1_x, c1_y, c2_x, c2_y, to_x, to_y);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_pen_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
			     ಶೂನ್ಯ *data, hb_draw_state_t *st HB_UNUSED,
			     ಶೂನ್ಯ *user_data HB_UNUSED)
{
  auto *c = (hb_gpu_paint_pen_t *) data;
  c->dfuncs->close_path (c->data, c->down_st);
}

ಸ್ಥಿರ int64_t *
hb_gpu_paint_pen_get_budget_remaining (hb_draw_funcs_t *, ಶೂನ್ಯ *draw_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ ((hb_gpu_paint_pen_t *) draw_data)->budget_remaining;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_gpu_paint_pen_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_set_move_to_func      (funcs, hb_gpu_paint_pen_move_to,      nullptr, nullptr);
    hb_draw_funcs_set_ಸಾಲು_to_func      (funcs, hb_gpu_paint_pen_ಸಾಲು_to,      nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_gpu_paint_pen_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func     (funcs, hb_gpu_paint_pen_cubic_to,     nullptr, nullptr);
    hb_draw_funcs_set_close_path_func   (funcs, hb_gpu_paint_pen_close_path,   nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_gpu_paint_pen_get_budget_remaining, nullptr, nullptr);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_gpu_paint_pen_funcs);
    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_gpu_paint_pen_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
emit_clip_sub_blob (hb_gpu_paint_t *c,
		    hb_gpu_paint_t::pending_clip_t &clip)
{
  /* Path clips were already encoded into sub_blobs at
   * push_clip_path_end time; glyph clips are cached here on first
   * use.  Either way: just accumulate extents and hand back the
   * recorded index. */
  ಯದಿ (clip.sub_blob_index >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    ಪ್ರತ್ಯಾಗಮ clip.sub_blob_index;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  ಯದಿ (unlikely (c->sub_blobs.length >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ಯದಿ (unlikely (c->budget_remaining < 0))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ -1;
  }

  ಯದಿ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ಯದಿ (unlikely (!c->scratch_draw))
    {
      c->unsupported = ಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_set_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_remaining);

  ತಾರ್ಕಿಕ ok;
  ಯದಿ (clip.transform.is_identity ())
  {
    /* Fast path: feed the glyph outline straight into the draw
     * encoder with no adapter. */
    ok = hb_gpu_draw_glyph_or_fail (c->scratch_draw, clip.font, clip.glyph);
  }
  ಅನ್ಯಥಾ
  {
    /* Transform each outline point by the transform that was
     * current at push_clip_glyph time -- NOT the innermost
     * cur_transform, which may have additional PaintTransform
     * wrappers from the paint child underneath. */
    ಪೂರ್ಣಾಙ್ಕ x_scale, y_scale;
    hb_font_get_scale (clip.font, &x_scale, &y_scale);
    hb_gpu_draw_set_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.data      = c->scratch_draw;
    pen.down_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_remaining = &c->scratch_draw->budget_remaining;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_path (pen.data, pen.down_st);
  }
  c->budget_remaining = hb_draw_get_budget_remaining (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  ಯದಿ (!ok)
    ಪ್ರತ್ಯಾಗಮ -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ಯದಿ (unlikely (!blob || !c->ತಳ್ಳು_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  ಪೂರ್ಣಾಙ್ಕ x0 = ext.x_bearing;
  ಪೂರ್ಣಾಙ್ಕ x1 = hb_saturate_add (ext.x_bearing, ext.width);
  ಪೂರ್ಣಾಙ್ಕ y0 = ext.y_bearing;
  ಪೂರ್ಣಾಙ್ಕ y1 = hb_saturate_add (ext.y_bearing, ext.height);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  ಪೂರ್ಣಾಙ್ಕ idx = (ಪೂರ್ಣಾಙ್ಕ) (c->sub_blobs.length - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_index = idx;
  clip.ext_x0 = hb_min (x0, x1);
  clip.ext_y0 = hb_min (y0, y1);
  clip.ext_x1 = hb_max (x0, x1);
  clip.ext_y1 = hb_max (y0, y1);

  ಪ್ರತ್ಯಾಗಮ idx;
}

/* Emit a sub-blob for every clip currently on the stack and return
 * their indices in @out (set to -1 for unused slots).  Returns
 * false if any clip's outline is empty (caller should skip the
 * layer entirely) or hb_gpu_draw_encode failed. */
ಸ್ಥಿರ ತಾರ್ಕಿಕ
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, ಪೂರ್ಣಾಙ್ಕ out[HB_GPU_PAINT_MAX_CLIP_DEPTH])
{
  ಪ್ರತಿ (unsigned i = 0; i < HB_GPU_PAINT_MAX_CLIP_DEPTH; i++)
    out[i] = -1;
  ಪ್ರತಿ (unsigned i = 0; i < hb_min (c->clip_depth, (unsigned) HB_GPU_PAINT_MAX_CLIP_DEPTH); i++)
  {
    out[i] = emit_clip_sub_blob (c, c->clip_ರಾಶಿ[i]);
    ಯದಿ (out[i] < 0)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
ಸ್ಥಿರ int16_t *
emit_gradient_op_header (hb_gpu_paint_t *c, unsigned subtype,
			 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ clip_idx[HB_GPU_PAINT_MAX_CLIP_DEPTH])
{
  ಯದಿ (unlikely (!c->ops.resize (c->ops.length + 12)))
  { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ nullptr; }
  int16_t *o = &c->ops.arrayZ[c->ops.length - 12];
  int16_t flags = (int16_t) subtype;
  ಯದಿ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ಯದಿ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_GRADIENT;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  ಪ್ರತ್ಯಾಗಮ o + 8;  /* texel 2: gradient meta -- caller fills */
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_emit_solid (hb_gpu_paint_t *c,
			 hb_bool_t       is_foreground,
			 hb_color_t      color)
{
  ಯದಿ (unlikely (c->unsupported))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->clip_depth == 0))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->ಅಂಕೆ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }

  ಪೂರ್ಣಾಙ್ಕ clip_idx[HB_GPU_PAINT_MAX_CLIP_DEPTH];
  ಯದಿ (!emit_all_clip_sub_blobs (c, clip_idx))
    ಪ್ರತ್ಯಾಗಮ;

  /* Emit LAYER_SOLID op: 3 texels (12 int16s).
   *   texel 0: op_type, flags, clip1_hi, clip1_lo
   *   texel 1: clip2_hi, clip2_lo, 0, 0  (only meaningful if HAS_CLIP2)
   *   texel 2: r_q15, g_q15, b_q15, a_q15
   * Payloads hold sub_blob indices; hb_gpu_paint_encode() patches
   * them into texel offsets at serialize time.
   * flags bit 0: use shader foreground uniform instead of the
   * baked color (so runtime foreground-color changes still work).
   * flags bit 8: a second clip outline is present in texel 1. */
  ಯದಿ (unlikely (!c->ops.resize (c->ops.length + 12)))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.length - 12];
  int16_t flags = (int16_t) (is_foreground ? HB_GPU_PAINT_FLAG_IS_FOREGROUND : 0);
  ಯದಿ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ಯದಿ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_SOLID;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  color_to_q15_rgba (color, o + 8);
  c->ಅಂಕೆ_ops++;
}

/* Append a color line (@count stops) to @grad_data in the encoded
 * stop format used by gradient ops: 2 texels per stop.
 *   texel a: (offset_q15, flags, _, _) with flags bit 0 = is_foreground
 *   texel b: (r_q15, g_q15, b_q15, a_q15)
 * Returns false on allocation failure. */
ಸ್ಥಿರ ತಾರ್ಕಿಕ
append_color_stops (hb_vector_t<int16_t> &grad_data,
		    ಧ್ರುವ hb_color_stop_t *stops,
		    unsigned ಎಣಿಕೆ)
{
  unsigned base = grad_data.length;
  ಯದಿ (unlikely (!grad_data.resize (base + ಎಣಿಕೆ * 8)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  int16_t *p = grad_data.arrayZ + base;
  ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
  {
    ಭಿನ್ನ off = stops[i].offset;
    ಯದಿ (off < -1.f) off = -1.f;
    ಅನ್ಯಥಾ ಯದಿ (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_to_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_emit_linear (hb_gpu_paint_t  *c,
			  hb_color_ಸಾಲು_t *color_ಸಾಲು,
			  ಭಿನ್ನ x0, ಭಿನ್ನ y0,
			  ಭಿನ್ನ x1, ಭಿನ್ನ y1,
			  ಭಿನ್ನ x2, ಭಿನ್ನ y2)
{
  ಯದಿ (unlikely (c->unsupported))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->clip_depth == 0))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->ಅಂಕೆ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  ಯದಿ (unlikely (!c->fetch_color_stops (color_ಸಾಲು)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  unsigned got = c->color_stops_scratch.length;

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);

  /* Emit clip sub-blob(s) first. */
  ಪೂರ್ಣಾಙ್ಕ clip_idx[HB_GPU_PAINT_MAX_CLIP_DEPTH];
  ಯದಿ (!emit_all_clip_sub_blobs (c, clip_idx)) ಪ್ರತ್ಯಾಗಮ;

  /* Build gradient params sub-blob.
   *   texel 0: (p0_rendered_x, p0_rendered_y, d_canonical_x, d_canonical_y)
   *     i16 font units; d = p1 - p0 stored in untransformed space
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * The shader computes p_canonical = L^-1 * (renderCoord - p0_rendered)
   * and evaluates t = dot(p_canonical, d) / dot(d, d) in
   * untransformed space, so affine distortions of the axis are
   * handled exactly rather than approximated. */
  hb_vector_t<int16_t> grad_data;
  ಯದಿ (unlikely (!grad_data.resize (8)))
  { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ; }

  ಭಿನ್ನ lx0, ly0, lx1, ly1;
  hb_paint_reduce_linear_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, got, &mn, &mx);
  ಭಿನ್ನ dx = lx1 - lx0, dy = ly1 - ly0;
  ಭಿನ್ನ ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  ಭಿನ್ನ ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  ಭಿನ್ನ p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_point (p0x_r, p0y_r);
  grad_data[0] = clamp_i16 (p0x_r);
  grad_data[1] = clamp_i16 (p0y_r);
  grad_data[2] = clamp_i16 (ax1 - ax0);
  grad_data[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_data[4] = Linv[0];
  grad_data[5] = Linv[1];
  grad_data[6] = Linv[2];
  grad_data[7] = Linv[3];

  ಯದಿ (unlikely (!append_color_stops (grad_data, stops, got)))
  { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ; }

  unsigned grad_bytes = grad_data.length * ಪರಿಮಾಣ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) grad_data.arrayZ,
					 grad_bytes, HB_MEMORY_MODE_DUPLICATE,
					 nullptr, nullptr);
  ಯದಿ (unlikely (!grad_blob || !c->ತಳ್ಳು_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  unsigned grad_idx = c->sub_blobs.length - 1;

  int16_t *meta = emit_gradient_op_header (c, 0 /* linear */, clip_idx);
  ಯದಿ (unlikely (!meta)) ಪ್ರತ್ಯಾಗಮ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ಅಂಕೆ_ops++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_ಸಾಲು_t *color_ಸಾಲು,
			  ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
			  ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1)
{
  ಯದಿ (unlikely (c->unsupported))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->clip_depth == 0))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->ಅಂಕೆ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }

  ಯದಿ (unlikely (!c->fetch_color_stops (color_ಸಾಲು)))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  unsigned got = c->color_stops_scratch.length;

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);

  ಪೂರ್ಣಾಙ್ಕ clip_idx[HB_GPU_PAINT_MAX_CLIP_DEPTH];
  ಯದಿ (!emit_all_clip_sub_blobs (c, clip_idx)) ಪ್ರತ್ಯಾಗಮ;

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
  hb_vector_t<int16_t> grad_data;
  ಯದಿ (unlikely (!grad_data.resize (12)))
  { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ; }
  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, got, &mn, &mx);
  ಭಿನ್ನ dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  ಭಿನ್ನ cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  ಭಿನ್ನ cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  ಭಿನ್ನ cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_point (cx0_r, cy0_r);
  grad_data[0] = clamp_i16 (cx0_r);
  grad_data[1] = clamp_i16 (cy0_r);
  grad_data[2] = clamp_i16 (cx1 - cx0);
  grad_data[3] = clamp_i16 (cy1 - cy0);
  grad_data[4] = clamp_i16 (cr0);
  grad_data[5] = clamp_i16 (cr1);
  grad_data[6] = 0;
  grad_data[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_data[8]  = Linv[0];
  grad_data[9]  = Linv[1];
  grad_data[10] = Linv[2];
  grad_data[11] = Linv[3];

  ಯದಿ (unlikely (!append_color_stops (grad_data, stops, got)))
  { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ; }

  unsigned grad_bytes = grad_data.length * ಪರಿಮಾಣ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) grad_data.arrayZ,
					 grad_bytes, HB_MEMORY_MODE_DUPLICATE,
					 nullptr, nullptr);
  ಯದಿ (unlikely (!grad_blob || !c->ತಳ್ಳು_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  unsigned grad_idx = c->sub_blobs.length - 1;

  int16_t *meta = emit_gradient_op_header (c, 1 /* radial */, clip_idx);
  ಯದಿ (unlikely (!meta)) ಪ್ರತ್ಯಾಗಮ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ಅಂಕೆ_ops++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_UNUSED,
		    ಶೂನ್ಯ             *paint_data,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_data, is_foreground, color);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_linear_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
			      ಶೂನ್ಯ             *paint_data,
			      hb_color_ಸಾಲು_t  *color_ಸಾಲು,
			      ಭಿನ್ನ x0, ಭಿನ್ನ y0,
			      ಭಿನ್ನ x1, ಭಿನ್ನ y1,
			      ಭಿನ್ನ x2, ಭಿನ್ನ y2,
			      ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_emit_linear ((hb_gpu_paint_t *) paint_data, color_ಸಾಲು,
			    x0, y0, x1, y1, x2, y2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
			      ಶೂನ್ಯ             *paint_data,
			      hb_color_ಸಾಲು_t  *color_ಸಾಲು,
			      ಭಿನ್ನ x0, ಭಿನ್ನ y0, ಭಿನ್ನ r0,
			      ಭಿನ್ನ x1, ಭಿನ್ನ y1, ಭಿನ್ನ r1,
			      ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_data, color_ಸಾಲು,
			    x0, y0, r0, x1, y1, r1);
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_ಸಾಲು_t *color_ಸಾಲು,
			 ಭಿನ್ನ cx, ಭಿನ್ನ cy,
			 ಭಿನ್ನ start_angle, ಭಿನ್ನ end_angle)
{
  ಯದಿ (unlikely (c->unsupported))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->clip_depth == 0))
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (unlikely (c->ಅಂಕೆ_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }

  unsigned ಎಣಿಕೆ = hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, nullptr, nullptr);
  ಯದಿ (unlikely (!ಎಣಿಕೆ))
    ಪ್ರತ್ಯಾಗಮ;
  hb_color_stop_t ರಾಶಿ_stops[16];
  hb_color_stop_t *stops = ರಾಶಿ_stops;
  hb_color_stop_t *heap_stops = nullptr;
  ಯದಿ (ಎಣಿಕೆ > 16)
  {
    heap_stops = (hb_color_stop_t *) hb_malloc2 (ಎಣಿಕೆ, ಪರಿಮಾಣ (hb_color_stop_t));
    ಯದಿ (unlikely (!heap_stops)) { c->unsupported = ಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ; }
    stops = heap_stops;
  }
  unsigned got = ಎಣಿಕೆ;
  hb_color_ಸಾಲು_get_color_stops (color_ಸಾಲು, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_ಸಾಲು_get_extend (color_ಸಾಲು);

  ಪೂರ್ಣಾಙ್ಕ clip_idx[HB_GPU_PAINT_MAX_CLIP_DEPTH];
  ಯದಿ (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (heap_stops); ಪ್ರತ್ಯಾಗಮ; }

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
  hb_vector_t<int16_t> grad_data;
  ಯದಿ (unlikely (!grad_data.resize (8)))
  { c->unsupported = ಸತ್ಯ; hb_free (heap_stops); ಪ್ರತ್ಯಾಗಮ; }

  ಭಿನ್ನ mn, mx;
  hb_paint_normalize_color_ಸಾಲು (stops, got, &mn, &mx);
  ಭಿನ್ನ a_span = end_angle - start_angle;
  ಭಿನ್ನ a_lo = start_angle + mn * a_span;
  ಭಿನ್ನ a_hi = start_angle + mx * a_span;
  auto rad_to_q14_pi = [] (ಭಿನ್ನ rad) -> int16_t {
    ಭಿನ್ನ v = rad * (ಭಿನ್ನ) (1.0 / M_PI);  /* fraction of pi */
    ಯದಿ (v <= -2.f) ಪ್ರತ್ಯಾಗಮ -32768;
    ಯದಿ (v >=  2.f) ಪ್ರತ್ಯಾಗಮ  32767;
    ಪ್ರತ್ಯಾಗಮ (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  ಭಿನ್ನ tcx = cx, tcy = cy;
  c->cur_transform.transform_point (tcx, tcy);
  grad_data[0] = clamp_i16 (tcx);
  grad_data[1] = clamp_i16 (tcy);
  grad_data[2] = rad_to_q14_pi (a_lo);
  grad_data[3] = rad_to_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_data[4] = Linv[0];
  grad_data[5] = Linv[1];
  grad_data[6] = Linv[2];
  grad_data[7] = Linv[3];

  ಯದಿ (unlikely (!append_color_stops (grad_data, stops, got)))
  { c->unsupported = ಸತ್ಯ; hb_free (heap_stops); ಪ್ರತ್ಯಾಗಮ; }
  hb_free (heap_stops);

  unsigned grad_bytes = grad_data.length * ಪರಿಮಾಣ (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) grad_data.arrayZ,
					 grad_bytes, HB_MEMORY_MODE_DUPLICATE,
					 nullptr, nullptr);
  ಯದಿ (unlikely (!grad_blob || !c->ತಳ್ಳು_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ;
  }
  unsigned grad_idx = c->sub_blobs.length - 1;

  int16_t *meta = emit_gradient_op_header (c, 2 /* sweep */, clip_idx);
  ಯದಿ (unlikely (!meta)) ಪ್ರತ್ಯಾಗಮ;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->ಅಂಕೆ_ops++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_UNUSED,
			     ಶೂನ್ಯ             *paint_data,
			     hb_color_ಸಾಲು_t  *color_ಸಾಲು,
			     ಭಿನ್ನ cx, ಭಿನ್ನ cy,
			     ಭಿನ್ನ start_angle,
			     ಭಿನ್ನ end_angle,
			     ಶೂನ್ಯ             *user_data HB_UNUSED)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_data, color_ಸಾಲು,
			   cx, cy, start_angle, end_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
ಸ್ಥಿರ hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_UNUSED,
		    ಶೂನ್ಯ               *paint_data,
		    hb_blob_t          *image   HB_UNUSED,
		    unsigned ಪೂರ್ಣಾಙ್ಕ        width   HB_UNUSED,
		    unsigned ಪೂರ್ಣಾಙ್ಕ        height  HB_UNUSED,
		    hb_tag_t            format  HB_UNUSED,
		    ಭಿನ್ನ               slant   HB_UNUSED,
		    hb_glyph_extents_t *extents HB_UNUSED,
		    ಶೂನ್ಯ               *user_data HB_UNUSED)
{
  ((hb_gpu_paint_t *) paint_data)->unsupported = ಸತ್ಯ;
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ hb_bool_t
hb_gpu_paint_set_budget (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data,
			 int64_t budget, ಶೂನ್ಯ *)
{
  auto *paint = (hb_gpu_paint_t *) paint_data;
  paint->budget = budget;
  paint->recharge_budget ();
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ ((hb_gpu_paint_t *) paint_data)->budget;
}

ಸ್ಥಿರ int64_t *
hb_gpu_paint_get_budget_remaining (hb_paint_funcs_t *, ಶೂನ್ಯ *paint_data, ಶೂನ್ಯ *)
{
  ಪ್ರತ್ಯಾಗಮ &((hb_gpu_paint_t *) paint_data)->budget_remaining;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_gpu_paint_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_set_ತಳ್ಳು_transform_func        (funcs, hb_gpu_paint_ತಳ್ಳು_transform,        nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_transform_func         (funcs, hb_gpu_paint_ತೆಗೆ_transform,         nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_glyph_func       (funcs, hb_gpu_paint_ತಳ್ಳು_clip_glyph,       nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_path_start_func  (funcs, hb_gpu_paint_ತಳ್ಳು_clip_path_start,  nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_clip_path_end_func    (funcs, hb_gpu_paint_ತಳ್ಳು_clip_path_end,    nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_clip_func              (funcs, hb_gpu_paint_ತೆಗೆ_clip,              nullptr, nullptr);
    hb_paint_funcs_set_ತಳ್ಳು_group_func            (funcs, hb_gpu_paint_ತಳ್ಳು_group,            nullptr, nullptr);
    hb_paint_funcs_set_ತೆಗೆ_group_func             (funcs, hb_gpu_paint_ತೆಗೆ_group,             nullptr, nullptr);
    hb_paint_funcs_set_color_func                 (funcs, hb_gpu_paint_color,                 nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func       (funcs, hb_gpu_paint_linear_gradient,       nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        nullptr, nullptr);
    hb_paint_funcs_set_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  nullptr, nullptr);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_set_image_func                 (funcs, hb_gpu_paint_image,                 nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_gpu_paint_set_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_gpu_paint_get_budget, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_gpu_paint_get_budget_remaining, nullptr, nullptr);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_paint_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_gpu_paint_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
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
hb_gpu_paint_create_or_fail (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_create<hb_gpu_paint_t> ();
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
  ಪ್ರತ್ಯಾಗಮ hb_object_reference (paint);
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
ಶೂನ್ಯ
hb_gpu_paint_destroy (hb_gpu_paint_t *paint)
{
  ಯದಿ (!hb_object_should_destroy (paint))
    ಪ್ರತ್ಯಾಗಮ;

  hb_map_destroy (paint->custom_palette);
  ಪ್ರತಿ (hb_blob_t *b : paint->sub_blobs)
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
hb_gpu_paint_set_user_data (hb_gpu_paint_t     *paint,
			    hb_user_data_ಕೀಲಿ_t *key,
			    ಶೂನ್ಯ               *data,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_set_user_data (paint, key, data, destroy, replace);
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
ಶೂನ್ಯ *
hb_gpu_paint_get_user_data (ಧ್ರುವ hb_gpu_paint_t *paint,
			    hb_user_data_ಕೀಲಿ_t   *key)
{
  ಪ್ರತ್ಯಾಗಮ hb_object_get_user_data (paint, key);
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
hb_gpu_paint_get_funcs (ಧ್ರುವ hb_gpu_paint_t *paint HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ static_gpu_paint_funcs.get_unconst ();
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
ಶೂನ್ಯ
hb_gpu_paint_set_palette (hb_gpu_paint_t *paint,
			  unsigned        palette)
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
unsigned
hb_gpu_paint_get_palette (ಧ್ರುವ hb_gpu_paint_t *paint)
{
  ಪ್ರತ್ಯಾಗಮ paint->palette;
}

/**
 * hb_gpu_paint_clear_custom_palette_colors:
 * @paint: a GPU color-glyph paint encoder
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * Since: 14.2.0
 **/
ಶೂನ್ಯ
hb_gpu_paint_clear_custom_palette_colors (hb_gpu_paint_t *paint)
{
  ಯದಿ (paint->custom_palette)
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
hb_gpu_paint_set_custom_palette_color (hb_gpu_paint_t *paint,
				       unsigned ಪೂರ್ಣಾಙ್ಕ    color_index,
				       hb_color_t      color)
{
  ಯದಿ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ಯದಿ (unlikely (!paint->custom_palette))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  hb_map_set (paint->custom_palette, color_index, color);
  ಪ್ರತ್ಯಾಗಮ hb_map_allocation_successful (paint->custom_palette);
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
ಶೂನ್ಯ
hb_gpu_paint_set_scale (hb_gpu_paint_t *paint,
			ಪೂರ್ಣಾಙ್ಕ             x_scale,
			ಪೂರ್ಣಾಙ್ಕ             y_scale)
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
ಶೂನ್ಯ
hb_gpu_paint_get_scale (ಧ್ರುವ hb_gpu_paint_t *paint,
			ಪೂರ್ಣಾಙ್ಕ                  *x_scale,
			ಪೂರ್ಣಾಙ್ಕ                  *y_scale)
{
  ಯದಿ (x_scale) *x_scale = paint->x_scale;
  ಯದಿ (y_scale) *y_scale = paint->y_scale;
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
  ಪೂರ್ಣಾಙ್ಕ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_set_scale (paint, x_scale, y_scale);
  ಪ್ರತ್ಯಾಗಮ hb_font_paint_glyph_or_fail (font, glyph,
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
ಶೂನ್ಯ
hb_gpu_paint_glyph (hb_gpu_paint_t *paint,
		    hb_font_t      *font,
		    hb_codepoint_t  glyph)
{
  ಪೂರ್ಣಾಙ್ಕ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_set_scale (paint, x_scale, y_scale);
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

  ಯದಿ (unlikely (paint->unsupported))
    ಪ್ರತ್ಯಾಗಮ nullptr;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  ಯದಿ (paint->ಅಂಕೆ_ops == 0)
    ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  ಧ್ರುವ unsigned header_texels = 3;
  ಧ್ರುವ unsigned texel_bytes   = 8;

  unsigned ops_texels = paint->ops.length / 4;
  unsigned sub_bytes = 0;
  ಪ್ರತಿ (hb_blob_t *b : paint->sub_blobs)
    ಯದಿ (unlikely (hb_unsigned_add_overflows (sub_bytes,
					     hb_blob_get_length (b),
					     &sub_bytes)))
      ಪ್ರತ್ಯಾಗಮ nullptr;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  ಯದಿ (unlikely (sub_bytes % texel_bytes))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  unsigned total_bytes = header_texels * texel_bytes
			+ paint->ops.length * 2;
  ಯದಿ (unlikely (hb_unsigned_add_overflows (total_bytes, sub_bytes,
					   &total_bytes)))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  unsigned buf_capacity = 0;
  ವರ್ಣ *replaced_recycled_buf = nullptr;
  ವರ್ಣ *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, total_bytes,
					&buf_capacity, &replaced_recycled_buf);
  ಯದಿ (unlikely (!buf_raw))
    ಪ್ರತ್ಯಾಗಮ nullptr;
  int16_t *buf = (int16_t *) (ಶೂನ್ಯ *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  unsigned sub_offsets_ಎಣಿಕೆ = paint->sub_blobs.length;
  hb_vector_t<unsigned> sub_offsets;
  ಯದಿ (unlikely (!sub_offsets.resize (sub_offsets_ಎಣಿಕೆ)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
  unsigned cursor = header_texels + ops_texels;
  ಪ್ರತಿ (unsigned i = 0; i < sub_offsets_ಎಣಿಕೆ; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_length (paint->sub_blobs[i]) / texel_bytes;
  }

  /* Header. */
  buf[0] = (int16_t) paint->ಅಂಕೆ_ops;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  buf[4] = (int16_t) hb_clamp (paint->ext_min_x, -0x7fff, 0x7fff);
  buf[5] = (int16_t) hb_clamp (paint->ext_min_y, -0x7fff, 0x7fff);
  buf[6] = (int16_t) hb_clamp (paint->ext_max_x, -0x7fff, 0x7fff);
  buf[7] = (int16_t) hb_clamp (paint->ext_max_y, -0x7fff, 0x7fff);
  buf[8] = (int16_t) header_texels;
  buf[9]  = 0;
  buf[10] = 0;
  buf[11] = 0;

  /* Op stream: copy raw, then patch payload slots to texel
   * offsets where op_type indicates a sub_blob reference. */
  int16_t *ops_out = buf + header_texels * 4;
  hb_memcpy (ops_out, paint->ops.arrayZ, paint->ops.length * 2);

  unsigned i = 0;
  ಯಾವತ್ (i < paint->ops.length)
  {
    int16_t op_type = ops_out[i];
    ವಿಕಲ್ಪ (op_type)
    {
      ಪಕ್ಷ HB_GPU_PAINT_OP_LAYER_SOLID:
      ಪಕ್ಷ HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_out[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	unsigned slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	ಪ್ರತಿ (unsigned k = 0; k < 4; k++)
	{
	  ಯದಿ (!slots[k]) ಅನುವರ್ತ;
	  unsigned p = i + slots[k];
	  unsigned idx = ((uint16_t) ops_out[p] << 16)
		       |  (uint16_t) ops_out[p + 1];
	  ಯದಿ (idx < sub_offsets_ಎಣಿಕೆ)
	  {
	    unsigned off = sub_offsets[idx];
	    ops_out[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_out[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	ಭಙ್ಗ;
      }
      ಪಕ್ಷ HB_GPU_PAINT_OP_ತಳ್ಳು_GROUP:
      ಪಕ್ಷ HB_GPU_PAINT_OP_ತೆಗೆ_GROUP:
	i += 4;  /* 1 texel */
	ಭಙ್ಗ;
      ಸ್ವಭಾವ:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  ವರ್ಣ *dst = (ವರ್ಣ *) buf + header_texels * texel_bytes
			+ paint->ops.length * 2;
  ಪ್ರತಿ (hb_blob_t *b : paint->sub_blobs)
  {
    unsigned len = hb_blob_get_length (b);
    hb_memcpy (dst, hb_blob_get_data (b, nullptr), len);
    dst += len;
  }

  ಯದಿ (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->width     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->height    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = nullptr;
  ಪ್ರತ್ಯಾಗಮ hb_blob_t::recycle_finalize (buf_raw, buf_capacity, total_bytes,
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
ಶೂನ್ಯ
hb_gpu_paint_clear (hb_gpu_paint_t *paint)
{
  /* Use reset() not resize(0): if an earlier push_or_fail() set
   * the vector's error state (common under failing-malloc),
   * resize(0) short-circuits without clearing length.  A
   * subsequent clear would double-destroy the blob pointers left
   * in the array, or keep stale ops across encodes. */
  paint->ops.reset ();
  paint->ಅಂಕೆ_ops = 0;
  ಪ್ರತಿ (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  paint->sub_blobs.reset ();
  paint->sub_bytes = 0;
  paint->clip_depth = 0;
  paint->pending_clip_path = ಅಸತ್ಯ;
  paint->unsupported = ಅಸತ್ಯ;
  paint->recharge_budget ();
  paint->cur_transform = {1, 0, 0, 1, 0, 0};
  paint->transform_ರಾಶಿ.reset ();
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
ಶೂನ್ಯ
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = nullptr;
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
ಶೂನ್ಯ
hb_gpu_paint_recycle_blob (hb_gpu_paint_t *paint,
			   hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&paint->recycled_blob, blob);
}


#ಅನ್ತರ್ಭಾವ "hb-gpu-paint-fragment-glsl.hh"
#ಅನ್ತರ್ಭಾವ "hb-gpu-paint-fragment-msl.hh"
#ಅನ್ತರ್ಭಾವ "hb-gpu-paint-fragment-wgsl.hh"
#ಅನ್ತರ್ಭಾವ "hb-gpu-paint-fragment-hlsl.hh"

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
ಧ್ರುವ ವರ್ಣ *
hb_gpu_paint_shader_source (hb_gpu_shader_stage_t stage,
			    hb_gpu_shader_lang_t  lang)
{
  ವಿಕಲ್ಪ (stage) {
  ಪಕ್ಷ HB_GPU_SHADER_STAGE_FRAGMENT:
    ವಿಕಲ್ಪ (lang) {
    ಪಕ್ಷ HB_GPU_SHADER_LANG_GLSL: ಪ್ರತ್ಯಾಗಮ hb_gpu_paint_fragment_glsl;
    ಪಕ್ಷ HB_GPU_SHADER_LANG_MSL:  ಪ್ರತ್ಯಾಗಮ hb_gpu_paint_fragment_msl;
    ಪಕ್ಷ HB_GPU_SHADER_LANG_WGSL: ಪ್ರತ್ಯಾಗಮ hb_gpu_paint_fragment_wgsl;
    ಪಕ್ಷ HB_GPU_SHADER_LANG_HLSL: ಪ್ರತ್ಯಾಗಮ hb_gpu_paint_fragment_hlsl;
    ಪಕ್ಷ HB_GPU_SHADER_LANG_INVALID:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  ಪಕ್ಷ HB_GPU_SHADER_STAGE_VERTEX:
    ವಿಕಲ್ಪ (lang) {
    ಪಕ್ಷ HB_GPU_SHADER_LANG_GLSL:
    ಪಕ್ಷ HB_GPU_SHADER_LANG_MSL:
    ಪಕ್ಷ HB_GPU_SHADER_LANG_WGSL:
    ಪಕ್ಷ HB_GPU_SHADER_LANG_HLSL: ಪ್ರತ್ಯಾಗಮ "";
    ಪಕ್ಷ HB_GPU_SHADER_LANG_INVALID:
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  ಸ್ವಭಾವ:
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
}
