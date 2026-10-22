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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#અન્તર્ભાવ "hb-shaper-impl.hh"

#અન્તર્ભાવ <windows.h>
#અન્તર્ભાવ <usp10.h>
#અન્તર્ભાવ <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

#અન્તર્ભાવ "hb-uniscribe.h"

#અન્તર્ભાવ "hb-ms-feature-ranges.hh"
#અન્તર્ભાવ "hb-open-file.hh"
#અન્તર્ભાવ "hb-ot-name-table.hh"
#અન્તર્ભાવ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

પ્રકારવ્યાખ્યા HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  અચલ WCHAR *pwcInChars,
  પૂર્ણાંક cInChars,
  પૂર્ણાંક c_Max_વસ્તુઓ,
  અચલ SCRIPT_CONTROL *psControl,
  અચલ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_વસ્તુઓ,
  OPENTYPE_TAG *pScriptTags,
  પૂર્ણાંક *pc_વસ્તુઓ
);

પ્રકારવ્યાખ્યા HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  પૂર્ણાંક *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  પૂર્ણાંક cRanges,
  અચલ WCHAR *pwcChars,
  પૂર્ણાંક cChars,
  પૂર્ણાંક cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  પૂર્ણાંક *pcGlyphs
);

પ્રકારવ્યાખ્યા HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  પૂર્ણાંક *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  પૂર્ણાંક cRanges,
  અચલ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  પૂર્ણાંક cChars,
  અચલ WORD *pwGlyphs,
  અચલ SCRIPT_GLYPHPROP *pGlyphProps,
  પૂર્ણાંક cGlyphs,
  પૂર્ણાંક *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

સ્થિર HRESULT WINAPI
hb_ScriptItemizeOpenType(
  અચલ WCHAR *pwcInChars,
  પૂર્ણાંક cInChars,
  પૂર્ણાંક c_Max_વસ્તુઓ,
  અચલ SCRIPT_CONTROL *psControl,
  અચલ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_વસ્તુઓ,
  OPENTYPE_TAG *pScriptTags,
  પૂર્ણાંક *pc_વસ્તુઓ
)
{
{
  પાછા ScriptItemize (pwcInChars,
			cInChars,
			c_Max_વસ્તુઓ,
			psControl,
			psState,
			p_વસ્તુઓ,
			pc_વસ્તુઓ);
}
}

સ્થિર HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  પૂર્ણાંક *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  પૂર્ણાંક cRanges,
  અચલ WCHAR *pwcChars,
  પૂર્ણાંક cChars,
  પૂર્ણાંક cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  પૂર્ણાંક *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  પાછા ScriptShape (hdc,
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

સ્થિર HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  પૂર્ણાંક *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  પૂર્ણાંક cRanges,
  અચલ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  પૂર્ણાંક cChars,
  અચલ WORD *pwGlyphs,
  અચલ SCRIPT_GLYPHPROP *pGlyphProps,
  પૂર્ણાંક cGlyphs,
  પૂર્ણાંક *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  પાછા ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


સંરચના hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  શૂન્ય init ()
  {
    HMODULE hinstLib;
    આ->ScriptItemizeOpenType = nullptr;
    આ->ScriptShapeOpenType   = nullptr;
    આ->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    જો (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      આ->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      આ->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      આ->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic કાઢો
    }
    જો (!આ->ScriptItemizeOpenType ||
	!આ->ScriptShapeOpenType   ||
	!આ->ScriptPlaceOpenType)
    {
      DEBUG_સંદેશ (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      આ->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      આ->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      આ->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

સ્થિર અંતઃરેખ શૂન્ય free_static_uniscribe_shaper_funcs ();

સ્થિર સંરચના hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  સ્થિર hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, કદ (hb_uniscribe_shaper_funcs_t));
    જો (unlikely (!funcs))
      પાછા nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    પાછા funcs;
  }
  સ્થિર શૂન્ય destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((શૂન્ય *) p);
  }
  સ્થિર hb_uniscribe_shaper_funcs_t *get_null ()
  {
    પાછા nullptr;
  }
} static_uniscribe_shaper_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

સ્થિર hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  પાછા static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

સંરચના hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
સ્થિર શૂન્ય
_hb_generate_unique_face_name (wchar_t *face_name, unsigned પૂર્ણાંક *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  અચલ અક્ષર *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned પૂર્ણાંક name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned અક્ષર *p = (unsigned અક્ષર *) &id;
  માટે (unsigned પૂર્ણાંક i = 0; i < 16; i += 2)
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
  જો (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
સ્થિર hb_blob_t *
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

  unsigned પૂર્ણાંક length, new_length, name_str_len;
  અચલ અક્ષર *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  સ્થિર અચલ uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned પૂર્ણાંક name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned પૂર્ણાંક padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned પૂર્ણાંક name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  શૂન્ય *new_sfnt_data = hb_calloc (1, new_length);
  જો (!new_sfnt_data)
    પાછા nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.ગણતરી = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  unsigned અક્ષર *p = &StructAfter<unsigned અક્ષર> (name);
  માટે (unsigned પૂર્ણાંક i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  અચલ OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned પૂર્ણાંક face_ગણતરી = file.get_face_ગણતરી ();
  માટે (unsigned પૂર્ણાંક face_index = 0; face_index < face_ગણતરી; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    અચલ OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned પૂર્ણાંક index;
    જો (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    નહીંતર જો (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      પાછા nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  પાછા hb_blob_create ((અચલ અક્ષર *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, કદ (hb_uniscribe_face_data_t));
  જો (unlikely (!data))
    પાછા nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  જો (unlikely (!data->funcs))
  {
    hb_free (data);
    પાછા nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  જો (unlikely (!hb_blob_get_length (blob)))
    DEBUG_સંદેશ (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  જો (unlikely (!blob))
  {
    hb_free (data);
    પાછા nullptr;
  }

  DWORD અંક_fonts_installed;
  data->fh = AddFontMemResourceEx ((શૂન્ય *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &અંક_fonts_installed);
  જો (unlikely (!data->fh))
  {
    DEBUG_સંદેશ (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    પાછા nullptr;
  }

  પાછા data;
}

શૂન્ય
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

સંરચના hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  દ્વિગુણ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

સ્થિર તાર્કિક
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned પૂર્ણાંક font_size)
{
  hb_memset (lf, 0, કદ (*lf));
  lf->lfHeight = - (પૂર્ણાંક) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, કદ (lf->lfFaceName));

  પાછા સાચું;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, કદ (hb_uniscribe_font_data_t));
  જો (unlikely (!data))
    પાછા nullptr;

  પૂર્ણાંક font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  જો (font->y_ppem)
    font_size = font->y_ppem;

  જો (font_size < 0)
    font_size = -font_size;
  data->x_mult = (દ્વિગુણ) font->x_scale / font_size;
  data->y_mult = (દ્વિગુણ) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  જો (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_સંદેશ (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    પાછા nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  જો (unlikely (!data->hfont)) {
    DEBUG_સંદેશ (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     પાછા nullptr;
  }

  જો (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_સંદેશ (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     પાછા nullptr;
  }

  પાછા data;
}

શૂન્ય
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  જો (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  જો (data->hfont)
    DeleteObject (data->hfont);
  જો (data->script_cache)
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
  અચલ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  પાછા data ? &data->log_font : nullptr;
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
  અચલ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  પાછા data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     અચલ hb_feature_t *features,
		     unsigned પૂર્ણાંક        અંક_features)
{
  hb_face_t *face = font->face;
  અચલ hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  અચલ hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_સંદેશ (UNISCRIBE, nullptr, __VA_ARGS__); \
    પાછા ખોટું; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned પૂર્ણાંક scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  કરો { \
    unsigned પૂર્ણાંક _consumed = DIV_CEIL ((len) * કદ (Type), કદ (*scratch)); \
    પ્રતિજ્ઞા (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } જ્યારે (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned પૂર્ણાંક chars_len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    જો (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    નહીંતર જો (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    નહીંતર {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  જો (અંક_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned પૂર્ણાંક cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      જો (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned પૂર્ણાંક glyphs_size = (scratch_size * કદ (પૂર્ણાંક) - 2)
			   / (કદ (WORD) +
			      કદ (SCRIPT_GLYPHPROP) +
			      કદ (પૂર્ણાંક) +
			      કદ (GOFFSET) +
			      કદ (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (પૂર્ણાંક, advances, glyphs_size);
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

#define MAX_વસ્તુઓ 256

  SCRIPT_ITEM વસ્તુઓ[MAX_વસ્તુઓ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_વસ્તુઓ];
  પૂર્ણાંક item_ગણતરી_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_વસ્તુઓ,
				     &bidi_control,
				     &bidi_state,
				     વસ્તુઓ,
				     script_tags,
				     &item_ગણતરી_2);
  જો (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_વસ્તુઓ

  hb_tag_t lang_tag;
  unsigned પૂર્ણાંક lang_ગણતરી = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_ગણતરી, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_ગણતરી ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((કદ (TEXTRANGE_PROPERTIES) == કદ (hb_ms_features_t)), "");
  static_assert ((કદ (OPENTYPE_FEATURE_RECORD) == કદ (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  તાર્કિક has_features = ખોટું;
  જો (અંક_features)
    has_features = hb_ms_setup_features (features,
					 અંક_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned પૂર્ણાંક glyphs_offset = 0;
  unsigned પૂર્ણાંક glyphs_len;
  તાર્કિક backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  માટે (પૂર્ણાંક i = 0; i < item_ગણતરી_2; i++)
  {
    unsigned પૂર્ણાંક chars_offset = વસ્તુઓ[i].iCharPos;
    unsigned પૂર્ણાંક item_chars_len = વસ્તુઓ[i + 1].iCharPos - chars_offset;

    જો (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    વસ્તુઓ[i].a.fLogicalOrder = સાચું;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &વસ્તુઓ[i].a,
				     script_tags[i],
				     language_tag,
				     (પૂર્ણાંક *) range_char_counts.arrayZ,
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
				     (પૂર્ણાંક *) &glyphs_len);

    જો (unlikely (વસ્તુઓ[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    જો (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      જો (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      જાઓ retry;
    }
    જો (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      જો (વસ્તુઓ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      વસ્તુઓ[i].a.eScript = SCRIPT_UNDEFINED;
      જાઓ retry_shape;
    }
    જો (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    માટે (unsigned પૂર્ણાંક j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &વસ્તુઓ[i].a,
				     script_tags[i],
				     language_tag,
				     (પૂર્ણાંક *) range_char_counts.arrayZ,
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
    જો (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    જો (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       વસ્તુઓ[i].a.fRTL,
	       વસ્તુઓ[i].a.fLayoutRTL,
	       વસ્તુઓ[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  માટે (unsigned પૂર્ણાંક i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  માટે (unsigned પૂર્ણાંક i = 1; i < glyphs_len; i++)
    જો (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  જો (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < glyphs_len; i++)
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
  દ્વિગુણ x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  માટે (unsigned પૂર્ણાંક i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    દ્વિગુણ x_offset = backward ? -(દ્વિગુણ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  જો (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  પાછા સાચું;
}


#endif
