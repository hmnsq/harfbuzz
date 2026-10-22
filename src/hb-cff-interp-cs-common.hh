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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-common.hh"

ನಾಮಾಕಾಶ CFF {

using ನಾಮಾಕಾಶ OT;

ಗಣನಾ cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ಸಂರಚನಾ call_context_t
{
  ಶೂನ್ಯ init (ಧ್ರುವ byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ=0)
  {
    str_ref = substr_;
    type = type_;
    subr_ಅಂಕೆ_2 = subr_ಅಂಕೆ;
  }

  ಶೂನ್ಯ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subr_ಅಂಕೆ_2;
};

/* call stack */
ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ kMaxCallLimit = 10;
ಸಂರಚನಾ call_ರಾಶಿ_t : cff_ರಾಶಿ_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
ಸಂರಚನಾ biased_subrs_t
{
  ಶೂನ್ಯ init (ಧ್ರುವ SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned ಪೂರ್ಣಾಙ್ಕ  nSubrs = get_ಎಣಿಕೆ ();
    ಯದಿ (nSubrs < 1240)
      bias = 107;
    ಅನ್ಯಥಾ ಯದಿ (nSubrs < 33900)
      bias = 1131;
    ಅನ್ಯಥಾ
      bias = 32768;
  }

  ಶೂನ್ಯ fini () {}

  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ subrs ? subrs->ಎಣಿಕೆ : 0; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_bias () ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ bias; }

  hb_ubytes_t operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!subrs || index >= subrs->ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ hb_ubytes_t ();
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ (*subrs)[index];
  }

  ರಕ್ಷಿತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ  bias;
  ಧ್ರುವ SUBRS   *subrs;
};

ಸಂರಚನಾ point_t
{
  ಶೂನ್ಯ set_int (ಪೂರ್ಣಾಙ್ಕ _x, ಪೂರ್ಣಾಙ್ಕ _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  ಶೂನ್ಯ move_x (ಧ್ರುವ ಸಂಖ್ಯೆ_t &dx) { x += dx; }
  ಶೂನ್ಯ move_y (ಧ್ರುವ ಸಂಖ್ಯೆ_t &dy) { y += dy; }
  ಶೂನ್ಯ move (ಧ್ರುವ ಸಂಖ್ಯೆ_t &dx, ಧ್ರುವ ಸಂಖ್ಯೆ_t &dy) { move_x (dx); move_y (dy); }
  ಶೂನ್ಯ move (ಧ್ರುವ point_t &d) { move_x (d.x); move_y (d.y); }

  ಸಂಖ್ಯೆ_t  x;
  ಸಂಖ್ಯೆ_t  y;
};

template <typename ARG, typename SUBRS>
ಸಂರಚನಾ cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ಧ್ರುವ hb_ubytes_t &str, ಧ್ರುವ SUBRS *globalSubrs_, ಧ್ರುವ SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = ಸತ್ಯ;
    seen_hintmask = ಅಸತ್ಯ;
    hstem_ಎಣಿಕೆ = 0;
    vstem_ಎಣಿಕೆ = 0;
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

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ call_ರಾಶಿ.in_error () || SUPER::in_error ();
  }

  ತಾರ್ಕಿಕ ತೆಗೆ_subr_ಅಂಕೆ (ಧ್ರುವ biased_subrs_t<SUBRS>& biasedSubrs, unsigned ಪೂರ್ಣಾಙ್ಕ &subr_ಅಂಕೆ_2)
  {
    subr_ಅಂಕೆ_2 = 0;
    ಪೂರ್ಣಾಙ್ಕ n = SUPER::arg_ರಾಶಿ.ತೆಗೆ_int ();
    n += biasedSubrs.get_bias ();
    ಯದಿ (unlikely ((n < 0) || ((unsigned ಪೂರ್ಣಾಙ್ಕ)n >= biasedSubrs.get_ಎಣಿಕೆ ())))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    subr_ಅಂಕೆ_2 = (unsigned ಪೂರ್ಣಾಙ್ಕ)n;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ call_subr (ಧ್ರುವ biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ_2 = 0;

    ಯದಿ (unlikely (!ತೆಗೆ_subr_ಅಂಕೆ (biasedSubrs, subr_ಅಂಕೆ_2)
		 || call_ರಾಶಿ.get_ಎಣಿಕೆ () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    context.str_ref = SUPER::str_ref;
    call_ರಾಶಿ.push (context);

    context.init ( biasedSubrs[subr_ಅಂಕೆ_2], type, subr_ಅಂಕೆ_2);
    SUPER::str_ref = context.str_ref;
  }

  ಶೂನ್ಯ return_from_subr ()
  {
    ಯದಿ (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_ರಾಶಿ.ತೆಗೆ ();
    SUPER::str_ref = context.str_ref;
  }

  ಶೂನ್ಯ determine_hintmask_size ()
  {
    ಯದಿ (!seen_hintmask)
    {
      vstem_ಎಣಿಕೆ += SUPER::arg_ರಾಶಿ.get_ಎಣಿಕೆ() / 2;
      hintmask_size = (hstem_ಎಣಿಕೆ + vstem_ಎಣಿಕೆ + 7) >> 3;
      seen_hintmask = ಸತ್ಯ;
    }
  }

  ಶೂನ್ಯ set_endchar (ತಾರ್ಕಿಕ endchar_flag_) { endchar_flag = endchar_flag_; }
  ತಾರ್ಕಿಕ is_endchar () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ endchar_flag; }

  ಶೂನ್ಯ set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ budget; }
  ತಾರ್ಕಿಕ spend_budget (unsigned ಪೂರ್ಣಾಙ್ಕ cost, unsigned ಪೂರ್ಣಾಙ್ಕ mult = 1)
  { ಪ್ರತ್ಯಾಗಮ !budget || hb_budget_spend (*budget, cost, mult); }

  ಧ್ರುವ ಸಂಖ್ಯೆ_t &get_x () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pt.x; }
  ಧ್ರುವ ಸಂಖ್ಯೆ_t &get_y () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pt.y; }
  ಧ್ರುವ point_t &get_pt () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ pt; }

  ಶೂನ್ಯ moveto (ಧ್ರುವ point_t &pt_ ) { pt = pt_; }

  ಸಾರ್ವಜನಿಕ:
  call_context_t   context;
  ತಾರ್ಕಿಕ	  endchar_flag;
  ತಾರ್ಕಿಕ	  seen_moveto;
  ತಾರ್ಕಿಕ	  seen_hintmask;

  unsigned ಪೂರ್ಣಾಙ್ಕ  hstem_ಎಣಿಕೆ;
  unsigned ಪೂರ್ಣಾಙ್ಕ  vstem_ಎಣಿಕೆ;
  unsigned ಪೂರ್ಣಾಙ್ಕ  hintmask_size;
  int64_t	*budget;
  call_ರಾಶಿ_t	call_ರಾಶಿ;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  ಗುಪ್ತ:
  point_t	 pt;

  ಪ್ರಕಾರನಿರ್ದೇಶ interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
ಸಂರಚನಾ path_procs_null_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ rmoveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hmoveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ vmoveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ rlineto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hlineto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ vlineto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ rrcurveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ rcurveline (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ rlinecurve (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ vvcurveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hhcurveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ vhcurveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hvcurveto (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ moveto (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt) {}
  ಸ್ಥಿರ ಶೂನ್ಯ ಸಾಲು (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt1) {}
  ಸ್ಥಿರ ಶೂನ್ಯ curve (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt1, ಧ್ರುವ point_t &pt2, ಧ್ರುವ point_t &pt3) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hflex (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ flex (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ hflex1 (ENV &env, PARAM& param) {}
  ಸ್ಥಿರ ಶೂನ್ಯ flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
ಸಂರಚನಾ cs_opset_t : opset_t<ARG>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ENV &env, PARAM& param)
  {
    ವಿಕಲ್ಪ (op) {

      ಪಕ್ಷ OpCode_return:
	env.return_from_subr ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (ಸತ್ಯ);
	OPSET::flush_args_and_op (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_fixedcs:
	env.arg_ರಾಶಿ.ತಳ್ಳು_fixed_from_substr (env.str_ref);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_hstem:
      ಪಕ್ಷ OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vstem:
      ಪಕ್ಷ OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_hintmask:
      ಪಕ್ಷ OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env);
	ಭಙ್ಗ;
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_ಎಣಿಕೆ += env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_ಎಣಿಕೆ += env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    ಯದಿ (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  ಸ್ಥಿರ ಶೂನ್ಯ process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    ಯದಿ (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = ಸತ್ಯ;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_args (ENV &env, PARAM& param)
  {
    env.ತೆಗೆ_n_args (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ());
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ is_ಸಂಖ್ಯೆ_op (op_code_t op)
  {
    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_shortint:
      ಪಕ್ಷ OpCode_fixedcs:
      ಪಕ್ಷ OpCode_TwoBytePosInt0: ಪಕ್ಷ OpCode_TwoBytePosInt1:
      ಪಕ್ಷ OpCode_TwoBytePosInt2: ಪಕ್ಷ OpCode_TwoBytePosInt3:
      ಪಕ್ಷ OpCode_TwoByteNegInt0: ಪಕ್ಷ OpCode_TwoByteNegInt1:
      ಪಕ್ಷ OpCode_TwoByteNegInt2: ಪಕ್ಷ OpCode_TwoByteNegInt3:
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      ಸ್ವಭಾವ:
	/* 1-byte integer */
	ಪ್ರತ್ಯಾಗಮ (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  ರಕ್ಷಿತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
ಸಂರಚನಾ path_procs_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ಧ್ರುವ ಸಂಖ್ಯೆ_t &dy = env.ತೆಗೆ_arg ();
    ಧ್ರುವ ಸಂಖ್ಯೆ_t &dx = env.ತೆಗೆ_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.ತೆಗೆ_arg ());
    PATH::moveto (env, param, pt1);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.ತೆಗೆ_arg ());
    PATH::moveto (env, param, pt1);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ rlineto (ENV &env, PARAM& param)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i + 2 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::ಸಾಲು (env, param, pt1);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪ್ರತಿ (; i + 2 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::ಸಾಲು (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::ಸಾಲು (env, param, pt1);
    }
    ಯದಿ (i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::ಸಾಲು (env, param, pt1);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪ್ರತಿ (; i + 2 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::ಸಾಲು (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::ಸಾಲು (env, param, pt1);
    }
    ಯದಿ (i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::ಸಾಲು (env, param, pt1);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ rrcurveto (ENV &env, PARAM& param)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i + 6 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 6)
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

  ಸ್ಥಿರ ಶೂನ್ಯ rcurveline (ENV &env, PARAM& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ arg_ಎಣಿಕೆ = env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();
    ಯದಿ (unlikely (arg_ಎಣಿಕೆ < 8))
      ಪ್ರತ್ಯಾಗಮ;

    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ curve_limit = arg_ಎಣಿಕೆ - 2;
    ಪ್ರತಿ (; i + 6 <= curve_limit; i += 6)
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
    PATH::ಸಾಲು (env, param, pt1);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ rlinecurve (ENV &env, PARAM& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ arg_ಎಣಿಕೆ = env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ();
    ಯದಿ (unlikely (arg_ಎಣಿಕೆ < 8))
      ಪ್ರತ್ಯಾಗಮ;

    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_limit = arg_ಎಣಿಕೆ - 6;
    ಪ್ರತಿ (; i + 2 <= ಸಾಲು_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::ಸಾಲು (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ vvcurveto (ENV &env, PARAM& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    point_t pt1 = env.get_pt ();
    ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    ಪ್ರತಿ (; i + 4 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 4)
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

  ಸ್ಥಿರ ಶೂನ್ಯ hhcurveto (ENV &env, PARAM& param)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    point_t pt1 = env.get_pt ();
    ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    ಪ್ರತಿ (; i + 4 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 4)
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

  ಸ್ಥಿರ ಶೂನ್ಯ vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      ಪ್ರತಿ (; i + 8 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 8)
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
      ಯದಿ (i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (; i + 8 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 8)
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
	ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () - i < 16) && ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      ಪ್ರತಿ (; i + 8 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 8)
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
      ಯದಿ (i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (; i + 8 <= env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i += 8)
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
	ಯದಿ ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () - i < 16) && ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ಸ್ಥಿರ ಶೂನ್ಯ moveto (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt)
  { env.moveto (pt); }

  ಸ್ಥಿರ ಶೂನ್ಯ ಸಾಲು (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  ಸ್ಥಿರ ಶೂನ್ಯ curve (ENV &env, PARAM& param, ಧ್ರುವ point_t &pt1, ಧ್ರುವ point_t &pt2, ಧ್ರುವ point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  ಸ್ಥಿರ ಶೂನ್ಯ hflex (ENV &env, PARAM& param)
  {
    ಯದಿ (likely (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () == 7))
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
    ಅನ್ಯಥಾ
      env.set_error ();
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flex (ENV &env, PARAM& param)
  {
    ಯದಿ (likely (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () == 13))
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
    ಅನ್ಯಥಾ
      env.set_error ();
  }

  ಸ್ಥಿರ ಶೂನ್ಯ hflex1 (ENV &env, PARAM& param)
  {
    ಯದಿ (likely (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () == 9))
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
    ಅನ್ಯಥಾ
      env.set_error ();
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flex1 (ENV &env, PARAM& param)
  {
    ಯದಿ (likely (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () == 11))
    {
      point_t d;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < 10; i += 2)
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

      ಯದಿ (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      ಅನ್ಯಥಾ
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ಅನ್ಯಥಾ
      env.set_error ();
  }

  ರಕ್ಷಿತ:
  ಸ್ಥಿರ ಶೂನ್ಯ curve2 (ENV &env, PARAM& param,
		      ಧ್ರುವ point_t &pt1, ಧ್ರುವ point_t &pt2, ಧ್ರುವ point_t &pt3,
		      ಧ್ರುವ point_t &pt4, ಧ್ರುವ point_t &pt5, ಧ್ರುವ point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
ಸಂರಚನಾ cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ತಾರ್ಕಿಕ interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (ಅಸತ್ಯ);
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

    ತಾರ್ಕಿಕ ret = ಸತ್ಯ;
    ಯದಿ (likely (max_ops))
      ಪ್ರತಿ (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	ಯದಿ (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = ಅಸತ್ಯ;
	  ಭಙ್ಗ;
	}
	ಯದಿ (SUPER::env.is_endchar ())
	  ಭಙ್ಗ;
      }
    ಅನ್ಯಥಾ
    {
      SUPER::env.set_error ();
      ret = ಅಸತ್ಯ;
    }

    ಯದಿ (budget) *budget -= (int64_t) (start_ops - max_ops);

    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
