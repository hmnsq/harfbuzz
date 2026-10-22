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

#اگر_تعریف_شده HAVE_UNISCRIBE

#اگر_تعریف_شده HB_NO_OT_برچسب
#خطا "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#پایان_اگر

#ضمن "hb-shaper-impl.hh"

#ضمن <windows.h>
#ضمن <usp10.h>
#ضمن <rpc.h>

#اگر_تعریف_نشده E_NOT_SUFFICIENT_میانگیر
#تعریف_کن E_NOT_SUFFICIENT_میانگیر HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_میانگیر)
#پایان_اگر

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
  ثابت WCHAR *pwc_درون_Chars,
  صحيح c_درون_Chars,
  صحيح c_Max_اقلام,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_قلم *p_اقلام,
  OPENTYPE_برچسب *p_Script_برچسبها,
  صحيح *pc_اقلام
);

تعريفنوع HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_برچسب برچسب_Script,
  OPENTYPE_برچسب برچسب_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  کلمه *pw_بیرون_Glyphs,
  SCRIPT_GLYPHPROP *p_بیرون_Glyph_Props,
  صحيح *pcGlyphs
);

تعريفنوع HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_برچسب برچسب_Script,
  OPENTYPE_برچسب برچسب_Lang_Sys,
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
  ثابت WCHAR *pwc_درون_Chars,
  صحيح c_درون_Chars,
  صحيح c_Max_اقلام,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_قلم *p_اقلام,
  OPENTYPE_برچسب *p_Script_برچسبها,
  صحيح *pc_اقلام
)
{
{
  أرجع ScriptItemize (pwc_درون_Chars,
			c_درون_Chars,
			c_Max_اقلام,
			psControl,
			psState,
			p_اقلام,
			pc_اقلام);
}
}

ساكن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_برچسب برچسب_Script,
  OPENTYPE_برچسب برچسب_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  کلمه *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  کلمه *pw_بیرون_Glyphs,
  SCRIPT_GLYPHPROP *p_بیرون_Glyph_Props,
  صحيح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_بیرون_Glyph_Props;
  أرجع ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_بیرون_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ساكن HRESULT WINAPI
hb_Script_قراربده_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_برچسب برچسب_Script,
  OPENTYPE_برچسب برچسب_Lang_Sys,
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
  أرجع Script_قراربده (hdc,
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
  SPOT Script_قراربده_Open_Type;

  فراغ init ()
  {
    HMODULE hinstLib;
    هذا->ScriptItemizeOpenType = اشاره_تهی;
    هذا->ScriptShapeOpenType   = اشاره_تهی;
    هذا->Script_قراربده_Open_Type   = اشاره_تهی;

    hinstLib = GetModuleHandle (متن_2 ("usp10.dll"));
    إذا (hinstLib)
    {
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wcast-function-type"
      هذا->ScriptItemizeOpenType = (SIOT) Get_Proc_آدرس (hinstLib, "ScriptItemizeOpenType");
      هذا->ScriptShapeOpenType   = (SSOT) Get_Proc_آدرس (hinstLib, "ScriptShapeOpenType");
      هذا->Script_قراربده_Open_Type   = (SPOT) Get_Proc_آدرس (hinstLib, "ScriptPlaceOpenType");
#رهنمود GCC diagnostic برداشت
    }
    إذا (!هذا->ScriptItemizeOpenType ||
	!هذا->ScriptShapeOpenType   ||
	!هذا->Script_قراربده_Open_Type)
    {
      DEBUG_پیام (UNISCRIBE, اشاره_تهی, "OpenType versions of functions not found; falling back.");
      هذا->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      هذا->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      هذا->Script_قراربده_Open_Type   = hb_Script_قراربده_Open_Type;
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
      أرجع اشاره_تهی;

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
    أرجع اشاره_تهی;
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

بنية hb_uniscribe_face_داده_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ساكن فراغ
_hb_generate_unique_face_name (wchar_t *face_name, بیعلامت صحيح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  بیعلامت صحيح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  بیعلامت حرف *p = (بیعلامت حرف *) &id;
  لكل (بیعلامت صحيح i = 0; i < 16; i += 2)
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
_hb_rename_font (hb_blob_t *blob_درون, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_درون));

  بیعلامت صحيح درازی, new_درازی, name_str_len;
  ثابت حرف *orig_sfnt_داده = hb_blob_get_داده (blob, &درازی);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساكن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  بیعلامت صحيح name_جدول_درازی = OT::name::min_اندازه +
				   ARRAY_درازی (name_IDs) * OT::Name_رکورد::static_اندازه +
				   name_str_len * 2; /* for name data in UTF16BE form */
  بیعلامت صحيح padded_name_جدول_درازی = ((name_جدول_درازی + 3) & ~3);
  بیعلامت صحيح name_جدول_جابه_جایی = (درازی + 3) & ~3;

  new_درازی = name_جدول_جابه_جایی + padded_name_جدول_درازی;
  فراغ *new_sfnt_داده = hb_calloc (1, new_درازی);
  إذا (!new_sfnt_داده)
    أرجع اشاره_تهی;
  خودکار sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_داده); });

  hb_memcpy(new_sfnt_داده, orig_sfnt_داده, درازی);

  OT::name &name = Struct_At_جابه_جایی<OT::name> (new_sfnt_داده, name_جدول_جابه_جایی);
  name.format = 0;
  name.شمار = ARRAY_درازی (name_IDs);
  name.string_جابه_جایی = name.get_اندازه ();
  لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (name_IDs); i++)
  {
    OT::Name_رکورد &رکورد = name.name_رکورد_Z[i];
    رکورد.platformID = 3;
    رکورد.encodingID = 1;
    رکورد.languageID = 0x0409u; /* English */
    رکورد.nameID = name_IDs[i];
    رکورد.درازی = name_str_len * 2;
    رکورد.جابه_جایی = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  بیعلامت حرف *p = &StructAfter<بیعلامت حرف> (name);
  لكل (بیعلامت صحيح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_داده);
  بیعلامت صحيح face_شمار = file.get_face_شمار ();
  لكل (بیعلامت صحيح face_شاخص = 0; face_شاخص < face_شمار; face_شاخص++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_شاخص);
    بیعلامت صحيح شاخص_2;
    إذا (face.پیدا_کن_جدول_شاخص (HB_OT_برچسب_name, &شاخص_2))
    {
      OT::جدول_رکورد &رکورد = const_cast<OT::جدول_رکورد &> (face.get_جدول (شاخص_2));
      رکورد.check_جمع.مجموعه_for_داده (&name, padded_name_جدول_درازی);
      رکورد.جابه_جایی = name_جدول_جابه_جایی;
      رکورد.درازی = name_جدول_درازی;
    }
    وإلا إذا (face_شاخص == 0) /* Fail if first face doesn't have 'name' table. */
      أرجع اشاره_تهی;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) new_sfnt_داده, new_درازی,
			 HB_حافظه_نما_WRITABLE, new_sfnt_داده, hb_free);
}

hb_uniscribe_face_داده_t *
hb_uniscribe_shaper_face_داده_create (hb_face_t *face)
{
  hb_uniscribe_face_داده_t *داده = (hb_uniscribe_face_داده_t *) hb_calloc (1, حجم (hb_uniscribe_face_داده_t));
  إذا (unlikely (!داده))
    أرجع اشاره_تهی;

  داده->funcs = hb_uniscribe_shaper_get_funcs ();
  إذا (unlikely (!داده->funcs))
  {
    hb_free (داده);
    أرجع اشاره_تهی;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  إذا (unlikely (!hb_blob_get_درازی (blob)))
    DEBUG_پیام (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, داده->face_name);
  إذا (unlikely (!blob))
  {
    hb_free (داده);
    أرجع اشاره_تهی;
  }

  DWORD رقم_fonts_installed;
  داده->fh = جمع_کن_Font_Mem_Resource_Ex ((فراغ *) hb_blob_get_داده (blob, اشاره_تهی),
				   hb_blob_get_درازی (blob),
				   0, &رقم_fonts_installed);
  إذا (unlikely (!داده->fh))
  {
    DEBUG_پیام (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (داده);
    أرجع اشاره_تهی;
  }

  أرجع داده;
}

فراغ
hb_uniscribe_shaper_face_داده_destroy (hb_uniscribe_face_داده_t *داده)
{
  RemoveFontMemResourceEx (داده->fh);
  hb_free (داده);
}


/*
 * shaper font data
 */

بنية hb_uniscribe_font_داده_t
{
  HDC hdc;
  تغییرپذیر LOGFONTW log_font;
  HFONT hfont;
  تغییرپذیر SCRIPT_CACHE script_cache;
  مزدوج x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساكن منطقي
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   بیعلامت صحيح font_اندازه)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lf_ارتفاع = - (صحيح) font_اندازه;
  lf->lf_Char_مجموعه = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->داده.uniscribe->face_name, حجم (lf->lfFaceName));

  أرجع درست;
}

hb_uniscribe_font_داده_t *
hb_uniscribe_shaper_font_داده_create (hb_font_t *font)
{
  hb_uniscribe_font_داده_t *داده = (hb_uniscribe_font_داده_t *) hb_calloc (1, حجم (hb_uniscribe_font_داده_t));
  إذا (unlikely (!داده))
    أرجع اشاره_تهی;

  صحيح font_اندازه = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  إذا (font->y_ppem)
    font_اندازه = font->y_ppem;

  إذا (font_اندازه < 0)
    font_اندازه = -font_اندازه;
  داده->x_mult = (مزدوج) font->x_scale / font_اندازه;
  داده->y_mult = (مزدوج) font->y_scale / font_اندازه;

  داده->hdc = GetDC (اشاره_تهی);

  إذا (unlikely (!populate_log_font (&داده->log_font, font, font_اندازه))) {
    DEBUG_پیام (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_داده_destroy (داده);
    أرجع اشاره_تهی;
  }

  داده->hfont = Create_Font_غیرمستقیم_W (&داده->log_font);
  إذا (unlikely (!داده->hfont)) {
    DEBUG_پیام (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_داده_destroy (داده);
     أرجع اشاره_تهی;
  }

  إذا (!SelectObject (داده->hdc, داده->hfont)) {
    DEBUG_پیام (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_داده_destroy (داده);
     أرجع اشاره_تهی;
  }

  أرجع داده;
}

فراغ
hb_uniscribe_shaper_font_داده_destroy (hb_uniscribe_font_داده_t *داده)
{
  إذا (داده->hdc)
    ReleaseDC (اشاره_تهی, داده->hdc);
  إذا (داده->hfont)
    DeleteObject (داده->hfont);
  إذا (داده->script_cache)
    ScriptFreeCache (&داده->script_cache);
  hb_free (داده);
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
  ثابت hb_uniscribe_font_داده_t *داده =  font->داده.uniscribe;
  أرجع داده ? &داده->log_font : اشاره_تهی;
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
  ثابت hb_uniscribe_font_داده_t *داده =  font->داده.uniscribe;
  أرجع داده ? داده->hfont : اشاره_تهی;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_میانگیر_t        *میانگیر,
		     ثابت hb_feature_t *features,
		     بیعلامت صحيح        رقم_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_داده_t *face_داده = face->داده.uniscribe;
  ثابت hb_uniscribe_font_داده_t *font_داده = font->داده.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_داده->funcs;

#تعریف_کن FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیام (UNISCRIBE, اشاره_تهی, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_انتها

  HRESULT hr;

retry:

  بیعلامت صحيح scratch_اندازه;
  hb_میانگیر_t::scratch_میانگیر_t *scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);

#تعریف_کن ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بیعلامت صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_اندازه); \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

#تعریف_کن utf16_شاخص() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, میانگیر->len * 2);

  بیعلامت صحيح chars_len = 0;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
  {
    hb_codepoint_t c = میانگیر->info[i].codepoint;
    میانگیر->info[i].utf16_شاخص() = chars_len;
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

  إذا (رقم_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
    {
      hb_codepoint_t c = میانگیر->info[i].codepoint;
      بیعلامت صحيح cluster = میانگیر->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      إذا (hb_درون_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  بیعلامت صحيح glyphs_اندازه = (scratch_اندازه * حجم (صحيح) - 2)
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

#لغو_تعریف ALLOCATE_ARRAY

#تعریف_کن MAX_اقلام 256

  SCRIPT_قلم اقلام[MAX_اقلام + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_برچسبها[MAX_اقلام];
  صحيح قلم_شمار_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (میانگیر->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_اقلام,
				     &bidi_control,
				     &bidi_state,
				     اقلام,
				     script_برچسبها,
				     &قلم_شمار_2);
  إذا (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (بیعلامت دراز) hr);

#لغو_تعریف MAX_اقلام

  hb_برچسب_t lang_برچسب;
  بیعلامت صحيح lang_شمار = 1;
  hb_ot_برچسبها_from_script_and_language (میانگیر->props.script,
				       میانگیر->props.language,
				       اشاره_تهی, اشاره_تهی,
				       &lang_شمار, &lang_برچسب);
  OPENTYPE_برچسب language_برچسب = hb_uint32_جابجا (lang_شمار ? lang_برچسب : HB_برچسب_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_رکورد) == حجم (hb_ms_feature_t)), "");
  hb_وکتور_t<hb_ms_feature_t> feature_رکوردها;
  hb_وکتور_t<hb_ms_range_رکورد_t> range_رکوردها;
  منطقي has_features = خطأ;
  إذا (رقم_features)
    has_features = hb_ms_راهاندازی_features (features,
					 رقم_features,
					 feature_رکوردها,
					 range_رکوردها);

  hb_وکتور_t<hb_ms_features_t*> range_properties;
  hb_وکتور_t<uint32_t> range_char_counts;

  بیعلامت صحيح glyphs_جابه_جایی = 0;
  بیعلامت صحيح glyphs_len;
  منطقي backward = HB_DIRECTION_IS_BACKWARD (میانگیر->props.direction);
  لكل (صحيح i = 0; i < قلم_شمار_2; i++)
  {
    بیعلامت صحيح chars_جابه_جایی = اقلام[i].iCharPos;
    بیعلامت صحيح قلم_chars_len = اقلام[i + 1].iCharPos - chars_جابه_جایی;

    إذا (has_features)
      hb_ms_make_feature_ranges (feature_رکوردها,
				 range_رکوردها,
				 قلم_chars_len,
				 chars_جابه_جایی,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    اقلام[i].a.f_Logical_ترتیب = درست;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_داده->hdc,
				     &font_داده->script_cache,
				     &اقلام[i].a,
				     script_برچسبها[i],
				     language_برچسب,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.درازی,
				     pchars + chars_جابه_جایی,
				     قلم_chars_len,
				     glyphs_اندازه - glyphs_جابه_جایی,
				     /* out */
				     log_clusters + chars_جابه_جایی,
				     char_props + chars_جابه_جایی,
				     glyphs + glyphs_جابه_جایی,
				     glyph_props + glyphs_جابه_جایی,
				     (صحيح *) &glyphs_len);

    إذا (unlikely (اقلام[i].a.f_No_Glyph_شاخص))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    إذا (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_میانگیر))
    {
      إذا (unlikely (!میانگیر->ensure (میانگیر->allocated * 2)))
	FAIL ("Buffer resize failed");
      اذهب retry;
    }
    إذا (unlikely (hr == USP_E_SCRIPT_NOT_درون_FONT))
    {
      إذا (اقلام[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      اقلام[i].a.eScript = SCRIPT_UNDEFINED;
      اذهب retry_shape;
    }
    إذا (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (بیعلامت دراز) hr);
    }

    لكل (بیعلامت صحيح j = chars_جابه_جایی; j < chars_جابه_جایی + قلم_chars_len; j++)
      log_clusters[j] += glyphs_جابه_جایی;

    hr = funcs->Script_قراربده_Open_Type (font_داده->hdc,
				     &font_داده->script_cache,
				     &اقلام[i].a,
				     script_برچسبها[i],
				     language_برچسب,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.درازی,
				     pchars + chars_جابه_جایی,
				     log_clusters + chars_جابه_جایی,
				     char_props + chars_جابه_جایی,
				     قلم_chars_len,
				     glyphs + glyphs_جابه_جایی,
				     glyph_props + glyphs_جابه_جایی,
				     glyphs_len,
				     /* out */
				     advances + glyphs_جابه_جایی,
				     offsets + glyphs_جابه_جایی,
				     اشاره_تهی);
    إذا (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (بیعلامت دراز) hr);

    إذا (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       اقلام[i].a.fRTL,
	       اقلام[i].a.fLayoutRTL,
	       اقلام[i].a.f_Logical_ترتیب,
	       HB_UNTAG (hb_uint32_جابجا (script_برچسبها[i])));

    glyphs_جابه_جایی += glyphs_len;
  }
  glyphs_len = glyphs_جابه_جایی;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  لكل (بیعلامت صحيح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[میانگیر->info[i].utf16_شاخص()]];
    *p = hb_min (*p, میانگیر->info[i].cluster);
  }
  لكل (بیعلامت صحيح i = 1; i < glyphs_len; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#لغو_تعریف utf16_شاخص

  إذا (unlikely (!میانگیر->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#لغو_تعریف FAIL

  /* Set glyph infos */
  میانگیر->len = 0;
  لكل (بیعلامت صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &میانگیر->info[میانگیر->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ماسک = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  میانگیر->clear_positions ();
  مزدوج x_mult = font_داده->x_mult, y_mult = font_داده->y_mult;
  لكل (بیعلامت صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &میانگیر->info[i];
    hb_glyph_position_t *pos = &میانگیر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_مقصد<hb_position_t> (x_mult * (int32_t) info->ماسک);
    مزدوج x_جابه_جایی_2 = backward ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (x_mult * x_جابه_جایی_2);
    pos->y_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (y_mult * info->var2.i32);
  }

  إذا (backward)
    hb_میانگیر_معکوس (میانگیر);

  میانگیر->clear_glyph_flags ();
  میانگیر->unsafe_مقصد_break ();

  /* Wow, done! */
  أرجع درست;
}


#پایان_اگر
