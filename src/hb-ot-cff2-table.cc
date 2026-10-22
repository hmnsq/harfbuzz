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

#ضمن "hb.hh"

#ifndef HB_NO_OT_FONT_CFF

#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-draw.hh"

using نطاق CFF;

بنية cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (INT_MAX);
    min_y.set_int (INT_MAX);
    max_x.set_int (INT_MIN);
    max_y.set_int (INT_MIN);
  }

  فراغ   start_path ()       { path_open = אמת; }
  فراغ     end_path ()       { path_open = שקר; }
  منطقي is_path_open () ثابت { החזר path_open; }

  فراغ update_bounds (ثابت point_t &pt)
  {
    אם (pt.x < min_x) min_x = pt.x;
    אם (pt.x > max_x) max_x = pt.x;
    אם (pt.y < min_y) min_y = pt.y;
    אם (pt.y > max_y) max_y = pt.y;
  }

  منطقي  path_open = שקר;
  number_t min_x;
  number_t min_y;
  number_t max_x;
  number_t max_y;
};

بنية cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<number_t>, cff2_extents_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<number_t> &env, cff2_extents_param_t& param, ثابت point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  ساكن فراغ line (cff2_cs_interp_env_t<number_t> &env, cff2_extents_param_t& param, ثابت point_t &pt1)
  {
    אם (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ساكن فراغ curve (cff2_cs_interp_env_t<number_t> &env, cff2_extents_param_t& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3)
  {
    אם (!param.is_path_open ())
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

بنية cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, number_t, cff2_path_procs_extents_t> {};

منطقي OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ثابت
{
  החזר get_extents_at (font, glyph, extents, hb_array (font->coords, font->num_coords));
}

منطقي OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ثابت صحيح> coords,
					      int64_t *budget) ثابت
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  החזר אמת;
#endif

  אם (unlikely (!is_valid () || (glyph >= num_glyphs))) החזר שקר;

  int64_t stack_budget = HB_BUDGET_GLYPH;
  אם (!budget) budget = &stack_budget;

  unsigned صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<number_t> env (str, *هذا, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, number_t> interp (env);
  cff2_extents_param_t  param;
  אם (unlikely (!interp.interpret (param, budget))) החזר שקר;

  אם (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  אחרת
  {
    مزدوج x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((مزدوج) roundf (param.max_x.to_real ()) - x_bearing);
  }
  אם (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  אחרת
  {
    مزدوج y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((مزدوج) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  החזר אמת;
}

بنية cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  فراغ move_to (ثابت point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  فراغ line_to (ثابت point_t &p)
  { draw_session->line_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  فراغ cubic_to (ثابت point_t &p1, ثابت point_t &p2, ثابت point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  محمي:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

بنية cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<number_t>, cff2_path_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<number_t> &env, cff2_path_param_t& param, ثابت point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  ساكن فراغ line (cff2_cs_interp_env_t<number_t> &env, cff2_path_param_t& param, ثابت point_t &pt1)
  {
    param.line_to (pt1);
    env.moveto (pt1);
  }

  ساكن فراغ curve (cff2_cs_interp_env_t<number_t> &env, cff2_path_param_t& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

بنية cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, number_t, cff2_path_procs_path_t> {};

منطقي OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ثابت
{
  int64_t stack_budget = HB_BUDGET_GLYPH;
  אם (!budget) budget = &stack_budget;

  החזר get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->num_coords : 0),
		      budget);
}

منطقي OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget) ثابت
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  החזר אמת;
#endif

  אם (unlikely (!is_valid () || (glyph >= num_glyphs))) החזר שקר;

  int64_t stack_budget = HB_BUDGET_GLYPH;
  אם (!budget) budget = &stack_budget;

  unsigned صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<number_t> env (str, *هذا, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, number_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  אם (unlikely (!interp.interpret (param, budget))) החזר שקר;
  החזר אמת;
}

#endif

#endif /* HB_OT_CFF2_TABLE_CC pacify */
#endif /* HB_OT_CFF2_TABLE_CC guard */
