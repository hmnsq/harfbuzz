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

#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-common.hh"

ନାମାକାଶ CFF {

using ନାମାକାଶ OT;

/* an opstr and the parsed out dict value(s) */
ସଂରଚନା dict_val_t : op_str_t
{
  ଶୂନ୍ୟ init () {}
  ଶୂନ୍ୟ fini () {}
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_val_t ଅଙ୍କ_dict_val_t;

template <typename VAL> ସଂରଚନା dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : parsed_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL> {};

template <typename OPSTR=op_str_t>
ସଂରଚନା ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<OPSTR>
{
  ଶୂନ୍ୟ init ()
  {
    dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<OPSTR>::init ();
    charStringsOffset = 0;
    FDArrayOffset = 0;
  }
  ଶୂନ୍ୟ fini () { dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<OPSTR>::fini (); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା  charStringsOffset;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା  FDArrayOffset;
};

ସଂରଚନା dict_opset_t : opset_t<ସଂଖ୍ୟା_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, interp_env_t<ସଂଖ୍ୟା_t>& env)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_longintdict:  /* 5-byte integer */
	env.arg_ଗଦା.ଠେଲ_longint_from_substr (env.str_ref);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_BCD:  /* real number */
	env.arg_ଗଦା.ଠେଲ_real (parse_bcd (env.str_ref));
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	opset_t<ସଂଖ୍ୟା_t>::process_op (op, env);
	ଭାଙ୍ଗ;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  ସ୍ଥିର ଦ୍ୱିଗୁଣ parse_bcd (byte_str_ref_t& str_ref)
  {
    ଯଦି (unlikely (str_ref.in_error ())) ଫେରନ୍ତୁ .0;

    ଗଣନା Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, END };

    ଅକ୍ଷର buf[32] = {0};
    unsigned ଅକ୍ଷର byte = 0;
    ପାଇଁ (unsigned i = 0, ଗଣନା_2 = 0; ଗଣନା_2 < ARRAY_LENGTH (buf); ++i, ++ଗଣନା_2)
    {
      unsigned nibble;
      ଯଦି (!(i & 1))
      {
	ଯଦି (unlikely (!str_ref.avail ())) ଭାଙ୍ଗ;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      ଅନ୍ୟଥା
	nibble = byte & 0x0F;

      ଯଦି (unlikely (nibble == RESERVED)) ଭାଙ୍ଗ;
      ଅନ୍ୟଥା ଯଦି (nibble == END)
      {
	ଅଚଳ ଅକ୍ଷର *p = buf;
	ଦ୍ୱିଗୁଣ pv;
	ଯଦି (unlikely (!hb_parse_double (&p, p + ଗଣନା_2, &pv, ସତ୍ୟ/* whole buffer */)))
	  ଭାଙ୍ଗ;
	ଫେରନ୍ତୁ pv;
      }
      ଅନ୍ୟଥା
      {
	buf[ଗଣନା_2] = "0123456789.EE?-?"[nibble];
	ଯଦି (nibble == EXP_NEG)
	{
	  ++ଗଣନା_2;
	  ଯଦି (unlikely (ଗଣନା_2 == ARRAY_LENGTH (buf))) ଭାଙ୍ଗ;
	  buf[ଗଣନା_2] = '-';
	}
      }
    }

    str_ref.set_error ();
    ଫେରନ୍ତୁ .0;
  }

  ସ୍ଥିର ତାର୍କିକ is_hint_op (op_code_t op)
  {
    ବାଛ (op)
    {
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_ForceBold:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି OpCode_ExpansionFactor:
	ଫେରନ୍ତୁ ସତ୍ୟ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }
};

template <typename VAL=op_str_t>
ସଂରଚନା ଶୀର୍ଷ_dict_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, interp_env_t<ସଂଖ୍ୟା_t>& env, ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL> & dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_CharStrings:
	dictval.charStringsOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_FDArray:
	dictval.FDArrayOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_FontMatrix:
	env.clear_args ();
	ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଭାଙ୍ଗ;
    }
  }
};

template <typename OPSET, typename PARAM, typename ENV=ଅଙ୍କ_interp_env_t>
ସଂରଚନା dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  ତାର୍କିକ interpret (PARAM& param)
  {
    param.init ();
    ଯେତେବେଳେ (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      ଯଦି (unlikely (SUPER::env.in_error ()))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#endif /* HB_CFF_INTERP_DICT_COMMON_HH */
