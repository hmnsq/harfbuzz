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
#که_نه_تعریف_شوی HB_CFF_INTERP_CS_COMMON_HH
#تعریف_کړه HB_CFF_INTERP_CS_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

کارول نطاق OT;

تعداد cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

بنية call_context_t
{
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, بینښه صحيح subr_عدد=0)
  {
    str_ref = substr_;
    type = type_;
    subr_عدد_2 = subr_عدد;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  بینښه صحيح    subr_عدد_2;
};

/* call stack */
ثابت بینښه صحيح k_Max_Call_حد = 10;
بنية call_ډېری_t : cff_ډېری_t<call_context_t, k_Max_Call_حد> {};

کينډۍ <ډول_نوم SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    بینښه صحيح  nSubrs = get_شمېر ();
    إذا (nSubrs < 1240)
      bias = 107;
    وإلا إذا (nSubrs < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  فراغ fini () {}

  بینښه صحيح get_شمېر () ثابت { أرجع subrs ? subrs->شمېر : 0; }
  بینښه صحيح get_bias () ثابت  { أرجع bias; }

  hb_ubytes_t چلوونکی [] (بینښه صحيح شاخص_2) ثابت
  {
    إذا (unlikely (!subrs || شاخص_2 >= subrs->شمېر))
      أرجع hb_ubytes_t ();
    وإلا
      أرجع (*subrs)[شاخص_2];
  }

  محمي:
  بینښه صحيح  bias;
  ثابت SUBRS   *subrs;
};

بنية ټکی_t
{
  فراغ سېټ_int (صحيح _x, صحيح _y)
  {
    x.سېټ_int (_x);
    y.سېټ_int (_y);
  }

  فراغ move_x (ثابت شمېره_t &dx) { x += dx; }
  فراغ move_y (ثابت شمېره_t &dy) { y += dy; }
  فراغ move (ثابت شمېره_t &dx, ثابت شمېره_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت ټکی_t &d) { move_x (d.x); move_y (d.y); }

  شمېره_t  x;
  شمېره_t  y;
};

کينډۍ <ډول_نوم ARG, ډول_نوم SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    کتل_شوی_moveto = ریښتیا;
    کتل_شوی_hintmask = خطأ;
    hstem_شمېر = 0;
    vstem_شمېر = 0;
    hintmask_اندازه = 0;
    budget = تش_اشاره;
    pt.سېټ_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  منطقي دننه_error () ثابت
  {
    أرجع call_ډېری.دننه_error () || SUPER::دننه_error ();
  }

  منطقي راوباسه_subr_عدد (ثابت biased_subrs_t<SUBRS>& biasedSubrs, بینښه صحيح &subr_عدد_2)
  {
    subr_عدد_2 = 0;
    صحيح n = SUPER::arg_ډېری.راوباسه_int ();
    n += biasedSubrs.get_bias ();
    إذا (unlikely ((n < 0) || ((بینښه صحيح)n >= biasedSubrs.get_شمېر ())))
      أرجع خطأ;

    subr_عدد_2 = (بینښه صحيح)n;
    أرجع ریښتیا;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    بینښه صحيح subr_عدد_2 = 0;

    إذا (unlikely (!راوباسه_subr_عدد (biasedSubrs, subr_عدد_2)
		 || call_ډېری.get_شمېر () >= k_Max_Call_حد))
    {
      SUPER::سېټ_error ();
      أرجع;
    }
    context.str_ref = SUPER::str_ref;
    call_ډېری.push (context);

    context.init ( biasedSubrs[subr_عدد_2], type, subr_عدد_2);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    إذا (unlikely (SUPER::str_ref.دننه_error ()))
      SUPER::سېټ_error ();
    context = call_ډېری.راوباسه ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_اندازه ()
  {
    إذا (!کتل_شوی_hintmask)
    {
      vstem_شمېر += SUPER::arg_ډېری.get_شمېر() / 2;
      hintmask_اندازه = (hstem_شمېر + vstem_شمېر + 7) >> 3;
      کتل_شوی_hintmask = ریښتیا;
    }
  }

  فراغ سېټ_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { أرجع endchar_flag; }

  فراغ سېټ_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { أرجع budget; }
  منطقي spend_budget (بینښه صحيح لګښت, بینښه صحيح mult = 1)
  { أرجع !budget || hb_budget_spend (*budget, لګښت, mult); }

  ثابت شمېره_t &get_x () ثابت { أرجع pt.x; }
  ثابت شمېره_t &get_y () ثابت { أرجع pt.y; }
  ثابت ټکی_t &get_pt () ثابت { أرجع pt; }

  فراغ moveto (ثابت ټکی_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  کتل_شوی_moveto;
  منطقي	  کتل_شوی_hintmask;

  بینښه صحيح  hstem_شمېر;
  بینښه صحيح  vstem_شمېر;
  بینښه صحيح  hintmask_اندازه;
  int64_t	*budget;
  call_ډېری_t	call_ډېری;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  خاص:
  ټکی_t	 pt;

  تعريفنوع interp_env_t<ARG> SUPER;
};

کينډۍ <ډول_نوم ENV, ډول_نوم PARAM>
بنية لار_procs_null_t
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
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت ټکی_t &pt) {}
  ساكن فراغ کرښه (ENV &env, PARAM& param, ثابت ټکی_t &pt1) {}
  ساكن فراغ منحني (ENV &env, PARAM& param, ثابت ټکی_t &pt1, ثابت ټکی_t &pt2, ثابت ټکی_t &pt3) {}
  ساكن فراغ hflex (ENV &env, PARAM& param) {}
  ساكن فراغ flex (ENV &env, PARAM& param) {}
  ساكن فراغ hflex1 (ENV &env, PARAM& param) {}
  ساكن فراغ flex1 (ENV &env, PARAM& param) {}
};

کينډۍ <ډول_نوم ARG, ډول_نوم OPSET, ډول_نوم ENV, ډول_نوم PARAM, ډول_نوم لار_3=لار_procs_null_t<ENV, PARAM>>
بنية cs_opset_t : opset_t<ARG>
{
  ساكن فراغ process_op (op_کوډ_t op, ENV &env, PARAM& param)
  {
    بدل (op) {

      حالة Op_کوډ_return:
	env.return_from_subr ();
	اكسر;
      حالة Op_کوډ_endchar:
	OPSET::check_پلنوالی (op, env, param);
	env.سېټ_endchar (ریښتیا);
	OPSET::flush_دلیلونه_and_op (op, env, param);
	اكسر;

      حالة Op_کوډ_fixedcs:
	env.arg_ډېری.ټېل_fixed_from_substr (env.str_ref);
	اكسر;

      حالة Op_کوډ_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	اكسر;

      حالة Op_کوډ_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	اكسر;

      حالة Op_کوډ_hstem:
      حالة Op_کوډ_hstemhm:
	OPSET::check_پلنوالی (op, env, param);
	OPSET::process_hstem (op, env, param);
	اكسر;
      حالة Op_کوډ_vstem:
      حالة Op_کوډ_vstemhm:
	OPSET::check_پلنوالی (op, env, param);
	OPSET::process_vstem (op, env, param);
	اكسر;
      حالة Op_کوډ_hintmask:
      حالة Op_کوډ_cntrmask:
	OPSET::check_پلنوالی (op, env, param);
	OPSET::process_hintmask (op, env, param);
	اكسر;
      حالة Op_کوډ_rmoveto:
	OPSET::check_پلنوالی (op, env, param);
	لار_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوډ_hmoveto:
	OPSET::check_پلنوالی (op, env, param);
	لار_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوډ_vmoveto:
	OPSET::check_پلنوالی (op, env, param);
	لار_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	اكسر;
      حالة Op_کوډ_rlineto:
	لار_3::rlineto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_hlineto:
	لار_3::hlineto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_vlineto:
	لار_3::vlineto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_rrcurveto:
	لار_3::rrcurveto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_rcurveline:
	لار_3::rcurveline (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_rlinecurve:
	لار_3::rlinecurve (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_vvcurveto:
	لار_3::vvcurveto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_hhcurveto:
	لار_3::hhcurveto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_vhcurveto:
	لار_3::vhcurveto (env, param);
	process_post_لار (op, env, param);
	اكسر;
      حالة Op_کوډ_hvcurveto:
	لار_3::hvcurveto (env, param);
	process_post_لار (op, env, param);
	اكسر;

      حالة Op_کوډ_hflex:
	لار_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوډ_flex:
	لار_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوډ_hflex1:
	لار_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      حالة Op_کوډ_flex1:
	لار_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	اكسر;
    }
  }

  ساكن فراغ process_hstem (op_کوډ_t op, ENV &env, PARAM& param)
  {
    env.hstem_شمېر += env.arg_ډېری.get_شمېر () / 2;
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_کوډ_t op, ENV &env, PARAM& param)
  {
    env.vstem_شمېر += env.arg_ډېری.get_شمېر () / 2;
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_کوډ_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_اندازه ();
    إذا (likely (env.str_ref.avail (env.hintmask_اندازه)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_اندازه);
    }
  }

  ساكن فراغ process_post_flex (op_کوډ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن فراغ check_پلنوالی (op_کوډ_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_کوډ_t op, ENV &env, PARAM& param)
  {
    إذا (!env.کتل_شوی_moveto)
    {
      env.determine_hintmask_اندازه ();
      env.کتل_شوی_moveto = ریښتیا;
    }
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن فراغ process_post_لار (op_کوډ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن فراغ flush_دلیلونه_and_op (op_کوډ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلونه (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_دلیلونه (ENV &env, PARAM& param)
  {
    env.راوباسه_n_دلیلونه (env.arg_ډېری.get_شمېر ());
  }

  ساكن فراغ flush_op (op_کوډ_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_کوډ_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_دلیلونه_and_op (op, env, param);
  }

  ساكن منطقي is_شمېره_op (op_کوډ_t op)
  {
    بدل (op)
    {
      حالة Op_کوډ_shortint:
      حالة Op_کوډ_fixedcs:
      حالة Op_کوډ_Two_Byte_Pos_Int0: حالة Op_کوډ_Two_Byte_Pos_Int1:
      حالة Op_کوډ_Two_Byte_Pos_Int2: حالة Op_کوډ_Two_Byte_Pos_Int3:
      حالة Op_کوډ_Two_Byte_Neg_Int0: حالة Op_کوډ_Two_Byte_Neg_Int1:
      حالة Op_کوډ_Two_Byte_Neg_Int2: حالة Op_کوډ_Two_Byte_Neg_Int3:
	أرجع ریښتیا;

      افتراضي:
	/* 1-byte integer */
	أرجع (Op_کوډ_One_Byte_Int_لومړی <= op) && (op <= Op_کوډ_One_Byte_Int_وروستی);
    }
  }

  محمي:
  تعريفنوع opset_t<ARG>  SUPER;
};

کينډۍ <ډول_نوم لار_3, ډول_نوم ENV, ډول_نوم PARAM>
بنية لار_procs_t
{
  ساكن فراغ rmoveto (ENV &env, PARAM& param)
  {
    ټکی_t pt1 = env.get_pt ();
    ثابت شمېره_t &dy = env.راوباسه_arg ();
    ثابت شمېره_t &dx = env.راوباسه_arg ();
    pt1.move (dx, dy);
    لار_3::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    ټکی_t pt1 = env.get_pt ();
    pt1.move_x (env.راوباسه_arg ());
    لار_3::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    ټکی_t pt1 = env.get_pt ();
    pt1.move_y (env.راوباسه_arg ());
    لار_3::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    لكل (بینښه صحيح i = 0; i + 2 <= env.arg_ډېری.get_شمېر (); i += 2)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      لار_3::کرښه (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    ټکی_t pt1;
    بینښه صحيح i = 0;
    لكل (; i + 2 <= env.arg_ډېری.get_شمېر (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      لار_3::کرښه (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      لار_3::کرښه (env, param, pt1);
    }
    إذا (i < env.arg_ډېری.get_شمېر ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      لار_3::کرښه (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    ټکی_t pt1;
    بینښه صحيح i = 0;
    لكل (; i + 2 <= env.arg_ډېری.get_شمېر (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      لار_3::کرښه (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      لار_3::کرښه (env, param, pt1);
    }
    إذا (i < env.arg_ډېری.get_شمېر ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      لار_3::کرښه (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    لكل (بینښه صحيح i = 0; i + 6 <= env.arg_ډېری.get_شمېر (); i += 6)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      ټکی_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      لار_3::منحني (env, param, pt1, pt2, pt3);
    }
  }

  ساكن فراغ rcurveline (ENV &env, PARAM& param)
  {
    بینښه صحيح arg_شمېر = env.arg_ډېری.get_شمېر ();
    إذا (unlikely (arg_شمېر < 8))
      أرجع;

    بینښه صحيح i = 0;
    بینښه صحيح منحني_حد = arg_شمېر - 2;
    لكل (; i + 6 <= منحني_حد; i += 6)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      ټکی_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      لار_3::منحني (env, param, pt1, pt2, pt3);
    }

    ټکی_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    لار_3::کرښه (env, param, pt1);
  }

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    بینښه صحيح arg_شمېر = env.arg_ډېری.get_شمېر ();
    إذا (unlikely (arg_شمېر < 8))
      أرجع;

    بینښه صحيح i = 0;
    بینښه صحيح کرښه_حد = arg_شمېر - 6;
    لكل (; i + 2 <= کرښه_حد; i += 2)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      لار_3::کرښه (env, param, pt1);
    }

    ټکی_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    ټکی_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    ټکی_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    لار_3::منحني (env, param, pt1, pt2, pt3);
  }

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    بینښه صحيح i = 0;
    ټکی_t pt1 = env.get_pt ();
    إذا ((env.arg_ډېری.get_شمېر () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ډېری.get_شمېر (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ټکی_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      لار_3::منحني (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ hhcurveto (ENV &env, PARAM& param)
  {
    بینښه صحيح i = 0;
    ټکی_t pt1 = env.get_pt ();
    إذا ((env.arg_ډېری.get_شمېر () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_ډېری.get_شمېر (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ټکی_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      لار_3::منحني (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  ساكن فراغ vhcurveto (ENV &env, PARAM& param)
  {
    ټکی_t pt1, pt2, pt3;
    بینښه صحيح i = 0;
    إذا ((env.arg_ډېری.get_شمېر () % 8) >= 4)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ټکی_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ډېری.get_شمېر (); i += 8)
      {
	لار_3::منحني (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	لار_3::منحني (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ډېری.get_شمېر ())
	pt3.move_y (env.eval_arg (i));
      لار_3::منحني (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ډېری.get_شمېر (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	لار_3::منحني (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	إذا ((env.arg_ډېری.get_شمېر () - i < 16) && ((env.arg_ډېری.get_شمېر () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	لار_3::منحني (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    ټکی_t pt1, pt2, pt3;
    بینښه صحيح i = 0;
    إذا ((env.arg_ډېری.get_شمېر () % 8) >= 4)
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      ټکی_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_ډېری.get_شمېر (); i += 8)
      {
	لار_3::منحني (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	لار_3::منحني (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      إذا (i < env.arg_ډېری.get_شمېر ())
	pt3.move_x (env.eval_arg (i));
      لار_3::منحني (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_ډېری.get_شمېر (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	لار_3::منحني (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	إذا ((env.arg_ډېری.get_شمېر () - i < 16) && ((env.arg_ډېری.get_شمېر () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	لار_3::منحني (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت ټکی_t &pt)
  { env.moveto (pt); }

  ساكن فراغ کرښه (ENV &env, PARAM& param, ثابت ټکی_t &pt1)
  { لار_3::moveto (env, param, pt1); }

  ساكن فراغ منحني (ENV &env, PARAM& param, ثابت ټکی_t &pt1, ثابت ټکی_t &pt2, ثابت ټکی_t &pt3)
  { لار_3::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ډېری.get_شمېر () == 7))
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      ټکی_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      ټکی_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      ټکی_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      ټکی_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سېټ_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ډېری.get_شمېر () == 13))
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ټکی_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      ټکی_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      ټکی_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      ټکی_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سېټ_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ډېری.get_شمېر () == 9))
    {
      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ټکی_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      ټکی_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      ټکی_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      ټکی_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    وإلا
      env.سېټ_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_ډېری.get_شمېر () == 11))
    {
      ټکی_t d;
      لكل (بینښه صحيح i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      ټکی_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      ټکی_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      ټکی_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      ټکی_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      ټکی_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      ټکی_t pt6 = pt5;

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
      env.سېټ_error ();
  }

  محمي:
  ساكن فراغ curve2 (ENV &env, PARAM& param,
		      ثابت ټکی_t &pt1, ثابت ټکی_t &pt2, ثابت ټکی_t &pt3,
		      ثابت ټکی_t &pt4, ثابت ټکی_t &pt5, ثابت ټکی_t &pt6)
  {
    لار_3::منحني (env, param, pt1, pt2, pt3);
    لار_3::منحني (env, param, pt4, pt5, pt6);
  }
};

کينډۍ <ډول_نوم ENV, ډول_نوم OPSET, ډول_نوم PARAM>
بنية cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param, int64_t *budget = تش_اشاره)
  {
    SUPER::env.سېټ_endchar (خطأ);
    SUPER::env.سېټ_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t پاتې_شوی = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    بینښه max_ops = (بینښه) hb_clamp (پاتې_شوی, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    بینښه پیل_ops = max_ops;

    منطقي ret = ریښتیا;
    إذا (likely (max_ops))
      لكل (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	إذا (unlikely (SUPER::env.دننه_error () || !--max_ops))
	{
	  SUPER::env.سېټ_error ();
	  ret = خطأ;
	  اكسر;
	}
	إذا (SUPER::env.is_endchar ())
	  اكسر;
      }
    وإلا
    {
      SUPER::env.سېټ_error ();
      ret = خطأ;
    }

    إذا (budget) *budget -= (int64_t) (پیل_ops - max_ops);

    أرجع ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#د_که_پای /* HB_CFF_INTERP_CS_COMMON_HH */
