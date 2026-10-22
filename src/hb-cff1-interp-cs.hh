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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-cs-common.hh"

ನಾಮಾಕಾಶ CFF {

using ನಾಮಾಕಾಶ OT;

ಪ್ರಕಾರನಿರ್ದೇಶ biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ಸಂರಚನಾ cff1_cs_interp_env_t : cs_interp_env_t<ಸಂಖ್ಯೆ_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ಧ್ರುವ hb_ubytes_t &str, ACC &acc, unsigned ಪೂರ್ಣಾಙ್ಕ fd,
			ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords_=nullptr, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = ಅಸತ್ಯ;
    has_width = ಅಸತ್ಯ;
    arg_start = 0;
    in_seac = ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ set_width (ತಾರ್ಕಿಕ has_width_)
  {
    ಯದಿ (likely (!processed_width && (SUPER::arg_ರಾಶಿ.get_ಎಣಿಕೆ () > 0)))
    {
      ಯದಿ (has_width_)
      {
	width = SUPER::arg_ರಾಶಿ[0];
	has_width = ಸತ್ಯ;
	arg_start = 1;
      }
    }
    processed_width = ಸತ್ಯ;
  }

  ಶೂನ್ಯ clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  ಶೂನ್ಯ set_in_seac (ತಾರ್ಕಿಕ _in_seac) { in_seac = _in_seac; }

  ತಾರ್ಕಿಕ	  processed_width;
  ತಾರ್ಕಿಕ	  has_width;
  unsigned ಪೂರ್ಣಾಙ್ಕ  arg_start;
  ಸಂಖ್ಯೆ_t	width;
  ತಾರ್ಕಿಕ	  in_seac;

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cs_interp_env_t<ಸಂಖ್ಯೆ_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ಸಂರಚನಾ cff1_cs_opset_t : cs_opset_t<ಸಂಖ್ಯೆ_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_endchar:
	OPSET::check_width (op, env, param);
	ಯದಿ (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (ಸತ್ಯ);
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env, param);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ಯದಿ (!env.processed_width)
    {
      ತಾರ್ಕಿಕ  has_width = ಅಸತ್ಯ;
      ವಿಕಲ್ಪ (op)
      {
	ಪಕ್ಷ OpCode_endchar:
	ಪಕ್ಷ OpCode_hstem:
	ಪಕ್ಷ OpCode_hstemhm:
	ಪಕ್ಷ OpCode_vstem:
	ಪಕ್ಷ OpCode_vstemhm:
	ಪಕ್ಷ OpCode_hintmask:
	ಪಕ್ಷ OpCode_cntrmask:
	  has_width = ((env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () & 1) != 0);
	  ಭಙ್ಗ;
	ಪಕ್ಷ OpCode_hmoveto:
	ಪಕ್ಷ OpCode_vmoveto:
	  has_width = (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () > 1);
	  ಭಙ್ಗ;
	ಪಕ್ಷ OpCode_rmoveto:
	  has_width = (env.arg_ರಾಶಿ.get_ಎಣಿಕೆ () > 2);
	  ಭಙ್ಗ;
	ಸ್ವಭಾವ:
	  ಪ್ರತ್ಯಾಗಮ;
      }
      env.set_width (has_width);
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cs_opset_t<ಸಂಖ್ಯೆ_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
