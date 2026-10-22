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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-cff-interp-cs-common.hh"

નામાકાશ CFF {

using નામાકાશ OT;

પ્રકારવ્યાખ્યા biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

સંરચના cff1_cs_interp_env_t : cs_interp_env_t<સંખ્યા_t, CFF1Subrs>
{
  template <typename ACC>
  cff1_cs_interp_env_t (અચલ hb_ubytes_t &str, ACC &acc, unsigned પૂર્ણાંક fd,
			અચલ પૂર્ણાંક *coords_=nullptr, unsigned પૂર્ણાંક અંક_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = ખોટું;
    has_width = ખોટું;
    arg_start = 0;
    in_seac = ખોટું;
  }

  શૂન્ય set_width (તાર્કિક has_width_)
  {
    જો (likely (!processed_width && (SUPER::arg_ઢગલો.get_ગણતરી () > 0)))
    {
      જો (has_width_)
      {
	width = SUPER::arg_ઢગલો[0];
	has_width = સાચું;
	arg_start = 1;
      }
    }
    processed_width = સાચું;
  }

  શૂન્ય clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  શૂન્ય set_in_seac (તાર્કિક _in_seac) { in_seac = _in_seac; }

  તાર્કિક	  processed_width;
  તાર્કિક	  has_width;
  unsigned પૂર્ણાંક  arg_start;
  સંખ્યા_t	width;
  તાર્કિક	  in_seac;

  ખાનગી:
  પ્રકારવ્યાખ્યા cs_interp_env_t<સંખ્યા_t, CFF1Subrs> SUPER;
};

template <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
સંરચના cff1_cs_opset_t : cs_opset_t<સંખ્યા_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  સ્થિર શૂન્ય process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	તોડો;

      સ્થિતિ OpCode_endchar:
	OPSET::check_width (op, env, param);
	જો (env.arg_ઢગલો.get_ગણતરી () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (સાચું);
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env, param);
    }
  }

  સ્થિર શૂન્ય check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    જો (!env.processed_width)
    {
      તાર્કિક  has_width = ખોટું;
      પસંદગી (op)
      {
	સ્થિતિ OpCode_endchar:
	સ્થિતિ OpCode_hstem:
	સ્થિતિ OpCode_hstemhm:
	સ્થિતિ OpCode_vstem:
	સ્થિતિ OpCode_vstemhm:
	સ્થિતિ OpCode_hintmask:
	સ્થિતિ OpCode_cntrmask:
	  has_width = ((env.arg_ઢગલો.get_ગણતરી () & 1) != 0);
	  તોડો;
	સ્થિતિ OpCode_hmoveto:
	સ્થિતિ OpCode_vmoveto:
	  has_width = (env.arg_ઢગલો.get_ગણતરી () > 1);
	  તોડો;
	સ્થિતિ OpCode_rmoveto:
	  has_width = (env.arg_ઢગલો.get_ગણતરી () > 2);
	  તોડો;
	મૂળભૂત:
	  પાછા;
      }
      env.set_width (has_width);
    }
  }

  સ્થિર શૂન્ય process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  સ્થિર શૂન્ય flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cs_opset_t<સંખ્યા_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

template <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#endif /* HB_CFF1_INTERP_CS_HH */
