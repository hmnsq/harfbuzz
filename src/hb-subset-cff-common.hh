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
स्थिर ओळीत अचिन्हित
subset_get_fd (स्थिरांक ACC &acc, hb_codepoint_t glyph)
{
  परत likely (glyph < acc.अंक_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
संरचना str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  शून्य reset () { buff.reset (); }

  शून्य encode_byte (अचिन्हित अक्षर b)
  {
    जर (likely ((चिन्हित) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    अन्यथा
      buff.push (b);
  }

  शून्य encode_int (पूर्णांक v)
  {
    जर ((-1131 <= v) && (v <= 1131))
    {
      जर ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      अन्यथा जर (v > 0)
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
      जर (unlikely (v < -32768))
	v = -32768;
      अन्यथा जर (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  शून्य encode_अंक_cs (स्थिरांक संख्या_t& n)
  {
    जर (n.in_int_range ())
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
  शून्य encode_अंक_tp (स्थिरांक संख्या_t& n)
  {
    जर (n.in_int_range ())
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

      अक्षर buf[16];
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
      snprintf (buf, आकार (buf), "%.8G", v);
      (शून्य) hb_uselocale (((शून्य) freelocale (clocale), oldlocale));

      अक्षर *s = buf;
      माप_प्रकार len;
      अक्षर *comma = म_अक्षर (s, ',');
      जर (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      जर (s[0] == '0' && s[1] == '.')
	s++;
      अन्यथा जर (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      अन्यथा जर ((len = म_माप (s)) > 3 && !म_भेद (s + len - 3, "000"))
      {
	अचिन्हित exponent = len - 3;
	अक्षर *s2 = s + exponent - 1;
	पर्यंत (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, आकार (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      अन्यथा
      {
	अक्षर *dot = म_अक्षर (s, '.');
	अक्षर *e = म_अक्षर (s, 'E');
	जर (dot && e)
	{
	  स_हटाओ (dot, dot + 1, e - (dot + 1));
	  पूर्णांक exponent = म_से_प (e + 1);
	  पूर्णांक new_exponent = exponent - (e - (dot + 1));
	  जर (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  अन्यथा
	    snprintf (e - 1, आकार (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      जर ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	पूर्णांक sign = s[0] == '-';
	अक्षर *s2 = s + sign + 1;
	पर्यंत (*s2 == '0')
	  s2++;
	len = म_माप (s2);
	स_हटाओ (s + sign, s2, len);
	snprintf (s + sign + len, आकार (buf) - (s + sign + len - buf), "E-%u", (अचिन्हित) (म_माप (s + sign) - 1));
      }
      hb_vector_t<अक्षर> nibbles;
      पर्यंत (*s)
      {
	अक्षर c = s[0];
	s++;

	निवड (c)
	{
	  स्थिती 'E':
	  {
	    अक्षर c2 = *s;
	    जर (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } अन्यथा {
	      जर (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    जर (*s == '0')
	      s++;
	    सुरूठेवा;
	  }

	  स्थिती '.':
	    nibbles.push (0x0A); // .
	    सुरूठेवा;

	  स्थिती '-':
	    nibbles.push (0x0E); // -
	    सुरूठेवा;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      जर (nibbles.length % 2)
	nibbles.push (0x0F);

      अचिन्हित मोजणी = nibbles.length;
      साठी (अचिन्हित i = 0; i < मोजणी; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  शून्य encode_op (op_code_t op)
  {
    जर (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    अन्यथा
      encode_byte (op);
  }

  शून्य copy_str (स्थिरांक अचिन्हित अक्षर *str, अचिन्हित length)
  {
    प्रतिज्ञा ((चिन्हित) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  तार्किक in_error () स्थिरांक { परत buff.in_error (); }

  संरक्षित:

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
		  स्थिरांक OPSTR &opstr,
		  स्थिरांक cff_sub_table_info_t &info) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    निवड (opstr.op)
    {
      स्थिती OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      स्थिती OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      स्थिती OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      पूर्वनिर्धारित:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (सत्य);
  }
};

संरचना cff_font_dict_op_serializer_t : op_serializer_t
{
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक op_str_t &opstr,
		  स्थिरांक table_info_t &privateDictInfo) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    जर (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    अन्यथा
    {
      अचिन्हित अक्षर *d = c->allocate_size<अचिन्हित अक्षर> (opstr.length);
      जर (unlikely (!d)) return_trace (असत्य);
      /* Faster than hb_memcpy for small strings. */
      साठी (अचिन्हित i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (सत्य);
  }
};

/* CharString command for specialization */
संरचना cs_command_t
{
  hb_vector_t<संख्या_t> args;
  hb_vector_t<अचिन्हित अक्षर> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

प्रकारव्याख्या hb_vector_t<cs_command_t> *cs_command_vec_t;

संरचना cff2_instancing_plan_t;

संरचना flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   तार्किक drop_hints_,
                   स्थिरांक hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  तार्किक	drop_hints;
  स्थिरांक hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  स्थिरांक cff2_instancing_plan_t *instancer = nullptr;
  तार्किक emitted_blend = असत्य;
};

ढाँचा <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_flattener_t
{
  subr_flattener_t (स्थिरांक ACC &acc_,
		    स्थिरांक hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  तार्किक flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    अचिन्हित मोजणी = plan->अंक_output_glyphs ();
    जर (!flat_charstrings.resize_exact (मोजणी))
      परत असत्य;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      hb_codepoint_t  glyph;
      जर (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	जर (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	सुरूठेवा;
      }
      स्थिरांक hb_ubytes_t str = (*acc.charStrings)[glyph];
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, glyph);
      जर (unlikely (fd >= acc.fd_मोजणी))
	परत असत्य;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (तार्किक) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      जर (unlikely (!interp.interpret (param)))
	परत असत्य;
    }
    परत सत्य;
  }

  स्थिरांक ACC &acc;
  स्थिरांक hb_subset_plan_t *plan;
};

संरचना subr_closures_t
{
  subr_closures_t (अचिन्हित पूर्णांक fd_मोजणी_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_मोजणी_2);
  }

  शून्य reset ()
  {
    global_closure.clear();
    साठी (अचिन्हित पूर्णांक i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  तार्किक in_error () स्थिरांक { परत local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

संरचना parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (अचिन्हित पूर्णांक subr_अंक = 0) :
    subr_अंक_2 (subr_अंक) {}

  तार्किक is_hinting () स्थिरांक { परत hinting_flag; }
  शून्य set_hinting ()       { hinting_flag = सत्य; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  संरक्षित:
  तार्किक	  hinting_flag = असत्य;

  सार्वजनिक:
  uint16_t subr_अंक_2;
};

संरचना parsed_cs_str_t : parsed_मूल्ये_t<parsed_cs_op_t>
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
  शून्य add_op (op_code_t op, स्थिरांक byte_str_ref_t& str_ref)
  {
    जर (is_parsed ()) परत;
    जर (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      जर (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      परत;
    }
    SUPER::add_op (op, str_ref);
  }

  शून्य add_call_op (op_code_t op, स्थिरांक byte_str_ref_t& str_ref, अचिन्हित पूर्णांक subr_अंक_2)
  {
    जर (is_parsed ()) परत;
    has_calls_ = सत्य;

    जर (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_अंक_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      परत;
    }

    /* Pop the subroutine number. */
    values.काढा ();

    SUPER::add_op (op, str_ref, {subr_अंक_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  शून्य flush_segment (स्थिरांक byte_str_ref_t& str_ref, अचिन्हित end)
  {
    अचिन्हित start = opStart;
    जर (end <= start) परत;
    पर्यंत (start < end)
    {
      स्वतः arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      जर (unlikely (!arr.length)) थांबा;
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
  शून्य flush_coalesced (स्थिरांक byte_str_ref_t& str_ref)
  {
    जर (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  शून्य enable_coalescing () { coalescing_ = सत्य; }
  तार्किक is_coalescing () स्थिरांक { परत coalescing_; }

  शून्य set_prefix (स्थिरांक संख्या_t &अंक, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = सत्य;
    prefix_op_ = op;
    prefix_अंक = अंक;
  }

  तार्किक at_end (अचिन्हित पूर्णांक pos) स्थिरांक
  {
    परत ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  तार्किक is_parsed () स्थिरांक { परत parsed; }
  शून्य set_parsed ()      { parsed = सत्य; }

  तार्किक is_hint_dropped () स्थिरांक { परत hint_dropped; }
  शून्य set_hint_dropped ()      { hint_dropped = सत्य; }

  तार्किक is_vsindex_dropped () स्थिरांक { परत vsindex_dropped; }
  शून्य set_vsindex_dropped ()      { vsindex_dropped = सत्य; }

  तार्किक has_prefix () स्थिरांक          { परत has_prefix_; }
  op_code_t prefix_op () स्थिरांक         { परत prefix_op_; }
  स्थिरांक संख्या_t &prefix_अंक_2 () स्थिरांक { परत prefix_अंक; }

  तार्किक has_calls () स्थिरांक          { परत has_calls_; }

  शून्य compact ()
  {
    अचिन्हित मोजणी = values.length;
    जर (!मोजणी) परत;
    स्वतः &opstr = values.arrayZ;
    अचिन्हित j = 0;
    साठी (अचिन्हित i = 1; i < मोजणी; i++)
    {
      /* See if we can combine op j and op i. */
      तार्किक combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      जर (combine)
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

  संरक्षित:
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
  संख्या_t	prefix_अंक;

  खाजगी:
  प्रकारव्याख्या parsed_मूल्ये_t<parsed_cs_op_t> SUPER;
};

संरचना parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  खाजगी:
  प्रकारव्याख्या hb_vector_t<parsed_cs_str_t> SUPER;
};

संरचना cff_subset_accelerator_t
{
  स्थिर cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      स्थिरांक parsed_cs_str_vec_t& parsed_charstrings,
      स्थिरांक parsed_cs_str_vec_t& parsed_global_subrs,
      स्थिरांक hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (आकार(cff_subset_accelerator_t));
    जर (unlikely (!accel)) परत nullptr;
    नवीन (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    परत accel;
  }

  स्थिर शून्य destroy (शून्य* value) {
    जर (!value) परत;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      स्थिरांक parsed_cs_str_vec_t& parsed_charstrings_,
      स्थिरांक parsed_cs_str_vec_t& parsed_global_subrs_,
      स्थिरांक hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    जर (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 खाजगी:
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
    जर (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    निवड (context.type)
    {
      स्थिती CSType_CharString:
	परत parsed_charstring;

      स्थिती CSType_LocalSubr:
	जर (likely (context.subr_अंक_2 < parsed_local_subrs->length))
	  परत &(*parsed_local_subrs)[context.subr_अंक_2];
	थांबा;

      स्थिती CSType_GlobalSubr:
	जर (likely (context.subr_अंक_2 < parsed_global_subrs->length))
	  परत &(*parsed_global_subrs)[context.subr_अंक_2];
	थांबा;
    }
    परत nullptr;
  }

  ढाँचा <typename ENV>
  शून्य set_current_str (ENV &env, तार्किक calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    जर (unlikely (!parsed_str))
    {
      env.set_error ();
      परत;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    जर (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    अन्यथा
    {
      जर (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        जर (coalesce) parsed_str->enable_coalescing ();
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
  शून्य create (स्थिरांक hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    साठी (स्वतः old_अंक : *closure)
      add (old_अंक);

    जर (get_population () < 1240)
      bias = 107;
    अन्यथा जर (get_population () < 33900)
      bias = 1131;
    अन्यथा
      bias = 32768;
  }

  पूर्णांक biased_अंक (अचिन्हित पूर्णांक old_अंक) स्थिरांक
  {
    hb_codepoint_t new_अंक = get (old_अंक);
    परत (पूर्णांक)new_अंक - bias;
  }

  संरक्षित:
  पूर्णांक bias;
};

संरचना subr_remaps_t
{
  subr_remaps_t (अचिन्हित पूर्णांक fd_मोजणी)
  {
    local_remaps.resize (fd_मोजणी);
  }

  तार्किक in_error()
  {
    परत local_remaps.in_error ();
  }

  शून्य create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    साठी (अचिन्हित पूर्णांक i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

ढाँचा <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
संरचना subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, स्थिरांक hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_मोजणी),
        remaps(acc_.fd_मोजणी)
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
    अचिन्हित fd_मोजणी_2 = acc.fd_मोजणी;
    स्थिरांक cff_subset_accelerator_t* cff_accelerator = nullptr;
    जर (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_मोजणी_2 = cff_accelerator->parsed_local_subrs.length;
    }

    जर (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->अंक_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } अन्यथा {
      parsed_charstrings.resize_exact (plan->अंक_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->मोजणी);

      जर (unlikely (!parsed_local_subrs_storage.resize (fd_मोजणी_2))) परत असत्य;

      साठी (अचिन्हित पूर्णांक i = 0; i < acc.fd_मोजणी; i++)
      {
        अचिन्हित मोजणी = acc.privateDicts[i].localSubrs->मोजणी;
        parsed_local_subrs_storage[i].resize (मोजणी);
        जर (unlikely (parsed_local_subrs_storage[i].in_error ())) परत असत्य;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    जर (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      परत असत्य;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    तार्किक coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    साठी (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      स्थिरांक hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, old_glyph);
      जर (unlikely (fd >= acc.fd_मोजणी))
        परत असत्य;

      जर (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        जर (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        अन्यथा
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        सुरूठेवा;
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

      जर (unlikely (!interp.interpret (param)))
        परत असत्य;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      जर ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	जर (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  जर (drop.vsindex_dropped)
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
      जर (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    जर ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      परत असत्य;

    remaps.create (closures);

    populate_subset_accelerator ();
    परत सत्य;
  }

  तार्किक encode_charstrings (str_buff_vec_t &buffArray, तार्किक encode_prefix = सत्य) स्थिरांक
  {
    अचिन्हित अंक_glyphs = plan->अंक_output_glyphs ();
    जर (unlikely (!buffArray.resize_exact (अंक_glyphs)))
      परत असत्य;
    hb_codepoint_t last = 0;
    साठी (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      जर (endchar_op != OpCode_Invalid)
        साठी (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  स्वतः &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<अचिन्हित अक्षर *>(endchar_str), 1);
	}

      last++; // Skip over gid
      अचिन्हित पूर्णांक  fd = subset_get_fd (acc, old_glyph);
      जर (unlikely (fd >= acc.fd_मोजणी))
	परत असत्य;
      जर (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	परत असत्य;
    }
    जर (endchar_op != OpCode_Invalid)
      साठी (; last < अंक_glyphs; last++)
      {
	// Hack to point vector to static string.
	स्वतः &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<अचिन्हित अक्षर *>(endchar_str), 1);
      }

    परत सत्य;
  }

  तार्किक encode_subrs (स्थिरांक parsed_cs_str_vec_t &subrs, स्थिरांक subr_remap_t& remap, अचिन्हित पूर्णांक fd, str_buff_vec_t &buffArray) स्थिरांक
  {
    अचिन्हित पूर्णांक  मोजणी = remap.get_population ();

    जर (unlikely (!buffArray.resize_exact (मोजणी)))
      परत असत्य;
    साठी (अचिन्हित पूर्णांक new_अंक = 0; new_अंक < मोजणी; new_अंक++)
    {
      hb_codepoint_t old_अंक = remap.backward (new_अंक);
      प्रतिज्ञा (old_अंक != CFF_UNDEF_CODE);

      जर (unlikely (!encode_str (subrs[old_अंक], fd, buffArray[new_अंक])))
	परत असत्य;
    }
    परत सत्य;
  }

  तार्किक encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    परत encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  तार्किक encode_localsubrs (अचिन्हित पूर्णांक fd, str_buff_vec_t &buffArray) स्थिरांक
  {
    परत encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  संरक्षित:
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

  तार्किक drop_hints_in_subr (parsed_cs_str_t &str, अचिन्हित पूर्णांक pos,
			   parsed_cs_str_vec_t &subrs, अचिन्हित पूर्णांक subr_अंक_2,
			   स्थिरांक subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = असत्य;
    तार्किक has_hint = drop_hints_in_str (subrs[subr_अंक_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    जर (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      जर (!str.at_end (pos))
	drop.ends_in_hint = असत्य;
    }
    अन्यथा जर (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    परत has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  तार्किक drop_hints_in_str (parsed_cs_str_t &str, स्थिरांक subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    तार्किक  seen_hint = असत्य;

    अचिन्हित मोजणी = str.values.length;
    स्वतः *values = str.values.arrayZ;
    साठी (अचिन्हित पूर्णांक pos = 0; pos < मोजणी; pos++)
    {
      तार्किक  has_hint = असत्य;
      निवड (values[pos].op)
      {
	स्थिती OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_अंक_2,
					param, drop);
	  थांबा;

	स्थिती OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_अंक_2,
					param, drop);
	  थांबा;

	स्थिती OpCode_rmoveto:
	स्थिती OpCode_hmoveto:
	स्थिती OpCode_vmoveto:
	  drop.seen_moveto = सत्य;
	  थांबा;

	स्थिती OpCode_hintmask:
	स्थिती OpCode_cntrmask:
	  जर (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    थांबा;
	  }
	  HB_FALLTHROUGH;

	स्थिती OpCode_hstemhm:
	स्थिती OpCode_vstemhm:
	स्थिती OpCode_hstem:
	स्थिती OpCode_vstem:
	  has_hint = सत्य;
	  values[pos].set_hinting ();
	  जर (str.at_end (pos))
	    drop.ends_in_hint = सत्य;
	  थांबा;

	स्थिती OpCode_dotsection:
	  values[pos].set_hinting ();
	  थांबा;

	पूर्वनिर्धारित:
	  /* NONE */
	  थांबा;
      }
      जर (has_hint)
      {
	साठी (पूर्णांक i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(अचिन्हित)i];
	  जर (csop.is_hinting ())
	    थांबा;
	  csop.set_hinting ();
	  जर (csop.op == OpCode_vsindexcs)
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
    साठी (अचिन्हित पूर्णांक pos = 0; pos < मोजणी; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      जर (csop.op == OpCode_return)
	थांबा;
      जर (!csop.is_hinting ())
      {
	drop.all_dropped = असत्य;
	थांबा;
      }
    }

    परत seen_hint;
  }

  तार्किक closure_subroutines (स्थिरांक parsed_cs_str_vec_t& global_subrs,
                            स्थिरांक hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    साठी (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, old_glyph);
      जर (unlikely (fd >= acc.fd_मोजणी))
        परत असत्य;

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

    परत सत्य;
  }

  शून्य collect_subr_refs_in_subr (अचिन्हित पूर्णांक subr_अंक_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  स्थिरांक subr_subset_param_t &param)
  {
    जर (closure->has (subr_अंक_2))
      परत;
    closure->add (subr_अंक_2);
    collect_subr_refs_in_str (subrs[subr_अंक_2], param);
  }

  शून्य collect_subr_refs_in_str (स्थिरांक parsed_cs_str_t &str,
                                 स्थिरांक subr_subset_param_t &param)
  {
    जर (!str.has_calls ())
      परत;

    साठी (स्वतः &opstr : str.values)
    {
      जर (!param.drop_hints || !opstr.is_hinting ())
      {
	निवड (opstr.op)
	{
	  स्थिती OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_अंक_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    थांबा;

	  स्थिती OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_अंक_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    थांबा;

	  पूर्वनिर्धारित: थांबा;
	}
      }
    }
  }

  तार्किक encode_str (स्थिरांक parsed_cs_str_t &str, स्थिरांक अचिन्हित पूर्णांक fd, str_buff_t &buff, तार्किक encode_prefix = सत्य) स्थिरांक
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    तार्किक hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    जर (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_अंक_cs (str.prefix_अंक_2 ());
      जर (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    अचिन्हित size = 0;
    साठी (स्वतः &opstr : str.values)
    {
      size += opstr.length;
      जर (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    जर (!buff.alloc_exact (buff.length + size))
      परत असत्य;

    साठी (स्वतः &opstr : str.values)
    {
      जर (hinting || !opstr.is_hinting ())
      {
	निवड (opstr.op)
	{
	  स्थिती OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_अंक (opstr.subr_अंक_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    थांबा;

	  स्थिती OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_अंक (opstr.subr_अंक_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    थांबा;

	  पूर्वनिर्धारित:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    थांबा;
	}
      }
    }
    परत !encoder.in_error ();
  }

  शून्य compact_parsed_subrs () स्थिरांक
  {
    साठी (स्वतः &cs : parsed_global_subrs_storage)
      cs.compact ();
    साठी (स्वतः &vec : parsed_local_subrs_storage)
      साठी (स्वतः &cs : vec)
	cs.compact ();
  }

  शून्य populate_subset_accelerator () स्थिरांक
  {
    जर (!plan->inprogress_accelerator) परत;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  स्थिरांक parsed_cs_str_t& get_parsed_charstring (अचिन्हित i) स्थिरांक
  {
    जर (cached_charstrings) परत *(cached_charstrings[i]);
    परत parsed_charstrings[i];
  }

  संरक्षित:
  स्थिरांक ACC			&acc;
  स्थिरांक hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<स्थिरांक parsed_cs_str_t*>     cached_charstrings;
  स्थिरांक parsed_cs_str_vec_t*              parsed_global_subrs;
  स्थिरांक hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  खाजगी:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  प्रकारव्याख्या typename SUBRS::मोजणी_type subr_मोजणी_type;
};

} /* namespace CFF */

HB_INTERNAL तार्किक
hb_plan_subset_cff_fdselect (स्थिरांक hb_subset_plan_t *plan,
			    अचिन्हित पूर्णांक fd_मोजणी,
			    स्थिरांक CFF::FDSelect &src, /* IN */
			    अचिन्हित पूर्णांक src_अंक_glyphs,
			    अचिन्हित पूर्णांक &subset_fd_मोजणी /* OUT */,
			    अचिन्हित पूर्णांक &subset_fdselect_size /* OUT */,
			    अचिन्हित पूर्णांक &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL तार्किक
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  अचिन्हित पूर्णांक अंक_glyphs,
			  स्थिरांक CFF::FDSelect &src,
			  अचिन्हित पूर्णांक fd_मोजणी_2,
			  अचिन्हित पूर्णांक fdselect_format,
			  अचिन्हित पूर्णांक size,
			  स्थिरांक hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
