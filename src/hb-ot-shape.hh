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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_SHAPE_HH
#وصف_ڪريو HB_OT_SHAPE_HH

#ضمن "hb.hh"

#ضمن "hb-ot-map.hh"
#ضمن "hb-aat-map.hh"


HB_اندروني منطقي
hb_ot_layout_جدول_get_feature_variations_state (hb_face_t             *face,
						   hb_ٽيگ_t               جدول_ٽيگ_2,
						   ثابت صحيح             *coords,
						   بنانشان صحيح           انگ_coords,
						   بنانشان صحيح          *variations_اشاريو,
						   hb_ويڪٽر_t<بنانشان> *lookup_conditions);

HB_اندروني منطقي
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_ٽيگ_t جدول_ٽيگ_2,
                                             بنانشان feature_اشاريو,
                                             بنانشان variations_اشاريو,
                                             hb_sorted_array_t<ثابت بنانشان> conditions,
                                             hb_سيٽ_t *lookup_indexes);

بنية hb_ot_shape_plan_ڪنجي_t
{
  بنانشان صحيح variations_اشاريو[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_ويڪٽر_t<بنانشان> lookup_conditions[2];

  منطقي init (hb_face_t *face,
	     ثابت صحيح *coords,
	     بنانشان   انگ_coords)
  {
    لكل (بنانشان صحيح جدول_اشاريو = 0; جدول_اشاريو < 2; جدول_اشاريو++)
      إذا (unlikely (!hb_ot_layout_جدول_get_feature_variations_state (
			face,
			جدول_ٽيگ_فهرست[جدول_اشاريو],
			coords,
			انگ_coords,
			&variations_اشاريو[جدول_اشاريو],
			&lookup_conditions[جدول_اشاريو])))
      {
	لكل (خودڪار &v : lookup_conditions)
	  v.fini ();
	أرجع خطأ;
      }
    أرجع سچ;
  }

  فراغ fini ()
  {
    لكل (خودڪار &v : lookup_conditions)
      v.fini ();
  }

  منطقي get_lookup_variations (hb_face_t *face,
                              بنانشان صحيح جدول_اشاريو,
			      بنانشان صحيح feature_اشاريو,
			      hb_سيٽ_t *lookups) ثابت
  {
    أرجع _hb_ot_layout_feature_get_lookup_variations (
	face, جدول_ٽيگ_فهرست[جدول_اشاريو], feature_اشاريو, variations_اشاريو[جدول_اشاريو],
	lookup_conditions[جدول_اشاريو].as_sorted_array (), lookups);
  }

  منطقي equal (ثابت hb_ot_shape_plan_ڪنجي_t *other)
  {
    أرجع variations_اشاريو[0] == other->variations_اشاريو[0] &&
	   variations_اشاريو[1] == other->variations_اشاريو[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


بنية hb_shape_plan_ڪنجي_t;

بنية hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ثابت بنية hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ثابت فراغ *معلومات;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE_FRACTIONS
  hb_نقاب_t frac_نقاب, numr_نقاب, dnom_نقاب;
#وإلا
  ساكن constexpr hb_نقاب_t frac_نقاب = 0;
  ساكن constexpr hb_نقاب_t numr_نقاب = 0;
  ساكن constexpr hb_نقاب_t dnom_نقاب = 0;
#جيڪڏهن_پورو
  hb_نقاب_t rtlm_نقاب;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
  hb_نقاب_t kern_نقاب;
#وإلا
  ساكن constexpr hb_نقاب_t kern_نقاب = 0;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
  منطقي requested_kerning : 1;
#وإلا
  ساكن constexpr منطقي requested_kerning = خطأ;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_SHAPE_FRACTIONS
  منطقي has_frac : 1;
#وإلا
  ساكن constexpr منطقي has_frac = خطأ;
#جيڪڏهن_پورو
  منطقي has_vert : 1;
  منطقي has_gpos_mark : 1;
  منطقي zero_marks : 1;
  منطقي fallback_glyph_classes : 1;
  منطقي fallback_mark_positioning : 1;
  منطقي adjust_mark_positioning_when_zeroing : 1;

  منطقي apply_gpos : 1;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_KERN
  منطقي apply_kern : 1;
#وإلا
  ساكن constexpr منطقي apply_kern = خطأ;
#جيڪڏهن_پورو
  منطقي apply_fallback_kern : 1;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  منطقي apply_kerx : 1;
  منطقي apply_morx : 1;
  منطقي apply_trak : 1;
#وإلا
  ساكن constexpr منطقي apply_kerx = خطأ;
  ساكن constexpr منطقي apply_morx = خطأ;
  ساكن constexpr منطقي apply_trak = خطأ;
#جيڪڏهن_پورو

  فراغ collect_lookups (hb_ٽيگ_t جدول_ٽيگ_2, hb_سيٽ_t *lookups) ثابت
  {
    بنانشان صحيح جدول_اشاريو;
    بدل (جدول_ٽيگ_2) {
      حالة HB_OT_ٽيگ_GSUB: جدول_اشاريو = 0; اكسر;
      حالة HB_OT_ٽيگ_GPOS: جدول_اشاريو = 1; اكسر;
      افتراضي: أرجع;
    }
    map.collect_lookups (جدول_اشاريو, lookups);
  }

  HB_اندروني منطقي init0 (hb_face_t                     *face,
			  ثابت hb_shape_plan_ڪنجي_t     *key);
  HB_اندروني فراغ fini ();

  HB_اندروني فراغ substitute (hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندروني فراغ position (hb_font_t *font, hb_بفر_t *بفر) ثابت;
};

بنية hb_shape_plan_t;

بنية hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  منطقي apply_morx : 1;
#وإلا
  ساكن constexpr منطقي apply_morx = خطأ;
#جيڪڏهن_پورو
  منطقي script_zero_marks : 1;
  منطقي script_fallback_position : 1;
  ثابت بنية hb_ot_shaper_t *shaper;

  HB_اندروني hb_ot_shape_planner_t (hb_face_t                     *face,
				     ثابت hb_segment_properties_t &props);

  HB_اندروني فراغ compile (hb_ot_shape_plan_t           &plan,
			    ثابت hb_ot_shape_plan_ڪنجي_t &key);
};


#جيڪڏهن_پورو /* HB_OT_SHAPE_HH */
