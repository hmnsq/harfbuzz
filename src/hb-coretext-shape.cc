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

#समावेश "hb.hh"

#अगर_घोषित HAVE_CORETEXT

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-coretext.hh"
#समावेश "hb-aat-layout.hh"

hb_coretext_face_तथ्य_t *
hb_coretext_shaper_face_तथ्य_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  अगर (unlikely (!cg_font))
  {
    DEBUG_संदेश (CORETEXT, face, "CGFont creation failed..");
    वापस nullptr;
  }

  वापस (hb_coretext_face_तथ्य_t *) cg_font;
}

व्योम
hb_coretext_shaper_face_तथ्य_destroy (hb_coretext_face_तथ्य_t *तथ्य)
{
  अगर (तथ्य)
    CFRelease ((CGFontRef) तथ्य);
}


hb_coretext_font_तथ्य_t *
hb_coretext_shaper_font_तथ्य_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  स्थिर hb_coretext_face_तथ्य_t *face_तथ्य = face->तथ्य.coretext;
  अगर (unlikely (!face_तथ्य)) वापस nullptr;
  CGFontRef cg_font = (CGFontRef) (स्थिर व्योम *) face->तथ्य.coretext;

  CGFloat font_आकार = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_आकार);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_आकार));

  अगर (unlikely (!ct_font))
  {
    DEBUG_संदेश (CORETEXT, font, "CGFont creation failed..");
    वापस nullptr;
  }

  अगर (font->अंक_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->अंक_coords,
				 &k_CFType_Dictionary_कुंजी_Call_Backs,
				 &k_CFType_Dictionary_मूल्य_Call_Backs));

    अचिन्हित गिनती = font->अंक_coords;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_ot_var_axis_info_t info;
      अचिन्हित पूर्णांक c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      भग्न v = hb_clamp (font->design_coords[i], info.min_मूल्य, info.max_मूल्य);

      hb_cf_ptr_t<CFNumberRef> tag_संख्या (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.tag));
      hb_cf_ptr_t<CFNumberRef> मूल्य_संख्या (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      अगर (variations && tag_संख्या && मूल्य_संख्या)
	CFDictionary_Set_मूल्य (variations, tag_संख्या, मूल्य_संख्या);
    }

    स्थिर व्योम *कुंजियाँ[] = { kCTFontVariationAttribute };
    स्थिर व्योम *values[] = { variations.लो () };
    hb_cf_ptr_t<CFDictionaryRef> attributes (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       कुंजियाँ,
				       values,
				       1,
				       &k_CFType_Dictionary_कुंजी_Call_Backs,
				       &k_CFType_Dictionary_मूल्य_Call_Backs) : nullptr);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (attributes ? CTFontDescriptorCreateWithAttributes (attributes) : nullptr);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFontCreateCopyWithAttributes (ct_font, 0, nullptr, varDesc) : nullptr);
    अगर (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  वापस (hb_coretext_font_तथ्य_t *) ct_font.release ();
}

व्योम
hb_coretext_shaper_font_तथ्य_destroy (hb_coretext_font_तथ्य_t *तथ्य)
{
  अगर (तथ्य)
    CFRelease ((CTFontRef) तथ्य);
}

/*
 * shaper
 */

काष्ठा feature_record_t {
  अचिन्हित पूर्णांक feature;
  अचिन्हित पूर्णांक setting;
};

काष्ठा active_feature_t {
  feature_record_t rec;
  अचिन्हित पूर्णांक order;

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb) {
    स्थिर active_feature_t *a = (स्थिर active_feature_t *) pa;
    स्थिर active_feature_t *b = (स्थिर active_feature_t *) pb;
    वापस a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  bool चालक== (स्थिर active_feature_t& f) स्थिर {
    वापस cmp (यह, &f) == 0;
  }
};

काष्ठा feature_event_t {
  अचिन्हित पूर्णांक सूचकांक_2;
  bool start;
  active_feature_t feature;

  HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb) {
    स्थिर feature_event_t *a = (स्थिर feature_event_t *) pa;
    स्थिर feature_event_t *b = (स्थिर feature_event_t *) pb;
    वापस a->सूचकांक_2 < b->सूचकांक_2 ? -1 : a->सूचकांक_2 > b->सूचकांक_2 ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   active_feature_t::cmp (&a->feature, &b->feature);
  }
};

काष्ठा range_record_t {
  CTFontRef font;
  अचिन्हित पूर्णांक सूचकांक_first; /* == start */
  अचिन्हित पूर्णांक सूचकांक_last;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_बफर_t        *बफर,
		    स्थिर hb_feature_t *features,
		    अचिन्हित पूर्णांक        अंक_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (स्थिर व्योम *) face->तथ्य.coretext;
  CTFontRef ct_font = (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext;

  CGFloat ct_font_आकार = CTFont_Get_आकार (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_आकार;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_आकार;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  अगर (HB_बफर_CLUSTER_LEVEL_IS_GRAPHEMES (बफर->cluster_level))
  {
    hb_unicode_funcs_t *unicode = बफर->unicode;
    अचिन्हित पूर्णांक गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
      अगर (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	बफर->merge_grapheme_clusters (i - 1, i + 1);
  }

  hb_vector_t<range_record_t> range_records;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  अगर (अंक_features)
  {
    /* Sort features by start/end events. */
    hb_vector_t<feature_event_t> feature_events;
    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
    {
      active_feature_t feature;

#अगर MAC_OS_X_VERSION_MIN_REQUIRED < 101000
      स्थिर hb_aat_feature_mapping_t * mapping = hb_aat_layout_find_feature_mapping (features[i].tag);
      अगर (!mapping)
	जारी;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selectorToEnable : mapping->selectorToDisable;
#अन्यथा
      feature.rec.feature = features[i].tag;
      feature.rec.setting = features[i].value;
#पूर्ण_अगर
      feature.order = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->सूचकांक_2 = features[i].start;
      event->start = true;
      event->feature = feature;

      event = feature_events.push ();
      event->सूचकांक_2 = features[i].end;
      event->start = false;
      event->feature = feature;
    }
    feature_events.क्विक ();
    /* Add a strategic final event. */
    {
      active_feature_t feature;
      feature.rec.feature = HB_TAG_NONE;
      feature.rec.setting = 0;
      feature.order = अंक_features + 1;

      feature_event_t *event = feature_events.push ();
      event->सूचकांक_2 = 0; /* This value does magic. */
      event->start = false;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_vector_t<active_feature_t> active_features;
    अचिन्हित पूर्णांक last_सूचकांक = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < feature_events.लंबाई; i++)
    {
      feature_event_t *event = &feature_events[i];

      अगर (event->सूचकांक_2 != last_सूचकांक)
      {
	/* Save a snapshot of active features and the range. */
	range_record_t *range = range_records.push ();

	अगर (active_features.लंबाई)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  क्रम (अचिन्हित पूर्णांक j = 0; j < active_features.लंबाई; j++)
	  {
#अगर MAC_OS_X_VERSION_MIN_REQUIRED < 101000
	    CFStringRef कुंजियाँ[] = {
	      k_CTFont_Feature_Type_Identifier_कुंजी,
	      k_CTFont_Feature_Selector_Identifier_कुंजी
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#अन्यथा
	    अक्षर tag[5] = {HB_UNTAG (active_features[j].rec.feature)};
	    CFTypeRef कुंजियाँ[] = {
	      kCTFontOpenTypeFeatureTag,
	      k_CTFont_Open_Type_Feature_मूल्य
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, tag, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#पूर्ण_अगर
	    static_assert ((ARRAY_लंबाई_CONST (कुंजियाँ) == ARRAY_लंबाई_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (स्थिर व्योम **) कुंजियाँ,
						       (स्थिर व्योम **) values,
						       ARRAY_लंबाई (कुंजियाँ),
						       &k_CFType_Dictionary_कुंजी_Call_Backs,
						       &k_CFType_Dictionary_मूल्य_Call_Backs);
	    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_लंबाई (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_मूल्य (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef attributes = CFDictionaryCreate (kCFAllocatorDefault,
							   (स्थिर व्योम **) &kCTFontFeatureSettingsAttribute,
							   (स्थिर व्योम **) &features_array,
							   1,
							   &k_CFType_Dictionary_कुंजी_Call_Backs,
							   &k_CFType_Dictionary_मूल्य_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFontDescriptorCreateWithAttributes (attributes);
	  CFRelease (attributes);

	  range->font = CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, font_desc);
	  CFRelease (font_desc);
	}
	अन्यथा
	{
	  range->font = nullptr;
	}

	range->सूचकांक_first = last_सूचकांक;
	range->सूचकांक_last  = event->सूचकांक_2 - 1;

	last_सूचकांक = event->सूचकांक_2;
      }

      अगर (event->start)
      {
	active_features.push (event->feature);
      } अन्यथा {
	active_feature_t *feature = active_features.lsearch (event->feature);
	अगर (feature)
	  active_features.remove_ordered (feature - active_features.arrayZ);
      }
    }
  }

  अचिन्हित पूर्णांक scratch_आकार;
  hb_बफर_t::scratch_बफर_t *scratch = बफर->get_scratch_बफर (&scratch_आकार);

#घोषणा ALLOCATE_ARRAY(Type, name, len, on_no_room) \
  Type *name = (Type *) scratch; \
  करो { \
    अचिन्हित पूर्णांक _consumed = DIV_CEIL ((len) * माप (Type), माप (*scratch)); \
    अगर (unlikely (_consumed > scratch_आकार)) \
    { \
      on_no_room; \
      निश्चित (0); \
    } \
    scratch += _consumed; \
    scratch_आकार -= _consumed; \
  } जबतक (0)

  ALLOCATE_ARRAY (UniChar, pchars, बफर->len * 2, ((व्योम)nullptr) /*nothing*/);
  अचिन्हित पूर्णांक chars_len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++) {
    hb_codepoint_t c = बफर->info[i].codepoint;
    अगर (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    अन्यथा अगर (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    अन्यथा {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (अचिन्हित पूर्णांक, log_clusters, chars_len, ((व्योम)nullptr) /*nothing*/);
  chars_len = 0;
  क्रम (अचिन्हित पूर्णांक i = 0; i < बफर->len; i++)
  {
    hb_codepoint_t c = बफर->info[i].codepoint;
    अचिन्हित पूर्णांक cluster = बफर->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    अगर (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#घोषणा FAIL(...) \
  HB_STMT_START { \
    DEBUG_संदेश (CORETEXT, nullptr, __VA_ARGS__); \
    ret = false; \
    जाओ हार; \
  } HB_STMT_END

  bool ret = true;
  CFStringRef string_ref = nullptr;
  CTLineRef पंक्ति = nullptr;

  अगर (false)
  {
resize_and_retry:
    DEBUG_संदेश (CORETEXT, बफर, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    निश्चित (string_ref);
    निश्चित (पंक्ति);
    CFRelease (string_ref);
    CFRelease (पंक्ति);
    string_ref = nullptr;
    पंक्ति = nullptr;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    अचिन्हित पूर्णांक old_scratch_used;
    hb_बफर_t::scratch_बफर_t *old_scratch;
    old_scratch = बफर->get_scratch_बफर (&old_scratch_used);
    old_scratch_used = scratch - old_scratch;

    अगर (unlikely (!बफर->ensure (बफर->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = बफर->get_scratch_बफर (&scratch_आकार);
    pchars = reinterpret_cast<UniChar *> (((अक्षर *) scratch + ((अक्षर *) pchars - (अक्षर *) old_scratch)));
    log_clusters = reinterpret_cast<अचिन्हित पूर्णांक *> (((अक्षर *) scratch + ((अक्षर *) log_clusters - (अक्षर *) old_scratch)));
    scratch += old_scratch_used;
    scratch_आकार -= old_scratch_used;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (nullptr,
						     pchars, chars_len,
						     kCFAllocatorNull);
    अगर (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      अगर (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      अगर (HB_DIRECTION_IS_VERTICAL (बफर->props.direction))
      {
	CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      अगर (बफर->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#अगर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#पूर्ण_अगर
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_to_string (बफर->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	अगर (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      अगर (अंक_features && range_records.लंबाई)
      {
	अचिन्हित पूर्णांक start = 0;
	range_record_t *last_range = &range_records[0];
	क्रम (अचिन्हित पूर्णांक k = 0; k < chars_len; k++)
	{
	  range_record_t *range = last_range;
	  जबतक (log_clusters[k] < range->सूचकांक_first)
	    range--;
	  जबतक (log_clusters[k] > range->सूचकांक_last)
	    range++;
	  अगर (range != last_range)
	  {
	    अगर (last_range->font)
	      CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, k - start),
					      kCTFontAttributeName, last_range->font);

	    start = k;
	  }

	  last_range = range;
	}
	अगर (start != chars_len && last_range->font)
	  CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, chars_len - start),
					  kCTFontAttributeName, last_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      अगर (अंक_features)
      {
	अचिन्हित पूर्णांक zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_features; i++)
	{
	  स्थिर hb_feature_t &feature = features[i];
	  अगर (feature.tag == HB_TAG('k','e','r','n') &&
	      feature.start < chars_len && feature.start < feature.end)
	  {
	    CFRange feature_range = CFRangeMake (feature.start,
						 hb_min (feature.end, chars_len) - feature.start);
	    अगर (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    अन्यथा
	      CFAttributedStringSetAttribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      पूर्णांक level = HB_DIRECTION_IS_FORWARD (बफर->props.direction) ? 0 : 1;
      CFNumberRef level_संख्या = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#अगर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
      बाह्य स्थिर CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#पूर्ण_अगर
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (स्थिर व्योम **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (स्थिर व्योम **) &level_संख्या,
						    1,
						    &k_CFType_Dictionary_कुंजी_Call_Backs,
						    &k_CFType_Dictionary_मूल्य_Call_Backs);
      CFRelease (level_संख्या);
      अगर (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      अगर (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      पंक्ति = CTTypesetter_Create_पंक्ति (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      अगर (unlikely (!पंक्ति))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (पंक्ति);
    अचिन्हित पूर्णांक अंक_runs = CFArray_Get_गिनती (glyph_runs);
    DEBUG_संदेश (CORETEXT, nullptr, "Num runs: %u", अंक_runs);

    बफर->len = 0;
    uint32_t status_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    अगर (HB_DIRECTION_IS_BACKWARD (बफर->props.direction))
    {
      advances_so_far -= CTLineGetTrailingWhitespaceWidth (पंक्ति);
      अगर (HB_DIRECTION_IS_VERTICAL (बफर->props.direction))
	  advances_so_far = -advances_so_far;
    }

    स्थिर CFRange range_all = CFRangeMake (0, 0);

    क्रम (अचिन्हित पूर्णांक i = 0; i < अंक_runs; i++)
    {
      CTRunRef run = static_cast<CTRunRef>(CFArray_Get_मूल्य_At_सूचकांक (glyph_runs, i));
      CTRunStatus run_status = CTRunGetStatus (run);
      status_or  |= run_status;
      DEBUG_संदेश (CORETEXT, run, "CTRunStatus: %x", run_status);
      CGFloat run_advance = CTRunGetTypographicBounds (run, range_all, nullptr, nullptr, nullptr);
      अगर (HB_DIRECTION_IS_VERTICAL (बफर->props.direction))
	  run_advance = -run_advance;
      DEBUG_संदेश (CORETEXT, run, "Run advance: %g", (द्विगुन) run_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef attributes = CTRunGetAttributes (run);
      CTFontRef run_ct_font = static_cast<CTFontRef>(CFDictionary_Get_मूल्य (attributes, kCTFontAttributeName));
      अगर (!CFEqual (run_ct_font, ct_font))
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
	bool matched = false;
	क्रम (अचिन्हित पूर्णांक i = 0; i < range_records.लंबाई; i++)
	  अगर (range_records[i].font && CFEqual (run_ct_font, range_records[i].font))
	  {
	    matched = true;
	    अवरोध;
	  }
	अगर (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> run_cg_font (CTFontCopyGraphicsFont (run_ct_font, nullptr));
	  अगर (run_cg_font)
	    matched = CFEqual (run_cg_font, cg_font);
	}
	अगर (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_कुंजी));
	  hb_cf_ptr_t<CFStringRef> run_ps_name (CTFontCopyName (run_ct_font, k_CTFont_Post_Script_Name_कुंजी));
	  अगर (font_ps_name && run_ps_name)
	  {
	    अगर (CFStringCompare (run_ps_name, font_ps_name, 0) == kCFCompareEqualTo)
	      matched = true;
	  }
	}
	अगर (!matched)
	{
	  CFRange range = CTRunGetStringRange (run);
	  DEBUG_संदेश (CORETEXT, run, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.लंबाई);
	  अगर (!बफर->ensure_inplace (बफर->len + range.लंबाई))
	    जाओ resize_and_retry;
	  hb_glyph_info_t *info = बफर->info + बफर->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = बफर->props.direction;
	  hb_position_t x_advance, y_advance, x_offset, y_offset;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_offset, &y_offset);
	  hb_position_t advance = hb_saturate_add (x_advance, y_advance);
	  x_offset = hb_saturate_neg (x_offset);
	  y_offset = hb_saturate_neg (y_offset);

	  अचिन्हित पूर्णांक old_len = बफर->len;
	  क्रम (CFIndex j = range.location; j < range.location + range.लंबाई; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_सूचकांक (string_ref, j);
	      अगर (hb_in_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_सूचकांक (string_ref, j - 1);
		अगर (hb_in_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  जारी;
	      }
	      अगर (बफर->unicode->is_default_ignorable (ch))
		जारी;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->mask = advance;
	      info->var1.i32 = x_offset;
	      info->var2.i32 = y_offset;

	      info++;
	      बफर->len++;
	  }
	  अगर (HB_DIRECTION_IS_BACKWARD (बफर->props.direction))
	    बफर->reverse_range (old_len, बफर->len);
	  advances_so_far += run_advance;
	  जारी;
	}
      }

      अचिन्हित पूर्णांक अंक_glyphs = CTRun_Get_Glyph_गिनती (run);
      अगर (अंक_glyphs == 0)
	जारी;

      अगर (!बफर->ensure_inplace (बफर->len + अंक_glyphs))
	जाओ resize_and_retry;

      hb_glyph_info_t *run_info = बफर->info + बफर->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#घोषणा USE_PTR true

#घोषणा SCRATCH_SAVE() \
  अचिन्हित पूर्णांक scratch_आकार_saved = scratch_आकार; \
  hb_बफर_t::scratch_बफर_t *scratch_saved = scratch

#घोषणा SCRATCH_RESTORE() \
  scratch_आकार = scratch_आकार_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	स्थिर CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (run) : nullptr;
	अगर (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, अंक_glyphs, जाओ resize_and_retry);
	  CTRunGetGlyphs (run, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	स्थिर CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (run) : nullptr;
	अगर (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, सूचकांक_buf, अंक_glyphs, जाओ resize_and_retry);
	  CTRunGetStringIndices (run, range_all, सूचकांक_buf);
	  string_indices = सूचकांक_buf;
	}
	hb_glyph_info_t *info = run_info;
	क्रम (अचिन्हित पूर्णांक j = 0; j < अंक_glyphs; j++)
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
	स्थिर CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (run) : nullptr;
	अगर (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, अंक_glyphs, जाओ resize_and_retry);
	  CTRunGetPositions (run, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = run_info;
	अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
	{
	  hb_position_t x_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  क्रम (अचिन्हित पूर्णांक j = 0; j < अंक_glyphs; j++)
	  {
	    CGFloat advance;
	    अगर (likely (j + 1 < अंक_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    अन्यथा /* last glyph */
	      advance = run_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_offset;
	    info->var2.i32 = hb_clamp_to<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	अन्यथा
	{
	  hb_position_t y_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  क्रम (अचिन्हित पूर्णांक j = 0; j < अंक_glyphs; j++)
	  {
	    CGFloat advance;
	    अगर (likely (j + 1 < अंक_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    अन्यथा /* last glyph */
	      advance = run_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_to<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_offset;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += run_advance;
      }
#अघोषित SCRATCH_RESTORE
#अघोषित SCRATCH_SAVE
#अघोषित USE_PTR
#अघोषित ALLOCATE_ARRAY

      बफर->len += अंक_glyphs;
    }

    बफर->clear_positions ();

    अचिन्हित पूर्णांक गिनती = बफर->len;
    hb_glyph_info_t *info = बफर->info;
    hb_glyph_position_t *pos = बफर->pos;
    अगर (HB_DIRECTION_IS_HORIZONTAL (बफर->props.direction))
      क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      {
	pos->x_advance = info->mask;
	pos->x_offset = info->var1.i32;
	pos->y_offset = info->var2.i32;

	info++; pos++;
      }
    अन्यथा
      क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      {
	pos->y_advance = info->mask;
	pos->x_offset = info->var1.i32;
	pos->y_offset = info->var2.i32;

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
    अगर (गिनती > 1 && (status_or & kCTRunStatusNonMonotonic) &&
	HB_बफर_CLUSTER_LEVEL_IS_MONOTONE (बफर->cluster_level))
    {
      hb_glyph_info_t *info = बफर->info;
      अगर (HB_DIRECTION_IS_FORWARD (बफर->props.direction))
      {
	अचिन्हित पूर्णांक cluster = info[गिनती - 1].cluster;
	क्रम (अचिन्हित पूर्णांक i = गिनती - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      अन्यथा
      {
	अचिन्हित पूर्णांक cluster = info[0].cluster;
	क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
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

  बफर->clear_glyph_flags ();
  बफर->unsafe_to_break ();

#अघोषित FAIL

हार:
  अगर (string_ref)
    CFRelease (string_ref);
  अगर (पंक्ति)
    CFRelease (पंक्ति);

  क्रम (अचिन्हित पूर्णांक i = 0; i < range_records.लंबाई; i++)
    अगर (range_records[i].font)
      CFRelease (range_records[i].font);

  वापस ret;
}


#पूर्ण_अगर
