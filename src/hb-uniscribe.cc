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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#അന്തര്ഭാവ <windows.h>
#അന്തര്ഭാവ <usp10.h>
#അന്തര്ഭാവ <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

#അന്തര്ഭാവ "hb-uniscribe.h"

#അന്തര്ഭാവ "hb-ms-feature-ranges.hh"
#അന്തര്ഭാവ "hb-open-file.hh"
#അന്തര്ഭാവ "hb-ot-name-table.hh"
#അന്തര്ഭാവ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

തരനിർവചനം HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  സ്ഥിരം WCHAR *pwcInChars,
  പൂർണ്ണസംഖ്യ cInChars,
  പൂർണ്ണസംഖ്യ c_Max_ഇനങ്ങൾ,
  സ്ഥിരം SCRIPT_CONTROL *psControl,
  സ്ഥിരം SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ഇനങ്ങൾ,
  OPENTYPE_TAG *pScriptTags,
  പൂർണ്ണസംഖ്യ *pc_ഇനങ്ങൾ
);

തരനിർവചനം HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  പൂർണ്ണസംഖ്യ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  പൂർണ്ണസംഖ്യ cRanges,
  സ്ഥിരം WCHAR *pwcChars,
  പൂർണ്ണസംഖ്യ cChars,
  പൂർണ്ണസംഖ്യ cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  പൂർണ്ണസംഖ്യ *pcGlyphs
);

തരനിർവചനം HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  പൂർണ്ണസംഖ്യ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  പൂർണ്ണസംഖ്യ cRanges,
  സ്ഥിരം WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  പൂർണ്ണസംഖ്യ cChars,
  സ്ഥിരം WORD *pwGlyphs,
  സ്ഥിരം SCRIPT_GLYPHPROP *pGlyphProps,
  പൂർണ്ണസംഖ്യ cGlyphs,
  പൂർണ്ണസംഖ്യ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

സ്ഥിതം HRESULT WINAPI
hb_ScriptItemizeOpenType(
  സ്ഥിരം WCHAR *pwcInChars,
  പൂർണ്ണസംഖ്യ cInChars,
  പൂർണ്ണസംഖ്യ c_Max_ഇനങ്ങൾ,
  സ്ഥിരം SCRIPT_CONTROL *psControl,
  സ്ഥിരം SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ഇനങ്ങൾ,
  OPENTYPE_TAG *pScriptTags,
  പൂർണ്ണസംഖ്യ *pc_ഇനങ്ങൾ
)
{
{
  തിരികെ ScriptItemize (pwcInChars,
			cInChars,
			c_Max_ഇനങ്ങൾ,
			psControl,
			psState,
			p_ഇനങ്ങൾ,
			pc_ഇനങ്ങൾ);
}
}

സ്ഥിതം HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  പൂർണ്ണസംഖ്യ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  പൂർണ്ണസംഖ്യ cRanges,
  സ്ഥിരം WCHAR *pwcChars,
  പൂർണ്ണസംഖ്യ cChars,
  പൂർണ്ണസംഖ്യ cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  പൂർണ്ണസംഖ്യ *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  തിരികെ ScriptShape (hdc,
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

സ്ഥിതം HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  പൂർണ്ണസംഖ്യ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  പൂർണ്ണസംഖ്യ cRanges,
  സ്ഥിരം WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  പൂർണ്ണസംഖ്യ cChars,
  സ്ഥിരം WORD *pwGlyphs,
  സ്ഥിരം SCRIPT_GLYPHPROP *pGlyphProps,
  പൂർണ്ണസംഖ്യ cGlyphs,
  പൂർണ്ണസംഖ്യ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  തിരികെ ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


ഘടന hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  ശൂന്യം init ()
  {
    HMODULE hinstLib;
    ഇത്->ScriptItemizeOpenType = nullptr;
    ഇത്->ScriptShapeOpenType   = nullptr;
    ഇത്->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    എങ്കിൽ (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      ഇത്->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      ഇത്->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      ഇത്->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic എടുക്കുക
    }
    എങ്കിൽ (!ഇത്->ScriptItemizeOpenType ||
	!ഇത്->ScriptShapeOpenType   ||
	!ഇത്->ScriptPlaceOpenType)
    {
      DEBUG_സന്ദേശം (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      ഇത്->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      ഇത്->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      ഇത്->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_uniscribe_shaper_funcs ();

സ്ഥിതം ഘടന hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, വലുപ്പം (hb_uniscribe_shaper_funcs_t));
    എങ്കിൽ (unlikely (!funcs))
      തിരികെ nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    തിരികെ funcs;
  }
  സ്ഥിതം ശൂന്യം destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((ശൂന്യം *) p);
  }
  സ്ഥിതം hb_uniscribe_shaper_funcs_t *get_null ()
  {
    തിരികെ nullptr;
  }
} static_uniscribe_shaper_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

സ്ഥിതം hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  തിരികെ static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

ഘടന hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
സ്ഥിതം ശൂന്യം
_hb_generate_unique_face_name (wchar_t *face_name, unsigned പൂർണ്ണസംഖ്യ *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  സ്ഥിരം അക്ഷരം *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned പൂർണ്ണസംഖ്യ name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned അക്ഷരം *p = (unsigned അക്ഷരം *) &id;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < 16; i += 2)
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
  എങ്കിൽ (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
സ്ഥിതം hb_blob_t *
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

  unsigned പൂർണ്ണസംഖ്യ length, new_length, name_str_len;
  സ്ഥിരം അക്ഷരം *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  സ്ഥിതം സ്ഥിരം uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned പൂർണ്ണസംഖ്യ name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned പൂർണ്ണസംഖ്യ padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned പൂർണ്ണസംഖ്യ name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  ശൂന്യം *new_sfnt_data = hb_calloc (1, new_length);
  എങ്കിൽ (!new_sfnt_data)
    തിരികെ nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.എണ്ണം = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (name_IDs); i++)
  {
    OT::NameRecord &record = name.nameRecordZ[i];
    record.platformID = 3;
    record.encodingID = 1;
    record.languageID = 0x0409u; /* English */
    record.nameID = name_IDs[i];
    record.length = name_str_len * 2;
    record.offset = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  unsigned അക്ഷരം *p = &StructAfter<unsigned അക്ഷരം> (name);
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  സ്ഥിരം OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned പൂർണ്ണസംഖ്യ face_എണ്ണം = file.get_face_എണ്ണം ();
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ face_index = 0; face_index < face_എണ്ണം; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    സ്ഥിരം OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned പൂർണ്ണസംഖ്യ index;
    എങ്കിൽ (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      തിരികെ nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  തിരികെ hb_blob_create ((സ്ഥിരം അക്ഷരം *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, വലുപ്പം (hb_uniscribe_face_data_t));
  എങ്കിൽ (unlikely (!data))
    തിരികെ nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  എങ്കിൽ (unlikely (!data->funcs))
  {
    hb_free (data);
    തിരികെ nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  എങ്കിൽ (unlikely (!hb_blob_get_length (blob)))
    DEBUG_സന്ദേശം (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  എങ്കിൽ (unlikely (!blob))
  {
    hb_free (data);
    തിരികെ nullptr;
  }

  DWORD അക്കം_fonts_installed;
  data->fh = AddFontMemResourceEx ((ശൂന്യം *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &അക്കം_fonts_installed);
  എങ്കിൽ (unlikely (!data->fh))
  {
    DEBUG_സന്ദേശം (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    തിരികെ nullptr;
  }

  തിരികെ data;
}

ശൂന്യം
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

ഘടന hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  ഇരട്ട x_mult, y_mult; /* From LOGFONT space to HB space. */
};

സ്ഥിതം തർക്കം
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned പൂർണ്ണസംഖ്യ font_size)
{
  hb_memset (lf, 0, വലുപ്പം (*lf));
  lf->lfHeight = - (പൂർണ്ണസംഖ്യ) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, വലുപ്പം (lf->lfFaceName));

  തിരികെ സത്യം;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, വലുപ്പം (hb_uniscribe_font_data_t));
  എങ്കിൽ (unlikely (!data))
    തിരികെ nullptr;

  പൂർണ്ണസംഖ്യ font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  എങ്കിൽ (font->y_ppem)
    font_size = font->y_ppem;

  എങ്കിൽ (font_size < 0)
    font_size = -font_size;
  data->x_mult = (ഇരട്ട) font->x_scale / font_size;
  data->y_mult = (ഇരട്ട) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  എങ്കിൽ (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_സന്ദേശം (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    തിരികെ nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  എങ്കിൽ (unlikely (!data->hfont)) {
    DEBUG_സന്ദേശം (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     തിരികെ nullptr;
  }

  എങ്കിൽ (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_സന്ദേശം (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     തിരികെ nullptr;
  }

  തിരികെ data;
}

ശൂന്യം
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  എങ്കിൽ (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  എങ്കിൽ (data->hfont)
    DeleteObject (data->hfont);
  എങ്കിൽ (data->script_cache)
    ScriptFreeCache (&data->script_cache);
  hb_free (data);
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
  സ്ഥിരം hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  തിരികെ data ? &data->log_font : nullptr;
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
  സ്ഥിരം hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  തിരികെ data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     സ്ഥിരം hb_feature_t *features,
		     unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  hb_face_t *face = font->face;
  സ്ഥിരം hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  സ്ഥിരം hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_സന്ദേശം (UNISCRIBE, nullptr, __VA_ARGS__); \
    തിരികെ അസത്യം; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned പൂർണ്ണസംഖ്യ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ചെയ്യുക { \
    unsigned പൂർണ്ണസംഖ്യ _consumed = DIV_CEIL ((len) * വലുപ്പം (Type), വലുപ്പം (*scratch)); \
    പ്രതിജ്ഞാ (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ആകുമ്പോൾ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned പൂർണ്ണസംഖ്യ chars_len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    എങ്കിൽ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    അല്ലെങ്കിൽ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  എങ്കിൽ (അക്കം_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned പൂർണ്ണസംഖ്യ cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      എങ്കിൽ (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned പൂർണ്ണസംഖ്യ glyphs_size = (scratch_size * വലുപ്പം (പൂർണ്ണസംഖ്യ) - 2)
			   / (വലുപ്പം (WORD) +
			      വലുപ്പം (SCRIPT_GLYPHPROP) +
			      വലുപ്പം (പൂർണ്ണസംഖ്യ) +
			      വലുപ്പം (GOFFSET) +
			      വലുപ്പം (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (പൂർണ്ണസംഖ്യ, advances, glyphs_size);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_size);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_size);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#undef ALLOCATE_ARRAY

#define MAX_ഇനങ്ങൾ 256

  SCRIPT_ITEM ഇനങ്ങൾ[MAX_ഇനങ്ങൾ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_ഇനങ്ങൾ];
  പൂർണ്ണസംഖ്യ item_എണ്ണം_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_ഇനങ്ങൾ,
				     &bidi_control,
				     &bidi_state,
				     ഇനങ്ങൾ,
				     script_tags,
				     &item_എണ്ണം_2);
  എങ്കിൽ (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_ഇനങ്ങൾ

  hb_tag_t lang_tag;
  unsigned പൂർണ്ണസംഖ്യ lang_എണ്ണം = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_എണ്ണം, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_എണ്ണം ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((വലുപ്പം (TEXTRANGE_PROPERTIES) == വലുപ്പം (hb_ms_features_t)), "");
  static_assert ((വലുപ്പം (OPENTYPE_FEATURE_RECORD) == വലുപ്പം (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  തർക്കം has_features = അസത്യം;
  എങ്കിൽ (അക്കം_features)
    has_features = hb_ms_setup_features (features,
					 അക്കം_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned പൂർണ്ണസംഖ്യ glyphs_offset = 0;
  unsigned പൂർണ്ണസംഖ്യ glyphs_len;
  തർക്കം backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < item_എണ്ണം_2; i++)
  {
    unsigned പൂർണ്ണസംഖ്യ chars_offset = ഇനങ്ങൾ[i].iCharPos;
    unsigned പൂർണ്ണസംഖ്യ item_chars_len = ഇനങ്ങൾ[i + 1].iCharPos - chars_offset;

    എങ്കിൽ (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    ഇനങ്ങൾ[i].a.fLogicalOrder = സത്യം;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ഇനങ്ങൾ[i].a,
				     script_tags[i],
				     language_tag,
				     (പൂർണ്ണസംഖ്യ *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.length,
				     pchars + chars_offset,
				     item_chars_len,
				     glyphs_size - glyphs_offset,
				     /* out */
				     log_clusters + chars_offset,
				     char_props + chars_offset,
				     glyphs + glyphs_offset,
				     glyph_props + glyphs_offset,
				     (പൂർണ്ണസംഖ്യ *) &glyphs_len);

    എങ്കിൽ (unlikely (ഇനങ്ങൾ[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    എങ്കിൽ (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      എങ്കിൽ (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      പോകുക retry;
    }
    എങ്കിൽ (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      എങ്കിൽ (ഇനങ്ങൾ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      ഇനങ്ങൾ[i].a.eScript = SCRIPT_UNDEFINED;
      പോകുക retry_shape;
    }
    എങ്കിൽ (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ഇനങ്ങൾ[i].a,
				     script_tags[i],
				     language_tag,
				     (പൂർണ്ണസംഖ്യ *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.length,
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
    എങ്കിൽ (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    എങ്കിൽ (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       ഇനങ്ങൾ[i].a.fRTL,
	       ഇനങ്ങൾ[i].a.fLayoutRTL,
	       ഇനങ്ങൾ[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < glyphs_len; i++)
    എങ്കിൽ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  എങ്കിൽ (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[buffer->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  buffer->clear_positions ();
  ഇരട്ട x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    ഇരട്ട x_offset = backward ? -(ഇരട്ട) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  എങ്കിൽ (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  തിരികെ സത്യം;
}


#endif
