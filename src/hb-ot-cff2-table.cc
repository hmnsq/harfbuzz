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

#ifndef HB_OT_CFF2_TABLE_CC
#define HB_OT_CFF2_TABLE_CC
#ifdef HB_OT_CFF2_TABLE_CC /* Pacify -Wunused-macros. */

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_FONT_CFF

#ଅନ୍ତର୍ଭାଵ "hb-ot-cff2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff2-interp-cs.hh"
#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"

using ନାମାକାଶ CFF;

ସଂରଚନା cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (INT_MAX);
    min_y.set_int (INT_MAX);
    max_x.set_int (INT_MIN);
    max_y.set_int (INT_MIN);
  }

  ଶୂନ୍ୟ   start_path ()       { path_open = ସତ୍ୟ; }
  ଶୂନ୍ୟ     end_path ()       { path_open = ମିଥ୍ୟା; }
  ତାର୍କିକ is_path_open () ଅଚଳ { ଫେରନ୍ତୁ path_open; }

  ଶୂନ୍ୟ update_bounds (ଅଚଳ point_t &pt)
  {
    ଯଦି (pt.x < min_x) min_x = pt.x;
    ଯଦି (pt.x > max_x) max_x = pt.x;
    ଯଦି (pt.y < min_y) min_y = pt.y;
    ଯଦି (pt.y > max_y) max_y = pt.y;
  }

  ତାର୍କିକ  path_open = ମିଥ୍ୟା;
  ସଂଖ୍ୟା_t min_x;
  ସଂଖ୍ୟା_t min_y;
  ସଂଖ୍ୟା_t max_x;
  ସଂଖ୍ୟା_t max_y;
};

ସଂରଚନା cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<ସଂଖ୍ୟା_t>, cff2_extents_param_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ moveto (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_extents_param_t& param, ଅଚଳ point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  ସ୍ଥିର ଶୂନ୍ୟ ଧାଡ଼ି (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_extents_param_t& param, ଅଚଳ point_t &pt1)
  {
    ଯଦି (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ସ୍ଥିର ଶୂନ୍ୟ curve (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_extents_param_t& param, ଅଚଳ point_t &pt1, ଅଚଳ point_t &pt2, ଅଚଳ point_t &pt3)
  {
    ଯଦି (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

ସଂରଚନା cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, ସଂଖ୍ୟା_t, cff2_path_procs_extents_t> {};

ତାର୍କିକ OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ଅଚଳ
{
  ଫେରନ୍ତୁ get_extents_at (font, glyph, extents, hb_array (font->coords, font->ଅଙ୍କ_coords));
}

ତାର୍କିକ OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
					      int64_t *budget) ଅଚଳ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

  ଯଦି (unlikely (!is_valid () || (glyph >= ଅଙ୍କ_glyphs))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  int64_t ଗଦା_budget = HB_BUDGET_GLYPH;
  ଯଦି (!budget) budget = &ଗଦା_budget;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = fdSelect->get_fd (glyph);
  ଅଚଳ hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> env (str, *ଏହି, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, ସଂଖ୍ୟା_t> interp (env);
  cff2_extents_param_t  param;
  ଯଦି (unlikely (!interp.interpret (param, budget))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  ଅନ୍ୟଥା
  {
    ଦ୍ୱିଗୁଣ x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((ଦ୍ୱିଗୁଣ) roundf (param.max_x.to_real ()) - x_bearing);
  }
  ଯଦି (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  ଅନ୍ୟଥା
  {
    ଦ୍ୱିଗୁଣ y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((ଦ୍ୱିଗୁଣ) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସଂରଚନା cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  ଶୂନ୍ୟ move_to (ଅଚଳ point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  ଶୂନ୍ୟ ଧାଡ଼ି_to (ଅଚଳ point_t &p)
  { draw_session->ଧାଡ଼ି_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  ଶୂନ୍ୟ cubic_to (ଅଚଳ point_t &p1, ଅଚଳ point_t &p2, ଅଚଳ point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  ସୁରକ୍ଷିତ:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

ସଂରଚନା cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<ସଂଖ୍ୟା_t>, cff2_path_param_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ moveto (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_path_param_t& param, ଅଚଳ point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  ସ୍ଥିର ଶୂନ୍ୟ ଧାଡ଼ି (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_path_param_t& param, ଅଚଳ point_t &pt1)
  {
    param.ଧାଡ଼ି_to (pt1);
    env.moveto (pt1);
  }

  ସ୍ଥିର ଶୂନ୍ୟ curve (cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> &env, cff2_path_param_t& param, ଅଚଳ point_t &pt1, ଅଚଳ point_t &pt2, ଅଚଳ point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ସଂରଚନା cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, ସଂଖ୍ୟା_t, cff2_path_procs_path_t> {};

ତାର୍କିକ OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ଅଚଳ
{
  int64_t ଗଦା_budget = HB_BUDGET_GLYPH;
  ଯଦି (!budget) budget = &ଗଦା_budget;

  ଫେରନ୍ତୁ get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->ଅଙ୍କ_coords : 0),
		      budget);
}

ତାର୍କିକ OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords, int64_t *budget) ଅଚଳ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ଫେରନ୍ତୁ ସତ୍ୟ;
#endif

  ଯଦି (unlikely (!is_valid () || (glyph >= ଅଙ୍କ_glyphs))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  int64_t ଗଦା_budget = HB_BUDGET_GLYPH;
  ଯଦି (!budget) budget = &ଗଦା_budget;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = fdSelect->get_fd (glyph);
  ଅଚଳ hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<ସଂଖ୍ୟା_t> env (str, *ଏହି, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, ସଂଖ୍ୟା_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  ଯଦି (unlikely (!interp.interpret (param, budget))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

#endif

#endif /* HB_OT_CFF2_TABLE_CC pacify */
#endif /* HB_OT_CFF2_TABLE_CC guard */
