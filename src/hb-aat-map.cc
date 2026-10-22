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

#ضمن "hb.hh"

#که_نه_تعریف_شوی HB_NO_AAT_SHAPE

#ضمن "hb-aat-map.hh"

#ضمن "hb-aat-layout.hh"
#ضمن "hb-aat-layout-feat-table.hh"


فراغ hb_aat_map_builder_t::جمع_کړه_feature (ثابت hb_feature_t &feature)
{
  إذا (!face->جدول.feat->has_معلومات ()) أرجع;

  إذا (feature.نښان_2 == HB_نښان ('a','a','l','t'))
  {
    إذا (!face->جدول.feat->exposes_feature (HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES))
      أرجع;
    feature_range_t *range = features.push();
    range->پیل = feature.پیل;
    range->پای_ټکی = feature.پای_ټکی;
    range->info.type = HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_ALTERNATIVES;
    range->info.setting = (hb_aat_layout_feature_selector_t) feature.value;
    range->info.seq = features.اوږدتیا;
    range->info.is_exclusive = ریښتیا;
    أرجع;
  }

  ثابت hb_aat_feature_mapping_t *mapping = hb_aat_layout_پیدا_کړه_feature_mapping (feature.نښان_2);
  إذا (!mapping) أرجع;

  ثابت AAT::FeatureName* feature_name = &face->جدول.feat->get_feature (mapping->aatFeatureType);
  إذا (!feature_name->has_معلومات ())
  {
    /* Special case: Chain::compile_flags will fall back to the deprecated version of
     * small-caps if necessary, so we need to check for that possibility.
     * https://github.com/harfbuzz/harfbuzz/issues/2307 */
    إذا (mapping->aatFeatureType == HB_AAT_LAYOUT_FEATURE_TYPE_ښکتنی_CASE &&
	mapping->selector_منزل_Enable == HB_AAT_LAYOUT_FEATURE_SELECTOR_ښکتنی_CASE_SMALL_CAPS)
    {
      feature_name = &face->جدول.feat->get_feature (HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE);
      إذا (!feature_name->has_معلومات ()) أرجع;
    }
    وإلا أرجع;
  }

  feature_range_t *range = features.push();
  range->پیل = feature.پیل;
  range->پای_ټکی = feature.پای_ټکی;
  range->info.type = mapping->aatFeatureType;
  range->info.setting = feature.value ? mapping->selector_منزل_Enable : mapping->selector_منزل_Disable;
  range->info.seq = features.اوږدتیا;
  range->info.is_exclusive = feature_name->is_exclusive ();
}

فراغ
hb_aat_map_builder_t::compile (hb_aat_map_t  &m)
{
  /* Compute active features per range, and compile each. */
  إذا (!features.اوږدتیا)
  {
    hb_aat_layout_compile_map (هذا, &m);
    أرجع;
  }

  /* Sort features by start/end events. */
  hb_ویکټور_t<feature_event_t> feature_events;
  feature_events.alloc_exact (features.اوږدتیا * 2 + 1);
  لكل (بینښه صحيح i = 0; i < features.اوږدتیا; i++)
  {
    خپلکاری &feature = features.arrayZ[i];

    إذا (feature.پیل == feature.پای_ټکی)
      استمر;

    feature_event_t *event;

    event = feature_events.push ();
    event->شاخص_2 = feature.پیل;
    event->پیل = ریښتیا;
    event->feature = feature.info;

    event = feature_events.push ();
    event->شاخص_2 = feature.پای_ټکی;
    event->پیل = خطأ;
    event->feature = feature.info;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    feature_info_t feature;
    feature.seq = features.اوږدتیا + 1;

    feature_event_t *event = feature_events.push ();
    event->شاخص_2 = -1; /* This value does magic. */
    event->پیل = خطأ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_sorted_ویکټور_t<feature_info_t> فعال_features;
  بینښه صحيح وروستی_شاخص = 0;
  لكل (بینښه صحيح i = 0; i < feature_events.اوږدتیا; i++)
  {
    feature_event_t *event = &feature_events[i];

    إذا (event->شاخص_2 != وروستی_شاخص)
    {
      /* Save a snapshot of active features and the range. */

      /* Sort features and merge duplicates */
      current_features = فعال_features;
      range_لومړی = وروستی_شاخص;
      range_وروستی = event->شاخص_2 - 1;
      إذا (current_features.اوږدتیا)
      {
	current_features.qsort ();
	بینښه صحيح j = 0;
	لكل (بینښه صحيح i = 1; i < current_features.اوږدتیا; i++)
	  إذا (current_features.arrayZ[i].type != current_features.arrayZ[j].type ||
	      /* Nonexclusive feature selectors come in even/odd pairs to turn a setting on/off
	       * respectively, so we mask out the low-order bit when checking for "duplicates"
	       * (selectors referring to the same feature setting) here. */
	      (!current_features.arrayZ[i].is_exclusive && ((current_features.arrayZ[i].setting & ~1) != (current_features.arrayZ[j].setting & ~1))))
	    current_features.arrayZ[++j] = current_features.arrayZ[i];
	current_features.shrink (j + 1);
      }

      hb_aat_layout_compile_map (هذا, &m);

      وروستی_شاخص = event->شاخص_2;
    }

    إذا (event->پیل)
    {
      فعال_features.push (event->feature);
    } وإلا {
      feature_info_t *feature = فعال_features.lsearch (event->feature);
      إذا (feature)
	فعال_features.remove_ordered (feature - فعال_features.arrayZ);
    }
  }

  لكل (خپلکاری &chain_flags : m.chain_flags)
    // With our above setup this value is one less than desired; adjust it.
    chain_flags.لکۍ().cluster_وروستی = HB_FEATURE_GLOBAL_پای_ټکی;
}


#د_که_پای
