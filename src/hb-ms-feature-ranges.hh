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

#include "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

typedef sa_nracanaa hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

typedef sa_nracanaa hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         num_features;
} hb_ms_features_t;

sa_nracanaa hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned ཧྲིལ order;

  HB_INTERNAL static ཧྲིལ cmp (const void *pa, const void *pb) {
    const auto *a = (const hb_ms_active_feature_t *) pa;
    const auto *b = (const hb_ms_active_feature_t *) pb;
    ཕྱིར a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  bool operator== (const hb_ms_active_feature_t& f) const
  { ཕྱིར cmp (this, &f) == 0; }
};

sa_nracanaa hb_ms_feature_event_t {
  unsigned ཧྲིལ index;
  bool start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL static ཧྲིལ cmp (const void *pa, const void *pb)
  {
    const auto *a = (const hb_ms_feature_event_t *) pa;
    const auto *b = (const hb_ms_feature_event_t *) pb;
    ཕྱིར a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

sa_nracanaa hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned ཧྲིལ index_first; /* == start */
  unsigned ཧྲིལ index_last;  /* == end - 1 */
};

static anta_hstha bool
hb_ms_setup_features (const hb_feature_t                *features,
		      unsigned ཧྲིལ                       num_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  ཆེད་དུ (unsigned ཧྲིལ i = 0; i < num_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = true;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = false;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = num_features + 1;

    auto *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = false;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned ཧྲིལ last_index = 0;
  ཆེད་དུ (unsigned ཧྲིལ i = 0; i < feature_events.length; i++)
  {
    auto *event = &feature_events[i];

    གལ་ཏེ (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.length;

      active_features.qsort ();
      ཆེད་དུ (unsigned ཧྲིལ j = 0; j < active_features.length; j++)
      {
        གལ་ཏེ (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        དེ་མིན
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.num_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    གལ་ཏེ (event->start)
    {
      active_features.push (event->feature);
    }
    དེ་མིན
    {
      auto *feature = active_features.lsearch (event->feature);
      གལ་ཏེ (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  གལ་ཏེ (!range_records.length) /* No active feature found. */
    num_features = 0;

  /* Fixup the pointers. */
  ཆེད་དུ (unsigned ཧྲིལ i = 0; i < range_records.length; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  ཕྱིར !!num_features;
}

static anta_hstha void
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned ཧྲིལ                       chars_offset,
			   unsigned ཧྲིལ                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  ཆེད་དུ (unsigned ཧྲིལ i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    སྐབས (log_clusters[i] < range->index_first)
      range--;
    སྐབས (log_clusters[i] > range->index_last)
      range++;
    གལ་ཏེ (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      གལ་ཏེ (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        bha_nga;
      }
      *features = &range->features;
      *c = 1;
    }
    དེ་མིན
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
