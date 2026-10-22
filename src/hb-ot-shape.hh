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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-ot-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-map.hh"


HB_INTERNAL ତାର୍କିକ
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା             *coords,
						   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା           ଅଙ୍କ_coords,
						   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL ତାର୍କିକ
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ଅଚଳ unsigned> conditions,
                                             hb_set_t *lookup_indexes);

ସଂରଚନା hb_ot_shape_plan_ଚାବି_t
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  ତାର୍କିକ init (hb_face_t *face,
	     ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *coords,
	     unsigned   ଅଙ୍କ_coords)
  {
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index = 0; table_index < 2; table_index++)
      ଯଦି (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			ଅଙ୍କ_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	ପାଇଁ (auto &v : lookup_conditions)
	  v.fini ();
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ fini ()
  {
    ପାଇଁ (auto &v : lookup_conditions)
      v.fini ();
  }

  ତାର୍କିକ get_lookup_variations (hb_face_t *face,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature_index,
			      hb_set_t *lookups) ଅଚଳ
  {
    ଫେରନ୍ତୁ _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  ତାର୍କିକ equal (ଅଚଳ hb_ot_shape_plan_ଚାବି_t *other)
  {
    ଫେରନ୍ତୁ variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ସଂରଚନା hb_shape_plan_ଚାବି_t;

ସଂରଚନା hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ଅଚଳ ସଂରଚନା hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ଅଚଳ ଶୂନ୍ୟ *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr hb_mask_t frac_mask = 0;
  ସ୍ଥିର constexpr hb_mask_t numr_mask = 0;
  ସ୍ଥିର constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  ତାର୍କିକ requested_kerning : 1;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr ତାର୍କିକ requested_kerning = ମିଥ୍ୟା;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  ତାର୍କିକ has_frac : 1;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr ତାର୍କିକ has_frac = ମିଥ୍ୟା;
#endif
  ତାର୍କିକ has_vert : 1;
  ତାର୍କିକ has_gpos_mark : 1;
  ତାର୍କିକ zero_marks : 1;
  ତାର୍କିକ fallback_glyph_classes : 1;
  ତାର୍କିକ fallback_mark_positioning : 1;
  ତାର୍କିକ adjust_mark_positioning_when_zeroing : 1;

  ତାର୍କିକ apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  ତାର୍କିକ apply_kern : 1;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr ତାର୍କିକ apply_kern = ମିଥ୍ୟା;
#endif
  ତାର୍କିକ apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  ତାର୍କିକ apply_kerx : 1;
  ତାର୍କିକ apply_morx : 1;
  ତାର୍କିକ apply_trak : 1;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr ତାର୍କିକ apply_kerx = ମିଥ୍ୟା;
  ସ୍ଥିର constexpr ତାର୍କିକ apply_morx = ମିଥ୍ୟା;
  ସ୍ଥିର constexpr ତାର୍କିକ apply_trak = ମିଥ୍ୟା;
#endif

  ଶୂନ୍ୟ collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା table_index;
    ବାଛ (table_tag) {
      ସ୍ଥିତି HB_OT_TAG_GSUB: table_index = 0; ଭାଙ୍ଗ;
      ସ୍ଥିତି HB_OT_TAG_GPOS: table_index = 1; ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL ତାର୍କିକ init0 (hb_face_t                     *face,
			  ଅଚଳ hb_shape_plan_ଚାବି_t     *key);
  HB_INTERNAL ଶୂନ୍ୟ fini ();

  HB_INTERNAL ଶୂନ୍ୟ substitute (hb_font_t *font, hb_buffer_t *buffer) ଅଚଳ;
  HB_INTERNAL ଶୂନ୍ୟ position (hb_font_t *font, hb_buffer_t *buffer) ଅଚଳ;
};

ସଂରଚନା hb_shape_plan_t;

ସଂରଚନା hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  ତାର୍କିକ apply_morx : 1;
#ଅନ୍ୟଥା
  ସ୍ଥିର constexpr ତାର୍କିକ apply_morx = ମିଥ୍ୟା;
#endif
  ତାର୍କିକ script_zero_marks : 1;
  ତାର୍କିକ script_fallback_position : 1;
  ଅଚଳ ସଂରଚନା hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ଅଚଳ hb_segment_properties_t &props);

  HB_INTERNAL ଶୂନ୍ୟ compile (hb_ot_shape_plan_t           &plan,
			    ଅଚଳ hb_ot_shape_plan_ଚାବି_t &key);
};


#endif /* HB_OT_SHAPE_HH */
