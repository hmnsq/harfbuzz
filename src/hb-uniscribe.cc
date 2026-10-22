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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_UNISCRIBE

#ifdef HB_NO_OT_TAG
#error "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#endif

#ଅନ୍ତର୍ଭାଵ "hb-shaper-impl.hh"

#ଅନ୍ତର୍ଭାଵ <windows.h>
#ଅନ୍ତର୍ଭାଵ <usp10.h>
#ଅନ୍ତର୍ଭାଵ <rpc.h>

#ifndef E_NOT_SUFFICIENT_BUFFER
#define E_NOT_SUFFICIENT_BUFFER HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_BUFFER)
#endif

#ଅନ୍ତର୍ଭାଵ "hb-uniscribe.h"

#ଅନ୍ତର୍ଭାଵ "hb-ms-feature-ranges.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-file.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-name-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

ପ୍ରକାରବ୍ୟାଖ୍ୟା HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ଅଚଳ WCHAR *pwcInChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cInChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_Max_ବସ୍ତୁଗୁଡ଼ିକ,
  ଅଚଳ SCRIPT_CONTROL *psControl,
  ଅଚଳ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ବସ୍ତୁଗୁଡ଼ିକ,
  OPENTYPE_TAG *pScriptTags,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pc_ବସ୍ତୁଗୁଡ଼ିକ
);

ପ୍ରକାରବ୍ୟାଖ୍ୟା HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cRanges,
  ଅଚଳ WCHAR *pwcChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pcGlyphs
);

ପ୍ରକାରବ୍ୟାଖ୍ୟା HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cRanges,
  ଅଚଳ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cChars,
  ଅଚଳ WORD *pwGlyphs,
  ଅଚଳ SCRIPT_GLYPHPROP *pGlyphProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cGlyphs,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ସ୍ଥିର HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ଅଚଳ WCHAR *pwcInChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cInChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c_Max_ବସ୍ତୁଗୁଡ଼ିକ,
  ଅଚଳ SCRIPT_CONTROL *psControl,
  ଅଚଳ SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_ବସ୍ତୁଗୁଡ଼ିକ,
  OPENTYPE_TAG *pScriptTags,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pc_ବସ୍ତୁଗୁଡ଼ିକ
)
{
{
  ଫେରନ୍ତୁ ScriptItemize (pwcInChars,
			cInChars,
			c_Max_ବସ୍ତୁଗୁଡ଼ିକ,
			psControl,
			psState,
			p_ବସ୍ତୁଗୁଡ଼ିକ,
			pc_ବସ୍ତୁଗୁଡ଼ିକ);
}
}

ସ୍ଥିର HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cRanges,
  ଅଚଳ WCHAR *pwcChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cChars,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pOutGlyphProps;
  ଫେରନ୍ତୁ ScriptShape (hdc,
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

ସ୍ଥିର HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cRanges,
  ଅଚଳ WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cChars,
  ଅଚଳ WORD *pwGlyphs,
  ଅଚଳ SCRIPT_GLYPHPROP *pGlyphProps,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cGlyphs,
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  ଫେରନ୍ତୁ ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


ସଂରଚନା hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  ଶୂନ୍ୟ init ()
  {
    HMODULE hinstLib;
    ଏହି->ScriptItemizeOpenType = nullptr;
    ଏହି->ScriptShapeOpenType   = nullptr;
    ଏହି->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    ଯଦି (hinstLib)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
      ଏହି->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      ଏହି->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      ଏହି->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#pragma GCC diagnostic କାଢ଼
    }
    ଯଦି (!ଏହି->ScriptItemizeOpenType ||
	!ଏହି->ScriptShapeOpenType   ||
	!ଏହି->ScriptPlaceOpenType)
    {
      DEBUG_ସନ୍ଦେଶ (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      ଏହି->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      ଏହି->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      ଏହି->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_uniscribe_shaper_funcs ();

ସ୍ଥିର ସଂରଚନା hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, ଆକାର (hb_uniscribe_shaper_funcs_t));
    ଯଦି (unlikely (!funcs))
      ଫେରନ୍ତୁ nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    ଫେରନ୍ତୁ funcs;
  }
  ସ୍ଥିର ଶୂନ୍ୟ destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((ଶୂନ୍ୟ *) p);
  }
  ସ୍ଥିର hb_uniscribe_shaper_funcs_t *get_null ()
  {
    ଫେରନ୍ତୁ nullptr;
  }
} static_uniscribe_shaper_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

ସ୍ଥିର hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  ଫେରନ୍ତୁ static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

ସଂରଚନା hb_uniscribe_face_data_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ସ୍ଥିର ଶୂନ୍ୟ
_hb_generate_unique_face_name (wchar_t *face_name, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ଅଚଳ ଅକ୍ଷର *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  unsigned ଅକ୍ଷର *p = (unsigned ଅକ୍ଷର *) &id;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 16; i += 2)
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
  ଯଦି (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ସ୍ଥିର hb_blob_t *
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

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length, new_length, name_str_len;
  ଅଚଳ ଅକ୍ଷର *orig_sfnt_data = hb_blob_get_data (blob, &length);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ସ୍ଥିର ଅଚଳ uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_table_length = OT::name::min_size +
				   ARRAY_LENGTH (name_IDs) * OT::NameRecord::static_size +
				   name_str_len * 2; /* for name data in UTF16BE form */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା padded_name_table_length = ((name_table_length + 3) & ~3);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_table_offset = (length + 3) & ~3;

  new_length = name_table_offset + padded_name_table_length;
  ଶୂନ୍ୟ *new_sfnt_data = hb_calloc (1, new_length);
  ଯଦି (!new_sfnt_data)
    ଫେରନ୍ତୁ nullptr;
  auto sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_data); });

  hb_memcpy(new_sfnt_data, orig_sfnt_data, length);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_data, name_table_offset);
  name.format = 0;
  name.ଗଣନା_2 = ARRAY_LENGTH (name_IDs);
  name.stringOffset = name.get_size ();
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_LENGTH (name_IDs); i++)
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
  unsigned ଅକ୍ଷର *p = &StructAfter<unsigned ଅକ୍ଷର> (name);
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ଅଚଳ OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_data);
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା face_ଗଣନା = file.get_face_ଗଣନା ();
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା face_index = 0; face_index < face_ଗଣନା; face_index++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ଅଚଳ OT::OpenTypeFontFace &face = file.get_face (face_index);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
    ଯଦି (face.find_table_index (HB_OT_TAG_name, &index))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (index));
      record.checkSum.set_for_data (&name, padded_name_table_length);
      record.offset = name_table_offset;
      record.length = name_table_length;
    }
    ଅନ୍ୟଥା ଯଦି (face_index == 0) /* Fail if first face doesn't have 'name' table. */
      ଫେରନ୍ତୁ nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  ଫେରନ୍ତୁ hb_blob_create ((ଅଚଳ ଅକ୍ଷର *) new_sfnt_data, new_length,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_data, hb_free);
}

hb_uniscribe_face_data_t *
_hb_uniscribe_shaper_face_data_create (hb_face_t *face)
{
  hb_uniscribe_face_data_t *data = (hb_uniscribe_face_data_t *) hb_calloc (1, ଆକାର (hb_uniscribe_face_data_t));
  ଯଦି (unlikely (!data))
    ଫେରନ୍ତୁ nullptr;

  data->funcs = hb_uniscribe_shaper_get_funcs ();
  ଯଦି (unlikely (!data->funcs))
  {
    hb_free (data);
    ଫେରନ୍ତୁ nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  ଯଦି (unlikely (!hb_blob_get_length (blob)))
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, data->face_name);
  ଯଦି (unlikely (!blob))
  {
    hb_free (data);
    ଫେରନ୍ତୁ nullptr;
  }

  DWORD ଅଙ୍କ_fonts_installed;
  data->fh = AddFontMemResourceEx ((ଶୂନ୍ୟ *) hb_blob_get_data (blob, nullptr),
				   hb_blob_get_length (blob),
				   0, &ଅଙ୍କ_fonts_installed);
  ଯଦି (unlikely (!data->fh))
  {
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (data);
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ data;
}

ଶୂନ୍ୟ
_hb_uniscribe_shaper_face_data_destroy (hb_uniscribe_face_data_t *data)
{
  RemoveFontMemResourceEx (data->fh);
  hb_free (data);
}


/*
 * shaper font data
 */

ସଂରଚନା hb_uniscribe_font_data_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  ଦ୍ୱିଗୁଣ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ସ୍ଥିର ତାର୍କିକ
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା font_size)
{
  hb_memset (lf, 0, ଆକାର (*lf));
  lf->lfHeight = - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) font_size;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->data.uniscribe->face_name, ଆକାର (lf->lfFaceName));

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

hb_uniscribe_font_data_t *
_hb_uniscribe_shaper_font_data_create (hb_font_t *font)
{
  hb_uniscribe_font_data_t *data = (hb_uniscribe_font_data_t *) hb_calloc (1, ଆକାର (hb_uniscribe_font_data_t));
  ଯଦି (unlikely (!data))
    ଫେରନ୍ତୁ nullptr;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା font_size = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  ଯଦି (font->y_ppem)
    font_size = font->y_ppem;

  ଯଦି (font_size < 0)
    font_size = -font_size;
  data->x_mult = (ଦ୍ୱିଗୁଣ) font->x_scale / font_size;
  data->y_mult = (ଦ୍ୱିଗୁଣ) font->y_scale / font_size;

  data->hdc = GetDC (nullptr);

  ଯଦି (unlikely (!populate_log_font (&data->log_font, font, font_size))) {
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, font, "Font populate_log_font() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
    ଫେରନ୍ତୁ nullptr;
  }

  data->hfont = CreateFontIndirectW (&data->log_font);
  ଯଦି (unlikely (!data->hfont)) {
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     ଫେରନ୍ତୁ nullptr;
  }

  ଯଦି (!SelectObject (data->hdc, data->hfont)) {
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, font, "Font SelectObject() failed");
    _hb_uniscribe_shaper_font_data_destroy (data);
     ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ data;
}

ଶୂନ୍ୟ
_hb_uniscribe_shaper_font_data_destroy (hb_uniscribe_font_data_t *data)
{
  ଯଦି (data->hdc)
    ReleaseDC (nullptr, data->hdc);
  ଯଦି (data->hfont)
    DeleteObject (data->hfont);
  ଯଦି (data->script_cache)
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
  ଅଚଳ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  ଫେରନ୍ତୁ data ? &data->log_font : nullptr;
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
  ଅଚଳ hb_uniscribe_font_data_t *data =  font->data.uniscribe;
  ଫେରନ୍ତୁ data ? data->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_buffer_t        *buffer,
		     ଅଚଳ hb_feature_t *features,
		     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା        ଅଙ୍କ_features)
{
  hb_face_t *face = font->face;
  ଅଚଳ hb_uniscribe_face_data_t *face_data = face->data.uniscribe;
  ଅଚଳ hb_uniscribe_font_data_t *font_data = font->data.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_data->funcs;

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_ସନ୍ଦେଶ (UNISCRIBE, nullptr, __VA_ARGS__); \
    ଫେରନ୍ତୁ ମିଥ୍ୟା; \
  } HB_STMT_END

  HRESULT hr;

retry:

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  କର { \
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା _consumed = DIV_CEIL ((len) * ଆକାର (Type), ଆକାର (*scratch)); \
    ପ୍ରତିଜ୍ଞା (_consumed <= scratch_size); \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ଯେତେବେଳେ (0)

#define utf16_index() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, buffer->len * 2);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_len = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    buffer->info[i].utf16_index() = chars_len;
    ଯଦି (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ଅନ୍ୟଥା ଯଦି (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ଅନ୍ୟଥା {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  ଯଦି (ଅଙ୍କ_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++)
    {
      hb_codepoint_t c = buffer->info[i].codepoint;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cluster = buffer->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      ଯଦି (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyphs_size = (scratch_size * ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) - 2)
			   / (ଆକାର (WORD) +
			      ଆକାର (SCRIPT_GLYPHPROP) +
			      ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) +
			      ଆକାର (GOFFSET) +
			      ଆକାର (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_size);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_size);
  ALLOCATE_ARRAY (ପୂର୍ଣ୍ଣସଂଖ୍ୟା, advances, glyphs_size);
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

#define MAX_ବସ୍ତୁଗୁଡ଼ିକ 256

  SCRIPT_ITEM ବସ୍ତୁଗୁଡ଼ିକ[MAX_ବସ୍ତୁଗୁଡ଼ିକ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_ବସ୍ତୁଗୁଡ଼ିକ];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା item_ଗଣନା_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_ବସ୍ତୁଗୁଡ଼ିକ,
				     &bidi_control,
				     &bidi_state,
				     ବସ୍ତୁଗୁଡ଼ିକ,
				     script_tags,
				     &item_ଗଣନା_2);
  ଯଦି (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (unsigned long) hr);

#undef MAX_ବସ୍ତୁଗୁଡ଼ିକ

  hb_tag_t lang_tag;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lang_ଗଣନା = 1;
  hb_ot_tags_from_script_and_language (buffer->props.script,
				       buffer->props.language,
				       nullptr, nullptr,
				       &lang_ଗଣନା, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_ଗଣନା ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((ଆକାର (TEXTRANGE_PROPERTIES) == ଆକାର (hb_ms_features_t)), "");
  static_assert ((ଆକାର (OPENTYPE_FEATURE_RECORD) == ଆକାର (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  ତାର୍କିକ has_features = ମିଥ୍ୟା;
  ଯଦି (ଅଙ୍କ_features)
    has_features = hb_ms_setup_features (features,
					 ଅଙ୍କ_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyphs_offset = 0;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyphs_len;
  ତାର୍କିକ backward = HB_DIRECTION_IS_BACKWARD (buffer->props.direction);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < item_ଗଣନା_2; i++)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା chars_offset = ବସ୍ତୁଗୁଡ଼ିକ[i].iCharPos;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା item_chars_len = ବସ୍ତୁଗୁଡ଼ିକ[i + 1].iCharPos - chars_offset;

    ଯଦି (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    ବସ୍ତୁଗୁଡ଼ିକ[i].a.fLogicalOrder = ସତ୍ୟ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ବସ୍ତୁଗୁଡ଼ିକ[i].a,
				     script_tags[i],
				     language_tag,
				     (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) range_char_counts.arrayZ,
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
				     (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) &glyphs_len);

    ଯଦି (unlikely (ବସ୍ତୁଗୁଡ଼ିକ[i].a.fNoGlyphIndex))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    ଯଦି (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_BUFFER))
    {
      ଯଦି (unlikely (!buffer->ensure (buffer->allocated * 2)))
	FAIL ("Buffer resize failed");
      ଯାଅ retry;
    }
    ଯଦି (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      ଯଦି (ବସ୍ତୁଗୁଡ଼ିକ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      ବସ୍ତୁଗୁଡ଼ିକ[i].a.eScript = SCRIPT_UNDEFINED;
      ଯାଅ retry_shape;
    }
    ଯଦି (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (unsigned long) hr);
    }

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_data->hdc,
				     &font_data->script_cache,
				     &ବସ୍ତୁଗୁଡ଼ିକ[i].a,
				     script_tags[i],
				     language_tag,
				     (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) range_char_counts.arrayZ,
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
    ଯଦି (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (unsigned long) hr);

    ଯଦି (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       ବସ୍ତୁଗୁଡ଼ିକ[i].a.fRTL,
	       ବସ୍ତୁଗୁଡ଼ିକ[i].a.fLayoutRTL,
	       ବସ୍ତୁଗୁଡ଼ିକ[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < buffer->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[buffer->info[i].utf16_index()]];
    *p = hb_min (*p, buffer->info[i].cluster);
  }
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < glyphs_len; i++)
    ଯଦି (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#undef utf16_index

  ଯଦି (unlikely (!buffer->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#undef FAIL

  /* Set glyph infos */
  buffer->len = 0;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyphs_len; i++)
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
  ଦ୍ୱିଗୁଣ x_mult = font_data->x_mult, y_mult = font_data->y_mult;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &buffer->info[i];
    hb_glyph_position_t *pos = &buffer->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    ଦ୍ୱିଗୁଣ x_offset = backward ? -(ଦ୍ୱିଗୁଣ) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  ଯଦି (backward)
    hb_buffer_reverse (buffer);

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

  /* Wow, done! */
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


#endif
