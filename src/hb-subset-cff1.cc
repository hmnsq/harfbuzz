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

#include "hb.hh"

#ifndef HB_NO_SUBSET_CFF

#include "hb-open-type.hh"
#include "hb-ot-cff1-table.hh"
#include "hb-set.h"
#include "hb-bimap.hh"
#include "hb-subset-plan.hh"
#include "hb-subset-cff-common.hh"
#include "hb-cff1-interp-cs.hh"

using namespace CFF;

struct remap_sid_t
{
  unsigned get_population () const { 返回 vector.length; }

  void alloc (unsigned size)
  {
    map.alloc (size);
    vector.alloc_exact (size);
  }

  bool in_error () const
  { 返回 map.in_error () || vector.in_error (); }

  unsigned 整数 add (unsigned 整数 sid)
  {
    如果 (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      返回 sid;

    sid = unoffset_sid (sid);
    unsigned v = next;
    如果 (map.set (sid, v, false))
    {
      vector.push (sid);
      next++;
    }
    否则
      v = map.get (sid); // already exists
    返回 offset_sid (v);
  }

  unsigned 整数 operator[] (unsigned 整数 sid) const
  {
    如果 (is_std_str (sid) || (sid == CFF_UNDEF_SID))
      返回 sid;

    返回 offset_sid (map.get (unoffset_sid (sid)));
  }

  static const unsigned 整数 num_std_strings = 391;

  static bool is_std_str (unsigned 整数 sid) { 返回 sid < num_std_strings; }
  static unsigned 整数 offset_sid (unsigned 整数 sid) { 返回 sid + num_std_strings; }
  static unsigned 整数 unoffset_sid (unsigned 整数 sid) { 返回 sid - num_std_strings; }
  unsigned next = 0;

  hb_map_t map;
  hb_vector_t<unsigned> vector;
};

struct cff1_sub_table_info_t : cff_sub_table_info_t
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
struct cff1_top_dict_values_mod_t : cff1_top_dict_values_t
{
  void init (const cff1_top_dict_values_t *base_= &Null (cff1_top_dict_values_t))
  {
    SUPER::init ();
    base = base_;
  }

  void fini () { SUPER::fini (); }

  unsigned get_count () const { 返回 base->get_count () + SUPER::get_count (); }
  const cff1_top_dict_val_t &get_value (unsigned 整数 i) const
  {
    如果 (i < base->get_count ())
      返回 (*base)[i];
    否则
      返回 SUPER::values[i - base->get_count ()];
  }
  const cff1_top_dict_val_t &operator [] (unsigned 整数 i) const { 返回 get_value (i); }

  void reassignSIDs (const remap_sid_t& sidmap)
  {
    为 (unsigned 整数 i = 0; i < name_dict_values_t::ValCount; i++)
      nameSIDs[i] = sidmap[base->nameSIDs[i]];
  }

  protected:
  typedef cff1_top_dict_values_t SUPER;
  const cff1_top_dict_values_t *base;
};

struct top_dict_modifiers_t
{
  top_dict_modifiers_t (const cff1_sub_table_info_t &info_,
			const unsigned 整数 (&nameSIDs_)[name_dict_values_t::ValCount])
    : info (info_),
      nameSIDs (nameSIDs_)
  {}

  const cff1_sub_table_info_t &info;
  const unsigned 整数	(&nameSIDs)[name_dict_values_t::ValCount];
};

struct cff1_top_dict_op_serializer_t : cff_top_dict_op_serializer_t<cff1_top_dict_val_t>
{
  bool serialize (hb_serialize_context_t *c,
		  const cff1_top_dict_val_t &opstr,
		  const top_dict_modifiers_t &mod) const
  {
    TRACE_SERIALIZE (this);

    op_code_t op = opstr.op;
    switch (op)
    {
      case OpCode_charset:
	如果 (mod.info.charset_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.charset_link, whence_t::Absolute));
	否则
	  goto fall_back;

      case OpCode_Encoding:
	如果 (mod.info.encoding_link)
	  return_trace (FontDict::serialize_link4_op(c, op, mod.info.encoding_link, whence_t::Absolute));
	否则
	  goto fall_back;

      case OpCode_Private:
	return_trace (UnsizedByteStr::serialize_int2 (c, mod.info.privateDictInfo.size) &&
		      Dict::serialize_link4_op (c, op, mod.info.privateDictInfo.link, whence_t::Absolute));

      case OpCode_version:
      case OpCode_Notice:
      case OpCode_Copyright:
      case OpCode_FullName:
      case OpCode_FamilyName:
      case OpCode_Weight:
      case OpCode_PostScript:
      case OpCode_BaseFontName:
      case OpCode_FontName:
	return_trace (FontDict::serialize_int2_op (c, op, mod.nameSIDs[name_dict_values_t::name_op_to_index (op)]));

      case OpCode_ROS:
	{
	  /* for registry & ordering, reassigned SIDs are serialized
	   * for supplement, the original byte string is copied along with the op code */
	  op_str_t supp_op;
	  supp_op.op = op;
	  如果 ( unlikely (!(opstr.length >= opstr.last_arg_offset + 3)))
	    return_trace (false);
	  supp_op.ptr = opstr.ptr + opstr.last_arg_offset;
	  supp_op.length = opstr.length - opstr.last_arg_offset;
	  return_trace (UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_values_t::registry]) &&
			UnsizedByteStr::serialize_int2 (c, mod.nameSIDs[name_dict_values_t::ordering]) &&
			copy_opstr (c, supp_op));
	}
      fall_back:
      default:
	return_trace (cff_top_dict_op_serializer_t<cff1_top_dict_val_t>::serialize (c, opstr, mod.info));
    }
    return_trace (true);
  }

};

struct cff1_font_dict_op_serializer_t : cff_font_dict_op_serializer_t
{
  bool serialize (hb_serialize_context_t *c,
		  const op_str_t &opstr,
		  const cff1_font_dict_values_mod_t &mod) const
  {
    TRACE_SERIALIZE (this);

    如果 (opstr.op == OpCode_FontName)
      return_trace (FontDict::serialize_int2_op (c, opstr.op, mod.fontName));
    否则
      return_trace (SUPER::serialize (c, opstr, mod.privateDictInfo));
  }

  private:
  typedef cff_font_dict_op_serializer_t SUPER;
};

struct cff1_cs_opset_flatten_t : cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t>
{
  static void flush_args_and_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    如果 (env.arg_start > 0)
      flush_width (env, param);

    switch (op)
    {
      case OpCode_hstem:
      case OpCode_hstemhm:
      case OpCode_vstem:
      case OpCode_vstemhm:
      case OpCode_hintmask:
      case OpCode_cntrmask:
      case OpCode_dotsection:
	如果 (param.drop_hints)
	{
	  env.clear_args ();
	  返回;
	}
	HB_FALLTHROUGH;

      default:
	SUPER::flush_args_and_op (op, env, param);
	break;
    }
  }
  static void flush_args (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    为 (unsigned 整数 i = env.arg_start; i < env.argStack.get_count (); i++)
      encoder.encode_num_cs (env.eval_arg (i));
    SUPER::flush_args (env, param);
  }

  static void flush_op (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_op (op);
  }

  static void flush_width (cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    assert (env.has_width);
    str_encoder_t  encoder (param.flatStr);
    encoder.encode_num_cs (env.width);
  }

  static void flush_hintmask (op_code_t op, cff1_cs_interp_env_t &env, flatten_param_t& param)
  {
    SUPER::flush_hintmask (op, env, param);
    如果 (!param.drop_hints)
    {
      str_encoder_t  encoder (param.flatStr);
      为 (unsigned 整数 i = 0; i < env.hintmask_size; i++)
	encoder.encode_byte (env.str_ref[i]);
    }
  }

  private:
  typedef cff1_cs_opset_t<cff1_cs_opset_flatten_t, flatten_param_t> SUPER;
};

struct range_list_t : hb_vector_t<code_pair_t>
{
  /* replace the first glyph ID in the "glyph" field each range with a nLeft value */
  bool complete (unsigned 整数 last_glyph)
  {
    hb_codepoint_t all_glyphs = 0;
    unsigned count = this->length;
    为 (unsigned 整数 i = count; i; i--)
    {
      code_pair_t &pair = arrayZ[i - 1];
      unsigned 整数 nLeft = last_glyph - pair.glyph - 1;
      all_glyphs |= nLeft;
      last_glyph = pair.glyph;
      pair.glyph = nLeft;
    }
    bool two_byte = all_glyphs >= 0x100;
    返回 two_byte;
  }
};

struct cff1_cs_opset_subr_subset_t : cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t>
{
  static void process_op (op_code_t op, cff1_cs_interp_env_t &env, subr_subset_param_t& param)
  {
    switch (op) {

      case OpCode_return:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	env.return_from_subr ();
	param.set_current_str (env, false);
	break;

      case OpCode_endchar:
	param.current_parsed_str->add_op (op, env.str_ref);
	param.current_parsed_str->set_parsed ();
	SUPER::process_op (op, env, param);
	break;

      case OpCode_callsubr:
	process_call_subr (op, CSType_LocalSubr, env, param, env.localSubrs, param.local_closure);
	break;

      case OpCode_callgsubr:
	process_call_subr (op, CSType_GlobalSubr, env, param, env.globalSubrs, param.global_closure);
	break;

      default:
	SUPER::process_op (op, env, param);
	param.current_parsed_str->add_op (op, env.str_ref);
	break;
    }
  }

  protected:
  static void process_call_subr (op_code_t op, cs_type_t type,
				 cff1_cs_interp_env_t &env, subr_subset_param_t& param,
				 cff1_biased_subrs_t& subrs, hb_set_t *closure)
  {
    byte_str_ref_t    str_ref = env.str_ref;
    env.call_subr (subrs, type);
    param.current_parsed_str->add_call_op (op, str_ref, env.context.subr_num);
    closure->add (env.context.subr_num);
    param.set_current_str (env, true);
  }

  private:
  typedef cff1_cs_opset_t<cff1_cs_opset_subr_subset_t, subr_subset_param_t> SUPER;
};

struct cff1_private_dict_op_serializer_t : op_serializer_t
{
  cff1_private_dict_op_serializer_t (bool desubroutinize_, bool drop_hints_)
    : desubroutinize (desubroutinize_), drop_hints (drop_hints_) {}

  bool serialize (hb_serialize_context_t *c,
		  const op_str_t &opstr,
		  objidx_t subrs_link) const
  {
    TRACE_SERIALIZE (this);

    如果 (drop_hints && dict_opset_t::is_hint_op (opstr.op))
      return_trace (true);

    如果 (opstr.op == OpCode_Subrs)
    {
      如果 (desubroutinize || !subrs_link)
	return_trace (true);
      否则
	return_trace (FontDict::serialize_link2_op (c, opstr.op, subrs_link));
    }

    return_trace (copy_opstr (c, opstr));
  }

  protected:
  const bool desubroutinize;
  const bool drop_hints;
};

struct cff1_subr_subsetter_t : subr_subsetter_t<cff1_subr_subsetter_t, CFF1Subrs, const OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_subr_subset_t, OpCode_endchar>
{
  cff1_subr_subsetter_t (const OT::cff1::accelerator_subset_t &acc_, const hb_subset_plan_t *plan_)
    : subr_subsetter_t (acc_, plan_) {}

  static void complete_parsed_str (cff1_cs_interp_env_t &env, subr_subset_param_t& param, parsed_cs_str_t &charstring)
  {
    /* insert width at the beginning of the charstring as necessary */
    如果 (env.has_width)
      charstring.set_prefix (env.width);

    /* subroutines/charstring left on the call stack are legally left unmarked
     * unmarked when a subroutine terminates with endchar. mark them.
     */
    param.current_parsed_str->set_parsed ();
    为 (unsigned 整数 i = 0; i < env.callStack.get_count (); i++)
    {
      parsed_cs_str_t *parsed_str = param.get_parsed_str_for_context (env.callStack[i]);
      如果 (likely (parsed_str))
	parsed_str->set_parsed ();
      否则
	env.set_error ();
    }
  }
};

namespace OT {
struct cff1_subset_plan
{
  cff1_subset_plan ()
  {
    为 (unsigned 整数 i = 0; i < name_dict_values_t::ValCount; i++)
      topDictModSIDs[i] = CFF_UNDEF_SID;
  }

  void plan_subset_encoding (const OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    const Encoding *encoding = acc.encoding;
    unsigned 整数  size0, size1;
    unsigned code, last_code = CFF_UNDEF_CODE - 1;
    hb_vector_t<hb_codepoint_t> supp_codes;

    subset_enc_code_ranges.clear ();
    如果 (unlikely (subset_enc_code_ranges.in_error ()))
    {
      plan->check_success (false);
      返回;
    }

    supp_codes.init ();

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};
    subset_enc_num_codes = plan->num_output_glyphs () - 1;
    unsigned 整数 glyph;
    auto it = hb_iter (plan->new_to_old_gid_list);
    如果 (it->first == 0) it++;
    auto _ = *it;
    为 (glyph = 1; glyph < num_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      如果 (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      否则
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      code = acc.glyph_to_code (old_glyph, &glyph_to_sid_cache);
      如果 (code == CFF_UNDEF_CODE)
      {
	subset_enc_num_codes = glyph - 1;
	break;
      }

      如果 (code != last_code + 1)
	subset_enc_code_ranges.push (code_pair_t {code, glyph});
      last_code = code;

      如果 (encoding != &Null (Encoding))
      {
	hb_codepoint_t  sid = acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);
	encoding->get_supplement_codes (sid, supp_codes);
	为 (unsigned 整数 i = 0; i < supp_codes.length; i++)
	  subset_enc_supp_codes.push (code_pair_t {supp_codes[i], sid});
      }
    }
    supp_codes.fini ();

    subset_enc_code_ranges.complete (glyph);

    assert (subset_enc_num_codes <= 0xFF);
    size0 = Encoding0::min_size + HBUINT8::static_size * subset_enc_num_codes;
    size1 = Encoding1::min_size + Encoding1_Range::static_size * subset_enc_code_ranges.length;

    如果 (size0 < size1)
      subset_enc_format = 0;
    否则
      subset_enc_format = 1;
  }

  bool plan_subset_charset (const OT::cff1::accelerator_subset_t &acc, hb_subset_plan_t *plan)
  {
    unsigned 整数  size0, size_ranges;
    unsigned last_sid = CFF_UNDEF_CODE - 1;

    subset_charset_ranges.clear ();
    如果 (unlikely (subset_charset_ranges.in_error ()))
    {
      plan->check_success (false);
      返回 false;
    }

    code_pair_t glyph_to_sid_cache {0, HB_CODEPOINT_INVALID};

    unsigned num_glyphs = plan->num_output_glyphs ();

    如果 (unlikely (!subset_charset_ranges.alloc (hb_min (num_glyphs,
							acc.num_charset_entries))))
    {
      plan->check_success (false);
      返回 false;
    }

    glyph_to_sid_map_t *glyph_to_sid_map = acc.cff_accelerator ?
					   acc.cff_accelerator->glyph_to_sid_map.get_acquire () :
					   nullptr;

    bool created_map = false;
    如果 (!glyph_to_sid_map && acc.cff_accelerator)
    {
      created_map = true;
      glyph_to_sid_map = acc.create_glyph_to_sid_map ();
    }

    如果 (!glyph_to_sid_map || glyph_to_sid_map->in_error ())
    {
      glyph_to_sid_map = nullptr;
      created_map = false;
    }

    auto it = hb_iter (plan->new_to_old_gid_list);
    如果 (it->first == 0) it++;
    auto _ = *it;
    bool not_is_cid = !acc.is_CID ();
    bool skip = !not_is_cid && glyph_to_sid_map && !identity_charset;
    如果 (not_is_cid)
      sidmap.alloc (num_glyphs);
    为 (hb_codepoint_t glyph = 1; glyph < num_glyphs; glyph++)
    {
      hb_codepoint_t old_glyph;
      如果 (glyph == _.first)
      {
	old_glyph = _.second;
	_ = *++it;
      }
      否则
      {
	/* Retain the SID for the old missing glyph ID */
	old_glyph = glyph;
      }
      unsigned sid;
      如果 (identity_charset)
	sid = glyph;
      否则
      {
	sid = glyph_to_sid_map ?
		       glyph_to_sid_map->arrayZ[old_glyph].code :
		       acc.glyph_to_sid (old_glyph, &glyph_to_sid_cache);

	如果 (not_is_cid)
	  sid = sidmap.add (sid);
      }

      如果 (sid != last_sid + 1)
	subset_charset_ranges.push (code_pair_t {sid, glyph});

      如果 (glyph == old_glyph && skip)
      {
	glyph = hb_min (_.first - 1, glyph_to_sid_map->arrayZ[old_glyph].glyph);
	sid += glyph - old_glyph;
      }
      last_sid = sid;
    }

    如果 (created_map)
    {
      如果 ((!plan->accelerator && acc.cff_accelerator) ||
	  !acc.cff_accelerator->glyph_to_sid_map.cmpexch (nullptr, glyph_to_sid_map))
      {
	glyph_to_sid_map->~glyph_to_sid_map_t ();
	hb_free (glyph_to_sid_map);
      }
    }

    bool two_byte = subset_charset_ranges.complete (num_glyphs);

    size0 = Charset0::get_size (plan->num_output_glyphs ());
    如果 (!two_byte)
      size_ranges = Charset1::get_size_for_ranges (subset_charset_ranges.length);
    否则
      size_ranges = Charset2::get_size_for_ranges (subset_charset_ranges.length);

    如果 (size0 < size_ranges)
      subset_charset_format = 0;
    否则 如果 (!two_byte)
      subset_charset_format = 1;
    否则
      subset_charset_format = 2;

    返回 true;
  }

  bool collect_sids_in_dicts (const OT::cff1::accelerator_subset_t &acc)
  {
    为 (unsigned 整数 i = 0; i < name_dict_values_t::ValCount; i++)
    {
      unsigned 整数 sid = acc.topDict.nameSIDs[i];
      如果 (sid != CFF_UNDEF_SID)
      {
	topDictModSIDs[i] = sidmap.add (sid);
      }
    }

    如果 (acc.fdArray != &Null (CFF1FDArray))
      为 (unsigned 整数 i = 0; i < orig_fdcount; i++)
	如果 (fdmap.has (i))
	  (void)sidmap.add (acc.fontDicts[i].fontName);

    返回 true;
  }

  bool create (const OT::cff1::accelerator_subset_t &acc,
	       hb_subset_plan_t *plan)
  {
    /* make sure notdef is first */
    hb_codepoint_t old_glyph;
    如果 (!plan->old_gid_for_new_gid (0, &old_glyph) || (old_glyph != 0)) 返回 false;

    num_glyphs = plan->num_output_glyphs ();
    orig_fdcount = acc.fdCount;
    drop_hints = plan->flags & HB_SUBSET_FLAGS_NO_HINTING;
    desubroutinize = plan->flags & HB_SUBSET_FLAGS_DESUBROUTINIZE;
    identity_charset = (plan->flags & HB_SUBSET_FLAGS_CFF_IDENTITY_CHARSET) && acc.is_CID ();

 #ifdef HB_EXPERIMENTAL_API
    min_charstrings_off_size = (plan->flags & HB_SUBSET_FLAGS_IFTB_REQUIREMENTS) ? 4 : 0;
 #否则
    min_charstrings_off_size = 0;
 #endif

    subset_charset = !acc.is_predef_charset () || identity_charset;
    如果 (!subset_charset)
      /* check whether the subset renumbers any glyph IDs */
      为 (const auto &_ : plan->new_to_old_gid_list)
      {
	如果 (_.first != _.second)
	{
	  subset_charset = true;
	  break;
	}
      }

    subset_encoding = !acc.is_CID() && !acc.is_predef_encoding ();

    /* top dict INDEX */
    {
      /* Add encoding/charset to a (copy of) top dict as necessary */
      topdict_mod.init (&acc.topDict);
      bool need_to_add_enc = (subset_encoding && !acc.topDict.has_op (OpCode_Encoding));
      bool need_to_add_set = (subset_charset && !acc.topDict.has_op (OpCode_charset));
      如果 (need_to_add_enc || need_to_add_set)
      {
	如果 (need_to_add_enc)
	  topdict_mod.add_op (OpCode_Encoding);
	如果 (need_to_add_set)
	  topdict_mod.add_op (OpCode_charset);
      }
    }

    /* Determine re-mapping of font index as fdmap among other info */
    如果 (acc.fdSelect != &Null (CFF1FDSelect))
    {
	如果 (unlikely (!hb_plan_subset_cff_fdselect (plan,
				  orig_fdcount,
				  *acc.fdSelect,
				  acc.num_glyphs,
				  subset_fdcount,
				  info.fd_select.size,
				  subset_fdselect_format,
				  subset_fdselect_ranges,
				  fdmap)))
	返回 false;
    }
    否则
      fdmap.identity (1);

    /* remove unused SIDs & reassign SIDs */
    {
      /* SIDs for name strings in dicts are added before glyph names so they fit in 16-bit int range */
      如果 (unlikely (!collect_sids_in_dicts (acc)))
	返回 false;
      如果 (unlikely (sidmap.get_population () > 0x8000))	/* assumption: a dict won't reference that many strings */
	返回 false;

      如果 (subset_charset && !plan_subset_charset (acc, plan))
        返回 false;

      topdict_mod.reassignSIDs (sidmap);
    }

    如果 (desubroutinize)
    {
      /* Flatten global & local subrs */
      subr_flattener_t<const OT::cff1::accelerator_subset_t, cff1_cs_interp_env_t, cff1_cs_opset_flatten_t, OpCode_endchar>
		    flattener(acc, plan);
      如果 (!flattener.flatten (subset_charstrings))
	返回 false;
    }
    否则
    {
      cff1_subr_subsetter_t       subr_subsetter (acc, plan);

      /* Subset subrs: collect used subroutines, leaving all unused ones behind */
      如果 (!subr_subsetter.subset ())
	返回 false;

      /* encode charstrings, global subrs, local subrs with new subroutine numbers */
      如果 (!subr_subsetter.encode_charstrings (subset_charstrings))
	返回 false;

      如果 (!subr_subsetter.encode_globalsubrs (subset_globalsubrs))
	返回 false;

      /* local subrs */
      如果 (!subset_localsubrs.resize (orig_fdcount))
	返回 false;
      为 (unsigned 整数 fd = 0; fd < orig_fdcount; fd++)
      {
	subset_localsubrs[fd].init ();
	如果 (fdmap.has (fd))
	{
	  如果 (!subr_subsetter.encode_localsubrs (fd, subset_localsubrs[fd]))
	    返回 false;
	}
      }
    }

    /* Encoding */
    如果 (subset_encoding)
      plan_subset_encoding (acc, plan);

    /* private dicts & local subrs */
    如果 (!acc.is_CID ())
      fontdicts_mod.push (cff1_font_dict_values_mod_t ());
    否则
    {
      + hb_iter (acc.fontDicts)
      | hb_filter ([&] (const cff1_font_dict_values_t &_)
	{ 返回 fdmap.has (&_ - &acc.fontDicts[0]); } )
      | hb_map ([&] (const cff1_font_dict_values_t &_)
	{
	  cff1_font_dict_values_mod_t mod;
	  mod.init (&_, sidmap[_.fontName]);
	  返回 mod;
	})
      | hb_sink (fontdicts_mod)
      ;
    }

    返回 !plan->in_error () &&
	   (subset_charstrings.length == plan->num_output_glyphs ()) &&
	   (fontdicts_mod.length == subset_fdcount);
  }

  cff1_top_dict_values_mod_t	topdict_mod;
  cff1_sub_table_info_t		info;

  unsigned 整数    num_glyphs;
  unsigned 整数    orig_fdcount = 0;
  unsigned 整数    subset_fdcount = 1;
  unsigned 整数    subset_fdselect_format = 0;
  hb_vector_t<code_pair_t>   subset_fdselect_ranges;

  /* font dict index remap table from fullset FDArray to subset FDArray.
   * set to CFF_UNDEF_CODE if excluded from subset */
  hb_inc_bimap_t   fdmap;

  str_buff_vec_t		subset_charstrings;
  str_buff_vec_t		subset_globalsubrs;
  hb_vector_t<str_buff_vec_t>	subset_localsubrs;
  hb_vector_t<cff1_font_dict_values_mod_t>  fontdicts_mod;

  bool		drop_hints = false;

  bool		gid_renum;
  bool		subset_encoding;
  uint8_t	subset_enc_format;
  unsigned 整数	subset_enc_num_codes;
  range_list_t	subset_enc_code_ranges;
  hb_vector_t<code_pair_t>  subset_enc_supp_codes;

  uint8_t	subset_charset_format;
  range_list_t	subset_charset_ranges;
  bool		subset_charset;

  remap_sid_t	sidmap;
  unsigned 整数	topDictModSIDs[name_dict_values_t::ValCount];

  bool		desubroutinize = false;
  bool		identity_charset = false;

  unsigned	min_charstrings_off_size = 0;
};
} // namespace OT

static bool _serialize_cff1_charstrings (hb_serialize_context_t *c,
                                         struct OT::cff1_subset_plan &plan,
                                         const OT::cff1::accelerator_subset_t  &acc)
{
  c->push<CFF1CharStrings> ();

  unsigned data_size = 0;
  unsigned total_size = CFF1CharStrings::total_size (plan.subset_charstrings, &data_size, plan.min_charstrings_off_size);
  如果 (unlikely (!c->start_zerocopy (total_size)))
    返回 false;

  auto *cs = c->start_embed<CFF1CharStrings> ();
  如果 (unlikely (!cs->serialize (c, plan.subset_charstrings, &data_size, plan.min_charstrings_off_size))) {
    c->pop_discard ();
    返回 false;
  }

  plan.info.char_strings_link = c->pop_pack (false);
  返回 true;
}

bool
OT::cff1::accelerator_subset_t::serialize (hb_serialize_context_t *c,
					   struct OT::cff1_subset_plan &plan) const
{
  /* push charstrings onto the object stack first which will ensure it packs as the last
     object in the table. Keeping the chastrings last satisfies the requirements for patching
     via IFTB. If this ordering needs to be changed in the future, charstrings should be left
     at the end whenever HB_SUBSET_FLAGS_ITFB_REQUIREMENTS is enabled. */
  如果 (!_serialize_cff1_charstrings(c, plan, *this))
    返回 false;

  /* private dicts & local subrs */
  为 (整数 i = (整数) privateDicts.length; --i >= 0 ;)
  {
    如果 (plan.fdmap.has (i))
    {
      objidx_t	subrs_link = 0;
      如果 (plan.subset_localsubrs[i].length > 0)
      {
	auto *dest = c->push <CFF1Subrs> ();
	如果 (likely (dest->serialize (c, plan.subset_localsubrs[i])))
	  subrs_link = c->pop_pack ();
	否则
	{
	  c->pop_discard ();
	  返回 false;
	}
      }

      auto *pd = c->push<PrivateDict> ();
      cff1_private_dict_op_serializer_t privSzr (plan.desubroutinize, plan.drop_hints);
      /* N.B. local subrs immediately follows its corresponding private dict. i.e., subr offset == private dict size */
      如果 (likely (pd->serialize (c, privateDicts[i], privSzr, subrs_link)))
      {
	unsigned fd = plan.fdmap[i];
	plan.fontdicts_mod[fd].privateDictInfo.size = c->length ();
	plan.fontdicts_mod[fd].privateDictInfo.link = c->pop_pack ();
      }
      否则
      {
	c->pop_discard ();
	返回 false;
      }
    }
  }

  如果 (!is_CID ())
    plan.info.privateDictInfo = plan.fontdicts_mod[0].privateDictInfo;

  /* FDArray (FD Index) */
  如果 (fdArray != &Null (CFF1FDArray))
  {
    auto *fda = c->push<CFF1FDArray> ();
    cff1_font_dict_op_serializer_t  fontSzr;
    auto it = + hb_zip (+ hb_iter (plan.fontdicts_mod), + hb_iter (plan.fontdicts_mod));
    如果 (likely (fda->serialize (c, it, fontSzr)))
      plan.info.fd_array_link = c->pop_pack (false);
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  /* FDSelect */
  如果 (fdSelect != &Null (CFF1FDSelect))
  {
    c->push ();
    如果 (likely (hb_serialize_cff_fdselect (c, plan.num_glyphs, *fdSelect, fdCount,
					   plan.subset_fdselect_format, plan.info.fd_select.size,
					   plan.subset_fdselect_ranges)))
      plan.info.fd_select.link = c->pop_pack ();
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  /* Charset */
  如果 (plan.subset_charset)
  {
    auto *dest = c->push<Charset> ();
    如果 (likely (dest->serialize (c,
				 plan.subset_charset_format,
				 plan.num_glyphs,
				 plan.subset_charset_ranges)))
      plan.info.charset_link = c->pop_pack ();
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  /* Encoding */
  如果 (plan.subset_encoding)
  {
    auto *dest = c->push<Encoding> ();
    如果 (likely (dest->serialize (c,
				 plan.subset_enc_format,
				 plan.subset_enc_num_codes,
				 plan.subset_enc_code_ranges,
				 plan.subset_enc_supp_codes)))
      plan.info.encoding_link = c->pop_pack ();
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  /* global subrs */
  {
    auto *dest = c->push <CFF1Subrs> ();
    如果 (likely (dest->serialize (c, plan.subset_globalsubrs)))
      c->pop_pack (false);
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  /* String INDEX */
  {
    auto *dest = c->push<CFF1StringIndex> ();
    如果 (likely (!plan.sidmap.in_error () &&
		dest->serialize (c, *stringIndex, plan.sidmap.vector)))
      c->pop_pack ();
    否则
    {
      c->pop_discard ();
      返回 false;
    }
  }

  OT::cff1 *cff = c->allocate_min<OT::cff1> ();
  如果 (unlikely (!cff))
    返回 false;

  /* header */
  cff->version.major = 0x01;
  cff->version.minor = 0x00;
  cff->nameIndex = cff->min_size;
  cff->offSize = 4; /* unused? */

  /* name INDEX */
  如果 (unlikely (!c->embed (*nameIndex))) 返回 false;

  /* top dict INDEX */
  {
    /* serialize singleton TopDict */
    auto *top = c->push<TopDict> ();
    cff1_top_dict_op_serializer_t topSzr;
    unsigned top_size = 0;
    top_dict_modifiers_t  modifier (plan.info, plan.topDictModSIDs);
    如果 (likely (top->serialize (c, plan.topdict_mod, topSzr, modifier)))
    {
      top_size = c->length ();
      c->pop_pack (false);
    }
    否则
    {
      c->pop_discard ();
      返回 false;
    }
    /* serialize INDEX header for above */
    auto *dest = c->start_embed<CFF1Index> ();
    返回 dest->serialize_header (c, hb_iter (&top_size, 1), top_size);
  }
}

bool
OT::cff1::accelerator_subset_t::subset (hb_subset_context_t *c) const
{
  cff1_subset_plan cff_plan;

  如果 (unlikely (!cff_plan.create (*this, c->plan)))
  {
    DEBUG_MSG(SUBSET, nullptr, "Failed to generate a cff subsetting plan.");
    返回 false;
  }

  返回 serialize (c->serializer, cff_plan);
}


#endif
