/*
 * Copyright © 2011,2012,2013  Google, Inc.
 * Copyright © 2021  Khaled Hosny
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

#ifndef HB_MS_FEATURE_RANGES_HH
#define HB_MS_FEATURE_RANGES_HH

#অন্তর্ভাৱ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

প্রকারনির্দেশ সংরচনা hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

প্রকারনির্দেশ সংরচনা hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         অঙ্ক_features;
} hb_ms_features_t;

সংরচনা hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned পূর্ণাঙ্ক order;

  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুৱ শূন্য *pa, ধ্রুৱ শূন্য *pb) {
    ধ্রুৱ auto *a = (ধ্রুৱ hb_ms_active_feature_t *) pa;
    ধ্রুৱ auto *b = (ধ্রুৱ hb_ms_active_feature_t *) pb;
    ঘূৰাই a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  তার্কিক operator== (ধ্রুৱ hb_ms_active_feature_t& f) ধ্রুৱ
  { ঘূৰাই cmp (এতত্, &f) == 0; }
};

সংরচনা hb_ms_feature_event_t {
  unsigned পূর্ণাঙ্ক index;
  তার্কিক start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL স্থির পূর্ণাঙ্ক cmp (ধ্রুৱ শূন্য *pa, ধ্রুৱ শূন্য *pb)
  {
    ধ্রুৱ auto *a = (ধ্রুৱ hb_ms_feature_event_t *) pa;
    ধ্রুৱ auto *b = (ধ্রুৱ hb_ms_feature_event_t *) pb;
    ঘূৰাই a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

সংরচনা hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned পূর্ণাঙ্ক index_first; /* == start */
  unsigned পূর্ণাঙ্ক index_last;  /* == end - 1 */
};

স্থির অন্তঃস্থ তার্কিক
hb_ms_setup_features (ধ্রুৱ hb_feature_t                *features,
		      unsigned পূর্ণাঙ্ক                       অঙ্ক_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = সঁচা;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = মিছা;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = অঙ্ক_features + 1;

    auto *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = মিছা;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned পূর্ণাঙ্ক last_index = 0;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < feature_events.length; i++)
  {
    auto *event = &feature_events[i];

    যদি (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.length;

      active_features.qsort ();
      বাবে (unsigned পূর্ণাঙ্ক j = 0; j < active_features.length; j++)
      {
        যদি (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        অন্যথা
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.অঙ্ক_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    যদি (event->start)
    {
      active_features.push (event->feature);
    }
    অন্যথা
    {
      auto *feature = active_features.lsearch (event->feature);
      যদি (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  যদি (!range_records.length) /* No active feature found. */
    অঙ্ক_features = 0;

  /* Fixup the pointers. */
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < range_records.length; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  ঘূৰাই !!অঙ্ক_features;
}

স্থির অন্তঃস্থ শূন্য
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned পূর্ণাঙ্ক                       chars_offset,
			   unsigned পূর্ণাঙ্ক                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  বাবে (unsigned পূর্ণাঙ্ক i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    যেতিয়ালৈ (log_clusters[i] < range->index_first)
      range--;
    যেতিয়ালৈ (log_clusters[i] > range->index_last)
      range++;
    যদি (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      যদি (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        ভঙ্গ;
      }
      *features = &range->features;
      *c = 1;
    }
    অন্যথা
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
