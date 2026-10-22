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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_LAYOUT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_LAYOUT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-shape.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-ltag-table.hh"

/* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CORETEXT_DEFAULT_FONT_ਮਾਪ 12.f

ਬਣਤਰ hb_aat_feature_mapping_t
{
  hb_ਟੈਗ_t ot_Feature_ਟੈਗ;
  hb_aat_layout_feature_type_t aatFeatureType;
  hb_aat_layout_feature_selector_t selector_ਮੰਜ਼ਿਲ_Enable;
  hb_aat_layout_feature_selector_t selector_ਮੰਜ਼ਿਲ_Disable;

  ਪੂਰਨ cmp (hb_ਟੈਗ_t key) ਅਚਲ
  { ਵਾਪਸ key < ot_Feature_ਟੈਗ ? -1 : key > ot_Feature_ਟੈਗ ? 1 : 0; }
};

HB_ਅੰਦਰੂਨੀ ਅਚਲ hb_aat_feature_mapping_t *
hb_aat_layout_ਲੱਭੋ_feature_mapping (hb_ਟੈਗ_t ਟੈਗ_2);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_aat_layout_compile_map (ਅਚਲ hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_aat_layout_substitute (ਅਚਲ hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_ਬਫਰ_t *ਬਫਰ,
			  ਅਚਲ hb_feature_t *features,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_aat_layout_remove_deleted_glyphs (hb_ਬਫਰ_t *ਬਫਰ);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_aat_layout_position (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_ਬਫਰ_t *ਬਫਰ);

HB_ਅੰਦਰੂਨੀ ਖਾਲੀ
hb_aat_layout_track (ਅਚਲ hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_ਬਫਰ_t *ਬਫਰ);


#ਜੇ_ਅੰਤ /* HB_AAT_LAYOUT_HH */
