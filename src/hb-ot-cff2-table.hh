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

#അന്തര്ഭാവ "hb-ot-cff-common.hh"
#അന്തര്ഭാവ "hb-subset-cff-common.hh"
#അന്തര്ഭാവ "hb-draw.hh"
#അന്തര്ഭാവ "hb-paint.hh"

നാമാകാശ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

തരനിർവചനം CFF2Index         CFF2CharStrings;
തരനിർവചനം Subrs<HBUINT32>   CFF2Subrs;

തരനിർവചനം FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
തരനിർവചനം FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ഘടന CFF2FDSelect
{
  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം CFF2FDSelect &src, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ size = src.get_size (അക്കം_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    എങ്കിൽ (unlikely (!dest)) return_trace (അസത്യം);
    hb_memcpy (dest, &src, size);
    return_trace (സത്യം);
  }

  size_t get_size (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ format.static_size + u.format0.get_size (അക്കം_glyphs);
    അവസ്ഥ 3: hb_barrier (); തിരികെ format.static_size + u.format3.get_size ();
    അവസ്ഥ 4: hb_barrier (); തിരികെ format.static_size + u.format4.get_size ();
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) സ്ഥിരം
  {
    എങ്കിൽ (ഇത് == &Null (CFF2FDSelect))
      തിരികെ 0;

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_fd (glyph);
    അവസ്ഥ 3: hb_barrier (); തിരികെ u.format3.get_fd (glyph);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_fd (glyph);
    സ്ഥിരസ്ഥിതി:തിരികെ 0;
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned പൂർണ്ണസംഖ്യ fdcount,
                 unsigned പൂർണ്ണസംഖ്യ charstring_എണ്ണം = HB_CODEPOINT_INVALID) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!c->check_struct (ഇത്)))
      return_trace (അസത്യം);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (format)
    {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_എണ്ണം));
    അവസ്ഥ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_എണ്ണം));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_എണ്ണം));
    സ്ഥിരസ്ഥിതി:return_trace (അസത്യം);
    }
  }

  HBUINT8	format;
  സംഘം {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  പൊതു:
  DEFINE_SIZE_MIN (2);
};

ഘടന CFF2ItemVariationStore
{
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  തർക്കം serialize (hb_serialize_context_t *c, സ്ഥിരം CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    എങ്കിൽ (unlikely (!dest)) return_trace (അസത്യം);
    hb_memcpy (dest, varStore, size_);
    return_trace (സത്യം);
  }

  size_t get_size () സ്ഥിരം { തിരികെ HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

ഘടന cff2_മുകൾ_dict_മൂല്യങ്ങൾ_t : മുകൾ_dict_മൂല്യങ്ങൾ_t<>
{
  ശൂന്യം init ()
  {
    മുകൾ_dict_മൂല്യങ്ങൾ_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  ശൂന്യം fini () { മുകൾ_dict_മൂല്യങ്ങൾ_t<>::fini (); }

  പൂർണ്ണസംഖ്യ  vstoreOffset;
  പൂർണ്ണസംഖ്യ  FDSelectOffset;
};

ഘടന cff2_മുകൾ_dict_opset_t : മുകൾ_dict_opset_t<>
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, അക്കം_interp_env_t& env, cff2_മുകൾ_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	നിർത്തുക;

      അവസ്ഥ OpCode_vstore:
	dictval.vstoreOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref);
  }

  തരനിർവചനം മുകൾ_dict_opset_t<> SUPER;
};

ഘടന cff2_font_dict_മൂല്യങ്ങൾ_t : dict_മൂല്യങ്ങൾ_t<op_str_t>
{
  ശൂന്യം init ()
  {
    dict_മൂല്യങ്ങൾ_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  ശൂന്യം fini () { dict_മൂല്യങ്ങൾ_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

ഘടന cff2_font_dict_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, അക്കം_interp_env_t& env, cff2_font_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_അട്ടി.എടുക്കുക_uint ();
	dictval.privateDictInfo.size = env.arg_അട്ടി.എടുക്കുക_uint ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ())
	  തിരികെ;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref);
  }

  സ്വകാര്യ:
  തരനിർവചനം dict_opset_t SUPER;
};

template <typename VAL>
ഘടന cff2_private_dict_മൂല്യങ്ങൾ_base_t : dict_മൂല്യങ്ങൾ_t<VAL>
{
  ശൂന്യം init ()
  {
    dict_മൂല്യങ്ങൾ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  ശൂന്യം fini () { dict_മൂല്യങ്ങൾ_t<VAL>::fini (); }

  പൂർണ്ണസംഖ്യ                subrsOffset;
  സ്ഥിരം CFF2Subrs   *localSubrs;
  unsigned പൂർണ്ണസംഖ്യ      ivs;
};

തരനിർവചനം cff2_private_dict_മൂല്യങ്ങൾ_base_t<op_str_t> cff2_private_dict_മൂല്യങ്ങൾ_subset_t;
തരനിർവചനം cff2_private_dict_മൂല്യങ്ങൾ_base_t<അക്കം_dict_val_t> cff2_private_dict_മൂല്യങ്ങൾ_t;

ഘടന cff2_priv_dict_interp_env_t : അക്കം_interp_env_t
{
  cff2_priv_dict_interp_env_t (സ്ഥിരം hb_ubytes_t &str) :
    അക്കം_interp_env_t (str) {}

  ശൂന്യം process_vsindex ()
  {
    എങ്കിൽ (likely (!seen_vsindex))
    {
      set_ivs (arg_അട്ടി.എടുക്കുക_uint ());
    }
    seen_vsindex = സത്യം;
  }

  unsigned പൂർണ്ണസംഖ്യ get_ivs () സ്ഥിരം { തിരികെ ivs; }
  ശൂന്യം	 set_ivs (unsigned പൂർണ്ണസംഖ്യ ivs_) { ivs = ivs_; }

  സംരക്ഷിത:
  unsigned പൂർണ്ണസംഖ്യ  ivs = 0;
  തർക്കം	  seen_vsindex = അസത്യം;
};

ഘടന cff2_private_dict_opset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_മൂല്യങ്ങൾ_t& dictval)
  {
    അക്കം_dict_val_t val;
    val.init ();

    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_ExpansionFactor:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_Subrs:
	dictval.subrsOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	നിർത്തുക;
      അവസ്ഥ OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	dict_opset_t::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ഘടന cff2_private_dict_opset_subset_t : dict_opset_t
{
  സ്ഥിതം ശൂന്യം process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_മൂല്യങ്ങൾ_subset_t& dictval)
  {
    തിരഞ്ഞെടുപ്പ് (op) {
      അവസ്ഥ Op_Code_Blue_മൂല്യങ്ങൾ:
      അവസ്ഥ OpCode_OtherBlues:
      അവസ്ഥ OpCode_FamilyBlues:
      അവസ്ഥ OpCode_FamilyOtherBlues:
      അവസ്ഥ OpCode_StdHW:
      അവസ്ഥ OpCode_StdVW:
      അവസ്ഥ OpCode_BlueScale:
      അവസ്ഥ OpCode_BlueShift:
      അവസ്ഥ OpCode_BlueFuzz:
      അവസ്ഥ OpCode_StemSnapH:
      അവസ്ഥ OpCode_StemSnapV:
      അവസ്ഥ OpCode_LanguageGroup:
      അവസ്ഥ OpCode_ExpansionFactor:
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_blenddict:
	env.clear_args ();
	തിരികെ;

      അവസ്ഥ OpCode_Subrs:
	dictval.subrsOffset = env.arg_അട്ടി.എടുക്കുക_int ();
	env.clear_args ();
	നിർത്തുക;

      അവസ്ഥ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	നിർത്തുക;

      സ്ഥിരസ്ഥിതി:
	SUPER::process_op (op, env);
	എങ്കിൽ (!env.arg_അട്ടി.is_empty ()) തിരികെ;
	നിർത്തുക;
    }

    എങ്കിൽ (unlikely (env.in_error ())) തിരികെ;

    dictval.add_op (op, env.str_ref);
  }

  സ്വകാര്യ:
  തരനിർവചനം dict_opset_t SUPER;
};

തരനിർവചനം dict_interpreter_t<cff2_മുകൾ_dict_opset_t, cff2_മുകൾ_dict_മൂല്യങ്ങൾ_t> cff2_മുകൾ_dict_interpreter_t;
തരനിർവചനം dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_മൂല്യങ്ങൾ_t> cff2_font_dict_interpreter_t;

ഘടന CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  തർക്കം serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { തിരികെ FDArray<HBUINT32>::serialize<cff2_font_dict_മൂല്യങ്ങൾ_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

നാമാകാശ OT {

using നാമാകാശ CFF;

ഘടന cff2
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ഘടന accelerator_templ_t
  {
    സ്ഥിതം constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      എങ്കിൽ (!face) തിരികെ;

      മുകൾ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ഇത്->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (ഇത്->blob);
      sc.start_processing ();

      സ്ഥിരം OT::cff2 *cff2 = ഇത്->blob->template as<OT::cff2> ();

      എങ്കിൽ (cff2 == &Null (OT::cff2))
        പോകുക fail;

      { /* parse top dict */
	hb_ubytes_t മുകൾ_Dict_Str = (cff2 + cff2->മുകൾ_Dict_2).as_ubytes (cff2->മുകൾ_Dict_Size);
	എങ്കിൽ (unlikely (!മുകൾ_Dict_Str.sanitize (&sc))) പോകുക fail;
	hb_barrier ();
	അക്കം_interp_env_t env (മുകൾ_Dict_Str);
	cff2_മുകൾ_dict_interpreter_t മുകൾ_interp (env);
	മുകൾ_Dict_2.init ();
	എങ്കിൽ (unlikely (!മുകൾ_interp.interpret (മുകൾ_Dict_2))) പോകുക fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->മുകൾ_Dict_2 + cff2->മുകൾ_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, മുകൾ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, മുകൾ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, മുകൾ_Dict_2.FDArrayOffset, sc);

      എങ്കിൽ (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        പോകുക fail;

      അക്കം_glyphs = charStrings->എണ്ണം;
      എങ്കിൽ (അക്കം_glyphs != sc.get_അക്കം_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        തർക്കം valid_എണ്ണം = അക്കം_glyphs < sc.get_അക്കം_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        എങ്കിൽ (!valid_എണ്ണം) പോകുക fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, മുകൾ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->എണ്ണം, അക്കം_glyphs);
      എങ്കിൽ (മുകൾ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        പോകുക fail;

      fd_എണ്ണം = fdArray->എണ്ണം;
      എങ്കിൽ (!privateDicts.resize (fd_എണ്ണം))
        പോകുക fail;

      /* parse font dicts and gather private dicts */
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < fd_എണ്ണം; i++)
      {
	സ്ഥിരം hb_ubytes_t fontDictStr = (*fdArray)[i];
	എങ്കിൽ (unlikely (!fontDictStr.sanitize (&sc))) പോകുക fail;
	hb_barrier ();
	cff2_font_dict_മൂല്യങ്ങൾ_t  *font;
	അക്കം_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	എങ്കിൽ (unlikely (font == &Crap (cff2_font_dict_മൂല്യങ്ങൾ_t))) പോകുക fail;
	font->init ();
	എങ്കിൽ (unlikely (!font_interp.interpret (*font))) പോകുക fail;

	സ്ഥിരം hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	എങ്കിൽ (unlikely (font->privateDictInfo.size &&
		      privDictStr == (സ്ഥിരം unsigned അക്ഷരം *) &Null (UnsizedByteStr))) പോകുക fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	എങ്കിൽ (unlikely (!priv_interp.interpret (privateDicts[i]))) പോകുക fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      തിരികെ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ശൂന്യം _fini ()
    {
      sc.end_processing ();
      മുകൾ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      എങ്കിൽ (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () സ്ഥിരം
    {
      തിരികെ nullptr;
    }

    hb_blob_t *get_blob () സ്ഥിരം { തിരികെ blob; }

    തർക്കം is_valid () സ്ഥിരം { തിരികെ blob; }

    സംരക്ഷിത:
    hb_sanitize_context_t	sc;

    പൊതു:
    hb_blob_t			*blob = nullptr;
    cff2_മുകൾ_dict_മൂല്യങ്ങൾ_t	മുകൾ_Dict_2;
    സ്ഥിരം CFF2Subrs		*globalSubrs = nullptr;
    സ്ഥിരം CFF2ItemVariationStore	*varStore = nullptr;
    സ്ഥിരം CFF2CharStrings	*charStrings = nullptr;
    സ്ഥിരം CFF2FDArray		*fdArray = nullptr;
    സ്ഥിരം CFF2FDSelect		*fdSelect = nullptr;
    unsigned പൂർണ്ണസംഖ്യ		fd_എണ്ണം = 0;

    hb_vector_t<cff2_font_dict_മൂല്യങ്ങൾ_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<ദശാംശം> *> cached_scalars_vector;

    unsigned പൂർണ്ണസംഖ്യ	      അക്കം_glyphs = 0;
  };

  ഘടന accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_മൂല്യങ്ങൾ_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL തർക്കം get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) സ്ഥിരം;
    HB_INTERNAL തർക്കം get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords,
				     int64_t *budget = nullptr) സ്ഥിരം;
    HB_INTERNAL തർക്കം get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) സ്ഥിരം;
    HB_INTERNAL തർക്കം get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords, int64_t *budget = nullptr) സ്ഥിരം;
  };

  ഘടന accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_മൂല്യങ്ങൾ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      എങ്കിൽ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം;
    HB_INTERNAL തർക്കം serialize (hb_serialize_context_t *c,
				ഘടന cff2_subset_plan &plan,
				hb_array_t<പൂർണ്ണസംഖ്യ> normalized_coords) സ്ഥിരം;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    തരനിർവചനം accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_മൂല്യങ്ങൾ_subset_t> SUPER;
  };

  പൊതു:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<മുകൾ_Dict, HBUINT8>	മുകൾ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			മുകൾ_Dict_Size;	/* Top DICT size */

  പൊതു:
  DEFINE_SIZE_STATIC (5);
};

ഘടന cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ഘടന cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
