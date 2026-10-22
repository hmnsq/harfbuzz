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

#اگر_تعریف_نشده HB_OT_CFF2_جدول_HH
#تعریف_کن HB_OT_CFF2_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

نطاق CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#تعریف_کن HB_OT_برچسب_CFF2 HB_برچسب('C','F','F','2')

تعريفنوع CFF2_شاخص         CFF2_Char_رشتهها;
تعريفنوع Subrs<HBUINT32>   CFF2Subrs;

تعريفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعريفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

بنية CFF2FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, بیعلامت صحيح رقم_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت صحيح اندازه = src.get_اندازه (رقم_glyphs);
    CFF2FDSelect *dest = c->allocate_اندازه<CFF2FDSelect> (اندازه);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, اندازه);
    return_trace (درست);
  }

  size_t get_اندازه (بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع format.static_اندازه + u.format0.get_اندازه (رقم_glyphs);
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

  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت صحيح fdcount,
                 بیعلامت صحيح charstring_شمار = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا)))
      return_trace (خطأ);
    hb_barrier ();

    بدل (format)
    {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_شمار));
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_شمار));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_شمار));
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

بنية CFF2_قلم_Variation_Store
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (&varStore, اندازه) &&
		  varStore.sanitize (c));
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2_قلم_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت صحيح اندازه_3 = varStore->get_اندازه ();
    CFF2_قلم_Variation_Store *dest = c->allocate_اندازه<CFF2_قلم_Variation_Store> (اندازه_3);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, varStore, اندازه_3);
    return_trace (درست);
  }

  size_t get_اندازه () ثابت { أرجع HBUINT16::static_اندازه + اندازه; }

  HBUINT16	اندازه;
  قلم_Variation_Store  varStore;

  DEFINE_اندازه_MIN (2 + قلم_Variation_Store::min_اندازه);
};

بنية cff2_بالاترین_dict_مقادیر_t : بالاترین_dict_مقادیر_t<>
{
  فراغ init ()
  {
    بالاترین_dict_مقادیر_t<>::init ();
    vstore_جابه_جایی = 0;
    FDSelect_جابه_جایی = 0;
  }
  فراغ fini () { بالاترین_dict_مقادیر_t<>::fini (); }

  صحيح  vstore_جابه_جایی;
  صحيح  FDSelect_جابه_جایی;
};

بنية cff2_بالاترین_dict_opset_t : بالاترین_dict_opset_t<>
{
  ساكن فراغ process_op (op_کد_t op, رقم_interp_env_t& env, cff2_بالاترین_dict_مقادیر_t& dictval)
  {
    بدل (op) {
      حالة Op_کد_Font_ماتریس:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.جمع_کن_op (op, env.str_ref);
	  env.clear_آرگومانها ();
	}
	اكسر;

      حالة Op_کد_vstore:
	dictval.vstore_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_FDSelect:
	dictval.FDSelect_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_پشته.is_empty ()) أرجع;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref);
  }

  تعريفنوع بالاترین_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_مقادیر_t : dict_مقادیر_t<op_str_t>
{
  فراغ init ()
  {
    dict_مقادیر_t<op_str_t>::init ();
    خصوصی_Dict_Info.init ();
  }
  فراغ fini () { dict_مقادیر_t<op_str_t>::fini (); }

  جدول_info_t    خصوصی_Dict_Info;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, رقم_interp_env_t& env, cff2_font_dict_مقادیر_t& dictval)
  {
    بدل (op) {
      حالة Op_کد_خصوصی:
	dictval.خصوصی_Dict_Info.جابه_جایی = env.arg_پشته.برداشت_uint ();
	dictval.خصوصی_Dict_Info.اندازه = env.arg_پشته.برداشت_uint ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ())
	  أرجع;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

قالب <نام_نوع VAL>
بنية cff2_خصوصی_dict_مقادیر_پایه_t : dict_مقادیر_t<VAL>
{
  فراغ init ()
  {
    dict_مقادیر_t<VAL>::init ();
    subrs_جابه_جایی = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_مقادیر_t<VAL>::fini (); }

  صحيح                subrs_جابه_جایی;
  ثابت CFF2Subrs   *localSubrs;
  بیعلامت صحيح      ivs;
};

تعريفنوع cff2_خصوصی_dict_مقادیر_پایه_t<op_str_t> cff2_خصوصی_dict_مقادیر_subset_t;
تعريفنوع cff2_خصوصی_dict_مقادیر_پایه_t<رقم_dict_val_t> cff2_خصوصی_dict_مقادیر_t;

بنية cff2_priv_dict_interp_env_t : رقم_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    رقم_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    إذا (likely (!دیده_vsindex))
    {
      مجموعه_ivs (arg_پشته.برداشت_uint ());
    }
    دیده_vsindex = درست;
  }

  بیعلامت صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 مجموعه_ivs (بیعلامت صحيح ivs_) { ivs = ivs_; }

  محمي:
  بیعلامت صحيح  ivs = 0;
  منطقي	  دیده_vsindex = خطأ;
};

بنية cff2_خصوصی_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, cff2_priv_dict_interp_env_t& env, cff2_خصوصی_dict_مقادیر_t& dictval)
  {
    رقم_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Expansion_ضریب:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_Subrs:
	dictval.subrs_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_آرگومانها ();
	اكسر;
      حالة Op_کد_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref, val);
  }
};

بنية cff2_خصوصی_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کد_t op, cff2_priv_dict_interp_env_t& env, cff2_خصوصی_dict_مقادیر_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کد_Blue_مقادیر:
      حالة Op_کد_Other_Blues:
      حالة Op_کد_Family_Blues:
      حالة Op_کد_Family_Other_Blues:
      حالة Op_کد_Std_HW:
      حالة Op_کد_Std_VW:
      حالة Op_کد_Blue_Scale:
      حالة Op_کد_Blue_لغزش:
      حالة Op_کد_Blue_Fuzz:
      حالة Op_کد_Stem_Snap_H:
      حالة Op_کد_Stem_Snap_V:
      حالة Op_کد_Language_گروه:
      حالة Op_کد_Expansion_ضریب:
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_blenddict:
	env.clear_آرگومانها ();
	أرجع;

      حالة Op_کد_Subrs:
	dictval.subrs_جابه_جایی = env.arg_پشته.برداشت_int ();
	env.clear_آرگومانها ();
	اكسر;

      حالة Op_کد_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_آرگومانها ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_پشته.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.درون_error ())) أرجع;

    dictval.جمع_کن_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_بالاترین_dict_opset_t, cff2_بالاترین_dict_مقادیر_t> cff2_بالاترین_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_مقادیر_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  قالب <نام_نوع ITER, نام_نوع OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT32>::serialize<cff2_font_dict_مقادیر_t, جدول_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

استفاده نطاق CFF;

بنية cff2
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_CFF2;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخه.major == 2));
  }

  قالب <نام_نوع PRIVOPSET, نام_نوع PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_برچسب_t جدول_برچسب = cff2::جدول_برچسب;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      بالاترین_Dict_2.init ();
      fontDicts.init ();
      خصوصی_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروع_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->قالب as<OT::cff2> ();

      إذا (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t بالاترین_Dict_Str = (cff2 + cff2->بالاترین_Dict_2).as_ubytes (cff2->بالاترین_Dict_اندازه);
	إذا (unlikely (!بالاترین_Dict_Str.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	رقم_interp_env_t env (بالاترین_Dict_Str);
	cff2_بالاترین_dict_interpreter_t بالاترین_interp (env);
	بالاترین_Dict_2.init ();
	إذا (unlikely (!بالاترین_interp.interpret (بالاترین_Dict_2))) اذهب fail;
      }

      globalSubrs = &Struct_At_جابه_جایی_Or_Null<CFF2Subrs> (cff2, cff2->بالاترین_Dict_2 + cff2->بالاترین_Dict_اندازه, sc);
      varStore = &Struct_At_جابه_جایی_Or_Null<CFF2_قلم_Variation_Store> (cff2, بالاترین_Dict_2.vstore_جابه_جایی, sc);
      char_رشتهها = &Struct_At_جابه_جایی_Or_Null<CFF2_Char_رشتهها> (cff2, بالاترین_Dict_2.char_رشتهها_جابه_جایی, sc);
      fdArray = &Struct_At_جابه_جایی_Or_Null<CFF2FDArray> (cff2, بالاترین_Dict_2.FDArray_جابه_جایی, sc);

      إذا (char_رشتهها == &Null (CFF2_Char_رشتهها) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      رقم_glyphs = char_رشتهها->شمار;
      إذا (رقم_glyphs != sc.get_رقم_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_جدول (HB_برچسب ('V','A','R','C'));
        منطقي valid_شمار = رقم_glyphs < sc.get_رقم_glyphs () &&
                           hb_blob_get_درازی (varc);
        hb_blob_destroy (varc);
        إذا (!valid_شمار) اذهب fail;
      }

      fdSelect = &Struct_At_جابه_جایی_Or_Null<CFF2FDSelect> (cff2, بالاترین_Dict_2.FDSelect_جابه_جایی, sc,
                                                   fdArray->شمار, رقم_glyphs);
      إذا (بالاترین_Dict_2.FDSelect_جابه_جایی && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fd_شمار = fdArray->شمار;
      إذا (!خصوصی_Dicts.resize (fd_شمار))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (بیعلامت صحيح i = 0; i < fd_شمار; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	إذا (unlikely (!fontDictStr.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	cff2_font_dict_مقادیر_t  *font;
	رقم_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	إذا (unlikely (font == &Crap (cff2_font_dict_مقادیر_t))) اذهب fail;
	font->init ();
	إذا (unlikely (!font_interp.interpret (*font))) اذهب fail;

	ثابت hb_ubytes_t privDictStr = Struct_At_جابه_جایی_Or_Null<UnsizedByteStr> (cff2, font->خصوصی_Dict_Info.جابه_جایی, sc, font->خصوصی_Dict_Info.اندازه).as_ubytes (font->خصوصی_Dict_Info.اندازه);
	إذا (unlikely (font->خصوصی_Dict_Info.اندازه &&
		      privDictStr == (ثابت بیعلامت حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	خصوصی_Dicts[i].init ();
	إذا (unlikely (!priv_interp.interpret (خصوصی_Dicts[i]))) اذهب fail;

	خصوصی_Dicts[i].localSubrs = &Struct_At_جابه_جایی_Or_Null<CFF2Subrs> (&privDictStr[0], خصوصی_Dicts[i].subrs_جابه_جایی, sc);
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.انتها_processing ();
      بالاترین_Dict_2.fini ();
      fontDicts.fini ();
      خصوصی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = اشاره_تهی;

      خودکار *scalars = cached_scalars_وکتور.get_acquire ();
      إذا (scalars && cached_scalars_وکتور.cmpexch (scalars, اشاره_تهی))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_وکتور_t<uint16_t> *create_glyph_مقصد_sid_map () ثابت
    {
      أرجع اشاره_تهی;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = اشاره_تهی;
    cff2_بالاترین_dict_مقادیر_t	بالاترین_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = اشاره_تهی;
    ثابت CFF2_قلم_Variation_Store	*varStore = اشاره_تهی;
    ثابت CFF2_Char_رشتهها	*char_رشتهها = اشاره_تهی;
    ثابت CFF2FDArray		*fdArray = اشاره_تهی;
    ثابت CFF2FDSelect		*fdSelect = اشاره_تهی;
    بیعلامت صحيح		fd_شمار = 0;

    hb_وکتور_t<cff2_font_dict_مقادیر_t>     fontDicts;
    hb_وکتور_t<PRIVDICTVAL>  خصوصی_Dicts;

    تغییرپذیر hb_atomic_t<hb_وکتور_t<عائم> *> cached_scalars_وکتور;

    بیعلامت صحيح	      رقم_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_خصوصی_dict_opset_t, cff2_خصوصی_dict_مقادیر_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_داخلی منطقي get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_داخلی منطقي get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحيح> coords,
				     int64_t *budget = اشاره_تهی) ثابت;
    HB_داخلی منطقي get_مسیر (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = اشاره_تهی) ثابت;
    HB_داخلی منطقي get_مسیر_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget = اشاره_تهی) ثابت;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff2_خصوصی_dict_opset_subset_t, cff2_خصوصی_dict_مقادیر_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_داخلی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_داخلی منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    تغییرپذیر CFF::cff_subset_accelerator_t* cff_accelerator = اشاره_تهی;

    تعريفنوع accelerator_templ_t<cff2_خصوصی_dict_opset_subset_t, cff2_خصوصی_dict_مقادیر_subset_t> SUPER;
  };

  عام:
  Fixed_نسخه<HBUINT8>		نسخه;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_مقصد<بالاترین_Dict, HBUINT8>	بالاترین_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			بالاترین_Dict_اندازه;	/* Top DICT size */

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

#پایان_اگر /* HB_OT_CFF2_TABLE_HH */
