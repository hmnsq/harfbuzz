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

#اگر_تعریف_نشده HB_OT_SHAPE_HH
#تعریف_کن HB_OT_SHAPE_HH

#ضمن "hb.hh"

#ضمن "hb-ot-map.hh"
#ضمن "hb-aat-map.hh"


HB_داخلی منطقي
hb_ot_layout_جدول_get_feature_variations_state (hb_face_t             *face,
						   hb_برچسب_t               جدول_برچسب_2,
						   ثابت صحيح             *coords,
						   بیعلامت صحيح           رقم_coords,
						   بیعلامت صحيح          *variations_شاخص,
						   hb_وکتور_t<بیعلامت> *lookup_conditions);

HB_داخلی منطقي
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_برچسب_t جدول_برچسب_2,
                                             بیعلامت feature_شاخص,
                                             بیعلامت variations_شاخص,
                                             hb_sorted_array_t<ثابت بیعلامت> conditions,
                                             hb_مجموعه_t *lookup_indexes);

بنية hb_ot_shape_plan_کلید_t
{
  بیعلامت صحيح variations_شاخص[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_وکتور_t<بیعلامت> lookup_conditions[2];

  منطقي init (hb_face_t *face,
	     ثابت صحيح *coords,
	     بیعلامت   رقم_coords)
  {
    لكل (بیعلامت صحيح جدول_شاخص = 0; جدول_شاخص < 2; جدول_شاخص++)
      إذا (unlikely (!hb_ot_layout_جدول_get_feature_variations_state (
			face,
			جدول_برچسبها[جدول_شاخص],
			coords,
			رقم_coords,
			&variations_شاخص[جدول_شاخص],
			&lookup_conditions[جدول_شاخص])))
      {
	لكل (خودکار &v : lookup_conditions)
	  v.fini ();
	أرجع خطأ;
      }
    أرجع درست;
  }

  فراغ fini ()
  {
    لكل (خودکار &v : lookup_conditions)
      v.fini ();
  }

  منطقي get_lookup_variations (hb_face_t *face,
                              بیعلامت صحيح جدول_شاخص,
			      بیعلامت صحيح feature_شاخص,
			      hb_مجموعه_t *lookups) ثابت
  {
    أرجع _hb_ot_layout_feature_get_lookup_variations (
	face, جدول_برچسبها[جدول_شاخص], feature_شاخص, variations_شاخص[جدول_شاخص],
	lookup_conditions[جدول_شاخص].as_sorted_array (), lookups);
  }

  منطقي equal (ثابت hb_ot_shape_plan_کلید_t *other)
  {
    أرجع variations_شاخص[0] == other->variations_شاخص[0] &&
	   variations_شاخص[1] == other->variations_شاخص[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


بنية hb_shape_plan_کلید_t;

بنية hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  ثابت بنية hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  ثابت فراغ *داده;
#اگر_تعریف_نشده HB_NO_OT_SHAPE_FRACTIONS
  hb_ماسک_t frac_ماسک, numr_ماسک, dnom_ماسک;
#وإلا
  ساكن constexpr hb_ماسک_t frac_ماسک = 0;
  ساكن constexpr hb_ماسک_t numr_ماسک = 0;
  ساكن constexpr hb_ماسک_t dnom_ماسک = 0;
#پایان_اگر
  hb_ماسک_t rtlm_ماسک;
#اگر_تعریف_نشده HB_NO_OT_KERN
  hb_ماسک_t kern_ماسک;
#وإلا
  ساكن constexpr hb_ماسک_t kern_ماسک = 0;
#پایان_اگر

#اگر_تعریف_نشده HB_NO_OT_KERN
  منطقي requested_kerning : 1;
#وإلا
  ساكن constexpr منطقي requested_kerning = خطأ;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_OT_SHAPE_FRACTIONS
  منطقي has_frac : 1;
#وإلا
  ساكن constexpr منطقي has_frac = خطأ;
#پایان_اگر
  منطقي has_vert : 1;
  منطقي has_gpos_mark : 1;
  منطقي zero_marks : 1;
  منطقي fallback_glyph_classes : 1;
  منطقي fallback_mark_positioning : 1;
  منطقي adjust_mark_positioning_when_zeroing : 1;

  منطقي apply_gpos : 1;
#اگر_تعریف_نشده HB_NO_OT_KERN
  منطقي apply_kern : 1;
#وإلا
  ساكن constexpr منطقي apply_kern = خطأ;
#پایان_اگر
  منطقي apply_fallback_kern : 1;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  منطقي apply_kerx : 1;
  منطقي apply_morx : 1;
  منطقي apply_trak : 1;
#وإلا
  ساكن constexpr منطقي apply_kerx = خطأ;
  ساكن constexpr منطقي apply_morx = خطأ;
  ساكن constexpr منطقي apply_trak = خطأ;
#پایان_اگر

  فراغ collect_lookups (hb_برچسب_t جدول_برچسب_2, hb_مجموعه_t *lookups) ثابت
  {
    بیعلامت صحيح جدول_شاخص;
    بدل (جدول_برچسب_2) {
      حالة HB_OT_برچسب_GSUB: جدول_شاخص = 0; اكسر;
      حالة HB_OT_برچسب_GPOS: جدول_شاخص = 1; اكسر;
      افتراضي: أرجع;
    }
    map.collect_lookups (جدول_شاخص, lookups);
  }

  HB_داخلی منطقي init0 (hb_face_t                     *face,
			  ثابت hb_shape_plan_کلید_t     *key);
  HB_داخلی فراغ fini ();

  HB_داخلی فراغ substitute (hb_font_t *font, hb_میانگیر_t *میانگیر) ثابت;
  HB_داخلی فراغ position (hb_font_t *font, hb_میانگیر_t *میانگیر) ثابت;
};

بنية hb_shape_plan_t;

بنية hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#اگر_تعریف_نشده HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  منطقي apply_morx : 1;
#وإلا
  ساكن constexpr منطقي apply_morx = خطأ;
#پایان_اگر
  منطقي script_zero_marks : 1;
  منطقي script_fallback_position : 1;
  ثابت بنية hb_ot_shaper_t *shaper;

  HB_داخلی hb_ot_shape_planner_t (hb_face_t                     *face,
				     ثابت hb_segment_properties_t &props);

  HB_داخلی فراغ compile (hb_ot_shape_plan_t           &plan,
			    ثابت hb_ot_shape_plan_کلید_t &key);
};


#پایان_اگر /* HB_OT_SHAPE_HH */
