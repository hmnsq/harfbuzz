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

#اگر_غیر_تعریف_شدہ HB_SUBSET_CFF_COMMON_HH
#تعریف_کرو HB_SUBSET_CFF_COMMON_HH

#شامل "hb.hh"

#شامل "hb-subset-plan.hh"
#شامل "hb-cff-interp-cs-common.hh"

نامخلا CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
سانچہ <قسم_نام ACC>
ساکن اندرونی غیرعلامتی
subset_get_fd (ثابت ACC &acc, hb_codepoint_t glyph)
{
  واپس likely (glyph < acc.عدد_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
ساخت str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  خالی reset () { buff.reset (); }

  خالی encode_byte (غیرعلامتی حرف b)
  {
    اگر (likely ((علامتی) buff.لمبائی < buff.allocated))
      buff.arrayZ[buff.لمبائی++] = b;
    ورنہ
      buff.push (b);
  }

  خالی encode_int (صحیح v)
  {
    اگر ((-1131 <= v) && (v <= 1131))
    {
      اگر ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      ورنہ اگر (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + Op_رمز_Two_Byte_Pos_Int0);
	encode_byte (v & 0xFF);
      }
      ورنہ
      {
	v = -v - 108;
	encode_byte ((v >> 8) + Op_رمز_Two_Byte_Neg_Int0);
	encode_byte (v & 0xFF);
      }
    }
    ورنہ
    {
      اگر (unlikely (v < -32768))
	v = -32768;
      ورنہ اگر (unlikely (v > 32767))
	v = 32767;
      encode_byte (Op_رمز_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  خالی encode_عدد_cs (ثابت نمبر_t& n)
  {
    اگر (n.اندر_int_range ())
    {
      encode_int (n.مقصد_int ());
    }
    ورنہ
    {
      int32_t v = n.مقصد_fixed ();
      encode_byte (Op_رمز_fixedcs);
      encode_byte ((v >> 24) & 0xFF);
      encode_byte ((v >> 16) & 0xFF);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for TopDict / Private
  خالی encode_عدد_tp (ثابت نمبر_t& n)
  {
    اگر (n.اندر_int_range ())
    {
      // TODO longint
      encode_int (n.مقصد_int ());
    }
    ورنہ
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      دوہرا v = n.مقصد_real ();
      encode_byte (Op_رمز_BCD);

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

      hb_locale_t clocale HB_غیر_مستعمل;
      hb_locale_t oldlocale HB_غیر_مستعمل;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", تہی));
      snprintf (buf, حجم (buf), "%.8G", v);
      (خالی) hb_uselocale (((خالی) freelocale (clocale), oldlocale));

      حرف *s = buf;
      size_t len;
      حرف *comma = strchr (s, ',');
      اگر (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      اگر (s[0] == '0' && s[1] == '.')
	s++;
      ورنہ اگر (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      ورنہ اگر ((len = لڑی_لمبائی (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	غیرعلامتی exponent = len - 3;
	حرف *s2 = s + exponent - 1;
	جب (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, حجم (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      ورنہ
      {
	حرف *dot = strchr (s, '.');
	حرف *e = strchr (s, 'E');
	اگر (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  صحیح exponent = atoi (e + 1);
	  صحیح new_exponent = exponent - (e - (dot + 1));
	  اگر (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  ورنہ
	    snprintf (e - 1, حجم (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      اگر ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	صحیح sign = s[0] == '-';
	حرف *s2 = s + sign + 1;
	جب (*s2 == '0')
	  s2++;
	len = لڑی_لمبائی (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, حجم (buf) - (s + sign + len - buf), "E-%u", (غیرعلامتی) (لڑی_لمبائی (s + sign) - 1));
      }
      hb_سمتیہ_t<حرف> nibbles;
      جب (*s)
      {
	حرف c = s[0];
	s++;

	انتخاب (c)
	{
	  حالت 'E':
	  {
	    حرف c2 = *s;
	    اگر (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } ورنہ {
	      اگر (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    اگر (*s == '0')
	      s++;
	    جاری;
	  }

	  حالت '.':
	    nibbles.push (0x0A); // .
	    جاری;

	  حالت '-':
	    nibbles.push (0x0E); // -
	    جاری;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      اگر (nibbles.لمبائی % 2)
	nibbles.push (0x0F);

      غیرعلامتی گنتی = nibbles.لمبائی;
      برائے (غیرعلامتی i = 0; i < گنتی; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  خالی encode_op (op_رمز_t op)
  {
    اگر (Is_Op_رمز_ESC (op))
    {
      encode_byte (Op_رمز_escape);
      encode_byte (Unmake_Op_رمز_ESC (op));
    }
    ورنہ
      encode_byte (op);
  }

  خالی copy_str (ثابت غیرعلامتی حرف *str, غیرعلامتی لمبائی)
  {
    أكد ((علامتی) (buff.لمبائی + لمبائی) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.لمبائی, str, لمبائی);
    buff.لمبائی += لمبائی;
  }

  منطقی اندر_error () ثابت { واپس buff.اندر_error (); }

  محفوظ:

  str_buff_t &buff;
};

ساخت cff_sub_جدول_info_t {
  cff_sub_جدول_info_t ()
    : fd_array_link (0),
      char_لڑیاں_link (0)
  {
    fd_select.init ();
  }

  جدول_info_t     fd_select;
  objidx_t     	   fd_array_link;
  objidx_t     	   char_لڑیاں_link;
};

سانچہ <قسم_نام OPSTR=op_str_t>
ساخت cff_چوٹی_dict_op_serializer_t : op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت OPSTR &opstr,
		  ثابت cff_sub_جدول_info_t &info) ثابت
  {
    TRACE_SERIALIZE (یہ);

    انتخاب (opstr.op)
    {
      حالت Op_رمز_Char_لڑیاں:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_لڑیاں_link, whence_t::Absolute));

      حالت Op_رمز_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      حالت Op_رمز_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      طےشدہ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (سچ);
  }
};

ساخت cff_font_dict_op_serializer_t : op_serializer_t
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت op_str_t &opstr,
		  ثابت جدول_info_t &نجی_Dict_Info) ثابت
  {
    TRACE_SERIALIZE (یہ);

    اگر (opstr.op == Op_رمز_نجی)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, نجی_Dict_Info.سائز) &&
		    Dict::serialize_link4_op (c, opstr.op, نجی_Dict_Info.link, whence_t::Absolute));
    }
    ورنہ
    {
      غیرعلامتی حرف *d = c->allocate_سائز<غیرعلامتی حرف> (opstr.لمبائی);
      اگر (unlikely (!d)) return_trace (جھوٹ);
      /* Faster than hb_memcpy for small strings. */
      برائے (غیرعلامتی i = 0; i < opstr.لمبائی; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (سچ);
  }
};

/* CharString command for specialization */
ساخت cs_command_t
{
  hb_سمتیہ_t<نمبر_t> دلائل;
  hb_سمتیہ_t<غیرعلامتی حرف> نقاب_بائٹیں; /* For hintmask/cntrmask payload bytes. */
  op_رمز_t op;

  cs_command_t () : op (Op_رمز_Invalid) {}
  cs_command_t (op_رمز_t op_) : op (op_) {}
};

قسمتعریف hb_سمتیہ_t<cs_command_t> *cs_command_vec_t;

ساخت cff2_instancing_plan_t;

ساخت flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   منطقی drop_hints_,
                   ثابت hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = خالی_اشاریہ)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  منطقی	drop_hints;
  ثابت hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  ثابت cff2_instancing_plan_t *instancer = خالی_اشاریہ;
  منطقی emitted_blend = جھوٹ;
};

سانچہ <قسم_نام ACC, قسم_نام ENV, قسم_نام OPSET, op_رمز_t endchar_op=Op_رمز_Invalid>
ساخت subr_flattener_t
{
  subr_flattener_t (ثابت ACC &acc_,
		    ثابت hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  منطقی flatten (str_buff_vec_t &flat_charstrings,
                hb_سمتیہ_t<hb_سمتیہ_t<cs_command_t>> *command_capture = خالی_اشاریہ)
  {
    غیرعلامتی گنتی = plan->عدد_output_glyphs ();
    اگر (!flat_charstrings.resize_exact (گنتی))
      واپس جھوٹ;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      hb_codepoint_t  glyph;
      اگر (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	اگر (endchar_op != Op_رمز_Invalid) flat_charstrings[i].push (endchar_op);
	جاری;
      }
      ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[glyph];
      غیرعلامتی صحیح fd = subset_get_fd (acc, glyph);
      اگر (unlikely (fd >= acc.fd_گنتی))
	واپس جھوٹ;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.لمبائی);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (منطقی) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : خالی_اشاریہ
      };
      اگر (unlikely (!interp.interpret (param)))
	واپس جھوٹ;
    }
    واپس سچ;
  }

  ثابت ACC &acc;
  ثابت hb_subset_plan_t *plan;
};

ساخت subr_closures_t
{
  subr_closures_t (غیرعلامتی صحیح fd_گنتی_2) : global_احاطہ (), local_closures ()
  {
    local_closures.resize_exact (fd_گنتی_2);
  }

  خالی reset ()
  {
    global_احاطہ.clear();
    برائے (غیرعلامتی صحیح i = 0; i < local_closures.لمبائی; i++)
      local_closures[i].clear();
  }

  منطقی اندر_error () ثابت { واپس local_closures.اندر_error (); }
  hb_سیٹ_t  global_احاطہ;
  hb_سمتیہ_t<hb_سیٹ_t> local_closures;
};

ساخت parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (غیرعلامتی صحیح subr_عدد = 0) :
    subr_عدد_2 (subr_عدد) {}

  منطقی is_hinting () ثابت { واپس hinting_flag; }
  خالی سیٹ_hinting ()       { hinting_flag = سچ; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  محفوظ:
  منطقی	  hinting_flag = جھوٹ;

  عوامی:
  uint16_t subr_عدد_2;
};

ساخت parsed_cs_str_t : parsed_اقدار_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (جھوٹ),
    hint_dropped (جھوٹ),
    has_سابقہ (جھوٹ),
    has_calls_ (جھوٹ),
    coalescing_ (جھوٹ)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  خالی جوڑو_op (op_رمز_t op, ثابت byte_str_ref_t& str_ref)
  {
    اگر (is_parsed ()) واپس;
    اگر (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_انتہا = آخری_انتہا_2;
      آخری_انتہا_2 = str_ref.get_انحراف ();
      اگر (unlikely (op == Op_رمز_return || op == Op_رمز_endchar))
	flush_segment (str_ref, آخری_انتہا_2);
      واپس;
    }
    SUPER::جوڑو_op (op, str_ref);
  }

  خالی جوڑو_call_op (op_رمز_t op, ثابت byte_str_ref_t& str_ref, غیرعلامتی صحیح subr_عدد_2)
  {
    اگر (is_parsed ()) واپس;
    has_calls_ = سچ;

    اگر (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_انتہا);
      op_آغاز = آخری_انتہا_2;
      SUPER::جوڑو_op (op, str_ref, {subr_عدد_2});
      penultimate_انتہا = آخری_انتہا_2 = str_ref.get_انحراف ();
      واپس;
    }

    /* Pop the subroutine number. */
    values.نکالو ();

    SUPER::جوڑو_op (op, str_ref, {subr_عدد_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  خالی flush_segment (ثابت byte_str_ref_t& str_ref, غیرعلامتی انتہا)
  {
    غیرعلامتی آغاز = op_آغاز;
    اگر (انتہا <= آغاز) واپس;
    جب (آغاز < انتہا)
    {
      خودکار arr = str_ref.sub_array (آغاز, hb_min (انتہا - آغاز, 255u));
      اگر (unlikely (!arr.لمبائی)) توڑو;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->لمبائی = arr.لمبائی;
      آغاز += arr.لمبائی;
    }
    op_آغاز = انتہا;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  خالی flush_coalesced (ثابت byte_str_ref_t& str_ref)
  {
    اگر (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_انحراف ());
  }

  خالی enable_coalescing () { coalescing_ = سچ; }
  منطقی is_coalescing () ثابت { واپس coalescing_; }

  خالی سیٹ_سابقہ (ثابت نمبر_t &عدد, op_رمز_t op = Op_رمز_Invalid)
  {
    has_سابقہ = سچ;
    سابقہ_op = op;
    سابقہ_عدد = عدد;
  }

  منطقی at_انتہا (غیرعلامتی صحیح pos) ثابت
  {
    واپس ((pos + 1 >= values.لمبائی) /* CFF2 */
	|| (values[pos + 1].op == Op_رمز_return));
  }

  منطقی is_parsed () ثابت { واپس parsed; }
  خالی سیٹ_parsed ()      { parsed = سچ; }

  منطقی is_hint_dropped () ثابت { واپس hint_dropped; }
  خالی سیٹ_hint_dropped ()      { hint_dropped = سچ; }

  منطقی is_vsindex_dropped () ثابت { واپس vsindex_dropped; }
  خالی سیٹ_vsindex_dropped ()      { vsindex_dropped = سچ; }

  منطقی has_سابقہ_2 () ثابت          { واپس has_سابقہ; }
  op_رمز_t سابقہ_op_2 () ثابت         { واپس سابقہ_op; }
  ثابت نمبر_t &سابقہ_عدد_2 () ثابت { واپس سابقہ_عدد; }

  منطقی has_calls () ثابت          { واپس has_calls_; }

  خالی compact ()
  {
    غیرعلامتی گنتی = values.لمبائی;
    اگر (!گنتی) واپس;
    خودکار &opstr = values.arrayZ;
    غیرعلامتی j = 0;
    برائے (غیرعلامتی i = 1; i < گنتی; i++)
    {
      /* See if we can combine op j and op i. */
      منطقی combine =
        (opstr[j].op != Op_رمز_callsubr && opstr[j].op != Op_رمز_callgsubr) &&
        (opstr[i].op != Op_رمز_callsubr && opstr[i].op != Op_رمز_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].لمبائی == opstr[i].ptr) &&
        (opstr[j].لمبائی + opstr[i].لمبائی <= 255);

      اگر (combine)
      {
	opstr[j].لمبائی += opstr[i].لمبائی;
	opstr[j].op = Op_رمز_Invalid;
      }
      ورنہ
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  محفوظ:
  منطقی    parsed : 1;
  منطقی    hint_dropped : 1;
  منطقی    vsindex_dropped : 1;
  منطقی    has_سابقہ : 1;
  منطقی    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  منطقی    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  غیرعلامتی penultimate_انتہا = 0;
  غیرعلامتی آخری_انتہا_2 = 0;
  op_رمز_t	سابقہ_op;
  نمبر_t	سابقہ_عدد;

  نجی:
  قسمتعریف parsed_اقدار_t<parsed_cs_op_t> SUPER;
};

ساخت parsed_cs_str_vec_t : hb_سمتیہ_t<parsed_cs_str_t>
{
  نجی:
  قسمتعریف hb_سمتیہ_t<parsed_cs_str_t> SUPER;
};

ساخت cff_subset_accelerator_t
{
  ساکن cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      ثابت parsed_cs_str_vec_t& parsed_charstrings,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs,
      ثابت hb_سمتیہ_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (حجم(cff_subset_accelerator_t));
    اگر (unlikely (!accel)) واپس خالی_اشاریہ;
    نیا (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    واپس accel;
  }

  ساکن خالی destroy (خالی* value) {
    اگر (!value) واپس;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      ثابت parsed_cs_str_vec_t& parsed_charstrings_,
      ثابت parsed_cs_str_vec_t& parsed_global_subrs_,
      ثابت hb_سمتیہ_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    خودکار *mapping = glyph_مقصد_sid_map.get_relaxed ();
    اگر (mapping)
    {
      mapping->~glyph_مقصد_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_سمتیہ_t<parsed_cs_str_vec_t> parsed_local_subrs;
  تغیرپذیر hb_atomic_t<glyph_مقصد_sid_map_t *> glyph_مقصد_sid_map;

 نجی:
  hb_blob_t* original_blob;
};

ساخت subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_سیٹ_t *global_احاطہ_2,
		       hb_سیٹ_t *local_احاطہ,
		       منطقی drop_hints_,
		       منطقی coalesce_ = جھوٹ) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_احاطہ (global_احاطہ_2),
      local_احاطہ_2 (local_احاطہ),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    اگر (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    انتخاب (context.type)
    {
      حالت CSType_CharString:
	واپس parsed_charstring;

      حالت CSType_LocalSubr:
	اگر (likely (context.subr_عدد_2 < parsed_local_subrs->لمبائی))
	  واپس &(*parsed_local_subrs)[context.subr_عدد_2];
	توڑو;

      حالت CSType_GlobalSubr:
	اگر (likely (context.subr_عدد_2 < parsed_global_subrs->لمبائی))
	  واپس &(*parsed_global_subrs)[context.subr_عدد_2];
	توڑو;
    }
    واپس خالی_اشاریہ;
  }

  سانچہ <قسم_نام ENV>
  خالی سیٹ_current_str (ENV &env, منطقی calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    اگر (unlikely (!parsed_str))
    {
      env.سیٹ_error ();
      واپس;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    اگر (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.لمبائی > 0)))
      env.سیٹ_error ();
    ورنہ
    {
      اگر (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.کل_سائز ());
        اگر (coalesce) parsed_str->enable_coalescing ();
      }
      current_parsed_str = parsed_str;
    }
  }

  parsed_cs_str_t	*current_parsed_str;

  parsed_cs_str_t	*parsed_charstring;
  parsed_cs_str_vec_t	*parsed_global_subrs;
  parsed_cs_str_vec_t	*parsed_local_subrs;
  hb_سیٹ_t      *global_احاطہ;
  hb_سیٹ_t      *local_احاطہ_2;
  منطقی	  drop_hints;
  منطقی	  coalesce;
};

ساخت subr_remap_t : hb_inc_bimap_t
{
  خالی create (ثابت hb_سیٹ_t *احاطہ)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (احاطہ->get_population ());
    برائے (خودکار old_عدد : *احاطہ)
      جوڑو (old_عدد);

    اگر (get_population () < 1240)
      bias = 107;
    ورنہ اگر (get_population () < 33900)
      bias = 1131;
    ورنہ
      bias = 32768;
  }

  صحیح biased_عدد (غیرعلامتی صحیح old_عدد) ثابت
  {
    hb_codepoint_t new_عدد = get (old_عدد);
    واپس (صحیح)new_عدد - bias;
  }

  محفوظ:
  صحیح bias;
};

ساخت subr_remaps_t
{
  subr_remaps_t (غیرعلامتی صحیح fd_گنتی)
  {
    local_remaps.resize (fd_گنتی);
  }

  منطقی اندر_error()
  {
    واپس local_remaps.اندر_error ();
  }

  خالی create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_احاطہ);
    برائے (غیرعلامتی صحیح i = 0; i < local_remaps.لمبائی; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_سمتیہ_t<subr_remap_t>  local_remaps;
};

سانچہ <قسم_نام SUBSETTER, قسم_نام SUBRS, قسم_نام ACC, قسم_نام ENV, قسم_نام OPSET, op_رمز_t endchar_op=Op_رمز_Invalid>
ساخت subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, ثابت hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_گنتی),
        remaps(acc_.fd_گنتی)
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
  منطقی subset (خالی)
  {
    غیرعلامتی fd_گنتی_2 = acc.fd_گنتی;
    ثابت cff_subset_accelerator_t* cff_accelerator = خالی_اشاریہ;
    اگر (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_گنتی_2 = cff_accelerator->parsed_local_subrs.لمبائی;
    }

    اگر (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->عدد_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } ورنہ {
      parsed_charstrings.resize_exact (plan->عدد_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->گنتی);

      اگر (unlikely (!parsed_local_subrs_storage.resize (fd_گنتی_2))) واپس جھوٹ;

      برائے (غیرعلامتی صحیح i = 0; i < acc.fd_گنتی; i++)
      {
        غیرعلامتی گنتی = acc.نجی_Dicts[i].localSubrs->گنتی;
        parsed_local_subrs_storage[i].resize (گنتی);
        اگر (unlikely (parsed_local_subrs_storage[i].اندر_error ())) واپس جھوٹ;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    اگر (unlikely (remaps.اندر_error()
                  || cached_charstrings.اندر_error ()
                  || parsed_charstrings.اندر_error ()
                  || parsed_global_subrs->اندر_error ()
                  || closures.اندر_error ())) {
      واپس جھوٹ;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    منطقی coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    برائے (خودکار _ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.پہلا;
      hb_codepoint_t old_glyph = _.second;

      ثابت hb_ubytes_t str = (*acc.char_لڑیاں)[old_glyph];
      غیرعلامتی صحیح fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_گنتی))
        واپس جھوٹ;

      اگر (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        اگر (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        ورنہ
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        جاری;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.لمبائی);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_احاطہ,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      اگر (unlikely (!interp.interpret (param)))
        واپس جھوٹ;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      اگر ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_احاطہ,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	اگر (drop_hints_اندر_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].سیٹ_hint_dropped ();
	  اگر (drop.vsindex_dropped)
	    parsed_charstrings[new_glyph].سیٹ_vsindex_dropped ();
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
      اگر (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    اگر ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !احاطہ_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      واپس جھوٹ;

    remaps.create (closures);

    populate_subset_accelerator ();
    واپس سچ;
  }

  منطقی encode_charstrings (str_buff_vec_t &buffArray, منطقی encode_سابقہ = سچ) ثابت
  {
    غیرعلامتی عدد_glyphs = plan->عدد_output_glyphs ();
    اگر (unlikely (!buffArray.resize_exact (عدد_glyphs)))
      واپس جھوٹ;
    hb_codepoint_t آخری = 0;
    برائے (خودکار _ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t gid = _.پہلا;
      hb_codepoint_t old_glyph = _.second;

      اگر (endchar_op != Op_رمز_Invalid)
        برائے (; آخری < gid; آخری++)
	{
	  // Hack to point vector to static string.
	  خودکار &b = buffArray.arrayZ[آخری];
	  b.سیٹ_storage (const_cast<غیرعلامتی حرف *>(endchar_str), 1);
	}

      آخری++; // Skip over gid
      غیرعلامتی صحیح  fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_گنتی))
	واپس جھوٹ;
      اگر (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_سابقہ)))
	واپس جھوٹ;
    }
    اگر (endchar_op != Op_رمز_Invalid)
      برائے (; آخری < عدد_glyphs; آخری++)
      {
	// Hack to point vector to static string.
	خودکار &b = buffArray.arrayZ[آخری];
	b.سیٹ_storage (const_cast<غیرعلامتی حرف *>(endchar_str), 1);
      }

    واپس سچ;
  }

  منطقی encode_subrs (ثابت parsed_cs_str_vec_t &subrs, ثابت subr_remap_t& remap, غیرعلامتی صحیح fd, str_buff_vec_t &buffArray) ثابت
  {
    غیرعلامتی صحیح  گنتی = remap.get_population ();

    اگر (unlikely (!buffArray.resize_exact (گنتی)))
      واپس جھوٹ;
    برائے (غیرعلامتی صحیح new_عدد = 0; new_عدد < گنتی; new_عدد++)
    {
      hb_codepoint_t old_عدد = remap.backward (new_عدد);
      أكد (old_عدد != CFF_UNDEF_رمز);

      اگر (unlikely (!encode_str (subrs[old_عدد], fd, buffArray[new_عدد])))
	واپس جھوٹ;
    }
    واپس سچ;
  }

  منطقی encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    واپس encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  منطقی encode_localsubrs (غیرعلامتی صحیح fd, str_buff_vec_t &buffArray) ثابت
  {
    واپس encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  محفوظ:
  ساخت drop_hints_param_t
  {
    drop_hints_param_t ()
      : نظر_آیا_moveto (جھوٹ),
	سرے_اندر_hint (جھوٹ),
	all_dropped (جھوٹ),
	vsindex_dropped (جھوٹ) {}

    منطقی  نظر_آیا_moveto;
    منطقی  سرے_اندر_hint;
    منطقی  all_dropped;
    منطقی  vsindex_dropped;
  };

  منطقی drop_hints_اندر_subr (parsed_cs_str_t &str, غیرعلامتی صحیح pos,
			   parsed_cs_str_vec_t &subrs, غیرعلامتی صحیح subr_عدد_2,
			   ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.سرے_اندر_hint = جھوٹ;
    منطقی has_hint = drop_hints_اندر_str (subrs[subr_عدد_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    اگر (drop.سرے_اندر_hint)
    {
      str.values[pos].سیٹ_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      اگر (!str.at_انتہا (pos))
	drop.سرے_اندر_hint = جھوٹ;
    }
    ورنہ اگر (drop.all_dropped)
    {
      str.values[pos].سیٹ_hinting ();
    }

    واپس has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  منطقی drop_hints_اندر_str (parsed_cs_str_t &str, ثابت subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    منطقی  نظر_آیا_hint = جھوٹ;

    غیرعلامتی گنتی = str.values.لمبائی;
    خودکار *values = str.values.arrayZ;
    برائے (غیرعلامتی صحیح pos = 0; pos < گنتی; pos++)
    {
      منطقی  has_hint = جھوٹ;
      انتخاب (values[pos].op)
      {
	حالت Op_رمز_callsubr:
	  has_hint = drop_hints_اندر_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_عدد_2,
					param, drop);
	  توڑو;

	حالت Op_رمز_callgsubr:
	  has_hint = drop_hints_اندر_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_عدد_2,
					param, drop);
	  توڑو;

	حالت Op_رمز_rmoveto:
	حالت Op_رمز_hmoveto:
	حالت Op_رمز_vmoveto:
	  drop.نظر_آیا_moveto = سچ;
	  توڑو;

	حالت Op_رمز_hintmask:
	حالت Op_رمز_cntrmask:
	  اگر (drop.نظر_آیا_moveto)
	  {
	    values[pos].سیٹ_hinting ();
	    توڑو;
	  }
	  HB_FALLTHROUGH;

	حالت Op_رمز_hstemhm:
	حالت Op_رمز_vstemhm:
	حالت Op_رمز_hstem:
	حالت Op_رمز_vstem:
	  has_hint = سچ;
	  values[pos].سیٹ_hinting ();
	  اگر (str.at_انتہا (pos))
	    drop.سرے_اندر_hint = سچ;
	  توڑو;

	حالت Op_رمز_dotsection:
	  values[pos].سیٹ_hinting ();
	  توڑو;

	طےشدہ:
	  /* NONE */
	  توڑو;
      }
      اگر (has_hint)
      {
	برائے (صحیح i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(غیرعلامتی)i];
	  اگر (csop.is_hinting ())
	    توڑو;
	  csop.سیٹ_hinting ();
	  اگر (csop.op == Op_رمز_vsindexcs)
	    drop.vsindex_dropped = سچ;
	}
	نظر_آیا_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = سچ;
    برائے (غیرعلامتی صحیح pos = 0; pos < گنتی; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      اگر (csop.op == Op_رمز_return)
	توڑو;
      اگر (!csop.is_hinting ())
      {
	drop.all_dropped = جھوٹ;
	توڑو;
      }
    }

    واپس نظر_آیا_hint;
  }

  منطقی احاطہ_subroutines (ثابت parsed_cs_str_vec_t& global_subrs,
                            ثابت hb_سمتیہ_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    برائے (خودکار _ : plan->new_مقصد_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.پہلا;
      hb_codepoint_t old_glyph = _.second;
      غیرعلامتی صحیح fd = subset_get_fd (acc, old_glyph);
      اگر (unlikely (fd >= acc.fd_گنتی))
        واپس جھوٹ;

      // Note: const cast is safe here because the collect_subr_refs_in_str only performs a
      //       closure and does not modify any of the charstrings.
      subr_subset_param_t  param (const_cast<parsed_cs_str_t*> (&get_parsed_charstring (new_glyph)),
                                  const_cast<parsed_cs_str_vec_t*> (&global_subrs),
                                  const_cast<parsed_cs_str_vec_t*> (&local_subrs[fd]),
                                  &closures.global_احاطہ,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
      collect_subr_refs_اندر_str (get_parsed_charstring (new_glyph), param);
    }

    واپس سچ;
  }

  خالی collect_subr_refs_اندر_subr (غیرعلامتی صحیح subr_عدد_2, parsed_cs_str_vec_t &subrs,
				  hb_سیٹ_t *احاطہ,
				  ثابت subr_subset_param_t &param)
  {
    اگر (احاطہ->has (subr_عدد_2))
      واپس;
    احاطہ->جوڑو (subr_عدد_2);
    collect_subr_refs_اندر_str (subrs[subr_عدد_2], param);
  }

  خالی collect_subr_refs_اندر_str (ثابت parsed_cs_str_t &str,
                                 ثابت subr_subset_param_t &param)
  {
    اگر (!str.has_calls ())
      واپس;

    برائے (خودکار &opstr : str.values)
    {
      اگر (!param.drop_hints || !opstr.is_hinting ())
      {
	انتخاب (opstr.op)
	{
	  حالت Op_رمز_callsubr:
	    collect_subr_refs_اندر_subr (opstr.subr_عدد_2, *param.parsed_local_subrs,
				       param.local_احاطہ_2, param);
	    توڑو;

	  حالت Op_رمز_callgsubr:
	    collect_subr_refs_اندر_subr (opstr.subr_عدد_2, *param.parsed_global_subrs,
				       param.global_احاطہ, param);
	    توڑو;

	  طےشدہ: توڑو;
	}
      }
    }
  }

  منطقی encode_str (ثابت parsed_cs_str_t &str, ثابت غیرعلامتی صحیح fd, str_buff_t &buff, منطقی encode_سابقہ = سچ) ثابت
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    منطقی hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    اگر (encode_سابقہ && str.has_سابقہ_2 () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_عدد_cs (str.سابقہ_عدد_2 ());
      اگر (str.سابقہ_op_2 () != Op_رمز_Invalid)
	encoder.encode_op (str.سابقہ_op_2 ());
    }

    غیرعلامتی سائز = 0;
    برائے (خودکار &opstr : str.values)
    {
      سائز += opstr.لمبائی;
      اگر (opstr.op == Op_رمز_callsubr || opstr.op == Op_رمز_callgsubr)
        سائز += 3;
    }
    اگر (!buff.alloc_exact (buff.لمبائی + سائز))
      واپس جھوٹ;

    برائے (خودکار &opstr : str.values)
    {
      اگر (hinting || !opstr.is_hinting ())
      {
	انتخاب (opstr.op)
	{
	  حالت Op_رمز_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_عدد (opstr.subr_عدد_2));
	    encoder.copy_str (opstr.ptr, opstr.لمبائی);
	    توڑو;

	  حالت Op_رمز_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_عدد (opstr.subr_عدد_2));
	    encoder.copy_str (opstr.ptr, opstr.لمبائی);
	    توڑو;

	  طےشدہ:
	    encoder.copy_str (opstr.ptr, opstr.لمبائی);
	    توڑو;
	}
      }
    }
    واپس !encoder.اندر_error ();
  }

  خالی compact_parsed_subrs () ثابت
  {
    برائے (خودکار &cs : parsed_global_subrs_storage)
      cs.compact ();
    برائے (خودکار &vec : parsed_local_subrs_storage)
      برائے (خودکار &cs : vec)
	cs.compact ();
  }

  خالی populate_subset_accelerator () ثابت
  {
    اگر (!plan->inprogress_accelerator) واپس;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  ثابت parsed_cs_str_t& get_parsed_charstring (غیرعلامتی i) ثابت
  {
    اگر (cached_charstrings) واپس *(cached_charstrings[i]);
    واپس parsed_charstrings[i];
  }

  محفوظ:
  ثابت ACC			&acc;
  ثابت hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_سمتیہ_t<ثابت parsed_cs_str_t*>     cached_charstrings;
  ثابت parsed_cs_str_vec_t*              parsed_global_subrs;
  ثابت hb_سمتیہ_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  نجی:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_سمتیہ_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  قسمتعریف قسم_نام SUBRS::گنتی_type subr_گنتی_type;
};

} /* namespace CFF */

HB_اندرونی منطقی
hb_plan_subset_cff_fdselect (ثابت hb_subset_plan_t *plan,
			    غیرعلامتی صحیح fd_گنتی,
			    ثابت CFF::FDSelect &src, /*اندر*/
			    غیرعلامتی صحیح src_عدد_glyphs,
			    غیرعلامتی صحیح &subset_fd_گنتی /*برآمد*/,
			    غیرعلامتی صحیح &subset_fdselect_سائز /*برآمد*/,
			    غیرعلامتی صحیح &subset_fdselect_format /*برآمد*/,
			    hb_سمتیہ_t<CFF::رمز_pair_t> &fdselect_ranges /*برآمد*/,
			    hb_inc_bimap_t &fdmap /*برآمد*/);

HB_اندرونی منطقی
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  غیرعلامتی صحیح عدد_glyphs,
			  ثابت CFF::FDSelect &src,
			  غیرعلامتی صحیح fd_گنتی_2,
			  غیرعلامتی صحیح fdselect_format,
			  غیرعلامتی صحیح سائز,
			  ثابت hb_سمتیہ_t<CFF::رمز_pair_t> &fdselect_ranges);

#اگر_ختم /* HB_SUBSET_CFF_COMMON_HH */
