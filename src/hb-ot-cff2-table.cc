/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#నిర్వచితం_కాకపోతే HB_OT_CFF2_పట్టిక_CC
#నిర్వచించు HB_OT_CFF2_పట్టిక_CC
#నిర్వచితమైతే HB_OT_CFF2_పట్టిక_CC /* Pacify -Wunused-macros. */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_FONT_CFF

#అన్తర్భావ "hb-ot-cff2-table.hh"
#అన్తర్భావ "hb-cff2-interp-cs.hh"
#అన్తర్భావ "hb-draw.hh"

ఉపయోగించి నామాకాశ CFF;

నిర్మాణం cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.సమితి_int (గరిష్ఠ_పూర్ణాంకం);
    min_y.సమితి_int (గరిష్ఠ_పూర్ణాంకం);
    max_x.సమితి_int (INT_MIN);
    max_y.సమితి_int (INT_MIN);
  }

  శూన్య   ప్రారంభం_మార్గం ()       { మార్గం_open = నిజం; }
  శూన్య     చివర_మార్గం ()       { మార్గం_open = అబద్ధం; }
  తార్కిక is_మార్గం_open () స్థిరరాశి { తిరిగి మార్గం_open; }

  శూన్య update_bounds (స్థిరరాశి బిందువు_t &pt)
  {
    ఒకవేళ (pt.x < min_x) min_x = pt.x;
    ఒకవేళ (pt.x > max_x) max_x = pt.x;
    ఒకవేళ (pt.y < min_y) min_y = pt.y;
    ఒకవేళ (pt.y > max_y) max_y = pt.y;
  }

  తార్కిక  మార్గం_open = అబద్ధం;
  సంఖ్య_t min_x;
  సంఖ్య_t min_y;
  సంఖ్య_t max_x;
  సంఖ్య_t max_y;
};

నిర్మాణం cff2_మార్గం_procs_extents_t : మార్గం_procs_t<cff2_మార్గం_procs_extents_t, cff2_cs_interp_env_t<సంఖ్య_t>, cff2_extents_param_t>
{
  స్థిర శూన్య moveto (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_extents_param_t& param, స్థిరరాశి బిందువు_t &pt)
  {
    param.చివర_మార్గం ();
    env.moveto (pt);
  }

  స్థిర శూన్య పంక్తి (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_extents_param_t& param, స్థిరరాశి బిందువు_t &pt1)
  {
    ఒకవేళ (!param.is_మార్గం_open ())
    {
      param.ప్రారంభం_మార్గం ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  స్థిర శూన్య వక్రం (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_extents_param_t& param, స్థిరరాశి బిందువు_t &pt1, స్థిరరాశి బిందువు_t &pt2, స్థిరరాశి బిందువు_t &pt3)
  {
    ఒకవేళ (!param.is_మార్గం_open ())
    {
      param.ప్రారంభం_మార్గం ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

నిర్మాణం cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, సంఖ్య_t, cff2_మార్గం_procs_extents_t> {};

తార్కిక OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) స్థిరరాశి
{
  తిరిగి get_extents_at (font, glyph, extents, hb_array (font->coords, font->అంకె_coords));
}

తార్కిక OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
					      int64_t *budget) స్థిరరాశి
{
#నిర్వచితమైతే HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  తిరిగి నిజం;
#అయితే_ముగింపు

  ఒకవేళ (unlikely (!is_valid () || (glyph >= అంకె_glyphs))) తిరిగి అబద్ధం;

  int64_t దొంతర_budget = HB_BUDGET_GLYPH;
  ఒకవేళ (!budget) budget = &దొంతర_budget;

  గుర్తులేని పూర్ణాంకం fd = fdSelect->get_fd (glyph);
  స్థిరరాశి hb_ubytes_t str = (*char_అక్షరశ్రేణులు)[glyph];
  cff2_cs_interp_env_t<సంఖ్య_t> env (str, *ఇది, fd, coords.arrayZ, coords.నిడివి);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, సంఖ్య_t> interp (env);
  cff2_extents_param_t  param;
  ఒకవేళ (unlikely (!interp.interpret (param, budget))) తిరిగి అబద్ధం;

  ఒకవేళ (param.min_x >= param.max_x)
  {
    extents->వెడల్పు = 0;
    extents->x_bearing = 0;
  }
  లేకపోతే
  {
    ద్విగుణ x_bearing = roundf (param.min_x.గమ్యం_real ());
    extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (x_bearing);
    extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) roundf (param.max_x.గమ్యం_real ()) - x_bearing);
  }
  ఒకవేళ (param.min_y >= param.max_y)
  {
    extents->ఎత్తు = 0;
    extents->y_bearing = 0;
  }
  లేకపోతే
  {
    ద్విగుణ y_bearing = roundf (param.max_y.గమ్యం_real ());
    extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (y_bearing);
    extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> ((ద్విగుణ) roundf (param.min_y.గమ్యం_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  తిరిగి నిజం;
}

నిర్మాణం cff2_మార్గం_param_t
{
  cff2_మార్గం_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  శూన్య move_గమ్యం (స్థిరరాశి బిందువు_t &p)
  { draw_session->move_గమ్యం (font->em_fscalef_x (p.x.గమ్యం_real ()), font->em_fscalef_y (p.y.గమ్యం_real ())); }

  శూన్య పంక్తి_గమ్యం (స్థిరరాశి బిందువు_t &p)
  { draw_session->పంక్తి_గమ్యం (font->em_fscalef_x (p.x.గమ్యం_real ()), font->em_fscalef_y (p.y.గమ్యం_real ())); }

  శూన్య cubic_గమ్యం (స్థిరరాశి బిందువు_t &p1, స్థిరరాశి బిందువు_t &p2, స్థిరరాశి బిందువు_t &p3)
  {
    draw_session->cubic_గమ్యం (font->em_fscalef_x (p1.x.గమ్యం_real ()), font->em_fscalef_y (p1.y.గమ్యం_real ()),
			   font->em_fscalef_x (p2.x.గమ్యం_real ()), font->em_fscalef_y (p2.y.గమ్యం_real ()),
			   font->em_fscalef_x (p3.x.గమ్యం_real ()), font->em_fscalef_y (p3.y.గమ్యం_real ()));
  }

  రక్షిత:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

నిర్మాణం cff2_మార్గం_procs_మార్గం_t : మార్గం_procs_t<cff2_మార్గం_procs_మార్గం_t, cff2_cs_interp_env_t<సంఖ్య_t>, cff2_మార్గం_param_t>
{
  స్థిర శూన్య moveto (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_మార్గం_param_t& param, స్థిరరాశి బిందువు_t &pt)
  {
    param.move_గమ్యం (pt);
    env.moveto (pt);
  }

  స్థిర శూన్య పంక్తి (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_మార్గం_param_t& param, స్థిరరాశి బిందువు_t &pt1)
  {
    param.పంక్తి_గమ్యం (pt1);
    env.moveto (pt1);
  }

  స్థిర శూన్య వక్రం (cff2_cs_interp_env_t<సంఖ్య_t> &env, cff2_మార్గం_param_t& param, స్థిరరాశి బిందువు_t &pt1, స్థిరరాశి బిందువు_t &pt2, స్థిరరాశి బిందువు_t &pt3)
  {
    param.cubic_గమ్యం (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

నిర్మాణం cff2_cs_opset_మార్గం_t : cff2_cs_opset_t<cff2_cs_opset_మార్గం_t, cff2_మార్గం_param_t, సంఖ్య_t, cff2_మార్గం_procs_మార్గం_t> {};

తార్కిక OT::cff2::accelerator_t::get_మార్గం (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) స్థిరరాశి
{
  int64_t దొంతర_budget = HB_BUDGET_GLYPH;
  ఒకవేళ (!budget) budget = &దొంతర_budget;

  తిరిగి get_మార్గం_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->అంకె_coords : 0),
		      budget);
}

తార్కిక OT::cff2::accelerator_t::get_మార్గం_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<స్థిరరాశి పూర్ణాంకం> coords, int64_t *budget) స్థిరరాశి
{
#నిర్వచితమైతే HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  తిరిగి నిజం;
#అయితే_ముగింపు

  ఒకవేళ (unlikely (!is_valid () || (glyph >= అంకె_glyphs))) తిరిగి అబద్ధం;

  int64_t దొంతర_budget = HB_BUDGET_GLYPH;
  ఒకవేళ (!budget) budget = &దొంతర_budget;

  గుర్తులేని పూర్ణాంకం fd = fdSelect->get_fd (glyph);
  స్థిరరాశి hb_ubytes_t str = (*char_అక్షరశ్రేణులు)[glyph];
  cff2_cs_interp_env_t<సంఖ్య_t> env (str, *ఇది, fd, coords.arrayZ, coords.నిడివి);
  cff2_cs_interpreter_t<cff2_cs_opset_మార్గం_t, cff2_మార్గం_param_t, సంఖ్య_t> interp (env);
  cff2_మార్గం_param_t param (font, draw_session);
  ఒకవేళ (unlikely (!interp.interpret (param, budget))) తిరిగి అబద్ధం;
  తిరిగి నిజం;
}

#అయితే_ముగింపు

#అయితే_ముగింపు /* HB_OT_CFF2_TABLE_CC pacify */
#అయితే_ముగింపు /* HB_OT_CFF2_TABLE_CC guard */
