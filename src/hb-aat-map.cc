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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_AAT_SHAPE

#شامل "hb-aat-map.hh"

#شامل "hb-aat-layout.hh"
#شامل "hb-aat-layout-feat-table.hh"


خالی hb_aat_map_builder_t::جوڑو_feature (ثابت hb_feature_t &feature)
{
  اگر (!face->جدول.feat->has_ڈیٹا ()) واپس;

  اگر (feature.لیبل_2 == HB_لیبل ('a','a','l','t'))
  {
    اگر (!face->جدول.feat->exposes_feature (HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES))
      واپس;
    feature_range_t *range = features.push();
    range->آغاز = feature.آغاز;
    range->انتہا = feature.انتہا;
    range->info.type = HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES;
    range->info.setting = (hb_aat_layout_feature_selector_t) feature.value;
    range->info.seq = features.لمبائی;
    range->info.is_exclusive = سچ;
    واپس;
  }

  ثابت hb_aat_feature_mapping_t *mapping = hb_aat_layout_ڈھونڈو_feature_mapping (feature.لیبل_2);
  اگر (!mapping) واپس;

  ثابت AAT::FeatureName* feature_name = &face->جدول.feat->get_feature (mapping->aatFeatureType);
  اگر (!feature_name->has_ڈیٹا ())
  {
    /* Special case: Chain::compile_flags will fall back to the deprecated version of
     * small-caps if necessary, so we need to check for that possibility.
     * https://github.com/harfbuzz/harfbuzz/issues/2307 */
    اگر (mapping->aatFeatureType == HB_AAT_LAYOUT_FEATURE_TYPE_زیریں_CASE &&
	mapping->selector_مقصد_Enable == HB_AAT_LAYOUT_FEATURE_SELECTOR_زیریں_CASE_SMALL_CAPS)
    {
      feature_name = &face->جدول.feat->get_feature (HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE);
      اگر (!feature_name->has_ڈیٹا ()) واپس;
    }
    ورنہ واپس;
  }

  feature_range_t *range = features.push();
  range->آغاز = feature.آغاز;
  range->انتہا = feature.انتہا;
  range->info.type = mapping->aatFeatureType;
  range->info.setting = feature.value ? mapping->selector_مقصد_Enable : mapping->selector_مقصد_Disable;
  range->info.seq = features.لمبائی;
  range->info.is_exclusive = feature_name->is_exclusive ();
}

خالی
hb_aat_map_builder_t::compile (hb_aat_map_t  &m)
{
  /* Compute active features per range, and compile each. */
  اگر (!features.لمبائی)
  {
    hb_aat_layout_compile_map (یہ, &m);
    واپس;
  }

  /* Sort features by start/end events. */
  hb_سمتیہ_t<feature_event_t> feature_events;
  feature_events.alloc_exact (features.لمبائی * 2 + 1);
  برائے (غیرعلامتی صحیح i = 0; i < features.لمبائی; i++)
  {
    خودکار &feature = features.arrayZ[i];

    اگر (feature.آغاز == feature.انتہا)
      جاری;

    feature_event_t *event;

    event = feature_events.push ();
    event->اشاریہ_2 = feature.آغاز;
    event->آغاز = سچ;
    event->feature = feature.info;

    event = feature_events.push ();
    event->اشاریہ_2 = feature.انتہا;
    event->آغاز = جھوٹ;
    event->feature = feature.info;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    feature_info_t feature;
    feature.seq = features.لمبائی + 1;

    feature_event_t *event = feature_events.push ();
    event->اشاریہ_2 = -1; /* This value does magic. */
    event->آغاز = جھوٹ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_sorted_سمتیہ_t<feature_info_t> فعال_features;
  غیرعلامتی صحیح آخری_اشاریہ = 0;
  برائے (غیرعلامتی صحیح i = 0; i < feature_events.لمبائی; i++)
  {
    feature_event_t *event = &feature_events[i];

    اگر (event->اشاریہ_2 != آخری_اشاریہ)
    {
      /* Save a snapshot of active features and the range. */

      /* Sort features and merge duplicates */
      current_features = فعال_features;
      range_پہلا = آخری_اشاریہ;
      range_آخری = event->اشاریہ_2 - 1;
      اگر (current_features.لمبائی)
      {
	current_features.qsort ();
	غیرعلامتی صحیح j = 0;
	برائے (غیرعلامتی صحیح i = 1; i < current_features.لمبائی; i++)
	  اگر (current_features.arrayZ[i].type != current_features.arrayZ[j].type ||
	      /* Nonexclusive feature selectors come in even/odd pairs to turn a setting on/off
	       * respectively, so we mask out the low-order bit when checking for "duplicates"
	       * (selectors referring to the same feature setting) here. */
	      (!current_features.arrayZ[i].is_exclusive && ((current_features.arrayZ[i].setting & ~1) != (current_features.arrayZ[j].setting & ~1))))
	    current_features.arrayZ[++j] = current_features.arrayZ[i];
	current_features.shrink (j + 1);
      }

      hb_aat_layout_compile_map (یہ, &m);

      آخری_اشاریہ = event->اشاریہ_2;
    }

    اگر (event->آغاز)
    {
      فعال_features.push (event->feature);
    } ورنہ {
      feature_info_t *feature = فعال_features.lsearch (event->feature);
      اگر (feature)
	فعال_features.remove_ordered (feature - فعال_features.arrayZ);
    }
  }

  برائے (خودکار &chain_flags : m.chain_flags)
    // With our above setup this value is one less than desired; adjust it.
    chain_flags.دم().cluster_آخری = HB_FEATURE_GLOBAL_انتہا;
}


#اگر_ختم
