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
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-cu2qu.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>

/* ---- Accumulator ---- */

ਸਥਿਰ ਖਾਲੀ
acc_update_extents (hb_gpu_draw_t *g, ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

ਸਥਿਰ ਖਾਲੀ
acc_emit (hb_gpu_draw_t *g,
	  ਤਾਰਕਿਕ contour_ਸ਼ੁਰੂ,
	  ਦੁੱਗਣਾ p1x, ਦੁੱਗਣਾ p1y,
	  ਦੁੱਗਣਾ p2x, ਦੁੱਗਣਾ p2y,
	  ਦੁੱਗਣਾ p3x, ਦੁੱਗਣਾ p3y)
{
  int64_t *budget = g->get_budget_ਬਚਿਆ ();
  ਜੇ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    ਵਾਪਸ;

  ਜੇ (unlikely (g->ਅੰਕ_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = ਝੂਠ;
    ਵਾਪਸ;
  }

  hb_gpu_ਵਕਰ_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_ਸ਼ੁਰੂ};
  ਜੇ (unlikely (!g->curves.ਧੱਕੋ_or_fail (c)))
  {
    g->success = ਝੂਠ;
    ਵਾਪਸ;
  }

  g->ਅੰਕ_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

ਸਥਿਰ ਖਾਲੀ
acc_emit_conic (hb_gpu_draw_t *g,
		ਦੁੱਗਣਾ cx, ਦੁੱਗਣਾ cy,
		ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  ਜੇ (g->current_x == x && g->current_y == y)
    ਵਾਪਸ;

  ਤਾਰਕਿਕ contour_ਸ਼ੁਰੂ = g->need_moveto;
  ਜੇ (g->need_moveto) {
    g->ਸ਼ੁਰੂ_x = g->current_x;
    g->ਸ਼ੁਰੂ_y = g->current_y;
    g->need_moveto = ਝੂਠ;
  }

  acc_emit (g, contour_ਸ਼ੁਰੂ,
	    g->current_x, g->current_y, cx, cy, x, y);
}

ਖਾਲੀ
hb_gpu_draw_t::acc_move_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  ਜੇ (unlikely (!success))
    ਵਾਪਸ;

  need_moveto = ਸੱਚ;
  current_x = x;
  current_y = y;
}

ਖਾਲੀ
hb_gpu_draw_t::acc_ਸਤਰ_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  ਜੇ (unlikely (!success))
    ਵਾਪਸ;

  acc_emit_conic (ਇਹ, current_x, current_y, x, y);
}

ਖਾਲੀ
hb_gpu_draw_t::acc_conic_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ cx, ਦੁੱਗਣਾ cy, ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  ਜੇ (unlikely (!success))
    ਵਾਪਸ;

  acc_emit_conic (ਇਹ, cx, cy, x, y);
}

ਖਾਲੀ
hb_gpu_draw_t::acc_close_ਰਸਤਾ ()
{
  ਜੇ (unlikely (!success))
    ਵਾਪਸ;

  ਜੇ (!need_moveto &&
      (current_x != ਸ਼ੁਰੂ_x || current_y != ਸ਼ੁਰੂ_y))
    acc_ਸਤਰ_ਮੰਜ਼ਿਲ (ਸ਼ੁਰੂ_x, ਸ਼ੁਰੂ_y);
}


ਖਾਲੀ
hb_gpu_draw_t::acc_cubic_ਮੰਜ਼ਿਲ (ਦੁੱਗਣਾ c1x, ਦੁੱਗਣਾ c1y,
			       ਦੁੱਗਣਾ c2x, ਦੁੱਗਣਾ c2y,
			       ਦੁੱਗਣਾ x, ਦੁੱਗਣਾ y)
{
  ਜੇ (unlikely (!success))
    ਵਾਪਸ;

  ਦੁੱਗਣਾ c0x = current_x, c0y = current_y;

  ਜੇ (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    ਵਾਪਸ;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  ਦੁੱਗਣਾ dx = x - c0x, dy = y - c0y;
  ਦੁੱਗਣਾ len_sq = dx * dx + dy * dy;
  ਜੇ (len_sq > 0.)
  {
    ਦੁੱਗਣਾ inv = 1.0 / len_sq;
    ਦੁੱਗਣਾ cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    ਦੁੱਗਣਾ cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    ਜੇ (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_ਸਤਰ_ਮੰਜ਼ਿਲ (x, y);
      ਵਾਪਸ;
    }
  }

  hb_gpu_cubic_ਮੰਜ਼ਿਲ_quadratics (ਇਹ,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

ਸਥਿਰ ਖਾਲੀ
hb_gpu_draw_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *ਅੰਕੜੇ,
		     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ਅੰਕੜੇ;
  g->acc_close_ਰਸਤਾ ();
  g->acc_move_ਮੰਜ਼ਿਲ ((ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_draw_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		     ਖਾਲੀ *ਅੰਕੜੇ,
		     hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		     ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		     ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ਅੰਕੜੇ;
  g->acc_ਸਤਰ_ਮੰਜ਼ਿਲ ((ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_draw_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			  ਖਾਲੀ *ਅੰਕੜੇ,
			  hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			  ਦਸ਼ਮਲਵ control_x, ਦਸ਼ਮਲਵ control_y,
			  ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
			  ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ਅੰਕੜੇ;
  g->acc_conic_ਮੰਜ਼ਿਲ ((ਦੁੱਗਣਾ) control_x, (ਦੁੱਗਣਾ) control_y,
		   (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_draw_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
		      ਖਾਲੀ *ਅੰਕੜੇ,
		      hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
		      ਦਸ਼ਮਲਵ control1_x, ਦਸ਼ਮਲਵ control1_y,
		      ਦਸ਼ਮਲਵ control2_x, ਦਸ਼ਮਲਵ control2_y,
		      ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_x, ਦਸ਼ਮਲਵ ਮੰਜ਼ਿਲ_y,
		      ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ਅੰਕੜੇ;
  g->acc_cubic_ਮੰਜ਼ਿਲ ((ਦੁੱਗਣਾ) control1_x, (ਦੁੱਗਣਾ) control1_y,
		   (ਦੁੱਗਣਾ) control2_x, (ਦੁੱਗਣਾ) control2_y,
		   (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_x, (ਦੁੱਗਣਾ) ਮੰਜ਼ਿਲ_y);
}

ਸਥਿਰ ਖਾਲੀ
hb_gpu_draw_close_ਰਸਤਾ (hb_draw_funcs_t *dfuncs HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *ਅੰਕੜੇ,
			hb_draw_state_t *st HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) ਅੰਕੜੇ;
  g->acc_close_ਰਸਤਾ ();
}

ਸਥਿਰ hb_bool_t
hb_gpu_draw_ਸੈੱਟ_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *draw = (hb_gpu_draw_t *) draw_ਅੰਕੜੇ;
  draw->budget = budget;
  draw->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_gpu_draw_t *) draw_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_gpu_draw_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_gpu_draw_t *) draw_ਅੰਕੜੇ)->get_budget_ਬਚਿਆ ();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_gpu_draw_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func      (funcs, hb_gpu_draw_move_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func      (funcs, hb_gpu_draw_ਸਤਰ_ਮੰਜ਼ਿਲ,      ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, hb_gpu_draw_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func     (funcs, hb_gpu_draw_cubic_ਮੰਜ਼ਿਲ,     ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func   (funcs, hb_gpu_draw_close_ਰਸਤਾ,   ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_gpu_draw_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_gpu_draw_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_gpu_draw_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    ਵਾਪਸ funcs;
  }
} static_gpu_draw_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

ਬਣਤਰ hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

ਸਥਿਰ hb_position_t
clamp_ਮੰਜ਼ਿਲ_hb_position (ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ (hb_position_t) hb_clamp (v,
				   (ਦੁੱਗਣਾ) hb_int_min (hb_position_t),
				   (ਦੁੱਗਣਾ) hb_int_max (hb_position_t));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
quantize (ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ (int16_t) round (v * HB_GPU_UNITS_ਪ੍ਰਤੀ_EM);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
quantize_ਥੱਲੇ (ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ (int16_t) floor (v * HB_GPU_UNITS_ਪ੍ਰਤੀ_EM);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
quantize_ਉੱਪਰ (ਦੁੱਗਣਾ v)
{
  ਵਾਪਸ (int16_t) ceil (v * HB_GPU_UNITS_ਪ੍ਰਤੀ_EM);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਦੁੱਗਣਾ
dequantize (ਪੂਰਨ v)
{
  ਵਾਪਸ (ਦੁੱਗਣਾ) v / HB_GPU_UNITS_ਪ੍ਰਤੀ_EM;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
quantize_ਥੱਲੇ_ਸਮਾਉਂਦਾ_i16 (ਦੁੱਗਣਾ v)
{
  ਦੁੱਗਣਾ q = floor (v * HB_GPU_UNITS_ਪ੍ਰਤੀ_EM);
  ਵਾਪਸ q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
quantize_ਉੱਪਰ_ਸਮਾਉਂਦਾ_i16 (ਦੁੱਗਣਾ v)
{
  ਦੁੱਗਣਾ q = ceil (v * HB_GPU_UNITS_ਪ੍ਰਤੀ_EM);
  ਵਾਪਸ q >= INT16_MIN &&
	 q <= INT16_MAX;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ int16_t
encode_ਵਿੱਥ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ)
{
  ਵਾਪਸ (int16_t) (ਵਿੱਥ - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
ਸਥਿਰ hb_gpu_encode_ਵਕਰ_info_t
encode_ਵਕਰ_info (ਅਚਲ hb_gpu_ਵਕਰ_t *c)
{
  hb_gpu_encode_ਵਕਰ_info_t info;

  ਪੂਰਨ p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  ਪੂਰਨ p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

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

  ਵਾਪਸ info;
}

ਸਥਿਰ ਖਾਲੀ
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  ਜੇ (unlikely (!draw->success) ||
      draw->ਅੰਕ_curves == 0 ||
      draw->ext_min_x == HUGE_VAL)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->ਚੌੜਾਈ = 0;
    extents->ਉਚਾਈ = 0;
    ਵਾਪਸ;
  }

  ਦੁੱਗਣਾ min_x = floor (draw->ext_min_x);
  ਦੁੱਗਣਾ min_y = floor (draw->ext_min_y);
  ਦੁੱਗਣਾ max_x = ceil  (draw->ext_max_x);
  ਦੁੱਗਣਾ max_y = ceil  (draw->ext_max_y);

  ਜੇ (unlikely (!std::isfinite (min_x) ||
		!std::isfinite (min_y) ||
		!std::isfinite (max_x) ||
		!std::isfinite (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->ਚੌੜਾਈ = 0;
    extents->ਉਚਾਈ = 0;
    ਵਾਪਸ;
  }

  extents->x_bearing = clamp_ਮੰਜ਼ਿਲ_hb_position (min_x);
  extents->y_bearing = clamp_ਮੰਜ਼ਿਲ_hb_position (max_y);
  extents->ਚੌੜਾਈ     = clamp_ਮੰਜ਼ਿਲ_hb_position (max_x - min_x);
  extents->ਉਚਾਈ    = clamp_ਮੰਜ਼ਿਲ_hb_position (min_y - max_y);
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
  ਜੇ (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  ਜੇ (unlikely (!draw->success))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ hb_gpu_ਵਕਰ_t *curves = draw->curves.arrayZ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_curves = draw->curves.ਲੰਮਾਈ;

  ਜੇ (ਅੰਕ_curves == 0)
    ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_gpu_encode_scratch_t &s = draw->scratch;

  s.ਵਕਰ_infos.reset_if_error ();
  s.hband_ਵਕਰ_counts.reset_if_error ();
  s.vband_ਵਕਰ_counts.reset_if_error ();
  s.hband_offsets.reset_if_error ();
  s.vband_offsets.reset_if_error ();
  s.hband_curves.reset_if_error ();
  s.hband_curves_asc.reset_if_error ();
  s.vband_curves.reset_if_error ();
  s.vband_curves_asc.reset_if_error ();
  s.hband_cursors.reset_if_error ();
  s.vband_cursors.reset_if_error ();
  s.ਵਕਰ_texel_ਵਿੱਥ.reset_if_error ();

  /* Compute per-curve info and extents */
  ਜੇ (unlikely (!s.ਵਕਰ_infos.resize (ਅੰਕ_curves)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  ਜੇ (!quantize_ਥੱਲੇ_ਸਮਾਉਂਦਾ_i16 (draw->ext_min_x) ||
      !quantize_ਥੱਲੇ_ਸਮਾਉਂਦਾ_i16 (draw->ext_min_y) ||
      !quantize_ਉੱਪਰ_ਸਮਾਉਂਦਾ_i16 (draw->ext_max_x) ||
      !quantize_ਉੱਪਰ_ਸਮਾਉਂਦਾ_i16 (draw->ext_max_y))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  int16_t min_x_q = quantize_ਥੱਲੇ (draw->ext_min_x);
  int16_t min_y_q = quantize_ਥੱਲੇ (draw->ext_min_y);
  int16_t max_x_q = quantize_ਉੱਪਰ (draw->ext_max_x);
  int16_t max_y_q = quantize_ਉੱਪਰ (draw->ext_max_y);

  ਦੁੱਗਣਾ min_x = dequantize (min_x_q);
  ਦੁੱਗਣਾ min_y = dequantize (min_y_q);
  ਦੁੱਗਣਾ max_x = dequantize (max_x_q);
  ਦੁੱਗਣਾ max_y = dequantize (max_y_q);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_curves; i++)
    s.ਵਕਰ_infos.arrayZ[i] = encode_ਵਕਰ_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_hbands = hb_min (ਅੰਕ_curves, 16u);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_vbands = hb_min (ਅੰਕ_curves, 16u);
  ਅੰਕ_hbands = hb_max (ਅੰਕ_hbands, 1u);
  ਅੰਕ_vbands = hb_max (ਅੰਕ_vbands, 1u);

  ਦੁੱਗਣਾ ਉਚਾਈ = max_y - min_y;
  ਦੁੱਗਣਾ ਚੌੜਾਈ  = max_x - min_x;

  ਜੇ (ਉਚਾਈ <= 0) ਅੰਕ_hbands = 1;
  ਜੇ (ਚੌੜਾਈ  <= 0) ਅੰਕ_vbands = 1;

  ਦੁੱਗਣਾ hband_ਮਾਪ = ਉਚਾਈ / ਅੰਕ_hbands;
  ਦੁੱਗਣਾ vband_ਮਾਪ = ਚੌੜਾਈ  / ਅੰਕ_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  ਸਥਿਰ ਅਚਲ ਦੁੱਗਣਾ BAND_EPSILON = 1.0 / 1024;

  ਜੇ (unlikely (!s.hband_ਵਕਰ_counts.resize (ਅੰਕ_hbands) ||
		!s.vband_ਵਕਰ_counts.resize (ਅੰਕ_vbands)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_hbands; b++) s.hband_ਵਕਰ_counts.arrayZ[b] = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_vbands; b++) s.vband_ਵਕਰ_counts.arrayZ[b] = 0;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_curves; i++)
  {
    hb_gpu_encode_ਵਕਰ_info_t &info = s.ਵਕਰ_infos.arrayZ[i];

    ਜੇ (!info.is_horizontal)
    {
      ਜੇ (ਉਚਾਈ > 0) {
	info.hband_lo = (ਪੂਰਨ) floor ((info.min_y - min_y) / hband_ਮਾਪ - BAND_EPSILON);
	info.hband_hi = (ਪੂਰਨ) floor ((info.max_y - min_y) / hband_ਮਾਪ + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (ਪੂਰਨ) ਅੰਕ_hbands - 1);
	ਲਈ (ਪੂਰਨ b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_ਵਕਰ_counts.arrayZ[b]++;
      } ਨਹੀਂਤਾਂ {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_ਵਕਰ_counts.arrayZ[0]++;
      }
    }

    ਜੇ (!info.is_vertical)
    {
      ਜੇ (ਚੌੜਾਈ > 0) {
	info.vband_lo = (ਪੂਰਨ) floor ((info.min_x - min_x) / vband_ਮਾਪ - BAND_EPSILON);
	info.vband_hi = (ਪੂਰਨ) floor ((info.max_x - min_x) / vband_ਮਾਪ + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (ਪੂਰਨ) ਅੰਕ_vbands - 1);
	ਲਈ (ਪੂਰਨ b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_ਵਕਰ_counts.arrayZ[b]++;
      } ਨਹੀਂਤਾਂ {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_ਵਕਰ_counts.arrayZ[0]++;
      }
    }
  }

  ਜੇ (unlikely (!s.hband_offsets.resize (ਅੰਕ_hbands) ||
		!s.vband_offsets.resize (ਅੰਕ_vbands)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_hband_indices = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_vband_indices = 0;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_hbands; b++) {
    s.hband_offsets.arrayZ[b] = ਕੁੱਲ_hband_indices;
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_hband_indices,
					     s.hband_ਵਕਰ_counts.arrayZ[b],
					     &ਕੁੱਲ_hband_indices)))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_vbands; b++) {
    s.vband_offsets.arrayZ[b] = ਕੁੱਲ_vband_indices;
    ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_vband_indices,
					     s.vband_ਵਕਰ_counts.arrayZ[b],
					     &ਕੁੱਲ_vband_indices)))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  /* Assign curves to bands */
  ਜੇ (unlikely (!s.hband_curves.resize (ਕੁੱਲ_hband_indices) ||
		!s.hband_curves_asc.resize (ਕੁੱਲ_hband_indices) ||
		!s.vband_curves.resize (ਕੁੱਲ_vband_indices) ||
		!s.vband_curves_asc.resize (ਕੁੱਲ_vband_indices) ||
		!s.hband_cursors.resize (ਅੰਕ_hbands) ||
		!s.vband_cursors.resize (ਅੰਕ_vbands)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_curves; i++)
  {
    ਅਚਲ hb_gpu_encode_ਵਕਰ_info_t &info = s.ਵਕਰ_infos.arrayZ[i];

    ਲਈ (ਪੂਰਨ b = info.hband_lo; b <= info.hband_hi; b++) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    ਲਈ (ਪੂਰਨ b = info.vband_lo; b <= info.vband_hi; b++) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  ਅਚਲ hb_gpu_encode_ਵਕਰ_info_t *infos = s.ਵਕਰ_infos.arrayZ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_hbands; b++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ off = s.hband_offsets.arrayZ[b];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = s.hband_ਵਕਰ_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, ਗਿਣਤੀ_2)
      .qsort ([infos] (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &a, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &b) {
	ਸਵੈਚਾਲਿਤ av = infos[a].max_x, bv = infos[b].max_x;
	ਵਾਪਸ (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, ਗਿਣਤੀ_2)
      .qsort ([infos] (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &a, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &b) {
	ਸਵੈਚਾਲਿਤ av = infos[a].min_x, bv = infos[b].min_x;
	ਵਾਪਸ (av > bv) - (av < bv);
      });
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_vbands; b++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ off = s.vband_offsets.arrayZ[b];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = s.vband_ਵਕਰ_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, ਗਿਣਤੀ_2)
      .qsort ([infos] (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &a, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &b) {
	ਸਵੈਚਾਲਿਤ av = infos[a].max_y, bv = infos[b].max_y;
	ਵਾਪਸ (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, ਗਿਣਤੀ_2)
      .qsort ([infos] (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &a, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ &b) {
	ਸਵੈਚਾਲਿਤ av = infos[a].min_y, bv = infos[b].min_y;
	ਵਾਪਸ (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਵਕਰ_indices;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_hband_indices,
					   ਕੁੱਲ_vband_indices,
					   &ਕੁੱਲ_ਵਕਰ_indices) ||
		hb_unsigned_mul_overflows (ਕੁੱਲ_ਵਕਰ_indices,
					   2,
					   &ਕੁੱਲ_ਵਕਰ_indices)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਲੇਖ_len = 2;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_contour_breaks = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਅੰਕ_curves; i++)
    ਜੇ (curves[i + 1].contour_ਸ਼ੁਰੂ)
      ਅੰਕ_contour_breaks++;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਕਰ_ਅੰਕੜੇ_len;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਅੰਕ_curves,
					   ਅੰਕ_contour_breaks,
					   &ਵਕਰ_ਅੰਕੜੇ_len) ||
		hb_unsigned_ਜੋੜੋ_overflows (ਵਕਰ_ਅੰਕੜੇ_len,
					   1,
					   &ਵਕਰ_ਅੰਕੜੇ_len)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ band_ਸਿਰਲੇਖਾਂ_len;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਅੰਕ_hbands,
					   ਅੰਕ_vbands,
					   &band_ਸਿਰਲੇਖਾਂ_len)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_len = ਸਿਰਲੇਖ_len;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_len,
					   band_ਸਿਰਲੇਖਾਂ_len,
					   &ਕੁੱਲ_len) ||
		hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_len,
					   ਕੁੱਲ_ਵਕਰ_indices,
					   &ਕੁੱਲ_len) ||
		hb_unsigned_ਜੋੜੋ_overflows (ਕੁੱਲ_len,
					   ਵਕਰ_ਅੰਕੜੇ_len,
					   &ਕੁੱਲ_len)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  ਜੇ (ਕੁੱਲ_len > (ਬਿਨਾਨਿਸ਼ਾਨੀ) UINT16_MAX + 1u)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Allocate or reuse encode buffer */
  ਬਿਨਾਨਿਸ਼ਾਨੀ needed_ਬਾਈਟਾਂ;
  ਜੇ (unlikely (hb_unsigned_mul_overflows (ਕੁੱਲ_len,
					   ਆਕਾਰ (hb_gpu_texel_t),
					   &needed_ਬਾਈਟਾਂ)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ buf_ਸਮਰੱਥਾ = 0;
  ਅੱਖਰ *replaced_recycled_buf = ਖਾਲੀ_ਸੂਚਕ;
  ਅੱਖਰ *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_ਬਾਈਟਾਂ,
					&buf_ਸਮਰੱਥਾ, &replaced_recycled_buf);
  ਜੇ (unlikely (!buf_raw))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (ਖਾਲੀ *) buf_raw;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ = ਸਿਰਲੇਖ_len;
  ਜੇ (unlikely (hb_unsigned_ਜੋੜੋ_overflows (ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ,
					   band_ਸਿਰਲੇਖਾਂ_len,
					   &ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ) ||
		hb_unsigned_ਜੋੜੋ_overflows (ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ,
					   ਕੁੱਲ_ਵਕਰ_indices,
					   &ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ)))
  {
    hb_blob_t::recycle_abort ((ਅੱਖਰ *) buf, draw->recycled_blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  /* Pack header */
  buf[0].r = min_x_q;
  buf[0].g = min_y_q;
  buf[0].b = max_x_q;
  buf[0].a = max_y_q;
  buf[1].r = (int16_t) ਅੰਕ_hbands;
  buf[1].g = (int16_t) ਅੰਕ_vbands;
  buf[1].b = (int16_t) hb_clamp (draw->x_scale, -32768, 32767);
  buf[1].a = (int16_t) hb_clamp (draw->y_scale, -32768, 32767);

  /* Pack curve data with shared endpoints */
  ਜੇ (unlikely (!s.ਵਕਰ_texel_ਵਿੱਥ.resize (ਅੰਕ_curves)))
  {
    hb_blob_t::recycle_abort ((ਅੱਖਰ *) buf, draw->recycled_blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ texel = ਵਕਰ_ਅੰਕੜੇ_ਵਿੱਥ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_curves; i++)
  {
    ਤਾਰਕਿਕ contour_ਸ਼ੁਰੂ = curves[i].contour_ਸ਼ੁਰੂ;

    ਜੇ (contour_ਸ਼ੁਰੂ) {
      s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } ਨਹੀਂਤਾਂ {
      s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[i] = texel - 1;
    }

    ਤਾਰਕਿਕ has_ਅਗਲਾ = i + 1 < ਅੰਕ_curves &&
		    !curves[i + 1].contour_ਸ਼ੁਰੂ;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    ਜੇ (has_ਅਗਲਾ) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } ਨਹੀਂਤਾਂ {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_ਵਿੱਥ = ਸਿਰਲੇਖ_len + band_ਸਿਰਲੇਖਾਂ_len;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_hbands; b++)
  {
    int16_t hband_ਵੰਡੋ;
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ off = s.hband_offsets.arrayZ[b];
      ਬਿਨਾਨਿਸ਼ਾਨੀ n = s.hband_ਵਕਰ_counts.arrayZ[b];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_worst = n;
      ਦੁੱਗਣਾ ਉੱਤਮ_ਵੰਡੋ = (min_x + max_x) * 0.5;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਖੱਬਾ_ਗਿਣਤੀ = n;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < n; ci++) {
	ਦੁੱਗਣਾ ਵੰਡੋ = s.ਵਕਰ_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜਾ_ਗਿਣਤੀ = ci + 1;
	ਜਦੋਂ (ਖੱਬਾ_ਗਿਣਤੀ &&
	       s.ਵਕਰ_infos.arrayZ[s.hband_curves_asc.arrayZ[off + ਖੱਬਾ_ਗਿਣਤੀ - 1]].min_x > ਵੰਡੋ)
	  ਖੱਬਾ_ਗਿਣਤੀ--;
	ਬਿਨਾਨਿਸ਼ਾਨੀ worst = hb_max (ਸੱਜਾ_ਗਿਣਤੀ, ਖੱਬਾ_ਗਿਣਤੀ);
	ਜੇ (worst < ਉੱਤਮ_worst) {
	  ਉੱਤਮ_worst = worst;
	  ਉੱਤਮ_ਵੰਡੋ = ਵੰਡੋ;
	}
      }
      hband_ਵੰਡੋ = quantize (ਉੱਤਮ_ਵੰਡੋ);
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ hdr = ਸਿਰਲੇਖ_len + b;
    ਬਿਨਾਨਿਸ਼ਾਨੀ desc_off = ਸੂਚਕਾਂਕ_ਵਿੱਥ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < s.hband_ਵਕਰ_counts.arrayZ[b]; ci++) {
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].r = encode_ਵਿੱਥ (s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].g = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].b = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].a = 0;
      ਸੂਚਕਾਂਕ_ਵਿੱਥ++;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ asc_off = ਸੂਚਕਾਂਕ_ਵਿੱਥ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < s.hband_ਵਕਰ_counts.arrayZ[b]; ci++) {
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].r = encode_ਵਿੱਥ (s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].g = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].b = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].a = 0;
      ਸੂਚਕਾਂਕ_ਵਿੱਥ++;
    }

    buf[hdr].r = (int16_t) s.hband_ਵਕਰ_counts.arrayZ[b];
    buf[hdr].g = encode_ਵਿੱਥ (desc_off);
    buf[hdr].b = encode_ਵਿੱਥ (asc_off);
    buf[hdr].a = hband_ਵੰਡੋ;
  }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ b = 0; b < ਅੰਕ_vbands; b++)
  {
    int16_t vband_ਵੰਡੋ;
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ off = s.vband_offsets.arrayZ[b];
      ਬਿਨਾਨਿਸ਼ਾਨੀ n = s.vband_ਵਕਰ_counts.arrayZ[b];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_worst = n;
      ਦੁੱਗਣਾ ਉੱਤਮ_ਵੰਡੋ = (min_y + max_y) * 0.5;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਖੱਬਾ_ਗਿਣਤੀ = n;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < n; ci++) {
	ਦੁੱਗਣਾ ਵੰਡੋ = s.ਵਕਰ_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜਾ_ਗਿਣਤੀ = ci + 1;
	ਜਦੋਂ (ਖੱਬਾ_ਗਿਣਤੀ &&
	       s.ਵਕਰ_infos.arrayZ[s.vband_curves_asc.arrayZ[off + ਖੱਬਾ_ਗਿਣਤੀ - 1]].min_y > ਵੰਡੋ)
	  ਖੱਬਾ_ਗਿਣਤੀ--;
	ਬਿਨਾਨਿਸ਼ਾਨੀ worst = hb_max (ਸੱਜਾ_ਗਿਣਤੀ, ਖੱਬਾ_ਗਿਣਤੀ);
	ਜੇ (worst < ਉੱਤਮ_worst) {
	  ਉੱਤਮ_worst = worst;
	  ਉੱਤਮ_ਵੰਡੋ = ਵੰਡੋ;
	}
      }
      vband_ਵੰਡੋ = quantize (ਉੱਤਮ_ਵੰਡੋ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ hdr = ਸਿਰਲੇਖ_len + ਅੰਕ_hbands + b;
    ਬਿਨਾਨਿਸ਼ਾਨੀ desc_off = ਸੂਚਕਾਂਕ_ਵਿੱਥ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < s.vband_ਵਕਰ_counts.arrayZ[b]; ci++) {
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].r = encode_ਵਿੱਥ (s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].g = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].b = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].a = 0;
      ਸੂਚਕਾਂਕ_ਵਿੱਥ++;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ asc_off = ਸੂਚਕਾਂਕ_ਵਿੱਥ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ci = 0; ci < s.vband_ਵਕਰ_counts.arrayZ[b]; ci++) {
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].r = encode_ਵਿੱਥ (s.ਵਕਰ_texel_ਵਿੱਥ.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].g = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].b = 0;
      buf[ਸੂਚਕਾਂਕ_ਵਿੱਥ].a = 0;
      ਸੂਚਕਾਂਕ_ਵਿੱਥ++;
    }

    buf[hdr].r = (int16_t) s.vband_ਵਕਰ_counts.arrayZ[b];
    buf[hdr].g = encode_ਵਿੱਥ (desc_off);
    buf[hdr].b = encode_ਵਿੱਥ (asc_off);
    buf[hdr].a = vband_ਵੰਡੋ;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = ਖਾਲੀ_ਸੂਚਕ;
  ਵਾਪਸ hb_blob_t::recycle_finalize ((ਅੱਖਰ *) buf, buf_ਸਮਰੱਥਾ, needed_ਬਾਈਟਾਂ,
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
hb_gpu_draw_create_or_fail (ਖਾਲੀ)
{
  ਵਾਪਸ hb_object_create<hb_gpu_draw_t> ();
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
  ਵਾਪਸ hb_object_reference (draw);
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
ਖਾਲੀ
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  ਜੇ (!hb_object_should_destroy (draw))
    ਵਾਪਸ;

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
hb_gpu_draw_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_gpu_draw_t     *draw,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
			     ਖਾਲੀ               *ਅੰਕੜੇ,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (draw, key, ਅੰਕੜੇ, destroy, replace);
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
ਖਾਲੀ *
hb_gpu_draw_get_user_ਅੰਕੜੇ (ਅਚਲ hb_gpu_draw_t     *draw,
			     hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (draw, key);
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
hb_gpu_draw_get_funcs (ਅਚਲ hb_gpu_draw_t *draw HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ static_gpu_draw_funcs.get_unconst ();
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
ਖਾਲੀ
hb_gpu_draw_ਸੈੱਟ_scale (hb_gpu_draw_t *draw,
		       ਪੂਰਨ            x_scale,
		       ਪੂਰਨ            y_scale)
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
ਖਾਲੀ
hb_gpu_draw_get_scale (ਅਚਲ hb_gpu_draw_t *draw,
		       ਪੂਰਨ                 *x_scale,
		       ਪੂਰਨ                 *y_scale)
{
  ਜੇ (x_scale) *x_scale = draw->x_scale;
  ਜੇ (y_scale) *y_scale = draw->y_scale;
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
  ਪੂਰਨ x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_ਸੈੱਟ_scale (draw, x_scale, y_scale);

  ਵਾਪਸ hb_font_draw_glyph_or_fail (font, glyph,
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
ਖਾਲੀ
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
ਖਾਲੀ
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->ਸ਼ੁਰੂ_x = draw->ਸ਼ੁਰੂ_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = ਸੱਚ;
  draw->ਅੰਕ_curves = 0;
  draw->success = ਸੱਚ;
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
ਖਾਲੀ
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
ਖਾਲੀ
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw-fragment-glsl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw-fragment-msl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw-fragment-wgsl.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw-fragment-hlsl.hh"

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
ਅਚਲ ਅੱਖਰ *
hb_gpu_draw_shader_ਸਰੋਤ (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  ਚੋਣ (stage) {
  ਹਾਲਤ HB_GPU_SHADER_STAGE_FRAGMENT:
    ਚੋਣ (lang) {
    ਹਾਲਤ HB_GPU_SHADER_LANG_GLSL: ਵਾਪਸ hb_gpu_draw_fragment_glsl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_MSL:  ਵਾਪਸ hb_gpu_draw_fragment_msl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_WGSL: ਵਾਪਸ hb_gpu_draw_fragment_wgsl;
    ਹਾਲਤ HB_GPU_SHADER_LANG_HLSL: ਵਾਪਸ hb_gpu_draw_fragment_hlsl;
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
