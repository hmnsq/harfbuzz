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

#اگر_غیر_تعریف_شدہ HB_OT_CFF2_جدول_HH
#تعریف_کرو HB_OT_CFF2_جدول_HH

#شامل "hb-ot-cff-common.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-draw.hh"
#شامل "hb-paint.hh"

نامخلا CFF {

/*
 * CFF2 -- Compact Font Format (CFF) Version 2
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cff2
 */
#تعریف_کرو HB_OT_لیبل_CFF2 HB_لیبل('C','F','F','2')

قسمتعریف CFF2_اشاریہ         CFF2_Char_لڑیاں;
قسمتعریف Subrs<HBUINT32>   CFF2Subrs;

قسمتعریف FDSelect3_4<HBUINT32, HBUINT16> FDSelect4;
قسمتعریف FDSelect3_4_Range<HBUINT32, HBUINT16> FDSelect4_Range;

ساخت CFF2FDSelect
{
  منطقی serialize (hb_serialize_context_t *c, ثابت CFF2FDSelect &src, غیرعلامتی صحیح عدد_glyphs)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی صحیح سائز = src.get_سائز (عدد_glyphs);
    CFF2FDSelect *dest = c->allocate_سائز<CFF2FDSelect> (سائز);
    اگر (unlikely (!dest)) return_trace (جھوٹ);
    hb_memcpy (dest, &src, سائز);
    return_trace (سچ);
  }

  size_t get_سائز (غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس format.static_سائز + u.format0.get_سائز (عدد_glyphs);
    حالت 3: hb_barrier (); واپس format.static_سائز + u.format3.get_سائز ();
    حالت 4: hb_barrier (); واپس format.static_سائز + u.format4.get_سائز ();
    طےشدہ:واپس 0;
    }
  }

  hb_codepoint_t get_fd (hb_codepoint_t glyph) ثابت
  {
    اگر (یہ == &Null (CFF2FDSelect))
      واپس 0;

    انتخاب (format)
    {
    حالت 0: hb_barrier (); واپس u.format0.get_fd (glyph);
    حالت 3: hb_barrier (); واپس u.format3.get_fd (glyph);
    حالت 4: hb_barrier (); واپس u.format4.get_fd (glyph);
    طےشدہ:واپس 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی صحیح fdcount,
                 غیرعلامتی صحیح charstring_گنتی = HB_CODEPOINT_INVALID) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ)))
      return_trace (جھوٹ);
    hb_barrier ();

    انتخاب (format)
    {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, fdcount, charstring_گنتی));
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c, fdcount, charstring_گنتی));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c, fdcount, charstring_گنتی));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  HBUINT8	format;
  اتحاد {
  FDSelect0	format0;
  FDSelect3	format3;
  FDSelect4	format4;
  } u;
  عوامی:
  DEFINE_سائز_MIN (2);
};

ساخت CFF2_شے_Variation_Store
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  c->check_range (&varStore, سائز) &&
		  varStore.sanitize (c));
  }

  منطقی serialize (hb_serialize_context_t *c, ثابت CFF2_شے_Variation_Store *varStore)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی صحیح سائز_3 = varStore->get_سائز ();
    CFF2_شے_Variation_Store *dest = c->allocate_سائز<CFF2_شے_Variation_Store> (سائز_3);
    اگر (unlikely (!dest)) return_trace (جھوٹ);
    hb_memcpy (dest, varStore, سائز_3);
    return_trace (سچ);
  }

  size_t get_سائز () ثابت { واپس HBUINT16::static_سائز + سائز; }

  HBUINT16	سائز;
  شے_Variation_Store  varStore;

  DEFINE_سائز_MIN (2 + شے_Variation_Store::min_سائز);
};

ساخت cff2_چوٹی_dict_اقدار_t : چوٹی_dict_اقدار_t<>
{
  خالی init ()
  {
    چوٹی_dict_اقدار_t<>::init ();
    vstore_انحراف = 0;
    FDSelect_انحراف = 0;
  }
  خالی fini () { چوٹی_dict_اقدار_t<>::fini (); }

  صحیح  vstore_انحراف;
  صحیح  FDSelect_انحراف;
};

ساخت cff2_چوٹی_dict_opset_t : چوٹی_dict_opset_t<>
{
  ساکن خالی process_op (op_رمز_t op, عدد_interp_env_t& env, cff2_چوٹی_dict_اقدار_t& dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_Font_میٹرکس:
	{
	  dict_val_t val;
	  val.init ();
	  dictval.جوڑو_op (op, env.str_ref);
	  env.clear_دلائل ();
	}
	توڑو;

      حالت Op_رمز_vstore:
	dictval.vstore_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_FDSelect:
	dictval.FDSelect_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env, dictval);
	/* Record this operand below if stack is empty, otherwise done */
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref);
  }

  قسمتعریف چوٹی_dict_opset_t<> SUPER;
};

ساخت cff2_font_dict_اقدار_t : dict_اقدار_t<op_str_t>
{
  خالی init ()
  {
    dict_اقدار_t<op_str_t>::init ();
    نجی_Dict_Info.init ();
  }
  خالی fini () { dict_اقدار_t<op_str_t>::fini (); }

  جدول_info_t    نجی_Dict_Info;
};

ساخت cff2_font_dict_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, عدد_interp_env_t& env, cff2_font_dict_اقدار_t& dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_نجی:
	dictval.نجی_Dict_Info.انحراف = env.arg_ڈھیر.نکالو_uint ();
	dictval.نجی_Dict_Info.سائز = env.arg_ڈھیر.نکالو_uint ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ())
	  واپس;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref);
  }

  نجی:
  قسمتعریف dict_opset_t SUPER;
};

سانچہ <قسم_نام VAL>
ساخت cff2_نجی_dict_اقدار_بنیاد_t : dict_اقدار_t<VAL>
{
  خالی init ()
  {
    dict_اقدار_t<VAL>::init ();
    subrs_انحراف = 0;
    localSubrs = &Null (CFF2Subrs);
    ivs = 0;
  }
  خالی fini () { dict_اقدار_t<VAL>::fini (); }

  صحیح                subrs_انحراف;
  ثابت CFF2Subrs   *localSubrs;
  غیرعلامتی صحیح      ivs;
};

قسمتعریف cff2_نجی_dict_اقدار_بنیاد_t<op_str_t> cff2_نجی_dict_اقدار_subset_t;
قسمتعریف cff2_نجی_dict_اقدار_بنیاد_t<عدد_dict_val_t> cff2_نجی_dict_اقدار_t;

ساخت cff2_priv_dict_interp_env_t : عدد_interp_env_t
{
  cff2_priv_dict_interp_env_t (ثابت hb_ubytes_t &str) :
    عدد_interp_env_t (str) {}

  خالی process_vsindex ()
  {
    اگر (likely (!نظر_آیا_vsindex))
    {
      سیٹ_ivs (arg_ڈھیر.نکالو_uint ());
    }
    نظر_آیا_vsindex = سچ;
  }

  غیرعلامتی صحیح get_ivs () ثابت { واپس ivs; }
  خالی	 سیٹ_ivs (غیرعلامتی صحیح ivs_) { ivs = ivs_; }

  محفوظ:
  غیرعلامتی صحیح  ivs = 0;
  منطقی	  نظر_آیا_vsindex = جھوٹ;
};

ساخت cff2_نجی_dict_opset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_dict_اقدار_t& dictval)
  {
    عدد_dict_val_t val;
    val.init ();

    انتخاب (op) {
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Expansion_ضربی_عدد:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_Subrs:
	dictval.subrs_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلائل ();
	توڑو;
      حالت Op_رمز_blenddict:
        /* Hint values are ignored here. Discard each group so raw operands
         * from successive blends cannot overflow the stack. */
        env.clear_دلائل ();
	توڑو;

      طےشدہ:
	dict_opset_t::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref, val);
  }
};

ساخت cff2_نجی_dict_opset_subset_t : dict_opset_t
{
  ساکن خالی process_op (op_رمز_t op, cff2_priv_dict_interp_env_t& env, cff2_نجی_dict_اقدار_subset_t& dictval)
  {
    انتخاب (op) {
      حالت Op_رمز_Blue_اقدار:
      حالت Op_رمز_Other_Blues:
      حالت Op_رمز_Family_Blues:
      حالت Op_رمز_Family_Other_Blues:
      حالت Op_رمز_Std_HW:
      حالت Op_رمز_Std_VW:
      حالت Op_رمز_Blue_Scale:
      حالت Op_رمز_Blue_سرکاؤ:
      حالت Op_رمز_Blue_Fuzz:
      حالت Op_رمز_Stem_Snap_H:
      حالت Op_رمز_Stem_Snap_V:
      حالت Op_رمز_Language_گروہ:
      حالت Op_رمز_Expansion_ضربی_عدد:
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_blenddict:
	env.clear_دلائل ();
	واپس;

      حالت Op_رمز_Subrs:
	dictval.subrs_انحراف = env.arg_ڈھیر.نکالو_int ();
	env.clear_دلائل ();
	توڑو;

      حالت Op_رمز_vsindexdict:
	env.process_vsindex ();
	dictval.ivs = env.get_ivs ();
	env.clear_دلائل ();
	توڑو;

      طےشدہ:
	SUPER::process_op (op, env);
	اگر (!env.arg_ڈھیر.is_empty ()) واپس;
	توڑو;
    }

    اگر (unlikely (env.اندر_error ())) واپس;

    dictval.جوڑو_op (op, env.str_ref);
  }

  نجی:
  قسمتعریف dict_opset_t SUPER;
};

قسمتعریف dict_interpreter_t<cff2_چوٹی_dict_opset_t, cff2_چوٹی_dict_اقدار_t> cff2_چوٹی_dict_interpreter_t;
قسمتعریف dict_interpreter_t<cff2_font_dict_opset_t, cff2_font_dict_اقدار_t> cff2_font_dict_interpreter_t;

ساخت CFF2FDArray : FDArray<HBUINT32>
{
  /* FDArray::serialize does not compile without this partial specialization */
  سانچہ <قسم_نام ITER, قسم_نام OP_SERIALIZER>
  منطقی serialize (hb_serialize_context_t *c, ITER it, OP_SERIALIZER& opszr)
  { واپس FDArray<HBUINT32>::serialize<cff2_font_dict_اقدار_t, جدول_info_t> (c, it, opszr); }
};

} /* namespace CFF */

نامخلا OT {

استعمال نامخلا CFF;

ساخت cff2
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_CFF2;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 2));
  }

  سانچہ <قسم_نام PRIVOPSET, قسم_نام PRIVDICTVAL>
  ساخت accelerator_templ_t
  {
    ساکن constexpr hb_لیبل_t جدول_لیبل = cff2::جدول_لیبل;

    accelerator_templ_t (hb_face_t *face)
    {
      اگر (!face) واپس;

      چوٹی_Dict_2.init ();
      fontDicts.init ();
      نجی_Dicts.init ();

      یہ->blob = sc.reference_جدول<cff2> (face);

      /* setup for run-time sanitization */
      sc.init (یہ->blob);
      sc.آغاز_processing ();

      ثابت OT::cff2 *cff2 = یہ->blob->سانچہ as<OT::cff2> ();

      اگر (cff2 == &Null (OT::cff2))
        جاؤ fail;

      { /* parse top dict */
	hb_ubytes_t چوٹی_Dict_Str = (cff2 + cff2->چوٹی_Dict_2).as_ubytes (cff2->چوٹی_Dict_سائز);
	اگر (unlikely (!چوٹی_Dict_Str.sanitize (&sc))) جاؤ fail;
	hb_barrier ();
	عدد_interp_env_t env (چوٹی_Dict_Str);
	cff2_چوٹی_dict_interpreter_t چوٹی_interp (env);
	چوٹی_Dict_2.init ();
	اگر (unlikely (!چوٹی_interp.interpret (چوٹی_Dict_2))) جاؤ fail;
      }

      globalSubrs = &Struct_At_انحراف_Or_Null<CFF2Subrs> (cff2, cff2->چوٹی_Dict_2 + cff2->چوٹی_Dict_سائز, sc);
      varStore = &Struct_At_انحراف_Or_Null<CFF2_شے_Variation_Store> (cff2, چوٹی_Dict_2.vstore_انحراف, sc);
      char_لڑیاں = &Struct_At_انحراف_Or_Null<CFF2_Char_لڑیاں> (cff2, چوٹی_Dict_2.char_لڑیاں_انحراف, sc);
      fdArray = &Struct_At_انحراف_Or_Null<CFF2FDArray> (cff2, چوٹی_Dict_2.FDArray_انحراف, sc);

      اگر (char_لڑیاں == &Null (CFF2_Char_لڑیاں) ||
	  globalSubrs == &Null (CFF2Subrs) ||
	  fdArray == &Null (CFF2FDArray))
        جاؤ fail;

      عدد_glyphs = char_لڑیاں->گنتی;
      اگر (عدد_glyphs != sc.get_عدد_glyphs ())
      {
        /* VARC can define glyphs beyond the CFF2 CharStrings INDEX. */
        hb_blob_t *varc = face->reference_جدول (HB_لیبل ('V','A','R','C'));
        منطقی valid_گنتی = عدد_glyphs < sc.get_عدد_glyphs () &&
                           hb_blob_get_لمبائی (varc);
        hb_blob_destroy (varc);
        اگر (!valid_گنتی) جاؤ fail;
      }

      fdSelect = &Struct_At_انحراف_Or_Null<CFF2FDSelect> (cff2, چوٹی_Dict_2.FDSelect_انحراف, sc,
                                                   fdArray->گنتی, عدد_glyphs);
      اگر (چوٹی_Dict_2.FDSelect_انحراف && fdSelect == &Null (CFF2FDSelect))
        جاؤ fail;

      fd_گنتی = fdArray->گنتی;
      اگر (!نجی_Dicts.resize (fd_گنتی))
        جاؤ fail;

      /* parse font dicts and gather private dicts */
      برائے (غیرعلامتی صحیح i = 0; i < fd_گنتی; i++)
      {
	ثابت hb_ubytes_t fontDictStr = (*fdArray)[i];
	اگر (unlikely (!fontDictStr.sanitize (&sc))) جاؤ fail;
	hb_barrier ();
	cff2_font_dict_اقدار_t  *font;
	عدد_interp_env_t env (fontDictStr);
	cff2_font_dict_interpreter_t font_interp (env);
	font = fontDicts.push ();
	اگر (unlikely (font == &Crap (cff2_font_dict_اقدار_t))) جاؤ fail;
	font->init ();
	اگر (unlikely (!font_interp.interpret (*font))) جاؤ fail;

	ثابت hb_ubytes_t privDictStr = Struct_At_انحراف_Or_Null<UnsizedByteStr> (cff2, font->نجی_Dict_Info.انحراف, sc, font->نجی_Dict_Info.سائز).as_ubytes (font->نجی_Dict_Info.سائز);
	اگر (unlikely (font->نجی_Dict_Info.سائز &&
		      privDictStr == (ثابت غیرعلامتی حرف *) &Null (UnsizedByteStr))) جاؤ fail;
	cff2_priv_dict_interp_env_t env2 (privDictStr);
	dict_interpreter_t<PRIVOPSET, PRIVDICTVAL, cff2_priv_dict_interp_env_t> priv_interp (env2);
	نجی_Dicts[i].init ();
	اگر (unlikely (!priv_interp.interpret (نجی_Dicts[i]))) جاؤ fail;

	نجی_Dicts[i].localSubrs = &Struct_At_انحراف_Or_Null<CFF2Subrs> (&privDictStr[0], نجی_Dicts[i].subrs_انحراف, sc);
      }

      واپس;

      fail:
        _fini ();
    }
    ~accelerator_templ_t () { _fini (); }
    خالی _fini ()
    {
      sc.انتہا_processing ();
      چوٹی_Dict_2.fini ();
      fontDicts.fini ();
      نجی_Dicts.fini ();
      hb_blob_destroy (blob);
      blob = خالی_اشاریہ;

      خودکار *scalars = cached_scalars_سمتیہ.get_acquire ();
      اگر (scalars && cached_scalars_سمتیہ.cmpexch (scalars, خالی_اشاریہ))
      {
	scalars->fini ();
	hb_free (scalars);
      }
    }

    hb_سمتیہ_t<uint16_t> *create_glyph_مقصد_sid_map () ثابت
    {
      واپس خالی_اشاریہ;
    }

    hb_blob_t *get_blob () ثابت { واپس blob; }

    منطقی is_valid () ثابت { واپس blob; }

    محفوظ:
    hb_sanitize_context_t	sc;

    عوامی:
    hb_blob_t			*blob = خالی_اشاریہ;
    cff2_چوٹی_dict_اقدار_t	چوٹی_Dict_2;
    ثابت CFF2Subrs		*globalSubrs = خالی_اشاریہ;
    ثابت CFF2_شے_Variation_Store	*varStore = خالی_اشاریہ;
    ثابت CFF2_Char_لڑیاں	*char_لڑیاں = خالی_اشاریہ;
    ثابت CFF2FDArray		*fdArray = خالی_اشاریہ;
    ثابت CFF2FDSelect		*fdSelect = خالی_اشاریہ;
    غیرعلامتی صحیح		fd_گنتی = 0;

    hb_سمتیہ_t<cff2_font_dict_اقدار_t>     fontDicts;
    hb_سمتیہ_t<PRIVDICTVAL>  نجی_Dicts;

    تغیرپذیر hb_atomic_t<hb_سمتیہ_t<اعشاری> *> cached_scalars_سمتیہ;

    غیرعلامتی صحیح	      عدد_glyphs = 0;
  };

  ساخت accelerator_t : accelerator_templ_t<cff2_نجی_dict_opset_t, cff2_نجی_dict_اقدار_t>
  {
    accelerator_t (hb_face_t *face) : accelerator_templ_t (face) {}

    HB_اندرونی منطقی get_extents (hb_font_t *font,
				  hb_codepoint_t glyph,
				  hb_glyph_extents_t *extents) ثابت;
    HB_اندرونی منطقی get_extents_at (hb_font_t *font,
				     hb_codepoint_t glyph,
				     hb_glyph_extents_t *extents,
				     hb_array_t<ثابت صحیح> coords,
				     int64_t *budget = خالی_اشاریہ) ثابت;
    HB_اندرونی منطقی get_راستہ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget = خالی_اشاریہ) ثابت;
    HB_اندرونی منطقی get_راستہ_at (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, hb_array_t<ثابت صحیح> coords, int64_t *budget = خالی_اشاریہ) ثابت;
  };

  ساخت accelerator_subset_t : accelerator_templ_t<cff2_نجی_dict_opset_subset_t, cff2_نجی_dict_اقدار_subset_t>
  {
    accelerator_subset_t (hb_face_t *face) : SUPER (face) {}
    ~accelerator_subset_t ()
    {
      اگر (cff_accelerator)
	cff_subset_accelerator_t::destroy (cff_accelerator);
    }

    HB_اندرونی منطقی subset (hb_subset_context_t *c) ثابت;
    HB_اندرونی منطقی serialize (hb_serialize_context_t *c,
				ساخت cff2_subset_plan &plan,
				hb_array_t<صحیح> normalized_coords) ثابت;

    تغیرپذیر CFF::cff_subset_accelerator_t* cff_accelerator = خالی_اشاریہ;

    قسمتعریف accelerator_templ_t<cff2_نجی_dict_opset_subset_t, cff2_نجی_dict_اقدار_subset_t> SUPER;
  };

  عوامی:
  Fixed_نسخہ<HBUINT8>		نسخہ;	/* Version of CFF2 table. set to 0x0200u */
  NNOffset_مقصد<چوٹی_Dict, HBUINT8>	چوٹی_Dict_2;	/* headerSize = Offset to Top DICT. */
  HBUINT16			چوٹی_Dict_سائز;	/* Top DICT size */

  عوامی:
  DEFINE_سائز_STATIC (5);
};

ساخت cff2_accelerator_t : cff2::accelerator_t {
  cff2_accelerator_t (hb_face_t *face) : cff2::accelerator_t (face) {}
};

ساخت cff2_subset_accelerator_t : cff2::accelerator_subset_t {
  cff2_subset_accelerator_t (hb_face_t *face) : cff2::accelerator_subset_t (face) {}
};

} /* namespace OT */

#اگر_ختم /* HB_OT_CFF2_TABLE_HH */
