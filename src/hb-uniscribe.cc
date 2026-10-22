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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#अन्तर्भाव "hb-shaper-impl.hh"

#अन्तर्भाव <windows.h>
#अन्तर्भाव <usp10.h>
#अन्तर्भाव <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

#अन्तर्भाव "hb-uniscribe.h"

#अन्तर्भाव "hb-ms-feature-ranges.hh"
#अन्तर्भाव "hb-open-file.hh"
#अन्तर्भाव "hb-ot-name-table.hh"
#अन्तर्भाव "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

प्रकारव्याख्या HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  स्थिरांक WCHAR *pwcInChars,
  पूर्णांक cInChars,
  पूर्णांक c_Max_वस्तुसंच,
  स्थिरांक SCRIPT_CONTROL *psControl,
  स्थिरांक SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुसंच,
  OPENTYPE_TAG *pScriptTags,
  पूर्णांक *pc_वस्तुसंच
);

प्रकारव्याख्या HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिरांक WCHAR *pwcChars,
  पूर्णांक cChars,
  पूर्णांक cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  पूर्णांक *pcGlyphs
);

प्रकारव्याख्या HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिरांक WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णांक cChars,
  स्थिरांक WORD *pwGlyphs,
  स्थिरांक SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णांक cGlyphs,
  पूर्णांक *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

स्थिर HRESULT WINAPI
hb_ScriptItemizeOpenType(
  स्थिरांक WCHAR *pwcInChars,
  पूर्णांक cInChars,
  पूर्णांक c_Max_वस्तुसंच,
  स्थिरांक SCRIPT_CONTROL *psControl,
  स्थिरांक SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुसंच,
  OPENTYPE_TAG *pScriptTags,
  पूर्णांक *pc_वस्तुसंच
)
{
{
  परत ScriptItemize (pwcInChars,
			cInChars,
			c_Max_वस्तुसंच,
			psControl,
			psState,
			p_वस्तुसंच,
			pc_वस्तुसंच);
}
}

स्थिर HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिरांक WCHAR *pwcChars,
  पूर्णांक cChars,
  पूर्णांक cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  पूर्णांक *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  परत ScriptShape (hdc,
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

स्थिर HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिरांक WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णांक cChars,
  स्थिरांक WORD *pwGlyphs,
  स्थिरांक SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णांक cGlyphs,
  पूर्णांक *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  परत ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


संरचना hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  शून्य init ()
  {
    HMODULE hinstLib;
    हे->ScriptItemizeOpenType = nullptr;
    हे->ScriptShapeOpenType   = nullptr;
    हे->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    जर (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      हे->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      हे->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      हे->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic काढा
    }
    जर (!हे->ScriptItemizeOpenType ||
	!हे->ScriptShapeOpenType   ||
	!हे->ScriptPlaceOpenType)
    {
      DEBUG_संदेश (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      हे->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      हे->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      हे->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

स्थिर ओळीत शून्य free_static_uniscribe_shaper_funcs ();

स्थिर संरचना hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  स्थिर hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, आकार (hb_uniscribe_shaper_funcs_t));
    जर (unlikely (!funcs))
      परत nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    परत funcs;
  }
  स्थिर शून्य destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((शून्य *) p);
  }
  स्थिर hb_uniscribe_shaper_funcs_t *get_null ()
  {
    परत nullptr;
  }
} static_uniscribe_shaper_funcs;

स्थिर ओळीत
शून्य free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

स्थिर hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  परत static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

संरचना hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  ब_अक्षर_प्रकार face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
स्थिर शून्य
_hb_generate_unique_face_name (ब_अक्षर_प्रकार *face_name, अचिन्हित पूर्णांक *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  स्थिरांक अक्षर *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  अचिन्हित पूर्णांक name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  अचिन्हित अक्षर *p = (अचिन्हित अक्षर *) &id;
  साठी (अचिन्हित पूर्णांक i = 0; i < 16; i += 2)
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
  जर (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
स्थिर hb_blob_t *
_hb_rename_font (hb_blob_t *blob_in, ब_अक्षर_प्रकार *new_name)
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

  अचिन्हित पूर्णांक length, new_length, name_str_len;
  स्थिरांक अक्षर *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  स्थिर स्थिरांक uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  अचिन्हित पूर्णांक name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  अचिन्हित पूर्णांक padded_name_table_length = ((name_table_length + 3) & ~3);
  अचिन्हित पूर्णांक name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  शून्य *new_sfnt_data = hb_calloc (1, new_length);
  जर (!new_sfnt_data)
    परत nullptr;
  स्वतः sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.मोजणी = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  अचिन्हित अक्षर *p = &StructAfter<अचिन्हित अक्षर> (name);
  साठी (अचिन्हित पूर्णांक i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  स्थिरांक OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  अचिन्हित पूर्णांक face_मोजणी = file.get_face_मोजणी ();
  साठी (अचिन्हित पूर्णांक face_index = 0; face_index < face_मोजणी; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    स्थिरांक OT::OpenTypeFontFace &face = file.get_face (face_index);
    अचिन्हित पूर्णांक index;
    जर (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    अन्यथा जर (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      परत nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  परत hb_blob_create ((स्थिरांक अक्षर *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, आकार (hb_uniscribe_face_data_t));
  जर (unlikely (!data))
    परत nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  जर (unlikely (!data->funcs))
  {
    hb_free (data);
    परत nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  जर (unlikely (!hb_blob_get_length (blob)))
    DEBUG_संदेश (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  जर (unlikely (!blob))
  {
    hb_free (data);
    परत nullptr;
  }

  DWORD अंक_fonts_installed;
  data->fh = AddFontMemResourceEx ((शून्य *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &अंक_fonts_installed);
  जर (unlikely (!data->fh))
  {
    DEBUG_संदेश (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    परत nullptr;
  }

  परत data;
}

शून्य
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

संरचना hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  द्विगुण x_mult, y_mult; /* From LOGFONT space to HB space. */
};

स्थिर तार्किक
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   अचिन्हित पूर्णांक font_size)
{
  hb_memset (lf, 0, आकार (*lf));
  lf->lfHeight = - (पूर्णांक) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, आकार (lf->lfFaceName));

  परत सत्य;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, आकार (hb_uniscribe_font_data_t));
  जर (unlikely (!data))
    परत nullptr;

  पूर्णांक font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  जर (font->y_ppem)
    font_size = font->y_ppem;

  जर (font_size < 0)
    font_size = -font_size;
  data->x_mult = (द्विगुण) font->x_scale / font_size;
  data->y_mult = (द्विगुण) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  जर (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_संदेश (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    परत nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  जर (unlikely (!data->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     परत nullptr;
  }

  जर (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     परत nullptr;
  }

  परत data;
}

शून्य
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  जर (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  जर (data->hfont)
    DeleteObject (data->hfont);
  जर (data->script_cache)
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
  स्थिरांक hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  परत data ? &data->log_font : nullptr;
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
  स्थिरांक hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  परत data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     स्थिरांक hb_feature_t *features,
		     अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  स्थिरांक hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  स्थिरांक hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (UNISCRIBE, nullptr, __VA_ARGS__); \
    परत असत्य; \
  } HB_STMT_END

  HRESULT hr;

retry:

  अचिन्हित पूर्णांक scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  करा { \
    अचिन्हित पूर्णांक _consumed = DIV_CEIL ((len) * आकार (Type), आकार (*scratch)); \
    प्रतिज्ञा (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } पर्यंत (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  अचिन्हित पूर्णांक chars_len = 0;
  साठी (अचिन्हित पूर्णांक i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    जर (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    अन्यथा जर (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    अन्यथा {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  जर (अंक_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    साठी (अचिन्हित पूर्णांक i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      अचिन्हित पूर्णांक cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      जर (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  अचिन्हित पूर्णांक glyphs_size = (scratch_size * आकार (पूर्णांक) - 2)
			   / (आकार (WORD) +
			      आकार (SCRIPT_GLYPHPROP) +
			      आकार (पूर्णांक) +
			      आकार (GOFFSET) +
			      आकार (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (पूर्णांक, advances, glyphs_size);
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

#define MAX_वस्तुसंच 256

  SCRIPT_ITEM वस्तुसंच[MAX_वस्तुसंच + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_वस्तुसंच];
  पूर्णांक item_मोजणी_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_वस्तुसंच,
				     &bidi_control,
				     &bidi_state,
				     वस्तुसंच,
				     script_tags,
				     &item_मोजणी_2);
  जर (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);

#undef MAX_वस्तुसंच

  hb_tag_t lang_tag;
  अचिन्हित पूर्णांक lang_मोजणी = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_मोजणी, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_मोजणी ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((आकार (TEXTRANGE_PROPERTIES) == आकार (hb_ms_features_t)), "");
  static_assert ((आकार (OPENTYPE_FEATURE_RECORD) == आकार (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  तार्किक has_features = असत्य;
  जर (अंक_features)
    has_features = hb_ms_setup_features (features,
					 अंक_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  अचिन्हित पूर्णांक glyphs_offset = 0;
  अचिन्हित पूर्णांक glyphs_len;
  तार्किक backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  साठी (पूर्णांक i = 0; i < item_मोजणी_2; i++)
  {
    अचिन्हित पूर्णांक chars_offset = वस्तुसंच[i].iCharPos;
    अचिन्हित पूर्णांक item_chars_len = वस्तुसंच[i + 1].iCharPos - chars_offset;

    जर (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    वस्तुसंच[i].a.fLogicalOrder = सत्य;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &वस्तुसंच[i].a,
				     script_tags[i],
				     language_tag,
				     (पूर्णांक *) range_char_counts.arrayZ,
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
				     (पूर्णांक *) &glyphs_len);

    जर (unlikely (वस्तुसंच[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    जर (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      जर (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      जा retry;
    }
    जर (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      जर (वस्तुसंच[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      वस्तुसंच[i].a.eScript = SCRIPT_UNDEFINED;
      जा retry_shape;
    }
    जर (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);
    }

    साठी (अचिन्हित पूर्णांक j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &वस्तुसंच[i].a,
				     script_tags[i],
				     language_tag,
				     (पूर्णांक *) range_char_counts.arrayZ,
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
    जर (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);

    जर (DEBUG_ENABLED (UNISCRIBE))
      ख_लिखो (मानक_त्रुटि, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       वस्तुसंच[i].a.fRTL,
	       वस्तुसंच[i].a.fLayoutRTL,
	       वस्तुसंच[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  साठी (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  साठी (अचिन्हित पूर्णांक i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  साठी (अचिन्हित पूर्णांक i = 1; i < glyphs_len; i++)
    जर (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  जर (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  साठी (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
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
  द्विगुण x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  साठी (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    द्विगुण x_offset = backward ? -(द्विगुण) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  जर (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  परत सत्य;
}


#endif
