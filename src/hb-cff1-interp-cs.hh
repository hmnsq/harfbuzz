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

#شامل "hb.hh"
#شامل "hb-cff-interp-cs-common.hh"

فضاینام CFF {

using فضاینام OT;

تعریفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ساختار cff1_cs_interp_env_t : cs_interp_env_t<عدد_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, unsigned صحیح fd,
			ثابت صحیح *coords_=nullptr, unsigned صحیح رقم_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = نادرست;
    has_width = نادرست;
    arg_start = 0;
    in_seac = نادرست;
  }

  تهی set_width (منطقی has_width_)
  {
    اگر (likely (!processed_width && (SUPER::arg_پشته.get_شمار () > 0)))
    {
      اگر (has_width_)
      {
	width = SUPER::arg_پشته[0];
	has_width = درست;
	arg_start = 1;
      }
    }
    processed_width = درست;
  }

  تهی clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  تهی set_in_seac (منطقی _in_seac) { in_seac = _in_seac; }

  منطقی	  processed_width;
  منطقی	  has_width;
  unsigned صحیح  arg_start;
  عدد_t	width;
  منطقی	  in_seac;

  خصوصی:
  تعریفنوع cs_interp_env_t<عدد_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ساختار cff1_cs_opset_t : cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ایستا تهی process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    انتخاب (op) {
      حالت OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	بشکن;

      حالت OpCode_endchar:
	OPSET::check_width (op, env, param);
	اگر (env.arg_پشته.get_شمار () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (درست);
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env, param);
    }
  }

  ایستا تهی check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    اگر (!env.processed_width)
    {
      منطقی  has_width = نادرست;
      انتخاب (op)
      {
	حالت OpCode_endchar:
	حالت OpCode_hstem:
	حالت OpCode_hstemhm:
	حالت OpCode_vstem:
	حالت OpCode_vstemhm:
	حالت OpCode_hintmask:
	حالت OpCode_cntrmask:
	  has_width = ((env.arg_پشته.get_شمار () & 1) != 0);
	  بشکن;
	حالت OpCode_hmoveto:
	حالت OpCode_vmoveto:
	  has_width = (env.arg_پشته.get_شمار () > 1);
	  بشکن;
	حالت OpCode_rmoveto:
	  has_width = (env.arg_پشته.get_شمار () > 2);
	  بشکن;
	پیشفرض:
	  بازگشت;
      }
      env.set_width (has_width);
    }
  }

  ایستا تهی process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ایستا تهی flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  خصوصی:
  تعریفنوع cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
