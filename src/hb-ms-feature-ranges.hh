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

#جے_بنا_تعریف HB_MS_FEATURE_RANGES_HH
#تعریف_کرو HB_MS_FEATURE_RANGES_HH

#ضمن "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

تعريفنوع بنية hb_ms_feature_t {
  uint32_t ٹیگ_le;
  uint32_t value;
} hb_ms_feature_t;

تعريفنوع بنية hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         عدد_features;
} hb_ms_features_t;

بنية hb_ms_سرگرم_feature_t {
  hb_ms_feature_t fea;
  بنانشانی صحيح ترتیب;

  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت خودکار *a = (ثابت hb_ms_سرگرم_feature_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_سرگرم_feature_t *) pb;
    أرجع a->fea.ٹیگ_le < b->fea.ٹیگ_le ? -1 : a->fea.ٹیگ_le > b->fea.ٹیگ_le ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  منطقي عامل== (ثابت hb_ms_سرگرم_feature_t& f) ثابت
  { أرجع cmp (هذا, &f) == 0; }
};

بنية hb_ms_feature_event_t {
  بنانشانی صحيح اشاریہ_2;
  منطقي شروع;
  hb_ms_سرگرم_feature_t feature;

  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت خودکار *a = (ثابت hb_ms_feature_event_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_feature_event_t *) pb;
    أرجع a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 :
	   a->شروع < b->شروع ? -1 : a->شروع > b->شروع ? 1 :
	   hb_ms_سرگرم_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية hb_ms_range_ریکارڈ_t {
  hb_ms_features_t features;
  بنانشانی صحيح اشاریہ_پہلا; /* == start */
  بنانشانی صحيح اشاریہ_آخری;  /* == end - 1 */
};

ساكن مضمن منطقي
hb_ms_تیاری_features (ثابت hb_feature_t                *features,
		      بنانشانی صحيح                       عدد_features,
		      hb_سمتیہ_t<hb_ms_feature_t>      &feature_ریکارڈاں, /*باہرلا*/
		      hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> &range_ریکارڈاں /*باہرلا*/)
{
  feature_ریکارڈاں.clear ();
  range_ریکارڈاں.clear ();

  /* Sort features by start/end events. */
  hb_سمتیہ_t<hb_ms_feature_event_t> feature_events;
  لكل (بنانشانی صحيح i = 0; i < عدد_features; i++)
  {
    hb_ms_سرگرم_feature_t feature;
    feature.fea.ٹیگ_le = hb_uint32_وٹاندرا (features[i].ٹیگ_2);
    feature.fea.value = features[i].value;
    feature.ترتیب = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->اشاریہ_2 = features[i].شروع;
    event->شروع = سچ;
    event->feature = feature;

    event = feature_events.push ();
    event->اشاریہ_2 = features[i].سرا;
    event->شروع = خطأ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_سرگرم_feature_t feature;
    feature.fea.ٹیگ_le = 0;
    feature.fea.value = 0;
    feature.ترتیب = عدد_features + 1;

    خودکار *event = feature_events.push ();
    event->اشاریہ_2 = 0; /* This value does magic. */
    event->شروع = خطأ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_سمتیہ_t<hb_ms_سرگرم_feature_t> سرگرم_features;
  بنانشانی صحيح آخری_اشاریہ = 0;
  لكل (بنانشانی صحيح i = 0; i < feature_events.لمائی; i++)
  {
    خودکار *event = &feature_events[i];

    إذا (event->اشاریہ_2 != آخری_اشاریہ)
    {
      /* Save a snapshot of active features and the range. */
      خودکار *range = range_ریکارڈاں.push ();
      خودکار وتھ = feature_ریکارڈاں.لمائی;

      سرگرم_features.qsort ();
      لكل (بنانشانی صحيح j = 0; j < سرگرم_features.لمائی; j++)
      {
        إذا (!j || سرگرم_features[j].fea.ٹیگ_le != feature_ریکارڈاں[feature_ریکارڈاں.لمائی - 1].ٹیگ_le)
        {
          feature_ریکارڈاں.push (سرگرم_features[j].fea);
        }
        وإلا
        {
          /* Overrides value for existing feature. */
          feature_ریکارڈاں[feature_ریکارڈاں.لمائی - 1].value = سرگرم_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (وتھ);
      range->features.عدد_features = feature_ریکارڈاں.لمائی - وتھ;
      range->اشاریہ_پہلا = آخری_اشاریہ;
      range->اشاریہ_آخری  = event->اشاریہ_2 - 1;

      آخری_اشاریہ = event->اشاریہ_2;
    }

    إذا (event->شروع)
    {
      سرگرم_features.push (event->feature);
    }
    وإلا
    {
      خودکار *feature = سرگرم_features.lsearch (event->feature);
      إذا (feature)
        سرگرم_features.remove_ordered (feature - سرگرم_features.arrayZ);
    }
  }

  إذا (!range_ریکارڈاں.لمائی) /* No active feature found. */
    عدد_features = 0;

  /* Fixup the pointers. */
  لكل (بنانشانی صحيح i = 0; i < range_ریکارڈاں.لمائی; i++)
  {
    خودکار *range = &range_ریکارڈاں[i];
    range->features.features = (hb_ms_feature_t *) feature_ریکارڈاں + reinterpret_cast<uintptr_t> (range->features.features);
  }

  أرجع !!عدد_features;
}

ساكن مضمن فراغ
hb_ms_make_feature_ranges (hb_سمتیہ_t<hb_ms_feature_t>      &feature_ریکارڈاں,
			   hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> &range_ریکارڈاں,
			   بنانشانی صحيح                       chars_وتھ,
			   بنانشانی صحيح                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_سمتیہ_t<hb_ms_features_t*>    &range_features, /*باہرلا*/
			   hb_سمتیہ_t<uint32_t>             &range_counts /*باہرلا*/)
{
  range_features.clear ();
  range_counts.clear ();

  خودکار *آخری_range = &range_ریکارڈاں[0];
  لكل (بنانشانی صحيح i = chars_وتھ; i < chars_len; i++)
  {
    خودکار *range = آخری_range;
    طالما (log_clusters[i] < range->اشاریہ_پہلا)
      range--;
    طالما (log_clusters[i] > range->اشاریہ_آخری)
      range++;
    إذا (!range_features.لمائی ||
        &range->features != range_features[range_features.لمائی - 1])
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
      range_counts[range_counts.لمائی - 1]++;
    }

    آخری_range = range;
  }
}

#جے_انت /* HB_MS_FEATURE_RANGES_HH */
