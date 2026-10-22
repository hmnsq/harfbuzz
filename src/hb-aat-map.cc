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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-map.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-feat-table.hh"


ਖਾਲੀ hb_aat_map_builder_t::ਜੋੜੋ_feature (ਅਚਲ hb_feature_t &feature)
{
  ਜੇ (!face->ਸਾਰਣੀ.feat->has_ਅੰਕੜੇ ()) ਵਾਪਸ;

  ਜੇ (feature.ਟੈਗ_2 == HB_ਟੈਗ ('a','a','l','t'))
  {
    ਜੇ (!face->ਸਾਰਣੀ.feat->exposes_feature (HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES))
      ਵਾਪਸ;
    feature_range_t *range = features.push();
    range->ਸ਼ੁਰੂ = feature.ਸ਼ੁਰੂ;
    range->ਸਿਰਾ = feature.ਸਿਰਾ;
    range->info.type = HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES;
    range->info.setting = (hb_aat_layout_feature_selector_t) feature.value;
    range->info.seq = features.ਲੰਮਾਈ;
    range->info.is_exclusive = ਸੱਚ;
    ਵਾਪਸ;
  }

  ਅਚਲ hb_aat_feature_mapping_t *mapping = hb_aat_layout_ਲੱਭੋ_feature_mapping (feature.ਟੈਗ_2);
  ਜੇ (!mapping) ਵਾਪਸ;

  ਅਚਲ AAT::FeatureName* feature_name = &face->ਸਾਰਣੀ.feat->get_feature (mapping->aatFeatureType);
  ਜੇ (!feature_name->has_ਅੰਕੜੇ ())
  {
    /* Special case: Chain::compile_flags will fall back to the deprecated version of
     * small-caps if necessary, so we need to check for that possibility.
     * https://github.com/harfbuzz/harfbuzz/issues/2307 */
    ਜੇ (mapping->aatFeatureType == HB_AAT_LAYOUT_FEATURE_TYPE_ਹੇਠਲਾ_CASE &&
	mapping->selector_ਮੰਜ਼ਿਲ_Enable == HB_AAT_LAYOUT_FEATURE_SELECTOR_ਹੇਠਲਾ_CASE_SMALL_CAPS)
    {
      feature_name = &face->ਸਾਰਣੀ.feat->get_feature (HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE);
      ਜੇ (!feature_name->has_ਅੰਕੜੇ ()) ਵਾਪਸ;
    }
    ਨਹੀਂਤਾਂ ਵਾਪਸ;
  }

  feature_range_t *range = features.push();
  range->ਸ਼ੁਰੂ = feature.ਸ਼ੁਰੂ;
  range->ਸਿਰਾ = feature.ਸਿਰਾ;
  range->info.type = mapping->aatFeatureType;
  range->info.setting = feature.value ? mapping->selector_ਮੰਜ਼ਿਲ_Enable : mapping->selector_ਮੰਜ਼ਿਲ_Disable;
  range->info.seq = features.ਲੰਮਾਈ;
  range->info.is_exclusive = feature_name->is_exclusive ();
}

ਖਾਲੀ
hb_aat_map_builder_t::compile (hb_aat_map_t  &m)
{
  /* Compute active features per range, and compile each. */
  ਜੇ (!features.ਲੰਮਾਈ)
  {
    hb_aat_layout_compile_map (ਇਹ, &m);
    ਵਾਪਸ;
  }

  /* Sort features by start/end events. */
  hb_ਸਦਿਸ਼_t<feature_event_t> feature_events;
  feature_events.alloc_exact (features.ਲੰਮਾਈ * 2 + 1);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < features.ਲੰਮਾਈ; i++)
  {
    ਸਵੈਚਾਲਿਤ &feature = features.arrayZ[i];

    ਜੇ (feature.ਸ਼ੁਰੂ == feature.ਸਿਰਾ)
      ਜਾਰੀ;

    feature_event_t *event;

    event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = feature.ਸ਼ੁਰੂ;
    event->ਸ਼ੁਰੂ = ਸੱਚ;
    event->feature = feature.info;

    event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = feature.ਸਿਰਾ;
    event->ਸ਼ੁਰੂ = ਝੂਠ;
    event->feature = feature.info;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    feature_info_t feature;
    feature.seq = features.ਲੰਮਾਈ + 1;

    feature_event_t *event = feature_events.push ();
    event->ਸੂਚਕਾਂਕ_2 = -1; /* This value does magic. */
    event->ਸ਼ੁਰੂ = ਝੂਠ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_sorted_ਸਦਿਸ਼_t<feature_info_t> ਸਰਗਰਮ_features;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਸੂਚਕਾਂਕ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < feature_events.ਲੰਮਾਈ; i++)
  {
    feature_event_t *event = &feature_events[i];

    ਜੇ (event->ਸੂਚਕਾਂਕ_2 != ਆਖਰੀ_ਸੂਚਕਾਂਕ)
    {
      /* Save a snapshot of active features and the range. */

      /* Sort features and merge duplicates */
      current_features = ਸਰਗਰਮ_features;
      range_ਪਹਿਲਾ = ਆਖਰੀ_ਸੂਚਕਾਂਕ;
      range_ਆਖਰੀ = event->ਸੂਚਕਾਂਕ_2 - 1;
      ਜੇ (current_features.ਲੰਮਾਈ)
      {
	current_features.qsort ();
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < current_features.ਲੰਮਾਈ; i++)
	  ਜੇ (current_features.arrayZ[i].type != current_features.arrayZ[j].type ||
	      /* Nonexclusive feature selectors come in even/odd pairs to turn a setting on/off
	       * respectively, so we mask out the low-order bit when checking for "duplicates"
	       * (selectors referring to the same feature setting) here. */
	      (!current_features.arrayZ[i].is_exclusive && ((current_features.arrayZ[i].setting & ~1) != (current_features.arrayZ[j].setting & ~1))))
	    current_features.arrayZ[++j] = current_features.arrayZ[i];
	current_features.shrink (j + 1);
      }

      hb_aat_layout_compile_map (ਇਹ, &m);

      ਆਖਰੀ_ਸੂਚਕਾਂਕ = event->ਸੂਚਕਾਂਕ_2;
    }

    ਜੇ (event->ਸ਼ੁਰੂ)
    {
      ਸਰਗਰਮ_features.push (event->feature);
    } ਨਹੀਂਤਾਂ {
      feature_info_t *feature = ਸਰਗਰਮ_features.lsearch (event->feature);
      ਜੇ (feature)
	ਸਰਗਰਮ_features.remove_ordered (feature - ਸਰਗਰਮ_features.arrayZ);
    }
  }

  ਲਈ (ਸਵੈਚਾਲਿਤ &chain_flags : m.chain_flags)
    // With our above setup this value is one less than desired; adjust it.
    chain_flags.ਪੂਛ().cluster_ਆਖਰੀ = HB_FEATURE_GLOBAL_ਸਿਰਾ;
}


#ਜੇ_ਅੰਤ
