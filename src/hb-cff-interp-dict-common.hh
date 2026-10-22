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
#جے_بنا_تعریف HB_CFF_INTERP_DICT_COMMON_HH
#تعریف_کرو HB_CFF_INTERP_DICT_COMMON_HH

#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

ورتوں نطاق OT;

/* an opstr and the parsed out dict value(s) */
بنية dict_val_t : op_str_t
{
  فراغ init () {}
  فراغ fini () {}
};

تعريفنوع dict_val_t عدد_dict_val_t;

سانچا <قسم_ناں VAL> بنية dict_اقدار_t : parsed_اقدار_t<VAL> {};

سانچا <قسم_ناں OPSTR=op_str_t>
بنية چوٹی_dict_اقدار_t : dict_اقدار_t<OPSTR>
{
  فراغ init ()
  {
    dict_اقدار_t<OPSTR>::init ();
    char_لڑیاں_وتھ = 0;
    FDArray_وتھ = 0;
  }
  فراغ fini () { dict_اقدار_t<OPSTR>::fini (); }

  صحيح  char_لڑیاں_وتھ;
  صحيح  FDArray_وتھ;
};

بنية dict_opset_t : opset_t<نمبر_t>
{
  ساكن فراغ process_op (op_کوڈ_t op, interp_env_t<نمبر_t>& env)
  {
    بدل (op) {
      حالة Op_کوڈ_longintdict:  /* 5-byte integer */
	env.arg_ڈھیر.ڈالو_longint_from_substr (env.str_ref);
	اكسر;

      حالة Op_کوڈ_BCD:  /* real number */
	env.arg_ڈھیر.ڈالو_real (parse_bcd (env.str_ref));
	اكسر;

      افتراضي:
	opset_t<نمبر_t>::process_op (op, env);
	اكسر;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساكن مزدوج parse_bcd (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (str_ref.وچ_error ())) أرجع .0;

    تعداد Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, سرا_2 };

    حرف buf[32] = {0};
    بنانشانی حرف byte = 0;
    لكل (بنانشانی i = 0, گنتی = 0; گنتی < ARRAY_لمائی (buf); ++i, ++گنتی)
    {
      بنانشانی nibble;
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
      وإلا إذا (nibble == سرا_2)
      {
	ثابت حرف *p = buf;
	مزدوج pv;
	إذا (unlikely (!hb_parse_double (&p, p + گنتی, &pv, سچ/* whole buffer */)))
	  اكسر;
	أرجع pv;
      }
      وإلا
      {
	buf[گنتی] = "0123456789.EE?-?"[nibble];
	إذا (nibble == EXP_NEG)
	{
	  ++گنتی;
	  إذا (unlikely (گنتی == ARRAY_لمائی (buf))) اكسر;
	  buf[گنتی] = '-';
	}
      }
    }

    str_ref.سیٹ_error ();
    أرجع .0;
  }

  ساكن منطقي is_hint_op (op_کوڈ_t op)
  {
    بدل (op)
    {
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Force_Bold:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Expansion_ضارب:
	أرجع سچ;
      افتراضي:
	أرجع خطأ;
    }
  }
};

سانچا <قسم_ناں VAL=op_str_t>
بنية چوٹی_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, interp_env_t<نمبر_t>& env, چوٹی_dict_اقدار_t<VAL> & dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_Char_لڑیاں:
	dictval.char_لڑیاں_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_FDArray:
	dictval.FDArray_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_Font_میٹرکس:
	env.clear_دلیلاں ();
	اكسر;
      افتراضي:
	dict_opset_t::process_op (op, env);
	اكسر;
    }
  }
};

سانچا <قسم_ناں OPSET, قسم_ناں PARAM, قسم_ناں ENV=عدد_interp_env_t>
بنية dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param)
  {
    param.init ();
    طالما (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      إذا (unlikely (SUPER::env.وچ_error ()))
	أرجع خطأ;
    }

    أرجع سچ;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#جے_انت /* HB_CFF_INTERP_DICT_COMMON_HH */
