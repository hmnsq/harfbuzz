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

#إذا !defined(HB_OT_H_اندريون) && !defined(HB_NO_SINGLE_سرو_ERROR)
#غلطي "Include <hb-ot.h> instead."
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_OT_METRICS_H
#وصف_ڪريو HB_OT_METRICS_H

#ضمن "hb.h"
#ضمن "hb-ot-name.h"

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
تعريفنوع تعداد {
  HB_OT_METRICS_ٽيگ_HORIZONTAL_ASCENDER		= HB_ٽيگ ('h','a','s','c'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_DESCENDER	= HB_ٽيگ ('h','d','s','c'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_سٽ_GAP		= HB_ٽيگ ('h','l','g','p'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_ASCENT	= HB_ٽيگ ('h','c','l','a'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_CLIPPING_DESCENT	= HB_ٽيگ ('h','c','l','d'),
  HB_OT_METRICS_ٽيگ_VERTICAL_ASCENDER		= HB_ٽيگ ('v','a','s','c'),
  HB_OT_METRICS_ٽيگ_VERTICAL_DESCENDER		= HB_ٽيگ ('v','d','s','c'),
  HB_OT_METRICS_ٽيگ_VERTICAL_سٽ_GAP		= HB_ٽيگ ('v','l','g','p'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_RISE	= HB_ٽيگ ('h','c','r','s'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_لاڳيتو	= HB_ٽيگ ('h','c','r','n'),
  HB_OT_METRICS_ٽيگ_HORIZONTAL_CARET_وٿي	= HB_ٽيگ ('h','c','o','f'),
  HB_OT_METRICS_ٽيگ_VERTICAL_CARET_RISE		= HB_ٽيگ ('v','c','r','s'),
  HB_OT_METRICS_ٽيگ_VERTICAL_CARET_لاڳيتو		= HB_ٽيگ ('v','c','r','n'),
  HB_OT_METRICS_ٽيگ_VERTICAL_CARET_وٿي	= HB_ٽيگ ('v','c','o','f'),
  HB_OT_METRICS_ٽيگ_X_اوچائي			= HB_ٽيگ ('x','h','g','t'),
  HB_OT_METRICS_ٽيگ_CAP_اوچائي			= HB_ٽيگ ('c','p','h','t'),
  HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_سائيز		= HB_ٽيگ ('s','b','x','s'),
  HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_سائيز		= HB_ٽيگ ('s','b','y','s'),
  HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_X_وٿي	= HB_ٽيگ ('s','b','x','o'),
  HB_OT_METRICS_ٽيگ_SUBSCRIPT_EM_Y_وٿي	= HB_ٽيگ ('s','b','y','o'),
  HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_سائيز	= HB_ٽيگ ('s','p','x','s'),
  HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_سائيز	= HB_ٽيگ ('s','p','y','s'),
  HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_X_وٿي	= HB_ٽيگ ('s','p','x','o'),
  HB_OT_METRICS_ٽيگ_SUPERSCRIPT_EM_Y_وٿي	= HB_ٽيگ ('s','p','y','o'),
  HB_OT_METRICS_ٽيگ_STRIKEOUT_سائيز		= HB_ٽيگ ('s','t','r','s'),
  HB_OT_METRICS_ٽيگ_STRIKEOUT_وٿي		= HB_ٽيگ ('s','t','r','o'),
  HB_OT_METRICS_ٽيگ_UNDERLINE_سائيز		= HB_ٽيگ ('u','n','d','s'),
  HB_OT_METRICS_ٽيگ_UNDERLINE_وٿي		= HB_ٽيگ ('u','n','d','o'),

  /*< private >*/
  HB_OT_METRICS_ٽيگ_MAX_قدر = HB_ٽيگ_MAX_SIGNED /*< skip >*/
} hb_ot_metrics_ٽيگ_t;

HB_EXTERN hb_bool_t
hb_ot_metrics_get_position (hb_font_t           *font,
			    hb_ot_metrics_ٽيگ_t  metrics_ٽيگ,
			    hb_position_t       *position     /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_ٽيگ_t  metrics_ٽيگ,
					  hb_position_t       *position     /*ٻاهريون*/);

HB_EXTERN عائم
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ);

HB_EXTERN hb_position_t
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ);

HB_EXTERN hb_position_t
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ٽيگ_t metrics_ٽيگ);

HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_OT_METRICS_H */
