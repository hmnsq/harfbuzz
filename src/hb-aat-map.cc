/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2010,2011,2013  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_AAT_SHAPE

#అన్తర్భావ "hb-aat-map.hh"

#అన్తర్భావ "hb-aat-layout.hh"
#అన్తర్భావ "hb-aat-layout-feat-table.hh"


శూన్య hb_aat_map_builder_t::కూడు_feature (స్థిరరాశి hb_feature_t &feature)
{
  ఒకవేళ (!face->పట్టిక.feat->has_దత్తాంశం ()) తిరిగి;

  ఒకవేళ (feature.ట్యాగ్_2 == HB_ట్యాగ్ ('a','a','l','t'))
  {
    ఒకవేళ (!face->పట్టిక.feat->exposes_feature (HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES))
      తిరిగి;
    feature_range_t *range = features.push();
    range->ప్రారంభం = feature.ప్రారంభం;
    range->చివర = feature.చివర;
    range->info.type = HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES;
    range->info.setting = (hb_aat_layout_feature_selector_t) feature.value;
    range->info.seq = features.నిడివి;
    range->info.is_exclusive = నిజం;
    తిరిగి;
  }

  స్థిరరాశి hb_aat_feature_mapping_t *mapping = hb_aat_layout_కనుగొను_feature_mapping (feature.ట్యాగ్_2);
  ఒకవేళ (!mapping) తిరిగి;

  స్థిరరాశి AAT::FeatureName* feature_name = &face->పట్టిక.feat->get_feature (mapping->aatFeatureType);
  ఒకవేళ (!feature_name->has_దత్తాంశం ())
  {
    /* Special case: Chain::compile_flags will fall back to the deprecated version of
     * small-caps if necessary, so we need to check for that possibility.
     * https://github.com/harfbuzz/harfbuzz/issues/2307 */
    ఒకవేళ (mapping->aatFeatureType == HB_AAT_LAYOUT_FEATURE_TYPE_క్రింది_CASE &&
	mapping->selector_గమ్యం_Enable == HB_AAT_LAYOUT_FEATURE_SELECTOR_క్రింది_CASE_SMALL_CAPS)
    {
      feature_name = &face->పట్టిక.feat->get_feature (HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE);
      ఒకవేళ (!feature_name->has_దత్తాంశం ()) తిరిగి;
    }
    లేకపోతే తిరిగి;
  }

  feature_range_t *range = features.push();
  range->ప్రారంభం = feature.ప్రారంభం;
  range->చివర = feature.చివర;
  range->info.type = mapping->aatFeatureType;
  range->info.setting = feature.value ? mapping->selector_గమ్యం_Enable : mapping->selector_గమ్యం_Disable;
  range->info.seq = features.నిడివి;
  range->info.is_exclusive = feature_name->is_exclusive ();
}

శూన్య
hb_aat_map_builder_t::compile (hb_aat_map_t  &m)
{
  /* Compute active features per range, and compile each. */
  ఒకవేళ (!features.నిడివి)
  {
    hb_aat_layout_compile_map (ఇది, &m);
    తిరిగి;
  }

  /* Sort features by start/end events. */
  hb_సదిశ_t<feature_event_t> feature_events;
  feature_events.alloc_exact (features.నిడివి * 2 + 1);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < features.నిడివి; i++)
  {
    స్వయంచాలక &feature = features.arrayZ[i];

    ఒకవేళ (feature.ప్రారంభం == feature.చివర)
      కొనసాగు;

    feature_event_t *event;

    event = feature_events.push ();
    event->సూచీ_2 = feature.ప్రారంభం;
    event->ప్రారంభం = నిజం;
    event->feature = feature.info;

    event = feature_events.push ();
    event->సూచీ_2 = feature.చివర;
    event->ప్రారంభం = అబద్ధం;
    event->feature = feature.info;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    feature_info_t feature;
    feature.seq = features.నిడివి + 1;

    feature_event_t *event = feature_events.push ();
    event->సూచీ_2 = -1; /* This value does magic. */
    event->ప్రారంభం = అబద్ధం;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_sorted_సదిశ_t<feature_info_t> చురుకైన_features;
  గుర్తులేని పూర్ణాంకం చివరి_సూచీ = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < feature_events.నిడివి; i++)
  {
    feature_event_t *event = &feature_events[i];

    ఒకవేళ (event->సూచీ_2 != చివరి_సూచీ)
    {
      /* Save a snapshot of active features and the range. */

      /* Sort features and merge duplicates */
      current_features = చురుకైన_features;
      range_మొదటి = చివరి_సూచీ;
      range_చివరి = event->సూచీ_2 - 1;
      ఒకవేళ (current_features.నిడివి)
      {
	current_features.qsort ();
	గుర్తులేని పూర్ణాంకం j = 0;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < current_features.నిడివి; i++)
	  ఒకవేళ (current_features.arrayZ[i].type != current_features.arrayZ[j].type ||
	      /* Nonexclusive feature selectors come in even/odd pairs to turn a setting on/off
	       * respectively, so we mask out the low-order bit when checking for "duplicates"
	       * (selectors referring to the same feature setting) here. */
	      (!current_features.arrayZ[i].is_exclusive && ((current_features.arrayZ[i].setting & ~1) != (current_features.arrayZ[j].setting & ~1))))
	    current_features.arrayZ[++j] = current_features.arrayZ[i];
	current_features.shrink (j + 1);
      }

      hb_aat_layout_compile_map (ఇది, &m);

      చివరి_సూచీ = event->సూచీ_2;
    }

    ఒకవేళ (event->ప్రారంభం)
    {
      చురుకైన_features.push (event->feature);
    } లేకపోతే {
      feature_info_t *feature = చురుకైన_features.lsearch (event->feature);
      ఒకవేళ (feature)
	చురుకైన_features.remove_ordered (feature - చురుకైన_features.arrayZ);
    }
  }

  కొరకు (స్వయంచాలక &chain_flags : m.chain_flags)
    // With our above setup this value is one less than desired; adjust it.
    chain_flags.తోక().cluster_చివరి = HB_FEATURE_GLOBAL_చివర;
}


#అయితే_ముగింపు
