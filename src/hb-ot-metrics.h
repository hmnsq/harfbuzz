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

#إذا !defined(HB_OT_H_وچ) && !defined(HB_NO_SINGLE_سرخی_ERROR)
#غلطی "Include <hb-ot.h> instead."
#جے_انت

#جے_بنا_تعریف HB_OT_METRICS_H
#تعریف_کرو HB_OT_METRICS_H

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
  HB_OT_METRICS_ٹیگ_HORIZONTAL_ASCENDER		= HB_ٹیگ ('h','a','s','c'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_DESCENDER	= HB_ٹیگ ('h','d','s','c'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_سطر_GAP		= HB_ٹیگ ('h','l','g','p'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_CLIPPING_ASCENT	= HB_ٹیگ ('h','c','l','a'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_CLIPPING_DESCENT	= HB_ٹیگ ('h','c','l','d'),
  HB_OT_METRICS_ٹیگ_VERTICAL_ASCENDER		= HB_ٹیگ ('v','a','s','c'),
  HB_OT_METRICS_ٹیگ_VERTICAL_DESCENDER		= HB_ٹیگ ('v','d','s','c'),
  HB_OT_METRICS_ٹیگ_VERTICAL_سطر_GAP		= HB_ٹیگ ('v','l','g','p'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_RISE	= HB_ٹیگ ('h','c','r','s'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_لگاتار	= HB_ٹیگ ('h','c','r','n'),
  HB_OT_METRICS_ٹیگ_HORIZONTAL_CARET_وتھ	= HB_ٹیگ ('h','c','o','f'),
  HB_OT_METRICS_ٹیگ_VERTICAL_CARET_RISE		= HB_ٹیگ ('v','c','r','s'),
  HB_OT_METRICS_ٹیگ_VERTICAL_CARET_لگاتار		= HB_ٹیگ ('v','c','r','n'),
  HB_OT_METRICS_ٹیگ_VERTICAL_CARET_وتھ	= HB_ٹیگ ('v','c','o','f'),
  HB_OT_METRICS_ٹیگ_X_اچائی			= HB_ٹیگ ('x','h','g','t'),
  HB_OT_METRICS_ٹیگ_CAP_اچائی			= HB_ٹیگ ('c','p','h','t'),
  HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_X_ناپ		= HB_ٹیگ ('s','b','x','s'),
  HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_Y_ناپ		= HB_ٹیگ ('s','b','y','s'),
  HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_X_وتھ	= HB_ٹیگ ('s','b','x','o'),
  HB_OT_METRICS_ٹیگ_SUBSCRIPT_EM_Y_وتھ	= HB_ٹیگ ('s','b','y','o'),
  HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_X_ناپ	= HB_ٹیگ ('s','p','x','s'),
  HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_Y_ناپ	= HB_ٹیگ ('s','p','y','s'),
  HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_X_وتھ	= HB_ٹیگ ('s','p','x','o'),
  HB_OT_METRICS_ٹیگ_SUPERSCRIPT_EM_Y_وتھ	= HB_ٹیگ ('s','p','y','o'),
  HB_OT_METRICS_ٹیگ_STRIKEOUT_ناپ		= HB_ٹیگ ('s','t','r','s'),
  HB_OT_METRICS_ٹیگ_STRIKEOUT_وتھ		= HB_ٹیگ ('s','t','r','o'),
  HB_OT_METRICS_ٹیگ_UNDERLINE_ناپ		= HB_ٹیگ ('u','n','d','s'),
  HB_OT_METRICS_ٹیگ_UNDERLINE_وتھ		= HB_ٹیگ ('u','n','d','o'),

  /*< private >*/
  HB_OT_METRICS_ٹیگ_MAX_قدر = HB_ٹیگ_MAX_SIGNED /*< skip >*/
} hb_ot_metrics_ٹیگ_t;

HB_EXTERN hb_bool_t
hb_ot_metrics_get_position (hb_font_t           *font,
			    hb_ot_metrics_ٹیگ_t  metrics_ٹیگ,
			    hb_position_t       *position     /* OUT.  May be NULL. */);

HB_EXTERN فراغ
hb_ot_metrics_get_position_with_fallback (hb_font_t           *font,
					  hb_ot_metrics_ٹیگ_t  metrics_ٹیگ,
					  hb_position_t       *position     /*باہرلا*/);

HB_EXTERN عائم
hb_ot_metrics_get_variation (hb_font_t *font, hb_ot_metrics_ٹیگ_t metrics_ٹیگ);

HB_EXTERN hb_position_t
hb_ot_metrics_get_x_variation (hb_font_t *font, hb_ot_metrics_ٹیگ_t metrics_ٹیگ);

HB_EXTERN hb_position_t
hb_ot_metrics_get_y_variation (hb_font_t *font, hb_ot_metrics_ٹیگ_t metrics_ٹیگ);

HB_سرا_DECLS

#جے_انت /* HB_OT_METRICS_H */
