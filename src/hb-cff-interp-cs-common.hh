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

#ضمن "hb.hh"
#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

using نطاق OT;

تعداد cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

بنية call_context_t
{
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned صحيح subr_num_=0)
  {
    str_ref = substr_;
    type = type_;
    subr_num = subr_num_;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned صحيح    subr_num;
};

/* call stack */
ثابت unsigned صحيح kMaxCallLimit = 10;
بنية call_stack_t : cff_stack_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned صحيح  nSubrs = get_count ();
    אם (nSubrs < 1240)
      bias = 107;
    אחרת אם (nSubrs < 33900)
      bias = 1131;
    אחרת
      bias = 32768;
  }

  فراغ fini () {}

  unsigned صحيح get_count () ثابت { החזר subrs ? subrs->count : 0; }
  unsigned صحيح get_bias () ثابت  { החזר bias; }

  hb_ubytes_t operator [] (unsigned صحيح index) ثابت
  {
    אם (unlikely (!subrs || index >= subrs->count))
      החזר hb_ubytes_t ();
    אחרת
      החזר (*subrs)[index];
  }

  محمي:
  unsigned صحيح  bias;
  ثابت SUBRS   *subrs;
};

بنية point_t
{
  فراغ set_int (صحيح _x, صحيح _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  فراغ move_x (ثابت number_t &dx) { x += dx; }
  فراغ move_y (ثابت number_t &dy) { y += dy; }
  فراغ move (ثابت number_t &dx, ثابت number_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت point_t &d) { move_x (d.x); move_y (d.y); }

  number_t  x;
  number_t  y;
};

template <typename ARG, typename SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = אמת;
    seen_hintmask = שקר;
    hstem_count = 0;
    vstem_count = 0;
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

  منطقي in_error () ثابت
  {
    החזר callStack.in_error () || SUPER::in_error ();
  }

  منطقي pop_subr_num (ثابت biased_subrs_t<SUBRS>& biasedSubrs, unsigned صحيح &subr_num)
  {
    subr_num = 0;
    صحيح n = SUPER::argStack.pop_int ();
    n += biasedSubrs.get_bias ();
    אם (unlikely ((n < 0) || ((unsigned صحيح)n >= biasedSubrs.get_count ())))
      החזר שקר;

    subr_num = (unsigned صحيح)n;
    החזר אמת;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned صحيح subr_num = 0;

    אם (unlikely (!pop_subr_num (biasedSubrs, subr_num)
		 || callStack.get_count () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      החזר;
    }
    context.str_ref = SUPER::str_ref;
    callStack.push (context);

    context.init ( biasedSubrs[subr_num], type, subr_num);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    אם (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = callStack.pop ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_size ()
  {
    אם (!seen_hintmask)
    {
      vstem_count += SUPER::argStack.get_count() / 2;
      hintmask_size = (hstem_count + vstem_count + 7) >> 3;
      seen_hintmask = אמת;
    }
  }

  فراغ set_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { החזר endchar_flag; }

  فراغ set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { החזר budget; }
  منطقي spend_budget (unsigned صحيح cost, unsigned صحيح mult = 1)
  { החזר !budget || hb_budget_spend (*budget, cost, mult); }

  ثابت number_t &get_x () ثابت { החזר pt.x; }
  ثابت number_t &get_y () ثابت { החזר pt.y; }
  ثابت point_t &get_pt () ثابت { החזר pt; }

  فراغ moveto (ثابت point_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  seen_moveto;
  منطقي	  seen_hintmask;

  unsigned صحيح  hstem_count;
  unsigned صحيح  vstem_count;
  unsigned صحيح  hintmask_size;
  int64_t	*budget;
  call_stack_t	callStack;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خاص:
  point_t	 pt;

  تعريفنوع interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
بنية path_procs_null_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param) {}
  ساكن فراغ hmoveto (ENV &env, PARAM& param) {}
  ساكن فراغ vmoveto (ENV &env, PARAM& param) {}
  ساكن فراغ rlineto (ENV &env, PARAM& param) {}
  ساكن فراغ hlineto (ENV &env, PARAM& param) {}
  ساكن فراغ vlineto (ENV &env, PARAM& param) {}
  ساكن فراغ rrcurveto (ENV &env, PARAM& param) {}
  ساكن فراغ rcurveline (ENV &env, PARAM& param) {}
  ساكن فراغ rlinecurve (ENV &env, PARAM& param) {}
  ساكن فراغ vvcurveto (ENV &env, PARAM& param) {}
  ساكن فراغ hhcurveto (ENV &env, PARAM& param) {}
  ساكن فراغ vhcurveto (ENV &env, PARAM& param) {}
  ساكن فراغ hvcurveto (ENV &env, PARAM& param) {}
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت point_t &pt) {}
  ساكن فراغ line (ENV &env, PARAM& param, ثابت point_t &pt1) {}
  ساكن فراغ curve (ENV &env, PARAM& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3) {}
  ساكن فراغ hflex (ENV &env, PARAM& param) {}
  ساكن فراغ flex (ENV &env, PARAM& param) {}
  ساكن فراغ hflex1 (ENV &env, PARAM& param) {}
  ساكن فراغ flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
بنية cs_opset_t : opset_t<ARG>
{
  ساكن فراغ process_op (op_code_t op, ENV &env, PARAM& param)
  {
    بدل (op) {

      حالة OpCode_return:
	env.return_from_subr ();
	اكسر;
      حالة OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (אמת);
	OPSET::flush_args_and_op (op, env, param);
	اكسر;

      حالة OpCode_fixedcs:
	env.argStack.push_fixed_from_substr (env.str_ref);
	اكسر;

      حالة OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	اكسر;

      حالة OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	اكسر;

      حالة OpCode_hstem:
      حالة OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	اكسر;
      حالة OpCode_vstem:
      حالة OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	اكسر;
      حالة OpCode_hintmask:
      حالة OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	اكسر;
      حالة OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	اكسر;
      حالة OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	اكسر;

      حالة OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	اكسر;
    }
  }

  ساكن فراغ process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_count += env.argStack.get_count () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_count += env.argStack.get_count () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    אם (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  ساكن فراغ process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن فراغ check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    אם (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = אמת;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن فراغ process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن فراغ flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_args (ENV &env, PARAM& param)
  {
    env.pop_n_args (env.argStack.get_count ());
  }

  ساكن فراغ flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ساكن منطقي is_number_op (op_code_t op)
  {
    بدل (op)
    {
      حالة OpCode_shortint:
      حالة OpCode_fixedcs:
      حالة OpCode_TwoBytePosInt0: حالة OpCode_TwoBytePosInt1:
      حالة OpCode_TwoBytePosInt2: حالة OpCode_TwoBytePosInt3:
      حالة OpCode_TwoByteNegInt0: حالة OpCode_TwoByteNegInt1:
      حالة OpCode_TwoByteNegInt2: حالة OpCode_TwoByteNegInt3:
	החזר אמת;

      افتراضي:
	/* 1-byte integer */
	החזר (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  محمي:
  تعريفنوع opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
بنية path_procs_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ثابت number_t &dy = env.pop_arg ();
    ثابت number_t &dx = env.pop_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.pop_arg ());
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.pop_arg ());
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    עבור (unsigned صحيح i = 0; i + 2 <= env.argStack.get_count (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::line (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحيح i = 0;
    עבור (; i + 2 <= env.argStack.get_count (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::line (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::line (env, param, pt1);
    }
    אם (i < env.argStack.get_count ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::line (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحيح i = 0;
    עבור (; i + 2 <= env.argStack.get_count (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::line (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::line (env, param, pt1);
    }
    אם (i < env.argStack.get_count ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::line (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    עבור (unsigned صحيح i = 0; i + 6 <= env.argStack.get_count (); i += 6)
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

  ساكن فراغ rcurveline (ENV &env, PARAM& param)
  {
    unsigned صحيح arg_count = env.argStack.get_count ();
    אם (unlikely (arg_count < 8))
      החזר;

    unsigned صحيح i = 0;
    unsigned صحيح curve_limit = arg_count - 2;
    עבור (; i + 6 <= curve_limit; i += 6)
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
    PATH::line (env, param, pt1);
  }

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    unsigned صحيح arg_count = env.argStack.get_count ();
    אם (unlikely (arg_count < 8))
      החזר;

    unsigned صحيح i = 0;
    unsigned صحيح line_limit = arg_count - 6;
    עבור (; i + 2 <= line_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::line (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    unsigned صحيح i = 0;
    point_t pt1 = env.get_pt ();
    אם ((env.argStack.get_count () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    עבור (; i + 4 <= env.argStack.get_count (); i += 4)
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

  ساكن فراغ hhcurveto (ENV &env, PARAM& param)
  {
    unsigned صحيح i = 0;
    point_t pt1 = env.get_pt ();
    אם ((env.argStack.get_count () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    עבור (; i + 4 <= env.argStack.get_count (); i += 4)
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

  ساكن فراغ vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned صحيح i = 0;
    אם ((env.argStack.get_count () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      עבור (; i + 8 <= env.argStack.get_count (); i += 8)
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
      אם (i < env.argStack.get_count ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    אחרת
    {
      עבור (; i + 8 <= env.argStack.get_count (); i += 8)
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
	אם ((env.argStack.get_count () - i < 16) && ((env.argStack.get_count () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned صحيح i = 0;
    אם ((env.argStack.get_count () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      עבור (; i + 8 <= env.argStack.get_count (); i += 8)
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
      אם (i < env.argStack.get_count ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    אחרת
    {
      עבור (; i + 8 <= env.argStack.get_count (); i += 8)
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
	אם ((env.argStack.get_count () - i < 16) && ((env.argStack.get_count () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت point_t &pt)
  { env.moveto (pt); }

  ساكن فراغ line (ENV &env, PARAM& param, ثابت point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  ساكن فراغ curve (ENV &env, PARAM& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    אם (likely (env.argStack.get_count () == 7))
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
    אחרת
      env.set_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    אם (likely (env.argStack.get_count () == 13))
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
    אחרת
      env.set_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    אם (likely (env.argStack.get_count () == 9))
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
    אחרת
      env.set_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    אם (likely (env.argStack.get_count () == 11))
    {
      point_t d;
      עבור (unsigned صحيح i = 0; i < 10; i += 2)
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

      אם (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      אחרת
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    אחרת
      env.set_error ();
  }

  محمي:
  ساكن فراغ curve2 (ENV &env, PARAM& param,
		      ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3,
		      ثابت point_t &pt4, ثابت point_t &pt5, ثابت point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
بنية cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (שקר);
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

    منطقي ret = אמת;
    אם (likely (max_ops))
      עבור (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	אם (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = שקר;
	  اكسر;
	}
	אם (SUPER::env.is_endchar ())
	  اكسر;
      }
    אחרת
    {
      SUPER::env.set_error ();
      ret = שקר;
    }

    אם (budget) *budget -= (int64_t) (start_ops - max_ops);

    החזר ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
