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
#جے_بنا_تعریف HB_CFF_INTERP_CS_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_CS_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

ورتوں نطاق OT;

تعداد cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

بنية call_context_t
{
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, بنانشانی صحيح subr_عدد=0)
  {
    str_ref = substr_;
    type = type_;
    subr_عدد_2 = subr_عدد;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  بنانشانی صحيح    subr_عدد_2;
};

/* call stack */
ثابت بنانشانی صحيح k_Max_Call_حد_بندی = 10;
بنية call_ڈھیر_t : cff_ڈھیر_t<call_context_t, k_Max_Call_حد_بندی> {};

سانچا <قسم_ناں SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    بنانشانی صحيح  nSubrs = get_گنتی ();
    إذا (nSubrs < 1240)
      bias = 107;
    وإلا إذا (nSubrs < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  فراغ fini () {}

  بنانشانی صحيح get_گنتی () ثابت { أرجع subrs ? subrs->گنتی : 0; }
  بنانشانی صحيح get_bias () ثابت  { أرجع bias; }

  hb_ubytes_t عامل [] (بنانشانی صحيح اشاریہ_2) ثابت
  {
    إذا (unlikely (!subrs || اشاریہ_2 >= subrs->گنتی))
      أرجع hb_ubytes_t ();
    وإلا
      أرجع (*subrs)[اشاریہ_2];
  }

  محمي:
  بنانشانی صحيح  bias;
  ثابت SUBRS   *subrs;
};

بنية نقطہ_t
{
  فراغ سیٹ_int (صحيح _x, صحيح _y)
  {
    x.سیٹ_int (_x);
    y.سیٹ_int (_y);
  }

  فراغ move_x (ثابت نمبر_t &dx) { x += dx; }
  فراغ move_y (ثابت نمبر_t &dy) { y += dy; }
  فراغ move (ثابت نمبر_t &dx, ثابت نمبر_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت نقطہ_t &d) { move_x (d.x); move_y (d.y); }

  نمبر_t  x;
  نمبر_t  y;
};

سانچا <قسم_ناں ARG, قسم_ناں SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    تکیا_moveto = سچ;
    تکیا_hintmask = خطأ;
    hstem_گنتی = 0;
    vstem_گنتی = 0;
    hintmask_ناپ = 0;
    budget = خالی_سوچک;
    pt.سیٹ_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  منطقي وچ_error () ثابت
  {
    أرجع call_ڈھیر.وچ_error () || SUPER::وچ_error ();
  }

  منطقي نکالو_subr_عدد (ثابت biased_subrs_t<SUBRS>& biasedSubrs, بنانشانی صحيح &subr_عدد_2)
  {
    subr_عدد_2 = 0;
    صحيح n = SUPER::arg_ڈھیر.نکالو_int ();
    n += biasedSubrs.get_bias ();
    إذا (unlikely ((n < 0) || ((بنانشانی صحيح)n >= biasedSubrs.get_گنتی ())))
      أرجع خطأ;

    subr_عدد_2 = (بنانشانی صحيح)n;
    أرجع سچ;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    بنانشانی صحيح subr_عدد_2 = 0;

    إذا (unlikely (!نکالو_subr_عدد (biasedSubrs, subr_عدد_2)
		 || call_ڈھیر.get_گنتی () >= k_Max_Call_حد_بندی))
    {
      SUPER::سیٹ_error ();
      أرجع;
    }
    context.str_ref = SUPER::str_ref;
    call_ڈھیر.push (context);

    context.init ( biasedSubrs[subr_عدد_2], type, subr_عدد_2);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    إذا (unlikely (SUPER::str_ref.وچ_error ()))
      SUPER::سیٹ_error ();
    context = call_ڈھیر.نکالو ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_ناپ ()
  {
    إذا (!تکیا_hintmask)
    {
      vstem_گنتی += SUPER::arg_ڈھیر.get_گنتی() / 2;
      hintmask_ناپ = (hstem_گنتی + vstem_گنتی + 7) >> 3;
      تکیا_hintmask = سچ;
    }
  }

  فراغ سیٹ_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { أرجع endchar_flag; }

  فراغ سیٹ_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { أرجع budget; }
  منطقي spend_budget (بنانشانی صحيح لاگت, بنانشانی صحيح mult = 1)
  { أرجع !budget || hb_budget_spend (*budget, لاگت, mult); }

  ثابت نمبر_t &get_x () ثابت { أرجع pt.x; }
  ثابت نمبر_t &get_y () ثابت { أرجع pt.y; }
  ثابت نقطہ_t &get_pt () ثابت { أرجع pt; }

  فراغ moveto (ثابت نقطہ_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  تکیا_moveto;
  منطقي	  تکیا_hintmask;

  بنانشانی صحيح  hstem_گنتی;
  بنانشانی صحيح  vstem_گنتی;
  بنانشانی صحيح  hintmask_ناپ;
  int64_t	*budget;
  call_ڈھیر_t	call_ڈھیر;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خاص:
  نقطہ_t	 pt;

  تعريفنوع interp_env_t<ARG> SUPER;
};

سانچا <قسم_ناں ENV, قسم_ناں PARAM>
بنية رستہ_procs_null_t
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
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطہ_t &pt) {}
  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت نقطہ_t &pt1) {}
  ساكن فراغ ونگ (ENV &env, PARAM& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3) {}
  ساكن فراغ hflex (ENV &env, PARAM& param) {}
  ساكن فراغ flex (ENV &env, PARAM& param) {}
  ساكن فراغ hflex1 (ENV &env, PARAM& param) {}
  ساكن فراغ flex1 (ENV &env, PARAM& param) {}
};

سانچا <قسم_ناں ARG, قسم_ناں OPSET, قسم_ناں ENV, قسم_ناں PARAM, قسم_ناں رستہ_3=رستہ_procs_null_t<ENV, PARAM>>
بنية cs_opset_t : opset_t<ARG>
{
  ساكن فراغ process_op (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    بدل (op) {

      حالة Op_کوڈ_return:
	env.return_from_subr ();
	اكسر;
      حالة Op_کوڈ_endchar:
	OPSET::check_چوڑائی (op, env, param);
	env.سیٹ_endchar (سچ);
	OPSET::flush_دلیلاں_and_op (op, env, param);
	اكسر;

      حالة Op_کوڈ_fixedcs:
	env.arg_ڈھیر.ڈالو_fixed_from_substr (env.str_ref);
	اكسر;

      حالة Op_کوڈ_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	اكسر;

      حالة Op_کوڈ_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	اكسر;

      حالة Op_کوڈ_hstem:
      حالة Op_کوڈ_hstemhm:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_hstem (op, env, param);
	اكسر;
      حالة Op_کوڈ_vstem:
      حالة Op_کوڈ_vstemhm:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_vstem (op, env, param);
	اكسر;
      حالة Op_کوڈ_hintmask:
      حالة Op_کوڈ_cntrmask:
	OPSET::check_چوڑائی (op, env, param);
	OPSET::process_hintmask (op, env, param);
	اكسر;
      حالة Op_کوڈ_rmoveto:
	OPSET::check_چوڑائی (op, env, param);
	رستہ_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوڈ_hmoveto:
	OPSET::check_چوڑائی (op, env, param);
	رستہ_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوڈ_vmoveto:
	OPSET::check_چوڑائی (op, env, param);
	رستہ_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوڈ_rlineto:
	رستہ_3::rlineto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_hlineto:
	رستہ_3::hlineto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_vlineto:
	رستہ_3::vlineto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_rrcurveto:
	رستہ_3::rrcurveto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_rcurveline:
	رستہ_3::rcurveline (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_rlinecurve:
	رستہ_3::rlinecurve (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_vvcurveto:
	رستہ_3::vvcurveto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_hhcurveto:
	رستہ_3::hhcurveto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_vhcurveto:
	رستہ_3::vhcurveto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;
      حالة Op_کوڈ_hvcurveto:
	رستہ_3::hvcurveto (env, param);
	process_post_رستہ (op, env, param);
	اكسر;

      حالة Op_کوڈ_hflex:
	رستہ_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوڈ_flex:
	رستہ_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوڈ_hflex1:
	رستہ_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوڈ_flex1:
	رستہ_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	اكسر;
    }
  }

  ساكن فراغ process_hstem (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    env.hstem_گنتی += env.arg_ڈھیر.get_گنتی () / 2;
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    env.vstem_گنتی += env.arg_ڈھیر.get_گنتی () / 2;
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_ناپ ();
    إذا (likely (env.str_ref.avail (env.hintmask_ناپ)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_ناپ);
    }
  }

  ساكن فراغ process_post_flex (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن فراغ check_چوڑائی (op_کوڈ_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    إذا (!env.تکیا_moveto)
    {
      env.determine_hintmask_ناپ ();
      env.تکیا_moveto = سچ;
    }
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن فراغ process_post_رستہ (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن فراغ flush_دلیلاں_and_op (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلاں (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_دلیلاں (ENV &env, PARAM& param)
  {
    env.نکالو_n_دلیلاں (env.arg_ڈھیر.get_گنتی ());
  }

  ساكن فراغ flush_op (op_کوڈ_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_کوڈ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلاں_and_op (op, env, param);
  }

  ساكن منطقي is_نمبر_op (op_کوڈ_t op)
  {
    بدل (op)
    {
      حالة Op_کوڈ_shortint:
      حالة Op_کوڈ_fixedcs:
      حالة Op_کوڈ_Two_Byte_Pos_Int0: حالة Op_کوڈ_Two_Byte_Pos_Int1:
      حالة Op_کوڈ_Two_Byte_Pos_Int2: حالة Op_کوڈ_Two_Byte_Pos_Int3:
      حالة Op_کوڈ_Two_Byte_Neg_Int0: حالة Op_کوڈ_Two_Byte_Neg_Int1:
      حالة Op_کوڈ_Two_Byte_Neg_Int2: حالة Op_کوڈ_Two_Byte_Neg_Int3:
	أرجع سچ;

      افتراضي:
	/* 1-byte integer */
	أرجع (Op_کوڈ_One_Byte_Int_پہلا <= op) && (op <= Op_کوڈ_One_Byte_Int_آخری);
    }
  }

  محمي:
  تعريفنوع opset_t<ARG>  SUPER;
};

سانچا <قسم_ناں رستہ_3, قسم_ناں ENV, قسم_ناں PARAM>
بنية رستہ_procs_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    ثابت نمبر_t &dy = env.نکالو_arg ();
    ثابت نمبر_t &dx = env.نکالو_arg ();
    pt1.move (dx, dy);
    رستہ_3::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    pt1.move_x (env.نکالو_arg ());
    رستہ_3::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1 = env.get_pt ();
    pt1.move_y (env.نکالو_arg ());
    رستہ_3::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    لكل (بنانشانی صحيح i = 0; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      رستہ_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1;
    بنانشانی صحيح i = 0;
    لكل (; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      رستہ_3::سطر (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      رستہ_3::سطر (env, param, pt1);
    }
    إذا (i < env.arg_ڈھیر.get_گنتی ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      رستہ_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1;
    بنانشانی صحيح i = 0;
    لكل (; i + 2 <= env.arg_ڈھیر.get_گنتی (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      رستہ_3::سطر (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      رستہ_3::سطر (env, param, pt1);
    }
    إذا (i < env.arg_ڈھیر.get_گنتی ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      رستہ_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    لكل (بنانشانی صحيح i = 0; i + 6 <= env.arg_ڈھیر.get_گنتی (); i += 6)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
    }
  }

  ساكن فراغ rcurveline (ENV &env, PARAM& param)
  {
    بنانشانی صحيح arg_گنتی = env.arg_ڈھیر.get_گنتی ();
    إذا (unlikely (arg_گنتی < 8))
      أرجع;

    بنانشانی صحيح i = 0;
    بنانشانی صحيح ونگ_حد_بندی = arg_گنتی - 2;
    لكل (; i + 6 <= ونگ_حد_بندی; i += 6)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطہ_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
    }

    نقطہ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    رستہ_3::سطر (env, param, pt1);
  }

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    بنانشانی صحيح arg_گنتی = env.arg_ڈھیر.get_گنتی ();
    إذا (unlikely (arg_گنتی < 8))
      أرجع;

    بنانشانی صحيح i = 0;
    بنانشانی صحيح سطر_حد_بندی = arg_گنتی - 6;
    لكل (; i + 2 <= سطر_حد_بندی; i += 2)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      رستہ_3::سطر (env, param, pt1);
    }

    نقطہ_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    نقطہ_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    نقطہ_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    رستہ_3::ونگ (env, param, pt1, pt2, pt3);
  }

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    بنانشانی صحيح i = 0;
    نقطہ_t pt1 = env.get_pt ();
    إذا ((env.arg_ڈھیر.get_گنتی () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ڈھیر.get_گنتی (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ hhcurveto (ENV &env, PARAM& param)
  {
    بنانشانی صحيح i = 0;
    نقطہ_t pt1 = env.get_pt ();
    إذا ((env.arg_ڈھیر.get_گنتی () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ڈھیر.get_گنتی (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ vhcurveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1, pt2, pt3;
    بنانشانی صحيح i = 0;
    إذا ((env.arg_ڈھیر.get_گنتی () % 8) >= 4)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ڈھیر.get_گنتی ())
	pt3.move_y (env.eval_arg (i));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	إذا ((env.arg_ڈھیر.get_گنتی () - i < 16) && ((env.arg_ڈھیر.get_گنتی () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    نقطہ_t pt1, pt2, pt3;
    بنانشانی صحيح i = 0;
    إذا ((env.arg_ڈھیر.get_گنتی () % 8) >= 4)
    {
      نقطہ_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      نقطہ_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطہ_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ڈھیر.get_گنتی ())
	pt3.move_x (env.eval_arg (i));
      رستہ_3::ونگ (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ڈھیر.get_گنتی (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	إذا ((env.arg_ڈھیر.get_گنتی () - i < 16) && ((env.arg_ڈھیر.get_گنتی () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	رستہ_3::ونگ (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطہ_t &pt)
  { env.moveto (pt); }

  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت نقطہ_t &pt1)
  { رستہ_3::moveto (env, param, pt1); }

  ساكن فراغ ونگ (ENV &env, PARAM& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  { رستہ_3::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڈھیر.get_گنتی () == 7))
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
    وإلا
      env.سیٹ_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڈھیر.get_گنتی () == 13))
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
    وإلا
      env.سیٹ_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڈھیر.get_گنتی () == 9))
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
    وإلا
      env.سیٹ_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڈھیر.get_گنتی () == 11))
    {
      نقطہ_t d;
      لكل (بنانشانی صحيح i = 0; i < 10; i += 2)
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

      إذا (fabs (d.x.منزل_real ()) > fabs (d.y.منزل_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      وإلا
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سیٹ_error ();
  }

  محمي:
  ساكن فراغ curve2 (ENV &env, PARAM& param,
		      ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3,
		      ثابت نقطہ_t &pt4, ثابت نقطہ_t &pt5, ثابت نقطہ_t &pt6)
  {
    رستہ_3::ونگ (env, param, pt1, pt2, pt3);
    رستہ_3::ونگ (env, param, pt4, pt5, pt6);
  }
};

سانچا <قسم_ناں ENV, قسم_ناں OPSET, قسم_ناں PARAM>
بنية cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param, int64_t *budget = خالی_سوچک)
  {
    SUPER::env.سیٹ_endchar (خطأ);
    SUPER::env.سیٹ_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t بچیا = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    بنانشانی max_ops = (بنانشانی) hb_clamp (بچیا, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    بنانشانی شروع_ops = max_ops;

    منطقي ret = سچ;
    إذا (likely (max_ops))
      لكل (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	إذا (unlikely (SUPER::env.وچ_error () || !--max_ops))
	{
	  SUPER::env.سیٹ_error ();
	  ret = خطأ;
	  اكسر;
	}
	إذا (SUPER::env.is_endchar ())
	  اكسر;
      }
    وإلا
    {
      SUPER::env.سیٹ_error ();
      ret = خطأ;
    }

    إذا (budget) *budget -= (int64_t) (شروع_ops - max_ops);

    أرجع ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#جے_انت /* HB_CFF_INTERP_CS_COMMON_HH */
