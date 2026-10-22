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
#ifndef HB_CFF1_INTERP_CS_HH
#define HB_CFF1_INTERP_CS_HH

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-cs-common.hh"

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

ପ୍ରକାରବ୍ୟାଖ୍ୟା biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ସଂରଚନା cff1_cs_interp_env_t : cs_interp_env_t<ସଂଖ୍ୟା_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ଅଚଳ hb_ubytes_t &str, ACC &acc, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
			ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords_=nullptr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = ମିଥ୍ୟା;
    has_width = ମିଥ୍ୟା;
    arg_start = 0;
    in_seac = ମିଥ୍ୟା;
  }

  ଶୂନ୍ୟ set_width (ତାର୍କିକ has_width_)
  {
    ଯଦି (likely (!processed_width && (SUPER::arg_ଗଦା.get_ଗଣନା () > 0)))
    {
      ଯଦି (has_width_)
      {
	width = SUPER::arg_ଗଦା[0];
	has_width = ସତ୍ୟ;
	arg_start = 1;
      }
    }
    processed_width = ସତ୍ୟ;
  }

  ଶୂନ୍ୟ clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  ଶୂନ୍ୟ set_in_seac (ତାର୍କିକ _in_seac) { in_seac = _in_seac; }

  ତାର୍କିକ	  processed_width;
  ତାର୍କିକ	  has_width;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  arg_start;
  ସଂଖ୍ୟା_t	width;
  ତାର୍କିକ	  in_seac;

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cs_interp_env_t<ସଂଖ୍ୟା_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ସଂରଚନା cff1_cs_opset_t : cs_opset_t<ସଂଖ୍ୟା_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_endchar:
	OPSET::check_width (op, env, param);
	ଯଦି (env.arg_ଗଦା.get_ଗଣନା () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (ସତ୍ୟ);
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env, param);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ଯଦି (!env.processed_width)
    {
      ତାର୍କିକ  has_width = ମିଥ୍ୟା;
      ବାଛ (op)
      {
	ସ୍ଥିତି OpCode_endchar:
	ସ୍ଥିତି OpCode_hstem:
	ସ୍ଥିତି OpCode_hstemhm:
	ସ୍ଥିତି OpCode_vstem:
	ସ୍ଥିତି OpCode_vstemhm:
	ସ୍ଥିତି OpCode_hintmask:
	ସ୍ଥିତି OpCode_cntrmask:
	  has_width = ((env.arg_ଗଦା.get_ଗଣନା () & 1) != 0);
	  ଭାଙ୍ଗ;
	ସ୍ଥିତି OpCode_hmoveto:
	ସ୍ଥିତି OpCode_vmoveto:
	  has_width = (env.arg_ଗଦା.get_ଗଣନା () > 1);
	  ଭାଙ୍ଗ;
	ସ୍ଥିତି OpCode_rmoveto:
	  has_width = (env.arg_ଗଦା.get_ଗଣନା () > 2);
	  ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	  ଫେରନ୍ତୁ;
      }
      env.set_width (has_width);
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cs_opset_t<ସଂଖ୍ୟା_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
