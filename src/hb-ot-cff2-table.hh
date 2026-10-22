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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_CFF2_جدول_HH
#وصف_ڪريو HB_OT_CFF2_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

نطاق CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#وصف_ڪريو HB_OT_ٽيگ_CFF2 HB_ٽيگ('C','F','F','2')

تعريفنوع CFF2_اشاريو         CFF2_Char_لڙيون;
تعريفنوع Subrs<HBUINT32>   CFF2Subrs;

تعريفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعريفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

بنية CFF2FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, بنانشان صحيح انگ_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان صحيح سائيز = src.get_سائيز (انگ_glyphs);
    CFF2FDSelect *dest = c->allocate_سائيز<CFF2FDSelect> (سائيز);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, سائيز);
    return_trace (سچ);
  }

  size_t get_سائيز (بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع format.static_سائيز + u.format0.get_سائيز (انگ_glyphs);
    حالة 3: hb_barrier (); أرجع format.static_سائيز + u.format3.get_سائيز ();
    حالة 4: hb_barrier (); أرجع format.static_سائيز + u.format4.get_سائيز ();
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

  منطقي sanitize (hb_sanitize_context_t *c, بنانشان صحيح fdcount,
                 بنانشان صحيح charstring_ڳڻپ = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_ڳڻپ));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_ڳڻپ));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_ڳڻپ));
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
  DEFINE_سائيز_MIN (2);
};

بنية CFF2_شي_Variation_Store
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (&varStore, سائيز) &&
		  varStore.sanitize (c));
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2_شي_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان صحيح سائيز_3 = varStore->get_سائيز ();
    CFF2_شي_Variation_Store *dest = c->allocate_سائيز<CFF2_شي_Variation_Store> (سائيز_3);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, varStore, سائيز_3);
    return_trace (سچ);
  }

  size_t get_سائيز () ثابت { أرجع HBUINT16::static_سائيز + سائيز; }

  HBUINT16	سائيز;
  شي_Variation_Store  varStore;

  DEFINE_سائيز_MIN (2 + شي_Variation_Store::min_سائيز);
};

بنية cff2_چوٽي_dict_قدرون_t : چوٽي_dict_قدرون_t<>
{
  فراغ init ()
  {
    چوٽي_dict_قدرون_t<>::init ();
    vstore_وٿي = 0;
    FDSelect_وٿي = 0;
  }
  فراغ fini () { چوٽي_dict_قدرون_t<>::fini (); }

  صحيح  vstore_وٿي;
  صحيح  FDSelect_وٿي;
};

بنية cff2_چوٽي_dict_opset_t : چوٽي_dict_opset_t<>
{
  ساكن فراغ process_op (op_ڪوڊ_t op, انگ_interp_env_t& env, cff2_چوٽي_dict_قدرون_t& dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Font_ميٽرڪس:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.جوڙيو_op (op, env.str_ref);
	  env.clear_دليلون ();
	}
	اكسر;

      حالة Op_ڪوڊ_vstore:
	dictval.vstore_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_FDSelect:
	dictval.FDSelect_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref);
  }

  تعريفنوع چوٽي_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_قدرون_t : dict_قدرون_t<op_str_t>
{
  فراغ init ()
  {
    dict_قدرون_t<op_str_t>::init ();
    ذاتي_Dict_Info.init ();
  }
  فراغ fini () { dict_قدرون_t<op_str_t>::fini (); }

  جدول_info_t    ذاتي_Dict_Info;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, انگ_interp_env_t& env, cff2_font_dict_قدرون_t& dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_ذاتي:
	dictval.ذاتي_Dict_Info.وٿي = env.arg_ڍير.ڪڍو_uint ();
	dictval.ذاتي_Dict_Info.سائيز = env.arg_ڍير.ڪڍو_uint ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ())
	  أرجع;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

سانچو <قسم_نالو VAL>
بنية cff2_ذاتي_dict_قدرون_بنياد_t : dict_قدرون_t<VAL>
{
  فراغ init ()
  {
    dict_قدرون_t<VAL>::init ();
    subrs_وٿي = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_قدرون_t<VAL>::fini (); }

  صحيح                subrs_وٿي;
  ثابت CFF2Subrs   *localSubrs;
  بنانشان صحيح      ivs;
};

تعريفنوع cff2_ذاتي_dict_قدرون_بنياد_t<op_str_t> cff2_ذاتي_dict_قدرون_subset_t;
تعريفنوع cff2_ذاتي_dict_قدرون_بنياد_t<انگ_dict_val_t> cff2_ذاتي_dict_قدرون_t;

بنية cff2_priv_dict_interp_env_t : انگ_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    انگ_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    إذا (likely (!ڏٺل_vsindex))
    {
      سيٽ_ivs (arg_ڍير.ڪڍو_uint ());
    }
    ڏٺل_vsindex = سچ;
  }

  بنانشان صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سيٽ_ivs (بنانشان صحيح ivs_) { ivs = ivs_; }

  محمي:
  بنانشان صحيح  ivs = 0;
  منطقي	  ڏٺل_vsindex = خطأ;
};

بنية cff2_ذاتي_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_priv_dict_interp_env_t& env, cff2_ذاتي_dict_قدرون_t& dictval)
  {
    انگ_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_Subrs:
	dictval.subrs_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دليلون ();
	اكسر;
      حالة Op_ڪوڊ_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_دليلون ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref, val);
  }
};

بنية cff2_ذاتي_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_ڪوڊ_t op, cff2_priv_dict_interp_env_t& env, cff2_ذاتي_dict_قدرون_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_ڪوڊ_Blue_قدرون:
      حالة Op_ڪوڊ_Other_Blues:
      حالة Op_ڪوڊ_Family_Blues:
      حالة Op_ڪوڊ_Family_Other_Blues:
      حالة Op_ڪوڊ_Std_HW:
      حالة Op_ڪوڊ_Std_VW:
      حالة Op_ڪوڊ_Blue_Scale:
      حالة Op_ڪوڊ_Blue_سرڪايو:
      حالة Op_ڪوڊ_Blue_Fuzz:
      حالة Op_ڪوڊ_Stem_Snap_H:
      حالة Op_ڪوڊ_Stem_Snap_V:
      حالة Op_ڪوڊ_Language_گروهه:
      حالة Op_ڪوڊ_Expansion_ضرب_ڪندڙ:
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_blenddict:
	env.clear_دليلون ();
	أرجع;

      حالة Op_ڪوڊ_Subrs:
	dictval.subrs_وٿي = env.arg_ڍير.ڪڍو_int ();
	env.clear_دليلون ();
	اكسر;

      حالة Op_ڪوڊ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دليلون ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڍير.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.اندريون_error ())) أرجع;

    dictval.جوڙيو_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_چوٽي_dict_opset_t, cff2_چوٽي_dict_قدرون_t> cff2_چوٽي_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_قدرون_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  سانچو <قسم_نالو ITER, قسم_نالو OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT32>::serialize<cff2_font_dict_قدرون_t, جدول_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

استعمال نطاق CFF;

بنية cff2
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_CFF2;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخو.major == 2));
  }

  سانچو <قسم_نالو PRIVOPSET, قسم_نالو PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = cff2::جدول_ٽيگ;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      چوٽي_Dict_2.init ();
      fontDicts.init ();
      ذاتي_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروعات_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->سانچو as<OT::cff2> ();

      إذا (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t چوٽي_Dict_Str = (cff2 + cff2->چوٽي_Dict_2).as_ubytes (cff2->چوٽي_Dict_سائيز);
	إذا (unlikely (!چوٽي_Dict_Str.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	انگ_interp_env_t env (چوٽي_Dict_Str);
	cff2_چوٽي_dict_interpreter_t چوٽي_interp (env);
	چوٽي_Dict_2.init ();
	إذا (unlikely (!چوٽي_interp.interpret (چوٽي_Dict_2))) اذهب fail;
      }

      globalSubrs = &Struct_At_وٿي_Or_Null<CFF2Subrs> (cff2, cff2->چوٽي_Dict_2 + cff2->چوٽي_Dict_سائيز, sc);
      varStore = &Struct_At_وٿي_Or_Null<CFF2_شي_Variation_Store> (cff2, چوٽي_Dict_2.vstore_وٿي, sc);
      char_لڙيون = &Struct_At_وٿي_Or_Null<CFF2_Char_لڙيون> (cff2, چوٽي_Dict_2.char_لڙيون_وٿي, sc);
      fdArray = &Struct_At_وٿي_Or_Null<CFF2FDArray> (cff2, چوٽي_Dict_2.FDArray_وٿي, sc);

      إذا (char_لڙيون == &Null (CFF2_Char_لڙيون) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      انگ_glyphs = char_لڙيون->ڳڻپ;
      إذا (انگ_glyphs != sc.get_انگ_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_جدول (HB_ٽيگ ('V','A','R','C'));
        منطقي valid_ڳڻپ = انگ_glyphs < sc.get_انگ_glyphs () &&
                           hb_blob_get_ڊيگهائي (varc);
        hb_blob_destroy (varc);
        إذا (!valid_ڳڻپ) اذهب fail;
      }

      fdSelect = &Struct_At_وٿي_Or_Null<CFF2FDSelect> (cff2, چوٽي_Dict_2.FDSelect_وٿي, sc,
                                                   fdArray->ڳڻپ, انگ_glyphs);
      إذا (چوٽي_Dict_2.FDSelect_وٿي && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fd_ڳڻپ = fdArray->ڳڻپ;
      إذا (!ذاتي_Dicts.resize (fd_ڳڻپ))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (بنانشان صحيح i = 0; i < fd_ڳڻپ; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	إذا (unlikely (!fontDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	cff2_font_dict_قدرون_t  *font;
	انگ_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	إذا (unlikely (font == &Crap (cff2_font_dict_قدرون_t))) اذهب fail;
	font->init ();
	إذا (unlikely (!font_interp.interpret (*font))) اذهب fail;

	ثابت hb_ubytes_t privDictStr = Struct_At_وٿي_Or_Null<UnsizedByteStr> (cff2, font->ذاتي_Dict_Info.وٿي, sc, font->ذاتي_Dict_Info.سائيز).as_ubytes (font->ذاتي_Dict_Info.سائيز);
	إذا (unlikely (font->ذاتي_Dict_Info.سائيز &&
		      privDictStr == (ثابت بنانشان حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	ذاتي_Dicts[i].init ();
	إذا (unlikely (!priv_interp.interpret (ذاتي_Dicts[i]))) اذهب fail;

	ذاتي_Dicts[i].localSubrs = &Struct_At_وٿي_Or_Null<CFF2Subrs> (&privDictStr[0], ذاتي_Dicts[i].subrs_وٿي, sc);
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.پڇاڙي_processing ();
      چوٽي_Dict_2.fini ();
      fontDicts.fini ();
      ذاتي_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالي_اشارو;

      خودڪار *scalars = cached_scalars_ويڪٽر.get_acquire ();
      إذا (scalars && cached_scalars_ويڪٽر.cmpexch (scalars, خالي_اشارو))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_ويڪٽر_t<uint16_t> *create_glyph_منزل_sid_map () ثابت
    {
      أرجع خالي_اشارو;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = خالي_اشارو;
    cff2_چوٽي_dict_قدرون_t	چوٽي_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = خالي_اشارو;
    ثابت CFF2_شي_Variation_Store	*varStore = خالي_اشارو;
    ثابت CFF2_Char_لڙيون	*char_لڙيون = خالي_اشارو;
    ثابت CFF2FDArray		*fdArray = خالي_اشارو;
    ثابت CFF2FDSelect		*fdSelect = خالي_اشارو;
    بنانشان صحيح		fd_ڳڻپ = 0;

    hb_ويڪٽر_t<cff2_font_dict_قدرون_t>     fontDicts;
    hb_ويڪٽر_t<PRIVDICTVAL>  ذاتي_Dicts;

    بدلجندڙ hb_atomic_t<hb_ويڪٽر_t<عائم> *> cached_scalars_ويڪٽر;

    بنانشان صحيح	      انگ_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_ذاتي_dict_opset_t, cff2_ذاتي_dict_قدرون_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_اندروني منطقي get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_اندروني منطقي get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحيح> coords,
				     int64_t *budget = خالي_اشارو) ثابت;
    HB_اندروني منطقي get_رستو (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالي_اشارو) ثابت;
    HB_اندروني منطقي get_رستو_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget = خالي_اشارو) ثابت;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff2_ذاتي_dict_opset_subset_t, cff2_ذاتي_dict_قدرون_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندروني منطقي subset (hb_subset_context_t *c) ثابت;
    HB_اندروني منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    بدلجندڙ CFF::cff_subset_accelerator_t* cff_accelerator = خالي_اشارو;

    تعريفنوع accelerator_templ_t<cff2_ذاتي_dict_opset_subset_t, cff2_ذاتي_dict_قدرون_subset_t> SUPER;
  };

  عام:
  Fixed_نسخو<HBUINT8>		نسخو;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_منزل<چوٽي_Dict, HBUINT8>	چوٽي_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			چوٽي_Dict_سائيز;	/* Top DICT size */

  عام:
  DEFINE_سائيز_STATIC (5);
};

بنية cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

بنية cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#جيڪڏهن_پورو /* HB_OT_CFF2_TABLE_HH */
