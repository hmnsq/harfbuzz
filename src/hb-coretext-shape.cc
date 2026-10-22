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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_CORETEXT

#অন্তর্ভাব "hb-shaper-impl.hh"

#অন্তর্ভাব "hb-coretext.hh"
#অন্তর্ভাব "hb-aat-layout.hh"

hb_coretext_face_তথ্য_t *
hb_coretext_shaper_face_তথ্য_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  যদি (unlikely (!cg_font))
  {
    DEBUG_বার্তা (CORETEXT, face, "CGFont creation failed..");
    ফেরত nullptr;
  }

  ফেরত (hb_coretext_face_তথ্য_t *) cg_font;
}

শূন্য
hb_coretext_shaper_face_তথ্য_destroy (hb_coretext_face_তথ্য_t *তথ্য)
{
  যদি (তথ্য)
    CFRelease ((CGFontRef) তথ্য);
}


hb_coretext_font_তথ্য_t *
hb_coretext_shaper_font_তথ্য_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ধ্রুবক hb_coretext_face_তথ্য_t *face_তথ্য = face->তথ্য.coretext;
  যদি (unlikely (!face_তথ্য)) ফেরত nullptr;
  CGFontRef cg_font = (CGFontRef) (ধ্রুবক শূন্য *) face->তথ্য.coretext;

  CGFloat font_আকার = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_আকার);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_আকার));

  যদি (unlikely (!ct_font))
  {
    DEBUG_বার্তা (CORETEXT, font, "CGFont creation failed..");
    ফেরত nullptr;
  }

  যদি (font->অঙ্ক_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->অঙ্ক_coords,
				 &k_CFType_Dictionary_চাবি_Call_Backs,
				 &k_CFType_Dictionary_মান_Call_Backs));

    unsigned গুনতি = font->অঙ্ক_coords;
    জন্য (unsigned i = 0; i < গুনতি; i++)
    {
      hb_ot_var_axis_info_t info;
      unsigned পূর্ণসংখ্যা c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      দশমিক v = hb_clamp (font->design_coords[i], info.min_মান, info.max_মান);

      hb_cf_ptr_t<CFNumberRef> tag_সংখ্যা (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.tag));
      hb_cf_ptr_t<CFNumberRef> মান_সংখ্যা (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      যদি (variations && tag_সংখ্যা && মান_সংখ্যা)
	CFDictionary_Set_মান (variations, tag_সংখ্যা, মান_সংখ্যা);
    }

    ধ্রুবক শূন্য *চাবিগুলি[] = { kCTFontVariationAttribute };
    ধ্রুবক শূন্য *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> attributes (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       চাবিগুলি,
				       values,
				       1,
				       &k_CFType_Dictionary_চাবি_Call_Backs,
				       &k_CFType_Dictionary_মান_Call_Backs) : nullptr);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (attributes ? CTFontDescriptorCreateWithAttributes (attributes) : nullptr);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFontCreateCopyWithAttributes (ct_font, 0, nullptr, varDesc) : nullptr);
    যদি (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  ফেরত (hb_coretext_font_তথ্য_t *) ct_font.release ();
}

শূন্য
hb_coretext_shaper_font_তথ্য_destroy (hb_coretext_font_তথ্য_t *তথ্য)
{
  যদি (তথ্য)
    CFRelease ((CTFontRef) তথ্য);
}

/*
 * shaper
 */

কাঠামো feature_record_t {
  unsigned পূর্ণসংখ্যা feature;
  unsigned পূর্ণসংখ্যা setting;
};

কাঠামো active_feature_t {
  feature_record_t rec;
  unsigned পূর্ণসংখ্যা order;

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb) {
    ধ্রুবক active_feature_t *a = (ধ্রুবক active_feature_t *) pa;
    ধ্রুবক active_feature_t *b = (ধ্রুবক active_feature_t *) pb;
    ফেরত a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  যৌক্তিক operator== (ধ্রুবক active_feature_t& f) ধ্রুবক {
    ফেরত cmp (এই, &f) == 0;
  }
};

কাঠামো feature_event_t {
  unsigned পূর্ণসংখ্যা সূচক_2;
  যৌক্তিক start;
  active_feature_t feature;

  HB_INTERNAL স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য *pa, ধ্রুবক শূন্য *pb) {
    ধ্রুবক feature_event_t *a = (ধ্রুবক feature_event_t *) pa;
    ধ্রুবক feature_event_t *b = (ধ্রুবক feature_event_t *) pb;
    ফেরত a->সূচক_2 < b->সূচক_2 ? -1 : a->সূচক_2 > b->সূচক_2 ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   active_feature_t::cmp (&a->feature, &b->feature);
  }
};

কাঠামো range_record_t {
  CTFontRef font;
  unsigned পূর্ণসংখ্যা সূচক_first; /* == start */
  unsigned পূর্ণসংখ্যা সূচক_last;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_বাফার_t        *বাফার,
		    ধ্রুবক hb_feature_t *features,
		    unsigned পূর্ণসংখ্যা        অঙ্ক_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ধ্রুবক শূন্য *) face->তথ্য.coretext;
  CTFontRef ct_font = (CTFontRef) (ধ্রুবক শূন্য *) font->তথ্য.coretext;

  CGFloat ct_font_আকার = CTFont_Get_আকার (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_আকার;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_আকার;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  যদি (HB_বাফার_CLUSTER_LEVEL_IS_GRAPHEMES (বাফার->cluster_level))
  {
    hb_unicode_funcs_t *unicode = বাফার->unicode;
    unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
    hb_glyph_info_t *info = বাফার->info;
    জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
      যদি (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	বাফার->merge_grapheme_clusters (i - 1, i + 1);
  }

  hb_vector_t<range_record_t> range_records;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  যদি (অঙ্ক_features)
  {
    /* Sort features by start/end events. */
    hb_vector_t<feature_event_t> feature_events;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_features; i++)
    {
      active_feature_t feature;

#যদি MAC_OS_X_VERSION_MIN_REQUIRED < 101000
      ধ্রুবক hb_aat_feature_mapping_t * mapping = hb_aat_layout_find_feature_mapping (features[i].tag);
      যদি (!mapping)
	চালু;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selectorToEnable : mapping->selectorToDisable;
#অন্যথা
      feature.rec.feature = features[i].tag;
      feature.rec.setting = features[i].value;
#endif
      feature.order = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->সূচক_2 = features[i].start;
      event->start = সত্য;
      event->feature = feature;

      event = feature_events.push ();
      event->সূচক_2 = features[i].end;
      event->start = মিথ্যা;
      event->feature = feature;
    }
    feature_events.qsort ();
    /* Add a strategic final event. */
    {
      active_feature_t feature;
      feature.rec.feature = HB_TAG_NONE;
      feature.rec.setting = 0;
      feature.order = অঙ্ক_features + 1;

      feature_event_t *event = feature_events.push ();
      event->সূচক_2 = 0; /* This value does magic. */
      event->start = মিথ্যা;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_vector_t<active_feature_t> active_features;
    unsigned পূর্ণসংখ্যা last_সূচক = 0;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < feature_events.দৈর্ঘ্য; i++)
    {
      feature_event_t *event = &feature_events[i];

      যদি (event->সূচক_2 != last_সূচক)
      {
	/* Save a snapshot of active features and the range. */
	range_record_t *range = range_records.push ();

	যদি (active_features.দৈর্ঘ্য)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  জন্য (unsigned পূর্ণসংখ্যা j = 0; j < active_features.দৈর্ঘ্য; j++)
	  {
#যদি MAC_OS_X_VERSION_MIN_REQUIRED < 101000
	    CFStringRef চাবিগুলি[] = {
	      k_CTFont_Feature_Type_Identifier_চাবি,
	      k_CTFont_Feature_Selector_Identifier_চাবি
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#অন্যথা
	    অক্ষর tag[5] = {HB_UNTAG (active_features[j].rec.feature)};
	    CFTypeRef চাবিগুলি[] = {
	      kCTFontOpenTypeFeatureTag,
	      k_CTFont_Open_Type_Feature_মান
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, tag, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#endif
	    static_assert ((ARRAY_দৈর্ঘ্য_CONST (চাবিগুলি) == ARRAY_দৈর্ঘ্য_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ধ্রুবক শূন্য **) চাবিগুলি,
						       (ধ্রুবক শূন্য **) values,
						       ARRAY_দৈর্ঘ্য (চাবিগুলি),
						       &k_CFType_Dictionary_চাবি_Call_Backs,
						       &k_CFType_Dictionary_মান_Call_Backs);
	    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_মান (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef attributes = CFDictionaryCreate (kCFAllocatorDefault,
							   (ধ্রুবক শূন্য **) &kCTFontFeatureSettingsAttribute,
							   (ধ্রুবক শূন্য **) &features_array,
							   1,
							   &k_CFType_Dictionary_চাবি_Call_Backs,
							   &k_CFType_Dictionary_মান_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFontDescriptorCreateWithAttributes (attributes);
	  CFRelease (attributes);

	  range->font = CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, font_desc);
	  CFRelease (font_desc);
	}
	অন্যথা
	{
	  range->font = nullptr;
	}

	range->সূচক_first = last_সূচক;
	range->সূচক_last  = event->সূচক_2 - 1;

	last_সূচক = event->সূচক_2;
      }

      যদি (event->start)
      {
	active_features.push (event->feature);
      } অন্যথা {
	active_feature_t *feature = active_features.lsearch (event->feature);
	যদি (feature)
	  active_features.remove_ordered (feature - active_features.arrayZ);
      }
    }
  }

  unsigned পূর্ণসংখ্যা scratch_আকার;
  hb_বাফার_t::scratch_বাফার_t *scratch = বাফার->get_scratch_বাফার (&scratch_আকার);

#define ALLOCATE_ARRAY(Type, name, len, on_no_room) \
  Type *name = (Type *) scratch; \
  করো { \
    unsigned পূর্ণসংখ্যা _consumed = DIV_CEIL ((len) * আকার (Type), আকার (*scratch)); \
    যদি (unlikely (_consumed > scratch_আকার)) \
    { \
      on_no_room; \
      প্রতিজ্ঞা (0); \
    } \
    scratch += _consumed; \
    scratch_আকার -= _consumed; \
  } যখন (0)

  ALLOCATE_ARRAY (UniChar, pchars, বাফার->len * 2, ((শূন্য)nullptr) /*nothing*/);
  unsigned পূর্ণসংখ্যা chars_len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++) {
    hb_codepoint_t c = বাফার->info[i].codepoint;
    যদি (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    অন্যথা যদি (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    অন্যথা {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (unsigned পূর্ণসংখ্যা, log_clusters, chars_len, ((শূন্য)nullptr) /*nothing*/);
  chars_len = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < বাফার->len; i++)
  {
    hb_codepoint_t c = বাফার->info[i].codepoint;
    unsigned পূর্ণসংখ্যা cluster = বাফার->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    যদি (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_বার্তা (CORETEXT, nullptr, __VA_ARGS__); \
    ret = মিথ্যা; \
    যাও fail; \
  } HB_STMT_END

  যৌক্তিক ret = সত্য;
  CFStringRef string_ref = nullptr;
  CTLineRef সারি = nullptr;

  যদি (মিথ্যা)
  {
resize_and_retry:
    DEBUG_বার্তা (CORETEXT, বাফার, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    প্রতিজ্ঞা (string_ref);
    প্রতিজ্ঞা (সারি);
    CFRelease (string_ref);
    CFRelease (সারি);
    string_ref = nullptr;
    সারি = nullptr;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    unsigned পূর্ণসংখ্যা old_scratch_used;
    hb_বাফার_t::scratch_বাফার_t *old_scratch;
    old_scratch = বাফার->get_scratch_বাফার (&old_scratch_used);
    old_scratch_used = scratch - old_scratch;

    যদি (unlikely (!বাফার->ensure (বাফার->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = বাফার->get_scratch_বাফার (&scratch_আকার);
    pchars = reinterpret_cast<UniChar *> (((অক্ষর *) scratch + ((অক্ষর *) pchars - (অক্ষর *) old_scratch)));
    log_clusters = reinterpret_cast<unsigned পূর্ণসংখ্যা *> (((অক্ষর *) scratch + ((অক্ষর *) log_clusters - (অক্ষর *) old_scratch)));
    scratch += old_scratch_used;
    scratch_আকার -= old_scratch_used;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (nullptr,
						     pchars, chars_len,
						     kCFAllocatorNull);
    যদি (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      যদি (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      যদি (HB_DIRECTION_IS_VERTICAL (বাফার->props.direction))
      {
	CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      যদি (বাফার->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#যদি !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#endif
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_to_string (বাফার->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	যদি (unlikely (!lang))
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

      যদি (অঙ্ক_features && range_records.দৈর্ঘ্য)
      {
	unsigned পূর্ণসংখ্যা start = 0;
	range_record_t *last_range = &range_records[0];
	জন্য (unsigned পূর্ণসংখ্যা k = 0; k < chars_len; k++)
	{
	  range_record_t *range = last_range;
	  যখন (log_clusters[k] < range->সূচক_first)
	    range--;
	  যখন (log_clusters[k] > range->সূচক_last)
	    range++;
	  যদি (range != last_range)
	  {
	    যদি (last_range->font)
	      CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, k - start),
					      kCTFontAttributeName, last_range->font);

	    start = k;
	  }

	  last_range = range;
	}
	যদি (start != chars_len && last_range->font)
	  CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, chars_len - start),
					  kCTFontAttributeName, last_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      যদি (অঙ্ক_features)
      {
	unsigned পূর্ণসংখ্যা zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_features; i++)
	{
	  ধ্রুবক hb_feature_t &feature = features[i];
	  যদি (feature.tag == HB_TAG('k','e','r','n') &&
	      feature.start < chars_len && feature.start < feature.end)
	  {
	    CFRange feature_range = CFRangeMake (feature.start,
						 hb_min (feature.end, chars_len) - feature.start);
	    যদি (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    অন্যথা
	      CFAttributedStringSetAttribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      পূর্ণসংখ্যা level = HB_DIRECTION_IS_FORWARD (বাফার->props.direction) ? 0 : 1;
      CFNumberRef level_সংখ্যা = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#যদি !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
      বাহ্যিক ধ্রুবক CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#endif
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ধ্রুবক শূন্য **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ধ্রুবক শূন্য **) &level_সংখ্যা,
						    1,
						    &k_CFType_Dictionary_চাবি_Call_Backs,
						    &k_CFType_Dictionary_মান_Call_Backs);
      CFRelease (level_সংখ্যা);
      যদি (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      যদি (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      সারি = CTTypesetter_Create_সারি (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      যদি (unlikely (!সারি))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (সারি);
    unsigned পূর্ণসংখ্যা অঙ্ক_runs = CFArray_Get_গুনতি (glyph_runs);
    DEBUG_বার্তা (CORETEXT, nullptr, "Num runs: %u", অঙ্ক_runs);

    বাফার->len = 0;
    uint32_t status_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    যদি (HB_DIRECTION_IS_BACKWARD (বাফার->props.direction))
    {
      advances_so_far -= CTLineGetTrailingWhitespaceWidth (সারি);
      যদি (HB_DIRECTION_IS_VERTICAL (বাফার->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ধ্রুবক CFRange range_all = CFRangeMake (0, 0);

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_runs; i++)
    {
      CTRunRef run = static_cast<CTRunRef>(CFArray_Get_মান_At_সূচক (glyph_runs, i));
      CTRunStatus run_status = CTRunGetStatus (run);
      status_or  |= run_status;
      DEBUG_বার্তা (CORETEXT, run, "CTRunStatus: %x", run_status);
      CGFloat run_advance = CTRunGetTypographicBounds (run, range_all, nullptr, nullptr, nullptr);
      যদি (HB_DIRECTION_IS_VERTICAL (বাফার->props.direction))
	  run_advance = -run_advance;
      DEBUG_বার্তা (CORETEXT, run, "Run advance: %g", (দ্বিগুণ) run_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef attributes = CTRunGetAttributes (run);
      CTFontRef run_ct_font = static_cast<CTFontRef>(CFDictionary_Get_মান (attributes, kCTFontAttributeName));
      যদি (!CFEqual (run_ct_font, ct_font))
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
	যৌক্তিক matched = মিথ্যা;
	জন্য (unsigned পূর্ণসংখ্যা i = 0; i < range_records.দৈর্ঘ্য; i++)
	  যদি (range_records[i].font && CFEqual (run_ct_font, range_records[i].font))
	  {
	    matched = সত্য;
	    বিরতি;
	  }
	যদি (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> run_cg_font (CTFontCopyGraphicsFont (run_ct_font, nullptr));
	  যদি (run_cg_font)
	    matched = CFEqual (run_cg_font, cg_font);
	}
	যদি (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_চাবি));
	  hb_cf_ptr_t<CFStringRef> run_ps_name (CTFontCopyName (run_ct_font, k_CTFont_Post_Script_Name_চাবি));
	  যদি (font_ps_name && run_ps_name)
	  {
	    যদি (CFStringCompare (run_ps_name, font_ps_name, 0) == kCFCompareEqualTo)
	      matched = সত্য;
	  }
	}
	যদি (!matched)
	{
	  CFRange range = CTRunGetStringRange (run);
	  DEBUG_বার্তা (CORETEXT, run, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.দৈর্ঘ্য);
	  যদি (!বাফার->ensure_inplace (বাফার->len + range.দৈর্ঘ্য))
	    যাও resize_and_retry;
	  hb_glyph_info_t *info = বাফার->info + বাফার->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = বাফার->props.direction;
	  hb_position_t x_advance, y_advance, x_offset, y_offset;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_offset, &y_offset);
	  hb_position_t advance = hb_saturate_add (x_advance, y_advance);
	  x_offset = hb_saturate_neg (x_offset);
	  y_offset = hb_saturate_neg (y_offset);

	  unsigned পূর্ণসংখ্যা old_len = বাফার->len;
	  জন্য (CFIndex j = range.location; j < range.location + range.দৈর্ঘ্য; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_সূচক (string_ref, j);
	      যদি (hb_in_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_সূচক (string_ref, j - 1);
		যদি (hb_in_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  চালু;
	      }
	      যদি (বাফার->unicode->is_default_ignorable (ch))
		চালু;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->mask = advance;
	      info->var1.i32 = x_offset;
	      info->var2.i32 = y_offset;

	      info++;
	      বাফার->len++;
	  }
	  যদি (HB_DIRECTION_IS_BACKWARD (বাফার->props.direction))
	    বাফার->reverse_range (old_len, বাফার->len);
	  advances_so_far += run_advance;
	  চালু;
	}
      }

      unsigned পূর্ণসংখ্যা অঙ্ক_glyphs = CTRun_Get_Glyph_গুনতি (run);
      যদি (অঙ্ক_glyphs == 0)
	চালু;

      যদি (!বাফার->ensure_inplace (বাফার->len + অঙ্ক_glyphs))
	যাও resize_and_retry;

      hb_glyph_info_t *run_info = বাফার->info + বাফার->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#define USE_PTR সত্য

#define SCRATCH_SAVE() \
  unsigned পূর্ণসংখ্যা scratch_আকার_saved = scratch_আকার; \
  hb_বাফার_t::scratch_বাফার_t *scratch_saved = scratch

#define SCRATCH_RESTORE() \
  scratch_আকার = scratch_আকার_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ধ্রুবক CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (run) : nullptr;
	যদি (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, অঙ্ক_glyphs, যাও resize_and_retry);
	  CTRunGetGlyphs (run, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ধ্রুবক CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (run) : nullptr;
	যদি (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, সূচক_buf, অঙ্ক_glyphs, যাও resize_and_retry);
	  CTRunGetStringIndices (run, range_all, সূচক_buf);
	  string_indices = সূচক_buf;
	}
	hb_glyph_info_t *info = run_info;
	জন্য (unsigned পূর্ণসংখ্যা j = 0; j < অঙ্ক_glyphs; j++)
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
	ধ্রুবক CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (run) : nullptr;
	যদি (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, অঙ্ক_glyphs, যাও resize_and_retry);
	  CTRunGetPositions (run, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = run_info;
	যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction))
	{
	  hb_position_t x_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  জন্য (unsigned পূর্ণসংখ্যা j = 0; j < অঙ্ক_glyphs; j++)
	  {
	    CGFloat advance;
	    যদি (likely (j + 1 < অঙ্ক_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    অন্যথা /* last glyph */
	      advance = run_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_offset;
	    info->var2.i32 = hb_clamp_to<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	অন্যথা
	{
	  hb_position_t y_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  জন্য (unsigned পূর্ণসংখ্যা j = 0; j < অঙ্ক_glyphs; j++)
	  {
	    CGFloat advance;
	    যদি (likely (j + 1 < অঙ্ক_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    অন্যথা /* last glyph */
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
#undef SCRATCH_RESTORE
#undef SCRATCH_SAVE
#undef USE_PTR
#undef ALLOCATE_ARRAY

      বাফার->len += অঙ্ক_glyphs;
    }

    বাফার->clear_positions ();

    unsigned পূর্ণসংখ্যা গুনতি = বাফার->len;
    hb_glyph_info_t *info = বাফার->info;
    hb_glyph_position_t *pos = বাফার->pos;
    যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction))
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      {
	pos->x_advance = info->mask;
	pos->x_offset = info->var1.i32;
	pos->y_offset = info->var2.i32;

	info++; pos++;
      }
    অন্যথা
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
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
    যদি (গুনতি > 1 && (status_or & kCTRunStatusNonMonotonic) &&
	HB_বাফার_CLUSTER_LEVEL_IS_MONOTONE (বাফার->cluster_level))
    {
      hb_glyph_info_t *info = বাফার->info;
      যদি (HB_DIRECTION_IS_FORWARD (বাফার->props.direction))
      {
	unsigned পূর্ণসংখ্যা cluster = info[গুনতি - 1].cluster;
	জন্য (unsigned পূর্ণসংখ্যা i = গুনতি - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      অন্যথা
      {
	unsigned পূর্ণসংখ্যা cluster = info[0].cluster;
	জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
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

  বাফার->clear_glyph_flags ();
  বাফার->unsafe_to_break ();

#undef FAIL

fail:
  যদি (string_ref)
    CFRelease (string_ref);
  যদি (সারি)
    CFRelease (সারি);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < range_records.দৈর্ঘ্য; i++)
    যদি (range_records[i].font)
      CFRelease (range_records[i].font);

  ফেরত ret;
}


#endif
