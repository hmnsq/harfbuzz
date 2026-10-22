/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_META

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-meta-table.hh"

/**
 * SECTION:hb-ot-meta
 * @title: hb-ot-meta
 * @short_description: OpenType Metadata
 * @include: hb-ot.h
 *
 * Functions for fetching metadata from fonts.
 **/

/**
 * hb_ot_meta_get_entry_tags:
 * @face: a face object
 * @start_offset: iteration's start offset
 * @entries_count:(inout) (optional): buffer size as input, filled size as output
 * @entries: (out caller-allocates) (array length=entries_count) (nullable): entries tags buffer
 *
 * Fetches all available feature types.
 *
 * Return value: Number of all available feature types.
 *
 * Since: 2.6.0
 **/
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_ot_meta_get_ਇੰਦਰਾਜ_ਟੈਗ_ਸੂਚੀ (hb_face_t        *face,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ      ਸ਼ੁਰੂ_ਵਿੱਥ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ     *entries_ਗਿਣਤੀ, /* IN/OUT.  May be NULL. */
			   hb_ot_meta_ਟੈਗ_t *entries        /* OUT.     May be NULL. */)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.meta->get_entries (ਸ਼ੁਰੂ_ਵਿੱਥ, entries_ਗਿਣਤੀ, entries);
}

/**
 * hb_ot_meta_reference_entry:
 * @face: a #hb_face_t object.
 * @meta_tag: tag of metadata you like to have.
 *
 * It fetches metadata entry of a given tag from a font.
 *
 * Returns: (transfer full): A blob containing the blob.
 *
 * Since: 2.6.0
 **/
hb_blob_t *
hb_ot_meta_reference_ਇੰਦਰਾਜ (hb_face_t *face, hb_ot_meta_ਟੈਗ_t meta_ਟੈਗ)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.meta->reference_ਇੰਦਰਾਜ (meta_ਟੈਗ);
}

#ਜੇ_ਅੰਤ
