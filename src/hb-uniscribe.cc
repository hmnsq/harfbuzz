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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_UNISCRIBE

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_ਟੈਗ
#ਗਲਤੀ "Cannot compile 'uniscribe' shaper with HB_NO_OT_TAG."
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <usp10.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <rpc.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ E_NOT_SUFFICIENT_ਬਫਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ E_NOT_SUFFICIENT_ਬਫਰ HRESULT_FROM_WIN32 (ERROR_INSUFFICIENT_ਬਫਰ)
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-uniscribe.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ms-feature-ranges.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-file.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout.h"


/**
 * SECTION:hb-uniscribe
 * @title: hb-uniscribe
 * @short_description: Windows integration
 * @include: hb-uniscribe.h
 *
 * Functions for using HarfBuzz with Windows fonts.
 **/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ HRESULT (WINAPI *SIOT) /*ScriptItemizeOpenType*/(
  ਅਚਲ WCHAR *pwc_ਵਿੱਚ_Chars,
  ਪੂਰਨ c_ਵਿੱਚ_Chars,
  ਪੂਰਨ c_Max_ਚੀਜ਼ਾਂ,
  ਅਚਲ SCRIPT_CONTROL *psControl,
  ਅਚਲ SCRIPT_STATE *psState,
  SCRIPT_ਵਸਤੂ *p_ਚੀਜ਼ਾਂ,
  OPENTYPE_ਟੈਗ *p_Script_ਟੈਗ_ਸੂਚੀ,
  ਪੂਰਨ *pc_ਚੀਜ਼ਾਂ
);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ HRESULT (WINAPI *SSOT) /*ScriptShapeOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ਟੈਗ ਟੈਗ_Script,
  OPENTYPE_ਟੈਗ ਟੈਗ_Lang_Sys,
  ਪੂਰਨ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ਪੂਰਨ cRanges,
  ਅਚਲ WCHAR *pwcChars,
  ਪੂਰਨ cChars,
  ਪੂਰਨ cMaxGlyphs,
  ਸ਼ਬਦ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ਸ਼ਬਦ *pw_ਬਾਹਰਲਾ_Glyphs,
  SCRIPT_GLYPHPROP *p_ਬਾਹਰਲਾ_Glyph_Props,
  ਪੂਰਨ *pcGlyphs
);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ HRESULT (WINAPI *SPOT) /*ScriptPlaceOpenType*/(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ਟੈਗ ਟੈਗ_Script,
  OPENTYPE_ਟੈਗ ਟੈਗ_Lang_Sys,
  ਪੂਰਨ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ਪੂਰਨ cRanges,
  ਅਚਲ WCHAR *pwcChars,
  ਸ਼ਬਦ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ਪੂਰਨ cChars,
  ਅਚਲ ਸ਼ਬਦ *pwGlyphs,
  ਅਚਲ SCRIPT_GLYPHPROP *pGlyphProps,
  ਪੂਰਨ cGlyphs,
  ਪੂਰਨ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
);


/* Fallback implementations. */

ਸਥਿਰ HRESULT WINAPI
hb_ScriptItemizeOpenType(
  ਅਚਲ WCHAR *pwc_ਵਿੱਚ_Chars,
  ਪੂਰਨ c_ਵਿੱਚ_Chars,
  ਪੂਰਨ c_Max_ਚੀਜ਼ਾਂ,
  ਅਚਲ SCRIPT_CONTROL *psControl,
  ਅਚਲ SCRIPT_STATE *psState,
  SCRIPT_ਵਸਤੂ *p_ਚੀਜ਼ਾਂ,
  OPENTYPE_ਟੈਗ *p_Script_ਟੈਗ_ਸੂਚੀ,
  ਪੂਰਨ *pc_ਚੀਜ਼ਾਂ
)
{
{
  ਵਾਪਸ ScriptItemize (pwc_ਵਿੱਚ_Chars,
			c_ਵਿੱਚ_Chars,
			c_Max_ਚੀਜ਼ਾਂ,
			psControl,
			psState,
			p_ਚੀਜ਼ਾਂ,
			pc_ਚੀਜ਼ਾਂ);
}
}

ਸਥਿਰ HRESULT WINAPI
hb_ScriptShapeOpenType(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ਟੈਗ ਟੈਗ_Script,
  OPENTYPE_ਟੈਗ ਟੈਗ_Lang_Sys,
  ਪੂਰਨ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ਪੂਰਨ cRanges,
  ਅਚਲ WCHAR *pwcChars,
  ਪੂਰਨ cChars,
  ਪੂਰਨ cMaxGlyphs,
  ਸ਼ਬਦ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ਸ਼ਬਦ *pw_ਬਾਹਰਲਾ_Glyphs,
  SCRIPT_GLYPHPROP *p_ਬਾਹਰਲਾ_Glyph_Props,
  ਪੂਰਨ *pcGlyphs
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) p_ਬਾਹਰਲਾ_Glyph_Props;
  ਵਾਪਸ ScriptShape (hdc,
		      psc,
		      pwcChars,
		      cChars,
		      cMaxGlyphs,
		      psa,
		      pw_ਬਾਹਰਲਾ_Glyphs,
		      pwLogClust,
		      psva,
		      pcGlyphs);
}

ਸਥਿਰ HRESULT WINAPI
hb_Script_ਰੱਖੋ_Open_Type(
  HDC hdc,
  SCRIPT_CACHE *psc,
  SCRIPT_ANALYSIS *psa,
  OPENTYPE_ਟੈਗ ਟੈਗ_Script,
  OPENTYPE_ਟੈਗ ਟੈਗ_Lang_Sys,
  ਪੂਰਨ *rcRangeChars,
  TEXTRANGE_PROPERTIES **rpRangeProperties,
  ਪੂਰਨ cRanges,
  ਅਚਲ WCHAR *pwcChars,
  ਸ਼ਬਦ *pwLogClust,
  SCRIPT_CHARPROP *pCharProps,
  ਪੂਰਨ cChars,
  ਅਚਲ ਸ਼ਬਦ *pwGlyphs,
  ਅਚਲ SCRIPT_GLYPHPROP *pGlyphProps,
  ਪੂਰਨ cGlyphs,
  ਪੂਰਨ *piAdvance,
  GOFFSET *pGoffset,
  ABC *pABC
)
{
  SCRIPT_VISATTR *psva = (SCRIPT_VISATTR *) pGlyphProps;
  ਵਾਪਸ Script_ਰੱਖੋ (hdc,
		      psc,
		      pwGlyphs,
		      cGlyphs,
		      psva,
		      psa,
		      piAdvance,
		      pGoffset,
		      pABC);
}


ਬਣਤਰ hb_uniscribe_shaper_funcs_t
{
  SIOT ScriptItemizeOpenType;
  SSOT ScriptShapeOpenType;
  SPOT Script_ਰੱਖੋ_Open_Type;

  ਖਾਲੀ init ()
  {
    HMODULE hinstLib;
    ਇਹ->ScriptItemizeOpenType = ਖਾਲੀ_ਸੂਚਕ;
    ਇਹ->ScriptShapeOpenType   = ਖਾਲੀ_ਸੂਚਕ;
    ਇਹ->Script_ਰੱਖੋ_Open_Type   = ਖਾਲੀ_ਸੂਚਕ;

    hinstLib = GetModuleHandle (ਪਾਠ_2 ("usp10.dll"));
    ਜੇ (hinstLib)
    {
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wcast-function-type"
      ਇਹ->ScriptItemizeOpenType = (SIOT) Get_Proc_ਪਤਾ (hinstLib, "ScriptItemizeOpenType");
      ਇਹ->ScriptShapeOpenType   = (SSOT) Get_Proc_ਪਤਾ (hinstLib, "ScriptShapeOpenType");
      ਇਹ->Script_ਰੱਖੋ_Open_Type   = (SPOT) Get_Proc_ਪਤਾ (hinstLib, "ScriptPlaceOpenType");
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
    }
    ਜੇ (!ਇਹ->ScriptItemizeOpenType ||
	!ਇਹ->ScriptShapeOpenType   ||
	!ਇਹ->Script_ਰੱਖੋ_Open_Type)
    {
      DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, ਖਾਲੀ_ਸੂਚਕ, "OpenType versions of functions not found; falling back.");
      ਇਹ->ScriptItemizeOpenType = hb_ScriptItemizeOpenType;
      ਇਹ->ScriptShapeOpenType   = hb_ScriptShapeOpenType;
      ਇਹ->Script_ਰੱਖੋ_Open_Type   = hb_Script_ਰੱਖੋ_Open_Type;
    }
  }
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_uniscribe_shaper_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_uniscribe_shaper_funcs_lazy_loader_t : hb_lazy_loader_t<hb_uniscribe_shaper_funcs_t,
									 hb_uniscribe_shaper_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_uniscribe_shaper_funcs_t *create ()
  {
    hb_uniscribe_shaper_funcs_t *funcs = (hb_uniscribe_shaper_funcs_t *) hb_calloc (1, ਆਕਾਰ (hb_uniscribe_shaper_funcs_t));
    ਜੇ (unlikely (!funcs))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    funcs->init ();

    hb_atexit (free_static_uniscribe_shaper_funcs);

    ਵਾਪਸ funcs;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_uniscribe_shaper_funcs_t *p)
  {
    hb_free ((ਖਾਲੀ *) p);
  }
  ਸਥਿਰ hb_uniscribe_shaper_funcs_t *get_null ()
  {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
} static_uniscribe_shaper_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_uniscribe_shaper_funcs ()
{
  static_uniscribe_shaper_funcs.free_instance ();
}

ਸਥਿਰ hb_uniscribe_shaper_funcs_t *
hb_uniscribe_shaper_get_funcs ()
{
  ਵਾਪਸ static_uniscribe_shaper_funcs.get_unconst ();
}


/*
 * shaper face data
 */

ਬਣਤਰ hb_uniscribe_face_ਅੰਕੜੇ_t {
  HANDLE fh;
  hb_uniscribe_shaper_funcs_t *funcs;
  wchar_t face_name[LF_FACESIZE];
};

/* face_name should point to a wchar_t[LF_FACESIZE] object. */
ਸਥਿਰ ਖਾਲੀ
_hb_generate_unique_face_name (wchar_t *face_name, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *plen)
{
  /* We'll create a private name for the font from a UUID using a simple,
   * somewhat base64-like encoding scheme */
  ਅਚਲ ਅੱਖਰ *enc = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";
  UUID id;
  UuidCreate ((UUID*) &id);
  static_assert ((2 + 3 * (16/2) < LF_FACESIZE), "");
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ name_str_len = 0;
  face_name[name_str_len++] = 'F';
  face_name[name_str_len++] = '_';
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) &id;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < 16; i += 2)
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
  ਜੇ (plen)
    *plen = name_str_len;
}

/* Destroys blob. */
ਸਥਿਰ hb_blob_t *
_hb_rename_font (hb_blob_t *blob_ਵਿੱਚ, wchar_t *new_name)
{
  /* Create a copy of the font data, with the 'name' table replaced by a
   * table that names the font with our private F_* name created above.
   * For simplicity, we just append a new 'name' table and update the
   * sfnt directory; the original table is left in place, but unused.
   *
   * The new table will contain just 5 name IDs: family, style, unique,
   * full, PS. All of them point to the same name data with our unique name.
   */

  hb_unique_ptr_t<hb_blob_t> blob (hb_sanitize_context_t ().sanitize_blob<OT::OpenTypeFontFile> (blob_ਵਿੱਚ));

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ, new_ਲੰਮਾਈ, name_str_len;
  ਅਚਲ ਅੱਖਰ *orig_sfnt_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &ਲੰਮਾਈ);

  _hb_generate_unique_face_name (new_name, &name_str_len);

  ਸਥਿਰ ਅਚਲ uint16_t name_IDs[] = { 1, 2, 3, 4, 6 };

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ name_ਸਾਰਣੀ_ਲੰਮਾਈ = OT::name::min_ਮਾਪ +
				   ARRAY_ਲੰਮਾਈ (name_IDs) * OT::Name_ਰਿਕਾਰਡ::static_ਮਾਪ +
				   name_str_len * 2; /* for name data in UTF16BE form */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ padded_name_ਸਾਰਣੀ_ਲੰਮਾਈ = ((name_ਸਾਰਣੀ_ਲੰਮਾਈ + 3) & ~3);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ name_ਸਾਰਣੀ_ਵਿੱਥ = (ਲੰਮਾਈ + 3) & ~3;

  new_ਲੰਮਾਈ = name_ਸਾਰਣੀ_ਵਿੱਥ + padded_name_ਸਾਰਣੀ_ਲੰਮਾਈ;
  ਖਾਲੀ *new_sfnt_ਅੰਕੜੇ = hb_calloc (1, new_ਲੰਮਾਈ);
  ਜੇ (!new_sfnt_ਅੰਕੜੇ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਸਵੈਚਾਲਿਤ sfnt_guard = hb_make_scope_guard ([&]() { hb_free (new_sfnt_ਅੰਕੜੇ); });

  hb_memcpy(new_sfnt_ਅੰਕੜੇ, orig_sfnt_ਅੰਕੜੇ, ਲੰਮਾਈ);

  OT::name &name = Struct_At_ਵਿੱਥ<OT::name> (new_sfnt_ਅੰਕੜੇ, name_ਸਾਰਣੀ_ਵਿੱਥ);
  name.format = 0;
  name.ਗਿਣਤੀ_2 = ARRAY_ਲੰਮਾਈ (name_IDs);
  name.string_ਵਿੱਥ = name.get_ਮਾਪ ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (name_IDs); i++)
  {
    OT::Name_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = name.name_ਰਿਕਾਰਡ_Z[i];
    ਰਿਕਾਰਡ.platformID = 3;
    ਰਿਕਾਰਡ.encodingID = 1;
    ਰਿਕਾਰਡ.languageID = 0x0409u; /* English */
    ਰਿਕਾਰਡ.nameID = name_IDs[i];
    ਰਿਕਾਰਡ.ਲੰਮਾਈ = name_str_len * 2;
    ਰਿਕਾਰਡ.ਵਿੱਥ = 0;
  }

  /* Copy string data from new_name, converting wchar_t to UTF16BE. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = &StructAfter<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> (name);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < name_str_len; i++)
  {
    *p++ = new_name[i] >> 8;
    *p++ = new_name[i] & 0xff;
  }

  /* Adjust name table entry to point to new name table */
  ਅਚਲ OT::OpenTypeFontFile &file = * (OT::OpenTypeFontFile *) (new_sfnt_ਅੰਕੜੇ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ face_ਗਿਣਤੀ = file.get_face_ਗਿਣਤੀ ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ face_ਸੂਚਕਾਂਕ = 0; face_ਸੂਚਕਾਂਕ < face_ਗਿਣਤੀ; face_ਸੂਚਕਾਂਕ++)
  {
    /* Note: doing multiple edits (ie. TTC) can be unsafe.  There may be
     * toe-stepping.  But we don't really care. */
    ਅਚਲ OT::OpenTypeFontFace &face = file.get_face (face_ਸੂਚਕਾਂਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
    ਜੇ (face.ਲੱਭੋ_ਸਾਰਣੀ_ਸੂਚਕਾਂਕ (HB_OT_ਟੈਗ_name, &ਸੂਚਕਾਂਕ_2))
    {
      OT::ਸਾਰਣੀ_ਰਿਕਾਰਡ &ਰਿਕਾਰਡ = const_cast<OT::ਸਾਰਣੀ_ਰਿਕਾਰਡ &> (face.get_ਸਾਰਣੀ (ਸੂਚਕਾਂਕ_2));
      ਰਿਕਾਰਡ.check_ਜੋੜ.ਸੈੱਟ_for_ਅੰਕੜੇ (&name, padded_name_ਸਾਰਣੀ_ਲੰਮਾਈ);
      ਰਿਕਾਰਡ.ਵਿੱਥ = name_ਸਾਰਣੀ_ਵਿੱਥ;
      ਰਿਕਾਰਡ.ਲੰਮਾਈ = name_ਸਾਰਣੀ_ਲੰਮਾਈ;
    }
    ਨਹੀਂਤਾਂ ਜੇ (face_ਸੂਚਕਾਂਕ == 0) /* Fail if first face doesn't have 'name' table. */
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  /* The checkSumAdjustment field in the 'head' table is now wrong,
   * but that doesn't actually seem to cause any problems so we don't
   * bother. */

  sfnt_guard.release ();
  ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) new_sfnt_ਅੰਕੜੇ, new_ਲੰਮਾਈ,
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_WRITABLE, new_sfnt_ਅੰਕੜੇ, hb_free);
}

hb_uniscribe_face_ਅੰਕੜੇ_t *
hb_uniscribe_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  hb_uniscribe_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_uniscribe_face_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (hb_uniscribe_face_ਅੰਕੜੇ_t));
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅੰਕੜੇ->funcs = hb_uniscribe_shaper_get_funcs ();
  ਜੇ (unlikely (!ਅੰਕੜੇ->funcs))
  {
    hb_free (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_blob_t *blob = hb_face_reference_blob (face);
  ਜੇ (unlikely (!hb_blob_get_ਲੰਮਾਈ (blob)))
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, face, "Face has empty blob");

  blob = _hb_rename_font (blob, ਅੰਕੜੇ->face_name);
  ਜੇ (unlikely (!blob))
  {
    hb_free (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  DWORD ਅੰਕ_fonts_installed;
  ਅੰਕੜੇ->fh = ਜੋੜੋ_Font_Mem_Resource_Ex ((ਖਾਲੀ *) hb_blob_get_ਅੰਕੜੇ (blob, ਖਾਲੀ_ਸੂਚਕ),
				   hb_blob_get_ਲੰਮਾਈ (blob),
				   0, &ਅੰਕ_fonts_installed);
  ਜੇ (unlikely (!ਅੰਕੜੇ->fh))
  {
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, face, "Face AddFontMemResourceEx() failed");
    hb_free (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਅੰਕੜੇ;
}

ਖਾਲੀ
hb_uniscribe_shaper_face_ਅੰਕੜੇ_destroy (hb_uniscribe_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  RemoveFontMemResourceEx (ਅੰਕੜੇ->fh);
  hb_free (ਅੰਕੜੇ);
}


/*
 * shaper font data
 */

ਬਣਤਰ hb_uniscribe_font_ਅੰਕੜੇ_t
{
  HDC hdc;
  ਬਦਲਣਯੋਗ LOGFONTW log_font;
  HFONT hfont;
  ਬਦਲਣਯੋਗ SCRIPT_CACHE script_cache;
  ਦੁੱਗਣਾ x_mult, y_mult; /* From LOGFONT space to HB space. */
};

ਸਥਿਰ ਤਾਰਕਿਕ
populate_log_font (LOGFONTW  *lf,
		   hb_font_t *font,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ font_ਮਾਪ)
{
  hb_memset (lf, 0, ਆਕਾਰ (*lf));
  lf->lf_ਉਚਾਈ = - (ਪੂਰਨ) font_ਮਾਪ;
  lf->lf_Char_ਸੈੱਟ = DEFAULT_CHARSET;

  hb_memcpy (lf->lfFaceName, font->face->ਅੰਕੜੇ.uniscribe->face_name, ਆਕਾਰ (lf->lfFaceName));

  ਵਾਪਸ ਸੱਚ;
}

hb_uniscribe_font_ਅੰਕੜੇ_t *
hb_uniscribe_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  hb_uniscribe_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ = (hb_uniscribe_font_ਅੰਕੜੇ_t *) hb_calloc (1, ਆਕਾਰ (hb_uniscribe_font_ਅੰਕੜੇ_t));
  ਜੇ (unlikely (!ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਪੂਰਨ font_ਮਾਪ = font->face->get_upem (); /* Default... */
  /* No idea if the following is even a good idea. */
  ਜੇ (font->y_ppem)
    font_ਮਾਪ = font->y_ppem;

  ਜੇ (font_ਮਾਪ < 0)
    font_ਮਾਪ = -font_ਮਾਪ;
  ਅੰਕੜੇ->x_mult = (ਦੁੱਗਣਾ) font->x_scale / font_ਮਾਪ;
  ਅੰਕੜੇ->y_mult = (ਦੁੱਗਣਾ) font->y_scale / font_ਮਾਪ;

  ਅੰਕੜੇ->hdc = GetDC (ਖਾਲੀ_ਸੂਚਕ);

  ਜੇ (unlikely (!populate_log_font (&ਅੰਕੜੇ->log_font, font, font_ਮਾਪ))) {
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, font, "Font populate_log_font() failed");
    hb_uniscribe_shaper_font_ਅੰਕੜੇ_destroy (ਅੰਕੜੇ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਅੰਕੜੇ->hfont = Create_Font_ਅਸਿੱਧਾ_W (&ਅੰਕੜੇ->log_font);
  ਜੇ (unlikely (!ਅੰਕੜੇ->hfont)) {
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, font, "Font CreateFontIndirectW() failed");
    hb_uniscribe_shaper_font_ਅੰਕੜੇ_destroy (ਅੰਕੜੇ);
     ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (!SelectObject (ਅੰਕੜੇ->hdc, ਅੰਕੜੇ->hfont)) {
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, font, "Font SelectObject() failed");
    hb_uniscribe_shaper_font_ਅੰਕੜੇ_destroy (ਅੰਕੜੇ);
     ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ ਅੰਕੜੇ;
}

ਖਾਲੀ
hb_uniscribe_shaper_font_ਅੰਕੜੇ_destroy (hb_uniscribe_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ->hdc)
    ReleaseDC (ਖਾਲੀ_ਸੂਚਕ, ਅੰਕੜੇ->hdc);
  ਜੇ (ਅੰਕੜੇ->hfont)
    DeleteObject (ਅੰਕੜੇ->hfont);
  ਜੇ (ਅੰਕੜੇ->script_cache)
    ScriptFreeCache (&ਅੰਕੜੇ->script_cache);
  hb_free (ਅੰਕੜੇ);
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
  ਅਚਲ hb_uniscribe_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ =  font->ਅੰਕੜੇ.uniscribe;
  ਵਾਪਸ ਅੰਕੜੇ ? &ਅੰਕੜੇ->log_font : ਖਾਲੀ_ਸੂਚਕ;
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
  ਅਚਲ hb_uniscribe_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ =  font->ਅੰਕੜੇ.uniscribe;
  ਵਾਪਸ ਅੰਕੜੇ ? ਅੰਕੜੇ->hfont : ਖਾਲੀ_ਸੂਚਕ;
}


/*
 * shaper
 */


hb_bool_t
_hb_uniscribe_shape (hb_shape_plan_t    *shape_plan,
		     hb_font_t          *font,
		     hb_ਬਫਰ_t        *ਬਫਰ,
		     ਅਚਲ hb_feature_t *features,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  hb_face_t *face = font->face;
  ਅਚਲ hb_uniscribe_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ = face->ਅੰਕੜੇ.uniscribe;
  ਅਚਲ hb_uniscribe_font_ਅੰਕੜੇ_t *font_ਅੰਕੜੇ = font->ਅੰਕੜੇ.uniscribe;
  hb_uniscribe_shaper_funcs_t *funcs = face_ਅੰਕੜੇ->funcs;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL(...) \
  HB_STMT_ਸ਼ੁਰੂ { \
    DEBUG_ਸੁਨੇਹਾ (UNISCRIBE, ਖਾਲੀ_ਸੂਚਕ, __VA_ARGS__); \
    ਵਾਪਸ ਝੂਠ; \
  } HB_STMT_ਸਿਰਾ

  HRESULT hr;

retry:

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scratch_ਮਾਪ;
  hb_ਬਫਰ_t::scratch_ਬਫਰ_t *scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALLOCATE_ARRAY(Type, name, len) \
  Type *name = (Type *) scratch; \
  ਕਰੋ { \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ _consumed = DIV_CEIL ((len) * ਆਕਾਰ (Type), ਆਕਾਰ (*scratch)); \
    ਪ੍ਰਤਿਜ੍ਞਾ (_consumed <= scratch_ਮਾਪ); \
    scratch += _consumed; \
    scratch_ਮਾਪ -= _consumed; \
  } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ utf16_ਸੂਚਕਾਂਕ() var1.u32

  ALLOCATE_ARRAY (WCHAR, pchars, ਬਫਰ->len * 2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ chars_len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
  {
    hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
    ਬਫਰ->info[i].utf16_ਸੂਚਕਾਂਕ() = chars_len;
    ਜੇ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ਨਹੀਂਤਾਂ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (ਸ਼ਬਦ, log_clusters, chars_len);
  ALLOCATE_ARRAY (SCRIPT_CHARPROP, char_props, chars_len);

  ਜੇ (ਅੰਕ_features)
  {
    /* Need log_clusters to assign features. */
    chars_len = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
    {
      hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = ਬਫਰ->info[i].cluster;
      log_clusters[chars_len++] = cluster;
      ਜੇ (hb_ਵਿੱਚ_range (c, 0x10000u, 0x10FFFFu))
	log_clusters[chars_len++] = cluster; /* Surrogates. */
    }
  }

  /* The -2 in the following is to compensate for possible
   * alignment needed after the WORD array.  sizeof(WORD) == 2. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyphs_ਮਾਪ = (scratch_ਮਾਪ * ਆਕਾਰ (ਪੂਰਨ) - 2)
			   / (ਆਕਾਰ (ਸ਼ਬਦ) +
			      ਆਕਾਰ (SCRIPT_GLYPHPROP) +
			      ਆਕਾਰ (ਪੂਰਨ) +
			      ਆਕਾਰ (GOFFSET) +
			      ਆਕਾਰ (uint32_t));

  ALLOCATE_ARRAY (ਸ਼ਬਦ, glyphs, glyphs_ਮਾਪ);
  ALLOCATE_ARRAY (SCRIPT_GLYPHPROP, glyph_props, glyphs_ਮਾਪ);
  ALLOCATE_ARRAY (ਪੂਰਨ, advances, glyphs_ਮਾਪ);
  ALLOCATE_ARRAY (GOFFSET, offsets, glyphs_ਮਾਪ);
  ALLOCATE_ARRAY (uint32_t, vis_clusters, glyphs_ਮਾਪ);

  /* Note:
   * We can't touch the contents of glyph_props.  Our fallback
   * implementations of Shape and Place functions use that buffer
   * by casting it to a different type.  It works because they
   * both agree about it, but if we want to access it here we
   * need address that issue first.
   */

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALLOCATE_ARRAY

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਚੀਜ਼ਾਂ 256

  SCRIPT_ਵਸਤੂ ਚੀਜ਼ਾਂ[MAX_ਚੀਜ਼ਾਂ + 1];
  SCRIPT_CONTROL bidi_control = {0};
  SCRIPT_STATE bidi_state = {0};
  ULONG script_ਟੈਗ_ਸੂਚੀ[MAX_ਚੀਜ਼ਾਂ];
  ਪੂਰਨ ਵਸਤੂ_ਗਿਣਤੀ_2;

  /* MinGW32 doesn't define fMergeNeutralItems, so we bruteforce */
  //bidi_control.fMergeNeutralItems = true;
  *(uint32_t*)&bidi_control |= 1u<<24;

  bidi_state.uBidiLevel = HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction) ? 0 : 1;
  bidi_state.fOverrideDirection = 1;

  hr = funcs->ScriptItemizeOpenType (pchars,
				     chars_len,
				     MAX_ਚੀਜ਼ਾਂ,
				     &bidi_control,
				     &bidi_state,
				     ਚੀਜ਼ਾਂ,
				     script_ਟੈਗ_ਸੂਚੀ,
				     &ਵਸਤੂ_ਗਿਣਤੀ_2);
  ਜੇ (unlikely (FAILED (hr)))
    FAIL ("ScriptItemizeOpenType() failed: 0x%08lx", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) hr);

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MAX_ਚੀਜ਼ਾਂ

  hb_ਟੈਗ_t lang_ਟੈਗ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lang_ਗਿਣਤੀ = 1;
  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (ਬਫਰ->props.script,
				       ਬਫਰ->props.language,
				       ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ,
				       &lang_ਗਿਣਤੀ, &lang_ਟੈਗ);
  OPENTYPE_ਟੈਗ language_ਟੈਗ = hb_uint32_ਅਦਲਾ_ਬਦਲੀ (lang_ਗਿਣਤੀ ? lang_ਟੈਗ : HB_ਟੈਗ_NONE);

  /*
   * Set up features.
   */
  static_assert ((ਆਕਾਰ (TEXTRANGE_PROPERTIES) == ਆਕਾਰ (hb_ms_features_t)), "");
  static_assert ((ਆਕਾਰ (OPENTYPE_FEATURE_ਰਿਕਾਰਡ) == ਆਕਾਰ (hb_ms_feature_t)), "");
  hb_ਸਦਿਸ਼_t<hb_ms_feature_t> feature_ਰਿਕਾਰਡਾਂ;
  hb_ਸਦਿਸ਼_t<hb_ms_range_ਰਿਕਾਰਡ_t> range_ਰਿਕਾਰਡਾਂ;
  ਤਾਰਕਿਕ has_features = ਝੂਠ;
  ਜੇ (ਅੰਕ_features)
    has_features = hb_ms_ਤਿਆਰੀ_features (features,
					 ਅੰਕ_features,
					 feature_ਰਿਕਾਰਡਾਂ,
					 range_ਰਿਕਾਰਡਾਂ);

  hb_ਸਦਿਸ਼_t<hb_ms_features_t*> range_properties;
  hb_ਸਦਿਸ਼_t<uint32_t> range_char_counts;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyphs_ਵਿੱਥ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyphs_len;
  ਤਾਰਕਿਕ backward = HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction);
  ਲਈ (ਪੂਰਨ i = 0; i < ਵਸਤੂ_ਗਿਣਤੀ_2; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ chars_ਵਿੱਥ = ਚੀਜ਼ਾਂ[i].iCharPos;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਸਤੂ_chars_len = ਚੀਜ਼ਾਂ[i + 1].iCharPos - chars_ਵਿੱਥ;

    ਜੇ (has_features)
      hb_ms_make_feature_ranges (feature_ਰਿਕਾਰਡਾਂ,
				 range_ਰਿਕਾਰਡਾਂ,
				 ਵਸਤੂ_chars_len,
				 chars_ਵਿੱਥ,
				 log_clusters,
				 range_properties,
				 range_char_counts);

    /* Asking for glyphs in logical order circumvents at least
     * one bug in Uniscribe. */
    ਚੀਜ਼ਾਂ[i].a.f_Logical_ਕ੍ਰਮ = ਸੱਚ;

  retry_shape:
    hr = funcs->ScriptShapeOpenType (font_ਅੰਕੜੇ->hdc,
				     &font_ਅੰਕੜੇ->script_cache,
				     &ਚੀਜ਼ਾਂ[i].a,
				     script_ਟੈਗ_ਸੂਚੀ[i],
				     language_ਟੈਗ,
				     (ਪੂਰਨ *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.ਲੰਮਾਈ,
				     pchars + chars_ਵਿੱਥ,
				     ਵਸਤੂ_chars_len,
				     glyphs_ਮਾਪ - glyphs_ਵਿੱਥ,
				     /* out */
				     log_clusters + chars_ਵਿੱਥ,
				     char_props + chars_ਵਿੱਥ,
				     glyphs + glyphs_ਵਿੱਥ,
				     glyph_props + glyphs_ਵਿੱਥ,
				     (ਪੂਰਨ *) &glyphs_len);

    ਜੇ (unlikely (ਚੀਜ਼ਾਂ[i].a.f_No_Glyph_ਸੂਚਕਾਂਕ))
      FAIL ("ScriptShapeOpenType() set fNoGlyphIndex");
    ਜੇ (unlikely (hr == E_OUTOFMEMORY || hr == E_NOT_SUFFICIENT_ਬਫਰ))
    {
      ਜੇ (unlikely (!ਬਫਰ->ensure (ਬਫਰ->allocated * 2)))
	FAIL ("Buffer resize failed");
      ਜਾਓ retry;
    }
    ਜੇ (unlikely (hr == USP_E_SCRIPT_NOT_ਵਿੱਚ_FONT))
    {
      ਜੇ (ਚੀਜ਼ਾਂ[i].a.eScript == SCRIPT_UNDEFINED)
	FAIL ("ScriptShapeOpenType() failed: Font doesn't support script");
      ਚੀਜ਼ਾਂ[i].a.eScript = SCRIPT_UNDEFINED;
      ਜਾਓ retry_shape;
    }
    ਜੇ (unlikely (FAILED (hr)))
    {
      FAIL ("ScriptShapeOpenType() failed: 0x%08lx", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) hr);
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = chars_ਵਿੱਥ; j < chars_ਵਿੱਥ + ਵਸਤੂ_chars_len; j++)
      log_clusters[j] += glyphs_ਵਿੱਥ;

    hr = funcs->Script_ਰੱਖੋ_Open_Type (font_ਅੰਕੜੇ->hdc,
				     &font_ਅੰਕੜੇ->script_cache,
				     &ਚੀਜ਼ਾਂ[i].a,
				     script_ਟੈਗ_ਸੂਚੀ[i],
				     language_ਟੈਗ,
				     (ਪੂਰਨ *) range_char_counts.arrayZ,
				     (TEXTRANGE_PROPERTIES**) range_properties.arrayZ,
				     range_properties.ਲੰਮਾਈ,
				     pchars + chars_ਵਿੱਥ,
				     log_clusters + chars_ਵਿੱਥ,
				     char_props + chars_ਵਿੱਥ,
				     ਵਸਤੂ_chars_len,
				     glyphs + glyphs_ਵਿੱਥ,
				     glyph_props + glyphs_ਵਿੱਥ,
				     glyphs_len,
				     /* out */
				     advances + glyphs_ਵਿੱਥ,
				     offsets + glyphs_ਵਿੱਥ,
				     ਖਾਲੀ_ਸੂਚਕ);
    ਜੇ (unlikely (FAILED (hr)))
      FAIL ("ScriptPlaceOpenType() failed: 0x%08lx", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) hr);

    ਜੇ (DEBUG_ENABLED (UNISCRIBE))
      fprintf (stderr, "Item %d RTL %d LayoutRTL %d LogicalOrder %d ScriptTag %c%c%c%c\n",
	       i,
	       ਚੀਜ਼ਾਂ[i].a.fRTL,
	       ਚੀਜ਼ਾਂ[i].a.fLayoutRTL,
	       ਚੀਜ਼ਾਂ[i].a.f_Logical_ਕ੍ਰਮ,
	       HB_UNTAG (hb_uint32_ਅਦਲਾ_ਬਦਲੀ (script_ਟੈਗ_ਸੂਚੀ[i])));

    glyphs_ਵਿੱਥ += glyphs_len;
  }
  glyphs_len = glyphs_ਵਿੱਥ;

  /* Ok, we've got everything we need, now compose output buffer,
   * very, *very*, carefully! */

  /* Calculate visual-clusters.  That's what we ship. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyphs_len; i++)
    vis_clusters[i] = (uint32_t) -1;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++) {
    uint32_t *p = &vis_clusters[log_clusters[ਬਫਰ->info[i].utf16_ਸੂਚਕਾਂਕ()]];
    *p = hb_min (*p, ਬਫਰ->info[i].cluster);
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < glyphs_len; i++)
    ਜੇ (vis_clusters[i] == (uint32_t) -1)
      vis_clusters[i] = vis_clusters[i - 1];

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ utf16_ਸੂਚਕਾਂਕ

  ਜੇ (unlikely (!ਬਫਰ->ensure (glyphs_len)))
    FAIL ("Buffer in error");

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FAIL

  /* Set glyph infos */
  ਬਫਰ->len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &ਬਫਰ->info[ਬਫਰ->len++];

    info->codepoint = glyphs[i];
    info->cluster = vis_clusters[i];

    /* The rest is crap.  Let's store position info there for now. */
    info->ਨਕਾਬ = advances[i];
    info->var1.i32 = offsets[i].du;
    info->var2.i32 = offsets[i].dv;
  }

  /* Set glyph positions */
  ਬਫਰ->clear_positions ();
  ਦੁੱਗਣਾ x_mult = font_ਅੰਕੜੇ->x_mult, y_mult = font_ਅੰਕੜੇ->y_mult;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < glyphs_len; i++)
  {
    hb_glyph_info_t *info = &ਬਫਰ->info[i];
    hb_glyph_position_t *pos = &ਬਫਰ->pos[i];

    /* TODO vertical */
    pos->x_advance = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x_mult * (int32_t) info->ਨਕਾਬ);
    ਦੁੱਗਣਾ x_ਵਿੱਥ_2 = backward ? -(ਦੁੱਗਣਾ) info->var1.i32 : info->var1.i32;
    pos->x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x_mult * x_ਵਿੱਥ_2);
    pos->y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (y_mult * info->var2.i32);
  }

  ਜੇ (backward)
    hb_ਬਫਰ_ਉਲਟ (ਬਫਰ);

  ਬਫਰ->clear_glyph_flags ();
  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();

  /* Wow, done! */
  ਵਾਪਸ ਸੱਚ;
}


#ਜੇ_ਅੰਤ
