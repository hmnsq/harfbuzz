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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#অন্তর্ভাব "hb-shaper-impl.hh"

#অন্তর্ভাব <windows.h>
#অন্তর্ভাব <usp10.h>
#অন্তর্ভাব <rpc.h>

#ifndef E_NOT_SUFFICIENT_বাফার
#define E_NOT_SUFFICIENT_বাফার HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_বাফার)
#endif

#অন্তর্ভাব "hb-uniscribe.h"

#অন্তর্ভাব "hb-ms-feature-ranges.hh"
#অন্তর্ভাব "hb-open-file.hh"
#অন্তর্ভাব "hb-ot-name-table.hh"
#অন্তর্ভাব "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

প্রকারলেখ HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ধ্রুবক WCHAR *pwcInChars,
  পূর্ণসংখ্যা cInChars,
  পূর্ণসংখ্যা c_Max_বস্তুসমূহ,
  ধ্রুবক SCRIPT_CONTROL *psControl,
  ধ্রুবক SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_বস্তুসমূহ,
  OPENTYPE_TAG *pScriptTags,
  পূর্ণসংখ্যা *pc_বস্তুসমূহ
);

প্রকারলেখ HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  পূর্ণসংখ্যা *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  পূর্ণসংখ্যা cRanges,
  ধ্রুবক WCHAR *pwcChars,
  পূর্ণসংখ্যা cChars,
  পূর্ণসংখ্যা cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  পূর্ণসংখ্যা *pcGlyphs
);

প্রকারলেখ HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  পূর্ণসংখ্যা *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  পূর্ণসংখ্যা cRanges,
  ধ্রুবক WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  পূর্ণসংখ্যা cChars,
  ধ্রুবক WORD *pwGlyphs,
  ধ্রুবক SCRIPT_GLYPHPROP *pGlyphProps,
  পূর্ণসংখ্যা cGlyphs,
  পূর্ণসংখ্যা *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

স্থির HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ধ্রুবক WCHAR *pwcInChars,
  পূর্ণসংখ্যা cInChars,
  পূর্ণসংখ্যা c_Max_বস্তুসমূহ,
  ধ্রুবক SCRIPT_CONTROL *psControl,
  ধ্রুবক SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_বস্তুসমূহ,
  OPENTYPE_TAG *pScriptTags,
  পূর্ণসংখ্যা *pc_বস্তুসমূহ
)
{
{
  ফেরত ScriptItemize (pwcInChars,
			cInChars,
			c_Max_বস্তুসমূহ,
			psControl,
			psState,
			p_বস্তুসমূহ,
			pc_বস্তুসমূহ);
}
}

স্থির HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  পূর্ণসংখ্যা *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  পূর্ণসংখ্যা cRanges,
  ধ্রুবক WCHAR *pwcChars,
  পূর্ণসংখ্যা cChars,
  পূর্ণসংখ্যা cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  পূর্ণসংখ্যা *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  ফেরত ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pwOutGlyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

স্থির HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  পূর্ণসংখ্যা *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  পূর্ণসংখ্যা cRanges,
  ধ্রুবক WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  পূর্ণসংখ্যা cChars,
  ধ্রুবক WORD *pwGlyphs,
  ধ্রুবক SCRIPT_GLYPHPROP *pGlyphProps,
  পূর্ণসংখ্যা cGlyphs,
  পূর্ণসংখ্যা *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  ফেরত ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


কাঠামো hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  শূন্য init ()
  {
    HMODULE hinstLib;
    এই->ScriptItemizeOpenType = nullptr;
    এই->ScriptShapeOpenType   = nullptr;
    এই->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    যদি (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      এই->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      এই->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      এই->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic বের_করো
    }
    যদি (!এই->ScriptItemizeOpenType ||
	!এই->ScriptShapeOpenType   ||
	!এই->ScriptPlaceOpenType)
    {
      DEBUG_বার্তা (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      এই->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      এই->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      এই->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

স্থির অন্তঃরেখ শূন্য free_static_uniscribe_shaper_funcs ();

স্থির কাঠামো hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  স্থির hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, আকার (hb_uniscribe_shaper_funcs_t));
    যদি (unlikely (!funcs))
      ফেরত nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    ফেরত funcs;
  }
  স্থির শূন্য destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((শূন্য *) p);
  }
  স্থির hb_uniscribe_shaper_funcs_t *get_null ()
  {
    ফেরত nullptr;
  }
} static_uniscribe_shaper_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

স্থির hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  ফেরত static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

কাঠামো hb_uniscribe_face_তথ্য_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
স্থির শূন্য
_hb_generate_unique_face_name (wchar_t *face_name, unsigned পূর্ণসংখ্যা *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ধ্রুবক অক্ষর *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned পূর্ণসংখ্যা name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned অক্ষর *p = (unsigned অক্ষর *) &id;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < 16; i += 2)
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
  যদি (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
স্থির hb_blob_t *
_hb_rename_font (hb_blob_t *blob_in, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_in));

  unsigned পূর্ণসংখ্যা দৈর্ঘ্য, new_দৈর্ঘ্য, name_str_len;
  ধ্রুবক অক্ষর *orig_sfnt_তথ্য = hb_blob_get_তথ্য (blob, &দৈর্ঘ্য);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  স্থির ধ্রুবক uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned পূর্ণসংখ্যা name_table_দৈর্ঘ্য = OT::name::min_আকার +
				   ARRAY_দৈর্ঘ্য (name_IDs) * OT::NameRecord::static_আকার +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned পূর্ণসংখ্যা padded_name_table_দৈর্ঘ্য = ((name_table_দৈর্ঘ্য + 3) & ~3);
  unsigned পূর্ণসংখ্যা name_table_offset = (দৈর্ঘ্য + 3) & ~3;

  new_দৈর্ঘ্য = name_table_offset + padded_name_table_দৈর্ঘ্য;
  শূন্য *new_sfnt_তথ্য = hb_calloc (1, new_দৈর্ঘ্য);
  যদি (!new_sfnt_তথ্য)
    ফেরত nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_তথ্য); });

  hb_memcpy(new_sfnt_তথ্য, orig_sfnt_তথ্য, দৈর্ঘ্য);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_তথ্য, name_table_offset);
  name.format = 0;
  name.গুনতি = ARRAY_দৈর্ঘ্য (name_IDs);
  name.stringOffset = name.get_আকার ();
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (name_IDs); i++)
  {
    OT::NameRecord &record = name.nameRecordZ[i];
    record.platformID = 3;
    record.encodingID = 1;
    record.languageID = 0x0409u; /* English */
    record.nameID = name_IDs[i];
    record.দৈর্ঘ্য = name_str_len * 2;
    record.offset = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  unsigned অক্ষর *p = &StructAfter<unsigned অক্ষর> (name);
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ধ্রুবক OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_তথ্য);
  unsigned পূর্ণসংখ্যা face_গুনতি = file.get_face_গুনতি ();
  জন্য (unsigned পূর্ণসংখ্যা face_সূচক = 0; face_সূচক < face_গুনতি; face_সূচক++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ধ্রুবক OT::OpenTypeFontFace &face = file.get_face (face_সূচক);
    unsigned পূর্ণসংখ্যা সূচক_2;
    যদি (face.find_table_সূচক (HB_OT_TAG_name, &সূচক_2))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (সূচক_2));
      record.check_যোগ.set_for_তথ্য (&name, padded_name_table_দৈর্ঘ্য);
      record.offset = name_table_offset;
      record.দৈর্ঘ্য = name_table_দৈর্ঘ্য;
    }
    অন্যথা যদি (face_সূচক == 0) /* Fail if first face doesn't have 'name' table. */
      ফেরত nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  ফেরত hb_blob_create ((ধ্রুবক অক্ষর *) new_sfnt_তথ্য, new_দৈর্ঘ্য,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_তথ্য, hb_free);
}

hb_uniscribe_face_তথ্য_t *
hb_uniscribe_shaper_face_তথ্য_create (hb_face_t *face)
{
  hb_uniscribe_face_তথ্য_t *তথ্য = (hb_uniscribe_face_তথ্য_t *) hb_calloc (1, আকার (hb_uniscribe_face_তথ্য_t));
  যদি (unlikely (!তথ্য))
    ফেরত nullptr;

  তথ্য->funcs = hb_uniscribe_shaper_get_funcs ();
  যদি (unlikely (!তথ্য->funcs))
  {
    hb_free (তথ্য);
    ফেরত nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  যদি (unlikely (!hb_blob_get_দৈর্ঘ্য (blob)))
    DEBUG_বার্তা (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, তথ্য->face_name);
  যদি (unlikely (!blob))
  {
    hb_free (তথ্য);
    ফেরত nullptr;
  }

  DWORD অঙ্ক_fonts_installed;
  তথ্য->fh = AddFontMemResourceEx ((শূন্য *) hb_blob_get_তথ্য (blob, nullptr),
				   hb_blob_get_দৈর্ঘ্য (blob),
				   0, &অঙ্ক_fonts_installed);
  যদি (unlikely (!তথ্য->fh))
  {
    DEBUG_বার্তা (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (তথ্য);
    ফেরত nullptr;
  }

  ফেরত তথ্য;
}

শূন্য
hb_uniscribe_shaper_face_তথ্য_destroy (hb_uniscribe_face_তথ্য_t *তথ্য)
{
  RemoveFontMemResourceEx (তথ্য->fh);
  hb_free (তথ্য);
}


/*
 * shaper font data
 */

কাঠামো hb_uniscribe_font_তথ্য_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  দ্বিগুণ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

স্থির যৌক্তিক
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned পূর্ণসংখ্যা font_আকার)
{
  hb_memset (lf, 0, আকার (*lf));
  lf->lfHeight = - (পূর্ণসংখ্যা) font_আকার;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->তথ্য.uniscribe->face_name, আকার (lf->lfFaceName));

  ফেরত সত্য;
}

hb_uniscribe_font_তথ্য_t *
hb_uniscribe_shaper_font_তথ্য_create (hb_font_t *font)
{
  hb_uniscribe_font_তথ্য_t *তথ্য = (hb_uniscribe_font_তথ্য_t *) hb_calloc (1, আকার (hb_uniscribe_font_তথ্য_t));
  যদি (unlikely (!তথ্য))
    ফেরত nullptr;

  পূর্ণসংখ্যা font_আকার = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  যদি (font->y_ppem)
    font_আকার = font->y_ppem;

  যদি (font_আকার < 0)
    font_আকার = -font_আকার;
  তথ্য->x_mult = (দ্বিগুণ) font->x_scale / font_আকার;
  তথ্য->y_mult = (দ্বিগুণ) font->y_scale / font_আকার;

  তথ্য->hdc = GetDC (nullptr);

  যদি (unlikely (!populate_log_font (&তথ্য->log_font, font, font_আকার))) {
    DEBUG_বার্তা (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_তথ্য_destroy (তথ্য);
    ফেরত nullptr;
  }

  তথ্য->hfont = CreateFontIndirectW (&তথ্য->log_font);
  যদি (unlikely (!তথ্য->hfont)) {
    DEBUG_বার্তা (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_তথ্য_destroy (তথ্য);
     ফেরত nullptr;
  }

  যদি (!SelectObject (তথ্য->hdc, তথ্য->hfont)) {
    DEBUG_বার্তা (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_তথ্য_destroy (তথ্য);
     ফেরত nullptr;
  }

  ফেরত তথ্য;
}

শূন্য
hb_uniscribe_shaper_font_তথ্য_destroy (hb_uniscribe_font_তথ্য_t *তথ্য)
{
  যদি (তথ্য->hdc)
    ReleaseDC (nullptr, তথ্য->hdc);
  যদি (তথ্য->hfont)
    DeleteObject (তথ্য->hfont);
  যদি (তথ্য->script_cache)
    ScriptFreeCache (&তথ্য->script_cache);
  hb_free (তথ্য);
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
  ধ্রুবক hb_uniscribe_font_তথ্য_t *তথ্য =  font->তথ্য.uniscribe;
  ফেরত তথ্য ? &তথ্য->log_font : nullptr;
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
  ধ্রুবক hb_uniscribe_font_তথ্য_t *তথ্য =  font->তথ্য.uniscribe;
  ফেরত তথ্য ? তথ্য->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_বাফার_t        *বাফার,
		     ধ্রুবক hb_feature_t *features,
		     unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  hb_face_t *face = font->face;
  ধ্রুবক hb_uniscribe_face_তথ্য_t *face_তথ্য = face->তথ্য.uniscribe;
  ধ্রুবক hb_uniscribe_font_তথ্য_t *font_তথ্য = font->তথ্য.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_তথ্য->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_বার্তা (UNISCRIBE, nullptr, __VA_ARGS__); \
    ফেরত মিথ্যা; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned পূর্ণসংখ্যা scratch_আকার;
  hb_বাফার_t::scratch_বাফার_t *scratch = বাফার->get_scratch_বাফার (&scratch_আকার);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  করো { \
    unsigned পূর্ণসংখ্যা _consumed = DIV_CEIL ((len) * আকার (Type), আকার (*scratch)); \
    প্রতিজ্ঞা (_consumed <= scratch_আকার); \
    scratch += _consumed; \
    scratch_আকার -= _consumed; \
  } যখন (0)

#define utf16_সূচক() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, বাফার->len * 2);

  unsigned পূর্ণসংখ্যা chars_len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
  {
    hb_codepoint_t c = বাফার->info[i].codepoint;
    বাফার->info[i].utf16_সূচক() = chars_len;
    যদি (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    অন্যথা যদি (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    অন্যথা {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  যদি (অঙ্ক_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
    {
      hb_codepoint_t c = বাফার->info[i].codepoint;
      unsigned পূর্ণসংখ্যা cluster = বাফার->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      যদি (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned পূর্ণসংখ্যা glyphs_আকার = (scratch_আকার * আকার (পূর্ণসংখ্যা) - 2)
			   / (আকার (WORD) +
			      আকার (SCRIPT_GLYPHPROP) +
			      আকার (পূর্ণসংখ্যা) +
			      আকার (GOFFSET) +
			      আকার (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_আকার);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_আকার);
  ALLOCATE_ARRAY (পূর্ণসংখ্যা, advances, glyphs_আকার);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_আকার);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_আকার);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#undef ALLOCATE_ARRAY

#define MAX_বস্তুসমূহ 256

  SCRIPT_ITEM বস্তুসমূহ[MAX_বস্তুসমূহ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_বস্তুসমূহ];
  পূর্ণসংখ্যা item_গুনতি_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (বাফার->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_বস্তুসমূহ,
				     &bidi_control,
				     &bidi_state,
				     বস্তুসমূহ,
				     script_tags,
				     &item_গুনতি_2);
  যদি (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_বস্তুসমূহ

  hb_tag_t lang_tag;
  unsigned পূর্ণসংখ্যা lang_গুনতি = 1;
  hb_ot_tags_from_script_and_language (বাফার->props.script,
				       বাফার->props.language,
				       nullptr, nullptr,
				       &lang_গুনতি, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_গুনতি ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((আকার (TEXTRANGE_PROPERTIES) == আকার (hb_ms_features_t)), "");
  static_assert ((আকার (OPENTYPE_FEATURE_RECORD) == আকার (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  যৌক্তিক has_features = মিথ্যা;
  যদি (অঙ্ক_features)
    has_features = hb_ms_setup_features (features,
					 অঙ্ক_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned পূর্ণসংখ্যা glyphs_offset = 0;
  unsigned পূর্ণসংখ্যা glyphs_len;
  যৌক্তিক backward = HB_DIRECTION_IS_BACKWARD (বাফার->props.direction);
  জন্য (পূর্ণসংখ্যা i = 0; i < item_গুনতি_2; i++)
  {
    unsigned পূর্ণসংখ্যা chars_offset = বস্তুসমূহ[i].iCharPos;
    unsigned পূর্ণসংখ্যা item_chars_len = বস্তুসমূহ[i + 1].iCharPos - chars_offset;

    যদি (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    বস্তুসমূহ[i].a.fLogicalOrder = সত্য;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_তথ্য->hdc,
				     &font_তথ্য->script_cache,
				     &বস্তুসমূহ[i].a,
				     script_tags[i],
				     language_tag,
				     (পূর্ণসংখ্যা *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.দৈর্ঘ্য,
				     pchars + chars_offset,
				     item_chars_len,
				     glyphs_আকার - glyphs_offset,
				     /* out */
				     log_clusters + chars_offset,
				     char_props + chars_offset,
				     glyphs + glyphs_offset,
				     glyph_props + glyphs_offset,
				     (পূর্ণসংখ্যা *) &glyphs_len);

    যদি (unlikely (বস্তুসমূহ[i].a.f_No_Glyph_সূচক))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    যদি (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_বাফার))
    {
      যদি (unlikely (!বাফার->ensure (বাফার->allocated * 2)))
	FAIL ("Buffer resize failed");
      যাও retry;
    }
    যদি (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      যদি (বস্তুসমূহ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      বস্তুসমূহ[i].a.eScript = SCRIPT_UNDEFINED;
      যাও retry_shape;
    }
    যদি (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    জন্য (unsigned পূর্ণসংখ্যা j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_তথ্য->hdc,
				     &font_তথ্য->script_cache,
				     &বস্তুসমূহ[i].a,
				     script_tags[i],
				     language_tag,
				     (পূর্ণসংখ্যা *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.দৈর্ঘ্য,
				     pchars + chars_offset,
				     log_clusters + chars_offset,
				     char_props + chars_offset,
				     item_chars_len,
				     glyphs + glyphs_offset,
				     glyph_props + glyphs_offset,
				     glyphs_len,
				     /* out */
				     advances + glyphs_offset,
				     offsets + glyphs_offset,
				     nullptr);
    যদি (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    যদি (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       বস্তুসমূহ[i].a.fRTL,
	       বস্তুসমূহ[i].a.fLayoutRTL,
	       বস্তুসমূহ[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[বাফার->info[i].utf16_সূচক()]];
    *p = hb_min (*p, বাফার->info[i].cluster);
  }
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < glyphs_len; i++)
    যদি (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_সূচক

  যদি (unlikely (!বাফার->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  বাফার->len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &বাফার->info[বাফার->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  বাফার->clear_positions ();
  দ্বিগুণ x_mult = font_তথ্য->x_mult, y_mult = font_তথ্য->y_mult;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &বাফার->info[i];
    hb_glyph_position_t *pos = &বাফার->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    দ্বিগুণ x_offset = backward ? -(দ্বিগুণ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  যদি (backward)
    hb_বাফার_reverse (বাফার);

  বাফার->clear_glyph_flags ();
  বাফার->unsafe_to_break ();

  /* Wow, done! */
  ফেরত সত্য;
}


#endif
