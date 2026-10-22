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

#ଅନ୍ତର୍ଭାଵ "hb-ot-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-cff-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-draw.hh"
#ଅନ୍ତର୍ଭାଵ "hb-paint.hh"

ନାମାକାଶ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

ପ୍ରକାରବ୍ୟାଖ୍ୟା CFF2Index         CFF2CharStrings;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Subrs<HBUINT32>   CFF2Subrs;

ପ୍ରକାରବ୍ୟାଖ୍ୟା FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
ପ୍ରକାରବ୍ୟାଖ୍ୟା FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ସଂରଚନା CFF2FDSelect
{
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ CFF2FDSelect &src, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = src.get_size (ଅଙ୍କ_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    ଯଦି (unlikely (!dest)) return_trace (ମିଥ୍ୟା);
    hb_memcpy (dest, &src, size);
    return_trace (ସତ୍ୟ);
  }

  size_t get_size (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ format.static_size + u.format0.get_size (ଅଙ୍କ_glyphs);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ format.static_size + u.format3.get_size ();
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ format.static_size + u.format4.get_size ();
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ଅଚଳ
  {
    ଯଦି (ଏହି == &Null (CFF2FDSelect))
      ଫେରନ୍ତୁ 0;

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_fd (glyph);
    ସ୍ଥିତି 3: hb_barrier (); ଫେରନ୍ତୁ u.format3.get_fd (glyph);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_fd (glyph);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdcount,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା charstring_ଗଣନା = HB_CODEPOINT_INVALID) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!c->check_struct (ଏହି)))
      return_trace (ମିଥ୍ୟା);
    hb_barrier ();

    ବାଛ (format)
    {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_ଗଣନା));
    ସ୍ଥିତି 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_ଗଣନା));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_ଗଣନା));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ମିଥ୍ୟା);
    }
  }

  HBUINT8	format;
  ସଂଘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (2);
};

ସଂରଚନା CFF2ItemVariationStore
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  ତାର୍କିକ serialize (hb_serialize_context_t *c, ଅଚଳ CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    ଯଦି (unlikely (!dest)) return_trace (ମିଥ୍ୟା);
    hb_memcpy (dest, varStore, size_);
    return_trace (ସତ୍ୟ);
  }

  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

ସଂରଚନା cff2_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<>
{
  ଶୂନ୍ୟ init ()
  {
    ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  ଶୂନ୍ୟ fini () { ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<>::fini (); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା  vstoreOffset;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା  FDSelectOffset;
};

ସଂରଚନା cff2_ଶୀର୍ଷ_dict_opset_t : ଶୀର୍ଷ_dict_opset_t<>
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ଅଙ୍କ_interp_env_t& env, cff2_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_vstore:
	dictval.vstoreOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref);
  }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୀର୍ଷ_dict_opset_t<> SUPER;
};

ସଂରଚନା cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t : dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>
{
  ଶୂନ୍ୟ init ()
  {
    dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  ଶୂନ୍ୟ fini () { dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

ସଂରଚନା cff2_font_dict_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, ଅଙ୍କ_interp_env_t& env, cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ଗଦା.କାଢ଼_uint ();
	dictval.privateDictInfo.size = env.arg_ଗଦା.କାଢ଼_uint ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ())
	  ଫେରନ୍ତୁ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref);
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_opset_t SUPER;
};

template <typename VAL>
ସଂରଚନା cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t : dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>
{
  ଶୂନ୍ୟ init ()
  {
    dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  ଶୂନ୍ୟ fini () { dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t<VAL>::fini (); }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା                subrsOffset;
  ଅଚଳ CFF2Subrs   *localSubrs;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା      ivs;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t<op_str_t> cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_base_t<ଅଙ୍କ_dict_val_t> cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t;

ସଂରଚନା cff2_priv_dict_interp_env_t : ଅଙ୍କ_interp_env_t
{
  cff2_priv_dict_interp_env_t (ଅଚଳ hb_ubytes_t &str) :
    ଅଙ୍କ_interp_env_t (str) {}

  ଶୂନ୍ୟ process_vsindex ()
  {
    ଯଦି (likely (!seen_vsindex))
    {
      set_ivs (arg_ଗଦା.କାଢ଼_uint ());
    }
    seen_vsindex = ସତ୍ୟ;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_ivs () ଅଚଳ { ଫେରନ୍ତୁ ivs; }
  ଶୂନ୍ୟ	 set_ivs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ivs_) { ivs = ivs_; }

  ସୁରକ୍ଷିତ:
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ivs = 0;
  ତାର୍କିକ	  seen_vsindex = ମିଥ୍ୟା;
};

ସଂରଚନା cff2_private_dict_opset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t& dictval)
  {
    ଅଙ୍କ_dict_val_t val;
    val.init ();

    ବାଛ (op) {
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_ExpansionFactor:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_Subrs:
	dictval.subrsOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ଭାଙ୍ଗ;
      ସ୍ଥିତି OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	dict_opset_t::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ସଂରଚନା cff2_private_dict_opset_subset_t : dict_opset_t
{
  ସ୍ଥିର ଶୂନ୍ୟ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t& dictval)
  {
    ବାଛ (op) {
      ସ୍ଥିତି Op_Code_Blue_ମୂଲ୍ୟଗୁଡ଼ିକ:
      ସ୍ଥିତି OpCode_OtherBlues:
      ସ୍ଥିତି OpCode_FamilyBlues:
      ସ୍ଥିତି OpCode_FamilyOtherBlues:
      ସ୍ଥିତି OpCode_StdHW:
      ସ୍ଥିତି OpCode_StdVW:
      ସ୍ଥିତି OpCode_BlueScale:
      ସ୍ଥିତି OpCode_BlueShift:
      ସ୍ଥିତି OpCode_BlueFuzz:
      ସ୍ଥିତି OpCode_StemSnapH:
      ସ୍ଥିତି OpCode_StemSnapV:
      ସ୍ଥିତି OpCode_LanguageGroup:
      ସ୍ଥିତି OpCode_ExpansionFactor:
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_blenddict:
	env.clear_args ();
	ଫେରନ୍ତୁ;

      ସ୍ଥିତି OpCode_Subrs:
	dictval.subrsOffset = env.arg_ଗଦା.କାଢ଼_int ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ସ୍ଥିତି OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ଭାଙ୍ଗ;

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
	SUPER::process_op (op, env);
	ଯଦି (!env.arg_ଗଦା.is_empty ()) ଫେରନ୍ତୁ;
	ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (env.in_error ())) ଫେରନ୍ତୁ;

    dictval.add_op (op, env.str_ref);
  }

  ବ୍ୟକ୍ତିଗତ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_opset_t SUPER;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_interpreter_t<cff2_ଶୀର୍ଷ_dict_opset_t, cff2_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t> cff2_ଶୀର୍ଷ_dict_interpreter_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t> cff2_font_dict_interpreter_t;

ସଂରଚନା CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  ତାର୍କିକ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ଫେରନ୍ତୁ FDArray<HBUINT32>::serialize<cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

ନାମାକାଶ OT {

using ନାମାକାଶ CFF;

ସଂରଚନା cff2
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ସଂରଚନା accelerator_templ_t
  {
    ସ୍ଥିର constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      ଯଦି (!face) ଫେରନ୍ତୁ;

      ଶୀର୍ଷ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ଏହି->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (ଏହି->blob);
      sc.start_processing ();

      ଅଚଳ OT::cff2 *cff2 = ଏହି->blob->template as<OT::cff2> ();

      ଯଦି (cff2 == &Null (OT::cff2))
        ଯାଅ fail;

      { /* parse top dict */
	hb_ubytes_t ଶୀର୍ଷ_Dict_Str = (cff2 + cff2->ଶୀର୍ଷ_Dict_2).as_ubytes (cff2->ଶୀର୍ଷ_Dict_Size);
	ଯଦି (unlikely (!ଶୀର୍ଷ_Dict_Str.sanitize (&sc))) ଯାଅ fail;
	hb_barrier ();
	ଅଙ୍କ_interp_env_t env (ଶୀର୍ଷ_Dict_Str);
	cff2_ଶୀର୍ଷ_dict_interpreter_t ଶୀର୍ଷ_interp (env);
	ଶୀର୍ଷ_Dict_2.init ();
	ଯଦି (unlikely (!ଶୀର୍ଷ_interp.interpret (ଶୀର୍ଷ_Dict_2))) ଯାଅ fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->ଶୀର୍ଷ_Dict_2 + cff2->ଶୀର୍ଷ_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, ଶୀର୍ଷ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, ଶୀର୍ଷ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, ଶୀର୍ଷ_Dict_2.FDArrayOffset, sc);

      ଯଦି (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        ଯାଅ fail;

      ଅଙ୍କ_glyphs = charStrings->ଗଣନା_2;
      ଯଦି (ଅଙ୍କ_glyphs != sc.get_ଅଙ୍କ_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        ତାର୍କିକ valid_ଗଣନା = ଅଙ୍କ_glyphs < sc.get_ଅଙ୍କ_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        ଯଦି (!valid_ଗଣନା) ଯାଅ fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, ଶୀର୍ଷ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->ଗଣନା_2, ଅଙ୍କ_glyphs);
      ଯଦି (ଶୀର୍ଷ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        ଯାଅ fail;

      fd_ଗଣନା = fdArray->ଗଣନା_2;
      ଯଦି (!privateDicts.resize (fd_ଗଣନା))
        ଯାଅ fail;

      /* parse font dicts and gather private dicts */
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < fd_ଗଣନା; i++)
      {
	ଅଚଳ hb_ubytes_t fontDictStr = (*fdArray)[i];
	ଯଦି (unlikely (!fontDictStr.sanitize (&sc))) ଯାଅ fail;
	hb_barrier ();
	cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t  *font;
	ଅଙ୍କ_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	ଯଦି (unlikely (font == &Crap (cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t))) ଯାଅ fail;
	font->init ();
	ଯଦି (unlikely (!font_interp.interpret (*font))) ଯାଅ fail;

	ଅଚଳ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	ଯଦି (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ଅଚଳ unsigned ଅକ୍ଷର *) &Null (UnsizedByteStr))) ଯାଅ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	ଯଦି (unlikely (!priv_interp.interpret (privateDicts[i]))) ଯାଅ fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      ଫେରନ୍ତୁ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ଶୂନ୍ୟ _fini ()
    {
      sc.end_processing ();
      ଶୀର୍ଷ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      ଯଦି (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ଅଚଳ
    {
      ଫେରନ୍ତୁ nullptr;
    }

    hb_blob_t *get_blob () ଅଚଳ { ଫେରନ୍ତୁ blob; }

    ତାର୍କିକ is_valid () ଅଚଳ { ଫେରନ୍ତୁ blob; }

    ସୁରକ୍ଷିତ:
    hb_sanitize_context_t	sc;

    ସର୍ବସାଧାରଣ:
    hb_blob_t			*blob = nullptr;
    cff2_ଶୀର୍ଷ_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t	ଶୀର୍ଷ_Dict_2;
    ଅଚଳ CFF2Subrs		*globalSubrs = nullptr;
    ଅଚଳ CFF2ItemVariationStore	*varStore = nullptr;
    ଅଚଳ CFF2CharStrings	*charStrings = nullptr;
    ଅଚଳ CFF2FDArray		*fdArray = nullptr;
    ଅଚଳ CFF2FDSelect		*fdSelect = nullptr;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		fd_ଗଣନା = 0;

    hb_vector_t<cff2_font_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<ଦଶମିକ> *> cached_scalars_vector;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	      ଅଙ୍କ_glyphs = 0;
  };

  ସଂରଚନା accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL ତାର୍କିକ get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords,
				     int64_t *budget = nullptr) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords, int64_t *budget = nullptr) ଅଚଳ;
  };

  ସଂରଚନା accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ଯଦି (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ;
    HB_INTERNAL ତାର୍କିକ serialize (hb_serialize_context_t *c,
				ସଂରଚନା cff2_subset_plan &plan,
				hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> normalized_coords) ଅଚଳ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    ପ୍ରକାରବ୍ୟାଖ୍ୟା accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_ମୂଲ୍ୟଗୁଡ଼ିକ_subset_t> SUPER;
  };

  ସର୍ବସାଧାରଣ:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<ଶୀର୍ଷ_Dict, HBUINT8>	ଶୀର୍ଷ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			ଶୀର୍ଷ_Dict_Size;	/* Top DICT size */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (5);
};

ସଂରଚନା cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ସଂରଚନା cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
