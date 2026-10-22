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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_AAT_MAP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_AAT_MAP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


ਬਣਤਰ hb_aat_map_t
{
  ਮਿੱਤਰ ਬਣਤਰ hb_aat_map_builder_t;

  ਜਨਤਕ:
  ਬਣਤਰ range_flags_t
  {
    hb_ਨਕਾਬ_t flags;
    ਬਿਨਾਨਿਸ਼ਾਨੀ cluster_ਪਹਿਲਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ cluster_ਆਖਰੀ; // end - 1
  };

  ਜਨਤਕ:
  hb_ਸਦਿਸ਼_t<hb_sorted_ਸਦਿਸ਼_t<range_flags_t>> chain_flags;
};

ਬਣਤਰ hb_aat_map_builder_t
{
  ਜਨਤਕ:

  HB_ਅੰਦਰੂਨੀ hb_aat_map_builder_t (hb_face_t *face_,
				    ਅਚਲ hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ ਜੋੜੋ_feature (ਅਚਲ hb_feature_t &feature);

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ compile (hb_aat_map_t  &m);

  ਜਨਤਕ:
  ਬਣਤਰ feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    ਤਾਰਕਿਕ is_exclusive;
    ਬਿਨਾਨਿਸ਼ਾਨੀ  seq; /* For stable sorting only. */

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
    {
      ਅਚਲ feature_info_t *a = (ਅਚਲ feature_info_t *) pa;
      ਅਚਲ feature_info_t *b = (ਅਚਲ feature_info_t *) pb;
      ਜੇ (a->type != b->type) ਵਾਪਸ (a->type < b->type ? -1 : 1);
      ਜੇ (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) ਵਾਪਸ (a->setting < b->setting ? -1 : 1);
	    ਵਾਪਸ (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    ਪੂਰਨ cmp (ਅਚਲ feature_info_t& f) ਅਚਲ
    {
      ਵਾਪਸ (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  ਬਣਤਰ feature_range_t
  {
    feature_info_t info;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ;
  };

  ਨਿਜੀ:
  ਬਣਤਰ feature_event_t
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
    ਤਾਰਕਿਕ ਸ਼ੁਰੂ;
    feature_info_t feature;

    HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb) {
      ਅਚਲ feature_event_t *a = (ਅਚਲ feature_event_t *) pa;
      ਅਚਲ feature_event_t *b = (ਅਚਲ feature_event_t *) pb;
      ਵਾਪਸ a->ਸੂਚਕਾਂਕ_2 < b->ਸੂਚਕਾਂਕ_2 ? -1 : a->ਸੂਚਕਾਂਕ_2 > b->ਸੂਚਕਾਂਕ_2 ? 1 :
	     a->ਸ਼ੁਰੂ < b->ਸ਼ੁਰੂ ? -1 : a->ਸ਼ੁਰੂ > b->ਸ਼ੁਰੂ ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  ਜਨਤਕ:
  hb_face_t *face;
  hb_segment_properties_t props;

  ਜਨਤਕ:
  hb_sorted_ਸਦਿਸ਼_t<feature_range_t> features;
  hb_sorted_ਸਦਿਸ਼_t<feature_info_t> current_features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ range_ਪਹਿਲਾ = HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ range_ਆਖਰੀ = HB_FEATURE_GLOBAL_ਸਿਰਾ;
};


#ਜੇ_ਅੰਤ /* HB_AAT_MAP_HH */
