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

#اگر_تعریف_نشده HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-set.h"
#ضمن "hb-bimap.hh"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff1-interp-cs.hh"

استفاده نطاق CFF;

بنية remap_sid_t
{
  بیعلامت get_population () ثابت { أرجع vector.درازی; }

  فراغ alloc (بیعلامت اندازه)
  {
    map.alloc (اندازه);
    vector.alloc_exact (اندازه);
  }

  منطقي درون_error () ثابت
  { أرجع map.درون_error () || vector.درون_error (); }

  بیعلامت صحيح جمع_کن (بیعلامت صحيح sid)
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    sid = unoffset_sid (sid);
    بیعلامت v = next;
    إذا (map.مجموعه (sid, v, خطأ))
    {
      vector.push (sid);
      next++;
    }
    وإلا
      v = map.get (sid); // already exists
    أرجع جابه_جایی_sid (v);
  }

  بیعلامت صحيح عملگر[] (بیعلامت صحيح sid) ثابت
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    أرجع جابه_جایی_sid (map.get (unoffset_sid (sid)));
  }

  ساكن ثابت بیعلامت صحيح رقم_std_رشتهها = 391;

  ساكن منطقي is_std_str (بیعلامت صحيح sid) { أرجع sid < رقم_std_رشتهها; }
  ساكن بیعلامت صحيح جابه_جایی_sid (بیعلامت صحيح sid) { أرجع sid + رقم_std_رشتهها; }
  ساكن بیعلامت صحيح unoffset_sid (بیعلامت صحيح sid) { أرجع sid - رقم_std_رشتهها; }
  بیعلامت next = 0;

  hb_map_t map;
  hb_وکتور_t<بیعلامت> vector;
};

بنية cff1_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff1_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    خصوصی_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  جدول_info_t	خصوصی_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
بنية cff1_بالاترین_dict_مقادیر_mod_t : cff1_بالاترین_dict_مقادیر_t
{
  فراغ init (ثابت cff1_بالاترین_dict_مقادیر_t *پایه_2= &Null (cff1_بالاترین_dict_مقادیر_t))
  {
    SUPER::init ();
    پایه = پایه_2;
  }

  فراغ fini () { SUPER::fini (); }

  بیعلامت get_شمار () ثابت { أرجع پایه->get_شمار () + SUPER::get_شمار (); }
  ثابت cff1_بالاترین_dict_val_t &get_مقدار (بیعلامت صحيح i) ثابت
  {
    إذا (i < پایه->get_شمار ())
      أرجع (*پایه)[i];
    وإلا
      أرجع SUPER::values[i - پایه->get_شمار ()];
  }
  ثابت cff1_بالاترین_dict_val_t &عملگر [] (بیعلامت صحيح i) ثابت { أرجع get_مقدار (i); }

  فراغ reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    لكل (بیعلامت صحيح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
      nameSIDs[i] = sidmap[پایه->nameSIDs[i]];
  }

  محمي:
  تعريفنوع cff1_بالاترین_dict_مقادیر_t SUPER;
  ثابت cff1_بالاترین_dict_مقادیر_t *پایه;
};

بنية بالاترین_dict_modifiers_t
{
  بالاترین_dict_modifiers_t (ثابت cff1_sub_جدول_info_t &info_,
			ثابت بیعلامت صحيح (&nameSIDs_)[name_dict_مقادیر_t::Val_شمار])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_جدول_info_t &info;
  ثابت بیعلامت صحيح	(&nameSIDs)[name_dict_مقادیر_t::Val_شمار];
};

بنية cff1_بالاترین_dict_op_serializer_t : cff_بالاترین_dict_op_serializer_t<cff1_بالاترین_dict_val_t>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت cff1_بالاترین_dict_val_t &opstr,
		  ثابت بالاترین_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    op_کد_t op = opstr.op;
    بدل (op)
    {
      حالة Op_کد_charset:
	إذا (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پشت;

      حالة Op_کد_Encoding:
	إذا (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پشت;

      حالة Op_کد_خصوصی:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.خصوصی_Dict_Info.اندازه) &&
		      Dict::serialize_link4_op (c, op, mod.info.خصوصی_Dict_Info.link, whence_t::Absolute));

      حالة Op_کد_نسخه:
      حالة Op_کد_Notice:
      حالة Op_کد_Copyright:
      حالة Op_کد_پر_Name:
      حالة Op_کد_Family_Name:
      حالة Op_کد_وزن:
      حالة Op_کد_Post_Script:
      حالة Op_کد_پایه_Font_Name:
      حالة Op_کد_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_مقادیر_t::name_op_مقصد_شاخص (op)]));

      حالة Op_کد_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  إذا ( unlikely (!(opstr.درازی >= opstr.آخرین_arg_جابه_جایی + 3)))
	    return_trace (خطأ);
	  supp_op.ptr = opstr.ptr + opstr.آخرین_arg_جابه_جایی;
	  supp_op.درازی = opstr.درازی - opstr.آخرین_arg_جابه_جایی;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_مقادیر_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_مقادیر_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_پشت:
      افتراضي:
	return_trace (cff_بالاترین_dict_op_serializer_t<cff1_بالاترین_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (درست);
  }

};

بنية cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_مقادیر_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == Op_کد_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وإلا
      return_trace (SUPER::serialize (c, opstr, mod.خصوصی_Dict_Info));
  }

  خاص:
  تعريفنوع cff_font_dict_op_serializer_t SUPER;
};

بنية cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساكن فراغ flush_آرگومانها_and_op (op_کد_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    إذا (env.arg_شروع > 0)
      flush_عرض (env, param);

    بدل (op)
    {
      حالة Op_کد_hstem:
      حالة Op_کد_hstemhm:
      حالة Op_کد_vstem:
      حالة Op_کد_vstemhm:
      حالة Op_کد_hintmask:
      حالة Op_کد_cntrmask:
      حالة Op_کد_dotsection:
	إذا (param.drop_hints)
	{
	  env.clear_آرگومانها ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_آرگومانها_and_op (op, env, param);
	اكسر;
    }
  }
  ساكن فراغ flush_آرگومانها (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    لكل (بیعلامت صحيح i = env.arg_شروع; i < env.arg_پشته.get_شمار (); i++)
      encoder.encode_رقم_cs (env.eval_arg (i));
    SUPER::flush_آرگومانها (env, param);
  }

  ساكن فراغ flush_op (op_کد_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساكن فراغ flush_عرض (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_عرض);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_رقم_cs (env.عرض);
  }

  ساكن فراغ flush_hintmask (op_کد_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بیعلامت صحيح i = 0; i < env.hintmask_اندازه; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

بنية range_list_t : hb_وکتور_t<کد_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقي complete (بیعلامت صحيح آخرین_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    بیعلامت شمار = هذا->درازی;
    لكل (بیعلامت صحيح i = شمار; i; i--)
    {
      کد_pair_t &pair = arrayZ[i - 1];
      بیعلامت صحيح n_چپ = آخرین_glyph - pair.glyph - 1;
      all_glyphs |= n_چپ;
      آخرین_glyph = pair.glyph;
      pair.glyph = n_چپ;
    }
    منطقي two_byte = all_glyphs >= 0x100;
    أرجع two_byte;
  }
};

بنية cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساكن فراغ process_op (op_کد_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_کد_return:
	param.current_parsed_str->جمع_کن_op (op, env.str_ref);
	param.current_parsed_str->مجموعه_parsed ();
	env.return_from_subr ();
	param.مجموعه_current_str (env, خطأ);
	اكسر;

      حالة Op_کد_endchar:
	param.current_parsed_str->جمع_کن_op (op, env.str_ref);
	param.current_parsed_str->مجموعه_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کد_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_بستگی_2);
	اكسر;

      حالة Op_کد_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_بستگی);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جمع_کن_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_کد_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_مجموعه_t *بستگی)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جمع_کن_call_op (op, str_ref, env.context.subr_رقم_2);
    بستگی->جمع_کن (env.context.subr_رقم_2);
    param.مجموعه_current_str (env, درست);
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

بنية cff1_خصوصی_dict_op_serializer_t : op_serializer_t
{
  cff1_خصوصی_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (درست);

    إذا (opstr.op == Op_کد_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (درست);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
};

بنية cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_کد_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    إذا (env.has_عرض)
      charstring.مجموعه_پیشوند (env.عرض);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->مجموعه_parsed ();
    لكل (بیعلامت صحيح i = 0; i < env.call_پشته.get_شمار (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_پشته[i]);
      إذا (likely (parsed_str))
	parsed_str->مجموعه_parsed ();
      وإلا
	env.مجموعه_error ();
    }
  }
};

نطاق OT {
بنية cff1_subset_plan
{
  cff1_subset_plan ()
  {
    لكل (بیعلامت صحيح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
      بالاترین_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  فراغ plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    بیعلامت صحيح  size0, size1;
    بیعلامت کد, آخرین_کد_2 = CFF_UNDEF_کد - 1;
    hb_وکتور_t<hb_codepoint_t> supp_codes;

    subset_enc_کد_ranges.clear ();
    إذا (unlikely (subset_enc_کد_ranges.درون_error ()))
    {
      plan->check_success (خطأ);
      أرجع;
    }

    supp_codes.init ();

    کد_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_رقم_codes = plan->رقم_output_glyphs () - 1;
    بیعلامت صحيح glyph;
    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    إذا (it->اولی == 0) it++;
    خودکار _ = *it;
    لكل (glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.اولی)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      کد = acc.glyph_مقصد_کد (old_glyph, &glyph_مقصد_sid_cache);
      إذا (کد == CFF_UNDEF_کد)
      {
	subset_enc_رقم_codes = glyph - 1;
	اكسر;
      }

      إذا (کد != آخرین_کد_2 + 1)
	subset_enc_کد_ranges.push (کد_pair_t {کد, glyph});
      آخرین_کد_2 = کد;

      إذا (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_مقصد_sid (old_glyph, &glyph_مقصد_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	لكل (بیعلامت صحيح i = 0; i < supp_codes.درازی; i++)
	  subset_enc_supp_codes.push (کد_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_کد_ranges.complete (glyph);

    أكد (subset_enc_رقم_codes <= 0xFF);
    size0 = Encoding0::min_اندازه + HBUINT8::static_اندازه * subset_enc_رقم_codes;
    size1 = Encoding1::min_اندازه + Encoding1_Range::static_اندازه * subset_enc_کد_ranges.درازی;

    إذا (size0 < size1)
      subset_enc_format = 0;
    وإلا
      subset_enc_format = 1;
  }

  منطقي plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    بیعلامت صحيح  size0, اندازه_ranges;
    بیعلامت آخرین_sid = CFF_UNDEF_کد - 1;

    subset_charset_ranges.clear ();
    إذا (unlikely (subset_charset_ranges.درون_error ()))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    کد_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};

    بیعلامت رقم_glyphs = plan->رقم_output_glyphs ();

    إذا (unlikely (!subset_charset_ranges.alloc (hb_min (رقم_glyphs,
							acc.رقم_charset_entries))))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    glyph_مقصد_sid_map_t *glyph_مقصد_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_مقصد_sid_map.get_acquire () :
					   اشاره_تهی;

    منطقي created_map = خطأ;
    إذا (!glyph_مقصد_sid_map && acc.cff_accelerator)
    {
      created_map = درست;
      glyph_مقصد_sid_map = acc.create_glyph_مقصد_sid_map ();
    }

    إذا (!glyph_مقصد_sid_map || glyph_مقصد_sid_map->درون_error ())
    {
      glyph_مقصد_sid_map = اشاره_تهی;
      created_map = خطأ;
    }

    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    إذا (it->اولی == 0) it++;
    خودکار _ = *it;
    منطقي not_is_cid = !acc.is_CID ();
    منطقي skip = !not_is_cid && glyph_مقصد_sid_map && !عنصر_خنثی_charset;
    إذا (not_is_cid)
      sidmap.alloc (رقم_glyphs);
    لكل (hb_codepoint_t glyph = 1; glyph < رقم_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.اولی)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      بیعلامت sid;
      إذا (عنصر_خنثی_charset)
	sid = glyph;
      وإلا
      {
	sid = glyph_مقصد_sid_map ?
		       glyph_مقصد_sid_map->arrayZ[old_glyph].کد :
		       acc.glyph_مقصد_sid (old_glyph, &glyph_مقصد_sid_cache);

	إذا (not_is_cid)
	  sid = sidmap.جمع_کن (sid);
      }

      إذا (sid != آخرین_sid + 1)
	subset_charset_ranges.push (کد_pair_t {sid, glyph});

      إذا (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.اولی - 1, glyph_مقصد_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      آخرین_sid = sid;
    }

    إذا (created_map)
    {
      إذا ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_مقصد_sid_map.cmpexch (اشاره_تهی, glyph_مقصد_sid_map))
      {
	glyph_مقصد_sid_map->~glyph_مقصد_sid_map_t ();
	hb_free (glyph_مقصد_sid_map);
      }
    }

    منطقي two_byte = subset_charset_ranges.complete (رقم_glyphs);

    size0 = Charset0::get_اندازه (plan->رقم_output_glyphs ());
    إذا (!two_byte)
      اندازه_ranges = Charset1::get_اندازه_for_ranges (subset_charset_ranges.درازی);
    وإلا
      اندازه_ranges = Charset2::get_اندازه_for_ranges (subset_charset_ranges.درازی);

    إذا (size0 < اندازه_ranges)
      subset_charset_format = 0;
    وإلا إذا (!two_byte)
      subset_charset_format = 1;
    وإلا
      subset_charset_format = 2;

    أرجع درست;
  }

  منطقي collect_sids_درون_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    لكل (بیعلامت صحيح i = 0; i < name_dict_مقادیر_t::Val_شمار; i++)
    {
      بیعلامت صحيح sid = acc.بالاترین_Dict_2.nameSIDs[i];
      إذا (sid != CFF_UNDEF_SID)
      {
	بالاترین_Dict_Mod_SIDs[i] = sidmap.جمع_کن (sid);
      }
    }

    إذا (acc.fdArray != &Null (CFF1FDArray))
      لكل (بیعلامت صحيح i = 0; i < orig_fdcount; i++)
	إذا (fdmap.has (i))
	  (فراغ)sidmap.جمع_کن (acc.fontDicts[i].fontName);

    أرجع درست;
  }

  منطقي create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) أرجع خطأ;

    رقم_glyphs = plan->رقم_output_glyphs ();
    orig_fdcount = acc.fd_شمار;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    عنصر_خنثی_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_عنصر_خنثی_CHARSET) && acc.is_CID ();

 #اگر_تعریف_شده HB_EXPERIMENTAL_API
    min_charstrings_off_اندازه = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_اندازه = 0;
 #پایان_اگر

    subset_charset = !acc.is_predef_charset () || عنصر_خنثی_charset;
    إذا (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      لكل (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
      {
	إذا (_.اولی != _.second)
	{
	  subset_charset = درست;
	  اكسر;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.بالاترین_Dict_2);
      منطقي need_مقصد_جمع_کن_enc = (subset_encoding && !acc.بالاترین_Dict_2.has_op (Op_کد_Encoding));
      منطقي need_مقصد_جمع_کن_مجموعه = (subset_charset && !acc.بالاترین_Dict_2.has_op (Op_کد_charset));
      إذا (need_مقصد_جمع_کن_enc || need_مقصد_جمع_کن_مجموعه)
      {
	إذا (need_مقصد_جمع_کن_enc)
	  topdict_mod.جمع_کن_op (Op_کد_Encoding);
	إذا (need_مقصد_جمع_کن_مجموعه)
	  topdict_mod.جمع_کن_op (Op_کد_charset);
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
				  info.fd_select.اندازه,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.عنصر_خنثی (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      إذا (unlikely (!collect_sids_درون_dicts (acc)))
	أرجع خطأ;
      إذا (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	أرجع خطأ;

      إذا (subset_charset && !plan_subset_charset (acc, plan))
        أرجع خطأ;

      topdict_mod.reassignSIDs (sidmap);
    }

    إذا (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_کد_endchar>
		    flattener(acc, plan);
      إذا (!flattener.flatten (subset_charstrings))
	أرجع خطأ;
    }
    وإلا
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      إذا (!subr_subsetter.subset ())
	أرجع خطأ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      إذا (!subr_subsetter.encode_charstrings (subset_charstrings))
	أرجع خطأ;

      إذا (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	أرجع خطأ;

      /* local subrs */
      إذا (!subset_localsubrs.resize (orig_fdcount))
	أرجع خطأ;
      لكل (بیعلامت صحيح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	إذا (fdmap.has (fd))
	{
	  إذا (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    أرجع خطأ;
	}
      }
    }

    /* Encoding */
    إذا (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    إذا (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_مقادیر_mod_t ());
    وإلا
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_مقادیر_t &_)
	{ أرجع fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_مقادیر_t &_)
	{
	  cff1_font_dict_مقادیر_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  أرجع mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    أرجع !plan->درون_error () &&
	   (subset_charstrings.درازی == plan->رقم_output_glyphs ()) &&
	   (fontdicts_mod.درازی == subset_fdcount);
  }

  cff1_بالاترین_dict_مقادیر_mod_t	topdict_mod;
  cff1_sub_جدول_info_t		info;

  بیعلامت صحيح    رقم_glyphs;
  بیعلامت صحيح    orig_fdcount = 0;
  بیعلامت صحيح    subset_fdcount = 1;
  بیعلامت صحيح    subset_fdselect_format = 0;
  hb_وکتور_t<کد_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_وکتور_t<str_buff_vec_t>	subset_localsubrs;
  hb_وکتور_t<cff1_font_dict_مقادیر_mod_t>  fontdicts_mod;

  منطقي		drop_hints = خطأ;

  منطقي		gid_renum;
  منطقي		subset_encoding;
  uint8_t	subset_enc_format;
  بیعلامت صحيح	subset_enc_رقم_codes;
  range_list_t	subset_enc_کد_ranges;
  hb_وکتور_t<کد_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقي		subset_charset;

  remap_sid_t	sidmap;
  بیعلامت صحيح	بالاترین_Dict_Mod_SIDs[name_dict_مقادیر_t::Val_شمار];

  منطقي		desubroutinize = خطأ;
  منطقي		عنصر_خنثی_charset = خطأ;

  بیعلامت	min_charstrings_off_اندازه = 0;
};
} // namespace OT

ساكن منطقي _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         بنية OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_رشتهها> ();

  بیعلامت داده_اندازه = 0;
  بیعلامت مجموع_اندازه = CFF1_Char_رشتهها::مجموع_اندازه (plan.subset_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه);
  إذا (unlikely (!c->شروع_zerocopy (مجموع_اندازه)))
    أرجع خطأ;

  خودکار *cs = c->شروع_embed<CFF1_Char_رشتهها> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &داده_اندازه, plan.min_charstrings_off_اندازه))) {
    c->برداشت_discard ();
    أرجع خطأ;
  }

  plan.info.char_رشتهها_link = c->برداشت_pack (خطأ);
  أرجع درست;
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
    أرجع خطأ;

  /* private dicts & local subrs */
  لكل (صحيح i = (صحيح) خصوصی_Dicts.درازی; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      إذا (plan.subset_localsubrs[i].درازی > 0)
      {
	خودکار *dest = c->push <CFF1Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->برداشت_pack ();
	وإلا
	{
	  c->برداشت_discard ();
	  أرجع خطأ;
	}
      }

      خودکار *pd = c->push<خصوصی_Dict> ();
      cff1_خصوصی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      إذا (likely (pd->serialize (c, خصوصی_Dicts[i], privSzr, subrs_link)))
      {
	بیعلامت fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].خصوصی_Dict_Info.اندازه = c->درازی ();
	plan.fontdicts_mod[fd].خصوصی_Dict_Info.link = c->برداشت_pack ();
      }
      وإلا
      {
	c->برداشت_discard ();
	أرجع خطأ;
      }
    }
  }

  إذا (!is_CID ())
    plan.info.خصوصی_Dict_Info = plan.fontdicts_mod[0].خصوصی_Dict_Info;

  /* FDArray (FD Index) */
  إذا (fdArray != &Null (CFF1FDArray))
  {
    خودکار *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    خودکار it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    إذا (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->برداشت_pack (خطأ);
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.رقم_glyphs, *fdSelect, fd_شمار,
					   plan.subset_fdselect_format, plan.info.fd_select.اندازه,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* Charset */
  إذا (plan.subset_charset)
  {
    خودکار *dest = c->push<Charset> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.رقم_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* Encoding */
  إذا (plan.subset_encoding)
  {
    خودکار *dest = c->push<Encoding> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_رقم_codes,
				 plan.subset_enc_کد_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* global subrs */
  {
    خودکار *dest = c->push <CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->برداشت_pack (خطأ);
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  /* String INDEX */
  {
    خودکار *dest = c->push<CFF1_String_شاخص> ();
    إذا (likely (!plan.sidmap.درون_error () &&
		dest->serialize (c, *string_شاخص, plan.sidmap.vector)))
      c->برداشت_pack ();
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff))
    أرجع خطأ;

  /*سرآیند*/
  cff->نسخه.major = 0x01;
  cff->نسخه.minor = 0x00;
  cff->name_شاخص = cff->min_اندازه;
  cff->off_اندازه = 4; /* unused? */

  /* name INDEX */
  إذا (unlikely (!c->embed (*name_شاخص))) أرجع خطأ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    خودکار *بالاترین_2 = c->push<بالاترین_Dict> ();
    cff1_بالاترین_dict_op_serializer_t بالاترین_Szr;
    بیعلامت بالاترین_اندازه = 0;
    بالاترین_dict_modifiers_t  modifier (plan.info, plan.بالاترین_Dict_Mod_SIDs);
    إذا (likely (بالاترین_2->serialize (c, plan.topdict_mod, بالاترین_Szr, modifier)))
    {
      بالاترین_اندازه = c->درازی ();
      c->برداشت_pack (خطأ);
    }
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
    /* serialize INDEX header for above */
    خودکار *dest = c->شروع_embed<CFF1_شاخص> ();
    أرجع dest->serialize_سرآیند (c, hb_iter (&بالاترین_اندازه, 1), بالاترین_اندازه);
  }
}

منطقي
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  إذا (unlikely (!cff_plan.create (*هذا, c->plan)))
  {
    DEBUG_پیام(SUBSET, اشاره_تهی, "Failed to generate a cff subsetting plan.");
    أرجع خطأ;
  }

  أرجع serialize (c->serializer, cff_plan);
}


#پایان_اگر
