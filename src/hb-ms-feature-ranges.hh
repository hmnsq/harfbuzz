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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_MS_FEATURE_RANGES_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MS_FEATURE_RANGES_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ms_feature_t {
  uint32_t ਟੈਗ_le;
  uint32_t value;
} hb_ms_feature_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         ਅੰਕ_features;
} hb_ms_features_t;

ਬਣਤਰ hb_ms_ਸਰਗਰਮ_feature_t {
  hb_ms_feature_t fea;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕ੍ਰਮ;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb) {
    ਅਚਲ ਸਵੈਚਾਲਿਤ *a = (ਅਚਲ hb_ms_ਸਰਗਰਮ_feature_t *) pa;
    ਅਚਲ ਸਵੈਚਾਲਿਤ *b = (ਅਚਲ hb_ms_ਸਰਗਰਮ_feature_t *) pb;
    ਵਾਪਸ a->fea.ਟੈਗ_le < b->fea.ਟੈਗ_le ? -1 : a->fea.ਟੈਗ_le > b->fea.ਟੈਗ_le ? 1 :
	   a->ਕ੍ਰਮ < b->ਕ੍ਰਮ ? -1 : a->ਕ੍ਰਮ > b->ਕ੍ਰਮ ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ== (ਅਚਲ hb_ms_ਸਰਗਰਮ_feature_t& f) ਅਚਲ
  { ਵਾਪਸ cmp (ਇਹ, &f) == 0; }
};

ਬਣਤਰ hb_ms_feature_event_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
  ਤਾਰਕਿਕ ਸ਼ੁਰੂ;
  hb_ms_ਸਰਗਰਮ_feature_t feature;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ *a = (ਅਚਲ hb_ms_feature_event_t *) pa;
    ਅਚਲ ਸਵੈਚਾਲਿਤ *b = (ਅਚਲ hb_ms_feature_event_t *) pb;
    ਵਾਪਸ a->ਸੂਚਕਾਂਕ_2 < b->ਸੂਚਕਾਂਕ_2 ? -1 : a->ਸੂਚਕਾਂਕ_2 > b->ਸੂਚਕਾਂਕ_2 ? 1 :
	   a->ਸ਼ੁਰੂ < b->ਸ਼ੁਰੂ ? -1 : a->ਸ਼ੁਰੂ > b->ਸ਼ੁਰੂ ? 1 :
	   hb_ms_ਸਰਗਰਮ_feature_t::cmp (&a->feature, &b->feature);
  }
};

ਬਣਤਰ hb_ms_range_ਰਿਕਾਰਡ_t {
  hb_ms_features_t features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_ਪਹਿਲਾ; /* == start */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_ਆਖਰੀ;  /* == end - 1 */
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ms_ਤਿਆਰੀ_features (ਅਚਲ hb_feature_t                *features,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                       ਅੰਕ_features,
		      hb_ਸਦਿਸ਼_t<hb_ms_feature_t>      &feature_ਰਿਕਾਰਡਾਂ, /*ਬਾਹਰਲਾ*/
		      hb_ਸਦਿਸ਼_t<hb_ms_range_ਰਿਕਾਰਡ_t> &range_ਰਿਕਾਰਡਾਂ /*ਬਾਹਰਲਾ*/)
{
  feature_ਰਿਕਾਰਡਾਂ.clear ();
  range_ਰਿਕਾਰਡਾਂ.clear ();

  /* Sort features by start/end events. */
  hb_ਸਦਿਸ਼_t<hb_ms_feature_event_t> feature_events;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
  {
    hb_ms_ਸਰਗਰਮ_feature_t feature;
    feature.fea.ਟੈਗ_le = hb_uint32_ਅਦਲਾ_ਬਦਲੀ (features[i].ਟੈਗ_2);
    feature.fea.value = features[i].value;
    feature.ਕ੍ਰਮ = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = features[i].ਸ਼ੁਰੂ;
    event->ਸ਼ੁਰੂ = ਸੱਚ;
    event->feature = feature;

    event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = features[i].ਸਿਰਾ;
    event->ਸ਼ੁਰੂ = ਝੂਠ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_ਸਰਗਰਮ_feature_t feature;
    feature.fea.ਟੈਗ_le = 0;
    feature.fea.value = 0;
    feature.ਕ੍ਰਮ = ਅੰਕ_features + 1;

    ਸਵੈਚਾਲਿਤ *event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = 0; /* This value does magic. */
    event->ਸ਼ੁਰੂ = ਝੂਠ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_ਸਦਿਸ਼_t<hb_ms_ਸਰਗਰਮ_feature_t> ਸਰਗਰਮ_features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਸੂਚਕਾਂਕ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < feature_events.ਲੰਮਾਈ; i++)
  {
    ਸਵੈਚਾਲਿਤ *event = &feature_events[i];

    ਜੇ (event->ਸੂਚਕਾਂਕ_2 != ਆਖਰੀ_ਸੂਚਕਾਂਕ)
    {
      /* Save a snapshot of active features and the range. */
      ਸਵੈਚਾਲਿਤ *range = range_ਰਿਕਾਰਡਾਂ.push ();
      ਸਵੈਚਾਲਿਤ ਵਿੱਥ = feature_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ;

      ਸਰਗਰਮ_features.qsort ();
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਸਰਗਰਮ_features.ਲੰਮਾਈ; j++)
      {
        ਜੇ (!j || ਸਰਗਰਮ_features[j].fea.ਟੈਗ_le != feature_ਰਿਕਾਰਡਾਂ[feature_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ - 1].ਟੈਗ_le)
        {
          feature_ਰਿਕਾਰਡਾਂ.push (ਸਰਗਰਮ_features[j].fea);
        }
        ਨਹੀਂਤਾਂ
        {
          /* Overrides value for existing feature. */
          feature_ਰਿਕਾਰਡਾਂ[feature_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ - 1].value = ਸਰਗਰਮ_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (ਵਿੱਥ);
      range->features.ਅੰਕ_features = feature_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ - ਵਿੱਥ;
      range->ਸੂਚਕਾਂਕ_ਪਹਿਲਾ = ਆਖਰੀ_ਸੂਚਕਾਂਕ;
      range->ਸੂਚਕਾਂਕ_ਆਖਰੀ  = event->ਸੂਚਕਾਂਕ_2 - 1;

      ਆਖਰੀ_ਸੂਚਕਾਂਕ = event->ਸੂਚਕਾਂਕ_2;
    }

    ਜੇ (event->ਸ਼ੁਰੂ)
    {
      ਸਰਗਰਮ_features.push (event->feature);
    }
    ਨਹੀਂਤਾਂ
    {
      ਸਵੈਚਾਲਿਤ *feature = ਸਰਗਰਮ_features.lsearch (event->feature);
      ਜੇ (feature)
        ਸਰਗਰਮ_features.remove_ordered (feature - ਸਰਗਰਮ_features.arrayZ);
    }
  }

  ਜੇ (!range_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ) /* No active feature found. */
    ਅੰਕ_features = 0;

  /* Fixup the pointers. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < range_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
  {
    ਸਵੈਚਾਲਿਤ *range = &range_ਰਿਕਾਰਡਾਂ[i];
    range->features.features = (hb_ms_feature_t *) feature_ਰਿਕਾਰਡਾਂ + reinterpret_cast<uintptr_t> (range->features.features);
  }

  ਵਾਪਸ !!ਅੰਕ_features;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_ms_make_feature_ranges (hb_ਸਦਿਸ਼_t<hb_ms_feature_t>      &feature_ਰਿਕਾਰਡਾਂ,
			   hb_ਸਦਿਸ਼_t<hb_ms_range_ਰਿਕਾਰਡ_t> &range_ਰਿਕਾਰਡਾਂ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                       chars_ਵਿੱਥ,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_ਸਦਿਸ਼_t<hb_ms_features_t*>    &range_features, /*ਬਾਹਰਲਾ*/
			   hb_ਸਦਿਸ਼_t<uint32_t>             &range_counts /*ਬਾਹਰਲਾ*/)
{
  range_features.clear ();
  range_counts.clear ();

  ਸਵੈਚਾਲਿਤ *ਆਖਰੀ_range = &range_ਰਿਕਾਰਡਾਂ[0];
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = chars_ਵਿੱਥ; i < chars_len; i++)
  {
    ਸਵੈਚਾਲਿਤ *range = ਆਖਰੀ_range;
    ਜਦੋਂ (log_clusters[i] < range->ਸੂਚਕਾਂਕ_ਪਹਿਲਾ)
      range--;
    ਜਦੋਂ (log_clusters[i] > range->ਸੂਚਕਾਂਕ_ਆਖਰੀ)
      range++;
    ਜੇ (!range_features.ਲੰਮਾਈ ||
        &range->features != range_features[range_features.ਲੰਮਾਈ - 1])
    {
      ਸਵੈਚਾਲਿਤ **features = range_features.push ();
      ਸਵੈਚਾਲਿਤ *c = range_counts.push ();
      ਜੇ (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        ਰੋਕੋ;
      }
      *features = &range->features;
      *c = 1;
    }
    ਨਹੀਂਤਾਂ
    {
      range_counts[range_counts.ਲੰਮਾਈ - 1]++;
    }

    ਆਖਰੀ_range = range;
  }
}

#ਜੇ_ਅੰਤ /* HB_MS_FEATURE_RANGES_HH */
