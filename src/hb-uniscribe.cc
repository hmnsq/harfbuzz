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

#جيڪڏهن_وصف_ٿيل HAVE_UNISCRIBE

#جيڪڏهن_وصف_ٿيل HB_NO_OT_ٽيگ
#غلطي "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#جيڪڏهن_پورو

#ضمن "hb-shaper-impl.hh"

#ضمن <windows.h>
#ضمن <usp10.h>
#ضمن <rpc.h>

#جيڪڏهن_وصف_نه_ٿيل E_NOT_SUFFICIENT_بفر
#وصف_ڪريو E_NOT_SUFFICIENT_بفر HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)
#جيڪڏهن_پورو

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
  ثابت WCHAR *pwc_اندريون_Chars,
  صحيح c_اندريون_Chars,
  صحيح c_Max_شيون,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شي *p_شيون,
  OPENTYPE_ٽيگ *p_Script_ٽيگ_فهرست,
  صحيح *pc_شيون
);

تعريفنوع HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٽيگ ٽيگ_Script,
  OPENTYPE_ٽيگ ٽيگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_ٻاهريون_Glyphs,
  SCRIPT_GLYPHPROP *p_ٻاهريون_Glyph_Props,
  صحيح *pcGlyphs
);

تعريفنوع HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٽيگ ٽيگ_Script,
  OPENTYPE_ٽيگ ٽيگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت لفظ *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ساكن HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ثابت WCHAR *pwc_اندريون_Chars,
  صحيح c_اندريون_Chars,
  صحيح c_Max_شيون,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شي *p_شيون,
  OPENTYPE_ٽيگ *p_Script_ٽيگ_فهرست,
  صحيح *pc_شيون
)
{
{
  أرجع ScriptItemize (pwc_اندريون_Chars,
			c_اندريون_Chars,
			c_Max_شيون,
			psControl,
			psState,
			p_شيون,
			pc_شيون);
}
}

ساكن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٽيگ ٽيگ_Script,
  OPENTYPE_ٽيگ ٽيگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_ٻاهريون_Glyphs,
  SCRIPT_GLYPHPROP *p_ٻاهريون_Glyph_Props,
  صحيح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_ٻاهريون_Glyph_Props;
  أرجع ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_ٻاهريون_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ساكن HRESULT WINAPI
hb_Script_رکو_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٽيگ ٽيگ_Script,
  OPENTYPE_ٽيگ ٽيگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت لفظ *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  أرجع Script_رکو (hdc,
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
  SPOT Script_رکو_Open_Type;

  فراغ init ()
  {
    HMODULE hinstLib;
    هذا->ScriptItemizeOpenType = خالي_اشارو;
    هذا->ScriptShapeOpenType   = خالي_اشارو;
    هذا->Script_رکو_Open_Type   = خالي_اشارو;

    hinstLib = GetModuleHandle (متن_2 ("usp10.dll"));
    إذا (hinstLib)
    {
#هدايت GCC diagnostic push
#هدايت GCC diagnostic ignored "-Wcast-function-type"
      هذا->ScriptItemizeOpenType = (SIOT) Get_Proc_پتو (hinstLib, "ScriptItemizeOpenType");
      هذا->ScriptShapeOpenType   = (SSOT) Get_Proc_پتو (hinstLib, "ScriptShapeOpenType");
      هذا->Script_رکو_Open_Type   = (SPOT) Get_Proc_پتو (hinstLib, "ScriptPlaceOpenType");
#هدايت GCC diagnostic ڪڍو
    }
    إذا (!هذا->ScriptItemizeOpenType ||
	!هذا->ScriptShapeOpenType   ||
	!هذا->Script_رکو_Open_Type)
    {
      DEBUG_نياپو (UNISCRIBE, خالي_اشارو, "OpenType versions of functions not found; falling back.");
      هذا->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      هذا->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      هذا->Script_رکو_Open_Type   = hb_Script_رکو_Open_Type;
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
      أرجع خالي_اشارو;

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
    أرجع خالي_اشارو;
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
_hb_generate_unique_face_name (wchar_t *face_name, بنانشان صحيح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  بنانشان صحيح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  بنانشان حرف *p = (بنانشان حرف *) &id;
  لكل (بنانشان صحيح i = 0; i < 16; i += 2)
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
_hb_rename_font (hb_blob_t *blob_اندريون, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_اندريون));

  بنانشان صحيح ڊيگهائي, new_ڊيگهائي, name_str_len;
  ثابت حرف *orig_sfnt_معلومات = hb_blob_get_معلومات (blob, &ڊيگهائي);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساكن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  بنانشان صحيح name_جدول_ڊيگهائي = OT::name::min_سائيز +
				   ARRAY_ڊيگهائي (name_IDs) * OT::Name_رڪارڊ::static_سائيز +
				   name_str_len * 2; /* for name data in UTF16BE form */
  بنانشان صحيح padded_name_جدول_ڊيگهائي = ((name_جدول_ڊيگهائي + 3) & ~3);
  بنانشان صحيح name_جدول_وٿي = (ڊيگهائي + 3) & ~3;

  new_ڊيگهائي = name_جدول_وٿي + padded_name_جدول_ڊيگهائي;
  فراغ *new_sfnt_معلومات = hb_calloc (1, new_ڊيگهائي);
  إذا (!new_sfnt_معلومات)
    أرجع خالي_اشارو;
  خودڪار sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_معلومات); });

  hb_memcpy(new_sfnt_معلومات, orig_sfnt_معلومات, ڊيگهائي);

  OT::name &name = Struct_At_وٿي<OT::name> (new_sfnt_معلومات, name_جدول_وٿي);
  name.format = 0;
  name.ڳڻپ = ARRAY_ڊيگهائي (name_IDs);
  name.string_وٿي = name.get_سائيز ();
  لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (name_IDs); i++)
  {
    OT::Name_رڪارڊ &رڪارڊ = name.name_رڪارڊ_Z[i];
    رڪارڊ.platformID = 3;
    رڪارڊ.encodingID = 1;
    رڪارڊ.languageID = 0x0409u; /* English */
    رڪارڊ.nameID = name_IDs[i];
    رڪارڊ.ڊيگهائي = name_str_len * 2;
    رڪارڊ.وٿي = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  بنانشان حرف *p = &StructAfter<بنانشان حرف> (name);
  لكل (بنانشان صحيح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_معلومات);
  بنانشان صحيح face_ڳڻپ = file.get_face_ڳڻپ ();
  لكل (بنانشان صحيح face_اشاريو = 0; face_اشاريو < face_ڳڻپ; face_اشاريو++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_اشاريو);
    بنانشان صحيح اشاريو_2;
    إذا (face.ڳوليو_جدول_اشاريو (HB_OT_ٽيگ_name, &اشاريو_2))
    {
      OT::جدول_رڪارڊ &رڪارڊ = const_cast<OT::جدول_رڪارڊ &> (face.get_جدول (اشاريو_2));
      رڪارڊ.check_جوڙ.سيٽ_for_معلومات (&name, padded_name_جدول_ڊيگهائي);
      رڪارڊ.وٿي = name_جدول_وٿي;
      رڪارڊ.ڊيگهائي = name_جدول_ڊيگهائي;
    }
    وإلا إذا (face_اشاريو == 0) /* Fail if first face doesn't have 'name' table. */
      أرجع خالي_اشارو;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) new_sfnt_معلومات, new_ڊيگهائي,
			 HB_يادگيري_گهڻائي_WRITABLE, new_sfnt_معلومات, hb_free);
}

hb_uniscribe_face_معلومات_t *
hb_uniscribe_shaper_face_معلومات_create (hb_face_t *face)
{
  hb_uniscribe_face_معلومات_t *معلومات = (hb_uniscribe_face_معلومات_t *) hb_calloc (1, حجم (hb_uniscribe_face_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع خالي_اشارو;

  معلومات->funcs = hb_uniscribe_shaper_get_funcs ();
  إذا (unlikely (!معلومات->funcs))
  {
    hb_free (معلومات);
    أرجع خالي_اشارو;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  إذا (unlikely (!hb_blob_get_ڊيگهائي (blob)))
    DEBUG_نياپو (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, معلومات->face_name);
  إذا (unlikely (!blob))
  {
    hb_free (معلومات);
    أرجع خالي_اشارو;
  }

  DWORD انگ_fonts_installed;
  معلومات->fh = جوڙيو_Font_Mem_Resource_Ex ((فراغ *) hb_blob_get_معلومات (blob, خالي_اشارو),
				   hb_blob_get_ڊيگهائي (blob),
				   0, &انگ_fonts_installed);
  إذا (unlikely (!معلومات->fh))
  {
    DEBUG_نياپو (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (معلومات);
    أرجع خالي_اشارو;
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
  بدلجندڙ LOGFONTW log_font;
  HFONT hfont;
  بدلجندڙ SCRIPT_CACHE script_cache;
  مزدوج x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساكن منطقي
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   بنانشان صحيح font_سائيز)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lf_اوچائي = - (صحيح) font_سائيز;
  lf->lf_Char_سيٽ = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->معلومات.uniscribe->face_name, حجم (lf->lfFaceName));

  أرجع سچ;
}

hb_uniscribe_font_معلومات_t *
hb_uniscribe_shaper_font_معلومات_create (hb_font_t *font)
{
  hb_uniscribe_font_معلومات_t *معلومات = (hb_uniscribe_font_معلومات_t *) hb_calloc (1, حجم (hb_uniscribe_font_معلومات_t));
  إذا (unlikely (!معلومات))
    أرجع خالي_اشارو;

  صحيح font_سائيز = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  إذا (font->y_ppem)
    font_سائيز = font->y_ppem;

  إذا (font_سائيز < 0)
    font_سائيز = -font_سائيز;
  معلومات->x_mult = (مزدوج) font->x_scale / font_سائيز;
  معلومات->y_mult = (مزدوج) font->y_scale / font_سائيز;

  معلومات->hdc = GetDC (خالي_اشارو);

  إذا (unlikely (!populate_log_font (&معلومات->log_font, font, font_سائيز))) {
    DEBUG_نياپو (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
    أرجع خالي_اشارو;
  }

  معلومات->hfont = Create_Font_اڻسڌو_W (&معلومات->log_font);
  إذا (unlikely (!معلومات->hfont)) {
    DEBUG_نياپو (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
     أرجع خالي_اشارو;
  }

  إذا (!SelectObject (معلومات->hdc, معلومات->hfont)) {
    DEBUG_نياپو (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_معلومات_destroy (معلومات);
     أرجع خالي_اشارو;
  }

  أرجع معلومات;
}

فراغ
hb_uniscribe_shaper_font_معلومات_destroy (hb_uniscribe_font_معلومات_t *معلومات)
{
  إذا (معلومات->hdc)
    ReleaseDC (خالي_اشارو, معلومات->hdc);
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
  أرجع معلومات ? &معلومات->log_font : خالي_اشارو;
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
  أرجع معلومات ? معلومات->hfont : خالي_اشارو;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     بنانشان صحيح        انگ_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_معلومات_t *face_معلومات = face->معلومات.uniscribe;
  ثابت hb_uniscribe_font_معلومات_t *font_معلومات = font->معلومات.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_معلومات->funcs;

#وصف_ڪريو FAIL(...) \
  HB_STMT_شروعات { \
    DEBUG_نياپو (UNISCRIBE, خالي_اشارو, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_پڇاڙي

  HRESULT hr;

retry:

  بنانشان صحيح scratch_سائيز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائيز);

#وصف_ڪريو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشان صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_سائيز); \
    scratch += _consumed; \
    scratch_سائيز -= _consumed; \
  } طالما (0)

#وصف_ڪريو utf16_اشاريو() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, بفر->len * 2);

  بنانشان صحيح chars_len = 0;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاريو() = chars_len;
    إذا (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    وإلا {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (لفظ, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  إذا (انگ_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
    {
      hb_codepoint_t c = بفر->info[i].codepoint;
      بنانشان صحيح cluster = بفر->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      إذا (hb_اندريون_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  بنانشان صحيح glyphs_سائيز = (scratch_سائيز * حجم (صحيح) - 2)
			   / (حجم (لفظ) +
			      حجم (SCRIPT_GLYPHPROP) +
			      حجم (صحيح) +
			      حجم (GOFFSET) +
			      حجم (uint32_t));

  ALLOCATE_ARRAY (لفظ, glyphs, glyphs_سائيز);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_سائيز);
  ALLOCATE_ARRAY (صحيح, advances, glyphs_سائيز);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_سائيز);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_سائيز);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#وصف_رد ALLOCATE_ARRAY

#وصف_ڪريو MAX_شيون 256

  SCRIPT_شي شيون[MAX_شيون + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_ٽيگ_فهرست[MAX_شيون];
  صحيح شي_ڳڻپ_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_شيون,
				     &bidi_control,
				     &bidi_state,
				     شيون,
				     script_ٽيگ_فهرست,
				     &شي_ڳڻپ_2);
  إذا (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (بنانشان ڊگهو) hr);

#وصف_رد MAX_شيون

  hb_ٽيگ_t lang_ٽيگ;
  بنانشان صحيح lang_ڳڻپ = 1;
  hb_ot_ٽيگ_فهرست_from_script_and_language (بفر->props.script,
				       بفر->props.language,
				       خالي_اشارو, خالي_اشارو,
				       &lang_ڳڻپ, &lang_ٽيگ);
  OPENTYPE_ٽيگ language_ٽيگ = hb_uint32_مٽا_سٽا (lang_ڳڻپ ? lang_ٽيگ : HB_ٽيگ_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_رڪارڊ) == حجم (hb_ms_feature_t)), "");
  hb_ويڪٽر_t<hb_ms_feature_t> feature_رڪارڊون;
  hb_ويڪٽر_t<hb_ms_range_رڪارڊ_t> range_رڪارڊون;
  منطقي has_features = خطأ;
  إذا (انگ_features)
    has_features = hb_ms_تياري_features (features,
					 انگ_features,
					 feature_رڪارڊون,
					 range_رڪارڊون);

  hb_ويڪٽر_t<hb_ms_features_t*> range_properties;
  hb_ويڪٽر_t<uint32_t> range_char_counts;

  بنانشان صحيح glyphs_وٿي = 0;
  بنانشان صحيح glyphs_len;
  منطقي backward = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);
  لكل (صحيح i = 0; i < شي_ڳڻپ_2; i++)
  {
    بنانشان صحيح chars_وٿي = شيون[i].iCharPos;
    بنانشان صحيح شي_chars_len = شيون[i + 1].iCharPos - chars_وٿي;

    إذا (has_features)
      hb_ms_make_feature_ranges (feature_رڪارڊون,
				 range_رڪارڊون,
				 شي_chars_len,
				 chars_وٿي,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    شيون[i].a.f_Logical_ترتيب = سچ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_معلومات->hdc,
				     &font_معلومات->script_cache,
				     &شيون[i].a,
				     script_ٽيگ_فهرست[i],
				     language_ٽيگ,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.ڊيگهائي,
				     pchars + chars_وٿي,
				     شي_chars_len,
				     glyphs_سائيز - glyphs_وٿي,
				     /* out */
				     log_clusters + chars_وٿي,
				     char_props + chars_وٿي,
				     glyphs + glyphs_وٿي,
				     glyph_props + glyphs_وٿي,
				     (صحيح *) &glyphs_len);

    إذا (unlikely (شيون[i].a.f_No_Glyph_اشاريو))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    إذا (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_بفر))
    {
      إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
	FAIL ("Buffer resize failed");
      اذهب retry;
    }
    إذا (unlikely (hr == USP_E_SCRIPT_NOT_اندريون_FONT))
    {
      إذا (شيون[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      شيون[i].a.eScript = SCRIPT_UNDEFINED;
      اذهب retry_shape;
    }
    إذا (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (بنانشان ڊگهو) hr);
    }

    لكل (بنانشان صحيح j = chars_وٿي; j < chars_وٿي + شي_chars_len; j++)
      log_clusters[j] += glyphs_وٿي;

    hr = funcs->Script_رکو_Open_Type (font_معلومات->hdc,
				     &font_معلومات->script_cache,
				     &شيون[i].a,
				     script_ٽيگ_فهرست[i],
				     language_ٽيگ,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.ڊيگهائي,
				     pchars + chars_وٿي,
				     log_clusters + chars_وٿي,
				     char_props + chars_وٿي,
				     شي_chars_len,
				     glyphs + glyphs_وٿي,
				     glyph_props + glyphs_وٿي,
				     glyphs_len,
				     /* out */
				     advances + glyphs_وٿي,
				     offsets + glyphs_وٿي,
				     خالي_اشارو);
    إذا (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (بنانشان ڊگهو) hr);

    إذا (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       شيون[i].a.fRTL,
	       شيون[i].a.fLayoutRTL,
	       شيون[i].a.f_Logical_ترتيب,
	       HB_UNTAG (hb_uint32_مٽا_سٽا (script_ٽيگ_فهرست[i])));

    glyphs_وٿي += glyphs_len;
  }
  glyphs_len = glyphs_وٿي;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  لكل (بنانشان صحيح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[بفر->info[i].utf16_اشاريو()]];
    *p = hb_min (*p, بفر->info[i].cluster);
  }
  لكل (بنانشان صحيح i = 1; i < glyphs_len; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#وصف_رد utf16_اشاريو

  إذا (unlikely (!بفر->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#وصف_رد FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بنانشان صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[بفر->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->نقاب = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  بفر->clear_positions ();
  مزدوج x_mult = font_معلومات->x_mult, y_mult = font_معلومات->y_mult;
  لكل (بنانشان صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (x_mult * (int32_t) info->نقاب);
    مزدوج x_وٿي_2 = backward ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_وٿي_2 = hb_clamp_منزل<hb_position_t> (x_mult * x_وٿي_2);
    pos->y_وٿي_2 = hb_clamp_منزل<hb_position_t> (y_mult * info->var2.i32);
  }

  إذا (backward)
    hb_بفر_ابتو (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع سچ;
}


#جيڪڏهن_پورو
