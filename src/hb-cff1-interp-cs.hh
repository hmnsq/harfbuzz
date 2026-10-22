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
#اگر_غیر_تعریف_شدہ HB_CFF1_INTERP_CS_HH
#تعریف_کرو HB_CFF1_INTERP_CS_HH

#شامل "hb.hh"
#شامل "hb-cff-interp-cs-common.hh"

نامخلا CFF {

استعمال نامخلا OT;

قسمتعریف biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ساخت cff1_cs_interp_env_t : cs_interp_env_t<نمبر_t, CFF1Subrs>
{
  سانچہ <قسم_نام ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, غیرعلامتی صحیح fd,
			ثابت صحیح *coords_=خالی_اشاریہ, غیرعلامتی صحیح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.نجی_Dicts[fd].localSubrs)
  {
    processed_چوڑائی = جھوٹ;
    has_چوڑائی = جھوٹ;
    arg_آغاز = 0;
    اندر_seac = جھوٹ;
  }

  خالی سیٹ_چوڑائی (منطقی has_چوڑائی_2)
  {
    اگر (likely (!processed_چوڑائی && (SUPER::arg_ڈھیر.get_گنتی () > 0)))
    {
      اگر (has_چوڑائی_2)
      {
	چوڑائی = SUPER::arg_ڈھیر[0];
	has_چوڑائی = سچ;
	arg_آغاز = 1;
      }
    }
    processed_چوڑائی = سچ;
  }

  خالی clear_دلائل ()
  {
    arg_آغاز = 0;
    SUPER::clear_دلائل ();
  }

  خالی سیٹ_اندر_seac (منطقی اندر_seac_2) { اندر_seac = اندر_seac_2; }

  منطقی	  processed_چوڑائی;
  منطقی	  has_چوڑائی;
  غیرعلامتی صحیح  arg_آغاز;
  نمبر_t	چوڑائی;
  منطقی	  اندر_seac;

  نجی:
  قسمتعریف cs_interp_env_t<نمبر_t, CFF1Subrs> SUPER;
};

سانچہ <قسم_نام OPSET, قسم_نام PARAM, قسم_نام راستہ_3=راستہ_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ساخت cff1_cs_opset_t : cs_opset_t<نمبر_t, OPSET, cff1_cs_interp_env_t, PARAM, راستہ_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساکن خالی process_op (op_رمز_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    انتخاب (op) {
      حالت Op_رمز_dotsection:
	SUPER::flush_دلائل_and_op (op, env, param);
	توڑو;

      حالت Op_رمز_endchar:
	OPSET::check_چوڑائی (op, env, param);
	اگر (env.arg_ڈھیر.get_گنتی () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_دلائل_and_op (op, env, param);
	env.سیٹ_endchar (سچ);
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env, param);
    }
  }

  ساکن خالی check_چوڑائی (op_رمز_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    اگر (!env.processed_چوڑائی)
    {
      منطقی  has_چوڑائی = جھوٹ;
      انتخاب (op)
      {
	حالت Op_رمز_endchar:
	حالت Op_رمز_hstem:
	حالت Op_رمز_hstemhm:
	حالت Op_رمز_vstem:
	حالت Op_رمز_vstemhm:
	حالت Op_رمز_hintmask:
	حالت Op_رمز_cntrmask:
	  has_چوڑائی = ((env.arg_ڈھیر.get_گنتی () & 1) != 0);
	  توڑو;
	حالت Op_رمز_hmoveto:
	حالت Op_رمز_vmoveto:
	  has_چوڑائی = (env.arg_ڈھیر.get_گنتی () > 1);
	  توڑو;
	حالت Op_رمز_rmoveto:
	  has_چوڑائی = (env.arg_ڈھیر.get_گنتی () > 2);
	  توڑو;
	طےشدہ:
	  واپس;
      }
      env.سیٹ_چوڑائی (has_چوڑائی);
    }
  }

  ساکن خالی process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساکن خالی flush_دلائل (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_دلائل (env, param);
    env.clear_دلائل ();  /* pop off width */
  }

  نجی:
  قسمتعریف cs_opset_t<نمبر_t, OPSET, cff1_cs_interp_env_t, PARAM, راستہ_3>  SUPER;
};

سانچہ <قسم_نام OPSET, قسم_نام PARAM>
استعمال cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#اگر_ختم /* HB_CFF1_INTERP_CS_HH */
