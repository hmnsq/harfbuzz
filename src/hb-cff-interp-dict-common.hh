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

#അന്തര്ഭാവ "hb-cff-interp-common.hh"

നാമാകാശ CFF {

using നാമാകാശ OT;

/* an opstr and the parsed out dict value(s) */
ഘടന dict_val_t : op_str_t
{
  ശൂന്യം init () {}
  ശൂന്യം fini () {}
};

തരനിർവചനം dict_val_t അക്കം_dict_val_t;

template <typename VAL> ഘടന dict_മൂല്യങ്ങൾ_t : parsed_മൂല്യങ്ങൾ_t<VAL> {};

template <typename OPSTR=op_str_t>
ഘടന മുകൾ_dict_മൂല്യങ്ങൾ_t : dict_മൂല്യങ്ങൾ_t<OPSTR>
{
  ശൂന്യം init ()
  {
    dict_മൂല്യങ്ങൾ_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  ശൂന്യം fini () { dict_മൂല്യങ്ങൾ_t<OPSTR>::fini (); }

  പൂർണ്ണസംഖ്യ  charStringsOffset;
  പൂർണ്ണസംഖ്യ  FDArrayOffset;
};

ഘടന dict_opset_t : opset_t<സംഖ്യ_t>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, interp_env_t<സംഖ്യ_t>& env)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_longintdict:  /* 5-byte integer */
	env.arg_അട്ടി.തള്ളുക_longint_from_substr (env.str_ref);
	നിർത്തുക;

      അവസ്ഥ OpCode_BCD:  /* real number */
	env.arg_അട്ടി.തള്ളുക_real (parse_bcd (env.str_ref));
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	opset_t<സംഖ്യ_t>::process_op (op, env);
	നിർത്തുക;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  സ്ഥിതം ഇരട്ട parse_bcd (byte_str_ref_t& str_ref)
  {
    എങ്കിൽ (unlikely (str_ref.in_error ())) തിരികെ .0;

    എണ്ണൽ Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    അക്ഷരം buf[32] = {0};
    unsigned അക്ഷരം byte = 0;
    വേണ്ടി (unsigned i = 0, എണ്ണം = 0; എണ്ണം < ARRAY_LENGTH (buf); ++i, ++എണ്ണം)
    {
      unsigned nibble;
      എങ്കിൽ (!(i & 1))
      {
	എങ്കിൽ (unlikely (!str_ref.avail ())) നിർത്തുക;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      അല്ലെങ്കിൽ
	nibble = byte & 0x0F;

      എങ്കിൽ (unlikely (nibble == RESERVED)) നിർത്തുക;
      അല്ലെങ്കിൽ എങ്കിൽ (nibble == END)
      {
	സ്ഥിരം അക്ഷരം *p = buf;
	ഇരട്ട pv;
	എങ്കിൽ (unlikely (!hb_parse_double (&p, p + എണ്ണം, &pv, സത്യം/* whole buffer */)))
	  നിർത്തുക;
	തിരികെ pv;
      }
      അല്ലെങ്കിൽ
      {
	buf[എണ്ണം] = "0123456789.EE?-?"[nibble];
	എങ്കിൽ (nibble == EXP_NEG)
	{
	  ++എണ്ണം;
	  എങ്കിൽ (unlikely (എണ്ണം == ARRAY_LENGTH (buf))) നിർത്തുക;
	  buf[എണ്ണം] = '-';
	}
      }
    }

    str_ref.set_error ();
    തിരികെ .0;
  }

  സ്ഥിതം തർക്കം is_hint_op (op_code_t op)
  {
    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_ForceBold:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ OpCode_ExpansionFactor:
	തിരികെ സത്യം;
      സ്ഥിരസ്ഥിതി:
	തിരികെ അസത്യം;
    }
  }
};

template <typename VAL=op_str_t>
ഘടന മുകൾ_dict_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, interp_env_t<സംഖ്യ_t>& env, മുകൾ_dict_മൂല്യങ്ങൾ_t<VAL> & dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_FontMatrix:
	env.clear_args ();
	നിർത്തുക;
      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	നിർത്തുക;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=അക്കം_interp_env_t>
ഘടന dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  തർക്കം interpret (PARAM& param)
  {
    param.init ();
    ആകുമ്പോൾ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      എങ്കിൽ (unlikely (SUPER::env.in_error ()))
	തിരികെ അസത്യം;
    }

    തിരികെ സത്യം;
  }

  സ്വകാര്യ:
  തരനിർവചനം interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
