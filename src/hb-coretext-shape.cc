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

#که_تعریف_شوی HAVE_CORETEXT

#ضمن "hb-shaper-impl.hh"

#ضمن "hb-coretext.hh"
#ضمن "hb-aat-layout.hh"

hb_coretext_face_معلومات_t *
hb_coretext_shaper_face_معلومات_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  إذا (unlikely (!cg_font))
  {
    DEBUG_پیغام (CORETEXT, face, "CGFont creation failed..");
    أرجع تش_اشاره;
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
  إذا (unlikely (!face_معلومات)) أرجع تش_اشاره;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->معلومات.coretext;

  CGFloat font_اندازه = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_اندازه);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_اندازه));

  إذا (unlikely (!ct_font))
  {
    DEBUG_پیغام (CORETEXT, font, "CGFont creation failed..");
    أرجع تش_اشاره;
  }

  إذا (font->عدد_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->عدد_coords,
				 &k_CFType_Dictionary_کلی_Call_Backs,
				 &k_CFType_Dictionary_ارزښت_Call_Backs));

    بینښه شمېر = font->عدد_coords;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      hb_ot_var_axis_info_t info;
      بینښه صحيح c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      عائم v = hb_clamp (font->design_coords[i], info.min_ارزښت, info.max_ارزښت);

      hb_cf_ptr_t<CFNumberRef> نښان_شمېره (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.نښان_2));
      hb_cf_ptr_t<CFNumberRef> ارزښت_شمېره (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      إذا (variations && نښان_شمېره && ارزښت_شمېره)
	CFDictionary_سېټ_ارزښت (variations, نښان_شمېره, ارزښت_شمېره);
    }

    ثابت فراغ *کلیګانې[] = { kCTFontVariationAttribute };
    ثابت فراغ *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> ځانګړتیاوې (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       کلیګانې,
				       values,
				       1,
				       &k_CFType_Dictionary_کلی_Call_Backs,
				       &k_CFType_Dictionary_ارزښت_Call_Backs) : تش_اشاره);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (ځانګړتیاوې ? CTFont_Descriptor_Create_With_ځانګړتیاوې (ځانګړتیاوې) : تش_اشاره);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_ځانګړتیاوې (ct_font, 0, تش_اشاره, varDesc) : تش_اشاره);
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

بنية feature_ریکارډ_t {
  بینښه صحيح feature;
  بینښه صحيح setting;
};

بنية فعال_feature_t {
  feature_ریکارډ_t rec;
  بینښه صحيح ترتیب;

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت فعال_feature_t *a = (ثابت فعال_feature_t *) pa;
    ثابت فعال_feature_t *b = (ثابت فعال_feature_t *) pb;
    أرجع a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ترتیب < b->ترتیب ? -1 : a->ترتیب > b->ترتیب ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  منطقي چلوونکی== (ثابت فعال_feature_t& f) ثابت {
    أرجع cmp (هذا, &f) == 0;
  }
};

بنية feature_event_t {
  بینښه صحيح شاخص_2;
  منطقي پیل;
  فعال_feature_t feature;

  HB_دننی ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb) {
    ثابت feature_event_t *a = (ثابت feature_event_t *) pa;
    ثابت feature_event_t *b = (ثابت feature_event_t *) pb;
    أرجع a->شاخص_2 < b->شاخص_2 ? -1 : a->شاخص_2 > b->شاخص_2 ? 1 :
	   a->پیل < b->پیل ? -1 : a->پیل > b->پیل ? 1 :
	   فعال_feature_t::cmp (&a->feature, &b->feature);
  }
};

بنية range_ریکارډ_t {
  CTFontRef font;
  بینښه صحيح شاخص_لومړی; /* == start */
  بینښه صحيح شاخص_وروستی;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_بفر_t        *بفر,
		    ثابت hb_feature_t *features,
		    بینښه صحيح        عدد_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ثابت فراغ *) face->معلومات.coretext;
  CTFontRef ct_font = (CTFontRef) (ثابت فراغ *) font->معلومات.coretext;

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
  إذا (HB_بفر_CLUSTER_LEVEL_IS_GRAPHEMES (بفر->cluster_level))
  {
    hb_unicode_funcs_t *unicode = بفر->unicode;
    بینښه صحيح شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    لكل (بینښه صحيح i = 1; i < شمېر; i++)
      إذا (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	بفر->یوځای_grapheme_clusters (i - 1, i + 1);
  }

  hb_ویکټور_t<range_ریکارډ_t> range_ریکارډونه;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  إذا (عدد_features)
  {
    /* Sort features by start/end events. */
    hb_ویکټور_t<feature_event_t> feature_events;
    لكل (بینښه صحيح i = 0; i < عدد_features; i++)
    {
      فعال_feature_t feature;

#إذا MAC_OS_X_نسخه_MIN_REQUIRED < 101000
      ثابت hb_aat_feature_mapping_t * mapping = hb_aat_layout_پیدا_کړه_feature_mapping (features[i].نښان_2);
      إذا (!mapping)
	استمر;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_منزل_Enable : mapping->selector_منزل_Disable;
#وإلا
      feature.rec.feature = features[i].نښان_2;
      feature.rec.setting = features[i].value;
#د_که_پای
      feature.ترتیب = i;

      feature_event_t *event;

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
      فعال_feature_t feature;
      feature.rec.feature = HB_نښان_NONE;
      feature.rec.setting = 0;
      feature.ترتیب = عدد_features + 1;

      feature_event_t *event = feature_events.push ();
      event->شاخص_2 = 0; /* This value does magic. */
      event->پیل = خطأ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_ویکټور_t<فعال_feature_t> فعال_features;
    بینښه صحيح وروستی_شاخص = 0;
    لكل (بینښه صحيح i = 0; i < feature_events.اوږدتیا; i++)
    {
      feature_event_t *event = &feature_events[i];

      إذا (event->شاخص_2 != وروستی_شاخص)
      {
	/* Save a snapshot of active features and the range. */
	range_ریکارډ_t *range = range_ریکارډونه.push ();

	إذا (فعال_features.اوږدتیا)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  لكل (بینښه صحيح j = 0; j < فعال_features.اوږدتیا; j++)
	  {
#إذا MAC_OS_X_نسخه_MIN_REQUIRED < 101000
	    CFStringRef کلیګانې[] = {
	      k_CTFont_Feature_Type_Identifier_کلی,
	      k_CTFont_Feature_Selector_Identifier_کلی
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#وإلا
	    حرف نښان_2[5] = {HB_UNTAG (فعال_features[j].rec.feature)};
	    CFTypeRef کلیګانې[] = {
	      k_CTFont_Open_Type_Feature_نښان,
	      k_CTFont_Open_Type_Feature_ارزښت
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, نښان_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &فعال_features[j].rec.setting)
	    };
#د_که_پای
	    static_assert ((ARRAY_اوږدتیا_CONST (کلیګانې) == ARRAY_اوږدتیا_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ثابت فراغ **) کلیګانې,
						       (ثابت فراغ **) values,
						       ARRAY_اوږدتیا (کلیګانې),
						       &k_CFType_Dictionary_کلی_Call_Backs,
						       &k_CFType_Dictionary_ارزښت_Call_Backs);
	    لكل (بینښه صحيح i = 0; i < ARRAY_اوږدتیا (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_ارزښت (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef ځانګړتیاوې = CFDictionaryCreate (kCFAllocatorDefault,
							   (ثابت فراغ **) &kCTFontFeatureSettingsAttribute,
							   (ثابت فراغ **) &features_array,
							   1,
							   &k_CFType_Dictionary_کلی_Call_Backs,
							   &k_CFType_Dictionary_ارزښت_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_ځانګړتیاوې (ځانګړتیاوې);
	  CFRelease (ځانګړتیاوې);

	  range->font = CTFont_Create_Copy_With_ځانګړتیاوې (ct_font, 0.0, تش_اشاره, font_desc);
	  CFRelease (font_desc);
	}
	وإلا
	{
	  range->font = تش_اشاره;
	}

	range->شاخص_لومړی = وروستی_شاخص;
	range->شاخص_وروستی  = event->شاخص_2 - 1;

	وروستی_شاخص = event->شاخص_2;
      }

      إذا (event->پیل)
      {
	فعال_features.push (event->feature);
      } وإلا {
	فعال_feature_t *feature = فعال_features.lsearch (event->feature);
	إذا (feature)
	  فعال_features.remove_ordered (feature - فعال_features.arrayZ);
      }
    }
  }

  بینښه صحيح scratch_اندازه;
  hb_بفر_t::scratch_بفر_t *scratch = بفر->get_scratch_بفر (&scratch_اندازه);

#تعریف_کړه ALLOCATE_ARRAY(Type, name, len, on_no_ځای) \
  Type *name = (Type *) scratch; \
  افعل { \
    بینښه صحيح _consumed = DIV_CEIL ((len) * حجم (Type), حجم (*scratch)); \
    إذا (unlikely (_consumed > scratch_اندازه)) \
    { \
      on_no_ځای; \
      أكد (0); \
    } \
    scratch += _consumed; \
    scratch_اندازه -= _consumed; \
  } طالما (0)

  ALLOCATE_ARRAY (UniChar, pchars, بفر->len * 2, ((فراغ)تش_اشاره) /*nothing*/);
  بینښه صحيح chars_len = 0;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++) {
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

  ALLOCATE_ARRAY (بینښه صحيح, log_clusters, chars_len, ((فراغ)تش_اشاره) /*nothing*/);
  chars_len = 0;
  لكل (بینښه صحيح i = 0; i < بفر->len; i++)
  {
    hb_codepoint_t c = بفر->info[i].codepoint;
    بینښه صحيح cluster = بفر->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    إذا (hb_دننه_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#تعریف_کړه FAIL(...) \
  HB_STMT_پیل { \
    DEBUG_پیغام (CORETEXT, تش_اشاره, __VA_ARGS__); \
    ret = خطأ; \
    اذهب fail; \
  } HB_STMT_پای_ټکی

  منطقي ret = ریښتیا;
  CFStringRef string_ref = تش_اشاره;
  CTLineRef کرښه = تش_اشاره;

  إذا (خطأ)
  {
resize_and_retry:
    DEBUG_پیغام (CORETEXT, بفر, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    أكد (string_ref);
    أكد (کرښه);
    CFRelease (string_ref);
    CFRelease (کرښه);
    string_ref = تش_اشاره;
    کرښه = تش_اشاره;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    بینښه صحيح old_scratch_کارول_شوی;
    hb_بفر_t::scratch_بفر_t *old_scratch;
    old_scratch = بفر->get_scratch_بفر (&old_scratch_کارول_شوی);
    old_scratch_کارول_شوی = scratch - old_scratch;

    إذا (unlikely (!بفر->ensure (بفر->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = بفر->get_scratch_بفر (&scratch_اندازه);
    pchars = reinterpret_cast<UniChar *> (((حرف *) scratch + ((حرف *) pchars - (حرف *) old_scratch)));
    log_clusters = reinterpret_cast<بینښه صحيح *> (((حرف *) scratch + ((حرف *) log_clusters - (حرف *) old_scratch)));
    scratch += old_scratch_کارول_شوی;
    scratch_اندازه -= old_scratch_کارول_شوی;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (تش_اشاره,
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
	CFAttributed_String_سېټ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      إذا (بفر->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#إذا !(defined(موخه_OS_IPHONE) && موخه_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#د_که_پای
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_منزل_string (بفر->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	إذا (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_سېټ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_سېټ_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      إذا (عدد_features && range_ریکارډونه.اوږدتیا)
      {
	بینښه صحيح پیل = 0;
	range_ریکارډ_t *وروستی_range = &range_ریکارډونه[0];
	لكل (بینښه صحيح k = 0; k < chars_len; k++)
	{
	  range_ریکارډ_t *range = وروستی_range;
	  طالما (log_clusters[k] < range->شاخص_لومړی)
	    range--;
	  طالما (log_clusters[k] > range->شاخص_وروستی)
	    range++;
	  إذا (range != وروستی_range)
	  {
	    إذا (وروستی_range->font)
	      CFAttributed_String_سېټ_Attribute (attr_string, CFRangeMake (پیل, k - پیل),
					      kCTFontAttributeName, وروستی_range->font);

	    پیل = k;
	  }

	  وروستی_range = range;
	}
	إذا (پیل != chars_len && وروستی_range->font)
	  CFAttributed_String_سېټ_Attribute (attr_string, CFRangeMake (پیل, chars_len - پیل),
					  kCTFontAttributeName, وروستی_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      إذا (عدد_features)
      {
	بینښه صحيح zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	لكل (بینښه صحيح i = 0; i < عدد_features; i++)
	{
	  ثابت hb_feature_t &feature = features[i];
	  إذا (feature.نښان_2 == HB_نښان('k','e','r','n') &&
	      feature.پیل < chars_len && feature.پیل < feature.پای_ټکی)
	  {
	    CFRange feature_range = CFRangeMake (feature.پیل,
						 hb_min (feature.پای_ټکی, chars_len) - feature.پیل);
	    إذا (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    وإلا
	      CFAttributed_String_سېټ_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      صحيح level = HB_DIRECTION_IS_FORWARD (بفر->props.direction) ? 0 : 1;
      CFNumberRef level_شمېره = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#إذا !(defined(موخه_OS_IPHONE) && موخه_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
      خارجي ثابت CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#د_که_پای
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ثابت فراغ **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ثابت فراغ **) &level_شمېره,
						    1,
						    &k_CFType_Dictionary_کلی_Call_Backs,
						    &k_CFType_Dictionary_ارزښت_Call_Backs);
      CFRelease (level_شمېره);
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

      کرښه = CTTypesetter_Create_کرښه (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      إذا (unlikely (!کرښه))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (کرښه);
    بینښه صحيح عدد_runs = CFArray_Get_شمېر (glyph_runs);
    DEBUG_پیغام (CORETEXT, تش_اشاره, "Num runs: %u", عدد_runs);

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
      advances_so_far -= CTLine_Get_Trailing_Whitespace_پلنوالی (کرښه);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ثابت CFRange range_all = CFRangeMake (0, 0);

    لكل (بینښه صحيح i = 0; i < عدد_runs; i++)
    {
      CTRunRef پرلپسې = static_cast<CTRunRef>(CFArray_Get_ارزښت_At_شاخص (glyph_runs, i));
      CTRun_حالت پرلپسې_حالت = CTRun_Get_حالت (پرلپسې);
      حالت_or  |= پرلپسې_حالت;
      DEBUG_پیغام (CORETEXT, پرلپسې, "CTRunStatus: %x", پرلپسې_حالت);
      CGFloat پرلپسې_advance = CTRunGetTypographicBounds (پرلپسې, range_all, تش_اشاره, تش_اشاره, تش_اشاره);
      إذا (HB_DIRECTION_IS_VERTICAL (بفر->props.direction))
	  پرلپسې_advance = -پرلپسې_advance;
      DEBUG_پیغام (CORETEXT, پرلپسې, "Run advance: %g", (مزدوج) پرلپسې_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef ځانګړتیاوې = CTRun_Get_ځانګړتیاوې (پرلپسې);
      CTFontRef پرلپسې_ct_font = static_cast<CTFontRef>(CFDictionary_Get_ارزښت (ځانګړتیاوې, kCTFontAttributeName));
      إذا (!CFEqual (پرلپسې_ct_font, ct_font))
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
	لكل (بینښه صحيح i = 0; i < range_ریکارډونه.اوږدتیا; i++)
	  إذا (range_ریکارډونه[i].font && CFEqual (پرلپسې_ct_font, range_ریکارډونه[i].font))
	  {
	    matched = ریښتیا;
	    اكسر;
	  }
	إذا (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> پرلپسې_cg_font (CTFontCopyGraphicsFont (پرلپسې_ct_font, تش_اشاره));
	  إذا (پرلپسې_cg_font)
	    matched = CFEqual (پرلپسې_cg_font, cg_font);
	}
	إذا (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_کلی));
	  hb_cf_ptr_t<CFStringRef> پرلپسې_ps_name (CTFontCopyName (پرلپسې_ct_font, k_CTFont_Post_Script_Name_کلی));
	  إذا (font_ps_name && پرلپسې_ps_name)
	  {
	    إذا (CFStringCompare (پرلپسې_ps_name, font_ps_name, 0) == k_CFCompare_Equal_منزل)
	      matched = ریښتیا;
	  }
	}
	إذا (!matched)
	{
	  CFRange range = CTRunGetStringRange (پرلپسې);
	  DEBUG_پیغام (CORETEXT, پرلپسې, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.اوږدتیا);
	  إذا (!بفر->ensure_inplace (بفر->len + range.اوږدتیا))
	    اذهب resize_and_retry;
	  hb_glyph_info_t *info = بفر->info + بفر->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = بفر->props.direction;
	  hb_position_t x_advance, y_advance, x_دننه_واټن_2, y_دننه_واټن_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_دننه_واټن_2, &y_دننه_واټن_2);
	  hb_position_t advance = hb_saturate_جمع_کړه (x_advance, y_advance);
	  x_دننه_واټن_2 = hb_saturate_neg (x_دننه_واټن_2);
	  y_دننه_واټن_2 = hb_saturate_neg (y_دننه_واټن_2);

	  بینښه صحيح old_len = بفر->len;
	  لكل (CFIndex j = range.location; j < range.location + range.اوږدتیا; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_شاخص (string_ref, j);
	      إذا (hb_دننه_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_شاخص (string_ref, j - 1);
		إذا (hb_دننه_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  استمر;
	      }
	      إذا (بفر->unicode->is_default_ignorable (ch))
		استمر;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->ماسک = advance;
	      info->var1.i32 = x_دننه_واټن_2;
	      info->var2.i32 = y_دننه_واټن_2;

	      info++;
	      بفر->len++;
	  }
	  إذا (HB_DIRECTION_IS_BACKWARD (بفر->props.direction))
	    بفر->سرچپه_range (old_len, بفر->len);
	  advances_so_far += پرلپسې_advance;
	  استمر;
	}
      }

      بینښه صحيح عدد_glyphs = CTRun_Get_Glyph_شمېر (پرلپسې);
      إذا (عدد_glyphs == 0)
	استمر;

      إذا (!بفر->ensure_inplace (بفر->len + عدد_glyphs))
	اذهب resize_and_retry;

      hb_glyph_info_t *پرلپسې_info = بفر->info + بفر->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#تعریف_کړه USE_PTR ریښتیا

#تعریف_کړه SCRATCH_SAVE() \
  بینښه صحيح scratch_اندازه_saved = scratch_اندازه; \
  hb_بفر_t::scratch_بفر_t *scratch_saved = scratch

#تعریف_کړه SCRATCH_RESTORE() \
  scratch_اندازه = scratch_اندازه_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ثابت CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (پرلپسې) : تش_اشاره;
	إذا (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetGlyphs (پرلپسې, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ثابت CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (پرلپسې) : تش_اشاره;
	إذا (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, شاخص_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetStringIndices (پرلپسې, range_all, شاخص_buf);
	  string_indices = شاخص_buf;
	}
	hb_glyph_info_t *info = پرلپسې_info;
	لكل (بینښه صحيح j = 0; j < عدد_glyphs; j++)
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
	ثابت CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (پرلپسې) : تش_اشاره;
	إذا (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, عدد_glyphs, اذهب resize_and_retry);
	  CTRunGetPositions (پرلپسې, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = پرلپسې_info;
	إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
	{
	  hb_position_t x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  لكل (بینښه صحيح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    وإلا /* last glyph */
	      advance = پرلپسې_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->ماسک = (hb_ماسک_t) hb_clamp_منزل<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_دننه_واټن_2;
	    info->var2.i32 = hb_clamp_منزل<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	وإلا
	{
	  hb_position_t y_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  لكل (بینښه صحيح j = 0; j < عدد_glyphs; j++)
	  {
	    CGFloat advance;
	    إذا (likely (j + 1 < عدد_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    وإلا /* last glyph */
	      advance = پرلپسې_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->ماسک = (hb_ماسک_t) hb_clamp_منزل<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_منزل<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_دننه_واټن_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += پرلپسې_advance;
      }
#تعریف_لغوه SCRATCH_RESTORE
#تعریف_لغوه SCRATCH_SAVE
#تعریف_لغوه USE_PTR
#تعریف_لغوه ALLOCATE_ARRAY

      بفر->len += عدد_glyphs;
    }

    بفر->clear_positions ();

    بینښه صحيح شمېر = بفر->len;
    hb_glyph_info_t *info = بفر->info;
    hb_glyph_position_t *pos = بفر->pos;
    إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
      لكل (بینښه صحيح i = 0; i < شمېر; i++)
      {
	pos->x_advance = info->ماسک;
	pos->x_دننه_واټن_2 = info->var1.i32;
	pos->y_دننه_واټن_2 = info->var2.i32;

	info++; pos++;
      }
    وإلا
      لكل (بینښه صحيح i = 0; i < شمېر; i++)
      {
	pos->y_advance = info->ماسک;
	pos->x_دننه_واټن_2 = info->var1.i32;
	pos->y_دننه_واټن_2 = info->var2.i32;

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
    إذا (شمېر > 1 && (حالت_or & k_CTRun_حالت_Non_Monotonic) &&
	HB_بفر_CLUSTER_LEVEL_IS_MONOTONE (بفر->cluster_level))
    {
      hb_glyph_info_t *info = بفر->info;
      إذا (HB_DIRECTION_IS_FORWARD (بفر->props.direction))
      {
	بینښه صحيح cluster = info[شمېر - 1].cluster;
	لكل (بینښه صحيح i = شمېر - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      وإلا
      {
	بینښه صحيح cluster = info[0].cluster;
	لكل (بینښه صحيح i = 1; i < شمېر; i++)
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

#تعریف_لغوه FAIL

fail:
  إذا (string_ref)
    CFRelease (string_ref);
  إذا (کرښه)
    CFRelease (کرښه);

  لكل (بینښه صحيح i = 0; i < range_ریکارډونه.اوږدتیا; i++)
    إذا (range_ریکارډونه[i].font)
      CFRelease (range_ریکارډونه[i].font);

  أرجع ret;
}


#د_که_پای
