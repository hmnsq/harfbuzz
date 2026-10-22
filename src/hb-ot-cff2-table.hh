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

প্রকারলেখ CFF2_সূচক         CFF2CharStrings;
প্রকারলেখ Subrs<HBUINT32>   CFF2Subrs;

প্রকারলেখ FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
প্রকারলেখ FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

কাঠামো CFF2FDSelect
{
  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক CFF2FDSelect &src, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs)
  {
    TRACE_SERIALIZE (এই);
    unsigned পূর্ণসংখ্যা আকার_2 = src.get_আকার (অঙ্ক_glyphs);
    CFF2FDSelect *dest = c->allocate_আকার<CFF2FDSelect> (আকার_2);
    যদি (unlikely (!dest)) return_trace (মিথ্যা);
    hb_memcpy (dest, &src, আকার_2);
    return_trace (সত্য);
  }

  size_t get_আকার (unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত format.static_আকার + u.format0.get_আকার (অঙ্ক_glyphs);
    ক্ষেত্র 3: hb_barrier (); ফেরত format.static_আকার + u.format3.get_আকার ();
    ক্ষেত্র 4: hb_barrier (); ফেরত format.static_আকার + u.format4.get_আকার ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ধ্রুবক
  {
    যদি (এই == &Null (CFF2FDSelect))
      ফেরত 0;

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_fd (glyph);
    ক্ষেত্র 3: hb_barrier (); ফেরত u.format3.get_fd (glyph);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_fd (glyph);
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned পূর্ণসংখ্যা fdcount,
                 unsigned পূর্ণসংখ্যা charstring_গুনতি = HB_CODEPOINT_INVALID) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই)))
      return_trace (মিথ্যা);
    hb_barrier ();

    নির্বাচন (format)
    {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_গুনতি));
    ক্ষেত্র 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_গুনতি));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_গুনতি));
    পূর্বনির্ধারিত:return_trace (মিথ্যা);
    }
  }

  HBUINT8	format;
  সংঘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  সর্বজনীন:
  DEFINE_আকার_MIN (2);
};

কাঠামো CFF2ItemVariationStore
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  c->check_range (&varStore, আকার_2) &&
		  varStore.sanitize (c));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c, ধ্রুবক CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (এই);
    unsigned পূর্ণসংখ্যা আকার_4 = varStore->get_আকার ();
    CFF2ItemVariationStore *dest = c->allocate_আকার<CFF2ItemVariationStore> (আকার_4);
    যদি (unlikely (!dest)) return_trace (মিথ্যা);
    hb_memcpy (dest, varStore, আকার_4);
    return_trace (সত্য);
  }

  size_t get_আকার () ধ্রুবক { ফেরত HBUINT16::static_আকার + আকার_2; }

  HBUINT16	আকার_2;
  ItemVariationStore  varStore;

  DEFINE_আকার_MIN (2 + ItemVariationStore::min_আকার);
};

কাঠামো cff2_শীর্ষ_dict_মানসমূহ_t : শীর্ষ_dict_মানসমূহ_t<>
{
  শূন্য init ()
  {
    শীর্ষ_dict_মানসমূহ_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  শূন্য fini () { শীর্ষ_dict_মানসমূহ_t<>::fini (); }

  পূর্ণসংখ্যা  vstoreOffset;
  পূর্ণসংখ্যা  FDSelectOffset;
};

কাঠামো cff2_শীর্ষ_dict_opset_t : শীর্ষ_dict_opset_t<>
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff2_শীর্ষ_dict_মানসমূহ_t& dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	বিরতি;

      ক্ষেত্র OpCode_vstore:
	dictval.vstoreOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref);
  }

  প্রকারলেখ শীর্ষ_dict_opset_t<> SUPER;
};

কাঠামো cff2_font_dict_মানসমূহ_t : dict_মানসমূহ_t<op_str_t>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  শূন্য fini () { dict_মানসমূহ_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

কাঠামো cff2_font_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, অঙ্ক_interp_env_t& env, cff2_font_dict_মানসমূহ_t& dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_স্তূপ.বের_করো_uint ();
	dictval.privateDictInfo.আকার_2 = env.arg_স্তূপ.বের_করো_uint ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ())
	  ফেরত;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref);
  }

  ব্যক্তিগত:
  প্রকারলেখ dict_opset_t SUPER;
};

template <typename VAL>
কাঠামো cff2_private_dict_মানসমূহ_base_t : dict_মানসমূহ_t<VAL>
{
  শূন্য init ()
  {
    dict_মানসমূহ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  শূন্য fini () { dict_মানসমূহ_t<VAL>::fini (); }

  পূর্ণসংখ্যা                subrsOffset;
  ধ্রুবক CFF2Subrs   *localSubrs;
  unsigned পূর্ণসংখ্যা      ivs;
};

প্রকারলেখ cff2_private_dict_মানসমূহ_base_t<op_str_t> cff2_private_dict_মানসমূহ_subset_t;
প্রকারলেখ cff2_private_dict_মানসমূহ_base_t<অঙ্ক_dict_val_t> cff2_private_dict_মানসমূহ_t;

কাঠামো cff2_priv_dict_interp_env_t : অঙ্ক_interp_env_t
{
  cff2_priv_dict_interp_env_t (ধ্রুবক hb_ubytes_t &str) :
    অঙ্ক_interp_env_t (str) {}

  শূন্য process_vsindex ()
  {
    যদি (likely (!seen_vsindex))
    {
      set_ivs (arg_স্তূপ.বের_করো_uint ());
    }
    seen_vsindex = সত্য;
  }

  unsigned পূর্ণসংখ্যা get_ivs () ধ্রুবক { ফেরত ivs; }
  শূন্য	 set_ivs (unsigned পূর্ণসংখ্যা ivs_) { ivs = ivs_; }

  সুরক্ষিত:
  unsigned পূর্ণসংখ্যা  ivs = 0;
  যৌক্তিক	  seen_vsindex = মিথ্যা;
};

কাঠামো cff2_private_dict_opset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_মানসমূহ_t& dictval)
  {
    অঙ্ক_dict_val_t val;
    val.init ();

    নির্বাচন (op) {
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_ExpansionFactor:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	বিরতি;
      ক্ষেত্র OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	dict_opset_t::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref, val);
  }
};

কাঠামো cff2_private_dict_opset_subset_t : dict_opset_t
{
  স্থির শূন্য process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_মানসমূহ_subset_t& dictval)
  {
    নির্বাচন (op) {
      ক্ষেত্র Op_Code_Blue_মানসমূহ:
      ক্ষেত্র OpCode_OtherBlues:
      ক্ষেত্র OpCode_FamilyBlues:
      ক্ষেত্র OpCode_FamilyOtherBlues:
      ক্ষেত্র OpCode_StdHW:
      ক্ষেত্র OpCode_StdVW:
      ক্ষেত্র OpCode_BlueScale:
      ক্ষেত্র OpCode_BlueShift:
      ক্ষেত্র OpCode_BlueFuzz:
      ক্ষেত্র OpCode_StemSnapH:
      ক্ষেত্র OpCode_StemSnapV:
      ক্ষেত্র OpCode_LanguageGroup:
      ক্ষেত্র OpCode_ExpansionFactor:
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_blenddict:
	env.clear_args ();
	ফেরত;

      ক্ষেত্র OpCode_Subrs:
	dictval.subrsOffset = env.arg_স্তূপ.বের_করো_int ();
	env.clear_args ();
	বিরতি;

      ক্ষেত্র OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	বিরতি;

      পূর্বনির্ধারিত:
	SUPER::process_op (op, env);
	যদি (!env.arg_স্তূপ.is_empty ()) ফেরত;
	বিরতি;
    }

    যদি (unlikely (env.in_error ())) ফেরত;

    dictval.add_op (op, env.str_ref);
  }

  ব্যক্তিগত:
  প্রকারলেখ dict_opset_t SUPER;
};

প্রকারলেখ dict_interpreter_t<cff2_শীর্ষ_dict_opset_t, cff2_শীর্ষ_dict_মানসমূহ_t> cff2_শীর্ষ_dict_interpreter_t;
প্রকারলেখ dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_মানসমূহ_t> cff2_font_dict_interpreter_t;

কাঠামো CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  যৌক্তিক serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ফেরত FDArray<HBUINT32>::serialize<cff2_font_dict_মানসমূহ_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

নামাকাশ OT {

using নামাকাশ CFF;

কাঠামো cff2
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  কাঠামো accelerator_templ_t
  {
    স্থির constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      যদি (!face) ফেরত;

      শীর্ষ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      এই->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (এই->blob);
      sc.start_processing ();

      ধ্রুবক OT::cff2 *cff2 = এই->blob->template as<OT::cff2> ();

      যদি (cff2 == &Null (OT::cff2))
        যাও fail;

      { /* parse top dict */
	hb_ubytes_t শীর্ষ_Dict_Str = (cff2 + cff2->শীর্ষ_Dict_2).as_ubytes (cff2->শীর্ষ_Dict_আকার);
	যদি (unlikely (!শীর্ষ_Dict_Str.sanitize (&sc))) যাও fail;
	hb_barrier ();
	অঙ্ক_interp_env_t env (শীর্ষ_Dict_Str);
	cff2_শীর্ষ_dict_interpreter_t শীর্ষ_interp (env);
	শীর্ষ_Dict_2.init ();
	যদি (unlikely (!শীর্ষ_interp.interpret (শীর্ষ_Dict_2))) যাও fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->শীর্ষ_Dict_2 + cff2->শীর্ষ_Dict_আকার, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, শীর্ষ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, শীর্ষ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, শীর্ষ_Dict_2.FDArrayOffset, sc);

      যদি (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        যাও fail;

      অঙ্ক_glyphs = charStrings->গুনতি;
      যদি (অঙ্ক_glyphs != sc.get_অঙ্ক_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        যৌক্তিক valid_গুনতি = অঙ্ক_glyphs < sc.get_অঙ্ক_glyphs () &&
                           hb_blob_get_দৈর্ঘ্য (varc);
        hb_blob_destroy (varc);
        যদি (!valid_গুনতি) যাও fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, শীর্ষ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->গুনতি, অঙ্ক_glyphs);
      যদি (শীর্ষ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        যাও fail;

      fd_গুনতি = fdArray->গুনতি;
      যদি (!privateDicts.resize (fd_গুনতি))
        যাও fail;

      /* parse font dicts and gather private dicts */
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < fd_গুনতি; i++)
      {
	ধ্রুবক hb_ubytes_t fontDictStr = (*fdArray)[i];
	যদি (unlikely (!fontDictStr.sanitize (&sc))) যাও fail;
	hb_barrier ();
	cff2_font_dict_মানসমূহ_t  *font;
	অঙ্ক_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	যদি (unlikely (font == &Crap (cff2_font_dict_মানসমূহ_t))) যাও fail;
	font->init ();
	যদি (unlikely (!font_interp.interpret (*font))) যাও fail;

	ধ্রুবক hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.আকার_2).as_ubytes (font->privateDictInfo.আকার_2);
	যদি (unlikely (font->privateDictInfo.আকার_2 &&
		      privDictStr == (ধ্রুবক unsigned অক্ষর *) &Null (UnsizedByteStr))) যাও fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	যদি (unlikely (!priv_interp.interpret (privateDicts[i]))) যাও fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      ফেরত;

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

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ধ্রুবক
    {
      ফেরত nullptr;
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত blob; }

    যৌক্তিক is_valid () ধ্রুবক { ফেরত blob; }

    সুরক্ষিত:
    hb_sanitize_context_t	sc;

    সর্বজনীন:
    hb_blob_t			*blob = nullptr;
    cff2_শীর্ষ_dict_মানসমূহ_t	শীর্ষ_Dict_2;
    ধ্রুবক CFF2Subrs		*globalSubrs = nullptr;
    ধ্রুবক CFF2ItemVariationStore	*varStore = nullptr;
    ধ্রুবক CFF2CharStrings	*charStrings = nullptr;
    ধ্রুবক CFF2FDArray		*fdArray = nullptr;
    ধ্রুবক CFF2FDSelect		*fdSelect = nullptr;
    unsigned পূর্ণসংখ্যা		fd_গুনতি = 0;

    hb_vector_t<cff2_font_dict_মানসমূহ_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<দশমিক> *> cached_scalars_vector;

    unsigned পূর্ণসংখ্যা	      অঙ্ক_glyphs = 0;
  };

  কাঠামো accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_মানসমূহ_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL যৌক্তিক get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ধ্রুবক;
    HB_INTERNAL যৌক্তিক get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords,
				     int64_t *budget = nullptr) ধ্রুবক;
    HB_INTERNAL যৌক্তিক get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ধ্রুবক;
    HB_INTERNAL যৌক্তিক get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ধ্রুবক পূর্ণসংখ্যা> coords, int64_t *budget = nullptr) ধ্রুবক;
  };

  কাঠামো accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_মানসমূহ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      যদি (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক;
    HB_INTERNAL যৌক্তিক serialize (hb_serialize_context_t *c,
				কাঠামো cff2_subset_plan &plan,
				hb_array_t<পূর্ণসংখ্যা> normalized_coords) ধ্রুবক;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    প্রকারলেখ accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_মানসমূহ_subset_t> SUPER;
  };

  সর্বজনীন:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<শীর্ষ_Dict, HBUINT8>	শীর্ষ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			শীর্ষ_Dict_আকার;	/* Top DICT size */

  সর্বজনীন:
  DEFINE_আকার_STATIC (5);
};

কাঠামো cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

কাঠামো cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
