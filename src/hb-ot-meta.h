/*
 * Copyright © 2019  Ebrahim Byagowi
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

#اگر !defined(HB_OT_H_اندر) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_OT_META_H
#تعریف_کرو HB_OT_META_H

#شامل "hb.h"

HB_BEGIN_DECLS

/**
 * hb_ot_meta_tag_t:
 * @HB_OT_META_TAG_DESIGN_LANGUAGES: Design languages. Text, using only
 * Basic Latin (ASCII) characters. Indicates languages and/or scripts
 * for the user audiences that the font was primarily designed for.
 * @HB_OT_META_TAG_SUPPORTED_LANGUAGES: Supported languages. Text, using
 * only Basic Latin (ASCII) characters. Indicates languages and/or scripts
 * that the font is declared to be capable of supporting.
 *
 * Known metadata tags from https://docs.microsoft.com/en-us/typography/opentype/spec/meta
 *
 * Since: 2.6.0
 **/
قسمتعریف شمار {
/*
   HB_OT_META_TAG_APPL		= HB_TAG ('a','p','p','l'),
   HB_OT_META_TAG_BILD		= HB_TAG ('b','i','l','d'),
*/
  HB_OT_META_لیبل_DESIGN_LANGUAGES	= HB_لیبل ('d','l','n','g'),
  HB_OT_META_لیبل_SUPPORTED_LANGUAGES	= HB_لیبل ('s','l','n','g'),

  /*< private >*/
  HB_OT_META_لیبل_MAX_قدر = HB_لیبل_MAX_SIGNED /*< skip >*/
} hb_ot_meta_لیبل_t;

HB_EXTERN غیرعلامتی صحیح
hb_ot_meta_get_اندراج_لیبل_فہرست (hb_face_t        *face,
			   غیرعلامتی صحیح      آغاز_انحراف,
			   غیرعلامتی صحیح     *entries_گنتی, /* IN/OUT.  May be NULL. */
			   hb_ot_meta_لیبل_t *entries        /* OUT.     May be NULL. */);

HB_EXTERN hb_blob_t *
hb_ot_meta_reference_اندراج (hb_face_t *face, hb_ot_meta_لیبل_t meta_لیبل);

HB_انتہا_DECLS

#اگر_ختم /* HB_OT_META_H */
