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

#include "hb-cff-interp-common.hh"

namespace CFF {

using namespace OT;

/* an opstr and the parsed out dict value(s) */
sa_nracanaa dict_val_t : op_str_t
{
  void init () {}
  void fini () {}
};

typedef dict_val_t num_dict_val_t;

template <typename VAL> sa_nracanaa dict_values_t : parsed_values_t<VAL> {};

template <typename OPSTR=op_str_t>
sa_nracanaa top_dict_values_t : dict_values_t<OPSTR>
{
  void init ()
  {
    dict_values_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  void fini () { dict_values_t<OPSTR>::fini (); }

  puur_naa_nka  charStringsOffset;
  puur_naa_nka  FDArrayOffset;
};

sa_nracanaa dict_opset_t : opset_t<number_t>
{
  static void process_op (op_code_t op, interp_env_t<number_t>& env)
  {
    switch (op) {
      pak_sa OpCode_longintdict:  /* 5-byte integer */
	env.argStack.push_longint_from_substr (env.str_ref);
	bha_nga;

      pak_sa OpCode_BCD:  /* real number */
	env.argStack.push_real (parse_bcd (env.str_ref));
	bha_nga;

      default:
	opset_t<number_t>::process_op (op, env);
	bha_nga;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  static dviguu_na parse_bcd (byte_str_ref_t& str_ref)
  {
    ᬬᬾᬦ᭄ (unlikely (str_ref.in_error ())) ᬯᬮᬶ .0;

    ga_nanaa Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    var_na buf[32] = {0};
    unsigned var_na byte = 0;
    for (unsigned i = 0, count = 0; count < ARRAY_LENGTH (buf); ++i, ++count)
    {
      unsigned nibble;
      ᬬᬾᬦ᭄ (!(i & 1))
      {
	ᬬᬾᬦ᭄ (unlikely (!str_ref.avail ())) bha_nga;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      ᬮᬶᬬ
	nibble = byte & 0x0F;

      ᬬᬾᬦ᭄ (unlikely (nibble == RESERVED)) bha_nga;
      ᬮᬶᬬ ᬬᬾᬦ᭄ (nibble == END)
      {
	const var_na *p = buf;
	dviguu_na pv;
	ᬬᬾᬦ᭄ (unlikely (!hb_parse_double (&p, p + count, &pv, true/* whole buffer */)))
	  bha_nga;
	ᬯᬮᬶ pv;
      }
      ᬮᬶᬬ
      {
	buf[count] = "0123456789.EE?-?"[nibble];
	ᬬᬾᬦ᭄ (nibble == EXP_NEG)
	{
	  ++count;
	  ᬬᬾᬦ᭄ (unlikely (count == ARRAY_LENGTH (buf))) bha_nga;
	  buf[count] = '-';
	}
      }
    }

    str_ref.set_error ();
    ᬯᬮᬶ .0;
  }

  static bool is_hint_op (op_code_t op)
  {
    switch (op)
    {
      pak_sa OpCode_BlueValues:
      pak_sa OpCode_OtherBlues:
      pak_sa OpCode_FamilyBlues:
      pak_sa OpCode_FamilyOtherBlues:
      pak_sa OpCode_StemSnapH:
      pak_sa OpCode_StemSnapV:
      pak_sa OpCode_StdHW:
      pak_sa OpCode_StdVW:
      pak_sa OpCode_BlueScale:
      pak_sa OpCode_BlueShift:
      pak_sa OpCode_BlueFuzz:
      pak_sa OpCode_ForceBold:
      pak_sa OpCode_LanguageGroup:
      pak_sa OpCode_ExpansionFactor:
	ᬯᬮᬶ true;
      default:
	ᬯᬮᬶ false;
    }
  }
};

template <typename VAL=op_str_t>
sa_nracanaa top_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, interp_env_t<number_t>& env, top_dict_values_t<VAL> & dictval)
  {
    switch (op) {
      pak_sa OpCode_CharStrings:
	dictval.charStringsOffset = env.argStack.pop_int ();
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_FDArray:
	dictval.FDArrayOffset = env.argStack.pop_int ();
	env.clear_args ();
	bha_nga;
      pak_sa OpCode_FontMatrix:
	env.clear_args ();
	bha_nga;
      default:
	dict_opset_t::process_op (op, env);
	bha_nga;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=num_interp_env_t>
sa_nracanaa dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  bool interpret (PARAM& param)
  {
    param.init ();
    ᬲᭂᬤᭂᬮ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      ᬬᬾᬦ᭄ (unlikely (SUPER::env.in_error ()))
	ᬯᬮᬶ false;
    }

    ᬯᬮᬶ true;
  }

  private:
  typedef interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
