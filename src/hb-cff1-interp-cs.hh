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

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

using نطاق OT;

تعريفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

بنية cff1_cs_interp_env_t : cs_interp_env_t<لمب_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, unsigned صحيح fd,
			ثابت صحيح *coords_=nullptr, unsigned صحيح لمب_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = خطأ;
    has_width = خطأ;
    arg_start = 0;
    in_seac = خطأ;
  }

  فراغ set_width (منطقي has_width_)
  {
    إذا (likely (!processed_width && (SUPER::arg_تر.get_كدي () > 0)))
    {
      إذا (has_width_)
      {
	width = SUPER::arg_تر[0];
	has_width = true;
	arg_start = 1;
      }
    }
    processed_width = true;
  }

  فراغ clear_حجوجي ()
  {
    arg_start = 0;
    SUPER::clear_حجوجي ();
  }

  فراغ set_in_seac (منطقي _in_seac) { in_seac = _in_seac; }

  منطقي	  processed_width;
  منطقي	  has_width;
  unsigned صحيح  arg_start;
  لمب_t	width;
  منطقي	  in_seac;

  خاص:
  تعريفنوع cs_interp_env_t<لمب_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
بنية cff1_cs_opset_t : cs_opset_t<لمب_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساكن فراغ process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    بدل (op) {
      حالة OpCode_dotsection:
	SUPER::flush_حجوجي_and_op (op, env, param);
	اكسر;

      حالة OpCode_endchar:
	OPSET::check_width (op, env, param);
	إذا (env.arg_تر.get_كدي () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_حجوجي_and_op (op, env, param);
	env.set_endchar (true);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  ساكن فراغ check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    إذا (!env.processed_width)
    {
      منطقي  has_width = خطأ;
      بدل (op)
      {
	حالة OpCode_endchar:
	حالة OpCode_hstem:
	حالة OpCode_hstemhm:
	حالة OpCode_vstem:
	حالة OpCode_vstemhm:
	حالة OpCode_hintmask:
	حالة OpCode_cntrmask:
	  has_width = ((env.arg_تر.get_كدي () & 1) != 0);
	  اكسر;
	حالة OpCode_hmoveto:
	حالة OpCode_vmoveto:
	  has_width = (env.arg_تر.get_كدي () > 1);
	  اكسر;
	حالة OpCode_rmoveto:
	  has_width = (env.arg_تر.get_كدي () > 2);
	  اكسر;
	افتراضي:
	  أرجع;
      }
      env.set_width (has_width);
    }
  }

  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساكن فراغ flush_حجوجي (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_حجوجي (env, param);
    env.clear_حجوجي ();  /* pop off width */
  }

  خاص:
  تعريفنوع cs_opset_t<لمب_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
