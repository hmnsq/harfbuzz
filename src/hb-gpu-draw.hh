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

#अगर_अघोषित HB_GPU_DRAW_HH
#घोषणा HB_GPU_DRAW_HH

#समावेश "hb.hh"
#समावेश "hb-gpu.h"
#समावेश "hb-object.hh"
#समावेश "hb-vector.hh"

#समावेश <cmath>


#घोषणा HB_GPU_UNITS_PER_EM 4


काष्ठा hb_gpu_curve_t
{
  द्विगुन p1x, p1y;
  द्विगुन p2x, p2y;
  द्विगुन p3x, p3y;
  bool contour_start;
};

काष्ठा hb_gpu_encode_curve_info_t
{
  द्विगुन min_x, max_x, min_y, max_y;
  bool is_horizontal;
  bool is_vertical;
  पूर्णांक hband_lo, hband_hi;
  पूर्णांक vband_lo, vband_hi;
};

काष्ठा hb_gpu_encode_scratch_t
{
  hb_vector_t<hb_gpu_encode_curve_info_t> curve_infos;
  hb_vector_t<अचिन्हित> hband_curve_counts;
  hb_vector_t<अचिन्हित> vband_curve_counts;
  hb_vector_t<अचिन्हित> hband_offsets;
  hb_vector_t<अचिन्हित> vband_offsets;
  hb_vector_t<अचिन्हित> hband_curves;
  hb_vector_t<अचिन्हित> hband_curves_asc;
  hb_vector_t<अचिन्हित> vband_curves;
  hb_vector_t<अचिन्हित> vband_curves_asc;
  hb_vector_t<अचिन्हित> hband_cursors;
  hb_vector_t<अचिन्हित> vband_cursors;
  hb_vector_t<अचिन्हित> curve_texel_offset;

  व्योम सफा ()
  {
    curve_infos.सफा ();
    hband_curve_counts.सफा ();
    vband_curve_counts.सफा ();
    hband_offsets.सफा ();
    vband_offsets.सफा ();
    hband_curves.सफा ();
    hband_curves_asc.सफा ();
    vband_curves.सफा ();
    vband_curves_asc.सफा ();
    hband_cursors.सफा ();
    vband_cursors.सफा ();
    curve_texel_offset.सफा ();
  }
};

काष्ठा hb_gpu_draw_t
{
  hb_object_header_t header;

  /* Accumulator state */
  द्विगुन start_x = 0, start_y = 0;
  द्विगुन current_x = 0, current_y = 0;
  bool need_moveto = true;
  अचिन्हित अंक_curves = 0;
  bool success = true;

  hb_vector_t<hb_gpu_curve_t> curves;

  /* Extents tracking (updated during accumulation) */
  द्विगुन ext_min_x =  विशाल_मान;
  द्विगुन ext_min_y =  विशाल_मान;
  द्विगुन ext_max_x = -विशाल_मान;
  द्विगुन ext_max_y = -विशाल_मान;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  पूर्णांक x_scale = 0;
  पूर्णांक y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_remaining = HB_BUDGET_GLYPH;

  व्योम recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_remaining ()
  { वापस &budget_remaining; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = nullptr;

  /* Internal accumulation methods */
  HB_INTERNAL व्योम acc_move_to (द्विगुन x, द्विगुन y);
  HB_INTERNAL व्योम acc_पंक्ति_to (द्विगुन x, द्विगुन y);
  HB_INTERNAL व्योम acc_conic_to (द्विगुन cx, द्विगुन cy, द्विगुन x, द्विगुन y);
  HB_INTERNAL व्योम acc_cubic_to (द्विगुन c1x, द्विगुन c1y,
				 द्विगुन c2x, द्विगुन c2y,
				 द्विगुन x, द्विगुन y);
  HB_INTERNAL व्योम acc_close_path ();
};


#पूर्ण_अगर /* HB_GPU_DRAW_HH */
