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

#अन्तर्भाव "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

प्रकारनिर्देश संरचना hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

प्रकारनिर्देश संरचना hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         अंक_features;
} hb_ms_features_t;

संरचना hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  अचिन्हित पूर्णाङ्क order;

  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb) {
    ध्रुव स्वतः *a = (ध्रुव hb_ms_active_feature_t *) pa;
    ध्रुव स्वतः *b = (ध्रुव hb_ms_active_feature_t *) pb;
    प्रत्यागम a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  तार्किक चालक== (ध्रुव hb_ms_active_feature_t& f) ध्रुव
  { प्रत्यागम cmp (एतत्, &f) == 0; }
};

संरचना hb_ms_feature_event_t {
  अचिन्हित पूर्णाङ्क index;
  तार्किक start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL स्थिर पूर्णाङ्क cmp (ध्रुव शून्य *pa, ध्रुव शून्य *pb)
  {
    ध्रुव स्वतः *a = (ध्रुव hb_ms_feature_event_t *) pa;
    ध्रुव स्वतः *b = (ध्रुव hb_ms_feature_event_t *) pb;
    प्रत्यागम a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

संरचना hb_ms_range_record_t {
  hb_ms_features_t features;
  अचिन्हित पूर्णाङ्क index_first; /* == start */
  अचिन्हित पूर्णाङ्क index_last;  /* == end - 1 */
};

स्थिर अन्तःस्थ तार्किक
hb_ms_setup_features (ध्रुव hb_feature_t                *features,
		      अचिन्हित पूर्णाङ्क                       अंक_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.clear ();
  range_records.clear ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->index = features[i].start;
    event->start = सत्य;
    event->feature = feature;

    event = feature_events.push ();
    event->index = features[i].end;
    event->start = असत्य;
    event->feature = feature;
  }
  feature_events.क्विक ();
  /* Add a strategic final event. */
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = 0;
    feature.fea.value = 0;
    feature.order = अंक_features + 1;

    स्वतः *event = feature_events.push ();
    event->index = 0; /* This value does magic. */
    event->start = असत्य;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  अचिन्हित पूर्णाङ्क last_index = 0;
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < feature_events.length; i++)
  {
    स्वतः *event = &feature_events[i];

    यदि (event->index != last_index)
    {
      /* Save a snapshot of active features and the range. */
      स्वतः *range = range_records.push ();
      स्वतः offset = feature_records.length;

      active_features.क्विक ();
      प्रति (अचिन्हित पूर्णाङ्क j = 0; j < active_features.length; j++)
      {
        यदि (!j || active_features[j].fea.tag_le != feature_records[feature_records.length - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        अन्यथा
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.length - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.अंक_features = feature_records.length - offset;
      range->index_first = last_index;
      range->index_last  = event->index - 1;

      last_index = event->index;
    }

    यदि (event->start)
    {
      active_features.push (event->feature);
    }
    अन्यथा
    {
      स्वतः *feature = active_features.lsearch (event->feature);
      यदि (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  यदि (!range_records.length) /* No active feature found. */
    अंक_features = 0;

  /* Fixup the pointers. */
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < range_records.length; i++)
  {
    स्वतः *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  प्रत्यागम !!अंक_features;
}

स्थिर अन्तःस्थ शून्य
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   अचिन्हित पूर्णाङ्क                       chars_offset,
			   अचिन्हित पूर्णाङ्क                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.clear ();
  range_counts.clear ();

  स्वतः *last_range = &range_records[0];
  प्रति (अचिन्हित पूर्णाङ्क i = chars_offset; i < chars_len; i++)
  {
    स्वतः *range = last_range;
    यावत् (log_clusters[i] < range->index_first)
      range--;
    यावत् (log_clusters[i] > range->index_last)
      range++;
    यदि (!range_features.length ||
        &range->features != range_features[range_features.length - 1])
    {
      स्वतः **features = range_features.push ();
      स्वतः *c = range_counts.push ();
      यदि (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        भङ्ग;
      }
      *features = &range->features;
      *c = 1;
    }
    अन्यथा
    {
      range_counts[range_counts.length - 1]++;
    }

    last_range = range;
  }
}

#endif /* HB_MS_FEATURE_RANGES_HH */
