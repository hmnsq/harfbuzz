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

प्रकारनिर्देश dict_val_t अङ्क_dict_val_t;

ढाँचा <typename VAL> संरचना dict_मानहरू_t : parsed_मानहरू_t<VAL> {};

ढाँचा <typename OPSTR=op_str_t>
संरचना शीर्ष_dict_मानहरू_t : dict_मानहरू_t<OPSTR>
{
  शून्य init ()
  {
    dict_मानहरू_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  शून्य fini () { dict_मानहरू_t<OPSTR>::fini (); }

  पूर्णाङ्क  charStringsOffset;
  पूर्णाङ्क  FDArrayOffset;
};

संरचना dict_opset_t : opset_t<सङ्ख्या_t>
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<सङ्ख्या_t>& env)
  {
    विकल्प (op) {
      पक्ष OpCode_longintdict:  /* 5-byte integer */
	env.arg_थाक.थप_longint_from_substr (env.str_ref);
	रोक;

      पक्ष OpCode_BCD:  /* real number */
	env.arg_थाक.थप_real (parse_bcd (env.str_ref));
	रोक;

      स्वभाव:
	opset_t<सङ्ख्या_t>::process_op (op, env);
	रोक;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  स्थिर द्विगुण parse_bcd (byte_str_ref_t& str_ref)
  {
    यदि (unlikely (str_ref.in_error ())) फर्काऊ .0;

    गणना Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    वर्ण buf[32] = {0};
    अचिन्हित वर्ण byte = 0;
    लागि (अचिन्हित i = 0, गन्ती = 0; गन्ती < ARRAY_LENGTH (buf); ++i, ++गन्ती)
    {
      अचिन्हित nibble;
      यदि (!(i & 1))
      {
	यदि (unlikely (!str_ref.avail ())) रोक;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      अन्यथा
	nibble = byte & 0x0F;

      यदि (unlikely (nibble == RESERVED)) रोक;
      अन्यथा यदि (nibble == END)
      {
	अचर वर्ण *p = buf;
	द्विगुण pv;
	यदि (unlikely (!hb_parse_double (&p, p + गन्ती, &pv, सत्य/* whole buffer */)))
	  रोक;
	फर्काऊ pv;
      }
      अन्यथा
      {
	buf[गन्ती] = "0123456789.EE?-?"[nibble];
	यदि (nibble == EXP_NEG)
	{
	  ++गन्ती;
	  यदि (unlikely (गन्ती == ARRAY_LENGTH (buf))) रोक;
	  buf[गन्ती] = '-';
	}
      }
    }

    str_ref.set_error ();
    फर्काऊ .0;
  }

  स्थिर तार्किक is_hint_op (op_code_t op)
  {
    विकल्प (op)
    {
      पक्ष Op_Code_Blue_मानहरू:
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
	फर्काऊ सत्य;
      स्वभाव:
	फर्काऊ असत्य;
    }
  }
};

ढाँचा <typename VAL=op_str_t>
संरचना शीर्ष_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, interp_env_t<सङ्ख्या_t>& env, शीर्ष_dict_मानहरू_t<VAL> & dictval)
  {
    विकल्प (op) {
      पक्ष OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_थाक.झिक_int ();
	env.clear_args ();
	रोक;
      पक्ष OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_थाक.झिक_int ();
	env.clear_args ();
	रोक;
      पक्ष OpCode_FontMatrix:
	env.clear_args ();
	रोक;
      स्वभाव:
	dict_opset_t::process_op (op, env);
	रोक;
    }
  }
};

ढाँचा <typename OPSET, typename PARAM, typename ENV=अङ्क_interp_env_t>
संरचना dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  तार्किक interpret (PARAM& param)
  {
    param.init ();
    जबसम्म (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      यदि (unlikely (SUPER::env.in_error ()))
	फर्काऊ असत्य;
    }

    फर्काऊ सत्य;
  }

  गुप्त:
  प्रकारनिर्देश interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
