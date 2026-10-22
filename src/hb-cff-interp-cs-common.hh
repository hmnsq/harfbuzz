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
#جيڪڏهن_وصف_نه_ٿيل HB_CFF_INTERP_CS_COMMON_HH
#وصف_ڪريو HB_CFF_INTERP_CS_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

استعمال نطاق OT;

تعداد cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

بنية call_context_t
{
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, بنانشان صحيح subr_انگ=0)
  {
    str_ref = substr_;
    type = type_;
    subr_انگ_2 = subr_انگ;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  بنانشان صحيح    subr_انگ_2;
};

/* call stack */
ثابت بنانشان صحيح k_Max_Call_حد_بندي = 10;
بنية call_ڍير_t : cff_ڍير_t<call_context_t, k_Max_Call_حد_بندي> {};

سانچو <قسم_نالو SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    بنانشان صحيح  nSubrs = get_ڳڻپ ();
    إذا (nSubrs < 1240)
      bias = 107;
    وإلا إذا (nSubrs < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  فراغ fini () {}

  بنانشان صحيح get_ڳڻپ () ثابت { أرجع subrs ? subrs->ڳڻپ : 0; }
  بنانشان صحيح get_bias () ثابت  { أرجع bias; }

  hb_ubytes_t عامل [] (بنانشان صحيح اشاريو_2) ثابت
  {
    إذا (unlikely (!subrs || اشاريو_2 >= subrs->ڳڻپ))
      أرجع hb_ubytes_t ();
    وإلا
      أرجع (*subrs)[اشاريو_2];
  }

  محمي:
  بنانشان صحيح  bias;
  ثابت SUBRS   *subrs;
};

بنية نقطو_t
{
  فراغ سيٽ_int (صحيح _x, صحيح _y)
  {
    x.سيٽ_int (_x);
    y.سيٽ_int (_y);
  }

  فراغ move_x (ثابت عدد_t &dx) { x += dx; }
  فراغ move_y (ثابت عدد_t &dy) { y += dy; }
  فراغ move (ثابت عدد_t &dx, ثابت عدد_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت نقطو_t &d) { move_x (d.x); move_y (d.y); }

  عدد_t  x;
  عدد_t  y;
};

سانچو <قسم_نالو ARG, قسم_نالو SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    ڏٺل_moveto = سچ;
    ڏٺل_hintmask = خطأ;
    hstem_ڳڻپ = 0;
    vstem_ڳڻپ = 0;
    hintmask_سائيز = 0;
    budget = خالي_اشارو;
    pt.سيٽ_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  منطقي اندريون_error () ثابت
  {
    أرجع call_ڍير.اندريون_error () || SUPER::اندريون_error ();
  }

  منطقي ڪڍو_subr_انگ (ثابت biased_subrs_t<SUBRS>& biasedSubrs, بنانشان صحيح &subr_انگ_2)
  {
    subr_انگ_2 = 0;
    صحيح n = SUPER::arg_ڍير.ڪڍو_int ();
    n += biasedSubrs.get_bias ();
    إذا (unlikely ((n < 0) || ((بنانشان صحيح)n >= biasedSubrs.get_ڳڻپ ())))
      أرجع خطأ;

    subr_انگ_2 = (بنانشان صحيح)n;
    أرجع سچ;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    بنانشان صحيح subr_انگ_2 = 0;

    إذا (unlikely (!ڪڍو_subr_انگ (biasedSubrs, subr_انگ_2)
		 || call_ڍير.get_ڳڻپ () >= k_Max_Call_حد_بندي))
    {
      SUPER::سيٽ_error ();
      أرجع;
    }
    context.str_ref = SUPER::str_ref;
    call_ڍير.push (context);

    context.init ( biasedSubrs[subr_انگ_2], type, subr_انگ_2);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    إذا (unlikely (SUPER::str_ref.اندريون_error ()))
      SUPER::سيٽ_error ();
    context = call_ڍير.ڪڍو ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_سائيز ()
  {
    إذا (!ڏٺل_hintmask)
    {
      vstem_ڳڻپ += SUPER::arg_ڍير.get_ڳڻپ() / 2;
      hintmask_سائيز = (hstem_ڳڻپ + vstem_ڳڻپ + 7) >> 3;
      ڏٺل_hintmask = سچ;
    }
  }

  فراغ سيٽ_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { أرجع endchar_flag; }

  فراغ سيٽ_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { أرجع budget; }
  منطقي spend_budget (بنانشان صحيح لاڳت, بنانشان صحيح mult = 1)
  { أرجع !budget || hb_budget_spend (*budget, لاڳت, mult); }

  ثابت عدد_t &get_x () ثابت { أرجع pt.x; }
  ثابت عدد_t &get_y () ثابت { أرجع pt.y; }
  ثابت نقطو_t &get_pt () ثابت { أرجع pt; }

  فراغ moveto (ثابت نقطو_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  ڏٺل_moveto;
  منطقي	  ڏٺل_hintmask;

  بنانشان صحيح  hstem_ڳڻپ;
  بنانشان صحيح  vstem_ڳڻپ;
  بنانشان صحيح  hintmask_سائيز;
  int64_t	*budget;
  call_ڍير_t	call_ڍير;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خاص:
  نقطو_t	 pt;

  تعريفنوع interp_env_t<ARG> SUPER;
};

سانچو <قسم_نالو ENV, قسم_نالو PARAM>
بنية رستو_procs_null_t
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
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطو_t &pt) {}
  ساكن فراغ سٽ (ENV &env, PARAM& param, ثابت نقطو_t &pt1) {}
  ساكن فراغ وڪر (ENV &env, PARAM& param, ثابت نقطو_t &pt1, ثابت نقطو_t &pt2, ثابت نقطو_t &pt3) {}
  ساكن فراغ hflex (ENV &env, PARAM& param) {}
  ساكن فراغ flex (ENV &env, PARAM& param) {}
  ساكن فراغ hflex1 (ENV &env, PARAM& param) {}
  ساكن فراغ flex1 (ENV &env, PARAM& param) {}
};

سانچو <قسم_نالو ARG, قسم_نالو OPSET, قسم_نالو ENV, قسم_نالو PARAM, قسم_نالو رستو_3=رستو_procs_null_t<ENV, PARAM>>
بنية cs_opset_t : opset_t<ARG>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    بدل (op) {

      حالة Op_ڪوڊ_return:
	env.return_from_subr ();
	اكسر;
      حالة Op_ڪوڊ_endchar:
	OPSET::check_ويڪر (op, env, param);
	env.سيٽ_endchar (سچ);
	OPSET::flush_دليلون_and_op (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_fixedcs:
	env.arg_ڍير.ڌڪو_fixed_from_substr (env.str_ref);
	اكسر;

      حالة Op_ڪوڊ_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	اكسر;

      حالة Op_ڪوڊ_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	اكسر;

      حالة Op_ڪوڊ_hstem:
      حالة Op_ڪوڊ_hstemhm:
	OPSET::check_ويڪر (op, env, param);
	OPSET::process_hstem (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_vstem:
      حالة Op_ڪوڊ_vstemhm:
	OPSET::check_ويڪر (op, env, param);
	OPSET::process_vstem (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_hintmask:
      حالة Op_ڪوڊ_cntrmask:
	OPSET::check_ويڪر (op, env, param);
	OPSET::process_hintmask (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_rmoveto:
	OPSET::check_ويڪر (op, env, param);
	رستو_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_hmoveto:
	OPSET::check_ويڪر (op, env, param);
	رستو_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_vmoveto:
	OPSET::check_ويڪر (op, env, param);
	رستو_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_rlineto:
	رستو_3::rlineto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_hlineto:
	رستو_3::hlineto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_vlineto:
	رستو_3::vlineto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_rrcurveto:
	رستو_3::rrcurveto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_rcurveline:
	رستو_3::rcurveline (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_rlinecurve:
	رستو_3::rlinecurve (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_vvcurveto:
	رستو_3::vvcurveto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_hhcurveto:
	رستو_3::hhcurveto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_vhcurveto:
	رستو_3::vhcurveto (env, param);
	process_post_رستو (op, env, param);
	اكسر;
      حالة Op_ڪوڊ_hvcurveto:
	رستو_3::hvcurveto (env, param);
	process_post_رستو (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_hflex:
	رستو_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_flex:
	رستو_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_hflex1:
	رستو_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_flex1:
	رستو_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	اكسر;
    }
  }

  ساكن فراغ process_hstem (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    env.hstem_ڳڻپ += env.arg_ڍير.get_ڳڻپ () / 2;
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    env.vstem_ڳڻپ += env.arg_ڍير.get_ڳڻپ () / 2;
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_سائيز ();
    إذا (likely (env.str_ref.avail (env.hintmask_سائيز)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_سائيز);
    }
  }

  ساكن فراغ process_post_flex (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن فراغ check_ويڪر (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    إذا (!env.ڏٺل_moveto)
    {
      env.determine_hintmask_سائيز ();
      env.ڏٺل_moveto = سچ;
    }
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن فراغ process_post_رستو (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن فراغ flush_دليلون_and_op (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دليلون (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_دليلون (ENV &env, PARAM& param)
  {
    env.ڪڍو_n_دليلون (env.arg_ڍير.get_ڳڻپ ());
  }

  ساكن فراغ flush_op (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_ڪوڊ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دليلون_and_op (op, env, param);
  }

  ساكن منطقي is_عدد_op (op_ڪوڊ_t op)
  {
    بدل (op)
    {
      حالة Op_ڪوڊ_shortint:
      حالة Op_ڪوڊ_fixedcs:
      حالة Op_ڪوڊ_Two_Byte_Pos_Int0: حالة Op_ڪوڊ_Two_Byte_Pos_Int1:
      حالة Op_ڪوڊ_Two_Byte_Pos_Int2: حالة Op_ڪوڊ_Two_Byte_Pos_Int3:
      حالة Op_ڪوڊ_Two_Byte_Neg_Int0: حالة Op_ڪوڊ_Two_Byte_Neg_Int1:
      حالة Op_ڪوڊ_Two_Byte_Neg_Int2: حالة Op_ڪوڊ_Two_Byte_Neg_Int3:
	أرجع سچ;

      افتراضي:
	/* 1-byte integer */
	أرجع (Op_ڪوڊ_One_Byte_Int_پهريون <= op) && (op <= Op_ڪوڊ_One_Byte_Int_آخري);
    }
  }

  محمي:
  تعريفنوع opset_t<ARG>  SUPER;
};

سانچو <قسم_نالو رستو_3, قسم_نالو ENV, قسم_نالو PARAM>
بنية رستو_procs_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param)
  {
    نقطو_t pt1 = env.get_pt ();
    ثابت عدد_t &dy = env.ڪڍو_arg ();
    ثابت عدد_t &dx = env.ڪڍو_arg ();
    pt1.move (dx, dy);
    رستو_3::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    نقطو_t pt1 = env.get_pt ();
    pt1.move_x (env.ڪڍو_arg ());
    رستو_3::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    نقطو_t pt1 = env.get_pt ();
    pt1.move_y (env.ڪڍو_arg ());
    رستو_3::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    لكل (بنانشان صحيح i = 0; i + 2 <= env.arg_ڍير.get_ڳڻپ (); i += 2)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      رستو_3::سٽ (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    نقطو_t pt1;
    بنانشان صحيح i = 0;
    لكل (; i + 2 <= env.arg_ڍير.get_ڳڻپ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      رستو_3::سٽ (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      رستو_3::سٽ (env, param, pt1);
    }
    إذا (i < env.arg_ڍير.get_ڳڻپ ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      رستو_3::سٽ (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    نقطو_t pt1;
    بنانشان صحيح i = 0;
    لكل (; i + 2 <= env.arg_ڍير.get_ڳڻپ (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      رستو_3::سٽ (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      رستو_3::سٽ (env, param, pt1);
    }
    إذا (i < env.arg_ڍير.get_ڳڻپ ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      رستو_3::سٽ (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    لكل (بنانشان صحيح i = 0; i + 6 <= env.arg_ڍير.get_ڳڻپ (); i += 6)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطو_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
    }
  }

  ساكن فراغ rcurveline (ENV &env, PARAM& param)
  {
    بنانشان صحيح arg_ڳڻپ = env.arg_ڍير.get_ڳڻپ ();
    إذا (unlikely (arg_ڳڻپ < 8))
      أرجع;

    بنانشان صحيح i = 0;
    بنانشان صحيح وڪر_حد_بندي = arg_ڳڻپ - 2;
    لكل (; i + 6 <= وڪر_حد_بندي; i += 6)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      نقطو_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
    }

    نقطو_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    رستو_3::سٽ (env, param, pt1);
  }

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    بنانشان صحيح arg_ڳڻپ = env.arg_ڍير.get_ڳڻپ ();
    إذا (unlikely (arg_ڳڻپ < 8))
      أرجع;

    بنانشان صحيح i = 0;
    بنانشان صحيح سٽ_حد_بندي = arg_ڳڻپ - 6;
    لكل (; i + 2 <= سٽ_حد_بندي; i += 2)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      رستو_3::سٽ (env, param, pt1);
    }

    نقطو_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    نقطو_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    نقطو_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    رستو_3::وڪر (env, param, pt1, pt2, pt3);
  }

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    بنانشان صحيح i = 0;
    نقطو_t pt1 = env.get_pt ();
    إذا ((env.arg_ڍير.get_ڳڻپ () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ڍير.get_ڳڻپ (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطو_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ hhcurveto (ENV &env, PARAM& param)
  {
    بنانشان صحيح i = 0;
    نقطو_t pt1 = env.get_pt ();
    إذا ((env.arg_ڍير.get_ڳڻپ () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ڍير.get_ڳڻپ (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطو_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ vhcurveto (ENV &env, PARAM& param)
  {
    نقطو_t pt1, pt2, pt3;
    بنانشان صحيح i = 0;
    إذا ((env.arg_ڍير.get_ڳڻپ () % 8) >= 4)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطو_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ڍير.get_ڳڻپ (); i += 8)
      {
	رستو_3::وڪر (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ڍير.get_ڳڻپ ())
	pt3.move_y (env.eval_arg (i));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ڍير.get_ڳڻپ (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	إذا ((env.arg_ڍير.get_ڳڻپ () - i < 16) && ((env.arg_ڍير.get_ڳڻپ () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    نقطو_t pt1, pt2, pt3;
    بنانشان صحيح i = 0;
    إذا ((env.arg_ڍير.get_ڳڻپ () % 8) >= 4)
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      نقطو_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ڍير.get_ڳڻپ (); i += 8)
      {
	رستو_3::وڪر (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ڍير.get_ڳڻپ ())
	pt3.move_x (env.eval_arg (i));
      رستو_3::وڪر (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ڍير.get_ڳڻپ (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	إذا ((env.arg_ڍير.get_ڳڻپ () - i < 16) && ((env.arg_ڍير.get_ڳڻپ () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	رستو_3::وڪر (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت نقطو_t &pt)
  { env.moveto (pt); }

  ساكن فراغ سٽ (ENV &env, PARAM& param, ثابت نقطو_t &pt1)
  { رستو_3::moveto (env, param, pt1); }

  ساكن فراغ وڪر (ENV &env, PARAM& param, ثابت نقطو_t &pt1, ثابت نقطو_t &pt2, ثابت نقطو_t &pt3)
  { رستو_3::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڍير.get_ڳڻپ () == 7))
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      نقطو_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      نقطو_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      نقطو_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      نقطو_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سيٽ_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڍير.get_ڳڻپ () == 13))
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطو_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطو_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطو_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطو_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سيٽ_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڍير.get_ڳڻپ () == 9))
    {
      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطو_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      نقطو_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      نقطو_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      نقطو_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سيٽ_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ڍير.get_ڳڻپ () == 11))
    {
      نقطو_t d;
      لكل (بنانشان صحيح i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      نقطو_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      نقطو_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      نقطو_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      نقطو_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      نقطو_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      نقطو_t pt6 = pt5;

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
      env.سيٽ_error ();
  }

  محمي:
  ساكن فراغ curve2 (ENV &env, PARAM& param,
		      ثابت نقطو_t &pt1, ثابت نقطو_t &pt2, ثابت نقطو_t &pt3,
		      ثابت نقطو_t &pt4, ثابت نقطو_t &pt5, ثابت نقطو_t &pt6)
  {
    رستو_3::وڪر (env, param, pt1, pt2, pt3);
    رستو_3::وڪر (env, param, pt4, pt5, pt6);
  }
};

سانچو <قسم_نالو ENV, قسم_نالو OPSET, قسم_نالو PARAM>
بنية cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param, int64_t *budget = خالي_اشارو)
  {
    SUPER::env.سيٽ_endchar (خطأ);
    SUPER::env.سيٽ_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t بچيل = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    بنانشان max_ops = (بنانشان) hb_clamp (بچيل, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    بنانشان شروعات_ops = max_ops;

    منطقي ret = سچ;
    إذا (likely (max_ops))
      لكل (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	إذا (unlikely (SUPER::env.اندريون_error () || !--max_ops))
	{
	  SUPER::env.سيٽ_error ();
	  ret = خطأ;
	  اكسر;
	}
	إذا (SUPER::env.is_endchar ())
	  اكسر;
      }
    وإلا
    {
      SUPER::env.سيٽ_error ();
      ret = خطأ;
    }

    إذا (budget) *budget -= (int64_t) (شروعات_ops - max_ops);

    أرجع ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF_INTERP_CS_COMMON_HH */
