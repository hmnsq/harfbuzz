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

प्रकारव्याख्या CFF2Index         CFF2CharStrings;
प्रकारव्याख्या Subrs<HBUINT32>   CFF2Subrs;

प्रकारव्याख्या FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
प्रकारव्याख्या FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

संरचना CFF2FDSelect
{
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक CFF2FDSelect &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित पूर्णांक size = src.get_size (अंक_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    जर (unlikely (!dest)) return_trace (असत्य);
    hb_memcpy (dest, &src, size);
    return_trace (सत्य);
  }

  माप_प्रकार get_size (अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत format.static_size + u.format0.get_size (अंक_glyphs);
    स्थिती 3: hb_barrier (); परत format.static_size + u.format3.get_size ();
    स्थिती 4: hb_barrier (); परत format.static_size + u.format4.get_size ();
    पूर्वनिर्धारित:परत 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) स्थिरांक
  {
    जर (हे == &Null (CFF2FDSelect))
      परत 0;

    निवड (format)
    {
    स्थिती 0: hb_barrier (); परत u.format0.get_fd (glyph);
    स्थिती 3: hb_barrier (); परत u.format3.get_fd (glyph);
    स्थिती 4: hb_barrier (); परत u.format4.get_fd (glyph);
    पूर्वनिर्धारित:परत 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक charstring_मोजणी = HB_CODEPOINT_INVALID) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे)))
      return_trace (असत्य);
    hb_barrier ();

    निवड (format)
    {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_मोजणी));
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_मोजणी));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_मोजणी));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  HBUINT8	format;
  संघ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (2);
};

संरचना CFF2ItemVariationStore
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित पूर्णांक size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    जर (unlikely (!dest)) return_trace (असत्य);
    hb_memcpy (dest, varStore, size_);
    return_trace (सत्य);
  }

  माप_प्रकार get_size () स्थिरांक { परत HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

संरचना cff2_शीर्ष_dict_मूल्ये_t : शीर्ष_dict_मूल्ये_t<>
{
  शून्य init ()
  {
    शीर्ष_dict_मूल्ये_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  शून्य fini () { शीर्ष_dict_मूल्ये_t<>::fini (); }

  पूर्णांक  vstoreOffset;
  पूर्णांक  FDSelectOffset;
};

संरचना cff2_शीर्ष_dict_opset_t : शीर्ष_dict_opset_t<>
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff2_शीर्ष_dict_मूल्ये_t& dictval)
  {
    निवड (op) {
      स्थिती OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	थांबा;

      स्थिती OpCode_vstore:
	dictval.vstoreOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	जर (!env.arg_चळत.is_empty ()) परत;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref);
  }

  प्रकारव्याख्या शीर्ष_dict_opset_t<> SUPER;
};

संरचना cff2_font_dict_मूल्ये_t : dict_मूल्ये_t<op_str_t>
{
  शून्य init ()
  {
    dict_मूल्ये_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  शून्य fini () { dict_मूल्ये_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

संरचना cff2_font_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, अंक_interp_env_t& env, cff2_font_dict_मूल्ये_t& dictval)
  {
    निवड (op) {
      स्थिती OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_चळत.काढा_uint ();
	dictval.privateDictInfo.size = env.arg_चळत.काढा_uint ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env);
	जर (!env.arg_चळत.is_empty ())
	  परत;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref);
  }

  खाजगी:
  प्रकारव्याख्या dict_opset_t SUPER;
};

ढाँचा <typename VAL>
संरचना cff2_private_dict_मूल्ये_base_t : dict_मूल्ये_t<VAL>
{
  शून्य init ()
  {
    dict_मूल्ये_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  शून्य fini () { dict_मूल्ये_t<VAL>::fini (); }

  पूर्णांक                subrsOffset;
  स्थिरांक CFF2Subrs   *localSubrs;
  अचिन्हित पूर्णांक      ivs;
};

प्रकारव्याख्या cff2_private_dict_मूल्ये_base_t<op_str_t> cff2_private_dict_मूल्ये_subset_t;
प्रकारव्याख्या cff2_private_dict_मूल्ये_base_t<अंक_dict_val_t> cff2_private_dict_मूल्ये_t;

संरचना cff2_priv_dict_interp_env_t : अंक_interp_env_t
{
  cff2_priv_dict_interp_env_t (स्थिरांक hb_ubytes_t &str) :
    अंक_interp_env_t (str) {}

  शून्य process_vsindex ()
  {
    जर (likely (!seen_vsindex))
    {
      set_ivs (arg_चळत.काढा_uint ());
    }
    seen_vsindex = सत्य;
  }

  अचिन्हित पूर्णांक get_ivs () स्थिरांक { परत ivs; }
  शून्य	 set_ivs (अचिन्हित पूर्णांक ivs_) { ivs = ivs_; }

  संरक्षित:
  अचिन्हित पूर्णांक  ivs = 0;
  तार्किक	  seen_vsindex = असत्य;
};

संरचना cff2_private_dict_opset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मूल्ये_t& dictval)
  {
    अंक_dict_val_t val;
    val.init ();

    निवड (op) {
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_ExpansionFactor:
      स्थिती OpCode_LanguageGroup:
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
	env.clear_args ();
	थांबा;
      स्थिती OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	थांबा;
      स्थिती OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	dict_opset_t::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref, val);
  }
};

संरचना cff2_private_dict_opset_subset_t : dict_opset_t
{
  स्थिर शून्य process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मूल्ये_subset_t& dictval)
  {
    निवड (op) {
      स्थिती Op_Code_Blue_मूल्ये:
      स्थिती OpCode_OtherBlues:
      स्थिती OpCode_FamilyBlues:
      स्थिती OpCode_FamilyOtherBlues:
      स्थिती OpCode_StdHW:
      स्थिती OpCode_StdVW:
      स्थिती OpCode_BlueScale:
      स्थिती OpCode_BlueShift:
      स्थिती OpCode_BlueFuzz:
      स्थिती OpCode_StemSnapH:
      स्थिती OpCode_StemSnapV:
      स्थिती OpCode_LanguageGroup:
      स्थिती OpCode_ExpansionFactor:
	env.clear_args ();
	थांबा;

      स्थिती OpCode_blenddict:
	env.clear_args ();
	परत;

      स्थिती OpCode_Subrs:
	dictval.subrsOffset = env.arg_चळत.काढा_int ();
	env.clear_args ();
	थांबा;

      स्थिती OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	थांबा;

      पूर्वनिर्धारित:
	SUPER::process_op (op, env);
	जर (!env.arg_चळत.is_empty ()) परत;
	थांबा;
    }

    जर (unlikely (env.in_error ())) परत;

    dictval.add_op (op, env.str_ref);
  }

  खाजगी:
  प्रकारव्याख्या dict_opset_t SUPER;
};

प्रकारव्याख्या dict_interpreter_t<cff2_शीर्ष_dict_opset_t, cff2_शीर्ष_dict_मूल्ये_t> cff2_शीर्ष_dict_interpreter_t;
प्रकारव्याख्या dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_मूल्ये_t> cff2_font_dict_interpreter_t;

संरचना CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  ढाँचा <typename ITER, typename OP_SERIALIZER>
  तार्किक serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { परत FDArray<HBUINT32>::serialize<cff2_font_dict_मूल्ये_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

नामाकाश OT {

using नामाकाश CFF;

संरचना cff2
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  ढाँचा <typename PRIVOPSET, typename PRIVDICTVAL>
  संरचना accelerator_templ_t
  {
    स्थिर constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      जर (!face) परत;

      शीर्ष_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      हे->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (हे->blob);
      sc.start_processing ();

      स्थिरांक OT::cff2 *cff2 = हे->blob->ढाँचा as<OT::cff2> ();

      जर (cff2 == &Null (OT::cff2))
        जा fail;

      { /* parse top dict */
	hb_ubytes_t शीर्ष_Dict_Str = (cff2 + cff2->शीर्ष_Dict_2).as_ubytes (cff2->शीर्ष_Dict_Size);
	जर (unlikely (!शीर्ष_Dict_Str.sanitize (&sc))) जा fail;
	hb_barrier ();
	अंक_interp_env_t env (शीर्ष_Dict_Str);
	cff2_शीर्ष_dict_interpreter_t शीर्ष_interp (env);
	शीर्ष_Dict_2.init ();
	जर (unlikely (!शीर्ष_interp.interpret (शीर्ष_Dict_2))) जा fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->शीर्ष_Dict_2 + cff2->शीर्ष_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, शीर्ष_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, शीर्ष_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, शीर्ष_Dict_2.FDArrayOffset, sc);

      जर (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        जा fail;

      अंक_glyphs = charStrings->मोजणी;
      जर (अंक_glyphs != sc.get_अंक_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        तार्किक valid_मोजणी = अंक_glyphs < sc.get_अंक_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        जर (!valid_मोजणी) जा fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, शीर्ष_Dict_2.FDSelectOffset, sc,
                                                   fdArray->मोजणी, अंक_glyphs);
      जर (शीर्ष_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        जा fail;

      fd_मोजणी = fdArray->मोजणी;
      जर (!privateDicts.resize (fd_मोजणी))
        जा fail;

      /* parse font dicts and gather private dicts */
      साठी (अचिन्हित पूर्णांक i = 0; i < fd_मोजणी; i++)
      {
	स्थिरांक hb_ubytes_t fontDictStr = (*fdArray)[i];
	जर (unlikely (!fontDictStr.sanitize (&sc))) जा fail;
	hb_barrier ();
	cff2_font_dict_मूल्ये_t  *font;
	अंक_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	जर (unlikely (font == &Crap (cff2_font_dict_मूल्ये_t))) जा fail;
	font->init ();
	जर (unlikely (!font_interp.interpret (*font))) जा fail;

	स्थिरांक hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	जर (unlikely (font->privateDictInfo.size &&
		      privDictStr == (स्थिरांक अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जा fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	जर (unlikely (!priv_interp.interpret (privateDicts[i]))) जा fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      परत;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    शून्य _fini ()
    {
      sc.end_processing ();
      शीर्ष_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      स्वतः *scalars = cached_scalars_vector.get_acquire ();
      जर (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () स्थिरांक
    {
      परत nullptr;
    }

    hb_blob_t *get_blob () स्थिरांक { परत blob; }

    तार्किक is_valid () स्थिरांक { परत blob; }

    संरक्षित:
    hb_sanitize_context_t	sc;

    सार्वजनिक:
    hb_blob_t			*blob = nullptr;
    cff2_शीर्ष_dict_मूल्ये_t	शीर्ष_Dict_2;
    स्थिरांक CFF2Subrs		*globalSubrs = nullptr;
    स्थिरांक CFF2ItemVariationStore	*varStore = nullptr;
    स्थिरांक CFF2CharStrings	*charStrings = nullptr;
    स्थिरांक CFF2FDArray		*fdArray = nullptr;
    स्थिरांक CFF2FDSelect		*fdSelect = nullptr;
    अचिन्हित पूर्णांक		fd_मोजणी = 0;

    hb_vector_t<cff2_font_dict_मूल्ये_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<दशांश> *> cached_scalars_vector;

    अचिन्हित पूर्णांक	      अंक_glyphs = 0;
  };

  संरचना accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_मूल्ये_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL तार्किक get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) स्थिरांक;
    HB_INTERNAL तार्किक get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<स्थिरांक पूर्णांक> coords,
				     int64_t *budget = nullptr) स्थिरांक;
    HB_INTERNAL तार्किक get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) स्थिरांक;
    HB_INTERNAL तार्किक get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<स्थिरांक पूर्णांक> coords, int64_t *budget = nullptr) स्थिरांक;
  };

  संरचना accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मूल्ये_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      जर (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL तार्किक subset (hb_subset_context_t *c) स्थिरांक;
    HB_INTERNAL तार्किक serialize (hb_serialize_context_t *c,
				संरचना cff2_subset_plan &plan,
				hb_array_t<पूर्णांक> normalized_coords) स्थिरांक;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकारव्याख्या accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मूल्ये_subset_t> SUPER;
  };

  सार्वजनिक:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<शीर्ष_Dict, HBUINT8>	शीर्ष_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			शीर्ष_Dict_Size;	/* Top DICT size */

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
