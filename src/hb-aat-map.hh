/*
 * Copyright © 2018  Google, Inc.
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

#اگر_تعریف_نشده HB_AAT_MAP_HH
#تعریف_کن HB_AAT_MAP_HH

#ضمن "hb.hh"


بنية hb_aat_map_t
{
  دوست بنية hb_aat_map_builder_t;

  عام:
  بنية range_flags_t
  {
    hb_ماسک_t flags;
    بیعلامت cluster_اولی;
    بیعلامت cluster_آخرین; // end - 1
  };

  عام:
  hb_وکتور_t<hb_sorted_وکتور_t<range_flags_t>> chain_flags;
};

بنية hb_aat_map_builder_t
{
  عام:

  HB_داخلی hb_aat_map_builder_t (hb_face_t *face_,
				    ثابت hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_داخلی فراغ جمع_کن_feature (ثابت hb_feature_t &feature);

  HB_داخلی فراغ compile (hb_aat_map_t  &m);

  عام:
  بنية feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    منطقي is_exclusive;
    بیعلامت  seq; /* For stable sorting only. */

    HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      إذا (a->type != b->type) أرجع (a->type < b->type ? -1 : 1);
      إذا (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) أرجع (a->setting < b->setting ? -1 : 1);
	    أرجع (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    صحيح cmp (ثابت feature_info_t& f) ثابت
    {
      أرجع (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  بنية feature_range_t
  {
    feature_info_t info;
    بیعلامت شروع;
    بیعلامت انتها;
  };

  خاص:
  بنية feature_event_t
  {
    بیعلامت صحيح شاخص_2;
    منطقي شروع;
    feature_info_t feature;

    HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
      ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
      ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
      أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 :
	     a->شروع < b->شروع ? -1 : a->شروع > b->شروع ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  عام:
  hb_face_t *face;
  hb_segment_properties_t props;

  عام:
  hb_sorted_وکتور_t<feature_range_t> features;
  hb_sorted_وکتور_t<feature_info_t> current_features;
  بیعلامت range_اولی = HB_FEATURE_GLOBAL_شروع;
  بیعلامت range_آخرین = HB_FEATURE_GLOBAL_انتها;
};


#پایان_اگر /* HB_AAT_MAP_HH */
