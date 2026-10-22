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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_CORETEXT

#شامل "hb-shaper-impl.hh"

#شامل "hb-coretext.hh"
#شامل "hb-aat-layout.hh"

hb_coretext_face_ڈیٹا_t *
hb_coretext_shaper_face_ڈیٹا_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  اگر (unlikely (!cg_font))
  {
    DEBUG_پیغام (CORETEXT, face, "CGFont creation failed..");
    واپس خالی_اشاریہ;
  }

  واپس (hb_coretext_face_ڈیٹا_t *) cg_font;
}

خالی
hb_coretext_shaper_face_ڈیٹا_destroy (hb_coretext_face_ڈیٹا_t *ڈیٹا)
{
  اگر (ڈیٹا)
    CFRelease ((CGFontRef) ڈیٹا);
}


hb_coretext_font_ڈیٹا_t *
hb_coretext_shaper_font_ڈیٹا_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ثابت hb_coretext_face_ڈیٹا_t *face_ڈیٹا = face->ڈیٹا.coretext;
  اگر (unlikely (!face_ڈیٹا)) واپس خالی_اشاریہ;
  CGFontRef cg_font = (CGFontRef) (ثابت خالی *) face->ڈیٹا.coretext;

  CGFloat font_سائز = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائز);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_سائز));

  اگر (unlikely (!ct_font))
  {
    DEBUG_پیغام (CORETEXT, font, "CGFont creation failed..");
    واپس خالی_اشاریہ;
  }

  اگر (font->عدد_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->عدد_coords,
				 &k_CFType_Dictionary_کلید_Call_Backs,
				 &k_CFType_Dictionary_قدر_Call_Backs));

    غیرعلامتی گنتی = font->عدد_coords;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_ot_var_axis_info_t info;
      غیرعلامتی صحیح c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      اعشاری v = hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر);

      hb_cf_ptr_t<CFNumberRef> لیبل_نمبر (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.لیبل_2));
      hb_cf_ptr_t<CFNumberRef> قدر_نمبر (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      اگر (variations && لیبل_نمبر && قدر_نمبر)
	CFDictionary_سیٹ_قدر (variations, لیبل_نمبر, قدر_نمبر);
    }

    ثابت خالی *کلیدیں[] = { kCTFontVariationAttribute };
    ثابت خالی *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> اوصاف (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       کلیدیں,
				       values,
				       1,
				       &k_CFType_Dictionary_کلید_Call_Backs,
				       &k_CFType_Dictionary_قدر_Call_Backs) : خالی_اشاریہ);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (اوصاف ? CTFont_Descriptor_Create_With_اوصاف (اوصاف) : خالی_اشاریہ);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_اوصاف (ct_font, 0, خالی_اشاریہ, varDesc) : خالی_اشاریہ);
    اگر (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  واپس (hb_coretext_font_ڈیٹا_t *) ct_font.release ();
}

خالی
hb_coretext_shaper_font_ڈیٹا_destroy (hb_coretext_font_ڈیٹا_t *ڈیٹا)
{
  اگر (ڈیٹا)
    CFRelease ((CTFontRef) ڈیٹا);
}

/*
 * shaper
 */

ساخت feature_ریکارڈ_t {
  غیرعلامتی صحیح feature;
  غیرعلامتی صحیح setting;
};

ساخت فعال_feature_t {
  feature_ریکارڈ_t rec;
  غیرعلامتی صحیح ترتیب;

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb) {
    ثابت فعال_feature_t *a = (ثابت فعال_feature_t *) pa;
    ثابت فعال_feature_t *b = (ثابت فعال_feature_t *) pb;
    واپس a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  منطقی عامل== (ثابت فعال_feature_t& f) ثابت {
    واپس cmp (یہ, &f) == 0;
  }
};

ساخت feature_event_t {
  غیرعلامتی صحیح اشاریہ_2;
  منطقی آغاز;
  فعال_feature_t feature;

  HB_اندرونی ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb) {
    ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
    ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
    واپس a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 :
	   a->آغاز < b->آغاز ? -1 : a->آغاز > b->آغاز ? 1 :
	   فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

ساخت range_ریکارڈ_t {
  CTFontRef font;
  غیرعلامتی صحیح اشاریہ_پہلا; /* == start */
  غیرعلامتی صحیح اشاریہ_آخری;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    غیرعلامتی صحیح        عدد_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ثابت خالی *) face->ڈیٹا.coretext;
  CTFontRef ct_font = (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext;

  CGFloat ct_font_سائز = CTFont_Get_سائز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائز;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  اگر (HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (بفر->cluster_level))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    غیرعلامتی صحیح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
      اگر (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	بفر->ملاؤ_grapheme_clusters (i - 1, i + 1);
  }

  hb_سمتیہ_t<range_ریکارڈ_t> range_اندراجات;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  اگر (عدد_features)
  {
    /* Sort features by start/end events. */
    hb_سمتیہ_t<feature_event_t> feature_events;
    برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
    {
      فعال_feature_t feature;

#اگر MAC_OS_X_نسخہ_MIN_REQUIRED < 101000
      ثابت hb_aat_feature_mapping_t * mapping = hb_aat_layout_ڈھونڈو_feature_mapping (features[i].لیبل_2);
      اگر (!mapping)
	جاری;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_مقصد_Enable : mapping->selector_مقصد_Disable;
#ورنہ
      feature.rec.feature = features[i].لیبل_2;
      feature.rec.setting = features[i].value;
#اگر_ختم
      feature.ترتیب = i;

      feature_event_t *event;

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
      فعال_feature_t feature;
      feature.rec.feature = HB_لیبل_NONE;
      feature.rec.setting = 0;
      feature.ترتیب = عدد_features + 1;

      feature_event_t *event = feature_events.push ();
      event->اشاریہ_2 = 0; /* This value does magic. */
      event->آغاز = جھوٹ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_سمتیہ_t<فعال_feature_t> فعال_features;
    غیرعلامتی صحیح آخری_اشاریہ = 0;
    برائے (غیرعلامتی صحیح i = 0; i < feature_events.لمبائی; i++)
    {
      feature_event_t *event = &feature_events[i];

      اگر (event->اشاریہ_2 != آخری_اشاریہ)
      {
	/* Save a snapshot of active features and the range. */
	range_ریکارڈ_t *range = range_اندراجات.push ();

	اگر (فعال_features.لمبائی)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  برائے (غیرعلامتی صحیح j = 0; j < فعال_features.لمبائی; j++)
	  {
#اگر MAC_OS_X_نسخہ_MIN_REQUIRED < 101000
	    CFStringRef کلیدیں[] = {
	      k_CTFont_Feature_Type_Identifier_کلید,
	      k_CTFont_Feature_Selector_Identifier_کلید
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#ورنہ
	    حرف لیبل_2[5] = {HB_UNTAG (فعال_features[j].rec.feature)};
	    CFTypeRef کلیدیں[] = {
	      k_CTFont_Open_Type_Feature_لیبل,
	      k_CTFont_Open_Type_Feature_قدر
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, لیبل_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#اگر_ختم
	    static_assert ((ARRAY_لمبائی_CONST (کلیدیں) == ARRAY_لمبائی_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ثابت خالی **) کلیدیں,
						       (ثابت خالی **) values,
						       ARRAY_لمبائی (کلیدیں),
						       &k_CFType_Dictionary_کلید_Call_Backs,
						       &k_CFType_Dictionary_قدر_Call_Backs);
	    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_قدر (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef اوصاف = CFDictionaryCreate (kCFAllocatorDefault,
							   (ثابت خالی **) &kCTFontFeatureSettingsAttribute,
							   (ثابت خالی **) &features_array,
							   1,
							   &k_CFType_Dictionary_کلید_Call_Backs,
							   &k_CFType_Dictionary_قدر_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_اوصاف (اوصاف);
	  CFRelease (اوصاف);

	  range->font = CTFont_Create_Copy_With_اوصاف (ct_font, 0.0, خالی_اشاریہ, font_desc);
	  CFRelease (font_desc);
	}
	ورنہ
	{
	  range->font = خالی_اشاریہ;
	}

	range->اشاریہ_پہلا = آخری_اشاریہ;
	range->اشاریہ_آخری  = event->اشاریہ_2 - 1;

	آخری_اشاریہ = event->اشاریہ_2;
      }

      اگر (event->آغاز)
      {
	فعال_features.push (event->feature);
      } ورنہ {
	فعال_feature_t *feature = فعال_features.lsearch (event->feature);
	اگر (feature)
	  فعال_features.remove_ordered (feature - فعال_features.arrayZ);
      }
    }
  }

  غیرعلامتی صحیح scratch_سائز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائز);

#تعریف_کرو ALLOCATE_ARRAY(Type, name, len, on_no_گنجائش) \
  Type *name = (Type *) scratch; \
  کرو { \
    غیرعلامتی صحیح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    اگر (unlikely (_consumed > scratch_سائز)) \
    { \
      on_no_گنجائش; \
      أكد (0); \
    } \
    scratch += _consumed; \
    scratch_سائز -= _consumed; \
  } جب (0)

  ALLOCATE_ARRAY (UniChar, pchars, بفر->len * 2, ((خالی)خالی_اشاریہ) /*nothing*/);
  غیرعلامتی صحیح chars_len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++) {
    hb_codepoint_t c = بفر->info[i].codepoint;
    اگر (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ورنہ اگر (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ورنہ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (غیرعلامتی صحیح, log_clusters, chars_len, ((خالی)خالی_اشاریہ) /*nothing*/);
  chars_len = 0;
  برائے (غیرعلامتی صحیح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    غیرعلامتی صحیح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    اگر (hb_اندر_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#تعریف_کرو FAIL(...) \
  HB_STMT_آغاز { \
    DEBUG_پیغام (CORETEXT, خالی_اشاریہ, __VA_ARGS__); \
    ret = جھوٹ; \
    جاؤ fail; \
  } HB_STMT_انتہا

  منطقی ret = سچ;
  CFStringRef string_ref = خالی_اشاریہ;
  CTLineRef سطر = خالی_اشاریہ;

  اگر (جھوٹ)
  {
resize_and_retry:
    DEBUG_پیغام (CORETEXT, بفر, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    أكد (string_ref);
    أكد (سطر);
    CFRelease (string_ref);
    CFRelease (سطر);
    string_ref = خالی_اشاریہ;
    سطر = خالی_اشاریہ;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    غیرعلامتی صحیح old_scratch_مستعمل;
    hb_بفر_t::scratch_بفر_t *old_scratch;
    old_scratch = بفر->get_scratch_بفر (&old_scratch_مستعمل);
    old_scratch_مستعمل = scratch - old_scratch;

    اگر (unlikely (!بفر->ensure (بفر->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = بفر->get_scratch_بفر (&scratch_سائز);
    pchars = reinterpret_cast<UniChar *> (((حرف *) scratch + ((حرف *) pchars - (حرف *) old_scratch)));
    log_clusters = reinterpret_cast<غیرعلامتی صحیح *> (((حرف *) scratch + ((حرف *) log_clusters - (حرف *) old_scratch)));
    scratch += old_scratch_مستعمل;
    scratch_سائز -= old_scratch_مستعمل;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (خالی_اشاریہ,
						     pchars, chars_len,
						     kCFAllocatorNull);
    اگر (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      اگر (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      اگر (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
      {
	CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      اگر (بفر->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#اگر !(defined(ہدف_OS_IPHONE) && ہدف_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#اگر_ختم
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_مقصد_string (بفر->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	اگر (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      اگر (عدد_features && range_اندراجات.لمبائی)
      {
	غیرعلامتی صحیح آغاز = 0;
	range_ریکارڈ_t *آخری_range = &range_اندراجات[0];
	برائے (غیرعلامتی صحیح k = 0; k < chars_len; k++)
	{
	  range_ریکارڈ_t *range = آخری_range;
	  جب (log_clusters[k] < range->اشاریہ_پہلا)
	    range--;
	  جب (log_clusters[k] > range->اشاریہ_آخری)
	    range++;
	  اگر (range != آخری_range)
	  {
	    اگر (آخری_range->font)
	      CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (آغاز, k - آغاز),
					      kCTFontAttributeName, آخری_range->font);

	    آغاز = k;
	  }

	  آخری_range = range;
	}
	اگر (آغاز != chars_len && آخری_range->font)
	  CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (آغاز, chars_len - آغاز),
					  kCTFontAttributeName, آخری_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      اگر (عدد_features)
      {
	غیرعلامتی صحیح zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	برائے (غیرعلامتی صحیح i = 0; i < عدد_features; i++)
	{
	  ثابت hb_feature_t &feature = features[i];
	  اگر (feature.لیبل_2 == HB_لیبل('k','e','r','n') &&
	      feature.آغاز < chars_len && feature.آغاز < feature.انتہا)
	  {
	    CFRange feature_range = CFRangeMake (feature.آغاز,
						 hb_min (feature.انتہا, chars_len) - feature.آغاز);
	    اگر (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    ورنہ
	      CFAttributed_String_سیٹ_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      صحیح level = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
      CFNumberRef level_نمبر = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#اگر !(defined(ہدف_OS_IPHONE) && ہدف_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1060
      خارجی ثابت CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#اگر_ختم
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ثابت خالی **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ثابت خالی **) &level_نمبر,
						    1,
						    &k_CFType_Dictionary_کلید_Call_Backs,
						    &k_CFType_Dictionary_قدر_Call_Backs);
      CFRelease (level_نمبر);
      اگر (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      اگر (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      سطر = CTTypesetter_Create_سطر (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      اگر (unlikely (!سطر))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (سطر);
    غیرعلامتی صحیح عدد_runs = CFArray_Get_گنتی (glyph_runs);
    DEBUG_پیغام (CORETEXT, خالی_اشاریہ, "Num runs: %u", عدد_runs);

    بفر->len = 0;
    uint32_t حالت_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    اگر (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
    {
      advances_so_far -= CTLine_Get_Trailing_Whitespace_چوڑائی (سطر);
      اگر (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ثابت CFRange range_all = CFRangeMake (0, 0);

    برائے (غیرعلامتی صحیح i = 0; i < عدد_runs; i++)
    {
      CTRunRef تسلسل = static_cast<CTRunRef>(CFArray_Get_قدر_At_اشاریہ (glyph_runs, i));
      CTRun_حالت تسلسل_حالت = CTRun_Get_حالت (تسلسل);
      حالت_or  |= تسلسل_حالت;
      DEBUG_پیغام (CORETEXT, تسلسل, "CTRunStatus: %x", تسلسل_حالت);
      CGFloat تسلسل_advance = CTRunGetTypographicBounds (تسلسل, range_all, خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ);
      اگر (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  تسلسل_advance = -تسلسل_advance;
      DEBUG_پیغام (CORETEXT, تسلسل, "Run advance: %g", (دوہرا) تسلسل_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef اوصاف = CTRun_Get_اوصاف (تسلسل);
      CTFontRef تسلسل_ct_font = static_cast<CTFontRef>(CFDictionary_Get_قدر (اوصاف, kCTFontAttributeName));
      اگر (!CFEqual (تسلسل_ct_font, ct_font))
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
	منطقی matched = جھوٹ;
	برائے (غیرعلامتی صحیح i = 0; i < range_اندراجات.لمبائی; i++)
	  اگر (range_اندراجات[i].font && CFEqual (تسلسل_ct_font, range_اندراجات[i].font))
	  {
	    matched = سچ;
	    توڑو;
	  }
	اگر (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> تسلسل_cg_font (CTFontCopyGraphicsFont (تسلسل_ct_font, خالی_اشاریہ));
	  اگر (تسلسل_cg_font)
	    matched = CFEqual (تسلسل_cg_font, cg_font);
	}
	اگر (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_کلید));
	  hb_cf_ptr_t<CFStringRef> تسلسل_ps_name (CTFontCopyName (تسلسل_ct_font, k_CTFont_Post_Script_Name_کلید));
	  اگر (font_ps_name && تسلسل_ps_name)
	  {
	    اگر (CFStringCompare (تسلسل_ps_name, font_ps_name, 0) == k_CFCompare_Equal_مقصد)
	      matched = سچ;
	  }
	}
	اگر (!matched)
	{
	  CFRange range = CTRunGetStringRange (تسلسل);
	  DEBUG_پیغام (CORETEXT, تسلسل, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.لمبائی);
	  اگر (!بفر->ensure_inplace (بفر->len + range.لمبائی))
	    جاؤ resize_and_retry;
	  hb_glyph_info_t *info = بفر->info + بفر->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = بفر->props.direction;
	  hb_position_t x_advance, y_advance, x_انحراف_2, y_انحراف_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_انحراف_2, &y_انحراف_2);
	  hb_position_t advance = hb_saturate_جوڑو (x_advance, y_advance);
	  x_انحراف_2 = hb_saturate_neg (x_انحراف_2);
	  y_انحراف_2 = hb_saturate_neg (y_انحراف_2);

	  غیرعلامتی صحیح old_len = بفر->len;
	  برائے (CFIndex j = range.location; j < range.location + range.لمبائی; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_اشاریہ (string_ref, j);
	      اگر (hb_اندر_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_اشاریہ (string_ref, j - 1);
		اگر (hb_اندر_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  جاری;
	      }
	      اگر (بفر->unicode->is_default_ignorable (ch))
		جاری;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->نقاب = advance;
	      info->var1.i32 = x_انحراف_2;
	      info->var2.i32 = y_انحراف_2;

	      info++;
	      بفر->len++;
	  }
	  اگر (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
	    بفر->الٹ_range (old_len, بفر->len);
	  advances_so_far += تسلسل_advance;
	  جاری;
	}
      }

      غیرعلامتی صحیح عدد_glyphs = CTRun_Get_Glyph_گنتی (تسلسل);
      اگر (عدد_glyphs == 0)
	جاری;

      اگر (!بفر->ensure_inplace (بفر->len + عدد_glyphs))
	جاؤ resize_and_retry;

      hb_glyph_info_t *تسلسل_info = بفر->info + بفر->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#تعریف_کرو USE_PTR سچ

#تعریف_کرو SCRATCH_SAVE() \
  غیرعلامتی صحیح scratch_سائز_saved = scratch_سائز; \
  hb_بفر_t::scratch_بفر_t *scratch_saved = scratch

#تعریف_کرو SCRATCH_RESTORE() \
  scratch_سائز = scratch_سائز_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ثابت CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (تسلسل) : خالی_اشاریہ;
	اگر (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, عدد_glyphs, جاؤ resize_and_retry);
	  CTRunGetGlyphs (تسلسل, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ثابت CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (تسلسل) : خالی_اشاریہ;
	اگر (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, اشاریہ_buf, عدد_glyphs, جاؤ resize_and_retry);
	  CTRunGetStringIndices (تسلسل, range_all, اشاریہ_buf);
	  string_indices = اشاریہ_buf;
	}
	hb_glyph_info_t *info = تسلسل_info;
	برائے (غیرعلامتی صحیح j = 0; j < عدد_glyphs; j++)
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
	ثابت CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (تسلسل) : خالی_اشاریہ;
	اگر (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, عدد_glyphs, جاؤ resize_and_retry);
	  CTRunGetPositions (تسلسل, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = تسلسل_info;
	اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	{
	  hb_position_t x_انحراف_2 = hb_clamp_مقصد<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  برائے (غیرعلامتی صحیح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    اگر (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    ورنہ /* last glyph */
	      advance = تسلسل_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_مقصد<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_انحراف_2;
	    info->var2.i32 = hb_clamp_مقصد<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	ورنہ
	{
	  hb_position_t y_انحراف_2 = hb_clamp_مقصد<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  برائے (غیرعلامتی صحیح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    اگر (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    ورنہ /* last glyph */
	      advance = تسلسل_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_مقصد<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_مقصد<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_انحراف_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += تسلسل_advance;
      }
#تعریف_منسوخ SCRATCH_RESTORE
#تعریف_منسوخ SCRATCH_SAVE
#تعریف_منسوخ USE_PTR
#تعریف_منسوخ ALLOCATE_ARRAY

      بفر->len += عدد_glyphs;
    }

    بفر->clear_positions ();

    غیرعلامتی صحیح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
      برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      {
	pos->x_advance = info->نقاب;
	pos->x_انحراف_2 = info->var1.i32;
	pos->y_انحراف_2 = info->var2.i32;

	info++; pos++;
      }
    ورنہ
      برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      {
	pos->y_advance = info->نقاب;
	pos->x_انحراف_2 = info->var1.i32;
	pos->y_انحراف_2 = info->var2.i32;

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
    اگر (گنتی > 1 && (حالت_or & k_CTRun_حالت_Non_Monotonic) &&
	HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
    {
      hb_glyph_info_t *info = بفر->info;
      اگر (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
      {
	غیرعلامتی صحیح cluster = info[گنتی - 1].cluster;
	برائے (غیرعلامتی صحیح i = گنتی - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      ورنہ
      {
	غیرعلامتی صحیح cluster = info[0].cluster;
	برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
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

  بفر->clear_glyph_flags ();
  بفر->unsafe_مقصد_break ();

#تعریف_منسوخ FAIL

fail:
  اگر (string_ref)
    CFRelease (string_ref);
  اگر (سطر)
    CFRelease (سطر);

  برائے (غیرعلامتی صحیح i = 0; i < range_اندراجات.لمبائی; i++)
    اگر (range_اندراجات[i].font)
      CFRelease (range_اندراجات[i].font);

  واپس ret;
}


#اگر_ختم
