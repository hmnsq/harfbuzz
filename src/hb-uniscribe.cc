/*
 * Copyright © 2011,2012,2013  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ضمن "hb.hh"

#که_تعریف_شوی HAVE_UNISCRIBE

#که_تعریف_شوی HB_NO_OT_نښان
#تېروتنه "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#د_که_پای

#ضمن "hb-shaper-impl.hh"

#ضمن <windows.h>
#ضمن <usp10.h>
#ضمن <rpc.h>

#که_نه_تعریف_شوی E_NOT_SUFFICIENT_بفر
#تعریف_کړه E_NOT_SUFFICIENT_بفر HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)
#د_که_پای

#ضمن "hb-uniscribe.h"

#ضمن "hb-ms-feature-ranges.hh"
#ضمن "hb-open-file.hh"
#ضمن "hb-ot-name-table.hh"
#ضمن "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

تعريفنوع HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ثابت WCHAR *pwc_دننه_Chars,
  صحيح c_دننه_Chars,
  صحيح c_Max_توکي,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_توکی *p_توکي,
  OPENTYPE_نښان *p_Script_نښانونه,
  صحيح *pc_توکي
);

تعريفنوع HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_نښان نښان_Script,
  OPENTYPE_نښان نښان_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  کلمه *pw_بهر_Glyphs,
  SCRIPT_GLYPHPROP *p_بهر_Glyph_Props,
  صحيح *pcGlyphs
);

تعريفنوع HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_نښان نښان_Script,
  OPENTYPE_نښان نښان_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت کلمه *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ساكن HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ثابت WCHAR *pwc_دننه_Chars,
  صحيح c_دننه_Chars,
  صحيح c_Max_توکي,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_توکی *p_توکي,
  OPENTYPE_نښان *p_Script_نښانونه,
  صحيح *pc_توکي
)
{
{
  أرجع ScriptItemize (pwc_دننه_Chars,
			c_دننه_Chars,
			c_Max_توکي,
			psControl,
			psState,
			p_توکي,
			pc_توکي);
}
}

ساكن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_نښان نښان_Script,
  OPENTYPE_نښان نښان_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  کلمه *pw_بهر_Glyphs,
  SCRIPT_GLYPHPROP *p_بهر_Glyph_Props,
  صحيح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_بهر_Glyph_Props;
  أرجع ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_بهر_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ساكن HRESULT WINAPI
hb_Script_ځای_پرځای_کړه_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_نښان نښان_Script,
  OPENTYPE_نښان نښان_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت کلمه *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  أرجع Script_ځای_پرځای_کړه (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


بنية hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT Script_ځای_پرځای_کړه_Open_Type;

  فراغ init ()
  {
    HMODULE hinstLib;
    هذا->ScriptItemizeOpenType = تش_اشاره;
    هذا->ScriptShapeOpenType   = تش_اشاره;
    هذا->Script_ځای_پرځای_کړه_Open_Type   = تش_اشاره;

    hinstLib = GetModuleHandle (متن_2 ("usp10.dll"));
    إذا (hinstLib)
    {
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wcast-function-type"
      هذا->ScriptItemizeOpenType = (SIOT) Get_Proc_پته (hinstLib, "ScriptItemizeOpenType");
      هذا->ScriptShapeOpenType   = (SSOT) Get_Proc_پته (hinstLib, "ScriptShapeOpenType");
      هذا->Script_ځای_پرځای_کړه_Open_Type   = (SPOT) Get_Proc_پته (hinstLib, "ScriptPlaceOpenType");
#لارښوونه GCC diagnostic راوباسه
    }
    إذا (!هذا->ScriptItemizeOpenType ||
	!هذا->ScriptShapeOpenType   ||
	!هذا->Script_ځای_پرځای_کړه_Open_Type)
    {
      DEBUG_پیغام (UNISCRIBE, تش_اشاره, "OpenType versions of functions not found; falling back.");
      هذا->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      هذا->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      هذا->Script_ځای_پرځای_کړه_Open_Type   = hb_Script_ځای_پرځای_کړه_Open_Type;
    }
  }
};

ساكن مضمن فراغ free_static_uniscribe_shaper_funcs ();

ساكن بنية hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  ساكن hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, حجم (hb_uniscribe_shaper_funcs_t));
    إذا (unlikely (!funcs))
      أرجع تش_اشاره;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    أرجع funcs;
  }
  ساكن فراغ destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((فراغ *) p);
  }
  ساكن hb_uniscribe_shaper_funcs_t *get_null ()
  {
    أرجع تش_اشاره;
  }
} static_uniscribe_shaper_funcs;

ساكن مضمن
فراغ free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

ساكن hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  أرجع static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

بنية hb_uniscribe_face_معلومات_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ساكن فراغ
_hb_generate_unique_face_name (wchar_t *face_name, بینښه صحيح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  بینښه صحيح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  بینښه حرف *p = (بینښه حرف *) &id;
  لكل (بینښه صحيح i = 0; i < 16; i += 2)
  {
    /* Spread the 16 bits from two bytes of the UUID across three chars of face_name,
     * using the bits in groups of 5,5,6 to select chars from enc.
     * This will generate 24 characters; with the 'F_' prefix we already provided,
     * the name will be 26 chars (plus the NUL terminator), so will always fit within
     * face_name (LF_FACESIZE = 32). */
    face_name[name_str_len++] = enc[p[i] >> 3];
    face_name[name_str_len++] = enc[((p[i] << 2) | (p[i + 1] >> 6)) & 0x1f];
    face_name[name_str_len++] = enc[p[i + 1] & 0x3f];
  }
  face_name[name_str_len] = 0;
  إذا (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ساكن hb_blob_t *
_hb_rename_font (hb_blob_t *blob_دننه, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_دننه));

  بینښه صحيح اوږدتیا, new_اوږدتیا, name_str_len;
  ثابت حرف *orig_sfnt_معلومات = hb_blob_get_معلومات (blob, &اوږدتیا);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساكن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  بینښه صحيح name_جدول_اوږدتیا = OT::name::min_اندازه +
				   ARRAY_اوږدتیا (name_IDs) * OT::Name_ریکارډ::static_اندازه +
				   name_str_len * 2; /* for name data in UTF16BE form */
  بینښه صحيح padded_name_جدول_اوږدتیا = ((name_جدول_اوږدتیا + 3) & ~3);
  بینښه صحيح name_جدول_دننه_واټن = (اوږدتیا + 3) & ~3;

  new_اوږدتیا = name_جدول_دننه_واټن + padded_name_جدول_اوږدتیا;
  فراغ *new_sfnt_معلومات = hb_calloc (1, new_اوږدتیا);
  إذا (!new_sfnt_معلومات)
    أرجع تش_اشاره;
  خپلکاری sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_معلومات); });

  hb_memcpy(new_sfnt_معلومات, orig_sfnt_معلومات, اوږدتیا);

  OT::name &name = Struct_At_دننه_واټن<OT::name> (new_sfnt_معلومات, name_جدول_دننه_واټن);
  name.format = 0;
  name.شمېر = ARRAY_اوږدتیا (name_IDs);
  name.string_دننه_واټن = name.get_اندازه ();
  لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (name_IDs); i++)
  {
    OT::Name_ریکارډ &ریکارډ = name.name_ریکارډ_Z[i];
    ریکارډ.platformID = 3;
    ریکارډ.encodingID = 1;
    ریکارډ.languageID = 0x0409u; /* English */
    ریکارډ.nameID = name_IDs[i];
    ریکارډ.اوږدتیا = name_str_len * 2;
    ریکارډ.دننه_واټن = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  بینښه حرف *p = &StructAfter<بینښه حرف> (name);
  لكل (بینښه صحيح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_معلومات);
  بینښه صحيح face_شمېر = file.get_face_شمېر ();
  لكل (بینښه صحيح face_شاخص = 0; face_شاخص < face_شمېر; face_شاخص++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_شاخص);
    بینښه صحيح شاخص_2;
    إذا (face.پیدا_کړه_جدول_شاخص (HB_OT_نښان_name, &شاخص_2))
    {
      OT::جدول_ریکارډ &ریکارډ = const_cast<OT::جدول_ریکارډ &> (face.get_جدول (شاخص_2));
      ریکارډ.check_مجموعه.سېټ_for_معلومات (&name, padded_name_جدول_اوږدتیا);
      ریکارډ.دننه_واټن = name_جدول_دننه_واټن;
      ریکارډ.اوږدتیا = name_جدول_اوږدتیا;
    }
    وإلا إذا (face_شاخص == 0) /* Fail if first face doesn't have 'name' table. */
      أرجع تش_اشاره;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) new_sfnt_معلومات, new_اوږدتیا,
			 HB_حافظه_ډېرپېښ_WRITABLE, new_sfnt_معلومات, hb_free);
}

hb_uniscribe_face_معلومات_t *
hb_uniscribe_shaper_face_معلومات_create (hb_face_t *face)
{
  hb_uniscribe_face_معلومات_t *معلومات = (hb_uniscribe_face_معلومات_t *) hb_calloc (1, حجم (hb_uniscribe_face_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع تش_اشاره;

  معلومات->funcs = hb_uniscribe_shaper_get_funcs ();
  إذا (unlikely (!معلومات->funcs))
  {
    hb_free (معلومات);
    أرجع تش_اشاره;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  إذا (unlikely (!hb_blob_get_اوږدتیا (blob)))
    DEBUG_پیغام (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, معلومات->face_name);
  إذا (unlikely (!blob))
  {
    hb_free (معلومات);
    أرجع تش_اشاره;
  }

  DWORD عدد_fonts_installed;
  معلومات->fh = جمع_کړه_Font_Mem_Resource_Ex ((فراغ *) hb_blob_get_معلومات (blob, تش_اشاره),
				   hb_blob_get_اوږدتیا (blob),
				   0, &عدد_fonts_installed);
  إذا (unlikely (!معلومات->fh))
  {
    DEBUG_پیغام (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (معلومات);
    أرجع تش_اشاره;
  }

  أرجع معلومات;
}

فراغ
hb_uniscribe_shaper_face_معلومات_destroy (hb_uniscribe_face_معلومات_t *معلومات)
{
  RemoveFontMemResourceEx (معلومات->fh);
  hb_free (معلومات);
}


/*
 * shaper font data
 */

بنية hb_uniscribe_font_معلومات_t
{
  HDC hdc;
  بدلیدونکی LOGFONTW log_font;
  HFONT hfont;
  بدلیدونکی SCRIPT_CACHE script_cache;
  مزدوج x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساكن منطقي
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   بینښه صحيح font_اندازه)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lf_لوړوالی = - (صحيح) font_اندازه;
  lf->lf_Char_سېټ = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->معلومات.uniscribe->face_name, حجم (lf->lfFaceName));

  أرجع ریښتیا;
}

hb_uniscribe_font_معلومات_t *
hb_uniscribe_shaper_font_معلومات_create (hb_font_t *font)
{
  hb_uniscribe_font_معلومات_t *معلومات = (hb_uniscribe_font_معلومات_t *) hb_calloc (1, حجم (hb_uniscribe_font_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع تش_اشاره;

  صحيح font_اندازه = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  إذا (font->y_ppem)
    font_اندازه = font->y_ppem;

  إذا (font_اندازه < 0)
    font_اندازه = -font_اندازه;
  معلومات->x_mult = (مزدوج) font->x_scale / font_اندازه;
  معلومات->y_mult = (مزدوج) font->y_scale / font_اندازه;

  معلومات->hdc = GetDC (تش_اشاره);

  إذا (unlikely (!populate_log_font (&معلومات->log_font, font, font_اندازه))) {
    DEBUG_پیغام (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
    أرجع تش_اشاره;
  }

  معلومات->hfont = Create_Font_غیرمستقیم_W (&معلومات->log_font);
  إذا (unlikely (!معلومات->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
     أرجع تش_اشاره;
  }

  إذا (!SelectObject (معلومات->hdc, معلومات->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
     أرجع تش_اشاره;
  }

  أرجع معلومات;
}

فراغ
hb_uniscribe_shaper_font_معلومات_destroy (hb_uniscribe_font_معلومات_t *معلومات)
{
  إذا (معلومات->hdc)
    ReleaseDC (تش_اشاره, معلومات->hdc);
  إذا (معلومات->hfont)
    DeleteObject (معلومات->hfont);
  إذا (معلومات->script_cache)
    ScriptFreeCache (&معلومات->script_cache);
  hb_free (معلومات);
}

/**
 * hb_uniscribe_font_get_logfontw:
 * @font: The #hb_font_t to work upon
 *
 * Fetches the LOGFONTW structure that corresponds to the
 * specified #hb_font_t font.
 *
 * Return value: a pointer to the LOGFONTW retrieved
 *
 **/
LOGFONTW *
hb_uniscribe_font_get_logfontw (hb_font_t *font)
{
  ثابت hb_uniscribe_font_معلومات_t *معلومات =  font->معلومات.uniscribe;
  أرجع معلومات ? &معلومات->log_font : تش_اشاره;
}

/**
 * hb_uniscribe_font_get_hfont:
 * @font: The #hb_font_t to work upon
 *
 * Fetches the HFONT handle that corresponds to the
 * specified #hb_font_t font.
 *
 * Return value: the HFONT retreieved
 *
 **/
HFONT
hb_uniscribe_font_get_hfont (hb_font_t *font)
{
  ثابت hb_uniscribe_font_معلومات_t *معلومات =  font->معلومات.uniscribe;
  أرجع معلومات ? معلومات->hfont : تش_اشاره;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     بینښه صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_معلومات_t *face_معلومات = face->معلومات.uniscribe;
  ثابت hb_uniscribe_font_معلومات_t *font_معلومات = font->معلومات.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_معلومات->funcs;

#تعریف_کړه FAIL(...) \
  HB_STMT_پیل { \
    DEBUG_پیغام (UNISCRIBE, تش_اشاره, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_پای_ټکی

  HRESULT hr;

retry:

  بینښه صحيح scratch_اندازه;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_اندازه);

#تعریف_کړه ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بینښه صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

#تعریف_کړه utf16_شاخص() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, بفر->len * 2);

  بینښه صحيح chars_len = 0;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_شاخص() = chars_len;
    إذا (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    وإلا {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (کلمه, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  إذا (عدد_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    لكل (بینښه صحيح i = 0; i < بفر->len; i++)
    {
      hb_codepoint_t c = بفر->info[i].codepoint;
      بینښه صحيح cluster = بفر->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      إذا (hb_دننه_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  بینښه صحيح glyphs_اندازه = (scratch_اندازه * حجم (صحيح) - 2)
			   / (حجم (کلمه) +
			      حجم (SCRIPT_GLYPHPROP) +
			      حجم (صحيح) +
			      حجم (GOFFSET) +
			      حجم (uint32_t));

  ALLOCATE_ARRAY (کلمه, glyphs, glyphs_اندازه);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_اندازه);
  ALLOCATE_ARRAY (صحيح, advances, glyphs_اندازه);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_اندازه);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_اندازه);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#تعریف_لغوه ALLOCATE_ARRAY

#تعریف_کړه MAX_توکي 256

  SCRIPT_توکی توکي[MAX_توکي + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_نښانونه[MAX_توکي];
  صحيح توکی_شمېر_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_توکي,
				     &bidi_control,
				     &bidi_state,
				     توکي,
				     script_نښانونه,
				     &توکی_شمېر_2);
  إذا (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (بینښه اوږد) hr);

#تعریف_لغوه MAX_توکي

  hb_نښان_t lang_نښان;
  بینښه صحيح lang_شمېر = 1;
  hb_ot_نښانونه_from_script_and_language (بفر->props.script,
				       بفر->props.language,
				       تش_اشاره, تش_اشاره,
				       &lang_شمېر, &lang_نښان);
  OPENTYPE_نښان language_نښان = hb_uint32_بدلون (lang_شمېر ? lang_نښان : HB_نښان_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_ریکارډ) == حجم (hb_ms_feature_t)), "");
  hb_ویکټور_t<hb_ms_feature_t> feature_ریکارډونه;
  hb_ویکټور_t<hb_ms_range_ریکارډ_t> range_ریکارډونه;
  منطقي has_features = خطأ;
  إذا (عدد_features)
    has_features = hb_ms_تیاری_features (features,
					 عدد_features,
					 feature_ریکارډونه,
					 range_ریکارډونه);

  hb_ویکټور_t<hb_ms_features_t*> range_properties;
  hb_ویکټور_t<uint32_t> range_char_counts;

  بینښه صحيح glyphs_دننه_واټن = 0;
  بینښه صحيح glyphs_len;
  منطقي backward = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);
  لكل (صحيح i = 0; i < توکی_شمېر_2; i++)
  {
    بینښه صحيح chars_دننه_واټن = توکي[i].iCharPos;
    بینښه صحيح توکی_chars_len = توکي[i + 1].iCharPos - chars_دننه_واټن;

    إذا (has_features)
      hb_ms_make_feature_ranges (feature_ریکارډونه,
				 range_ریکارډونه,
				 توکی_chars_len,
				 chars_دننه_واټن,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    توکي[i].a.f_Logical_ترتیب = ریښتیا;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_معلومات->hdc,
				     &font_معلومات->script_cache,
				     &توکي[i].a,
				     script_نښانونه[i],
				     language_نښان,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.اوږدتیا,
				     pchars + chars_دننه_واټن,
				     توکی_chars_len,
				     glyphs_اندازه - glyphs_دننه_واټن,
				     /* out */
				     log_clusters + chars_دننه_واټن,
				     char_props + chars_دننه_واټن,
				     glyphs + glyphs_دننه_واټن,
				     glyph_props + glyphs_دننه_واټن,
				     (صحيح *) &glyphs_len);

    إذا (unlikely (توکي[i].a.f_No_Glyph_شاخص))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    إذا (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_بفر))
    {
      إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
	FAIL ("Buffer resize failed");
      اذهب retry;
    }
    إذا (unlikely (hr == USP_E_SCRIPT_NOT_دننه_FONT))
    {
      إذا (توکي[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      توکي[i].a.eScript = SCRIPT_UNDEFINED;
      اذهب retry_shape;
    }
    إذا (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (بینښه اوږد) hr);
    }

    لكل (بینښه صحيح j = chars_دننه_واټن; j < chars_دننه_واټن + توکی_chars_len; j++)
      log_clusters[j] += glyphs_دننه_واټن;

    hr = funcs->Script_ځای_پرځای_کړه_Open_Type (font_معلومات->hdc,
				     &font_معلومات->script_cache,
				     &توکي[i].a,
				     script_نښانونه[i],
				     language_نښان,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.اوږدتیا,
				     pchars + chars_دننه_واټن,
				     log_clusters + chars_دننه_واټن,
				     char_props + chars_دننه_واټن,
				     توکی_chars_len,
				     glyphs + glyphs_دننه_واټن,
				     glyph_props + glyphs_دننه_واټن,
				     glyphs_len,
				     /* out */
				     advances + glyphs_دننه_واټن,
				     offsets + glyphs_دننه_واټن,
				     تش_اشاره);
    إذا (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (بینښه اوږد) hr);

    إذا (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       توکي[i].a.fRTL,
	       توکي[i].a.fLayoutRTL,
	       توکي[i].a.f_Logical_ترتیب,
	       HB_UNTAG (hb_uint32_بدلون (script_نښانونه[i])));

    glyphs_دننه_واټن += glyphs_len;
  }
  glyphs_len = glyphs_دننه_واټن;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  لكل (بینښه صحيح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[بفر->info[i].utf16_شاخص()]];
    *p = hb_min (*p, بفر->info[i].cluster);
  }
  لكل (بینښه صحيح i = 1; i < glyphs_len; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#تعریف_لغوه utf16_شاخص

  إذا (unlikely (!بفر->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#تعریف_لغوه FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بینښه صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ماسک = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  مزدوج x_mult = font_معلومات->x_mult, y_mult = font_معلومات->y_mult;
  لكل (بینښه صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (x_mult * (int32_t) info->ماسک);
    مزدوج x_دننه_واټن_2 = backward ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (x_mult * x_دننه_واټن_2);
    pos->y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (y_mult * info->var2.i32);
  }

  إذا (backward)
    hb_بفر_سرچپه (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع ریښتیا;
}


#د_که_پای
