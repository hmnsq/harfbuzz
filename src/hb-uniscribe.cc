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

#جے_تعریف_شدہ HAVE_UNISCRIBE

#جے_تعریف_شدہ HB_NO_OT_ٹیگ
#غلطی "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#جے_انت

#ضمن "hb-shaper-impl.hh"

#ضمن <windows.h>
#ضمن <usp10.h>
#ضمن <rpc.h>

#جے_بنا_تعریف E_NOT_SUFFICIENT_بفر
#تعریف_کرو E_NOT_SUFFICIENT_بفر HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)
#جے_انت

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
  ثابت WCHAR *pwc_وچ_Chars,
  صحيح c_وچ_Chars,
  صحيح c_Max_اشیاء,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شے *p_اشیاء,
  OPENTYPE_ٹیگ *p_Script_ٹیگ_سوچی,
  صحيح *pc_اشیاء
);

تعريفنوع HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٹیگ ٹیگ_Script,
  OPENTYPE_ٹیگ ٹیگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_باہرلا_Glyphs,
  SCRIPT_GLYPHPROP *p_باہرلا_Glyph_Props,
  صحيح *pcGlyphs
);

تعريفنوع HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٹیگ ٹیگ_Script,
  OPENTYPE_ٹیگ ٹیگ_Lang_Sys,
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
  ثابت WCHAR *pwc_وچ_Chars,
  صحيح c_وچ_Chars,
  صحيح c_Max_اشیاء,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شے *p_اشیاء,
  OPENTYPE_ٹیگ *p_Script_ٹیگ_سوچی,
  صحيح *pc_اشیاء
)
{
{
  أرجع ScriptItemize (pwc_وچ_Chars,
			c_وچ_Chars,
			c_Max_اشیاء,
			psControl,
			psState,
			p_اشیاء,
			pc_اشیاء);
}
}

ساكن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٹیگ ٹیگ_Script,
  OPENTYPE_ٹیگ ٹیگ_Lang_Sys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_باہرلا_Glyphs,
  SCRIPT_GLYPHPROP *p_باہرلا_Glyph_Props,
  صحيح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_باہرلا_Glyph_Props;
  أرجع ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_باہرلا_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ساكن HRESULT WINAPI
hb_Script_رکھو_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ٹیگ ٹیگ_Script,
  OPENTYPE_ٹیگ ٹیگ_Lang_Sys,
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
  أرجع Script_رکھو (hdc,
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
  SPOT Script_رکھو_Open_Type;

  فراغ init ()
  {
    HMODULE hinstLib;
    هذا->ScriptItemizeOpenType = خالی_سوچک;
    هذا->ScriptShapeOpenType   = خالی_سوچک;
    هذا->Script_رکھو_Open_Type   = خالی_سوچک;

    hinstLib = GetModuleHandle (متن_2 ("usp10.dll"));
    إذا (hinstLib)
    {
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-function-type"
      هذا->ScriptItemizeOpenType = (SIOT) Get_Proc_پتہ (hinstLib, "ScriptItemizeOpenType");
      هذا->ScriptShapeOpenType   = (SSOT) Get_Proc_پتہ (hinstLib, "ScriptShapeOpenType");
      هذا->Script_رکھو_Open_Type   = (SPOT) Get_Proc_پتہ (hinstLib, "ScriptPlaceOpenType");
#ہدایت GCC diagnostic نکالو
    }
    إذا (!هذا->ScriptItemizeOpenType ||
	!هذا->ScriptShapeOpenType   ||
	!هذا->Script_رکھو_Open_Type)
    {
      DEBUG_پیغام (UNISCRIBE, خالی_سوچک, "OpenType versions of functions not found; falling back.");
      هذا->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      هذا->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      هذا->Script_رکھو_Open_Type   = hb_Script_رکھو_Open_Type;
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
      أرجع خالی_سوچک;

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
    أرجع خالی_سوچک;
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

بنية hb_uniscribe_face_انکڑے_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ساكن فراغ
_hb_generate_unique_face_name (wchar_t *face_name, بنانشانی صحيح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  بنانشانی صحيح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  بنانشانی حرف *p = (بنانشانی حرف *) &id;
  لكل (بنانشانی صحيح i = 0; i < 16; i += 2)
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
_hb_rename_font (hb_blob_t *blob_وچ, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_وچ));

  بنانشانی صحيح لمائی, new_لمائی, name_str_len;
  ثابت حرف *orig_sfnt_انکڑے = hb_blob_get_انکڑے (blob, &لمائی);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساكن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  بنانشانی صحيح name_جدول_لمائی = OT::name::min_ناپ +
				   ARRAY_لمائی (name_IDs) * OT::Name_ریکارڈ::static_ناپ +
				   name_str_len * 2; /* for name data in UTF16BE form */
  بنانشانی صحيح padded_name_جدول_لمائی = ((name_جدول_لمائی + 3) & ~3);
  بنانشانی صحيح name_جدول_وتھ = (لمائی + 3) & ~3;

  new_لمائی = name_جدول_وتھ + padded_name_جدول_لمائی;
  فراغ *new_sfnt_انکڑے = hb_calloc (1, new_لمائی);
  إذا (!new_sfnt_انکڑے)
    أرجع خالی_سوچک;
  خودکار sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_انکڑے); });

  hb_memcpy(new_sfnt_انکڑے, orig_sfnt_انکڑے, لمائی);

  OT::name &name = Struct_At_وتھ<OT::name> (new_sfnt_انکڑے, name_جدول_وتھ);
  name.format = 0;
  name.گنتی = ARRAY_لمائی (name_IDs);
  name.string_وتھ = name.get_ناپ ();
  لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (name_IDs); i++)
  {
    OT::Name_ریکارڈ &ریکارڈ = name.name_ریکارڈ_Z[i];
    ریکارڈ.platformID = 3;
    ریکارڈ.encodingID = 1;
    ریکارڈ.languageID = 0x0409u; /* English */
    ریکارڈ.nameID = name_IDs[i];
    ریکارڈ.لمائی = name_str_len * 2;
    ریکارڈ.وتھ = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  بنانشانی حرف *p = &StructAfter<بنانشانی حرف> (name);
  لكل (بنانشانی صحيح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_انکڑے);
  بنانشانی صحيح face_گنتی = file.get_face_گنتی ();
  لكل (بنانشانی صحيح face_اشاریہ = 0; face_اشاریہ < face_گنتی; face_اشاریہ++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_اشاریہ);
    بنانشانی صحيح اشاریہ_2;
    إذا (face.لبھو_جدول_اشاریہ (HB_OT_ٹیگ_name, &اشاریہ_2))
    {
      OT::جدول_ریکارڈ &ریکارڈ = const_cast<OT::جدول_ریکارڈ &> (face.get_جدول (اشاریہ_2));
      ریکارڈ.check_جوڑ.سیٹ_for_انکڑے (&name, padded_name_جدول_لمائی);
      ریکارڈ.وتھ = name_جدول_وتھ;
      ریکارڈ.لمائی = name_جدول_لمائی;
    }
    وإلا إذا (face_اشاریہ == 0) /* Fail if first face doesn't have 'name' table. */
      أرجع خالی_سوچک;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  أرجع hb_blob_create ((ثابت حرف *) new_sfnt_انکڑے, new_لمائی,
			 HB_حافظہ_منوال_WRITABLE, new_sfnt_انکڑے, hb_free);
}

hb_uniscribe_face_انکڑے_t *
hb_uniscribe_shaper_face_انکڑے_create (hb_face_t *face)
{
  hb_uniscribe_face_انکڑے_t *انکڑے = (hb_uniscribe_face_انکڑے_t *) hb_calloc (1, حجم (hb_uniscribe_face_انکڑے_t));
  إذا (unlikely (!انکڑے))
    أرجع خالی_سوچک;

  انکڑے->funcs = hb_uniscribe_shaper_get_funcs ();
  إذا (unlikely (!انکڑے->funcs))
  {
    hb_free (انکڑے);
    أرجع خالی_سوچک;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  إذا (unlikely (!hb_blob_get_لمائی (blob)))
    DEBUG_پیغام (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, انکڑے->face_name);
  إذا (unlikely (!blob))
  {
    hb_free (انکڑے);
    أرجع خالی_سوچک;
  }

  DWORD عدد_fonts_installed;
  انکڑے->fh = جوڑو_Font_Mem_Resource_Ex ((فراغ *) hb_blob_get_انکڑے (blob, خالی_سوچک),
				   hb_blob_get_لمائی (blob),
				   0, &عدد_fonts_installed);
  إذا (unlikely (!انکڑے->fh))
  {
    DEBUG_پیغام (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (انکڑے);
    أرجع خالی_سوچک;
  }

  أرجع انکڑے;
}

فراغ
hb_uniscribe_shaper_face_انکڑے_destroy (hb_uniscribe_face_انکڑے_t *انکڑے)
{
  RemoveFontMemResourceEx (انکڑے->fh);
  hb_free (انکڑے);
}


/*
 * shaper font data
 */

بنية hb_uniscribe_font_انکڑے_t
{
  HDC hdc;
  بدلنجوگ LOGFONTW log_font;
  HFONT hfont;
  بدلنجوگ SCRIPT_CACHE script_cache;
  مزدوج x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساكن منطقي
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   بنانشانی صحيح font_ناپ)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lf_اچائی = - (صحيح) font_ناپ;
  lf->lf_Char_سیٹ = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->انکڑے.uniscribe->face_name, حجم (lf->lfFaceName));

  أرجع سچ;
}

hb_uniscribe_font_انکڑے_t *
hb_uniscribe_shaper_font_انکڑے_create (hb_font_t *font)
{
  hb_uniscribe_font_انکڑے_t *انکڑے = (hb_uniscribe_font_انکڑے_t *) hb_calloc (1, حجم (hb_uniscribe_font_انکڑے_t));
  إذا (unlikely (!انکڑے))
    أرجع خالی_سوچک;

  صحيح font_ناپ = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  إذا (font->y_ppem)
    font_ناپ = font->y_ppem;

  إذا (font_ناپ < 0)
    font_ناپ = -font_ناپ;
  انکڑے->x_mult = (مزدوج) font->x_scale / font_ناپ;
  انکڑے->y_mult = (مزدوج) font->y_scale / font_ناپ;

  انکڑے->hdc = GetDC (خالی_سوچک);

  إذا (unlikely (!populate_log_font (&انکڑے->log_font, font, font_ناپ))) {
    DEBUG_پیغام (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_انکڑے_destroy (انکڑے);
    أرجع خالی_سوچک;
  }

  انکڑے->hfont = Create_Font_اسدھا_W (&انکڑے->log_font);
  إذا (unlikely (!انکڑے->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_انکڑے_destroy (انکڑے);
     أرجع خالی_سوچک;
  }

  إذا (!SelectObject (انکڑے->hdc, انکڑے->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_انکڑے_destroy (انکڑے);
     أرجع خالی_سوچک;
  }

  أرجع انکڑے;
}

فراغ
hb_uniscribe_shaper_font_انکڑے_destroy (hb_uniscribe_font_انکڑے_t *انکڑے)
{
  إذا (انکڑے->hdc)
    ReleaseDC (خالی_سوچک, انکڑے->hdc);
  إذا (انکڑے->hfont)
    DeleteObject (انکڑے->hfont);
  إذا (انکڑے->script_cache)
    ScriptFreeCache (&انکڑے->script_cache);
  hb_free (انکڑے);
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
  ثابت hb_uniscribe_font_انکڑے_t *انکڑے =  font->انکڑے.uniscribe;
  أرجع انکڑے ? &انکڑے->log_font : خالی_سوچک;
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
  ثابت hb_uniscribe_font_انکڑے_t *انکڑے =  font->انکڑے.uniscribe;
  أرجع انکڑے ? انکڑے->hfont : خالی_سوچک;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     بنانشانی صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_انکڑے_t *face_انکڑے = face->انکڑے.uniscribe;
  ثابت hb_uniscribe_font_انکڑے_t *font_انکڑے = font->انکڑے.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_انکڑے->funcs;

#تعریف_کرو FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیغام (UNISCRIBE, خالی_سوچک, __VA_ARGS__); \
    أرجع خطأ; \
  } HB_STMT_سرا

  HRESULT hr;

retry:

  بنانشانی صحيح scratch_ناپ;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_ناپ);

#تعریف_کرو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشانی صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_ناپ); \
    scratch += _consumed; \
    scratch_ناپ -= _consumed; \
  } طالما (0)

#تعریف_کرو utf16_اشاریہ() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, بفر->len * 2);

  بنانشانی صحيح chars_len = 0;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاریہ() = chars_len;
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

  إذا (عدد_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
    {
      hb_codepoint_t c = بفر->info[i].codepoint;
      بنانشانی صحيح cluster = بفر->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      إذا (hb_وچ_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  بنانشانی صحيح glyphs_ناپ = (scratch_ناپ * حجم (صحيح) - 2)
			   / (حجم (لفظ) +
			      حجم (SCRIPT_GLYPHPROP) +
			      حجم (صحيح) +
			      حجم (GOFFSET) +
			      حجم (uint32_t));

  ALLOCATE_ARRAY (لفظ, glyphs, glyphs_ناپ);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_ناپ);
  ALLOCATE_ARRAY (صحيح, advances, glyphs_ناپ);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_ناپ);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_ناپ);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#تعریف_رد ALLOCATE_ARRAY

#تعریف_کرو MAX_اشیاء 256

  SCRIPT_شے اشیاء[MAX_اشیاء + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_ٹیگ_سوچی[MAX_اشیاء];
  صحيح شے_گنتی_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_اشیاء,
				     &bidi_control,
				     &bidi_state,
				     اشیاء,
				     script_ٹیگ_سوچی,
				     &شے_گنتی_2);
  إذا (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (بنانشانی لما) hr);

#تعریف_رد MAX_اشیاء

  hb_ٹیگ_t lang_ٹیگ;
  بنانشانی صحيح lang_گنتی = 1;
  hb_ot_ٹیگ_سوچی_from_script_and_language (بفر->props.script,
				       بفر->props.language,
				       خالی_سوچک, خالی_سوچک,
				       &lang_گنتی, &lang_ٹیگ);
  OPENTYPE_ٹیگ language_ٹیگ = hb_uint32_وٹاندرا (lang_گنتی ? lang_ٹیگ : HB_ٹیگ_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_ریکارڈ) == حجم (hb_ms_feature_t)), "");
  hb_سمتیہ_t<hb_ms_feature_t> feature_ریکارڈاں;
  hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> range_ریکارڈاں;
  منطقي has_features = خطأ;
  إذا (عدد_features)
    has_features = hb_ms_تیاری_features (features,
					 عدد_features,
					 feature_ریکارڈاں,
					 range_ریکارڈاں);

  hb_سمتیہ_t<hb_ms_features_t*> range_properties;
  hb_سمتیہ_t<uint32_t> range_char_counts;

  بنانشانی صحيح glyphs_وتھ = 0;
  بنانشانی صحيح glyphs_len;
  منطقي backward = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);
  لكل (صحيح i = 0; i < شے_گنتی_2; i++)
  {
    بنانشانی صحيح chars_وتھ = اشیاء[i].iCharPos;
    بنانشانی صحيح شے_chars_len = اشیاء[i + 1].iCharPos - chars_وتھ;

    إذا (has_features)
      hb_ms_make_feature_ranges (feature_ریکارڈاں,
				 range_ریکارڈاں,
				 شے_chars_len,
				 chars_وتھ,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    اشیاء[i].a.f_Logical_ترتیب = سچ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_انکڑے->hdc,
				     &font_انکڑے->script_cache,
				     &اشیاء[i].a,
				     script_ٹیگ_سوچی[i],
				     language_ٹیگ,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.لمائی,
				     pchars + chars_وتھ,
				     شے_chars_len,
				     glyphs_ناپ - glyphs_وتھ,
				     /* out */
				     log_clusters + chars_وتھ,
				     char_props + chars_وتھ,
				     glyphs + glyphs_وتھ,
				     glyph_props + glyphs_وتھ,
				     (صحيح *) &glyphs_len);

    إذا (unlikely (اشیاء[i].a.f_No_Glyph_اشاریہ))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    إذا (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_بفر))
    {
      إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
	FAIL ("Buffer resize failed");
      اذهب retry;
    }
    إذا (unlikely (hr == USP_E_SCRIPT_NOT_وچ_FONT))
    {
      إذا (اشیاء[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      اشیاء[i].a.eScript = SCRIPT_UNDEFINED;
      اذهب retry_shape;
    }
    إذا (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (بنانشانی لما) hr);
    }

    لكل (بنانشانی صحيح j = chars_وتھ; j < chars_وتھ + شے_chars_len; j++)
      log_clusters[j] += glyphs_وتھ;

    hr = funcs->Script_رکھو_Open_Type (font_انکڑے->hdc,
				     &font_انکڑے->script_cache,
				     &اشیاء[i].a,
				     script_ٹیگ_سوچی[i],
				     language_ٹیگ,
				     (صحيح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.لمائی,
				     pchars + chars_وتھ,
				     log_clusters + chars_وتھ,
				     char_props + chars_وتھ,
				     شے_chars_len,
				     glyphs + glyphs_وتھ,
				     glyph_props + glyphs_وتھ,
				     glyphs_len,
				     /* out */
				     advances + glyphs_وتھ,
				     offsets + glyphs_وتھ,
				     خالی_سوچک);
    إذا (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (بنانشانی لما) hr);

    إذا (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       اشیاء[i].a.fRTL,
	       اشیاء[i].a.fLayoutRTL,
	       اشیاء[i].a.f_Logical_ترتیب,
	       HB_UNTAG (hb_uint32_وٹاندرا (script_ٹیگ_سوچی[i])));

    glyphs_وتھ += glyphs_len;
  }
  glyphs_len = glyphs_وتھ;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  لكل (بنانشانی صحيح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[بفر->info[i].utf16_اشاریہ()]];
    *p = hb_min (*p, بفر->info[i].cluster);
  }
  لكل (بنانشانی صحيح i = 1; i < glyphs_len; i++)
    إذا (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#تعریف_رد utf16_اشاریہ

  إذا (unlikely (!بفر->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#تعریف_رد FAIL

  /* Set glyph infos */
  بفر->len = 0;
  لكل (بنانشانی صحيح i = 0; i < glyphs_len; i++)
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
  مزدوج x_mult = font_انکڑے->x_mult, y_mult = font_انکڑے->y_mult;
  لكل (بنانشانی صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_منزل<hb_position_t> (x_mult * (int32_t) info->نقاب);
    مزدوج x_وتھ_2 = backward ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_وتھ_2 = hb_clamp_منزل<hb_position_t> (x_mult * x_وتھ_2);
    pos->y_وتھ_2 = hb_clamp_منزل<hb_position_t> (y_mult * info->var2.i32);
  }

  إذا (backward)
    hb_بفر_الٹ (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_منزل_break ();

  /* Wow, done! */
  أرجع سچ;
}


#جے_انت
