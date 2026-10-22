/*
 * Copyright © 2018  Google, Inc.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_NAME

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-name-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-utf.hh"


/**
 * SECTION:hb-ot-name
 * @title: hb-ot-name
 * @short_description: OpenType font name information
 * @include: hb-ot.h
 *
 * Functions for fetching name strings from OpenType fonts.
 **/


/**
 * hb_ot_name_list_names:
 * @face: font face.
 * @num_entries: (out) (optional): number of returned entries.
 *
 * Enumerates all available name IDs and language combinations. Returned
 * array is owned by the @face and should not be modified.  It can be
 * used as long as @face is alive.
 *
 * Returns: (transfer none) (array length=num_entries): Array of available name entries.
 * Since: 2.1.0
 **/
ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *
hb_ot_name_list_ਨਾਂ (hb_face_t    *face,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਅੰਕ_entries /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::name_accelerator_t &name = *face->ਸਾਰਣੀ.name;
  ਜੇ (ਅੰਕ_entries) *ਅੰਕ_entries = name.ਨਾਂ.ਲੰਮਾਈ;
  ਵਾਪਸ (ਅਚਲ hb_ot_name_ਇੰਦਰਾਜ_t *) name.ਨਾਂ;
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ utf_t>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_name_get_utf (hb_face_t       *face,
		    hb_ot_name_id_t  name_id,
		    hb_language_t    language,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਪਾਠ_ਮਾਪ /* IN/OUT */,
		    ਕਿਸਮ_ਨਾਂ utf_t::codepoint_t *ਪਾਠ /*ਬਾਹਰਲਾ*/)
{
  ਅਚਲ OT::name_accelerator_t &name = *face->ਸਾਰਣੀ.name;

  ਜੇ (!language)
    language = hb_language_from_string ("en", 2);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ;
  ਪੂਰਨ idx = name.get_ਸੂਚਕਾਂਕ (name_id, language, &ਚੌੜਾਈ);
  ਜੇ (idx != -1)
  {
    hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ = name.get_name (idx);

    ਜੇ (ਚੌੜਾਈ == 2) /* UTF16-BE */
      ਵਾਪਸ OT::hb_ot_name_convert_utf<hb_utf16_be_t, utf_t> (ਬਾਈਟਾਂ, ਪਾਠ_ਮਾਪ, ਪਾਠ);

    ਜੇ (ਚੌੜਾਈ == 1) /* ASCII */
      ਵਾਪਸ OT::hb_ot_name_convert_utf<hb_ascii_t, utf_t> (ਬਾਈਟਾਂ, ਪਾਠ_ਮਾਪ, ਪਾਠ);
  }

  ਜੇ (ਪਾਠ_ਮਾਪ)
  {
    ਜੇ (*ਪਾਠ_ਮਾਪ)
      *ਪਾਠ = 0;
    *ਪਾਠ_ਮਾਪ = 0;
  }
  ਵਾਪਸ 0;
}

/**
 * hb_ot_name_get_utf8:
 * @face: font face.
 * @name_id: OpenType name identifier to fetch.
 * @language: language to fetch the name for.
 * @text_size: (inout) (optional): input size of @text buffer, and output size of
 *                                   text written to buffer.
 * @text: (out caller-allocates) (array length=text_size): buffer to write fetched name into.
 *
 * Fetches a font name from the OpenType 'name' table.
 * If @language is #HB_LANGUAGE_INVALID, English ("en") is assumed.
 * Returns string in UTF-8 encoding. A NUL terminator is always written
 * for convenience, and isn't included in the output @text_size.
 *
 * Returns: full length of the requested string, or 0 if not found.
 * Since: 2.1.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_name_get_utf8 (hb_face_t       *face,
		     hb_ot_name_id_t  name_id,
		     hb_language_t    language,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਪਾਠ_ਮਾਪ /* IN/OUT */,
		     ਅੱਖਰ            *ਪਾਠ      /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_name_get_utf<hb_utf8_t> (face, name_id, language, ਪਾਠ_ਮਾਪ,
					(hb_utf8_t::codepoint_t *) ਪਾਠ);
}

/**
 * hb_ot_name_get_utf16:
 * @face: font face.
 * @name_id: OpenType name identifier to fetch.
 * @language: language to fetch the name for.
 * @text_size: (inout) (optional): input size of @text buffer, and output size of
 *                                   text written to buffer.
 * @text: (out caller-allocates) (array length=text_size): buffer to write fetched name into.
 *
 * Fetches a font name from the OpenType 'name' table.
 * If @language is #HB_LANGUAGE_INVALID, English ("en") is assumed.
 * Returns string in UTF-16 encoding. A NUL terminator is always written
 * for convenience, and isn't included in the output @text_size.
 *
 * Returns: full length of the requested string, or 0 if not found.
 * Since: 2.1.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_name_get_utf16 (hb_face_t       *face,
		      hb_ot_name_id_t  name_id,
		      hb_language_t    language,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਪਾਠ_ਮਾਪ /* IN/OUT */,
		      uint16_t        *ਪਾਠ      /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_name_get_utf<hb_utf16_t> (face, name_id, language, ਪਾਠ_ਮਾਪ, ਪਾਠ);
}

/**
 * hb_ot_name_get_utf32:
 * @face: font face.
 * @name_id: OpenType name identifier to fetch.
 * @language: language to fetch the name for.
 * @text_size: (inout) (optional): input size of @text buffer, and output size of
 *                                   text written to buffer.
 * @text: (out caller-allocates) (array length=text_size): buffer to write fetched name into.
 *
 * Fetches a font name from the OpenType 'name' table.
 * If @language is #HB_LANGUAGE_INVALID, English ("en") is assumed.
 * Returns string in UTF-32 encoding. A NUL terminator is always written
 * for convenience, and isn't included in the output @text_size.
 *
 * Returns: full length of the requested string, or 0 if not found.
 * Since: 2.1.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_name_get_utf32 (hb_face_t       *face,
		      hb_ot_name_id_t  name_id,
		      hb_language_t    language,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ    *ਪਾਠ_ਮਾਪ /* IN/OUT */,
		      uint32_t        *ਪਾਠ      /*ਬਾਹਰਲਾ*/)
{
  ਵਾਪਸ hb_ot_name_get_utf<hb_utf32_t> (face, name_id, language, ਪਾਠ_ਮਾਪ, ਪਾਠ);
}

#ਜੇ_ਅੰਤ
