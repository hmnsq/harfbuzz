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

#അന്തര്ഭാവ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

തരനിർവചനം ഘടന hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

തരനിർവചനം ഘടന hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         അക്കം_features;
} hb_ms_features_t;

ഘടന hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  unsigned പൂർണ്ണസംഖ്യ order;

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb) {
    സ്ഥിരം auto *a = (സ്ഥിരം hb_ms_active_feature_t *) pa;
    സ്ഥിരം auto *b = (സ്ഥിരം hb_ms_active_feature_t *) pb;
    തിരികെ a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  തർക്കം operator== (സ്ഥിരം hb_ms_active_feature_t& f) സ്ഥിരം
  { തിരികെ cmp (ഇത്, &f) == 0; }
};

ഘടന hb_ms_feature_event_t {
  unsigned പൂർണ്ണസംഖ്യ index;
  തർക്കം start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    സ്ഥിരം auto *a = (സ്ഥിരം hb_ms_feature_event_t *) pa;
    സ്ഥിരം auto *b = (സ്ഥിരം hb_ms_feature_event_t *) pb;
    തിരികെ a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

ഘടന hb_ms_range_record_t {
  hb_ms_features_t features;
  unsigned പൂർണ്ണസംഖ്യ index_first; /* == start */
  unsigned പൂർണ്ണസംഖ്യ index_last;  /* == end - 1 */
};

സ്ഥിതം വരിയിൽ തർക്കം
hb_ms_setup_features (സ്ഥിരം hb_feature_t                *features,
		      unsigned പൂർണ്ണസംഖ്യ                       അക്കം_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = സത്യം;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = അസത്യം;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = അക്കം_features + 1;

    auto *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = അസത്യം;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  unsigned പൂർണ്ണസംഖ്യ last_index = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < feature_events.length; i++)
  {
    auto *event = &feature_events[i];

    എങ്കിൽ (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      auto *range = range_records.push ();
      auto offset = feature_records.length;

      active_features.qsort ();
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < active_features.length; j++)
      {
        എങ്കിൽ (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        അല്ലെങ്കിൽ
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.അക്കം_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    എങ്കിൽ (event->start)
    {
      active_features.push (event->feature);
    }
    അല്ലെങ്കിൽ
    {
      auto *feature = active_features.lsearch (event->feature);
      എങ്കിൽ (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  എങ്കിൽ (!range_records.length) /* No active feature found. */
    അക്കം_features = 0;

  /* Fixup the pointers. */
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < range_records.length; i++)
  {
    auto *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  തിരികെ !!അക്കം_features;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   unsigned പൂർണ്ണസംഖ്യ                       chars_offset,
			   unsigned പൂർണ്ണസംഖ്യ                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  auto *last_range = &range_records[0];
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = chars_offset; i < chars_len; i++)
  {
    auto *range = last_range;
    ആകുമ്പോൾ (log_clusters[i] < range->index_first)
      range--;
    ആകുമ്പോൾ (log_clusters[i] > range->index_last)
      range++;
    എങ്കിൽ (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      auto **features = range_features.push ();
      auto *c = range_counts.push ();
      എങ്കിൽ (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        നിർത്തുക;
      }
      *features = &range->features;
      *c = 1;
    }
    അല്ലെങ്കിൽ
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
