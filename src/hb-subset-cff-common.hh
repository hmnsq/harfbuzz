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

#अगर_अघोषित HB_SUBSET_CFF_COMMON_HH
#घोषणा HB_SUBSET_CFF_COMMON_HH

#समावेश "hb.hh"

#समावेश "hb-subset-plan.hh"
#समावेश "hb-cff-interp-cs-common.hh"

namespace CFF {

/* CFF2 CharStrings may omit a tail of glyphs provided by VARC, in which
 * case FDSelect only covers the CharStrings glyphs.  Use FD 0 for glyphs
 * beyond that. */
ढाँचा <typename ACC>
अटल अंतरभूत अचिन्हित
subset_get_fd (स्थिर ACC &acc, hb_codepoint_t glyph)
{
  वापस likely (glyph < acc.अंक_glyphs) ? acc.fdSelect->get_fd (glyph) : 0;
}

/* Used for writing a temporary charstring */
काष्ठा str_encoder_t
{
  str_encoder_t (str_buff_t &buff_)
    : buff (buff_) {}

  व्योम reset () { buff.reset (); }

  व्योम encode_byte (अचिन्हित अक्षर b)
  {
    अगर (likely ((चिन्हित) buff.लंबाई < buff.allocated))
      buff.arrayZ[buff.लंबाई++] = b;
    अन्यथा
      buff.push (b);
  }

  व्योम encode_int (पूर्णांक v)
  {
    अगर ((-1131 <= v) && (v <= 1131))
    {
      अगर ((-107 <= v) && (v <= 107))
	encode_byte (v + 139);
      अन्यथा अगर (v > 0)
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
      अगर (unlikely (v < -32768))
	v = -32768;
      अन्यथा अगर (unlikely (v > 32767))
	v = 32767;
      encode_byte (OpCode_shortint);
      encode_byte ((v >> 8) & 0xFF);
      encode_byte (v & 0xFF);
    }
  }

  // Encode number for CharString
  व्योम encode_अंक_cs (स्थिर संख्या_t& n)
  {
    अगर (n.in_int_range ())
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
  व्योम encode_अंक_tp (स्थिर संख्या_t& n)
  {
    अगर (n.in_int_range ())
    {
      // TODO longint
      encode_int (n.to_int ());
    }
    अन्यथा
    {
      // Sigh. BCD
      // https://learn.microsoft.com/en-us/typography/opentype/spec/cff2#table-5-nibble-definitions
      द्विगुन v = n.to_real ();
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
      oldlocale = hb_uselocale (clocale = newlocale (LC_ALL_MASK, "C", शून्य));
      snprintf (buf, माप (buf), "%.8G", v);
      (व्योम) hb_uselocale (((व्योम) freelocale (clocale), oldlocale));

      अक्षर *s = buf;
      माप_प्रकार len;
      अक्षर *comma = म_अक्षर (s, ',');
      अगर (comma) // Comma for some European locales in case no uselocale available.
	*comma = '.';
      अगर (s[0] == '0' && s[1] == '.')
	s++;
      अन्यथा अगर (s[0] == '-' && s[1] == '0' && s[2] == '.')
      {
	s[1] = '-';
	s++;
      }
      अन्यथा अगर ((len = म_माप (s)) > 3 && !म_भेद (s + len - 3, "000"))
      {
	अचिन्हित exponent = len - 3;
	अक्षर *s2 = s + exponent - 1;
	जबतक (*s2 == '0' && exponent > 1)
	{
	  s2--;
	  exponent++;
	}
	snprintf (s2 + 1, माप (buf) - (s2 + 1 - buf), "E%u", exponent);
      }
      अन्यथा
      {
	अक्षर *dot = म_अक्षर (s, '.');
	अक्षर *e = म_अक्षर (s, 'E');
	अगर (dot && e)
	{
	  स_हटाओ (dot, dot + 1, e - (dot + 1));
	  पूर्णांक exponent = म_से_प (e + 1);
	  पूर्णांक new_exponent = exponent - (e - (dot + 1));
	  अगर (new_exponent == 1)
	  {
	    e[-1] = '0';
	    e[0] = '\0';
	  }
	  अन्यथा
	    snprintf (e - 1, माप (buf) - (e - 1 - buf), "E%d", new_exponent);
	}
      }
      अगर ((s[0] == '.' && s[1] == '0') || (s[0] == '-' && s[1] == '.' && s[2] == '0'))
      {
	पूर्णांक sign = s[0] == '-';
	अक्षर *s2 = s + sign + 1;
	जबतक (*s2 == '0')
	  s2++;
	len = म_माप (s2);
	स_हटाओ (s + sign, s2, len);
	snprintf (s + sign + len, माप (buf) - (s + sign + len - buf), "E-%u", (अचिन्हित) (म_माप (s + sign) - 1));
      }
      hb_vector_t<अक्षर> nibbles;
      जबतक (*s)
      {
	अक्षर c = s[0];
	s++;

	चयन (c)
	{
	  हाल 'E':
	  {
	    अक्षर c2 = *s;
	    अगर (c2 == '-')
	    {
	      s++;
	      nibbles.push (0x0C); // E-
	    } अन्यथा {
	      अगर (c2 == '+')
		s++;
	      nibbles.push (0x0B); // E
	    }
	    अगर (*s == '0')
	      s++;
	    जारी;
	  }

	  हाल '.':
	    nibbles.push (0x0A); // .
	    जारी;

	  हाल '-':
	    nibbles.push (0x0E); // -
	    जारी;
	}

	nibbles.push (c - '0');
      }
      nibbles.push (0x0F);
      अगर (nibbles.लंबाई % 2)
	nibbles.push (0x0F);

      अचिन्हित गिनती = nibbles.लंबाई;
      क्रम (अचिन्हित i = 0; i < गिनती; i += 2)
        encode_byte ((nibbles[i] << 4) | nibbles[i+1]);
    }
  }

  व्योम encode_op (op_code_t op)
  {
    अगर (Is_OpCode_ESC (op))
    {
      encode_byte (OpCode_escape);
      encode_byte (Unmake_OpCode_ESC (op));
    }
    अन्यथा
      encode_byte (op);
  }

  व्योम copy_str (स्थिर अचिन्हित अक्षर *str, अचिन्हित लंबाई)
  {
    निश्चित ((चिन्हित) (buff.लंबाई + लंबाई) <= buff.allocated);
    hb_memcpy (buff.arrayZ + buff.लंबाई, str, लंबाई);
    buff.लंबाई += लंबाई;
  }

  bool in_error () स्थिर { वापस buff.in_error (); }

  रक्षित:

  str_buff_t &buff;
};

काष्ठा cff_sub_table_info_t {
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
काष्ठा cff_शीर्ष_dict_op_serializer_t : op_serializer_t
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर OPSTR &opstr,
		  स्थिर cff_sub_table_info_t &info) स्थिर
  {
    TRACE_SERIALIZE (यह);

    चयन (opstr.op)
    {
      हाल OpCode_CharStrings:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.char_strings_link, whence_t::Absolute));

      हाल OpCode_FDArray:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_array_link, whence_t::Absolute));

      हाल OpCode_FDSelect:
	return_trace (FontDict::serialize_link4_op(c, opstr.op, info.fd_select.link, whence_t::Absolute));

      शेष:
	return_trace (copy_opstr (c, opstr));
    }
    return_trace (true);
  }
};

काष्ठा cff_font_dict_op_serializer_t : op_serializer_t
{
  bool serialize (hb_serialize_context_t *c,
		  स्थिर op_str_t &opstr,
		  स्थिर table_info_t &privateDictInfo) स्थिर
  {
    TRACE_SERIALIZE (यह);

    अगर (opstr.op == OpCode_Private)
    {
      /* serialize the private dict size & offset as 2-byte & 4-byte integers */
      return_trace (UnsizedByteStr::serialize_int2 (c, privateDictInfo.आकार) &&
		    Dict::serialize_link4_op (c, opstr.op, privateDictInfo.link, whence_t::Absolute));
    }
    अन्यथा
    {
      अचिन्हित अक्षर *d = c->allocate_आकार<अचिन्हित अक्षर> (opstr.लंबाई);
      अगर (unlikely (!d)) return_trace (false);
      /* Faster than hb_memcpy for small strings. */
      क्रम (अचिन्हित i = 0; i < opstr.लंबाई; i++)
	d[i] = opstr.ptr[i];
      //hb_memcpy (d, opstr.ptr, opstr.length);
    }
    return_trace (true);
  }
};

/* CharString command for specialization */
काष्ठा cs_command_t
{
  hb_vector_t<संख्या_t> args;
  hb_vector_t<अचिन्हित अक्षर> mask_bytes; /* For hintmask/cntrmask payload bytes. */
  op_code_t op;

  cs_command_t () : op (OpCode_Invalid) {}
  cs_command_t (op_code_t op_) : op (op_) {}
};

प्रकार hb_vector_t<cs_command_t> *cs_command_vec_t;

काष्ठा cff2_instancing_plan_t;

काष्ठा flatten_param_t
{
  flatten_param_t (str_buff_t &flatStr_,
                   bool drop_hints_,
                   स्थिर hb_subset_plan_t *plan_,
                   cs_command_vec_t commands_ = nullptr)
    : flatStr (flatStr_), drop_hints (drop_hints_), plan (plan_), commands (commands_) {}

  str_buff_t     &flatStr;
  bool	drop_hints;
  स्थिर hb_subset_plan_t *plan;
  cs_command_vec_t commands; /* Optional: capture parsed commands for specialization */

  /* CFF2 partial instancing: when set, blends are rewritten against the
   * instanced variation store instead of being copied or flattened. */
  स्थिर cff2_instancing_plan_t *instancer = nullptr;
  bool emitted_blend = false;
};

ढाँचा <typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
काष्ठा subr_flattener_t
{
  subr_flattener_t (स्थिर ACC &acc_,
		    स्थिर hb_subset_plan_t *plan_)
		   : acc (acc_), plan (plan_) {}

  bool flatten (str_buff_vec_t &flat_charstrings,
                hb_vector_t<hb_vector_t<cs_command_t>> *command_capture = nullptr)
  {
    अचिन्हित गिनती = plan->अंक_output_glyphs ();
    अगर (!flat_charstrings.resize_exact (गिनती))
      वापस false;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      hb_codepoint_t  glyph;
      अगर (!plan->old_gid_for_new_gid (i, &glyph))
      {
	/* add an endchar only charstring for a missing glyph if CFF1 */
	अगर (endchar_op != OpCode_Invalid) flat_charstrings[i].push (endchar_op);
	जारी;
      }
      स्थिर hb_ubytes_t str = (*acc.charStrings)[glyph];
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, glyph);
      अगर (unlikely (fd >= acc.fd_गिनती))
	वापस false;


      ENV env (str, acc, fd,
	       plan->normalized_coords.arrayZ, plan->normalized_coords.लंबाई);
      cs_interpreter_t<ENV, OPSET, flatten_param_t> interp (env);
      flatten_param_t  param = {
        flat_charstrings.arrayZ[i],
        (bool) (plan->flags & HB_SUBSET_FLAGS_NO_HINTING),
	plan,
	command_capture ? &(*command_capture)[i] : nullptr
      };
      अगर (unlikely (!interp.interpret (param)))
	वापस false;
    }
    वापस true;
  }

  स्थिर ACC &acc;
  स्थिर hb_subset_plan_t *plan;
};

काष्ठा subr_closures_t
{
  subr_closures_t (अचिन्हित पूर्णांक fd_गिनती_2) : global_closure (), local_closures ()
  {
    local_closures.resize_exact (fd_गिनती_2);
  }

  व्योम reset ()
  {
    global_closure.सफा();
    क्रम (अचिन्हित पूर्णांक i = 0; i < local_closures.लंबाई; i++)
      local_closures[i].सफा();
  }

  bool in_error () स्थिर { वापस local_closures.in_error (); }
  hb_set_t  global_closure;
  hb_vector_t<hb_set_t> local_closures;
};

काष्ठा parsed_cs_op_t : op_str_t
{
  parsed_cs_op_t (अचिन्हित पूर्णांक subr_अंक = 0) :
    subr_अंक_2 (subr_अंक) {}

  bool is_hinting () स्थिर { वापस hinting_flag; }
  व्योम set_hinting ()       { hinting_flag = true; }

  /* The layout of this struct is designed to fit within the
   * padding of op_str_t! */

  रक्षित:
  bool	  hinting_flag = false;

  खुला:
  uint16_t subr_अंक_2;
};

काष्ठा parsed_cs_str_t : parsed_मूल्यावली_t<parsed_cs_op_t>
{
  parsed_cs_str_t () :
    parsed (false),
    hint_dropped (false),
    has_prefix_ (false),
    has_calls_ (false),
    coalescing_ (false)
  {
    SUPER::init ();
  }

  HB_ALWAYS_INLINE
  व्योम add_op (op_code_t op, स्थिर byte_str_ref_t& str_ref)
  {
    अगर (is_parsed ()) वापस;
    अगर (coalescing_)
    {
      /* Do not record individual tokens; only note their boundaries.
       * Bytes are flushed as verbatim segments at call sites and at
       * the end of the string.  Only enabled when per-op granularity
       * is not needed later for hint analysis. */
      penultimate_end_ = last_end_;
      last_end_ = str_ref.get_offset ();
      अगर (unlikely (op == OpCode_return || op == OpCode_endchar))
	flush_segment (str_ref, last_end_);
      वापस;
    }
    SUPER::add_op (op, str_ref);
  }

  व्योम add_call_op (op_code_t op, स्थिर byte_str_ref_t& str_ref, अचिन्हित पूर्णांक subr_अंक_2)
  {
    अगर (is_parsed ()) वापस;
    has_calls_ = true;

    अगर (coalescing_)
    {
      /* Flush bytes preceding the subroutine-number token, then skip
       * over the number: it is re-encoded with the new bias. */
      flush_segment (str_ref, penultimate_end_);
      opStart = last_end_;
      SUPER::add_op (op, str_ref, {subr_अंक_2});
      penultimate_end_ = last_end_ = str_ref.get_offset ();
      वापस;
    }

    /* Pop the subroutine number. */
    values.निकालो ();

    SUPER::add_op (op, str_ref, {subr_अंक_2});
  }

  /* Flush pending bytes [opStart, end) as verbatim segment entries. */
  व्योम flush_segment (स्थिर byte_str_ref_t& str_ref, अचिन्हित end)
  {
    अचिन्हित start = opStart;
    अगर (end <= start) वापस;
    जबतक (start < end)
    {
      स्वतः arr = str_ref.sub_array (start, hb_min (end - start, 255u));
      अगर (unlikely (!arr.लंबाई)) अवरोध;
      parsed_cs_op_t *val = values.push ();
      val->ptr = arr.arrayZ;
      val->लंबाई = arr.लंबाई;
      start += arr.लंबाई;
    }
    opStart = end;
  }

  /* For coalescing mode: flush any pending bytes through the current
   * position; used where the string ends without an explicit
   * return/endchar op (CFF2). */
  व्योम flush_coalesced (स्थिर byte_str_ref_t& str_ref)
  {
    अगर (coalescing_ && !is_parsed ())
      flush_segment (str_ref, str_ref.get_offset ());
  }

  व्योम enable_coalescing () { coalescing_ = true; }
  bool is_coalescing () स्थिर { वापस coalescing_; }

  व्योम set_prefix (स्थिर संख्या_t &अंक, op_code_t op = OpCode_Invalid)
  {
    has_prefix_ = true;
    prefix_op_ = op;
    prefix_अंक = अंक;
  }

  bool at_end (अचिन्हित पूर्णांक pos) स्थिर
  {
    वापस ((pos + 1 >= values.लंबाई) /* CFF2 */
	|| (values[pos + 1].op == OpCode_return));
  }

  bool is_parsed () स्थिर { वापस parsed; }
  व्योम set_parsed ()      { parsed = true; }

  bool is_hint_dropped () स्थिर { वापस hint_dropped; }
  व्योम set_hint_dropped ()      { hint_dropped = true; }

  bool is_vsindex_dropped () स्थिर { वापस vsindex_dropped; }
  व्योम set_vsindex_dropped ()      { vsindex_dropped = true; }

  bool has_prefix () स्थिर          { वापस has_prefix_; }
  op_code_t prefix_op () स्थिर         { वापस prefix_op_; }
  स्थिर संख्या_t &prefix_अंक_2 () स्थिर { वापस prefix_अंक; }

  bool has_calls () स्थिर          { वापस has_calls_; }

  व्योम compact ()
  {
    अचिन्हित गिनती = values.लंबाई;
    अगर (!गिनती) वापस;
    स्वतः &opstr = values.arrayZ;
    अचिन्हित j = 0;
    क्रम (अचिन्हित i = 1; i < गिनती; i++)
    {
      /* See if we can combine op j and op i. */
      bool combine =
        (opstr[j].op != OpCode_callsubr && opstr[j].op != OpCode_callgsubr) &&
        (opstr[i].op != OpCode_callsubr && opstr[i].op != OpCode_callgsubr) &&
        (opstr[j].is_hinting () == opstr[i].is_hinting ()) &&
        (opstr[j].ptr + opstr[j].लंबाई == opstr[i].ptr) &&
        (opstr[j].लंबाई + opstr[i].लंबाई <= 255);

      अगर (combine)
      {
	opstr[j].लंबाई += opstr[i].लंबाई;
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
  bool    parsed : 1;
  bool    hint_dropped : 1;
  bool    vsindex_dropped : 1;
  bool    has_prefix_ : 1;
  bool    has_calls_ : 1;
  /* Record verbatim byte segments instead of individual tokens,
   * making a separate compact() pass unnecessary; incompatible with
   * hint analysis. */
  bool    coalescing_ : 1;
  /* End offsets of the last two tokens seen (coalescing mode). */
  अचिन्हित penultimate_end_ = 0;
  अचिन्हित last_end_ = 0;
  op_code_t	prefix_op_;
  संख्या_t	prefix_अंक;

  निजी:
  प्रकार parsed_मूल्यावली_t<parsed_cs_op_t> SUPER;
};

काष्ठा parsed_cs_str_vec_t : hb_vector_t<parsed_cs_str_t>
{
  निजी:
  प्रकार hb_vector_t<parsed_cs_str_t> SUPER;
};

काष्ठा cff_subset_accelerator_t
{
  अटल cff_subset_accelerator_t* create (
      hb_blob_t* original_blob,
      स्थिर parsed_cs_str_vec_t& parsed_charstrings,
      स्थिर parsed_cs_str_vec_t& parsed_global_subrs,
      स्थिर hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs) {
    cff_subset_accelerator_t* accel =
        (cff_subset_accelerator_t*) hb_malloc (माप(cff_subset_accelerator_t));
    अगर (unlikely (!accel)) वापस nullptr;
    नया (accel) cff_subset_accelerator_t (original_blob,
                                          parsed_charstrings,
                                          parsed_global_subrs,
                                          parsed_local_subrs);
    वापस accel;
  }

  अटल व्योम destroy (व्योम* value) {
    अगर (!value) वापस;

    cff_subset_accelerator_t* accel = (cff_subset_accelerator_t*) value;
    accel->~cff_subset_accelerator_t ();
    hb_free (accel);
  }

  cff_subset_accelerator_t(
      hb_blob_t* original_blob_,
      स्थिर parsed_cs_str_vec_t& parsed_charstrings_,
      स्थिर parsed_cs_str_vec_t& parsed_global_subrs_,
      स्थिर hb_vector_t<parsed_cs_str_vec_t>& parsed_local_subrs_)
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
    अगर (mapping)
    {
      mapping->~glyph_to_sid_map_t ();
      hb_free (mapping);
    }
  }

  parsed_cs_str_vec_t parsed_charstrings;
  parsed_cs_str_vec_t parsed_global_subrs;
  hb_vector_t<parsed_cs_str_vec_t> parsed_local_subrs;
  mutable hb_atomic_t<glyph_to_sid_map_t *> glyph_to_sid_map;

 निजी:
  hb_blob_t* original_blob;
};

काष्ठा subr_subset_param_t
{
  subr_subset_param_t (parsed_cs_str_t *parsed_charstring_,
		       parsed_cs_str_vec_t *parsed_global_subrs_,
		       parsed_cs_str_vec_t *parsed_local_subrs_,
		       hb_set_t *global_closure_,
		       hb_set_t *local_closure_,
		       bool drop_hints_,
		       bool coalesce_ = false) :
      current_parsed_str (parsed_charstring_),
      parsed_charstring (parsed_charstring_),
      parsed_global_subrs (parsed_global_subrs_),
      parsed_local_subrs (parsed_local_subrs_),
      global_closure (global_closure_),
      local_closure (local_closure_),
      drop_hints (drop_hints_),
      coalesce (coalesce_)
  {
    अगर (coalesce) parsed_charstring->enable_coalescing ();
  }

  parsed_cs_str_t *get_parsed_str_for_context (call_context_t &context)
  {
    चयन (context.type)
    {
      हाल CSType_CharString:
	वापस parsed_charstring;

      हाल CSType_LocalSubr:
	अगर (likely (context.subr_अंक_2 < parsed_local_subrs->लंबाई))
	  वापस &(*parsed_local_subrs)[context.subr_अंक_2];
	अवरोध;

      हाल CSType_GlobalSubr:
	अगर (likely (context.subr_अंक_2 < parsed_global_subrs->लंबाई))
	  वापस &(*parsed_global_subrs)[context.subr_अंक_2];
	अवरोध;
    }
    वापस nullptr;
  }

  ढाँचा <typename ENV>
  व्योम set_current_str (ENV &env, bool calling)
  {
    parsed_cs_str_t *parsed_str = get_parsed_str_for_context (env.context);
    अगर (unlikely (!parsed_str))
    {
      env.set_error ();
      वापस;
    }
    /* If the called subroutine is parsed partially but not completely yet,
     * it must be because we are calling it recursively.
     * Handle it as an error. */
    अगर (unlikely (calling && !parsed_str->is_parsed () && (parsed_str->values.लंबाई > 0)))
      env.set_error ();
    अन्यथा
    {
      अगर (!parsed_str->is_parsed ())
      {
        parsed_str->alloc (env.str_ref.योग_आकार ());
        अगर (coalesce) parsed_str->enable_coalescing ();
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
  bool	  drop_hints;
  bool	  coalesce;
};

काष्ठा subr_remap_t : hb_inc_bimap_t
{
  व्योम create (स्थिर hb_set_t *closure)
  {
    /* create a remapping of subroutine numbers from old to new.
     * no optimization based on usage counts. fonttools doesn't appear doing that either.
     */

    alloc (closure->get_population ());
    क्रम (स्वतः old_अंक : *closure)
      add (old_अंक);

    अगर (get_population () < 1240)
      bias = 107;
    अन्यथा अगर (get_population () < 33900)
      bias = 1131;
    अन्यथा
      bias = 32768;
  }

  पूर्णांक biased_अंक (अचिन्हित पूर्णांक old_अंक) स्थिर
  {
    hb_codepoint_t new_अंक = लो (old_अंक);
    वापस (पूर्णांक)new_अंक - bias;
  }

  रक्षित:
  पूर्णांक bias;
};

काष्ठा subr_remaps_t
{
  subr_remaps_t (अचिन्हित पूर्णांक fd_गिनती)
  {
    local_remaps.resize (fd_गिनती);
  }

  bool in_error()
  {
    वापस local_remaps.in_error ();
  }

  व्योम create (subr_closures_t& closures)
  {
    global_remap.create (&closures.global_closure);
    क्रम (अचिन्हित पूर्णांक i = 0; i < local_remaps.लंबाई; i++)
      local_remaps.arrayZ[i].create (&closures.local_closures[i]);
  }

  subr_remap_t	       global_remap;
  hb_vector_t<subr_remap_t>  local_remaps;
};

ढाँचा <typename SUBSETTER, typename SUBRS, typename ACC, typename ENV, typename OPSET, op_code_t endchar_op=OpCode_Invalid>
काष्ठा subr_subsetter_t
{
  subr_subsetter_t (ACC &acc_, स्थिर hb_subset_plan_t *plan_)
      : acc (acc_), plan (plan_), closures(acc_.fd_गिनती),
        remaps(acc_.fd_गिनती)
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
  bool subset (व्योम)
  {
    अचिन्हित fd_गिनती_2 = acc.fd_गिनती;
    स्थिर cff_subset_accelerator_t* cff_accelerator = nullptr;
    अगर (acc.cff_accelerator) {
      cff_accelerator = acc.cff_accelerator;
      fd_गिनती_2 = cff_accelerator->parsed_local_subrs.लंबाई;
    }

    अगर (cff_accelerator) {
      // If we are not dropping hinting then charstrings are not modified so we can
      // just use a reference to the cached copies.
      cached_charstrings.resize_exact (plan->अंक_output_glyphs ());
      parsed_global_subrs = &cff_accelerator->parsed_global_subrs;
      parsed_local_subrs = &cff_accelerator->parsed_local_subrs;
    } अन्यथा {
      parsed_charstrings.resize_exact (plan->अंक_output_glyphs ());
      parsed_global_subrs_storage.resize_exact (acc.globalSubrs->गिनती);

      अगर (unlikely (!parsed_local_subrs_storage.resize (fd_गिनती_2))) वापस false;

      क्रम (अचिन्हित पूर्णांक i = 0; i < acc.fd_गिनती; i++)
      {
        अचिन्हित गिनती = acc.privateDicts[i].localSubrs->गिनती;
        parsed_local_subrs_storage[i].resize (गिनती);
        अगर (unlikely (parsed_local_subrs_storage[i].in_error ())) वापस false;
      }

      parsed_global_subrs = &parsed_global_subrs_storage;
      parsed_local_subrs = &parsed_local_subrs_storage;
    }

    अगर (unlikely (remaps.in_error()
                  || cached_charstrings.in_error ()
                  || parsed_charstrings.in_error ()
                  || parsed_global_subrs->in_error ()
                  || closures.in_error ())) {
      वापस false;
    }

    /* When hints are not analyzed (not dropping hints, not populating
     * the accelerator), coalesce parsed tokens as they are added,
     * instead of a separate compact() pass. */
    bool coalesce = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
		    !plan->inprogress_accelerator;

    /* phase 1 & 2 */
    क्रम (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;

      स्थिर hb_ubytes_t str = (*acc.charStrings)[old_glyph];
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, old_glyph);
      अगर (unlikely (fd >= acc.fd_गिनती))
        वापस false;

      अगर (cff_accelerator)
      {
        // parsed string already exists in accelerator, copy it and move
        // on.
        अगर (cached_charstrings)
          cached_charstrings[new_glyph] = &cff_accelerator->parsed_charstrings[old_glyph];
        अन्यथा
          parsed_charstrings[new_glyph] = cff_accelerator->parsed_charstrings[old_glyph];

        जारी;
      }

      ENV env (str, acc, fd);
      cs_interpreter_t<ENV, OPSET, subr_subset_param_t> interp (env);

      parsed_charstrings[new_glyph].alloc (str.लंबाई);
      subr_subset_param_t  param (&parsed_charstrings[new_glyph],
                                  &parsed_global_subrs_storage,
                                  &parsed_local_subrs_storage[fd],
                                  &closures.global_closure,
                                  &closures.local_closures[fd],
                                  plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
                                  coalesce);

      अगर (unlikely (!interp.interpret (param)))
        वापस false;

      /* complete parsed string esp. copy CFF1 width or CFF2 vsindex to the parsed charstring for encoding */
      SUBSETTER::complete_parsed_str (interp.env, param, parsed_charstrings[new_glyph]);

      /* mark hint ops and arguments for drop */
      अगर ((plan->flags & HB_SUBSET_FLAGS_NO_HINTING) || plan->inprogress_accelerator)
      {
	subr_subset_param_t  param (&parsed_charstrings[new_glyph],
				    &parsed_global_subrs_storage,
				    &parsed_local_subrs_storage[fd],
				    &closures.global_closure,
				    &closures.local_closures[fd],
				    plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

	drop_hints_param_t  drop;
	अगर (drop_hints_in_str (parsed_charstrings[new_glyph], param, drop))
	{
	  parsed_charstrings[new_glyph].set_hint_dropped ();
	  अगर (drop.vsindex_dropped)
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
      अगर (!coalesce)
	parsed_charstrings[new_glyph].compact ();
    }

    /* Since parsed strings were loaded from accelerator, we still need
     * to compute the subroutine closures which would have normally happened during
     * parsing.
     *
     * Or if we are dropping hinting, redo closure to get actually used subrs.
     */
    अगर ((cff_accelerator ||
	(!cff_accelerator && plan->flags & HB_SUBSET_FLAGS_NO_HINTING)) &&
        !closure_subroutines(*parsed_global_subrs,
                             *parsed_local_subrs))
      वापस false;

    remaps.create (closures);

    populate_subset_accelerator ();
    वापस true;
  }

  bool encode_charstrings (str_buff_vec_t &buffArray, bool encode_prefix = true) स्थिर
  {
    अचिन्हित अंक_glyphs = plan->अंक_output_glyphs ();
    अगर (unlikely (!buffArray.resize_exact (अंक_glyphs)))
      वापस false;
    hb_codepoint_t last = 0;
    क्रम (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t gid = _.first;
      hb_codepoint_t old_glyph = _.second;

      अगर (endchar_op != OpCode_Invalid)
        क्रम (; last < gid; last++)
	{
	  // Hack to point vector to static string.
	  स्वतः &b = buffArray.arrayZ[last];
	  b.set_storage (const_cast<अचिन्हित अक्षर *>(endchar_str), 1);
	}

      last++; // Skip over gid
      अचिन्हित पूर्णांक  fd = subset_get_fd (acc, old_glyph);
      अगर (unlikely (fd >= acc.fd_गिनती))
	वापस false;
      अगर (unlikely (!encode_str (get_parsed_charstring (gid), fd, buffArray.arrayZ[gid], encode_prefix)))
	वापस false;
    }
    अगर (endchar_op != OpCode_Invalid)
      क्रम (; last < अंक_glyphs; last++)
      {
	// Hack to point vector to static string.
	स्वतः &b = buffArray.arrayZ[last];
	b.set_storage (const_cast<अचिन्हित अक्षर *>(endchar_str), 1);
      }

    वापस true;
  }

  bool encode_subrs (स्थिर parsed_cs_str_vec_t &subrs, स्थिर subr_remap_t& remap, अचिन्हित पूर्णांक fd, str_buff_vec_t &buffArray) स्थिर
  {
    अचिन्हित पूर्णांक  गिनती = remap.get_population ();

    अगर (unlikely (!buffArray.resize_exact (गिनती)))
      वापस false;
    क्रम (अचिन्हित पूर्णांक new_अंक = 0; new_अंक < गिनती; new_अंक++)
    {
      hb_codepoint_t old_अंक = remap.backward (new_अंक);
      निश्चित (old_अंक != CFF_UNDEF_CODE);

      अगर (unlikely (!encode_str (subrs[old_अंक], fd, buffArray[new_अंक])))
	वापस false;
    }
    वापस true;
  }

  bool encode_globalsubrs (str_buff_vec_t &buffArray)
  {
    वापस encode_subrs (*parsed_global_subrs, remaps.global_remap, 0, buffArray);
  }

  bool encode_localsubrs (अचिन्हित पूर्णांक fd, str_buff_vec_t &buffArray) स्थिर
  {
    वापस encode_subrs ((*parsed_local_subrs)[fd], remaps.local_remaps[fd], fd, buffArray);
  }

  रक्षित:
  काष्ठा drop_hints_param_t
  {
    drop_hints_param_t ()
      : seen_moveto (false),
	ends_in_hint (false),
	all_dropped (false),
	vsindex_dropped (false) {}

    bool  seen_moveto;
    bool  ends_in_hint;
    bool  all_dropped;
    bool  vsindex_dropped;
  };

  bool drop_hints_in_subr (parsed_cs_str_t &str, अचिन्हित पूर्णांक pos,
			   parsed_cs_str_vec_t &subrs, अचिन्हित पूर्णांक subr_अंक_2,
			   स्थिर subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    drop.ends_in_hint = false;
    bool has_hint = drop_hints_in_str (subrs[subr_अंक_2], param, drop);

    /* if this subr ends with a stem hint (i.e., not a number; potential argument for moveto),
     * then this entire subroutine must be a hint. drop its call. */
    अगर (drop.ends_in_hint)
    {
      str.values[pos].set_hinting ();
      /* if this subr call is at the end of the parent subr, propagate the flag
       * otherwise reset the flag */
      अगर (!str.at_end (pos))
	drop.ends_in_hint = false;
    }
    अन्यथा अगर (drop.all_dropped)
    {
      str.values[pos].set_hinting ();
    }

    वापस has_hint;
  }

  /* returns true if it sees a hint op before the first moveto */
  bool drop_hints_in_str (parsed_cs_str_t &str, स्थिर subr_subset_param_t &param, drop_hints_param_t &drop)
  {
    bool  seen_hint = false;

    अचिन्हित गिनती = str.values.लंबाई;
    स्वतः *values = str.values.arrayZ;
    क्रम (अचिन्हित पूर्णांक pos = 0; pos < गिनती; pos++)
    {
      bool  has_hint = false;
      चयन (values[pos].op)
      {
	हाल OpCode_callsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_local_subrs, values[pos].subr_अंक_2,
					param, drop);
	  अवरोध;

	हाल OpCode_callgsubr:
	  has_hint = drop_hints_in_subr (str, pos,
					*param.parsed_global_subrs, values[pos].subr_अंक_2,
					param, drop);
	  अवरोध;

	हाल OpCode_rmoveto:
	हाल OpCode_hmoveto:
	हाल OpCode_vmoveto:
	  drop.seen_moveto = true;
	  अवरोध;

	हाल OpCode_hintmask:
	हाल OpCode_cntrmask:
	  अगर (drop.seen_moveto)
	  {
	    values[pos].set_hinting ();
	    अवरोध;
	  }
	  HB_FALLTHROUGH;

	हाल OpCode_hstemhm:
	हाल OpCode_vstemhm:
	हाल OpCode_hstem:
	हाल OpCode_vstem:
	  has_hint = true;
	  values[pos].set_hinting ();
	  अगर (str.at_end (pos))
	    drop.ends_in_hint = true;
	  अवरोध;

	हाल OpCode_dotsection:
	  values[pos].set_hinting ();
	  अवरोध;

	शेष:
	  /* NONE */
	  अवरोध;
      }
      अगर (has_hint)
      {
	क्रम (पूर्णांक i = pos - 1; i >= 0; i--)
	{
	  parsed_cs_op_t  &csop = values[(अचिन्हित)i];
	  अगर (csop.is_hinting ())
	    अवरोध;
	  csop.set_hinting ();
	  अगर (csop.op == OpCode_vsindexcs)
	    drop.vsindex_dropped = true;
	}
	seen_hint |= has_hint;
      }
    }

    /* Raise all_dropped flag if all operators except return are dropped from a subr.
     * It may happen even after seeing the first moveto if a subr contains
     * only (usually one) hintmask operator, then calls to this subr can be dropped.
     */
    drop.all_dropped = true;
    क्रम (अचिन्हित पूर्णांक pos = 0; pos < गिनती; pos++)
    {
      parsed_cs_op_t  &csop = values[pos];
      अगर (csop.op == OpCode_return)
	अवरोध;
      अगर (!csop.is_hinting ())
      {
	drop.all_dropped = false;
	अवरोध;
      }
    }

    वापस seen_hint;
  }

  bool closure_subroutines (स्थिर parsed_cs_str_vec_t& global_subrs,
                            स्थिर hb_vector_t<parsed_cs_str_vec_t>& local_subrs)
  {
    closures.reset ();
    क्रम (स्वतः _ : plan->new_to_old_gid_list)
    {
      hb_codepoint_t new_glyph = _.first;
      hb_codepoint_t old_glyph = _.second;
      अचिन्हित पूर्णांक fd = subset_get_fd (acc, old_glyph);
      अगर (unlikely (fd >= acc.fd_गिनती))
        वापस false;

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

    वापस true;
  }

  व्योम collect_subr_refs_in_subr (अचिन्हित पूर्णांक subr_अंक_2, parsed_cs_str_vec_t &subrs,
				  hb_set_t *closure,
				  स्थिर subr_subset_param_t &param)
  {
    अगर (closure->has (subr_अंक_2))
      वापस;
    closure->add (subr_अंक_2);
    collect_subr_refs_in_str (subrs[subr_अंक_2], param);
  }

  व्योम collect_subr_refs_in_str (स्थिर parsed_cs_str_t &str,
                                 स्थिर subr_subset_param_t &param)
  {
    अगर (!str.has_calls ())
      वापस;

    क्रम (स्वतः &opstr : str.values)
    {
      अगर (!param.drop_hints || !opstr.is_hinting ())
      {
	चयन (opstr.op)
	{
	  हाल OpCode_callsubr:
	    collect_subr_refs_in_subr (opstr.subr_अंक_2, *param.parsed_local_subrs,
				       param.local_closure, param);
	    अवरोध;

	  हाल OpCode_callgsubr:
	    collect_subr_refs_in_subr (opstr.subr_अंक_2, *param.parsed_global_subrs,
				       param.global_closure, param);
	    अवरोध;

	  शेष: अवरोध;
	}
      }
    }
  }

  bool encode_str (स्थिर parsed_cs_str_t &str, स्थिर अचिन्हित पूर्णांक fd, str_buff_t &buff, bool encode_prefix = true) स्थिर
  {
    str_encoder_t  encoder (buff);
    encoder.reset ();
    bool hinting = !(plan->flags & HB_SUBSET_FLAGS_NO_HINTING);
    /* if a prefix (CFF1 width or CFF2 vsindex) has been removed along with hints,
     * re-insert it at the beginning of charstreing */
    अगर (encode_prefix && str.has_prefix () && !hinting && str.is_hint_dropped ())
    {
      encoder.encode_अंक_cs (str.prefix_अंक_2 ());
      अगर (str.prefix_op () != OpCode_Invalid)
	encoder.encode_op (str.prefix_op ());
    }

    अचिन्हित आकार = 0;
    क्रम (स्वतः &opstr : str.values)
    {
      आकार += opstr.लंबाई;
      अगर (opstr.op == OpCode_callsubr || opstr.op == OpCode_callgsubr)
        आकार += 3;
    }
    अगर (!buff.alloc_exact (buff.लंबाई + आकार))
      वापस false;

    क्रम (स्वतः &opstr : str.values)
    {
      अगर (hinting || !opstr.is_hinting ())
      {
	चयन (opstr.op)
	{
	  हाल OpCode_callsubr:
	    encoder.encode_int (remaps.local_remaps[fd].biased_अंक (opstr.subr_अंक_2));
	    encoder.copy_str (opstr.ptr, opstr.लंबाई);
	    अवरोध;

	  हाल OpCode_callgsubr:
	    encoder.encode_int (remaps.global_remap.biased_अंक (opstr.subr_अंक_2));
	    encoder.copy_str (opstr.ptr, opstr.लंबाई);
	    अवरोध;

	  शेष:
	    encoder.copy_str (opstr.ptr, opstr.लंबाई);
	    अवरोध;
	}
      }
    }
    वापस !encoder.in_error ();
  }

  व्योम compact_parsed_subrs () स्थिर
  {
    क्रम (स्वतः &cs : parsed_global_subrs_storage)
      cs.compact ();
    क्रम (स्वतः &vec : parsed_local_subrs_storage)
      क्रम (स्वतः &cs : vec)
	cs.compact ();
  }

  व्योम populate_subset_accelerator () स्थिर
  {
    अगर (!plan->inprogress_accelerator) वापस;

    compact_parsed_subrs ();

    acc.cff_accelerator =
        cff_subset_accelerator_t::create(acc.blob,
                                         parsed_charstrings,
                                         parsed_global_subrs_storage,
                                         parsed_local_subrs_storage);
  }

  स्थिर parsed_cs_str_t& get_parsed_charstring (अचिन्हित i) स्थिर
  {
    अगर (cached_charstrings) वापस *(cached_charstrings[i]);
    वापस parsed_charstrings[i];
  }

  रक्षित:
  स्थिर ACC			&acc;
  स्थिर hb_subset_plan_t	*plan;

  subr_closures_t		closures;

  hb_vector_t<स्थिर parsed_cs_str_t*>     cached_charstrings;
  स्थिर parsed_cs_str_vec_t*              parsed_global_subrs;
  स्थिर hb_vector_t<parsed_cs_str_vec_t>* parsed_local_subrs;

  subr_remaps_t			remaps;

  निजी:

  parsed_cs_str_vec_t		parsed_charstrings;
  parsed_cs_str_vec_t		parsed_global_subrs_storage;
  hb_vector_t<parsed_cs_str_vec_t>  parsed_local_subrs_storage;
  प्रकार typename SUBRS::गिनती_type subr_गिनती_type;
};

} /* namespace CFF */

HB_INTERNAL bool
hb_plan_subset_cff_fdselect (स्थिर hb_subset_plan_t *plan,
			    अचिन्हित पूर्णांक fd_गिनती,
			    स्थिर CFF::FDSelect &src, /* IN */
			    अचिन्हित पूर्णांक src_अंक_glyphs,
			    अचिन्हित पूर्णांक &subset_fd_गिनती /* OUT */,
			    अचिन्हित पूर्णांक &subset_fdselect_आकार /* OUT */,
			    अचिन्हित पूर्णांक &subset_fdselect_format /* OUT */,
			    hb_vector_t<CFF::code_pair_t> &fdselect_ranges /* OUT */,
			    hb_inc_bimap_t &fdmap /* OUT */);

HB_INTERNAL bool
hb_serialize_cff_fdselect (hb_serialize_context_t *c,
			  अचिन्हित पूर्णांक अंक_glyphs,
			  स्थिर CFF::FDSelect &src,
			  अचिन्हित पूर्णांक fd_गिनती_2,
			  अचिन्हित पूर्णांक fdselect_format,
			  अचिन्हित पूर्णांक आकार,
			  स्थिर hb_vector_t<CFF::code_pair_t> &fdselect_ranges);

#पूर्ण_अगर /* HB_SUBSET_CFF_COMMON_HH */
