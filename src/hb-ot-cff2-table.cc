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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_FONT_CFF

#অন্তর্ভাব "hb-ot-cff2-table.hh"
#অন্তর্ভাব "hb-cff2-interp-cs.hh"
#অন্তর্ভাব "hb-draw.hh"

using নামাকাশ CFF;

কাঠামো cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (INT_MAX);
    min_y.set_int (INT_MAX);
    max_x.set_int (INT_MIN);
    max_y.set_int (INT_MIN);
  }

  শূন্য   start_path ()       { path_open = সত্য; }
  শূন্য     end_path ()       { path_open = মিথ্যা; }
  যৌক্তিক is_path_open () ধ্রুবক { ফেরত path_open; }

  শূন্য update_bounds (ধ্রুবক point_t &pt)
  {
    যদি (pt.x < min_x) min_x = pt.x;
    যদি (pt.x > max_x) max_x = pt.x;
    যদি (pt.y < min_y) min_y = pt.y;
    যদি (pt.y > max_y) max_y = pt.y;
  }

  যৌক্তিক  path_open = মিথ্যা;
  সংখ্যা_t min_x;
  সংখ্যা_t min_y;
  সংখ্যা_t max_x;
  সংখ্যা_t max_y;
};

কাঠামো cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<সংখ্যা_t>, cff2_extents_param_t>
{
  স্থির শূন্য moveto (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_extents_param_t& param, ধ্রুবক point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  স্থির শূন্য সারি (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_extents_param_t& param, ধ্রুবক point_t &pt1)
  {
    যদি (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  স্থির শূন্য curve (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_extents_param_t& param, ধ্রুবক point_t &pt1, ধ্রুবক point_t &pt2, ধ্রুবক point_t &pt3)
  {
    যদি (!param.is_path_open ())
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

কাঠামো cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, সংখ্যা_t, cff2_path_procs_extents_t> {};

যৌক্তিক OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ধ্রুবক
{
  ফেরত get_extents_at (font, glyph, extents, hb_array (font->coords, font->অঙ্ক_coords));
}

যৌক্তিক OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
					      int64_t *budget) ধ্রুবক
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ফেরত সত্য;
#endif

  যদি (unlikely (!is_valid () || (glyph >= অঙ্ক_glyphs))) ফেরত মিথ্যা;

  int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
  যদি (!budget) budget = &স্তূপ_budget;

  unsigned পূর্ণসংখ্যা fd = fdSelect->get_fd (glyph);
  ধ্রুবক hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<সংখ্যা_t> env (str, *এই, fd, coords.arrayZ, coords.দৈর্ঘ্য);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, সংখ্যা_t> interp (env);
  cff2_extents_param_t  param;
  যদি (unlikely (!interp.interpret (param, budget))) ফেরত মিথ্যা;

  যদি (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  অন্যথা
  {
    দ্বিগুণ x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((দ্বিগুণ) roundf (param.max_x.to_real ()) - x_bearing);
  }
  যদি (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  অন্যথা
  {
    দ্বিগুণ y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((দ্বিগুণ) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  ফেরত সত্য;
}

কাঠামো cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  শূন্য move_to (ধ্রুবক point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  শূন্য সারি_to (ধ্রুবক point_t &p)
  { draw_session->সারি_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  শূন্য cubic_to (ধ্রুবক point_t &p1, ধ্রুবক point_t &p2, ধ্রুবক point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  সুরক্ষিত:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

কাঠামো cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<সংখ্যা_t>, cff2_path_param_t>
{
  স্থির শূন্য moveto (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_path_param_t& param, ধ্রুবক point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  স্থির শূন্য সারি (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_path_param_t& param, ধ্রুবক point_t &pt1)
  {
    param.সারি_to (pt1);
    env.moveto (pt1);
  }

  স্থির শূন্য curve (cff2_cs_interp_env_t<সংখ্যা_t> &env, cff2_path_param_t& param, ধ্রুবক point_t &pt1, ধ্রুবক point_t &pt2, ধ্রুবক point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

কাঠামো cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, সংখ্যা_t, cff2_path_procs_path_t> {};

যৌক্তিক OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ধ্রুবক
{
  int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
  যদি (!budget) budget = &স্তূপ_budget;

  ফেরত get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->অঙ্ক_coords : 0),
		      budget);
}

যৌক্তিক OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords, int64_t *budget) ধ্রুবক
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ফেরত সত্য;
#endif

  যদি (unlikely (!is_valid () || (glyph >= অঙ্ক_glyphs))) ফেরত মিথ্যা;

  int64_t স্তূপ_budget = HB_BUDGET_GLYPH;
  যদি (!budget) budget = &স্তূপ_budget;

  unsigned পূর্ণসংখ্যা fd = fdSelect->get_fd (glyph);
  ধ্রুবক hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<সংখ্যা_t> env (str, *এই, fd, coords.arrayZ, coords.দৈর্ঘ্য);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, সংখ্যা_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  যদি (unlikely (!interp.interpret (param, budget))) ফেরত মিথ্যা;
  ফেরত সত্য;
}

#endif

#endif /* HB_OT_CFF2_TABLE_CC pacify */
#endif /* HB_OT_CFF2_TABLE_CC guard */
