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

#ifndef HB_OT_CFF2_TABLE_HH
#define HB_OT_CFF2_TABLE_HH

#અન્તર્ભાવ "hb-ot-cff-common.hh"
#અન્તર્ભાવ "hb-subset-cff-common.hh"
#અન્તર્ભાવ "hb-draw.hh"
#અન્તર્ભાવ "hb-paint.hh"

નામાકાશ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

પ્રકારવ્યાખ્યા CFF2Index         CFF2CharStrings;
પ્રકારવ્યાખ્યા Subrs<HBUINT32>   CFF2Subrs;

પ્રકારવ્યાખ્યા FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
પ્રકારવ્યાખ્યા FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

સંરચના CFF2FDSelect
{
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ CFF2FDSelect &src, unsigned પૂર્ણાંક અંક_glyphs)
  {
    TRACE_SERIALIZE (આ);
    unsigned પૂર્ણાંક size = src.get_size (અંક_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    જો (unlikely (!dest)) return_trace (ખોટું);
    hb_memcpy (dest, &src, size);
    return_trace (સાચું);
  }

  size_t get_size (unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા format.static_size + u.format0.get_size (અંક_glyphs);
    સ્થિતિ 3: hb_barrier (); પાછા format.static_size + u.format3.get_size ();
    સ્થિતિ 4: hb_barrier (); પાછા format.static_size + u.format4.get_size ();
    મૂળભૂત:પાછા 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) અચલ
  {
    જો (આ == &Null (CFF2FDSelect))
      પાછા 0;

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_fd (glyph);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_fd (glyph);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_fd (glyph);
    મૂળભૂત:પાછા 0;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned પૂર્ણાંક fdcount,
                 unsigned પૂર્ણાંક charstring_ગણતરી = HB_CODEPOINT_INVALID) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ)))
      return_trace (ખોટું);
    hb_barrier ();

    પસંદગી (format)
    {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_ગણતરી));
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_ગણતરી));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_ગણતરી));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  HBUINT8	format;
  સંઘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (2);
};

સંરચના CFF2ItemVariationStore
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  તાર્કિક serialize (hb_serialize_context_t *c, અચલ CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (આ);
    unsigned પૂર્ણાંક size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    જો (unlikely (!dest)) return_trace (ખોટું);
    hb_memcpy (dest, varStore, size_);
    return_trace (સાચું);
  }

  size_t get_size () અચલ { પાછા HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

સંરચના cff2_ટોચ_dict_કિંમતો_t : ટોચ_dict_કિંમતો_t<>
{
  શૂન્ય init ()
  {
    ટોચ_dict_કિંમતો_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  શૂન્ય fini () { ટોચ_dict_કિંમતો_t<>::fini (); }

  પૂર્ણાંક  vstoreOffset;
  પૂર્ણાંક  FDSelectOffset;
};

સંરચના cff2_ટોચ_dict_opset_t : ટોચ_dict_opset_t<>
{
  સ્થિર શૂન્ય process_op (op_code_t op, અંક_interp_env_t& env, cff2_ટોચ_dict_કિંમતો_t& dictval)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	તોડો;

      સ્થિતિ OpCode_vstore:
	dictval.vstoreOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref);
  }

  પ્રકારવ્યાખ્યા ટોચ_dict_opset_t<> SUPER;
};

સંરચના cff2_font_dict_કિંમતો_t : dict_કિંમતો_t<op_str_t>
{
  શૂન્ય init ()
  {
    dict_કિંમતો_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  શૂન્ય fini () { dict_કિંમતો_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

સંરચના cff2_font_dict_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, અંક_interp_env_t& env, cff2_font_dict_કિંમતો_t& dictval)
  {
    પસંદગી (op) {
      સ્થિતિ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ઢગલો.કાઢો_uint ();
	dictval.privateDictInfo.size = env.arg_ઢગલો.કાઢો_uint ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ())
	  પાછા;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref);
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા dict_opset_t SUPER;
};

template <typename VAL>
સંરચના cff2_private_dict_કિંમતો_base_t : dict_કિંમતો_t<VAL>
{
  શૂન્ય init ()
  {
    dict_કિંમતો_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  શૂન્ય fini () { dict_કિંમતો_t<VAL>::fini (); }

  પૂર્ણાંક                subrsOffset;
  અચલ CFF2Subrs   *localSubrs;
  unsigned પૂર્ણાંક      ivs;
};

પ્રકારવ્યાખ્યા cff2_private_dict_કિંમતો_base_t<op_str_t> cff2_private_dict_કિંમતો_subset_t;
પ્રકારવ્યાખ્યા cff2_private_dict_કિંમતો_base_t<અંક_dict_val_t> cff2_private_dict_કિંમતો_t;

સંરચના cff2_priv_dict_interp_env_t : અંક_interp_env_t
{
  cff2_priv_dict_interp_env_t (અચલ hb_ubytes_t &str) :
    અંક_interp_env_t (str) {}

  શૂન્ય process_vsindex ()
  {
    જો (likely (!seen_vsindex))
    {
      set_ivs (arg_ઢગલો.કાઢો_uint ());
    }
    seen_vsindex = સાચું;
  }

  unsigned પૂર્ણાંક get_ivs () અચલ { પાછા ivs; }
  શૂન્ય	 set_ivs (unsigned પૂર્ણાંક ivs_) { ivs = ivs_; }

  સુરક્ષિત:
  unsigned પૂર્ણાંક  ivs = 0;
  તાર્કિક	  seen_vsindex = ખોટું;
};

સંરચના cff2_private_dict_opset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_કિંમતો_t& dictval)
  {
    અંક_dict_val_t val;
    val.init ();

    પસંદગી (op) {
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_ExpansionFactor:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	તોડો;
      સ્થિતિ OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	તોડો;

      મૂળભૂત:
	dict_opset_t::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref, val);
  }
};

સંરચના cff2_private_dict_opset_subset_t : dict_opset_t
{
  સ્થિર શૂન્ય process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_કિંમતો_subset_t& dictval)
  {
    પસંદગી (op) {
      સ્થિતિ Op_Code_Blue_કિંમતો:
      સ્થિતિ OpCode_OtherBlues:
      સ્થિતિ OpCode_FamilyBlues:
      સ્થિતિ OpCode_FamilyOtherBlues:
      સ્થિતિ OpCode_StdHW:
      સ્થિતિ OpCode_StdVW:
      સ્થિતિ OpCode_BlueScale:
      સ્થિતિ OpCode_BlueShift:
      સ્થિતિ OpCode_BlueFuzz:
      સ્થિતિ OpCode_StemSnapH:
      સ્થિતિ OpCode_StemSnapV:
      સ્થિતિ OpCode_LanguageGroup:
      સ્થિતિ OpCode_ExpansionFactor:
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_blenddict:
	env.clear_args ();
	પાછા;

      સ્થિતિ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ઢગલો.કાઢો_int ();
	env.clear_args ();
	તોડો;

      સ્થિતિ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	તોડો;

      મૂળભૂત:
	SUPER::process_op (op, env);
	જો (!env.arg_ઢગલો.is_empty ()) પાછા;
	તોડો;
    }

    જો (unlikely (env.in_error ())) પાછા;

    dictval.add_op (op, env.str_ref);
  }

  ખાનગી:
  પ્રકારવ્યાખ્યા dict_opset_t SUPER;
};

પ્રકારવ્યાખ્યા dict_interpreter_t<cff2_ટોચ_dict_opset_t, cff2_ટોચ_dict_કિંમતો_t> cff2_ટોચ_dict_interpreter_t;
પ્રકારવ્યાખ્યા dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_કિંમતો_t> cff2_font_dict_interpreter_t;

સંરચના CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  તાર્કિક serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { પાછા FDArray<HBUINT32>::serialize<cff2_font_dict_કિંમતો_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

નામાકાશ OT {

using નામાકાશ CFF;

સંરચના cff2
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  સંરચના accelerator_templ_t
  {
    સ્થિર constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      જો (!face) પાછા;

      ટોચ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      આ->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (આ->blob);
      sc.start_processing ();

      અચલ OT::cff2 *cff2 = આ->blob->template as<OT::cff2> ();

      જો (cff2 == &Null (OT::cff2))
        જાઓ fail;

      { /* parse top dict */
	hb_ubytes_t ટોચ_Dict_Str = (cff2 + cff2->ટોચ_Dict_2).as_ubytes (cff2->ટોચ_Dict_Size);
	જો (unlikely (!ટોચ_Dict_Str.sanitize (&sc))) જાઓ fail;
	hb_barrier ();
	અંક_interp_env_t env (ટોચ_Dict_Str);
	cff2_ટોચ_dict_interpreter_t ટોચ_interp (env);
	ટોચ_Dict_2.init ();
	જો (unlikely (!ટોચ_interp.interpret (ટોચ_Dict_2))) જાઓ fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->ટોચ_Dict_2 + cff2->ટોચ_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, ટોચ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, ટોચ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, ટોચ_Dict_2.FDArrayOffset, sc);

      જો (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        જાઓ fail;

      અંક_glyphs = charStrings->ગણતરી;
      જો (અંક_glyphs != sc.get_અંક_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        તાર્કિક valid_ગણતરી = અંક_glyphs < sc.get_અંક_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        જો (!valid_ગણતરી) જાઓ fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, ટોચ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->ગણતરી, અંક_glyphs);
      જો (ટોચ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        જાઓ fail;

      fd_ગણતરી = fdArray->ગણતરી;
      જો (!privateDicts.resize (fd_ગણતરી))
        જાઓ fail;

      /* parse font dicts and gather private dicts */
      માટે (unsigned પૂર્ણાંક i = 0; i < fd_ગણતરી; i++)
      {
	અચલ hb_ubytes_t fontDictStr = (*fdArray)[i];
	જો (unlikely (!fontDictStr.sanitize (&sc))) જાઓ fail;
	hb_barrier ();
	cff2_font_dict_કિંમતો_t  *font;
	અંક_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	જો (unlikely (font == &Crap (cff2_font_dict_કિંમતો_t))) જાઓ fail;
	font->init ();
	જો (unlikely (!font_interp.interpret (*font))) જાઓ fail;

	અચલ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	જો (unlikely (font->privateDictInfo.size &&
		      privDictStr == (અચલ unsigned અક્ષર *) &Null (UnsizedByteStr))) જાઓ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	જો (unlikely (!priv_interp.interpret (privateDicts[i]))) જાઓ fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      પાછા;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    શૂન્ય _fini ()
    {
      sc.end_processing ();
      ટોચ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      જો (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () અચલ
    {
      પાછા nullptr;
    }

    hb_blob_t *get_blob () અચલ { પાછા blob; }

    તાર્કિક is_valid () અચલ { પાછા blob; }

    સુરક્ષિત:
    hb_sanitize_context_t	sc;

    જાહેર:
    hb_blob_t			*blob = nullptr;
    cff2_ટોચ_dict_કિંમતો_t	ટોચ_Dict_2;
    અચલ CFF2Subrs		*globalSubrs = nullptr;
    અચલ CFF2ItemVariationStore	*varStore = nullptr;
    અચલ CFF2CharStrings	*charStrings = nullptr;
    અચલ CFF2FDArray		*fdArray = nullptr;
    અચલ CFF2FDSelect		*fdSelect = nullptr;
    unsigned પૂર્ણાંક		fd_ગણતરી = 0;

    hb_vector_t<cff2_font_dict_કિંમતો_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<દશાંશ> *> cached_scalars_vector;

    unsigned પૂર્ણાંક	      અંક_glyphs = 0;
  };

  સંરચના accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_કિંમતો_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL તાર્કિક get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) અચલ;
    HB_INTERNAL તાર્કિક get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<અચલ પૂર્ણાંક> coords,
				     int64_t *budget = nullptr) અચલ;
    HB_INTERNAL તાર્કિક get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) અચલ;
    HB_INTERNAL તાર્કિક get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<અચલ પૂર્ણાંક> coords, int64_t *budget = nullptr) અચલ;
  };

  સંરચના accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_કિંમતો_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      જો (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL તાર્કિક subset (hb_subset_context_t *c) અચલ;
    HB_INTERNAL તાર્કિક serialize (hb_serialize_context_t *c,
				સંરચના cff2_subset_plan &plan,
				hb_array_t<પૂર્ણાંક> normalized_coords) અચલ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    પ્રકારવ્યાખ્યા accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_કિંમતો_subset_t> SUPER;
  };

  જાહેર:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<ટોચ_Dict, HBUINT8>	ટોચ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			ટોચ_Dict_Size;	/* Top DICT size */

  જાહેર:
  DEFINE_SIZE_STATIC (5);
};

સંરચના cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

સંરચના cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
