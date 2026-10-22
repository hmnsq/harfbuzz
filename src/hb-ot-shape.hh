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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-map.hh"


HB_INTERNAL ತಾರ್ಕಿಕ
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ             *coords,
						   unsigned ಪೂರ್ಣಾಙ್ಕ           ಅಂಕೆ_coords,
						   unsigned ಪೂರ್ಣಾಙ್ಕ          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL ತಾರ್ಕಿಕ
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ಧ್ರುವ unsigned> conditions,
                                             hb_set_t *lookup_indexes);

ಸಂರಚನಾ hb_ot_shape_plan_ಕೀಲಿ_t
{
  unsigned ಪೂರ್ಣಾಙ್ಕ variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  ತಾರ್ಕಿಕ init (hb_face_t *face,
	     ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *coords,
	     unsigned   ಅಂಕೆ_coords)
  {
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ table_index = 0; table_index < 2; table_index++)
      ಯದಿ (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			ಅಂಕೆ_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	ಪ್ರತಿ (auto &v : lookup_conditions)
	  v.fini ();
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ fini ()
  {
    ಪ್ರತಿ (auto &v : lookup_conditions)
      v.fini ();
  }

  ತಾರ್ಕಿಕ get_lookup_variations (hb_face_t *face,
                              unsigned ಪೂರ್ಣಾಙ್ಕ table_index,
			      unsigned ಪೂರ್ಣಾಙ್ಕ feature_index,
			      hb_set_t *lookups) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  ತಾರ್ಕಿಕ equal (ಧ್ರುವ hb_ot_shape_plan_ಕೀಲಿ_t *other)
  {
    ಪ್ರತ್ಯಾಗಮ variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ಸಂರಚನಾ hb_shape_plan_ಕೀಲಿ_t;

ಸಂರಚನಾ hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ಧ್ರುವ ಸಂರಚನಾ hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ಧ್ರುವ ಶೂನ್ಯ *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr hb_mask_t frac_mask = 0;
  ಸ್ಥಿರ constexpr hb_mask_t numr_mask = 0;
  ಸ್ಥಿರ constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  ತಾರ್ಕಿಕ requested_kerning : 1;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ requested_kerning = ಅಸತ್ಯ;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  ತಾರ್ಕಿಕ has_frac : 1;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_frac = ಅಸತ್ಯ;
#endif
  ತಾರ್ಕಿಕ has_vert : 1;
  ತಾರ್ಕಿಕ has_gpos_mark : 1;
  ತಾರ್ಕಿಕ zero_marks : 1;
  ತಾರ್ಕಿಕ fallback_glyph_classes : 1;
  ತಾರ್ಕಿಕ fallback_mark_positioning : 1;
  ತಾರ್ಕಿಕ adjust_mark_positioning_when_zeroing : 1;

  ತಾರ್ಕಿಕ apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  ತಾರ್ಕಿಕ apply_kern : 1;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apply_kern = ಅಸತ್ಯ;
#endif
  ತಾರ್ಕಿಕ apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  ತಾರ್ಕಿಕ apply_kerx : 1;
  ತಾರ್ಕಿಕ apply_morx : 1;
  ತಾರ್ಕಿಕ apply_trak : 1;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apply_kerx = ಅಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apply_morx = ಅಸತ್ಯ;
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apply_trak = ಅಸತ್ಯ;
#endif

  ಶೂನ್ಯ collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ table_index;
    ವಿಕಲ್ಪ (table_tag) {
      ಪಕ್ಷ HB_OT_TAG_GSUB: table_index = 0; ಭಙ್ಗ;
      ಪಕ್ಷ HB_OT_TAG_GPOS: table_index = 1; ಭಙ್ಗ;
      ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL ತಾರ್ಕಿಕ init0 (hb_face_t                     *face,
			  ಧ್ರುವ hb_shape_plan_ಕೀಲಿ_t     *key);
  HB_INTERNAL ಶೂನ್ಯ fini ();

  HB_INTERNAL ಶೂನ್ಯ substitute (hb_font_t *font, hb_buffer_t *buffer) ಧ್ರುವ;
  HB_INTERNAL ಶೂನ್ಯ position (hb_font_t *font, hb_buffer_t *buffer) ಧ್ರುವ;
};

ಸಂರಚನಾ hb_shape_plan_t;

ಸಂರಚನಾ hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  ತಾರ್ಕಿಕ apply_morx : 1;
#ಅನ್ಯಥಾ
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ apply_morx = ಅಸತ್ಯ;
#endif
  ತಾರ್ಕಿಕ script_zero_marks : 1;
  ತಾರ್ಕಿಕ script_fallback_position : 1;
  ಧ್ರುವ ಸಂರಚನಾ hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ಧ್ರುವ hb_segment_properties_t &props);

  HB_INTERNAL ಶೂನ್ಯ compile (hb_ot_shape_plan_t           &plan,
			    ಧ್ರುವ hb_ot_shape_plan_ಕೀಲಿ_t &key);
};


#endif /* HB_OT_SHAPE_HH */
