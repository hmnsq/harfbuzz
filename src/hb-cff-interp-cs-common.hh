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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cff-interp-common.hh"

नामाकाश CFF {

using नामाकाश OT;

गणना cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

संरचना call_context_t
{
  शून्य init (ध्रुव byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned पूर्णाङ्क subr_अङ्कः=0)
  {
    str_ref = substr_;
    type = type_;
    subr_अङ्कः_2 = subr_अङ्कः;
  }

  शून्य fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned पूर्णाङ्क    subr_अङ्कः_2;
};

/* call stack */
ध्रुव unsigned पूर्णाङ्क kMaxCallLimit = 10;
संरचना call_राशिः_t : cff_राशिः_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
संरचना biased_subrs_t
{
  शून्य init (ध्रुव SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned पूर्णाङ्क  nSubrs = get_गणनम् ();
    जइ (nSubrs < 1240)
      bias = 107;
    अण्णहा जइ (nSubrs < 33900)
      bias = 1131;
    अण्णहा
      bias = 32768;
  }

  शून्य fini () {}

  unsigned पूर्णाङ्क get_गणनम् () ध्रुव { प्रत्यागम subrs ? subrs->गणनम् : 0; }
  unsigned पूर्णाङ्क get_bias () ध्रुव  { प्रत्यागम bias; }

  hb_ubytes_t operator [] (unsigned पूर्णाङ्क index) ध्रुव
  {
    जइ (unlikely (!subrs || index >= subrs->गणनम्))
      प्रत्यागम hb_ubytes_t ();
    अण्णहा
      प्रत्यागम (*subrs)[index];
  }

  रक्षित:
  unsigned पूर्णाङ्क  bias;
  ध्रुव SUBRS   *subrs;
};

संरचना point_t
{
  शून्य set_int (पूर्णाङ्क _x, पूर्णाङ्क _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  शून्य move_x (ध्रुव सङ्ख्या_t &dx) { x += dx; }
  शून्य move_y (ध्रुव सङ्ख्या_t &dy) { y += dy; }
  शून्य move (ध्रुव सङ्ख्या_t &dx, ध्रुव सङ्ख्या_t &dy) { move_x (dx); move_y (dy); }
  शून्य move (ध्रुव point_t &d) { move_x (d.x); move_y (d.y); }

  सङ्ख्या_t  x;
  सङ्ख्या_t  y;
};

template <typename ARG, typename SUBRS>
संरचना cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ध्रुव hb_ubytes_t &str, ध्रुव SUBRS *globalSubrs_, ध्रुव SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = सच्च;
    seen_hintmask = मिच्छ;
    hstem_गणनम् = 0;
    vstem_गणनम् = 0;
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

  तार्किक in_error () ध्रुव
  {
    प्रत्यागम call_राशिः.in_error () || SUPER::in_error ();
  }

  तार्किक निष्कासय_subr_अङ्कः (ध्रुव biased_subrs_t<SUBRS>& biasedSubrs, unsigned पूर्णाङ्क &subr_अङ्कः_2)
  {
    subr_अङ्कः_2 = 0;
    पूर्णाङ्क n = SUPER::arg_राशिः.निष्कासय_int ();
    n += biasedSubrs.get_bias ();
    जइ (unlikely ((n < 0) || ((unsigned पूर्णाङ्क)n >= biasedSubrs.get_गणनम् ())))
      प्रत्यागम मिच्छ;

    subr_अङ्कः_2 = (unsigned पूर्णाङ्क)n;
    प्रत्यागम सच्च;
  }

  शून्य call_subr (ध्रुव biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned पूर्णाङ्क subr_अङ्कः_2 = 0;

    जइ (unlikely (!निष्कासय_subr_अङ्कः (biasedSubrs, subr_अङ्कः_2)
		 || call_राशिः.get_गणनम् () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      प्रत्यागम;
    }
    context.str_ref = SUPER::str_ref;
    call_राशिः.push (context);

    context.init ( biasedSubrs[subr_अङ्कः_2], type, subr_अङ्कः_2);
    SUPER::str_ref = context.str_ref;
  }

  शून्य return_from_subr ()
  {
    जइ (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_राशिः.निष्कासय ();
    SUPER::str_ref = context.str_ref;
  }

  शून्य determine_hintmask_size ()
  {
    जइ (!seen_hintmask)
    {
      vstem_गणनम् += SUPER::arg_राशिः.get_गणनम्() / 2;
      hintmask_size = (hstem_गणनम् + vstem_गणनम् + 7) >> 3;
      seen_hintmask = सच्च;
    }
  }

  शून्य set_endchar (तार्किक endchar_flag_) { endchar_flag = endchar_flag_; }
  तार्किक is_endchar () ध्रुव { प्रत्यागम endchar_flag; }

  शून्य set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ध्रुव { प्रत्यागम budget; }
  तार्किक spend_budget (unsigned पूर्णाङ्क cost, unsigned पूर्णाङ्क mult = 1)
  { प्रत्यागम !budget || hb_budget_spend (*budget, cost, mult); }

  ध्रुव सङ्ख्या_t &get_x () ध्रुव { प्रत्यागम pt.x; }
  ध्रुव सङ्ख्या_t &get_y () ध्रुव { प्रत्यागम pt.y; }
  ध्रुव point_t &get_pt () ध्रुव { प्रत्यागम pt; }

  शून्य moveto (ध्रुव point_t &pt_ ) { pt = pt_; }

  सार्वजनिक:
  call_context_t   context;
  तार्किक	  endchar_flag;
  तार्किक	  seen_moveto;
  तार्किक	  seen_hintmask;

  unsigned पूर्णाङ्क  hstem_गणनम्;
  unsigned पूर्णाङ्क  vstem_गणनम्;
  unsigned पूर्णाङ्क  hintmask_size;
  int64_t	*budget;
  call_राशिः_t	call_राशिः;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  गुप्त:
  point_t	 pt;

  प्रकारनिर्देश interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
संरचना path_procs_null_t
{
  स्थिर शून्य rmoveto (ENV &env, PARAM& param) {}
  स्थिर शून्य hmoveto (ENV &env, PARAM& param) {}
  स्थिर शून्य vmoveto (ENV &env, PARAM& param) {}
  स्थिर शून्य rlineto (ENV &env, PARAM& param) {}
  स्थिर शून्य hlineto (ENV &env, PARAM& param) {}
  स्थिर शून्य vlineto (ENV &env, PARAM& param) {}
  स्थिर शून्य rrcurveto (ENV &env, PARAM& param) {}
  स्थिर शून्य rcurveline (ENV &env, PARAM& param) {}
  स्थिर शून्य rlinecurve (ENV &env, PARAM& param) {}
  स्थिर शून्य vvcurveto (ENV &env, PARAM& param) {}
  स्थिर शून्य hhcurveto (ENV &env, PARAM& param) {}
  स्थिर शून्य vhcurveto (ENV &env, PARAM& param) {}
  स्थिर शून्य hvcurveto (ENV &env, PARAM& param) {}
  स्थिर शून्य moveto (ENV &env, PARAM& param, ध्रुव point_t &pt) {}
  स्थिर शून्य पङ्क्तिः (ENV &env, PARAM& param, ध्रुव point_t &pt1) {}
  स्थिर शून्य curve (ENV &env, PARAM& param, ध्रुव point_t &pt1, ध्रुव point_t &pt2, ध्रुव point_t &pt3) {}
  स्थिर शून्य hflex (ENV &env, PARAM& param) {}
  स्थिर शून्य flex (ENV &env, PARAM& param) {}
  स्थिर शून्य hflex1 (ENV &env, PARAM& param) {}
  स्थिर शून्य flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
संरचना cs_opset_t : opset_t<ARG>
{
  स्थिर शून्य process_op (op_code_t op, ENV &env, PARAM& param)
  {
    विकल्प (op) {

      पक्ष OpCode_return:
	env.return_from_subr ();
	भङ्ग;
      पक्ष OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (सच्च);
	OPSET::flush_args_and_op (op, env, param);
	भङ्ग;

      पक्ष OpCode_fixedcs:
	env.arg_राशिः.प्रक्षिप_fixed_from_substr (env.str_ref);
	भङ्ग;

      पक्ष OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	भङ्ग;

      पक्ष OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	भङ्ग;

      पक्ष OpCode_hstem:
      पक्ष OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	भङ्ग;
      पक्ष OpCode_vstem:
      पक्ष OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	भङ्ग;
      पक्ष OpCode_hintmask:
      पक्ष OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	भङ्ग;
      पक्ष OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	भङ्ग;
      पक्ष OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	भङ्ग;
      पक्ष OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	भङ्ग;
      पक्ष OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	भङ्ग;
      पक्ष OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	भङ्ग;

      पक्ष OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	भङ्ग;

      पक्ष OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	भङ्ग;

      पक्ष OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	भङ्ग;

      पक्ष OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env);
	भङ्ग;
    }
  }

  स्थिर शून्य process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_गणनम् += env.arg_राशिः.get_गणनम् () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर शून्य process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_गणनम् += env.arg_राशिः.get_गणनम् () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर शून्य process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    जइ (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  स्थिर शून्य process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर शून्य check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  स्थिर शून्य process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    जइ (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = सच्च;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर शून्य process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर शून्य flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  स्थिर शून्य flush_args (ENV &env, PARAM& param)
  {
    env.निष्कासय_n_args (env.arg_राशिः.get_गणनम् ());
  }

  स्थिर शून्य flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  स्थिर शून्य flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  स्थिर तार्किक is_सङ्ख्या_op (op_code_t op)
  {
    विकल्प (op)
    {
      पक्ष OpCode_shortint:
      पक्ष OpCode_fixedcs:
      पक्ष OpCode_TwoBytePosInt0: पक्ष OpCode_TwoBytePosInt1:
      पक्ष OpCode_TwoBytePosInt2: पक्ष OpCode_TwoBytePosInt3:
      पक्ष OpCode_TwoByteNegInt0: पक्ष OpCode_TwoByteNegInt1:
      पक्ष OpCode_TwoByteNegInt2: पक्ष OpCode_TwoByteNegInt3:
	प्रत्यागम सच्च;

      स्वभाव:
	/* 1-byte integer */
	प्रत्यागम (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  रक्षित:
  प्रकारनिर्देश opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
संरचना path_procs_t
{
  स्थिर शून्य rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ध्रुव सङ्ख्या_t &dy = env.निष्कासय_arg ();
    ध्रुव सङ्ख्या_t &dx = env.निष्कासय_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  स्थिर शून्य hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.निष्कासय_arg ());
    PATH::moveto (env, param, pt1);
  }

  स्थिर शून्य vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.निष्कासय_arg ());
    PATH::moveto (env, param, pt1);
  }

  स्थिर शून्य rlineto (ENV &env, PARAM& param)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i + 2 <= env.arg_राशिः.get_गणनम् (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::पङ्क्तिः (env, param, pt1);
    }
  }

  स्थिर शून्य hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned पूर्णाङ्क i = 0;
    प्रति (; i + 2 <= env.arg_राशिः.get_गणनम् (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::पङ्क्तिः (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::पङ्क्तिः (env, param, pt1);
    }
    जइ (i < env.arg_राशिः.get_गणनम् ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::पङ्क्तिः (env, param, pt1);
    }
  }

  स्थिर शून्य vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned पूर्णाङ्क i = 0;
    प्रति (; i + 2 <= env.arg_राशिः.get_गणनम् (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::पङ्क्तिः (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::पङ्क्तिः (env, param, pt1);
    }
    जइ (i < env.arg_राशिः.get_गणनम् ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::पङ्क्तिः (env, param, pt1);
    }
  }

  स्थिर शून्य rrcurveto (ENV &env, PARAM& param)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i + 6 <= env.arg_राशिः.get_गणनम् (); i += 6)
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

  स्थिर शून्य rcurveline (ENV &env, PARAM& param)
  {
    unsigned पूर्णाङ्क arg_गणनम् = env.arg_राशिः.get_गणनम् ();
    जइ (unlikely (arg_गणनम् < 8))
      प्रत्यागम;

    unsigned पूर्णाङ्क i = 0;
    unsigned पूर्णाङ्क curve_limit = arg_गणनम् - 2;
    प्रति (; i + 6 <= curve_limit; i += 6)
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
    PATH::पङ्क्तिः (env, param, pt1);
  }

  स्थिर शून्य rlinecurve (ENV &env, PARAM& param)
  {
    unsigned पूर्णाङ्क arg_गणनम् = env.arg_राशिः.get_गणनम् ();
    जइ (unlikely (arg_गणनम् < 8))
      प्रत्यागम;

    unsigned पूर्णाङ्क i = 0;
    unsigned पूर्णाङ्क पङ्क्तिः_limit = arg_गणनम् - 6;
    प्रति (; i + 2 <= पङ्क्तिः_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::पङ्क्तिः (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  स्थिर शून्य vvcurveto (ENV &env, PARAM& param)
  {
    unsigned पूर्णाङ्क i = 0;
    point_t pt1 = env.get_pt ();
    जइ ((env.arg_राशिः.get_गणनम् () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    प्रति (; i + 4 <= env.arg_राशिः.get_गणनम् (); i += 4)
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

  स्थिर शून्य hhcurveto (ENV &env, PARAM& param)
  {
    unsigned पूर्णाङ्क i = 0;
    point_t pt1 = env.get_pt ();
    जइ ((env.arg_राशिः.get_गणनम् () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    प्रति (; i + 4 <= env.arg_राशिः.get_गणनम् (); i += 4)
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

  स्थिर शून्य vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned पूर्णाङ्क i = 0;
    जइ ((env.arg_राशिः.get_गणनम् () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      प्रति (; i + 8 <= env.arg_राशिः.get_गणनम् (); i += 8)
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
      जइ (i < env.arg_राशिः.get_गणनम् ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    अण्णहा
    {
      प्रति (; i + 8 <= env.arg_राशिः.get_गणनम् (); i += 8)
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
	जइ ((env.arg_राशिः.get_गणनम् () - i < 16) && ((env.arg_राशिः.get_गणनम् () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  स्थिर शून्य hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned पूर्णाङ्क i = 0;
    जइ ((env.arg_राशिः.get_गणनम् () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      प्रति (; i + 8 <= env.arg_राशिः.get_गणनम् (); i += 8)
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
      जइ (i < env.arg_राशिः.get_गणनम् ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    अण्णहा
    {
      प्रति (; i + 8 <= env.arg_राशिः.get_गणनम् (); i += 8)
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
	जइ ((env.arg_राशिः.get_गणनम् () - i < 16) && ((env.arg_राशिः.get_गणनम् () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  स्थिर शून्य moveto (ENV &env, PARAM& param, ध्रुव point_t &pt)
  { env.moveto (pt); }

  स्थिर शून्य पङ्क्तिः (ENV &env, PARAM& param, ध्रुव point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  स्थिर शून्य curve (ENV &env, PARAM& param, ध्रुव point_t &pt1, ध्रुव point_t &pt2, ध्रुव point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  स्थिर शून्य hflex (ENV &env, PARAM& param)
  {
    जइ (likely (env.arg_राशिः.get_गणनम् () == 7))
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
    अण्णहा
      env.set_error ();
  }

  स्थिर शून्य flex (ENV &env, PARAM& param)
  {
    जइ (likely (env.arg_राशिः.get_गणनम् () == 13))
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
    अण्णहा
      env.set_error ();
  }

  स्थिर शून्य hflex1 (ENV &env, PARAM& param)
  {
    जइ (likely (env.arg_राशिः.get_गणनम् () == 9))
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
    अण्णहा
      env.set_error ();
  }

  स्थिर शून्य flex1 (ENV &env, PARAM& param)
  {
    जइ (likely (env.arg_राशिः.get_गणनम् () == 11))
    {
      point_t d;
      प्रति (unsigned पूर्णाङ्क i = 0; i < 10; i += 2)
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

      जइ (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      अण्णहा
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    अण्णहा
      env.set_error ();
  }

  रक्षित:
  स्थिर शून्य curve2 (ENV &env, PARAM& param,
		      ध्रुव point_t &pt1, ध्रुव point_t &pt2, ध्रुव point_t &pt3,
		      ध्रुव point_t &pt4, ध्रुव point_t &pt5, ध्रुव point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
संरचना cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  तार्किक interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (मिच्छ);
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

    तार्किक ret = सच्च;
    जइ (likely (max_ops))
      प्रति (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	जइ (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = मिच्छ;
	  भङ्ग;
	}
	जइ (SUPER::env.is_endchar ())
	  भङ्ग;
      }
    अण्णहा
    {
      SUPER::env.set_error ();
      ret = मिच्छ;
    }

    जइ (budget) *budget -= (int64_t) (start_ops - max_ops);

    प्रत्यागम ret;
  }

  गुप्त:
  प्रकारनिर्देश interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
