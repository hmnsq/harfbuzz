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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_FETCH

#समावेश "hb-ot.h"

#समावेश "hb-ot-head-table.hh"
#समावेश "hb-ot-os2-table.hh"
#समावेश "hb-ot-post-table.hh"

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
  चयन ((अचिन्हित) tag)
  {
  हाल HB_OT_BITS_TAG_FS_TYPE:		 वापस face->table.OS2->fsType;
  हाल HB_OT_BITS_TAG_FS_SELECTION:	 वापस face->table.OS2->fsSelection;
  हाल HB_OT_BITS_TAG_MAC_STYLE:	 वापस face->table.शीर्ष_2->get_mac_style ();
  हाल HB_OT_BITS_TAG_IS_FIXED_PITCH:	 वापस face->table.post->table->isFixedPitch;
  हाल HB_OT_BITS_TAG_UNICODE_RANGE_1:	 वापस face->table.OS2->ulUnicodeRange[0];
  हाल HB_OT_BITS_TAG_UNICODE_RANGE_2:	 वापस face->table.OS2->ulUnicodeRange[1];
  हाल HB_OT_BITS_TAG_UNICODE_RANGE_3:	 वापस face->table.OS2->ulUnicodeRange[2];
  हाल HB_OT_BITS_TAG_UNICODE_RANGE_4:	 वापस face->table.OS2->ulUnicodeRange[3];
  हाल HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: वापस face->table.OS2->v1 ().ulCodePageRange1;
  हाल HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: वापस face->table.OS2->v1 ().ulCodePageRange2;
  शेष:				 वापस 0;
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
hb_ot_fetch_संख्या (hb_face_t          *face,
		  hb_ot_संख्या_tag_t  tag)
{
  चयन ((अचिन्हित) tag)
  {
  हाल HB_OT_संख्या_TAG_FONT_X_MIN: वापस face->table.शीर्ष_2->xMin;
  हाल HB_OT_संख्या_TAG_FONT_Y_MIN: वापस face->table.शीर्ष_2->yMin;
  हाल HB_OT_संख्या_TAG_FONT_X_MAX: वापस face->table.शीर्ष_2->xMax;
  हाल HB_OT_संख्या_TAG_FONT_Y_MAX: वापस face->table.शीर्ष_2->yMax;
  शेष:		       वापस 0;
  }
}

#पूर्ण_अगर
