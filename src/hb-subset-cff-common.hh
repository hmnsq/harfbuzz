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

#నిర్వచితం_కాకపోతే HB_SUBSET_CFF_COMMON_HH
#నిర్వచించు HB_SUBSET_CFF_COMMON_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset-plan.hh"
#అన్తర్భావ "hb-cff-interp-cs-common.hh"

నామాకాశ CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
మూస <రకం_పేరు ACC>
స్థిర అంతఃవరుస గుర్తులేని
subset_get_fd (స్థిరరాశి ACC &acc, hb_codepoint_t glyph)
{
  తిరిగి likely (glyph < acc.అంకె_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
నిర్మాణం str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  శూన్య reset () { buff.reset (); }

  శూన్య encode_byte (గుర్తులేని అక్షరం b)
  {
    ఒకవేళ (likely ((గుర్తుగల) buff.నిడివి < buff.allocated))
      buff.arrayZ[buff.నిడివి++] = b;
    లేకపోతే
      buff.push (b);
  }

  శూన్య encode_int (పూర్ణాంకం v)
  {
    ఒకవేళ ((-1131 <= v) && (v <= 1131))
    {
      ఒకవేళ ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      లేకపోతే ఒకవేళ (v > 0)
      {
	v -= 108;
	encode_byte ((v >> 8) + Op_సంకేతం_Two_Byte_Pos_Int0);
	encode_byte (v & 0xFF);
      }
      లేకపోతే
      {
	v = -v - 108;
	encode_byte ((v >> 8) + Op_సంకేతం_Two_Byte_Neg_Int0);
	encode_byte (v & 0xFF);
      }
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (v < -32768))
	v = -32768;
      లేకపోతే ఒకవేళ (unlikely (v > 32767))
	v = 32767;
      encode_byte (Op_సంకేతం_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  శూన్య encode_అంకె_cs (స్థిరరాశి సంఖ్య_t& n)
  {
    ఒకవేళ (n.లోపల_int_range ())
    {
      encode_int (n.గమ్యం_int ());
    }
    లేకపోతే
    {
      int32_t v = n.గమ్యం_fixed ();
      encode_byte (Op_సంకేతం_fixedcs);
      encode_byte ((v >> 24) & 0xFF);
      encode_byte ((v >> 16) & 0xFF);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for TopDict / Private
  శూన్య encode_అంకె_tp (స్థిరరాశి సంఖ్య_t& n)
  {
    ఒకవేళ (n.లోపల_int_range ())
    {
      // TODO longint
      encode_int (n.గమ్యం_int ());
    }
    లేకపోతే
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      ద్విగుణ v = n.గమ్యం_real ();
      encode_byte (Op_సంకేతం_BCD);

      // Based on:
      // https://github.com/fonttools/fonttools/blob/0738c41dfbcbc213ab9263f486ef0cccc6eb5ce5/Lib/fontTools/misc/psCharStrings.py#L267-L316

      అక్షరం buf[16];
      /* FontTools has the following comment:
       *
       * # Note: 14 decimal digits seems to be the limitation for CFF real numbers
       * # in macOS. However, we use 8 here to match the implementation of AFDKO.
       *
       * We use 8 here to match FontTools X-).
       */

      hb_locale_t clocale HB_వాడని;
      hb_locale_t oldlocale HB_వాడని;
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", ఖాళీ));
      snprintf (buf, పరిమాణం (buf), "%.8G", v);
      (శూన్య) hb_uselocale (((శూన్య) freelocale (clocale), oldlocale));

      అక్షరం *s = buf;
      size_t len;
      అక్షరం *comma = strchr (s, ',');
      ఒకవేళ (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      ఒకవేళ (s[0] == '0' && s[1] == '.')
	s++;
      లేకపోతే ఒకవేళ (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      లేకపోతే ఒకవేళ ((len = అక్షరశ్రేణి_పొడవు (s)) > 3 && !strcmp (s + len - 3, "000"))
      {
	గుర్తులేని exponent = len - 3;
	అక్షరం *s2 = s + exponent - 1;
	అయితే (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, పరిమాణం (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      లేకపోతే
      {
	అక్షరం *dot = strchr (s, '.');
	అక్షరం *e = strchr (s, 'E');
	ఒకవేళ (dot && e)
	{
	  memmove (dot, dot + 1, e - (dot + 1));
	  పూర్ణాంకం exponent = atoi (e + 1);
	  పూర్ణాంకం new_exponent = exponent - (e - (dot + 1));
	  ఒకవేళ (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  లేకపోతే
	    snprintf (e - 1, పరిమాణం (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      ఒకవేళ ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	పూర్ణాంకం sign = s[0] == '-';
	అక్షరం *s2 = s + sign + 1;
	అయితే (*s2 == '0')
	  s2++;
	len = అక్షరశ్రేణి_పొడవు (s2);
	memmove (s + sign, s2, len);
	snprintf (s + sign + len, పరిమాణం (buf) - (s + sign + len - buf), "E-%u", (గుర్తులేని) (అక్షరశ్రేణి_పొడవు (s + sign) - 1));
      }
      hb_సదిశ_t<అక్షరం> nibbles;
      అయితే (*s)
      {
	అక్షరం c = s[0];
	s++;

	ఎంపిక (c)
	{
	  స్థితి 'E':
	  {
	    అక్షరం c2 = *s;
	    ఒకవేళ (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } లేకపోతే {
	      ఒకవేళ (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    ఒకవేళ (*s == '0')
	      s++;
	    కొనసాగు;
	  }

	  స్థితి '.':
	    nibbles.push (0x0A); // .
	    కొనసాగు;

	  స్థితి '-':
	    nibbles.push (0x0E); // -
	    కొనసాగు;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      ఒకవేళ (nibbles.నిడివి % 2)
	nibbles.push (0x0F);

      గుర్తులేని లెక్క = nibbles.నిడివి;
      కొరకు (గుర్తులేని i = 0; i < లెక్క; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  శూన్య encode_op (op_సంకేతం_t op)
  {
    ఒకవేళ (Is_Op_సంకేతం_ESC (op))
    {
      encode_byte (Op_సంకేతం_escape);
      encode_byte (Unmake_Op_సంకేతం_ESC (op));
    }
    లేకపోతే
      encode_byte (op);
  }

  శూన్య copy_str (స్థిరరాశి గుర్తులేని అక్షరం *str, గుర్తులేని నిడివి)
  {
    ప్రతిజ్ఞా ((గుర్తుగల) (buff.నిడివి + నిడివి) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.నిడివి, str, నిడివి);
    buff.నిడివి += నిడివి;
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి buff.లోపల_error (); }

  రక్షిత:

  str_buff_t &buff;
};

నిర్మాణం cff_sub_పట్టిక_info_t {
  cff_sub_పట్టిక_info_t ()
    : fd_array_link (0),
      char_అక్షరశ్రేణులు_link (0)
  {
    fd_select.init ();
  }

  పట్టిక_info_t     fd_select;
  objidx_t     	   fd_array_link;
  objidx_t     	   char_అక్షరశ్రేణులు_link;
};

మూస <రకం_పేరు OPSTR=op_str_t>
నిర్మాణం cff_శిఖరం_dict_op_serializer_t : op_serializer_t
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి OPSTR &opstr,
		  స్థిరరాశి cff_sub_పట్టిక_info_t &info) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఎంపిక (opstr.op)
    {
      స్థితి Op_సంకేతం_Char_అక్షరశ్రేణులు:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_అక్షరశ్రేణులు_link, whence_t::Absolute));

      స్థితి Op_సంకేతం_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      స్థితి Op_సంకేతం_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      అప్రమేయ:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (నిజం);
  }
};

నిర్మాణం cff_font_dict_op_serializer_t : op_serializer_t
{
  తార్కిక serialize (hb_serialize_context_t *c,
		  స్థిరరాశి op_str_t &opstr,
		  స్థిరరాశి పట్టిక_info_t &వ్యక్తిగత_Dict_Info) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    ఒకవేళ (opstr.op == Op_సంకేతం_వ్యక్తిగత)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, వ్యక్తిగత_Dict_Info.కొలత) &&
		    Dict::serialize_link4_op (c, opstr.op, వ్యక్తిగత_Dict_Info.link, whence_t::Absolute));
    }
    లేకపోతే
    {
      గుర్తులేని అక్షరం *d = c->allocate_కొలత<గుర్తులేని అక్షరం> (opstr.నిడివి);
      ఒకవేళ (unlikely (!d)) return_trace (అబద్ధం);
      /* Faster than hb_memcpy for small strings. */
      కొరకు (గుర్తులేని i = 0; i < opstr.నిడివి; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (నిజం);
  }
};

/* CharString command for specialization */
నిర్మాణం cs_command_t
{
  hb_సదిశ_t<సంఖ్య_t> వాదనలు;
  hb_సదిశ_t<గుర్తులేని అక్షరం> ముసుగు_బైట్లు; /* For hintmask/cntrmask payload bytes. */
  op_సంకేతం_t op;

  cs_command_t () : op (Op_సంకేతం_Invalid) {}
  cs_command_t (op_సంకేతం_t op_) : op (op_) {}
};

రకనిర్వచనం hb_సదిశ_t<cs_command_t> *cs_command_vec_t;

నిర్మాణం cff2_instancing_plan_t;

నిర్మాణం flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   తార్కిక drop_hints_,
                   స్థిరరాశి hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = శూన్యసూచిక)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  తార్కిక	drop_hints;
  స్థిరరాశి hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  స్థిరరాశి cff2_instancing_plan_t *instancer = శూన్యసూచిక;
  తార్కిక emitted_blend = అబద్ధం;
};

మూస <రకం_పేరు ACC, రకం_పేరు ENV, రకం_పేరు OPSET, op_సంకేతం_t endchar_op=Op_సంకేతం_Invalid>
నిర్మాణం subr_flattener_t
{
  subr_flattener_t (స్థిరరాశి ACC &acc_,
		    స్థిరరాశి hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  తార్కిక flatten (str_buff_vec_t &flat_charstrings,
                hb_సదిశ_t<hb_సదిశ_t<cs_command_t>> *command_capture = శూన్యసూచిక)
  {
    గుర్తులేని లెక్క = plan->అంకె_output_glyphs ();
    ఒకవేళ (!flat_charstrings.resize_exact (లెక్క))
      తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      hb_codepoint_t  glyph;
      ఒకవేళ (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	ఒకవేళ (endchar_op != Op_సంకేతం_Invalid) flat_charstrings[i].push (endchar_op);
	కొనసాగు;
      }
      స్థిరరాశి hb_ubytes_t str = (*acc.char_అక్షరశ్రేణులు)[glyph];
      గుర్తులేని పూర్ణాంకం fd = subset_get_fd (acc, glyph);
      ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
	తిరిగి అబద్ధం;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.నిడివి);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (తార్కిక) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : శూన్యసూచిక
      };
      ఒకవేళ (unlikely (!interp.interpret (param)))
	తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  స్థిరరాశి ACC &acc;
  స్థిరరాశి hb_subset_plan_t *plan;
};

నిర్మాణం subr_closures_t
{
  subr_closures_t (గుర్తులేని పూర్ణాంకం fd_లెక్క_2) : global_సంవృతం (), local_closures ()
  {
    local_closures.resize_exact (fd_లెక్క_2);
  }

  శూన్య reset ()
  {
    global_సంవృతం.clear();
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < local_closures.నిడివి; i++)
      local_closures[i].clear();
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి local_closures.లోపల_error (); }
  hb_సమితి_t  global_సంవృతం;
  hb_సదిశ_t<hb_సమితి_t> local_closures;
};

నిర్మాణం parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (గుర్తులేని పూర్ణాంకం subr_అంకె = 0) :
    subr_అంకె_2 (subr_అంకె) {}

  తార్కిక is_hinting () స్థిరరాశి { తిరిగి hinting_flag; }
  శూన్య సమితి_hinting ()       { hinting_flag = నిజం; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  రక్షిత:
  తార్కిక	  hinting_flag = అబద్ధం;

  బహిరంగ:
  uint16_t subr_అంకె_2;
};

నిర్మాణం parsed_cs_str_t : parsed_విలువలు_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (అబద్ధం),
    hint_dropped (అబద్ధం),
    has_ఉపసర్గ (అబద్ధం),
    has_calls_ (అబద్ధం),
    coalescing_ (అబద్ధం)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  శూన్య కూడు_op (op_సంకేతం_t op, స్థిరరాశి byte_str_ref_t& str_ref)
  {
    ఒకవేళ (is_parsed ()) తిరిగి;
    ఒకవేళ (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_చివర = చివరి_చివర_2;
      చివరి_చివర_2 = str_ref.get_స్థానభ్రంశం ();
      ఒకవేళ (unlikely (op == Op_సంకేతం_return || op == Op_సంకేతం_endchar))
	flush_segment (str_ref, చివరి_చివర_2);
      తిరిగి;
    }
    SUPER::కూడు_op (op, str_ref);
  }

  శూన్య కూడు_call_op (op_సంకేతం_t op, స్థిరరాశి byte_str_ref_t& str_ref, గుర్తులేని పూర్ణాంకం subr_అంకె_2)
  {
    ఒకవేళ (is_parsed ()) తిరిగి;
    has_calls_ = నిజం;

    ఒకవేళ (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_చివర);
      op_ప్రారంభం = చివరి_చివర_2;
      SUPER::కూడు_op (op, str_ref, {subr_అంకె_2});
      penultimate_చివర = చివరి_చివర_2 = str_ref.get_స్థానభ్రంశం ();
      తిరిగి;
    }

    /* Pop the subroutine number. */
    values.తీయి ();

    SUPER::కూడు_op (op, str_ref, {subr_అంకె_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  శూన్య flush_segment (స్థిరరాశి byte_str_ref_t& str_ref, గుర్తులేని చివర)
  {
    గుర్తులేని ప్రారంభం = op_ప్రారంభం;
    ఒకవేళ (చివర <= ప్రారంభం) తిరిగి;
    అయితే (ప్రారంభం < చివర)
    {
      స్వయంచాలక arr = str_ref.sub_array (ప్రారంభం, hb_min (చివర - ప్రారంభం, 255u));
      ఒకవేళ (unlikely (!arr.నిడివి)) ఆపు;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->నిడివి = arr.నిడివి;
      ప్రారంభం += arr.నిడివి;
    }
    op_ప్రారంభం = చివర;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  శూన్య flush_coalesced (స్థిరరాశి byte_str_ref_t& str_ref)
  {
    ఒకవేళ (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_స్థానభ్రంశం ());
  }

  శూన్య enable_coalescing () { coalescing_ = నిజం; }
  తార్కిక is_coalescing () స్థిరరాశి { తిరిగి coalescing_; }

  శూన్య సమితి_ఉపసర్గ (స్థిరరాశి సంఖ్య_t &అంకె, op_సంకేతం_t op = Op_సంకేతం_Invalid)
  {
    has_ఉపసర్గ = నిజం;
    ఉపసర్గ_op = op;
    ఉపసర్గ_అంకె = అంకె;
  }

  తార్కిక at_చివర (గుర్తులేని పూర్ణాంకం pos) స్థిరరాశి
  {
    తిరిగి ((pos + 1 >= values.నిడివి) /* CFF2 */
	|| (values[pos + 1].op == Op_సంకేతం_return));
  }

  తార్కిక is_parsed () స్థిరరాశి { తిరిగి parsed; }
  శూన్య సమితి_parsed ()      { parsed = నిజం; }

  తార్కిక is_hint_dropped () స్థిరరాశి { తిరిగి hint_dropped; }
  శూన్య సమితి_hint_dropped ()      { hint_dropped = నిజం; }

  తార్కిక is_vsindex_dropped () స్థిరరాశి { తిరిగి vsindex_dropped; }
  శూన్య సమితి_vsindex_dropped ()      { vsindex_dropped = నిజం; }

  తార్కిక has_ఉపసర్గ_2 () స్థిరరాశి          { తిరిగి has_ఉపసర్గ; }
  op_సంకేతం_t ఉపసర్గ_op_2 () స్థిరరాశి         { తిరిగి ఉపసర్గ_op; }
  స్థిరరాశి సంఖ్య_t &ఉపసర్గ_అంకె_2 () స్థిరరాశి { తిరిగి ఉపసర్గ_అంకె; }

  తార్కిక has_calls () స్థిరరాశి          { తిరిగి has_calls_; }

  శూన్య compact ()
  {
    గుర్తులేని లెక్క = values.నిడివి;
    ఒకవేళ (!లెక్క) తిరిగి;
    స్వయంచాలక &opstr = values.arrayZ;
    గుర్తులేని j = 0;
    కొరకు (గుర్తులేని i = 1; i < లెక్క; i++)
    {
      /* See if we can combine op j and op i. */
      తార్కిక combine =
        (opstr[j].op != Op_సంకేతం_callsubr && opstr[j].op != Op_సంకేతం_callgsubr) &&
        (opstr[i].op != Op_సంకేతం_callsubr && opstr[i].op != Op_సంకేతం_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].నిడివి == opstr[i].ptr) &&
        (opstr[j].నిడివి + opstr[i].నిడివి <= 255);

      ఒకవేళ (combine)
      {
	opstr[j].నిడివి += opstr[i].నిడివి;
	opstr[j].op = Op_సంకేతం_Invalid;
      }
      లేకపోతే
      {
	opstr[++j] = opstr[i];
      }
    }
    values.shrink (j + 1);
  }

  రక్షిత:
  తార్కిక    parsed : 1;
  తార్కిక    hint_dropped : 1;
  తార్కిక    vsindex_dropped : 1;
  తార్కిక    has_ఉపసర్గ : 1;
  తార్కిక    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  తార్కిక    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  గుర్తులేని penultimate_చివర = 0;
  గుర్తులేని చివరి_చివర_2 = 0;
  op_సంకేతం_t	ఉపసర్గ_op;
  సంఖ్య_t	ఉపసర్గ_అంకె;

  అంతరంగ:
  రకనిర్వచనం parsed_విలువలు_t<parsed_cs_op_t> SUPER;
};

నిర్మాణం parsed_cs_str_vec_t : hb_సదిశ_t<parsed_cs_str_t>
{
  అంతరంగ:
  రకనిర్వచనం hb_సదిశ_t<parsed_cs_str_t> SUPER;
};

నిర్మాణం cff_subset_accelerator_t
{
  స్థిర cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      స్థిరరాశి parsed_cs_str_vec_t& parsed_charstrings,
      స్థిరరాశి parsed_cs_str_vec_t& parsed_global_subrs,
      స్థిరరాశి hb_సదిశ_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (పరిమాణం(cff_subset_accelerator_t));
    ఒకవేళ (unlikely (!accel)) తిరిగి శూన్యసూచిక;
    కొత్త (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    తిరిగి accel;
  }

  స్థిర శూన్య destroy (శూన్య* value) {
    ఒకవేళ (!value) తిరిగి;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      స్థిరరాశి parsed_cs_str_vec_t& parsed_charstrings_,
      స్థిరరాశి parsed_cs_str_vec_t& parsed_global_subrs_,
      స్థిరరాశి hb_సదిశ_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    స్వయంచాలక *mapping = glyph_గమ్యం_sid_map.get_relaxed ();
    ఒకవేళ (mapping)
    {
      mapping->~glyph_గమ్యం_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_సదిశ_t<parsed_cs_str_vec_t> parsed_local_subrs;
  మార్చదగిన hb_atomic_t<glyph_గమ్యం_sid_map_t *> glyph_గమ్యం_sid_map;

 అంతరంగ:
  hb_blob_t* original_blob;
};

నిర్మాణం subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_సమితి_t *global_సంవృతం_2,
		       hb_సమితి_t *local_సంవృతం,
		       తార్కిక drop_hints_,
		       తార్కిక coalesce_ = అబద్ధం) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_సంవృతం (global_సంవృతం_2),
      local_సంవృతం_2 (local_సంవృతం),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    ఒకవేళ (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    ఎంపిక (context.type)
    {
      స్థితి CSType_CharString:
	తిరిగి parsed_charstring;

      స్థితి CSType_LocalSubr:
	ఒకవేళ (likely (context.subr_అంకె_2 < parsed_local_subrs->నిడివి))
	  తిరిగి &(*parsed_local_subrs)[context.subr_అంకె_2];
	ఆపు;

      స్థితి CSType_GlobalSubr:
	ఒకవేళ (likely (context.subr_అంకె_2 < parsed_global_subrs->నిడివి))
	  తిరిగి &(*parsed_global_subrs)[context.subr_అంకె_2];
	ఆపు;
    }
    తిరిగి శూన్యసూచిక;
  }

  మూస <రకం_పేరు ENV>
  శూన్య సమితి_current_str (ENV &env, తార్కిక calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    ఒకవేళ (unlikely (!parsed_str))
    {
      env.సమితి_error ();
      తిరిగి;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    ఒకవేళ (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.నిడివి > 0)))
      env.సమితి_error ();
    లేకపోతే
    {
      ఒకవేళ (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.మొత్తం_కొలత ());
        ఒకవేళ (coalesce) parsed_str->enable_coalescing ();
      }
      current_parsed_str = parsed_str;
    }
  }

  parsed_cs_str_t	*current_parsed_str;

  parsed_cs_str_t	*parsed_charstring;
  parsed_cs_str_vec_t	*parsed_global_subrs;
  parsed_cs_str_vec_t	*parsed_local_subrs;
  hb_సమితి_t      *global_సంవృతం;
  hb_సమితి_t      *local_సంవృతం_2;
  తార్కిక	  drop_hints;
  తార్కిక	  coalesce;
};

నిర్మాణం subr_remap_t : hb_inc_bimap_t
{
  శూన్య create (స్థిరరాశి hb_సమితి_t *సంవృతం)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (సంవృతం->get_population ());
    కొరకు (స్వయంచాలక old_అంకె : *సంవృతం)
      కూడు (old_అంకె);

    ఒకవేళ (get_population () < 1240)
      bias = 107;
    లేకపోతే ఒకవేళ (get_population () < 33900)
      bias = 1131;
    లేకపోతే
      bias = 32768;
  }

  పూర్ణాంకం biased_అంకె (గుర్తులేని పూర్ణాంకం old_అంకె) స్థిరరాశి
  {
    hb_codepoint_t new_అంకె = get (old_అంకె);
    తిరిగి (పూర్ణాంకం)new_అంకె - bias;
  }

  రక్షిత:
  పూర్ణాంకం bias;
};

నిర్మాణం subr_remaps_t
{
  subr_remaps_t (గుర్తులేని పూర్ణాంకం fd_లెక్క)
  {
    local_remaps.resize (fd_లెక్క);
  }

  తార్కిక లోపల_error()
  {
    తిరిగి local_remaps.లోపల_error ();
  }

  శూన్య create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_సంవృతం);
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < local_remaps.నిడివి; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_సదిశ_t<subr_remap_t>  local_remaps;
};

మూస <రకం_పేరు SUBSETTER, రకం_పేరు SUBRS, రకం_పేరు ACC, రకం_పేరు ENV, రకం_పేరు OPSET, op_సంకేతం_t endchar_op=Op_సంకేతం_Invalid>
నిర్మాణం subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, స్థిరరాశి hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_లెక్క),
        remaps(acc_.fd_లెక్క)
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
  తార్కిక subset (శూన్య)
  {
    గుర్తులేని fd_లెక్క_2 = acc.fd_లెక్క;
    స్థిరరాశి cff_subset_accelerator_t* cff_accelerator = శూన్యసూచిక;
    ఒకవేళ (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_లెక్క_2 = cff_accelerator->parsed_local_subrs.నిడివి;
    }

    ఒకవేళ (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->అంకె_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } లేకపోతే {
      parsed_charstrings.resize_exact (plan->అంకె_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->లెక్క);

      ఒకవేళ (unlikely (!parsed_local_subrs_storage.resize (fd_లెక్క_2))) తిరిగి అబద్ధం;

      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < acc.fd_లెక్క; i++)
      {
        గుర్తులేని లెక్క = acc.వ్యక్తిగత_Dicts[i].localSubrs->లెక్క;
        parsed_local_subrs_storage[i].resize (లెక్క);
        ఒకవేళ (unlikely (parsed_local_subrs_storage[i].లోపల_error ())) తిరిగి అబద్ధం;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    ఒకవేళ (unlikely (remaps.లోపల_error()
                  || cached_charstrings.లోపల_error ()
                  || parsed_charstrings.లోపల_error ()
                  || parsed_global_subrs->లోపల_error ()
                  || closures.లోపల_error ())) {
      తిరిగి అబద్ధం;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    తార్కిక coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    కొరకు (స్వయంచాలక _ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.మొదటి;
      hb_codepoint_t old_glyph = _.second;

      స్థిరరాశి hb_ubytes_t str = (*acc.char_అక్షరశ్రేణులు)[old_glyph];
      గుర్తులేని పూర్ణాంకం fd = subset_get_fd (acc, old_glyph);
      ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
        తిరిగి అబద్ధం;

      ఒకవేళ (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        ఒకవేళ (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        లేకపోతే
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        కొనసాగు;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.నిడివి);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_సంవృతం,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      ఒకవేళ (unlikely (!interp.interpret (param)))
        తిరిగి అబద్ధం;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      ఒకవేళ ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_సంవృతం,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	ఒకవేళ (drop_hints_లోపల_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].సమితి_hint_dropped ();
	  ఒకవేళ (drop.vsindex_dropped)
	    parsed_charstrings[new_glyph].సమితి_vsindex_dropped ();
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
      ఒకవేళ (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    ఒకవేళ ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !సంవృతం_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      తిరిగి అబద్ధం;

    remaps.create (closures);

    populate_subset_accelerator ();
    తిరిగి నిజం;
  }

  తార్కిక encode_charstrings (str_buff_vec_t &buffArray, తార్కిక encode_ఉపసర్గ = నిజం) స్థిరరాశి
  {
    గుర్తులేని అంకె_glyphs = plan->అంకె_output_glyphs ();
    ఒకవేళ (unlikely (!buffArray.resize_exact (అంకె_glyphs)))
      తిరిగి అబద్ధం;
    hb_codepoint_t చివరి = 0;
    కొరకు (స్వయంచాలక _ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t gid = _.మొదటి;
      hb_codepoint_t old_glyph = _.second;

      ఒకవేళ (endchar_op != Op_సంకేతం_Invalid)
        కొరకు (; చివరి < gid; చివరి++)
	{
	  // Hack to point vector to static string.
	  స్వయంచాలక &b = buffArray.arrayZ[చివరి];
	  b.సమితి_storage (const_cast<గుర్తులేని అక్షరం *>(endchar_str), 1);
	}

      చివరి++; // Skip over gid
      గుర్తులేని పూర్ణాంకం  fd = subset_get_fd (acc, old_glyph);
      ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
	తిరిగి అబద్ధం;
      ఒకవేళ (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_ఉపసర్గ)))
	తిరిగి అబద్ధం;
    }
    ఒకవేళ (endchar_op != Op_సంకేతం_Invalid)
      కొరకు (; చివరి < అంకె_glyphs; చివరి++)
      {
	// Hack to point vector to static string.
	స్వయంచాలక &b = buffArray.arrayZ[చివరి];
	b.సమితి_storage (const_cast<గుర్తులేని అక్షరం *>(endchar_str), 1);
      }

    తిరిగి నిజం;
  }

  తార్కిక encode_subrs (స్థిరరాశి parsed_cs_str_vec_t &subrs, స్థిరరాశి subr_remap_t& remap, గుర్తులేని పూర్ణాంకం fd, str_buff_vec_t &buffArray) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం  లెక్క = remap.get_population ();

    ఒకవేళ (unlikely (!buffArray.resize_exact (లెక్క)))
      తిరిగి అబద్ధం;
    కొరకు (గుర్తులేని పూర్ణాంకం new_అంకె = 0; new_అంకె < లెక్క; new_అంకె++)
    {
      hb_codepoint_t old_అంకె = remap.backward (new_అంకె);
      ప్రతిజ్ఞా (old_అంకె != CFF_UNDEF_సంకేతం);

      ఒకవేళ (unlikely (!encode_str (subrs[old_అంకె], fd, buffArray[new_అంకె])))
	తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  తార్కిక encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    తిరిగి encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  తార్కిక encode_localsubrs (గుర్తులేని పూర్ణాంకం fd, str_buff_vec_t &buffArray) స్థిరరాశి
  {
    తిరిగి encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  రక్షిత:
  నిర్మాణం drop_hints_param_t
  {
    drop_hints_param_t ()
      : చూసిన_moveto (అబద్ధం),
	చివర్లు_లోపల_hint (అబద్ధం),
	all_dropped (అబద్ధం),
	vsindex_dropped (అబద్ధం) {}

    తార్కిక  చూసిన_moveto;
    తార్కిక  చివర్లు_లోపల_hint;
    తార్కిక  all_dropped;
    తార్కిక  vsindex_dropped;
  };

  తార్కిక drop_hints_లోపల_subr (parsed_cs_str_t &str, గుర్తులేని పూర్ణాంకం pos,
			   parsed_cs_str_vec_t &subrs, గుర్తులేని పూర్ణాంకం subr_అంకె_2,
			   స్థిరరాశి subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.చివర్లు_లోపల_hint = అబద్ధం;
    తార్కిక has_hint = drop_hints_లోపల_str (subrs[subr_అంకె_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    ఒకవేళ (drop.చివర్లు_లోపల_hint)
    {
      str.values[pos].సమితి_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      ఒకవేళ (!str.at_చివర (pos))
	drop.చివర్లు_లోపల_hint = అబద్ధం;
    }
    లేకపోతే ఒకవేళ (drop.all_dropped)
    {
      str.values[pos].సమితి_hinting ();
    }

    తిరిగి has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  తార్కిక drop_hints_లోపల_str (parsed_cs_str_t &str, స్థిరరాశి subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    తార్కిక  చూసిన_hint = అబద్ధం;

    గుర్తులేని లెక్క = str.values.నిడివి;
    స్వయంచాలక *values = str.values.arrayZ;
    కొరకు (గుర్తులేని పూర్ణాంకం pos = 0; pos < లెక్క; pos++)
    {
      తార్కిక  has_hint = అబద్ధం;
      ఎంపిక (values[pos].op)
      {
	స్థితి Op_సంకేతం_callsubr:
	  has_hint = drop_hints_లోపల_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_అంకె_2,
					param, drop);
	  ఆపు;

	స్థితి Op_సంకేతం_callgsubr:
	  has_hint = drop_hints_లోపల_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_అంకె_2,
					param, drop);
	  ఆపు;

	స్థితి Op_సంకేతం_rmoveto:
	స్థితి Op_సంకేతం_hmoveto:
	స్థితి Op_సంకేతం_vmoveto:
	  drop.చూసిన_moveto = నిజం;
	  ఆపు;

	స్థితి Op_సంకేతం_hintmask:
	స్థితి Op_సంకేతం_cntrmask:
	  ఒకవేళ (drop.చూసిన_moveto)
	  {
	    values[pos].సమితి_hinting ();
	    ఆపు;
	  }
	  HB_FALLTHROUGH;

	స్థితి Op_సంకేతం_hstemhm:
	స్థితి Op_సంకేతం_vstemhm:
	స్థితి Op_సంకేతం_hstem:
	స్థితి Op_సంకేతం_vstem:
	  has_hint = నిజం;
	  values[pos].సమితి_hinting ();
	  ఒకవేళ (str.at_చివర (pos))
	    drop.చివర్లు_లోపల_hint = నిజం;
	  ఆపు;

	స్థితి Op_సంకేతం_dotsection:
	  values[pos].సమితి_hinting ();
	  ఆపు;

	అప్రమేయ:
	  /* NONE */
	  ఆపు;
      }
      ఒకవేళ (has_hint)
      {
	కొరకు (పూర్ణాంకం i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(గుర్తులేని)i];
	  ఒకవేళ (csop.is_hinting ())
	    ఆపు;
	  csop.సమితి_hinting ();
	  ఒకవేళ (csop.op == Op_సంకేతం_vsindexcs)
	    drop.vsindex_dropped = నిజం;
	}
	చూసిన_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = నిజం;
    కొరకు (గుర్తులేని పూర్ణాంకం pos = 0; pos < లెక్క; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      ఒకవేళ (csop.op == Op_సంకేతం_return)
	ఆపు;
      ఒకవేళ (!csop.is_hinting ())
      {
	drop.all_dropped = అబద్ధం;
	ఆపు;
      }
    }

    తిరిగి చూసిన_hint;
  }

  తార్కిక సంవృతం_subroutines (స్థిరరాశి parsed_cs_str_vec_t& global_subrs,
                            స్థిరరాశి hb_సదిశ_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    కొరకు (స్వయంచాలక _ : plan->new_గమ్యం_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.మొదటి;
      hb_codepoint_t old_glyph = _.second;
      గుర్తులేని పూర్ణాంకం fd = subset_get_fd (acc, old_glyph);
      ఒకవేళ (unlikely (fd >= acc.fd_లెక్క))
        తిరిగి అబద్ధం;

      // Note: const cast is safe here because the collect_subr_refs_in_str only performs a
      //       closure and does not modify any of the charstrings.
      subr_subset_param_t  param (const_cast<parsed_cs_str_t*> (&get_parsed_charstring (new_glyph)),
                                  const_cast<parsed_cs_str_vec_t*> (&global_subrs),
                                  const_cast<parsed_cs_str_vec_t*> (&local_subrs[fd]),
                                  &closures.global_సంవృతం,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
      collect_subr_refs_లోపల_str (get_parsed_charstring (new_glyph), param);
    }

    తిరిగి నిజం;
  }

  శూన్య collect_subr_refs_లోపల_subr (గుర్తులేని పూర్ణాంకం subr_అంకె_2, parsed_cs_str_vec_t &subrs,
				  hb_సమితి_t *సంవృతం,
				  స్థిరరాశి subr_subset_param_t &param)
  {
    ఒకవేళ (సంవృతం->has (subr_అంకె_2))
      తిరిగి;
    సంవృతం->కూడు (subr_అంకె_2);
    collect_subr_refs_లోపల_str (subrs[subr_అంకె_2], param);
  }

  శూన్య collect_subr_refs_లోపల_str (స్థిరరాశి parsed_cs_str_t &str,
                                 స్థిరరాశి subr_subset_param_t &param)
  {
    ఒకవేళ (!str.has_calls ())
      తిరిగి;

    కొరకు (స్వయంచాలక &opstr : str.values)
    {
      ఒకవేళ (!param.drop_hints || !opstr.is_hinting ())
      {
	ఎంపిక (opstr.op)
	{
	  స్థితి Op_సంకేతం_callsubr:
	    collect_subr_refs_లోపల_subr (opstr.subr_అంకె_2, *param.parsed_local_subrs,
				       param.local_సంవృతం_2, param);
	    ఆపు;

	  స్థితి Op_సంకేతం_callgsubr:
	    collect_subr_refs_లోపల_subr (opstr.subr_అంకె_2, *param.parsed_global_subrs,
				       param.global_సంవృతం, param);
	    ఆపు;

	  అప్రమేయ: ఆపు;
	}
      }
    }
  }

  తార్కిక encode_str (స్థిరరాశి parsed_cs_str_t &str, స్థిరరాశి గుర్తులేని పూర్ణాంకం fd, str_buff_t &buff, తార్కిక encode_ఉపసర్గ = నిజం) స్థిరరాశి
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    తార్కిక hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    ఒకవేళ (encode_ఉపసర్గ && str.has_ఉపసర్గ_2 () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_అంకె_cs (str.ఉపసర్గ_అంకె_2 ());
      ఒకవేళ (str.ఉపసర్గ_op_2 () != Op_సంకేతం_Invalid)
	encoder.encode_op (str.ఉపసర్గ_op_2 ());
    }

    గుర్తులేని కొలత = 0;
    కొరకు (స్వయంచాలక &opstr : str.values)
    {
      కొలత += opstr.నిడివి;
      ఒకవేళ (opstr.op == Op_సంకేతం_callsubr || opstr.op == Op_సంకేతం_callgsubr)
        కొలత += 3;
    }
    ఒకవేళ (!buff.alloc_exact (buff.నిడివి + కొలత))
      తిరిగి అబద్ధం;

    కొరకు (స్వయంచాలక &opstr : str.values)
    {
      ఒకవేళ (hinting || !opstr.is_hinting ())
      {
	ఎంపిక (opstr.op)
	{
	  స్థితి Op_సంకేతం_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_అంకె (opstr.subr_అంకె_2));
	    encoder.copy_str (opstr.ptr, opstr.నిడివి);
	    ఆపు;

	  స్థితి Op_సంకేతం_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_అంకె (opstr.subr_అంకె_2));
	    encoder.copy_str (opstr.ptr, opstr.నిడివి);
	    ఆపు;

	  అప్రమేయ:
	    encoder.copy_str (opstr.ptr, opstr.నిడివి);
	    ఆపు;
	}
      }
    }
    తిరిగి !encoder.లోపల_error ();
  }

  శూన్య compact_parsed_subrs () స్థిరరాశి
  {
    కొరకు (స్వయంచాలక &cs : parsed_global_subrs_storage)
      cs.compact ();
    కొరకు (స్వయంచాలక &vec : parsed_local_subrs_storage)
      కొరకు (స్వయంచాలక &cs : vec)
	cs.compact ();
  }

  శూన్య populate_subset_accelerator () స్థిరరాశి
  {
    ఒకవేళ (!plan->inprogress_accelerator) తిరిగి;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  స్థిరరాశి parsed_cs_str_t& get_parsed_charstring (గుర్తులేని i) స్థిరరాశి
  {
    ఒకవేళ (cached_charstrings) తిరిగి *(cached_charstrings[i]);
    తిరిగి parsed_charstrings[i];
  }

  రక్షిత:
  స్థిరరాశి ACC			&acc;
  స్థిరరాశి hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_సదిశ_t<స్థిరరాశి parsed_cs_str_t*>     cached_charstrings;
  స్థిరరాశి parsed_cs_str_vec_t*              parsed_global_subrs;
  స్థిరరాశి hb_సదిశ_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  అంతరంగ:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_సదిశ_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  రకనిర్వచనం రకం_పేరు SUBRS::లెక్క_type subr_లెక్క_type;
};

} /* namespace CFF */

HB_అంతర్గత తార్కిక
hb_plan_subset_cff_fdselect (స్థిరరాశి hb_subset_plan_t *plan,
			    గుర్తులేని పూర్ణాంకం fd_లెక్క,
			    స్థిరరాశి CFF::FDSelect &src, /*లోపల*/
			    గుర్తులేని పూర్ణాంకం src_అంకె_glyphs,
			    గుర్తులేని పూర్ణాంకం &subset_fd_లెక్క /*బహిర్గతం*/,
			    గుర్తులేని పూర్ణాంకం &subset_fdselect_కొలత /*బహిర్గతం*/,
			    గుర్తులేని పూర్ణాంకం &subset_fdselect_format /*బహిర్గతం*/,
			    hb_సదిశ_t<CFF::సంకేతం_pair_t> &fdselect_ranges /*బహిర్గతం*/,
			    hb_inc_bimap_t &fdmap /*బహిర్గతం*/);

HB_అంతర్గత తార్కిక
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  గుర్తులేని పూర్ణాంకం అంకె_glyphs,
			  స్థిరరాశి CFF::FDSelect &src,
			  గుర్తులేని పూర్ణాంకం fd_లెక్క_2,
			  గుర్తులేని పూర్ణాంకం fdselect_format,
			  గుర్తులేని పూర్ణాంకం కొలత,
			  స్థిరరాశి hb_సదిశ_t<CFF::సంకేతం_pair_t> &fdselect_ranges);

#అయితే_ముగింపు /* HB_SUBSET_CFF_COMMON_HH */
