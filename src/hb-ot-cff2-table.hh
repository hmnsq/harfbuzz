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
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, unsigned صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    unsigned صحيح size = src.get_size (عدد_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, size);
    return_trace (true);
  }

  size_t get_size (unsigned صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع format.static_size + u.format0.get_size (عدد_glyphs);
    حالة 3: hb_barrier (); أرجع format.static_size + u.format3.get_size ();
    حالة 4: hb_barrier (); أرجع format.static_size + u.format4.get_size ();
    افتراضي:أرجع 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ثابت
  {
    إذا (هذا == &Null (CFF2FDSelect))
      أرجع 0;

    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع u.format0.get_fd (glyph);
    حالة 3: hb_barrier (); أرجع u.format3.get_fd (glyph);
    حالة 4: hb_barrier (); أرجع u.format4.get_fd (glyph);
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c, unsigned صحيح fdcount,
                 unsigned صحيح charstring_گنتی = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_گنتی));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_گنتی));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_گنتی));
    افتراضي:return_trace (خطأ);
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
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, varStore, size_);
    return_trace (true);
  }

  size_t get_size () ثابت { أرجع HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

بنية cff2_چوٹی_dict_اقدار_t : چوٹی_dict_اقدار_t<>
{
  فراغ init ()
  {
    چوٹی_dict_اقدار_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  فراغ fini () { چوٹی_dict_اقدار_t<>::fini (); }

  صحيح  vstoreOffset;
  صحيح  FDSelectOffset;
};

بنية cff2_چوٹی_dict_opset_t : چوٹی_dict_opset_t<>
{
  ساكن فراغ process_op (op_code_t op, عدد_interp_env_t& env, cff2_چوٹی_dict_اقدار_t& dictval)
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
	dictval.vstoreOffset = env.arg_ڈھیر.نکالو_int ();
	env.clear_args ();
	اكسر;
      حالة OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ڈھیر.نکالو_int ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
    }

    إذا (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref);
  }

  تعريفنوع چوٹی_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_اقدار_t : dict_اقدار_t<op_str_t>
{
  فراغ init ()
  {
    dict_اقدار_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  فراغ fini () { dict_اقدار_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, عدد_interp_env_t& env, cff2_font_dict_اقدار_t& dictval)
  {
    بدل (op) {
      حالة OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ڈھیر.نکالو_uint ();
	dictval.privateDictInfo.size = env.arg_ڈھیر.نکالو_uint ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ())
	  أرجع;
    }

    إذا (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

template <typename VAL>
بنية cff2_private_dict_اقدار_base_t : dict_اقدار_t<VAL>
{
  فراغ init ()
  {
    dict_اقدار_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_اقدار_t<VAL>::fini (); }

  صحيح                subrsOffset;
  ثابت CFF2Subrs   *localSubrs;
  unsigned صحيح      ivs;
};

تعريفنوع cff2_private_dict_اقدار_base_t<op_str_t> cff2_private_dict_اقدار_subset_t;
تعريفنوع cff2_private_dict_اقدار_base_t<عدد_dict_val_t> cff2_private_dict_اقدار_t;

بنية cff2_priv_dict_interp_env_t : عدد_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    عدد_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    إذا (likely (!seen_vsindex))
    {
      set_ivs (arg_ڈھیر.نکالو_uint ());
    }
    seen_vsindex = true;
  }

  unsigned صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 set_ivs (unsigned صحيح ivs_) { ivs = ivs_; }

  محمي:
  unsigned صحيح  ivs = 0;
  منطقي	  seen_vsindex = خطأ;
};

بنية cff2_private_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_اقدار_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة OpCode_StdHW:
      حالة OpCode_StdVW:
      حالة OpCode_BlueScale:
      حالة OpCode_BlueShift:
      حالة OpCode_BlueFuzz:
      حالة OpCode_ExpansionFactor:
      حالة OpCode_LanguageGroup:
      حالة Op_Code_Blue_اقدار:
      حالة OpCode_OtherBlues:
      حالة OpCode_FamilyBlues:
      حالة OpCode_FamilyOtherBlues:
      حالة OpCode_StemSnapH:
      حالة OpCode_StemSnapV:
	env.clear_args ();
	اكسر;
      حالة OpCode_Subrs:
	dictval.subrsOffset = env.arg_ڈھیر.نکالو_int ();
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
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref, val);
  }
};

بنية cff2_private_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_اقدار_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_Code_Blue_اقدار:
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
	أرجع;

      حالة OpCode_Subrs:
	dictval.subrsOffset = env.arg_ڈھیر.نکالو_int ();
	env.clear_args ();
	اكسر;

      حالة OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.in_error ())) أرجع;

    dictval.add_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_چوٹی_dict_opset_t, cff2_چوٹی_dict_اقدار_t> cff2_چوٹی_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_اقدار_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT32>::serialize<cff2_font_dict_اقدار_t, table_info_t> (c, it, opszr); }
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
      إذا (!face) أرجع;

      چوٹی_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      هذا->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.start_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->template as<OT::cff2> ();

      إذا (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t چوٹی_Dict_Str = (cff2 + cff2->چوٹی_Dict_2).as_ubytes (cff2->چوٹی_Dict_Size);
	إذا (unlikely (!چوٹی_Dict_Str.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	عدد_interp_env_t env (چوٹی_Dict_Str);
	cff2_چوٹی_dict_interpreter_t چوٹی_interp (env);
	چوٹی_Dict_2.init ();
	إذا (unlikely (!چوٹی_interp.interpret (چوٹی_Dict_2))) اذهب fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->چوٹی_Dict_2 + cff2->چوٹی_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, چوٹی_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, چوٹی_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, چوٹی_Dict_2.FDArrayOffset, sc);

      إذا (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      عدد_glyphs = charStrings->گنتی;
      إذا (عدد_glyphs != sc.get_عدد_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        منطقي valid_گنتی = عدد_glyphs < sc.get_عدد_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        إذا (!valid_گنتی) اذهب fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, چوٹی_Dict_2.FDSelectOffset, sc,
                                                   fdArray->گنتی, عدد_glyphs);
      إذا (چوٹی_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fd_گنتی = fdArray->گنتی;
      إذا (!privateDicts.resize (fd_گنتی))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (unsigned صحيح i = 0; i < fd_گنتی; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	إذا (unlikely (!fontDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	cff2_font_dict_اقدار_t  *font;
	عدد_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	إذا (unlikely (font == &Crap (cff2_font_dict_اقدار_t))) اذهب fail;
	font->init ();
	إذا (unlikely (!font_interp.interpret (*font))) اذهب fail;

	ثابت hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	إذا (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ثابت unsigned حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	إذا (unlikely (!priv_interp.interpret (privateDicts[i]))) اذهب fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.end_processing ();
      چوٹی_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      إذا (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ثابت
    {
      أرجع nullptr;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = nullptr;
    cff2_چوٹی_dict_اقدار_t	چوٹی_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = nullptr;
    ثابت CFF2ItemVariationStore	*varStore = nullptr;
    ثابت CFF2CharStrings	*charStrings = nullptr;
    ثابت CFF2FDArray		*fdArray = nullptr;
    ثابت CFF2FDSelect		*fdSelect = nullptr;
    unsigned صحيح		fd_گنتی = 0;

    hb_vector_t<cff2_font_dict_اقدار_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<عائم> *> cached_scalars_vector;

    unsigned صحيح	      عدد_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_اقدار_t>
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

  بنية accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_اقدار_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL منطقي subset (hb_subset_context_t *c) ثابت;
    HB_INTERNAL منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    تعريفنوع accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_اقدار_subset_t> SUPER;
  };

  عام:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<چوٹی_Dict, HBUINT8>	چوٹی_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			چوٹی_Dict_Size;	/* Top DICT size */

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
