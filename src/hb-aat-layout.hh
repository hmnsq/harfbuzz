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

#अगर_अघोषित HB_AAT_LAYOUT_HH
#घोषणा HB_AAT_LAYOUT_HH

#समावेश "hb.hh"

#समावेश "hb-ot-shape.hh"
#समावेश "hb-aat-ltag-table.hh"

/* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
#घोषणा HB_CORETEXT_DEFAULT_FONT_आकार 12.f

काष्ठा hb_aat_feature_mapping_t
{
  hb_tag_t otFeatureTag;
  hb_aat_layout_feature_type_t aatFeatureType;
  hb_aat_layout_feature_selector_t selectorToEnable;
  hb_aat_layout_feature_selector_t selectorToDisable;

  पूर्णांक cmp (hb_tag_t key) स्थिर
  { वापस key < otFeatureTag ? -1 : key > otFeatureTag ? 1 : 0; }
};

HB_INTERNAL स्थिर hb_aat_feature_mapping_t *
hb_aat_layout_find_feature_mapping (hb_tag_t tag);

HB_INTERNAL व्योम
hb_aat_layout_compile_map (स्थिर hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map);

HB_INTERNAL व्योम
hb_aat_layout_substitute (स्थिर hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_बफर_t *बफर,
			  स्थिर hb_feature_t *features,
			  अचिन्हित अंक_features);

HB_INTERNAL व्योम
hb_aat_layout_remove_deleted_glyphs (hb_बफर_t *बफर);

HB_INTERNAL व्योम
hb_aat_layout_position (स्थिर hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_बफर_t *बफर);

HB_INTERNAL व्योम
hb_aat_layout_track (स्थिर hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_बफर_t *बफर);


#पूर्ण_अगर /* HB_AAT_LAYOUT_HH */
