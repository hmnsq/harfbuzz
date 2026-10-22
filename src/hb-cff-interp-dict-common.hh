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

#شامل "hb-cff-interp-common.hh"

فضاینام CFF {

using فضاینام OT;

/* an opstr and the parsed out dict value(s) */
ساختار dict_val_t : op_str_t
{
  تهی init () {}
  تهی fini () {}
};

تعریفنوع dict_val_t رقم_dict_val_t;

template <typename VAL> ساختار dict_مقادیر_t : parsed_مقادیر_t<VAL> {};

template <typename OPSTR=op_str_t>
ساختار بالاترین_dict_مقادیر_t : dict_مقادیر_t<OPSTR>
{
  تهی init ()
  {
    dict_مقادیر_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  تهی fini () { dict_مقادیر_t<OPSTR>::fini (); }

  صحیح  charStringsOffset;
  صحیح  FDArrayOffset;
};

ساختار dict_opset_t : opset_t<عدد_t>
{
  ایستا تهی process_op (op_code_t op, interp_env_t<عدد_t>& env)
  {
    انتخاب (op) {
      حالت OpCode_longintdict:  /* 5-byte integer */
	env.arg_پشته.افزودن_longint_from_substr (env.str_ref);
	بشکن;

      حالت OpCode_BCD:  /* real number */
	env.arg_پشته.افزودن_real (parse_bcd (env.str_ref));
	بشکن;

      پیشفرض:
	opset_t<عدد_t>::process_op (op, env);
	بشکن;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ایستا دوگانه parse_bcd (byte_str_ref_t& str_ref)
  {
    اگر (unlikely (str_ref.in_error ())) بازگشت .0;

    شمارش Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    نویسه buf[32] = {0};
    unsigned نویسه byte = 0;
    برای (unsigned i = 0, شمار = 0; شمار < ARRAY_LENGTH (buf); ++i, ++شمار)
    {
      unsigned nibble;
      اگر (!(i & 1))
      {
	اگر (unlikely (!str_ref.avail ())) بشکن;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      وگرنه
	nibble = byte & 0x0F;

      اگر (unlikely (nibble == RESERVED)) بشکن;
      وگرنه اگر (nibble == END)
      {
	ثابت نویسه *p = buf;
	دوگانه pv;
	اگر (unlikely (!hb_parse_double (&p, p + شمار, &pv, درست/* whole buffer */)))
	  بشکن;
	بازگشت pv;
      }
      وگرنه
      {
	buf[شمار] = "0123456789.EE?-?"[nibble];
	اگر (nibble == EXP_NEG)
	{
	  ++شمار;
	  اگر (unlikely (شمار == ARRAY_LENGTH (buf))) بشکن;
	  buf[شمار] = '-';
	}
      }
    }

    str_ref.set_error ();
    بازگشت .0;
  }

  ایستا منطقی is_hint_op (op_code_t op)
  {
    انتخاب (op)
    {
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_ForceBold:
      حالت OpCode_LanguageGroup:
      حالت OpCode_ExpansionFactor:
	بازگشت درست;
      پیشفرض:
	بازگشت نادرست;
    }
  }
};

template <typename VAL=op_str_t>
ساختار بالاترین_dict_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, interp_env_t<عدد_t>& env, بالاترین_dict_مقادیر_t<VAL> & dictval)
  {
    انتخاب (op) {
      حالت OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;
      حالت OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;
      حالت OpCode_FontMatrix:
	env.clear_args ();
	بشکن;
      پیشفرض:
	dict_opset_t::process_op (op, env);
	بشکن;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=رقم_interp_env_t>
ساختار dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقی interpret (PARAM& param)
  {
    param.init ();
    وقتی (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      اگر (unlikely (SUPER::env.in_error ()))
	بازگشت نادرست;
    }

    بازگشت درست;
  }

  خصوصی:
  تعریفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
