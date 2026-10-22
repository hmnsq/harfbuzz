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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-cff1-table.hh"
#అన్తర్భావ "hb-set.h"
#అన్తర్భావ "hb-bimap.hh"
#అన్తర్భావ "hb-subset-plan.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"
#అన్తర్భావ "hb-cff1-interp-cs.hh"

ఉపయోగించి నామాకాశ CFF;

నిర్మాణం remap_sid_t
{
  గుర్తులేని get_population () స్థిరరాశి { తిరిగి vector.నిడివి; }

  శూన్య alloc (గుర్తులేని కొలత)
  {
    map.alloc (కొలత);
    vector.alloc_exact (కొలత);
  }

  తార్కిక లోపల_error () స్థిరరాశి
  { తిరిగి map.లోపల_error () || vector.లోపల_error (); }

  గుర్తులేని పూర్ణాంకం కూడు (గుర్తులేని పూర్ణాంకం sid)
  {
    ఒకవేళ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      తిరిగి sid;

    sid = unoffset_sid (sid);
    గుర్తులేని v = next;
    ఒకవేళ (map.సమితి (sid, v, అబద్ధం))
    {
      vector.push (sid);
      next++;
    }
    లేకపోతే
      v = map.get (sid); // already exists
    తిరిగి స్థానభ్రంశం_sid (v);
  }

  గుర్తులేని పూర్ణాంకం క్రియాకారకం[] (గుర్తులేని పూర్ణాంకం sid) స్థిరరాశి
  {
    ఒకవేళ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      తిరిగి sid;

    తిరిగి స్థానభ్రంశం_sid (map.get (unoffset_sid (sid)));
  }

  స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం అంకె_std_అక్షరశ్రేణులు = 391;

  స్థిర తార్కిక is_std_str (గుర్తులేని పూర్ణాంకం sid) { తిరిగి sid < అంకె_std_అక్షరశ్రేణులు; }
  స్థిర గుర్తులేని పూర్ణాంకం స్థానభ్రంశం_sid (గుర్తులేని పూర్ణాంకం sid) { తిరిగి sid + అంకె_std_అక్షరశ్రేణులు; }
  స్థిర గుర్తులేని పూర్ణాంకం unoffset_sid (గుర్తులేని పూర్ణాంకం sid) { తిరిగి sid - అంకె_std_అక్షరశ్రేణులు; }
  గుర్తులేని next = 0;

  hb_map_t map;
  hb_సదిశ_t<గుర్తులేని> vector;
};

నిర్మాణం cff1_sub_పట్టిక_info_t : cff_sub_పట్టిక_info_t
{
  cff1_sub_పట్టిక_info_t ()
    : cff_sub_పట్టిక_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    వ్యక్తిగత_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  పట్టిక_info_t	వ్యక్తిగత_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
నిర్మాణం cff1_శిఖరం_dict_విలువలు_mod_t : cff1_శిఖరం_dict_విలువలు_t
{
  శూన్య init (స్థిరరాశి cff1_శిఖరం_dict_విలువలు_t *పునాది_2= &Null (cff1_శిఖరం_dict_విలువలు_t))
  {
    SUPER::init ();
    పునాది = పునాది_2;
  }

  శూన్య fini () { SUPER::fini (); }

  గుర్తులేని get_లెక్క () స్థిరరాశి { తిరిగి పునాది->get_లెక్క () + SUPER::get_లెక్క (); }
  స్థిరరాశి cff1_శిఖరం_dict_val_t &get_విలువ (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ఒకవేళ (i < పునాది->get_లెక్క ())
      తిరిగి (*పునాది)[i];
    లేకపోతే
      తిరిగి SUPER::values[i - పునాది->get_లెక్క ()];
  }
  స్థిరరాశి cff1_శిఖరం_dict_val_t &క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి get_విలువ (i); }

  శూన్య reassignSIDs (స్థిరరాశి remap_sid_t& sidmap)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < name_dict_విలువలు_t::Val_లెక్క; i++)
      nameSIDs[i] = sidmap[పునాది->nameSIDs[i]];
  }

  రక్షిత:
  రకనిర్వచనం cff1_శిఖరం_dict_విలువలు_t SUPER;
  స్థిరరాశి cff1_శిఖరం_dict_విలువలు_t *పునాది;
};

నిర్మాణం శిఖరం_dict_modifiers_t
{
  శిఖరం_dict_modifiers_t (స్థిరరాశి cff1_sub_పట్టిక_info_t &info_,
			స్థిరరాశి గుర్తులేని పూర్ణాంకం (&nameSIDs_)[name_dict_విలువలు_t::Val_లెక్క])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  స్థిరరాశి cff1_sub_పట్టిక_info_t &info;
  స్థిరరాశి గుర్తులేని పూర్ణాంకం	(&nameSIDs)[name_dict_విలువలు_t::Val_లెక్క];
};

నిర్మాణం cff1_శిఖరం_dict_op_serializer_t : cff_శిఖరం_dict_op_serializer_t<cff1_శిఖరం_dict_val_t>
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి cff1_శిఖరం_dict_val_t &opstr,
		  స్థిరరాశి శిఖరం_dict_modifiers_t &mod) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    op_సంకేతం_t op = opstr.op;
    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_charset:
	ఒకవేళ (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	లేకపోతే
	  వెళ్ళు fall_వెనుక;

      స్థితి Op_సంకేతం_Encoding:
	ఒకవేళ (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	లేకపోతే
	  వెళ్ళు fall_వెనుక;

      స్థితి Op_సంకేతం_వ్యక్తిగత:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.వ్యక్తిగత_Dict_Info.కొలత) &&
		      Dict::serialize_link4_op (c, op, mod.info.వ్యక్తిగత_Dict_Info.link, whence_t::Absolute));

      స్థితి Op_సంకేతం_సంస్కరణ:
      స్థితి Op_సంకేతం_Notice:
      స్థితి Op_సంకేతం_Copyright:
      స్థితి Op_సంకేతం_నిండిన_Name:
      స్థితి Op_సంకేతం_Family_Name:
      స్థితి Op_సంకేతం_బరువు:
      స్థితి Op_సంకేతం_Post_Script:
      స్థితి Op_సంకేతం_పునాది_Font_Name:
      స్థితి Op_సంకేతం_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_విలువలు_t::name_op_గమ్యం_సూచీ (op)]));

      స్థితి Op_సంకేతం_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  ఒకవేళ ( unlikely (!(opstr.నిడివి >= opstr.చివరి_arg_స్థానభ్రంశం + 3)))
	    return_trace (అబద్ధం);
	  supp_op.ptr = opstr.ptr + opstr.చివరి_arg_స్థానభ్రంశం;
	  supp_op.నిడివి = opstr.నిడివి - opstr.చివరి_arg_స్థానభ్రంశం;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_విలువలు_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_విలువలు_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_వెనుక:
      అప్రమేయ:
	return_trace (cff_శిఖరం_dict_op_serializer_t<cff1_శిఖరం_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (నిజం);
  }

};

నిర్మాణం cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి op_str_t &opstr,
		  స్థిరరాశి cff1_font_dict_విలువలు_mod_t &mod) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (opstr.op == Op_సంకేతం_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    లేకపోతే
      return_trace (SUPER::serialize (c, opstr, mod.వ్యక్తిగత_Dict_Info));
  }

  అంతరంగ:
  రకనిర్వచనం cff_font_dict_op_serializer_t SUPER;
};

నిర్మాణం cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  స్థిర శూన్య flush_వాదనలు_and_op (op_సంకేతం_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ఒకవేళ (env.arg_ప్రారంభం > 0)
      flush_వెడల్పు (env, param);

    ఎంపిక (op)
    {
      స్థితి Op_సంకేతం_hstem:
      స్థితి Op_సంకేతం_hstemhm:
      స్థితి Op_సంకేతం_vstem:
      స్థితి Op_సంకేతం_vstemhm:
      స్థితి Op_సంకేతం_hintmask:
      స్థితి Op_సంకేతం_cntrmask:
      స్థితి Op_సంకేతం_dotsection:
	ఒకవేళ (param.drop_hints)
	{
	  env.clear_వాదనలు ();
	  తిరిగి;
	}
	HB_FALLTHROUGH;

      అప్రమేయ:
	SUPER::flush_వాదనలు_and_op (op, env, param);
	ఆపు;
    }
  }
  స్థిర శూన్య flush_వాదనలు (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    కొరకు (గుర్తులేని పూర్ణాంకం i = env.arg_ప్రారంభం; i < env.arg_దొంతర.get_లెక్క (); i++)
      encoder.encode_అంకె_cs (env.eval_arg (i));
    SUPER::flush_వాదనలు (env, param);
  }

  స్థిర శూన్య flush_op (op_సంకేతం_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  స్థిర శూన్య flush_వెడల్పు (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ప్రతిజ్ఞా (env.has_వెడల్పు);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_అంకె_cs (env.వెడల్పు);
  }

  స్థిర శూన్య flush_hintmask (op_సంకేతం_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    ఒకవేళ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.hintmask_కొలత; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  అంతరంగ:
  రకనిర్వచనం cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

నిర్మాణం range_list_t : hb_సదిశ_t<సంకేతం_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  తార్కిక complete (గుర్తులేని పూర్ణాంకం చివరి_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    గుర్తులేని లెక్క = ఇది->నిడివి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = లెక్క; i; i--)
    {
      సంకేతం_pair_t &pair = arrayZ[i - 1];
      గుర్తులేని పూర్ణాంకం n_ఎడమ = చివరి_glyph - pair.glyph - 1;
      all_glyphs |= n_ఎడమ;
      చివరి_glyph = pair.glyph;
      pair.glyph = n_ఎడమ;
    }
    తార్కిక two_byte = all_glyphs >= 0x100;
    తిరిగి two_byte;
  }
};

నిర్మాణం cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  స్థిర శూన్య process_op (op_సంకేతం_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    ఎంపిక (op) {

      స్థితి Op_సంకేతం_return:
	param.current_parsed_str->కూడు_op (op, env.str_ref);
	param.current_parsed_str->సమితి_parsed ();
	env.return_from_subr ();
	param.సమితి_current_str (env, అబద్ధం);
	ఆపు;

      స్థితి Op_సంకేతం_endchar:
	param.current_parsed_str->కూడు_op (op, env.str_ref);
	param.current_parsed_str->సమితి_parsed ();
	SUPER::process_op (op, env, param);
	ఆపు;

      స్థితి Op_సంకేతం_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_సంవృతం_2);
	ఆపు;

      స్థితి Op_సంకేతం_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_సంవృతం);
	ఆపు;

      అప్రమేయ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->కూడు_op (op, env.str_ref);
	ఆపు;
    }
  }

  రక్షిత:
  స్థిర శూన్య process_call_subr (op_సంకేతం_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_సమితి_t *సంవృతం)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->కూడు_call_op (op, str_ref, env.context.subr_అంకె_2);
    సంవృతం->కూడు (env.context.subr_అంకె_2);
    param.సమితి_current_str (env, నిజం);
  }

  అంతరంగ:
  రకనిర్వచనం cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

నిర్మాణం cff1_వ్యక్తిగత_dict_op_serializer_t : op_serializer_t
{
  cff1_వ్యక్తిగత_dict_op_serializer_t (తార్కిక desubroutinize_, తార్కిక drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి op_str_t &opstr,
		  objidx_t subrs_link) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (నిజం);

    ఒకవేళ (opstr.op == Op_సంకేతం_Subrs)
    {
      ఒకవేళ (desubroutinize || !subrs_link)
	return_trace (నిజం);
      లేకపోతే
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  రక్షిత:
  స్థిరరాశి తార్కిక desubroutinize;
  స్థిరరాశి తార్కిక drop_hints;
};

నిర్మాణం cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, స్థిరరాశి OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_సంకేతం_endchar>
{
  cff1_subr_subsetter_t (స్థిరరాశి OT::cff1::accelerator_subset_t &acc_, స్థిరరాశి hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  స్థిర శూన్య complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    ఒకవేళ (env.has_వెడల్పు)
      charstring.సమితి_ఉపసర్గ (env.వెడల్పు);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->సమితి_parsed ();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < env.call_దొంతర.get_లెక్క (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_దొంతర[i]);
      ఒకవేళ (likely (parsed_str))
	parsed_str->సమితి_parsed ();
      లేకపోతే
	env.సమితి_error ();
    }
  }
};

నామాకాశ OT {
నిర్మాణం cff1_subset_plan
{
  cff1_subset_plan ()
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < name_dict_విలువలు_t::Val_లెక్క; i++)
      శిఖరం_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  శూన్య plan_subset_encoding (స్థిరరాశి OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    స్థిరరాశి Encoding *encoding = acc.encoding;
    గుర్తులేని పూర్ణాంకం  size0, size1;
    గుర్తులేని సంకేతం_2, చివరి_సంకేతం_2 = CFF_UNDEF_సంకేతం - 1;
    hb_సదిశ_t<hb_codepoint_t> supp_codes;

    subset_enc_సంకేతం_ranges.clear ();
    ఒకవేళ (unlikely (subset_enc_సంకేతం_ranges.లోపల_error ()))
    {
      plan->check_success (అబద్ధం);
      తిరిగి;
    }

    supp_codes.init ();

    సంకేతం_pair_t glyph_గమ్యం_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_అంకె_codes = plan->అంకె_output_glyphs () - 1;
    గుర్తులేని పూర్ణాంకం glyph;
    స్వయంచాలక it = hb_iter (plan->new_గమ్యం_old_gid_list);
    ఒకవేళ (it->మొదటి == 0) it++;
    స్వయంచాలక _ = *it;
    కొరకు (glyph = 1; glyph < అంకె_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ఒకవేళ (glyph == _.మొదటి)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      లేకపోతే
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      సంకేతం_2 = acc.glyph_గమ్యం_సంకేతం (old_glyph, &glyph_గమ్యం_sid_cache);
      ఒకవేళ (సంకేతం_2 == CFF_UNDEF_సంకేతం)
      {
	subset_enc_అంకె_codes = glyph - 1;
	ఆపు;
      }

      ఒకవేళ (సంకేతం_2 != చివరి_సంకేతం_2 + 1)
	subset_enc_సంకేతం_ranges.push (సంకేతం_pair_t {సంకేతం_2, glyph});
      చివరి_సంకేతం_2 = సంకేతం_2;

      ఒకవేళ (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_గమ్యం_sid (old_glyph, &glyph_గమ్యం_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < supp_codes.నిడివి; i++)
	  subset_enc_supp_codes.push (సంకేతం_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_సంకేతం_ranges.complete (glyph);

    ప్రతిజ్ఞా (subset_enc_అంకె_codes <= 0xFF);
    size0 = Encoding0::min_కొలత + HBUINT8::static_కొలత * subset_enc_అంకె_codes;
    size1 = Encoding1::min_కొలత + Encoding1_Range::static_కొలత * subset_enc_సంకేతం_ranges.నిడివి;

    ఒకవేళ (size0 < size1)
      subset_enc_format = 0;
    లేకపోతే
      subset_enc_format = 1;
  }

  తార్కిక plan_subset_charset (స్థిరరాశి OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    గుర్తులేని పూర్ణాంకం  size0, కొలత_ranges;
    గుర్తులేని చివరి_sid = CFF_UNDEF_సంకేతం - 1;

    subset_charset_ranges.clear ();
    ఒకవేళ (unlikely (subset_charset_ranges.లోపల_error ()))
    {
      plan->check_success (అబద్ధం);
      తిరిగి అబద్ధం;
    }

    సంకేతం_pair_t glyph_గమ్యం_sid_cache {0, HB_CODEPOINT_INVALID};

    గుర్తులేని అంకె_glyphs = plan->అంకె_output_glyphs ();

    ఒకవేళ (unlikely (!subset_charset_ranges.alloc (hb_min (అంకె_glyphs,
							acc.అంకె_charset_entries))))
    {
      plan->check_success (అబద్ధం);
      తిరిగి అబద్ధం;
    }

    glyph_గమ్యం_sid_map_t *glyph_గమ్యం_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_గమ్యం_sid_map.get_acquire () :
					   శూన్యసూచిక;

    తార్కిక created_map = అబద్ధం;
    ఒకవేళ (!glyph_గమ్యం_sid_map && acc.cff_accelerator)
    {
      created_map = నిజం;
      glyph_గమ్యం_sid_map = acc.create_glyph_గమ్యం_sid_map ();
    }

    ఒకవేళ (!glyph_గమ్యం_sid_map || glyph_గమ్యం_sid_map->లోపల_error ())
    {
      glyph_గమ్యం_sid_map = శూన్యసూచిక;
      created_map = అబద్ధం;
    }

    స్వయంచాలక it = hb_iter (plan->new_గమ్యం_old_gid_list);
    ఒకవేళ (it->మొదటి == 0) it++;
    స్వయంచాలక _ = *it;
    తార్కిక not_is_cid = !acc.is_CID ();
    తార్కిక skip = !not_is_cid && glyph_గమ్యం_sid_map && !తటస్థం_charset;
    ఒకవేళ (not_is_cid)
      sidmap.alloc (అంకె_glyphs);
    కొరకు (hb_codepoint_t glyph = 1; glyph < అంకె_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ఒకవేళ (glyph == _.మొదటి)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      లేకపోతే
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      గుర్తులేని sid;
      ఒకవేళ (తటస్థం_charset)
	sid = glyph;
      లేకపోతే
      {
	sid = glyph_గమ్యం_sid_map ?
		       glyph_గమ్యం_sid_map->arrayZ[old_glyph].సంకేతం_2 :
		       acc.glyph_గమ్యం_sid (old_glyph, &glyph_గమ్యం_sid_cache);

	ఒకవేళ (not_is_cid)
	  sid = sidmap.కూడు (sid);
      }

      ఒకవేళ (sid != చివరి_sid + 1)
	subset_charset_ranges.push (సంకేతం_pair_t {sid, glyph});

      ఒకవేళ (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.మొదటి - 1, glyph_గమ్యం_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      చివరి_sid = sid;
    }

    ఒకవేళ (created_map)
    {
      ఒకవేళ ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_గమ్యం_sid_map.cmpexch (శూన్యసూచిక, glyph_గమ్యం_sid_map))
      {
	glyph_గమ్యం_sid_map->~glyph_గమ్యం_sid_map_t ();
	hb_free (glyph_గమ్యం_sid_map);
      }
    }

    తార్కిక two_byte = subset_charset_ranges.complete (అంకె_glyphs);

    size0 = Charset0::get_కొలత (plan->అంకె_output_glyphs ());
    ఒకవేళ (!two_byte)
      కొలత_ranges = Charset1::get_కొలత_for_ranges (subset_charset_ranges.నిడివి);
    లేకపోతే
      కొలత_ranges = Charset2::get_కొలత_for_ranges (subset_charset_ranges.నిడివి);

    ఒకవేళ (size0 < కొలత_ranges)
      subset_charset_format = 0;
    లేకపోతే ఒకవేళ (!two_byte)
      subset_charset_format = 1;
    లేకపోతే
      subset_charset_format = 2;

    తిరిగి నిజం;
  }

  తార్కిక collect_sids_లోపల_dicts (స్థిరరాశి OT::cff1::accelerator_subset_t &acc)
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < name_dict_విలువలు_t::Val_లెక్క; i++)
    {
      గుర్తులేని పూర్ణాంకం sid = acc.శిఖరం_Dict_2.nameSIDs[i];
      ఒకవేళ (sid != CFF_UNDEF_SID)
      {
	శిఖరం_Dict_Mod_SIDs[i] = sidmap.కూడు (sid);
      }
    }

    ఒకవేళ (acc.fdArray != &Null (CFF1FDArray))
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < orig_fdcount; i++)
	ఒకవేళ (fdmap.has (i))
	  (శూన్య)sidmap.కూడు (acc.fontDicts[i].fontName);

    తిరిగి నిజం;
  }

  తార్కిక create (స్థిరరాశి OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ఒకవేళ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) తిరిగి అబద్ధం;

    అంకె_glyphs = plan->అంకె_output_glyphs ();
    orig_fdcount = acc.fd_లెక్క;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    తటస్థం_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_తటస్థం_CHARSET) && acc.is_CID ();

 #నిర్వచితమైతే HB_EXPERIMENTAL_API
    min_charstrings_off_కొలత = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #లేకపోతే
    min_charstrings_off_కొలత = 0;
 #అయితే_ముగింపు

    subset_charset = !acc.is_predef_charset () || తటస్థం_charset;
    ఒకవేళ (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      కొరకు (స్థిరరాశి స్వయంచాలక &_ : plan->new_గమ్యం_old_gid_list)
      {
	ఒకవేళ (_.మొదటి != _.second)
	{
	  subset_charset = నిజం;
	  ఆపు;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.శిఖరం_Dict_2);
      తార్కిక need_గమ్యం_కూడు_enc = (subset_encoding && !acc.శిఖరం_Dict_2.has_op (Op_సంకేతం_Encoding));
      తార్కిక need_గమ్యం_కూడు_సమితి = (subset_charset && !acc.శిఖరం_Dict_2.has_op (Op_సంకేతం_charset));
      ఒకవేళ (need_గమ్యం_కూడు_enc || need_గమ్యం_కూడు_సమితి)
      {
	ఒకవేళ (need_గమ్యం_కూడు_enc)
	  topdict_mod.కూడు_op (Op_సంకేతం_Encoding);
	ఒకవేళ (need_గమ్యం_కూడు_సమితి)
	  topdict_mod.కూడు_op (Op_సంకేతం_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    ఒకవేళ (acc.fdSelect != &Null (CFF1FDSelect))
    {
	ఒకవేళ (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.అంకె_glyphs,
				  subset_fdcount,
				  info.fd_select.కొలత,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	తిరిగి అబద్ధం;
    }
    లేకపోతే
      fdmap.తటస్థం (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      ఒకవేళ (unlikely (!collect_sids_లోపల_dicts (acc)))
	తిరిగి అబద్ధం;
      ఒకవేళ (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	తిరిగి అబద్ధం;

      ఒకవేళ (subset_charset && !plan_subset_charset (acc, plan))
        తిరిగి అబద్ధం;

      topdict_mod.reassignSIDs (sidmap);
    }

    ఒకవేళ (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<స్థిరరాశి OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_సంకేతం_endchar>
		    flattener(acc, plan);
      ఒకవేళ (!flattener.flatten (subset_charstrings))
	తిరిగి అబద్ధం;
    }
    లేకపోతే
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ఒకవేళ (!subr_subsetter.subset ())
	తిరిగి అబద్ధం;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ఒకవేళ (!subr_subsetter.encode_charstrings (subset_charstrings))
	తిరిగి అబద్ధం;

      ఒకవేళ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	తిరిగి అబద్ధం;

      /* local subrs */
      ఒకవేళ (!subset_localsubrs.resize (orig_fdcount))
	తిరిగి అబద్ధం;
      కొరకు (గుర్తులేని పూర్ణాంకం fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ఒకవేళ (fdmap.has (fd))
	{
	  ఒకవేళ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    తిరిగి అబద్ధం;
	}
      }
    }

    /* Encoding */
    ఒకవేళ (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    ఒకవేళ (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_విలువలు_mod_t ());
    లేకపోతే
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (స్థిరరాశి cff1_font_dict_విలువలు_t &_)
	{ తిరిగి fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (స్థిరరాశి cff1_font_dict_విలువలు_t &_)
	{
	  cff1_font_dict_విలువలు_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  తిరిగి mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    తిరిగి !plan->లోపల_error () &&
	   (subset_charstrings.నిడివి == plan->అంకె_output_glyphs ()) &&
	   (fontdicts_mod.నిడివి == subset_fdcount);
  }

  cff1_శిఖరం_dict_విలువలు_mod_t	topdict_mod;
  cff1_sub_పట్టిక_info_t		info;

  గుర్తులేని పూర్ణాంకం    అంకె_glyphs;
  గుర్తులేని పూర్ణాంకం    orig_fdcount = 0;
  గుర్తులేని పూర్ణాంకం    subset_fdcount = 1;
  గుర్తులేని పూర్ణాంకం    subset_fdselect_format = 0;
  hb_సదిశ_t<సంకేతం_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_సదిశ_t<str_buff_vec_t>	subset_localsubrs;
  hb_సదిశ_t<cff1_font_dict_విలువలు_mod_t>  fontdicts_mod;

  తార్కిక		drop_hints = అబద్ధం;

  తార్కిక		gid_renum;
  తార్కిక		subset_encoding;
  uint8_t	subset_enc_format;
  గుర్తులేని పూర్ణాంకం	subset_enc_అంకె_codes;
  range_list_t	subset_enc_సంకేతం_ranges;
  hb_సదిశ_t<సంకేతం_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  తార్కిక		subset_charset;

  remap_sid_t	sidmap;
  గుర్తులేని పూర్ణాంకం	శిఖరం_Dict_Mod_SIDs[name_dict_విలువలు_t::Val_లెక్క];

  తార్కిక		desubroutinize = అబద్ధం;
  తార్కిక		తటస్థం_charset = అబద్ధం;

  గుర్తులేని	min_charstrings_off_కొలత = 0;
};
} // namespace OT

స్థిర తార్కిక _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         నిర్మాణం OT::cff1_subset_plan &plan,
                                         స్థిరరాశి OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_అక్షరశ్రేణులు> ();

  గుర్తులేని దత్తాంశం_కొలత = 0;
  గుర్తులేని మొత్తం_కొలత = CFF1_Char_అక్షరశ్రేణులు::మొత్తం_కొలత (plan.subset_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత);
  ఒకవేళ (unlikely (!c->ప్రారంభం_zerocopy (మొత్తం_కొలత)))
    తిరిగి అబద్ధం;

  స్వయంచాలక *cs = c->ప్రారంభం_embed<CFF1_Char_అక్షరశ్రేణులు> ();
  ఒకవేళ (unlikely (!cs->serialize (c, plan.subset_charstrings, &దత్తాంశం_కొలత, plan.min_charstrings_off_కొలత))) {
    c->తీయి_discard ();
    తిరిగి అబద్ధం;
  }

  plan.info.char_అక్షరశ్రేణులు_link = c->తీయి_pack (అబద్ధం);
  తిరిగి నిజం;
}

తార్కిక
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   నిర్మాణం OT::cff1_subset_plan &plan) స్థిరరాశి
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ఒకవేళ (!_serialize_cff1_charstrings(c, plan, *ఇది))
    తిరిగి అబద్ధం;

  /* private dicts & local subrs */
  కొరకు (పూర్ణాంకం i = (పూర్ణాంకం) వ్యక్తిగత_Dicts.నిడివి; --i >= 0 ;)
  {
    ఒకవేళ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      ఒకవేళ (plan.subset_localsubrs[i].నిడివి > 0)
      {
	స్వయంచాలక *dest = c->push <CFF1Subrs> ();
	ఒకవేళ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->తీయి_pack ();
	లేకపోతే
	{
	  c->తీయి_discard ();
	  తిరిగి అబద్ధం;
	}
      }

      స్వయంచాలక *pd = c->push<వ్యక్తిగత_Dict> ();
      cff1_వ్యక్తిగత_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      ఒకవేళ (likely (pd->serialize (c, వ్యక్తిగత_Dicts[i], privSzr, subrs_link)))
      {
	గుర్తులేని fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].వ్యక్తిగత_Dict_Info.కొలత = c->నిడివి ();
	plan.fontdicts_mod[fd].వ్యక్తిగత_Dict_Info.link = c->తీయి_pack ();
      }
      లేకపోతే
      {
	c->తీయి_discard ();
	తిరిగి అబద్ధం;
      }
    }
  }

  ఒకవేళ (!is_CID ())
    plan.info.వ్యక్తిగత_Dict_Info = plan.fontdicts_mod[0].వ్యక్తిగత_Dict_Info;

  /* FDArray (FD Index) */
  ఒకవేళ (fdArray != &Null (CFF1FDArray))
  {
    స్వయంచాలక *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    స్వయంచాలక it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    ఒకవేళ (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->తీయి_pack (అబద్ధం);
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* FDSelect */
  ఒకవేళ (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    ఒకవేళ (likely (hb_serialize_cff_fdselect (c, plan.అంకె_glyphs, *fdSelect, fd_లెక్క,
					   plan.subset_fdselect_format, plan.info.fd_select.కొలత,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* Charset */
  ఒకవేళ (plan.subset_charset)
  {
    స్వయంచాలక *dest = c->push<Charset> ();
    ఒకవేళ (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.అంకె_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* Encoding */
  ఒకవేళ (plan.subset_encoding)
  {
    స్వయంచాలక *dest = c->push<Encoding> ();
    ఒకవేళ (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_అంకె_codes,
				 plan.subset_enc_సంకేతం_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* global subrs */
  {
    స్వయంచాలక *dest = c->push <CFF1Subrs> ();
    ఒకవేళ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->తీయి_pack (అబద్ధం);
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  /* String INDEX */
  {
    స్వయంచాలక *dest = c->push<CFF1_String_సూచీ> ();
    ఒకవేళ (likely (!plan.sidmap.లోపల_error () &&
		dest->serialize (c, *string_సూచీ, plan.sidmap.vector)))
      c->తీయి_pack ();
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ఒకవేళ (unlikely (!cff))
    తిరిగి అబద్ధం;

  /*శీర్షిక*/
  cff->సంస్కరణ.major = 0x01;
  cff->సంస్కరణ.minor = 0x00;
  cff->name_సూచీ = cff->min_కొలత;
  cff->off_కొలత = 4; /* unused? */

  /* name INDEX */
  ఒకవేళ (unlikely (!c->embed (*name_సూచీ))) తిరిగి అబద్ధం;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    స్వయంచాలక *శిఖరం_2 = c->push<శిఖరం_Dict> ();
    cff1_శిఖరం_dict_op_serializer_t శిఖరం_Szr;
    గుర్తులేని శిఖరం_కొలత = 0;
    శిఖరం_dict_modifiers_t  modifier (plan.info, plan.శిఖరం_Dict_Mod_SIDs);
    ఒకవేళ (likely (శిఖరం_2->serialize (c, plan.topdict_mod, శిఖరం_Szr, modifier)))
    {
      శిఖరం_కొలత = c->నిడివి ();
      c->తీయి_pack (అబద్ధం);
    }
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
    /* serialize INDEX header for above */
    స్వయంచాలక *dest = c->ప్రారంభం_embed<CFF1_సూచీ> ();
    తిరిగి dest->serialize_శీర్షిక (c, hb_iter (&శిఖరం_కొలత, 1), శిఖరం_కొలత);
  }
}

తార్కిక
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) స్థిరరాశి
{
  cff1_subset_plan cff_plan;

  ఒకవేళ (unlikely (!cff_plan.create (*ఇది, c->plan)))
  {
    DEBUG_సందేశం(SUBSET, శూన్యసూచిక, "Failed to generate a cff subsetting plan.");
    తిరిగి అబద్ధం;
  }

  తిరిగి serialize (c->serializer, cff_plan);
}


#అయితే_ముగింపు
