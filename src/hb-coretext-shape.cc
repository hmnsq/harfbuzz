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

#ضمن "hb.hh"

#اگر_تعریف_شده HAVE_CORETEXT

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-coretext.hh"
#ضمن "hb-aat-layout.hh"

hb_coretext_face_داده_t *
hb_coretext_shaper_face_داده_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  إذا (unlikely (!cg_font))
  {
    DEBUG_پیام (CORETEXT, face, "CGFont creation failed..");
    أرجع اشاره_تهی;
  }

  أرجع (hb_coretext_face_داده_t *) cg_font;
}

فراغ
hb_coretext_shaper_face_داده_destroy (hb_coretext_face_داده_t *داده)
{
  إذا (داده)
    CFRelease ((CGFontRef) داده);
}


hb_coretext_font_داده_t *
hb_coretext_shaper_font_داده_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ثابت hb_coretext_face_داده_t *face_داده = face->داده.coretext;
  إذا (unlikely (!face_داده)) أرجع اشاره_تهی;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->داده.coretext;

  CGFloat font_اندازه = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_اندازه));

  إذا (unlikely (!ct_font))
  {
    DEBUG_پیام (CORETEXT, font, "CGFont creation failed..");
    أرجع اشاره_تهی;
  }

  إذا (font->رقم_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->رقم_coords,
				 &k_CFType_Dictionary_کلید_Call_Backs,
				 &k_CFType_Dictionary_مقدار_Call_Backs));

    بیعلامت شمار = font->رقم_coords;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_ot_var_axis_info_t info;
      بیعلامت صحيح c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      عائم v = hb_clamp (font->design_coords[i], info.min_مقدار, info.max_مقدار);

      hb_cf_ptr_t<CFNumberRef> برچسب_عدد (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.برچسب_2));
      hb_cf_ptr_t<CFNumberRef> مقدار_عدد (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      إذا (variations && برچسب_عدد && مقدار_عدد)
	CFDictionary_مجموعه_مقدار (variations, برچسب_عدد, مقدار_عدد);
    }

    ثابت فراغ *کلیدها[] = { kCTFontVariationAttribute };
    ثابت فراغ *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> صفات (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       کلیدها,
				       values,
				       1,
				       &k_CFType_Dictionary_کلید_Call_Backs,
				       &k_CFType_Dictionary_مقدار_Call_Backs) : اشاره_تهی);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (صفات ? CTFont_Descriptor_Create_With_صفات (صفات) : اشاره_تهی);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_صفات (ct_font, 0, اشاره_تهی, varDesc) : اشاره_تهی);
    إذا (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  أرجع (hb_coretext_font_داده_t *) ct_font.release ();
}

فراغ
hb_coretext_shaper_font_داده_destroy (hb_coretext_font_داده_t *داده)
{
  إذا (داده)
    CFRelease ((CTFontRef) داده);
}

/*
 * shaper
 */

بنية feature_رکورد_t {
  بیعلامت صحيح feature;
  بیعلامت صحيح setting;
};

بنية فعال_feature_t {
  feature_رکورد_t rec;
  بیعلامت صحيح ترتیب;

  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت فعال_feature_t *a = (ثابت فعال_feature_t *) pa;
    ثابت فعال_feature_t *b = (ثابت فعال_feature_t *) pb;
    أرجع a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  منطقي عملگر== (ثابت فعال_feature_t& f) ثابت {
    أرجع cmp (هذا, &f) == 0;
  }
};

بنية feature_event_t {
  بیعلامت صحيح شاخص_2;
  منطقي شروع;
  فعال_feature_t feature;

  HB_داخلی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
    ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
    أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 :
	   a->شروع < b->شروع ? -1 : a->شروع > b->شروع ? 1 :
	   فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية range_رکورد_t {
  CTFontRef font;
  بیعلامت صحيح شاخص_اولی; /* == start */
  بیعلامت صحيح شاخص_آخرین;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_میانگیر_t        *میانگیر,
		    ثابت hb_feature_t *features,
		    بیعلامت صحيح        رقم_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->داده.coretext;
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->داده.coretext;

  CGFloat ct_font_اندازه = CTFont_Get_اندازه (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_اندازه;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_اندازه;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  إذا (HB_میانگیر_CLUSTER_LEVEL_IS_GRAPHEMES (میانگیر->cluster_level))
  {
    hb_unicode_funcs_t *unicode = میانگیر->unicode;
    بیعلامت صحيح شمار = میانگیر->len;
    hb_glyph_info_t *info = میانگیر->info;
    لكل (بیعلامت صحيح i = 1; i < شمار; i++)
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	میانگیر->ادغام_grapheme_clusters (i - 1, i + 1);
  }

  hb_وکتور_t<range_رکورد_t> range_رکوردها;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  إذا (رقم_features)
  {
    /* Sort features by start/end events. */
    hb_وکتور_t<feature_event_t> feature_events;
    لكل (بیعلامت صحيح i = 0; i < رقم_features; i++)
    {
      فعال_feature_t feature;

#إذا MAC_OS_X_نسخه_MIN_REQUIRED < 101000
      ثابت hb_aat_feature_mapping_t * mapping = hb_aat_layout_پیدا_کن_feature_mapping (features[i].برچسب_2);
      إذا (!mapping)
	استمر;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_مقصد_Enable : mapping->selector_مقصد_Disable;
#وإلا
      feature.rec.feature = features[i].برچسب_2;
      feature.rec.setting = features[i].value;
#پایان_اگر
      feature.ترتیب = i;

      feature_event_t *event;

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
      فعال_feature_t feature;
      feature.rec.feature = HB_برچسب_NONE;
      feature.rec.setting = 0;
      feature.ترتیب = رقم_features + 1;

      feature_event_t *event = feature_events.push ();
      event->شاخص_2 = 0; /* This value does magic. */
      event->شروع = خطأ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_وکتور_t<فعال_feature_t> فعال_features;
    بیعلامت صحيح آخرین_شاخص = 0;
    لكل (بیعلامت صحيح i = 0; i < feature_events.درازی; i++)
    {
      feature_event_t *event = &feature_events[i];

      إذا (event->شاخص_2 != آخرین_شاخص)
      {
	/* Save a snapshot of active features and the range. */
	range_رکورد_t *range = range_رکوردها.push ();

	إذا (فعال_features.درازی)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  لكل (بیعلامت صحيح j = 0; j < فعال_features.درازی; j++)
	  {
#إذا MAC_OS_X_نسخه_MIN_REQUIRED < 101000
	    CFStringRef کلیدها[] = {
	      k_CTFont_Feature_Type_Identifier_کلید,
	      k_CTFont_Feature_Selector_Identifier_کلید
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#وإلا
	    حرف برچسب_2[5] = {HB_UNTAG (فعال_features[j].rec.feature)};
	    CFTypeRef کلیدها[] = {
	      k_CTFont_Open_Type_Feature_برچسب,
	      k_CTFont_Open_Type_Feature_مقدار
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, برچسب_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#پایان_اگر
	    static_assert ((ARRAY_درازی_CONST (کلیدها) == ARRAY_درازی_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ثابت فراغ **) کلیدها,
						       (ثابت فراغ **) values,
						       ARRAY_درازی (کلیدها),
						       &k_CFType_Dictionary_کلید_Call_Backs,
						       &k_CFType_Dictionary_مقدار_Call_Backs);
	    لكل (بیعلامت صحيح i = 0; i < ARRAY_درازی (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_مقدار (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef صفات = CFDictionaryCreate (kCFAllocatorDefault,
							   (ثابت فراغ **) &kCTFontFeatureSettingsAttribute,
							   (ثابت فراغ **) &features_array,
							   1,
							   &k_CFType_Dictionary_کلید_Call_Backs,
							   &k_CFType_Dictionary_مقدار_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_صفات (صفات);
	  CFRelease (صفات);

	  range->font = CTFont_Create_Copy_With_صفات (ct_font, 0.0, اشاره_تهی, font_desc);
	  CFRelease (font_desc);
	}
	وإلا
	{
	  range->font = اشاره_تهی;
	}

	range->شاخص_اولی = آخرین_شاخص;
	range->شاخص_آخرین  = event->شاخص_2 - 1;

	آخرین_شاخص = event->شاخص_2;
      }

      إذا (event->شروع)
      {
	فعال_features.push (event->feature);
      } وإلا {
	فعال_feature_t *feature = فعال_features.lsearch (event->feature);
	إذا (feature)
	  فعال_features.remove_ordered (feature - فعال_features.arrayZ);
      }
    }
  }

  بیعلامت صحيح scratch_اندازه;
  hb_میانگیر_t::scratch_میانگیر_t *scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);

#تعریف_کن ALLOCATE_ARRAY(Type, name, len, on_no_جا) \
  Type *name = (Type *) scratch; \
  افعل { \
    بیعلامت صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    إذا (unlikely (_consumed > scratch_اندازه)) \
    { \
      on_no_جا; \
      أكد (0); \
    } \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (UniChar, pchars, میانگیر->len * 2, ((فراغ)اشاره_تهی) /*nothing*/);
  بیعلامت صحيح chars_len = 0;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++) {
    hb_codepoint_t c = میانگیر->info[i].codepoint;
    إذا (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    وإلا {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (بیعلامت صحيح, log_clusters, chars_len, ((فراغ)اشاره_تهی) /*nothing*/);
  chars_len = 0;
  لكل (بیعلامت صحيح i = 0; i < میانگیر->len; i++)
  {
    hb_codepoint_t c = میانگیر->info[i].codepoint;
    بیعلامت صحيح cluster = میانگیر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_درون_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#تعریف_کن FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیام (CORETEXT, اشاره_تهی, __VA_ARGS__); \
    ret = خطأ; \
    اذهب fail; \
  } HB_STMT_انتها

  منطقي ret = درست;
  CFStringRef string_ref = اشاره_تهی;
  CTLineRef سطر = اشاره_تهی;

  إذا (خطأ)
  {
resize_and_retry:
    DEBUG_پیام (CORETEXT, میانگیر, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    أكد (string_ref);
    أكد (سطر);
    CFRelease (string_ref);
    CFRelease (سطر);
    string_ref = اشاره_تهی;
    سطر = اشاره_تهی;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    بیعلامت صحيح old_scratch_استفاده_شده;
    hb_میانگیر_t::scratch_میانگیر_t *old_scratch;
    old_scratch = میانگیر->get_scratch_میانگیر (&old_scratch_استفاده_شده);
    old_scratch_استفاده_شده = scratch - old_scratch;

    إذا (unlikely (!میانگیر->ensure (میانگیر->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = میانگیر->get_scratch_میانگیر (&scratch_اندازه);
    pchars = reinterpret_cast<UniChar *> (((حرف *) scratch + ((حرف *) pchars - (حرف *) old_scratch)));
    log_clusters = reinterpret_cast<بیعلامت صحيح *> (((حرف *) scratch + ((حرف *) log_clusters - (حرف *) old_scratch)));
    scratch += old_scratch_استفاده_شده;
    scratch_اندازه -= old_scratch_استفاده_شده;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (اشاره_تهی,
						     pchars, chars_len,
						     kCFAllocatorNull);
    إذا (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      إذا (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      إذا (HB_DIRECTION_IS_VERTICAL (میانگیر->props.direction))
      {
	CFAttributed_String_مجموعه_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      إذا (میانگیر->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#پایان_اگر
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_مقصد_string (میانگیر->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	إذا (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_مجموعه_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_مجموعه_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      إذا (رقم_features && range_رکوردها.درازی)
      {
	بیعلامت صحيح شروع = 0;
	range_رکورد_t *آخرین_range = &range_رکوردها[0];
	لكل (بیعلامت صحيح k = 0; k < chars_len; k++)
	{
	  range_رکورد_t *range = آخرین_range;
	  طالما (log_clusters[k] < range->شاخص_اولی)
	    range--;
	  طالما (log_clusters[k] > range->شاخص_آخرین)
	    range++;
	  إذا (range != آخرین_range)
	  {
	    إذا (آخرین_range->font)
	      CFAttributed_String_مجموعه_Attribute (attr_string, CFRangeMake (شروع, k - شروع),
					      kCTFontAttributeName, آخرین_range->font);

	    شروع = k;
	  }

	  آخرین_range = range;
	}
	إذا (شروع != chars_len && آخرین_range->font)
	  CFAttributed_String_مجموعه_Attribute (attr_string, CFRangeMake (شروع, chars_len - شروع),
					  kCTFontAttributeName, آخرین_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      إذا (رقم_features)
      {
	بیعلامت صحيح zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	لكل (بیعلامت صحيح i = 0; i < رقم_features; i++)
	{
	  ثابت hb_feature_t &feature = features[i];
	  إذا (feature.برچسب_2 == HB_برچسب('k','e','r','n') &&
	      feature.شروع < chars_len && feature.شروع < feature.انتها)
	  {
	    CFRange feature_range = CFRangeMake (feature.شروع,
						 hb_min (feature.انتها, chars_len) - feature.شروع);
	    إذا (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    وإلا
	      CFAttributed_String_مجموعه_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      صحيح level = HB_DIRECTION_IS_FORWARD (میانگیر->props.direction) ? 0 : 1;
      CFNumberRef level_عدد = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
      خارجي ثابت CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#پایان_اگر
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ثابت فراغ **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ثابت فراغ **) &level_عدد,
						    1,
						    &k_CFType_Dictionary_کلید_Call_Backs,
						    &k_CFType_Dictionary_مقدار_Call_Backs);
      CFRelease (level_عدد);
      إذا (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      إذا (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      سطر = CTTypesetter_Create_سطر (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      إذا (unlikely (!سطر))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (سطر);
    بیعلامت صحيح رقم_runs = CFArray_Get_شمار (glyph_runs);
    DEBUG_پیام (CORETEXT, اشاره_تهی, "Num runs: %u", رقم_runs);

    میانگیر->len = 0;
    uint32_t وضعیت_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    إذا (HB_DIRECTION_IS_BACKWARD (میانگیر->props.direction))
    {
      advances_so_far -= CTLine_Get_Trailing_Whitespace_عرض (سطر);
      إذا (HB_DIRECTION_IS_VERTICAL (میانگیر->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ثابت CFRange range_all = CFRangeMake (0, 0);

    لكل (بیعلامت صحيح i = 0; i < رقم_runs; i++)
    {
      CTRunRef پیاپی = static_cast<CTRunRef>(CFArray_Get_مقدار_At_شاخص (glyph_runs, i));
      CTRun_وضعیت پیاپی_وضعیت = CTRun_Get_وضعیت (پیاپی);
      وضعیت_or  |= پیاپی_وضعیت;
      DEBUG_پیام (CORETEXT, پیاپی, "CTRunStatus: %x", پیاپی_وضعیت);
      CGFloat پیاپی_advance = CTRunGetTypographicBounds (پیاپی, range_all, اشاره_تهی, اشاره_تهی, اشاره_تهی);
      إذا (HB_DIRECTION_IS_VERTICAL (میانگیر->props.direction))
	  پیاپی_advance = -پیاپی_advance;
      DEBUG_پیام (CORETEXT, پیاپی, "Run advance: %g", (مزدوج) پیاپی_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef صفات = CTRun_Get_صفات (پیاپی);
      CTFontRef پیاپی_ct_font = static_cast<CTFontRef>(CFDictionary_Get_مقدار (صفات, kCTFontAttributeName));
      إذا (!CFEqual (پیاپی_ct_font, ct_font))
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
	منطقي matched = خطأ;
	لكل (بیعلامت صحيح i = 0; i < range_رکوردها.درازی; i++)
	  إذا (range_رکوردها[i].font && CFEqual (پیاپی_ct_font, range_رکوردها[i].font))
	  {
	    matched = درست;
	    اكسر;
	  }
	إذا (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> پیاپی_cg_font (CTFontCopyGraphicsFont (پیاپی_ct_font, اشاره_تهی));
	  إذا (پیاپی_cg_font)
	    matched = CFEqual (پیاپی_cg_font, cg_font);
	}
	إذا (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_کلید));
	  hb_cf_ptr_t<CFStringRef> پیاپی_ps_name (CTFontCopyName (پیاپی_ct_font, k_CTFont_Post_Script_Name_کلید));
	  إذا (font_ps_name && پیاپی_ps_name)
	  {
	    إذا (CFStringCompare (پیاپی_ps_name, font_ps_name, 0) == k_CFCompare_Equal_مقصد)
	      matched = درست;
	  }
	}
	إذا (!matched)
	{
	  CFRange range = CTRunGetStringRange (پیاپی);
	  DEBUG_پیام (CORETEXT, پیاپی, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.درازی);
	  إذا (!میانگیر->ensure_inplace (میانگیر->len + range.درازی))
	    اذهب resize_and_retry;
	  hb_glyph_info_t *info = میانگیر->info + میانگیر->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = میانگیر->props.direction;
	  hb_position_t x_advance, y_advance, x_جابه_جایی_2, y_جابه_جایی_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_جابه_جایی_2, &y_جابه_جایی_2);
	  hb_position_t advance = hb_saturate_جمع_کن (x_advance, y_advance);
	  x_جابه_جایی_2 = hb_saturate_neg (x_جابه_جایی_2);
	  y_جابه_جایی_2 = hb_saturate_neg (y_جابه_جایی_2);

	  بیعلامت صحيح old_len = میانگیر->len;
	  لكل (CFIndex j = range.location; j < range.location + range.درازی; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_شاخص (string_ref, j);
	      إذا (hb_درون_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_شاخص (string_ref, j - 1);
		إذا (hb_درون_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  استمر;
	      }
	      إذا (میانگیر->unicode->is_default_ignorable (ch))
		استمر;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->ماسک = advance;
	      info->var1.i32 = x_جابه_جایی_2;
	      info->var2.i32 = y_جابه_جایی_2;

	      info++;
	      میانگیر->len++;
	  }
	  إذا (HB_DIRECTION_IS_BACKWARD (میانگیر->props.direction))
	    میانگیر->معکوس_range (old_len, میانگیر->len);
	  advances_so_far += پیاپی_advance;
	  استمر;
	}
      }

      بیعلامت صحيح رقم_glyphs = CTRun_Get_Glyph_شمار (پیاپی);
      إذا (رقم_glyphs == 0)
	استمر;

      إذا (!میانگیر->ensure_inplace (میانگیر->len + رقم_glyphs))
	اذهب resize_and_retry;

      hb_glyph_info_t *پیاپی_info = میانگیر->info + میانگیر->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#تعریف_کن USE_PTR درست

#تعریف_کن SCRATCH_SAVE() \
  بیعلامت صحيح scratch_اندازه_saved = scratch_اندازه; \
  hb_میانگیر_t::scratch_میانگیر_t *scratch_saved = scratch

#تعریف_کن SCRATCH_RESTORE() \
  scratch_اندازه = scratch_اندازه_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ثابت CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (پیاپی) : اشاره_تهی;
	إذا (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, رقم_glyphs, اذهب resize_and_retry);
	  CTRunGetGlyphs (پیاپی, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ثابت CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (پیاپی) : اشاره_تهی;
	إذا (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, شاخص_buf, رقم_glyphs, اذهب resize_and_retry);
	  CTRunGetStringIndices (پیاپی, range_all, شاخص_buf);
	  string_indices = شاخص_buf;
	}
	hb_glyph_info_t *info = پیاپی_info;
	لكل (بیعلامت صحيح j = 0; j < رقم_glyphs; j++)
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
	ثابت CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (پیاپی) : اشاره_تهی;
	إذا (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, رقم_glyphs, اذهب resize_and_retry);
	  CTRunGetPositions (پیاپی, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = پیاپی_info;
	إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
	{
	  hb_position_t x_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  لكل (بیعلامت صحيح j = 0; j < رقم_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < رقم_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    وإلا /* last glyph */
	      advance = پیاپی_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->ماسک = (hb_ماسک_t) hb_clamp_مقصد<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_جابه_جایی_2;
	    info->var2.i32 = hb_clamp_مقصد<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	وإلا
	{
	  hb_position_t y_جابه_جایی_2 = hb_clamp_مقصد<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  لكل (بیعلامت صحيح j = 0; j < رقم_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < رقم_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    وإلا /* last glyph */
	      advance = پیاپی_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->ماسک = (hb_ماسک_t) hb_clamp_مقصد<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_مقصد<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_جابه_جایی_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += پیاپی_advance;
      }
#لغو_تعریف SCRATCH_RESTORE
#لغو_تعریف SCRATCH_SAVE
#لغو_تعریف USE_PTR
#لغو_تعریف ALLOCATE_ARRAY

      میانگیر->len += رقم_glyphs;
    }

    میانگیر->clear_positions ();

    بیعلامت صحيح شمار = میانگیر->len;
    hb_glyph_info_t *info = میانگیر->info;
    hb_glyph_position_t *pos = میانگیر->pos;
    إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
      لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      {
	pos->x_advance = info->ماسک;
	pos->x_جابه_جایی_2 = info->var1.i32;
	pos->y_جابه_جایی_2 = info->var2.i32;

	info++; pos++;
      }
    وإلا
      لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      {
	pos->y_advance = info->ماسک;
	pos->x_جابه_جایی_2 = info->var1.i32;
	pos->y_جابه_جایی_2 = info->var2.i32;

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
    إذا (شمار > 1 && (وضعیت_or & k_CTRun_وضعیت_Non_Monotonic) &&
	HB_میانگیر_CLUSTER_LEVEL_IS_MONOTONE (میانگیر->cluster_level))
    {
      hb_glyph_info_t *info = میانگیر->info;
      إذا (HB_DIRECTION_IS_FORWARD (میانگیر->props.direction))
      {
	بیعلامت صحيح cluster = info[شمار - 1].cluster;
	لكل (بیعلامت صحيح i = شمار - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      وإلا
      {
	بیعلامت صحيح cluster = info[0].cluster;
	لكل (بیعلامت صحيح i = 1; i < شمار; i++)
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

  میانگیر->clear_glyph_flags ();
  میانگیر->unsafe_مقصد_break ();

#لغو_تعریف FAIL

fail:
  إذا (string_ref)
    CFRelease (string_ref);
  إذا (سطر)
    CFRelease (سطر);

  لكل (بیعلامت صحيح i = 0; i < range_رکوردها.درازی; i++)
    إذا (range_رکوردها[i].font)
      CFRelease (range_رکوردها[i].font);

  أرجع ret;
}


#پایان_اگر
