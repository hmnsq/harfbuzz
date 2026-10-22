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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-gpu.h"
#अन्तर्भाव "hb-gpu-draw.hh"
#अन्तर्भाव "hb-gpu-cu2qu.hh"
#अन्तर्भाव "hb-machinery.hh"

#अन्तर्भाव <cmath>

/* ---- Accumulator ---- */

स्थिर शून्य
acc_update_extents (hb_gpu_draw_t *g, द्विगुण x, द्विगुण y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

स्थिर शून्य
acc_emit (hb_gpu_draw_t *g,
	  तार्किक contour_start,
	  द्विगुण p1x, द्विगुण p1y,
	  द्विगुण p2x, द्विगुण p2y,
	  द्विगुण p3x, द्विगुण p3y)
{
  int64_t *budget = g->get_budget_remaining ();
  यदि (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    प्रत्यागम;

  यदि (unlikely (g->अंक_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = असत्य;
    प्रत्यागम;
  }

  hb_gpu_curve_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_start};
  यदि (unlikely (!g->curves.ढकला_or_fail (c)))
  {
    g->success = असत्य;
    प्रत्यागम;
  }

  g->अंक_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

स्थिर शून्य
acc_emit_conic (hb_gpu_draw_t *g,
		द्विगुण cx, द्विगुण cy,
		द्विगुण x, द्विगुण y)
{
  यदि (g->current_x == x && g->current_y == y)
    प्रत्यागम;

  तार्किक contour_start = g->need_moveto;
  यदि (g->need_moveto) {
    g->start_x = g->current_x;
    g->start_y = g->current_y;
    g->need_moveto = असत्य;
  }

  acc_emit (g, contour_start,
	    g->current_x, g->current_y, cx, cy, x, y);
}

शून्य
hb_gpu_draw_t::acc_move_to (द्विगुण x, द्विगुण y)
{
  यदि (unlikely (!success))
    प्रत्यागम;

  need_moveto = सत्य;
  current_x = x;
  current_y = y;
}

शून्य
hb_gpu_draw_t::acc_ओळ_to (द्विगुण x, द्विगुण y)
{
  यदि (unlikely (!success))
    प्रत्यागम;

  acc_emit_conic (एतत्, current_x, current_y, x, y);
}

शून्य
hb_gpu_draw_t::acc_conic_to (द्विगुण cx, द्विगुण cy, द्विगुण x, द्विगुण y)
{
  यदि (unlikely (!success))
    प्रत्यागम;

  acc_emit_conic (एतत्, cx, cy, x, y);
}

शून्य
hb_gpu_draw_t::acc_close_path ()
{
  यदि (unlikely (!success))
    प्रत्यागम;

  यदि (!need_moveto &&
      (current_x != start_x || current_y != start_y))
    acc_ओळ_to (start_x, start_y);
}


शून्य
hb_gpu_draw_t::acc_cubic_to (द्विगुण c1x, द्विगुण c1y,
			       द्विगुण c2x, द्विगुण c2y,
			       द्विगुण x, द्विगुण y)
{
  यदि (unlikely (!success))
    प्रत्यागम;

  द्विगुण c0x = current_x, c0y = current_y;

  यदि (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    प्रत्यागम;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  द्विगुण dx = x - c0x, dy = y - c0y;
  द्विगुण len_sq = dx * dx + dy * dy;
  यदि (len_sq > 0.)
  {
    द्विगुण inv = 1.0 / len_sq;
    द्विगुण cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    द्विगुण cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    यदि (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_ओळ_to (x, y);
      प्रत्यागम;
    }
  }

  hb_gpu_cubic_to_quadratics (एतत्,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

स्थिर शून्य
hb_gpu_draw_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     शून्य *data,
		     hb_draw_state_t *st HB_UNUSED,
		     भिन्न to_x, भिन्न to_y,
		     शून्य *user_data HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) data;
  g->acc_close_path ();
  g->acc_move_to ((द्विगुण) to_x, (द्विगुण) to_y);
}

स्थिर शून्य
hb_gpu_draw_ओळ_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     शून्य *data,
		     hb_draw_state_t *st HB_UNUSED,
		     भिन्न to_x, भिन्न to_y,
		     शून्य *user_data HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) data;
  g->acc_ओळ_to ((द्विगुण) to_x, (द्विगुण) to_y);
}

स्थिर शून्य
hb_gpu_draw_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  शून्य *data,
			  hb_draw_state_t *st HB_UNUSED,
			  भिन्न control_x, भिन्न control_y,
			  भिन्न to_x, भिन्न to_y,
			  शून्य *user_data HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) data;
  g->acc_conic_to ((द्विगुण) control_x, (द्विगुण) control_y,
		   (द्विगुण) to_x, (द्विगुण) to_y);
}

स्थिर शून्य
hb_gpu_draw_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		      शून्य *data,
		      hb_draw_state_t *st HB_UNUSED,
		      भिन्न control1_x, भिन्न control1_y,
		      भिन्न control2_x, भिन्न control2_y,
		      भिन्न to_x, भिन्न to_y,
		      शून्य *user_data HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) data;
  g->acc_cubic_to ((द्विगुण) control1_x, (द्विगुण) control1_y,
		   (द्विगुण) control2_x, (द्विगुण) control2_y,
		   (द्विगुण) to_x, (द्विगुण) to_y);
}

स्थिर शून्य
hb_gpu_draw_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
			शून्य *data,
			hb_draw_state_t *st HB_UNUSED,
			शून्य *user_data HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) data;
  g->acc_close_path ();
}

स्थिर hb_bool_t
hb_gpu_draw_set_budget (hb_draw_funcs_t *, शून्य *draw_data,
			int64_t budget, शून्य *)
{
  auto *draw = (hb_gpu_draw_t *) draw_data;
  draw->budget = budget;
  draw->recharge_budget ();
  प्रत्यागम सत्य;
}

स्थिर int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, शून्य *draw_data, शून्य *)
{
  प्रत्यागम ((hb_gpu_draw_t *) draw_data)->budget;
}

स्थिर int64_t *
hb_gpu_draw_get_budget_remaining (hb_draw_funcs_t *, शून्य *draw_data, शून्य *)
{
  प्रत्यागम ((hb_gpu_draw_t *) draw_data)->get_budget_remaining ();
}

स्थिर अन्तःस्थ शून्य free_static_gpu_draw_funcs ();

स्थिर संरचना hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  स्थिर hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func      (funcs, hb_gpu_draw_move_to,      nullptr, nullptr);
    hb_draw_funcs_set_ओळ_to_func      (funcs, hb_gpu_draw_ओळ_to,      nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_gpu_draw_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func     (funcs, hb_gpu_draw_cubic_to,     nullptr, nullptr);
    hb_draw_funcs_set_close_path_func   (funcs, hb_gpu_draw_close_path,   nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_gpu_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_gpu_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_gpu_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    प्रत्यागम funcs;
  }
} static_gpu_draw_funcs;

स्थिर अन्तःस्थ शून्य
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

संरचना hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

स्थिर hb_position_t
clamp_to_hb_position (द्विगुण v)
{
  प्रत्यागम (hb_position_t) hb_clamp (v,
				   (द्विगुण) hb_int_min (hb_position_t),
				   (द्विगुण) hb_int_max (hb_position_t));
}

स्थिर अन्तःस्थ int16_t
quantize (द्विगुण v)
{
  प्रत्यागम (int16_t) round (v * HB_GPU_UNITS_PER_EM);
}

स्थिर अन्तःस्थ int16_t
quantize_down (द्विगुण v)
{
  प्रत्यागम (int16_t) floor (v * HB_GPU_UNITS_PER_EM);
}

स्थिर अन्तःस्थ int16_t
quantize_up (द्विगुण v)
{
  प्रत्यागम (int16_t) ceil (v * HB_GPU_UNITS_PER_EM);
}

स्थिर अन्तःस्थ द्विगुण
dequantize (पूर्णाङ्क v)
{
  प्रत्यागम (द्विगुण) v / HB_GPU_UNITS_PER_EM;
}

स्थिर अन्तःस्थ तार्किक
quantize_down_fits_i16 (द्विगुण v)
{
  द्विगुण q = floor (v * HB_GPU_UNITS_PER_EM);
  प्रत्यागम q >= INT16_MIN &&
	 q <= INT16_MAX;
}

स्थिर अन्तःस्थ तार्किक
quantize_up_fits_i16 (द्विगुण v)
{
  द्विगुण q = ceil (v * HB_GPU_UNITS_PER_EM);
  प्रत्यागम q >= INT16_MIN &&
	 q <= INT16_MAX;
}

स्थिर अन्तःस्थ int16_t
encode_offset (unsigned offset)
{
  प्रत्यागम (int16_t) (offset - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
स्थिर hb_gpu_encode_curve_info_t
encode_curve_info (ध्रुव hb_gpu_curve_t *c)
{
  hb_gpu_encode_curve_info_t info;

  पूर्णाङ्क p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  पूर्णाङ्क p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

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

  प्रत्यागम info;
}

स्थिर शून्य
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  यदि (unlikely (!draw->success) ||
      draw->अंक_curves == 0 ||
      draw->ext_min_x == HUGE_VAL)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->width = 0;
    extents->height = 0;
    प्रत्यागम;
  }

  द्विगुण min_x = floor (draw->ext_min_x);
  द्विगुण min_y = floor (draw->ext_min_y);
  द्विगुण max_x = ceil  (draw->ext_max_x);
  द्विगुण max_y = ceil  (draw->ext_max_y);

  यदि (unlikely (!std::isfinite (min_x) ||
		!std::isfinite (min_y) ||
		!std::isfinite (max_x) ||
		!std::isfinite (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->width = 0;
    extents->height = 0;
    प्रत्यागम;
  }

  extents->x_bearing = clamp_to_hb_position (min_x);
  extents->y_bearing = clamp_to_hb_position (max_y);
  extents->width     = clamp_to_hb_position (max_x - min_x);
  extents->height    = clamp_to_hb_position (min_y - max_y);
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
  यदि (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  यदि (unlikely (!draw->success))
    प्रत्यागम nullptr;

  ध्रुव hb_gpu_curve_t *curves = draw->curves.arrayZ;
  unsigned अंक_curves = draw->curves.length;

  यदि (अंक_curves == 0)
    प्रत्यागम hb_blob_get_empty ();

  hb_gpu_encode_scratch_t &s = draw->scratch;

  s.curve_infos.reset_if_error ();
  s.hband_curve_counts.reset_if_error ();
  s.vband_curve_counts.reset_if_error ();
  s.hband_offsets.reset_if_error ();
  s.vband_offsets.reset_if_error ();
  s.hband_curves.reset_if_error ();
  s.hband_curves_asc.reset_if_error ();
  s.vband_curves.reset_if_error ();
  s.vband_curves_asc.reset_if_error ();
  s.hband_cursors.reset_if_error ();
  s.vband_cursors.reset_if_error ();
  s.curve_texel_offset.reset_if_error ();

  /* Compute per-curve info and extents */
  यदि (unlikely (!s.curve_infos.resize (अंक_curves)))
    प्रत्यागम nullptr;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  यदि (!quantize_down_fits_i16 (draw->ext_min_x) ||
      !quantize_down_fits_i16 (draw->ext_min_y) ||
      !quantize_up_fits_i16 (draw->ext_max_x) ||
      !quantize_up_fits_i16 (draw->ext_max_y))
    प्रत्यागम nullptr;

  int16_t min_x_q = quantize_down (draw->ext_min_x);
  int16_t min_y_q = quantize_down (draw->ext_min_y);
  int16_t max_x_q = quantize_up (draw->ext_max_x);
  int16_t max_y_q = quantize_up (draw->ext_max_y);

  द्विगुण min_x = dequantize (min_x_q);
  द्विगुण min_y = dequantize (min_y_q);
  द्विगुण max_x = dequantize (max_x_q);
  द्विगुण max_y = dequantize (max_y_q);

  प्रति (unsigned i = 0; i < अंक_curves; i++)
    s.curve_infos.arrayZ[i] = encode_curve_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  unsigned अंक_hbands = hb_min (अंक_curves, 16u);
  unsigned अंक_vbands = hb_min (अंक_curves, 16u);
  अंक_hbands = hb_max (अंक_hbands, 1u);
  अंक_vbands = hb_max (अंक_vbands, 1u);

  द्विगुण height = max_y - min_y;
  द्विगुण width  = max_x - min_x;

  यदि (height <= 0) अंक_hbands = 1;
  यदि (width  <= 0) अंक_vbands = 1;

  द्विगुण hband_size = height / अंक_hbands;
  द्विगुण vband_size = width  / अंक_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  स्थिर ध्रुव द्विगुण BAND_EPSILON = 1.0 / 1024;

  यदि (unlikely (!s.hband_curve_counts.resize (अंक_hbands) ||
		!s.vband_curve_counts.resize (अंक_vbands)))
    प्रत्यागम nullptr;

  प्रति (unsigned b = 0; b < अंक_hbands; b++) s.hband_curve_counts.arrayZ[b] = 0;
  प्रति (unsigned b = 0; b < अंक_vbands; b++) s.vband_curve_counts.arrayZ[b] = 0;

  प्रति (unsigned i = 0; i < अंक_curves; i++)
  {
    hb_gpu_encode_curve_info_t &info = s.curve_infos.arrayZ[i];

    यदि (!info.is_horizontal)
    {
      यदि (height > 0) {
	info.hband_lo = (पूर्णाङ्क) floor ((info.min_y - min_y) / hband_size - BAND_EPSILON);
	info.hband_hi = (पूर्णाङ्क) floor ((info.max_y - min_y) / hband_size + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (पूर्णाङ्क) अंक_hbands - 1);
	प्रति (पूर्णाङ्क b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_curve_counts.arrayZ[b]++;
      } अन्यथा {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_curve_counts.arrayZ[0]++;
      }
    }

    यदि (!info.is_vertical)
    {
      यदि (width > 0) {
	info.vband_lo = (पूर्णाङ्क) floor ((info.min_x - min_x) / vband_size - BAND_EPSILON);
	info.vband_hi = (पूर्णाङ्क) floor ((info.max_x - min_x) / vband_size + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (पूर्णाङ्क) अंक_vbands - 1);
	प्रति (पूर्णाङ्क b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_curve_counts.arrayZ[b]++;
      } अन्यथा {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_curve_counts.arrayZ[0]++;
      }
    }
  }

  यदि (unlikely (!s.hband_offsets.resize (अंक_hbands) ||
		!s.vband_offsets.resize (अंक_vbands)))
    प्रत्यागम nullptr;

  unsigned total_hband_indices = 0;
  unsigned total_vband_indices = 0;

  प्रति (unsigned b = 0; b < अंक_hbands; b++) {
    s.hband_offsets.arrayZ[b] = total_hband_indices;
    यदि (unlikely (hb_unsigned_add_overflows (total_hband_indices,
					     s.hband_curve_counts.arrayZ[b],
					     &total_hband_indices)))
      प्रत्यागम nullptr;
  }

  प्रति (unsigned b = 0; b < अंक_vbands; b++) {
    s.vband_offsets.arrayZ[b] = total_vband_indices;
    यदि (unlikely (hb_unsigned_add_overflows (total_vband_indices,
					     s.vband_curve_counts.arrayZ[b],
					     &total_vband_indices)))
      प्रत्यागम nullptr;
  }

  /* Assign curves to bands */
  यदि (unlikely (!s.hband_curves.resize (total_hband_indices) ||
		!s.hband_curves_asc.resize (total_hband_indices) ||
		!s.vband_curves.resize (total_vband_indices) ||
		!s.vband_curves_asc.resize (total_vband_indices) ||
		!s.hband_cursors.resize (अंक_hbands) ||
		!s.vband_cursors.resize (अंक_vbands)))
    प्रत्यागम nullptr;

  प्रति (unsigned b = 0; b < अंक_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  प्रति (unsigned b = 0; b < अंक_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  प्रति (unsigned i = 0; i < अंक_curves; i++)
  {
    ध्रुव hb_gpu_encode_curve_info_t &info = s.curve_infos.arrayZ[i];

    प्रति (पूर्णाङ्क b = info.hband_lo; b <= info.hband_hi; b++) {
      unsigned idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    प्रति (पूर्णाङ्क b = info.vband_lo; b <= info.vband_hi; b++) {
      unsigned idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  ध्रुव hb_gpu_encode_curve_info_t *infos = s.curve_infos.arrayZ;

  प्रति (unsigned b = 0; b < अंक_hbands; b++)
  {
    unsigned off = s.hband_offsets.arrayZ[b];
    unsigned मोजणी = s.hband_curve_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, मोजणी)
      .qsort ([infos] (ध्रुव unsigned &a, ध्रुव unsigned &b) {
	auto av = infos[a].max_x, bv = infos[b].max_x;
	प्रत्यागम (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, मोजणी)
      .qsort ([infos] (ध्रुव unsigned &a, ध्रुव unsigned &b) {
	auto av = infos[a].min_x, bv = infos[b].min_x;
	प्रत्यागम (av > bv) - (av < bv);
      });
  }

  प्रति (unsigned b = 0; b < अंक_vbands; b++)
  {
    unsigned off = s.vband_offsets.arrayZ[b];
    unsigned मोजणी = s.vband_curve_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, मोजणी)
      .qsort ([infos] (ध्रुव unsigned &a, ध्रुव unsigned &b) {
	auto av = infos[a].max_y, bv = infos[b].max_y;
	प्रत्यागम (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, मोजणी)
      .qsort ([infos] (ध्रुव unsigned &a, ध्रुव unsigned &b) {
	auto av = infos[a].min_y, bv = infos[b].min_y;
	प्रत्यागम (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  unsigned total_curve_indices;
  यदि (unlikely (hb_unsigned_add_overflows (total_hband_indices,
					   total_vband_indices,
					   &total_curve_indices) ||
		hb_unsigned_mul_overflows (total_curve_indices,
					   2,
					   &total_curve_indices)))
    प्रत्यागम nullptr;

  unsigned header_len = 2;

  unsigned अंक_contour_breaks = 0;
  प्रति (unsigned i = 0; i + 1 < अंक_curves; i++)
    यदि (curves[i + 1].contour_start)
      अंक_contour_breaks++;

  unsigned curve_data_len;
  यदि (unlikely (hb_unsigned_add_overflows (अंक_curves,
					   अंक_contour_breaks,
					   &curve_data_len) ||
		hb_unsigned_add_overflows (curve_data_len,
					   1,
					   &curve_data_len)))
    प्रत्यागम nullptr;

  unsigned band_headers_len;
  यदि (unlikely (hb_unsigned_add_overflows (अंक_hbands,
					   अंक_vbands,
					   &band_headers_len)))
    प्रत्यागम nullptr;

  unsigned total_len = header_len;
  यदि (unlikely (hb_unsigned_add_overflows (total_len,
					   band_headers_len,
					   &total_len) ||
		hb_unsigned_add_overflows (total_len,
					   total_curve_indices,
					   &total_len) ||
		hb_unsigned_add_overflows (total_len,
					   curve_data_len,
					   &total_len)))
    प्रत्यागम nullptr;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  यदि (total_len > (unsigned) UINT16_MAX + 1u)
    प्रत्यागम nullptr;

  /* Allocate or reuse encode buffer */
  unsigned needed_bytes;
  यदि (unlikely (hb_unsigned_mul_overflows (total_len,
					   परिमाण (hb_gpu_texel_t),
					   &needed_bytes)))
    प्रत्यागम nullptr;
  unsigned buf_capacity = 0;
  वर्ण *replaced_recycled_buf = nullptr;
  वर्ण *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_bytes,
					&buf_capacity, &replaced_recycled_buf);
  यदि (unlikely (!buf_raw))
    प्रत्यागम nullptr;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (शून्य *) buf_raw;

  unsigned curve_data_offset = header_len;
  यदि (unlikely (hb_unsigned_add_overflows (curve_data_offset,
					   band_headers_len,
					   &curve_data_offset) ||
		hb_unsigned_add_overflows (curve_data_offset,
					   total_curve_indices,
					   &curve_data_offset)))
  {
    hb_blob_t::recycle_abort ((वर्ण *) buf, draw->recycled_blob);
    प्रत्यागम nullptr;
  }

  /* Pack header */
  buf[0].r = min_x_q;
  buf[0].g = min_y_q;
  buf[0].b = max_x_q;
  buf[0].a = max_y_q;
  buf[1].r = (int16_t) अंक_hbands;
  buf[1].g = (int16_t) अंक_vbands;
  buf[1].b = (int16_t) hb_clamp (draw->x_scale, -32768, 32767);
  buf[1].a = (int16_t) hb_clamp (draw->y_scale, -32768, 32767);

  /* Pack curve data with shared endpoints */
  यदि (unlikely (!s.curve_texel_offset.resize (अंक_curves)))
  {
    hb_blob_t::recycle_abort ((वर्ण *) buf, draw->recycled_blob);
    प्रत्यागम nullptr;
  }

  unsigned texel = curve_data_offset;

  प्रति (unsigned i = 0; i < अंक_curves; i++)
  {
    तार्किक contour_start = curves[i].contour_start;

    यदि (contour_start) {
      s.curve_texel_offset.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } अन्यथा {
      s.curve_texel_offset.arrayZ[i] = texel - 1;
    }

    तार्किक has_next = i + 1 < अंक_curves &&
		    !curves[i + 1].contour_start;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    यदि (has_next) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } अन्यथा {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  unsigned index_offset = header_len + band_headers_len;

  प्रति (unsigned b = 0; b < अंक_hbands; b++)
  {
    int16_t hband_split;
    {
      unsigned off = s.hband_offsets.arrayZ[b];
      unsigned n = s.hband_curve_counts.arrayZ[b];
      unsigned best_worst = n;
      द्विगुण best_split = (min_x + max_x) * 0.5;
      unsigned डावा_मोजणी = n;
      प्रति (unsigned ci = 0; ci < n; ci++) {
	द्विगुण split = s.curve_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	unsigned उजवा_मोजणी = ci + 1;
	यावत् (डावा_मोजणी &&
	       s.curve_infos.arrayZ[s.hband_curves_asc.arrayZ[off + डावा_मोजणी - 1]].min_x > split)
	  डावा_मोजणी--;
	unsigned worst = hb_max (उजवा_मोजणी, डावा_मोजणी);
	यदि (worst < best_worst) {
	  best_worst = worst;
	  best_split = split;
	}
      }
      hband_split = quantize (best_split);
    }
    unsigned hdr = header_len + b;
    unsigned desc_off = index_offset;

    प्रति (unsigned ci = 0; ci < s.hband_curve_counts.arrayZ[b]; ci++) {
      buf[index_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[index_offset].g = 0;
      buf[index_offset].b = 0;
      buf[index_offset].a = 0;
      index_offset++;
    }

    unsigned asc_off = index_offset;

    प्रति (unsigned ci = 0; ci < s.hband_curve_counts.arrayZ[b]; ci++) {
      buf[index_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[index_offset].g = 0;
      buf[index_offset].b = 0;
      buf[index_offset].a = 0;
      index_offset++;
    }

    buf[hdr].r = (int16_t) s.hband_curve_counts.arrayZ[b];
    buf[hdr].g = encode_offset (desc_off);
    buf[hdr].b = encode_offset (asc_off);
    buf[hdr].a = hband_split;
  }

  प्रति (unsigned b = 0; b < अंक_vbands; b++)
  {
    int16_t vband_split;
    {
      unsigned off = s.vband_offsets.arrayZ[b];
      unsigned n = s.vband_curve_counts.arrayZ[b];
      unsigned best_worst = n;
      द्विगुण best_split = (min_y + max_y) * 0.5;
      unsigned डावा_मोजणी = n;
      प्रति (unsigned ci = 0; ci < n; ci++) {
	द्विगुण split = s.curve_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	unsigned उजवा_मोजणी = ci + 1;
	यावत् (डावा_मोजणी &&
	       s.curve_infos.arrayZ[s.vband_curves_asc.arrayZ[off + डावा_मोजणी - 1]].min_y > split)
	  डावा_मोजणी--;
	unsigned worst = hb_max (उजवा_मोजणी, डावा_मोजणी);
	यदि (worst < best_worst) {
	  best_worst = worst;
	  best_split = split;
	}
      }
      vband_split = quantize (best_split);
    }

    unsigned hdr = header_len + अंक_hbands + b;
    unsigned desc_off = index_offset;

    प्रति (unsigned ci = 0; ci < s.vband_curve_counts.arrayZ[b]; ci++) {
      buf[index_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[index_offset].g = 0;
      buf[index_offset].b = 0;
      buf[index_offset].a = 0;
      index_offset++;
    }

    unsigned asc_off = index_offset;

    प्रति (unsigned ci = 0; ci < s.vband_curve_counts.arrayZ[b]; ci++) {
      buf[index_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[index_offset].g = 0;
      buf[index_offset].b = 0;
      buf[index_offset].a = 0;
      index_offset++;
    }

    buf[hdr].r = (int16_t) s.vband_curve_counts.arrayZ[b];
    buf[hdr].g = encode_offset (desc_off);
    buf[hdr].b = encode_offset (asc_off);
    buf[hdr].a = vband_split;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = nullptr;
  प्रत्यागम hb_blob_t::recycle_finalize ((वर्ण *) buf, buf_capacity, needed_bytes,
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
hb_gpu_draw_create_or_fail (शून्य)
{
  प्रत्यागम hb_object_create<hb_gpu_draw_t> ();
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
  प्रत्यागम hb_object_reference (draw);
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
शून्य
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  यदि (!hb_object_should_destroy (draw))
    प्रत्यागम;

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
hb_gpu_draw_set_user_data (hb_gpu_draw_t     *draw,
			     hb_user_data_किल्ली_t *key,
			     शून्य               *data,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  प्रत्यागम hb_object_set_user_data (draw, key, data, destroy, replace);
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
शून्य *
hb_gpu_draw_get_user_data (ध्रुव hb_gpu_draw_t     *draw,
			     hb_user_data_किल्ली_t *key)
{
  प्रत्यागम hb_object_get_user_data (draw, key);
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
hb_gpu_draw_get_funcs (ध्रुव hb_gpu_draw_t *draw HB_UNUSED)
{
  प्रत्यागम static_gpu_draw_funcs.get_unconst ();
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
शून्य
hb_gpu_draw_set_scale (hb_gpu_draw_t *draw,
		       पूर्णाङ्क            x_scale,
		       पूर्णाङ्क            y_scale)
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
शून्य
hb_gpu_draw_get_scale (ध्रुव hb_gpu_draw_t *draw,
		       पूर्णाङ्क                 *x_scale,
		       पूर्णाङ्क                 *y_scale)
{
  यदि (x_scale) *x_scale = draw->x_scale;
  यदि (y_scale) *y_scale = draw->y_scale;
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
  पूर्णाङ्क x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_set_scale (draw, x_scale, y_scale);

  प्रत्यागम hb_font_draw_glyph_or_fail (font, glyph,
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
शून्य
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
शून्य
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->start_x = draw->start_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = सत्य;
  draw->अंक_curves = 0;
  draw->success = सत्य;
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
शून्य
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
शून्य
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#अन्तर्भाव "hb-gpu-draw-fragment-glsl.hh"
#अन्तर्भाव "hb-gpu-draw-fragment-msl.hh"
#अन्तर्भाव "hb-gpu-draw-fragment-wgsl.hh"
#अन्तर्भाव "hb-gpu-draw-fragment-hlsl.hh"

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
ध्रुव वर्ण *
hb_gpu_draw_shader_source (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  विकल्प (stage) {
  पक्ष HB_GPU_SHADER_STAGE_FRAGMENT:
    विकल्प (lang) {
    पक्ष HB_GPU_SHADER_LANG_GLSL: प्रत्यागम hb_gpu_draw_fragment_glsl;
    पक्ष HB_GPU_SHADER_LANG_MSL:  प्रत्यागम hb_gpu_draw_fragment_msl;
    पक्ष HB_GPU_SHADER_LANG_WGSL: प्रत्यागम hb_gpu_draw_fragment_wgsl;
    पक्ष HB_GPU_SHADER_LANG_HLSL: प्रत्यागम hb_gpu_draw_fragment_hlsl;
    पक्ष HB_GPU_SHADER_LANG_INVALID:
    स्वभाव: प्रत्यागम nullptr;
    }
  पक्ष HB_GPU_SHADER_STAGE_VERTEX:
    विकल्प (lang) {
    पक्ष HB_GPU_SHADER_LANG_GLSL:
    पक्ष HB_GPU_SHADER_LANG_MSL:
    पक्ष HB_GPU_SHADER_LANG_WGSL:
    पक्ष HB_GPU_SHADER_LANG_HLSL: प्रत्यागम "";
    पक्ष HB_GPU_SHADER_LANG_INVALID:
    स्वभाव: प्रत्यागम nullptr;
    }
  स्वभाव:
    प्रत्यागम nullptr;
  }
}
