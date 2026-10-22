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

#که_نه_تعریف_شوی HB_MS_FEATURE_RANGES_HH
#تعریف_کړه HB_MS_FEATURE_RANGES_HH

#ضمن "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

تعريفنوع بنية hb_ms_feature_t {
  uint32_t نښان_le;
  uint32_t value;
} hb_ms_feature_t;

تعريفنوع بنية hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         عدد_features;
} hb_ms_features_t;

بنية hb_ms_فعال_feature_t {
  hb_ms_feature_t fea;
  بینښه صحيح ترتیب;

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت خپلکاری *a = (ثابت hb_ms_فعال_feature_t *) pa;
    ثابت خپلکاری *b = (ثابت hb_ms_فعال_feature_t *) pb;
    أرجع a->fea.نښان_le < b->fea.نښان_le ? -1 : a->fea.نښان_le > b->fea.نښان_le ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  منطقي چلوونکی== (ثابت hb_ms_فعال_feature_t& f) ثابت
  { أرجع cmp (هذا, &f) == 0; }
};

بنية hb_ms_feature_event_t {
  بینښه صحيح شاخص_2;
  منطقي پیل;
  hb_ms_فعال_feature_t feature;

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت خپلکاری *a = (ثابت hb_ms_feature_event_t *) pa;
    ثابت خپلکاری *b = (ثابت hb_ms_feature_event_t *) pb;
    أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 :
	   a->پیل < b->پیل ? -1 : a->پیل > b->پیل ? 1 :
	   hb_ms_فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية hb_ms_range_ریکارډ_t {
  hb_ms_features_t features;
  بینښه صحيح شاخص_لومړی; /* == start */
  بینښه صحيح شاخص_وروستی;  /* == end - 1 */
};

ساكن مضمن منطقي
hb_ms_تیاری_features (ثابت hb_feature_t                *features,
		      بینښه صحيح                       عدد_features,
		      hb_ویکټور_t<hb_ms_feature_t>      &feature_ریکارډونه, /*بهر*/
		      hb_ویکټور_t<hb_ms_range_ریکارډ_t> &range_ریکارډونه /*بهر*/)
{
  feature_ریکارډونه.clear ();
  range_ریکارډونه.clear ();

  /* Sort features by start/end events. */
  hb_ویکټور_t<hb_ms_feature_event_t> feature_events;
  لكل (بینښه صحيح i = 0; i < عدد_features; i++)
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.نښان_le = hb_uint32_بدلون (features[i].نښان_2);
    feature.fea.value = features[i].value;
    feature.ترتیب = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->شاخص_2 = features[i].پیل;
    event->پیل = ریښتیا;
    event->feature = feature;

    event = feature_events.push ();
    event->شاخص_2 = features[i].پای_ټکی;
    event->پیل = خطأ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.نښان_le = 0;
    feature.fea.value = 0;
    feature.ترتیب = عدد_features + 1;

    خپلکاری *event = feature_events.push ();
    event->شاخص_2 = 0; /* This value does magic. */
    event->پیل = خطأ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_ویکټور_t<hb_ms_فعال_feature_t> فعال_features;
  بینښه صحيح وروستی_شاخص = 0;
  لكل (بینښه صحيح i = 0; i < feature_events.اوږدتیا; i++)
  {
    خپلکاری *event = &feature_events[i];

    إذا (event->شاخص_2 != وروستی_شاخص)
    {
      /* Save a snapshot of active features and the range. */
      خپلکاری *range = range_ریکارډونه.push ();
      خپلکاری دننه_واټن = feature_ریکارډونه.اوږدتیا;

      فعال_features.qsort ();
      لكل (بینښه صحيح j = 0; j < فعال_features.اوږدتیا; j++)
      {
        إذا (!j || فعال_features[j].fea.نښان_le != feature_ریکارډونه[feature_ریکارډونه.اوږدتیا - 1].نښان_le)
        {
          feature_ریکارډونه.push (فعال_features[j].fea);
        }
        وإلا
        {
          /* Overrides value for existing feature. */
          feature_ریکارډونه[feature_ریکارډونه.اوږدتیا - 1].value = فعال_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (دننه_واټن);
      range->features.عدد_features = feature_ریکارډونه.اوږدتیا - دننه_واټن;
      range->شاخص_لومړی = وروستی_شاخص;
      range->شاخص_وروستی  = event->شاخص_2 - 1;

      وروستی_شاخص = event->شاخص_2;
    }

    إذا (event->پیل)
    {
      فعال_features.push (event->feature);
    }
    وإلا
    {
      خپلکاری *feature = فعال_features.lsearch (event->feature);
      إذا (feature)
        فعال_features.remove_ordered (feature - فعال_features.arrayZ);
    }
  }

  إذا (!range_ریکارډونه.اوږدتیا) /* No active feature found. */
    عدد_features = 0;

  /* Fixup the pointers. */
  لكل (بینښه صحيح i = 0; i < range_ریکارډونه.اوږدتیا; i++)
  {
    خپلکاری *range = &range_ریکارډونه[i];
    range->features.features = (hb_ms_feature_t *) feature_ریکارډونه + reinterpret_cast<uintptr_t> (range->features.features);
  }

  أرجع !!عدد_features;
}

ساكن مضمن فراغ
hb_ms_make_feature_ranges (hb_ویکټور_t<hb_ms_feature_t>      &feature_ریکارډونه,
			   hb_ویکټور_t<hb_ms_range_ریکارډ_t> &range_ریکارډونه,
			   بینښه صحيح                       chars_دننه_واټن,
			   بینښه صحيح                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_ویکټور_t<hb_ms_features_t*>    &range_features, /*بهر*/
			   hb_ویکټور_t<uint32_t>             &range_counts /*بهر*/)
{
  range_features.clear ();
  range_counts.clear ();

  خپلکاری *وروستی_range = &range_ریکارډونه[0];
  لكل (بینښه صحيح i = chars_دننه_واټن; i < chars_len; i++)
  {
    خپلکاری *range = وروستی_range;
    طالما (log_clusters[i] < range->شاخص_لومړی)
      range--;
    طالما (log_clusters[i] > range->شاخص_وروستی)
      range++;
    إذا (!range_features.اوږدتیا ||
        &range->features != range_features[range_features.اوږدتیا - 1])
    {
      خپلکاری **features = range_features.push ();
      خپلکاری *c = range_counts.push ();
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
      range_counts[range_counts.اوږدتیا - 1]++;
    }

    وروستی_range = range;
  }
}

#د_که_پای /* HB_MS_FEATURE_RANGES_HH */
