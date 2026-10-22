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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-subset-plan.hh"
#ಅನ್ತರ್ಭಾವ "hb-cff-interp-cs-common.hh"

ನಾಮಾಕಾಶ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
template <typename ACC>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
subset_get_fd (ಧ್ರುವ ACC &acc, hb_codepoint_t glyph)
{
  ಪ್ರತ್ಯಾಗಮ likely (glyph < acc.ಅಂಕೆ_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ಸಂರಚನಾ str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  ಶೂನ್ಯ reset () { buff.reset (); }

  ಶೂನ್ಯ encode_byte (unsigned ವರ್ಣ b)
  {
    ಯದಿ (likely ((signed) buff.length < buff.allocated))
      buff.arrayZ[buff.length++] = b;
    ಅನ್ಯಥಾ
      buff.push (b);
  }

  ಶೂನ್ಯ encode_int (ಪೂರ್ಣಾಙ್ಕ v)
  {
    ಯದಿ ((-1131 <= v) && (v <= 1131))
    {
      ಯದಿ ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      ಅನ್ಯಥಾ ಯದಿ (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + OpCode_TwoBytePosInt0);
	encode_byte (v & 0xFF);
      }
      ಅನ್ಯಥಾ
      {
	v = -v - 108;
	encode_byte ((v >> 8) + OpCode_TwoByteNegInt0);
	encode_byte (v & 0xFF);
      }
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (unlikely (v < -32768))
	v = -32768;
      ಅನ್ಯಥಾ ಯದಿ (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  ಶೂನ್ಯ encode_ಅಂಕೆ_cs (ಧ್ರುವ ಸಂಖ್ಯೆ_t& n)
  {
    ಯದಿ (n.in_int_range ())
    {
      encode_int (n.to_int ());
    }
    ಅನ್ಯಥಾ
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
  ಶೂನ್ಯ encode_ಅಂಕೆ_tp (ಧ್ರುವ ಸಂಖ್ಯೆ_t& n)
  {
    ಯದಿ (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    ಅನ್ಯಥಾ
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      ದ್ವಿಗುಣ v = n.to_real ();
      encode_byte (OpCode_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      ವರ್ಣ buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_UNUSED;
      hb_locale_t oldlocale HB_UNUSED;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ರಿಕ್ತ));
      snprintf (buf, ಪರಿಮಾಣ (buf), "%.8G", v);
      (ಶೂನ್ಯ) hb_uselocale (((ಶೂನ್ಯ) freelocale (clocale), oldlocale));

      ವರ್ಣ *s = buf;
      size_t len;
      ವರ್ಣ *comma = strchr (s, ',');
      ಯದಿ (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      ಯದಿ (s[0] == '0' && s[1] == '.')
	s++;
      ಅನ್ಯಥಾ ಯದಿ (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      ಅನ್ಯಥಾ ಯದಿ ((len = strlen (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	unsigned exponent = len - 3;
	ವರ್ಣ *s2 = s + exponent - 1;
	ಯಾವತ್ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, ಪರಿಮಾಣ (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      ಅನ್ಯಥಾ
      {
	ವರ್ಣ *dot = strchr (s, '.');
	ವರ್ಣ *e = strchr (s, 'E');
	ಯದಿ (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  ಪೂರ್ಣಾಙ್ಕ exponent = atoi (e + 1);
	  ಪೂರ್ಣಾಙ್ಕ new_exponent = exponent - (e - (dot + 1));
	  ಯದಿ (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  ಅನ್ಯಥಾ
	    snprintf (e - 1, ಪರಿಮಾಣ (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      ಯದಿ ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	ಪೂರ್ಣಾಙ್ಕ sign = s[0] == '-';
	ವರ್ಣ *s2 = s + sign + 1;
	ಯಾವತ್ (*s2 == '0')
	  s2++;
	len = strlen (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, ಪರಿಮಾಣ (buf) - (s + sign + len - buf), "E-%u", (unsigned) (strlen (s + sign) - 1));
      }
      hb_vector_t<ವರ್ಣ> nibbles;
      ಯಾವತ್ (*s)
      {
	ವರ್ಣ c = s[0];
	s++;

	ವಿಕಲ್ಪ (c)
	{
	  ಪಕ್ಷ 'E':
	  {
	    ವರ್ಣ c2 = *s;
	    ಯದಿ (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } ಅನ್ಯಥಾ {
	      ಯದಿ (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    ಯದಿ (*s == '0')
	      s++;
	    ಅನುವರ್ತ;
	  }

	  ಪಕ್ಷ '.':
	    nibbles.push (0x0A); // .
	    ಅನುವರ್ತ;

	  ಪಕ್ಷ '-':
	    nibbles.push (0x0E); // -
	    ಅನುವರ್ತ;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      ಯದಿ (nibbles.length % 2)
	nibbles.push (0x0F);

      unsigned ಎಣಿಕೆ = nibbles.length;
      ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  ಶೂನ್ಯ encode_op (op_code_t op)
  {
    ಯದಿ (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    ಅನ್ಯಥಾ
      encode_byte (op);
  }

  ಶೂನ್ಯ copy_str (ಧ್ರುವ unsigned ವರ್ಣ *str, unsigned length)
  {
    ಪ್ರತಿಜ್ಞಾ ((signed) (buff.length + length) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.length, str, length);
    buff.length += length;
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ buff.in_error (); }

  ರಕ್ಷಿತ:

  str_buff_t &buff;
};

ಸಂರಚನಾ cff_sub_table_info_t {
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
ಸಂರಚನಾ cff_ತುದಿ_dict_op_serializer_t : op_serializer_t
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ OPSTR &opstr,
		  ಧ್ರುವ cff_sub_table_info_t &info) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ವಿಕಲ್ಪ (opstr.op)
    {
      ಪಕ್ಷ OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      ಪಕ್ಷ OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      ಪಕ್ಷ OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      ಸ್ವಭಾವ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (ಸತ್ಯ);
  }
};

ಸಂರಚನಾ cff_font_dict_op_serializer_t : op_serializer_t
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
		  ಧ್ರುವ op_str_t &opstr,
		  ಧ್ರುವ table_info_t &privateDictInfo) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    ಯದಿ (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.size) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    ಅನ್ಯಥಾ
    {
      unsigned ವರ್ಣ *d = c->allocate_size<unsigned ವರ್ಣ> (opstr.length);
      ಯದಿ (unlikely (!d)) return_trace (ಅಸತ್ಯ);
      /* Faster than hb_memcpy for small strings. */
      ಪ್ರತಿ (unsigned i = 0; i < opstr.length; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (ಸತ್ಯ);
  }
};

/* CharString command for specialization */
ಸಂರಚನಾ cs_command_t
{
  hb_vector_t<ಸಂಖ್ಯೆ_t> args;
  hb_vector_t<unsigned ವರ್ಣ> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

ಪ್ರಕಾರನಿರ್ದೇಶ hb_vector_t<cs_command_t> *cs_command_vec_t;

ಸಂರಚನಾ cff2_instancing_plan_t;

ಸಂರಚನಾ flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   ತಾರ್ಕಿಕ drop_hints_,
                   ಧ್ರುವ hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  ತಾರ್ಕಿಕ	drop_hints;
  ಧ್ರುವ hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ಧ್ರುವ cff2_instancing_plan_t *instancer = nullptr;
  ತಾರ್ಕಿಕ emitted_blend = ಅಸತ್ಯ;
};

template <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ಸಂರಚನಾ subr_flattener_t
{
  subr_flattener_t (ಧ್ರುವ ACC &acc_,
		    ಧ್ರುವ hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  ತಾರ್ಕಿಕ flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    unsigned ಎಣಿಕೆ = plan->ಅಂಕೆ_output_glyphs ();
    ಯದಿ (!flat_charstrings.resize_exact (ಎಣಿಕೆ))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      hb_codepoint_t  glyph;
      ಯದಿ (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	ಯದಿ (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	ಅನುವರ್ತ;
      }
      ಧ್ರುವ hb_ubytes_t str = (*acc.charStrings)[glyph];
      unsigned ಪೂರ್ಣಾಙ್ಕ fd = subset_get_fd (acc, glyph);
      ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.length);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (ತಾರ್ಕಿಕ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      ಯದಿ (unlikely (!interp.interpret (param)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಧ್ರುವ ACC &acc;
  ಧ್ರುವ hb_subset_plan_t *plan;
};

ಸಂರಚನಾ subr_closures_t
{
  subr_closures_t (unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_ಎಣಿಕೆ_2);
  }

  ಶೂನ್ಯ reset ()
  {
    global_closure.clear();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < local_closures.length; i++)
      local_closures[i].clear();
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

ಸಂರಚನಾ parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ = 0) :
    subr_ಅಂಕೆ_2 (subr_ಅಂಕೆ) {}

  ತಾರ್ಕಿಕ is_hinting () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hinting_flag; }
  ಶೂನ್ಯ set_hinting ()       { hinting_flag = ಸತ್ಯ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  ರಕ್ಷಿತ:
  ತಾರ್ಕಿಕ	  hinting_flag = ಅಸತ್ಯ;

  ಸಾರ್ವಜನಿಕ:
  uint16_t subr_ಅಂಕೆ_2;
};

ಸಂರಚನಾ parsed_cs_str_t : parsed_ಮೌಲ್ಯಗಳು_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (ಅಸತ್ಯ),
    hint_dropped (ಅಸತ್ಯ),
    has_prefix_ (ಅಸತ್ಯ),
    has_calls_ (ಅಸತ್ಯ),
    coalescing_ (ಅಸತ್ಯ)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ add_op (op_code_t op, ಧ್ರುವ byte_str_ref_t& str_ref)
  {
    ಯದಿ (is_parsed ()) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      ಯದಿ (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      ಪ್ರತ್ಯಾಗಮ;
    }
    SUPER::add_op (op, str_ref);
  }

  ಶೂನ್ಯ add_call_op (op_code_t op, ಧ್ರುವ byte_str_ref_t& str_ref, unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ_2)
  {
    ಯದಿ (is_parsed ()) ಪ್ರತ್ಯಾಗಮ;
    has_calls_ = ಸತ್ಯ;

    ಯದಿ (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_ಅಂಕೆ_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      ಪ್ರತ್ಯಾಗಮ;
    }

    /* Pop the subroutine number. */
    values.ತೆಗೆ ();

    SUPER::add_op (op, str_ref, {subr_ಅಂಕೆ_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  ಶೂನ್ಯ flush_segment (ಧ್ರುವ byte_str_ref_t& str_ref, unsigned end)
  {
    unsigned start = opStart;
    ಯದಿ (end <= start) ಪ್ರತ್ಯಾಗಮ;
    ಯಾವತ್ (start < end)
    {
      auto arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      ಯದಿ (unlikely (!arr.length)) ಭಙ್ಗ;
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
  ಶೂನ್ಯ flush_coalesced (ಧ್ರುವ byte_str_ref_t& str_ref)
  {
    ಯದಿ (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  ಶೂನ್ಯ enable_coalescing () { coalescing_ = ಸತ್ಯ; }
  ತಾರ್ಕಿಕ is_coalescing () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ coalescing_; }

  ಶೂನ್ಯ set_prefix (ಧ್ರುವ ಸಂಖ್ಯೆ_t &ಅಂಕೆ, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = ಸತ್ಯ;
    prefix_op_ = op;
    prefix_ಅಂಕೆ = ಅಂಕೆ;
  }

  ತಾರ್ಕಿಕ at_end (unsigned ಪೂರ್ಣಾಙ್ಕ pos) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ((pos + 1 >= values.length) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  ತಾರ್ಕಿಕ is_parsed () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ parsed; }
  ಶೂನ್ಯ set_parsed ()      { parsed = ಸತ್ಯ; }

  ತಾರ್ಕಿಕ is_hint_dropped () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ hint_dropped; }
  ಶೂನ್ಯ set_hint_dropped ()      { hint_dropped = ಸತ್ಯ; }

  ತಾರ್ಕಿಕ is_vsindex_dropped () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ vsindex_dropped; }
  ಶೂನ್ಯ set_vsindex_dropped ()      { vsindex_dropped = ಸತ್ಯ; }

  ತಾರ್ಕಿಕ has_prefix () ಧ್ರುವ          { ಪ್ರತ್ಯಾಗಮ has_prefix_; }
  op_code_t prefix_op () ಧ್ರುವ         { ಪ್ರತ್ಯಾಗಮ prefix_op_; }
  ಧ್ರುವ ಸಂಖ್ಯೆ_t &prefix_ಅಂಕೆ_2 () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ prefix_ಅಂಕೆ; }

  ತಾರ್ಕಿಕ has_calls () ಧ್ರುವ          { ಪ್ರತ್ಯಾಗಮ has_calls_; }

  ಶೂನ್ಯ compact ()
  {
    unsigned ಎಣಿಕೆ = values.length;
    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ;
    auto &opstr = values.arrayZ;
    unsigned j = 0;
    ಪ್ರತಿ (unsigned i = 1; i < ಎಣಿಕೆ; i++)
    {
      /* See if we can combine op j and op i. */
      ತಾರ್ಕಿಕ combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].length == opstr[i].ptr) &&
        (opstr[j].length + opstr[i].length <= 255);

      ಯದಿ (combine)
      {
	opstr[j].length += opstr[i].length;
	opstr[j].op = OpCode_Invalid;
      }
      ಅನ್ಯಥಾ
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  ರಕ್ಷಿತ:
  ತಾರ್ಕಿಕ    parsed : 1;
  ತಾರ್ಕಿಕ    hint_dropped : 1;
  ತಾರ್ಕಿಕ    vsindex_dropped : 1;
  ತಾರ್ಕಿಕ    has_prefix_ : 1;
  ತಾರ್ಕಿಕ    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  ತಾರ್ಕಿಕ    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  unsigned penultimate_end_ = 0;
  unsigned last_end_ = 0;
  op_code_t	prefix_op_;
  ಸಂಖ್ಯೆ_t	prefix_ಅಂಕೆ;

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ parsed_ಮೌಲ್ಯಗಳು_t<parsed_cs_op_t> SUPER;
};

ಸಂರಚನಾ parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_vector_t<parsed_cs_str_t> SUPER;
};

ಸಂರಚನಾ cff_subset_accelerator_t
{
  ಸ್ಥಿರ cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ಧ್ರುವ parsed_cs_str_vec_t& parsed_charstrings,
      ಧ್ರುವ parsed_cs_str_vec_t& parsed_global_subrs,
      ಧ್ರುವ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (ಪರಿಮಾಣ(cff_subset_accelerator_t));
    ಯದಿ (unlikely (!accel)) ಪ್ರತ್ಯಾಗಮ nullptr;
    ನೂತನ (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ಪ್ರತ್ಯಾಗಮ accel;
  }

  ಸ್ಥಿರ ಶೂನ್ಯ destroy (ಶೂನ್ಯ* value) {
    ಯದಿ (!value) ಪ್ರತ್ಯಾಗಮ;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ಧ್ರುವ parsed_cs_str_vec_t& parsed_charstrings_,
      ಧ್ರುವ parsed_cs_str_vec_t& parsed_global_subrs_,
      ಧ್ರುವ hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    ಯದಿ (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 ಗುಪ್ತ:
  hb_blob_t* original_blob;
};

ಸಂರಚನಾ subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       ತಾರ್ಕಿಕ drop_hints_,
		       ತಾರ್ಕಿಕ coalesce_ = ಅಸತ್ಯ) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    ಯದಿ (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    ವಿಕಲ್ಪ (context.type)
    {
      ಪಕ್ಷ CSType_CharString:
	ಪ್ರತ್ಯಾಗಮ parsed_charstring;

      ಪಕ್ಷ CSType_LocalSubr:
	ಯದಿ (likely (context.subr_ಅಂಕೆ_2 < parsed_local_subrs->length))
	  ಪ್ರತ್ಯಾಗಮ &(*parsed_local_subrs)[context.subr_ಅಂಕೆ_2];
	ಭಙ್ಗ;

      ಪಕ್ಷ CSType_GlobalSubr:
	ಯದಿ (likely (context.subr_ಅಂಕೆ_2 < parsed_global_subrs->length))
	  ಪ್ರತ್ಯಾಗಮ &(*parsed_global_subrs)[context.subr_ಅಂಕೆ_2];
	ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  template <typename ENV>
  ಶೂನ್ಯ set_current_str (ENV &env, ತಾರ್ಕಿಕ calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    ಯದಿ (unlikely (!parsed_str))
    {
      env.set_error ();
      ಪ್ರತ್ಯಾಗಮ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    ಯದಿ (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.length > 0)))
      env.set_error ();
    ಅನ್ಯಥಾ
    {
      ಯದಿ (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.total_size ());
        ಯದಿ (coalesce) parsed_str->enable_coalescing ();
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
  ತಾರ್ಕಿಕ	  drop_hints;
  ತಾರ್ಕಿಕ	  coalesce;
};

ಸಂರಚನಾ subr_remap_t : hb_inc_bimap_t
{
  ಶೂನ್ಯ create (ಧ್ರುವ hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    ಪ್ರತಿ (auto old_ಅಂಕೆ : *closure)
      add (old_ಅಂಕೆ);

    ಯದಿ (get_population () < 1240)
      bias = 107;
    ಅನ್ಯಥಾ ಯದಿ (get_population () < 33900)
      bias = 1131;
    ಅನ್ಯಥಾ
      bias = 32768;
  }

  ಪೂರ್ಣಾಙ್ಕ biased_ಅಂಕೆ (unsigned ಪೂರ್ಣಾಙ್ಕ old_ಅಂಕೆ) ಧ್ರುವ
  {
    hb_codepoint_t new_ಅಂಕೆ = get (old_ಅಂಕೆ);
    ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)new_ಅಂಕೆ - bias;
  }

  ರಕ್ಷಿತ:
  ಪೂರ್ಣಾಙ್ಕ bias;
};

ಸಂರಚನಾ subr_remaps_t
{
  subr_remaps_t (unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ)
  {
    local_remaps.resize (fd_ಎಣಿಕೆ);
  }

  ತಾರ್ಕಿಕ in_error()
  {
    ಪ್ರತ್ಯಾಗಮ local_remaps.in_error ();
  }

  ಶೂನ್ಯ create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < local_remaps.length; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

template <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
ಸಂರಚನಾ subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ಧ್ರುವ hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_ಎಣಿಕೆ),
        remaps(acc_.fd_ಎಣಿಕೆ)
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
  ತಾರ್ಕಿಕ subset (ಶೂನ್ಯ)
  {
    unsigned fd_ಎಣಿಕೆ_2 = acc.fd_ಎಣಿಕೆ;
    ಧ್ರುವ cff_subset_accelerator_t* cff_accelerator = nullptr;
    ಯದಿ (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_ಎಣಿಕೆ_2 = cff_accelerator->parsed_local_subrs.length;
    }

    ಯದಿ (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->ಅಂಕೆ_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } ಅನ್ಯಥಾ {
      parsed_charstrings.resize_exact (plan->ಅಂಕೆ_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->ಎಣಿಕೆ);

      ಯದಿ (unlikely (!parsed_local_subrs_storage.resize (fd_ಎಣಿಕೆ_2))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < acc.fd_ಎಣಿಕೆ; i++)
      {
        unsigned ಎಣಿಕೆ = acc.privateDicts[i].localSubrs->ಎಣಿಕೆ;
        parsed_local_subrs_storage[i].resize (ಎಣಿಕೆ);
        ಯದಿ (unlikely (parsed_local_subrs_storage[i].in_error ())) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    ಯದಿ (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    ತಾರ್ಕಿಕ coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    ಪ್ರತಿ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      ಧ್ರುವ hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      unsigned ಪೂರ್ಣಾಙ್ಕ fd = subset_get_fd (acc, old_glyph);
      ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        ಯದಿ (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        ಅನ್ಯಥಾ
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        ಅನುವರ್ತ;
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

      ಯದಿ (unlikely (!interp.interpret (param)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      ಯದಿ ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	ಯದಿ (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  ಯದಿ (drop.vsindex_dropped)
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
      ಯದಿ (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    ಯದಿ ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    remaps.create (closures);

    populate_subset_accelerator ();
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ encode_charstrings (str_buff_vec_t &buffArray, ತಾರ್ಕಿಕ encode_prefix = ಸತ್ಯ) ಧ್ರುವ
  {
    unsigned ಅಂಕೆ_glyphs = plan->ಅಂಕೆ_output_glyphs ();
    ಯದಿ (unlikely (!buffArray.resize_exact (ಅಂಕೆ_glyphs)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    hb_codepoint_t last = 0;
    ಪ್ರತಿ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      ಯದಿ (endchar_op != OpCode_Invalid)
        ಪ್ರತಿ (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  auto &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<unsigned ವರ್ಣ *>(endchar_str), 1);
	}

      last++; // Skip over gid
      unsigned ಪೂರ್ಣಾಙ್ಕ  fd = subset_get_fd (acc, old_glyph);
      ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      ಯದಿ (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಯದಿ (endchar_op != OpCode_Invalid)
      ಪ್ರತಿ (; last < ಅಂಕೆ_glyphs; last++)
      {
	// Hack to point vector to static string.
	auto &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<unsigned ವರ್ಣ *>(endchar_str), 1);
      }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ encode_subrs (ಧ್ರುವ parsed_cs_str_vec_t &subrs, ಧ್ರುವ subr_remap_t& remap, unsigned ಪೂರ್ಣಾಙ್ಕ fd, str_buff_vec_t &buffArray) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ  ಎಣಿಕೆ = remap.get_population ();

    ಯದಿ (unlikely (!buffArray.resize_exact (ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ new_ಅಂಕೆ = 0; new_ಅಂಕೆ < ಎಣಿಕೆ; new_ಅಂಕೆ++)
    {
      hb_codepoint_t old_ಅಂಕೆ = remap.backward (new_ಅಂಕೆ);
      ಪ್ರತಿಜ್ಞಾ (old_ಅಂಕೆ != CFF_UNDEF_CODE);

      ಯದಿ (unlikely (!encode_str (subrs[old_ಅಂಕೆ], fd, buffArray[new_ಅಂಕೆ])))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ಪ್ರತ್ಯಾಗಮ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  ತಾರ್ಕಿಕ encode_localsubrs (unsigned ಪೂರ್ಣಾಙ್ಕ fd, str_buff_vec_t &buffArray) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  ರಕ್ಷಿತ:
  ಸಂರಚನಾ drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (ಅಸತ್ಯ),
	ends_in_hint (ಅಸತ್ಯ),
	all_dropped (ಅಸತ್ಯ),
	vsindex_dropped (ಅಸತ್ಯ) {}

    ತಾರ್ಕಿಕ  seen_moveto;
    ತಾರ್ಕಿಕ  ends_in_hint;
    ತಾರ್ಕಿಕ  all_dropped;
    ತಾರ್ಕಿಕ  vsindex_dropped;
  };

  ತಾರ್ಕಿಕ drop_hints_in_subr (parsed_cs_str_t &str, unsigned ಪೂರ್ಣಾಙ್ಕ pos,
			   parsed_cs_str_vec_t &subrs, unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ_2,
			   ಧ್ರುವ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = ಅಸತ್ಯ;
    ತಾರ್ಕಿಕ has_hint = drop_hints_in_str (subrs[subr_ಅಂಕೆ_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    ಯದಿ (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      ಯದಿ (!str.at_end (pos))
	drop.ends_in_hint = ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ ಯದಿ (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    ಪ್ರತ್ಯಾಗಮ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  ತಾರ್ಕಿಕ drop_hints_in_str (parsed_cs_str_t &str, ಧ್ರುವ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    ತಾರ್ಕಿಕ  seen_hint = ಅಸತ್ಯ;

    unsigned ಎಣಿಕೆ = str.values.length;
    auto *values = str.values.arrayZ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ pos = 0; pos < ಎಣಿಕೆ; pos++)
    {
      ತಾರ್ಕಿಕ  has_hint = ಅಸತ್ಯ;
      ವಿಕಲ್ಪ (values[pos].op)
      {
	ಪಕ್ಷ OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_ಅಂಕೆ_2,
					param, drop);
	  ಭಙ್ಗ;

	ಪಕ್ಷ OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_ಅಂಕೆ_2,
					param, drop);
	  ಭಙ್ಗ;

	ಪಕ್ಷ OpCode_rmoveto:
	ಪಕ್ಷ OpCode_hmoveto:
	ಪಕ್ಷ OpCode_vmoveto:
	  drop.seen_moveto = ಸತ್ಯ;
	  ಭಙ್ಗ;

	ಪಕ್ಷ OpCode_hintmask:
	ಪಕ್ಷ OpCode_cntrmask:
	  ಯದಿ (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    ಭಙ್ಗ;
	  }
	  HB_FALLTHROUGH;

	ಪಕ್ಷ OpCode_hstemhm:
	ಪಕ್ಷ OpCode_vstemhm:
	ಪಕ್ಷ OpCode_hstem:
	ಪಕ್ಷ OpCode_vstem:
	  has_hint = ಸತ್ಯ;
	  values[pos].set_hinting ();
	  ಯದಿ (str.at_end (pos))
	    drop.ends_in_hint = ಸತ್ಯ;
	  ಭಙ್ಗ;

	ಪಕ್ಷ OpCode_dotsection:
	  values[pos].set_hinting ();
	  ಭಙ್ಗ;

	ಸ್ವಭಾವ:
	  /* NONE */
	  ಭಙ್ಗ;
      }
      ಯದಿ (has_hint)
      {
	ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(unsigned)i];
	  ಯದಿ (csop.is_hinting ())
	    ಭಙ್ಗ;
	  csop.set_hinting ();
	  ಯದಿ (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = ಸತ್ಯ;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = ಸತ್ಯ;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ pos = 0; pos < ಎಣಿಕೆ; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      ಯದಿ (csop.op == OpCode_return)
	ಭಙ್ಗ;
      ಯದಿ (!csop.is_hinting ())
      {
	drop.all_dropped = ಅಸತ್ಯ;
	ಭಙ್ಗ;
      }
    }

    ಪ್ರತ್ಯಾಗಮ seen_hint;
  }

  ತಾರ್ಕಿಕ closure_subroutines (ಧ್ರುವ parsed_cs_str_vec_t& global_subrs,
                            ಧ್ರುವ hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    ಪ್ರತಿ (auto _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      unsigned ಪೂರ್ಣಾಙ್ಕ fd = subset_get_fd (acc, old_glyph);
      ಯದಿ (unlikely (fd >= acc.fd_ಎಣಿಕೆ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

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

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ collect_subr_refs_in_subr (unsigned ಪೂರ್ಣಾಙ್ಕ subr_ಅಂಕೆ_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  ಧ್ರುವ subr_subset_param_t &param)
  {
    ಯದಿ (closure->has (subr_ಅಂಕೆ_2))
      ಪ್ರತ್ಯಾಗಮ;
    closure->add (subr_ಅಂಕೆ_2);
    collect_subr_refs_in_str (subrs[subr_ಅಂಕೆ_2], param);
  }

  ಶೂನ್ಯ collect_subr_refs_in_str (ಧ್ರುವ parsed_cs_str_t &str,
                                 ಧ್ರುವ subr_subset_param_t &param)
  {
    ಯದಿ (!str.has_calls ())
      ಪ್ರತ್ಯಾಗಮ;

    ಪ್ರತಿ (auto &opstr : str.values)
    {
      ಯದಿ (!param.drop_hints || !opstr.is_hinting ())
      {
	ವಿಕಲ್ಪ (opstr.op)
	{
	  ಪಕ್ಷ OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_ಅಂಕೆ_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    ಭಙ್ಗ;

	  ಪಕ್ಷ OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_ಅಂಕೆ_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    ಭಙ್ಗ;

	  ಸ್ವಭಾವ: ಭಙ್ಗ;
	}
      }
    }
  }

  ತಾರ್ಕಿಕ encode_str (ಧ್ರುವ parsed_cs_str_t &str, ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ fd, str_buff_t &buff, ತಾರ್ಕಿಕ encode_prefix = ಸತ್ಯ) ಧ್ರುವ
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    ತಾರ್ಕಿಕ hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    ಯದಿ (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_ಅಂಕೆ_cs (str.prefix_ಅಂಕೆ_2 ());
      ಯದಿ (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    unsigned size = 0;
    ಪ್ರತಿ (auto &opstr : str.values)
    {
      size += opstr.length;
      ಯದಿ (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        size += 3;
    }
    ಯದಿ (!buff.alloc_exact (buff.length + size))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (auto &opstr : str.values)
    {
      ಯದಿ (hinting || !opstr.is_hinting ())
      {
	ವಿಕಲ್ಪ (opstr.op)
	{
	  ಪಕ್ಷ OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_ಅಂಕೆ (opstr.subr_ಅಂಕೆ_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ಭಙ್ಗ;

	  ಪಕ್ಷ OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_ಅಂಕೆ (opstr.subr_ಅಂಕೆ_2));
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ಭಙ್ಗ;

	  ಸ್ವಭಾವ:
	    encoder.copy_str (opstr.ptr, opstr.length);
	    ಭಙ್ಗ;
	}
      }
    }
    ಪ್ರತ್ಯಾಗಮ !encoder.in_error ();
  }

  ಶೂನ್ಯ compact_parsed_subrs () ಧ್ರುವ
  {
    ಪ್ರತಿ (auto &cs : parsed_global_subrs_storage)
      cs.compact ();
    ಪ್ರತಿ (auto &vec : parsed_local_subrs_storage)
      ಪ್ರತಿ (auto &cs : vec)
	cs.compact ();
  }

  ಶೂನ್ಯ populate_subset_accelerator () ಧ್ರುವ
  {
    ಯದಿ (!plan->inprogress_accelerator) ಪ್ರತ್ಯಾಗಮ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ಧ್ರುವ parsed_cs_str_t& get_parsed_charstring (unsigned i) ಧ್ರುವ
  {
    ಯದಿ (cached_charstrings) ಪ್ರತ್ಯಾಗಮ *(cached_charstrings[i]);
    ಪ್ರತ್ಯಾಗಮ parsed_charstrings[i];
  }

  ರಕ್ಷಿತ:
  ಧ್ರುವ ACC			&acc;
  ಧ್ರುವ hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<ಧ್ರುವ parsed_cs_str_t*>     cached_charstrings;
  ಧ್ರುವ parsed_cs_str_vec_t*              parsed_global_subrs;
  ಧ್ರುವ hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  ಗುಪ್ತ:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename SUBRS::ಎಣಿಕೆ_type subr_ಎಣಿಕೆ_type;
};

} /* namespace CFF */

HB_INTERNAL ತಾರ್ಕಿಕ
hb_plan_subset_cff_fdselect (ಧ್ರುವ hb_subset_plan_t *plan,
			    unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ,
			    ಧ್ರುವ CFF::FDSelect &src, /* IN */
			    unsigned ಪೂರ್ಣಾಙ್ಕ src_ಅಂಕೆ_glyphs,
			    unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fd_ಎಣಿಕೆ /* OUT */,
			    unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fdselect_size /* OUT */,
			    unsigned ಪೂರ್ಣಾಙ್ಕ &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL ತಾರ್ಕಿಕ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			  ಧ್ರುವ CFF::FDSelect &src,
			  unsigned ಪೂರ್ಣಾಙ್ಕ fd_ಎಣಿಕೆ_2,
			  unsigned ಪೂರ್ಣಾಙ್ಕ fdselect_format,
			  unsigned ಪೂರ್ಣಾಙ್ಕ size,
			  ಧ್ರುವ hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#endif /* HB_SUBSET_CFF_COMMON_HH */
