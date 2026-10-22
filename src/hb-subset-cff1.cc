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

#که_نه_تعریف_شوی HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-set.h"
#ضمن "hb-bimap.hh"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff1-interp-cs.hh"

کارول نطاق CFF;

بنية remap_sid_t
{
  بینښه get_population () ثابت { أرجع vector.اوږدتیا; }

  فراغ alloc (بینښه اندازه)
  {
    map.alloc (اندازه);
    vector.alloc_exact (اندازه);
  }

  منطقي دننه_error () ثابت
  { أرجع map.دننه_error () || vector.دننه_error (); }

  بینښه صحيح جمع_کړه (بینښه صحيح sid)
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    sid = unoffset_sid (sid);
    بینښه v = next;
    إذا (map.سېټ (sid, v, خطأ))
    {
      vector.push (sid);
      next++;
    }
    وإلا
      v = map.get (sid); // already exists
    أرجع دننه_واټن_sid (v);
  }

  بینښه صحيح چلوونکی[] (بینښه صحيح sid) ثابت
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    أرجع دننه_واټن_sid (map.get (unoffset_sid (sid)));
  }

  ساكن ثابت بینښه صحيح عدد_std_مزي_ګانې = 391;

  ساكن منطقي is_std_str (بینښه صحيح sid) { أرجع sid < عدد_std_مزي_ګانې; }
  ساكن بینښه صحيح دننه_واټن_sid (بینښه صحيح sid) { أرجع sid + عدد_std_مزي_ګانې; }
  ساكن بینښه صحيح unoffset_sid (بینښه صحيح sid) { أرجع sid - عدد_std_مزي_ګانې; }
  بینښه next = 0;

  hb_map_t map;
  hb_ویکټور_t<بینښه> vector;
};

بنية cff1_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff1_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    شخصي_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  جدول_info_t	شخصي_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
بنية cff1_سر_dict_ارزښتونه_mod_t : cff1_سر_dict_ارزښتونه_t
{
  فراغ init (ثابت cff1_سر_dict_ارزښتونه_t *بنسټ_2= &Null (cff1_سر_dict_ارزښتونه_t))
  {
    SUPER::init ();
    بنسټ = بنسټ_2;
  }

  فراغ fini () { SUPER::fini (); }

  بینښه get_شمېر () ثابت { أرجع بنسټ->get_شمېر () + SUPER::get_شمېر (); }
  ثابت cff1_سر_dict_val_t &get_ارزښت (بینښه صحيح i) ثابت
  {
    إذا (i < بنسټ->get_شمېر ())
      أرجع (*بنسټ)[i];
    وإلا
      أرجع SUPER::values[i - بنسټ->get_شمېر ()];
  }
  ثابت cff1_سر_dict_val_t &چلوونکی [] (بینښه صحيح i) ثابت { أرجع get_ارزښت (i); }

  فراغ reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    لكل (بینښه صحيح i = 0; i < name_dict_ارزښتونه_t::Val_شمېر; i++)
      nameSIDs[i] = sidmap[بنسټ->nameSIDs[i]];
  }

  محمي:
  تعريفنوع cff1_سر_dict_ارزښتونه_t SUPER;
  ثابت cff1_سر_dict_ارزښتونه_t *بنسټ;
};

بنية سر_dict_modifiers_t
{
  سر_dict_modifiers_t (ثابت cff1_sub_جدول_info_t &info_,
			ثابت بینښه صحيح (&nameSIDs_)[name_dict_ارزښتونه_t::Val_شمېر])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_جدول_info_t &info;
  ثابت بینښه صحيح	(&nameSIDs)[name_dict_ارزښتونه_t::Val_شمېر];
};

بنية cff1_سر_dict_op_serializer_t : cff_سر_dict_op_serializer_t<cff1_سر_dict_val_t>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت cff1_سر_dict_val_t &opstr,
		  ثابت سر_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    op_کوډ_t op = opstr.op;
    بدل (op)
    {
      حالة Op_کوډ_charset:
	إذا (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وإلا
	  اذهب fall_شا;

      حالة Op_کوډ_Encoding:
	إذا (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وإلا
	  اذهب fall_شا;

      حالة Op_کوډ_شخصي:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.شخصي_Dict_Info.اندازه) &&
		      Dict::serialize_link4_op (c, op, mod.info.شخصي_Dict_Info.link, whence_t::Absolute));

      حالة Op_کوډ_نسخه:
      حالة Op_کوډ_Notice:
      حالة Op_کوډ_Copyright:
      حالة Op_کوډ_ډک_Name:
      حالة Op_کوډ_Family_Name:
      حالة Op_کوډ_وزن:
      حالة Op_کوډ_Post_Script:
      حالة Op_کوډ_بنسټ_Font_Name:
      حالة Op_کوډ_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_ارزښتونه_t::name_op_منزل_شاخص (op)]));

      حالة Op_کوډ_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  إذا ( unlikely (!(opstr.اوږدتیا >= opstr.وروستی_arg_دننه_واټن + 3)))
	    return_trace (خطأ);
	  supp_op.ptr = opstr.ptr + opstr.وروستی_arg_دننه_واټن;
	  supp_op.اوږدتیا = opstr.اوږدتیا - opstr.وروستی_arg_دننه_واټن;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ارزښتونه_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_ارزښتونه_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_شا:
      افتراضي:
	return_trace (cff_سر_dict_op_serializer_t<cff1_سر_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (ریښتیا);
  }

};

بنية cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_ارزښتونه_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == Op_کوډ_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وإلا
      return_trace (SUPER::serialize (c, opstr, mod.شخصي_Dict_Info));
  }

  خاص:
  تعريفنوع cff_font_dict_op_serializer_t SUPER;
};

بنية cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساكن فراغ flush_دلیلونه_and_op (op_کوډ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    إذا (env.arg_پیل > 0)
      flush_پلنوالی (env, param);

    بدل (op)
    {
      حالة Op_کوډ_hstem:
      حالة Op_کوډ_hstemhm:
      حالة Op_کوډ_vstem:
      حالة Op_کوډ_vstemhm:
      حالة Op_کوډ_hintmask:
      حالة Op_کوډ_cntrmask:
      حالة Op_کوډ_dotsection:
	إذا (param.drop_hints)
	{
	  env.clear_دلیلونه ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_دلیلونه_and_op (op, env, param);
	اكسر;
    }
  }
  ساكن فراغ flush_دلیلونه (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    لكل (بینښه صحيح i = env.arg_پیل; i < env.arg_ډېری.get_شمېر (); i++)
      encoder.encode_عدد_cs (env.eval_arg (i));
    SUPER::flush_دلیلونه (env, param);
  }

  ساكن فراغ flush_op (op_کوډ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساكن فراغ flush_پلنوالی (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_پلنوالی);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_عدد_cs (env.پلنوالی);
  }

  ساكن فراغ flush_hintmask (op_کوډ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بینښه صحيح i = 0; i < env.hintmask_اندازه; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

بنية range_list_t : hb_ویکټور_t<کوډ_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقي complete (بینښه صحيح وروستی_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    بینښه شمېر = هذا->اوږدتیا;
    لكل (بینښه صحيح i = شمېر; i; i--)
    {
      کوډ_pair_t &pair = arrayZ[i - 1];
      بینښه صحيح n_کیڼ = وروستی_glyph - pair.glyph - 1;
      all_glyphs |= n_کیڼ;
      وروستی_glyph = pair.glyph;
      pair.glyph = n_کیڼ;
    }
    منطقي two_byte = all_glyphs >= 0x100;
    أرجع two_byte;
  }
};

بنية cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساكن فراغ process_op (op_کوډ_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_کوډ_return:
	param.current_parsed_str->جمع_کړه_op (op, env.str_ref);
	param.current_parsed_str->سېټ_parsed ();
	env.return_from_subr ();
	param.سېټ_current_str (env, خطأ);
	اكسر;

      حالة Op_کوډ_endchar:
	param.current_parsed_str->جمع_کړه_op (op, env.str_ref);
	param.current_parsed_str->سېټ_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کوډ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_تړون_2);
	اكسر;

      حالة Op_کوډ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_تړون);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جمع_کړه_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_کوډ_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_سېټ_t *تړون)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جمع_کړه_call_op (op, str_ref, env.context.subr_عدد_2);
    تړون->جمع_کړه (env.context.subr_عدد_2);
    param.سېټ_current_str (env, ریښتیا);
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

بنية cff1_شخصي_dict_op_serializer_t : op_serializer_t
{
  cff1_شخصي_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (ریښتیا);

    إذا (opstr.op == Op_کوډ_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (ریښتیا);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
};

بنية cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_کوډ_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    إذا (env.has_پلنوالی)
      charstring.سېټ_مختاړی (env.پلنوالی);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->سېټ_parsed ();
    لكل (بینښه صحيح i = 0; i < env.call_ډېری.get_شمېر (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ډېری[i]);
      إذا (likely (parsed_str))
	parsed_str->سېټ_parsed ();
      وإلا
	env.سېټ_error ();
    }
  }
};

نطاق OT {
بنية cff1_subset_plan
{
  cff1_subset_plan ()
  {
    لكل (بینښه صحيح i = 0; i < name_dict_ارزښتونه_t::Val_شمېر; i++)
      سر_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  فراغ plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    بینښه صحيح  size0, size1;
    بینښه کوډ, وروستی_کوډ_2 = CFF_UNDEF_کوډ - 1;
    hb_ویکټور_t<hb_codepoint_t> supp_codes;

    subset_enc_کوډ_ranges.clear ();
    إذا (unlikely (subset_enc_کوډ_ranges.دننه_error ()))
    {
      plan->check_success (خطأ);
      أرجع;
    }

    supp_codes.init ();

    کوډ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_عدد_codes = plan->عدد_output_glyphs () - 1;
    بینښه صحيح glyph;
    خپلکاری it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->لومړی == 0) it++;
    خپلکاری _ = *it;
    لكل (glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.لومړی)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      کوډ = acc.glyph_منزل_کوډ (old_glyph, &glyph_منزل_sid_cache);
      إذا (کوډ == CFF_UNDEF_کوډ)
      {
	subset_enc_عدد_codes = glyph - 1;
	اكسر;
      }

      إذا (کوډ != وروستی_کوډ_2 + 1)
	subset_enc_کوډ_ranges.push (کوډ_pair_t {کوډ, glyph});
      وروستی_کوډ_2 = کوډ;

      إذا (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	لكل (بینښه صحيح i = 0; i < supp_codes.اوږدتیا; i++)
	  subset_enc_supp_codes.push (کوډ_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_کوډ_ranges.complete (glyph);

    أكد (subset_enc_عدد_codes <= 0xFF);
    size0 = Encoding0::min_اندازه + HBUINT8::static_اندازه * subset_enc_عدد_codes;
    size1 = Encoding1::min_اندازه + Encoding1_Range::static_اندازه * subset_enc_کوډ_ranges.اوږدتیا;

    إذا (size0 < size1)
      subset_enc_format = 0;
    وإلا
      subset_enc_format = 1;
  }

  منطقي plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    بینښه صحيح  size0, اندازه_ranges;
    بینښه وروستی_sid = CFF_UNDEF_کوډ - 1;

    subset_charset_ranges.clear ();
    إذا (unlikely (subset_charset_ranges.دننه_error ()))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    کوډ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};

    بینښه عدد_glyphs = plan->عدد_output_glyphs ();

    إذا (unlikely (!subset_charset_ranges.alloc (hb_min (عدد_glyphs,
							acc.عدد_charset_entries))))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    glyph_منزل_sid_map_t *glyph_منزل_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_منزل_sid_map.get_acquire () :
					   تش_اشاره;

    منطقي created_map = خطأ;
    إذا (!glyph_منزل_sid_map && acc.cff_accelerator)
    {
      created_map = ریښتیا;
      glyph_منزل_sid_map = acc.create_glyph_منزل_sid_map ();
    }

    إذا (!glyph_منزل_sid_map || glyph_منزل_sid_map->دننه_error ())
    {
      glyph_منزل_sid_map = تش_اشاره;
      created_map = خطأ;
    }

    خپلکاری it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->لومړی == 0) it++;
    خپلکاری _ = *it;
    منطقي not_is_cid = !acc.is_CID ();
    منطقي skip = !not_is_cid && glyph_منزل_sid_map && !بې_اغېزه_عنصر_charset;
    إذا (not_is_cid)
      sidmap.alloc (عدد_glyphs);
    لكل (hb_codepoint_t glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.لومړی)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      بینښه sid;
      إذا (بې_اغېزه_عنصر_charset)
	sid = glyph;
      وإلا
      {
	sid = glyph_منزل_sid_map ?
		       glyph_منزل_sid_map->arrayZ[old_glyph].کوډ :
		       acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);

	إذا (not_is_cid)
	  sid = sidmap.جمع_کړه (sid);
      }

      إذا (sid != وروستی_sid + 1)
	subset_charset_ranges.push (کوډ_pair_t {sid, glyph});

      إذا (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.لومړی - 1, glyph_منزل_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      وروستی_sid = sid;
    }

    إذا (created_map)
    {
      إذا ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_منزل_sid_map.cmpexch (تش_اشاره, glyph_منزل_sid_map))
      {
	glyph_منزل_sid_map->~glyph_منزل_sid_map_t ();
	hb_free (glyph_منزل_sid_map);
      }
    }

    منطقي two_byte = subset_charset_ranges.complete (عدد_glyphs);

    size0 = Charset0::get_اندازه (plan->عدد_output_glyphs ());
    إذا (!two_byte)
      اندازه_ranges = Charset1::get_اندازه_for_ranges (subset_charset_ranges.اوږدتیا);
    وإلا
      اندازه_ranges = Charset2::get_اندازه_for_ranges (subset_charset_ranges.اوږدتیا);

    إذا (size0 < اندازه_ranges)
      subset_charset_format = 0;
    وإلا إذا (!two_byte)
      subset_charset_format = 1;
    وإلا
      subset_charset_format = 2;

    أرجع ریښتیا;
  }

  منطقي collect_sids_دننه_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    لكل (بینښه صحيح i = 0; i < name_dict_ارزښتونه_t::Val_شمېر; i++)
    {
      بینښه صحيح sid = acc.سر_Dict_2.nameSIDs[i];
      إذا (sid != CFF_UNDEF_SID)
      {
	سر_Dict_Mod_SIDs[i] = sidmap.جمع_کړه (sid);
      }
    }

    إذا (acc.fdArray != &Null (CFF1FDArray))
      لكل (بینښه صحيح i = 0; i < orig_fdcount; i++)
	إذا (fdmap.has (i))
	  (فراغ)sidmap.جمع_کړه (acc.fontDicts[i].fontName);

    أرجع ریښتیا;
  }

  منطقي create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) أرجع خطأ;

    عدد_glyphs = plan->عدد_output_glyphs ();
    orig_fdcount = acc.fd_شمېر;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    بې_اغېزه_عنصر_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_بې_اغېزه_عنصر_CHARSET) && acc.is_CID ();

 #که_تعریف_شوی HB_EXPERIMENTAL_API
    min_charstrings_off_اندازه = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_اندازه = 0;
 #د_که_پای

    subset_charset = !acc.is_predef_charset () || بې_اغېزه_عنصر_charset;
    إذا (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      لكل (ثابت خپلکاری &_ : plan->new_منزل_old_gid_list)
      {
	إذا (_.لومړی != _.second)
	{
	  subset_charset = ریښتیا;
	  اكسر;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.سر_Dict_2);
      منطقي need_منزل_جمع_کړه_enc = (subset_encoding && !acc.سر_Dict_2.has_op (Op_کوډ_Encoding));
      منطقي need_منزل_جمع_کړه_سېټ = (subset_charset && !acc.سر_Dict_2.has_op (Op_کوډ_charset));
      إذا (need_منزل_جمع_کړه_enc || need_منزل_جمع_کړه_سېټ)
      {
	إذا (need_منزل_جمع_کړه_enc)
	  topdict_mod.جمع_کړه_op (Op_کوډ_Encoding);
	إذا (need_منزل_جمع_کړه_سېټ)
	  topdict_mod.جمع_کړه_op (Op_کوډ_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    إذا (acc.fdSelect != &Null (CFF1FDSelect))
    {
	إذا (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.عدد_glyphs,
				  subset_fdcount,
				  info.fd_select.اندازه,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.بې_اغېزه_عنصر (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      إذا (unlikely (!collect_sids_دننه_dicts (acc)))
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
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_کوډ_endchar>
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
      لكل (بینښه صحيح fd = 0; fd < orig_fdcount; fd++)
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
      fontdicts_mod.push (cff1_font_dict_ارزښتونه_mod_t ());
    وإلا
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_ارزښتونه_t &_)
	{ أرجع fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_ارزښتونه_t &_)
	{
	  cff1_font_dict_ارزښتونه_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  أرجع mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    أرجع !plan->دننه_error () &&
	   (subset_charstrings.اوږدتیا == plan->عدد_output_glyphs ()) &&
	   (fontdicts_mod.اوږدتیا == subset_fdcount);
  }

  cff1_سر_dict_ارزښتونه_mod_t	topdict_mod;
  cff1_sub_جدول_info_t		info;

  بینښه صحيح    عدد_glyphs;
  بینښه صحيح    orig_fdcount = 0;
  بینښه صحيح    subset_fdcount = 1;
  بینښه صحيح    subset_fdselect_format = 0;
  hb_ویکټور_t<کوډ_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_ویکټور_t<str_buff_vec_t>	subset_localsubrs;
  hb_ویکټور_t<cff1_font_dict_ارزښتونه_mod_t>  fontdicts_mod;

  منطقي		drop_hints = خطأ;

  منطقي		gid_renum;
  منطقي		subset_encoding;
  uint8_t	subset_enc_format;
  بینښه صحيح	subset_enc_عدد_codes;
  range_list_t	subset_enc_کوډ_ranges;
  hb_ویکټور_t<کوډ_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقي		subset_charset;

  remap_sid_t	sidmap;
  بینښه صحيح	سر_Dict_Mod_SIDs[name_dict_ارزښتونه_t::Val_شمېر];

  منطقي		desubroutinize = خطأ;
  منطقي		بې_اغېزه_عنصر_charset = خطأ;

  بینښه	min_charstrings_off_اندازه = 0;
};
} // namespace OT

ساكن منطقي _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         بنية OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_مزي_ګانې> ();

  بینښه معلومات_اندازه = 0;
  بینښه ټول_اندازه = CFF1_Char_مزي_ګانې::ټول_اندازه (plan.subset_charstrings, &معلومات_اندازه, plan.min_charstrings_off_اندازه);
  إذا (unlikely (!c->پیل_zerocopy (ټول_اندازه)))
    أرجع خطأ;

  خپلکاری *cs = c->پیل_embed<CFF1_Char_مزي_ګانې> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &معلومات_اندازه, plan.min_charstrings_off_اندازه))) {
    c->راوباسه_discard ();
    أرجع خطأ;
  }

  plan.info.char_مزي_ګانې_link = c->راوباسه_pack (خطأ);
  أرجع ریښتیا;
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
  لكل (صحيح i = (صحيح) شخصي_Dicts.اوږدتیا; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      إذا (plan.subset_localsubrs[i].اوږدتیا > 0)
      {
	خپلکاری *dest = c->push <CFF1Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->راوباسه_pack ();
	وإلا
	{
	  c->راوباسه_discard ();
	  أرجع خطأ;
	}
      }

      خپلکاری *pd = c->push<شخصي_Dict> ();
      cff1_شخصي_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      إذا (likely (pd->serialize (c, شخصي_Dicts[i], privSzr, subrs_link)))
      {
	بینښه fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].شخصي_Dict_Info.اندازه = c->اوږدتیا ();
	plan.fontdicts_mod[fd].شخصي_Dict_Info.link = c->راوباسه_pack ();
      }
      وإلا
      {
	c->راوباسه_discard ();
	أرجع خطأ;
      }
    }
  }

  إذا (!is_CID ())
    plan.info.شخصي_Dict_Info = plan.fontdicts_mod[0].شخصي_Dict_Info;

  /* FDArray (FD Index) */
  إذا (fdArray != &Null (CFF1FDArray))
  {
    خپلکاری *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    خپلکاری it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    إذا (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->راوباسه_pack (خطأ);
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs, *fdSelect, fd_شمېر,
					   plan.subset_fdselect_format, plan.info.fd_select.اندازه,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->راوباسه_pack ();
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  /* Charset */
  إذا (plan.subset_charset)
  {
    خپلکاری *dest = c->push<Charset> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.عدد_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->راوباسه_pack ();
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  /* Encoding */
  إذا (plan.subset_encoding)
  {
    خپلکاری *dest = c->push<Encoding> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_عدد_codes,
				 plan.subset_enc_کوډ_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->راوباسه_pack ();
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  /* global subrs */
  {
    خپلکاری *dest = c->push <CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->راوباسه_pack (خطأ);
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  /* String INDEX */
  {
    خپلکاری *dest = c->push<CFF1_String_شاخص> ();
    إذا (likely (!plan.sidmap.دننه_error () &&
		dest->serialize (c, *string_شاخص, plan.sidmap.vector)))
      c->راوباسه_pack ();
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff))
    أرجع خطأ;

  /*سرلیک*/
  cff->نسخه.major = 0x01;
  cff->نسخه.minor = 0x00;
  cff->name_شاخص = cff->min_اندازه;
  cff->off_اندازه = 4; /* unused? */

  /* name INDEX */
  إذا (unlikely (!c->embed (*name_شاخص))) أرجع خطأ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    خپلکاری *سر_4 = c->push<سر_Dict> ();
    cff1_سر_dict_op_serializer_t سر_Szr;
    بینښه سر_اندازه = 0;
    سر_dict_modifiers_t  modifier (plan.info, plan.سر_Dict_Mod_SIDs);
    إذا (likely (سر_4->serialize (c, plan.topdict_mod, سر_Szr, modifier)))
    {
      سر_اندازه = c->اوږدتیا ();
      c->راوباسه_pack (خطأ);
    }
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
    /* serialize INDEX header for above */
    خپلکاری *dest = c->پیل_embed<CFF1_شاخص> ();
    أرجع dest->serialize_سرلیک (c, hb_iter (&سر_اندازه, 1), سر_اندازه);
  }
}

منطقي
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  إذا (unlikely (!cff_plan.create (*هذا, c->plan)))
  {
    DEBUG_پیغام(SUBSET, تش_اشاره, "Failed to generate a cff subsetting plan.");
    أرجع خطأ;
  }

  أرجع serialize (c->serializer, cff_plan);
}


#د_که_پای
