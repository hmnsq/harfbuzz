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

#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-subset-plan.hh"
#অন্তর্ভাৱ "hb-cff-interp-cs-common.hh"

নামাকাশ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
স্থির অন্তঃস্থ unsigned
subset_get_fd (ধ্রুৱ ACC &acc, hb_codepoint_t glyph)
{
  ঘূৰাই likely (glyph < acc.অঙ্ক_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
সংরচনা str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  শূন্য reset () { buff.reset (); }

  শূন্য encode_byte (unsigned ৱর্ণ b)
  {
    যদি (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    অন্যথা
      buff.push (b);
  }

  শূন্য encode_int (পূর্ণাঙ্ক v)
  {
    যদি ((-1131 <= v) && (v <= 1131))
    {
      যদি ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      অন্যথা যদি (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      অন্যথা
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    অন্যথা
    {
      যদি (unlikely (v < -32768))
	v = -32768;
      অন্যথা যদি (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  শূন্য encode_অঙ্ক_cs (ধ্রুৱ সংখ্যা_t& n)
  {
    যদি (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    অন্যথা
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
  শূন্য encode_অঙ্ক_tp (ধ্রুৱ সংখ্যা_t& n)
  {
    যদি (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    অন্যথা
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      দ্ৱিগুণ v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      ৱর্ণ buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", রিক্ত));
      snprintf (buf, পরিমাণ (buf), "%.8G", v);
      (শূন্য) hb_uselocale (((শূন্য) freelocale (clocale), oldlocale));

      ৱর্ণ *s = buf;
      size_t len;
      ৱর্ণ *comma = strchr (s, ',');
      যদি (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      যদি (s[0] == '0' && s[1] == '.')
	s++;
      অন্যথা যদি (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      অন্যথা যদি ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	ৱর্ণ *s2 = s + exponent - 1;
	যেতিয়ালৈ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, পরিমাণ (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      অন্যথা
      {
	ৱর্ণ *dot = strchr (s, '.');
	ৱর্ণ *e = strchr (s, 'E');
	যদি (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  পূর্ণাঙ্ক exponent = atoi (e + 1);
	  পূর্ণাঙ্ক new_exponent = exponent - (e - (dot + 1));
	  যদি (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  অন্যথা
	    snprintf (e - 1, পরিমাণ (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      যদি ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	পূর্ণাঙ্ক sign = s[0] == '-';
	ৱর্ণ *s2 = s + sign + 1;
	যেতিয়ালৈ (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, পরিমাণ (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<ৱর্ণ> nibbles;
      যেতিয়ালৈ (*s)
      {
	ৱর্ণ c = s[0];
	s++;

	ৱিকল্প (c)
	{
	  পক্ষ 'E':
	  {
	    ৱর্ণ c2 = *s;
	    যদি (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } অন্যথা {
	      যদি (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    যদি (*s == '0')
	      s++;
	    অনুৱর্ত;
	  }

	  পক্ষ '.':
	    nibbles.push (0x0A); // .
	    অনুৱর্ত;

	  পক্ষ '-':
	    nibbles.push (0x0E); // -
	    অনুৱর্ত;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      যদি (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned গুনতি = nibbles.length;
      বাবে (unsigned i = 0; i < গুনতি; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  শূন্য encode_op (op_code_t op)
  {
    যদি (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    অন্যথা
      encode_byte (op);
  }

  শূন্য copy_str (ধ্রুৱ unsigned ৱর্ণ *str, unsigned length)
  {
    প্রতিজ্ঞা ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই buff.in_error (); }

  রক্ষিত:

  str_buff_t &buff;
};

সংরচনা cff_sub_table_info_t {
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
সংরচনা cff_শীৰ্ষ_dict_op_serializer_t : op_serializer_t
{
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ OPSTR &opstr,
		  ধ্রুৱ cff_sub_table_info_t &info) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    ৱিকল্প (opstr.op)
    {
      পক্ষ OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      পক্ষ OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      পক্ষ OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      স্ৱভাৱ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (সঁচা);
  }
};

সংরচনা cff_font_dict_op_serializer_t : op_serializer_t
{
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ op_str_t &opstr,
		  ধ্রুৱ table_info_t &privateDictInfo) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    যদি (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    অন্যথা
    {
      unsigned ৱর্ণ *d = c->allocate_size<unsigned ৱর্ণ> (opstr.length);
      যদি (unlikely (!d)) return_trace (মিছা);
      /* Faster than hb_memcpy for small strings. */
      বাবে (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (সঁচা);
  }
};

/* CharString command for specialization */
সংরচনা cs_command_t
{
  hb_vector_t<সংখ্যা_t> args;
  hb_vector_t<unsigned ৱর্ণ> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

প্রকারনির্দেশ hb_vector_t<cs_command_t> *cs_command_vec_t;

সংরচনা cff2_instancing_plan_t;

সংরচনা flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   তার্কিক drop_hints_,
                   ধ্রুৱ hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  তার্কিক	drop_hints;
  ধ্রুৱ hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ধ্রুৱ cff2_instancing_plan_t *instancer = nullptr;
  তার্কিক emitted_blend = মিছা;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
সংরচনা subr_flattener_t
{
  subr_flattener_t (ধ্রুৱ ACC &acc_,
		    ধ্রুৱ hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  তার্কিক flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned গুনতি = plan->অঙ্ক_output_glyphs ();
    যদি (!flat_charstrings.resize_exact (গুনতি))
      ঘূৰাই মিছা;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t  glyph;
      যদি (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	যদি (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	অনুৱর্ত;
      }
      ধ্রুৱ hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned পূর্ণাঙ্ক fd = subset_get_fd (acc, glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
	ঘূৰাই মিছা;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (তার্কিক) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      যদি (unlikely (!interp.interpret (param)))
	ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
  }

  ধ্রুৱ ACC &acc;
  ধ্রুৱ hb_subset_plan_t *plan;
};

সংরচনা subr_closures_t
{
  subr_closures_t (unsigned পূর্ণাঙ্ক fd_গুনতি_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_গুনতি_2);
  }

  শূন্য reset ()
  {
    global_closure.clear();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

সংরচনা parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned পূর্ণাঙ্ক subr_অঙ্ক = 0) :
    subr_অঙ্ক_2 (subr_অঙ্ক) {}

  তার্কিক is_hinting () ধ্রুৱ { ঘূৰাই hinting_flag; }
  শূন্য set_hinting ()       { hinting_flag = সঁচা; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  রক্ষিত:
  তার্কিক	  hinting_flag = মিছা;

  সার্ৱজনিক:
  uint16_t subr_অঙ্ক_2;
};

সংরচনা parsed_cs_str_t : parsed_মানসমূহ_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (মিছা),
    hint_dropped (মিছা),
    has_prefix_ (মিছা),
    has_calls_ (মিছা),
    coalescing_ (মিছা)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  শূন্য add_op (op_code_t op, ধ্রুৱ byte_str_ref_t& str_ref)
  {
    যদি (is_parsed ()) ঘূৰাই;
    যদি (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      যদি (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      ঘূৰাই;
    }
    SUPER::add_op (op, str_ref);
  }

  শূন্য add_call_op (op_code_t op, ধ্রুৱ byte_str_ref_t& str_ref, unsigned পূর্ণাঙ্ক subr_অঙ্ক_2)
  {
    যদি (is_parsed ()) ঘূৰাই;
    has_calls_ = সঁচা;

    যদি (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_অঙ্ক_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      ঘূৰাই;
    }

    /* Pop the subroutine number. */
    values.বেৰ_কৰো ();

    SUPER::add_op (op, str_ref, {subr_অঙ্ক_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  শূন্য flush_segment (ধ্রুৱ byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    যদি (end <= start) ঘূৰাই;
    যেতিয়ালৈ (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      যদি (unlikely (!arr.length)) ভঙ্গ;
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
  শূন্য flush_coalesced (ধ্রুৱ byte_str_ref_t& str_ref)
  {
    যদি (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  শূন্য enable_coalescing () { coalescing_ = সঁচা; }
  তার্কিক is_coalescing () ধ্রুৱ { ঘূৰাই coalescing_; }

  শূন্য set_prefix (ধ্রুৱ সংখ্যা_t &অঙ্ক, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = সঁচা;
    prefix_op_ = op;
    prefix_অঙ্ক = অঙ্ক;
  }

  তার্কিক at_end (unsigned পূর্ণাঙ্ক pos) ধ্রুৱ
  {
    ঘূৰাই ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  তার্কিক is_parsed () ধ্রুৱ { ঘূৰাই parsed; }
  শূন্য set_parsed ()      { parsed = সঁচা; }

  তার্কিক is_hint_dropped () ধ্রুৱ { ঘূৰাই hint_dropped; }
  শূন্য set_hint_dropped ()      { hint_dropped = সঁচা; }

  তার্কিক is_vsindex_dropped () ধ্রুৱ { ঘূৰাই vsindex_dropped; }
  শূন্য set_vsindex_dropped ()      { vsindex_dropped = সঁচা; }

  তার্কিক has_prefix () ধ্রুৱ          { ঘূৰাই has_prefix_; }
  op_code_t prefix_op () ধ্রুৱ         { ঘূৰাই prefix_op_; }
  ধ্রুৱ সংখ্যা_t &prefix_অঙ্ক_2 () ধ্রুৱ { ঘূৰাই prefix_অঙ্ক; }

  তার্কিক has_calls () ধ্রুৱ          { ঘূৰাই has_calls_; }

  শূন্য compact ()
  {
    unsigned গুনতি = values.length;
    যদি (!গুনতি) ঘূৰাই;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    বাবে (unsigned i = 1; i < গুনতি; i++)
    {
      /* See if we can combine op j and op i. */
      তার্কিক combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      যদি (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      অন্যথা
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  রক্ষিত:
  তার্কিক    parsed : 1;
  তার্কিক    hint_dropped : 1;
  তার্কিক    vsindex_dropped : 1;
  তার্কিক    has_prefix_ : 1;
  তার্কিক    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  তার্কিক    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  সংখ্যা_t	prefix_অঙ্ক;

  গুপ্ত:
  প্রকারনির্দেশ parsed_মানসমূহ_t<parsed_cs_op_t> SUPER;
};

সংরচনা parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  গুপ্ত:
  প্রকারনির্দেশ hb_vector_t<parsed_cs_str_t> SUPER;
};

সংরচনা cff_subset_accelerator_t
{
  স্থির cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ধ্রুৱ parsed_cs_str_vec_t& parsed_charstrings,
      ধ্রুৱ parsed_cs_str_vec_t& parsed_global_subrs,
      ধ্রুৱ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (পরিমাণ(cff_subset_accelerator_t));
    যদি (unlikely (!accel)) ঘূৰাই nullptr;
    নূতন (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ঘূৰাই accel;
  }

  স্থির শূন্য destroy (শূন্য* value) {
    যদি (!value) ঘূৰাই;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ধ্রুৱ parsed_cs_str_vec_t& parsed_charstrings_,
      ধ্রুৱ parsed_cs_str_vec_t& parsed_global_subrs_,
      ধ্রুৱ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    যদি (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 গুপ্ত:
  hb_blob_t* original_blob;
};

সংরচনা subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       তার্কিক drop_hints_,
		       তার্কিক coalesce_ = মিছা) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    যদি (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    ৱিকল্প (context.type)
    {
      পক্ষ CSType_CharString:
	ঘূৰাই parsed_charstring;

      পক্ষ CSType_LocalSubr:
	যদি (likely (context.subr_অঙ্ক_2 < parsed_local_subrs->length))
	  ঘূৰাই &(*parsed_local_subrs)[context.subr_অঙ্ক_2];
	ভঙ্গ;

      পক্ষ CSType_GlobalSubr:
	যদি (likely (context.subr_অঙ্ক_2 < parsed_global_subrs->length))
	  ঘূৰাই &(*parsed_global_subrs)[context.subr_অঙ্ক_2];
	ভঙ্গ;
    }
    ঘূৰাই nullptr;
  }

  template <typename ENV>
  শূন্য set_current_str (ENV &env, তার্কিক calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    যদি (unlikely (!parsed_str))
    {
      env.set_error ();
      ঘূৰাই;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    যদি (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    অন্যথা
    {
      যদি (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        যদি (coalesce) parsed_str->enable_coalescing ();
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
  তার্কিক	  drop_hints;
  তার্কিক	  coalesce;
};

সংরচনা subr_remap_t : hb_inc_bimap_t
{
  শূন্য create (ধ্রুৱ hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    বাবে (auto old_অঙ্ক : *closure)
      add (old_অঙ্ক);

    যদি (get_population () < 1240)
      bias = 107;
    অন্যথা যদি (get_population () < 33900)
      bias = 1131;
    অন্যথা
      bias = 32768;
  }

  পূর্ণাঙ্ক biased_অঙ্ক (unsigned পূর্ণাঙ্ক old_অঙ্ক) ধ্রুৱ
  {
    hb_codepoint_t new_অঙ্ক = get (old_অঙ্ক);
    ঘূৰাই (পূর্ণাঙ্ক)new_অঙ্ক - bias;
  }

  রক্ষিত:
  পূর্ণাঙ্ক bias;
};

সংরচনা subr_remaps_t
{
  subr_remaps_t (unsigned পূর্ণাঙ্ক fd_গুনতি)
  {
    local_remaps.resize (fd_গুনতি);
  }

  তার্কিক in_error()
  {
    ঘূৰাই local_remaps.in_error ();
  }

  শূন্য create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
সংরচনা subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ধ্রুৱ hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_গুনতি),
        remaps(acc_.fd_গুনতি)
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
  তার্কিক subset (শূন্য)
  {
    unsigned fd_গুনতি_2 = acc.fd_গুনতি;
    ধ্রুৱ cff_subset_accelerator_t* cff_accelerator = nullptr;
    যদি (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_গুনতি_2 = cff_accelerator->parsed_local_subrs.length;
    }

    যদি (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->অঙ্ক_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } অন্যথা {
      parsed_charstrings.resize_exact (plan->অঙ্ক_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->গুনতি);

      যদি (unlikely (!parsed_local_subrs_storage.resize (fd_গুনতি_2))) ঘূৰাই মিছা;

      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < acc.fd_গুনতি; i++)
      {
        unsigned গুনতি = acc.privateDicts[i].localSubrs->গুনতি;
        parsed_local_subrs_storage[i].resize (গুনতি);
        যদি (unlikely (parsed_local_subrs_storage[i].in_error ())) ঘূৰাই মিছা;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    যদি (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      ঘূৰাই মিছা;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    তার্কিক coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    বাবে (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ধ্রুৱ hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned পূর্ণাঙ্ক fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
        ঘূৰাই মিছা;

      যদি (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        যদি (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        অন্যথা
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        অনুৱর্ত;
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

      যদি (unlikely (!interp.interpret (param)))
        ঘূৰাই মিছা;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      যদি ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	যদি (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  যদি (drop.vsindex_dropped)
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
      যদি (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    যদি ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      ঘূৰাই মিছা;

    remaps.create (closures);

    populate_subset_accelerator ();
    ঘূৰাই সঁচা;
  }

  তার্কিক encode_charstrings (str_buff_vec_t &buffArray, তার্কিক encode_prefix = সঁচা) ধ্রুৱ
  {
    unsigned অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
    যদি (unlikely (!buffArray.resize_exact (অঙ্ক_glyphs)))
      ঘূৰাই মিছা;
    hb_codepoint_t last = 0;
    বাবে (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      যদি (endchar_op != OpCode_Invalid)
        বাবে (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned ৱর্ণ *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned পূর্ণাঙ্ক  fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
	ঘূৰাই মিছা;
      যদি (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	ঘূৰাই মিছা;
    }
    যদি (endchar_op != OpCode_Invalid)
      বাবে (; last < অঙ্ক_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned ৱর্ণ *>(endchar_str), 1);
      }

    ঘূৰাই সঁচা;
  }

  তার্কিক encode_subrs (ধ্রুৱ parsed_cs_str_vec_t &subrs, ধ্রুৱ subr_remap_t& remap, unsigned পূর্ণাঙ্ক fd, str_buff_vec_t &buffArray) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক  গুনতি = remap.get_population ();

    যদি (unlikely (!buffArray.resize_exact (গুনতি)))
      ঘূৰাই মিছা;
    বাবে (unsigned পূর্ণাঙ্ক new_অঙ্ক = 0; new_অঙ্ক < গুনতি; new_অঙ্ক++)
    {
      hb_codepoint_t old_অঙ্ক = remap.backward (new_অঙ্ক);
      প্রতিজ্ঞা (old_অঙ্ক != CFF_UNDEF_CODE);

      যদি (unlikely (!encode_str (subrs[old_অঙ্ক], fd, buffArray[new_অঙ্ক])))
	ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
  }

  তার্কিক encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ঘূৰাই encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  তার্কিক encode_localsubrs (unsigned পূর্ণাঙ্ক fd, str_buff_vec_t &buffArray) ধ্রুৱ
  {
    ঘূৰাই encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  রক্ষিত:
  সংরচনা drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (মিছা),
	ends_in_hint (মিছা),
	all_dropped (মিছা),
	vsindex_dropped (মিছা) {}

    তার্কিক  seen_moveto;
    তার্কিক  ends_in_hint;
    তার্কিক  all_dropped;
    তার্কিক  vsindex_dropped;
  };

  তার্কিক drop_hints_in_subr (parsed_cs_str_t &str, unsigned পূর্ণাঙ্ক pos,
			   parsed_cs_str_vec_t &subrs, unsigned পূর্ণাঙ্ক subr_অঙ্ক_2,
			   ধ্রুৱ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = মিছা;
    তার্কিক has_hint = drop_hints_in_str (subrs[subr_অঙ্ক_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    যদি (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      যদি (!str.at_end (pos))
	drop.ends_in_hint = মিছা;
    }
    অন্যথা যদি (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    ঘূৰাই has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  তার্কিক drop_hints_in_str (parsed_cs_str_t &str, ধ্রুৱ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    তার্কিক  seen_hint = মিছা;

    unsigned গুনতি = str.values.length;
    auto *values = str.values.arrayZ;
    বাবে (unsigned পূর্ণাঙ্ক pos = 0; pos < গুনতি; pos++)
    {
      তার্কিক  has_hint = মিছা;
      ৱিকল্প (values[pos].op)
      {
	পক্ষ OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_অঙ্ক_2,
					param, drop);
	  ভঙ্গ;

	পক্ষ OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_অঙ্ক_2,
					param, drop);
	  ভঙ্গ;

	পক্ষ OpCode_rmoveto:
	পক্ষ OpCode_hmoveto:
	পক্ষ OpCode_vmoveto:
	  drop.seen_moveto = সঁচা;
	  ভঙ্গ;

	পক্ষ OpCode_hintmask:
	পক্ষ OpCode_cntrmask:
	  যদি (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    ভঙ্গ;
	  }
	  HB_FALLTHROUGH;

	পক্ষ OpCode_hstemhm:
	পক্ষ OpCode_vstemhm:
	পক্ষ OpCode_hstem:
	পক্ষ OpCode_vstem:
	  has_hint = সঁচা;
	  values[pos].set_hinting ();
	  যদি (str.at_end (pos))
	    drop.ends_in_hint = সঁচা;
	  ভঙ্গ;

	পক্ষ OpCode_dotsection:
	  values[pos].set_hinting ();
	  ভঙ্গ;

	স্ৱভাৱ:
	  /* NONE */
	  ভঙ্গ;
      }
      যদি (has_hint)
      {
	বাবে (পূর্ণাঙ্ক i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  যদি (csop.is_hinting ())
	    ভঙ্গ;
	  csop.set_hinting ();
	  যদি (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = সঁচা;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = সঁচা;
    বাবে (unsigned পূর্ণাঙ্ক pos = 0; pos < গুনতি; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      যদি (csop.op == OpCode_return)
	ভঙ্গ;
      যদি (!csop.is_hinting ())
      {
	drop.all_dropped = মিছা;
	ভঙ্গ;
      }
    }

    ঘূৰাই seen_hint;
  }

  তার্কিক closure_subroutines (ধ্রুৱ parsed_cs_str_vec_t& global_subrs,
                            ধ্রুৱ hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    বাবে (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned পূর্ণাঙ্ক fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
        ঘূৰাই মিছা;

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

    ঘূৰাই সঁচা;
  }

  শূন্য collect_subr_refs_in_subr (unsigned পূর্ণাঙ্ক subr_অঙ্ক_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ধ্রুৱ subr_subset_param_t &param)
  {
    যদি (closure->has (subr_অঙ্ক_2))
      ঘূৰাই;
    closure->add (subr_অঙ্ক_2);
    collect_subr_refs_in_str (subrs[subr_অঙ্ক_2], param);
  }

  শূন্য collect_subr_refs_in_str (ধ্রুৱ parsed_cs_str_t &str,
                                 ধ্রুৱ subr_subset_param_t &param)
  {
    যদি (!str.has_calls ())
      ঘূৰাই;

    বাবে (auto &opstr : str.values)
    {
      যদি (!param.drop_hints || !opstr.is_hinting ())
      {
	ৱিকল্প (opstr.op)
	{
	  পক্ষ OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_অঙ্ক_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    ভঙ্গ;

	  পক্ষ OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_অঙ্ক_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    ভঙ্গ;

	  স্ৱভাৱ: ভঙ্গ;
	}
      }
    }
  }

  তার্কিক encode_str (ধ্রুৱ parsed_cs_str_t &str, ধ্রুৱ unsigned পূর্ণাঙ্ক fd, str_buff_t &buff, তার্কিক encode_prefix = সঁচা) ধ্রুৱ
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    তার্কিক hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    যদি (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_অঙ্ক_cs (str.prefix_অঙ্ক_2 ());
      যদি (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    বাবে (auto &opstr : str.values)
    {
      size += opstr.length;
      যদি (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    যদি (!buff.alloc_exact (buff.length + size))
      ঘূৰাই মিছা;

    বাবে (auto &opstr : str.values)
    {
      যদি (hinting || !opstr.is_hinting ())
      {
	ৱিকল্প (opstr.op)
	{
	  পক্ষ OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_অঙ্ক (opstr.subr_অঙ্ক_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ভঙ্গ;

	  পক্ষ OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_অঙ্ক (opstr.subr_অঙ্ক_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ভঙ্গ;

	  স্ৱভাৱ:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ভঙ্গ;
	}
      }
    }
    ঘূৰাই !encoder.in_error ();
  }

  শূন্য compact_parsed_subrs () ধ্রুৱ
  {
    বাবে (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    বাবে (auto &vec : parsed_local_subrs_storage)
      বাবে (auto &cs : vec)
	cs.compact ();
  }

  শূন্য populate_subset_accelerator () ধ্রুৱ
  {
    যদি (!plan->inprogress_accelerator) ঘূৰাই;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ধ্রুৱ parsed_cs_str_t& get_parsed_charstring (unsigned i) ধ্রুৱ
  {
    যদি (cached_charstrings) ঘূৰাই *(cached_charstrings[i]);
    ঘূৰাই parsed_charstrings[i];
  }

  রক্ষিত:
  ধ্রুৱ ACC			&acc;
  ধ্রুৱ hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ধ্রুৱ parsed_cs_str_t*>     cached_charstrings;
  ধ্রুৱ parsed_cs_str_vec_t*              parsed_global_subrs;
  ধ্রুৱ hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  গুপ্ত:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  প্রকারনির্দেশ typename SUBRS::গুনতি_type subr_গুনতি_type;
};

} /* namespace CFF */

HB_INTERNAL তার্কিক
hb_plan_subset_cff_fdselect (ধ্রুৱ hb_subset_plan_t *plan,
			    unsigned পূর্ণাঙ্ক fd_গুনতি,
			    ধ্রুৱ CFF::FDSelect &src, /* IN */
			    unsigned পূর্ণাঙ্ক src_অঙ্ক_glyphs,
			    unsigned পূর্ণাঙ্ক &subset_fd_গুনতি /* OUT */,
			    unsigned পূর্ণাঙ্ক &subset_fdselect_size /* OUT */,
			    unsigned পূর্ণাঙ্ক &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL তার্কিক
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			  ধ্রুৱ CFF::FDSelect &src,
			  unsigned পূর্ণাঙ্ক fd_গুনতি_2,
			  unsigned পূর্ণাঙ্ক fdselect_format,
			  unsigned পূর্ণাঙ্ক size,
			  ধ্রুৱ hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
