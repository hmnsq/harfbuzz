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

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#ضمن "hb-shaper-impl.hh"

#ضمن <windows.h>
#ضمن <usp10.h>
#ضمن <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

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
  ثابت WCHAR *pwcInChars,
  صحيح cInChars,
  صحيح cMaxItems,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_ITEM *pItems,
  OPENTYPE_TAG *pScriptTags,
  صحيح *pcItems
);

تعريفنوع HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  صحيح *pcGlyphs
);

تعريفنوع HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت WORD *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ساكن HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ثابت WCHAR *pwcInChars,
  صحيح cInChars,
  صحيح cMaxItems,
  ثابت SCRIPT_CONTROL *psControl,
  ثابت SCRIPT_STATE *psState,
  SCRIPT_ITEM *pItems,
  OPENTYPE_TAG *pScriptTags,
  صحيح *pcItems
)
{
{
  החזר ScriptItemize (pwcInChars,
			cInChars,
			cMaxItems,
			psControl,
			psState,
			pItems,
			pcItems);
}
}

ساكن HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  صحيح cChars,
  صحيح cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  صحيح *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  החזר ScriptShape (hdc,
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

ساكن HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  صحيح *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  صحيح cRanges,
  ثابت WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  صحيح cChars,
  ثابت WORD *pwGlyphs,
  ثابت SCRIPT_GLYPHPROP *pGlyphProps,
  صحيح cGlyphs,
  صحيح *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  החזר ScriptPlace (hdc,
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
  SPOT ScriptPlaceOpenType;

  فراغ init ()
  {
    HMODULE hinstLib;
    هذا->ScriptItemizeOpenType = nullptr;
    هذا->ScriptShapeOpenType   = nullptr;
    هذا->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    אם (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      هذا->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      هذا->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      هذا->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic pop
    }
    אם (!هذا->ScriptItemizeOpenType ||
	!هذا->ScriptShapeOpenType   ||
	!هذا->ScriptPlaceOpenType)
    {
      DEBUG_MSG (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      هذا->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      هذا->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      هذا->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
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
    אם (unlikely (!funcs))
      החזר nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    החזר funcs;
  }
  ساكن فراغ destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((فراغ *) p);
  }
  ساكن hb_uniscribe_shaper_funcs_t *get_null ()
  {
    החזר nullptr;
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
  החזר static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

بنية hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ساكن فراغ
_hb_generate_unique_face_name (wchar_t *face_name, unsigned صحيح *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ثابت حرف *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned صحيح name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned حرف *p = (unsigned حرف *) &id;
  עבור (unsigned صحيح i = 0; i < 16; i += 2)
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
  אם (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ساكن hb_blob_t *
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

  unsigned صحيح length, new_length, name_str_len;
  ثابت حرف *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ساكن ثابت uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned صحيح name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned صحيح padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned صحيح name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  فراغ *new_sfnt_data = hb_calloc (1, new_length);
  אם (!new_sfnt_data)
    החזר nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.count = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  עבור (unsigned صحيح i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  unsigned حرف *p = &StructAfter<unsigned حرف> (name);
  עבור (unsigned صحيح i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ثابت OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned صحيح face_count = file.get_face_count ();
  עבור (unsigned صحيح face_index = 0; face_index < face_count; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ثابت OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned صحيح index;
    אם (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    אחרת אם (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      החזר nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  החזר hb_blob_create ((ثابت حرف *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, حجم (hb_uniscribe_face_data_t));
  אם (unlikely (!data))
    החזר nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  אם (unlikely (!data->funcs))
  {
    hb_free (data);
    החזר nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  אם (unlikely (!hb_blob_get_length (blob)))
    DEBUG_MSG (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  אם (unlikely (!blob))
  {
    hb_free (data);
    החזר nullptr;
  }

  DWORD num_fonts_installed;
  data->fh = AddFontMemResourceEx ((فراغ *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &num_fonts_installed);
  אם (unlikely (!data->fh))
  {
    DEBUG_MSG (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    החזר nullptr;
  }

  החזר data;
}

فراغ
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

بنية hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  مزدوج x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ساكن منطقي
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned صحيح font_size)
{
  hb_memset (lf, 0, حجم (*lf));
  lf->lfHeight = - (صحيح) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, حجم (lf->lfFaceName));

  החזר אמת;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, حجم (hb_uniscribe_font_data_t));
  אם (unlikely (!data))
    החזר nullptr;

  صحيح font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  אם (font->y_ppem)
    font_size = font->y_ppem;

  אם (font_size < 0)
    font_size = -font_size;
  data->x_mult = (مزدوج) font->x_scale / font_size;
  data->y_mult = (مزدوج) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  אם (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_MSG (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    החזר nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  אם (unlikely (!data->hfont)) {
    DEBUG_MSG (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     החזר nullptr;
  }

  אם (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_MSG (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     החזר nullptr;
  }

  החזר data;
}

فراغ
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  אם (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  אם (data->hfont)
    DeleteObject (data->hfont);
  אם (data->script_cache)
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
  ثابت hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  החזר data ? &data->log_font : nullptr;
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
  ثابت hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  החזר data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     ثابت hb_feature_t *features,
		     unsigned صحيح        num_features)
{
  hb_face_t *face = font->face;
  ثابت hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  ثابت hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_MSG (UNISCRIBE, nullptr, __VA_ARGS__); \
    החזר שקר; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned صحيح scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  עשה { \
    unsigned صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    أكد (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } כלעוד (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned صحيح chars_len = 0;
  עבור (unsigned صحيح i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    אם (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    אחרת אם (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    אחרת {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  אם (num_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    עבור (unsigned صحيح i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned صحيح cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      אם (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned صحيح glyphs_size = (scratch_size * حجم (صحيح) - 2)
			   / (حجم (WORD) +
			      حجم (SCRIPT_GLYPHPROP) +
			      حجم (صحيح) +
			      حجم (GOFFSET) +
			      حجم (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (صحيح, advances, glyphs_size);
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

#define MAX_ITEMS 256

  SCRIPT_ITEM items[MAX_ITEMS + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_ITEMS];
  صحيح item_count;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_ITEMS,
				     &bidi_control,
				     &bidi_state,
				     items,
				     script_tags,
				     &item_count);
  אם (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_ITEMS

  hb_tag_t lang_tag;
  unsigned صحيح lang_count = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_count, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_count ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((حجم (TEXTRANGE_PROPERTIES) == حجم (hb_ms_features_t)), "");
  static_assert ((حجم (OPENTYPE_FEATURE_RECORD) == حجم (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  منطقي has_features = שקר;
  אם (num_features)
    has_features = hb_ms_setup_features (features,
					 num_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned صحيح glyphs_offset = 0;
  unsigned صحيح glyphs_len;
  منطقي backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  עבור (صحيح i = 0; i < item_count; i++)
  {
    unsigned صحيح chars_offset = items[i].iCharPos;
    unsigned صحيح item_chars_len = items[i + 1].iCharPos - chars_offset;

    אם (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    items[i].a.fLogicalOrder = אמת;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &items[i].a,
				     script_tags[i],
				     language_tag,
				     (صحيح *) range_char_counts.arrayZ,
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
				     (صحيح *) &glyphs_len);

    אם (unlikely (items[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    אם (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      אם (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      اذهب retry;
    }
    אם (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      אם (items[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      items[i].a.eScript = SCRIPT_UNDEFINED;
      اذهب retry_shape;
    }
    אם (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    עבור (unsigned صحيح j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &items[i].a,
				     script_tags[i],
				     language_tag,
				     (صحيح *) range_char_counts.arrayZ,
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
    אם (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    אם (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       items[i].a.fRTL,
	       items[i].a.fLayoutRTL,
	       items[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  עבור (unsigned صحيح i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  עבור (unsigned صحيح i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  עבור (unsigned صحيح i = 1; i < glyphs_len; i++)
    אם (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  אם (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  עבור (unsigned صحيح i = 0; i < glyphs_len; i++)
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
  مزدوج x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  עבור (unsigned صحيح i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    مزدوج x_offset = backward ? -(مزدوج) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  אם (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  החזר אמת;
}


#endif
