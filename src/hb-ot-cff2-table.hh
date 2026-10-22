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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF2_ਸਾਰਣੀ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CFF2_ਸਾਰਣੀ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

ਨਾਮਾਕਾਸ਼ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_CFF2 HB_ਟੈਗ('C','F','F','2')

ਕਿਸਮਪਰਿਭਾਸ਼ਾ CFF2_ਸੂਚਕਾਂਕ         CFF2_Char_ਲੜੀਆਂ;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Subrs<HBUINT32>   CFF2Subrs;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ਬਣਤਰ CFF2FDSelect
{
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ CFF2FDSelect &src, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = src.get_ਮਾਪ (ਅੰਕ_glyphs);
    CFF2FDSelect *dest = c->allocate_ਮਾਪ<CFF2FDSelect> (ਮਾਪ);
    ਜੇ (unlikely (!dest)) return_trace (ਝੂਠ);
    hb_memcpy (dest, &src, ਮਾਪ);
    return_trace (ਸੱਚ);
  }

  size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs) ਅਚਲ
  {
    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ format.static_ਮਾਪ + u.format0.get_ਮਾਪ (ਅੰਕ_glyphs);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ format.static_ਮਾਪ + u.format3.get_ਮਾਪ ();
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ format.static_ਮਾਪ + u.format4.get_ਮਾਪ ();
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ਅਚਲ
  {
    ਜੇ (ਇਹ == &Null (CFF2FDSelect))
      ਵਾਪਸ 0;

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); ਵਾਪਸ u.format0.get_fd (glyph);
    ਹਾਲਤ 3: hb_barrier (); ਵਾਪਸ u.format3.get_fd (glyph);
    ਹਾਲਤ 4: hb_barrier (); ਵਾਪਸ u.format4.get_fd (glyph);
    ਮੂਲ:ਵਾਪਸ 0;
    }
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fdcount,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ charstring_ਗਿਣਤੀ = HB_CODEPOINT_INVALID) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਜੇ (unlikely (!c->check_struct (ਇਹ)))
      return_trace (ਝੂਠ);
    hb_barrier ();

    ਚੋਣ (format)
    {
    ਹਾਲਤ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_ਗਿਣਤੀ));
    ਹਾਲਤ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_ਗਿਣਤੀ));
    ਹਾਲਤ 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_ਗਿਣਤੀ));
    ਮੂਲ:return_trace (ਝੂਠ);
    }
  }

  HBUINT8	format;
  ਸੰਘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (2);
};

ਬਣਤਰ CFF2_ਵਸਤੂ_Variation_Store
{
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  c->check_range (&varStore, ਮਾਪ) &&
		  varStore.sanitize (c));
  }

  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ਅਚਲ CFF2_ਵਸਤੂ_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ_3 = varStore->get_ਮਾਪ ();
    CFF2_ਵਸਤੂ_Variation_Store *dest = c->allocate_ਮਾਪ<CFF2_ਵਸਤੂ_Variation_Store> (ਮਾਪ_3);
    ਜੇ (unlikely (!dest)) return_trace (ਝੂਠ);
    hb_memcpy (dest, varStore, ਮਾਪ_3);
    return_trace (ਸੱਚ);
  }

  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ HBUINT16::static_ਮਾਪ + ਮਾਪ; }

  HBUINT16	ਮਾਪ;
  ਵਸਤੂ_Variation_Store  varStore;

  DEFINE_ਮਾਪ_MIN (2 + ਵਸਤੂ_Variation_Store::min_ਮਾਪ);
};

ਬਣਤਰ cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t : ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<>
{
  ਖਾਲੀ init ()
  {
    ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<>::init ();
    vstore_ਵਿੱਥ = 0;
    FDSelect_ਵਿੱਥ = 0;
  }
  ਖਾਲੀ fini () { ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t<>::fini (); }

  ਪੂਰਨ  vstore_ਵਿੱਥ;
  ਪੂਰਨ  FDSelect_ਵਿੱਥ;
};

ਬਣਤਰ cff2_ਸਿਖਰ_dict_opset_t : ਸਿਖਰ_dict_opset_t<>
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ਅੰਕ_interp_env_t& env, cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Font_ਮੈਟ੍ਰਿਕਸ:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.ਜੋੜੋ_op (op, env.str_ref);
	  env.clear_ਦਲੀਲਾਂ ();
	}
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_vstore:
	dictval.vstore_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_FDSelect:
	dictval.FDSelect_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref);
  }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਸਿਖਰ_dict_opset_t<> SUPER;
};

ਬਣਤਰ cff2_font_dict_ਮੁੱਲਾਂ_t : dict_ਮੁੱਲਾਂ_t<op_str_t>
{
  ਖਾਲੀ init ()
  {
    dict_ਮੁੱਲਾਂ_t<op_str_t>::init ();
    ਨਿੱਜੀ_Dict_Info.init ();
  }
  ਖਾਲੀ fini () { dict_ਮੁੱਲਾਂ_t<op_str_t>::fini (); }

  ਸਾਰਣੀ_info_t    ਨਿੱਜੀ_Dict_Info;
};

ਬਣਤਰ cff2_font_dict_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, ਅੰਕ_interp_env_t& env, cff2_font_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_ਨਿੱਜੀ:
	dictval.ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	dictval.ਨਿੱਜੀ_Dict_Info.ਮਾਪ = env.arg_ਢੇਰ.ਕੱਢੋ_uint ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ())
	  ਵਾਪਸ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref);
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_opset_t SUPER;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ VAL>
ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t : dict_ਮੁੱਲਾਂ_t<VAL>
{
  ਖਾਲੀ init ()
  {
    dict_ਮੁੱਲਾਂ_t<VAL>::init ();
    subrs_ਵਿੱਥ = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  ਖਾਲੀ fini () { dict_ਮੁੱਲਾਂ_t<VAL>::fini (); }

  ਪੂਰਨ                subrs_ਵਿੱਥ;
  ਅਚਲ CFF2Subrs   *localSubrs;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ivs;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t<op_str_t> cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_ਅਧਾਰ_t<ਅੰਕ_dict_val_t> cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t;

ਬਣਤਰ cff2_priv_dict_interp_env_t : ਅੰਕ_interp_env_t
{
  cff2_priv_dict_interp_env_t (ਅਚਲ hb_ubytes_t &str) :
    ਅੰਕ_interp_env_t (str) {}

  ਖਾਲੀ process_vsindex ()
  {
    ਜੇ (likely (!ਵੇਖਿਆ_vsindex))
    {
      ਸੈੱਟ_ivs (arg_ਢੇਰ.ਕੱਢੋ_uint ());
    }
    ਵੇਖਿਆ_vsindex = ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ivs () ਅਚਲ { ਵਾਪਸ ivs; }
  ਖਾਲੀ	 ਸੈੱਟ_ivs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ivs_) { ivs = ivs_; }

  ਸੁਰੱਖਿਅਤ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ivs = 0;
  ਤਾਰਕਿਕ	  ਵੇਖਿਆ_vsindex = ਝੂਠ;
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_opset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_priv_dict_interp_env_t& env, cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t& dictval)
  {
    ਅੰਕ_dict_val_t val;
    val.init ();

    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_Subrs:
	dictval.subrs_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;
      ਹਾਲਤ Op_ਕੋਡ_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	dict_opset_t::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref, val);
  }
};

ਬਣਤਰ cff2_ਨਿੱਜੀ_dict_opset_subset_t : dict_opset_t
{
  ਸਥਿਰ ਖਾਲੀ process_op (op_ਕੋਡ_t op, cff2_priv_dict_interp_env_t& env, cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t& dictval)
  {
    ਚੋਣ (op) {
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਮੁੱਲਾਂ:
      ਹਾਲਤ Op_ਕੋਡ_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Family_Other_Blues:
      ਹਾਲਤ Op_ਕੋਡ_Std_HW:
      ਹਾਲਤ Op_ਕੋਡ_Std_VW:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Scale:
      ਹਾਲਤ Op_ਕੋਡ_Blue_ਖਿਸਕਾਓ:
      ਹਾਲਤ Op_ਕੋਡ_Blue_Fuzz:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_H:
      ਹਾਲਤ Op_ਕੋਡ_Stem_Snap_V:
      ਹਾਲਤ Op_ਕੋਡ_Language_ਸਮੂਹ:
      ਹਾਲਤ Op_ਕੋਡ_Expansion_ਗੁਣਕ:
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_blenddict:
	env.clear_ਦਲੀਲਾਂ ();
	ਵਾਪਸ;

      ਹਾਲਤ Op_ਕੋਡ_Subrs:
	dictval.subrs_ਵਿੱਥ = env.arg_ਢੇਰ.ਕੱਢੋ_int ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਹਾਲਤ Op_ਕੋਡ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_ਦਲੀਲਾਂ ();
	ਰੋਕੋ;

      ਮੂਲ:
	SUPER::process_op (op, env);
	ਜੇ (!env.arg_ਢੇਰ.is_empty ()) ਵਾਪਸ;
	ਰੋਕੋ;
    }

    ਜੇ (unlikely (env.ਵਿੱਚ_error ())) ਵਾਪਸ;

    dictval.ਜੋੜੋ_op (op, env.str_ref);
  }

  ਨਿਜੀ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_opset_t SUPER;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_interpreter_t<cff2_ਸਿਖਰ_dict_opset_t, cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t> cff2_ਸਿਖਰ_dict_interpreter_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_ਮੁੱਲਾਂ_t> cff2_font_dict_interpreter_t;

ਬਣਤਰ CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ITER, ਕਿਸਮ_ਨਾਂ OP_SERIALIZER>
  ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ਵਾਪਸ FDArray<HBUINT32>::serialize<cff2_font_dict_ਮੁੱਲਾਂ_t, ਸਾਰਣੀ_info_t> (c, it, opszr); }
};

} /* namespace CFF */

ਨਾਮਾਕਾਸ਼ OT {

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਬਣਤਰ cff2
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_CFF2;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  hb_barrier () &&
		  likely (ਸੰਸਕਰਣ.major == 2));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ PRIVOPSET, ਕਿਸਮ_ਨਾਂ PRIVDICTVAL>
  ਬਣਤਰ accelerator_templ_t
  {
    ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = cff2::ਸਾਰਣੀ_ਟੈਗ;

    accelerator_templ_t (hb_face_t *face)
    {
      ਜੇ (!face) ਵਾਪਸ;

      ਸਿਖਰ_Dict_2.init ();
      fontDicts.init ();
      ਨਿੱਜੀ_Dicts.init ();

      ਇਹ->blob = sc.reference_ਸਾਰਣੀ<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (ਇਹ->blob);
      sc.ਸ਼ੁਰੂ_processing ();

      ਅਚਲ OT::cff2 *cff2 = ਇਹ->blob->ਸਾਂਚਾ as<OT::cff2> ();

      ਜੇ (cff2 == &Null (OT::cff2))
        ਜਾਓ fail;

      { /* parse top dict */
	hb_ubytes_t ਸਿਖਰ_Dict_Str = (cff2 + cff2->ਸਿਖਰ_Dict_2).as_ubytes (cff2->ਸਿਖਰ_Dict_ਮਾਪ);
	ਜੇ (unlikely (!ਸਿਖਰ_Dict_Str.sanitize (&sc))) ਜਾਓ fail;
	hb_barrier ();
	ਅੰਕ_interp_env_t env (ਸਿਖਰ_Dict_Str);
	cff2_ਸਿਖਰ_dict_interpreter_t ਸਿਖਰ_interp (env);
	ਸਿਖਰ_Dict_2.init ();
	ਜੇ (unlikely (!ਸਿਖਰ_interp.interpret (ਸਿਖਰ_Dict_2))) ਜਾਓ fail;
      }

      globalSubrs = &Struct_At_ਵਿੱਥ_Or_Null<CFF2Subrs> (cff2, cff2->ਸਿਖਰ_Dict_2 + cff2->ਸਿਖਰ_Dict_ਮਾਪ, sc);
      varStore = &Struct_At_ਵਿੱਥ_Or_Null<CFF2_ਵਸਤੂ_Variation_Store> (cff2, ਸਿਖਰ_Dict_2.vstore_ਵਿੱਥ, sc);
      char_ਲੜੀਆਂ = &Struct_At_ਵਿੱਥ_Or_Null<CFF2_Char_ਲੜੀਆਂ> (cff2, ਸਿਖਰ_Dict_2.char_ਲੜੀਆਂ_ਵਿੱਥ, sc);
      fdArray = &Struct_At_ਵਿੱਥ_Or_Null<CFF2FDArray> (cff2, ਸਿਖਰ_Dict_2.FDArray_ਵਿੱਥ, sc);

      ਜੇ (char_ਲੜੀਆਂ == &Null (CFF2_Char_ਲੜੀਆਂ) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        ਜਾਓ fail;

      ਅੰਕ_glyphs = char_ਲੜੀਆਂ->ਗਿਣਤੀ_2;
      ਜੇ (ਅੰਕ_glyphs != sc.get_ਅੰਕ_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_ਸਾਰਣੀ (HB_ਟੈਗ ('V','A','R','C'));
        ਤਾਰਕਿਕ valid_ਗਿਣਤੀ = ਅੰਕ_glyphs < sc.get_ਅੰਕ_glyphs () &&
                           hb_blob_get_ਲੰਮਾਈ (varc);
        hb_blob_destroy (varc);
        ਜੇ (!valid_ਗਿਣਤੀ) ਜਾਓ fail;
      }

      fdSelect = &Struct_At_ਵਿੱਥ_Or_Null<CFF2FDSelect> (cff2, ਸਿਖਰ_Dict_2.FDSelect_ਵਿੱਥ, sc,
                                                   fdArray->ਗਿਣਤੀ_2, ਅੰਕ_glyphs);
      ਜੇ (ਸਿਖਰ_Dict_2.FDSelect_ਵਿੱਥ && fdSelect == &Null (CFF2FDSelect))
        ਜਾਓ fail;

      fd_ਗਿਣਤੀ = fdArray->ਗਿਣਤੀ_2;
      ਜੇ (!ਨਿੱਜੀ_Dicts.resize (fd_ਗਿਣਤੀ))
        ਜਾਓ fail;

      /* parse font dicts and gather private dicts */
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < fd_ਗਿਣਤੀ; i++)
      {
	ਅਚਲ hb_ubytes_t fontDictStr = (*fdArray)[i];
	ਜੇ (unlikely (!fontDictStr.sanitize (&sc))) ਜਾਓ fail;
	hb_barrier ();
	cff2_font_dict_ਮੁੱਲਾਂ_t  *font;
	ਅੰਕ_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	ਜੇ (unlikely (font == &Crap (cff2_font_dict_ਮੁੱਲਾਂ_t))) ਜਾਓ fail;
	font->init ();
	ਜੇ (unlikely (!font_interp.interpret (*font))) ਜਾਓ fail;

	ਅਚਲ hb_ubytes_t privDictStr = Struct_At_ਵਿੱਥ_Or_Null<UnsizedByteStr> (cff2, font->ਨਿੱਜੀ_Dict_Info.ਵਿੱਥ, sc, font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ).as_ubytes (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ);
	ਜੇ (unlikely (font->ਨਿੱਜੀ_Dict_Info.ਮਾਪ &&
		      privDictStr == (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &Null (UnsizedByteStr))) ਜਾਓ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	ਨਿੱਜੀ_Dicts[i].init ();
	ਜੇ (unlikely (!priv_interp.interpret (ਨਿੱਜੀ_Dicts[i]))) ਜਾਓ fail;

	ਨਿੱਜੀ_Dicts[i].localSubrs = &Struct_At_ਵਿੱਥ_Or_Null<CFF2Subrs> (&privDictStr[0], ਨਿੱਜੀ_Dicts[i].subrs_ਵਿੱਥ, sc);
      }

      ਵਾਪਸ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ਖਾਲੀ _fini ()
    {
      sc.ਸਿਰਾ_processing ();
      ਸਿਖਰ_Dict_2.fini ();
      fontDicts.fini ();
      ਨਿੱਜੀ_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = ਖਾਲੀ_ਸੂਚਕ;

      ਸਵੈਚਾਲਿਤ *scalars = cached_scalars_ਸਦਿਸ਼.get_acquire ();
      ਜੇ (scalars && cached_scalars_ਸਦਿਸ਼.cmpexch (scalars, ਖਾਲੀ_ਸੂਚਕ))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_ਸਦਿਸ਼_t<uint16_t> *create_glyph_ਮੰਜ਼ਿਲ_sid_map () ਅਚਲ
    {
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

    hb_blob_t *get_blob () ਅਚਲ { ਵਾਪਸ blob; }

    ਤਾਰਕਿਕ is_valid () ਅਚਲ { ਵਾਪਸ blob; }

    ਸੁਰੱਖਿਅਤ:
    hb_sanitize_context_t	sc;

    ਜਨਤਕ:
    hb_blob_t			*blob = ਖਾਲੀ_ਸੂਚਕ;
    cff2_ਸਿਖਰ_dict_ਮੁੱਲਾਂ_t	ਸਿਖਰ_Dict_2;
    ਅਚਲ CFF2Subrs		*globalSubrs = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF2_ਵਸਤੂ_Variation_Store	*varStore = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF2_Char_ਲੜੀਆਂ	*char_ਲੜੀਆਂ = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF2FDArray		*fdArray = ਖਾਲੀ_ਸੂਚਕ;
    ਅਚਲ CFF2FDSelect		*fdSelect = ਖਾਲੀ_ਸੂਚਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		fd_ਗਿਣਤੀ = 0;

    hb_ਸਦਿਸ਼_t<cff2_font_dict_ਮੁੱਲਾਂ_t>     fontDicts;
    hb_ਸਦਿਸ਼_t<PRIVDICTVAL>  ਨਿੱਜੀ_Dicts;

    ਬਦਲਣਯੋਗ hb_atomic_t<hb_ਸਦਿਸ਼_t<ਦਸ਼ਮਲਵ> *> cached_scalars_ਸਦਿਸ਼;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	      ਅੰਕ_glyphs = 0;
  };

  ਬਣਤਰ accelerator_t : accelerator_templ_t<cff2_ਨਿੱਜੀ_dict_opset_t, cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ਅਚਲ ਪੂਰਨ> coords,
				     int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ get_ਰਸਤਾ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ਅਚਲ ਪੂਰਨ> coords, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ;
  };

  ਬਣਤਰ accelerator_subset_t : accelerator_templ_t<cff2_ਨਿੱਜੀ_dict_opset_subset_t, cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ਜੇ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ;
    HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ serialize (hb_serialize_context_t *c,
				ਬਣਤਰ cff2_subset_plan &plan,
				hb_array_t<ਪੂਰਨ> normalized_coords) ਅਚਲ;

    ਬਦਲਣਯੋਗ CFF::cff_subset_accelerator_t* cff_accelerator = ਖਾਲੀ_ਸੂਚਕ;

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ accelerator_templ_t<cff2_ਨਿੱਜੀ_dict_opset_subset_t, cff2_ਨਿੱਜੀ_dict_ਮੁੱਲਾਂ_subset_t> SUPER;
  };

  ਜਨਤਕ:
  Fixed_ਸੰਸਕਰਣ<HBUINT8>		ਸੰਸਕਰਣ;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_ਮੰਜ਼ਿਲ<ਸਿਖਰ_Dict, HBUINT8>	ਸਿਖਰ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			ਸਿਖਰ_Dict_ਮਾਪ;	/* Top DICT size */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (5);
};

ਬਣਤਰ cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ਬਣਤਰ cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#ਜੇ_ਅੰਤ /* HB_OT_CFF2_TABLE_HH */
