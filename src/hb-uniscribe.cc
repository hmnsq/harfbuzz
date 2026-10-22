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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#ಅನ್ತರ್ಭಾವ "hb-shaper-impl.hh"

#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <usp10.h>
#ಅನ್ತರ್ಭಾವ <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

#ಅನ್ತರ್ಭಾವ "hb-uniscribe.h"

#ಅನ್ತರ್ಭಾವ "hb-ms-feature-ranges.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-file.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-name-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

ಪ್ರಕಾರನಿರ್ದೇಶ HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ಧ್ರುವ WCHAR *pwcInChars,
  ಪೂರ್ಣಾಙ್ಕ cInChars,
  ಪೂರ್ಣಾಙ್ಕ c_Max_ವಸ್ತುಗಳು,
  ಧ್ರುವ SCRIPT_CONTROL *psControl,
  ಧ್ರುವ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ವಸ್ತುಗಳು,
  OPENTYPE_TAG *pScriptTags,
  ಪೂರ್ಣಾಙ್ಕ *pc_ವಸ್ತುಗಳು
);

ಪ್ರಕಾರನಿರ್ದೇಶ HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ಪೂರ್ಣಾಙ್ಕ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ಪೂರ್ಣಾಙ್ಕ cRanges,
  ಧ್ರುವ WCHAR *pwcChars,
  ಪೂರ್ಣಾಙ್ಕ cChars,
  ಪೂರ್ಣಾಙ್ಕ cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  ಪೂರ್ಣಾಙ್ಕ *pcGlyphs
);

ಪ್ರಕಾರನಿರ್ದೇಶ HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ಪೂರ್ಣಾಙ್ಕ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ಪೂರ್ಣಾಙ್ಕ cRanges,
  ಧ್ರುವ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ಪೂರ್ಣಾಙ್ಕ cChars,
  ಧ್ರುವ WORD *pwGlyphs,
  ಧ್ರುವ SCRIPT_GLYPHPROP *pGlyphProps,
  ಪೂರ್ಣಾಙ್ಕ cGlyphs,
  ಪೂರ್ಣಾಙ್ಕ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ಸ್ಥಿರ HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ಧ್ರುವ WCHAR *pwcInChars,
  ಪೂರ್ಣಾಙ್ಕ cInChars,
  ಪೂರ್ಣಾಙ್ಕ c_Max_ವಸ್ತುಗಳು,
  ಧ್ರುವ SCRIPT_CONTROL *psControl,
  ಧ್ರುವ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ವಸ್ತುಗಳು,
  OPENTYPE_TAG *pScriptTags,
  ಪೂರ್ಣಾಙ್ಕ *pc_ವಸ್ತುಗಳು
)
{
{
  ಪ್ರತ್ಯಾಗಮ ScriptItemize (pwcInChars,
			cInChars,
			c_Max_ವಸ್ತುಗಳು,
			psControl,
			psState,
			p_ವಸ್ತುಗಳು,
			pc_ವಸ್ತುಗಳು);
}
}

ಸ್ಥಿರ HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ಪೂರ್ಣಾಙ್ಕ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ಪೂರ್ಣಾಙ್ಕ cRanges,
  ಧ್ರುವ WCHAR *pwcChars,
  ಪೂರ್ಣಾಙ್ಕ cChars,
  ಪೂರ್ಣಾಙ್ಕ cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  ಪೂರ್ಣಾಙ್ಕ *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  ಪ್ರತ್ಯಾಗಮ ScriptShape (hdc,
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

ಸ್ಥಿರ HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ಪೂರ್ಣಾಙ್ಕ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ಪೂರ್ಣಾಙ್ಕ cRanges,
  ಧ್ರುವ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ಪೂರ್ಣಾಙ್ಕ cChars,
  ಧ್ರುವ WORD *pwGlyphs,
  ಧ್ರುವ SCRIPT_GLYPHPROP *pGlyphProps,
  ಪೂರ್ಣಾಙ್ಕ cGlyphs,
  ಪೂರ್ಣಾಙ್ಕ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  ಪ್ರತ್ಯಾಗಮ ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


ಸಂರಚನಾ hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  ಶೂನ್ಯ init ()
  {
    HMODULE hinstLib;
    ಏತತ್->ScriptItemizeOpenType = nullptr;
    ಏತತ್->ScriptShapeOpenType   = nullptr;
    ಏತತ್->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    ಯದಿ (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      ಏತತ್->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      ಏತತ್->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      ಏತತ್->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic ತೆಗೆ
    }
    ಯದಿ (!ಏತತ್->ScriptItemizeOpenType ||
	!ಏತತ್->ScriptShapeOpenType   ||
	!ಏತತ್->ScriptPlaceOpenType)
    {
      DEBUG_ಸಂದೇಶ (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      ಏತತ್->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      ಏತತ್->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      ಏತತ್->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_uniscribe_shaper_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_uniscribe_shaper_funcs_t));
    ಯದಿ (unlikely (!funcs))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((ಶೂನ್ಯ *) p);
  }
  ಸ್ಥಿರ hb_uniscribe_shaper_funcs_t *get_null ()
  {
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }
} static_uniscribe_shaper_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

ಸ್ಥಿರ hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  ಪ್ರತ್ಯಾಗಮ static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

ಸಂರಚನಾ hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ಸ್ಥಿರ ಶೂನ್ಯ
_hb_generate_unique_face_name (wchar_t *face_name, unsigned ಪೂರ್ಣಾಙ್ಕ *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ಧ್ರುವ ವರ್ಣ *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned ಪೂರ್ಣಾಙ್ಕ name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned ವರ್ಣ *p = (unsigned ವರ್ಣ *) &id;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < 16; i += 2)
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
  ಯದಿ (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ಸ್ಥಿರ hb_blob_t *
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

  unsigned ಪೂರ್ಣಾಙ್ಕ length, new_length, name_str_len;
  ಧ್ರುವ ವರ್ಣ *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ಸ್ಥಿರ ಧ್ರುವ uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned ಪೂರ್ಣಾಙ್ಕ name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned ಪೂರ್ಣಾಙ್ಕ padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned ಪೂರ್ಣಾಙ್ಕ name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  ಶೂನ್ಯ *new_sfnt_data = hb_calloc (1, new_length);
  ಯದಿ (!new_sfnt_data)
    ಪ್ರತ್ಯಾಗಮ nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.ಎಣಿಕೆ = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  unsigned ವರ್ಣ *p = &StructAfter<unsigned ವರ್ಣ> (name);
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ಧ್ರುವ OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned ಪೂರ್ಣಾಙ್ಕ face_ಎಣಿಕೆ = file.get_face_ಎಣಿಕೆ ();
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ face_index = 0; face_index < face_ಎಣಿಕೆ; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ಧ್ರುವ OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned ಪೂರ್ಣಾಙ್ಕ index;
    ಯದಿ (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    ಅನ್ಯಥಾ ಯದಿ (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  ಪ್ರತ್ಯಾಗಮ hb_blob_create ((ಧ್ರುವ ವರ್ಣ *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_uniscribe_face_data_t));
  ಯದಿ (unlikely (!data))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  ಯದಿ (unlikely (!data->funcs))
  {
    hb_free (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  ಯದಿ (unlikely (!hb_blob_get_length (blob)))
    DEBUG_ಸಂದೇಶ (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  ಯದಿ (unlikely (!blob))
  {
    hb_free (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  DWORD ಅಂಕೆ_fonts_installed;
  data->fh = AddFontMemResourceEx ((ಶೂನ್ಯ *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &ಅಂಕೆ_fonts_installed);
  ಯದಿ (unlikely (!data->fh))
  {
    DEBUG_ಸಂದೇಶ (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ data;
}

ಶೂನ್ಯ
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

ಸಂರಚನಾ hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  ದ್ವಿಗುಣ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned ಪೂರ್ಣಾಙ್ಕ font_size)
{
  hb_memset (lf, 0, ಪರಿಮಾಣ (*lf));
  lf->lfHeight = - (ಪೂರ್ಣಾಙ್ಕ) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, ಪರಿಮಾಣ (lf->lfFaceName));

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_uniscribe_font_data_t));
  ಯದಿ (unlikely (!data))
    ಪ್ರತ್ಯಾಗಮ nullptr;

  ಪೂರ್ಣಾಙ್ಕ font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  ಯದಿ (font->y_ppem)
    font_size = font->y_ppem;

  ಯದಿ (font_size < 0)
    font_size = -font_size;
  data->x_mult = (ದ್ವಿಗುಣ) font->x_scale / font_size;
  data->y_mult = (ದ್ವಿಗುಣ) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  ಯದಿ (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_ಸಂದೇಶ (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  ಯದಿ (unlikely (!data->hfont)) {
    DEBUG_ಸಂದೇಶ (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಯದಿ (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_ಸಂದೇಶ (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  ಪ್ರತ್ಯಾಗಮ data;
}

ಶೂನ್ಯ
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  ಯದಿ (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  ಯದಿ (data->hfont)
    DeleteObject (data->hfont);
  ಯದಿ (data->script_cache)
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
  ಧ್ರುವ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  ಪ್ರತ್ಯಾಗಮ data ? &data->log_font : nullptr;
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
  ಧ್ರುವ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  ಪ್ರತ್ಯಾಗಮ data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     ಧ್ರುವ hb_feature_t *features,
		     unsigned ಪೂರ್ಣಾಙ್ಕ        ಅಂಕೆ_features)
{
  hb_face_t *face = font->face;
  ಧ್ರುವ hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  ಧ್ರುವ hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_ಸಂದೇಶ (UNISCRIBE, nullptr, __VA_ARGS__); \
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned ಪೂರ್ಣಾಙ್ಕ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ಕುರು { \
    unsigned ಪೂರ್ಣಾಙ್ಕ _consumed = DIV_CEIL ((len) * ಪರಿಮಾಣ (Type), ಪರಿಮಾಣ (*scratch)); \
    ಪ್ರತಿಜ್ಞಾ (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ಯಾವತ್ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned ಪೂರ್ಣಾಙ್ಕ chars_len = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    ಯದಿ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ಅನ್ಯಥಾ ಯದಿ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ಅನ್ಯಥಾ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  ಯದಿ (ಅಂಕೆ_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned ಪೂರ್ಣಾಙ್ಕ cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      ಯದಿ (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned ಪೂರ್ಣಾಙ್ಕ glyphs_size = (scratch_size * ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ) - 2)
			   / (ಪರಿಮಾಣ (WORD) +
			      ಪರಿಮಾಣ (SCRIPT_GLYPHPROP) +
			      ಪರಿಮಾಣ (ಪೂರ್ಣಾಙ್ಕ) +
			      ಪರಿಮಾಣ (GOFFSET) +
			      ಪರಿಮಾಣ (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (ಪೂರ್ಣಾಙ್ಕ, advances, glyphs_size);
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

#define MAX_ವಸ್ತುಗಳು 256

  SCRIPT_ITEM ವಸ್ತುಗಳು[MAX_ವಸ್ತುಗಳು + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_ವಸ್ತುಗಳು];
  ಪೂರ್ಣಾಙ್ಕ item_ಎಣಿಕೆ_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_ವಸ್ತುಗಳು,
				     &bidi_control,
				     &bidi_state,
				     ವಸ್ತುಗಳು,
				     script_tags,
				     &item_ಎಣಿಕೆ_2);
  ಯದಿ (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_ವಸ್ತುಗಳು

  hb_tag_t lang_tag;
  unsigned ಪೂರ್ಣಾಙ್ಕ lang_ಎಣಿಕೆ = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_ಎಣಿಕೆ, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_ಎಣಿಕೆ ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((ಪರಿಮಾಣ (TEXTRANGE_PROPERTIES) == ಪರಿಮಾಣ (hb_ms_features_t)), "");
  static_assert ((ಪರಿಮಾಣ (OPENTYPE_FEATURE_RECORD) == ಪರಿಮಾಣ (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  ತಾರ್ಕಿಕ has_features = ಅಸತ್ಯ;
  ಯದಿ (ಅಂಕೆ_features)
    has_features = hb_ms_setup_features (features,
					 ಅಂಕೆ_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned ಪೂರ್ಣಾಙ್ಕ glyphs_offset = 0;
  unsigned ಪೂರ್ಣಾಙ್ಕ glyphs_len;
  ತಾರ್ಕಿಕ backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < item_ಎಣಿಕೆ_2; i++)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ chars_offset = ವಸ್ತುಗಳು[i].iCharPos;
    unsigned ಪೂರ್ಣಾಙ್ಕ item_chars_len = ವಸ್ತುಗಳು[i + 1].iCharPos - chars_offset;

    ಯದಿ (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    ವಸ್ತುಗಳು[i].a.fLogicalOrder = ಸತ್ಯ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ವಸ್ತುಗಳು[i].a,
				     script_tags[i],
				     language_tag,
				     (ಪೂರ್ಣಾಙ್ಕ *) range_char_counts.arrayZ,
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
				     (ಪೂರ್ಣಾಙ್ಕ *) &glyphs_len);

    ಯದಿ (unlikely (ವಸ್ತುಗಳು[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    ಯದಿ (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      ಯದಿ (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      ಗಚ್ಛ retry;
    }
    ಯದಿ (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      ಯದಿ (ವಸ್ತುಗಳು[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      ವಸ್ತುಗಳು[i].a.eScript = SCRIPT_UNDEFINED;
      ಗಚ್ಛ retry_shape;
    }
    ಯದಿ (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ವಸ್ತುಗಳು[i].a,
				     script_tags[i],
				     language_tag,
				     (ಪೂರ್ಣಾಙ್ಕ *) range_char_counts.arrayZ,
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
    ಯದಿ (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    ಯದಿ (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       ವಸ್ತುಗಳು[i].a.fRTL,
	       ವಸ್ತುಗಳು[i].a.fLayoutRTL,
	       ವಸ್ತುಗಳು[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < glyphs_len; i++)
    ಯದಿ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  ಯದಿ (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyphs_len; i++)
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
  ದ್ವಿಗುಣ x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    ದ್ವಿಗುಣ x_offset = backward ? -(ದ್ವಿಗುಣ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  ಯದಿ (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


#endif
