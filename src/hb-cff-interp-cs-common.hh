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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-cff-interp-common.hh"

নামাকাশ CFF {

using নামাকাশ OT;

গণনা cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

সংরচনা call_context_t
{
  শূন্য init (ধ্রুব byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned পূর্ণাঙ্ক subr_অঙ্ক=0)
  {
    str_ref = substr_;
    type = type_;
    subr_অঙ্ক_2 = subr_অঙ্ক;
  }

  শূন্য fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned পূর্ণাঙ্ক    subr_অঙ্ক_2;
};

/* call stack */
ধ্রুব unsigned পূর্ণাঙ্ক kMaxCallLimit = 10;
সংরচনা call_স্তূপ_t : cff_স্তূপ_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
সংরচনা biased_subrs_t
{
  শূন্য init (ধ্রুব SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned পূর্ণাঙ্ক  nSubrs = get_গুনতি ();
    যদি (nSubrs < 1240)
      bias = 107;
    অন্যথা যদি (nSubrs < 33900)
      bias = 1131;
    অন্যথা
      bias = 32768;
  }

  শূন্য fini () {}

  unsigned পূর্ণাঙ্ক get_গুনতি () ধ্রুব { প্রত্যাগম subrs ? subrs->গুনতি : 0; }
  unsigned পূর্ণাঙ্ক get_bias () ধ্রুব  { প্রত্যাগম bias; }

  hb_ubytes_t operator [] (unsigned পূর্ণাঙ্ক index) ধ্রুব
  {
    যদি (unlikely (!subrs || index >= subrs->গুনতি))
      প্রত্যাগম hb_ubytes_t ();
    অন্যথা
      প্রত্যাগম (*subrs)[index];
  }

  রক্ষিত:
  unsigned পূর্ণাঙ্ক  bias;
  ধ্রুব SUBRS   *subrs;
};

সংরচনা point_t
{
  শূন্য set_int (পূর্ণাঙ্ক _x, পূর্ণাঙ্ক _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  শূন্য move_x (ধ্রুব সংখ্যা_t &dx) { x += dx; }
  শূন্য move_y (ধ্রুব সংখ্যা_t &dy) { y += dy; }
  শূন্য move (ধ্রুব সংখ্যা_t &dx, ধ্রুব সংখ্যা_t &dy) { move_x (dx); move_y (dy); }
  শূন্য move (ধ্রুব point_t &d) { move_x (d.x); move_y (d.y); }

  সংখ্যা_t  x;
  সংখ্যা_t  y;
};

template <typename ARG, typename SUBRS>
সংরচনা cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (ধ্রুব hb_ubytes_t &str, ধ্রুব SUBRS *globalSubrs_, ধ্রুব SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = সত্য;
    seen_hintmask = অসত্য;
    hstem_গুনতি = 0;
    vstem_গুনতি = 0;
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

  তার্কিক in_error () ধ্রুব
  {
    প্রত্যাগম call_স্তূপ.in_error () || SUPER::in_error ();
  }

  তার্কিক বের_করো_subr_অঙ্ক (ধ্রুব biased_subrs_t<SUBRS>& biasedSubrs, unsigned পূর্ণাঙ্ক &subr_অঙ্ক_2)
  {
    subr_অঙ্ক_2 = 0;
    পূর্ণাঙ্ক n = SUPER::arg_স্তূপ.বের_করো_int ();
    n += biasedSubrs.get_bias ();
    যদি (unlikely ((n < 0) || ((unsigned পূর্ণাঙ্ক)n >= biasedSubrs.get_গুনতি ())))
      প্রত্যাগম অসত্য;

    subr_অঙ্ক_2 = (unsigned পূর্ণাঙ্ক)n;
    প্রত্যাগম সত্য;
  }

  শূন্য call_subr (ধ্রুব biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned পূর্ণাঙ্ক subr_অঙ্ক_2 = 0;

    যদি (unlikely (!বের_করো_subr_অঙ্ক (biasedSubrs, subr_অঙ্ক_2)
		 || call_স্তূপ.get_গুনতি () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      প্রত্যাগম;
    }
    context.str_ref = SUPER::str_ref;
    call_স্তূপ.push (context);

    context.init ( biasedSubrs[subr_অঙ্ক_2], type, subr_অঙ্ক_2);
    SUPER::str_ref = context.str_ref;
  }

  শূন্য return_from_subr ()
  {
    যদি (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_স্তূপ.বের_করো ();
    SUPER::str_ref = context.str_ref;
  }

  শূন্য determine_hintmask_size ()
  {
    যদি (!seen_hintmask)
    {
      vstem_গুনতি += SUPER::arg_স্তূপ.get_গুনতি() / 2;
      hintmask_size = (hstem_গুনতি + vstem_গুনতি + 7) >> 3;
      seen_hintmask = সত্য;
    }
  }

  শূন্য set_endchar (তার্কিক endchar_flag_) { endchar_flag = endchar_flag_; }
  তার্কিক is_endchar () ধ্রুব { প্রত্যাগম endchar_flag; }

  শূন্য set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () ধ্রুব { প্রত্যাগম budget; }
  তার্কিক spend_budget (unsigned পূর্ণাঙ্ক cost, unsigned পূর্ণাঙ্ক mult = 1)
  { প্রত্যাগম !budget || hb_budget_spend (*budget, cost, mult); }

  ধ্রুব সংখ্যা_t &get_x () ধ্রুব { প্রত্যাগম pt.x; }
  ধ্রুব সংখ্যা_t &get_y () ধ্রুব { প্রত্যাগম pt.y; }
  ধ্রুব point_t &get_pt () ধ্রুব { প্রত্যাগম pt; }

  শূন্য moveto (ধ্রুব point_t &pt_ ) { pt = pt_; }

  সার্বজনিক:
  call_context_t   context;
  তার্কিক	  endchar_flag;
  তার্কিক	  seen_moveto;
  তার্কিক	  seen_hintmask;

  unsigned পূর্ণাঙ্ক  hstem_গুনতি;
  unsigned পূর্ণাঙ্ক  vstem_গুনতি;
  unsigned পূর্ণাঙ্ক  hintmask_size;
  int64_t	*budget;
  call_স্তূপ_t	call_স্তূপ;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  গুপ্ত:
  point_t	 pt;

  প্রকারনির্দেশ interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
সংরচনা path_procs_null_t
{
  স্থির শূন্য rmoveto (ENV &env, PARAM& param) {}
  স্থির শূন্য hmoveto (ENV &env, PARAM& param) {}
  স্থির শূন্য vmoveto (ENV &env, PARAM& param) {}
  স্থির শূন্য rlineto (ENV &env, PARAM& param) {}
  স্থির শূন্য hlineto (ENV &env, PARAM& param) {}
  স্থির শূন্য vlineto (ENV &env, PARAM& param) {}
  স্থির শূন্য rrcurveto (ENV &env, PARAM& param) {}
  স্থির শূন্য rcurveline (ENV &env, PARAM& param) {}
  স্থির শূন্য rlinecurve (ENV &env, PARAM& param) {}
  স্থির শূন্য vvcurveto (ENV &env, PARAM& param) {}
  স্থির শূন্য hhcurveto (ENV &env, PARAM& param) {}
  স্থির শূন্য vhcurveto (ENV &env, PARAM& param) {}
  স্থির শূন্য hvcurveto (ENV &env, PARAM& param) {}
  স্থির শূন্য moveto (ENV &env, PARAM& param, ধ্রুব point_t &pt) {}
  স্থির শূন্য সারি (ENV &env, PARAM& param, ধ্রুব point_t &pt1) {}
  স্থির শূন্য curve (ENV &env, PARAM& param, ধ্রুব point_t &pt1, ধ্রুব point_t &pt2, ধ্রুব point_t &pt3) {}
  স্থির শূন্য hflex (ENV &env, PARAM& param) {}
  স্থির শূন্য flex (ENV &env, PARAM& param) {}
  স্থির শূন্য hflex1 (ENV &env, PARAM& param) {}
  স্থির শূন্য flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
সংরচনা cs_opset_t : opset_t<ARG>
{
  স্থির শূন্য process_op (op_code_t op, ENV &env, PARAM& param)
  {
    বিকল্প (op) {

      পক্ষ OpCode_return:
	env.return_from_subr ();
	ভঙ্গ;
      পক্ষ OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (সত্য);
	OPSET::flush_args_and_op (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_fixedcs:
	env.arg_স্তূপ.ঠেলো_fixed_from_substr (env.str_ref);
	ভঙ্গ;

      পক্ষ OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	ভঙ্গ;

      পক্ষ OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	ভঙ্গ;

      পক্ষ OpCode_hstem:
      পক্ষ OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_vstem:
      পক্ষ OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_hintmask:
      পক্ষ OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;
      পক্ষ OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env);
	ভঙ্গ;
    }
  }

  স্থির শূন্য process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_গুনতি += env.arg_স্তূপ.get_গুনতি () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির শূন্য process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_গুনতি += env.arg_স্তূপ.get_গুনতি () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির শূন্য process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    যদি (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  স্থির শূন্য process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির শূন্য check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  স্থির শূন্য process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    যদি (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = সত্য;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির শূন্য process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির শূন্য flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  স্থির শূন্য flush_args (ENV &env, PARAM& param)
  {
    env.বের_করো_n_args (env.arg_স্তূপ.get_গুনতি ());
  }

  স্থির শূন্য flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  স্থির শূন্য flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  স্থির তার্কিক is_সংখ্যা_op (op_code_t op)
  {
    বিকল্প (op)
    {
      পক্ষ OpCode_shortint:
      পক্ষ OpCode_fixedcs:
      পক্ষ OpCode_TwoBytePosInt0: পক্ষ OpCode_TwoBytePosInt1:
      পক্ষ OpCode_TwoBytePosInt2: পক্ষ OpCode_TwoBytePosInt3:
      পক্ষ OpCode_TwoByteNegInt0: পক্ষ OpCode_TwoByteNegInt1:
      পক্ষ OpCode_TwoByteNegInt2: পক্ষ OpCode_TwoByteNegInt3:
	প্রত্যাগম সত্য;

      স্বভাব:
	/* 1-byte integer */
	প্রত্যাগম (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  রক্ষিত:
  প্রকারনির্দেশ opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
সংরচনা path_procs_t
{
  স্থির শূন্য rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    ধ্রুব সংখ্যা_t &dy = env.বের_করো_arg ();
    ধ্রুব সংখ্যা_t &dx = env.বের_করো_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  স্থির শূন্য hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.বের_করো_arg ());
    PATH::moveto (env, param, pt1);
  }

  স্থির শূন্য vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.বের_করো_arg ());
    PATH::moveto (env, param, pt1);
  }

  স্থির শূন্য rlineto (ENV &env, PARAM& param)
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i + 2 <= env.arg_স্তূপ.get_গুনতি (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::সারি (env, param, pt1);
    }
  }

  স্থির শূন্য hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned পূর্ণাঙ্ক i = 0;
    প্রতি (; i + 2 <= env.arg_স্তূপ.get_গুনতি (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::সারি (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::সারি (env, param, pt1);
    }
    যদি (i < env.arg_স্তূপ.get_গুনতি ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::সারি (env, param, pt1);
    }
  }

  স্থির শূন্য vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned পূর্ণাঙ্ক i = 0;
    প্রতি (; i + 2 <= env.arg_স্তূপ.get_গুনতি (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::সারি (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::সারি (env, param, pt1);
    }
    যদি (i < env.arg_স্তূপ.get_গুনতি ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::সারি (env, param, pt1);
    }
  }

  স্থির শূন্য rrcurveto (ENV &env, PARAM& param)
  {
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i + 6 <= env.arg_স্তূপ.get_গুনতি (); i += 6)
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

  স্থির শূন্য rcurveline (ENV &env, PARAM& param)
  {
    unsigned পূর্ণাঙ্ক arg_গুনতি = env.arg_স্তূপ.get_গুনতি ();
    যদি (unlikely (arg_গুনতি < 8))
      প্রত্যাগম;

    unsigned পূর্ণাঙ্ক i = 0;
    unsigned পূর্ণাঙ্ক curve_limit = arg_গুনতি - 2;
    প্রতি (; i + 6 <= curve_limit; i += 6)
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
    PATH::সারি (env, param, pt1);
  }

  স্থির শূন্য rlinecurve (ENV &env, PARAM& param)
  {
    unsigned পূর্ণাঙ্ক arg_গুনতি = env.arg_স্তূপ.get_গুনতি ();
    যদি (unlikely (arg_গুনতি < 8))
      প্রত্যাগম;

    unsigned পূর্ণাঙ্ক i = 0;
    unsigned পূর্ণাঙ্ক সারি_limit = arg_গুনতি - 6;
    প্রতি (; i + 2 <= সারি_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::সারি (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  স্থির শূন্য vvcurveto (ENV &env, PARAM& param)
  {
    unsigned পূর্ণাঙ্ক i = 0;
    point_t pt1 = env.get_pt ();
    যদি ((env.arg_স্তূপ.get_গুনতি () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    প্রতি (; i + 4 <= env.arg_স্তূপ.get_গুনতি (); i += 4)
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

  স্থির শূন্য hhcurveto (ENV &env, PARAM& param)
  {
    unsigned পূর্ণাঙ্ক i = 0;
    point_t pt1 = env.get_pt ();
    যদি ((env.arg_স্তূপ.get_গুনতি () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    প্রতি (; i + 4 <= env.arg_স্তূপ.get_গুনতি (); i += 4)
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

  স্থির শূন্য vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned পূর্ণাঙ্ক i = 0;
    যদি ((env.arg_স্তূপ.get_গুনতি () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      প্রতি (; i + 8 <= env.arg_স্তূপ.get_গুনতি (); i += 8)
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
      যদি (i < env.arg_স্তূপ.get_গুনতি ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    অন্যথা
    {
      প্রতি (; i + 8 <= env.arg_স্তূপ.get_গুনতি (); i += 8)
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
	যদি ((env.arg_স্তূপ.get_গুনতি () - i < 16) && ((env.arg_স্তূপ.get_গুনতি () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  স্থির শূন্য hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned পূর্ণাঙ্ক i = 0;
    যদি ((env.arg_স্তূপ.get_গুনতি () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      প্রতি (; i + 8 <= env.arg_স্তূপ.get_গুনতি (); i += 8)
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
      যদি (i < env.arg_স্তূপ.get_গুনতি ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    অন্যথা
    {
      প্রতি (; i + 8 <= env.arg_স্তূপ.get_গুনতি (); i += 8)
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
	যদি ((env.arg_স্তূপ.get_গুনতি () - i < 16) && ((env.arg_স্তূপ.get_গুনতি () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  স্থির শূন্য moveto (ENV &env, PARAM& param, ধ্রুব point_t &pt)
  { env.moveto (pt); }

  স্থির শূন্য সারি (ENV &env, PARAM& param, ধ্রুব point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  স্থির শূন্য curve (ENV &env, PARAM& param, ধ্রুব point_t &pt1, ধ্রুব point_t &pt2, ধ্রুব point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  স্থির শূন্য hflex (ENV &env, PARAM& param)
  {
    যদি (likely (env.arg_স্তূপ.get_গুনতি () == 7))
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
    অন্যথা
      env.set_error ();
  }

  স্থির শূন্য flex (ENV &env, PARAM& param)
  {
    যদি (likely (env.arg_স্তূপ.get_গুনতি () == 13))
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
    অন্যথা
      env.set_error ();
  }

  স্থির শূন্য hflex1 (ENV &env, PARAM& param)
  {
    যদি (likely (env.arg_স্তূপ.get_গুনতি () == 9))
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
    অন্যথা
      env.set_error ();
  }

  স্থির শূন্য flex1 (ENV &env, PARAM& param)
  {
    যদি (likely (env.arg_স্তূপ.get_গুনতি () == 11))
    {
      point_t d;
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < 10; i += 2)
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

      যদি (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      অন্যথা
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    অন্যথা
      env.set_error ();
  }

  রক্ষিত:
  স্থির শূন্য curve2 (ENV &env, PARAM& param,
		      ধ্রুব point_t &pt1, ধ্রুব point_t &pt2, ধ্রুব point_t &pt3,
		      ধ্রুব point_t &pt4, ধ্রুব point_t &pt5, ধ্রুব point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
সংরচনা cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  তার্কিক interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (অসত্য);
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

    তার্কিক ret = সত্য;
    যদি (likely (max_ops))
      প্রতি (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	যদি (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = অসত্য;
	  ভঙ্গ;
	}
	যদি (SUPER::env.is_endchar ())
	  ভঙ্গ;
      }
    অন্যথা
    {
      SUPER::env.set_error ();
      ret = অসত্য;
    }

    যদি (budget) *budget -= (int64_t) (start_ops - max_ops);

    প্রত্যাগম ret;
  }

  গুপ্ত:
  প্রকারনির্দেশ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
