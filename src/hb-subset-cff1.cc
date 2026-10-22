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

#অন্তর্ভাৱ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-ot-cff1-table.hh"
#অন্তর্ভাৱ "hb-set.h"
#অন্তর্ভাৱ "hb-bimap.hh"
#অন্তর্ভাৱ "hb-subset-plan.hh"
#অন্তর্ভাৱ "hb-subset-cff-common.hh"
#অন্তর্ভাৱ "hb-cff1-interp-cs.hh"

using নামাকাশ CFF;

সংরচনা remap_sid_t
{
  unsigned get_population () ধ্রুৱ { ঘূৰাই vector.length; }

  শূন্য alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  তার্কিক in_error () ধ্রুৱ
  { ঘূৰাই map.in_error () || vector.in_error (); }

  unsigned পূর্ণাঙ্ক add (unsigned পূর্ণাঙ্ক sid)
  {
    যদি (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ঘূৰাই sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    যদি (map.set (sid, v, মিছা))
    {
      vector.push (sid);
      next++;
    }
    অন্যথা
      v = map.get (sid); // already exists
    ঘূৰাই offset_sid (v);
  }

  unsigned পূর্ণাঙ্ক operator[] (unsigned পূর্ণাঙ্ক sid) ধ্রুৱ
  {
    যদি (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ঘূৰাই sid;

    ঘূৰাই offset_sid (map.get (unoffset_sid (sid)));
  }

  স্থির ধ্রুৱ unsigned পূর্ণাঙ্ক অঙ্ক_std_strings = 391;

  স্থির তার্কিক is_std_str (unsigned পূর্ণাঙ্ক sid) { ঘূৰাই sid < অঙ্ক_std_strings; }
  স্থির unsigned পূর্ণাঙ্ক offset_sid (unsigned পূর্ণাঙ্ক sid) { ঘূৰাই sid + অঙ্ক_std_strings; }
  স্থির unsigned পূর্ণাঙ্ক unoffset_sid (unsigned পূর্ণাঙ্ক sid) { ঘূৰাই sid - অঙ্ক_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

সংরচনা cff1_sub_table_info_t : cff_sub_table_info_t
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
সংরচনা cff1_শীৰ্ষ_dict_মানসমূহ_mod_t : cff1_শীৰ্ষ_dict_মানসমূহ_t
{
  শূন্য init (ধ্রুৱ cff1_শীৰ্ষ_dict_মানসমূহ_t *base_= &Null (cff1_শীৰ্ষ_dict_মানসমূহ_t))
  {
    SUPER::init ();
    base = base_;
  }

  শূন্য fini () { SUPER::fini (); }

  unsigned get_গুনতি () ধ্রুৱ { ঘূৰাই base->get_গুনতি () + SUPER::get_গুনতি (); }
  ধ্রুৱ cff1_শীৰ্ষ_dict_val_t &get_মান (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
    যদি (i < base->get_গুনতি ())
      ঘূৰাই (*base)[i];
    অন্যথা
      ঘূৰাই SUPER::values[i - base->get_গুনতি ()];
  }
  ধ্রুৱ cff1_শীৰ্ষ_dict_val_t &operator [] (unsigned পূর্ণাঙ্ক i) ধ্রুৱ { ঘূৰাই get_মান (i); }

  শূন্য reassignSIDs (ধ্রুৱ remap_sid_t& sidmap)
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  রক্ষিত:
  প্রকারনির্দেশ cff1_শীৰ্ষ_dict_মানসমূহ_t SUPER;
  ধ্রুৱ cff1_শীৰ্ষ_dict_মানসমূহ_t *base;
};

সংরচনা শীৰ্ষ_dict_modifiers_t
{
  শীৰ্ষ_dict_modifiers_t (ধ্রুৱ cff1_sub_table_info_t &info_,
			ধ্রুৱ unsigned পূর্ণাঙ্ক (&nameSIDs_)[name_dict_মানসমূহ_t::Val_গুনতি])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ধ্রুৱ cff1_sub_table_info_t &info;
  ধ্রুৱ unsigned পূর্ণাঙ্ক	(&nameSIDs)[name_dict_মানসমূহ_t::Val_গুনতি];
};

সংরচনা cff1_শীৰ্ষ_dict_op_serializer_t : cff_শীৰ্ষ_dict_op_serializer_t<cff1_শীৰ্ষ_dict_val_t>
{
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ cff1_শীৰ্ষ_dict_val_t &opstr,
		  ধ্রুৱ শীৰ্ষ_dict_modifiers_t &mod) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    op_code_t op = opstr.op;
    ৱিকল্প (op)
    {
      পক্ষ OpCode_charset:
	যদি (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	অন্যথা
	  গচ্ছ fall_back;

      পক্ষ OpCode_Encoding:
	যদি (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	অন্যথা
	  গচ্ছ fall_back;

      পক্ষ OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      পক্ষ OpCode_version:
      পক্ষ OpCode_Notice:
      পক্ষ OpCode_Copyright:
      পক্ষ OpCode_FullName:
      পক্ষ OpCode_FamilyName:
      পক্ষ OpCode_Weight:
      পক্ষ OpCode_PostScript:
      পক্ষ OpCode_BaseFontName:
      পক্ষ OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_মানসমূহ_t::name_op_to_index (op)]));

      পক্ষ OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  যদি ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (মিছা);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_মানসমূহ_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_মানসমূহ_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      স্ৱভাৱ:
	return_trace (cff_শীৰ্ষ_dict_op_serializer_t<cff1_শীৰ্ষ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (সঁচা);
  }

};

সংরচনা cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ op_str_t &opstr,
		  ধ্রুৱ cff1_font_dict_মানসমূহ_mod_t &mod) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    যদি (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    অন্যথা
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  গুপ্ত:
  প্রকারনির্দেশ cff_font_dict_op_serializer_t SUPER;
};

সংরচনা cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  স্থির শূন্য flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    যদি (env.arg_start > 0)
      flush_width (env, param);

    ৱিকল্প (op)
    {
      পক্ষ OpCode_hstem:
      পক্ষ OpCode_hstemhm:
      পক্ষ OpCode_vstem:
      পক্ষ OpCode_vstemhm:
      পক্ষ OpCode_hintmask:
      পক্ষ OpCode_cntrmask:
      পক্ষ OpCode_dotsection:
	যদি (param.drop_hints)
	{
	  env.clear_args ();
	  ঘূৰাই;
	}
	HB_FALLTHROUGH;

      স্ৱভাৱ:
	SUPER::flush_args_and_op (op, env, param);
	ভঙ্গ;
    }
  }
  স্থির শূন্য flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    বাবে (unsigned পূর্ণাঙ্ক i = env.arg_start; i < env.arg_স্তূপ.get_গুনতি (); i++)
      encoder.encode_অঙ্ক_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  স্থির শূন্য flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  স্থির শূন্য flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    প্রতিজ্ঞা (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_অঙ্ক_cs (env.width);
  }

  স্থির শূন্য flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    যদি (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  গুপ্ত:
  প্রকারনির্দেশ cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

সংরচনা range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  তার্কিক complete (unsigned পূর্ণাঙ্ক last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned গুনতি = এতত্->length;
    বাবে (unsigned পূর্ণাঙ্ক i = গুনতি; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned পূর্ণাঙ্ক n_বাম = last_glyph - pair.glyph - 1;
      all_glyphs |= n_বাম;
      last_glyph = pair.glyph;
      pair.glyph = n_বাম;
    }
    তার্কিক two_byte = all_glyphs >= 0x100;
    ঘূৰাই two_byte;
  }
};

সংরচনা cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  স্থির শূন্য process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    ৱিকল্প (op) {

      পক্ষ OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, মিছা);
	ভঙ্গ;

      পক্ষ OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	ভঙ্গ;

      পক্ষ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	ভঙ্গ;

      পক্ষ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	ভঙ্গ;

      স্ৱভাৱ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	ভঙ্গ;
    }
  }

  রক্ষিত:
  স্থির শূন্য process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_অঙ্ক_2);
    closure->add (env.context.subr_অঙ্ক_2);
    param.set_current_str (env, সঁচা);
  }

  গুপ্ত:
  প্রকারনির্দেশ cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

সংরচনা cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (তার্কিক desubroutinize_, তার্কিক drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ op_str_t &opstr,
		  objidx_t subrs_link) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    যদি (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (সঁচা);

    যদি (opstr.op == OpCode_Subrs)
    {
      যদি (desubroutinize || !subrs_link)
	return_trace (সঁচা);
      অন্যথা
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  রক্ষিত:
  ধ্রুৱ তার্কিক desubroutinize;
  ধ্রুৱ তার্কিক drop_hints;
};

সংরচনা cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ধ্রুৱ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ধ্রুৱ OT::cff1::accelerator_subset_t &acc_, ধ্রুৱ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  স্থির শূন্য complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    যদি (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < env.call_স্তূপ.get_গুনতি (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_স্তূপ[i]);
      যদি (likely (parsed_str))
	parsed_str->set_parsed ();
      অন্যথা
	env.set_error ();
    }
  }
};

নামাকাশ OT {
সংরচনা cff1_subset_plan
{
  cff1_subset_plan ()
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
      শীৰ্ষ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  শূন্য plan_subset_encoding (ধ্রুৱ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ধ্রুৱ Encoding *encoding = acc.encoding;
    unsigned পূর্ণাঙ্ক  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    যদি (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (মিছা);
      ঘূৰাই;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_অঙ্ক_codes = plan->অঙ্ক_output_glyphs () - 1;
    unsigned পূর্ণাঙ্ক glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    যদি (it->first == 0) it++;
    auto _ = *it;
    বাবে (glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      যদি (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      অন্যথা
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      যদি (code == CFF_UNDEF_CODE)
      {
	subset_enc_অঙ্ক_codes = glyph - 1;
	ভঙ্গ;
      }

      যদি (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      যদি (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	বাবে (unsigned পূর্ণাঙ্ক i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    প্রতিজ্ঞা (subset_enc_অঙ্ক_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_অঙ্ক_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    যদি (size0 < size1)
      subset_enc_format = 0;
    অন্যথা
      subset_enc_format = 1;
  }

  তার্কিক plan_subset_charset (ধ্রুৱ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned পূর্ণাঙ্ক  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    যদি (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (মিছা);
      ঘূৰাই মিছা;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();

    যদি (unlikely (!subset_charset_ranges.alloc (hb_min (অঙ্ক_glyphs,
							acc.অঙ্ক_charset_entries))))
    {
      plan->check_success (মিছা);
      ঘূৰাই মিছা;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    তার্কিক created_map = মিছা;
    যদি (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = সঁচা;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    যদি (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = মিছা;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    যদি (it->first == 0) it++;
    auto _ = *it;
    তার্কিক not_is_cid = !acc.is_CID ();
    তার্কিক skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    যদি (not_is_cid)
      sidmap.alloc (অঙ্ক_glyphs);
    বাবে (hb_codepoint_t glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      যদি (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      অন্যথা
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      যদি (identity_charset)
	sid = glyph;
      অন্যথা
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	যদি (not_is_cid)
	  sid = sidmap.add (sid);
      }

      যদি (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      যদি (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    যদি (created_map)
    {
      যদি ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    তার্কিক two_byte = subset_charset_ranges.complete (অঙ্ক_glyphs);

    size0 = Charset0::get_size (plan->অঙ্ক_output_glyphs ());
    যদি (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    অন্যথা
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    যদি (size0 < size_ranges)
      subset_charset_format = 0;
    অন্যথা যদি (!two_byte)
      subset_charset_format = 1;
    অন্যথা
      subset_charset_format = 2;

    ঘূৰাই সঁচা;
  }

  তার্কিক collect_sids_in_dicts (ধ্রুৱ OT::cff1::accelerator_subset_t &acc)
  {
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
    {
      unsigned পূর্ণাঙ্ক sid = acc.শীৰ্ষ_Dict_2.nameSIDs[i];
      যদি (sid != CFF_UNDEF_SID)
      {
	শীৰ্ষ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    যদি (acc.fdArray != &Null (CFF1FDArray))
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < orig_fdcount; i++)
	যদি (fdmap.has (i))
	  (শূন্য)sidmap.add (acc.fontDicts[i].fontName);

    ঘূৰাই সঁচা;
  }

  তার্কিক create (ধ্রুৱ OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    যদি (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ঘূৰাই মিছা;

    অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
    orig_fdcount = acc.fd_গুনতি;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #অন্যথা
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    যদি (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      বাবে (ধ্রুৱ auto &_ : plan->new_to_old_gid_list)
      {
	যদি (_.first != _.second)
	{
	  subset_charset = সঁচা;
	  ভঙ্গ;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.শীৰ্ষ_Dict_2);
      তার্কিক need_to_add_enc = (subset_encoding && !acc.শীৰ্ষ_Dict_2.has_op (OpCode_Encoding));
      তার্কিক need_to_add_set = (subset_charset && !acc.শীৰ্ষ_Dict_2.has_op (OpCode_charset));
      যদি (need_to_add_enc || need_to_add_set)
      {
	যদি (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	যদি (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    যদি (acc.fdSelect != &Null (CFF1FDSelect))
    {
	যদি (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.অঙ্ক_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	ঘূৰাই মিছা;
    }
    অন্যথা
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      যদি (unlikely (!collect_sids_in_dicts (acc)))
	ঘূৰাই মিছা;
      যদি (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	ঘূৰাই মিছা;

      যদি (subset_charset && !plan_subset_charset (acc, plan))
        ঘূৰাই মিছা;

      topdict_mod.reassignSIDs (sidmap);
    }

    যদি (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ধ্রুৱ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      যদি (!flattener.flatten (subset_charstrings))
	ঘূৰাই মিছা;
    }
    অন্যথা
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      যদি (!subr_subsetter.subset ())
	ঘূৰাই মিছা;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      যদি (!subr_subsetter.encode_charstrings (subset_charstrings))
	ঘূৰাই মিছা;

      যদি (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ঘূৰাই মিছা;

      /* local subrs */
      যদি (!subset_localsubrs.resize (orig_fdcount))
	ঘূৰাই মিছা;
      বাবে (unsigned পূর্ণাঙ্ক fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	যদি (fdmap.has (fd))
	{
	  যদি (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    ঘূৰাই মিছা;
	}
      }
    }

    /* Encoding */
    যদি (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    যদি (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_মানসমূহ_mod_t ());
    অন্যথা
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ধ্রুৱ cff1_font_dict_মানসমূহ_t &_)
	{ ঘূৰাই fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ধ্রুৱ cff1_font_dict_মানসমূহ_t &_)
	{
	  cff1_font_dict_মানসমূহ_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  ঘূৰাই mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    ঘূৰাই !plan->in_error () &&
	   (subset_charstrings.length == plan->অঙ্ক_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_শীৰ্ষ_dict_মানসমূহ_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned পূর্ণাঙ্ক    অঙ্ক_glyphs;
  unsigned পূর্ণাঙ্ক    orig_fdcount = 0;
  unsigned পূর্ণাঙ্ক    subset_fdcount = 1;
  unsigned পূর্ণাঙ্ক    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_মানসমূহ_mod_t>  fontdicts_mod;

  তার্কিক		drop_hints = মিছা;

  তার্কিক		gid_renum;
  তার্কিক		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned পূর্ণাঙ্ক	subset_enc_অঙ্ক_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  তার্কিক		subset_charset;

  remap_sid_t	sidmap;
  unsigned পূর্ণাঙ্ক	শীৰ্ষ_Dict_Mod_SIDs[name_dict_মানসমূহ_t::Val_গুনতি];

  তার্কিক		desubroutinize = মিছা;
  তার্কিক		identity_charset = মিছা;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

স্থির তার্কিক _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         সংরচনা OT::cff1_subset_plan &plan,
                                         ধ্রুৱ OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  যদি (unlikely (!c->start_zerocopy (total_size)))
    ঘূৰাই মিছা;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  যদি (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->বেৰ_কৰো_discard ();
    ঘূৰাই মিছা;
  }

  plan.info.char_strings_link = c->বেৰ_কৰো_pack (মিছা);
  ঘূৰাই সঁচা;
}

তার্কিক
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   সংরচনা OT::cff1_subset_plan &plan) ধ্রুৱ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  যদি (!_serialize_cff1_charstrings(c, plan, *এতত্))
    ঘূৰাই মিছা;

  /* private dicts & local subrs */
  বাবে (পূর্ণাঙ্ক i = (পূর্ণাঙ্ক) privateDicts.length; --i >= 0 ;)
  {
    যদি (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      যদি (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	যদি (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->বেৰ_কৰো_pack ();
	অন্যথা
	{
	  c->বেৰ_কৰো_discard ();
	  ঘূৰাই মিছা;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      যদি (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->বেৰ_কৰো_pack ();
      }
      অন্যথা
      {
	c->বেৰ_কৰো_discard ();
	ঘূৰাই মিছা;
      }
    }
  }

  যদি (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  যদি (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    যদি (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->বেৰ_কৰো_pack (মিছা);
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  /* FDSelect */
  যদি (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    যদি (likely (hb_serialize_cff_fdselect (c, plan.অঙ্ক_glyphs, *fdSelect, fd_গুনতি,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->বেৰ_কৰো_pack ();
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  /* Charset */
  যদি (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    যদি (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.অঙ্ক_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->বেৰ_কৰো_pack ();
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  /* Encoding */
  যদি (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    যদি (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_অঙ্ক_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->বেৰ_কৰো_pack ();
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    যদি (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->বেৰ_কৰো_pack (মিছা);
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    যদি (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->বেৰ_কৰো_pack ();
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  যদি (unlikely (!cff))
    ঘূৰাই মিছা;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  যদি (unlikely (!c->embed (*nameIndex))) ঘূৰাই মিছা;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *শীৰ্ষ_2 = c->push<শীৰ্ষ_Dict> ();
    cff1_শীৰ্ষ_dict_op_serializer_t শীৰ্ষ_Szr;
    unsigned শীৰ্ষ_size = 0;
    শীৰ্ষ_dict_modifiers_t  modifier (plan.info, plan.শীৰ্ষ_Dict_Mod_SIDs);
    যদি (likely (শীৰ্ষ_2->serialize (c, plan.topdict_mod, শীৰ্ষ_Szr, modifier)))
    {
      শীৰ্ষ_size = c->length ();
      c->বেৰ_কৰো_pack (মিছা);
    }
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    ঘূৰাই dest->serialize_header (c, hb_iter (&শীৰ্ষ_size, 1), শীৰ্ষ_size);
  }
}

তার্কিক
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ধ্রুৱ
{
  cff1_subset_plan cff_plan;

  যদি (unlikely (!cff_plan.create (*এতত্, c->plan)))
  {
    DEBUG_বাৰ্তা(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    ঘূৰাই মিছা;
  }

  ঘূৰাই serialize (c->serializer, cff_plan);
}


#endif
