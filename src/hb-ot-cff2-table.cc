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

#که_نه_تعریف_شوی HB_OT_CFF2_جدول_CC
#تعریف_کړه HB_OT_CFF2_جدول_CC
#که_تعریف_شوی HB_OT_CFF2_جدول_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_OT_FONT_CFF

#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-draw.hh"

کارول نطاق CFF;

بنية cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.سېټ_int (لوی_عدد);
    min_y.سېټ_int (لوی_عدد);
    max_x.سېټ_int (INT_MIN);
    max_y.سېټ_int (INT_MIN);
  }

  فراغ   پیل_لار ()       { لار_open = ریښتیا; }
  فراغ     پای_ټکی_لار ()       { لار_open = خطأ; }
  منطقي is_لار_open () ثابت { أرجع لار_open; }

  فراغ update_bounds (ثابت ټکی_t &pt)
  {
    إذا (pt.x < min_x) min_x = pt.x;
    إذا (pt.x > max_x) max_x = pt.x;
    إذا (pt.y < min_y) min_y = pt.y;
    إذا (pt.y > max_y) max_y = pt.y;
  }

  منطقي  لار_open = خطأ;
  شمېره_t min_x;
  شمېره_t min_y;
  شمېره_t max_x;
  شمېره_t max_y;
};

بنية cff2_لار_procs_extents_t : لار_procs_t<cff2_لار_procs_extents_t, cff2_cs_interp_env_t<شمېره_t>, cff2_extents_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<شمېره_t> &env, cff2_extents_param_t& param, ثابت ټکی_t &pt)
  {
    param.پای_ټکی_لار ();
    env.moveto (pt);
  }

  ساكن فراغ کرښه (cff2_cs_interp_env_t<شمېره_t> &env, cff2_extents_param_t& param, ثابت ټکی_t &pt1)
  {
    إذا (!param.is_لار_open ())
    {
      param.پیل_لار ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ساكن فراغ منحني (cff2_cs_interp_env_t<شمېره_t> &env, cff2_extents_param_t& param, ثابت ټکی_t &pt1, ثابت ټکی_t &pt2, ثابت ټکی_t &pt3)
  {
    إذا (!param.is_لار_open ())
    {
      param.پیل_لار ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

بنية cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, شمېره_t, cff2_لار_procs_extents_t> {};

منطقي OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ثابت
{
  أرجع get_extents_at (font, glyph, extents, hb_array (font->coords, font->عدد_coords));
}

منطقي OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ثابت صحيح> coords,
					      int64_t *budget) ثابت
{
#که_تعریف_شوی HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع ریښتیا;
#د_که_پای

  إذا (unlikely (!is_valid () || (glyph >= عدد_glyphs))) أرجع خطأ;

  int64_t ډېری_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ډېری_budget;

  بینښه صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_مزي_ګانې)[glyph];
  cff2_cs_interp_env_t<شمېره_t> env (str, *هذا, fd, coords.arrayZ, coords.اوږدتیا);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, شمېره_t> interp (env);
  cff2_extents_param_t  param;
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;

  إذا (param.min_x >= param.max_x)
  {
    extents->پلنوالی = 0;
    extents->x_bearing = 0;
  }
  وإلا
  {
    مزدوج x_bearing = roundf (param.min_x.منزل_real ());
    extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
    extents->پلنوالی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (param.max_x.منزل_real ()) - x_bearing);
  }
  إذا (param.min_y >= param.max_y)
  {
    extents->لوړوالی = 0;
    extents->y_bearing = 0;
  }
  وإلا
  {
    مزدوج y_bearing = roundf (param.max_y.منزل_real ());
    extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
    extents->لوړوالی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (param.min_y.منزل_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  أرجع ریښتیا;
}

بنية cff2_لار_param_t
{
  cff2_لار_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  فراغ move_منزل (ثابت ټکی_t &p)
  { draw_session->move_منزل (font->em_fscalef_x (p.x.منزل_real ()), font->em_fscalef_y (p.y.منزل_real ())); }

  فراغ کرښه_منزل (ثابت ټکی_t &p)
  { draw_session->کرښه_منزل (font->em_fscalef_x (p.x.منزل_real ()), font->em_fscalef_y (p.y.منزل_real ())); }

  فراغ cubic_منزل (ثابت ټکی_t &p1, ثابت ټکی_t &p2, ثابت ټکی_t &p3)
  {
    draw_session->cubic_منزل (font->em_fscalef_x (p1.x.منزل_real ()), font->em_fscalef_y (p1.y.منزل_real ()),
			   font->em_fscalef_x (p2.x.منزل_real ()), font->em_fscalef_y (p2.y.منزل_real ()),
			   font->em_fscalef_x (p3.x.منزل_real ()), font->em_fscalef_y (p3.y.منزل_real ()));
  }

  محمي:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

بنية cff2_لار_procs_لار_t : لار_procs_t<cff2_لار_procs_لار_t, cff2_cs_interp_env_t<شمېره_t>, cff2_لار_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<شمېره_t> &env, cff2_لار_param_t& param, ثابت ټکی_t &pt)
  {
    param.move_منزل (pt);
    env.moveto (pt);
  }

  ساكن فراغ کرښه (cff2_cs_interp_env_t<شمېره_t> &env, cff2_لار_param_t& param, ثابت ټکی_t &pt1)
  {
    param.کرښه_منزل (pt1);
    env.moveto (pt1);
  }

  ساكن فراغ منحني (cff2_cs_interp_env_t<شمېره_t> &env, cff2_لار_param_t& param, ثابت ټکی_t &pt1, ثابت ټکی_t &pt2, ثابت ټکی_t &pt3)
  {
    param.cubic_منزل (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

بنية cff2_cs_opset_لار_t : cff2_cs_opset_t<cff2_cs_opset_لار_t, cff2_لار_param_t, شمېره_t, cff2_لار_procs_لار_t> {};

منطقي OT::cff2::accelerator_t::get_لار (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ثابت
{
  int64_t ډېری_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ډېری_budget;

  أرجع get_لار_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->عدد_coords : 0),
		      budget);
}

منطقي OT::cff2::accelerator_t::get_لار_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget) ثابت
{
#که_تعریف_شوی HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع ریښتیا;
#د_که_پای

  إذا (unlikely (!is_valid () || (glyph >= عدد_glyphs))) أرجع خطأ;

  int64_t ډېری_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ډېری_budget;

  بینښه صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_مزي_ګانې)[glyph];
  cff2_cs_interp_env_t<شمېره_t> env (str, *هذا, fd, coords.arrayZ, coords.اوږدتیا);
  cff2_cs_interpreter_t<cff2_cs_opset_لار_t, cff2_لار_param_t, شمېره_t> interp (env);
  cff2_لار_param_t param (font, draw_session);
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;
  أرجع ریښتیا;
}

#د_که_پای

#د_که_پای /* HB_OT_CFF2_TABLE_CC pacify */
#د_که_پای /* HB_OT_CFF2_TABLE_CC guard */
