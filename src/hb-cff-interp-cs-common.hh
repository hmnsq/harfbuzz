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
#اگر_تعریف_نشده HB_CFF_INTERP_CS_COMMON_HH
#تعریف_کن HB_CFF_INTERP_CS_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

استفاده نطاق OT;

تعداد cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

بنية call_context_t
{
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, بیعلامت صحيح subr_رقم=0)
  {
    str_ref = substr_;
    type = type_;
    subr_رقم_2 = subr_رقم;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  بیعلامت صحيح    subr_رقم_2;
};

/* call stack */
ثابت بیعلامت صحيح k_Max_Call_حد = 10;
بنية call_پشته_t : cff_پشته_t<call_context_t, k_Max_Call_حد> {};

قالب <نام_نوع SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    بیعلامت صحيح  nSubrs = get_شمار ();
    إذا (nSubrs < 1240)
      bias = 107;
    وإلا إذا (nSubrs < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  فراغ fini () {}

  بیعلامت صحيح get_شمار () ثابت { أرجع subrs ? subrs->شمار : 0; }
  بیعلامت صحيح get_bias () ثابت  { أرجع bias; }

  hb_ubytes_t عملگر [] (بیعلامت صحيح شاخص_2) ثابت
  {
    إذا (unlikely (!subrs || شاخص_2 >= subrs->شمار))
      أرجع hb_ubytes_t ();
    وإلا
      أرجع (*subrs)[شاخص_2];
  }

  محمي:
  بیعلامت صحيح  bias;
  ثابت SUBRS   *subrs;
};

بنية نقطه_t
{
  فراغ مجموعه_int (صحيح _x, صحيح _y)
  {
    x.مجموعه_int (_x);
    y.مجموعه_int (_y);
  }

  فراغ move_x (ثابت عدد_t &dx) { x += dx; }
  فراغ move_y (ثابت عدد_t &dy) { y += dy; }
  فراغ move (ثابت عدد_t &dx, ثابت عدد_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت نقطه_t &d) { move_x (d.x); move_y (d.y); }

  عدد_t  x;
  عدد_t  y;
};

قالب <نام_نوع ARG, نام_نوع SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    دیده_moveto = درست;
    دیده_hintmask = خطأ;
    hstem_شمار = 0;
    vstem_شمار = 0;
    hintmask_اندازه = 0;
    budget = اشاره_تهی;
    pt.مجموعه_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  منطقي درون_error () ثابت
  {
    أرجع call_پشته.درون_error () || SUPER::درون_error ();
  }

  منطقي برداشت_subr_رقم (ثابت biased_subrs_t<SUBRS>& biasedSubrs, بیعلامت صحيح &subr_رقم_2)
  {
    subr_رقم_2 = 0;
    صحيح n = SUPER::arg_پشته.برداشت_int ();
    n += biasedSubrs.get_bias ();
    إذا (unlikely ((n < 0) || ((بیعلامت صحيح)n >= biasedSubrs.get_شمار ())))
      أرجع خطأ;

    subr_رقم_2 = (بیعلامت صحيح)n;
    أرجع درست;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    بیعلامت صحيح subr_رقم_2 = 0;

    إذا (unlikely (!برداشت_subr_رقم (biasedSubrs, subr_رقم_2)
		 || call_پشته.get_شمار () >= k_Max_Call_حد))
    {
      SUPER::مجموعه_error ();
      أرجع;
    }
    context.str_ref = SUPER::str_ref;
    call_پشته.push (context);

    context.init ( biasedSubrs[subr_رقم_2], type, subr_رقم_2);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    إذا (unlikely (SUPER::str_ref.درون_error ()))
      SUPER::مجموعه_error ();
    context = call_پشته.برداشت ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_اندازه ()
  {
    إذا (!دیده_hintmask)
    {
      vstem_شمار += SUPER::arg_پشته.get_شمار() / 2;
      hintmask_اندازه = (hstem_شمار + vstem_شمار + 7) >> 3;
      دیده_hintmask = درست;
    }
  }

  فراغ مجموعه_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { أرجع endchar_flag; }

  فراغ مجموعه_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { أرجع budget; }
  منطقي spend_budget (بیعلامت صحيح هزینه, بیعلامت صحيح mult = 1)
  { أرجع !budget || hb_budget_spend (*budget, هزینه, mult); }

  ثابت عدد_t &get_x () ثابت { أرجع pt.x; }
  ثابت عدد_t &get_y () ثابت { أرجع pt.y; }
  ثابت نقطه_t &get_pt () ثابت { أرجع pt; }

  فراغ moveto (ثابت نقطه_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  دیده_moveto;
  منطقي	  دیده_hintmask;

  بیعلامت صحيح  hstem_شمار;
  بیعلامت صحيح  vstem_شمار;
  بیعلامت صحيح  hintmask_اندازه;
  int64_t	*budget;
  call_پشته_t	call_پشته;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خاص:
  نقطه_t	 pt;

  تعريفنوع interp_env_t<ARG> SUPER;
};

قالب <نام_نوع ENV, نام_نوع PARAM>
بنية مسیر_procs_null_t
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
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطه_t &pt) {}
  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت نقطه_t &pt1) {}
  ساكن فراغ منحنی (ENV &env, PARAM& param, ثابت نقطه_t &pt1, ثابت نقطه_t &pt2, ثابت نقطه_t &pt3) {}
  ساكن فراغ hflex (ENV &env, PARAM& param) {}
  ساكن فراغ flex (ENV &env, PARAM& param) {}
  ساكن فراغ hflex1 (ENV &env, PARAM& param) {}
  ساكن فراغ flex1 (ENV &env, PARAM& param) {}
};

قالب <نام_نوع ARG, نام_نوع OPSET, نام_نوع ENV, نام_نوع PARAM, نام_نوع مسیر_3=مسیر_procs_null_t<ENV, PARAM>>
بنية cs_opset_t : opset_t<ARG>
{
  ساكن فراغ process_op (op_کد_t op, ENV &env, PARAM& param)
  {
    بدل (op) {

      حالة Op_کد_return:
	env.return_from_subr ();
	اكسر;
      حالة Op_کد_endchar:
	OPSET::check_عرض (op, env, param);
	env.مجموعه_endchar (درست);
	OPSET::flush_آرگومانها_and_op (op, env, param);
	اكسر;

      حالة Op_کد_fixedcs:
	env.arg_پشته.افزودن_fixed_from_substr (env.str_ref);
	اكسر;

      حالة Op_کد_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	اكسر;

      حالة Op_کد_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	اكسر;

      حالة Op_کد_hstem:
      حالة Op_کد_hstemhm:
	OPSET::check_عرض (op, env, param);
	OPSET::process_hstem (op, env, param);
	اكسر;
      حالة Op_کد_vstem:
      حالة Op_کد_vstemhm:
	OPSET::check_عرض (op, env, param);
	OPSET::process_vstem (op, env, param);
	اكسر;
      حالة Op_کد_hintmask:
      حالة Op_کد_cntrmask:
	OPSET::check_عرض (op, env, param);
	OPSET::process_hintmask (op, env, param);
	اكسر;
      حالة Op_کد_rmoveto:
	OPSET::check_عرض (op, env, param);
	مسیر_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کد_hmoveto:
	OPSET::check_عرض (op, env, param);
	مسیر_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کد_vmoveto:
	OPSET::check_عرض (op, env, param);
	مسیر_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کد_rlineto:
	مسیر_3::rlineto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_hlineto:
	مسیر_3::hlineto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_vlineto:
	مسیر_3::vlineto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_rrcurveto:
	مسیر_3::rrcurveto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_rcurveline:
	مسیر_3::rcurveline (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_rlinecurve:
	مسیر_3::rlinecurve (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_vvcurveto:
	مسیر_3::vvcurveto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_hhcurveto:
	مسیر_3::hhcurveto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_vhcurveto:
	مسیر_3::vhcurveto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;
      حالة Op_کد_hvcurveto:
	مسیر_3::hvcurveto (env, param);
	process_post_مسیر (op, env, param);
	اكسر;

      حالة Op_کد_hflex:
	مسیر_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کد_flex:
	مسیر_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کد_hflex1:
	مسیر_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کد_flex1:
	مسیر_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	اكسر;
    }
  }

  ساكن فراغ process_hstem (op_کد_t op, ENV &env, PARAM& param)
  {
    env.hstem_شمار += env.arg_پشته.get_شمار () / 2;
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_کد_t op, ENV &env, PARAM& param)
  {
    env.vstem_شمار += env.arg_پشته.get_شمار () / 2;
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_کد_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_اندازه ();
    إذا (likely (env.str_ref.avail (env.hintmask_اندازه)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_اندازه);
    }
  }

  ساكن فراغ process_post_flex (op_کد_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن فراغ check_عرض (op_کد_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_کد_t op, ENV &env, PARAM& param)
  {
    إذا (!env.دیده_moveto)
    {
      env.determine_hintmask_اندازه ();
      env.دیده_moveto = درست;
    }
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن فراغ process_post_مسیر (op_کد_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن فراغ flush_آرگومانها_and_op (op_کد_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_آرگومانها (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_آرگومانها (ENV &env, PARAM& param)
  {
    env.برداشت_n_آرگومانها (env.arg_پشته.get_شمار ());
  }

  ساكن فراغ flush_op (op_کد_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_کد_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_آرگومانها_and_op (op, env, param);
  }

  ساكن منطقي is_عدد_op (op_کد_t op)
  {
    بدل (op)
    {
      حالة Op_کد_shortint:
      حالة Op_کد_fixedcs:
      حالة Op_کد_Two_Byte_Pos_Int0: حالة Op_کد_Two_Byte_Pos_Int1:
      حالة Op_کد_Two_Byte_Pos_Int2: حالة Op_کد_Two_Byte_Pos_Int3:
      حالة Op_کد_Two_Byte_Neg_Int0: حالة Op_کد_Two_Byte_Neg_Int1:
      حالة Op_کد_Two_Byte_Neg_Int2: حالة Op_کد_Two_Byte_Neg_Int3:
	أرجع درست;

      افتراضي:
	/* 1-byte integer */
	أرجع (Op_کد_One_Byte_Int_اولی <= op) && (op <= Op_کد_One_Byte_Int_آخرین);
    }
  }

  محمي:
  تعريفنوع opset_t<ARG>  SUPER;
};

قالب <نام_نوع مسیر_3, نام_نوع ENV, نام_نوع PARAM>
بنية مسیر_procs_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param)
  {
    نقطه_t pt1 = env.get_pt ();
    ثابت عدد_t &dy = env.برداشت_arg ();
    ثابت عدد_t &dx = env.برداشت_arg ();
    pt1.move (dx, dy);
    مسیر_3::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    نقطه_t pt1 = env.get_pt ();
    pt1.move_x (env.برداشت_arg ());
    مسیر_3::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    نقطه_t pt1 = env.get_pt ();
    pt1.move_y (env.برداشت_arg ());
    مسیر_3::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    لكل (بیعلامت صحيح i = 0; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      مسیر_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    نقطه_t pt1;
    بیعلامت صحيح i = 0;
    لكل (; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      مسیر_3::سطر (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      مسیر_3::سطر (env, param, pt1);
    }
    إذا (i < env.arg_پشته.get_شمار ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      مسیر_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    نقطه_t pt1;
    بیعلامت صحيح i = 0;
    لكل (; i + 2 <= env.arg_پشته.get_شمار (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      مسیر_3::سطر (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      مسیر_3::سطر (env, param, pt1);
    }
    إذا (i < env.arg_پشته.get_شمار ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      مسیر_3::سطر (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    لكل (بیعلامت صحيح i = 0; i + 6 <= env.arg_پشته.get_شمار (); i += 6)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطه_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
    }
  }

  ساكن فراغ rcurveline (ENV &env, PARAM& param)
  {
    بیعلامت صحيح arg_شمار = env.arg_پشته.get_شمار ();
    إذا (unlikely (arg_شمار < 8))
      أرجع;

    بیعلامت صحيح i = 0;
    بیعلامت صحيح منحنی_حد = arg_شمار - 2;
    لكل (; i + 6 <= منحنی_حد; i += 6)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطه_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
    }

    نقطه_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    مسیر_3::سطر (env, param, pt1);
  }

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    بیعلامت صحيح arg_شمار = env.arg_پشته.get_شمار ();
    إذا (unlikely (arg_شمار < 8))
      أرجع;

    بیعلامت صحيح i = 0;
    بیعلامت صحيح سطر_حد = arg_شمار - 6;
    لكل (; i + 2 <= سطر_حد; i += 2)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      مسیر_3::سطر (env, param, pt1);
    }

    نقطه_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    نقطه_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    نقطه_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    مسیر_3::منحنی (env, param, pt1, pt2, pt3);
  }

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    بیعلامت صحيح i = 0;
    نقطه_t pt1 = env.get_pt ();
    إذا ((env.arg_پشته.get_شمار () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_پشته.get_شمار (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطه_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ hhcurveto (ENV &env, PARAM& param)
  {
    بیعلامت صحيح i = 0;
    نقطه_t pt1 = env.get_pt ();
    إذا ((env.arg_پشته.get_شمار () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_پشته.get_شمار (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطه_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ vhcurveto (ENV &env, PARAM& param)
  {
    نقطه_t pt1, pt2, pt3;
    بیعلامت صحيح i = 0;
    إذا ((env.arg_پشته.get_شمار () % 8) >= 4)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطه_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
      {
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      إذا (i < env.arg_پشته.get_شمار ())
	pt3.move_y (env.eval_arg (i));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	إذا ((env.arg_پشته.get_شمار () - i < 16) && ((env.arg_پشته.get_شمار () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    نقطه_t pt1, pt2, pt3;
    بیعلامت صحيح i = 0;
    إذا ((env.arg_پشته.get_شمار () % 8) >= 4)
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطه_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
      {
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      إذا (i < env.arg_پشته.get_شمار ())
	pt3.move_x (env.eval_arg (i));
      مسیر_3::منحنی (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_پشته.get_شمار (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	إذا ((env.arg_پشته.get_شمار () - i < 16) && ((env.arg_پشته.get_شمار () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	مسیر_3::منحنی (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطه_t &pt)
  { env.moveto (pt); }

  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت نقطه_t &pt1)
  { مسیر_3::moveto (env, param, pt1); }

  ساكن فراغ منحنی (ENV &env, PARAM& param, ثابت نقطه_t &pt1, ثابت نقطه_t &pt2, ثابت نقطه_t &pt3)
  { مسیر_3::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_پشته.get_شمار () == 7))
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      نقطه_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      نقطه_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      نقطه_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      نقطه_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.مجموعه_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_پشته.get_شمار () == 13))
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطه_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطه_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطه_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطه_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.مجموعه_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_پشته.get_شمار () == 9))
    {
      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطه_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      نقطه_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      نقطه_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      نقطه_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.مجموعه_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_پشته.get_شمار () == 11))
    {
      نقطه_t d;
      لكل (بیعلامت صحيح i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      نقطه_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطه_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطه_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطه_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطه_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطه_t pt6 = pt5;

      إذا (fabs (d.x.مقصد_real ()) > fabs (d.y.مقصد_real ()))
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
      env.مجموعه_error ();
  }

  محمي:
  ساكن فراغ curve2 (ENV &env, PARAM& param,
		      ثابت نقطه_t &pt1, ثابت نقطه_t &pt2, ثابت نقطه_t &pt3,
		      ثابت نقطه_t &pt4, ثابت نقطه_t &pt5, ثابت نقطه_t &pt6)
  {
    مسیر_3::منحنی (env, param, pt1, pt2, pt3);
    مسیر_3::منحنی (env, param, pt4, pt5, pt6);
  }
};

قالب <نام_نوع ENV, نام_نوع OPSET, نام_نوع PARAM>
بنية cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param, int64_t *budget = اشاره_تهی)
  {
    SUPER::env.مجموعه_endchar (خطأ);
    SUPER::env.مجموعه_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t مانده = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    بیعلامت max_ops = (بیعلامت) hb_clamp (مانده, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    بیعلامت شروع_ops = max_ops;

    منطقي ret = درست;
    إذا (likely (max_ops))
      لكل (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	إذا (unlikely (SUPER::env.درون_error () || !--max_ops))
	{
	  SUPER::env.مجموعه_error ();
	  ret = خطأ;
	  اكسر;
	}
	إذا (SUPER::env.is_endchar ())
	  اكسر;
      }
    وإلا
    {
      SUPER::env.مجموعه_error ();
      ret = خطأ;
    }

    إذا (budget) *budget -= (int64_t) (شروع_ops - max_ops);

    أرجع ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#پایان_اگر /* HB_CFF_INTERP_CS_COMMON_HH */
