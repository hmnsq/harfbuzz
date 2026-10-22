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

#ಅನ್ತರ್ಭಾವ "hb-cff-interp-common.hh"

ನಾಮಾಕಾಶ CFF {

using ನಾಮಾಕಾಶ OT;

/* an opstr and the parsed out dict value(s) */
ಸಂರಚನಾ dict_val_t : op_str_t
{
  ಶೂನ್ಯ init () {}
  ಶೂನ್ಯ fini () {}
};

ಪ್ರಕಾರನಿರ್ದೇಶ dict_val_t ಅಂಕೆ_dict_val_t;

template <typename VAL> ಸಂರಚನಾ dict_ಮೌಲ್ಯಗಳು_t : parsed_ಮೌಲ್ಯಗಳು_t<VAL> {};

template <typename OPSTR=op_str_t>
ಸಂರಚನಾ ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t : dict_ಮೌಲ್ಯಗಳು_t<OPSTR>
{
  ಶೂನ್ಯ init ()
  {
    dict_ಮೌಲ್ಯಗಳು_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  ಶೂನ್ಯ fini () { dict_ಮೌಲ್ಯಗಳು_t<OPSTR>::fini (); }

  ಪೂರ್ಣಾಙ್ಕ  charStringsOffset;
  ಪೂರ್ಣಾಙ್ಕ  FDArrayOffset;
};

ಸಂರಚನಾ dict_opset_t : opset_t<ಸಂಖ್ಯೆ_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, interp_env_t<ಸಂಖ್ಯೆ_t>& env)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_longintdict:  /* 5-byte integer */
	env.arg_ರಾಶಿ.ತಳ್ಳು_longint_from_substr (env.str_ref);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_BCD:  /* real number */
	env.arg_ರಾಶಿ.ತಳ್ಳು_real (parse_bcd (env.str_ref));
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	opset_t<ಸಂಖ್ಯೆ_t>::process_op (op, env);
	ಭಙ್ಗ;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ಸ್ಥಿರ ದ್ವಿಗುಣ parse_bcd (byte_str_ref_t& str_ref)
  {
    ಯದಿ (unlikely (str_ref.in_error ())) ಪ್ರತ್ಯಾಗಮ .0;

    ಗಣನಾ Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    ವರ್ಣ buf[32] = {0};
    unsigned ವರ್ಣ byte = 0;
    ಪ್ರತಿ (unsigned i = 0, ಎಣಿಕೆ = 0; ಎಣಿಕೆ < ARRAY_LENGTH (buf); ++i, ++ಎಣಿಕೆ)
    {
      unsigned nibble;
      ಯದಿ (!(i & 1))
      {
	ಯದಿ (unlikely (!str_ref.avail ())) ಭಙ್ಗ;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      ಅನ್ಯಥಾ
	nibble = byte & 0x0F;

      ಯದಿ (unlikely (nibble == RESERVED)) ಭಙ್ಗ;
      ಅನ್ಯಥಾ ಯದಿ (nibble == END)
      {
	ಧ್ರುವ ವರ್ಣ *p = buf;
	ದ್ವಿಗುಣ pv;
	ಯದಿ (unlikely (!hb_parse_double (&p, p + ಎಣಿಕೆ, &pv, ಸತ್ಯ/* whole buffer */)))
	  ಭಙ್ಗ;
	ಪ್ರತ್ಯಾಗಮ pv;
      }
      ಅನ್ಯಥಾ
      {
	buf[ಎಣಿಕೆ] = "0123456789.EE?-?"[nibble];
	ಯದಿ (nibble == EXP_NEG)
	{
	  ++ಎಣಿಕೆ;
	  ಯದಿ (unlikely (ಎಣಿಕೆ == ARRAY_LENGTH (buf))) ಭಙ್ಗ;
	  buf[ಎಣಿಕೆ] = '-';
	}
      }
    }

    str_ref.set_error ();
    ಪ್ರತ್ಯಾಗಮ .0;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ is_hint_op (op_code_t op)
  {
    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_ForceBold:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ OpCode_ExpansionFactor:
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      ಸ್ವಭಾವ:
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }
};

template <typename VAL=op_str_t>
ಸಂರಚನಾ ತುದಿ_dict_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, interp_env_t<ಸಂಖ್ಯೆ_t>& env, ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<VAL> & dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_FontMatrix:
	env.clear_args ();
	ಭಙ್ಗ;
      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಭಙ್ಗ;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=ಅಂಕೆ_interp_env_t>
ಸಂರಚನಾ dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ತಾರ್ಕಿಕ interpret (PARAM& param)
  {
    param.init ();
    ಯಾವತ್ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      ಯದಿ (unlikely (SUPER::env.in_error ()))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
