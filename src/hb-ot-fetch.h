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

#إذا !defined(HB_OT_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#جے_انت

#جے_بنا_تعریف HB_OT_FETCH_H
#تعریف_کرو HB_OT_FETCH_H

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
  HB_OT_بٹ_ٹیگ_FS_TYPE		= HB_ٹیگ ('f','s','t','p'),
  HB_OT_بٹ_ٹیگ_FS_SELECTION		= HB_ٹیگ ('f','s','s','l'),
  HB_OT_بٹ_ٹیگ_MAC_STYLE		= HB_ٹیگ ('m','c','s','t'),
  HB_OT_بٹ_ٹیگ_IS_FIXED_PITCH		= HB_ٹیگ ('f','x','p','t'),
  HB_OT_بٹ_ٹیگ_UNICODE_RANGE_1	= HB_ٹیگ ('u','r','n','1'),
  HB_OT_بٹ_ٹیگ_UNICODE_RANGE_2	= HB_ٹیگ ('u','r','n','2'),
  HB_OT_بٹ_ٹیگ_UNICODE_RANGE_3	= HB_ٹیگ ('u','r','n','3'),
  HB_OT_بٹ_ٹیگ_UNICODE_RANGE_4	= HB_ٹیگ ('u','r','n','4'),
  HB_OT_بٹ_ٹیگ_کوڈ_ورقہ_RANGE_1	= HB_ٹیگ ('c','p','r','1'),
  HB_OT_بٹ_ٹیگ_کوڈ_ورقہ_RANGE_2	= HB_ٹیگ ('c','p','r','2'),

  /*< private >*/
  HB_OT_بٹ_ٹیگ_MAX_قدر = HB_ٹیگ_MAX_SIGNED /*< skip >*/
} hb_ot_بٹ_ٹیگ_t;


HB_EXTERN uint32_t
hb_ot_fetch_بٹ (hb_face_t        *face,
		hb_ot_بٹ_ٹیگ_t  ٹیگ_2);

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
  HB_OT_نمبر_ٹیگ_FONT_X_MIN		= HB_ٹیگ ('x','m','i','n'),
  HB_OT_نمبر_ٹیگ_FONT_Y_MIN		= HB_ٹیگ ('y','m','i','n'),
  HB_OT_نمبر_ٹیگ_FONT_X_MAX		= HB_ٹیگ ('x','m','a','x'),
  HB_OT_نمبر_ٹیگ_FONT_Y_MAX		= HB_ٹیگ ('y','m','a','x'),

  /*< private >*/
  HB_OT_نمبر_ٹیگ_MAX_قدر = HB_ٹیگ_MAX_SIGNED /*< skip >*/
} hb_ot_نمبر_ٹیگ_t;

HB_EXTERN int32_t
hb_ot_fetch_نمبر (hb_face_t          *face,
		  hb_ot_نمبر_ٹیگ_t  ٹیگ_2);

HB_سرا_DECLS

#جے_انت /* HB_OT_FETCH_H */
