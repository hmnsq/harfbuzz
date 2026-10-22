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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_UNISCRIBE

#اگر_تعریف_شدہ HB_NO_OT_لیبل
#غلطی "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#اگر_ختم

#شامل "hb-shaper-impl.hh"

#شامل <windows.h>
#شامل <usp10.h>
#شامل <rpc.h>

#اگر_غیر_تعریف_شدہ E_NOT_SUFFICIENT_بفر
#تعریف_کرو E_NOT_SUFFICIENT_بفر HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_بفر)
#اگر_ختم

#شامل "hb-uniscribe.h"

#شامل "hb-ms-feature-ranges.hh"
#شامل "hb-open-file.hh"
#شامل "hb-ot-name-table.hh"
#شامل "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

قسمتعریف HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ثابت WCHAR *pwc_اندر_Chars,
  صحیح c_اندر_Chars,
  صحیح c_Max_اشیاء,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شے *p_اشیاء,
  OPENTYPE_لیبل *p_Script_لیبل_فہرست,
  صحیح *pc_اشیاء
);

قسمتعریف HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_لیبل لیبل_Script,
  OPENTYPE_لیبل لیبل_Lang_Sys,
  صحیح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحیح cRanges,
  ثابت WCHAR *pwcChars,
  صحیح cChars,
  صحیح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_برآمد_Glyphs,
  SCRIPT_GLYPHPROP *p_برآمد_Glyph_Props,
  صحیح *pcGlyphs
);

قسمتعریف HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_لیبل لیبل_Script,
  OPENTYPE_لیبل لیبل_Lang_Sys,
  صحیح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحیح cRanges,
  ثابت WCHAR *pwcChars,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحیح cChars,
  ثابت لفظ *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحیح cGlyphs,
  صحیح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ساکن HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ثابت WCHAR *pwc_اندر_Chars,
  صحیح c_اندر_Chars,
  صحیح c_Max_اشیاء,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_شے *p_اشیاء,
  OPENTYPE_لیبل *p_Script_لیبل_فہرست,
  صحیح *pc_اشیاء
)
{
{
  واپس ScriptItemize (pwc_اندر_Chars,
			c_اندر_Chars,
			c_Max_اشیاء,
			psControl,
			psState,
			p_اشیاء,
			pc_اشیاء);
}
}

ساکن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_لیبل لیبل_Script,
  OPENTYPE_لیبل لیبل_Lang_Sys,
  صحیح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحیح cRanges,
  ثابت WCHAR *pwcChars,
  صحیح cChars,
  صحیح cMaxGlyphs,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  لفظ *pw_برآمد_Glyphs,
  SCRIPT_GLYPHPROP *p_برآمد_Glyph_Props,
  صحیح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_برآمد_Glyph_Props;
  واپس ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_برآمد_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ساکن HRESULT WINAPI
hb_Script_رکھو_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_لیبل لیبل_Script,
  OPENTYPE_لیبل لیبل_Lang_Sys,
  صحیح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحیح cRanges,
  ثابت WCHAR *pwcChars,
  لفظ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحیح cChars,
  ثابت لفظ *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحیح cGlyphs,
  صحیح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  واپس Script_رکھو (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


ساخت hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT Script_رکھو_Open_Type;

  خالی init ()
  {
    HMODULE hinstLib;
    یہ->ScriptItemizeOpenType = خالی_اشاریہ;
    یہ->ScriptShapeOpenType   = خالی_اشاریہ;
    یہ->Script_رکھو_Open_Type   = خالی_اشاریہ;

    hinstLib = GetModuleHandle (متن_2 ("usp10.dll"));
    اگر (hinstLib)
    {
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wcast-function-type"
      یہ->ScriptItemizeOpenType = (SIOT) Get_Proc_پتہ (hinstLib, "ScriptItemizeOpenType");
      یہ->ScriptShapeOpenType   = (SSOT) Get_Proc_پتہ (hinstLib, "ScriptShapeOpenType");
      یہ->Script_رکھو_Open_Type   = (SPOT) Get_Proc_پتہ (hinstLib, "ScriptPlaceOpenType");
#ہدایت GCC diagnostic نکالو
    }
    اگر (!یہ->ScriptItemizeOpenType ||
	!یہ->ScriptShapeOpenType   ||
	!یہ->Script_رکھو_Open_Type)
    {
      DEBUG_پیغام (UNISCRIBE, خالی_اشاریہ, "OpenType versions of functions not found; falling back.");
      یہ->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      یہ->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      یہ->Script_رکھو_Open_Type   = hb_Script_رکھو_Open_Type;
    }
  }
};

ساکن اندرونی خالی free_static_uniscribe_shaper_funcs ();

ساکن ساخت hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  ساکن hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, حجم (hb_uniscribe_shaper_funcs_t));
    اگر (unlikely (!funcs))
      واپس خالی_اشاریہ;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    واپس funcs;
  }
  ساکن خالی destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((خالی *) p);
  }
  ساکن hb_uniscribe_shaper_funcs_t *get_null ()
  {
    واپس خالی_اشاریہ;
  }
} static_uniscribe_shaper_funcs;

ساکن اندرونی
خالی free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

ساکن hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  واپس static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

ساخت hb_uniscribe_face_ڈیٹا_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ساکن خالی
_hb_generate_unique_face_name (wchar_t *face_name, غیرعلامتی صحیح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  غیرعلامتی صحیح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  غیرعلامتی حرف *p = (غیرعلامتی حرف *) &id;
  برائے (غیرعلامتی صحیح i = 0; i < 16; i += 2)
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
  اگر (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ساکن hb_blob_t *
_hb_rename_font (hb_blob_t *blob_اندر, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_اندر));

  غیرعلامتی صحیح لمبائی, new_لمبائی, name_str_len;
  ثابت حرف *orig_sfnt_ڈیٹا = hb_blob_get_ڈیٹا (blob, &لمبائی);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساکن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  غیرعلامتی صحیح name_جدول_لمبائی = OT::name::min_سائز +
				   ARRAY_لمبائی (name_IDs) * OT::Name_ریکارڈ::static_سائز +
				   name_str_len * 2; /* for name data in UTF16BE form */
  غیرعلامتی صحیح padded_name_جدول_لمبائی = ((name_جدول_لمبائی + 3) & ~3);
  غیرعلامتی صحیح name_جدول_انحراف = (لمبائی + 3) & ~3;

  new_لمبائی = name_جدول_انحراف + padded_name_جدول_لمبائی;
  خالی *new_sfnt_ڈیٹا = hb_calloc (1, new_لمبائی);
  اگر (!new_sfnt_ڈیٹا)
    واپس خالی_اشاریہ;
  خودکار sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_ڈیٹا); });

  hb_memcpy(new_sfnt_ڈیٹا, orig_sfnt_ڈیٹا, لمبائی);

  OT::name &name = Struct_At_انحراف<OT::name> (new_sfnt_ڈیٹا, name_جدول_انحراف);
  name.format = 0;
  name.گنتی = ARRAY_لمبائی (name_IDs);
  name.string_انحراف = name.get_سائز ();
  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (name_IDs); i++)
  {
    OT::Name_ریکارڈ &ریکارڈ = name.name_ریکارڈ_Z[i];
    ریکارڈ.platformID = 3;
    ریکارڈ.encodingID = 1;
    ریکارڈ.languageID = 0x0409u; /* English */
    ریکارڈ.nameID = name_IDs[i];
    ریکارڈ.لمبائی = name_str_len * 2;
    ریکارڈ.انحراف = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  غیرعلامتی حرف *p = &StructAfter<غیرعلامتی حرف> (name);
  برائے (غیرعلامتی صحیح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_ڈیٹا);
  غیرعلامتی صحیح face_گنتی = file.get_face_گنتی ();
  برائے (غیرعلامتی صحیح face_اشاریہ = 0; face_اشاریہ < face_گنتی; face_اشاریہ++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_اشاریہ);
    غیرعلامتی صحیح اشاریہ_2;
    اگر (face.ڈھونڈو_جدول_اشاریہ (HB_OT_لیبل_name, &اشاریہ_2))
    {
      OT::جدول_ریکارڈ &ریکارڈ = const_cast<OT::جدول_ریکارڈ &> (face.get_جدول (اشاریہ_2));
      ریکارڈ.check_مجموع.سیٹ_for_ڈیٹا (&name, padded_name_جدول_لمبائی);
      ریکارڈ.انحراف = name_جدول_انحراف;
      ریکارڈ.لمبائی = name_جدول_لمبائی;
    }
    ورنہ اگر (face_اشاریہ == 0) /* Fail if first face doesn't have 'name' table. */
      واپس خالی_اشاریہ;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  واپس hb_blob_create ((ثابت حرف *) new_sfnt_ڈیٹا, new_لمبائی,
			 HB_حافظہ_منوال_WRITABLE, new_sfnt_ڈیٹا, hb_free);
}

hb_uniscribe_face_ڈیٹا_t *
hb_uniscribe_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  hb_uniscribe_face_ڈیٹا_t *ڈیٹا = (hb_uniscribe_face_ڈیٹا_t *) hb_calloc (1, حجم (hb_uniscribe_face_ڈیٹا_t));
  اگر (unlikely (!ڈیٹا))
    واپس خالی_اشاریہ;

  ڈیٹا->funcs = hb_uniscribe_shaper_get_funcs ();
  اگر (unlikely (!ڈیٹا->funcs))
  {
    hb_free (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  اگر (unlikely (!hb_blob_get_لمبائی (blob)))
    DEBUG_پیغام (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, ڈیٹا->face_name);
  اگر (unlikely (!blob))
  {
    hb_free (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  DWORD عدد_fonts_installed;
  ڈیٹا->fh = جوڑو_Font_Mem_Resource_Ex ((خالی *) hb_blob_get_ڈیٹا (blob, خالی_اشاریہ),
				   hb_blob_get_لمبائی (blob),
				   0, &عدد_fonts_installed);
  اگر (unlikely (!ڈیٹا->fh))
  {
    DEBUG_پیغام (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  واپس ڈیٹا;
}

خالی
hb_uniscribe_shaper_face_ڈیٹا_destroy (hb_uniscribe_face_ڈیٹا_t *ڈیٹا)
{
  RemoveFontMemResourceEx (ڈیٹا->fh);
  hb_free (ڈیٹا);
}


/*
 * shaper font data
 */

ساخت hb_uniscribe_font_ڈیٹا_t
{
  HDC hdc;
  تغیرپذیر LOGFONTW log_font;
  HFONT hfont;
  تغیرپذیر SCRIPT_CACHE script_cache;
  دوہرا x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساکن منطقی
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   غیرعلامتی صحیح font_سائز)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lf_اونچائی = - (صحیح) font_سائز;
  lf->lf_Char_سیٹ = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->ڈیٹا.uniscribe->face_name, حجم (lf->lfFaceName));

  واپس سچ;
}

hb_uniscribe_font_ڈیٹا_t *
hb_uniscribe_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  hb_uniscribe_font_ڈیٹا_t *ڈیٹا = (hb_uniscribe_font_ڈیٹا_t *) hb_calloc (1, حجم (hb_uniscribe_font_ڈیٹا_t));
  اگر (unlikely (!ڈیٹا))
    واپس خالی_اشاریہ;

  صحیح font_سائز = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  اگر (font->y_ppem)
    font_سائز = font->y_ppem;

  اگر (font_سائز < 0)
    font_سائز = -font_سائز;
  ڈیٹا->x_mult = (دوہرا) font->x_scale / font_سائز;
  ڈیٹا->y_mult = (دوہرا) font->y_scale / font_سائز;

  ڈیٹا->hdc = GetDC (خالی_اشاریہ);

  اگر (unlikely (!populate_log_font (&ڈیٹا->log_font, font, font_سائز))) {
    DEBUG_پیغام (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_ڈیٹا_destroy (ڈیٹا);
    واپس خالی_اشاریہ;
  }

  ڈیٹا->hfont = Create_Font_بالواسطہ_W (&ڈیٹا->log_font);
  اگر (unlikely (!ڈیٹا->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_ڈیٹا_destroy (ڈیٹا);
     واپس خالی_اشاریہ;
  }

  اگر (!SelectObject (ڈیٹا->hdc, ڈیٹا->hfont)) {
    DEBUG_پیغام (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_ڈیٹا_destroy (ڈیٹا);
     واپس خالی_اشاریہ;
  }

  واپس ڈیٹا;
}

خالی
hb_uniscribe_shaper_font_ڈیٹا_destroy (hb_uniscribe_font_ڈیٹا_t *ڈیٹا)
{
  اگر (ڈیٹا->hdc)
    ReleaseDC (خالی_اشاریہ, ڈیٹا->hdc);
  اگر (ڈیٹا->hfont)
    DeleteObject (ڈیٹا->hfont);
  اگر (ڈیٹا->script_cache)
    ScriptFreeCache (&ڈیٹا->script_cache);
  hb_free (ڈیٹا);
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
  ثابت hb_uniscribe_font_ڈیٹا_t *ڈیٹا =  font->ڈیٹا.uniscribe;
  واپس ڈیٹا ? &ڈیٹا->log_font : خالی_اشاریہ;
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
  ثابت hb_uniscribe_font_ڈیٹا_t *ڈیٹا =  font->ڈیٹا.uniscribe;
  واپس ڈیٹا ? ڈیٹا->hfont : خالی_اشاریہ;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_بفر_t        *بفر,
		     ثابت hb_feature_t *features,
		     غیرعلامتی صحیح        عدد_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_ڈیٹا_t *face_ڈیٹا = face->ڈیٹا.uniscribe;
  ثابت hb_uniscribe_font_ڈیٹا_t *font_ڈیٹا = font->ڈیٹا.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_ڈیٹا->funcs;

#تعریف_کرو FAIL(...) \
  HB_STMT_آغاز { \
    DEBUG_پیغام (UNISCRIBE, خالی_اشاریہ, __VA_ARGS__); \
    واپس جھوٹ; \
  } HB_STMT_انتہا

  HRESULT hr;

retry:

  غیرعلامتی صحیح scratch_سائز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائز);

#تعریف_کرو ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  کرو { \
    غیرعلامتی صحیح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_سائز); \
    scratch += _consumed; \
    scratch_سائز -= _consumed; \
  } جب (0)

#تعریف_کرو utf16_اشاریہ() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, بفر->len * 2);

  غیرعلامتی صحیح chars_len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بفر->info[i].utf16_اشاریہ() = chars_len;
    اگر (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ورنہ اگر (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ورنہ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (لفظ, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  اگر (عدد_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
    {
      hb_codepoint_t c = بفر->info[i].codepoint;
      غیرعلامتی صحیح cluster = بفر->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      اگر (hb_اندر_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  غیرعلامتی صحیح glyphs_سائز = (scratch_سائز * حجم (صحیح) - 2)
			   / (حجم (لفظ) +
			      حجم (SCRIPT_GLYPHPROP) +
			      حجم (صحیح) +
			      حجم (GOFFSET) +
			      حجم (uint32_t));

  ALLOCATE_ARRAY (لفظ, glyphs, glyphs_سائز);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_سائز);
  ALLOCATE_ARRAY (صحیح, advances, glyphs_سائز);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_سائز);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_سائز);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#تعریف_منسوخ ALLOCATE_ARRAY

#تعریف_کرو MAX_اشیاء 256

  SCRIPT_شے اشیاء[MAX_اشیاء + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_لیبل_فہرست[MAX_اشیاء];
  صحیح شے_گنتی_2;

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
				     script_لیبل_فہرست,
				     &شے_گنتی_2);
  اگر (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (غیرعلامتی طویل) hr);

#تعریف_منسوخ MAX_اشیاء

  hb_لیبل_t lang_لیبل;
  غیرعلامتی صحیح lang_گنتی = 1;
  hb_ot_لیبل_فہرست_from_script_and_language (بفر->props.script,
				       بفر->props.language,
				       خالی_اشاریہ, خالی_اشاریہ,
				       &lang_گنتی, &lang_لیبل);
  OPENTYPE_لیبل language_لیبل = hb_uint32_تبادلہ (lang_گنتی ? lang_لیبل : HB_لیبل_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_ریکارڈ) == حجم (hb_ms_feature_t)), "");
  hb_سمتیہ_t<hb_ms_feature_t> feature_اندراجات;
  hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> range_اندراجات;
  منطقی has_features = جھوٹ;
  اگر (عدد_features)
    has_features = hb_ms_تیاری_features (features,
					 عدد_features,
					 feature_اندراجات,
					 range_اندراجات);

  hb_سمتیہ_t<hb_ms_features_t*> range_properties;
  hb_سمتیہ_t<uint32_t> range_char_counts;

  غیرعلامتی صحیح glyphs_انحراف = 0;
  غیرعلامتی صحیح glyphs_len;
  منطقی backward = HB_DIRECTION_IS_BACKWARD (بفر->props.direction);
  برائے (صحیح i = 0; i < شے_گنتی_2; i++)
  {
    غیرعلامتی صحیح chars_انحراف = اشیاء[i].iCharPos;
    غیرعلامتی صحیح شے_chars_len = اشیاء[i + 1].iCharPos - chars_انحراف;

    اگر (has_features)
      hb_ms_make_feature_ranges (feature_اندراجات,
				 range_اندراجات,
				 شے_chars_len,
				 chars_انحراف,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    اشیاء[i].a.f_Logical_ترتیب = سچ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_ڈیٹا->hdc,
				     &font_ڈیٹا->script_cache,
				     &اشیاء[i].a,
				     script_لیبل_فہرست[i],
				     language_لیبل,
				     (صحیح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.لمبائی,
				     pchars + chars_انحراف,
				     شے_chars_len,
				     glyphs_سائز - glyphs_انحراف,
				     /* out */
				     log_clusters + chars_انحراف,
				     char_props + chars_انحراف,
				     glyphs + glyphs_انحراف,
				     glyph_props + glyphs_انحراف,
				     (صحیح *) &glyphs_len);

    اگر (unlikely (اشیاء[i].a.f_No_Glyph_اشاریہ))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    اگر (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_بفر))
    {
      اگر (unlikely (!بفر->ensure (بفر->allocated * 2)))
	FAIL ("Buffer resize failed");
      جاؤ retry;
    }
    اگر (unlikely (hr == USP_E_SCRIPT_NOT_اندر_FONT))
    {
      اگر (اشیاء[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      اشیاء[i].a.eScript = SCRIPT_UNDEFINED;
      جاؤ retry_shape;
    }
    اگر (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (غیرعلامتی طویل) hr);
    }

    برائے (غیرعلامتی صحیح j = chars_انحراف; j < chars_انحراف + شے_chars_len; j++)
      log_clusters[j] += glyphs_انحراف;

    hr = funcs->Script_رکھو_Open_Type (font_ڈیٹا->hdc,
				     &font_ڈیٹا->script_cache,
				     &اشیاء[i].a,
				     script_لیبل_فہرست[i],
				     language_لیبل,
				     (صحیح *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.لمبائی,
				     pchars + chars_انحراف,
				     log_clusters + chars_انحراف,
				     char_props + chars_انحراف,
				     شے_chars_len,
				     glyphs + glyphs_انحراف,
				     glyph_props + glyphs_انحراف,
				     glyphs_len,
				     /* out */
				     advances + glyphs_انحراف,
				     offsets + glyphs_انحراف,
				     خالی_اشاریہ);
    اگر (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (غیرعلامتی طویل) hr);

    اگر (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       اشیاء[i].a.fRTL,
	       اشیاء[i].a.fLayoutRTL,
	       اشیاء[i].a.f_Logical_ترتیب,
	       HB_UNTAG (hb_uint32_تبادلہ (script_لیبل_فہرست[i])));

    glyphs_انحراف += glyphs_len;
  }
  glyphs_len = glyphs_انحراف;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  برائے (غیرعلامتی صحیح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[بفر->info[i].utf16_اشاریہ()]];
    *p = hb_min (*p, بفر->info[i].cluster);
  }
  برائے (غیرعلامتی صحیح i = 1; i < glyphs_len; i++)
    اگر (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#تعریف_منسوخ utf16_اشاریہ

  اگر (unlikely (!بفر->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#تعریف_منسوخ FAIL

  /* Set glyph infos */
  بفر->len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < glyphs_len; i++)
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
  دوہرا x_mult = font_ڈیٹا->x_mult, y_mult = font_ڈیٹا->y_mult;
  برائے (غیرعلامتی صحیح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &بفر->info[i];
    hb_glyph_position_t *pos = &بفر->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_مقصد<hb_position_t> (x_mult * (int32_t) info->نقاب);
    دوہرا x_انحراف_2 = backward ? -(دوہرا) info->var1.i32 : info->var1.i32;
    pos->x_انحراف_2 = hb_clamp_مقصد<hb_position_t> (x_mult * x_انحراف_2);
    pos->y_انحراف_2 = hb_clamp_مقصد<hb_position_t> (y_mult * info->var2.i32);
  }

  اگر (backward)
    hb_بفر_الٹ (بفر);

  بفر->clear_glyph_flags ();
  بفر->unsafe_مقصد_break ();

  /* Wow, done! */
  واپس سچ;
}


#اگر_ختم
