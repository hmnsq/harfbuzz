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

প্রকারনির্দেশ biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

সংরচনা cff1_cs_interp_env_t : cs_interp_env_t<সংখ্যা_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (ধ্রুব hb_ubytes_t &str, ACC &acc, unsigned পূর্ণাঙ্ক fd,
			ধ্রুব পূর্ণাঙ্ক *coords_=nullptr, unsigned পূর্ণাঙ্ক অঙ্ক_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = অসত্য;
    has_width = অসত্য;
    arg_start = 0;
    in_seac = অসত্য;
  }

  শূন্য set_width (তার্কিক has_width_)
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

  শূন্য set_in_seac (তার্কিক _in_seac) { in_seac = _in_seac; }

  তার্কিক	  processed_width;
  তার্কিক	  has_width;
  unsigned পূর্ণাঙ্ক  arg_start;
  সংখ্যা_t	width;
  তার্কিক	  in_seac;

  গুপ্ত:
  প্রকারনির্দেশ cs_interp_env_t<সংখ্যা_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
সংরচনা cff1_cs_opset_t : cs_opset_t<সংখ্যা_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  স্থির শূন্য process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    বিকল্প (op) {
      পক্ষ OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_endchar:
	OPSET::check_width (op, env, param);
	যদি (env.arg_স্তূপ.get_গুনতি () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (সত্য);
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env, param);
    }
  }

  স্থির শূন্য check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    যদি (!env.processed_width)
    {
      তার্কিক  has_width = অসত্য;
      বিকল্প (op)
      {
	পক্ষ OpCode_endchar:
	পক্ষ OpCode_hstem:
	পক্ষ OpCode_hstemhm:
	পক্ষ OpCode_vstem:
	পক্ষ OpCode_vstemhm:
	পক্ষ OpCode_hintmask:
	পক্ষ OpCode_cntrmask:
	  has_width = ((env.arg_স্তূপ.get_গুনতি () & 1) != 0);
	  ভঙ্গ;
	পক্ষ OpCode_hmoveto:
	পক্ষ OpCode_vmoveto:
	  has_width = (env.arg_স্তূপ.get_গুনতি () > 1);
	  ভঙ্গ;
	পক্ষ OpCode_rmoveto:
	  has_width = (env.arg_স্তূপ.get_গুনতি () > 2);
	  ভঙ্গ;
	স্বভাব:
	  প্রত্যাগম;
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

  গুপ্ত:
  প্রকারনির্দেশ cs_opset_t<সংখ্যা_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
