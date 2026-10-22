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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_FETCH

#ଅନ୍ତର୍ଭାଵ "hb-ot.h"

#ଅନ୍ତର୍ଭାଵ "hb-ot-head-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-os2-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-post-table.hh"

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
  ବାଛ ((unsigned) tag)
  {
  ସ୍ଥିତି HB_OT_BITS_TAG_FS_TYPE:		 ଫେରନ୍ତୁ face->table.OS2->fsType;
  ସ୍ଥିତି HB_OT_BITS_TAG_FS_SELECTION:	 ଫେରନ୍ତୁ face->table.OS2->fsSelection;
  ସ୍ଥିତି HB_OT_BITS_TAG_MAC_STYLE:	 ଫେରନ୍ତୁ face->table.head->get_mac_style ();
  ସ୍ଥିତି HB_OT_BITS_TAG_IS_FIXED_PITCH:	 ଫେରନ୍ତୁ face->table.post->table->isFixedPitch;
  ସ୍ଥିତି HB_OT_BITS_TAG_UNICODE_RANGE_1:	 ଫେରନ୍ତୁ face->table.OS2->ulUnicodeRange[0];
  ସ୍ଥିତି HB_OT_BITS_TAG_UNICODE_RANGE_2:	 ଫେରନ୍ତୁ face->table.OS2->ulUnicodeRange[1];
  ସ୍ଥିତି HB_OT_BITS_TAG_UNICODE_RANGE_3:	 ଫେରନ୍ତୁ face->table.OS2->ulUnicodeRange[2];
  ସ୍ଥିତି HB_OT_BITS_TAG_UNICODE_RANGE_4:	 ଫେରନ୍ତୁ face->table.OS2->ulUnicodeRange[3];
  ସ୍ଥିତି HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: ଫେରନ୍ତୁ face->table.OS2->v1 ().ulCodePageRange1;
  ସ୍ଥିତି HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: ଫେରନ୍ତୁ face->table.OS2->v1 ().ulCodePageRange2;
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:				 ଫେରନ୍ତୁ 0;
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
hb_ot_fetch_ସଂଖ୍ୟା (hb_face_t          *face,
		  hb_ot_ସଂଖ୍ୟା_tag_t  tag)
{
  ବାଛ ((unsigned) tag)
  {
  ସ୍ଥିତି HB_OT_ସଂଖ୍ୟା_TAG_FONT_X_MIN: ଫେରନ୍ତୁ face->table.head->xMin;
  ସ୍ଥିତି HB_OT_ସଂଖ୍ୟା_TAG_FONT_Y_MIN: ଫେରନ୍ତୁ face->table.head->yMin;
  ସ୍ଥିତି HB_OT_ସଂଖ୍ୟା_TAG_FONT_X_MAX: ଫେରନ୍ତୁ face->table.head->xMax;
  ସ୍ଥିତି HB_OT_ସଂଖ୍ୟା_TAG_FONT_Y_MAX: ଫେରନ୍ତୁ face->table.head->yMax;
  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:		       ଫେରନ୍ତୁ 0;
  }
}

#endif
