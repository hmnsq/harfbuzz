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

#جيڪڏهن_وصف_ٿيل HAVE_CORETEXT

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-coretext.hh"
#ضمن "hb-aat-layout.hh"

hb_coretext_face_معلومات_t *
hb_coretext_shaper_face_معلومات_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  إذا (unlikely (!cg_font))
  {
    DEBUG_نياپو (CORETEXT, face, "CGFont creation failed..");
    أرجع خالي_اشارو;
  }

  أرجع (hb_coretext_face_معلومات_t *) cg_font;
}

فراغ
hb_coretext_shaper_face_معلومات_destroy (hb_coretext_face_معلومات_t *معلومات)
{
  إذا (معلومات)
    CFRelease ((CGFontRef) معلومات);
}


hb_coretext_font_معلومات_t *
hb_coretext_shaper_font_معلومات_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ثابت hb_coretext_face_معلومات_t *face_معلومات = face->معلومات.coretext;
  إذا (unlikely (!face_معلومات)) أرجع خالي_اشارو;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->معلومات.coretext;

  CGFloat font_سائيز = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_سائيز);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_سائيز));

  إذا (unlikely (!ct_font))
  {
    DEBUG_نياپو (CORETEXT, font, "CGFont creation failed..");
    أرجع خالي_اشارو;
  }

  إذا (font->انگ_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->انگ_coords,
				 &k_CFType_Dictionary_ڪنجي_Call_Backs,
				 &k_CFType_Dictionary_قدر_Call_Backs));

    بنانشان ڳڻپ = font->انگ_coords;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      hb_ot_var_axis_info_t info;
      بنانشان صحيح c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      عائم v = hb_clamp (font->design_coords[i], info.min_قدر, info.max_قدر);

      hb_cf_ptr_t<CFNumberRef> ٽيگ_عدد (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.ٽيگ_2));
      hb_cf_ptr_t<CFNumberRef> قدر_عدد (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      إذا (variations && ٽيگ_عدد && قدر_عدد)
	CFDictionary_سيٽ_قدر (variations, ٽيگ_عدد, قدر_عدد);
    }

    ثابت فراغ *ڪنجيون[] = { kCTFontVariationAttribute };
    ثابت فراغ *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> خاصيتون (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       ڪنجيون,
				       values,
				       1,
				       &k_CFType_Dictionary_ڪنجي_Call_Backs,
				       &k_CFType_Dictionary_قدر_Call_Backs) : خالي_اشارو);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (خاصيتون ? CTFont_Descriptor_Create_With_خاصيتون (خاصيتون) : خالي_اشارو);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_خاصيتون (ct_font, 0, خالي_اشارو, varDesc) : خالي_اشارو);
    إذا (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  أرجع (hb_coretext_font_معلومات_t *) ct_font.release ();
}

فراغ
hb_coretext_shaper_font_معلومات_destroy (hb_coretext_font_معلومات_t *معلومات)
{
  إذا (معلومات)
    CFRelease ((CTFontRef) معلومات);
}

/*
 * shaper
 */

بنية feature_رڪارڊ_t {
  بنانشان صحيح feature;
  بنانشان صحيح setting;
};

بنية سرگرم_feature_t {
  feature_رڪارڊ_t rec;
  بنانشان صحيح ترتيب;

  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت سرگرم_feature_t *a = (ثابت سرگرم_feature_t *) pa;
    ثابت سرگرم_feature_t *b = (ثابت سرگرم_feature_t *) pb;
    أرجع a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ترتيب < b->ترتيب ? -1 : a->ترتيب > b->ترتيب ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  منطقي عامل== (ثابت سرگرم_feature_t& f) ثابت {
    أرجع cmp (هذا, &f) == 0;
  }
};

بنية feature_event_t {
  بنانشان صحيح اشاريو_2;
  منطقي شروعات;
  سرگرم_feature_t feature;

  HB_اندروني ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
    ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
    أرجع a->اشاريو_2 < b->اشاريو_2 ? -1 : a->اشاريو_2 > b->اشاريو_2 ? 1 :
	   a->شروعات < b->شروعات ? -1 : a->شروعات > b->شروعات ? 1 :
	   سرگرم_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية range_رڪارڊ_t {
  CTFontRef font;
  بنانشان صحيح اشاريو_پهريون; /* == start */
  بنانشان صحيح اشاريو_آخري;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    بنانشان صحيح        انگ_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->معلومات.coretext;
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

  CGFloat ct_font_سائيز = CTFont_Get_سائيز (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_سائيز;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_سائيز;

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
    بنانشان صحيح ڳڻپ = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	بفر->ملايو_grapheme_clusters (i - 1, i + 1);
  }

  hb_ويڪٽر_t<range_رڪارڊ_t> range_رڪارڊون;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  إذا (انگ_features)
  {
    /* Sort features by start/end events. */
    hb_ويڪٽر_t<feature_event_t> feature_events;
    لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
    {
      سرگرم_feature_t feature;

#إذا MAC_OS_X_نسخو_MIN_REQUIRED < 101000
      ثابت hb_aat_feature_mapping_t * mapping = hb_aat_layout_ڳوليو_feature_mapping (features[i].ٽيگ_2);
      إذا (!mapping)
	استمر;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_منزل_Enable : mapping->selector_منزل_Disable;
#وإلا
      feature.rec.feature = features[i].ٽيگ_2;
      feature.rec.setting = features[i].value;
#جيڪڏهن_پورو
      feature.ترتيب = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->اشاريو_2 = features[i].شروعات;
      event->شروعات = سچ;
      event->feature = feature;

      event = feature_events.push ();
      event->اشاريو_2 = features[i].پڇاڙي;
      event->شروعات = خطأ;
      event->feature = feature;
    }
    feature_events.qsort ();
    /* Add a strategic final event. */
    {
      سرگرم_feature_t feature;
      feature.rec.feature = HB_ٽيگ_NONE;
      feature.rec.setting = 0;
      feature.ترتيب = انگ_features + 1;

      feature_event_t *event = feature_events.push ();
      event->اشاريو_2 = 0; /* This value does magic. */
      event->شروعات = خطأ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_ويڪٽر_t<سرگرم_feature_t> سرگرم_features;
    بنانشان صحيح آخري_اشاريو = 0;
    لكل (بنانشان صحيح i = 0; i < feature_events.ڊيگهائي; i++)
    {
      feature_event_t *event = &feature_events[i];

      إذا (event->اشاريو_2 != آخري_اشاريو)
      {
	/* Save a snapshot of active features and the range. */
	range_رڪارڊ_t *range = range_رڪارڊون.push ();

	إذا (سرگرم_features.ڊيگهائي)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  لكل (بنانشان صحيح j = 0; j < سرگرم_features.ڊيگهائي; j++)
	  {
#إذا MAC_OS_X_نسخو_MIN_REQUIRED < 101000
	    CFStringRef ڪنجيون[] = {
	      k_CTFont_Feature_Type_Identifier_ڪنجي,
	      k_CTFont_Feature_Selector_Identifier_ڪنجي
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.setting)
	    };
#وإلا
	    حرف ٽيگ_2[5] = {HB_UNTAG (سرگرم_features[j].rec.feature)};
	    CFTypeRef ڪنجيون[] = {
	      k_CTFont_Open_Type_Feature_ٽيگ,
	      k_CTFont_Open_Type_Feature_قدر
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, ٽيگ_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &سرگرم_features[j].rec.setting)
	    };
#جيڪڏهن_پورو
	    static_assert ((ARRAY_ڊيگهائي_CONST (ڪنجيون) == ARRAY_ڊيگهائي_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ثابت فراغ **) ڪنجيون,
						       (ثابت فراغ **) values,
						       ARRAY_ڊيگهائي (ڪنجيون),
						       &k_CFType_Dictionary_ڪنجي_Call_Backs,
						       &k_CFType_Dictionary_قدر_Call_Backs);
	    لكل (بنانشان صحيح i = 0; i < ARRAY_ڊيگهائي (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_قدر (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef خاصيتون = CFDictionaryCreate (kCFAllocatorDefault,
							   (ثابت فراغ **) &kCTFontFeatureSettingsAttribute,
							   (ثابت فراغ **) &features_array,
							   1,
							   &k_CFType_Dictionary_ڪنجي_Call_Backs,
							   &k_CFType_Dictionary_قدر_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_خاصيتون (خاصيتون);
	  CFRelease (خاصيتون);

	  range->font = CTFont_Create_Copy_With_خاصيتون (ct_font, 0.0, خالي_اشارو, font_desc);
	  CFRelease (font_desc);
	}
	وإلا
	{
	  range->font = خالي_اشارو;
	}

	range->اشاريو_پهريون = آخري_اشاريو;
	range->اشاريو_آخري  = event->اشاريو_2 - 1;

	آخري_اشاريو = event->اشاريو_2;
      }

      إذا (event->شروعات)
      {
	سرگرم_features.push (event->feature);
      } وإلا {
	سرگرم_feature_t *feature = سرگرم_features.lsearch (event->feature);
	إذا (feature)
	  سرگرم_features.remove_ordered (feature - سرگرم_features.arrayZ);
      }
    }
  }

  بنانشان صحيح scratch_سائيز;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_سائيز);

#وصف_ڪريو ALLOCATE_ARRAY(Type, name, len, on_no_گنجائش) \
  Type *name = (Type *) scratch; \
  افعل { \
    بنانشان صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    إذا (unlikely (_consumed > scratch_سائيز)) \
    { \
      on_no_گنجائش; \
      أكد (0); \
    } \
    scratch += _consumed; \
    scratch_سائيز -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (UniChar, pchars, بفر->len * 2, ((فراغ)خالي_اشارو) /*nothing*/);
  بنانشان صحيح chars_len = 0;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++) {
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

  ALLOCATE_ARRAY (بنانشان صحيح, log_clusters, chars_len, ((فراغ)خالي_اشارو) /*nothing*/);
  chars_len = 0;
  لكل (بنانشان صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بنانشان صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_اندريون_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#وصف_ڪريو FAIL(...) \
  HB_STMT_شروعات { \
    DEBUG_نياپو (CORETEXT, خالي_اشارو, __VA_ARGS__); \
    ret = خطأ; \
    اذهب fail; \
  } HB_STMT_پڇاڙي

  منطقي ret = سچ;
  CFStringRef string_ref = خالي_اشارو;
  CTLineRef سٽ = خالي_اشارو;

  إذا (خطأ)
  {
resize_and_retry:
    DEBUG_نياپو (CORETEXT, بفر, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    أكد (string_ref);
    أكد (سٽ);
    CFRelease (string_ref);
    CFRelease (سٽ);
    string_ref = خالي_اشارو;
    سٽ = خالي_اشارو;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    بنانشان صحيح old_scratch_استعمال_ٿيل;
    hb_بفر_t::scratch_بفر_t *old_scratch;
    old_scratch = بفر->get_scratch_بفر (&old_scratch_استعمال_ٿيل);
    old_scratch_استعمال_ٿيل = scratch - old_scratch;

    إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = بفر->get_scratch_بفر (&scratch_سائيز);
    pchars = reinterpret_cast<UniChar *> (((حرف *) scratch + ((حرف *) pchars - (حرف *) old_scratch)));
    log_clusters = reinterpret_cast<بنانشان صحيح *> (((حرف *) scratch + ((حرف *) log_clusters - (حرف *) old_scratch)));
    scratch += old_scratch_استعمال_ٿيل;
    scratch_سائيز -= old_scratch_استعمال_ٿيل;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (خالي_اشارو,
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
	CFAttributed_String_سيٽ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      إذا (بفر->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخو_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#جيڪڏهن_پورو
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_منزل_string (بفر->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	إذا (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_سيٽ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_سيٽ_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      إذا (انگ_features && range_رڪارڊون.ڊيگهائي)
      {
	بنانشان صحيح شروعات = 0;
	range_رڪارڊ_t *آخري_range = &range_رڪارڊون[0];
	لكل (بنانشان صحيح k = 0; k < chars_len; k++)
	{
	  range_رڪارڊ_t *range = آخري_range;
	  طالما (log_clusters[k] < range->اشاريو_پهريون)
	    range--;
	  طالما (log_clusters[k] > range->اشاريو_آخري)
	    range++;
	  إذا (range != آخري_range)
	  {
	    إذا (آخري_range->font)
	      CFAttributed_String_سيٽ_Attribute (attr_string, CFRangeMake (شروعات, k - شروعات),
					      kCTFontAttributeName, آخري_range->font);

	    شروعات = k;
	  }

	  آخري_range = range;
	}
	إذا (شروعات != chars_len && آخري_range->font)
	  CFAttributed_String_سيٽ_Attribute (attr_string, CFRangeMake (شروعات, chars_len - شروعات),
					  kCTFontAttributeName, آخري_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      إذا (انگ_features)
      {
	بنانشان صحيح zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	لكل (بنانشان صحيح i = 0; i < انگ_features; i++)
	{
	  ثابت hb_feature_t &feature = features[i];
	  إذا (feature.ٽيگ_2 == HB_ٽيگ('k','e','r','n') &&
	      feature.شروعات < chars_len && feature.شروعات < feature.پڇاڙي)
	  {
	    CFRange feature_range = CFRangeMake (feature.شروعات,
						 hb_min (feature.پڇاڙي, chars_len) - feature.شروعات);
	    إذا (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    وإلا
	      CFAttributed_String_سيٽ_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      صحيح level = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
      CFNumberRef level_عدد = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخو_MIN_REQUIRED < 1060
      خارجي ثابت CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#جيڪڏهن_پورو
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ثابت فراغ **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ثابت فراغ **) &level_عدد,
						    1,
						    &k_CFType_Dictionary_ڪنجي_Call_Backs,
						    &k_CFType_Dictionary_قدر_Call_Backs);
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

      سٽ = CTTypesetter_Create_سٽ (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      إذا (unlikely (!سٽ))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (سٽ);
    بنانشان صحيح انگ_runs = CFArray_Get_ڳڻپ (glyph_runs);
    DEBUG_نياپو (CORETEXT, خالي_اشارو, "Num runs: %u", انگ_runs);

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
      advances_so_far -= CTLine_Get_Trailing_Whitespace_ويڪر (سٽ);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ثابت CFRange range_all = CFRangeMake (0, 0);

    لكل (بنانشان صحيح i = 0; i < انگ_runs; i++)
    {
      CTRunRef لاڳيتو = static_cast<CTRunRef>(CFArray_Get_قدر_At_اشاريو (glyph_runs, i));
      CTRun_حالت لاڳيتو_حالت = CTRun_Get_حالت (لاڳيتو);
      حالت_or  |= لاڳيتو_حالت;
      DEBUG_نياپو (CORETEXT, لاڳيتو, "CTRunStatus: %x", لاڳيتو_حالت);
      CGFloat لاڳيتو_advance = CTRunGetTypographicBounds (لاڳيتو, range_all, خالي_اشارو, خالي_اشارو, خالي_اشارو);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  لاڳيتو_advance = -لاڳيتو_advance;
      DEBUG_نياپو (CORETEXT, لاڳيتو, "Run advance: %g", (مزدوج) لاڳيتو_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef خاصيتون = CTRun_Get_خاصيتون (لاڳيتو);
      CTFontRef لاڳيتو_ct_font = static_cast<CTFontRef>(CFDictionary_Get_قدر (خاصيتون, kCTFontAttributeName));
      إذا (!CFEqual (لاڳيتو_ct_font, ct_font))
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
	لكل (بنانشان صحيح i = 0; i < range_رڪارڊون.ڊيگهائي; i++)
	  إذا (range_رڪارڊون[i].font && CFEqual (لاڳيتو_ct_font, range_رڪارڊون[i].font))
	  {
	    matched = سچ;
	    اكسر;
	  }
	إذا (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> لاڳيتو_cg_font (CTFontCopyGraphicsFont (لاڳيتو_ct_font, خالي_اشارو));
	  إذا (لاڳيتو_cg_font)
	    matched = CFEqual (لاڳيتو_cg_font, cg_font);
	}
	إذا (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_ڪنجي));
	  hb_cf_ptr_t<CFStringRef> لاڳيتو_ps_name (CTFontCopyName (لاڳيتو_ct_font, k_CTFont_Post_Script_Name_ڪنجي));
	  إذا (font_ps_name && لاڳيتو_ps_name)
	  {
	    إذا (CFStringCompare (لاڳيتو_ps_name, font_ps_name, 0) == k_CFCompare_Equal_منزل)
	      matched = سچ;
	  }
	}
	إذا (!matched)
	{
	  CFRange range = CTRunGetStringRange (لاڳيتو);
	  DEBUG_نياپو (CORETEXT, لاڳيتو, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.ڊيگهائي);
	  إذا (!بفر->ensure_inplace (بفر->len + range.ڊيگهائي))
	    اذهب resize_and_retry;
	  hb_glyph_info_t *info = بفر->info + بفر->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = بفر->props.direction;
	  hb_position_t x_advance, y_advance, x_وٿي_2, y_وٿي_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_وٿي_2, &y_وٿي_2);
	  hb_position_t advance = hb_saturate_جوڙيو (x_advance, y_advance);
	  x_وٿي_2 = hb_saturate_neg (x_وٿي_2);
	  y_وٿي_2 = hb_saturate_neg (y_وٿي_2);

	  بنانشان صحيح old_len = بفر->len;
	  لكل (CFIndex j = range.location; j < range.location + range.ڊيگهائي; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_اشاريو (string_ref, j);
	      إذا (hb_اندريون_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_اشاريو (string_ref, j - 1);
		إذا (hb_اندريون_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  استمر;
	      }
	      إذا (بفر->unicode->is_default_ignorable (ch))
		استمر;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->نقاب = advance;
	      info->var1.i32 = x_وٿي_2;
	      info->var2.i32 = y_وٿي_2;

	      info++;
	      بفر->len++;
	  }
	  إذا (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
	    بفر->ابتو_range (old_len, بفر->len);
	  advances_so_far += لاڳيتو_advance;
	  استمر;
	}
      }

      بنانشان صحيح انگ_glyphs = CTRun_Get_Glyph_ڳڻپ (لاڳيتو);
      إذا (انگ_glyphs == 0)
	استمر;

      إذا (!بفر->ensure_inplace (بفر->len + انگ_glyphs))
	اذهب resize_and_retry;

      hb_glyph_info_t *لاڳيتو_info = بفر->info + بفر->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#وصف_ڪريو USE_PTR سچ

#وصف_ڪريو SCRATCH_SAVE() \
  بنانشان صحيح scratch_سائيز_saved = scratch_سائيز; \
  hb_بفر_t::scratch_بفر_t *scratch_saved = scratch

#وصف_ڪريو SCRATCH_RESTORE() \
  scratch_سائيز = scratch_سائيز_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ثابت CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (لاڳيتو) : خالي_اشارو;
	إذا (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, انگ_glyphs, اذهب resize_and_retry);
	  CTRunGetGlyphs (لاڳيتو, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ثابت CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (لاڳيتو) : خالي_اشارو;
	إذا (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, اشاريو_buf, انگ_glyphs, اذهب resize_and_retry);
	  CTRunGetStringIndices (لاڳيتو, range_all, اشاريو_buf);
	  string_indices = اشاريو_buf;
	}
	hb_glyph_info_t *info = لاڳيتو_info;
	لكل (بنانشان صحيح j = 0; j < انگ_glyphs; j++)
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
	ثابت CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (لاڳيتو) : خالي_اشارو;
	إذا (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, انگ_glyphs, اذهب resize_and_retry);
	  CTRunGetPositions (لاڳيتو, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = لاڳيتو_info;
	إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	{
	  hb_position_t x_وٿي_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  لكل (بنانشان صحيح j = 0; j < انگ_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < انگ_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    وإلا /* last glyph */
	      advance = لاڳيتو_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_وٿي_2;
	    info->var2.i32 = hb_clamp_منزل<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	وإلا
	{
	  hb_position_t y_وٿي_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  لكل (بنانشان صحيح j = 0; j < انگ_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < انگ_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    وإلا /* last glyph */
	      advance = لاڳيتو_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->نقاب = (hb_نقاب_t) hb_clamp_منزل<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_منزل<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_وٿي_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += لاڳيتو_advance;
      }
#وصف_رد SCRATCH_RESTORE
#وصف_رد SCRATCH_SAVE
#وصف_رد USE_PTR
#وصف_رد ALLOCATE_ARRAY

      بفر->len += انگ_glyphs;
    }

    بفر->clear_positions ();

    بنانشان صحيح ڳڻپ = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
      لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      {
	pos->x_advance = info->نقاب;
	pos->x_وٿي_2 = info->var1.i32;
	pos->y_وٿي_2 = info->var2.i32;

	info++; pos++;
      }
    وإلا
      لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      {
	pos->y_advance = info->نقاب;
	pos->x_وٿي_2 = info->var1.i32;
	pos->y_وٿي_2 = info->var2.i32;

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
    إذا (ڳڻپ > 1 && (حالت_or & k_CTRun_حالت_Non_Monotonic) &&
	HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
    {
      hb_glyph_info_t *info = بفر->info;
      إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
      {
	بنانشان صحيح cluster = info[ڳڻپ - 1].cluster;
	لكل (بنانشان صحيح i = ڳڻپ - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      وإلا
      {
	بنانشان صحيح cluster = info[0].cluster;
	لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
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

#وصف_رد FAIL

fail:
  إذا (string_ref)
    CFRelease (string_ref);
  إذا (سٽ)
    CFRelease (سٽ);

  لكل (بنانشان صحيح i = 0; i < range_رڪارڊون.ڊيگهائي; i++)
    إذا (range_رڪارڊون[i].font)
      CFRelease (range_رڪارڊون[i].font);

  أرجع ret;
}


#جيڪڏهن_پورو
