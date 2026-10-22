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
#اگر_تعریف_نشده HB_CFF1_INTERP_CS_HH
#تعریف_کن HB_CFF1_INTERP_CS_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

استفاده نطاق OT;

تعريفنوع biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

بنية cff1_cs_interp_env_t : cs_interp_env_t<عدد_t, CFF1Subrs>
{
  قالب <نام_نوع ACC>
  cff1_cs_interp_env_t (ثابت hb_ubytes_t &str, ACC &acc, بیعلامت صحيح fd,
			ثابت صحيح *coords_=اشاره_تهی, بیعلامت صحيح رقم_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.خصوصی_Dicts[fd].localSubrs)
  {
    processed_عرض = خطأ;
    has_عرض = خطأ;
    arg_شروع = 0;
    درون_seac = خطأ;
  }

  فراغ مجموعه_عرض (منطقي has_عرض_2)
  {
    إذا (likely (!processed_عرض && (SUPER::arg_پشته.get_شمار () > 0)))
    {
      إذا (has_عرض_2)
      {
	عرض = SUPER::arg_پشته[0];
	has_عرض = درست;
	arg_شروع = 1;
      }
    }
    processed_عرض = درست;
  }

  فراغ clear_آرگومانها ()
  {
    arg_شروع = 0;
    SUPER::clear_آرگومانها ();
  }

  فراغ مجموعه_درون_seac (منطقي درون_seac_2) { درون_seac = درون_seac_2; }

  منطقي	  processed_عرض;
  منطقي	  has_عرض;
  بیعلامت صحيح  arg_شروع;
  عدد_t	عرض;
  منطقي	  درون_seac;

  خاص:
  تعريفنوع cs_interp_env_t<عدد_t, CFF1Subrs> SUPER;
};

قالب <نام_نوع OPSET, نام_نوع PARAM, نام_نوع مسیر_3=مسیر_procs_null_t<cff1_cs_interp_env_t, PARAM>>
بنية cff1_cs_opset_t : cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, مسیر_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ساكن فراغ process_op (op_کد_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    بدل (op) {
      حالة Op_کد_dotsection:
	SUPER::flush_آرگومانها_and_op (op, env, param);
	اكسر;

      حالة Op_کد_endchar:
	OPSET::check_عرض (op, env, param);
	إذا (env.arg_پشته.get_شمار () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_آرگومانها_and_op (op, env, param);
	env.مجموعه_endchar (درست);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
    }
  }

  ساكن فراغ check_عرض (op_کد_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    إذا (!env.processed_عرض)
    {
      منطقي  has_عرض = خطأ;
      بدل (op)
      {
	حالة Op_کد_endchar:
	حالة Op_کد_hstem:
	حالة Op_کد_hstemhm:
	حالة Op_کد_vstem:
	حالة Op_کد_vstemhm:
	حالة Op_کد_hintmask:
	حالة Op_کد_cntrmask:
	  has_عرض = ((env.arg_پشته.get_شمار () & 1) != 0);
	  اكسر;
	حالة Op_کد_hmoveto:
	حالة Op_کد_vmoveto:
	  has_عرض = (env.arg_پشته.get_شمار () > 1);
	  اكسر;
	حالة Op_کد_rmoveto:
	  has_عرض = (env.arg_پشته.get_شمار () > 2);
	  اكسر;
	افتراضي:
	  أرجع;
      }
      env.مجموعه_عرض (has_عرض);
    }
  }

  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ساكن فراغ flush_آرگومانها (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_آرگومانها (env, param);
    env.clear_آرگومانها ();  /* pop off width */
  }

  خاص:
  تعريفنوع cs_opset_t<عدد_t, OPSET, cff1_cs_interp_env_t, PARAM, مسیر_3>  SUPER;
};

قالب <نام_نوع OPSET, نام_نوع PARAM>
استفاده cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#پایان_اگر /* HB_CFF1_INTERP_CS_HH */
