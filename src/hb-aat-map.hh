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

#ifndef HB_AAT_MAP_HH
#define HB_AAT_MAP_HH

#অন্তর্ভাব "hb.hh"


সংরচনা hb_aat_map_t
{
  friend সংরচনা hb_aat_map_builder_t;

  সার্বজনিক:
  সংরচনা range_flags_t
  {
    hb_mask_t flags;
    unsigned cluster_first;
    unsigned cluster_last; // end - 1
  };

  সার্বজনিক:
  hb_vector_t<hb_sorted_vector_t<range_flags_t>> chain_flags;
};

সংরচনা hb_aat_map_builder_t
{
  সার্বজনিক:

  HB_INTERNAL hb_aat_map_builder_t (hb_face_t *face_,
				    ধ্রুব hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_INTERNAL শূন্য add_feature (ধ্রুব hb_feature_t &feature);

  HB_INTERNAL শূন্য compile (hb_aat_map_t  &m);

  সার্বজনিক:
  সংরচনা feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    তার্কিক is_exclusive;
    unsigned  seq; /* For stable sorting only. */

    HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb)
    {
      ধ্রুব feature_info_t *a = (ধ্রুব feature_info_t *) pa;
      ধ্রুব feature_info_t *b = (ধ্রুব feature_info_t *) pb;
      যদি (a->type != b->type) প্রত্যাগম (a->type < b->type ? -1 : 1);
      যদি (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) প্রত্যাগম (a->setting < b->setting ? -1 : 1);
	    প্রত্যাগম (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    পূর্ণাঙ্ক cmp (ধ্রুব feature_info_t& f) ধ্রুব
    {
      প্রত্যাগম (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  সংরচনা feature_range_t
  {
    feature_info_t info;
    unsigned start;
    unsigned end;
  };

  গুপ্ত:
  সংরচনা feature_event_t
  {
    unsigned পূর্ণাঙ্ক index;
    তার্কিক start;
    feature_info_t feature;

    HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুব শূন্য *pa, ধ্রুব শূন্য *pb) {
      ধ্রুব feature_event_t *a = (ধ্রুব feature_event_t *) pa;
      ধ্রুব feature_event_t *b = (ধ্রুব feature_event_t *) pb;
      প্রত্যাগম a->index < b->index ? -1 : a->index > b->index ? 1 :
	     a->start < b->start ? -1 : a->start > b->start ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  সার্বজনিক:
  hb_face_t *face;
  hb_segment_properties_t props;

  সার্বজনিক:
  hb_sorted_vector_t<feature_range_t> features;
  hb_sorted_vector_t<feature_info_t> current_features;
  unsigned range_first = HB_FEATURE_GLOBAL_START;
  unsigned range_last = HB_FEATURE_GLOBAL_END;
};


#endif /* HB_AAT_MAP_HH */
