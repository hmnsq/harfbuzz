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

#که_نه_تعریف_شوی HB_OT_CFF2_جدول_HH
#تعریف_کړه HB_OT_CFF2_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

نطاق CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#تعریف_کړه HB_OT_نښان_CFF2 HB_نښان('C','F','F','2')

تعريفنوع CFF2_شاخص         CFF2_Char_مزي_ګانې;
تعريفنوع Subrs<HBUINT32>   CFF2Subrs;

تعريفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعريفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

بنية CFF2FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, بینښه صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بینښه صحيح اندازه = src.get_اندازه (عدد_glyphs);
    CFF2FDSelect *dest = c->allocate_اندازه<CFF2FDSelect> (اندازه);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, اندازه);
    return_trace (ریښتیا);
  }

  size_t get_اندازه (بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع format.static_اندازه + u.format0.get_اندازه (عدد_glyphs);
    حالة 3: hb_barrier (); أرجع format.static_اندازه + u.format3.get_اندازه ();
    حالة 4: hb_barrier (); أرجع format.static_اندازه + u.format4.get_اندازه ();
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

  منطقي sanitize (hb_sanitize_context_t *c, بینښه صحيح fdcount,
                 بینښه صحيح charstring_شمېر = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_شمېر));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_شمېر));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_شمېر));
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
  DEFINE_اندازه_MIN (2);
};

بنية CFF2_توکی_Variation_Store
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (&varStore, اندازه) &&
		  varStore.sanitize (c));
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2_توکی_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (هذا);
    بینښه صحيح اندازه_3 = varStore->get_اندازه ();
    CFF2_توکی_Variation_Store *dest = c->allocate_اندازه<CFF2_توکی_Variation_Store> (اندازه_3);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, varStore, اندازه_3);
    return_trace (ریښتیا);
  }

  size_t get_اندازه () ثابت { أرجع HBUINT16::static_اندازه + اندازه; }

  HBUINT16	اندازه;
  توکی_Variation_Store  varStore;

  DEFINE_اندازه_MIN (2 + توکی_Variation_Store::min_اندازه);
};

بنية cff2_سر_dict_ارزښتونه_t : سر_dict_ارزښتونه_t<>
{
  فراغ init ()
  {
    سر_dict_ارزښتونه_t<>::init ();
    vstore_دننه_واټن = 0;
    FDSelect_دننه_واټن = 0;
  }
  فراغ fini () { سر_dict_ارزښتونه_t<>::fini (); }

  صحيح  vstore_دننه_واټن;
  صحيح  FDSelect_دننه_واټن;
};

بنية cff2_سر_dict_opset_t : سر_dict_opset_t<>
{
  ساكن فراغ process_op (op_کوډ_t op, عدد_interp_env_t& env, cff2_سر_dict_ارزښتونه_t& dictval)
  {
    بدل (op) {
      حالة Op_کوډ_Font_ماتریکس:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.جمع_کړه_op (op, env.str_ref);
	  env.clear_دلیلونه ();
	}
	اكسر;

      حالة Op_کوډ_vstore:
	dictval.vstore_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_FDSelect:
	dictval.FDSelect_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref);
  }

  تعريفنوع سر_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_ارزښتونه_t : dict_ارزښتونه_t<op_str_t>
{
  فراغ init ()
  {
    dict_ارزښتونه_t<op_str_t>::init ();
    شخصي_Dict_Info.init ();
  }
  فراغ fini () { dict_ارزښتونه_t<op_str_t>::fini (); }

  جدول_info_t    شخصي_Dict_Info;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, عدد_interp_env_t& env, cff2_font_dict_ارزښتونه_t& dictval)
  {
    بدل (op) {
      حالة Op_کوډ_شخصي:
	dictval.شخصي_Dict_Info.دننه_واټن = env.arg_ډېری.راوباسه_uint ();
	dictval.شخصي_Dict_Info.اندازه = env.arg_ډېری.راوباسه_uint ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ())
	  أرجع;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

کينډۍ <ډول_نوم VAL>
بنية cff2_شخصي_dict_ارزښتونه_بنسټ_t : dict_ارزښتونه_t<VAL>
{
  فراغ init ()
  {
    dict_ارزښتونه_t<VAL>::init ();
    subrs_دننه_واټن = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_ارزښتونه_t<VAL>::fini (); }

  صحيح                subrs_دننه_واټن;
  ثابت CFF2Subrs   *localSubrs;
  بینښه صحيح      ivs;
};

تعريفنوع cff2_شخصي_dict_ارزښتونه_بنسټ_t<op_str_t> cff2_شخصي_dict_ارزښتونه_subset_t;
تعريفنوع cff2_شخصي_dict_ارزښتونه_بنسټ_t<عدد_dict_val_t> cff2_شخصي_dict_ارزښتونه_t;

بنية cff2_priv_dict_interp_env_t : عدد_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    عدد_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    إذا (likely (!کتل_شوی_vsindex))
    {
      سېټ_ivs (arg_ډېری.راوباسه_uint ());
    }
    کتل_شوی_vsindex = ریښتیا;
  }

  بینښه صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سېټ_ivs (بینښه صحيح ivs_) { ivs = ivs_; }

  محمي:
  بینښه صحيح  ivs = 0;
  منطقي	  کتل_شوی_vsindex = خطأ;
};

بنية cff2_شخصي_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, cff2_priv_dict_interp_env_t& env, cff2_شخصي_dict_ارزښتونه_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کوډ_Std_HW:
      حالة Op_کوډ_Std_VW:
      حالة Op_کوډ_Blue_Scale:
      حالة Op_کوډ_Blue_ښويدنه:
      حالة Op_کوډ_Blue_Fuzz:
      حالة Op_کوډ_Expansion_ضریب:
      حالة Op_کوډ_Language_ډله:
      حالة Op_کوډ_Blue_ارزښتونه:
      حالة Op_کوډ_Other_Blues:
      حالة Op_کوډ_Family_Blues:
      حالة Op_کوډ_Family_Other_Blues:
      حالة Op_کوډ_Stem_Snap_H:
      حالة Op_کوډ_Stem_Snap_V:
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_Subrs:
	dictval.subrs_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلیلونه ();
	اكسر;
      حالة Op_کوډ_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref, val);
  }
};

بنية cff2_شخصي_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوډ_t op, cff2_priv_dict_interp_env_t& env, cff2_شخصي_dict_ارزښتونه_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کوډ_Blue_ارزښتونه:
      حالة Op_کوډ_Other_Blues:
      حالة Op_کوډ_Family_Blues:
      حالة Op_کوډ_Family_Other_Blues:
      حالة Op_کوډ_Std_HW:
      حالة Op_کوډ_Std_VW:
      حالة Op_کوډ_Blue_Scale:
      حالة Op_کوډ_Blue_ښويدنه:
      حالة Op_کوډ_Blue_Fuzz:
      حالة Op_کوډ_Stem_Snap_H:
      حالة Op_کوډ_Stem_Snap_V:
      حالة Op_کوډ_Language_ډله:
      حالة Op_کوډ_Expansion_ضریب:
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_blenddict:
	env.clear_دلیلونه ();
	أرجع;

      حالة Op_کوډ_Subrs:
	dictval.subrs_دننه_واټن = env.arg_ډېری.راوباسه_int ();
	env.clear_دلیلونه ();
	اكسر;

      حالة Op_کوډ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلیلونه ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ډېری.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.دننه_error ())) أرجع;

    dictval.جمع_کړه_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_سر_dict_opset_t, cff2_سر_dict_ارزښتونه_t> cff2_سر_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_ارزښتونه_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  کينډۍ <ډول_نوم ITER, ډول_نوم OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT32>::serialize<cff2_font_dict_ارزښتونه_t, جدول_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

کارول نطاق CFF;

بنية cff2
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_CFF2;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 2));
  }

  کينډۍ <ډول_نوم PRIVOPSET, ډول_نوم PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_نښان_t جدول_نښان = cff2::جدول_نښان;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      سر_Dict_2.init ();
      fontDicts.init ();
      شخصي_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.پیل_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->کينډۍ as<OT::cff2> ();

      إذا (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t سر_Dict_Str = (cff2 + cff2->سر_Dict_2).as_ubytes (cff2->سر_Dict_اندازه);
	إذا (unlikely (!سر_Dict_Str.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	عدد_interp_env_t env (سر_Dict_Str);
	cff2_سر_dict_interpreter_t سر_interp (env);
	سر_Dict_2.init ();
	إذا (unlikely (!سر_interp.interpret (سر_Dict_2))) اذهب fail;
      }

      globalSubrs = &Struct_At_دننه_واټن_Or_Null<CFF2Subrs> (cff2, cff2->سر_Dict_2 + cff2->سر_Dict_اندازه, sc);
      varStore = &Struct_At_دننه_واټن_Or_Null<CFF2_توکی_Variation_Store> (cff2, سر_Dict_2.vstore_دننه_واټن, sc);
      char_مزي_ګانې = &Struct_At_دننه_واټن_Or_Null<CFF2_Char_مزي_ګانې> (cff2, سر_Dict_2.char_مزي_ګانې_دننه_واټن, sc);
      fdArray = &Struct_At_دننه_واټن_Or_Null<CFF2FDArray> (cff2, سر_Dict_2.FDArray_دننه_واټن, sc);

      إذا (char_مزي_ګانې == &Null (CFF2_Char_مزي_ګانې) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      عدد_glyphs = char_مزي_ګانې->شمېر;
      إذا (عدد_glyphs != sc.get_عدد_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_جدول (HB_نښان ('V','A','R','C'));
        منطقي valid_شمېر = عدد_glyphs < sc.get_عدد_glyphs () &&
                           hb_blob_get_اوږدتیا (varc);
        hb_blob_destroy (varc);
        إذا (!valid_شمېر) اذهب fail;
      }

      fdSelect = &Struct_At_دننه_واټن_Or_Null<CFF2FDSelect> (cff2, سر_Dict_2.FDSelect_دننه_واټن, sc,
                                                   fdArray->شمېر, عدد_glyphs);
      إذا (سر_Dict_2.FDSelect_دننه_واټن && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fd_شمېر = fdArray->شمېر;
      إذا (!شخصي_Dicts.resize (fd_شمېر))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (بینښه صحيح i = 0; i < fd_شمېر; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	إذا (unlikely (!fontDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	cff2_font_dict_ارزښتونه_t  *font;
	عدد_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	إذا (unlikely (font == &Crap (cff2_font_dict_ارزښتونه_t))) اذهب fail;
	font->init ();
	إذا (unlikely (!font_interp.interpret (*font))) اذهب fail;

	ثابت hb_ubytes_t privDictStr = Struct_At_دننه_واټن_Or_Null<UnsizedByteStr> (cff2, font->شخصي_Dict_Info.دننه_واټن, sc, font->شخصي_Dict_Info.اندازه).as_ubytes (font->شخصي_Dict_Info.اندازه);
	إذا (unlikely (font->شخصي_Dict_Info.اندازه &&
		      privDictStr == (ثابت بینښه حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	شخصي_Dicts[i].init ();
	إذا (unlikely (!priv_interp.interpret (شخصي_Dicts[i]))) اذهب fail;

	شخصي_Dicts[i].localSubrs = &Struct_At_دننه_واټن_Or_Null<CFF2Subrs> (&privDictStr[0], شخصي_Dicts[i].subrs_دننه_واټن, sc);
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.پای_ټکی_processing ();
      سر_Dict_2.fini ();
      fontDicts.fini ();
      شخصي_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = تش_اشاره;

      خپلکاری *scalars = cached_scalars_ویکټور.get_acquire ();
      إذا (scalars && cached_scalars_ویکټور.cmpexch (scalars, تش_اشاره))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_ویکټور_t<uint16_t> *create_glyph_منزل_sid_map () ثابت
    {
      أرجع تش_اشاره;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = تش_اشاره;
    cff2_سر_dict_ارزښتونه_t	سر_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = تش_اشاره;
    ثابت CFF2_توکی_Variation_Store	*varStore = تش_اشاره;
    ثابت CFF2_Char_مزي_ګانې	*char_مزي_ګانې = تش_اشاره;
    ثابت CFF2FDArray		*fdArray = تش_اشاره;
    ثابت CFF2FDSelect		*fdSelect = تش_اشاره;
    بینښه صحيح		fd_شمېر = 0;

    hb_ویکټور_t<cff2_font_dict_ارزښتونه_t>     fontDicts;
    hb_ویکټور_t<PRIVDICTVAL>  شخصي_Dicts;

    بدلیدونکی hb_atomic_t<hb_ویکټور_t<عائم> *> cached_scalars_ویکټور;

    بینښه صحيح	      عدد_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_شخصي_dict_opset_t, cff2_شخصي_dict_ارزښتونه_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_دننی منطقي get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_دننی منطقي get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحيح> coords,
				     int64_t *budget = تش_اشاره) ثابت;
    HB_دننی منطقي get_لار (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = تش_اشاره) ثابت;
    HB_دننی منطقي get_لار_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget = تش_اشاره) ثابت;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff2_شخصي_dict_opset_subset_t, cff2_شخصي_dict_ارزښتونه_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_دننی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_دننی منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    بدلیدونکی CFF::cff_subset_accelerator_t* cff_accelerator = تش_اشاره;

    تعريفنوع accelerator_templ_t<cff2_شخصي_dict_opset_subset_t, cff2_شخصي_dict_ارزښتونه_subset_t> SUPER;
  };

  عام:
  Fixed_نسخه<HBUINT8>		نسخه;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_منزل<سر_Dict, HBUINT8>	سر_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			سر_Dict_اندازه;	/* Top DICT size */

  عام:
  DEFINE_اندازه_STATIC (5);
};

بنية cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

بنية cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#د_که_پای /* HB_OT_CFF2_TABLE_HH */
