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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-cff1-table.hh"
#অন্তর্ভাব "hb-set.h"
#অন্তর্ভাব "hb-bimap.hh"
#অন্তর্ভাব "hb-subset-plan.hh"
#অন্তর্ভাব "hb-subset-cff-common.hh"
#অন্তর্ভাব "hb-cff1-interp-cs.hh"

using নামাকাশ CFF;

কাঠামো remap_sid_t
{
  unsigned get_population () ধ্রুবক { ফেরত vector.দৈর্ঘ্য; }

  শূন্য alloc (unsigned আকার_2)
  {
    map.alloc (আকার_2);
    vector.alloc_exact (আকার_2);
  }

  যৌক্তিক in_error () ধ্রুবক
  { ফেরত map.in_error () || vector.in_error (); }

  unsigned পূর্ণসংখ্যা add (unsigned পূর্ণসংখ্যা sid)
  {
    যদি (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ফেরত sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    যদি (map.set (sid, v, মিথ্যা))
    {
      vector.push (sid);
      next++;
    }
    অন্যথা
      v = map.get (sid); // already exists
    ফেরত offset_sid (v);
  }

  unsigned পূর্ণসংখ্যা operator[] (unsigned পূর্ণসংখ্যা sid) ধ্রুবক
  {
    যদি (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ফেরত sid;

    ফেরত offset_sid (map.get (unoffset_sid (sid)));
  }

  স্থির ধ্রুবক unsigned পূর্ণসংখ্যা অঙ্ক_std_strings = 391;

  স্থির যৌক্তিক is_std_str (unsigned পূর্ণসংখ্যা sid) { ফেরত sid < অঙ্ক_std_strings; }
  স্থির unsigned পূর্ণসংখ্যা offset_sid (unsigned পূর্ণসংখ্যা sid) { ফেরত sid + অঙ্ক_std_strings; }
  স্থির unsigned পূর্ণসংখ্যা unoffset_sid (unsigned পূর্ণসংখ্যা sid) { ফেরত sid - অঙ্ক_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

কাঠামো cff1_sub_table_info_t : cff_sub_table_info_t
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
কাঠামো cff1_শীর্ষ_dict_মানসমূহ_mod_t : cff1_শীর্ষ_dict_মানসমূহ_t
{
  শূন্য init (ধ্রুবক cff1_শীর্ষ_dict_মানসমূহ_t *base_= &Null (cff1_শীর্ষ_dict_মানসমূহ_t))
  {
    SUPER::init ();
    base = base_;
  }

  শূন্য fini () { SUPER::fini (); }

  unsigned get_গুনতি () ধ্রুবক { ফেরত base->get_গুনতি () + SUPER::get_গুনতি (); }
  ধ্রুবক cff1_শীর্ষ_dict_val_t &get_মান (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
    যদি (i < base->get_গুনতি ())
      ফেরত (*base)[i];
    অন্যথা
      ফেরত SUPER::values[i - base->get_গুনতি ()];
  }
  ধ্রুবক cff1_শীর্ষ_dict_val_t &operator [] (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত get_মান (i); }

  শূন্য reassignSIDs (ধ্রুবক remap_sid_t& sidmap)
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  সুরক্ষিত:
  প্রকারলেখ cff1_শীর্ষ_dict_মানসমূহ_t SUPER;
  ধ্রুবক cff1_শীর্ষ_dict_মানসমূহ_t *base;
};

কাঠামো শীর্ষ_dict_modifiers_t
{
  শীর্ষ_dict_modifiers_t (ধ্রুবক cff1_sub_table_info_t &info_,
			ধ্রুবক unsigned পূর্ণসংখ্যা (&nameSIDs_)[name_dict_মানসমূহ_t::Val_গুনতি])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ধ্রুবক cff1_sub_table_info_t &info;
  ধ্রুবক unsigned পূর্ণসংখ্যা	(&nameSIDs)[name_dict_মানসমূহ_t::Val_গুনতি];
};

কাঠামো cff1_শীর্ষ_dict_op_serializer_t : cff_শীর্ষ_dict_op_serializer_t<cff1_শীর্ষ_dict_val_t>
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক cff1_শীর্ষ_dict_val_t &opstr,
		  ধ্রুবক শীর্ষ_dict_modifiers_t &mod) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    op_code_t op = opstr.op;
    নির্বাচন (op)
    {
      ক্ষেত্র OpCode_charset:
	যদি (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	অন্যথা
	  যাও fall_back;

      ক্ষেত্র OpCode_Encoding:
	যদি (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	অন্যথা
	  যাও fall_back;

      ক্ষেত্র OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.আকার_2) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      ক্ষেত্র OpCode_version:
      ক্ষেত্র OpCode_Notice:
      ক্ষেত্র OpCode_Copyright:
      ক্ষেত্র OpCode_FullName:
      ক্ষেত্র OpCode_FamilyName:
      ক্ষেত্র OpCode_Weight:
      ক্ষেত্র OpCode_PostScript:
      ক্ষেত্র OpCode_BaseFontName:
      ক্ষেত্র OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_মানসমূহ_t::name_op_to_সূচক (op)]));

      ক্ষেত্র OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  যদি ( unlikely (!(opstr.দৈর্ঘ্য >= opstr.last_arg_offset + 3)))
	    return_trace (মিথ্যা);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.দৈর্ঘ্য = opstr.দৈর্ঘ্য - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_মানসমূহ_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_মানসমূহ_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      পূর্বনির্ধারিত:
	return_trace (cff_শীর্ষ_dict_op_serializer_t<cff1_শীর্ষ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (সত্য);
  }

};

কাঠামো cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক op_str_t &opstr,
		  ধ্রুবক cff1_font_dict_মানসমূহ_mod_t &mod) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    যদি (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    অন্যথা
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  ব্যক্তিগত:
  প্রকারলেখ cff_font_dict_op_serializer_t SUPER;
};

কাঠামো cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  স্থির শূন্য flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    যদি (env.arg_start > 0)
      flush_width (env, param);

    নির্বাচন (op)
    {
      ক্ষেত্র OpCode_hstem:
      ক্ষেত্র OpCode_hstemhm:
      ক্ষেত্র OpCode_vstem:
      ক্ষেত্র OpCode_vstemhm:
      ক্ষেত্র OpCode_hintmask:
      ক্ষেত্র OpCode_cntrmask:
      ক্ষেত্র OpCode_dotsection:
	যদি (param.drop_hints)
	{
	  env.clear_args ();
	  ফেরত;
	}
	HB_FALLTHROUGH;

      পূর্বনির্ধারিত:
	SUPER::flush_args_and_op (op, env, param);
	বিরতি;
    }
  }
  স্থির শূন্য flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    জন্য (unsigned পূর্ণসংখ্যা i = env.arg_start; i < env.arg_স্তূপ.get_গুনতি (); i++)
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
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.hintmask_আকার; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ব্যক্তিগত:
  প্রকারলেখ cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

কাঠামো range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  যৌক্তিক complete (unsigned পূর্ণসংখ্যা last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned গুনতি = এই->দৈর্ঘ্য;
    জন্য (unsigned পূর্ণসংখ্যা i = গুনতি; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned পূর্ণসংখ্যা n_বাম = last_glyph - pair.glyph - 1;
      all_glyphs |= n_বাম;
      last_glyph = pair.glyph;
      pair.glyph = n_বাম;
    }
    যৌক্তিক two_byte = all_glyphs >= 0x100;
    ফেরত two_byte;
  }
};

কাঠামো cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  স্থির শূন্য process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    নির্বাচন (op) {

      ক্ষেত্র OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, মিথ্যা);
	বিরতি;

      ক্ষেত্র OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	বিরতি;

      ক্ষেত্র OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	বিরতি;

      ক্ষেত্র OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	বিরতি;
    }
  }

  সুরক্ষিত:
  স্থির শূন্য process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_অঙ্ক_2);
    closure->add (env.context.subr_অঙ্ক_2);
    param.set_current_str (env, সত্য);
  }

  ব্যক্তিগত:
  প্রকারলেখ cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

কাঠামো cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (যৌক্তিক desubroutinize_, যৌক্তিক drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক op_str_t &opstr,
		  objidx_t subrs_link) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    যদি (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (সত্য);

    যদি (opstr.op == OpCode_Subrs)
    {
      যদি (desubroutinize || !subrs_link)
	return_trace (সত্য);
      অন্যথা
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  সুরক্ষিত:
  ধ্রুবক যৌক্তিক desubroutinize;
  ধ্রুবক যৌক্তিক drop_hints;
};

কাঠামো cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ধ্রুবক OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ধ্রুবক OT::cff1::accelerator_subset_t &acc_, ধ্রুবক hb_subset_plan_t *plan_)
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
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < env.call_স্তূপ.get_গুনতি (); i++)
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
কাঠামো cff1_subset_plan
{
  cff1_subset_plan ()
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
      শীর্ষ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  শূন্য plan_subset_encoding (ধ্রুবক OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ধ্রুবক Encoding *encoding = acc.encoding;
    unsigned পূর্ণসংখ্যা  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    যদি (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (মিথ্যা);
      ফেরত;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_অঙ্ক_codes = plan->অঙ্ক_output_glyphs () - 1;
    unsigned পূর্ণসংখ্যা glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    যদি (it->first == 0) it++;
    auto _ = *it;
    জন্য (glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
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
	বিরতি;
      }

      যদি (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      যদি (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < supp_codes.দৈর্ঘ্য; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    প্রতিজ্ঞা (subset_enc_অঙ্ক_codes <= 0xFF);
    size0 = Encoding0::min_আকার + HBUINT8::static_আকার * subset_enc_অঙ্ক_codes;
    size1 = Encoding1::min_আকার + Encoding1_Range::static_আকার * subset_enc_code_ranges.দৈর্ঘ্য;

    যদি (size0 < size1)
      subset_enc_format = 0;
    অন্যথা
      subset_enc_format = 1;
  }

  যৌক্তিক plan_subset_charset (ধ্রুবক OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned পূর্ণসংখ্যা  size0, আকার_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    যদি (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (মিথ্যা);
      ফেরত মিথ্যা;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();

    যদি (unlikely (!subset_charset_ranges.alloc (hb_min (অঙ্ক_glyphs,
							acc.অঙ্ক_charset_entries))))
    {
      plan->check_success (মিথ্যা);
      ফেরত মিথ্যা;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    যৌক্তিক created_map = মিথ্যা;
    যদি (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = সত্য;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    যদি (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = মিথ্যা;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    যদি (it->first == 0) it++;
    auto _ = *it;
    যৌক্তিক not_is_cid = !acc.is_CID ();
    যৌক্তিক skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    যদি (not_is_cid)
      sidmap.alloc (অঙ্ক_glyphs);
    জন্য (hb_codepoint_t glyph = 1; glyph < অঙ্ক_glyphs; glyph++)
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

    যৌক্তিক two_byte = subset_charset_ranges.complete (অঙ্ক_glyphs);

    size0 = Charset0::get_আকার (plan->অঙ্ক_output_glyphs ());
    যদি (!two_byte)
      আকার_ranges = Charset1::get_আকার_for_ranges (subset_charset_ranges.দৈর্ঘ্য);
    অন্যথা
      আকার_ranges = Charset2::get_আকার_for_ranges (subset_charset_ranges.দৈর্ঘ্য);

    যদি (size0 < আকার_ranges)
      subset_charset_format = 0;
    অন্যথা যদি (!two_byte)
      subset_charset_format = 1;
    অন্যথা
      subset_charset_format = 2;

    ফেরত সত্য;
  }

  যৌক্তিক collect_sids_in_dicts (ধ্রুবক OT::cff1::accelerator_subset_t &acc)
  {
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < name_dict_মানসমূহ_t::Val_গুনতি; i++)
    {
      unsigned পূর্ণসংখ্যা sid = acc.শীর্ষ_Dict_2.nameSIDs[i];
      যদি (sid != CFF_UNDEF_SID)
      {
	শীর্ষ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    যদি (acc.fdArray != &Null (CFF1FDArray))
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < orig_fdcount; i++)
	যদি (fdmap.has (i))
	  (শূন্য)sidmap.add (acc.fontDicts[i].fontName);

    ফেরত সত্য;
  }

  যৌক্তিক create (ধ্রুবক OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    যদি (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ফেরত মিথ্যা;

    অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
    orig_fdcount = acc.fd_গুনতি;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_আকার = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #অন্যথা
    min_charstrings_off_আকার = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    যদি (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      জন্য (ধ্রুবক auto &_ : plan->new_to_old_gid_list)
      {
	যদি (_.first != _.second)
	{
	  subset_charset = সত্য;
	  বিরতি;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.শীর্ষ_Dict_2);
      যৌক্তিক need_to_add_enc = (subset_encoding && !acc.শীর্ষ_Dict_2.has_op (OpCode_Encoding));
      যৌক্তিক need_to_add_set = (subset_charset && !acc.শীর্ষ_Dict_2.has_op (OpCode_charset));
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
				  info.fd_select.আকার_2,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	ফেরত মিথ্যা;
    }
    অন্যথা
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      যদি (unlikely (!collect_sids_in_dicts (acc)))
	ফেরত মিথ্যা;
      যদি (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	ফেরত মিথ্যা;

      যদি (subset_charset && !plan_subset_charset (acc, plan))
        ফেরত মিথ্যা;

      topdict_mod.reassignSIDs (sidmap);
    }

    যদি (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ধ্রুবক OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      যদি (!flattener.flatten (subset_charstrings))
	ফেরত মিথ্যা;
    }
    অন্যথা
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      যদি (!subr_subsetter.subset ())
	ফেরত মিথ্যা;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      যদি (!subr_subsetter.encode_charstrings (subset_charstrings))
	ফেরত মিথ্যা;

      যদি (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ফেরত মিথ্যা;

      /* local subrs */
      যদি (!subset_localsubrs.resize (orig_fdcount))
	ফেরত মিথ্যা;
      জন্য (unsigned পূর্ণসংখ্যা fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	যদি (fdmap.has (fd))
	{
	  যদি (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    ফেরত মিথ্যা;
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
      | hb_filter ([&] (ধ্রুবক cff1_font_dict_মানসমূহ_t &_)
	{ ফেরত fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ধ্রুবক cff1_font_dict_মানসমূহ_t &_)
	{
	  cff1_font_dict_মানসমূহ_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  ফেরত mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    ফেরত !plan->in_error () &&
	   (subset_charstrings.দৈর্ঘ্য == plan->অঙ্ক_output_glyphs ()) &&
	   (fontdicts_mod.দৈর্ঘ্য == subset_fdcount);
  }

  cff1_শীর্ষ_dict_মানসমূহ_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned পূর্ণসংখ্যা    অঙ্ক_glyphs;
  unsigned পূর্ণসংখ্যা    orig_fdcount = 0;
  unsigned পূর্ণসংখ্যা    subset_fdcount = 1;
  unsigned পূর্ণসংখ্যা    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_মানসমূহ_mod_t>  fontdicts_mod;

  যৌক্তিক		drop_hints = মিথ্যা;

  যৌক্তিক		gid_renum;
  যৌক্তিক		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned পূর্ণসংখ্যা	subset_enc_অঙ্ক_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  যৌক্তিক		subset_charset;

  remap_sid_t	sidmap;
  unsigned পূর্ণসংখ্যা	শীর্ষ_Dict_Mod_SIDs[name_dict_মানসমূহ_t::Val_গুনতি];

  যৌক্তিক		desubroutinize = মিথ্যা;
  যৌক্তিক		identity_charset = মিথ্যা;

  unsigned	min_charstrings_off_আকার = 0;
};
} // namespace OT

স্থির যৌক্তিক _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         কাঠামো OT::cff1_subset_plan &plan,
                                         ধ্রুবক OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned তথ্য_আকার = 0;
  unsigned যোগ_আকার = CFF1CharStrings::যোগ_আকার (plan.subset_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার);
  যদি (unlikely (!c->start_zerocopy (যোগ_আকার)))
    ফেরত মিথ্যা;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  যদি (unlikely (!cs->serialize (c, plan.subset_charstrings, &তথ্য_আকার, plan.min_charstrings_off_আকার))) {
    c->বের_করো_discard ();
    ফেরত মিথ্যা;
  }

  plan.info.char_strings_link = c->বের_করো_pack (মিথ্যা);
  ফেরত সত্য;
}

যৌক্তিক
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   কাঠামো OT::cff1_subset_plan &plan) ধ্রুবক
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  যদি (!_serialize_cff1_charstrings(c, plan, *এই))
    ফেরত মিথ্যা;

  /* private dicts & local subrs */
  জন্য (পূর্ণসংখ্যা i = (পূর্ণসংখ্যা) privateDicts.দৈর্ঘ্য; --i >= 0 ;)
  {
    যদি (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      যদি (plan.subset_localsubrs[i].দৈর্ঘ্য > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	যদি (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->বের_করো_pack ();
	অন্যথা
	{
	  c->বের_করো_discard ();
	  ফেরত মিথ্যা;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      যদি (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.আকার_2 = c->দৈর্ঘ্য ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->বের_করো_pack ();
      }
      অন্যথা
      {
	c->বের_করো_discard ();
	ফেরত মিথ্যা;
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
      plan.info.fd_array_link = c->বের_করো_pack (মিথ্যা);
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  /* FDSelect */
  যদি (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    যদি (likely (hb_serialize_cff_fdselect (c, plan.অঙ্ক_glyphs, *fdSelect, fd_গুনতি,
					   plan.subset_fdselect_format, plan.info.fd_select.আকার_2,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
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
      plan.info.charset_link = c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
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
      plan.info.encoding_link = c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    যদি (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->বের_করো_pack (মিথ্যা);
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1_String_সূচক> ();
    যদি (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *string_সূচক, plan.sidmap.vector)))
      c->বের_করো_pack ();
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  যদি (unlikely (!cff))
    ফেরত মিথ্যা;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->name_সূচক = cff->min_আকার;
  cff->off_আকার = 4; /* unused? */

  /* name INDEX */
  যদি (unlikely (!c->embed (*name_সূচক))) ফেরত মিথ্যা;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *শীর্ষ_4 = c->push<শীর্ষ_Dict> ();
    cff1_শীর্ষ_dict_op_serializer_t শীর্ষ_Szr;
    unsigned শীর্ষ_আকার = 0;
    শীর্ষ_dict_modifiers_t  modifier (plan.info, plan.শীর্ষ_Dict_Mod_SIDs);
    যদি (likely (শীর্ষ_4->serialize (c, plan.topdict_mod, শীর্ষ_Szr, modifier)))
    {
      শীর্ষ_আকার = c->দৈর্ঘ্য ();
      c->বের_করো_pack (মিথ্যা);
    }
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1_সূচক> ();
    ফেরত dest->serialize_header (c, hb_iter (&শীর্ষ_আকার, 1), শীর্ষ_আকার);
  }
}

যৌক্তিক
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ধ্রুবক
{
  cff1_subset_plan cff_plan;

  যদি (unlikely (!cff_plan.create (*এই, c->plan)))
  {
    DEBUG_বার্তা(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    ফেরত মিথ্যা;
  }

  ফেরত serialize (c->serializer, cff_plan);
}


#endif
