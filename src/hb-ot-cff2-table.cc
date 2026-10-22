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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF2_ਸਾਰਣੀ_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CFF2_ਸਾਰਣੀ_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF2_ਸਾਰਣੀ_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff2-interp-cs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਬਣਤਰ cff2_extents_param_t
{
  cff2_extents_param_t ()
  {
    min_x.ਸੈੱਟ_int (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
    min_y.ਸੈੱਟ_int (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
    max_x.ਸੈੱਟ_int (INT_MIN);
    max_y.ਸੈੱਟ_int (INT_MIN);
  }

  ਖਾਲੀ   ਸ਼ੁਰੂ_ਰਸਤਾ ()       { ਰਸਤਾ_open = ਸੱਚ; }
  ਖਾਲੀ     ਸਿਰਾ_ਰਸਤਾ ()       { ਰਸਤਾ_open = ਝੂਠ; }
  ਤਾਰਕਿਕ is_ਰਸਤਾ_open () ਅਚਲ { ਵਾਪਸ ਰਸਤਾ_open; }

  ਖਾਲੀ update_bounds (ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    ਜੇ (pt.x < min_x) min_x = pt.x;
    ਜੇ (pt.x > max_x) max_x = pt.x;
    ਜੇ (pt.y < min_y) min_y = pt.y;
    ਜੇ (pt.y > max_y) max_y = pt.y;
  }

  ਤਾਰਕਿਕ  ਰਸਤਾ_open = ਝੂਠ;
  ਸੰਖਿਆ_t min_x;
  ਸੰਖਿਆ_t min_y;
  ਸੰਖਿਆ_t max_x;
  ਸੰਖਿਆ_t max_y;
};

ਬਣਤਰ cff2_ਰਸਤਾ_procs_extents_t : ਰਸਤਾ_procs_t<cff2_ਰਸਤਾ_procs_extents_t, cff2_cs_interp_env_t<ਸੰਖਿਆ_t>, cff2_extents_param_t>
{
  ਸਥਿਰ ਖਾਲੀ moveto (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    param.ਸਿਰਾ_ਰਸਤਾ ();
    env.moveto (pt);
  }

  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1)
  {
    ਜੇ (!param.is_ਰਸਤਾ_open ())
    {
      param.ਸ਼ੁਰੂ_ਰਸਤਾ ();
      param.update_bounds (env.get_pt ());
    }
    env.moveto (pt1);
    param.update_bounds (env.get_pt ());
  }

  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3)
  {
    ਜੇ (!param.is_ਰਸਤਾ_open ())
    {
      param.ਸ਼ੁਰੂ_ਰਸਤਾ ();
      param.update_bounds (env.get_pt ());
    }
    /* include control points */
    param.update_bounds (pt1);
    param.update_bounds (pt2);
    env.moveto (pt3);
    param.update_bounds (env.get_pt ());
  }
};

ਬਣਤਰ cff2_cs_opset_extents_t : cff2_cs_opset_t<cff2_cs_opset_extents_t, cff2_extents_param_t, ਸੰਖਿਆ_t, cff2_ਰਸਤਾ_procs_extents_t> {};

ਤਾਰਕਿਕ OT::cff2::accelerator_t::get_extents (hb_font_t *font,
					   hb_codepoint_t glyph,
					   hb_glyph_extents_t *extents) ਅਚਲ
{
  ਵਾਪਸ get_extents_at (font, glyph, extents, hb_array (font->coords, font->ਅੰਕ_coords));
}

ਤਾਰਕਿਕ OT::cff2::accelerator_t::get_extents_at (hb_font_t *font,
					      hb_codepoint_t glyph,
					      hb_glyph_extents_t *extents,
					      hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
					      int64_t *budget) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  ਜੇ (unlikely (!is_valid () || (glyph >= ਅੰਕ_glyphs))) ਵਾਪਸ ਝੂਠ;

  int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
  ਜੇ (!budget) budget = &ਢੇਰ_budget;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = fdSelect->get_fd (glyph);
  ਅਚਲ hb_ubytes_t str = (*char_ਲੜੀਆਂ)[glyph];
  cff2_cs_interp_env_t<ਸੰਖਿਆ_t> env (str, *ਇਹ, fd, coords.arrayZ, coords.ਲੰਮਾਈ);
  cff2_cs_interpreter_t<cff2_cs_opset_extents_t, cff2_extents_param_t, ਸੰਖਿਆ_t> interp (env);
  cff2_extents_param_t  param;
  ਜੇ (unlikely (!interp.interpret (param, budget))) ਵਾਪਸ ਝੂਠ;

  ਜੇ (param.min_x >= param.max_x)
  {
    extents->ਚੌੜਾਈ = 0;
    extents->x_bearing = 0;
  }
  ਨਹੀਂਤਾਂ
  {
    ਦੁੱਗਣਾ x_bearing = roundf (param.min_x.ਮੰਜ਼ਿਲ_real ());
    extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x_bearing);
    extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (param.max_x.ਮੰਜ਼ਿਲ_real ()) - x_bearing);
  }
  ਜੇ (param.min_y >= param.max_y)
  {
    extents->ਉਚਾਈ = 0;
    extents->y_bearing = 0;
  }
  ਨਹੀਂਤਾਂ
  {
    ਦੁੱਗਣਾ y_bearing = roundf (param.max_y.ਮੰਜ਼ਿਲ_real ());
    extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (y_bearing);
    extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (param.min_y.ਮੰਜ਼ਿਲ_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  ਵਾਪਸ ਸੱਚ;
}

ਬਣਤਰ cff2_ਰਸਤਾ_param_t
{
  cff2_ਰਸਤਾ_param_t (hb_font_t *font_, hb_draw_session_t &draw_session_)
  {
    draw_session = &draw_session_;
    font = font_;
  }

  ਖਾਲੀ move_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p)
  { draw_session->move_ਮੰਜ਼ਿਲ (font->em_fscalef_x (p.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (p.y.ਮੰਜ਼ਿਲ_real ())); }

  ਖਾਲੀ ਸਤਰ_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p)
  { draw_session->ਸਤਰ_ਮੰਜ਼ਿਲ (font->em_fscalef_x (p.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (p.y.ਮੰਜ਼ਿਲ_real ())); }

  ਖਾਲੀ cubic_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p1, ਅਚਲ ਬਿੰਦੂ_t &p2, ਅਚਲ ਬਿੰਦੂ_t &p3)
  {
    draw_session->cubic_ਮੰਜ਼ਿਲ (font->em_fscalef_x (p1.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (p1.y.ਮੰਜ਼ਿਲ_real ()),
			   font->em_fscalef_x (p2.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (p2.y.ਮੰਜ਼ਿਲ_real ()),
			   font->em_fscalef_x (p3.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (p3.y.ਮੰਜ਼ਿਲ_real ()));
  }

  ਸੁਰੱਖਿਅਤ:
  hb_draw_session_t *draw_session;
  hb_font_t *font;
};

ਬਣਤਰ cff2_ਰਸਤਾ_procs_ਰਸਤਾ_t : ਰਸਤਾ_procs_t<cff2_ਰਸਤਾ_procs_ਰਸਤਾ_t, cff2_cs_interp_env_t<ਸੰਖਿਆ_t>, cff2_ਰਸਤਾ_param_t>
{
  ਸਥਿਰ ਖਾਲੀ moveto (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_ਰਸਤਾ_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    param.move_ਮੰਜ਼ਿਲ (pt);
    env.moveto (pt);
  }

  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_ਰਸਤਾ_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1)
  {
    param.ਸਤਰ_ਮੰਜ਼ਿਲ (pt1);
    env.moveto (pt1);
  }

  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (cff2_cs_interp_env_t<ਸੰਖਿਆ_t> &env, cff2_ਰਸਤਾ_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3)
  {
    param.cubic_ਮੰਜ਼ਿਲ (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ਬਣਤਰ cff2_cs_opset_ਰਸਤਾ_t : cff2_cs_opset_t<cff2_cs_opset_ਰਸਤਾ_t, cff2_ਰਸਤਾ_param_t, ਸੰਖਿਆ_t, cff2_ਰਸਤਾ_procs_ਰਸਤਾ_t> {};

ਤਾਰਕਿਕ OT::cff2::accelerator_t::get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t glyph,
				       hb_draw_session_t &draw_session,
				       int64_t *budget) ਅਚਲ
{
  int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
  ਜੇ (!budget) budget = &ਢੇਰ_budget;

  ਵਾਪਸ get_ਰਸਤਾ_at (font,
		      glyph,
		      draw_session,
		      hb_array (font->coords,
				font->has_nonzero_coords ? font->ਅੰਕ_coords : 0),
		      budget);
}

ਤਾਰਕਿਕ OT::cff2::accelerator_t::get_ਰਸਤਾ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ਅਚਲ ਪੂਰਨ> coords, int64_t *budget) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  ਜੇ (unlikely (!is_valid () || (glyph >= ਅੰਕ_glyphs))) ਵਾਪਸ ਝੂਠ;

  int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
  ਜੇ (!budget) budget = &ਢੇਰ_budget;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = fdSelect->get_fd (glyph);
  ਅਚਲ hb_ubytes_t str = (*char_ਲੜੀਆਂ)[glyph];
  cff2_cs_interp_env_t<ਸੰਖਿਆ_t> env (str, *ਇਹ, fd, coords.arrayZ, coords.ਲੰਮਾਈ);
  cff2_cs_interpreter_t<cff2_cs_opset_ਰਸਤਾ_t, cff2_ਰਸਤਾ_param_t, ਸੰਖਿਆ_t> interp (env);
  cff2_ਰਸਤਾ_param_t param (font, draw_session);
  ਜੇ (unlikely (!interp.interpret (param, budget))) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ ਸੱਚ;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_OT_CFF2_TABLE_CC pacify */
#ਜੇ_ਅੰਤ /* HB_OT_CFF2_TABLE_CC guard */
