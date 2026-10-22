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

#অন্তর্ভাব "hb-ot-cff-common.hh"
#অন্তর্ভাব "hb-subset-cff-common.hh"
#অন্তর্ভাব "hb-draw.hh"
#অন্তর্ভাব "hb-paint.hh"

নামাকাশ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

প্রকারনির্দেশ CFF2Index         CFF2CharStrings;
প্রকারনির্দেশ Subrs<HBUINT32>   CFF2Subrs;

প্রকারনির্দেশ FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
প্রকারনির্দেশ FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

সংরচনা CFF2FDSelect
{
  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুব CFF2FDSelect &src, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned পূর্ণাঙ্ক size = src.get_size (অঙ্ক_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    যদি (unlikely (!dest)) return_trace (অসত্য);
    hb_memcpy (dest, &src, size);
    return_trace (সত্য);
  }

  size_t get_size (unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুব
  {
    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); প্রত্যাগম format.static_size + u.format0.get_size (অঙ্ক_glyphs);
    পক্ষ 3: hb_barrier (); প্রত্যাগম format.static_size + u.format3.get_size ();
    পক্ষ 4: hb_barrier (); প্রত্যাগম format.static_size + u.format4.get_size ();
    স্বভাব:প্রত্যাগম 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ধ্রুব
  {
    যদি (এতত্ == &Null (CFF2FDSelect))
      প্রত্যাগম 0;

    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); প্রত্যাগম u.format0.get_fd (glyph);
    পক্ষ 3: hb_barrier (); প্রত্যাগম u.format3.get_fd (glyph);
    পক্ষ 4: hb_barrier (); প্রত্যাগম u.format4.get_fd (glyph);
    স্বভাব:প্রত্যাগম 0;
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণাঙ্ক fdcount,
                 unsigned পূর্ণাঙ্ক charstring_গুনতি = HB_CODEPOINT_INVALID) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্)))
      return_trace (অসত্য);
    hb_barrier ();

    বিকল্প (format)
    {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_গুনতি));
    পক্ষ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_গুনতি));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_গুনতি));
    স্বভাব:return_trace (অসত্য);
    }
  }

  HBUINT8	format;
  সঙ্ঘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  সার্বজনিক:
  DEFINE_SIZE_MIN (2);
};

সংরচনা CFF2ItemVariationStore
{
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  তার্কিক serialize (hb_serialize_context_t *c, ধ্রুব CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (এতত্);
    unsigned পূর্ণাঙ্ক size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    যদি (unlikely (!dest)) return_trace (অসত্য);
    hb_memcpy (dest, varStore, size_);
    return_trace (সত্য);
  }

  size_t get_size () ধ্রুব { প্রত্যাগম HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

সংরচনা cff2_শীর্ষ_dict_মানসমূহ_t : শীর্ষ_dict_মানসমূহ_t<>
{
  শূন্য init ()
  {
    শীর্ষ_dict_মানসমূহ_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  শূন্য fini () { শীর্ষ_dict_মানসমূহ_t<>::fini (); }

  পূর্ণাঙ্ক  vstoreOffset;
  পূর্ণাঙ্ক  FDSelectOffset;
};

সংরচনা cff2_শীর্ষ_dict_opset_t : শীর্ষ_dict_opset_t<>
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff2_শীর্ষ_dict_মানসমূহ_t& dictval)
  {
    বিকল্প (op) {
      পক্ষ OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	ভঙ্গ;

      পক্ষ OpCode_vstore:
	dictval.vstoreOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	যদি (!env.arg_স্তূপ.is_empty ()) প্রত্যাগম;
    }

    যদি (unlikely (env.in_error ())) প্রত্যাগম;

    dictval.add_op (op, env.str_ref);
  }

  প্রকারনির্দেশ শীর্ষ_dict_opset_t<> SUPER;
};

সংরচনা cff2_font_dict_মানসমূহ_t : dict_মানসমূহ_t<op_str_t>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  শূন্য fini () { dict_মানসমূহ_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

সংরচনা cff2_font_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff2_font_dict_মানসমূহ_t& dictval)
  {
    বিকল্প (op) {
      পক্ষ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বের_করো_uint ();
	dictval.privateDictInfo.size = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ())
	  প্রত্যাগম;
    }

    যদি (unlikely (env.in_error ())) প্রত্যাগম;

    dictval.add_op (op, env.str_ref);
  }

  গুপ্ত:
  প্রকারনির্দেশ dict_opset_t SUPER;
};

template <typename VAL>
সংরচনা cff2_private_dict_মানসমূহ_base_t : dict_মানসমূহ_t<VAL>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  শূন্য fini () { dict_মানসমূহ_t<VAL>::fini (); }

  পূর্ণাঙ্ক                subrsOffset;
  ধ্রুব CFF2Subrs   *localSubrs;
  unsigned পূর্ণাঙ্ক      ivs;
};

প্রকারনির্দেশ cff2_private_dict_মানসমূহ_base_t<op_str_t> cff2_private_dict_মানসমূহ_subset_t;
প্রকারনির্দেশ cff2_private_dict_মানসমূহ_base_t<অঙ্ক_dict_val_t> cff2_private_dict_মানসমূহ_t;

সংরচনা cff2_priv_dict_interp_env_t : অঙ্ক_interp_env_t
{
  cff2_priv_dict_interp_env_t (ধ্রুব hb_ubytes_t &str) :
    অঙ্ক_interp_env_t (str) {}

  শূন্য process_vsindex ()
  {
    যদি (likely (!seen_vsindex))
    {
      set_ivs (arg_স্তূপ.বের_করো_uint ());
    }
    seen_vsindex = সত্য;
  }

  unsigned পূর্ণাঙ্ক get_ivs () ধ্রুব { প্রত্যাগম ivs; }
  শূন্য	 set_ivs (unsigned পূর্ণাঙ্ক ivs_) { ivs = ivs_; }

  রক্ষিত:
  unsigned পূর্ণাঙ্ক  ivs = 0;
  তার্কিক	  seen_vsindex = অসত্য;
};

সংরচনা cff2_private_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_মানসমূহ_t& dictval)
  {
    অঙ্ক_dict_val_t val;
    val.init ();

    বিকল্প (op) {
      পক্ষ OpCode_StdHW:
      পক্ষ OpCode_StdVW:
      পক্ষ OpCode_BlueScale:
      পক্ষ OpCode_BlueShift:
      পক্ষ OpCode_BlueFuzz:
      পক্ষ OpCode_ExpansionFactor:
      পক্ষ OpCode_LanguageGroup:
      পক্ষ Op_Code_Blue_মানসমূহ:
      পক্ষ OpCode_OtherBlues:
      পক্ষ OpCode_FamilyBlues:
      পক্ষ OpCode_FamilyOtherBlues:
      পক্ষ OpCode_StemSnapH:
      পক্ষ OpCode_StemSnapV:
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ভঙ্গ;
      পক্ষ OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	ভঙ্গ;

      স্বভাব:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) প্রত্যাগম;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) প্রত্যাগম;

    dictval.add_op (op, env.str_ref, val);
  }
};

সংরচনা cff2_private_dict_opset_subset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_মানসমূহ_subset_t& dictval)
  {
    বিকল্প (op) {
      পক্ষ Op_Code_Blue_মানসমূহ:
      পক্ষ OpCode_OtherBlues:
      পক্ষ OpCode_FamilyBlues:
      পক্ষ OpCode_FamilyOtherBlues:
      পক্ষ OpCode_StdHW:
      পক্ষ OpCode_StdVW:
      পক্ষ OpCode_BlueScale:
      পক্ষ OpCode_BlueShift:
      পক্ষ OpCode_BlueFuzz:
      পক্ষ OpCode_StemSnapH:
      পক্ষ OpCode_StemSnapV:
      পক্ষ OpCode_LanguageGroup:
      পক্ষ OpCode_ExpansionFactor:
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_blenddict:
	env.clear_args ();
	প্রত্যাগম;

      পক্ষ OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	ভঙ্গ;

      পক্ষ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ভঙ্গ;

      স্বভাব:
	SUPER::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) প্রত্যাগম;
	ভঙ্গ;
    }

    যদি (unlikely (env.in_error ())) প্রত্যাগম;

    dictval.add_op (op, env.str_ref);
  }

  গুপ্ত:
  প্রকারনির্দেশ dict_opset_t SUPER;
};

প্রকারনির্দেশ dict_interpreter_t<cff2_শীর্ষ_dict_opset_t, cff2_শীর্ষ_dict_মানসমূহ_t> cff2_শীর্ষ_dict_interpreter_t;
প্রকারনির্দেশ dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_মানসমূহ_t> cff2_font_dict_interpreter_t;

সংরচনা CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  তার্কিক serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { প্রত্যাগম FDArray<HBUINT32>::serialize<cff2_font_dict_মানসমূহ_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

নামাকাশ OT {

using নামাকাশ CFF;

সংরচনা cff2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  সংরচনা accelerator_templ_t
  {
    স্থির constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      যদি (!face) প্রত্যাগম;

      শীর্ষ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      এতত্->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (এতত্->blob);
      sc.start_processing ();

      ধ্রুব OT::cff2 *cff2 = এতত্->blob->template as<OT::cff2> ();

      যদি (cff2 == &Null (OT::cff2))
        গচ্ছ fail;

      { /* parse top dict */
	hb_ubytes_t শীর্ষ_Dict_Str = (cff2 + cff2->শীর্ষ_Dict_2).as_ubytes (cff2->শীর্ষ_Dict_Size);
	যদি (unlikely (!শীর্ষ_Dict_Str.sanitize (&sc))) গচ্ছ fail;
	hb_barrier ();
	অঙ্ক_interp_env_t env (শীর্ষ_Dict_Str);
	cff2_শীর্ষ_dict_interpreter_t শীর্ষ_interp (env);
	শীর্ষ_Dict_2.init ();
	যদি (unlikely (!শীর্ষ_interp.interpret (শীর্ষ_Dict_2))) গচ্ছ fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->শীর্ষ_Dict_2 + cff2->শীর্ষ_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, শীর্ষ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, শীর্ষ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, শীর্ষ_Dict_2.FDArrayOffset, sc);

      যদি (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        গচ্ছ fail;

      অঙ্ক_glyphs = charStrings->গুনতি;
      যদি (অঙ্ক_glyphs != sc.get_অঙ্ক_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        তার্কিক valid_গুনতি = অঙ্ক_glyphs < sc.get_অঙ্ক_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        যদি (!valid_গুনতি) গচ্ছ fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, শীর্ষ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->গুনতি, অঙ্ক_glyphs);
      যদি (শীর্ষ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        গচ্ছ fail;

      fd_গুনতি = fdArray->গুনতি;
      যদি (!privateDicts.resize (fd_গুনতি))
        গচ্ছ fail;

      /* parse font dicts and gather private dicts */
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < fd_গুনতি; i++)
      {
	ধ্রুব hb_ubytes_t fontDictStr = (*fdArray)[i];
	যদি (unlikely (!fontDictStr.sanitize (&sc))) গচ্ছ fail;
	hb_barrier ();
	cff2_font_dict_মানসমূহ_t  *font;
	অঙ্ক_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	যদি (unlikely (font == &Crap (cff2_font_dict_মানসমূহ_t))) গচ্ছ fail;
	font->init ();
	যদি (unlikely (!font_interp.interpret (*font))) গচ্ছ fail;

	ধ্রুব hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	যদি (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ধ্রুব unsigned বর্ণ *) &Null (UnsizedByteStr))) গচ্ছ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	যদি (unlikely (!priv_interp.interpret (privateDicts[i]))) গচ্ছ fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      প্রত্যাগম;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    শূন্য _fini ()
    {
      sc.end_processing ();
      শীর্ষ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      যদি (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ধ্রুব
    {
      প্রত্যাগম nullptr;
    }

    hb_blob_t *get_blob () ধ্রুব { প্রত্যাগম blob; }

    তার্কিক is_valid () ধ্রুব { প্রত্যাগম blob; }

    রক্ষিত:
    hb_sanitize_context_t	sc;

    সার্বজনিক:
    hb_blob_t			*blob = nullptr;
    cff2_শীর্ষ_dict_মানসমূহ_t	শীর্ষ_Dict_2;
    ধ্রুব CFF2Subrs		*globalSubrs = nullptr;
    ধ্রুব CFF2ItemVariationStore	*varStore = nullptr;
    ধ্রুব CFF2CharStrings	*charStrings = nullptr;
    ধ্রুব CFF2FDArray		*fdArray = nullptr;
    ধ্রুব CFF2FDSelect		*fdSelect = nullptr;
    unsigned পূর্ণাঙ্ক		fd_গুনতি = 0;

    hb_vector_t<cff2_font_dict_মানসমূহ_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<ভিন্ন> *> cached_scalars_vector;

    unsigned পূর্ণাঙ্ক	      অঙ্ক_glyphs = 0;
  };

  সংরচনা accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_মানসমূহ_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL তার্কিক get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ধ্রুব;
    HB_INTERNAL তার্কিক get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords,
				     int64_t *budget = nullptr) ধ্রুব;
    HB_INTERNAL তার্কিক get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ধ্রুব;
    HB_INTERNAL তার্কিক get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ধ্রুব পূর্ণাঙ্ক> coords, int64_t *budget = nullptr) ধ্রুব;
  };

  সংরচনা accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_মানসমূহ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      যদি (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL তার্কিক subset (hb_subset_context_t *c) ধ্রুব;
    HB_INTERNAL তার্কিক serialize (hb_serialize_context_t *c,
				সংরচনা cff2_subset_plan &plan,
				hb_array_t<পূর্ণাঙ্ক> normalized_coords) ধ্রুব;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    প্রকারনির্দেশ accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_মানসমূহ_subset_t> SUPER;
  };

  সার্বজনিক:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<শীর্ষ_Dict, HBUINT8>	শীর্ষ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			শীর্ষ_Dict_Size;	/* Top DICT size */

  সার্বজনিক:
  DEFINE_SIZE_STATIC (5);
};

সংরচনা cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

সংরচনা cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
