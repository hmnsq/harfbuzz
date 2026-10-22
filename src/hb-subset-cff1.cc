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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-cff1-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.h"
#ଅନ୍ତର୍ଭାଵ "hb-bimap.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff1-interp-cs.hh"

using ନାମାକାଶ CFF;

ସଂରଚନା remap_sid_t
{
  unsigned get_population () ଅଚଳ { ଫେରନ୍ତୁ vector.length; }

  ଶୂନ୍ୟ alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  ତାର୍କିକ in_error () ଅଚଳ
  { ଫେରନ୍ତୁ map.in_error () || vector.in_error (); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା add (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid)
  {
    ଯଦି (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ଫେରନ୍ତୁ sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    ଯଦି (map.set (sid, v, ମିଥ୍ୟା))
    {
      vector.push (sid);
      next++;
    }
    ଅନ୍ୟଥା
      v = map.get (sid); // already exists
    ଫେରନ୍ତୁ offset_sid (v);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା operator[] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid) ଅଚଳ
  {
    ଯଦି (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ଫେରନ୍ତୁ sid;

    ଫେରନ୍ତୁ offset_sid (map.get (unoffset_sid (sid)));
  }

  ସ୍ଥିର ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_std_strings = 391;

  ସ୍ଥିର ତାର୍କିକ is_std_str (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid) { ଫେରନ୍ତୁ sid < ଅଙ୍କ_std_strings; }
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset_sid (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid) { ଫେରନ୍ତୁ sid + ଅଙ୍କ_std_strings; }
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା unoffset_sid (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid) { ଫେରନ୍ତୁ sid - ଅଙ୍କ_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

ସଂରଚନା cff1_sub_table_info_t : cff_sub_table_info_t
{
  cff1_sub_table_info_t ()
    : cff_sub_table_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    privateDictInfo.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  table_info_t	privateDictInfo;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
ସଂରଚନା cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t : cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t
{
  ଶୂନ୍ୟ init (ଅଚଳ cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t *base_= &Null (cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t))
  {
    SUPER::init ();
    base = base_;
  }

  ଶୂନ୍ୟ fini () { SUPER::fini (); }

  unsigned get_ଗଣନା () ଅଚଳ { ଫେରନ୍ତୁ base->get_ଗଣନା () + SUPER::get_ଗଣନା (); }
  ଅଚଳ cff1_ଶୀର୍ଷ_dict_val_t &get_ମୂଲ୍ୟ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ଯଦି (i < base->get_ଗଣନା ())
      ଫେରନ୍ତୁ (*base)[i];
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ SUPER::values[i - base->get_ଗଣନା ()];
  }
  ଅଚଳ cff1_ଶୀର୍ଷ_dict_val_t &operator [] (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ get_ମୂଲ୍ୟ (i); }

  ଶୂନ୍ୟ reassignSIDs (ଅଚଳ remap_sid_t& sidmap)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  ସୁରକ୍ଷିତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t SUPER;
  ଅଚଳ cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t *base;
};

ସଂରଚନା ଶୀର୍ଷ_dict_modifiers_t
{
  ଶୀର୍ଷ_dict_modifiers_t (ଅଚଳ cff1_sub_table_info_t &info_,
			ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା (&nameSIDs_)[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ଅଚଳ cff1_sub_table_info_t &info;
  ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	(&nameSIDs)[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା];
};

ସଂରଚନା cff1_ଶୀର୍ଷ_dict_op_serializer_t : cff_ଶୀର୍ଷ_dict_op_serializer_t<cff1_ଶୀର୍ଷ_dict_val_t>
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ cff1_ଶୀର୍ଷ_dict_val_t &opstr,
		  ଅଚଳ ଶୀର୍ଷ_dict_modifiers_t &mod) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    op_code_t op = opstr.op;
    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_charset:
	ଯଦି (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	ଅନ୍ୟଥା
	  ଯାଅ fall_back;

      ସ୍ଥିତି OpCode_Encoding:
	ଯଦି (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	ଅନ୍ୟଥା
	  ଯାଅ fall_back;

      ସ୍ଥିତି OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      ସ୍ଥିତି OpCode_version:
      ସ୍ଥିତି OpCode_Notice:
      ସ୍ଥିତି OpCode_Copyright:
      ସ୍ଥିତି OpCode_FullName:
      ସ୍ଥିତି OpCode_FamilyName:
      ସ୍ଥିତି OpCode_Weight:
      ସ୍ଥିତି OpCode_PostScript:
      ସ୍ଥିତି OpCode_BaseFontName:
      ସ୍ଥିତି OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::name_op_to_index (op)]));

      ସ୍ଥିତି OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  ଯଦି ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (ମିଥ୍ୟା);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	return_trace (cff_ଶୀର୍ଷ_dict_op_serializer_t<cff1_ଶୀର୍ଷ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (ସତ୍ୟ);
  }

};

ସଂରଚନା cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ op_str_t &opstr,
		  ଅଚଳ cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t &mod) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    ଅନ୍ୟଥା
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff_font_dict_op_serializer_t SUPER;
};

ସଂରଚନା cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ଯଦି (env.arg_start > 0)
      flush_width (env, param);

    ବାଛ (op)
    {
      ସ୍ଥିତି OpCode_hstem:
      ସ୍ଥିତି OpCode_hstemhm:
      ସ୍ଥିତି OpCode_vstem:
      ସ୍ଥିତି OpCode_vstemhm:
      ସ୍ଥିତି OpCode_hintmask:
      ସ୍ଥିତି OpCode_cntrmask:
      ସ୍ଥିତି OpCode_dotsection:
	ଯଦି (param.drop_hints)
	{
	  env.clear_args ();
	  ଫେରନ୍ତୁ;
	}
	HB_FALLTHROUGH;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::flush_args_and_op (op, env, param);
	ଭାଙ୍ଗ;
    }
  }
  ସ୍ଥିର ଶୂନ୍ୟ flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = env.arg_start; i < env.arg_ଗଦା.get_ଗଣନା (); i++)
      encoder.encode_ଅଙ୍କ_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ପ୍ରତିଜ୍ଞା (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_ଅଙ୍କ_cs (env.width);
  }

  ସ୍ଥିର ଶୂନ୍ୟ flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    ଯଦି (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ସଂରଚନା range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  ତାର୍କିକ complete (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned ଗଣନା_2 = ଏହି->length;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = ଗଣନା_2; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_ବାମ = last_glyph - pair.glyph - 1;
      all_glyphs |= n_ବାମ;
      last_glyph = pair.glyph;
      pair.glyph = n_ବାମ;
    }
    ତାର୍କିକ two_byte = all_glyphs >= 0x100;
    ଫେରନ୍ତୁ two_byte;
  }
};

ସଂରଚନା cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    ବାଛ (op) {

      ସ୍ଥିତି OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ମିଥ୍ୟା);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	ଭାଙ୍ଗ;
    }
  }

  ସୁରକ୍ଷିତ:
  ସ୍ଥିର ଶୂନ୍ୟ process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_ଅଙ୍କ_2);
    closure->add (env.context.subr_ଅଙ୍କ_2);
    param.set_current_str (env, ସତ୍ୟ);
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ସଂରଚନା cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (ତାର୍କିକ desubroutinize_, ତାର୍କିକ drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ op_str_t &opstr,
		  objidx_t subrs_link) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ସତ୍ୟ);

    ଯଦି (opstr.op == OpCode_Subrs)
    {
      ଯଦି (desubroutinize || !subrs_link)
	return_trace (ସତ୍ୟ);
      ଅନ୍ୟଥା
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  ସୁରକ୍ଷିତ:
  ଅଚଳ ତାର୍କିକ desubroutinize;
  ଅଚଳ ତାର୍କିକ drop_hints;
};

ସଂରଚନା cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ଅଚଳ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ଅଚଳ OT::cff1::accelerator_subset_t &acc_, ଅଚଳ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ସ୍ଥିର ଶୂନ୍ୟ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    ଯଦି (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < env.call_ଗଦା.get_ଗଣନା (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ଗଦା[i]);
      ଯଦି (likely (parsed_str))
	parsed_str->set_parsed ();
      ଅନ୍ୟଥା
	env.set_error ();
    }
  }
};

ନାମାକାଶ OT {
ସଂରଚନା cff1_subset_plan
{
  cff1_subset_plan ()
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା; i++)
      ଶୀର୍ଷ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  ଶୂନ୍ୟ plan_subset_encoding (ଅଚଳ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ଅଚଳ Encoding *encoding = acc.encoding;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    ଯଦି (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (ମିଥ୍ୟା);
      ଫେରନ୍ତୁ;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_ଅଙ୍କ_codes = plan->ଅଙ୍କ_output_glyphs () - 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    ଯଦି (it->first == 0) it++;
    auto _ = *it;
    ପାଇଁ (glyph = 1; glyph < ଅଙ୍କ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ଯଦି (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ଅନ୍ୟଥା
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      ଯଦି (code == CFF_UNDEF_CODE)
      {
	subset_enc_ଅଙ୍କ_codes = glyph - 1;
	ଭାଙ୍ଗ;
      }

      ଯଦି (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      ଯଦି (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    ପ୍ରତିଜ୍ଞା (subset_enc_ଅଙ୍କ_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_ଅଙ୍କ_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    ଯଦି (size0 < size1)
      subset_enc_format = 0;
    ଅନ୍ୟଥା
      subset_enc_format = 1;
  }

  ତାର୍କିକ plan_subset_charset (ଅଚଳ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    ଯଦି (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (ମିଥ୍ୟା);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned ଅଙ୍କ_glyphs = plan->ଅଙ୍କ_output_glyphs ();

    ଯଦି (unlikely (!subset_charset_ranges.alloc (hb_min (ଅଙ୍କ_glyphs,
							acc.ଅଙ୍କ_charset_entries))))
    {
      plan->check_success (ମିଥ୍ୟା);
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    ତାର୍କିକ created_map = ମିଥ୍ୟା;
    ଯଦି (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = ସତ୍ୟ;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    ଯଦି (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = ମିଥ୍ୟା;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    ଯଦି (it->first == 0) it++;
    auto _ = *it;
    ତାର୍କିକ not_is_cid = !acc.is_CID ();
    ତାର୍କିକ skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    ଯଦି (not_is_cid)
      sidmap.alloc (ଅଙ୍କ_glyphs);
    ପାଇଁ (hb_codepoint_t glyph = 1; glyph < ଅଙ୍କ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ଯଦି (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ଅନ୍ୟଥା
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      ଯଦି (identity_charset)
	sid = glyph;
      ଅନ୍ୟଥା
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	ଯଦି (not_is_cid)
	  sid = sidmap.add (sid);
      }

      ଯଦି (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      ଯଦି (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    ଯଦି (created_map)
    {
      ଯଦି ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    ତାର୍କିକ two_byte = subset_charset_ranges.complete (ଅଙ୍କ_glyphs);

    size0 = Charset0::get_size (plan->ଅଙ୍କ_output_glyphs ());
    ଯଦି (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    ଅନ୍ୟଥା
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    ଯଦି (size0 < size_ranges)
      subset_charset_format = 0;
    ଅନ୍ୟଥା ଯଦି (!two_byte)
      subset_charset_format = 1;
    ଅନ୍ୟଥା
      subset_charset_format = 2;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ collect_sids_in_dicts (ଅଚଳ OT::cff1::accelerator_subset_t &acc)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sid = acc.ଶୀର୍ଷ_Dict_2.nameSIDs[i];
      ଯଦି (sid != CFF_UNDEF_SID)
      {
	ଶୀର୍ଷ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    ଯଦି (acc.fdArray != &Null (CFF1FDArray))
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < orig_fdcount; i++)
	ଯଦି (fdmap.has (i))
	  (ଶୂନ୍ୟ)sidmap.add (acc.fontDicts[i].fontName);

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ create (ଅଚଳ OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ଯଦି (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଙ୍କ_glyphs = plan->ଅଙ୍କ_output_glyphs ();
    orig_fdcount = acc.fd_ଗଣନା;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ଅନ୍ୟଥା
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    ଯଦି (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      ପାଇଁ (ଅଚଳ auto &_ : plan->new_to_old_gid_list)
      {
	ଯଦି (_.first != _.second)
	{
	  subset_charset = ସତ୍ୟ;
	  ଭାଙ୍ଗ;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.ଶୀର୍ଷ_Dict_2);
      ତାର୍କିକ need_to_add_enc = (subset_encoding && !acc.ଶୀର୍ଷ_Dict_2.has_op (OpCode_Encoding));
      ତାର୍କିକ need_to_add_set = (subset_charset && !acc.ଶୀର୍ଷ_Dict_2.has_op (OpCode_charset));
      ଯଦି (need_to_add_enc || need_to_add_set)
      {
	ଯଦି (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	ଯଦି (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    ଯଦି (acc.fdSelect != &Null (CFF1FDSelect))
    {
	ଯଦି (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.ଅଙ୍କ_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      ଯଦି (unlikely (!collect_sids_in_dicts (acc)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (subset_charset && !plan_subset_charset (acc, plan))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      topdict_mod.reassignSIDs (sidmap);
    }

    ଯଦି (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ଅଚଳ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      ଯଦି (!flattener.flatten (subset_charstrings))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ଯଦି (!subr_subsetter.subset ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ଯଦି (!subr_subsetter.encode_charstrings (subset_charstrings))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* local subrs */
      ଯଦି (!subset_localsubrs.resize (orig_fdcount))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ଯଦି (fdmap.has (fd))
	{
	  ଯଦି (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
      }
    }

    /* Encoding */
    ଯଦି (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    ଯଦି (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t ());
    ଅନ୍ୟଥା
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ଅଚଳ cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t &_)
	{ ଫେରନ୍ତୁ fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ଅଚଳ cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t &_)
	{
	  cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  ଫେରନ୍ତୁ mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    ଫେରନ୍ତୁ !plan->in_error () &&
	   (subset_charstrings.length == plan->ଅଙ୍କ_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    ଅଙ୍କ_glyphs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    orig_fdcount = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subset_fdcount = 1;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_mod_t>  fontdicts_mod;

  ତାର୍କିକ		drop_hints = ମିଥ୍ୟା;

  ତାର୍କିକ		gid_renum;
  ତାର୍କିକ		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	subset_enc_ଅଙ୍କ_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  ତାର୍କିକ		subset_charset;

  remap_sid_t	sidmap;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ଶୀର୍ଷ_Dict_Mod_SIDs[name_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t::Val_ଗଣନା];

  ତାର୍କିକ		desubroutinize = ମିଥ୍ୟା;
  ତାର୍କିକ		identity_charset = ମିଥ୍ୟା;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

ସ୍ଥିର ତାର୍କିକ _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ସଂରଚନା OT::cff1_subset_plan &plan,
                                         ଅଚଳ OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  ଯଦି (unlikely (!c->start_zerocopy (total_size)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  ଯଦି (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->କାଢ଼_discard ();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  plan.info.char_strings_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ସଂରଚନା OT::cff1_subset_plan &plan) ଅଚଳ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ଯଦି (!_serialize_cff1_charstrings(c, plan, *ଏହି))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  /* private dicts & local subrs */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) privateDicts.length; --i >= 0 ;)
  {
    ଯଦି (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      ଯଦି (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	ଯଦି (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->କାଢ଼_pack ();
	ଅନ୍ୟଥା
	{
	  c->କାଢ଼_discard ();
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      ଯଦି (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->କାଢ଼_pack ();
      }
      ଅନ୍ୟଥା
      {
	c->କାଢ଼_discard ();
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    }
  }

  ଯଦି (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  ଯଦି (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    ଯଦି (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->କାଢ଼_pack (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* FDSelect */
  ଯଦି (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    ଯଦି (likely (hb_serialize_cff_fdselect (c, plan.ଅଙ୍କ_glyphs, *fdSelect, fd_ଗଣନା,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* Charset */
  ଯଦି (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    ଯଦି (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.ଅଙ୍କ_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* Encoding */
  ଯଦି (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    ଯଦି (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_ଅଙ୍କ_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    ଯଦି (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->କାଢ଼_pack (ମିଥ୍ୟା);
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    ଯଦି (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->କାଢ଼_pack ();
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ଯଦି (unlikely (!cff))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  ଯଦି (unlikely (!c->embed (*nameIndex))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *ଶୀର୍ଷ_2 = c->push<ଶୀର୍ଷ_Dict> ();
    cff1_ଶୀର୍ଷ_dict_op_serializer_t ଶୀର୍ଷ_Szr;
    unsigned ଶୀର୍ଷ_size = 0;
    ଶୀର୍ଷ_dict_modifiers_t  modifier (plan.info, plan.ଶୀର୍ଷ_Dict_Mod_SIDs);
    ଯଦି (likely (ଶୀର୍ଷ_2->serialize (c, plan.topdict_mod, ଶୀର୍ଷ_Szr, modifier)))
    {
      ଶୀର୍ଷ_size = c->length ();
      c->କାଢ଼_pack (ମିଥ୍ୟା);
    }
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    ଫେରନ୍ତୁ dest->serialize_header (c, hb_iter (&ଶୀର୍ଷ_size, 1), ଶୀର୍ଷ_size);
  }
}

ତାର୍କିକ
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ଅଚଳ
{
  cff1_subset_plan cff_plan;

  ଯଦି (unlikely (!cff_plan.create (*ଏହି, c->plan)))
  {
    DEBUG_ସନ୍ଦେଶ(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଫେରନ୍ତୁ serialize (c->serializer, cff_plan);
}


#endif
