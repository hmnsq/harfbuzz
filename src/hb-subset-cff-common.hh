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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-subset-plan.hh"
#അന്തര്ഭാവ "hb-cff-interp-cs-common.hh"

നാമാകാശ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
സ്ഥിതം വരിയിൽ unsigned
subset_get_fd (സ്ഥിരം ACC &acc, hb_codepoint_t glyph)
{
  തിരികെ likely (glyph < acc.അക്കം_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ഘടന str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  ശൂന്യം reset () { buff.reset (); }

  ശൂന്യം encode_byte (unsigned അക്ഷരം b)
  {
    എങ്കിൽ (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    അല്ലെങ്കിൽ
      buff.push (b);
  }

  ശൂന്യം encode_int (പൂർണ്ണസംഖ്യ v)
  {
    എങ്കിൽ ((-1131 <= v) && (v <= 1131))
    {
      എങ്കിൽ ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      അല്ലെങ്കിൽ എങ്കിൽ (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      അല്ലെങ്കിൽ
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (v < -32768))
	v = -32768;
      അല്ലെങ്കിൽ എങ്കിൽ (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  ശൂന്യം encode_അക്കം_cs (സ്ഥിരം സംഖ്യ_t& n)
  {
    എങ്കിൽ (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    അല്ലെങ്കിൽ
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
  ശൂന്യം encode_അക്കം_tp (സ്ഥിരം സംഖ്യ_t& n)
  {
    എങ്കിൽ (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    അല്ലെങ്കിൽ
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      ഇരട്ട v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      അക്ഷരം buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ശൂന്യ));
      snprintf (buf, വലുപ്പം (buf), "%.8G", v);
      (ശൂന്യം) hb_uselocale (((ശൂന്യം) freelocale (clocale), oldlocale));

      അക്ഷരം *s = buf;
      size_t len;
      അക്ഷരം *comma = strchr (s, ',');
      എങ്കിൽ (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      എങ്കിൽ (s[0] == '0' && s[1] == '.')
	s++;
      അല്ലെങ്കിൽ എങ്കിൽ (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      അല്ലെങ്കിൽ എങ്കിൽ ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	അക്ഷരം *s2 = s + exponent - 1;
	ആകുമ്പോൾ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, വലുപ്പം (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      അല്ലെങ്കിൽ
      {
	അക്ഷരം *dot = strchr (s, '.');
	അക്ഷരം *e = strchr (s, 'E');
	എങ്കിൽ (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  പൂർണ്ണസംഖ്യ exponent = atoi (e + 1);
	  പൂർണ്ണസംഖ്യ new_exponent = exponent - (e - (dot + 1));
	  എങ്കിൽ (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  അല്ലെങ്കിൽ
	    snprintf (e - 1, വലുപ്പം (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      എങ്കിൽ ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	പൂർണ്ണസംഖ്യ sign = s[0] == '-';
	അക്ഷരം *s2 = s + sign + 1;
	ആകുമ്പോൾ (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, വലുപ്പം (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<അക്ഷരം> nibbles;
      ആകുമ്പോൾ (*s)
      {
	അക്ഷരം c = s[0];
	s++;

	തിരഞ്ഞെടുപ്പ് (c)
	{
	  അവസ്ഥ 'E':
	  {
	    അക്ഷരം c2 = *s;
	    എങ്കിൽ (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } അല്ലെങ്കിൽ {
	      എങ്കിൽ (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    എങ്കിൽ (*s == '0')
	      s++;
	    തുടരുക;
	  }

	  അവസ്ഥ '.':
	    nibbles.push (0x0A); // .
	    തുടരുക;

	  അവസ്ഥ '-':
	    nibbles.push (0x0E); // -
	    തുടരുക;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      എങ്കിൽ (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned എണ്ണം = nibbles.length;
      വേണ്ടി (unsigned i = 0; i < എണ്ണം; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  ശൂന്യം encode_op (op_code_t op)
  {
    എങ്കിൽ (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    അല്ലെങ്കിൽ
      encode_byte (op);
  }

  ശൂന്യം copy_str (സ്ഥിരം unsigned അക്ഷരം *str, unsigned length)
  {
    പ്രതിജ്ഞാ ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ buff.in_error (); }

  സംരക്ഷിത:

  str_buff_t &buff;
};

ഘടന cff_sub_table_info_t {
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
ഘടന cff_മുകൾ_dict_op_serializer_t : op_serializer_t
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം OPSTR &opstr,
		  സ്ഥിരം cff_sub_table_info_t &info) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    തിരഞ്ഞെടുപ്പ് (opstr.op)
    {
      അവസ്ഥ OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      അവസ്ഥ OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      അവസ്ഥ OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      സ്ഥിരസ്ഥിതി:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (സത്യം);
  }
};

ഘടന cff_font_dict_op_serializer_t : op_serializer_t
{
  തർക്കം serialize (hb_serialize_context_t *c,
		  സ്ഥിരം op_str_t &opstr,
		  സ്ഥിരം table_info_t &privateDictInfo) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    എങ്കിൽ (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    അല്ലെങ്കിൽ
    {
      unsigned അക്ഷരം *d = c->allocate_size<unsigned അക്ഷരം> (opstr.length);
      എങ്കിൽ (unlikely (!d)) return_trace (അസത്യം);
      /* Faster than hb_memcpy for small strings. */
      വേണ്ടി (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (സത്യം);
  }
};

/* CharString command for specialization */
ഘടന cs_command_t
{
  hb_vector_t<സംഖ്യ_t> args;
  hb_vector_t<unsigned അക്ഷരം> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

തരനിർവചനം hb_vector_t<cs_command_t> *cs_command_vec_t;

ഘടന cff2_instancing_plan_t;

ഘടന flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   തർക്കം drop_hints_,
                   സ്ഥിരം hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  തർക്കം	drop_hints;
  സ്ഥിരം hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  സ്ഥിരം cff2_instancing_plan_t *instancer = nullptr;
  തർക്കം emitted_blend = അസത്യം;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ഘടന subr_flattener_t
{
  subr_flattener_t (സ്ഥിരം ACC &acc_,
		    സ്ഥിരം hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  തർക്കം flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned എണ്ണം = plan->അക്കം_output_glyphs ();
    എങ്കിൽ (!flat_charstrings.resize_exact (എണ്ണം))
      തിരികെ അസത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      hb_codepoint_t  glyph;
      എങ്കിൽ (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	എങ്കിൽ (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	തുടരുക;
      }
      സ്ഥിരം hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned പൂർണ്ണസംഖ്യ fd = subset_get_fd (acc, glyph);
      എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
	തിരികെ അസത്യം;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (തർക്കം) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      എങ്കിൽ (unlikely (!interp.interpret (param)))
	തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  സ്ഥിരം ACC &acc;
  സ്ഥിരം hb_subset_plan_t *plan;
};

ഘടന subr_closures_t
{
  subr_closures_t (unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_എണ്ണം_2);
  }

  ശൂന്യം reset ()
  {
    global_closure.clear();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

ഘടന parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned പൂർണ്ണസംഖ്യ subr_അക്കം = 0) :
    subr_അക്കം_2 (subr_അക്കം) {}

  തർക്കം is_hinting () സ്ഥിരം { തിരികെ hinting_flag; }
  ശൂന്യം set_hinting ()       { hinting_flag = സത്യം; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  സംരക്ഷിത:
  തർക്കം	  hinting_flag = അസത്യം;

  പൊതു:
  uint16_t subr_അക്കം_2;
};

ഘടന parsed_cs_str_t : parsed_മൂല്യങ്ങൾ_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (അസത്യം),
    hint_dropped (അസത്യം),
    has_prefix_ (അസത്യം),
    has_calls_ (അസത്യം),
    coalescing_ (അസത്യം)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  ശൂന്യം add_op (op_code_t op, സ്ഥിരം byte_str_ref_t& str_ref)
  {
    എങ്കിൽ (is_parsed ()) തിരികെ;
    എങ്കിൽ (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      എങ്കിൽ (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      തിരികെ;
    }
    SUPER::add_op (op, str_ref);
  }

  ശൂന്യം add_call_op (op_code_t op, സ്ഥിരം byte_str_ref_t& str_ref, unsigned പൂർണ്ണസംഖ്യ subr_അക്കം_2)
  {
    എങ്കിൽ (is_parsed ()) തിരികെ;
    has_calls_ = സത്യം;

    എങ്കിൽ (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_അക്കം_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      തിരികെ;
    }

    /* Pop the subroutine number. */
    values.എടുക്കുക ();

    SUPER::add_op (op, str_ref, {subr_അക്കം_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  ശൂന്യം flush_segment (സ്ഥിരം byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    എങ്കിൽ (end <= start) തിരികെ;
    ആകുമ്പോൾ (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      എങ്കിൽ (unlikely (!arr.length)) നിർത്തുക;
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
  ശൂന്യം flush_coalesced (സ്ഥിരം byte_str_ref_t& str_ref)
  {
    എങ്കിൽ (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  ശൂന്യം enable_coalescing () { coalescing_ = സത്യം; }
  തർക്കം is_coalescing () സ്ഥിരം { തിരികെ coalescing_; }

  ശൂന്യം set_prefix (സ്ഥിരം സംഖ്യ_t &അക്കം, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = സത്യം;
    prefix_op_ = op;
    prefix_അക്കം = അക്കം;
  }

  തർക്കം at_end (unsigned പൂർണ്ണസംഖ്യ pos) സ്ഥിരം
  {
    തിരികെ ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  തർക്കം is_parsed () സ്ഥിരം { തിരികെ parsed; }
  ശൂന്യം set_parsed ()      { parsed = സത്യം; }

  തർക്കം is_hint_dropped () സ്ഥിരം { തിരികെ hint_dropped; }
  ശൂന്യം set_hint_dropped ()      { hint_dropped = സത്യം; }

  തർക്കം is_vsindex_dropped () സ്ഥിരം { തിരികെ vsindex_dropped; }
  ശൂന്യം set_vsindex_dropped ()      { vsindex_dropped = സത്യം; }

  തർക്കം has_prefix () സ്ഥിരം          { തിരികെ has_prefix_; }
  op_code_t prefix_op () സ്ഥിരം         { തിരികെ prefix_op_; }
  സ്ഥിരം സംഖ്യ_t &prefix_അക്കം_2 () സ്ഥിരം { തിരികെ prefix_അക്കം; }

  തർക്കം has_calls () സ്ഥിരം          { തിരികെ has_calls_; }

  ശൂന്യം compact ()
  {
    unsigned എണ്ണം = values.length;
    എങ്കിൽ (!എണ്ണം) തിരികെ;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    വേണ്ടി (unsigned i = 1; i < എണ്ണം; i++)
    {
      /* See if we can combine op j and op i. */
      തർക്കം combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      എങ്കിൽ (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      അല്ലെങ്കിൽ
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  സംരക്ഷിത:
  തർക്കം    parsed : 1;
  തർക്കം    hint_dropped : 1;
  തർക്കം    vsindex_dropped : 1;
  തർക്കം    has_prefix_ : 1;
  തർക്കം    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  തർക്കം    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  സംഖ്യ_t	prefix_അക്കം;

  സ്വകാര്യ:
  തരനിർവചനം parsed_മൂല്യങ്ങൾ_t<parsed_cs_op_t> SUPER;
};

ഘടന parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  സ്വകാര്യ:
  തരനിർവചനം hb_vector_t<parsed_cs_str_t> SUPER;
};

ഘടന cff_subset_accelerator_t
{
  സ്ഥിതം cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      സ്ഥിരം parsed_cs_str_vec_t& parsed_charstrings,
      സ്ഥിരം parsed_cs_str_vec_t& parsed_global_subrs,
      സ്ഥിരം hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (വലുപ്പം(cff_subset_accelerator_t));
    എങ്കിൽ (unlikely (!accel)) തിരികെ nullptr;
    പുതിയ (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    തിരികെ accel;
  }

  സ്ഥിതം ശൂന്യം destroy (ശൂന്യം* value) {
    എങ്കിൽ (!value) തിരികെ;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      സ്ഥിരം parsed_cs_str_vec_t& parsed_charstrings_,
      സ്ഥിരം parsed_cs_str_vec_t& parsed_global_subrs_,
      സ്ഥിരം hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    എങ്കിൽ (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 സ്വകാര്യ:
  hb_blob_t* original_blob;
};

ഘടന subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       തർക്കം drop_hints_,
		       തർക്കം coalesce_ = അസത്യം) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    എങ്കിൽ (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    തിരഞ്ഞെടുപ്പ് (context.type)
    {
      അവസ്ഥ CSType_CharString:
	തിരികെ parsed_charstring;

      അവസ്ഥ CSType_LocalSubr:
	എങ്കിൽ (likely (context.subr_അക്കം_2 < parsed_local_subrs->length))
	  തിരികെ &(*parsed_local_subrs)[context.subr_അക്കം_2];
	നിർത്തുക;

      അവസ്ഥ CSType_GlobalSubr:
	എങ്കിൽ (likely (context.subr_അക്കം_2 < parsed_global_subrs->length))
	  തിരികെ &(*parsed_global_subrs)[context.subr_അക്കം_2];
	നിർത്തുക;
    }
    തിരികെ nullptr;
  }

  template <typename ENV>
  ശൂന്യം set_current_str (ENV &env, തർക്കം calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    എങ്കിൽ (unlikely (!parsed_str))
    {
      env.set_error ();
      തിരികെ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    എങ്കിൽ (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        എങ്കിൽ (coalesce) parsed_str->enable_coalescing ();
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
  തർക്കം	  drop_hints;
  തർക്കം	  coalesce;
};

ഘടന subr_remap_t : hb_inc_bimap_t
{
  ശൂന്യം create (സ്ഥിരം hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    വേണ്ടി (auto old_അക്കം : *closure)
      add (old_അക്കം);

    എങ്കിൽ (get_population () < 1240)
      bias = 107;
    അല്ലെങ്കിൽ എങ്കിൽ (get_population () < 33900)
      bias = 1131;
    അല്ലെങ്കിൽ
      bias = 32768;
  }

  പൂർണ്ണസംഖ്യ biased_അക്കം (unsigned പൂർണ്ണസംഖ്യ old_അക്കം) സ്ഥിരം
  {
    hb_codepoint_t new_അക്കം = get (old_അക്കം);
    തിരികെ (പൂർണ്ണസംഖ്യ)new_അക്കം - bias;
  }

  സംരക്ഷിത:
  പൂർണ്ണസംഖ്യ bias;
};

ഘടന subr_remaps_t
{
  subr_remaps_t (unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം)
  {
    local_remaps.resize (fd_എണ്ണം);
  }

  തർക്കം in_error()
  {
    തിരികെ local_remaps.in_error ();
  }

  ശൂന്യം create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ഘടന subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, സ്ഥിരം hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_എണ്ണം),
        remaps(acc_.fd_എണ്ണം)
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
  തർക്കം subset (ശൂന്യം)
  {
    unsigned fd_എണ്ണം_2 = acc.fd_എണ്ണം;
    സ്ഥിരം cff_subset_accelerator_t* cff_accelerator = nullptr;
    എങ്കിൽ (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_എണ്ണം_2 = cff_accelerator->parsed_local_subrs.length;
    }

    എങ്കിൽ (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->അക്കം_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } അല്ലെങ്കിൽ {
      parsed_charstrings.resize_exact (plan->അക്കം_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->എണ്ണം);

      എങ്കിൽ (unlikely (!parsed_local_subrs_storage.resize (fd_എണ്ണം_2))) തിരികെ അസത്യം;

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < acc.fd_എണ്ണം; i++)
      {
        unsigned എണ്ണം = acc.privateDicts[i].localSubrs->എണ്ണം;
        parsed_local_subrs_storage[i].resize (എണ്ണം);
        എങ്കിൽ (unlikely (parsed_local_subrs_storage[i].in_error ())) തിരികെ അസത്യം;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    എങ്കിൽ (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      തിരികെ അസത്യം;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    തർക്കം coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    വേണ്ടി (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      സ്ഥിരം hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned പൂർണ്ണസംഖ്യ fd = subset_get_fd (acc, old_glyph);
      എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
        തിരികെ അസത്യം;

      എങ്കിൽ (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        എങ്കിൽ (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        അല്ലെങ്കിൽ
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        തുടരുക;
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

      എങ്കിൽ (unlikely (!interp.interpret (param)))
        തിരികെ അസത്യം;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      എങ്കിൽ ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	എങ്കിൽ (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  എങ്കിൽ (drop.vsindex_dropped)
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
      എങ്കിൽ (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    എങ്കിൽ ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      തിരികെ അസത്യം;

    remaps.create (closures);

    populate_subset_accelerator ();
    തിരികെ സത്യം;
  }

  തർക്കം encode_charstrings (str_buff_vec_t &buffArray, തർക്കം encode_prefix = സത്യം) സ്ഥിരം
  {
    unsigned അക്കം_glyphs = plan->അക്കം_output_glyphs ();
    എങ്കിൽ (unlikely (!buffArray.resize_exact (അക്കം_glyphs)))
      തിരികെ അസത്യം;
    hb_codepoint_t last = 0;
    വേണ്ടി (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      എങ്കിൽ (endchar_op != OpCode_Invalid)
        വേണ്ടി (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned അക്ഷരം *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned പൂർണ്ണസംഖ്യ  fd = subset_get_fd (acc, old_glyph);
      എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
	തിരികെ അസത്യം;
      എങ്കിൽ (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	തിരികെ അസത്യം;
    }
    എങ്കിൽ (endchar_op != OpCode_Invalid)
      വേണ്ടി (; last < അക്കം_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned അക്ഷരം *>(endchar_str), 1);
      }

    തിരികെ സത്യം;
  }

  തർക്കം encode_subrs (സ്ഥിരം parsed_cs_str_vec_t &subrs, സ്ഥിരം subr_remap_t& remap, unsigned പൂർണ്ണസംഖ്യ fd, str_buff_vec_t &buffArray) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ  എണ്ണം = remap.get_population ();

    എങ്കിൽ (unlikely (!buffArray.resize_exact (എണ്ണം)))
      തിരികെ അസത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ new_അക്കം = 0; new_അക്കം < എണ്ണം; new_അക്കം++)
    {
      hb_codepoint_t old_അക്കം = remap.backward (new_അക്കം);
      പ്രതിജ്ഞാ (old_അക്കം != CFF_UNDEF_CODE);

      എങ്കിൽ (unlikely (!encode_str (subrs[old_അക്കം], fd, buffArray[new_അക്കം])))
	തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  തർക്കം encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    തിരികെ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  തർക്കം encode_localsubrs (unsigned പൂർണ്ണസംഖ്യ fd, str_buff_vec_t &buffArray) സ്ഥിരം
  {
    തിരികെ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  സംരക്ഷിത:
  ഘടന drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (അസത്യം),
	ends_in_hint (അസത്യം),
	all_dropped (അസത്യം),
	vsindex_dropped (അസത്യം) {}

    തർക്കം  seen_moveto;
    തർക്കം  ends_in_hint;
    തർക്കം  all_dropped;
    തർക്കം  vsindex_dropped;
  };

  തർക്കം drop_hints_in_subr (parsed_cs_str_t &str, unsigned പൂർണ്ണസംഖ്യ pos,
			   parsed_cs_str_vec_t &subrs, unsigned പൂർണ്ണസംഖ്യ subr_അക്കം_2,
			   സ്ഥിരം subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = അസത്യം;
    തർക്കം has_hint = drop_hints_in_str (subrs[subr_അക്കം_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    എങ്കിൽ (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      എങ്കിൽ (!str.at_end (pos))
	drop.ends_in_hint = അസത്യം;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    തിരികെ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  തർക്കം drop_hints_in_str (parsed_cs_str_t &str, സ്ഥിരം subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    തർക്കം  seen_hint = അസത്യം;

    unsigned എണ്ണം = str.values.length;
    auto *values = str.values.arrayZ;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ pos = 0; pos < എണ്ണം; pos++)
    {
      തർക്കം  has_hint = അസത്യം;
      തിരഞ്ഞെടുപ്പ് (values[pos].op)
      {
	അവസ്ഥ OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_അക്കം_2,
					param, drop);
	  നിർത്തുക;

	അവസ്ഥ OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_അക്കം_2,
					param, drop);
	  നിർത്തുക;

	അവസ്ഥ OpCode_rmoveto:
	അവസ്ഥ OpCode_hmoveto:
	അവസ്ഥ OpCode_vmoveto:
	  drop.seen_moveto = സത്യം;
	  നിർത്തുക;

	അവസ്ഥ OpCode_hintmask:
	അവസ്ഥ OpCode_cntrmask:
	  എങ്കിൽ (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    നിർത്തുക;
	  }
	  HB_FALLTHROUGH;

	അവസ്ഥ OpCode_hstemhm:
	അവസ്ഥ OpCode_vstemhm:
	അവസ്ഥ OpCode_hstem:
	അവസ്ഥ OpCode_vstem:
	  has_hint = സത്യം;
	  values[pos].set_hinting ();
	  എങ്കിൽ (str.at_end (pos))
	    drop.ends_in_hint = സത്യം;
	  നിർത്തുക;

	അവസ്ഥ OpCode_dotsection:
	  values[pos].set_hinting ();
	  നിർത്തുക;

	സ്ഥിരസ്ഥിതി:
	  /* NONE */
	  നിർത്തുക;
      }
      എങ്കിൽ (has_hint)
      {
	വേണ്ടി (പൂർണ്ണസംഖ്യ i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  എങ്കിൽ (csop.is_hinting ())
	    നിർത്തുക;
	  csop.set_hinting ();
	  എങ്കിൽ (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = സത്യം;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = സത്യം;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ pos = 0; pos < എണ്ണം; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      എങ്കിൽ (csop.op == OpCode_return)
	നിർത്തുക;
      എങ്കിൽ (!csop.is_hinting ())
      {
	drop.all_dropped = അസത്യം;
	നിർത്തുക;
      }
    }

    തിരികെ seen_hint;
  }

  തർക്കം closure_subroutines (സ്ഥിരം parsed_cs_str_vec_t& global_subrs,
                            സ്ഥിരം hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    വേണ്ടി (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned പൂർണ്ണസംഖ്യ fd = subset_get_fd (acc, old_glyph);
      എങ്കിൽ (unlikely (fd >= acc.fd_എണ്ണം))
        തിരികെ അസത്യം;

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

    തിരികെ സത്യം;
  }

  ശൂന്യം collect_subr_refs_in_subr (unsigned പൂർണ്ണസംഖ്യ subr_അക്കം_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  സ്ഥിരം subr_subset_param_t &param)
  {
    എങ്കിൽ (closure->has (subr_അക്കം_2))
      തിരികെ;
    closure->add (subr_അക്കം_2);
    collect_subr_refs_in_str (subrs[subr_അക്കം_2], param);
  }

  ശൂന്യം collect_subr_refs_in_str (സ്ഥിരം parsed_cs_str_t &str,
                                 സ്ഥിരം subr_subset_param_t &param)
  {
    എങ്കിൽ (!str.has_calls ())
      തിരികെ;

    വേണ്ടി (auto &opstr : str.values)
    {
      എങ്കിൽ (!param.drop_hints || !opstr.is_hinting ())
      {
	തിരഞ്ഞെടുപ്പ് (opstr.op)
	{
	  അവസ്ഥ OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_അക്കം_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    നിർത്തുക;

	  അവസ്ഥ OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_അക്കം_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    നിർത്തുക;

	  സ്ഥിരസ്ഥിതി: നിർത്തുക;
	}
      }
    }
  }

  തർക്കം encode_str (സ്ഥിരം parsed_cs_str_t &str, സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ fd, str_buff_t &buff, തർക്കം encode_prefix = സത്യം) സ്ഥിരം
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    തർക്കം hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    എങ്കിൽ (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_അക്കം_cs (str.prefix_അക്കം_2 ());
      എങ്കിൽ (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    വേണ്ടി (auto &opstr : str.values)
    {
      size += opstr.length;
      എങ്കിൽ (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    എങ്കിൽ (!buff.alloc_exact (buff.length + size))
      തിരികെ അസത്യം;

    വേണ്ടി (auto &opstr : str.values)
    {
      എങ്കിൽ (hinting || !opstr.is_hinting ())
      {
	തിരഞ്ഞെടുപ്പ് (opstr.op)
	{
	  അവസ്ഥ OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_അക്കം (opstr.subr_അക്കം_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    നിർത്തുക;

	  അവസ്ഥ OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_അക്കം (opstr.subr_അക്കം_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    നിർത്തുക;

	  സ്ഥിരസ്ഥിതി:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    നിർത്തുക;
	}
      }
    }
    തിരികെ !encoder.in_error ();
  }

  ശൂന്യം compact_parsed_subrs () സ്ഥിരം
  {
    വേണ്ടി (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    വേണ്ടി (auto &vec : parsed_local_subrs_storage)
      വേണ്ടി (auto &cs : vec)
	cs.compact ();
  }

  ശൂന്യം populate_subset_accelerator () സ്ഥിരം
  {
    എങ്കിൽ (!plan->inprogress_accelerator) തിരികെ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  സ്ഥിരം parsed_cs_str_t& get_parsed_charstring (unsigned i) സ്ഥിരം
  {
    എങ്കിൽ (cached_charstrings) തിരികെ *(cached_charstrings[i]);
    തിരികെ parsed_charstrings[i];
  }

  സംരക്ഷിത:
  സ്ഥിരം ACC			&acc;
  സ്ഥിരം hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<സ്ഥിരം parsed_cs_str_t*>     cached_charstrings;
  സ്ഥിരം parsed_cs_str_vec_t*              parsed_global_subrs;
  സ്ഥിരം hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  സ്വകാര്യ:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  തരനിർവചനം typename SUBRS::എണ്ണം_type subr_എണ്ണം_type;
};

} /* namespace CFF */

HB_INTERNAL തർക്കം
hb_plan_subset_cff_fdselect (സ്ഥിരം hb_subset_plan_t *plan,
			    unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം,
			    സ്ഥിരം CFF::FDSelect &src, /* IN */
			    unsigned പൂർണ്ണസംഖ്യ src_അക്കം_glyphs,
			    unsigned പൂർണ്ണസംഖ്യ &subset_fd_എണ്ണം /* OUT */,
			    unsigned പൂർണ്ണസംഖ്യ &subset_fdselect_size /* OUT */,
			    unsigned പൂർണ്ണസംഖ്യ &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL തർക്കം
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			  സ്ഥിരം CFF::FDSelect &src,
			  unsigned പൂർണ്ണസംഖ്യ fd_എണ്ണം_2,
			  unsigned പൂർണ്ണസംഖ്യ fdselect_format,
			  unsigned പൂർണ്ണസംഖ്യ size,
			  സ്ഥിരം hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
