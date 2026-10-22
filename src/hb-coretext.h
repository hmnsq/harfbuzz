/*
 * Copyright © 2012  Mozilla Foundation.
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
 *
 * Mozilla Author(s): Jonathan Kew
 */

#నిర్వచితం_కాకపోతే HB_CORETEXT_H
#నిర్వచించు HB_CORETEXT_H

#అన్తర్భావ "hb.h"

#అన్తర్భావ <TargetConditionals.h>
#ఒకవేళ లక్ష్యం_OS_IPHONE
#  include <CoreText/CoreText.h>
#  include <CoreGraphics/CoreGraphics.h>
#లేకపోతే
#  include <ApplicationServices/ApplicationServices.h>
#అయితే_ముగింపు

HB_BEGIN_DECLS


/**
 * HB_CORETEXT_TAG_MORT:
 *
 * The #hb_tag_t tag for the `mort` (glyph metamorphosis) table,
 * which holds AAT features.
 *
 * For more information, see
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6mort.html
 *
 **/
#నిర్వచించు HB_CORETEXT_ట్యాగ్_MORT HB_ట్యాగ్('m','o','r','t')

/**
 * HB_CORETEXT_TAG_MORX:
 *
 * The #hb_tag_t tag for the `morx` (extended glyph metamorphosis)
 * table, which holds AAT features.
 *
 * For more information, see
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6morx.html
 *
 **/
#నిర్వచించు HB_CORETEXT_ట్యాగ్_MORX HB_ట్యాగ్('m','o','r','x')

/**
 * HB_CORETEXT_TAG_KERX:
 *
 * The #hb_tag_t tag for the `kerx` (extended kerning) table, which
 * holds AAT kerning information.
 *
 * For more information, see
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6kerx.html
 *
 **/
#నిర్వచించు HB_CORETEXT_ట్యాగ్_KERX HB_ట్యాగ్('k','e','r','x')


HB_EXTERN hb_face_t *
hb_coretext_face_create (CGFontRef cg_font);

HB_EXTERN hb_face_t *
hb_coretext_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
					   గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN hb_face_t *
hb_coretext_face_create_from_blob_or_fail (hb_blob_t    *blob,
					   గుర్తులేని పూర్ణాంకం  సూచీ_2);

HB_EXTERN hb_font_t *
hb_coretext_font_create (CTFontRef ct_font);


HB_EXTERN CGFontRef
hb_coretext_face_get_cg_font (hb_face_t *face);

HB_EXTERN CTFontRef
hb_coretext_font_get_ct_font (hb_font_t *font);

HB_EXTERN శూన్య
hb_coretext_font_సమితి_funcs (hb_font_t *font);


HB_చివర_DECLS

#అయితే_ముగింపు /* HB_CORETEXT_H */
