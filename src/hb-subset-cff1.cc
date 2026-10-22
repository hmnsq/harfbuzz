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

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-ot-cff1-table.hh"
#અન્તર્ભાવ "hb-set.h"
#અન્તર્ભાવ "hb-bimap.hh"
#અન્તર્ભાવ "hb-subset-plan.hh"
#અન્તર્ભાવ "hb-subset-cff-common.hh"
#અન્તર્ભાવ "hb-cff1-interp-cs.hh"

using નામાકાશ CFF;

સંરચના remap_sid_t
{
  unsigned get_population () અચલ { પાછા vector.length; }

  શૂન્ય alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  તાર્કિક in_error () અચલ
  { પાછા map.in_error () || vector.in_error (); }

  unsigned પૂર્ણાંક add (unsigned પૂર્ણાંક sid)
  {
    જો (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      પાછા sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    જો (map.set (sid, v, ખોટું))
    {
      vector.push (sid);
      next++;
    }
    નહીંતર
      v = map.get (sid); // already exists
    પાછા offset_sid (v);
  }

  unsigned પૂર્ણાંક operator[] (unsigned પૂર્ણાંક sid) અચલ
  {
    જો (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      પાછા sid;

    પાછા offset_sid (map.get (unoffset_sid (sid)));
  }

  સ્થિર અચલ unsigned પૂર્ણાંક અંક_std_strings = 391;

  સ્થિર તાર્કિક is_std_str (unsigned પૂર્ણાંક sid) { પાછા sid < અંક_std_strings; }
  સ્થિર unsigned પૂર્ણાંક offset_sid (unsigned પૂર્ણાંક sid) { પાછા sid + અંક_std_strings; }
  સ્થિર unsigned પૂર્ણાંક unoffset_sid (unsigned પૂર્ણાંક sid) { પાછા sid - અંક_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

સંરચના cff1_sub_table_info_t : cff_sub_table_info_t
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
સંરચના cff1_ટોચ_dict_કિંમતો_mod_t : cff1_ટોચ_dict_કિંમતો_t
{
  શૂન્ય init (અચલ cff1_ટોચ_dict_કિંમતો_t *base_= &Null (cff1_ટોચ_dict_કિંમતો_t))
  {
    SUPER::init ();
    base = base_;
  }

  શૂન્ય fini () { SUPER::fini (); }

  unsigned get_ગણતરી () અચલ { પાછા base->get_ગણતરી () + SUPER::get_ગણતરી (); }
  અચલ cff1_ટોચ_dict_val_t &get_કિંમત (unsigned પૂર્ણાંક i) અચલ
  {
    જો (i < base->get_ગણતરી ())
      પાછા (*base)[i];
    નહીંતર
      પાછા SUPER::values[i - base->get_ગણતરી ()];
  }
  અચલ cff1_ટોચ_dict_val_t &operator [] (unsigned પૂર્ણાંક i) અચલ { પાછા get_કિંમત (i); }

  શૂન્ય reassignSIDs (અચલ remap_sid_t& sidmap)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < name_dict_કિંમતો_t::Val_ગણતરી; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  સુરક્ષિત:
  પ્રકારવ્યાખ્યા cff1_ટોચ_dict_કિંમતો_t SUPER;
  અચલ cff1_ટોચ_dict_કિંમતો_t *base;
};

સંરચના ટોચ_dict_modifiers_t
{
  ટોચ_dict_modifiers_t (અચલ cff1_sub_table_info_t &info_,
			અચલ unsigned પૂર્ણાંક (&nameSIDs_)[name_dict_કિંમતો_t::Val_ગણતરી])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  અચલ cff1_sub_table_info_t &info;
  અચલ unsigned પૂર્ણાંક	(&nameSIDs)[name_dict_કિંમતો_t::Val_ગણતરી];
};

સંરચના cff1_ટોચ_dict_op_serializer_t : cff_ટોચ_dict_op_serializer_t<cff1_ટોચ_dict_val_t>
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ cff1_ટોચ_dict_val_t &opstr,
		  અચલ ટોચ_dict_modifiers_t &mod) અચલ
  {
    TRACE_SERIALIZE (આ);

    op_code_t op = opstr.op;
    પસંદગી (op)
    {
      સ્થિતિ OpCode_charset:
	જો (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	નહીંતર
	  જાઓ fall_back;

      સ્થિતિ OpCode_Encoding:
	જો (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	નહીંતર
	  જાઓ fall_back;

      સ્થિતિ OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      સ્થિતિ OpCode_version:
      સ્થિતિ OpCode_Notice:
      સ્થિતિ OpCode_Copyright:
      સ્થિતિ OpCode_FullName:
      સ્થિતિ OpCode_FamilyName:
      સ્થિતિ OpCode_Weight:
      સ્થિતિ OpCode_PostScript:
      સ્થિતિ OpCode_BaseFontName:
      સ્થિતિ OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_કિંમતો_t::name_op_to_index (op)]));

      સ્થિતિ OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  જો ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (ખોટું);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_કિંમતો_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_કિંમતો_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      મૂળભૂત:
	return_trace (cff_ટોચ_dict_op_serializer_t<cff1_ટોચ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (સાચું);
  }

};

સંરચના cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ op_str_t &opstr,
		  અચલ cff1_font_dict_કિંમતો_mod_t &mod) અચલ
  {
    TRACE_SERIALIZE (આ);

    જો (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    નહીંતર
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff_font_dict_op_serializer_t SUPER;
};

સંરચના cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  સ્થિર શૂન્ય flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    જો (env.arg_start > 0)
      flush_width (env, param);

    પસંદગી (op)
    {
      સ્થિતિ OpCode_hstem:
      સ્થિતિ OpCode_hstemhm:
      સ્થિતિ OpCode_vstem:
      સ્થિતિ OpCode_vstemhm:
      સ્થિતિ OpCode_hintmask:
      સ્થિતિ OpCode_cntrmask:
      સ્થિતિ OpCode_dotsection:
	જો (param.drop_hints)
	{
	  env.clear_args ();
	  પાછા;
	}
	HB_FALLTHROUGH;

      મૂળભૂત:
	SUPER::flush_args_and_op (op, env, param);
	તોડો;
    }
  }
  સ્થિર શૂન્ય flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    માટે (unsigned પૂર્ણાંક i = env.arg_start; i < env.arg_ઢગલો.get_ગણતરી (); i++)
      encoder.encode_અંક_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  સ્થિર શૂન્ય flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  સ્થિર શૂન્ય flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    પ્રતિજ્ઞા (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_અંક_cs (env.width);
  }

  સ્થિર શૂન્ય flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    જો (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      માટે (unsigned પૂર્ણાંક i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

સંરચના range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  તાર્કિક complete (unsigned પૂર્ણાંક last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned ગણતરી = આ->length;
    માટે (unsigned પૂર્ણાંક i = ગણતરી; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned પૂર્ણાંક n_ડાબું = last_glyph - pair.glyph - 1;
      all_glyphs |= n_ડાબું;
      last_glyph = pair.glyph;
      pair.glyph = n_ડાબું;
    }
    તાર્કિક two_byte = all_glyphs >= 0x100;
    પાછા two_byte;
  }
};

સંરચના cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    પસંદગી (op) {

      સ્થિતિ OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ખોટું);
	તોડો;

      સ્થિતિ OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	તોડો;

      સ્થિતિ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	તોડો;

      સ્થિતિ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	તોડો;
    }
  }

  સુરક્ષિત:
  સ્થિર શૂન્ય process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_અંક_2);
    closure->add (env.context.subr_અંક_2);
    param.set_current_str (env, સાચું);
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

સંરચના cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (તાર્કિક desubroutinize_, તાર્કિક drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ op_str_t &opstr,
		  objidx_t subrs_link) અચલ
  {
    TRACE_SERIALIZE (આ);

    જો (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (સાચું);

    જો (opstr.op == OpCode_Subrs)
    {
      જો (desubroutinize || !subrs_link)
	return_trace (સાચું);
      નહીંતર
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  સુરક્ષિત:
  અચલ તાર્કિક desubroutinize;
  અચલ તાર્કિક drop_hints;
};

સંરચના cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, અચલ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (અચલ OT::cff1::accelerator_subset_t &acc_, અચલ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  સ્થિર શૂન્ય complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    જો (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    માટે (unsigned પૂર્ણાંક i = 0; i < env.call_ઢગલો.get_ગણતરી (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ઢગલો[i]);
      જો (likely (parsed_str))
	parsed_str->set_parsed ();
      નહીંતર
	env.set_error ();
    }
  }
};

નામાકાશ OT {
સંરચના cff1_subset_plan
{
  cff1_subset_plan ()
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < name_dict_કિંમતો_t::Val_ગણતરી; i++)
      ટોચ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  શૂન્ય plan_subset_encoding (અચલ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    અચલ Encoding *encoding = acc.encoding;
    unsigned પૂર્ણાંક  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    જો (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (ખોટું);
      પાછા;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_અંક_codes = plan->અંક_output_glyphs () - 1;
    unsigned પૂર્ણાંક glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    જો (it->first == 0) it++;
    auto _ = *it;
    માટે (glyph = 1; glyph < અંક_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      જો (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      નહીંતર
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      જો (code == CFF_UNDEF_CODE)
      {
	subset_enc_અંક_codes = glyph - 1;
	તોડો;
      }

      જો (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      જો (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	માટે (unsigned પૂર્ણાંક i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    પ્રતિજ્ઞા (subset_enc_અંક_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_અંક_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    જો (size0 < size1)
      subset_enc_format = 0;
    નહીંતર
      subset_enc_format = 1;
  }

  તાર્કિક plan_subset_charset (અચલ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned પૂર્ણાંક  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    જો (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (ખોટું);
      પાછા ખોટું;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned અંક_glyphs = plan->અંક_output_glyphs ();

    જો (unlikely (!subset_charset_ranges.alloc (hb_min (અંક_glyphs,
							acc.અંક_charset_entries))))
    {
      plan->check_success (ખોટું);
      પાછા ખોટું;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    તાર્કિક created_map = ખોટું;
    જો (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = સાચું;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    જો (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = ખોટું;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    જો (it->first == 0) it++;
    auto _ = *it;
    તાર્કિક not_is_cid = !acc.is_CID ();
    તાર્કિક skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    જો (not_is_cid)
      sidmap.alloc (અંક_glyphs);
    માટે (hb_codepoint_t glyph = 1; glyph < અંક_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      જો (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      નહીંતર
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      જો (identity_charset)
	sid = glyph;
      નહીંતર
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	જો (not_is_cid)
	  sid = sidmap.add (sid);
      }

      જો (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      જો (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    જો (created_map)
    {
      જો ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    તાર્કિક two_byte = subset_charset_ranges.complete (અંક_glyphs);

    size0 = Charset0::get_size (plan->અંક_output_glyphs ());
    જો (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    નહીંતર
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    જો (size0 < size_ranges)
      subset_charset_format = 0;
    નહીંતર જો (!two_byte)
      subset_charset_format = 1;
    નહીંતર
      subset_charset_format = 2;

    પાછા સાચું;
  }

  તાર્કિક collect_sids_in_dicts (અચલ OT::cff1::accelerator_subset_t &acc)
  {
    માટે (unsigned પૂર્ણાંક i = 0; i < name_dict_કિંમતો_t::Val_ગણતરી; i++)
    {
      unsigned પૂર્ણાંક sid = acc.ટોચ_Dict_2.nameSIDs[i];
      જો (sid != CFF_UNDEF_SID)
      {
	ટોચ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    જો (acc.fdArray != &Null (CFF1FDArray))
      માટે (unsigned પૂર્ણાંક i = 0; i < orig_fdcount; i++)
	જો (fdmap.has (i))
	  (શૂન્ય)sidmap.add (acc.fontDicts[i].fontName);

    પાછા સાચું;
  }

  તાર્કિક create (અચલ OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    જો (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) પાછા ખોટું;

    અંક_glyphs = plan->અંક_output_glyphs ();
    orig_fdcount = acc.fd_ગણતરી;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #નહીંતર
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    જો (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      માટે (અચલ auto &_ : plan->new_to_old_gid_list)
      {
	જો (_.first != _.second)
	{
	  subset_charset = સાચું;
	  તોડો;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.ટોચ_Dict_2);
      તાર્કિક need_to_add_enc = (subset_encoding && !acc.ટોચ_Dict_2.has_op (OpCode_Encoding));
      તાર્કિક need_to_add_set = (subset_charset && !acc.ટોચ_Dict_2.has_op (OpCode_charset));
      જો (need_to_add_enc || need_to_add_set)
      {
	જો (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	જો (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    જો (acc.fdSelect != &Null (CFF1FDSelect))
    {
	જો (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.અંક_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	પાછા ખોટું;
    }
    નહીંતર
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      જો (unlikely (!collect_sids_in_dicts (acc)))
	પાછા ખોટું;
      જો (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	પાછા ખોટું;

      જો (subset_charset && !plan_subset_charset (acc, plan))
        પાછા ખોટું;

      topdict_mod.reassignSIDs (sidmap);
    }

    જો (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<અચલ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      જો (!flattener.flatten (subset_charstrings))
	પાછા ખોટું;
    }
    નહીંતર
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      જો (!subr_subsetter.subset ())
	પાછા ખોટું;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      જો (!subr_subsetter.encode_charstrings (subset_charstrings))
	પાછા ખોટું;

      જો (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	પાછા ખોટું;

      /* local subrs */
      જો (!subset_localsubrs.resize (orig_fdcount))
	પાછા ખોટું;
      માટે (unsigned પૂર્ણાંક fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	જો (fdmap.has (fd))
	{
	  જો (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    પાછા ખોટું;
	}
      }
    }

    /* Encoding */
    જો (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    જો (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_કિંમતો_mod_t ());
    નહીંતર
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (અચલ cff1_font_dict_કિંમતો_t &_)
	{ પાછા fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (અચલ cff1_font_dict_કિંમતો_t &_)
	{
	  cff1_font_dict_કિંમતો_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  પાછા mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    પાછા !plan->in_error () &&
	   (subset_charstrings.length == plan->અંક_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_ટોચ_dict_કિંમતો_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned પૂર્ણાંક    અંક_glyphs;
  unsigned પૂર્ણાંક    orig_fdcount = 0;
  unsigned પૂર્ણાંક    subset_fdcount = 1;
  unsigned પૂર્ણાંક    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_કિંમતો_mod_t>  fontdicts_mod;

  તાર્કિક		drop_hints = ખોટું;

  તાર્કિક		gid_renum;
  તાર્કિક		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned પૂર્ણાંક	subset_enc_અંક_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  તાર્કિક		subset_charset;

  remap_sid_t	sidmap;
  unsigned પૂર્ણાંક	ટોચ_Dict_Mod_SIDs[name_dict_કિંમતો_t::Val_ગણતરી];

  તાર્કિક		desubroutinize = ખોટું;
  તાર્કિક		identity_charset = ખોટું;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

સ્થિર તાર્કિક _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         સંરચના OT::cff1_subset_plan &plan,
                                         અચલ OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  જો (unlikely (!c->start_zerocopy (total_size)))
    પાછા ખોટું;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  જો (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->કાઢો_discard ();
    પાછા ખોટું;
  }

  plan.info.char_strings_link = c->કાઢો_pack (ખોટું);
  પાછા સાચું;
}

તાર્કિક
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   સંરચના OT::cff1_subset_plan &plan) અચલ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  જો (!_serialize_cff1_charstrings(c, plan, *આ))
    પાછા ખોટું;

  /* private dicts & local subrs */
  માટે (પૂર્ણાંક i = (પૂર્ણાંક) privateDicts.length; --i >= 0 ;)
  {
    જો (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      જો (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	જો (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->કાઢો_pack ();
	નહીંતર
	{
	  c->કાઢો_discard ();
	  પાછા ખોટું;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      જો (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->કાઢો_pack ();
      }
      નહીંતર
      {
	c->કાઢો_discard ();
	પાછા ખોટું;
      }
    }
  }

  જો (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  જો (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    જો (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->કાઢો_pack (ખોટું);
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* FDSelect */
  જો (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    જો (likely (hb_serialize_cff_fdselect (c, plan.અંક_glyphs, *fdSelect, fd_ગણતરી,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* Charset */
  જો (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    જો (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.અંક_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* Encoding */
  જો (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    જો (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_અંક_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    જો (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->કાઢો_pack (ખોટું);
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    જો (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->કાઢો_pack ();
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  જો (unlikely (!cff))
    પાછા ખોટું;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  જો (unlikely (!c->embed (*nameIndex))) પાછા ખોટું;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *ટોચ_2 = c->push<ટોચ_Dict> ();
    cff1_ટોચ_dict_op_serializer_t ટોચ_Szr;
    unsigned ટોચ_size = 0;
    ટોચ_dict_modifiers_t  modifier (plan.info, plan.ટોચ_Dict_Mod_SIDs);
    જો (likely (ટોચ_2->serialize (c, plan.topdict_mod, ટોચ_Szr, modifier)))
    {
      ટોચ_size = c->length ();
      c->કાઢો_pack (ખોટું);
    }
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    પાછા dest->serialize_header (c, hb_iter (&ટોચ_size, 1), ટોચ_size);
  }
}

તાર્કિક
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) અચલ
{
  cff1_subset_plan cff_plan;

  જો (unlikely (!cff_plan.create (*આ, c->plan)))
  {
    DEBUG_સંદેશ(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    પાછા ખોટું;
  }

  પાછા serialize (c->serializer, cff_plan);
}


#endif
