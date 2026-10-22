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
  HB_GPU_PAINT_OP_ڈالو_گروہ     = 2,
  HB_GPU_PAINT_OP_نکالو_گروہ      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#تعریف_کرو HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#تعریف_کرو HB_GPU_PAINT_MAX_گروہ_ڈونگھائی 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#تعریف_کرو HB_GPU_PAINT_MAX_CLIP_ڈونگھائی  3      /* shader can intersect up to 3 clip outlines per layer */
#تعریف_کرو HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#تعریف_کرو HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#تعریف_کرو HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#تعریف_کرو HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

ساكن hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				   فراغ             *paint_انکڑے,
				   بنانشانی          color_اشاریہ,
				   hb_color_t       *color,
				   فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  إذا (likely (c->custom_palette && hb_map_has (c->custom_palette, color_اشاریہ)))
  {
    *color = hb_map_get (c->custom_palette, color_اشاریہ);
    أرجع سچ;
  }
  أرجع خطأ;
}

ساكن فراغ
hb_gpu_paint_ڈالو_clip_glyph (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ             *paint_انکڑے,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;

  إذا (unlikely (c->clip_ڈونگھائی >= HB_GPU_PAINT_MAX_CLIP_ڈونگھائی))
  {
    c->unsupported = سچ;
    c->clip_ڈونگھائی++;
    أرجع;
  }
  c->clip_ڈھیر[c->clip_ڈونگھائی] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_ڈونگھائی++;
}

ساكن فراغ
hb_gpu_paint_نکالو_clip (hb_paint_funcs_t *funcs HB_بن_ورتیا,
		       فراغ             *paint_انکڑے,
		       فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  إذا (likely (c->clip_ڈونگھائی > 0)) c->clip_ڈونگھائی--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
ساكن hb_draw_funcs_t *
hb_gpu_paint_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				   فراغ             *paint_انکڑے,
				   فراغ            **draw_انکڑے,
				   فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;

  إذا (unlikely (c->clip_ڈونگھائی >= HB_GPU_PAINT_MAX_CLIP_ڈونگھائی))
  {
    c->unsupported = سچ;
    *draw_انکڑے = خالی_سوچک;
    أرجع خالی_سوچک;
  }

  إذا (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    إذا (unlikely (!c->scratch_draw))
    {
      c->unsupported = سچ;
      *draw_انکڑے = خالی_سوچک;
      أرجع خالی_سوچک;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_سیٹ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_بچیا);
  hb_gpu_draw_سیٹ_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_رستہ_transform = c->cur_transform;
  c->pending_clip_رستہ = سچ;

  *draw_انکڑے = c->scratch_draw;
  أرجع hb_gpu_draw_get_funcs (c->scratch_draw);
}

ساكن فراغ
hb_gpu_paint_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *funcs HB_بن_ورتیا,
				 فراغ             *paint_انکڑے,
				 فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;

  إذا (unlikely (!c->pending_clip_رستہ))
    أرجع;
  c->pending_clip_رستہ = خطأ;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_بچیا = hb_draw_get_budget_بچیا (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  إذا (unlikely (!blob || !c->ڈالو_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = سچ;
    أرجع;
  }
  صحيح idx = (صحيح) (c->sub_blobs.لمائی - 1);

  إذا (unlikely (c->clip_ڈونگھائی >= HB_GPU_PAINT_MAX_CLIP_ڈونگھائی))
  {
    c->unsupported = سچ;
    c->clip_ڈونگھائی++;
    أرجع;
  }

  صحيح x0 = ext.x_bearing;
  صحيح x1 = hb_saturate_جوڑو (ext.x_bearing, ext.چوڑائی);
  صحيح y0 = ext.y_bearing;
  صحيح y1 = hb_saturate_جوڑو (ext.y_bearing, ext.اچائی);

  c->clip_ڈھیر[c->clip_ڈونگھائی] = {
    HB_CODEPOINT_INVALID, خالی_سوچک,
    c->pending_clip_رستہ_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_ڈونگھائی++;
}

ساكن فراغ
hb_gpu_paint_ڈالو_transform (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			     فراغ             *paint_انکڑے,
			     عائم xx, عائم yx,
			     عائم xy, عائم yy,
			     عائم dx, عائم dy,
			     فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  إذا (unlikely (!c->transform_ڈھیر.ڈالو_or_fail (c->cur_transform)))
  {
    c->unsupported = سچ;
    أرجع;
  }
  hb_transform_t<عائم> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

ساكن فراغ
hb_gpu_paint_نکالو_transform (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			    فراغ             *paint_انکڑے,
			    فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  إذا (unlikely (!c->transform_ڈھیر.لمائی))
    أرجع;
  c->cur_transform = c->transform_ڈھیر.arrayZ[c->transform_ڈھیر.لمائی - 1];
  c->transform_ڈھیر.shrink (c->transform_ڈھیر.لمائی - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
ساكن فراغ
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    أرجع;
  }
  إذا (unlikely (!c->ops.resize (c->ops.لمائی + 4)))
  {
    c->unsupported = سچ;
    أرجع;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.لمائی - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_ڈالو_گروہ (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			 فراغ             *paint_انکڑے,
			 فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  c->گروہ_ڈونگھائی++;
  إذا (unlikely (c->گروہ_ڈونگھائی > HB_GPU_PAINT_MAX_گروہ_ڈونگھائی))
  {
    c->unsupported = سچ;
    أرجع;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_ڈالو_گروہ, 0);
}

ساكن فراغ
hb_gpu_paint_نکالو_گروہ (hb_paint_funcs_t    *funcs HB_بن_ورتیا,
			فراغ                *paint_انکڑے,
			hb_paint_مرکب_منوال_t منوال,
			فراغ                *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_انکڑے;
  منطقي was_وچ_range = c->گروہ_ڈونگھائی <= HB_GPU_PAINT_MAX_گروہ_ڈونگھائی;
  إذا (likely (c->گروہ_ڈونگھائی > 0)) c->گروہ_ڈونگھائی--;
  إذا (was_وچ_range)
    emit_control_op (c, HB_GPU_PAINT_OP_نکالو_گروہ, (int16_t) منوال);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
ساكن مضمن int16_t
color_منزل_q15 (بنانشانی byte_قدر)
{
  أرجع (int16_t) ((byte_قدر * 32767u + 127u) / 255u);
}

ساكن مضمن فراغ
color_منزل_q15_rgba (hb_color_t color, int16_t باہرلا[4])
{
  باہرلا[0] = color_منزل_q15 (hb_color_get_red   (color));
  باہرلا[1] = color_منزل_q15 (hb_color_get_green (color));
  باہرلا[2] = color_منزل_q15 (hb_color_get_blue  (color));
  باہرلا[3] = color_منزل_q15 (hb_color_get_alpha (color));
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
linv_q10 (ثابت hb_transform_t<عائم> &t, int16_t باہرلا[4])
{
  عائم det = t.xx * t.yy - t.xy * t.yx;
  عائم inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  باہرلا[0] = clamp_i16 ( t.yy * inv * 1024.f);
  باہرلا[1] = clamp_i16 (-t.xy * inv * 1024.f);
  باہرلا[2] = clamp_i16 (-t.yx * inv * 1024.f);
  باہرلا[3] = clamp_i16 ( t.xx * inv * 1024.f);
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
  فراغ             *انکڑے;
  hb_draw_state_t   تھلے_st;
  int64_t          *budget_بچیا;
};

ساكن فراغ
hb_gpu_paint_pen_move_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			  فراغ *انکڑے, hb_draw_state_t *st HB_بن_ورتیا,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  خودکار *c = (hb_gpu_paint_pen_t *) انکڑے;
  c->transform.transform_نقطہ (منزل_x, منزل_y);
  c->dfuncs->move_منزل (c->انکڑے, c->تھلے_st, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_سطر_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			  فراغ *انکڑے, hb_draw_state_t *st HB_بن_ورتیا,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_انکڑے HB_بن_ورتیا)
{
  خودکار *c = (hb_gpu_paint_pen_t *) انکڑے;
  c->transform.transform_نقطہ (منزل_x, منزل_y);
  c->dfuncs->سطر_منزل (c->انکڑے, c->تھلے_st, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			       فراغ *انکڑے, hb_draw_state_t *st HB_بن_ورتیا,
			       عائم c_x, عائم c_y,
			       عائم منزل_x, عائم منزل_y,
			       فراغ *user_انکڑے HB_بن_ورتیا)
{
  خودکار *c = (hb_gpu_paint_pen_t *) انکڑے;
  c->transform.transform_نقطہ (c_x, c_y);
  c->transform.transform_نقطہ (منزل_x, منزل_y);
  c->dfuncs->quadratic_منزل (c->انکڑے, c->تھلے_st, c_x, c_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_cubic_منزل (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			   فراغ *انکڑے, hb_draw_state_t *st HB_بن_ورتیا,
			   عائم c1_x, عائم c1_y,
			   عائم c2_x, عائم c2_y,
			   عائم منزل_x, عائم منزل_y,
			   فراغ *user_انکڑے HB_بن_ورتیا)
{
  خودکار *c = (hb_gpu_paint_pen_t *) انکڑے;
  c->transform.transform_نقطہ (c1_x, c1_y);
  c->transform.transform_نقطہ (c2_x, c2_y);
  c->transform.transform_نقطہ (منزل_x, منزل_y);
  c->dfuncs->cubic_منزل (c->انکڑے, c->تھلے_st, c1_x, c1_y, c2_x, c2_y, منزل_x, منزل_y);
}

ساكن فراغ
hb_gpu_paint_pen_close_رستہ (hb_draw_funcs_t *dfuncs HB_بن_ورتیا,
			     فراغ *انکڑے, hb_draw_state_t *st HB_بن_ورتیا,
			     فراغ *user_انکڑے HB_بن_ورتیا)
{
  خودکار *c = (hb_gpu_paint_pen_t *) انکڑے;
  c->dfuncs->close_رستہ (c->انکڑے, c->تھلے_st);
}

ساكن int64_t *
hb_gpu_paint_pen_get_budget_بچیا (hb_draw_funcs_t *, فراغ *draw_انکڑے, فراغ *)
{
  أرجع ((hb_gpu_paint_pen_t *) draw_انکڑے)->budget_بچیا;
}

ساكن مضمن فراغ free_static_gpu_paint_pen_funcs ();

ساكن بنية hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_سیٹ_move_منزل_func      (funcs, hb_gpu_paint_pen_move_منزل,      خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_سطر_منزل_func      (funcs, hb_gpu_paint_pen_سطر_منزل,      خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_quadratic_منزل_func (funcs, hb_gpu_paint_pen_quadratic_منزل, خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_cubic_منزل_func     (funcs, hb_gpu_paint_pen_cubic_منزل,     خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_close_رستہ_func   (funcs, hb_gpu_paint_pen_close_رستہ,   خالی_سوچک, خالی_سوچک);
    hb_draw_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_gpu_paint_pen_get_budget_بچیا, خالی_سوچک, خالی_سوچک);
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
  إذا (clip.sub_blob_اشاریہ >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    أرجع clip.sub_blob_اشاریہ;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  إذا (unlikely (c->sub_blobs.لمائی >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = سچ;
    أرجع -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  إذا (unlikely (c->budget_بچیا < 0))
  {
    c->unsupported = سچ;
    أرجع -1;
  }

  إذا (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    إذا (unlikely (!c->scratch_draw))
    {
      c->unsupported = سچ;
      أرجع -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_سیٹ_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_بچیا);

  منطقي ok;
  إذا (clip.transform.is_پچھان_تت ())
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
    hb_gpu_draw_سیٹ_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.انکڑے      = c->scratch_draw;
    pen.تھلے_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_بچیا = &c->scratch_draw->budget_بچیا;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_رستہ (pen.انکڑے, pen.تھلے_st);
  }
  c->budget_بچیا = hb_draw_get_budget_بچیا (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  إذا (!ok)
    أرجع -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  إذا (unlikely (!blob || !c->ڈالو_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = سچ;
    أرجع -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  صحيح x0 = ext.x_bearing;
  صحيح x1 = hb_saturate_جوڑو (ext.x_bearing, ext.چوڑائی);
  صحيح y0 = ext.y_bearing;
  صحيح y1 = hb_saturate_جوڑو (ext.y_bearing, ext.اچائی);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  صحيح idx = (صحيح) (c->sub_blobs.لمائی - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_اشاریہ = idx;
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
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, صحيح باہرلا[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی])
{
  لكل (بنانشانی i = 0; i < HB_GPU_PAINT_MAX_CLIP_ڈونگھائی; i++)
    باہرلا[i] = -1;
  لكل (بنانشانی i = 0; i < hb_min (c->clip_ڈونگھائی, (بنانشانی) HB_GPU_PAINT_MAX_CLIP_ڈونگھائی); i++)
  {
    باہرلا[i] = emit_clip_sub_blob (c, c->clip_ڈھیر[i]);
    إذا (باہرلا[i] < 0)
      أرجع خطأ;
  }
  أرجع سچ;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
ساكن int16_t *
emit_gradient_op_سرخی (hb_gpu_paint_t *c, بنانشانی subtype,
			 ثابت صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی])
{
  إذا (unlikely (!c->ops.resize (c->ops.لمائی + 12)))
  { c->unsupported = سچ; أرجع خالی_سوچک; }
  int16_t *o = &c->ops.arrayZ[c->ops.لمائی - 12];
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
  إذا (unlikely (c->clip_ڈونگھائی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    أرجع;
  }

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی];
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
  إذا (unlikely (!c->ops.resize (c->ops.لمائی + 12)))
  {
    c->unsupported = سچ;
    أرجع;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.لمائی - 12];
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
append_color_stops (hb_سمتیہ_t<int16_t> &grad_انکڑے,
		    ثابت hb_color_stop_t *stops,
		    بنانشانی گنتی)
{
  بنانشانی ادھار = grad_انکڑے.لمائی;
  إذا (unlikely (!grad_انکڑے.resize (ادھار + گنتی * 8)))
    أرجع خطأ;
  int16_t *p = grad_انکڑے.arrayZ + ادھار;
  لكل (بنانشانی i = 0; i < گنتی; i++)
  {
    عائم off = stops[i].وتھ;
    إذا (off < -1.f) off = -1.f;
    وإلا إذا (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_منزل_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  أرجع سچ;
}

ساكن فراغ
hb_gpu_paint_emit_خطی (hb_gpu_paint_t  *c,
			  hb_color_سطر_t *color_سطر,
			  عائم x0, عائم y0,
			  عائم x1, عائم y1,
			  عائم x2, عائم y2)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ڈونگھائی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    أرجع;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  إذا (unlikely (!c->fetch_color_stops (color_سطر)))
    أرجع;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  بنانشانی got = c->color_stops_scratch.لمائی;

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  /* Emit clip sub-blob(s) first. */
  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی];
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
  hb_سمتیہ_t<int16_t> grad_انکڑے;
  إذا (unlikely (!grad_انکڑے.resize (8)))
  { c->unsupported = سچ; أرجع; }

  عائم lx0, ly0, lx1, ly1;
  hb_paint_reduce_خطی_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  عائم dx = lx1 - lx0, dy = ly1 - ly0;
  عائم ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  عائم ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  عائم p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_نقطہ (p0x_r, p0y_r);
  grad_انکڑے[0] = clamp_i16 (p0x_r);
  grad_انکڑے[1] = clamp_i16 (p0y_r);
  grad_انکڑے[2] = clamp_i16 (ax1 - ax0);
  grad_انکڑے[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_انکڑے[4] = Linv[0];
  grad_انکڑے[5] = Linv[1];
  grad_انکڑے[6] = Linv[2];
  grad_انکڑے[7] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_انکڑے, stops, got)))
  { c->unsupported = سچ; أرجع; }

  بنانشانی grad_بائٹاں = grad_انکڑے.لمائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_انکڑے.arrayZ,
					 grad_بائٹاں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_سوچک, خالی_سوچک);
  إذا (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    أرجع;
  }
  بنانشانی grad_idx = c->sub_blobs.لمائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 0 /*خطی*/, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_سطر_t *color_سطر,
			  عائم x0, عائم y0, عائم r0,
			  عائم x1, عائم y1, عائم r1)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ڈونگھائی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    أرجع;
  }

  إذا (unlikely (!c->fetch_color_stops (color_سطر)))
    أرجع;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  بنانشانی got = c->color_stops_scratch.لمائی;

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی];
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
  hb_سمتیہ_t<int16_t> grad_انکڑے;
  إذا (unlikely (!grad_انکڑے.resize (12)))
  { c->unsupported = سچ; أرجع; }
  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  عائم dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  عائم cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  عائم cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  عائم cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_نقطہ (cx0_r, cy0_r);
  grad_انکڑے[0] = clamp_i16 (cx0_r);
  grad_انکڑے[1] = clamp_i16 (cy0_r);
  grad_انکڑے[2] = clamp_i16 (cx1 - cx0);
  grad_انکڑے[3] = clamp_i16 (cy1 - cy0);
  grad_انکڑے[4] = clamp_i16 (cr0);
  grad_انکڑے[5] = clamp_i16 (cr1);
  grad_انکڑے[6] = 0;
  grad_انکڑے[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_انکڑے[8]  = Linv[0];
  grad_انکڑے[9]  = Linv[1];
  grad_انکڑے[10] = Linv[2];
  grad_انکڑے[11] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_انکڑے, stops, got)))
  { c->unsupported = سچ; أرجع; }

  بنانشانی grad_بائٹاں = grad_انکڑے.لمائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_انکڑے.arrayZ,
					 grad_بائٹاں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_سوچک, خالی_سوچک);
  إذا (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    أرجع;
  }
  بنانشانی grad_idx = c->sub_blobs.لمائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 1 /* radial */, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_بن_ورتیا,
		    فراغ             *paint_انکڑے,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_انکڑے, is_foreground, color);
}

ساكن فراغ
hb_gpu_paint_خطی_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ             *paint_انکڑے,
			      hb_color_سطر_t  *color_سطر,
			      عائم x0, عائم y0,
			      عائم x1, عائم y1,
			      عائم x2, عائم y2,
			      فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_emit_خطی ((hb_gpu_paint_t *) paint_انکڑے, color_سطر,
			    x0, y0, x1, y1, x2, y2);
}

ساكن فراغ
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			      فراغ             *paint_انکڑے,
			      hb_color_سطر_t  *color_سطر,
			      عائم x0, عائم y0, عائم r0,
			      عائم x1, عائم y1, عائم r1,
			      فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_انکڑے, color_سطر,
			    x0, y0, r0, x1, y1, r1);
}

ساكن فراغ
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_سطر_t *color_سطر,
			 عائم cx, عائم cy,
			 عائم شروع_angle, عائم سرا_angle)
{
  إذا (unlikely (c->unsupported))
    أرجع;
  إذا (unlikely (c->clip_ڈونگھائی == 0))
    أرجع;
  إذا (unlikely (c->عدد_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = سچ;
    أرجع;
  }

  بنانشانی گنتی = hb_color_سطر_get_color_stops (color_سطر, 0, خالی_سوچک, خالی_سوچک);
  إذا (unlikely (!گنتی))
    أرجع;
  hb_color_stop_t ڈھیر_stops[16];
  hb_color_stop_t *stops = ڈھیر_stops;
  hb_color_stop_t *ڈھیر_stops_2 = خالی_سوچک;
  إذا (گنتی > 16)
  {
    ڈھیر_stops_2 = (hb_color_stop_t *) hb_malloc2 (گنتی, حجم (hb_color_stop_t));
    إذا (unlikely (!ڈھیر_stops_2)) { c->unsupported = سچ; أرجع; }
    stops = ڈھیر_stops_2;
  }
  بنانشانی got = گنتی;
  hb_color_سطر_get_color_stops (color_سطر, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  صحيح clip_idx[HB_GPU_PAINT_MAX_CLIP_ڈونگھائی];
  إذا (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (ڈھیر_stops_2); أرجع; }

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
  hb_سمتیہ_t<int16_t> grad_انکڑے;
  إذا (unlikely (!grad_انکڑے.resize (8)))
  { c->unsupported = سچ; hb_free (ڈھیر_stops_2); أرجع; }

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops, got, &mn, &mx);
  عائم a_وسعت = سرا_angle - شروع_angle;
  عائم a_lo = شروع_angle + mn * a_وسعت;
  عائم a_hi = شروع_angle + mx * a_وسعت;
  خودکار rad_منزل_q14_pi = [] (عائم rad) -> int16_t {
    عائم v = rad * (عائم) (1.0 / M_PI);  /* fraction of pi */
    إذا (v <= -2.f) أرجع -32768;
    إذا (v >=  2.f) أرجع  32767;
    أرجع (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  عائم tcx = cx, tcy = cy;
  c->cur_transform.transform_نقطہ (tcx, tcy);
  grad_انکڑے[0] = clamp_i16 (tcx);
  grad_انکڑے[1] = clamp_i16 (tcy);
  grad_انکڑے[2] = rad_منزل_q14_pi (a_lo);
  grad_انکڑے[3] = rad_منزل_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_انکڑے[4] = Linv[0];
  grad_انکڑے[5] = Linv[1];
  grad_انکڑے[6] = Linv[2];
  grad_انکڑے[7] = Linv[3];

  إذا (unlikely (!append_color_stops (grad_انکڑے, stops, got)))
  { c->unsupported = سچ; hb_free (ڈھیر_stops_2); أرجع; }
  hb_free (ڈھیر_stops_2);

  بنانشانی grad_بائٹاں = grad_انکڑے.لمائی * حجم (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((ثابت حرف *) grad_انکڑے.arrayZ,
					 grad_بائٹاں, HB_حافظہ_منوال_DUPLICATE,
					 خالی_سوچک, خالی_سوچک);
  إذا (unlikely (!grad_blob || !c->ڈالو_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = سچ;
    أرجع;
  }
  بنانشانی grad_idx = c->sub_blobs.لمائی - 1;

  int16_t *meta = emit_gradient_op_سرخی (c, 2 /* sweep */, clip_idx);
  إذا (unlikely (!meta)) أرجع;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->عدد_ops++;
}

ساكن فراغ
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_بن_ورتیا,
			     فراغ             *paint_انکڑے,
			     hb_color_سطر_t  *color_سطر,
			     عائم cx, عائم cy,
			     عائم شروع_angle,
			     عائم سرا_angle,
			     فراغ             *user_انکڑے HB_بن_ورتیا)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_انکڑے, color_سطر,
			   cx, cy, شروع_angle, سرا_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
ساكن hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_بن_ورتیا,
		    فراغ               *paint_انکڑے,
		    hb_blob_t          *image   HB_بن_ورتیا,
		    بنانشانی صحيح        چوڑائی   HB_بن_ورتیا,
		    بنانشانی صحيح        اچائی  HB_بن_ورتیا,
		    hb_ٹیگ_t            format  HB_بن_ورتیا,
		    عائم               slant   HB_بن_ورتیا,
		    hb_glyph_extents_t *extents HB_بن_ورتیا,
		    فراغ               *user_انکڑے HB_بن_ورتیا)
{
  ((hb_gpu_paint_t *) paint_انکڑے)->unsupported = سچ;
  أرجع خطأ;
}

ساكن hb_bool_t
hb_gpu_paint_سیٹ_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے,
			 int64_t budget, فراغ *)
{
  خودکار *paint = (hb_gpu_paint_t *) paint_انکڑے;
  paint->budget = budget;
  paint->recharge_budget ();
  أرجع سچ;
}

ساكن int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع ((hb_gpu_paint_t *) paint_انکڑے)->budget;
}

ساكن int64_t *
hb_gpu_paint_get_budget_بچیا (hb_paint_funcs_t *, فراغ *paint_انکڑے, فراغ *)
{
  أرجع &((hb_gpu_paint_t *) paint_انکڑے)->budget_بچیا;
}

ساكن مضمن فراغ free_static_gpu_paint_funcs ();

ساكن بنية hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_سیٹ_ڈالو_transform_func        (funcs, hb_gpu_paint_ڈالو_transform,        خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func         (funcs, hb_gpu_paint_نکالو_transform,         خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func       (funcs, hb_gpu_paint_ڈالو_clip_glyph,       خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_شروع_func  (funcs, hb_gpu_paint_ڈالو_clip_رستہ_شروع,  خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_سرا_func    (funcs, hb_gpu_paint_ڈالو_clip_رستہ_سرا,    خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func              (funcs, hb_gpu_paint_نکالو_clip,              خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func            (funcs, hb_gpu_paint_ڈالو_گروہ,            خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func             (funcs, hb_gpu_paint_نکالو_گروہ,             خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func                 (funcs, hb_gpu_paint_color,                 خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func       (funcs, hb_gpu_paint_خطی_gradient,       خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  خالی_سوچک, خالی_سوچک);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_سیٹ_image_func                 (funcs, hb_gpu_paint_image,                 خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_gpu_paint_سیٹ_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_gpu_paint_get_budget, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_gpu_paint_get_budget_بچیا, خالی_سوچک, خالی_سوچک);

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
hb_gpu_paint_سیٹ_user_انکڑے (hb_gpu_paint_t     *paint,
			    hb_user_انکڑے_کلید_t *key,
			    فراغ               *انکڑے,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  أرجع hb_object_سیٹ_user_انکڑے (paint, key, انکڑے, destroy, replace);
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
hb_gpu_paint_get_user_انکڑے (ثابت hb_gpu_paint_t *paint,
			    hb_user_انکڑے_کلید_t   *key)
{
  أرجع hb_object_get_user_انکڑے (paint, key);
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
hb_gpu_paint_get_funcs (ثابت hb_gpu_paint_t *paint HB_بن_ورتیا)
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
hb_gpu_paint_سیٹ_palette (hb_gpu_paint_t *paint,
			  بنانشانی        palette)
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
بنانشانی
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
hb_gpu_paint_سیٹ_custom_palette_color (hb_gpu_paint_t *paint,
				       بنانشانی صحيح    color_اشاریہ,
				       hb_color_t      color)
{
  إذا (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    إذا (unlikely (!paint->custom_palette))
      أرجع خطأ;
  }
  hb_map_سیٹ (paint->custom_palette, color_اشاریہ, color);
  أرجع hb_map_ونڈ_successful (paint->custom_palette);
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
hb_gpu_paint_سیٹ_scale (hb_gpu_paint_t *paint,
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
  hb_gpu_paint_سیٹ_scale (paint, x_scale, y_scale);
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

  إذا (unlikely (paint->unsupported))
    أرجع خالی_سوچک;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  إذا (paint->عدد_ops == 0)
    أرجع hb_blob_get_خالی_تھاواں ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  ثابت بنانشانی سرخی_texels = 3;
  ثابت بنانشانی texel_بائٹاں   = 8;

  بنانشانی ops_texels = paint->ops.لمائی / 4;
  بنانشانی sub_بائٹاں = 0;
  لكل (hb_blob_t *b : paint->sub_blobs)
    إذا (unlikely (hb_unsigned_جوڑو_overflows (sub_بائٹاں,
					     hb_blob_get_لمائی (b),
					     &sub_بائٹاں)))
      أرجع خالی_سوچک;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  إذا (unlikely (sub_بائٹاں % texel_بائٹاں))
    أرجع خالی_سوچک;

  بنانشانی کل_بائٹاں = سرخی_texels * texel_بائٹاں
			+ paint->ops.لمائی * 2;
  إذا (unlikely (hb_unsigned_جوڑو_overflows (کل_بائٹاں, sub_بائٹاں,
					   &کل_بائٹاں)))
    أرجع خالی_سوچک;

  بنانشانی buf_سمرتھا = 0;
  حرف *replaced_recycled_buf = خالی_سوچک;
  حرف *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, کل_بائٹاں,
					&buf_سمرتھا, &replaced_recycled_buf);
  إذا (unlikely (!buf_raw))
    أرجع خالی_سوچک;
  int16_t *buf = (int16_t *) (فراغ *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  بنانشانی sub_offsets_گنتی = paint->sub_blobs.لمائی;
  hb_سمتیہ_t<بنانشانی> sub_offsets;
  إذا (unlikely (!sub_offsets.resize (sub_offsets_گنتی)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    أرجع خالی_سوچک;
  }
  بنانشانی cursor = سرخی_texels + ops_texels;
  لكل (بنانشانی i = 0; i < sub_offsets_گنتی; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_لمائی (paint->sub_blobs[i]) / texel_بائٹاں;
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
  int16_t *ops_باہرلا = buf + سرخی_texels * 4;
  hb_memcpy (ops_باہرلا, paint->ops.arrayZ, paint->ops.لمائی * 2);

  بنانشانی i = 0;
  طالما (i < paint->ops.لمائی)
  {
    int16_t op_type = ops_باہرلا[i];
    بدل (op_type)
    {
      حالة HB_GPU_PAINT_OP_LAYER_SOLID:
      حالة HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_باہرلا[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	بنانشانی slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	لكل (بنانشانی k = 0; k < 4; k++)
	{
	  إذا (!slots[k]) استمر;
	  بنانشانی p = i + slots[k];
	  بنانشانی idx = ((uint16_t) ops_باہرلا[p] << 16)
		       |  (uint16_t) ops_باہرلا[p + 1];
	  إذا (idx < sub_offsets_گنتی)
	  {
	    بنانشانی off = sub_offsets[idx];
	    ops_باہرلا[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_باہرلا[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	اكسر;
      }
      حالة HB_GPU_PAINT_OP_ڈالو_گروہ:
      حالة HB_GPU_PAINT_OP_نکالو_گروہ:
	i += 4;  /* 1 texel */
	اكسر;
      افتراضي:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	أرجع خالی_سوچک;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  حرف *dst = (حرف *) buf + سرخی_texels * texel_بائٹاں
			+ paint->ops.لمائی * 2;
  لكل (hb_blob_t *b : paint->sub_blobs)
  {
    بنانشانی len = hb_blob_get_لمائی (b);
    hb_memcpy (dst, hb_blob_get_انکڑے (b, خالی_سوچک), len);
    dst += len;
  }

  إذا (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->چوڑائی     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->اچائی    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = خالی_سوچک;
  أرجع hb_blob_t::recycle_finalize (buf_raw, buf_سمرتھا, کل_بائٹاں,
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
  paint->sub_بائٹاں = 0;
  paint->clip_ڈونگھائی = 0;
  paint->pending_clip_رستہ = خطأ;
  paint->unsupported = خطأ;
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
فراغ
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = خالی_سوچک;
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
hb_gpu_paint_shader_سروت (hb_gpu_shader_stage_t stage,
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
    افتراضي: أرجع خالی_سوچک;
    }
  حالة HB_GPU_SHADER_STAGE_راس:
    بدل (lang) {
    حالة HB_GPU_SHADER_LANG_GLSL:
    حالة HB_GPU_SHADER_LANG_MSL:
    حالة HB_GPU_SHADER_LANG_WGSL:
    حالة HB_GPU_SHADER_LANG_HLSL: أرجع "";
    حالة HB_GPU_SHADER_LANG_INVALID:
    افتراضي: أرجع خالی_سوچک;
    }
  افتراضي:
    أرجع خالی_سوچک;
  }
}
