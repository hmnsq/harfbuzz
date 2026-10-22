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

#समावेश "hb.hh"

#अगर_घोषित HAVE_UNISCRIBE

#अगर_घोषित HB_NO_OT_TAG
#त्रुटि "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#पूर्ण_अगर

#समावेश "hb-shaper-impl.hh"

#समावेश <windows.h>
#समावेश <usp10.h>
#समावेश <rpc.h>

#अगर_अघोषित E_NOT_SUFFICIENT_बफर
#घोषणा E_NOT_SUFFICIENT_बफर HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_बफर)
#पूर्ण_अगर

#समावेश "hb-uniscribe.h"

#समावेश "hb-ms-feature-ranges.hh"
#समावेश "hb-open-file.hh"
#समावेश "hb-ot-name-table.hh"
#समावेश "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

प्रकार HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  स्थिर WCHAR *pwcInChars,
  पूर्णांक cInChars,
  पूर्णांक c_Max_वस्तुएँ,
  स्थिर SCRIPT_CONTROL *psControl,
  स्थिर SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुएँ,
  OPENTYPE_TAG *pScriptTags,
  पूर्णांक *pc_वस्तुएँ
);

प्रकार HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिर WCHAR *pwcChars,
  पूर्णांक cChars,
  पूर्णांक cMaxGlyphs,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  WORD *pwOutGlyphs,
  SCRIPT_GLYPHPROP *pOutGlyphProps,
  पूर्णांक *pcGlyphs
);

प्रकार HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिर WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णांक cChars,
  स्थिर WORD *pwGlyphs,
  स्थिर SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णांक cGlyphs,
  पूर्णांक *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

अटल HRESULT WINAPI
hb_ScriptItemizeOpenType(
  स्थिर WCHAR *pwcInChars,
  पूर्णांक cInChars,
  पूर्णांक c_Max_वस्तुएँ,
  स्थिर SCRIPT_CONTROL *psControl,
  स्थिर SCRIPT_STATE *psState,
  SCRIPT_ITEM *p_वस्तुएँ,
  OPENTYPE_TAG *pScriptTags,
  पूर्णांक *pc_वस्तुएँ
)
{
{
  वापस ScriptItemize (pwcInChars,
			cInChars,
			c_Max_वस्तुएँ,
			psControl,
			psState,
			p_वस्तुएँ,
			pc_वस्तुएँ);
}
}

अटल HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिर WCHAR *pwcChars,
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
  वापस ScriptShape (hdc,
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

अटल HRESULT WINAPI
hb_ScriptPlaceOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_TAG tagScript,
  OPENTYPE_TAG tagLangSys,
  पूर्णांक *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  पूर्णांक cRanges,
  स्थिर WCHAR *pwcChars,
  WORD *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  पूर्णांक cChars,
  स्थिर WORD *pwGlyphs,
  स्थिर SCRIPT_GLYPHPROP *pGlyphProps,
  पूर्णांक cGlyphs,
  पूर्णांक *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  वापस ScriptPlace (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


काष्ठा hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT ScriptPlaceOpenType;

  व्योम init ()
  {
    HMODULE hinstLib;
    यह->ScriptItemizeOpenType = nullptr;
    यह->ScriptShapeOpenType   = nullptr;
    यह->ScriptPlaceOpenType   = nullptr;

    hinstLib = GetModuleHandle (TEXT ("usp10.dll"));
    अगर (hinstLib)
    {
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-function-type"
      यह->ScriptItemizeOpenType = (SIOT) GetProcAddress (hinstLib, "ScriptItemizeOpenType");
      यह->ScriptShapeOpenType   = (SSOT) GetProcAddress (hinstLib, "ScriptShapeOpenType");
      यह->ScriptPlaceOpenType   = (SPOT) GetProcAddress (hinstLib, "ScriptPlaceOpenType");
#आशय GCC diagnostic निकालो
    }
    अगर (!यह->ScriptItemizeOpenType ||
	!यह->ScriptShapeOpenType   ||
	!यह->ScriptPlaceOpenType)
    {
      DEBUG_संदेश (UNISCRIBE, nullptr, "OpenType versions of functions not found; falling back.");
      यह->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      यह->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      यह->ScriptPlaceOpenType   = hb_ScriptPlaceOpenType;
    }
  }
};

अटल अंतरभूत व्योम free_static_uniscribe_shaper_funcs ();

अटल काष्ठा hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  अटल hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, माप (hb_uniscribe_shaper_funcs_t));
    अगर (unlikely (!funcs))
      वापस nullptr;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    वापस funcs;
  }
  अटल व्योम destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((व्योम *) p);
  }
  अटल hb_uniscribe_shaper_funcs_t *get_null ()
  {
    वापस nullptr;
  }
} static_uniscribe_shaper_funcs;

अटल अंतरभूत
व्योम free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

अटल hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  वापस static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

काष्ठा hb_uniscribe_face_तथ्य_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  ब_अक्षर_प्रकार face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
अटल व्योम
_hb_generate_unique_face_name (ब_अक्षर_प्रकार *face_name, अचिन्हित पूर्णांक *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  स्थिर अक्षर *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  अचिन्हित पूर्णांक name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  अचिन्हित अक्षर *p = (अचिन्हित अक्षर *) &id;
  क्रम (अचिन्हित पूर्णांक i = 0; i < 16; i += 2)
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
  अगर (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
अटल hb_blob_t *
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

  अचिन्हित पूर्णांक लंबाई, new_लंबाई, name_str_len;
  स्थिर अक्षर *orig_sfnt_तथ्य = hb_blob_get_तथ्य (blob, &लंबाई);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  अटल स्थिर uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  अचिन्हित पूर्णांक name_table_लंबाई = OT::name::min_आकार +
				   ARRAY_लंबाई (name_IDs) * OT::NameRecord::static_आकार +
				   name_str_len * 2; /* for name data in UTF16BE form */
  अचिन्हित पूर्णांक padded_name_table_लंबाई = ((name_table_लंबाई + 3) & ~3);
  अचिन्हित पूर्णांक name_table_offset = (लंबाई + 3) & ~3;

  new_लंबाई = name_table_offset + padded_name_table_लंबाई;
  व्योम *new_sfnt_तथ्य = hb_calloc (1, new_लंबाई);
  अगर (!new_sfnt_तथ्य)
    वापस nullptr;
  स्वतः sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_तथ्य); });

  hb_memcpy(new_sfnt_तथ्य, orig_sfnt_तथ्य, लंबाई);

  OT::name &name = StructAtOffset<OT::name> (new_sfnt_तथ्य, name_table_offset);
  name.format = 0;
  name.गिनती = ARRAY_लंबाई (name_IDs);
  name.stringOffset = name.get_आकार ();
  क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (name_IDs); i++)
  {
    OT::NameRecord &record = name.nameRecordZ[i];
    record.platformID = 3;
    record.encodingID = 1;
    record.languageID = 0x0409u; /* English */
    record.nameID = name_IDs[i];
    record.लंबाई = name_str_len * 2;
    record.offset = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  अचिन्हित अक्षर *p = &StructAfter<अचिन्हित अक्षर> (name);
  क्रम (अचिन्हित पूर्णांक i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  स्थिर OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_तथ्य);
  अचिन्हित पूर्णांक face_गिनती = file.get_face_गिनती ();
  क्रम (अचिन्हित पूर्णांक face_सूचकांक = 0; face_सूचकांक < face_गिनती; face_सूचकांक++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    स्थिर OT::OpenTypeFontFace &face = file.get_face (face_सूचकांक);
    अचिन्हित पूर्णांक सूचकांक_2;
    अगर (face.find_table_सूचकांक (HB_OT_TAG_name, &सूचकांक_2))
    {
      OT::TableRecord &record = const_cast<OT::TableRecord &> (face.get_table (सूचकांक_2));
      record.check_योग.set_for_तथ्य (&name, padded_name_table_लंबाई);
      record.offset = name_table_offset;
      record.लंबाई = name_table_लंबाई;
    }
    अन्यथा अगर (face_सूचकांक == 0) /* Fail if first face doesn't have 'name' table. */
      वापस nullptr;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  वापस hb_blob_create ((स्थिर अक्षर *) new_sfnt_तथ्य, new_लंबाई,
			 HB_MEMORY_MODE_WRITABLE, new_sfnt_तथ्य, hb_free);
}

hb_uniscribe_face_तथ्य_t *
hb_uniscribe_shaper_face_तथ्य_create (hb_face_t *face)
{
  hb_uniscribe_face_तथ्य_t *तथ्य = (hb_uniscribe_face_तथ्य_t *) hb_calloc (1, माप (hb_uniscribe_face_तथ्य_t));
  अगर (unlikely (!तथ्य))
    वापस nullptr;

  तथ्य->funcs = hb_uniscribe_shaper_get_funcs ();
  अगर (unlikely (!तथ्य->funcs))
  {
    hb_free (तथ्य);
    वापस nullptr;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  अगर (unlikely (!hb_blob_get_लंबाई (blob)))
    DEBUG_संदेश (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, तथ्य->face_name);
  अगर (unlikely (!blob))
  {
    hb_free (तथ्य);
    वापस nullptr;
  }

  DWORD अंक_fonts_installed;
  तथ्य->fh = AddFontMemResourceEx ((व्योम *) hb_blob_get_तथ्य (blob, nullptr),
				   hb_blob_get_लंबाई (blob),
				   0, &अंक_fonts_installed);
  अगर (unlikely (!तथ्य->fh))
  {
    DEBUG_संदेश (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (तथ्य);
    वापस nullptr;
  }

  वापस तथ्य;
}

व्योम
hb_uniscribe_shaper_face_तथ्य_destroy (hb_uniscribe_face_तथ्य_t *तथ्य)
{
  RemoveFontMemResourceEx (तथ्य->fh);
  hb_free (तथ्य);
}


/*
 * shaper font data
 */

काष्ठा hb_uniscribe_font_तथ्य_t
{
  HDC hdc;
  mutable LOGFONTW log_font;
  HFONT hfont;
  mutable SCRIPT_CACHE script_cache;
  द्विगुन x_mult, y_mult; /* From LOGFONT space to HB space. */
};

अटल bool
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   अचिन्हित पूर्णांक font_आकार)
{
  hb_memset (lf, 0, माप (*lf));
  lf->lfHeight = - (पूर्णांक) font_आकार;
  lf->lfCharSet = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->तथ्य.uniscribe->face_name, माप (lf->lfFaceName));

  वापस true;
}

hb_uniscribe_font_तथ्य_t *
hb_uniscribe_shaper_font_तथ्य_create (hb_font_t *font)
{
  hb_uniscribe_font_तथ्य_t *तथ्य = (hb_uniscribe_font_तथ्य_t *) hb_calloc (1, माप (hb_uniscribe_font_तथ्य_t));
  अगर (unlikely (!तथ्य))
    वापस nullptr;

  पूर्णांक font_आकार = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  अगर (font->y_ppem)
    font_आकार = font->y_ppem;

  अगर (font_आकार < 0)
    font_आकार = -font_आकार;
  तथ्य->x_mult = (द्विगुन) font->x_scale / font_आकार;
  तथ्य->y_mult = (द्विगुन) font->y_scale / font_आकार;

  तथ्य->hdc = GetDC (nullptr);

  अगर (unlikely (!populate_log_font (&तथ्य->log_font, font, font_आकार))) {
    DEBUG_संदेश (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_तथ्य_destroy (तथ्य);
    वापस nullptr;
  }

  तथ्य->hfont = CreateFontIndirectW (&तथ्य->log_font);
  अगर (unlikely (!तथ्य->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_तथ्य_destroy (तथ्य);
     वापस nullptr;
  }

  अगर (!SelectObject (तथ्य->hdc, तथ्य->hfont)) {
    DEBUG_संदेश (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_तथ्य_destroy (तथ्य);
     वापस nullptr;
  }

  वापस तथ्य;
}

व्योम
hb_uniscribe_shaper_font_तथ्य_destroy (hb_uniscribe_font_तथ्य_t *तथ्य)
{
  अगर (तथ्य->hdc)
    ReleaseDC (nullptr, तथ्य->hdc);
  अगर (तथ्य->hfont)
    DeleteObject (तथ्य->hfont);
  अगर (तथ्य->script_cache)
    ScriptFreeCache (&तथ्य->script_cache);
  hb_free (तथ्य);
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
  स्थिर hb_uniscribe_font_तथ्य_t *तथ्य =  font->तथ्य.uniscribe;
  वापस तथ्य ? &तथ्य->log_font : nullptr;
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
  स्थिर hb_uniscribe_font_तथ्य_t *तथ्य =  font->तथ्य.uniscribe;
  वापस तथ्य ? तथ्य->hfont : nullptr;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_बफर_t        *बफर,
		     स्थिर hb_feature_t *features,
		     अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  स्थिर hb_uniscribe_face_तथ्य_t *face_तथ्य = face->तथ्य.uniscribe;
  स्थिर hb_uniscribe_font_तथ्य_t *font_तथ्य = font->तथ्य.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_तथ्य->funcs;

#घोषणा FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (UNISCRIBE, nullptr, __VA_ARGS__); \
    वापस false; \
  } HB_STMT_END

  HRESULT hr;

retry:

  अचिन्हित पूर्णांक scratch_आकार;
  hb_बफर_t::scratch_बफर_t *scratch = बफर->get_scratch_बफर (&scratch_आकार);

#घोषणा ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  करो { \
    अचिन्हित पूर्णांक _consumed = DIV_CEIL ((len) * माप (Type), माप (*scratch)); \
    निश्चित (_consumed <= scratch_आकार); \
    scratch += _consumed; \
    scratch_आकार -= _consumed; \
  } जबतक (0)

#घोषणा utf16_सूचकांक() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, बफर->len * 2);

  अचिन्हित पूर्णांक chars_len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
  {
    hb_codepoint_t c = बफर->info[i].codepoint;
    बफर->info[i].utf16_सूचकांक() = chars_len;
    अगर (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    अन्यथा अगर (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    अन्यथा {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (WORD, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  अगर (अंक_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
    {
      hb_codepoint_t c = बफर->info[i].codepoint;
      अचिन्हित पूर्णांक cluster = बफर->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      अगर (hb_in_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  अचिन्हित पूर्णांक glyphs_आकार = (scratch_आकार * माप (पूर्णांक) - 2)
			   / (माप (WORD) +
			      माप (SCRIPT_GLYPHPROP) +
			      माप (पूर्णांक) +
			      माप (GOFFSET) +
			      माप (uint32_t));

  ALLOCATE_ARRAY (WORD, glyphs, glyphs_आकार);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_आकार);
  ALLOCATE_ARRAY (पूर्णांक, advances, glyphs_आकार);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_आकार);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_आकार);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#अघोषित ALLOCATE_ARRAY

#घोषणा MAX_वस्तुएँ 256

  SCRIPT_ITEM वस्तुएँ[MAX_वस्तुएँ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_tags[MAX_वस्तुएँ];
  पूर्णांक item_गिनती_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (बफर->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_वस्तुएँ,
				     &bidi_control,
				     &bidi_state,
				     वस्तुएँ,
				     script_tags,
				     &item_गिनती_2);
  अगर (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);

#अघोषित MAX_वस्तुएँ

  hb_tag_t lang_tag;
  अचिन्हित पूर्णांक lang_गिनती = 1;
  hb_ot_tags_from_script_and_language (बफर->props.script,
				       बफर->props.language,
				       nullptr, nullptr,
				       &lang_गिनती, &lang_tag);
  OPENTYPE_TAG language_tag = hb_uint32_swap (lang_गिनती ? lang_tag : HB_TAG_NONE);

  /*
   * Set up features.
   */
  static_assert ((माप (TEXTRANGE_PROPERTIES) == माप (hb_ms_features_t)), "");
  static_assert ((माप (OPENTYPE_FEATURE_RECORD) == माप (hb_ms_feature_t)), "");
  hb_vector_t<hb_ms_feature_t> feature_records;
  hb_vector_t<hb_ms_range_record_t> range_records;
  bool has_features = false;
  अगर (अंक_features)
    has_features = hb_ms_setup_features (features,
					 अंक_features,
					 feature_records,
					 range_records);

  hb_vector_t<hb_ms_features_t*> range_properties;
  hb_vector_t<uint32_t> range_char_counts;

  अचिन्हित पूर्णांक glyphs_offset = 0;
  अचिन्हित पूर्णांक glyphs_len;
  bool backward = HB_DIRECTION_IS_BACKWARD (बफर->props.direction);
  क्रम (पूर्णांक i = 0; i < item_गिनती_2; i++)
  {
    अचिन्हित पूर्णांक chars_offset = वस्तुएँ[i].iCharPos;
    अचिन्हित पूर्णांक item_chars_len = वस्तुएँ[i + 1].iCharPos - chars_offset;

    अगर (has_features)
      hb_ms_make_feature_ranges (feature_records,
				 range_records,
				 item_chars_len,
				 chars_offset,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    वस्तुएँ[i].a.fLogicalOrder = true;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_तथ्य->hdc,
				     &font_तथ्य->script_cache,
				     &वस्तुएँ[i].a,
				     script_tags[i],
				     language_tag,
				     (पूर्णांक *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.लंबाई,
				     pchars + chars_offset,
				     item_chars_len,
				     glyphs_आकार - glyphs_offset,
				     /* out */
				     log_clusters + chars_offset,
				     char_props + chars_offset,
				     glyphs + glyphs_offset,
				     glyph_props + glyphs_offset,
				     (पूर्णांक *) &glyphs_len);

    अगर (unlikely (वस्तुएँ[i].a.f_No_Glyph_सूचकांक))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    अगर (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_बफर))
    {
      अगर (unlikely (!बफर->ensure (बफर->allocated * 2)))
	FAIL ("Buffer resize failed");
      जाओ retry;
    }
    अगर (unlikely (hr == USP_E_SCRIPT_NOT_IN_FONT))
    {
      अगर (वस्तुएँ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      वस्तुएँ[i].a.eScript = SCRIPT_UNDEFINED;
      जाओ retry_shape;
    }
    अगर (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);
    }

    क्रम (अचिन्हित पूर्णांक j = chars_offset; j < chars_offset + item_chars_len; j++)
      log_clusters[j] += glyphs_offset;

    hr = funcs->ScriptPlaceOpenType (font_तथ्य->hdc,
				     &font_तथ्य->script_cache,
				     &वस्तुएँ[i].a,
				     script_tags[i],
				     language_tag,
				     (पूर्णांक *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.लंबाई,
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
    अगर (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (अचिन्हित दीर्घ) hr);

    अगर (DEBUG_ENABLED (UNISCRIBE))
      ख_लिखो (मानक_त्रुटि, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       वस्तुएँ[i].a.fRTL,
	       वस्तुएँ[i].a.fLayoutRTL,
	       वस्तुएँ[i].a.fLogicalOrder,
	       HB_UNTAG (hb_uint32_swap (script_tags[i])));

    glyphs_offset += glyphs_len;
  }
  glyphs_len = glyphs_offset;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  क्रम (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[बफर->info[i].utf16_सूचकांक()]];
    *p = hb_min (*p, बफर->info[i].cluster);
  }
  क्रम (अचिन्हित पूर्णांक i = 1; i < glyphs_len; i++)
    अगर (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#अघोषित utf16_सूचकांक

  अगर (unlikely (!बफर->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#अघोषित FAIL

  /* Set glyph infos */
  बफर->len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &बफर->info[बफर->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->mask = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  बफर->clear_positions ();
  द्विगुन x_mult = font_तथ्य->x_mult, y_mult = font_तथ्य->y_mult;
  क्रम (अचिन्हित पूर्णांक i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &बफर->info[i];
    hb_glyph_position_t *pos = &बफर->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_to<hb_position_t> (x_mult * (int32_t) info->mask);
    द्विगुन x_offset = backward ? -(द्विगुन) info->var1.i32 : info->var1.i32;
    pos->x_offset = hb_clamp_to<hb_position_t> (x_mult * x_offset);
    pos->y_offset = hb_clamp_to<hb_position_t> (y_mult * info->var2.i32);
  }

  अगर (backward)
    hb_बफर_reverse (बफर);

  बफर->clear_glyph_flags ();
  बफर->unsafe_to_break ();

  /* Wow, done! */
  वापस true;
}


#पूर्ण_अगर
