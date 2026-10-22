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

#अन्तर्भाव "hb-ot-cff-common.hh"
#अन्तर्भाव "hb-subset-cff-common.hh"
#अन्तर्भाव "hb-draw.hh"
#अन्तर्भाव "hb-paint.hh"

नामाकाश CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

प्रकारनिर्देश CFF2Index         CFF2CharStrings;
प्रकारनिर्देश Subrs<HBUINT32>   CFF2Subrs;

प्रकारनिर्देश FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
प्रकारनिर्देश FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

संरचना CFF2FDSelect
{
  तार्किक serialize (hb_serialize_context_t *c, ध्रुव CFF2FDSelect &src, unsigned पूर्णाङ्क अङ्कः_glyphs)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned पूर्णाङ्क size = src.get_size (अङ्कः_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    सचे (unlikely (!dest)) return_trace (मिच्छा);
    hb_memcpy (dest, &src, size);
    return_trace (सच्च);
  }

  size_t get_size (unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); पटिनिवत्त format.static_size + u.format0.get_size (अङ्कः_glyphs);
    पक्ष 3: hb_barrier (); पटिनिवत्त format.static_size + u.format3.get_size ();
    पक्ष 4: hb_barrier (); पटिनिवत्त format.static_size + u.format4.get_size ();
    स्वभाव:पटिनिवत्त 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ध्रुव
  {
    सचे (एतत् == &Null (CFF2FDSelect))
      पटिनिवत्त 0;

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); पटिनिवत्त u.format0.get_fd (glyph);
    पक्ष 3: hb_barrier (); पटिनिवत्त u.format3.get_fd (glyph);
    पक्ष 4: hb_barrier (); पटिनिवत्त u.format4.get_fd (glyph);
    स्वभाव:पटिनिवत्त 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, unsigned पूर्णाङ्क fdcount,
                 unsigned पूर्णाङ्क charstring_गणनम् = HB_CODEPOINT_INVALID) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    सचे (unlikely (!c->check_struct (एतत्)))
      return_trace (मिच्छा);
    hb_barrier ();

    विकल्प (format)
    {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_गणनम्));
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_गणनम्));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_गणनम्));
    स्वभाव:return_trace (मिच्छा);
    }
  }

  HBUINT8	format;
  सङ्घ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

संरचना CFF2ItemVariationStore
{
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  तार्किक serialize (hb_serialize_context_t *c, ध्रुव CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (एतत्);
    unsigned पूर्णाङ्क size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    सचे (unlikely (!dest)) return_trace (मिच्छा);
    hb_memcpy (dest, varStore, size_);
    return_trace (सच्च);
  }

  size_t get_size () ध्रुव { पटिनिवत्त HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

संरचना cff2_शिखरम्_dict_मानानि_t : शिखरम्_dict_मानानि_t<>
{
  शून्य init ()
  {
    शिखरम्_dict_मानानि_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  शून्य fini () { शिखरम्_dict_मानानि_t<>::fini (); }

  पूर्णाङ्क  vstoreOffset;
  पूर्णाङ्क  FDSelectOffset;
};

संरचना cff2_शिखरम्_dict_opset_t : शिखरम्_dict_opset_t<>
{
  स्थिर शून्य process_op (op_code_t op, अङ्कः_interp_env_t& env, cff2_शिखरम्_dict_मानानि_t& dictval)
  {
    विकल्प (op) {
      पक्ष OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	भङ्ग;

      पक्ष OpCode_vstore:
	dictval.vstoreOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	सचे (!env.arg_राशिः.is_empty ()) पटिनिवत्त;
    }

    सचे (unlikely (env.in_error ())) पटिनिवत्त;

    dictval.add_op (op, env.str_ref);
  }

  प्रकारनिर्देश शिखरम्_dict_opset_t<> SUPER;
};

संरचना cff2_font_dict_मानानि_t : dict_मानानि_t<op_str_t>
{
  शून्य init ()
  {
    dict_मानानि_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  शून्य fini () { dict_मानानि_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

संरचना cff2_font_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अङ्कः_interp_env_t& env, cff2_font_dict_मानानि_t& dictval)
  {
    विकल्प (op) {
      पक्ष OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_राशिः.निष्कासय_uint ();
	dictval.privateDictInfo.size = env.arg_राशिः.निष्कासय_uint ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env);
	सचे (!env.arg_राशिः.is_empty ())
	  पटिनिवत्त;
    }

    सचे (unlikely (env.in_error ())) पटिनिवत्त;

    dictval.add_op (op, env.str_ref);
  }

  गुप्त:
  प्रकारनिर्देश dict_opset_t SUPER;
};

template <typename VAL>
संरचना cff2_private_dict_मानानि_base_t : dict_मानानि_t<VAL>
{
  शून्य init ()
  {
    dict_मानानि_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  शून्य fini () { dict_मानानि_t<VAL>::fini (); }

  पूर्णाङ्क                subrsOffset;
  ध्रुव CFF2Subrs   *localSubrs;
  unsigned पूर्णाङ्क      ivs;
};

प्रकारनिर्देश cff2_private_dict_मानानि_base_t<op_str_t> cff2_private_dict_मानानि_subset_t;
प्रकारनिर्देश cff2_private_dict_मानानि_base_t<अङ्कः_dict_val_t> cff2_private_dict_मानानि_t;

संरचना cff2_priv_dict_interp_env_t : अङ्कः_interp_env_t
{
  cff2_priv_dict_interp_env_t (ध्रुव hb_ubytes_t &str) :
    अङ्कः_interp_env_t (str) {}

  शून्य process_vsindex ()
  {
    सचे (likely (!seen_vsindex))
    {
      set_ivs (arg_राशिः.निष्कासय_uint ());
    }
    seen_vsindex = सच्च;
  }

  unsigned पूर्णाङ्क get_ivs () ध्रुव { पटिनिवत्त ivs; }
  शून्य	 set_ivs (unsigned पूर्णाङ्क ivs_) { ivs = ivs_; }

  रक्षित:
  unsigned पूर्णाङ्क  ivs = 0;
  तार्किक	  seen_vsindex = मिच्छा;
};

संरचना cff2_private_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मानानि_t& dictval)
  {
    अङ्कः_dict_val_t val;
    val.init ();

    विकल्प (op) {
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_ExpansionFactor:
      पक्ष OpCode_LanguageGroup:
      पक्ष Op_Code_Blue_मानानि:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_Subrs:
	dictval.subrsOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	भङ्ग;
      पक्ष OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	भङ्ग;

      स्वभाव:
	dict_opset_t::process_op (op, env);
	सचे (!env.arg_राशिः.is_empty ()) पटिनिवत्त;
	भङ्ग;
    }

    सचे (unlikely (env.in_error ())) पटिनिवत्त;

    dictval.add_op (op, env.str_ref, val);
  }
};

संरचना cff2_private_dict_opset_subset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मानानि_subset_t& dictval)
  {
    विकल्प (op) {
      पक्ष Op_Code_Blue_मानानि:
      पक्ष OpCode_OtherBlues:
      पक्ष OpCode_FamilyBlues:
      पक्ष OpCode_FamilyOtherBlues:
      पक्ष OpCode_StdHW:
      पक्ष OpCode_StdVW:
      पक्ष OpCode_BlueScale:
      पक्ष OpCode_BlueShift:
      पक्ष OpCode_BlueFuzz:
      पक्ष OpCode_StemSnapH:
      पक्ष OpCode_StemSnapV:
      पक्ष OpCode_LanguageGroup:
      पक्ष OpCode_ExpansionFactor:
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_blenddict:
	env.clear_args ();
	पटिनिवत्त;

      पक्ष OpCode_Subrs:
	dictval.subrsOffset = env.arg_राशिः.निष्कासय_int ();
	env.clear_args ();
	भङ्ग;

      पक्ष OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	भङ्ग;

      स्वभाव:
	SUPER::process_op (op, env);
	सचे (!env.arg_राशिः.is_empty ()) पटिनिवत्त;
	भङ्ग;
    }

    सचे (unlikely (env.in_error ())) पटिनिवत्त;

    dictval.add_op (op, env.str_ref);
  }

  गुप्त:
  प्रकारनिर्देश dict_opset_t SUPER;
};

प्रकारनिर्देश dict_interpreter_t<cff2_शिखरम्_dict_opset_t, cff2_शिखरम्_dict_मानानि_t> cff2_शिखरम्_dict_interpreter_t;
प्रकारनिर्देश dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_मानानि_t> cff2_font_dict_interpreter_t;

संरचना CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { पटिनिवत्त FDArray<HBUINT32>::serialize<cff2_font_dict_मानानि_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

नामाकाश OT {

using नामाकाश CFF;

संरचना cff2
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  संरचना accelerator_templ_t
  {
    स्थिर constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      सचे (!face) पटिनिवत्त;

      शिखरम्_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      एतत्->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (एतत्->blob);
      sc.start_processing ();

      ध्रुव OT::cff2 *cff2 = एतत्->blob->template as<OT::cff2> ();

      सचे (cff2 == &Null (OT::cff2))
        गच्छ fail;

      { /* parse top dict */
	hb_ubytes_t शिखरम्_Dict_Str = (cff2 + cff2->शिखरम्_Dict_2).as_ubytes (cff2->शिखरम्_Dict_Size);
	सचे (unlikely (!शिखरम्_Dict_Str.sanitize (&sc))) गच्छ fail;
	hb_barrier ();
	अङ्कः_interp_env_t env (शिखरम्_Dict_Str);
	cff2_शिखरम्_dict_interpreter_t शिखरम्_interp (env);
	शिखरम्_Dict_2.init ();
	सचे (unlikely (!शिखरम्_interp.interpret (शिखरम्_Dict_2))) गच्छ fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->शिखरम्_Dict_2 + cff2->शिखरम्_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, शिखरम्_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, शिखरम्_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, शिखरम्_Dict_2.FDArrayOffset, sc);

      सचे (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        गच्छ fail;

      अङ्कः_glyphs = charStrings->गणनम्;
      सचे (अङ्कः_glyphs != sc.get_अङ्कः_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        तार्किक valid_गणनम् = अङ्कः_glyphs < sc.get_अङ्कः_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        सचे (!valid_गणनम्) गच्छ fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, शिखरम्_Dict_2.FDSelectOffset, sc,
                                                   fdArray->गणनम्, अङ्कः_glyphs);
      सचे (शिखरम्_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        गच्छ fail;

      fd_गणनम् = fdArray->गणनम्;
      सचे (!privateDicts.resize (fd_गणनम्))
        गच्छ fail;

      /* parse font dicts and gather private dicts */
      अत्थाय (unsigned पूर्णाङ्क i = 0; i < fd_गणनम्; i++)
      {
	ध्रुव hb_ubytes_t fontDictStr = (*fdArray)[i];
	सचे (unlikely (!fontDictStr.sanitize (&sc))) गच्छ fail;
	hb_barrier ();
	cff2_font_dict_मानानि_t  *font;
	अङ्कः_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	सचे (unlikely (font == &Crap (cff2_font_dict_मानानि_t))) गच्छ fail;
	font->init ();
	सचे (unlikely (!font_interp.interpret (*font))) गच्छ fail;

	ध्रुव hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	सचे (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ध्रुव unsigned वर्ण *) &Null (UnsizedByteStr))) गच्छ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	सचे (unlikely (!priv_interp.interpret (privateDicts[i]))) गच्छ fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      पटिनिवत्त;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    शून्य _fini ()
    {
      sc.end_processing ();
      शिखरम्_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      सचे (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ध्रुव
    {
      पटिनिवत्त nullptr;
    }

    hb_blob_t *get_blob () ध्रुव { पटिनिवत्त blob; }

    तार्किक is_valid () ध्रुव { पटिनिवत्त blob; }

    रक्षित:
    hb_sanitize_context_t	sc;

    सार्वजनिक:
    hb_blob_t			*blob = nullptr;
    cff2_शिखरम्_dict_मानानि_t	शिखरम्_Dict_2;
    ध्रुव CFF2Subrs		*globalSubrs = nullptr;
    ध्रुव CFF2ItemVariationStore	*varStore = nullptr;
    ध्रुव CFF2CharStrings	*charStrings = nullptr;
    ध्रुव CFF2FDArray		*fdArray = nullptr;
    ध्रुव CFF2FDSelect		*fdSelect = nullptr;
    unsigned पूर्णाङ्क		fd_गणनम् = 0;

    hb_vector_t<cff2_font_dict_मानानि_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<भिन्न> *> cached_scalars_vector;

    unsigned पूर्णाङ्क	      अङ्कः_glyphs = 0;
  };

  संरचना accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_मानानि_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL तार्किक get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ध्रुव;
    HB_INTERNAL तार्किक get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ध्रुव पूर्णाङ्क> coords,
				     int64_t *budget = nullptr) ध्रुव;
    HB_INTERNAL तार्किक get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ध्रुव;
    HB_INTERNAL तार्किक get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ध्रुव पूर्णाङ्क> coords, int64_t *budget = nullptr) ध्रुव;
  };

  संरचना accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मानानि_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      सचे (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL तार्किक subset (hb_subset_context_t *c) ध्रुव;
    HB_INTERNAL तार्किक serialize (hb_serialize_context_t *c,
				संरचना cff2_subset_plan &plan,
				hb_array_t<पूर्णाङ्क> normalized_coords) ध्रुव;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकारनिर्देश accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मानानि_subset_t> SUPER;
  };

  सार्वजनिक:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<शिखरम्_Dict, HBUINT8>	शिखरम्_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			शिखरम्_Dict_Size;	/* Top DICT size */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (5);
};

संरचना cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

संरचना cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
