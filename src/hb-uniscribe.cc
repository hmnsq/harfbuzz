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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_UNISCRIBE

#నిర్వచితమైతే HB_NO_OT_ట్యాగ్
#దోషం "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#అయితే_ముగింపు

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ <windows.h>
#అన్తర్భావ <usp10.h>
#అన్తర్భావ <rpc.h>

#నిర్వచితం_కాకపోతే E_NOT_SUFFICIENT_బఫరు
#నిర్వచించు E_NOT_SUFFICIENT_బఫరు HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_బఫరు)
#అయితే_ముగింపు

#అన్తర్భావ "hb-uniscribe.h"

#అన్తర్భావ "hb-ms-feature-ranges.hh"
#అన్తర్భావ "hb-open-file.hh"
#అన్తర్భావ "hb-ot-name-table.hh"
#అన్తర్భావ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

రకనిర్వచనం HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  స్థిరరాశి WCHAR *pwc_లోపల_Chars,
  పూర్ణాంకం c_లోపల_Chars,
  పూర్ణాంకం c_Max_వస్తువులు,
  స్థిరరాశి SCRIPT_CONTROL *psControl,
  స్థిరరాశి SCRIPT_STATE *psState,
  SCRIPT_అంశం *p_వస్తువులు,
  OPENTYPE_ట్యాగ్ *p_Script_ట్యాగులు,
  పూర్ణాంకం *pc_వస్తువులు
);

రకనిర్వచనం HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Script,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Lang_Sys,
  పూర్ణాంకం *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  పూర్ణాంకం cRanges,
  స్థిరరాశి WCHAR *pwcChars,
  పూర్ణాంకం cChars,
  పూర్ణాంకం cMaxGlyphs,
  మాట *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  మాట *pw_బహిర్గతం_Glyphs,
  SCRIPT_GLYPHPROP *p_బహిర్గతం_Glyph_Props,
  పూర్ణాంకం *pcGlyphs
);

రకనిర్వచనం HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Script,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Lang_Sys,
  పూర్ణాంకం *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  పూర్ణాంకం cRanges,
  స్థిరరాశి WCHAR *pwcChars,
  మాట *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  పూర్ణాంకం cChars,
  స్థిరరాశి మాట *pwGlyphs,
  స్థిరరాశి SCRIPT_GLYPHPROP *pGlyphProps,
  పూర్ణాంకం cGlyphs,
  పూర్ణాంకం *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

స్థిర HRESULT WINAPI
hb_ScriptItemizeOpenType(
  స్థిరరాశి WCHAR *pwc_లోపల_Chars,
  పూర్ణాంకం c_లోపల_Chars,
  పూర్ణాంకం c_Max_వస్తువులు,
  స్థిరరాశి SCRIPT_CONTROL *psControl,
  స్థిరరాశి SCRIPT_STATE *psState,
  SCRIPT_అంశం *p_వస్తువులు,
  OPENTYPE_ట్యాగ్ *p_Script_ట్యాగులు,
  పూర్ణాంకం *pc_వస్తువులు
)
{
{
  తిరిగి ScriptItemize (pwc_లోపల_Chars,
			c_లోపల_Chars,
			c_Max_వస్తువులు,
			psControl,
			psState,
			p_వస్తువులు,
			pc_వస్తువులు);
}
}

స్థిర HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Script,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Lang_Sys,
  పూర్ణాంకం *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  పూర్ణాంకం cRanges,
  స్థిరరాశి WCHAR *pwcChars,
  పూర్ణాంకం cChars,
  పూర్ణాంకం cMaxGlyphs,
  మాట *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  మాట *pw_బహిర్గతం_Glyphs,
  SCRIPT_GLYPHPROP *p_బహిర్గతం_Glyph_Props,
  పూర్ణాంకం *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_బహిర్గతం_Glyph_Props;
  తిరిగి ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_బహిర్గతం_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

స్థిర HRESULT WINAPI
hb_Script_ఉంచు_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Script,
  OPENTYPE_ట్యాగ్ ట్యాగ్_Lang_Sys,
  పూర్ణాంకం *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  పూర్ణాంకం cRanges,
  స్థిరరాశి WCHAR *pwcChars,
  మాట *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  పూర్ణాంకం cChars,
  స్థిరరాశి మాట *pwGlyphs,
  స్థిరరాశి SCRIPT_GLYPHPROP *pGlyphProps,
  పూర్ణాంకం cGlyphs,
  పూర్ణాంకం *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  తిరిగి Script_ఉంచు (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


నిర్మాణం hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT Script_ఉంచు_Open_Type;

  శూన్య init ()
  {
    HMODULE hinstLib;
    ఇది->ScriptItemizeOpenType = శూన్యసూచిక;
    ఇది->ScriptShapeOpenType   = శూన్యసూచిక;
    ఇది->Script_ఉంచు_Open_Type   = శూన్యసూచిక;

    hinstLib = GetModuleHandle (పాఠ్యం_2 ("usp10.dll"));
    ఒకవేళ (hinstLib)
    {
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-function-type"
      ఇది->ScriptItemizeOpenType = (SIOT) Get_Proc_చిరునామా (hinstLib, "ScriptItemizeOpenType");
      ఇది->ScriptShapeOpenType   = (SSOT) Get_Proc_చిరునామా (hinstLib, "ScriptShapeOpenType");
      ఇది->Script_ఉంచు_Open_Type   = (SPOT) Get_Proc_చిరునామా (hinstLib, "ScriptPlaceOpenType");
#సూచన GCC diagnostic తీయి
    }
    ఒకవేళ (!ఇది->ScriptItemizeOpenType ||
	!ఇది->ScriptShapeOpenType   ||
	!ఇది->Script_ఉంచు_Open_Type)
    {
      DEBUG_సందేశం (UNISCRIBE, శూన్యసూచిక, "OpenType versions of functions not found; falling back.");
      ఇది->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      ఇది->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      ఇది->Script_ఉంచు_Open_Type   = hb_Script_ఉంచు_Open_Type;
    }
  }
};

స్థిర అంతఃవరుస శూన్య free_static_uniscribe_shaper_funcs ();

స్థిర నిర్మాణం hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  స్థిర hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, పరిమాణం (hb_uniscribe_shaper_funcs_t));
    ఒకవేళ (unlikely (!funcs))
      తిరిగి శూన్యసూచిక;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    తిరిగి funcs;
  }
  స్థిర శూన్య destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((శూన్య *) p);
  }
  స్థిర hb_uniscribe_shaper_funcs_t *get_null ()
  {
    తిరిగి శూన్యసూచిక;
  }
} static_uniscribe_shaper_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

స్థిర hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  తిరిగి static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

నిర్మాణం hb_uniscribe_face_దత్తాంశం_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
స్థిర శూన్య
_hb_generate_unique_face_name (wchar_t *face_name, గుర్తులేని పూర్ణాంకం *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  స్థిరరాశి అక్షరం *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  గుర్తులేని పూర్ణాంకం name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  గుర్తులేని అక్షరం *p = (గుర్తులేని అక్షరం *) &id;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < 16; i += 2)
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
  ఒకవేళ (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
స్థిర hb_blob_t *
_hb_rename_font (hb_blob_t *blob_లోపల, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_లోపల));

  గుర్తులేని పూర్ణాంకం నిడివి, new_నిడివి, name_str_len;
  స్థిరరాశి అక్షరం *orig_sfnt_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &నిడివి);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  స్థిర స్థిరరాశి uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  గుర్తులేని పూర్ణాంకం name_పట్టిక_నిడివి = OT::name::min_కొలత +
				   ARRAY_నిడివి (name_IDs) * OT::Name_రికార్డు::static_కొలత +
				   name_str_len * 2; /* for name data in UTF16BE form */
  గుర్తులేని పూర్ణాంకం padded_name_పట్టిక_నిడివి = ((name_పట్టిక_నిడివి + 3) & ~3);
  గుర్తులేని పూర్ణాంకం name_పట్టిక_స్థానభ్రంశం = (నిడివి + 3) & ~3;

  new_నిడివి = name_పట్టిక_స్థానభ్రంశం + padded_name_పట్టిక_నిడివి;
  శూన్య *new_sfnt_దత్తాంశం = hb_calloc (1, new_నిడివి);
  ఒకవేళ (!new_sfnt_దత్తాంశం)
    తిరిగి శూన్యసూచిక;
  స్వయంచాలక sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_దత్తాంశం); });

  hb_memcpy(new_sfnt_దత్తాంశం, orig_sfnt_దత్తాంశం, నిడివి);

  OT::name &name = Struct_At_స్థానభ్రంశం<OT::name> (new_sfnt_దత్తాంశం, name_పట్టిక_స్థానభ్రంశం);
  name.format = 0;
  name.లెక్క = ARRAY_నిడివి (name_IDs);
  name.string_స్థానభ్రంశం = name.get_కొలత ();
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (name_IDs); i++)
  {
    OT::Name_రికార్డు &రికార్డు = name.name_రికార్డు_Z[i];
    రికార్డు.platformID = 3;
    రికార్డు.encodingID = 1;
    రికార్డు.languageID = 0x0409u; /* English */
    రికార్డు.nameID = name_IDs[i];
    రికార్డు.నిడివి = name_str_len * 2;
    రికార్డు.స్థానభ్రంశం = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  గుర్తులేని అక్షరం *p = &StructAfter<గుర్తులేని అక్షరం> (name);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  స్థిరరాశి OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_దత్తాంశం);
  గుర్తులేని పూర్ణాంకం face_లెక్క = file.get_face_లెక్క ();
  కొరకు (గుర్తులేని పూర్ణాంకం face_సూచీ = 0; face_సూచీ < face_లెక్క; face_సూచీ++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    స్థిరరాశి OT::OpenTypeFontFace &face = file.get_face (face_సూచీ);
    గుర్తులేని పూర్ణాంకం సూచీ_2;
    ఒకవేళ (face.కనుగొను_పట్టిక_సూచీ (HB_OT_ట్యాగ్_name, &సూచీ_2))
    {
      OT::పట్టిక_రికార్డు &రికార్డు = const_cast<OT::పట్టిక_రికార్డు &> (face.get_పట్టిక (సూచీ_2));
      రికార్డు.check_కూడిక.సమితి_for_దత్తాంశం (&name, padded_name_పట్టిక_నిడివి);
      రికార్డు.స్థానభ్రంశం = name_పట్టిక_స్థానభ్రంశం;
      రికార్డు.నిడివి = name_పట్టిక_నిడివి;
    }
    లేకపోతే ఒకవేళ (face_సూచీ == 0) /* Fail if first face doesn't have 'name' table. */
      తిరిగి శూన్యసూచిక;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) new_sfnt_దత్తాంశం, new_నిడివి,
			 HB_జ్ఞాపకం_బహుళకం_WRITABLE, new_sfnt_దత్తాంశం, hb_free);
}

hb_uniscribe_face_దత్తాంశం_t *
hb_uniscribe_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  hb_uniscribe_face_దత్తాంశం_t *దత్తాంశం = (hb_uniscribe_face_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (hb_uniscribe_face_దత్తాంశం_t));
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  దత్తాంశం->funcs = hb_uniscribe_shaper_get_funcs ();
  ఒకవేళ (unlikely (!దత్తాంశం->funcs))
  {
    hb_free (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  ఒకవేళ (unlikely (!hb_blob_get_నిడివి (blob)))
    DEBUG_సందేశం (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, దత్తాంశం->face_name);
  ఒకవేళ (unlikely (!blob))
  {
    hb_free (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  DWORD అంకె_fonts_installed;
  దత్తాంశం->fh = కూడు_Font_Mem_Resource_Ex ((శూన్య *) hb_blob_get_దత్తాంశం (blob, శూన్యసూచిక),
				   hb_blob_get_నిడివి (blob),
				   0, &అంకె_fonts_installed);
  ఒకవేళ (unlikely (!దత్తాంశం->fh))
  {
    DEBUG_సందేశం (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి దత్తాంశం;
}

శూన్య
hb_uniscribe_shaper_face_దత్తాంశం_destroy (hb_uniscribe_face_దత్తాంశం_t *దత్తాంశం)
{
  RemoveFontMemResourceEx (దత్తాంశం->fh);
  hb_free (దత్తాంశం);
}


/*
 * shaper font data
 */

నిర్మాణం hb_uniscribe_font_దత్తాంశం_t
{
  HDC hdc;
  మార్చదగిన LOGFONTW log_font;
  HFONT hfont;
  మార్చదగిన SCRIPT_CACHE script_cache;
  ద్విగుణ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

స్థిర తార్కిక
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   గుర్తులేని పూర్ణాంకం font_కొలత)
{
  hb_memset (lf, 0, పరిమాణం (*lf));
  lf->lf_ఎత్తు = - (పూర్ణాంకం) font_కొలత;
  lf->lf_Char_సమితి = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->దత్తాంశం.uniscribe->face_name, పరిమాణం (lf->lfFaceName));

  తిరిగి నిజం;
}

hb_uniscribe_font_దత్తాంశం_t *
hb_uniscribe_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  hb_uniscribe_font_దత్తాంశం_t *దత్తాంశం = (hb_uniscribe_font_దత్తాంశం_t *) hb_calloc (1, పరిమాణం (hb_uniscribe_font_దత్తాంశం_t));
  ఒకవేళ (unlikely (!దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  పూర్ణాంకం font_కొలత = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  ఒకవేళ (font->y_ppem)
    font_కొలత = font->y_ppem;

  ఒకవేళ (font_కొలత < 0)
    font_కొలత = -font_కొలత;
  దత్తాంశం->x_mult = (ద్విగుణ) font->x_scale / font_కొలత;
  దత్తాంశం->y_mult = (ద్విగుణ) font->y_scale / font_కొలత;

  దత్తాంశం->hdc = GetDC (శూన్యసూచిక);

  ఒకవేళ (unlikely (!populate_log_font (&దత్తాంశం->log_font, font, font_కొలత))) {
    DEBUG_సందేశం (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_దత్తాంశం_destroy (దత్తాంశం);
    తిరిగి శూన్యసూచిక;
  }

  దత్తాంశం->hfont = Create_Font_పరోక్ష_W (&దత్తాంశం->log_font);
  ఒకవేళ (unlikely (!దత్తాంశం->hfont)) {
    DEBUG_సందేశం (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_దత్తాంశం_destroy (దత్తాంశం);
     తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (!SelectObject (దత్తాంశం->hdc, దత్తాంశం->hfont)) {
    DEBUG_సందేశం (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_దత్తాంశం_destroy (దత్తాంశం);
     తిరిగి శూన్యసూచిక;
  }

  తిరిగి దత్తాంశం;
}

శూన్య
hb_uniscribe_shaper_font_దత్తాంశం_destroy (hb_uniscribe_font_దత్తాంశం_t *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం->hdc)
    ReleaseDC (శూన్యసూచిక, దత్తాంశం->hdc);
  ఒకవేళ (దత్తాంశం->hfont)
    DeleteObject (దత్తాంశం->hfont);
  ఒకవేళ (దత్తాంశం->script_cache)
    ScriptFreeCache (&దత్తాంశం->script_cache);
  hb_free (దత్తాంశం);
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
  స్థిరరాశి hb_uniscribe_font_దత్తాంశం_t *దత్తాంశం =  font->దత్తాంశం.uniscribe;
  తిరిగి దత్తాంశం ? &దత్తాంశం->log_font : శూన్యసూచిక;
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
  స్థిరరాశి hb_uniscribe_font_దత్తాంశం_t *దత్తాంశం =  font->దత్తాంశం.uniscribe;
  తిరిగి దత్తాంశం ? దత్తాంశం->hfont : శూన్యసూచిక;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_బఫరు_t        *బఫరు,
		     స్థిరరాశి hb_feature_t *features,
		     గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  hb_face_t *face = font->face;
  స్థిరరాశి hb_uniscribe_face_దత్తాంశం_t *face_దత్తాంశం = face->దత్తాంశం.uniscribe;
  స్థిరరాశి hb_uniscribe_font_దత్తాంశం_t *font_దత్తాంశం = font->దత్తాంశం.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_దత్తాంశం->funcs;

#నిర్వచించు FAIL(...) \
  HB_STMT_ప్రారంభం { \
    DEBUG_సందేశం (UNISCRIBE, శూన్యసూచిక, __VA_ARGS__); \
    తిరిగి అబద్ధం; \
  } HB_STMT_చివర

  HRESULT hr;

retry:

  గుర్తులేని పూర్ణాంకం scratch_కొలత;
  hb_బఫరు_t::scratch_బఫరు_t *scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);

#నిర్వచించు ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  చేయి { \
    గుర్తులేని పూర్ణాంకం _consumed = DIV_CEIL ((len) * పరిమాణం (Type), పరిమాణం (*scratch)); \
    ప్రతిజ్ఞా (_consumed <= scratch_కొలత); \
    scratch += _consumed; \
    scratch_కొలత -= _consumed; \
  } అయితే (0)

#నిర్వచించు utf16_సూచీ() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, బఫరు->len * 2);

  గుర్తులేని పూర్ణాంకం chars_len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
  {
    hb_codepoint_t c = బఫరు->info[i].codepoint;
    బఫరు->info[i].utf16_సూచీ() = chars_len;
    ఒకవేళ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    లేకపోతే ఒకవేళ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    లేకపోతే {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (మాట, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  ఒకవేళ (అంకె_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
    {
      hb_codepoint_t c = బఫరు->info[i].codepoint;
      గుర్తులేని పూర్ణాంకం cluster = బఫరు->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      ఒకవేళ (hb_లోపల_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  గుర్తులేని పూర్ణాంకం glyphs_కొలత = (scratch_కొలత * పరిమాణం (పూర్ణాంకం) - 2)
			   / (పరిమాణం (మాట) +
			      పరిమాణం (SCRIPT_GLYPHPROP) +
			      పరిమాణం (పూర్ణాంకం) +
			      పరిమాణం (GOFFSET) +
			      పరిమాణం (uint32_t));

  ALLOCATE_ARRAY (మాట, glyphs, glyphs_కొలత);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_కొలత);
  ALLOCATE_ARRAY (పూర్ణాంకం, advances, glyphs_కొలత);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_కొలత);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_కొలత);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#నిర్వచనం_రద్దు ALLOCATE_ARRAY

#నిర్వచించు MAX_వస్తువులు 256

  SCRIPT_అంశం వస్తువులు[MAX_వస్తువులు + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_ట్యాగులు[MAX_వస్తువులు];
  పూర్ణాంకం అంశం_లెక్క_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (బఫరు->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_వస్తువులు,
				     &bidi_control,
				     &bidi_state,
				     వస్తువులు,
				     script_ట్యాగులు,
				     &అంశం_లెక్క_2);
  ఒకవేళ (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (గుర్తులేని దీర్ఘ) hr);

#నిర్వచనం_రద్దు MAX_వస్తువులు

  hb_ట్యాగ్_t lang_ట్యాగ్;
  గుర్తులేని పూర్ణాంకం lang_లెక్క = 1;
  hb_ot_ట్యాగులు_from_script_and_language (బఫరు->props.script,
				       బఫరు->props.language,
				       శూన్యసూచిక, శూన్యసూచిక,
				       &lang_లెక్క, &lang_ట్యాగ్);
  OPENTYPE_ట్యాగ్ language_ట్యాగ్ = hb_uint32_మార్పిడి (lang_లెక్క ? lang_ట్యాగ్ : HB_ట్యాగ్_NONE);

  /*
   * Set up features.
   */
  static_assert ((పరిమాణం (TEXTRANGE_PROPERTIES) == పరిమాణం (hb_ms_features_t)), "");
  static_assert ((పరిమాణం (OPENTYPE_FEATURE_రికార్డు) == పరిమాణం (hb_ms_feature_t)), "");
  hb_సదిశ_t<hb_ms_feature_t> feature_రికార్డులు;
  hb_సదిశ_t<hb_ms_range_రికార్డు_t> range_రికార్డులు;
  తార్కిక has_features = అబద్ధం;
  ఒకవేళ (అంకె_features)
    has_features = hb_ms_అమరిక_features (features,
					 అంకె_features,
					 feature_రికార్డులు,
					 range_రికార్డులు);

  hb_సదిశ_t<hb_ms_features_t*> range_properties;
  hb_సదిశ_t<uint32_t> range_char_counts;

  గుర్తులేని పూర్ణాంకం glyphs_స్థానభ్రంశం = 0;
  గుర్తులేని పూర్ణాంకం glyphs_len;
  తార్కిక backward = HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction);
  కొరకు (పూర్ణాంకం i = 0; i < అంశం_లెక్క_2; i++)
  {
    గుర్తులేని పూర్ణాంకం chars_స్థానభ్రంశం = వస్తువులు[i].iCharPos;
    గుర్తులేని పూర్ణాంకం అంశం_chars_len = వస్తువులు[i + 1].iCharPos - chars_స్థానభ్రంశం;

    ఒకవేళ (has_features)
      hb_ms_make_feature_ranges (feature_రికార్డులు,
				 range_రికార్డులు,
				 అంశం_chars_len,
				 chars_స్థానభ్రంశం,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    వస్తువులు[i].a.f_Logical_క్రమం = నిజం;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_దత్తాంశం->hdc,
				     &font_దత్తాంశం->script_cache,
				     &వస్తువులు[i].a,
				     script_ట్యాగులు[i],
				     language_ట్యాగ్,
				     (పూర్ణాంకం *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.నిడివి,
				     pchars + chars_స్థానభ్రంశం,
				     అంశం_chars_len,
				     glyphs_కొలత - glyphs_స్థానభ్రంశం,
				     /* out */
				     log_clusters + chars_స్థానభ్రంశం,
				     char_props + chars_స్థానభ్రంశం,
				     glyphs + glyphs_స్థానభ్రంశం,
				     glyph_props + glyphs_స్థానభ్రంశం,
				     (పూర్ణాంకం *) &glyphs_len);

    ఒకవేళ (unlikely (వస్తువులు[i].a.f_No_Glyph_సూచీ))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    ఒకవేళ (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_బఫరు))
    {
      ఒకవేళ (unlikely (!బఫరు->ensure (బఫరు->allocated * 2)))
	FAIL ("Buffer resize failed");
      వెళ్ళు retry;
    }
    ఒకవేళ (unlikely (hr == USP_E_SCRIPT_NOT_లోపల_FONT))
    {
      ఒకవేళ (వస్తువులు[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      వస్తువులు[i].a.eScript = SCRIPT_UNDEFINED;
      వెళ్ళు retry_shape;
    }
    ఒకవేళ (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (గుర్తులేని దీర్ఘ) hr);
    }

    కొరకు (గుర్తులేని పూర్ణాంకం j = chars_స్థానభ్రంశం; j < chars_స్థానభ్రంశం + అంశం_chars_len; j++)
      log_clusters[j] += glyphs_స్థానభ్రంశం;

    hr = funcs->Script_ఉంచు_Open_Type (font_దత్తాంశం->hdc,
				     &font_దత్తాంశం->script_cache,
				     &వస్తువులు[i].a,
				     script_ట్యాగులు[i],
				     language_ట్యాగ్,
				     (పూర్ణాంకం *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.నిడివి,
				     pchars + chars_స్థానభ్రంశం,
				     log_clusters + chars_స్థానభ్రంశం,
				     char_props + chars_స్థానభ్రంశం,
				     అంశం_chars_len,
				     glyphs + glyphs_స్థానభ్రంశం,
				     glyph_props + glyphs_స్థానభ్రంశం,
				     glyphs_len,
				     /* out */
				     advances + glyphs_స్థానభ్రంశం,
				     offsets + glyphs_స్థానభ్రంశం,
				     శూన్యసూచిక);
    ఒకవేళ (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (గుర్తులేని దీర్ఘ) hr);

    ఒకవేళ (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       వస్తువులు[i].a.fRTL,
	       వస్తువులు[i].a.fLayoutRTL,
	       వస్తువులు[i].a.f_Logical_క్రమం,
	       HB_UNTAG (hb_uint32_మార్పిడి (script_ట్యాగులు[i])));

    glyphs_స్థానభ్రంశం += glyphs_len;
  }
  glyphs_len = glyphs_స్థానభ్రంశం;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[బఫరు->info[i].utf16_సూచీ()]];
    *p = hb_min (*p, బఫరు->info[i].cluster);
  }
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < glyphs_len; i++)
    ఒకవేళ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#నిర్వచనం_రద్దు utf16_సూచీ

  ఒకవేళ (unlikely (!బఫరు->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#నిర్వచనం_రద్దు FAIL

  /* Set glyph infos */
  బఫరు->len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &బఫరు->info[బఫరు->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ముసుగు = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  బఫరు->clear_positions ();
  ద్విగుణ x_mult = font_దత్తాంశం->x_mult, y_mult = font_దత్తాంశం->y_mult;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &బఫరు->info[i];
    hb_glyph_position_t *pos = &బఫరు->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_గమ్యం<hb_position_t> (x_mult * (int32_t) info->ముసుగు);
    ద్విగుణ x_స్థానభ్రంశం_2 = backward ? -(ద్విగుణ) info->var1.i32 : info->var1.i32;
    pos->x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (x_mult * x_స్థానభ్రంశం_2);
    pos->y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (y_mult * info->var2.i32);
  }

  ఒకవేళ (backward)
    hb_బఫరు_తిరగేయి (బఫరు);

  బఫరు->clear_glyph_flags ();
  బఫరు->unsafe_గమ్యం_break ();

  /* Wow, done! */
  తిరిగి నిజం;
}


#అయితే_ముగింపు
