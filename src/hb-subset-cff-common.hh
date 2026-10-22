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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-subset-plan.hh"
#অন্তর্ভাব "hb-cff-interp-cs-common.hh"

নামাকাশ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
স্থির অন্তঃরেখ unsigned
subset_get_fd (ধ্রুবক ACC &acc, hb_codepoint_t glyph)
{
  ফেরত likely (glyph < acc.অঙ্ক_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
কাঠামো str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  শূন্য reset () { buff.reset (); }

  শূন্য encode_byte (unsigned অক্ষর b)
  {
    যদি (likely ((signed) buff.দৈর্ঘ্য < buff.allocated))
      buff.arrayZ[buff.দৈর্ঘ্য++] = b;
    অন্যথা
      buff.push (b);
  }

  শূন্য encode_int (পূর্ণসংখ্যা v)
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
  শূন্য encode_অঙ্ক_cs (ধ্রুবক সংখ্যা_t& n)
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
  শূন্য encode_অঙ্ক_tp (ধ্রুবক সংখ্যা_t& n)
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
      দ্বিগুণ v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      অক্ষর buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ফাঁকা));
      snprintf (buf, আকার (buf), "%.8G", v);
      (শূন্য) hb_uselocale (((শূন্য) freelocale (clocale), oldlocale));

      অক্ষর *s = buf;
      size_t len;
      অক্ষর *comma = strchr (s, ',');
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
	অক্ষর *s2 = s + exponent - 1;
	যখন (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, আকার (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      অন্যথা
      {
	অক্ষর *dot = strchr (s, '.');
	অক্ষর *e = strchr (s, 'E');
	যদি (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  পূর্ণসংখ্যা exponent = atoi (e + 1);
	  পূর্ণসংখ্যা new_exponent = exponent - (e - (dot + 1));
	  যদি (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  অন্যথা
	    snprintf (e - 1, আকার (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      যদি ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	পূর্ণসংখ্যা sign = s[0] == '-';
	অক্ষর *s2 = s + sign + 1;
	যখন (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, আকার (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<অক্ষর> nibbles;
      যখন (*s)
      {
	অক্ষর c = s[0];
	s++;

	নির্বাচন (c)
	{
	  ক্ষেত্র 'E':
	  {
	    অক্ষর c2 = *s;
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
	    চালু;
	  }

	  ক্ষেত্র '.':
	    nibbles.push (0x0A); // .
	    চালু;

	  ক্ষেত্র '-':
	    nibbles.push (0x0E); // -
	    চালু;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      যদি (nibbles.দৈর্ঘ্য % 2)
	nibbles.push (0x0F);

      unsigned গুনতি = nibbles.দৈর্ঘ্য;
      জন্য (unsigned i = 0; i < গুনতি; i += 2)
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

  শূন্য copy_str (ধ্রুবক unsigned অক্ষর *str, unsigned দৈর্ঘ্য)
  {
    প্রতিজ্ঞা ((signed) (buff.দৈর্ঘ্য + দৈর্ঘ্য) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.দৈর্ঘ্য, str, দৈর্ঘ্য);
    buff.দৈর্ঘ্য += দৈর্ঘ্য;
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত buff.in_error (); }

  সুরক্ষিত:

  str_buff_t &buff;
};

কাঠামো cff_sub_table_info_t {
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
কাঠামো cff_শীর্ষ_dict_op_serializer_t : op_serializer_t
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক OPSTR &opstr,
		  ধ্রুবক cff_sub_table_info_t &info) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    নির্বাচন (opstr.op)
    {
      ক্ষেত্র OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      ক্ষেত্র OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      ক্ষেত্র OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      পূর্বনির্ধারিত:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (সত্য);
  }
};

কাঠামো cff_font_dict_op_serializer_t : op_serializer_t
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক op_str_t &opstr,
		  ধ্রুবক table_info_t &privateDictInfo) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    যদি (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.আকার_2) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    অন্যথা
    {
      unsigned অক্ষর *d = c->allocate_আকার<unsigned অক্ষর> (opstr.দৈর্ঘ্য);
      যদি (unlikely (!d)) return_trace (মিথ্যা);
      /* Faster than hb_memcpy for small strings. */
      জন্য (unsigned i = 0; i < opstr.দৈর্ঘ্য; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (সত্য);
  }
};

/* CharString command for specialization */
কাঠামো cs_command_t
{
  hb_vector_t<সংখ্যা_t> args;
  hb_vector_t<unsigned অক্ষর> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

প্রকারলেখ hb_vector_t<cs_command_t> *cs_command_vec_t;

কাঠামো cff2_instancing_plan_t;

কাঠামো flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   যৌক্তিক drop_hints_,
                   ধ্রুবক hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  যৌক্তিক	drop_hints;
  ধ্রুবক hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ধ্রুবক cff2_instancing_plan_t *instancer = nullptr;
  যৌক্তিক emitted_blend = মিথ্যা;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
কাঠামো subr_flattener_t
{
  subr_flattener_t (ধ্রুবক ACC &acc_,
		    ধ্রুবক hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  যৌক্তিক flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned গুনতি = plan->অঙ্ক_output_glyphs ();
    যদি (!flat_charstrings.resize_exact (গুনতি))
      ফেরত মিথ্যা;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      hb_codepoint_t  glyph;
      যদি (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	যদি (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	চালু;
      }
      ধ্রুবক hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned পূর্ণসংখ্যা fd = subset_get_fd (acc, glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
	ফেরত মিথ্যা;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.দৈর্ঘ্য);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (যৌক্তিক) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      যদি (unlikely (!interp.interpret (param)))
	ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  ধ্রুবক ACC &acc;
  ধ্রুবক hb_subset_plan_t *plan;
};

কাঠামো subr_closures_t
{
  subr_closures_t (unsigned পূর্ণসংখ্যা fd_গুনতি_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_গুনতি_2);
  }

  শূন্য reset ()
  {
    global_closure.clear();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < local_closures.দৈর্ঘ্য; i++)
      local_closures[i].clear();
  }

  যৌক্তিক in_error () ধ্রুবক { ফেরত local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

কাঠামো parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned পূর্ণসংখ্যা subr_অঙ্ক = 0) :
    subr_অঙ্ক_2 (subr_অঙ্ক) {}

  যৌক্তিক is_hinting () ধ্রুবক { ফেরত hinting_flag; }
  শূন্য set_hinting ()       { hinting_flag = সত্য; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  সুরক্ষিত:
  যৌক্তিক	  hinting_flag = মিথ্যা;

  সর্বজনীন:
  uint16_t subr_অঙ্ক_2;
};

কাঠামো parsed_cs_str_t : parsed_মানসমূহ_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (মিথ্যা),
    hint_dropped (মিথ্যা),
    has_prefix_ (মিথ্যা),
    has_calls_ (মিথ্যা),
    coalescing_ (মিথ্যা)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  শূন্য add_op (op_code_t op, ধ্রুবক byte_str_ref_t& str_ref)
  {
    যদি (is_parsed ()) ফেরত;
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
      ফেরত;
    }
    SUPER::add_op (op, str_ref);
  }

  শূন্য add_call_op (op_code_t op, ধ্রুবক byte_str_ref_t& str_ref, unsigned পূর্ণসংখ্যা subr_অঙ্ক_2)
  {
    যদি (is_parsed ()) ফেরত;
    has_calls_ = সত্য;

    যদি (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_অঙ্ক_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      ফেরত;
    }

    /* Pop the subroutine number. */
    values.বের_করো ();

    SUPER::add_op (op, str_ref, {subr_অঙ্ক_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  শূন্য flush_segment (ধ্রুবক byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    যদি (end <= start) ফেরত;
    যখন (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      যদি (unlikely (!arr.দৈর্ঘ্য)) বিরতি;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->দৈর্ঘ্য = arr.দৈর্ঘ্য;
      start += arr.দৈর্ঘ্য;
    }
    opStart = end;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  শূন্য flush_coalesced (ধ্রুবক byte_str_ref_t& str_ref)
  {
    যদি (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  শূন্য enable_coalescing () { coalescing_ = সত্য; }
  যৌক্তিক is_coalescing () ধ্রুবক { ফেরত coalescing_; }

  শূন্য set_prefix (ধ্রুবক সংখ্যা_t &অঙ্ক, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = সত্য;
    prefix_op_ = op;
    prefix_অঙ্ক = অঙ্ক;
  }

  যৌক্তিক at_end (unsigned পূর্ণসংখ্যা pos) ধ্রুবক
  {
    ফেরত ((pos + 1 >= values.দৈর্ঘ্য) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  যৌক্তিক is_parsed () ধ্রুবক { ফেরত parsed; }
  শূন্য set_parsed ()      { parsed = সত্য; }

  যৌক্তিক is_hint_dropped () ধ্রুবক { ফেরত hint_dropped; }
  শূন্য set_hint_dropped ()      { hint_dropped = সত্য; }

  যৌক্তিক is_vsindex_dropped () ধ্রুবক { ফেরত vsindex_dropped; }
  শূন্য set_vsindex_dropped ()      { vsindex_dropped = সত্য; }

  যৌক্তিক has_prefix () ধ্রুবক          { ফেরত has_prefix_; }
  op_code_t prefix_op () ধ্রুবক         { ফেরত prefix_op_; }
  ধ্রুবক সংখ্যা_t &prefix_অঙ্ক_2 () ধ্রুবক { ফেরত prefix_অঙ্ক; }

  যৌক্তিক has_calls () ধ্রুবক          { ফেরত has_calls_; }

  শূন্য compact ()
  {
    unsigned গুনতি = values.দৈর্ঘ্য;
    যদি (!গুনতি) ফেরত;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    জন্য (unsigned i = 1; i < গুনতি; i++)
    {
      /* See if we can combine op j and op i. */
      যৌক্তিক combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].দৈর্ঘ্য == opstr[i].ptr) &&
        (opstr[j].দৈর্ঘ্য + opstr[i].দৈর্ঘ্য <= 255);

      যদি (combine)
      {
	opstr[j].দৈর্ঘ্য += opstr[i].দৈর্ঘ্য;
	opstr[j].op = OpCode_Invalid;
      }
      অন্যথা
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  সুরক্ষিত:
  যৌক্তিক    parsed : 1;
  যৌক্তিক    hint_dropped : 1;
  যৌক্তিক    vsindex_dropped : 1;
  যৌক্তিক    has_prefix_ : 1;
  যৌক্তিক    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  যৌক্তিক    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  সংখ্যা_t	prefix_অঙ্ক;

  ব্যক্তিগত:
  প্রকারলেখ parsed_মানসমূহ_t<parsed_cs_op_t> SUPER;
};

কাঠামো parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  ব্যক্তিগত:
  প্রকারলেখ hb_vector_t<parsed_cs_str_t> SUPER;
};

কাঠামো cff_subset_accelerator_t
{
  স্থির cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ধ্রুবক parsed_cs_str_vec_t& parsed_charstrings,
      ধ্রুবক parsed_cs_str_vec_t& parsed_global_subrs,
      ধ্রুবক hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (আকার(cff_subset_accelerator_t));
    যদি (unlikely (!accel)) ফেরত nullptr;
    নতুন (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ফেরত accel;
  }

  স্থির শূন্য destroy (শূন্য* value) {
    যদি (!value) ফেরত;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ধ্রুবক parsed_cs_str_vec_t& parsed_charstrings_,
      ধ্রুবক parsed_cs_str_vec_t& parsed_global_subrs_,
      ধ্রুবক hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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

 ব্যক্তিগত:
  hb_blob_t* original_blob;
};

কাঠামো subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       যৌক্তিক drop_hints_,
		       যৌক্তিক coalesce_ = মিথ্যা) :
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
    নির্বাচন (context.type)
    {
      ক্ষেত্র CSType_CharString:
	ফেরত parsed_charstring;

      ক্ষেত্র CSType_LocalSubr:
	যদি (likely (context.subr_অঙ্ক_2 < parsed_local_subrs->দৈর্ঘ্য))
	  ফেরত &(*parsed_local_subrs)[context.subr_অঙ্ক_2];
	বিরতি;

      ক্ষেত্র CSType_GlobalSubr:
	যদি (likely (context.subr_অঙ্ক_2 < parsed_global_subrs->দৈর্ঘ্য))
	  ফেরত &(*parsed_global_subrs)[context.subr_অঙ্ক_2];
	বিরতি;
    }
    ফেরত nullptr;
  }

  template <typename ENV>
  শূন্য set_current_str (ENV &env, যৌক্তিক calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    যদি (unlikely (!parsed_str))
    {
      env.set_error ();
      ফেরত;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    যদি (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.দৈর্ঘ্য > 0)))
      env.set_error ();
    অন্যথা
    {
      যদি (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.যোগ_আকার ());
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
  যৌক্তিক	  drop_hints;
  যৌক্তিক	  coalesce;
};

কাঠামো subr_remap_t : hb_inc_bimap_t
{
  শূন্য create (ধ্রুবক hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    জন্য (auto old_অঙ্ক : *closure)
      add (old_অঙ্ক);

    যদি (get_population () < 1240)
      bias = 107;
    অন্যথা যদি (get_population () < 33900)
      bias = 1131;
    অন্যথা
      bias = 32768;
  }

  পূর্ণসংখ্যা biased_অঙ্ক (unsigned পূর্ণসংখ্যা old_অঙ্ক) ধ্রুবক
  {
    hb_codepoint_t new_অঙ্ক = get (old_অঙ্ক);
    ফেরত (পূর্ণসংখ্যা)new_অঙ্ক - bias;
  }

  সুরক্ষিত:
  পূর্ণসংখ্যা bias;
};

কাঠামো subr_remaps_t
{
  subr_remaps_t (unsigned পূর্ণসংখ্যা fd_গুনতি)
  {
    local_remaps.resize (fd_গুনতি);
  }

  যৌক্তিক in_error()
  {
    ফেরত local_remaps.in_error ();
  }

  শূন্য create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < local_remaps.দৈর্ঘ্য; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
কাঠামো subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ধ্রুবক hb_subset_plan_t *plan_)
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
  যৌক্তিক subset (শূন্য)
  {
    unsigned fd_গুনতি_2 = acc.fd_গুনতি;
    ধ্রুবক cff_subset_accelerator_t* cff_accelerator = nullptr;
    যদি (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_গুনতি_2 = cff_accelerator->parsed_local_subrs.দৈর্ঘ্য;
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

      যদি (unlikely (!parsed_local_subrs_storage.resize (fd_গুনতি_2))) ফেরত মিথ্যা;

      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < acc.fd_গুনতি; i++)
      {
        unsigned গুনতি = acc.privateDicts[i].localSubrs->গুনতি;
        parsed_local_subrs_storage[i].resize (গুনতি);
        যদি (unlikely (parsed_local_subrs_storage[i].in_error ())) ফেরত মিথ্যা;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    যদি (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      ফেরত মিথ্যা;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    যৌক্তিক coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    জন্য (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ধ্রুবক hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned পূর্ণসংখ্যা fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
        ফেরত মিথ্যা;

      যদি (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        যদি (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        অন্যথা
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        চালু;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.দৈর্ঘ্য);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_closure,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      যদি (unlikely (!interp.interpret (param)))
        ফেরত মিথ্যা;

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
      ফেরত মিথ্যা;

    remaps.create (closures);

    populate_subset_accelerator ();
    ফেরত সত্য;
  }

  যৌক্তিক encode_charstrings (str_buff_vec_t &buffArray, যৌক্তিক encode_prefix = সত্য) ধ্রুবক
  {
    unsigned অঙ্ক_glyphs = plan->অঙ্ক_output_glyphs ();
    যদি (unlikely (!buffArray.resize_exact (অঙ্ক_glyphs)))
      ফেরত মিথ্যা;
    hb_codepoint_t last = 0;
    জন্য (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      যদি (endchar_op != OpCode_Invalid)
        জন্য (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned অক্ষর *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned পূর্ণসংখ্যা  fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
	ফেরত মিথ্যা;
      যদি (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	ফেরত মিথ্যা;
    }
    যদি (endchar_op != OpCode_Invalid)
      জন্য (; last < অঙ্ক_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned অক্ষর *>(endchar_str), 1);
      }

    ফেরত সত্য;
  }

  যৌক্তিক encode_subrs (ধ্রুবক parsed_cs_str_vec_t &subrs, ধ্রুবক subr_remap_t& remap, unsigned পূর্ণসংখ্যা fd, str_buff_vec_t &buffArray) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা  গুনতি = remap.get_population ();

    যদি (unlikely (!buffArray.resize_exact (গুনতি)))
      ফেরত মিথ্যা;
    জন্য (unsigned পূর্ণসংখ্যা new_অঙ্ক = 0; new_অঙ্ক < গুনতি; new_অঙ্ক++)
    {
      hb_codepoint_t old_অঙ্ক = remap.backward (new_অঙ্ক);
      প্রতিজ্ঞা (old_অঙ্ক != CFF_UNDEF_CODE);

      যদি (unlikely (!encode_str (subrs[old_অঙ্ক], fd, buffArray[new_অঙ্ক])))
	ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  যৌক্তিক encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ফেরত encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  যৌক্তিক encode_localsubrs (unsigned পূর্ণসংখ্যা fd, str_buff_vec_t &buffArray) ধ্রুবক
  {
    ফেরত encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  সুরক্ষিত:
  কাঠামো drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (মিথ্যা),
	ends_in_hint (মিথ্যা),
	all_dropped (মিথ্যা),
	vsindex_dropped (মিথ্যা) {}

    যৌক্তিক  seen_moveto;
    যৌক্তিক  ends_in_hint;
    যৌক্তিক  all_dropped;
    যৌক্তিক  vsindex_dropped;
  };

  যৌক্তিক drop_hints_in_subr (parsed_cs_str_t &str, unsigned পূর্ণসংখ্যা pos,
			   parsed_cs_str_vec_t &subrs, unsigned পূর্ণসংখ্যা subr_অঙ্ক_2,
			   ধ্রুবক subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = মিথ্যা;
    যৌক্তিক has_hint = drop_hints_in_str (subrs[subr_অঙ্ক_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    যদি (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      যদি (!str.at_end (pos))
	drop.ends_in_hint = মিথ্যা;
    }
    অন্যথা যদি (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    ফেরত has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  যৌক্তিক drop_hints_in_str (parsed_cs_str_t &str, ধ্রুবক subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    যৌক্তিক  seen_hint = মিথ্যা;

    unsigned গুনতি = str.values.দৈর্ঘ্য;
    auto *values = str.values.arrayZ;
    জন্য (unsigned পূর্ণসংখ্যা pos = 0; pos < গুনতি; pos++)
    {
      যৌক্তিক  has_hint = মিথ্যা;
      নির্বাচন (values[pos].op)
      {
	ক্ষেত্র OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_অঙ্ক_2,
					param, drop);
	  বিরতি;

	ক্ষেত্র OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_অঙ্ক_2,
					param, drop);
	  বিরতি;

	ক্ষেত্র OpCode_rmoveto:
	ক্ষেত্র OpCode_hmoveto:
	ক্ষেত্র OpCode_vmoveto:
	  drop.seen_moveto = সত্য;
	  বিরতি;

	ক্ষেত্র OpCode_hintmask:
	ক্ষেত্র OpCode_cntrmask:
	  যদি (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    বিরতি;
	  }
	  HB_FALLTHROUGH;

	ক্ষেত্র OpCode_hstemhm:
	ক্ষেত্র OpCode_vstemhm:
	ক্ষেত্র OpCode_hstem:
	ক্ষেত্র OpCode_vstem:
	  has_hint = সত্য;
	  values[pos].set_hinting ();
	  যদি (str.at_end (pos))
	    drop.ends_in_hint = সত্য;
	  বিরতি;

	ক্ষেত্র OpCode_dotsection:
	  values[pos].set_hinting ();
	  বিরতি;

	পূর্বনির্ধারিত:
	  /* NONE */
	  বিরতি;
      }
      যদি (has_hint)
      {
	জন্য (পূর্ণসংখ্যা i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  যদি (csop.is_hinting ())
	    বিরতি;
	  csop.set_hinting ();
	  যদি (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = সত্য;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = সত্য;
    জন্য (unsigned পূর্ণসংখ্যা pos = 0; pos < গুনতি; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      যদি (csop.op == OpCode_return)
	বিরতি;
      যদি (!csop.is_hinting ())
      {
	drop.all_dropped = মিথ্যা;
	বিরতি;
      }
    }

    ফেরত seen_hint;
  }

  যৌক্তিক closure_subroutines (ধ্রুবক parsed_cs_str_vec_t& global_subrs,
                            ধ্রুবক hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    জন্য (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned পূর্ণসংখ্যা fd = subset_get_fd (acc, old_glyph);
      যদি (unlikely (fd >= acc.fd_গুনতি))
        ফেরত মিথ্যা;

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

    ফেরত সত্য;
  }

  শূন্য collect_subr_refs_in_subr (unsigned পূর্ণসংখ্যা subr_অঙ্ক_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ধ্রুবক subr_subset_param_t &param)
  {
    যদি (closure->has (subr_অঙ্ক_2))
      ফেরত;
    closure->add (subr_অঙ্ক_2);
    collect_subr_refs_in_str (subrs[subr_অঙ্ক_2], param);
  }

  শূন্য collect_subr_refs_in_str (ধ্রুবক parsed_cs_str_t &str,
                                 ধ্রুবক subr_subset_param_t &param)
  {
    যদি (!str.has_calls ())
      ফেরত;

    জন্য (auto &opstr : str.values)
    {
      যদি (!param.drop_hints || !opstr.is_hinting ())
      {
	নির্বাচন (opstr.op)
	{
	  ক্ষেত্র OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_অঙ্ক_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    বিরতি;

	  ক্ষেত্র OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_অঙ্ক_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    বিরতি;

	  পূর্বনির্ধারিত: বিরতি;
	}
      }
    }
  }

  যৌক্তিক encode_str (ধ্রুবক parsed_cs_str_t &str, ধ্রুবক unsigned পূর্ণসংখ্যা fd, str_buff_t &buff, যৌক্তিক encode_prefix = সত্য) ধ্রুবক
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    যৌক্তিক hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    যদি (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_অঙ্ক_cs (str.prefix_অঙ্ক_2 ());
      যদি (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned আকার_2 = 0;
    জন্য (auto &opstr : str.values)
    {
      আকার_2 += opstr.দৈর্ঘ্য;
      যদি (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        আকার_2 += 3;
    }
    যদি (!buff.alloc_exact (buff.দৈর্ঘ্য + আকার_2))
      ফেরত মিথ্যা;

    জন্য (auto &opstr : str.values)
    {
      যদি (hinting || !opstr.is_hinting ())
      {
	নির্বাচন (opstr.op)
	{
	  ক্ষেত্র OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_অঙ্ক (opstr.subr_অঙ্ক_2));
	    encoder.copy_str (opstr.ptr, opstr.দৈর্ঘ্য);
	    বিরতি;

	  ক্ষেত্র OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_অঙ্ক (opstr.subr_অঙ্ক_2));
	    encoder.copy_str (opstr.ptr, opstr.দৈর্ঘ্য);
	    বিরতি;

	  পূর্বনির্ধারিত:
	    encoder.copy_str (opstr.ptr, opstr.দৈর্ঘ্য);
	    বিরতি;
	}
      }
    }
    ফেরত !encoder.in_error ();
  }

  শূন্য compact_parsed_subrs () ধ্রুবক
  {
    জন্য (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    জন্য (auto &vec : parsed_local_subrs_storage)
      জন্য (auto &cs : vec)
	cs.compact ();
  }

  শূন্য populate_subset_accelerator () ধ্রুবক
  {
    যদি (!plan->inprogress_accelerator) ফেরত;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ধ্রুবক parsed_cs_str_t& get_parsed_charstring (unsigned i) ধ্রুবক
  {
    যদি (cached_charstrings) ফেরত *(cached_charstrings[i]);
    ফেরত parsed_charstrings[i];
  }

  সুরক্ষিত:
  ধ্রুবক ACC			&acc;
  ধ্রুবক hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ধ্রুবক parsed_cs_str_t*>     cached_charstrings;
  ধ্রুবক parsed_cs_str_vec_t*              parsed_global_subrs;
  ধ্রুবক hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  ব্যক্তিগত:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  প্রকারলেখ typename SUBRS::গুনতি_type subr_গুনতি_type;
};

} /* namespace CFF */

HB_INTERNAL যৌক্তিক
hb_plan_subset_cff_fdselect (ধ্রুবক hb_subset_plan_t *plan,
			    unsigned পূর্ণসংখ্যা fd_গুনতি,
			    ধ্রুবক CFF::FDSelect &src, /* IN */
			    unsigned পূর্ণসংখ্যা src_অঙ্ক_glyphs,
			    unsigned পূর্ণসংখ্যা &subset_fd_গুনতি /* OUT */,
			    unsigned পূর্ণসংখ্যা &subset_fdselect_আকার /* OUT */,
			    unsigned পূর্ণসংখ্যা &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL যৌক্তিক
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			  ধ্রুবক CFF::FDSelect &src,
			  unsigned পূর্ণসংখ্যা fd_গুনতি_2,
			  unsigned পূর্ণসংখ্যা fdselect_format,
			  unsigned পূর্ণসংখ্যা আকার_2,
			  ধ্রুবক hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
