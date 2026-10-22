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
#که_نه_تعریف_شوی HB_CFF1_INTERP_CS_HH
#تعریف_کړه HB_CFF1_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

کارول نطاق OT;

تعريفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

بنية cff1_cs_interp_env_t : cs_interp_env_t<شمېره_t, CFF1Subrs>
{
  کينډۍ <ډول_نوم ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بینښه صحيح fd,
			ثابت صحيح *coords_=تش_اشاره, بینښه صحيح عدد_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.شخصي_Dicts[fd].localSubrs)
  {
    processed_پلنوالی = خطأ;
    has_پلنوالی = خطأ;
    arg_پیل = 0;
    دننه_seac = خطأ;
  }

  فراغ سېټ_پلنوالی (منطقي has_پلنوالی_2)
  {
    إذا (likely (!processed_پلنوالی && (SUPER::arg_ډېری.get_شمېر () > 0)))
    {
      إذا (has_پلنوالی_2)
      {
	پلنوالی = SUPER::arg_ډېری[0];
	has_پلنوالی = ریښتیا;
	arg_پیل = 1;
      }
    }
    processed_پلنوالی = ریښتیا;
  }

  فراغ clear_دلیلونه ()
  {
    arg_پیل = 0;
    SUPER::clear_دلیلونه ();
  }

  فراغ سېټ_دننه_seac (منطقي دننه_seac_2) { دننه_seac = دننه_seac_2; }

  منطقي	  processed_پلنوالی;
  منطقي	  has_پلنوالی;
  بینښه صحيح  arg_پیل;
  شمېره_t	پلنوالی;
  منطقي	  دننه_seac;

  خاص:
  تعريفنوع cs_interp_env_t<شمېره_t, CFF1Subrs> SUPER;
};

کينډۍ <ډول_نوم OPSET, ډول_نوم PARAM, ډول_نوم لار_3=لار_procs_null_t<cff1_cs_interp_env_t, PARAM>>
بنية cff1_cs_opset_t : cs_opset_t<شمېره_t, OPSET, cff1_cs_interp_env_t, PARAM, لار_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساكن فراغ process_op (op_کوډ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کوډ_dotsection:
	SUPER::flush_دلیلونه_and_op (op, env, param);
	اكسر;

      حالة Op_کوډ_endchar:
	OPSET::check_پلنوالی (op, env, param);
	إذا (env.arg_ډېری.get_شمېر () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_دلیلونه_and_op (op, env, param);
	env.سېټ_endchar (ریښتیا);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  ساكن فراغ check_پلنوالی (op_کوډ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    إذا (!env.processed_پلنوالی)
    {
      منطقي  has_پلنوالی = خطأ;
      بدل (op)
      {
	حالة Op_کوډ_endchar:
	حالة Op_کوډ_hstem:
	حالة Op_کوډ_hstemhm:
	حالة Op_کوډ_vstem:
	حالة Op_کوډ_vstemhm:
	حالة Op_کوډ_hintmask:
	حالة Op_کوډ_cntrmask:
	  has_پلنوالی = ((env.arg_ډېری.get_شمېر () & 1) != 0);
	  اكسر;
	حالة Op_کوډ_hmoveto:
	حالة Op_کوډ_vmoveto:
	  has_پلنوالی = (env.arg_ډېری.get_شمېر () > 1);
	  اكسر;
	حالة Op_کوډ_rmoveto:
	  has_پلنوالی = (env.arg_ډېری.get_شمېر () > 2);
	  اكسر;
	افتراضي:
	  أرجع;
      }
      env.سېټ_پلنوالی (has_پلنوالی);
    }
  }

  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساكن فراغ flush_دلیلونه (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_دلیلونه (env, param);
    env.clear_دلیلونه ();  /* pop off width */
  }

  خاص:
  تعريفنوع cs_opset_t<شمېره_t, OPSET, cff1_cs_interp_env_t, PARAM, لار_3>  SUPER;
};

کينډۍ <ډول_نوم OPSET, ډول_نوم PARAM>
کارول cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#د_که_پای /* HB_CFF1_INTERP_CS_HH */
