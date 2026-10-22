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

#اگر_تعریف_نشده HB_MS_FEATURE_RANGES_HH
#تعریف_کن HB_MS_FEATURE_RANGES_HH

#ضمن "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

تعريفنوع بنية hb_ms_feature_t {
  uint32_t برچسب_le;
  uint32_t value;
} hb_ms_feature_t;

تعريفنوع بنية hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         رقم_features;
} hb_ms_features_t;

بنية hb_ms_فعال_feature_t {
  hb_ms_feature_t fea;
  بیعلامت صحيح ترتیب;

  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت خودکار *a = (ثابت hb_ms_فعال_feature_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_فعال_feature_t *) pb;
    أرجع a->fea.برچسب_le < b->fea.برچسب_le ? -1 : a->fea.برچسب_le > b->fea.برچسب_le ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  منطقي عملگر== (ثابت hb_ms_فعال_feature_t& f) ثابت
  { أرجع cmp (هذا, &f) == 0; }
};

بنية hb_ms_feature_event_t {
  بیعلامت صحيح شاخص_2;
  منطقي شروع;
  hb_ms_فعال_feature_t feature;

  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت خودکار *a = (ثابت hb_ms_feature_event_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_feature_event_t *) pb;
    أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 :
	   a->شروع < b->شروع ? -1 : a->شروع > b->شروع ? 1 :
	   hb_ms_فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية hb_ms_range_رکورد_t {
  hb_ms_features_t features;
  بیعلامت صحيح شاخص_اولی; /* == start */
  بیعلامت صحيح شاخص_آخرین;  /* == end - 1 */
};

ساكن مضمن منطقي
hb_ms_راهاندازی_features (ثابت hb_feature_t                *features,
		      بیعلامت صحيح                       رقم_features,
		      hb_وکتور_t<hb_ms_feature_t>      &feature_رکوردها, /*بیرون*/
		      hb_وکتور_t<hb_ms_range_رکورد_t> &range_رکوردها /*بیرون*/)
{
  feature_رکوردها.clear ();
  range_رکوردها.clear ();

  /* Sort features by start/end events. */
  hb_وکتور_t<hb_ms_feature_event_t> feature_events;
  لكل (بیعلامت صحيح i = 0; i < رقم_features; i++)
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.برچسب_le = hb_uint32_جابجا (features[i].برچسب_2);
    feature.fea.value = features[i].value;
    feature.ترتیب = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->شاخص_2 = features[i].شروع;
    event->شروع = درست;
    event->feature = feature;

    event = feature_events.push ();
    event->شاخص_2 = features[i].انتها;
    event->شروع = خطأ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.برچسب_le = 0;
    feature.fea.value = 0;
    feature.ترتیب = رقم_features + 1;

    خودکار *event = feature_events.push ();
    event->شاخص_2 = 0; /* This value does magic. */
    event->شروع = خطأ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_وکتور_t<hb_ms_فعال_feature_t> فعال_features;
  بیعلامت صحيح آخرین_شاخص = 0;
  لكل (بیعلامت صحيح i = 0; i < feature_events.درازی; i++)
  {
    خودکار *event = &feature_events[i];

    إذا (event->شاخص_2 != آخرین_شاخص)
    {
      /* Save a snapshot of active features and the range. */
      خودکار *range = range_رکوردها.push ();
      خودکار جابه_جایی = feature_رکوردها.درازی;

      فعال_features.qsort ();
      لكل (بیعلامت صحيح j = 0; j < فعال_features.درازی; j++)
      {
        إذا (!j || فعال_features[j].fea.برچسب_le != feature_رکوردها[feature_رکوردها.درازی - 1].برچسب_le)
        {
          feature_رکوردها.push (فعال_features[j].fea);
        }
        وإلا
        {
          /* Overrides value for existing feature. */
          feature_رکوردها[feature_رکوردها.درازی - 1].value = فعال_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (جابه_جایی);
      range->features.رقم_features = feature_رکوردها.درازی - جابه_جایی;
      range->شاخص_اولی = آخرین_شاخص;
      range->شاخص_آخرین  = event->شاخص_2 - 1;

      آخرین_شاخص = event->شاخص_2;
    }

    إذا (event->شروع)
    {
      فعال_features.push (event->feature);
    }
    وإلا
    {
      خودکار *feature = فعال_features.lsearch (event->feature);
      إذا (feature)
        فعال_features.remove_ordered (feature - فعال_features.arrayZ);
    }
  }

  إذا (!range_رکوردها.درازی) /* No active feature found. */
    رقم_features = 0;

  /* Fixup the pointers. */
  لكل (بیعلامت صحيح i = 0; i < range_رکوردها.درازی; i++)
  {
    خودکار *range = &range_رکوردها[i];
    range->features.features = (hb_ms_feature_t *) feature_رکوردها + reinterpret_cast<uintptr_t> (range->features.features);
  }

  أرجع !!رقم_features;
}

ساكن مضمن فراغ
hb_ms_make_feature_ranges (hb_وکتور_t<hb_ms_feature_t>      &feature_رکوردها,
			   hb_وکتور_t<hb_ms_range_رکورد_t> &range_رکوردها,
			   بیعلامت صحيح                       chars_جابه_جایی,
			   بیعلامت صحيح                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_وکتور_t<hb_ms_features_t*>    &range_features, /*بیرون*/
			   hb_وکتور_t<uint32_t>             &range_counts /*بیرون*/)
{
  range_features.clear ();
  range_counts.clear ();

  خودکار *آخرین_range = &range_رکوردها[0];
  لكل (بیعلامت صحيح i = chars_جابه_جایی; i < chars_len; i++)
  {
    خودکار *range = آخرین_range;
    طالما (log_clusters[i] < range->شاخص_اولی)
      range--;
    طالما (log_clusters[i] > range->شاخص_آخرین)
      range++;
    إذا (!range_features.درازی ||
        &range->features != range_features[range_features.درازی - 1])
    {
      خودکار **features = range_features.push ();
      خودکار *c = range_counts.push ();
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
      range_counts[range_counts.درازی - 1]++;
    }

    آخرین_range = range;
  }
}

#پایان_اگر /* HB_MS_FEATURE_RANGES_HH */
