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
#నిర్వచితం_కాకపోతే HB_CFF_INTERP_DICT_COMMON_HH
#నిర్వచించు HB_CFF_INTERP_DICT_COMMON_HH

#అన్తర్భావ "hb-cff-interp-common.hh"

నామాకాశ CFF {

ఉపయోగించి నామాకాశ OT;

/* an opstr and the parsed out dict value(s) */
నిర్మాణం dict_val_t : op_str_t
{
  శూన్య init () {}
  శూన్య fini () {}
};

రకనిర్వచనం dict_val_t అంకె_dict_val_t;

మూస <రకం_పేరు VAL> నిర్మాణం dict_విలువలు_t : parsed_విలువలు_t<VAL> {};

మూస <రకం_పేరు OPSTR=op_str_t>
నిర్మాణం శిఖరం_dict_విలువలు_t : dict_విలువలు_t<OPSTR>
{
  శూన్య init ()
  {
    dict_విలువలు_t<OPSTR>::init ();
    char_అక్షరశ్రేణులు_స్థానభ్రంశం = 0;
    FDArray_స్థానభ్రంశం = 0;
  }
  శూన్య fini () { dict_విలువలు_t<OPSTR>::fini (); }

  పూర్ణాంకం  char_అక్షరశ్రేణులు_స్థానభ్రంశం;
  పూర్ణాంకం  FDArray_స్థానభ్రంశం;
};

నిర్మాణం dict_opset_t : opset_t<సంఖ్య_t>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, interp_env_t<సంఖ్య_t>& env)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_longintdict:  /* 5-byte integer */
	env.arg_దొంతర.నెట్టు_longint_from_substr (env.str_ref);
	ఆపు;

      స్థితి Op_సంకేతం_BCD:  /* real number */
	env.arg_దొంతర.నెట్టు_real (parse_bcd (env.str_ref));
	ఆపు;

      అప్రమేయ:
	opset_t<సంఖ్య_t>::process_op (op, env);
	ఆపు;
    }
  }

  /* Turns CFF's BCD format into strtod understandable string */
  స్థిర ద్విగుణ parse_bcd (byte_str_ref_t& str_ref)
  {
    ఒకవేళ (unlikely (str_ref.లోపల_error ())) తిరిగి .0;

    గణన Nibble { DECIMAL=10, EXP_POS, EXP_NEG, RESERVED, NEG, చివర_2 };

    అక్షరం buf[32] = {0};
    గుర్తులేని అక్షరం byte = 0;
    కొరకు (గుర్తులేని i = 0, లెక్క = 0; లెక్క < ARRAY_నిడివి (buf); ++i, ++లెక్క)
    {
      గుర్తులేని nibble;
      ఒకవేళ (!(i & 1))
      {
	ఒకవేళ (unlikely (!str_ref.avail ())) ఆపు;

	byte = str_ref[0];
	str_ref.inc ();
	nibble = byte >> 4;
      }
      లేకపోతే
	nibble = byte & 0x0F;

      ఒకవేళ (unlikely (nibble == RESERVED)) ఆపు;
      లేకపోతే ఒకవేళ (nibble == చివర_2)
      {
	స్థిరరాశి అక్షరం *p = buf;
	ద్విగుణ pv;
	ఒకవేళ (unlikely (!hb_parse_double (&p, p + లెక్క, &pv, నిజం/* whole buffer */)))
	  ఆపు;
	తిరిగి pv;
      }
      లేకపోతే
      {
	buf[లెక్క] = "0123456789.EE?-?"[nibble];
	ఒకవేళ (nibble == EXP_NEG)
	{
	  ++లెక్క;
	  ఒకవేళ (unlikely (లెక్క == ARRAY_నిడివి (buf))) ఆపు;
	  buf[లెక్క] = '-';
	}
      }
    }

    str_ref.సమితి_error ();
    తిరిగి .0;
  }

  స్థిర తార్కిక is_hint_op (op_సంకేతం_t op)
  {
    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Force_Bold:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
	తిరిగి నిజం;
      అప్రమేయ:
	తిరిగి అబద్ధం;
    }
  }
};

మూస <రకం_పేరు VAL=op_str_t>
నిర్మాణం శిఖరం_dict_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, interp_env_t<సంఖ్య_t>& env, శిఖరం_dict_విలువలు_t<VAL> & dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Char_అక్షరశ్రేణులు:
	dictval.char_అక్షరశ్రేణులు_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_FDArray:
	dictval.FDArray_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_Font_మాతృక:
	env.clear_వాదనలు ();
	ఆపు;
      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఆపు;
    }
  }
};

మూస <రకం_పేరు OPSET, రకం_పేరు PARAM, రకం_పేరు ENV=అంకె_interp_env_t>
నిర్మాణం dict_interpreter_t : interpreter_t<ENV>
{
  dict_interpreter_t (ENV& env_) : interpreter_t<ENV> (env_) {}

  తార్కిక interpret (PARAM& param)
  {
    param.init ();
    అయితే (SUPER::env.str_ref.avail ())
    {
      OPSET::process_op (SUPER::env.fetch_op (), SUPER::env, param);
      ఒకవేళ (unlikely (SUPER::env.లోపల_error ()))
	తిరిగి అబద్ధం;
    }

    తిరిగి నిజం;
  }

  అంతరంగ:
  రకనిర్వచనం interpreter_t<ENV> SUPER;
};

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF_INTERP_DICT_COMMON_HH */
