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

#अगर_अघोषित HB_MS_FEATURE_RANGES_HH
#घोषणा HB_MS_FEATURE_RANGES_HH

#समावेश "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

प्रकार काष्ठा hb_ms_feature_t {
  uint32_t tag_le;
  uint32_t value;
} hb_ms_feature_t;

प्रकार काष्ठा hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         अंक_features;
} hb_ms_features_t;

काष्ठा hb_ms_active_feature_t {
  hb_ms_feature_t fea;
  अचिन्हित पूर्णांक order;

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb) {
    स्थिर स्वतः *a = (स्थिर hb_ms_active_feature_t *) pa;
    स्थिर स्वतः *b = (स्थिर hb_ms_active_feature_t *) pb;
    वापस a->fea.tag_le < b->fea.tag_le ? -1 : a->fea.tag_le > b->fea.tag_le ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  bool चालक== (स्थिर hb_ms_active_feature_t& f) स्थिर
  { वापस cmp (यह, &f) == 0; }
};

काष्ठा hb_ms_feature_event_t {
  अचिन्हित पूर्णांक सूचकांक_2;
  bool start;
  hb_ms_active_feature_t feature;

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर स्वतः *a = (स्थिर hb_ms_feature_event_t *) pa;
    स्थिर स्वतः *b = (स्थिर hb_ms_feature_event_t *) pb;
    वापस a->सूचकांक_2 < b->सूचकांक_2 ? -1 : a->सूचकांक_2 > b->सूचकांक_2 ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   hb_ms_active_feature_t::cmp (&a->feature, &b->feature);
  }
};

काष्ठा hb_ms_range_record_t {
  hb_ms_features_t features;
  अचिन्हित पूर्णांक सूचकांक_first; /* == start */
  अचिन्हित पूर्णांक सूचकांक_last;  /* == end - 1 */
};

अटल अंतरभूत bool
hb_ms_setup_features (स्थिर hb_feature_t                *features,
		      अचिन्हित पूर्णांक                       अंक_features,
		      hb_vector_t<hb_ms_feature_t>      &feature_records, /* OUT */
		      hb_vector_t<hb_ms_range_record_t> &range_records /* OUT */)
{
  feature_records.सफा ();
  range_records.सफा ();

  /* Sort features by start/end events. */
  hb_vector_t<hb_ms_feature_event_t> feature_events;
  क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
  {
    hb_ms_active_feature_t feature;
    feature.fea.tag_le = hb_uint32_swap (features[i].tag);
    feature.fea.value = features[i].value;
    feature.order = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->सूचकांक_2 = features[i].start;
    event->start = true;
    event->feature = feature;

    event = feature_events.push ();
    event->सूचकांक_2 = features[i].end;
    event->start = false;
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
    event->सूचकांक_2 = 0; /* This value does magic. */
    event->start = false;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_vector_t<hb_ms_active_feature_t> active_features;
  अचिन्हित पूर्णांक last_सूचकांक = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < feature_events.लंबाई; i++)
  {
    स्वतः *event = &feature_events[i];

    अगर (event->सूचकांक_2 != last_सूचकांक)
    {
      /* Save a snapshot of active features and the range. */
      स्वतः *range = range_records.push ();
      स्वतः offset = feature_records.लंबाई;

      active_features.क्विक ();
      क्रम (अचिन्हित पूर्णांक j = 0; j < active_features.लंबाई; j++)
      {
        अगर (!j || active_features[j].fea.tag_le != feature_records[feature_records.लंबाई - 1].tag_le)
        {
          feature_records.push (active_features[j].fea);
        }
        अन्यथा
        {
          /* Overrides value for existing feature. */
          feature_records[feature_records.लंबाई - 1].value = active_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (offset);
      range->features.अंक_features = feature_records.लंबाई - offset;
      range->सूचकांक_first = last_सूचकांक;
      range->सूचकांक_last  = event->सूचकांक_2 - 1;

      last_सूचकांक = event->सूचकांक_2;
    }

    अगर (event->start)
    {
      active_features.push (event->feature);
    }
    अन्यथा
    {
      स्वतः *feature = active_features.lsearch (event->feature);
      अगर (feature)
        active_features.remove_ordered (feature - active_features.arrayZ);
    }
  }

  अगर (!range_records.लंबाई) /* No active feature found. */
    अंक_features = 0;

  /* Fixup the pointers. */
  क्रम (अचिन्हित पूर्णांक i = 0; i < range_records.लंबाई; i++)
  {
    स्वतः *range = &range_records[i];
    range->features.features = (hb_ms_feature_t *) feature_records + reinterpret_cast<uintptr_t> (range->features.features);
  }

  वापस !!अंक_features;
}

अटल अंतरभूत व्योम
hb_ms_make_feature_ranges (hb_vector_t<hb_ms_feature_t>      &feature_records,
			   hb_vector_t<hb_ms_range_record_t> &range_records,
			   अचिन्हित पूर्णांक                       chars_offset,
			   अचिन्हित पूर्णांक                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_vector_t<hb_ms_features_t*>    &range_features, /* OUT */
			   hb_vector_t<uint32_t>             &range_counts /* OUT */)
{
  range_features.सफा ();
  range_counts.सफा ();

  स्वतः *last_range = &range_records[0];
  क्रम (अचिन्हित पूर्णांक i = chars_offset; i < chars_len; i++)
  {
    स्वतः *range = last_range;
    जबतक (log_clusters[i] < range->सूचकांक_first)
      range--;
    जबतक (log_clusters[i] > range->सूचकांक_last)
      range++;
    अगर (!range_features.लंबाई ||
        &range->features != range_features[range_features.लंबाई - 1])
    {
      स्वतः **features = range_features.push ();
      स्वतः *c = range_counts.push ();
      अगर (unlikely (!features || !c))
      {
        range_features.सफा ();
        range_counts.सफा ();
        अवरोध;
      }
      *features = &range->features;
      *c = 1;
    }
    अन्यथा
    {
      range_counts[range_counts.लंबाई - 1]++;
    }

    last_range = range;
  }
}

#पूर्ण_अगर /* HB_MS_FEATURE_RANGES_HH */
