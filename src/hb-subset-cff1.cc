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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-set.h"
#ضمن "hb-bimap.hh"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff1-interp-cs.hh"

استعمال نطاق CFF;

بنية remap_sid_t
{
  بنانشان get_population () ثابت { أرجع vector.ڊيگهائي; }

  فراغ alloc (بنانشان سائيز)
  {
    map.alloc (سائيز);
    vector.alloc_exact (سائيز);
  }

  منطقي اندريون_error () ثابت
  { أرجع map.اندريون_error () || vector.اندريون_error (); }

  بنانشان صحيح جوڙيو (بنانشان صحيح sid)
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    sid = unoffset_sid (sid);
    بنانشان v = next;
    إذا (map.سيٽ (sid, v, خطأ))
    {
      vector.push (sid);
      next++;
    }
    وإلا
      v = map.get (sid); // already exists
    أرجع وٿي_sid (v);
  }

  بنانشان صحيح عامل[] (بنانشان صحيح sid) ثابت
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    أرجع وٿي_sid (map.get (unoffset_sid (sid)));
  }

  ساكن ثابت بنانشان صحيح انگ_std_لڙيون = 391;

  ساكن منطقي is_std_str (بنانشان صحيح sid) { أرجع sid < انگ_std_لڙيون; }
  ساكن بنانشان صحيح وٿي_sid (بنانشان صحيح sid) { أرجع sid + انگ_std_لڙيون; }
  ساكن بنانشان صحيح unoffset_sid (بنانشان صحيح sid) { أرجع sid - انگ_std_لڙيون; }
  بنانشان next = 0;

  hb_map_t map;
  hb_ويڪٽر_t<بنانشان> vector;
};

بنية cff1_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff1_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    ذاتي_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  جدول_info_t	ذاتي_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
بنية cff1_چوٽي_dict_قدرون_mod_t : cff1_چوٽي_dict_قدرون_t
{
  فراغ init (ثابت cff1_چوٽي_dict_قدرون_t *بنياد_2= &Null (cff1_چوٽي_dict_قدرون_t))
  {
    SUPER::init ();
    بنياد = بنياد_2;
  }

  فراغ fini () { SUPER::fini (); }

  بنانشان get_ڳڻپ () ثابت { أرجع بنياد->get_ڳڻپ () + SUPER::get_ڳڻپ (); }
  ثابت cff1_چوٽي_dict_val_t &get_قدر (بنانشان صحيح i) ثابت
  {
    إذا (i < بنياد->get_ڳڻپ ())
      أرجع (*بنياد)[i];
    وإلا
      أرجع SUPER::values[i - بنياد->get_ڳڻپ ()];
  }
  ثابت cff1_چوٽي_dict_val_t &عامل [] (بنانشان صحيح i) ثابت { أرجع get_قدر (i); }

  فراغ reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    لكل (بنانشان صحيح i = 0; i < name_dict_قدرون_t::Val_ڳڻپ; i++)
      nameSIDs[i] = sidmap[بنياد->nameSIDs[i]];
  }

  محمي:
  تعريفنوع cff1_چوٽي_dict_قدرون_t SUPER;
  ثابت cff1_چوٽي_dict_قدرون_t *بنياد;
};

بنية چوٽي_dict_modifiers_t
{
  چوٽي_dict_modifiers_t (ثابت cff1_sub_جدول_info_t &info_,
			ثابت بنانشان صحيح (&nameSIDs_)[name_dict_قدرون_t::Val_ڳڻپ])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_جدول_info_t &info;
  ثابت بنانشان صحيح	(&nameSIDs)[name_dict_قدرون_t::Val_ڳڻپ];
};

بنية cff1_چوٽي_dict_op_serializer_t : cff_چوٽي_dict_op_serializer_t<cff1_چوٽي_dict_val_t>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت cff1_چوٽي_dict_val_t &opstr,
		  ثابت چوٽي_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    op_ڪوڊ_t op = opstr.op;
    بدل (op)
    {
      حالة Op_ڪوڊ_charset:
	إذا (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پٺ;

      حالة Op_ڪوڊ_Encoding:
	إذا (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پٺ;

      حالة Op_ڪوڊ_ذاتي:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.ذاتي_Dict_Info.سائيز) &&
		      Dict::serialize_link4_op (c, op, mod.info.ذاتي_Dict_Info.link, whence_t::Absolute));

      حالة Op_ڪوڊ_نسخو:
      حالة Op_ڪوڊ_Notice:
      حالة Op_ڪوڊ_Copyright:
      حالة Op_ڪوڊ_ڀريل_Name:
      حالة Op_ڪوڊ_Family_Name:
      حالة Op_ڪوڊ_وزن:
      حالة Op_ڪوڊ_Post_Script:
      حالة Op_ڪوڊ_بنياد_Font_Name:
      حالة Op_ڪوڊ_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_قدرون_t::name_op_منزل_اشاريو (op)]));

      حالة Op_ڪوڊ_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  إذا ( unlikely (!(opstr.ڊيگهائي >= opstr.آخري_arg_وٿي + 3)))
	    return_trace (خطأ);
	  supp_op.ptr = opstr.ptr + opstr.آخري_arg_وٿي;
	  supp_op.ڊيگهائي = opstr.ڊيگهائي - opstr.آخري_arg_وٿي;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_قدرون_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_قدرون_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_پٺ:
      افتراضي:
	return_trace (cff_چوٽي_dict_op_serializer_t<cff1_چوٽي_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (سچ);
  }

};

بنية cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_قدرون_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == Op_ڪوڊ_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وإلا
      return_trace (SUPER::serialize (c, opstr, mod.ذاتي_Dict_Info));
  }

  خاص:
  تعريفنوع cff_font_dict_op_serializer_t SUPER;
};

بنية cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساكن فراغ flush_دليلون_and_op (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    إذا (env.arg_شروعات > 0)
      flush_ويڪر (env, param);

    بدل (op)
    {
      حالة Op_ڪوڊ_hstem:
      حالة Op_ڪوڊ_hstemhm:
      حالة Op_ڪوڊ_vstem:
      حالة Op_ڪوڊ_vstemhm:
      حالة Op_ڪوڊ_hintmask:
      حالة Op_ڪوڊ_cntrmask:
      حالة Op_ڪوڊ_dotsection:
	إذا (param.drop_hints)
	{
	  env.clear_دليلون ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_دليلون_and_op (op, env, param);
	اكسر;
    }
  }
  ساكن فراغ flush_دليلون (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    لكل (بنانشان صحيح i = env.arg_شروعات; i < env.arg_ڍير.get_ڳڻپ (); i++)
      encoder.encode_انگ_cs (env.eval_arg (i));
    SUPER::flush_دليلون (env, param);
  }

  ساكن فراغ flush_op (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساكن فراغ flush_ويڪر (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_ويڪر);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_انگ_cs (env.ويڪر);
  }

  ساكن فراغ flush_hintmask (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بنانشان صحيح i = 0; i < env.hintmask_سائيز; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

بنية range_list_t : hb_ويڪٽر_t<ڪوڊ_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقي complete (بنانشان صحيح آخري_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    بنانشان ڳڻپ = هذا->ڊيگهائي;
    لكل (بنانشان صحيح i = ڳڻپ; i; i--)
    {
      ڪوڊ_pair_t &pair = arrayZ[i - 1];
      بنانشان صحيح n_کاٻو = آخري_glyph - pair.glyph - 1;
      all_glyphs |= n_کاٻو;
      آخري_glyph = pair.glyph;
      pair.glyph = n_کاٻو;
    }
    منطقي two_byte = all_glyphs >= 0x100;
    أرجع two_byte;
  }
};

بنية cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_ڪوڊ_return:
	param.current_parsed_str->جوڙيو_op (op, env.str_ref);
	param.current_parsed_str->سيٽ_parsed ();
	env.return_from_subr ();
	param.سيٽ_current_str (env, خطأ);
	اكسر;

      حالة Op_ڪوڊ_endchar:
	param.current_parsed_str->جوڙيو_op (op, env.str_ref);
	param.current_parsed_str->سيٽ_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_ڪوڊ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_گهيرو_2);
	اكسر;

      حالة Op_ڪوڊ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_گهيرو);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڙيو_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_ڪوڊ_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_سيٽ_t *گهيرو)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڙيو_call_op (op, str_ref, env.context.subr_انگ_2);
    گهيرو->جوڙيو (env.context.subr_انگ_2);
    param.سيٽ_current_str (env, سچ);
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

بنية cff1_ذاتي_dict_op_serializer_t : op_serializer_t
{
  cff1_ذاتي_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    إذا (opstr.op == Op_ڪوڊ_Subrs)
    {
      إذا (desubroutinize || !subrs_link)
	return_trace (سچ);
      وإلا
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محمي:
  ثابت منطقي desubroutinize;
  ثابت منطقي drop_hints;
};

بنية cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_ڪوڊ_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    إذا (env.has_ويڪر)
      charstring.سيٽ_اڳياڙي (env.ويڪر);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->سيٽ_parsed ();
    لكل (بنانشان صحيح i = 0; i < env.call_ڍير.get_ڳڻپ (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ڍير[i]);
      إذا (likely (parsed_str))
	parsed_str->سيٽ_parsed ();
      وإلا
	env.سيٽ_error ();
    }
  }
};

نطاق OT {
بنية cff1_subset_plan
{
  cff1_subset_plan ()
  {
    لكل (بنانشان صحيح i = 0; i < name_dict_قدرون_t::Val_ڳڻپ; i++)
      چوٽي_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  فراغ plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    بنانشان صحيح  size0, size1;
    بنانشان ڪوڊ, آخري_ڪوڊ_2 = CFF_UNDEF_ڪوڊ - 1;
    hb_ويڪٽر_t<hb_codepoint_t> supp_codes;

    subset_enc_ڪوڊ_ranges.clear ();
    إذا (unlikely (subset_enc_ڪوڊ_ranges.اندريون_error ()))
    {
      plan->check_success (خطأ);
      أرجع;
    }

    supp_codes.init ();

    ڪوڊ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_انگ_codes = plan->انگ_output_glyphs () - 1;
    بنانشان صحيح glyph;
    خودڪار it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->پهريون == 0) it++;
    خودڪار _ = *it;
    لكل (glyph = 1; glyph < انگ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.پهريون)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      ڪوڊ = acc.glyph_منزل_ڪوڊ (old_glyph, &glyph_منزل_sid_cache);
      إذا (ڪوڊ == CFF_UNDEF_ڪوڊ)
      {
	subset_enc_انگ_codes = glyph - 1;
	اكسر;
      }

      إذا (ڪوڊ != آخري_ڪوڊ_2 + 1)
	subset_enc_ڪوڊ_ranges.push (ڪوڊ_pair_t {ڪوڊ, glyph});
      آخري_ڪوڊ_2 = ڪوڊ;

      إذا (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	لكل (بنانشان صحيح i = 0; i < supp_codes.ڊيگهائي; i++)
	  subset_enc_supp_codes.push (ڪوڊ_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_ڪوڊ_ranges.complete (glyph);

    أكد (subset_enc_انگ_codes <= 0xFF);
    size0 = Encoding0::min_سائيز + HBUINT8::static_سائيز * subset_enc_انگ_codes;
    size1 = Encoding1::min_سائيز + Encoding1_Range::static_سائيز * subset_enc_ڪوڊ_ranges.ڊيگهائي;

    إذا (size0 < size1)
      subset_enc_format = 0;
    وإلا
      subset_enc_format = 1;
  }

  منطقي plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    بنانشان صحيح  size0, سائيز_ranges;
    بنانشان آخري_sid = CFF_UNDEF_ڪوڊ - 1;

    subset_charset_ranges.clear ();
    إذا (unlikely (subset_charset_ranges.اندريون_error ()))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    ڪوڊ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};

    بنانشان انگ_glyphs = plan->انگ_output_glyphs ();

    إذا (unlikely (!subset_charset_ranges.alloc (hb_min (انگ_glyphs,
							acc.انگ_charset_entries))))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    glyph_منزل_sid_map_t *glyph_منزل_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_منزل_sid_map.get_acquire () :
					   خالي_اشارو;

    منطقي created_map = خطأ;
    إذا (!glyph_منزل_sid_map && acc.cff_accelerator)
    {
      created_map = سچ;
      glyph_منزل_sid_map = acc.create_glyph_منزل_sid_map ();
    }

    إذا (!glyph_منزل_sid_map || glyph_منزل_sid_map->اندريون_error ())
    {
      glyph_منزل_sid_map = خالي_اشارو;
      created_map = خطأ;
    }

    خودڪار it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->پهريون == 0) it++;
    خودڪار _ = *it;
    منطقي not_is_cid = !acc.is_CID ();
    منطقي skip = !not_is_cid && glyph_منزل_sid_map && !سڃاڻپ_عنصر_charset;
    إذا (not_is_cid)
      sidmap.alloc (انگ_glyphs);
    لكل (hb_codepoint_t glyph = 1; glyph < انگ_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.پهريون)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      بنانشان sid;
      إذا (سڃاڻپ_عنصر_charset)
	sid = glyph;
      وإلا
      {
	sid = glyph_منزل_sid_map ?
		       glyph_منزل_sid_map->arrayZ[old_glyph].ڪوڊ :
		       acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);

	إذا (not_is_cid)
	  sid = sidmap.جوڙيو (sid);
      }

      إذا (sid != آخري_sid + 1)
	subset_charset_ranges.push (ڪوڊ_pair_t {sid, glyph});

      إذا (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.پهريون - 1, glyph_منزل_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      آخري_sid = sid;
    }

    إذا (created_map)
    {
      إذا ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_منزل_sid_map.cmpexch (خالي_اشارو, glyph_منزل_sid_map))
      {
	glyph_منزل_sid_map->~glyph_منزل_sid_map_t ();
	hb_free (glyph_منزل_sid_map);
      }
    }

    منطقي two_byte = subset_charset_ranges.complete (انگ_glyphs);

    size0 = Charset0::get_سائيز (plan->انگ_output_glyphs ());
    إذا (!two_byte)
      سائيز_ranges = Charset1::get_سائيز_for_ranges (subset_charset_ranges.ڊيگهائي);
    وإلا
      سائيز_ranges = Charset2::get_سائيز_for_ranges (subset_charset_ranges.ڊيگهائي);

    إذا (size0 < سائيز_ranges)
      subset_charset_format = 0;
    وإلا إذا (!two_byte)
      subset_charset_format = 1;
    وإلا
      subset_charset_format = 2;

    أرجع سچ;
  }

  منطقي collect_sids_اندريون_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    لكل (بنانشان صحيح i = 0; i < name_dict_قدرون_t::Val_ڳڻپ; i++)
    {
      بنانشان صحيح sid = acc.چوٽي_Dict_2.nameSIDs[i];
      إذا (sid != CFF_UNDEF_SID)
      {
	چوٽي_Dict_Mod_SIDs[i] = sidmap.جوڙيو (sid);
      }
    }

    إذا (acc.fdArray != &Null (CFF1FDArray))
      لكل (بنانشان صحيح i = 0; i < orig_fdcount; i++)
	إذا (fdmap.has (i))
	  (فراغ)sidmap.جوڙيو (acc.fontDicts[i].fontName);

    أرجع سچ;
  }

  منطقي create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) أرجع خطأ;

    انگ_glyphs = plan->انگ_output_glyphs ();
    orig_fdcount = acc.fd_ڳڻپ;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    سڃاڻپ_عنصر_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_سڃاڻپ_عنصر_CHARSET) && acc.is_CID ();

 #جيڪڏهن_وصف_ٿيل HB_EXPERIMENTAL_API
    min_charstrings_off_سائيز = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_سائيز = 0;
 #جيڪڏهن_پورو

    subset_charset = !acc.is_predef_charset () || سڃاڻپ_عنصر_charset;
    إذا (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      لكل (ثابت خودڪار &_ : plan->new_منزل_old_gid_list)
      {
	إذا (_.پهريون != _.second)
	{
	  subset_charset = سچ;
	  اكسر;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.چوٽي_Dict_2);
      منطقي need_منزل_جوڙيو_enc = (subset_encoding && !acc.چوٽي_Dict_2.has_op (Op_ڪوڊ_Encoding));
      منطقي need_منزل_جوڙيو_سيٽ = (subset_charset && !acc.چوٽي_Dict_2.has_op (Op_ڪوڊ_charset));
      إذا (need_منزل_جوڙيو_enc || need_منزل_جوڙيو_سيٽ)
      {
	إذا (need_منزل_جوڙيو_enc)
	  topdict_mod.جوڙيو_op (Op_ڪوڊ_Encoding);
	إذا (need_منزل_جوڙيو_سيٽ)
	  topdict_mod.جوڙيو_op (Op_ڪوڊ_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    إذا (acc.fdSelect != &Null (CFF1FDSelect))
    {
	إذا (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.انگ_glyphs,
				  subset_fdcount,
				  info.fd_select.سائيز,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.سڃاڻپ_عنصر (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      إذا (unlikely (!collect_sids_اندريون_dicts (acc)))
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
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_ڪوڊ_endchar>
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
      لكل (بنانشان صحيح fd = 0; fd < orig_fdcount; fd++)
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
      fontdicts_mod.push (cff1_font_dict_قدرون_mod_t ());
    وإلا
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_قدرون_t &_)
	{ أرجع fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_قدرون_t &_)
	{
	  cff1_font_dict_قدرون_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  أرجع mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    أرجع !plan->اندريون_error () &&
	   (subset_charstrings.ڊيگهائي == plan->انگ_output_glyphs ()) &&
	   (fontdicts_mod.ڊيگهائي == subset_fdcount);
  }

  cff1_چوٽي_dict_قدرون_mod_t	topdict_mod;
  cff1_sub_جدول_info_t		info;

  بنانشان صحيح    انگ_glyphs;
  بنانشان صحيح    orig_fdcount = 0;
  بنانشان صحيح    subset_fdcount = 1;
  بنانشان صحيح    subset_fdselect_format = 0;
  hb_ويڪٽر_t<ڪوڊ_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_ويڪٽر_t<str_buff_vec_t>	subset_localsubrs;
  hb_ويڪٽر_t<cff1_font_dict_قدرون_mod_t>  fontdicts_mod;

  منطقي		drop_hints = خطأ;

  منطقي		gid_renum;
  منطقي		subset_encoding;
  uint8_t	subset_enc_format;
  بنانشان صحيح	subset_enc_انگ_codes;
  range_list_t	subset_enc_ڪوڊ_ranges;
  hb_ويڪٽر_t<ڪوڊ_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقي		subset_charset;

  remap_sid_t	sidmap;
  بنانشان صحيح	چوٽي_Dict_Mod_SIDs[name_dict_قدرون_t::Val_ڳڻپ];

  منطقي		desubroutinize = خطأ;
  منطقي		سڃاڻپ_عنصر_charset = خطأ;

  بنانشان	min_charstrings_off_سائيز = 0;
};
} // namespace OT

ساكن منطقي _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         بنية OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_لڙيون> ();

  بنانشان معلومات_سائيز = 0;
  بنانشان ڪل_سائيز = CFF1_Char_لڙيون::ڪل_سائيز (plan.subset_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز);
  إذا (unlikely (!c->شروعات_zerocopy (ڪل_سائيز)))
    أرجع خطأ;

  خودڪار *cs = c->شروعات_embed<CFF1_Char_لڙيون> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &معلومات_سائيز, plan.min_charstrings_off_سائيز))) {
    c->ڪڍو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڙيون_link = c->ڪڍو_pack (خطأ);
  أرجع سچ;
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
  لكل (صحيح i = (صحيح) ذاتي_Dicts.ڊيگهائي; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      إذا (plan.subset_localsubrs[i].ڊيگهائي > 0)
      {
	خودڪار *dest = c->push <CFF1Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->ڪڍو_pack ();
	وإلا
	{
	  c->ڪڍو_discard ();
	  أرجع خطأ;
	}
      }

      خودڪار *pd = c->push<ذاتي_Dict> ();
      cff1_ذاتي_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      إذا (likely (pd->serialize (c, ذاتي_Dicts[i], privSzr, subrs_link)))
      {
	بنانشان fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].ذاتي_Dict_Info.سائيز = c->ڊيگهائي ();
	plan.fontdicts_mod[fd].ذاتي_Dict_Info.link = c->ڪڍو_pack ();
      }
      وإلا
      {
	c->ڪڍو_discard ();
	أرجع خطأ;
      }
    }
  }

  إذا (!is_CID ())
    plan.info.ذاتي_Dict_Info = plan.fontdicts_mod[0].ذاتي_Dict_Info;

  /* FDArray (FD Index) */
  إذا (fdArray != &Null (CFF1FDArray))
  {
    خودڪار *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    خودڪار it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    إذا (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->ڪڍو_pack (خطأ);
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.انگ_glyphs, *fdSelect, fd_ڳڻپ,
					   plan.subset_fdselect_format, plan.info.fd_select.سائيز,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* Charset */
  إذا (plan.subset_charset)
  {
    خودڪار *dest = c->push<Charset> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.انگ_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* Encoding */
  إذا (plan.subset_encoding)
  {
    خودڪار *dest = c->push<Encoding> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_انگ_codes,
				 plan.subset_enc_ڪوڊ_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* global subrs */
  {
    خودڪار *dest = c->push <CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->ڪڍو_pack (خطأ);
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  /* String INDEX */
  {
    خودڪار *dest = c->push<CFF1_String_اشاريو> ();
    إذا (likely (!plan.sidmap.اندريون_error () &&
		dest->serialize (c, *string_اشاريو, plan.sidmap.vector)))
      c->ڪڍو_pack ();
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff))
    أرجع خطأ;

  /*سرو*/
  cff->نسخو.major = 0x01;
  cff->نسخو.minor = 0x00;
  cff->name_اشاريو = cff->min_سائيز;
  cff->off_سائيز = 4; /* unused? */

  /* name INDEX */
  إذا (unlikely (!c->embed (*name_اشاريو))) أرجع خطأ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    خودڪار *چوٽي_2 = c->push<چوٽي_Dict> ();
    cff1_چوٽي_dict_op_serializer_t چوٽي_Szr;
    بنانشان چوٽي_سائيز = 0;
    چوٽي_dict_modifiers_t  modifier (plan.info, plan.چوٽي_Dict_Mod_SIDs);
    إذا (likely (چوٽي_2->serialize (c, plan.topdict_mod, چوٽي_Szr, modifier)))
    {
      چوٽي_سائيز = c->ڊيگهائي ();
      c->ڪڍو_pack (خطأ);
    }
    وإلا
    {
      c->ڪڍو_discard ();
      أرجع خطأ;
    }
    /* serialize INDEX header for above */
    خودڪار *dest = c->شروعات_embed<CFF1_اشاريو> ();
    أرجع dest->serialize_سرو (c, hb_iter (&چوٽي_سائيز, 1), چوٽي_سائيز);
  }
}

منطقي
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  إذا (unlikely (!cff_plan.create (*هذا, c->plan)))
  {
    DEBUG_نياپو(SUBSET, خالي_اشارو, "Failed to generate a cff subsetting plan.");
    أرجع خطأ;
  }

  أرجع serialize (c->serializer, cff_plan);
}


#جيڪڏهن_پورو
