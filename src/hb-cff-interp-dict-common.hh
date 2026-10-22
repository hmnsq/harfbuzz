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

#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

using نطاق OT;

/* an opstr and the parsed out dict value(s) */
بنية dict_val_t : op_str_t
{
  فراغ init () {}
  فراغ fini () {}
};

تعريفنوع dict_val_t ژمارە_dict_val_t;

template <typename VAL> بنية dict_بەهاکان_t : parsed_بەهاکان_t<VAL> {};

template <typename OPSTR=op_str_t>
بنية لووتکە_dict_بەهاکان_t : dict_بەهاکان_t<OPSTR>
{
  فراغ init ()
  {
    dict_بەهاکان_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  فراغ fini () { dict_بەهاکان_t<OPSTR>::fini (); }

  صحيح  charStringsOffset;
  صحيح  FDArrayOffset;
};

بنية dict_opset_t : opset_t<ژمار_t>
{
  ساكن فراغ process_op (op_code_t op, interp_env_t<ژمار_t>& env)
  {
    بدل (op) {
      حالة OpCode_longintdict:  /* 5-byte integer */
	env.arg_کەڵەکە.پاڵنان_longint_from_substr (env.str_ref);
	اكسر;

      حالة OpCode_BCD:  /* real number */
	env.arg_کەڵەکە.پاڵنان_real (parse_bcd (env.str_ref));
	اكسر;

      افتراضي:
	opset_t<ژمار_t>::process_op (op, env);
	اكسر;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساكن مزدوج parse_bcd (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (str_ref.in_error ())) أرجع .0;

    تعداد Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    حرف buf[32] = {0};
    unsigned حرف byte = 0;
    لكل (unsigned i = 0, ژماردن = 0; ژماردن < ARRAY_LENGTH (buf); ++i, ++ژماردن)
    {
      unsigned nibble;
      إذا (!(i & 1))
      {
	إذا (unlikely (!str_ref.avail ())) اكسر;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      وإلا
	nibble = byte & 0x0F;

      إذا (unlikely (nibble == RESERVED)) اكسر;
      وإلا إذا (nibble == END)
      {
	ثابت حرف *p = buf;
	مزدوج pv;
	إذا (unlikely (!hb_parse_double (&p, p + ژماردن, &pv, true/* whole buffer */)))
	  اكسر;
	أرجع pv;
      }
      وإلا
      {
	buf[ژماردن] = "0123456789.EE?-?"[nibble];
	إذا (nibble == EXP_NEG)
	{
	  ++ژماردن;
	  إذا (unlikely (ژماردن == ARRAY_LENGTH (buf))) اكسر;
	  buf[ژماردن] = '-';
	}
      }
    }

    str_ref.set_error ();
    أرجع .0;
  }

  ساكن منطقي is_hint_op (op_code_t op)
  {
    بدل (op)
    {
      حالة Op_Code_Blue_بەهاکان:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ForceBold:
      حالة OpCode_LanguageGroup:
      حالة OpCode_ExpansionFactor:
	أرجع true;
      افتراضي:
	أرجع خطأ;
    }
  }
};

template <typename VAL=op_str_t>
بنية لووتکە_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, interp_env_t<ژمار_t>& env, لووتکە_dict_بەهاکان_t<VAL> & dictval)
  {
    بدل (op) {
      حالة OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_کەڵەکە.دەرهێنان_int ();
	env.clear_args ();
	اكسر;
      حالة OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_کەڵەکە.دەرهێنان_int ();
	env.clear_args ();
	اكسر;
      حالة OpCode_FontMatrix:
	env.clear_args ();
	اكسر;
      افتراضي:
	dict_opset_t::process_op (op, env);
	اكسر;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=ژمارە_interp_env_t>
بنية dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param)
  {
    param.init ();
    طالما (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      إذا (unlikely (SUPER::env.in_error ()))
	أرجع خطأ;
    }

    أرجع true;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
