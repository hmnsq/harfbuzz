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

#جے_بنا_تعریف HB_OT_CFF2_جدول_CC
#تعریف_کرو HB_OT_CFF2_جدول_CC
#جے_تعریف_شدہ HB_OT_CFF2_جدول_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#جے_بنا_تعریف HB_NO_OT_FONT_CFF

#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-draw.hh"

ورتوں نطاق CFF;

بنية cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.سیٹ_int (ودھ_توں_ودھ_عدد);
    min_y.سیٹ_int (ودھ_توں_ودھ_عدد);
    max_x.سیٹ_int (INT_MIN);
    max_y.سیٹ_int (INT_MIN);
  }

  فراغ   شروع_رستہ ()       { رستہ_open = سچ; }
  فراغ     سرا_رستہ ()       { رستہ_open = خطأ; }
  منطقي is_رستہ_open () ثابت { أرجع رستہ_open; }

  فراغ update_bounds (ثابت نقطہ_t &pt)
  {
    إذا (pt.x < min_x) min_x = pt.x;
    إذا (pt.x > max_x) max_x = pt.x;
    إذا (pt.y < min_y) min_y = pt.y;
    إذا (pt.y > max_y) max_y = pt.y;
  }

  منطقي  رستہ_open = خطأ;
  نمبر_t min_x;
  نمبر_t min_y;
  نمبر_t max_x;
  نمبر_t max_y;
};

بنية cff2_رستہ_procs_extents_t : رستہ_procs_t<cff2_رستہ_procs_extents_t, cff2_cs_interp_env_t<نمبر_t>, cff2_extents_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt)
  {
    param.سرا_رستہ ();
    env.moveto (pt);
  }

  ساكن فراغ سطر (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt1)
  {
    إذا (!param.is_رستہ_open ())
    {
      param.شروع_رستہ ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ساكن فراغ ونگ (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    إذا (!param.is_رستہ_open ())
    {
      param.شروع_رستہ ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

بنية cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, نمبر_t, cff2_رستہ_procs_extents_t> {};

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
#جے_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع سچ;
#جے_انت

  إذا (unlikely (!is_valid () || (glyph >= عدد_glyphs))) أرجع خطأ;

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ڈھیر_budget;

  بنانشانی صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڑیاں)[glyph];
  cff2_cs_interp_env_t<نمبر_t> env (str, *هذا, fd, coords.arrayZ, coords.لمائی);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, نمبر_t> interp (env);
  cff2_extents_param_t  param;
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;

  إذا (param.min_x >= param.max_x)
  {
    extents->چوڑائی = 0;
    extents->x_bearing = 0;
  }
  وإلا
  {
    مزدوج x_bearing = roundf (param.min_x.منزل_real ());
    extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
    extents->چوڑائی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (param.max_x.منزل_real ()) - x_bearing);
  }
  إذا (param.min_y >= param.max_y)
  {
    extents->اچائی = 0;
    extents->y_bearing = 0;
  }
  وإلا
  {
    مزدوج y_bearing = roundf (param.max_y.منزل_real ());
    extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
    extents->اچائی = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (param.min_y.منزل_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  أرجع سچ;
}

بنية cff2_رستہ_param_t
{
  cff2_رستہ_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  فراغ move_منزل (ثابت نقطہ_t &p)
  { draw_session->move_منزل (font->em_fscalef_x (p.x.منزل_real ()), font->em_fscalef_y (p.y.منزل_real ())); }

  فراغ سطر_منزل (ثابت نقطہ_t &p)
  { draw_session->سطر_منزل (font->em_fscalef_x (p.x.منزل_real ()), font->em_fscalef_y (p.y.منزل_real ())); }

  فراغ cubic_منزل (ثابت نقطہ_t &p1, ثابت نقطہ_t &p2, ثابت نقطہ_t &p3)
  {
    draw_session->cubic_منزل (font->em_fscalef_x (p1.x.منزل_real ()), font->em_fscalef_y (p1.y.منزل_real ()),
			   font->em_fscalef_x (p2.x.منزل_real ()), font->em_fscalef_y (p2.y.منزل_real ()),
			   font->em_fscalef_x (p3.x.منزل_real ()), font->em_fscalef_y (p3.y.منزل_real ()));
  }

  محمي:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

بنية cff2_رستہ_procs_رستہ_t : رستہ_procs_t<cff2_رستہ_procs_رستہ_t, cff2_cs_interp_env_t<نمبر_t>, cff2_رستہ_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<نمبر_t> &env, cff2_رستہ_param_t& param, ثابت نقطہ_t &pt)
  {
    param.move_منزل (pt);
    env.moveto (pt);
  }

  ساكن فراغ سطر (cff2_cs_interp_env_t<نمبر_t> &env, cff2_رستہ_param_t& param, ثابت نقطہ_t &pt1)
  {
    param.سطر_منزل (pt1);
    env.moveto (pt1);
  }

  ساكن فراغ ونگ (cff2_cs_interp_env_t<نمبر_t> &env, cff2_رستہ_param_t& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    param.cubic_منزل (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

بنية cff2_cs_opset_رستہ_t : cff2_cs_opset_t<cff2_cs_opset_رستہ_t, cff2_رستہ_param_t, نمبر_t, cff2_رستہ_procs_رستہ_t> {};

منطقي OT::cff2::accelerator_t::get_رستہ (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ثابت
{
  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ڈھیر_budget;

  أرجع get_رستہ_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->عدد_coords : 0),
		      budget);
}

منطقي OT::cff2::accelerator_t::get_رستہ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget) ثابت
{
#جے_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع سچ;
#جے_انت

  إذا (unlikely (!is_valid () || (glyph >= عدد_glyphs))) أرجع خطأ;

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ڈھیر_budget;

  بنانشانی صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڑیاں)[glyph];
  cff2_cs_interp_env_t<نمبر_t> env (str, *هذا, fd, coords.arrayZ, coords.لمائی);
  cff2_cs_interpreter_t<cff2_cs_opset_رستہ_t, cff2_رستہ_param_t, نمبر_t> interp (env);
  cff2_رستہ_param_t param (font, draw_session);
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;
  أرجع سچ;
}

#جے_انت

#جے_انت /* HB_OT_CFF2_TABLE_CC pacify */
#جے_انت /* HB_OT_CFF2_TABLE_CC guard */
