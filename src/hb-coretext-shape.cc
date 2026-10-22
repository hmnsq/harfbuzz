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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_CORETEXT

#અન્તર્ભાવ "hb-shaper-impl.hh"

#અન્તર્ભાવ "hb-coretext.hh"
#અન્તર્ભાવ "hb-aat-layout.hh"

hb_coretext_face_data_t *
_hb_coretext_shaper_face_data_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  જો (unlikely (!cg_font))
  {
    DEBUG_સંદેશ (CORETEXT, face, "CGFont creation failed..");
    પાછા nullptr;
  }

  પાછા (hb_coretext_face_data_t *) cg_font;
}

શૂન્ય
_hb_coretext_shaper_face_data_destroy (hb_coretext_face_data_t *data)
{
  જો (data)
    CFRelease ((CGFontRef) data);
}


hb_coretext_font_data_t *
_hb_coretext_shaper_font_data_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  અચલ hb_coretext_face_data_t *face_data = face->data.coretext;
  જો (unlikely (!face_data)) પાછા nullptr;
  CGFontRef cg_font = (CGFontRef) (અચલ શૂન્ય *) face->data.coretext;

  CGFloat font_size = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_SIZE);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_size));

  જો (unlikely (!ct_font))
  {
    DEBUG_સંદેશ (CORETEXT, font, "CGFont creation failed..");
    પાછા nullptr;
  }

  જો (font->અંક_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->અંક_coords,
				 &k_CFType_Dictionary_ચાવી_Call_Backs,
				 &k_CFType_Dictionary_કિંમત_Call_Backs));

    unsigned ગણતરી = font->અંક_coords;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_ot_var_axis_info_t info;
      unsigned પૂર્ણાંક c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      દશાંશ v = hb_clamp (font->design_coords[i], info.min_કિંમત, info.max_કિંમત);

      hb_cf_ptr_t<CFNumberRef> tag_સંખ્યા (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.tag));
      hb_cf_ptr_t<CFNumberRef> કિંમત_સંખ્યા (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      જો (variations && tag_સંખ્યા && કિંમત_સંખ્યા)
	CFDictionary_Set_કિંમત (variations, tag_સંખ્યા, કિંમત_સંખ્યા);
    }

    અચલ શૂન્ય *ચાવીઓ[] = { kCTFontVariationAttribute };
    અચલ શૂન્ય *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> attributes (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       ચાવીઓ,
				       values,
				       1,
				       &k_CFType_Dictionary_ચાવી_Call_Backs,
				       &k_CFType_Dictionary_કિંમત_Call_Backs) : nullptr);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (attributes ? CTFontDescriptorCreateWithAttributes (attributes) : nullptr);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFontCreateCopyWithAttributes (ct_font, 0, nullptr, varDesc) : nullptr);
    જો (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  પાછા (hb_coretext_font_data_t *) ct_font.release ();
}

શૂન્ય
_hb_coretext_shaper_font_data_destroy (hb_coretext_font_data_t *data)
{
  જો (data)
    CFRelease ((CTFontRef) data);
}

/*
 * shaper
 */

સંરચના feature_record_t {
  unsigned પૂર્ણાંક feature;
  unsigned પૂર્ણાંક setting;
};

સંરચના active_feature_t {
  feature_record_t rec;
  unsigned પૂર્ણાંક order;

  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb) {
    અચલ active_feature_t *a = (અચલ active_feature_t *) pa;
    અચલ active_feature_t *b = (અચલ active_feature_t *) pb;
    પાછા a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->order < b->order ? -1 : a->order > b->order ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  તાર્કિક operator== (અચલ active_feature_t& f) અચલ {
    પાછા cmp (આ, &f) == 0;
  }
};

સંરચના feature_event_t {
  unsigned પૂર્ણાંક index;
  તાર્કિક start;
  active_feature_t feature;

  HB_INTERNAL સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb) {
    અચલ feature_event_t *a = (અચલ feature_event_t *) pa;
    અચલ feature_event_t *b = (અચલ feature_event_t *) pb;
    પાછા a->index < b->index ? -1 : a->index > b->index ? 1 :
	   a->start < b->start ? -1 : a->start > b->start ? 1 :
	   active_feature_t::cmp (&a->feature, &b->feature);
  }
};

સંરચના range_record_t {
  CTFontRef font;
  unsigned પૂર્ણાંક index_first; /* == start */
  unsigned પૂર્ણાંક index_last;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_buffer_t        *buffer,
		    અચલ hb_feature_t *features,
		    unsigned પૂર્ણાંક        અંક_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (અચલ શૂન્ય *) face->data.coretext;
  CTFontRef ct_font = (CTFontRef) (અચલ શૂન્ય *) font->data.coretext;

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
  જો (HB_BUFFER_CLUSTER_LEVEL_IS_GRAPHEMES (buffer->cluster_level))
  {
    hb_unicode_funcs_t *unicode = buffer->unicode;
    unsigned પૂર્ણાંક ગણતરી = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
      જો (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	buffer->merge_grapheme_clusters (i - 1, i + 1);
  }

  hb_vector_t<range_record_t> range_records;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  જો (અંક_features)
  {
    /* Sort features by start/end events. */
    hb_vector_t<feature_event_t> feature_events;
    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_features; i++)
    {
      active_feature_t feature;

#જો MAC_OS_X_VERSION_MIN_REQUIRED < 101000
      અચલ hb_aat_feature_mapping_t * mapping = hb_aat_layout_find_feature_mapping (features[i].tag);
      જો (!mapping)
	ચાલુ;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selectorToEnable : mapping->selectorToDisable;
#નહીંતર
      feature.rec.feature = features[i].tag;
      feature.rec.setting = features[i].value;
#endif
      feature.order = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->index = features[i].start;
      event->start = સાચું;
      event->feature = feature;

      event = feature_events.push ();
      event->index = features[i].end;
      event->start = ખોટું;
      event->feature = feature;
    }
    feature_events.qsort ();
    /* Add a strategic final event. */
    {
      active_feature_t feature;
      feature.rec.feature = HB_TAG_NONE;
      feature.rec.setting = 0;
      feature.order = અંક_features + 1;

      feature_event_t *event = feature_events.push ();
      event->index = 0; /* This value does magic. */
      event->start = ખોટું;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_vector_t<active_feature_t> active_features;
    unsigned પૂર્ણાંક last_index = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < feature_events.length; i++)
    {
      feature_event_t *event = &feature_events[i];

      જો (event->index != last_index)
      {
	/* Save a snapshot of active features and the range. */
	range_record_t *range = range_records.push ();

	જો (active_features.length)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  માટે (unsigned પૂર્ણાંક j = 0; j < active_features.length; j++)
	  {
#જો MAC_OS_X_VERSION_MIN_REQUIRED < 101000
	    CFStringRef ચાવીઓ[] = {
	      k_CTFont_Feature_Type_Identifier_ચાવી,
	      k_CTFont_Feature_Selector_Identifier_ચાવી
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#નહીંતર
	    અક્ષર tag[5] = {HB_UNTAG (active_features[j].rec.feature)};
	    CFTypeRef ચાવીઓ[] = {
	      kCTFontOpenTypeFeatureTag,
	      k_CTFont_Open_Type_Feature_કિંમત
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, tag, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &active_features[j].rec.setting)
	    };
#endif
	    static_assert ((ARRAY_LENGTH_CONST (ચાવીઓ) == ARRAY_LENGTH_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (અચલ શૂન્ય **) ચાવીઓ,
						       (અચલ શૂન્ય **) values,
						       ARRAY_LENGTH (ચાવીઓ),
						       &k_CFType_Dictionary_ચાવી_Call_Backs,
						       &k_CFType_Dictionary_કિંમત_Call_Backs);
	    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_LENGTH (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_કિંમત (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef attributes = CFDictionaryCreate (kCFAllocatorDefault,
							   (અચલ શૂન્ય **) &kCTFontFeatureSettingsAttribute,
							   (અચલ શૂન્ય **) &features_array,
							   1,
							   &k_CFType_Dictionary_ચાવી_Call_Backs,
							   &k_CFType_Dictionary_કિંમત_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFontDescriptorCreateWithAttributes (attributes);
	  CFRelease (attributes);

	  range->font = CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, font_desc);
	  CFRelease (font_desc);
	}
	નહીંતર
	{
	  range->font = nullptr;
	}

	range->index_first = last_index;
	range->index_last  = event->index - 1;

	last_index = event->index;
      }

      જો (event->start)
      {
	active_features.push (event->feature);
      } નહીંતર {
	active_feature_t *feature = active_features.lsearch (event->feature);
	જો (feature)
	  active_features.remove_ordered (feature - active_features.arrayZ);
      }
    }
  }

  unsigned પૂર્ણાંક scratch_size;
  hb_buffer_t::scratch_buffer_t *scratch = buffer->get_scratch_buffer (&scratch_size);

#define ALLOCATE_ARRAY(Type, name, len, on_no_room) \
  Type *name = (Type *) scratch; \
  કરો { \
    unsigned પૂર્ણાંક _consumed = DIV_CEIL ((len) * કદ (Type), કદ (*scratch)); \
    જો (unlikely (_consumed > scratch_size)) \
    { \
      on_no_room; \
      પ્રતિજ્ઞા (0); \
    } \
    scratch += _consumed; \
    scratch_size -= _consumed; \
  } જ્યારે (0)

  ALLOCATE_ARRAY (UniChar, pchars, buffer->len * 2, ((શૂન્ય)nullptr) /*nothing*/);
  unsigned પૂર્ણાંક chars_len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++) {
    hb_codepoint_t c = buffer->info[i].codepoint;
    જો (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    નહીંતર જો (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    નહીંતર {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (unsigned પૂર્ણાંક, log_clusters, chars_len, ((શૂન્ય)nullptr) /*nothing*/);
  chars_len = 0;
  માટે (unsigned પૂર્ણાંક i = 0; i < buffer->len; i++)
  {
    hb_codepoint_t c = buffer->info[i].codepoint;
    unsigned પૂર્ણાંક cluster = buffer->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    જો (hb_in_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#define FAIL(...) \
  HB_STMT_START { \
    DEBUG_સંદેશ (CORETEXT, nullptr, __VA_ARGS__); \
    ret = ખોટું; \
    જાઓ fail; \
  } HB_STMT_END

  તાર્કિક ret = સાચું;
  CFStringRef string_ref = nullptr;
  CTLineRef લીટી = nullptr;

  જો (ખોટું)
  {
resize_and_retry:
    DEBUG_સંદેશ (CORETEXT, buffer, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    પ્રતિજ્ઞા (string_ref);
    પ્રતિજ્ઞા (લીટી);
    CFRelease (string_ref);
    CFRelease (લીટી);
    string_ref = nullptr;
    લીટી = nullptr;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    unsigned પૂર્ણાંક old_scratch_used;
    hb_buffer_t::scratch_buffer_t *old_scratch;
    old_scratch = buffer->get_scratch_buffer (&old_scratch_used);
    old_scratch_used = scratch - old_scratch;

    જો (unlikely (!buffer->ensure (buffer->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = buffer->get_scratch_buffer (&scratch_size);
    pchars = reinterpret_cast<UniChar *> (((અક્ષર *) scratch + ((અક્ષર *) pchars - (અક્ષર *) old_scratch)));
    log_clusters = reinterpret_cast<unsigned પૂર્ણાંક *> (((અક્ષર *) scratch + ((અક્ષર *) log_clusters - (અક્ષર *) old_scratch)));
    scratch += old_scratch_used;
    scratch_size -= old_scratch_used;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (nullptr,
						     pchars, chars_len,
						     kCFAllocatorNull);
    જો (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      જો (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      જો (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
      {
	CFAttributedStringSetAttribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      જો (buffer->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#જો !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#endif
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_to_string (buffer->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	જો (unlikely (!lang))
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

      જો (અંક_features && range_records.length)
      {
	unsigned પૂર્ણાંક start = 0;
	range_record_t *last_range = &range_records[0];
	માટે (unsigned પૂર્ણાંક k = 0; k < chars_len; k++)
	{
	  range_record_t *range = last_range;
	  જ્યારે (log_clusters[k] < range->index_first)
	    range--;
	  જ્યારે (log_clusters[k] > range->index_last)
	    range++;
	  જો (range != last_range)
	  {
	    જો (last_range->font)
	      CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, k - start),
					      kCTFontAttributeName, last_range->font);

	    start = k;
	  }

	  last_range = range;
	}
	જો (start != chars_len && last_range->font)
	  CFAttributedStringSetAttribute (attr_string, CFRangeMake (start, chars_len - start),
					  kCTFontAttributeName, last_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      જો (અંક_features)
      {
	unsigned પૂર્ણાંક zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	માટે (unsigned પૂર્ણાંક i = 0; i < અંક_features; i++)
	{
	  અચલ hb_feature_t &feature = features[i];
	  જો (feature.tag == HB_TAG('k','e','r','n') &&
	      feature.start < chars_len && feature.start < feature.end)
	  {
	    CFRange feature_range = CFRangeMake (feature.start,
						 hb_min (feature.end, chars_len) - feature.start);
	    જો (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    નહીંતર
	      CFAttributedStringSetAttribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      પૂર્ણાંક level = HB_DIRECTION_IS_FORWARD (buffer->props.direction) ? 0 : 1;
      CFNumberRef level_સંખ્યા = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#જો !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
      બાહ્ય અચલ CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#endif
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (અચલ શૂન્ય **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (અચલ શૂન્ય **) &level_સંખ્યા,
						    1,
						    &k_CFType_Dictionary_ચાવી_Call_Backs,
						    &k_CFType_Dictionary_કિંમત_Call_Backs);
      CFRelease (level_સંખ્યા);
      જો (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      જો (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      લીટી = CTTypesetter_Create_લીટી (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      જો (unlikely (!લીટી))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (લીટી);
    unsigned પૂર્ણાંક અંક_runs = CFArray_Get_ગણતરી (glyph_runs);
    DEBUG_સંદેશ (CORETEXT, nullptr, "Num runs: %u", અંક_runs);

    buffer->len = 0;
    uint32_t status_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    જો (HB_DIRECTION_IS_BACKWARD (buffer->props.direction))
    {
      advances_so_far -= CTLineGetTrailingWhitespaceWidth (લીટી);
      જો (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
	  advances_so_far = -advances_so_far;
    }

    અચલ CFRange range_all = CFRangeMake (0, 0);

    માટે (unsigned પૂર્ણાંક i = 0; i < અંક_runs; i++)
    {
      CTRunRef run = static_cast<CTRunRef>(CFArray_Get_કિંમત_At_Index (glyph_runs, i));
      CTRunStatus run_status = CTRunGetStatus (run);
      status_or  |= run_status;
      DEBUG_સંદેશ (CORETEXT, run, "CTRunStatus: %x", run_status);
      CGFloat run_advance = CTRunGetTypographicBounds (run, range_all, nullptr, nullptr, nullptr);
      જો (HB_DIRECTION_IS_VERTICAL (buffer->props.direction))
	  run_advance = -run_advance;
      DEBUG_સંદેશ (CORETEXT, run, "Run advance: %g", (દ્વિગુણ) run_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef attributes = CTRunGetAttributes (run);
      CTFontRef run_ct_font = static_cast<CTFontRef>(CFDictionary_Get_કિંમત (attributes, kCTFontAttributeName));
      જો (!CFEqual (run_ct_font, ct_font))
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
	તાર્કિક matched = ખોટું;
	માટે (unsigned પૂર્ણાંક i = 0; i < range_records.length; i++)
	  જો (range_records[i].font && CFEqual (run_ct_font, range_records[i].font))
	  {
	    matched = સાચું;
	    તોડો;
	  }
	જો (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> run_cg_font (CTFontCopyGraphicsFont (run_ct_font, nullptr));
	  જો (run_cg_font)
	    matched = CFEqual (run_cg_font, cg_font);
	}
	જો (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_ચાવી));
	  hb_cf_ptr_t<CFStringRef> run_ps_name (CTFontCopyName (run_ct_font, k_CTFont_Post_Script_Name_ચાવી));
	  જો (font_ps_name && run_ps_name)
	  {
	    જો (CFStringCompare (run_ps_name, font_ps_name, 0) == kCFCompareEqualTo)
	      matched = સાચું;
	  }
	}
	જો (!matched)
	{
	  CFRange range = CTRunGetStringRange (run);
	  DEBUG_સંદેશ (CORETEXT, run, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.length);
	  જો (!buffer->ensure_inplace (buffer->len + range.length))
	    જાઓ resize_and_retry;
	  hb_glyph_info_t *info = buffer->info + buffer->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = buffer->props.direction;
	  hb_position_t x_advance, y_advance, x_offset, y_offset;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_offset, &y_offset);
	  hb_position_t advance = hb_saturate_add (x_advance, y_advance);
	  x_offset = hb_saturate_neg (x_offset);
	  y_offset = hb_saturate_neg (y_offset);

	  unsigned પૂર્ણાંક old_len = buffer->len;
	  માટે (CFIndex j = range.location; j < range.location + range.length; j++)
	  {
	      UniChar ch = CFStringGetCharacterAtIndex (string_ref, j);
	      જો (hb_in_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFStringGetCharacterAtIndex (string_ref, j - 1);
		જો (hb_in_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  ચાલુ;
	      }
	      જો (buffer->unicode->is_default_ignorable (ch))
		ચાલુ;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->mask = advance;
	      info->var1.i32 = x_offset;
	      info->var2.i32 = y_offset;

	      info++;
	      buffer->len++;
	  }
	  જો (HB_DIRECTION_IS_BACKWARD (buffer->props.direction))
	    buffer->reverse_range (old_len, buffer->len);
	  advances_so_far += run_advance;
	  ચાલુ;
	}
      }

      unsigned પૂર્ણાંક અંક_glyphs = CTRun_Get_Glyph_ગણતરી (run);
      જો (અંક_glyphs == 0)
	ચાલુ;

      જો (!buffer->ensure_inplace (buffer->len + અંક_glyphs))
	જાઓ resize_and_retry;

      hb_glyph_info_t *run_info = buffer->info + buffer->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#define USE_PTR સાચું

#define SCRATCH_SAVE() \
  unsigned પૂર્ણાંક scratch_size_saved = scratch_size; \
  hb_buffer_t::scratch_buffer_t *scratch_saved = scratch

#define SCRATCH_RESTORE() \
  scratch_size = scratch_size_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	અચલ CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (run) : nullptr;
	જો (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, અંક_glyphs, જાઓ resize_and_retry);
	  CTRunGetGlyphs (run, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	અચલ CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (run) : nullptr;
	જો (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, index_buf, અંક_glyphs, જાઓ resize_and_retry);
	  CTRunGetStringIndices (run, range_all, index_buf);
	  string_indices = index_buf;
	}
	hb_glyph_info_t *info = run_info;
	માટે (unsigned પૂર્ણાંક j = 0; j < અંક_glyphs; j++)
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
	અચલ CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (run) : nullptr;
	જો (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, અંક_glyphs, જાઓ resize_and_retry);
	  CTRunGetPositions (run, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = run_info;
	જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
	{
	  hb_position_t x_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  માટે (unsigned પૂર્ણાંક j = 0; j < અંક_glyphs; j++)
	  {
	    CGFloat advance;
	    જો (likely (j + 1 < અંક_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    નહીંતર /* last glyph */
	      advance = run_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->mask = (hb_mask_t) hb_clamp_to<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_offset;
	    info->var2.i32 = hb_clamp_to<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	નહીંતર
	{
	  hb_position_t y_offset = hb_clamp_to<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  માટે (unsigned પૂર્ણાંક j = 0; j < અંક_glyphs; j++)
	  {
	    CGFloat advance;
	    જો (likely (j + 1 < અંક_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    નહીંતર /* last glyph */
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

      buffer->len += અંક_glyphs;
    }

    buffer->clear_positions ();

    unsigned પૂર્ણાંક ગણતરી = buffer->len;
    hb_glyph_info_t *info = buffer->info;
    hb_glyph_position_t *pos = buffer->pos;
    જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
      માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      {
	pos->x_advance = info->mask;
	pos->x_offset = info->var1.i32;
	pos->y_offset = info->var2.i32;

	info++; pos++;
      }
    નહીંતર
      માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
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
    જો (ગણતરી > 1 && (status_or & kCTRunStatusNonMonotonic) &&
	HB_BUFFER_CLUSTER_LEVEL_IS_MONOTONE (buffer->cluster_level))
    {
      hb_glyph_info_t *info = buffer->info;
      જો (HB_DIRECTION_IS_FORWARD (buffer->props.direction))
      {
	unsigned પૂર્ણાંક cluster = info[ગણતરી - 1].cluster;
	માટે (unsigned પૂર્ણાંક i = ગણતરી - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      નહીંતર
      {
	unsigned પૂર્ણાંક cluster = info[0].cluster;
	માટે (unsigned પૂર્ણાંક i = 1; i < ગણતરી; i++)
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
  જો (string_ref)
    CFRelease (string_ref);
  જો (લીટી)
    CFRelease (લીટી);

  માટે (unsigned પૂર્ણાંક i = 0; i < range_records.length; i++)
    જો (range_records[i].font)
      CFRelease (range_records[i].font);

  પાછા ret;
}


#endif
