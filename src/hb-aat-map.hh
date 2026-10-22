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

#ଅନ୍ତର୍ଭାଵ "hb.hh"


ସଂରଚନା hb_aat_map_t
{
  friend ସଂରଚନା hb_aat_map_builder_t;

  ସର୍ବସାଧାରଣ:
  ସଂରଚନା range_flags_t
  {
    hb_mask_t flags;
    unsigned cluster_first;
    unsigned cluster_last; // end - 1
  };

  ସର୍ବସାଧାରଣ:
  hb_vector_t<hb_sorted_vector_t<range_flags_t>> chain_flags;
};

ସଂରଚନା hb_aat_map_builder_t
{
  ସର୍ବସାଧାରଣ:

  HB_INTERNAL hb_aat_map_builder_t (hb_face_t *face_,
				    ଅଚଳ hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_INTERNAL ଶୂନ୍ୟ add_feature (ଅଚଳ hb_feature_t &feature);

  HB_INTERNAL ଶୂନ୍ୟ compile (hb_aat_map_t  &m);

  ସର୍ବସାଧାରଣ:
  ସଂରଚନା feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    ତାର୍କିକ is_exclusive;
    unsigned  seq; /* For stable sorting only. */

    HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb)
    {
      ଅଚଳ feature_info_t *a = (ଅଚଳ feature_info_t *) pa;
      ଅଚଳ feature_info_t *b = (ଅଚଳ feature_info_t *) pb;
      ଯଦି (a->type != b->type) ଫେରନ୍ତୁ (a->type < b->type ? -1 : 1);
      ଯଦି (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) ଫେରନ୍ତୁ (a->setting < b->setting ? -1 : 1);
	    ଫେରନ୍ତୁ (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ feature_info_t& f) ଅଚଳ
    {
      ଫେରନ୍ତୁ (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  ସଂରଚନା feature_range_t
  {
    feature_info_t info;
    unsigned start;
    unsigned end;
  };

  ବ୍ୟକ୍ତିଗତ:
  ସଂରଚନା feature_event_t
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
    ତାର୍କିକ start;
    feature_info_t feature;

    HB_INTERNAL ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ *pa, ଅଚଳ ଶୂନ୍ୟ *pb) {
      ଅଚଳ feature_event_t *a = (ଅଚଳ feature_event_t *) pa;
      ଅଚଳ feature_event_t *b = (ଅଚଳ feature_event_t *) pb;
      ଫେରନ୍ତୁ a->index < b->index ? -1 : a->index > b->index ? 1 :
	     a->start < b->start ? -1 : a->start > b->start ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  ସର୍ବସାଧାରଣ:
  hb_face_t *face;
  hb_segment_properties_t props;

  ସର୍ବସାଧାରଣ:
  hb_sorted_vector_t<feature_range_t> features;
  hb_sorted_vector_t<feature_info_t> current_features;
  unsigned range_first = HB_FEATURE_GLOBAL_START;
  unsigned range_last = HB_FEATURE_GLOBAL_END;
};


#endif /* HB_AAT_MAP_HH */
