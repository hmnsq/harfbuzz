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

#నిర్వచితం_కాకపోతే HB_OT_CFF2_పట్టిక_HH
#నిర్వచించు HB_OT_CFF2_పట్టిక_HH

#అన్తర్భావ "hb-ot-cff-common.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"
#అన్తర్భావ "hb-draw.hh"
#అన్తర్భావ "hb-paint.hh"

నామాకాశ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#నిర్వచించు HB_OT_ట్యాగ్_CFF2 HB_ట్యాగ్('C','F','F','2')

రకనిర్వచనం CFF2_సూచీ         CFF2_Char_అక్షరశ్రేణులు;
రకనిర్వచనం Subrs<HBUINT32>   CFF2Subrs;

రకనిర్వచనం FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
రకనిర్వచనం FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

నిర్మాణం CFF2FDSelect
{
  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి CFF2FDSelect &src, గుర్తులేని పూర్ణాంకం అంకె_glyphs)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని పూర్ణాంకం కొలత = src.get_కొలత (అంకె_glyphs);
    CFF2FDSelect *dest = c->allocate_కొలత<CFF2FDSelect> (కొలత);
    ఒకవేళ (unlikely (!dest)) return_trace (అబద్ధం);
    hb_memcpy (dest, &src, కొలత);
    return_trace (నిజం);
  }

  size_t get_కొలత (గుర్తులేని పూర్ణాంకం అంకె_glyphs) స్థిరరాశి
  {
    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి format.static_కొలత + u.format0.get_కొలత (అంకె_glyphs);
    స్థితి 3: hb_barrier (); తిరిగి format.static_కొలత + u.format3.get_కొలత ();
    స్థితి 4: hb_barrier (); తిరిగి format.static_కొలత + u.format4.get_కొలత ();
    అప్రమేయ:తిరిగి 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) స్థిరరాశి
  {
    ఒకవేళ (ఇది == &Null (CFF2FDSelect))
      తిరిగి 0;

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); తిరిగి u.format0.get_fd (glyph);
    స్థితి 3: hb_barrier (); తిరిగి u.format3.get_fd (glyph);
    స్థితి 4: hb_barrier (); తిరిగి u.format4.get_fd (glyph);
    అప్రమేయ:తిరిగి 0;
    }
  }

  తార్కిక sanitize (hb_sanitize_context_t *c, గుర్తులేని పూర్ణాంకం fdcount,
                 గుర్తులేని పూర్ణాంకం charstring_లెక్క = HB_CODEPOINT_INVALID) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!c->check_struct (ఇది)))
      return_trace (అబద్ధం);
    hb_barrier ();

    ఎంపిక (format)
    {
    స్థితి 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_లెక్క));
    స్థితి 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_లెక్క));
    స్థితి 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_లెక్క));
    అప్రమేయ:return_trace (అబద్ధం);
    }
  }

  HBUINT8	format;
  సంఘం {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  బహిరంగ:
  DEFINE_కొలత_MIN (2);
};

నిర్మాణం CFF2_అంశం_Variation_Store
{
  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  c->check_range (&varStore, కొలత) &&
		  varStore.sanitize (c));
  }

  తార్కిక serialize (hb_serialize_context_t *c, స్థిరరాశి CFF2_అంశం_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (ఇది);
    గుర్తులేని పూర్ణాంకం కొలత_3 = varStore->get_కొలత ();
    CFF2_అంశం_Variation_Store *dest = c->allocate_కొలత<CFF2_అంశం_Variation_Store> (కొలత_3);
    ఒకవేళ (unlikely (!dest)) return_trace (అబద్ధం);
    hb_memcpy (dest, varStore, కొలత_3);
    return_trace (నిజం);
  }

  size_t get_కొలత () స్థిరరాశి { తిరిగి HBUINT16::static_కొలత + కొలత; }

  HBUINT16	కొలత;
  అంశం_Variation_Store  varStore;

  DEFINE_కొలత_MIN (2 + అంశం_Variation_Store::min_కొలత);
};

నిర్మాణం cff2_శిఖరం_dict_విలువలు_t : శిఖరం_dict_విలువలు_t<>
{
  శూన్య init ()
  {
    శిఖరం_dict_విలువలు_t<>::init ();
    vstore_స్థానభ్రంశం = 0;
    FDSelect_స్థానభ్రంశం = 0;
  }
  శూన్య fini () { శిఖరం_dict_విలువలు_t<>::fini (); }

  పూర్ణాంకం  vstore_స్థానభ్రంశం;
  పూర్ణాంకం  FDSelect_స్థానభ్రంశం;
};

నిర్మాణం cff2_శిఖరం_dict_opset_t : శిఖరం_dict_opset_t<>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, అంకె_interp_env_t& env, cff2_శిఖరం_dict_విలువలు_t& dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Font_మాతృక:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.కూడు_op (op, env.str_ref);
	  env.clear_వాదనలు ();
	}
	ఆపు;

      స్థితి Op_సంకేతం_vstore:
	dictval.vstore_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_FDSelect:
	dictval.FDSelect_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref);
  }

  రకనిర్వచనం శిఖరం_dict_opset_t<> SUPER;
};

నిర్మాణం cff2_font_dict_విలువలు_t : dict_విలువలు_t<op_str_t>
{
  శూన్య init ()
  {
    dict_విలువలు_t<op_str_t>::init ();
    వ్యక్తిగత_Dict_Info.init ();
  }
  శూన్య fini () { dict_విలువలు_t<op_str_t>::fini (); }

  పట్టిక_info_t    వ్యక్తిగత_Dict_Info;
};

నిర్మాణం cff2_font_dict_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, అంకె_interp_env_t& env, cff2_font_dict_విలువలు_t& dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_వ్యక్తిగత:
	dictval.వ్యక్తిగత_Dict_Info.స్థానభ్రంశం = env.arg_దొంతర.తీయి_uint ();
	dictval.వ్యక్తిగత_Dict_Info.కొలత = env.arg_దొంతర.తీయి_uint ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ())
	  తిరిగి;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref);
  }

  అంతరంగ:
  రకనిర్వచనం dict_opset_t SUPER;
};

మూస <రకం_పేరు VAL>
నిర్మాణం cff2_వ్యక్తిగత_dict_విలువలు_పునాది_t : dict_విలువలు_t<VAL>
{
  శూన్య init ()
  {
    dict_విలువలు_t<VAL>::init ();
    subrs_స్థానభ్రంశం = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  శూన్య fini () { dict_విలువలు_t<VAL>::fini (); }

  పూర్ణాంకం                subrs_స్థానభ్రంశం;
  స్థిరరాశి CFF2Subrs   *localSubrs;
  గుర్తులేని పూర్ణాంకం      ivs;
};

రకనిర్వచనం cff2_వ్యక్తిగత_dict_విలువలు_పునాది_t<op_str_t> cff2_వ్యక్తిగత_dict_విలువలు_subset_t;
రకనిర్వచనం cff2_వ్యక్తిగత_dict_విలువలు_పునాది_t<అంకె_dict_val_t> cff2_వ్యక్తిగత_dict_విలువలు_t;

నిర్మాణం cff2_priv_dict_interp_env_t : అంకె_interp_env_t
{
  cff2_priv_dict_interp_env_t (స్థిరరాశి hb_ubytes_t &str) :
    అంకె_interp_env_t (str) {}

  శూన్య process_vsindex ()
  {
    ఒకవేళ (likely (!చూసిన_vsindex))
    {
      సమితి_ivs (arg_దొంతర.తీయి_uint ());
    }
    చూసిన_vsindex = నిజం;
  }

  గుర్తులేని పూర్ణాంకం get_ivs () స్థిరరాశి { తిరిగి ivs; }
  శూన్య	 సమితి_ivs (గుర్తులేని పూర్ణాంకం ivs_) { ivs = ivs_; }

  రక్షిత:
  గుర్తులేని పూర్ణాంకం  ivs = 0;
  తార్కిక	  చూసిన_vsindex = అబద్ధం;
};

నిర్మాణం cff2_వ్యక్తిగత_dict_opset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_priv_dict_interp_env_t& env, cff2_వ్యక్తిగత_dict_విలువలు_t& dictval)
  {
    అంకె_dict_val_t val;
    val.init ();

    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_Subrs:
	dictval.subrs_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_వాదనలు ();
	ఆపు;
      స్థితి Op_సంకేతం_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	dict_opset_t::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref, val);
  }
};

నిర్మాణం cff2_వ్యక్తిగత_dict_opset_subset_t : dict_opset_t
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff2_priv_dict_interp_env_t& env, cff2_వ్యక్తిగత_dict_విలువలు_subset_t& dictval)
  {
    ఎంపిక (op) {
      స్థితి Op_సంకేతం_Blue_విలువలు:
      స్థితి Op_సంకేతం_Other_Blues:
      స్థితి Op_సంకేతం_Family_Blues:
      స్థితి Op_సంకేతం_Family_Other_Blues:
      స్థితి Op_సంకేతం_Std_HW:
      స్థితి Op_సంకేతం_Std_VW:
      స్థితి Op_సంకేతం_Blue_Scale:
      స్థితి Op_సంకేతం_Blue_జరుపు:
      స్థితి Op_సంకేతం_Blue_Fuzz:
      స్థితి Op_సంకేతం_Stem_Snap_H:
      స్థితి Op_సంకేతం_Stem_Snap_V:
      స్థితి Op_సంకేతం_Language_సమూహం:
      స్థితి Op_సంకేతం_Expansion_గుణకం:
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_blenddict:
	env.clear_వాదనలు ();
	తిరిగి;

      స్థితి Op_సంకేతం_Subrs:
	dictval.subrs_స్థానభ్రంశం = env.arg_దొంతర.తీయి_int ();
	env.clear_వాదనలు ();
	ఆపు;

      స్థితి Op_సంకేతం_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_వాదనలు ();
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env);
	ఒకవేళ (!env.arg_దొంతర.is_empty ()) తిరిగి;
	ఆపు;
    }

    ఒకవేళ (unlikely (env.లోపల_error ())) తిరిగి;

    dictval.కూడు_op (op, env.str_ref);
  }

  అంతరంగ:
  రకనిర్వచనం dict_opset_t SUPER;
};

రకనిర్వచనం dict_interpreter_t<cff2_శిఖరం_dict_opset_t, cff2_శిఖరం_dict_విలువలు_t> cff2_శిఖరం_dict_interpreter_t;
రకనిర్వచనం dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_విలువలు_t> cff2_font_dict_interpreter_t;

నిర్మాణం CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  మూస <రకం_పేరు ITER, రకం_పేరు OP_SERIALIZER>
  తార్కిక serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { తిరిగి FDArray<HBUINT32>::serialize<cff2_font_dict_విలువలు_t, పట్టిక_info_t> (c, it, opszr); }
};

} /* namespace CFF */

నామాకాశ OT {

ఉపయోగించి నామాకాశ CFF;

నిర్మాణం cff2
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_CFF2;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  likely (సంస్కరణ.major == 2));
  }

  మూస <రకం_పేరు PRIVOPSET, రకం_పేరు PRIVDICTVAL>
  నిర్మాణం accelerator_templ_t
  {
    స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = cff2::పట్టిక_ట్యాగ్;

    accelerator_templ_t (hb_face_t *face)
    {
      ఒకవేళ (!face) తిరిగి;

      శిఖరం_Dict_2.init ();
      fontDicts.init ();
      వ్యక్తిగత_Dicts.init ();

      ఇది->blob = sc.reference_పట్టిక<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (ఇది->blob);
      sc.ప్రారంభం_processing ();

      స్థిరరాశి OT::cff2 *cff2 = ఇది->blob->మూస as<OT::cff2> ();

      ఒకవేళ (cff2 == &Null (OT::cff2))
        వెళ్ళు fail;

      { /* parse top dict */
	hb_ubytes_t శిఖరం_Dict_Str = (cff2 + cff2->శిఖరం_Dict_2).as_ubytes (cff2->శిఖరం_Dict_కొలత);
	ఒకవేళ (unlikely (!శిఖరం_Dict_Str.sanitize (&sc))) వెళ్ళు fail;
	hb_barrier ();
	అంకె_interp_env_t env (శిఖరం_Dict_Str);
	cff2_శిఖరం_dict_interpreter_t శిఖరం_interp (env);
	శిఖరం_Dict_2.init ();
	ఒకవేళ (unlikely (!శిఖరం_interp.interpret (శిఖరం_Dict_2))) వెళ్ళు fail;
      }

      globalSubrs = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2Subrs> (cff2, cff2->శిఖరం_Dict_2 + cff2->శిఖరం_Dict_కొలత, sc);
      varStore = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2_అంశం_Variation_Store> (cff2, శిఖరం_Dict_2.vstore_స్థానభ్రంశం, sc);
      char_అక్షరశ్రేణులు = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2_Char_అక్షరశ్రేణులు> (cff2, శిఖరం_Dict_2.char_అక్షరశ్రేణులు_స్థానభ్రంశం, sc);
      fdArray = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2FDArray> (cff2, శిఖరం_Dict_2.FDArray_స్థానభ్రంశం, sc);

      ఒకవేళ (char_అక్షరశ్రేణులు == &Null (CFF2_Char_అక్షరశ్రేణులు) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        వెళ్ళు fail;

      అంకె_glyphs = char_అక్షరశ్రేణులు->లెక్క;
      ఒకవేళ (అంకె_glyphs != sc.get_అంకె_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_పట్టిక (HB_ట్యాగ్ ('V','A','R','C'));
        తార్కిక valid_లెక్క = అంకె_glyphs < sc.get_అంకె_glyphs () &&
                           hb_blob_get_నిడివి (varc);
        hb_blob_destroy (varc);
        ఒకవేళ (!valid_లెక్క) వెళ్ళు fail;
      }

      fdSelect = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2FDSelect> (cff2, శిఖరం_Dict_2.FDSelect_స్థానభ్రంశం, sc,
                                                   fdArray->లెక్క, అంకె_glyphs);
      ఒకవేళ (శిఖరం_Dict_2.FDSelect_స్థానభ్రంశం && fdSelect == &Null (CFF2FDSelect))
        వెళ్ళు fail;

      fd_లెక్క = fdArray->లెక్క;
      ఒకవేళ (!వ్యక్తిగత_Dicts.resize (fd_లెక్క))
        వెళ్ళు fail;

      /* parse font dicts and gather private dicts */
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < fd_లెక్క; i++)
      {
	స్థిరరాశి hb_ubytes_t fontDictStr = (*fdArray)[i];
	ఒకవేళ (unlikely (!fontDictStr.sanitize (&sc))) వెళ్ళు fail;
	hb_barrier ();
	cff2_font_dict_విలువలు_t  *font;
	అంకె_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	ఒకవేళ (unlikely (font == &Crap (cff2_font_dict_విలువలు_t))) వెళ్ళు fail;
	font->init ();
	ఒకవేళ (unlikely (!font_interp.interpret (*font))) వెళ్ళు fail;

	స్థిరరాశి hb_ubytes_t privDictStr = Struct_At_స్థానభ్రంశం_Or_Null<UnsizedByteStr> (cff2, font->వ్యక్తిగత_Dict_Info.స్థానభ్రంశం, sc, font->వ్యక్తిగత_Dict_Info.కొలత).as_ubytes (font->వ్యక్తిగత_Dict_Info.కొలత);
	ఒకవేళ (unlikely (font->వ్యక్తిగత_Dict_Info.కొలత &&
		      privDictStr == (స్థిరరాశి గుర్తులేని అక్షరం *) &Null (UnsizedByteStr))) వెళ్ళు fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	వ్యక్తిగత_Dicts[i].init ();
	ఒకవేళ (unlikely (!priv_interp.interpret (వ్యక్తిగత_Dicts[i]))) వెళ్ళు fail;

	వ్యక్తిగత_Dicts[i].localSubrs = &Struct_At_స్థానభ్రంశం_Or_Null<CFF2Subrs> (&privDictStr[0], వ్యక్తిగత_Dicts[i].subrs_స్థానభ్రంశం, sc);
      }

      తిరిగి;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    శూన్య _fini ()
    {
      sc.చివర_processing ();
      శిఖరం_Dict_2.fini ();
      fontDicts.fini ();
      వ్యక్తిగత_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = శూన్యసూచిక;

      స్వయంచాలక *scalars = cached_scalars_సదిశ.get_acquire ();
      ఒకవేళ (scalars && cached_scalars_సదిశ.cmpexch (scalars, శూన్యసూచిక))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_సదిశ_t<uint16_t> *create_glyph_గమ్యం_sid_map () స్థిరరాశి
    {
      తిరిగి శూన్యసూచిక;
    }

    hb_blob_t *get_blob () స్థిరరాశి { తిరిగి blob; }

    తార్కిక is_valid () స్థిరరాశి { తిరిగి blob; }

    రక్షిత:
    hb_sanitize_context_t	sc;

    బహిరంగ:
    hb_blob_t			*blob = శూన్యసూచిక;
    cff2_శిఖరం_dict_విలువలు_t	శిఖరం_Dict_2;
    స్థిరరాశి CFF2Subrs		*globalSubrs = శూన్యసూచిక;
    స్థిరరాశి CFF2_అంశం_Variation_Store	*varStore = శూన్యసూచిక;
    స్థిరరాశి CFF2_Char_అక్షరశ్రేణులు	*char_అక్షరశ్రేణులు = శూన్యసూచిక;
    స్థిరరాశి CFF2FDArray		*fdArray = శూన్యసూచిక;
    స్థిరరాశి CFF2FDSelect		*fdSelect = శూన్యసూచిక;
    గుర్తులేని పూర్ణాంకం		fd_లెక్క = 0;

    hb_సదిశ_t<cff2_font_dict_విలువలు_t>     fontDicts;
    hb_సదిశ_t<PRIVDICTVAL>  వ్యక్తిగత_Dicts;

    మార్చదగిన hb_atomic_t<hb_సదిశ_t<దశాంశం> *> cached_scalars_సదిశ;

    గుర్తులేని పూర్ణాంకం	      అంకె_glyphs = 0;
  };

  నిర్మాణం accelerator_t : accelerator_templ_t<cff2_వ్యక్తిగత_dict_opset_t, cff2_వ్యక్తిగత_dict_విలువలు_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_అంతర్గత తార్కిక get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) స్థిరరాశి;
    HB_అంతర్గత తార్కిక get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<స్థిరరాశి పూర్ణాంకం> coords,
				     int64_t *budget = శూన్యసూచిక) స్థిరరాశి;
    HB_అంతర్గత తార్కిక get_మార్గం (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = శూన్యసూచిక) స్థిరరాశి;
    HB_అంతర్గత తార్కిక get_మార్గం_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<స్థిరరాశి పూర్ణాంకం> coords, int64_t *budget = శూన్యసూచిక) స్థిరరాశి;
  };

  నిర్మాణం accelerator_subset_t : accelerator_templ_t<cff2_వ్యక్తిగత_dict_opset_subset_t, cff2_వ్యక్తిగత_dict_విలువలు_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ఒకవేళ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_అంతర్గత తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి;
    HB_అంతర్గత తార్కిక serialize (hb_serialize_context_t *c,
				నిర్మాణం cff2_subset_plan &plan,
				hb_array_t<పూర్ణాంకం> normalized_coords) స్థిరరాశి;

    మార్చదగిన CFF::cff_subset_accelerator_t* cff_accelerator = శూన్యసూచిక;

    రకనిర్వచనం accelerator_templ_t<cff2_వ్యక్తిగత_dict_opset_subset_t, cff2_వ్యక్తిగత_dict_విలువలు_subset_t> SUPER;
  };

  బహిరంగ:
  Fixed_సంస్కరణ<HBUINT8>		సంస్కరణ;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_గమ్యం<శిఖరం_Dict, HBUINT8>	శిఖరం_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			శిఖరం_Dict_కొలత;	/* Top DICT size */

  బహిరంగ:
  DEFINE_కొలత_STATIC (5);
};

నిర్మాణం cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

నిర్మాణం cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#అయితే_ముగింపు /* HB_OT_CFF2_TABLE_HH */
