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
কাঠামো dict_val_t : op_str_t
{
  শূন্য init () {}
  শূন্য fini () {}
};

প্রকারলেখ dict_val_t অঙ্ক_dict_val_t;

template <typename VAL> কাঠামো dict_মানসমূহ_t : parsed_মানসমূহ_t<VAL> {};

template <typename OPSTR=op_str_t>
কাঠামো শীর্ষ_dict_মানসমূহ_t : dict_মানসমূহ_t<OPSTR>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  শূন্য fini () { dict_মানসমূহ_t<OPSTR>::fini (); }

  পূর্ণসংখ্যা  charStringsOffset;
  পূর্ণসংখ্যা  FDArrayOffset;
};

কাঠামো dict_opset_t : opset_t<সংখ্যা_t>
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<সংখ্যা_t>& env)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_longintdict:  /* 5-byte integer */
	env.arg_স্তূপ.ঠেলো_longint_from_substr (env.str_ref);
	বিরতি;

      ক্ষেত্র OpCode_BCD:  /* real number */
	env.arg_স্তূপ.ঠেলো_real (parse_bcd (env.str_ref));
	বিরতি;

      পূর্বনির্ধারিত:
	opset_t<সংখ্যা_t>::process_op (op, env);
	বিরতি;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  স্থির দ্বিগুণ parse_bcd (byte_str_ref_t& str_ref)
  {
    যদি (unlikely (str_ref.in_error ())) ফেরত .0;

    গণনা Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    অক্ষর buf[32] = {0};
    unsigned অক্ষর byte = 0;
    জন্য (unsigned i = 0, গুনতি = 0; গুনতি < ARRAY_দৈর্ঘ্য (buf); ++i, ++গুনতি)
    {
      unsigned nibble;
      যদি (!(i & 1))
      {
	যদি (unlikely (!str_ref.avail ())) বিরতি;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      অন্যথা
	nibble = byte & 0x0F;

      যদি (unlikely (nibble == RESERVED)) বিরতি;
      অন্যথা যদি (nibble == END)
      {
	ধ্রুবক অক্ষর *p = buf;
	দ্বিগুণ pv;
	যদি (unlikely (!hb_parse_double (&p, p + গুনতি, &pv, সত্য/* whole buffer */)))
	  বিরতি;
	ফেরত pv;
      }
      অন্যথা
      {
	buf[গুনতি] = "0123456789.EE?-?"[nibble];
	যদি (nibble == EXP_NEG)
	{
	  ++গুনতি;
	  যদি (unlikely (গুনতি == ARRAY_দৈর্ঘ্য (buf))) বিরতি;
	  buf[গুনতি] = '-';
	}
      }
    }

    str_ref.set_error ();
    ফেরত .0;
  }

  স্থির যৌক্তিক is_hint_op (op_code_t op)
  {
    নির্বাচন (op)
    {
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_ForceBold:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র OpCode_ExpansionFactor:
	ফেরত সত্য;
      পূর্বনির্ধারিত:
	ফেরত মিথ্যা;
    }
  }
};

template <typename VAL=op_str_t>
কাঠামো শীর্ষ_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, interp_env_t<সংখ্যা_t>& env, শীর্ষ_dict_মানসমূহ_t<VAL> & dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_FontMatrix:
	env.clear_args ();
	বিরতি;
      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	বিরতি;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=অঙ্ক_interp_env_t>
কাঠামো dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  যৌক্তিক interpret (PARAM& param)
  {
    param.init ();
    যখন (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      যদি (unlikely (SUPER::env.in_error ()))
	ফেরত মিথ্যা;
    }

    ফেরত সত্য;
  }

  ব্যক্তিগত:
  প্রকারলেখ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
