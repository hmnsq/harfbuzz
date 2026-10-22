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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_SUBSET_CFF

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-cff1-table.hh"
#समावेश "hb-set.h"
#समावेश "hb-bimap.hh"
#समावेश "hb-subset-plan.hh"
#समावेश "hb-subset-cff-common.hh"
#समावेश "hb-cff1-interp-cs.hh"

using namespace CFF;

काष्ठा remap_sid_t
{
  अचिन्हित get_population () स्थिर { वापस vector.लंबाई; }

  व्योम alloc (अचिन्हित आकार)
  {
    map.alloc (आकार);
    vector.alloc_exact (आकार);
  }

  bool in_error () स्थिर
  { वापस map.in_error () || vector.in_error (); }

  अचिन्हित पूर्णांक add (अचिन्हित पूर्णांक sid)
  {
    अगर (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      वापस sid;

    sid = unoffset_sid (sid);
    अचिन्हित v = next;
    अगर (map.set (sid, v, false))
    {
      vector.push (sid);
      next++;
    }
    अन्यथा
      v = map.लो (sid); // already exists
    वापस offset_sid (v);
  }

  अचिन्हित पूर्णांक चालक[] (अचिन्हित पूर्णांक sid) स्थिर
  {
    अगर (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      वापस sid;

    वापस offset_sid (map.लो (unoffset_sid (sid)));
  }

  अटल स्थिर अचिन्हित पूर्णांक अंक_std_strings = 391;

  अटल bool is_std_str (अचिन्हित पूर्णांक sid) { वापस sid < अंक_std_strings; }
  अटल अचिन्हित पूर्णांक offset_sid (अचिन्हित पूर्णांक sid) { वापस sid + अंक_std_strings; }
  अटल अचिन्हित पूर्णांक unoffset_sid (अचिन्हित पूर्णांक sid) { वापस sid - अंक_std_strings; }
  अचिन्हित next = 0;

  hb_map_t map;
  hb_vector_t<अचिन्हित> vector;
};

काष्ठा cff1_sub_table_info_t : cff_sub_table_info_t
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
काष्ठा cff1_शीर्ष_dict_मूल्यावली_mod_t : cff1_शीर्ष_dict_मूल्यावली_t
{
  व्योम init (स्थिर cff1_शीर्ष_dict_मूल्यावली_t *base_= &Null (cff1_शीर्ष_dict_मूल्यावली_t))
  {
    SUPER::init ();
    base = base_;
  }

  व्योम fini () { SUPER::fini (); }

  अचिन्हित get_गिनती () स्थिर { वापस base->get_गिनती () + SUPER::get_गिनती (); }
  स्थिर cff1_शीर्ष_dict_val_t &get_मूल्य (अचिन्हित पूर्णांक i) स्थिर
  {
    अगर (i < base->get_गिनती ())
      वापस (*base)[i];
    अन्यथा
      वापस SUPER::values[i - base->get_गिनती ()];
  }
  स्थिर cff1_शीर्ष_dict_val_t &चालक [] (अचिन्हित पूर्णांक i) स्थिर { वापस get_मूल्य (i); }

  व्योम reassignSIDs (स्थिर remap_sid_t& sidmap)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  रक्षित:
  प्रकार cff1_शीर्ष_dict_मूल्यावली_t SUPER;
  स्थिर cff1_शीर्ष_dict_मूल्यावली_t *base;
};

काष्ठा शीर्ष_dict_modifiers_t
{
  शीर्ष_dict_modifiers_t (स्थिर cff1_sub_table_info_t &info_,
			स्थिर अचिन्हित पूर्णांक (&nameSIDs_)[name_dict_मूल्यावली_t::Val_गिनती])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  स्थिर cff1_sub_table_info_t &info;
  स्थिर अचिन्हित पूर्णांक	(&nameSIDs)[name_dict_मूल्यावली_t::Val_गिनती];
};

काष्ठा cff1_शीर्ष_dict_op_serializer_t : cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर cff1_शीर्ष_dict_val_t &opstr,
		  स्थिर शीर्ष_dict_modifiers_t &mod) स्थिर
  {
    TRACE_SERIALIZE (यह);

    op_code_t op = opstr.op;
    चयन (op)
    {
      हाल OpCode_charset:
	अगर (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	अन्यथा
	  जाओ fall_back;

      हाल OpCode_Encoding:
	अगर (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	अन्यथा
	  जाओ fall_back;

      हाल OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.आकार) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      हाल OpCode_version:
      हाल OpCode_Notice:
      हाल OpCode_Copyright:
      हाल OpCode_FullName:
      हाल OpCode_FamilyName:
      हाल OpCode_Weight:
      हाल OpCode_PostScript:
      हाल OpCode_BaseFontName:
      हाल OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_मूल्यावली_t::name_op_to_सूचकांक (op)]));

      हाल OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  अगर ( unlikely (!(opstr.लंबाई >= opstr.last_arg_offset + 3)))
	    return_trace (false);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.लंबाई = opstr.लंबाई - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्यावली_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_मूल्यावली_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      शेष:
	return_trace (cff_शीर्ष_dict_op_serializer_t<cff1_शीर्ष_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (true);
  }

};

काष्ठा cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर op_str_t &opstr,
		  स्थिर cff1_font_dict_मूल्यावली_mod_t &mod) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अगर (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    अन्यथा
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  निजी:
  प्रकार cff_font_dict_op_serializer_t SUPER;
};

काष्ठा cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  अटल व्योम flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    अगर (env.arg_start > 0)
      flush_width (env, param);

    चयन (op)
    {
      हाल OpCode_hstem:
      हाल OpCode_hstemhm:
      हाल OpCode_vstem:
      हाल OpCode_vstemhm:
      हाल OpCode_hintmask:
      हाल OpCode_cntrmask:
      हाल OpCode_dotsection:
	अगर (param.drop_hints)
	{
	  env.clear_args ();
	  वापस;
	}
	HB_FALLTHROUGH;

      शेष:
	SUPER::flush_args_and_op (op, env, param);
	अवरोध;
    }
  }
  अटल व्योम flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    क्रम (अचिन्हित पूर्णांक i = env.arg_start; i < env.arg_ढेर.get_गिनती (); i++)
      encoder.encode_अंक_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  अटल व्योम flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  अटल व्योम flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    निश्चित (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_अंक_cs (env.width);
  }

  अटल व्योम flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    अगर (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      क्रम (अचिन्हित पूर्णांक i = 0; i < env.hintmask_आकार; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  निजी:
  प्रकार cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

काष्ठा range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  bool complete (अचिन्हित पूर्णांक last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    अचिन्हित गिनती = यह->लंबाई;
    क्रम (अचिन्हित पूर्णांक i = गिनती; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      अचिन्हित पूर्णांक n_बायाँ = last_glyph - pair.glyph - 1;
      all_glyphs |= n_बायाँ;
      last_glyph = pair.glyph;
      pair.glyph = n_बायाँ;
    }
    bool two_byte = all_glyphs >= 0x100;
    वापस two_byte;
  }
};

काष्ठा cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  अटल व्योम process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    चयन (op) {

      हाल OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, false);
	अवरोध;

      हाल OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	अवरोध;

      हाल OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	अवरोध;

      हाल OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	अवरोध;

      शेष:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	अवरोध;
    }
  }

  रक्षित:
  अटल व्योम process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_अंक_2);
    closure->add (env.context.subr_अंक_2);
    param.set_current_str (env, true);
  }

  निजी:
  प्रकार cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

काष्ठा cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (bool desubroutinize_, bool drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  bool serialize (hb_serialize_context_t *c,
		  स्थिर op_str_t &opstr,
		  objidx_t subrs_link) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अगर (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    अगर (opstr.op == OpCode_Subrs)
    {
      अगर (desubroutinize || !subrs_link)
	return_trace (true);
      अन्यथा
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  रक्षित:
  स्थिर bool desubroutinize;
  स्थिर bool drop_hints;
};

काष्ठा cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, स्थिर OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (स्थिर OT::cff1::accelerator_subset_t &acc_, स्थिर hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  अटल व्योम complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    अगर (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < env.call_ढेर.get_गिनती (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ढेर[i]);
      अगर (likely (parsed_str))
	parsed_str->set_parsed ();
      अन्यथा
	env.set_error ();
    }
  }
};

namespace OT {
काष्ठा cff1_subset_plan
{
  cff1_subset_plan ()
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
      शीर्ष_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  व्योम plan_subset_encoding (स्थिर OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    स्थिर Encoding *encoding = acc.encoding;
    अचिन्हित पूर्णांक  size0, size1;
    अचिन्हित code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.सफा ();
    अगर (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (false);
      वापस;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_अंक_codes = plan->अंक_output_glyphs () - 1;
    अचिन्हित पूर्णांक glyph;
    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    अगर (it->first == 0) it++;
    स्वतः _ = *it;
    क्रम (glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      अगर (glyph == _.first)
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
      अगर (code == CFF_UNDEF_CODE)
      {
	subset_enc_अंक_codes = glyph - 1;
	अवरोध;
      }

      अगर (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      अगर (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	क्रम (अचिन्हित पूर्णांक i = 0; i < supp_codes.लंबाई; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    निश्चित (subset_enc_अंक_codes <= 0xFF);
    size0 = Encoding0::min_आकार + HBUINT8::static_आकार * subset_enc_अंक_codes;
    size1 = Encoding1::min_आकार + Encoding1_Range::static_आकार * subset_enc_code_ranges.लंबाई;

    अगर (size0 < size1)
      subset_enc_format = 0;
    अन्यथा
      subset_enc_format = 1;
  }

  bool plan_subset_charset (स्थिर OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    अचिन्हित पूर्णांक  size0, आकार_ranges;
    अचिन्हित last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.सफा ();
    अगर (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (false);
      वापस false;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    अचिन्हित अंक_glyphs = plan->अंक_output_glyphs ();

    अगर (unlikely (!subset_charset_ranges.alloc (hb_min (अंक_glyphs,
							acc.अंक_charset_entries))))
    {
      plan->check_success (false);
      वापस false;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    bool created_map = false;
    अगर (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = true;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    अगर (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = false;
    }

    स्वतः it = hb_iter (plan->new_to_old_gid_list);
    अगर (it->first == 0) it++;
    स्वतः _ = *it;
    bool not_is_cid = !acc.is_CID ();
    bool skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    अगर (not_is_cid)
      sidmap.alloc (अंक_glyphs);
    क्रम (hb_codepoint_t glyph = 1; glyph < अंक_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      अगर (glyph == _.first)
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
      अगर (identity_charset)
	sid = glyph;
      अन्यथा
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	अगर (not_is_cid)
	  sid = sidmap.add (sid);
      }

      अगर (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      अगर (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    अगर (created_map)
    {
      अगर ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    bool two_byte = subset_charset_ranges.complete (अंक_glyphs);

    size0 = Charset0::get_आकार (plan->अंक_output_glyphs ());
    अगर (!two_byte)
      आकार_ranges = Charset1::get_आकार_for_ranges (subset_charset_ranges.लंबाई);
    अन्यथा
      आकार_ranges = Charset2::get_आकार_for_ranges (subset_charset_ranges.लंबाई);

    अगर (size0 < आकार_ranges)
      subset_charset_format = 0;
    अन्यथा अगर (!two_byte)
      subset_charset_format = 1;
    अन्यथा
      subset_charset_format = 2;

    वापस true;
  }

  bool collect_sids_in_dicts (स्थिर OT::cff1::accelerator_subset_t &acc)
  {
    क्रम (अचिन्हित पूर्णांक i = 0; i < name_dict_मूल्यावली_t::Val_गिनती; i++)
    {
      अचिन्हित पूर्णांक sid = acc.शीर्ष_Dict_2.nameSIDs[i];
      अगर (sid != CFF_UNDEF_SID)
      {
	शीर्ष_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    अगर (acc.fdArray != &Null (CFF1FDArray))
      क्रम (अचिन्हित पूर्णांक i = 0; i < orig_fdcount; i++)
	अगर (fdmap.has (i))
	  (व्योम)sidmap.add (acc.fontDicts[i].fontName);

    वापस true;
  }

  bool create (स्थिर OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    अगर (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) वापस false;

    अंक_glyphs = plan->अंक_output_glyphs ();
    orig_fdcount = acc.fd_गिनती;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #अगर_घोषित HB_EXPERIMENTAL_API
    min_charstrings_off_आकार = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #अन्यथा
    min_charstrings_off_आकार = 0;
 #पूर्ण_अगर

    subset_charset = !acc.is_predef_charset () || identity_charset;
    अगर (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      क्रम (स्थिर स्वतः &_ : plan->new_to_old_gid_list)
      {
	अगर (_.first != _.second)
	{
	  subset_charset = true;
	  अवरोध;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.शीर्ष_Dict_2);
      bool need_to_add_enc = (subset_encoding && !acc.शीर्ष_Dict_2.has_op (OpCode_Encoding));
      bool need_to_add_set = (subset_charset && !acc.शीर्ष_Dict_2.has_op (OpCode_charset));
      अगर (need_to_add_enc || need_to_add_set)
      {
	अगर (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	अगर (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    अगर (acc.fdSelect != &Null (CFF1FDSelect))
    {
	अगर (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.अंक_glyphs,
				  subset_fdcount,
				  info.fd_select.आकार,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	वापस false;
    }
    अन्यथा
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      अगर (unlikely (!collect_sids_in_dicts (acc)))
	वापस false;
      अगर (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	वापस false;

      अगर (subset_charset && !plan_subset_charset (acc, plan))
        वापस false;

      topdict_mod.reassignSIDs (sidmap);
    }

    अगर (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<स्थिर OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      अगर (!flattener.flatten (subset_charstrings))
	वापस false;
    }
    अन्यथा
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      अगर (!subr_subsetter.subset ())
	वापस false;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      अगर (!subr_subsetter.encode_charstrings (subset_charstrings))
	वापस false;

      अगर (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	वापस false;

      /* local subrs */
      अगर (!subset_localsubrs.resize (orig_fdcount))
	वापस false;
      क्रम (अचिन्हित पूर्णांक fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	अगर (fdmap.has (fd))
	{
	  अगर (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    वापस false;
	}
      }
    }

    /* Encoding */
    अगर (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    अगर (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_मूल्यावली_mod_t ());
    अन्यथा
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (स्थिर cff1_font_dict_मूल्यावली_t &_)
	{ वापस fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (स्थिर cff1_font_dict_मूल्यावली_t &_)
	{
	  cff1_font_dict_मूल्यावली_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  वापस mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    वापस !plan->in_error () &&
	   (subset_charstrings.लंबाई == plan->अंक_output_glyphs ()) &&
	   (fontdicts_mod.लंबाई == subset_fdcount);
  }

  cff1_शीर्ष_dict_मूल्यावली_mod_t	topdict_mod;
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
  hb_vector_t<cff1_font_dict_मूल्यावली_mod_t>  fontdicts_mod;

  bool		drop_hints = false;

  bool		gid_renum;
  bool		subset_encoding;
  uint8_t	subset_enc_format;
  अचिन्हित पूर्णांक	subset_enc_अंक_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  bool		subset_charset;

  remap_sid_t	sidmap;
  अचिन्हित पूर्णांक	शीर्ष_Dict_Mod_SIDs[name_dict_मूल्यावली_t::Val_गिनती];

  bool		desubroutinize = false;
  bool		identity_charset = false;

  अचिन्हित	min_charstrings_off_आकार = 0;
};
} // namespace OT

अटल bool _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         काष्ठा OT::cff1_subset_plan &plan,
                                         स्थिर OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  अचिन्हित तथ्य_आकार = 0;
  अचिन्हित योग_आकार = CFF1CharStrings::योग_आकार (plan.subset_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार);
  अगर (unlikely (!c->start_zerocopy (योग_आकार)))
    वापस false;

  स्वतः *cs = c->start_embed<CFF1CharStrings> ();
  अगर (unlikely (!cs->serialize (c, plan.subset_charstrings, &तथ्य_आकार, plan.min_charstrings_off_आकार))) {
    c->निकालो_discard ();
    वापस false;
  }

  plan.info.char_strings_link = c->निकालो_pack (false);
  वापस true;
}

bool
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   काष्ठा OT::cff1_subset_plan &plan) स्थिर
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  अगर (!_serialize_cff1_charstrings(c, plan, *यह))
    वापस false;

  /* private dicts & local subrs */
  क्रम (पूर्णांक i = (पूर्णांक) privateDicts.लंबाई; --i >= 0 ;)
  {
    अगर (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      अगर (plan.subset_localsubrs[i].लंबाई > 0)
      {
	स्वतः *dest = c->push <CFF1Subrs> ();
	अगर (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->निकालो_pack ();
	अन्यथा
	{
	  c->निकालो_discard ();
	  वापस false;
	}
      }

      स्वतः *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      अगर (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	अचिन्हित fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.आकार = c->लंबाई ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->निकालो_pack ();
      }
      अन्यथा
      {
	c->निकालो_discard ();
	वापस false;
      }
    }
  }

  अगर (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  अगर (fdArray != &Null (CFF1FDArray))
  {
    स्वतः *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    स्वतः it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    अगर (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->निकालो_pack (false);
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* FDSelect */
  अगर (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    अगर (likely (hb_serialize_cff_fdselect (c, plan.अंक_glyphs, *fdSelect, fd_गिनती,
					   plan.subset_fdselect_format, plan.info.fd_select.आकार,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* Charset */
  अगर (plan.subset_charset)
  {
    स्वतः *dest = c->push<Charset> ();
    अगर (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.अंक_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* Encoding */
  अगर (plan.subset_encoding)
  {
    स्वतः *dest = c->push<Encoding> ();
    अगर (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_अंक_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* global subrs */
  {
    स्वतः *dest = c->push <CFF1Subrs> ();
    अगर (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->निकालो_pack (false);
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  /* String INDEX */
  {
    स्वतः *dest = c->push<CFF1_String_सूचकांक> ();
    अगर (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *string_सूचकांक, plan.sidmap.vector)))
      c->निकालो_pack ();
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  अगर (unlikely (!cff))
    वापस false;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->name_सूचकांक = cff->min_आकार;
  cff->off_आकार = 4; /* unused? */

  /* name INDEX */
  अगर (unlikely (!c->embed (*name_सूचकांक))) वापस false;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    स्वतः *शीर्ष_4 = c->push<शीर्ष_Dict> ();
    cff1_शीर्ष_dict_op_serializer_t शीर्ष_Szr;
    अचिन्हित शीर्ष_आकार = 0;
    शीर्ष_dict_modifiers_t  modifier (plan.info, plan.शीर्ष_Dict_Mod_SIDs);
    अगर (likely (शीर्ष_4->serialize (c, plan.topdict_mod, शीर्ष_Szr, modifier)))
    {
      शीर्ष_आकार = c->लंबाई ();
      c->निकालो_pack (false);
    }
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
    /* serialize INDEX header for above */
    स्वतः *dest = c->start_embed<CFF1_सूचकांक> ();
    वापस dest->serialize_header (c, hb_iter (&शीर्ष_आकार, 1), शीर्ष_आकार);
  }
}

bool
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) स्थिर
{
  cff1_subset_plan cff_plan;

  अगर (unlikely (!cff_plan.create (*यह, c->plan)))
  {
    DEBUG_संदेश(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    वापस false;
  }

  वापस serialize (c->serializer, cff_plan);
}


#पूर्ण_अगर
