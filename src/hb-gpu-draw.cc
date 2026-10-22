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
#అన్తర్భావ "hb-gpu-draw.hh"
#అన్తర్భావ "hb-gpu-cu2qu.hh"
#అన్తర్భావ "hb-machinery.hh"

#అన్తర్భావ <cmath>

/* ---- Accumulator ---- */

స్థిర శూన్య
acc_update_extents (hb_gpu_draw_t *g, ద్విగుణ x, ద్విగుణ y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

స్థిర శూన్య
acc_emit (hb_gpu_draw_t *g,
	  తార్కిక contour_ప్రారంభం,
	  ద్విగుణ p1x, ద్విగుణ p1y,
	  ద్విగుణ p2x, ద్విగుణ p2y,
	  ద్విగుణ p3x, ద్విగుణ p3y)
{
  int64_t *budget = g->get_budget_మిగిలిన ();
  ఒకవేళ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    తిరిగి;

  ఒకవేళ (unlikely (g->అంకె_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = అబద్ధం;
    తిరిగి;
  }

  hb_gpu_వక్రం_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_ప్రారంభం};
  ఒకవేళ (unlikely (!g->curves.నెట్టు_or_fail (c)))
  {
    g->success = అబద్ధం;
    తిరిగి;
  }

  g->అంకె_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

స్థిర శూన్య
acc_emit_conic (hb_gpu_draw_t *g,
		ద్విగుణ cx, ద్విగుణ cy,
		ద్విగుణ x, ద్విగుణ y)
{
  ఒకవేళ (g->current_x == x && g->current_y == y)
    తిరిగి;

  తార్కిక contour_ప్రారంభం = g->need_moveto;
  ఒకవేళ (g->need_moveto) {
    g->ప్రారంభం_x = g->current_x;
    g->ప్రారంభం_y = g->current_y;
    g->need_moveto = అబద్ధం;
  }

  acc_emit (g, contour_ప్రారంభం,
	    g->current_x, g->current_y, cx, cy, x, y);
}

శూన్య
hb_gpu_draw_t::acc_move_గమ్యం (ద్విగుణ x, ద్విగుణ y)
{
  ఒకవేళ (unlikely (!success))
    తిరిగి;

  need_moveto = నిజం;
  current_x = x;
  current_y = y;
}

శూన్య
hb_gpu_draw_t::acc_పంక్తి_గమ్యం (ద్విగుణ x, ద్విగుణ y)
{
  ఒకవేళ (unlikely (!success))
    తిరిగి;

  acc_emit_conic (ఇది, current_x, current_y, x, y);
}

శూన్య
hb_gpu_draw_t::acc_conic_గమ్యం (ద్విగుణ cx, ద్విగుణ cy, ద్విగుణ x, ద్విగుణ y)
{
  ఒకవేళ (unlikely (!success))
    తిరిగి;

  acc_emit_conic (ఇది, cx, cy, x, y);
}

శూన్య
hb_gpu_draw_t::acc_close_మార్గం ()
{
  ఒకవేళ (unlikely (!success))
    తిరిగి;

  ఒకవేళ (!need_moveto &&
      (current_x != ప్రారంభం_x || current_y != ప్రారంభం_y))
    acc_పంక్తి_గమ్యం (ప్రారంభం_x, ప్రారంభం_y);
}


శూన్య
hb_gpu_draw_t::acc_cubic_గమ్యం (ద్విగుణ c1x, ద్విగుణ c1y,
			       ద్విగుణ c2x, ద్విగుణ c2y,
			       ద్విగుణ x, ద్విగుణ y)
{
  ఒకవేళ (unlikely (!success))
    తిరిగి;

  ద్విగుణ c0x = current_x, c0y = current_y;

  ఒకవేళ (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    తిరిగి;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  ద్విగుణ dx = x - c0x, dy = y - c0y;
  ద్విగుణ len_sq = dx * dx + dy * dy;
  ఒకవేళ (len_sq > 0.)
  {
    ద్విగుణ inv = 1.0 / len_sq;
    ద్విగుణ cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    ద్విగుణ cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    ఒకవేళ (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_పంక్తి_గమ్యం (x, y);
      తిరిగి;
    }
  }

  hb_gpu_cubic_గమ్యం_quadratics (ఇది,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

స్థిర శూన్య
hb_gpu_draw_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		     శూన్య *దత్తాంశం,
		     hb_draw_state_t *st HB_వాడని,
		     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) దత్తాంశం;
  g->acc_close_మార్గం ();
  g->acc_move_గమ్యం ((ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_draw_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		     శూన్య *దత్తాంశం,
		     hb_draw_state_t *st HB_వాడని,
		     దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) దత్తాంశం;
  g->acc_పంక్తి_గమ్యం ((ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_draw_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
			  శూన్య *దత్తాంశం,
			  hb_draw_state_t *st HB_వాడని,
			  దశాంశం control_x, దశాంశం control_y,
			  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
			  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) దత్తాంశం;
  g->acc_conic_గమ్యం ((ద్విగుణ) control_x, (ద్విగుణ) control_y,
		   (ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_draw_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
		      శూన్య *దత్తాంశం,
		      hb_draw_state_t *st HB_వాడని,
		      దశాంశం control1_x, దశాంశం control1_y,
		      దశాంశం control2_x, దశాంశం control2_y,
		      దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
		      శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) దత్తాంశం;
  g->acc_cubic_గమ్యం ((ద్విగుణ) control1_x, (ద్విగుణ) control1_y,
		   (ద్విగుణ) control2_x, (ద్విగుణ) control2_y,
		   (ద్విగుణ) గమ్యం_x, (ద్విగుణ) గమ్యం_y);
}

స్థిర శూన్య
hb_gpu_draw_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
			శూన్య *దత్తాంశం,
			hb_draw_state_t *st HB_వాడని,
			శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) దత్తాంశం;
  g->acc_close_మార్గం ();
}

స్థిర hb_bool_t
hb_gpu_draw_సమితి_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం,
			int64_t budget, శూన్య *)
{
  స్వయంచాలక *draw = (hb_gpu_draw_t *) draw_దత్తాంశం;
  draw->budget = budget;
  draw->recharge_budget ();
  తిరిగి నిజం;
}

స్థిర int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_gpu_draw_t *) draw_దత్తాంశం)->budget;
}

స్థిర int64_t *
hb_gpu_draw_get_budget_మిగిలిన (hb_draw_funcs_t *, శూన్య *draw_దత్తాంశం, శూన్య *)
{
  తిరిగి ((hb_gpu_draw_t *) draw_దత్తాంశం)->get_budget_మిగిలిన ();
}

స్థిర అంతఃవరుస శూన్య free_static_gpu_draw_funcs ();

స్థిర నిర్మాణం hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func      (funcs, hb_gpu_draw_move_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func      (funcs, hb_gpu_draw_పంక్తి_గమ్యం,      శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_gpu_draw_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func     (funcs, hb_gpu_draw_cubic_గమ్యం,     శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func   (funcs, hb_gpu_draw_close_మార్గం,   శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_సమితి_budget_func (funcs, hb_gpu_draw_సమితి_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_func (funcs, hb_gpu_draw_get_budget, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_gpu_draw_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    తిరిగి funcs;
  }
} static_gpu_draw_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

నిర్మాణం hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

స్థిర hb_position_t
clamp_గమ్యం_hb_position (ద్విగుణ v)
{
  తిరిగి (hb_position_t) hb_clamp (v,
				   (ద్విగుణ) hb_int_min (hb_position_t),
				   (ద్విగుణ) hb_int_max (hb_position_t));
}

స్థిర అంతఃవరుస int16_t
quantize (ద్విగుణ v)
{
  తిరిగి (int16_t) round (v * HB_GPU_UNITS_చొప్పున_EM);
}

స్థిర అంతఃవరుస int16_t
quantize_కిందికి (ద్విగుణ v)
{
  తిరిగి (int16_t) floor (v * HB_GPU_UNITS_చొప్పున_EM);
}

స్థిర అంతఃవరుస int16_t
quantize_పైకి (ద్విగుణ v)
{
  తిరిగి (int16_t) ceil (v * HB_GPU_UNITS_చొప్పున_EM);
}

స్థిర అంతఃవరుస ద్విగుణ
dequantize (పూర్ణాంకం v)
{
  తిరిగి (ద్విగుణ) v / HB_GPU_UNITS_చొప్పున_EM;
}

స్థిర అంతఃవరుస తార్కిక
quantize_కిందికి_సరిపోతుంది_i16 (ద్విగుణ v)
{
  ద్విగుణ q = floor (v * HB_GPU_UNITS_చొప్పున_EM);
  తిరిగి q >= INT16_MIN &&
	 q <= INT16_MAX;
}

స్థిర అంతఃవరుస తార్కిక
quantize_పైకి_సరిపోతుంది_i16 (ద్విగుణ v)
{
  ద్విగుణ q = ceil (v * HB_GPU_UNITS_చొప్పున_EM);
  తిరిగి q >= INT16_MIN &&
	 q <= INT16_MAX;
}

స్థిర అంతఃవరుస int16_t
encode_స్థానభ్రంశం (గుర్తులేని స్థానభ్రంశం)
{
  తిరిగి (int16_t) (స్థానభ్రంశం - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
స్థిర hb_gpu_encode_వక్రం_info_t
encode_వక్రం_info (స్థిరరాశి hb_gpu_వక్రం_t *c)
{
  hb_gpu_encode_వక్రం_info_t info;

  పూర్ణాంకం p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  పూర్ణాంకం p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

  info.min_x = dequantize (hb_min (hb_min (p1x, p2x), p3x));
  info.max_x = dequantize (hb_max (hb_max (p1x, p2x), p3x));
  info.min_y = dequantize (hb_min (hb_min (p1y, p2y), p3y));
  info.max_y = dequantize (hb_max (hb_max (p1y, p2y), p3y));
  info.is_horizontal = p1y == p2y && p2y == p3y;
  info.is_vertical   = p1x == p2x && p2x == p3x;
  info.hband_lo = 0;
  info.hband_hi = -1;
  info.vband_lo = 0;
  info.vband_hi = -1;

  తిరిగి info;
}

స్థిర శూన్య
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  ఒకవేళ (unlikely (!draw->success) ||
      draw->అంకె_curves == 0 ||
      draw->ext_min_x == HUGE_VAL)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->వెడల్పు = 0;
    extents->ఎత్తు = 0;
    తిరిగి;
  }

  ద్విగుణ min_x = floor (draw->ext_min_x);
  ద్విగుణ min_y = floor (draw->ext_min_y);
  ద్విగుణ max_x = ceil  (draw->ext_max_x);
  ద్విగుణ max_y = ceil  (draw->ext_max_y);

  ఒకవేళ (unlikely (!std::isfinite (min_x) ||
		!std::isfinite (min_y) ||
		!std::isfinite (max_x) ||
		!std::isfinite (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->వెడల్పు = 0;
    extents->ఎత్తు = 0;
    తిరిగి;
  }

  extents->x_bearing = clamp_గమ్యం_hb_position (min_x);
  extents->y_bearing = clamp_గమ్యం_hb_position (max_y);
  extents->వెడల్పు     = clamp_గమ్యం_hb_position (max_x - min_x);
  extents->ఎత్తు    = clamp_గమ్యం_hb_position (min_y - max_y);
}


/**
 * hb_gpu_draw_encode:
 * @draw: a GPU shape encoder
 * @extents: (out) (nullable): where to store the computed glyph
 *           extents (in font units, Y-up).  Pass `NULL` if not
 *           needed.
 *
 * Encodes the accumulated outlines into a compact blob
 * suitable for GPU rendering.  The blob data is an array of
 * RGBA16I texels (8 bytes each) to be uploaded to a texture
 * buffer object.
 *
 * The returned blob owns its own copy of the data.  On success
 * @draw is auto-cleared so it can be reused for the next glyph;
 * user configuration (font scale) is preserved.
 *
 * Return value: (transfer full):
 * An #hb_blob_t containing the encoded data, or `NULL` if encoding
 * failed (allocation failure or accumulation error).  When the
 * encoder accumulated no outline (e.g. the glyph has no ink, like a
 * space), returns the empty-blob singleton instead of `NULL`, so
 * callers can distinguish "nothing to render" (length 0) from a
 * real failure (`NULL`).
 *
 * Since: 14.0.0
 **/
hb_blob_t *
hb_gpu_draw_encode (hb_gpu_draw_t      *draw,
                    hb_glyph_extents_t *extents)
{
  /* Capture computed extents before auto-clear wipes them. */
  ఒకవేళ (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  ఒకవేళ (unlikely (!draw->success))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి hb_gpu_వక్రం_t *curves = draw->curves.arrayZ;
  గుర్తులేని అంకె_curves = draw->curves.నిడివి;

  ఒకవేళ (అంకె_curves == 0)
    తిరిగి hb_blob_get_ఖాళీలు ();

  hb_gpu_encode_scratch_t &s = draw->scratch;

  s.వక్రం_infos.reset_if_error ();
  s.hband_వక్రం_counts.reset_if_error ();
  s.vband_వక్రం_counts.reset_if_error ();
  s.hband_offsets.reset_if_error ();
  s.vband_offsets.reset_if_error ();
  s.hband_curves.reset_if_error ();
  s.hband_curves_asc.reset_if_error ();
  s.vband_curves.reset_if_error ();
  s.vband_curves_asc.reset_if_error ();
  s.hband_cursors.reset_if_error ();
  s.vband_cursors.reset_if_error ();
  s.వక్రం_texel_స్థానభ్రంశం.reset_if_error ();

  /* Compute per-curve info and extents */
  ఒకవేళ (unlikely (!s.వక్రం_infos.resize (అంకె_curves)))
    తిరిగి శూన్యసూచిక;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  ఒకవేళ (!quantize_కిందికి_సరిపోతుంది_i16 (draw->ext_min_x) ||
      !quantize_కిందికి_సరిపోతుంది_i16 (draw->ext_min_y) ||
      !quantize_పైకి_సరిపోతుంది_i16 (draw->ext_max_x) ||
      !quantize_పైకి_సరిపోతుంది_i16 (draw->ext_max_y))
    తిరిగి శూన్యసూచిక;

  int16_t min_x_q = quantize_కిందికి (draw->ext_min_x);
  int16_t min_y_q = quantize_కిందికి (draw->ext_min_y);
  int16_t max_x_q = quantize_పైకి (draw->ext_max_x);
  int16_t max_y_q = quantize_పైకి (draw->ext_max_y);

  ద్విగుణ min_x = dequantize (min_x_q);
  ద్విగుణ min_y = dequantize (min_y_q);
  ద్విగుణ max_x = dequantize (max_x_q);
  ద్విగుణ max_y = dequantize (max_y_q);

  కొరకు (గుర్తులేని i = 0; i < అంకె_curves; i++)
    s.వక్రం_infos.arrayZ[i] = encode_వక్రం_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  గుర్తులేని అంకె_hbands = hb_min (అంకె_curves, 16u);
  గుర్తులేని అంకె_vbands = hb_min (అంకె_curves, 16u);
  అంకె_hbands = hb_max (అంకె_hbands, 1u);
  అంకె_vbands = hb_max (అంకె_vbands, 1u);

  ద్విగుణ ఎత్తు = max_y - min_y;
  ద్విగుణ వెడల్పు  = max_x - min_x;

  ఒకవేళ (ఎత్తు <= 0) అంకె_hbands = 1;
  ఒకవేళ (వెడల్పు  <= 0) అంకె_vbands = 1;

  ద్విగుణ hband_కొలత = ఎత్తు / అంకె_hbands;
  ద్విగుణ vband_కొలత = వెడల్పు  / అంకె_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  స్థిర స్థిరరాశి ద్విగుణ BAND_EPSILON = 1.0 / 1024;

  ఒకవేళ (unlikely (!s.hband_వక్రం_counts.resize (అంకె_hbands) ||
		!s.vband_వక్రం_counts.resize (అంకె_vbands)))
    తిరిగి శూన్యసూచిక;

  కొరకు (గుర్తులేని b = 0; b < అంకె_hbands; b++) s.hband_వక్రం_counts.arrayZ[b] = 0;
  కొరకు (గుర్తులేని b = 0; b < అంకె_vbands; b++) s.vband_వక్రం_counts.arrayZ[b] = 0;

  కొరకు (గుర్తులేని i = 0; i < అంకె_curves; i++)
  {
    hb_gpu_encode_వక్రం_info_t &info = s.వక్రం_infos.arrayZ[i];

    ఒకవేళ (!info.is_horizontal)
    {
      ఒకవేళ (ఎత్తు > 0) {
	info.hband_lo = (పూర్ణాంకం) floor ((info.min_y - min_y) / hband_కొలత - BAND_EPSILON);
	info.hband_hi = (పూర్ణాంకం) floor ((info.max_y - min_y) / hband_కొలత + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (పూర్ణాంకం) అంకె_hbands - 1);
	కొరకు (పూర్ణాంకం b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_వక్రం_counts.arrayZ[b]++;
      } లేకపోతే {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_వక్రం_counts.arrayZ[0]++;
      }
    }

    ఒకవేళ (!info.is_vertical)
    {
      ఒకవేళ (వెడల్పు > 0) {
	info.vband_lo = (పూర్ణాంకం) floor ((info.min_x - min_x) / vband_కొలత - BAND_EPSILON);
	info.vband_hi = (పూర్ణాంకం) floor ((info.max_x - min_x) / vband_కొలత + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (పూర్ణాంకం) అంకె_vbands - 1);
	కొరకు (పూర్ణాంకం b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_వక్రం_counts.arrayZ[b]++;
      } లేకపోతే {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_వక్రం_counts.arrayZ[0]++;
      }
    }
  }

  ఒకవేళ (unlikely (!s.hband_offsets.resize (అంకె_hbands) ||
		!s.vband_offsets.resize (అంకె_vbands)))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని మొత్తం_hband_indices = 0;
  గుర్తులేని మొత్తం_vband_indices = 0;

  కొరకు (గుర్తులేని b = 0; b < అంకె_hbands; b++) {
    s.hband_offsets.arrayZ[b] = మొత్తం_hband_indices;
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (మొత్తం_hband_indices,
					     s.hband_వక్రం_counts.arrayZ[b],
					     &మొత్తం_hband_indices)))
      తిరిగి శూన్యసూచిక;
  }

  కొరకు (గుర్తులేని b = 0; b < అంకె_vbands; b++) {
    s.vband_offsets.arrayZ[b] = మొత్తం_vband_indices;
    ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (మొత్తం_vband_indices,
					     s.vband_వక్రం_counts.arrayZ[b],
					     &మొత్తం_vband_indices)))
      తిరిగి శూన్యసూచిక;
  }

  /* Assign curves to bands */
  ఒకవేళ (unlikely (!s.hband_curves.resize (మొత్తం_hband_indices) ||
		!s.hband_curves_asc.resize (మొత్తం_hband_indices) ||
		!s.vband_curves.resize (మొత్తం_vband_indices) ||
		!s.vband_curves_asc.resize (మొత్తం_vband_indices) ||
		!s.hband_cursors.resize (అంకె_hbands) ||
		!s.vband_cursors.resize (అంకె_vbands)))
    తిరిగి శూన్యసూచిక;

  కొరకు (గుర్తులేని b = 0; b < అంకె_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  కొరకు (గుర్తులేని b = 0; b < అంకె_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  కొరకు (గుర్తులేని i = 0; i < అంకె_curves; i++)
  {
    స్థిరరాశి hb_gpu_encode_వక్రం_info_t &info = s.వక్రం_infos.arrayZ[i];

    కొరకు (పూర్ణాంకం b = info.hband_lo; b <= info.hband_hi; b++) {
      గుర్తులేని idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    కొరకు (పూర్ణాంకం b = info.vband_lo; b <= info.vband_hi; b++) {
      గుర్తులేని idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  స్థిరరాశి hb_gpu_encode_వక్రం_info_t *infos = s.వక్రం_infos.arrayZ;

  కొరకు (గుర్తులేని b = 0; b < అంకె_hbands; b++)
  {
    గుర్తులేని off = s.hband_offsets.arrayZ[b];
    గుర్తులేని లెక్క = s.hband_వక్రం_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, లెక్క)
      .qsort ([infos] (స్థిరరాశి గుర్తులేని &a, స్థిరరాశి గుర్తులేని &b) {
	స్వయంచాలక av = infos[a].max_x, bv = infos[b].max_x;
	తిరిగి (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, లెక్క)
      .qsort ([infos] (స్థిరరాశి గుర్తులేని &a, స్థిరరాశి గుర్తులేని &b) {
	స్వయంచాలక av = infos[a].min_x, bv = infos[b].min_x;
	తిరిగి (av > bv) - (av < bv);
      });
  }

  కొరకు (గుర్తులేని b = 0; b < అంకె_vbands; b++)
  {
    గుర్తులేని off = s.vband_offsets.arrayZ[b];
    గుర్తులేని లెక్క = s.vband_వక్రం_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, లెక్క)
      .qsort ([infos] (స్థిరరాశి గుర్తులేని &a, స్థిరరాశి గుర్తులేని &b) {
	స్వయంచాలక av = infos[a].max_y, bv = infos[b].max_y;
	తిరిగి (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, లెక్క)
      .qsort ([infos] (స్థిరరాశి గుర్తులేని &a, స్థిరరాశి గుర్తులేని &b) {
	స్వయంచాలక av = infos[a].min_y, bv = infos[b].min_y;
	తిరిగి (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  గుర్తులేని మొత్తం_వక్రం_indices;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (మొత్తం_hband_indices,
					   మొత్తం_vband_indices,
					   &మొత్తం_వక్రం_indices) ||
		hb_unsigned_mul_overflows (మొత్తం_వక్రం_indices,
					   2,
					   &మొత్తం_వక్రం_indices)))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని శీర్షిక_len = 2;

  గుర్తులేని అంకె_contour_breaks = 0;
  కొరకు (గుర్తులేని i = 0; i + 1 < అంకె_curves; i++)
    ఒకవేళ (curves[i + 1].contour_ప్రారంభం)
      అంకె_contour_breaks++;

  గుర్తులేని వక్రం_దత్తాంశం_len;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (అంకె_curves,
					   అంకె_contour_breaks,
					   &వక్రం_దత్తాంశం_len) ||
		hb_unsigned_కూడు_overflows (వక్రం_దత్తాంశం_len,
					   1,
					   &వక్రం_దత్తాంశం_len)))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని band_శీర్షికలు_len;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (అంకె_hbands,
					   అంకె_vbands,
					   &band_శీర్షికలు_len)))
    తిరిగి శూన్యసూచిక;

  గుర్తులేని మొత్తం_len = శీర్షిక_len;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (మొత్తం_len,
					   band_శీర్షికలు_len,
					   &మొత్తం_len) ||
		hb_unsigned_కూడు_overflows (మొత్తం_len,
					   మొత్తం_వక్రం_indices,
					   &మొత్తం_len) ||
		hb_unsigned_కూడు_overflows (మొత్తం_len,
					   వక్రం_దత్తాంశం_len,
					   &మొత్తం_len)))
    తిరిగి శూన్యసూచిక;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  ఒకవేళ (మొత్తం_len > (గుర్తులేని) UINT16_MAX + 1u)
    తిరిగి శూన్యసూచిక;

  /* Allocate or reuse encode buffer */
  గుర్తులేని needed_బైట్లు;
  ఒకవేళ (unlikely (hb_unsigned_mul_overflows (మొత్తం_len,
					   పరిమాణం (hb_gpu_texel_t),
					   &needed_బైట్లు)))
    తిరిగి శూన్యసూచిక;
  గుర్తులేని buf_సామర్థ్యం = 0;
  అక్షరం *replaced_recycled_buf = శూన్యసూచిక;
  అక్షరం *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_బైట్లు,
					&buf_సామర్థ్యం, &replaced_recycled_buf);
  ఒకవేళ (unlikely (!buf_raw))
    తిరిగి శూన్యసూచిక;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (శూన్య *) buf_raw;

  గుర్తులేని వక్రం_దత్తాంశం_స్థానభ్రంశం = శీర్షిక_len;
  ఒకవేళ (unlikely (hb_unsigned_కూడు_overflows (వక్రం_దత్తాంశం_స్థానభ్రంశం,
					   band_శీర్షికలు_len,
					   &వక్రం_దత్తాంశం_స్థానభ్రంశం) ||
		hb_unsigned_కూడు_overflows (వక్రం_దత్తాంశం_స్థానభ్రంశం,
					   మొత్తం_వక్రం_indices,
					   &వక్రం_దత్తాంశం_స్థానభ్రంశం)))
  {
    hb_blob_t::recycle_abort ((అక్షరం *) buf, draw->recycled_blob);
    తిరిగి శూన్యసూచిక;
  }

  /* Pack header */
  buf[0].r = min_x_q;
  buf[0].g = min_y_q;
  buf[0].b = max_x_q;
  buf[0].a = max_y_q;
  buf[1].r = (int16_t) అంకె_hbands;
  buf[1].g = (int16_t) అంకె_vbands;
  buf[1].b = (int16_t) hb_clamp (draw->x_scale, -32768, 32767);
  buf[1].a = (int16_t) hb_clamp (draw->y_scale, -32768, 32767);

  /* Pack curve data with shared endpoints */
  ఒకవేళ (unlikely (!s.వక్రం_texel_స్థానభ్రంశం.resize (అంకె_curves)))
  {
    hb_blob_t::recycle_abort ((అక్షరం *) buf, draw->recycled_blob);
    తిరిగి శూన్యసూచిక;
  }

  గుర్తులేని texel = వక్రం_దత్తాంశం_స్థానభ్రంశం;

  కొరకు (గుర్తులేని i = 0; i < అంకె_curves; i++)
  {
    తార్కిక contour_ప్రారంభం = curves[i].contour_ప్రారంభం;

    ఒకవేళ (contour_ప్రారంభం) {
      s.వక్రం_texel_స్థానభ్రంశం.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } లేకపోతే {
      s.వక్రం_texel_స్థానభ్రంశం.arrayZ[i] = texel - 1;
    }

    తార్కిక has_తదుపరి = i + 1 < అంకె_curves &&
		    !curves[i + 1].contour_ప్రారంభం;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    ఒకవేళ (has_తదుపరి) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } లేకపోతే {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  గుర్తులేని సూచీ_స్థానభ్రంశం = శీర్షిక_len + band_శీర్షికలు_len;

  కొరకు (గుర్తులేని b = 0; b < అంకె_hbands; b++)
  {
    int16_t hband_విభజించు;
    {
      గుర్తులేని off = s.hband_offsets.arrayZ[b];
      గుర్తులేని n = s.hband_వక్రం_counts.arrayZ[b];
      గుర్తులేని ఉత్తమ_worst = n;
      ద్విగుణ ఉత్తమ_విభజించు = (min_x + max_x) * 0.5;
      గుర్తులేని ఎడమ_లెక్క = n;
      కొరకు (గుర్తులేని ci = 0; ci < n; ci++) {
	ద్విగుణ విభజించు = s.వక్రం_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	గుర్తులేని కుడి_లెక్క = ci + 1;
	అయితే (ఎడమ_లెక్క &&
	       s.వక్రం_infos.arrayZ[s.hband_curves_asc.arrayZ[off + ఎడమ_లెక్క - 1]].min_x > విభజించు)
	  ఎడమ_లెక్క--;
	గుర్తులేని worst = hb_max (కుడి_లెక్క, ఎడమ_లెక్క);
	ఒకవేళ (worst < ఉత్తమ_worst) {
	  ఉత్తమ_worst = worst;
	  ఉత్తమ_విభజించు = విభజించు;
	}
      }
      hband_విభజించు = quantize (ఉత్తమ_విభజించు);
    }
    గుర్తులేని hdr = శీర్షిక_len + b;
    గుర్తులేని desc_off = సూచీ_స్థానభ్రంశం;

    కొరకు (గుర్తులేని ci = 0; ci < s.hband_వక్రం_counts.arrayZ[b]; ci++) {
      buf[సూచీ_స్థానభ్రంశం].r = encode_స్థానభ్రంశం (s.వక్రం_texel_స్థానభ్రంశం.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[సూచీ_స్థానభ్రంశం].g = 0;
      buf[సూచీ_స్థానభ్రంశం].b = 0;
      buf[సూచీ_స్థానభ్రంశం].a = 0;
      సూచీ_స్థానభ్రంశం++;
    }

    గుర్తులేని asc_off = సూచీ_స్థానభ్రంశం;

    కొరకు (గుర్తులేని ci = 0; ci < s.hband_వక్రం_counts.arrayZ[b]; ci++) {
      buf[సూచీ_స్థానభ్రంశం].r = encode_స్థానభ్రంశం (s.వక్రం_texel_స్థానభ్రంశం.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[సూచీ_స్థానభ్రంశం].g = 0;
      buf[సూచీ_స్థానభ్రంశం].b = 0;
      buf[సూచీ_స్థానభ్రంశం].a = 0;
      సూచీ_స్థానభ్రంశం++;
    }

    buf[hdr].r = (int16_t) s.hband_వక్రం_counts.arrayZ[b];
    buf[hdr].g = encode_స్థానభ్రంశం (desc_off);
    buf[hdr].b = encode_స్థానభ్రంశం (asc_off);
    buf[hdr].a = hband_విభజించు;
  }

  కొరకు (గుర్తులేని b = 0; b < అంకె_vbands; b++)
  {
    int16_t vband_విభజించు;
    {
      గుర్తులేని off = s.vband_offsets.arrayZ[b];
      గుర్తులేని n = s.vband_వక్రం_counts.arrayZ[b];
      గుర్తులేని ఉత్తమ_worst = n;
      ద్విగుణ ఉత్తమ_విభజించు = (min_y + max_y) * 0.5;
      గుర్తులేని ఎడమ_లెక్క = n;
      కొరకు (గుర్తులేని ci = 0; ci < n; ci++) {
	ద్విగుణ విభజించు = s.వక్రం_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	గుర్తులేని కుడి_లెక్క = ci + 1;
	అయితే (ఎడమ_లెక్క &&
	       s.వక్రం_infos.arrayZ[s.vband_curves_asc.arrayZ[off + ఎడమ_లెక్క - 1]].min_y > విభజించు)
	  ఎడమ_లెక్క--;
	గుర్తులేని worst = hb_max (కుడి_లెక్క, ఎడమ_లెక్క);
	ఒకవేళ (worst < ఉత్తమ_worst) {
	  ఉత్తమ_worst = worst;
	  ఉత్తమ_విభజించు = విభజించు;
	}
      }
      vband_విభజించు = quantize (ఉత్తమ_విభజించు);
    }

    గుర్తులేని hdr = శీర్షిక_len + అంకె_hbands + b;
    గుర్తులేని desc_off = సూచీ_స్థానభ్రంశం;

    కొరకు (గుర్తులేని ci = 0; ci < s.vband_వక్రం_counts.arrayZ[b]; ci++) {
      buf[సూచీ_స్థానభ్రంశం].r = encode_స్థానభ్రంశం (s.వక్రం_texel_స్థానభ్రంశం.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[సూచీ_స్థానభ్రంశం].g = 0;
      buf[సూచీ_స్థానభ్రంశం].b = 0;
      buf[సూచీ_స్థానభ్రంశం].a = 0;
      సూచీ_స్థానభ్రంశం++;
    }

    గుర్తులేని asc_off = సూచీ_స్థానభ్రంశం;

    కొరకు (గుర్తులేని ci = 0; ci < s.vband_వక్రం_counts.arrayZ[b]; ci++) {
      buf[సూచీ_స్థానభ్రంశం].r = encode_స్థానభ్రంశం (s.వక్రం_texel_స్థానభ్రంశం.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[సూచీ_స్థానభ్రంశం].g = 0;
      buf[సూచీ_స్థానభ్రంశం].b = 0;
      buf[సూచీ_స్థానభ్రంశం].a = 0;
      సూచీ_స్థానభ్రంశం++;
    }

    buf[hdr].r = (int16_t) s.vband_వక్రం_counts.arrayZ[b];
    buf[hdr].g = encode_స్థానభ్రంశం (desc_off);
    buf[hdr].b = encode_స్థానభ్రంశం (asc_off);
    buf[hdr].a = vband_విభజించు;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = శూన్యసూచిక;
  తిరిగి hb_blob_t::recycle_finalize ((అక్షరం *) buf, buf_సామర్థ్యం, needed_బైట్లు,
				recycled, replaced_recycled_buf);
}


/* ---- Public API ---- */

/**
 * hb_gpu_draw_create_or_fail:
 *
 * Creates a new GPU shape encoder.
 *
 * Return value: (transfer full):
 * A newly allocated #hb_gpu_draw_t, or `NULL` on allocation failure.
 *
 * Since: 14.0.0
 **/
hb_gpu_draw_t *
hb_gpu_draw_create_or_fail (శూన్య)
{
  తిరిగి hb_object_create<hb_gpu_draw_t> ();
}

/**
 * hb_gpu_draw_reference: (skip)
 * @draw: a GPU shape encoder
 *
 * Increases the reference count on @draw by one.
 *
 * Return value: (transfer full):
 * The referenced #hb_gpu_draw_t.
 *
 * Since: 14.0.0
 **/
hb_gpu_draw_t *
hb_gpu_draw_reference (hb_gpu_draw_t *draw)
{
  తిరిగి hb_object_reference (draw);
}

/**
 * hb_gpu_draw_destroy: (skip)
 * @draw: a GPU shape encoder
 *
 * Decreases the reference count on @draw by one. When the
 * reference count reaches zero, the encoder is freed.
 *
 * Since: 14.0.0
 **/
శూన్య
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  ఒకవేళ (!hb_object_should_destroy (draw))
    తిరిగి;

  hb_blob_destroy (draw->recycled_blob);

  hb_object_actually_destroy (draw);
  hb_free (draw);
}

/**
 * hb_gpu_draw_set_user_data: (skip)
 * @draw: a GPU shape encoder
 * @key: the user-data key
 * @data: a pointer to the user data
 * @destroy: (nullable): a callback to call when @data is not needed anymore
 * @replace: whether to replace an existing data with the same key
 *
 * Attaches user data to @draw.
 *
 * Return value: `true` if success, `false` otherwise
 *
 * Since: 14.0.0
 **/
hb_bool_t
hb_gpu_draw_సమితి_user_దత్తాంశం (hb_gpu_draw_t     *draw,
			     hb_user_దత్తాంశం_కీలకం_t *key,
			     శూన్య               *దత్తాంశం,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  తిరిగి hb_object_సమితి_user_దత్తాంశం (draw, key, దత్తాంశం, destroy, replace);
}

/**
 * hb_gpu_draw_get_user_data: (skip)
 * @draw: a GPU shape encoder
 * @key: the user-data key
 *
 * Fetches the user-data associated with the specified key.
 *
 * Return value: (transfer none):
 * A pointer to the user data
 *
 * Since: 14.0.0
 **/
శూన్య *
hb_gpu_draw_get_user_దత్తాంశం (స్థిరరాశి hb_gpu_draw_t     *draw,
			     hb_user_దత్తాంశం_కీలకం_t *key)
{
  తిరిగి hb_object_get_user_దత్తాంశం (draw, key);
}

/**
 * hb_gpu_draw_get_funcs:
 * @draw: a GPU draw context.
 *
 * Fetches the #hb_draw_funcs_t that feeds outline data into
 * @draw.  Pass @draw as the @draw_data argument when calling
 * the draw functions.
 *
 * Return value: (transfer none):
 * The GPU draw functions
 *
 * Since: 14.2.0
 **/
hb_draw_funcs_t *
hb_gpu_draw_get_funcs (స్థిరరాశి hb_gpu_draw_t *draw HB_వాడని)
{
  తిరిగి static_gpu_draw_funcs.get_unconst ();
}

/**
 * hb_gpu_draw_set_scale:
 * @draw: a GPU shape encoder
 * @x_scale: horizontal scale (typically from hb_font_get_scale())
 * @y_scale: vertical scale
 *
 * Sets the font scale so the encoded blob can embed it for
 * shader use (e.g. computing pixels-per-em).  Called
 * automatically by hb_gpu_draw_glyph().
 *
 * Since: 14.1.0
 **/
శూన్య
hb_gpu_draw_సమితి_scale (hb_gpu_draw_t *draw,
		       పూర్ణాంకం            x_scale,
		       పూర్ణాంకం            y_scale)
{
  draw->x_scale = x_scale;
  draw->y_scale = y_scale;
}

/**
 * hb_gpu_draw_get_scale:
 * @draw: a GPU shape encoder
 * @x_scale: (out): horizontal scale
 * @y_scale: (out): vertical scale
 *
 * Gets the font scale previously set via hb_gpu_draw_set_scale() or
 * hb_gpu_draw_glyph().
 *
 * Since: 14.2.0
 **/
శూన్య
hb_gpu_draw_get_scale (స్థిరరాశి hb_gpu_draw_t *draw,
		       పూర్ణాంకం                 *x_scale,
		       పూర్ణాంకం                 *y_scale)
{
  ఒకవేళ (x_scale) *x_scale = draw->x_scale;
  ఒకవేళ (y_scale) *y_scale = draw->y_scale;
}

/**
 * hb_gpu_draw_glyph_or_fail:
 * @draw: a GPU shape encoder
 * @font: font to draw from
 * @glyph: glyph ID to draw
 *
 * Convenience to draw one glyph outline.  Equivalent to:
 *
 * |[<!-- language="plain" -->
 * hb_gpu_draw_set_scale (draw, x_scale, y_scale);
 * hb_font_draw_glyph_or_fail (font, glyph,
 *   hb_gpu_draw_get_funcs (draw), draw);
 * ]|
 *
 * Return value: `true` if the glyph was drawn, `false` if the font
 * has no outlines for @glyph.
 *
 * Since: 14.2.0
 **/
hb_bool_t
hb_gpu_draw_glyph_or_fail (hb_gpu_draw_t  *draw,
			   hb_font_t      *font,
			   hb_codepoint_t  glyph)
{
  పూర్ణాంకం x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_సమితి_scale (draw, x_scale, y_scale);

  తిరిగి hb_font_draw_glyph_or_fail (font, glyph,
				     hb_gpu_draw_get_funcs (draw),
				     draw);
}

/**
 * hb_gpu_draw_glyph:
 * @draw: a GPU shape encoder
 * @font: font to draw from
 * @glyph: glyph ID to draw
 *
 * Draws a single glyph outline into the encoder.  Equivalent to
 * hb_gpu_draw_glyph_or_fail() with the return value ignored.
 *
 * Since: 14.0.0
 **/
శూన్య
hb_gpu_draw_glyph (hb_gpu_draw_t  *draw,
		   hb_font_t      *font,
		   hb_codepoint_t  glyph)
{
  hb_gpu_draw_glyph_or_fail (draw, font, glyph);
}

/**
 * hb_gpu_draw_clear:
 * @draw: a GPU shape encoder
 *
 * Discards accumulated outlines so @draw can be reused for another
 * encode.  User configuration (font scale) is preserved.  Call
 * hb_gpu_draw_reset() to also reset user configuration to defaults.
 *
 * Since: 14.2.0
 **/
శూన్య
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->ప్రారంభం_x = draw->ప్రారంభం_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = నిజం;
  draw->అంకె_curves = 0;
  draw->success = నిజం;
  draw->curves.reset ();

  draw->ext_min_x =  HUGE_VAL;
  draw->ext_min_y =  HUGE_VAL;
  draw->ext_max_x = -HUGE_VAL;
  draw->ext_max_y = -HUGE_VAL;

  draw->recharge_budget ();
}

/**
 * hb_gpu_draw_reset:
 * @draw: a GPU shape encoder
 *
 * Resets the encoder, discarding all accumulated outlines and
 * resetting user configuration to defaults.
 *
 * Since: 14.0.0
 **/
శూన్య
hb_gpu_draw_reset (hb_gpu_draw_t *draw)
{
  draw->x_scale = 0;
  draw->y_scale = 0;
  draw->budget = HB_BUDGET_DEFAULT;
  hb_gpu_draw_clear (draw);
}

/**
 * hb_gpu_draw_recycle_blob:
 * @draw: a GPU shape encoder
 * @blob: (transfer full): a blob previously returned by hb_gpu_draw_encode()
 *
 * Returns a blob to the encoder for potential reuse.
 * The caller transfers ownership of @blob.
 *
 * Currently this simply destroys the blob.  A future version
 * may reclaim the underlying buffer to avoid allocation on the
 * next hb_gpu_draw_encode() call.
 *
 * Since: 14.0.0
 **/
శూన్య
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#అన్తర్భావ "hb-gpu-draw-fragment-glsl.hh"
#అన్తర్భావ "hb-gpu-draw-fragment-msl.hh"
#అన్తర్భావ "hb-gpu-draw-fragment-wgsl.hh"
#అన్తర్భావ "hb-gpu-draw-fragment-hlsl.hh"

/**
 * hb_gpu_draw_shader_source:
 * @stage: pipeline stage (vertex or fragment)
 * @lang: shader language variant
 *
 * Returns the draw-renderer-specific shader source for the
 * specified stage and language.  The returned string is static
 * and must not be freed.
 *
 * This source assumes the shared helpers returned by
 * hb_gpu_shader_source() are concatenated ahead of it.  The
 * caller should assemble the full shader as
 * `#version`-directive + hb_gpu_shader_source() +
 * hb_gpu_draw_shader_source() + caller's `main()`.
 *
 * The vertex stage currently has no draw-specific helpers; this
 * function returns an empty string for that stage so the caller
 * can concatenate unconditionally.
 *
 * Return value: (transfer none):
 * A shader source string, or `NULL` if @stage or @lang is
 * unsupported.
 *
 * Since: 14.2.0
 **/
స్థిరరాశి అక్షరం *
hb_gpu_draw_shader_ఆధారం (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  ఎంపిక (stage) {
  స్థితి HB_GPU_SHADER_STAGE_FRAGMENT:
    ఎంపిక (lang) {
    స్థితి HB_GPU_SHADER_LANG_GLSL: తిరిగి hb_gpu_draw_fragment_glsl;
    స్థితి HB_GPU_SHADER_LANG_MSL:  తిరిగి hb_gpu_draw_fragment_msl;
    స్థితి HB_GPU_SHADER_LANG_WGSL: తిరిగి hb_gpu_draw_fragment_wgsl;
    స్థితి HB_GPU_SHADER_LANG_HLSL: తిరిగి hb_gpu_draw_fragment_hlsl;
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
