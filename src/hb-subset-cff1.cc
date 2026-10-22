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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-cff1-table.hh"
#अन्तर्भाव "hb-set.h"
#अन्तर्भाव "hb-bimap.hh"
#अन्तर्भाव "hb-subset-plan.hh"
#अन्तर्भाव "hb-subset-cff-common.hh"
#अन्तर्भाव "hb-cff1-interp-cs.hh"

using नामाकाश CFF;

संरचना remap_sid_t
{
  unsigned get_population () ध्रुव { प्रत्यागम vector.length; }

  शून्य alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  तार्किक in_error () ध्रुव
  { प्रत्यागम map.in_error () || vector.in_error (); }

  unsigned पूर्णाङ्क add (unsigned पूर्णाङ्क sid)
  {
    यदि (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      प्रत्यागम sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    यदि (map.set (sid, v, असत्य))
    {
      vector.push (sid);
      next++;
    }
    अन्यथा
      v = map.get (sid); // already exists
    प्रत्यागम offset_sid (v);
  }

  unsigned पूर्णाङ्क operator[] (unsigned पूर्णाङ्क sid) ध्रुव
  {
    यदि (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      प्रत्यागम sid;

    प्रत्यागम offset_sid (map.get (unoffset_sid (sid)));
  }

  स्थिर ध्रुव unsigned पूर्णाङ्क अंक_std_strings = 391;

  स्थिर तार्किक is_std_str (unsigned पूर्णाङ्क sid) { प्रत्यागम sid < अंक_std_strings; }
  स्थिर unsigned पूर्णाङ्क offset_sid (unsigned पूर्णाङ्क sid) { प्रत्यागम sid + अंक_std_strings; }
  स्थिर unsigned पूर्णाङ्क unoffset_sid (unsigned पूर्णाङ्क sid) { प्रत्यागम sid - अंक_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

संरचना cff1_sub_table_info_t : cff_sub_table_info_t
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
संरचना cff1_शीर्ष_dict_मूल्यावली_mod_t : cff1_शीर्ष_dict_मूल्यावली_t
{
  शून्य init (ध्रुव cff1_शीर्ष_dict_मूल्यावली_t *base_= &Null (cff1_शीर्ष_dict_मूल्यावली_t))
  {
    SUPER::init ();
    base = base_;
  }

  शून्य fini () { SUPER::fini (); }

  unsigned get_गिनती () ध्रुव { प्रत्यागम base->get_गिनती () + SUPER::get_गिनती (); }
  ध्रुव cff1_शीर्ष_dict_val_t &get_मूल्य (unsigned पूर्णाङ्क i) ध्रुव
  {
    यदि (i < base->get_गिनती ())
      प्रत्यागम (*base)[i];
    अन्यथा
      प्रत्यागम SUPER::values[i - base->get_गिनती ()];
  }
  ध्रुव cff1_शीर्ष_dict_val_t &operator [] (unsigned पूर्णाङ्क i) ध्रुव { प्रत्यागम get_मूल्य (i); }

  शून्य reassignSIDs (ध्रुव remap_sid_t& sidmap)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  रक्षित:
  प्रकारनिर्देश cff1_शीर्ष_dict_मूल्यावली_t SUPER;
  ध्रुव cff1_शीर्ष_dict_मूल्यावली_t *base;
};

संरचना शीर्ष_dict_modifiers_t
{
  शीर्ष_dict_modifiers_t (ध्रुव cff1_sub_table_info_t &info_,
			ध्रुव unsigned पूर्णाङ्क (&nameSIDs_)[name_dict_मूल्यावली_t::Val_गिनती])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ध्रुव cff1_sub_table_info_t &info;
  ध्रुव unsigned पूर्णाङ्क	(&nameSIDs)[name_dict_मूल्यावली_t::Val_गिनती];
};

संरचना cff1_शीर्ष_dict_op_serializer_t : cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>
{
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव cff1_शीर्ष_dict_val_t &opstr,
		  ध्रुव शीर्ष_dict_modifiers_t &mod) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    op_code_t op = opstr.op;
    विकल्प (op)
    {
      पक्ष OpCode_charset:
	यदि (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	अन्यथा
	  गच्छ fall_back;

      पक्ष OpCode_Encoding:
	यदि (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	अन्यथा
	  गच्छ fall_back;

      पक्ष OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      पक्ष OpCode_version:
      पक्ष OpCode_Notice:
      पक्ष OpCode_Copyright:
      पक्ष OpCode_FullName:
      पक्ष OpCode_FamilyName:
      पक्ष OpCode_Weight:
      पक्ष OpCode_PostScript:
      पक्ष OpCode_BaseFontName:
      पक्ष OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_मूल्यावली_t::name_op_to_index (op)]));

      पक्ष OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  यदि ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (असत्य);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्यावली_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्यावली_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      स्वभाव:
	return_trace (cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (सत्य);
  }

};

संरचना cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव op_str_t &opstr,
		  ध्रुव cff1_font_dict_मूल्यावली_mod_t &mod) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    यदि (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    अन्यथा
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  गुप्त:
  प्रकारनिर्देश cff_font_dict_op_serializer_t SUPER;
};

संरचना cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    यदि (env.arg_start > 0)
      flush_width (env, param);

    विकल्प (op)
    {
      पक्ष OpCode_hstem:
      पक्ष OpCode_hstemhm:
      पक्ष OpCode_vstem:
      पक्ष OpCode_vstemhm:
      पक्ष OpCode_hintmask:
      पक्ष OpCode_cntrmask:
      पक्ष OpCode_dotsection:
	यदि (param.drop_hints)
	{
	  env.clear_args ();
	  प्रत्यागम;
	}
	HB_FALLTHROUGH;

      स्वभाव:
	SUPER::flush_args_and_op (op, env, param);
	भङ्ग;
    }
  }
  स्थिर शून्य flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    प्रति (unsigned पूर्णाङ्क i = env.arg_start; i < env.arg_ढेर.get_गिनती (); i++)
      encoder.encode_अंक_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  स्थिर शून्य flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  स्थिर शून्य flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    प्रतिज्ञा (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_अंक_cs (env.width);
  }

  स्थिर शून्य flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    यदि (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      प्रति (unsigned पूर्णाङ्क i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  गुप्त:
  प्रकारनिर्देश cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

संरचना range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  तार्किक complete (unsigned पूर्णाङ्क last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned गिनती = एतत्->length;
    प्रति (unsigned पूर्णाङ्क i = गिनती; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned पूर्णाङ्क n_बायाँ = last_glyph - pair.glyph - 1;
      all_glyphs |= n_बायाँ;
      last_glyph = pair.glyph;
      pair.glyph = n_बायाँ;
    }
    तार्किक two_byte = all_glyphs >= 0x100;
    प्रत्यागम two_byte;
  }
};

संरचना cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  स्थिर शून्य process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    विकल्प (op) {

      पक्ष OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, असत्य);
	भङ्ग;

      पक्ष OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	भङ्ग;

      पक्ष OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	भङ्ग;

      पक्ष OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	भङ्ग;
    }
  }

  रक्षित:
  स्थिर शून्य process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_अंक_2);
    closure->add (env.context.subr_अंक_2);
    param.set_current_str (env, सत्य);
  }

  गुप्त:
  प्रकारनिर्देश cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

संरचना cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (तार्किक desubroutinize_, तार्किक drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव op_str_t &opstr,
		  objidx_t subrs_link) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    यदि (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (सत्य);

    यदि (opstr.op == OpCode_Subrs)
    {
      यदि (desubroutinize || !subrs_link)
	return_trace (सत्य);
      अन्यथा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  रक्षित:
  ध्रुव तार्किक desubroutinize;
  ध्रुव तार्किक drop_hints;
};

संरचना cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ध्रुव OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ध्रुव OT::cff1::accelerator_subset_t &acc_, ध्रुव hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  स्थिर शून्य complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    यदि (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < env.call_ढेर.get_गिनती (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ढेर[i]);
      यदि (likely (parsed_str))
	parsed_str->set_parsed ();
      अन्यथा
	env.set_error ();
    }
  }
};

नामाकाश OT {
संरचना cff1_subset_plan
{
  cff1_subset_plan ()
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
      शीर्ष_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  शून्य plan_subset_encoding (ध्रुव OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ध्रुव Encoding *encoding = acc.encoding;
    unsigned पूर्णाङ्क  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    यदि (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (असत्य);
      प्रत्यागम;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_अंक_codes = plan->अंक_output_glyphs () - 1;
    unsigned पूर्णाङ्क glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    यदि (it->first == 0) it++;
    auto _ = *it;
    प्रति (glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      यदि (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      अन्यथा
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      यदि (code == CFF_UNDEF_CODE)
      {
	subset_enc_अंक_codes = glyph - 1;
	भङ्ग;
      }

      यदि (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      यदि (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	प्रति (unsigned पूर्णाङ्क i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    प्रतिज्ञा (subset_enc_अंक_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_अंक_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    यदि (size0 < size1)
      subset_enc_format = 0;
    अन्यथा
      subset_enc_format = 1;
  }

  तार्किक plan_subset_charset (ध्रुव OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned पूर्णाङ्क  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    यदि (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (असत्य);
      प्रत्यागम असत्य;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned अंक_glyphs = plan->अंक_output_glyphs ();

    यदि (unlikely (!subset_charset_ranges.alloc (hb_min (अंक_glyphs,
							acc.अंक_charset_entries))))
    {
      plan->check_success (असत्य);
      प्रत्यागम असत्य;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    तार्किक created_map = असत्य;
    यदि (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = सत्य;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    यदि (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = असत्य;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    यदि (it->first == 0) it++;
    auto _ = *it;
    तार्किक not_is_cid = !acc.is_CID ();
    तार्किक skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    यदि (not_is_cid)
      sidmap.alloc (अंक_glyphs);
    प्रति (hb_codepoint_t glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      यदि (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      अन्यथा
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      यदि (identity_charset)
	sid = glyph;
      अन्यथा
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	यदि (not_is_cid)
	  sid = sidmap.add (sid);
      }

      यदि (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      यदि (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    यदि (created_map)
    {
      यदि ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    तार्किक two_byte = subset_charset_ranges.complete (अंक_glyphs);

    size0 = Charset0::get_size (plan->अंक_output_glyphs ());
    यदि (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    अन्यथा
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    यदि (size0 < size_ranges)
      subset_charset_format = 0;
    अन्यथा यदि (!two_byte)
      subset_charset_format = 1;
    अन्यथा
      subset_charset_format = 2;

    प्रत्यागम सत्य;
  }

  तार्किक collect_sids_in_dicts (ध्रुव OT::cff1::accelerator_subset_t &acc)
  {
    प्रति (unsigned पूर्णाङ्क i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
    {
      unsigned पूर्णाङ्क sid = acc.शीर्ष_Dict_2.nameSIDs[i];
      यदि (sid != CFF_UNDEF_SID)
      {
	शीर्ष_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    यदि (acc.fdArray != &Null (CFF1FDArray))
      प्रति (unsigned पूर्णाङ्क i = 0; i < orig_fdcount; i++)
	यदि (fdmap.has (i))
	  (शून्य)sidmap.add (acc.fontDicts[i].fontName);

    प्रत्यागम सत्य;
  }

  तार्किक create (ध्रुव OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    यदि (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) प्रत्यागम असत्य;

    अंक_glyphs = plan->अंक_output_glyphs ();
    orig_fdcount = acc.fd_गिनती;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अन्यथा
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    यदि (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      प्रति (ध्रुव auto &_ : plan->new_to_old_gid_list)
      {
	यदि (_.first != _.second)
	{
	  subset_charset = सत्य;
	  भङ्ग;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.शीर्ष_Dict_2);
      तार्किक need_to_add_enc = (subset_encoding && !acc.शीर्ष_Dict_2.has_op (OpCode_Encoding));
      तार्किक need_to_add_set = (subset_charset && !acc.शीर्ष_Dict_2.has_op (OpCode_charset));
      यदि (need_to_add_enc || need_to_add_set)
      {
	यदि (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	यदि (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    यदि (acc.fdSelect != &Null (CFF1FDSelect))
    {
	यदि (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.अंक_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	प्रत्यागम असत्य;
    }
    अन्यथा
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      यदि (unlikely (!collect_sids_in_dicts (acc)))
	प्रत्यागम असत्य;
      यदि (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	प्रत्यागम असत्य;

      यदि (subset_charset && !plan_subset_charset (acc, plan))
        प्रत्यागम असत्य;

      topdict_mod.reassignSIDs (sidmap);
    }

    यदि (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ध्रुव OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      यदि (!flattener.flatten (subset_charstrings))
	प्रत्यागम असत्य;
    }
    अन्यथा
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      यदि (!subr_subsetter.subset ())
	प्रत्यागम असत्य;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      यदि (!subr_subsetter.encode_charstrings (subset_charstrings))
	प्रत्यागम असत्य;

      यदि (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	प्रत्यागम असत्य;

      /* local subrs */
      यदि (!subset_localsubrs.resize (orig_fdcount))
	प्रत्यागम असत्य;
      प्रति (unsigned पूर्णाङ्क fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	यदि (fdmap.has (fd))
	{
	  यदि (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    प्रत्यागम असत्य;
	}
      }
    }

    /* Encoding */
    यदि (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    यदि (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_मूल्यावली_mod_t ());
    अन्यथा
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ध्रुव cff1_font_dict_मूल्यावली_t &_)
	{ प्रत्यागम fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ध्रुव cff1_font_dict_मूल्यावली_t &_)
	{
	  cff1_font_dict_मूल्यावली_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  प्रत्यागम mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    प्रत्यागम !plan->in_error () &&
	   (subset_charstrings.length == plan->अंक_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_शीर्ष_dict_मूल्यावली_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned पूर्णाङ्क    अंक_glyphs;
  unsigned पूर्णाङ्क    orig_fdcount = 0;
  unsigned पूर्णाङ्क    subset_fdcount = 1;
  unsigned पूर्णाङ्क    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_मूल्यावली_mod_t>  fontdicts_mod;

  तार्किक		drop_hints = असत्य;

  तार्किक		gid_renum;
  तार्किक		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned पूर्णाङ्क	subset_enc_अंक_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  तार्किक		subset_charset;

  remap_sid_t	sidmap;
  unsigned पूर्णाङ्क	शीर्ष_Dict_Mod_SIDs[name_dict_मूल्यावली_t::Val_गिनती];

  तार्किक		desubroutinize = असत्य;
  तार्किक		identity_charset = असत्य;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

स्थिर तार्किक _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         संरचना OT::cff1_subset_plan &plan,
                                         ध्रुव OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  यदि (unlikely (!c->start_zerocopy (total_size)))
    प्रत्यागम असत्य;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  यदि (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->निकालो_discard ();
    प्रत्यागम असत्य;
  }

  plan.info.char_strings_link = c->निकालो_pack (असत्य);
  प्रत्यागम सत्य;
}

तार्किक
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   संरचना OT::cff1_subset_plan &plan) ध्रुव
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  यदि (!_serialize_cff1_charstrings(c, plan, *एतत्))
    प्रत्यागम असत्य;

  /* private dicts & local subrs */
  प्रति (पूर्णाङ्क i = (पूर्णाङ्क) privateDicts.length; --i >= 0 ;)
  {
    यदि (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      यदि (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	यदि (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->निकालो_pack ();
	अन्यथा
	{
	  c->निकालो_discard ();
	  प्रत्यागम असत्य;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      यदि (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->निकालो_pack ();
      }
      अन्यथा
      {
	c->निकालो_discard ();
	प्रत्यागम असत्य;
      }
    }
  }

  यदि (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  यदि (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    यदि (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->निकालो_pack (असत्य);
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  /* FDSelect */
  यदि (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    यदि (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs, *fdSelect, fd_गिनती,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  /* Charset */
  यदि (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    यदि (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.अंक_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  /* Encoding */
  यदि (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    यदि (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_अंक_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    यदि (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->निकालो_pack (असत्य);
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    यदि (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  यदि (unlikely (!cff))
    प्रत्यागम असत्य;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  यदि (unlikely (!c->embed (*nameIndex))) प्रत्यागम असत्य;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *शीर्ष_2 = c->push<शीर्ष_Dict> ();
    cff1_शीर्ष_dict_op_serializer_t शीर्ष_Szr;
    unsigned शीर्ष_size = 0;
    शीर्ष_dict_modifiers_t  modifier (plan.info, plan.शीर्ष_Dict_Mod_SIDs);
    यदि (likely (शीर्ष_2->serialize (c, plan.topdict_mod, शीर्ष_Szr, modifier)))
    {
      शीर्ष_size = c->length ();
      c->निकालो_pack (असत्य);
    }
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    प्रत्यागम dest->serialize_header (c, hb_iter (&शीर्ष_size, 1), शीर्ष_size);
  }
}

तार्किक
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ध्रुव
{
  cff1_subset_plan cff_plan;

  यदि (unlikely (!cff_plan.create (*एतत्, c->plan)))
  {
    DEBUG_संदेश(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    प्रत्यागम असत्य;
  }

  प्रत्यागम serialize (c->serializer, cff_plan);
}


#endif
