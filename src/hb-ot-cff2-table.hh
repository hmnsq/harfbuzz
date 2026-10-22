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

#جے_بنا_تعریف HB_OT_CFF2_جدول_HH
#تعریف_کرو HB_OT_CFF2_جدول_HH

#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-draw.hh"
#ضمن "hb-paint.hh"

نطاق CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#تعریف_کرو HB_OT_ٹیگ_CFF2 HB_ٹیگ('C','F','F','2')

تعريفنوع CFF2_اشاریہ         CFF2_Char_لڑیاں;
تعريفنوع Subrs<HBUINT32>   CFF2Subrs;

تعريفنوع FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
تعريفنوع FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

بنية CFF2FDSelect
{
  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, بنانشانی صحيح عدد_glyphs)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی صحيح ناپ = src.get_ناپ (عدد_glyphs);
    CFF2FDSelect *dest = c->allocate_ناپ<CFF2FDSelect> (ناپ);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, &src, ناپ);
    return_trace (سچ);
  }

  size_t get_ناپ (بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (format)
    {
    حالة 0: hb_barrier (); أرجع format.static_ناپ + u.format0.get_ناپ (عدد_glyphs);
    حالة 3: hb_barrier (); أرجع format.static_ناپ + u.format3.get_ناپ ();
    حالة 4: hb_barrier (); أرجع format.static_ناپ + u.format4.get_ناپ ();
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

  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی صحيح fdcount,
                 بنانشانی صحيح charstring_گنتی = HB_CODEPOINT_INVALID) ثابت
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
  DEFINE_ناپ_MIN (2);
};

بنية CFF2_شے_Variation_Store
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (&varStore, ناپ) &&
		  varStore.sanitize (c));
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت CFF2_شے_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی صحيح ناپ_3 = varStore->get_ناپ ();
    CFF2_شے_Variation_Store *dest = c->allocate_ناپ<CFF2_شے_Variation_Store> (ناپ_3);
    إذا (unlikely (!dest)) return_trace (خطأ);
    hb_memcpy (dest, varStore, ناپ_3);
    return_trace (سچ);
  }

  size_t get_ناپ () ثابت { أرجع HBUINT16::static_ناپ + ناپ; }

  HBUINT16	ناپ;
  شے_Variation_Store  varStore;

  DEFINE_ناپ_MIN (2 + شے_Variation_Store::min_ناپ);
};

بنية cff2_چوٹی_dict_اقدار_t : چوٹی_dict_اقدار_t<>
{
  فراغ init ()
  {
    چوٹی_dict_اقدار_t<>::init ();
    vstore_وتھ = 0;
    FDSelect_وتھ = 0;
  }
  فراغ fini () { چوٹی_dict_اقدار_t<>::fini (); }

  صحيح  vstore_وتھ;
  صحيح  FDSelect_وتھ;
};

بنية cff2_چوٹی_dict_opset_t : چوٹی_dict_opset_t<>
{
  ساكن فراغ process_op (op_کوڈ_t op, عدد_interp_env_t& env, cff2_چوٹی_dict_اقدار_t& dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_Font_میٹرکس:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.جوڑو_op (op, env.str_ref);
	  env.clear_دلیلاں ();
	}
	اكسر;

      حالة Op_کوڈ_vstore:
	dictval.vstore_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_FDSelect:
	dictval.FDSelect_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref);
  }

  تعريفنوع چوٹی_dict_opset_t<> SUPER;
};

بنية cff2_font_dict_اقدار_t : dict_اقدار_t<op_str_t>
{
  فراغ init ()
  {
    dict_اقدار_t<op_str_t>::init ();
    نجی_Dict_Info.init ();
  }
  فراغ fini () { dict_اقدار_t<op_str_t>::fini (); }

  جدول_info_t    نجی_Dict_Info;
};

بنية cff2_font_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, عدد_interp_env_t& env, cff2_font_dict_اقدار_t& dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_نجی:
	dictval.نجی_Dict_Info.وتھ = env.arg_ڈھیر.نکالو_uint ();
	dictval.نجی_Dict_Info.ناپ = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ())
	  أرجع;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

سانچا <قسم_ناں VAL>
بنية cff2_نجی_dict_اقدار_ادھار_t : dict_اقدار_t<VAL>
{
  فراغ init ()
  {
    dict_اقدار_t<VAL>::init ();
    subrs_وتھ = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  فراغ fini () { dict_اقدار_t<VAL>::fini (); }

  صحيح                subrs_وتھ;
  ثابت CFF2Subrs   *localSubrs;
  بنانشانی صحيح      ivs;
};

تعريفنوع cff2_نجی_dict_اقدار_ادھار_t<op_str_t> cff2_نجی_dict_اقدار_subset_t;
تعريفنوع cff2_نجی_dict_اقدار_ادھار_t<عدد_dict_val_t> cff2_نجی_dict_اقدار_t;

بنية cff2_priv_dict_interp_env_t : عدد_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    عدد_interp_env_t (str) {}

  فراغ process_vsindex ()
  {
    إذا (likely (!تکیا_vsindex))
    {
      سیٹ_ivs (arg_ڈھیر.نکالو_uint ());
    }
    تکیا_vsindex = سچ;
  }

  بنانشانی صحيح get_ivs () ثابت { أرجع ivs; }
  فراغ	 سیٹ_ivs (بنانشانی صحيح ivs_) { ivs = ivs_; }

  محمي:
  بنانشانی صحيح  ivs = 0;
  منطقي	  تکیا_vsindex = خطأ;
};

بنية cff2_نجی_dict_opset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_dict_اقدار_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    بدل (op) {
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Expansion_ضارب:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_Subrs:
	dictval.subrs_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلیلاں ();
	اكسر;
      حالة Op_کوڈ_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	dict_opset_t::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

بنية cff2_نجی_dict_opset_subset_t : dict_opset_t
{
  ساكن فراغ process_op (op_کوڈ_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_dict_اقدار_subset_t& dictval)
  {
    بدل (op) {
      حالة Op_کوڈ_Blue_اقدار:
      حالة Op_کوڈ_Other_Blues:
      حالة Op_کوڈ_Family_Blues:
      حالة Op_کوڈ_Family_Other_Blues:
      حالة Op_کوڈ_Std_HW:
      حالة Op_کوڈ_Std_VW:
      حالة Op_کوڈ_Blue_Scale:
      حالة Op_کوڈ_Blue_کھسکاؤ:
      حالة Op_کوڈ_Blue_Fuzz:
      حالة Op_کوڈ_Stem_Snap_H:
      حالة Op_کوڈ_Stem_Snap_V:
      حالة Op_کوڈ_Language_گروہ:
      حالة Op_کوڈ_Expansion_ضارب:
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_blenddict:
	env.clear_دلیلاں ();
	أرجع;

      حالة Op_کوڈ_Subrs:
	dictval.subrs_وتھ = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلیلاں ();
	اكسر;

      حالة Op_کوڈ_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلیلاں ();
	اكسر;

      افتراضي:
	SUPER::process_op (op, env);
	إذا (!env.arg_ڈھیر.is_empty ()) أرجع;
	اكسر;
    }

    إذا (unlikely (env.وچ_error ())) أرجع;

    dictval.جوڑو_op (op, env.str_ref);
  }

  خاص:
  تعريفنوع dict_opset_t SUPER;
};

تعريفنوع dict_interpreter_t<cff2_چوٹی_dict_opset_t, cff2_چوٹی_dict_اقدار_t> cff2_چوٹی_dict_interpreter_t;
تعريفنوع dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_اقدار_t> cff2_font_dict_interpreter_t;

بنية CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  سانچا <قسم_ناں ITER, قسم_ناں OP_SERIALIZER>
  منطقي serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { أرجع FDArray<HBUINT32>::serialize<cff2_font_dict_اقدار_t, جدول_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نطاق OT {

ورتوں نطاق CFF;

بنية cff2
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_CFF2;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2));
  }

  سانچا <قسم_ناں PRIVOPSET, قسم_ناں PRIVDICTVAL>
  بنية accelerator_templ_t
  {
    ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = cff2::جدول_ٹیگ;

    accelerator_templ_t (hb_face_t *face)
    {
      إذا (!face) أرجع;

      چوٹی_Dict_2.init ();
      fontDicts.init ();
      نجی_Dicts.init ();

      هذا->blob = sc.reference_جدول<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (هذا->blob);
      sc.شروع_processing ();

      ثابت OT::cff2 *cff2 = هذا->blob->سانچا as<OT::cff2> ();

      إذا (cff2 == &Null (OT::cff2))
        اذهب fail;

      { /* parse top dict */
	hb_ubytes_t چوٹی_Dict_Str = (cff2 + cff2->چوٹی_Dict_2).as_ubytes (cff2->چوٹی_Dict_ناپ);
	إذا (unlikely (!چوٹی_Dict_Str.sanitize (&sc))) اذهب fail;
	hb_barrier ();
	عدد_interp_env_t env (چوٹی_Dict_Str);
	cff2_چوٹی_dict_interpreter_t چوٹی_interp (env);
	چوٹی_Dict_2.init ();
	إذا (unlikely (!چوٹی_interp.interpret (چوٹی_Dict_2))) اذهب fail;
      }

      globalSubrs = &Struct_At_وتھ_Or_Null<CFF2Subrs> (cff2, cff2->چوٹی_Dict_2 + cff2->چوٹی_Dict_ناپ, sc);
      varStore = &Struct_At_وتھ_Or_Null<CFF2_شے_Variation_Store> (cff2, چوٹی_Dict_2.vstore_وتھ, sc);
      char_لڑیاں = &Struct_At_وتھ_Or_Null<CFF2_Char_لڑیاں> (cff2, چوٹی_Dict_2.char_لڑیاں_وتھ, sc);
      fdArray = &Struct_At_وتھ_Or_Null<CFF2FDArray> (cff2, چوٹی_Dict_2.FDArray_وتھ, sc);

      إذا (char_لڑیاں == &Null (CFF2_Char_لڑیاں) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        اذهب fail;

      عدد_glyphs = char_لڑیاں->گنتی;
      إذا (عدد_glyphs != sc.get_عدد_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_جدول (HB_ٹیگ ('V','A','R','C'));
        منطقي valid_گنتی = عدد_glyphs < sc.get_عدد_glyphs () &&
                           hb_blob_get_لمائی (varc);
        hb_blob_destroy (varc);
        إذا (!valid_گنتی) اذهب fail;
      }

      fdSelect = &Struct_At_وتھ_Or_Null<CFF2FDSelect> (cff2, چوٹی_Dict_2.FDSelect_وتھ, sc,
                                                   fdArray->گنتی, عدد_glyphs);
      إذا (چوٹی_Dict_2.FDSelect_وتھ && fdSelect == &Null (CFF2FDSelect))
        اذهب fail;

      fd_گنتی = fdArray->گنتی;
      إذا (!نجی_Dicts.resize (fd_گنتی))
        اذهب fail;

      /* parse font dicts and gather private dicts */
      لكل (بنانشانی صحيح i = 0; i < fd_گنتی; i++)
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

	ثابت hb_ubytes_t privDictStr = Struct_At_وتھ_Or_Null<UnsizedByteStr> (cff2, font->نجی_Dict_Info.وتھ, sc, font->نجی_Dict_Info.ناپ).as_ubytes (font->نجی_Dict_Info.ناپ);
	إذا (unlikely (font->نجی_Dict_Info.ناپ &&
		      privDictStr == (ثابت بنانشانی حرف *) &Null (UnsizedByteStr))) اذهب fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	نجی_Dicts[i].init ();
	إذا (unlikely (!priv_interp.interpret (نجی_Dicts[i]))) اذهب fail;

	نجی_Dicts[i].localSubrs = &Struct_At_وتھ_Or_Null<CFF2Subrs> (&privDictStr[0], نجی_Dicts[i].subrs_وتھ, sc);
      }

      أرجع;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    فراغ _fini ()
    {
      sc.سرا_processing ();
      چوٹی_Dict_2.fini ();
      fontDicts.fini ();
      نجی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالی_سوچک;

      خودکار *scalars = cached_scalars_سمتیہ.get_acquire ();
      إذا (scalars && cached_scalars_سمتیہ.cmpexch (scalars, خالی_سوچک))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_سمتیہ_t<uint16_t> *create_glyph_منزل_sid_map () ثابت
    {
      أرجع خالی_سوچک;
    }

    hb_blob_t *get_blob () ثابت { أرجع blob; }

    منطقي is_valid () ثابت { أرجع blob; }

    محمي:
    hb_sanitize_context_t	sc;

    عام:
    hb_blob_t			*blob = خالی_سوچک;
    cff2_چوٹی_dict_اقدار_t	چوٹی_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = خالی_سوچک;
    ثابت CFF2_شے_Variation_Store	*varStore = خالی_سوچک;
    ثابت CFF2_Char_لڑیاں	*char_لڑیاں = خالی_سوچک;
    ثابت CFF2FDArray		*fdArray = خالی_سوچک;
    ثابت CFF2FDSelect		*fdSelect = خالی_سوچک;
    بنانشانی صحيح		fd_گنتی = 0;

    hb_سمتیہ_t<cff2_font_dict_اقدار_t>     fontDicts;
    hb_سمتیہ_t<PRIVDICTVAL>  نجی_Dicts;

    بدلنجوگ hb_atomic_t<hb_سمتیہ_t<عائم> *> cached_scalars_سمتیہ;

    بنانشانی صحيح	      عدد_glyphs = 0;
  };

  بنية accelerator_t : accelerator_templ_t<cff2_نجی_dict_opset_t, cff2_نجی_dict_اقدار_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_اندرونی منطقي get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_اندرونی منطقي get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحيح> coords,
				     int64_t *budget = خالی_سوچک) ثابت;
    HB_اندرونی منطقي get_رستہ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالی_سوچک) ثابت;
    HB_اندرونی منطقي get_رستہ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحيح> coords, int64_t *budget = خالی_سوچک) ثابت;
  };

  بنية accelerator_subset_t : accelerator_templ_t<cff2_نجی_dict_opset_subset_t, cff2_نجی_dict_اقدار_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      إذا (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندرونی منطقي subset (hb_subset_context_t *c) ثابت;
    HB_اندرونی منطقي serialize (hb_serialize_context_t *c,
				بنية cff2_subset_plan &plan,
				hb_array_t<صحيح> normalized_coords) ثابت;

    بدلنجوگ CFF::cff_subset_accelerator_t* cff_accelerator = خالی_سوچک;

    تعريفنوع accelerator_templ_t<cff2_نجی_dict_opset_subset_t, cff2_نجی_dict_اقدار_subset_t> SUPER;
  };

  عام:
  Fixed_نسخہ<HBUINT8>		نسخہ;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_منزل<چوٹی_Dict, HBUINT8>	چوٹی_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			چوٹی_Dict_ناپ;	/* Top DICT size */

  عام:
  DEFINE_ناپ_STATIC (5);
};

بنية cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

بنية cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#جے_انت /* HB_OT_CFF2_TABLE_HH */
