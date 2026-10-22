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

#அந்தர்பாவ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

வகைவரை கட்டமைப்பு hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

வகைவரை கட்டமைப்பு hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         இலக்கம்_features;
} hb_ms_features_t;

கட்டமைப்பு hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned முழு order;

  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb) {
    மாறிலி auto *a = (மாறிலி hb_ms_active_feature_t *) pa;
    மாறிலி auto *b = (மாறிலி hb_ms_active_feature_t *) pb;
    திரும்பு a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  தருக்க operator== (மாறிலி hb_ms_active_feature_t& f) மாறிலி
  { திரும்பு cmp (இது, &f) == 0; }
};

கட்டமைப்பு hb_ms_feature_event_t {
  unsigned முழு index;
  தருக்க start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL நிலையான முழு cmp (மாறிலி வெறுமை *pa, மாறிலி வெறுமை *pb)
  {
    மாறிலி auto *a = (மாறிலி hb_ms_feature_event_t *) pa;
    மாறிலி auto *b = (மாறிலி hb_ms_feature_event_t *) pb;
    திரும்பு a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

கட்டமைப்பு hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned முழு index_first; /* == start */
  unsigned முழு index_last;  /* == end - 1 */
};

நிலையான உள்வரி தருக்க
hb_ms_setup_features (மாறிலி hb_feature_t                *features,
		      unsigned முழு                       இலக்கம்_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  க்கு (unsigned முழு i = 0; i < இலக்கம்_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = உண்மை;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = பொய்;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = இலக்கம்_features + 1;

    auto *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = பொய்;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned முழு last_index = 0;
  க்கு (unsigned முழு i = 0; i < feature_events.length; i++)
  {
    auto *event = &feature_events[i];

    எனில் (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.length;

      active_features.qsort ();
      க்கு (unsigned முழு j = 0; j < active_features.length; j++)
      {
        எனில் (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        இல்லையெனில்
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.இலக்கம்_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    எனில் (event->start)
    {
      active_features.push (event->feature);
    }
    இல்லையெனில்
    {
      auto *feature = active_features.lsearch (event->feature);
      எனில் (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  எனில் (!range_records.length) /* No active feature found. */
    இலக்கம்_features = 0;

  /* Fixup the pointers. */
  க்கு (unsigned முழு i = 0; i < range_records.length; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  திரும்பு !!இலக்கம்_features;
}

நிலையான உள்வரி வெறுமை
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned முழு                       chars_offset,
			   unsigned முழு                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  க்கு (unsigned முழு i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    போது (log_clusters[i] < range->index_first)
      range--;
    போது (log_clusters[i] > range->index_last)
      range++;
    எனில் (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      எனில் (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        நிறுத்து;
      }
      *features = &range->features;
      *c = 1;
    }
    இல்லையெனில்
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
