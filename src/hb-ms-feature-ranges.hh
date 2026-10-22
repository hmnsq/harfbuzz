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

#నిర్వచితం_కాకపోతే HB_MS_FEATURE_RANGES_HH
#నిర్వచించు HB_MS_FEATURE_RANGES_HH

#అన్తర్భావ "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

రకనిర్వచనం నిర్మాణం hb_ms_feature_t {
  uint32_t ట్యాగ్_le;
  uint32_t value;
} hb_ms_feature_t;

రకనిర్వచనం నిర్మాణం hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         అంకె_features;
} hb_ms_features_t;

నిర్మాణం hb_ms_చురుకైన_feature_t {
  hb_ms_feature_t fea;
  గుర్తులేని పూర్ణాంకం క్రమం;

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb) {
    స్థిరరాశి స్వయంచాలక *a = (స్థిరరాశి hb_ms_చురుకైన_feature_t *) pa;
    స్థిరరాశి స్వయంచాలక *b = (స్థిరరాశి hb_ms_చురుకైన_feature_t *) pb;
    తిరిగి a->fea.ట్యాగ్_le < b->fea.ట్యాగ్_le ? -1 : a->fea.ట్యాగ్_le > b->fea.ట్యాగ్_le ? 1 :
	   a->క్రమం < b->క్రమం ? -1 : a->క్రమం > b->క్రమం ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  తార్కిక క్రియాకారకం== (స్థిరరాశి hb_ms_చురుకైన_feature_t& f) స్థిరరాశి
  { తిరిగి cmp (ఇది, &f) == 0; }
};

నిర్మాణం hb_ms_feature_event_t {
  గుర్తులేని పూర్ణాంకం సూచీ_2;
  తార్కిక ప్రారంభం;
  hb_ms_చురుకైన_feature_t feature;

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb)
  {
    స్థిరరాశి స్వయంచాలక *a = (స్థిరరాశి hb_ms_feature_event_t *) pa;
    స్థిరరాశి స్వయంచాలక *b = (స్థిరరాశి hb_ms_feature_event_t *) pb;
    తిరిగి a->సూచీ_2 < b->సూచీ_2 ? -1 : a->సూచీ_2 > b->సూచీ_2 ? 1 :
	   a->ప్రారంభం < b->ప్రారంభం ? -1 : a->ప్రారంభం > b->ప్రారంభం ? 1 :
	   hb_ms_చురుకైన_feature_t::cmp (&a->feature, &b->feature);
  }
};

నిర్మాణం hb_ms_range_రికార్డు_t {
  hb_ms_features_t features;
  గుర్తులేని పూర్ణాంకం సూచీ_మొదటి; /* == start */
  గుర్తులేని పూర్ణాంకం సూచీ_చివరి;  /* == end - 1 */
};

స్థిర అంతఃవరుస తార్కిక
hb_ms_అమరిక_features (స్థిరరాశి hb_feature_t                *features,
		      గుర్తులేని పూర్ణాంకం                       అంకె_features,
		      hb_సదిశ_t<hb_ms_feature_t>      &feature_రికార్డులు, /*బహిర్గతం*/
		      hb_సదిశ_t<hb_ms_range_రికార్డు_t> &range_రికార్డులు /*బహిర్గతం*/)
{
  feature_రికార్డులు.clear ();
  range_రికార్డులు.clear ();

  /* Sort features by start/end events. */
  hb_సదిశ_t<hb_ms_feature_event_t> feature_events;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
  {
    hb_ms_చురుకైన_feature_t feature;
    feature.fea.ట్యాగ్_le = hb_uint32_మార్పిడి (features[i].ట్యాగ్_2);
    feature.fea.value = features[i].value;
    feature.క్రమం = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->సూచీ_2 = features[i].ప్రారంభం;
    event->ప్రారంభం = నిజం;
    event->feature = feature;

    event = feature_events.push ();
    event->సూచీ_2 = features[i].చివర;
    event->ప్రారంభం = అబద్ధం;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_చురుకైన_feature_t feature;
    feature.fea.ట్యాగ్_le = 0;
    feature.fea.value = 0;
    feature.క్రమం = అంకె_features + 1;

    స్వయంచాలక *event = feature_events.push ();
    event->సూచీ_2 = 0; /* This value does magic. */
    event->ప్రారంభం = అబద్ధం;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_సదిశ_t<hb_ms_చురుకైన_feature_t> చురుకైన_features;
  గుర్తులేని పూర్ణాంకం చివరి_సూచీ = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < feature_events.నిడివి; i++)
  {
    స్వయంచాలక *event = &feature_events[i];

    ఒకవేళ (event->సూచీ_2 != చివరి_సూచీ)
    {
      /* Save a snapshot of active features and the range. */
      స్వయంచాలక *range = range_రికార్డులు.push ();
      స్వయంచాలక స్థానభ్రంశం = feature_రికార్డులు.నిడివి;

      చురుకైన_features.qsort ();
      కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < చురుకైన_features.నిడివి; j++)
      {
        ఒకవేళ (!j || చురుకైన_features[j].fea.ట్యాగ్_le != feature_రికార్డులు[feature_రికార్డులు.నిడివి - 1].ట్యాగ్_le)
        {
          feature_రికార్డులు.push (చురుకైన_features[j].fea);
        }
        లేకపోతే
        {
          /* Overrides value for existing feature. */
          feature_రికార్డులు[feature_రికార్డులు.నిడివి - 1].value = చురుకైన_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (స్థానభ్రంశం);
      range->features.అంకె_features = feature_రికార్డులు.నిడివి - స్థానభ్రంశం;
      range->సూచీ_మొదటి = చివరి_సూచీ;
      range->సూచీ_చివరి  = event->సూచీ_2 - 1;

      చివరి_సూచీ = event->సూచీ_2;
    }

    ఒకవేళ (event->ప్రారంభం)
    {
      చురుకైన_features.push (event->feature);
    }
    లేకపోతే
    {
      స్వయంచాలక *feature = చురుకైన_features.lsearch (event->feature);
      ఒకవేళ (feature)
        చురుకైన_features.remove_ordered (feature - చురుకైన_features.arrayZ);
    }
  }

  ఒకవేళ (!range_రికార్డులు.నిడివి) /* No active feature found. */
    అంకె_features = 0;

  /* Fixup the pointers. */
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < range_రికార్డులు.నిడివి; i++)
  {
    స్వయంచాలక *range = &range_రికార్డులు[i];
    range->features.features = (hb_ms_feature_t *) feature_రికార్డులు + reinterpret_cast<uintptr_t> (range->features.features);
  }

  తిరిగి !!అంకె_features;
}

స్థిర అంతఃవరుస శూన్య
hb_ms_make_feature_ranges (hb_సదిశ_t<hb_ms_feature_t>      &feature_రికార్డులు,
			   hb_సదిశ_t<hb_ms_range_రికార్డు_t> &range_రికార్డులు,
			   గుర్తులేని పూర్ణాంకం                       chars_స్థానభ్రంశం,
			   గుర్తులేని పూర్ణాంకం                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_సదిశ_t<hb_ms_features_t*>    &range_features, /*బహిర్గతం*/
			   hb_సదిశ_t<uint32_t>             &range_counts /*బహిర్గతం*/)
{
  range_features.clear ();
  range_counts.clear ();

  స్వయంచాలక *చివరి_range = &range_రికార్డులు[0];
  కొరకు (గుర్తులేని పూర్ణాంకం i = chars_స్థానభ్రంశం; i < chars_len; i++)
  {
    స్వయంచాలక *range = చివరి_range;
    అయితే (log_clusters[i] < range->సూచీ_మొదటి)
      range--;
    అయితే (log_clusters[i] > range->సూచీ_చివరి)
      range++;
    ఒకవేళ (!range_features.నిడివి ||
        &range->features != range_features[range_features.నిడివి - 1])
    {
      స్వయంచాలక **features = range_features.push ();
      స్వయంచాలక *c = range_counts.push ();
      ఒకవేళ (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        ఆపు;
      }
      *features = &range->features;
      *c = 1;
    }
    లేకపోతే
    {
      range_counts[range_counts.నిడివి - 1]++;
    }

    చివరి_range = range;
  }
}

#అయితే_ముగింపు /* HB_MS_FEATURE_RANGES_HH */
