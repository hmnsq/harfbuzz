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

#شامل "hb.hh"

#شامل "hb-gpu.h"
#شامل "hb-gpu-paint.hh"
#شامل "hb-gpu-draw.hh"
#شامل "hb-draw.hh"
#شامل "hb-machinery.hh"
#شامل "hb-paint.hh"


/* ---- Paint callbacks ---- */

/* Op-type tags.  Must match the layout documented in
 * hb-gpu-paint.hh. */
شمار {
  HB_GPU_PAINT_OP_LAYER_SOLID    = 0,
  HB_GPU_PAINT_OP_LAYER_GRADIENT = 1,
  HB_GPU_PAINT_OP_ڈالو_گروہ     = 2,
  HB_GPU_PAINT_OP_نکالو_گروہ      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#تعریف_کرو HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#تعریف_کرو HB_GPU_PAINT_MAX_گروہ_گہرائی 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#تعریف_کرو HB_GPU_PAINT_MAX_CLIP_گہرائی  3      /* shader can intersect up to 3 clip outlines per layer */
#تعریف_کرو HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#تعریف_کرو HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#تعریف_کرو HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#تعریف_کرو HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

ساکن hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				   خالی             *paint_ڈیٹا,
				   غیرعلامتی          color_اشاریہ,
				   hb_color_t       *color,
				   خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  اگر (likely (c->custom_palette && hb_map_has (c->custom_palette, color_اشاریہ)))
  {
    *color = hb_map_get (c->custom_palette, color_اشاریہ);
    واپس سچ;
  }
  واپس جھوٹ;
}

ساکن خالی
hb_gpu_paint_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی             *paint_ڈیٹا,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;

  اگر (unlikely (c->clip_گہرائی >= HB_GPU_PAINT_MAX_CLIP_گہرائی))
  {
    c->unsupported = سچ;
    c->clip_گہرائی++;
    واپس;
  }
  c->clip_ڈھیر[c->clip_گہرائی] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_گہرائی++;
}

ساکن خالی
hb_gpu_paint_نکالو_clip (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
		       خالی             *paint_ڈیٹا,
		       خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  اگر (likely (c->clip_گہرائی > 0)) c->clip_گہرائی--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
ساکن hb_draw_funcs_t *
hb_gpu_paint_ڈالو_clip_راستہ_آغاز (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				   خالی             *paint_ڈیٹا,
				   خالی            **draw_ڈیٹا,
				   خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;

  اگر (unlikely (c->clip_گہرائی >= HB_GPU_PAINT_MAX_CLIP_گہرائی))
  {
    c->unsupported = سچ;
    *draw_ڈیٹا = خالی_اشاریہ;
    واپس خالی_اشاریہ;
  }

  اگر (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    اگر (unlikely (!c->scratch_draw))
    {
      c->unsupported = سچ;
      *draw_ڈیٹا = خالی_اشاریہ;
      واپس خالی_اشاریہ;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_سیٹ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_بقایا);
  hb_gpu_draw_سیٹ_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_راستہ_transform = c->cur_transform;
  c->pending_clip_راستہ = سچ;

  *draw_ڈیٹا = c->scratch_draw;
  واپس hb_gpu_draw_get_funcs (c->scratch_draw);
}

ساکن خالی
hb_gpu_paint_ڈالو_clip_راستہ_انتہا (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
				 خالی             *paint_ڈیٹا,
				 خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;

  اگر (unlikely (!c->pending_clip_راستہ))
    واپس;
  c->pending_clip_راستہ = جھوٹ;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_بقایا = hb_draw_get_budget_بقایا (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  اگر (unlikely (!blob || !c->ڈالو_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = سچ;
    واپس;
  }
  صحیح idx = (صحیح) (c->sub_blobs.لمبائی - 1);

  اگر (unlikely (c->clip_گہرائی >= HB_GPU_PAINT_MAX_CLIP_گہرائی))
  {
    c->unsupported = سچ;
    c->clip_گہرائی++;
    واپس;
  }

  صحیح x0 = ext.x_bearing;
  صحیح x1 = hb_saturate_جوڑو (ext.x_bearing, ext.چوڑائی);
  صحیح y0 = ext.y_bearing;
  صحیح y1 = hb_saturate_جوڑو (ext.y_bearing, ext.اونچائی);

  c->clip_ڈھیر[c->clip_گہرائی] = {
    HB_CODEPOINT_INVALID, خالی_اشاریہ,
    c->pending_clip_راستہ_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_گہرائی++;
}

ساکن خالی
hb_gpu_paint_ڈالو_transform (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			     خالی             *paint_ڈیٹا,
			     اعشاری xx, اعشاری yx,
			     اعشاری xy, اعشاری yy,
			     اعشاری dx, اعشاری dy,
			     خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!c->transform_ڈھیر.ڈالو_or_fail (c->cur_transform)))
  {
    c->unsupported = سچ;
    واپس;
  }
  hb_transform_t<اعشاری> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

ساکن خالی
hb_gpu_paint_نکالو_transform (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			    خالی             *paint_ڈیٹا,
			    خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!c->transform_ڈھیر.لمبائی))
    واپس;
  c->cur_transform = c->transform_ڈھیر.arrayZ[c->transform_ڈھیر.لمبائی - 1];
  c->transform_ڈھیر.shrink (c->transform_ڈھیر.لمبائی - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
ساکن خالی
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  اگر (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    واپس;
  }
  اگر (unlikely (!c->ops.resize (c->ops.لمبائی + 4)))
  {
    c->unsupported = سچ;
    واپس;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.لمبائی - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->عدد_ops++;
}

ساکن خالی
hb_gpu_paint_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			 خالی             *paint_ڈیٹا,
			 خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  c->گروہ_گہرائی++;
  اگر (unlikely (c->گروہ_گہرائی > HB_GPU_PAINT_MAX_گروہ_گہرائی))
  {
    c->unsupported = سچ;
    واپس;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_ڈالو_گروہ, 0);
}

ساکن خالی
hb_gpu_paint_نکالو_گروہ (hb_paint_funcs_t    *funcs HB_غیر_مستعمل,
			خالی                *paint_ڈیٹا,
			hb_paint_مرکب_منوال_t منوال,
			خالی                *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_ڈیٹا;
  منطقی was_اندر_range = c->گروہ_گہرائی <= HB_GPU_PAINT_MAX_گروہ_گہرائی;
  اگر (likely (c->گروہ_گہرائی > 0)) c->گروہ_گہرائی--;
  اگر (was_اندر_range)
    emit_control_op (c, HB_GPU_PAINT_OP_نکالو_گروہ, (int16_t) منوال);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
ساکن اندرونی int16_t
color_مقصد_q15 (غیرعلامتی byte_قدر)
{
  واپس (int16_t) ((byte_قدر * 32767u + 127u) / 255u);
}

ساکن اندرونی خالی
color_مقصد_q15_rgba (hb_color_t color, int16_t برآمد[4])
{
  برآمد[0] = color_مقصد_q15 (hb_color_get_red   (color));
  برآمد[1] = color_مقصد_q15 (hb_color_get_green (color));
  برآمد[2] = color_مقصد_q15 (hb_color_get_blue  (color));
  برآمد[3] = color_مقصد_q15 (hb_color_get_alpha (color));
}

ساکن اندرونی int16_t
clamp_i16 (اعشاری v)
{
  اگر (v <= -32768.f) واپس -32768;
  اگر (v >=  32767.f) واپس  32767;
  اگر (unlikely (std::isnan (v))) واپس 0;
  واپس (int16_t) v;
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
ساکن اندرونی خالی
linv_q10 (ثابت hb_transform_t<اعشاری> &t, int16_t برآمد[4])
{
  اعشاری det = t.xx * t.yy - t.xy * t.yx;
  اعشاری inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  برآمد[0] = clamp_i16 ( t.yy * inv * 1024.f);
  برآمد[1] = clamp_i16 (-t.xy * inv * 1024.f);
  برآمد[2] = clamp_i16 (-t.yx * inv * 1024.f);
  برآمد[3] = clamp_i16 ( t.xx * inv * 1024.f);
}

/* Transforming pen: applies an affine transform to each outline
 * point before forwarding to a downstream hb_draw_funcs_t.  The
 * downstream's inline draw-state bookkeeping runs against a
 * separate state (down_st) so transformed-space updates don't
 * pollute the caller's pre-transform state. */
ساخت hb_gpu_paint_pen_t
{
  hb_transform_t<اعشاری> transform;
  hb_draw_funcs_t  *dfuncs;
  خالی             *ڈیٹا;
  hb_draw_state_t   نیچے_کو_st;
  int64_t          *budget_بقایا;
};

ساکن خالی
hb_gpu_paint_pen_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			  خالی *ڈیٹا, hb_draw_state_t *st HB_غیر_مستعمل,
			  اعشاری مقصد_x, اعشاری مقصد_y,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  خودکار *c = (hb_gpu_paint_pen_t *) ڈیٹا;
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);
  c->dfuncs->move_مقصد (c->ڈیٹا, c->نیچے_کو_st, مقصد_x, مقصد_y);
}

ساکن خالی
hb_gpu_paint_pen_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			  خالی *ڈیٹا, hb_draw_state_t *st HB_غیر_مستعمل,
			  اعشاری مقصد_x, اعشاری مقصد_y,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  خودکار *c = (hb_gpu_paint_pen_t *) ڈیٹا;
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);
  c->dfuncs->سطر_مقصد (c->ڈیٹا, c->نیچے_کو_st, مقصد_x, مقصد_y);
}

ساکن خالی
hb_gpu_paint_pen_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			       خالی *ڈیٹا, hb_draw_state_t *st HB_غیر_مستعمل,
			       اعشاری c_x, اعشاری c_y,
			       اعشاری مقصد_x, اعشاری مقصد_y,
			       خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  خودکار *c = (hb_gpu_paint_pen_t *) ڈیٹا;
  c->transform.transform_نقطہ (c_x, c_y);
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);
  c->dfuncs->quadratic_مقصد (c->ڈیٹا, c->نیچے_کو_st, c_x, c_y, مقصد_x, مقصد_y);
}

ساکن خالی
hb_gpu_paint_pen_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			   خالی *ڈیٹا, hb_draw_state_t *st HB_غیر_مستعمل,
			   اعشاری c1_x, اعشاری c1_y,
			   اعشاری c2_x, اعشاری c2_y,
			   اعشاری مقصد_x, اعشاری مقصد_y,
			   خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  خودکار *c = (hb_gpu_paint_pen_t *) ڈیٹا;
  c->transform.transform_نقطہ (c1_x, c1_y);
  c->transform.transform_نقطہ (c2_x, c2_y);
  c->transform.transform_نقطہ (مقصد_x, مقصد_y);
  c->dfuncs->cubic_مقصد (c->ڈیٹا, c->نیچے_کو_st, c1_x, c1_y, c2_x, c2_y, مقصد_x, مقصد_y);
}

ساکن خالی
hb_gpu_paint_pen_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			     خالی *ڈیٹا, hb_draw_state_t *st HB_غیر_مستعمل,
			     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  خودکار *c = (hb_gpu_paint_pen_t *) ڈیٹا;
  c->dfuncs->close_راستہ (c->ڈیٹا, c->نیچے_کو_st);
}

ساکن int64_t *
hb_gpu_paint_pen_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_gpu_paint_pen_t *) draw_ڈیٹا)->budget_بقایا;
}

ساکن اندرونی خالی free_static_gpu_paint_pen_funcs ();

ساکن ساخت hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_مقصد_func      (funcs, hb_gpu_paint_pen_move_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func      (funcs, hb_gpu_paint_pen_سطر_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_gpu_paint_pen_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func     (funcs, hb_gpu_paint_pen_cubic_مقصد,     خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func   (funcs, hb_gpu_paint_pen_close_راستہ,   خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_gpu_paint_pen_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_gpu_paint_pen_funcs);
    واپس funcs;
  }
} static_gpu_paint_pen_funcs;

ساکن اندرونی خالی
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
ساکن صحیح
emit_clip_sub_blob (hb_gpu_paint_t *c,
		    hb_gpu_paint_t::pending_clip_t &clip)
{
  /* Path clips were already encoded into sub_blobs at
   * push_clip_path_end time; glyph clips are cached here on first
   * use.  Either way: just accumulate extents and hand back the
   * recorded index. */
  اگر (clip.sub_blob_اشاریہ >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    واپس clip.sub_blob_اشاریہ;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  اگر (unlikely (c->sub_blobs.لمبائی >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = سچ;
    واپس -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  اگر (unlikely (c->budget_بقایا < 0))
  {
    c->unsupported = سچ;
    واپس -1;
  }

  اگر (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    اگر (unlikely (!c->scratch_draw))
    {
      c->unsupported = سچ;
      واپس -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_سیٹ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_بقایا);

  منطقی ok;
  اگر (clip.transform.is_شناختی_عنصر ())
  {
    /* Fast path: feed the glyph outline straight into the draw
     * encoder with no adapter. */
    ok = hb_gpu_draw_glyph_or_fail (c->scratch_draw, clip.font, clip.glyph);
  }
  ورنہ
  {
    /* Transform each outline point by the transform that was
     * current at push_clip_glyph time -- NOT the innermost
     * cur_transform, which may have additional PaintTransform
     * wrappers from the paint child underneath. */
    صحیح x_scale, y_scale;
    hb_font_get_scale (clip.font, &x_scale, &y_scale);
    hb_gpu_draw_سیٹ_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.ڈیٹا      = c->scratch_draw;
    pen.نیچے_کو_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_بقایا = &c->scratch_draw->budget_بقایا;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_راستہ (pen.ڈیٹا, pen.نیچے_کو_st);
  }
  c->budget_بقایا = hb_draw_get_budget_بقایا (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  اگر (!ok)
    واپس -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  اگر (unlikely (!blob || !c->ڈالو_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = سچ;
    واپس -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  صحیح x0 = ext.x_bearing;
  صحیح x1 = hb_saturate_جوڑو (ext.x_bearing, ext.چوڑائی);
  صحیح y0 = ext.y_bearing;
  صحیح y1 = hb_saturate_جوڑو (ext.y_bearing, ext.اونچائی);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  صحیح idx = (صحیح) (c->sub_blobs.لمبائی - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_اشاریہ = idx;
  clip.ext_x0 = hb_min (x0, x1);
  clip.ext_y0 = hb_min (y0, y1);
  clip.ext_x1 = hb_max (x0, x1);
  clip.ext_y1 = hb_max (y0, y1);

  واپس idx;
}

/* Emit a sub-blob for every clip currently on the stack and return
 * their indices in @out (set to -1 for unused slots).  Returns
 * false if any clip's outline is empty (caller should skip the
 * layer entirely) or hb_gpu_draw_encode failed. */
ساکن منطقی
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, صحیح برآمد[HB_GPU_PAINT_MAX_CLIP_گہرائی])
{
  برائے (غیرعلامتی i = 0; i < HB_GPU_PAINT_MAX_CLIP_گہرائی; i++)
    برآمد[i] = -1;
  برائے (غیرعلامتی i = 0; i < hb_min (c->clip_گہرائی, (غیرعلامتی) HB_GPU_PAINT_MAX_CLIP_گہرائی); i++)
  {
    برآمد[i] = emit_clip_sub_blob (c, c->clip_ڈھیر[i]);
    اگر (برآمد[i] < 0)
      واپس جھوٹ;
  }
  واپس سچ;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
ساکن int16_t *
emit_gradient_op_سرخی (hb_gpu_paint_t *c, غیرعلامتی subtype,
			 ثابت صحیح clip_idx[HB_GPU_PAINT_MAX_CLIP_گہرائی])
{
  اگر (unlikely (!c->ops.resize (c->ops.لمبائی + 12)))
  { c->unsupported = سچ; واپس خالی_اشاریہ; }
  int16_t *o = &c->ops.arrayZ[c->ops.لمبائی - 12];
  int16_t flags = (int16_t) subtype;
  اگر (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  اگر (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_GRADIENT;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  واپس o + 8;  /* texel 2: gradient meta -- caller fills */
}

ساکن خالی
hb_gpu_paint_emit_solid (hb_gpu_paint_t *c,
			 hb_bool_t       is_foreground,
			 hb_color_t      color)
{
  اگر (unlikely (c->unsupported))
    واپس;
  اگر (unlikely (c->clip_گہرائی == 0))
    واپس;
  اگر (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    واپس;
  }

  صحیح clip_idx[HB_GPU_PAINT_MAX_CLIP_گہرائی];
  اگر (!emit_all_clip_sub_blobs (c, clip_idx))
    واپس;

  /* Emit LAYER_SOLID op: 3 texels (12 int16s).
   *   texel 0: op_type, flags, clip1_hi, clip1_lo
   *   texel 1: clip2_hi, clip2_lo, 0, 0  (only meaningful if HAS_CLIP2)
   *   texel 2: r_q15, g_q15, b_q15, a_q15
   * Payloads hold sub_blob indices; hb_gpu_paint_encode() patches
   * them into texel offsets at serialize time.
   * flags bit 0: use shader foreground uniform instead of the
   * baked color (so runtime foreground-color changes still work).
   * flags bit 8: a second clip outline is present in texel 1. */
  اگر (unlikely (!c->ops.resize (c->ops.لمبائی + 12)))
  {
    c->unsupported = سچ;
    واپس;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.لمبائی - 12];
  int16_t flags = (int16_t) (is_foreground ? HB_GPU_PAINT_FLAG_IS_FOREGROUND : 0);
  اگر (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  اگر (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_SOLID;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  color_مقصد_q15_rgba (color, o + 8);
  c->عدد_ops++;
}

/* Append a color line (@count stops) to @grad_data in the encoded
 * stop format used by gradient ops: 2 texels per stop.
 *   texel a: (offset_q15, flags, _, _) with flags bit 0 = is_foreground
 *   texel b: (r_q15, g_q15, b_q15, a_q15)
 * Returns false on allocation failure. */
ساکن منطقی
append_color_stops (hb_سمتیہ_t<int16_t> &grad_ڈیٹا,
		    ثابت hb_color_stop_t *stops,
		    غیرعلامتی گنتی)
{
  غیرعلامتی بنیاد = grad_ڈیٹا.لمبائی;
  اگر (unlikely (!grad_ڈیٹا.resize (بنیاد + گنتی * 8)))
    واپس جھوٹ;
  int16_t *p = grad_ڈیٹا.arrayZ + بنیاد;
  برائے (غیرعلامتی i = 0; i < گنتی; i++)
  {
    اعشاری off = stops[i].انحراف;
    اگر (off < -1.f) off = -1.f;
    ورنہ اگر (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_مقصد_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  واپس سچ;
}

ساکن خالی
hb_gpu_paint_emit_خطی (hb_gpu_paint_t  *c,
			  hb_color_سطر_t *color_سطر,
			  اعشاری x0, اعشاری y0,
			  اعشاری x1, اعشاری y1,
			  اعشاری x2, اعشاری y2)
{
  اگر (unlikely (c->unsupported))
    واپس;
  اگر (unlikely (c->clip_گہرائی == 0))
    واپس;
  اگر (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    واپس;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  اگر (unlikely (!c->fetch_color_stops (color_سطر)))
    واپس;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  غیرعلامتی got = c->color_stops_scratch.لمبائی;

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  /* Emit clip sub-blob(s) first. */
  صحیح clip_idx[HB_GPU_PAINT_MAX_CLIP_گہرائی];
  اگر (!emit_all_clip_sub_blobs (c, clip_idx)) واپس;

  /* Build gradient params sub-blob.
   *   texel 0: (p0_rendered_x, p0_rendered_y, d_canonical_x, d_canonical_y)
   *     i16 font units; d = p1 - p0 stored in untransformed space
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * The shader computes p_canonical = L^-1 * (renderCoord - p0_rendered)
   * and evaluates t = dot(p_canonical, d) / dot(d, d) in
   * untransformed space, so affine distortions of the axis are
   * handled exactly rather than approximated. */
  hb_سمتیہ_t<int16_t> grad_ڈیٹا;
  اگر (unlikely (!grad_ڈیٹا.resize (8)))
  { c->unsupported = سچ; واپس; }

  اعشاری lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  اعشاری dx = lx1 - lx0, dy = ly1 - ly0;
  اعشاری ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  اعشاری ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  اعشاری p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_نقطہ (p0x_r, p0y_r);
  grad_ڈیٹا[0] = clamp_i16 (p0x_r);
  grad_ڈیٹا[1] = clamp_i16 (p0y_r);
  grad_ڈیٹا[2] = clamp_i16 (ax1 - ax0);
  grad_ڈیٹا[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ڈیٹا[4] = Linv[0];
  grad_ڈیٹا[5] = Linv[1];
  grad_ڈیٹا[6] = Linv[2];
  grad_ڈیٹا[7] = Linv[3];

  اگر (unlikely (!append_color_stops (grad_ڈیٹا, stops, got)))
  { c->unsupported = سچ; واپس; }

  غیرعلامتی grad_بائٹیں = grad_ڈیٹا.لمبائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_ڈیٹا.arrayZ,
					 grad_بائٹیں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_اشاریہ, خالی_اشاریہ);
  اگر (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    واپس;
  }
  غیرعلامتی grad_idx = c->sub_blobs.لمبائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 0 /*خطی*/, clip_idx);
  اگر (unlikely (!meta)) واپس;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساکن خالی
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_سطر_t *color_سطر,
			  اعشاری x0, اعشاری y0, اعشاری r0,
			  اعشاری x1, اعشاری y1, اعشاری r1)
{
  اگر (unlikely (c->unsupported))
    واپس;
  اگر (unlikely (c->clip_گہرائی == 0))
    واپس;
  اگر (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    واپس;
  }

  اگر (unlikely (!c->fetch_color_stops (color_سطر)))
    واپس;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  غیرعلامتی got = c->color_stops_scratch.لمبائی;

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  صحیح clip_idx[HB_GPU_PAINT_MAX_CLIP_گہرائی];
  اگر (!emit_all_clip_sub_blobs (c, clip_idx)) واپس;

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
  hb_سمتیہ_t<int16_t> grad_ڈیٹا;
  اگر (unlikely (!grad_ڈیٹا.resize (12)))
  { c->unsupported = سچ; واپس; }
  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  اعشاری dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  اعشاری cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  اعشاری cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  اعشاری cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_نقطہ (cx0_r, cy0_r);
  grad_ڈیٹا[0] = clamp_i16 (cx0_r);
  grad_ڈیٹا[1] = clamp_i16 (cy0_r);
  grad_ڈیٹا[2] = clamp_i16 (cx1 - cx0);
  grad_ڈیٹا[3] = clamp_i16 (cy1 - cy0);
  grad_ڈیٹا[4] = clamp_i16 (cr0);
  grad_ڈیٹا[5] = clamp_i16 (cr1);
  grad_ڈیٹا[6] = 0;
  grad_ڈیٹا[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ڈیٹا[8]  = Linv[0];
  grad_ڈیٹا[9]  = Linv[1];
  grad_ڈیٹا[10] = Linv[2];
  grad_ڈیٹا[11] = Linv[3];

  اگر (unlikely (!append_color_stops (grad_ڈیٹا, stops, got)))
  { c->unsupported = سچ; واپس; }

  غیرعلامتی grad_بائٹیں = grad_ڈیٹا.لمبائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_ڈیٹا.arrayZ,
					 grad_بائٹیں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_اشاریہ, خالی_اشاریہ);
  اگر (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    واپس;
  }
  غیرعلامتی grad_idx = c->sub_blobs.لمبائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 1 /* radial */, clip_idx);
  اگر (unlikely (!meta)) واپس;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساکن خالی
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
		    خالی             *paint_ڈیٹا,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_ڈیٹا, is_foreground, color);
}

ساکن خالی
hb_gpu_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی             *paint_ڈیٹا,
			      hb_color_سطر_t  *color_سطر,
			      اعشاری x0, اعشاری y0,
			      اعشاری x1, اعشاری y1,
			      اعشاری x2, اعشاری y2,
			      خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_emit_خطی ((hb_gpu_paint_t *) paint_ڈیٹا, color_سطر,
			    x0, y0, x1, y1, x2, y2);
}

ساکن خالی
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			      خالی             *paint_ڈیٹا,
			      hb_color_سطر_t  *color_سطر,
			      اعشاری x0, اعشاری y0, اعشاری r0,
			      اعشاری x1, اعشاری y1, اعشاری r1,
			      خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_ڈیٹا, color_سطر,
			    x0, y0, r0, x1, y1, r1);
}

ساکن خالی
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_سطر_t *color_سطر,
			 اعشاری cx, اعشاری cy,
			 اعشاری آغاز_angle, اعشاری انتہا_angle)
{
  اگر (unlikely (c->unsupported))
    واپس;
  اگر (unlikely (c->clip_گہرائی == 0))
    واپس;
  اگر (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    واپس;
  }

  غیرعلامتی گنتی = hb_color_سطر_get_color_stops (color_سطر, 0, خالی_اشاریہ, خالی_اشاریہ);
  اگر (unlikely (!گنتی))
    واپس;
  hb_color_stop_t ڈھیر_stops[16];
  hb_color_stop_t *stops = ڈھیر_stops;
  hb_color_stop_t *ڈھیر_stops_2 = خالی_اشاریہ;
  اگر (گنتی > 16)
  {
    ڈھیر_stops_2 = (hb_color_stop_t *) hb_malloc2 (گنتی, حجم (hb_color_stop_t));
    اگر (unlikely (!ڈھیر_stops_2)) { c->unsupported = سچ; واپس; }
    stops = ڈھیر_stops_2;
  }
  غیرعلامتی got = گنتی;
  hb_color_سطر_get_color_stops (color_سطر, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  صحیح clip_idx[HB_GPU_PAINT_MAX_CLIP_گہرائی];
  اگر (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (ڈھیر_stops_2); واپس; }

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
  hb_سمتیہ_t<int16_t> grad_ڈیٹا;
  اگر (unlikely (!grad_ڈیٹا.resize (8)))
  { c->unsupported = سچ; hb_free (ڈھیر_stops_2); واپس; }

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  اعشاری a_وسعت = انتہا_angle - آغاز_angle;
  اعشاری a_lo = آغاز_angle + mn * a_وسعت;
  اعشاری a_hi = آغاز_angle + mx * a_وسعت;
  خودکار rad_مقصد_q14_pi = [] (اعشاری rad) -> int16_t {
    اعشاری v = rad * (اعشاری) (1.0 / M_PI);  /* fraction of pi */
    اگر (v <= -2.f) واپس -32768;
    اگر (v >=  2.f) واپس  32767;
    واپس (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  اعشاری tcx = cx, tcy = cy;
  c->cur_transform.transform_نقطہ (tcx, tcy);
  grad_ڈیٹا[0] = clamp_i16 (tcx);
  grad_ڈیٹا[1] = clamp_i16 (tcy);
  grad_ڈیٹا[2] = rad_مقصد_q14_pi (a_lo);
  grad_ڈیٹا[3] = rad_مقصد_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_ڈیٹا[4] = Linv[0];
  grad_ڈیٹا[5] = Linv[1];
  grad_ڈیٹا[6] = Linv[2];
  grad_ڈیٹا[7] = Linv[3];

  اگر (unlikely (!append_color_stops (grad_ڈیٹا, stops, got)))
  { c->unsupported = سچ; hb_free (ڈھیر_stops_2); واپس; }
  hb_free (ڈھیر_stops_2);

  غیرعلامتی grad_بائٹیں = grad_ڈیٹا.لمبائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_ڈیٹا.arrayZ,
					 grad_بائٹیں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_اشاریہ, خالی_اشاریہ);
  اگر (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    واپس;
  }
  غیرعلامتی grad_idx = c->sub_blobs.لمبائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 2 /* sweep */, clip_idx);
  اگر (unlikely (!meta)) واپس;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساکن خالی
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_غیر_مستعمل,
			     خالی             *paint_ڈیٹا,
			     hb_color_سطر_t  *color_سطر,
			     اعشاری cx, اعشاری cy,
			     اعشاری آغاز_angle,
			     اعشاری انتہا_angle,
			     خالی             *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_ڈیٹا, color_سطر,
			   cx, cy, آغاز_angle, انتہا_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
ساکن hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_غیر_مستعمل,
		    خالی               *paint_ڈیٹا,
		    hb_blob_t          *image   HB_غیر_مستعمل,
		    غیرعلامتی صحیح        چوڑائی   HB_غیر_مستعمل,
		    غیرعلامتی صحیح        اونچائی  HB_غیر_مستعمل,
		    hb_لیبل_t            format  HB_غیر_مستعمل,
		    اعشاری               slant   HB_غیر_مستعمل,
		    hb_glyph_extents_t *extents HB_غیر_مستعمل,
		    خالی               *user_ڈیٹا HB_غیر_مستعمل)
{
  ((hb_gpu_paint_t *) paint_ڈیٹا)->unsupported = سچ;
  واپس جھوٹ;
}

ساکن hb_bool_t
hb_gpu_paint_سیٹ_budget (hb_paint_funcs_t *, خالی *paint_ڈیٹا,
			 int64_t budget, خالی *)
{
  خودکار *paint = (hb_gpu_paint_t *) paint_ڈیٹا;
  paint->budget = budget;
  paint->recharge_budget ();
  واپس سچ;
}

ساکن int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, خالی *paint_ڈیٹا, خالی *)
{
  واپس ((hb_gpu_paint_t *) paint_ڈیٹا)->budget;
}

ساکن int64_t *
hb_gpu_paint_get_budget_بقایا (hb_paint_funcs_t *, خالی *paint_ڈیٹا, خالی *)
{
  واپس &((hb_gpu_paint_t *) paint_ڈیٹا)->budget_بقایا;
}

ساکن اندرونی خالی free_static_gpu_paint_funcs ();

ساکن ساخت hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func        (funcs, hb_gpu_paint_ڈالو_transform,        خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_transform_func         (funcs, hb_gpu_paint_نکالو_transform,         خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func       (funcs, hb_gpu_paint_ڈالو_clip_glyph,       خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_آغاز_func  (funcs, hb_gpu_paint_ڈالو_clip_راستہ_آغاز,  خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_انتہا_func    (funcs, hb_gpu_paint_ڈالو_clip_راستہ_انتہا,    خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func              (funcs, hb_gpu_paint_نکالو_clip,              خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func            (funcs, hb_gpu_paint_ڈالو_گروہ,            خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func             (funcs, hb_gpu_paint_نکالو_گروہ,             خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func                 (funcs, hb_gpu_paint_color,                 خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func       (funcs, hb_gpu_paint_خطی_gradient,       خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  خالی_اشاریہ, خالی_اشاریہ);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_سیٹ_image_func                 (funcs, hb_gpu_paint_image,                 خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_gpu_paint_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_gpu_paint_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_gpu_paint_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_paint_funcs);

    واپس funcs;
  }
} static_gpu_paint_funcs;

ساکن اندرونی خالی
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
hb_gpu_paint_create_or_fail (خالی)
{
  واپس hb_object_create<hb_gpu_paint_t> ();
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
  واپس hb_object_reference (paint);
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
خالی
hb_gpu_paint_destroy (hb_gpu_paint_t *paint)
{
  اگر (!hb_object_should_destroy (paint))
    واپس;

  hb_map_destroy (paint->custom_palette);
  برائے (hb_blob_t *b : paint->sub_blobs)
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
hb_gpu_paint_سیٹ_user_ڈیٹا (hb_gpu_paint_t     *paint,
			    hb_user_ڈیٹا_کلید_t *key,
			    خالی               *ڈیٹا,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (paint, key, ڈیٹا, destroy, replace);
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
خالی *
hb_gpu_paint_get_user_ڈیٹا (ثابت hb_gpu_paint_t *paint,
			    hb_user_ڈیٹا_کلید_t   *key)
{
  واپس hb_object_get_user_ڈیٹا (paint, key);
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
hb_gpu_paint_get_funcs (ثابت hb_gpu_paint_t *paint HB_غیر_مستعمل)
{
  واپس static_gpu_paint_funcs.get_unconst ();
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
خالی
hb_gpu_paint_سیٹ_palette (hb_gpu_paint_t *paint,
			  غیرعلامتی        palette)
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
غیرعلامتی
hb_gpu_paint_get_palette (ثابت hb_gpu_paint_t *paint)
{
  واپس paint->palette;
}

/**
 * hb_gpu_paint_clear_custom_palette_colors:
 * @paint: a GPU color-glyph paint encoder
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * Since: 14.2.0
 **/
خالی
hb_gpu_paint_clear_custom_palette_colors (hb_gpu_paint_t *paint)
{
  اگر (paint->custom_palette)
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
hb_gpu_paint_سیٹ_custom_palette_color (hb_gpu_paint_t *paint,
				       غیرعلامتی صحیح    color_اشاریہ,
				       hb_color_t      color)
{
  اگر (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    اگر (unlikely (!paint->custom_palette))
      واپس جھوٹ;
  }
  hb_map_سیٹ (paint->custom_palette, color_اشاریہ, color);
  واپس hb_map_تخصیص_successful (paint->custom_palette);
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
خالی
hb_gpu_paint_سیٹ_scale (hb_gpu_paint_t *paint,
			صحیح             x_scale,
			صحیح             y_scale)
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
خالی
hb_gpu_paint_get_scale (ثابت hb_gpu_paint_t *paint,
			صحیح                  *x_scale,
			صحیح                  *y_scale)
{
  اگر (x_scale) *x_scale = paint->x_scale;
  اگر (y_scale) *y_scale = paint->y_scale;
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
  صحیح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_سیٹ_scale (paint, x_scale, y_scale);
  واپس hb_font_paint_glyph_or_fail (font, glyph,
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
خالی
hb_gpu_paint_glyph (hb_gpu_paint_t *paint,
		    hb_font_t      *font,
		    hb_codepoint_t  glyph)
{
  صحیح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_سیٹ_scale (paint, x_scale, y_scale);
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

  اگر (unlikely (paint->unsupported))
    واپس خالی_اشاریہ;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  اگر (paint->عدد_ops == 0)
    واپس hb_blob_get_خالی_خانے ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  ثابت غیرعلامتی سرخی_texels = 3;
  ثابت غیرعلامتی texel_بائٹیں   = 8;

  غیرعلامتی ops_texels = paint->ops.لمبائی / 4;
  غیرعلامتی sub_بائٹیں = 0;
  برائے (hb_blob_t *b : paint->sub_blobs)
    اگر (unlikely (hb_unsigned_جوڑو_overflows (sub_بائٹیں,
					     hb_blob_get_لمبائی (b),
					     &sub_بائٹیں)))
      واپس خالی_اشاریہ;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  اگر (unlikely (sub_بائٹیں % texel_بائٹیں))
    واپس خالی_اشاریہ;

  غیرعلامتی کل_بائٹیں = سرخی_texels * texel_بائٹیں
			+ paint->ops.لمبائی * 2;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (کل_بائٹیں, sub_بائٹیں,
					   &کل_بائٹیں)))
    واپس خالی_اشاریہ;

  غیرعلامتی buf_استعداد = 0;
  حرف *replaced_recycled_buf = خالی_اشاریہ;
  حرف *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, کل_بائٹیں,
					&buf_استعداد, &replaced_recycled_buf);
  اگر (unlikely (!buf_raw))
    واپس خالی_اشاریہ;
  int16_t *buf = (int16_t *) (خالی *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  غیرعلامتی sub_offsets_گنتی = paint->sub_blobs.لمبائی;
  hb_سمتیہ_t<غیرعلامتی> sub_offsets;
  اگر (unlikely (!sub_offsets.resize (sub_offsets_گنتی)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    واپس خالی_اشاریہ;
  }
  غیرعلامتی cursor = سرخی_texels + ops_texels;
  برائے (غیرعلامتی i = 0; i < sub_offsets_گنتی; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_لمبائی (paint->sub_blobs[i]) / texel_بائٹیں;
  }

  /* Header. */
  buf[0] = (int16_t) paint->عدد_ops;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  buf[4] = (int16_t) hb_clamp (paint->ext_min_x, -0x7fff, 0x7fff);
  buf[5] = (int16_t) hb_clamp (paint->ext_min_y, -0x7fff, 0x7fff);
  buf[6] = (int16_t) hb_clamp (paint->ext_max_x, -0x7fff, 0x7fff);
  buf[7] = (int16_t) hb_clamp (paint->ext_max_y, -0x7fff, 0x7fff);
  buf[8] = (int16_t) سرخی_texels;
  buf[9]  = 0;
  buf[10] = 0;
  buf[11] = 0;

  /* Op stream: copy raw, then patch payload slots to texel
   * offsets where op_type indicates a sub_blob reference. */
  int16_t *ops_برآمد = buf + سرخی_texels * 4;
  hb_memcpy (ops_برآمد, paint->ops.arrayZ, paint->ops.لمبائی * 2);

  غیرعلامتی i = 0;
  جب (i < paint->ops.لمبائی)
  {
    int16_t op_type = ops_برآمد[i];
    انتخاب (op_type)
    {
      حالت HB_GPU_PAINT_OP_LAYER_SOLID:
      حالت HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_برآمد[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	غیرعلامتی slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	برائے (غیرعلامتی k = 0; k < 4; k++)
	{
	  اگر (!slots[k]) جاری;
	  غیرعلامتی p = i + slots[k];
	  غیرعلامتی idx = ((uint16_t) ops_برآمد[p] << 16)
		       |  (uint16_t) ops_برآمد[p + 1];
	  اگر (idx < sub_offsets_گنتی)
	  {
	    غیرعلامتی off = sub_offsets[idx];
	    ops_برآمد[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_برآمد[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	توڑو;
      }
      حالت HB_GPU_PAINT_OP_ڈالو_گروہ:
      حالت HB_GPU_PAINT_OP_نکالو_گروہ:
	i += 4;  /* 1 texel */
	توڑو;
      طےشدہ:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	واپس خالی_اشاریہ;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  حرف *dst = (حرف *) buf + سرخی_texels * texel_بائٹیں
			+ paint->ops.لمبائی * 2;
  برائے (hb_blob_t *b : paint->sub_blobs)
  {
    غیرعلامتی len = hb_blob_get_لمبائی (b);
    hb_memcpy (dst, hb_blob_get_ڈیٹا (b, خالی_اشاریہ), len);
    dst += len;
  }

  اگر (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->چوڑائی     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->اونچائی    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = خالی_اشاریہ;
  واپس hb_blob_t::recycle_finalize (buf_raw, buf_استعداد, کل_بائٹیں,
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
خالی
hb_gpu_paint_clear (hb_gpu_paint_t *paint)
{
  /* Use reset() not resize(0): if an earlier push_or_fail() set
   * the vector's error state (common under failing-malloc),
   * resize(0) short-circuits without clearing length.  A
   * subsequent clear would double-destroy the blob pointers left
   * in the array, or keep stale ops across encodes. */
  paint->ops.reset ();
  paint->عدد_ops = 0;
  برائے (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  paint->sub_blobs.reset ();
  paint->sub_بائٹیں = 0;
  paint->clip_گہرائی = 0;
  paint->pending_clip_راستہ = جھوٹ;
  paint->unsupported = جھوٹ;
  paint->recharge_budget ();
  paint->cur_transform = {1, 0, 0, 1, 0, 0};
  paint->transform_ڈھیر.reset ();
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
خالی
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = خالی_اشاریہ;
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
خالی
hb_gpu_paint_recycle_blob (hb_gpu_paint_t *paint,
			   hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&paint->recycled_blob, blob);
}


#شامل "hb-gpu-paint-fragment-glsl.hh"
#شامل "hb-gpu-paint-fragment-msl.hh"
#شامل "hb-gpu-paint-fragment-wgsl.hh"
#شامل "hb-gpu-paint-fragment-hlsl.hh"

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
ثابت حرف *
hb_gpu_paint_shader_ماخذ (hb_gpu_shader_stage_t stage,
			    hb_gpu_shader_lang_t  lang)
{
  انتخاب (stage) {
  حالت HB_GPU_SHADER_STAGE_FRAGMENT:
    انتخاب (lang) {
    حالت HB_GPU_SHADER_LANG_GLSL: واپس hb_gpu_paint_fragment_glsl;
    حالت HB_GPU_SHADER_LANG_MSL:  واپس hb_gpu_paint_fragment_msl;
    حالت HB_GPU_SHADER_LANG_WGSL: واپس hb_gpu_paint_fragment_wgsl;
    حالت HB_GPU_SHADER_LANG_HLSL: واپس hb_gpu_paint_fragment_hlsl;
    حالت HB_GPU_SHADER_LANG_INVALID:
    طےشدہ: واپس خالی_اشاریہ;
    }
  حالت HB_GPU_SHADER_STAGE_راس:
    انتخاب (lang) {
    حالت HB_GPU_SHADER_LANG_GLSL:
    حالت HB_GPU_SHADER_LANG_MSL:
    حالت HB_GPU_SHADER_LANG_WGSL:
    حالت HB_GPU_SHADER_LANG_HLSL: واپس "";
    حالت HB_GPU_SHADER_LANG_INVALID:
    طےشدہ: واپس خالی_اشاریہ;
    }
  طےشدہ:
    واپس خالی_اشاریہ;
  }
}
