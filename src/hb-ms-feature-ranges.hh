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

#অন্তর্ভাব "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

প্রকারলেখ কাঠামো hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

প্রকারলেখ কাঠামো hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         অঙ্ক_features;
} hb_ms_features_t;

কাঠামো hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned পূর্ণসংখ্যা order;

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb) {
    ধ্রুবক auto *a = (ধ্রুবক hb_ms_active_feature_t *) pa;
    ধ্রুবক auto *b = (ধ্রুবক hb_ms_active_feature_t *) pb;
    ফেরত a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  যৌক্তিক operator== (ধ্রুবক hb_ms_active_feature_t& f) ধ্রুবক
  { ফেরত cmp (এই, &f) == 0; }
};

কাঠামো hb_ms_feature_event_t {
  unsigned পূর্ণসংখ্যা সূচক_2;
  যৌক্তিক start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb)
  {
    ধ্রুবক auto *a = (ধ্রুবক hb_ms_feature_event_t *) pa;
    ধ্রুবক auto *b = (ধ্রুবক hb_ms_feature_event_t *) pb;
    ফেরত a->সূচক_2 < b->সূচক_2 ? -1 : a->সূচক_2 > b->সূচক_2 ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

কাঠামো hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned পূর্ণসংখ্যা সূচক_first; /* == start */
  unsigned পূর্ণসংখ্যা সূচক_last;  /* == end - 1 */
};

স্থির অন্তঃরেখ যৌক্তিক
hb_ms_setup_features (ধ্রুবক hb_feature_t                *features,
		      unsigned পূর্ণসংখ্যা                       অঙ্ক_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->সূচক_2 = features[i].start;
    event->start = সত্য;
    event->feature = feature;

    event = feature_events.push ();
    event->সূচক_2 = features[i].end;
    event->start = মিথ্যা;
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
    event->সূচক_2 = 0; /* This value does magic. */
    event->start = মিথ্যা;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned পূর্ণসংখ্যা last_সূচক = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < feature_events.দৈর্ঘ্য; i++)
  {
    auto *event = &feature_events[i];

    যদি (event->সূচক_2 != last_সূচক)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.দৈর্ঘ্য;

      active_features.qsort ();
      জন্য (unsigned পূর্ণসংখ্যা j = 0; j < active_features.দৈর্ঘ্য; j++)
      {
        যদি (!j || active_features[j].fea.tag_le != feature_records[feature_records.দৈর্ঘ্য - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        অন্যথা
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.দৈর্ঘ্য - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.অঙ্ক_features = feature_records.দৈর্ঘ্য - offset;
      range->সূচক_first = last_সূচক;
      range->সূচক_last  = event->সূচক_2 - 1;

      last_সূচক = event->সূচক_2;
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

  যদি (!range_records.দৈর্ঘ্য) /* No active feature found. */
    অঙ্ক_features = 0;

  /* Fixup the pointers. */
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < range_records.দৈর্ঘ্য; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  ফেরত !!অঙ্ক_features;
}

স্থির অন্তঃরেখ শূন্য
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned পূর্ণসংখ্যা                       chars_offset,
			   unsigned পূর্ণসংখ্যা                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  জন্য (unsigned পূর্ণসংখ্যা i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    যখন (log_clusters[i] < range->সূচক_first)
      range--;
    যখন (log_clusters[i] > range->সূচক_last)
      range++;
    যদি (!range_features.দৈর্ঘ্য ||
        &range->features != range_features[range_features.দৈর্ঘ্য - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      যদি (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        বিরতি;
      }
      *features = &range->features;
      *c = 1;
    }
    অন্যথা
    {
      range_counts[range_counts.দৈর্ঘ্য - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
