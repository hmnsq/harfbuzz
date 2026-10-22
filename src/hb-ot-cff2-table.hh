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

#ಅನ್ತರ್ಭಾವ "hb-ot-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-cff-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-draw.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.hh"

ನಾಮಾಕಾಶ CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#define HB_OT_TAG_CFF2 HB_TAG('C','F','F','2')

ಪ್ರಕಾರನಿರ್ದೇಶ CFF2Index         CFF2CharStrings;
ಪ್ರಕಾರನಿರ್ದೇಶ Subrs<HBUINT32>   CFF2Subrs;

ಪ್ರಕಾರನಿರ್ದೇಶ FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
ಪ್ರಕಾರನಿರ್ದೇಶ FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ಸಂರಚನಾ CFF2FDSelect
{
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ CFF2FDSelect &src, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ size = src.get_size (ಅಂಕೆ_glyphs);
    CFF2FDSelect *dest = c->allocate_size<CFF2FDSelect> (size);
    ಯದಿ (unlikely (!dest)) return_trace (ಅಸತ್ಯ);
    hb_memcpy (dest, &src, size);
    return_trace (ಸತ್ಯ);
  }

  size_t get_size (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ format.static_size + u.format0.get_size (ಅಂಕೆ_glyphs);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ format.static_size + u.format3.get_size ();
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ format.static_size + u.format4.get_size ();
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ಧ್ರುವ
  {
    ಯದಿ (ಏತತ್ == &Null (CFF2FDSelect))
      ಪ್ರತ್ಯಾಗಮ 0;

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_fd (glyph);
    ಪಕ್ಷ 3: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format3.get_fd (glyph);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_fd (glyph);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ 0;
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ fdcount,
                 unsigned ಪೂರ್ಣಾಙ್ಕ charstring_ಎಣಿಕೆ = HB_CODEPOINT_INVALID) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!c->check_struct (ಏತತ್)))
      return_trace (ಅಸತ್ಯ);
    hb_barrier ();

    ವಿಕಲ್ಪ (format)
    {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_ಎಣಿಕೆ));
    ಪಕ್ಷ 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_ಎಣಿಕೆ));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_ಎಣಿಕೆ));
    ಸ್ವಭಾವ:return_trace (ಅಸತ್ಯ);
    }
  }

  HBUINT8	format;
  ಸಙ್ಘ {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (2);
};

ಸಂರಚನಾ CFF2ItemVariationStore
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  c->check_range (&varStore, size) &&
		  varStore.sanitize (c));
  }

  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ಧ್ರುವ CFF2ItemVariationStore *varStore)
  {
    TRACE_SERIALIZE (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ size_ = varStore->get_size ();
    CFF2ItemVariationStore *dest = c->allocate_size<CFF2ItemVariationStore> (size_);
    ಯದಿ (unlikely (!dest)) return_trace (ಅಸತ್ಯ);
    hb_memcpy (dest, varStore, size_);
    return_trace (ಸತ್ಯ);
  }

  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ HBUINT16::static_size + size; }

  HBUINT16	size;
  ItemVariationStore  varStore;

  DEFINE_SIZE_MIN (2 + ItemVariationStore::min_size);
};

ಸಂರಚನಾ cff2_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t : ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<>
{
  ಶೂನ್ಯ init ()
  {
    ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<>::init ();
    vstoreOffset = 0;
    FDSelectOffset = 0;
  }
  ಶೂನ್ಯ fini () { ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t<>::fini (); }

  ಪೂರ್ಣಾಙ್ಕ  vstoreOffset;
  ಪೂರ್ಣಾಙ್ಕ  FDSelectOffset;
};

ಸಂರಚನಾ cff2_ತುದಿ_dict_opset_t : ತುದಿ_dict_opset_t<>
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ಅಂಕೆ_interp_env_t& env, cff2_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_FontMatrix:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.add_op (op, env.str_ref);
	  env.clear_args ();
	}
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_vstore:
	dictval.vstoreOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_FDSelect:
	dictval.FDSelectOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref);
  }

  ಪ್ರಕಾರನಿರ್ದೇಶ ತುದಿ_dict_opset_t<> SUPER;
};

ಸಂರಚನಾ cff2_font_dict_ಮೌಲ್ಯಗಳು_t : dict_ಮೌಲ್ಯಗಳು_t<op_str_t>
{
  ಶೂನ್ಯ init ()
  {
    dict_ಮೌಲ್ಯಗಳು_t<op_str_t>::init ();
    privateDictInfo.init ();
  }
  ಶೂನ್ಯ fini () { dict_ಮೌಲ್ಯಗಳು_t<op_str_t>::fini (); }

  table_info_t    privateDictInfo;
};

ಸಂರಚನಾ cff2_font_dict_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, ಅಂಕೆ_interp_env_t& env, cff2_font_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_Private:
	dictval.privateDictInfo.offset = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	dictval.privateDictInfo.size = env.arg_ರಾಶಿ.ತೆಗೆ_uint ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ())
	  ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref);
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ dict_opset_t SUPER;
};

template <typename VAL>
ಸಂರಚನಾ cff2_private_dict_ಮೌಲ್ಯಗಳು_base_t : dict_ಮೌಲ್ಯಗಳು_t<VAL>
{
  ಶೂನ್ಯ init ()
  {
    dict_ಮೌಲ್ಯಗಳು_t<VAL>::init ();
    subrsOffset = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  ಶೂನ್ಯ fini () { dict_ಮೌಲ್ಯಗಳು_t<VAL>::fini (); }

  ಪೂರ್ಣಾಙ್ಕ                subrsOffset;
  ಧ್ರುವ CFF2Subrs   *localSubrs;
  unsigned ಪೂರ್ಣಾಙ್ಕ      ivs;
};

ಪ್ರಕಾರನಿರ್ದೇಶ cff2_private_dict_ಮೌಲ್ಯಗಳು_base_t<op_str_t> cff2_private_dict_ಮೌಲ್ಯಗಳು_subset_t;
ಪ್ರಕಾರನಿರ್ದೇಶ cff2_private_dict_ಮೌಲ್ಯಗಳು_base_t<ಅಂಕೆ_dict_val_t> cff2_private_dict_ಮೌಲ್ಯಗಳು_t;

ಸಂರಚನಾ cff2_priv_dict_interp_env_t : ಅಂಕೆ_interp_env_t
{
  cff2_priv_dict_interp_env_t (ಧ್ರುವ hb_ubytes_t &str) :
    ಅಂಕೆ_interp_env_t (str) {}

  ಶೂನ್ಯ process_vsindex ()
  {
    ಯದಿ (likely (!seen_vsindex))
    {
      set_ivs (arg_ರಾಶಿ.ತೆಗೆ_uint ());
    }
    seen_vsindex = ಸತ್ಯ;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_ivs () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ivs; }
  ಶೂನ್ಯ	 set_ivs (unsigned ಪೂರ್ಣಾಙ್ಕ ivs_) { ivs = ivs_; }

  ರಕ್ಷಿತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ  ivs = 0;
  ತಾರ್ಕಿಕ	  seen_vsindex = ಅಸತ್ಯ;
};

ಸಂರಚನಾ cff2_private_dict_opset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_ಮೌಲ್ಯಗಳು_t& dictval)
  {
    ಅಂಕೆ_dict_val_t val;
    val.init ();

    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_ExpansionFactor:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ಭಙ್ಗ;
      ಪಕ್ಷ OpCode_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	dict_opset_t::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref, val);
  }
};

ಸಂರಚನಾ cff2_private_dict_opset_subset_t : dict_opset_t
{
  ಸ್ಥಿರ ಶೂನ್ಯ process_op (op_code_t op, cff2_priv_dict_interp_env_t& env, cff2_private_dict_ಮೌಲ್ಯಗಳು_subset_t& dictval)
  {
    ವಿಕಲ್ಪ (op) {
      ಪಕ್ಷ Op_Code_Blue_ಮೌಲ್ಯಗಳು:
      ಪಕ್ಷ OpCode_OtherBlues:
      ಪಕ್ಷ OpCode_FamilyBlues:
      ಪಕ್ಷ OpCode_FamilyOtherBlues:
      ಪಕ್ಷ OpCode_StdHW:
      ಪಕ್ಷ OpCode_StdVW:
      ಪಕ್ಷ OpCode_BlueScale:
      ಪಕ್ಷ OpCode_BlueShift:
      ಪಕ್ಷ OpCode_BlueFuzz:
      ಪಕ್ಷ OpCode_StemSnapH:
      ಪಕ್ಷ OpCode_StemSnapV:
      ಪಕ್ಷ OpCode_LanguageGroup:
      ಪಕ್ಷ OpCode_ExpansionFactor:
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_blenddict:
	env.clear_args ();
	ಪ್ರತ್ಯಾಗಮ;

      ಪಕ್ಷ OpCode_Subrs:
	dictval.subrsOffset = env.arg_ರಾಶಿ.ತೆಗೆ_int ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಪಕ್ಷ OpCode_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_args ();
	ಭಙ್ಗ;

      ಸ್ವಭಾವ:
	SUPER::process_op (op, env);
	ಯದಿ (!env.arg_ರಾಶಿ.is_empty ()) ಪ್ರತ್ಯಾಗಮ;
	ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (env.in_error ())) ಪ್ರತ್ಯಾಗಮ;

    dictval.add_op (op, env.str_ref);
  }

  ಗುಪ್ತ:
  ಪ್ರಕಾರನಿರ್ದೇಶ dict_opset_t SUPER;
};

ಪ್ರಕಾರನಿರ್ದೇಶ dict_interpreter_t<cff2_ತುದಿ_dict_opset_t, cff2_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t> cff2_ತುದಿ_dict_interpreter_t;
ಪ್ರಕಾರನಿರ್ದೇಶ dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_ಮೌಲ್ಯಗಳು_t> cff2_font_dict_interpreter_t;

ಸಂರಚನಾ CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  template <typename ITER, typename OP_SERIALIZER>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { ಪ್ರತ್ಯಾಗಮ FDArray<HBUINT32>::serialize<cff2_font_dict_ಮೌಲ್ಯಗಳು_t, table_info_t> (c, it, opszr); }
};

} /* namespace CFF */

ನಾಮಾಕಾಶ OT {

using ನಾಮಾಕಾಶ CFF;

ಸಂರಚನಾ cff2
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_CFF2;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  likely (version.major == 2));
  }

  template <typename PRIVOPSET, typename PRIVDICTVAL>
  ಸಂರಚನಾ accelerator_templ_t
  {
    ಸ್ಥಿರ constexpr hb_tag_t tableTag = cff2::tableTag;

    accelerator_templ_t (hb_face_t *face)
    {
      ಯದಿ (!face) ಪ್ರತ್ಯಾಗಮ;

      ತುದಿ_Dict_2.init ();
      fontDicts.init ();
      privateDicts.init ();

      ಏತತ್->blob = sc.reference_table<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (ಏತತ್->blob);
      sc.start_processing ();

      ಧ್ರುವ OT::cff2 *cff2 = ಏತತ್->blob->template as<OT::cff2> ();

      ಯದಿ (cff2 == &Null (OT::cff2))
        ಗಚ್ಛ fail;

      { /* parse top dict */
	hb_ubytes_t ತುದಿ_Dict_Str = (cff2 + cff2->ತುದಿ_Dict_2).as_ubytes (cff2->ತುದಿ_Dict_Size);
	ಯದಿ (unlikely (!ತುದಿ_Dict_Str.sanitize (&sc))) ಗಚ್ಛ fail;
	hb_barrier ();
	ಅಂಕೆ_interp_env_t env (ತುದಿ_Dict_Str);
	cff2_ತುದಿ_dict_interpreter_t ತುದಿ_interp (env);
	ತುದಿ_Dict_2.init ();
	ಯದಿ (unlikely (!ತುದಿ_interp.interpret (ತುದಿ_Dict_2))) ಗಚ್ಛ fail;
      }

      globalSubrs = &StructAtOffsetOrNull<CFF2Subrs> (cff2, cff2->ತುದಿ_Dict_2 + cff2->ತುದಿ_Dict_Size, sc);
      varStore = &StructAtOffsetOrNull<CFF2ItemVariationStore> (cff2, ತುದಿ_Dict_2.vstoreOffset, sc);
      charStrings = &StructAtOffsetOrNull<CFF2CharStrings> (cff2, ತುದಿ_Dict_2.charStringsOffset, sc);
      fdArray = &StructAtOffsetOrNull<CFF2FDArray> (cff2, ತುದಿ_Dict_2.FDArrayOffset, sc);

      ಯದಿ (charStrings == &Null (CFF2CharStrings) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        ಗಚ್ಛ fail;

      ಅಂಕೆ_glyphs = charStrings->ಎಣಿಕೆ;
      ಯದಿ (ಅಂಕೆ_glyphs != sc.get_ಅಂಕೆ_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_table (HB_TAG ('V','A','R','C'));
        ತಾರ್ಕಿಕ valid_ಎಣಿಕೆ = ಅಂಕೆ_glyphs < sc.get_ಅಂಕೆ_glyphs () &&
                           hb_blob_get_length (varc);
        hb_blob_destroy (varc);
        ಯದಿ (!valid_ಎಣಿಕೆ) ಗಚ್ಛ fail;
      }

      fdSelect = &StructAtOffsetOrNull<CFF2FDSelect> (cff2, ತುದಿ_Dict_2.FDSelectOffset, sc,
                                                   fdArray->ಎಣಿಕೆ, ಅಂಕೆ_glyphs);
      ಯದಿ (ತುದಿ_Dict_2.FDSelectOffset && fdSelect == &Null (CFF2FDSelect))
        ಗಚ್ಛ fail;

      fd_ಎಣಿಕೆ = fdArray->ಎಣಿಕೆ;
      ಯದಿ (!privateDicts.resize (fd_ಎಣಿಕೆ))
        ಗಚ್ಛ fail;

      /* parse font dicts and gather private dicts */
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < fd_ಎಣಿಕೆ; i++)
      {
	ಧ್ರುವ hb_ubytes_t fontDictStr = (*fdArray)[i];
	ಯದಿ (unlikely (!fontDictStr.sanitize (&sc))) ಗಚ್ಛ fail;
	hb_barrier ();
	cff2_font_dict_ಮೌಲ್ಯಗಳು_t  *font;
	ಅಂಕೆ_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	ಯದಿ (unlikely (font == &Crap (cff2_font_dict_ಮೌಲ್ಯಗಳು_t))) ಗಚ್ಛ fail;
	font->init ();
	ಯದಿ (unlikely (!font_interp.interpret (*font))) ಗಚ್ಛ fail;

	ಧ್ರುವ hb_ubytes_t privDictStr = StructAtOffsetOrNull<UnsizedByteStr> (cff2, font->privateDictInfo.offset, sc, font->privateDictInfo.size).as_ubytes (font->privateDictInfo.size);
	ಯದಿ (unlikely (font->privateDictInfo.size &&
		      privDictStr == (ಧ್ರುವ unsigned ವರ್ಣ *) &Null (UnsizedByteStr))) ಗಚ್ಛ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	privateDicts[i].init ();
	ಯದಿ (unlikely (!priv_interp.interpret (privateDicts[i]))) ಗಚ್ಛ fail;

	privateDicts[i].localSubrs = &StructAtOffsetOrNull<CFF2Subrs> (&privDictStr[0], privateDicts[i].subrsOffset, sc);
      }

      ಪ್ರತ್ಯಾಗಮ;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    ಶೂನ್ಯ _fini ()
    {
      sc.end_processing ();
      ತುದಿ_Dict_2.fini ();
      fontDicts.fini ();
      privateDicts.fini ();
      hb_blob_destroy (blob);
      blob = nullptr;

      auto *scalars = cached_scalars_vector.get_acquire ();
      ಯದಿ (scalars && cached_scalars_vector.cmpexch (scalars, nullptr))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_vector_t<uint16_t> *create_glyph_to_sid_map () ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ nullptr;
    }

    hb_blob_t *get_blob () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ blob; }

    ತಾರ್ಕಿಕ is_valid () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ blob; }

    ರಕ್ಷಿತ:
    hb_sanitize_context_t	sc;

    ಸಾರ್ವಜನಿಕ:
    hb_blob_t			*blob = nullptr;
    cff2_ತುದಿ_dict_ಮೌಲ್ಯಗಳು_t	ತುದಿ_Dict_2;
    ಧ್ರುವ CFF2Subrs		*globalSubrs = nullptr;
    ಧ್ರುವ CFF2ItemVariationStore	*varStore = nullptr;
    ಧ್ರುವ CFF2CharStrings	*charStrings = nullptr;
    ಧ್ರುವ CFF2FDArray		*fdArray = nullptr;
    ಧ್ರುವ CFF2FDSelect		*fdSelect = nullptr;
    unsigned ಪೂರ್ಣಾಙ್ಕ		fd_ಎಣಿಕೆ = 0;

    hb_vector_t<cff2_font_dict_ಮೌಲ್ಯಗಳು_t>     fontDicts;
    hb_vector_t<PRIVDICTVAL>  privateDicts;

    mutable hb_atomic_t<hb_vector_t<ಭಿನ್ನ> *> cached_scalars_vector;

    unsigned ಪೂರ್ಣಾಙ್ಕ	      ಅಂಕೆ_glyphs = 0;
  };

  ಸಂರಚನಾ accelerator_t : accelerator_templ_t<cff2_private_dict_opset_t, cff2_private_dict_ಮೌಲ್ಯಗಳು_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_INTERNAL ತಾರ್ಕಿಕ get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords,
				     int64_t *budget = nullptr) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = nullptr) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ get_path_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> coords, int64_t *budget = nullptr) ಧ್ರುವ;
  };

  ಸಂರಚನಾ accelerator_subset_t : accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_ಮೌಲ್ಯಗಳು_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      ಯದಿ (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_INTERNAL ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ;
    HB_INTERNAL ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
				ಸಂರಚನಾ cff2_subset_plan &plan,
				hb_array_t<ಪೂರ್ಣಾಙ್ಕ> normalized_coords) ಧ್ರುವ;

    mutable CFF::cff_subset_accelerator_t* cff_accelerator = nullptr;

    ಪ್ರಕಾರನಿರ್ದೇಶ accelerator_templ_t<cff2_private_dict_opset_subset_t, cff2_private_dict_ಮೌಲ್ಯಗಳು_subset_t> SUPER;
  };

  ಸಾರ್ವಜನಿಕ:
  FixedVersion<HBUINT8>		version;	/* Version of CFF2 table. set to 0x0200u */
  NNOffsetTo<ತುದಿ_Dict, HBUINT8>	ತುದಿ_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			ತುದಿ_Dict_Size;	/* Top DICT size */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (5);
};

ಸಂರಚನಾ cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ಸಂರಚನಾ cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#endif /* HB_OT_CFF2_TABLE_HH */
