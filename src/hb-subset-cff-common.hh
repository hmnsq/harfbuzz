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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-subset-plan.hh"
#அந்தர்பாவ "hb-cff-interp-cs-common.hh"

நாமாகாஶ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
நிலையான உள்வரி unsigned
subset_get_fd (மாறிலி ACC &acc, hb_codepoint_t glyph)
{
  திரும்பு likely (glyph < acc.இலக்கம்_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
கட்டமைப்பு str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  வெறுமை reset () { buff.reset (); }

  வெறுமை encode_byte (unsigned எழுத்து b)
  {
    எனில் (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    இல்லையெனில்
      buff.push (b);
  }

  வெறுமை encode_int (முழு v)
  {
    எனில் ((-1131 <= v) && (v <= 1131))
    {
      எனில் ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      இல்லையெனில் எனில் (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      இல்லையெனில்
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    இல்லையெனில்
    {
      எனில் (unlikely (v < -32768))
	v = -32768;
      இல்லையெனில் எனில் (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  வெறுமை encode_இலக்கம்_cs (மாறிலி எண்_t& n)
  {
    எனில் (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    இல்லையெனில்
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
  வெறுமை encode_இலக்கம்_tp (மாறிலி எண்_t& n)
  {
    எனில் (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    இல்லையெனில்
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      இரட்டை v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      எழுத்து buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", வெற்று));
      snprintf (buf, அளவு (buf), "%.8G", v);
      (வெறுமை) hb_uselocale (((வெறுமை) freelocale (clocale), oldlocale));

      எழுத்து *s = buf;
      size_t len;
      எழுத்து *comma = strchr (s, ',');
      எனில் (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      எனில் (s[0] == '0' && s[1] == '.')
	s++;
      இல்லையெனில் எனில் (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      இல்லையெனில் எனில் ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	எழுத்து *s2 = s + exponent - 1;
	போது (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, அளவு (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      இல்லையெனில்
      {
	எழுத்து *dot = strchr (s, '.');
	எழுத்து *e = strchr (s, 'E');
	எனில் (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  முழு exponent = atoi (e + 1);
	  முழு new_exponent = exponent - (e - (dot + 1));
	  எனில் (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  இல்லையெனில்
	    snprintf (e - 1, அளவு (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      எனில் ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	முழு sign = s[0] == '-';
	எழுத்து *s2 = s + sign + 1;
	போது (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, அளவு (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<எழுத்து> nibbles;
      போது (*s)
      {
	எழுத்து c = s[0];
	s++;

	தேர்வு (c)
	{
	  நிலை 'E':
	  {
	    எழுத்து c2 = *s;
	    எனில் (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } இல்லையெனில் {
	      எனில் (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    எனில் (*s == '0')
	      s++;
	    தொடர்;
	  }

	  நிலை '.':
	    nibbles.push (0x0A); // .
	    தொடர்;

	  நிலை '-':
	    nibbles.push (0x0E); // -
	    தொடர்;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      எனில் (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned மொத்தம் = nibbles.length;
      க்கு (unsigned i = 0; i < மொத்தம்; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  வெறுமை encode_op (op_code_t op)
  {
    எனில் (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    இல்லையெனில்
      encode_byte (op);
  }

  வெறுமை copy_str (மாறிலி unsigned எழுத்து *str, unsigned length)
  {
    ப்ரதிஜ்ஞா ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  தருக்க in_error () மாறிலி { திரும்பு buff.in_error (); }

  பாதுகாக்கப்பட்ட:

  str_buff_t &buff;
};

கட்டமைப்பு cff_sub_table_info_t {
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
கட்டமைப்பு cff_உச்சி_dict_op_serializer_t : op_serializer_t
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி OPSTR &opstr,
		  மாறிலி cff_sub_table_info_t &info) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    தேர்வு (opstr.op)
    {
      நிலை OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      நிலை OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      நிலை OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      இயல்பு:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (உண்மை);
  }
};

கட்டமைப்பு cff_font_dict_op_serializer_t : op_serializer_t
{
  தருக்க serialize (hb_serialize_context_t *c,
		  மாறிலி op_str_t &opstr,
		  மாறிலி table_info_t &privateDictInfo) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    எனில் (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    இல்லையெனில்
    {
      unsigned எழுத்து *d = c->allocate_size<unsigned எழுத்து> (opstr.length);
      எனில் (unlikely (!d)) return_trace (பொய்);
      /* Faster than hb_memcpy for small strings. */
      க்கு (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (உண்மை);
  }
};

/* CharString command for specialization */
கட்டமைப்பு cs_command_t
{
  hb_vector_t<எண்_t> args;
  hb_vector_t<unsigned எழுத்து> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

வகைவரை hb_vector_t<cs_command_t> *cs_command_vec_t;

கட்டமைப்பு cff2_instancing_plan_t;

கட்டமைப்பு flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   தருக்க drop_hints_,
                   மாறிலி hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  தருக்க	drop_hints;
  மாறிலி hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  மாறிலி cff2_instancing_plan_t *instancer = nullptr;
  தருக்க emitted_blend = பொய்;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
கட்டமைப்பு subr_flattener_t
{
  subr_flattener_t (மாறிலி ACC &acc_,
		    மாறிலி hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  தருக்க flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned மொத்தம் = plan->இலக்கம்_output_glyphs ();
    எனில் (!flat_charstrings.resize_exact (மொத்தம்))
      திரும்பு பொய்;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      hb_codepoint_t  glyph;
      எனில் (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	எனில் (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	தொடர்;
      }
      மாறிலி hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned முழு fd = subset_get_fd (acc, glyph);
      எனில் (unlikely (fd >= acc.fd_மொத்தம்))
	திரும்பு பொய்;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (தருக்க) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      எனில் (unlikely (!interp.interpret (param)))
	திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  மாறிலி ACC &acc;
  மாறிலி hb_subset_plan_t *plan;
};

கட்டமைப்பு subr_closures_t
{
  subr_closures_t (unsigned முழு fd_மொத்தம்_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_மொத்தம்_2);
  }

  வெறுமை reset ()
  {
    global_closure.clear();
    க்கு (unsigned முழு i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  தருக்க in_error () மாறிலி { திரும்பு local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

கட்டமைப்பு parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned முழு subr_இலக்கம் = 0) :
    subr_இலக்கம்_2 (subr_இலக்கம்) {}

  தருக்க is_hinting () மாறிலி { திரும்பு hinting_flag; }
  வெறுமை set_hinting ()       { hinting_flag = உண்மை; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  பாதுகாக்கப்பட்ட:
  தருக்க	  hinting_flag = பொய்;

  பொது:
  uint16_t subr_இலக்கம்_2;
};

கட்டமைப்பு parsed_cs_str_t : parsed_மதிப்புகள்_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (பொய்),
    hint_dropped (பொய்),
    has_prefix_ (பொய்),
    has_calls_ (பொய்),
    coalescing_ (பொய்)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  வெறுமை add_op (op_code_t op, மாறிலி byte_str_ref_t& str_ref)
  {
    எனில் (is_parsed ()) திரும்பு;
    எனில் (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      எனில் (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      திரும்பு;
    }
    SUPER::add_op (op, str_ref);
  }

  வெறுமை add_call_op (op_code_t op, மாறிலி byte_str_ref_t& str_ref, unsigned முழு subr_இலக்கம்_2)
  {
    எனில் (is_parsed ()) திரும்பு;
    has_calls_ = உண்மை;

    எனில் (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_இலக்கம்_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      திரும்பு;
    }

    /* Pop the subroutine number. */
    values.எடு ();

    SUPER::add_op (op, str_ref, {subr_இலக்கம்_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  வெறுமை flush_segment (மாறிலி byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    எனில் (end <= start) திரும்பு;
    போது (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      எனில் (unlikely (!arr.length)) நிறுத்து;
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
  வெறுமை flush_coalesced (மாறிலி byte_str_ref_t& str_ref)
  {
    எனில் (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  வெறுமை enable_coalescing () { coalescing_ = உண்மை; }
  தருக்க is_coalescing () மாறிலி { திரும்பு coalescing_; }

  வெறுமை set_prefix (மாறிலி எண்_t &இலக்கம், op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = உண்மை;
    prefix_op_ = op;
    prefix_இலக்கம் = இலக்கம்;
  }

  தருக்க at_end (unsigned முழு pos) மாறிலி
  {
    திரும்பு ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  தருக்க is_parsed () மாறிலி { திரும்பு parsed; }
  வெறுமை set_parsed ()      { parsed = உண்மை; }

  தருக்க is_hint_dropped () மாறிலி { திரும்பு hint_dropped; }
  வெறுமை set_hint_dropped ()      { hint_dropped = உண்மை; }

  தருக்க is_vsindex_dropped () மாறிலி { திரும்பு vsindex_dropped; }
  வெறுமை set_vsindex_dropped ()      { vsindex_dropped = உண்மை; }

  தருக்க has_prefix () மாறிலி          { திரும்பு has_prefix_; }
  op_code_t prefix_op () மாறிலி         { திரும்பு prefix_op_; }
  மாறிலி எண்_t &prefix_இலக்கம்_2 () மாறிலி { திரும்பு prefix_இலக்கம்; }

  தருக்க has_calls () மாறிலி          { திரும்பு has_calls_; }

  வெறுமை compact ()
  {
    unsigned மொத்தம் = values.length;
    எனில் (!மொத்தம்) திரும்பு;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    க்கு (unsigned i = 1; i < மொத்தம்; i++)
    {
      /* See if we can combine op j and op i. */
      தருக்க combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      எனில் (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      இல்லையெனில்
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  பாதுகாக்கப்பட்ட:
  தருக்க    parsed : 1;
  தருக்க    hint_dropped : 1;
  தருக்க    vsindex_dropped : 1;
  தருக்க    has_prefix_ : 1;
  தருக்க    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  தருக்க    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  எண்_t	prefix_இலக்கம்;

  தனிப்பட்ட:
  வகைவரை parsed_மதிப்புகள்_t<parsed_cs_op_t> SUPER;
};

கட்டமைப்பு parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  தனிப்பட்ட:
  வகைவரை hb_vector_t<parsed_cs_str_t> SUPER;
};

கட்டமைப்பு cff_subset_accelerator_t
{
  நிலையான cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      மாறிலி parsed_cs_str_vec_t& parsed_charstrings,
      மாறிலி parsed_cs_str_vec_t& parsed_global_subrs,
      மாறிலி hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (அளவு(cff_subset_accelerator_t));
    எனில் (unlikely (!accel)) திரும்பு nullptr;
    புதிய (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    திரும்பு accel;
  }

  நிலையான வெறுமை destroy (வெறுமை* value) {
    எனில் (!value) திரும்பு;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      மாறிலி parsed_cs_str_vec_t& parsed_charstrings_,
      மாறிலி parsed_cs_str_vec_t& parsed_global_subrs_,
      மாறிலி hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    எனில் (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 தனிப்பட்ட:
  hb_blob_t* original_blob;
};

கட்டமைப்பு subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       தருக்க drop_hints_,
		       தருக்க coalesce_ = பொய்) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    எனில் (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    தேர்வு (context.type)
    {
      நிலை CSType_CharString:
	திரும்பு parsed_charstring;

      நிலை CSType_LocalSubr:
	எனில் (likely (context.subr_இலக்கம்_2 < parsed_local_subrs->length))
	  திரும்பு &(*parsed_local_subrs)[context.subr_இலக்கம்_2];
	நிறுத்து;

      நிலை CSType_GlobalSubr:
	எனில் (likely (context.subr_இலக்கம்_2 < parsed_global_subrs->length))
	  திரும்பு &(*parsed_global_subrs)[context.subr_இலக்கம்_2];
	நிறுத்து;
    }
    திரும்பு nullptr;
  }

  template <typename ENV>
  வெறுமை set_current_str (ENV &env, தருக்க calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    எனில் (unlikely (!parsed_str))
    {
      env.set_error ();
      திரும்பு;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    எனில் (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    இல்லையெனில்
    {
      எனில் (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        எனில் (coalesce) parsed_str->enable_coalescing ();
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
  தருக்க	  drop_hints;
  தருக்க	  coalesce;
};

கட்டமைப்பு subr_remap_t : hb_inc_bimap_t
{
  வெறுமை create (மாறிலி hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    க்கு (auto old_இலக்கம் : *closure)
      add (old_இலக்கம்);

    எனில் (get_population () < 1240)
      bias = 107;
    இல்லையெனில் எனில் (get_population () < 33900)
      bias = 1131;
    இல்லையெனில்
      bias = 32768;
  }

  முழு biased_இலக்கம் (unsigned முழு old_இலக்கம்) மாறிலி
  {
    hb_codepoint_t new_இலக்கம் = get (old_இலக்கம்);
    திரும்பு (முழு)new_இலக்கம் - bias;
  }

  பாதுகாக்கப்பட்ட:
  முழு bias;
};

கட்டமைப்பு subr_remaps_t
{
  subr_remaps_t (unsigned முழு fd_மொத்தம்)
  {
    local_remaps.resize (fd_மொத்தம்);
  }

  தருக்க in_error()
  {
    திரும்பு local_remaps.in_error ();
  }

  வெறுமை create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    க்கு (unsigned முழு i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
கட்டமைப்பு subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, மாறிலி hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_மொத்தம்),
        remaps(acc_.fd_மொத்தம்)
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
  தருக்க subset (வெறுமை)
  {
    unsigned fd_மொத்தம்_2 = acc.fd_மொத்தம்;
    மாறிலி cff_subset_accelerator_t* cff_accelerator = nullptr;
    எனில் (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_மொத்தம்_2 = cff_accelerator->parsed_local_subrs.length;
    }

    எனில் (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->இலக்கம்_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } இல்லையெனில் {
      parsed_charstrings.resize_exact (plan->இலக்கம்_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->மொத்தம்);

      எனில் (unlikely (!parsed_local_subrs_storage.resize (fd_மொத்தம்_2))) திரும்பு பொய்;

      க்கு (unsigned முழு i = 0; i < acc.fd_மொத்தம்; i++)
      {
        unsigned மொத்தம் = acc.privateDicts[i].localSubrs->மொத்தம்;
        parsed_local_subrs_storage[i].resize (மொத்தம்);
        எனில் (unlikely (parsed_local_subrs_storage[i].in_error ())) திரும்பு பொய்;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    எனில் (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      திரும்பு பொய்;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    தருக்க coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    க்கு (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      மாறிலி hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned முழு fd = subset_get_fd (acc, old_glyph);
      எனில் (unlikely (fd >= acc.fd_மொத்தம்))
        திரும்பு பொய்;

      எனில் (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        எனில் (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        இல்லையெனில்
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        தொடர்;
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

      எனில் (unlikely (!interp.interpret (param)))
        திரும்பு பொய்;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      எனில் ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	எனில் (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  எனில் (drop.vsindex_dropped)
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
      எனில் (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    எனில் ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      திரும்பு பொய்;

    remaps.create (closures);

    populate_subset_accelerator ();
    திரும்பு உண்மை;
  }

  தருக்க encode_charstrings (str_buff_vec_t &buffArray, தருக்க encode_prefix = உண்மை) மாறிலி
  {
    unsigned இலக்கம்_glyphs = plan->இலக்கம்_output_glyphs ();
    எனில் (unlikely (!buffArray.resize_exact (இலக்கம்_glyphs)))
      திரும்பு பொய்;
    hb_codepoint_t last = 0;
    க்கு (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      எனில் (endchar_op != OpCode_Invalid)
        க்கு (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned எழுத்து *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned முழு  fd = subset_get_fd (acc, old_glyph);
      எனில் (unlikely (fd >= acc.fd_மொத்தம்))
	திரும்பு பொய்;
      எனில் (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	திரும்பு பொய்;
    }
    எனில் (endchar_op != OpCode_Invalid)
      க்கு (; last < இலக்கம்_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned எழுத்து *>(endchar_str), 1);
      }

    திரும்பு உண்மை;
  }

  தருக்க encode_subrs (மாறிலி parsed_cs_str_vec_t &subrs, மாறிலி subr_remap_t& remap, unsigned முழு fd, str_buff_vec_t &buffArray) மாறிலி
  {
    unsigned முழு  மொத்தம் = remap.get_population ();

    எனில் (unlikely (!buffArray.resize_exact (மொத்தம்)))
      திரும்பு பொய்;
    க்கு (unsigned முழு new_இலக்கம் = 0; new_இலக்கம் < மொத்தம்; new_இலக்கம்++)
    {
      hb_codepoint_t old_இலக்கம் = remap.backward (new_இலக்கம்);
      ப்ரதிஜ்ஞா (old_இலக்கம் != CFF_UNDEF_CODE);

      எனில் (unlikely (!encode_str (subrs[old_இலக்கம்], fd, buffArray[new_இலக்கம்])))
	திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  தருக்க encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    திரும்பு encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  தருக்க encode_localsubrs (unsigned முழு fd, str_buff_vec_t &buffArray) மாறிலி
  {
    திரும்பு encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  பாதுகாக்கப்பட்ட:
  கட்டமைப்பு drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (பொய்),
	ends_in_hint (பொய்),
	all_dropped (பொய்),
	vsindex_dropped (பொய்) {}

    தருக்க  seen_moveto;
    தருக்க  ends_in_hint;
    தருக்க  all_dropped;
    தருக்க  vsindex_dropped;
  };

  தருக்க drop_hints_in_subr (parsed_cs_str_t &str, unsigned முழு pos,
			   parsed_cs_str_vec_t &subrs, unsigned முழு subr_இலக்கம்_2,
			   மாறிலி subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = பொய்;
    தருக்க has_hint = drop_hints_in_str (subrs[subr_இலக்கம்_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    எனில் (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      எனில் (!str.at_end (pos))
	drop.ends_in_hint = பொய்;
    }
    இல்லையெனில் எனில் (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    திரும்பு has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  தருக்க drop_hints_in_str (parsed_cs_str_t &str, மாறிலி subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    தருக்க  seen_hint = பொய்;

    unsigned மொத்தம் = str.values.length;
    auto *values = str.values.arrayZ;
    க்கு (unsigned முழு pos = 0; pos < மொத்தம்; pos++)
    {
      தருக்க  has_hint = பொய்;
      தேர்வு (values[pos].op)
      {
	நிலை OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_இலக்கம்_2,
					param, drop);
	  நிறுத்து;

	நிலை OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_இலக்கம்_2,
					param, drop);
	  நிறுத்து;

	நிலை OpCode_rmoveto:
	நிலை OpCode_hmoveto:
	நிலை OpCode_vmoveto:
	  drop.seen_moveto = உண்மை;
	  நிறுத்து;

	நிலை OpCode_hintmask:
	நிலை OpCode_cntrmask:
	  எனில் (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    நிறுத்து;
	  }
	  HB_FALLTHROUGH;

	நிலை OpCode_hstemhm:
	நிலை OpCode_vstemhm:
	நிலை OpCode_hstem:
	நிலை OpCode_vstem:
	  has_hint = உண்மை;
	  values[pos].set_hinting ();
	  எனில் (str.at_end (pos))
	    drop.ends_in_hint = உண்மை;
	  நிறுத்து;

	நிலை OpCode_dotsection:
	  values[pos].set_hinting ();
	  நிறுத்து;

	இயல்பு:
	  /* NONE */
	  நிறுத்து;
      }
      எனில் (has_hint)
      {
	க்கு (முழு i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  எனில் (csop.is_hinting ())
	    நிறுத்து;
	  csop.set_hinting ();
	  எனில் (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = உண்மை;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = உண்மை;
    க்கு (unsigned முழு pos = 0; pos < மொத்தம்; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      எனில் (csop.op == OpCode_return)
	நிறுத்து;
      எனில் (!csop.is_hinting ())
      {
	drop.all_dropped = பொய்;
	நிறுத்து;
      }
    }

    திரும்பு seen_hint;
  }

  தருக்க closure_subroutines (மாறிலி parsed_cs_str_vec_t& global_subrs,
                            மாறிலி hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    க்கு (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned முழு fd = subset_get_fd (acc, old_glyph);
      எனில் (unlikely (fd >= acc.fd_மொத்தம்))
        திரும்பு பொய்;

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

    திரும்பு உண்மை;
  }

  வெறுமை collect_subr_refs_in_subr (unsigned முழு subr_இலக்கம்_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  மாறிலி subr_subset_param_t &param)
  {
    எனில் (closure->has (subr_இலக்கம்_2))
      திரும்பு;
    closure->add (subr_இலக்கம்_2);
    collect_subr_refs_in_str (subrs[subr_இலக்கம்_2], param);
  }

  வெறுமை collect_subr_refs_in_str (மாறிலி parsed_cs_str_t &str,
                                 மாறிலி subr_subset_param_t &param)
  {
    எனில் (!str.has_calls ())
      திரும்பு;

    க்கு (auto &opstr : str.values)
    {
      எனில் (!param.drop_hints || !opstr.is_hinting ())
      {
	தேர்வு (opstr.op)
	{
	  நிலை OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_இலக்கம்_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    நிறுத்து;

	  நிலை OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_இலக்கம்_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    நிறுத்து;

	  இயல்பு: நிறுத்து;
	}
      }
    }
  }

  தருக்க encode_str (மாறிலி parsed_cs_str_t &str, மாறிலி unsigned முழு fd, str_buff_t &buff, தருக்க encode_prefix = உண்மை) மாறிலி
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    தருக்க hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    எனில் (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_இலக்கம்_cs (str.prefix_இலக்கம்_2 ());
      எனில் (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    க்கு (auto &opstr : str.values)
    {
      size += opstr.length;
      எனில் (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    எனில் (!buff.alloc_exact (buff.length + size))
      திரும்பு பொய்;

    க்கு (auto &opstr : str.values)
    {
      எனில் (hinting || !opstr.is_hinting ())
      {
	தேர்வு (opstr.op)
	{
	  நிலை OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_இலக்கம் (opstr.subr_இலக்கம்_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    நிறுத்து;

	  நிலை OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_இலக்கம் (opstr.subr_இலக்கம்_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    நிறுத்து;

	  இயல்பு:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    நிறுத்து;
	}
      }
    }
    திரும்பு !encoder.in_error ();
  }

  வெறுமை compact_parsed_subrs () மாறிலி
  {
    க்கு (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    க்கு (auto &vec : parsed_local_subrs_storage)
      க்கு (auto &cs : vec)
	cs.compact ();
  }

  வெறுமை populate_subset_accelerator () மாறிலி
  {
    எனில் (!plan->inprogress_accelerator) திரும்பு;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  மாறிலி parsed_cs_str_t& get_parsed_charstring (unsigned i) மாறிலி
  {
    எனில் (cached_charstrings) திரும்பு *(cached_charstrings[i]);
    திரும்பு parsed_charstrings[i];
  }

  பாதுகாக்கப்பட்ட:
  மாறிலி ACC			&acc;
  மாறிலி hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<மாறிலி parsed_cs_str_t*>     cached_charstrings;
  மாறிலி parsed_cs_str_vec_t*              parsed_global_subrs;
  மாறிலி hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  தனிப்பட்ட:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  வகைவரை typename SUBRS::மொத்தம்_type subr_மொத்தம்_type;
};

} /* namespace CFF */

HB_INTERNAL தருக்க
hb_plan_subset_cff_fdselect (மாறிலி hb_subset_plan_t *plan,
			    unsigned முழு fd_மொத்தம்,
			    மாறிலி CFF::FDSelect &src, /* IN */
			    unsigned முழு src_இலக்கம்_glyphs,
			    unsigned முழு &subset_fd_மொத்தம் /* OUT */,
			    unsigned முழு &subset_fdselect_size /* OUT */,
			    unsigned முழு &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL தருக்க
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned முழு இலக்கம்_glyphs,
			  மாறிலி CFF::FDSelect &src,
			  unsigned முழு fd_மொத்தம்_2,
			  unsigned முழு fdselect_format,
			  unsigned முழு size,
			  மாறிலி hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
