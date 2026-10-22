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

#اگر_غیر_تعریف_شدہ HB_OT_SHAPE_HH
#تعریف_کرو HB_OT_SHAPE_HH

#شامل "hb.hh"

#شامل "hb-ot-map.hh"
#شامل "hb-aat-map.hh"


HB_اندرونی منطقی
hb_ot_layout_جدول_get_feature_variations_state (hb_face_t             *face,
						   hb_لیبل_t               جدول_لیبل_2,
						   ثابت صحیح             *coords,
						   غیرعلامتی صحیح           عدد_coords,
						   غیرعلامتی صحیح          *variations_اشاریہ,
						   hb_سمتیہ_t<غیرعلامتی> *lookup_conditions);

HB_اندرونی منطقی
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_لیبل_t جدول_لیبل_2,
                                             غیرعلامتی feature_اشاریہ,
                                             غیرعلامتی variations_اشاریہ,
                                             hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                                             hb_سیٹ_t *lookup_indexes);

ساخت hb_ot_shape_plan_کلید_t
{
  غیرعلامتی صحیح variations_اشاریہ[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_سمتیہ_t<غیرعلامتی> lookup_conditions[2];

  منطقی init (hb_face_t *face,
	     ثابت صحیح *coords,
	     غیرعلامتی   عدد_coords)
  {
    برائے (غیرعلامتی صحیح جدول_اشاریہ = 0; جدول_اشاریہ < 2; جدول_اشاریہ++)
      اگر (unlikely (!hb_ot_layout_جدول_get_feature_variations_state (
			face,
			جدول_لیبل_فہرست[جدول_اشاریہ],
			coords,
			عدد_coords,
			&variations_اشاریہ[جدول_اشاریہ],
			&lookup_conditions[جدول_اشاریہ])))
      {
	برائے (خودکار &v : lookup_conditions)
	  v.fini ();
	واپس جھوٹ;
      }
    واپس سچ;
  }

  خالی fini ()
  {
    برائے (خودکار &v : lookup_conditions)
      v.fini ();
  }

  منطقی get_lookup_variations (hb_face_t *face,
                              غیرعلامتی صحیح جدول_اشاریہ,
			      غیرعلامتی صحیح feature_اشاریہ,
			      hb_سیٹ_t *lookups) ثابت
  {
    واپس _hb_ot_layout_feature_get_lookup_variations (
	face, جدول_لیبل_فہرست[جدول_اشاریہ], feature_اشاریہ, variations_اشاریہ[جدول_اشاریہ],
	lookup_conditions[جدول_اشاریہ].as_sorted_array (), lookups);
  }

  منطقی equal (ثابت hb_ot_shape_plan_کلید_t *other)
  {
    واپس variations_اشاریہ[0] == other->variations_اشاریہ[0] &&
	   variations_اشاریہ[1] == other->variations_اشاریہ[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


ساخت hb_shape_plan_کلید_t;

ساخت hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ثابت ساخت hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ثابت خالی *ڈیٹا;
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FRACTIONS
  hb_نقاب_t frac_نقاب, numr_نقاب, dnom_نقاب;
#ورنہ
  ساکن constexpr hb_نقاب_t frac_نقاب = 0;
  ساکن constexpr hb_نقاب_t numr_نقاب = 0;
  ساکن constexpr hb_نقاب_t dnom_نقاب = 0;
#اگر_ختم
  hb_نقاب_t rtlm_نقاب;
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
  hb_نقاب_t kern_نقاب;
#ورنہ
  ساکن constexpr hb_نقاب_t kern_نقاب = 0;
#اگر_ختم

#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
  منطقی requested_kerning : 1;
#ورنہ
  ساکن constexpr منطقی requested_kerning = جھوٹ;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_OT_SHAPE_FRACTIONS
  منطقی has_frac : 1;
#ورنہ
  ساکن constexpr منطقی has_frac = جھوٹ;
#اگر_ختم
  منطقی has_vert : 1;
  منطقی has_gpos_mark : 1;
  منطقی zero_marks : 1;
  منطقی fallback_glyph_classes : 1;
  منطقی fallback_mark_positioning : 1;
  منطقی adjust_mark_positioning_when_zeroing : 1;

  منطقی apply_gpos : 1;
#اگر_غیر_تعریف_شدہ HB_NO_OT_KERN
  منطقی apply_kern : 1;
#ورنہ
  ساکن constexpr منطقی apply_kern = جھوٹ;
#اگر_ختم
  منطقی apply_fallback_kern : 1;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  منطقی apply_kerx : 1;
  منطقی apply_morx : 1;
  منطقی apply_trak : 1;
#ورنہ
  ساکن constexpr منطقی apply_kerx = جھوٹ;
  ساکن constexpr منطقی apply_morx = جھوٹ;
  ساکن constexpr منطقی apply_trak = جھوٹ;
#اگر_ختم

  خالی collect_lookups (hb_لیبل_t جدول_لیبل_2, hb_سیٹ_t *lookups) ثابت
  {
    غیرعلامتی صحیح جدول_اشاریہ;
    انتخاب (جدول_لیبل_2) {
      حالت HB_OT_لیبل_GSUB: جدول_اشاریہ = 0; توڑو;
      حالت HB_OT_لیبل_GPOS: جدول_اشاریہ = 1; توڑو;
      طےشدہ: واپس;
    }
    map.collect_lookups (جدول_اشاریہ, lookups);
  }

  HB_اندرونی منطقی init0 (hb_face_t                     *face,
			  ثابت hb_shape_plan_کلید_t     *key);
  HB_اندرونی خالی fini ();

  HB_اندرونی خالی substitute (hb_font_t *font, hb_بفر_t *بفر) ثابت;
  HB_اندرونی خالی position (hb_font_t *font, hb_بفر_t *بفر) ثابت;
};

ساخت hb_shape_plan_t;

ساخت hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  منطقی apply_morx : 1;
#ورنہ
  ساکن constexpr منطقی apply_morx = جھوٹ;
#اگر_ختم
  منطقی script_zero_marks : 1;
  منطقی script_fallback_position : 1;
  ثابت ساخت hb_ot_shaper_t *shaper;

  HB_اندرونی hb_ot_shape_planner_t (hb_face_t                     *face,
				     ثابت hb_segment_properties_t &props);

  HB_اندرونی خالی compile (hb_ot_shape_plan_t           &plan,
			    ثابت hb_ot_shape_plan_کلید_t &key);
};


#اگر_ختم /* HB_OT_SHAPE_HH */
