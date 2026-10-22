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

#ضمن "hb.hh"

#ضمن "hb-gpu.h"
#ضمن "hb-gpu-paint.hh"
#ضمن "hb-gpu-draw.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-machinery.hh"
#ضمن "hb-paint.hh"


/* ---- Paint callbacks ---- */

/* Op-type tags.  Must match the layout documented in
 * hb-gpu-paint.hh. */
تعداد {
  HB_GPU_PAINT_OP_LAYER_SOLID    = 0,
  HB_GPU_PAINT_OP_LAYER_GRADIENT = 1,
  HB_GPU_PAINT_OP_ټېل_ډله     = 2,
  HB_GPU_PAINT_OP_راوباسه_ډله      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#تعریف_کړه HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#تعریف_کړه HB_GPU_PAINT_MAX_ډله_ژوروالی 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#تعریف_کړه HB_GPU_PAINT_MAX_CLIP_ژوروالی  3      /* shader can intersect up to 3 clip outlines per layer */
#تعریف_کړه HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#تعریف_کړه HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#تعریف_کړه HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#تعریف_کړه HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

ساكن hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				   فراغ             *paint_معلومات,
				   بینښه          color_شاخص,
				   hb_color_t       *color,
				   فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  إذا (likely (c->custom_palette && hb_map_has (c->custom_palette, color_شاخص)))
  {
    *color = hb_map_get (c->custom_palette, color_شاخص);
    أرجع ریښتیا;
  }
  أرجع خطأ;
}

ساكن فراغ
hb_gpu_paint_ټېل_clip_glyph (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ             *paint_معلومات,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;

  إذا (unlikely (c->clip_ژوروالی >= HB_GPU_PAINT_MAX_CLIP_ژوروالی))
  {
    c->unsupported = ریښتیا;
    c->clip_ژوروالی++;
    أرجع;
  }
  c->clip_ډېری[c->clip_ژوروالی] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_ژوروالی++;
}

ساكن فراغ
hb_gpu_paint_راوباسه_clip (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
		       فراغ             *paint_معلومات,
		       فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  إذا (likely (c->clip_ژوروالی > 0)) c->clip_ژوروالی--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
ساكن hb_draw_funcs_t *
hb_gpu_paint_ټېل_clip_لار_پیل (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				   فراغ             *paint_معلومات,
				   فراغ            **draw_معلومات,
				   فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;

  إذا (unlikely (c->clip_ژوروالی >= HB_GPU_PAINT_MAX_CLIP_ژوروالی))
  {
    c->unsupported = ریښتیا;
    *draw_معلومات = تش_اشاره;
    أرجع تش_اشاره;
  }

  إذا (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    إذا (unlikely (!c->scratch_draw))
    {
      c->unsupported = ریښتیا;
      *draw_معلومات = تش_اشاره;
      أرجع تش_اشاره;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_سېټ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_پاتې_شوی);
  hb_gpu_draw_سېټ_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_لار_transform = c->cur_transform;
  c->pending_clip_لار = ریښتیا;

  *draw_معلومات = c->scratch_draw;
  أرجع hb_gpu_draw_get_funcs (c->scratch_draw);
}

ساكن فراغ
hb_gpu_paint_ټېل_clip_لار_پای_ټکی (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
				 فراغ             *paint_معلومات,
				 فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;

  إذا (unlikely (!c->pending_clip_لار))
    أرجع;
  c->pending_clip_لار = خطأ;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_پاتې_شوی = hb_draw_get_budget_پاتې_شوی (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  إذا (unlikely (!blob || !c->ټېل_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ریښتیا;
    أرجع;
  }
  صحيح idx = (صحيح) (c->sub_blobs.اوږدتیا - 1);

  إذا (unlikely (c->clip_ژوروالی >= HB_GPU_PAINT_MAX_CLIP_ژوروالی))
  {
    c->unsupported = ریښتیا;
    c->clip_ژوروالی++;
    أرجع;
  }

  صحيح x0 = ext.x_bearing;
  صحيح x1 = hb_saturate_جمع_کړه (ext.x_bearing, ext.پلنوالی);
  صحيح y0 = ext.y_bearing;
  صحيح y1 = hb_saturate_جمع_کړه (ext.y_bearing, ext.لوړوالی);

  c->clip_ډېری[c->clip_ژوروالی] = {
    HB_CODEPOINT_INVALID, تش_اشاره,
    c->pending_clip_لار_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_ژوروالی++;
}

ساكن فراغ
hb_gpu_paint_ټېل_transform (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			     فراغ             *paint_معلومات,
			     عائم xx, عائم yx,
			     عائم xy, عائم yy,
			     عائم dx, عائم dy,
			     فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  إذا (unlikely (!c->transform_ډېری.ټېل_or_fail (c->cur_transform)))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }
  hb_transform_t<عائم> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

ساكن فراغ
hb_gpu_paint_راوباسه_transform (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			    فراغ             *paint_معلومات,
			    فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  إذا (unlikely (!c->transform_ډېری.اوږدتیا))
    أرجع;
  c->cur_transform = c->transform_ډېری.arrayZ[c->transform_ډېری.اوږدتیا - 1];
  c->transform_ډېری.shrink (c->transform_ډېری.اوږدتیا - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
ساكن فراغ
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }
  إذا (unlikely (!c->ops.resize (c->ops.اوږدتیا + 4)))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.اوږدتیا - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_ټېل_ډله (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			 فراغ             *paint_معلومات,
			 فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  c->ډله_ژوروالی++;
  إذا (unlikely (c->ډله_ژوروالی > HB_GPU_PAINT_MAX_ډله_ژوروالی))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_ټېل_ډله, 0);
}

ساكن فراغ
hb_gpu_paint_راوباسه_ډله (hb_paint_funcs_t    *funcs HB_ناکارول_شوی,
			فراغ                *paint_معلومات,
			hb_paint_مرکب_ډېرپېښ_t ډېرپېښ,
			فراغ                *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_معلومات;
  منطقي was_دننه_range = c->ډله_ژوروالی <= HB_GPU_PAINT_MAX_ډله_ژوروالی;
  إذا (likely (c->ډله_ژوروالی > 0)) c->ډله_ژوروالی--;
  إذا (was_دننه_range)
    emit_control_op (c, HB_GPU_PAINT_OP_راوباسه_ډله, (int16_t) ډېرپېښ);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
ساكن مضمن int16_t
color_منزل_q15 (بینښه byte_ارزښت)
{
  أرجع (int16_t) ((byte_ارزښت * 32767u + 127u) / 255u);
}

ساكن مضمن فراغ
color_منزل_q15_rgba (hb_color_t color, int16_t بهر[4])
{
  بهر[0] = color_منزل_q15 (hb_color_get_red   (color));
  بهر[1] = color_منزل_q15 (hb_color_get_green (color));
  بهر[2] = color_منزل_q15 (hb_color_get_blue  (color));
  بهر[3] = color_منزل_q15 (hb_color_get_alpha (color));
}

ساكن مضمن int16_t
clamp_i16 (عائم v)
{
  إذا (v <= -32768.f) أرجع -32768;
  إذا (v >=  32767.f) أرجع  32767;
  إذا (unlikely (std::isnan (v))) أرجع 0;
  أرجع (int16_t) v;
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
ساكن مضمن فراغ
linv_q10 (ثابت hb_transform_t<عائم> &t, int16_t بهر[4])
{
  عائم det = t.xx * t.yy - t.xy * t.yx;
  عائم inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  بهر[0] = clamp_i16 ( t.yy * inv * 1024.f);
  بهر[1] = clamp_i16 (-t.xy * inv * 1024.f);
  بهر[2] = clamp_i16 (-t.yx * inv * 1024.f);
  بهر[3] = clamp_i16 ( t.xx * inv * 1024.f);
}

/* Transforming pen: applies an affine transform to each outline
 * point before forwarding to a downstream hb_draw_funcs_t.  The
 * downstream's inline draw-state bookkeeping runs against a
 * separate state (down_st) so transformed-space updates don't
 * pollute the caller's pre-transform state. */
بنية hb_gpu_paint_pen_t
{
  hb_transform_t<عائم> transform;
  hb_draw_funcs_t  *dfuncs;
  فراغ             *معلومات;
  hb_draw_state_t   ښکته_st;
  int64_t          *budget_پاتې_شوی;
};

ساكن فراغ
hb_gpu_paint_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			  فراغ *معلومات, hb_draw_state_t *st HB_ناکارول_شوی,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  خپلکاری *c = (hb_gpu_paint_pen_t *) معلومات;
  c->transform.transform_ټکی (منزل_x, منزل_y);
  c->dfuncs->move_منزل (c->معلومات, c->ښکته_st, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_کرښه_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			  فراغ *معلومات, hb_draw_state_t *st HB_ناکارول_شوی,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  خپلکاری *c = (hb_gpu_paint_pen_t *) معلومات;
  c->transform.transform_ټکی (منزل_x, منزل_y);
  c->dfuncs->کرښه_منزل (c->معلومات, c->ښکته_st, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			       فراغ *معلومات, hb_draw_state_t *st HB_ناکارول_شوی,
			       عائم c_x, عائم c_y,
			       عائم منزل_x, عائم منزل_y,
			       فراغ *user_معلومات HB_ناکارول_شوی)
{
  خپلکاری *c = (hb_gpu_paint_pen_t *) معلومات;
  c->transform.transform_ټکی (c_x, c_y);
  c->transform.transform_ټکی (منزل_x, منزل_y);
  c->dfuncs->quadratic_منزل (c->معلومات, c->ښکته_st, c_x, c_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			   فراغ *معلومات, hb_draw_state_t *st HB_ناکارول_شوی,
			   عائم c1_x, عائم c1_y,
			   عائم c2_x, عائم c2_y,
			   عائم منزل_x, عائم منزل_y,
			   فراغ *user_معلومات HB_ناکارول_شوی)
{
  خپلکاری *c = (hb_gpu_paint_pen_t *) معلومات;
  c->transform.transform_ټکی (c1_x, c1_y);
  c->transform.transform_ټکی (c2_x, c2_y);
  c->transform.transform_ټکی (منزل_x, منزل_y);
  c->dfuncs->cubic_منزل (c->معلومات, c->ښکته_st, c1_x, c1_y, c2_x, c2_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_close_لار (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			     فراغ *معلومات, hb_draw_state_t *st HB_ناکارول_شوی,
			     فراغ *user_معلومات HB_ناکارول_شوی)
{
  خپلکاری *c = (hb_gpu_paint_pen_t *) معلومات;
  c->dfuncs->close_لار (c->معلومات, c->ښکته_st);
}

ساكن int64_t *
hb_gpu_paint_pen_get_budget_پاتې_شوی (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_gpu_paint_pen_t *) draw_معلومات)->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_gpu_paint_pen_funcs ();

ساكن بنية hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سېټ_move_منزل_func      (funcs, hb_gpu_paint_pen_move_منزل,      تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func      (funcs, hb_gpu_paint_pen_کرښه_منزل,      تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, hb_gpu_paint_pen_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func     (funcs, hb_gpu_paint_pen_cubic_منزل,     تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func   (funcs, hb_gpu_paint_pen_close_لار,   تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_gpu_paint_pen_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_gpu_paint_pen_funcs);
    أرجع funcs;
  }
} static_gpu_paint_pen_funcs;

ساكن مضمن فراغ
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
ساكن صحيح
emit_clip_sub_blob (hb_gpu_paint_t *c,
		    hb_gpu_paint_t::pending_clip_t &clip)
{
  /* Path clips were already encoded into sub_blobs at
   * push_clip_path_end time; glyph clips are cached here on first
   * use.  Either way: just accumulate extents and hand back the
   * recorded index. */
  إذا (clip.sub_blob_شاخص >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    أرجع clip.sub_blob_شاخص;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  إذا (unlikely (c->sub_blobs.اوږدتیا >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = ریښتیا;
    أرجع -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  إذا (unlikely (c->budget_پاتې_شوی < 0))
  {
    c->unsupported = ریښتیا;
    أرجع -1;
  }

  إذا (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    إذا (unlikely (!c->scratch_draw))
    {
      c->unsupported = ریښتیا;
      أرجع -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_سېټ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_پاتې_شوی);

  منطقي ok;
  إذا (clip.transform.is_بې_اغېزه_عنصر ())
  {
    /* Fast path: feed the glyph outline straight into the draw
     * encoder with no adapter. */
    ok = hb_gpu_draw_glyph_or_fail (c->scratch_draw, clip.font, clip.glyph);
  }
  وإلا
  {
    /* Transform each outline point by the transform that was
     * current at push_clip_glyph time -- NOT the innermost
     * cur_transform, which may have additional PaintTransform
     * wrappers from the paint child underneath. */
    صحيح x_scale, y_scale;
    hb_font_get_scale (clip.font, &x_scale, &y_scale);
    hb_gpu_draw_سېټ_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.معلومات      = c->scratch_draw;
    pen.ښکته_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_پاتې_شوی = &c->scratch_draw->budget_پاتې_شوی;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_لار (pen.معلومات, pen.ښکته_st);
  }
  c->budget_پاتې_شوی = hb_draw_get_budget_پاتې_شوی (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  إذا (!ok)
    أرجع -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  إذا (unlikely (!blob || !c->ټېل_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = ریښتیا;
    أرجع -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  صحيح x0 = ext.x_bearing;
  صحيح x1 = hb_saturate_جمع_کړه (ext.x_bearing, ext.پلنوالی);
  صحيح y0 = ext.y_bearing;
  صحيح y1 = hb_saturate_جمع_کړه (ext.y_bearing, ext.لوړوالی);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  صحيح idx = (صحيح) (c->sub_blobs.اوږدتیا - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_شاخص = idx;
  clip.ext_x0 = hb_min (x0, x1);
  clip.ext_y0 = hb_min (y0, y1);
  clip.ext_x1 = hb_max (x0, x1);
  clip.ext_y1 = hb_max (y0, y1);

  أرجع idx;
}

/* Emit a sub-blob for every clip currently on the stack and return
 * their indices in @out (set to -1 for unused slots).  Returns
 * false if any clip's outline is empty (caller should skip the
 * layer entirely) or hb_gpu_draw_encode failed. */
ساكن منطقي
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, صحيح بهر[HB_GPU_PAINT_MAX_CLIP_ژوروالی])
{
  لكل (بینښه i = 0; i < HB_GPU_PAINT_MAX_CLIP_ژوروالی; i++)
    بهر[i] = -1;
  لكل (بینښه i = 0; i < hb_min (c->clip_ژوروالی, (بینښه) HB_GPU_PAINT_MAX_CLIP_ژوروالی); i++)
  {
    بهر[i] = emit_clip_sub_blob (c, c->clip_ډېری[i]);
    إذا (بهر[i] < 0)
      أرجع خطأ;
  }
  أرجع ریښتیا;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
ساكن int16_t *
emit_gradient_op_سرلیک (hb_gpu_paint_t *c, بینښه subtype,
			 ثابت صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ژوروالی])
{
  إذا (unlikely (!c->ops.resize (c->ops.اوږدتیا + 12)))
  { c->unsupported = ریښتیا; أرجع تش_اشاره; }
  int16_t *o = &c->ops.arrayZ[c->ops.اوږدتیا - 12];
  int16_t flags = (int16_t) subtype;
  إذا (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  إذا (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_GRADIENT;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  أرجع o + 8;  /* texel 2: gradient meta -- caller fills */
}

ساكن فراغ
hb_gpu_paint_emit_solid (hb_gpu_paint_t *c,
			 hb_bool_t       is_foreground,
			 hb_color_t      color)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ژوروالی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ژوروالی];
  إذا (!emit_all_clip_sub_blobs (c, clip_idx))
    أرجع;

  /* Emit LAYER_SOLID op: 3 texels (12 int16s).
   *   texel 0: op_type, flags, clip1_hi, clip1_lo
   *   texel 1: clip2_hi, clip2_lo, 0, 0  (only meaningful if HAS_CLIP2)
   *   texel 2: r_q15, g_q15, b_q15, a_q15
   * Payloads hold sub_blob indices; hb_gpu_paint_encode() patches
   * them into texel offsets at serialize time.
   * flags bit 0: use shader foreground uniform instead of the
   * baked color (so runtime foreground-color changes still work).
   * flags bit 8: a second clip outline is present in texel 1. */
  إذا (unlikely (!c->ops.resize (c->ops.اوږدتیا + 12)))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.اوږدتیا - 12];
  int16_t flags = (int16_t) (is_foreground ? HB_GPU_PAINT_FLAG_IS_FOREGROUND : 0);
  إذا (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  إذا (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_SOLID;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  color_منزل_q15_rgba (color, o + 8);
  c->عدد_ops++;
}

/* Append a color line (@count stops) to @grad_data in the encoded
 * stop format used by gradient ops: 2 texels per stop.
 *   texel a: (offset_q15, flags, _, _) with flags bit 0 = is_foreground
 *   texel b: (r_q15, g_q15, b_q15, a_q15)
 * Returns false on allocation failure. */
ساكن منطقي
append_color_stops (hb_ویکټور_t<int16_t> &grad_معلومات,
		    ثابت hb_color_stop_t *stops,
		    بینښه شمېر)
{
  بینښه بنسټ = grad_معلومات.اوږدتیا;
  إذا (unlikely (!grad_معلومات.resize (بنسټ + شمېر * 8)))
    أرجع خطأ;
  int16_t *p = grad_معلومات.arrayZ + بنسټ;
  لكل (بینښه i = 0; i < شمېر; i++)
  {
    عائم off = stops[i].دننه_واټن;
    إذا (off < -1.f) off = -1.f;
    وإلا إذا (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_منزل_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  أرجع ریښتیا;
}

ساكن فراغ
hb_gpu_paint_emit_خطي (hb_gpu_paint_t  *c,
			  hb_color_کرښه_t *color_کرښه,
			  عائم x0, عائم y0,
			  عائم x1, عائم y1,
			  عائم x2, عائم y2)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ژوروالی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  إذا (unlikely (!c->fetch_color_stops (color_کرښه)))
    أرجع;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  بینښه got = c->color_stops_scratch.اوږدتیا;

  hb_paint_extend_t extend = hb_color_کرښه_get_extend (color_کرښه);

  /* Emit clip sub-blob(s) first. */
  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ژوروالی];
  إذا (!emit_all_clip_sub_blobs (c, clip_idx)) أرجع;

  /* Build gradient params sub-blob.
   *   texel 0: (p0_rendered_x, p0_rendered_y, d_canonical_x, d_canonical_y)
   *     i16 font units; d = p1 - p0 stored in untransformed space
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * The shader computes p_canonical = L^-1 * (renderCoord - p0_rendered)
   * and evaluates t = dot(p_canonical, d) / dot(d, d) in
   * untransformed space, so affine distortions of the axis are
   * handled exactly rather than approximated. */
  hb_ویکټور_t<int16_t> grad_معلومات;
  إذا (unlikely (!grad_معلومات.resize (8)))
  { c->unsupported = ریښتیا; أرجع; }

  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطي_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops, got, &mn, &mx);
  عائم dx = lx1 - lx0, dy = ly1 - ly0;
  عائم ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  عائم ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  عائم p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_ټکی (p0x_r, p0y_r);
  grad_معلومات[0] = clamp_i16 (p0x_r);
  grad_معلومات[1] = clamp_i16 (p0y_r);
  grad_معلومات[2] = clamp_i16 (ax1 - ax0);
  grad_معلومات[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_معلومات[4] = Linv[0];
  grad_معلومات[5] = Linv[1];
  grad_معلومات[6] = Linv[2];
  grad_معلومات[7] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_معلومات, stops, got)))
  { c->unsupported = ریښتیا; أرجع; }

  بینښه grad_بایټونه = grad_معلومات.اوږدتیا * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_معلومات.arrayZ,
					 grad_بایټونه, HB_حافظه_ډېرپېښ_DUPLICATE,
					 تش_اشاره, تش_اشاره);
  إذا (unlikely (!grad_blob || !c->ټېل_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ریښتیا;
    أرجع;
  }
  بینښه grad_idx = c->sub_blobs.اوږدتیا - 1;

  int16_t *meta = emit_gradient_op_سرلیک (c, 0 /*خطي*/, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_کرښه_t *color_کرښه,
			  عائم x0, عائم y0, عائم r0,
			  عائم x1, عائم y1, عائم r1)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ژوروالی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }

  إذا (unlikely (!c->fetch_color_stops (color_کرښه)))
    أرجع;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  بینښه got = c->color_stops_scratch.اوږدتیا;

  hb_paint_extend_t extend = hb_color_کرښه_get_extend (color_کرښه);

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ژوروالی];
  إذا (!emit_all_clip_sub_blobs (c, clip_idx)) أرجع;

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
  hb_ویکټور_t<int16_t> grad_معلومات;
  إذا (unlikely (!grad_معلومات.resize (12)))
  { c->unsupported = ریښتیا; أرجع; }
  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops, got, &mn, &mx);
  عائم dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  عائم cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  عائم cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  عائم cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_ټکی (cx0_r, cy0_r);
  grad_معلومات[0] = clamp_i16 (cx0_r);
  grad_معلومات[1] = clamp_i16 (cy0_r);
  grad_معلومات[2] = clamp_i16 (cx1 - cx0);
  grad_معلومات[3] = clamp_i16 (cy1 - cy0);
  grad_معلومات[4] = clamp_i16 (cr0);
  grad_معلومات[5] = clamp_i16 (cr1);
  grad_معلومات[6] = 0;
  grad_معلومات[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_معلومات[8]  = Linv[0];
  grad_معلومات[9]  = Linv[1];
  grad_معلومات[10] = Linv[2];
  grad_معلومات[11] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_معلومات, stops, got)))
  { c->unsupported = ریښتیا; أرجع; }

  بینښه grad_بایټونه = grad_معلومات.اوږدتیا * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_معلومات.arrayZ,
					 grad_بایټونه, HB_حافظه_ډېرپېښ_DUPLICATE,
					 تش_اشاره, تش_اشاره);
  إذا (unlikely (!grad_blob || !c->ټېل_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ریښتیا;
    أرجع;
  }
  بینښه grad_idx = c->sub_blobs.اوږدتیا - 1;

  int16_t *meta = emit_gradient_op_سرلیک (c, 1 /* radial */, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
		    فراغ             *paint_معلومات,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_معلومات, is_foreground, color);
}

ساكن فراغ
hb_gpu_paint_خطي_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ             *paint_معلومات,
			      hb_color_کرښه_t  *color_کرښه,
			      عائم x0, عائم y0,
			      عائم x1, عائم y1,
			      عائم x2, عائم y2,
			      فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_emit_خطي ((hb_gpu_paint_t *) paint_معلومات, color_کرښه,
			    x0, y0, x1, y1, x2, y2);
}

ساكن فراغ
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			      فراغ             *paint_معلومات,
			      hb_color_کرښه_t  *color_کرښه,
			      عائم x0, عائم y0, عائم r0,
			      عائم x1, عائم y1, عائم r1,
			      فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_معلومات, color_کرښه,
			    x0, y0, r0, x1, y1, r1);
}

ساكن فراغ
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_کرښه_t *color_کرښه,
			 عائم cx, عائم cy,
			 عائم پیل_angle, عائم پای_ټکی_angle)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ژوروالی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = ریښتیا;
    أرجع;
  }

  بینښه شمېر = hb_color_کرښه_get_color_stops (color_کرښه, 0, تش_اشاره, تش_اشاره);
  إذا (unlikely (!شمېر))
    أرجع;
  hb_color_stop_t ډېری_stops[16];
  hb_color_stop_t *stops = ډېری_stops;
  hb_color_stop_t *ډېرۍ_stops = تش_اشاره;
  إذا (شمېر > 16)
  {
    ډېرۍ_stops = (hb_color_stop_t *) hb_malloc2 (شمېر, حجم (hb_color_stop_t));
    إذا (unlikely (!ډېرۍ_stops)) { c->unsupported = ریښتیا; أرجع; }
    stops = ډېرۍ_stops;
  }
  بینښه got = شمېر;
  hb_color_کرښه_get_color_stops (color_کرښه, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_کرښه_get_extend (color_کرښه);

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ژوروالی];
  إذا (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (ډېرۍ_stops); أرجع; }

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
  hb_ویکټور_t<int16_t> grad_معلومات;
  إذا (unlikely (!grad_معلومات.resize (8)))
  { c->unsupported = ریښتیا; hb_free (ډېرۍ_stops); أرجع; }

  عائم mn, mx;
  hb_paint_normalize_color_کرښه (stops, got, &mn, &mx);
  عائم a_پراختیا = پای_ټکی_angle - پیل_angle;
  عائم a_lo = پیل_angle + mn * a_پراختیا;
  عائم a_hi = پیل_angle + mx * a_پراختیا;
  خپلکاری rad_منزل_q14_pi = [] (عائم rad) -> int16_t {
    عائم v = rad * (عائم) (1.0 / M_PI);  /* fraction of pi */
    إذا (v <= -2.f) أرجع -32768;
    إذا (v >=  2.f) أرجع  32767;
    أرجع (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  عائم tcx = cx, tcy = cy;
  c->cur_transform.transform_ټکی (tcx, tcy);
  grad_معلومات[0] = clamp_i16 (tcx);
  grad_معلومات[1] = clamp_i16 (tcy);
  grad_معلومات[2] = rad_منزل_q14_pi (a_lo);
  grad_معلومات[3] = rad_منزل_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_معلومات[4] = Linv[0];
  grad_معلومات[5] = Linv[1];
  grad_معلومات[6] = Linv[2];
  grad_معلومات[7] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_معلومات, stops, got)))
  { c->unsupported = ریښتیا; hb_free (ډېرۍ_stops); أرجع; }
  hb_free (ډېرۍ_stops);

  بینښه grad_بایټونه = grad_معلومات.اوږدتیا * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_معلومات.arrayZ,
					 grad_بایټونه, HB_حافظه_ډېرپېښ_DUPLICATE,
					 تش_اشاره, تش_اشاره);
  إذا (unlikely (!grad_blob || !c->ټېل_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = ریښتیا;
    أرجع;
  }
  بینښه grad_idx = c->sub_blobs.اوږدتیا - 1;

  int16_t *meta = emit_gradient_op_سرلیک (c, 2 /* sweep */, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_ناکارول_شوی,
			     فراغ             *paint_معلومات,
			     hb_color_کرښه_t  *color_کرښه,
			     عائم cx, عائم cy,
			     عائم پیل_angle,
			     عائم پای_ټکی_angle,
			     فراغ             *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_معلومات, color_کرښه,
			   cx, cy, پیل_angle, پای_ټکی_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
ساكن hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_ناکارول_شوی,
		    فراغ               *paint_معلومات,
		    hb_blob_t          *image   HB_ناکارول_شوی,
		    بینښه صحيح        پلنوالی   HB_ناکارول_شوی,
		    بینښه صحيح        لوړوالی  HB_ناکارول_شوی,
		    hb_نښان_t            format  HB_ناکارول_شوی,
		    عائم               slant   HB_ناکارول_شوی,
		    hb_glyph_extents_t *extents HB_ناکارول_شوی,
		    فراغ               *user_معلومات HB_ناکارول_شوی)
{
  ((hb_gpu_paint_t *) paint_معلومات)->unsupported = ریښتیا;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_gpu_paint_سېټ_budget (hb_paint_funcs_t *, فراغ *paint_معلومات,
			 int64_t budget, فراغ *)
{
  خپلکاری *paint = (hb_gpu_paint_t *) paint_معلومات;
  paint->budget = budget;
  paint->recharge_budget ();
  أرجع ریښتیا;
}

ساكن int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع ((hb_gpu_paint_t *) paint_معلومات)->budget;
}

ساكن int64_t *
hb_gpu_paint_get_budget_پاتې_شوی (hb_paint_funcs_t *, فراغ *paint_معلومات, فراغ *)
{
  أرجع &((hb_gpu_paint_t *) paint_معلومات)->budget_پاتې_شوی;
}

ساكن مضمن فراغ free_static_gpu_paint_funcs ();

ساكن بنية hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سېټ_ټېل_transform_func        (funcs, hb_gpu_paint_ټېل_transform,        تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_transform_func         (funcs, hb_gpu_paint_راوباسه_transform,         تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_glyph_func       (funcs, hb_gpu_paint_ټېل_clip_glyph,       تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_لار_پیل_func  (funcs, hb_gpu_paint_ټېل_clip_لار_پیل,  تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_clip_لار_پای_ټکی_func    (funcs, hb_gpu_paint_ټېل_clip_لار_پای_ټکی,    تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_clip_func              (funcs, hb_gpu_paint_راوباسه_clip,              تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_ټېل_ډله_func            (funcs, hb_gpu_paint_ټېل_ډله,            تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_راوباسه_ډله_func             (funcs, hb_gpu_paint_راوباسه_ډله,             تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_color_func                 (funcs, hb_gpu_paint_color,                 تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_خطي_gradient_func       (funcs, hb_gpu_paint_خطي_gradient,       تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  تش_اشاره, تش_اشاره);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_سېټ_image_func                 (funcs, hb_gpu_paint_image,                 تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_سېټ_budget_func (funcs, hb_gpu_paint_سېټ_budget, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_func (funcs, hb_gpu_paint_get_budget, تش_اشاره, تش_اشاره);
    hb_paint_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_gpu_paint_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_paint_funcs);

    أرجع funcs;
  }
} static_gpu_paint_funcs;

ساكن مضمن فراغ
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
hb_gpu_paint_create_or_fail (فراغ)
{
  أرجع hb_object_create<hb_gpu_paint_t> ();
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
  أرجع hb_object_reference (paint);
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
فراغ
hb_gpu_paint_destroy (hb_gpu_paint_t *paint)
{
  إذا (!hb_object_should_destroy (paint))
    أرجع;

  hb_map_destroy (paint->custom_palette);
  لكل (hb_blob_t *b : paint->sub_blobs)
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
hb_gpu_paint_سېټ_user_معلومات (hb_gpu_paint_t     *paint,
			    hb_user_معلومات_کلی_t *key,
			    فراغ               *معلومات,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  أرجع hb_object_سېټ_user_معلومات (paint, key, معلومات, destroy, replace);
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
فراغ *
hb_gpu_paint_get_user_معلومات (ثابت hb_gpu_paint_t *paint,
			    hb_user_معلومات_کلی_t   *key)
{
  أرجع hb_object_get_user_معلومات (paint, key);
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
hb_gpu_paint_get_funcs (ثابت hb_gpu_paint_t *paint HB_ناکارول_شوی)
{
  أرجع static_gpu_paint_funcs.get_unconst ();
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
فراغ
hb_gpu_paint_سېټ_palette (hb_gpu_paint_t *paint,
			  بینښه        palette)
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
بینښه
hb_gpu_paint_get_palette (ثابت hb_gpu_paint_t *paint)
{
  أرجع paint->palette;
}

/**
 * hb_gpu_paint_clear_custom_palette_colors:
 * @paint: a GPU color-glyph paint encoder
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * Since: 14.2.0
 **/
فراغ
hb_gpu_paint_clear_custom_palette_colors (hb_gpu_paint_t *paint)
{
  إذا (paint->custom_palette)
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
hb_gpu_paint_سېټ_custom_palette_color (hb_gpu_paint_t *paint,
				       بینښه صحيح    color_شاخص,
				       hb_color_t      color)
{
  إذا (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    إذا (unlikely (!paint->custom_palette))
      أرجع خطأ;
  }
  hb_map_سېټ (paint->custom_palette, color_شاخص, color);
  أرجع hb_map_ویش_successful (paint->custom_palette);
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
فراغ
hb_gpu_paint_سېټ_scale (hb_gpu_paint_t *paint,
			صحيح             x_scale,
			صحيح             y_scale)
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
فراغ
hb_gpu_paint_get_scale (ثابت hb_gpu_paint_t *paint,
			صحيح                  *x_scale,
			صحيح                  *y_scale)
{
  إذا (x_scale) *x_scale = paint->x_scale;
  إذا (y_scale) *y_scale = paint->y_scale;
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
  صحيح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_سېټ_scale (paint, x_scale, y_scale);
  أرجع hb_font_paint_glyph_or_fail (font, glyph,
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
فراغ
hb_gpu_paint_glyph (hb_gpu_paint_t *paint,
		    hb_font_t      *font,
		    hb_codepoint_t  glyph)
{
  صحيح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_سېټ_scale (paint, x_scale, y_scale);
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

  إذا (unlikely (paint->unsupported))
    أرجع تش_اشاره;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  إذا (paint->عدد_ops == 0)
    أرجع hb_blob_get_تشه ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  ثابت بینښه سرلیک_texels = 3;
  ثابت بینښه texel_بایټونه   = 8;

  بینښه ops_texels = paint->ops.اوږدتیا / 4;
  بینښه sub_بایټونه = 0;
  لكل (hb_blob_t *b : paint->sub_blobs)
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (sub_بایټونه,
					     hb_blob_get_اوږدتیا (b),
					     &sub_بایټونه)))
      أرجع تش_اشاره;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  إذا (unlikely (sub_بایټونه % texel_بایټونه))
    أرجع تش_اشاره;

  بینښه ټول_بایټونه = سرلیک_texels * texel_بایټونه
			+ paint->ops.اوږدتیا * 2;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (ټول_بایټونه, sub_بایټونه,
					   &ټول_بایټونه)))
    أرجع تش_اشاره;

  بینښه buf_ظرفیت = 0;
  حرف *replaced_recycled_buf = تش_اشاره;
  حرف *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, ټول_بایټونه,
					&buf_ظرفیت, &replaced_recycled_buf);
  إذا (unlikely (!buf_raw))
    أرجع تش_اشاره;
  int16_t *buf = (int16_t *) (فراغ *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  بینښه sub_offsets_شمېر = paint->sub_blobs.اوږدتیا;
  hb_ویکټور_t<بینښه> sub_offsets;
  إذا (unlikely (!sub_offsets.resize (sub_offsets_شمېر)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    أرجع تش_اشاره;
  }
  بینښه cursor = سرلیک_texels + ops_texels;
  لكل (بینښه i = 0; i < sub_offsets_شمېر; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_اوږدتیا (paint->sub_blobs[i]) / texel_بایټونه;
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
  buf[8] = (int16_t) سرلیک_texels;
  buf[9]  = 0;
  buf[10] = 0;
  buf[11] = 0;

  /* Op stream: copy raw, then patch payload slots to texel
   * offsets where op_type indicates a sub_blob reference. */
  int16_t *ops_بهر = buf + سرلیک_texels * 4;
  hb_memcpy (ops_بهر, paint->ops.arrayZ, paint->ops.اوږدتیا * 2);

  بینښه i = 0;
  طالما (i < paint->ops.اوږدتیا)
  {
    int16_t op_type = ops_بهر[i];
    بدل (op_type)
    {
      حالة HB_GPU_PAINT_OP_LAYER_SOLID:
      حالة HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_بهر[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	بینښه slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	لكل (بینښه k = 0; k < 4; k++)
	{
	  إذا (!slots[k]) استمر;
	  بینښه p = i + slots[k];
	  بینښه idx = ((uint16_t) ops_بهر[p] << 16)
		       |  (uint16_t) ops_بهر[p + 1];
	  إذا (idx < sub_offsets_شمېر)
	  {
	    بینښه off = sub_offsets[idx];
	    ops_بهر[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_بهر[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	اكسر;
      }
      حالة HB_GPU_PAINT_OP_ټېل_ډله:
      حالة HB_GPU_PAINT_OP_راوباسه_ډله:
	i += 4;  /* 1 texel */
	اكسر;
      افتراضي:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	أرجع تش_اشاره;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  حرف *dst = (حرف *) buf + سرلیک_texels * texel_بایټونه
			+ paint->ops.اوږدتیا * 2;
  لكل (hb_blob_t *b : paint->sub_blobs)
  {
    بینښه len = hb_blob_get_اوږدتیا (b);
    hb_memcpy (dst, hb_blob_get_معلومات (b, تش_اشاره), len);
    dst += len;
  }

  إذا (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->پلنوالی     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->لوړوالی    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = تش_اشاره;
  أرجع hb_blob_t::recycle_finalize (buf_raw, buf_ظرفیت, ټول_بایټونه,
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
فراغ
hb_gpu_paint_clear (hb_gpu_paint_t *paint)
{
  /* Use reset() not resize(0): if an earlier push_or_fail() set
   * the vector's error state (common under failing-malloc),
   * resize(0) short-circuits without clearing length.  A
   * subsequent clear would double-destroy the blob pointers left
   * in the array, or keep stale ops across encodes. */
  paint->ops.reset ();
  paint->عدد_ops = 0;
  لكل (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  paint->sub_blobs.reset ();
  paint->sub_بایټونه = 0;
  paint->clip_ژوروالی = 0;
  paint->pending_clip_لار = خطأ;
  paint->unsupported = خطأ;
  paint->recharge_budget ();
  paint->cur_transform = {1, 0, 0, 1, 0, 0};
  paint->transform_ډېری.reset ();
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
فراغ
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = تش_اشاره;
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
فراغ
hb_gpu_paint_recycle_blob (hb_gpu_paint_t *paint,
			   hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&paint->recycled_blob, blob);
}


#ضمن "hb-gpu-paint-fragment-glsl.hh"
#ضمن "hb-gpu-paint-fragment-msl.hh"
#ضمن "hb-gpu-paint-fragment-wgsl.hh"
#ضمن "hb-gpu-paint-fragment-hlsl.hh"

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
hb_gpu_paint_shader_سرچینه (hb_gpu_shader_stage_t stage,
			    hb_gpu_shader_lang_t  lang)
{
  بدل (stage) {
  حالة HB_GPU_SHADER_STAGE_FRAGMENT:
    بدل (lang) {
    حالة HB_GPU_SHADER_LANG_GLSL: أرجع hb_gpu_paint_fragment_glsl;
    حالة HB_GPU_SHADER_LANG_MSL:  أرجع hb_gpu_paint_fragment_msl;
    حالة HB_GPU_SHADER_LANG_WGSL: أرجع hb_gpu_paint_fragment_wgsl;
    حالة HB_GPU_SHADER_LANG_HLSL: أرجع hb_gpu_paint_fragment_hlsl;
    حالة HB_GPU_SHADER_LANG_INVALID:
    افتراضي: أرجع تش_اشاره;
    }
  حالة HB_GPU_SHADER_STAGE_راس:
    بدل (lang) {
    حالة HB_GPU_SHADER_LANG_GLSL:
    حالة HB_GPU_SHADER_LANG_MSL:
    حالة HB_GPU_SHADER_LANG_WGSL:
    حالة HB_GPU_SHADER_LANG_HLSL: أرجع "";
    حالة HB_GPU_SHADER_LANG_INVALID:
    افتراضي: أرجع تش_اشاره;
    }
  افتراضي:
    أرجع تش_اشاره;
  }
}
