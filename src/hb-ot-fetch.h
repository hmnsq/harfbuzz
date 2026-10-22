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

#إذا !defined(HB_OT_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb-ot.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_OT_FETCH_H
#تعریف_کړه HB_OT_FETCH_H

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
  HB_OT_بټونه_نښان_FS_TYPE		= HB_نښان ('f','s','t','p'),
  HB_OT_بټونه_نښان_FS_SELECTION		= HB_نښان ('f','s','s','l'),
  HB_OT_بټونه_نښان_MAC_STYLE		= HB_نښان ('m','c','s','t'),
  HB_OT_بټونه_نښان_IS_FIXED_PITCH		= HB_نښان ('f','x','p','t'),
  HB_OT_بټونه_نښان_UNICODE_RANGE_1	= HB_نښان ('u','r','n','1'),
  HB_OT_بټونه_نښان_UNICODE_RANGE_2	= HB_نښان ('u','r','n','2'),
  HB_OT_بټونه_نښان_UNICODE_RANGE_3	= HB_نښان ('u','r','n','3'),
  HB_OT_بټونه_نښان_UNICODE_RANGE_4	= HB_نښان ('u','r','n','4'),
  HB_OT_بټونه_نښان_کوډ_پاڼه_RANGE_1	= HB_نښان ('c','p','r','1'),
  HB_OT_بټونه_نښان_کوډ_پاڼه_RANGE_2	= HB_نښان ('c','p','r','2'),

  /*< private >*/
  HB_OT_بټونه_نښان_MAX_ارزښت = HB_نښان_MAX_SIGNED /*< skip >*/
} hb_ot_بټونه_نښان_t;


HB_EXTERN uint32_t
hb_ot_fetch_بټونه (hb_face_t        *face,
		hb_ot_بټونه_نښان_t  نښان_2);

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
  HB_OT_شمېره_نښان_FONT_X_MIN		= HB_نښان ('x','m','i','n'),
  HB_OT_شمېره_نښان_FONT_Y_MIN		= HB_نښان ('y','m','i','n'),
  HB_OT_شمېره_نښان_FONT_X_MAX		= HB_نښان ('x','m','a','x'),
  HB_OT_شمېره_نښان_FONT_Y_MAX		= HB_نښان ('y','m','a','x'),

  /*< private >*/
  HB_OT_شمېره_نښان_MAX_ارزښت = HB_نښان_MAX_SIGNED /*< skip >*/
} hb_ot_شمېره_نښان_t;

HB_EXTERN int32_t
hb_ot_fetch_شمېره (hb_face_t          *face,
		  hb_ot_شمېره_نښان_t  نښان_2);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_OT_FETCH_H */
