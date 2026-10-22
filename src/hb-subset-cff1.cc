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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff1-interp-cs.hh"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਬਣਤਰ remap_sid_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ get_population () ਅਚਲ { ਵਾਪਸ vector.ਲੰਮਾਈ; }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
  {
    map.alloc (ਮਾਪ);
    vector.alloc_exact (ਮਾਪ);
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  { ਵਾਪਸ map.ਵਿੱਚ_error () || vector.ਵਿੱਚ_error (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜੋੜੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid)
  {
    ਜੇ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ਵਾਪਸ sid;

    sid = unoffset_sid (sid);
    ਬਿਨਾਨਿਸ਼ਾਨੀ v = next;
    ਜੇ (map.ਸੈੱਟ (sid, v, ਝੂਠ))
    {
      vector.push (sid);
      next++;
    }
    ਨਹੀਂਤਾਂ
      v = map.get (sid); // already exists
    ਵਾਪਸ ਵਿੱਥ_sid (v);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੰਚਾਲਕ[] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid) ਅਚਲ
  {
    ਜੇ (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      ਵਾਪਸ sid;

    ਵਾਪਸ ਵਿੱਥ_sid (map.get (unoffset_sid (sid)));
  }

  ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_std_ਲੜੀਆਂ = 391;

  ਸਥਿਰ ਤਾਰਕਿਕ is_std_str (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid) { ਵਾਪਸ sid < ਅੰਕ_std_ਲੜੀਆਂ; }
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ_sid (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid) { ਵਾਪਸ sid + ਅੰਕ_std_ਲੜੀਆਂ; }
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ unoffset_sid (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid) { ਵਾਪਸ sid - ਅੰਕ_std_ਲੜੀਆਂ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ next = 0;

  hb_map_t map;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> vector;
};

ਬਣਤਰ cff1_sub_ਸਾਰਣੀ_info_t : cff_sub_ਸਾਰਣੀ_info_t
{
  cff1_sub_ਸਾਰਣੀ_info_t ()
    : cff_sub_ਸਾਰਣੀ_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    ਨਿੱਜੀ_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  ਸਾਰਣੀ_info_t	ਨਿੱਜੀ_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
ਬਣਤਰ cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_mod_t : cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t
{
  ਖਾਲੀ init (ਅਚਲ cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t *ਅਧਾਰ_2= &Null (cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t))
  {
    SUPER::init ();
    ਅਧਾਰ = ਅਧਾਰ_2;
  }

  ਖਾਲੀ fini () { SUPER::fini (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ get_ਗਿਣਤੀ () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ->get_ਗਿਣਤੀ () + SUPER::get_ਗਿਣਤੀ (); }
  ਅਚਲ cff1_ਸਿਖਰ_dict_val_t &get_ਮੁੱਲ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  {
    ਜੇ (i < ਅਧਾਰ->get_ਗਿਣਤੀ ())
      ਵਾਪਸ (*ਅਧਾਰ)[i];
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ SUPER::values[i - ਅਧਾਰ->get_ਗਿਣਤੀ ()];
  }
  ਅਚਲ cff1_ਸਿਖਰ_dict_val_t &ਸੰਚਾਲਕ [] (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ get_ਮੁੱਲ (i); }

  ਖਾਲੀ reassignSIDs (ਅਚਲ remap_sid_t& sidmap)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ; i++)
      nameSIDs[i] = sidmap[ਅਧਾਰ->nameSIDs[i]];
  }

  ਸੁਰੱਖਿਅਤ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t SUPER;
  ਅਚਲ cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t *ਅਧਾਰ;
};

ਬਣਤਰ ਸਿਖਰ_dict_modifiers_t
{
  ਸਿਖਰ_dict_modifiers_t (ਅਚਲ cff1_sub_ਸਾਰਣੀ_info_t &info_,
			ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (&nameSIDs_)[name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ਅਚਲ cff1_sub_ਸਾਰਣੀ_info_t &info;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	(&nameSIDs)[name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ];
};

ਬਣਤਰ cff1_ਸਿਖਰ_dict_op_serializer_t : cff_ਸਿਖਰ_dict_op_serializer_t<cff1_ਸਿਖਰ_dict_val_t>
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ cff1_ਸਿਖਰ_dict_val_t &opstr,
		  ਅਚਲ ਸਿਖਰ_dict_modifiers_t &mod) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    op_ਕੋਡ_t op = opstr.op;
    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_charset:
	ਜੇ (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	ਨਹੀਂਤਾਂ
	  ਜਾਓ fall_ਪਿੱਠ;

      ਹਾਲਤ Op_ਕੋਡ_Encoding:
	ਜੇ (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	ਨਹੀਂਤਾਂ
	  ਜਾਓ fall_ਪਿੱਠ;

      ਹਾਲਤ Op_ਕੋਡ_ਨਿੱਜੀ:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.ਨਿੱਜੀ_Dict_Info.ਮਾਪ) &&
		      Dict::serialize_link4_op (c, op, mod.info.ਨਿੱਜੀ_Dict_Info.link, whence_t::Absolute));

      ਹਾਲਤ Op_ਕੋਡ_ਸੰਸਕਰਣ:
      ਹਾਲਤ Op_ਕੋਡ_Notice:
      ਹਾਲਤ Op_ਕੋਡ_Copyright:
      ਹਾਲਤ Op_ਕੋਡ_ਭਰਿਆ_Name:
      ਹਾਲਤ Op_ਕੋਡ_Family_Name:
      ਹਾਲਤ Op_ਕੋਡ_ਭਾਰ:
      ਹਾਲਤ Op_ਕੋਡ_Post_Script:
      ਹਾਲਤ Op_ਕੋਡ_ਅਧਾਰ_Font_Name:
      ਹਾਲਤ Op_ਕੋਡ_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::name_op_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ (op)]));

      ਹਾਲਤ Op_ਕੋਡ_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  ਜੇ ( unlikely (!(opstr.ਲੰਮਾਈ >= opstr.ਆਖਰੀ_arg_ਵਿੱਥ + 3)))
	    return_trace (ਝੂਠ);
	  supp_op.ptr = opstr.ptr + opstr.ਆਖਰੀ_arg_ਵਿੱਥ;
	  supp_op.ਲੰਮਾਈ = opstr.ਲੰਮਾਈ - opstr.ਆਖਰੀ_arg_ਵਿੱਥ;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ਮੁੱਲਾਂ_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_ਪਿੱਠ:
      ਮੂਲ:
	return_trace (cff_ਸਿਖਰ_dict_op_serializer_t<cff1_ਸਿਖਰ_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (ਸੱਚ);
  }

};

ਬਣਤਰ cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ op_str_t &opstr,
		  ਅਚਲ cff1_font_dict_ਮੁੱਲਾਂ_mod_t &mod) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (opstr.op == Op_ਕੋਡ_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    ਨਹੀਂਤਾਂ
      return_trace (SUPER::serialize (c, opstr, mod.ਨਿੱਜੀ_Dict_Info));
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff_font_dict_op_serializer_t SUPER;
};

ਬਣਤਰ cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ_and_op (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ਜੇ (env.arg_ਸ਼ੁਰੂ > 0)
      flush_ਚੌੜਾਈ (env, param);

    ਚੋਣ (op)
    {
      ਹਾਲਤ Op_ਕੋਡ_hstem:
      ਹਾਲਤ Op_ਕੋਡ_hstemhm:
      ਹਾਲਤ Op_ਕੋਡ_vstem:
      ਹਾਲਤ Op_ਕੋਡ_vstemhm:
      ਹਾਲਤ Op_ਕੋਡ_hintmask:
      ਹਾਲਤ Op_ਕੋਡ_cntrmask:
      ਹਾਲਤ Op_ਕੋਡ_dotsection:
	ਜੇ (param.drop_hints)
	{
	  env.clear_ਦਲੀਲਾਂ ();
	  ਵਾਪਸ;
	}
	HB_FALLTHROUGH;

      ਮੂਲ:
	SUPER::flush_ਦਲੀਲਾਂ_and_op (op, env, param);
	ਰੋਕੋ;
    }
  }
  ਸਥਿਰ ਖਾਲੀ flush_ਦਲੀਲਾਂ (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = env.arg_ਸ਼ੁਰੂ; i < env.arg_ਢੇਰ.get_ਗਿਣਤੀ (); i++)
      encoder.encode_ਅੰਕ_cs (env.eval_arg (i));
    SUPER::flush_ਦਲੀਲਾਂ (env, param);
  }

  ਸਥਿਰ ਖਾਲੀ flush_op (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ਸਥਿਰ ਖਾਲੀ flush_ਚੌੜਾਈ (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (env.has_ਚੌੜਾਈ);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_ਅੰਕ_cs (env.ਚੌੜਾਈ);
  }

  ਸਥਿਰ ਖਾਲੀ flush_hintmask (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    ਜੇ (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.hintmask_ਮਾਪ; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ਬਣਤਰ range_list_t : hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  ਤਾਰਕਿਕ complete (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਇਹ->ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਗਿਣਤੀ_2; i; i--)
    {
      ਕੋਡ_pair_t &pair = arrayZ[i - 1];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n_ਖੱਬਾ = ਆਖਰੀ_glyph - pair.glyph - 1;
      all_glyphs |= n_ਖੱਬਾ;
      ਆਖਰੀ_glyph = pair.glyph;
      pair.glyph = n_ਖੱਬਾ;
    }
    ਤਾਰਕਿਕ two_byte = all_glyphs >= 0x100;
    ਵਾਪਸ two_byte;
  }
};

ਬਣਤਰ cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    ਚੋਣ (op) {

      ਹਾਲਤ Op_ਕੋਡ_return:
	param.current_parsed_str->ਜੋੜੋ_op (op, env.str_ref);
	param.current_parsed_str->ਸੈੱਟ_parsed ();
	env.return_from_subr ();
	param.ਸੈੱਟ_current_str (env, ਝੂਠ);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_endchar:
	param.current_parsed_str->ਜੋੜੋ_op (op, env.str_ref);
	param.current_parsed_str->ਸੈੱਟ_parsed ();
	SUPER::process_op (op, env, param);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_ਘੇਰਾ_2);
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_ਘੇਰਾ);
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->ਜੋੜੋ_op (op, env.str_ref);
	ਰੋਕੋ;
    }
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਥਿਰ ਖਾਲੀ process_call_subr (op_ਕੋਡ_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_ਸੈੱਟ_t *ਘੇਰਾ)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->ਜੋੜੋ_call_op (op, str_ref, env.context.subr_ਅੰਕ_2);
    ਘੇਰਾ->ਜੋੜੋ (env.context.subr_ਅੰਕ_2);
    param.ਸੈੱਟ_current_str (env, ਸੱਚ);
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ਬਣਤਰ cff1_ਨਿੱਜੀ_dict_op_serializer_t : op_serializer_t
{
  cff1_ਨਿੱਜੀ_dict_op_serializer_t (ਤਾਰਕਿਕ desubroutinize_, ਤਾਰਕਿਕ drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ op_str_t &opstr,
		  objidx_t subrs_link) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ਸੱਚ);

    ਜੇ (opstr.op == Op_ਕੋਡ_Subrs)
    {
      ਜੇ (desubroutinize || !subrs_link)
	return_trace (ਸੱਚ);
      ਨਹੀਂਤਾਂ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ ਤਾਰਕਿਕ desubroutinize;
  ਅਚਲ ਤਾਰਕਿਕ drop_hints;
};

ਬਣਤਰ cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ਅਚਲ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_ਕੋਡ_endchar>
{
  cff1_subr_subsetter_t (ਅਚਲ OT::cff1::accelerator_subset_t &acc_, ਅਚਲ hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ਸਥਿਰ ਖਾਲੀ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    ਜੇ (env.has_ਚੌੜਾਈ)
      charstring.ਸੈੱਟ_ਅਗੇਤਰ (env.ਚੌੜਾਈ);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->ਸੈੱਟ_parsed ();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < env.call_ਢੇਰ.get_ਗਿਣਤੀ (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ਢੇਰ[i]);
      ਜੇ (likely (parsed_str))
	parsed_str->ਸੈੱਟ_parsed ();
      ਨਹੀਂਤਾਂ
	env.ਸੈੱਟ_error ();
    }
  }
};

ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ cff1_subset_plan
{
  cff1_subset_plan ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ; i++)
      ਸਿਖਰ_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  ਖਾਲੀ plan_subset_encoding (ਅਚਲ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ਅਚਲ Encoding *encoding = acc.encoding;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  size0, size1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੋਡ, ਆਖਰੀ_ਕੋਡ_2 = CFF_UNDEF_ਕੋਡ - 1;
    hb_ਸਦਿਸ਼_t<hb_codepoint_t> supp_codes;

    subset_enc_ਕੋਡ_ranges.clear ();
    ਜੇ (unlikely (subset_enc_ਕੋਡ_ranges.ਵਿੱਚ_error ()))
    {
      plan->check_success (ਝੂਠ);
      ਵਾਪਸ;
    }

    supp_codes.init ();

    ਕੋਡ_pair_t glyph_ਮੰਜ਼ਿਲ_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_ਅੰਕ_codes = plan->ਅੰਕ_output_glyphs () - 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph;
    ਸਵੈਚਾਲਿਤ it = hb_iter (plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਜੇ (it->ਪਹਿਲਾ == 0) it++;
    ਸਵੈਚਾਲਿਤ _ = *it;
    ਲਈ (glyph = 1; glyph < ਅੰਕ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ਜੇ (glyph == _.ਪਹਿਲਾ)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ਨਹੀਂਤਾਂ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      ਕੋਡ = acc.glyph_ਮੰਜ਼ਿਲ_ਕੋਡ (old_glyph, &glyph_ਮੰਜ਼ਿਲ_sid_cache);
      ਜੇ (ਕੋਡ == CFF_UNDEF_ਕੋਡ)
      {
	subset_enc_ਅੰਕ_codes = glyph - 1;
	ਰੋਕੋ;
      }

      ਜੇ (ਕੋਡ != ਆਖਰੀ_ਕੋਡ_2 + 1)
	subset_enc_ਕੋਡ_ranges.push (ਕੋਡ_pair_t {ਕੋਡ, glyph});
      ਆਖਰੀ_ਕੋਡ_2 = ਕੋਡ;

      ਜੇ (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_ਮੰਜ਼ਿਲ_sid (old_glyph, &glyph_ਮੰਜ਼ਿਲ_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < supp_codes.ਲੰਮਾਈ; i++)
	  subset_enc_supp_codes.push (ਕੋਡ_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_ਕੋਡ_ranges.complete (glyph);

    ਪ੍ਰਤਿਜ੍ਞਾ (subset_enc_ਅੰਕ_codes <= 0xFF);
    size0 = Encoding0::min_ਮਾਪ + HBUINT8::static_ਮਾਪ * subset_enc_ਅੰਕ_codes;
    size1 = Encoding1::min_ਮਾਪ + Encoding1_Range::static_ਮਾਪ * subset_enc_ਕੋਡ_ranges.ਲੰਮਾਈ;

    ਜੇ (size0 < size1)
      subset_enc_format = 0;
    ਨਹੀਂਤਾਂ
      subset_enc_format = 1;
  }

  ਤਾਰਕਿਕ plan_subset_charset (ਅਚਲ OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  size0, ਮਾਪ_ranges;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_sid = CFF_UNDEF_ਕੋਡ - 1;

    subset_charset_ranges.clear ();
    ਜੇ (unlikely (subset_charset_ranges.ਵਿੱਚ_error ()))
    {
      plan->check_success (ਝੂਠ);
      ਵਾਪਸ ਝੂਠ;
    }

    ਕੋਡ_pair_t glyph_ਮੰਜ਼ਿਲ_sid_cache {0, HB_CODEPOINT_INVALID};

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = plan->ਅੰਕ_output_glyphs ();

    ਜੇ (unlikely (!subset_charset_ranges.alloc (hb_min (ਅੰਕ_glyphs,
							acc.ਅੰਕ_charset_entries))))
    {
      plan->check_success (ਝੂਠ);
      ਵਾਪਸ ਝੂਠ;
    }

    glyph_ਮੰਜ਼ਿਲ_sid_map_t *glyph_ਮੰਜ਼ਿਲ_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_ਮੰਜ਼ਿਲ_sid_map.get_acquire () :
					   ਖਾਲੀ_ਸੂਚਕ;

    ਤਾਰਕਿਕ created_map = ਝੂਠ;
    ਜੇ (!glyph_ਮੰਜ਼ਿਲ_sid_map && acc.cff_accelerator)
    {
      created_map = ਸੱਚ;
      glyph_ਮੰਜ਼ਿਲ_sid_map = acc.create_glyph_ਮੰਜ਼ਿਲ_sid_map ();
    }

    ਜੇ (!glyph_ਮੰਜ਼ਿਲ_sid_map || glyph_ਮੰਜ਼ਿਲ_sid_map->ਵਿੱਚ_error ())
    {
      glyph_ਮੰਜ਼ਿਲ_sid_map = ਖਾਲੀ_ਸੂਚਕ;
      created_map = ਝੂਠ;
    }

    ਸਵੈਚਾਲਿਤ it = hb_iter (plan->new_ਮੰਜ਼ਿਲ_old_gid_list);
    ਜੇ (it->ਪਹਿਲਾ == 0) it++;
    ਸਵੈਚਾਲਿਤ _ = *it;
    ਤਾਰਕਿਕ not_is_cid = !acc.is_CID ();
    ਤਾਰਕਿਕ skip = !not_is_cid && glyph_ਮੰਜ਼ਿਲ_sid_map && !ਪਛਾਣ_ਤੱਤ_charset;
    ਜੇ (not_is_cid)
      sidmap.alloc (ਅੰਕ_glyphs);
    ਲਈ (hb_codepoint_t glyph = 1; glyph < ਅੰਕ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      ਜੇ (glyph == _.ਪਹਿਲਾ)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ਨਹੀਂਤਾਂ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      ਬਿਨਾਨਿਸ਼ਾਨੀ sid;
      ਜੇ (ਪਛਾਣ_ਤੱਤ_charset)
	sid = glyph;
      ਨਹੀਂਤਾਂ
      {
	sid = glyph_ਮੰਜ਼ਿਲ_sid_map ?
		       glyph_ਮੰਜ਼ਿਲ_sid_map->arrayZ[old_glyph].ਕੋਡ :
		       acc.glyph_ਮੰਜ਼ਿਲ_sid (old_glyph, &glyph_ਮੰਜ਼ਿਲ_sid_cache);

	ਜੇ (not_is_cid)
	  sid = sidmap.ਜੋੜੋ (sid);
      }

      ਜੇ (sid != ਆਖਰੀ_sid + 1)
	subset_charset_ranges.push (ਕੋਡ_pair_t {sid, glyph});

      ਜੇ (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.ਪਹਿਲਾ - 1, glyph_ਮੰਜ਼ਿਲ_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      ਆਖਰੀ_sid = sid;
    }

    ਜੇ (created_map)
    {
      ਜੇ ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_ਮੰਜ਼ਿਲ_sid_map.cmpexch (ਖਾਲੀ_ਸੂਚਕ, glyph_ਮੰਜ਼ਿਲ_sid_map))
      {
	glyph_ਮੰਜ਼ਿਲ_sid_map->~glyph_ਮੰਜ਼ਿਲ_sid_map_t ();
	hb_free (glyph_ਮੰਜ਼ਿਲ_sid_map);
      }
    }

    ਤਾਰਕਿਕ two_byte = subset_charset_ranges.complete (ਅੰਕ_glyphs);

    size0 = Charset0::get_ਮਾਪ (plan->ਅੰਕ_output_glyphs ());
    ਜੇ (!two_byte)
      ਮਾਪ_ranges = Charset1::get_ਮਾਪ_for_ranges (subset_charset_ranges.ਲੰਮਾਈ);
    ਨਹੀਂਤਾਂ
      ਮਾਪ_ranges = Charset2::get_ਮਾਪ_for_ranges (subset_charset_ranges.ਲੰਮਾਈ);

    ਜੇ (size0 < ਮਾਪ_ranges)
      subset_charset_format = 0;
    ਨਹੀਂਤਾਂ ਜੇ (!two_byte)
      subset_charset_format = 1;
    ਨਹੀਂਤਾਂ
      subset_charset_format = 2;

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ collect_sids_ਵਿੱਚ_dicts (ਅਚਲ OT::cff1::accelerator_subset_t &acc)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sid = acc.ਸਿਖਰ_Dict_2.nameSIDs[i];
      ਜੇ (sid != CFF_UNDEF_SID)
      {
	ਸਿਖਰ_Dict_Mod_SIDs[i] = sidmap.ਜੋੜੋ (sid);
      }
    }

    ਜੇ (acc.fdArray != &Null (CFF1FDArray))
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < orig_fdcount; i++)
	ਜੇ (fdmap.has (i))
	  (ਖਾਲੀ)sidmap.ਜੋੜੋ (acc.fontDicts[i].fontName);

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ create (ਅਚਲ OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    ਜੇ (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) ਵਾਪਸ ਝੂਠ;

    ਅੰਕ_glyphs = plan->ਅੰਕ_output_glyphs ();
    orig_fdcount = acc.fd_ਗਿਣਤੀ;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    ਪਛਾਣ_ਤੱਤ_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_ਪਛਾਣ_ਤੱਤ_CHARSET) && acc.is_CID ();

 #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    min_charstrings_off_ਮਾਪ = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ਨਹੀਂਤਾਂ
    min_charstrings_off_ਮਾਪ = 0;
 #ਜੇ_ਅੰਤ

    subset_charset = !acc.is_predef_charset () || ਪਛਾਣ_ਤੱਤ_charset;
    ਜੇ (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
      {
	ਜੇ (_.ਪਹਿਲਾ != _.second)
	{
	  subset_charset = ਸੱਚ;
	  ਰੋਕੋ;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.ਸਿਖਰ_Dict_2);
      ਤਾਰਕਿਕ need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_enc = (subset_encoding && !acc.ਸਿਖਰ_Dict_2.has_op (Op_ਕੋਡ_Encoding));
      ਤਾਰਕਿਕ need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_ਸੈੱਟ = (subset_charset && !acc.ਸਿਖਰ_Dict_2.has_op (Op_ਕੋਡ_charset));
      ਜੇ (need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_enc || need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_ਸੈੱਟ)
      {
	ਜੇ (need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_enc)
	  topdict_mod.ਜੋੜੋ_op (Op_ਕੋਡ_Encoding);
	ਜੇ (need_ਮੰਜ਼ਿਲ_ਜੋੜੋ_ਸੈੱਟ)
	  topdict_mod.ਜੋੜੋ_op (Op_ਕੋਡ_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    ਜੇ (acc.fdSelect != &Null (CFF1FDSelect))
    {
	ਜੇ (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.ਅੰਕ_glyphs,
				  subset_fdcount,
				  info.fd_select.ਮਾਪ,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
      fdmap.ਪਛਾਣ_ਤੱਤ (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      ਜੇ (unlikely (!collect_sids_ਵਿੱਚ_dicts (acc)))
	ਵਾਪਸ ਝੂਠ;
      ਜੇ (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	ਵਾਪਸ ਝੂਠ;

      ਜੇ (subset_charset && !plan_subset_charset (acc, plan))
        ਵਾਪਸ ਝੂਠ;

      topdict_mod.reassignSIDs (sidmap);
    }

    ਜੇ (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ਅਚਲ OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_ਕੋਡ_endchar>
		    flattener(acc, plan);
      ਜੇ (!flattener.flatten (subset_charstrings))
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      ਜੇ (!subr_subsetter.subset ())
	ਵਾਪਸ ਝੂਠ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      ਜੇ (!subr_subsetter.encode_charstrings (subset_charstrings))
	ਵਾਪਸ ਝੂਠ;

      ਜੇ (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	ਵਾਪਸ ਝੂਠ;

      /* local subrs */
      ਜੇ (!subset_localsubrs.resize (orig_fdcount))
	ਵਾਪਸ ਝੂਠ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	ਜੇ (fdmap.has (fd))
	{
	  ਜੇ (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    ਵਾਪਸ ਝੂਠ;
	}
      }
    }

    /* Encoding */
    ਜੇ (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    ਜੇ (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_ਮੁੱਲਾਂ_mod_t ());
    ਨਹੀਂਤਾਂ
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ਅਚਲ cff1_font_dict_ਮੁੱਲਾਂ_t &_)
	{ ਵਾਪਸ fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ਅਚਲ cff1_font_dict_ਮੁੱਲਾਂ_t &_)
	{
	  cff1_font_dict_ਮੁੱਲਾਂ_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  ਵਾਪਸ mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    ਵਾਪਸ !plan->ਵਿੱਚ_error () &&
	   (subset_charstrings.ਲੰਮਾਈ == plan->ਅੰਕ_output_glyphs ()) &&
	   (fontdicts_mod.ਲੰਮਾਈ == subset_fdcount);
  }

  cff1_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_mod_t	topdict_mod;
  cff1_sub_ਸਾਰਣੀ_info_t		info;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    ਅੰਕ_glyphs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    orig_fdcount = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subset_fdcount = 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    subset_fdselect_format = 0;
  hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_ਸਦਿਸ਼_t<str_buff_vec_t>	subset_localsubrs;
  hb_ਸਦਿਸ਼_t<cff1_font_dict_ਮੁੱਲਾਂ_mod_t>  fontdicts_mod;

  ਤਾਰਕਿਕ		drop_hints = ਝੂਠ;

  ਤਾਰਕਿਕ		gid_renum;
  ਤਾਰਕਿਕ		subset_encoding;
  uint8_t	subset_enc_format;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	subset_enc_ਅੰਕ_codes;
  range_list_t	subset_enc_ਕੋਡ_ranges;
  hb_ਸਦਿਸ਼_t<ਕੋਡ_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  ਤਾਰਕਿਕ		subset_charset;

  remap_sid_t	sidmap;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਸਿਖਰ_Dict_Mod_SIDs[name_dict_ਮੁੱਲਾਂ_t::Val_ਗਿਣਤੀ];

  ਤਾਰਕਿਕ		desubroutinize = ਝੂਠ;
  ਤਾਰਕਿਕ		ਪਛਾਣ_ਤੱਤ_charset = ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ	min_charstrings_off_ਮਾਪ = 0;
};
} // namespace OT

ਸਥਿਰ ਤਾਰਕਿਕ _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ਬਣਤਰ OT::cff1_subset_plan &plan,
                                         ਅਚਲ OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_ਲੜੀਆਂ> ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕੜੇ_ਮਾਪ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_ਮਾਪ = CFF1_Char_ਲੜੀਆਂ::ਕੁੱਲ_ਮਾਪ (plan.subset_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ);
  ਜੇ (unlikely (!c->ਸ਼ੁਰੂ_zerocopy (ਕੁੱਲ_ਮਾਪ)))
    ਵਾਪਸ ਝੂਠ;

  ਸਵੈਚਾਲਿਤ *cs = c->ਸ਼ੁਰੂ_embed<CFF1_Char_ਲੜੀਆਂ> ();
  ਜੇ (unlikely (!cs->serialize (c, plan.subset_charstrings, &ਅੰਕੜੇ_ਮਾਪ, plan.min_charstrings_off_ਮਾਪ))) {
    c->ਕੱਢੋ_discard ();
    ਵਾਪਸ ਝੂਠ;
  }

  plan.info.char_ਲੜੀਆਂ_link = c->ਕੱਢੋ_pack (ਝੂਠ);
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ਬਣਤਰ OT::cff1_subset_plan &plan) ਅਚਲ
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  ਜੇ (!_serialize_cff1_charstrings(c, plan, *ਇਹ))
    ਵਾਪਸ ਝੂਠ;

  /* private dicts & local subrs */
  ਲਈ (ਪੂਰਨ i = (ਪੂਰਨ) ਨਿੱਜੀ_Dicts.ਲੰਮਾਈ; --i >= 0 ;)
  {
    ਜੇ (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      ਜੇ (plan.subset_localsubrs[i].ਲੰਮਾਈ > 0)
      {
	ਸਵੈਚਾਲਿਤ *dest = c->push <CFF1Subrs> ();
	ਜੇ (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ਕੱਢੋ_pack ();
	ਨਹੀਂਤਾਂ
	{
	  c->ਕੱਢੋ_discard ();
	  ਵਾਪਸ ਝੂਠ;
	}
      }

      ਸਵੈਚਾਲਿਤ *pd = c->push<ਨਿੱਜੀ_Dict> ();
      cff1_ਨਿੱਜੀ_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      ਜੇ (likely (pd->serialize (c, ਨਿੱਜੀ_Dicts[i], privSzr, subrs_link)))
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].ਨਿੱਜੀ_Dict_Info.ਮਾਪ = c->ਲੰਮਾਈ ();
	plan.fontdicts_mod[fd].ਨਿੱਜੀ_Dict_Info.link = c->ਕੱਢੋ_pack ();
      }
      ਨਹੀਂਤਾਂ
      {
	c->ਕੱਢੋ_discard ();
	ਵਾਪਸ ਝੂਠ;
      }
    }
  }

  ਜੇ (!is_CID ())
    plan.info.ਨਿੱਜੀ_Dict_Info = plan.fontdicts_mod[0].ਨਿੱਜੀ_Dict_Info;

  /* FDArray (FD Index) */
  ਜੇ (fdArray != &Null (CFF1FDArray))
  {
    ਸਵੈਚਾਲਿਤ *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    ਸਵੈਚਾਲਿਤ it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    ਜੇ (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->ਕੱਢੋ_pack (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* FDSelect */
  ਜੇ (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    ਜੇ (likely (hb_serialize_cff_fdselect (c, plan.ਅੰਕ_glyphs, *fdSelect, fd_ਗਿਣਤੀ,
					   plan.subset_fdselect_format, plan.info.fd_select.ਮਾਪ,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* Charset */
  ਜੇ (plan.subset_charset)
  {
    ਸਵੈਚਾਲਿਤ *dest = c->push<Charset> ();
    ਜੇ (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.ਅੰਕ_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* Encoding */
  ਜੇ (plan.subset_encoding)
  {
    ਸਵੈਚਾਲਿਤ *dest = c->push<Encoding> ();
    ਜੇ (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_ਅੰਕ_codes,
				 plan.subset_enc_ਕੋਡ_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* global subrs */
  {
    ਸਵੈਚਾਲਿਤ *dest = c->push <CFF1Subrs> ();
    ਜੇ (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ਕੱਢੋ_pack (ਝੂਠ);
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  /* String INDEX */
  {
    ਸਵੈਚਾਲਿਤ *dest = c->push<CFF1_String_ਸੂਚਕਾਂਕ> ();
    ਜੇ (likely (!plan.sidmap.ਵਿੱਚ_error () &&
		dest->serialize (c, *string_ਸੂਚਕਾਂਕ, plan.sidmap.vector)))
      c->ਕੱਢੋ_pack ();
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  ਜੇ (unlikely (!cff))
    ਵਾਪਸ ਝੂਠ;

  /*ਸਿਰਲੇਖ*/
  cff->ਸੰਸਕਰਣ.major = 0x01;
  cff->ਸੰਸਕਰਣ.minor = 0x00;
  cff->name_ਸੂਚਕਾਂਕ = cff->min_ਮਾਪ;
  cff->off_ਮਾਪ = 4; /* unused? */

  /* name INDEX */
  ਜੇ (unlikely (!c->embed (*name_ਸੂਚਕਾਂਕ))) ਵਾਪਸ ਝੂਠ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    ਸਵੈਚਾਲਿਤ *ਸਿਖਰ_2 = c->push<ਸਿਖਰ_Dict> ();
    cff1_ਸਿਖਰ_dict_op_serializer_t ਸਿਖਰ_Szr;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਖਰ_ਮਾਪ = 0;
    ਸਿਖਰ_dict_modifiers_t  modifier (plan.info, plan.ਸਿਖਰ_Dict_Mod_SIDs);
    ਜੇ (likely (ਸਿਖਰ_2->serialize (c, plan.topdict_mod, ਸਿਖਰ_Szr, modifier)))
    {
      ਸਿਖਰ_ਮਾਪ = c->ਲੰਮਾਈ ();
      c->ਕੱਢੋ_pack (ਝੂਠ);
    }
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
    /* serialize INDEX header for above */
    ਸਵੈਚਾਲਿਤ *dest = c->ਸ਼ੁਰੂ_embed<CFF1_ਸੂਚਕਾਂਕ> ();
    ਵਾਪਸ dest->serialize_ਸਿਰਲੇਖ (c, hb_iter (&ਸਿਖਰ_ਮਾਪ, 1), ਸਿਖਰ_ਮਾਪ);
  }
}

ਤਾਰਕਿਕ
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ਅਚਲ
{
  cff1_subset_plan cff_plan;

  ਜੇ (unlikely (!cff_plan.create (*ਇਹ, c->plan)))
  {
    DEBUG_ਸੁਨੇਹਾ(SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Failed to generate a cff subsetting plan.");
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ serialize (c->serializer, cff_plan);
}


#ਜੇ_ਅੰਤ
