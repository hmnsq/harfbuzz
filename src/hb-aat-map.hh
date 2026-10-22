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

#اگر_غیر_تعریف_شدہ HB_AAT_MAP_HH
#تعریف_کرو HB_AAT_MAP_HH

#شامل "hb.hh"


ساخت hb_aat_map_t
{
  دوست ساخت hb_aat_map_builder_t;

  عوامی:
  ساخت range_flags_t
  {
    hb_نقاب_t flags;
    غیرعلامتی cluster_پہلا;
    غیرعلامتی cluster_آخری; // end - 1
  };

  عوامی:
  hb_سمتیہ_t<hb_sorted_سمتیہ_t<range_flags_t>> chain_flags;
};

ساخت hb_aat_map_builder_t
{
  عوامی:

  HB_اندرونی hb_aat_map_builder_t (hb_face_t *face_,
				    ثابت hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_اندرونی خالی جوڑو_feature (ثابت hb_feature_t &feature);

  HB_اندرونی خالی compile (hb_aat_map_t  &m);

  عوامی:
  ساخت feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    منطقی is_exclusive;
    غیرعلامتی  seq; /* For stable sorting only. */

    HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
    {
      ثابت feature_info_t *a = (ثابت feature_info_t *) pa;
      ثابت feature_info_t *b = (ثابت feature_info_t *) pb;
      اگر (a->type != b->type) واپس (a->type < b->type ? -1 : 1);
      اگر (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) واپس (a->setting < b->setting ? -1 : 1);
	    واپس (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    صحیح cmp (ثابت feature_info_t& f) ثابت
    {
      واپس (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  ساخت feature_range_t
  {
    feature_info_t info;
    غیرعلامتی آغاز;
    غیرعلامتی انتہا;
  };

  نجی:
  ساخت feature_event_t
  {
    غیرعلامتی صحیح اشاریہ_2;
    منطقی آغاز;
    feature_info_t feature;

    HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb) {
      ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
      ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
      واپس a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 :
	     a->آغاز < b->آغاز ? -1 : a->آغاز > b->آغاز ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  عوامی:
  hb_face_t *face;
  hb_segment_properties_t props;

  عوامی:
  hb_sorted_سمتیہ_t<feature_range_t> features;
  hb_sorted_سمتیہ_t<feature_info_t> current_features;
  غیرعلامتی range_پہلا = HB_FEATURE_GLOBAL_آغاز;
  غیرعلامتی range_آخری = HB_FEATURE_GLOBAL_انتہا;
};


#اگر_ختم /* HB_AAT_MAP_HH */
