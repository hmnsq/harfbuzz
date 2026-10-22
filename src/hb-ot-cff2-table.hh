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

#अगर_अघोषित HB_OT_CFF2_TABLE_HH
#घोषणा HB_OT_CFF2_TABLE_HH

#समावेश "hb-ot-cff-common.hh"
#समावेश "hb-subset-cff-common.hh"
#समावेश "hb-draw.hh"
#समावेश "hb-paint.hh"

namespace CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#घोषणा HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

प्रकार CFF2_सूचकांक         CFF2CharStrings;
प्रकार Subrs<HBUINT32>   CFF2Subrs;

प्रकार FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
प्रकार FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

काष्ठा CFF2FDSelect
{
  bool serialize (hb_serialize_context_t *c, स्थिर CFF2FDSelect &src, अचिन्हित पूर्णांक अंक_glyphs)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित पूर्णांक आकार = src.get_आकार (अंक_glyphs);
    CFF2FDSelect *dest = c->allocate_आकार<CFF2FDSelect> (आकार);
    अगर (unlikely (!dest)) return_trace (false);
    hb_memcpy (dest, &src, आकार);
    return_trace (true);
  }

  माप_प्रकार get_आकार (अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (format)
    {
    हाल 0: hb_barrier (); वापस format.static_आकार + u.format0.get_आकार (अंक_glyphs);
    हाल 3: hb_barrier (); वापस format.static_आकार + u.format3.get_आकार ();
    हाल 4: hb_barrier (); वापस format.static_आकार + u.format4.get_आकार ();
    शेष:वापस 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) स्थिर
  {
    अगर (यह == &Null (CFF2FDSelect))
      वापस 0;

    चयन (format)
    {
    हाल 0: hb_barrier (); वापस u.format0.get_fd (glyph);
    हाल 3: hb_barrier (); वापस u.format3.get_fd (glyph);
    हाल 4: hb_barrier (); वापस u.format4.get_fd (glyph);
    शेष:वापस 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c, अचिन्हित पूर्णांक fdcount,
                 अचिन्हित पूर्णांक charstring_गिनती = HB_CODEPOINT_INVALID) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह)))
      return_trace (false);
    hb_barrier ();

    चयन (format)
    {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_गिनती));
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_गिनती));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_गिनती));
    शेष:return_trace (false);
    }
  }

  HBUINT8	format;
  जोड़ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  खुला:
  DEFINE_आकार_MIN (2);
};

काष्ठा CFF2ItemVariationStore
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  c->check_range (&varStore, आकार) &&
		  varStore.sanitize (c));
  }

  bool serialize (hb_serialize_context_t *c, स्थिर CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित पूर्णांक आकार_3 = varStore->get_आकार ();
    CFF2ItemVariationStore *dest = c->allocate_आकार<CFF2ItemVariationStore> (आकार_3);
    अगर (unlikely (!dest)) return_trace (false);
    hb_memcpy (dest, varStore, आकार_3);
    return_trace (true);
  }

  माप_प्रकार get_आकार () स्थिर { वापस HBUINT16::static_आकार + आकार; }

  HBUINT16	आकार;
  ItemVariationStore  varStore;

  DEFINE_आकार_MIN (2 + ItemVariationStore::min_आकार);
};

काष्ठा cff2_शीर्ष_dict_मूल्यावली_t : शीर्ष_dict_मूल्यावली_t<>
{
  व्योम init ()
  {
    शीर्ष_dict_मूल्यावली_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  व्योम fini () { शीर्ष_dict_मूल्यावली_t<>::fini (); }

  पूर्णांक  vstoreOffset;
  पूर्णांक  FDSelectOffset;
};

काष्ठा cff2_शीर्ष_dict_opset_t : शीर्ष_dict_opset_t<>
{
  अटल व्योम process_op (op_code_t op, अंक_interp_env_t& env, cff2_शीर्ष_dict_मूल्यावली_t& dictval)
  {
    चयन (op) {
      हाल OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	अवरोध;

      हाल OpCode_vstore:
	dictval.vstoreOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;

      शेष:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	अगर (!env.arg_ढेर.is_empty ()) वापस;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref);
  }

  प्रकार शीर्ष_dict_opset_t<> SUPER;
};

काष्ठा cff2_font_dict_मूल्यावली_t : dict_मूल्यावली_t<op_str_t>
{
  व्योम init ()
  {
    dict_मूल्यावली_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  व्योम fini () { dict_मूल्यावली_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

काष्ठा cff2_font_dict_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, अंक_interp_env_t& env, cff2_font_dict_मूल्यावली_t& dictval)
  {
    चयन (op) {
      हाल OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ढेर.निकालो_uint ();
	dictval.privateDictInfo.आकार = env.arg_ढेर.निकालो_uint ();
	env.clear_args ();
	अवरोध;

      शेष:
	SUPER::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ())
	  वापस;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref);
  }

  निजी:
  प्रकार dict_opset_t SUPER;
};

ढाँचा <typename VAL>
काष्ठा cff2_private_dict_मूल्यावली_base_t : dict_मूल्यावली_t<VAL>
{
  व्योम init ()
  {
    dict_मूल्यावली_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  व्योम fini () { dict_मूल्यावली_t<VAL>::fini (); }

  पूर्णांक                subrsOffset;
  स्थिर CFF2Subrs   *localSubrs;
  अचिन्हित पूर्णांक      ivs;
};

प्रकार cff2_private_dict_मूल्यावली_base_t<op_str_t> cff2_private_dict_मूल्यावली_subset_t;
प्रकार cff2_private_dict_मूल्यावली_base_t<अंक_dict_val_t> cff2_private_dict_मूल्यावली_t;

काष्ठा cff2_priv_dict_interp_env_t : अंक_interp_env_t
{
  cff2_priv_dict_interp_env_t (स्थिर hb_ubytes_t &str) :
    अंक_interp_env_t (str) {}

  व्योम process_vsindex ()
  {
    अगर (likely (!seen_vsindex))
    {
      set_ivs (arg_ढेर.निकालो_uint ());
    }
    seen_vsindex = true;
  }

  अचिन्हित पूर्णांक get_ivs () स्थिर { वापस ivs; }
  व्योम	 set_ivs (अचिन्हित पूर्णांक ivs_) { ivs = ivs_; }

  रक्षित:
  अचिन्हित पूर्णांक  ivs = 0;
  bool	  seen_vsindex = false;
};

काष्ठा cff2_private_dict_opset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मूल्यावली_t& dictval)
  {
    अंक_dict_val_t val;
    val.init ();

    चयन (op) {
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_ExpansionFactor:
      हाल OpCode_LanguageGroup:
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
	env.clear_args ();
	अवरोध;
      हाल OpCode_Subrs:
	dictval.subrsOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	अवरोध;
      हाल OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	अवरोध;

      शेष:
	dict_opset_t::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref, val);
  }
};

काष्ठा cff2_private_dict_opset_subset_t : dict_opset_t
{
  अटल व्योम process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_मूल्यावली_subset_t& dictval)
  {
    चयन (op) {
      हाल Op_Code_Blue_मूल्यावली:
      हाल OpCode_OtherBlues:
      हाल OpCode_FamilyBlues:
      हाल OpCode_FamilyOtherBlues:
      हाल OpCode_StdHW:
      हाल OpCode_StdVW:
      हाल OpCode_BlueScale:
      हाल OpCode_BlueShift:
      हाल OpCode_BlueFuzz:
      हाल OpCode_StemSnapH:
      हाल OpCode_StemSnapV:
      हाल OpCode_LanguageGroup:
      हाल OpCode_ExpansionFactor:
	env.clear_args ();
	अवरोध;

      हाल OpCode_blenddict:
	env.clear_args ();
	वापस;

      हाल OpCode_Subrs:
	dictval.subrsOffset = env.arg_ढेर.निकालो_int ();
	env.clear_args ();
	अवरोध;

      हाल OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	अवरोध;

      शेष:
	SUPER::process_op (op, env);
	अगर (!env.arg_ढेर.is_empty ()) वापस;
	अवरोध;
    }

    अगर (unlikely (env.in_error ())) वापस;

    dictval.add_op (op, env.str_ref);
  }

  निजी:
  प्रकार dict_opset_t SUPER;
};

प्रकार dict_interpreter_t<cff2_शीर्ष_dict_opset_t, cff2_शीर्ष_dict_मूल्यावली_t> cff2_शीर्ष_dict_interpreter_t;
प्रकार dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_मूल्यावली_t> cff2_font_dict_interpreter_t;

काष्ठा CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  ढाँचा <typename ITER, typename OP_SERIALIZER>
  bool serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { वापस FDArray<HBUINT32>::serialize<cff2_font_dict_मूल्यावली_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

namespace OT {

using namespace CFF;

काष्ठा cff2
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  ढाँचा <typename PRIVOPSET, typename PRIVDICTVAL>
  काष्ठा accelerator_templ_t
  {
    अटल constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      अगर (!face) वापस;

      शीर्ष_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      यह->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (यह->blob);
      sc.start_processing ();

      स्थिर OT::cff2 *cff2 = यह->blob->ढाँचा as<OT::cff2> ();

      अगर (cff2 == &Null (OT::cff2))
        जाओ हार;

      { /* parse top dict */
	hb_ubytes_t शीर्ष_Dict_Str = (cff2 + cff2->शीर्ष_Dict_2).as_ubytes (cff2->शीर्ष_Dict_आकार);
	अगर (unlikely (!शीर्ष_Dict_Str.sanitize (&sc))) जाओ हार;
	hb_barrier ();
	अंक_interp_env_t env (शीर्ष_Dict_Str);
	cff2_शीर्ष_dict_interpreter_t शीर्ष_interp (env);
	शीर्ष_Dict_2.init ();
	अगर (unlikely (!शीर्ष_interp.interpret (शीर्ष_Dict_2))) जाओ हार;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->शीर्ष_Dict_2 + cff2->शीर्ष_Dict_आकार, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, शीर्ष_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, शीर्ष_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, शीर्ष_Dict_2.FDArrayOffset, sc);

      अगर (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        जाओ हार;

      अंक_glyphs = charStrings->गिनती;
      अगर (अंक_glyphs != sc.get_अंक_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        bool valid_गिनती = अंक_glyphs < sc.get_अंक_glyphs () &&
                           hb_blob_get_लंबाई (varc);
        hb_blob_destroy (varc);
        अगर (!valid_गिनती) जाओ हार;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, शीर्ष_Dict_2.FDSelectOffset, sc,
                                                   fdArray->गिनती, अंक_glyphs);
      अगर (शीर्ष_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        जाओ हार;

      fd_गिनती = fdArray->गिनती;
      अगर (!privateDicts.resize (fd_गिनती))
        जाओ हार;

      /* parse font dicts and gather private dicts */
      क्रम (अचिन्हित पूर्णांक i = 0; i < fd_गिनती; i++)
      {
	स्थिर hb_ubytes_t fontDictStr = (*fdArray)[i];
	अगर (unlikely (!fontDictStr.sanitize (&sc))) जाओ हार;
	hb_barrier ();
	cff2_font_dict_मूल्यावली_t  *font;
	अंक_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	अगर (unlikely (font == &Crap (cff2_font_dict_मूल्यावली_t))) जाओ हार;
	font->init ();
	अगर (unlikely (!font_interp.interpret (*font))) जाओ हार;

	स्थिर hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.आकार).as_ubytes (font->privateDictInfo.आकार);
	अगर (unlikely (font->privateDictInfo.आकार &&
		      privDictStr == (स्थिर अचिन्हित अक्षर *) &Null (UnsizedByteStr))) जाओ हार;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	अगर (unlikely (!priv_interp.interpret (privateDicts[i]))) जाओ हार;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      वापस;

      हार:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    व्योम _fini ()
    {
      sc.end_processing ();
      शीर्ष_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      स्वतः *scalars = cached_scalars_vector.get_acquire ();
      अगर (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () स्थिर
    {
      वापस nullptr;
    }

    hb_blob_t *get_blob () स्थिर { वापस blob; }

    bool is_valid () स्थिर { वापस blob; }

    रक्षित:
    hb_sanitize_context_t	sc;

    खुला:
    hb_blob_t			*blob = nullptr;
    cff2_शीर्ष_dict_मूल्यावली_t	शीर्ष_Dict_2;
    स्थिर CFF2Subrs		*globalSubrs = nullptr;
    स्थिर CFF2ItemVariationStore	*varStore = nullptr;
    स्थिर CFF2CharStrings	*charStrings = nullptr;
    स्थिर CFF2FDArray		*fdArray = nullptr;
    स्थिर CFF2FDSelect		*fdSelect = nullptr;
    अचिन्हित पूर्णांक		fd_गिनती = 0;

    hb_vector_t<cff2_font_dict_मूल्यावली_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<भग्न> *> cached_scalars_vector;

    अचिन्हित पूर्णांक	      अंक_glyphs = 0;
  };

  काष्ठा accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_मूल्यावली_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL bool get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) स्थिर;
    HB_INTERNAL bool get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<स्थिर पूर्णांक> coords,
				     int64_t *budget = nullptr) स्थिर;
    HB_INTERNAL bool get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) स्थिर;
    HB_INTERNAL bool get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<स्थिर पूर्णांक> coords, int64_t *budget = nullptr) स्थिर;
  };

  काष्ठा accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मूल्यावली_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      अगर (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL bool subset (hb_subset_context_t *c) स्थिर;
    HB_INTERNAL bool serialize (hb_serialize_context_t *c,
				काष्ठा cff2_subset_plan &plan,
				hb_array_t<पूर्णांक> normalized_coords) स्थिर;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    प्रकार accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_मूल्यावली_subset_t> SUPER;
  };

  खुला:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<शीर्ष_Dict, HBUINT8>	शीर्ष_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			शीर्ष_Dict_आकार;	/* Top DICT size */

  खुला:
  DEFINE_आकार_STATIC (5);
};

काष्ठा cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

काष्ठा cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#पूर्ण_अगर /* HB_OT_CFF2_TABLE_HH */
