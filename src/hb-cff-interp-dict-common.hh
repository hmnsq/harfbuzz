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

प्रकारव्याख्या dict_val_t अंक_dict_val_t;

ढाँचा <typename VAL> संरचना dict_मूल्ये_t : parsed_मूल्ये_t<VAL> {};

ढाँचा <typename OPSTR=op_str_t>
संरचना शीर्ष_dict_मूल्ये_t : dict_मूल्ये_t<OPSTR>
{
  शून्य init ()
  {
    dict_मूल्ये_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  शून्य fini () { dict_मूल्ये_t<OPSTR>::fini (); }

  पूर्णांक  charStringsOffset;
  पूर्णांक  FDArrayOffset;
};

संरचना dict_opset_t : opset_t<संख्या_t>
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<संख्या_t>& env)
  {
    निवड (op) {
      स्थिती OpCode_longintdict:  /* 5-byte integer */
	env.arg_चळत.ढकला_longint_from_substr (env.str_ref);
	थांबा;

      स्थिती OpCode_BCD:  /* real number */
	env.arg_चळत.ढकला_real (parse_bcd (env.str_ref));
	थांबा;

      पूर्वनिर्धारित:
	opset_t<संख्या_t>::process_op (op, env);
	थांबा;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  स्थिर द्विगुण parse_bcd (byte_str_ref_t& str_ref)
  {
    जर (unlikely (str_ref.in_error ())) परत .0;

    गणना Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    अक्षर buf[32] = {0};
    अचिन्हित अक्षर byte = 0;
    साठी (अचिन्हित i = 0, मोजणी = 0; मोजणी < ARRAY_LENGTH (buf); ++i, ++मोजणी)
    {
      अचिन्हित nibble;
      जर (!(i & 1))
      {
	जर (unlikely (!str_ref.avail ())) थांबा;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      अन्यथा
	nibble = byte & 0x0F;

      जर (unlikely (nibble == RESERVED)) थांबा;
      अन्यथा जर (nibble == END)
      {
	स्थिरांक अक्षर *p = buf;
	द्विगुण pv;
	जर (unlikely (!hb_parse_double (&p, p + मोजणी, &pv, सत्य/* whole buffer */)))
	  थांबा;
	परत pv;
      }
      अन्यथा
      {
	buf[मोजणी] = "0123456789.EE?-?"[nibble];
	जर (nibble == EXP_NEG)
	{
	  ++मोजणी;
	  जर (unlikely (मोजणी == ARRAY_LENGTH (buf))) थांबा;
	  buf[मोजणी] = '-';
	}
      }
    }

    str_ref.set_error ();
    परत .0;
  }

  स्थिर तार्किक is_hint_op (op_code_t op)
  {
    निवड (op)
    {
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_ForceBold:
      स्थिती OpCode_LanguageGroup:
      स्थिती OpCode_ExpansionFactor:
	परत सत्य;
      पूर्वनिर्धारित:
	परत असत्य;
    }
  }
};

ढाँचा <typename VAL=op_str_t>
संरचना शीर्ष_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<संख्या_t>& env, शीर्ष_dict_मूल्ये_t<VAL> & dictval)
  {
    निवड (op) {
      स्थिती OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_FontMatrix:
	env.clear_args ();
	थांबा;
      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	थांबा;
    }
  }
};

ढाँचा <typename OPSET, typename PARAM, typename ENV=अंक_interp_env_t>
संरचना dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  तार्किक interpret (PARAM& param)
  {
    param.init ();
    पर्यंत (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      जर (unlikely (SUPER::env.in_error ()))
	परत असत्य;
    }

    परत सत्य;
  }

  खाजगी:
  प्रकारव्याख्या interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
