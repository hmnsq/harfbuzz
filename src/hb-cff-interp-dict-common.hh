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

#অন্তর্ভাব "hb-cff-interp-common.hh"

নামাকাশ CFF {

using নামাকাশ OT;

/* an opstr and the parsed out dict value(s) */
সংরচনা dict_val_t : op_str_t
{
  শূন্য init () {}
  শূন্য fini () {}
};

প্রকারনির্দেশ dict_val_t অঙ্ক_dict_val_t;

template <typename VAL> সংরচনা dict_মানসমূহ_t : parsed_মানসমূহ_t<VAL> {};

template <typename OPSTR=op_str_t>
সংরচনা শীর্ষ_dict_মানসমূহ_t : dict_মানসমূহ_t<OPSTR>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  শূন্য fini () { dict_মানসমূহ_t<OPSTR>::fini (); }

  পূর্ণাঙ্ক  charStringsOffset;
  পূর্ণাঙ্ক  FDArrayOffset;
};

সংরচনা dict_opset_t : opset_t<সংখ্যা_t>
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<সংখ্যা_t>& env)
  {
    বিকল্প (op) {
      পক্ষ OpCode_longintdict:  /* 5-byte integer */
	env.arg_স্তূপ.ঠেলো_longint_from_substr (env.str_ref);
	ভঙ্গ;

      পক্ষ OpCode_BCD:  /* real number */
	env.arg_স্তূপ.ঠেলো_real (parse_bcd (env.str_ref));
	ভঙ্গ;

      স্বভাব:
	opset_t<সংখ্যা_t>::process_op (op, env);
	ভঙ্গ;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  স্থির দ্বিগুণ parse_bcd (byte_str_ref_t& str_ref)
  {
    যদি (unlikely (str_ref.in_error ())) প্রত্যাগম .0;

    গণনা Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    বর্ণ buf[32] = {0};
    unsigned বর্ণ byte = 0;
    প্রতি (unsigned i = 0, গুনতি = 0; গুনতি < ARRAY_LENGTH (buf); ++i, ++গুনতি)
    {
      unsigned nibble;
      যদি (!(i & 1))
      {
	যদি (unlikely (!str_ref.avail ())) ভঙ্গ;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      অন্যথা
	nibble = byte & 0x0F;

      যদি (unlikely (nibble == RESERVED)) ভঙ্গ;
      অন্যথা যদি (nibble == END)
      {
	ধ্রুব বর্ণ *p = buf;
	দ্বিগুণ pv;
	যদি (unlikely (!hb_parse_double (&p, p + গুনতি, &pv, সত্য/* whole buffer */)))
	  ভঙ্গ;
	প্রত্যাগম pv;
      }
      অন্যথা
      {
	buf[গুনতি] = "0123456789.EE?-?"[nibble];
	যদি (nibble == EXP_NEG)
	{
	  ++গুনতি;
	  যদি (unlikely (গুনতি == ARRAY_LENGTH (buf))) ভঙ্গ;
	  buf[গুনতি] = '-';
	}
      }
    }

    str_ref.set_error ();
    প্রত্যাগম .0;
  }

  স্থির তার্কিক is_hint_op (op_code_t op)
  {
    বিকল্প (op)
    {
      পক্ষ Op_Code_Blue_মানসমূহ:
      পক্ষ OpCode_OtherBlues:
      পক্ষ OpCode_FamilyBlues:
      পক্ষ OpCode_FamilyOtherBlues:
      পক্ষ OpCode_StemSnapH:
      পক্ষ OpCode_StemSnapV:
      পক্ষ OpCode_StdHW:
      পক্ষ OpCode_StdVW:
      পক্ষ OpCode_BlueScale:
      পক্ষ OpCode_BlueShift:
      পক্ষ OpCode_BlueFuzz:
      পক্ষ OpCode_ForceBold:
      পক্ষ OpCode_LanguageGroup:
      পক্ষ OpCode_ExpansionFactor:
	প্রত্যাগম সত্য;
      স্বভাব:
	প্রত্যাগম অসত্য;
    }
  }
};

template <typename VAL=op_str_t>
সংরচনা শীর্ষ_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<সংখ্যা_t>& env, শীর্ষ_dict_মানসমূহ_t<VAL> & dictval)
  {
    বিকল্প (op) {
      পক্ষ OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_FontMatrix:
	env.clear_args ();
	ভঙ্গ;
      স্বভাব:
	dict_opset_t::process_op (op, env);
	ভঙ্গ;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=অঙ্ক_interp_env_t>
সংরচনা dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  তার্কিক interpret (PARAM& param)
  {
    param.init ();
    যাবত্ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      যদি (unlikely (SUPER::env.in_error ()))
	প্রত্যাগম অসত্য;
    }

    প্রত্যাগম সত্য;
  }

  গুপ্ত:
  প্রকারনির্দেশ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
