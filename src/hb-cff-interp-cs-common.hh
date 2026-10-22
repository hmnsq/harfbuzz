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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_INTERP_CS_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_INTERP_CS_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਗਿਣਤੀ cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ਬਣਤਰ call_context_t
{
  ਖਾਲੀ init (ਅਚਲ byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ=0)
  {
    str_ref = substr_;
    type = type_;
    subr_ਅੰਕ_2 = subr_ਅੰਕ;
  }

  ਖਾਲੀ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subr_ਅੰਕ_2;
};

/* call stack */
ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k_Max_Call_ਹੱਦਬੰਦੀ = 10;
ਬਣਤਰ call_ਢੇਰ_t : cff_ਢੇਰ_t<call_context_t, k_Max_Call_ਹੱਦਬੰਦੀ> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ SUBRS>
ਬਣਤਰ biased_subrs_t
{
  ਖਾਲੀ init (ਅਚਲ SUBRS *subrs_)
  {
    subrs = subrs_;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  nSubrs = get_ਗਿਣਤੀ ();
    ਜੇ (nSubrs < 1240)
      bias = 107;
    ਨਹੀਂਤਾਂ ਜੇ (nSubrs < 33900)
      bias = 1131;
    ਨਹੀਂਤਾਂ
      bias = 32768;
  }

  ਖਾਲੀ fini () {}

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ subrs ? subrs->ਗਿਣਤੀ_2 : 0; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_bias () ਅਚਲ  { ਵਾਪਸ bias; }

  hb_ubytes_t ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2) ਅਚਲ
  {
    ਜੇ (unlikely (!subrs || ਸੂਚਕਾਂਕ_2 >= subrs->ਗਿਣਤੀ_2))
      ਵਾਪਸ hb_ubytes_t ();
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ (*subrs)[ਸੂਚਕਾਂਕ_2];
  }

  ਸੁਰੱਖਿਅਤ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  bias;
  ਅਚਲ SUBRS   *subrs;
};

ਬਣਤਰ ਬਿੰਦੂ_t
{
  ਖਾਲੀ ਸੈੱਟ_int (ਪੂਰਨ _x, ਪੂਰਨ _y)
  {
    x.ਸੈੱਟ_int (_x);
    y.ਸੈੱਟ_int (_y);
  }

  ਖਾਲੀ move_x (ਅਚਲ ਸੰਖਿਆ_t &dx) { x += dx; }
  ਖਾਲੀ move_y (ਅਚਲ ਸੰਖਿਆ_t &dy) { y += dy; }
  ਖਾਲੀ move (ਅਚਲ ਸੰਖਿਆ_t &dx, ਅਚਲ ਸੰਖਿਆ_t &dy) { move_x (dx); move_y (dy); }
  ਖਾਲੀ move (ਅਚਲ ਬਿੰਦੂ_t &d) { move_x (d.x); move_y (d.y); }

  ਸੰਖਿਆ_t  x;
  ਸੰਖਿਆ_t  y;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ARG, ਕਿਸਮ_ਨਾਂ SUBRS>
ਬਣਤਰ cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ਅਚਲ hb_ubytes_t &str, ਅਚਲ SUBRS *globalSubrs_, ਅਚਲ SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    ਵੇਖਿਆ_moveto = ਸੱਚ;
    ਵੇਖਿਆ_hintmask = ਝੂਠ;
    hstem_ਗਿਣਤੀ = 0;
    vstem_ਗਿਣਤੀ = 0;
    hintmask_ਮਾਪ = 0;
    budget = ਖਾਲੀ_ਸੂਚਕ;
    pt.ਸੈੱਟ_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  {
    ਵਾਪਸ call_ਢੇਰ.ਵਿੱਚ_error () || SUPER::ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ ਕੱਢੋ_subr_ਅੰਕ (ਅਚਲ biased_subrs_t<SUBRS>& biasedSubrs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subr_ਅੰਕ_2)
  {
    subr_ਅੰਕ_2 = 0;
    ਪੂਰਨ n = SUPER::arg_ਢੇਰ.ਕੱਢੋ_int ();
    n += biasedSubrs.get_bias ();
    ਜੇ (unlikely ((n < 0) || ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)n >= biasedSubrs.get_ਗਿਣਤੀ ())))
      ਵਾਪਸ ਝੂਠ;

    subr_ਅੰਕ_2 = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)n;
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ call_subr (ਅਚਲ biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ_2 = 0;

    ਜੇ (unlikely (!ਕੱਢੋ_subr_ਅੰਕ (biasedSubrs, subr_ਅੰਕ_2)
		 || call_ਢੇਰ.get_ਗਿਣਤੀ () >= k_Max_Call_ਹੱਦਬੰਦੀ))
    {
      SUPER::ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }
    context.str_ref = SUPER::str_ref;
    call_ਢੇਰ.push (context);

    context.init ( biasedSubrs[subr_ਅੰਕ_2], type, subr_ਅੰਕ_2);
    SUPER::str_ref = context.str_ref;
  }

  ਖਾਲੀ return_from_subr ()
  {
    ਜੇ (unlikely (SUPER::str_ref.ਵਿੱਚ_error ()))
      SUPER::ਸੈੱਟ_error ();
    context = call_ਢੇਰ.ਕੱਢੋ ();
    SUPER::str_ref = context.str_ref;
  }

  ਖਾਲੀ determine_hintmask_ਮਾਪ ()
  {
    ਜੇ (!ਵੇਖਿਆ_hintmask)
    {
      vstem_ਗਿਣਤੀ += SUPER::arg_ਢੇਰ.get_ਗਿਣਤੀ() / 2;
      hintmask_ਮਾਪ = (hstem_ਗਿਣਤੀ + vstem_ਗਿਣਤੀ + 7) >> 3;
      ਵੇਖਿਆ_hintmask = ਸੱਚ;
    }
  }

  ਖਾਲੀ ਸੈੱਟ_endchar (ਤਾਰਕਿਕ endchar_flag_) { endchar_flag = endchar_flag_; }
  ਤਾਰਕਿਕ is_endchar () ਅਚਲ { ਵਾਪਸ endchar_flag; }

  ਖਾਲੀ ਸੈੱਟ_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ਅਚਲ { ਵਾਪਸ budget; }
  ਤਾਰਕਿਕ spend_budget (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲਾਗਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mult = 1)
  { ਵਾਪਸ !budget || hb_budget_spend (*budget, ਲਾਗਤ, mult); }

  ਅਚਲ ਸੰਖਿਆ_t &get_x () ਅਚਲ { ਵਾਪਸ pt.x; }
  ਅਚਲ ਸੰਖਿਆ_t &get_y () ਅਚਲ { ਵਾਪਸ pt.y; }
  ਅਚਲ ਬਿੰਦੂ_t &get_pt () ਅਚਲ { ਵਾਪਸ pt; }

  ਖਾਲੀ moveto (ਅਚਲ ਬਿੰਦੂ_t &pt_ ) { pt = pt_; }

  ਜਨਤਕ:
  call_context_t   context;
  ਤਾਰਕਿਕ	  endchar_flag;
  ਤਾਰਕਿਕ	  ਵੇਖਿਆ_moveto;
  ਤਾਰਕਿਕ	  ਵੇਖਿਆ_hintmask;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  hstem_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  vstem_ਗਿਣਤੀ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  hintmask_ਮਾਪ;
  int64_t	*budget;
  call_ਢੇਰ_t	call_ਢੇਰ;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  ਨਿਜੀ:
  ਬਿੰਦੂ_t	 pt;

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ interp_env_t<ARG> SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ PARAM>
ਬਣਤਰ ਰਸਤਾ_procs_null_t
{
  ਸਥਿਰ ਖਾਲੀ rmoveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ hmoveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ vmoveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ rlineto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ hlineto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ vlineto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ rrcurveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ rcurveline (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ rlinecurve (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ vvcurveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ hhcurveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ vhcurveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ hvcurveto (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ moveto (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt) {}
  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt1) {}
  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3) {}
  ਸਥਿਰ ਖਾਲੀ hflex (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ flex (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ hflex1 (ENV &env, PARAM& param) {}
  ਸਥਿਰ ਖਾਲੀ flex1 (ENV &env, PARAM& param) {}
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ARG, ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ PARAM, ਕਿਸਮ_ਨਾਂ ਰਸਤਾ_3=ਰਸਤਾ_procs_null_t<ENV, PARAM>>
ਬਣਤਰ cs_opset_t : opset_t<ARG>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    ਚੋਣ (op) {

      ਹਾਲਤ Op_ਕੋਡ_return:
	env.return_from_subr ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_endchar:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	env.ਸੈੱਟ_endchar (ਸੱਚ);
	OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_fixedcs:
	env.arg_ਢੇਰ.ਧੱਕੋ_fixed_from_substr (env.str_ref);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_hstem:
      ਹਾਲਤ Op_ਕੋਡ_hstemhm:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	OPSET::process_hstem (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vstem:
      ਹਾਲਤ Op_ਕੋਡ_vstemhm:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	OPSET::process_vstem (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_hintmask:
      ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	OPSET::process_hintmask (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_rmoveto:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	ਰਸਤਾ_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_hmoveto:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	ਰਸਤਾ_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vmoveto:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	ਰਸਤਾ_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_rlineto:
	ਰਸਤਾ_3::rlineto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_hlineto:
	ਰਸਤਾ_3::hlineto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vlineto:
	ਰਸਤਾ_3::vlineto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_rrcurveto:
	ਰਸਤਾ_3::rrcurveto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_rcurveline:
	ਰਸਤਾ_3::rcurveline (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_rlinecurve:
	ਰਸਤਾ_3::rlinecurve (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vvcurveto:
	ਰਸਤਾ_3::vvcurveto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_hhcurveto:
	ਰਸਤਾ_3::hhcurveto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vhcurveto:
	ਰਸਤਾ_3::vhcurveto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_hvcurveto:
	ਰਸਤਾ_3::hvcurveto (env, param);
	process_post_ਰਸਤਾ (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_hflex:
	ਰਸਤਾ_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_flex:
	ਰਸਤਾ_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_hflex1:
	ਰਸਤਾ_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_flex1:
	ਰਸਤਾ_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env);
	ਰੋਕੋ;
    }
  }

  ਸਥਿਰ ਖਾਲੀ process_hstem (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    env.hstem_ਗਿਣਤੀ += env.arg_ਢੇਰ.get_ਗਿਣਤੀ () / 2;
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ process_vstem (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    env.vstem_ਗਿਣਤੀ += env.arg_ਢੇਰ.get_ਗਿਣਤੀ () / 2;
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ process_hintmask (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_ਮਾਪ ();
    ਜੇ (likely (env.str_ref.avail (env.hintmask_ਮਾਪ)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_ਮਾਪ);
    }
  }

  ਸਥਿਰ ਖਾਲੀ process_post_flex (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ check_ਚੌੜਾਈ (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {}

  ਸਥਿਰ ਖਾਲੀ process_post_move (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    ਜੇ (!env.ਵੇਖਿਆ_moveto)
    {
      env.determine_hintmask_ਮਾਪ ();
      env.ਵੇਖਿਆ_moveto = ਸੱਚ;
    }
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ process_post_ਰਸਤਾ (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ_and_op (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_ਦਲੀਲਾਂ (env, param);
    OPSET::flush_op (op, env, param);
  }

  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ (ENV &env, PARAM& param)
  {
    env.ਕੱਢੋ_n_ਦਲੀਲਾਂ (env.arg_ਢੇਰ.get_ਗਿਣਤੀ ());
  }

  ਸਥਿਰ ਖਾਲੀ flush_op (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
  }

  ਸਥਿਰ ਖਾਲੀ flush_hintmask (op_ਕੋਡ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
  }

  ਸਥਿਰ ਤਾਰਕਿਕ is_ਸੰਖਿਆ_op (op_ਕੋਡ_t op)
  {
    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_shortint:
      ਹਾਲਤ Op_ਕੋਡ_fixedcs:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int0: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int1:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int2: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Pos_Int3:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int0: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int1:
      ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int2: ਹਾਲਤ Op_ਕੋਡ_Two_Byte_Neg_Int3:
	ਵਾਪਸ ਸੱਚ;

      ਮੂਲ:
	/* 1-byte integer */
	ਵਾਪਸ (Op_ਕੋਡ_One_Byte_Int_ਪਹਿਲਾ <= op) && (op <= Op_ਕੋਡ_One_Byte_Int_ਆਖਰੀ);
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ opset_t<ARG>  SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ਰਸਤਾ_3, ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ PARAM>
ਬਣਤਰ ਰਸਤਾ_procs_t
{
  ਸਥਿਰ ਖਾਲੀ rmoveto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    ਅਚਲ ਸੰਖਿਆ_t &dy = env.ਕੱਢੋ_arg ();
    ਅਚਲ ਸੰਖਿਆ_t &dx = env.ਕੱਢੋ_arg ();
    pt1.move (dx, dy);
    ਰਸਤਾ_3::moveto (env, param, pt1);
  }

  ਸਥਿਰ ਖਾਲੀ hmoveto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    pt1.move_x (env.ਕੱਢੋ_arg ());
    ਰਸਤਾ_3::moveto (env, param, pt1);
  }

  ਸਥਿਰ ਖਾਲੀ vmoveto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    pt1.move_y (env.ਕੱਢੋ_arg ());
    ਰਸਤਾ_3::moveto (env, param, pt1);
  }

  ਸਥਿਰ ਖਾਲੀ rlineto (ENV &env, PARAM& param)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i + 2 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 2)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }
  }

  ਸਥਿਰ ਖਾਲੀ hlineto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਲਈ (; i + 2 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }
    ਜੇ (i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }
  }

  ਸਥਿਰ ਖਾਲੀ vlineto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਲਈ (; i + 2 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }
    ਜੇ (i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }
  }

  ਸਥਿਰ ਖਾਲੀ rrcurveto (ENV &env, PARAM& param)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i + 6 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 6)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
    }
  }

  ਸਥਿਰ ਖਾਲੀ rcurveline (ENV &env, PARAM& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ arg_ਗਿਣਤੀ = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਜੇ (unlikely (arg_ਗਿਣਤੀ < 8))
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਕਰ_ਹੱਦਬੰਦੀ = arg_ਗਿਣਤੀ - 2;
    ਲਈ (; i + 6 <= ਵਕਰ_ਹੱਦਬੰਦੀ; i += 6)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
    }

    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
  }

  ਸਥਿਰ ਖਾਲੀ rlinecurve (ENV &env, PARAM& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ arg_ਗਿਣਤੀ = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਜੇ (unlikely (arg_ਗਿਣਤੀ < 8))
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰ_ਹੱਦਬੰਦੀ = arg_ਗਿਣਤੀ - 6;
    ਲਈ (; i + 2 <= ਸਤਰ_ਹੱਦਬੰਦੀ; i += 2)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ਰਸਤਾ_3::ਸਤਰ (env, param, pt1);
    }

    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    ਬਿੰਦੂ_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    ਬਿੰਦੂ_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
  }

  ਸਥਿਰ ਖਾਲੀ vvcurveto (ENV &env, PARAM& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    ਲਈ (; i + 4 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ਸਥਿਰ ਖਾਲੀ hhcurveto (ENV &env, PARAM& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਬਿੰਦੂ_t pt1 = env.get_pt ();
    ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    ਲਈ (; i + 4 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ਸਥਿਰ ਖਾਲੀ vhcurveto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1, pt2, pt3;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () % 8) >= 4)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      ਲਈ (; i + 8 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 8)
      {
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      ਜੇ (i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ())
	pt3.move_y (env.eval_arg (i));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (; i + 8 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () - i < 16) && ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
      }
    }
  }

  ਸਥਿਰ ਖਾਲੀ hvcurveto (ENV &env, PARAM& param)
  {
    ਬਿੰਦੂ_t pt1, pt2, pt3;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () % 8) >= 4)
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      ਲਈ (; i + 8 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 8)
      {
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      ਜੇ (i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ ())
	pt3.move_x (env.eval_arg (i));
      ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
    }
    ਨਹੀਂਤਾਂ
    {
      ਲਈ (; i + 8 <= env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	ਜੇ ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () - i < 16) && ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ਸਥਿਰ ਖਾਲੀ moveto (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt)
  { env.moveto (pt); }

  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt1)
  { ਰਸਤਾ_3::moveto (env, param, pt1); }

  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (ENV &env, PARAM& param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3)
  { ਰਸਤਾ_3::moveto (env, param, pt3); }

  ਸਥਿਰ ਖਾਲੀ hflex (ENV &env, PARAM& param)
  {
    ਜੇ (likely (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () == 7))
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      ਬਿੰਦੂ_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      ਬਿੰਦੂ_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      ਬਿੰਦੂ_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ਨਹੀਂਤਾਂ
      env.ਸੈੱਟ_error ();
  }

  ਸਥਿਰ ਖਾਲੀ flex (ENV &env, PARAM& param)
  {
    ਜੇ (likely (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () == 13))
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      ਬਿੰਦੂ_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      ਬਿੰਦੂ_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      ਬਿੰਦੂ_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ਨਹੀਂਤਾਂ
      env.ਸੈੱਟ_error ();
  }

  ਸਥਿਰ ਖਾਲੀ hflex1 (ENV &env, PARAM& param)
  {
    ਜੇ (likely (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () == 9))
    {
      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      ਬਿੰਦੂ_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      ਬਿੰਦੂ_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      ਬਿੰਦੂ_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ਨਹੀਂਤਾਂ
      env.ਸੈੱਟ_error ();
  }

  ਸਥਿਰ ਖਾਲੀ flex1 (ENV &env, PARAM& param)
  {
    ਜੇ (likely (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () == 11))
    {
      ਬਿੰਦੂ_t d;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      ਬਿੰਦੂ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ਬਿੰਦੂ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ਬਿੰਦੂ_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      ਬਿੰਦੂ_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      ਬਿੰਦੂ_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      ਬਿੰਦੂ_t pt6 = pt5;

      ਜੇ (fabs (d.x.ਮੰਜ਼ਿਲ_real ()) > fabs (d.y.ਮੰਜ਼ਿਲ_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      ਨਹੀਂਤਾਂ
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ਨਹੀਂਤਾਂ
      env.ਸੈੱਟ_error ();
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਥਿਰ ਖਾਲੀ curve2 (ENV &env, PARAM& param,
		      ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3,
		      ਅਚਲ ਬਿੰਦੂ_t &pt4, ਅਚਲ ਬਿੰਦੂ_t &pt5, ਅਚਲ ਬਿੰਦੂ_t &pt6)
  {
    ਰਸਤਾ_3::ਵਕਰ (env, param, pt1, pt2, pt3);
    ਰਸਤਾ_3::ਵਕਰ (env, param, pt4, pt5, pt6);
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM>
ਬਣਤਰ cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ਤਾਰਕਿਕ interpret (PARAM& param, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ)
  {
    SUPER::env.ਸੈੱਟ_endchar (ਝੂਠ);
    SUPER::env.ਸੈੱਟ_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t ਬਚਿਆ = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    ਬਿਨਾਨਿਸ਼ਾਨੀ max_ops = (ਬਿਨਾਨਿਸ਼ਾਨੀ) hb_clamp (ਬਚਿਆ, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ_ops = max_ops;

    ਤਾਰਕਿਕ ret = ਸੱਚ;
    ਜੇ (likely (max_ops))
      ਲਈ (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	ਜੇ (unlikely (SUPER::env.ਵਿੱਚ_error () || !--max_ops))
	{
	  SUPER::env.ਸੈੱਟ_error ();
	  ret = ਝੂਠ;
	  ਰੋਕੋ;
	}
	ਜੇ (SUPER::env.is_endchar ())
	  ਰੋਕੋ;
      }
    ਨਹੀਂਤਾਂ
    {
      SUPER::env.ਸੈੱਟ_error ();
      ret = ਝੂਠ;
    }

    ਜੇ (budget) *budget -= (int64_t) (ਸ਼ੁਰੂ_ops - max_ops);

    ਵਾਪਸ ret;
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF_INTERP_CS_COMMON_HH */
