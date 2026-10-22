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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-common.hh"

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

ଗଣନା cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ସଂରଚନା call_context_t
{
  ଶୂନ୍ୟ init (ଅଚଳ byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ=0)
  {
    str_ref = substr_;
    type = type_;
    subr_ଅଙ୍କ_2 = subr_ଅଙ୍କ;
  }

  ଶୂନ୍ୟ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subr_ଅଙ୍କ_2;
};

/* call stack */
ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା kMaxCallLimit = 10;
ସଂରଚନା call_ଗଦା_t : cff_ଗଦା_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
ସଂରଚନା biased_subrs_t
{
  ଶୂନ୍ୟ init (ଅଚଳ SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  nSubrs = get_ଗଣନା ();
    ଯଦି (nSubrs < 1240)
      bias = 107;
    ଅନ୍ୟଥା ଯଦି (nSubrs < 33900)
      bias = 1131;
    ଅନ୍ୟଥା
      bias = 32768;
  }

  ଶୂନ୍ୟ fini () {}

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ subrs ? subrs->ଗଣନା_2 : 0; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_bias () ଅଚଳ  { ଫେରନ୍ତୁ bias; }

  hb_ubytes_t operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index) ଅଚଳ
  {
    ଯଦି (unlikely (!subrs || index >= subrs->ଗଣନା_2))
      ଫେରନ୍ତୁ hb_ubytes_t ();
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ (*subrs)[index];
  }

  ସୁରକ୍ଷିତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  bias;
  ଅଚଳ SUBRS   *subrs;
};

ସଂରଚନା point_t
{
  ଶୂନ୍ୟ set_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା _x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  ଶୂନ୍ୟ move_x (ଅଚଳ ସଂଖ୍ୟା_t &dx) { x += dx; }
  ଶୂନ୍ୟ move_y (ଅଚଳ ସଂଖ୍ୟା_t &dy) { y += dy; }
  ଶୂନ୍ୟ move (ଅଚଳ ସଂଖ୍ୟା_t &dx, ଅଚଳ ସଂଖ୍ୟା_t &dy) { move_x (dx); move_y (dy); }
  ଶୂନ୍ୟ move (ଅଚଳ point_t &d) { move_x (d.x); move_y (d.y); }

  ସଂଖ୍ୟା_t  x;
  ସଂଖ୍ୟା_t  y;
};

template <typename ARG, typename SUBRS>
ସଂରଚନା cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ଅଚଳ hb_ubytes_t &str, ଅଚଳ SUBRS *globalSubrs_, ଅଚଳ SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = ସତ୍ୟ;
    seen_hintmask = ମିଥ୍ୟା;
    hstem_ଗଣନା = 0;
    vstem_ଗଣନା = 0;
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

  ତାର୍କିକ in_error () ଅଚଳ
  {
    ଫେରନ୍ତୁ call_ଗଦା.in_error () || SUPER::in_error ();
  }

  ତାର୍କିକ କାଢ଼_subr_ଅଙ୍କ (ଅଚଳ biased_subrs_t<SUBRS>& biasedSubrs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subr_ଅଙ୍କ_2)
  {
    subr_ଅଙ୍କ_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = SUPER::arg_ଗଦା.କାଢ଼_int ();
    n += biasedSubrs.get_bias ();
    ଯଦି (unlikely ((n < 0) || ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n >= biasedSubrs.get_ଗଣନା ())))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    subr_ଅଙ୍କ_2 = (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ call_subr (ଅଚଳ biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ_2 = 0;

    ଯଦି (unlikely (!କାଢ଼_subr_ଅଙ୍କ (biasedSubrs, subr_ଅଙ୍କ_2)
		 || call_ଗଦା.get_ଗଣନା () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      ଫେରନ୍ତୁ;
    }
    context.str_ref = SUPER::str_ref;
    call_ଗଦା.push (context);

    context.init ( biasedSubrs[subr_ଅଙ୍କ_2], type, subr_ଅଙ୍କ_2);
    SUPER::str_ref = context.str_ref;
  }

  ଶୂନ୍ୟ return_from_subr ()
  {
    ଯଦି (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_ଗଦା.କାଢ଼ ();
    SUPER::str_ref = context.str_ref;
  }

  ଶୂନ୍ୟ determine_hintmask_size ()
  {
    ଯଦି (!seen_hintmask)
    {
      vstem_ଗଣନା += SUPER::arg_ଗଦା.get_ଗଣନା() / 2;
      hintmask_size = (hstem_ଗଣନା + vstem_ଗଣନା + 7) >> 3;
      seen_hintmask = ସତ୍ୟ;
    }
  }

  ଶୂନ୍ୟ set_endchar (ତାର୍କିକ endchar_flag_) { endchar_flag = endchar_flag_; }
  ତାର୍କିକ is_endchar () ଅଚଳ { ଫେରନ୍ତୁ endchar_flag; }

  ଶୂନ୍ୟ set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ଅଚଳ { ଫେରନ୍ତୁ budget; }
  ତାର୍କିକ spend_budget (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cost, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mult = 1)
  { ଫେରନ୍ତୁ !budget || hb_budget_spend (*budget, cost, mult); }

  ଅଚଳ ସଂଖ୍ୟା_t &get_x () ଅଚଳ { ଫେରନ୍ତୁ pt.x; }
  ଅଚଳ ସଂଖ୍ୟା_t &get_y () ଅଚଳ { ଫେରନ୍ତୁ pt.y; }
  ଅଚଳ point_t &get_pt () ଅଚଳ { ଫେରନ୍ତୁ pt; }

  ଶୂନ୍ୟ moveto (ଅଚଳ point_t &pt_ ) { pt = pt_; }

  ସର୍ବସାଧାରଣ:
  call_context_t   context;
  ତାର୍କିକ	  endchar_flag;
  ତାର୍କିକ	  seen_moveto;
  ତାର୍କିକ	  seen_hintmask;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  hstem_ଗଣନା;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  vstem_ଗଣନା;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  hintmask_size;
  int64_t	*budget;
  call_ଗଦା_t	call_ଗଦା;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  ବ୍ୟକ୍ତିଗତ:
  point_t	 pt;

  ପ୍ରକାରବ୍ୟାଖ୍ୟା interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
ସଂରଚନା path_procs_null_t
{
  ସ୍ଥିର ଶୂନ୍ୟ rmoveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ hmoveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ vmoveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ rlineto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ hlineto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ vlineto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ rrcurveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ rcurveline (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ rlinecurve (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ vvcurveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ hhcurveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ vhcurveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ hvcurveto (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ moveto (ENV &env, PARAM& param, ଅଚଳ point_t &pt) {}
  ସ୍ଥିର ଶୂନ୍ୟ ଧାଡ଼ି (ENV &env, PARAM& param, ଅଚଳ point_t &pt1) {}
  ସ୍ଥିର ଶୂନ୍ୟ curve (ENV &env, PARAM& param, ଅଚଳ point_t &pt1, ଅଚଳ point_t &pt2, ଅଚଳ point_t &pt3) {}
  ସ୍ଥିର ଶୂନ୍ୟ hflex (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ flex (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ hflex1 (ENV &env, PARAM& param) {}
  ସ୍ଥିର ଶୂନ୍ୟ flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
ସଂରଚନା cs_opset_t : opset_t<ARG>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ENV &env, PARAM& param)
  {
    ବାଛ (op) {

      ସ୍ଥିତି OpCode_return:
	env.return_from_subr ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (ସତ୍ୟ);
	OPSET::flush_args_and_op (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_fixedcs:
	env.arg_ଗଦା.ଠେଲ_fixed_from_substr (env.str_ref);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_hstem:
      ସ୍ଥିତି OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vstem:
      ସ୍ଥିତି OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_hintmask:
      ସ୍ଥିତି OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env);
	ଭାଙ୍ଗ;
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_ଗଣନା += env.arg_ଗଦା.get_ଗଣନା () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_ଗଣନା += env.arg_ଗଦା.get_ଗଣନା () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    ଯଦି (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  ସ୍ଥିର ଶୂନ୍ୟ process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    ଯଦି (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = ସତ୍ୟ;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_args (ENV &env, PARAM& param)
  {
    env.କାଢ଼_n_args (env.arg_ଗଦା.get_ଗଣନା ());
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ସ୍ଥିର ତାର୍କିକ is_ସଂଖ୍ୟା_op (op_code_t op)
  {
    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_shortint:
      ସ୍ଥିତି OpCode_fixedcs:
      ସ୍ଥିତି OpCode_TwoBytePosInt0: ସ୍ଥିତି OpCode_TwoBytePosInt1:
      ସ୍ଥିତି OpCode_TwoBytePosInt2: ସ୍ଥିତି OpCode_TwoBytePosInt3:
      ସ୍ଥିତି OpCode_TwoByteNegInt0: ସ୍ଥିତି OpCode_TwoByteNegInt1:
      ସ୍ଥିତି OpCode_TwoByteNegInt2: ସ୍ଥିତି OpCode_TwoByteNegInt3:
	ଫେରନ୍ତୁ ସତ୍ୟ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	/* 1-byte integer */
	ଫେରନ୍ତୁ (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  ସୁରକ୍ଷିତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
ସଂରଚନା path_procs_t
{
  ସ୍ଥିର ଶୂନ୍ୟ rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ଅଚଳ ସଂଖ୍ୟା_t &dy = env.କାଢ଼_arg ();
    ଅଚଳ ସଂଖ୍ୟା_t &dx = env.କାଢ଼_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  ସ୍ଥିର ଶୂନ୍ୟ hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.କାଢ଼_arg ());
    PATH::moveto (env, param, pt1);
  }

  ସ୍ଥିର ଶୂନ୍ୟ vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.କାଢ଼_arg ());
    PATH::moveto (env, param, pt1);
  }

  ସ୍ଥିର ଶୂନ୍ୟ rlineto (ENV &env, PARAM& param)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i + 2 <= env.arg_ଗଦା.get_ଗଣନା (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପାଇଁ (; i + 2 <= env.arg_ଗଦା.get_ଗଣନା (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::ଧାଡ଼ି (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }
    ଯଦି (i < env.arg_ଗଦା.get_ଗଣନା ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପାଇଁ (; i + 2 <= env.arg_ଗଦା.get_ଗଣନା (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::ଧାଡ଼ି (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }
    ଯଦି (i < env.arg_ଗଦା.get_ଗଣନା ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ rrcurveto (ENV &env, PARAM& param)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i + 6 <= env.arg_ଗଦା.get_ଗଣନା (); i += 6)
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

  ସ୍ଥିର ଶୂନ୍ୟ rcurveline (ENV &env, PARAM& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_ଗଣନା = env.arg_ଗଦା.get_ଗଣନା ();
    ଯଦି (unlikely (arg_ଗଣନା < 8))
      ଫେରନ୍ତୁ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା curve_limit = arg_ଗଣନା - 2;
    ପାଇଁ (; i + 6 <= curve_limit; i += 6)
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
    PATH::ଧାଡ଼ି (env, param, pt1);
  }

  ସ୍ଥିର ଶୂନ୍ୟ rlinecurve (ENV &env, PARAM& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg_ଗଣନା = env.arg_ଗଦା.get_ଗଣନା ();
    ଯଦି (unlikely (arg_ଗଣନା < 8))
      ଫେରନ୍ତୁ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_limit = arg_ଗଣନା - 6;
    ପାଇଁ (; i + 2 <= ଧାଡ଼ି_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::ଧାଡ଼ି (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  ସ୍ଥିର ଶୂନ୍ୟ vvcurveto (ENV &env, PARAM& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    point_t pt1 = env.get_pt ();
    ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    ପାଇଁ (; i + 4 <= env.arg_ଗଦା.get_ଗଣନା (); i += 4)
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

  ସ୍ଥିର ଶୂନ୍ୟ hhcurveto (ENV &env, PARAM& param)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    point_t pt1 = env.get_pt ();
    ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    ପାଇଁ (; i + 4 <= env.arg_ଗଦା.get_ଗଣନା (); i += 4)
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

  ସ୍ଥିର ଶୂନ୍ୟ vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      ପାଇଁ (; i + 8 <= env.arg_ଗଦା.get_ଗଣନା (); i += 8)
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
      ଯଦି (i < env.arg_ଗଦା.get_ଗଣନା ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (; i + 8 <= env.arg_ଗଦା.get_ଗଣନା (); i += 8)
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
	ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () - i < 16) && ((env.arg_ଗଦା.get_ଗଣନା () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      ପାଇଁ (; i + 8 <= env.arg_ଗଦା.get_ଗଣନା (); i += 8)
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
      ଯଦି (i < env.arg_ଗଦା.get_ଗଣନା ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    ଅନ୍ୟଥା
    {
      ପାଇଁ (; i + 8 <= env.arg_ଗଦା.get_ଗଣନା (); i += 8)
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
	ଯଦି ((env.arg_ଗଦା.get_ଗଣନା () - i < 16) && ((env.arg_ଗଦା.get_ଗଣନା () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ସ୍ଥିର ଶୂନ୍ୟ moveto (ENV &env, PARAM& param, ଅଚଳ point_t &pt)
  { env.moveto (pt); }

  ସ୍ଥିର ଶୂନ୍ୟ ଧାଡ଼ି (ENV &env, PARAM& param, ଅଚଳ point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  ସ୍ଥିର ଶୂନ୍ୟ curve (ENV &env, PARAM& param, ଅଚଳ point_t &pt1, ଅଚଳ point_t &pt2, ଅଚଳ point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  ସ୍ଥିର ଶୂନ୍ୟ hflex (ENV &env, PARAM& param)
  {
    ଯଦି (likely (env.arg_ଗଦା.get_ଗଣନା () == 7))
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
    ଅନ୍ୟଥା
      env.set_error ();
  }

  ସ୍ଥିର ଶୂନ୍ୟ flex (ENV &env, PARAM& param)
  {
    ଯଦି (likely (env.arg_ଗଦା.get_ଗଣନା () == 13))
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
    ଅନ୍ୟଥା
      env.set_error ();
  }

  ସ୍ଥିର ଶୂନ୍ୟ hflex1 (ENV &env, PARAM& param)
  {
    ଯଦି (likely (env.arg_ଗଦା.get_ଗଣନା () == 9))
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
    ଅନ୍ୟଥା
      env.set_error ();
  }

  ସ୍ଥିର ଶୂନ୍ୟ flex1 (ENV &env, PARAM& param)
  {
    ଯଦି (likely (env.arg_ଗଦା.get_ଗଣନା () == 11))
    {
      point_t d;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 10; i += 2)
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

      ଯଦି (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      ଅନ୍ୟଥା
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ଅନ୍ୟଥା
      env.set_error ();
  }

  ସୁରକ୍ଷିତ:
  ସ୍ଥିର ଶୂନ୍ୟ curve2 (ENV &env, PARAM& param,
		      ଅଚଳ point_t &pt1, ଅଚଳ point_t &pt2, ଅଚଳ point_t &pt3,
		      ଅଚଳ point_t &pt4, ଅଚଳ point_t &pt5, ଅଚଳ point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
ସଂରଚନା cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ତାର୍କିକ interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (ମିଥ୍ୟା);
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

    ତାର୍କିକ ret = ସତ୍ୟ;
    ଯଦି (likely (max_ops))
      ପାଇଁ (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	ଯଦି (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = ମିଥ୍ୟା;
	  ଭାଙ୍ଗ;
	}
	ଯଦି (SUPER::env.is_endchar ())
	  ଭାଙ୍ଗ;
      }
    ଅନ୍ୟଥା
    {
      SUPER::env.set_error ();
      ret = ମିଥ୍ୟା;
    }

    ଯଦି (budget) *budget -= (int64_t) (start_ops - max_ops);

    ଫେରନ୍ତୁ ret;
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
