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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_CORETEXT

#അന്തര്ഭാവ "hb-shaper-impl.hh"

#അന്തര്ഭാവ "hb-coretext.hh"
#അന്തര്ഭാവ "hb-aat-layout.hh"

hb_coretext_face_data_t *
_hb_coretext_shaper_face_data_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  എങ്കിൽ (unlikely (!cg_font))
  {
    DEBUG_സന്ദേശം (CORETEXT, face, "CGFont creation failed..");
    തിരികെ nullptr;
  }

  തിരികെ (hb_coretext_face_data_t *) cg_font;
}

ശൂന്യം
_hb_coretext_shaper_face_data_destroy (hb_coretext_face_data_t *data)
{
  എങ്കിൽ (data)
    CFRelease ((CGFontRef) data);
}


hb_coretext_font_data_t *
_hb_coretext_shaper_font_data_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  സ്ഥിരം hb_coretext_face_data_t *face_data = face->data.coretext;
  എങ്കിൽ (unlikely (!face_data)) തിരികെ nullptr;
  CGFontRef cg_font = (CGFontRef) (സ്ഥിരം ശൂന്യം *) face->data.coretext;

  CGFloat font_size = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_size));

  എങ്കിൽ (unlikely (!ct_font))
  {
    DEBUG_സന്ദേശം (CORETEXT, font, "CGFont creation failed..");
    തിരികെ nullptr;
  }

  എങ്കിൽ (font->അക്കം_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->അക്കം_coords,
				 &k_CFType_Dictionary_താക്കോൽ_Call_Backs,
				 &k_CFType_Dictionary_മൂല്യം_Call_Backs));

    unsigned എണ്ണം = font->അക്കം_coords;
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
    {
      hb_ot_var_axis_info_t info;
      unsigned പൂർണ്ണസംഖ്യ c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      ദശാംശം v = hb_clamp (font->design_coords[i], info.min_മൂല്യം, info.max_മൂല്യം);

      hb_cf_ptr_t<CFNumberRef> tag_സംഖ്യ (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.tag));
      hb_cf_ptr_t<CFNumberRef> മൂല്യം_സംഖ്യ (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      എങ്കിൽ (variations && tag_സംഖ്യ && മൂല്യം_സംഖ്യ)
	CFDictionary_Set_മൂല്യം (variations, tag_സംഖ്യ, മൂല്യം_സംഖ്യ);
    }

    സ്ഥിരം ശൂന്യം *താക്കോലുകൾ[] = { kCTFontVariationAttribute };
    സ്ഥിരം ശൂന്യം *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> attributes (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       താക്കോലുകൾ,
				       values,
				       1,
				       &k_CFType_Dictionary_താക്കോൽ_Call_Backs,
				       &k_CFType_Dictionary_മൂല്യം_Call_Backs) : nullptr);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (attributes ? CTFontDescriptorCreateWithAttributes (attributes) : nullptr);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFontCreateCopyWithAttributes (ct_font, 0, nullptr, varDesc) : nullptr);
    എങ്കിൽ (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  തിരികെ (hb_coretext_font_data_t *) ct_font.release ();
}

ശൂന്യം
_hb_coretext_shaper_font_data_destroy (hb_coretext_font_data_t *data)
{
  എങ്കിൽ (data)
    CFRelease ((CTFontRef) data);
}

/*
 * shaper
 */

ഘടന feature_record_t {
  unsigned പൂർണ്ണസംഖ്യ feature;
  unsigned പൂർണ്ണസംഖ്യ setting;
};

ഘടന active_feature_t {
  feature_record_t rec;
  unsigned പൂർണ്ണസംഖ്യ order;

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb) {
    സ്ഥിരം active_feature_t *a = (സ്ഥിരം active_feature_t *) pa;
    സ്ഥിരം active_feature_t *b = (സ്ഥിരം active_feature_t *) pb;
    തിരികെ a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  തർക്കം operator== (സ്ഥിരം active_feature_t& f) സ്ഥിരം {
    തിരികെ cmp (ഇത്, &f) == 0;
  }
};

ഘടന feature_event_t {
  unsigned പൂർണ്ണസംഖ്യ index;
  തർക്കം start;
  active_feature_t feature;

  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb) {
    സ്ഥിരം feature_event_t *a = (സ്ഥിരം feature_event_t *) pa;
    സ്ഥിരം feature_event_t *b = (സ്ഥിരം feature_event_t *) pb;
    തിരികെ a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   active_feature_t::cmp (&a->feature, &b->feature);
  }
};

ഘടന range_record_t {
  CTFontRef font;
  unsigned പൂർണ്ണസംഖ്യ index_first; /* == start */
  unsigned പൂർണ്ണസംഖ്യ index_last;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    സ്ഥിരം hb_feature_t *features,
		    unsigned പൂർണ്ണസംഖ്യ        അക്കം_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (സ്ഥിരം ശൂന്യം *) face->data.coretext;
  CTFontRef ct_font = (CTFontRef) (സ്ഥിരം ശൂന്യം *) font->data.coretext;

  CGFloat ct_font_size = CTFontGetSize (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_size;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_size;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  എങ്കിൽ (HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (buffer->cluster_level))
  {
    hb_unicode_funcs_t *unicode = buffer->unicode;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
      എങ്കിൽ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	buffer->merge_grapheme_clusters (i - 1, i + 1);
  }

  hb_vector_t<range_record_t> range_records;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  എങ്കിൽ (അക്കം_features)
  {
    /* Sort features by start/end events. */
    hb_vector_t<feature_event_t> feature_events;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
    {
      active_feature_t feature;

#എങ്കിൽ MAC_OS_X_VERSION_MIN_REQUIRED < 101000
      സ്ഥിരം hb_aat_feature_mapping_t * mapping = hb_aat_layout_find_feature_mapping (features[i].tag);
      എങ്കിൽ (!mapping)
	തുടരുക;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selectorToEnable : mapping->selectorToDisable;
#അല്ലെങ്കിൽ
      feature.rec.feature = features[i].tag;
      feature.rec.setting = features[i].value;
#endif
      feature.order = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->index = features[i].start;
      event->start = സത്യം;
      event->feature = feature;

      event = feature_events.push ();
      event->index = features[i].end;
      event->start = അസത്യം;
      event->feature = feature;
    }
    feature_events.qsort ();
    /* Add a strategic final event. */
    {
      active_feature_t feature;
      feature.rec.feature = HB_TAG_NONE;
      feature.rec.setting = 0;
      feature.order = അക്കം_features + 1;

      feature_event_t *event = feature_events.push ();
      event->index = 0; /* This value does magic. */
      event->start = അസത്യം;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_vector_t<active_feature_t> active_features;
    unsigned പൂർണ്ണസംഖ്യ last_index = 0;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < feature_events.length; i++)
    {
      feature_event_t *event = &feature_events[i];

      എങ്കിൽ (event->index != last_index)
      {
	/* Save a snapshot of active features and the range. */
	range_record_t *range = range_records.push ();

	എങ്കിൽ (active_features.length)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < active_features.length; j++)
	  {
#എങ്കിൽ MAC_OS_X_VERSION_MIN_REQUIRED < 101000
	    CFStringRef താക്കോലുകൾ[] = {
	      k_CTFont_Feature_Type_Identifier_താക്കോൽ,
	      k_CTFont_Feature_Selector_Identifier_താക്കോൽ
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#അല്ലെങ്കിൽ
	    അക്ഷരം tag[5] = {HB_UNTAG (active_features[j].rec.feature)};
	    CFTypeRef താക്കോലുകൾ[] = {
	      kCTFontOpenTypeFeatureTag,
	      k_CTFont_Open_Type_Feature_മൂല്യം
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, tag, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#endif
	    static_assert ((ARRAY_LENGTH_CONST (താക്കോലുകൾ) == ARRAY_LENGTH_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (സ്ഥിരം ശൂന്യം **) താക്കോലുകൾ,
						       (സ്ഥിരം ശൂന്യം **) values,
						       ARRAY_LENGTH (താക്കോലുകൾ),
						       &k_CFType_Dictionary_താക്കോൽ_Call_Backs,
						       &k_CFType_Dictionary_മൂല്യം_Call_Backs);
	    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_LENGTH (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_മൂല്യം (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef attributes = CFDictionaryCreate (kCFAllocatorDefault,
							   (സ്ഥിരം ശൂന്യം **) &kCTFontFeatureSettingsAttribute,
							   (സ്ഥിരം ശൂന്യം **) &features_array,
							   1,
							   &k_CFType_Dictionary_താക്കോൽ_Call_Backs,
							   &k_CFType_Dictionary_മൂല്യം_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFontDescriptorCreateWithAttributes (attributes);
	  CFRelease (attributes);

	  range->font = CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, font_desc);
	  CFRelease (font_desc);
	}
	അല്ലെങ്കിൽ
	{
	  range->font = nullptr;
	}

	range->index_first = last_index;
	range->index_last  = event->index - 1;

	last_index = event->index;
      }

      എങ്കിൽ (event->start)
      {
	active_features.push (event->feature);
      } അല്ലെങ്കിൽ {
	active_feature_t *feature = active_features.lsearch (event->feature);
	എങ്കിൽ (feature)
	  active_features.remove_ordered (feature - active_features.arrayZ);
      }
    }
  }

  unsigned പൂർണ്ണസംഖ്യ scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len, on_no_room) \
  Type *name = (Type *) scratch; \
  ചെയ്യുക { \
    unsigned പൂർണ്ണസംഖ്യ _consumed = DIV_CEIL ((len) * വലുപ്പം (Type), വലുപ്പം (*scratch)); \
    എങ്കിൽ (unlikely (_consumed > scratch_size)) \
    { \
      on_no_room; \
      പ്രതിജ്ഞാ (0); \
    } \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } ആകുമ്പോൾ (0)

  ALLOCATE_ARRAY (UniChar, pchars, buffer->len * 2, ((ശൂന്യം)nullptr) /*nothing*/);
  unsigned പൂർണ്ണസംഖ്യ chars_len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++) {
    hb_codepoint_t c = buffer->info[i].codepoint;
    എങ്കിൽ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    അല്ലെങ്കിൽ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (unsigned പൂർണ്ണസംഖ്യ, log_clusters, chars_len, ((ശൂന്യം)nullptr) /*nothing*/);
  chars_len = 0;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned പൂർണ്ണസംഖ്യ cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    എങ്കിൽ (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_സന്ദേശം (CORETEXT, nullptr, __VA_ARGS__); \
    ret = അസത്യം; \
    പോകുക fail; \
  } HB_STMT_END

  തർക്കം ret = സത്യം;
  CFStringRef string_ref = nullptr;
  CTLineRef വരി = nullptr;

  എങ്കിൽ (അസത്യം)
  {
resize_and_retry:
    DEBUG_സന്ദേശം (CORETEXT, buffer, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    പ്രതിജ്ഞാ (string_ref);
    പ്രതിജ്ഞാ (വരി);
    CFRelease (string_ref);
    CFRelease (വരി);
    string_ref = nullptr;
    വരി = nullptr;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    unsigned പൂർണ്ണസംഖ്യ old_scratch_used;
    hb_buffer_t::scratch_buffer_t *old_scratch;
    old_scratch = buffer->get_scratch_buffer (&old_scratch_used);
    old_scratch_used = scratch - old_scratch;

    എങ്കിൽ (unlikely (!buffer->ensure (buffer->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = buffer->get_scratch_buffer (&scratch_size);
    pchars = reinterpret_cast<UniChar *> (((അക്ഷരം *) scratch + ((അക്ഷരം *) pchars - (അക്ഷരം *) old_scratch)));
    log_clusters = reinterpret_cast<unsigned പൂർണ്ണസംഖ്യ *> (((അക്ഷരം *) scratch + ((അക്ഷരം *) log_clusters - (അക്ഷരം *) old_scratch)));
    scratch += old_scratch_used;
    scratch_size -= old_scratch_used;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (nullptr,
						     pchars, chars_len,
						     kCFAllocatorNull);
    എങ്കിൽ (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      എങ്കിൽ (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      എങ്കിൽ (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
      {
	CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      എങ്കിൽ (buffer->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#എങ്കിൽ !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#endif
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_to_string (buffer->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	എങ്കിൽ (unlikely (!lang))
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

      എങ്കിൽ (അക്കം_features && range_records.length)
      {
	unsigned പൂർണ്ണസംഖ്യ start = 0;
	range_record_t *last_range = &range_records[0];
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = 0; k < chars_len; k++)
	{
	  range_record_t *range = last_range;
	  ആകുമ്പോൾ (log_clusters[k] < range->index_first)
	    range--;
	  ആകുമ്പോൾ (log_clusters[k] > range->index_last)
	    range++;
	  എങ്കിൽ (range != last_range)
	  {
	    എങ്കിൽ (last_range->font)
	      CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, k - start),
					      kCTFontAttributeName, last_range->font);

	    start = k;
	  }

	  last_range = range;
	}
	എങ്കിൽ (start != chars_len && last_range->font)
	  CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, chars_len - start),
					  kCTFontAttributeName, last_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      എങ്കിൽ (അക്കം_features)
      {
	unsigned പൂർണ്ണസംഖ്യ zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_features; i++)
	{
	  സ്ഥിരം hb_feature_t &feature = features[i];
	  എങ്കിൽ (feature.tag == HB_TAG('k','e','r','n') &&
	      feature.start < chars_len && feature.start < feature.end)
	  {
	    CFRange feature_range = CFRangeMake (feature.start,
						 hb_min (feature.end, chars_len) - feature.start);
	    എങ്കിൽ (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    അല്ലെങ്കിൽ
	      CFAttributedStringSetAttribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      പൂർണ്ണസംഖ്യ level = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
      CFNumberRef level_സംഖ്യ = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#എങ്കിൽ !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
      ബാഹ്യ സ്ഥിരം CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#endif
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (സ്ഥിരം ശൂന്യം **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (സ്ഥിരം ശൂന്യം **) &level_സംഖ്യ,
						    1,
						    &k_CFType_Dictionary_താക്കോൽ_Call_Backs,
						    &k_CFType_Dictionary_മൂല്യം_Call_Backs);
      CFRelease (level_സംഖ്യ);
      എങ്കിൽ (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      എങ്കിൽ (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      വരി = CTTypesetter_Create_വരി (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      എങ്കിൽ (unlikely (!വരി))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (വരി);
    unsigned പൂർണ്ണസംഖ്യ അക്കം_runs = CFArray_Get_എണ്ണം (glyph_runs);
    DEBUG_സന്ദേശം (CORETEXT, nullptr, "Num runs: %u", അക്കം_runs);

    buffer->len = 0;
    uint32_t status_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    എങ്കിൽ (HB_DIRECTION_IS_BACKWARD (buffer->props.direction))
    {
      advances_so_far -= CTLineGetTrailingWhitespaceWidth (വരി);
      എങ്കിൽ (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
	  advances_so_far = -advances_so_far;
    }

    സ്ഥിരം CFRange range_all = CFRangeMake (0, 0);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_runs; i++)
    {
      CTRunRef run = static_cast<CTRunRef>(CFArray_Get_മൂല്യം_At_Index (glyph_runs, i));
      CTRunStatus run_status = CTRunGetStatus (run);
      status_or  |= run_status;
      DEBUG_സന്ദേശം (CORETEXT, run, "CTRunStatus: %x", run_status);
      CGFloat run_advance = CTRunGetTypographicBounds (run, range_all, nullptr, nullptr, nullptr);
      എങ്കിൽ (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
	  run_advance = -run_advance;
      DEBUG_സന്ദേശം (CORETEXT, run, "Run advance: %g", (ഇരട്ട) run_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef attributes = CTRunGetAttributes (run);
      CTFontRef run_ct_font = static_cast<CTFontRef>(CFDictionary_Get_മൂല്യം (attributes, kCTFontAttributeName));
      എങ്കിൽ (!CFEqual (run_ct_font, ct_font))
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
	തർക്കം matched = അസത്യം;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < range_records.length; i++)
	  എങ്കിൽ (range_records[i].font && CFEqual (run_ct_font, range_records[i].font))
	  {
	    matched = സത്യം;
	    നിർത്തുക;
	  }
	എങ്കിൽ (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> run_cg_font (CTFontCopyGraphicsFont (run_ct_font, nullptr));
	  എങ്കിൽ (run_cg_font)
	    matched = CFEqual (run_cg_font, cg_font);
	}
	എങ്കിൽ (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_താക്കോൽ));
	  hb_cf_ptr_t<CFStringRef> run_ps_name (CTFontCopyName (run_ct_font, k_CTFont_Post_Script_Name_താക്കോൽ));
	  എങ്കിൽ (font_ps_name && run_ps_name)
	  {
	    എങ്കിൽ (CFStringCompare (run_ps_name, font_ps_name, 0) == kCFCompareEqualTo)
	      matched = സത്യം;
	  }
	}
	എങ്കിൽ (!matched)
	{
	  CFRange range = CTRunGetStringRange (run);
	  DEBUG_സന്ദേശം (CORETEXT, run, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.length);
	  എങ്കിൽ (!buffer->ensure_inplace (buffer->len + range.length))
	    പോകുക resize_and_retry;
	  hb_glyph_info_t *info = buffer->info + buffer->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = buffer->props.direction;
	  hb_position_t x_advance, y_advance, x_offset, y_offset;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_offset, &y_offset);
	  hb_position_t advance = hb_saturate_add (x_advance, y_advance);
	  x_offset = hb_saturate_neg (x_offset);
	  y_offset = hb_saturate_neg (y_offset);

	  unsigned പൂർണ്ണസംഖ്യ old_len = buffer->len;
	  വേണ്ടി (CFIndex j = range.location; j < range.location + range.length; j++)
	  {
	      UniChar ch = CFStringGetCharacterAtIndex (string_ref, j);
	      എങ്കിൽ (hb_in_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFStringGetCharacterAtIndex (string_ref, j - 1);
		എങ്കിൽ (hb_in_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  തുടരുക;
	      }
	      എങ്കിൽ (buffer->unicode->is_default_ignorable (ch))
		തുടരുക;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->mask = advance;
	      info->var1.i32 = x_offset;
	      info->var2.i32 = y_offset;

	      info++;
	      buffer->len++;
	  }
	  എങ്കിൽ (HB_DIRECTION_IS_BACKWARD (buffer->props.direction))
	    buffer->reverse_range (old_len, buffer->len);
	  advances_so_far += run_advance;
	  തുടരുക;
	}
      }

      unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs = CTRun_Get_Glyph_എണ്ണം (run);
      എങ്കിൽ (അക്കം_glyphs == 0)
	തുടരുക;

      എങ്കിൽ (!buffer->ensure_inplace (buffer->len + അക്കം_glyphs))
	പോകുക resize_and_retry;

      hb_glyph_info_t *run_info = buffer->info + buffer->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#define USE_PTR സത്യം

#define SCRATCH_SAVE() \
  unsigned പൂർണ്ണസംഖ്യ scratch_size_saved = scratch_size; \
  hb_buffer_t::scratch_buffer_t *scratch_saved = scratch

#define SCRATCH_RESTORE() \
  scratch_size = scratch_size_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	സ്ഥിരം CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (run) : nullptr;
	എങ്കിൽ (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, അക്കം_glyphs, പോകുക resize_and_retry);
	  CTRunGetGlyphs (run, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	സ്ഥിരം CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (run) : nullptr;
	എങ്കിൽ (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, index_buf, അക്കം_glyphs, പോകുക resize_and_retry);
	  CTRunGetStringIndices (run, range_all, index_buf);
	  string_indices = index_buf;
	}
	hb_glyph_info_t *info = run_info;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_glyphs; j++)
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
	സ്ഥിരം CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (run) : nullptr;
	എങ്കിൽ (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, അക്കം_glyphs, പോകുക resize_and_retry);
	  CTRunGetPositions (run, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = run_info;
	എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	{
	  hb_position_t x_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_glyphs; j++)
	  {
	    CGFloat advance;
	    എങ്കിൽ (likely (j + 1 < അക്കം_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    അല്ലെങ്കിൽ /* last glyph */
	      advance = run_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_offset;
	    info->var2.i32 = hb_clamp_to<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	അല്ലെങ്കിൽ
	{
	  hb_position_t y_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < അക്കം_glyphs; j++)
	  {
	    CGFloat advance;
	    എങ്കിൽ (likely (j + 1 < അക്കം_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    അല്ലെങ്കിൽ /* last glyph */
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

      buffer->len += അക്കം_glyphs;
    }

    buffer->clear_positions ();

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      {
	pos->x_advance = info->mask;
	pos->x_offset = info->var1.i32;
	pos->y_offset = info->var2.i32;

	info++; pos++;
      }
    അല്ലെങ്കിൽ
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
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
    എങ്കിൽ (എണ്ണം > 1 && (status_or & kCTRunStatusNonMonotonic) &&
	HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
    {
      hb_glyph_info_t *info = buffer->info;
      എങ്കിൽ (HB_DIRECTION_IS_FORWARD (buffer->props.direction))
      {
	unsigned പൂർണ്ണസംഖ്യ cluster = info[എണ്ണം - 1].cluster;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = എണ്ണം - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      അല്ലെങ്കിൽ
      {
	unsigned പൂർണ്ണസംഖ്യ cluster = info[0].cluster;
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
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

  buffer->clear_glyph_flags ();
  buffer->unsafe_to_break ();

#undef FAIL

fail:
  എങ്കിൽ (string_ref)
    CFRelease (string_ref);
  എങ്കിൽ (വരി)
    CFRelease (വരി);

  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < range_records.length; i++)
    എങ്കിൽ (range_records[i].font)
      CFRelease (range_records[i].font);

  തിരികെ ret;
}


#endif
