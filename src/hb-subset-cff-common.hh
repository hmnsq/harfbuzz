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

#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_CFF_COMMON_HH
#وصف_ڪريو HB_SUBSET_CFF_COMMON_HH

#ضمن "hb.hh"

#ضمن "hb-subset-plan.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
سانچو <قسم_نالو ACC>
ساكن مضمن بنانشان
subset_get_fd (ثابت ACC &acc, hb_codepoint_t glyph)
{
  أرجع likely (glyph < acc.انگ_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
بنية str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  فراغ reset () { buff.reset (); }

  فراغ encode_byte (بنانشان حرف b)
  {
    إذا (likely ((نشانوارو) buff.ڊيگهائي < buff.allocated))
      buff.arrayZ[buff.ڊيگهائي++] = b;
    وإلا
      buff.push (b);
  }

  فراغ encode_int (صحيح v)
  {
    إذا ((-1131 <= v) && (v <= 1131))
    {
      إذا ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      وإلا إذا (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + Op_ڪوڊ_Two_Byte_Pos_Int0);
	encode_byte (v & 0xFF);
      }
      وإلا
      {
	v = -v - 108;
	encode_byte ((v >> 8) + Op_ڪوڊ_Two_Byte_Neg_Int0);
	encode_byte (v & 0xFF);
      }
    }
    وإلا
    {
      إذا (unlikely (v < -32768))
	v = -32768;
      وإلا إذا (unlikely (v > 32767))
	v = 32767;
      encode_byte (Op_ڪوڊ_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  فراغ encode_انگ_cs (ثابت عدد_t& n)
  {
    إذا (n.اندريون_int_range ())
    {
      encode_int (n.منزل_int ());
    }
    وإلا
    {
      int32_t v = n.منزل_fixed ();
      encode_byte (Op_ڪوڊ_fixedcs);
      encode_byte ((v >> 24) & 0xFF);
      encode_byte ((v >> 16) & 0xFF);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for TopDict / Private
  فراغ encode_انگ_tp (ثابت عدد_t& n)
  {
    إذا (n.اندريون_int_range ())
    {
      // TODO longint
      encode_int (n.منزل_int ());
    }
    وإلا
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      مزدوج v = n.منزل_real ();
      encode_byte (Op_ڪوڊ_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      حرف buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_اڻ_استعمال_ٿيل;
      hb_locale_t oldlocale HB_اڻ_استعمال_ٿيل;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", عدم));
      snprintf (buf, حجم (buf), "%.8G", v);
      (فراغ) hb_uselocale (((فراغ) freelocale (clocale), oldlocale));

      حرف *s = buf;
      size_t len;
      حرف *comma = strchr (s, ',');
      إذا (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      إذا (s[0] == '0' && s[1] == '.')
	s++;
      وإلا إذا (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      وإلا إذا ((len = لڙي_ڊيگهه (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	بنانشان exponent = len - 3;
	حرف *s2 = s + exponent - 1;
	طالما (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, حجم (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      وإلا
      {
	حرف *dot = strchr (s, '.');
	حرف *e = strchr (s, 'E');
	إذا (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  صحيح exponent = atoi (e + 1);
	  صحيح new_exponent = exponent - (e - (dot + 1));
	  إذا (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  وإلا
	    snprintf (e - 1, حجم (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      إذا ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	صحيح sign = s[0] == '-';
	حرف *s2 = s + sign + 1;
	طالما (*s2 == '0')
	  s2++;
	len = لڙي_ڊيگهه (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, حجم (buf) - (s + sign + len - buf), "E-%u", (بنانشان) (لڙي_ڊيگهه (s + sign) - 1));
      }
      hb_ويڪٽر_t<حرف> nibbles;
      طالما (*s)
      {
	حرف c = s[0];
	s++;

	بدل (c)
	{
	  حالة 'E':
	  {
	    حرف c2 = *s;
	    إذا (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } وإلا {
	      إذا (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    إذا (*s == '0')
	      s++;
	    استمر;
	  }

	  حالة '.':
	    nibbles.push (0x0A); // .
	    استمر;

	  حالة '-':
	    nibbles.push (0x0E); // -
	    استمر;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      إذا (nibbles.ڊيگهائي % 2)
	nibbles.push (0x0F);

      بنانشان ڳڻپ = nibbles.ڊيگهائي;
      لكل (بنانشان i = 0; i < ڳڻپ; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  فراغ encode_op (op_ڪوڊ_t op)
  {
    إذا (Is_Op_ڪوڊ_ESC (op))
    {
      encode_byte (Op_ڪوڊ_escape);
      encode_byte (Unmake_Op_ڪوڊ_ESC (op));
    }
    وإلا
      encode_byte (op);
  }

  فراغ copy_str (ثابت بنانشان حرف *str, بنانشان ڊيگهائي)
  {
    أكد ((نشانوارو) (buff.ڊيگهائي + ڊيگهائي) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.ڊيگهائي, str, ڊيگهائي);
    buff.ڊيگهائي += ڊيگهائي;
  }

  منطقي اندريون_error () ثابت { أرجع buff.اندريون_error (); }

  محمي:

  str_buff_t &buff;
};

بنية cff_sub_جدول_info_t {
  cff_sub_جدول_info_t ()
    : fd_array_link (0),
      char_لڙيون_link (0)
  {
    fd_select.init ();
  }

  جدول_info_t     fd_select;
  objidx_t     	   fd_array_link;
  objidx_t     	   char_لڙيون_link;
};

سانچو <قسم_نالو OPSTR=op_str_t>
بنية cff_چوٽي_dict_op_serializer_t : op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت OPSTR &opstr,
		  ثابت cff_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    بدل (opstr.op)
    {
      حالة Op_ڪوڊ_Char_لڙيون:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_لڙيون_link, whence_t::Absolute));

      حالة Op_ڪوڊ_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      حالة Op_ڪوڊ_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      افتراضي:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (سچ);
  }
};

بنية cff_font_dict_op_serializer_t : op_serializer_t
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت جدول_info_t &ذاتي_Dict_Info) ثابت
  {
    TRACE_SERIALIZE (هذا);

    إذا (opstr.op == Op_ڪوڊ_ذاتي)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, ذاتي_Dict_Info.سائيز) &&
		    Dict::serialize_link4_op (c, opstr.op, ذاتي_Dict_Info.link, whence_t::Absolute));
    }
    وإلا
    {
      بنانشان حرف *d = c->allocate_سائيز<بنانشان حرف> (opstr.ڊيگهائي);
      إذا (unlikely (!d)) return_trace (خطأ);
      /* Faster than hb_memcpy for small strings. */
      لكل (بنانشان i = 0; i < opstr.ڊيگهائي; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (سچ);
  }
};

/* CharString command for specialization */
بنية cs_command_t
{
  hb_ويڪٽر_t<عدد_t> دليلون;
  hb_ويڪٽر_t<بنانشان حرف> نقاب_بائيٽون; /* For hintmask/cntrmask payload bytes. */
  op_ڪوڊ_t op;

  cs_command_t () : op (Op_ڪوڊ_Invalid) {}
  cs_command_t (op_ڪوڊ_t op_) : op (op_) {}
};

تعريفنوع hb_ويڪٽر_t<cs_command_t> *cs_command_vec_t;

بنية cff2_instancing_plan_t;

بنية flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   منطقي drop_hints_,
                   ثابت hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = خالي_اشارو)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  منطقي	drop_hints;
  ثابت hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ثابت cff2_instancing_plan_t *instancer = خالي_اشارو;
  منطقي emitted_blend = خطأ;
};

سانچو <قسم_نالو ACC, قسم_نالو ENV, قسم_نالو OPSET, op_ڪوڊ_t endchar_op=Op_ڪوڊ_Invalid>
بنية subr_flattener_t
{
  subr_flattener_t (ثابت ACC &acc_,
		    ثابت hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  منطقي flatten (str_buff_vec_t &flat_charstrings,
                hb_ويڪٽر_t<hb_ويڪٽر_t<cs_command_t>> *command_capture = خالي_اشارو)
  {
    بنانشان ڳڻپ = plan->انگ_output_glyphs ();
    إذا (!flat_charstrings.resize_exact (ڳڻپ))
      أرجع خطأ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      hb_codepoint_t  glyph;
      إذا (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	إذا (endchar_op != Op_ڪوڊ_Invalid) flat_charstrings[i].push (endchar_op);
	استمر;
      }
      ثابت hb_ubytes_t str = (*acc.char_لڙيون)[glyph];
      بنانشان صحيح fd = subset_get_fd (acc, glyph);
      إذا (unlikely (fd >= acc.fd_ڳڻپ))
	أرجع خطأ;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.ڊيگهائي);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (منطقي) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : خالي_اشارو
      };
      إذا (unlikely (!interp.interpret (param)))
	أرجع خطأ;
    }
    أرجع سچ;
  }

  ثابت ACC &acc;
  ثابت hb_subset_plan_t *plan;
};

بنية subr_closures_t
{
  subr_closures_t (بنانشان صحيح fd_ڳڻپ_2) : global_گهيرو (), local_closures ()
  {
    local_closures.resize_exact (fd_ڳڻپ_2);
  }

  فراغ reset ()
  {
    global_گهيرو.clear();
    لكل (بنانشان صحيح i = 0; i < local_closures.ڊيگهائي; i++)
      local_closures[i].clear();
  }

  منطقي اندريون_error () ثابت { أرجع local_closures.اندريون_error (); }
  hb_سيٽ_t  global_گهيرو;
  hb_ويڪٽر_t<hb_سيٽ_t> local_closures;
};

بنية parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (بنانشان صحيح subr_انگ = 0) :
    subr_انگ_2 (subr_انگ) {}

  منطقي is_hinting () ثابت { أرجع hinting_flag; }
  فراغ سيٽ_hinting ()       { hinting_flag = سچ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  محمي:
  منطقي	  hinting_flag = خطأ;

  عام:
  uint16_t subr_انگ_2;
};

بنية parsed_cs_str_t : parsed_قدرون_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (خطأ),
    hint_dropped (خطأ),
    has_اڳياڙي (خطأ),
    has_calls_ (خطأ),
    coalescing_ (خطأ)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  فراغ جوڙيو_op (op_ڪوڊ_t op, ثابت byte_str_ref_t& str_ref)
  {
    إذا (is_parsed ()) أرجع;
    إذا (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_پڇاڙي = آخري_پڇاڙي_2;
      آخري_پڇاڙي_2 = str_ref.get_وٿي ();
      إذا (unlikely (op == Op_ڪوڊ_return || op == Op_ڪوڊ_endchar))
	flush_segment (str_ref, آخري_پڇاڙي_2);
      أرجع;
    }
    SUPER::جوڙيو_op (op, str_ref);
  }

  فراغ جوڙيو_call_op (op_ڪوڊ_t op, ثابت byte_str_ref_t& str_ref, بنانشان صحيح subr_انگ_2)
  {
    إذا (is_parsed ()) أرجع;
    has_calls_ = سچ;

    إذا (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_پڇاڙي);
      op_شروعات = آخري_پڇاڙي_2;
      SUPER::جوڙيو_op (op, str_ref, {subr_انگ_2});
      penultimate_پڇاڙي = آخري_پڇاڙي_2 = str_ref.get_وٿي ();
      أرجع;
    }

    /* Pop the subroutine number. */
    values.ڪڍو ();

    SUPER::جوڙيو_op (op, str_ref, {subr_انگ_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  فراغ flush_segment (ثابت byte_str_ref_t& str_ref, بنانشان پڇاڙي)
  {
    بنانشان شروعات = op_شروعات;
    إذا (پڇاڙي <= شروعات) أرجع;
    طالما (شروعات < پڇاڙي)
    {
      خودڪار arr = str_ref.sub_array (شروعات, hb_min (پڇاڙي - شروعات, 255u));
      إذا (unlikely (!arr.ڊيگهائي)) اكسر;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->ڊيگهائي = arr.ڊيگهائي;
      شروعات += arr.ڊيگهائي;
    }
    op_شروعات = پڇاڙي;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  فراغ flush_coalesced (ثابت byte_str_ref_t& str_ref)
  {
    إذا (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_وٿي ());
  }

  فراغ enable_coalescing () { coalescing_ = سچ; }
  منطقي is_coalescing () ثابت { أرجع coalescing_; }

  فراغ سيٽ_اڳياڙي (ثابت عدد_t &انگ, op_ڪوڊ_t op = Op_ڪوڊ_Invalid)
  {
    has_اڳياڙي = سچ;
    اڳياڙي_op = op;
    اڳياڙي_انگ = انگ;
  }

  منطقي at_پڇاڙي (بنانشان صحيح pos) ثابت
  {
    أرجع ((pos + 1 >= values.ڊيگهائي) /* CFF2 */
	|| (values[pos + 1].op == Op_ڪوڊ_return));
  }

  منطقي is_parsed () ثابت { أرجع parsed; }
  فراغ سيٽ_parsed ()      { parsed = سچ; }

  منطقي is_hint_dropped () ثابت { أرجع hint_dropped; }
  فراغ سيٽ_hint_dropped ()      { hint_dropped = سچ; }

  منطقي is_vsindex_dropped () ثابت { أرجع vsindex_dropped; }
  فراغ سيٽ_vsindex_dropped ()      { vsindex_dropped = سچ; }

  منطقي has_اڳياڙي_2 () ثابت          { أرجع has_اڳياڙي; }
  op_ڪوڊ_t اڳياڙي_op_2 () ثابت         { أرجع اڳياڙي_op; }
  ثابت عدد_t &اڳياڙي_انگ_2 () ثابت { أرجع اڳياڙي_انگ; }

  منطقي has_calls () ثابت          { أرجع has_calls_; }

  فراغ compact ()
  {
    بنانشان ڳڻپ = values.ڊيگهائي;
    إذا (!ڳڻپ) أرجع;
    خودڪار &opstr = values.arrayZ;
    بنانشان j = 0;
    لكل (بنانشان i = 1; i < ڳڻپ; i++)
    {
      /* See if we can combine op j and op i. */
      منطقي combine =
        (opstr[j].op != Op_ڪوڊ_callsubr && opstr[j].op != Op_ڪوڊ_callgsubr) &&
        (opstr[i].op != Op_ڪوڊ_callsubr && opstr[i].op != Op_ڪوڊ_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].ڊيگهائي == opstr[i].ptr) &&
        (opstr[j].ڊيگهائي + opstr[i].ڊيگهائي <= 255);

      إذا (combine)
      {
	opstr[j].ڊيگهائي += opstr[i].ڊيگهائي;
	opstr[j].op = Op_ڪوڊ_Invalid;
      }
      وإلا
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  محمي:
  منطقي    parsed : 1;
  منطقي    hint_dropped : 1;
  منطقي    vsindex_dropped : 1;
  منطقي    has_اڳياڙي : 1;
  منطقي    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  منطقي    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  بنانشان penultimate_پڇاڙي = 0;
  بنانشان آخري_پڇاڙي_2 = 0;
  op_ڪوڊ_t	اڳياڙي_op;
  عدد_t	اڳياڙي_انگ;

  خاص:
  تعريفنوع parsed_قدرون_t<parsed_cs_op_t> SUPER;
};

بنية parsed_cs_str_vec_t : hb_ويڪٽر_t<parsed_cs_str_t>
{
  خاص:
  تعريفنوع hb_ويڪٽر_t<parsed_cs_str_t> SUPER;
};

بنية cff_subset_accelerator_t
{
  ساكن cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ثابت parsed_cs_str_vec_t& parsed_charstrings,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs,
      ثابت hb_ويڪٽر_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (حجم(cff_subset_accelerator_t));
    إذا (unlikely (!accel)) أرجع خالي_اشارو;
    جديد (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    أرجع accel;
  }

  ساكن فراغ destroy (فراغ* value) {
    إذا (!value) أرجع;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ثابت parsed_cs_str_vec_t& parsed_charstrings_,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs_,
      ثابت hb_ويڪٽر_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    خودڪار *mapping = glyph_منزل_sid_map.get_relaxed ();
    إذا (mapping)
    {
      mapping->~glyph_منزل_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_ويڪٽر_t<parsed_cs_str_vec_t> parsed_local_subrs;
  بدلجندڙ hb_atomic_t<glyph_منزل_sid_map_t *> glyph_منزل_sid_map;

 خاص:
  hb_blob_t* original_blob;
};

بنية subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_سيٽ_t *global_گهيرو_2,
		       hb_سيٽ_t *local_گهيرو,
		       منطقي drop_hints_,
		       منطقي coalesce_ = خطأ) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_گهيرو (global_گهيرو_2),
      local_گهيرو_2 (local_گهيرو),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    إذا (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    بدل (context.type)
    {
      حالة CSType_CharString:
	أرجع parsed_charstring;

      حالة CSType_LocalSubr:
	إذا (likely (context.subr_انگ_2 < parsed_local_subrs->ڊيگهائي))
	  أرجع &(*parsed_local_subrs)[context.subr_انگ_2];
	اكسر;

      حالة CSType_GlobalSubr:
	إذا (likely (context.subr_انگ_2 < parsed_global_subrs->ڊيگهائي))
	  أرجع &(*parsed_global_subrs)[context.subr_انگ_2];
	اكسر;
    }
    أرجع خالي_اشارو;
  }

  سانچو <قسم_نالو ENV>
  فراغ سيٽ_current_str (ENV &env, منطقي calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    إذا (unlikely (!parsed_str))
    {
      env.سيٽ_error ();
      أرجع;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    إذا (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.ڊيگهائي > 0)))
      env.سيٽ_error ();
    وإلا
    {
      إذا (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.ڪل_سائيز ());
        إذا (coalesce) parsed_str->enable_coalescing ();
      }
      current_parsed_str = parsed_str;
    }
  }

  parsed_cs_str_t	*current_parsed_str;

  parsed_cs_str_t	*parsed_charstring;
  parsed_cs_str_vec_t	*parsed_global_subrs;
  parsed_cs_str_vec_t	*parsed_local_subrs;
  hb_سيٽ_t      *global_گهيرو;
  hb_سيٽ_t      *local_گهيرو_2;
  منطقي	  drop_hints;
  منطقي	  coalesce;
};

بنية subr_remap_t : hb_inc_bimap_t
{
  فراغ create (ثابت hb_سيٽ_t *گهيرو)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (گهيرو->get_population ());
    لكل (خودڪار old_انگ : *گهيرو)
      جوڙيو (old_انگ);

    إذا (get_population () < 1240)
      bias = 107;
    وإلا إذا (get_population () < 33900)
      bias = 1131;
    وإلا
      bias = 32768;
  }

  صحيح biased_انگ (بنانشان صحيح old_انگ) ثابت
  {
    hb_codepoint_t new_انگ = get (old_انگ);
    أرجع (صحيح)new_انگ - bias;
  }

  محمي:
  صحيح bias;
};

بنية subr_remaps_t
{
  subr_remaps_t (بنانشان صحيح fd_ڳڻپ)
  {
    local_remaps.resize (fd_ڳڻپ);
  }

  منطقي اندريون_error()
  {
    أرجع local_remaps.اندريون_error ();
  }

  فراغ create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_گهيرو);
    لكل (بنانشان صحيح i = 0; i < local_remaps.ڊيگهائي; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_ويڪٽر_t<subr_remap_t>  local_remaps;
};

سانچو <قسم_نالو SUBSETTER, قسم_نالو SUBRS, قسم_نالو ACC, قسم_نالو ENV, قسم_نالو OPSET, op_ڪوڊ_t endchar_op=Op_ڪوڊ_Invalid>
بنية subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ثابت hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_ڳڻپ),
        remaps(acc_.fd_ڳڻپ)
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
  منطقي subset (فراغ)
  {
    بنانشان fd_ڳڻپ_2 = acc.fd_ڳڻپ;
    ثابت cff_subset_accelerator_t* cff_accelerator = خالي_اشارو;
    إذا (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_ڳڻپ_2 = cff_accelerator->parsed_local_subrs.ڊيگهائي;
    }

    إذا (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->انگ_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } وإلا {
      parsed_charstrings.resize_exact (plan->انگ_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->ڳڻپ);

      إذا (unlikely (!parsed_local_subrs_storage.resize (fd_ڳڻپ_2))) أرجع خطأ;

      لكل (بنانشان صحيح i = 0; i < acc.fd_ڳڻپ; i++)
      {
        بنانشان ڳڻپ = acc.ذاتي_Dicts[i].localSubrs->ڳڻپ;
        parsed_local_subrs_storage[i].resize (ڳڻپ);
        إذا (unlikely (parsed_local_subrs_storage[i].اندريون_error ())) أرجع خطأ;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    إذا (unlikely (remaps.اندريون_error()
                  || cached_charstrings.اندريون_error ()
                  || parsed_charstrings.اندريون_error ()
                  || parsed_global_subrs->اندريون_error ()
                  || closures.اندريون_error ())) {
      أرجع خطأ;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    منطقي coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    لكل (خودڪار _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.پهريون;
      hb_codepoint_t old_glyph = _.second;

      ثابت hb_ubytes_t str = (*acc.char_لڙيون)[old_glyph];
      بنانشان صحيح fd = subset_get_fd (acc, old_glyph);
      إذا (unlikely (fd >= acc.fd_ڳڻپ))
        أرجع خطأ;

      إذا (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        إذا (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        وإلا
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        استمر;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.ڊيگهائي);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_گهيرو,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      إذا (unlikely (!interp.interpret (param)))
        أرجع خطأ;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      إذا ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_گهيرو,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	إذا (drop_hints_اندريون_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].سيٽ_hint_dropped ();
	  إذا (drop.vsindex_dropped)
	    parsed_charstrings[new_glyph].سيٽ_vsindex_dropped ();
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
      إذا (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    إذا ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !گهيرو_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      أرجع خطأ;

    remaps.create (closures);

    populate_subset_accelerator ();
    أرجع سچ;
  }

  منطقي encode_charstrings (str_buff_vec_t &buffArray, منطقي encode_اڳياڙي = سچ) ثابت
  {
    بنانشان انگ_glyphs = plan->انگ_output_glyphs ();
    إذا (unlikely (!buffArray.resize_exact (انگ_glyphs)))
      أرجع خطأ;
    hb_codepoint_t آخري = 0;
    لكل (خودڪار _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t gid = _.پهريون;
      hb_codepoint_t old_glyph = _.second;

      إذا (endchar_op != Op_ڪوڊ_Invalid)
        لكل (; آخري < gid; آخري++)
	{
	  // Hack to point vector to static string.
	  خودڪار &b = buffArray.arrayZ[آخري];
	  b.سيٽ_storage (const_cast<بنانشان حرف *>(endchar_str), 1);
	}

      آخري++; // Skip over gid
      بنانشان صحيح  fd = subset_get_fd (acc, old_glyph);
      إذا (unlikely (fd >= acc.fd_ڳڻپ))
	أرجع خطأ;
      إذا (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_اڳياڙي)))
	أرجع خطأ;
    }
    إذا (endchar_op != Op_ڪوڊ_Invalid)
      لكل (; آخري < انگ_glyphs; آخري++)
      {
	// Hack to point vector to static string.
	خودڪار &b = buffArray.arrayZ[آخري];
	b.سيٽ_storage (const_cast<بنانشان حرف *>(endchar_str), 1);
      }

    أرجع سچ;
  }

  منطقي encode_subrs (ثابت parsed_cs_str_vec_t &subrs, ثابت subr_remap_t& remap, بنانشان صحيح fd, str_buff_vec_t &buffArray) ثابت
  {
    بنانشان صحيح  ڳڻپ = remap.get_population ();

    إذا (unlikely (!buffArray.resize_exact (ڳڻپ)))
      أرجع خطأ;
    لكل (بنانشان صحيح new_انگ = 0; new_انگ < ڳڻپ; new_انگ++)
    {
      hb_codepoint_t old_انگ = remap.backward (new_انگ);
      أكد (old_انگ != CFF_UNDEF_ڪوڊ);

      إذا (unlikely (!encode_str (subrs[old_انگ], fd, buffArray[new_انگ])))
	أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    أرجع encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  منطقي encode_localsubrs (بنانشان صحيح fd, str_buff_vec_t &buffArray) ثابت
  {
    أرجع encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  محمي:
  بنية drop_hints_param_t
  {
    drop_hints_param_t ()
      : ڏٺل_moveto (خطأ),
	پڇاڙيون_اندريون_hint (خطأ),
	all_dropped (خطأ),
	vsindex_dropped (خطأ) {}

    منطقي  ڏٺل_moveto;
    منطقي  پڇاڙيون_اندريون_hint;
    منطقي  all_dropped;
    منطقي  vsindex_dropped;
  };

  منطقي drop_hints_اندريون_subr (parsed_cs_str_t &str, بنانشان صحيح pos,
			   parsed_cs_str_vec_t &subrs, بنانشان صحيح subr_انگ_2,
			   ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.پڇاڙيون_اندريون_hint = خطأ;
    منطقي has_hint = drop_hints_اندريون_str (subrs[subr_انگ_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    إذا (drop.پڇاڙيون_اندريون_hint)
    {
      str.values[pos].سيٽ_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      إذا (!str.at_پڇاڙي (pos))
	drop.پڇاڙيون_اندريون_hint = خطأ;
    }
    وإلا إذا (drop.all_dropped)
    {
      str.values[pos].سيٽ_hinting ();
    }

    أرجع has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  منطقي drop_hints_اندريون_str (parsed_cs_str_t &str, ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    منطقي  ڏٺل_hint = خطأ;

    بنانشان ڳڻپ = str.values.ڊيگهائي;
    خودڪار *values = str.values.arrayZ;
    لكل (بنانشان صحيح pos = 0; pos < ڳڻپ; pos++)
    {
      منطقي  has_hint = خطأ;
      بدل (values[pos].op)
      {
	حالة Op_ڪوڊ_callsubr:
	  has_hint = drop_hints_اندريون_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_انگ_2,
					param, drop);
	  اكسر;

	حالة Op_ڪوڊ_callgsubr:
	  has_hint = drop_hints_اندريون_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_انگ_2,
					param, drop);
	  اكسر;

	حالة Op_ڪوڊ_rmoveto:
	حالة Op_ڪوڊ_hmoveto:
	حالة Op_ڪوڊ_vmoveto:
	  drop.ڏٺل_moveto = سچ;
	  اكسر;

	حالة Op_ڪوڊ_hintmask:
	حالة Op_ڪوڊ_cntrmask:
	  إذا (drop.ڏٺل_moveto)
	  {
	    values[pos].سيٽ_hinting ();
	    اكسر;
	  }
	  HB_FALLTHROUGH;

	حالة Op_ڪوڊ_hstemhm:
	حالة Op_ڪوڊ_vstemhm:
	حالة Op_ڪوڊ_hstem:
	حالة Op_ڪوڊ_vstem:
	  has_hint = سچ;
	  values[pos].سيٽ_hinting ();
	  إذا (str.at_پڇاڙي (pos))
	    drop.پڇاڙيون_اندريون_hint = سچ;
	  اكسر;

	حالة Op_ڪوڊ_dotsection:
	  values[pos].سيٽ_hinting ();
	  اكسر;

	افتراضي:
	  /* NONE */
	  اكسر;
      }
      إذا (has_hint)
      {
	لكل (صحيح i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(بنانشان)i];
	  إذا (csop.is_hinting ())
	    اكسر;
	  csop.سيٽ_hinting ();
	  إذا (csop.op == Op_ڪوڊ_vsindexcs)
	    drop.vsindex_dropped = سچ;
	}
	ڏٺل_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = سچ;
    لكل (بنانشان صحيح pos = 0; pos < ڳڻپ; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      إذا (csop.op == Op_ڪوڊ_return)
	اكسر;
      إذا (!csop.is_hinting ())
      {
	drop.all_dropped = خطأ;
	اكسر;
      }
    }

    أرجع ڏٺل_hint;
  }

  منطقي گهيرو_subroutines (ثابت parsed_cs_str_vec_t& global_subrs,
                            ثابت hb_ويڪٽر_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    لكل (خودڪار _ : plan->new_منزل_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.پهريون;
      hb_codepoint_t old_glyph = _.second;
      بنانشان صحيح fd = subset_get_fd (acc, old_glyph);
      إذا (unlikely (fd >= acc.fd_ڳڻپ))
        أرجع خطأ;

      // Note: const cast is safe here because the collect_subr_refs_in_str only performs a
      //       closure and does not modify any of the charstrings.
      subr_subset_param_t  param (const_cast<parsed_cs_str_t*> (&get_parsed_charstring (new_glyph)),
                                  const_cast<parsed_cs_str_vec_t*> (&global_subrs),
                                  const_cast<parsed_cs_str_vec_t*> (&local_subrs[fd]),
                                  &closures.global_گهيرو,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
      collect_subr_refs_اندريون_str (get_parsed_charstring (new_glyph), param);
    }

    أرجع سچ;
  }

  فراغ collect_subr_refs_اندريون_subr (بنانشان صحيح subr_انگ_2, parsed_cs_str_vec_t &subrs,
				  hb_سيٽ_t *گهيرو,
				  ثابت subr_subset_param_t &param)
  {
    إذا (گهيرو->has (subr_انگ_2))
      أرجع;
    گهيرو->جوڙيو (subr_انگ_2);
    collect_subr_refs_اندريون_str (subrs[subr_انگ_2], param);
  }

  فراغ collect_subr_refs_اندريون_str (ثابت parsed_cs_str_t &str,
                                 ثابت subr_subset_param_t &param)
  {
    إذا (!str.has_calls ())
      أرجع;

    لكل (خودڪار &opstr : str.values)
    {
      إذا (!param.drop_hints || !opstr.is_hinting ())
      {
	بدل (opstr.op)
	{
	  حالة Op_ڪوڊ_callsubr:
	    collect_subr_refs_اندريون_subr (opstr.subr_انگ_2, *param.parsed_local_subrs,
				       param.local_گهيرو_2, param);
	    اكسر;

	  حالة Op_ڪوڊ_callgsubr:
	    collect_subr_refs_اندريون_subr (opstr.subr_انگ_2, *param.parsed_global_subrs,
				       param.global_گهيرو, param);
	    اكسر;

	  افتراضي: اكسر;
	}
      }
    }
  }

  منطقي encode_str (ثابت parsed_cs_str_t &str, ثابت بنانشان صحيح fd, str_buff_t &buff, منطقي encode_اڳياڙي = سچ) ثابت
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    منطقي hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    إذا (encode_اڳياڙي && str.has_اڳياڙي_2 () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_انگ_cs (str.اڳياڙي_انگ_2 ());
      إذا (str.اڳياڙي_op_2 () != Op_ڪوڊ_Invalid)
	encoder.encode_op (str.اڳياڙي_op_2 ());
    }

    بنانشان سائيز = 0;
    لكل (خودڪار &opstr : str.values)
    {
      سائيز += opstr.ڊيگهائي;
      إذا (opstr.op == Op_ڪوڊ_callsubr || opstr.op == Op_ڪوڊ_callgsubr)
        سائيز += 3;
    }
    إذا (!buff.alloc_exact (buff.ڊيگهائي + سائيز))
      أرجع خطأ;

    لكل (خودڪار &opstr : str.values)
    {
      إذا (hinting || !opstr.is_hinting ())
      {
	بدل (opstr.op)
	{
	  حالة Op_ڪوڊ_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_انگ (opstr.subr_انگ_2));
	    encoder.copy_str (opstr.ptr, opstr.ڊيگهائي);
	    اكسر;

	  حالة Op_ڪوڊ_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_انگ (opstr.subr_انگ_2));
	    encoder.copy_str (opstr.ptr, opstr.ڊيگهائي);
	    اكسر;

	  افتراضي:
	    encoder.copy_str (opstr.ptr, opstr.ڊيگهائي);
	    اكسر;
	}
      }
    }
    أرجع !encoder.اندريون_error ();
  }

  فراغ compact_parsed_subrs () ثابت
  {
    لكل (خودڪار &cs : parsed_global_subrs_storage)
      cs.compact ();
    لكل (خودڪار &vec : parsed_local_subrs_storage)
      لكل (خودڪار &cs : vec)
	cs.compact ();
  }

  فراغ populate_subset_accelerator () ثابت
  {
    إذا (!plan->inprogress_accelerator) أرجع;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ثابت parsed_cs_str_t& get_parsed_charstring (بنانشان i) ثابت
  {
    إذا (cached_charstrings) أرجع *(cached_charstrings[i]);
    أرجع parsed_charstrings[i];
  }

  محمي:
  ثابت ACC			&acc;
  ثابت hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_ويڪٽر_t<ثابت parsed_cs_str_t*>     cached_charstrings;
  ثابت parsed_cs_str_vec_t*              parsed_global_subrs;
  ثابت hb_ويڪٽر_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  خاص:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_ويڪٽر_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  تعريفنوع قسم_نالو SUBRS::ڳڻپ_type subr_ڳڻپ_type;
};

} /* namespace CFF */

HB_اندروني منطقي
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			    بنانشان صحيح fd_ڳڻپ,
			    ثابت CFF::FDSelect &src, /*اندريون*/
			    بنانشان صحيح src_انگ_glyphs,
			    بنانشان صحيح &subset_fd_ڳڻپ /*ٻاهريون*/,
			    بنانشان صحيح &subset_fdselect_سائيز /*ٻاهريون*/,
			    بنانشان صحيح &subset_fdselect_format /*ٻاهريون*/,
			    hb_ويڪٽر_t<CFF::ڪوڊ_pair_t> &fdselect_ranges /*ٻاهريون*/,
			    hb_inc_bimap_t &fdmap /*ٻاهريون*/);

HB_اندروني منطقي
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  بنانشان صحيح انگ_glyphs,
			  ثابت CFF::FDSelect &src,
			  بنانشان صحيح fd_ڳڻپ_2,
			  بنانشان صحيح fdselect_format,
			  بنانشان صحيح سائيز,
			  ثابت hb_ويڪٽر_t<CFF::ڪوڊ_pair_t> &fdselect_ranges);

#جيڪڏهن_پورو /* HB_SUBSET_CFF_COMMON_HH */
