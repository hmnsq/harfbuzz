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

#અન્તર્ભાવ "hb-cff-interp-common.hh"

નામાકાશ CFF {

using નામાકાશ OT;

/* an opstr and the parsed out dict value(s) */
સંરચના dict_val_t : op_str_t
{
  શૂન્ય init () {}
  શૂન્ય fini () {}
};

પ્રકારવ્યાખ્યા dict_val_t અંક_dict_val_t;

template <typename VAL> સંરચના dict_કિંમતો_t : parsed_કિંમતો_t<VAL> {};

template <typename OPSTR=op_str_t>
સંરચના ટોચ_dict_કિંમતો_t : dict_કિંમતો_t<OPSTR>
{
  શૂન્ય init ()
  {
    dict_કિંમતો_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  શૂન્ય fini () { dict_કિંમતો_t<OPSTR>::fini (); }

  પૂર્ણાંક  charStringsOffset;
  પૂર્ણાંક  FDArrayOffset;
};

સંરચના dict_opset_t : opset_t<સંખ્યા_t>
{
  સ્થિર શૂન્ય process_op (op_code_t op, interp_env_t<સંખ્યા_t>& env)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_longintdict:  /* 5-byte integer */
	env.arg_ઢગલો.ધકેલો_longint_from_substr (env.str_ref);
	તોડો;

      સ્થિતિ OpCode_BCD:  /* real number */
	env.arg_ઢગલો.ધકેલો_real (parse_bcd (env.str_ref));
	તોડો;

      મૂળભૂત:
	opset_t<સંખ્યા_t>::process_op (op, env);
	તોડો;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  સ્થિર દ્વિગુણ parse_bcd (byte_str_ref_t& str_ref)
  {
    જો (unlikely (str_ref.in_error ())) પાછા .0;

    ગણના Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    અક્ષર buf[32] = {0};
    unsigned અક્ષર byte = 0;
    માટે (unsigned i = 0, ગણતરી = 0; ગણતરી < ARRAY_LENGTH (buf); ++i, ++ગણતરી)
    {
      unsigned nibble;
      જો (!(i & 1))
      {
	જો (unlikely (!str_ref.avail ())) તોડો;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      નહીંતર
	nibble = byte & 0x0F;

      જો (unlikely (nibble == RESERVED)) તોડો;
      નહીંતર જો (nibble == END)
      {
	અચલ અક્ષર *p = buf;
	દ્વિગુણ pv;
	જો (unlikely (!hb_parse_double (&p, p + ગણતરી, &pv, સાચું/* whole buffer */)))
	  તોડો;
	પાછા pv;
      }
      નહીંતર
      {
	buf[ગણતરી] = "0123456789.EE?-?"[nibble];
	જો (nibble == EXP_NEG)
	{
	  ++ગણતરી;
	  જો (unlikely (ગણતરી == ARRAY_LENGTH (buf))) તોડો;
	  buf[ગણતરી] = '-';
	}
      }
    }

    str_ref.set_error ();
    પાછા .0;
  }

  સ્થિર તાર્કિક is_hint_op (op_code_t op)
  {
    પસંદગી (op)
    {
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_ForceBold:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ OpCode_ExpansionFactor:
	પાછા સાચું;
      મૂળભૂત:
	પાછા ખોટું;
    }
  }
};

template <typename VAL=op_str_t>
સંરચના ટોચ_dict_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, interp_env_t<સંખ્યા_t>& env, ટોચ_dict_કિંમતો_t<VAL> & dictval)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_FontMatrix:
	env.clear_args ();
	તોડો;
      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	તોડો;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=અંક_interp_env_t>
સંરચના dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  તાર્કિક interpret (PARAM& param)
  {
    param.init ();
    જ્યારે (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      જો (unlikely (SUPER::env.in_error ()))
	પાછા ખોટું;
    }

    પાછા સાચું;
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
