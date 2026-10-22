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

प्रकारनिर्देश HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ध्रुव WCHAR *pwcInChars,
  पूर्णाङ्क cInChars,
  पूर्णाङ्क c_Max_वस्तुसंच,
  ध्रुव SCRIPT_CONTROL *psControl,
  ध्रुव SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुसंच,
  OPENTYPE_TAG *pScriptTags,
  पूर्णाङ्क *pc_वस्तुसंच
);

प्रकारनिर्देश HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णाङ्क *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णाङ्क cRanges,
  ध्रुव WCHAR *pwcChars,
  पूर्णाङ्क cChars,
  पूर्णाङ्क cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  पूर्णाङ्क *pcGlyphs
);

प्रकारनिर्देश HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णाङ्क *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णाङ्क cRanges,
  ध्रुव WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णाङ्क cChars,
  ध्रुव WORD *pwGlyphs,
  ध्रुव SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णाङ्क cGlyphs,
  पूर्णाङ्क *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

स्थिर HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ध्रुव WCHAR *pwcInChars,
  पूर्णाङ्क cInChars,
  पूर्णाङ्क c_Max_वस्तुसंच,
  ध्रुव SCRIPT_CONTROL *psControl,
  ध्रुव SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुसंच,
  OPENTYPE_TAG *pScriptTags,
  पूर्णाङ्क *pc_वस्तुसंच
)
{
{
  प्रत्यागम ScriptItemize (pwcInChars,
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
  पूर्णाङ्क *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णाङ्क cRanges,
  ध्रुव WCHAR *pwcChars,
  पूर्णाङ्क cChars,
  पूर्णाङ्क cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  पूर्णाङ्क *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  प्रत्यागम ScriptShape (hdc,
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
  पूर्णाङ्क *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णाङ्क cRanges,
  ध्रुव WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णाङ्क cChars,
  ध्रुव WORD *pwGlyphs,
  ध्रुव SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णाङ्क cGlyphs,
  पूर्णाङ्क *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  प्रत्यागम ScriptPlace (hdc,
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
    एतत्->ScriptItemizeOpenType = nullptr;
    एतत्->ScriptShapeOpenType   = nullptr;
    एतत्->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    यदि (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      एतत्->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      एतत्->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      एतत्->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic काढा
    }
    यदि (!एतत्->ScriptItemizeOpenType ||
	!एतत्->ScriptShapeOpenType   ||
	!एतत्->ScriptPlaceOpenType)
    {
      DEBUG_संदेश (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      एतत्->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      एतत्->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      एतत्->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

स्थिर अन्तःस्थ शून्य free_static_uniscribe_shaper_funcs ();

स्थिर संरचना hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  स्थिर hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, परिमाण (hb_uniscribe_shaper_funcs_t));
    यदि (unlikely (!funcs))
      प्रत्यागम nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    प्रत्यागम funcs;
  }
  स्थिर शून्य destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((शून्य *) p);
  }
  स्थिर hb_uniscribe_shaper_funcs_t *get_null ()
  {
    प्रत्यागम nullptr;
  }
} static_uniscribe_shaper_funcs;

स्थिर अन्तःस्थ
शून्य free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

स्थिर hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  प्रत्यागम static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

संरचना hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
स्थिर शून्य
_hb_generate_unique_face_name (wchar_t *face_name, unsigned पूर्णाङ्क *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ध्रुव वर्ण *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned पूर्णाङ्क name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned वर्ण *p = (unsigned वर्ण *) &id;
  प्रति (unsigned पूर्णाङ्क i = 0; i < 16; i += 2)
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
  यदि (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
स्थिर hb_blob_t *
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

  unsigned पूर्णाङ्क length, new_length, name_str_len;
  ध्रुव वर्ण *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  स्थिर ध्रुव uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned पूर्णाङ्क name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned पूर्णाङ्क padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned पूर्णाङ्क name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  शून्य *new_sfnt_data = hb_calloc (1, new_length);
  यदि (!new_sfnt_data)
    प्रत्यागम nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.मोजणी = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  प्रति (unsigned पूर्णाङ्क i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  unsigned वर्ण *p = &StructAfter<unsigned वर्ण> (name);
  प्रति (unsigned पूर्णाङ्क i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ध्रुव OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned पूर्णाङ्क face_मोजणी = file.get_face_मोजणी ();
  प्रति (unsigned पूर्णाङ्क face_index = 0; face_index < face_मोजणी; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ध्रुव OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned पूर्णाङ्क index;
    यदि (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    अन्यथा यदि (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      प्रत्यागम nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  प्रत्यागम hb_blob_create ((ध्रुव वर्ण *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, परिमाण (hb_uniscribe_face_data_t));
  यदि (unlikely (!data))
    प्रत्यागम nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  यदि (unlikely (!data->funcs))
  {
    hb_free (data);
    प्रत्यागम nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  यदि (unlikely (!hb_blob_get_length (blob)))
    DEBUG_संदेश (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  यदि (unlikely (!blob))
  {
    hb_free (data);
    प्रत्यागम nullptr;
  }

  DWORD अंक_fonts_installed;
  data->fh = AddFontMemResourceEx ((शून्य *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &अंक_fonts_installed);
  यदि (unlikely (!data->fh))
  {
    DEBUG_संदेश (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    प्रत्यागम nullptr;
  }

  प्रत्यागम data;
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
		   unsigned पूर्णाङ्क font_size)
{
  hb_memset (lf, 0, परिमाण (*lf));
  lf->lfHeight = - (पूर्णाङ्क) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, परिमाण (lf->lfFaceName));

  प्रत्यागम सत्य;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, परिमाण (hb_uniscribe_font_data_t));
  यदि (unlikely (!data))
    प्रत्यागम nullptr;

  पूर्णाङ्क font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  यदि (font->y_ppem)
    font_size = font->y_ppem;

  यदि (font_size < 0)
    font_size = -font_size;
  data->x_mult = (द्विगुण) font->x_scale / font_size;
  data->y_mult = (द्विगुण) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  यदि (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_संदेश (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    प्रत्यागम nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  यदि (unlikely (!data->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     प्रत्यागम nullptr;
  }

  यदि (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     प्रत्यागम nullptr;
  }

  प्रत्यागम data;
}

शून्य
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  यदि (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  यदि (data->hfont)
    DeleteObject (data->hfont);
  यदि (data->script_cache)
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
  ध्रुव hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  प्रत्यागम data ? &data->log_font : nullptr;
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
  ध्रुव hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  प्रत्यागम data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     ध्रुव hb_feature_t *features,
		     unsigned पूर्णाङ्क        अंक_features)
{
  hb_face_t *face = font->face;
  ध्रुव hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  ध्रुव hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (UNISCRIBE, nullptr, __VA_ARGS__); \
    प्रत्यागम असत्य; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned पूर्णाङ्क scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  कुरु { \
    unsigned पूर्णाङ्क _consumed = DIV_CEIL ((len) * परिमाण (Type), परिमाण (*scratch)); \
    प्रतिज्ञा (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } यावत् (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned पूर्णाङ्क chars_len = 0;
  प्रति (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    यदि (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    अन्यथा यदि (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    अन्यथा {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  यदि (अंक_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    प्रति (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned पूर्णाङ्क cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      यदि (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned पूर्णाङ्क glyphs_size = (scratch_size * परिमाण (पूर्णाङ्क) - 2)
			   / (परिमाण (WORD) +
			      परिमाण (SCRIPT_GLYPHPROP) +
			      परिमाण (पूर्णाङ्क) +
			      परिमाण (GOFFSET) +
			      परिमाण (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (पूर्णाङ्क, advances, glyphs_size);
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
  पूर्णाङ्क item_मोजणी_2;

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
  यदि (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_वस्तुसंच

  hb_tag_t lang_tag;
  unsigned पूर्णाङ्क lang_मोजणी = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_मोजणी, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_मोजणी ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((परिमाण (TEXTRANGE_PROPERTIES) == परिमाण (hb_ms_features_t)), "");
  static_assert ((परिमाण (OPENTYPE_FEATURE_RECORD) == परिमाण (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  तार्किक has_features = असत्य;
  यदि (अंक_features)
    has_features = hb_ms_setup_features (features,
					 अंक_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned पूर्णाङ्क glyphs_offset = 0;
  unsigned पूर्णाङ्क glyphs_len;
  तार्किक backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  प्रति (पूर्णाङ्क i = 0; i < item_मोजणी_2; i++)
  {
    unsigned पूर्णाङ्क chars_offset = वस्तुसंच[i].iCharPos;
    unsigned पूर्णाङ्क item_chars_len = वस्तुसंच[i + 1].iCharPos - chars_offset;

    यदि (has_features)
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
				     (पूर्णाङ्क *) range_char_counts.arrayZ,
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
				     (पूर्णाङ्क *) &glyphs_len);

    यदि (unlikely (वस्तुसंच[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    यदि (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      यदि (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      गच्छ retry;
    }
    यदि (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      यदि (वस्तुसंच[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      वस्तुसंच[i].a.eScript = SCRIPT_UNDEFINED;
      गच्छ retry_shape;
    }
    यदि (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    प्रति (unsigned पूर्णाङ्क j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &वस्तुसंच[i].a,
				     script_tags[i],
				     language_tag,
				     (पूर्णाङ्क *) range_char_counts.arrayZ,
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
    यदि (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    यदि (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
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
  प्रति (unsigned पूर्णाङ्क i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  प्रति (unsigned पूर्णाङ्क i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  प्रति (unsigned पूर्णाङ्क i = 1; i < glyphs_len; i++)
    यदि (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  यदि (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  प्रति (unsigned पूर्णाङ्क i = 0; i < glyphs_len; i++)
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
  प्रति (unsigned पूर्णाङ्क i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    द्विगुण x_offset = backward ? -(द्विगुण) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  यदि (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  प्रत्यागम सत्य;
}


#endif
