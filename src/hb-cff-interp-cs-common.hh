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
  فراغ init (ثابت byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned صحيح subr_رقم=0)
  {
    str_ref = substr_;
    type = type_;
    subr_رقم_2 = subr_رقم;
  }

  فراغ fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned صحيح    subr_رقم_2;
};

/* call stack */
ثابت unsigned صحيح kMaxCallLimit = 10;
بنية call_مكدس_t : cff_مكدس_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
بنية biased_subrs_t
{
  فراغ init (ثابت SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned صحيح  nSubrs = get_عدد ();
    إذا (nSubrs < 1240)
      bias = 107;
    وإلا إذا (nSubrs < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  فراغ fini () {}

  unsigned صحيح get_عدد () ثابت { أرجع subrs ? subrs->عدد : 0; }
  unsigned صحيح get_bias () ثابت  { أرجع bias; }

  hb_ubytes_t operator [] (unsigned صحيح index) ثابت
  {
    إذا (unlikely (!subrs || index >= subrs->عدد))
      أرجع hb_ubytes_t ();
    وإلا
      أرجع (*subrs)[index];
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

  فراغ move_x (ثابت رقم_t &dx) { x += dx; }
  فراغ move_y (ثابت رقم_t &dy) { y += dy; }
  فراغ move (ثابت رقم_t &dx, ثابت رقم_t &dy) { move_x (dx); move_y (dy); }
  فراغ move (ثابت point_t &d) { move_x (d.x); move_y (d.y); }

  رقم_t  x;
  رقم_t  y;
};

template <typename ARG, typename SUBRS>
بنية cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ثابت hb_ubytes_t &str, ثابت SUBRS *globalSubrs_, ثابت SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = true;
    seen_hintmask = خطأ;
    hstem_عدد = 0;
    vstem_عدد = 0;
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
    أرجع call_مكدس.in_error () || SUPER::in_error ();
  }

  منطقي اسحب_subr_رقم (ثابت biased_subrs_t<SUBRS>& biasedSubrs, unsigned صحيح &subr_رقم_2)
  {
    subr_رقم_2 = 0;
    صحيح n = SUPER::arg_مكدس.اسحب_int ();
    n += biasedSubrs.get_bias ();
    إذا (unlikely ((n < 0) || ((unsigned صحيح)n >= biasedSubrs.get_عدد ())))
      أرجع خطأ;

    subr_رقم_2 = (unsigned صحيح)n;
    أرجع true;
  }

  فراغ call_subr (ثابت biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned صحيح subr_رقم_2 = 0;

    إذا (unlikely (!اسحب_subr_رقم (biasedSubrs, subr_رقم_2)
		 || call_مكدس.get_عدد () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      أرجع;
    }
    context.str_ref = SUPER::str_ref;
    call_مكدس.push (context);

    context.init ( biasedSubrs[subr_رقم_2], type, subr_رقم_2);
    SUPER::str_ref = context.str_ref;
  }

  فراغ return_from_subr ()
  {
    إذا (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_مكدس.اسحب ();
    SUPER::str_ref = context.str_ref;
  }

  فراغ determine_hintmask_size ()
  {
    إذا (!seen_hintmask)
    {
      vstem_عدد += SUPER::arg_مكدس.get_عدد() / 2;
      hintmask_size = (hstem_عدد + vstem_عدد + 7) >> 3;
      seen_hintmask = true;
    }
  }

  فراغ set_endchar (منطقي endchar_flag_) { endchar_flag = endchar_flag_; }
  منطقي is_endchar () ثابت { أرجع endchar_flag; }

  فراغ set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ثابت { أرجع budget; }
  منطقي spend_budget (unsigned صحيح cost, unsigned صحيح mult = 1)
  { أرجع !budget || hb_budget_spend (*budget, cost, mult); }

  ثابت رقم_t &get_x () ثابت { أرجع pt.x; }
  ثابت رقم_t &get_y () ثابت { أرجع pt.y; }
  ثابت point_t &get_pt () ثابت { أرجع pt; }

  فراغ moveto (ثابت point_t &pt_ ) { pt = pt_; }

  عام:
  call_context_t   context;
  منطقي	  endchar_flag;
  منطقي	  seen_moveto;
  منطقي	  seen_hintmask;

  unsigned صحيح  hstem_عدد;
  unsigned صحيح  vstem_عدد;
  unsigned صحيح  hintmask_size;
  int64_t	*budget;
  call_مكدس_t	call_مكدس;
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
  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت point_t &pt1) {}
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
	env.set_endchar (true);
	OPSET::flush_وسائط_and_op (op, env, param);
	اكسر;

      حالة OpCode_fixedcs:
	env.arg_مكدس.ادفع_fixed_from_substr (env.str_ref);
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
    env.hstem_عدد += env.arg_مكدس.get_عدد () / 2;
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن فراغ process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_عدد += env.arg_مكدس.get_عدد () / 2;
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن فراغ process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    إذا (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  ساكن فراغ process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن فراغ check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  ساكن فراغ process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    إذا (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = true;
    }
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن فراغ process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن فراغ flush_وسائط_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_وسائط (env, param);
    OPSET::flush_op (op, env, param);
  }

  ساكن فراغ flush_وسائط (ENV &env, PARAM& param)
  {
    env.اسحب_n_وسائط (env.arg_مكدس.get_عدد ());
  }

  ساكن فراغ flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  ساكن فراغ flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_وسائط_and_op (op, env, param);
  }

  ساكن منطقي is_رقم_op (op_code_t op)
  {
    بدل (op)
    {
      حالة OpCode_shortint:
      حالة OpCode_fixedcs:
      حالة OpCode_TwoBytePosInt0: حالة OpCode_TwoBytePosInt1:
      حالة OpCode_TwoBytePosInt2: حالة OpCode_TwoBytePosInt3:
      حالة OpCode_TwoByteNegInt0: حالة OpCode_TwoByteNegInt1:
      حالة OpCode_TwoByteNegInt2: حالة OpCode_TwoByteNegInt3:
	أرجع true;

      افتراضي:
	/* 1-byte integer */
	أرجع (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
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
    ثابت رقم_t &dy = env.اسحب_arg ();
    ثابت رقم_t &dx = env.اسحب_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.اسحب_arg ());
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.اسحب_arg ());
    PATH::moveto (env, param, pt1);
  }

  ساكن فراغ rlineto (ENV &env, PARAM& param)
  {
    لكل (unsigned صحيح i = 0; i + 2 <= env.arg_مكدس.get_عدد (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
  }

  ساكن فراغ hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحيح i = 0;
    لكل (; i + 2 <= env.arg_مكدس.get_عدد (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
    إذا (i < env.arg_مكدس.get_عدد ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
    }
  }

  ساكن فراغ vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned صحيح i = 0;
    لكل (; i + 2 <= env.arg_مكدس.get_عدد (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::سطر (env, param, pt1);
    }
    إذا (i < env.arg_مكدس.get_عدد ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::سطر (env, param, pt1);
    }
  }

  ساكن فراغ rrcurveto (ENV &env, PARAM& param)
  {
    لكل (unsigned صحيح i = 0; i + 6 <= env.arg_مكدس.get_عدد (); i += 6)
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
    unsigned صحيح arg_عدد = env.arg_مكدس.get_عدد ();
    إذا (unlikely (arg_عدد < 8))
      أرجع;

    unsigned صحيح i = 0;
    unsigned صحيح curve_limit = arg_عدد - 2;
    لكل (; i + 6 <= curve_limit; i += 6)
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

  ساكن فراغ rlinecurve (ENV &env, PARAM& param)
  {
    unsigned صحيح arg_عدد = env.arg_مكدس.get_عدد ();
    إذا (unlikely (arg_عدد < 8))
      أرجع;

    unsigned صحيح i = 0;
    unsigned صحيح سطر_limit = arg_عدد - 6;
    لكل (; i + 2 <= سطر_limit; i += 2)
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

  ساكن فراغ vvcurveto (ENV &env, PARAM& param)
  {
    unsigned صحيح i = 0;
    point_t pt1 = env.get_pt ();
    إذا ((env.arg_مكدس.get_عدد () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_مكدس.get_عدد (); i += 4)
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
    إذا ((env.arg_مكدس.get_عدد () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    لكل (; i + 4 <= env.arg_مكدس.get_عدد (); i += 4)
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
    إذا ((env.arg_مكدس.get_عدد () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_مكدس.get_عدد (); i += 8)
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
      إذا (i < env.arg_مكدس.get_عدد ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_مكدس.get_عدد (); i += 8)
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
	إذا ((env.arg_مكدس.get_عدد () - i < 16) && ((env.arg_مكدس.get_عدد () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  ساكن فراغ hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned صحيح i = 0;
    إذا ((env.arg_مكدس.get_عدد () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      لكل (; i + 8 <= env.arg_مكدس.get_عدد (); i += 8)
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
      إذا (i < env.arg_مكدس.get_عدد ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    وإلا
    {
      لكل (; i + 8 <= env.arg_مكدس.get_عدد (); i += 8)
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
	إذا ((env.arg_مكدس.get_عدد () - i < 16) && ((env.arg_مكدس.get_عدد () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  ساكن فراغ moveto (ENV &env, PARAM& param, ثابت point_t &pt)
  { env.moveto (pt); }

  ساكن فراغ سطر (ENV &env, PARAM& param, ثابت point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  ساكن فراغ curve (ENV &env, PARAM& param, ثابت point_t &pt1, ثابت point_t &pt2, ثابت point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  ساكن فراغ hflex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_مكدس.get_عدد () == 7))
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
    وإلا
      env.set_error ();
  }

  ساكن فراغ flex (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_مكدس.get_عدد () == 13))
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
    وإلا
      env.set_error ();
  }

  ساكن فراغ hflex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_مكدس.get_عدد () == 9))
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
    وإلا
      env.set_error ();
  }

  ساكن فراغ flex1 (ENV &env, PARAM& param)
  {
    إذا (likely (env.arg_مكدس.get_عدد () == 11))
    {
      point_t d;
      لكل (unsigned صحيح i = 0; i < 10; i += 2)
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

      إذا (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
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
    SUPER::env.set_endchar (خطأ);
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

    منطقي ret = true;
    إذا (likely (max_ops))
      لكل (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	إذا (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = خطأ;
	  اكسر;
	}
	إذا (SUPER::env.is_endchar ())
	  اكسر;
      }
    وإلا
    {
      SUPER::env.set_error ();
      ret = خطأ;
    }

    إذا (budget) *budget -= (int64_t) (start_ops - max_ops);

    أرجع ret;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
