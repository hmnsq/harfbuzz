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

#அந்தர்பாவ "hb-ot-cff-common.hh"
#அந்தர்பாவ "hb-subset-cff-common.hh"
#அந்தர்பாவ "hb-draw.hh"
#அந்தர்பாவ "hb-paint.hh"

நாமாகாஶ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

வகைவரை CFF2Index         CFF2CharStrings;
வகைவரை Subrs<HBUINT32>   CFF2Subrs;

வகைவரை FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
வகைவரை FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

கட்டமைப்பு CFF2FDSelect
{
  தருக்க serialize (hb_serialize_context_t *c, மாறிலி CFF2FDSelect &src, unsigned முழு இலக்கம்_glyphs)
  {
    TRACE_SERIALIZE (இது);
    unsigned முழு size = src.get_size (இலக்கம்_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    எனில் (unlikely (!dest)) return_trace (பொய்);
    hb_memcpy (dest, &src, size);
    return_trace (உண்மை);
  }

  size_t get_size (unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு format.static_size + u.format0.get_size (இலக்கம்_glyphs);
    நிலை 3: hb_barrier (); திரும்பு format.static_size + u.format3.get_size ();
    நிலை 4: hb_barrier (); திரும்பு format.static_size + u.format4.get_size ();
    இயல்பு:திரும்பு 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) மாறிலி
  {
    எனில் (இது == &Null (CFF2FDSelect))
      திரும்பு 0;

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_fd (glyph);
    நிலை 3: hb_barrier (); திரும்பு u.format3.get_fd (glyph);
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_fd (glyph);
    இயல்பு:திரும்பு 0;
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c, unsigned முழு fdcount,
                 unsigned முழு charstring_மொத்தம் = HB_CODEPOINT_INVALID) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!c->check_struct (இது)))
      return_trace (பொய்);
    hb_barrier ();

    தேர்வு (format)
    {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_மொத்தம்));
    நிலை 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_மொத்தம்));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_மொத்தம்));
    இயல்பு:return_trace (பொய்);
    }
  }

  HBUINT8	format;
  ஒன்றிப்பு {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  பொது:
  DEFINE_SIZE_MIN (2);
};

கட்டமைப்பு CFF2ItemVariationStore
{
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  தருக்க serialize (hb_serialize_context_t *c, மாறிலி CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (இது);
    unsigned முழு size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    எனில் (unlikely (!dest)) return_trace (பொய்);
    hb_memcpy (dest, varStore, size_);
    return_trace (உண்மை);
  }

  size_t get_size () மாறிலி { திரும்பு HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

கட்டமைப்பு cff2_உச்சி_dict_மதிப்புகள்_t : உச்சி_dict_மதிப்புகள்_t<>
{
  வெறுமை init ()
  {
    உச்சி_dict_மதிப்புகள்_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  வெறுமை fini () { உச்சி_dict_மதிப்புகள்_t<>::fini (); }

  முழு  vstoreOffset;
  முழு  FDSelectOffset;
};

கட்டமைப்பு cff2_உச்சி_dict_opset_t : உச்சி_dict_opset_t<>
{
  நிலையான வெறுமை process_op (op_code_t op, இலக்கம்_interp_env_t& env, cff2_உச்சி_dict_மதிப்புகள்_t& dictval)
  {
    தேர்வு (op) {
      நிலை OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	நிறுத்து;

      நிலை OpCode_vstore:
	dictval.vstoreOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref);
  }

  வகைவரை உச்சி_dict_opset_t<> SUPER;
};

கட்டமைப்பு cff2_font_dict_மதிப்புகள்_t : dict_மதிப்புகள்_t<op_str_t>
{
  வெறுமை init ()
  {
    dict_மதிப்புகள்_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  வெறுமை fini () { dict_மதிப்புகள்_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

கட்டமைப்பு cff2_font_dict_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, இலக்கம்_interp_env_t& env, cff2_font_dict_மதிப்புகள்_t& dictval)
  {
    தேர்வு (op) {
      நிலை OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_அடுக்கு.எடு_uint ();
	dictval.privateDictInfo.size = env.arg_அடுக்கு.எடு_uint ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ())
	  திரும்பு;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref);
  }

  தனிப்பட்ட:
  வகைவரை dict_opset_t SUPER;
};

template <typename VAL>
கட்டமைப்பு cff2_private_dict_மதிப்புகள்_base_t : dict_மதிப்புகள்_t<VAL>
{
  வெறுமை init ()
  {
    dict_மதிப்புகள்_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  வெறுமை fini () { dict_மதிப்புகள்_t<VAL>::fini (); }

  முழு                subrsOffset;
  மாறிலி CFF2Subrs   *localSubrs;
  unsigned முழு      ivs;
};

வகைவரை cff2_private_dict_மதிப்புகள்_base_t<op_str_t> cff2_private_dict_மதிப்புகள்_subset_t;
வகைவரை cff2_private_dict_மதிப்புகள்_base_t<இலக்கம்_dict_val_t> cff2_private_dict_மதிப்புகள்_t;

கட்டமைப்பு cff2_priv_dict_interp_env_t : இலக்கம்_interp_env_t
{
  cff2_priv_dict_interp_env_t (மாறிலி hb_ubytes_t &str) :
    இலக்கம்_interp_env_t (str) {}

  வெறுமை process_vsindex ()
  {
    எனில் (likely (!seen_vsindex))
    {
      set_ivs (arg_அடுக்கு.எடு_uint ());
    }
    seen_vsindex = உண்மை;
  }

  unsigned முழு get_ivs () மாறிலி { திரும்பு ivs; }
  வெறுமை	 set_ivs (unsigned முழு ivs_) { ivs = ivs_; }

  பாதுகாக்கப்பட்ட:
  unsigned முழு  ivs = 0;
  தருக்க	  seen_vsindex = பொய்;
};

கட்டமைப்பு cff2_private_dict_opset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_மதிப்புகள்_t& dictval)
  {
    இலக்கம்_dict_val_t val;
    val.init ();

    தேர்வு (op) {
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_ExpansionFactor:
      நிலை OpCode_LanguageGroup:
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_Subrs:
	dictval.subrsOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	நிறுத்து;
      நிலை OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	நிறுத்து;

      இயல்பு:
	dict_opset_t::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref, val);
  }
};

கட்டமைப்பு cff2_private_dict_opset_subset_t : dict_opset_t
{
  நிலையான வெறுமை process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_மதிப்புகள்_subset_t& dictval)
  {
    தேர்வு (op) {
      நிலை Op_Code_Blue_மதிப்புகள்:
      நிலை OpCode_OtherBlues:
      நிலை OpCode_FamilyBlues:
      நிலை OpCode_FamilyOtherBlues:
      நிலை OpCode_StdHW:
      நிலை OpCode_StdVW:
      நிலை OpCode_BlueScale:
      நிலை OpCode_BlueShift:
      நிலை OpCode_BlueFuzz:
      நிலை OpCode_StemSnapH:
      நிலை OpCode_StemSnapV:
      நிலை OpCode_LanguageGroup:
      நிலை OpCode_ExpansionFactor:
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_blenddict:
	env.clear_args ();
	திரும்பு;

      நிலை OpCode_Subrs:
	dictval.subrsOffset = env.arg_அடுக்கு.எடு_int ();
	env.clear_args ();
	நிறுத்து;

      நிலை OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	நிறுத்து;

      இயல்பு:
	SUPER::process_op (op, env);
	எனில் (!env.arg_அடுக்கு.is_empty ()) திரும்பு;
	நிறுத்து;
    }

    எனில் (unlikely (env.in_error ())) திரும்பு;

    dictval.add_op (op, env.str_ref);
  }

  தனிப்பட்ட:
  வகைவரை dict_opset_t SUPER;
};

வகைவரை dict_interpreter_t<cff2_உச்சி_dict_opset_t, cff2_உச்சி_dict_மதிப்புகள்_t> cff2_உச்சி_dict_interpreter_t;
வகைவரை dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_மதிப்புகள்_t> cff2_font_dict_interpreter_t;

கட்டமைப்பு CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  தருக்க serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { திரும்பு FDArray<HBUINT32>::serialize<cff2_font_dict_மதிப்புகள்_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

நாமாகாஶ OT {

using நாமாகாஶ CFF;

கட்டமைப்பு cff2
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  கட்டமைப்பு accelerator_templ_t
  {
    நிலையான constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      எனில் (!face) திரும்பு;

      உச்சி_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      இது->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (இது->blob);
      sc.start_processing ();

      மாறிலி OT::cff2 *cff2 = இது->blob->template as<OT::cff2> ();

      எனில் (cff2 == &Null (OT::cff2))
        செல் fail;

      { /* parse top dict */
	hb_ubytes_t உச்சி_Dict_Str = (cff2 + cff2->உச்சி_Dict_2).as_ubytes (cff2->உச்சி_Dict_Size);
	எனில் (unlikely (!உச்சி_Dict_Str.sanitize (&sc))) செல் fail;
	hb_barrier ();
	இலக்கம்_interp_env_t env (உச்சி_Dict_Str);
	cff2_உச்சி_dict_interpreter_t உச்சி_interp (env);
	உச்சி_Dict_2.init ();
	எனில் (unlikely (!உச்சி_interp.interpret (உச்சி_Dict_2))) செல் fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->உச்சி_Dict_2 + cff2->உச்சி_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, உச்சி_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, உச்சி_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, உச்சி_Dict_2.FDArrayOffset, sc);

      எனில் (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        செல் fail;

      இலக்கம்_glyphs = charStrings->மொத்தம்;
      எனில் (இலக்கம்_glyphs != sc.get_இலக்கம்_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        தருக்க valid_மொத்தம் = இலக்கம்_glyphs < sc.get_இலக்கம்_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        எனில் (!valid_மொத்தம்) செல் fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, உச்சி_Dict_2.FDSelectOffset, sc,
                                                   fdArray->மொத்தம், இலக்கம்_glyphs);
      எனில் (உச்சி_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        செல் fail;

      fd_மொத்தம் = fdArray->மொத்தம்;
      எனில் (!privateDicts.resize (fd_மொத்தம்))
        செல் fail;

      /* parse font dicts and gather private dicts */
      க்கு (unsigned முழு i = 0; i < fd_மொத்தம்; i++)
      {
	மாறிலி hb_ubytes_t fontDictStr = (*fdArray)[i];
	எனில் (unlikely (!fontDictStr.sanitize (&sc))) செல் fail;
	hb_barrier ();
	cff2_font_dict_மதிப்புகள்_t  *font;
	இலக்கம்_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	எனில் (unlikely (font == &Crap (cff2_font_dict_மதிப்புகள்_t))) செல் fail;
	font->init ();
	எனில் (unlikely (!font_interp.interpret (*font))) செல் fail;

	மாறிலி hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	எனில் (unlikely (font->privateDictInfo.size &&
		      privDictStr == (மாறிலி unsigned எழுத்து *) &Null (UnsizedByteStr))) செல் fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	எனில் (unlikely (!priv_interp.interpret (privateDicts[i]))) செல் fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      திரும்பு;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    வெறுமை _fini ()
    {
      sc.end_processing ();
      உச்சி_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      எனில் (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () மாறிலி
    {
      திரும்பு nullptr;
    }

    hb_blob_t *get_blob () மாறிலி { திரும்பு blob; }

    தருக்க is_valid () மாறிலி { திரும்பு blob; }

    பாதுகாக்கப்பட்ட:
    hb_sanitize_context_t	sc;

    பொது:
    hb_blob_t			*blob = nullptr;
    cff2_உச்சி_dict_மதிப்புகள்_t	உச்சி_Dict_2;
    மாறிலி CFF2Subrs		*globalSubrs = nullptr;
    மாறிலி CFF2ItemVariationStore	*varStore = nullptr;
    மாறிலி CFF2CharStrings	*charStrings = nullptr;
    மாறிலி CFF2FDArray		*fdArray = nullptr;
    மாறிலி CFF2FDSelect		*fdSelect = nullptr;
    unsigned முழு		fd_மொத்தம் = 0;

    hb_vector_t<cff2_font_dict_மதிப்புகள்_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<தசம> *> cached_scalars_vector;

    unsigned முழு	      இலக்கம்_glyphs = 0;
  };

  கட்டமைப்பு accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_மதிப்புகள்_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL தருக்க get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) மாறிலி;
    HB_INTERNAL தருக்க get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<மாறிலி முழு> coords,
				     int64_t *budget = nullptr) மாறிலி;
    HB_INTERNAL தருக்க get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) மாறிலி;
    HB_INTERNAL தருக்க get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<மாறிலி முழு> coords, int64_t *budget = nullptr) மாறிலி;
  };

  கட்டமைப்பு accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_மதிப்புகள்_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      எனில் (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL தருக்க subset (hb_subset_context_t *c) மாறிலி;
    HB_INTERNAL தருக்க serialize (hb_serialize_context_t *c,
				கட்டமைப்பு cff2_subset_plan &plan,
				hb_array_t<முழு> normalized_coords) மாறிலி;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    வகைவரை accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_மதிப்புகள்_subset_t> SUPER;
  };

  பொது:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<உச்சி_Dict, HBUINT8>	உச்சி_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			உச்சி_Dict_Size;	/* Top DICT size */

  பொது:
  DEFINE_SIZE_STATIC (5);
};

கட்டமைப்பு cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

கட்டமைப்பு cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
