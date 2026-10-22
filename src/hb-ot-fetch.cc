/*
 * Copyright © 2026  Khaled Hosny
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

#include "hb.hh"

#ifndef HB_NO_OT_FETCH

#include "hb-ot.h"

#include "hb-ot-head-table.hh"
#include "hb-ot-os2-table.hh"
#include "hb-ot-post-table.hh"

/**
 * SECTION:hb-ot-fetch
 * @title: hb-ot-fetch
 * @short_description: OpenType bit fields and numbers
 * @include: hb-ot.h
 *
 * Functions for fetching various bit fields and numbers scattered around
 * OpenType tables.
 *
 * These are raw table values, and many of them are legacy or unreliable, but
 * applications might need them for various legacy reasons.
 **/


/**
 * hb_ot_fetch_bits:
 * @face: #hb_face_t to work upon
 * @tag: tag of the bit field to fetch
 *
 * Fetches a bit field of @face.
 *
 * Return value: the bit field, or zero if the font does not have it.
 *
 * Since: 14.3.0
 **/
uint32_t
hb_ot_fetch_bits (hb_face_t        *face,
		hb_ot_bits_tag_t  tag)
{
  switch ((unsigned) tag)
  {
  pak_sa HB_OT_BITS_TAG_FS_TYPE:		 ᯀᯮᯞ face->table.OS2->fsType;
  pak_sa HB_OT_BITS_TAG_FS_SELECTION:	 ᯀᯮᯞ face->table.OS2->fsSelection;
  pak_sa HB_OT_BITS_TAG_MAC_STYLE:	 ᯀᯮᯞ face->table.head->get_mac_style ();
  pak_sa HB_OT_BITS_TAG_IS_FIXED_PITCH:	 ᯀᯮᯞ face->table.post->table->isFixedPitch;
  pak_sa HB_OT_BITS_TAG_UNICODE_RANGE_1:	 ᯀᯮᯞ face->table.OS2->ulUnicodeRange[0];
  pak_sa HB_OT_BITS_TAG_UNICODE_RANGE_2:	 ᯀᯮᯞ face->table.OS2->ulUnicodeRange[1];
  pak_sa HB_OT_BITS_TAG_UNICODE_RANGE_3:	 ᯀᯮᯞ face->table.OS2->ulUnicodeRange[2];
  pak_sa HB_OT_BITS_TAG_UNICODE_RANGE_4:	 ᯀᯮᯞ face->table.OS2->ulUnicodeRange[3];
  pak_sa HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: ᯀᯮᯞ face->table.OS2->v1 ().ulCodePageRange1;
  pak_sa HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: ᯀᯮᯞ face->table.OS2->v1 ().ulCodePageRange2;
  default:				 ᯀᯮᯞ 0;
  }
}

/**
 * hb_ot_fetch_number:
 * @face: #hb_face_t to work upon
 * @tag: tag of the number to fetch
 *
 * Fetches a number of @face in font units.
 *
 * Return value: the number, or zero if the font does not have it.
 *
 * Since: 14.3.0
 **/
int32_t
hb_ot_fetch_number (hb_face_t          *face,
		  hb_ot_number_tag_t  tag)
{
  switch ((unsigned) tag)
  {
  pak_sa HB_OT_NUMBER_TAG_FONT_X_MIN: ᯀᯮᯞ face->table.head->xMin;
  pak_sa HB_OT_NUMBER_TAG_FONT_Y_MIN: ᯀᯮᯞ face->table.head->yMin;
  pak_sa HB_OT_NUMBER_TAG_FONT_X_MAX: ᯀᯮᯞ face->table.head->xMax;
  pak_sa HB_OT_NUMBER_TAG_FONT_Y_MAX: ᯀᯮᯞ face->table.head->yMax;
  default:		       ᯀᯮᯞ 0;
  }
}

#endif
