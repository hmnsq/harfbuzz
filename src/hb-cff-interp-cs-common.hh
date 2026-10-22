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
#اگر_غیر_تعریف_شدہ HB_CFF_INTERP_CS_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_CS_COMMON_HH

#شامل "hb.hh"
#شامل "hb-cff-interp-common.hh"

نامخلا CFF {

استعمال نامخلا OT;

شمار cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

ساخت call_context_t
{
  خالی init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, غیرعلامتی صحیح subr_عدد=0)
  {
    str_ref = substr_;
    type = type_;
    subr_عدد_2 = subr_عدد;
  }

  خالی fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  غیرعلامتی صحیح    subr_عدد_2;
};

/* call stack */
ثابت غیرعلامتی صحیح k_Max_Call_حد_بندی = 10;
ساخت call_ڈھیر_t : cff_ڈھیر_t<call_context_t, k_Max_Call_حد_بندی> {};

سانچہ <قسم_نام SUBRS>
ساخت biased_subrs_t
{
  خالی init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    غیرعلامتی صحیح  nSubrs = get_گنتی ();
    اگر (nSubrs < 1240)
      bias = 107;
    ورنہ اگر (nSubrs < 33900)
      bias = 1131;
    ورنہ
      bias = 32768;
  }

  خالی fini () {}

  غیرعلامتی صحیح get_گنتی () ثابت { واپس subrs ? subrs->گنتی : 0; }
  غیرعلامتی صحیح get_bias () ثابت  { واپس bias; }

  hb_ubytes_t عامل [] (غیرعلامتی صحیح اشاریہ_2) ثابت
  {
    اگر (unlikely (!subrs || اشاریہ_2 >= subrs->گنتی))
      واپس hb_ubytes_t ();
    ورنہ
      واپس (*subrs)[اشاریہ_2];
  }

  محفوظ:
  غیرعلامتی صحیح  bias;
  ثابت SUBRS   *subrs;
};

ساخت نقطہ_t
{
  خالی سیٹ_int (صحیح _x, صحیح _y)
  {
    x.سیٹ_int (_x);
    y.سیٹ_int (_y);
  }

  خالی move_x (ثابت نمبر_t &dx) { x += dx; }
  خالی move_y (ثابت نمبر_t &dy) { y += dy; }
  خالی move (ثابت نمبر_t &dx, ثابت نمبر_t &dy) { move_x (dx); move_y (dy); }
  خالی move (ثابت نقطہ_t &d) { move_x (d.x); move_y (d.y); }

  نمبر_t  x;
  نمبر_t  y;
};

سانچہ <قسم_نام ARG, قسم_نام SUBRS>
ساخت cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    نظر_آیا_moveto = سچ;
    نظر_آیا_hintmask = جھوٹ;
    hstem_گنتی = 0;
    vstem_گنتی = 0;
    hintmask_سائز = 0;
    budget = خالی_اشاریہ;
    pt.سیٹ_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  منطقی اندر_error () ثابت
  {
    واپس call_ڈھیر.اندر_error () || SUPER::اندر_error ();
  }

  منطقی نکالو_subr_عدد (ثابت biased_subrs_t<SUBRS>& biasedSubrs, غیرعلامتی صحیح &subr_عدد_2)
  {
    subr_عدد_2 = 0;
    صحیح n = SUPER::arg_ڈھیر.نکالو_int ();
    n += biasedSubrs.get_bias ();
    اگر (unlikely ((n < 0) || ((غیرعلامتی صحیح)n >= biasedSubrs.get_گنتی ())))
      واپس جھوٹ;

    subr_عدد_2 = (غیرعلامتی صحیح)n;
    واپس سچ;
  }

  خالی call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    غیرعلامتی صحیح subr_عدد_2 = 0;

    اگر (unlikely (!نکالو_subr_عدد (biasedSubrs, subr_عدد_2)
		 || call_ڈھیر.get_گنتی () >= k_Max_Call_حد_بندی))
    {
      SUPER::سیٹ_error ();
      واپس;
    }
    context.str_ref = SUPER::str_ref;
    call_ڈھیر.push (context);

    context.init ( biasedSubrs[subr_عدد_2], type, subr_عدد_2);
    SUPER::str_ref = context.str_ref;
  }

  خالی return_from_subr ()
  {
    اگر (unlikely (SUPER::str_ref.اندر_error ()))
      SUPER::سیٹ_error ();
    context = call_ڈھیر.نکالو ();
    SUPER::str_ref = context.str_ref;
  }

  خالی determine_hintmask_سائز ()
  {
    اگر (!نظر_آیا_hintmask)
    {
      vstem_گنتی += SUPER::arg_ڈھیر.get_گنتی() / 2;
      hintmask_سائز = (hstem_گنتی + vstem_گنتی + 7) >> 3;
      نظر_آیا_hintmask = سچ;
    }
  }

  خالی سیٹ_endchar (منطقی endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقی is_endchar () ثابت { واپس endchar_flag; }

  خالی سیٹ_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { واپس budget; }
  منطقی spend_budget (غیرعلامتی صحیح لاگت, غیرعلامتی صحیح mult = 1)
  { واپس !budget || hb_budget_spend (*budget, لاگت, mult); }

  ثابت نمبر_t &get_x () ثابت { واپس pt.x; }
  ثابت نمبر_t &get_y () ثابت { واپس pt.y; }
  ثابت نقطہ_t &get_pt () ثابت { واپس pt; }

  خالی moveto (ثابت نقطہ_t &pt_ ) { pt = pt_; }

  عوامی:
  call_context_t   context;
  منطقی	  endchar_flag;
  منطقی	  نظر_آیا_moveto;
  منطقی	  نظر_آیا_hintmask;

  غیرعلامتی صحیح  hstem_گنتی;
  غیرعلامتی صحیح  vstem_گنتی;
  غیرعلامتی صحیح  hintmask_سائز;
  int64_t	*budget;
  call_ڈھیر_t	call_ڈھیر;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  نجی:
  نقطہ_t	 pt;

  قسمتعریف interp_env_t<ARG> SUPER;
};

سانچہ <قسم_نام ENV, قسم_نام PARAM>
ساخت راستہ_procs_null_t
{
  ساکن خالی rmoveto (ENV &env, PARAM& param) {}
  ساکن خالی hmoveto (ENV &env, PARAM& param) {}
  ساکن خالی vmoveto (ENV &env, PARAM& param) {}
  ساکن خالی rlineto (ENV &env, PARAM& param) {}
  ساکن خالی hlineto (ENV &env, PARAM& param) {}
  ساکن خالی vlineto (ENV &env, PARAM& param) {}
  ساکن خالی rrcurveto (ENV &env, PARAM& param) {}
  ساکن خالی rcurveline (ENV &env, PARAM& param) {}
  ساکن خالی rlinecurve (ENV &env, PARAM& param) {}
  ساکن خالی vvcurveto (ENV &env, PARAM& param) {}
  ساکن خالی hhcurveto (ENV &env, PARAM& param) {}
  ساکن خالی vhcurveto (ENV &env, PARAM& param) {}
  ساکن خالی hvcurveto (ENV &env, PARAM& param) {}
  ساکن خالی moveto (ENV &env, PARAM& param, ثابت نقطہ_t &pt) {}
  ساکن خالی سطر (ENV &env, PARAM& param, ثابت نقطہ_t &pt1) {}
  ساکن خالی خم (ENV &env, PARAM& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3) {}
  ساکن خالی hflex (ENV &env, PARAM& param) {}
  ساکن خالی flex (ENV &env, PARAM& param) {}
  ساکن خالی hflex1 (ENV &env, PARAM& param) {}
  ساکن خالی flex1 (ENV &env, PARAM& param) {}
};

سانچہ <قسم_نام ARG, قسم_نام OPSET, قسم_نام ENV, قسم_نام PARAM, قسم_نام راستہ_3=راستہ_procs_null_t<ENV, PARAM>>
ساخت cs_opset_t : opset_t<ARG>
{
  ساکن خالی process_op (op_رمز_t op, ENV &env, PARAM& param)
  {
    انتخاب (op) {

      حالت Op_رمز_return:
	env.return_from_subr ();
	توڑو;
      حالت Op_رمز_endchar:
	OPSET::check_چوڑائی (op, env, param);
	env.سیٹ_endchar (سچ);
	OPSET::flush_دلائل_and_op (op, env, param);
	توڑو;

      حالت Op_رمز_fixedcs:
	env.arg_ڈھیر.ڈالو_fixed_from_substr (env.str_ref);
	توڑو;

      حالت Op_رمز_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	توڑو;

      حالت Op_رمز_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	توڑو;

      حالت Op_رمز_hstem:
      حالت Op_رمز_hstemhm:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_hstem (op, env, param);
	توڑو;
      حالت Op_رمز_vstem:
      حالت Op_رمز_vstemhm:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_vstem (op, env, param);
	توڑو;
      حالت Op_رمز_hintmask:
      حالت Op_رمز_cntrmask:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_hintmask (op, env, param);
	توڑو;
      حالت Op_رمز_rmoveto:
	OPSET::check_چوڑائی (op, env, param);
	راستہ_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	توڑو;
      حالت Op_رمز_hmoveto:
	OPSET::check_چوڑائی (op, env, param);
	راستہ_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	توڑو;
      حالت Op_رمز_vmoveto:
	OPSET::check_چوڑائی (op, env, param);
	راستہ_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	توڑو;
      حالت Op_رمز_rlineto:
	راستہ_3::rlineto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_hlineto:
	راستہ_3::hlineto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_vlineto:
	راستہ_3::vlineto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_rrcurveto:
	راستہ_3::rrcurveto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_rcurveline:
	راستہ_3::rcurveline (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_rlinecurve:
	راستہ_3::rlinecurve (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_vvcurveto:
	راستہ_3::vvcurveto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_hhcurveto:
	راستہ_3::hhcurveto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_vhcurveto:
	راستہ_3::vhcurveto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;
      حالت Op_رمز_hvcurveto:
	راستہ_3::hvcurveto (env, param);
	process_post_راستہ (op, env, param);
	توڑو;

      حالت Op_رمز_hflex:
	راستہ_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	توڑو;

      حالت Op_رمز_flex:
	راستہ_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	توڑو;

      حالت Op_رمز_hflex1:
	راستہ_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	توڑو;

      حالت Op_رمز_flex1:
	راستہ_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env);
	توڑو;
    }
  }

  ساکن خالی process_hstem (op_رمز_t op, ENV &env, PARAM& param)
  {
    env.hstem_گنتی += env.arg_ڈھیر.get_گنتی () / 2;
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن خالی process_vstem (op_رمز_t op, ENV &env, PARAM& param)
  {
    env.vstem_گنتی += env.arg_ڈھیر.get_گنتی () / 2;
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن خالی process_hintmask (op_رمز_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_سائز ();
    اگر (likely (env.str_ref.avail (env.hintmask_سائز)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_سائز);
    }
  }

  ساکن خالی process_post_flex (op_رمز_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن خالی check_چوڑائی (op_رمز_t op, ENV &env, PARAM& param)
  {}

  ساکن خالی process_post_move (op_رمز_t op, ENV &env, PARAM& param)
  {
    اگر (!env.نظر_آیا_moveto)
    {
      env.determine_hintmask_سائز ();
      env.نظر_آیا_moveto = سچ;
    }
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن خالی process_post_راستہ (op_رمز_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن خالی flush_دلائل_and_op (op_رمز_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلائل (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساکن خالی flush_دلائل (ENV &env, PARAM& param)
  {
    env.نکالو_n_دلائل (env.arg_ڈھیر.get_گنتی ());
  }

  ساکن خالی flush_op (op_رمز_t op, ENV &env, PARAM& param)
  {
  }

  ساکن خالی flush_hintmask (op_رمز_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلائل_and_op (op, env, param);
  }

  ساکن منطقی is_نمبر_op (op_رمز_t op)
  {
    انتخاب (op)
    {
      حالت Op_رمز_shortint:
      حالت Op_رمز_fixedcs:
      حالت Op_رمز_Two_Byte_Pos_Int0: حالت Op_رمز_Two_Byte_Pos_Int1:
      حالت Op_رمز_Two_Byte_Pos_Int2: حالت Op_رمز_Two_Byte_Pos_Int3:
      حالت Op_رمز_Two_Byte_Neg_Int0: حالت Op_رمز_Two_Byte_Neg_Int1:
      حالت Op_رمز_Two_Byte_Neg_Int2: حالت Op_رمز_Two_Byte_Neg_Int3:
	واپس سچ;

      طےشدہ:
	/* 1-byte integer */
	واپس (Op_رمز_One_Byte_Int_پہلا <= op) && (op <= Op_رمز_One_Byte_Int_آخری);
    }
  }

  محفوظ:
  قسمتعریف opset_t<ARG>  SUPER;
};

سانچہ <قسم_نام راستہ_3, قسم_نام ENV, قسم_نام PARAM>
ساخت راستہ_procs_t
{
  ساکن خالی rmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    ثابت نمبر_t &dy = env.نکالو_arg ();
    ثابت نمبر_t &dx = env.نکالو_arg ();
    pt1.move (dx, dy);
    راستہ_3::moveto (env, param, pt1);
  }

  ساکن خالی hmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    pt1.move_x (env.نکالو_arg ());
    راستہ_3::moveto (env, param, pt1);
  }

  ساکن خالی vmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    pt1.move_y (env.نکالو_arg ());
    راستہ_3::moveto (env, param, pt1);
  }

  ساکن خالی rlineto (ENV &env, PARAM& param)
  {
    برائے (غیرعلامتی صحیح i = 0; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      راستہ_3::سطر (env, param, pt1);
    }
  }

  ساکن خالی hlineto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1;
    غیرعلامتی صحیح i = 0;
    برائے (; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      راستہ_3::سطر (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      راستہ_3::سطر (env, param, pt1);
    }
    اگر (i < env.arg_ڈھیر.get_گنتی ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      راستہ_3::سطر (env, param, pt1);
    }
  }

  ساکن خالی vlineto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1;
    غیرعلامتی صحیح i = 0;
    برائے (; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      راستہ_3::سطر (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      راستہ_3::سطر (env, param, pt1);
    }
    اگر (i < env.arg_ڈھیر.get_گنتی ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      راستہ_3::سطر (env, param, pt1);
    }
  }

  ساکن خالی rrcurveto (ENV &env, PARAM& param)
  {
    برائے (غیرعلامتی صحیح i = 0; i + 6 <= env.arg_ڈھیر.get_گنتی (); i += 6)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
    }
  }

  ساکن خالی rcurveline (ENV &env, PARAM& param)
  {
    غیرعلامتی صحیح arg_گنتی = env.arg_ڈھیر.get_گنتی ();
    اگر (unlikely (arg_گنتی < 8))
      واپس;

    غیرعلامتی صحیح i = 0;
    غیرعلامتی صحیح خم_حد_بندی = arg_گنتی - 2;
    برائے (; i + 6 <= خم_حد_بندی; i += 6)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
    }

    نقطہ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    راستہ_3::سطر (env, param, pt1);
  }

  ساکن خالی rlinecurve (ENV &env, PARAM& param)
  {
    غیرعلامتی صحیح arg_گنتی = env.arg_ڈھیر.get_گنتی ();
    اگر (unlikely (arg_گنتی < 8))
      واپس;

    غیرعلامتی صحیح i = 0;
    غیرعلامتی صحیح سطر_حد_بندی = arg_گنتی - 6;
    برائے (; i + 2 <= سطر_حد_بندی; i += 2)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      راستہ_3::سطر (env, param, pt1);
    }

    نقطہ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    نقطہ_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    نقطہ_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    راستہ_3::خم (env, param, pt1, pt2, pt3);
  }

  ساکن خالی vvcurveto (ENV &env, PARAM& param)
  {
    غیرعلامتی صحیح i = 0;
    نقطہ_t pt1 = env.get_pt ();
    اگر ((env.arg_ڈھیر.get_گنتی () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    برائے (; i + 4 <= env.arg_ڈھیر.get_گنتی (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساکن خالی hhcurveto (ENV &env, PARAM& param)
  {
    غیرعلامتی صحیح i = 0;
    نقطہ_t pt1 = env.get_pt ();
    اگر ((env.arg_ڈھیر.get_گنتی () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    برائے (; i + 4 <= env.arg_ڈھیر.get_گنتی (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساکن خالی vhcurveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1, pt2, pt3;
    غیرعلامتی صحیح i = 0;
    اگر ((env.arg_ڈھیر.get_گنتی () % 8) >= 4)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      برائے (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	راستہ_3::خم (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	راستہ_3::خم (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      اگر (i < env.arg_ڈھیر.get_گنتی ())
	pt3.move_y (env.eval_arg (i));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
    }
    ورنہ
    {
      برائے (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	راستہ_3::خم (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	اگر ((env.arg_ڈھیر.get_گنتی () - i < 16) && ((env.arg_ڈھیر.get_گنتی () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	راستہ_3::خم (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساکن خالی hvcurveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1, pt2, pt3;
    غیرعلامتی صحیح i = 0;
    اگر ((env.arg_ڈھیر.get_گنتی () % 8) >= 4)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      برائے (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	راستہ_3::خم (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	راستہ_3::خم (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      اگر (i < env.arg_ڈھیر.get_گنتی ())
	pt3.move_x (env.eval_arg (i));
      راستہ_3::خم (env, param, pt1, pt2, pt3);
    }
    ورنہ
    {
      برائے (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	راستہ_3::خم (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	اگر ((env.arg_ڈھیر.get_گنتی () - i < 16) && ((env.arg_ڈھیر.get_گنتی () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	راستہ_3::خم (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساکن خالی moveto (ENV &env, PARAM& param, ثابت نقطہ_t &pt)
  { env.moveto (pt); }

  ساکن خالی سطر (ENV &env, PARAM& param, ثابت نقطہ_t &pt1)
  { راستہ_3::moveto (env, param, pt1); }

  ساکن خالی خم (ENV &env, PARAM& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  { راستہ_3::moveto (env, param, pt3); }

  ساکن خالی hflex (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_ڈھیر.get_گنتی () == 7))
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      نقطہ_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      نقطہ_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      نقطہ_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ورنہ
      env.سیٹ_error ();
  }

  ساکن خالی flex (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_ڈھیر.get_گنتی () == 13))
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطہ_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطہ_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطہ_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ورنہ
      env.سیٹ_error ();
  }

  ساکن خالی hflex1 (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_ڈھیر.get_گنتی () == 9))
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      نقطہ_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      نقطہ_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      نقطہ_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ورنہ
      env.سیٹ_error ();
  }

  ساکن خالی flex1 (ENV &env, PARAM& param)
  {
    اگر (likely (env.arg_ڈھیر.get_گنتی () == 11))
    {
      نقطہ_t d;
      برائے (غیرعلامتی صحیح i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطہ_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطہ_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطہ_t pt6 = pt5;

      اگر (fabs (d.x.مقصد_real ()) > fabs (d.y.مقصد_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      ورنہ
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    ورنہ
      env.سیٹ_error ();
  }

  محفوظ:
  ساکن خالی curve2 (ENV &env, PARAM& param,
		      ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3,
		      ثابت نقطہ_t &pt4, ثابت نقطہ_t &pt5, ثابت نقطہ_t &pt6)
  {
    راستہ_3::خم (env, param, pt1, pt2, pt3);
    راستہ_3::خم (env, param, pt4, pt5, pt6);
  }
};

سانچہ <قسم_نام ENV, قسم_نام OPSET, قسم_نام PARAM>
ساخت cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقی interpret (PARAM& param, int64_t *budget = خالی_اشاریہ)
  {
    SUPER::env.سیٹ_endchar (جھوٹ);
    SUPER::env.سیٹ_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t بقایا = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    غیرعلامتی max_ops = (غیرعلامتی) hb_clamp (بقایا, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    غیرعلامتی آغاز_ops = max_ops;

    منطقی ret = سچ;
    اگر (likely (max_ops))
      برائے (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	اگر (unlikely (SUPER::env.اندر_error () || !--max_ops))
	{
	  SUPER::env.سیٹ_error ();
	  ret = جھوٹ;
	  توڑو;
	}
	اگر (SUPER::env.is_endchar ())
	  توڑو;
      }
    ورنہ
    {
      SUPER::env.سیٹ_error ();
      ret = جھوٹ;
    }

    اگر (budget) *budget -= (int64_t) (آغاز_ops - max_ops);

    واپس ret;
  }

  نجی:
  قسمتعریف interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#اگر_ختم /* HB_CFF_INTERP_CS_COMMON_HH */
