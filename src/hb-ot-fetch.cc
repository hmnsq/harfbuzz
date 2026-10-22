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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_FETCH

#অন্তর্ভাব "hb-ot.h"

#অন্তর্ভাব "hb-ot-head-table.hh"
#অন্তর্ভাব "hb-ot-os2-table.hh"
#অন্তর্ভাব "hb-ot-post-table.hh"

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
  নির্বাচন ((unsigned) tag)
  {
  ক্ষেত্র HB_OT_BITS_TAG_FS_TYPE:		 ফেরত face->table.OS2->fsType;
  ক্ষেত্র HB_OT_BITS_TAG_FS_SELECTION:	 ফেরত face->table.OS2->fsSelection;
  ক্ষেত্র HB_OT_BITS_TAG_MAC_STYLE:	 ফেরত face->table.শীর্ষ_2->get_mac_style ();
  ক্ষেত্র HB_OT_BITS_TAG_IS_FIXED_PITCH:	 ফেরত face->table.post->table->isFixedPitch;
  ক্ষেত্র HB_OT_BITS_TAG_UNICODE_RANGE_1:	 ফেরত face->table.OS2->ulUnicodeRange[0];
  ক্ষেত্র HB_OT_BITS_TAG_UNICODE_RANGE_2:	 ফেরত face->table.OS2->ulUnicodeRange[1];
  ক্ষেত্র HB_OT_BITS_TAG_UNICODE_RANGE_3:	 ফেরত face->table.OS2->ulUnicodeRange[2];
  ক্ষেত্র HB_OT_BITS_TAG_UNICODE_RANGE_4:	 ফেরত face->table.OS2->ulUnicodeRange[3];
  ক্ষেত্র HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: ফেরত face->table.OS2->v1 ().ulCodePageRange1;
  ক্ষেত্র HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: ফেরত face->table.OS2->v1 ().ulCodePageRange2;
  পূর্বনির্ধারিত:				 ফেরত 0;
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
hb_ot_fetch_সংখ্যা (hb_face_t          *face,
		  hb_ot_সংখ্যা_tag_t  tag)
{
  নির্বাচন ((unsigned) tag)
  {
  ক্ষেত্র HB_OT_সংখ্যা_TAG_FONT_X_MIN: ফেরত face->table.শীর্ষ_2->xMin;
  ক্ষেত্র HB_OT_সংখ্যা_TAG_FONT_Y_MIN: ফেরত face->table.শীর্ষ_2->yMin;
  ক্ষেত্র HB_OT_সংখ্যা_TAG_FONT_X_MAX: ফেরত face->table.শীর্ষ_2->xMax;
  ক্ষেত্র HB_OT_সংখ্যা_TAG_FONT_Y_MAX: ফেরত face->table.শীর্ষ_2->yMax;
  পূর্বনির্ধারিত:		       ফেরত 0;
  }
}

#endif
