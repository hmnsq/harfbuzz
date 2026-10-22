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

#അന്തര്ഭാവ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-cff1-table.hh"
#അന്തര്ഭാവ "hb-set.h"
#അന്തര്ഭാവ "hb-bimap.hh"
#അന്തര്ഭാവ "hb-subset-plan.hh"
#അന്തര്ഭാവ "hb-subset-cff-common.hh"
#അന്തര്ഭാവ "hb-cff1-interp-cs.hh"

using നാമാകാശ CFF;

ഘടന remap_sid_t
{
  unsigned get_population () സ്ഥിരം { തിരികെ vector.length; }

  ശൂന്യം alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  തർക്കം in_error () സ്ഥിരം
  { തിരികെ map.in_error () || vector.in_error (); }

  unsigned പൂർണ്ണസംഖ്യ add (unsigned പൂർണ്ണസംഖ്യ sid)
  {
    എങ്കിൽ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      തിരികെ sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    എങ്കിൽ (map.set (sid, v, അസത്യം))
    {
      vector.push (sid);
      next++;
    }
    അല്ലെങ്കിൽ
      v = map.get (sid); // already exists
    തിരികെ offset_sid (v);
  }

  unsigned പൂർണ്ണസംഖ്യ operator[] (unsigned പൂർണ്ണസംഖ്യ sid) സ്ഥിരം
  {
    എങ്കിൽ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      തിരികെ sid;

    തിരികെ offset_sid (map.get (unoffset_sid (sid)));
  }

  സ്ഥിതം സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ അക്കം_std_strings = 391;

  സ്ഥിതം തർക്കം is_std_str (unsigned പൂർണ്ണസംഖ്യ sid) { തിരികെ sid < അക്കം_std_strings; }
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ offset_sid (unsigned പൂർണ്ണസംഖ്യ sid) { തിരികെ sid + അക്കം_std_strings; }
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ unoffset_sid (unsigned പൂർണ്ണസംഖ്യ sid) { തിരികെ sid - അക്കം_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

ഘടന cff1_sub_table_info_t : cff_sub_table_info_t
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
ഘടന cff1_മുകൾ_dict_മൂല്യങ്ങൾ_mod_t : cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t
{
  ശൂന്യം init (സ്ഥിരം cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t *base_= &Null (cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t))
  {
    SUPER::init ();
    base = base_;
  }

  ശൂന്യം fini () { SUPER::fini (); }

  unsigned get_എണ്ണം () സ്ഥിരം { തിരികെ base->get_എണ്ണം () + SUPER::get_എണ്ണം (); }
  സ്ഥിരം cff1_മുകൾ_dict_val_t &get_മൂല്യം (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  {
    എങ്കിൽ (i < base->get_എണ്ണം ())
      തിരികെ (*base)[i];
    അല്ലെങ്കിൽ
      തിരികെ SUPER::values[i - base->get_എണ്ണം ()];
  }
  സ്ഥിരം cff1_മുകൾ_dict_val_t &operator [] (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ get_മൂല്യം (i); }

  ശൂന്യം reassignSIDs (സ്ഥിരം remap_sid_t& sidmap)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  സംരക്ഷിത:
  തരനിർവചനം cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t SUPER;
  സ്ഥിരം cff1_മുകൾ_dict_മൂല്യങ്ങൾ_t *base;
};

ഘടന മുകൾ_dict_modifiers_t
{
  മുകൾ_dict_modifiers_t (സ്ഥിരം cff1_sub_table_info_t &info_,
			സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ (&nameSIDs_)[name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  സ്ഥിരം cff1_sub_table_info_t &info;
  സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ	(&nameSIDs)[name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം];
};

ഘടന cff1_മുകൾ_dict_op_serializer_t : cff_മുകൾ_dict_op_serializer_t<cff1_മുകൾ_dict_val_t>
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം cff1_മുകൾ_dict_val_t &opstr,
		  സ്ഥിരം മുകൾ_dict_modifiers_t &mod) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    op_code_t op = opstr.op;
    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_charset:
	എങ്കിൽ (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	അല്ലെങ്കിൽ
	  പോകുക fall_back;

      അവസ്ഥ OpCode_Encoding:
	എങ്കിൽ (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	അല്ലെങ്കിൽ
	  പോകുക fall_back;

      അവസ്ഥ OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      അവസ്ഥ OpCode_version:
      അവസ്ഥ OpCode_Notice:
      അവസ്ഥ OpCode_Copyright:
      അവസ്ഥ OpCode_FullName:
      അവസ്ഥ OpCode_FamilyName:
      അവസ്ഥ OpCode_Weight:
      അവസ്ഥ OpCode_PostScript:
      അവസ്ഥ OpCode_BaseFontName:
      അവസ്ഥ OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::name_op_to_index (op)]));

      അവസ്ഥ OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  എങ്കിൽ ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (അസത്യം);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_മൂല്യങ്ങൾ_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      സ്ഥിരസ്ഥിതി:
	return_trace (cff_മുകൾ_dict_op_serializer_t<cff1_മുകൾ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (സത്യം);
  }

};

ഘടന cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം op_str_t &opstr,
		  സ്ഥിരം cff1_font_dict_മൂല്യങ്ങൾ_mod_t &mod) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    അല്ലെങ്കിൽ
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  സ്വകാര്യ:
  തരനിർവചനം cff_font_dict_op_serializer_t SUPER;
};

ഘടന cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  സ്ഥിതം ശൂന്യം flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    എങ്കിൽ (env.arg_start > 0)
      flush_width (env, param);

    തിരഞ്ഞെടുപ്പ് (op)
    {
      അവസ്ഥ OpCode_hstem:
      അവസ്ഥ OpCode_hstemhm:
      അവസ്ഥ OpCode_vstem:
      അവസ്ഥ OpCode_vstemhm:
      അവസ്ഥ OpCode_hintmask:
      അവസ്ഥ OpCode_cntrmask:
      അവസ്ഥ OpCode_dotsection:
	എങ്കിൽ (param.drop_hints)
	{
	  env.clear_args ();
	  തിരികെ;
	}
	HB_FALLTHROUGH;

      സ്ഥിരസ്ഥിതി:
	SUPER::flush_args_and_op (op, env, param);
	നിർത്തുക;
    }
  }
  സ്ഥിതം ശൂന്യം flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = env.arg_start; i < env.arg_അട്ടി.get_എണ്ണം (); i++)
      encoder.encode_അക്കം_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  സ്ഥിതം ശൂന്യം flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  സ്ഥിതം ശൂന്യം flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    പ്രതിജ്ഞാ (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_അക്കം_cs (env.width);
  }

  സ്ഥിതം ശൂന്യം flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    എങ്കിൽ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  സ്വകാര്യ:
  തരനിർവചനം cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ഘടന range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  തർക്കം complete (unsigned പൂർണ്ണസംഖ്യ last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned എണ്ണം = ഇത്->length;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = എണ്ണം; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned പൂർണ്ണസംഖ്യ n_ഇടത് = last_glyph - pair.glyph - 1;
      all_glyphs |= n_ഇടത്;
      last_glyph = pair.glyph;
      pair.glyph = n_ഇടത്;
    }
    തർക്കം two_byte = all_glyphs >= 0x100;
    തിരികെ two_byte;
  }
};

ഘടന cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    തിരഞ്ഞെടുപ്പ് (op) {

      അവസ്ഥ OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, അസത്യം);
	നിർത്തുക;

      അവസ്ഥ OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	നിർത്തുക;

      അവസ്ഥ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	നിർത്തുക;

      അവസ്ഥ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	നിർത്തുക;
    }
  }

  സംരക്ഷിത:
  സ്ഥിതം ശൂന്യം process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_അക്കം_2);
    closure->add (env.context.subr_അക്കം_2);
    param.set_current_str (env, സത്യം);
  }

  സ്വകാര്യ:
  തരനിർവചനം cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ഘടന cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (തർക്കം desubroutinize_, തർക്കം drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം op_str_t &opstr,
		  objidx_t subrs_link) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (സത്യം);

    എങ്കിൽ (opstr.op == OpCode_Subrs)
    {
      എങ്കിൽ (desubroutinize || !subrs_link)
	return_trace (സത്യം);
      അല്ലെങ്കിൽ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  സംരക്ഷിത:
  സ്ഥിരം തർക്കം desubroutinize;
  സ്ഥിരം തർക്കം drop_hints;
};

ഘടന cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, സ്ഥിരം OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (സ്ഥിരം OT::cff1::accelerator_subset_t &acc_, സ്ഥിരം hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  സ്ഥിതം ശൂന്യം complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    എങ്കിൽ (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < env.call_അട്ടി.get_എണ്ണം (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_അട്ടി[i]);
      എങ്കിൽ (likely (parsed_str))
	parsed_str->set_parsed ();
      അല്ലെങ്കിൽ
	env.set_error ();
    }
  }
};

നാമാകാശ OT {
ഘടന cff1_subset_plan
{
  cff1_subset_plan ()
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം; i++)
      മുകൾ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  ശൂന്യം plan_subset_encoding (സ്ഥിരം OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    സ്ഥിരം Encoding *encoding = acc.encoding;
    unsigned പൂർണ്ണസംഖ്യ  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    എങ്കിൽ (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (അസത്യം);
      തിരികെ;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_അക്കം_codes = plan->അക്കം_output_glyphs () - 1;
    unsigned പൂർണ്ണസംഖ്യ glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    എങ്കിൽ (it->first == 0) it++;
    auto _ = *it;
    വേണ്ടി (glyph = 1; glyph < അക്കം_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      എങ്കിൽ (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      അല്ലെങ്കിൽ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      എങ്കിൽ (code == CFF_UNDEF_CODE)
      {
	subset_enc_അക്കം_codes = glyph - 1;
	നിർത്തുക;
      }

      എങ്കിൽ (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      എങ്കിൽ (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    പ്രതിജ്ഞാ (subset_enc_അക്കം_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_അക്കം_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    എങ്കിൽ (size0 < size1)
      subset_enc_format = 0;
    അല്ലെങ്കിൽ
      subset_enc_format = 1;
  }

  തർക്കം plan_subset_charset (സ്ഥിരം OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned പൂർണ്ണസംഖ്യ  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    എങ്കിൽ (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (അസത്യം);
      തിരികെ അസത്യം;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned അക്കം_glyphs = plan->അക്കം_output_glyphs ();

    എങ്കിൽ (unlikely (!subset_charset_ranges.alloc (hb_min (അക്കം_glyphs,
							acc.അക്കം_charset_entries))))
    {
      plan->check_success (അസത്യം);
      തിരികെ അസത്യം;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    തർക്കം created_map = അസത്യം;
    എങ്കിൽ (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = സത്യം;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    എങ്കിൽ (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = അസത്യം;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    എങ്കിൽ (it->first == 0) it++;
    auto _ = *it;
    തർക്കം not_is_cid = !acc.is_CID ();
    തർക്കം skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    എങ്കിൽ (not_is_cid)
      sidmap.alloc (അക്കം_glyphs);
    വേണ്ടി (hb_codepoint_t glyph = 1; glyph < അക്കം_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      എങ്കിൽ (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      അല്ലെങ്കിൽ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      എങ്കിൽ (identity_charset)
	sid = glyph;
      അല്ലെങ്കിൽ
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	എങ്കിൽ (not_is_cid)
	  sid = sidmap.add (sid);
      }

      എങ്കിൽ (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      എങ്കിൽ (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    എങ്കിൽ (created_map)
    {
      എങ്കിൽ ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    തർക്കം two_byte = subset_charset_ranges.complete (അക്കം_glyphs);

    size0 = Charset0::get_size (plan->അക്കം_output_glyphs ());
    എങ്കിൽ (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    അല്ലെങ്കിൽ
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    എങ്കിൽ (size0 < size_ranges)
      subset_charset_format = 0;
    അല്ലെങ്കിൽ എങ്കിൽ (!two_byte)
      subset_charset_format = 1;
    അല്ലെങ്കിൽ
      subset_charset_format = 2;

    തിരികെ സത്യം;
  }

  തർക്കം collect_sids_in_dicts (സ്ഥിരം OT::cff1::accelerator_subset_t &acc)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ sid = acc.മുകൾ_Dict_2.nameSIDs[i];
      എങ്കിൽ (sid != CFF_UNDEF_SID)
      {
	മുകൾ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    എങ്കിൽ (acc.fdArray != &Null (CFF1FDArray))
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < orig_fdcount; i++)
	എങ്കിൽ (fdmap.has (i))
	  (ശൂന്യം)sidmap.add (acc.fontDicts[i].fontName);

    തിരികെ സത്യം;
  }

  തർക്കം create (സ്ഥിരം OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    എങ്കിൽ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) തിരികെ അസത്യം;

    അക്കം_glyphs = plan->അക്കം_output_glyphs ();
    orig_fdcount = acc.fd_എണ്ണം;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #അല്ലെങ്കിൽ
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    എങ്കിൽ (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      വേണ്ടി (സ്ഥിരം auto &_ : plan->new_to_old_gid_list)
      {
	എങ്കിൽ (_.first != _.second)
	{
	  subset_charset = സത്യം;
	  നിർത്തുക;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.മുകൾ_Dict_2);
      തർക്കം need_to_add_enc = (subset_encoding && !acc.മുകൾ_Dict_2.has_op (OpCode_Encoding));
      തർക്കം need_to_add_set = (subset_charset && !acc.മുകൾ_Dict_2.has_op (OpCode_charset));
      എങ്കിൽ (need_to_add_enc || need_to_add_set)
      {
	എങ്കിൽ (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	എങ്കിൽ (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    എങ്കിൽ (acc.fdSelect != &Null (CFF1FDSelect))
    {
	എങ്കിൽ (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.അക്കം_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      എങ്കിൽ (unlikely (!collect_sids_in_dicts (acc)))
	തിരികെ അസത്യം;
      എങ്കിൽ (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	തിരികെ അസത്യം;

      എങ്കിൽ (subset_charset && !plan_subset_charset (acc, plan))
        തിരികെ അസത്യം;

      topdict_mod.reassignSIDs (sidmap);
    }

    എങ്കിൽ (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<സ്ഥിരം OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      എങ്കിൽ (!flattener.flatten (subset_charstrings))
	തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      എങ്കിൽ (!subr_subsetter.subset ())
	തിരികെ അസത്യം;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      എങ്കിൽ (!subr_subsetter.encode_charstrings (subset_charstrings))
	തിരികെ അസത്യം;

      എങ്കിൽ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	തിരികെ അസത്യം;

      /* local subrs */
      എങ്കിൽ (!subset_localsubrs.resize (orig_fdcount))
	തിരികെ അസത്യം;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	എങ്കിൽ (fdmap.has (fd))
	{
	  എങ്കിൽ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    തിരികെ അസത്യം;
	}
      }
    }

    /* Encoding */
    എങ്കിൽ (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    എങ്കിൽ (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_മൂല്യങ്ങൾ_mod_t ());
    അല്ലെങ്കിൽ
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (സ്ഥിരം cff1_font_dict_മൂല്യങ്ങൾ_t &_)
	{ തിരികെ fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (സ്ഥിരം cff1_font_dict_മൂല്യങ്ങൾ_t &_)
	{
	  cff1_font_dict_മൂല്യങ്ങൾ_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  തിരികെ mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    തിരികെ !plan->in_error () &&
	   (subset_charstrings.length == plan->അക്കം_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_മുകൾ_dict_മൂല്യങ്ങൾ_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned പൂർണ്ണസംഖ്യ    അക്കം_glyphs;
  unsigned പൂർണ്ണസംഖ്യ    orig_fdcount = 0;
  unsigned പൂർണ്ണസംഖ്യ    subset_fdcount = 1;
  unsigned പൂർണ്ണസംഖ്യ    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_മൂല്യങ്ങൾ_mod_t>  fontdicts_mod;

  തർക്കം		drop_hints = അസത്യം;

  തർക്കം		gid_renum;
  തർക്കം		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned പൂർണ്ണസംഖ്യ	subset_enc_അക്കം_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  തർക്കം		subset_charset;

  remap_sid_t	sidmap;
  unsigned പൂർണ്ണസംഖ്യ	മുകൾ_Dict_Mod_SIDs[name_dict_മൂല്യങ്ങൾ_t::Val_എണ്ണം];

  തർക്കം		desubroutinize = അസത്യം;
  തർക്കം		identity_charset = അസത്യം;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

സ്ഥിതം തർക്കം _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ഘടന OT::cff1_subset_plan &plan,
                                         സ്ഥിരം OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  എങ്കിൽ (unlikely (!c->start_zerocopy (total_size)))
    തിരികെ അസത്യം;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  എങ്കിൽ (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->എടുക്കുക_discard ();
    തിരികെ അസത്യം;
  }

  plan.info.char_strings_link = c->എടുക്കുക_pack (അസത്യം);
  തിരികെ സത്യം;
}

തർക്കം
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ഘടന OT::cff1_subset_plan &plan) സ്ഥിരം
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  എങ്കിൽ (!_serialize_cff1_charstrings(c, plan, *ഇത്))
    തിരികെ അസത്യം;

  /* private dicts & local subrs */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = (പൂർണ്ണസംഖ്യ) privateDicts.length; --i >= 0 ;)
  {
    എങ്കിൽ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      എങ്കിൽ (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	എങ്കിൽ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->എടുക്കുക_pack ();
	അല്ലെങ്കിൽ
	{
	  c->എടുക്കുക_discard ();
	  തിരികെ അസത്യം;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      എങ്കിൽ (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->എടുക്കുക_pack ();
      }
      അല്ലെങ്കിൽ
      {
	c->എടുക്കുക_discard ();
	തിരികെ അസത്യം;
      }
    }
  }

  എങ്കിൽ (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  എങ്കിൽ (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    എങ്കിൽ (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->എടുക്കുക_pack (അസത്യം);
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* FDSelect */
  എങ്കിൽ (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    എങ്കിൽ (likely (hb_serialize_cff_fdselect (c, plan.അക്കം_glyphs, *fdSelect, fd_എണ്ണം,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* Charset */
  എങ്കിൽ (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    എങ്കിൽ (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.അക്കം_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* Encoding */
  എങ്കിൽ (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    എങ്കിൽ (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_അക്കം_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    എങ്കിൽ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->എടുക്കുക_pack (അസത്യം);
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    എങ്കിൽ (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->എടുക്കുക_pack ();
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  എങ്കിൽ (unlikely (!cff))
    തിരികെ അസത്യം;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  എങ്കിൽ (unlikely (!c->embed (*nameIndex))) തിരികെ അസത്യം;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *മുകൾ_2 = c->push<മുകൾ_Dict> ();
    cff1_മുകൾ_dict_op_serializer_t മുകൾ_Szr;
    unsigned മുകൾ_size = 0;
    മുകൾ_dict_modifiers_t  modifier (plan.info, plan.മുകൾ_Dict_Mod_SIDs);
    എങ്കിൽ (likely (മുകൾ_2->serialize (c, plan.topdict_mod, മുകൾ_Szr, modifier)))
    {
      മുകൾ_size = c->length ();
      c->എടുക്കുക_pack (അസത്യം);
    }
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    തിരികെ dest->serialize_header (c, hb_iter (&മുകൾ_size, 1), മുകൾ_size);
  }
}

തർക്കം
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) സ്ഥിരം
{
  cff1_subset_plan cff_plan;

  എങ്കിൽ (unlikely (!cff_plan.create (*ഇത്, c->plan)))
  {
    DEBUG_സന്ദേശം(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    തിരികെ അസത്യം;
  }

  തിരികെ serialize (c->serializer, cff_plan);
}


#endif
