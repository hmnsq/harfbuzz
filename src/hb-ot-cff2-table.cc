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

#اگر_تعریف_نشده HB_OT_CFF2_جدول_CC
#تعریف_کن HB_OT_CFF2_جدول_CC
#اگر_تعریف_شده HB_OT_CFF2_جدول_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#اگر_تعریف_نشده HB_NO_OT_FONT_CFF

#ضمن "hb-ot-cff2-table.hh"
#ضمن "hb-cff2-interp-cs.hh"
#ضمن "hb-draw.hh"

استفاده نطاق CFF;

بنية cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.مجموعه_int (بیشترین_صحیح);
    min_y.مجموعه_int (بیشترین_صحیح);
    max_x.مجموعه_int (INT_MIN);
    max_y.مجموعه_int (INT_MIN);
  }

  فراغ   شروع_مسیر ()       { مسیر_open = درست; }
  فراغ     انتها_مسیر ()       { مسیر_open = خطأ; }
  منطقي is_مسیر_open () ثابت { أرجع مسیر_open; }

  فراغ update_bounds (ثابت نقطه_t &pt)
  {
    إذا (pt.x < min_x) min_x = pt.x;
    إذا (pt.x > max_x) max_x = pt.x;
    إذا (pt.y < min_y) min_y = pt.y;
    إذا (pt.y > max_y) max_y = pt.y;
  }

  منطقي  مسیر_open = خطأ;
  عدد_t min_x;
  عدد_t min_y;
  عدد_t max_x;
  عدد_t max_y;
};

بنية cff2_مسیر_procs_extents_t : مسیر_procs_t<cff2_مسیر_procs_extents_t, cff2_cs_interp_env_t<عدد_t>, cff2_extents_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<عدد_t> &env, cff2_extents_param_t& param, ثابت نقطه_t &pt)
  {
    param.انتها_مسیر ();
    env.moveto (pt);
  }

  ساكن فراغ سطر (cff2_cs_interp_env_t<عدد_t> &env, cff2_extents_param_t& param, ثابت نقطه_t &pt1)
  {
    إذا (!param.is_مسیر_open ())
    {
      param.شروع_مسیر ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ساكن فراغ منحنی (cff2_cs_interp_env_t<عدد_t> &env, cff2_extents_param_t& param, ثابت نقطه_t &pt1, ثابت نقطه_t &pt2, ثابت نقطه_t &pt3)
  {
    إذا (!param.is_مسیر_open ())
    {
      param.شروع_مسیر ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

بنية cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, عدد_t, cff2_مسیر_procs_extents_t> {};

منطقي OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ثابت
{
  أرجع get_extents_at (font, glyph, extents, hb_array (font->coords, font->رقم_coords));
}

منطقي OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ثابت صحيح> coords,
					      int64_t *budget) ثابت
{
#اگر_تعریف_شده HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع درست;
#پایان_اگر

  إذا (unlikely (!is_valid () || (glyph >= رقم_glyphs))) أرجع خطأ;

  int64_t پشته_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &پشته_budget;

  بیعلامت صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_رشتهها)[glyph];
  cff2_cs_interp_env_t<عدد_t> env (str, *هذا, fd, coords.arrayZ, coords.درازی);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, عدد_t> interp (env);
  cff2_extents_param_t  param;
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;

  إذا (param.min_x >= param.max_x)
  {
    extents->عرض = 0;
    extents->x_bearing = 0;
  }
  وإلا
  {
    مزدوج x_bearing = roundf (param.min_x.مقصد_real ());
    extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x_bearing);
    extents->عرض = hb_clamp_مقصد<hb_position_t> ((مزدوج) roundf (param.max_x.مقصد_real ()) - x_bearing);
  }
  إذا (param.min_y >= param.max_y)
  {
    extents->ارتفاع = 0;
    extents->y_bearing = 0;
  }
  وإلا
  {
    مزدوج y_bearing = roundf (param.max_y.مقصد_real ());
    extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
    extents->ارتفاع = hb_clamp_مقصد<hb_position_t> ((مزدوج) roundf (param.min_y.مقصد_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  أرجع درست;
}

بنية cff2_مسیر_param_t
{
  cff2_مسیر_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  فراغ move_مقصد (ثابت نقطه_t &p)
  { draw_session->move_مقصد (font->em_fscalef_x (p.x.مقصد_real ()), font->em_fscalef_y (p.y.مقصد_real ())); }

  فراغ سطر_مقصد (ثابت نقطه_t &p)
  { draw_session->سطر_مقصد (font->em_fscalef_x (p.x.مقصد_real ()), font->em_fscalef_y (p.y.مقصد_real ())); }

  فراغ cubic_مقصد (ثابت نقطه_t &p1, ثابت نقطه_t &p2, ثابت نقطه_t &p3)
  {
    draw_session->cubic_مقصد (font->em_fscalef_x (p1.x.مقصد_real ()), font->em_fscalef_y (p1.y.مقصد_real ()),
			   font->em_fscalef_x (p2.x.مقصد_real ()), font->em_fscalef_y (p2.y.مقصد_real ()),
			   font->em_fscalef_x (p3.x.مقصد_real ()), font->em_fscalef_y (p3.y.مقصد_real ()));
  }

  محمي:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

بنية cff2_مسیر_procs_مسیر_t : مسیر_procs_t<cff2_مسیر_procs_مسیر_t, cff2_cs_interp_env_t<عدد_t>, cff2_مسیر_param_t>
{
  ساكن فراغ moveto (cff2_cs_interp_env_t<عدد_t> &env, cff2_مسیر_param_t& param, ثابت نقطه_t &pt)
  {
    param.move_مقصد (pt);
    env.moveto (pt);
  }

  ساكن فراغ سطر (cff2_cs_interp_env_t<عدد_t> &env, cff2_مسیر_param_t& param, ثابت نقطه_t &pt1)
  {
    param.سطر_مقصد (pt1);
    env.moveto (pt1);
  }

  ساكن فراغ منحنی (cff2_cs_interp_env_t<عدد_t> &env, cff2_مسیر_param_t& param, ثابت نقطه_t &pt1, ثابت نقطه_t &pt2, ثابت نقطه_t &pt3)
  {
    param.cubic_مقصد (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

بنية cff2_cs_opset_مسیر_t : cff2_cs_opset_t<cff2_cs_opset_مسیر_t, cff2_مسیر_param_t, عدد_t, cff2_مسیر_procs_مسیر_t> {};

منطقي OT::cff2::accelerator_t::get_مسیر (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ثابت
{
  int64_t پشته_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &پشته_budget;

  أرجع get_مسیر_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->رقم_coords : 0),
		      budget);
}

منطقي OT::cff2::accelerator_t::get_مسیر_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget) ثابت
{
#اگر_تعریف_شده HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع درست;
#پایان_اگر

  إذا (unlikely (!is_valid () || (glyph >= رقم_glyphs))) أرجع خطأ;

  int64_t پشته_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &پشته_budget;

  بیعلامت صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_رشتهها)[glyph];
  cff2_cs_interp_env_t<عدد_t> env (str, *هذا, fd, coords.arrayZ, coords.درازی);
  cff2_cs_interpreter_t<cff2_cs_opset_مسیر_t, cff2_مسیر_param_t, عدد_t> interp (env);
  cff2_مسیر_param_t param (font, draw_session);
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;
  أرجع درست;
}

#پایان_اگر

#پایان_اگر /* HB_OT_CFF2_TABLE_CC pacify */
#پایان_اگر /* HB_OT_CFF2_TABLE_CC guard */
