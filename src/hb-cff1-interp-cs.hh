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
#నిర్వచితం_కాకపోతే HB_CFF1_INTERP_CS_HH
#నిర్వచించు HB_CFF1_INTERP_CS_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-cff-interp-cs-common.hh"

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

రకనిర్వచనం biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

నిర్మాణం cff1_cs_interp_env_t : cs_interp_env_t<సంఖ్య_t, CFF1Subrs>
{
  మూస <రకం_పేరు ACC>
  cff1_cs_interp_env_t (స్థిరరాశి hb_ubytes_t &str, ACC &acc, గుర్తులేని పూర్ణాంకం fd,
			స్థిరరాశి పూర్ణాంకం *coords_=శూన్యసూచిక, గుర్తులేని పూర్ణాంకం అంకె_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.వ్యక్తిగత_Dicts[fd].localSubrs)
  {
    processed_వెడల్పు = అబద్ధం;
    has_వెడల్పు = అబద్ధం;
    arg_ప్రారంభం = 0;
    లోపల_seac = అబద్ధం;
  }

  శూన్య సమితి_వెడల్పు (తార్కిక has_వెడల్పు_2)
  {
    ఒకవేళ (likely (!processed_వెడల్పు && (SUPER::arg_దొంతర.get_లెక్క () > 0)))
    {
      ఒకవేళ (has_వెడల్పు_2)
      {
	వెడల్పు = SUPER::arg_దొంతర[0];
	has_వెడల్పు = నిజం;
	arg_ప్రారంభం = 1;
      }
    }
    processed_వెడల్పు = నిజం;
  }

  శూన్య clear_వాదనలు ()
  {
    arg_ప్రారంభం = 0;
    SUPER::clear_వాదనలు ();
  }

  శూన్య సమితి_లోపల_seac (తార్కిక లోపల_seac_2) { లోపల_seac = లోపల_seac_2; }

  తార్కిక	  processed_వెడల్పు;
  తార్కిక	  has_వెడల్పు;
  గుర్తులేని పూర్ణాంకం  arg_ప్రారంభం;
  సంఖ్య_t	వెడల్పు;
  తార్కిక	  లోపల_seac;

  అంతరంగ:
  రకనిర్వచనం cs_interp_env_t<సంఖ్య_t, CFF1Subrs> SUPER;
};

మూస <రకం_పేరు OPSET, రకం_పేరు PARAM, రకం_పేరు మార్గం_3=మార్గం_procs_null_t<cff1_cs_interp_env_t, PARAM>>
నిర్మాణం cff1_cs_opset_t : cs_opset_t<సంఖ్య_t, OPSET, cff1_cs_interp_env_t, PARAM, మార్గం_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  స్థిర శూన్య process_op (op_సంకేతం_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_dotsection:
	SUPER::flush_వాదనలు_and_op (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_endchar:
	OPSET::check_వెడల్పు (op, env, param);
	ఒకవేళ (env.arg_దొంతర.get_లెక్క () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_వాదనలు_and_op (op, env, param);
	env.సమితి_endchar (నిజం);
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env, param);
    }
  }

  స్థిర శూన్య check_వెడల్పు (op_సంకేతం_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ఒకవేళ (!env.processed_వెడల్పు)
    {
      తార్కిక  has_వెడల్పు = అబద్ధం;
      ఎంపిక (op)
      {
	స్థితి Op_సంకేతం_endchar:
	స్థితి Op_సంకేతం_hstem:
	స్థితి Op_సంకేతం_hstemhm:
	స్థితి Op_సంకేతం_vstem:
	స్థితి Op_సంకేతం_vstemhm:
	స్థితి Op_సంకేతం_hintmask:
	స్థితి Op_సంకేతం_cntrmask:
	  has_వెడల్పు = ((env.arg_దొంతర.get_లెక్క () & 1) != 0);
	  ఆపు;
	స్థితి Op_సంకేతం_hmoveto:
	స్థితి Op_సంకేతం_vmoveto:
	  has_వెడల్పు = (env.arg_దొంతర.get_లెక్క () > 1);
	  ఆపు;
	స్థితి Op_సంకేతం_rmoveto:
	  has_వెడల్పు = (env.arg_దొంతర.get_లెక్క () > 2);
	  ఆపు;
	అప్రమేయ:
	  తిరిగి;
      }
      env.సమితి_వెడల్పు (has_వెడల్పు);
    }
  }

  స్థిర శూన్య process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  స్థిర శూన్య flush_వాదనలు (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_వాదనలు (env, param);
    env.clear_వాదనలు ();  /* pop off width */
  }

  అంతరంగ:
  రకనిర్వచనం cs_opset_t<సంఖ్య_t, OPSET, cff1_cs_interp_env_t, PARAM, మార్గం_3>  SUPER;
};

మూస <రకం_పేరు OPSET, రకం_పేరు PARAM>
ఉపయోగించి cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF1_INTERP_CS_HH */
