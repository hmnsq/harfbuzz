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
#جيڪڏهن_وصف_نه_ٿيل HB_CFF1_INTERP_CS_HH
#وصف_ڪريو HB_CFF1_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

استعمال نطاق OT;

تعريفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

بنية cff1_cs_interp_env_t : cs_interp_env_t<عدد_t, CFF1Subrs>
{
  سانچو <قسم_نالو ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بنانشان صحيح fd,
			ثابت صحيح *coords_=خالي_اشارو, بنانشان صحيح انگ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.ذاتي_Dicts[fd].localSubrs)
  {
    processed_ويڪر = خطأ;
    has_ويڪر = خطأ;
    arg_شروعات = 0;
    اندريون_seac = خطأ;
  }

  فراغ سيٽ_ويڪر (منطقي has_ويڪر_2)
  {
    إذا (likely (!processed_ويڪر && (SUPER::arg_ڍير.get_ڳڻپ () > 0)))
    {
      إذا (has_ويڪر_2)
      {
	ويڪر = SUPER::arg_ڍير[0];
	has_ويڪر = سچ;
	arg_شروعات = 1;
      }
    }
    processed_ويڪر = سچ;
  }

  فراغ clear_دليلون ()
  {
    arg_شروعات = 0;
    SUPER::clear_دليلون ();
  }

  فراغ سيٽ_اندريون_seac (منطقي اندريون_seac_2) { اندريون_seac = اندريون_seac_2; }

  منطقي	  processed_ويڪر;
  منطقي	  has_ويڪر;
  بنانشان صحيح  arg_شروعات;
  عدد_t	ويڪر;
  منطقي	  اندريون_seac;

  خاص:
  تعريفنوع cs_interp_env_t<عدد_t, CFF1Subrs> SUPER;
};

سانچو <قسم_نالو OPSET, قسم_نالو PARAM, قسم_نالو رستو_3=رستو_procs_null_t<cff1_cs_interp_env_t, PARAM>>
بنية cff1_cs_opset_t : cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, رستو_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساكن فراغ process_op (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_ڪوڊ_dotsection:
	SUPER::flush_دليلون_and_op (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_endchar:
	OPSET::check_ويڪر (op, env, param);
	إذا (env.arg_ڍير.get_ڳڻپ () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_دليلون_and_op (op, env, param);
	env.سيٽ_endchar (سچ);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  ساكن فراغ check_ويڪر (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    إذا (!env.processed_ويڪر)
    {
      منطقي  has_ويڪر = خطأ;
      بدل (op)
      {
	حالة Op_ڪوڊ_endchar:
	حالة Op_ڪوڊ_hstem:
	حالة Op_ڪوڊ_hstemhm:
	حالة Op_ڪوڊ_vstem:
	حالة Op_ڪوڊ_vstemhm:
	حالة Op_ڪوڊ_hintmask:
	حالة Op_ڪوڊ_cntrmask:
	  has_ويڪر = ((env.arg_ڍير.get_ڳڻپ () & 1) != 0);
	  اكسر;
	حالة Op_ڪوڊ_hmoveto:
	حالة Op_ڪوڊ_vmoveto:
	  has_ويڪر = (env.arg_ڍير.get_ڳڻپ () > 1);
	  اكسر;
	حالة Op_ڪوڊ_rmoveto:
	  has_ويڪر = (env.arg_ڍير.get_ڳڻپ () > 2);
	  اكسر;
	افتراضي:
	  أرجع;
      }
      env.سيٽ_ويڪر (has_ويڪر);
    }
  }

  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساكن فراغ flush_دليلون (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_دليلون (env, param);
    env.clear_دليلون ();  /* pop off width */
  }

  خاص:
  تعريفنوع cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, رستو_3>  SUPER;
};

سانچو <قسم_نالو OPSET, قسم_نالو PARAM>
استعمال cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF1_INTERP_CS_HH */
