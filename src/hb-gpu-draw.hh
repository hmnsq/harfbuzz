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

#నిర్వచితం_కాకపోతే HB_GPU_DRAW_HH
#నిర్వచించు HB_GPU_DRAW_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-gpu.h"
#అన్తర్భావ "hb-object.hh"
#అన్తర్భావ "hb-vector.hh"

#అన్తర్భావ <cmath>


#నిర్వచించు HB_GPU_UNITS_చొప్పున_EM 4


నిర్మాణం hb_gpu_వక్రం_t
{
  ద్విగుణ p1x, p1y;
  ద్విగుణ p2x, p2y;
  ద్విగుణ p3x, p3y;
  తార్కిక contour_ప్రారంభం;
};

నిర్మాణం hb_gpu_encode_వక్రం_info_t
{
  ద్విగుణ min_x, max_x, min_y, max_y;
  తార్కిక is_horizontal;
  తార్కిక is_vertical;
  పూర్ణాంకం hband_lo, hband_hi;
  పూర్ణాంకం vband_lo, vband_hi;
};

నిర్మాణం hb_gpu_encode_scratch_t
{
  hb_సదిశ_t<hb_gpu_encode_వక్రం_info_t> వక్రం_infos;
  hb_సదిశ_t<గుర్తులేని> hband_వక్రం_counts;
  hb_సదిశ_t<గుర్తులేని> vband_వక్రం_counts;
  hb_సదిశ_t<గుర్తులేని> hband_offsets;
  hb_సదిశ_t<గుర్తులేని> vband_offsets;
  hb_సదిశ_t<గుర్తులేని> hband_curves;
  hb_సదిశ_t<గుర్తులేని> hband_curves_asc;
  hb_సదిశ_t<గుర్తులేని> vband_curves;
  hb_సదిశ_t<గుర్తులేని> vband_curves_asc;
  hb_సదిశ_t<గుర్తులేని> hband_cursors;
  hb_సదిశ_t<గుర్తులేని> vband_cursors;
  hb_సదిశ_t<గుర్తులేని> వక్రం_texel_స్థానభ్రంశం;

  శూన్య clear ()
  {
    వక్రం_infos.clear ();
    hband_వక్రం_counts.clear ();
    vband_వక్రం_counts.clear ();
    hband_offsets.clear ();
    vband_offsets.clear ();
    hband_curves.clear ();
    hband_curves_asc.clear ();
    vband_curves.clear ();
    vband_curves_asc.clear ();
    hband_cursors.clear ();
    vband_cursors.clear ();
    వక్రం_texel_స్థానభ్రంశం.clear ();
  }
};

నిర్మాణం hb_gpu_draw_t
{
  hb_object_శీర్షిక_t శీర్షిక;

  /* Accumulator state */
  ద్విగుణ ప్రారంభం_x = 0, ప్రారంభం_y = 0;
  ద్విగుణ current_x = 0, current_y = 0;
  తార్కిక need_moveto = నిజం;
  గుర్తులేని అంకె_curves = 0;
  తార్కిక success = నిజం;

  hb_సదిశ_t<hb_gpu_వక్రం_t> curves;

  /* Extents tracking (updated during accumulation) */
  ద్విగుణ ext_min_x =  HUGE_VAL;
  ద్విగుణ ext_min_y =  HUGE_VAL;
  ద్విగుణ ext_max_x = -HUGE_VAL;
  ద్విగుణ ext_max_y = -HUGE_VAL;

  /* Font scale (set by hb_gpu_draw_set_scale or hb_gpu_draw_glyph). */
  పూర్ణాంకం x_scale = 0;
  పూర్ణాంకం y_scale = 0;

  /* Work budget for outline traversal and generated curves.  A paint
   * backend seeds this through the public draw-budget API before drawing
   * and reads the remainder back afterwards, so a whole paint session
   * shares one budget without reaching into this object. */
  int64_t  budget = HB_BUDGET_DEFAULT;
  int64_t  budget_మిగిలిన = HB_BUDGET_GLYPH;

  శూన్య recharge_budget ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  int64_t *get_budget_మిగిలిన ()
  { తిరిగి &budget_మిగిలిన; }

  /* Encode scratch (reused across calls) */
  hb_gpu_encode_scratch_t scratch;

  /* Recycled blob for encode output */
  hb_blob_t *recycled_blob = శూన్యసూచిక;

  /* Internal accumulation methods */
  HB_అంతర్గత శూన్య acc_move_గమ్యం (ద్విగుణ x, ద్విగుణ y);
  HB_అంతర్గత శూన్య acc_పంక్తి_గమ్యం (ద్విగుణ x, ద్విగుణ y);
  HB_అంతర్గత శూన్య acc_conic_గమ్యం (ద్విగుణ cx, ద్విగుణ cy, ద్విగుణ x, ద్విగుణ y);
  HB_అంతర్గత శూన్య acc_cubic_గమ్యం (ద్విగుణ c1x, ద్విగుణ c1y,
				 ద్విగుణ c2x, ద్విగుణ c2y,
				 ద్విగుణ x, ద్విగుణ y);
  HB_అంతర్గత శూన్య acc_close_మార్గం ();
};


#అయితే_ముగింపు /* HB_GPU_DRAW_HH */
