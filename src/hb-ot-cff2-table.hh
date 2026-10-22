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

#شامل "hb-ot-cff-common.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-draw.hh"
#شامل "hb-paint.hh"

فضاینام CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

تعریفنوع CFF2Index         CFF2CharStrings;
تعریفنوع Subrs<HBUINT32>   CFF2Subrs;

تعریفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعریفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ساختار CFF2FDSelect
{
  منطقی serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, unsigned صحیح رقم_glyphs)
  {
    TRACE_SERIALIZE (این);
    unsigned صحیح size = src.get_size (رقم_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    اگر (unlikely (!dest)) return_trace (نادرست);
    hb_memcpy (dest, &src, size);
    return_trace (درست);
  }

  size_t get_size (unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت format.static_size + u.format0.get_size (رقم_glyphs);
    حالت 3: hb_barrier (); بازگشت format.static_size + u.format3.get_size ();
    حالت 4: hb_barrier (); بازگشت format.static_size + u.format4.get_size ();
    پیشفرض:بازگشت 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ثابت
  {
    اگر (این == &Null (CFF2FDSelect))
      بازگشت 0;

    انتخاب (format)
    {
    حالت 0: hb_barrier (); بازگشت u.format0.get_fd (glyph);
    حالت 3: hb_barrier (); بازگشت u.format3.get_fd (glyph);
    حالت 4: hb_barrier (); بازگشت u.format4.get_fd (glyph);
    پیشفرض:بازگشت 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, unsigned صحیح fdcount,
                 unsigned صحیح charstring_شمار = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این)))
      return_trace (نادرست);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_شمار));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_شمار));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_شمار));
    پیشفرض:return_trace (نادرست);
    }
  }

  HBUINT8	format;
  اجتماع {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  عمومی:
  DEFINE_SIZE_MIN (2);
};

ساختار CFF2ItemVariationStore
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  منطقی serialize (hb_serialize_context_t *c, ثابت CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (این);
    unsigned صحیح size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    اگر (unlikely (!dest)) return_trace (نادرست);
    hb_memcpy (dest, varStore, size_);
    return_trace (درست);
  }

  size_t get_size () ثابت { بازگشت HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

ساختار cff2_بالاترین_dict_مقادیر_t : بالاترین_dict_مقادیر_t<>
{
  تهی init ()
  {
    بالاترین_dict_مقادیر_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  تهی fini () { بالاترین_dict_مقادیر_t<>::fini (); }

  صحیح  vstoreOffset;
  صحیح  FDSelectOffset;
};

ساختار cff2_بالاترین_dict_opset_t : بالاترین_dict_opset_t<>
{
  ایستا تهی process_op (op_code_t op, رقم_interp_env_t& env, cff2_بالاترین_dict_مقادیر_t& dictval)
  {
    انتخاب (op) {
      حالت OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	بشکن;

      حالت OpCode_vstore:
	dictval.vstoreOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;
      حالت OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref);
  }

  تعریفنوع بالاترین_dict_opset_t<> SUPER;
};

ساختار cff2_font_dict_مقادیر_t : dict_مقادیر_t<op_str_t>
{
  تهی init ()
  {
    dict_مقادیر_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  تهی fini () { dict_مقادیر_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

ساختار cff2_font_dict_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, رقم_interp_env_t& env, cff2_font_dict_مقادیر_t& dictval)
  {
    انتخاب (op) {
      حالت OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_پشته.برداشت_uint ();
	dictval.privateDictInfo.size = env.arg_پشته.برداشت_uint ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ())
	  بازگشت;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref);
  }

  خصوصی:
  تعریفنوع dict_opset_t SUPER;
};

template <typename VAL>
ساختار cff2_private_dict_مقادیر_base_t : dict_مقادیر_t<VAL>
{
  تهی init ()
  {
    dict_مقادیر_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  تهی fini () { dict_مقادیر_t<VAL>::fini (); }

  صحیح                subrsOffset;
  ثابت CFF2Subrs   *localSubrs;
  unsigned صحیح      ivs;
};

تعریفنوع cff2_private_dict_مقادیر_base_t<op_str_t> cff2_private_dict_مقادیر_subset_t;
تعریفنوع cff2_private_dict_مقادیر_base_t<رقم_dict_val_t> cff2_private_dict_مقادیر_t;

ساختار cff2_priv_dict_interp_env_t : رقم_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    رقم_interp_env_t (str) {}

  تهی process_vsindex ()
  {
    اگر (likely (!seen_vsindex))
    {
      set_ivs (arg_پشته.برداشت_uint ());
    }
    seen_vsindex = درست;
  }

  unsigned صحیح get_ivs () ثابت { بازگشت ivs; }
  تهی	 set_ivs (unsigned صحیح ivs_) { ivs = ivs_; }

  محافظتشده:
  unsigned صحیح  ivs = 0;
  منطقی	  seen_vsindex = نادرست;
};

ساختار cff2_private_dict_opset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_مقادیر_t& dictval)
  {
    رقم_dict_val_t val;
    val.init ();

    انتخاب (op) {
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_ExpansionFactor:
      حالت OpCode_LanguageGroup:
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
	env.clear_args ();
	بشکن;
      حالت OpCode_Subrs:
	dictval.subrsOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;
      حالت OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	بشکن;
      حالت OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	بشکن;

      پیشفرض:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref, val);
  }
};

ساختار cff2_private_dict_opset_subset_t : dict_opset_t
{
  ایستا تهی process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_مقادیر_subset_t& dictval)
  {
    انتخاب (op) {
      حالت Op_Code_Blue_مقادیر:
      حالت OpCode_OtherBlues:
      حالت OpCode_FamilyBlues:
      حالت OpCode_FamilyOtherBlues:
      حالت OpCode_StdHW:
      حالت OpCode_StdVW:
      حالت OpCode_BlueScale:
      حالت OpCode_BlueShift:
      حالت OpCode_BlueFuzz:
      حالت OpCode_StemSnapH:
      حالت OpCode_StemSnapV:
      حالت OpCode_LanguageGroup:
      حالت OpCode_ExpansionFactor:
	env.clear_args ();
	بشکن;

      حالت OpCode_blenddict:
	env.clear_args ();
	بازگشت;

      حالت OpCode_Subrs:
	dictval.subrsOffset = env.arg_پشته.برداشت_int ();
	env.clear_args ();
	بشکن;

      حالت OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	بشکن;

      پیشفرض:
	SUPER::process_op (op, env);
	اگر (!env.arg_پشته.is_empty ()) بازگشت;
	بشکن;
    }

    اگر (unlikely (env.in_error ())) بازگشت;

    dictval.add_op (op, env.str_ref);
  }

  خصوصی:
  تعریفنوع dict_opset_t SUPER;
};

تعریفنوع dict_interpreter_t<cff2_بالاترین_dict_opset_t, cff2_بالاترین_dict_مقادیر_t> cff2_بالاترین_dict_interpreter_t;
تعریفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_مقادیر_t> cff2_font_dict_interpreter_t;

ساختار CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { بازگشت FDArray<HBUINT32>::serialize<cff2_font_dict_مقادیر_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

فضاینام OT {

using فضاینام CFF;

ساختار cff2
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ساختار accelerator_templ_t
  {
    ایستا constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      اگر (!face) بازگشت;

      بالاترین_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      این->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (این->blob);
      sc.start_processing ();

      ثابت OT::cff2 *cff2 = این->blob->template as<OT::cff2> ();

      اگر (cff2 == &Null (OT::cff2))
        برو fail;

      { /* parse top dict */
	hb_ubytes_t بالاترین_Dict_Str = (cff2 + cff2->بالاترین_Dict_2).as_ubytes (cff2->بالاترین_Dict_Size);
	اگر (unlikely (!بالاترین_Dict_Str.sanitize (&sc))) برو fail;
	hb_barrier ();
	رقم_interp_env_t env (بالاترین_Dict_Str);
	cff2_بالاترین_dict_interpreter_t بالاترین_interp (env);
	بالاترین_Dict_2.init ();
	اگر (unlikely (!بالاترین_interp.interpret (بالاترین_Dict_2))) برو fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->بالاترین_Dict_2 + cff2->بالاترین_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, بالاترین_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, بالاترین_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, بالاترین_Dict_2.FDArrayOffset, sc);

      اگر (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        برو fail;

      رقم_glyphs = charStrings->شمار;
      اگر (رقم_glyphs != sc.get_رقم_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        منطقی valid_شمار = رقم_glyphs < sc.get_رقم_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        اگر (!valid_شمار) برو fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, بالاترین_Dict_2.FDSelectOffset, sc,
                                                   fdArray->شمار, رقم_glyphs);
      اگر (بالاترین_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        برو fail;

      fd_شمار = fdArray->شمار;
      اگر (!privateDicts.resize (fd_شمار))
        برو fail;

      /* parse font dicts and gather private dicts */
      برای (unsigned صحیح i = 0; i < fd_شمار; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	اگر (unlikely (!fontDictStr.sanitize (&sc))) برو fail;
	hb_barrier ();
	cff2_font_dict_مقادیر_t  *font;
	رقم_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	اگر (unlikely (font == &Crap (cff2_font_dict_مقادیر_t))) برو fail;
	font->init ();
	اگر (unlikely (!font_interp.interpret (*font))) برو fail;

	ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	اگر (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ثابت unsigned نویسه *) &Null (UnsizedByteStr))) برو fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	اگر (unlikely (!priv_interp.interpret (privateDicts[i]))) برو fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      بازگشت;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    تهی _fini ()
    {
      sc.end_processing ();
      بالاترین_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      اگر (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ثابت
    {
      بازگشت nullptr;
    }

    hb_blob_t *get_blob () ثابت { بازگشت blob; }

    منطقی is_valid () ثابت { بازگشت blob; }

    محافظتشده:
    hb_sanitize_context_t	sc;

    عمومی:
    hb_blob_t			*blob = nullptr;
    cff2_بالاترین_dict_مقادیر_t	بالاترین_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = nullptr;
    ثابت CFF2ItemVariationStore	*varStore = nullptr;
    ثابت CFF2CharStrings	*charStrings = nullptr;
    ثابت CFF2FDArray		*fdArray = nullptr;
    ثابت CFF2FDSelect		*fdSelect = nullptr;
    unsigned صحیح		fd_شمار = 0;

    hb_vector_t<cff2_font_dict_مقادیر_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<اعشاری> *> cached_scalars_vector;

    unsigned صحیح	      رقم_glyphs = 0;
  };

  ساختار accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_مقادیر_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL منطقی get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_INTERNAL منطقی get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحیح> coords,
				     int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقی get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ثابت;
    HB_INTERNAL منطقی get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحیح> coords, int64_t *budget = nullptr) ثابت;
  };

  ساختار accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_مقادیر_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      اگر (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL منطقی subset (hb_subset_context_t *c) ثابت;
    HB_INTERNAL منطقی serialize (hb_serialize_context_t *c,
				ساختار cff2_subset_plan &plan,
				hb_array_t<صحیح> normalized_coords) ثابت;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    تعریفنوع accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_مقادیر_subset_t> SUPER;
  };

  عمومی:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<بالاترین_Dict, HBUINT8>	بالاترین_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			بالاترین_Dict_Size;	/* Top DICT size */

  عمومی:
  DEFINE_SIZE_STATIC (5);
};

ساختار cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ساختار cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
