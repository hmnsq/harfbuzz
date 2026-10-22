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
#ifndef HB_CFF_INTERP_CS_COMMON_HH
#define HB_CFF_INTERP_CS_COMMON_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-cff-interp-common.hh"

നാമാകാശ CFF {

using നാമാകാശ OT;

എണ്ണൽ cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ഘടന call_context_t
{
  ശൂന്യം init (സ്ഥിരം byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned പൂർണ്ണസംഖ്യ subr_അക്കം=0)
  {
    str_ref = substr_;
    type = type_;
    subr_അക്കം_2 = subr_അക്കം;
  }

  ശൂന്യം fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned പൂർണ്ണസംഖ്യ    subr_അക്കം_2;
};

/* call stack */
സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ kMaxCallLimit = 10;
ഘടന call_അട്ടി_t : cff_അട്ടി_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
ഘടന biased_subrs_t
{
  ശൂന്യം init (സ്ഥിരം SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned പൂർണ്ണസംഖ്യ  nSubrs = get_എണ്ണം ();
    എങ്കിൽ (nSubrs < 1240)
      bias = 107;
    അല്ലെങ്കിൽ എങ്കിൽ (nSubrs < 33900)
      bias = 1131;
    അല്ലെങ്കിൽ
      bias = 32768;
  }

  ശൂന്യം fini () {}

  unsigned പൂർണ്ണസംഖ്യ get_എണ്ണം () സ്ഥിരം { തിരികെ subrs ? subrs->എണ്ണം : 0; }
  unsigned പൂർണ്ണസംഖ്യ get_bias () സ്ഥിരം  { തിരികെ bias; }

  hb_ubytes_t operator [] (unsigned പൂർണ്ണസംഖ്യ index) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!subrs || index >= subrs->എണ്ണം))
      തിരികെ hb_ubytes_t ();
    അല്ലെങ്കിൽ
      തിരികെ (*subrs)[index];
  }

  സംരക്ഷിത:
  unsigned പൂർണ്ണസംഖ്യ  bias;
  സ്ഥിരം SUBRS   *subrs;
};

ഘടന point_t
{
  ശൂന്യം set_int (പൂർണ്ണസംഖ്യ _x, പൂർണ്ണസംഖ്യ _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  ശൂന്യം move_x (സ്ഥിരം സംഖ്യ_t &dx) { x += dx; }
  ശൂന്യം move_y (സ്ഥിരം സംഖ്യ_t &dy) { y += dy; }
  ശൂന്യം move (സ്ഥിരം സംഖ്യ_t &dx, സ്ഥിരം സംഖ്യ_t &dy) { move_x (dx); move_y (dy); }
  ശൂന്യം move (സ്ഥിരം point_t &d) { move_x (d.x); move_y (d.y); }

  സംഖ്യ_t  x;
  സംഖ്യ_t  y;
};

template <typename ARG, typename SUBRS>
ഘടന cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (സ്ഥിരം hb_ubytes_t &str, സ്ഥിരം SUBRS *globalSubrs_, സ്ഥിരം SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = സത്യം;
    seen_hintmask = അസത്യം;
    hstem_എണ്ണം = 0;
    vstem_എണ്ണം = 0;
    hintmask_size = 0;
    budget = nullptr;
    pt.set_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  തർക്കം in_error () സ്ഥിരം
  {
    തിരികെ call_അട്ടി.in_error () || SUPER::in_error ();
  }

  തർക്കം എടുക്കുക_subr_അക്കം (സ്ഥിരം biased_subrs_t<SUBRS>& biasedSubrs, unsigned പൂർണ്ണസംഖ്യ &subr_അക്കം_2)
  {
    subr_അക്കം_2 = 0;
    പൂർണ്ണസംഖ്യ n = SUPER::arg_അട്ടി.എടുക്കുക_int ();
    n += biasedSubrs.get_bias ();
    എങ്കിൽ (unlikely ((n < 0) || ((unsigned പൂർണ്ണസംഖ്യ)n >= biasedSubrs.get_എണ്ണം ())))
      തിരികെ അസത്യം;

    subr_അക്കം_2 = (unsigned പൂർണ്ണസംഖ്യ)n;
    തിരികെ സത്യം;
  }

  ശൂന്യം call_subr (സ്ഥിരം biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned പൂർണ്ണസംഖ്യ subr_അക്കം_2 = 0;

    എങ്കിൽ (unlikely (!എടുക്കുക_subr_അക്കം (biasedSubrs, subr_അക്കം_2)
		 || call_അട്ടി.get_എണ്ണം () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      തിരികെ;
    }
    context.str_ref = SUPER::str_ref;
    call_അട്ടി.push (context);

    context.init ( biasedSubrs[subr_അക്കം_2], type, subr_അക്കം_2);
    SUPER::str_ref = context.str_ref;
  }

  ശൂന്യം return_from_subr ()
  {
    എങ്കിൽ (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_അട്ടി.എടുക്കുക ();
    SUPER::str_ref = context.str_ref;
  }

  ശൂന്യം determine_hintmask_size ()
  {
    എങ്കിൽ (!seen_hintmask)
    {
      vstem_എണ്ണം += SUPER::arg_അട്ടി.get_എണ്ണം() / 2;
      hintmask_size = (hstem_എണ്ണം + vstem_എണ്ണം + 7) >> 3;
      seen_hintmask = സത്യം;
    }
  }

  ശൂന്യം set_endchar (തർക്കം endchar_flag_) { endchar_flag = endchar_flag_; }
  തർക്കം is_endchar () സ്ഥിരം { തിരികെ endchar_flag; }

  ശൂന്യം set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () സ്ഥിരം { തിരികെ budget; }
  തർക്കം spend_budget (unsigned പൂർണ്ണസംഖ്യ cost, unsigned പൂർണ്ണസംഖ്യ mult = 1)
  { തിരികെ !budget || hb_budget_spend (*budget, cost, mult); }

  സ്ഥിരം സംഖ്യ_t &get_x () സ്ഥിരം { തിരികെ pt.x; }
  സ്ഥിരം സംഖ്യ_t &get_y () സ്ഥിരം { തിരികെ pt.y; }
  സ്ഥിരം point_t &get_pt () സ്ഥിരം { തിരികെ pt; }

  ശൂന്യം moveto (സ്ഥിരം point_t &pt_ ) { pt = pt_; }

  പൊതു:
  call_context_t   context;
  തർക്കം	  endchar_flag;
  തർക്കം	  seen_moveto;
  തർക്കം	  seen_hintmask;

  unsigned പൂർണ്ണസംഖ്യ  hstem_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ  vstem_എണ്ണം;
  unsigned പൂർണ്ണസംഖ്യ  hintmask_size;
  int64_t	*budget;
  call_അട്ടി_t	call_അട്ടി;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  സ്വകാര്യ:
  point_t	 pt;

  തരനിർവചനം interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
ഘടന path_procs_null_t
{
  സ്ഥിതം ശൂന്യം rmoveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം hmoveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം vmoveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം rlineto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം hlineto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം vlineto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം rrcurveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം rcurveline (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം rlinecurve (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം vvcurveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം hhcurveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം vhcurveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം hvcurveto (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം moveto (ENV &env, PARAM& param, സ്ഥിരം point_t &pt) {}
  സ്ഥിതം ശൂന്യം വരി (ENV &env, PARAM& param, സ്ഥിരം point_t &pt1) {}
  സ്ഥിതം ശൂന്യം curve (ENV &env, PARAM& param, സ്ഥിരം point_t &pt1, സ്ഥിരം point_t &pt2, സ്ഥിരം point_t &pt3) {}
  സ്ഥിതം ശൂന്യം hflex (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം flex (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം hflex1 (ENV &env, PARAM& param) {}
  സ്ഥിതം ശൂന്യം flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
ഘടന cs_opset_t : opset_t<ARG>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, ENV &env, PARAM& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {

      അവസ്ഥ OpCode_return:
	env.return_from_subr ();
	നിർത്തുക;
      അവസ്ഥ OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (സത്യം);
	OPSET::flush_args_and_op (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_fixedcs:
	env.arg_അട്ടി.തള്ളുക_fixed_from_substr (env.str_ref);
	നിർത്തുക;

      അവസ്ഥ OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	നിർത്തുക;

      അവസ്ഥ OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	നിർത്തുക;

      അവസ്ഥ OpCode_hstem:
      അവസ്ഥ OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_vstem:
      അവസ്ഥ OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_hintmask:
      അവസ്ഥ OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;
      അവസ്ഥ OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env);
	നിർത്തുക;
    }
  }

  സ്ഥിതം ശൂന്യം process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_എണ്ണം += env.arg_അട്ടി.get_എണ്ണം () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_എണ്ണം += env.arg_അട്ടി.get_എണ്ണം () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    എങ്കിൽ (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  സ്ഥിതം ശൂന്യം process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  സ്ഥിതം ശൂന്യം process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    എങ്കിൽ (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = സത്യം;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  സ്ഥിതം ശൂന്യം flush_args (ENV &env, PARAM& param)
  {
    env.എടുക്കുക_n_args (env.arg_അട്ടി.get_എണ്ണം ());
  }

  സ്ഥിതം ശൂന്യം flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  സ്ഥിതം ശൂന്യം flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  സ്ഥിതം തർക്കം is_സംഖ്യ_op (op_code_t op)
  {
    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_shortint:
      അവസ്ഥ OpCode_fixedcs:
      അവസ്ഥ OpCode_TwoBytePosInt0: അവസ്ഥ OpCode_TwoBytePosInt1:
      അവസ്ഥ OpCode_TwoBytePosInt2: അവസ്ഥ OpCode_TwoBytePosInt3:
      അവസ്ഥ OpCode_TwoByteNegInt0: അവസ്ഥ OpCode_TwoByteNegInt1:
      അവസ്ഥ OpCode_TwoByteNegInt2: അവസ്ഥ OpCode_TwoByteNegInt3:
	തിരികെ സത്യം;

      സ്ഥിരസ്ഥിതി:
	/* 1-byte integer */
	തിരികെ (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  സംരക്ഷിത:
  തരനിർവചനം opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
ഘടന path_procs_t
{
  സ്ഥിതം ശൂന്യം rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    സ്ഥിരം സംഖ്യ_t &dy = env.എടുക്കുക_arg ();
    സ്ഥിരം സംഖ്യ_t &dx = env.എടുക്കുക_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  സ്ഥിതം ശൂന്യം hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.എടുക്കുക_arg ());
    PATH::moveto (env, param, pt1);
  }

  സ്ഥിതം ശൂന്യം vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.എടുക്കുക_arg ());
    PATH::moveto (env, param, pt1);
  }

  സ്ഥിതം ശൂന്യം rlineto (ENV &env, PARAM& param)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i + 2 <= env.arg_അട്ടി.get_എണ്ണം (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::വരി (env, param, pt1);
    }
  }

  സ്ഥിതം ശൂന്യം hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    വേണ്ടി (; i + 2 <= env.arg_അട്ടി.get_എണ്ണം (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::വരി (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::വരി (env, param, pt1);
    }
    എങ്കിൽ (i < env.arg_അട്ടി.get_എണ്ണം ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::വരി (env, param, pt1);
    }
  }

  സ്ഥിതം ശൂന്യം vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    വേണ്ടി (; i + 2 <= env.arg_അട്ടി.get_എണ്ണം (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::വരി (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::വരി (env, param, pt1);
    }
    എങ്കിൽ (i < env.arg_അട്ടി.get_എണ്ണം ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::വരി (env, param, pt1);
    }
  }

  സ്ഥിതം ശൂന്യം rrcurveto (ENV &env, PARAM& param)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i + 6 <= env.arg_അട്ടി.get_എണ്ണം (); i += 6)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      point_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
  }

  സ്ഥിതം ശൂന്യം rcurveline (ENV &env, PARAM& param)
  {
    unsigned പൂർണ്ണസംഖ്യ arg_എണ്ണം = env.arg_അട്ടി.get_എണ്ണം ();
    എങ്കിൽ (unlikely (arg_എണ്ണം < 8))
      തിരികെ;

    unsigned പൂർണ്ണസംഖ്യ i = 0;
    unsigned പൂർണ്ണസംഖ്യ curve_limit = arg_എണ്ണം - 2;
    വേണ്ടി (; i + 6 <= curve_limit; i += 6)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      point_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      PATH::curve (env, param, pt1, pt2, pt3);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    PATH::വരി (env, param, pt1);
  }

  സ്ഥിതം ശൂന്യം rlinecurve (ENV &env, PARAM& param)
  {
    unsigned പൂർണ്ണസംഖ്യ arg_എണ്ണം = env.arg_അട്ടി.get_എണ്ണം ();
    എങ്കിൽ (unlikely (arg_എണ്ണം < 8))
      തിരികെ;

    unsigned പൂർണ്ണസംഖ്യ i = 0;
    unsigned പൂർണ്ണസംഖ്യ വരി_limit = arg_എണ്ണം - 6;
    വേണ്ടി (; i + 2 <= വരി_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::വരി (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  സ്ഥിതം ശൂന്യം vvcurveto (ENV &env, PARAM& param)
  {
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    point_t pt1 = env.get_pt ();
    എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    വേണ്ടി (; i + 4 <= env.arg_അട്ടി.get_എണ്ണം (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      PATH::curve (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  സ്ഥിതം ശൂന്യം hhcurveto (ENV &env, PARAM& param)
  {
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    point_t pt1 = env.get_pt ();
    എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    വേണ്ടി (; i + 4 <= env.arg_അട്ടി.get_എണ്ണം (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      PATH::curve (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  സ്ഥിതം ശൂന്യം vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      വേണ്ടി (; i + 8 <= env.arg_അട്ടി.get_എണ്ണം (); i += 8)
      {
	PATH::curve (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	PATH::curve (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      എങ്കിൽ (i < env.arg_അട്ടി.get_എണ്ണം ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    അല്ലെങ്കിൽ
    {
      വേണ്ടി (; i + 8 <= env.arg_അട്ടി.get_എണ്ണം (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	PATH::curve (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () - i < 16) && ((env.arg_അട്ടി.get_എണ്ണം () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  സ്ഥിതം ശൂന്യം hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      വേണ്ടി (; i + 8 <= env.arg_അട്ടി.get_എണ്ണം (); i += 8)
      {
	PATH::curve (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	PATH::curve (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      എങ്കിൽ (i < env.arg_അട്ടി.get_എണ്ണം ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    അല്ലെങ്കിൽ
    {
      വേണ്ടി (; i + 8 <= env.arg_അട്ടി.get_എണ്ണം (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	PATH::curve (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	എങ്കിൽ ((env.arg_അട്ടി.get_എണ്ണം () - i < 16) && ((env.arg_അട്ടി.get_എണ്ണം () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  സ്ഥിതം ശൂന്യം moveto (ENV &env, PARAM& param, സ്ഥിരം point_t &pt)
  { env.moveto (pt); }

  സ്ഥിതം ശൂന്യം വരി (ENV &env, PARAM& param, സ്ഥിരം point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  സ്ഥിതം ശൂന്യം curve (ENV &env, PARAM& param, സ്ഥിരം point_t &pt1, സ്ഥിരം point_t &pt2, സ്ഥിരം point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  സ്ഥിതം ശൂന്യം hflex (ENV &env, PARAM& param)
  {
    എങ്കിൽ (likely (env.arg_അട്ടി.get_എണ്ണം () == 7))
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      point_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      point_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      point_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    അല്ലെങ്കിൽ
      env.set_error ();
  }

  സ്ഥിതം ശൂന്യം flex (ENV &env, PARAM& param)
  {
    എങ്കിൽ (likely (env.arg_അട്ടി.get_എണ്ണം () == 13))
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      point_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      point_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      point_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      point_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    അല്ലെങ്കിൽ
      env.set_error ();
  }

  സ്ഥിതം ശൂന്യം hflex1 (ENV &env, PARAM& param)
  {
    എങ്കിൽ (likely (env.arg_അട്ടി.get_എണ്ണം () == 9))
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      point_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      point_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      point_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    അല്ലെങ്കിൽ
      env.set_error ();
  }

  സ്ഥിതം ശൂന്യം flex1 (ENV &env, PARAM& param)
  {
    എങ്കിൽ (likely (env.arg_അട്ടി.get_എണ്ണം () == 11))
    {
      point_t d;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      point_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      point_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      point_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      point_t pt6 = pt5;

      എങ്കിൽ (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      അല്ലെങ്കിൽ
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    അല്ലെങ്കിൽ
      env.set_error ();
  }

  സംരക്ഷിത:
  സ്ഥിതം ശൂന്യം curve2 (ENV &env, PARAM& param,
		      സ്ഥിരം point_t &pt1, സ്ഥിരം point_t &pt2, സ്ഥിരം point_t &pt3,
		      സ്ഥിരം point_t &pt4, സ്ഥിരം point_t &pt5, സ്ഥിരം point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
ഘടന cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  തർക്കം interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (അസത്യം);
    SUPER::env.set_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t remaining = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    unsigned max_ops = (unsigned) hb_clamp (remaining, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    unsigned start_ops = max_ops;

    തർക്കം ret = സത്യം;
    എങ്കിൽ (likely (max_ops))
      വേണ്ടി (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	എങ്കിൽ (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = അസത്യം;
	  നിർത്തുക;
	}
	എങ്കിൽ (SUPER::env.is_endchar ())
	  നിർത്തുക;
      }
    അല്ലെങ്കിൽ
    {
      SUPER::env.set_error ();
      ret = അസത്യം;
    }

    എങ്കിൽ (budget) *budget -= (int64_t) (start_ops - max_ops);

    തിരികെ ret;
  }

  സ്വകാര്യ:
  തരനിർവചനം interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
