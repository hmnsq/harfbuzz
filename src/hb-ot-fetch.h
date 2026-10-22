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

#إذا !defined(HB_OT_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb-ot.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_OT_FETCH_H
#وصف_ڪريو HB_OT_FETCH_H

#ضمن "hb.h"

HB_BEGIN_DECLS

/**
 * hb_ot_bits_tag_t:
 * @HB_OT_BITS_TAG_FS_TYPE: `fsType` of the `OS/2` table.
 * @HB_OT_BITS_TAG_FS_SELECTION: `fsSelection` of the `OS/2` table.
 * @HB_OT_BITS_TAG_MAC_STYLE: `macStyle` of the `head` table.
 * @HB_OT_BITS_TAG_IS_FIXED_PITCH: `isFixedPitch` of the `post` table.
 * @HB_OT_BITS_TAG_UNICODE_RANGE_1: `ulUnicodeRange1` of the `OS/2` table.
 * @HB_OT_BITS_TAG_UNICODE_RANGE_2: `ulUnicodeRange2` of the `OS/2` table.
 * @HB_OT_BITS_TAG_UNICODE_RANGE_3: `ulUnicodeRange3` of the `OS/2` table.
 * @HB_OT_BITS_TAG_UNICODE_RANGE_4: `ulUnicodeRange4` of the `OS/2` table.
 * @HB_OT_BITS_TAG_CODE_PAGE_RANGE_1: `ulCodePageRange1` of the `OS/2` table.
 * @HB_OT_BITS_TAG_CODE_PAGE_RANGE_2: `ulCodePageRange2` of the `OS/2` table.
 *
 * Bit fields that can be fetched with hb_ot_fetch_bits().
 *
 * Since: 14.3.0
 **/
تعريفنوع تعداد {
  HB_OT_بٽ_ٽيگ_FS_TYPE		= HB_ٽيگ ('f','s','t','p'),
  HB_OT_بٽ_ٽيگ_FS_SELECTION		= HB_ٽيگ ('f','s','s','l'),
  HB_OT_بٽ_ٽيگ_MAC_STYLE		= HB_ٽيگ ('m','c','s','t'),
  HB_OT_بٽ_ٽيگ_IS_FIXED_PITCH		= HB_ٽيگ ('f','x','p','t'),
  HB_OT_بٽ_ٽيگ_UNICODE_RANGE_1	= HB_ٽيگ ('u','r','n','1'),
  HB_OT_بٽ_ٽيگ_UNICODE_RANGE_2	= HB_ٽيگ ('u','r','n','2'),
  HB_OT_بٽ_ٽيگ_UNICODE_RANGE_3	= HB_ٽيگ ('u','r','n','3'),
  HB_OT_بٽ_ٽيگ_UNICODE_RANGE_4	= HB_ٽيگ ('u','r','n','4'),
  HB_OT_بٽ_ٽيگ_ڪوڊ_صفحو_RANGE_1	= HB_ٽيگ ('c','p','r','1'),
  HB_OT_بٽ_ٽيگ_ڪوڊ_صفحو_RANGE_2	= HB_ٽيگ ('c','p','r','2'),

  /*< private >*/
  HB_OT_بٽ_ٽيگ_MAX_قدر = HB_ٽيگ_MAX_SIGNED /*< skip >*/
} hb_ot_بٽ_ٽيگ_t;


HB_EXTERN uint32_t
hb_ot_fetch_بٽ (hb_face_t        *face,
		hb_ot_بٽ_ٽيگ_t  ٽيگ_2);

/**
 * hb_ot_number_tag_t:
 * @HB_OT_NUMBER_TAG_FONT_X_MIN: `xMin` of the `head` table.
 * @HB_OT_NUMBER_TAG_FONT_Y_MIN: `yMin` of the `head` table.
 * @HB_OT_NUMBER_TAG_FONT_X_MAX: `xMax` of the `head` table.
 * @HB_OT_NUMBER_TAG_FONT_Y_MAX: `yMax` of the `head` table.
 *
 * Numbers that can be fetched with hb_ot_fetch_number().
 *
 * Since: 14.3.0
 **/
تعريفنوع تعداد {
  HB_OT_عدد_ٽيگ_FONT_X_MIN		= HB_ٽيگ ('x','m','i','n'),
  HB_OT_عدد_ٽيگ_FONT_Y_MIN		= HB_ٽيگ ('y','m','i','n'),
  HB_OT_عدد_ٽيگ_FONT_X_MAX		= HB_ٽيگ ('x','m','a','x'),
  HB_OT_عدد_ٽيگ_FONT_Y_MAX		= HB_ٽيگ ('y','m','a','x'),

  /*< private >*/
  HB_OT_عدد_ٽيگ_MAX_قدر = HB_ٽيگ_MAX_SIGNED /*< skip >*/
} hb_ot_عدد_ٽيگ_t;

HB_EXTERN int32_t
hb_ot_fetch_عدد (hb_face_t          *face,
		  hb_ot_عدد_ٽيگ_t  ٽيگ_2);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_OT_FETCH_H */
