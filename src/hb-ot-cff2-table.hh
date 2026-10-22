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

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

نطاق CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

تعريفنوع CFF2Index         CFF2CharStrings;
تعريفنوع Subrs<HBUINT32>   CFF2Subrs;

تعريفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعريفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

بنية CFF2FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, unsigned صحيح num_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    unsigned صحيح size = src.get_size (num_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    ܐܢ (unlikely (!dest)) return_trace (ܕܓܠܐ);
    hb_memcpy (dest, &src, size);
    return_trace (ܫܪܪܐ);
  }

  size_t get_size (unsigned صحيح num_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); ܦܢܐ format.static_size + u.format0.get_size (num_glyphs);
    حالة 3: hb_barrier (); ܦܢܐ format.static_size + u.format3.get_size ();
    حالة 4: hb_barrier (); ܦܢܐ format.static_size + u.format4.get_size ();
    افتراضي:ܦܢܐ 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ثابت
  {
    ܐܢ (هذا == &Null (CFF2FDSelect))
      ܦܢܐ 0;

    بدل (format)
    {
    حالة 0: hb_barrier (); ܦܢܐ u.format0.get_fd (glyph);
    حالة 3: hb_barrier (); ܦܢܐ u.format3.get_fd (glyph);
    حالة 4: hb_barrier (); ܦܢܐ u.format4.get_fd (glyph);
    افتراضي:ܦܢܐ 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح charstring_count = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    ܐܢ (unlikely (!c->check_struct (هذا)))
      return_trace (ܕܓܠܐ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_count));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_count));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_count));
    افتراضي:return_trace (ܕܓܠܐ);
    }
  }

  HBUINT8	format;
  اتحاد {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  عام:
  DEFINE_SIZE_MIN (2);
};

بنية CFF2ItemVariationStore
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (هذا);
    unsigned صحيح size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    ܐܢ (unlikely (!dest)) return_trace (ܕܓܠܐ);
    hb_memcpy (dest, varStore, size_);
    return_trace (ܫܪܪܐ);
  }

  size_t get_size () ثابت { ܦܢܐ HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

بنية cff2_top_dict_values_t : top_dict_values_t<>
{
  فراغ init ()
  {
    top_dict_values_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  فراغ fini () { top_dict_values_t<>::fini (); }

  صحيح  vstoreOffset;
  صحيح  FDSelectOffset;
};

بنية cff2_top_dict_opset_t : top_dict_opset_t<>
{
  ساكن فراغ process_op (op_code_t op, num_interp_env_t& env, cff2_top_dict_values_t& dictval)
  {
    بدل (op) {
      حالة OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	اكسر;

      حالة OpCode_vstore:
	dictval.vstoreOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;
      حالة OpCode_FDSelect:
	dictval.FDSelectOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ܐܢ (!env.argStack.is_empty ()) ܦܢܐ;
    }

    ܐܢ (unlikely (env.in_error ())) ܦܢܐ;

    dictval.add_op (op, env.str_ref);
  }

  تعريفنوع top_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_values_t : dict_values_t<op_str_t>
{
  فراغ init ()
  {
    dict_values_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  فراغ fini () { dict_values_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, num_interp_env_t& env, cff2_font_dict_values_t& dictval)
  {
    بدل (op) {
      حالة OpCode_Private:
	dictval.privateDictInfo.offset = env.argStack.pop_uint ();
	dictval.privateDictInfo.size = env.argStack.pop_uint ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	ܐܢ (!env.argStack.is_empty ())
	  ܦܢܐ;
    }

    ܐܢ (unlikely (env.in_error ())) ܦܢܐ;

    dictval.add_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

template <typename VAL>
بنية cff2_private_dict_values_base_t : dict_values_t<VAL>
{
  فراغ init ()
  {
    dict_values_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_values_t<VAL>::fini (); }

  صحيح                subrsOffset;
  ثابت CFF2Subrs   *localSubrs;
  unsigned صحيح      ivs;
};

تعريفنوع cff2_private_dict_values_base_t<op_str_t> cff2_private_dict_values_subset_t;
تعريفنوع cff2_private_dict_values_base_t<num_dict_val_t> cff2_private_dict_values_t;

بنية cff2_priv_dict_interp_env_t : num_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    num_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    ܐܢ (likely (!seen_vsindex))
    {
      set_ivs (argStack.pop_uint ());
    }
    seen_vsindex = ܫܪܪܐ;
  }

  unsigned صحيح get_ivs () ثابت { ܦܢܐ ivs; }
  فراغ	 set_ivs (unsigned صحيح ivs_) { ivs = ivs_; }

  محمي:
  unsigned صحيح  ivs = 0;
  منطقي	  seen_vsindex = ܕܓܠܐ;
};

بنية cff2_private_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_values_t& dictval)
  {
    num_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ExpansionFactor:
      حالة OpCode_LanguageGroup:
      حالة OpCode_BlueValues:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
	env.clear_args ();
	اكسر;
      حالة OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;
      حالة OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	اكسر;
      حالة OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	ܐܢ (!env.argStack.is_empty ()) ܦܢܐ;
	اكسر;
    }

    ܐܢ (unlikely (env.in_error ())) ܦܢܐ;

    dictval.add_op (op, env.str_ref, val);
  }
};

بنية cff2_private_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_values_subset_t& dictval)
  {
    بدل (op) {
      حالة OpCode_BlueValues:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
      حالة OpCode_LanguageGroup:
      حالة OpCode_ExpansionFactor:
	env.clear_args ();
	اكسر;

      حالة OpCode_blenddict:
	env.clear_args ();
	ܦܢܐ;

      حالة OpCode_Subrs:
	dictval.subrsOffset = env.argStack.pop_int ();
	env.clear_args ();
	اكسر;

      حالة OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	ܐܢ (!env.argStack.is_empty ()) ܦܢܐ;
	اكسر;
    }

    ܐܢ (unlikely (env.in_error ())) ܦܢܐ;

    dictval.add_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_top_dict_opset_t, cff2_top_dict_values_t> cff2_top_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_values_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ܦܢܐ FDArray<HBUINT32>::serialize<cff2_font_dict_values_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

using نطاق CFF;

بنية cff2
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      ܐܢ (!face) ܦܢܐ;

      topDict.init ();
      fontDicts.init ();
      privateDicts.init ();

      هذا->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.start_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->template as<OT::cff2> ();

      ܐܢ (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t topDictStr = (cff2 + cff2->topDict).as_ubytes (cff2->topDictSize);
	ܐܢ (unlikely (!topDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	num_interp_env_t env (topDictStr);
	cff2_top_dict_interpreter_t top_interp (env);
	topDict.init ();
	ܐܢ (unlikely (!top_interp.interpret (topDict))) اذهب fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->topDict + cff2->topDictSize, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, topDict.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, topDict.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, topDict.FDArrayOffset, sc);

      ܐܢ (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      num_glyphs = charStrings->count;
      ܐܢ (num_glyphs != sc.get_num_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        منطقي valid_count = num_glyphs < sc.get_num_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        ܐܢ (!valid_count) اذهب fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, topDict.FDSelectOffset, sc,
                                                   fdArray->count, num_glyphs);
      ܐܢ (topDict.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fdCount = fdArray->count;
      ܐܢ (!privateDicts.resize (fdCount))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (unsigned صحيح i = 0; i < fdCount; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	ܐܢ (unlikely (!fontDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	cff2_font_dict_values_t  *font;
	num_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	ܐܢ (unlikely (font == &Crap (cff2_font_dict_values_t))) اذهب fail;
	font->init ();
	ܐܢ (unlikely (!font_interp.interpret (*font))) اذهب fail;

	ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	ܐܢ (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ثابت unsigned حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	ܐܢ (unlikely (!priv_interp.interpret (privateDicts[i]))) اذهب fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      ܦܢܐ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.end_processing ();
      topDict.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      ܐܢ (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ثابت
    {
      ܦܢܐ nullptr;
    }

    hb_blob_t *get_blob () ثابت { ܦܢܐ blob; }

    منطقي is_valid () ثابت { ܦܢܐ blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = nullptr;
    cff2_top_dict_values_t	topDict;
    ثابت CFF2Subrs		*globalSubrs = nullptr;
    ثابت CFF2ItemVariationStore	*varStore = nullptr;
    ثابت CFF2CharStrings	*charStrings = nullptr;
    ثابت CFF2FDArray		*fdArray = nullptr;
    ثابت CFF2FDSelect		*fdSelect = nullptr;
    unsigned صحيح		fdCount = 0;

    hb_vector_t<cff2_font_dict_values_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<عائم> *> cached_scalars_vector;

    unsigned صحيح	      num_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_values_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL منطقي get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_INTERNAL منطقي get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحيح> coords,
				     int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقي get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقي get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget = nullptr) ثابت;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_values_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ܐܢ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL منطقي subset (hb_subset_context_t *c) ثابت;
    HB_INTERNAL منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    تعريفنوع accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_values_subset_t> SUPER;
  };

  عام:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<TopDict, HBUINT8>	topDict;	/* headerSize = Offset to Top DICT. */
  HBUINT16			topDictSize;	/* Top DICT size */

  عام:
  DEFINE_SIZE_STATIC (5);
};

بنية cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

بنية cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
