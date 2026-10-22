/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_HH
#تعریف_کرو HB_AAT_LAYOUT_HH

#شامل "hb.hh"

#شامل "hb-ot-shape.hh"
#شامل "hb-aat-ltag-table.hh"

/* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
#تعریف_کرو HB_CORETEXT_DEFAULT_FONT_سائز 12.f

ساخت hb_aat_feature_mapping_t
{
  hb_لیبل_t ot_Feature_لیبل;
  hb_aat_layout_feature_type_t aatFeatureType;
  hb_aat_layout_feature_selector_t selector_مقصد_Enable;
  hb_aat_layout_feature_selector_t selector_مقصد_Disable;

  صحیح cmp (hb_لیبل_t key) ثابت
  { واپس key < ot_Feature_لیبل ? -1 : key > ot_Feature_لیبل ? 1 : 0; }
};

HB_اندرونی ثابت hb_aat_feature_mapping_t *
hb_aat_layout_ڈھونڈو_feature_mapping (hb_لیبل_t لیبل_2);

HB_اندرونی خالی
hb_aat_layout_compile_map (ثابت hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map);

HB_اندرونی خالی
hb_aat_layout_substitute (ثابت hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_بفر_t *بفر,
			  ثابت hb_feature_t *features,
			  غیرعلامتی عدد_features);

HB_اندرونی خالی
hb_aat_layout_remove_deleted_glyphs (hb_بفر_t *بفر);

HB_اندرونی خالی
hb_aat_layout_position (ثابت hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_بفر_t *بفر);

HB_اندرونی خالی
hb_aat_layout_track (ثابت hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_بفر_t *بفر);


#اگر_ختم /* HB_AAT_LAYOUT_HH */
