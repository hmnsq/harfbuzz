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

#شامل "hb.hh"
#شامل "hb-cff-interp-common.hh"

فضاینام CFF {

using فضاینام OT;

شمارش cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ساختار call_context_t
{
  تهی init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned صحیح subr_رقم=0)
  {
    str_ref = substr_;
    type = type_;
    subr_رقم_2 = subr_رقم;
  }

  تهی fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned صحیح    subr_رقم_2;
};

/* call stack */
ثابت unsigned صحیح kMaxCallLimit = 10;
ساختار call_پشته_t : cff_پشته_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
ساختار biased_subrs_t
{
  تهی init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned صحیح  nSubrs = get_شمار ();
    اگر (nSubrs < 1240)
      bias = 107;
    وگرنه اگر (nSubrs < 33900)
      bias = 1131;
    وگرنه
      bias = 32768;
  }

  تهی fini () {}

  unsigned صحیح get_شمار () ثابت { بازگشت subrs ? subrs->شمار : 0; }
  unsigned صحیح get_bias () ثابت  { بازگشت bias; }

  hb_ubytes_t operator [] (unsigned صحیح index) ثابت
  {
    اگر (unlikely (!subrs || index >= subrs->شمار))
      بازگشت hb_ubytes_t ();
    وگرنه
      بازگشت (*subrs)[index];
  }

  محافظتشده:
  unsigned صحیح  bias;
  ثابت SUBRS   *subrs;
};

ساختار point_t
{
  تهی set_int (صحیح _x, صحیح _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  تهی move_x (ثابت عدد_t &dx) { x += dx; }
  تهی move_y (ثابت عدد_t &dy) { y += dy; }
  تهی move (ثابت عدد_t &dx, ثابت عدد_t &dy) { move_x (dx); move_y (dy); }
  تهی move (ثابت point_t &d) { move_x (d.x); move_y (d.y); }

  عدد_t  x;
  عدد_t  y;
};

template <typename ARG, typename SUBRS>
ساختار cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = درست;
    seen_hintmask = نادرست;
    hstem_شمار = 0;
    vstem_شمار = 0;
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

  منطقی in_error () ثابت
  {
    بازگشت call_پشته.in_error () || SUPER::in_error ();
  }

  منطقی برداشت_subr_رقم (ثابت biased_subrs_t<SUBRS>& biasedSubrs, unsigned صحیح &subr_رقم_2)
  {
    subr_رقم_2 = 0;
    صحیح n = SUPER::arg_پشته.برداشت_int ();
    n += biasedSubrs.get_bias ();
    اگر (unlikely ((n < 0) || ((unsigned صحیح)n >= biasedSubrs.get_شمار ())))
      بازگشت نادرست;

    subr_رقم_2 = (unsigned صحیح)n;
    بازگشت درست;
  }

  تهی call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned صحیح subr_رقم_2 = 0;

    اگر (unlikely (!برداشت_subr_رقم (biasedSubrs, subr_رقم_2)
		 || call_پشته.get_شمار () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      بازگشت;
    }
    context.str_ref = SUPER::str_ref;
    call_پشته.push (context);

    context.init ( biasedSubrs[subr_رقم_2], type, subr_رقم_2);
    SUPER::str_ref = context.str_ref;
  }

  تهی return_from_subr ()
  {
    اگر (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_پشته.برداشت ();
    SUPER::str_ref = context.str_ref;
  }

  تهی determine_hintmask_size ()
  {
    اگر (!seen_hintmask)
    {
      vstem_شمار += SUPER::arg_پشته.get_شمار() / 2;
      hintmask_size = (hstem_شمار + vstem_شمار + 7) >> 3;
      seen_hintmask = درست;
    }
  }

  تهی set_endchar (منطقی endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقی is_endchar () ثابت { بازگشت endchar_flag; }

  تهی set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { بازگشت budget; }
  منطقی spend_budget (unsigned صحیح cost, unsigned صحیح mult = 1)
  { بازگشت !budget || hb_budget_spend (*budget, cost, mult); }

  ثابت عدد_t &get_x () ثابت { بازگشت pt.x; }
  ثابت عدد_t &get_y () ثابت { بازگشت pt.y; }
  ثابت point_t &get_pt () ثابت { بازگشت pt; }

  تهی moveto (ثابت point_t &pt_ ) { pt = pt_; }

  عمومی:
  call_context_t   context;
  منطقی	  endchar_flag;
  منطقی	  seen_moveto;
  منطقی	  seen_hintmask;

  unsigned صحیح  hstem_شمار;
  unsigned صحیح  vstem_شمار;
  unsigned صحیح  hintmask_size;
  int64_t	*budget;
  call_پشته_t	call_پشته;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خصوصی:
  point_t	 pt;

  تعریفنوع interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
ساختار path_procs_null_t
{
  ایستا تهی rmoveto (ENV &env, PARAM& param) {}
  ایستا تهی hmoveto (ENV &env, PARAM& param) {}
  ایستا تهی vmoveto (ENV &env, PARAM& param) {}
  ایستا تهی rlineto (ENV &env, PARAM& param) {}
  ایستا تهی hlineto (ENV &env, PARAM& param) {}
  ایستا تهی vlineto (ENV &env, PARAM& param) {}
  ایستا تهی rrcurveto (ENV &env, PARAM& param) {}
  ایستا تهی rcurveline (ENV &env, PARAM& param) {}
  ایستا تهی rlinecurve (ENV &env, PARAM& param) {}
  ایستا تهی vvcurveto (ENV &env, PARAM& param) {}
  ایستا تهی hhcurveto (ENV &env, PARAM& param) {}
  ایستا تهی vhcurveto (ENV &env, PARAM& param) {}
  ایستا تهی hvcurveto (ENV &env, PARAM& param) {}
  ایستا تهی moveto (ENV &env, PARAM& param, ثابت point_t &pt) {}
  ایستا تهی سطر (ENV &env, PARAM& param, ثابت point_t &pt1) {}
  ایستا تهی curve (ENV &env, PARAM& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3) {}
  ایستا تهی hflex (ENV &env, PARAM& param) {}
  ایستا تهی flex (ENV &env, PARAM& param) {}
  ایستا تهی hflex1 (ENV &env, PARAM& param) {}
  ایستا تهی flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
ساختار cs_opset_t : opset_t<ARG>
{
  ایستا تهی process_op (op_code_t op, ENV &env, PARAM& param)
  {
    انتخاب (op) {

      حالت OpCode_return:
	env.return_from_subr ();
	بشکن;
      حالت OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (درست);
	OPSET::flush_args_and_op (op, env, param);
	بشکن;

      حالت OpCode_fixedcs:
	env.arg_پشته.افزودن_fixed_from_substr (env.str_ref);
	بشکن;

      حالت OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	بشکن;

      حالت OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	بشکن;

      حالت OpCode_hstem:
      حالت OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	بشکن;
      حالت OpCode_vstem:
      حالت OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	بشکن;
      حالت OpCode_hintmask:
      حالت OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	بشکن;
      حالت OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	بشکن;
      حالت OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	بشکن;
      حالت OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	بشکن;
      حالت OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	بشکن;
      حالت OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	بشکن;

      حالت OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	بشکن;

      حالت OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	بشکن;

      حالت OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	بشکن;

      حالت OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env);
	بشکن;
    }
  }

  ایستا تهی process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_شمار += env.arg_پشته.get_شمار () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا تهی process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_شمار += env.arg_پشته.get_شمار () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا تهی process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    اگر (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  ایستا تهی process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا تهی check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  ایستا تهی process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    اگر (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = درست;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا تهی process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا تهی flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  ایستا تهی flush_args (ENV &env, PARAM& param)
  {
    env.برداشت_n_args (env.arg_پشته.get_شمار ());
  }

  ایستا تهی flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  ایستا تهی flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  ایستا منطقی is_عدد_op (op_code_t op)
  {
    انتخاب (op)
    {
      حالت OpCode_shortint:
      حالت OpCode_fixedcs:
      حالت OpCode_TwoBytePosInt0: حالت OpCode_TwoBytePosInt1:
      حالت OpCode_TwoBytePosInt2: حالت OpCode_TwoBytePosInt3:
      حالت OpCode_TwoByteNegInt0: حالت OpCode_TwoByteNegInt1:
      حالت OpCode_TwoByteNegInt2: حالت OpCode_TwoByteNegInt3:
	بازگشت درست;

      پیشفرض:
	/* 1-byte integer */
	بازگشت (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  محافظتشده:
  تعریفنوع opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
ساختار path_procs_t
{
  ایستا تهی rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ثابت عدد_t &dy = env.برداشت_arg ();
    ثابت عدد_t &dx = env.برداشت_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  ایستا تهی hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.برداشت_arg ());
    PATH::moveto (env, param, pt1);
  }

  ایستا تهی vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.برداشت_arg ());
    PATH::moveto (env, param, pt1);
  }

  ایستا تهی rlineto (ENV &env, PARAM& param)
  {
    برای (unsigned صحیح i = 0; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
  }

  ایستا تهی hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحیح i = 0;
    برای (; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
    اگر (i < env.arg_پشته.get_شمار ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
    }
  }

  ایستا تهی vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحیح i = 0;
    برای (; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
    اگر (i < env.arg_پشته.get_شمار ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
    }
  }

  ایستا تهی rrcurveto (ENV &env, PARAM& param)
  {
    برای (unsigned صحیح i = 0; i + 6 <= env.arg_پشته.get_شمار (); i += 6)
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

  ایستا تهی rcurveline (ENV &env, PARAM& param)
  {
    unsigned صحیح arg_شمار = env.arg_پشته.get_شمار ();
    اگر (unlikely (arg_شمار < 8))
      بازگشت;

    unsigned صحیح i = 0;
    unsigned صحیح curve_limit = arg_شمار - 2;
    برای (; i + 6 <= curve_limit; i += 6)
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
    PATH::سطر (env, param, pt1);
  }

  ایستا تهی rlinecurve (ENV &env, PARAM& param)
  {
    unsigned صحیح arg_شمار = env.arg_پشته.get_شمار ();
    اگر (unlikely (arg_شمار < 8))
      بازگشت;

    unsigned صحیح i = 0;
    unsigned صحیح سطر_limit = arg_شمار - 6;
    برای (; i + 2 <= سطر_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  ایستا تهی vvcurveto (ENV &env, PARAM& param)
  {
    unsigned صحیح i = 0;
    point_t pt1 = env.get_pt ();
    اگر ((env.arg_پشته.get_شمار () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    برای (; i + 4 <= env.arg_پشته.get_شمار (); i += 4)
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

  ایستا تهی hhcurveto (ENV &env, PARAM& param)
  {
    unsigned صحیح i = 0;
    point_t pt1 = env.get_pt ();
    اگر ((env.arg_پشته.get_شمار () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    برای (; i + 4 <= env.arg_پشته.get_شمار (); i += 4)
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

  ایستا تهی vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned صحیح i = 0;
    اگر ((env.arg_پشته.get_شمار () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      برای (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
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
      اگر (i < env.arg_پشته.get_شمار ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    وگرنه
    {
      برای (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
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
	اگر ((env.arg_پشته.get_شمار () - i < 16) && ((env.arg_پشته.get_شمار () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  ایستا تهی hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned صحیح i = 0;
    اگر ((env.arg_پشته.get_شمار () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      برای (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
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
      اگر (i < env.arg_پشته.get_شمار ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    وگرنه
    {
      برای (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
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
	اگر ((env.arg_پشته.get_شمار () - i < 16) && ((env.arg_پشته.get_شمار () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ایستا تهی moveto (ENV &env, PARAM& param, ثابت point_t &pt)
  { env.moveto (pt); }

  ایستا تهی سطر (ENV &env, PARAM& param, ثابت point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  ایستا تهی curve (ENV &env, PARAM& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  ایستا تهی hflex (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_پشته.get_شمار () == 7))
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
    وگرنه
      env.set_error ();
  }

  ایستا تهی flex (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_پشته.get_شمار () == 13))
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
    وگرنه
      env.set_error ();
  }

  ایستا تهی hflex1 (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_پشته.get_شمار () == 9))
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
    وگرنه
      env.set_error ();
  }

  ایستا تهی flex1 (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_پشته.get_شمار () == 11))
    {
      point_t d;
      برای (unsigned صحیح i = 0; i < 10; i += 2)
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

      اگر (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      وگرنه
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وگرنه
      env.set_error ();
  }

  محافظتشده:
  ایستا تهی curve2 (ENV &env, PARAM& param,
		      ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3,
		      ثابت point_t &pt4, ثابت point_t &pt5, ثابت point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
ساختار cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقی interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (نادرست);
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

    منطقی ret = درست;
    اگر (likely (max_ops))
      برای (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	اگر (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = نادرست;
	  بشکن;
	}
	اگر (SUPER::env.is_endchar ())
	  بشکن;
      }
    وگرنه
    {
      SUPER::env.set_error ();
      ret = نادرست;
    }

    اگر (budget) *budget -= (int64_t) (start_ops - max_ops);

    بازگشت ret;
  }

  خصوصی:
  تعریفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
