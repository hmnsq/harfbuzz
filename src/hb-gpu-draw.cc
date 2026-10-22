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
#ضمن "hb-gpu-draw.hh"
#ضمن "hb-gpu-cu2qu.hh"
#ضمن "hb-machinery.hh"

#ضمن <cmath>

/* ---- Accumulator ---- */

ساكن فراغ
acc_update_extents (hb_gpu_draw_t *g, مزدوج x, مزدوج y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

ساكن فراغ
acc_emit (hb_gpu_draw_t *g,
	  منطقي contour_پیل,
	  مزدوج p1x, مزدوج p1y,
	  مزدوج p2x, مزدوج p2y,
	  مزدوج p3x, مزدوج p3y)
{
  int64_t *budget = g->get_budget_پاتې_شوی ();
  إذا (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    أرجع;

  إذا (unlikely (g->عدد_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = خطأ;
    أرجع;
  }

  hb_gpu_منحني_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_پیل};
  إذا (unlikely (!g->curves.ټېل_or_fail (c)))
  {
    g->success = خطأ;
    أرجع;
  }

  g->عدد_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

ساكن فراغ
acc_emit_conic (hb_gpu_draw_t *g,
		مزدوج cx, مزدوج cy,
		مزدوج x, مزدوج y)
{
  إذا (g->current_x == x && g->current_y == y)
    أرجع;

  منطقي contour_پیل = g->need_moveto;
  إذا (g->need_moveto) {
    g->پیل_x = g->current_x;
    g->پیل_y = g->current_y;
    g->need_moveto = خطأ;
  }

  acc_emit (g, contour_پیل,
	    g->current_x, g->current_y, cx, cy, x, y);
}

فراغ
hb_gpu_draw_t::acc_move_منزل (مزدوج x, مزدوج y)
{
  إذا (unlikely (!success))
    أرجع;

  need_moveto = ریښتیا;
  current_x = x;
  current_y = y;
}

فراغ
hb_gpu_draw_t::acc_کرښه_منزل (مزدوج x, مزدوج y)
{
  إذا (unlikely (!success))
    أرجع;

  acc_emit_conic (هذا, current_x, current_y, x, y);
}

فراغ
hb_gpu_draw_t::acc_conic_منزل (مزدوج cx, مزدوج cy, مزدوج x, مزدوج y)
{
  إذا (unlikely (!success))
    أرجع;

  acc_emit_conic (هذا, cx, cy, x, y);
}

فراغ
hb_gpu_draw_t::acc_close_لار ()
{
  إذا (unlikely (!success))
    أرجع;

  إذا (!need_moveto &&
      (current_x != پیل_x || current_y != پیل_y))
    acc_کرښه_منزل (پیل_x, پیل_y);
}


فراغ
hb_gpu_draw_t::acc_cubic_منزل (مزدوج c1x, مزدوج c1y,
			       مزدوج c2x, مزدوج c2y,
			       مزدوج x, مزدوج y)
{
  إذا (unlikely (!success))
    أرجع;

  مزدوج c0x = current_x, c0y = current_y;

  إذا (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    أرجع;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  مزدوج dx = x - c0x, dy = y - c0y;
  مزدوج len_sq = dx * dx + dy * dy;
  إذا (len_sq > 0.)
  {
    مزدوج inv = 1.0 / len_sq;
    مزدوج cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    مزدوج cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    إذا (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_کرښه_منزل (x, y);
      أرجع;
    }
  }

  hb_gpu_cubic_منزل_quadratics (هذا,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

ساكن فراغ
hb_gpu_draw_move_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
		     فراغ *معلومات,
		     hb_draw_state_t *st HB_ناکارول_شوی,
		     عائم منزل_x, عائم منزل_y,
		     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) معلومات;
  g->acc_close_لار ();
  g->acc_move_منزل ((مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_gpu_draw_کرښه_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
		     فراغ *معلومات,
		     hb_draw_state_t *st HB_ناکارول_شوی,
		     عائم منزل_x, عائم منزل_y,
		     فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) معلومات;
  g->acc_کرښه_منزل ((مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_gpu_draw_quadratic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			  فراغ *معلومات,
			  hb_draw_state_t *st HB_ناکارول_شوی,
			  عائم control_x, عائم control_y,
			  عائم منزل_x, عائم منزل_y,
			  فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) معلومات;
  g->acc_conic_منزل ((مزدوج) control_x, (مزدوج) control_y,
		   (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_gpu_draw_cubic_منزل (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
		      فراغ *معلومات,
		      hb_draw_state_t *st HB_ناکارول_شوی,
		      عائم control1_x, عائم control1_y,
		      عائم control2_x, عائم control2_y,
		      عائم منزل_x, عائم منزل_y,
		      فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) معلومات;
  g->acc_cubic_منزل ((مزدوج) control1_x, (مزدوج) control1_y,
		   (مزدوج) control2_x, (مزدوج) control2_y,
		   (مزدوج) منزل_x, (مزدوج) منزل_y);
}

ساكن فراغ
hb_gpu_draw_close_لار (hb_draw_funcs_t *dfuncs HB_ناکارول_شوی,
			فراغ *معلومات,
			hb_draw_state_t *st HB_ناکارول_شوی,
			فراغ *user_معلومات HB_ناکارول_شوی)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) معلومات;
  g->acc_close_لار ();
}

ساكن hb_bool_t
hb_gpu_draw_سېټ_budget (hb_draw_funcs_t *, فراغ *draw_معلومات,
			int64_t budget, فراغ *)
{
  خپلکاری *draw = (hb_gpu_draw_t *) draw_معلومات;
  draw->budget = budget;
  draw->recharge_budget ();
  أرجع ریښتیا;
}

ساكن int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_gpu_draw_t *) draw_معلومات)->budget;
}

ساكن int64_t *
hb_gpu_draw_get_budget_پاتې_شوی (hb_draw_funcs_t *, فراغ *draw_معلومات, فراغ *)
{
  أرجع ((hb_gpu_draw_t *) draw_معلومات)->get_budget_پاتې_شوی ();
}

ساكن مضمن فراغ free_static_gpu_draw_funcs ();

ساكن بنية hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  ساكن hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_سېټ_move_منزل_func      (funcs, hb_gpu_draw_move_منزل,      تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_کرښه_منزل_func      (funcs, hb_gpu_draw_کرښه_منزل,      تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_quadratic_منزل_func (funcs, hb_gpu_draw_quadratic_منزل, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_cubic_منزل_func     (funcs, hb_gpu_draw_cubic_منزل,     تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_close_لار_func   (funcs, hb_gpu_draw_close_لار,   تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_سېټ_budget_func (funcs, hb_gpu_draw_سېټ_budget, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_func (funcs, hb_gpu_draw_get_budget, تش_اشاره, تش_اشاره);
    hb_draw_funcs_سېټ_get_budget_پاتې_شوی_func (funcs, hb_gpu_draw_get_budget_پاتې_شوی, تش_اشاره, تش_اشاره);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    أرجع funcs;
  }
} static_gpu_draw_funcs;

ساكن مضمن فراغ
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

بنية hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

ساكن hb_position_t
clamp_منزل_hb_position (مزدوج v)
{
  أرجع (hb_position_t) hb_clamp (v,
				   (مزدوج) hb_int_min (hb_position_t),
				   (مزدوج) hb_int_max (hb_position_t));
}

ساكن مضمن int16_t
quantize (مزدوج v)
{
  أرجع (int16_t) round (v * HB_GPU_UNITS_فی_EM);
}

ساكن مضمن int16_t
quantize_ښکته (مزدوج v)
{
  أرجع (int16_t) floor (v * HB_GPU_UNITS_فی_EM);
}

ساكن مضمن int16_t
quantize_پورته (مزدوج v)
{
  أرجع (int16_t) ceil (v * HB_GPU_UNITS_فی_EM);
}

ساكن مضمن مزدوج
dequantize (صحيح v)
{
  أرجع (مزدوج) v / HB_GPU_UNITS_فی_EM;
}

ساكن مضمن منطقي
quantize_ښکته_ځاییږي_i16 (مزدوج v)
{
  مزدوج q = floor (v * HB_GPU_UNITS_فی_EM);
  أرجع q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ساكن مضمن منطقي
quantize_پورته_ځاییږي_i16 (مزدوج v)
{
  مزدوج q = ceil (v * HB_GPU_UNITS_فی_EM);
  أرجع q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ساكن مضمن int16_t
encode_دننه_واټن (بینښه دننه_واټن)
{
  أرجع (int16_t) (دننه_واټن - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
ساكن hb_gpu_encode_منحني_info_t
encode_منحني_info (ثابت hb_gpu_منحني_t *c)
{
  hb_gpu_encode_منحني_info_t info;

  صحيح p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  صحيح p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

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

  أرجع info;
}

ساكن فراغ
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  إذا (unlikely (!draw->success) ||
      draw->عدد_curves == 0 ||
      draw->ext_min_x == HUGE_VAL)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->پلنوالی = 0;
    extents->لوړوالی = 0;
    أرجع;
  }

  مزدوج min_x = floor (draw->ext_min_x);
  مزدوج min_y = floor (draw->ext_min_y);
  مزدوج max_x = ceil  (draw->ext_max_x);
  مزدوج max_y = ceil  (draw->ext_max_y);

  إذا (unlikely (!std::isfinite (min_x) ||
		!std::isfinite (min_y) ||
		!std::isfinite (max_x) ||
		!std::isfinite (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->پلنوالی = 0;
    extents->لوړوالی = 0;
    أرجع;
  }

  extents->x_bearing = clamp_منزل_hb_position (min_x);
  extents->y_bearing = clamp_منزل_hb_position (max_y);
  extents->پلنوالی     = clamp_منزل_hb_position (max_x - min_x);
  extents->لوړوالی    = clamp_منزل_hb_position (min_y - max_y);
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
  إذا (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  إذا (unlikely (!draw->success))
    أرجع تش_اشاره;

  ثابت hb_gpu_منحني_t *curves = draw->curves.arrayZ;
  بینښه عدد_curves = draw->curves.اوږدتیا;

  إذا (عدد_curves == 0)
    أرجع hb_blob_get_تشه ();

  hb_gpu_encode_scratch_t &s = draw->scratch;

  s.منحني_infos.reset_if_error ();
  s.hband_منحني_counts.reset_if_error ();
  s.vband_منحني_counts.reset_if_error ();
  s.hband_offsets.reset_if_error ();
  s.vband_offsets.reset_if_error ();
  s.hband_curves.reset_if_error ();
  s.hband_curves_asc.reset_if_error ();
  s.vband_curves.reset_if_error ();
  s.vband_curves_asc.reset_if_error ();
  s.hband_cursors.reset_if_error ();
  s.vband_cursors.reset_if_error ();
  s.منحني_texel_دننه_واټن.reset_if_error ();

  /* Compute per-curve info and extents */
  إذا (unlikely (!s.منحني_infos.resize (عدد_curves)))
    أرجع تش_اشاره;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  إذا (!quantize_ښکته_ځاییږي_i16 (draw->ext_min_x) ||
      !quantize_ښکته_ځاییږي_i16 (draw->ext_min_y) ||
      !quantize_پورته_ځاییږي_i16 (draw->ext_max_x) ||
      !quantize_پورته_ځاییږي_i16 (draw->ext_max_y))
    أرجع تش_اشاره;

  int16_t min_x_q = quantize_ښکته (draw->ext_min_x);
  int16_t min_y_q = quantize_ښکته (draw->ext_min_y);
  int16_t max_x_q = quantize_پورته (draw->ext_max_x);
  int16_t max_y_q = quantize_پورته (draw->ext_max_y);

  مزدوج min_x = dequantize (min_x_q);
  مزدوج min_y = dequantize (min_y_q);
  مزدوج max_x = dequantize (max_x_q);
  مزدوج max_y = dequantize (max_y_q);

  لكل (بینښه i = 0; i < عدد_curves; i++)
    s.منحني_infos.arrayZ[i] = encode_منحني_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  بینښه عدد_hbands = hb_min (عدد_curves, 16u);
  بینښه عدد_vbands = hb_min (عدد_curves, 16u);
  عدد_hbands = hb_max (عدد_hbands, 1u);
  عدد_vbands = hb_max (عدد_vbands, 1u);

  مزدوج لوړوالی = max_y - min_y;
  مزدوج پلنوالی  = max_x - min_x;

  إذا (لوړوالی <= 0) عدد_hbands = 1;
  إذا (پلنوالی  <= 0) عدد_vbands = 1;

  مزدوج hband_اندازه = لوړوالی / عدد_hbands;
  مزدوج vband_اندازه = پلنوالی  / عدد_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  ساكن ثابت مزدوج BAND_EPSILON = 1.0 / 1024;

  إذا (unlikely (!s.hband_منحني_counts.resize (عدد_hbands) ||
		!s.vband_منحني_counts.resize (عدد_vbands)))
    أرجع تش_اشاره;

  لكل (بینښه b = 0; b < عدد_hbands; b++) s.hband_منحني_counts.arrayZ[b] = 0;
  لكل (بینښه b = 0; b < عدد_vbands; b++) s.vband_منحني_counts.arrayZ[b] = 0;

  لكل (بینښه i = 0; i < عدد_curves; i++)
  {
    hb_gpu_encode_منحني_info_t &info = s.منحني_infos.arrayZ[i];

    إذا (!info.is_horizontal)
    {
      إذا (لوړوالی > 0) {
	info.hband_lo = (صحيح) floor ((info.min_y - min_y) / hband_اندازه - BAND_EPSILON);
	info.hband_hi = (صحيح) floor ((info.max_y - min_y) / hband_اندازه + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (صحيح) عدد_hbands - 1);
	لكل (صحيح b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_منحني_counts.arrayZ[b]++;
      } وإلا {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_منحني_counts.arrayZ[0]++;
      }
    }

    إذا (!info.is_vertical)
    {
      إذا (پلنوالی > 0) {
	info.vband_lo = (صحيح) floor ((info.min_x - min_x) / vband_اندازه - BAND_EPSILON);
	info.vband_hi = (صحيح) floor ((info.max_x - min_x) / vband_اندازه + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (صحيح) عدد_vbands - 1);
	لكل (صحيح b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_منحني_counts.arrayZ[b]++;
      } وإلا {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_منحني_counts.arrayZ[0]++;
      }
    }
  }

  إذا (unlikely (!s.hband_offsets.resize (عدد_hbands) ||
		!s.vband_offsets.resize (عدد_vbands)))
    أرجع تش_اشاره;

  بینښه ټول_hband_indices = 0;
  بینښه ټول_vband_indices = 0;

  لكل (بینښه b = 0; b < عدد_hbands; b++) {
    s.hband_offsets.arrayZ[b] = ټول_hband_indices;
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (ټول_hband_indices,
					     s.hband_منحني_counts.arrayZ[b],
					     &ټول_hband_indices)))
      أرجع تش_اشاره;
  }

  لكل (بینښه b = 0; b < عدد_vbands; b++) {
    s.vband_offsets.arrayZ[b] = ټول_vband_indices;
    إذا (unlikely (hb_unsigned_جمع_کړه_overflows (ټول_vband_indices,
					     s.vband_منحني_counts.arrayZ[b],
					     &ټول_vband_indices)))
      أرجع تش_اشاره;
  }

  /* Assign curves to bands */
  إذا (unlikely (!s.hband_curves.resize (ټول_hband_indices) ||
		!s.hband_curves_asc.resize (ټول_hband_indices) ||
		!s.vband_curves.resize (ټول_vband_indices) ||
		!s.vband_curves_asc.resize (ټول_vband_indices) ||
		!s.hband_cursors.resize (عدد_hbands) ||
		!s.vband_cursors.resize (عدد_vbands)))
    أرجع تش_اشاره;

  لكل (بینښه b = 0; b < عدد_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  لكل (بینښه b = 0; b < عدد_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  لكل (بینښه i = 0; i < عدد_curves; i++)
  {
    ثابت hb_gpu_encode_منحني_info_t &info = s.منحني_infos.arrayZ[i];

    لكل (صحيح b = info.hband_lo; b <= info.hband_hi; b++) {
      بینښه idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    لكل (صحيح b = info.vband_lo; b <= info.vband_hi; b++) {
      بینښه idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  ثابت hb_gpu_encode_منحني_info_t *infos = s.منحني_infos.arrayZ;

  لكل (بینښه b = 0; b < عدد_hbands; b++)
  {
    بینښه off = s.hband_offsets.arrayZ[b];
    بینښه شمېر = s.hband_منحني_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, شمېر)
      .qsort ([infos] (ثابت بینښه &a, ثابت بینښه &b) {
	خپلکاری av = infos[a].max_x, bv = infos[b].max_x;
	أرجع (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, شمېر)
      .qsort ([infos] (ثابت بینښه &a, ثابت بینښه &b) {
	خپلکاری av = infos[a].min_x, bv = infos[b].min_x;
	أرجع (av > bv) - (av < bv);
      });
  }

  لكل (بینښه b = 0; b < عدد_vbands; b++)
  {
    بینښه off = s.vband_offsets.arrayZ[b];
    بینښه شمېر = s.vband_منحني_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, شمېر)
      .qsort ([infos] (ثابت بینښه &a, ثابت بینښه &b) {
	خپلکاری av = infos[a].max_y, bv = infos[b].max_y;
	أرجع (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, شمېر)
      .qsort ([infos] (ثابت بینښه &a, ثابت بینښه &b) {
	خپلکاری av = infos[a].min_y, bv = infos[b].min_y;
	أرجع (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  بینښه ټول_منحني_indices;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (ټول_hband_indices,
					   ټول_vband_indices,
					   &ټول_منحني_indices) ||
		hb_unsigned_mul_overflows (ټول_منحني_indices,
					   2,
					   &ټول_منحني_indices)))
    أرجع تش_اشاره;

  بینښه سرلیک_len = 2;

  بینښه عدد_contour_breaks = 0;
  لكل (بینښه i = 0; i + 1 < عدد_curves; i++)
    إذا (curves[i + 1].contour_پیل)
      عدد_contour_breaks++;

  بینښه منحني_معلومات_len;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (عدد_curves,
					   عدد_contour_breaks,
					   &منحني_معلومات_len) ||
		hb_unsigned_جمع_کړه_overflows (منحني_معلومات_len,
					   1,
					   &منحني_معلومات_len)))
    أرجع تش_اشاره;

  بینښه band_سرلیکونه_len;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (عدد_hbands,
					   عدد_vbands,
					   &band_سرلیکونه_len)))
    أرجع تش_اشاره;

  بینښه ټول_len = سرلیک_len;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (ټول_len,
					   band_سرلیکونه_len,
					   &ټول_len) ||
		hb_unsigned_جمع_کړه_overflows (ټول_len,
					   ټول_منحني_indices,
					   &ټول_len) ||
		hb_unsigned_جمع_کړه_overflows (ټول_len,
					   منحني_معلومات_len,
					   &ټول_len)))
    أرجع تش_اشاره;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  إذا (ټول_len > (بینښه) UINT16_MAX + 1u)
    أرجع تش_اشاره;

  /* Allocate or reuse encode buffer */
  بینښه needed_بایټونه;
  إذا (unlikely (hb_unsigned_mul_overflows (ټول_len,
					   حجم (hb_gpu_texel_t),
					   &needed_بایټونه)))
    أرجع تش_اشاره;
  بینښه buf_ظرفیت = 0;
  حرف *replaced_recycled_buf = تش_اشاره;
  حرف *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_بایټونه,
					&buf_ظرفیت, &replaced_recycled_buf);
  إذا (unlikely (!buf_raw))
    أرجع تش_اشاره;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (فراغ *) buf_raw;

  بینښه منحني_معلومات_دننه_واټن = سرلیک_len;
  إذا (unlikely (hb_unsigned_جمع_کړه_overflows (منحني_معلومات_دننه_واټن,
					   band_سرلیکونه_len,
					   &منحني_معلومات_دننه_واټن) ||
		hb_unsigned_جمع_کړه_overflows (منحني_معلومات_دننه_واټن,
					   ټول_منحني_indices,
					   &منحني_معلومات_دننه_واټن)))
  {
    hb_blob_t::recycle_abort ((حرف *) buf, draw->recycled_blob);
    أرجع تش_اشاره;
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
  إذا (unlikely (!s.منحني_texel_دننه_واټن.resize (عدد_curves)))
  {
    hb_blob_t::recycle_abort ((حرف *) buf, draw->recycled_blob);
    أرجع تش_اشاره;
  }

  بینښه texel = منحني_معلومات_دننه_واټن;

  لكل (بینښه i = 0; i < عدد_curves; i++)
  {
    منطقي contour_پیل = curves[i].contour_پیل;

    إذا (contour_پیل) {
      s.منحني_texel_دننه_واټن.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } وإلا {
      s.منحني_texel_دننه_واټن.arrayZ[i] = texel - 1;
    }

    منطقي has_بل = i + 1 < عدد_curves &&
		    !curves[i + 1].contour_پیل;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    إذا (has_بل) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } وإلا {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  بینښه شاخص_دننه_واټن = سرلیک_len + band_سرلیکونه_len;

  لكل (بینښه b = 0; b < عدد_hbands; b++)
  {
    int16_t hband_وېشل;
    {
      بینښه off = s.hband_offsets.arrayZ[b];
      بینښه n = s.hband_منحني_counts.arrayZ[b];
      بینښه غوره_worst = n;
      مزدوج غوره_وېشل = (min_x + max_x) * 0.5;
      بینښه کیڼ_شمېر = n;
      لكل (بینښه ci = 0; ci < n; ci++) {
	مزدوج وېشل = s.منحني_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	بینښه ښی_شمېر = ci + 1;
	طالما (کیڼ_شمېر &&
	       s.منحني_infos.arrayZ[s.hband_curves_asc.arrayZ[off + کیڼ_شمېر - 1]].min_x > وېشل)
	  کیڼ_شمېر--;
	بینښه worst = hb_max (ښی_شمېر, کیڼ_شمېر);
	إذا (worst < غوره_worst) {
	  غوره_worst = worst;
	  غوره_وېشل = وېشل;
	}
      }
      hband_وېشل = quantize (غوره_وېشل);
    }
    بینښه hdr = سرلیک_len + b;
    بینښه desc_off = شاخص_دننه_واټن;

    لكل (بینښه ci = 0; ci < s.hband_منحني_counts.arrayZ[b]; ci++) {
      buf[شاخص_دننه_واټن].r = encode_دننه_واټن (s.منحني_texel_دننه_واټن.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[شاخص_دننه_واټن].g = 0;
      buf[شاخص_دننه_واټن].b = 0;
      buf[شاخص_دننه_واټن].a = 0;
      شاخص_دننه_واټن++;
    }

    بینښه asc_off = شاخص_دننه_واټن;

    لكل (بینښه ci = 0; ci < s.hband_منحني_counts.arrayZ[b]; ci++) {
      buf[شاخص_دننه_واټن].r = encode_دننه_واټن (s.منحني_texel_دننه_واټن.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[شاخص_دننه_واټن].g = 0;
      buf[شاخص_دننه_واټن].b = 0;
      buf[شاخص_دننه_واټن].a = 0;
      شاخص_دننه_واټن++;
    }

    buf[hdr].r = (int16_t) s.hband_منحني_counts.arrayZ[b];
    buf[hdr].g = encode_دننه_واټن (desc_off);
    buf[hdr].b = encode_دننه_واټن (asc_off);
    buf[hdr].a = hband_وېشل;
  }

  لكل (بینښه b = 0; b < عدد_vbands; b++)
  {
    int16_t vband_وېشل;
    {
      بینښه off = s.vband_offsets.arrayZ[b];
      بینښه n = s.vband_منحني_counts.arrayZ[b];
      بینښه غوره_worst = n;
      مزدوج غوره_وېشل = (min_y + max_y) * 0.5;
      بینښه کیڼ_شمېر = n;
      لكل (بینښه ci = 0; ci < n; ci++) {
	مزدوج وېشل = s.منحني_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	بینښه ښی_شمېر = ci + 1;
	طالما (کیڼ_شمېر &&
	       s.منحني_infos.arrayZ[s.vband_curves_asc.arrayZ[off + کیڼ_شمېر - 1]].min_y > وېشل)
	  کیڼ_شمېر--;
	بینښه worst = hb_max (ښی_شمېر, کیڼ_شمېر);
	إذا (worst < غوره_worst) {
	  غوره_worst = worst;
	  غوره_وېشل = وېشل;
	}
      }
      vband_وېشل = quantize (غوره_وېشل);
    }

    بینښه hdr = سرلیک_len + عدد_hbands + b;
    بینښه desc_off = شاخص_دننه_واټن;

    لكل (بینښه ci = 0; ci < s.vband_منحني_counts.arrayZ[b]; ci++) {
      buf[شاخص_دننه_واټن].r = encode_دننه_واټن (s.منحني_texel_دننه_واټن.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[شاخص_دننه_واټن].g = 0;
      buf[شاخص_دننه_واټن].b = 0;
      buf[شاخص_دننه_واټن].a = 0;
      شاخص_دننه_واټن++;
    }

    بینښه asc_off = شاخص_دننه_واټن;

    لكل (بینښه ci = 0; ci < s.vband_منحني_counts.arrayZ[b]; ci++) {
      buf[شاخص_دننه_واټن].r = encode_دننه_واټن (s.منحني_texel_دننه_واټن.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[شاخص_دننه_واټن].g = 0;
      buf[شاخص_دننه_واټن].b = 0;
      buf[شاخص_دننه_واټن].a = 0;
      شاخص_دننه_واټن++;
    }

    buf[hdr].r = (int16_t) s.vband_منحني_counts.arrayZ[b];
    buf[hdr].g = encode_دننه_واټن (desc_off);
    buf[hdr].b = encode_دننه_واټن (asc_off);
    buf[hdr].a = vband_وېشل;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = تش_اشاره;
  أرجع hb_blob_t::recycle_finalize ((حرف *) buf, buf_ظرفیت, needed_بایټونه,
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
hb_gpu_draw_create_or_fail (فراغ)
{
  أرجع hb_object_create<hb_gpu_draw_t> ();
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
  أرجع hb_object_reference (draw);
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
فراغ
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  إذا (!hb_object_should_destroy (draw))
    أرجع;

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
hb_gpu_draw_سېټ_user_معلومات (hb_gpu_draw_t     *draw,
			     hb_user_معلومات_کلی_t *key,
			     فراغ               *معلومات,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  أرجع hb_object_سېټ_user_معلومات (draw, key, معلومات, destroy, replace);
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
فراغ *
hb_gpu_draw_get_user_معلومات (ثابت hb_gpu_draw_t     *draw,
			     hb_user_معلومات_کلی_t *key)
{
  أرجع hb_object_get_user_معلومات (draw, key);
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
hb_gpu_draw_get_funcs (ثابت hb_gpu_draw_t *draw HB_ناکارول_شوی)
{
  أرجع static_gpu_draw_funcs.get_unconst ();
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
فراغ
hb_gpu_draw_سېټ_scale (hb_gpu_draw_t *draw,
		       صحيح            x_scale,
		       صحيح            y_scale)
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
فراغ
hb_gpu_draw_get_scale (ثابت hb_gpu_draw_t *draw,
		       صحيح                 *x_scale,
		       صحيح                 *y_scale)
{
  إذا (x_scale) *x_scale = draw->x_scale;
  إذا (y_scale) *y_scale = draw->y_scale;
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
  صحيح x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_سېټ_scale (draw, x_scale, y_scale);

  أرجع hb_font_draw_glyph_or_fail (font, glyph,
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
فراغ
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
فراغ
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->پیل_x = draw->پیل_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = ریښتیا;
  draw->عدد_curves = 0;
  draw->success = ریښتیا;
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
فراغ
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
فراغ
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#ضمن "hb-gpu-draw-fragment-glsl.hh"
#ضمن "hb-gpu-draw-fragment-msl.hh"
#ضمن "hb-gpu-draw-fragment-wgsl.hh"
#ضمن "hb-gpu-draw-fragment-hlsl.hh"

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
hb_gpu_draw_shader_سرچینه (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  بدل (stage) {
  حالة HB_GPU_SHADER_STAGE_FRAGMENT:
    بدل (lang) {
    حالة HB_GPU_SHADER_LANG_GLSL: أرجع hb_gpu_draw_fragment_glsl;
    حالة HB_GPU_SHADER_LANG_MSL:  أرجع hb_gpu_draw_fragment_msl;
    حالة HB_GPU_SHADER_LANG_WGSL: أرجع hb_gpu_draw_fragment_wgsl;
    حالة HB_GPU_SHADER_LANG_HLSL: أرجع hb_gpu_draw_fragment_hlsl;
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
