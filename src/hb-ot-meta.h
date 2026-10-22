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

#ఒకవేళ !defined(HB_OT_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb-ot.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_OT_META_H
#నిర్వచించు HB_OT_META_H

#అన్తర్భావ "hb.h"

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
రకనిర్వచనం గణన {
/*
   HB_OT_META_TAG_APPL		= HB_TAG ('a','p','p','l'),
   HB_OT_META_TAG_BILD		= HB_TAG ('b','i','l','d'),
*/
  HB_OT_META_ట్యాగ్_DESIGN_LANGUAGES	= HB_ట్యాగ్ ('d','l','n','g'),
  HB_OT_META_ట్యాగ్_SUPPORTED_LANGUAGES	= HB_ట్యాగ్ ('s','l','n','g'),

  /*< private >*/
  HB_OT_META_ట్యాగ్_MAX_విలువ = HB_ట్యాగ్_MAX_SIGNED /*< skip >*/
} hb_ot_meta_ట్యాగ్_t;

HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_ot_meta_get_నమోదు_ట్యాగులు (hb_face_t        *face,
			   గుర్తులేని పూర్ణాంకం      ప్రారంభం_స్థానభ్రంశం,
			   గుర్తులేని పూర్ణాంకం     *entries_లెక్క, /* IN/OUT.  May be NULL. */
			   hb_ot_meta_ట్యాగ్_t *entries        /* OUT.     May be NULL. */);

HB_EXTERN hb_blob_t *
hb_ot_meta_reference_నమోదు (hb_face_t *face, hb_ot_meta_ట్యాగ్_t meta_ట్యాగ్);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_META_H */
