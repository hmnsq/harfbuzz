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

#समावेश "hb.hh"

#समावेश "hb-gpu.h"
#समावेश "hb-gpu-draw.hh"
#समावेश "hb-gpu-cu2qu.hh"
#समावेश "hb-machinery.hh"

#समावेश <cmath>

/* ---- Accumulator ---- */

अटल व्योम
acc_update_extents (hb_gpu_draw_t *g, द्विगुन x, द्विगुन y)
{
  g->ext_min_x = hb_min (g->ext_min_x, x);
  g->ext_min_y = hb_min (g->ext_min_y, y);
  g->ext_max_x = hb_max (g->ext_max_x, x);
  g->ext_max_y = hb_max (g->ext_max_y, y);
}

अटल व्योम
acc_emit (hb_gpu_draw_t *g,
	  bool contour_start,
	  द्विगुन p1x, द्विगुन p1y,
	  द्विगुन p2x, द्विगुन p2y,
	  द्विगुन p3x, द्विगुन p3y)
{
  int64_t *budget = g->get_budget_remaining ();
  अगर (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1)))
    वापस;

  अगर (unlikely (g->अंक_curves >= HB_GPU_DRAW_MAX_CURVES))
  {
    g->success = false;
    वापस;
  }

  hb_gpu_curve_t c = {p1x, p1y, p2x, p2y, p3x, p3y, contour_start};
  अगर (unlikely (!g->curves.धकेलो_or_fail (c)))
  {
    g->success = false;
    वापस;
  }

  g->अंक_curves++;
  g->current_x = p3x;
  g->current_y = p3y;

  acc_update_extents (g, p1x, p1y);
  acc_update_extents (g, p2x, p2y);
  acc_update_extents (g, p3x, p3y);
}

अटल व्योम
acc_emit_conic (hb_gpu_draw_t *g,
		द्विगुन cx, द्विगुन cy,
		द्विगुन x, द्विगुन y)
{
  अगर (g->current_x == x && g->current_y == y)
    वापस;

  bool contour_start = g->need_moveto;
  अगर (g->need_moveto) {
    g->start_x = g->current_x;
    g->start_y = g->current_y;
    g->need_moveto = false;
  }

  acc_emit (g, contour_start,
	    g->current_x, g->current_y, cx, cy, x, y);
}

व्योम
hb_gpu_draw_t::acc_move_to (द्विगुन x, द्विगुन y)
{
  अगर (unlikely (!success))
    वापस;

  need_moveto = true;
  current_x = x;
  current_y = y;
}

व्योम
hb_gpu_draw_t::acc_पंक्ति_to (द्विगुन x, द्विगुन y)
{
  अगर (unlikely (!success))
    वापस;

  acc_emit_conic (यह, current_x, current_y, x, y);
}

व्योम
hb_gpu_draw_t::acc_conic_to (द्विगुन cx, द्विगुन cy, द्विगुन x, द्विगुन y)
{
  अगर (unlikely (!success))
    वापस;

  acc_emit_conic (यह, cx, cy, x, y);
}

व्योम
hb_gpu_draw_t::acc_close_path ()
{
  अगर (unlikely (!success))
    वापस;

  अगर (!need_moveto &&
      (current_x != start_x || current_y != start_y))
    acc_पंक्ति_to (start_x, start_y);
}


व्योम
hb_gpu_draw_t::acc_cubic_to (द्विगुन c1x, द्विगुन c1y,
			       द्विगुन c2x, द्विगुन c2y,
			       द्विगुन x, द्विगुन y)
{
  अगर (unlikely (!success))
    वापस;

  द्विगुन c0x = current_x, c0y = current_y;

  अगर (c0x == x && c0y == y &&
      c1x == x && c1y == y &&
      c2x == x && c2y == y)
    वापस;

  /* Degenerate cubic: all control points are collinear.
   * Emit as a line instead of running cu2qu subdivision. */
  द्विगुन dx = x - c0x, dy = y - c0y;
  द्विगुन len_sq = dx * dx + dy * dy;
  अगर (len_sq > 0.)
  {
    द्विगुन inv = 1.0 / len_sq;
    द्विगुन cross1 = (c1x - c0x) * dy - (c1y - c0y) * dx;
    द्विगुन cross2 = (c2x - c0x) * dy - (c2y - c0y) * dx;
    अगर (cross1 * cross1 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE &&
	cross2 * cross2 * inv <= HB_GPU_CU2QU_TOLERANCE * HB_GPU_CU2QU_TOLERANCE)
    {
      acc_पंक्ति_to (x, y);
      वापस;
    }
  }

  hb_gpu_cubic_to_quadratics (यह,
			      {c0x, c0y}, {c1x, c1y},
			      {c2x, c2y}, {x, y},
			      HB_GPU_CU2QU_TOLERANCE, 0);
}


/* ---- Draw funcs ---- */

अटल व्योम
hb_gpu_draw_move_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     व्योम *तथ्य,
		     hb_draw_state_t *st HB_UNUSED,
		     भग्न to_x, भग्न to_y,
		     व्योम *user_तथ्य HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) तथ्य;
  g->acc_close_path ();
  g->acc_move_to ((द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_gpu_draw_पंक्ति_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		     व्योम *तथ्य,
		     hb_draw_state_t *st HB_UNUSED,
		     भग्न to_x, भग्न to_y,
		     व्योम *user_तथ्य HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) तथ्य;
  g->acc_पंक्ति_to ((द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_gpu_draw_quadratic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
			  व्योम *तथ्य,
			  hb_draw_state_t *st HB_UNUSED,
			  भग्न control_x, भग्न control_y,
			  भग्न to_x, भग्न to_y,
			  व्योम *user_तथ्य HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) तथ्य;
  g->acc_conic_to ((द्विगुन) control_x, (द्विगुन) control_y,
		   (द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_gpu_draw_cubic_to (hb_draw_funcs_t *dfuncs HB_UNUSED,
		      व्योम *तथ्य,
		      hb_draw_state_t *st HB_UNUSED,
		      भग्न control1_x, भग्न control1_y,
		      भग्न control2_x, भग्न control2_y,
		      भग्न to_x, भग्न to_y,
		      व्योम *user_तथ्य HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) तथ्य;
  g->acc_cubic_to ((द्विगुन) control1_x, (द्विगुन) control1_y,
		   (द्विगुन) control2_x, (द्विगुन) control2_y,
		   (द्विगुन) to_x, (द्विगुन) to_y);
}

अटल व्योम
hb_gpu_draw_close_path (hb_draw_funcs_t *dfuncs HB_UNUSED,
			व्योम *तथ्य,
			hb_draw_state_t *st HB_UNUSED,
			व्योम *user_तथ्य HB_UNUSED)
{
  hb_gpu_draw_t *g = (hb_gpu_draw_t *) तथ्य;
  g->acc_close_path ();
}

अटल hb_bool_t
hb_gpu_draw_set_budget (hb_draw_funcs_t *, व्योम *draw_तथ्य,
			int64_t budget, व्योम *)
{
  स्वतः *draw = (hb_gpu_draw_t *) draw_तथ्य;
  draw->budget = budget;
  draw->recharge_budget ();
  वापस true;
}

अटल int64_t
hb_gpu_draw_get_budget (hb_draw_funcs_t *, व्योम *draw_तथ्य, व्योम *)
{
  वापस ((hb_gpu_draw_t *) draw_तथ्य)->budget;
}

अटल int64_t *
hb_gpu_draw_get_budget_remaining (hb_draw_funcs_t *, व्योम *draw_तथ्य, व्योम *)
{
  वापस ((hb_gpu_draw_t *) draw_तथ्य)->get_budget_remaining ();
}

अटल अंतरभूत व्योम free_static_gpu_draw_funcs ();

अटल काष्ठा hb_gpu_draw_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_gpu_draw_funcs_lazy_loader_t>
{
  अटल hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_set_move_to_func      (funcs, hb_gpu_draw_move_to,      nullptr, nullptr);
    hb_draw_funcs_set_पंक्ति_to_func      (funcs, hb_gpu_draw_पंक्ति_to,      nullptr, nullptr);
    hb_draw_funcs_set_quadratic_to_func (funcs, hb_gpu_draw_quadratic_to, nullptr, nullptr);
    hb_draw_funcs_set_cubic_to_func     (funcs, hb_gpu_draw_cubic_to,     nullptr, nullptr);
    hb_draw_funcs_set_close_path_func   (funcs, hb_gpu_draw_close_path,   nullptr, nullptr);
    hb_draw_funcs_set_set_budget_func (funcs, hb_gpu_draw_set_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_func (funcs, hb_gpu_draw_get_budget, nullptr, nullptr);
    hb_draw_funcs_set_get_budget_remaining_func (funcs, hb_gpu_draw_get_budget_remaining, nullptr, nullptr);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_gpu_draw_funcs);

    वापस funcs;
  }
} static_gpu_draw_funcs;

अटल अंतरभूत व्योम
free_static_gpu_draw_funcs ()
{
  static_gpu_draw_funcs.free_instance ();
}


/* ---- Encode ---- */

काष्ठा hb_gpu_texel_t
{
  int16_t r, g, b, a;
};

अटल hb_position_t
clamp_to_hb_position (द्विगुन v)
{
  वापस (hb_position_t) hb_clamp (v,
				   (द्विगुन) hb_int_min (hb_position_t),
				   (द्विगुन) hb_int_max (hb_position_t));
}

अटल अंतरभूत int16_t
quantize (द्विगुन v)
{
  वापस (int16_t) round (v * HB_GPU_UNITS_PER_EM);
}

अटल अंतरभूत int16_t
quantize_down (द्विगुन v)
{
  वापस (int16_t) न्यूनमान (v * HB_GPU_UNITS_PER_EM);
}

अटल अंतरभूत int16_t
quantize_up (द्विगुन v)
{
  वापस (int16_t) उच्चमान (v * HB_GPU_UNITS_PER_EM);
}

अटल अंतरभूत द्विगुन
dequantize (पूर्णांक v)
{
  वापस (द्विगुन) v / HB_GPU_UNITS_PER_EM;
}

अटल अंतरभूत bool
quantize_down_fits_i16 (द्विगुन v)
{
  द्विगुन q = न्यूनमान (v * HB_GPU_UNITS_PER_EM);
  वापस q >= INT16_MIN &&
	 q <= INT16_MAX;
}

अटल अंतरभूत bool
quantize_up_fits_i16 (द्विगुन v)
{
  द्विगुन q = उच्चमान (v * HB_GPU_UNITS_PER_EM);
  वापस q >= INT16_MIN &&
	 q <= INT16_MAX;
}

अटल अंतरभूत int16_t
encode_offset (अचिन्हित offset)
{
  वापस (int16_t) (offset - 32768u);
}

/* Note: the bounds are computed from the *quantized* control points,
 * because that is what the shader sees.  Band membership derived from
 * the unquantized bounds can leave a curve out of the band that its
 * quantized extent reaches into; fragments in that sliver then never
 * see the curve, get the wrong winding number, and render as a thin
 * black line through the glyph. */
अटल hb_gpu_encode_curve_info_t
encode_curve_info (स्थिर hb_gpu_curve_t *c)
{
  hb_gpu_encode_curve_info_t info;

  पूर्णांक p1x = quantize (c->p1x), p2x = quantize (c->p2x), p3x = quantize (c->p3x);
  पूर्णांक p1y = quantize (c->p1y), p2y = quantize (c->p2y), p3y = quantize (c->p3y);

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

  वापस info;
}

अटल व्योम
_hb_gpu_draw_get_extents (hb_gpu_draw_t      *draw,
			  hb_glyph_extents_t *extents)
{
  अगर (unlikely (!draw->success) ||
      draw->अंक_curves == 0 ||
      draw->ext_min_x == विशाल_मान)
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->width = 0;
    extents->height = 0;
    वापस;
  }

  द्विगुन min_x = न्यूनमान (draw->ext_min_x);
  द्विगुन min_y = न्यूनमान (draw->ext_min_y);
  द्विगुन max_x = उच्चमान  (draw->ext_max_x);
  द्विगुन max_y = उच्चमान  (draw->ext_max_y);

  अगर (unlikely (!std::है_सीमित (min_x) ||
		!std::है_सीमित (min_y) ||
		!std::है_सीमित (max_x) ||
		!std::है_सीमित (max_y)))
  {
    extents->x_bearing = 0;
    extents->y_bearing = 0;
    extents->width = 0;
    extents->height = 0;
    वापस;
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
  अगर (extents)
    _hb_gpu_draw_get_extents (draw, extents);
  HB_SCOPE_GUARD (hb_gpu_draw_clear (draw));

  अगर (unlikely (!draw->success))
    वापस nullptr;

  स्थिर hb_gpu_curve_t *curves = draw->curves.arrayZ;
  अचिन्हित अंक_curves = draw->curves.लंबाई;

  अगर (अंक_curves == 0)
    वापस hb_blob_get_empty ();

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
  अगर (unlikely (!s.curve_infos.resize (अंक_curves)))
    वापस nullptr;

  /* Validate the extents fit in i16 before quantizing: the casts in
   * quantize_down()/quantize_up() are undefined for out-of-range
   * values, and every later i16 conversion of a curve point is bounded
   * by these extents. */
  अगर (!quantize_down_fits_i16 (draw->ext_min_x) ||
      !quantize_down_fits_i16 (draw->ext_min_y) ||
      !quantize_up_fits_i16 (draw->ext_max_x) ||
      !quantize_up_fits_i16 (draw->ext_max_y))
    वापस nullptr;

  int16_t min_x_q = quantize_down (draw->ext_min_x);
  int16_t min_y_q = quantize_down (draw->ext_min_y);
  int16_t max_x_q = quantize_up (draw->ext_max_x);
  int16_t max_y_q = quantize_up (draw->ext_max_y);

  द्विगुन min_x = dequantize (min_x_q);
  द्विगुन min_y = dequantize (min_y_q);
  द्विगुन max_x = dequantize (max_x_q);
  द्विगुन max_y = dequantize (max_y_q);

  क्रम (अचिन्हित i = 0; i < अंक_curves; i++)
    s.curve_infos.arrayZ[i] = encode_curve_info (&curves[i]);

  /* Choose number of bands (capped at 16 per Slug paper) */
  अचिन्हित अंक_hbands = hb_min (अंक_curves, 16u);
  अचिन्हित अंक_vbands = hb_min (अंक_curves, 16u);
  अंक_hbands = hb_max (अंक_hbands, 1u);
  अंक_vbands = hb_max (अंक_vbands, 1u);

  द्विगुन height = max_y - min_y;
  द्विगुन width  = max_x - min_x;

  अगर (height <= 0) अंक_hbands = 1;
  अगर (width  <= 0) अंक_vbands = 1;

  द्विगुन hband_आकार = height / अंक_hbands;
  द्विगुन vband_आकार = width  / अंक_vbands;

  /* The shader recomputes the band index from the fragment position in
   * float32, this code does it in double.  Widen the band range of each
   * curve by a hair so the two cannot disagree at a band boundary. */
  अटल स्थिर द्विगुन BAND_EPSILON = 1.0 / 1024;

  अगर (unlikely (!s.hband_curve_counts.resize (अंक_hbands) ||
		!s.vband_curve_counts.resize (अंक_vbands)))
    वापस nullptr;

  क्रम (अचिन्हित b = 0; b < अंक_hbands; b++) s.hband_curve_counts.arrayZ[b] = 0;
  क्रम (अचिन्हित b = 0; b < अंक_vbands; b++) s.vband_curve_counts.arrayZ[b] = 0;

  क्रम (अचिन्हित i = 0; i < अंक_curves; i++)
  {
    hb_gpu_encode_curve_info_t &info = s.curve_infos.arrayZ[i];

    अगर (!info.is_horizontal)
    {
      अगर (height > 0) {
	info.hband_lo = (पूर्णांक) न्यूनमान ((info.min_y - min_y) / hband_आकार - BAND_EPSILON);
	info.hband_hi = (पूर्णांक) न्यूनमान ((info.max_y - min_y) / hband_आकार + BAND_EPSILON);
	info.hband_lo = hb_max (info.hband_lo, 0);
	info.hband_hi = hb_min (info.hband_hi, (पूर्णांक) अंक_hbands - 1);
	क्रम (पूर्णांक b = info.hband_lo; b <= info.hband_hi; b++)
	  s.hband_curve_counts.arrayZ[b]++;
      } अन्यथा {
	info.hband_lo = 0;
	info.hband_hi = 0;
	s.hband_curve_counts.arrayZ[0]++;
      }
    }

    अगर (!info.is_vertical)
    {
      अगर (width > 0) {
	info.vband_lo = (पूर्णांक) न्यूनमान ((info.min_x - min_x) / vband_आकार - BAND_EPSILON);
	info.vband_hi = (पूर्णांक) न्यूनमान ((info.max_x - min_x) / vband_आकार + BAND_EPSILON);
	info.vband_lo = hb_max (info.vband_lo, 0);
	info.vband_hi = hb_min (info.vband_hi, (पूर्णांक) अंक_vbands - 1);
	क्रम (पूर्णांक b = info.vband_lo; b <= info.vband_hi; b++)
	  s.vband_curve_counts.arrayZ[b]++;
      } अन्यथा {
	info.vband_lo = 0;
	info.vband_hi = 0;
	s.vband_curve_counts.arrayZ[0]++;
      }
    }
  }

  अगर (unlikely (!s.hband_offsets.resize (अंक_hbands) ||
		!s.vband_offsets.resize (अंक_vbands)))
    वापस nullptr;

  अचिन्हित योग_hband_indices = 0;
  अचिन्हित योग_vband_indices = 0;

  क्रम (अचिन्हित b = 0; b < अंक_hbands; b++) {
    s.hband_offsets.arrayZ[b] = योग_hband_indices;
    अगर (unlikely (hb_unsigned_add_overflows (योग_hband_indices,
					     s.hband_curve_counts.arrayZ[b],
					     &योग_hband_indices)))
      वापस nullptr;
  }

  क्रम (अचिन्हित b = 0; b < अंक_vbands; b++) {
    s.vband_offsets.arrayZ[b] = योग_vband_indices;
    अगर (unlikely (hb_unsigned_add_overflows (योग_vband_indices,
					     s.vband_curve_counts.arrayZ[b],
					     &योग_vband_indices)))
      वापस nullptr;
  }

  /* Assign curves to bands */
  अगर (unlikely (!s.hband_curves.resize (योग_hband_indices) ||
		!s.hband_curves_asc.resize (योग_hband_indices) ||
		!s.vband_curves.resize (योग_vband_indices) ||
		!s.vband_curves_asc.resize (योग_vband_indices) ||
		!s.hband_cursors.resize (अंक_hbands) ||
		!s.vband_cursors.resize (अंक_vbands)))
    वापस nullptr;

  क्रम (अचिन्हित b = 0; b < अंक_hbands; b++) s.hband_cursors.arrayZ[b] = s.hband_offsets.arrayZ[b];
  क्रम (अचिन्हित b = 0; b < अंक_vbands; b++) s.vband_cursors.arrayZ[b] = s.vband_offsets.arrayZ[b];

  क्रम (अचिन्हित i = 0; i < अंक_curves; i++)
  {
    स्थिर hb_gpu_encode_curve_info_t &info = s.curve_infos.arrayZ[i];

    क्रम (पूर्णांक b = info.hband_lo; b <= info.hband_hi; b++) {
      अचिन्हित idx = s.hband_cursors.arrayZ[b]++;
      s.hband_curves.arrayZ[idx] = i;
      s.hband_curves_asc.arrayZ[idx] = i;
    }

    क्रम (पूर्णांक b = info.vband_lo; b <= info.vband_hi; b++) {
      अचिन्हित idx = s.vband_cursors.arrayZ[b]++;
      s.vband_curves.arrayZ[idx] = i;
      s.vband_curves_asc.arrayZ[idx] = i;
    }
  }

  /* Sort: descending by max, ascending by min */
  स्थिर hb_gpu_encode_curve_info_t *infos = s.curve_infos.arrayZ;

  क्रम (अचिन्हित b = 0; b < अंक_hbands; b++)
  {
    अचिन्हित off = s.hband_offsets.arrayZ[b];
    अचिन्हित गिनती = s.hband_curve_counts.arrayZ[b];
    s.hband_curves.as_array ().sub_array (off, गिनती)
      .क्विक ([infos] (स्थिर अचिन्हित &a, स्थिर अचिन्हित &b) {
	स्वतः av = infos[a].max_x, bv = infos[b].max_x;
	वापस (av < bv) - (av > bv);
      });
    s.hband_curves_asc.as_array ().sub_array (off, गिनती)
      .क्विक ([infos] (स्थिर अचिन्हित &a, स्थिर अचिन्हित &b) {
	स्वतः av = infos[a].min_x, bv = infos[b].min_x;
	वापस (av > bv) - (av < bv);
      });
  }

  क्रम (अचिन्हित b = 0; b < अंक_vbands; b++)
  {
    अचिन्हित off = s.vband_offsets.arrayZ[b];
    अचिन्हित गिनती = s.vband_curve_counts.arrayZ[b];
    s.vband_curves.as_array ().sub_array (off, गिनती)
      .क्विक ([infos] (स्थिर अचिन्हित &a, स्थिर अचिन्हित &b) {
	स्वतः av = infos[a].max_y, bv = infos[b].max_y;
	वापस (av < bv) - (av > bv);
      });
    s.vband_curves_asc.as_array ().sub_array (off, गिनती)
      .क्विक ([infos] (स्थिर अचिन्हित &a, स्थिर अचिन्हित &b) {
	स्वतः av = infos[a].min_y, bv = infos[b].min_y;
	वापस (av > bv) - (av < bv);
      });
  }

  /* Compute sizes */
  अचिन्हित योग_curve_indices;
  अगर (unlikely (hb_unsigned_add_overflows (योग_hband_indices,
					   योग_vband_indices,
					   &योग_curve_indices) ||
		hb_unsigned_mul_overflows (योग_curve_indices,
					   2,
					   &योग_curve_indices)))
    वापस nullptr;

  अचिन्हित header_len = 2;

  अचिन्हित अंक_contour_breaks = 0;
  क्रम (अचिन्हित i = 0; i + 1 < अंक_curves; i++)
    अगर (curves[i + 1].contour_start)
      अंक_contour_breaks++;

  अचिन्हित curve_तथ्य_len;
  अगर (unlikely (hb_unsigned_add_overflows (अंक_curves,
					   अंक_contour_breaks,
					   &curve_तथ्य_len) ||
		hb_unsigned_add_overflows (curve_तथ्य_len,
					   1,
					   &curve_तथ्य_len)))
    वापस nullptr;

  अचिन्हित band_headers_len;
  अगर (unlikely (hb_unsigned_add_overflows (अंक_hbands,
					   अंक_vbands,
					   &band_headers_len)))
    वापस nullptr;

  अचिन्हित योग_len = header_len;
  अगर (unlikely (hb_unsigned_add_overflows (योग_len,
					   band_headers_len,
					   &योग_len) ||
		hb_unsigned_add_overflows (योग_len,
					   योग_curve_indices,
					   &योग_len) ||
		hb_unsigned_add_overflows (योग_len,
					   curve_तथ्य_len,
					   &योग_len)))
    वापस nullptr;

  /* Validate fits in uint16 offsets (stored as int16 with bias) */
  अगर (योग_len > (अचिन्हित) UINT16_MAX + 1u)
    वापस nullptr;

  /* Allocate or reuse encode buffer */
  अचिन्हित needed_bytes;
  अगर (unlikely (hb_unsigned_mul_overflows (योग_len,
					   माप (hb_gpu_texel_t),
					   &needed_bytes)))
    वापस nullptr;
  अचिन्हित buf_capacity = 0;
  अक्षर *replaced_recycled_buf = nullptr;
  अक्षर *buf_raw = hb_blob_t::recycle_acquire (draw->recycled_blob, needed_bytes,
					&buf_capacity, &replaced_recycled_buf);
  अगर (unlikely (!buf_raw))
    वापस nullptr;
  hb_gpu_texel_t *buf = (hb_gpu_texel_t *) (व्योम *) buf_raw;

  अचिन्हित curve_तथ्य_offset = header_len;
  अगर (unlikely (hb_unsigned_add_overflows (curve_तथ्य_offset,
					   band_headers_len,
					   &curve_तथ्य_offset) ||
		hb_unsigned_add_overflows (curve_तथ्य_offset,
					   योग_curve_indices,
					   &curve_तथ्य_offset)))
  {
    hb_blob_t::recycle_abort ((अक्षर *) buf, draw->recycled_blob);
    वापस nullptr;
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
  अगर (unlikely (!s.curve_texel_offset.resize (अंक_curves)))
  {
    hb_blob_t::recycle_abort ((अक्षर *) buf, draw->recycled_blob);
    वापस nullptr;
  }

  अचिन्हित texel = curve_तथ्य_offset;

  क्रम (अचिन्हित i = 0; i < अंक_curves; i++)
  {
    bool contour_start = curves[i].contour_start;

    अगर (contour_start) {
      s.curve_texel_offset.arrayZ[i] = texel;
      buf[texel].r = quantize (curves[i].p1x);
      buf[texel].g = quantize (curves[i].p1y);
      buf[texel].b = quantize (curves[i].p2x);
      buf[texel].a = quantize (curves[i].p2y);
      texel++;
    } अन्यथा {
      s.curve_texel_offset.arrayZ[i] = texel - 1;
    }

    bool has_अग्र = i + 1 < अंक_curves &&
		    !curves[i + 1].contour_start;

    buf[texel].r = quantize (curves[i].p3x);
    buf[texel].g = quantize (curves[i].p3y);
    अगर (has_अग्र) {
      buf[texel].b = quantize (curves[i + 1].p2x);
      buf[texel].a = quantize (curves[i + 1].p2y);
    } अन्यथा {
      buf[texel].b = 0;
      buf[texel].a = 0;
    }
    texel++;
  }

  /* Pack band headers and curve indices */
  अचिन्हित सूचकांक_offset = header_len + band_headers_len;

  क्रम (अचिन्हित b = 0; b < अंक_hbands; b++)
  {
    int16_t hband_split;
    {
      अचिन्हित off = s.hband_offsets.arrayZ[b];
      अचिन्हित n = s.hband_curve_counts.arrayZ[b];
      अचिन्हित best_worst = n;
      द्विगुन best_split = (min_x + max_x) * 0.5;
      अचिन्हित बायाँ_गिनती = n;
      क्रम (अचिन्हित ci = 0; ci < n; ci++) {
	द्विगुन split = s.curve_infos.arrayZ[s.hband_curves.arrayZ[off + ci]].max_x;
	अचिन्हित दायाँ_गिनती = ci + 1;
	जबतक (बायाँ_गिनती &&
	       s.curve_infos.arrayZ[s.hband_curves_asc.arrayZ[off + बायाँ_गिनती - 1]].min_x > split)
	  बायाँ_गिनती--;
	अचिन्हित worst = hb_max (दायाँ_गिनती, बायाँ_गिनती);
	अगर (worst < best_worst) {
	  best_worst = worst;
	  best_split = split;
	}
      }
      hband_split = quantize (best_split);
    }
    अचिन्हित hdr = header_len + b;
    अचिन्हित desc_off = सूचकांक_offset;

    क्रम (अचिन्हित ci = 0; ci < s.hband_curve_counts.arrayZ[b]; ci++) {
      buf[सूचकांक_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.hband_curves.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[सूचकांक_offset].g = 0;
      buf[सूचकांक_offset].b = 0;
      buf[सूचकांक_offset].a = 0;
      सूचकांक_offset++;
    }

    अचिन्हित asc_off = सूचकांक_offset;

    क्रम (अचिन्हित ci = 0; ci < s.hband_curve_counts.arrayZ[b]; ci++) {
      buf[सूचकांक_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.hband_curves_asc.arrayZ[s.hband_offsets.arrayZ[b] + ci]]);
      buf[सूचकांक_offset].g = 0;
      buf[सूचकांक_offset].b = 0;
      buf[सूचकांक_offset].a = 0;
      सूचकांक_offset++;
    }

    buf[hdr].r = (int16_t) s.hband_curve_counts.arrayZ[b];
    buf[hdr].g = encode_offset (desc_off);
    buf[hdr].b = encode_offset (asc_off);
    buf[hdr].a = hband_split;
  }

  क्रम (अचिन्हित b = 0; b < अंक_vbands; b++)
  {
    int16_t vband_split;
    {
      अचिन्हित off = s.vband_offsets.arrayZ[b];
      अचिन्हित n = s.vband_curve_counts.arrayZ[b];
      अचिन्हित best_worst = n;
      द्विगुन best_split = (min_y + max_y) * 0.5;
      अचिन्हित बायाँ_गिनती = n;
      क्रम (अचिन्हित ci = 0; ci < n; ci++) {
	द्विगुन split = s.curve_infos.arrayZ[s.vband_curves.arrayZ[off + ci]].max_y;
	अचिन्हित दायाँ_गिनती = ci + 1;
	जबतक (बायाँ_गिनती &&
	       s.curve_infos.arrayZ[s.vband_curves_asc.arrayZ[off + बायाँ_गिनती - 1]].min_y > split)
	  बायाँ_गिनती--;
	अचिन्हित worst = hb_max (दायाँ_गिनती, बायाँ_गिनती);
	अगर (worst < best_worst) {
	  best_worst = worst;
	  best_split = split;
	}
      }
      vband_split = quantize (best_split);
    }

    अचिन्हित hdr = header_len + अंक_hbands + b;
    अचिन्हित desc_off = सूचकांक_offset;

    क्रम (अचिन्हित ci = 0; ci < s.vband_curve_counts.arrayZ[b]; ci++) {
      buf[सूचकांक_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.vband_curves.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[सूचकांक_offset].g = 0;
      buf[सूचकांक_offset].b = 0;
      buf[सूचकांक_offset].a = 0;
      सूचकांक_offset++;
    }

    अचिन्हित asc_off = सूचकांक_offset;

    क्रम (अचिन्हित ci = 0; ci < s.vband_curve_counts.arrayZ[b]; ci++) {
      buf[सूचकांक_offset].r = encode_offset (s.curve_texel_offset.arrayZ[s.vband_curves_asc.arrayZ[s.vband_offsets.arrayZ[b] + ci]]);
      buf[सूचकांक_offset].g = 0;
      buf[सूचकांक_offset].b = 0;
      buf[सूचकांक_offset].a = 0;
      सूचकांक_offset++;
    }

    buf[hdr].r = (int16_t) s.vband_curve_counts.arrayZ[b];
    buf[hdr].g = encode_offset (desc_off);
    buf[hdr].b = encode_offset (asc_off);
    buf[hdr].a = vband_split;
  }

  hb_blob_t *recycled = draw->recycled_blob;
  draw->recycled_blob = nullptr;
  वापस hb_blob_t::recycle_finalize ((अक्षर *) buf, buf_capacity, needed_bytes,
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
hb_gpu_draw_create_or_fail (व्योम)
{
  वापस hb_object_create<hb_gpu_draw_t> ();
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
  वापस hb_object_reference (draw);
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
व्योम
hb_gpu_draw_destroy (hb_gpu_draw_t *draw)
{
  अगर (!hb_object_should_destroy (draw))
    वापस;

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
hb_gpu_draw_set_user_तथ्य (hb_gpu_draw_t     *draw,
			     hb_user_तथ्य_कुंजी_t *key,
			     व्योम               *तथ्य,
			     hb_destroy_func_t   destroy,
			     hb_bool_t           replace)
{
  वापस hb_object_set_user_तथ्य (draw, key, तथ्य, destroy, replace);
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
व्योम *
hb_gpu_draw_get_user_तथ्य (स्थिर hb_gpu_draw_t     *draw,
			     hb_user_तथ्य_कुंजी_t *key)
{
  वापस hb_object_get_user_तथ्य (draw, key);
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
hb_gpu_draw_get_funcs (स्थिर hb_gpu_draw_t *draw HB_UNUSED)
{
  वापस static_gpu_draw_funcs.get_unconst ();
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
व्योम
hb_gpu_draw_set_scale (hb_gpu_draw_t *draw,
		       पूर्णांक            x_scale,
		       पूर्णांक            y_scale)
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
व्योम
hb_gpu_draw_get_scale (स्थिर hb_gpu_draw_t *draw,
		       पूर्णांक                 *x_scale,
		       पूर्णांक                 *y_scale)
{
  अगर (x_scale) *x_scale = draw->x_scale;
  अगर (y_scale) *y_scale = draw->y_scale;
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
  पूर्णांक x_scale, y_scale;
  hb_font_get_scale (font, &x_scale, &y_scale);
  hb_gpu_draw_set_scale (draw, x_scale, y_scale);

  वापस hb_font_draw_glyph_or_fail (font, glyph,
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
व्योम
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
व्योम
hb_gpu_draw_clear (hb_gpu_draw_t *draw)
{
  draw->start_x = draw->start_y = 0;
  draw->current_x = draw->current_y = 0;
  draw->need_moveto = true;
  draw->अंक_curves = 0;
  draw->success = true;
  draw->curves.reset ();

  draw->ext_min_x =  विशाल_मान;
  draw->ext_min_y =  विशाल_मान;
  draw->ext_max_x = -विशाल_मान;
  draw->ext_max_y = -विशाल_मान;

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
व्योम
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
व्योम
hb_gpu_draw_recycle_blob (hb_gpu_draw_t *draw,
			    hb_blob_t      *blob)
{
  hb_blob_t::recycle_stash (&draw->recycled_blob, blob);
}


#समावेश "hb-gpu-draw-fragment-glsl.hh"
#समावेश "hb-gpu-draw-fragment-msl.hh"
#समावेश "hb-gpu-draw-fragment-wgsl.hh"
#समावेश "hb-gpu-draw-fragment-hlsl.hh"

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
स्थिर अक्षर *
hb_gpu_draw_shader_source (hb_gpu_shader_stage_t stage,
			   hb_gpu_shader_lang_t  lang)
{
  चयन (stage) {
  हाल HB_GPU_SHADER_STAGE_FRAGMENT:
    चयन (lang) {
    हाल HB_GPU_SHADER_LANG_GLSL: वापस hb_gpu_draw_fragment_glsl;
    हाल HB_GPU_SHADER_LANG_MSL:  वापस hb_gpu_draw_fragment_msl;
    हाल HB_GPU_SHADER_LANG_WGSL: वापस hb_gpu_draw_fragment_wgsl;
    हाल HB_GPU_SHADER_LANG_HLSL: वापस hb_gpu_draw_fragment_hlsl;
    हाल HB_GPU_SHADER_LANG_INVALID:
    शेष: वापस nullptr;
    }
  हाल HB_GPU_SHADER_STAGE_VERTEX:
    चयन (lang) {
    हाल HB_GPU_SHADER_LANG_GLSL:
    हाल HB_GPU_SHADER_LANG_MSL:
    हाल HB_GPU_SHADER_LANG_WGSL:
    हाल HB_GPU_SHADER_LANG_HLSL: वापस "";
    हाल HB_GPU_SHADER_LANG_INVALID:
    शेष: वापस nullptr;
    }
  शेष:
    वापस nullptr;
  }
}
