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

#جيڪڏهن_وصف_نه_ٿيل HB_MS_FEATURE_RANGES_HH
#وصف_ڪريو HB_MS_FEATURE_RANGES_HH

#ضمن "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

تعريفنوع بنية hb_ms_feature_t {
  uint32_t ٽيگ_le;
  uint32_t value;
} hb_ms_feature_t;

تعريفنوع بنية hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         انگ_features;
} hb_ms_features_t;

بنية hb_ms_سرگرم_feature_t {
  hb_ms_feature_t fea;
  بنانشان صحيح ترتيب;

  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت خودڪار *a = (ثابت hb_ms_سرگرم_feature_t *) pa;
    ثابت خودڪار *b = (ثابت hb_ms_سرگرم_feature_t *) pb;
    أرجع a->fea.ٽيگ_le < b->fea.ٽيگ_le ? -1 : a->fea.ٽيگ_le > b->fea.ٽيگ_le ? 1 :
	   a->ترتيب < b->ترتيب ? -1 : a->ترتيب > b->ترتيب ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  منطقي عامل== (ثابت hb_ms_سرگرم_feature_t& f) ثابت
  { أرجع cmp (هذا, &f) == 0; }
};

بنية hb_ms_feature_event_t {
  بنانشان صحيح اشاريو_2;
  منطقي شروعات;
  hb_ms_سرگرم_feature_t feature;

  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت خودڪار *a = (ثابت hb_ms_feature_event_t *) pa;
    ثابت خودڪار *b = (ثابت hb_ms_feature_event_t *) pb;
    أرجع a->اشاريو_2 < b->اشاريو_2 ? -1 : a->اشاريو_2 > b->اشاريو_2 ? 1 :
	   a->شروعات < b->شروعات ? -1 : a->شروعات > b->شروعات ? 1 :
	   hb_ms_سرگرم_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية hb_ms_range_رڪارڊ_t {
  hb_ms_features_t features;
  بنانشان صحيح اشاريو_پهريون; /* == start */
  بنانشان صحيح اشاريو_آخري;  /* == end - 1 */
};

ساكن مضمن منطقي
hb_ms_تياري_features (ثابت hb_feature_t                *features,
		      بنانشان صحيح                       انگ_features,
		      hb_ويڪٽر_t<hb_ms_feature_t>      &feature_رڪارڊون, /*ٻاهريون*/
		      hb_ويڪٽر_t<hb_ms_range_رڪارڊ_t> &range_رڪارڊون /*ٻاهريون*/)
{
  feature_رڪارڊون.clear ();
  range_رڪارڊون.clear ();

  /* Sort features by start/end events. */
  hb_ويڪٽر_t<hb_ms_feature_event_t> feature_events;
  لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
  {
    hb_ms_سرگرم_feature_t feature;
    feature.fea.ٽيگ_le = hb_uint32_مٽا_سٽا (features[i].ٽيگ_2);
    feature.fea.value = features[i].value;
    feature.ترتيب = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->اشاريو_2 = features[i].شروعات;
    event->شروعات = سچ;
    event->feature = feature;

    event = feature_events.push ();
    event->اشاريو_2 = features[i].پڇاڙي;
    event->شروعات = خطأ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_سرگرم_feature_t feature;
    feature.fea.ٽيگ_le = 0;
    feature.fea.value = 0;
    feature.ترتيب = انگ_features + 1;

    خودڪار *event = feature_events.push ();
    event->اشاريو_2 = 0; /* This value does magic. */
    event->شروعات = خطأ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_ويڪٽر_t<hb_ms_سرگرم_feature_t> سرگرم_features;
  بنانشان صحيح آخري_اشاريو = 0;
  لكل (بنانشان صحيح i = 0; i < feature_events.ڊيگهائي; i++)
  {
    خودڪار *event = &feature_events[i];

    إذا (event->اشاريو_2 != آخري_اشاريو)
    {
      /* Save a snapshot of active features and the range. */
      خودڪار *range = range_رڪارڊون.push ();
      خودڪار وٿي = feature_رڪارڊون.ڊيگهائي;

      سرگرم_features.qsort ();
      لكل (بنانشان صحيح j = 0; j < سرگرم_features.ڊيگهائي; j++)
      {
        إذا (!j || سرگرم_features[j].fea.ٽيگ_le != feature_رڪارڊون[feature_رڪارڊون.ڊيگهائي - 1].ٽيگ_le)
        {
          feature_رڪارڊون.push (سرگرم_features[j].fea);
        }
        وإلا
        {
          /* Overrides value for existing feature. */
          feature_رڪارڊون[feature_رڪارڊون.ڊيگهائي - 1].value = سرگرم_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (وٿي);
      range->features.انگ_features = feature_رڪارڊون.ڊيگهائي - وٿي;
      range->اشاريو_پهريون = آخري_اشاريو;
      range->اشاريو_آخري  = event->اشاريو_2 - 1;

      آخري_اشاريو = event->اشاريو_2;
    }

    إذا (event->شروعات)
    {
      سرگرم_features.push (event->feature);
    }
    وإلا
    {
      خودڪار *feature = سرگرم_features.lsearch (event->feature);
      إذا (feature)
        سرگرم_features.remove_ordered (feature - سرگرم_features.arrayZ);
    }
  }

  إذا (!range_رڪارڊون.ڊيگهائي) /* No active feature found. */
    انگ_features = 0;

  /* Fixup the pointers. */
  لكل (بنانشان صحيح i = 0; i < range_رڪارڊون.ڊيگهائي; i++)
  {
    خودڪار *range = &range_رڪارڊون[i];
    range->features.features = (hb_ms_feature_t *) feature_رڪارڊون + reinterpret_cast<uintptr_t> (range->features.features);
  }

  أرجع !!انگ_features;
}

ساكن مضمن فراغ
hb_ms_make_feature_ranges (hb_ويڪٽر_t<hb_ms_feature_t>      &feature_رڪارڊون,
			   hb_ويڪٽر_t<hb_ms_range_رڪارڊ_t> &range_رڪارڊون,
			   بنانشان صحيح                       chars_وٿي,
			   بنانشان صحيح                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_ويڪٽر_t<hb_ms_features_t*>    &range_features, /*ٻاهريون*/
			   hb_ويڪٽر_t<uint32_t>             &range_counts /*ٻاهريون*/)
{
  range_features.clear ();
  range_counts.clear ();

  خودڪار *آخري_range = &range_رڪارڊون[0];
  لكل (بنانشان صحيح i = chars_وٿي; i < chars_len; i++)
  {
    خودڪار *range = آخري_range;
    طالما (log_clusters[i] < range->اشاريو_پهريون)
      range--;
    طالما (log_clusters[i] > range->اشاريو_آخري)
      range++;
    إذا (!range_features.ڊيگهائي ||
        &range->features != range_features[range_features.ڊيگهائي - 1])
    {
      خودڪار **features = range_features.push ();
      خودڪار *c = range_counts.push ();
      إذا (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        اكسر;
      }
      *features = &range->features;
      *c = 1;
    }
    وإلا
    {
      range_counts[range_counts.ڊيگهائي - 1]++;
    }

    آخري_range = range;
  }
}

#جيڪڏهن_پورو /* HB_MS_FEATURE_RANGES_HH */
