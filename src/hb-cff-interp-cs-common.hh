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
#నిర్వచితం_కాకపోతే HB_CFF_INTERP_CS_COMMON_HH
#నిర్వచించు HB_CFF_INTERP_CS_COMMON_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-cff-interp-common.hh"

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

గణన cs_type_t {
  CSType_CharString,
  CSType_GlobalSubr,
  CSType_LocalSubr
};

నిర్మాణం call_context_t
{
  శూన్య init (స్థిరరాశి byte_str_ref_t substr_=byte_str_ref_t (), cs_type_t type_=CSType_CharString, గుర్తులేని పూర్ణాంకం subr_అంకె=0)
  {
    str_ref = substr_;
    type = type_;
    subr_అంకె_2 = subr_అంకె;
  }

  శూన్య fini () {}

  byte_str_ref_t  str_ref;
  cs_type_t	  type;
  గుర్తులేని పూర్ణాంకం    subr_అంకె_2;
};

/* call stack */
స్థిరరాశి గుర్తులేని పూర్ణాంకం k_Max_Call_హద్దు = 10;
నిర్మాణం call_దొంతర_t : cff_దొంతర_t<call_context_t, k_Max_Call_హద్దు> {};

మూస <రకం_పేరు SUBRS>
నిర్మాణం biased_subrs_t
{
  శూన్య init (స్థిరరాశి SUBRS *subrs_)
  {
    subrs = subrs_;
    గుర్తులేని పూర్ణాంకం  nSubrs = get_లెక్క ();
    ఒకవేళ (nSubrs < 1240)
      bias = 107;
    లేకపోతే ఒకవేళ (nSubrs < 33900)
      bias = 1131;
    లేకపోతే
      bias = 32768;
  }

  శూన్య fini () {}

  గుర్తులేని పూర్ణాంకం get_లెక్క () స్థిరరాశి { తిరిగి subrs ? subrs->లెక్క : 0; }
  గుర్తులేని పూర్ణాంకం get_bias () స్థిరరాశి  { తిరిగి bias; }

  hb_ubytes_t క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం సూచీ_2) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!subrs || సూచీ_2 >= subrs->లెక్క))
      తిరిగి hb_ubytes_t ();
    లేకపోతే
      తిరిగి (*subrs)[సూచీ_2];
  }

  రక్షిత:
  గుర్తులేని పూర్ణాంకం  bias;
  స్థిరరాశి SUBRS   *subrs;
};

నిర్మాణం బిందువు_t
{
  శూన్య సమితి_int (పూర్ణాంకం _x, పూర్ణాంకం _y)
  {
    x.సమితి_int (_x);
    y.సమితి_int (_y);
  }

  శూన్య move_x (స్థిరరాశి సంఖ్య_t &dx) { x += dx; }
  శూన్య move_y (స్థిరరాశి సంఖ్య_t &dy) { y += dy; }
  శూన్య move (స్థిరరాశి సంఖ్య_t &dx, స్థిరరాశి సంఖ్య_t &dy) { move_x (dx); move_y (dy); }
  శూన్య move (స్థిరరాశి బిందువు_t &d) { move_x (d.x); move_y (d.y); }

  సంఖ్య_t  x;
  సంఖ్య_t  y;
};

మూస <రకం_పేరు ARG, రకం_పేరు SUBRS>
నిర్మాణం cs_interp_env_t : interp_env_t<ARG>
{
  cs_interp_env_t (స్థిరరాశి hb_ubytes_t &str, స్థిరరాశి SUBRS *globalSubrs_, స్థిరరాశి SUBRS *localSubrs_) :
    interp_env_t<ARG> (str)
  {
    context.init (str, CSType_CharString);
    చూసిన_moveto = నిజం;
    చూసిన_hintmask = అబద్ధం;
    hstem_లెక్క = 0;
    vstem_లెక్క = 0;
    hintmask_కొలత = 0;
    budget = శూన్యసూచిక;
    pt.సమితి_int (0, 0);
    globalSubrs.init (globalSubrs_);
    localSubrs.init (localSubrs_);
  }
  ~cs_interp_env_t ()
  {
    globalSubrs.fini ();
    localSubrs.fini ();
  }

  తార్కిక లోపల_error () స్థిరరాశి
  {
    తిరిగి call_దొంతర.లోపల_error () || SUPER::లోపల_error ();
  }

  తార్కిక తీయి_subr_అంకె (స్థిరరాశి biased_subrs_t<SUBRS>& biasedSubrs, గుర్తులేని పూర్ణాంకం &subr_అంకె_2)
  {
    subr_అంకె_2 = 0;
    పూర్ణాంకం n = SUPER::arg_దొంతర.తీయి_int ();
    n += biasedSubrs.get_bias ();
    ఒకవేళ (unlikely ((n < 0) || ((గుర్తులేని పూర్ణాంకం)n >= biasedSubrs.get_లెక్క ())))
      తిరిగి అబద్ధం;

    subr_అంకె_2 = (గుర్తులేని పూర్ణాంకం)n;
    తిరిగి నిజం;
  }

  శూన్య call_subr (స్థిరరాశి biased_subrs_t<SUBRS>& biasedSubrs, cs_type_t type)
  {
    గుర్తులేని పూర్ణాంకం subr_అంకె_2 = 0;

    ఒకవేళ (unlikely (!తీయి_subr_అంకె (biasedSubrs, subr_అంకె_2)
		 || call_దొంతర.get_లెక్క () >= k_Max_Call_హద్దు))
    {
      SUPER::సమితి_error ();
      తిరిగి;
    }
    context.str_ref = SUPER::str_ref;
    call_దొంతర.push (context);

    context.init ( biasedSubrs[subr_అంకె_2], type, subr_అంకె_2);
    SUPER::str_ref = context.str_ref;
  }

  శూన్య return_from_subr ()
  {
    ఒకవేళ (unlikely (SUPER::str_ref.లోపల_error ()))
      SUPER::సమితి_error ();
    context = call_దొంతర.తీయి ();
    SUPER::str_ref = context.str_ref;
  }

  శూన్య determine_hintmask_కొలత ()
  {
    ఒకవేళ (!చూసిన_hintmask)
    {
      vstem_లెక్క += SUPER::arg_దొంతర.get_లెక్క() / 2;
      hintmask_కొలత = (hstem_లెక్క + vstem_లెక్క + 7) >> 3;
      చూసిన_hintmask = నిజం;
    }
  }

  శూన్య సమితి_endchar (తార్కిక endchar_flag_) { endchar_flag = endchar_flag_; }
  తార్కిక is_endchar () స్థిరరాశి { తిరిగి endchar_flag; }

  శూన్య సమితి_budget (int64_t *budget_) { budget = budget_; }
  int64_t *get_budget () స్థిరరాశి { తిరిగి budget; }
  తార్కిక spend_budget (గుర్తులేని పూర్ణాంకం ఖర్చు, గుర్తులేని పూర్ణాంకం mult = 1)
  { తిరిగి !budget || hb_budget_spend (*budget, ఖర్చు, mult); }

  స్థిరరాశి సంఖ్య_t &get_x () స్థిరరాశి { తిరిగి pt.x; }
  స్థిరరాశి సంఖ్య_t &get_y () స్థిరరాశి { తిరిగి pt.y; }
  స్థిరరాశి బిందువు_t &get_pt () స్థిరరాశి { తిరిగి pt; }

  శూన్య moveto (స్థిరరాశి బిందువు_t &pt_ ) { pt = pt_; }

  బహిరంగ:
  call_context_t   context;
  తార్కిక	  endchar_flag;
  తార్కిక	  చూసిన_moveto;
  తార్కిక	  చూసిన_hintmask;

  గుర్తులేని పూర్ణాంకం  hstem_లెక్క;
  గుర్తులేని పూర్ణాంకం  vstem_లెక్క;
  గుర్తులేని పూర్ణాంకం  hintmask_కొలత;
  int64_t	*budget;
  call_దొంతర_t	call_దొంతర;
  biased_subrs_t<SUBRS>   globalSubrs;
  biased_subrs_t<SUBRS>   localSubrs;

  అంతరంగ:
  బిందువు_t	 pt;

  రకనిర్వచనం interp_env_t<ARG> SUPER;
};

మూస <రకం_పేరు ENV, రకం_పేరు PARAM>
నిర్మాణం మార్గం_procs_null_t
{
  స్థిర శూన్య rmoveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య hmoveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య vmoveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య rlineto (ENV &env, PARAM& param) {}
  స్థిర శూన్య hlineto (ENV &env, PARAM& param) {}
  స్థిర శూన్య vlineto (ENV &env, PARAM& param) {}
  స్థిర శూన్య rrcurveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య rcurveline (ENV &env, PARAM& param) {}
  స్థిర శూన్య rlinecurve (ENV &env, PARAM& param) {}
  స్థిర శూన్య vvcurveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య hhcurveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య vhcurveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య hvcurveto (ENV &env, PARAM& param) {}
  స్థిర శూన్య moveto (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt) {}
  స్థిర శూన్య పంక్తి (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt1) {}
  స్థిర శూన్య వక్రం (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt1, స్థిరరాశి బిందువు_t &pt2, స్థిరరాశి బిందువు_t &pt3) {}
  స్థిర శూన్య hflex (ENV &env, PARAM& param) {}
  స్థిర శూన్య flex (ENV &env, PARAM& param) {}
  స్థిర శూన్య hflex1 (ENV &env, PARAM& param) {}
  స్థిర శూన్య flex1 (ENV &env, PARAM& param) {}
};

మూస <రకం_పేరు ARG, రకం_పేరు OPSET, రకం_పేరు ENV, రకం_పేరు PARAM, రకం_పేరు మార్గం_3=మార్గం_procs_null_t<ENV, PARAM>>
నిర్మాణం cs_opset_t : opset_t<ARG>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    ఎంపిక (op) {

      స్థితి Op_సంకేతం_return:
	env.return_from_subr ();
	ఆపు;
      స్థితి Op_సంకేతం_endchar:
	OPSET::check_వెడల్పు (op, env, param);
	env.సమితి_endchar (నిజం);
	OPSET::flush_వాదనలు_and_op (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_fixedcs:
	env.arg_దొంతర.నెట్టు_fixed_from_substr (env.str_ref);
	ఆపు;

      స్థితి Op_సంకేతం_callsubr:
	env.call_subr (env.localSubrs, CSType_LocalSubr);
	ఆపు;

      స్థితి Op_సంకేతం_callgsubr:
	env.call_subr (env.globalSubrs, CSType_GlobalSubr);
	ఆపు;

      స్థితి Op_సంకేతం_hstem:
      స్థితి Op_సంకేతం_hstemhm:
	OPSET::check_వెడల్పు (op, env, param);
	OPSET::process_hstem (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_vstem:
      స్థితి Op_సంకేతం_vstemhm:
	OPSET::check_వెడల్పు (op, env, param);
	OPSET::process_vstem (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_hintmask:
      స్థితి Op_సంకేతం_cntrmask:
	OPSET::check_వెడల్పు (op, env, param);
	OPSET::process_hintmask (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_rmoveto:
	OPSET::check_వెడల్పు (op, env, param);
	మార్గం_3::rmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_hmoveto:
	OPSET::check_వెడల్పు (op, env, param);
	మార్గం_3::hmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_vmoveto:
	OPSET::check_వెడల్పు (op, env, param);
	మార్గం_3::vmoveto (env, param);
	OPSET::process_post_move (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_rlineto:
	మార్గం_3::rlineto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_hlineto:
	మార్గం_3::hlineto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_vlineto:
	మార్గం_3::vlineto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_rrcurveto:
	మార్గం_3::rrcurveto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_rcurveline:
	మార్గం_3::rcurveline (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_rlinecurve:
	మార్గం_3::rlinecurve (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_vvcurveto:
	మార్గం_3::vvcurveto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_hhcurveto:
	మార్గం_3::hhcurveto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_vhcurveto:
	మార్గం_3::vhcurveto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;
      స్థితి Op_సంకేతం_hvcurveto:
	మార్గం_3::hvcurveto (env, param);
	process_post_మార్గం (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_hflex:
	మార్గం_3::hflex (env, param);
	OPSET::process_post_flex (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_flex:
	మార్గం_3::flex (env, param);
	OPSET::process_post_flex (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_hflex1:
	మార్గం_3::hflex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_flex1:
	మార్గం_3::flex1 (env, param);
	OPSET::process_post_flex (op, env, param);
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env);
	ఆపు;
    }
  }

  స్థిర శూన్య process_hstem (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    env.hstem_లెక్క += env.arg_దొంతర.get_లెక్క () / 2;
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర శూన్య process_vstem (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    env.vstem_లెక్క += env.arg_దొంతర.get_లెక్క () / 2;
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర శూన్య process_hintmask (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    env.determine_hintmask_కొలత ();
    ఒకవేళ (likely (env.str_ref.avail (env.hintmask_కొలత)))
    {
      OPSET::flush_hintmask (op, env, param);
      env.str_ref.inc (env.hintmask_కొలత);
    }
  }

  స్థిర శూన్య process_post_flex (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర శూన్య check_వెడల్పు (op_సంకేతం_t op, ENV &env, PARAM& param)
  {}

  స్థిర శూన్య process_post_move (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    ఒకవేళ (!env.చూసిన_moveto)
    {
      env.determine_hintmask_కొలత ();
      env.చూసిన_moveto = నిజం;
    }
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర శూన్య process_post_మార్గం (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర శూన్య flush_వాదనలు_and_op (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_వాదనలు (env, param);
    OPSET::flush_op (op, env, param);
  }

  స్థిర శూన్య flush_వాదనలు (ENV &env, PARAM& param)
  {
    env.తీయి_n_వాదనలు (env.arg_దొంతర.get_లెక్క ());
  }

  స్థిర శూన్య flush_op (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
  }

  స్థిర శూన్య flush_hintmask (op_సంకేతం_t op, ENV &env, PARAM& param)
  {
    OPSET::flush_వాదనలు_and_op (op, env, param);
  }

  స్థిర తార్కిక is_సంఖ్య_op (op_సంకేతం_t op)
  {
    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_shortint:
      స్థితి Op_సంకేతం_fixedcs:
      స్థితి Op_సంకేతం_Two_Byte_Pos_Int0: స్థితి Op_సంకేతం_Two_Byte_Pos_Int1:
      స్థితి Op_సంకేతం_Two_Byte_Pos_Int2: స్థితి Op_సంకేతం_Two_Byte_Pos_Int3:
      స్థితి Op_సంకేతం_Two_Byte_Neg_Int0: స్థితి Op_సంకేతం_Two_Byte_Neg_Int1:
      స్థితి Op_సంకేతం_Two_Byte_Neg_Int2: స్థితి Op_సంకేతం_Two_Byte_Neg_Int3:
	తిరిగి నిజం;

      అప్రమేయ:
	/* 1-byte integer */
	తిరిగి (Op_సంకేతం_One_Byte_Int_మొదటి <= op) && (op <= Op_సంకేతం_One_Byte_Int_చివరి);
    }
  }

  రక్షిత:
  రకనిర్వచనం opset_t<ARG>  SUPER;
};

మూస <రకం_పేరు మార్గం_3, రకం_పేరు ENV, రకం_పేరు PARAM>
నిర్మాణం మార్గం_procs_t
{
  స్థిర శూన్య rmoveto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1 = env.get_pt ();
    స్థిరరాశి సంఖ్య_t &dy = env.తీయి_arg ();
    స్థిరరాశి సంఖ్య_t &dx = env.తీయి_arg ();
    pt1.move (dx, dy);
    మార్గం_3::moveto (env, param, pt1);
  }

  స్థిర శూన్య hmoveto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1 = env.get_pt ();
    pt1.move_x (env.తీయి_arg ());
    మార్గం_3::moveto (env, param, pt1);
  }

  స్థిర శూన్య vmoveto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1 = env.get_pt ();
    pt1.move_y (env.తీయి_arg ());
    మార్గం_3::moveto (env, param, pt1);
  }

  స్థిర శూన్య rlineto (ENV &env, PARAM& param)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i + 2 <= env.arg_దొంతర.get_లెక్క (); i += 2)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      మార్గం_3::పంక్తి (env, param, pt1);
    }
  }

  స్థిర శూన్య hlineto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1;
    గుర్తులేని పూర్ణాంకం i = 0;
    కొరకు (; i + 2 <= env.arg_దొంతర.get_లెక్క (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      మార్గం_3::పంక్తి (env, param, pt1);
      pt1.move_y (env.eval_arg (i+1));
      మార్గం_3::పంక్తి (env, param, pt1);
    }
    ఒకవేళ (i < env.arg_దొంతర.get_లెక్క ())
    {
      pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      మార్గం_3::పంక్తి (env, param, pt1);
    }
  }

  స్థిర శూన్య vlineto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1;
    గుర్తులేని పూర్ణాంకం i = 0;
    కొరకు (; i + 2 <= env.arg_దొంతర.get_లెక్క (); i += 2)
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      మార్గం_3::పంక్తి (env, param, pt1);
      pt1.move_x (env.eval_arg (i+1));
      మార్గం_3::పంక్తి (env, param, pt1);
    }
    ఒకవేళ (i < env.arg_దొంతర.get_లెక్క ())
    {
      pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      మార్గం_3::పంక్తి (env, param, pt1);
    }
  }

  స్థిర శూన్య rrcurveto (ENV &env, PARAM& param)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i + 6 <= env.arg_దొంతర.get_లెక్క (); i += 6)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      బిందువు_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
    }
  }

  స్థిర శూన్య rcurveline (ENV &env, PARAM& param)
  {
    గుర్తులేని పూర్ణాంకం arg_లెక్క = env.arg_దొంతర.get_లెక్క ();
    ఒకవేళ (unlikely (arg_లెక్క < 8))
      తిరిగి;

    గుర్తులేని పూర్ణాంకం i = 0;
    గుర్తులేని పూర్ణాంకం వక్రం_హద్దు = arg_లెక్క - 2;
    కొరకు (; i + 6 <= వక్రం_హద్దు; i += 6)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
      బిందువు_t pt3 = pt2;
      pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
    }

    బిందువు_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    మార్గం_3::పంక్తి (env, param, pt1);
  }

  స్థిర శూన్య rlinecurve (ENV &env, PARAM& param)
  {
    గుర్తులేని పూర్ణాంకం arg_లెక్క = env.arg_దొంతర.get_లెక్క ();
    ఒకవేళ (unlikely (arg_లెక్క < 8))
      తిరిగి;

    గుర్తులేని పూర్ణాంకం i = 0;
    గుర్తులేని పూర్ణాంకం పంక్తి_హద్దు = arg_లెక్క - 6;
    కొరకు (; i + 2 <= పంక్తి_హద్దు; i += 2)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (i), env.eval_arg (i+1));
      మార్గం_3::పంక్తి (env, param, pt1);
    }

    బిందువు_t pt1 = env.get_pt ();
    pt1.move (env.eval_arg (i), env.eval_arg (i+1));
    బిందువు_t pt2 = pt1;
    pt2.move (env.eval_arg (i+2), env.eval_arg (i+3));
    బిందువు_t pt3 = pt2;
    pt3.move (env.eval_arg (i+4), env.eval_arg (i+5));
    మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
  }

  స్థిర శూన్య vvcurveto (ENV &env, PARAM& param)
  {
    గుర్తులేని పూర్ణాంకం i = 0;
    బిందువు_t pt1 = env.get_pt ();
    ఒకవేళ ((env.arg_దొంతర.get_లెక్క () & 1) != 0)
      pt1.move_x (env.eval_arg (i++));
    కొరకు (; i + 4 <= env.arg_దొంతర.get_లెక్క (); i += 4)
    {
      pt1.move_y (env.eval_arg (i));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      బిందువు_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  స్థిర శూన్య hhcurveto (ENV &env, PARAM& param)
  {
    గుర్తులేని పూర్ణాంకం i = 0;
    బిందువు_t pt1 = env.get_pt ();
    ఒకవేళ ((env.arg_దొంతర.get_లెక్క () & 1) != 0)
      pt1.move_y (env.eval_arg (i++));
    కొరకు (; i + 4 <= env.arg_దొంతర.get_లెక్క (); i += 4)
    {
      pt1.move_x (env.eval_arg (i));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      బిందువు_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
      pt1 = env.get_pt ();
    }
  }

  స్థిర శూన్య vhcurveto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1, pt2, pt3;
    గుర్తులేని పూర్ణాంకం i = 0;
    ఒకవేళ ((env.arg_దొంతర.get_లెక్క () % 8) >= 4)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move_y (env.eval_arg (i));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      బిందువు_t pt3 = pt2;
      pt3.move_x (env.eval_arg (i+3));
      i += 4;

      కొరకు (; i + 8 <= env.arg_దొంతర.get_లెక్క (); i += 8)
      {
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
      }
      ఒకవేళ (i < env.arg_దొంతర.get_లెక్క ())
	pt3.move_y (env.eval_arg (i));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
    }
    లేకపోతే
    {
      కొరకు (; i + 8 <= env.arg_దొంతర.get_లెక్క (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
	ఒకవేళ ((env.arg_దొంతర.get_లెక్క () - i < 16) && ((env.arg_దొంతర.get_లెక్క () & 1) != 0))
	  pt3.move_x (env.eval_arg (i+8));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
      }
    }
  }

  స్థిర శూన్య hvcurveto (ENV &env, PARAM& param)
  {
    బిందువు_t pt1, pt2, pt3;
    గుర్తులేని పూర్ణాంకం i = 0;
    ఒకవేళ ((env.arg_దొంతర.get_లెక్క () % 8) >= 4)
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (i));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
      బిందువు_t pt3 = pt2;
      pt3.move_y (env.eval_arg (i+3));
      i += 4;

      కొరకు (; i + 8 <= env.arg_దొంతర.get_లెక్క (); i += 8)
      {
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
	pt1 = env.get_pt ();
	pt1.move_y (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+3));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_x (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+7));
      }
      ఒకవేళ (i < env.arg_దొంతర.get_లెక్క ())
	pt3.move_x (env.eval_arg (i));
      మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
    }
    లేకపోతే
    {
      కొరకు (; i + 8 <= env.arg_దొంతర.get_లెక్క (); i += 8)
      {
	pt1 = env.get_pt ();
	pt1.move_x (env.eval_arg (i));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+1), env.eval_arg (i+2));
	pt3 = pt2;
	pt3.move_y (env.eval_arg (i+3));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);

	pt1 = pt3;
	pt1.move_y (env.eval_arg (i+4));
	pt2 = pt1;
	pt2.move (env.eval_arg (i+5), env.eval_arg (i+6));
	pt3 = pt2;
	pt3.move_x (env.eval_arg (i+7));
	ఒకవేళ ((env.arg_దొంతర.get_లెక్క () - i < 16) && ((env.arg_దొంతర.get_లెక్క () & 1) != 0))
	  pt3.move_y (env.eval_arg (i+8));
	మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
      }
    }
  }

  /* default actions to be overridden */
  స్థిర శూన్య moveto (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt)
  { env.moveto (pt); }

  స్థిర శూన్య పంక్తి (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt1)
  { మార్గం_3::moveto (env, param, pt1); }

  స్థిర శూన్య వక్రం (ENV &env, PARAM& param, స్థిరరాశి బిందువు_t &pt1, స్థిరరాశి బిందువు_t &pt2, స్థిరరాశి బిందువు_t &pt3)
  { మార్గం_3::moveto (env, param, pt3); }

  స్థిర శూన్య hflex (ENV &env, PARAM& param)
  {
    ఒకవేళ (likely (env.arg_దొంతర.get_లెక్క () == 7))
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move_x (env.eval_arg (0));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (1), env.eval_arg (2));
      బిందువు_t pt3 = pt2;
      pt3.move_x (env.eval_arg (3));
      బిందువు_t pt4 = pt3;
      pt4.move_x (env.eval_arg (4));
      బిందువు_t pt5 = pt4;
      pt5.move_x (env.eval_arg (5));
      pt5.y = pt1.y;
      బిందువు_t pt6 = pt5;
      pt6.move_x (env.eval_arg (6));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    లేకపోతే
      env.సమితి_error ();
  }

  స్థిర శూన్య flex (ENV &env, PARAM& param)
  {
    ఒకవేళ (likely (env.arg_దొంతర.get_లెక్క () == 13))
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      బిందువు_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      బిందువు_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      బిందువు_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      బిందువు_t pt6 = pt5;
      pt6.move (env.eval_arg (10), env.eval_arg (11));

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    లేకపోతే
      env.సమితి_error ();
  }

  స్థిర శూన్య hflex1 (ENV &env, PARAM& param)
  {
    ఒకవేళ (likely (env.arg_దొంతర.get_లెక్క () == 9))
    {
      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      బిందువు_t pt3 = pt2;
      pt3.move_x (env.eval_arg (4));
      బిందువు_t pt4 = pt3;
      pt4.move_x (env.eval_arg (5));
      బిందువు_t pt5 = pt4;
      pt5.move (env.eval_arg (6), env.eval_arg (7));
      బిందువు_t pt6 = pt5;
      pt6.move_x (env.eval_arg (8));
      pt6.y = env.get_pt ().y;

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    లేకపోతే
      env.సమితి_error ();
  }

  స్థిర శూన్య flex1 (ENV &env, PARAM& param)
  {
    ఒకవేళ (likely (env.arg_దొంతర.get_లెక్క () == 11))
    {
      బిందువు_t d;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 10; i += 2)
	d.move (env.eval_arg (i), env.eval_arg (i+1));

      బిందువు_t pt1 = env.get_pt ();
      pt1.move (env.eval_arg (0), env.eval_arg (1));
      బిందువు_t pt2 = pt1;
      pt2.move (env.eval_arg (2), env.eval_arg (3));
      బిందువు_t pt3 = pt2;
      pt3.move (env.eval_arg (4), env.eval_arg (5));
      బిందువు_t pt4 = pt3;
      pt4.move (env.eval_arg (6), env.eval_arg (7));
      బిందువు_t pt5 = pt4;
      pt5.move (env.eval_arg (8), env.eval_arg (9));
      బిందువు_t pt6 = pt5;

      ఒకవేళ (fabs (d.x.గమ్యం_real ()) > fabs (d.y.గమ్యం_real ()))
      {
	pt6.move_x (env.eval_arg (10));
	pt6.y = env.get_pt ().y;
      }
      లేకపోతే
      {
	pt6.x = env.get_pt ().x;
	pt6.move_y (env.eval_arg (10));
      }

      curve2 (env, param, pt1, pt2, pt3, pt4, pt5, pt6);
    }
    లేకపోతే
      env.సమితి_error ();
  }

  రక్షిత:
  స్థిర శూన్య curve2 (ENV &env, PARAM& param,
		      స్థిరరాశి బిందువు_t &pt1, స్థిరరాశి బిందువు_t &pt2, స్థిరరాశి బిందువు_t &pt3,
		      స్థిరరాశి బిందువు_t &pt4, స్థిరరాశి బిందువు_t &pt5, స్థిరరాశి బిందువు_t &pt6)
  {
    మార్గం_3::వక్రం (env, param, pt1, pt2, pt3);
    మార్గం_3::వక్రం (env, param, pt4, pt5, pt6);
  }
};

మూస <రకం_పేరు ENV, రకం_పేరు OPSET, రకం_పేరు PARAM>
నిర్మాణం cs_interpreter_t : interpreter_t<ENV>
{
  cs_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  తార్కిక interpret (PARAM& param, int64_t *budget = శూన్యసూచిక)
  {
    SUPER::env.సమితి_endchar (అబద్ధం);
    SUPER::env.సమితి_budget (budget);

    /* Cap the operator count by the caller's remaining budget so the single
     * max_ops counter enforces both the structural limit and the live work
     * budget, with no budget check in the hot loop.  The consumed operators
     * are charged back after the loop, and since the cap never exceeds the
     * remaining budget the charstring cannot overshoot it.  CFF2 blend
     * charges its proportional work separately, in place. */
    int64_t మిగిలిన = budget ? *budget : (int64_t) HB_CFF_MAX_OPS;
    గుర్తులేని max_ops = (గుర్తులేని) hb_clamp (మిగిలిన, (int64_t) 0, (int64_t) HB_CFF_MAX_OPS);
    గుర్తులేని ప్రారంభం_ops = max_ops;

    తార్కిక ret = నిజం;
    ఒకవేళ (likely (max_ops))
      కొరకు (;;) {
	OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
	ఒకవేళ (unlikely (SUPER::env.లోపల_error () || !--max_ops))
	{
	  SUPER::env.సమితి_error ();
	  ret = అబద్ధం;
	  ఆపు;
	}
	ఒకవేళ (SUPER::env.is_endchar ())
	  ఆపు;
      }
    లేకపోతే
    {
      SUPER::env.సమితి_error ();
      ret = అబద్ధం;
    }

    ఒకవేళ (budget) *budget -= (int64_t) (ప్రారంభం_ops - max_ops);

    తిరిగి ret;
  }

  అంతరంగ:
  రకనిర్వచనం interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF_INTERP_CS_COMMON_HH */
