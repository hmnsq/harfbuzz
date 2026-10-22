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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-cff-interp-cs-common.hh"

নামাকাশ CFF {

using নামাকাশ OT;

প্রকারলেখ biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

কাঠামো cff1_cs_interp_env_t : cs_interp_env_t<সংখ্যা_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ধ্রুবক hb_ubytes_t &str, ACC &acc, unsigned পূর্ণসংখ্যা fd,
			ধ্রুবক পূর্ণসংখ্যা *coords_=nullptr, unsigned পূর্ণসংখ্যা অঙ্ক_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = মিথ্যা;
    has_width = মিথ্যা;
    arg_start = 0;
    in_seac = মিথ্যা;
  }

  শূন্য set_width (যৌক্তিক has_width_)
  {
    যদি (likely (!processed_width && (SUPER::arg_স্তূপ.get_গুনতি () > 0)))
    {
      যদি (has_width_)
      {
	width = SUPER::arg_স্তূপ[0];
	has_width = সত্য;
	arg_start = 1;
      }
    }
    processed_width = সত্য;
  }

  শূন্য clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  শূন্য set_in_seac (যৌক্তিক _in_seac) { in_seac = _in_seac; }

  যৌক্তিক	  processed_width;
  যৌক্তিক	  has_width;
  unsigned পূর্ণসংখ্যা  arg_start;
  সংখ্যা_t	width;
  যৌক্তিক	  in_seac;

  ব্যক্তিগত:
  প্রকারলেখ cs_interp_env_t<সংখ্যা_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
কাঠামো cff1_cs_opset_t : cs_opset_t<সংখ্যা_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  স্থির শূন্য process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	বিরতি;

      ক্ষেত্র OpCode_endchar:
	OPSET::check_width (op, env, param);
	যদি (env.arg_স্তূপ.get_গুনতি () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (সত্য);
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env, param);
    }
  }

  স্থির শূন্য check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    যদি (!env.processed_width)
    {
      যৌক্তিক  has_width = মিথ্যা;
      নির্বাচন (op)
      {
	ক্ষেত্র OpCode_endchar:
	ক্ষেত্র OpCode_hstem:
	ক্ষেত্র OpCode_hstemhm:
	ক্ষেত্র OpCode_vstem:
	ক্ষেত্র OpCode_vstemhm:
	ক্ষেত্র OpCode_hintmask:
	ক্ষেত্র OpCode_cntrmask:
	  has_width = ((env.arg_স্তূপ.get_গুনতি () & 1) != 0);
	  বিরতি;
	ক্ষেত্র OpCode_hmoveto:
	ক্ষেত্র OpCode_vmoveto:
	  has_width = (env.arg_স্তূপ.get_গুনতি () > 1);
	  বিরতি;
	ক্ষেত্র OpCode_rmoveto:
	  has_width = (env.arg_স্তূপ.get_গুনতি () > 2);
	  বিরতি;
	পূর্বনির্ধারিত:
	  ফেরত;
      }
      env.set_width (has_width);
    }
  }

  স্থির শূন্য process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  স্থির শূন্য flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  ব্যক্তিগত:
  প্রকারলেখ cs_opset_t<সংখ্যা_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
