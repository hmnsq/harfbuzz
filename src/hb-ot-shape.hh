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

#அந்தர்பாவ "hb.hh"

#அந்தர்பாவ "hb-ot-map.hh"
#அந்தர்பாவ "hb-aat-map.hh"


HB_INTERNAL தருக்க
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   மாறிலி முழு             *coords,
						   unsigned முழு           இலக்கம்_coords,
						   unsigned முழு          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL தருக்க
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<மாறிலி unsigned> conditions,
                                             hb_set_t *lookup_indexes);

கட்டமைப்பு hb_ot_shape_plan_சாவி_t
{
  unsigned முழு variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  தருக்க init (hb_face_t *face,
	     மாறிலி முழு *coords,
	     unsigned   இலக்கம்_coords)
  {
    க்கு (unsigned முழு table_index = 0; table_index < 2; table_index++)
      எனில் (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			இலக்கம்_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	க்கு (auto &v : lookup_conditions)
	  v.fini ();
	திரும்பு பொய்;
      }
    திரும்பு உண்மை;
  }

  வெறுமை fini ()
  {
    க்கு (auto &v : lookup_conditions)
      v.fini ();
  }

  தருக்க get_lookup_variations (hb_face_t *face,
                              unsigned முழு table_index,
			      unsigned முழு feature_index,
			      hb_set_t *lookups) மாறிலி
  {
    திரும்பு _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  தருக்க equal (மாறிலி hb_ot_shape_plan_சாவி_t *other)
  {
    திரும்பு variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


கட்டமைப்பு hb_shape_plan_சாவி_t;

கட்டமைப்பு hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  மாறிலி கட்டமைப்பு hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  மாறிலி வெறுமை *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#இல்லையெனில்
  நிலையான constexpr hb_mask_t frac_mask = 0;
  நிலையான constexpr hb_mask_t numr_mask = 0;
  நிலையான constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#இல்லையெனில்
  நிலையான constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  தருக்க requested_kerning : 1;
#இல்லையெனில்
  நிலையான constexpr தருக்க requested_kerning = பொய்;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  தருக்க has_frac : 1;
#இல்லையெனில்
  நிலையான constexpr தருக்க has_frac = பொய்;
#endif
  தருக்க has_vert : 1;
  தருக்க has_gpos_mark : 1;
  தருக்க zero_marks : 1;
  தருக்க fallback_glyph_classes : 1;
  தருக்க fallback_mark_positioning : 1;
  தருக்க adjust_mark_positioning_when_zeroing : 1;

  தருக்க apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  தருக்க apply_kern : 1;
#இல்லையெனில்
  நிலையான constexpr தருக்க apply_kern = பொய்;
#endif
  தருக்க apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  தருக்க apply_kerx : 1;
  தருக்க apply_morx : 1;
  தருக்க apply_trak : 1;
#இல்லையெனில்
  நிலையான constexpr தருக்க apply_kerx = பொய்;
  நிலையான constexpr தருக்க apply_morx = பொய்;
  நிலையான constexpr தருக்க apply_trak = பொய்;
#endif

  வெறுமை collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) மாறிலி
  {
    unsigned முழு table_index;
    தேர்வு (table_tag) {
      நிலை HB_OT_TAG_GSUB: table_index = 0; நிறுத்து;
      நிலை HB_OT_TAG_GPOS: table_index = 1; நிறுத்து;
      இயல்பு: திரும்பு;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL தருக்க init0 (hb_face_t                     *face,
			  மாறிலி hb_shape_plan_சாவி_t     *key);
  HB_INTERNAL வெறுமை fini ();

  HB_INTERNAL வெறுமை substitute (hb_font_t *font, hb_buffer_t *buffer) மாறிலி;
  HB_INTERNAL வெறுமை position (hb_font_t *font, hb_buffer_t *buffer) மாறிலி;
};

கட்டமைப்பு hb_shape_plan_t;

கட்டமைப்பு hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  தருக்க apply_morx : 1;
#இல்லையெனில்
  நிலையான constexpr தருக்க apply_morx = பொய்;
#endif
  தருக்க script_zero_marks : 1;
  தருக்க script_fallback_position : 1;
  மாறிலி கட்டமைப்பு hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     மாறிலி hb_segment_properties_t &props);

  HB_INTERNAL வெறுமை compile (hb_ot_shape_plan_t           &plan,
			    மாறிலி hb_ot_shape_plan_சாவி_t &key);
};


#endif /* HB_OT_SHAPE_HH */
