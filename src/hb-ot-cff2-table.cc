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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_FONT_CFF

#अन्तर्भाव "hb-ot-cff2-table.hh"
#अन्तर्भाव "hb-cff2-interp-cs.hh"
#अन्तर्भाव "hb-draw.hh"

using नामाकाश CFF;

संरचना cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (INT_MAX);
    min_y.set_int (INT_MAX);
    max_x.set_int (INT_MIN);
    max_y.set_int (INT_MIN);
  }

  शून्य   start_path ()       { path_open = सच्च; }
  शून्य     end_path ()       { path_open = मिच्छ; }
  तार्किक is_path_open () ध्रुव { प्रत्यागम path_open; }

  शून्य update_bounds (ध्रुव point_t &pt)
  {
    जइ (pt.x < min_x) min_x = pt.x;
    जइ (pt.x > max_x) max_x = pt.x;
    जइ (pt.y < min_y) min_y = pt.y;
    जइ (pt.y > max_y) max_y = pt.y;
  }

  तार्किक  path_open = मिच्छ;
  सङ्ख्या_t min_x;
  सङ्ख्या_t min_y;
  सङ्ख्या_t max_x;
  सङ्ख्या_t max_y;
};

संरचना cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<सङ्ख्या_t>, cff2_extents_param_t>
{
  स्थिर शून्य moveto (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_extents_param_t& param, ध्रुव point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  स्थिर शून्य पङ्क्तिः (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_extents_param_t& param, ध्रुव point_t &pt1)
  {
    जइ (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  स्थिर शून्य curve (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_extents_param_t& param, ध्रुव point_t &pt1, ध्रुव point_t &pt2, ध्रुव point_t &pt3)
  {
    जइ (!param.is_path_open ())
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

संरचना cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, सङ्ख्या_t, cff2_path_procs_extents_t> {};

तार्किक OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ध्रुव
{
  प्रत्यागम get_extents_at (font, glyph, extents, hb_array (font->coords, font->अङ्कः_coords));
}

तार्किक OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ध्रुव पूर्णाङ्क> coords,
					      int64_t *budget) ध्रुव
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  प्रत्यागम सच्च;
#endif

  जइ (unlikely (!is_valid () || (glyph >= अङ्कः_glyphs))) प्रत्यागम मिच्छ;

  int64_t राशिः_budget = HB_BUDGET_GLYPH;
  जइ (!budget) budget = &राशिः_budget;

  unsigned पूर्णाङ्क fd = fdSelect->get_fd (glyph);
  ध्रुव hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<सङ्ख्या_t> env (str, *एतत्, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, सङ्ख्या_t> interp (env);
  cff2_extents_param_t  param;
  जइ (unlikely (!interp.interpret (param, budget))) प्रत्यागम मिच्छ;

  जइ (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  अण्णहा
  {
    द्विगुण x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((द्विगुण) roundf (param.max_x.to_real ()) - x_bearing);
  }
  जइ (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  अण्णहा
  {
    द्विगुण y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((द्विगुण) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  प्रत्यागम सच्च;
}

संरचना cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  शून्य move_to (ध्रुव point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  शून्य पङ्क्तिः_to (ध्रुव point_t &p)
  { draw_session->पङ्क्तिः_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  शून्य cubic_to (ध्रुव point_t &p1, ध्रुव point_t &p2, ध्रुव point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  रक्षित:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

संरचना cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<सङ्ख्या_t>, cff2_path_param_t>
{
  स्थिर शून्य moveto (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_path_param_t& param, ध्रुव point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  स्थिर शून्य पङ्क्तिः (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_path_param_t& param, ध्रुव point_t &pt1)
  {
    param.पङ्क्तिः_to (pt1);
    env.moveto (pt1);
  }

  स्थिर शून्य curve (cff2_cs_interp_env_t<सङ्ख्या_t> &env, cff2_path_param_t& param, ध्रुव point_t &pt1, ध्रुव point_t &pt2, ध्रुव point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

संरचना cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, सङ्ख्या_t, cff2_path_procs_path_t> {};

तार्किक OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ध्रुव
{
  int64_t राशिः_budget = HB_BUDGET_GLYPH;
  जइ (!budget) budget = &राशिः_budget;

  प्रत्यागम get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->अङ्कः_coords : 0),
		      budget);
}

तार्किक OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ध्रुव पूर्णाङ्क> coords, int64_t *budget) ध्रुव
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  प्रत्यागम सच्च;
#endif

  जइ (unlikely (!is_valid () || (glyph >= अङ्कः_glyphs))) प्रत्यागम मिच्छ;

  int64_t राशिः_budget = HB_BUDGET_GLYPH;
  जइ (!budget) budget = &राशिः_budget;

  unsigned पूर्णाङ्क fd = fdSelect->get_fd (glyph);
  ध्रुव hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<सङ्ख्या_t> env (str, *एतत्, fd, coords.arrayZ, coords.length);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, सङ्ख्या_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  जइ (unlikely (!interp.interpret (param, budget))) प्रत्यागम मिच्छ;
  प्रत्यागम सच्च;
}

#endif

#endif /* HB_OT_CFF2_TABLE_CC pacify */
#endif /* HB_OT_CFF2_TABLE_CC guard */
