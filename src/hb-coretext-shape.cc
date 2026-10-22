/*
 * Copyright © 2012,2013  Mozilla Foundation.
 * Copyright © 2012,2013  Google, Inc.
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
 * Mozilla Author(s): Jonathan Kew
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_CORETEXT

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-coretext.hh"
#అన్తర్భావ "hb-aat-layout.hh"

hb_coretext_face_దత్తాంశం_t *
hb_coretext_shaper_face_దత్తాంశం_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  ఒకవేళ (unlikely (!cg_font))
  {
    DEBUG_సందేశం (CORETEXT, face, "CGFont creation failed..");
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి (hb_coretext_face_దత్తాంశం_t *) cg_font;
}

శూన్య
hb_coretext_shaper_face_దత్తాంశం_destroy (hb_coretext_face_దత్తాంశం_t *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం)
    CFRelease ((CGFontRef) దత్తాంశం);
}


hb_coretext_font_దత్తాంశం_t *
hb_coretext_shaper_font_దత్తాంశం_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  స్థిరరాశి hb_coretext_face_దత్తాంశం_t *face_దత్తాంశం = face->దత్తాంశం.coretext;
  ఒకవేళ (unlikely (!face_దత్తాంశం)) తిరిగి శూన్యసూచిక;
  CGFontRef cg_font = (CGFontRef) (స్థిరరాశి శూన్య *) face->దత్తాంశం.coretext;

  CGFloat font_కొలత = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_కొలత);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_కొలత));

  ఒకవేళ (unlikely (!ct_font))
  {
    DEBUG_సందేశం (CORETEXT, font, "CGFont creation failed..");
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (font->అంకె_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->అంకె_coords,
				 &k_CFType_Dictionary_కీలకం_Call_Backs,
				 &k_CFType_Dictionary_విలువ_Call_Backs));

    గుర్తులేని లెక్క = font->అంకె_coords;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      hb_ot_var_axis_info_t info;
      గుర్తులేని పూర్ణాంకం c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      దశాంశం v = hb_clamp (font->design_coords[i], info.min_విలువ, info.max_విలువ);

      hb_cf_ptr_t<CFNumberRef> ట్యాగ్_సంఖ్య (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.ట్యాగ్_2));
      hb_cf_ptr_t<CFNumberRef> విలువ_సంఖ్య (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      ఒకవేళ (variations && ట్యాగ్_సంఖ్య && విలువ_సంఖ్య)
	CFDictionary_సమితి_విలువ (variations, ట్యాగ్_సంఖ్య, విలువ_సంఖ్య);
    }

    స్థిరరాశి శూన్య *కీలకాలు[] = { kCTFontVariationAttribute };
    స్థిరరాశి శూన్య *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> లక్షణాలు (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       కీలకాలు,
				       values,
				       1,
				       &k_CFType_Dictionary_కీలకం_Call_Backs,
				       &k_CFType_Dictionary_విలువ_Call_Backs) : శూన్యసూచిక);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (లక్షణాలు ? CTFont_Descriptor_Create_With_లక్షణాలు (లక్షణాలు) : శూన్యసూచిక);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_లక్షణాలు (ct_font, 0, శూన్యసూచిక, varDesc) : శూన్యసూచిక);
    ఒకవేళ (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  తిరిగి (hb_coretext_font_దత్తాంశం_t *) ct_font.release ();
}

శూన్య
hb_coretext_shaper_font_దత్తాంశం_destroy (hb_coretext_font_దత్తాంశం_t *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం)
    CFRelease ((CTFontRef) దత్తాంశం);
}

/*
 * shaper
 */

నిర్మాణం feature_రికార్డు_t {
  గుర్తులేని పూర్ణాంకం feature;
  గుర్తులేని పూర్ణాంకం setting;
};

నిర్మాణం చురుకైన_feature_t {
  feature_రికార్డు_t rec;
  గుర్తులేని పూర్ణాంకం క్రమం;

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb) {
    స్థిరరాశి చురుకైన_feature_t *a = (స్థిరరాశి చురుకైన_feature_t *) pa;
    స్థిరరాశి చురుకైన_feature_t *b = (స్థిరరాశి చురుకైన_feature_t *) pb;
    తిరిగి a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->క్రమం < b->క్రమం ? -1 : a->క్రమం > b->క్రమం ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  తార్కిక క్రియాకారకం== (స్థిరరాశి చురుకైన_feature_t& f) స్థిరరాశి {
    తిరిగి cmp (ఇది, &f) == 0;
  }
};

నిర్మాణం feature_event_t {
  గుర్తులేని పూర్ణాంకం సూచీ_2;
  తార్కిక ప్రారంభం;
  చురుకైన_feature_t feature;

  HB_అంతర్గత స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య *pa, స్థిరరాశి శూన్య *pb) {
    స్థిరరాశి feature_event_t *a = (స్థిరరాశి feature_event_t *) pa;
    స్థిరరాశి feature_event_t *b = (స్థిరరాశి feature_event_t *) pb;
    తిరిగి a->సూచీ_2 < b->సూచీ_2 ? -1 : a->సూచీ_2 > b->సూచీ_2 ? 1 :
	   a->ప్రారంభం < b->ప్రారంభం ? -1 : a->ప్రారంభం > b->ప్రారంభం ? 1 :
	   చురుకైన_feature_t::cmp (&a->feature, &b->feature);
  }
};

నిర్మాణం range_రికార్డు_t {
  CTFontRef font;
  గుర్తులేని పూర్ణాంకం సూచీ_మొదటి; /* == start */
  గుర్తులేని పూర్ణాంకం సూచీ_చివరి;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_బఫరు_t        *బఫరు,
		    స్థిరరాశి hb_feature_t *features,
		    గుర్తులేని పూర్ణాంకం        అంకె_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (స్థిరరాశి శూన్య *) face->దత్తాంశం.coretext;
  CTFontRef ct_font = (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext;

  CGFloat ct_font_కొలత = CTFont_Get_కొలత (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_కొలత;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_కొలత;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  ఒకవేళ (HB_బఫరు_CLUSTER_LEVEL_IS_GRAPHEMES (బఫరు->cluster_level))
  {
    hb_unicode_funcs_t *unicode = బఫరు->unicode;
    గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
      ఒకవేళ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	బఫరు->విలీనం_grapheme_clusters (i - 1, i + 1);
  }

  hb_సదిశ_t<range_రికార్డు_t> range_రికార్డులు;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  ఒకవేళ (అంకె_features)
  {
    /* Sort features by start/end events. */
    hb_సదిశ_t<feature_event_t> feature_events;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
    {
      చురుకైన_feature_t feature;

#ఒకవేళ MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 101000
      స్థిరరాశి hb_aat_feature_mapping_t * mapping = hb_aat_layout_కనుగొను_feature_mapping (features[i].ట్యాగ్_2);
      ఒకవేళ (!mapping)
	కొనసాగు;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_గమ్యం_Enable : mapping->selector_గమ్యం_Disable;
#లేకపోతే
      feature.rec.feature = features[i].ట్యాగ్_2;
      feature.rec.setting = features[i].value;
#అయితే_ముగింపు
      feature.క్రమం = i;

      feature_event_t *event;

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
      చురుకైన_feature_t feature;
      feature.rec.feature = HB_ట్యాగ్_NONE;
      feature.rec.setting = 0;
      feature.క్రమం = అంకె_features + 1;

      feature_event_t *event = feature_events.push ();
      event->సూచీ_2 = 0; /* This value does magic. */
      event->ప్రారంభం = అబద్ధం;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_సదిశ_t<చురుకైన_feature_t> చురుకైన_features;
    గుర్తులేని పూర్ణాంకం చివరి_సూచీ = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < feature_events.నిడివి; i++)
    {
      feature_event_t *event = &feature_events[i];

      ఒకవేళ (event->సూచీ_2 != చివరి_సూచీ)
      {
	/* Save a snapshot of active features and the range. */
	range_రికార్డు_t *range = range_రికార్డులు.push ();

	ఒకవేళ (చురుకైన_features.నిడివి)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < చురుకైన_features.నిడివి; j++)
	  {
#ఒకవేళ MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 101000
	    CFStringRef కీలకాలు[] = {
	      k_CTFont_Feature_Type_Identifier_కీలకం,
	      k_CTFont_Feature_Selector_Identifier_కీలకం
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &చురుకైన_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &చురుకైన_features[j].rec.setting)
	    };
#లేకపోతే
	    అక్షరం ట్యాగ్_2[5] = {HB_UNTAG (చురుకైన_features[j].rec.feature)};
	    CFTypeRef కీలకాలు[] = {
	      k_CTFont_Open_Type_Feature_ట్యాగ్,
	      k_CTFont_Open_Type_Feature_విలువ
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, ట్యాగ్_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &చురుకైన_features[j].rec.setting)
	    };
#అయితే_ముగింపు
	    static_assert ((ARRAY_నిడివి_CONST (కీలకాలు) == ARRAY_నిడివి_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (స్థిరరాశి శూన్య **) కీలకాలు,
						       (స్థిరరాశి శూన్య **) values,
						       ARRAY_నిడివి (కీలకాలు),
						       &k_CFType_Dictionary_కీలకం_Call_Backs,
						       &k_CFType_Dictionary_విలువ_Call_Backs);
	    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_విలువ (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef లక్షణాలు = CFDictionaryCreate (kCFAllocatorDefault,
							   (స్థిరరాశి శూన్య **) &kCTFontFeatureSettingsAttribute,
							   (స్థిరరాశి శూన్య **) &features_array,
							   1,
							   &k_CFType_Dictionary_కీలకం_Call_Backs,
							   &k_CFType_Dictionary_విలువ_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_లక్షణాలు (లక్షణాలు);
	  CFRelease (లక్షణాలు);

	  range->font = CTFont_Create_Copy_With_లక్షణాలు (ct_font, 0.0, శూన్యసూచిక, font_desc);
	  CFRelease (font_desc);
	}
	లేకపోతే
	{
	  range->font = శూన్యసూచిక;
	}

	range->సూచీ_మొదటి = చివరి_సూచీ;
	range->సూచీ_చివరి  = event->సూచీ_2 - 1;

	చివరి_సూచీ = event->సూచీ_2;
      }

      ఒకవేళ (event->ప్రారంభం)
      {
	చురుకైన_features.push (event->feature);
      } లేకపోతే {
	చురుకైన_feature_t *feature = చురుకైన_features.lsearch (event->feature);
	ఒకవేళ (feature)
	  చురుకైన_features.remove_ordered (feature - చురుకైన_features.arrayZ);
      }
    }
  }

  గుర్తులేని పూర్ణాంకం scratch_కొలత;
  hb_బఫరు_t::scratch_బఫరు_t *scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);

#నిర్వచించు ALLOCATE_ARRAY(Type, name, len, on_no_స్థలం) \
  Type *name = (Type *) scratch; \
  చేయి { \
    గుర్తులేని పూర్ణాంకం _consumed = DIV_CEIL ((len) * పరిమాణం (Type), పరిమాణం (*scratch)); \
    ఒకవేళ (unlikely (_consumed > scratch_కొలత)) \
    { \
      on_no_స్థలం; \
      ప్రతిజ్ఞా (0); \
    } \
    scratch += _consumed; \
    scratch_కొలత -= _consumed; \
  } అయితే (0)

  ALLOCATE_ARRAY (UniChar, pchars, బఫరు->len * 2, ((శూన్య)శూన్యసూచిక) /*nothing*/);
  గుర్తులేని పూర్ణాంకం chars_len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++) {
    hb_codepoint_t c = బఫరు->info[i].codepoint;
    ఒకవేళ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    లేకపోతే ఒకవేళ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    లేకపోతే {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (గుర్తులేని పూర్ణాంకం, log_clusters, chars_len, ((శూన్య)శూన్యసూచిక) /*nothing*/);
  chars_len = 0;
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < బఫరు->len; i++)
  {
    hb_codepoint_t c = బఫరు->info[i].codepoint;
    గుర్తులేని పూర్ణాంకం cluster = బఫరు->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ఒకవేళ (hb_లోపల_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#నిర్వచించు FAIL(...) \
  HB_STMT_ప్రారంభం { \
    DEBUG_సందేశం (CORETEXT, శూన్యసూచిక, __VA_ARGS__); \
    ret = అబద్ధం; \
    వెళ్ళు fail; \
  } HB_STMT_చివర

  తార్కిక ret = నిజం;
  CFStringRef string_ref = శూన్యసూచిక;
  CTLineRef పంక్తి = శూన్యసూచిక;

  ఒకవేళ (అబద్ధం)
  {
resize_and_retry:
    DEBUG_సందేశం (CORETEXT, బఫరు, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    ప్రతిజ్ఞా (string_ref);
    ప్రతిజ్ఞా (పంక్తి);
    CFRelease (string_ref);
    CFRelease (పంక్తి);
    string_ref = శూన్యసూచిక;
    పంక్తి = శూన్యసూచిక;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    గుర్తులేని పూర్ణాంకం old_scratch_వాడిన;
    hb_బఫరు_t::scratch_బఫరు_t *old_scratch;
    old_scratch = బఫరు->get_scratch_బఫరు (&old_scratch_వాడిన);
    old_scratch_వాడిన = scratch - old_scratch;

    ఒకవేళ (unlikely (!బఫరు->ensure (బఫరు->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = బఫరు->get_scratch_బఫరు (&scratch_కొలత);
    pchars = reinterpret_cast<UniChar *> (((అక్షరం *) scratch + ((అక్షరం *) pchars - (అక్షరం *) old_scratch)));
    log_clusters = reinterpret_cast<గుర్తులేని పూర్ణాంకం *> (((అక్షరం *) scratch + ((అక్షరం *) log_clusters - (అక్షరం *) old_scratch)));
    scratch += old_scratch_వాడిన;
    scratch_కొలత -= old_scratch_వాడిన;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (శూన్యసూచిక,
						     pchars, chars_len,
						     kCFAllocatorNull);
    ఒకవేళ (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      ఒకవేళ (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      ఒకవేళ (HB_DIRECTION_IS_VERTICAL (బఫరు->props.direction))
      {
	CFAttributed_String_సమితి_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      ఒకవేళ (బఫరు->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#ఒకవేళ !(defined(లక్ష్యం_OS_IPHONE) && లక్ష్యం_OS_IPHONE) && MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#అయితే_ముగింపు
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_గమ్యం_string (బఫరు->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	ఒకవేళ (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_సమితి_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_సమితి_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      ఒకవేళ (అంకె_features && range_రికార్డులు.నిడివి)
      {
	గుర్తులేని పూర్ణాంకం ప్రారంభం = 0;
	range_రికార్డు_t *చివరి_range = &range_రికార్డులు[0];
	కొరకు (గుర్తులేని పూర్ణాంకం k = 0; k < chars_len; k++)
	{
	  range_రికార్డు_t *range = చివరి_range;
	  అయితే (log_clusters[k] < range->సూచీ_మొదటి)
	    range--;
	  అయితే (log_clusters[k] > range->సూచీ_చివరి)
	    range++;
	  ఒకవేళ (range != చివరి_range)
	  {
	    ఒకవేళ (చివరి_range->font)
	      CFAttributed_String_సమితి_Attribute (attr_string, CFRangeMake (ప్రారంభం, k - ప్రారంభం),
					      kCTFontAttributeName, చివరి_range->font);

	    ప్రారంభం = k;
	  }

	  చివరి_range = range;
	}
	ఒకవేళ (ప్రారంభం != chars_len && చివరి_range->font)
	  CFAttributed_String_సమితి_Attribute (attr_string, CFRangeMake (ప్రారంభం, chars_len - ప్రారంభం),
					  kCTFontAttributeName, చివరి_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      ఒకవేళ (అంకె_features)
      {
	గుర్తులేని పూర్ణాంకం zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_features; i++)
	{
	  స్థిరరాశి hb_feature_t &feature = features[i];
	  ఒకవేళ (feature.ట్యాగ్_2 == HB_ట్యాగ్('k','e','r','n') &&
	      feature.ప్రారంభం < chars_len && feature.ప్రారంభం < feature.చివర)
	  {
	    CFRange feature_range = CFRangeMake (feature.ప్రారంభం,
						 hb_min (feature.చివర, chars_len) - feature.ప్రారంభం);
	    ఒకవేళ (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    లేకపోతే
	      CFAttributed_String_సమితి_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      పూర్ణాంకం level = HB_DIRECTION_IS_FORWARD (బఫరు->props.direction) ? 0 : 1;
      CFNumberRef level_సంఖ్య = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#ఒకవేళ !(defined(లక్ష్యం_OS_IPHONE) && లక్ష్యం_OS_IPHONE) && MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 1060
      బాహ్య స్థిరరాశి CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#అయితే_ముగింపు
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (స్థిరరాశి శూన్య **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (స్థిరరాశి శూన్య **) &level_సంఖ్య,
						    1,
						    &k_CFType_Dictionary_కీలకం_Call_Backs,
						    &k_CFType_Dictionary_విలువ_Call_Backs);
      CFRelease (level_సంఖ్య);
      ఒకవేళ (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      ఒకవేళ (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      పంక్తి = CTTypesetter_Create_పంక్తి (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      ఒకవేళ (unlikely (!పంక్తి))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (పంక్తి);
    గుర్తులేని పూర్ణాంకం అంకె_runs = CFArray_Get_లెక్క (glyph_runs);
    DEBUG_సందేశం (CORETEXT, శూన్యసూచిక, "Num runs: %u", అంకె_runs);

    బఫరు->len = 0;
    uint32_t స్థితి_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    ఒకవేళ (HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction))
    {
      advances_so_far -= CTLine_Get_Trailing_Whitespace_వెడల్పు (పంక్తి);
      ఒకవేళ (HB_DIRECTION_IS_VERTICAL (బఫరు->props.direction))
	  advances_so_far = -advances_so_far;
    }

    స్థిరరాశి CFRange range_all = CFRangeMake (0, 0);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < అంకె_runs; i++)
    {
      CTRunRef నిరంతరం = static_cast<CTRunRef>(CFArray_Get_విలువ_At_సూచీ (glyph_runs, i));
      CTRun_స్థితి నిరంతరం_స్థితి = CTRun_Get_స్థితి (నిరంతరం);
      స్థితి_or  |= నిరంతరం_స్థితి;
      DEBUG_సందేశం (CORETEXT, నిరంతరం, "CTRunStatus: %x", నిరంతరం_స్థితి);
      CGFloat నిరంతరం_advance = CTRunGetTypographicBounds (నిరంతరం, range_all, శూన్యసూచిక, శూన్యసూచిక, శూన్యసూచిక);
      ఒకవేళ (HB_DIRECTION_IS_VERTICAL (బఫరు->props.direction))
	  నిరంతరం_advance = -నిరంతరం_advance;
      DEBUG_సందేశం (CORETEXT, నిరంతరం, "Run advance: %g", (ద్విగుణ) నిరంతరం_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef లక్షణాలు = CTRun_Get_లక్షణాలు (నిరంతరం);
      CTFontRef నిరంతరం_ct_font = static_cast<CTFontRef>(CFDictionary_Get_విలువ (లక్షణాలు, kCTFontAttributeName));
      ఒకవేళ (!CFEqual (నిరంతరం_ct_font, ct_font))
      {
	/* The run doesn't use our main font instance.  We have to figure out
	 * whether font fallback happened, or this is just CoreText giving us
	 * another CTFont using the same underlying CGFont.  CoreText seems
	 * to do that in a variety of situations, one of which being vertical
	 * text, but also perhaps for caching reasons.
	 *
	 * First, see if it uses any of our subfonts created to set font features...
	 *
	 * Next, compare the CGFont to the one we used to create our fonts.
	 * Even this doesn't work all the time.
	 *
	 * Finally, we compare PS names, which I don't think are unique...
	 *
	 * Looks like if we really want to be sure here we have to modify the
	 * font to change the name table, similar to what we do in the uniscribe
	 * backend.
	 *
	 * However, even that wouldn't work if we were passed in the CGFont to
	 * construct a hb_face to begin with.
	 *
	 * See: https://github.com/harfbuzz/harfbuzz/pull/36
	 *
	 * Also see: https://bugs.chromium.org/p/chromium/issues/detail?id=597098
	 */
	తార్కిక matched = అబద్ధం;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < range_రికార్డులు.నిడివి; i++)
	  ఒకవేళ (range_రికార్డులు[i].font && CFEqual (నిరంతరం_ct_font, range_రికార్డులు[i].font))
	  {
	    matched = నిజం;
	    ఆపు;
	  }
	ఒకవేళ (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> నిరంతరం_cg_font (CTFontCopyGraphicsFont (నిరంతరం_ct_font, శూన్యసూచిక));
	  ఒకవేళ (నిరంతరం_cg_font)
	    matched = CFEqual (నిరంతరం_cg_font, cg_font);
	}
	ఒకవేళ (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_కీలకం));
	  hb_cf_ptr_t<CFStringRef> నిరంతరం_ps_name (CTFontCopyName (నిరంతరం_ct_font, k_CTFont_Post_Script_Name_కీలకం));
	  ఒకవేళ (font_ps_name && నిరంతరం_ps_name)
	  {
	    ఒకవేళ (CFStringCompare (నిరంతరం_ps_name, font_ps_name, 0) == k_CFCompare_Equal_గమ్యం)
	      matched = నిజం;
	  }
	}
	ఒకవేళ (!matched)
	{
	  CFRange range = CTRunGetStringRange (నిరంతరం);
	  DEBUG_సందేశం (CORETEXT, నిరంతరం, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.నిడివి);
	  ఒకవేళ (!బఫరు->ensure_inplace (బఫరు->len + range.నిడివి))
	    వెళ్ళు resize_and_retry;
	  hb_glyph_info_t *info = బఫరు->info + బఫరు->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = బఫరు->props.direction;
	  hb_position_t x_advance, y_advance, x_స్థానభ్రంశం_2, y_స్థానభ్రంశం_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_స్థానభ్రంశం_2, &y_స్థానభ్రంశం_2);
	  hb_position_t advance = hb_saturate_కూడు (x_advance, y_advance);
	  x_స్థానభ్రంశం_2 = hb_saturate_neg (x_స్థానభ్రంశం_2);
	  y_స్థానభ్రంశం_2 = hb_saturate_neg (y_స్థానభ్రంశం_2);

	  గుర్తులేని పూర్ణాంకం old_len = బఫరు->len;
	  కొరకు (CFIndex j = range.location; j < range.location + range.నిడివి; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_సూచీ (string_ref, j);
	      ఒకవేళ (hb_లోపల_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_సూచీ (string_ref, j - 1);
		ఒకవేళ (hb_లోపల_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  కొనసాగు;
	      }
	      ఒకవేళ (బఫరు->unicode->is_default_ignorable (ch))
		కొనసాగు;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->ముసుగు = advance;
	      info->var1.i32 = x_స్థానభ్రంశం_2;
	      info->var2.i32 = y_స్థానభ్రంశం_2;

	      info++;
	      బఫరు->len++;
	  }
	  ఒకవేళ (HB_DIRECTION_IS_BACKWARD (బఫరు->props.direction))
	    బఫరు->తిరగేయి_range (old_len, బఫరు->len);
	  advances_so_far += నిరంతరం_advance;
	  కొనసాగు;
	}
      }

      గుర్తులేని పూర్ణాంకం అంకె_glyphs = CTRun_Get_Glyph_లెక్క (నిరంతరం);
      ఒకవేళ (అంకె_glyphs == 0)
	కొనసాగు;

      ఒకవేళ (!బఫరు->ensure_inplace (బఫరు->len + అంకె_glyphs))
	వెళ్ళు resize_and_retry;

      hb_glyph_info_t *నిరంతరం_info = బఫరు->info + బఫరు->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#నిర్వచించు USE_PTR నిజం

#నిర్వచించు SCRATCH_SAVE() \
  గుర్తులేని పూర్ణాంకం scratch_కొలత_saved = scratch_కొలత; \
  hb_బఫరు_t::scratch_బఫరు_t *scratch_saved = scratch

#నిర్వచించు SCRATCH_RESTORE() \
  scratch_కొలత = scratch_కొలత_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	స్థిరరాశి CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (నిరంతరం) : శూన్యసూచిక;
	ఒకవేళ (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, అంకె_glyphs, వెళ్ళు resize_and_retry);
	  CTRunGetGlyphs (నిరంతరం, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	స్థిరరాశి CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (నిరంతరం) : శూన్యసూచిక;
	ఒకవేళ (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, సూచీ_buf, అంకె_glyphs, వెళ్ళు resize_and_retry);
	  CTRunGetStringIndices (నిరంతరం, range_all, సూచీ_buf);
	  string_indices = సూచీ_buf;
	}
	hb_glyph_info_t *info = నిరంతరం_info;
	కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < అంకె_glyphs; j++)
	{
	  info->codepoint = glyphs[j];
	  info->cluster = log_clusters[string_indices[j]];
	  info++;
	}
	SCRATCH_RESTORE();
      }
      {
	/* Setup positions.
	 * Note that CoreText does not return advances for glyphs.  As such,
	 * for all but last glyph, we use the delta position to next glyph as
	 * advance (in the advance direction only), and for last glyph we set
	 * whatever is needed to make the whole run's advance add up. */
	SCRATCH_SAVE();
	స్థిరరాశి CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (నిరంతరం) : శూన్యసూచిక;
	ఒకవేళ (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, అంకె_glyphs, వెళ్ళు resize_and_retry);
	  CTRunGetPositions (నిరంతరం, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = నిరంతరం_info;
	ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
	{
	  hb_position_t x_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < అంకె_glyphs; j++)
	  {
	    CGFloat advance;
	    ఒకవేళ (likely (j + 1 < అంకె_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    లేకపోతే /* last glyph */
	      advance = నిరంతరం_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->ముసుగు = (hb_ముసుగు_t) hb_clamp_గమ్యం<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_స్థానభ్రంశం_2;
	    info->var2.i32 = hb_clamp_గమ్యం<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	లేకపోతే
	{
	  hb_position_t y_స్థానభ్రంశం_2 = hb_clamp_గమ్యం<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < అంకె_glyphs; j++)
	  {
	    CGFloat advance;
	    ఒకవేళ (likely (j + 1 < అంకె_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    లేకపోతే /* last glyph */
	      advance = నిరంతరం_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->ముసుగు = (hb_ముసుగు_t) hb_clamp_గమ్యం<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_గమ్యం<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_స్థానభ్రంశం_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += నిరంతరం_advance;
      }
#నిర్వచనం_రద్దు SCRATCH_RESTORE
#నిర్వచనం_రద్దు SCRATCH_SAVE
#నిర్వచనం_రద్దు USE_PTR
#నిర్వచనం_రద్దు ALLOCATE_ARRAY

      బఫరు->len += అంకె_glyphs;
    }

    బఫరు->clear_positions ();

    గుర్తులేని పూర్ణాంకం లెక్క = బఫరు->len;
    hb_glyph_info_t *info = బఫరు->info;
    hb_glyph_position_t *pos = బఫరు->pos;
    ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      {
	pos->x_advance = info->ముసుగు;
	pos->x_స్థానభ్రంశం_2 = info->var1.i32;
	pos->y_స్థానభ్రంశం_2 = info->var2.i32;

	info++; pos++;
      }
    లేకపోతే
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      {
	pos->y_advance = info->ముసుగు;
	pos->x_స్థానభ్రంశం_2 = info->var1.i32;
	pos->y_స్థానభ్రంశం_2 = info->var2.i32;

	info++; pos++;
      }

    /* Fix up clusters so that we never return out-of-order indices;
     * if core text has reordered glyphs, we'll merge them to the
     * beginning of the reordered cluster.  CoreText is nice enough
     * to tell us whenever it has produced nonmonotonic results...
     * Note that we assume the input clusters were nonmonotonic to
     * begin with.
     *
     * This does *not* mean we'll form the same clusters as Uniscribe
     * or the native OT backend, only that the cluster indices will be
     * monotonic in the output buffer. */
    ఒకవేళ (లెక్క > 1 && (స్థితి_or & k_CTRun_స్థితి_Non_Monotonic) &&
	HB_బఫరు_CLUSTER_LEVEL_IS_MONOTONE (బఫరు->cluster_level))
    {
      hb_glyph_info_t *info = బఫరు->info;
      ఒకవేళ (HB_DIRECTION_IS_FORWARD (బఫరు->props.direction))
      {
	గుర్తులేని పూర్ణాంకం cluster = info[లెక్క - 1].cluster;
	కొరకు (గుర్తులేని పూర్ణాంకం i = లెక్క - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      లేకపోతే
      {
	గుర్తులేని పూర్ణాంకం cluster = info[0].cluster;
	కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
	{
	  cluster = hb_min (cluster, info[i].cluster);
	  info[i].cluster = cluster;
	}
      }
    }
  }

  /* TODO: Sometimes the above positioning code generates negative
   * advance values. Fix them up. Example, with NotoNastaliqUrdu
   * font and sequence ابهد. */

  బఫరు->clear_glyph_flags ();
  బఫరు->unsafe_గమ్యం_break ();

#నిర్వచనం_రద్దు FAIL

fail:
  ఒకవేళ (string_ref)
    CFRelease (string_ref);
  ఒకవేళ (పంక్తి)
    CFRelease (పంక్తి);

  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < range_రికార్డులు.నిడివి; i++)
    ఒకవేళ (range_రికార్డులు[i].font)
      CFRelease (range_రికార్డులు[i].font);

  తిరిగి ret;
}


#అయితే_ముగింపు
