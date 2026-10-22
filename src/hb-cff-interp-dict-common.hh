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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CFF_INTERP_DICT_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CFF_INTERP_DICT_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ OT;

/* an opstr and the parsed out dict value(s) */
ਬਣਤਰ dict_val_t : op_str_t
{
  ਖਾਲੀ init () {}
  ਖਾਲੀ fini () {}
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_val_t ਅੰਕ_dict_val_t;

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VAL> ਬਣਤਰ dict_ਮੁੱਲਾਂ_t : parsed_ਮੁੱਲਾਂ_t<VAL> {};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSTR=op_str_t>
ਬਣਤਰ ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t : dict_ਮੁੱਲਾਂ_t<OPSTR>
{
  ਖਾਲੀ init ()
  {
    dict_ਮੁੱਲਾਂ_t<OPSTR>::init ();
    char_ਲੜੀਆਂ_ਵਿੱਥ = 0;
    FDArray_ਵਿੱਥ = 0;
  }
  ਖਾਲੀ fini () { dict_ਮੁੱਲਾਂ_t<OPSTR>::fini (); }

  ਪੂਰਨ  char_ਲੜੀਆਂ_ਵਿੱਥ;
  ਪੂਰਨ  FDArray_ਵਿੱਥ;
};

ਬਣਤਰ dict_opset_t : opset_t<ਸੰਖਿਆ_t>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, interp_env_t<ਸੰਖਿਆ_t>& env)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_longintdict:  /* 5-byte integer */
	env.arg_ਢੇਰ.ਧੱਕੋ_longint_from_substr (env.str_ref);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_BCD:  /* real number */
	env.arg_ਢੇਰ.ਧੱਕੋ_real (parse_bcd (env.str_ref));
	ਰੋਕੋ;

      ਮੂਲ:
	opset_t<ਸੰਖਿਆ_t>::process_op (op, env);
	ਰੋਕੋ;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ਸਥਿਰ ਦੁੱਗਣਾ parse_bcd (byte_str_ref_t& str_ref)
  {
    ਜੇ (unlikely (str_ref.ਵਿੱਚ_error ())) ਵਾਪਸ .0;

    ਗਿਣਤੀ Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, ਸਿਰਾ_2 };

    ਅੱਖਰ buf[32] = {0};
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ byte = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0, ਗਿਣਤੀ_2 = 0; ਗਿਣਤੀ_2 < ARRAY_ਲੰਮਾਈ (buf); ++i, ++ਗਿਣਤੀ_2)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ nibble;
      ਜੇ (!(i & 1))
      {
	ਜੇ (unlikely (!str_ref.avail ())) ਰੋਕੋ;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      ਨਹੀਂਤਾਂ
	nibble = byte & 0x0F;

      ਜੇ (unlikely (nibble == RESERVED)) ਰੋਕੋ;
      ਨਹੀਂਤਾਂ ਜੇ (nibble == ਸਿਰਾ_2)
      {
	ਅਚਲ ਅੱਖਰ *p = buf;
	ਦੁੱਗਣਾ pv;
	ਜੇ (unlikely (!hb_parse_double (&p, p + ਗਿਣਤੀ_2, &pv, ਸੱਚ/* whole buffer */)))
	  ਰੋਕੋ;
	ਵਾਪਸ pv;
      }
      ਨਹੀਂਤਾਂ
      {
	buf[ਗਿਣਤੀ_2] = "0123456789.EE?-?"[nibble];
	ਜੇ (nibble == EXP_NEG)
	{
	  ++ਗਿਣਤੀ_2;
	  ਜੇ (unlikely (ਗਿਣਤੀ_2 == ARRAY_ਲੰਮਾਈ (buf))) ਰੋਕੋ;
	  buf[ਗਿਣਤੀ_2] = '-';
	}
      }
    }

    str_ref.ਸੈੱਟ_error ();
    ਵਾਪਸ .0;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ is_hint_op (op_ਕੋਡ_t op)
  {
    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Force_Bold:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
	ਵਾਪਸ ਸੱਚ;
      ਮੂਲ:
	ਵਾਪਸ ਝੂਠ;
    }
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VAL=op_str_t>
ਬਣਤਰ ਸਿਖਰ_dict_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, interp_env_t<ਸੰਖਿਆ_t>& env, ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<VAL> & dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Char_ਲੜੀਆਂ:
	dictval.char_ਲੜੀਆਂ_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_FDArray:
	dictval.FDArray_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_Font_ਮੈਟ੍ਰਿਕਸ:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਰੋਕੋ;
    }
  }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSET, ਕਿਸਮ_ਨਾਂ PARAM, ਕਿਸਮ_ਨਾਂ ENV=ਅੰਕ_interp_env_t>
ਬਣਤਰ dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ਤਾਰਕਿਕ interpret (PARAM& param)
  {
    param.init ();
    ਜਦੋਂ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      ਜੇ (unlikely (SUPER::env.ਵਿੱਚ_error ()))
	ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਸੱਚ;
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#ਜੇ_ਅੰਤ /* HB_CFF_INTERP_DICT_COMMON_HH */
