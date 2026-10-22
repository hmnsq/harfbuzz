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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset-plan.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cff-interp-cs-common.hh"

ନାମାକାଶ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned
subset_get_fd (ଅଚଳ ACC &acc, hb_codepoint_t glyph)
{
  ଫେରନ୍ତୁ likely (glyph < acc.ଅଙ୍କ_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ସଂରଚନା str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  ଶୂନ୍ୟ reset () { buff.reset (); }

  ଶୂନ୍ୟ encode_byte (unsigned ଅକ୍ଷର b)
  {
    ଯଦି (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    ଅନ୍ୟଥା
      buff.push (b);
  }

  ଶୂନ୍ୟ encode_int (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
  {
    ଯଦି ((-1131 <= v) && (v <= 1131))
    {
      ଯଦି ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      ଅନ୍ୟଥା ଯଦି (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      ଅନ୍ୟଥା
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (unlikely (v < -32768))
	v = -32768;
      ଅନ୍ୟଥା ଯଦି (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  ଶୂନ୍ୟ encode_ଅଙ୍କ_cs (ଅଚଳ ସଂଖ୍ୟା_t& n)
  {
    ଯଦି (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    ଅନ୍ୟଥା
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
  ଶୂନ୍ୟ encode_ଅଙ୍କ_tp (ଅଚଳ ସଂଖ୍ୟା_t& n)
  {
    ଯଦି (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    ଅନ୍ୟଥା
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      ଦ୍ୱିଗୁଣ v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      ଅକ୍ଷର buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ଖାଲି));
      snprintf (buf, ଆକାର (buf), "%.8G", v);
      (ଶୂନ୍ୟ) hb_uselocale (((ଶୂନ୍ୟ) freelocale (clocale), oldlocale));

      ଅକ୍ଷର *s = buf;
      size_t len;
      ଅକ୍ଷର *comma = strchr (s, ',');
      ଯଦି (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      ଯଦି (s[0] == '0' && s[1] == '.')
	s++;
      ଅନ୍ୟଥା ଯଦି (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      ଅନ୍ୟଥା ଯଦି ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	ଅକ୍ଷର *s2 = s + exponent - 1;
	ଯେତେବେଳେ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, ଆକାର (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      ଅନ୍ୟଥା
      {
	ଅକ୍ଷର *dot = strchr (s, '.');
	ଅକ୍ଷର *e = strchr (s, 'E');
	ଯଦି (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exponent = atoi (e + 1);
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_exponent = exponent - (e - (dot + 1));
	  ଯଦି (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  ଅନ୍ୟଥା
	    snprintf (e - 1, ଆକାର (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      ଯଦି ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sign = s[0] == '-';
	ଅକ୍ଷର *s2 = s + sign + 1;
	ଯେତେବେଳେ (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, ଆକାର (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<ଅକ୍ଷର> nibbles;
      ଯେତେବେଳେ (*s)
      {
	ଅକ୍ଷର c = s[0];
	s++;

	ବାଛ (c)
	{
	  ସ୍ଥିତି 'E':
	  {
	    ଅକ୍ଷର c2 = *s;
	    ଯଦି (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } ଅନ୍ୟଥା {
	      ଯଦି (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    ଯଦି (*s == '0')
	      s++;
	    ଜାରି;
	  }

	  ସ୍ଥିତି '.':
	    nibbles.push (0x0A); // .
	    ଜାରି;

	  ସ୍ଥିତି '-':
	    nibbles.push (0x0E); // -
	    ଜାରି;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      ଯଦି (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned ଗଣନା_2 = nibbles.length;
      ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  ଶୂନ୍ୟ encode_op (op_code_t op)
  {
    ଯଦି (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    ଅନ୍ୟଥା
      encode_byte (op);
  }

  ଶୂନ୍ୟ copy_str (ଅଚଳ unsigned ଅକ୍ଷର *str, unsigned length)
  {
    ପ୍ରତିଜ୍ଞା ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ buff.in_error (); }

  ସୁରକ୍ଷିତ:

  str_buff_t &buff;
};

ସଂରଚନା cff_sub_table_info_t {
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
ସଂରଚନା cff_ଶୀର୍ଷ_dict_op_serializer_t : op_serializer_t
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ OPSTR &opstr,
		  ଅଚଳ cff_sub_table_info_t &info) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ବାଛ (opstr.op)
    {
      ସ୍ଥିତି OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      ସ୍ଥିତି OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      ସ୍ଥିତି OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (ସତ୍ୟ);
  }
};

ସଂରଚନା cff_font_dict_op_serializer_t : op_serializer_t
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
		  ଅଚଳ op_str_t &opstr,
		  ଅଚଳ table_info_t &privateDictInfo) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    ଯଦି (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    ଅନ୍ୟଥା
    {
      unsigned ଅକ୍ଷର *d = c->allocate_size<unsigned ଅକ୍ଷର> (opstr.length);
      ଯଦି (unlikely (!d)) return_trace (ମିଥ୍ୟା);
      /* Faster than hb_memcpy for small strings. */
      ପାଇଁ (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (ସତ୍ୟ);
  }
};

/* CharString command for specialization */
ସଂରଚନା cs_command_t
{
  hb_vector_t<ସଂଖ୍ୟା_t> args;
  hb_vector_t<unsigned ଅକ୍ଷର> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_vector_t<cs_command_t> *cs_command_vec_t;

ସଂରଚନା cff2_instancing_plan_t;

ସଂରଚନା flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   ତାର୍କିକ drop_hints_,
                   ଅଚଳ hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  ତାର୍କିକ	drop_hints;
  ଅଚଳ hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ଅଚଳ cff2_instancing_plan_t *instancer = nullptr;
  ତାର୍କିକ emitted_blend = ମିଥ୍ୟା;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ସଂରଚନା subr_flattener_t
{
  subr_flattener_t (ଅଚଳ ACC &acc_,
		    ଅଚଳ hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  ତାର୍କିକ flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned ଗଣନା_2 = plan->ଅଙ୍କ_output_glyphs ();
    ଯଦି (!flat_charstrings.resize_exact (ଗଣନା_2))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      hb_codepoint_t  glyph;
      ଯଦି (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	ଯଦି (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	ଜାରି;
      }
      ଅଚଳ hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = subset_get_fd (acc, glyph);
      ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (ତାର୍କିକ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      ଯଦି (unlikely (!interp.interpret (param)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଅଚଳ ACC &acc;
  ଅଚଳ hb_subset_plan_t *plan;
};

ସଂରଚନା subr_closures_t
{
  subr_closures_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_ଗଣନା_2);
  }

  ଶୂନ୍ୟ reset ()
  {
    global_closure.clear();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

ସଂରଚନା parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ = 0) :
    subr_ଅଙ୍କ_2 (subr_ଅଙ୍କ) {}

  ତାର୍କିକ is_hinting () ଅଚଳ { ଫେରନ୍ତୁ hinting_flag; }
  ଶୂନ୍ୟ set_hinting ()       { hinting_flag = ସତ୍ୟ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  ସୁରକ୍ଷିତ:
  ତାର୍କିକ	  hinting_flag = ମିଥ୍ୟା;

  ସର୍ବସାଧାରଣ:
  uint16_t subr_ଅଙ୍କ_2;
};

ସଂରଚନା parsed_cs_str_t : parsed_ମୂଲ୍ୟଗୁଡ଼ିକ_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (ମିଥ୍ୟା),
    hint_dropped (ମିଥ୍ୟା),
    has_prefix_ (ମିଥ୍ୟା),
    has_calls_ (ମିଥ୍ୟା),
    coalescing_ (ମିଥ୍ୟା)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ add_op (op_code_t op, ଅଚଳ byte_str_ref_t& str_ref)
  {
    ଯଦି (is_parsed ()) ଫେରନ୍ତୁ;
    ଯଦି (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      ଯଦି (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      ଫେରନ୍ତୁ;
    }
    SUPER::add_op (op, str_ref);
  }

  ଶୂନ୍ୟ add_call_op (op_code_t op, ଅଚଳ byte_str_ref_t& str_ref, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ_2)
  {
    ଯଦି (is_parsed ()) ଫେରନ୍ତୁ;
    has_calls_ = ସତ୍ୟ;

    ଯଦି (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_ଅଙ୍କ_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      ଫେରନ୍ତୁ;
    }

    /* Pop the subroutine number. */
    values.କାଢ଼ ();

    SUPER::add_op (op, str_ref, {subr_ଅଙ୍କ_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  ଶୂନ୍ୟ flush_segment (ଅଚଳ byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    ଯଦି (end <= start) ଫେରନ୍ତୁ;
    ଯେତେବେଳେ (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      ଯଦି (unlikely (!arr.length)) ଭାଙ୍ଗ;
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
  ଶୂନ୍ୟ flush_coalesced (ଅଚଳ byte_str_ref_t& str_ref)
  {
    ଯଦି (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  ଶୂନ୍ୟ enable_coalescing () { coalescing_ = ସତ୍ୟ; }
  ତାର୍କିକ is_coalescing () ଅଚଳ { ଫେରନ୍ତୁ coalescing_; }

  ଶୂନ୍ୟ set_prefix (ଅଚଳ ସଂଖ୍ୟା_t &ଅଙ୍କ, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = ସତ୍ୟ;
    prefix_op_ = op;
    prefix_ଅଙ୍କ = ଅଙ୍କ;
  }

  ତାର୍କିକ at_end (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos) ଅଚଳ
  {
    ଫେରନ୍ତୁ ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  ତାର୍କିକ is_parsed () ଅଚଳ { ଫେରନ୍ତୁ parsed; }
  ଶୂନ୍ୟ set_parsed ()      { parsed = ସତ୍ୟ; }

  ତାର୍କିକ is_hint_dropped () ଅଚଳ { ଫେରନ୍ତୁ hint_dropped; }
  ଶୂନ୍ୟ set_hint_dropped ()      { hint_dropped = ସତ୍ୟ; }

  ତାର୍କିକ is_vsindex_dropped () ଅଚଳ { ଫେରନ୍ତୁ vsindex_dropped; }
  ଶୂନ୍ୟ set_vsindex_dropped ()      { vsindex_dropped = ସତ୍ୟ; }

  ତାର୍କିକ has_prefix () ଅଚଳ          { ଫେରନ୍ତୁ has_prefix_; }
  op_code_t prefix_op () ଅଚଳ         { ଫେରନ୍ତୁ prefix_op_; }
  ଅଚଳ ସଂଖ୍ୟା_t &prefix_ଅଙ୍କ_2 () ଅଚଳ { ଫେରନ୍ତୁ prefix_ଅଙ୍କ; }

  ତାର୍କିକ has_calls () ଅଚଳ          { ଫେରନ୍ତୁ has_calls_; }

  ଶୂନ୍ୟ compact ()
  {
    unsigned ଗଣନା_2 = values.length;
    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    ପାଇଁ (unsigned i = 1; i < ଗଣନା_2; i++)
    {
      /* See if we can combine op j and op i. */
      ତାର୍କିକ combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      ଯଦି (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      ଅନ୍ୟଥା
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  ସୁରକ୍ଷିତ:
  ତାର୍କିକ    parsed : 1;
  ତାର୍କିକ    hint_dropped : 1;
  ତାର୍କିକ    vsindex_dropped : 1;
  ତାର୍କିକ    has_prefix_ : 1;
  ତାର୍କିକ    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  ତାର୍କିକ    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  ସଂଖ୍ୟା_t	prefix_ଅଙ୍କ;

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା parsed_ମୂଲ୍ୟଗୁଡ଼ିକ_t<parsed_cs_op_t> SUPER;
};

ସଂରଚନା parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_vector_t<parsed_cs_str_t> SUPER;
};

ସଂରଚନା cff_subset_accelerator_t
{
  ସ୍ଥିର cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ଅଚଳ parsed_cs_str_vec_t& parsed_charstrings,
      ଅଚଳ parsed_cs_str_vec_t& parsed_global_subrs,
      ଅଚଳ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (ଆକାର(cff_subset_accelerator_t));
    ଯଦି (unlikely (!accel)) ଫେରନ୍ତୁ nullptr;
    ନୂତନ (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ଫେରନ୍ତୁ accel;
  }

  ସ୍ଥିର ଶୂନ୍ୟ destroy (ଶୂନ୍ୟ* value) {
    ଯଦି (!value) ଫେରନ୍ତୁ;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ଅଚଳ parsed_cs_str_vec_t& parsed_charstrings_,
      ଅଚଳ parsed_cs_str_vec_t& parsed_global_subrs_,
      ଅଚଳ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    ଯଦି (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 ବ୍ୟକ୍ତିଗତ:
  hb_blob_t* original_blob;
};

ସଂରଚନା subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       ତାର୍କିକ drop_hints_,
		       ତାର୍କିକ coalesce_ = ମିଥ୍ୟା) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    ଯଦି (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    ବାଛ (context.type)
    {
      ସ୍ଥିତି CSType_CharString:
	ଫେରନ୍ତୁ parsed_charstring;

      ସ୍ଥିତି CSType_LocalSubr:
	ଯଦି (likely (context.subr_ଅଙ୍କ_2 < parsed_local_subrs->length))
	  ଫେରନ୍ତୁ &(*parsed_local_subrs)[context.subr_ଅଙ୍କ_2];
	ଭାଙ୍ଗ;

      ସ୍ଥିତି CSType_GlobalSubr:
	ଯଦି (likely (context.subr_ଅଙ୍କ_2 < parsed_global_subrs->length))
	  ଫେରନ୍ତୁ &(*parsed_global_subrs)[context.subr_ଅଙ୍କ_2];
	ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ nullptr;
  }

  template <typename ENV>
  ଶୂନ୍ୟ set_current_str (ENV &env, ତାର୍କିକ calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    ଯଦି (unlikely (!parsed_str))
    {
      env.set_error ();
      ଫେରନ୍ତୁ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    ଯଦି (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    ଅନ୍ୟଥା
    {
      ଯଦି (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        ଯଦି (coalesce) parsed_str->enable_coalescing ();
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
  ତାର୍କିକ	  drop_hints;
  ତାର୍କିକ	  coalesce;
};

ସଂରଚନା subr_remap_t : hb_inc_bimap_t
{
  ଶୂନ୍ୟ create (ଅଚଳ hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    ପାଇଁ (auto old_ଅଙ୍କ : *closure)
      add (old_ଅଙ୍କ);

    ଯଦି (get_population () < 1240)
      bias = 107;
    ଅନ୍ୟଥା ଯଦି (get_population () < 33900)
      bias = 1131;
    ଅନ୍ୟଥା
      bias = 32768;
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା biased_ଅଙ୍କ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_ଅଙ୍କ) ଅଚଳ
  {
    hb_codepoint_t new_ଅଙ୍କ = get (old_ଅଙ୍କ);
    ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)new_ଅଙ୍କ - bias;
  }

  ସୁରକ୍ଷିତ:
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bias;
};

ସଂରଚନା subr_remaps_t
{
  subr_remaps_t (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା)
  {
    local_remaps.resize (fd_ଗଣନା);
  }

  ତାର୍କିକ in_error()
  {
    ଫେରନ୍ତୁ local_remaps.in_error ();
  }

  ଶୂନ୍ୟ create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ସଂରଚନା subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ଅଚଳ hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_ଗଣନା),
        remaps(acc_.fd_ଗଣନା)
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
  ତାର୍କିକ subset (ଶୂନ୍ୟ)
  {
    unsigned fd_ଗଣନା_2 = acc.fd_ଗଣନା;
    ଅଚଳ cff_subset_accelerator_t* cff_accelerator = nullptr;
    ଯଦି (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_ଗଣନା_2 = cff_accelerator->parsed_local_subrs.length;
    }

    ଯଦି (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->ଅଙ୍କ_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } ଅନ୍ୟଥା {
      parsed_charstrings.resize_exact (plan->ଅଙ୍କ_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->ଗଣନା_2);

      ଯଦି (unlikely (!parsed_local_subrs_storage.resize (fd_ଗଣନା_2))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < acc.fd_ଗଣନା; i++)
      {
        unsigned ଗଣନା_2 = acc.privateDicts[i].localSubrs->ଗଣନା_2;
        parsed_local_subrs_storage[i].resize (ଗଣନା_2);
        ଯଦି (unlikely (parsed_local_subrs_storage[i].in_error ())) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    ଯଦି (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    ତାର୍କିକ coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    ପାଇଁ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ଅଚଳ hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = subset_get_fd (acc, old_glyph);
      ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        ଯଦି (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        ଅନ୍ୟଥା
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        ଜାରି;
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

      ଯଦି (unlikely (!interp.interpret (param)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      ଯଦି ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	ଯଦି (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  ଯଦି (drop.vsindex_dropped)
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
      ଯଦି (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    ଯଦି ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    remaps.create (closures);

    populate_subset_accelerator ();
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ encode_charstrings (str_buff_vec_t &buffArray, ତାର୍କିକ encode_prefix = ସତ୍ୟ) ଅଚଳ
  {
    unsigned ଅଙ୍କ_glyphs = plan->ଅଙ୍କ_output_glyphs ();
    ଯଦି (unlikely (!buffArray.resize_exact (ଅଙ୍କ_glyphs)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    hb_codepoint_t last = 0;
    ପାଇଁ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      ଯଦି (endchar_op != OpCode_Invalid)
        ପାଇଁ (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned ଅକ୍ଷର *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  fd = subset_get_fd (acc, old_glyph);
      ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      ଯଦି (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (endchar_op != OpCode_Invalid)
      ପାଇଁ (; last < ଅଙ୍କ_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned ଅକ୍ଷର *>(endchar_str), 1);
      }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ encode_subrs (ଅଚଳ parsed_cs_str_vec_t &subrs, ଅଚଳ subr_remap_t& remap, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, str_buff_vec_t &buffArray) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ଗଣନା_2 = remap.get_population ();

    ଯଦି (unlikely (!buffArray.resize_exact (ଗଣନା_2)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_ଅଙ୍କ = 0; new_ଅଙ୍କ < ଗଣନା_2; new_ଅଙ୍କ++)
    {
      hb_codepoint_t old_ଅଙ୍କ = remap.backward (new_ଅଙ୍କ);
      ପ୍ରତିଜ୍ଞା (old_ଅଙ୍କ != CFF_UNDEF_CODE);

      ଯଦି (unlikely (!encode_str (subrs[old_ଅଙ୍କ], fd, buffArray[new_ଅଙ୍କ])))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ଫେରନ୍ତୁ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  ତାର୍କିକ encode_localsubrs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, str_buff_vec_t &buffArray) ଅଚଳ
  {
    ଫେରନ୍ତୁ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  ସୁରକ୍ଷିତ:
  ସଂରଚନା drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (ମିଥ୍ୟା),
	ends_in_hint (ମିଥ୍ୟା),
	all_dropped (ମିଥ୍ୟା),
	vsindex_dropped (ମିଥ୍ୟା) {}

    ତାର୍କିକ  seen_moveto;
    ତାର୍କିକ  ends_in_hint;
    ତାର୍କିକ  all_dropped;
    ତାର୍କିକ  vsindex_dropped;
  };

  ତାର୍କିକ drop_hints_in_subr (parsed_cs_str_t &str, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos,
			   parsed_cs_str_vec_t &subrs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ_2,
			   ଅଚଳ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = ମିଥ୍ୟା;
    ତାର୍କିକ has_hint = drop_hints_in_str (subrs[subr_ଅଙ୍କ_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    ଯଦି (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      ଯଦି (!str.at_end (pos))
	drop.ends_in_hint = ମିଥ୍ୟା;
    }
    ଅନ୍ୟଥା ଯଦି (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    ଫେରନ୍ତୁ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  ତାର୍କିକ drop_hints_in_str (parsed_cs_str_t &str, ଅଚଳ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    ତାର୍କିକ  seen_hint = ମିଥ୍ୟା;

    unsigned ଗଣନା_2 = str.values.length;
    auto *values = str.values.arrayZ;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = 0; pos < ଗଣନା_2; pos++)
    {
      ତାର୍କିକ  has_hint = ମିଥ୍ୟା;
      ବାଛ (values[pos].op)
      {
	ସ୍ଥିତି OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_ଅଙ୍କ_2,
					param, drop);
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_ଅଙ୍କ_2,
					param, drop);
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି OpCode_rmoveto:
	ସ୍ଥିତି OpCode_hmoveto:
	ସ୍ଥିତି OpCode_vmoveto:
	  drop.seen_moveto = ସତ୍ୟ;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି OpCode_hintmask:
	ସ୍ଥିତି OpCode_cntrmask:
	  ଯଦି (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    ଭାଙ୍ଗ;
	  }
	  HB_FALLTHROUGH;

	ସ୍ଥିତି OpCode_hstemhm:
	ସ୍ଥିତି OpCode_vstemhm:
	ସ୍ଥିତି OpCode_hstem:
	ସ୍ଥିତି OpCode_vstem:
	  has_hint = ସତ୍ୟ;
	  values[pos].set_hinting ();
	  ଯଦି (str.at_end (pos))
	    drop.ends_in_hint = ସତ୍ୟ;
	  ଭାଙ୍ଗ;

	ସ୍ଥିତି OpCode_dotsection:
	  values[pos].set_hinting ();
	  ଭାଙ୍ଗ;

	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	  /* NONE */
	  ଭାଙ୍ଗ;
      }
      ଯଦି (has_hint)
      {
	ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  ଯଦି (csop.is_hinting ())
	    ଭାଙ୍ଗ;
	  csop.set_hinting ();
	  ଯଦି (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = ସତ୍ୟ;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = ସତ୍ୟ;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = 0; pos < ଗଣନା_2; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      ଯଦି (csop.op == OpCode_return)
	ଭାଙ୍ଗ;
      ଯଦି (!csop.is_hinting ())
      {
	drop.all_dropped = ମିଥ୍ୟା;
	ଭାଙ୍ଗ;
      }
    }

    ଫେରନ୍ତୁ seen_hint;
  }

  ତାର୍କିକ closure_subroutines (ଅଚଳ parsed_cs_str_vec_t& global_subrs,
                            ଅଚଳ hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    ପାଇଁ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = subset_get_fd (acc, old_glyph);
      ଯଦି (unlikely (fd >= acc.fd_ଗଣନା))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

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

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ collect_subr_refs_in_subr (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା subr_ଅଙ୍କ_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ଅଚଳ subr_subset_param_t &param)
  {
    ଯଦି (closure->has (subr_ଅଙ୍କ_2))
      ଫେରନ୍ତୁ;
    closure->add (subr_ଅଙ୍କ_2);
    collect_subr_refs_in_str (subrs[subr_ଅଙ୍କ_2], param);
  }

  ଶୂନ୍ୟ collect_subr_refs_in_str (ଅଚଳ parsed_cs_str_t &str,
                                 ଅଚଳ subr_subset_param_t &param)
  {
    ଯଦି (!str.has_calls ())
      ଫେରନ୍ତୁ;

    ପାଇଁ (auto &opstr : str.values)
    {
      ଯଦି (!param.drop_hints || !opstr.is_hinting ())
      {
	ବାଛ (opstr.op)
	{
	  ସ୍ଥିତି OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_ଅଙ୍କ_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    ଭାଙ୍ଗ;

	  ସ୍ଥିତି OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_ଅଙ୍କ_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    ଭାଙ୍ଗ;

	  ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଭାଙ୍ଗ;
	}
      }
    }
  }

  ତାର୍କିକ encode_str (ଅଚଳ parsed_cs_str_t &str, ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, str_buff_t &buff, ତାର୍କିକ encode_prefix = ସତ୍ୟ) ଅଚଳ
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    ତାର୍କିକ hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    ଯଦି (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_ଅଙ୍କ_cs (str.prefix_ଅଙ୍କ_2 ());
      ଯଦି (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    ପାଇଁ (auto &opstr : str.values)
    {
      size += opstr.length;
      ଯଦି (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    ଯଦି (!buff.alloc_exact (buff.length + size))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (auto &opstr : str.values)
    {
      ଯଦି (hinting || !opstr.is_hinting ())
      {
	ବାଛ (opstr.op)
	{
	  ସ୍ଥିତି OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_ଅଙ୍କ (opstr.subr_ଅଙ୍କ_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ଭାଙ୍ଗ;

	  ସ୍ଥିତି OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_ଅଙ୍କ (opstr.subr_ଅଙ୍କ_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ଭାଙ୍ଗ;

	  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ଭାଙ୍ଗ;
	}
      }
    }
    ଫେରନ୍ତୁ !encoder.in_error ();
  }

  ଶୂନ୍ୟ compact_parsed_subrs () ଅଚଳ
  {
    ପାଇଁ (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    ପାଇଁ (auto &vec : parsed_local_subrs_storage)
      ପାଇଁ (auto &cs : vec)
	cs.compact ();
  }

  ଶୂନ୍ୟ populate_subset_accelerator () ଅଚଳ
  {
    ଯଦି (!plan->inprogress_accelerator) ଫେରନ୍ତୁ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ଅଚଳ parsed_cs_str_t& get_parsed_charstring (unsigned i) ଅଚଳ
  {
    ଯଦି (cached_charstrings) ଫେରନ୍ତୁ *(cached_charstrings[i]);
    ଫେରନ୍ତୁ parsed_charstrings[i];
  }

  ସୁରକ୍ଷିତ:
  ଅଚଳ ACC			&acc;
  ଅଚଳ hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ଅଚଳ parsed_cs_str_t*>     cached_charstrings;
  ଅଚଳ parsed_cs_str_vec_t*              parsed_global_subrs;
  ଅଚଳ hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  ବ୍ୟକ୍ତିଗତ:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename SUBRS::ଗଣନା_type subr_ଗଣନା_type;
};

} /* namespace CFF */

HB_INTERNAL ତାର୍କିକ
hb_plan_subset_cff_fdselect (ଅଚଳ hb_subset_plan_t *plan,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା,
			    ଅଚଳ CFF::FDSelect &src, /* IN */
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_ଅଙ୍କ_glyphs,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fd_ଗଣନା /* OUT */,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fdselect_size /* OUT */,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL ତାର୍କିକ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			  ଅଚଳ CFF::FDSelect &src,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd_ଗଣନା_2,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdselect_format,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size,
			  ଅଚଳ hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
