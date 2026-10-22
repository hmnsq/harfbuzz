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
#جيڪڏهن_وصف_نه_ٿيل HB_CFF_INTERP_DICT_COMMON_HH
#وصف_ڪريو HB_CFF_INTERP_DICT_COMMON_HH

#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

استعمال نطاق OT;

/* an opstr and the parsed out dict value(s) */
بنية dict_val_t : op_str_t
{
  فراغ init () {}
  فراغ fini () {}
};

تعريفنوع dict_val_t انگ_dict_val_t;

سانچو <قسم_نالو VAL> بنية dict_قدرون_t : parsed_قدرون_t<VAL> {};

سانچو <قسم_نالو OPSTR=op_str_t>
بنية چوٽي_dict_قدرون_t : dict_قدرون_t<OPSTR>
{
  فراغ init ()
  {
    dict_قدرون_t<OPSTR>::init ();
    char_لڙيون_وٿي = 0;
    FDArray_وٿي = 0;
  }
  فراغ fini () { dict_قدرون_t<OPSTR>::fini (); }

  صحيح  char_لڙيون_وٿي;
  صحيح  FDArray_وٿي;
};

بنية dict_opset_t : opset_t<عدد_t>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, interp_env_t<عدد_t>& env)
  {
    بدل (op) {
      حالة Op_ڪوڊ_longintdict:  /* 5-byte integer */
	env.arg_ڍير.ڌڪو_longint_from_substr (env.str_ref);
	اكسر;

      حالة Op_ڪوڊ_BCD:  /* real number */
	env.arg_ڍير.ڌڪو_real (parse_bcd (env.str_ref));
	اكسر;

      افتراضي:
	opset_t<عدد_t>::process_op (op, env);
	اكسر;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساكن مزدوج parse_bcd (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (str_ref.اندريون_error ())) أرجع .0;

    تعداد Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, پڇاڙي_2 };

    حرف buf[32] = {0};
    بنانشان حرف byte = 0;
    لكل (بنانشان i = 0, ڳڻپ = 0; ڳڻپ < ARRAY_ڊيگهائي (buf); ++i, ++ڳڻپ)
    {
      بنانشان nibble;
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
      وإلا إذا (nibble == پڇاڙي_2)
      {
	ثابت حرف *p = buf;
	مزدوج pv;
	إذا (unlikely (!hb_parse_double (&p, p + ڳڻپ, &pv, سچ/* whole buffer */)))
	  اكسر;
	أرجع pv;
      }
      وإلا
      {
	buf[ڳڻپ] = "0123456789.EE?-?"[nibble];
	إذا (nibble == EXP_NEG)
	{
	  ++ڳڻپ;
	  إذا (unlikely (ڳڻپ == ARRAY_ڊيگهائي (buf))) اكسر;
	  buf[ڳڻپ] = '-';
	}
      }
    }

    str_ref.سيٽ_error ();
    أرجع .0;
  }

  ساكن منطقي is_hint_op (op_ڪوڊ_t op)
  {
    بدل (op)
    {
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Force_Bold:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
	أرجع سچ;
      افتراضي:
	أرجع خطأ;
    }
  }
};

سانچو <قسم_نالو VAL=op_str_t>
بنية چوٽي_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, interp_env_t<عدد_t>& env, چوٽي_dict_قدرون_t<VAL> & dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Char_لڙيون:
	dictval.char_لڙيون_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_FDArray:
	dictval.FDArray_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_Font_ميٽرڪس:
	env.clear_دليلون ();
	اكسر;
      افتراضي:
	dict_opset_t::process_op (op, env);
	اكسر;
    }
  }
};

سانچو <قسم_نالو OPSET, قسم_نالو PARAM, قسم_نالو ENV=انگ_interp_env_t>
بنية dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param)
  {
    param.init ();
    طالما (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      إذا (unlikely (SUPER::env.اندريون_error ()))
	أرجع خطأ;
    }

    أرجع سچ;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF_INTERP_DICT_COMMON_HH */
