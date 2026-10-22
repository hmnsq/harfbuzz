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
#अगर_अघोषित HB_CFF1_INTERP_CS_HH
#घोषणा HB_CFF1_INTERP_CS_HH

#समावेश "hb.hh"
#समावेश "hb-cff-interp-cs-common.hh"

namespace CFF {

using namespace OT;

प्रकार biased_subrs_t<CFF1Subrs>   cff1_biased_subrs_t;

काष्ठा cff1_cs_interp_env_t : cs_interp_env_t<संख्या_t, CFF1Subrs>
{
  ढाँचा <typename ACC>
  cff1_cs_interp_env_t (स्थिर hb_ubytes_t &str, ACC &acc, अचिन्हित पूर्णांक fd,
			स्थिर पूर्णांक *coords_=nullptr, अचिन्हित पूर्णांक अंक_coords_2=0)
    : SUPER (str, acc.globalSubrs, acc.privateDicts[fd].localSubrs)
  {
    processed_width = false;
    has_width = false;
    arg_start = 0;
    in_seac = false;
  }

  व्योम set_width (bool has_width_)
  {
    अगर (likely (!processed_width && (SUPER::arg_ढेर.get_गिनती () > 0)))
    {
      अगर (has_width_)
      {
	width = SUPER::arg_ढेर[0];
	has_width = true;
	arg_start = 1;
      }
    }
    processed_width = true;
  }

  व्योम clear_args ()
  {
    arg_start = 0;
    SUPER::clear_args ();
  }

  व्योम set_in_seac (bool _in_seac) { in_seac = _in_seac; }

  bool	  processed_width;
  bool	  has_width;
  अचिन्हित पूर्णांक  arg_start;
  संख्या_t	width;
  bool	  in_seac;

  निजी:
  प्रकार cs_interp_env_t<संख्या_t, CFF1Subrs> SUPER;
};

ढाँचा <typename OPSET, typename PARAM, typename PATH=path_procs_null_t<cff1_cs_interp_env_t, PARAM>>
काष्ठा cff1_cs_opset_t : cs_opset_t<संख्या_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>
{
  /* PostScript-originated legacy opcodes (OpCode_add etc) are unsupported */
  /* Type 1-originated deprecated opcodes, seac behavior of endchar and dotsection are supported */

  अटल व्योम process_op (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    चयन (op) {
      हाल OpCode_dotsection:
	SUPER::flush_args_and_op (op, env, param);
	अवरोध;

      हाल OpCode_endchar:
	OPSET::check_width (op, env, param);
	अगर (env.arg_ढेर.get_गिनती () >= 4)
	{
	  OPSET::process_seac (env, param);
	}
	OPSET::flush_args_and_op (op, env, param);
	env.set_endchar (true);
	अवरोध;

      शेष:
	SUPER::process_op (op, env, param);
    }
  }

  अटल व्योम check_width (op_code_t op, cff1_cs_interp_env_t &env, PARAM& param)
  {
    अगर (!env.processed_width)
    {
      bool  has_width = false;
      चयन (op)
      {
	हाल OpCode_endchar:
	हाल OpCode_hstem:
	हाल OpCode_hstemhm:
	हाल OpCode_vstem:
	हाल OpCode_vstemhm:
	हाल OpCode_hintmask:
	हाल OpCode_cntrmask:
	  has_width = ((env.arg_ढेर.get_गिनती () & 1) != 0);
	  अवरोध;
	हाल OpCode_hmoveto:
	हाल OpCode_vmoveto:
	  has_width = (env.arg_ढेर.get_गिनती () > 1);
	  अवरोध;
	हाल OpCode_rmoveto:
	  has_width = (env.arg_ढेर.get_गिनती () > 2);
	  अवरोध;
	शेष:
	  वापस;
      }
      env.set_width (has_width);
    }
  }

  अटल व्योम process_seac (cff1_cs_interp_env_t &env, PARAM& param)
  {
  }

  अटल व्योम flush_args (cff1_cs_interp_env_t &env, PARAM& param)
  {
    SUPER::flush_args (env, param);
    env.clear_args ();  /* pop off width */
  }

  निजी:
  प्रकार cs_opset_t<संख्या_t, OPSET, cff1_cs_interp_env_t, PARAM, PATH>  SUPER;
};

ढाँचा <typename OPSET, typename PARAM>
using cff1_cs_interpreter_t = cs_interpreter_t<cff1_cs_interp_env_t, OPSET, PARAM>;

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF1_INTERP_CS_HH */
