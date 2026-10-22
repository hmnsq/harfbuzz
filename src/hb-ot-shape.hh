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

#شامل "hb.hh"

#شامل "hb-ot-map.hh"
#شامل "hb-aat-map.hh"


HB_INTERNAL منطقی
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   ثابت صحیح             *coords,
						   unsigned صحیح           رقم_coords,
						   unsigned صحیح          *variations_index,
						   hb_vector_t<unsigned> *lookup_conditions);

HB_INTERNAL منطقی
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             unsigned feature_index,
                                             unsigned variations_index,
                                             hb_sorted_array_t<ثابت unsigned> conditions,
                                             hb_set_t *lookup_indexes);

ساختار hb_ot_shape_plan_کلید_t
{
  unsigned صحیح variations_index[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<unsigned> lookup_conditions[2];

  منطقی init (hb_face_t *face,
	     ثابت صحیح *coords,
	     unsigned   رقم_coords)
  {
    برای (unsigned صحیح table_index = 0; table_index < 2; table_index++)
      اگر (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_index],
			coords,
			رقم_coords,
			&variations_index[table_index],
			&lookup_conditions[table_index])))
      {
	برای (auto &v : lookup_conditions)
	  v.fini ();
	بازگشت نادرست;
      }
    بازگشت درست;
  }

  تهی fini ()
  {
    برای (auto &v : lookup_conditions)
      v.fini ();
  }

  منطقی get_lookup_variations (hb_face_t *face,
                              unsigned صحیح table_index,
			      unsigned صحیح feature_index,
			      hb_set_t *lookups) ثابت
  {
    بازگشت _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_index], feature_index, variations_index[table_index],
	lookup_conditions[table_index].as_sorted_array (), lookups);
  }

  منطقی equal (ثابت hb_ot_shape_plan_کلید_t *other)
  {
    بازگشت variations_index[0] == other->variations_index[0] &&
	   variations_index[1] == other->variations_index[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ساختار hb_shape_plan_کلید_t;

ساختار hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ثابت ساختار hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ثابت تهی *data;
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#وگرنه
  ایستا constexpr hb_mask_t frac_mask = 0;
  ایستا constexpr hb_mask_t numr_mask = 0;
  ایستا constexpr hb_mask_t dnom_mask = 0;
#endif
  hb_mask_t rtlm_mask;
#ifndef HB_NO_OT_KERN
  hb_mask_t kern_mask;
#وگرنه
  ایستا constexpr hb_mask_t kern_mask = 0;
#endif

#ifndef HB_NO_OT_KERN
  منطقی requested_kerning : 1;
#وگرنه
  ایستا constexpr منطقی requested_kerning = نادرست;
#endif
#ifndef HB_NO_OT_SHAPE_FRACTIONS
  منطقی has_frac : 1;
#وگرنه
  ایستا constexpr منطقی has_frac = نادرست;
#endif
  منطقی has_vert : 1;
  منطقی has_gpos_mark : 1;
  منطقی zero_marks : 1;
  منطقی fallback_glyph_classes : 1;
  منطقی fallback_mark_positioning : 1;
  منطقی adjust_mark_positioning_when_zeroing : 1;

  منطقی apply_gpos : 1;
#ifndef HB_NO_OT_KERN
  منطقی apply_kern : 1;
#وگرنه
  ایستا constexpr منطقی apply_kern = نادرست;
#endif
  منطقی apply_fallback_kern : 1;
#ifndef HB_NO_AAT_SHAPE
  منطقی apply_kerx : 1;
  منطقی apply_morx : 1;
  منطقی apply_trak : 1;
#وگرنه
  ایستا constexpr منطقی apply_kerx = نادرست;
  ایستا constexpr منطقی apply_morx = نادرست;
  ایستا constexpr منطقی apply_trak = نادرست;
#endif

  تهی collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) ثابت
  {
    unsigned صحیح table_index;
    انتخاب (table_tag) {
      حالت HB_OT_TAG_GSUB: table_index = 0; بشکن;
      حالت HB_OT_TAG_GPOS: table_index = 1; بشکن;
      پیشفرض: بازگشت;
    }
    map.collect_lookups (table_index, lookups);
  }

  HB_INTERNAL منطقی init0 (hb_face_t                     *face,
			  ثابت hb_shape_plan_کلید_t     *key);
  HB_INTERNAL تهی fini ();

  HB_INTERNAL تهی substitute (hb_font_t *font, hb_buffer_t *buffer) ثابت;
  HB_INTERNAL تهی position (hb_font_t *font, hb_buffer_t *buffer) ثابت;
};

ساختار hb_shape_plan_t;

ساختار hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#ifndef HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  منطقی apply_morx : 1;
#وگرنه
  ایستا constexpr منطقی apply_morx = نادرست;
#endif
  منطقی script_zero_marks : 1;
  منطقی script_fallback_position : 1;
  ثابت ساختار hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     ثابت hb_segment_properties_t &props);

  HB_INTERNAL تهی compile (hb_ot_shape_plan_t           &plan,
			    ثابت hb_ot_shape_plan_کلید_t &key);
};


#endif /* HB_OT_SHAPE_HH */
