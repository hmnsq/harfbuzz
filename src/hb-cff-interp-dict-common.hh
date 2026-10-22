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
#اگر_تعریف_نشده HB_CFF_INTERP_DICT_COMMON_HH
#تعریف_کن HB_CFF_INTERP_DICT_COMMON_HH

#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

استفاده نطاق OT;

/* an opstr and the parsed out dict value(s) */
بنية dict_val_t : op_str_t
{
  فراغ init () {}
  فراغ fini () {}
};

تعريفنوع dict_val_t رقم_dict_val_t;

قالب <نام_نوع VAL> بنية dict_مقادیر_t : parsed_مقادیر_t<VAL> {};

قالب <نام_نوع OPSTR=op_str_t>
بنية بالاترین_dict_مقادیر_t : dict_مقادیر_t<OPSTR>
{
  فراغ init ()
  {
    dict_مقادیر_t<OPSTR>::init ();
    char_رشتهها_جابه_جایی = 0;
    FDArray_جابه_جایی = 0;
  }
  فراغ fini () { dict_مقادیر_t<OPSTR>::fini (); }

  صحيح  char_رشتهها_جابه_جایی;
  صحيح  FDArray_جابه_جایی;
};

بنية dict_opset_t : opset_t<عدد_t>
{
  ساكن فراغ process_op (op_کد_t op, interp_env_t<عدد_t>& env)
  {
    بدل (op) {
      حالة Op_کد_longintdict:  /* 5-byte integer */
	env.arg_پشته.افزودن_longint_from_substr (env.str_ref);
	اكسر;

      حالة Op_کد_BCD:  /* real number */
	env.arg_پشته.افزودن_real (parse_bcd (env.str_ref));
	اكسر;

      افتراضي:
	opset_t<عدد_t>::process_op (op, env);
	اكسر;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساكن مزدوج parse_bcd (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (str_ref.درون_error ())) أرجع .0;

    تعداد Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, انتها_2 };

    حرف buf[32] = {0};
    بیعلامت حرف byte = 0;
    لكل (بیعلامت i = 0, شمار = 0; شمار < ARRAY_درازی (buf); ++i, ++شمار)
    {
      بیعلامت nibble;
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
      وإلا إذا (nibble == انتها_2)
      {
	ثابت حرف *p = buf;
	مزدوج pv;
	إذا (unlikely (!hb_parse_double (&p, p + شمار, &pv, درست/* whole buffer */)))
	  اكسر;
	أرجع pv;
      }
      وإلا
      {
	buf[شمار] = "0123456789.EE?-?"[nibble];
	إذا (nibble == EXP_NEG)
	{
	  ++شمار;
	  إذا (unlikely (شمار == ARRAY_درازی (buf))) اكسر;
	  buf[شمار] = '-';
	}
      }
    }

    str_ref.مجموعه_error ();
    أرجع .0;
  }

  ساكن منطقي is_hint_op (op_کد_t op)
  {
    بدل (op)
    {
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Force_Bold:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Expansion_ضریب:
	أرجع درست;
      افتراضي:
	أرجع خطأ;
    }
  }
};

قالب <نام_نوع VAL=op_str_t>
بنية بالاترین_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, interp_env_t<عدد_t>& env, بالاترین_dict_مقادیر_t<VAL> & dictval)
  {
    بدل (op) {
      حالة Op_کد_Char_رشتهها:
	dictval.char_رشتهها_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_FDArray:
	dictval.FDArray_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_Font_ماتریس:
	env.clear_آرگومانها ();
	اكسر;
      افتراضي:
	dict_opset_t::process_op (op, env);
	اكسر;
    }
  }
};

قالب <نام_نوع OPSET, نام_نوع PARAM, نام_نوع ENV=رقم_interp_env_t>
بنية dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param)
  {
    param.init ();
    طالما (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      إذا (unlikely (SUPER::env.درون_error ()))
	أرجع خطأ;
    }

    أرجع درست;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#پایان_اگر /* HB_CFF_INTERP_DICT_COMMON_HH */
