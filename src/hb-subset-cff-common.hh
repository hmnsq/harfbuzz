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

#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-subset-plan.hh"
#અન્તર્ભાવ "hb-cff-interp-cs-common.hh"

નામાકાશ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
સ્થિર અંતઃરેખ unsigned
subset_get_fd (અચલ ACC &acc, hb_codepoint_t glyph)
{
  પાછા likely (glyph < acc.અંક_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
સંરચના str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  શૂન્ય reset () { buff.reset (); }

  શૂન્ય encode_byte (unsigned અક્ષર b)
  {
    જો (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    નહીંતર
      buff.push (b);
  }

  શૂન્ય encode_int (પૂર્ણાંક v)
  {
    જો ((-1131 <= v) && (v <= 1131))
    {
      જો ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      નહીંતર જો (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      નહીંતર
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    નહીંતર
    {
      જો (unlikely (v < -32768))
	v = -32768;
      નહીંતર જો (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  શૂન્ય encode_અંક_cs (અચલ સંખ્યા_t& n)
  {
    જો (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    નહીંતર
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
  શૂન્ય encode_અંક_tp (અચલ સંખ્યા_t& n)
  {
    જો (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    નહીંતર
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      દ્વિગુણ v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      અક્ષર buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ખાલી));
      snprintf (buf, કદ (buf), "%.8G", v);
      (શૂન્ય) hb_uselocale (((શૂન્ય) freelocale (clocale), oldlocale));

      અક્ષર *s = buf;
      size_t len;
      અક્ષર *comma = strchr (s, ',');
      જો (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      જો (s[0] == '0' && s[1] == '.')
	s++;
      નહીંતર જો (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      નહીંતર જો ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	અક્ષર *s2 = s + exponent - 1;
	જ્યારે (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, કદ (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      નહીંતર
      {
	અક્ષર *dot = strchr (s, '.');
	અક્ષર *e = strchr (s, 'E');
	જો (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  પૂર્ણાંક exponent = atoi (e + 1);
	  પૂર્ણાંક new_exponent = exponent - (e - (dot + 1));
	  જો (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  નહીંતર
	    snprintf (e - 1, કદ (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      જો ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	પૂર્ણાંક sign = s[0] == '-';
	અક્ષર *s2 = s + sign + 1;
	જ્યારે (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, કદ (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<અક્ષર> nibbles;
      જ્યારે (*s)
      {
	અક્ષર c = s[0];
	s++;

	પસંદગી (c)
	{
	  સ્થિતિ 'E':
	  {
	    અક્ષર c2 = *s;
	    જો (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } નહીંતર {
	      જો (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    જો (*s == '0')
	      s++;
	    ચાલુ;
	  }

	  સ્થિતિ '.':
	    nibbles.push (0x0A); // .
	    ચાલુ;

	  સ્થિતિ '-':
	    nibbles.push (0x0E); // -
	    ચાલુ;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      જો (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned ગણતરી = nibbles.length;
      માટે (unsigned i = 0; i < ગણતરી; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  શૂન્ય encode_op (op_code_t op)
  {
    જો (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    નહીંતર
      encode_byte (op);
  }

  શૂન્ય copy_str (અચલ unsigned અક્ષર *str, unsigned length)
  {
    પ્રતિજ્ઞા ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  તાર્કિક in_error () અચલ { પાછા buff.in_error (); }

  સુરક્ષિત:

  str_buff_t &buff;
};

સંરચના cff_sub_table_info_t {
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
સંરચના cff_ટોચ_dict_op_serializer_t : op_serializer_t
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ OPSTR &opstr,
		  અચલ cff_sub_table_info_t &info) અચલ
  {
    TRACE_SERIALIZE (આ);

    પસંદગી (opstr.op)
    {
      સ્થિતિ OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      સ્થિતિ OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      સ્થિતિ OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      મૂળભૂત:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (સાચું);
  }
};

સંરચના cff_font_dict_op_serializer_t : op_serializer_t
{
  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ op_str_t &opstr,
		  અચલ table_info_t &privateDictInfo) અચલ
  {
    TRACE_SERIALIZE (આ);

    જો (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    નહીંતર
    {
      unsigned અક્ષર *d = c->allocate_size<unsigned અક્ષર> (opstr.length);
      જો (unlikely (!d)) return_trace (ખોટું);
      /* Faster than hb_memcpy for small strings. */
      માટે (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (સાચું);
  }
};

/* CharString command for specialization */
સંરચના cs_command_t
{
  hb_vector_t<સંખ્યા_t> args;
  hb_vector_t<unsigned અક્ષર> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

પ્રકારવ્યાખ્યા hb_vector_t<cs_command_t> *cs_command_vec_t;

સંરચના cff2_instancing_plan_t;

સંરચના flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   તાર્કિક drop_hints_,
                   અચલ hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  તાર્કિક	drop_hints;
  અચલ hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  અચલ cff2_instancing_plan_t *instancer = nullptr;
  તાર્કિક emitted_blend = ખોટું;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
સંરચના subr_flattener_t
{
  subr_flattener_t (અચલ ACC &acc_,
		    અચલ hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  તાર્કિક flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned ગણતરી = plan->અંક_output_glyphs ();
    જો (!flat_charstrings.resize_exact (ગણતરી))
      પાછા ખોટું;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      hb_codepoint_t  glyph;
      જો (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	જો (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	ચાલુ;
      }
      અચલ hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned પૂર્ણાંક fd = subset_get_fd (acc, glyph);
      જો (unlikely (fd >= acc.fd_ગણતરી))
	પાછા ખોટું;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (તાર્કિક) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      જો (unlikely (!interp.interpret (param)))
	પાછા ખોટું;
    }
    પાછા સાચું;
  }

  અચલ ACC &acc;
  અચલ hb_subset_plan_t *plan;
};

સંરચના subr_closures_t
{
  subr_closures_t (unsigned પૂર્ણાંક fd_ગણતરી_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_ગણતરી_2);
  }

  શૂન્ય reset ()
  {
    global_closure.clear();
    માટે (unsigned પૂર્ણાંક i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  તાર્કિક in_error () અચલ { પાછા local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

સંરચના parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned પૂર્ણાંક subr_અંક = 0) :
    subr_અંક_2 (subr_અંક) {}

  તાર્કિક is_hinting () અચલ { પાછા hinting_flag; }
  શૂન્ય set_hinting ()       { hinting_flag = સાચું; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  સુરક્ષિત:
  તાર્કિક	  hinting_flag = ખોટું;

  જાહેર:
  uint16_t subr_અંક_2;
};

સંરચના parsed_cs_str_t : parsed_કિંમતો_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (ખોટું),
    hint_dropped (ખોટું),
    has_prefix_ (ખોટું),
    has_calls_ (ખોટું),
    coalescing_ (ખોટું)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  શૂન્ય add_op (op_code_t op, અચલ byte_str_ref_t& str_ref)
  {
    જો (is_parsed ()) પાછા;
    જો (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      જો (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      પાછા;
    }
    SUPER::add_op (op, str_ref);
  }

  શૂન્ય add_call_op (op_code_t op, અચલ byte_str_ref_t& str_ref, unsigned પૂર્ણાંક subr_અંક_2)
  {
    જો (is_parsed ()) પાછા;
    has_calls_ = સાચું;

    જો (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_અંક_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      પાછા;
    }

    /* Pop the subroutine number. */
    values.કાઢો ();

    SUPER::add_op (op, str_ref, {subr_અંક_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  શૂન્ય flush_segment (અચલ byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    જો (end <= start) પાછા;
    જ્યારે (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      જો (unlikely (!arr.length)) તોડો;
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
  શૂન્ય flush_coalesced (અચલ byte_str_ref_t& str_ref)
  {
    જો (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  શૂન્ય enable_coalescing () { coalescing_ = સાચું; }
  તાર્કિક is_coalescing () અચલ { પાછા coalescing_; }

  શૂન્ય set_prefix (અચલ સંખ્યા_t &અંક, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = સાચું;
    prefix_op_ = op;
    prefix_અંક = અંક;
  }

  તાર્કિક at_end (unsigned પૂર્ણાંક pos) અચલ
  {
    પાછા ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  તાર્કિક is_parsed () અચલ { પાછા parsed; }
  શૂન્ય set_parsed ()      { parsed = સાચું; }

  તાર્કિક is_hint_dropped () અચલ { પાછા hint_dropped; }
  શૂન્ય set_hint_dropped ()      { hint_dropped = સાચું; }

  તાર્કિક is_vsindex_dropped () અચલ { પાછા vsindex_dropped; }
  શૂન્ય set_vsindex_dropped ()      { vsindex_dropped = સાચું; }

  તાર્કિક has_prefix () અચલ          { પાછા has_prefix_; }
  op_code_t prefix_op () અચલ         { પાછા prefix_op_; }
  અચલ સંખ્યા_t &prefix_અંક_2 () અચલ { પાછા prefix_અંક; }

  તાર્કિક has_calls () અચલ          { પાછા has_calls_; }

  શૂન્ય compact ()
  {
    unsigned ગણતરી = values.length;
    જો (!ગણતરી) પાછા;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    માટે (unsigned i = 1; i < ગણતરી; i++)
    {
      /* See if we can combine op j and op i. */
      તાર્કિક combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      જો (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      નહીંતર
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  સુરક્ષિત:
  તાર્કિક    parsed : 1;
  તાર્કિક    hint_dropped : 1;
  તાર્કિક    vsindex_dropped : 1;
  તાર્કિક    has_prefix_ : 1;
  તાર્કિક    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  તાર્કિક    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  સંખ્યા_t	prefix_અંક;

  ખાનગી:
  પ્રકારવ્યાખ્યા parsed_કિંમતો_t<parsed_cs_op_t> SUPER;
};

સંરચના parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  ખાનગી:
  પ્રકારવ્યાખ્યા hb_vector_t<parsed_cs_str_t> SUPER;
};

સંરચના cff_subset_accelerator_t
{
  સ્થિર cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      અચલ parsed_cs_str_vec_t& parsed_charstrings,
      અચલ parsed_cs_str_vec_t& parsed_global_subrs,
      અચલ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (કદ(cff_subset_accelerator_t));
    જો (unlikely (!accel)) પાછા nullptr;
    નવું (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    પાછા accel;
  }

  સ્થિર શૂન્ય destroy (શૂન્ય* value) {
    જો (!value) પાછા;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      અચલ parsed_cs_str_vec_t& parsed_charstrings_,
      અચલ parsed_cs_str_vec_t& parsed_global_subrs_,
      અચલ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    જો (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 ખાનગી:
  hb_blob_t* original_blob;
};

સંરચના subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       તાર્કિક drop_hints_,
		       તાર્કિક coalesce_ = ખોટું) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    જો (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    પસંદગી (context.type)
    {
      સ્થિતિ CSType_CharString:
	પાછા parsed_charstring;

      સ્થિતિ CSType_LocalSubr:
	જો (likely (context.subr_અંક_2 < parsed_local_subrs->length))
	  પાછા &(*parsed_local_subrs)[context.subr_અંક_2];
	તોડો;

      સ્થિતિ CSType_GlobalSubr:
	જો (likely (context.subr_અંક_2 < parsed_global_subrs->length))
	  પાછા &(*parsed_global_subrs)[context.subr_અંક_2];
	તોડો;
    }
    પાછા nullptr;
  }

  template <typename ENV>
  શૂન્ય set_current_str (ENV &env, તાર્કિક calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    જો (unlikely (!parsed_str))
    {
      env.set_error ();
      પાછા;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    જો (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    નહીંતર
    {
      જો (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        જો (coalesce) parsed_str->enable_coalescing ();
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
  તાર્કિક	  drop_hints;
  તાર્કિક	  coalesce;
};

સંરચના subr_remap_t : hb_inc_bimap_t
{
  શૂન્ય create (અચલ hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    માટે (auto old_અંક : *closure)
      add (old_અંક);

    જો (get_population () < 1240)
      bias = 107;
    નહીંતર જો (get_population () < 33900)
      bias = 1131;
    નહીંતર
      bias = 32768;
  }

  પૂર્ણાંક biased_અંક (unsigned પૂર્ણાંક old_અંક) અચલ
  {
    hb_codepoint_t new_અંક = get (old_અંક);
    પાછા (પૂર્ણાંક)new_અંક - bias;
  }

  સુરક્ષિત:
  પૂર્ણાંક bias;
};

સંરચના subr_remaps_t
{
  subr_remaps_t (unsigned પૂર્ણાંક fd_ગણતરી)
  {
    local_remaps.resize (fd_ગણતરી);
  }

  તાર્કિક in_error()
  {
    પાછા local_remaps.in_error ();
  }

  શૂન્ય create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    માટે (unsigned પૂર્ણાંક i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
સંરચના subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, અચલ hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_ગણતરી),
        remaps(acc_.fd_ગણતરી)
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
  તાર્કિક subset (શૂન્ય)
  {
    unsigned fd_ગણતરી_2 = acc.fd_ગણતરી;
    અચલ cff_subset_accelerator_t* cff_accelerator = nullptr;
    જો (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_ગણતરી_2 = cff_accelerator->parsed_local_subrs.length;
    }

    જો (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->અંક_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } નહીંતર {
      parsed_charstrings.resize_exact (plan->અંક_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->ગણતરી);

      જો (unlikely (!parsed_local_subrs_storage.resize (fd_ગણતરી_2))) પાછા ખોટું;

      માટે (unsigned પૂર્ણાંક i = 0; i < acc.fd_ગણતરી; i++)
      {
        unsigned ગણતરી = acc.privateDicts[i].localSubrs->ગણતરી;
        parsed_local_subrs_storage[i].resize (ગણતરી);
        જો (unlikely (parsed_local_subrs_storage[i].in_error ())) પાછા ખોટું;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    જો (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      પાછા ખોટું;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    તાર્કિક coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    માટે (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      અચલ hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned પૂર્ણાંક fd = subset_get_fd (acc, old_glyph);
      જો (unlikely (fd >= acc.fd_ગણતરી))
        પાછા ખોટું;

      જો (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        જો (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        નહીંતર
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        ચાલુ;
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

      જો (unlikely (!interp.interpret (param)))
        પાછા ખોટું;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      જો ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	જો (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  જો (drop.vsindex_dropped)
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
      જો (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    જો ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      પાછા ખોટું;

    remaps.create (closures);

    populate_subset_accelerator ();
    પાછા સાચું;
  }

  તાર્કિક encode_charstrings (str_buff_vec_t &buffArray, તાર્કિક encode_prefix = સાચું) અચલ
  {
    unsigned અંક_glyphs = plan->અંક_output_glyphs ();
    જો (unlikely (!buffArray.resize_exact (અંક_glyphs)))
      પાછા ખોટું;
    hb_codepoint_t last = 0;
    માટે (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      જો (endchar_op != OpCode_Invalid)
        માટે (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned અક્ષર *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned પૂર્ણાંક  fd = subset_get_fd (acc, old_glyph);
      જો (unlikely (fd >= acc.fd_ગણતરી))
	પાછા ખોટું;
      જો (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	પાછા ખોટું;
    }
    જો (endchar_op != OpCode_Invalid)
      માટે (; last < અંક_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned અક્ષર *>(endchar_str), 1);
      }

    પાછા સાચું;
  }

  તાર્કિક encode_subrs (અચલ parsed_cs_str_vec_t &subrs, અચલ subr_remap_t& remap, unsigned પૂર્ણાંક fd, str_buff_vec_t &buffArray) અચલ
  {
    unsigned પૂર્ણાંક  ગણતરી = remap.get_population ();

    જો (unlikely (!buffArray.resize_exact (ગણતરી)))
      પાછા ખોટું;
    માટે (unsigned પૂર્ણાંક new_અંક = 0; new_અંક < ગણતરી; new_અંક++)
    {
      hb_codepoint_t old_અંક = remap.backward (new_અંક);
      પ્રતિજ્ઞા (old_અંક != CFF_UNDEF_CODE);

      જો (unlikely (!encode_str (subrs[old_અંક], fd, buffArray[new_અંક])))
	પાછા ખોટું;
    }
    પાછા સાચું;
  }

  તાર્કિક encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    પાછા encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  તાર્કિક encode_localsubrs (unsigned પૂર્ણાંક fd, str_buff_vec_t &buffArray) અચલ
  {
    પાછા encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  સુરક્ષિત:
  સંરચના drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (ખોટું),
	ends_in_hint (ખોટું),
	all_dropped (ખોટું),
	vsindex_dropped (ખોટું) {}

    તાર્કિક  seen_moveto;
    તાર્કિક  ends_in_hint;
    તાર્કિક  all_dropped;
    તાર્કિક  vsindex_dropped;
  };

  તાર્કિક drop_hints_in_subr (parsed_cs_str_t &str, unsigned પૂર્ણાંક pos,
			   parsed_cs_str_vec_t &subrs, unsigned પૂર્ણાંક subr_અંક_2,
			   અચલ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = ખોટું;
    તાર્કિક has_hint = drop_hints_in_str (subrs[subr_અંક_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    જો (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      જો (!str.at_end (pos))
	drop.ends_in_hint = ખોટું;
    }
    નહીંતર જો (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    પાછા has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  તાર્કિક drop_hints_in_str (parsed_cs_str_t &str, અચલ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    તાર્કિક  seen_hint = ખોટું;

    unsigned ગણતરી = str.values.length;
    auto *values = str.values.arrayZ;
    માટે (unsigned પૂર્ણાંક pos = 0; pos < ગણતરી; pos++)
    {
      તાર્કિક  has_hint = ખોટું;
      પસંદગી (values[pos].op)
      {
	સ્થિતિ OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_અંક_2,
					param, drop);
	  તોડો;

	સ્થિતિ OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_અંક_2,
					param, drop);
	  તોડો;

	સ્થિતિ OpCode_rmoveto:
	સ્થિતિ OpCode_hmoveto:
	સ્થિતિ OpCode_vmoveto:
	  drop.seen_moveto = સાચું;
	  તોડો;

	સ્થિતિ OpCode_hintmask:
	સ્થિતિ OpCode_cntrmask:
	  જો (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    તોડો;
	  }
	  HB_FALLTHROUGH;

	સ્થિતિ OpCode_hstemhm:
	સ્થિતિ OpCode_vstemhm:
	સ્થિતિ OpCode_hstem:
	સ્થિતિ OpCode_vstem:
	  has_hint = સાચું;
	  values[pos].set_hinting ();
	  જો (str.at_end (pos))
	    drop.ends_in_hint = સાચું;
	  તોડો;

	સ્થિતિ OpCode_dotsection:
	  values[pos].set_hinting ();
	  તોડો;

	મૂળભૂત:
	  /* NONE */
	  તોડો;
      }
      જો (has_hint)
      {
	માટે (પૂર્ણાંક i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  જો (csop.is_hinting ())
	    તોડો;
	  csop.set_hinting ();
	  જો (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = સાચું;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = સાચું;
    માટે (unsigned પૂર્ણાંક pos = 0; pos < ગણતરી; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      જો (csop.op == OpCode_return)
	તોડો;
      જો (!csop.is_hinting ())
      {
	drop.all_dropped = ખોટું;
	તોડો;
      }
    }

    પાછા seen_hint;
  }

  તાર્કિક closure_subroutines (અચલ parsed_cs_str_vec_t& global_subrs,
                            અચલ hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    માટે (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned પૂર્ણાંક fd = subset_get_fd (acc, old_glyph);
      જો (unlikely (fd >= acc.fd_ગણતરી))
        પાછા ખોટું;

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

    પાછા સાચું;
  }

  શૂન્ય collect_subr_refs_in_subr (unsigned પૂર્ણાંક subr_અંક_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  અચલ subr_subset_param_t &param)
  {
    જો (closure->has (subr_અંક_2))
      પાછા;
    closure->add (subr_અંક_2);
    collect_subr_refs_in_str (subrs[subr_અંક_2], param);
  }

  શૂન્ય collect_subr_refs_in_str (અચલ parsed_cs_str_t &str,
                                 અચલ subr_subset_param_t &param)
  {
    જો (!str.has_calls ())
      પાછા;

    માટે (auto &opstr : str.values)
    {
      જો (!param.drop_hints || !opstr.is_hinting ())
      {
	પસંદગી (opstr.op)
	{
	  સ્થિતિ OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_અંક_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    તોડો;

	  સ્થિતિ OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_અંક_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    તોડો;

	  મૂળભૂત: તોડો;
	}
      }
    }
  }

  તાર્કિક encode_str (અચલ parsed_cs_str_t &str, અચલ unsigned પૂર્ણાંક fd, str_buff_t &buff, તાર્કિક encode_prefix = સાચું) અચલ
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    તાર્કિક hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    જો (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_અંક_cs (str.prefix_અંક_2 ());
      જો (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    માટે (auto &opstr : str.values)
    {
      size += opstr.length;
      જો (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    જો (!buff.alloc_exact (buff.length + size))
      પાછા ખોટું;

    માટે (auto &opstr : str.values)
    {
      જો (hinting || !opstr.is_hinting ())
      {
	પસંદગી (opstr.op)
	{
	  સ્થિતિ OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_અંક (opstr.subr_અંક_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    તોડો;

	  સ્થિતિ OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_અંક (opstr.subr_અંક_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    તોડો;

	  મૂળભૂત:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    તોડો;
	}
      }
    }
    પાછા !encoder.in_error ();
  }

  શૂન્ય compact_parsed_subrs () અચલ
  {
    માટે (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    માટે (auto &vec : parsed_local_subrs_storage)
      માટે (auto &cs : vec)
	cs.compact ();
  }

  શૂન્ય populate_subset_accelerator () અચલ
  {
    જો (!plan->inprogress_accelerator) પાછા;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  અચલ parsed_cs_str_t& get_parsed_charstring (unsigned i) અચલ
  {
    જો (cached_charstrings) પાછા *(cached_charstrings[i]);
    પાછા parsed_charstrings[i];
  }

  સુરક્ષિત:
  અચલ ACC			&acc;
  અચલ hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<અચલ parsed_cs_str_t*>     cached_charstrings;
  અચલ parsed_cs_str_vec_t*              parsed_global_subrs;
  અચલ hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  ખાનગી:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  પ્રકારવ્યાખ્યા typename SUBRS::ગણતરી_type subr_ગણતરી_type;
};

} /* namespace CFF */

HB_INTERNAL તાર્કિક
hb_plan_subset_cff_fdselect (અચલ hb_subset_plan_t *plan,
			    unsigned પૂર્ણાંક fd_ગણતરી,
			    અચલ CFF::FDSelect &src, /* IN */
			    unsigned પૂર્ણાંક src_અંક_glyphs,
			    unsigned પૂર્ણાંક &subset_fd_ગણતરી /* OUT */,
			    unsigned પૂર્ણાંક &subset_fdselect_size /* OUT */,
			    unsigned પૂર્ણાંક &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL તાર્કિક
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned પૂર્ણાંક અંક_glyphs,
			  અચલ CFF::FDSelect &src,
			  unsigned પૂર્ણાંક fd_ગણતરી_2,
			  unsigned પૂર્ણાંક fdselect_format,
			  unsigned પૂર્ણાંક size,
			  અચલ hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
