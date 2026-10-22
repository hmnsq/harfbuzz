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

#अन्तर्भाव "hb.hh"


संरचना hb_aat_map_t
{
  मित्र संरचना hb_aat_map_builder_t;

  सार्वजनिक:
  संरचना range_flags_t
  {
    hb_mask_t flags;
    अचिन्हित cluster_first;
    अचिन्हित cluster_last; // end - 1
  };

  सार्वजनिक:
  hb_vector_t<hb_sorted_vector_t<range_flags_t>> chain_flags;
};

संरचना hb_aat_map_builder_t
{
  सार्वजनिक:

  HB_INTERNAL hb_aat_map_builder_t (hb_face_t *face_,
				    अचर hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_INTERNAL शून्य add_feature (अचर hb_feature_t &feature);

  HB_INTERNAL शून्य compile (hb_aat_map_t  &m);

  सार्वजनिक:
  संरचना feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    तार्किक is_exclusive;
    अचिन्हित  seq; /* For stable sorting only. */

    HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
    {
      अचर feature_info_t *a = (अचर feature_info_t *) pa;
      अचर feature_info_t *b = (अचर feature_info_t *) pb;
      यदि (a->type != b->type) फर्काऊ (a->type < b->type ? -1 : 1);
      यदि (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) फर्काऊ (a->setting < b->setting ? -1 : 1);
	    फर्काऊ (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    पूर्णाङ्क cmp (अचर feature_info_t& f) अचर
    {
      फर्काऊ (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  संरचना feature_range_t
  {
    feature_info_t info;
    अचिन्हित start;
    अचिन्हित end;
  };

  गुप्त:
  संरचना feature_event_t
  {
    अचिन्हित पूर्णाङ्क index;
    तार्किक start;
    feature_info_t feature;

    HB_INTERNAL स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb) {
      अचर feature_event_t *a = (अचर feature_event_t *) pa;
      अचर feature_event_t *b = (अचर feature_event_t *) pb;
      फर्काऊ a->index < b->index ? -1 : a->index > b->index ? 1 :
	     a->start < b->start ? -1 : a->start > b->start ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  सार्वजनिक:
  hb_face_t *face;
  hb_segment_properties_t props;

  सार्वजनिक:
  hb_sorted_vector_t<feature_range_t> features;
  hb_sorted_vector_t<feature_info_t> current_features;
  अचिन्हित range_first = HB_FEATURE_GLOBAL_START;
  अचिन्हित range_last = HB_FEATURE_GLOBAL_END;
};


#endif /* HB_AAT_MAP_HH */
