/*
 * Copyright © 2018  Ebrahim Byagowi
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

#నిర్వచితం_కాకపోతే HB_OT_METRICS_H
#నిర్వచించు HB_OT_METRICS_H

#అన్తర్భావ "hb.h"
#అన్తర్భావ "hb-ot-name.h"

HB_BEGIN_DECLS


/**
 * hb_ot_metrics_tag_t:
 * @HB_OT_METRICS_TAG_HORIZONTAL_ASCENDER: horizontal ascender.
 * @HB_OT_METRICS_TAG_HORIZONTAL_DESCENDER: horizontal descender.
 * @HB_OT_METRICS_TAG_HORIZONTAL_LINE_GAP: horizontal line gap.
 * @HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_ASCENT: horizontal clipping ascent.
 * @HB_OT_METRICS_TAG_HORIZONTAL_CLIPPING_DESCENT: horizontal clipping descent.
 * @HB_OT_METRICS_TAG_VERTICAL_ASCENDER: vertical ascender.
 * @HB_OT_METRICS_TAG_VERTICAL_DESCENDER: vertical descender.
 * @HB_OT_METRICS_TAG_VERTICAL_LINE_GAP: vertical line gap.
 * @HB_OT_METRICS_TAG_HORIZONTAL_CARET_RISE: horizontal caret rise.
 * @HB_OT_METRICS_TAG_HORIZONTAL_CARET_RUN: horizontal caret run.
 * @HB_OT_METRICS_TAG_HORIZONTAL_CARET_OFFSET: horizontal caret offset.
 * @HB_OT_METRICS_TAG_VERTICAL_CARET_RISE: vertical caret rise.
 * @HB_OT_METRICS_TAG_VERTICAL_CARET_RUN: vertical caret run.
 * @HB_OT_METRICS_TAG_VERTICAL_CARET_OFFSET: vertical caret offset.
 * @HB_OT_METRICS_TAG_X_HEIGHT: x height.
 * @HB_OT_METRICS_TAG_CAP_HEIGHT: cap height.
 * @HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_SIZE: subscript em x size.
 * @HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_SIZE: subscript em y size.
 * @HB_OT_METRICS_TAG_SUBSCRIPT_EM_X_OFFSET: subscript em x offset.
 * @HB_OT_METRICS_TAG_SUBSCRIPT_EM_Y_OFFSET: subscript em y offset.
 * @HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_SIZE: superscript em x size.
 * @HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_SIZE: superscript em y size.
 * @HB_OT_METRICS_TAG_SUPERSCRIPT_EM_X_OFFSET: superscript em x offset.
 * @HB_OT_METRICS_TAG_SUPERSCRIPT_EM_Y_OFFSET: superscript em y offset.
 * @HB_OT_METRICS_TAG_STRIKEOUT_SIZE: strikeout size.
 * @HB_OT_METRICS_TAG_STRIKEOUT_OFFSET: strikeout offset.
 * @HB_OT_METRICS_TAG_UNDERLINE_SIZE: underline size.
 * @HB_OT_METRICS_TAG_UNDERLINE_OFFSET: underline offset.
 *
 * Metric tags corresponding to [MVAR Value
 * Tags](https://docs.microsoft.com/en-us/typography/opentype/spec/mvar#value-tags)
 *
 * Since: 2.6.0
 **/
రకనిర్వచనం గణన {
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_ASCENDER		= HB_ట్యాగ్ ('h','a','s','c'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_DESCENDER	= HB_ట్యాగ్ ('h','d','s','c'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_పంక్తి_GAP		= HB_ట్యాగ్ ('h','l','g','p'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_ASCENT	= HB_ట్యాగ్ ('h','c','l','a'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CLIPPING_DESCENT	= HB_ట్యాగ్ ('h','c','l','d'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_ASCENDER		= HB_ట్యాగ్ ('v','a','s','c'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_DESCENDER		= HB_ట్యాగ్ ('v','d','s','c'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_పంక్తి_GAP		= HB_ట్యాగ్ ('v','l','g','p'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_RISE	= HB_ట్యాగ్ ('h','c','r','s'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_నిరంతరం	= HB_ట్యాగ్ ('h','c','r','n'),
  HB_OT_METRICS_ట్యాగ్_HORIZONTAL_CARET_స్థానభ్రంశం	= HB_ట్యాగ్ ('h','c','o','f'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_RISE		= HB_ట్యాగ్ ('v','c','r','s'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_నిరంతరం		= HB_ట్యాగ్ ('v','c','r','n'),
  HB_OT_METRICS_ట్యాగ్_VERTICAL_CARET_స్థానభ్రంశం	= HB_ట్యాగ్ ('v','c','o','f'),
  HB_OT_METRICS_ట్యాగ్_X_ఎత్తు			= HB_ట్యాగ్ ('x','h','g','t'),
  HB_OT_METRICS_ట్యాగ్_CAP_ఎత్తు			= HB_ట్యాగ్ ('c','p','h','t'),
  HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_కొలత		= HB_ట్యాగ్ ('s','b','x','s'),
  HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_కొలత		= HB_ట్యాగ్ ('s','b','y','s'),
  HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_X_స్థానభ్రంశం	= HB_ట్యాగ్ ('s','b','x','o'),
  HB_OT_METRICS_ట్యాగ్_SUBSCRIPT_EM_Y_స్థానభ్రంశం	= HB_ట్యాగ్ ('s','b','y','o'),
  HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_కొలత	= HB_ట్యాగ్ ('s','p','x','s'),
  HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_కొలత	= HB_ట్యాగ్ ('s','p','y','s'),
  HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_X_స్థానభ్రంశం	= HB_ట్యాగ్ ('s','p','x','o'),
  HB_OT_METRICS_ట్యాగ్_SUPERSCRIPT_EM_Y_స్థానభ్రంశం	= HB_ట్యాగ్ ('s','p','y','o'),
  HB_OT_METRICS_ట్యాగ్_STRIKEOUT_కొలత		= HB_ట్యాగ్ ('s','t','r','s'),
  HB_OT_METRICS_ట్యాగ్_STRIKEOUT_స్థానభ్రంశం		= HB_ట్యాగ్ ('s','t','r','o'),
  HB_OT_METRICS_ట్యాగ్_UNDERLINE_కొలత		= HB_ట్యాగ్ ('u','n','d','s'),
  HB_OT_METRICS_ట్యాగ్_UNDERLINE_స్థానభ్రంశం		= HB_ట్యాగ్ ('u','n','d','o'),

  /*< private >*/
  HB_OT_METRICS_ట్యాగ్_MAX_విలువ = HB_ట్యాగ్_MAX_SIGNED /*< skip >*/
} hb_ot_metrics_ట్యాగ్_t;

HB_EXTERN hb_bool_t
hb_ot_metrics_get_position (hb_font_t           *font,
			    hb_ot_metrics_ట్యాగ్_t  metrics_ట్యాగ్,
			    hb_position_t       *position     /* OUT.  May be NULL. */);

HB_EXTERN శూన్య
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_ట్యాగ్_t  metrics_ట్యాగ్,
					  hb_position_t       *position     /*బహిర్గతం*/);

HB_EXTERN దశాంశం
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్);

HB_EXTERN hb_position_t
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్);

HB_EXTERN hb_position_t
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ట్యాగ్_t metrics_ట్యాగ్);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_OT_METRICS_H */
