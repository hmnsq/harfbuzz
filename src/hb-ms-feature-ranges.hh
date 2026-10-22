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

#اگر_غیر_تعریف_شدہ HB_MS_FEATURE_RANGES_HH
#تعریف_کرو HB_MS_FEATURE_RANGES_HH

#شامل "hb.hh"

/* Variations of this code exist in hb-coretext-shape.cc as well
 * as hb-aat-map.cc... */

قسمتعریف ساخت hb_ms_feature_t {
  uint32_t لیبل_le;
  uint32_t value;
} hb_ms_feature_t;

قسمتعریف ساخت hb_ms_features_t {
  hb_ms_feature_t *features;
  uint32_t         عدد_features;
} hb_ms_features_t;

ساخت hb_ms_فعال_feature_t {
  hb_ms_feature_t fea;
  غیرعلامتی صحیح ترتیب;

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb) {
    ثابت خودکار *a = (ثابت hb_ms_فعال_feature_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_فعال_feature_t *) pb;
    واپس a->fea.لیبل_le < b->fea.لیبل_le ? -1 : a->fea.لیبل_le > b->fea.لیبل_le ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->fea.value < b->fea.value ? -1 : a->fea.value > b->fea.value ? 1 :
	   0;
  }
  منطقی عامل== (ثابت hb_ms_فعال_feature_t& f) ثابت
  { واپس cmp (یہ, &f) == 0; }
};

ساخت hb_ms_feature_event_t {
  غیرعلامتی صحیح اشاریہ_2;
  منطقی آغاز;
  hb_ms_فعال_feature_t feature;

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت خودکار *a = (ثابت hb_ms_feature_event_t *) pa;
    ثابت خودکار *b = (ثابت hb_ms_feature_event_t *) pb;
    واپس a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 :
	   a->آغاز < b->آغاز ? -1 : a->آغاز > b->آغاز ? 1 :
	   hb_ms_فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

ساخت hb_ms_range_ریکارڈ_t {
  hb_ms_features_t features;
  غیرعلامتی صحیح اشاریہ_پہلا; /* == start */
  غیرعلامتی صحیح اشاریہ_آخری;  /* == end - 1 */
};

ساکن اندرونی منطقی
hb_ms_تیاری_features (ثابت hb_feature_t                *features,
		      غیرعلامتی صحیح                       عدد_features,
		      hb_سمتیہ_t<hb_ms_feature_t>      &feature_اندراجات, /*برآمد*/
		      hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> &range_اندراجات /*برآمد*/)
{
  feature_اندراجات.clear ();
  range_اندراجات.clear ();

  /* Sort features by start/end events. */
  hb_سمتیہ_t<hb_ms_feature_event_t> feature_events;
  برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.لیبل_le = hb_uint32_تبادلہ (features[i].لیبل_2);
    feature.fea.value = features[i].value;
    feature.ترتیب = i;

    hb_ms_feature_event_t *event;

    event = feature_events.push ();
    event->اشاریہ_2 = features[i].آغاز;
    event->آغاز = سچ;
    event->feature = feature;

    event = feature_events.push ();
    event->اشاریہ_2 = features[i].انتہا;
    event->آغاز = جھوٹ;
    event->feature = feature;
  }
  feature_events.qsort ();
  /* Add a strategic final event. */
  {
    hb_ms_فعال_feature_t feature;
    feature.fea.لیبل_le = 0;
    feature.fea.value = 0;
    feature.ترتیب = عدد_features + 1;

    خودکار *event = feature_events.push ();
    event->اشاریہ_2 = 0; /* This value does magic. */
    event->آغاز = جھوٹ;
    event->feature = feature;
  }

  /* Scan events and save features for each range. */
  hb_سمتیہ_t<hb_ms_فعال_feature_t> فعال_features;
  غیرعلامتی صحیح آخری_اشاریہ = 0;
  برائے (غیرعلامتی صحیح i = 0; i < feature_events.لمبائی; i++)
  {
    خودکار *event = &feature_events[i];

    اگر (event->اشاریہ_2 != آخری_اشاریہ)
    {
      /* Save a snapshot of active features and the range. */
      خودکار *range = range_اندراجات.push ();
      خودکار انحراف = feature_اندراجات.لمبائی;

      فعال_features.qsort ();
      برائے (غیرعلامتی صحیح j = 0; j < فعال_features.لمبائی; j++)
      {
        اگر (!j || فعال_features[j].fea.لیبل_le != feature_اندراجات[feature_اندراجات.لمبائی - 1].لیبل_le)
        {
          feature_اندراجات.push (فعال_features[j].fea);
        }
        ورنہ
        {
          /* Overrides value for existing feature. */
          feature_اندراجات[feature_اندراجات.لمبائی - 1].value = فعال_features[j].fea.value;
        }
      }

      /* Will convert to pointer after all is ready, since feature_records.array
       * may move as we grow it. */
      range->features.features = reinterpret_cast<hb_ms_feature_t *> (انحراف);
      range->features.عدد_features = feature_اندراجات.لمبائی - انحراف;
      range->اشاریہ_پہلا = آخری_اشاریہ;
      range->اشاریہ_آخری  = event->اشاریہ_2 - 1;

      آخری_اشاریہ = event->اشاریہ_2;
    }

    اگر (event->آغاز)
    {
      فعال_features.push (event->feature);
    }
    ورنہ
    {
      خودکار *feature = فعال_features.lsearch (event->feature);
      اگر (feature)
        فعال_features.remove_ordered (feature - فعال_features.arrayZ);
    }
  }

  اگر (!range_اندراجات.لمبائی) /* No active feature found. */
    عدد_features = 0;

  /* Fixup the pointers. */
  برائے (غیرعلامتی صحیح i = 0; i < range_اندراجات.لمبائی; i++)
  {
    خودکار *range = &range_اندراجات[i];
    range->features.features = (hb_ms_feature_t *) feature_اندراجات + reinterpret_cast<uintptr_t> (range->features.features);
  }

  واپس !!عدد_features;
}

ساکن اندرونی خالی
hb_ms_make_feature_ranges (hb_سمتیہ_t<hb_ms_feature_t>      &feature_اندراجات,
			   hb_سمتیہ_t<hb_ms_range_ریکارڈ_t> &range_اندراجات,
			   غیرعلامتی صحیح                       chars_انحراف,
			   غیرعلامتی صحیح                       chars_len,
			   uint16_t                          *log_clusters,
			   hb_سمتیہ_t<hb_ms_features_t*>    &range_features, /*برآمد*/
			   hb_سمتیہ_t<uint32_t>             &range_counts /*برآمد*/)
{
  range_features.clear ();
  range_counts.clear ();

  خودکار *آخری_range = &range_اندراجات[0];
  برائے (غیرعلامتی صحیح i = chars_انحراف; i < chars_len; i++)
  {
    خودکار *range = آخری_range;
    جب (log_clusters[i] < range->اشاریہ_پہلا)
      range--;
    جب (log_clusters[i] > range->اشاریہ_آخری)
      range++;
    اگر (!range_features.لمبائی ||
        &range->features != range_features[range_features.لمبائی - 1])
    {
      خودکار **features = range_features.push ();
      خودکار *c = range_counts.push ();
      اگر (unlikely (!features || !c))
      {
        range_features.clear ();
        range_counts.clear ();
        توڑو;
      }
      *features = &range->features;
      *c = 1;
    }
    ورنہ
    {
      range_counts[range_counts.لمبائی - 1]++;
    }

    آخری_range = range;
  }
}

#اگر_ختم /* HB_MS_FEATURE_RANGES_HH */
