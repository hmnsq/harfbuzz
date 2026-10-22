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

#ಅನ್ತರ್ಭಾವ "hb.hh"


ಸಂರಚನಾ hb_aat_map_t
{
  friend ಸಂರಚನಾ hb_aat_map_builder_t;

  ಸಾರ್ವಜನಿಕ:
  ಸಂರಚನಾ range_flags_t
  {
    hb_mask_t flags;
    unsigned cluster_first;
    unsigned cluster_last; // end - 1
  };

  ಸಾರ್ವಜನಿಕ:
  hb_vector_t<hb_sorted_vector_t<range_flags_t>> chain_flags;
};

ಸಂರಚನಾ hb_aat_map_builder_t
{
  ಸಾರ್ವಜನಿಕ:

  HB_INTERNAL hb_aat_map_builder_t (hb_face_t *face_,
				    ಧ್ರುವ hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_INTERNAL ಶೂನ್ಯ add_feature (ಧ್ರುವ hb_feature_t &feature);

  HB_INTERNAL ಶೂನ್ಯ compile (hb_aat_map_t  &m);

  ಸಾರ್ವಜನಿಕ:
  ಸಂರಚನಾ feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    ತಾರ್ಕಿಕ is_exclusive;
    unsigned  seq; /* For stable sorting only. */

    HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
    {
      ಧ್ರುವ feature_info_t *a = (ಧ್ರುವ feature_info_t *) pa;
      ಧ್ರುವ feature_info_t *b = (ಧ್ರುವ feature_info_t *) pb;
      ಯದಿ (a->type != b->type) ಪ್ರತ್ಯಾಗಮ (a->type < b->type ? -1 : 1);
      ಯದಿ (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) ಪ್ರತ್ಯಾಗಮ (a->setting < b->setting ? -1 : 1);
	    ಪ್ರತ್ಯಾಗಮ (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ feature_info_t& f) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  ಸಂರಚನಾ feature_range_t
  {
    feature_info_t info;
    unsigned start;
    unsigned end;
  };

  ಗುಪ್ತ:
  ಸಂರಚನಾ feature_event_t
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ index;
    ತಾರ್ಕಿಕ start;
    feature_info_t feature;

    HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb) {
      ಧ್ರುವ feature_event_t *a = (ಧ್ರುವ feature_event_t *) pa;
      ಧ್ರುವ feature_event_t *b = (ಧ್ರುವ feature_event_t *) pb;
      ಪ್ರತ್ಯಾಗಮ a->index < b->index ? -1 : a->index > b->index ? 1 :
	     a->start < b->start ? -1 : a->start > b->start ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  ಸಾರ್ವಜನಿಕ:
  hb_face_t *face;
  hb_segment_properties_t props;

  ಸಾರ್ವಜನಿಕ:
  hb_sorted_vector_t<feature_range_t> features;
  hb_sorted_vector_t<feature_info_t> current_features;
  unsigned range_first = HB_FEATURE_GLOBAL_START;
  unsigned range_last = HB_FEATURE_GLOBAL_END;
};


#endif /* HB_AAT_MAP_HH */
