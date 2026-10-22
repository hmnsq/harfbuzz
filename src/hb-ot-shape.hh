/*
 * Copyright © 2010  Google, Inc.
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

#ifndef HB_OT_SHAPE_HH
#define HB_OT_SHAPE_HH

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-ot-map.hh"
#അന്തര്ഭാവ "hb-aat-map.hh"


HB_INTERNAL തർക്കം
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   സ്ഥിരം പൂർണ്ണസംഖ്യ             *coords,
						   unsigned പൂർണ്ണസംഖ്യ           അക്കം_coords,
						   unsigned പൂർണ്ണസംഖ്യ          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL തർക്കം
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<സ്ഥിരം unsigned> conditions,
                                             hb_set_t *lookup_indexes);

ഘടന hb_ot_shape_plan_താക്കോൽ_t
{
  unsigned പൂർണ്ണസംഖ്യ variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  തർക്കം init (hb_face_t *face,
	     സ്ഥിരം പൂർണ്ണസംഖ്യ *coords,
	     unsigned   അക്കം_coords)
  {
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ table_index = 0; table_index < 2; table_index++)
      എങ്കിൽ (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			അക്കം_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	വേണ്ടി (auto &v : lookup_conditions)
	  v.fini ();
	തിരികെ അസത്യം;
      }
    തിരികെ സത്യം;
  }

  ശൂന്യം fini ()
  {
    വേണ്ടി (auto &v : lookup_conditions)
      v.fini ();
  }

  തർക്കം get_lookup_variations (hb_face_t *face,
                              unsigned പൂർണ്ണസംഖ്യ table_index,
			      unsigned പൂർണ്ണസംഖ്യ feature_index,
			      hb_set_t *lookups) സ്ഥിരം
  {
    തിരികെ _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  തർക്കം equal (സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t *other)
  {
    തിരികെ variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ഘടന hb_shape_plan_താക്കോൽ_t;

ഘടന hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  സ്ഥിരം ഘടന hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  സ്ഥിരം ശൂന്യം *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr hb_mask_t frac_mask = 0;
  സ്ഥിതം constexpr hb_mask_t numr_mask = 0;
  സ്ഥിതം constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  തർക്കം requested_kerning : 1;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr തർക്കം requested_kerning = അസത്യം;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  തർക്കം has_frac : 1;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr തർക്കം has_frac = അസത്യം;
#endif
  തർക്കം has_vert : 1;
  തർക്കം has_gpos_mark : 1;
  തർക്കം zero_marks : 1;
  തർക്കം fallback_glyph_classes : 1;
  തർക്കം fallback_mark_positioning : 1;
  തർക്കം adjust_mark_positioning_when_zeroing : 1;

  തർക്കം apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  തർക്കം apply_kern : 1;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr തർക്കം apply_kern = അസത്യം;
#endif
  തർക്കം apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  തർക്കം apply_kerx : 1;
  തർക്കം apply_morx : 1;
  തർക്കം apply_trak : 1;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr തർക്കം apply_kerx = അസത്യം;
  സ്ഥിതം constexpr തർക്കം apply_morx = അസത്യം;
  സ്ഥിതം constexpr തർക്കം apply_trak = അസത്യം;
#endif

  ശൂന്യം collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ table_index;
    തിരഞ്ഞെടുപ്പ് (table_tag) {
      അവസ്ഥ HB_OT_TAG_GSUB: table_index = 0; നിർത്തുക;
      അവസ്ഥ HB_OT_TAG_GPOS: table_index = 1; നിർത്തുക;
      സ്ഥിരസ്ഥിതി: തിരികെ;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL തർക്കം init0 (hb_face_t                     *face,
			  സ്ഥിരം hb_shape_plan_താക്കോൽ_t     *key);
  HB_INTERNAL ശൂന്യം fini ();

  HB_INTERNAL ശൂന്യം substitute (hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം;
  HB_INTERNAL ശൂന്യം position (hb_font_t *font, hb_buffer_t *buffer) സ്ഥിരം;
};

ഘടന hb_shape_plan_t;

ഘടന hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  തർക്കം apply_morx : 1;
#അല്ലെങ്കിൽ
  സ്ഥിതം constexpr തർക്കം apply_morx = അസത്യം;
#endif
  തർക്കം script_zero_marks : 1;
  തർക്കം script_fallback_position : 1;
  സ്ഥിരം ഘടന hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     സ്ഥിരം hb_segment_properties_t &props);

  HB_INTERNAL ശൂന്യം compile (hb_ot_shape_plan_t           &plan,
			    സ്ഥിരം hb_ot_shape_plan_താക്കോൽ_t &key);
};


#endif /* HB_OT_SHAPE_HH */
