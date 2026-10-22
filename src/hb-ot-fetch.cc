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

#اگر_تعریف_نشده HB_NO_OT_FETCH

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
hb_ot_fetch_بیتها (hb_face_t        *face,
		hb_ot_بیتها_برچسب_t  برچسب_2)
{
  بدل ((بیعلامت) برچسب_2)
  {
  حالة HB_OT_بیتها_برچسب_FS_TYPE:		 أرجع face->جدول.OS2->fsType;
  حالة HB_OT_بیتها_برچسب_FS_SELECTION:	 أرجع face->جدول.OS2->fsSelection;
  حالة HB_OT_بیتها_برچسب_MAC_STYLE:	 أرجع face->جدول.سر_2->get_mac_style ();
  حالة HB_OT_بیتها_برچسب_IS_FIXED_PITCH:	 أرجع face->جدول.post->جدول->isFixedPitch;
  حالة HB_OT_بیتها_برچسب_UNICODE_RANGE_1:	 أرجع face->جدول.OS2->ulUnicodeRange[0];
  حالة HB_OT_بیتها_برچسب_UNICODE_RANGE_2:	 أرجع face->جدول.OS2->ulUnicodeRange[1];
  حالة HB_OT_بیتها_برچسب_UNICODE_RANGE_3:	 أرجع face->جدول.OS2->ulUnicodeRange[2];
  حالة HB_OT_بیتها_برچسب_UNICODE_RANGE_4:	 أرجع face->جدول.OS2->ulUnicodeRange[3];
  حالة HB_OT_بیتها_برچسب_کد_صفحه_RANGE_1: أرجع face->جدول.OS2->v1 ().ul_کد_صفحه_Range1;
  حالة HB_OT_بیتها_برچسب_کد_صفحه_RANGE_2: أرجع face->جدول.OS2->v1 ().ul_کد_صفحه_Range2;
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
hb_ot_fetch_عدد (hb_face_t          *face,
		  hb_ot_عدد_برچسب_t  برچسب_2)
{
  بدل ((بیعلامت) برچسب_2)
  {
  حالة HB_OT_عدد_برچسب_FONT_X_MIN: أرجع face->جدول.سر_2->xMin;
  حالة HB_OT_عدد_برچسب_FONT_Y_MIN: أرجع face->جدول.سر_2->yMin;
  حالة HB_OT_عدد_برچسب_FONT_X_MAX: أرجع face->جدول.سر_2->xMax;
  حالة HB_OT_عدد_برچسب_FONT_Y_MAX: أرجع face->جدول.سر_2->yMax;
  افتراضي:		       أرجع 0;
  }
}

#پایان_اگر
