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
#ifndef HB_CFF_INTERP_DICT_COMMON_HH
#define HB_CFF_INTERP_DICT_COMMON_HH

#अन्तर्भाव "hb-cff-interp-common.hh"

नामाकाश CFF {

using नामाकाश OT;

/* an opstr and the parsed out dict value(s) */
संरचना dict_val_t : op_str_t
{
  शून्य init () {}
  शून्य fini () {}
};

प्रकारनिर्देश dict_val_t अङ्कः_dict_val_t;

template <typename VAL> संरचना dict_मानानि_t : parsed_मानानि_t<VAL> {};

template <typename OPSTR=op_str_t>
संरचना शिखरम्_dict_मानानि_t : dict_मानानि_t<OPSTR>
{
  शून्य init ()
  {
    dict_मानानि_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  शून्य fini () { dict_मानानि_t<OPSTR>::fini (); }

  पूर्णाङ्क  charStringsOffset;
  पूर्णाङ्क  FDArrayOffset;
};

संरचना dict_opset_t : opset_t<सङ्ख्या_t>
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<सङ्ख्या_t>& env)
  {
    विकल्प (op) {
      पक्ष OpCode_longintdict:  /* 5-byte integer */
	env.arg_राशिः.प्रक्षिप_longint_from_substr (env.str_ref);
	भङ्ग;

      पक्ष OpCode_BCD:  /* real number */
	env.arg_राशिः.प्रक्षिप_real (parse_bcd (env.str_ref));
	भङ्ग;

      स्वभाव:
	opset_t<सङ्ख्या_t>::process_op (op, env);
	भङ्ग;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  स्थिर द्विगुण parse_bcd (byte_str_ref_t& str_ref)
  {
    जइ (unlikely (str_ref.in_error ())) प्रत्यागम .0;

    गणना Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    वर्ण buf[32] = {0};
    unsigned वर्ण byte = 0;
    प्रति (unsigned i = 0, गणनम् = 0; गणनम् < ARRAY_LENGTH (buf); ++i, ++गणनम्)
    {
      unsigned nibble;
      जइ (!(i & 1))
      {
	जइ (unlikely (!str_ref.avail ())) भङ्ग;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      अण्णहा
	nibble = byte & 0x0F;

      जइ (unlikely (nibble == RESERVED)) भङ्ग;
      अण्णहा जइ (nibble == END)
      {
	ध्रुव वर्ण *p = buf;
	द्विगुण pv;
	जइ (unlikely (!hb_parse_double (&p, p + गणनम्, &pv, सच्च/* whole buffer */)))
	  भङ्ग;
	प्रत्यागम pv;
      }
      अण्णहा
      {
	buf[गणनम्] = "0123456789.EE?-?"[nibble];
	जइ (nibble == EXP_NEG)
	{
	  ++गणनम्;
	  जइ (unlikely (गणनम् == ARRAY_LENGTH (buf))) भङ्ग;
	  buf[गणनम्] = '-';
	}
      }
    }

    str_ref.set_error ();
    प्रत्यागम .0;
  }

  स्थिर तार्किक is_hint_op (op_code_t op)
  {
    विकल्प (op)
    {
      पक्ष Op_Code_Blue_मानानि:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_ForceBold:
      पक्ष OpCode_LanguageGroup:
      पक्ष OpCode_ExpansionFactor:
	प्रत्यागम सच्च;
      स्वभाव:
	प्रत्यागम मिच्छ;
    }
  }
};

template <typename VAL=op_str_t>
संरचना शिखरम्_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<सङ्ख्या_t>& env, शिखरम्_dict_मानानि_t<VAL> & dictval)
  {
    विकल्प (op) {
      पक्ष OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_FontMatrix:
	env.clear_args ();
	भङ्ग;
      स्वभाव:
	dict_opset_t::process_op (op, env);
	भङ्ग;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=अङ्कः_interp_env_t>
संरचना dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  तार्किक interpret (PARAM& param)
  {
    param.init ();
    जाव (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      जइ (unlikely (SUPER::env.in_error ()))
	प्रत्यागम मिच्छ;
    }

    प्रत्यागम सच्च;
  }

  गुप्त:
  प्रकारनिर्देश interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
