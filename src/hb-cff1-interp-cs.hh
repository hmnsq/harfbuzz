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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-cff-interp-cs-common.hh"

नामाकाश CFF {

using नामाकाश OT;

प्रकारनिर्देश biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

संरचना cff1_cs_interp_env_t : cs_interp_env_t<सङ्ख्या_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ध्रुव hb_ubytes_t &str, ACC &acc, unsigned पूर्णाङ्क fd,
			ध्रुव पूर्णाङ्क *coords_=nullptr, unsigned पूर्णाङ्क अङ्कः_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = मिच्छा;
    has_width = मिच्छा;
    arg_start = 0;
    in_seac = मिच्छा;
  }

  शून्य set_width (तार्किक has_width_)
  {
    सचे (likely (!processed_width && (SUPER::arg_राशिः.get_गणनम् () > 0)))
    {
      सचे (has_width_)
      {
	width = SUPER::arg_राशिः[0];
	has_width = सच्च;
	arg_start = 1;
      }
    }
    processed_width = सच्च;
  }

  शून्य clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  शून्य set_in_seac (तार्किक _in_seac) { in_seac = _in_seac; }

  तार्किक	  processed_width;
  तार्किक	  has_width;
  unsigned पूर्णाङ्क  arg_start;
  सङ्ख्या_t	width;
  तार्किक	  in_seac;

  गुप्त:
  प्रकारनिर्देश cs_interp_env_t<सङ्ख्या_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
संरचना cff1_cs_opset_t : cs_opset_t<सङ्ख्या_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  स्थिर शून्य process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    विकल्प (op) {
      पक्ष OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	भङ्ग;

      पक्ष OpCode_endchar:
	OPSET::check_width (op, env, param);
	सचे (env.arg_राशिः.get_गणनम् () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (सच्च);
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env, param);
    }
  }

  स्थिर शून्य check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    सचे (!env.processed_width)
    {
      तार्किक  has_width = मिच्छा;
      विकल्प (op)
      {
	पक्ष OpCode_endchar:
	पक्ष OpCode_hstem:
	पक्ष OpCode_hstemhm:
	पक्ष OpCode_vstem:
	पक्ष OpCode_vstemhm:
	पक्ष OpCode_hintmask:
	पक्ष OpCode_cntrmask:
	  has_width = ((env.arg_राशिः.get_गणनम् () & 1) != 0);
	  भङ्ग;
	पक्ष OpCode_hmoveto:
	पक्ष OpCode_vmoveto:
	  has_width = (env.arg_राशिः.get_गणनम् () > 1);
	  भङ्ग;
	पक्ष OpCode_rmoveto:
	  has_width = (env.arg_राशिः.get_गणनम् () > 2);
	  भङ्ग;
	स्वभाव:
	  पटिनिवत्त;
      }
      env.set_width (has_width);
    }
  }

  स्थिर शून्य process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  स्थिर शून्य flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  गुप्त:
  प्रकारनिर्देश cs_opset_t<सङ्ख्या_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
