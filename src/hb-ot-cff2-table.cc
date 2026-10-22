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

#اگر_غیر_تعریف_شدہ HB_OT_CFF2_جدول_CC
#تعریف_کرو HB_OT_CFF2_جدول_CC
#اگر_تعریف_شدہ HB_OT_CFF2_جدول_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_FONT_CFF

#شامل "hb-ot-cff2-table.hh"
#شامل "hb-cff2-interp-cs.hh"
#شامل "hb-draw.hh"

استعمال نامخلا CFF;

ساخت cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.سیٹ_int (زیادہ_سے_زیادہ_عدد);
    min_y.سیٹ_int (زیادہ_سے_زیادہ_عدد);
    max_x.سیٹ_int (INT_MIN);
    max_y.سیٹ_int (INT_MIN);
  }

  خالی   آغاز_راستہ ()       { راستہ_open = سچ; }
  خالی     انتہا_راستہ ()       { راستہ_open = جھوٹ; }
  منطقی is_راستہ_open () ثابت { واپس راستہ_open; }

  خالی update_bounds (ثابت نقطہ_t &pt)
  {
    اگر (pt.x < min_x) min_x = pt.x;
    اگر (pt.x > max_x) max_x = pt.x;
    اگر (pt.y < min_y) min_y = pt.y;
    اگر (pt.y > max_y) max_y = pt.y;
  }

  منطقی  راستہ_open = جھوٹ;
  نمبر_t min_x;
  نمبر_t min_y;
  نمبر_t max_x;
  نمبر_t max_y;
};

ساخت cff2_راستہ_procs_extents_t : راستہ_procs_t<cff2_راستہ_procs_extents_t, cff2_cs_interp_env_t<نمبر_t>, cff2_extents_param_t>
{
  ساکن خالی moveto (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt)
  {
    param.انتہا_راستہ ();
    env.moveto (pt);
  }

  ساکن خالی سطر (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt1)
  {
    اگر (!param.is_راستہ_open ())
    {
      param.آغاز_راستہ ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ساکن خالی خم (cff2_cs_interp_env_t<نمبر_t> &env, cff2_extents_param_t& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    اگر (!param.is_راستہ_open ())
    {
      param.آغاز_راستہ ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

ساخت cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, نمبر_t, cff2_راستہ_procs_extents_t> {};

منطقی OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ثابت
{
  واپس get_extents_at (font, glyph, extents, hb_array (font->coords, font->عدد_coords));
}

منطقی OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ثابت صحیح> coords,
					      int64_t *budget) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  واپس سچ;
#اگر_ختم

  اگر (unlikely (!is_valid () || (glyph >= عدد_glyphs))) واپس جھوٹ;

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  اگر (!budget) budget = &ڈھیر_budget;

  غیرعلامتی صحیح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڑیاں)[glyph];
  cff2_cs_interp_env_t<نمبر_t> env (str, *یہ, fd, coords.arrayZ, coords.لمبائی);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, نمبر_t> interp (env);
  cff2_extents_param_t  param;
  اگر (unlikely (!interp.interpret (param, budget))) واپس جھوٹ;

  اگر (param.min_x >= param.max_x)
  {
    extents->چوڑائی = 0;
    extents->x_bearing = 0;
  }
  ورنہ
  {
    دوہرا x_bearing = roundf (param.min_x.مقصد_real ());
    extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x_bearing);
    extents->چوڑائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (param.max_x.مقصد_real ()) - x_bearing);
  }
  اگر (param.min_y >= param.max_y)
  {
    extents->اونچائی = 0;
    extents->y_bearing = 0;
  }
  ورنہ
  {
    دوہرا y_bearing = roundf (param.max_y.مقصد_real ());
    extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
    extents->اونچائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (param.min_y.مقصد_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  واپس سچ;
}

ساخت cff2_راستہ_param_t
{
  cff2_راستہ_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  خالی move_مقصد (ثابت نقطہ_t &p)
  { draw_session->move_مقصد (font->em_fscalef_x (p.x.مقصد_real ()), font->em_fscalef_y (p.y.مقصد_real ())); }

  خالی سطر_مقصد (ثابت نقطہ_t &p)
  { draw_session->سطر_مقصد (font->em_fscalef_x (p.x.مقصد_real ()), font->em_fscalef_y (p.y.مقصد_real ())); }

  خالی cubic_مقصد (ثابت نقطہ_t &p1, ثابت نقطہ_t &p2, ثابت نقطہ_t &p3)
  {
    draw_session->cubic_مقصد (font->em_fscalef_x (p1.x.مقصد_real ()), font->em_fscalef_y (p1.y.مقصد_real ()),
			   font->em_fscalef_x (p2.x.مقصد_real ()), font->em_fscalef_y (p2.y.مقصد_real ()),
			   font->em_fscalef_x (p3.x.مقصد_real ()), font->em_fscalef_y (p3.y.مقصد_real ()));
  }

  محفوظ:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

ساخت cff2_راستہ_procs_راستہ_t : راستہ_procs_t<cff2_راستہ_procs_راستہ_t, cff2_cs_interp_env_t<نمبر_t>, cff2_راستہ_param_t>
{
  ساکن خالی moveto (cff2_cs_interp_env_t<نمبر_t> &env, cff2_راستہ_param_t& param, ثابت نقطہ_t &pt)
  {
    param.move_مقصد (pt);
    env.moveto (pt);
  }

  ساکن خالی سطر (cff2_cs_interp_env_t<نمبر_t> &env, cff2_راستہ_param_t& param, ثابت نقطہ_t &pt1)
  {
    param.سطر_مقصد (pt1);
    env.moveto (pt1);
  }

  ساکن خالی خم (cff2_cs_interp_env_t<نمبر_t> &env, cff2_راستہ_param_t& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    param.cubic_مقصد (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ساخت cff2_cs_opset_راستہ_t : cff2_cs_opset_t<cff2_cs_opset_راستہ_t, cff2_راستہ_param_t, نمبر_t, cff2_راستہ_procs_راستہ_t> {};

منطقی OT::cff2::accelerator_t::get_راستہ (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ثابت
{
  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  اگر (!budget) budget = &ڈھیر_budget;

  واپس get_راستہ_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->عدد_coords : 0),
		      budget);
}

منطقی OT::cff2::accelerator_t::get_راستہ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحیح> coords, int64_t *budget) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  واپس سچ;
#اگر_ختم

  اگر (unlikely (!is_valid () || (glyph >= عدد_glyphs))) واپس جھوٹ;

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  اگر (!budget) budget = &ڈھیر_budget;

  غیرعلامتی صحیح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڑیاں)[glyph];
  cff2_cs_interp_env_t<نمبر_t> env (str, *یہ, fd, coords.arrayZ, coords.لمبائی);
  cff2_cs_interpreter_t<cff2_cs_opset_راستہ_t, cff2_راستہ_param_t, نمبر_t> interp (env);
  cff2_راستہ_param_t param (font, draw_session);
  اگر (unlikely (!interp.interpret (param, budget))) واپس جھوٹ;
  واپس سچ;
}

#اگر_ختم

#اگر_ختم /* HB_OT_CFF2_TABLE_CC pacify */
#اگر_ختم /* HB_OT_CFF2_TABLE_CC guard */
