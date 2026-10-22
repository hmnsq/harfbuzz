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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-ot-map.hh"
#अन्तर्भाव "hb-aat-map.hh"


HB_INTERNAL तार्किक
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ध्रुव पूर्णाङ्क             *coords,
						   unsigned पूर्णाङ्क           अंक_coords,
						   unsigned पूर्णाङ्क          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL तार्किक
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ध्रुव unsigned> conditions,
                                             hb_set_t *lookup_indexes);

संरचना hb_ot_shape_plan_कुंजी_t
{
  unsigned पूर्णाङ्क variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  तार्किक init (hb_face_t *face,
	     ध्रुव पूर्णाङ्क *coords,
	     unsigned   अंक_coords)
  {
    प्रति (unsigned पूर्णाङ्क table_index = 0; table_index < 2; table_index++)
      यदि (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			अंक_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	प्रति (auto &v : lookup_conditions)
	  v.fini ();
	प्रत्यागम असत्य;
      }
    प्रत्यागम सत्य;
  }

  शून्य fini ()
  {
    प्रति (auto &v : lookup_conditions)
      v.fini ();
  }

  तार्किक get_lookup_variations (hb_face_t *face,
                              unsigned पूर्णाङ्क table_index,
			      unsigned पूर्णाङ्क feature_index,
			      hb_set_t *lookups) ध्रुव
  {
    प्रत्यागम _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  तार्किक equal (ध्रुव hb_ot_shape_plan_कुंजी_t *other)
  {
    प्रत्यागम variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


संरचना hb_shape_plan_कुंजी_t;

संरचना hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ध्रुव संरचना hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ध्रुव शून्य *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#अन्यथा
  स्थिर constexpr hb_mask_t frac_mask = 0;
  स्थिर constexpr hb_mask_t numr_mask = 0;
  स्थिर constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#अन्यथा
  स्थिर constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  तार्किक requested_kerning : 1;
#अन्यथा
  स्थिर constexpr तार्किक requested_kerning = असत्य;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  तार्किक has_frac : 1;
#अन्यथा
  स्थिर constexpr तार्किक has_frac = असत्य;
#endif
  तार्किक has_vert : 1;
  तार्किक has_gpos_mark : 1;
  तार्किक zero_marks : 1;
  तार्किक fallback_glyph_classes : 1;
  तार्किक fallback_mark_positioning : 1;
  तार्किक adjust_mark_positioning_when_zeroing : 1;

  तार्किक apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  तार्किक apply_kern : 1;
#अन्यथा
  स्थिर constexpr तार्किक apply_kern = असत्य;
#endif
  तार्किक apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  तार्किक apply_kerx : 1;
  तार्किक apply_morx : 1;
  तार्किक apply_trak : 1;
#अन्यथा
  स्थिर constexpr तार्किक apply_kerx = असत्य;
  स्थिर constexpr तार्किक apply_morx = असत्य;
  स्थिर constexpr तार्किक apply_trak = असत्य;
#endif

  शून्य collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ध्रुव
  {
    unsigned पूर्णाङ्क table_index;
    विकल्प (table_tag) {
      पक्ष HB_OT_TAG_GSUB: table_index = 0; भङ्ग;
      पक्ष HB_OT_TAG_GPOS: table_index = 1; भङ्ग;
      स्वभाव: प्रत्यागम;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL तार्किक init0 (hb_face_t                     *face,
			  ध्रुव hb_shape_plan_कुंजी_t     *key);
  HB_INTERNAL शून्य fini ();

  HB_INTERNAL शून्य substitute (hb_font_t *font, hb_buffer_t *buffer) ध्रुव;
  HB_INTERNAL शून्य position (hb_font_t *font, hb_buffer_t *buffer) ध्रुव;
};

संरचना hb_shape_plan_t;

संरचना hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  तार्किक apply_morx : 1;
#अन्यथा
  स्थिर constexpr तार्किक apply_morx = असत्य;
#endif
  तार्किक script_zero_marks : 1;
  तार्किक script_fallback_position : 1;
  ध्रुव संरचना hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ध्रुव hb_segment_properties_t &props);

  HB_INTERNAL शून्य compile (hb_ot_shape_plan_t           &plan,
			    ध्रुव hb_ot_shape_plan_कुंजी_t &key);
};


#endif /* HB_OT_SHAPE_HH */
