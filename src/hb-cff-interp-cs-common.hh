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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-cff-interp-common.hh"

નામાકાશ CFF {

using નામાકાશ OT;

ગણના cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

સંરચના call_context_t
{
  શૂન્ય init (અચલ byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned પૂર્ણાંક subr_અંક=0)
  {
    str_ref = substr_;
    type = type_;
    subr_અંક_2 = subr_અંક;
  }

  શૂન્ય fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned પૂર્ણાંક    subr_અંક_2;
};

/* call stack */
અચલ unsigned પૂર્ણાંક kMaxCallLimit = 10;
સંરચના call_ઢગલો_t : cff_ઢગલો_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
સંરચના biased_subrs_t
{
  શૂન્ય init (અચલ SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned પૂર્ણાંક  nSubrs = get_ગણતરી ();
    જો (nSubrs < 1240)
      bias = 107;
    નહીંતર જો (nSubrs < 33900)
      bias = 1131;
    નહીંતર
      bias = 32768;
  }

  શૂન્ય fini () {}

  unsigned પૂર્ણાંક get_ગણતરી () અચલ { પાછા subrs ? subrs->ગણતરી : 0; }
  unsigned પૂર્ણાંક get_bias () અચલ  { પાછા bias; }

  hb_ubytes_t operator [] (unsigned પૂર્ણાંક index) અચલ
  {
    જો (unlikely (!subrs || index >= subrs->ગણતરી))
      પાછા hb_ubytes_t ();
    નહીંતર
      પાછા (*subrs)[index];
  }

  સુરક્ષિત:
  unsigned પૂર્ણાંક  bias;
  અચલ SUBRS   *subrs;
};

સંરચના point_t
{
  શૂન્ય set_int (પૂર્ણાંક _x, પૂર્ણાંક _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  શૂન્ય move_x (અચલ સંખ્યા_t &dx) { x += dx; }
  શૂન્ય move_y (અચલ સંખ્યા_t &dy) { y += dy; }
  શૂન્ય move (અચલ સંખ્યા_t &dx, અચલ સંખ્યા_t &dy) { move_x (dx); move_y (dy); }
  શૂન્ય move (અચલ point_t &d) { move_x (d.x); move_y (d.y); }

  સંખ્યા_t  x;
  સંખ્યા_t  y;
};

template <typename ARG, typename SUBRS>
સંરચના cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (અચલ hb_ubytes_t &str, અચલ SUBRS *globalSubrs_, અચલ SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = સાચું;
    seen_hintmask = ખોટું;
    hstem_ગણતરી = 0;
    vstem_ગણતરી = 0;
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

  તાર્કિક in_error () અચલ
  {
    પાછા call_ઢગલો.in_error () || SUPER::in_error ();
  }

  તાર્કિક કાઢો_subr_અંક (અચલ biased_subrs_t<SUBRS>& biasedSubrs, unsigned પૂર્ણાંક &subr_અંક_2)
  {
    subr_અંક_2 = 0;
    પૂર્ણાંક n = SUPER::arg_ઢગલો.કાઢો_int ();
    n += biasedSubrs.get_bias ();
    જો (unlikely ((n < 0) || ((unsigned પૂર્ણાંક)n >= biasedSubrs.get_ગણતરી ())))
      પાછા ખોટું;

    subr_અંક_2 = (unsigned પૂર્ણાંક)n;
    પાછા સાચું;
  }

  શૂન્ય call_subr (અચલ biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned પૂર્ણાંક subr_અંક_2 = 0;

    જો (unlikely (!કાઢો_subr_અંક (biasedSubrs, subr_અંક_2)
		 || call_ઢગલો.get_ગણતરી () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      પાછા;
    }
    context.str_ref = SUPER::str_ref;
    call_ઢગલો.push (context);

    context.init ( biasedSubrs[subr_અંક_2], type, subr_અંક_2);
    SUPER::str_ref = context.str_ref;
  }

  શૂન્ય return_from_subr ()
  {
    જો (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_ઢગલો.કાઢો ();
    SUPER::str_ref = context.str_ref;
  }

  શૂન્ય determine_hintmask_size ()
  {
    જો (!seen_hintmask)
    {
      vstem_ગણતરી += SUPER::arg_ઢગલો.get_ગણતરી() / 2;
      hintmask_size = (hstem_ગણતરી + vstem_ગણતરી + 7) >> 3;
      seen_hintmask = સાચું;
    }
  }

  શૂન્ય set_endchar (તાર્કિક endchar_flag_) { endchar_flag = endchar_flag_; }
  તાર્કિક is_endchar () અચલ { પાછા endchar_flag; }

  શૂન્ય set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () અચલ { પાછા budget; }
  તાર્કિક spend_budget (unsigned પૂર્ણાંક cost, unsigned પૂર્ણાંક mult = 1)
  { પાછા !budget || hb_budget_spend (*budget, cost, mult); }

  અચલ સંખ્યા_t &get_x () અચલ { પાછા pt.x; }
  અચલ સંખ્યા_t &get_y () અચલ { પાછા pt.y; }
  અચલ point_t &get_pt () અચલ { પાછા pt; }

  શૂન્ય moveto (અચલ point_t &pt_ ) { pt = pt_; }

  જાહેર:
  call_context_t   context;
  તાર્કિક	  endchar_flag;
  તાર્કિક	  seen_moveto;
  તાર્કિક	  seen_hintmask;

  unsigned પૂર્ણાંક  hstem_ગણતરી;
  unsigned પૂર્ણાંક  vstem_ગણતરી;
  unsigned પૂર્ણાંક  hintmask_size;
  int64_t	*budget;
  call_ઢગલો_t	call_ઢગલો;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  ખાનગી:
  point_t	 pt;

  પ્રકારવ્યાખ્યા interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
સંરચના path_procs_null_t
{
  સ્થિર શૂન્ય rmoveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય hmoveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય vmoveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય rlineto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય hlineto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય vlineto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય rrcurveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય rcurveline (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય rlinecurve (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય vvcurveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય hhcurveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય vhcurveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય hvcurveto (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય moveto (ENV &env, PARAM& param, અચલ point_t &pt) {}
  સ્થિર શૂન્ય લીટી (ENV &env, PARAM& param, અચલ point_t &pt1) {}
  સ્થિર શૂન્ય curve (ENV &env, PARAM& param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3) {}
  સ્થિર શૂન્ય hflex (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય flex (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય hflex1 (ENV &env, PARAM& param) {}
  સ્થિર શૂન્ય flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
સંરચના cs_opset_t : opset_t<ARG>
{
  સ્થિર શૂન્ય process_op (op_code_t op, ENV &env, PARAM& param)
  {
    પસંદગી (op) {

      સ્થિતિ OpCode_return:
	env.return_from_subr ();
	તોડો;
      સ્થિતિ OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (સાચું);
	OPSET::flush_args_and_op (op, env, param);
	તોડો;

      સ્થિતિ OpCode_fixedcs:
	env.arg_ઢગલો.ધકેલો_fixed_from_substr (env.str_ref);
	તોડો;

      સ્થિતિ OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	તોડો;

      સ્થિતિ OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	તોડો;

      સ્થિતિ OpCode_hstem:
      સ્થિતિ OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	તોડો;
      સ્થિતિ OpCode_vstem:
      સ્થિતિ OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	તોડો;
      સ્થિતિ OpCode_hintmask:
      સ્થિતિ OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	તોડો;
      સ્થિતિ OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	તોડો;
      સ્થિતિ OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	તોડો;
      સ્થિતિ OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	તોડો;
      સ્થિતિ OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	તોડો;
      સ્થિતિ OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	તોડો;

      સ્થિતિ OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	તોડો;

      સ્થિતિ OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	તોડો;

      સ્થિતિ OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	તોડો;

      સ્થિતિ OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env);
	તોડો;
    }
  }

  સ્થિર શૂન્ય process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_ગણતરી += env.arg_ઢગલો.get_ગણતરી () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર શૂન્ય process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_ગણતરી += env.arg_ઢગલો.get_ગણતરી () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર શૂન્ય process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    જો (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  સ્થિર શૂન્ય process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર શૂન્ય check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  સ્થિર શૂન્ય process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    જો (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = સાચું;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર શૂન્ય process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર શૂન્ય flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  સ્થિર શૂન્ય flush_args (ENV &env, PARAM& param)
  {
    env.કાઢો_n_args (env.arg_ઢગલો.get_ગણતરી ());
  }

  સ્થિર શૂન્ય flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  સ્થિર શૂન્ય flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  સ્થિર તાર્કિક is_સંખ્યા_op (op_code_t op)
  {
    પસંદગી (op)
    {
      સ્થિતિ OpCode_shortint:
      સ્થિતિ OpCode_fixedcs:
      સ્થિતિ OpCode_TwoBytePosInt0: સ્થિતિ OpCode_TwoBytePosInt1:
      સ્થિતિ OpCode_TwoBytePosInt2: સ્થિતિ OpCode_TwoBytePosInt3:
      સ્થિતિ OpCode_TwoByteNegInt0: સ્થિતિ OpCode_TwoByteNegInt1:
      સ્થિતિ OpCode_TwoByteNegInt2: સ્થિતિ OpCode_TwoByteNegInt3:
	પાછા સાચું;

      મૂળભૂત:
	/* 1-byte integer */
	પાછા (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  સુરક્ષિત:
  પ્રકારવ્યાખ્યા opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
સંરચના path_procs_t
{
  સ્થિર શૂન્ય rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    અચલ સંખ્યા_t &dy = env.કાઢો_arg ();
    અચલ સંખ્યા_t &dx = env.કાઢો_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  સ્થિર શૂન્ય hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.કાઢો_arg ());
    PATH::moveto (env, param, pt1);
  }

  સ્થિર શૂન્ય vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.કાઢો_arg ());
    PATH::moveto (env, param, pt1);
  }

  સ્થિર શૂન્ય rlineto (ENV &env, PARAM& param)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i + 2 <= env.arg_ઢગલો.get_ગણતરી (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::લીટી (env, param, pt1);
    }
  }

  સ્થિર શૂન્ય hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned પૂર્ણાંક i = 0;
    માટે (; i + 2 <= env.arg_ઢગલો.get_ગણતરી (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::લીટી (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::લીટી (env, param, pt1);
    }
    જો (i < env.arg_ઢગલો.get_ગણતરી ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::લીટી (env, param, pt1);
    }
  }

  સ્થિર શૂન્ય vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned પૂર્ણાંક i = 0;
    માટે (; i + 2 <= env.arg_ઢગલો.get_ગણતરી (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::લીટી (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::લીટી (env, param, pt1);
    }
    જો (i < env.arg_ઢગલો.get_ગણતરી ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::લીટી (env, param, pt1);
    }
  }

  સ્થિર શૂન્ય rrcurveto (ENV &env, PARAM& param)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i + 6 <= env.arg_ઢગલો.get_ગણતરી (); i += 6)
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

  સ્થિર શૂન્ય rcurveline (ENV &env, PARAM& param)
  {
    unsigned પૂર્ણાંક arg_ગણતરી = env.arg_ઢગલો.get_ગણતરી ();
    જો (unlikely (arg_ગણતરી < 8))
      પાછા;

    unsigned પૂર્ણાંક i = 0;
    unsigned પૂર્ણાંક curve_limit = arg_ગણતરી - 2;
    માટે (; i + 6 <= curve_limit; i += 6)
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
    PATH::લીટી (env, param, pt1);
  }

  સ્થિર શૂન્ય rlinecurve (ENV &env, PARAM& param)
  {
    unsigned પૂર્ણાંક arg_ગણતરી = env.arg_ઢગલો.get_ગણતરી ();
    જો (unlikely (arg_ગણતરી < 8))
      પાછા;

    unsigned પૂર્ણાંક i = 0;
    unsigned પૂર્ણાંક લીટી_limit = arg_ગણતરી - 6;
    માટે (; i + 2 <= લીટી_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::લીટી (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  સ્થિર શૂન્ય vvcurveto (ENV &env, PARAM& param)
  {
    unsigned પૂર્ણાંક i = 0;
    point_t pt1 = env.get_pt ();
    જો ((env.arg_ઢગલો.get_ગણતરી () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    માટે (; i + 4 <= env.arg_ઢગલો.get_ગણતરી (); i += 4)
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

  સ્થિર શૂન્ય hhcurveto (ENV &env, PARAM& param)
  {
    unsigned પૂર્ણાંક i = 0;
    point_t pt1 = env.get_pt ();
    જો ((env.arg_ઢગલો.get_ગણતરી () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    માટે (; i + 4 <= env.arg_ઢગલો.get_ગણતરી (); i += 4)
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

  સ્થિર શૂન્ય vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned પૂર્ણાંક i = 0;
    જો ((env.arg_ઢગલો.get_ગણતરી () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      માટે (; i + 8 <= env.arg_ઢગલો.get_ગણતરી (); i += 8)
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
      જો (i < env.arg_ઢગલો.get_ગણતરી ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    નહીંતર
    {
      માટે (; i + 8 <= env.arg_ઢગલો.get_ગણતરી (); i += 8)
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
	જો ((env.arg_ઢગલો.get_ગણતરી () - i < 16) && ((env.arg_ઢગલો.get_ગણતરી () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  સ્થિર શૂન્ય hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned પૂર્ણાંક i = 0;
    જો ((env.arg_ઢગલો.get_ગણતરી () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      માટે (; i + 8 <= env.arg_ઢગલો.get_ગણતરી (); i += 8)
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
      જો (i < env.arg_ઢગલો.get_ગણતરી ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    નહીંતર
    {
      માટે (; i + 8 <= env.arg_ઢગલો.get_ગણતરી (); i += 8)
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
	જો ((env.arg_ઢગલો.get_ગણતરી () - i < 16) && ((env.arg_ઢગલો.get_ગણતરી () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  સ્થિર શૂન્ય moveto (ENV &env, PARAM& param, અચલ point_t &pt)
  { env.moveto (pt); }

  સ્થિર શૂન્ય લીટી (ENV &env, PARAM& param, અચલ point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  સ્થિર શૂન્ય curve (ENV &env, PARAM& param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  સ્થિર શૂન્ય hflex (ENV &env, PARAM& param)
  {
    જો (likely (env.arg_ઢગલો.get_ગણતરી () == 7))
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
    નહીંતર
      env.set_error ();
  }

  સ્થિર શૂન્ય flex (ENV &env, PARAM& param)
  {
    જો (likely (env.arg_ઢગલો.get_ગણતરી () == 13))
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
    નહીંતર
      env.set_error ();
  }

  સ્થિર શૂન્ય hflex1 (ENV &env, PARAM& param)
  {
    જો (likely (env.arg_ઢગલો.get_ગણતરી () == 9))
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
    નહીંતર
      env.set_error ();
  }

  સ્થિર શૂન્ય flex1 (ENV &env, PARAM& param)
  {
    જો (likely (env.arg_ઢગલો.get_ગણતરી () == 11))
    {
      point_t d;
      માટે (unsigned પૂર્ણાંક i = 0; i < 10; i += 2)
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

      જો (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      નહીંતર
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    નહીંતર
      env.set_error ();
  }

  સુરક્ષિત:
  સ્થિર શૂન્ય curve2 (ENV &env, PARAM& param,
		      અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3,
		      અચલ point_t &pt4, અચલ point_t &pt5, અચલ point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
સંરચના cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  તાર્કિક interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (ખોટું);
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

    તાર્કિક ret = સાચું;
    જો (likely (max_ops))
      માટે (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	જો (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = ખોટું;
	  તોડો;
	}
	જો (SUPER::env.is_endchar ())
	  તોડો;
      }
    નહીંતર
    {
      SUPER::env.set_error ();
      ret = ખોટું;
    }

    જો (budget) *budget -= (int64_t) (start_ops - max_ops);

    પાછા ret;
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
