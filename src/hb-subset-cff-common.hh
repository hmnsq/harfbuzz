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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_CFF_COMMON_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_CFF_COMMON_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-interp-cs-common.hh"

ਨਾਮਾਕਾਸ਼ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ACC>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
subset_get_fd (ਅਚਲ ACC &acc, hb_codepoint_t glyph)
{
  ਵਾਪਸ likely (glyph < acc.ਅੰਕ_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ਬਣਤਰ str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  ਖਾਲੀ reset () { buff.reset (); }

  ਖਾਲੀ encode_byte (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ b)
  {
    ਜੇ (likely ((ਨਿਸ਼ਾਨੀਵਾਲਾ) buff.ਲੰਮਾਈ < buff.allocated))
      buff.arrayZ[buff.ਲੰਮਾਈ++] = b;
    ਨਹੀਂਤਾਂ
      buff.push (b);
  }

  ਖਾਲੀ encode_int (ਪੂਰਨ v)
  {
    ਜੇ ((-1131 <= v) && (v <= 1131))
    {
      ਜੇ ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      ਨਹੀਂਤਾਂ ਜੇ (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + Op_ਕੋਡ_Two_Byte_Pos_Int0);
	encode_byte (v & 0xFF);
      }
      ਨਹੀਂਤਾਂ
      {
	v = -v - 108;
	encode_byte ((v >> 8) + Op_ਕੋਡ_Two_Byte_Neg_Int0);
	encode_byte (v & 0xFF);
      }
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (unlikely (v < -32768))
	v = -32768;
      ਨਹੀਂਤਾਂ ਜੇ (unlikely (v > 32767))
	v = 32767;
      encode_byte (Op_ਕੋਡ_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  ਖਾਲੀ encode_ਅੰਕ_cs (ਅਚਲ ਸੰਖਿਆ_t& n)
  {
    ਜੇ (n.ਵਿੱਚ_int_range ())
    {
      encode_int (n.ਮੰਜ਼ਿਲ_int ());
    }
    ਨਹੀਂਤਾਂ
    {
      int32_t v = n.ਮੰਜ਼ਿਲ_fixed ();
      encode_byte (Op_ਕੋਡ_fixedcs);
      encode_byte ((v >> 24) & 0xFF);
      encode_byte ((v >> 16) & 0xFF);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for TopDict / Private
  ਖਾਲੀ encode_ਅੰਕ_tp (ਅਚਲ ਸੰਖਿਆ_t& n)
  {
    ਜੇ (n.ਵਿੱਚ_int_range ())
    {
      // TODO longint
      encode_int (n.ਮੰਜ਼ਿਲ_int ());
    }
    ਨਹੀਂਤਾਂ
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      ਦੁੱਗਣਾ v = n.ਮੰਜ਼ਿਲ_real ();
      encode_byte (Op_ਕੋਡ_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      ਅੱਖਰ buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_ਅਣਵਰਤਿਆ;
      hb_locale_t oldlocale HB_ਅਣਵਰਤਿਆ;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ਨਿਰਾ));
      snprintf (buf, ਆਕਾਰ (buf), "%.8G", v);
      (ਖਾਲੀ) hb_uselocale (((ਖਾਲੀ) freelocale (clocale), oldlocale));

      ਅੱਖਰ *s = buf;
      size_t len;
      ਅੱਖਰ *comma = strchr (s, ',');
      ਜੇ (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      ਜੇ (s[0] == '0' && s[1] == '.')
	s++;
      ਨਹੀਂਤਾਂ ਜੇ (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      ਨਹੀਂਤਾਂ ਜੇ ((len = ਲੜੀ_ਲੰਬਾਈ (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ exponent = len - 3;
	ਅੱਖਰ *s2 = s + exponent - 1;
	ਜਦੋਂ (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, ਆਕਾਰ (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      ਨਹੀਂਤਾਂ
      {
	ਅੱਖਰ *dot = strchr (s, '.');
	ਅੱਖਰ *e = strchr (s, 'E');
	ਜੇ (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  ਪੂਰਨ exponent = atoi (e + 1);
	  ਪੂਰਨ new_exponent = exponent - (e - (dot + 1));
	  ਜੇ (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  ਨਹੀਂਤਾਂ
	    snprintf (e - 1, ਆਕਾਰ (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      ਜੇ ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	ਪੂਰਨ sign = s[0] == '-';
	ਅੱਖਰ *s2 = s + sign + 1;
	ਜਦੋਂ (*s2 == '0')
	  s2++;
	len = ਲੜੀ_ਲੰਬਾਈ (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, ਆਕਾਰ (buf) - (s + sign + len - buf), "E-%u", (ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਲੜੀ_ਲੰਬਾਈ (s + sign) - 1));
      }
      hb_ਸਦਿਸ਼_t<ਅੱਖਰ> nibbles;
      ਜਦੋਂ (*s)
      {
	ਅੱਖਰ c = s[0];
	s++;

	ਚੋਣ (c)
	{
	  ਹਾਲਤ 'E':
	  {
	    ਅੱਖਰ c2 = *s;
	    ਜੇ (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } ਨਹੀਂਤਾਂ {
	      ਜੇ (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    ਜੇ (*s == '0')
	      s++;
	    ਜਾਰੀ;
	  }

	  ਹਾਲਤ '.':
	    nibbles.push (0x0A); // .
	    ਜਾਰੀ;

	  ਹਾਲਤ '-':
	    nibbles.push (0x0E); // -
	    ਜਾਰੀ;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      ਜੇ (nibbles.ਲੰਮਾਈ % 2)
	nibbles.push (0x0F);

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = nibbles.ਲੰਮਾਈ;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  ਖਾਲੀ encode_op (op_ਕੋਡ_t op)
  {
    ਜੇ (Is_Op_ਕੋਡ_ESC (op))
    {
      encode_byte (Op_ਕੋਡ_escape);
      encode_byte (Unmake_Op_ਕੋਡ_ESC (op));
    }
    ਨਹੀਂਤਾਂ
      encode_byte (op);
  }

  ਖਾਲੀ copy_str (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ)
  {
    ਪ੍ਰਤਿਜ੍ਞਾ ((ਨਿਸ਼ਾਨੀਵਾਲਾ) (buff.ਲੰਮਾਈ + ਲੰਮਾਈ) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.ਲੰਮਾਈ, str, ਲੰਮਾਈ);
    buff.ਲੰਮਾਈ += ਲੰਮਾਈ;
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ buff.ਵਿੱਚ_error (); }

  ਸੁਰੱਖਿਅਤ:

  str_buff_t &buff;
};

ਬਣਤਰ cff_sub_ਸਾਰਣੀ_info_t {
  cff_sub_ਸਾਰਣੀ_info_t ()
    : fd_array_link (0),
      char_ਲੜੀਆਂ_link (0)
  {
    fd_select.init ();
  }

  ਸਾਰਣੀ_info_t     fd_select;
  objidx_t     	   fd_array_link;
  objidx_t     	   char_ਲੜੀਆਂ_link;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ OPSTR=op_str_t>
ਬਣਤਰ cff_ਸਿਖਰ_dict_op_serializer_t : op_serializer_t
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ OPSTR &opstr,
		  ਅਚਲ cff_sub_ਸਾਰਣੀ_info_t &info) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਚੋਣ (opstr.op)
    {
      ਹਾਲਤ Op_ਕੋਡ_Char_ਲੜੀਆਂ:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_ਲੜੀਆਂ_link, whence_t::Absolute));

      ਹਾਲਤ Op_ਕੋਡ_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      ਹਾਲਤ Op_ਕੋਡ_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      ਮੂਲ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (ਸੱਚ);
  }
};

ਬਣਤਰ cff_font_dict_op_serializer_t : op_serializer_t
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
		  ਅਚਲ op_str_t &opstr,
		  ਅਚਲ ਸਾਰਣੀ_info_t &ਨਿੱਜੀ_Dict_Info) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਜੇ (opstr.op == Op_ਕੋਡ_ਨਿੱਜੀ)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, ਨਿੱਜੀ_Dict_Info.ਮਾਪ) &&
		    Dict::serialize_link4_op (c, opstr.op, ਨਿੱਜੀ_Dict_Info.link, whence_t::Absolute));
    }
    ਨਹੀਂਤਾਂ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *d = c->allocate_ਮਾਪ<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> (opstr.ਲੰਮਾਈ);
      ਜੇ (unlikely (!d)) return_trace (ਝੂਠ);
      /* Faster than hb_memcpy for small strings. */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < opstr.ਲੰਮਾਈ; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (ਸੱਚ);
  }
};

/* CharString command for specialization */
ਬਣਤਰ cs_command_t
{
  hb_ਸਦਿਸ਼_t<ਸੰਖਿਆ_t> ਦਲੀਲਾਂ;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> ਨਕਾਬ_ਬਾਈਟਾਂ; /* For hintmask/cntrmask payload bytes. */
  op_ਕੋਡ_t op;

  cs_command_t () : op (Op_ਕੋਡ_Invalid) {}
  cs_command_t (op_ਕੋਡ_t op_) : op (op_) {}
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_ਸਦਿਸ਼_t<cs_command_t> *cs_command_vec_t;

ਬਣਤਰ cff2_instancing_plan_t;

ਬਣਤਰ flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   ਤਾਰਕਿਕ drop_hints_,
                   ਅਚਲ hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = ਖਾਲੀ_ਸੂਚਕ)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  ਤਾਰਕਿਕ	drop_hints;
  ਅਚਲ hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ਅਚਲ cff2_instancing_plan_t *instancer = ਖਾਲੀ_ਸੂਚਕ;
  ਤਾਰਕਿਕ emitted_blend = ਝੂਠ;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ACC, ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ OPSET, op_ਕੋਡ_t endchar_op=Op_ਕੋਡ_Invalid>
ਬਣਤਰ subr_flattener_t
{
  subr_flattener_t (ਅਚਲ ACC &acc_,
		    ਅਚਲ hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  ਤਾਰਕਿਕ flatten (str_buff_vec_t &flat_charstrings,
                hb_ਸਦਿਸ਼_t<hb_ਸਦਿਸ਼_t<cs_command_t>> *command_capture = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = plan->ਅੰਕ_output_glyphs ();
    ਜੇ (!flat_charstrings.resize_exact (ਗਿਣਤੀ_2))
      ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_codepoint_t  glyph;
      ਜੇ (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	ਜੇ (endchar_op != Op_ਕੋਡ_Invalid) flat_charstrings[i].push (endchar_op);
	ਜਾਰੀ;
      }
      ਅਚਲ hb_ubytes_t str = (*acc.char_ਲੜੀਆਂ)[glyph];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = subset_get_fd (acc, glyph);
      ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
	ਵਾਪਸ ਝੂਠ;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.ਲੰਮਾਈ);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (ਤਾਰਕਿਕ) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : ਖਾਲੀ_ਸੂਚਕ
      };
      ਜੇ (unlikely (!interp.interpret (param)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਅਚਲ ACC &acc;
  ਅਚਲ hb_subset_plan_t *plan;
};

ਬਣਤਰ subr_closures_t
{
  subr_closures_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ_2) : global_ਘੇਰਾ (), local_closures ()
  {
    local_closures.resize_exact (fd_ਗਿਣਤੀ_2);
  }

  ਖਾਲੀ reset ()
  {
    global_ਘੇਰਾ.clear();
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < local_closures.ਲੰਮਾਈ; i++)
      local_closures[i].clear();
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ local_closures.ਵਿੱਚ_error (); }
  hb_ਸੈੱਟ_t  global_ਘੇਰਾ;
  hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> local_closures;
};

ਬਣਤਰ parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ = 0) :
    subr_ਅੰਕ_2 (subr_ਅੰਕ) {}

  ਤਾਰਕਿਕ is_hinting () ਅਚਲ { ਵਾਪਸ hinting_flag; }
  ਖਾਲੀ ਸੈੱਟ_hinting ()       { hinting_flag = ਸੱਚ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  ਸੁਰੱਖਿਅਤ:
  ਤਾਰਕਿਕ	  hinting_flag = ਝੂਠ;

  ਜਨਤਕ:
  uint16_t subr_ਅੰਕ_2;
};

ਬਣਤਰ parsed_cs_str_t : parsed_ਮੁੱਲਾਂ_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (ਝੂਠ),
    hint_dropped (ਝੂਠ),
    has_ਅਗੇਤਰ (ਝੂਠ),
    has_calls_ (ਝੂਠ),
    coalescing_ (ਝੂਠ)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ ਜੋੜੋ_op (op_ਕੋਡ_t op, ਅਚਲ byte_str_ref_t& str_ref)
  {
    ਜੇ (is_parsed ()) ਵਾਪਸ;
    ਜੇ (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_ਸਿਰਾ = ਆਖਰੀ_ਸਿਰਾ_2;
      ਆਖਰੀ_ਸਿਰਾ_2 = str_ref.get_ਵਿੱਥ ();
      ਜੇ (unlikely (op == Op_ਕੋਡ_return || op == Op_ਕੋਡ_endchar))
	flush_segment (str_ref, ਆਖਰੀ_ਸਿਰਾ_2);
      ਵਾਪਸ;
    }
    SUPER::ਜੋੜੋ_op (op, str_ref);
  }

  ਖਾਲੀ ਜੋੜੋ_call_op (op_ਕੋਡ_t op, ਅਚਲ byte_str_ref_t& str_ref, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ_2)
  {
    ਜੇ (is_parsed ()) ਵਾਪਸ;
    has_calls_ = ਸੱਚ;

    ਜੇ (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_ਸਿਰਾ);
      op_ਸ਼ੁਰੂ = ਆਖਰੀ_ਸਿਰਾ_2;
      SUPER::ਜੋੜੋ_op (op, str_ref, {subr_ਅੰਕ_2});
      penultimate_ਸਿਰਾ = ਆਖਰੀ_ਸਿਰਾ_2 = str_ref.get_ਵਿੱਥ ();
      ਵਾਪਸ;
    }

    /* Pop the subroutine number. */
    values.ਕੱਢੋ ();

    SUPER::ਜੋੜੋ_op (op, str_ref, {subr_ਅੰਕ_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  ਖਾਲੀ flush_segment (ਅਚਲ byte_str_ref_t& str_ref, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = op_ਸ਼ੁਰੂ;
    ਜੇ (ਸਿਰਾ <= ਸ਼ੁਰੂ) ਵਾਪਸ;
    ਜਦੋਂ (ਸ਼ੁਰੂ < ਸਿਰਾ)
    {
      ਸਵੈਚਾਲਿਤ arr = str_ref.sub_array (ਸ਼ੁਰੂ, hb_min (ਸਿਰਾ - ਸ਼ੁਰੂ, 255u));
      ਜੇ (unlikely (!arr.ਲੰਮਾਈ)) ਰੋਕੋ;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->ਲੰਮਾਈ = arr.ਲੰਮਾਈ;
      ਸ਼ੁਰੂ += arr.ਲੰਮਾਈ;
    }
    op_ਸ਼ੁਰੂ = ਸਿਰਾ;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  ਖਾਲੀ flush_coalesced (ਅਚਲ byte_str_ref_t& str_ref)
  {
    ਜੇ (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_ਵਿੱਥ ());
  }

  ਖਾਲੀ enable_coalescing () { coalescing_ = ਸੱਚ; }
  ਤਾਰਕਿਕ is_coalescing () ਅਚਲ { ਵਾਪਸ coalescing_; }

  ਖਾਲੀ ਸੈੱਟ_ਅਗੇਤਰ (ਅਚਲ ਸੰਖਿਆ_t &ਅੰਕ, op_ਕੋਡ_t op = Op_ਕੋਡ_Invalid)
  {
    has_ਅਗੇਤਰ = ਸੱਚ;
    ਅਗੇਤਰ_op = op;
    ਅਗੇਤਰ_ਅੰਕ = ਅੰਕ;
  }

  ਤਾਰਕਿਕ at_ਸਿਰਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos) ਅਚਲ
  {
    ਵਾਪਸ ((pos + 1 >= values.ਲੰਮਾਈ) /* CFF2 */
	|| (values[pos + 1].op == Op_ਕੋਡ_return));
  }

  ਤਾਰਕਿਕ is_parsed () ਅਚਲ { ਵਾਪਸ parsed; }
  ਖਾਲੀ ਸੈੱਟ_parsed ()      { parsed = ਸੱਚ; }

  ਤਾਰਕਿਕ is_hint_dropped () ਅਚਲ { ਵਾਪਸ hint_dropped; }
  ਖਾਲੀ ਸੈੱਟ_hint_dropped ()      { hint_dropped = ਸੱਚ; }

  ਤਾਰਕਿਕ is_vsindex_dropped () ਅਚਲ { ਵਾਪਸ vsindex_dropped; }
  ਖਾਲੀ ਸੈੱਟ_vsindex_dropped ()      { vsindex_dropped = ਸੱਚ; }

  ਤਾਰਕਿਕ has_ਅਗੇਤਰ_2 () ਅਚਲ          { ਵਾਪਸ has_ਅਗੇਤਰ; }
  op_ਕੋਡ_t ਅਗੇਤਰ_op_2 () ਅਚਲ         { ਵਾਪਸ ਅਗੇਤਰ_op; }
  ਅਚਲ ਸੰਖਿਆ_t &ਅਗੇਤਰ_ਅੰਕ_2 () ਅਚਲ { ਵਾਪਸ ਅਗੇਤਰ_ਅੰਕ; }

  ਤਾਰਕਿਕ has_calls () ਅਚਲ          { ਵਾਪਸ has_calls_; }

  ਖਾਲੀ compact ()
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = values.ਲੰਮਾਈ;
    ਜੇ (!ਗਿਣਤੀ_2) ਵਾਪਸ;
    ਸਵੈਚਾਲਿਤ &opstr = values.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < ਗਿਣਤੀ_2; i++)
    {
      /* See if we can combine op j and op i. */
      ਤਾਰਕਿਕ combine =
        (opstr[j].op != Op_ਕੋਡ_callsubr && opstr[j].op != Op_ਕੋਡ_callgsubr) &&
        (opstr[i].op != Op_ਕੋਡ_callsubr && opstr[i].op != Op_ਕੋਡ_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].ਲੰਮਾਈ == opstr[i].ptr) &&
        (opstr[j].ਲੰਮਾਈ + opstr[i].ਲੰਮਾਈ <= 255);

      ਜੇ (combine)
      {
	opstr[j].ਲੰਮਾਈ += opstr[i].ਲੰਮਾਈ;
	opstr[j].op = Op_ਕੋਡ_Invalid;
      }
      ਨਹੀਂਤਾਂ
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  ਸੁਰੱਖਿਅਤ:
  ਤਾਰਕਿਕ    parsed : 1;
  ਤਾਰਕਿਕ    hint_dropped : 1;
  ਤਾਰਕਿਕ    vsindex_dropped : 1;
  ਤਾਰਕਿਕ    has_ਅਗੇਤਰ : 1;
  ਤਾਰਕਿਕ    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  ਤਾਰਕਿਕ    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ penultimate_ਸਿਰਾ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_ਸਿਰਾ_2 = 0;
  op_ਕੋਡ_t	ਅਗੇਤਰ_op;
  ਸੰਖਿਆ_t	ਅਗੇਤਰ_ਅੰਕ;

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ parsed_ਮੁੱਲਾਂ_t<parsed_cs_op_t> SUPER;
};

ਬਣਤਰ parsed_cs_str_vec_t : hb_ਸਦਿਸ਼_t<parsed_cs_str_t>
{
  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_ਸਦਿਸ਼_t<parsed_cs_str_t> SUPER;
};

ਬਣਤਰ cff_subset_accelerator_t
{
  ਸਥਿਰ cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ਅਚਲ parsed_cs_str_vec_t& parsed_charstrings,
      ਅਚਲ parsed_cs_str_vec_t& parsed_global_subrs,
      ਅਚਲ hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (ਆਕਾਰ(cff_subset_accelerator_t));
    ਜੇ (unlikely (!accel)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਨਵਾਂ (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    ਵਾਪਸ accel;
  }

  ਸਥਿਰ ਖਾਲੀ destroy (ਖਾਲੀ* value) {
    ਜੇ (!value) ਵਾਪਸ;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ਅਚਲ parsed_cs_str_vec_t& parsed_charstrings_,
      ਅਚਲ parsed_cs_str_vec_t& parsed_global_subrs_,
      ਅਚਲ hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    ਸਵੈਚਾਲਿਤ *mapping = glyph_ਮੰਜ਼ਿਲ_sid_map.get_relaxed ();
    ਜੇ (mapping)
    {
      mapping->~glyph_ਮੰਜ਼ਿਲ_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t> parsed_local_subrs;
  ਬਦਲਣਯੋਗ hb_atomic_t<glyph_ਮੰਜ਼ਿਲ_sid_map_t *> glyph_ਮੰਜ਼ਿਲ_sid_map;

 ਨਿਜੀ:
  hb_blob_t* original_blob;
};

ਬਣਤਰ subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_ਸੈੱਟ_t *global_ਘੇਰਾ_2,
		       hb_ਸੈੱਟ_t *local_ਘੇਰਾ,
		       ਤਾਰਕਿਕ drop_hints_,
		       ਤਾਰਕਿਕ coalesce_ = ਝੂਠ) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_ਘੇਰਾ (global_ਘੇਰਾ_2),
      local_ਘੇਰਾ_2 (local_ਘੇਰਾ),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    ਜੇ (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    ਚੋਣ (context.type)
    {
      ਹਾਲਤ CSType_CharString:
	ਵਾਪਸ parsed_charstring;

      ਹਾਲਤ CSType_LocalSubr:
	ਜੇ (likely (context.subr_ਅੰਕ_2 < parsed_local_subrs->ਲੰਮਾਈ))
	  ਵਾਪਸ &(*parsed_local_subrs)[context.subr_ਅੰਕ_2];
	ਰੋਕੋ;

      ਹਾਲਤ CSType_GlobalSubr:
	ਜੇ (likely (context.subr_ਅੰਕ_2 < parsed_global_subrs->ਲੰਮਾਈ))
	  ਵਾਪਸ &(*parsed_global_subrs)[context.subr_ਅੰਕ_2];
	ਰੋਕੋ;
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ENV>
  ਖਾਲੀ ਸੈੱਟ_current_str (ENV &env, ਤਾਰਕਿਕ calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    ਜੇ (unlikely (!parsed_str))
    {
      env.ਸੈੱਟ_error ();
      ਵਾਪਸ;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    ਜੇ (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.ਲੰਮਾਈ > 0)))
      env.ਸੈੱਟ_error ();
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.ਕੁੱਲ_ਮਾਪ ());
        ਜੇ (coalesce) parsed_str->enable_coalescing ();
      }
      current_parsed_str = parsed_str;
    }
  }

  parsed_cs_str_t	*current_parsed_str;

  parsed_cs_str_t	*parsed_charstring;
  parsed_cs_str_vec_t	*parsed_global_subrs;
  parsed_cs_str_vec_t	*parsed_local_subrs;
  hb_ਸੈੱਟ_t      *global_ਘੇਰਾ;
  hb_ਸੈੱਟ_t      *local_ਘੇਰਾ_2;
  ਤਾਰਕਿਕ	  drop_hints;
  ਤਾਰਕਿਕ	  coalesce;
};

ਬਣਤਰ subr_remap_t : hb_inc_bimap_t
{
  ਖਾਲੀ create (ਅਚਲ hb_ਸੈੱਟ_t *ਘੇਰਾ)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (ਘੇਰਾ->get_population ());
    ਲਈ (ਸਵੈਚਾਲਿਤ old_ਅੰਕ : *ਘੇਰਾ)
      ਜੋੜੋ (old_ਅੰਕ);

    ਜੇ (get_population () < 1240)
      bias = 107;
    ਨਹੀਂਤਾਂ ਜੇ (get_population () < 33900)
      bias = 1131;
    ਨਹੀਂਤਾਂ
      bias = 32768;
  }

  ਪੂਰਨ biased_ਅੰਕ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_ਅੰਕ) ਅਚਲ
  {
    hb_codepoint_t new_ਅੰਕ = get (old_ਅੰਕ);
    ਵਾਪਸ (ਪੂਰਨ)new_ਅੰਕ - bias;
  }

  ਸੁਰੱਖਿਅਤ:
  ਪੂਰਨ bias;
};

ਬਣਤਰ subr_remaps_t
{
  subr_remaps_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ)
  {
    local_remaps.resize (fd_ਗਿਣਤੀ);
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error()
  {
    ਵਾਪਸ local_remaps.ਵਿੱਚ_error ();
  }

  ਖਾਲੀ create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_ਘੇਰਾ);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < local_remaps.ਲੰਮਾਈ; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_ਸਦਿਸ਼_t<subr_remap_t>  local_remaps;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ SUBSETTER, ਕਿਸਮ_ਨਾਂ SUBRS, ਕਿਸਮ_ਨਾਂ ACC, ਕਿਸਮ_ਨਾਂ ENV, ਕਿਸਮ_ਨਾਂ OPSET, op_ਕੋਡ_t endchar_op=Op_ਕੋਡ_Invalid>
ਬਣਤਰ subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ਅਚਲ hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_ਗਿਣਤੀ),
        remaps(acc_.fd_ਗਿਣਤੀ)
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
  ਤਾਰਕਿਕ subset (ਖਾਲੀ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ fd_ਗਿਣਤੀ_2 = acc.fd_ਗਿਣਤੀ;
    ਅਚਲ cff_subset_accelerator_t* cff_accelerator = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_ਗਿਣਤੀ_2 = cff_accelerator->parsed_local_subrs.ਲੰਮਾਈ;
    }

    ਜੇ (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->ਅੰਕ_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } ਨਹੀਂਤਾਂ {
      parsed_charstrings.resize_exact (plan->ਅੰਕ_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->ਗਿਣਤੀ_2);

      ਜੇ (unlikely (!parsed_local_subrs_storage.resize (fd_ਗਿਣਤੀ_2))) ਵਾਪਸ ਝੂਠ;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < acc.fd_ਗਿਣਤੀ; i++)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = acc.ਨਿੱਜੀ_Dicts[i].localSubrs->ਗਿਣਤੀ_2;
        parsed_local_subrs_storage[i].resize (ਗਿਣਤੀ_2);
        ਜੇ (unlikely (parsed_local_subrs_storage[i].ਵਿੱਚ_error ())) ਵਾਪਸ ਝੂਠ;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    ਜੇ (unlikely (remaps.ਵਿੱਚ_error()
                  || cached_charstrings.ਵਿੱਚ_error ()
                  || parsed_charstrings.ਵਿੱਚ_error ()
                  || parsed_global_subrs->ਵਿੱਚ_error ()
                  || closures.ਵਿੱਚ_error ())) {
      ਵਾਪਸ ਝੂਠ;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    ਤਾਰਕਿਕ coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.ਪਹਿਲਾ;
      hb_codepoint_t old_glyph = _.second;

      ਅਚਲ hb_ubytes_t str = (*acc.char_ਲੜੀਆਂ)[old_glyph];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = subset_get_fd (acc, old_glyph);
      ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        ਜੇ (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        ਨਹੀਂਤਾਂ
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        ਜਾਰੀ;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.ਲੰਮਾਈ);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_ਘੇਰਾ,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      ਜੇ (unlikely (!interp.interpret (param)))
        ਵਾਪਸ ਝੂਠ;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      ਜੇ ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_ਘੇਰਾ,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	ਜੇ (drop_hints_ਵਿੱਚ_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].ਸੈੱਟ_hint_dropped ();
	  ਜੇ (drop.vsindex_dropped)
	    parsed_charstrings[new_glyph].ਸੈੱਟ_vsindex_dropped ();
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
      ਜੇ (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    ਜੇ ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !ਘੇਰਾ_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      ਵਾਪਸ ਝੂਠ;

    remaps.create (closures);

    populate_subset_accelerator ();
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ encode_charstrings (str_buff_vec_t &buffArray, ਤਾਰਕਿਕ encode_ਅਗੇਤਰ = ਸੱਚ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = plan->ਅੰਕ_output_glyphs ();
    ਜੇ (unlikely (!buffArray.resize_exact (ਅੰਕ_glyphs)))
      ਵਾਪਸ ਝੂਠ;
    hb_codepoint_t ਆਖਰੀ = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t gid = _.ਪਹਿਲਾ;
      hb_codepoint_t old_glyph = _.second;

      ਜੇ (endchar_op != Op_ਕੋਡ_Invalid)
        ਲਈ (; ਆਖਰੀ < gid; ਆਖਰੀ++)
	{
	  // Hack to point vector to static string.
	  ਸਵੈਚਾਲਿਤ &b = buffArray.arrayZ[ਆਖਰੀ];
	  b.ਸੈੱਟ_storage (const_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *>(endchar_str), 1);
	}

      ਆਖਰੀ++; // Skip over gid
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  fd = subset_get_fd (acc, old_glyph);
      ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
	ਵਾਪਸ ਝੂਠ;
      ਜੇ (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_ਅਗੇਤਰ)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (endchar_op != Op_ਕੋਡ_Invalid)
      ਲਈ (; ਆਖਰੀ < ਅੰਕ_glyphs; ਆਖਰੀ++)
      {
	// Hack to point vector to static string.
	ਸਵੈਚਾਲਿਤ &b = buffArray.arrayZ[ਆਖਰੀ];
	b.ਸੈੱਟ_storage (const_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *>(endchar_str), 1);
      }

    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ encode_subrs (ਅਚਲ parsed_cs_str_vec_t &subrs, ਅਚਲ subr_remap_t& remap, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd, str_buff_vec_t &buffArray) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਗਿਣਤੀ_2 = remap.get_population ();

    ਜੇ (unlikely (!buffArray.resize_exact (ਗਿਣਤੀ_2)))
      ਵਾਪਸ ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਅੰਕ = 0; new_ਅੰਕ < ਗਿਣਤੀ_2; new_ਅੰਕ++)
    {
      hb_codepoint_t old_ਅੰਕ = remap.backward (new_ਅੰਕ);
      ਪ੍ਰਤਿਜ੍ਞਾ (old_ਅੰਕ != CFF_UNDEF_ਕੋਡ);

      ਜੇ (unlikely (!encode_str (subrs[old_ਅੰਕ], fd, buffArray[new_ਅੰਕ])))
	ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    ਵਾਪਸ encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  ਤਾਰਕਿਕ encode_localsubrs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd, str_buff_vec_t &buffArray) ਅਚਲ
  {
    ਵਾਪਸ encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  ਸੁਰੱਖਿਅਤ:
  ਬਣਤਰ drop_hints_param_t
  {
    drop_hints_param_t ()
      : ਵੇਖਿਆ_moveto (ਝੂਠ),
	ਸਿਰੇ_ਵਿੱਚ_hint (ਝੂਠ),
	all_dropped (ਝੂਠ),
	vsindex_dropped (ਝੂਠ) {}

    ਤਾਰਕਿਕ  ਵੇਖਿਆ_moveto;
    ਤਾਰਕਿਕ  ਸਿਰੇ_ਵਿੱਚ_hint;
    ਤਾਰਕਿਕ  all_dropped;
    ਤਾਰਕਿਕ  vsindex_dropped;
  };

  ਤਾਰਕਿਕ drop_hints_ਵਿੱਚ_subr (parsed_cs_str_t &str, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos,
			   parsed_cs_str_vec_t &subrs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ_2,
			   ਅਚਲ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ਸਿਰੇ_ਵਿੱਚ_hint = ਝੂਠ;
    ਤਾਰਕਿਕ has_hint = drop_hints_ਵਿੱਚ_str (subrs[subr_ਅੰਕ_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    ਜੇ (drop.ਸਿਰੇ_ਵਿੱਚ_hint)
    {
      str.values[pos].ਸੈੱਟ_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      ਜੇ (!str.at_ਸਿਰਾ (pos))
	drop.ਸਿਰੇ_ਵਿੱਚ_hint = ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (drop.all_dropped)
    {
      str.values[pos].ਸੈੱਟ_hinting ();
    }

    ਵਾਪਸ has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  ਤਾਰਕਿਕ drop_hints_ਵਿੱਚ_str (parsed_cs_str_t &str, ਅਚਲ subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    ਤਾਰਕਿਕ  ਵੇਖਿਆ_hint = ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = str.values.ਲੰਮਾਈ;
    ਸਵੈਚਾਲਿਤ *values = str.values.arrayZ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos = 0; pos < ਗਿਣਤੀ_2; pos++)
    {
      ਤਾਰਕਿਕ  has_hint = ਝੂਠ;
      ਚੋਣ (values[pos].op)
      {
	ਹਾਲਤ Op_ਕੋਡ_callsubr:
	  has_hint = drop_hints_ਵਿੱਚ_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_ਅੰਕ_2,
					param, drop);
	  ਰੋਕੋ;

	ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	  has_hint = drop_hints_ਵਿੱਚ_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_ਅੰਕ_2,
					param, drop);
	  ਰੋਕੋ;

	ਹਾਲਤ Op_ਕੋਡ_rmoveto:
	ਹਾਲਤ Op_ਕੋਡ_hmoveto:
	ਹਾਲਤ Op_ਕੋਡ_vmoveto:
	  drop.ਵੇਖਿਆ_moveto = ਸੱਚ;
	  ਰੋਕੋ;

	ਹਾਲਤ Op_ਕੋਡ_hintmask:
	ਹਾਲਤ Op_ਕੋਡ_cntrmask:
	  ਜੇ (drop.ਵੇਖਿਆ_moveto)
	  {
	    values[pos].ਸੈੱਟ_hinting ();
	    ਰੋਕੋ;
	  }
	  HB_FALLTHROUGH;

	ਹਾਲਤ Op_ਕੋਡ_hstemhm:
	ਹਾਲਤ Op_ਕੋਡ_vstemhm:
	ਹਾਲਤ Op_ਕੋਡ_hstem:
	ਹਾਲਤ Op_ਕੋਡ_vstem:
	  has_hint = ਸੱਚ;
	  values[pos].ਸੈੱਟ_hinting ();
	  ਜੇ (str.at_ਸਿਰਾ (pos))
	    drop.ਸਿਰੇ_ਵਿੱਚ_hint = ਸੱਚ;
	  ਰੋਕੋ;

	ਹਾਲਤ Op_ਕੋਡ_dotsection:
	  values[pos].ਸੈੱਟ_hinting ();
	  ਰੋਕੋ;

	ਮੂਲ:
	  /* NONE */
	  ਰੋਕੋ;
      }
      ਜੇ (has_hint)
      {
	ਲਈ (ਪੂਰਨ i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(ਬਿਨਾਨਿਸ਼ਾਨੀ)i];
	  ਜੇ (csop.is_hinting ())
	    ਰੋਕੋ;
	  csop.ਸੈੱਟ_hinting ();
	  ਜੇ (csop.op == Op_ਕੋਡ_vsindexcs)
	    drop.vsindex_dropped = ਸੱਚ;
	}
	ਵੇਖਿਆ_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = ਸੱਚ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pos = 0; pos < ਗਿਣਤੀ_2; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      ਜੇ (csop.op == Op_ਕੋਡ_return)
	ਰੋਕੋ;
      ਜੇ (!csop.is_hinting ())
      {
	drop.all_dropped = ਝੂਠ;
	ਰੋਕੋ;
      }
    }

    ਵਾਪਸ ਵੇਖਿਆ_hint;
  }

  ਤਾਰਕਿਕ ਘੇਰਾ_subroutines (ਅਚਲ parsed_cs_str_vec_t& global_subrs,
                            ਅਚਲ hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.ਪਹਿਲਾ;
      hb_codepoint_t old_glyph = _.second;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = subset_get_fd (acc, old_glyph);
      ਜੇ (unlikely (fd >= acc.fd_ਗਿਣਤੀ))
        ਵਾਪਸ ਝੂਠ;

      // Note: const cast is safe here because the collect_subr_refs_in_str only performs a
      //       closure and does not modify any of the charstrings.
      subr_subset_param_t  param (const_cast<parsed_cs_str_t*> (&get_parsed_charstring (new_glyph)),
                                  const_cast<parsed_cs_str_vec_t*> (&global_subrs),
                                  const_cast<parsed_cs_str_vec_t*> (&local_subrs[fd]),
                                  &closures.global_ਘੇਰਾ,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
      collect_subr_refs_ਵਿੱਚ_str (get_parsed_charstring (new_glyph), param);
    }

    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ collect_subr_refs_ਵਿੱਚ_subr (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ subr_ਅੰਕ_2, parsed_cs_str_vec_t &subrs,
				  hb_ਸੈੱਟ_t *ਘੇਰਾ,
				  ਅਚਲ subr_subset_param_t &param)
  {
    ਜੇ (ਘੇਰਾ->has (subr_ਅੰਕ_2))
      ਵਾਪਸ;
    ਘੇਰਾ->ਜੋੜੋ (subr_ਅੰਕ_2);
    collect_subr_refs_ਵਿੱਚ_str (subrs[subr_ਅੰਕ_2], param);
  }

  ਖਾਲੀ collect_subr_refs_ਵਿੱਚ_str (ਅਚਲ parsed_cs_str_t &str,
                                 ਅਚਲ subr_subset_param_t &param)
  {
    ਜੇ (!str.has_calls ())
      ਵਾਪਸ;

    ਲਈ (ਸਵੈਚਾਲਿਤ &opstr : str.values)
    {
      ਜੇ (!param.drop_hints || !opstr.is_hinting ())
      {
	ਚੋਣ (opstr.op)
	{
	  ਹਾਲਤ Op_ਕੋਡ_callsubr:
	    collect_subr_refs_ਵਿੱਚ_subr (opstr.subr_ਅੰਕ_2, *param.parsed_local_subrs,
				       param.local_ਘੇਰਾ_2, param);
	    ਰੋਕੋ;

	  ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	    collect_subr_refs_ਵਿੱਚ_subr (opstr.subr_ਅੰਕ_2, *param.parsed_global_subrs,
				       param.global_ਘੇਰਾ, param);
	    ਰੋਕੋ;

	  ਮੂਲ: ਰੋਕੋ;
	}
      }
    }
  }

  ਤਾਰਕਿਕ encode_str (ਅਚਲ parsed_cs_str_t &str, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd, str_buff_t &buff, ਤਾਰਕਿਕ encode_ਅਗੇਤਰ = ਸੱਚ) ਅਚਲ
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    ਤਾਰਕਿਕ hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    ਜੇ (encode_ਅਗੇਤਰ && str.has_ਅਗੇਤਰ_2 () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_ਅੰਕ_cs (str.ਅਗੇਤਰ_ਅੰਕ_2 ());
      ਜੇ (str.ਅਗੇਤਰ_op_2 () != Op_ਕੋਡ_Invalid)
	encoder.encode_op (str.ਅਗੇਤਰ_op_2 ());
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = 0;
    ਲਈ (ਸਵੈਚਾਲਿਤ &opstr : str.values)
    {
      ਮਾਪ += opstr.ਲੰਮਾਈ;
      ਜੇ (opstr.op == Op_ਕੋਡ_callsubr || opstr.op == Op_ਕੋਡ_callgsubr)
        ਮਾਪ += 3;
    }
    ਜੇ (!buff.alloc_exact (buff.ਲੰਮਾਈ + ਮਾਪ))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਸਵੈਚਾਲਿਤ &opstr : str.values)
    {
      ਜੇ (hinting || !opstr.is_hinting ())
      {
	ਚੋਣ (opstr.op)
	{
	  ਹਾਲਤ Op_ਕੋਡ_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_ਅੰਕ (opstr.subr_ਅੰਕ_2));
	    encoder.copy_str (opstr.ptr, opstr.ਲੰਮਾਈ);
	    ਰੋਕੋ;

	  ਹਾਲਤ Op_ਕੋਡ_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_ਅੰਕ (opstr.subr_ਅੰਕ_2));
	    encoder.copy_str (opstr.ptr, opstr.ਲੰਮਾਈ);
	    ਰੋਕੋ;

	  ਮੂਲ:
	    encoder.copy_str (opstr.ptr, opstr.ਲੰਮਾਈ);
	    ਰੋਕੋ;
	}
      }
    }
    ਵਾਪਸ !encoder.ਵਿੱਚ_error ();
  }

  ਖਾਲੀ compact_parsed_subrs () ਅਚਲ
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &cs : parsed_global_subrs_storage)
      cs.compact ();
    ਲਈ (ਸਵੈਚਾਲਿਤ &vec : parsed_local_subrs_storage)
      ਲਈ (ਸਵੈਚਾਲਿਤ &cs : vec)
	cs.compact ();
  }

  ਖਾਲੀ populate_subset_accelerator () ਅਚਲ
  {
    ਜੇ (!plan->inprogress_accelerator) ਵਾਪਸ;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ਅਚਲ parsed_cs_str_t& get_parsed_charstring (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਜੇ (cached_charstrings) ਵਾਪਸ *(cached_charstrings[i]);
    ਵਾਪਸ parsed_charstrings[i];
  }

  ਸੁਰੱਖਿਅਤ:
  ਅਚਲ ACC			&acc;
  ਅਚਲ hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_ਸਦਿਸ਼_t<ਅਚਲ parsed_cs_str_t*>     cached_charstrings;
  ਅਚਲ parsed_cs_str_vec_t*              parsed_global_subrs;
  ਅਚਲ hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  ਨਿਜੀ:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_ਸਦਿਸ਼_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ SUBRS::ਗਿਣਤੀ_type subr_ਗਿਣਤੀ_type;
};

} /* namespace CFF */

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_plan_subset_cff_fdselect (ਅਚਲ hb_subset_plan_t *plan,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ,
			    ਅਚਲ CFF::FDSelect &src, /*ਵਿੱਚ*/
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ src_ਅੰਕ_glyphs,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fd_ਗਿਣਤੀ /*ਬਾਹਰਲਾ*/,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fdselect_ਮਾਪ /*ਬਾਹਰਲਾ*/,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &subset_fdselect_format /*ਬਾਹਰਲਾ*/,
			    hb_ਸਦਿਸ਼_t<CFF::ਕੋਡ_pair_t> &fdselect_ranges /*ਬਾਹਰਲਾ*/,
			    hb_inc_bimap_t &fdmap /*ਬਾਹਰਲਾ*/);

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs,
			  ਅਚਲ CFF::FDSelect &src,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd_ਗਿਣਤੀ_2,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdselect_format,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ,
			  ਅਚਲ hb_ਸਦਿਸ਼_t<CFF::ਕੋਡ_pair_t> &fdselect_ranges);

#ਜੇ_ਅੰਤ /* HB_SUBSET_CFF_COMMON_HH */
