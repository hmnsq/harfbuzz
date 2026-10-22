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

#ضمن "hb.hh"

#ضمن "hb-subset-plan.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
ساكن مضمن unsigned
subset_get_fd (ثابت ACC &acc, hb_codepoint_t glyph)
{
  ܦܢܐ likely (glyph < acc.num_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
بنية str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  فراغ reset () { buff.reset (); }

  فراغ encode_byte (unsigned حرف b)
  {
    ܐܢ (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    ܐܚܪܢܐ
      buff.push (b);
  }

  فراغ encode_int (صحيح v)
  {
    ܐܢ ((-1131 <= v) && (v <= 1131))
    {
      ܐܢ ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      ܐܚܪܢܐ ܐܢ (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      ܐܚܪܢܐ
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    ܐܚܪܢܐ
    {
      ܐܢ (unlikely (v < -32768))
	v = -32768;
      ܐܚܪܢܐ ܐܢ (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  فراغ encode_num_cs (ثابت number_t& n)
  {
    ܐܢ (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    ܐܚܪܢܐ
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
  فراغ encode_num_tp (ثابت number_t& n)
  {
    ܐܢ (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    ܐܚܪܢܐ
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      مزدوج v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      حرف buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
      snprintf (buf, حجم (buf), "%.8G", v);
      (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

      حرف *s = buf;
      size_t len;
      حرف *comma = strchr (s, ',');
      ܐܢ (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      ܐܢ (s[0] == '0' && s[1] == '.')
	s++;
      ܐܚܪܢܐ ܐܢ (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      ܐܚܪܢܐ ܐܢ ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	حرف *s2 = s + exponent - 1;
	ܟܡܐ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, حجم (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      ܐܚܪܢܐ
      {
	حرف *dot = strchr (s, '.');
	حرف *e = strchr (s, 'E');
	ܐܢ (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  صحيح exponent = atoi (e + 1);
	  صحيح new_exponent = exponent - (e - (dot + 1));
	  ܐܢ (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  ܐܚܪܢܐ
	    snprintf (e - 1, حجم (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      ܐܢ ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	صحيح sign = s[0] == '-';
	حرف *s2 = s + sign + 1;
	ܟܡܐ (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, حجم (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<حرف> nibbles;
      ܟܡܐ (*s)
      {
	حرف c = s[0];
	s++;

	بدل (c)
	{
	  حالة 'E':
	  {
	    حرف c2 = *s;
	    ܐܢ (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } ܐܚܪܢܐ {
	      ܐܢ (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    ܐܢ (*s == '0')
	      s++;
	    استمر;
	  }

	  حالة '.':
	    nibbles.push (0x0A); // .
	    استمر;

	  حالة '-':
	    nibbles.push (0x0E); // -
	    استمر;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      ܐܢ (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned count = nibbles.length;
      لكل (unsigned i = 0; i < count; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  فراغ encode_op (op_code_t op)
  {
    ܐܢ (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    ܐܚܪܢܐ
      encode_byte (op);
  }

  فراغ copy_str (ثابت unsigned حرف *str, unsigned length)
  {
    أكد ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  منطقي in_error () ثابت { ܦܢܐ buff.in_error (); }

  محمي:

  str_buff_t &buff;
};

بنية cff_sub_table_info_t {
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
بنية cff_top_dict_op_serializer_t : op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت OPSTR &opstr,
		  ثابت cff_sub_table_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      حالة OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      حالة OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      افتراضي:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (ܫܪܪܐ);
  }
};

بنية cff_font_dict_op_serializer_t : op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت table_info_t &privateDictInfo) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ܐܢ (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    ܐܚܪܢܐ
    {
      unsigned حرف *d = c->allocate_size<unsigned حرف> (opstr.length);
      ܐܢ (unlikely (!d)) return_trace (ܕܓܠܐ);
      /* Faster than hb_memcpy for small strings. */
      لكل (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (ܫܪܪܐ);
  }
};

/* CharString command for specialization */
بنية cs_command_t
{
  hb_vector_t<number_t> args;
  hb_vector_t<unsigned حرف> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

تعريفنوع hb_vector_t<cs_command_t> *cs_command_vec_t;

بنية cff2_instancing_plan_t;

بنية flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   منطقي drop_hints_,
                   ثابت hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  منطقي	drop_hints;
  ثابت hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ثابت cff2_instancing_plan_t *instancer = nullptr;
  منطقي emitted_blend = ܕܓܠܐ;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
بنية subr_flattener_t
{
  subr_flattener_t (ثابت ACC &acc_,
		    ثابت hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  منطقي flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned count = plan->num_output_glyphs ();
    ܐܢ (!flat_charstrings.resize_exact (count))
      ܦܢܐ ܕܓܠܐ;
    لكل (unsigned صحيح i = 0; i < count; i++)
    {
      hb_codepoint_t  glyph;
      ܐܢ (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	ܐܢ (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	استمر;
      }
      ثابت hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned صحيح fd = subset_get_fd (acc, glyph);
      ܐܢ (unlikely (fd >= acc.fdCount))
	ܦܢܐ ܕܓܠܐ;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (منطقي) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      ܐܢ (unlikely (!interp.interpret (param)))
	ܦܢܐ ܕܓܠܐ;
    }
    ܦܢܐ ܫܪܪܐ;
  }

  ثابت ACC &acc;
  ثابت hb_subset_plan_t *plan;
};

بنية subr_closures_t
{
  subr_closures_t (unsigned صحيح fd_count) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_count);
  }

  فراغ reset ()
  {
    global_closure.clear();
    لكل (unsigned صحيح i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  منطقي in_error () ثابت { ܦܢܐ local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

بنية parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned صحيح subr_num_ = 0) :
    subr_num (subr_num_) {}

  منطقي is_hinting () ثابت { ܦܢܐ hinting_flag; }
  فراغ set_hinting ()       { hinting_flag = ܫܪܪܐ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  محمي:
  منطقي	  hinting_flag = ܕܓܠܐ;

  عام:
  uint16_t subr_num;
};

بنية parsed_cs_str_t : parsed_values_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (ܕܓܠܐ),
    hint_dropped (ܕܓܠܐ),
    has_prefix_ (ܕܓܠܐ),
    has_calls_ (ܕܓܠܐ),
    coalescing_ (ܕܓܠܐ)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  فراغ add_op (op_code_t op, ثابت byte_str_ref_t& str_ref)
  {
    ܐܢ (is_parsed ()) ܦܢܐ;
    ܐܢ (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      ܐܢ (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      ܦܢܐ;
    }
    SUPER::add_op (op, str_ref);
  }

  فراغ add_call_op (op_code_t op, ثابت byte_str_ref_t& str_ref, unsigned صحيح subr_num)
  {
    ܐܢ (is_parsed ()) ܦܢܐ;
    has_calls_ = ܫܪܪܐ;

    ܐܢ (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_num});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      ܦܢܐ;
    }

    /* Pop the subroutine number. */
    values.pop ();

    SUPER::add_op (op, str_ref, {subr_num});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  فراغ flush_segment (ثابت byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    ܐܢ (end <= start) ܦܢܐ;
    ܟܡܐ (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      ܐܢ (unlikely (!arr.length)) اكسر;
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
  فراغ flush_coalesced (ثابت byte_str_ref_t& str_ref)
  {
    ܐܢ (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  فراغ enable_coalescing () { coalescing_ = ܫܪܪܐ; }
  منطقي is_coalescing () ثابت { ܦܢܐ coalescing_; }

  فراغ set_prefix (ثابت number_t &num, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = ܫܪܪܐ;
    prefix_op_ = op;
    prefix_num_ = num;
  }

  منطقي at_end (unsigned صحيح pos) ثابت
  {
    ܦܢܐ ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  منطقي is_parsed () ثابت { ܦܢܐ parsed; }
  فراغ set_parsed ()      { parsed = ܫܪܪܐ; }

  منطقي is_hint_dropped () ثابت { ܦܢܐ hint_dropped; }
  فراغ set_hint_dropped ()      { hint_dropped = ܫܪܪܐ; }

  منطقي is_vsindex_dropped () ثابت { ܦܢܐ vsindex_dropped; }
  فراغ set_vsindex_dropped ()      { vsindex_dropped = ܫܪܪܐ; }

  منطقي has_prefix () ثابت          { ܦܢܐ has_prefix_; }
  op_code_t prefix_op () ثابت         { ܦܢܐ prefix_op_; }
  ثابت number_t &prefix_num () ثابت { ܦܢܐ prefix_num_; }

  منطقي has_calls () ثابت          { ܦܢܐ has_calls_; }

  فراغ compact ()
  {
    unsigned count = values.length;
    ܐܢ (!count) ܦܢܐ;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    لكل (unsigned i = 1; i < count; i++)
    {
      /* See if we can combine op j and op i. */
      منطقي combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      ܐܢ (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      ܐܚܪܢܐ
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  محمي:
  منطقي    parsed : 1;
  منطقي    hint_dropped : 1;
  منطقي    vsindex_dropped : 1;
  منطقي    has_prefix_ : 1;
  منطقي    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  منطقي    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  number_t	prefix_num_;

  خاص:
  تعريفنوع parsed_values_t<parsed_cs_op_t> SUPER;
};

بنية parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  خاص:
  تعريفنوع hb_vector_t<parsed_cs_str_t> SUPER;
};

بنية cff_subset_accelerator_t
{
  ساكن cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ثابت parsed_cs_str_vec_t& parsed_charstrings,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs,
      ثابت hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (حجم(cff_subset_accelerator_t));
    ܐܢ (unlikely (!accel)) ܦܢܐ nullptr;
    جديد (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ܦܢܐ accel;
  }

  ساكن فراغ destroy (فراغ* value) {
    ܐܢ (!value) ܦܢܐ;

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
    ܐܢ (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 خاص:
  hb_blob_t* original_blob;
};

بنية subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       منطقي drop_hints_,
		       منطقي coalesce_ = ܕܓܠܐ) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    ܐܢ (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    بدل (context.type)
    {
      حالة CSType_CharString:
	ܦܢܐ parsed_charstring;

      حالة CSType_LocalSubr:
	ܐܢ (likely (context.subr_num < parsed_local_subrs->length))
	  ܦܢܐ &(*parsed_local_subrs)[context.subr_num];
	اكسر;

      حالة CSType_GlobalSubr:
	ܐܢ (likely (context.subr_num < parsed_global_subrs->length))
	  ܦܢܐ &(*parsed_global_subrs)[context.subr_num];
	اكسر;
    }
    ܦܢܐ nullptr;
  }

  template <typename ENV>
  فراغ set_current_str (ENV &env, منطقي calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    ܐܢ (unlikely (!parsed_str))
    {
      env.set_error ();
      ܦܢܐ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    ܐܢ (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    ܐܚܪܢܐ
    {
      ܐܢ (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        ܐܢ (coalesce) parsed_str->enable_coalescing ();
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
  منطقي	  drop_hints;
  منطقي	  coalesce;
};

بنية subr_remap_t : hb_inc_bimap_t
{
  فراغ create (ثابت hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    لكل (auto old_num : *closure)
      add (old_num);

    ܐܢ (get_population () < 1240)
      bias = 107;
    ܐܚܪܢܐ ܐܢ (get_population () < 33900)
      bias = 1131;
    ܐܚܪܢܐ
      bias = 32768;
  }

  صحيح biased_num (unsigned صحيح old_num) ثابت
  {
    hb_codepoint_t new_num = get (old_num);
    ܦܢܐ (صحيح)new_num - bias;
  }

  محمي:
  صحيح bias;
};

بنية subr_remaps_t
{
  subr_remaps_t (unsigned صحيح fdCount)
  {
    local_remaps.resize (fdCount);
  }

  منطقي in_error()
  {
    ܦܢܐ local_remaps.in_error ();
  }

  فراغ create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    لكل (unsigned صحيح i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
بنية subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ثابت hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fdCount),
        remaps(acc_.fdCount)
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
  منطقي subset (فراغ)
  {
    unsigned fd_count = acc.fdCount;
    ثابت cff_subset_accelerator_t* cff_accelerator = nullptr;
    ܐܢ (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_count = cff_accelerator->parsed_local_subrs.length;
    }

    ܐܢ (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->num_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } ܐܚܪܢܐ {
      parsed_charstrings.resize_exact (plan->num_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->count);

      ܐܢ (unlikely (!parsed_local_subrs_storage.resize (fd_count))) ܦܢܐ ܕܓܠܐ;

      لكل (unsigned صحيح i = 0; i < acc.fdCount; i++)
      {
        unsigned count = acc.privateDicts[i].localSubrs->count;
        parsed_local_subrs_storage[i].resize (count);
        ܐܢ (unlikely (parsed_local_subrs_storage[i].in_error ())) ܦܢܐ ܕܓܠܐ;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    ܐܢ (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      ܦܢܐ ܕܓܠܐ;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    منطقي coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    لكل (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ثابت hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned صحيح fd = subset_get_fd (acc, old_glyph);
      ܐܢ (unlikely (fd >= acc.fdCount))
        ܦܢܐ ܕܓܠܐ;

      ܐܢ (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        ܐܢ (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        ܐܚܪܢܐ
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        استمر;
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

      ܐܢ (unlikely (!interp.interpret (param)))
        ܦܢܐ ܕܓܠܐ;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      ܐܢ ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	ܐܢ (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  ܐܢ (drop.vsindex_dropped)
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
      ܐܢ (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    ܐܢ ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      ܦܢܐ ܕܓܠܐ;

    remaps.create (closures);

    populate_subset_accelerator ();
    ܦܢܐ ܫܪܪܐ;
  }

  منطقي encode_charstrings (str_buff_vec_t &buffArray, منطقي encode_prefix = ܫܪܪܐ) ثابت
  {
    unsigned num_glyphs = plan->num_output_glyphs ();
    ܐܢ (unlikely (!buffArray.resize_exact (num_glyphs)))
      ܦܢܐ ܕܓܠܐ;
    hb_codepoint_t last = 0;
    لكل (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      ܐܢ (endchar_op != OpCode_Invalid)
        لكل (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned حرف *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned صحيح  fd = subset_get_fd (acc, old_glyph);
      ܐܢ (unlikely (fd >= acc.fdCount))
	ܦܢܐ ܕܓܠܐ;
      ܐܢ (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	ܦܢܐ ܕܓܠܐ;
    }
    ܐܢ (endchar_op != OpCode_Invalid)
      لكل (; last < num_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned حرف *>(endchar_str), 1);
      }

    ܦܢܐ ܫܪܪܐ;
  }

  منطقي encode_subrs (ثابت parsed_cs_str_vec_t &subrs, ثابت subr_remap_t& remap, unsigned صحيح fd, str_buff_vec_t &buffArray) ثابت
  {
    unsigned صحيح  count = remap.get_population ();

    ܐܢ (unlikely (!buffArray.resize_exact (count)))
      ܦܢܐ ܕܓܠܐ;
    لكل (unsigned صحيح new_num = 0; new_num < count; new_num++)
    {
      hb_codepoint_t old_num = remap.backward (new_num);
      أكد (old_num != CFF_UNDEF_CODE);

      ܐܢ (unlikely (!encode_str (subrs[old_num], fd, buffArray[new_num])))
	ܦܢܐ ܕܓܠܐ;
    }
    ܦܢܐ ܫܪܪܐ;
  }

  منطقي encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ܦܢܐ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  منطقي encode_localsubrs (unsigned صحيح fd, str_buff_vec_t &buffArray) ثابت
  {
    ܦܢܐ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  محمي:
  بنية drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (ܕܓܠܐ),
	ends_in_hint (ܕܓܠܐ),
	all_dropped (ܕܓܠܐ),
	vsindex_dropped (ܕܓܠܐ) {}

    منطقي  seen_moveto;
    منطقي  ends_in_hint;
    منطقي  all_dropped;
    منطقي  vsindex_dropped;
  };

  منطقي drop_hints_in_subr (parsed_cs_str_t &str, unsigned صحيح pos,
			   parsed_cs_str_vec_t &subrs, unsigned صحيح subr_num,
			   ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = ܕܓܠܐ;
    منطقي has_hint = drop_hints_in_str (subrs[subr_num], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    ܐܢ (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      ܐܢ (!str.at_end (pos))
	drop.ends_in_hint = ܕܓܠܐ;
    }
    ܐܚܪܢܐ ܐܢ (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    ܦܢܐ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  منطقي drop_hints_in_str (parsed_cs_str_t &str, ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    منطقي  seen_hint = ܕܓܠܐ;

    unsigned count = str.values.length;
    auto *values = str.values.arrayZ;
    لكل (unsigned صحيح pos = 0; pos < count; pos++)
    {
      منطقي  has_hint = ܕܓܠܐ;
      بدل (values[pos].op)
      {
	حالة OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_num,
					param, drop);
	  اكسر;

	حالة OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_num,
					param, drop);
	  اكسر;

	حالة OpCode_rmoveto:
	حالة OpCode_hmoveto:
	حالة OpCode_vmoveto:
	  drop.seen_moveto = ܫܪܪܐ;
	  اكسر;

	حالة OpCode_hintmask:
	حالة OpCode_cntrmask:
	  ܐܢ (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    اكسر;
	  }
	  HB_FALLTHROUGH;

	حالة OpCode_hstemhm:
	حالة OpCode_vstemhm:
	حالة OpCode_hstem:
	حالة OpCode_vstem:
	  has_hint = ܫܪܪܐ;
	  values[pos].set_hinting ();
	  ܐܢ (str.at_end (pos))
	    drop.ends_in_hint = ܫܪܪܐ;
	  اكسر;

	حالة OpCode_dotsection:
	  values[pos].set_hinting ();
	  اكسر;

	افتراضي:
	  /* NONE */
	  اكسر;
      }
      ܐܢ (has_hint)
      {
	لكل (صحيح i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  ܐܢ (csop.is_hinting ())
	    اكسر;
	  csop.set_hinting ();
	  ܐܢ (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = ܫܪܪܐ;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = ܫܪܪܐ;
    لكل (unsigned صحيح pos = 0; pos < count; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      ܐܢ (csop.op == OpCode_return)
	اكسر;
      ܐܢ (!csop.is_hinting ())
      {
	drop.all_dropped = ܕܓܠܐ;
	اكسر;
      }
    }

    ܦܢܐ seen_hint;
  }

  منطقي closure_subroutines (ثابت parsed_cs_str_vec_t& global_subrs,
                            ثابت hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    لكل (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned صحيح fd = subset_get_fd (acc, old_glyph);
      ܐܢ (unlikely (fd >= acc.fdCount))
        ܦܢܐ ܕܓܠܐ;

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

    ܦܢܐ ܫܪܪܐ;
  }

  فراغ collect_subr_refs_in_subr (unsigned صحيح subr_num, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ثابت subr_subset_param_t &param)
  {
    ܐܢ (closure->has (subr_num))
      ܦܢܐ;
    closure->add (subr_num);
    collect_subr_refs_in_str (subrs[subr_num], param);
  }

  فراغ collect_subr_refs_in_str (ثابت parsed_cs_str_t &str,
                                 ثابت subr_subset_param_t &param)
  {
    ܐܢ (!str.has_calls ())
      ܦܢܐ;

    لكل (auto &opstr : str.values)
    {
      ܐܢ (!param.drop_hints || !opstr.is_hinting ())
      {
	بدل (opstr.op)
	{
	  حالة OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_num, *param.parsed_local_subrs,
				       param.local_closure, param);
	    اكسر;

	  حالة OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_num, *param.parsed_global_subrs,
				       param.global_closure, param);
	    اكسر;

	  افتراضي: اكسر;
	}
      }
    }
  }

  منطقي encode_str (ثابت parsed_cs_str_t &str, ثابت unsigned صحيح fd, str_buff_t &buff, منطقي encode_prefix = ܫܪܪܐ) ثابت
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    منطقي hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    ܐܢ (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_num_cs (str.prefix_num ());
      ܐܢ (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    لكل (auto &opstr : str.values)
    {
      size += opstr.length;
      ܐܢ (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    ܐܢ (!buff.alloc_exact (buff.length + size))
      ܦܢܐ ܕܓܠܐ;

    لكل (auto &opstr : str.values)
    {
      ܐܢ (hinting || !opstr.is_hinting ())
      {
	بدل (opstr.op)
	{
	  حالة OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_num (opstr.subr_num));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    اكسر;

	  حالة OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_num (opstr.subr_num));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    اكسر;

	  افتراضي:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    اكسر;
	}
      }
    }
    ܦܢܐ !encoder.in_error ();
  }

  فراغ compact_parsed_subrs () ثابت
  {
    لكل (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    لكل (auto &vec : parsed_local_subrs_storage)
      لكل (auto &cs : vec)
	cs.compact ();
  }

  فراغ populate_subset_accelerator () ثابت
  {
    ܐܢ (!plan->inprogress_accelerator) ܦܢܐ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ثابت parsed_cs_str_t& get_parsed_charstring (unsigned i) ثابت
  {
    ܐܢ (cached_charstrings) ܦܢܐ *(cached_charstrings[i]);
    ܦܢܐ parsed_charstrings[i];
  }

  محمي:
  ثابت ACC			&acc;
  ثابت hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ثابت parsed_cs_str_t*>     cached_charstrings;
  ثابت parsed_cs_str_vec_t*              parsed_global_subrs;
  ثابت hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  خاص:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  تعريفنوع typename SUBRS::count_type subr_count_type;
};

} /* namespace CFF */

HB_INTERNAL منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			    unsigned صحيح fdCount,
			    ثابت CFF::FDSelect &src, /* IN */
			    unsigned صحيح src_num_glyphs,
			    unsigned صحيح &subset_fd_count /* OUT */,
			    unsigned صحيح &subset_fdselect_size /* OUT */,
			    unsigned صحيح &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned صحيح num_glyphs,
			  ثابت CFF::FDSelect &src,
			  unsigned صحيح fd_count,
			  unsigned صحيح fdselect_format,
			  unsigned صحيح size,
			  ثابت hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
