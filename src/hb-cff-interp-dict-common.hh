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
struct dict_val_t : op_str_t
{
  void init () {}
  void fini () {}
};

typedef dict_val_t नुमेरो_dict_val_t;

template <typename VAL> struct dict_मगा_हालागा_t : parsed_मगा_हालागा_t<VAL> {};

template <typename OPSTR=op_str_t>
struct इताअस_dict_मगा_हालागा_t : dict_मगा_हालागा_t<OPSTR>
{
  void init ()
  {
    dict_मगा_हालागा_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  void fini () { dict_मगा_हालागा_t<OPSTR>::fini (); }

  int  charStringsOffset;
  int  FDArrayOffset;
};

struct dict_opset_t : opset_t<नुमेरो_t>
{
  static void process_op (op_code_t op, interp_env_t<नुमेरो_t>& env)
  {
    switch (op) {
      case OpCode_longintdict:  /* 5-byte integer */
	env.arg_सालानसान.इतुलाक_longint_from_substr (env.str_ref);
	break;

      case OpCode_BCD:  /* real number */
	env.arg_सालानसान.इतुलाक_real (parse_bcd (env.str_ref));
	break;

      default:
	opset_t<नुमेरो_t>::process_op (op, env);
	break;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  static double parse_bcd (byte_str_ref_t& str_ref)
  {
    if (unlikely (str_ref.in_error ())) return .0;

    enum Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    char buf[32] = {0};
    unsigned char byte = 0;
    for (unsigned i = 0, बिलांग = 0; बिलांग < ARRAY_LENGTH (buf); ++i, ++बिलांग)
    {
      unsigned nibble;
      if (!(i & 1))
      {
	if (unlikely (!str_ref.avail ())) break;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      else
	nibble = byte & 0x0F;

      if (unlikely (nibble == RESERVED)) break;
      else if (nibble == END)
      {
	const char *p = buf;
	double pv;
	if (unlikely (!hb_parse_double (&p, p + बिलांग, &pv, true/* whole buffer */)))
	  break;
	return pv;
      }
      else
      {
	buf[बिलांग] = "0123456789.EE?-?"[nibble];
	if (nibble == EXP_NEG)
	{
	  ++बिलांग;
	  if (unlikely (बिलांग == ARRAY_LENGTH (buf))) break;
	  buf[बिलांग] = '-';
	}
      }
    }

    str_ref.set_error ();
    return .0;
  }

  static bool is_hint_op (op_code_t op)
  {
    switch (op)
    {
      case Op_Code_Blue_मगा_हालागा:
      case OpCode_OtherBlues:
      case OpCode_FamilyBlues:
      case OpCode_FamilyOtherBlues:
      case OpCode_StemSnapH:
      case OpCode_StemSnapV:
      case OpCode_StdHW:
      case OpCode_StdVW:
      case OpCode_BlueScale:
      case OpCode_BlueShift:
      case OpCode_BlueFuzz:
      case OpCode_ForceBold:
      case OpCode_LanguageGroup:
      case OpCode_ExpansionFactor:
	return true;
      default:
	return false;
    }
  }
};

template <typename VAL=op_str_t>
struct इताअस_dict_opset_t : dict_opset_t
{
  static void process_op (op_code_t op, interp_env_t<नुमेरो_t>& env, इताअस_dict_मगा_हालागा_t<VAL> & dictval)
  {
    switch (op) {
      case OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_सालानसान.आलिसिन_int ();
	env.clear_मगा_कातविरान ();
	break;
      case OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_सालानसान.आलिसिन_int ();
	env.clear_मगा_कातविरान ();
	break;
      case OpCode_FontMatrix:
	env.clear_मगा_कातविरान ();
	break;
      default:
	dict_opset_t::process_op (op, env);
	break;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=नुमेरो_interp_env_t>
struct dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  bool interpret (PARAM& param)
  {
    param.init ();
    while (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      if (unlikely (SUPER::env.in_error ()))
	return false;
    }

    return true;
  }

  private:
  typedef interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
