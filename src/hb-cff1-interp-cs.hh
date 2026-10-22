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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF1_INTERP_CS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF1_INTERP_CS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-cs-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

ਬਣਤਰ cff1_cs_interp_env_t : cs_interp_env_t<ਸੰਖਿਆ_t, CFF1Subrs>
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ACC>
  cff1_cs_interp_env_t (ਅਚਲ hb_ubytes_t &str, ACC &acc, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd,
			ਅਚਲ ਪੂਰਨ *coords_=ਖਾਲੀ_ਸੂਚਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.ਨਿੱਜੀ_Dicts[fd].localSubrs)
  {
    processed_ਚੌੜਾਈ = ਝੂਠ;
    has_ਚੌੜਾਈ = ਝੂਠ;
    arg_ਸ਼ੁਰੂ = 0;
    ਵਿੱਚ_seac = ਝੂਠ;
  }

  ਖਾਲੀ ਸੈੱਟ_ਚੌੜਾਈ (ਤਾਰਕਿਕ has_ਚੌੜਾਈ_2)
  {
    ਜੇ (likely (!processed_ਚੌੜਾਈ && (SUPER::arg_ਢੇਰ.get_ਗਿਣਤੀ () > 0)))
    {
      ਜੇ (has_ਚੌੜਾਈ_2)
      {
	ਚੌੜਾਈ = SUPER::arg_ਢੇਰ[0];
	has_ਚੌੜਾਈ = ਸੱਚ;
	arg_ਸ਼ੁਰੂ = 1;
      }
    }
    processed_ਚੌੜਾਈ = ਸੱਚ;
  }

  ਖਾਲੀ clear_ਦਲੀਲਾਂ ()
  {
    arg_ਸ਼ੁਰੂ = 0;
    SUPER::clear_ਦਲੀਲਾਂ ();
  }

  ਖਾਲੀ ਸੈੱਟ_ਵਿੱਚ_seac (ਤਾਰਕਿਕ ਵਿੱਚ_seac_2) { ਵਿੱਚ_seac = ਵਿੱਚ_seac_2; }

  ਤਾਰਕਿਕ	  processed_ਚੌੜਾਈ;
  ਤਾਰਕਿਕ	  has_ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  arg_ਸ਼ੁਰੂ;
  ਸੰਖਿਆ_t	ਚੌੜਾਈ;
  ਤਾਰਕਿਕ	  ਵਿੱਚ_seac;

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cs_interp_env_t<ਸੰਖਿਆ_t, CFF1Subrs> SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM, ਕਿਸਮ_ਨਾਂ ਰਸਤਾ_3=ਰਸਤਾ_procs_null_t<cff1_cs_interp_env_t, PARAM>>
ਬਣਤਰ cff1_cs_opset_t : cs_opset_t<ਸੰਖਿਆ_t, OPSET, cff1_cs_interp_env_t, PARAM, ਰਸਤਾ_3>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_dotsection:
	SUPER::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_endchar:
	OPSET::check_ਚੌੜਾਈ (op, env, param);
	ਜੇ (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
	env.ਸੈੱਟ_endchar (ਸੱਚ);
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env, param);
    }
  }

  ਸਥਿਰ ਖਾਲੀ check_ਚੌੜਾਈ (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    ਜੇ (!env.processed_ਚੌੜਾਈ)
    {
      ਤਾਰਕਿਕ  has_ਚੌੜਾਈ = ਝੂਠ;
      ਚੋਣ (op)
      {
	ਹਾਲਤ Op_ਕੋਡ_endchar:
	ਹਾਲਤ Op_ਕੋਡ_hstem:
	ਹਾਲਤ Op_ਕੋਡ_hstemhm:
	ਹਾਲਤ Op_ਕੋਡ_vstem:
	ਹਾਲਤ Op_ਕੋਡ_vstemhm:
	ਹਾਲਤ Op_ਕੋਡ_hintmask:
	ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	  has_ਚੌੜਾਈ = ((env.arg_ਢੇਰ.get_ਗਿਣਤੀ () & 1) != 0);
	  ਰੋਕੋ;
	ਹਾਲਤ Op_ਕੋਡ_hmoveto:
	ਹਾਲਤ Op_ਕੋਡ_vmoveto:
	  has_ਚੌੜਾਈ = (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () > 1);
	  ਰੋਕੋ;
	ਹਾਲਤ Op_ਕੋਡ_rmoveto:
	  has_ਚੌੜਾਈ = (env.arg_ਢੇਰ.get_ਗਿਣਤੀ () > 2);
	  ਰੋਕੋ;
	ਮੂਲ:
	  ਵਾਪਸ;
      }
      env.ਸੈੱਟ_ਚੌੜਾਈ (has_ਚੌੜਾਈ);
    }
  }

  ਸਥਿਰ ਖਾਲੀ process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_ਦਲੀਲਾਂ (env, param);
    env.clear_ਦਲੀਲਾਂ ();  /* pop off width */
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cs_opset_t<ਸੰਖਿਆ_t, OPSET, cff1_cs_interp_env_t, PARAM, ਰਸਤਾ_3>  SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM>
ਵਰਤੋਂ cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF1_INTERP_CS_HH */
