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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-cff-interp-cs-common.hh"

நாமாகாஶ CFF {

using நாமாகாஶ OT;

வகைவரை biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

கட்டமைப்பு cff1_cs_interp_env_t : cs_interp_env_t<எண்_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (மாறிலி hb_ubytes_t &str, ACC &acc, unsigned முழு fd,
			மாறிலி முழு *coords_=nullptr, unsigned முழு இலக்கம்_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = பொய்;
    has_width = பொய்;
    arg_start = 0;
    in_seac = பொய்;
  }

  வெறுமை set_width (தருக்க has_width_)
  {
    எனில் (likely (!processed_width && (SUPER::arg_அடுக்கு.get_மொத்தம் () > 0)))
    {
      எனில் (has_width_)
      {
	width = SUPER::arg_அடுக்கு[0];
	has_width = உண்மை;
	arg_start = 1;
      }
    }
    processed_width = உண்மை;
  }

  வெறுமை clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  வெறுமை set_in_seac (தருக்க _in_seac) { in_seac = _in_seac; }

  தருக்க	  processed_width;
  தருக்க	  has_width;
  unsigned முழு  arg_start;
  எண்_t	width;
  தருக்க	  in_seac;

  தனிப்பட்ட:
  வகைவரை cs_interp_env_t<எண்_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
கட்டமைப்பு cff1_cs_opset_t : cs_opset_t<எண்_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  நிலையான வெறுமை process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    தேர்வு (op) {
      நிலை OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	நிறுத்து;

      நிலை OpCode_endchar:
	OPSET::check_width (op, env, param);
	எனில் (env.arg_அடுக்கு.get_மொத்தம் () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (உண்மை);
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env, param);
    }
  }

  நிலையான வெறுமை check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    எனில் (!env.processed_width)
    {
      தருக்க  has_width = பொய்;
      தேர்வு (op)
      {
	நிலை OpCode_endchar:
	நிலை OpCode_hstem:
	நிலை OpCode_hstemhm:
	நிலை OpCode_vstem:
	நிலை OpCode_vstemhm:
	நிலை OpCode_hintmask:
	நிலை OpCode_cntrmask:
	  has_width = ((env.arg_அடுக்கு.get_மொத்தம் () & 1) != 0);
	  நிறுத்து;
	நிலை OpCode_hmoveto:
	நிலை OpCode_vmoveto:
	  has_width = (env.arg_அடுக்கு.get_மொத்தம் () > 1);
	  நிறுத்து;
	நிலை OpCode_rmoveto:
	  has_width = (env.arg_அடுக்கு.get_மொத்தம் () > 2);
	  நிறுத்து;
	இயல்பு:
	  திரும்பு;
      }
      env.set_width (has_width);
    }
  }

  நிலையான வெறுமை process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  நிலையான வெறுமை flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  தனிப்பட்ட:
  வகைவரை cs_opset_t<எண்_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
