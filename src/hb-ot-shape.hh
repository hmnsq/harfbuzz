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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-ot-map.hh"
#অন্তর্ভাব "hb-aat-map.hh"


HB_INTERNAL যৌক্তিক
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ধ্রুবক পূর্ণসংখ্যা             *coords,
						   unsigned পূর্ণসংখ্যা           অঙ্ক_coords,
						   unsigned পূর্ণসংখ্যা          *variations_সূচক,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL যৌক্তিক
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_সূচক,
                                             unsigned variations_সূচক,
                                             hb_sorted_array_t<ধ্রুবক unsigned> conditions,
                                             hb_set_t *lookup_indexes);

কাঠামো hb_ot_shape_plan_চাবি_t
{
  unsigned পূর্ণসংখ্যা variations_সূচক[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  যৌক্তিক init (hb_face_t *face,
	     ধ্রুবক পূর্ণসংখ্যা *coords,
	     unsigned   অঙ্ক_coords)
  {
    জন্য (unsigned পূর্ণসংখ্যা table_সূচক = 0; table_সূচক < 2; table_সূচক++)
      যদি (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_সূচক],
			coords,
			অঙ্ক_coords,
			&variations_সূচক[table_সূচক],
			&lookup_conditions[table_সূচক])))
      {
	জন্য (auto &v : lookup_conditions)
	  v.fini ();
	ফেরত মিথ্যা;
      }
    ফেরত সত্য;
  }

  শূন্য fini ()
  {
    জন্য (auto &v : lookup_conditions)
      v.fini ();
  }

  যৌক্তিক get_lookup_variations (hb_face_t *face,
                              unsigned পূর্ণসংখ্যা table_সূচক,
			      unsigned পূর্ণসংখ্যা feature_সূচক,
			      hb_set_t *lookups) ধ্রুবক
  {
    ফেরত _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_সূচক], feature_সূচক, variations_সূচক[table_সূচক],
	lookup_conditions[table_সূচক].as_sorted_array (), lookups);
  }

  যৌক্তিক equal (ধ্রুবক hb_ot_shape_plan_চাবি_t *other)
  {
    ফেরত variations_সূচক[0] == other->variations_সূচক[0] &&
	   variations_সূচক[1] == other->variations_সূচক[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


কাঠামো hb_shape_plan_চাবি_t;

কাঠামো hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ধ্রুবক কাঠামো hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ধ্রুবক শূন্য *তথ্য;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#অন্যথা
  স্থির constexpr hb_mask_t frac_mask = 0;
  স্থির constexpr hb_mask_t numr_mask = 0;
  স্থির constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#অন্যথা
  স্থির constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  যৌক্তিক requested_kerning : 1;
#অন্যথা
  স্থির constexpr যৌক্তিক requested_kerning = মিথ্যা;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  যৌক্তিক has_frac : 1;
#অন্যথা
  স্থির constexpr যৌক্তিক has_frac = মিথ্যা;
#endif
  যৌক্তিক has_vert : 1;
  যৌক্তিক has_gpos_mark : 1;
  যৌক্তিক zero_marks : 1;
  যৌক্তিক fallback_glyph_classes : 1;
  যৌক্তিক fallback_mark_positioning : 1;
  যৌক্তিক adjust_mark_positioning_when_zeroing : 1;

  যৌক্তিক apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  যৌক্তিক apply_kern : 1;
#অন্যথা
  স্থির constexpr যৌক্তিক apply_kern = মিথ্যা;
#endif
  যৌক্তিক apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  যৌক্তিক apply_kerx : 1;
  যৌক্তিক apply_morx : 1;
  যৌক্তিক apply_trak : 1;
#অন্যথা
  স্থির constexpr যৌক্তিক apply_kerx = মিথ্যা;
  স্থির constexpr যৌক্তিক apply_morx = মিথ্যা;
  স্থির constexpr যৌক্তিক apply_trak = মিথ্যা;
#endif

  শূন্য collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা table_সূচক;
    নির্বাচন (table_tag) {
      ক্ষেত্র HB_OT_TAG_GSUB: table_সূচক = 0; বিরতি;
      ক্ষেত্র HB_OT_TAG_GPOS: table_সূচক = 1; বিরতি;
      পূর্বনির্ধারিত: ফেরত;
    }
    map.collect_lookups (table_সূচক, lookups);
  }

  HB_INTERNAL যৌক্তিক init0 (hb_face_t                     *face,
			  ধ্রুবক hb_shape_plan_চাবি_t     *key);
  HB_INTERNAL শূন্য fini ();

  HB_INTERNAL শূন্য substitute (hb_font_t *font, hb_বাফার_t *বাফার) ধ্রুবক;
  HB_INTERNAL শূন্য position (hb_font_t *font, hb_বাফার_t *বাফার) ধ্রুবক;
};

কাঠামো hb_shape_plan_t;

কাঠামো hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  যৌক্তিক apply_morx : 1;
#অন্যথা
  স্থির constexpr যৌক্তিক apply_morx = মিথ্যা;
#endif
  যৌক্তিক script_zero_marks : 1;
  যৌক্তিক script_fallback_position : 1;
  ধ্রুবক কাঠামো hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ধ্রুবক hb_segment_properties_t &props);

  HB_INTERNAL শূন্য compile (hb_ot_shape_plan_t           &plan,
			    ধ্রুবক hb_ot_shape_plan_চাবি_t &key);
};


#endif /* HB_OT_SHAPE_HH */
