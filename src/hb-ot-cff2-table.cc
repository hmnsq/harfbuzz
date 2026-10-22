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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_FONT_CFF

#અન્તર્ભાવ "hb-ot-cff2-table.hh"
#અન્તર્ભાવ "hb-cff2-interp-cs.hh"
#અન્તર્ભાવ "hb-draw.hh"

using નામાકાશ CFF;

સંરચના cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (INT_MAX);
    min_y.set_int (INT_MAX);
    max_x.set_int (INT_MIN);
    max_y.set_int (INT_MIN);
  }

  શૂન્ય   start_path ()       { path_open = સાચું; }
  શૂન્ય     end_path ()       { path_open = ખોટું; }
  તાર્કિક is_path_open () અચલ { પાછા path_open; }

  શૂન્ય update_bounds (અચલ point_t &pt)
  {
    જો (pt.x < min_x) min_x = pt.x;
    જો (pt.x > max_x) max_x = pt.x;
    જો (pt.y < min_y) min_y = pt.y;
    જો (pt.y > max_y) max_y = pt.y;
  }

  તાર્કિક  path_open = ખોટું;
  સંખ્યા_t min_x;
  સંખ્યા_t min_y;
  સંખ્યા_t max_x;
  સંખ્યા_t max_y;
};

સંરચના cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<સંખ્યા_t>, cff2_extents_param_t>
{
  સ્થિર શૂન્ય moveto (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_extents_param_t& param, અચલ point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  સ્થિર શૂન્ય લીટી (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_extents_param_t& param, અચલ point_t &pt1)
  {
    જો (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  સ્થિર શૂન્ય curve (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_extents_param_t& param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3)
  {
    જો (!param.is_path_open ())
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

સંરચના cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, સંખ્યા_t, cff2_path_procs_extents_t> {};

તાર્કિક OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) અચલ
{
  પાછા get_extents_at (font, glyph, extents, hb_array (font->coords, font->અંક_coords));
}

તાર્કિક OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<અચલ પૂર્ણાંક> coords,
					      int64_t *budget) અચલ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  પાછા સાચું;
#endif

  જો (unlikely (!is_valid () || (glyph >= અંક_glyphs))) પાછા ખોટું;

  int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
  જો (!budget) budget = &ઢગલો_budget;

  unsigned પૂર્ણાંક fd = fdSelect->get_fd (glyph);
  અચલ hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<સંખ્યા_t> env (str, *આ, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, સંખ્યા_t> interp (env);
  cff2_extents_param_t  param;
  જો (unlikely (!interp.interpret (param, budget))) પાછા ખોટું;

  જો (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  નહીંતર
  {
    દ્વિગુણ x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (param.max_x.to_real ()) - x_bearing);
  }
  જો (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  નહીંતર
  {
    દ્વિગુણ y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  પાછા સાચું;
}

સંરચના cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  શૂન્ય move_to (અચલ point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  શૂન્ય લીટી_to (અચલ point_t &p)
  { draw_session->લીટી_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  શૂન્ય cubic_to (અચલ point_t &p1, અચલ point_t &p2, અચલ point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  સુરક્ષિત:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

સંરચના cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<સંખ્યા_t>, cff2_path_param_t>
{
  સ્થિર શૂન્ય moveto (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_path_param_t& param, અચલ point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  સ્થિર શૂન્ય લીટી (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_path_param_t& param, અચલ point_t &pt1)
  {
    param.લીટી_to (pt1);
    env.moveto (pt1);
  }

  સ્થિર શૂન્ય curve (cff2_cs_interp_env_t<સંખ્યા_t> &env, cff2_path_param_t& param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

સંરચના cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, સંખ્યા_t, cff2_path_procs_path_t> {};

તાર્કિક OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) અચલ
{
  int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
  જો (!budget) budget = &ઢગલો_budget;

  પાછા get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->અંક_coords : 0),
		      budget);
}

તાર્કિક OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<અચલ પૂર્ણાંક> coords, int64_t *budget) અચલ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  પાછા સાચું;
#endif

  જો (unlikely (!is_valid () || (glyph >= અંક_glyphs))) પાછા ખોટું;

  int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
  જો (!budget) budget = &ઢગલો_budget;

  unsigned પૂર્ણાંક fd = fdSelect->get_fd (glyph);
  અચલ hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<સંખ્યા_t> env (str, *આ, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, સંખ્યા_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  જો (unlikely (!interp.interpret (param, budget))) પાછા ખોટું;
  પાછા સાચું;
}

#endif

#endif /* HB_OT_CFF2_TABLE_CC pacify */
#endif /* HB_OT_CFF2_TABLE_CC guard */
