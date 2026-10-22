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
#अगर_अघोषित HB_CFF_INTERP_DICT_COMMON_HH
#घोषणा HB_CFF_INTERP_DICT_COMMON_HH

#समावेश "hb-cff-interp-common.hh"

namespace CFF {

using namespace OT;

/* an opstr and the parsed out dict value(s) */
काष्ठा dict_val_t : op_str_t
{
  व्योम init () {}
  व्योम fini () {}
};

प्रकार dict_val_t अंक_dict_val_t;

ढाँचा <typename VAL> काष्ठा dict_मूल्यावली_t : parsed_मूल्यावली_t<VAL> {};

ढाँचा <typename OPSTR=op_str_t>
काष्ठा शीर्ष_dict_मूल्यावली_t : dict_मूल्यावली_t<OPSTR>
{
  व्योम init ()
  {
    dict_मूल्यावली_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  व्योम fini () { dict_मूल्यावली_t<OPSTR>::fini (); }

  पूर्णांक  charStringsOffset;
  पूर्णांक  FDArrayOffset;
};

काष्ठा dict_opset_t : opset_t<संख्या_t>
{
  अटल व्योम process_op (op_code_t op, interp_env_t<संख्या_t>& env)
  {
    चयन (op) {
      हाल OpCode_longintdict:  /* 5-byte integer */
	env.arg_ढेर.धकेलो_longint_from_substr (env.str_ref);
	अवरोध;

      हाल OpCode_BCD:  /* real number */
	env.arg_ढेर.धकेलो_real (parse_bcd (env.str_ref));
	अवरोध;

      शेष:
	opset_t<संख्या_t>::process_op (op, env);
	अवरोध;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  अटल द्विगुन parse_bcd (byte_str_ref_t& str_ref)
  {
    अगर (unlikely (str_ref.in_error ())) वापस .0;

    क्रमागत Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    अक्षर buf[32] = {0};
    अचिन्हित अक्षर byte = 0;
    क्रम (अचिन्हित i = 0, गिनती = 0; गिनती < ARRAY_लंबाई (buf); ++i, ++गिनती)
    {
      अचिन्हित nibble;
      अगर (!(i & 1))
      {
	अगर (unlikely (!str_ref.avail ())) अवरोध;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      अन्यथा
	nibble = byte & 0x0F;

      अगर (unlikely (nibble == RESERVED)) अवरोध;
      अन्यथा अगर (nibble == END)
      {
	स्थिर अक्षर *p = buf;
	द्विगुन pv;
	अगर (unlikely (!hb_parse_double (&p, p + गिनती, &pv, true/* whole buffer */)))
	  अवरोध;
	वापस pv;
      }
      अन्यथा
      {
	buf[गिनती] = "0123456789.EE?-?"[nibble];
	अगर (nibble == EXP_NEG)
	{
	  ++गिनती;
	  अगर (unlikely (गिनती == ARRAY_लंबाई (buf))) अवरोध;
	  buf[गिनती] = '-';
	}
      }
    }

    str_ref.set_error ();
    वापस .0;
  }

  अटल bool is_hint_op (op_code_t op)
  {
    चयन (op)
    {
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_ForceBold:
      हाल OpCode_LanguageGroup:
      हाल OpCode_ExpansionFactor:
	वापस true;
      शेष:
	वापस false;
    }
  }
};

ढाँचा <typename VAL=op_str_t>
काष्ठा शीर्ष_dict_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, interp_env_t<संख्या_t>& env, शीर्ष_dict_मूल्यावली_t<VAL> & dictval)
  {
    चयन (op) {
      हाल OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_FontMatrix:
	env.clear_args ();
	अवरोध;
      शेष:
	dict_opset_t::process_op (op, env);
	अवरोध;
    }
  }
};

ढाँचा <typename OPSET, typename PARAM, typename ENV=अंक_interp_env_t>
काष्ठा dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  bool interpret (PARAM& param)
  {
    param.init ();
    जबतक (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      अगर (unlikely (SUPER::env.in_error ()))
	वापस false;
    }

    वापस true;
  }

  निजी:
  प्रकार interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF_INTERP_DICT_COMMON_HH */
