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
#اگر_غیر_تعریف_شدہ HB_CFF_INTERP_DICT_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_DICT_COMMON_HH

#شامل "hb-cff-interp-common.hh"

نامخلا CFF {

استعمال نامخلا OT;

/* an opstr and the parsed out dict value(s) */
ساخت dict_val_t : op_str_t
{
  خالی init () {}
  خالی fini () {}
};

قسمتعریف dict_val_t عدد_dict_val_t;

سانچہ <قسم_نام VAL> ساخت dict_اقدار_t : parsed_اقدار_t<VAL> {};

سانچہ <قسم_نام OPSTR=op_str_t>
ساخت چوٹی_dict_اقدار_t : dict_اقدار_t<OPSTR>
{
  خالی init ()
  {
    dict_اقدار_t<OPSTR>::init ();
    char_لڑیاں_انحراف = 0;
    FDArray_انحراف = 0;
  }
  خالی fini () { dict_اقدار_t<OPSTR>::fini (); }

  صحیح  char_لڑیاں_انحراف;
  صحیح  FDArray_انحراف;
};

ساخت dict_opset_t : opset_t<نمبر_t>
{
  ساکن خالی process_op (op_رمز_t op, interp_env_t<نمبر_t>& env)
  {
    انتخاب (op) {
      حالت Op_رمز_longintdict:  /* 5-byte integer */
	env.arg_ڈھیر.ڈالو_longint_from_substr (env.str_ref);
	توڑو;

      حالت Op_رمز_BCD:  /* real number */
	env.arg_ڈھیر.ڈالو_real (parse_bcd (env.str_ref));
	توڑو;

      طےشدہ:
	opset_t<نمبر_t>::process_op (op, env);
	توڑو;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساکن دوہرا parse_bcd (byte_str_ref_t& str_ref)
  {
    اگر (unlikely (str_ref.اندر_error ())) واپس .0;

    شمار Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, انتہا_2 };

    حرف buf[32] = {0};
    غیرعلامتی حرف byte = 0;
    برائے (غیرعلامتی i = 0, گنتی = 0; گنتی < ARRAY_لمبائی (buf); ++i, ++گنتی)
    {
      غیرعلامتی nibble;
      اگر (!(i & 1))
      {
	اگر (unlikely (!str_ref.avail ())) توڑو;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      ورنہ
	nibble = byte & 0x0F;

      اگر (unlikely (nibble == RESERVED)) توڑو;
      ورنہ اگر (nibble == انتہا_2)
      {
	ثابت حرف *p = buf;
	دوہرا pv;
	اگر (unlikely (!hb_parse_double (&p, p + گنتی, &pv, سچ/* whole buffer */)))
	  توڑو;
	واپس pv;
      }
      ورنہ
      {
	buf[گنتی] = "0123456789.EE?-?"[nibble];
	اگر (nibble == EXP_NEG)
	{
	  ++گنتی;
	  اگر (unlikely (گنتی == ARRAY_لمبائی (buf))) توڑو;
	  buf[گنتی] = '-';
	}
      }
    }

    str_ref.سیٹ_error ();
    واپس .0;
  }

  ساکن منطقی is_hint_op (op_رمز_t op)
  {
    انتخاب (op)
    {
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Force_Bold:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Expansion_ضربی_عدد:
	واپس سچ;
      طےشدہ:
	واپس جھوٹ;
    }
  }
};

سانچہ <قسم_نام VAL=op_str_t>
ساخت چوٹی_dict_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, interp_env_t<نمبر_t>& env, چوٹی_dict_اقدار_t<VAL> & dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_Char_لڑیاں:
	dictval.char_لڑیاں_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_FDArray:
	dictval.FDArray_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_Font_میٹرکس:
	env.clear_دلائل ();
	توڑو;
      طےشدہ:
	dict_opset_t::process_op (op, env);
	توڑو;
    }
  }
};

سانچہ <قسم_نام OPSET, قسم_نام PARAM, قسم_نام ENV=عدد_interp_env_t>
ساخت dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقی interpret (PARAM& param)
  {
    param.init ();
    جب (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      اگر (unlikely (SUPER::env.اندر_error ()))
	واپس جھوٹ;
    }

    واپس سچ;
  }

  نجی:
  قسمتعریف interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#اگر_ختم /* HB_CFF_INTERP_DICT_COMMON_HH */
