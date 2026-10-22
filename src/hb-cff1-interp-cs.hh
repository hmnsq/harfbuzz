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
#جے_بنا_تعریف HB_CFF1_INTERP_CS_HH
#تعریف_کرو HB_CFF1_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

ورتوں نطاق OT;

تعريفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

بنية cff1_cs_interp_env_t : cs_interp_env_t<نمبر_t, CFF1Subrs>
{
  سانچا <قسم_ناں ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بنانشانی صحيح fd,
			ثابت صحيح *coords_=خالی_سوچک, بنانشانی صحيح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.نجی_Dicts[fd].localSubrs)
  {
    processed_چوڑائی = خطأ;
    has_چوڑائی = خطأ;
    arg_شروع = 0;
    وچ_seac = خطأ;
  }

  فراغ سیٹ_چوڑائی (منطقي has_چوڑائی_2)
  {
    إذا (likely (!processed_چوڑائی && (SUPER::arg_ڈھیر.get_گنتی () > 0)))
    {
      إذا (has_چوڑائی_2)
      {
	چوڑائی = SUPER::arg_ڈھیر[0];
	has_چوڑائی = سچ;
	arg_شروع = 1;
      }
    }
    processed_چوڑائی = سچ;
  }

  فراغ clear_دلیلاں ()
  {
    arg_شروع = 0;
    SUPER::clear_دلیلاں ();
  }

  فراغ سیٹ_وچ_seac (منطقي وچ_seac_2) { وچ_seac = وچ_seac_2; }

  منطقي	  processed_چوڑائی;
  منطقي	  has_چوڑائی;
  بنانشانی صحيح  arg_شروع;
  نمبر_t	چوڑائی;
  منطقي	  وچ_seac;

  خاص:
  تعريفنوع cs_interp_env_t<نمبر_t, CFF1Subrs> SUPER;
};

سانچا <قسم_ناں OPSET, قسم_ناں PARAM, قسم_ناں رستہ_3=رستہ_procs_null_t<cff1_cs_interp_env_t, PARAM>>
بنية cff1_cs_opset_t : cs_opset_t<نمبر_t, OPSET, cff1_cs_interp_env_t, PARAM, رستہ_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساكن فراغ process_op (op_کوڈ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کوڈ_dotsection:
	SUPER::flush_دلیلاں_and_op (op, env, param);
	اكسر;

      حالة Op_کوڈ_endchar:
	OPSET::check_چوڑائی (op, env, param);
	إذا (env.arg_ڈھیر.get_گنتی () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_دلیلاں_and_op (op, env, param);
	env.سیٹ_endchar (سچ);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  ساكن فراغ check_چوڑائی (op_کوڈ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    إذا (!env.processed_چوڑائی)
    {
      منطقي  has_چوڑائی = خطأ;
      بدل (op)
      {
	حالة Op_کوڈ_endchar:
	حالة Op_کوڈ_hstem:
	حالة Op_کوڈ_hstemhm:
	حالة Op_کوڈ_vstem:
	حالة Op_کوڈ_vstemhm:
	حالة Op_کوڈ_hintmask:
	حالة Op_کوڈ_cntrmask:
	  has_چوڑائی = ((env.arg_ڈھیر.get_گنتی () & 1) != 0);
	  اكسر;
	حالة Op_کوڈ_hmoveto:
	حالة Op_کوڈ_vmoveto:
	  has_چوڑائی = (env.arg_ڈھیر.get_گنتی () > 1);
	  اكسر;
	حالة Op_کوڈ_rmoveto:
	  has_چوڑائی = (env.arg_ڈھیر.get_گنتی () > 2);
	  اكسر;
	افتراضي:
	  أرجع;
      }
      env.سیٹ_چوڑائی (has_چوڑائی);
    }
  }

  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساكن فراغ flush_دلیلاں (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_دلیلاں (env, param);
    env.clear_دلیلاں ();  /* pop off width */
  }

  خاص:
  تعريفنوع cs_opset_t<نمبر_t, OPSET, cff1_cs_interp_env_t, PARAM, رستہ_3>  SUPER;
};

سانچا <قسم_ناں OPSET, قسم_ناں PARAM>
ورتوں cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#جے_انت /* HB_CFF1_INTERP_CS_HH */
