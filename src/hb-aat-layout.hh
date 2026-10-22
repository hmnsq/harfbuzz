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

#ifndef HB_AAT_LAYOUT_HH
#define HB_AAT_LAYOUT_HH

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-ot-shape.hh"
#അന്തര്ഭാവ "hb-aat-ltag-table.hh"

/* https://developer.apple.com/documentation/coretext/1508745-ctfontcreatewithgraphicsfont */
#define HB_CORETEXT_DEFAULT_FONT_SIZE 12.f

ഘടന hb_aat_feature_mapping_t
{
  hb_tag_t otFeatureTag;
  hb_aat_layout_feature_type_t aatFeatureType;
  hb_aat_layout_feature_selector_t selectorToEnable;
  hb_aat_layout_feature_selector_t selectorToDisable;

  പൂർണ്ണസംഖ്യ cmp (hb_tag_t key) സ്ഥിരം
  { തിരികെ key < otFeatureTag ? -1 : key > otFeatureTag ? 1 : 0; }
};

HB_INTERNAL സ്ഥിരം hb_aat_feature_mapping_t *
hb_aat_layout_find_feature_mapping (hb_tag_t tag);

HB_INTERNAL ശൂന്യം
hb_aat_layout_compile_map (സ്ഥിരം hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map);

HB_INTERNAL ശൂന്യം
hb_aat_layout_substitute (സ്ഥിരം hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_buffer_t *buffer,
			  സ്ഥിരം hb_feature_t *features,
			  unsigned അക്കം_features);

HB_INTERNAL ശൂന്യം
hb_aat_layout_remove_deleted_glyphs (hb_buffer_t *buffer);

HB_INTERNAL ശൂന്യം
hb_aat_layout_position (സ്ഥിരം hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_buffer_t *buffer);

HB_INTERNAL ശൂന്യം
hb_aat_layout_track (സ്ഥിരം hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_buffer_t *buffer);


#endif /* HB_AAT_LAYOUT_HH */
