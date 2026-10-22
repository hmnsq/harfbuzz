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

#ಅನ್ತರ್ಭಾವ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         ಅಂಕೆ_features;
} hb_ms_features_t;

ಸಂರಚನಾ hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned ಪೂರ್ಣಾಙ್ಕ order;

  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb) {
    ಧ್ರುವ auto *a = (ಧ್ರುವ hb_ms_active_feature_t *) pa;
    ಧ್ರುವ auto *b = (ಧ್ರುವ hb_ms_active_feature_t *) pb;
    ಪ್ರತ್ಯಾಗಮ a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  ತಾರ್ಕಿಕ operator== (ಧ್ರುವ hb_ms_active_feature_t& f) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ cmp (ಏತತ್, &f) == 0; }
};

ಸಂರಚನಾ hb_ms_feature_event_t {
  unsigned ಪೂರ್ಣಾಙ್ಕ index;
  ತಾರ್ಕಿಕ start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ ಶೂನ್ಯ *pa, ಧ್ರುವ ಶೂನ್ಯ *pb)
  {
    ಧ್ರುವ auto *a = (ಧ್ರುವ hb_ms_feature_event_t *) pa;
    ಧ್ರುವ auto *b = (ಧ್ರುವ hb_ms_feature_event_t *) pb;
    ಪ್ರತ್ಯಾಗಮ a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

ಸಂರಚನಾ hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned ಪೂರ್ಣಾಙ್ಕ index_first; /* == start */
  unsigned ಪೂರ್ಣಾಙ್ಕ index_last;  /* == end - 1 */
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
hb_ms_setup_features (ಧ್ರುವ hb_feature_t                *features,
		      unsigned ಪೂರ್ಣಾಙ್ಕ                       ಅಂಕೆ_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = ಸತ್ಯ;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = ಅಸತ್ಯ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = ಅಂಕೆ_features + 1;

    auto *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = ಅಸತ್ಯ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned ಪೂರ್ಣಾಙ್ಕ last_index = 0;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < feature_events.length; i++)
  {
    auto *event = &feature_events[i];

    ಯದಿ (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.length;

      active_features.qsort ();
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < active_features.length; j++)
      {
        ಯದಿ (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        ಅನ್ಯಥಾ
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.ಅಂಕೆ_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    ಯದಿ (event->start)
    {
      active_features.push (event->feature);
    }
    ಅನ್ಯಥಾ
    {
      auto *feature = active_features.lsearch (event->feature);
      ಯದಿ (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  ಯದಿ (!range_records.length) /* No active feature found. */
    ಅಂಕೆ_features = 0;

  /* Fixup the pointers. */
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < range_records.length; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  ಪ್ರತ್ಯಾಗಮ !!ಅಂಕೆ_features;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned ಪೂರ್ಣಾಙ್ಕ                       chars_offset,
			   unsigned ಪೂರ್ಣಾಙ್ಕ                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    ಯಾವತ್ (log_clusters[i] < range->index_first)
      range--;
    ಯಾವತ್ (log_clusters[i] > range->index_last)
      range++;
    ಯದಿ (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      ಯದಿ (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        ಭಙ್ಗ;
      }
      *features = &range->features;
      *c = 1;
    }
    ಅನ್ಯಥಾ
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
