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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-cff1-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.h"
#ಅನ್ತರ್ಭಾವ "hb-bimap.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff1-interp-cs.hh"

using ನಾಮಾಕಾಶ CFF;

ಸಂರಚನಾ remap_sid_t
{
  unsigned get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ vector.length; }

  ಶೂನ್ಯ alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ map.in_error () || vector.in_error (); }

  unsigned ಪೂರ್ಣಾಙ್ಕ add (unsigned ಪೂರ್ಣಾಙ್ಕ sid)
  {
    ಯದಿ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ಪ್ರತ್ಯಾಗಮ sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    ಯದಿ (map.set (sid, v, ಅಸತ್ಯ))
    {
      vector.push (sid);
      next++;
    }
    ಅನ್ಯಥಾ
      v = map.get (sid); // already exists
    ಪ್ರತ್ಯಾಗಮ offset_sid (v);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ operator[] (unsigned ಪೂರ್ಣಾಙ್ಕ sid) ಧ್ರುವ
  {
    ಯದಿ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ಪ್ರತ್ಯಾಗಮ sid;

    ಪ್ರತ್ಯಾಗಮ offset_sid (map.get (unoffset_sid (sid)));
  }

  ಸ್ಥಿರ ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_std_strings = 391;

  ಸ್ಥಿರ ತಾರ್ಕಿಕ is_std_str (unsigned ಪೂರ್ಣಾಙ್ಕ sid) { ಪ್ರತ್ಯಾಗಮ sid < ಅಂಕೆ_std_strings; }
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ offset_sid (unsigned ಪೂರ್ಣಾಙ್ಕ sid) { ಪ್ರತ್ಯಾಗಮ sid + ಅಂಕೆ_std_strings; }
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ unoffset_sid (unsigned ಪೂರ್ಣಾಙ್ಕ sid) { ಪ್ರತ್ಯಾಗಮ sid - ಅಂಕೆ_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

ಸಂರಚನಾ cff1_sub_table_info_t : cff_sub_table_info_t
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
ಸಂರಚನಾ cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_mod_t : cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t
{
  ಶೂನ್ಯ init (ಧ್ರುವ cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t *base_= &Null (cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t))
  {
    SUPER::init ();
    base = base_;
  }

  ಶೂನ್ಯ fini () { SUPER::fini (); }

  unsigned get_ಎಣಿಕೆ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ base->get_ಎಣಿಕೆ () + SUPER::get_ಎಣಿಕೆ (); }
  ಧ್ರುವ cff1_ತುದಿ_dict_val_t &get_ಮೌಲ್ಯ (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಯದಿ (i < base->get_ಎಣಿಕೆ ())
      ಪ್ರತ್ಯಾಗಮ (*base)[i];
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ SUPER::values[i - base->get_ಎಣಿಕೆ ()];
  }
  ಧ್ರುವ cff1_ತುದಿ_dict_val_t &operator [] (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_ಮೌಲ್ಯ (i); }

  ಶೂನ್ಯ reassignSIDs (ಧ್ರುವ remap_sid_t& sidmap)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  ರಕ್ಷಿತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t SUPER;
  ಧ್ರುವ cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t *base;
};

ಸಂರಚನಾ ತುದಿ_dict_modifiers_t
{
  ತುದಿ_dict_modifiers_t (ಧ್ರುವ cff1_sub_table_info_t &info_,
			ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ (&nameSIDs_)[name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ಧ್ರುವ cff1_sub_table_info_t &info;
  ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ	(&nameSIDs)[name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ];
};

ಸಂರಚನಾ cff1_ತುದಿ_dict_op_serializer_t : cff_ತುದಿ_dict_op_serializer_t<cff1_ತುದಿ_dict_val_t>
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ cff1_ತುದಿ_dict_val_t &opstr,
		  ಧ್ರುವ ತುದಿ_dict_modifiers_t &mod) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    op_code_t op = opstr.op;
    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_charset:
	ಯದಿ (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	ಅನ್ಯಥಾ
	  ಗಚ್ಛ fall_back;

      ಪಕ್ಷ OpCode_Encoding:
	ಯದಿ (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	ಅನ್ಯಥಾ
	  ಗಚ್ಛ fall_back;

      ಪಕ್ಷ OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      ಪಕ್ಷ OpCode_version:
      ಪಕ್ಷ OpCode_Notice:
      ಪಕ್ಷ OpCode_Copyright:
      ಪಕ್ಷ OpCode_FullName:
      ಪಕ್ಷ OpCode_FamilyName:
      ಪಕ್ಷ OpCode_Weight:
      ಪಕ್ಷ OpCode_PostScript:
      ಪಕ್ಷ OpCode_BaseFontName:
      ಪಕ್ಷ OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::name_op_to_index (op)]));

      ಪಕ್ಷ OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  ಯದಿ ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (ಅಸತ್ಯ);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ಮೌಲ್ಯಗಳು_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      ಸ್ವಭಾವ:
	return_trace (cff_ತುದಿ_dict_op_serializer_t<cff1_ತುದಿ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (ಸತ್ಯ);
  }

};

ಸಂರಚನಾ cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ op_str_t &opstr,
		  ಧ್ರುವ cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t &mod) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    ಅನ್ಯಥಾ
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff_font_dict_op_serializer_t SUPER;
};

ಸಂರಚನಾ cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ಯದಿ (env.arg_start > 0)
      flush_width (env, param);

    ವಿಕಲ್ಪ (op)
    {
      ಪಕ್ಷ OpCode_hstem:
      ಪಕ್ಷ OpCode_hstemhm:
      ಪಕ್ಷ OpCode_vstem:
      ಪಕ್ಷ OpCode_vstemhm:
      ಪಕ್ಷ OpCode_hintmask:
      ಪಕ್ಷ OpCode_cntrmask:
      ಪಕ್ಷ OpCode_dotsection:
	ಯದಿ (param.drop_hints)
	{
	  env.clear_args ();
	  ಪ್ರತ್ಯಾಗಮ;
	}
	HB_FALLTHROUGH;

      ಸ್ವಭಾವ:
	SUPER::flush_args_and_op (op, env, param);
	ಭಙ್ಗ;
    }
  }
  ಸ್ಥಿರ ಶೂನ್ಯ flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = env.arg_start; i < env.arg_ರಾಶಿ.get_ಎಣಿಕೆ (); i++)
      encoder.encode_ಅಂಕೆ_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ಪ್ರತಿಜ್ಞಾ (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_ಅಂಕೆ_cs (env.width);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    ಯದಿ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ಸಂರಚನಾ range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  ತಾರ್ಕಿಕ complete (unsigned ಪೂರ್ಣಾಙ್ಕ last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned ಎಣಿಕೆ = ಏತತ್->length;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = ಎಣಿಕೆ; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned ಪೂರ್ಣಾಙ್ಕ n_ಎಡ = last_glyph - pair.glyph - 1;
      all_glyphs |= n_ಎಡ;
      last_glyph = pair.glyph;
      pair.glyph = n_ಎಡ;
    }
    ತಾರ್ಕಿಕ two_byte = all_glyphs >= 0x100;
    ಪ್ರತ್ಯಾಗಮ two_byte;
  }
};

ಸಂರಚನಾ cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    ವಿಕಲ್ಪ (op) {

      ಪಕ್ಷ OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, ಅಸತ್ಯ);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	ಭಙ್ಗ;
    }
  }

  ರಕ್ಷಿತ:
  ಸ್ಥಿರ ಶೂನ್ಯ process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_ಅಂಕೆ_2);
    closure->add (env.context.subr_ಅಂಕೆ_2);
    param.set_current_str (env, ಸತ್ಯ);
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ಸಂರಚನಾ cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (ತಾರ್ಕಿಕ desubroutinize_, ತಾರ್ಕಿಕ drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ op_str_t &opstr,
		  objidx_t subrs_link) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ಸತ್ಯ);

    ಯದಿ (opstr.op == OpCode_Subrs)
    {
      ಯದಿ (desubroutinize || !subrs_link)
	return_trace (ಸತ್ಯ);
      ಅನ್ಯಥಾ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  ರಕ್ಷಿತ:
  ಧ್ರುವ ತಾರ್ಕಿಕ desubroutinize;
  ಧ್ರುವ ತಾರ್ಕಿಕ drop_hints;
};

ಸಂರಚನಾ cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ಧ್ರುವ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ಧ್ರುವ OT::cff1::accelerator_subset_t &acc_, ಧ್ರುವ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ಸ್ಥಿರ ಶೂನ್ಯ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    ಯದಿ (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < env.call_ರಾಶಿ.get_ಎಣಿಕೆ (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ರಾಶಿ[i]);
      ಯದಿ (likely (parsed_str))
	parsed_str->set_parsed ();
      ಅನ್ಯಥಾ
	env.set_error ();
    }
  }
};

ನಾಮಾಕಾಶ OT {
ಸಂರಚನಾ cff1_subset_plan
{
  cff1_subset_plan ()
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ; i++)
      ತುದಿ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  ಶೂನ್ಯ plan_subset_encoding (ಧ್ರುವ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ಧ್ರುವ Encoding *encoding = acc.encoding;
    unsigned ಪೂರ್ಣಾಙ್ಕ  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    ಯದಿ (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (ಅಸತ್ಯ);
      ಪ್ರತ್ಯಾಗಮ;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_ಅಂಕೆ_codes = plan->ಅಂಕೆ_output_glyphs () - 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    ಯದಿ (it->first == 0) it++;
    auto _ = *it;
    ಪ್ರತಿ (glyph = 1; glyph < ಅಂಕೆ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ಯದಿ (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ಅನ್ಯಥಾ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      ಯದಿ (code == CFF_UNDEF_CODE)
      {
	subset_enc_ಅಂಕೆ_codes = glyph - 1;
	ಭಙ್ಗ;
      }

      ಯದಿ (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      ಯದಿ (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    ಪ್ರತಿಜ್ಞಾ (subset_enc_ಅಂಕೆ_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_ಅಂಕೆ_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    ಯದಿ (size0 < size1)
      subset_enc_format = 0;
    ಅನ್ಯಥಾ
      subset_enc_format = 1;
  }

  ತಾರ್ಕಿಕ plan_subset_charset (ಧ್ರುವ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    ಯದಿ (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (ಅಸತ್ಯ);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned ಅಂಕೆ_glyphs = plan->ಅಂಕೆ_output_glyphs ();

    ಯದಿ (unlikely (!subset_charset_ranges.alloc (hb_min (ಅಂಕೆ_glyphs,
							acc.ಅಂಕೆ_charset_entries))))
    {
      plan->check_success (ಅಸತ್ಯ);
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    ತಾರ್ಕಿಕ created_map = ಅಸತ್ಯ;
    ಯದಿ (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = ಸತ್ಯ;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    ಯದಿ (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = ಅಸತ್ಯ;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    ಯದಿ (it->first == 0) it++;
    auto _ = *it;
    ತಾರ್ಕಿಕ not_is_cid = !acc.is_CID ();
    ತಾರ್ಕಿಕ skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    ಯದಿ (not_is_cid)
      sidmap.alloc (ಅಂಕೆ_glyphs);
    ಪ್ರತಿ (hb_codepoint_t glyph = 1; glyph < ಅಂಕೆ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ಯದಿ (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ಅನ್ಯಥಾ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      ಯದಿ (identity_charset)
	sid = glyph;
      ಅನ್ಯಥಾ
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	ಯದಿ (not_is_cid)
	  sid = sidmap.add (sid);
      }

      ಯದಿ (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      ಯದಿ (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    ಯದಿ (created_map)
    {
      ಯದಿ ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    ತಾರ್ಕಿಕ two_byte = subset_charset_ranges.complete (ಅಂಕೆ_glyphs);

    size0 = Charset0::get_size (plan->ಅಂಕೆ_output_glyphs ());
    ಯದಿ (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    ಅನ್ಯಥಾ
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    ಯದಿ (size0 < size_ranges)
      subset_charset_format = 0;
    ಅನ್ಯಥಾ ಯದಿ (!two_byte)
      subset_charset_format = 1;
    ಅನ್ಯಥಾ
      subset_charset_format = 2;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ collect_sids_in_dicts (ಧ್ರುವ OT::cff1::accelerator_subset_t &acc)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ sid = acc.ತುದಿ_Dict_2.nameSIDs[i];
      ಯದಿ (sid != CFF_UNDEF_SID)
      {
	ತುದಿ_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    ಯದಿ (acc.fdArray != &Null (CFF1FDArray))
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < orig_fdcount; i++)
	ಯದಿ (fdmap.has (i))
	  (ಶೂನ್ಯ)sidmap.add (acc.fontDicts[i].fontName);

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ create (ಧ್ರುವ OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ಯದಿ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಅಂಕೆ_glyphs = plan->ಅಂಕೆ_output_glyphs ();
    orig_fdcount = acc.fd_ಎಣಿಕೆ;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ಅನ್ಯಥಾ
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    ಯದಿ (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      ಪ್ರತಿ (ಧ್ರುವ auto &_ : plan->new_to_old_gid_list)
      {
	ಯದಿ (_.first != _.second)
	{
	  subset_charset = ಸತ್ಯ;
	  ಭಙ್ಗ;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.ತುದಿ_Dict_2);
      ತಾರ್ಕಿಕ need_to_add_enc = (subset_encoding && !acc.ತುದಿ_Dict_2.has_op (OpCode_Encoding));
      ತಾರ್ಕಿಕ need_to_add_set = (subset_charset && !acc.ತುದಿ_Dict_2.has_op (OpCode_charset));
      ಯದಿ (need_to_add_enc || need_to_add_set)
      {
	ಯದಿ (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	ಯದಿ (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    ಯದಿ (acc.fdSelect != &Null (CFF1FDSelect))
    {
	ಯದಿ (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.ಅಂಕೆ_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      ಯದಿ (unlikely (!collect_sids_in_dicts (acc)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (subset_charset && !plan_subset_charset (acc, plan))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      topdict_mod.reassignSIDs (sidmap);
    }

    ಯದಿ (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ಧ್ರುವ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      ಯದಿ (!flattener.flatten (subset_charstrings))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ಯದಿ (!subr_subsetter.subset ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ಯದಿ (!subr_subsetter.encode_charstrings (subset_charstrings))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* local subrs */
      ಯದಿ (!subset_localsubrs.resize (orig_fdcount))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ಯದಿ (fdmap.has (fd))
	{
	  ಯದಿ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
      }
    }

    /* Encoding */
    ಯದಿ (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    ಯದಿ (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t ());
    ಅನ್ಯಥಾ
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ಧ್ರುವ cff1_font_dict_ಮೌಲ್ಯಗಳು_t &_)
	{ ಪ್ರತ್ಯಾಗಮ fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ಧ್ರುವ cff1_font_dict_ಮೌಲ್ಯಗಳು_t &_)
	{
	  cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  ಪ್ರತ್ಯಾಗಮ mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    ಪ್ರತ್ಯಾಗಮ !plan->in_error () &&
	   (subset_charstrings.length == plan->ಅಂಕೆ_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned ಪೂರ್ಣಾಙ್ಕ    ಅಂಕೆ_glyphs;
  unsigned ಪೂರ್ಣಾಙ್ಕ    orig_fdcount = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subset_fdcount = 1;
  unsigned ಪೂರ್ಣಾಙ್ಕ    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_ಮೌಲ್ಯಗಳು_mod_t>  fontdicts_mod;

  ತಾರ್ಕಿಕ		drop_hints = ಅಸತ್ಯ;

  ತಾರ್ಕಿಕ		gid_renum;
  ತಾರ್ಕಿಕ		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned ಪೂರ್ಣಾಙ್ಕ	subset_enc_ಅಂಕೆ_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  ತಾರ್ಕಿಕ		subset_charset;

  remap_sid_t	sidmap;
  unsigned ಪೂರ್ಣಾಙ್ಕ	ತುದಿ_Dict_Mod_SIDs[name_dict_ಮೌಲ್ಯಗಳು_t::Val_ಎಣಿಕೆ];

  ತಾರ್ಕಿಕ		desubroutinize = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ		identity_charset = ಅಸತ್ಯ;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

ಸ್ಥಿರ ತಾರ್ಕಿಕ _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ಸಂರಚನಾ OT::cff1_subset_plan &plan,
                                         ಧ್ರುವ OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  ಯದಿ (unlikely (!c->start_zerocopy (total_size)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  ಯದಿ (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->ತೆಗೆ_discard ();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  plan.info.char_strings_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ಸಂರಚನಾ OT::cff1_subset_plan &plan) ಧ್ರುವ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ಯದಿ (!_serialize_cff1_charstrings(c, plan, *ಏತತ್))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  /* private dicts & local subrs */
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (ಪೂರ್ಣಾಙ್ಕ) privateDicts.length; --i >= 0 ;)
  {
    ಯದಿ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      ಯದಿ (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	ಯದಿ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ತೆಗೆ_pack ();
	ಅನ್ಯಥಾ
	{
	  c->ತೆಗೆ_discard ();
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      ಯದಿ (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->ತೆಗೆ_pack ();
      }
      ಅನ್ಯಥಾ
      {
	c->ತೆಗೆ_discard ();
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    }
  }

  ಯದಿ (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  ಯದಿ (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    ಯದಿ (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* FDSelect */
  ಯದಿ (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    ಯದಿ (likely (hb_serialize_cff_fdselect (c, plan.ಅಂಕೆ_glyphs, *fdSelect, fd_ಎಣಿಕೆ,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* Charset */
  ಯದಿ (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    ಯದಿ (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.ಅಂಕೆ_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* Encoding */
  ಯದಿ (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    ಯದಿ (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_ಅಂಕೆ_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    ಯದಿ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    ಯದಿ (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->ತೆಗೆ_pack ();
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ಯದಿ (unlikely (!cff))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  ಯದಿ (unlikely (!c->embed (*nameIndex))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *ತುದಿ_2 = c->push<ತುದಿ_Dict> ();
    cff1_ತುದಿ_dict_op_serializer_t ತುದಿ_Szr;
    unsigned ತುದಿ_size = 0;
    ತುದಿ_dict_modifiers_t  modifier (plan.info, plan.ತುದಿ_Dict_Mod_SIDs);
    ಯದಿ (likely (ತುದಿ_2->serialize (c, plan.topdict_mod, ತುದಿ_Szr, modifier)))
    {
      ತುದಿ_size = c->length ();
      c->ತೆಗೆ_pack (ಅಸತ್ಯ);
    }
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    ಪ್ರತ್ಯಾಗಮ dest->serialize_header (c, hb_iter (&ತುದಿ_size, 1), ತುದಿ_size);
  }
}

ತಾರ್ಕಿಕ
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ಧ್ರುವ
{
  cff1_subset_plan cff_plan;

  ಯದಿ (unlikely (!cff_plan.create (*ಏತತ್, c->plan)))
  {
    DEBUG_ಸಂದೇಶ(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ serialize (c->serializer, cff_plan);
}


#endif
