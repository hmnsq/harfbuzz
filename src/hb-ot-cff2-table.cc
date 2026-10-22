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

#अगर_अघोषित HB_OT_CFF2_TABLE_CC
#घोषणा HB_OT_CFF2_TABLE_CC
#अगर_घोषित HB_OT_CFF2_TABLE_CC /* Pacify -Wunused-macros. */

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_FONT_CFF

#समावेश "hb-ot-cff2-table.hh"
#समावेश "hb-cff2-interp-cs.hh"
#समावेश "hb-draw.hh"

using namespace CFF;

काष्ठा cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.set_int (पूर्णांक_उच्च);
    min_y.set_int (पूर्णांक_उच्च);
    max_x.set_int (पूर्णांक_न्यून);
    max_y.set_int (पूर्णांक_न्यून);
  }

  व्योम   start_path ()       { path_open = true; }
  व्योम     end_path ()       { path_open = false; }
  bool is_path_open () स्थिर { वापस path_open; }

  व्योम update_bounds (स्थिर point_t &pt)
  {
    अगर (pt.x < min_x) min_x = pt.x;
    अगर (pt.x > max_x) max_x = pt.x;
    अगर (pt.y < min_y) min_y = pt.y;
    अगर (pt.y > max_y) max_y = pt.y;
  }

  bool  path_open = false;
  संख्या_t min_x;
  संख्या_t min_y;
  संख्या_t max_x;
  संख्या_t max_y;
};

काष्ठा cff2_path_procs_extents_t : path_procs_t<cff2_path_procs_extents_t, cff2_cs_interp_env_t<संख्या_t>, cff2_extents_param_t>
{
  अटल व्योम moveto (cff2_cs_interp_env_t<संख्या_t> &env, cff2_extents_param_t& param, स्थिर point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  अटल व्योम पंक्ति (cff2_cs_interp_env_t<संख्या_t> &env, cff2_extents_param_t& param, स्थिर point_t &pt1)
  {
    अगर (!param.is_path_open ())
    {
      param.start_path ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  अटल व्योम curve (cff2_cs_interp_env_t<संख्या_t> &env, cff2_extents_param_t& param, स्थिर point_t &pt1, स्थिर point_t &pt2, स्थिर point_t &pt3)
  {
    अगर (!param.is_path_open ())
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

काष्ठा cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, संख्या_t, cff2_path_procs_extents_t> {};

bool OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) स्थिर
{
  वापस get_extents_at (font, glyph, extents, hb_array (font->coords, font->अंक_coords));
}

bool OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<स्थिर पूर्णांक> coords,
					      int64_t *budget) स्थिर
{
#अगर_घोषित HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  वापस true;
#पूर्ण_अगर

  अगर (unlikely (!is_valid () || (glyph >= अंक_glyphs))) वापस false;

  int64_t ढेर_budget = HB_BUDGET_GLYPH;
  अगर (!budget) budget = &ढेर_budget;

  अचिन्हित पूर्णांक fd = fdSelect->get_fd (glyph);
  स्थिर hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<संख्या_t> env (str, *यह, fd, coords.arrayZ, coords.लंबाई);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, संख्या_t> interp (env);
  cff2_extents_param_t  param;
  अगर (unlikely (!interp.interpret (param, budget))) वापस false;

  अगर (param.min_x >= param.max_x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  अन्यथा
  {
    द्विगुन x_bearing = roundf (param.min_x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((द्विगुन) roundf (param.max_x.to_real ()) - x_bearing);
  }
  अगर (param.min_y >= param.max_y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  अन्यथा
  {
    द्विगुन y_bearing = roundf (param.max_y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((द्विगुन) roundf (param.min_y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  वापस true;
}

काष्ठा cff2_path_param_t
{
  cff2_path_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  व्योम move_to (स्थिर point_t &p)
  { draw_session->move_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  व्योम पंक्ति_to (स्थिर point_t &p)
  { draw_session->पंक्ति_to (font->em_fscalef_x (p.x.to_real ()), font->em_fscalef_y (p.y.to_real ())); }

  व्योम cubic_to (स्थिर point_t &p1, स्थिर point_t &p2, स्थिर point_t &p3)
  {
    draw_session->cubic_to (font->em_fscalef_x (p1.x.to_real ()), font->em_fscalef_y (p1.y.to_real ()),
			   font->em_fscalef_x (p2.x.to_real ()), font->em_fscalef_y (p2.y.to_real ()),
			   font->em_fscalef_x (p3.x.to_real ()), font->em_fscalef_y (p3.y.to_real ()));
  }

  रक्षित:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

काष्ठा cff2_path_procs_path_t : path_procs_t<cff2_path_procs_path_t, cff2_cs_interp_env_t<संख्या_t>, cff2_path_param_t>
{
  अटल व्योम moveto (cff2_cs_interp_env_t<संख्या_t> &env, cff2_path_param_t& param, स्थिर point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  अटल व्योम पंक्ति (cff2_cs_interp_env_t<संख्या_t> &env, cff2_path_param_t& param, स्थिर point_t &pt1)
  {
    param.पंक्ति_to (pt1);
    env.moveto (pt1);
  }

  अटल व्योम curve (cff2_cs_interp_env_t<संख्या_t> &env, cff2_path_param_t& param, स्थिर point_t &pt1, स्थिर point_t &pt2, स्थिर point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

काष्ठा cff2_cs_opset_path_t : cff2_cs_opset_t<cff2_cs_opset_path_t, cff2_path_param_t, संख्या_t, cff2_path_procs_path_t> {};

bool OT::cff2::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) स्थिर
{
  int64_t ढेर_budget = HB_BUDGET_GLYPH;
  अगर (!budget) budget = &ढेर_budget;

  वापस get_path_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->अंक_coords : 0),
		      budget);
}

bool OT::cff2::accelerator_t::get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<स्थिर पूर्णांक> coords, int64_t *budget) स्थिर
{
#अगर_घोषित HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  वापस true;
#पूर्ण_अगर

  अगर (unlikely (!is_valid () || (glyph >= अंक_glyphs))) वापस false;

  int64_t ढेर_budget = HB_BUDGET_GLYPH;
  अगर (!budget) budget = &ढेर_budget;

  अचिन्हित पूर्णांक fd = fdSelect->get_fd (glyph);
  स्थिर hb_ubytes_t str = (*charStrings)[glyph];
  cff2_cs_interp_env_t<संख्या_t> env (str, *यह, fd, coords.arrayZ, coords.लंबाई);
  cff2_cs_interpreter_t<cff2_cs_opset_path_t, cff2_path_param_t, संख्या_t> interp (env);
  cff2_path_param_t param (font, draw_session);
  अगर (unlikely (!interp.interpret (param, budget))) वापस false;
  वापस true;
}

#पूर्ण_अगर

#पूर्ण_अगर /* HB_OT_CFF2_TABLE_CC pacify */
#पूर्ण_अगर /* HB_OT_CFF2_TABLE_CC guard */
