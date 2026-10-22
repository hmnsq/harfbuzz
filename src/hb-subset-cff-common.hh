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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-subset-plan.hh"
#अन्तर्भाव "hb-cff-interp-cs-common.hh"

नामाकाश CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
स्थिर अन्तःस्थ unsigned
subset_get_fd (ध्रुव ACC &acc, hb_codepoint_t glyph)
{
  पटिनिवत्त likely (glyph < acc.अङ्कः_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
संरचना str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  शून्य reset () { buff.reset (); }

  शून्य encode_byte (unsigned वर्ण b)
  {
    सचे (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    अञ्ञथा
      buff.push (b);
  }

  शून्य encode_int (पूर्णाङ्क v)
  {
    सचे ((-1131 <= v) && (v <= 1131))
    {
      सचे ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      अञ्ञथा सचे (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      अञ्ञथा
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    अञ्ञथा
    {
      सचे (unlikely (v < -32768))
	v = -32768;
      अञ्ञथा सचे (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  शून्य encode_अङ्कः_cs (ध्रुव सङ्ख्या_t& n)
  {
    सचे (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    अञ्ञथा
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
  शून्य encode_अङ्कः_tp (ध्रुव सङ्ख्या_t& n)
  {
    सचे (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    अञ्ञथा
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      द्विगुण v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      वर्ण buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", रिक्त));
      snprintf (buf, परिमाण (buf), "%.8G", v);
      (शून्य) hb_uselocale (((शून्य) freelocale (clocale), oldlocale));

      वर्ण *s = buf;
      size_t len;
      वर्ण *comma = strchr (s, ',');
      सचे (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      सचे (s[0] == '0' && s[1] == '.')
	s++;
      अञ्ञथा सचे (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      अञ्ञथा सचे ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	वर्ण *s2 = s + exponent - 1;
	याव (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, परिमाण (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      अञ्ञथा
      {
	वर्ण *dot = strchr (s, '.');
	वर्ण *e = strchr (s, 'E');
	सचे (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  पूर्णाङ्क exponent = atoi (e + 1);
	  पूर्णाङ्क new_exponent = exponent - (e - (dot + 1));
	  सचे (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  अञ्ञथा
	    snprintf (e - 1, परिमाण (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      सचे ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	पूर्णाङ्क sign = s[0] == '-';
	वर्ण *s2 = s + sign + 1;
	याव (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, परिमाण (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<वर्ण> nibbles;
      याव (*s)
      {
	वर्ण c = s[0];
	s++;

	विकल्प (c)
	{
	  पक्ष 'E':
	  {
	    वर्ण c2 = *s;
	    सचे (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } अञ्ञथा {
	      सचे (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    सचे (*s == '0')
	      s++;
	    अनुवर्त;
	  }

	  पक्ष '.':
	    nibbles.push (0x0A); // .
	    अनुवर्त;

	  पक्ष '-':
	    nibbles.push (0x0E); // -
	    अनुवर्त;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      सचे (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned गणनम् = nibbles.length;
      अत्थाय (unsigned i = 0; i < गणनम्; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  शून्य encode_op (op_code_t op)
  {
    सचे (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    अञ्ञथा
      encode_byte (op);
  }

  शून्य copy_str (ध्रुव unsigned वर्ण *str, unsigned length)
  {
    प्रतिज्ञा ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  तार्किक in_error () ध्रुव { पटिनिवत्त buff.in_error (); }

  रक्षित:

  str_buff_t &buff;
};

संरचना cff_sub_table_info_t {
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
संरचना cff_शिखरम्_dict_op_serializer_t : op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव OPSTR &opstr,
		  ध्रुव cff_sub_table_info_t &info) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    विकल्प (opstr.op)
    {
      पक्ष OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      पक्ष OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      पक्ष OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      स्वभाव:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (सच्च);
  }
};

संरचना cff_font_dict_op_serializer_t : op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव op_str_t &opstr,
		  ध्रुव table_info_t &privateDictInfo) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    सचे (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    अञ्ञथा
    {
      unsigned वर्ण *d = c->allocate_size<unsigned वर्ण> (opstr.length);
      सचे (unlikely (!d)) return_trace (मिच्छा);
      /* Faster than hb_memcpy for small strings. */
      अत्थाय (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (सच्च);
  }
};

/* CharString command for specialization */
संरचना cs_command_t
{
  hb_vector_t<सङ्ख्या_t> args;
  hb_vector_t<unsigned वर्ण> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

प्रकारनिर्देश hb_vector_t<cs_command_t> *cs_command_vec_t;

संरचना cff2_instancing_plan_t;

संरचना flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   तार्किक drop_hints_,
                   ध्रुव hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  तार्किक	drop_hints;
  ध्रुव hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ध्रुव cff2_instancing_plan_t *instancer = nullptr;
  तार्किक emitted_blend = मिच्छा;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_flattener_t
{
  subr_flattener_t (ध्रुव ACC &acc_,
		    ध्रुव hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  तार्किक flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned गणनम् = plan->अङ्कः_output_glyphs ();
    सचे (!flat_charstrings.resize_exact (गणनम्))
      पटिनिवत्त मिच्छा;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      hb_codepoint_t  glyph;
      सचे (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	सचे (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	अनुवर्त;
      }
      ध्रुव hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned पूर्णाङ्क fd = subset_get_fd (acc, glyph);
      सचे (unlikely (fd >= acc.fd_गणनम्))
	पटिनिवत्त मिच्छा;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (तार्किक) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      सचे (unlikely (!interp.interpret (param)))
	पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }

  ध्रुव ACC &acc;
  ध्रुव hb_subset_plan_t *plan;
};

संरचना subr_closures_t
{
  subr_closures_t (unsigned पूर्णाङ्क fd_गणनम्_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_गणनम्_2);
  }

  शून्य reset ()
  {
    global_closure.clear();
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  तार्किक in_error () ध्रुव { पटिनिवत्त local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

संरचना parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned पूर्णाङ्क subr_अङ्कः = 0) :
    subr_अङ्कः_2 (subr_अङ्कः) {}

  तार्किक is_hinting () ध्रुव { पटिनिवत्त hinting_flag; }
  शून्य set_hinting ()       { hinting_flag = सच्च; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  रक्षित:
  तार्किक	  hinting_flag = मिच्छा;

  सार्वजनिक:
  uint16_t subr_अङ्कः_2;
};

संरचना parsed_cs_str_t : parsed_मानानि_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (मिच्छा),
    hint_dropped (मिच्छा),
    has_prefix_ (मिच्छा),
    has_calls_ (मिच्छा),
    coalescing_ (मिच्छा)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  शून्य add_op (op_code_t op, ध्रुव byte_str_ref_t& str_ref)
  {
    सचे (is_parsed ()) पटिनिवत्त;
    सचे (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      सचे (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      पटिनिवत्त;
    }
    SUPER::add_op (op, str_ref);
  }

  शून्य add_call_op (op_code_t op, ध्रुव byte_str_ref_t& str_ref, unsigned पूर्णाङ्क subr_अङ्कः_2)
  {
    सचे (is_parsed ()) पटिनिवत्त;
    has_calls_ = सच्च;

    सचे (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_अङ्कः_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      पटिनिवत्त;
    }

    /* Pop the subroutine number. */
    values.निष्कासय ();

    SUPER::add_op (op, str_ref, {subr_अङ्कः_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  शून्य flush_segment (ध्रुव byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    सचे (end <= start) पटिनिवत्त;
    याव (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      सचे (unlikely (!arr.length)) भङ्ग;
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
  शून्य flush_coalesced (ध्रुव byte_str_ref_t& str_ref)
  {
    सचे (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  शून्य enable_coalescing () { coalescing_ = सच्च; }
  तार्किक is_coalescing () ध्रुव { पटिनिवत्त coalescing_; }

  शून्य set_prefix (ध्रुव सङ्ख्या_t &अङ्कः, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = सच्च;
    prefix_op_ = op;
    prefix_अङ्कः = अङ्कः;
  }

  तार्किक at_end (unsigned पूर्णाङ्क pos) ध्रुव
  {
    पटिनिवत्त ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  तार्किक is_parsed () ध्रुव { पटिनिवत्त parsed; }
  शून्य set_parsed ()      { parsed = सच्च; }

  तार्किक is_hint_dropped () ध्रुव { पटिनिवत्त hint_dropped; }
  शून्य set_hint_dropped ()      { hint_dropped = सच्च; }

  तार्किक is_vsindex_dropped () ध्रुव { पटिनिवत्त vsindex_dropped; }
  शून्य set_vsindex_dropped ()      { vsindex_dropped = सच्च; }

  तार्किक has_prefix () ध्रुव          { पटिनिवत्त has_prefix_; }
  op_code_t prefix_op () ध्रुव         { पटिनिवत्त prefix_op_; }
  ध्रुव सङ्ख्या_t &prefix_अङ्कः_2 () ध्रुव { पटिनिवत्त prefix_अङ्कः; }

  तार्किक has_calls () ध्रुव          { पटिनिवत्त has_calls_; }

  शून्य compact ()
  {
    unsigned गणनम् = values.length;
    सचे (!गणनम्) पटिनिवत्त;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    अत्थाय (unsigned i = 1; i < गणनम्; i++)
    {
      /* See if we can combine op j and op i. */
      तार्किक combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      सचे (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      अञ्ञथा
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  रक्षित:
  तार्किक    parsed : 1;
  तार्किक    hint_dropped : 1;
  तार्किक    vsindex_dropped : 1;
  तार्किक    has_prefix_ : 1;
  तार्किक    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  तार्किक    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  सङ्ख्या_t	prefix_अङ्कः;

  गुप्त:
  प्रकारनिर्देश parsed_मानानि_t<parsed_cs_op_t> SUPER;
};

संरचना parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  गुप्त:
  प्रकारनिर्देश hb_vector_t<parsed_cs_str_t> SUPER;
};

संरचना cff_subset_accelerator_t
{
  स्थिर cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ध्रुव parsed_cs_str_vec_t& parsed_charstrings,
      ध्रुव parsed_cs_str_vec_t& parsed_global_subrs,
      ध्रुव hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (परिमाण(cff_subset_accelerator_t));
    सचे (unlikely (!accel)) पटिनिवत्त nullptr;
    नूतन (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    पटिनिवत्त accel;
  }

  स्थिर शून्य destroy (शून्य* value) {
    सचे (!value) पटिनिवत्त;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ध्रुव parsed_cs_str_vec_t& parsed_charstrings_,
      ध्रुव parsed_cs_str_vec_t& parsed_global_subrs_,
      ध्रुव hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    सचे (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 गुप्त:
  hb_blob_t* original_blob;
};

संरचना subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       तार्किक drop_hints_,
		       तार्किक coalesce_ = मिच्छा) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    सचे (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    विकल्प (context.type)
    {
      पक्ष CSType_CharString:
	पटिनिवत्त parsed_charstring;

      पक्ष CSType_LocalSubr:
	सचे (likely (context.subr_अङ्कः_2 < parsed_local_subrs->length))
	  पटिनिवत्त &(*parsed_local_subrs)[context.subr_अङ्कः_2];
	भङ्ग;

      पक्ष CSType_GlobalSubr:
	सचे (likely (context.subr_अङ्कः_2 < parsed_global_subrs->length))
	  पटिनिवत्त &(*parsed_global_subrs)[context.subr_अङ्कः_2];
	भङ्ग;
    }
    पटिनिवत्त nullptr;
  }

  template <typename ENV>
  शून्य set_current_str (ENV &env, तार्किक calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    सचे (unlikely (!parsed_str))
    {
      env.set_error ();
      पटिनिवत्त;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    सचे (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    अञ्ञथा
    {
      सचे (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        सचे (coalesce) parsed_str->enable_coalescing ();
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
  तार्किक	  drop_hints;
  तार्किक	  coalesce;
};

संरचना subr_remap_t : hb_inc_bimap_t
{
  शून्य create (ध्रुव hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    अत्थाय (auto old_अङ्कः : *closure)
      add (old_अङ्कः);

    सचे (get_population () < 1240)
      bias = 107;
    अञ्ञथा सचे (get_population () < 33900)
      bias = 1131;
    अञ्ञथा
      bias = 32768;
  }

  पूर्णाङ्क biased_अङ्कः (unsigned पूर्णाङ्क old_अङ्कः) ध्रुव
  {
    hb_codepoint_t new_अङ्कः = get (old_अङ्कः);
    पटिनिवत्त (पूर्णाङ्क)new_अङ्कः - bias;
  }

  रक्षित:
  पूर्णाङ्क bias;
};

संरचना subr_remaps_t
{
  subr_remaps_t (unsigned पूर्णाङ्क fd_गणनम्)
  {
    local_remaps.resize (fd_गणनम्);
  }

  तार्किक in_error()
  {
    पटिनिवत्त local_remaps.in_error ();
  }

  शून्य create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ध्रुव hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_गणनम्),
        remaps(acc_.fd_गणनम्)
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
  तार्किक subset (शून्य)
  {
    unsigned fd_गणनम्_2 = acc.fd_गणनम्;
    ध्रुव cff_subset_accelerator_t* cff_accelerator = nullptr;
    सचे (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_गणनम्_2 = cff_accelerator->parsed_local_subrs.length;
    }

    सचे (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->अङ्कः_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } अञ्ञथा {
      parsed_charstrings.resize_exact (plan->अङ्कः_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->गणनम्);

      सचे (unlikely (!parsed_local_subrs_storage.resize (fd_गणनम्_2))) पटिनिवत्त मिच्छा;

      अत्थाय (unsigned पूर्णाङ्क i = 0; i < acc.fd_गणनम्; i++)
      {
        unsigned गणनम् = acc.privateDicts[i].localSubrs->गणनम्;
        parsed_local_subrs_storage[i].resize (गणनम्);
        सचे (unlikely (parsed_local_subrs_storage[i].in_error ())) पटिनिवत्त मिच्छा;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    सचे (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      पटिनिवत्त मिच्छा;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    तार्किक coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    अत्थाय (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ध्रुव hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned पूर्णाङ्क fd = subset_get_fd (acc, old_glyph);
      सचे (unlikely (fd >= acc.fd_गणनम्))
        पटिनिवत्त मिच्छा;

      सचे (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        सचे (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        अञ्ञथा
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        अनुवर्त;
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

      सचे (unlikely (!interp.interpret (param)))
        पटिनिवत्त मिच्छा;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      सचे ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	सचे (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  सचे (drop.vsindex_dropped)
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
      सचे (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    सचे ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      पटिनिवत्त मिच्छा;

    remaps.create (closures);

    populate_subset_accelerator ();
    पटिनिवत्त सच्च;
  }

  तार्किक encode_charstrings (str_buff_vec_t &buffArray, तार्किक encode_prefix = सच्च) ध्रुव
  {
    unsigned अङ्कः_glyphs = plan->अङ्कः_output_glyphs ();
    सचे (unlikely (!buffArray.resize_exact (अङ्कः_glyphs)))
      पटिनिवत्त मिच्छा;
    hb_codepoint_t last = 0;
    अत्थाय (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      सचे (endchar_op != OpCode_Invalid)
        अत्थाय (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned वर्ण *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned पूर्णाङ्क  fd = subset_get_fd (acc, old_glyph);
      सचे (unlikely (fd >= acc.fd_गणनम्))
	पटिनिवत्त मिच्छा;
      सचे (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	पटिनिवत्त मिच्छा;
    }
    सचे (endchar_op != OpCode_Invalid)
      अत्थाय (; last < अङ्कः_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned वर्ण *>(endchar_str), 1);
      }

    पटिनिवत्त सच्च;
  }

  तार्किक encode_subrs (ध्रुव parsed_cs_str_vec_t &subrs, ध्रुव subr_remap_t& remap, unsigned पूर्णाङ्क fd, str_buff_vec_t &buffArray) ध्रुव
  {
    unsigned पूर्णाङ्क  गणनम् = remap.get_population ();

    सचे (unlikely (!buffArray.resize_exact (गणनम्)))
      पटिनिवत्त मिच्छा;
    अत्थाय (unsigned पूर्णाङ्क new_अङ्कः = 0; new_अङ्कः < गणनम्; new_अङ्कः++)
    {
      hb_codepoint_t old_अङ्कः = remap.backward (new_अङ्कः);
      प्रतिज्ञा (old_अङ्कः != CFF_UNDEF_CODE);

      सचे (unlikely (!encode_str (subrs[old_अङ्कः], fd, buffArray[new_अङ्कः])))
	पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }

  तार्किक encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    पटिनिवत्त encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  तार्किक encode_localsubrs (unsigned पूर्णाङ्क fd, str_buff_vec_t &buffArray) ध्रुव
  {
    पटिनिवत्त encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  रक्षित:
  संरचना drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (मिच्छा),
	ends_in_hint (मिच्छा),
	all_dropped (मिच्छा),
	vsindex_dropped (मिच्छा) {}

    तार्किक  seen_moveto;
    तार्किक  ends_in_hint;
    तार्किक  all_dropped;
    तार्किक  vsindex_dropped;
  };

  तार्किक drop_hints_in_subr (parsed_cs_str_t &str, unsigned पूर्णाङ्क pos,
			   parsed_cs_str_vec_t &subrs, unsigned पूर्णाङ्क subr_अङ्कः_2,
			   ध्रुव subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = मिच्छा;
    तार्किक has_hint = drop_hints_in_str (subrs[subr_अङ्कः_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    सचे (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      सचे (!str.at_end (pos))
	drop.ends_in_hint = मिच्छा;
    }
    अञ्ञथा सचे (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    पटिनिवत्त has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  तार्किक drop_hints_in_str (parsed_cs_str_t &str, ध्रुव subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    तार्किक  seen_hint = मिच्छा;

    unsigned गणनम् = str.values.length;
    auto *values = str.values.arrayZ;
    अत्थाय (unsigned पूर्णाङ्क pos = 0; pos < गणनम्; pos++)
    {
      तार्किक  has_hint = मिच्छा;
      विकल्प (values[pos].op)
      {
	पक्ष OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_अङ्कः_2,
					param, drop);
	  भङ्ग;

	पक्ष OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_अङ्कः_2,
					param, drop);
	  भङ्ग;

	पक्ष OpCode_rmoveto:
	पक्ष OpCode_hmoveto:
	पक्ष OpCode_vmoveto:
	  drop.seen_moveto = सच्च;
	  भङ्ग;

	पक्ष OpCode_hintmask:
	पक्ष OpCode_cntrmask:
	  सचे (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    भङ्ग;
	  }
	  HB_FALLTHROUGH;

	पक्ष OpCode_hstemhm:
	पक्ष OpCode_vstemhm:
	पक्ष OpCode_hstem:
	पक्ष OpCode_vstem:
	  has_hint = सच्च;
	  values[pos].set_hinting ();
	  सचे (str.at_end (pos))
	    drop.ends_in_hint = सच्च;
	  भङ्ग;

	पक्ष OpCode_dotsection:
	  values[pos].set_hinting ();
	  भङ्ग;

	स्वभाव:
	  /* NONE */
	  भङ्ग;
      }
      सचे (has_hint)
      {
	अत्थाय (पूर्णाङ्क i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  सचे (csop.is_hinting ())
	    भङ्ग;
	  csop.set_hinting ();
	  सचे (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = सच्च;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = सच्च;
    अत्थाय (unsigned पूर्णाङ्क pos = 0; pos < गणनम्; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      सचे (csop.op == OpCode_return)
	भङ्ग;
      सचे (!csop.is_hinting ())
      {
	drop.all_dropped = मिच्छा;
	भङ्ग;
      }
    }

    पटिनिवत्त seen_hint;
  }

  तार्किक closure_subroutines (ध्रुव parsed_cs_str_vec_t& global_subrs,
                            ध्रुव hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    अत्थाय (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned पूर्णाङ्क fd = subset_get_fd (acc, old_glyph);
      सचे (unlikely (fd >= acc.fd_गणनम्))
        पटिनिवत्त मिच्छा;

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

    पटिनिवत्त सच्च;
  }

  शून्य collect_subr_refs_in_subr (unsigned पूर्णाङ्क subr_अङ्कः_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ध्रुव subr_subset_param_t &param)
  {
    सचे (closure->has (subr_अङ्कः_2))
      पटिनिवत्त;
    closure->add (subr_अङ्कः_2);
    collect_subr_refs_in_str (subrs[subr_अङ्कः_2], param);
  }

  शून्य collect_subr_refs_in_str (ध्रुव parsed_cs_str_t &str,
                                 ध्रुव subr_subset_param_t &param)
  {
    सचे (!str.has_calls ())
      पटिनिवत्त;

    अत्थाय (auto &opstr : str.values)
    {
      सचे (!param.drop_hints || !opstr.is_hinting ())
      {
	विकल्प (opstr.op)
	{
	  पक्ष OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_अङ्कः_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    भङ्ग;

	  पक्ष OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_अङ्कः_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    भङ्ग;

	  स्वभाव: भङ्ग;
	}
      }
    }
  }

  तार्किक encode_str (ध्रुव parsed_cs_str_t &str, ध्रुव unsigned पूर्णाङ्क fd, str_buff_t &buff, तार्किक encode_prefix = सच्च) ध्रुव
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    तार्किक hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    सचे (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_अङ्कः_cs (str.prefix_अङ्कः_2 ());
      सचे (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    अत्थाय (auto &opstr : str.values)
    {
      size += opstr.length;
      सचे (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    सचे (!buff.alloc_exact (buff.length + size))
      पटिनिवत्त मिच्छा;

    अत्थाय (auto &opstr : str.values)
    {
      सचे (hinting || !opstr.is_hinting ())
      {
	विकल्प (opstr.op)
	{
	  पक्ष OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_अङ्कः (opstr.subr_अङ्कः_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    भङ्ग;

	  पक्ष OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_अङ्कः (opstr.subr_अङ्कः_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    भङ्ग;

	  स्वभाव:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    भङ्ग;
	}
      }
    }
    पटिनिवत्त !encoder.in_error ();
  }

  शून्य compact_parsed_subrs () ध्रुव
  {
    अत्थाय (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    अत्थाय (auto &vec : parsed_local_subrs_storage)
      अत्थाय (auto &cs : vec)
	cs.compact ();
  }

  शून्य populate_subset_accelerator () ध्रुव
  {
    सचे (!plan->inprogress_accelerator) पटिनिवत्त;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ध्रुव parsed_cs_str_t& get_parsed_charstring (unsigned i) ध्रुव
  {
    सचे (cached_charstrings) पटिनिवत्त *(cached_charstrings[i]);
    पटिनिवत्त parsed_charstrings[i];
  }

  रक्षित:
  ध्रुव ACC			&acc;
  ध्रुव hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ध्रुव parsed_cs_str_t*>     cached_charstrings;
  ध्रुव parsed_cs_str_vec_t*              parsed_global_subrs;
  ध्रुव hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  गुप्त:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  प्रकारनिर्देश typename SUBRS::गणनम्_type subr_गणनम्_type;
};

} /* namespace CFF */

HB_INTERNAL तार्किक
hb_plan_subset_cff_fdselect (ध्रुव hb_subset_plan_t *plan,
			    unsigned पूर्णाङ्क fd_गणनम्,
			    ध्रुव CFF::FDSelect &src, /* IN */
			    unsigned पूर्णाङ्क src_अङ्कः_glyphs,
			    unsigned पूर्णाङ्क &subset_fd_गणनम् /* OUT */,
			    unsigned पूर्णाङ्क &subset_fdselect_size /* OUT */,
			    unsigned पूर्णाङ्क &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL तार्किक
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned पूर्णाङ्क अङ्कः_glyphs,
			  ध्रुव CFF::FDSelect &src,
			  unsigned पूर्णाङ्क fd_गणनम्_2,
			  unsigned पूर्णाङ्क fdselect_format,
			  unsigned पूर्णाङ्क size,
			  ध्रुव hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
