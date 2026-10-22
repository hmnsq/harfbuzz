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

#ifndef HB_SUBSET_CFF_COMMON_HH
#define HB_SUBSET_CFF_COMMON_HH

#شامل "hb.hh"

#شامل "hb-subset-plan.hh"
#شامل "hb-cff-interp-cs-common.hh"

فضاینام CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
ایستا درونخطی unsigned
subset_get_fd (ثابت ACC &acc, hb_codepoint_t glyph)
{
  بازگشت likely (glyph < acc.رقم_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ساختار str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  تهی reset () { buff.reset (); }

  تهی encode_byte (unsigned نویسه b)
  {
    اگر (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    وگرنه
      buff.push (b);
  }

  تهی encode_int (صحیح v)
  {
    اگر ((-1131 <= v) && (v <= 1131))
    {
      اگر ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      وگرنه اگر (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      وگرنه
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    وگرنه
    {
      اگر (unlikely (v < -32768))
	v = -32768;
      وگرنه اگر (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  تهی encode_رقم_cs (ثابت عدد_t& n)
  {
    اگر (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    وگرنه
    {
      int32_t v = n.to_fixed ();
      encode_byte (OpCode_fixedcs);
      encode_byte ((v >> 24) & 0xFF);
      encode_byte ((v >> 16) & 0xFF);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for TopDict / Private
  تهی encode_رقم_tp (ثابت عدد_t& n)
  {
    اگر (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    وگرنه
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      دوگانه v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      نویسه buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", پوچ));
      snprintf (buf, اندازه (buf), "%.8G", v);
      (تهی) hb_uselocale (((تهی) freelocale (clocale), oldlocale));

      نویسه *s = buf;
      size_t len;
      نویسه *comma = strchr (s, ',');
      اگر (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      اگر (s[0] == '0' && s[1] == '.')
	s++;
      وگرنه اگر (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      وگرنه اگر ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	نویسه *s2 = s + exponent - 1;
	وقتی (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, اندازه (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      وگرنه
      {
	نویسه *dot = strchr (s, '.');
	نویسه *e = strchr (s, 'E');
	اگر (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  صحیح exponent = atoi (e + 1);
	  صحیح new_exponent = exponent - (e - (dot + 1));
	  اگر (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  وگرنه
	    snprintf (e - 1, اندازه (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      اگر ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	صحیح sign = s[0] == '-';
	نویسه *s2 = s + sign + 1;
	وقتی (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, اندازه (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<نویسه> nibbles;
      وقتی (*s)
      {
	نویسه c = s[0];
	s++;

	انتخاب (c)
	{
	  حالت 'E':
	  {
	    نویسه c2 = *s;
	    اگر (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } وگرنه {
	      اگر (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    اگر (*s == '0')
	      s++;
	    ادامه;
	  }

	  حالت '.':
	    nibbles.push (0x0A); // .
	    ادامه;

	  حالت '-':
	    nibbles.push (0x0E); // -
	    ادامه;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      اگر (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned شمار = nibbles.length;
      برای (unsigned i = 0; i < شمار; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  تهی encode_op (op_code_t op)
  {
    اگر (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    وگرنه
      encode_byte (op);
  }

  تهی copy_str (ثابت unsigned نویسه *str, unsigned length)
  {
    أكد ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  منطقی in_error () ثابت { بازگشت buff.in_error (); }

  محافظتشده:

  str_buff_t &buff;
};

ساختار cff_sub_table_info_t {
  cff_sub_table_info_t ()
    : fd_array_link (0),
      char_strings_link (0)
  {
    fd_select.init ();
  }

  table_info_t     fd_select;
  objidx_t     	   fd_array_link;
  objidx_t     	   char_strings_link;
};

template <typename OPSTR=op_str_t>
ساختار cff_بالاترین_dict_op_serializer_t : op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت OPSTR &opstr,
		  ثابت cff_sub_table_info_t &info) ثابت
  {
    TRACE_SERIALIZE (این);

    انتخاب (opstr.op)
    {
      حالت OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      حالت OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      حالت OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      پیشفرض:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (درست);
  }
};

ساختار cff_font_dict_op_serializer_t : op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت table_info_t &privateDictInfo) ثابت
  {
    TRACE_SERIALIZE (این);

    اگر (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    وگرنه
    {
      unsigned نویسه *d = c->allocate_size<unsigned نویسه> (opstr.length);
      اگر (unlikely (!d)) return_trace (نادرست);
      /* Faster than hb_memcpy for small strings. */
      برای (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (درست);
  }
};

/* CharString command for specialization */
ساختار cs_command_t
{
  hb_vector_t<عدد_t> args;
  hb_vector_t<unsigned نویسه> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

تعریفنوع hb_vector_t<cs_command_t> *cs_command_vec_t;

ساختار cff2_instancing_plan_t;

ساختار flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   منطقی drop_hints_,
                   ثابت hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  منطقی	drop_hints;
  ثابت hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ثابت cff2_instancing_plan_t *instancer = nullptr;
  منطقی emitted_blend = نادرست;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ساختار subr_flattener_t
{
  subr_flattener_t (ثابت ACC &acc_,
		    ثابت hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  منطقی flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned شمار = plan->رقم_output_glyphs ();
    اگر (!flat_charstrings.resize_exact (شمار))
      بازگشت نادرست;
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      hb_codepoint_t  glyph;
      اگر (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	اگر (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	ادامه;
      }
      ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned صحیح fd = subset_get_fd (acc, glyph);
      اگر (unlikely (fd >= acc.fd_شمار))
	بازگشت نادرست;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (منطقی) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      اگر (unlikely (!interp.interpret (param)))
	بازگشت نادرست;
    }
    بازگشت درست;
  }

  ثابت ACC &acc;
  ثابت hb_subset_plan_t *plan;
};

ساختار subr_closures_t
{
  subr_closures_t (unsigned صحیح fd_شمار_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_شمار_2);
  }

  تهی reset ()
  {
    global_closure.clear();
    برای (unsigned صحیح i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  منطقی in_error () ثابت { بازگشت local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

ساختار parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned صحیح subr_رقم = 0) :
    subr_رقم_2 (subr_رقم) {}

  منطقی is_hinting () ثابت { بازگشت hinting_flag; }
  تهی set_hinting ()       { hinting_flag = درست; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  محافظتشده:
  منطقی	  hinting_flag = نادرست;

  عمومی:
  uint16_t subr_رقم_2;
};

ساختار parsed_cs_str_t : parsed_مقادیر_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (نادرست),
    hint_dropped (نادرست),
    has_prefix_ (نادرست),
    has_calls_ (نادرست),
    coalescing_ (نادرست)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  تهی add_op (op_code_t op, ثابت byte_str_ref_t& str_ref)
  {
    اگر (is_parsed ()) بازگشت;
    اگر (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      اگر (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      بازگشت;
    }
    SUPER::add_op (op, str_ref);
  }

  تهی add_call_op (op_code_t op, ثابت byte_str_ref_t& str_ref, unsigned صحیح subr_رقم_2)
  {
    اگر (is_parsed ()) بازگشت;
    has_calls_ = درست;

    اگر (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_رقم_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      بازگشت;
    }

    /* Pop the subroutine number. */
    values.برداشت ();

    SUPER::add_op (op, str_ref, {subr_رقم_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  تهی flush_segment (ثابت byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    اگر (end <= start) بازگشت;
    وقتی (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      اگر (unlikely (!arr.length)) بشکن;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->length = arr.length;
      start += arr.length;
    }
    opStart = end;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  تهی flush_coalesced (ثابت byte_str_ref_t& str_ref)
  {
    اگر (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  تهی enable_coalescing () { coalescing_ = درست; }
  منطقی is_coalescing () ثابت { بازگشت coalescing_; }

  تهی set_prefix (ثابت عدد_t &رقم, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = درست;
    prefix_op_ = op;
    prefix_رقم = رقم;
  }

  منطقی at_end (unsigned صحیح pos) ثابت
  {
    بازگشت ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  منطقی is_parsed () ثابت { بازگشت parsed; }
  تهی set_parsed ()      { parsed = درست; }

  منطقی is_hint_dropped () ثابت { بازگشت hint_dropped; }
  تهی set_hint_dropped ()      { hint_dropped = درست; }

  منطقی is_vsindex_dropped () ثابت { بازگشت vsindex_dropped; }
  تهی set_vsindex_dropped ()      { vsindex_dropped = درست; }

  منطقی has_prefix () ثابت          { بازگشت has_prefix_; }
  op_code_t prefix_op () ثابت         { بازگشت prefix_op_; }
  ثابت عدد_t &prefix_رقم_2 () ثابت { بازگشت prefix_رقم; }

  منطقی has_calls () ثابت          { بازگشت has_calls_; }

  تهی compact ()
  {
    unsigned شمار = values.length;
    اگر (!شمار) بازگشت;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    برای (unsigned i = 1; i < شمار; i++)
    {
      /* See if we can combine op j and op i. */
      منطقی combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      اگر (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      وگرنه
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  محافظتشده:
  منطقی    parsed : 1;
  منطقی    hint_dropped : 1;
  منطقی    vsindex_dropped : 1;
  منطقی    has_prefix_ : 1;
  منطقی    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  منطقی    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  عدد_t	prefix_رقم;

  خصوصی:
  تعریفنوع parsed_مقادیر_t<parsed_cs_op_t> SUPER;
};

ساختار parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  خصوصی:
  تعریفنوع hb_vector_t<parsed_cs_str_t> SUPER;
};

ساختار cff_subset_accelerator_t
{
  ایستا cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ثابت parsed_cs_str_vec_t& parsed_charstrings,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs,
      ثابت hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (اندازه(cff_subset_accelerator_t));
    اگر (unlikely (!accel)) بازگشت nullptr;
    جدید (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    بازگشت accel;
  }

  ایستا تهی destroy (تهی* value) {
    اگر (!value) بازگشت;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ثابت parsed_cs_str_vec_t& parsed_charstrings_,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs_,
      ثابت hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
  {
    parsed_charstrings = parsed_charstrings_;
    parsed_global_subrs = parsed_global_subrs_;
    parsed_local_subrs = parsed_local_subrs_;

    // the parsed charstrings point to memory in the original CFF table so we must hold a reference
    // to it to keep the memory valid.
    original_blob = hb_blob_reference (original_blob_);
  }

  ~cff_subset_accelerator_t()
  {
    hb_blob_destroy (original_blob);
    auto *mapping = glyph_to_sid_map.get_relaxed ();
    اگر (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 خصوصی:
  hb_blob_t* original_blob;
};

ساختار subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       منطقی drop_hints_,
		       منطقی coalesce_ = نادرست) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    اگر (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    انتخاب (context.type)
    {
      حالت CSType_CharString:
	بازگشت parsed_charstring;

      حالت CSType_LocalSubr:
	اگر (likely (context.subr_رقم_2 < parsed_local_subrs->length))
	  بازگشت &(*parsed_local_subrs)[context.subr_رقم_2];
	بشکن;

      حالت CSType_GlobalSubr:
	اگر (likely (context.subr_رقم_2 < parsed_global_subrs->length))
	  بازگشت &(*parsed_global_subrs)[context.subr_رقم_2];
	بشکن;
    }
    بازگشت nullptr;
  }

  template <typename ENV>
  تهی set_current_str (ENV &env, منطقی calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    اگر (unlikely (!parsed_str))
    {
      env.set_error ();
      بازگشت;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    اگر (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    وگرنه
    {
      اگر (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        اگر (coalesce) parsed_str->enable_coalescing ();
      }
      current_parsed_str = parsed_str;
    }
  }

  parsed_cs_str_t	*current_parsed_str;

  parsed_cs_str_t	*parsed_charstring;
  parsed_cs_str_vec_t	*parsed_global_subrs;
  parsed_cs_str_vec_t	*parsed_local_subrs;
  hb_set_t      *global_closure;
  hb_set_t      *local_closure;
  منطقی	  drop_hints;
  منطقی	  coalesce;
};

ساختار subr_remap_t : hb_inc_bimap_t
{
  تهی create (ثابت hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    برای (auto old_رقم : *closure)
      add (old_رقم);

    اگر (get_population () < 1240)
      bias = 107;
    وگرنه اگر (get_population () < 33900)
      bias = 1131;
    وگرنه
      bias = 32768;
  }

  صحیح biased_رقم (unsigned صحیح old_رقم) ثابت
  {
    hb_codepoint_t new_رقم = get (old_رقم);
    بازگشت (صحیح)new_رقم - bias;
  }

  محافظتشده:
  صحیح bias;
};

ساختار subr_remaps_t
{
  subr_remaps_t (unsigned صحیح fd_شمار)
  {
    local_remaps.resize (fd_شمار);
  }

  منطقی in_error()
  {
    بازگشت local_remaps.in_error ();
  }

  تهی create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    برای (unsigned صحیح i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ساختار subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ثابت hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_شمار),
        remaps(acc_.fd_شمار)
  {}

  /* Subroutine subsetting with --no-desubroutinize runs in phases:
   *
   * 1. execute charstrings/subroutines to determine subroutine closures
   * 2. parse out all operators and numbers
   * 3. mark hint operators and operands for removal if --no-hinting
   * 4. re-encode all charstrings and subroutines with new subroutine numbers
   *
   * Phases #1 and #2 are done at the same time in collect_subrs ().
   * Phase #3 walks charstrings/subroutines forward then backward (hence parsing required),
   * because we can't tell if a number belongs to a hint op until we see the first moveto.
   *
   * Assumption: a callsubr/callgsubr operator must immediately follow a (biased) subroutine number
   * within the same charstring/subroutine, e.g., not split across a charstring and a subroutine.
   */
  منطقی subset (تهی)
  {
    unsigned fd_شمار_2 = acc.fd_شمار;
    ثابت cff_subset_accelerator_t* cff_accelerator = nullptr;
    اگر (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_شمار_2 = cff_accelerator->parsed_local_subrs.length;
    }

    اگر (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->رقم_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } وگرنه {
      parsed_charstrings.resize_exact (plan->رقم_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->شمار);

      اگر (unlikely (!parsed_local_subrs_storage.resize (fd_شمار_2))) بازگشت نادرست;

      برای (unsigned صحیح i = 0; i < acc.fd_شمار; i++)
      {
        unsigned شمار = acc.privateDicts[i].localSubrs->شمار;
        parsed_local_subrs_storage[i].resize (شمار);
        اگر (unlikely (parsed_local_subrs_storage[i].in_error ())) بازگشت نادرست;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    اگر (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      بازگشت نادرست;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    منطقی coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    برای (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ثابت hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned صحیح fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_شمار))
        بازگشت نادرست;

      اگر (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        اگر (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        وگرنه
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        ادامه;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.length);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_closure,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      اگر (unlikely (!interp.interpret (param)))
        بازگشت نادرست;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      اگر ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	اگر (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  اگر (drop.vsindex_dropped)
	    parsed_charstrings[new_glyph].set_vsindex_dropped ();
	}
      }

      /* Doing this here one by one instead of compacting all at the end
       * has massive peak-memory saving.
       *
       * The compacting both saves memory and makes further operations
       * faster.
       *
       * Not needed when tokens were coalesced during parsing.
       */
      اگر (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    اگر ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      بازگشت نادرست;

    remaps.create (closures);

    populate_subset_accelerator ();
    بازگشت درست;
  }

  منطقی encode_charstrings (str_buff_vec_t &buffArray, منطقی encode_prefix = درست) ثابت
  {
    unsigned رقم_glyphs = plan->رقم_output_glyphs ();
    اگر (unlikely (!buffArray.resize_exact (رقم_glyphs)))
      بازگشت نادرست;
    hb_codepoint_t last = 0;
    برای (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      اگر (endchar_op != OpCode_Invalid)
        برای (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned نویسه *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned صحیح  fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_شمار))
	بازگشت نادرست;
      اگر (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	بازگشت نادرست;
    }
    اگر (endchar_op != OpCode_Invalid)
      برای (; last < رقم_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned نویسه *>(endchar_str), 1);
      }

    بازگشت درست;
  }

  منطقی encode_subrs (ثابت parsed_cs_str_vec_t &subrs, ثابت subr_remap_t& remap, unsigned صحیح fd, str_buff_vec_t &buffArray) ثابت
  {
    unsigned صحیح  شمار = remap.get_population ();

    اگر (unlikely (!buffArray.resize_exact (شمار)))
      بازگشت نادرست;
    برای (unsigned صحیح new_رقم = 0; new_رقم < شمار; new_رقم++)
    {
      hb_codepoint_t old_رقم = remap.backward (new_رقم);
      أكد (old_رقم != CFF_UNDEF_CODE);

      اگر (unlikely (!encode_str (subrs[old_رقم], fd, buffArray[new_رقم])))
	بازگشت نادرست;
    }
    بازگشت درست;
  }

  منطقی encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    بازگشت encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  منطقی encode_localsubrs (unsigned صحیح fd, str_buff_vec_t &buffArray) ثابت
  {
    بازگشت encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  محافظتشده:
  ساختار drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (نادرست),
	ends_in_hint (نادرست),
	all_dropped (نادرست),
	vsindex_dropped (نادرست) {}

    منطقی  seen_moveto;
    منطقی  ends_in_hint;
    منطقی  all_dropped;
    منطقی  vsindex_dropped;
  };

  منطقی drop_hints_in_subr (parsed_cs_str_t &str, unsigned صحیح pos,
			   parsed_cs_str_vec_t &subrs, unsigned صحیح subr_رقم_2,
			   ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = نادرست;
    منطقی has_hint = drop_hints_in_str (subrs[subr_رقم_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    اگر (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      اگر (!str.at_end (pos))
	drop.ends_in_hint = نادرست;
    }
    وگرنه اگر (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    بازگشت has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  منطقی drop_hints_in_str (parsed_cs_str_t &str, ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    منطقی  seen_hint = نادرست;

    unsigned شمار = str.values.length;
    auto *values = str.values.arrayZ;
    برای (unsigned صحیح pos = 0; pos < شمار; pos++)
    {
      منطقی  has_hint = نادرست;
      انتخاب (values[pos].op)
      {
	حالت OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_رقم_2,
					param, drop);
	  بشکن;

	حالت OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_رقم_2,
					param, drop);
	  بشکن;

	حالت OpCode_rmoveto:
	حالت OpCode_hmoveto:
	حالت OpCode_vmoveto:
	  drop.seen_moveto = درست;
	  بشکن;

	حالت OpCode_hintmask:
	حالت OpCode_cntrmask:
	  اگر (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    بشکن;
	  }
	  HB_FALLTHROUGH;

	حالت OpCode_hstemhm:
	حالت OpCode_vstemhm:
	حالت OpCode_hstem:
	حالت OpCode_vstem:
	  has_hint = درست;
	  values[pos].set_hinting ();
	  اگر (str.at_end (pos))
	    drop.ends_in_hint = درست;
	  بشکن;

	حالت OpCode_dotsection:
	  values[pos].set_hinting ();
	  بشکن;

	پیشفرض:
	  /* NONE */
	  بشکن;
      }
      اگر (has_hint)
      {
	برای (صحیح i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  اگر (csop.is_hinting ())
	    بشکن;
	  csop.set_hinting ();
	  اگر (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = درست;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = درست;
    برای (unsigned صحیح pos = 0; pos < شمار; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      اگر (csop.op == OpCode_return)
	بشکن;
      اگر (!csop.is_hinting ())
      {
	drop.all_dropped = نادرست;
	بشکن;
      }
    }

    بازگشت seen_hint;
  }

  منطقی closure_subroutines (ثابت parsed_cs_str_vec_t& global_subrs,
                            ثابت hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    برای (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned صحیح fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_شمار))
        بازگشت نادرست;

      // Note: const cast is safe here because the collect_subr_refs_in_str only performs a
      //       closure and does not modify any of the charstrings.
      subr_subset_param_t  param (const_cast<parsed_cs_str_t*> (&get_parsed_charstring (new_glyph)),
                                  const_cast<parsed_cs_str_vec_t*> (&global_subrs),
                                  const_cast<parsed_cs_str_vec_t*> (&local_subrs[fd]),
                                  &closures.global_closure,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
      collect_subr_refs_in_str (get_parsed_charstring (new_glyph), param);
    }

    بازگشت درست;
  }

  تهی collect_subr_refs_in_subr (unsigned صحیح subr_رقم_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ثابت subr_subset_param_t &param)
  {
    اگر (closure->has (subr_رقم_2))
      بازگشت;
    closure->add (subr_رقم_2);
    collect_subr_refs_in_str (subrs[subr_رقم_2], param);
  }

  تهی collect_subr_refs_in_str (ثابت parsed_cs_str_t &str,
                                 ثابت subr_subset_param_t &param)
  {
    اگر (!str.has_calls ())
      بازگشت;

    برای (auto &opstr : str.values)
    {
      اگر (!param.drop_hints || !opstr.is_hinting ())
      {
	انتخاب (opstr.op)
	{
	  حالت OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_رقم_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    بشکن;

	  حالت OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_رقم_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    بشکن;

	  پیشفرض: بشکن;
	}
      }
    }
  }

  منطقی encode_str (ثابت parsed_cs_str_t &str, ثابت unsigned صحیح fd, str_buff_t &buff, منطقی encode_prefix = درست) ثابت
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    منطقی hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    اگر (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_رقم_cs (str.prefix_رقم_2 ());
      اگر (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    برای (auto &opstr : str.values)
    {
      size += opstr.length;
      اگر (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    اگر (!buff.alloc_exact (buff.length + size))
      بازگشت نادرست;

    برای (auto &opstr : str.values)
    {
      اگر (hinting || !opstr.is_hinting ())
      {
	انتخاب (opstr.op)
	{
	  حالت OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_رقم (opstr.subr_رقم_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    بشکن;

	  حالت OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_رقم (opstr.subr_رقم_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    بشکن;

	  پیشفرض:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    بشکن;
	}
      }
    }
    بازگشت !encoder.in_error ();
  }

  تهی compact_parsed_subrs () ثابت
  {
    برای (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    برای (auto &vec : parsed_local_subrs_storage)
      برای (auto &cs : vec)
	cs.compact ();
  }

  تهی populate_subset_accelerator () ثابت
  {
    اگر (!plan->inprogress_accelerator) بازگشت;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ثابت parsed_cs_str_t& get_parsed_charstring (unsigned i) ثابت
  {
    اگر (cached_charstrings) بازگشت *(cached_charstrings[i]);
    بازگشت parsed_charstrings[i];
  }

  محافظتشده:
  ثابت ACC			&acc;
  ثابت hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ثابت parsed_cs_str_t*>     cached_charstrings;
  ثابت parsed_cs_str_vec_t*              parsed_global_subrs;
  ثابت hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  خصوصی:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  تعریفنوع typename SUBRS::شمار_type subr_شمار_type;
};

} /* namespace CFF */

HB_INTERNAL منطقی
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			    unsigned صحیح fd_شمار,
			    ثابت CFF::FDSelect &src, /* IN */
			    unsigned صحیح src_رقم_glyphs,
			    unsigned صحیح &subset_fd_شمار /* OUT */,
			    unsigned صحیح &subset_fdselect_size /* OUT */,
			    unsigned صحیح &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL منطقی
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned صحیح رقم_glyphs,
			  ثابت CFF::FDSelect &src,
			  unsigned صحیح fd_شمار_2,
			  unsigned صحیح fdselect_format,
			  unsigned صحیح size,
			  ثابت hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
