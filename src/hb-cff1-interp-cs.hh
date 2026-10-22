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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-cff-interp-cs-common.hh"

നാമാകാശ CFF {

using നാമാകാശ OT;

തരനിർവചനം biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ഘടന cff1_cs_interp_env_t : cs_interp_env_t<സംഖ്യ_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (സ്ഥിരം hb_ubytes_t &str, ACC &acc, unsigned പൂർണ്ണസംഖ്യ fd,
			സ്ഥിരം പൂർണ്ണസംഖ്യ *coords_=nullptr, unsigned പൂർണ്ണസംഖ്യ അക്കം_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = അസത്യം;
    has_width = അസത്യം;
    arg_start = 0;
    in_seac = അസത്യം;
  }

  ശൂന്യം set_width (തർക്കം has_width_)
  {
    എങ്കിൽ (likely (!processed_width && (SUPER::arg_അട്ടി.get_എണ്ണം () > 0)))
    {
      എങ്കിൽ (has_width_)
      {
	width = SUPER::arg_അട്ടി[0];
	has_width = സത്യം;
	arg_start = 1;
      }
    }
    processed_width = സത്യം;
  }

  ശൂന്യം clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  ശൂന്യം set_in_seac (തർക്കം _in_seac) { in_seac = _in_seac; }

  തർക്കം	  processed_width;
  തർക്കം	  has_width;
  unsigned പൂർണ്ണസംഖ്യ  arg_start;
  സംഖ്യ_t	width;
  തർക്കം	  in_seac;

  സ്വകാര്യ:
  തരനിർവചനം cs_interp_env_t<സംഖ്യ_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ഘടന cff1_cs_opset_t : cs_opset_t<സംഖ്യ_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_endchar:
	OPSET::check_width (op, env, param);
	എങ്കിൽ (env.arg_അട്ടി.get_എണ്ണം () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (സത്യം);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env, param);
    }
  }

  സ്ഥിതം ശൂന്യം check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    എങ്കിൽ (!env.processed_width)
    {
      തർക്കം  has_width = അസത്യം;
      തിരഞ്ഞെടുപ്പ് (op)
      {
	അവസ്ഥ OpCode_endchar:
	അവസ്ഥ OpCode_hstem:
	അവസ്ഥ OpCode_hstemhm:
	അവസ്ഥ OpCode_vstem:
	അവസ്ഥ OpCode_vstemhm:
	അവസ്ഥ OpCode_hintmask:
	അവസ്ഥ OpCode_cntrmask:
	  has_width = ((env.arg_അട്ടി.get_എണ്ണം () & 1) != 0);
	  നിർത്തുക;
	അവസ്ഥ OpCode_hmoveto:
	അവസ്ഥ OpCode_vmoveto:
	  has_width = (env.arg_അട്ടി.get_എണ്ണം () > 1);
	  നിർത്തുക;
	അവസ്ഥ OpCode_rmoveto:
	  has_width = (env.arg_അട്ടി.get_എണ്ണം () > 2);
	  നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
	  തിരികെ;
      }
      env.set_width (has_width);
    }
  }

  സ്ഥിതം ശൂന്യം process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  സ്ഥിതം ശൂന്യം flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  സ്വകാര്യ:
  തരനിർവചനം cs_opset_t<സംഖ്യ_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
