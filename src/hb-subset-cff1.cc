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

#ضمن "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-set.h"
#ضمن "hb-bimap.hh"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff1-interp-cs.hh"

using نطاق CFF;

بنية remap_sid_t
{
  unsigned get_population () ثابت { إرجاع vector.length; }

  فراغ alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  منطقي in_error () ثابت
  { إرجاع map.in_error () || vector.in_error (); }

  unsigned صحيح add (unsigned صحيح sid)
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      إرجاع sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    إذا (map.set (sid, v, خطأ))
    {
      vector.push (sid);
      next++;
    }
    وإلا
      v = map.get (sid); // already exists
    إرجاع offset_sid (v);
  }

  unsigned صحيح operator[] (unsigned صحيح sid) ثابت
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      إرجاع sid;

    إرجاع offset_sid (map.get (unoffset_sid (sid)));
  }

  ساكن ثابت unsigned صحيح رقم_std_strings = 391;

  ساكن منطقي is_std_str (unsigned صحيح sid) { إرجاع sid < رقم_std_strings; }
  ساكن unsigned صحيح offset_sid (unsigned صحيح sid) { إرجاع sid + رقم_std_strings; }
  ساكن unsigned صحيح unoffset_sid (unsigned صحيح sid) { إرجاع sid - رقم_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

بنية cff1_sub_table_info_t : cff_sub_table_info_t
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
بنية cff1_قمة_dict_قيم_mod_t : cff1_قمة_dict_قيم_t
{
  فراغ init (ثابت cff1_قمة_dict_قيم_t *base_= &Null (cff1_قمة_dict_قيم_t))
  {
    SUPER::init ();
    base = base_;
  }

  فراغ fini () { SUPER::fini (); }

  unsigned get_تعداد () ثابت { إرجاع base->get_تعداد () + SUPER::get_تعداد (); }
  ثابت cff1_قمة_dict_val_t &get_قيمة (unsigned صحيح i) ثابت
  {
    إذا (i < base->get_تعداد ())
      إرجاع (*base)[i];
    وإلا
      إرجاع SUPER::values[i - base->get_تعداد ()];
  }
  ثابت cff1_قمة_dict_val_t &operator [] (unsigned صحيح i) ثابت { إرجاع get_قيمة (i); }

  فراغ reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    لكل (unsigned صحيح i = 0; i < name_dict_قيم_t::Val_تعداد; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  محمي:
  تعريفنوع cff1_قمة_dict_قيم_t SUPER;
  ثابت cff1_قمة_dict_قيم_t *base;
};

بنية قمة_dict_modifiers_t
{
  قمة_dict_modifiers_t (ثابت cff1_sub_table_info_t &info_,
			ثابت unsigned صحيح (&nameSIDs_)[name_dict_قيم_t::Val_تعداد])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_table_info_t &info;
  ثابت unsigned صحيح	(&nameSIDs)[name_dict_قيم_t::Val_تعداد];
};

بنية cff1_قمة_dict_op_serializer_t : cff_قمة_dict_op_serializer_t<cff1_قمة_dict_val_t>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت cff1_قمة_dict_val_t &opstr,
		  ثابت قمة_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    op_code_t op = opstr.op;
    بدل (op)
    {
      حالة OpCode_charset:
	إذا (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وإلا
	  اذهب fall_back;

      حالة OpCode_Encoding:
	إذا (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وإلا
	  اذهب fall_back;

      حالة OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      حالة OpCode_version:
      حالة OpCode_Notice:
      حالة OpCode_Copyright:
      حالة OpCode_FullName:
      حالة OpCode_FamilyName:
      حالة OpCode_Weight:
      حالة OpCode_PostScript:
      حالة OpCode_BaseFontName:
      حالة OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_قيم_t::name_op_to_index (op)]));

      حالة OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  إذا ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (خطأ);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_قيم_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_قيم_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      افتراضي:
	return_trace (cff_قمة_dict_op_serializer_t<cff1_قمة_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (true);
  }

};

بنية cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_قيم_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وإلا
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  خاص:
  تعريفنوع cff_font_dict_op_serializer_t SUPER;
};

بنية cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساكن فراغ flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    إذا (env.arg_start > 0)
      flush_width (env, param);

    بدل (op)
    {
      حالة OpCode_hstem:
      حالة OpCode_hstemhm:
      حالة OpCode_vstem:
      حالة OpCode_vstemhm:
      حالة OpCode_hintmask:
      حالة OpCode_cntrmask:
      حالة OpCode_dotsection:
	إذا (param.drop_hints)
	{
	  env.clear_args ();
	  إرجاع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_args_and_op (op, env, param);
	اكسر;
    }
  }
  ساكن فراغ flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    لكل (unsigned صحيح i = env.arg_start; i < env.arg_مكدس.get_تعداد (); i++)
      encoder.encode_رقم_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  ساكن فراغ flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساكن فراغ flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_رقم_cs (env.width);
  }

  ساكن فراغ flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (unsigned صحيح i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

بنية range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقي complete (unsigned صحيح last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned تعداد_2 = هذا->length;
    لكل (unsigned صحيح i = تعداد_2; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned صحيح n_يسار = last_glyph - pair.glyph - 1;
      all_glyphs |= n_يسار;
      last_glyph = pair.glyph;
      pair.glyph = n_يسار;
    }
    منطقي two_byte = all_glyphs >= 0x100;
    إرجاع two_byte;
  }
};

بنية cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساكن فراغ process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, خطأ);
	اكسر;

      حالة OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	اكسر;

      حالة OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_رقم_2);
    closure->add (env.context.subr_رقم_2);
    param.set_current_str (env, true);
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

بنية cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    إذا (opstr.op == OpCode_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (true);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
};

بنية cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    إذا (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    لكل (unsigned صحيح i = 0; i < env.call_مكدس.get_تعداد (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_مكدس[i]);
      إذا (likely (parsed_str))
	parsed_str->set_parsed ();
      وإلا
	env.set_error ();
    }
  }
};

نطاق OT {
بنية cff1_subset_plan
{
  cff1_subset_plan ()
  {
    لكل (unsigned صحيح i = 0; i < name_dict_قيم_t::Val_تعداد; i++)
      قمة_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  فراغ plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    unsigned صحيح  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    إذا (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (خطأ);
      إرجاع;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_رقم_codes = plan->رقم_output_glyphs () - 1;
    unsigned صحيح glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    إذا (it->first == 0) it++;
    auto _ = *it;
    لكل (glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      إذا (code == CFF_UNDEF_CODE)
      {
	subset_enc_رقم_codes = glyph - 1;
	اكسر;
      }

      إذا (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      إذا (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	لكل (unsigned صحيح i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    أكد (subset_enc_رقم_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_رقم_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    إذا (size0 < size1)
      subset_enc_format = 0;
    وإلا
      subset_enc_format = 1;
  }

  منطقي plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned صحيح  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    إذا (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (خطأ);
      إرجاع خطأ;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned رقم_glyphs = plan->رقم_output_glyphs ();

    إذا (unlikely (!subset_charset_ranges.alloc (hb_min (رقم_glyphs,
							acc.رقم_charset_entries))))
    {
      plan->check_success (خطأ);
      إرجاع خطأ;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    منطقي created_map = خطأ;
    إذا (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = true;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    إذا (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = خطأ;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    إذا (it->first == 0) it++;
    auto _ = *it;
    منطقي not_is_cid = !acc.is_CID ();
    منطقي skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    إذا (not_is_cid)
      sidmap.alloc (رقم_glyphs);
    لكل (hb_codepoint_t glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      إذا (identity_charset)
	sid = glyph;
      وإلا
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	إذا (not_is_cid)
	  sid = sidmap.add (sid);
      }

      إذا (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      إذا (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    إذا (created_map)
    {
      إذا ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    منطقي two_byte = subset_charset_ranges.complete (رقم_glyphs);

    size0 = Charset0::get_size (plan->رقم_output_glyphs ());
    إذا (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    وإلا
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    إذا (size0 < size_ranges)
      subset_charset_format = 0;
    وإلا إذا (!two_byte)
      subset_charset_format = 1;
    وإلا
      subset_charset_format = 2;

    إرجاع true;
  }

  منطقي collect_sids_in_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    لكل (unsigned صحيح i = 0; i < name_dict_قيم_t::Val_تعداد; i++)
    {
      unsigned صحيح sid = acc.قمة_Dict_2.nameSIDs[i];
      إذا (sid != CFF_UNDEF_SID)
      {
	قمة_Dict_Mod_SIDs[i] = sidmap.add (sid);
      }
    }

    إذا (acc.fdArray != &Null (CFF1FDArray))
      لكل (unsigned صحيح i = 0; i < orig_fdcount; i++)
	إذا (fdmap.has (i))
	  (فراغ)sidmap.add (acc.fontDicts[i].fontName);

    إرجاع true;
  }

  منطقي create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) إرجاع خطأ;

    رقم_glyphs = plan->رقم_output_glyphs ();
    orig_fdcount = acc.fd_تعداد;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    إذا (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      لكل (ثابت auto &_ : plan->new_to_old_gid_list)
      {
	إذا (_.first != _.second)
	{
	  subset_charset = true;
	  اكسر;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.قمة_Dict_2);
      منطقي need_to_add_enc = (subset_encoding && !acc.قمة_Dict_2.has_op (OpCode_Encoding));
      منطقي need_to_add_set = (subset_charset && !acc.قمة_Dict_2.has_op (OpCode_charset));
      إذا (need_to_add_enc || need_to_add_set)
      {
	إذا (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	إذا (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    إذا (acc.fdSelect != &Null (CFF1FDSelect))
    {
	إذا (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.رقم_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	إرجاع خطأ;
    }
    وإلا
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      إذا (unlikely (!collect_sids_in_dicts (acc)))
	إرجاع خطأ;
      إذا (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	إرجاع خطأ;

      إذا (subset_charset && !plan_subset_charset (acc, plan))
        إرجاع خطأ;

      topdict_mod.reassignSIDs (sidmap);
    }

    إذا (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      إذا (!flattener.flatten (subset_charstrings))
	إرجاع خطأ;
    }
    وإلا
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      إذا (!subr_subsetter.subset ())
	إرجاع خطأ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      إذا (!subr_subsetter.encode_charstrings (subset_charstrings))
	إرجاع خطأ;

      إذا (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	إرجاع خطأ;

      /* local subrs */
      إذا (!subset_localsubrs.resize (orig_fdcount))
	إرجاع خطأ;
      لكل (unsigned صحيح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	إذا (fdmap.has (fd))
	{
	  إذا (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    إرجاع خطأ;
	}
      }
    }

    /* Encoding */
    إذا (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    إذا (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_قيم_mod_t ());
    وإلا
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_قيم_t &_)
	{ إرجاع fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_قيم_t &_)
	{
	  cff1_font_dict_قيم_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  إرجاع mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    إرجاع !plan->in_error () &&
	   (subset_charstrings.length == plan->رقم_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_قمة_dict_قيم_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned صحيح    رقم_glyphs;
  unsigned صحيح    orig_fdcount = 0;
  unsigned صحيح    subset_fdcount = 1;
  unsigned صحيح    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_قيم_mod_t>  fontdicts_mod;

  منطقي		drop_hints = خطأ;

  منطقي		gid_renum;
  منطقي		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned صحيح	subset_enc_رقم_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقي		subset_charset;

  remap_sid_t	sidmap;
  unsigned صحيح	قمة_Dict_Mod_SIDs[name_dict_قيم_t::Val_تعداد];

  منطقي		desubroutinize = خطأ;
  منطقي		identity_charset = خطأ;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

ساكن منطقي _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         بنية OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  إذا (unlikely (!c->start_zerocopy (total_size)))
    إرجاع خطأ;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->اسحب_discard ();
    إرجاع خطأ;
  }

  plan.info.char_strings_link = c->اسحب_pack (خطأ);
  إرجاع true;
}

منطقي
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   بنية OT::cff1_subset_plan &plan) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  إذا (!_serialize_cff1_charstrings(c, plan, *هذا))
    إرجاع خطأ;

  /* private dicts & local subrs */
  لكل (صحيح i = (صحيح) privateDicts.length; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      إذا (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->اسحب_pack ();
	وإلا
	{
	  c->اسحب_discard ();
	  إرجاع خطأ;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      إذا (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->اسحب_pack ();
      }
      وإلا
      {
	c->اسحب_discard ();
	إرجاع خطأ;
      }
    }
  }

  إذا (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  إذا (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    إذا (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->اسحب_pack (خطأ);
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs, *fdSelect, fd_تعداد,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->اسحب_pack ();
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  /* Charset */
  إذا (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.رقم_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->اسحب_pack ();
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  /* Encoding */
  إذا (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_رقم_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->اسحب_pack ();
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->اسحب_pack (خطأ);
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    إذا (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->اسحب_pack ();
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff))
    إرجاع خطأ;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  إذا (unlikely (!c->embed (*nameIndex))) إرجاع خطأ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *قمة_2 = c->push<قمة_Dict> ();
    cff1_قمة_dict_op_serializer_t قمة_Szr;
    unsigned قمة_size = 0;
    قمة_dict_modifiers_t  modifier (plan.info, plan.قمة_Dict_Mod_SIDs);
    إذا (likely (قمة_2->serialize (c, plan.topdict_mod, قمة_Szr, modifier)))
    {
      قمة_size = c->length ();
      c->اسحب_pack (خطأ);
    }
    وإلا
    {
      c->اسحب_discard ();
      إرجاع خطأ;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    إرجاع dest->serialize_header (c, hb_iter (&قمة_size, 1), قمة_size);
  }
}

منطقي
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  إذا (unlikely (!cff_plan.create (*هذا, c->plan)))
  {
    DEBUG_رسالة(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    إرجاع خطأ;
  }

  إرجاع serialize (c->serializer, cff_plan);
}


#endif
