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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_FETCH

#شامل "hb-ot.h"

#شامل "hb-ot-head-table.hh"
#شامل "hb-ot-os2-table.hh"
#شامل "hb-ot-post-table.hh"

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
hb_ot_fetch_بٹ (hb_face_t        *face,
		hb_ot_بٹ_لیبل_t  لیبل_2)
{
  انتخاب ((غیرعلامتی) لیبل_2)
  {
  حالت HB_OT_بٹ_لیبل_FS_TYPE:		 واپس face->جدول.OS2->fsType;
  حالت HB_OT_بٹ_لیبل_FS_SELECTION:	 واپس face->جدول.OS2->fsSelection;
  حالت HB_OT_بٹ_لیبل_MAC_STYLE:	 واپس face->جدول.سر_2->get_mac_style ();
  حالت HB_OT_بٹ_لیبل_IS_FIXED_PITCH:	 واپس face->جدول.post->جدول->isFixedPitch;
  حالت HB_OT_بٹ_لیبل_UNICODE_RANGE_1:	 واپس face->جدول.OS2->ulUnicodeRange[0];
  حالت HB_OT_بٹ_لیبل_UNICODE_RANGE_2:	 واپس face->جدول.OS2->ulUnicodeRange[1];
  حالت HB_OT_بٹ_لیبل_UNICODE_RANGE_3:	 واپس face->جدول.OS2->ulUnicodeRange[2];
  حالت HB_OT_بٹ_لیبل_UNICODE_RANGE_4:	 واپس face->جدول.OS2->ulUnicodeRange[3];
  حالت HB_OT_بٹ_لیبل_رمز_صفحہ_RANGE_1: واپس face->جدول.OS2->v1 ().ul_رمز_صفحہ_Range1;
  حالت HB_OT_بٹ_لیبل_رمز_صفحہ_RANGE_2: واپس face->جدول.OS2->v1 ().ul_رمز_صفحہ_Range2;
  طےشدہ:				 واپس 0;
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
		  hb_ot_نمبر_لیبل_t  لیبل_2)
{
  انتخاب ((غیرعلامتی) لیبل_2)
  {
  حالت HB_OT_نمبر_لیبل_FONT_X_MIN: واپس face->جدول.سر_2->xMin;
  حالت HB_OT_نمبر_لیبل_FONT_Y_MIN: واپس face->جدول.سر_2->yMin;
  حالت HB_OT_نمبر_لیبل_FONT_X_MAX: واپس face->جدول.سر_2->xMax;
  حالت HB_OT_نمبر_لیبل_FONT_Y_MAX: واپس face->جدول.سر_2->yMax;
  طےشدہ:		       واپس 0;
  }
}

#اگر_ختم
