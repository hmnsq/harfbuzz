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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_FETCH

#అన్తర్భావ "hb-ot.h"

#అన్తర్భావ "hb-ot-head-table.hh"
#అన్తర్భావ "hb-ot-os2-table.hh"
#అన్తర్భావ "hb-ot-post-table.hh"

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
hb_ot_fetch_బిట్లు (hb_face_t        *face,
		hb_ot_బిట్లు_ట్యాగ్_t  ట్యాగ్_2)
{
  ఎంపిక ((గుర్తులేని) ట్యాగ్_2)
  {
  స్థితి HB_OT_బిట్లు_ట్యాగ్_FS_TYPE:		 తిరిగి face->పట్టిక.OS2->fsType;
  స్థితి HB_OT_బిట్లు_ట్యాగ్_FS_SELECTION:	 తిరిగి face->పట్టిక.OS2->fsSelection;
  స్థితి HB_OT_బిట్లు_ట్యాగ్_MAC_STYLE:	 తిరిగి face->పట్టిక.తల_2->get_mac_style ();
  స్థితి HB_OT_బిట్లు_ట్యాగ్_IS_FIXED_PITCH:	 తిరిగి face->పట్టిక.post->పట్టిక->isFixedPitch;
  స్థితి HB_OT_బిట్లు_ట్యాగ్_UNICODE_RANGE_1:	 తిరిగి face->పట్టిక.OS2->ulUnicodeRange[0];
  స్థితి HB_OT_బిట్లు_ట్యాగ్_UNICODE_RANGE_2:	 తిరిగి face->పట్టిక.OS2->ulUnicodeRange[1];
  స్థితి HB_OT_బిట్లు_ట్యాగ్_UNICODE_RANGE_3:	 తిరిగి face->పట్టిక.OS2->ulUnicodeRange[2];
  స్థితి HB_OT_బిట్లు_ట్యాగ్_UNICODE_RANGE_4:	 తిరిగి face->పట్టిక.OS2->ulUnicodeRange[3];
  స్థితి HB_OT_బిట్లు_ట్యాగ్_సంకేతం_పుట_RANGE_1: తిరిగి face->పట్టిక.OS2->v1 ().ul_సంకేతం_పుట_Range1;
  స్థితి HB_OT_బిట్లు_ట్యాగ్_సంకేతం_పుట_RANGE_2: తిరిగి face->పట్టిక.OS2->v1 ().ul_సంకేతం_పుట_Range2;
  అప్రమేయ:				 తిరిగి 0;
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
hb_ot_fetch_సంఖ్య (hb_face_t          *face,
		  hb_ot_సంఖ్య_ట్యాగ్_t  ట్యాగ్_2)
{
  ఎంపిక ((గుర్తులేని) ట్యాగ్_2)
  {
  స్థితి HB_OT_సంఖ్య_ట్యాగ్_FONT_X_MIN: తిరిగి face->పట్టిక.తల_2->xMin;
  స్థితి HB_OT_సంఖ్య_ట్యాగ్_FONT_Y_MIN: తిరిగి face->పట్టిక.తల_2->yMin;
  స్థితి HB_OT_సంఖ్య_ట్యాగ్_FONT_X_MAX: తిరిగి face->పట్టిక.తల_2->xMax;
  స్థితి HB_OT_సంఖ్య_ట్యాగ్_FONT_Y_MAX: తిరిగి face->పట్టిక.తల_2->yMax;
  అప్రమేయ:		       తిరిగి 0;
  }
}

#అయితే_ముగింపు
