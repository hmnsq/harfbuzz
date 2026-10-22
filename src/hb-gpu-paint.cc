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

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-gpu.h"
#అన్తర్భావ "hb-gpu-paint.hh"
#అన్తర్భావ "hb-gpu-draw.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-machinery.hh"
#అన్తర్భావ "hb-paint.hh"


/* ---- Paint callbacks ---- */

/* Op-type tags.  Must match the layout documented in
 * hb-gpu-paint.hh. */
గణన {
  HB_GPU_PAINT_OP_LAYER_SOLID    = 0,
  HB_GPU_PAINT_OP_LAYER_GRADIENT = 1,
  HB_GPU_PAINT_OP_నెట్టు_సమూహం     = 2,
  HB_GPU_PAINT_OP_తీయి_సమూహం      = 3,
};

/* Encoder limits.  Hitting any of these flips paint->unsupported and
 * makes hb_gpu_paint_encode() return NULL rather than emit a blob
 * that would render incorrectly. */
#నిర్వచించు HB_GPU_PAINT_MAX_OPS         0x7fff /* num_ops is i16 in the blob header */
#నిర్వచించు HB_GPU_PAINT_MAX_సమూహం_లోతు 4      /* matches HB_GPU_PAINT_GROUP_DEPTH in fragment shader */
#నిర్వచించు HB_GPU_PAINT_MAX_CLIP_లోతు  3      /* shader can intersect up to 3 clip outlines per layer */
#నిర్వచించు HB_GPU_PAINT_MAX_SUB_BLOBS   1024   /* bound clip-glyph rasterizations per paint walk */

/* Layer-op flag bits (LAYER_SOLID texel 0 .g, LAYER_GRADIENT
 * texel 0 .g where bits 0-7 hold the gradient subtype). */
#నిర్వచించు HB_GPU_PAINT_FLAG_IS_FOREGROUND  0x0001
#నిర్వచించు HB_GPU_PAINT_FLAG_HAS_CLIP2      0x0100
#నిర్వచించు HB_GPU_PAINT_FLAG_HAS_CLIP3      0x0200

స్థిర hb_bool_t
hb_gpu_paint_custom_palette_color (hb_paint_funcs_t *funcs HB_వాడని,
				   శూన్య             *paint_దత్తాంశం,
				   గుర్తులేని          color_సూచీ,
				   hb_color_t       *color,
				   శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (likely (c->custom_palette && hb_map_has (c->custom_palette, color_సూచీ)))
  {
    *color = hb_map_get (c->custom_palette, color_సూచీ);
    తిరిగి నిజం;
  }
  తిరిగి అబద్ధం;
}

స్థిర శూన్య
hb_gpu_paint_నెట్టు_clip_glyph (hb_paint_funcs_t *funcs HB_వాడని,
			      శూన్య             *paint_దత్తాంశం,
			      hb_codepoint_t    glyph,
			      hb_font_t        *font,
			      శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;

  ఒకవేళ (unlikely (c->clip_లోతు >= HB_GPU_PAINT_MAX_CLIP_లోతు))
  {
    c->unsupported = నిజం;
    c->clip_లోతు++;
    తిరిగి;
  }
  c->clip_దొంతర[c->clip_లోతు] = { glyph, font, c->cur_transform, -1, 0, 0, 0, 0 };
  c->clip_లోతు++;
}

స్థిర శూన్య
hb_gpu_paint_తీయి_clip (hb_paint_funcs_t *funcs HB_వాడని,
		       శూన్య             *paint_దత్తాంశం,
		       శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (likely (c->clip_లోతు > 0)) c->clip_లోతు--;
}

/* Arbitrary-path clip: hand the caller the same hb_gpu_draw
 * funcs the encoder uses for glyph outlines, with scratch_draw
 * as the accumulator.  push_clip_path_end encodes the captured
 * geometry into a sub-blob and commits it to clip_stack just
 * like a glyph clip.  No font ref needed — the sub-blob is a
 * self-contained slug bundle. */
స్థిర hb_draw_funcs_t *
hb_gpu_paint_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *funcs HB_వాడని,
				   శూన్య             *paint_దత్తాంశం,
				   శూన్య            **draw_దత్తాంశం,
				   శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;

  ఒకవేళ (unlikely (c->clip_లోతు >= HB_GPU_PAINT_MAX_CLIP_లోతు))
  {
    c->unsupported = నిజం;
    *draw_దత్తాంశం = శూన్యసూచిక;
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ఒకవేళ (unlikely (!c->scratch_draw))
    {
      c->unsupported = నిజం;
      *draw_దత్తాంశం = శూన్యసూచిక;
      తిరిగి శూన్యసూచిక;
    }
  }

  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder; it is
   * read back in push_clip_path_end after the caller has drawn the path. */
  hb_draw_సమితి_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_మిగిలిన);
  hb_gpu_draw_సమితి_scale (c->scratch_draw, c->x_scale, c->y_scale);

  c->pending_clip_మార్గం_transform = c->cur_transform;
  c->pending_clip_మార్గం = నిజం;

  *draw_దత్తాంశం = c->scratch_draw;
  తిరిగి hb_gpu_draw_get_funcs (c->scratch_draw);
}

స్థిర శూన్య
hb_gpu_paint_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *funcs HB_వాడని,
				 శూన్య             *paint_దత్తాంశం,
				 శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;

  ఒకవేళ (unlikely (!c->pending_clip_మార్గం))
    తిరిగి;
  c->pending_clip_మార్గం = అబద్ధం;

  /* Pull back the outline budget the caller's path draw consumed. */
  c->budget_మిగిలిన = hb_draw_get_budget_మిగిలిన (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ఒకవేళ (unlikely (!blob || !c->నెట్టు_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = నిజం;
    తిరిగి;
  }
  పూర్ణాంకం idx = (పూర్ణాంకం) (c->sub_blobs.నిడివి - 1);

  ఒకవేళ (unlikely (c->clip_లోతు >= HB_GPU_PAINT_MAX_CLIP_లోతు))
  {
    c->unsupported = నిజం;
    c->clip_లోతు++;
    తిరిగి;
  }

  పూర్ణాంకం x0 = ext.x_bearing;
  పూర్ణాంకం x1 = hb_saturate_కూడు (ext.x_bearing, ext.వెడల్పు);
  పూర్ణాంకం y0 = ext.y_bearing;
  పూర్ణాంకం y1 = hb_saturate_కూడు (ext.y_bearing, ext.ఎత్తు);

  c->clip_దొంతర[c->clip_లోతు] = {
    HB_CODEPOINT_INVALID, శూన్యసూచిక,
    c->pending_clip_మార్గం_transform,
    idx,
    hb_min (x0, x1), hb_min (y0, y1),
    hb_max (x0, x1), hb_max (y0, y1),
  };
  c->clip_లోతు++;
}

స్థిర శూన్య
hb_gpu_paint_నెట్టు_transform (hb_paint_funcs_t *funcs HB_వాడని,
			     శూన్య             *paint_దత్తాంశం,
			     దశాంశం xx, దశాంశం yx,
			     దశాంశం xy, దశాంశం yy,
			     దశాంశం dx, దశాంశం dy,
			     శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!c->transform_దొంతర.నెట్టు_or_fail (c->cur_transform)))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }
  hb_transform_t<దశాంశం> t (xx, yx, xy, yy, dx, dy);
  c->cur_transform.multiply (t);  /* cur = cur * t */
}

స్థిర శూన్య
hb_gpu_paint_తీయి_transform (hb_paint_funcs_t *funcs HB_వాడని,
			    శూన్య             *paint_దత్తాంశం,
			    శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!c->transform_దొంతర.నిడివి))
    తిరిగి;
  c->cur_transform = c->transform_దొంతర.arrayZ[c->transform_దొంతర.నిడివి - 1];
  c->transform_దొంతర.shrink (c->transform_దొంతర.నిడివి - 1);
}

/* Emit a 1-texel control op.  Used for PUSH_GROUP / POP_GROUP. */
స్థిర శూన్య
emit_control_op (hb_gpu_paint_t *c, int16_t op_type, int16_t aux)
{
  ఒకవేళ (unlikely (c->అంకె_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }
  ఒకవేళ (unlikely (!c->ops.resize (c->ops.నిడివి + 4)))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.నిడివి - 4];
  o[0] = op_type;
  o[1] = aux;
  o[2] = 0;
  o[3] = 0;
  c->అంకె_ops++;
}

స్థిర శూన్య
hb_gpu_paint_నెట్టు_సమూహం (hb_paint_funcs_t *funcs HB_వాడని,
			 శూన్య             *paint_దత్తాంశం,
			 శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  c->సమూహం_లోతు++;
  ఒకవేళ (unlikely (c->సమూహం_లోతు > HB_GPU_PAINT_MAX_సమూహం_లోతు))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }
  emit_control_op (c, HB_GPU_PAINT_OP_నెట్టు_సమూహం, 0);
}

స్థిర శూన్య
hb_gpu_paint_తీయి_సమూహం (hb_paint_funcs_t    *funcs HB_వాడని,
			శూన్య                *paint_దత్తాంశం,
			hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
			శూన్య                *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_t *c = (hb_gpu_paint_t *) paint_దత్తాంశం;
  తార్కిక was_లోపల_range = c->సమూహం_లోతు <= HB_GPU_PAINT_MAX_సమూహం_లోతు;
  ఒకవేళ (likely (c->సమూహం_లోతు > 0)) c->సమూహం_లోతు--;
  ఒకవేళ (was_లోపల_range)
    emit_control_op (c, HB_GPU_PAINT_OP_తీయి_సమూహం, (int16_t) బహుళకం);
}

/* Quantize unsigned byte 0..255 to signed Q15 0..32767. */
స్థిర అంతఃవరుస int16_t
color_గమ్యం_q15 (గుర్తులేని byte_విలువ)
{
  తిరిగి (int16_t) ((byte_విలువ * 32767u + 127u) / 255u);
}

స్థిర అంతఃవరుస శూన్య
color_గమ్యం_q15_rgba (hb_color_t color, int16_t బహిర్గతం[4])
{
  బహిర్గతం[0] = color_గమ్యం_q15 (hb_color_get_red   (color));
  బహిర్గతం[1] = color_గమ్యం_q15 (hb_color_get_green (color));
  బహిర్గతం[2] = color_గమ్యం_q15 (hb_color_get_blue  (color));
  బహిర్గతం[3] = color_గమ్యం_q15 (hb_color_get_alpha (color));
}

స్థిర అంతఃవరుస int16_t
clamp_i16 (దశాంశం v)
{
  ఒకవేళ (v <= -32768.f) తిరిగి -32768;
  ఒకవేళ (v >=  32767.f) తిరిగి  32767;
  ఒకవేళ (unlikely (std::isnan (v))) తిరిగి 0;
  తిరిగి (int16_t) v;
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
స్థిర అంతఃవరుస శూన్య
linv_q10 (స్థిరరాశి hb_transform_t<దశాంశం> &t, int16_t బహిర్గతం[4])
{
  దశాంశం det = t.xx * t.yy - t.xy * t.yx;
  దశాంశం inv = fabsf (det) > 1e-12f ? 1.0f / det : 0.0f;
  బహిర్గతం[0] = clamp_i16 ( t.yy * inv * 1024.f);
  బహిర్గతం[1] = clamp_i16 (-t.xy * inv * 1024.f);
  బహిర్గతం[2] = clamp_i16 (-t.yx * inv * 1024.f);
  బహిర్గతం[3] = clamp_i16 ( t.xx * inv * 1024.f);
}

/* Transforming pen: applies an affine transform to each outline
 * point before forwarding to a downstream hb_draw_funcs_t.  The
 * downstream's inline draw-state bookkeeping runs against a
 * separate state (down_st) so transformed-space updates don't
 * pollute the caller's pre-transform state. */
నిర్మాణం hb_gpu_paint_pen_t
{
  hb_transform_t<దశాంశం> transform;
  hb_draw_funcs_t  *dfuncs;
  శూన్య             *దత్తాంశం;
  hb_draw_state_t   కిందికి_st;
  int64_t          *budget_మిగిలిన;
};

స్థిర శూన్య
hb_gpu_paint_pen_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			  శూన్య *దత్తాంశం, hb_draw_state_t *st HB_వాడని,
			  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్వయంచాలక *c = (hb_gpu_paint_pen_t *) దత్తాంశం;
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);
  c->dfuncs->move_గమ్యం (c->దత్తాంశం, c->కిందికి_st, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_paint_pen_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			  శూన్య *దత్తాంశం, hb_draw_state_t *st HB_వాడని,
			  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్వయంచాలక *c = (hb_gpu_paint_pen_t *) దత్తాంశం;
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);
  c->dfuncs->పంక్తి_గమ్యం (c->దత్తాంశం, c->కిందికి_st, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_paint_pen_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			       శూన్య *దత్తాంశం, hb_draw_state_t *st HB_వాడని,
			       దశాంశం c_x, దశాంశం c_y,
			       దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			       శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్వయంచాలక *c = (hb_gpu_paint_pen_t *) దత్తాంశం;
  c->transform.transform_బిందువు (c_x, c_y);
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);
  c->dfuncs->quadratic_గమ్యం (c->దత్తాంశం, c->కిందికి_st, c_x, c_y, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_paint_pen_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			   శూన్య *దత్తాంశం, hb_draw_state_t *st HB_వాడని,
			   దశాంశం c1_x, దశాంశం c1_y,
			   దశాంశం c2_x, దశాంశం c2_y,
			   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			   శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్వయంచాలక *c = (hb_gpu_paint_pen_t *) దత్తాంశం;
  c->transform.transform_బిందువు (c1_x, c1_y);
  c->transform.transform_బిందువు (c2_x, c2_y);
  c->transform.transform_బిందువు (గమ్యం_x, గమ్యం_y);
  c->dfuncs->cubic_గమ్యం (c->దత్తాంశం, c->కిందికి_st, c1_x, c1_y, c2_x, c2_y, గమ్యం_x, గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_paint_pen_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
			     శూన్య *దత్తాంశం, hb_draw_state_t *st HB_వాడని,
			     శూన్య *user_దత్తాంశం HB_వాడని)
{
  స్వయంచాలక *c = (hb_gpu_paint_pen_t *) దత్తాంశం;
  c->dfuncs->close_మార్గం (c->దత్తాంశం, c->కిందికి_st);
}

స్థిర int64_t *
hb_gpu_paint_pen_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_gpu_paint_pen_t *) draw_దత్తాంశం)->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_gpu_paint_pen_funcs ();

స్థిర నిర్మాణం hb_gpu_paint_pen_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_gpu_paint_pen_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_సమితి_move_గమ్యం_func      (funcs, hb_gpu_paint_pen_move_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func      (funcs, hb_gpu_paint_pen_పంక్తి_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_gpu_paint_pen_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func     (funcs, hb_gpu_paint_pen_cubic_గమ్యం,     శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func   (funcs, hb_gpu_paint_pen_close_మార్గం,   శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_gpu_paint_pen_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_gpu_paint_pen_funcs);
    తిరిగి funcs;
  }
} static_gpu_paint_pen_funcs;

స్థిర అంతఃవరుస శూన్య
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
స్థిర పూర్ణాంకం
emit_clip_sub_blob (hb_gpu_paint_t *c,
		    hb_gpu_paint_t::pending_clip_t &clip)
{
  /* Path clips were already encoded into sub_blobs at
   * push_clip_path_end time; glyph clips are cached here on first
   * use.  Either way: just accumulate extents and hand back the
   * recorded index. */
  ఒకవేళ (clip.sub_blob_సూచీ >= 0)
  {
    c->ext_min_x = hb_min (c->ext_min_x, clip.ext_x0);
    c->ext_max_x = hb_max (c->ext_max_x, clip.ext_x1);
    c->ext_min_y = hb_min (c->ext_min_y, clip.ext_y0);
    c->ext_max_y = hb_max (c->ext_max_y, clip.ext_y1);
    తిరిగి clip.sub_blob_సూచీ;
  }

  /* Adversarial COLR trees can drive thousands of fresh
   * push_clip_glyph / paint_color cycles within a single paint
   * walk, each demanding a fresh sub-blob.  Bound the work. */
  ఒకవేళ (unlikely (c->sub_blobs.నిడివి >= HB_GPU_PAINT_MAX_SUB_BLOBS))
  {
    c->unsupported = నిజం;
    తిరిగి -1;
  }

  /* Out of budget: skip the glyph-outline extraction entirely, so
   * per-glyph outline limits cannot multiply with the caller's
   * paint-graph traversal limits. */
  ఒకవేళ (unlikely (c->budget_మిగిలిన < 0))
  {
    c->unsupported = నిజం;
    తిరిగి -1;
  }

  ఒకవేళ (unlikely (!c->scratch_draw))
  {
    c->scratch_draw = hb_gpu_draw_create_or_fail ();
    ఒకవేళ (unlikely (!c->scratch_draw))
    {
      c->unsupported = నిజం;
      తిరిగి -1;
    }
  }
  hb_gpu_draw_clear (c->scratch_draw);
  /* Seed the scratch encoder's budget from the session's remainder and
   * read it back after the glyph is drawn, so the whole paint walk shares
   * one budget through the public draw-budget API. */
  hb_draw_సమితి_budget (hb_gpu_draw_get_funcs (c->scratch_draw), c->scratch_draw,
		      c->budget_మిగిలిన);

  తార్కిక ok;
  ఒకవేళ (clip.transform.is_తటస్థం ())
  {
    /* Fast path: feed the glyph outline straight into the draw
     * encoder with no adapter. */
    ok = hb_gpu_draw_glyph_or_fail (c->scratch_draw, clip.font, clip.glyph);
  }
  లేకపోతే
  {
    /* Transform each outline point by the transform that was
     * current at push_clip_glyph time -- NOT the innermost
     * cur_transform, which may have additional PaintTransform
     * wrappers from the paint child underneath. */
    పూర్ణాంకం x_scale, y_scale;
    hb_font_get_scale (clip.font, &x_scale, &y_scale);
    hb_gpu_draw_సమితి_scale (c->scratch_draw, x_scale, y_scale);

    hb_gpu_paint_pen_t pen = {};
    pen.transform = clip.transform;
    pen.dfuncs    = hb_gpu_draw_get_funcs (c->scratch_draw);
    pen.దత్తాంశం      = c->scratch_draw;
    pen.కిందికి_st   = HB_DRAW_STATE_DEFAULT;
    pen.budget_మిగిలిన = &c->scratch_draw->budget_మిగిలిన;
    ok = hb_font_draw_glyph_or_fail (clip.font, clip.glyph,
				     static_gpu_paint_pen_funcs.get_unconst (),
				     &pen);
    /* Close any trailing open path on the downstream state, since
     * hb_font_draw_glyph_or_fail only closes via our pen's state. */
    pen.dfuncs->close_మార్గం (pen.దత్తాంశం, pen.కిందికి_st);
  }
  c->budget_మిగిలిన = hb_draw_get_budget_మిగిలిన (hb_gpu_draw_get_funcs (c->scratch_draw),
						      c->scratch_draw);
  ఒకవేళ (!ok)
    తిరిగి -1;  /* Clip glyph has no outline -- skip. */

  hb_glyph_extents_t ext;
  hb_blob_t *blob = hb_gpu_draw_encode (c->scratch_draw, &ext);
  ఒకవేళ (unlikely (!blob || !c->నెట్టు_sub_blob (blob)))
  {
    hb_blob_destroy (blob);
    c->unsupported = నిజం;
    తిరిగి -1;
  }

  /* Accumulate extents: x_bearing/y_bearing are top-left, width
   * positive, height negative (growing down). */
  పూర్ణాంకం x0 = ext.x_bearing;
  పూర్ణాంకం x1 = hb_saturate_కూడు (ext.x_bearing, ext.వెడల్పు);
  పూర్ణాంకం y0 = ext.y_bearing;
  పూర్ణాంకం y1 = hb_saturate_కూడు (ext.y_bearing, ext.ఎత్తు);
  c->ext_min_x = hb_min (c->ext_min_x, hb_min (x0, x1));
  c->ext_max_x = hb_max (c->ext_max_x, hb_max (x0, x1));
  c->ext_min_y = hb_min (c->ext_min_y, hb_min (y0, y1));
  c->ext_max_y = hb_max (c->ext_max_y, hb_max (y0, y1));

  పూర్ణాంకం idx = (పూర్ణాంకం) (c->sub_blobs.నిడివి - 1);

  /* Memoize on the slot.  The (glyph, font, transform) tuple is
   * fixed between push_clip_glyph and pop_clip, and pop_clip's
   * subsequent push reinitializes the slot to sub_blob_index = -1,
   * so a stale cache cannot leak across pushes. */
  clip.sub_blob_సూచీ = idx;
  clip.ext_x0 = hb_min (x0, x1);
  clip.ext_y0 = hb_min (y0, y1);
  clip.ext_x1 = hb_max (x0, x1);
  clip.ext_y1 = hb_max (y0, y1);

  తిరిగి idx;
}

/* Emit a sub-blob for every clip currently on the stack and return
 * their indices in @out (set to -1 for unused slots).  Returns
 * false if any clip's outline is empty (caller should skip the
 * layer entirely) or hb_gpu_draw_encode failed. */
స్థిర తార్కిక
emit_all_clip_sub_blobs (hb_gpu_paint_t *c, పూర్ణాంకం బహిర్గతం[HB_GPU_PAINT_MAX_CLIP_లోతు])
{
  కొరకు (గుర్తులేని i = 0; i < HB_GPU_PAINT_MAX_CLIP_లోతు; i++)
    బహిర్గతం[i] = -1;
  కొరకు (గుర్తులేని i = 0; i < hb_min (c->clip_లోతు, (గుర్తులేని) HB_GPU_PAINT_MAX_CLIP_లోతు); i++)
  {
    బహిర్గతం[i] = emit_clip_sub_blob (c, c->clip_దొంతర[i]);
    ఒకవేళ (బహిర్గతం[i] < 0)
      తిరిగి అబద్ధం;
  }
  తిరిగి నిజం;
}

/* Emit a 3-texel LAYER_GRADIENT op header (excluding the
 * gradient-meta texel which the caller fills in).  @subtype is
 * 0 = linear, 1 = radial, 2 = sweep. */
స్థిర int16_t *
emit_gradient_op_శీర్షిక (hb_gpu_paint_t *c, గుర్తులేని subtype,
			 స్థిరరాశి పూర్ణాంకం clip_idx[HB_GPU_PAINT_MAX_CLIP_లోతు])
{
  ఒకవేళ (unlikely (!c->ops.resize (c->ops.నిడివి + 12)))
  { c->unsupported = నిజం; తిరిగి శూన్యసూచిక; }
  int16_t *o = &c->ops.arrayZ[c->ops.నిడివి - 12];
  int16_t flags = (int16_t) subtype;
  ఒకవేళ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ఒకవేళ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_GRADIENT;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  తిరిగి o + 8;  /* texel 2: gradient meta -- caller fills */
}

స్థిర శూన్య
hb_gpu_paint_emit_solid (hb_gpu_paint_t *c,
			 hb_bool_t       is_foreground,
			 hb_color_t      color)
{
  ఒకవేళ (unlikely (c->unsupported))
    తిరిగి;
  ఒకవేళ (unlikely (c->clip_లోతు == 0))
    తిరిగి;
  ఒకవేళ (unlikely (c->అంకె_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }

  పూర్ణాంకం clip_idx[HB_GPU_PAINT_MAX_CLIP_లోతు];
  ఒకవేళ (!emit_all_clip_sub_blobs (c, clip_idx))
    తిరిగి;

  /* Emit LAYER_SOLID op: 3 texels (12 int16s).
   *   texel 0: op_type, flags, clip1_hi, clip1_lo
   *   texel 1: clip2_hi, clip2_lo, 0, 0  (only meaningful if HAS_CLIP2)
   *   texel 2: r_q15, g_q15, b_q15, a_q15
   * Payloads hold sub_blob indices; hb_gpu_paint_encode() patches
   * them into texel offsets at serialize time.
   * flags bit 0: use shader foreground uniform instead of the
   * baked color (so runtime foreground-color changes still work).
   * flags bit 8: a second clip outline is present in texel 1. */
  ఒకవేళ (unlikely (!c->ops.resize (c->ops.నిడివి + 12)))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }
  int16_t *o = &c->ops.arrayZ[c->ops.నిడివి - 12];
  int16_t flags = (int16_t) (is_foreground ? HB_GPU_PAINT_FLAG_IS_FOREGROUND : 0);
  ఒకవేళ (clip_idx[1] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP2;
  ఒకవేళ (clip_idx[2] >= 0) flags |= HB_GPU_PAINT_FLAG_HAS_CLIP3;
  o[0] = HB_GPU_PAINT_OP_LAYER_SOLID;
  o[1] = flags;
  o[2] = (int16_t) ((clip_idx[0] >> 16) & 0xffff);
  o[3] = (int16_t) (clip_idx[0] & 0xffff);
  o[4] = clip_idx[1] >= 0 ? (int16_t) ((clip_idx[1] >> 16) & 0xffff) : 0;
  o[5] = clip_idx[1] >= 0 ? (int16_t) (clip_idx[1] & 0xffff) : 0;
  o[6] = clip_idx[2] >= 0 ? (int16_t) ((clip_idx[2] >> 16) & 0xffff) : 0;
  o[7] = clip_idx[2] >= 0 ? (int16_t) (clip_idx[2] & 0xffff) : 0;
  color_గమ్యం_q15_rgba (color, o + 8);
  c->అంకె_ops++;
}

/* Append a color line (@count stops) to @grad_data in the encoded
 * stop format used by gradient ops: 2 texels per stop.
 *   texel a: (offset_q15, flags, _, _) with flags bit 0 = is_foreground
 *   texel b: (r_q15, g_q15, b_q15, a_q15)
 * Returns false on allocation failure. */
స్థిర తార్కిక
append_color_stops (hb_సదిశ_t<int16_t> &grad_దత్తాంశం,
		    స్థిరరాశి hb_color_stop_t *stops,
		    గుర్తులేని లెక్క)
{
  గుర్తులేని పునాది = grad_దత్తాంశం.నిడివి;
  ఒకవేళ (unlikely (!grad_దత్తాంశం.resize (పునాది + లెక్క * 8)))
    తిరిగి అబద్ధం;
  int16_t *p = grad_దత్తాంశం.arrayZ + పునాది;
  కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
  {
    దశాంశం off = stops[i].స్థానభ్రంశం;
    ఒకవేళ (off < -1.f) off = -1.f;
    లేకపోతే ఒకవేళ (off > 1.f) off = 1.f;
    p[0] = (int16_t) (off * 32767.f);
    p[1] = (int16_t) (stops[i].is_foreground ? 1 : 0);
    p[2] = 0;
    p[3] = 0;
    color_గమ్యం_q15_rgba (stops[i].color, p + 4);
    p += 8;
  }
  తిరిగి నిజం;
}

స్థిర శూన్య
hb_gpu_paint_emit_రేఖీయ (hb_gpu_paint_t  *c,
			  hb_color_పంక్తి_t *color_పంక్తి,
			  దశాంశం x0, దశాంశం y0,
			  దశాంశం x1, దశాంశం y1,
			  దశాంశం x2, దశాంశం y2)
{
  ఒకవేళ (unlikely (c->unsupported))
    తిరిగి;
  ఒకవేళ (unlikely (c->clip_లోతు == 0))
    తిరిగి;
  ఒకవేళ (unlikely (c->అంకె_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }

  /* Resolve stops (triggers custom_palette_color, etc). */
  ఒకవేళ (unlikely (!c->fetch_color_stops (color_పంక్తి)))
    తిరిగి;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  గుర్తులేని got = c->color_stops_scratch.నిడివి;

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);

  /* Emit clip sub-blob(s) first. */
  పూర్ణాంకం clip_idx[HB_GPU_PAINT_MAX_CLIP_లోతు];
  ఒకవేళ (!emit_all_clip_sub_blobs (c, clip_idx)) తిరిగి;

  /* Build gradient params sub-blob.
   *   texel 0: (p0_rendered_x, p0_rendered_y, d_canonical_x, d_canonical_y)
   *     i16 font units; d = p1 - p0 stored in untransformed space
   *   texel 1: L^-1 as 4 i16 Q10 (row-major)
   *   texels 2..N: color stops (2 texels each)
   * The shader computes p_canonical = L^-1 * (renderCoord - p0_rendered)
   * and evaluates t = dot(p_canonical, d) / dot(d, d) in
   * untransformed space, so affine distortions of the axis are
   * handled exactly rather than approximated. */
  hb_సదిశ_t<int16_t> grad_దత్తాంశం;
  ఒకవేళ (unlikely (!grad_దత్తాంశం.resize (8)))
  { c->unsupported = నిజం; తిరిగి; }

  దశాంశం lx0, ly0, lx1, ly1;
  hb_paint_reduce_రేఖీయ_anchors (x0, y0, x1, y1, x2, y2, &lx0, &ly0, &lx1, &ly1);
  /* Normalize stops to [0,1]; shift axis endpoints to compensate. */
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, got, &mn, &mx);
  దశాంశం dx = lx1 - lx0, dy = ly1 - ly0;
  దశాంశం ax0 = lx0 + mn * dx, ay0 = ly0 + mn * dy;
  దశాంశం ax1 = lx0 + mx * dx, ay1 = ly0 + mx * dy;
  /* p0 in rendered space; d in untransformed space. */
  దశాంశం p0x_r = ax0, p0y_r = ay0;
  c->cur_transform.transform_బిందువు (p0x_r, p0y_r);
  grad_దత్తాంశం[0] = clamp_i16 (p0x_r);
  grad_దత్తాంశం[1] = clamp_i16 (p0y_r);
  grad_దత్తాంశం[2] = clamp_i16 (ax1 - ax0);
  grad_దత్తాంశం[3] = clamp_i16 (ay1 - ay0);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_దత్తాంశం[4] = Linv[0];
  grad_దత్తాంశం[5] = Linv[1];
  grad_దత్తాంశం[6] = Linv[2];
  grad_దత్తాంశం[7] = Linv[3];

  ఒకవేళ (unlikely (!append_color_stops (grad_దత్తాంశం, stops, got)))
  { c->unsupported = నిజం; తిరిగి; }

  గుర్తులేని grad_బైట్లు = grad_దత్తాంశం.నిడివి * పరిమాణం (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((స్థిరరాశి అక్షరం *) grad_దత్తాంశం.arrayZ,
					 grad_బైట్లు, HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
					 శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (unlikely (!grad_blob || !c->నెట్టు_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = నిజం;
    తిరిగి;
  }
  గుర్తులేని grad_idx = c->sub_blobs.నిడివి - 1;

  int16_t *meta = emit_gradient_op_శీర్షిక (c, 0 /*రేఖీయ*/, clip_idx);
  ఒకవేళ (unlikely (!meta)) తిరిగి;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->అంకె_ops++;
}

స్థిర శూన్య
hb_gpu_paint_emit_radial (hb_gpu_paint_t  *c,
			  hb_color_పంక్తి_t *color_పంక్తి,
			  దశాంశం x0, దశాంశం y0, దశాంశం r0,
			  దశాంశం x1, దశాంశం y1, దశాంశం r1)
{
  ఒకవేళ (unlikely (c->unsupported))
    తిరిగి;
  ఒకవేళ (unlikely (c->clip_లోతు == 0))
    తిరిగి;
  ఒకవేళ (unlikely (c->అంకె_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }

  ఒకవేళ (unlikely (!c->fetch_color_stops (color_పంక్తి)))
    తిరిగి;
  hb_color_stop_t *stops = c->color_stops_scratch.arrayZ;
  గుర్తులేని got = c->color_stops_scratch.నిడివి;

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);

  పూర్ణాంకం clip_idx[HB_GPU_PAINT_MAX_CLIP_లోతు];
  ఒకవేళ (!emit_all_clip_sub_blobs (c, clip_idx)) తిరిగి;

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
  hb_సదిశ_t<int16_t> grad_దత్తాంశం;
  ఒకవేళ (unlikely (!grad_దత్తాంశం.resize (12)))
  { c->unsupported = నిజం; తిరిగి; }
  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, got, &mn, &mx);
  దశాంశం dx = x1 - x0, dy = y1 - y0, dr = r1 - r0;
  దశాంశం cx0 = x0 + mn * dx, cy0 = y0 + mn * dy, cr0 = r0 + mn * dr;
  దశాంశం cx1 = x0 + mx * dx, cy1 = y0 + mx * dy, cr1 = r0 + mx * dr;
  /* c0 in rendered space; d (= c1 - c0) and radii in untransformed. */
  దశాంశం cx0_r = cx0, cy0_r = cy0;
  c->cur_transform.transform_బిందువు (cx0_r, cy0_r);
  grad_దత్తాంశం[0] = clamp_i16 (cx0_r);
  grad_దత్తాంశం[1] = clamp_i16 (cy0_r);
  grad_దత్తాంశం[2] = clamp_i16 (cx1 - cx0);
  grad_దత్తాంశం[3] = clamp_i16 (cy1 - cy0);
  grad_దత్తాంశం[4] = clamp_i16 (cr0);
  grad_దత్తాంశం[5] = clamp_i16 (cr1);
  grad_దత్తాంశం[6] = 0;
  grad_దత్తాంశం[7] = 0;
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_దత్తాంశం[8]  = Linv[0];
  grad_దత్తాంశం[9]  = Linv[1];
  grad_దత్తాంశం[10] = Linv[2];
  grad_దత్తాంశం[11] = Linv[3];

  ఒకవేళ (unlikely (!append_color_stops (grad_దత్తాంశం, stops, got)))
  { c->unsupported = నిజం; తిరిగి; }

  గుర్తులేని grad_బైట్లు = grad_దత్తాంశం.నిడివి * పరిమాణం (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((స్థిరరాశి అక్షరం *) grad_దత్తాంశం.arrayZ,
					 grad_బైట్లు, HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
					 శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (unlikely (!grad_blob || !c->నెట్టు_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = నిజం;
    తిరిగి;
  }
  గుర్తులేని grad_idx = c->sub_blobs.నిడివి - 1;

  int16_t *meta = emit_gradient_op_శీర్షిక (c, 1 /* radial */, clip_idx);
  ఒకవేళ (unlikely (!meta)) తిరిగి;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->అంకె_ops++;
}

స్థిర శూన్య
hb_gpu_paint_color (hb_paint_funcs_t *funcs HB_వాడని,
		    శూన్య             *paint_దత్తాంశం,
		    hb_bool_t         is_foreground,
		    hb_color_t        color,
		    శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_emit_solid ((hb_gpu_paint_t *) paint_దత్తాంశం, is_foreground, color);
}

స్థిర శూన్య
hb_gpu_paint_రేఖీయ_gradient (hb_paint_funcs_t *funcs HB_వాడని,
			      శూన్య             *paint_దత్తాంశం,
			      hb_color_పంక్తి_t  *color_పంక్తి,
			      దశాంశం x0, దశాంశం y0,
			      దశాంశం x1, దశాంశం y1,
			      దశాంశం x2, దశాంశం y2,
			      శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_emit_రేఖీయ ((hb_gpu_paint_t *) paint_దత్తాంశం, color_పంక్తి,
			    x0, y0, x1, y1, x2, y2);
}

స్థిర శూన్య
hb_gpu_paint_radial_gradient (hb_paint_funcs_t *funcs HB_వాడని,
			      శూన్య             *paint_దత్తాంశం,
			      hb_color_పంక్తి_t  *color_పంక్తి,
			      దశాంశం x0, దశాంశం y0, దశాంశం r0,
			      దశాంశం x1, దశాంశం y1, దశాంశం r1,
			      శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_emit_radial ((hb_gpu_paint_t *) paint_దత్తాంశం, color_పంక్తి,
			    x0, y0, r0, x1, y1, r1);
}

స్థిర శూన్య
hb_gpu_paint_emit_sweep (hb_gpu_paint_t  *c,
			 hb_color_పంక్తి_t *color_పంక్తి,
			 దశాంశం cx, దశాంశం cy,
			 దశాంశం ప్రారంభం_angle, దశాంశం చివర_angle)
{
  ఒకవేళ (unlikely (c->unsupported))
    తిరిగి;
  ఒకవేళ (unlikely (c->clip_లోతు == 0))
    తిరిగి;
  ఒకవేళ (unlikely (c->అంకె_ops >= HB_GPU_PAINT_MAX_OPS))
  {
    c->unsupported = నిజం;
    తిరిగి;
  }

  గుర్తులేని లెక్క = hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (unlikely (!లెక్క))
    తిరిగి;
  hb_color_stop_t దొంతర_stops[16];
  hb_color_stop_t *stops = దొంతర_stops;
  hb_color_stop_t *కుప్ప_stops = శూన్యసూచిక;
  ఒకవేళ (లెక్క > 16)
  {
    కుప్ప_stops = (hb_color_stop_t *) hb_malloc2 (లెక్క, పరిమాణం (hb_color_stop_t));
    ఒకవేళ (unlikely (!కుప్ప_stops)) { c->unsupported = నిజం; తిరిగి; }
    stops = కుప్ప_stops;
  }
  గుర్తులేని got = లెక్క;
  hb_color_పంక్తి_get_color_stops (color_పంక్తి, 0, &got, stops);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);

  పూర్ణాంకం clip_idx[HB_GPU_PAINT_MAX_CLIP_లోతు];
  ఒకవేళ (!emit_all_clip_sub_blobs (c, clip_idx)) { hb_free (కుప్ప_stops); తిరిగి; }

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
  hb_సదిశ_t<int16_t> grad_దత్తాంశం;
  ఒకవేళ (unlikely (!grad_దత్తాంశం.resize (8)))
  { c->unsupported = నిజం; hb_free (కుప్ప_stops); తిరిగి; }

  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops, got, &mn, &mx);
  దశాంశం a_పరిధి = చివర_angle - ప్రారంభం_angle;
  దశాంశం a_lo = ప్రారంభం_angle + mn * a_పరిధి;
  దశాంశం a_hi = ప్రారంభం_angle + mx * a_పరిధి;
  స్వయంచాలక rad_గమ్యం_q14_pi = [] (దశాంశం rad) -> int16_t {
    దశాంశం v = rad * (దశాంశం) (1.0 / M_PI);  /* fraction of pi */
    ఒకవేళ (v <= -2.f) తిరిగి -32768;
    ఒకవేళ (v >=  2.f) తిరిగి  32767;
    తిరిగి (int16_t) (v * 16384.f);
  };
  /* center in rendered space; angles stay canonical. */
  దశాంశం tcx = cx, tcy = cy;
  c->cur_transform.transform_బిందువు (tcx, tcy);
  grad_దత్తాంశం[0] = clamp_i16 (tcx);
  grad_దత్తాంశం[1] = clamp_i16 (tcy);
  grad_దత్తాంశం[2] = rad_గమ్యం_q14_pi (a_lo);
  grad_దత్తాంశం[3] = rad_గమ్యం_q14_pi (a_hi);
  int16_t Linv[4];
  linv_q10 (c->cur_transform, Linv);
  grad_దత్తాంశం[4] = Linv[0];
  grad_దత్తాంశం[5] = Linv[1];
  grad_దత్తాంశం[6] = Linv[2];
  grad_దత్తాంశం[7] = Linv[3];

  ఒకవేళ (unlikely (!append_color_stops (grad_దత్తాంశం, stops, got)))
  { c->unsupported = నిజం; hb_free (కుప్ప_stops); తిరిగి; }
  hb_free (కుప్ప_stops);

  గుర్తులేని grad_బైట్లు = grad_దత్తాంశం.నిడివి * పరిమాణం (int16_t);
  hb_blob_t *grad_blob = hb_blob_create ((స్థిరరాశి అక్షరం *) grad_దత్తాంశం.arrayZ,
					 grad_బైట్లు, HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
					 శూన్యసూచిక, శూన్యసూచిక);
  ఒకవేళ (unlikely (!grad_blob || !c->నెట్టు_sub_blob (grad_blob)))
  {
    hb_blob_destroy (grad_blob);
    c->unsupported = నిజం;
    తిరిగి;
  }
  గుర్తులేని grad_idx = c->sub_blobs.నిడివి - 1;

  int16_t *meta = emit_gradient_op_శీర్షిక (c, 2 /* sweep */, clip_idx);
  ఒకవేళ (unlikely (!meta)) తిరిగి;
  meta[0] = (int16_t) ((grad_idx >> 16) & 0xffff);
  meta[1] = (int16_t) (grad_idx & 0xffff);
  meta[2] = (int16_t) extend;
  meta[3] = (int16_t) got;
  c->అంకె_ops++;
}

స్థిర శూన్య
hb_gpu_paint_sweep_gradient (hb_paint_funcs_t *funcs HB_వాడని,
			     శూన్య             *paint_దత్తాంశం,
			     hb_color_పంక్తి_t  *color_పంక్తి,
			     దశాంశం cx, దశాంశం cy,
			     దశాంశం ప్రారంభం_angle,
			     దశాంశం చివర_angle,
			     శూన్య             *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_paint_emit_sweep ((hb_gpu_paint_t *) paint_దత్తాంశం, color_పంక్తి,
			   cx, cy, ప్రారంభం_angle, చివర_angle);
}

/* PaintImage isn't representable by our slug+gradient encoder; mark
 * unsupported so the caller learns the glyph won't render rather
 * than getting a partial blob.  PaintRectangleClip is intentionally
 * left to the default no-op: the encoder simply ignores the
 * rectangle (output is bigger than it should be, but renders). */
స్థిర hb_bool_t
hb_gpu_paint_image (hb_paint_funcs_t   *funcs   HB_వాడని,
		    శూన్య               *paint_దత్తాంశం,
		    hb_blob_t          *image   HB_వాడని,
		    గుర్తులేని పూర్ణాంకం        వెడల్పు   HB_వాడని,
		    గుర్తులేని పూర్ణాంకం        ఎత్తు  HB_వాడని,
		    hb_ట్యాగ్_t            format  HB_వాడని,
		    దశాంశం               slant   HB_వాడని,
		    hb_glyph_extents_t *extents HB_వాడని,
		    శూన్య               *user_దత్తాంశం HB_వాడని)
{
  ((hb_gpu_paint_t *) paint_దత్తాంశం)->unsupported = నిజం;
  తిరిగి అబద్ధం;
}

స్థిర hb_bool_t
hb_gpu_paint_సమితి_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం,
			 int64_t budget, శూన్య *)
{
  స్వయంచాలక *paint = (hb_gpu_paint_t *) paint_దత్తాంశం;
  paint->budget = budget;
  paint->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_gpu_paint_get_budget (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_gpu_paint_t *) paint_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_gpu_paint_get_budget_మిగిలిన (hb_paint_funcs_t *, శూన్య *paint_దత్తాంశం, శూన్య *)
{
  తిరిగి &((hb_gpu_paint_t *) paint_దత్తాంశం)->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_gpu_paint_funcs ();

స్థిర నిర్మాణం hb_gpu_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_gpu_paint_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();

    hb_paint_funcs_సమితి_నెట్టు_transform_func        (funcs, hb_gpu_paint_నెట్టు_transform,        శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func         (funcs, hb_gpu_paint_తీయి_transform,         శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func       (funcs, hb_gpu_paint_నెట్టు_clip_glyph,       శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_ప్రారంభం_func  (funcs, hb_gpu_paint_నెట్టు_clip_మార్గం_ప్రారంభం,  శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_చివర_func    (funcs, hb_gpu_paint_నెట్టు_clip_మార్గం_చివర,    శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func              (funcs, hb_gpu_paint_తీయి_clip,              శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func            (funcs, hb_gpu_paint_నెట్టు_సమూహం,            శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func             (funcs, hb_gpu_paint_తీయి_సమూహం,             శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func                 (funcs, hb_gpu_paint_color,                 శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func       (funcs, hb_gpu_paint_రేఖీయ_gradient,       శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func       (funcs, hb_gpu_paint_radial_gradient,       శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func        (funcs, hb_gpu_paint_sweep_gradient,        శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_custom_palette_color_func  (funcs, hb_gpu_paint_custom_palette_color,  శూన్యసూచిక, శూన్యసూచిక);
    /* PaintImage can't be represented by our slug+gradient encoder. */
    hb_paint_funcs_సమితి_image_func                 (funcs, hb_gpu_paint_image,                 శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_gpu_paint_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_gpu_paint_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_gpu_paint_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_paint_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_paint_funcs);

    తిరిగి funcs;
  }
} static_gpu_paint_funcs;

స్థిర అంతఃవరుస శూన్య
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
hb_gpu_paint_create_or_fail (శూన్య)
{
  తిరిగి hb_object_create<hb_gpu_paint_t> ();
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
  తిరిగి hb_object_reference (paint);
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
శూన్య
hb_gpu_paint_destroy (hb_gpu_paint_t *paint)
{
  ఒకవేళ (!hb_object_should_destroy (paint))
    తిరిగి;

  hb_map_destroy (paint->custom_palette);
  కొరకు (hb_blob_t *b : paint->sub_blobs)
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
hb_gpu_paint_సమితి_user_దత్తాంశం (hb_gpu_paint_t     *paint,
			    hb_user_దత్తాంశం_కీలకం_t *key,
			    శూన్య               *దత్తాంశం,
			    hb_destroy_func_t   destroy,
			    hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (paint, key, దత్తాంశం, destroy, replace);
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
శూన్య *
hb_gpu_paint_get_user_దత్తాంశం (స్థిరరాశి hb_gpu_paint_t *paint,
			    hb_user_దత్తాంశం_కీలకం_t   *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (paint, key);
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
hb_gpu_paint_get_funcs (స్థిరరాశి hb_gpu_paint_t *paint HB_వాడని)
{
  తిరిగి static_gpu_paint_funcs.get_unconst ();
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
శూన్య
hb_gpu_paint_సమితి_palette (hb_gpu_paint_t *paint,
			  గుర్తులేని        palette)
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
గుర్తులేని
hb_gpu_paint_get_palette (స్థిరరాశి hb_gpu_paint_t *paint)
{
  తిరిగి paint->palette;
}

/**
 * hb_gpu_paint_clear_custom_palette_colors:
 * @paint: a GPU color-glyph paint encoder
 *
 * Clears all custom palette color overrides previously set on @paint.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_gpu_paint_clear_custom_palette_colors (hb_gpu_paint_t *paint)
{
  ఒకవేళ (paint->custom_palette)
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
hb_gpu_paint_సమితి_custom_palette_color (hb_gpu_paint_t *paint,
				       గుర్తులేని పూర్ణాంకం    color_సూచీ,
				       hb_color_t      color)
{
  ఒకవేళ (unlikely (!paint->custom_palette))
  {
    paint->custom_palette = hb_map_create ();
    ఒకవేళ (unlikely (!paint->custom_palette))
      తిరిగి అబద్ధం;
  }
  hb_map_సమితి (paint->custom_palette, color_సూచీ, color);
  తిరిగి hb_map_కేటాయింపు_successful (paint->custom_palette);
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
శూన్య
hb_gpu_paint_సమితి_scale (hb_gpu_paint_t *paint,
			పూర్ణాంకం             x_scale,
			పూర్ణాంకం             y_scale)
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
శూన్య
hb_gpu_paint_get_scale (స్థిరరాశి hb_gpu_paint_t *paint,
			పూర్ణాంకం                  *x_scale,
			పూర్ణాంకం                  *y_scale)
{
  ఒకవేళ (x_scale) *x_scale = paint->x_scale;
  ఒకవేళ (y_scale) *y_scale = paint->y_scale;
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
  పూర్ణాంకం x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_సమితి_scale (paint, x_scale, y_scale);
  తిరిగి hb_font_paint_glyph_or_fail (font, glyph,
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
శూన్య
hb_gpu_paint_glyph (hb_gpu_paint_t *paint,
		    hb_font_t      *font,
		    hb_codepoint_t  glyph)
{
  పూర్ణాంకం x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_paint_సమితి_scale (paint, x_scale, y_scale);
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

  ఒకవేళ (unlikely (paint->unsupported))
    తిరిగి శూన్యసూచిక;
  /* No ink (e.g. space glyph): return the empty-blob singleton, not
   * nullptr.  Same convention as hb_gpu_draw_encode(); callers can
   * distinguish "nothing to render" (length 0) from "encoder
   * failed" (NULL). */
  ఒకవేళ (paint->అంకె_ops == 0)
    తిరిగి hb_blob_get_ఖాళీలు ();

  /* Layout:
   *   header          (3 texels = 24 bytes)
   *   op stream       (ops.length i16 words, multiple of 4)
   *   sub-payloads    (concat of each sub_blob's data, 8-byte
   *                    aligned since draw blobs are RGBA16I)
   */
  స్థిరరాశి గుర్తులేని శీర్షిక_texels = 3;
  స్థిరరాశి గుర్తులేని texel_బైట్లు   = 8;

  గుర్తులేని ops_texels = paint->ops.నిడివి / 4;
  గుర్తులేని sub_బైట్లు = 0;
  కొరకు (hb_blob_t *b : paint->sub_blobs)
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (sub_బైట్లు,
					     hb_blob_get_నిడివి (b),
					     &sub_బైట్లు)))
      తిరిగి శూన్యసూచిక;
  /* Sub-blobs come from the draw encoder which produces 8-byte
   * aligned blobs; assert so we notice if that ever changes. */
  ఒకవేళ (unlikely (sub_బైట్లు % texel_బైట్లు))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని మొత్తం_బైట్లు = శీర్షిక_texels * texel_బైట్లు
			+ paint->ops.నిడివి * 2;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (మొత్తం_బైట్లు, sub_బైట్లు,
					   &మొత్తం_బైట్లు)))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని buf_సామర్థ్యం = 0;
  అక్షరం *replaced_recycled_buf = శూన్యసూచిక;
  అక్షరం *buf_raw = hb_blob_t::recycle_acquire (paint->recycled_blob, మొత్తం_బైట్లు,
					&buf_సామర్థ్యం, &replaced_recycled_buf);
  ఒకవేళ (unlikely (!buf_raw))
    తిరిగి శూన్యసూచిక;
  int16_t *buf = (int16_t *) (శూన్య *) buf_raw;

  /* Compute each sub_blob's texel offset (relative to blob base). */
  గుర్తులేని sub_offsets_లెక్క = paint->sub_blobs.నిడివి;
  hb_సదిశ_t<గుర్తులేని> sub_offsets;
  ఒకవేళ (unlikely (!sub_offsets.resize (sub_offsets_లెక్క)))
  {
    hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
    తిరిగి శూన్యసూచిక;
  }
  గుర్తులేని cursor = శీర్షిక_texels + ops_texels;
  కొరకు (గుర్తులేని i = 0; i < sub_offsets_లెక్క; i++)
  {
    sub_offsets[i] = cursor;
    cursor += hb_blob_get_నిడివి (paint->sub_blobs[i]) / texel_బైట్లు;
  }

  /* Header. */
  buf[0] = (int16_t) paint->అంకె_ops;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  buf[4] = (int16_t) hb_clamp (paint->ext_min_x, -0x7fff, 0x7fff);
  buf[5] = (int16_t) hb_clamp (paint->ext_min_y, -0x7fff, 0x7fff);
  buf[6] = (int16_t) hb_clamp (paint->ext_max_x, -0x7fff, 0x7fff);
  buf[7] = (int16_t) hb_clamp (paint->ext_max_y, -0x7fff, 0x7fff);
  buf[8] = (int16_t) శీర్షిక_texels;
  buf[9]  = 0;
  buf[10] = 0;
  buf[11] = 0;

  /* Op stream: copy raw, then patch payload slots to texel
   * offsets where op_type indicates a sub_blob reference. */
  int16_t *ops_బహిర్గతం = buf + శీర్షిక_texels * 4;
  hb_memcpy (ops_బహిర్గతం, paint->ops.arrayZ, paint->ops.నిడివి * 2);

  గుర్తులేని i = 0;
  అయితే (i < paint->ops.నిడివి)
  {
    int16_t op_type = ops_బహిర్గతం[i];
    ఎంపిక (op_type)
    {
      స్థితి HB_GPU_PAINT_OP_LAYER_SOLID:
      స్థితి HB_GPU_PAINT_OP_LAYER_GRADIENT:
      {
	int16_t flags = ops_బహిర్గతం[i + 1];
	/* Sub-blob slots in i16 offsets within the op:
	 *   clip1: [i+2, i+3]                     -- always present
	 *   clip2: [i+4, i+5]                     -- only if HAS_CLIP2
	 *   clip3: [i+6, i+7]                     -- only if HAS_CLIP3
	 *   grad : [i+8, i+9]  (LAYER_GRADIENT only) */
	గుర్తులేని slots[4] = {
	  2,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP2) ? 4u : 0u,
	  (flags & HB_GPU_PAINT_FLAG_HAS_CLIP3) ? 6u : 0u,
	  op_type == HB_GPU_PAINT_OP_LAYER_GRADIENT ? 8u : 0u
	};
	కొరకు (గుర్తులేని k = 0; k < 4; k++)
	{
	  ఒకవేళ (!slots[k]) కొనసాగు;
	  గుర్తులేని p = i + slots[k];
	  గుర్తులేని idx = ((uint16_t) ops_బహిర్గతం[p] << 16)
		       |  (uint16_t) ops_బహిర్గతం[p + 1];
	  ఒకవేళ (idx < sub_offsets_లెక్క)
	  {
	    గుర్తులేని off = sub_offsets[idx];
	    ops_బహిర్గతం[p]     = (int16_t) ((off >> 16) & 0xffff);
	    ops_బహిర్గతం[p + 1] = (int16_t) (off & 0xffff);
	  }
	}
	i += 12;  /* 3 texels */
	ఆపు;
      }
      స్థితి HB_GPU_PAINT_OP_నెట్టు_సమూహం:
      స్థితి HB_GPU_PAINT_OP_తీయి_సమూహం:
	i += 4;  /* 1 texel */
	ఆపు;
      అప్రమేయ:
	hb_blob_t::recycle_abort (buf_raw, paint->recycled_blob);
	తిరిగి శూన్యసూచిక;
    }
  }

  /* Sub-payloads: concat in recorded order. */
  అక్షరం *dst = (అక్షరం *) buf + శీర్షిక_texels * texel_బైట్లు
			+ paint->ops.నిడివి * 2;
  కొరకు (hb_blob_t *b : paint->sub_blobs)
  {
    గుర్తులేని len = hb_blob_get_నిడివి (b);
    hb_memcpy (dst, hb_blob_get_దత్తాంశం (b, శూన్యసూచిక), len);
    dst += len;
  }

  ఒకవేళ (extents)
  {
    extents->x_bearing = paint->ext_min_x;
    extents->y_bearing = paint->ext_max_y;
    extents->వెడల్పు     = hb_saturate_sub (paint->ext_max_x, paint->ext_min_x);
    extents->ఎత్తు    = hb_saturate_sub (paint->ext_min_y, paint->ext_max_y);
  }

  hb_blob_t *recycled = paint->recycled_blob;
  paint->recycled_blob = శూన్యసూచిక;
  తిరిగి hb_blob_t::recycle_finalize (buf_raw, buf_సామర్థ్యం, మొత్తం_బైట్లు,
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
శూన్య
hb_gpu_paint_clear (hb_gpu_paint_t *paint)
{
  /* Use reset() not resize(0): if an earlier push_or_fail() set
   * the vector's error state (common under failing-malloc),
   * resize(0) short-circuits without clearing length.  A
   * subsequent clear would double-destroy the blob pointers left
   * in the array, or keep stale ops across encodes. */
  paint->ops.reset ();
  paint->అంకె_ops = 0;
  కొరకు (hb_blob_t *b : paint->sub_blobs)
    hb_blob_destroy (b);
  paint->sub_blobs.reset ();
  paint->sub_బైట్లు = 0;
  paint->clip_లోతు = 0;
  paint->pending_clip_మార్గం = అబద్ధం;
  paint->unsupported = అబద్ధం;
  paint->recharge_budget ();
  paint->cur_transform = {1, 0, 0, 1, 0, 0};
  paint->transform_దొంతర.reset ();
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
శూన్య
hb_gpu_paint_reset (hb_gpu_paint_t *paint)
{
  paint->x_scale = 0;
  paint->y_scale = 0;
  paint->palette = 0;
  paint->budget = HB_BUDGET_DEFAULT;
  hb_map_destroy (paint->custom_palette);
  paint->custom_palette = శూన్యసూచిక;
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
శూన్య
hb_gpu_paint_recycle_blob (hb_gpu_paint_t *paint,
			   hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&paint->recycled_blob, blob);
}


#అన్తర్భావ "hb-gpu-paint-fragment-glsl.hh"
#అన్తర్భావ "hb-gpu-paint-fragment-msl.hh"
#అన్తర్భావ "hb-gpu-paint-fragment-wgsl.hh"
#అన్తర్భావ "hb-gpu-paint-fragment-hlsl.hh"

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
స్థిరరాశి అక్షరం *
hb_gpu_paint_shader_ఆధారం (hb_gpu_shader_stage_t stage,
			    hb_gpu_shader_lang_t  lang)
{
  ఎంపిక (stage) {
  స్థితి HB_GPU_SHADER_STAGE_FRAGMENT:
    ఎంపిక (lang) {
    స్థితి HB_GPU_SHADER_LANG_GLSL: తిరిగి hb_gpu_paint_fragment_glsl;
    స్థితి HB_GPU_SHADER_LANG_MSL:  తిరిగి hb_gpu_paint_fragment_msl;
    స్థితి HB_GPU_SHADER_LANG_WGSL: తిరిగి hb_gpu_paint_fragment_wgsl;
    స్థితి HB_GPU_SHADER_LANG_HLSL: తిరిగి hb_gpu_paint_fragment_hlsl;
    స్థితి HB_GPU_SHADER_LANG_INVALID:
    అప్రమేయ: తిరిగి శూన్యసూచిక;
    }
  స్థితి HB_GPU_SHADER_STAGE_శీర్షం:
    ఎంపిక (lang) {
    స్థితి HB_GPU_SHADER_LANG_GLSL:
    స్థితి HB_GPU_SHADER_LANG_MSL:
    స్థితి HB_GPU_SHADER_LANG_WGSL:
    స్థితి HB_GPU_SHADER_LANG_HLSL: తిరిగి "";
    స్థితి HB_GPU_SHADER_LANG_INVALID:
    అప్రమేయ: తిరిగి శూన్యసూచిక;
    }
  అప్రమేయ:
    తిరిగి శూన్యసూచిక;
  }
}
