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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-cff1-table.hh"
#அந்தர்பாவ "hb-set.h"
#அந்தர்பாவ "hb-bimap.hh"
#அந்தர்பாவ "hb-subset-plan.hh"
#அந்தர்பாவ "hb-subset-cff-common.hh"
#அந்தர்பாவ "hb-cff1-interp-cs.hh"

using நாமாகாஶ CFF;

கட்டமைப்பு remap_sid_t
{
  unsigned get_population () மாறிலி { திரும்பு vector.length; }

  வெறுமை alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  தருக்க in_error () மாறிலி
  { திரும்பு map.in_error () || vector.in_error (); }

  unsigned முழு add (unsigned முழு sid)
  {
    எனில் (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      திரும்பு sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    எனில் (map.set (sid, v, பொய்))
    {
      vector.push (sid);
      next++;
    }
    இல்லையெனில்
      v = map.get (sid); // already exists
    திரும்பு offset_sid (v);
  }

  unsigned முழு operator[] (unsigned முழு sid) மாறிலி
  {
    எனில் (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      திரும்பு sid;

    திரும்பு offset_sid (map.get (unoffset_sid (sid)));
  }

  நிலையான மாறிலி unsigned முழு இலக்கம்_std_strings = 391;

  நிலையான தருக்க is_std_str (unsigned முழு sid) { திரும்பு sid < இலக்கம்_std_strings; }
  நிலையான unsigned முழு offset_sid (unsigned முழு sid) { திரும்பு sid + இலக்கம்_std_strings; }
  நிலையான unsigned முழு unoffset_sid (unsigned முழு sid) { திரும்பு sid - இலக்கம்_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

கட்டமைப்பு cff1_sub_table_info_t : cff_sub_table_info_t
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
கட்டமைப்பு cff1_உச்சி_dict_மதிப்புகள்_mod_t : cff1_உச்சி_dict_மதிப்புகள்_t
{
  வெறுமை init (மாறிலி cff1_உச்சி_dict_மதிப்புகள்_t *base_= &Null (cff1_உச்சி_dict_மதிப்புகள்_t))
  {
    SUPER::init ();
    base = base_;
  }

  வெறுமை fini () { SUPER::fini (); }

  unsigned get_மொத்தம் () மாறிலி { திரும்பு base->get_மொத்தம் () + SUPER::get_மொத்தம் (); }
  மாறிலி cff1_உச்சி_dict_val_t &get_மதிப்பு (unsigned முழு i) மாறிலி
  {
    எனில் (i < base->get_மொத்தம் ())
      திரும்பு (*base)[i];
    இல்லையெனில்
      திரும்பு SUPER::values[i - base->get_மொத்தம் ()];
  }
  மாறிலி cff1_உச்சி_dict_val_t &operator [] (unsigned முழு i) மாறிலி { திரும்பு get_மதிப்பு (i); }

  வெறுமை reassignSIDs (மாறிலி remap_sid_t& sidmap)
  {
    க்கு (unsigned முழு i = 0; i < name_dict_மதிப்புகள்_t::Val_மொத்தம்; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  பாதுகாக்கப்பட்ட:
  வகைவரை cff1_உச்சி_dict_மதிப்புகள்_t SUPER;
  மாறிலி cff1_உச்சி_dict_மதிப்புகள்_t *base;
};

கட்டமைப்பு உச்சி_dict_modifiers_t
{
  உச்சி_dict_modifiers_t (மாறிலி cff1_sub_table_info_t &info_,
			மாறிலி unsigned முழு (&nameSIDs_)[name_dict_மதிப்புகள்_t::Val_மொத்தம்])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  மாறிலி cff1_sub_table_info_t &info;
  மாறிலி unsigned முழு	(&nameSIDs)[name_dict_மதிப்புகள்_t::Val_மொத்தம்];
};

கட்டமைப்பு cff1_உச்சி_dict_op_serializer_t : cff_உச்சி_dict_op_serializer_t<cff1_உச்சி_dict_val_t>
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி cff1_உச்சி_dict_val_t &opstr,
		  மாறிலி உச்சி_dict_modifiers_t &mod) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    op_code_t op = opstr.op;
    தேர்வு (op)
    {
      நிலை OpCode_charset:
	எனில் (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	இல்லையெனில்
	  செல் fall_back;

      நிலை OpCode_Encoding:
	எனில் (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	இல்லையெனில்
	  செல் fall_back;

      நிலை OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      நிலை OpCode_version:
      நிலை OpCode_Notice:
      நிலை OpCode_Copyright:
      நிலை OpCode_FullName:
      நிலை OpCode_FamilyName:
      நிலை OpCode_Weight:
      நிலை OpCode_PostScript:
      நிலை OpCode_BaseFontName:
      நிலை OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_மதிப்புகள்_t::name_op_to_index (op)]));

      நிலை OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  எனில் ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (பொய்);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_மதிப்புகள்_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_மதிப்புகள்_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      இயல்பு:
	return_trace (cff_உச்சி_dict_op_serializer_t<cff1_உச்சி_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (உண்மை);
  }

};

கட்டமைப்பு cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி op_str_t &opstr,
		  மாறிலி cff1_font_dict_மதிப்புகள்_mod_t &mod) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    எனில் (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    இல்லையெனில்
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  தனிப்பட்ட:
  வகைவரை cff_font_dict_op_serializer_t SUPER;
};

கட்டமைப்பு cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  நிலையான வெறுமை flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    எனில் (env.arg_start > 0)
      flush_width (env, param);

    தேர்வு (op)
    {
      நிலை OpCode_hstem:
      நிலை OpCode_hstemhm:
      நிலை OpCode_vstem:
      நிலை OpCode_vstemhm:
      நிலை OpCode_hintmask:
      நிலை OpCode_cntrmask:
      நிலை OpCode_dotsection:
	எனில் (param.drop_hints)
	{
	  env.clear_args ();
	  திரும்பு;
	}
	HB_FALLTHROUGH;

      இயல்பு:
	SUPER::flush_args_and_op (op, env, param);
	நிறுத்து;
    }
  }
  நிலையான வெறுமை flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    க்கு (unsigned முழு i = env.arg_start; i < env.arg_அடுக்கு.get_மொத்தம் (); i++)
      encoder.encode_இலக்கம்_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  நிலையான வெறுமை flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  நிலையான வெறுமை flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ப்ரதிஜ்ஞா (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_இலக்கம்_cs (env.width);
  }

  நிலையான வெறுமை flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    எனில் (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      க்கு (unsigned முழு i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  தனிப்பட்ட:
  வகைவரை cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

கட்டமைப்பு range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  தருக்க complete (unsigned முழு last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned மொத்தம் = இது->length;
    க்கு (unsigned முழு i = மொத்தம்; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned முழு n_இடது = last_glyph - pair.glyph - 1;
      all_glyphs |= n_இடது;
      last_glyph = pair.glyph;
      pair.glyph = n_இடது;
    }
    தருக்க two_byte = all_glyphs >= 0x100;
    திரும்பு two_byte;
  }
};

கட்டமைப்பு cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  நிலையான வெறுமை process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    தேர்வு (op) {

      நிலை OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, பொய்);
	நிறுத்து;

      நிலை OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	நிறுத்து;

      நிலை OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	நிறுத்து;

      நிலை OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	நிறுத்து;
    }
  }

  பாதுகாக்கப்பட்ட:
  நிலையான வெறுமை process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_இலக்கம்_2);
    closure->add (env.context.subr_இலக்கம்_2);
    param.set_current_str (env, உண்மை);
  }

  தனிப்பட்ட:
  வகைவரை cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

கட்டமைப்பு cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (தருக்க desubroutinize_, தருக்க drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி op_str_t &opstr,
		  objidx_t subrs_link) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    எனில் (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (உண்மை);

    எனில் (opstr.op == OpCode_Subrs)
    {
      எனில் (desubroutinize || !subrs_link)
	return_trace (உண்மை);
      இல்லையெனில்
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  பாதுகாக்கப்பட்ட:
  மாறிலி தருக்க desubroutinize;
  மாறிலி தருக்க drop_hints;
};

கட்டமைப்பு cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, மாறிலி OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (மாறிலி OT::cff1::accelerator_subset_t &acc_, மாறிலி hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  நிலையான வெறுமை complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    எனில் (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    க்கு (unsigned முழு i = 0; i < env.call_அடுக்கு.get_மொத்தம் (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_அடுக்கு[i]);
      எனில் (likely (parsed_str))
	parsed_str->set_parsed ();
      இல்லையெனில்
	env.set_error ();
    }
  }
};

நாமாகாஶ OT {
கட்டமைப்பு cff1_subset_plan
{
  cff1_subset_plan ()
  {
    க்கு (unsigned முழு i = 0; i < name_dict_மதிப்புகள்_t::Val_மொத்தம்; i++)
      உச்சி_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  வெறுமை plan_subset_encoding (மாறிலி OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    மாறிலி Encoding *encoding = acc.encoding;
    unsigned முழு  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    எனில் (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (பொய்);
      திரும்பு;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_இலக்கம்_codes = plan->இலக்கம்_output_glyphs () - 1;
    unsigned முழு glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    எனில் (it->first == 0) it++;
    auto _ = *it;
    க்கு (glyph = 1; glyph < இலக்கம்_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      எனில் (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      இல்லையெனில்
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      எனில் (code == CFF_UNDEF_CODE)
      {
	subset_enc_இலக்கம்_codes = glyph - 1;
	நிறுத்து;
      }

      எனில் (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      எனில் (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	க்கு (unsigned முழு i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    ப்ரதிஜ்ஞா (subset_enc_இலக்கம்_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_இலக்கம்_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    எனில் (size0 < size1)
      subset_enc_format = 0;
    இல்லையெனில்
      subset_enc_format = 1;
  }

  தருக்க plan_subset_charset (மாறிலி OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned முழு  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    எனில் (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (பொய்);
      திரும்பு பொய்;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned இலக்கம்_glyphs = plan->இலக்கம்_output_glyphs ();

    எனில் (unlikely (!subset_charset_ranges.alloc (hb_min (இலக்கம்_glyphs,
							acc.இலக்கம்_charset_entries))))
    {
      plan->check_success (பொய்);
      திரும்பு பொய்;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    தருக்க created_map = பொய்;
    எனில் (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = உண்மை;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    எனில் (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = பொய்;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    எனில் (it->first == 0) it++;
    auto _ = *it;
    தருக்க not_is_cid = !acc.is_CID ();
    தருக்க skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    எனில் (not_is_cid)
      sidmap.alloc (இலக்கம்_glyphs);
    க்கு (hb_codepoint_t glyph = 1; glyph < இலக்கம்_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      எனில் (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      இல்லையெனில்
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      எனில் (identity_charset)
	sid = glyph;
      இல்லையெனில்
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	எனில் (not_is_cid)
	  sid = sidmap.add (sid);
      }

      எனில் (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      எனில் (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    எனில் (created_map)
    {
      எனில் ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    தருக்க two_byte = subset_charset_ranges.complete (இலக்கம்_glyphs);

    size0 = Charset0::get_size (plan->இலக்கம்_output_glyphs ());
    எனில் (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    இல்லையெனில்
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    எனில் (size0 < size_ranges)
      subset_charset_format = 0;
    இல்லையெனில் எனில் (!two_byte)
      subset_charset_format = 1;
    இல்லையெனில்
      subset_charset_format = 2;

    திரும்பு உண்மை;
  }

  தருக்க collect_sids_in_dicts (மாறிலி OT::cff1::accelerator_subset_t &acc)
  {
    க்கு (unsigned முழு i = 0; i < name_dict_மதிப்புகள்_t::Val_மொத்தம்; i++)
    {
      unsigned முழு sid = acc.உச்சி_Dict_2.nameSIDs[i];
      எனில் (sid != CFF_UNDEF_SID)
      {
	உச்சி_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    எனில் (acc.fdArray != &Null (CFF1FDArray))
      க்கு (unsigned முழு i = 0; i < orig_fdcount; i++)
	எனில் (fdmap.has (i))
	  (வெறுமை)sidmap.add (acc.fontDicts[i].fontName);

    திரும்பு உண்மை;
  }

  தருக்க create (மாறிலி OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    எனில் (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) திரும்பு பொய்;

    இலக்கம்_glyphs = plan->இலக்கம்_output_glyphs ();
    orig_fdcount = acc.fd_மொத்தம்;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #இல்லையெனில்
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    எனில் (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      க்கு (மாறிலி auto &_ : plan->new_to_old_gid_list)
      {
	எனில் (_.first != _.second)
	{
	  subset_charset = உண்மை;
	  நிறுத்து;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.உச்சி_Dict_2);
      தருக்க need_to_add_enc = (subset_encoding && !acc.உச்சி_Dict_2.has_op (OpCode_Encoding));
      தருக்க need_to_add_set = (subset_charset && !acc.உச்சி_Dict_2.has_op (OpCode_charset));
      எனில் (need_to_add_enc || need_to_add_set)
      {
	எனில் (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	எனில் (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    எனில் (acc.fdSelect != &Null (CFF1FDSelect))
    {
	எனில் (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.இலக்கம்_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	திரும்பு பொய்;
    }
    இல்லையெனில்
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      எனில் (unlikely (!collect_sids_in_dicts (acc)))
	திரும்பு பொய்;
      எனில் (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	திரும்பு பொய்;

      எனில் (subset_charset && !plan_subset_charset (acc, plan))
        திரும்பு பொய்;

      topdict_mod.reassignSIDs (sidmap);
    }

    எனில் (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<மாறிலி OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      எனில் (!flattener.flatten (subset_charstrings))
	திரும்பு பொய்;
    }
    இல்லையெனில்
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      எனில் (!subr_subsetter.subset ())
	திரும்பு பொய்;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      எனில் (!subr_subsetter.encode_charstrings (subset_charstrings))
	திரும்பு பொய்;

      எனில் (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	திரும்பு பொய்;

      /* local subrs */
      எனில் (!subset_localsubrs.resize (orig_fdcount))
	திரும்பு பொய்;
      க்கு (unsigned முழு fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	எனில் (fdmap.has (fd))
	{
	  எனில் (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    திரும்பு பொய்;
	}
      }
    }

    /* Encoding */
    எனில் (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    எனில் (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_மதிப்புகள்_mod_t ());
    இல்லையெனில்
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (மாறிலி cff1_font_dict_மதிப்புகள்_t &_)
	{ திரும்பு fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (மாறிலி cff1_font_dict_மதிப்புகள்_t &_)
	{
	  cff1_font_dict_மதிப்புகள்_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  திரும்பு mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    திரும்பு !plan->in_error () &&
	   (subset_charstrings.length == plan->இலக்கம்_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_உச்சி_dict_மதிப்புகள்_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned முழு    இலக்கம்_glyphs;
  unsigned முழு    orig_fdcount = 0;
  unsigned முழு    subset_fdcount = 1;
  unsigned முழு    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_மதிப்புகள்_mod_t>  fontdicts_mod;

  தருக்க		drop_hints = பொய்;

  தருக்க		gid_renum;
  தருக்க		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned முழு	subset_enc_இலக்கம்_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  தருக்க		subset_charset;

  remap_sid_t	sidmap;
  unsigned முழு	உச்சி_Dict_Mod_SIDs[name_dict_மதிப்புகள்_t::Val_மொத்தம்];

  தருக்க		desubroutinize = பொய்;
  தருக்க		identity_charset = பொய்;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

நிலையான தருக்க _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         கட்டமைப்பு OT::cff1_subset_plan &plan,
                                         மாறிலி OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  எனில் (unlikely (!c->start_zerocopy (total_size)))
    திரும்பு பொய்;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  எனில் (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->எடு_discard ();
    திரும்பு பொய்;
  }

  plan.info.char_strings_link = c->எடு_pack (பொய்);
  திரும்பு உண்மை;
}

தருக்க
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   கட்டமைப்பு OT::cff1_subset_plan &plan) மாறிலி
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  எனில் (!_serialize_cff1_charstrings(c, plan, *இது))
    திரும்பு பொய்;

  /* private dicts & local subrs */
  க்கு (முழு i = (முழு) privateDicts.length; --i >= 0 ;)
  {
    எனில் (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      எனில் (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	எனில் (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->எடு_pack ();
	இல்லையெனில்
	{
	  c->எடு_discard ();
	  திரும்பு பொய்;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      எனில் (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->எடு_pack ();
      }
      இல்லையெனில்
      {
	c->எடு_discard ();
	திரும்பு பொய்;
      }
    }
  }

  எனில் (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  எனில் (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    எனில் (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->எடு_pack (பொய்);
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* FDSelect */
  எனில் (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    எனில் (likely (hb_serialize_cff_fdselect (c, plan.இலக்கம்_glyphs, *fdSelect, fd_மொத்தம்,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* Charset */
  எனில் (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    எனில் (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.இலக்கம்_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* Encoding */
  எனில் (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    எனில் (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_இலக்கம்_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    எனில் (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->எடு_pack (பொய்);
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    எனில் (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->எடு_pack ();
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  எனில் (unlikely (!cff))
    திரும்பு பொய்;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  எனில் (unlikely (!c->embed (*nameIndex))) திரும்பு பொய்;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *உச்சி_2 = c->push<உச்சி_Dict> ();
    cff1_உச்சி_dict_op_serializer_t உச்சி_Szr;
    unsigned உச்சி_size = 0;
    உச்சி_dict_modifiers_t  modifier (plan.info, plan.உச்சி_Dict_Mod_SIDs);
    எனில் (likely (உச்சி_2->serialize (c, plan.topdict_mod, உச்சி_Szr, modifier)))
    {
      உச்சி_size = c->length ();
      c->எடு_pack (பொய்);
    }
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    திரும்பு dest->serialize_header (c, hb_iter (&உச்சி_size, 1), உச்சி_size);
  }
}

தருக்க
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) மாறிலி
{
  cff1_subset_plan cff_plan;

  எனில் (unlikely (!cff_plan.create (*இது, c->plan)))
  {
    DEBUG_செய்தி(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    திரும்பு பொய்;
  }

  திரும்பு serialize (c->serializer, cff_plan);
}


#endif
