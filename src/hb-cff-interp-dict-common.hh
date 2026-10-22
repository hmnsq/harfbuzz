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
#که_نه_تعریف_شوی HB_CFF_INTERP_DICT_COMMON_HH
#تعریف_کړه HB_CFF_INTERP_DICT_COMMON_HH

#ضمن "hb-cff-interp-common.hh"

نطاق CFF {

کارول نطاق OT;

/* an opstr and the parsed out dict value(s) */
بنية dict_val_t : op_str_t
{
  فراغ init () {}
  فراغ fini () {}
};

تعريفنوع dict_val_t عدد_dict_val_t;

کينډۍ <ډول_نوم VAL> بنية dict_ارزښتونه_t : parsed_ارزښتونه_t<VAL> {};

کينډۍ <ډول_نوم OPSTR=op_str_t>
بنية سر_dict_ارزښتونه_t : dict_ارزښتونه_t<OPSTR>
{
  فراغ init ()
  {
    dict_ارزښتونه_t<OPSTR>::init ();
    char_مزي_ګانې_دننه_واټن = 0;
    FDArray_دننه_واټن = 0;
  }
  فراغ fini () { dict_ارزښتونه_t<OPSTR>::fini (); }

  صحيح  char_مزي_ګانې_دننه_واټن;
  صحيح  FDArray_دننه_واټن;
};

بنية dict_opset_t : opset_t<شمېره_t>
{
  ساكن فراغ process_op (op_کوډ_t op, interp_env_t<شمېره_t>& env)
  {
    بدل (op) {
      حالة Op_کوډ_longintdict:  /* 5-byte integer */
	env.arg_ډېری.ټېل_longint_from_substr (env.str_ref);
	اكسر;

      حالة Op_کوډ_BCD:  /* real number */
	env.arg_ډېری.ټېل_real (parse_bcd (env.str_ref));
	اكسر;

      افتراضي:
	opset_t<شمېره_t>::process_op (op, env);
	اكسر;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ساكن مزدوج parse_bcd (byte_str_ref_t& str_ref)
  {
    إذا (unlikely (str_ref.دننه_error ())) أرجع .0;

    تعداد Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, پای_ټکی_2 };

    حرف buf[32] = {0};
    بینښه حرف byte = 0;
    لكل (بینښه i = 0, شمېر = 0; شمېر < ARRAY_اوږدتیا (buf); ++i, ++شمېر)
    {
      بینښه nibble;
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
      وإلا إذا (nibble == پای_ټکی_2)
      {
	ثابت حرف *p = buf;
	مزدوج pv;
	إذا (unlikely (!hb_parse_double (&p, p + شمېر, &pv, ریښتیا/* whole buffer */)))
	  اكسر;
	أرجع pv;
      }
      وإلا
      {
	buf[شمېر] = "0123456789.EE?-?"[nibble];
	إذا (nibble == EXP_NEG)
	{
	  ++شمېر;
	  إذا (unlikely (شمېر == ARRAY_اوږدتیا (buf))) اكسر;
	  buf[شمېر] = '-';
	}
      }
    }

    str_ref.سېټ_error ();
    أرجع .0;
  }

  ساكن منطقي is_hint_op (op_کوډ_t op)
  {
    بدل (op)
    {
      حالة Op_کوډ_Blue_ارزښتونه:
      حالة Op_کوډ_Other_Blues:
      حالة Op_کوډ_Family_Blues:
      حالة Op_کوډ_Family_Other_Blues:
      حالة Op_کوډ_Stem_Snap_H:
      حالة Op_کوډ_Stem_Snap_V:
      حالة Op_کوډ_Std_HW:
      حالة Op_کوډ_Std_VW:
      حالة Op_کوډ_Blue_Scale:
      حالة Op_کوډ_Blue_ښويدنه:
      حالة Op_کوډ_Blue_Fuzz:
      حالة Op_کوډ_Force_Bold:
      حالة Op_کوډ_Language_ډله:
      حالة Op_کوډ_Expansion_ضریب:
	أرجع ریښتیا;
      افتراضي:
	أرجع خطأ;
    }
  }
};

کينډۍ <ډول_نوم VAL=op_str_t>
بنية سر_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, interp_env_t<شمېره_t>& env, سر_dict_ارزښتونه_t<VAL> & dictval)
  {
    بدل (op) {
      حالة Op_کوډ_Char_مزي_ګانې:
	dictval.char_مزي_ګانې_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_FDArray:
	dictval.FDArray_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_Font_ماتریکس:
	env.clear_دلیلونه ();
	اكسر;
      افتراضي:
	dict_opset_t::process_op (op, env);
	اكسر;
    }
  }
};

کينډۍ <ډول_نوم OPSET, ډول_نوم PARAM, ډول_نوم ENV=عدد_interp_env_t>
بنية dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  منطقي interpret (PARAM& param)
  {
    param.init ();
    طالما (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      إذا (unlikely (SUPER::env.دننه_error ()))
	أرجع خطأ;
    }

    أرجع ریښتیا;
  }

  خاص:
  تعريفنوع interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#د_که_پای /* HB_CFF_INTERP_DICT_COMMON_HH */
