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

#جے_بنا_تعریف HB_NO_SUBSET_CFF

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-set.h"
#ضمن "hb-bimap.hh"
#ضمن "hb-subset-plan.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff1-interp-cs.hh"

ورتوں نطاق CFF;

بنية remap_sid_t
{
  بنانشانی get_population () ثابت { أرجع vector.لمائی; }

  فراغ alloc (بنانشانی ناپ)
  {
    map.alloc (ناپ);
    vector.alloc_exact (ناپ);
  }

  منطقي وچ_error () ثابت
  { أرجع map.وچ_error () || vector.وچ_error (); }

  بنانشانی صحيح جوڑو (بنانشانی صحيح sid)
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    sid = unoffset_sid (sid);
    بنانشانی v = next;
    إذا (map.سیٹ (sid, v, خطأ))
    {
      vector.push (sid);
      next++;
    }
    وإلا
      v = map.get (sid); // already exists
    أرجع وتھ_sid (v);
  }

  بنانشانی صحيح عامل[] (بنانشانی صحيح sid) ثابت
  {
    إذا (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      أرجع sid;

    أرجع وتھ_sid (map.get (unoffset_sid (sid)));
  }

  ساكن ثابت بنانشانی صحيح عدد_std_لڑیاں = 391;

  ساكن منطقي is_std_str (بنانشانی صحيح sid) { أرجع sid < عدد_std_لڑیاں; }
  ساكن بنانشانی صحيح وتھ_sid (بنانشانی صحيح sid) { أرجع sid + عدد_std_لڑیاں; }
  ساكن بنانشانی صحيح unoffset_sid (بنانشانی صحيح sid) { أرجع sid - عدد_std_لڑیاں; }
  بنانشانی next = 0;

  hb_map_t map;
  hb_سمتیہ_t<بنانشانی> vector;
};

بنية cff1_sub_جدول_info_t : cff_sub_جدول_info_t
{
  cff1_sub_جدول_info_t ()
    : cff_sub_جدول_info_t (),
      encoding_link (0),
      charset_link (0)
   {
    نجی_Dict_Info.init ();
  }

  objidx_t	encoding_link;
  objidx_t	charset_link;
  جدول_info_t	نجی_Dict_Info;
};

/* a copy of a parsed out cff1_top_dict_values_t augmented with additional operators */
بنية cff1_چوٹی_dict_اقدار_mod_t : cff1_چوٹی_dict_اقدار_t
{
  فراغ init (ثابت cff1_چوٹی_dict_اقدار_t *ادھار_2= &Null (cff1_چوٹی_dict_اقدار_t))
  {
    SUPER::init ();
    ادھار = ادھار_2;
  }

  فراغ fini () { SUPER::fini (); }

  بنانشانی get_گنتی () ثابت { أرجع ادھار->get_گنتی () + SUPER::get_گنتی (); }
  ثابت cff1_چوٹی_dict_val_t &get_قدر (بنانشانی صحيح i) ثابت
  {
    إذا (i < ادھار->get_گنتی ())
      أرجع (*ادھار)[i];
    وإلا
      أرجع SUPER::values[i - ادھار->get_گنتی ()];
  }
  ثابت cff1_چوٹی_dict_val_t &عامل [] (بنانشانی صحيح i) ثابت { أرجع get_قدر (i); }

  فراغ reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    لكل (بنانشانی صحيح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
      nameSIDs[i] = sidmap[ادھار->nameSIDs[i]];
  }

  محمي:
  تعريفنوع cff1_چوٹی_dict_اقدار_t SUPER;
  ثابت cff1_چوٹی_dict_اقدار_t *ادھار;
};

بنية چوٹی_dict_modifiers_t
{
  چوٹی_dict_modifiers_t (ثابت cff1_sub_جدول_info_t &info_,
			ثابت بنانشانی صحيح (&nameSIDs_)[name_dict_اقدار_t::Val_گنتی])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_جدول_info_t &info;
  ثابت بنانشانی صحيح	(&nameSIDs)[name_dict_اقدار_t::Val_گنتی];
};

بنية cff1_چوٹی_dict_op_serializer_t : cff_چوٹی_dict_op_serializer_t<cff1_چوٹی_dict_val_t>
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت cff1_چوٹی_dict_val_t &opstr,
		  ثابت چوٹی_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    op_کوڈ_t op = opstr.op;
    بدل (op)
    {
      حالة Op_کوڈ_charset:
	إذا (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پٹھ;

      حالة Op_کوڈ_Encoding:
	إذا (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	وإلا
	  اذهب fall_پٹھ;

      حالة Op_کوڈ_نجی:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.نجی_Dict_Info.ناپ) &&
		      Dict::serialize_link4_op (c, op, mod.info.نجی_Dict_Info.link, whence_t::Absolute));

      حالة Op_کوڈ_نسخہ:
      حالة Op_کوڈ_Notice:
      حالة Op_کوڈ_Copyright:
      حالة Op_کوڈ_بھریا_Name:
      حالة Op_کوڈ_Family_Name:
      حالة Op_کوڈ_بھار:
      حالة Op_کوڈ_Post_Script:
      حالة Op_کوڈ_ادھار_Font_Name:
      حالة Op_کوڈ_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_اقدار_t::name_op_منزل_اشاریہ (op)]));

      حالة Op_کوڈ_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  إذا ( unlikely (!(opstr.لمائی >= opstr.آخری_arg_وتھ + 3)))
	    return_trace (خطأ);
	  supp_op.ptr = opstr.ptr + opstr.آخری_arg_وتھ;
	  supp_op.لمائی = opstr.لمائی - opstr.آخری_arg_وتھ;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_اقدار_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_اقدار_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_پٹھ:
      افتراضي:
	return_trace (cff_چوٹی_dict_op_serializer_t<cff1_چوٹی_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (سچ);
  }

};

بنية cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_اقدار_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == Op_کوڈ_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    وإلا
      return_trace (SUPER::serialize (c, opstr, mod.نجی_Dict_Info));
  }

  خاص:
  تعريفنوع cff_font_dict_op_serializer_t SUPER;
};

بنية cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساكن فراغ flush_دلیلاں_and_op (op_کوڈ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    إذا (env.arg_شروع > 0)
      flush_چوڑائی (env, param);

    بدل (op)
    {
      حالة Op_کوڈ_hstem:
      حالة Op_کوڈ_hstemhm:
      حالة Op_کوڈ_vstem:
      حالة Op_کوڈ_vstemhm:
      حالة Op_کوڈ_hintmask:
      حالة Op_کوڈ_cntrmask:
      حالة Op_کوڈ_dotsection:
	إذا (param.drop_hints)
	{
	  env.clear_دلیلاں ();
	  أرجع;
	}
	HB_FALLTHROUGH;

      افتراضي:
	SUPER::flush_دلیلاں_and_op (op, env, param);
	اكسر;
    }
  }
  ساكن فراغ flush_دلیلاں (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    لكل (بنانشانی صحيح i = env.arg_شروع; i < env.arg_ڈھیر.get_گنتی (); i++)
      encoder.encode_عدد_cs (env.eval_arg (i));
    SUPER::flush_دلیلاں (env, param);
  }

  ساكن فراغ flush_op (op_کوڈ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساكن فراغ flush_چوڑائی (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_چوڑائی);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_عدد_cs (env.چوڑائی);
  }

  ساكن فراغ flush_hintmask (op_کوڈ_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    إذا (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      لكل (بنانشانی صحيح i = 0; i < env.hintmask_ناپ; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

بنية range_list_t : hb_سمتیہ_t<کوڈ_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقي complete (بنانشانی صحيح آخری_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    بنانشانی گنتی = هذا->لمائی;
    لكل (بنانشانی صحيح i = گنتی; i; i--)
    {
      کوڈ_pair_t &pair = arrayZ[i - 1];
      بنانشانی صحيح n_بایاں = آخری_glyph - pair.glyph - 1;
      all_glyphs |= n_بایاں;
      آخری_glyph = pair.glyph;
      pair.glyph = n_بایاں;
    }
    منطقي two_byte = all_glyphs >= 0x100;
    أرجع two_byte;
  }
};

بنية cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساكن فراغ process_op (op_کوڈ_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    بدل (op) {

      حالة Op_کوڈ_return:
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	env.return_from_subr ();
	param.سیٹ_current_str (env, خطأ);
	اكسر;

      حالة Op_کوڈ_endchar:
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	SUPER::process_op (op, env, param);
	اكسر;

      حالة Op_کوڈ_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_گھیرا_2);
	اكسر;

      حالة Op_کوڈ_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_گھیرا);
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	اكسر;
    }
  }

  محمي:
  ساكن فراغ process_call_subr (op_کوڈ_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_سیٹ_t *گھیرا)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڑو_call_op (op, str_ref, env.context.subr_عدد_2);
    گھیرا->جوڑو (env.context.subr_عدد_2);
    param.سیٹ_current_str (env, سچ);
  }

  خاص:
  تعريفنوع cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

بنية cff1_نجی_dict_op_serializer_t : op_serializer_t
{
  cff1_نجی_dict_op_serializer_t (منطقي desubroutinize_, منطقي drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    إذا (opstr.op == Op_کوڈ_Subrs)
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

بنية cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_کوڈ_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساكن فراغ complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    إذا (env.has_چوڑائی)
      charstring.سیٹ_اگیتر (env.چوڑائی);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->سیٹ_parsed ();
    لكل (بنانشانی صحيح i = 0; i < env.call_ڈھیر.get_گنتی (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ڈھیر[i]);
      إذا (likely (parsed_str))
	parsed_str->سیٹ_parsed ();
      وإلا
	env.سیٹ_error ();
    }
  }
};

نطاق OT {
بنية cff1_subset_plan
{
  cff1_subset_plan ()
  {
    لكل (بنانشانی صحيح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
      چوٹی_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  فراغ plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    بنانشانی صحيح  size0, size1;
    بنانشانی کوڈ, آخری_کوڈ_2 = CFF_UNDEF_کوڈ - 1;
    hb_سمتیہ_t<hb_codepoint_t> supp_codes;

    subset_enc_کوڈ_ranges.clear ();
    إذا (unlikely (subset_enc_کوڈ_ranges.وچ_error ()))
    {
      plan->check_success (خطأ);
      أرجع;
    }

    supp_codes.init ();

    کوڈ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_عدد_codes = plan->عدد_output_glyphs () - 1;
    بنانشانی صحيح glyph;
    خودکار it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->پہلا == 0) it++;
    خودکار _ = *it;
    لكل (glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.پہلا)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      کوڈ = acc.glyph_منزل_کوڈ (old_glyph, &glyph_منزل_sid_cache);
      إذا (کوڈ == CFF_UNDEF_کوڈ)
      {
	subset_enc_عدد_codes = glyph - 1;
	اكسر;
      }

      إذا (کوڈ != آخری_کوڈ_2 + 1)
	subset_enc_کوڈ_ranges.push (کوڈ_pair_t {کوڈ, glyph});
      آخری_کوڈ_2 = کوڈ;

      إذا (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	لكل (بنانشانی صحيح i = 0; i < supp_codes.لمائی; i++)
	  subset_enc_supp_codes.push (کوڈ_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_کوڈ_ranges.complete (glyph);

    أكد (subset_enc_عدد_codes <= 0xFF);
    size0 = Encoding0::min_ناپ + HBUINT8::static_ناپ * subset_enc_عدد_codes;
    size1 = Encoding1::min_ناپ + Encoding1_Range::static_ناپ * subset_enc_کوڈ_ranges.لمائی;

    إذا (size0 < size1)
      subset_enc_format = 0;
    وإلا
      subset_enc_format = 1;
  }

  منطقي plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    بنانشانی صحيح  size0, ناپ_ranges;
    بنانشانی آخری_sid = CFF_UNDEF_کوڈ - 1;

    subset_charset_ranges.clear ();
    إذا (unlikely (subset_charset_ranges.وچ_error ()))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    کوڈ_pair_t glyph_منزل_sid_cache {0, HB_CODEPOINT_INVALID};

    بنانشانی عدد_glyphs = plan->عدد_output_glyphs ();

    إذا (unlikely (!subset_charset_ranges.alloc (hb_min (عدد_glyphs,
							acc.عدد_charset_entries))))
    {
      plan->check_success (خطأ);
      أرجع خطأ;
    }

    glyph_منزل_sid_map_t *glyph_منزل_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_منزل_sid_map.get_acquire () :
					   خالی_سوچک;

    منطقي created_map = خطأ;
    إذا (!glyph_منزل_sid_map && acc.cff_accelerator)
    {
      created_map = سچ;
      glyph_منزل_sid_map = acc.create_glyph_منزل_sid_map ();
    }

    إذا (!glyph_منزل_sid_map || glyph_منزل_sid_map->وچ_error ())
    {
      glyph_منزل_sid_map = خالی_سوچک;
      created_map = خطأ;
    }

    خودکار it = hb_iter (plan->new_منزل_old_gid_list);
    إذا (it->پہلا == 0) it++;
    خودکار _ = *it;
    منطقي not_is_cid = !acc.is_CID ();
    منطقي skip = !not_is_cid && glyph_منزل_sid_map && !پچھان_تت_charset;
    إذا (not_is_cid)
      sidmap.alloc (عدد_glyphs);
    لكل (hb_codepoint_t glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      إذا (glyph == _.پہلا)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      وإلا
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      بنانشانی sid;
      إذا (پچھان_تت_charset)
	sid = glyph;
      وإلا
      {
	sid = glyph_منزل_sid_map ?
		       glyph_منزل_sid_map->arrayZ[old_glyph].کوڈ :
		       acc.glyph_منزل_sid (old_glyph, &glyph_منزل_sid_cache);

	إذا (not_is_cid)
	  sid = sidmap.جوڑو (sid);
      }

      إذا (sid != آخری_sid + 1)
	subset_charset_ranges.push (کوڈ_pair_t {sid, glyph});

      إذا (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.پہلا - 1, glyph_منزل_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      آخری_sid = sid;
    }

    إذا (created_map)
    {
      إذا ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_منزل_sid_map.cmpexch (خالی_سوچک, glyph_منزل_sid_map))
      {
	glyph_منزل_sid_map->~glyph_منزل_sid_map_t ();
	hb_free (glyph_منزل_sid_map);
      }
    }

    منطقي two_byte = subset_charset_ranges.complete (عدد_glyphs);

    size0 = Charset0::get_ناپ (plan->عدد_output_glyphs ());
    إذا (!two_byte)
      ناپ_ranges = Charset1::get_ناپ_for_ranges (subset_charset_ranges.لمائی);
    وإلا
      ناپ_ranges = Charset2::get_ناپ_for_ranges (subset_charset_ranges.لمائی);

    إذا (size0 < ناپ_ranges)
      subset_charset_format = 0;
    وإلا إذا (!two_byte)
      subset_charset_format = 1;
    وإلا
      subset_charset_format = 2;

    أرجع سچ;
  }

  منطقي collect_sids_وچ_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    لكل (بنانشانی صحيح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
    {
      بنانشانی صحيح sid = acc.چوٹی_Dict_2.nameSIDs[i];
      إذا (sid != CFF_UNDEF_SID)
      {
	چوٹی_Dict_Mod_SIDs[i] = sidmap.جوڑو (sid);
      }
    }

    إذا (acc.fdArray != &Null (CFF1FDArray))
      لكل (بنانشانی صحيح i = 0; i < orig_fdcount; i++)
	إذا (fdmap.has (i))
	  (فراغ)sidmap.جوڑو (acc.fontDicts[i].fontName);

    أرجع سچ;
  }

  منطقي create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    إذا (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) أرجع خطأ;

    عدد_glyphs = plan->عدد_output_glyphs ();
    orig_fdcount = acc.fd_گنتی;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    پچھان_تت_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_پچھان_تت_CHARSET) && acc.is_CID ();

 #جے_تعریف_شدہ HB_EXPERIMENTAL_API
    min_charstrings_off_ناپ = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #وإلا
    min_charstrings_off_ناپ = 0;
 #جے_انت

    subset_charset = !acc.is_predef_charset () || پچھان_تت_charset;
    إذا (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      لكل (ثابت خودکار &_ : plan->new_منزل_old_gid_list)
      {
	إذا (_.پہلا != _.second)
	{
	  subset_charset = سچ;
	  اكسر;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.چوٹی_Dict_2);
      منطقي need_منزل_جوڑو_enc = (subset_encoding && !acc.چوٹی_Dict_2.has_op (Op_کوڈ_Encoding));
      منطقي need_منزل_جوڑو_سیٹ = (subset_charset && !acc.چوٹی_Dict_2.has_op (Op_کوڈ_charset));
      إذا (need_منزل_جوڑو_enc || need_منزل_جوڑو_سیٹ)
      {
	إذا (need_منزل_جوڑو_enc)
	  topdict_mod.جوڑو_op (Op_کوڈ_Encoding);
	إذا (need_منزل_جوڑو_سیٹ)
	  topdict_mod.جوڑو_op (Op_کوڈ_charset);
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
				  info.fd_select.ناپ,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	أرجع خطأ;
    }
    وإلا
      fdmap.پچھان_تت (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      إذا (unlikely (!collect_sids_وچ_dicts (acc)))
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
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_کوڈ_endchar>
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
      لكل (بنانشانی صحيح fd = 0; fd < orig_fdcount; fd++)
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
      fontdicts_mod.push (cff1_font_dict_اقدار_mod_t ());
    وإلا
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_اقدار_t &_)
	{ أرجع fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_اقدار_t &_)
	{
	  cff1_font_dict_اقدار_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  أرجع mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    أرجع !plan->وچ_error () &&
	   (subset_charstrings.لمائی == plan->عدد_output_glyphs ()) &&
	   (fontdicts_mod.لمائی == subset_fdcount);
  }

  cff1_چوٹی_dict_اقدار_mod_t	topdict_mod;
  cff1_sub_جدول_info_t		info;

  بنانشانی صحيح    عدد_glyphs;
  بنانشانی صحيح    orig_fdcount = 0;
  بنانشانی صحيح    subset_fdcount = 1;
  بنانشانی صحيح    subset_fdselect_format = 0;
  hb_سمتیہ_t<کوڈ_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_سمتیہ_t<str_buff_vec_t>	subset_localsubrs;
  hb_سمتیہ_t<cff1_font_dict_اقدار_mod_t>  fontdicts_mod;

  منطقي		drop_hints = خطأ;

  منطقي		gid_renum;
  منطقي		subset_encoding;
  uint8_t	subset_enc_format;
  بنانشانی صحيح	subset_enc_عدد_codes;
  range_list_t	subset_enc_کوڈ_ranges;
  hb_سمتیہ_t<کوڈ_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقي		subset_charset;

  remap_sid_t	sidmap;
  بنانشانی صحيح	چوٹی_Dict_Mod_SIDs[name_dict_اقدار_t::Val_گنتی];

  منطقي		desubroutinize = خطأ;
  منطقي		پچھان_تت_charset = خطأ;

  بنانشانی	min_charstrings_off_ناپ = 0;
};
} // namespace OT

ساكن منطقي _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         بنية OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_لڑیاں> ();

  بنانشانی انکڑے_ناپ = 0;
  بنانشانی کل_ناپ = CFF1_Char_لڑیاں::کل_ناپ (plan.subset_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ);
  إذا (unlikely (!c->شروع_zerocopy (کل_ناپ)))
    أرجع خطأ;

  خودکار *cs = c->شروع_embed<CFF1_Char_لڑیاں> ();
  إذا (unlikely (!cs->serialize (c, plan.subset_charstrings, &انکڑے_ناپ, plan.min_charstrings_off_ناپ))) {
    c->نکالو_discard ();
    أرجع خطأ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (خطأ);
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
  لكل (صحيح i = (صحيح) نجی_Dicts.لمائی; --i >= 0 ;)
  {
    إذا (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      إذا (plan.subset_localsubrs[i].لمائی > 0)
      {
	خودکار *dest = c->push <CFF1Subrs> ();
	إذا (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->نکالو_pack ();
	وإلا
	{
	  c->نکالو_discard ();
	  أرجع خطأ;
	}
      }

      خودکار *pd = c->push<نجی_Dict> ();
      cff1_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      إذا (likely (pd->serialize (c, نجی_Dicts[i], privSzr, subrs_link)))
      {
	بنانشانی fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].نجی_Dict_Info.ناپ = c->لمائی ();
	plan.fontdicts_mod[fd].نجی_Dict_Info.link = c->نکالو_pack ();
      }
      وإلا
      {
	c->نکالو_discard ();
	أرجع خطأ;
      }
    }
  }

  إذا (!is_CID ())
    plan.info.نجی_Dict_Info = plan.fontdicts_mod[0].نجی_Dict_Info;

  /* FDArray (FD Index) */
  إذا (fdArray != &Null (CFF1FDArray))
  {
    خودکار *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    خودکار it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    إذا (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->نکالو_pack (خطأ);
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* FDSelect */
  إذا (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    إذا (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs, *fdSelect, fd_گنتی,
					   plan.subset_fdselect_format, plan.info.fd_select.ناپ,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* Charset */
  إذا (plan.subset_charset)
  {
    خودکار *dest = c->push<Charset> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.عدد_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* Encoding */
  إذا (plan.subset_encoding)
  {
    خودکار *dest = c->push<Encoding> ();
    إذا (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_عدد_codes,
				 plan.subset_enc_کوڈ_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* global subrs */
  {
    خودکار *dest = c->push <CFF1Subrs> ();
    إذا (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->نکالو_pack (خطأ);
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  /* String INDEX */
  {
    خودکار *dest = c->push<CFF1_String_اشاریہ> ();
    إذا (likely (!plan.sidmap.وچ_error () &&
		dest->serialize (c, *string_اشاریہ, plan.sidmap.vector)))
      c->نکالو_pack ();
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  إذا (unlikely (!cff))
    أرجع خطأ;

  /*سرخی*/
  cff->نسخہ.major = 0x01;
  cff->نسخہ.minor = 0x00;
  cff->name_اشاریہ = cff->min_ناپ;
  cff->off_ناپ = 4; /* unused? */

  /* name INDEX */
  إذا (unlikely (!c->embed (*name_اشاریہ))) أرجع خطأ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    خودکار *چوٹی_2 = c->push<چوٹی_Dict> ();
    cff1_چوٹی_dict_op_serializer_t چوٹی_Szr;
    بنانشانی چوٹی_ناپ = 0;
    چوٹی_dict_modifiers_t  modifier (plan.info, plan.چوٹی_Dict_Mod_SIDs);
    إذا (likely (چوٹی_2->serialize (c, plan.topdict_mod, چوٹی_Szr, modifier)))
    {
      چوٹی_ناپ = c->لمائی ();
      c->نکالو_pack (خطأ);
    }
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
    /* serialize INDEX header for above */
    خودکار *dest = c->شروع_embed<CFF1_اشاریہ> ();
    أرجع dest->serialize_سرخی (c, hb_iter (&چوٹی_ناپ, 1), چوٹی_ناپ);
  }
}

منطقي
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  إذا (unlikely (!cff_plan.create (*هذا, c->plan)))
  {
    DEBUG_پیغام(SUBSET, خالی_سوچک, "Failed to generate a cff subsetting plan.");
    أرجع خطأ;
  }

  أرجع serialize (c->serializer, cff_plan);
}


#جے_انت
