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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF

#شامل "hb-open-type.hh"
#شامل "hb-ot-cff1-table.hh"
#شامل "hb-set.h"
#شامل "hb-bimap.hh"
#شامل "hb-subset-plan.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-cff1-interp-cs.hh"

استعمال نامخلا CFF;

ساخت remap_sid_t
{
  غیرعلامتی get_population () ثابت { واپس vector.لمبائی; }

  خالی alloc (غیرعلامتی سائز)
  {
    map.alloc (سائز);
    vector.alloc_exact (سائز);
  }

  منطقی اندر_error () ثابت
  { واپس map.اندر_error () || vector.اندر_error (); }

  غیرعلامتی صحیح جوڑو (غیرعلامتی صحیح sid)
  {
    اگر (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      واپس sid;

    sid = unoffset_sid (sid);
    غیرعلامتی v = next;
    اگر (map.سیٹ (sid, v, جھوٹ))
    {
      vector.push (sid);
      next++;
    }
    ورنہ
      v = map.get (sid); // already exists
    واپس انحراف_sid (v);
  }

  غیرعلامتی صحیح عامل[] (غیرعلامتی صحیح sid) ثابت
  {
    اگر (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      واپس sid;

    واپس انحراف_sid (map.get (unoffset_sid (sid)));
  }

  ساکن ثابت غیرعلامتی صحیح عدد_std_لڑیاں = 391;

  ساکن منطقی is_std_str (غیرعلامتی صحیح sid) { واپس sid < عدد_std_لڑیاں; }
  ساکن غیرعلامتی صحیح انحراف_sid (غیرعلامتی صحیح sid) { واپس sid + عدد_std_لڑیاں; }
  ساکن غیرعلامتی صحیح unoffset_sid (غیرعلامتی صحیح sid) { واپس sid - عدد_std_لڑیاں; }
  غیرعلامتی next = 0;

  hb_map_t map;
  hb_سمتیہ_t<غیرعلامتی> vector;
};

ساخت cff1_sub_جدول_info_t : cff_sub_جدول_info_t
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
ساخت cff1_چوٹی_dict_اقدار_mod_t : cff1_چوٹی_dict_اقدار_t
{
  خالی init (ثابت cff1_چوٹی_dict_اقدار_t *بنیاد_2= &Null (cff1_چوٹی_dict_اقدار_t))
  {
    SUPER::init ();
    بنیاد = بنیاد_2;
  }

  خالی fini () { SUPER::fini (); }

  غیرعلامتی get_گنتی () ثابت { واپس بنیاد->get_گنتی () + SUPER::get_گنتی (); }
  ثابت cff1_چوٹی_dict_val_t &get_قدر (غیرعلامتی صحیح i) ثابت
  {
    اگر (i < بنیاد->get_گنتی ())
      واپس (*بنیاد)[i];
    ورنہ
      واپس SUPER::values[i - بنیاد->get_گنتی ()];
  }
  ثابت cff1_چوٹی_dict_val_t &عامل [] (غیرعلامتی صحیح i) ثابت { واپس get_قدر (i); }

  خالی reassignSIDs (ثابت remap_sid_t& sidmap)
  {
    برائے (غیرعلامتی صحیح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
      nameSIDs[i] = sidmap[بنیاد->nameSIDs[i]];
  }

  محفوظ:
  قسمتعریف cff1_چوٹی_dict_اقدار_t SUPER;
  ثابت cff1_چوٹی_dict_اقدار_t *بنیاد;
};

ساخت چوٹی_dict_modifiers_t
{
  چوٹی_dict_modifiers_t (ثابت cff1_sub_جدول_info_t &info_,
			ثابت غیرعلامتی صحیح (&nameSIDs_)[name_dict_اقدار_t::Val_گنتی])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  ثابت cff1_sub_جدول_info_t &info;
  ثابت غیرعلامتی صحیح	(&nameSIDs)[name_dict_اقدار_t::Val_گنتی];
};

ساخت cff1_چوٹی_dict_op_serializer_t : cff_چوٹی_dict_op_serializer_t<cff1_چوٹی_dict_val_t>
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت cff1_چوٹی_dict_val_t &opstr,
		  ثابت چوٹی_dict_modifiers_t &mod) ثابت
  {
    TRACE_SERIALIZE (یہ);

    op_رمز_t op = opstr.op;
    انتخاب (op)
    {
      حالت Op_رمز_charset:
	اگر (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	ورنہ
	  جاؤ fall_عقب;

      حالت Op_رمز_Encoding:
	اگر (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	ورنہ
	  جاؤ fall_عقب;

      حالت Op_رمز_نجی:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.نجی_Dict_Info.سائز) &&
		      Dict::serialize_link4_op (c, op, mod.info.نجی_Dict_Info.link, whence_t::Absolute));

      حالت Op_رمز_نسخہ:
      حالت Op_رمز_Notice:
      حالت Op_رمز_Copyright:
      حالت Op_رمز_بھرا_Name:
      حالت Op_رمز_Family_Name:
      حالت Op_رمز_وزن:
      حالت Op_رمز_Post_Script:
      حالت Op_رمز_بنیاد_Font_Name:
      حالت Op_رمز_Font_Name:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_اقدار_t::name_op_مقصد_اشاریہ (op)]));

      حالت Op_رمز_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  اگر ( unlikely (!(opstr.لمبائی >= opstr.آخری_arg_انحراف + 3)))
	    return_trace (جھوٹ);
	  supp_op.ptr = opstr.ptr + opstr.آخری_arg_انحراف;
	  supp_op.لمبائی = opstr.لمبائی - opstr.آخری_arg_انحراف;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_اقدار_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_اقدار_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_عقب:
      طےشدہ:
	return_trace (cff_چوٹی_dict_op_serializer_t<cff1_چوٹی_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (سچ);
  }

};

ساخت cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت cff1_font_dict_اقدار_mod_t &mod) ثابت
  {
    TRACE_SERIALIZE (یہ);

    اگر (opstr.op == Op_رمز_Font_Name)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    ورنہ
      return_trace (SUPER::serialize (c, opstr, mod.نجی_Dict_Info));
  }

  نجی:
  قسمتعریف cff_font_dict_op_serializer_t SUPER;
};

ساخت cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  ساکن خالی flush_دلائل_and_op (op_رمز_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    اگر (env.arg_آغاز > 0)
      flush_چوڑائی (env, param);

    انتخاب (op)
    {
      حالت Op_رمز_hstem:
      حالت Op_رمز_hstemhm:
      حالت Op_رمز_vstem:
      حالت Op_رمز_vstemhm:
      حالت Op_رمز_hintmask:
      حالت Op_رمز_cntrmask:
      حالت Op_رمز_dotsection:
	اگر (param.drop_hints)
	{
	  env.clear_دلائل ();
	  واپس;
	}
	HB_FALLTHROUGH;

      طےشدہ:
	SUPER::flush_دلائل_and_op (op, env, param);
	توڑو;
    }
  }
  ساکن خالی flush_دلائل (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    برائے (غیرعلامتی صحیح i = env.arg_آغاز; i < env.arg_ڈھیر.get_گنتی (); i++)
      encoder.encode_عدد_cs (env.eval_arg (i));
    SUPER::flush_دلائل (env, param);
  }

  ساکن خالی flush_op (op_رمز_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  ساکن خالی flush_چوڑائی (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    أكد (env.has_چوڑائی);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_عدد_cs (env.چوڑائی);
  }

  ساکن خالی flush_hintmask (op_رمز_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    اگر (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      برائے (غیرعلامتی صحیح i = 0; i < env.hintmask_سائز; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  نجی:
  قسمتعریف cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

ساخت range_list_t : hb_سمتیہ_t<رمز_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  منطقی complete (غیرعلامتی صحیح آخری_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    غیرعلامتی گنتی = یہ->لمبائی;
    برائے (غیرعلامتی صحیح i = گنتی; i; i--)
    {
      رمز_pair_t &pair = arrayZ[i - 1];
      غیرعلامتی صحیح n_بایاں = آخری_glyph - pair.glyph - 1;
      all_glyphs |= n_بایاں;
      آخری_glyph = pair.glyph;
      pair.glyph = n_بایاں;
    }
    منطقی two_byte = all_glyphs >= 0x100;
    واپس two_byte;
  }
};

ساخت cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  ساکن خالی process_op (op_رمز_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    انتخاب (op) {

      حالت Op_رمز_return:
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	env.return_from_subr ();
	param.سیٹ_current_str (env, جھوٹ);
	توڑو;

      حالت Op_رمز_endchar:
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	param.current_parsed_str->سیٹ_parsed ();
	SUPER::process_op (op, env, param);
	توڑو;

      حالت Op_رمز_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_احاطہ_2);
	توڑو;

      حالت Op_رمز_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_احاطہ);
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->جوڑو_op (op, env.str_ref);
	توڑو;
    }
  }

  محفوظ:
  ساکن خالی process_call_subr (op_رمز_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_سیٹ_t *احاطہ)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->جوڑو_call_op (op, str_ref, env.context.subr_عدد_2);
    احاطہ->جوڑو (env.context.subr_عدد_2);
    param.سیٹ_current_str (env, سچ);
  }

  نجی:
  قسمتعریف cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

ساخت cff1_نجی_dict_op_serializer_t : op_serializer_t
{
  cff1_نجی_dict_op_serializer_t (منطقی desubroutinize_, منطقی drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  objidx_t subrs_link) ثابت
  {
    TRACE_SERIALIZE (یہ);

    اگر (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (سچ);

    اگر (opstr.op == Op_رمز_Subrs)
    {
      اگر (desubroutinize || !subrs_link)
	return_trace (سچ);
      ورنہ
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  محفوظ:
  ثابت منطقی desubroutinize;
  ثابت منطقی drop_hints;
};

ساخت cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, Op_رمز_endchar>
{
  cff1_subr_subsetter_t (ثابت OT::cff1::accelerator_subset_t &acc_, ثابت hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  ساکن خالی complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    اگر (env.has_چوڑائی)
      charstring.سیٹ_سابقہ (env.چوڑائی);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->سیٹ_parsed ();
    برائے (غیرعلامتی صحیح i = 0; i < env.call_ڈھیر.get_گنتی (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.call_ڈھیر[i]);
      اگر (likely (parsed_str))
	parsed_str->سیٹ_parsed ();
      ورنہ
	env.سیٹ_error ();
    }
  }
};

نامخلا OT {
ساخت cff1_subset_plan
{
  cff1_subset_plan ()
  {
    برائے (غیرعلامتی صحیح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
      چوٹی_Dict_Mod_SIDs[i] = CFF_UNDEF_SID;
  }

  خالی plan_subset_encoding (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    ثابت Encoding *encoding = acc.encoding;
    غیرعلامتی صحیح  size0, size1;
    غیرعلامتی رمز, آخری_رمز_2 = CFF_UNDEF_رمز - 1;
    hb_سمتیہ_t<hb_codepoint_t> supp_codes;

    subset_enc_رمز_ranges.clear ();
    اگر (unlikely (subset_enc_رمز_ranges.اندر_error ()))
    {
      plan->check_success (جھوٹ);
      واپس;
    }

    supp_codes.init ();

    رمز_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_عدد_codes = plan->عدد_output_glyphs () - 1;
    غیرعلامتی صحیح glyph;
    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    اگر (it->پہلا == 0) it++;
    خودکار _ = *it;
    برائے (glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      اگر (glyph == _.پہلا)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ورنہ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      رمز = acc.glyph_مقصد_رمز (old_glyph, &glyph_مقصد_sid_cache);
      اگر (رمز == CFF_UNDEF_رمز)
      {
	subset_enc_عدد_codes = glyph - 1;
	توڑو;
      }

      اگر (رمز != آخری_رمز_2 + 1)
	subset_enc_رمز_ranges.push (رمز_pair_t {رمز, glyph});
      آخری_رمز_2 = رمز;

      اگر (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_مقصد_sid (old_glyph, &glyph_مقصد_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	برائے (غیرعلامتی صحیح i = 0; i < supp_codes.لمبائی; i++)
	  subset_enc_supp_codes.push (رمز_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_رمز_ranges.complete (glyph);

    أكد (subset_enc_عدد_codes <= 0xFF);
    size0 = Encoding0::min_سائز + HBUINT8::static_سائز * subset_enc_عدد_codes;
    size1 = Encoding1::min_سائز + Encoding1_Range::static_سائز * subset_enc_رمز_ranges.لمبائی;

    اگر (size0 < size1)
      subset_enc_format = 0;
    ورنہ
      subset_enc_format = 1;
  }

  منطقی plan_subset_charset (ثابت OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    غیرعلامتی صحیح  size0, سائز_ranges;
    غیرعلامتی آخری_sid = CFF_UNDEF_رمز - 1;

    subset_charset_ranges.clear ();
    اگر (unlikely (subset_charset_ranges.اندر_error ()))
    {
      plan->check_success (جھوٹ);
      واپس جھوٹ;
    }

    رمز_pair_t glyph_مقصد_sid_cache {0, HB_CODEPOINT_INVALID};

    غیرعلامتی عدد_glyphs = plan->عدد_output_glyphs ();

    اگر (unlikely (!subset_charset_ranges.alloc (hb_min (عدد_glyphs,
							acc.عدد_charset_entries))))
    {
      plan->check_success (جھوٹ);
      واپس جھوٹ;
    }

    glyph_مقصد_sid_map_t *glyph_مقصد_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_مقصد_sid_map.get_acquire () :
					   خالی_اشاریہ;

    منطقی created_map = جھوٹ;
    اگر (!glyph_مقصد_sid_map && acc.cff_accelerator)
    {
      created_map = سچ;
      glyph_مقصد_sid_map = acc.create_glyph_مقصد_sid_map ();
    }

    اگر (!glyph_مقصد_sid_map || glyph_مقصد_sid_map->اندر_error ())
    {
      glyph_مقصد_sid_map = خالی_اشاریہ;
      created_map = جھوٹ;
    }

    خودکار it = hb_iter (plan->new_مقصد_old_gid_list);
    اگر (it->پہلا == 0) it++;
    خودکار _ = *it;
    منطقی not_is_cid = !acc.is_CID ();
    منطقی skip = !not_is_cid && glyph_مقصد_sid_map && !شناختی_عنصر_charset;
    اگر (not_is_cid)
      sidmap.alloc (عدد_glyphs);
    برائے (hb_codepoint_t glyph = 1; glyph < عدد_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      اگر (glyph == _.پہلا)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      ورنہ
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      غیرعلامتی sid;
      اگر (شناختی_عنصر_charset)
	sid = glyph;
      ورنہ
      {
	sid = glyph_مقصد_sid_map ?
		       glyph_مقصد_sid_map->arrayZ[old_glyph].رمز :
		       acc.glyph_مقصد_sid (old_glyph, &glyph_مقصد_sid_cache);

	اگر (not_is_cid)
	  sid = sidmap.جوڑو (sid);
      }

      اگر (sid != آخری_sid + 1)
	subset_charset_ranges.push (رمز_pair_t {sid, glyph});

      اگر (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.پہلا - 1, glyph_مقصد_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      آخری_sid = sid;
    }

    اگر (created_map)
    {
      اگر ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_مقصد_sid_map.cmpexch (خالی_اشاریہ, glyph_مقصد_sid_map))
      {
	glyph_مقصد_sid_map->~glyph_مقصد_sid_map_t ();
	hb_free (glyph_مقصد_sid_map);
      }
    }

    منطقی two_byte = subset_charset_ranges.complete (عدد_glyphs);

    size0 = Charset0::get_سائز (plan->عدد_output_glyphs ());
    اگر (!two_byte)
      سائز_ranges = Charset1::get_سائز_for_ranges (subset_charset_ranges.لمبائی);
    ورنہ
      سائز_ranges = Charset2::get_سائز_for_ranges (subset_charset_ranges.لمبائی);

    اگر (size0 < سائز_ranges)
      subset_charset_format = 0;
    ورنہ اگر (!two_byte)
      subset_charset_format = 1;
    ورنہ
      subset_charset_format = 2;

    واپس سچ;
  }

  منطقی collect_sids_اندر_dicts (ثابت OT::cff1::accelerator_subset_t &acc)
  {
    برائے (غیرعلامتی صحیح i = 0; i < name_dict_اقدار_t::Val_گنتی; i++)
    {
      غیرعلامتی صحیح sid = acc.چوٹی_Dict_2.nameSIDs[i];
      اگر (sid != CFF_UNDEF_SID)
      {
	چوٹی_Dict_Mod_SIDs[i] = sidmap.جوڑو (sid);
      }
    }

    اگر (acc.fdArray != &Null (CFF1FDArray))
      برائے (غیرعلامتی صحیح i = 0; i < orig_fdcount; i++)
	اگر (fdmap.has (i))
	  (خالی)sidmap.جوڑو (acc.fontDicts[i].fontName);

    واپس سچ;
  }

  منطقی create (ثابت OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    اگر (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) واپس جھوٹ;

    عدد_glyphs = plan->عدد_output_glyphs ();
    orig_fdcount = acc.fd_گنتی;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    شناختی_عنصر_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_شناختی_عنصر_CHARSET) && acc.is_CID ();

 #اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    min_charstrings_off_سائز = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #ورنہ
    min_charstrings_off_سائز = 0;
 #اگر_ختم

    subset_charset = !acc.is_predef_charset () || شناختی_عنصر_charset;
    اگر (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      برائے (ثابت خودکار &_ : plan->new_مقصد_old_gid_list)
      {
	اگر (_.پہلا != _.second)
	{
	  subset_charset = سچ;
	  توڑو;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.چوٹی_Dict_2);
      منطقی need_مقصد_جوڑو_enc = (subset_encoding && !acc.چوٹی_Dict_2.has_op (Op_رمز_Encoding));
      منطقی need_مقصد_جوڑو_سیٹ = (subset_charset && !acc.چوٹی_Dict_2.has_op (Op_رمز_charset));
      اگر (need_مقصد_جوڑو_enc || need_مقصد_جوڑو_سیٹ)
      {
	اگر (need_مقصد_جوڑو_enc)
	  topdict_mod.جوڑو_op (Op_رمز_Encoding);
	اگر (need_مقصد_جوڑو_سیٹ)
	  topdict_mod.جوڑو_op (Op_رمز_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    اگر (acc.fdSelect != &Null (CFF1FDSelect))
    {
	اگر (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.عدد_glyphs,
				  subset_fdcount,
				  info.fd_select.سائز,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	واپس جھوٹ;
    }
    ورنہ
      fdmap.شناختی_عنصر (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      اگر (unlikely (!collect_sids_اندر_dicts (acc)))
	واپس جھوٹ;
      اگر (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	واپس جھوٹ;

      اگر (subset_charset && !plan_subset_charset (acc, plan))
        واپس جھوٹ;

      topdict_mod.reassignSIDs (sidmap);
    }

    اگر (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<ثابت OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, Op_رمز_endchar>
		    flattener(acc, plan);
      اگر (!flattener.flatten (subset_charstrings))
	واپس جھوٹ;
    }
    ورنہ
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      اگر (!subr_subsetter.subset ())
	واپس جھوٹ;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      اگر (!subr_subsetter.encode_charstrings (subset_charstrings))
	واپس جھوٹ;

      اگر (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	واپس جھوٹ;

      /* local subrs */
      اگر (!subset_localsubrs.resize (orig_fdcount))
	واپس جھوٹ;
      برائے (غیرعلامتی صحیح fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	اگر (fdmap.has (fd))
	{
	  اگر (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    واپس جھوٹ;
	}
      }
    }

    /* Encoding */
    اگر (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    اگر (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_اقدار_mod_t ());
    ورنہ
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (ثابت cff1_font_dict_اقدار_t &_)
	{ واپس fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (ثابت cff1_font_dict_اقدار_t &_)
	{
	  cff1_font_dict_اقدار_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  واپس mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    واپس !plan->اندر_error () &&
	   (subset_charstrings.لمبائی == plan->عدد_output_glyphs ()) &&
	   (fontdicts_mod.لمبائی == subset_fdcount);
  }

  cff1_چوٹی_dict_اقدار_mod_t	topdict_mod;
  cff1_sub_جدول_info_t		info;

  غیرعلامتی صحیح    عدد_glyphs;
  غیرعلامتی صحیح    orig_fdcount = 0;
  غیرعلامتی صحیح    subset_fdcount = 1;
  غیرعلامتی صحیح    subset_fdselect_format = 0;
  hb_سمتیہ_t<رمز_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_سمتیہ_t<str_buff_vec_t>	subset_localsubrs;
  hb_سمتیہ_t<cff1_font_dict_اقدار_mod_t>  fontdicts_mod;

  منطقی		drop_hints = جھوٹ;

  منطقی		gid_renum;
  منطقی		subset_encoding;
  uint8_t	subset_enc_format;
  غیرعلامتی صحیح	subset_enc_عدد_codes;
  range_list_t	subset_enc_رمز_ranges;
  hb_سمتیہ_t<رمز_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  منطقی		subset_charset;

  remap_sid_t	sidmap;
  غیرعلامتی صحیح	چوٹی_Dict_Mod_SIDs[name_dict_اقدار_t::Val_گنتی];

  منطقی		desubroutinize = جھوٹ;
  منطقی		شناختی_عنصر_charset = جھوٹ;

  غیرعلامتی	min_charstrings_off_سائز = 0;
};
} // namespace OT

ساکن منطقی _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         ساخت OT::cff1_subset_plan &plan,
                                         ثابت OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1_Char_لڑیاں> ();

  غیرعلامتی ڈیٹا_سائز = 0;
  غیرعلامتی کل_سائز = CFF1_Char_لڑیاں::کل_سائز (plan.subset_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز);
  اگر (unlikely (!c->آغاز_zerocopy (کل_سائز)))
    واپس جھوٹ;

  خودکار *cs = c->آغاز_embed<CFF1_Char_لڑیاں> ();
  اگر (unlikely (!cs->serialize (c, plan.subset_charstrings, &ڈیٹا_سائز, plan.min_charstrings_off_سائز))) {
    c->نکالو_discard ();
    واپس جھوٹ;
  }

  plan.info.char_لڑیاں_link = c->نکالو_pack (جھوٹ);
  واپس سچ;
}

منطقی
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   ساخت OT::cff1_subset_plan &plan) ثابت
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  اگر (!_serialize_cff1_charstrings(c, plan, *یہ))
    واپس جھوٹ;

  /* private dicts & local subrs */
  برائے (صحیح i = (صحیح) نجی_Dicts.لمبائی; --i >= 0 ;)
  {
    اگر (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      اگر (plan.subset_localsubrs[i].لمبائی > 0)
      {
	خودکار *dest = c->push <CFF1Subrs> ();
	اگر (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->نکالو_pack ();
	ورنہ
	{
	  c->نکالو_discard ();
	  واپس جھوٹ;
	}
      }

      خودکار *pd = c->push<نجی_Dict> ();
      cff1_نجی_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      اگر (likely (pd->serialize (c, نجی_Dicts[i], privSzr, subrs_link)))
      {
	غیرعلامتی fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].نجی_Dict_Info.سائز = c->لمبائی ();
	plan.fontdicts_mod[fd].نجی_Dict_Info.link = c->نکالو_pack ();
      }
      ورنہ
      {
	c->نکالو_discard ();
	واپس جھوٹ;
      }
    }
  }

  اگر (!is_CID ())
    plan.info.نجی_Dict_Info = plan.fontdicts_mod[0].نجی_Dict_Info;

  /* FDArray (FD Index) */
  اگر (fdArray != &Null (CFF1FDArray))
  {
    خودکار *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    خودکار it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    اگر (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->نکالو_pack (جھوٹ);
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* FDSelect */
  اگر (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    اگر (likely (hb_serialize_cff_fdselect (c, plan.عدد_glyphs, *fdSelect, fd_گنتی,
					   plan.subset_fdselect_format, plan.info.fd_select.سائز,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* Charset */
  اگر (plan.subset_charset)
  {
    خودکار *dest = c->push<Charset> ();
    اگر (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.عدد_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* Encoding */
  اگر (plan.subset_encoding)
  {
    خودکار *dest = c->push<Encoding> ();
    اگر (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_عدد_codes,
				 plan.subset_enc_رمز_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* global subrs */
  {
    خودکار *dest = c->push <CFF1Subrs> ();
    اگر (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->نکالو_pack (جھوٹ);
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  /* String INDEX */
  {
    خودکار *dest = c->push<CFF1_String_اشاریہ> ();
    اگر (likely (!plan.sidmap.اندر_error () &&
		dest->serialize (c, *string_اشاریہ, plan.sidmap.vector)))
      c->نکالو_pack ();
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  اگر (unlikely (!cff))
    واپس جھوٹ;

  /*سرخی*/
  cff->نسخہ.major = 0x01;
  cff->نسخہ.minor = 0x00;
  cff->name_اشاریہ = cff->min_سائز;
  cff->off_سائز = 4; /* unused? */

  /* name INDEX */
  اگر (unlikely (!c->embed (*name_اشاریہ))) واپس جھوٹ;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    خودکار *چوٹی_2 = c->push<چوٹی_Dict> ();
    cff1_چوٹی_dict_op_serializer_t چوٹی_Szr;
    غیرعلامتی چوٹی_سائز = 0;
    چوٹی_dict_modifiers_t  modifier (plan.info, plan.چوٹی_Dict_Mod_SIDs);
    اگر (likely (چوٹی_2->serialize (c, plan.topdict_mod, چوٹی_Szr, modifier)))
    {
      چوٹی_سائز = c->لمبائی ();
      c->نکالو_pack (جھوٹ);
    }
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
    /* serialize INDEX header for above */
    خودکار *dest = c->آغاز_embed<CFF1_اشاریہ> ();
    واپس dest->serialize_سرخی (c, hb_iter (&چوٹی_سائز, 1), چوٹی_سائز);
  }
}

منطقی
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) ثابت
{
  cff1_subset_plan cff_plan;

  اگر (unlikely (!cff_plan.create (*یہ, c->plan)))
  {
    DEBUG_پیغام(SUBSET, خالی_اشاریہ, "Failed to generate a cff subsetting plan.");
    واپس جھوٹ;
  }

  واپس serialize (c->serializer, cff_plan);
}


#اگر_ختم
