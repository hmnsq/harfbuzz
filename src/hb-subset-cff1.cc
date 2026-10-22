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
  अचिन्हित get_population () स्थिरांक { परत vector.length; }

  शून्य alloc (अचिन्हित size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  तार्किक in_error () स्थिरांक
  { परत map.in_error () || vector.in_error (); }

  अचिन्हित पूर्णांक add (अचिन्हित पूर्णांक sid)
  {
    जर (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      परत sid;

    sid = unoffset_sid (sid);
    अचिन्हित v = next;
    जर (map.set (sid, v, असत्य))
    {
      vector.push (sid);
      next++;
    }
    अन्यथा
      v = map.get (sid); // already exists
    परत offset_sid (v);
  }

  अचिन्हित पूर्णांक चालक[] (अचिन्हित पूर्णांक sid) स्थिरांक
  {
    जर (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      परत sid;

    परत offset_sid (map.get (unoffset_sid (sid)));
  }

  स्थिर स्थिरांक अचिन्हित पूर्णांक अंक_std_strings = 391;

  स्थिर तार्किक is_std_str (अचिन्हित पूर्णांक sid) { परत sid < अंक_std_strings; }
  स्थिर अचिन्हित पूर्णांक offset_sid (अचिन्हित पूर्णांक sid) { परत sid + अंक_std_strings; }
  स्थिर अचिन्हित पूर्णांक unoffset_sid (अचिन्हित पूर्णांक sid) { परत sid - अंक_std_strings; }
  अचिन्हित next = 0;

  hb_map_t map;
  hb_vector_t<अचिन्हित> vector;
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
संरचना cff1_शीर्ष_dict_मूल्ये_mod_t : cff1_शीर्ष_dict_मूल्ये_t
{
  शून्य init (स्थिरांक cff1_शीर्ष_dict_मूल्ये_t *base_= &Null (cff1_शीर्ष_dict_मूल्ये_t))
  {
    SUPER::init ();
    base = base_;
  }

  शून्य fini () { SUPER::fini (); }

  अचिन्हित get_मोजणी () स्थिरांक { परत base->get_मोजणी () + SUPER::get_मोजणी (); }
  स्थिरांक cff1_शीर्ष_dict_val_t &get_मूल्य (अचिन्हित पूर्णांक i) स्थिरांक
  {
    जर (i < base->get_मोजणी ())
      परत (*base)[i];
    अन्यथा
      परत SUPER::values[i - base->get_मोजणी ()];
  }
  स्थिरांक cff1_शीर्ष_dict_val_t &चालक [] (अचिन्हित पूर्णांक i) स्थिरांक { परत get_मूल्य (i); }

  शून्य reassignSIDs (स्थिरांक remap_sid_t& sidmap)
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्ये_t::Val_मोजणी; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  संरक्षित:
  प्रकारव्याख्या cff1_शीर्ष_dict_मूल्ये_t SUPER;
  स्थिरांक cff1_शीर्ष_dict_मूल्ये_t *base;
};

संरचना शीर्ष_dict_modifiers_t
{
  शीर्ष_dict_modifiers_t (स्थिरांक cff1_sub_table_info_t &info_,
			स्थिरांक अचिन्हित पूर्णांक (&nameSIDs_)[name_dict_मूल्ये_t::Val_मोजणी])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  स्थिरांक cff1_sub_table_info_t &info;
  स्थिरांक अचिन्हित पूर्णांक	(&nameSIDs)[name_dict_मूल्ये_t::Val_मोजणी];
};

संरचना cff1_शीर्ष_dict_op_serializer_t : cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>
{
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक cff1_शीर्ष_dict_val_t &opstr,
		  स्थिरांक शीर्ष_dict_modifiers_t &mod) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    op_code_t op = opstr.op;
    निवड (op)
    {
      स्थिती OpCode_charset:
	जर (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	अन्यथा
	  जा fall_back;

      स्थिती OpCode_Encoding:
	जर (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	अन्यथा
	  जा fall_back;

      स्थिती OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      स्थिती OpCode_version:
      स्थिती OpCode_Notice:
      स्थिती OpCode_Copyright:
      स्थिती OpCode_FullName:
      स्थिती OpCode_FamilyName:
      स्थिती OpCode_Weight:
      स्थिती OpCode_PostScript:
      स्थिती OpCode_BaseFontName:
      स्थिती OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_मूल्ये_t::name_op_to_index (op)]));

      स्थिती OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  जर ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (असत्य);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्ये_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्ये_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      पूर्वनिर्धारित:
	return_trace (cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (सत्य);
  }

};

संरचना cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक op_str_t &opstr,
		  स्थिरांक cff1_font_dict_मूल्ये_mod_t &mod) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    जर (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    अन्यथा
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  खाजगी:
  प्रकारव्याख्या cff_font_dict_op_serializer_t SUPER;
};

संरचना cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  स्थिर शून्य flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    जर (env.arg_start > 0)
      flush_width (env, param);

    निवड (op)
    {
      स्थिती OpCode_hstem:
      स्थिती OpCode_hstemhm:
      स्थिती OpCode_vstem:
      स्थिती OpCode_vstemhm:
      स्थिती OpCode_hintmask:
      स्थिती OpCode_cntrmask:
      स्थिती OpCode_dotsection:
	जर (param.drop_hints)
	{
	  env.clear_args ();
	  परत;
	}
	HB_FALLTHROUGH;

      पूर्वनिर्धारित:
	SUPER::flush_args_and_op (op, env, param);
	थांबा;
    }
  }
  स्थिर शून्य flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    साठी (अचिन्हित पूर्णांक i = env.arg_start; i < env.arg_चळत.get_मोजणी (); i++)
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
    जर (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      साठी (अचिन्हित पूर्णांक i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  खाजगी:
  प्रकारव्याख्या cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

संरचना range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  तार्किक complete (अचिन्हित पूर्णांक last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    अचिन्हित मोजणी = हे->length;
    साठी (अचिन्हित पूर्णांक i = मोजणी; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      अचिन्हित पूर्णांक n_डावा = last_glyph - pair.glyph - 1;
      all_glyphs |= n_डावा;
      last_glyph = pair.glyph;
      pair.glyph = n_डावा;
    }
    तार्किक two_byte = all_glyphs >= 0x100;
    परत two_byte;
  }
};

संरचना cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  स्थिर शून्य process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    निवड (op) {

      स्थिती OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, असत्य);
	थांबा;

      स्थिती OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	थांबा;

      स्थिती OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	थांबा;

      स्थिती OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	थांबा;
    }
  }

  संरक्षित:
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

  खाजगी:
  प्रकारव्याख्या cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

संरचना cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (तार्किक desubroutinize_, तार्किक drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक op_str_t &opstr,
		  objidx_t subrs_link) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    जर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (सत्य);

    जर (opstr.op == OpCode_Subrs)
    {
      जर (desubroutinize || !subrs_link)
	return_trace (सत्य);
      अन्यथा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  संरक्षित:
  स्थिरांक तार्किक desubroutinize;
  स्थिरांक तार्किक drop_hints;
};

संरचना cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, स्थिरांक OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (स्थिरांक OT::cff1::accelerator_subset_t &acc_, स्थिरांक hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  स्थिर शून्य complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    जर (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    साठी (अचिन्हित पूर्णांक i = 0; i < env.call_चळत.get_मोजणी (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_चळत[i]);
      जर (likely (parsed_str))
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
    साठी (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्ये_t::Val_मोजणी; i++)
      शीर्ष_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  शून्य plan_subset_encoding (स्थिरांक OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    स्थिरांक Encoding *encoding = acc.encoding;
    अचिन्हित पूर्णांक  size0, size1;
    अचिन्हित code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    जर (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (असत्य);
      परत;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_अंक_codes = plan->अंक_output_glyphs () - 1;
    अचिन्हित पूर्णांक glyph;
    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    जर (it->first == 0) it++;
    स्वतः _ = *it;
    साठी (glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      जर (glyph == _.first)
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
      जर (code == CFF_UNDEF_CODE)
      {
	subset_enc_अंक_codes = glyph - 1;
	थांबा;
      }

      जर (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      जर (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	साठी (अचिन्हित पूर्णांक i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    प्रतिज्ञा (subset_enc_अंक_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_अंक_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    जर (size0 < size1)
      subset_enc_format = 0;
    अन्यथा
      subset_enc_format = 1;
  }

  तार्किक plan_subset_charset (स्थिरांक OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    अचिन्हित पूर्णांक  size0, size_ranges;
    अचिन्हित last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    जर (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (असत्य);
      परत असत्य;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    अचिन्हित अंक_glyphs = plan->अंक_output_glyphs ();

    जर (unlikely (!subset_charset_ranges.alloc (hb_min (अंक_glyphs,
							acc.अंक_charset_entries))))
    {
      plan->check_success (असत्य);
      परत असत्य;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    तार्किक created_map = असत्य;
    जर (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = सत्य;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    जर (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = असत्य;
    }

    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    जर (it->first == 0) it++;
    स्वतः _ = *it;
    तार्किक not_is_cid = !acc.is_CID ();
    तार्किक skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    जर (not_is_cid)
      sidmap.alloc (अंक_glyphs);
    साठी (hb_codepoint_t glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      जर (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      अन्यथा
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      अचिन्हित sid;
      जर (identity_charset)
	sid = glyph;
      अन्यथा
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	जर (not_is_cid)
	  sid = sidmap.add (sid);
      }

      जर (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      जर (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    जर (created_map)
    {
      जर ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    तार्किक two_byte = subset_charset_ranges.complete (अंक_glyphs);

    size0 = Charset0::get_size (plan->अंक_output_glyphs ());
    जर (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    अन्यथा
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    जर (size0 < size_ranges)
      subset_charset_format = 0;
    अन्यथा जर (!two_byte)
      subset_charset_format = 1;
    अन्यथा
      subset_charset_format = 2;

    परत सत्य;
  }

  तार्किक collect_sids_in_dicts (स्थिरांक OT::cff1::accelerator_subset_t &acc)
  {
    साठी (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्ये_t::Val_मोजणी; i++)
    {
      अचिन्हित पूर्णांक sid = acc.शीर्ष_Dict_2.nameSIDs[i];
      जर (sid != CFF_UNDEF_SID)
      {
	शीर्ष_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    जर (acc.fdArray != &Null (CFF1FDArray))
      साठी (अचिन्हित पूर्णांक i = 0; i < orig_fdcount; i++)
	जर (fdmap.has (i))
	  (शून्य)sidmap.add (acc.fontDicts[i].fontName);

    परत सत्य;
  }

  तार्किक create (स्थिरांक OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    जर (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) परत असत्य;

    अंक_glyphs = plan->अंक_output_glyphs ();
    orig_fdcount = acc.fd_मोजणी;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अन्यथा
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    जर (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      साठी (स्थिरांक स्वतः &_ : plan->new_to_old_gid_list)
      {
	जर (_.first != _.second)
	{
	  subset_charset = सत्य;
	  थांबा;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.शीर्ष_Dict_2);
      तार्किक need_to_add_enc = (subset_encoding && !acc.शीर्ष_Dict_2.has_op (OpCode_Encoding));
      तार्किक need_to_add_set = (subset_charset && !acc.शीर्ष_Dict_2.has_op (OpCode_charset));
      जर (need_to_add_enc || need_to_add_set)
      {
	जर (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	जर (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    जर (acc.fdSelect != &Null (CFF1FDSelect))
    {
	जर (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.अंक_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	परत असत्य;
    }
    अन्यथा
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      जर (unlikely (!collect_sids_in_dicts (acc)))
	परत असत्य;
      जर (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	परत असत्य;

      जर (subset_charset && !plan_subset_charset (acc, plan))
        परत असत्य;

      topdict_mod.reassignSIDs (sidmap);
    }

    जर (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<स्थिरांक OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      जर (!flattener.flatten (subset_charstrings))
	परत असत्य;
    }
    अन्यथा
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      जर (!subr_subsetter.subset ())
	परत असत्य;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      जर (!subr_subsetter.encode_charstrings (subset_charstrings))
	परत असत्य;

      जर (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	परत असत्य;

      /* local subrs */
      जर (!subset_localsubrs.resize (orig_fdcount))
	परत असत्य;
      साठी (अचिन्हित पूर्णांक fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	जर (fdmap.has (fd))
	{
	  जर (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    परत असत्य;
	}
      }
    }

    /* Encoding */
    जर (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    जर (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_मूल्ये_mod_t ());
    अन्यथा
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (स्थिरांक cff1_font_dict_मूल्ये_t &_)
	{ परत fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (स्थिरांक cff1_font_dict_मूल्ये_t &_)
	{
	  cff1_font_dict_मूल्ये_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  परत mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    परत !plan->in_error () &&
	   (subset_charstrings.length == plan->अंक_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_शीर्ष_dict_मूल्ये_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  अचिन्हित पूर्णांक    अंक_glyphs;
  अचिन्हित पूर्णांक    orig_fdcount = 0;
  अचिन्हित पूर्णांक    subset_fdcount = 1;
  अचिन्हित पूर्णांक    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_मूल्ये_mod_t>  fontdicts_mod;

  तार्किक		drop_hints = असत्य;

  तार्किक		gid_renum;
  तार्किक		subset_encoding;
  uint8_t	subset_enc_format;
  अचिन्हित पूर्णांक	subset_enc_अंक_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  तार्किक		subset_charset;

  remap_sid_t	sidmap;
  अचिन्हित पूर्णांक	शीर्ष_Dict_Mod_SIDs[name_dict_मूल्ये_t::Val_मोजणी];

  तार्किक		desubroutinize = असत्य;
  तार्किक		identity_charset = असत्य;

  अचिन्हित	min_charstrings_off_size = 0;
};
} // namespace OT

स्थिर तार्किक _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         संरचना OT::cff1_subset_plan &plan,
                                         स्थिरांक OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  अचिन्हित data_size = 0;
  अचिन्हित total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  जर (unlikely (!c->start_zerocopy (total_size)))
    परत असत्य;

  स्वतः *cs = c->start_embed<CFF1CharStrings> ();
  जर (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->काढा_discard ();
    परत असत्य;
  }

  plan.info.char_strings_link = c->काढा_pack (असत्य);
  परत सत्य;
}

तार्किक
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   संरचना OT::cff1_subset_plan &plan) स्थिरांक
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  जर (!_serialize_cff1_charstrings(c, plan, *हे))
    परत असत्य;

  /* private dicts & local subrs */
  साठी (पूर्णांक i = (पूर्णांक) privateDicts.length; --i >= 0 ;)
  {
    जर (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      जर (plan.subset_localsubrs[i].length > 0)
      {
	स्वतः *dest = c->push <CFF1Subrs> ();
	जर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->काढा_pack ();
	अन्यथा
	{
	  c->काढा_discard ();
	  परत असत्य;
	}
      }

      स्वतः *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      जर (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	अचिन्हित fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->काढा_pack ();
      }
      अन्यथा
      {
	c->काढा_discard ();
	परत असत्य;
      }
    }
  }

  जर (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  जर (fdArray != &Null (CFF1FDArray))
  {
    स्वतः *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    स्वतः it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    जर (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->काढा_pack (असत्य);
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* FDSelect */
  जर (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    जर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs, *fdSelect, fd_मोजणी,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* Charset */
  जर (plan.subset_charset)
  {
    स्वतः *dest = c->push<Charset> ();
    जर (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.अंक_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* Encoding */
  जर (plan.subset_encoding)
  {
    स्वतः *dest = c->push<Encoding> ();
    जर (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_अंक_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* global subrs */
  {
    स्वतः *dest = c->push <CFF1Subrs> ();
    जर (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->काढा_pack (असत्य);
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  /* String INDEX */
  {
    स्वतः *dest = c->push<CFF1StringIndex> ();
    जर (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->काढा_pack ();
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  जर (unlikely (!cff))
    परत असत्य;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  जर (unlikely (!c->embed (*nameIndex))) परत असत्य;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    स्वतः *शीर्ष_2 = c->push<शीर्ष_Dict> ();
    cff1_शीर्ष_dict_op_serializer_t शीर्ष_Szr;
    अचिन्हित शीर्ष_size = 0;
    शीर्ष_dict_modifiers_t  modifier (plan.info, plan.शीर्ष_Dict_Mod_SIDs);
    जर (likely (शीर्ष_2->serialize (c, plan.topdict_mod, शीर्ष_Szr, modifier)))
    {
      शीर्ष_size = c->length ();
      c->काढा_pack (असत्य);
    }
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
    /* serialize INDEX header for above */
    स्वतः *dest = c->start_embed<CFF1Index> ();
    परत dest->serialize_header (c, hb_iter (&शीर्ष_size, 1), शीर्ष_size);
  }
}

तार्किक
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) स्थिरांक
{
  cff1_subset_plan cff_plan;

  जर (unlikely (!cff_plan.create (*हे, c->plan)))
  {
    DEBUG_संदेश(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    परत असत्य;
  }

  परत serialize (c->serializer, cff_plan);
}


#endif
