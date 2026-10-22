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

#அந்தர்பாவ "hb-cff-interp-common.hh"

நாமாகாஶ CFF {

using நாமாகாஶ OT;

/* an opstr and the parsed out dict value(s) */
கட்டமைப்பு dict_val_t : op_str_t
{
  வெறுமை init () {}
  வெறுமை fini () {}
};

வகைவரை dict_val_t இலக்கம்_dict_val_t;

template <typename VAL> கட்டமைப்பு dict_மதிப்புகள்_t : parsed_மதிப்புகள்_t<VAL> {};

template <typename OPSTR=op_str_t>
கட்டமைப்பு உச்சி_dict_மதிப்புகள்_t : dict_மதிப்புகள்_t<OPSTR>
{
  வெறுமை init ()
  {
    dict_மதிப்புகள்_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  வெறுமை fini () { dict_மதிப்புகள்_t<OPSTR>::fini (); }

  முழு  charStringsOffset;
  முழு  FDArrayOffset;
};

கட்டமைப்பு dict_opset_t : opset_t<எண்_t>
{
  நிலையான வெறுமை process_op (op_code_t op, interp_env_t<எண்_t>& env)
  {
    தேர்வு (op) {
      நிலை OpCode_longintdict:  /* 5-byte integer */
	env.arg_அடுக்கு.தள்ளு_longint_from_substr (env.str_ref);
	நிறுத்து;

      நிலை OpCode_BCD:  /* real number */
	env.arg_அடுக்கு.தள்ளு_real (parse_bcd (env.str_ref));
	நிறுத்து;

      இயல்பு:
	opset_t<எண்_t>::process_op (op, env);
	நிறுத்து;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  நிலையான இரட்டை parse_bcd (byte_str_ref_t& str_ref)
  {
    எனில் (unlikely (str_ref.in_error ())) திரும்பு .0;

    எண்ணிக்கை Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    எழுத்து buf[32] = {0};
    unsigned எழுத்து byte = 0;
    க்கு (unsigned i = 0, மொத்தம் = 0; மொத்தம் < ARRAY_LENGTH (buf); ++i, ++மொத்தம்)
    {
      unsigned nibble;
      எனில் (!(i & 1))
      {
	எனில் (unlikely (!str_ref.avail ())) நிறுத்து;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      இல்லையெனில்
	nibble = byte & 0x0F;

      எனில் (unlikely (nibble == RESERVED)) நிறுத்து;
      இல்லையெனில் எனில் (nibble == END)
      {
	மாறிலி எழுத்து *p = buf;
	இரட்டை pv;
	எனில் (unlikely (!hb_parse_double (&p, p + மொத்தம், &pv, உண்மை/* whole buffer */)))
	  நிறுத்து;
	திரும்பு pv;
      }
      இல்லையெனில்
      {
	buf[மொத்தம்] = "0123456789.EE?-?"[nibble];
	எனில் (nibble == EXP_NEG)
	{
	  ++மொத்தம்;
	  எனில் (unlikely (மொத்தம் == ARRAY_LENGTH (buf))) நிறுத்து;
	  buf[மொத்தம்] = '-';
	}
      }
    }

    str_ref.set_error ();
    திரும்பு .0;
  }

  நிலையான தருக்க is_hint_op (op_code_t op)
  {
    தேர்வு (op)
    {
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_ForceBold:
      நிலை OpCode_LanguageGroup:
      நிலை OpCode_ExpansionFactor:
	திரும்பு உண்மை;
      இயல்பு:
	திரும்பு பொய்;
    }
  }
};

template <typename VAL=op_str_t>
கட்டமைப்பு உச்சி_dict_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, interp_env_t<எண்_t>& env, உச்சி_dict_மதிப்புகள்_t<VAL> & dictval)
  {
    தேர்வு (op) {
      நிலை OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_FontMatrix:
	env.clear_args ();
	நிறுத்து;
      இயல்பு:
	dict_opset_t::process_op (op, env);
	நிறுத்து;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=இலக்கம்_interp_env_t>
கட்டமைப்பு dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  தருக்க interpret (PARAM& param)
  {
    param.init ();
    போது (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      எனில் (unlikely (SUPER::env.in_error ()))
	திரும்பு பொய்;
    }

    திரும்பு உண்மை;
  }

  தனிப்பட்ட:
  வகைவரை interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
