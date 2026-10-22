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
#अगर_अघोषित HB_CFF_INTERP_CS_COMMON_HH
#घोषणा HB_CFF_INTERP_CS_COMMON_HH

#समावेश "hb.hh"
#समावेश "hb-cff-interp-common.hh"

namespace CFF {

using namespace OT;

क्रमागत cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

काष्ठा call_context_t
{
  व्योम init (स्थिर byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, अचिन्हित पूर्णांक subr_अंक=0)
  {
    str_ref = substr_;
    type = type_;
    subr_अंक_2 = subr_अंक;
  }

  व्योम fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  अचिन्हित पूर्णांक    subr_अंक_2;
};

/* call stack */
स्थिर अचिन्हित पूर्णांक kMaxCallLimit = 10;
काष्ठा call_ढेर_t : cff_ढेर_t<call_context_t, kMaxCallLimit> {};

ढाँचा <typename SUBRS>
काष्ठा biased_subrs_t
{
  व्योम init (स्थिर SUBRS *subrs_)
  {
    subrs = subrs_;
    अचिन्हित पूर्णांक  nSubrs = get_गिनती ();
    अगर (nSubrs < 1240)
      bias = 107;
    अन्यथा अगर (nSubrs < 33900)
      bias = 1131;
    अन्यथा
      bias = 32768;
  }

  व्योम fini () {}

  अचिन्हित पूर्णांक get_गिनती () स्थिर { वापस subrs ? subrs->गिनती : 0; }
  अचिन्हित पूर्णांक get_bias () स्थिर  { वापस bias; }

  hb_ubytes_t चालक [] (अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
    अगर (unlikely (!subrs || सूचकांक_2 >= subrs->गिनती))
      वापस hb_ubytes_t ();
    अन्यथा
      वापस (*subrs)[सूचकांक_2];
  }

  रक्षित:
  अचिन्हित पूर्णांक  bias;
  स्थिर SUBRS   *subrs;
};

काष्ठा point_t
{
  व्योम set_int (पूर्णांक _x, पूर्णांक _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  व्योम move_x (स्थिर संख्या_t &dx) { x += dx; }
  व्योम move_y (स्थिर संख्या_t &dy) { y += dy; }
  व्योम move (स्थिर संख्या_t &dx, स्थिर संख्या_t &dy) { move_x (dx); move_y (dy); }
  व्योम move (स्थिर point_t &d) { move_x (d.x); move_y (d.y); }

  संख्या_t  x;
  संख्या_t  y;
};

ढाँचा <typename ARG, typename SUBRS>
काष्ठा cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (स्थिर hb_ubytes_t &str, स्थिर SUBRS *globalSubrs_, स्थिर SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = true;
    seen_hintmask = false;
    hstem_गिनती = 0;
    vstem_गिनती = 0;
    hintmask_आकार = 0;
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

  bool in_error () स्थिर
  {
    वापस call_ढेर.in_error () || SUPER::in_error ();
  }

  bool निकालो_subr_अंक (स्थिर biased_subrs_t<SUBRS>& biasedSubrs, अचिन्हित पूर्णांक &subr_अंक_2)
  {
    subr_अंक_2 = 0;
    पूर्णांक n = SUPER::arg_ढेर.निकालो_int ();
    n += biasedSubrs.get_bias ();
    अगर (unlikely ((n < 0) || ((अचिन्हित पूर्णांक)n >= biasedSubrs.get_गिनती ())))
      वापस false;

    subr_अंक_2 = (अचिन्हित पूर्णांक)n;
    वापस true;
  }

  व्योम call_subr (स्थिर biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    अचिन्हित पूर्णांक subr_अंक_2 = 0;

    अगर (unlikely (!निकालो_subr_अंक (biasedSubrs, subr_अंक_2)
		 || call_ढेर.get_गिनती () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      वापस;
    }
    context.str_ref = SUPER::str_ref;
    call_ढेर.push (context);

    context.init ( biasedSubrs[subr_अंक_2], type, subr_अंक_2);
    SUPER::str_ref = context.str_ref;
  }

  व्योम return_from_subr ()
  {
    अगर (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_ढेर.निकालो ();
    SUPER::str_ref = context.str_ref;
  }

  व्योम determine_hintmask_आकार ()
  {
    अगर (!seen_hintmask)
    {
      vstem_गिनती += SUPER::arg_ढेर.get_गिनती() / 2;
      hintmask_आकार = (hstem_गिनती + vstem_गिनती + 7) >> 3;
      seen_hintmask = true;
    }
  }

  व्योम set_endchar (bool endchar_flag_) { endchar_flag = endchar_flag_; }
  bool is_endchar () स्थिर { वापस endchar_flag; }

  व्योम set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () स्थिर { वापस budget; }
  bool spend_budget (अचिन्हित पूर्णांक cost, अचिन्हित पूर्णांक mult = 1)
  { वापस !budget || hb_budget_spend (*budget, cost, mult); }

  स्थिर संख्या_t &get_x () स्थिर { वापस pt.x; }
  स्थिर संख्या_t &get_y () स्थिर { वापस pt.y; }
  स्थिर point_t &get_pt () स्थिर { वापस pt; }

  व्योम moveto (स्थिर point_t &pt_ ) { pt = pt_; }

  खुला:
  call_context_t   context;
  bool	  endchar_flag;
  bool	  seen_moveto;
  bool	  seen_hintmask;

  अचिन्हित पूर्णांक  hstem_गिनती;
  अचिन्हित पूर्णांक  vstem_गिनती;
  अचिन्हित पूर्णांक  hintmask_आकार;
  int64_t	*budget;
  call_ढेर_t	call_ढेर;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  निजी:
  point_t	 pt;

  प्रकार interp_env_t<ARG> SUPER;
};

ढाँचा <typename ENV, typename PARAM>
काष्ठा path_procs_null_t
{
  अटल व्योम rmoveto (ENV &env, PARAM& param) {}
  अटल व्योम hmoveto (ENV &env, PARAM& param) {}
  अटल व्योम vmoveto (ENV &env, PARAM& param) {}
  अटल व्योम rlineto (ENV &env, PARAM& param) {}
  अटल व्योम hlineto (ENV &env, PARAM& param) {}
  अटल व्योम vlineto (ENV &env, PARAM& param) {}
  अटल व्योम rrcurveto (ENV &env, PARAM& param) {}
  अटल व्योम rcurveline (ENV &env, PARAM& param) {}
  अटल व्योम rlinecurve (ENV &env, PARAM& param) {}
  अटल व्योम vvcurveto (ENV &env, PARAM& param) {}
  अटल व्योम hhcurveto (ENV &env, PARAM& param) {}
  अटल व्योम vhcurveto (ENV &env, PARAM& param) {}
  अटल व्योम hvcurveto (ENV &env, PARAM& param) {}
  अटल व्योम moveto (ENV &env, PARAM& param, स्थिर point_t &pt) {}
  अटल व्योम पंक्ति (ENV &env, PARAM& param, स्थिर point_t &pt1) {}
  अटल व्योम curve (ENV &env, PARAM& param, स्थिर point_t &pt1, स्थिर point_t &pt2, स्थिर point_t &pt3) {}
  अटल व्योम hflex (ENV &env, PARAM& param) {}
  अटल व्योम flex (ENV &env, PARAM& param) {}
  अटल व्योम hflex1 (ENV &env, PARAM& param) {}
  अटल व्योम flex1 (ENV &env, PARAM& param) {}
};

ढाँचा <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
काष्ठा cs_opset_t : opset_t<ARG>
{
  अटल व्योम process_op (op_code_t op, ENV &env, PARAM& param)
  {
    चयन (op) {

      हाल OpCode_return:
	env.return_from_subr ();
	अवरोध;
      हाल OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (true);
	OPSET::flush_args_and_op (op, env, param);
	अवरोध;

      हाल OpCode_fixedcs:
	env.arg_ढेर.धकेलो_fixed_from_substr (env.str_ref);
	अवरोध;

      हाल OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	अवरोध;

      हाल OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	अवरोध;

      हाल OpCode_hstem:
      हाल OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	अवरोध;
      हाल OpCode_vstem:
      हाल OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	अवरोध;
      हाल OpCode_hintmask:
      हाल OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	अवरोध;
      हाल OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	अवरोध;
      हाल OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	अवरोध;
      हाल OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	अवरोध;
      हाल OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	अवरोध;
      हाल OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	अवरोध;

      हाल OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	अवरोध;

      हाल OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	अवरोध;

      हाल OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	अवरोध;

      हाल OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	अवरोध;

      शेष:
	SUPER::process_op (op, env);
	अवरोध;
    }
  }

  अटल व्योम process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_गिनती += env.arg_ढेर.get_गिनती () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल व्योम process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_गिनती += env.arg_ढेर.get_गिनती () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल व्योम process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_आकार ();
    अगर (likely (env.str_ref.avail (env.hintmask_आकार)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_आकार);
    }
  }

  अटल व्योम process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल व्योम check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  अटल व्योम process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    अगर (!env.seen_moveto)
    {
      env.determine_hintmask_आकार ();
      env.seen_moveto = true;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल व्योम process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल व्योम flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  अटल व्योम flush_args (ENV &env, PARAM& param)
  {
    env.निकालो_n_args (env.arg_ढेर.get_गिनती ());
  }

  अटल व्योम flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  अटल व्योम flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  अटल bool is_संख्या_op (op_code_t op)
  {
    चयन (op)
    {
      हाल OpCode_shortint:
      हाल OpCode_fixedcs:
      हाल OpCode_TwoBytePosInt0: हाल OpCode_TwoBytePosInt1:
      हाल OpCode_TwoBytePosInt2: हाल OpCode_TwoBytePosInt3:
      हाल OpCode_TwoByteNegInt0: हाल OpCode_TwoByteNegInt1:
      हाल OpCode_TwoByteNegInt2: हाल OpCode_TwoByteNegInt3:
	वापस true;

      शेष:
	/* 1-byte integer */
	वापस (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  रक्षित:
  प्रकार opset_t<ARG>  SUPER;
};

ढाँचा <typename PATH, typename ENV, typename PARAM>
काष्ठा path_procs_t
{
  अटल व्योम rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    स्थिर संख्या_t &dy = env.निकालो_arg ();
    स्थिर संख्या_t &dx = env.निकालो_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  अटल व्योम hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.निकालो_arg ());
    PATH::moveto (env, param, pt1);
  }

  अटल व्योम vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.निकालो_arg ());
    PATH::moveto (env, param, pt1);
  }

  अटल व्योम rlineto (ENV &env, PARAM& param)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i + 2 <= env.arg_ढेर.get_गिनती (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::पंक्ति (env, param, pt1);
    }
  }

  अटल व्योम hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    अचिन्हित पूर्णांक i = 0;
    क्रम (; i + 2 <= env.arg_ढेर.get_गिनती (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::पंक्ति (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::पंक्ति (env, param, pt1);
    }
    अगर (i < env.arg_ढेर.get_गिनती ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::पंक्ति (env, param, pt1);
    }
  }

  अटल व्योम vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    अचिन्हित पूर्णांक i = 0;
    क्रम (; i + 2 <= env.arg_ढेर.get_गिनती (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::पंक्ति (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::पंक्ति (env, param, pt1);
    }
    अगर (i < env.arg_ढेर.get_गिनती ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::पंक्ति (env, param, pt1);
    }
  }

  अटल व्योम rrcurveto (ENV &env, PARAM& param)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i + 6 <= env.arg_ढेर.get_गिनती (); i += 6)
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

  अटल व्योम rcurveline (ENV &env, PARAM& param)
  {
    अचिन्हित पूर्णांक arg_गिनती = env.arg_ढेर.get_गिनती ();
    अगर (unlikely (arg_गिनती < 8))
      वापस;

    अचिन्हित पूर्णांक i = 0;
    अचिन्हित पूर्णांक curve_limit = arg_गिनती - 2;
    क्रम (; i + 6 <= curve_limit; i += 6)
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
    PATH::पंक्ति (env, param, pt1);
  }

  अटल व्योम rlinecurve (ENV &env, PARAM& param)
  {
    अचिन्हित पूर्णांक arg_गिनती = env.arg_ढेर.get_गिनती ();
    अगर (unlikely (arg_गिनती < 8))
      वापस;

    अचिन्हित पूर्णांक i = 0;
    अचिन्हित पूर्णांक पंक्ति_limit = arg_गिनती - 6;
    क्रम (; i + 2 <= पंक्ति_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::पंक्ति (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  अटल व्योम vvcurveto (ENV &env, PARAM& param)
  {
    अचिन्हित पूर्णांक i = 0;
    point_t pt1 = env.get_pt ();
    अगर ((env.arg_ढेर.get_गिनती () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    क्रम (; i + 4 <= env.arg_ढेर.get_गिनती (); i += 4)
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

  अटल व्योम hhcurveto (ENV &env, PARAM& param)
  {
    अचिन्हित पूर्णांक i = 0;
    point_t pt1 = env.get_pt ();
    अगर ((env.arg_ढेर.get_गिनती () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    क्रम (; i + 4 <= env.arg_ढेर.get_गिनती (); i += 4)
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

  अटल व्योम vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    अचिन्हित पूर्णांक i = 0;
    अगर ((env.arg_ढेर.get_गिनती () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      क्रम (; i + 8 <= env.arg_ढेर.get_गिनती (); i += 8)
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
      अगर (i < env.arg_ढेर.get_गिनती ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    अन्यथा
    {
      क्रम (; i + 8 <= env.arg_ढेर.get_गिनती (); i += 8)
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
	अगर ((env.arg_ढेर.get_गिनती () - i < 16) && ((env.arg_ढेर.get_गिनती () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  अटल व्योम hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    अचिन्हित पूर्णांक i = 0;
    अगर ((env.arg_ढेर.get_गिनती () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      क्रम (; i + 8 <= env.arg_ढेर.get_गिनती (); i += 8)
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
      अगर (i < env.arg_ढेर.get_गिनती ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    अन्यथा
    {
      क्रम (; i + 8 <= env.arg_ढेर.get_गिनती (); i += 8)
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
	अगर ((env.arg_ढेर.get_गिनती () - i < 16) && ((env.arg_ढेर.get_गिनती () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  अटल व्योम moveto (ENV &env, PARAM& param, स्थिर point_t &pt)
  { env.moveto (pt); }

  अटल व्योम पंक्ति (ENV &env, PARAM& param, स्थिर point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  अटल व्योम curve (ENV &env, PARAM& param, स्थिर point_t &pt1, स्थिर point_t &pt2, स्थिर point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  अटल व्योम hflex (ENV &env, PARAM& param)
  {
    अगर (likely (env.arg_ढेर.get_गिनती () == 7))
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
    अन्यथा
      env.set_error ();
  }

  अटल व्योम flex (ENV &env, PARAM& param)
  {
    अगर (likely (env.arg_ढेर.get_गिनती () == 13))
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
    अन्यथा
      env.set_error ();
  }

  अटल व्योम hflex1 (ENV &env, PARAM& param)
  {
    अगर (likely (env.arg_ढेर.get_गिनती () == 9))
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
    अन्यथा
      env.set_error ();
  }

  अटल व्योम flex1 (ENV &env, PARAM& param)
  {
    अगर (likely (env.arg_ढेर.get_गिनती () == 11))
    {
      point_t d;
      क्रम (अचिन्हित पूर्णांक i = 0; i < 10; i += 2)
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

      अगर (भ_असल (d.x.to_real ()) > भ_असल (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      अन्यथा
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    अन्यथा
      env.set_error ();
  }

  रक्षित:
  अटल व्योम curve2 (ENV &env, PARAM& param,
		      स्थिर point_t &pt1, स्थिर point_t &pt2, स्थिर point_t &pt3,
		      स्थिर point_t &pt4, स्थिर point_t &pt5, स्थिर point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

ढाँचा <typename ENV, typename OPSET, typename PARAM>
काष्ठा cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  bool interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (false);
    SUPER::env.set_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t remaining = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    अचिन्हित max_ops = (अचिन्हित) hb_clamp (remaining, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    अचिन्हित start_ops = max_ops;

    bool ret = true;
    अगर (likely (max_ops))
      क्रम (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	अगर (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = false;
	  अवरोध;
	}
	अगर (SUPER::env.is_endchar ())
	  अवरोध;
      }
    अन्यथा
    {
      SUPER::env.set_error ();
      ret = false;
    }

    अगर (budget) *budget -= (int64_t) (start_ops - max_ops);

    वापस ret;
  }

  निजी:
  प्रकार interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF_INTERP_CS_COMMON_HH */
