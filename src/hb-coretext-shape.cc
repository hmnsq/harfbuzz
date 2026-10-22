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

#جے_تعریف_شدہ HAVE_CORETEXT

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-coretext.hh"
#ضمن "hb-aat-layout.hh"

hb_coretext_face_انکڑے_t *
hb_coretext_shaper_face_انکڑے_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  إذا (unlikely (!cg_font))
  {
    DEBUG_پیغام (CORETEXT, face, "CGFont creation failed..");
    أرجع خالی_سوچک;
  }

  أرجع (hb_coretext_face_انکڑے_t *) cg_font;
}

فراغ
hb_coretext_shaper_face_انکڑے_destroy (hb_coretext_face_انکڑے_t *انکڑے)
{
  إذا (انکڑے)
    CFRelease ((CGFontRef) انکڑے);
}


hb_coretext_font_انکڑے_t *
hb_coretext_shaper_font_انکڑے_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ثابت hb_coretext_face_انکڑے_t *face_انکڑے = face->انکڑے.coretext;
  إذا (unlikely (!face_انکڑے)) أرجع خالی_سوچک;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->انکڑے.coretext;

  CGFloat font_ناپ = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_ناپ);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_ناپ));

  إذا (unlikely (!ct_font))
  {
    DEBUG_پیغام (CORETEXT, font, "CGFont creation failed..");
    أرجع خالی_سوچک;
  }

  إذا (font->عدد_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->عدد_coords,
				 &k_CFType_Dictionary_کلید_Call_Backs,
				 &k_CFType_Dictionary_قدر_Call_Backs));

    بنانشانی گنتی = font->عدد_coords;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_ot_var_axis_info_t info;
      بنانشانی صحيح c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      عائم v = hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر);

      hb_cf_ptr_t<CFNumberRef> ٹیگ_نمبر (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.ٹیگ_2));
      hb_cf_ptr_t<CFNumberRef> قدر_نمبر (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      إذا (variations && ٹیگ_نمبر && قدر_نمبر)
	CFDictionary_سیٹ_قدر (variations, ٹیگ_نمبر, قدر_نمبر);
    }

    ثابت فراغ *کلیدیں[] = { kCTFontVariationAttribute };
    ثابت فراغ *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> گن (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       کلیدیں,
				       values,
				       1,
				       &k_CFType_Dictionary_کلید_Call_Backs,
				       &k_CFType_Dictionary_قدر_Call_Backs) : خالی_سوچک);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (گن ? CTFont_Descriptor_Create_With_گن (گن) : خالی_سوچک);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_گن (ct_font, 0, خالی_سوچک, varDesc) : خالی_سوچک);
    إذا (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  أرجع (hb_coretext_font_انکڑے_t *) ct_font.release ();
}

فراغ
hb_coretext_shaper_font_انکڑے_destroy (hb_coretext_font_انکڑے_t *انکڑے)
{
  إذا (انکڑے)
    CFRelease ((CTFontRef) انکڑے);
}

/*
 * shaper
 */

بنية feature_ریکارڈ_t {
  بنانشانی صحيح feature;
  بنانشانی صحيح setting;
};

بنية سرگرم_feature_t {
  feature_ریکارڈ_t rec;
  بنانشانی صحيح ترتیب;

  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت سرگرم_feature_t *a = (ثابت سرگرم_feature_t *) pa;
    ثابت سرگرم_feature_t *b = (ثابت سرگرم_feature_t *) pb;
    أرجع a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  منطقي عامل== (ثابت سرگرم_feature_t& f) ثابت {
    أرجع cmp (هذا, &f) == 0;
  }
};

بنية feature_event_t {
  بنانشانی صحيح اشاریہ_2;
  منطقي شروع;
  سرگرم_feature_t feature;

  HB_اندرونی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
    ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
    أرجع a->اشاریہ_2 < b->اشاریہ_2 ? -1 : a->اشاریہ_2 > b->اشاریہ_2 ? 1 :
	   a->شروع < b->شروع ? -1 : a->شروع > b->شروع ? 1 :
	   سرگرم_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية range_ریکارڈ_t {
  CTFontRef font;
  بنانشانی صحيح اشاریہ_پہلا; /* == start */
  بنانشانی صحيح اشاریہ_آخری;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    بنانشانی صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->انکڑے.coretext;
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->انکڑے.coretext;

  CGFloat ct_font_ناپ = CTFont_Get_ناپ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ناپ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ناپ;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  إذا (HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (بفر->cluster_level))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    بنانشانی صحيح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	بفر->ملاؤ_grapheme_clusters (i - 1, i + 1);
  }

  hb_سمتیہ_t<range_ریکارڈ_t> range_ریکارڈاں;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  إذا (عدد_features)
  {
    /* Sort features by start/end events. */
    hb_سمتیہ_t<feature_event_t> feature_events;
    لكل (بنانشانی صحيح i = 0; i < عدد_features; i++)
    {
      سرگرم_feature_t feature;

#إذا MAC_OS_X_نسخہ_MIN_REQUIRED < 101000
      ثابت hb_aat_feature_mapping_t * mapping = hb_aat_layout_لبھو_feature_mapping (features[i].ٹیگ_2);
      إذا (!mapping)
	استمر;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_منزل_Enable : mapping->selector_منزل_Disable;
#وإلا
      feature.rec.feature = features[i].ٹیگ_2;
      feature.rec.setting = features[i].value;
#جے_انت
      feature.ترتیب = i;

      feature_event_t *event;

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
      سرگرم_feature_t feature;
      feature.rec.feature = HB_ٹیگ_NONE;
      feature.rec.setting = 0;
      feature.ترتیب = عدد_features + 1;

      feature_event_t *event = feature_events.push ();
      event->اشاریہ_2 = 0; /* This value does magic. */
      event->شروع = خطأ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_سمتیہ_t<سرگرم_feature_t> سرگرم_features;
    بنانشانی صحيح آخری_اشاریہ = 0;
    لكل (بنانشانی صحيح i = 0; i < feature_events.لمائی; i++)
    {
      feature_event_t *event = &feature_events[i];

      إذا (event->اشاریہ_2 != آخری_اشاریہ)
      {
	/* Save a snapshot of active features and the range. */
	range_ریکارڈ_t *range = range_ریکارڈاں.push ();

	إذا (سرگرم_features.لمائی)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  لكل (بنانشانی صحيح j = 0; j < سرگرم_features.لمائی; j++)
	  {
#إذا MAC_OS_X_نسخہ_MIN_REQUIRED < 101000
	    CFStringRef کلیدیں[] = {
	      k_CTFont_Feature_Type_Identifier_کلید,
	      k_CTFont_Feature_Selector_Identifier_کلید
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.setting)
	    };
#وإلا
	    حرف ٹیگ_2[5] = {HB_UNTAG (سرگرم_features[j].rec.feature)};
	    CFTypeRef کلیدیں[] = {
	      k_CTFont_Open_Type_Feature_ٹیگ,
	      k_CTFont_Open_Type_Feature_قدر
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, ٹیگ_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.setting)
	    };
#جے_انت
	    static_assert ((ARRAY_لمائی_CONST (کلیدیں) == ARRAY_لمائی_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ثابت فراغ **) کلیدیں,
						       (ثابت فراغ **) values,
						       ARRAY_لمائی (کلیدیں),
						       &k_CFType_Dictionary_کلید_Call_Backs,
						       &k_CFType_Dictionary_قدر_Call_Backs);
	    لكل (بنانشانی صحيح i = 0; i < ARRAY_لمائی (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_قدر (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef گن = CFDictionaryCreate (kCFAllocatorDefault,
							   (ثابت فراغ **) &kCTFontFeatureSettingsAttribute,
							   (ثابت فراغ **) &features_array,
							   1,
							   &k_CFType_Dictionary_کلید_Call_Backs,
							   &k_CFType_Dictionary_قدر_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_گن (گن);
	  CFRelease (گن);

	  range->font = CTFont_Create_Copy_With_گن (ct_font, 0.0, خالی_سوچک, font_desc);
	  CFRelease (font_desc);
	}
	وإلا
	{
	  range->font = خالی_سوچک;
	}

	range->اشاریہ_پہلا = آخری_اشاریہ;
	range->اشاریہ_آخری  = event->اشاریہ_2 - 1;

	آخری_اشاریہ = event->اشاریہ_2;
      }

      إذا (event->شروع)
      {
	سرگرم_features.push (event->feature);
      } وإلا {
	سرگرم_feature_t *feature = سرگرم_features.lsearch (event->feature);
	إذا (feature)
	  سرگرم_features.remove_ordered (feature - سرگرم_features.arrayZ);
      }
    }
  }

  بنانشانی صحيح scratch_ناپ;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_ناپ);

#تعریف_کرو ALLOCATE_ARRAY(Type, name, len, on_no_گنجائش) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشانی صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    إذا (unlikely (_consumed > scratch_ناپ)) \
    { \
      on_no_گنجائش; \
      أكد (0); \
    } \
    scratch += _consumed; \
    scratch_ناپ -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (UniChar, pchars, بفر->len * 2, ((فراغ)خالی_سوچک) /*nothing*/);
  بنانشانی صحيح chars_len = 0;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++) {
    hb_codepoint_t c = بفر->info[i].codepoint;
    إذا (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    وإلا إذا (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    وإلا {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (بنانشانی صحيح, log_clusters, chars_len, ((فراغ)خالی_سوچک) /*nothing*/);
  chars_len = 0;
  لكل (بنانشانی صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بنانشانی صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_وچ_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#تعریف_کرو FAIL(...) \
  HB_STMT_شروع { \
    DEBUG_پیغام (CORETEXT, خالی_سوچک, __VA_ARGS__); \
    ret = خطأ; \
    اذهب fail; \
  } HB_STMT_سرا

  منطقي ret = سچ;
  CFStringRef string_ref = خالی_سوچک;
  CTLineRef سطر = خالی_سوچک;

  إذا (خطأ)
  {
resize_and_retry:
    DEBUG_پیغام (CORETEXT, بفر, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    أكد (string_ref);
    أكد (سطر);
    CFRelease (string_ref);
    CFRelease (سطر);
    string_ref = خالی_سوچک;
    سطر = خالی_سوچک;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    بنانشانی صحيح old_scratch_ورتیا;
    hb_بفر_t::scratch_بفر_t *old_scratch;
    old_scratch = بفر->get_scratch_بفر (&old_scratch_ورتیا);
    old_scratch_ورتیا = scratch - old_scratch;

    إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = بفر->get_scratch_بفر (&scratch_ناپ);
    pchars = reinterpret_cast<UniChar *> (((حرف *) scratch + ((حرف *) pchars - (حرف *) old_scratch)));
    log_clusters = reinterpret_cast<بنانشانی صحيح *> (((حرف *) scratch + ((حرف *) log_clusters - (حرف *) old_scratch)));
    scratch += old_scratch_ورتیا;
    scratch_ناپ -= old_scratch_ورتیا;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (خالی_سوچک,
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
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
      {
	CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      إذا (بفر->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#إذا !(defined(نشانہ_OS_IPHONE) && نشانہ_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#جے_انت
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_منزل_string (بفر->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	إذا (unlikely (!lang))
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

      إذا (عدد_features && range_ریکارڈاں.لمائی)
      {
	بنانشانی صحيح شروع = 0;
	range_ریکارڈ_t *آخری_range = &range_ریکارڈاں[0];
	لكل (بنانشانی صحيح k = 0; k < chars_len; k++)
	{
	  range_ریکارڈ_t *range = آخری_range;
	  طالما (log_clusters[k] < range->اشاریہ_پہلا)
	    range--;
	  طالما (log_clusters[k] > range->اشاریہ_آخری)
	    range++;
	  إذا (range != آخری_range)
	  {
	    إذا (آخری_range->font)
	      CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (شروع, k - شروع),
					      kCTFontAttributeName, آخری_range->font);

	    شروع = k;
	  }

	  آخری_range = range;
	}
	إذا (شروع != chars_len && آخری_range->font)
	  CFAttributed_String_سیٹ_Attribute (attr_string, CFRangeMake (شروع, chars_len - شروع),
					  kCTFontAttributeName, آخری_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      إذا (عدد_features)
      {
	بنانشانی صحيح zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	لكل (بنانشانی صحيح i = 0; i < عدد_features; i++)
	{
	  ثابت hb_feature_t &feature = features[i];
	  إذا (feature.ٹیگ_2 == HB_ٹیگ('k','e','r','n') &&
	      feature.شروع < chars_len && feature.شروع < feature.سرا)
	  {
	    CFRange feature_range = CFRangeMake (feature.شروع,
						 hb_min (feature.سرا, chars_len) - feature.شروع);
	    إذا (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    وإلا
	      CFAttributed_String_سیٹ_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      صحيح level = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
      CFNumberRef level_نمبر = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#إذا !(defined(نشانہ_OS_IPHONE) && نشانہ_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1060
      خارجي ثابت CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#جے_انت
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ثابت فراغ **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ثابت فراغ **) &level_نمبر,
						    1,
						    &k_CFType_Dictionary_کلید_Call_Backs,
						    &k_CFType_Dictionary_قدر_Call_Backs);
      CFRelease (level_نمبر);
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
    بنانشانی صحيح عدد_runs = CFArray_Get_گنتی (glyph_runs);
    DEBUG_پیغام (CORETEXT, خالی_سوچک, "Num runs: %u", عدد_runs);

    بفر->len = 0;
    uint32_t حالت_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    إذا (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
    {
      advances_so_far -= CTLine_Get_Trailing_Whitespace_چوڑائی (سطر);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ثابت CFRange range_all = CFRangeMake (0, 0);

    لكل (بنانشانی صحيح i = 0; i < عدد_runs; i++)
    {
      CTRunRef لگاتار = static_cast<CTRunRef>(CFArray_Get_قدر_At_اشاریہ (glyph_runs, i));
      CTRun_حالت لگاتار_حالت = CTRun_Get_حالت (لگاتار);
      حالت_or  |= لگاتار_حالت;
      DEBUG_پیغام (CORETEXT, لگاتار, "CTRunStatus: %x", لگاتار_حالت);
      CGFloat لگاتار_advance = CTRunGetTypographicBounds (لگاتار, range_all, خالی_سوچک, خالی_سوچک, خالی_سوچک);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  لگاتار_advance = -لگاتار_advance;
      DEBUG_پیغام (CORETEXT, لگاتار, "Run advance: %g", (مزدوج) لگاتار_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef گن = CTRun_Get_گن (لگاتار);
      CTFontRef لگاتار_ct_font = static_cast<CTFontRef>(CFDictionary_Get_قدر (گن, kCTFontAttributeName));
      إذا (!CFEqual (لگاتار_ct_font, ct_font))
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
	لكل (بنانشانی صحيح i = 0; i < range_ریکارڈاں.لمائی; i++)
	  إذا (range_ریکارڈاں[i].font && CFEqual (لگاتار_ct_font, range_ریکارڈاں[i].font))
	  {
	    matched = سچ;
	    اكسر;
	  }
	إذا (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> لگاتار_cg_font (CTFontCopyGraphicsFont (لگاتار_ct_font, خالی_سوچک));
	  إذا (لگاتار_cg_font)
	    matched = CFEqual (لگاتار_cg_font, cg_font);
	}
	إذا (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_کلید));
	  hb_cf_ptr_t<CFStringRef> لگاتار_ps_name (CTFontCopyName (لگاتار_ct_font, k_CTFont_Post_Script_Name_کلید));
	  إذا (font_ps_name && لگاتار_ps_name)
	  {
	    إذا (CFStringCompare (لگاتار_ps_name, font_ps_name, 0) == k_CFCompare_Equal_منزل)
	      matched = سچ;
	  }
	}
	إذا (!matched)
	{
	  CFRange range = CTRunGetStringRange (لگاتار);
	  DEBUG_پیغام (CORETEXT, لگاتار, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.لمائی);
	  إذا (!بفر->ensure_inplace (بفر->len + range.لمائی))
	    اذهب resize_and_retry;
	  hb_glyph_info_t *info = بفر->info + بفر->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = بفر->props.direction;
	  hb_position_t x_advance, y_advance, x_وتھ_2, y_وتھ_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_وتھ_2, &y_وتھ_2);
	  hb_position_t advance = hb_saturate_جوڑو (x_advance, y_advance);
	  x_وتھ_2 = hb_saturate_neg (x_وتھ_2);
	  y_وتھ_2 = hb_saturate_neg (y_وتھ_2);

	  بنانشانی صحيح old_len = بفر->len;
	  لكل (CFIndex j = range.location; j < range.location + range.لمائی; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_اشاریہ (string_ref, j);
	      إذا (hb_وچ_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_اشاریہ (string_ref, j - 1);
		إذا (hb_وچ_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  استمر;
	      }
	      إذا (بفر->unicode->is_default_ignorable (ch))
		استمر;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->نقاب = advance;
	      info->var1.i32 = x_وتھ_2;
	      info->var2.i32 = y_وتھ_2;

	      info++;
	      بفر->len++;
	  }
	  إذا (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
	    بفر->الٹ_range (old_len, بفر->len);
	  advances_so_far += لگاتار_advance;
	  استمر;
	}
      }

      بنانشانی صحيح عدد_glyphs = CTRun_Get_Glyph_گنتی (لگاتار);
      إذا (عدد_glyphs == 0)
	استمر;

      إذا (!بفر->ensure_inplace (بفر->len + عدد_glyphs))
	اذهب resize_and_retry;

      hb_glyph_info_t *لگاتار_info = بفر->info + بفر->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#تعریف_کرو USE_PTR سچ

#تعریف_کرو SCRATCH_SAVE() \
  بنانشانی صحيح scratch_ناپ_saved = scratch_ناپ; \
  hb_بفر_t::scratch_بفر_t *scratch_saved = scratch

#تعریف_کرو SCRATCH_RESTORE() \
  scratch_ناپ = scratch_ناپ_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ثابت CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (لگاتار) : خالی_سوچک;
	إذا (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetGlyphs (لگاتار, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ثابت CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (لگاتار) : خالی_سوچک;
	إذا (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, اشاریہ_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetStringIndices (لگاتار, range_all, اشاریہ_buf);
	  string_indices = اشاریہ_buf;
	}
	hb_glyph_info_t *info = لگاتار_info;
	لكل (بنانشانی صحيح j = 0; j < عدد_glyphs; j++)
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
	ثابت CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (لگاتار) : خالی_سوچک;
	إذا (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetPositions (لگاتار, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = لگاتار_info;
	إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	{
	  hb_position_t x_وتھ_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  لكل (بنانشانی صحيح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    وإلا /* last glyph */
	      advance = لگاتار_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_وتھ_2;
	    info->var2.i32 = hb_clamp_منزل<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	وإلا
	{
	  hb_position_t y_وتھ_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  لكل (بنانشانی صحيح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    وإلا /* last glyph */
	      advance = لگاتار_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_منزل<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_وتھ_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += لگاتار_advance;
      }
#تعریف_رد SCRATCH_RESTORE
#تعریف_رد SCRATCH_SAVE
#تعریف_رد USE_PTR
#تعریف_رد ALLOCATE_ARRAY

      بفر->len += عدد_glyphs;
    }

    بفر->clear_positions ();

    بنانشانی صحيح گنتی = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
      لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      {
	pos->x_advance = info->نقاب;
	pos->x_وتھ_2 = info->var1.i32;
	pos->y_وتھ_2 = info->var2.i32;

	info++; pos++;
      }
    وإلا
      لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      {
	pos->y_advance = info->نقاب;
	pos->x_وتھ_2 = info->var1.i32;
	pos->y_وتھ_2 = info->var2.i32;

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
    إذا (گنتی > 1 && (حالت_or & k_CTRun_حالت_Non_Monotonic) &&
	HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
    {
      hb_glyph_info_t *info = بفر->info;
      إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
      {
	بنانشانی صحيح cluster = info[گنتی - 1].cluster;
	لكل (بنانشانی صحيح i = گنتی - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      وإلا
      {
	بنانشانی صحيح cluster = info[0].cluster;
	لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
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
  بفر->unsafe_منزل_break ();

#تعریف_رد FAIL

fail:
  إذا (string_ref)
    CFRelease (string_ref);
  إذا (سطر)
    CFRelease (سطر);

  لكل (بنانشانی صحيح i = 0; i < range_ریکارڈاں.لمائی; i++)
    إذا (range_ریکارڈاں[i].font)
      CFRelease (range_ریکارڈاں[i].font);

  أرجع ret;
}


#جے_انت
