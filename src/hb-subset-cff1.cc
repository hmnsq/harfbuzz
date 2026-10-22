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

#شامل "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#شامل "hb-open-type.hh"
#شامل "hb-ot-cff1-table.hh"
#شامل "hb-set.h"
#شامل "hb-bimap.hh"
#شامل "hb-subset-plan.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-cff1-interp-cs.hh"

using فضاینام CFF;

ساختار remap_sid_t
{
  unsigned get_population () ثابت { بازگشت vector.length; }

  تهی alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  منطقی in_error () ثابت
  { بازگشت map.in_error () || vector.in_error (); }

  unsigned صحیح add (unsigned صحیح sid)
  {
    اگر (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      بازگشت sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    اگر (map.set (sid, v, نادرست))
    {
      vector.push (sid);
      next++;
    }
    وگرنه
      v = map.get (sid); // already exists
    بازگشت offset_sid (v);
  }

  unsigned صحیح operator[] (unsigned صحیح sid) ثابت
  {
    اگر (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      بازگشت sid;

    بازگشت offset_sid (map.get (unoffset_sid (sid)));
  }

  ایستا ثابت unsigned صحیح رقم_std_strings = 391;

  ایستا منطقی is_std_str (unsigned صحیح sid) { بازگشت sid < رقم_std_strings; }
  ایستا unsigned صحیح offset_sid (unsigned صحیح sid) { بازگشت sid + رقم_std_strings; }
  ایستا unsigned صحیح unoffset_sid (unsigned صحیح sid) { بازگشت sid - رقم_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

ساختار cff1_sub_table_info_t : cff_sub_table_info_t
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
ساختار cff1_بالاترین_dict_مقادیر_mod_t : cff1_بالاترین_dict_مقادیر_t
{
  تهی init (ثابت cff1_بالاترین_dict_مقادیر_t *base_= &Null (cff1_بالاترین_dict_مقادیر_t))
  {
    SUPER::init ();
    base = base_;
  }

  تهی fini () { SUPER::fini (); }

  unsigned get_شمار () ثابت { بازگشت base->get_شمار () + SUPER::get_شمار (); }
  ثابت cff1_بالاترین_dict_val_t &get_مقدار (unsigned صحیح i) ثابت
  {
    اگر (i < base->get_شمار ())
      بازگشت (*base)[i];
    وگرنه
      بازگشت SUPER::values[i - base->get_شمار ()];
  }
  ثابت cff1_بالاترین_dict_val_t &operator [] (unsigned صحیح i) ثابت { بازگشت get_مقدار (i); }

  تهی reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    برای (unsigned صحیح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  محافظتشده:
  تعریفنوع cff1_بالاترین_dict_مقادیر_t SUPER;
  ثابت cff1_بالاترین_dict_مقادیر_t *base;
};

ساختار بالاترین_dict_modifiers_t
{
  بالاترین_dict_modifiers_t (ثابت cff1_sub_table_info_t &info_,
			ثابت unsigned صحیح (&nameSIDs_)[name_dict_مقادیر_t::Val_شمار])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_table_info_t &info;
  ثابت unsigned صحیح	(&nameSIDs)[name_dict_مقادیر_t::Val_شمار];
};

ساختار cff1_بالاترین_dict_op_serializer_t : cff_بالاترین_dict_op_serializer_t<cff1_بالاترین_dict_val_t>
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت cff1_بالاترین_dict_val_t &opstr,
		  ثابت بالاترین_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (این);

    op_code_t op = opstr.op;
    انتخاب (op)
    {
      حالت OpCode_charset:
	اگر (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وگرنه
	  برو fall_back;

      حالت OpCode_Encoding:
	اگر (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وگرنه
	  برو fall_back;

      حالت OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      حالت OpCode_version:
      حالت OpCode_Notice:
      حالت OpCode_Copyright:
      حالت OpCode_FullName:
      حالت OpCode_FamilyName:
      حالت OpCode_Weight:
      حالت OpCode_PostScript:
      حالت OpCode_BaseFontName:
      حالت OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_مقادیر_t::name_op_to_index (op)]));

      حالت OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  اگر ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (نادرست);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_مقادیر_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_مقادیر_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      پیشفرض:
	return_trace (cff_بالاترین_dict_op_serializer_t<cff1_بالاترین_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (درست);
  }

};

ساختار cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_مقادیر_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (این);

    اگر (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وگرنه
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  خصوصی:
  تعریفنوع cff_font_dict_op_serializer_t SUPER;
};

ساختار cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ایستا تهی flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    اگر (env.arg_start > 0)
      flush_width (env, param);

    انتخاب (op)
    {
      حالت OpCode_hstem:
      حالت OpCode_hstemhm:
      حالت OpCode_vstem:
      حالت OpCode_vstemhm:
      حالت OpCode_hintmask:
      حالت OpCode_cntrmask:
      حالت OpCode_dotsection:
	اگر (param.drop_hints)
	{
	  env.clear_args ();
	  بازگشت;
	}
	HB_FALLTHROUGH;

      پیشفرض:
	SUPER::flush_args_and_op (op, env, param);
	بشکن;
    }
  }
  ایستا تهی flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    برای (unsigned صحیح i = env.arg_start; i < env.arg_پشته.get_شمار (); i++)
      encoder.encode_رقم_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  ایستا تهی flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ایستا تهی flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_رقم_cs (env.width);
  }

  ایستا تهی flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    اگر (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      برای (unsigned صحیح i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خصوصی:
  تعریفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ساختار range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقی complete (unsigned صحیح last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned شمار = این->length;
    برای (unsigned صحیح i = شمار; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned صحیح n_چپ = last_glyph - pair.glyph - 1;
      all_glyphs |= n_چپ;
      last_glyph = pair.glyph;
      pair.glyph = n_چپ;
    }
    منطقی two_byte = all_glyphs >= 0x100;
    بازگشت two_byte;
  }
};

ساختار cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ایستا تهی process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    انتخاب (op) {

      حالت OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, نادرست);
	بشکن;

      حالت OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	بشکن;

      حالت OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	بشکن;

      حالت OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	بشکن;
    }
  }

  محافظتشده:
  ایستا تهی process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_رقم_2);
    closure->add (env.context.subr_رقم_2);
    param.set_current_str (env, درست);
  }

  خصوصی:
  تعریفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ساختار cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (منطقی desubroutinize_, منطقی drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (این);

    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (درست);

    اگر (opstr.op == OpCode_Subrs)
    {
      اگر (desubroutinize || !subrs_link)
	return_trace (درست);
      وگرنه
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محافظتشده:
  ثابت منطقی desubroutinize;
  ثابت منطقی drop_hints;
};

ساختار cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ایستا تهی complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    اگر (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    برای (unsigned صحیح i = 0; i < env.call_پشته.get_شمار (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_پشته[i]);
      اگر (likely (parsed_str))
	parsed_str->set_parsed ();
      وگرنه
	env.set_error ();
    }
  }
};

فضاینام OT {
ساختار cff1_subset_plan
{
  cff1_subset_plan ()
  {
    برای (unsigned صحیح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
      بالاترین_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  تهی plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    unsigned صحیح  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    اگر (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (نادرست);
      بازگشت;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_رقم_codes = plan->رقم_output_glyphs () - 1;
    unsigned صحیح glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    اگر (it->first == 0) it++;
    auto _ = *it;
    برای (glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      اگر (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وگرنه
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      اگر (code == CFF_UNDEF_CODE)
      {
	subset_enc_رقم_codes = glyph - 1;
	بشکن;
      }

      اگر (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      اگر (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	برای (unsigned صحیح i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    أكد (subset_enc_رقم_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_رقم_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    اگر (size0 < size1)
      subset_enc_format = 0;
    وگرنه
      subset_enc_format = 1;
  }

  منطقی plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned صحیح  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    اگر (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (نادرست);
      بازگشت نادرست;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned رقم_glyphs = plan->رقم_output_glyphs ();

    اگر (unlikely (!subset_charset_ranges.alloc (hb_min (رقم_glyphs,
							acc.رقم_charset_entries))))
    {
      plan->check_success (نادرست);
      بازگشت نادرست;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    منطقی created_map = نادرست;
    اگر (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = درست;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    اگر (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = نادرست;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    اگر (it->first == 0) it++;
    auto _ = *it;
    منطقی not_is_cid = !acc.is_CID ();
    منطقی skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    اگر (not_is_cid)
      sidmap.alloc (رقم_glyphs);
    برای (hb_codepoint_t glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      اگر (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وگرنه
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      اگر (identity_charset)
	sid = glyph;
      وگرنه
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	اگر (not_is_cid)
	  sid = sidmap.add (sid);
      }

      اگر (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      اگر (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    اگر (created_map)
    {
      اگر ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    منطقی two_byte = subset_charset_ranges.complete (رقم_glyphs);

    size0 = Charset0::get_size (plan->رقم_output_glyphs ());
    اگر (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    وگرنه
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    اگر (size0 < size_ranges)
      subset_charset_format = 0;
    وگرنه اگر (!two_byte)
      subset_charset_format = 1;
    وگرنه
      subset_charset_format = 2;

    بازگشت درست;
  }

  منطقی collect_sids_in_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    برای (unsigned صحیح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
    {
      unsigned صحیح sid = acc.بالاترین_Dict_2.nameSIDs[i];
      اگر (sid != CFF_UNDEF_SID)
      {
	بالاترین_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    اگر (acc.fdArray != &Null (CFF1FDArray))
      برای (unsigned صحیح i = 0; i < orig_fdcount; i++)
	اگر (fdmap.has (i))
	  (تهی)sidmap.add (acc.fontDicts[i].fontName);

    بازگشت درست;
  }

  منطقی create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    اگر (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) بازگشت نادرست;

    رقم_glyphs = plan->رقم_output_glyphs ();
    orig_fdcount = acc.fd_شمار;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وگرنه
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    اگر (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      برای (ثابت auto &_ : plan->new_to_old_gid_list)
      {
	اگر (_.first != _.second)
	{
	  subset_charset = درست;
	  بشکن;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.بالاترین_Dict_2);
      منطقی need_to_add_enc = (subset_encoding && !acc.بالاترین_Dict_2.has_op (OpCode_Encoding));
      منطقی need_to_add_set = (subset_charset && !acc.بالاترین_Dict_2.has_op (OpCode_charset));
      اگر (need_to_add_enc || need_to_add_set)
      {
	اگر (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	اگر (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    اگر (acc.fdSelect != &Null (CFF1FDSelect))
    {
	اگر (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.رقم_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	بازگشت نادرست;
    }
    وگرنه
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      اگر (unlikely (!collect_sids_in_dicts (acc)))
	بازگشت نادرست;
      اگر (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	بازگشت نادرست;

      اگر (subset_charset && !plan_subset_charset (acc, plan))
        بازگشت نادرست;

      topdict_mod.reassignSIDs (sidmap);
    }

    اگر (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      اگر (!flattener.flatten (subset_charstrings))
	بازگشت نادرست;
    }
    وگرنه
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      اگر (!subr_subsetter.subset ())
	بازگشت نادرست;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      اگر (!subr_subsetter.encode_charstrings (subset_charstrings))
	بازگشت نادرست;

      اگر (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	بازگشت نادرست;

      /* local subrs */
      اگر (!subset_localsubrs.resize (orig_fdcount))
	بازگشت نادرست;
      برای (unsigned صحیح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	اگر (fdmap.has (fd))
	{
	  اگر (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    بازگشت نادرست;
	}
      }
    }

    /* Encoding */
    اگر (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    اگر (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_مقادیر_mod_t ());
    وگرنه
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_مقادیر_t &_)
	{ بازگشت fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_مقادیر_t &_)
	{
	  cff1_font_dict_مقادیر_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  بازگشت mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    بازگشت !plan->in_error () &&
	   (subset_charstrings.length == plan->رقم_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_بالاترین_dict_مقادیر_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned صحیح    رقم_glyphs;
  unsigned صحیح    orig_fdcount = 0;
  unsigned صحیح    subset_fdcount = 1;
  unsigned صحیح    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_مقادیر_mod_t>  fontdicts_mod;

  منطقی		drop_hints = نادرست;

  منطقی		gid_renum;
  منطقی		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned صحیح	subset_enc_رقم_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقی		subset_charset;

  remap_sid_t	sidmap;
  unsigned صحیح	بالاترین_Dict_Mod_SIDs[name_dict_مقادیر_t::Val_شمار];

  منطقی		desubroutinize = نادرست;
  منطقی		identity_charset = نادرست;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

ایستا منطقی _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ساختار OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  اگر (unlikely (!c->start_zerocopy (total_size)))
    بازگشت نادرست;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  اگر (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->برداشت_discard ();
    بازگشت نادرست;
  }

  plan.info.char_strings_link = c->برداشت_pack (نادرست);
  بازگشت درست;
}

منطقی
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ساختار OT::cff1_subset_plan &plan) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  اگر (!_serialize_cff1_charstrings(c, plan, *این))
    بازگشت نادرست;

  /* private dicts & local subrs */
  برای (صحیح i = (صحیح) privateDicts.length; --i >= 0 ;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      اگر (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->برداشت_pack ();
	وگرنه
	{
	  c->برداشت_discard ();
	  بازگشت نادرست;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      اگر (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->برداشت_pack ();
      }
      وگرنه
      {
	c->برداشت_discard ();
	بازگشت نادرست;
      }
    }
  }

  اگر (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  اگر (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    اگر (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->برداشت_pack (نادرست);
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* FDSelect */
  اگر (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs, *fdSelect, fd_شمار,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* Charset */
  اگر (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    اگر (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.رقم_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* Encoding */
  اگر (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    اگر (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_رقم_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    اگر (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->برداشت_pack (نادرست);
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    اگر (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->برداشت_pack ();
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  اگر (unlikely (!cff))
    بازگشت نادرست;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  اگر (unlikely (!c->embed (*nameIndex))) بازگشت نادرست;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *بالاترین_2 = c->push<بالاترین_Dict> ();
    cff1_بالاترین_dict_op_serializer_t بالاترین_Szr;
    unsigned بالاترین_size = 0;
    بالاترین_dict_modifiers_t  modifier (plan.info, plan.بالاترین_Dict_Mod_SIDs);
    اگر (likely (بالاترین_2->serialize (c, plan.topdict_mod, بالاترین_Szr, modifier)))
    {
      بالاترین_size = c->length ();
      c->برداشت_pack (نادرست);
    }
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    بازگشت dest->serialize_header (c, hb_iter (&بالاترین_size, 1), بالاترین_size);
  }
}

منطقی
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  اگر (unlikely (!cff_plan.create (*این, c->plan)))
  {
    DEBUG_پیام(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    بازگشت نادرست;
  }

  بازگشت serialize (c->serializer, cff_plan);
}


#endif
