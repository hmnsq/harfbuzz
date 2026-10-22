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
ढाँचा <typename ACC>
स्थिर अन्तःस्थ अचिन्हित
subset_get_fd (अचर ACC &acc, hb_codepoint_t glyph)
{
  फर्काऊ likely (glyph < acc.अङ्क_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
संरचना str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  शून्य reset () { buff.reset (); }

  शून्य encode_byte (अचिन्हित वर्ण b)
  {
    यदि (likely ((चिन्हित) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    अन्यथा
      buff.push (b);
  }

  शून्य encode_int (पूर्णाङ्क v)
  {
    यदि ((-1131 <= v) && (v <= 1131))
    {
      यदि ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      अन्यथा यदि (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      अन्यथा
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    अन्यथा
    {
      यदि (unlikely (v < -32768))
	v = -32768;
      अन्यथा यदि (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  शून्य encode_अङ्क_cs (अचर सङ्ख्या_t& n)
  {
    यदि (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    अन्यथा
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
  शून्य encode_अङ्क_tp (अचर सङ्ख्या_t& n)
  {
    यदि (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    अन्यथा
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
      माप_प्रकार len;
      वर्ण *comma = म_अक्षर (s, ',');
      यदि (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      यदि (s[0] == '0' && s[1] == '.')
	s++;
      अन्यथा यदि (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      अन्यथा यदि ((len = म_माप (s)) > 3 && !म_भेद (s + len - 3, "000"))
      {
	अचिन्हित exponent = len - 3;
	वर्ण *s2 = s + exponent - 1;
	जबसम्म (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, परिमाण (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      अन्यथा
      {
	वर्ण *dot = म_अक्षर (s, '.');
	वर्ण *e = म_अक्षर (s, 'E');
	यदि (dot && e)
	{
	  स_हटाओ (dot, dot + 1, e - (dot + 1));
	  पूर्णाङ्क exponent = म_से_प (e + 1);
	  पूर्णाङ्क new_exponent = exponent - (e - (dot + 1));
	  यदि (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  अन्यथा
	    snprintf (e - 1, परिमाण (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      यदि ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	पूर्णाङ्क sign = s[0] == '-';
	वर्ण *s2 = s + sign + 1;
	जबसम्म (*s2 == '0')
	  s2++;
	len = म_माप (s2);
	स_हटाओ (s + sign, s2, len);
	snprintf (s + sign + len, परिमाण (buf) - (s + sign + len - buf), "E-%u", (अचिन्हित) (म_माप (s + sign) - 1));
      }
      hb_vector_t<वर्ण> nibbles;
      जबसम्म (*s)
      {
	वर्ण c = s[0];
	s++;

	विकल्प (c)
	{
	  पक्ष 'E':
	  {
	    वर्ण c2 = *s;
	    यदि (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } अन्यथा {
	      यदि (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    यदि (*s == '0')
	      s++;
	    जारी;
	  }

	  पक्ष '.':
	    nibbles.push (0x0A); // .
	    जारी;

	  पक्ष '-':
	    nibbles.push (0x0E); // -
	    जारी;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      यदि (nibbles.length % 2)
	nibbles.push (0x0F);

      अचिन्हित गन्ती = nibbles.length;
      लागि (अचिन्हित i = 0; i < गन्ती; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  शून्य encode_op (op_code_t op)
  {
    यदि (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    अन्यथा
      encode_byte (op);
  }

  शून्य copy_str (अचर अचिन्हित वर्ण *str, अचिन्हित length)
  {
    प्रतिज्ञा ((चिन्हित) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  तार्किक in_error () अचर { फर्काऊ buff.in_error (); }

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

ढाँचा <typename OPSTR=op_str_t>
संरचना cff_शीर्ष_dict_op_serializer_t : op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  अचर OPSTR &opstr,
		  अचर cff_sub_table_info_t &info) अचर
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
    return_trace (सत्य);
  }
};

संरचना cff_font_dict_op_serializer_t : op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  अचर op_str_t &opstr,
		  अचर table_info_t &privateDictInfo) अचर
  {
    TRACE_SERIALIZE (एतत्);

    यदि (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    अन्यथा
    {
      अचिन्हित वर्ण *d = c->allocate_size<अचिन्हित वर्ण> (opstr.length);
      यदि (unlikely (!d)) return_trace (असत्य);
      /* Faster than hb_memcpy for small strings. */
      लागि (अचिन्हित i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (सत्य);
  }
};

/* CharString command for specialization */
संरचना cs_command_t
{
  hb_vector_t<सङ्ख्या_t> args;
  hb_vector_t<अचिन्हित वर्ण> mask_bytes; /* For hintmask/cntrmask payload bytes. */
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
                   अचर hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  तार्किक	drop_hints;
  अचर hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  अचर cff2_instancing_plan_t *instancer = nullptr;
  तार्किक emitted_blend = असत्य;
};

ढाँचा <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_flattener_t
{
  subr_flattener_t (अचर ACC &acc_,
		    अचर hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  तार्किक flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    अचिन्हित गन्ती = plan->अङ्क_output_glyphs ();
    यदि (!flat_charstrings.resize_exact (गन्ती))
      फर्काऊ असत्य;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      hb_codepoint_t  glyph;
      यदि (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	यदि (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	जारी;
      }
      अचर hb_ubytes_t str = (*acc.charStrings)[glyph];
      अचिन्हित पूर्णाङ्क fd = subset_get_fd (acc, glyph);
      यदि (unlikely (fd >= acc.fd_गन्ती))
	फर्काऊ असत्य;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (तार्किक) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      यदि (unlikely (!interp.interpret (param)))
	फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
  }

  अचर ACC &acc;
  अचर hb_subset_plan_t *plan;
};

संरचना subr_closures_t
{
  subr_closures_t (अचिन्हित पूर्णाङ्क fd_गन्ती_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_गन्ती_2);
  }

  शून्य reset ()
  {
    global_closure.clear();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  तार्किक in_error () अचर { फर्काऊ local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

संरचना parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (अचिन्हित पूर्णाङ्क subr_अङ्क = 0) :
    subr_अङ्क_2 (subr_अङ्क) {}

  तार्किक is_hinting () अचर { फर्काऊ hinting_flag; }
  शून्य set_hinting ()       { hinting_flag = सत्य; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  रक्षित:
  तार्किक	  hinting_flag = असत्य;

  सार्वजनिक:
  uint16_t subr_अङ्क_2;
};

संरचना parsed_cs_str_t : parsed_मानहरू_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (असत्य),
    hint_dropped (असत्य),
    has_prefix_ (असत्य),
    has_calls_ (असत्य),
    coalescing_ (असत्य)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  शून्य add_op (op_code_t op, अचर byte_str_ref_t& str_ref)
  {
    यदि (is_parsed ()) फर्काऊ;
    यदि (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      यदि (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      फर्काऊ;
    }
    SUPER::add_op (op, str_ref);
  }

  शून्य add_call_op (op_code_t op, अचर byte_str_ref_t& str_ref, अचिन्हित पूर्णाङ्क subr_अङ्क_2)
  {
    यदि (is_parsed ()) फर्काऊ;
    has_calls_ = सत्य;

    यदि (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_अङ्क_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      फर्काऊ;
    }

    /* Pop the subroutine number. */
    values.झिक ();

    SUPER::add_op (op, str_ref, {subr_अङ्क_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  शून्य flush_segment (अचर byte_str_ref_t& str_ref, अचिन्हित end)
  {
    अचिन्हित start = opStart;
    यदि (end <= start) फर्काऊ;
    जबसम्म (start < end)
    {
      स्वतः arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      यदि (unlikely (!arr.length)) रोक;
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
  शून्य flush_coalesced (अचर byte_str_ref_t& str_ref)
  {
    यदि (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  शून्य enable_coalescing () { coalescing_ = सत्य; }
  तार्किक is_coalescing () अचर { फर्काऊ coalescing_; }

  शून्य set_prefix (अचर सङ्ख्या_t &अङ्क, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = सत्य;
    prefix_op_ = op;
    prefix_अङ्क = अङ्क;
  }

  तार्किक at_end (अचिन्हित पूर्णाङ्क pos) अचर
  {
    फर्काऊ ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  तार्किक is_parsed () अचर { फर्काऊ parsed; }
  शून्य set_parsed ()      { parsed = सत्य; }

  तार्किक is_hint_dropped () अचर { फर्काऊ hint_dropped; }
  शून्य set_hint_dropped ()      { hint_dropped = सत्य; }

  तार्किक is_vsindex_dropped () अचर { फर्काऊ vsindex_dropped; }
  शून्य set_vsindex_dropped ()      { vsindex_dropped = सत्य; }

  तार्किक has_prefix () अचर          { फर्काऊ has_prefix_; }
  op_code_t prefix_op () अचर         { फर्काऊ prefix_op_; }
  अचर सङ्ख्या_t &prefix_अङ्क_2 () अचर { फर्काऊ prefix_अङ्क; }

  तार्किक has_calls () अचर          { फर्काऊ has_calls_; }

  शून्य compact ()
  {
    अचिन्हित गन्ती = values.length;
    यदि (!गन्ती) फर्काऊ;
    स्वतः &opstr = values.arrayZ;
    अचिन्हित j = 0;
    लागि (अचिन्हित i = 1; i < गन्ती; i++)
    {
      /* See if we can combine op j and op i. */
      तार्किक combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      यदि (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      अन्यथा
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
  अचिन्हित penultimate_end_ = 0;
  अचिन्हित last_end_ = 0;
  op_code_t	prefix_op_;
  सङ्ख्या_t	prefix_अङ्क;

  गुप्त:
  प्रकारनिर्देश parsed_मानहरू_t<parsed_cs_op_t> SUPER;
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
      अचर parsed_cs_str_vec_t& parsed_charstrings,
      अचर parsed_cs_str_vec_t& parsed_global_subrs,
      अचर hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (परिमाण(cff_subset_accelerator_t));
    यदि (unlikely (!accel)) फर्काऊ nullptr;
    नूतन (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    फर्काऊ accel;
  }

  स्थिर शून्य destroy (शून्य* value) {
    यदि (!value) फर्काऊ;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      अचर parsed_cs_str_vec_t& parsed_charstrings_,
      अचर parsed_cs_str_vec_t& parsed_global_subrs_,
      अचर hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    स्वतः *mapping = glyph_to_sid_map.get_relaxed ();
    यदि (mapping)
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
		       तार्किक coalesce_ = असत्य) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    यदि (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    विकल्प (context.type)
    {
      पक्ष CSType_CharString:
	फर्काऊ parsed_charstring;

      पक्ष CSType_LocalSubr:
	यदि (likely (context.subr_अङ्क_2 < parsed_local_subrs->length))
	  फर्काऊ &(*parsed_local_subrs)[context.subr_अङ्क_2];
	रोक;

      पक्ष CSType_GlobalSubr:
	यदि (likely (context.subr_अङ्क_2 < parsed_global_subrs->length))
	  फर्काऊ &(*parsed_global_subrs)[context.subr_अङ्क_2];
	रोक;
    }
    फर्काऊ nullptr;
  }

  ढाँचा <typename ENV>
  शून्य set_current_str (ENV &env, तार्किक calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    यदि (unlikely (!parsed_str))
    {
      env.set_error ();
      फर्काऊ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    यदि (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    अन्यथा
    {
      यदि (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        यदि (coalesce) parsed_str->enable_coalescing ();
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
  शून्य create (अचर hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    लागि (स्वतः old_अङ्क : *closure)
      add (old_अङ्क);

    यदि (get_population () < 1240)
      bias = 107;
    अन्यथा यदि (get_population () < 33900)
      bias = 1131;
    अन्यथा
      bias = 32768;
  }

  पूर्णाङ्क biased_अङ्क (अचिन्हित पूर्णाङ्क old_अङ्क) अचर
  {
    hb_codepoint_t new_अङ्क = get (old_अङ्क);
    फर्काऊ (पूर्णाङ्क)new_अङ्क - bias;
  }

  रक्षित:
  पूर्णाङ्क bias;
};

संरचना subr_remaps_t
{
  subr_remaps_t (अचिन्हित पूर्णाङ्क fd_गन्ती)
  {
    local_remaps.resize (fd_गन्ती);
  }

  तार्किक in_error()
  {
    फर्काऊ local_remaps.in_error ();
  }

  शून्य create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

ढाँचा <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, अचर hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_गन्ती),
        remaps(acc_.fd_गन्ती)
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
    अचिन्हित fd_गन्ती_2 = acc.fd_गन्ती;
    अचर cff_subset_accelerator_t* cff_accelerator = nullptr;
    यदि (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_गन्ती_2 = cff_accelerator->parsed_local_subrs.length;
    }

    यदि (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->अङ्क_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } अन्यथा {
      parsed_charstrings.resize_exact (plan->अङ्क_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->गन्ती);

      यदि (unlikely (!parsed_local_subrs_storage.resize (fd_गन्ती_2))) फर्काऊ असत्य;

      लागि (अचिन्हित पूर्णाङ्क i = 0; i < acc.fd_गन्ती; i++)
      {
        अचिन्हित गन्ती = acc.privateDicts[i].localSubrs->गन्ती;
        parsed_local_subrs_storage[i].resize (गन्ती);
        यदि (unlikely (parsed_local_subrs_storage[i].in_error ())) फर्काऊ असत्य;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    यदि (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      फर्काऊ असत्य;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    तार्किक coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    लागि (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      अचर hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      अचिन्हित पूर्णाङ्क fd = subset_get_fd (acc, old_glyph);
      यदि (unlikely (fd >= acc.fd_गन्ती))
        फर्काऊ असत्य;

      यदि (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        यदि (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        अन्यथा
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        जारी;
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

      यदि (unlikely (!interp.interpret (param)))
        फर्काऊ असत्य;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      यदि ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	यदि (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  यदि (drop.vsindex_dropped)
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
      यदि (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    यदि ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      फर्काऊ असत्य;

    remaps.create (closures);

    populate_subset_accelerator ();
    फर्काऊ सत्य;
  }

  तार्किक encode_charstrings (str_buff_vec_t &buffArray, तार्किक encode_prefix = सत्य) अचर
  {
    अचिन्हित अङ्क_glyphs = plan->अङ्क_output_glyphs ();
    यदि (unlikely (!buffArray.resize_exact (अङ्क_glyphs)))
      फर्काऊ असत्य;
    hb_codepoint_t last = 0;
    लागि (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      यदि (endchar_op != OpCode_Invalid)
        लागि (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  स्वतः &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<अचिन्हित वर्ण *>(endchar_str), 1);
	}

      last++; // Skip over gid
      अचिन्हित पूर्णाङ्क  fd = subset_get_fd (acc, old_glyph);
      यदि (unlikely (fd >= acc.fd_गन्ती))
	फर्काऊ असत्य;
      यदि (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	फर्काऊ असत्य;
    }
    यदि (endchar_op != OpCode_Invalid)
      लागि (; last < अङ्क_glyphs; last++)
      {
	// Hack to point vector to static string.
	स्वतः &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<अचिन्हित वर्ण *>(endchar_str), 1);
      }

    फर्काऊ सत्य;
  }

  तार्किक encode_subrs (अचर parsed_cs_str_vec_t &subrs, अचर subr_remap_t& remap, अचिन्हित पूर्णाङ्क fd, str_buff_vec_t &buffArray) अचर
  {
    अचिन्हित पूर्णाङ्क  गन्ती = remap.get_population ();

    यदि (unlikely (!buffArray.resize_exact (गन्ती)))
      फर्काऊ असत्य;
    लागि (अचिन्हित पूर्णाङ्क new_अङ्क = 0; new_अङ्क < गन्ती; new_अङ्क++)
    {
      hb_codepoint_t old_अङ्क = remap.backward (new_अङ्क);
      प्रतिज्ञा (old_अङ्क != CFF_UNDEF_CODE);

      यदि (unlikely (!encode_str (subrs[old_अङ्क], fd, buffArray[new_अङ्क])))
	फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
  }

  तार्किक encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    फर्काऊ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  तार्किक encode_localsubrs (अचिन्हित पूर्णाङ्क fd, str_buff_vec_t &buffArray) अचर
  {
    फर्काऊ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  रक्षित:
  संरचना drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (असत्य),
	ends_in_hint (असत्य),
	all_dropped (असत्य),
	vsindex_dropped (असत्य) {}

    तार्किक  seen_moveto;
    तार्किक  ends_in_hint;
    तार्किक  all_dropped;
    तार्किक  vsindex_dropped;
  };

  तार्किक drop_hints_in_subr (parsed_cs_str_t &str, अचिन्हित पूर्णाङ्क pos,
			   parsed_cs_str_vec_t &subrs, अचिन्हित पूर्णाङ्क subr_अङ्क_2,
			   अचर subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = असत्य;
    तार्किक has_hint = drop_hints_in_str (subrs[subr_अङ्क_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    यदि (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      यदि (!str.at_end (pos))
	drop.ends_in_hint = असत्य;
    }
    अन्यथा यदि (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    फर्काऊ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  तार्किक drop_hints_in_str (parsed_cs_str_t &str, अचर subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    तार्किक  seen_hint = असत्य;

    अचिन्हित गन्ती = str.values.length;
    स्वतः *values = str.values.arrayZ;
    लागि (अचिन्हित पूर्णाङ्क pos = 0; pos < गन्ती; pos++)
    {
      तार्किक  has_hint = असत्य;
      विकल्प (values[pos].op)
      {
	पक्ष OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_अङ्क_2,
					param, drop);
	  रोक;

	पक्ष OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_अङ्क_2,
					param, drop);
	  रोक;

	पक्ष OpCode_rmoveto:
	पक्ष OpCode_hmoveto:
	पक्ष OpCode_vmoveto:
	  drop.seen_moveto = सत्य;
	  रोक;

	पक्ष OpCode_hintmask:
	पक्ष OpCode_cntrmask:
	  यदि (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    रोक;
	  }
	  HB_FALLTHROUGH;

	पक्ष OpCode_hstemhm:
	पक्ष OpCode_vstemhm:
	पक्ष OpCode_hstem:
	पक्ष OpCode_vstem:
	  has_hint = सत्य;
	  values[pos].set_hinting ();
	  यदि (str.at_end (pos))
	    drop.ends_in_hint = सत्य;
	  रोक;

	पक्ष OpCode_dotsection:
	  values[pos].set_hinting ();
	  रोक;

	स्वभाव:
	  /* NONE */
	  रोक;
      }
      यदि (has_hint)
      {
	लागि (पूर्णाङ्क i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(अचिन्हित)i];
	  यदि (csop.is_hinting ())
	    रोक;
	  csop.set_hinting ();
	  यदि (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = सत्य;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = सत्य;
    लागि (अचिन्हित पूर्णाङ्क pos = 0; pos < गन्ती; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      यदि (csop.op == OpCode_return)
	रोक;
      यदि (!csop.is_hinting ())
      {
	drop.all_dropped = असत्य;
	रोक;
      }
    }

    फर्काऊ seen_hint;
  }

  तार्किक closure_subroutines (अचर parsed_cs_str_vec_t& global_subrs,
                            अचर hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    लागि (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      अचिन्हित पूर्णाङ्क fd = subset_get_fd (acc, old_glyph);
      यदि (unlikely (fd >= acc.fd_गन्ती))
        फर्काऊ असत्य;

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

    फर्काऊ सत्य;
  }

  शून्य collect_subr_refs_in_subr (अचिन्हित पूर्णाङ्क subr_अङ्क_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  अचर subr_subset_param_t &param)
  {
    यदि (closure->has (subr_अङ्क_2))
      फर्काऊ;
    closure->add (subr_अङ्क_2);
    collect_subr_refs_in_str (subrs[subr_अङ्क_2], param);
  }

  शून्य collect_subr_refs_in_str (अचर parsed_cs_str_t &str,
                                 अचर subr_subset_param_t &param)
  {
    यदि (!str.has_calls ())
      फर्काऊ;

    लागि (स्वतः &opstr : str.values)
    {
      यदि (!param.drop_hints || !opstr.is_hinting ())
      {
	विकल्प (opstr.op)
	{
	  पक्ष OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_अङ्क_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    रोक;

	  पक्ष OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_अङ्क_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    रोक;

	  स्वभाव: रोक;
	}
      }
    }
  }

  तार्किक encode_str (अचर parsed_cs_str_t &str, अचर अचिन्हित पूर्णाङ्क fd, str_buff_t &buff, तार्किक encode_prefix = सत्य) अचर
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    तार्किक hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    यदि (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_अङ्क_cs (str.prefix_अङ्क_2 ());
      यदि (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    अचिन्हित size = 0;
    लागि (स्वतः &opstr : str.values)
    {
      size += opstr.length;
      यदि (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    यदि (!buff.alloc_exact (buff.length + size))
      फर्काऊ असत्य;

    लागि (स्वतः &opstr : str.values)
    {
      यदि (hinting || !opstr.is_hinting ())
      {
	विकल्प (opstr.op)
	{
	  पक्ष OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_अङ्क (opstr.subr_अङ्क_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    रोक;

	  पक्ष OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_अङ्क (opstr.subr_अङ्क_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    रोक;

	  स्वभाव:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    रोक;
	}
      }
    }
    फर्काऊ !encoder.in_error ();
  }

  शून्य compact_parsed_subrs () अचर
  {
    लागि (स्वतः &cs : parsed_global_subrs_storage)
      cs.compact ();
    लागि (स्वतः &vec : parsed_local_subrs_storage)
      लागि (स्वतः &cs : vec)
	cs.compact ();
  }

  शून्य populate_subset_accelerator () अचर
  {
    यदि (!plan->inprogress_accelerator) फर्काऊ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  अचर parsed_cs_str_t& get_parsed_charstring (अचिन्हित i) अचर
  {
    यदि (cached_charstrings) फर्काऊ *(cached_charstrings[i]);
    फर्काऊ parsed_charstrings[i];
  }

  रक्षित:
  अचर ACC			&acc;
  अचर hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<अचर parsed_cs_str_t*>     cached_charstrings;
  अचर parsed_cs_str_vec_t*              parsed_global_subrs;
  अचर hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  गुप्त:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  प्रकारनिर्देश typename SUBRS::गन्ती_type subr_गन्ती_type;
};

} /* namespace CFF */

HB_INTERNAL तार्किक
hb_plan_subset_cff_fdselect (अचर hb_subset_plan_t *plan,
			    अचिन्हित पूर्णाङ्क fd_गन्ती,
			    अचर CFF::FDSelect &src, /* IN */
			    अचिन्हित पूर्णाङ्क src_अङ्क_glyphs,
			    अचिन्हित पूर्णाङ्क &subset_fd_गन्ती /* OUT */,
			    अचिन्हित पूर्णाङ्क &subset_fdselect_size /* OUT */,
			    अचिन्हित पूर्णाङ्क &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL तार्किक
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  अचिन्हित पूर्णाङ्क अङ्क_glyphs,
			  अचर CFF::FDSelect &src,
			  अचिन्हित पूर्णाङ्क fd_गन्ती_2,
			  अचिन्हित पूर्णाङ्क fdselect_format,
			  अचिन्हित पूर्णाङ्क size,
			  अचर hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
