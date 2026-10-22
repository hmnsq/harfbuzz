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

#ضمن "hb.hh"

#ifndef HB_NO_OT_FETCH

#ضمن "hb-ot.h"

#ضمن "hb-ot-head-table.hh"
#ضمن "hb-ot-os2-table.hh"
#ضمن "hb-ot-post-table.hh"

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
  بدل ((unsigned) tag)
  {
  حالة HB_OT_BITS_TAG_FS_TYPE:		 أرجع face->table.OS2->fsType;
  حالة HB_OT_BITS_TAG_FS_SELECTION:	 أرجع face->table.OS2->fsSelection;
  حالة HB_OT_BITS_TAG_MAC_STYLE:	 أرجع face->table.head->get_mac_style ();
  حالة HB_OT_BITS_TAG_IS_FIXED_PITCH:	 أرجع face->table.post->table->isFixedPitch;
  حالة HB_OT_BITS_TAG_UNICODE_RANGE_1:	 أرجع face->table.OS2->ulUnicodeRange[0];
  حالة HB_OT_BITS_TAG_UNICODE_RANGE_2:	 أرجع face->table.OS2->ulUnicodeRange[1];
  حالة HB_OT_BITS_TAG_UNICODE_RANGE_3:	 أرجع face->table.OS2->ulUnicodeRange[2];
  حالة HB_OT_BITS_TAG_UNICODE_RANGE_4:	 أرجع face->table.OS2->ulUnicodeRange[3];
  حالة HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: أرجع face->table.OS2->v1 ().ulCodePageRange1;
  حالة HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: أرجع face->table.OS2->v1 ().ulCodePageRange2;
  افتراضي:				 أرجع 0;
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
hb_ot_fetch_نمبر (hb_face_t          *face,
		  hb_ot_نمبر_tag_t  tag)
{
  بدل ((unsigned) tag)
  {
  حالة HB_OT_نمبر_TAG_FONT_X_MIN: أرجع face->table.head->xMin;
  حالة HB_OT_نمبر_TAG_FONT_Y_MIN: أرجع face->table.head->yMin;
  حالة HB_OT_نمبر_TAG_FONT_X_MAX: أرجع face->table.head->xMax;
  حالة HB_OT_نمبر_TAG_FONT_Y_MAX: أرجع face->table.head->yMax;
  افتراضي:		       أرجع 0;
  }
}

#endif
