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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-cff-interp-common.hh"

நாமாகாஶ CFF {

using நாமாகாஶ OT;

எண்ணிக்கை cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

கட்டமைப்பு call_context_t
{
  வெறுமை init (மாறிலி byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, unsigned முழு subr_இலக்கம்=0)
  {
    str_ref = substr_;
    type = type_;
    subr_இலக்கம்_2 = subr_இலக்கம்;
  }

  வெறுமை fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  unsigned முழு    subr_இலக்கம்_2;
};

/* call stack */
மாறிலி unsigned முழு kMaxCallLimit = 10;
கட்டமைப்பு call_அடுக்கு_t : cff_அடுக்கு_t<call_context_t, kMaxCallLimit> {};

template <typename SUBRS>
கட்டமைப்பு biased_subrs_t
{
  வெறுமை init (மாறிலி SUBRS *subrs_)
  {
    subrs = subrs_;
    unsigned முழு  nSubrs = get_மொத்தம் ();
    எனில் (nSubrs < 1240)
      bias = 107;
    இல்லையெனில் எனில் (nSubrs < 33900)
      bias = 1131;
    இல்லையெனில்
      bias = 32768;
  }

  வெறுமை fini () {}

  unsigned முழு get_மொத்தம் () மாறிலி { திரும்பு subrs ? subrs->மொத்தம் : 0; }
  unsigned முழு get_bias () மாறிலி  { திரும்பு bias; }

  hb_ubytes_t operator [] (unsigned முழு index) மாறிலி
  {
    எனில் (unlikely (!subrs || index >= subrs->மொத்தம்))
      திரும்பு hb_ubytes_t ();
    இல்லையெனில்
      திரும்பு (*subrs)[index];
  }

  பாதுகாக்கப்பட்ட:
  unsigned முழு  bias;
  மாறிலி SUBRS   *subrs;
};

கட்டமைப்பு point_t
{
  வெறுமை set_int (முழு _x, முழு _y)
  {
    x.set_int (_x);
    y.set_int (_y);
  }

  வெறுமை move_x (மாறிலி எண்_t &dx) { x += dx; }
  வெறுமை move_y (மாறிலி எண்_t &dy) { y += dy; }
  வெறுமை move (மாறிலி எண்_t &dx, மாறிலி எண்_t &dy) { move_x (dx); move_y (dy); }
  வெறுமை move (மாறிலி point_t &d) { move_x (d.x); move_y (d.y); }

  எண்_t  x;
  எண்_t  y;
};

template <typename ARG, typename SUBRS>
கட்டமைப்பு cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (மாறிலி hb_ubytes_t &str, மாறிலி SUBRS *globalSubrs_, மாறிலி SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    seen_moveto = உண்மை;
    seen_hintmask = பொய்;
    hstem_மொத்தம் = 0;
    vstem_மொத்தம் = 0;
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

  தருக்க in_error () மாறிலி
  {
    திரும்பு call_அடுக்கு.in_error () || SUPER::in_error ();
  }

  தருக்க எடு_subr_இலக்கம் (மாறிலி biased_subrs_t<SUBRS>& biasedSubrs, unsigned முழு &subr_இலக்கம்_2)
  {
    subr_இலக்கம்_2 = 0;
    முழு n = SUPER::arg_அடுக்கு.எடு_int ();
    n += biasedSubrs.get_bias ();
    எனில் (unlikely ((n < 0) || ((unsigned முழு)n >= biasedSubrs.get_மொத்தம் ())))
      திரும்பு பொய்;

    subr_இலக்கம்_2 = (unsigned முழு)n;
    திரும்பு உண்மை;
  }

  வெறுமை call_subr (மாறிலி biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    unsigned முழு subr_இலக்கம்_2 = 0;

    எனில் (unlikely (!எடு_subr_இலக்கம் (biasedSubrs, subr_இலக்கம்_2)
		 || call_அடுக்கு.get_மொத்தம் () >= kMaxCallLimit))
    {
      SUPER::set_error ();
      திரும்பு;
    }
    context.str_ref = SUPER::str_ref;
    call_அடுக்கு.push (context);

    context.init ( biasedSubrs[subr_இலக்கம்_2], type, subr_இலக்கம்_2);
    SUPER::str_ref = context.str_ref;
  }

  வெறுமை return_from_subr ()
  {
    எனில் (unlikely (SUPER::str_ref.in_error ()))
      SUPER::set_error ();
    context = call_அடுக்கு.எடு ();
    SUPER::str_ref = context.str_ref;
  }

  வெறுமை determine_hintmask_size ()
  {
    எனில் (!seen_hintmask)
    {
      vstem_மொத்தம் += SUPER::arg_அடுக்கு.get_மொத்தம்() / 2;
      hintmask_size = (hstem_மொத்தம் + vstem_மொத்தம் + 7) >> 3;
      seen_hintmask = உண்மை;
    }
  }

  வெறுமை set_endchar (தருக்க endchar_flag_) { endchar_flag = endchar_flag_; }
  தருக்க is_endchar () மாறிலி { திரும்பு endchar_flag; }

  வெறுமை set_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () மாறிலி { திரும்பு budget; }
  தருக்க spend_budget (unsigned முழு cost, unsigned முழு mult = 1)
  { திரும்பு !budget || hb_budget_spend (*budget, cost, mult); }

  மாறிலி எண்_t &get_x () மாறிலி { திரும்பு pt.x; }
  மாறிலி எண்_t &get_y () மாறிலி { திரும்பு pt.y; }
  மாறிலி point_t &get_pt () மாறிலி { திரும்பு pt; }

  வெறுமை moveto (மாறிலி point_t &pt_ ) { pt = pt_; }

  பொது:
  call_context_t   context;
  தருக்க	  endchar_flag;
  தருக்க	  seen_moveto;
  தருக்க	  seen_hintmask;

  unsigned முழு  hstem_மொத்தம்;
  unsigned முழு  vstem_மொத்தம்;
  unsigned முழு  hintmask_size;
  int64_t	*budget;
  call_அடுக்கு_t	call_அடுக்கு;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  தனிப்பட்ட:
  point_t	 pt;

  வகைவரை interp_env_t<ARG> SUPER;
};

template <typename ENV, typename PARAM>
கட்டமைப்பு path_procs_null_t
{
  நிலையான வெறுமை rmoveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை hmoveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை vmoveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை rlineto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை hlineto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை vlineto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை rrcurveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை rcurveline (ENV &env, PARAM& param) {}
  நிலையான வெறுமை rlinecurve (ENV &env, PARAM& param) {}
  நிலையான வெறுமை vvcurveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை hhcurveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை vhcurveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை hvcurveto (ENV &env, PARAM& param) {}
  நிலையான வெறுமை moveto (ENV &env, PARAM& param, மாறிலி point_t &pt) {}
  நிலையான வெறுமை வரி (ENV &env, PARAM& param, மாறிலி point_t &pt1) {}
  நிலையான வெறுமை curve (ENV &env, PARAM& param, மாறிலி point_t &pt1, மாறிலி point_t &pt2, மாறிலி point_t &pt3) {}
  நிலையான வெறுமை hflex (ENV &env, PARAM& param) {}
  நிலையான வெறுமை flex (ENV &env, PARAM& param) {}
  நிலையான வெறுமை hflex1 (ENV &env, PARAM& param) {}
  நிலையான வெறுமை flex1 (ENV &env, PARAM& param) {}
};

template <typename ARG, typename OPSET, typename ENV, typename PARAM, typename PATH=path_procs_null_t<ENV, PARAM>>
கட்டமைப்பு cs_opset_t : opset_t<ARG>
{
  நிலையான வெறுமை process_op (op_code_t op, ENV &env, PARAM& param)
  {
    தேர்வு (op) {

      நிலை OpCode_return:
	env.return_from_subr ();
	நிறுத்து;
      நிலை OpCode_endchar:
	OPSET::check_width (op, env, param);
	env.set_endchar (உண்மை);
	OPSET::flush_args_and_op (op, env, param);
	நிறுத்து;

      நிலை OpCode_fixedcs:
	env.arg_அடுக்கு.தள்ளு_fixed_from_substr (env.str_ref);
	நிறுத்து;

      நிலை OpCode_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	நிறுத்து;

      நிலை OpCode_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	நிறுத்து;

      நிலை OpCode_hstem:
      நிலை OpCode_hstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_hstem (op, env, param);
	நிறுத்து;
      நிலை OpCode_vstem:
      நிலை OpCode_vstemhm:
	OPSET::check_width (op, env, param);
	OPSET::process_vstem (op, env, param);
	நிறுத்து;
      நிலை OpCode_hintmask:
      நிலை OpCode_cntrmask:
	OPSET::check_width (op, env, param);
	OPSET::process_hintmask (op, env, param);
	நிறுத்து;
      நிலை OpCode_rmoveto:
	OPSET::check_width (op, env, param);
	PATH::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	நிறுத்து;
      நிலை OpCode_hmoveto:
	OPSET::check_width (op, env, param);
	PATH::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	நிறுத்து;
      நிலை OpCode_vmoveto:
	OPSET::check_width (op, env, param);
	PATH::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	நிறுத்து;
      நிலை OpCode_rlineto:
	PATH::rlineto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_hlineto:
	PATH::hlineto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_vlineto:
	PATH::vlineto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_rrcurveto:
	PATH::rrcurveto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_rcurveline:
	PATH::rcurveline (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_rlinecurve:
	PATH::rlinecurve (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_vvcurveto:
	PATH::vvcurveto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_hhcurveto:
	PATH::hhcurveto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_vhcurveto:
	PATH::vhcurveto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;
      நிலை OpCode_hvcurveto:
	PATH::hvcurveto (env, param);
	process_post_path (op, env, param);
	நிறுத்து;

      நிலை OpCode_hflex:
	PATH::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	நிறுத்து;

      நிலை OpCode_flex:
	PATH::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	நிறுத்து;

      நிலை OpCode_hflex1:
	PATH::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	நிறுத்து;

      நிலை OpCode_flex1:
	PATH::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env);
	நிறுத்து;
    }
  }

  நிலையான வெறுமை process_hstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.hstem_மொத்தம் += env.arg_அடுக்கு.get_மொத்தம் () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான வெறுமை process_vstem (op_code_t op, ENV &env, PARAM& param)
  {
    env.vstem_மொத்தம் += env.arg_அடுக்கு.get_மொத்தம் () / 2;
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான வெறுமை process_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_size ();
    எனில் (likely (env.str_ref.avail (env.hintmask_size)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_size);
    }
  }

  நிலையான வெறுமை process_post_flex (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான வெறுமை check_width (op_code_t op, ENV &env, PARAM& param)
  {}

  நிலையான வெறுமை process_post_move (op_code_t op, ENV &env, PARAM& param)
  {
    எனில் (!env.seen_moveto)
    {
      env.determine_hintmask_size ();
      env.seen_moveto = உண்மை;
    }
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான வெறுமை process_post_path (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான வெறுமை flush_args_and_op (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args (env, param);
    OPSET::flush_op (op, env, param);
  }

  நிலையான வெறுமை flush_args (ENV &env, PARAM& param)
  {
    env.எடு_n_args (env.arg_அடுக்கு.get_மொத்தம் ());
  }

  நிலையான வெறுமை flush_op (op_code_t op, ENV &env, PARAM& param)
  {
  }

  நிலையான வெறுமை flush_hintmask (op_code_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_args_and_op (op, env, param);
  }

  நிலையான தருக்க is_எண்_op (op_code_t op)
  {
    தேர்வு (op)
    {
      நிலை OpCode_shortint:
      நிலை OpCode_fixedcs:
      நிலை OpCode_TwoBytePosInt0: நிலை OpCode_TwoBytePosInt1:
      நிலை OpCode_TwoBytePosInt2: நிலை OpCode_TwoBytePosInt3:
      நிலை OpCode_TwoByteNegInt0: நிலை OpCode_TwoByteNegInt1:
      நிலை OpCode_TwoByteNegInt2: நிலை OpCode_TwoByteNegInt3:
	திரும்பு உண்மை;

      இயல்பு:
	/* 1-byte integer */
	திரும்பு (OpCode_OneByteIntFirst <= op) && (op <= OpCode_OneByteIntLast);
    }
  }

  பாதுகாக்கப்பட்ட:
  வகைவரை opset_t<ARG>  SUPER;
};

template <typename PATH, typename ENV, typename PARAM>
கட்டமைப்பு path_procs_t
{
  நிலையான வெறுமை rmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    மாறிலி எண்_t &dy = env.எடு_arg ();
    மாறிலி எண்_t &dx = env.எடு_arg ();
    pt1.move (dx, dy);
    PATH::moveto (env, param, pt1);
  }

  நிலையான வெறுமை hmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_x (env.எடு_arg ());
    PATH::moveto (env, param, pt1);
  }

  நிலையான வெறுமை vmoveto (ENV &env, PARAM& param)
  {
    point_t pt1 = env.get_pt ();
    pt1.move_y (env.எடு_arg ());
    PATH::moveto (env, param, pt1);
  }

  நிலையான வெறுமை rlineto (ENV &env, PARAM& param)
  {
    க்கு (unsigned முழு i = 0; i + 2 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::வரி (env, param, pt1);
    }
  }

  நிலையான வெறுமை hlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned முழு i = 0;
    க்கு (; i + 2 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::வரி (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      PATH::வரி (env, param, pt1);
    }
    எனில் (i < env.arg_அடுக்கு.get_மொத்தம் ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      PATH::வரி (env, param, pt1);
    }
  }

  நிலையான வெறுமை vlineto (ENV &env, PARAM& param)
  {
    point_t pt1;
    unsigned முழு i = 0;
    க்கு (; i + 2 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::வரி (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      PATH::வரி (env, param, pt1);
    }
    எனில் (i < env.arg_அடுக்கு.get_மொத்தம் ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      PATH::வரி (env, param, pt1);
    }
  }

  நிலையான வெறுமை rrcurveto (ENV &env, PARAM& param)
  {
    க்கு (unsigned முழு i = 0; i + 6 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 6)
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

  நிலையான வெறுமை rcurveline (ENV &env, PARAM& param)
  {
    unsigned முழு arg_மொத்தம் = env.arg_அடுக்கு.get_மொத்தம் ();
    எனில் (unlikely (arg_மொத்தம் < 8))
      திரும்பு;

    unsigned முழு i = 0;
    unsigned முழு curve_limit = arg_மொத்தம் - 2;
    க்கு (; i + 6 <= curve_limit; i += 6)
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
    PATH::வரி (env, param, pt1);
  }

  நிலையான வெறுமை rlinecurve (ENV &env, PARAM& param)
  {
    unsigned முழு arg_மொத்தம் = env.arg_அடுக்கு.get_மொத்தம் ();
    எனில் (unlikely (arg_மொத்தம் < 8))
      திரும்பு;

    unsigned முழு i = 0;
    unsigned முழு வரி_limit = arg_மொத்தம் - 6;
    க்கு (; i + 2 <= வரி_limit; i += 2)
    {
      point_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      PATH::வரி (env, param, pt1);
    }

    point_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    point_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    point_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    PATH::curve (env, param, pt1, pt2, pt3);
  }

  நிலையான வெறுமை vvcurveto (ENV &env, PARAM& param)
  {
    unsigned முழு i = 0;
    point_t pt1 = env.get_pt ();
    எனில் ((env.arg_அடுக்கு.get_மொத்தம் () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    க்கு (; i + 4 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 4)
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

  நிலையான வெறுமை hhcurveto (ENV &env, PARAM& param)
  {
    unsigned முழு i = 0;
    point_t pt1 = env.get_pt ();
    எனில் ((env.arg_அடுக்கு.get_மொத்தம் () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    க்கு (; i + 4 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 4)
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

  நிலையான வெறுமை vhcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned முழு i = 0;
    எனில் ((env.arg_அடுக்கு.get_மொத்தம் () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      க்கு (; i + 8 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 8)
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
      எனில் (i < env.arg_அடுக்கு.get_மொத்தம் ())
	pt3.move_y (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    இல்லையெனில்
    {
      க்கு (; i + 8 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 8)
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
	எனில் ((env.arg_அடுக்கு.get_மொத்தம் () - i < 16) && ((env.arg_அடுக்கு.get_மொத்தம் () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  நிலையான வெறுமை hvcurveto (ENV &env, PARAM& param)
  {
    point_t pt1, pt2, pt3;
    unsigned முழு i = 0;
    எனில் ((env.arg_அடுக்கு.get_மொத்தம் () % 8) >= 4)
    {
      point_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      point_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      point_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      க்கு (; i + 8 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 8)
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
      எனில் (i < env.arg_அடுக்கு.get_மொத்தம் ())
	pt3.move_x (env.eval_arg (i));
      PATH::curve (env, param, pt1, pt2, pt3);
    }
    இல்லையெனில்
    {
      க்கு (; i + 8 <= env.arg_அடுக்கு.get_மொத்தம் (); i += 8)
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
	எனில் ((env.arg_அடுக்கு.get_மொத்தம் () - i < 16) && ((env.arg_அடுக்கு.get_மொத்தம் () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	PATH::curve (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  நிலையான வெறுமை moveto (ENV &env, PARAM& param, மாறிலி point_t &pt)
  { env.moveto (pt); }

  நிலையான வெறுமை வரி (ENV &env, PARAM& param, மாறிலி point_t &pt1)
  { PATH::moveto (env, param, pt1); }

  நிலையான வெறுமை curve (ENV &env, PARAM& param, மாறிலி point_t &pt1, மாறிலி point_t &pt2, மாறிலி point_t &pt3)
  { PATH::moveto (env, param, pt3); }

  நிலையான வெறுமை hflex (ENV &env, PARAM& param)
  {
    எனில் (likely (env.arg_அடுக்கு.get_மொத்தம் () == 7))
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
    இல்லையெனில்
      env.set_error ();
  }

  நிலையான வெறுமை flex (ENV &env, PARAM& param)
  {
    எனில் (likely (env.arg_அடுக்கு.get_மொத்தம் () == 13))
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
    இல்லையெனில்
      env.set_error ();
  }

  நிலையான வெறுமை hflex1 (ENV &env, PARAM& param)
  {
    எனில் (likely (env.arg_அடுக்கு.get_மொத்தம் () == 9))
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
    இல்லையெனில்
      env.set_error ();
  }

  நிலையான வெறுமை flex1 (ENV &env, PARAM& param)
  {
    எனில் (likely (env.arg_அடுக்கு.get_மொத்தம் () == 11))
    {
      point_t d;
      க்கு (unsigned முழு i = 0; i < 10; i += 2)
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

      எனில் (fabs (d.x.to_real ()) > fabs (d.y.to_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      இல்லையெனில்
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    இல்லையெனில்
      env.set_error ();
  }

  பாதுகாக்கப்பட்ட:
  நிலையான வெறுமை curve2 (ENV &env, PARAM& param,
		      மாறிலி point_t &pt1, மாறிலி point_t &pt2, மாறிலி point_t &pt3,
		      மாறிலி point_t &pt4, மாறிலி point_t &pt5, மாறிலி point_t &pt6)
  {
    PATH::curve (env, param, pt1, pt2, pt3);
    PATH::curve (env, param, pt4, pt5, pt6);
  }
};

template <typename ENV, typename OPSET, typename PARAM>
கட்டமைப்பு cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  தருக்க interpret (PARAM& param, int64_t *budget = nullptr)
  {
    SUPER::env.set_endchar (பொய்);
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

    தருக்க ret = உண்மை;
    எனில் (likely (max_ops))
      க்கு (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	எனில் (unlikely (SUPER::env.in_error () || !--max_ops))
	{
	  SUPER::env.set_error ();
	  ret = பொய்;
	  நிறுத்து;
	}
	எனில் (SUPER::env.is_endchar ())
	  நிறுத்து;
      }
    இல்லையெனில்
    {
      SUPER::env.set_error ();
      ret = பொய்;
    }

    எனில் (budget) *budget -= (int64_t) (start_ops - max_ops);

    திரும்பு ret;
  }

  தனிப்பட்ட:
  வகைவரை interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_CS_COMMON_HH */
