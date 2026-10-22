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
#شامل "hb-gpu-draw.hh"
#شامل "hb-gpu-cu2qu.hh"
#شامل "hb-machinery.hh"

#شامل <cmath>

/* ---- Accumulator ---- */

ساکن خالی
acc_update_extents (hb_gpu_draw_t *g, دوہرا x, دوہرا y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

ساکن خالی
acc_emit (hb_gpu_draw_t *g,
	  منطقی contour_آغاز,
	  دوہرا p1x, دوہرا p1y,
	  دوہرا p2x, دوہرا p2y,
	  دوہرا p3x, دوہرا p3y)
{
  int64_t *budget = g->get_budget_بقایا ();
  اگر (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    واپس;

  اگر (unlikely (g->عدد_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = جھوٹ;
    واپس;
  }

  hb_gpu_خم_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_آغاز};
  اگر (unlikely (!g->curves.ڈالو_or_fail (c)))
  {
    g->success = جھوٹ;
    واپس;
  }

  g->عدد_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

ساکن خالی
acc_emit_conic (hb_gpu_draw_t *g,
		دوہرا cx, دوہرا cy,
		دوہرا x, دوہرا y)
{
  اگر (g->current_x == x && g->current_y == y)
    واپس;

  منطقی contour_آغاز = g->need_moveto;
  اگر (g->need_moveto) {
    g->آغاز_x = g->current_x;
    g->آغاز_y = g->current_y;
    g->need_moveto = جھوٹ;
  }

  acc_emit (g, contour_آغاز,
	    g->current_x, g->current_y, cx, cy, x, y);
}

خالی
hb_gpu_draw_t::acc_move_مقصد (دوہرا x, دوہرا y)
{
  اگر (unlikely (!success))
    واپس;

  need_moveto = سچ;
  current_x = x;
  current_y = y;
}

خالی
hb_gpu_draw_t::acc_سطر_مقصد (دوہرا x, دوہرا y)
{
  اگر (unlikely (!success))
    واپس;

  acc_emit_conic (یہ, current_x, current_y, x, y);
}

خالی
hb_gpu_draw_t::acc_conic_مقصد (دوہرا cx, دوہرا cy, دوہرا x, دوہرا y)
{
  اگر (unlikely (!success))
    واپس;

  acc_emit_conic (یہ, cx, cy, x, y);
}

خالی
hb_gpu_draw_t::acc_close_راستہ ()
{
  اگر (unlikely (!success))
    واپس;

  اگر (!need_moveto &&
      (current_x != آغاز_x || current_y != آغاز_y))
    acc_سطر_مقصد (آغاز_x, آغاز_y);
}


خالی
hb_gpu_draw_t::acc_cubic_مقصد (دوہرا c1x, دوہرا c1y,
			       دوہرا c2x, دوہرا c2y,
			       دوہرا x, دوہرا y)
{
  اگر (unlikely (!success))
    واپس;

  دوہرا c0x = current_x, c0y = current_y;

  اگر (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    واپس;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  دوہرا dx = x - c0x, dy = y - c0y;
  دوہرا len_sq = dx * dx + dy * dy;
  اگر (len_sq > 0.)
  {
    دوہرا inv = 1.0 / len_sq;
    دوہرا cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    دوہرا cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    اگر (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_سطر_مقصد (x, y);
      واپس;
    }
  }

  hb_gpu_cubic_مقصد_quadratics (یہ,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

ساکن خالی
hb_gpu_draw_move_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		     خالی *ڈیٹا,
		     hb_draw_state_t *st HB_غیر_مستعمل,
		     اعشاری مقصد_x, اعشاری مقصد_y,
		     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ڈیٹا;
  g->acc_close_راستہ ();
  g->acc_move_مقصد ((دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_gpu_draw_سطر_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		     خالی *ڈیٹا,
		     hb_draw_state_t *st HB_غیر_مستعمل,
		     اعشاری مقصد_x, اعشاری مقصد_y,
		     خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ڈیٹا;
  g->acc_سطر_مقصد ((دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_gpu_draw_quadratic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			  خالی *ڈیٹا,
			  hb_draw_state_t *st HB_غیر_مستعمل,
			  اعشاری control_x, اعشاری control_y,
			  اعشاری مقصد_x, اعشاری مقصد_y,
			  خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ڈیٹا;
  g->acc_conic_مقصد ((دوہرا) control_x, (دوہرا) control_y,
		   (دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_gpu_draw_cubic_مقصد (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
		      خالی *ڈیٹا,
		      hb_draw_state_t *st HB_غیر_مستعمل,
		      اعشاری control1_x, اعشاری control1_y,
		      اعشاری control2_x, اعشاری control2_y,
		      اعشاری مقصد_x, اعشاری مقصد_y,
		      خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ڈیٹا;
  g->acc_cubic_مقصد ((دوہرا) control1_x, (دوہرا) control1_y,
		   (دوہرا) control2_x, (دوہرا) control2_y,
		   (دوہرا) مقصد_x, (دوہرا) مقصد_y);
}

ساکن خالی
hb_gpu_draw_close_راستہ (hb_draw_funcs_t *dfuncs HB_غیر_مستعمل,
			خالی *ڈیٹا,
			hb_draw_state_t *st HB_غیر_مستعمل,
			خالی *user_ڈیٹا HB_غیر_مستعمل)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ڈیٹا;
  g->acc_close_راستہ ();
}

ساکن hb_bool_t
hb_gpu_draw_سیٹ_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا,
			int64_t budget, خالی *)
{
  خودکار *draw = (hb_gpu_draw_t *) draw_ڈیٹا;
  draw->budget = budget;
  draw->recharge_budget ();
  واپس سچ;
}

ساکن int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_gpu_draw_t *) draw_ڈیٹا)->budget;
}

ساکن int64_t *
hb_gpu_draw_get_budget_بقایا (hb_draw_funcs_t *, خالی *draw_ڈیٹا, خالی *)
{
  واپس ((hb_gpu_draw_t *) draw_ڈیٹا)->get_budget_بقایا ();
}

ساکن اندرونی خالی free_static_gpu_draw_funcs ();

ساکن ساخت hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  ساکن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سیٹ_move_مقصد_func      (funcs, hb_gpu_draw_move_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سطر_مقصد_func      (funcs, hb_gpu_draw_سطر_مقصد,      خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_quadratic_مقصد_func (funcs, hb_gpu_draw_quadratic_مقصد, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_cubic_مقصد_func     (funcs, hb_gpu_draw_cubic_مقصد,     خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_close_راستہ_func   (funcs, hb_gpu_draw_close_راستہ,   خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_سیٹ_budget_func (funcs, hb_gpu_draw_سیٹ_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_func (funcs, hb_gpu_draw_get_budget, خالی_اشاریہ, خالی_اشاریہ);
    hb_draw_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_gpu_draw_get_budget_بقایا, خالی_اشاریہ, خالی_اشاریہ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    واپس funcs;
  }
} static_gpu_draw_funcs;

ساکن اندرونی خالی
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

ساخت hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

ساکن hb_position_t
clamp_مقصد_hb_position (دوہرا v)
{
  واپس (hb_position_t) hb_clamp (v,
				   (دوہرا) hb_int_min (hb_position_t),
				   (دوہرا) hb_int_max (hb_position_t));
}

ساکن اندرونی int16_t
quantize (دوہرا v)
{
  واپس (int16_t) round (v * HB_GPU_UNITS_فی_EM);
}

ساکن اندرونی int16_t
quantize_نیچے_کو (دوہرا v)
{
  واپس (int16_t) floor (v * HB_GPU_UNITS_فی_EM);
}

ساکن اندرونی int16_t
quantize_اوپر (دوہرا v)
{
  واپس (int16_t) ceil (v * HB_GPU_UNITS_فی_EM);
}

ساکن اندرونی دوہرا
dequantize (صحیح v)
{
  واپس (دوہرا) v / HB_GPU_UNITS_فی_EM;
}

ساکن اندرونی منطقی
quantize_نیچے_کو_سماتا_i16 (دوہرا v)
{
  دوہرا q = floor (v * HB_GPU_UNITS_فی_EM);
  واپس q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ساکن اندرونی منطقی
quantize_اوپر_سماتا_i16 (دوہرا v)
{
  دوہرا q = ceil (v * HB_GPU_UNITS_فی_EM);
  واپس q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ساکن اندرونی int16_t
encode_انحراف (غیرعلامتی انحراف)
{
  واپس (int16_t) (انحراف - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
ساکن hb_gpu_encode_خم_info_t
encode_خم_info (ثابت hb_gpu_خم_t *c)
{
  hb_gpu_encode_خم_info_t info;

  صحیح p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  صحیح p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

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

  واپس info;
}

ساکن خالی
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  اگر (unlikely (!draw->success) ||
      draw->عدد_curves == 0 ||
      draw->ext_min_x == HUGE_VAL)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->چوڑائی = 0;
    extents->اونچائی = 0;
    واپس;
  }

  دوہرا min_x = floor (draw->ext_min_x);
  دوہرا min_y = floor (draw->ext_min_y);
  دوہرا max_x = ceil  (draw->ext_max_x);
  دوہرا max_y = ceil  (draw->ext_max_y);

  اگر (unlikely (!std::isfinite (min_x) ||
		!std::isfinite (min_y) ||
		!std::isfinite (max_x) ||
		!std::isfinite (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->چوڑائی = 0;
    extents->اونچائی = 0;
    واپس;
  }

  extents->x_bearing = clamp_مقصد_hb_position (min_x);
  extents->y_bearing = clamp_مقصد_hb_position (max_y);
  extents->چوڑائی     = clamp_مقصد_hb_position (max_x - min_x);
  extents->اونچائی    = clamp_مقصد_hb_position (min_y - max_y);
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
  اگر (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  اگر (unlikely (!draw->success))
    واپس خالی_اشاریہ;

  ثابت hb_gpu_خم_t *curves = draw->curves.arrayZ;
  غیرعلامتی عدد_curves = draw->curves.لمبائی;

  اگر (عدد_curves == 0)
    واپس hb_blob_get_خالی_خانے ();

  hb_gpu_encode_scratch_t &s = draw->scratch;

  s.خم_infos.reset_if_error ();
  s.hband_خم_counts.reset_if_error ();
  s.vband_خم_counts.reset_if_error ();
  s.hband_offsets.reset_if_error ();
  s.vband_offsets.reset_if_error ();
  s.hband_curves.reset_if_error ();
  s.hband_curves_asc.reset_if_error ();
  s.vband_curves.reset_if_error ();
  s.vband_curves_asc.reset_if_error ();
  s.hband_cursors.reset_if_error ();
  s.vband_cursors.reset_if_error ();
  s.خم_texel_انحراف.reset_if_error ();

  /* Compute per-curve info and extents */
  اگر (unlikely (!s.خم_infos.resize (عدد_curves)))
    واپس خالی_اشاریہ;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  اگر (!quantize_نیچے_کو_سماتا_i16 (draw->ext_min_x) ||
      !quantize_نیچے_کو_سماتا_i16 (draw->ext_min_y) ||
      !quantize_اوپر_سماتا_i16 (draw->ext_max_x) ||
      !quantize_اوپر_سماتا_i16 (draw->ext_max_y))
    واپس خالی_اشاریہ;

  int16_t min_x_q = quantize_نیچے_کو (draw->ext_min_x);
  int16_t min_y_q = quantize_نیچے_کو (draw->ext_min_y);
  int16_t max_x_q = quantize_اوپر (draw->ext_max_x);
  int16_t max_y_q = quantize_اوپر (draw->ext_max_y);

  دوہرا min_x = dequantize (min_x_q);
  دوہرا min_y = dequantize (min_y_q);
  دوہرا max_x = dequantize (max_x_q);
  دوہرا max_y = dequantize (max_y_q);

  برائے (غیرعلامتی i = 0; i < عدد_curves; i++)
    s.خم_infos.arrayZ[i] = encode_خم_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  غیرعلامتی عدد_hbands = hb_min (عدد_curves, 16u);
  غیرعلامتی عدد_vbands = hb_min (عدد_curves, 16u);
  عدد_hbands = hb_max (عدد_hbands, 1u);
  عدد_vbands = hb_max (عدد_vbands, 1u);

  دوہرا اونچائی = max_y - min_y;
  دوہرا چوڑائی  = max_x - min_x;

  اگر (اونچائی <= 0) عدد_hbands = 1;
  اگر (چوڑائی  <= 0) عدد_vbands = 1;

  دوہرا hband_سائز = اونچائی / عدد_hbands;
  دوہرا vband_سائز = چوڑائی  / عدد_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  ساکن ثابت دوہرا BAND_EPSILON = 1.0 / 1024;

  اگر (unlikely (!s.hband_خم_counts.resize (عدد_hbands) ||
		!s.vband_خم_counts.resize (عدد_vbands)))
    واپس خالی_اشاریہ;

  برائے (غیرعلامتی b = 0; b < عدد_hbands; b++) s.hband_خم_counts.arrayZ[b] = 0;
  برائے (غیرعلامتی b = 0; b < عدد_vbands; b++) s.vband_خم_counts.arrayZ[b] = 0;

  برائے (غیرعلامتی i = 0; i < عدد_curves; i++)
  {
    hb_gpu_encode_خم_info_t &info = s.خم_infos.arrayZ[i];

    اگر (!info.is_horizontal)
    {
      اگر (اونچائی > 0) {
	info.hband_lo = (صحیح) floor ((info.min_y - min_y) / hband_سائز - BAND_EPSILON);
	info.hband_hi = (صحیح) floor ((info.max_y - min_y) / hband_سائز + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (صحیح) عدد_hbands - 1);
	برائے (صحیح b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_خم_counts.arrayZ[b]++;
      } ورنہ {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_خم_counts.arrayZ[0]++;
      }
    }

    اگر (!info.is_vertical)
    {
      اگر (چوڑائی > 0) {
	info.vband_lo = (صحیح) floor ((info.min_x - min_x) / vband_سائز - BAND_EPSILON);
	info.vband_hi = (صحیح) floor ((info.max_x - min_x) / vband_سائز + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (صحیح) عدد_vbands - 1);
	برائے (صحیح b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_خم_counts.arrayZ[b]++;
      } ورنہ {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_خم_counts.arrayZ[0]++;
      }
    }
  }

  اگر (unlikely (!s.hband_offsets.resize (عدد_hbands) ||
		!s.vband_offsets.resize (عدد_vbands)))
    واپس خالی_اشاریہ;

  غیرعلامتی کل_hband_indices = 0;
  غیرعلامتی کل_vband_indices = 0;

  برائے (غیرعلامتی b = 0; b < عدد_hbands; b++) {
    s.hband_offsets.arrayZ[b] = کل_hband_indices;
    اگر (unlikely (hb_unsigned_جوڑو_overflows (کل_hband_indices,
					     s.hband_خم_counts.arrayZ[b],
					     &کل_hband_indices)))
      واپس خالی_اشاریہ;
  }

  برائے (غیرعلامتی b = 0; b < عدد_vbands; b++) {
    s.vband_offsets.arrayZ[b] = کل_vband_indices;
    اگر (unlikely (hb_unsigned_جوڑو_overflows (کل_vband_indices,
					     s.vband_خم_counts.arrayZ[b],
					     &کل_vband_indices)))
      واپس خالی_اشاریہ;
  }

  /* Assign curves to bands */
  اگر (unlikely (!s.hband_curves.resize (کل_hband_indices) ||
		!s.hband_curves_asc.resize (کل_hband_indices) ||
		!s.vband_curves.resize (کل_vband_indices) ||
		!s.vband_curves_asc.resize (کل_vband_indices) ||
		!s.hband_cursors.resize (عدد_hbands) ||
		!s.vband_cursors.resize (عدد_vbands)))
    واپس خالی_اشاریہ;

  برائے (غیرعلامتی b = 0; b < عدد_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  برائے (غیرعلامتی b = 0; b < عدد_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  برائے (غیرعلامتی i = 0; i < عدد_curves; i++)
  {
    ثابت hb_gpu_encode_خم_info_t &info = s.خم_infos.arrayZ[i];

    برائے (صحیح b = info.hband_lo; b <= info.hband_hi; b++) {
      غیرعلامتی idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    برائے (صحیح b = info.vband_lo; b <= info.vband_hi; b++) {
      غیرعلامتی idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  ثابت hb_gpu_encode_خم_info_t *infos = s.خم_infos.arrayZ;

  برائے (غیرعلامتی b = 0; b < عدد_hbands; b++)
  {
    غیرعلامتی off = s.hband_offsets.arrayZ[b];
    غیرعلامتی گنتی = s.hband_خم_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, گنتی)
      .qsort ([infos] (ثابت غیرعلامتی &a, ثابت غیرعلامتی &b) {
	خودکار av = infos[a].max_x, bv = infos[b].max_x;
	واپس (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, گنتی)
      .qsort ([infos] (ثابت غیرعلامتی &a, ثابت غیرعلامتی &b) {
	خودکار av = infos[a].min_x, bv = infos[b].min_x;
	واپس (av > bv) - (av < bv);
      });
  }

  برائے (غیرعلامتی b = 0; b < عدد_vbands; b++)
  {
    غیرعلامتی off = s.vband_offsets.arrayZ[b];
    غیرعلامتی گنتی = s.vband_خم_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, گنتی)
      .qsort ([infos] (ثابت غیرعلامتی &a, ثابت غیرعلامتی &b) {
	خودکار av = infos[a].max_y, bv = infos[b].max_y;
	واپس (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, گنتی)
      .qsort ([infos] (ثابت غیرعلامتی &a, ثابت غیرعلامتی &b) {
	خودکار av = infos[a].min_y, bv = infos[b].min_y;
	واپس (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  غیرعلامتی کل_خم_indices;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (کل_hband_indices,
					   کل_vband_indices,
					   &کل_خم_indices) ||
		hb_unsigned_mul_overflows (کل_خم_indices,
					   2,
					   &کل_خم_indices)))
    واپس خالی_اشاریہ;

  غیرعلامتی سرخی_len = 2;

  غیرعلامتی عدد_contour_breaks = 0;
  برائے (غیرعلامتی i = 0; i + 1 < عدد_curves; i++)
    اگر (curves[i + 1].contour_آغاز)
      عدد_contour_breaks++;

  غیرعلامتی خم_ڈیٹا_len;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (عدد_curves,
					   عدد_contour_breaks,
					   &خم_ڈیٹا_len) ||
		hb_unsigned_جوڑو_overflows (خم_ڈیٹا_len,
					   1,
					   &خم_ڈیٹا_len)))
    واپس خالی_اشاریہ;

  غیرعلامتی band_سرخیاں_len;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (عدد_hbands,
					   عدد_vbands,
					   &band_سرخیاں_len)))
    واپس خالی_اشاریہ;

  غیرعلامتی کل_len = سرخی_len;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (کل_len,
					   band_سرخیاں_len,
					   &کل_len) ||
		hb_unsigned_جوڑو_overflows (کل_len,
					   کل_خم_indices,
					   &کل_len) ||
		hb_unsigned_جوڑو_overflows (کل_len,
					   خم_ڈیٹا_len,
					   &کل_len)))
    واپس خالی_اشاریہ;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  اگر (کل_len > (غیرعلامتی) UINT16_MAX + 1u)
    واپس خالی_اشاریہ;

  /* Allocate or reuse encode buffer */
  غیرعلامتی needed_بائٹیں;
  اگر (unlikely (hb_unsigned_mul_overflows (کل_len,
					   حجم (hb_gpu_texel_t),
					   &needed_بائٹیں)))
    واپس خالی_اشاریہ;
  غیرعلامتی buf_استعداد = 0;
  حرف *replaced_recycled_buf = خالی_اشاریہ;
  حرف *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_بائٹیں,
					&buf_استعداد, &replaced_recycled_buf);
  اگر (unlikely (!buf_raw))
    واپس خالی_اشاریہ;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (خالی *) buf_raw;

  غیرعلامتی خم_ڈیٹا_انحراف = سرخی_len;
  اگر (unlikely (hb_unsigned_جوڑو_overflows (خم_ڈیٹا_انحراف,
					   band_سرخیاں_len,
					   &خم_ڈیٹا_انحراف) ||
		hb_unsigned_جوڑو_overflows (خم_ڈیٹا_انحراف,
					   کل_خم_indices,
					   &خم_ڈیٹا_انحراف)))
  {
    hb_blob_t::recycle_abort ((حرف *) buf, draw->recycled_blob);
    واپس خالی_اشاریہ;
  }

  /* Pack header */
  buf[0].r = min_x_q;
  buf[0].g = min_y_q;
  buf[0].b = max_x_q;
  buf[0].a = max_y_q;
  buf[1].r = (int16_t) عدد_hbands;
  buf[1].g = (int16_t) عدد_vbands;
  buf[1].b = (int16_t) hb_clamp (draw->x_scale, -32768, 32767);
  buf[1].a = (int16_t) hb_clamp (draw->y_scale, -32768, 32767);

  /* Pack curve data with shared endpoints */
  اگر (unlikely (!s.خم_texel_انحراف.resize (عدد_curves)))
  {
    hb_blob_t::recycle_abort ((حرف *) buf, draw->recycled_blob);
    واپس خالی_اشاریہ;
  }

  غیرعلامتی texel = خم_ڈیٹا_انحراف;

  برائے (غیرعلامتی i = 0; i < عدد_curves; i++)
  {
    منطقی contour_آغاز = curves[i].contour_آغاز;

    اگر (contour_آغاز) {
      s.خم_texel_انحراف.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } ورنہ {
      s.خم_texel_انحراف.arrayZ[i] = texel - 1;
    }

    منطقی has_اگلا = i + 1 < عدد_curves &&
		    !curves[i + 1].contour_آغاز;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    اگر (has_اگلا) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } ورنہ {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  غیرعلامتی اشاریہ_انحراف = سرخی_len + band_سرخیاں_len;

  برائے (غیرعلامتی b = 0; b < عدد_hbands; b++)
  {
    int16_t hband_بانٹو;
    {
      غیرعلامتی off = s.hband_offsets.arrayZ[b];
      غیرعلامتی n = s.hband_خم_counts.arrayZ[b];
      غیرعلامتی بہترین_worst = n;
      دوہرا بہترین_بانٹو = (min_x + max_x) * 0.5;
      غیرعلامتی بایاں_گنتی = n;
      برائے (غیرعلامتی ci = 0; ci < n; ci++) {
	دوہرا بانٹو = s.خم_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	غیرعلامتی دایاں_گنتی = ci + 1;
	جب (بایاں_گنتی &&
	       s.خم_infos.arrayZ[s.hband_curves_asc.arrayZ[off + بایاں_گنتی - 1]].min_x > بانٹو)
	  بایاں_گنتی--;
	غیرعلامتی worst = hb_max (دایاں_گنتی, بایاں_گنتی);
	اگر (worst < بہترین_worst) {
	  بہترین_worst = worst;
	  بہترین_بانٹو = بانٹو;
	}
      }
      hband_بانٹو = quantize (بہترین_بانٹو);
    }
    غیرعلامتی hdr = سرخی_len + b;
    غیرعلامتی desc_off = اشاریہ_انحراف;

    برائے (غیرعلامتی ci = 0; ci < s.hband_خم_counts.arrayZ[b]; ci++) {
      buf[اشاریہ_انحراف].r = encode_انحراف (s.خم_texel_انحراف.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[اشاریہ_انحراف].g = 0;
      buf[اشاریہ_انحراف].b = 0;
      buf[اشاریہ_انحراف].a = 0;
      اشاریہ_انحراف++;
    }

    غیرعلامتی asc_off = اشاریہ_انحراف;

    برائے (غیرعلامتی ci = 0; ci < s.hband_خم_counts.arrayZ[b]; ci++) {
      buf[اشاریہ_انحراف].r = encode_انحراف (s.خم_texel_انحراف.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[اشاریہ_انحراف].g = 0;
      buf[اشاریہ_انحراف].b = 0;
      buf[اشاریہ_انحراف].a = 0;
      اشاریہ_انحراف++;
    }

    buf[hdr].r = (int16_t) s.hband_خم_counts.arrayZ[b];
    buf[hdr].g = encode_انحراف (desc_off);
    buf[hdr].b = encode_انحراف (asc_off);
    buf[hdr].a = hband_بانٹو;
  }

  برائے (غیرعلامتی b = 0; b < عدد_vbands; b++)
  {
    int16_t vband_بانٹو;
    {
      غیرعلامتی off = s.vband_offsets.arrayZ[b];
      غیرعلامتی n = s.vband_خم_counts.arrayZ[b];
      غیرعلامتی بہترین_worst = n;
      دوہرا بہترین_بانٹو = (min_y + max_y) * 0.5;
      غیرعلامتی بایاں_گنتی = n;
      برائے (غیرعلامتی ci = 0; ci < n; ci++) {
	دوہرا بانٹو = s.خم_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	غیرعلامتی دایاں_گنتی = ci + 1;
	جب (بایاں_گنتی &&
	       s.خم_infos.arrayZ[s.vband_curves_asc.arrayZ[off + بایاں_گنتی - 1]].min_y > بانٹو)
	  بایاں_گنتی--;
	غیرعلامتی worst = hb_max (دایاں_گنتی, بایاں_گنتی);
	اگر (worst < بہترین_worst) {
	  بہترین_worst = worst;
	  بہترین_بانٹو = بانٹو;
	}
      }
      vband_بانٹو = quantize (بہترین_بانٹو);
    }

    غیرعلامتی hdr = سرخی_len + عدد_hbands + b;
    غیرعلامتی desc_off = اشاریہ_انحراف;

    برائے (غیرعلامتی ci = 0; ci < s.vband_خم_counts.arrayZ[b]; ci++) {
      buf[اشاریہ_انحراف].r = encode_انحراف (s.خم_texel_انحراف.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[اشاریہ_انحراف].g = 0;
      buf[اشاریہ_انحراف].b = 0;
      buf[اشاریہ_انحراف].a = 0;
      اشاریہ_انحراف++;
    }

    غیرعلامتی asc_off = اشاریہ_انحراف;

    برائے (غیرعلامتی ci = 0; ci < s.vband_خم_counts.arrayZ[b]; ci++) {
      buf[اشاریہ_انحراف].r = encode_انحراف (s.خم_texel_انحراف.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[اشاریہ_انحراف].g = 0;
      buf[اشاریہ_انحراف].b = 0;
      buf[اشاریہ_انحراف].a = 0;
      اشاریہ_انحراف++;
    }

    buf[hdr].r = (int16_t) s.vband_خم_counts.arrayZ[b];
    buf[hdr].g = encode_انحراف (desc_off);
    buf[hdr].b = encode_انحراف (asc_off);
    buf[hdr].a = vband_بانٹو;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = خالی_اشاریہ;
  واپس hb_blob_t::recycle_finalize ((حرف *) buf, buf_استعداد, needed_بائٹیں,
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
hb_gpu_draw_create_or_fail (خالی)
{
  واپس hb_object_create<hb_gpu_draw_t> ();
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
  واپس hb_object_reference (draw);
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
خالی
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  اگر (!hb_object_should_destroy (draw))
    واپس;

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
hb_gpu_draw_سیٹ_user_ڈیٹا (hb_gpu_draw_t     *draw,
			     hb_user_ڈیٹا_کلید_t *key,
			     خالی               *ڈیٹا,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  واپس hb_object_سیٹ_user_ڈیٹا (draw, key, ڈیٹا, destroy, replace);
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
خالی *
hb_gpu_draw_get_user_ڈیٹا (ثابت hb_gpu_draw_t     *draw,
			     hb_user_ڈیٹا_کلید_t *key)
{
  واپس hb_object_get_user_ڈیٹا (draw, key);
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
hb_gpu_draw_get_funcs (ثابت hb_gpu_draw_t *draw HB_غیر_مستعمل)
{
  واپس static_gpu_draw_funcs.get_unconst ();
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
خالی
hb_gpu_draw_سیٹ_scale (hb_gpu_draw_t *draw,
		       صحیح            x_scale,
		       صحیح            y_scale)
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
خالی
hb_gpu_draw_get_scale (ثابت hb_gpu_draw_t *draw,
		       صحیح                 *x_scale,
		       صحیح                 *y_scale)
{
  اگر (x_scale) *x_scale = draw->x_scale;
  اگر (y_scale) *y_scale = draw->y_scale;
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
  صحیح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_سیٹ_scale (draw, x_scale, y_scale);

  واپس hb_font_draw_glyph_or_fail (font, glyph,
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
خالی
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
خالی
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->آغاز_x = draw->آغاز_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = سچ;
  draw->عدد_curves = 0;
  draw->success = سچ;
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
خالی
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
خالی
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#شامل "hb-gpu-draw-fragment-glsl.hh"
#شامل "hb-gpu-draw-fragment-msl.hh"
#شامل "hb-gpu-draw-fragment-wgsl.hh"
#شامل "hb-gpu-draw-fragment-hlsl.hh"

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
ثابت حرف *
hb_gpu_draw_shader_ماخذ (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  انتخاب (stage) {
  حالت HB_GPU_SHADER_STAGE_FRAGMENT:
    انتخاب (lang) {
    حالت HB_GPU_SHADER_LANG_GLSL: واپس hb_gpu_draw_fragment_glsl;
    حالت HB_GPU_SHADER_LANG_MSL:  واپس hb_gpu_draw_fragment_msl;
    حالت HB_GPU_SHADER_LANG_WGSL: واپس hb_gpu_draw_fragment_wgsl;
    حالت HB_GPU_SHADER_LANG_HLSL: واپس hb_gpu_draw_fragment_hlsl;
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
