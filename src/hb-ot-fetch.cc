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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FETCH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-head-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-os2-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"

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
hb_ot_fetch_ਬਿੱਟ (hb_face_t        *face,
		hb_ot_ਬਿੱਟ_ਟੈਗ_t  ਟੈਗ_2)
{
  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਟੈਗ_2)
  {
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_FS_TYPE:		 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->fsType;
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_FS_SELECTION:	 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->fsSelection;
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_MAC_STYLE:	 ਵਾਪਸ face->ਸਾਰਣੀ.ਸਿਰ_2->get_mac_style ();
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_IS_FIXED_PITCH:	 ਵਾਪਸ face->ਸਾਰਣੀ.post->ਸਾਰਣੀ->isFixedPitch;
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_UNICODE_RANGE_1:	 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->ulUnicodeRange[0];
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_UNICODE_RANGE_2:	 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->ulUnicodeRange[1];
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_UNICODE_RANGE_3:	 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->ulUnicodeRange[2];
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_UNICODE_RANGE_4:	 ਵਾਪਸ face->ਸਾਰਣੀ.OS2->ulUnicodeRange[3];
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_ਕੋਡ_ਪੰਨਾ_RANGE_1: ਵਾਪਸ face->ਸਾਰਣੀ.OS2->v1 ().ul_ਕੋਡ_ਪੰਨਾ_Range1;
  ਹਾਲਤ HB_OT_ਬਿੱਟ_ਟੈਗ_ਕੋਡ_ਪੰਨਾ_RANGE_2: ਵਾਪਸ face->ਸਾਰਣੀ.OS2->v1 ().ul_ਕੋਡ_ਪੰਨਾ_Range2;
  ਮੂਲ:				 ਵਾਪਸ 0;
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
hb_ot_fetch_ਸੰਖਿਆ (hb_face_t          *face,
		  hb_ot_ਸੰਖਿਆ_ਟੈਗ_t  ਟੈਗ_2)
{
  ਚੋਣ ((ਬਿਨਾਨਿਸ਼ਾਨੀ) ਟੈਗ_2)
  {
  ਹਾਲਤ HB_OT_ਸੰਖਿਆ_ਟੈਗ_FONT_X_MIN: ਵਾਪਸ face->ਸਾਰਣੀ.ਸਿਰ_2->xMin;
  ਹਾਲਤ HB_OT_ਸੰਖਿਆ_ਟੈਗ_FONT_Y_MIN: ਵਾਪਸ face->ਸਾਰਣੀ.ਸਿਰ_2->yMin;
  ਹਾਲਤ HB_OT_ਸੰਖਿਆ_ਟੈਗ_FONT_X_MAX: ਵਾਪਸ face->ਸਾਰਣੀ.ਸਿਰ_2->xMax;
  ਹਾਲਤ HB_OT_ਸੰਖਿਆ_ਟੈਗ_FONT_Y_MAX: ਵਾਪਸ face->ਸਾਰਣੀ.ਸਿਰ_2->yMax;
  ਮੂਲ:		       ਵਾਪਸ 0;
  }
}

#ਜੇ_ਅੰਤ
