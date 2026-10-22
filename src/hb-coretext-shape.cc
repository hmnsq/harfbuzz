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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CORETEXT

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-impl.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-coretext.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"

hb_coretext_face_ਅੰਕੜੇ_t *
hb_coretext_shaper_face_ਅੰਕੜੇ_create (hb_face_t *face)
{
  CGFontRef cg_font = create_cg_font (face);

  ਜੇ (unlikely (!cg_font))
  {
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, face, "CGFont creation failed..");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ (hb_coretext_face_ਅੰਕੜੇ_t *) cg_font;
}

ਖਾਲੀ
hb_coretext_shaper_face_ਅੰਕੜੇ_destroy (hb_coretext_face_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ)
    CFRelease ((CGFontRef) ਅੰਕੜੇ);
}


hb_coretext_font_ਅੰਕੜੇ_t *
hb_coretext_shaper_font_ਅੰਕੜੇ_create (hb_font_t *font)
{
  hb_face_t *face = font->face;
  ਅਚਲ hb_coretext_face_ਅੰਕੜੇ_t *face_ਅੰਕੜੇ = face->ਅੰਕੜੇ.coretext;
  ਜੇ (unlikely (!face_ਅੰਕੜੇ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  CGFontRef cg_font = (CGFontRef) (ਅਚਲ ਖਾਲੀ *) face->ਅੰਕੜੇ.coretext;

  CGFloat font_ਮਾਪ = (CGFloat) (font->ptem > 0.f ? font->ptem : HB_CORETEXT_DEFAULT_FONT_ਮਾਪ);
  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, font_ਮਾਪ));

  ਜੇ (unlikely (!ct_font))
  {
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, font, "CGFont creation failed..");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (font->ਅੰਕ_coords)
  {
    hb_cf_ptr_t<CFMutableDictionaryRef> variations (
      CFDictionaryCreateMutable (kCFAllocatorDefault,
				 font->ਅੰਕ_coords,
				 &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
				 &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs));

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = font->ਅੰਕ_coords;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      hb_ot_var_axis_info_t info;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c = 1;
      hb_ot_var_get_axis_infos (font->face, i, &c, &info);

      ਦਸ਼ਮਲਵ v = hb_clamp (font->design_coords[i], info.min_ਮੁੱਲ, info.max_ਮੁੱਲ);

      hb_cf_ptr_t<CFNumberRef> ਟੈਗ_ਸੰਖਿਆ (CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &info.ਟੈਗ_2));
      hb_cf_ptr_t<CFNumberRef> ਮੁੱਲ_ਸੰਖਿਆ (CFNumberCreate (kCFAllocatorDefault, kCFNumberFloatType, &v));
      ਜੇ (variations && ਟੈਗ_ਸੰਖਿਆ && ਮੁੱਲ_ਸੰਖਿਆ)
	CFDictionary_ਸੈੱਟ_ਮੁੱਲ (variations, ਟੈਗ_ਸੰਖਿਆ, ਮੁੱਲ_ਸੰਖਿਆ);
    }

    ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀਆਂ[] = { kCTFontVariationAttribute };
    ਅਚਲ ਖਾਲੀ *values[] = { variations.get () };
    hb_cf_ptr_t<CFDictionaryRef> ਗੁਣ (
      variations ? CFDictionaryCreate (kCFAllocatorDefault,
				       ਕੁੰਜੀਆਂ,
				       values,
				       1,
				       &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
				       &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs) : ਖਾਲੀ_ਸੂਚਕ);

    hb_cf_ptr_t<CTFontDescriptorRef> varDesc (ਗੁਣ ? CTFont_Descriptor_Create_With_ਗੁਣ (ਗੁਣ) : ਖਾਲੀ_ਸੂਚਕ);
    hb_cf_ptr_t<CTFontRef> new_ct_font (varDesc ? CTFont_Create_Copy_With_ਗੁਣ (ct_font, 0, ਖਾਲੀ_ਸੂਚਕ, varDesc) : ਖਾਲੀ_ਸੂਚਕ);
    ਜੇ (new_ct_font)
      ct_font = std::move (new_ct_font);
  }

  ਵਾਪਸ (hb_coretext_font_ਅੰਕੜੇ_t *) ct_font.release ();
}

ਖਾਲੀ
hb_coretext_shaper_font_ਅੰਕੜੇ_destroy (hb_coretext_font_ਅੰਕੜੇ_t *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ)
    CFRelease ((CTFontRef) ਅੰਕੜੇ);
}

/*
 * shaper
 */

ਬਣਤਰ feature_ਰਿਕਾਰਡ_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ feature;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ setting;
};

ਬਣਤਰ ਸਰਗਰਮ_feature_t {
  feature_ਰਿਕਾਰਡ_t rec;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕ੍ਰਮ;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb) {
    ਅਚਲ ਸਰਗਰਮ_feature_t *a = (ਅਚਲ ਸਰਗਰਮ_feature_t *) pa;
    ਅਚਲ ਸਰਗਰਮ_feature_t *b = (ਅਚਲ ਸਰਗਰਮ_feature_t *) pb;
    ਵਾਪਸ a->rec.feature < b->rec.feature ? -1 : a->rec.feature > b->rec.feature ? 1 :
	   a->ਕ੍ਰਮ < b->ਕ੍ਰਮ ? -1 : a->ਕ੍ਰਮ > b->ਕ੍ਰਮ ? 1 :
	   a->rec.setting < b->rec.setting ? -1 : a->rec.setting > b->rec.setting ? 1 :
	   0;
  }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ== (ਅਚਲ ਸਰਗਰਮ_feature_t& f) ਅਚਲ {
    ਵਾਪਸ cmp (ਇਹ, &f) == 0;
  }
};

ਬਣਤਰ feature_event_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
  ਤਾਰਕਿਕ ਸ਼ੁਰੂ;
  ਸਰਗਰਮ_feature_t feature;

  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb) {
    ਅਚਲ feature_event_t *a = (ਅਚਲ feature_event_t *) pa;
    ਅਚਲ feature_event_t *b = (ਅਚਲ feature_event_t *) pb;
    ਵਾਪਸ a->ਸੂਚਕਾਂਕ_2 < b->ਸੂਚਕਾਂਕ_2 ? -1 : a->ਸੂਚਕਾਂਕ_2 > b->ਸੂਚਕਾਂਕ_2 ? 1 :
	   a->ਸ਼ੁਰੂ < b->ਸ਼ੁਰੂ ? -1 : a->ਸ਼ੁਰੂ > b->ਸ਼ੁਰੂ ? 1 :
	   ਸਰਗਰਮ_feature_t::cmp (&a->feature, &b->feature);
  }
};

ਬਣਤਰ range_ਰਿਕਾਰਡ_t {
  CTFontRef font;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_ਪਹਿਲਾ; /* == start */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_ਆਖਰੀ;  /* == end - 1 */
};


hb_bool_t
_hb_coretext_shape (hb_shape_plan_t    *shape_plan,
		    hb_font_t          *font,
		    hb_ਬਫਰ_t        *ਬਫਰ,
		    ਅਚਲ hb_feature_t *features,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ        ਅੰਕ_features)
{
  hb_face_t *face = font->face;
  CGFontRef cg_font = (CGFontRef) (ਅਚਲ ਖਾਲੀ *) face->ਅੰਕੜੇ.coretext;
  CTFontRef ct_font = (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext;

  CGFloat ct_font_ਮਾਪ = CTFont_Get_ਮਾਪ (ct_font);
  CGFloat x_mult = (CGFloat) font->x_scale / ct_font_ਮਾਪ;
  CGFloat y_mult = (CGFloat) font->y_scale / ct_font_ਮਾਪ;

  /* Attach marks to their bases, to match the 'ot' shaper.
   * Adapted from a very old version of hb-ot-shape:hb_form_clusters().
   * Note that this only makes us be closer to the 'ot' shaper,
   * but by no means the same.  For example, if there's
   * B1 M1 B2 M2, and B1-B2 form a ligature, M2's cluster will
   * continue pointing to B2 even though B2 was merged into B1's
   * cluster... */
  ਜੇ (HB_ਬਫਰ_CLUSTER_LEVEL_IS_GRAPHEMES (ਬਫਰ->cluster_level))
  {
    hb_unicode_funcs_t *unicode = ਬਫਰ->unicode;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
      ਜੇ (HB_UNICODE_GENERAL_CATEGORY_IS_MARK (unicode->general_category (info[i].codepoint)))
	ਬਫਰ->ਮਿਲਾਓ_grapheme_clusters (i - 1, i + 1);
  }

  hb_ਸਦਿਸ਼_t<range_ਰਿਕਾਰਡ_t> range_ਰਿਕਾਰਡਾਂ;

  /*
   * Set up features.
   * (copied + modified from code from hb-uniscribe.cc)
   */
  ਜੇ (ਅੰਕ_features)
  {
    /* Sort features by start/end events. */
    hb_ਸਦਿਸ਼_t<feature_event_t> feature_events;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
    {
      ਸਰਗਰਮ_feature_t feature;

#ਜੇ MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 101000
      ਅਚਲ hb_aat_feature_mapping_t * mapping = hb_aat_layout_ਲੱਭੋ_feature_mapping (features[i].ਟੈਗ_2);
      ਜੇ (!mapping)
	ਜਾਰੀ;

      feature.rec.feature = mapping->aatFeatureType;
      feature.rec.setting = features[i].value ? mapping->selector_ਮੰਜ਼ਿਲ_Enable : mapping->selector_ਮੰਜ਼ਿਲ_Disable;
#ਨਹੀਂਤਾਂ
      feature.rec.feature = features[i].ਟੈਗ_2;
      feature.rec.setting = features[i].value;
#ਜੇ_ਅੰਤ
      feature.ਕ੍ਰਮ = i;

      feature_event_t *event;

      event = feature_events.push ();
      event->ਸੂਚਕਾਂਕ_2 = features[i].ਸ਼ੁਰੂ;
      event->ਸ਼ੁਰੂ = ਸੱਚ;
      event->feature = feature;

      event = feature_events.push ();
      event->ਸੂਚਕਾਂਕ_2 = features[i].ਸਿਰਾ;
      event->ਸ਼ੁਰੂ = ਝੂਠ;
      event->feature = feature;
    }
    feature_events.qsort ();
    /* Add a strategic final event. */
    {
      ਸਰਗਰਮ_feature_t feature;
      feature.rec.feature = HB_ਟੈਗ_NONE;
      feature.rec.setting = 0;
      feature.ਕ੍ਰਮ = ਅੰਕ_features + 1;

      feature_event_t *event = feature_events.push ();
      event->ਸੂਚਕਾਂਕ_2 = 0; /* This value does magic. */
      event->ਸ਼ੁਰੂ = ਝੂਠ;
      event->feature = feature;
    }

    /* Scan events and save features for each range. */
    hb_ਸਦਿਸ਼_t<ਸਰਗਰਮ_feature_t> ਸਰਗਰਮ_features;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਸੂਚਕਾਂਕ = 0;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < feature_events.ਲੰਮਾਈ; i++)
    {
      feature_event_t *event = &feature_events[i];

      ਜੇ (event->ਸੂਚਕਾਂਕ_2 != ਆਖਰੀ_ਸੂਚਕਾਂਕ)
      {
	/* Save a snapshot of active features and the range. */
	range_ਰਿਕਾਰਡ_t *range = range_ਰਿਕਾਰਡਾਂ.push ();

	ਜੇ (ਸਰਗਰਮ_features.ਲੰਮਾਈ)
	{
	  CFMutableArrayRef features_array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);

	  /* TODO sort and resolve conflicting features? */
	  /* active_features.qsort (); */
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਸਰਗਰਮ_features.ਲੰਮਾਈ; j++)
	  {
#ਜੇ MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 101000
	    CFStringRef ਕੁੰਜੀਆਂ[] = {
	      k_CTFont_Feature_Type_Identifier_ਕੁੰਜੀ,
	      k_CTFont_Feature_Selector_Identifier_ਕੁੰਜੀ
	    };
	    CFNumberRef values[] = {
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &ਸਰਗਰਮ_features[j].rec.feature),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &ਸਰਗਰਮ_features[j].rec.setting)
	    };
#ਨਹੀਂਤਾਂ
	    ਅੱਖਰ ਟੈਗ_2[5] = {HB_UNTAG (ਸਰਗਰਮ_features[j].rec.feature)};
	    CFTypeRef ਕੁੰਜੀਆਂ[] = {
	      k_CTFont_Open_Type_Feature_ਟੈਗ,
	      k_CTFont_Open_Type_Feature_ਮੁੱਲ
	    };
	    CFTypeRef values[] = {
	      CFStringCreateWithCString (kCFAllocatorDefault, ਟੈਗ_2, kCFStringEncodingASCII),
	      CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &ਸਰਗਰਮ_features[j].rec.setting)
	    };
#ਜੇ_ਅੰਤ
	    static_assert ((ARRAY_ਲੰਮਾਈ_CONST (ਕੁੰਜੀਆਂ) == ARRAY_ਲੰਮਾਈ_CONST (values)), "");
	    CFDictionaryRef dict = CFDictionaryCreate (kCFAllocatorDefault,
						       (ਅਚਲ ਖਾਲੀ **) ਕੁੰਜੀਆਂ,
						       (ਅਚਲ ਖਾਲੀ **) values,
						       ARRAY_ਲੰਮਾਈ (ਕੁੰਜੀਆਂ),
						       &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
						       &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs);
	    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (values); i++)
	      CFRelease (values[i]);

	    CFArray_Append_ਮੁੱਲ (features_array, dict);
	    CFRelease (dict);

	  }

	  CFDictionaryRef ਗੁਣ = CFDictionaryCreate (kCFAllocatorDefault,
							   (ਅਚਲ ਖਾਲੀ **) &kCTFontFeatureSettingsAttribute,
							   (ਅਚਲ ਖਾਲੀ **) &features_array,
							   1,
							   &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
							   &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs);
	  CFRelease (features_array);

	  CTFontDescriptorRef font_desc = CTFont_Descriptor_Create_With_ਗੁਣ (ਗੁਣ);
	  CFRelease (ਗੁਣ);

	  range->font = CTFont_Create_Copy_With_ਗੁਣ (ct_font, 0.0, ਖਾਲੀ_ਸੂਚਕ, font_desc);
	  CFRelease (font_desc);
	}
	ਨਹੀਂਤਾਂ
	{
	  range->font = ਖਾਲੀ_ਸੂਚਕ;
	}

	range->ਸੂਚਕਾਂਕ_ਪਹਿਲਾ = ਆਖਰੀ_ਸੂਚਕਾਂਕ;
	range->ਸੂਚਕਾਂਕ_ਆਖਰੀ  = event->ਸੂਚਕਾਂਕ_2 - 1;

	ਆਖਰੀ_ਸੂਚਕਾਂਕ = event->ਸੂਚਕਾਂਕ_2;
      }

      ਜੇ (event->ਸ਼ੁਰੂ)
      {
	ਸਰਗਰਮ_features.push (event->feature);
      } ਨਹੀਂਤਾਂ {
	ਸਰਗਰਮ_feature_t *feature = ਸਰਗਰਮ_features.lsearch (event->feature);
	ਜੇ (feature)
	  ਸਰਗਰਮ_features.remove_ordered (feature - ਸਰਗਰਮ_features.arrayZ);
      }
    }
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scratch_ਮਾਪ;
  hb_ਬਫਰ_t::scratch_ਬਫਰ_t *scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALLOCATE_ARRAY(Type, name, len, on_no_ਗੁੰਜਾਇਸ਼) \
  Type *name = (Type *) scratch; \
  ਕਰੋ { \
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ _consumed = DIV_CEIL ((len) * ਆਕਾਰ (Type), ਆਕਾਰ (*scratch)); \
    ਜੇ (unlikely (_consumed > scratch_ਮਾਪ)) \
    { \
      on_no_ਗੁੰਜਾਇਸ਼; \
      ਪ੍ਰਤਿਜ੍ਞਾ (0); \
    } \
    scratch += _consumed; \
    scratch_ਮਾਪ -= _consumed; \
  } ਜਦੋਂ (0)

  ALLOCATE_ARRAY (UniChar, pchars, ਬਫਰ->len * 2, ((ਖਾਲੀ)ਖਾਲੀ_ਸੂਚਕ) /*nothing*/);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ chars_len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++) {
    hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
    ਜੇ (likely (c <= 0xFFFFu))
      pchars[chars_len++] = c;
    ਨਹੀਂਤਾਂ ਜੇ (unlikely (c > 0x10FFFFu))
      pchars[chars_len++] = 0xFFFDu;
    ਨਹੀਂਤਾਂ {
      pchars[chars_len++] = 0xD800u + ((c - 0x10000u) >> 10);
      pchars[chars_len++] = 0xDC00u + ((c - 0x10000u) & ((1u << 10) - 1));
    }
  }

  ALLOCATE_ARRAY (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, log_clusters, chars_len, ((ਖਾਲੀ)ਖਾਲੀ_ਸੂਚਕ) /*nothing*/);
  chars_len = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਬਫਰ->len; i++)
  {
    hb_codepoint_t c = ਬਫਰ->info[i].codepoint;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = ਬਫਰ->info[i].cluster;
    log_clusters[chars_len++] = cluster;
    ਜੇ (hb_ਵਿੱਚ_range (c, 0x10000u, 0x10FFFFu))
      log_clusters[chars_len++] = cluster; /* Surrogates. */
  }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL(...) \
  HB_STMT_ਸ਼ੁਰੂ { \
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਖਾਲੀ_ਸੂਚਕ, __VA_ARGS__); \
    ret = ਝੂਠ; \
    ਜਾਓ fail; \
  } HB_STMT_ਸਿਰਾ

  ਤਾਰਕਿਕ ret = ਸੱਚ;
  CFStringRef string_ref = ਖਾਲੀ_ਸੂਚਕ;
  CTLineRef ਸਤਰ = ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (ਝੂਠ)
  {
resize_and_retry:
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਬਫਰ, "Buffer resize");
    /* string_ref uses the scratch-buffer for backing store, and line references
     * string_ref (via attr_string).  We must release those before resizing buffer. */
    ਪ੍ਰਤਿਜ੍ਞਾ (string_ref);
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸਤਰ);
    CFRelease (string_ref);
    CFRelease (ਸਤਰ);
    string_ref = ਖਾਲੀ_ਸੂਚਕ;
    ਸਤਰ = ਖਾਲੀ_ਸੂਚਕ;

    /* Get previous start-of-scratch-area, that we use later for readjusting
     * our existing scratch arrays. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_scratch_ਵਰਤਿਆ;
    hb_ਬਫਰ_t::scratch_ਬਫਰ_t *old_scratch;
    old_scratch = ਬਫਰ->get_scratch_ਬਫਰ (&old_scratch_ਵਰਤਿਆ);
    old_scratch_ਵਰਤਿਆ = scratch - old_scratch;

    ਜੇ (unlikely (!ਬਫਰ->ensure (ਬਫਰ->allocated * 2)))
      FAIL ("Buffer resize failed");

    /* Adjust scratch, pchars, and log_cluster arrays.  This is ugly, but really the
     * cleanest way to do without completely restructuring the rest of this shaper. */
    scratch = ਬਫਰ->get_scratch_ਬਫਰ (&scratch_ਮਾਪ);
    pchars = reinterpret_cast<UniChar *> (((ਅੱਖਰ *) scratch + ((ਅੱਖਰ *) pchars - (ਅੱਖਰ *) old_scratch)));
    log_clusters = reinterpret_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *> (((ਅੱਖਰ *) scratch + ((ਅੱਖਰ *) log_clusters - (ਅੱਖਰ *) old_scratch)));
    scratch += old_scratch_ਵਰਤਿਆ;
    scratch_ਮਾਪ -= old_scratch_ਵਰਤਿਆ;
  }
  {
    string_ref = CFStringCreateWithCharactersNoCopy (ਖਾਲੀ_ਸੂਚਕ,
						     pchars, chars_len,
						     kCFAllocatorNull);
    ਜੇ (unlikely (!string_ref))
      FAIL ("CFStringCreateWithCharactersNoCopy failed");

    /* Create an attributed string, populate it, and create a line from it, then release attributed string. */
    {
      CFMutableAttributedStringRef attr_string = CFAttributedStringCreateMutable (kCFAllocatorDefault,
										  chars_len);
      ਜੇ (unlikely (!attr_string))
	FAIL ("CFAttributedStringCreateMutable failed");
      CFAttributedStringReplaceString (attr_string, CFRangeMake (0, 0), string_ref);
      ਜੇ (HB_DIRECTION_IS_VERTICAL (ਬਫਰ->props.direction))
      {
	CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTVerticalFormsAttributeName, kCFBooleanTrue);
      }

      ਜੇ (ਬਫਰ->props.language)
      {
/* What's the iOS equivalent of this check?
 * The symbols was introduced in iOS 7.0.
 * At any rate, our fallback is safe and works fine. */
#ਜੇ !(defined(ਨਿਸ਼ਾਨਾ_OS_IPHONE) && ਨਿਸ਼ਾਨਾ_OS_IPHONE) && MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 1090
#  define kCTLanguageAttributeName CFSTR ("NSLanguage")
#ਜੇ_ਅੰਤ
	CFStringRef lang = CFStringCreateWithCStringNoCopy (kCFAllocatorDefault,
							    hb_language_ਮੰਜ਼ਿਲ_string (ਬਫਰ->props.language),
							    kCFStringEncodingUTF8,
							    kCFAllocatorNull);
	ਜੇ (unlikely (!lang))
	{
	  CFRelease (attr_string);
	  FAIL ("CFStringCreateWithCStringNoCopy failed");
	}
	CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, CFRangeMake (0, chars_len),
					kCTLanguageAttributeName, lang);
	CFRelease (lang);
      }
      CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, CFRangeMake (0, chars_len),
				      kCTFontAttributeName, ct_font);

      ਜੇ (ਅੰਕ_features && range_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = 0;
	range_ਰਿਕਾਰਡ_t *ਆਖਰੀ_range = &range_ਰਿਕਾਰਡਾਂ[0];
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = 0; k < chars_len; k++)
	{
	  range_ਰਿਕਾਰਡ_t *range = ਆਖਰੀ_range;
	  ਜਦੋਂ (log_clusters[k] < range->ਸੂਚਕਾਂਕ_ਪਹਿਲਾ)
	    range--;
	  ਜਦੋਂ (log_clusters[k] > range->ਸੂਚਕਾਂਕ_ਆਖਰੀ)
	    range++;
	  ਜੇ (range != ਆਖਰੀ_range)
	  {
	    ਜੇ (ਆਖਰੀ_range->font)
	      CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, CFRangeMake (ਸ਼ੁਰੂ, k - ਸ਼ੁਰੂ),
					      kCTFontAttributeName, ਆਖਰੀ_range->font);

	    ਸ਼ੁਰੂ = k;
	  }

	  ਆਖਰੀ_range = range;
	}
	ਜੇ (ਸ਼ੁਰੂ != chars_len && ਆਖਰੀ_range->font)
	  CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, CFRangeMake (ਸ਼ੁਰੂ, chars_len - ਸ਼ੁਰੂ),
					  kCTFontAttributeName, ਆਖਰੀ_range->font);
      }
      /* Enable/disable kern if requested.
       *
       * Note: once kern is disabled, reenabling it doesn't currently seem to work in CoreText.
       */
      ਜੇ (ਅੰਕ_features)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ zeroint = 0;
	CFNumberRef zero = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &zeroint);
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_features; i++)
	{
	  ਅਚਲ hb_feature_t &feature = features[i];
	  ਜੇ (feature.ਟੈਗ_2 == HB_ਟੈਗ('k','e','r','n') &&
	      feature.ਸ਼ੁਰੂ < chars_len && feature.ਸ਼ੁਰੂ < feature.ਸਿਰਾ)
	  {
	    CFRange feature_range = CFRangeMake (feature.ਸ਼ੁਰੂ,
						 hb_min (feature.ਸਿਰਾ, chars_len) - feature.ਸ਼ੁਰੂ);
	    ਜੇ (feature.value)
	      CFAttributedStringRemoveAttribute (attr_string, feature_range, kCTKernAttributeName);
	    ਨਹੀਂਤਾਂ
	      CFAttributed_String_ਸੈੱਟ_Attribute (attr_string, feature_range, kCTKernAttributeName, zero);
	  }
	}
	CFRelease (zero);
      }

      ਪੂਰਨ level = HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction) ? 0 : 1;
      CFNumberRef level_ਸੰਖਿਆ = CFNumberCreate (kCFAllocatorDefault, kCFNumberIntType, &level);
#ਜੇ !(defined(ਨਿਸ਼ਾਨਾ_OS_IPHONE) && ਨਿਸ਼ਾਨਾ_OS_IPHONE) && MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 1060
      ਬਾਹਰੀ ਅਚਲ CFStringRef kCTTypesetterOptionForcedEmbeddingLevel;
#ਜੇ_ਅੰਤ
      CFDictionaryRef options = CFDictionaryCreate (kCFAllocatorDefault,
						    (ਅਚਲ ਖਾਲੀ **) &kCTTypesetterOptionForcedEmbeddingLevel,
						    (ਅਚਲ ਖਾਲੀ **) &level_ਸੰਖਿਆ,
						    1,
						    &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
						    &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs);
      CFRelease (level_ਸੰਖਿਆ);
      ਜੇ (unlikely (!options))
      {
	CFRelease (attr_string);
	FAIL ("CFDictionaryCreate failed");
      }

      CTTypesetterRef typesetter = CTTypesetterCreateWithAttributedStringAndOptions (attr_string, options);
      CFRelease (options);
      CFRelease (attr_string);
      ਜੇ (unlikely (!typesetter))
	FAIL ("CTTypesetterCreateWithAttributedStringAndOptions failed");

      ਸਤਰ = CTTypesetter_Create_ਸਤਰ (typesetter, CFRangeMake(0, 0));
      CFRelease (typesetter);
      ਜੇ (unlikely (!ਸਤਰ))
	FAIL ("CTTypesetterCreateLine failed");
    }

    CFArrayRef glyph_runs = CTLineGetGlyphRuns (ਸਤਰ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_runs = CFArray_Get_ਗਿਣਤੀ (glyph_runs);
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਖਾਲੀ_ਸੂਚਕ, "Num runs: %u", ਅੰਕ_runs);

    ਬਫਰ->len = 0;
    uint32_t ਸਥਿਤੀ_or = 0;
    CGFloat advances_so_far = 0;
    /* For right-to-left runs, CoreText returns the glyphs positioned such that
     * any trailing whitespace is to the left of (0,0).  Adjust coordinate system
     * to fix for that.  Test with any RTL string with trailing spaces.
     * https://crbug.com/469028
     */
    ਜੇ (HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction))
    {
      advances_so_far -= CTLine_Get_Trailing_Whitespace_ਚੌੜਾਈ (ਸਤਰ);
      ਜੇ (HB_DIRECTION_IS_VERTICAL (ਬਫਰ->props.direction))
	  advances_so_far = -advances_so_far;
    }

    ਅਚਲ CFRange range_all = CFRangeMake (0, 0);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਅੰਕ_runs; i++)
    {
      CTRunRef ਲੜੀਵਾਰ = static_cast<CTRunRef>(CFArray_Get_ਮੁੱਲ_At_ਸੂਚਕਾਂਕ (glyph_runs, i));
      CTRun_ਸਥਿਤੀ ਲੜੀਵਾਰ_ਸਥਿਤੀ = CTRun_Get_ਸਥਿਤੀ (ਲੜੀਵਾਰ);
      ਸਥਿਤੀ_or  |= ਲੜੀਵਾਰ_ਸਥਿਤੀ;
      DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਲੜੀਵਾਰ, "CTRunStatus: %x", ਲੜੀਵਾਰ_ਸਥਿਤੀ);
      CGFloat ਲੜੀਵਾਰ_advance = CTRunGetTypographicBounds (ਲੜੀਵਾਰ, range_all, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
      ਜੇ (HB_DIRECTION_IS_VERTICAL (ਬਫਰ->props.direction))
	  ਲੜੀਵਾਰ_advance = -ਲੜੀਵਾਰ_advance;
      DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਲੜੀਵਾਰ, "Run advance: %g", (ਦੁੱਗਣਾ) ਲੜੀਵਾਰ_advance);

      /* CoreText does automatic font fallback (AKA "cascading") for  characters
       * not supported by the requested font, and provides no way to turn it off,
       * so we must detect if the returned run uses a font other than the requested
       * one and fill in the buffer with .notdef glyphs instead of random glyph
       * indices from a different font.
       */
      CFDictionaryRef ਗੁਣ = CTRun_Get_ਗੁਣ (ਲੜੀਵਾਰ);
      CTFontRef ਲੜੀਵਾਰ_ct_font = static_cast<CTFontRef>(CFDictionary_Get_ਮੁੱਲ (ਗੁਣ, kCTFontAttributeName));
      ਜੇ (!CFEqual (ਲੜੀਵਾਰ_ct_font, ct_font))
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
	ਤਾਰਕਿਕ matched = ਝੂਠ;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < range_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
	  ਜੇ (range_ਰਿਕਾਰਡਾਂ[i].font && CFEqual (ਲੜੀਵਾਰ_ct_font, range_ਰਿਕਾਰਡਾਂ[i].font))
	  {
	    matched = ਸੱਚ;
	    ਰੋਕੋ;
	  }
	ਜੇ (!matched)
	{
	  hb_cf_ptr_t<CGFontRef> ਲੜੀਵਾਰ_cg_font (CTFontCopyGraphicsFont (ਲੜੀਵਾਰ_ct_font, ਖਾਲੀ_ਸੂਚਕ));
	  ਜੇ (ਲੜੀਵਾਰ_cg_font)
	    matched = CFEqual (ਲੜੀਵਾਰ_cg_font, cg_font);
	}
	ਜੇ (!matched)
	{
	  hb_cf_ptr_t<CFStringRef> font_ps_name (CTFontCopyName (ct_font, k_CTFont_Post_Script_Name_ਕੁੰਜੀ));
	  hb_cf_ptr_t<CFStringRef> ਲੜੀਵਾਰ_ps_name (CTFontCopyName (ਲੜੀਵਾਰ_ct_font, k_CTFont_Post_Script_Name_ਕੁੰਜੀ));
	  ਜੇ (font_ps_name && ਲੜੀਵਾਰ_ps_name)
	  {
	    ਜੇ (CFStringCompare (ਲੜੀਵਾਰ_ps_name, font_ps_name, 0) == k_CFCompare_Equal_ਮੰਜ਼ਿਲ)
	      matched = ਸੱਚ;
	  }
	}
	ਜੇ (!matched)
	{
	  CFRange range = CTRunGetStringRange (ਲੜੀਵਾਰ);
	  DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਲੜੀਵਾਰ, "Run used fallback font: %ld..%ld",
		     range.location, range.location + range.ਲੰਮਾਈ);
	  ਜੇ (!ਬਫਰ->ensure_inplace (ਬਫਰ->len + range.ਲੰਮਾਈ))
	    ਜਾਓ resize_and_retry;
	  hb_glyph_info_t *info = ਬਫਰ->info + ਬਫਰ->len;

	  hb_codepoint_t notdef = 0;
	  hb_direction_t dir = ਬਫਰ->props.direction;
	  hb_position_t x_advance, y_advance, x_ਵਿੱਥ_2, y_ਵਿੱਥ_2;
	  hb_font_get_glyph_advance_for_direction (font, notdef, dir, &x_advance, &y_advance);
	  hb_font_get_glyph_origin_for_direction (font, notdef, dir, &x_ਵਿੱਥ_2, &y_ਵਿੱਥ_2);
	  hb_position_t advance = hb_saturate_ਜੋੜੋ (x_advance, y_advance);
	  x_ਵਿੱਥ_2 = hb_saturate_neg (x_ਵਿੱਥ_2);
	  y_ਵਿੱਥ_2 = hb_saturate_neg (y_ਵਿੱਥ_2);

	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ old_len = ਬਫਰ->len;
	  ਲਈ (CFIndex j = range.location; j < range.location + range.ਲੰਮਾਈ; j++)
	  {
	      UniChar ch = CFString_Get_Character_At_ਸੂਚਕਾਂਕ (string_ref, j);
	      ਜੇ (hb_ਵਿੱਚ_range<UniChar> (ch, 0xDC00u, 0xDFFFu) && range.location < j)
	      {
		ch = CFString_Get_Character_At_ਸੂਚਕਾਂਕ (string_ref, j - 1);
		ਜੇ (hb_ਵਿੱਚ_range<UniChar> (ch, 0xD800u, 0xDBFFu))
		  /* This is the second of a surrogate pair.  Don't need .notdef
		   * for this one. */
		  ਜਾਰੀ;
	      }
	      ਜੇ (ਬਫਰ->unicode->is_default_ignorable (ch))
		ਜਾਰੀ;

	      info->codepoint = notdef;
	      info->cluster = log_clusters[j];

	      info->ਨਕਾਬ = advance;
	      info->var1.i32 = x_ਵਿੱਥ_2;
	      info->var2.i32 = y_ਵਿੱਥ_2;

	      info++;
	      ਬਫਰ->len++;
	  }
	  ਜੇ (HB_DIRECTION_IS_BACKWARD (ਬਫਰ->props.direction))
	    ਬਫਰ->ਉਲਟ_range (old_len, ਬਫਰ->len);
	  advances_so_far += ਲੜੀਵਾਰ_advance;
	  ਜਾਰੀ;
	}
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs = CTRun_Get_Glyph_ਗਿਣਤੀ (ਲੜੀਵਾਰ);
      ਜੇ (ਅੰਕ_glyphs == 0)
	ਜਾਰੀ;

      ਜੇ (!ਬਫਰ->ensure_inplace (ਬਫਰ->len + ਅੰਕ_glyphs))
	ਜਾਓ resize_and_retry;

      hb_glyph_info_t *ਲੜੀਵਾਰ_info = ਬਫਰ->info + ਬਫਰ->len;

      /* Testing used to indicate that CTRunGetGlyphsPtr, etc (almost?) always
       * succeed, and so copying data to our own buffer will be rare.  Reports
       * have it that this changed in OS X 10.10 Yosemite, and nullptr is returned
       * frequently.  At any rate, we can test that codepath by setting USE_PTR
       * to false. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USE_PTR ਸੱਚ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCRATCH_SAVE() \
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scratch_ਮਾਪ_saved = scratch_ਮਾਪ; \
  hb_ਬਫਰ_t::scratch_ਬਫਰ_t *scratch_saved = scratch

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCRATCH_RESTORE() \
  scratch_ਮਾਪ = scratch_ਮਾਪ_saved; \
  scratch = scratch_saved

      { /* Setup glyphs */
	SCRATCH_SAVE();
	ਅਚਲ CGGlyph* glyphs = USE_PTR ? CTRunGetGlyphsPtr (ਲੜੀਵਾਰ) : ਖਾਲੀ_ਸੂਚਕ;
	ਜੇ (!glyphs) {
	  ALLOCATE_ARRAY (CGGlyph, glyph_buf, ਅੰਕ_glyphs, ਜਾਓ resize_and_retry);
	  CTRunGetGlyphs (ਲੜੀਵਾਰ, range_all, glyph_buf);
	  glyphs = glyph_buf;
	}
	ਅਚਲ CFIndex* string_indices = USE_PTR ? CTRunGetStringIndicesPtr (ਲੜੀਵਾਰ) : ਖਾਲੀ_ਸੂਚਕ;
	ਜੇ (!string_indices) {
	  ALLOCATE_ARRAY (CFIndex, ਸੂਚਕਾਂਕ_buf, ਅੰਕ_glyphs, ਜਾਓ resize_and_retry);
	  CTRunGetStringIndices (ਲੜੀਵਾਰ, range_all, ਸੂਚਕਾਂਕ_buf);
	  string_indices = ਸੂਚਕਾਂਕ_buf;
	}
	hb_glyph_info_t *info = ਲੜੀਵਾਰ_info;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਅੰਕ_glyphs; j++)
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
	ਅਚਲ CGPoint* positions = USE_PTR ? CTRunGetPositionsPtr (ਲੜੀਵਾਰ) : ਖਾਲੀ_ਸੂਚਕ;
	ਜੇ (!positions) {
	  ALLOCATE_ARRAY (CGPoint, position_buf, ਅੰਕ_glyphs, ਜਾਓ resize_and_retry);
	  CTRunGetPositions (ਲੜੀਵਾਰ, range_all, position_buf);
	  positions = position_buf;
	}
	hb_glyph_info_t *info = ਲੜੀਵਾਰ_info;
	ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
	{
	  hb_position_t x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
	      round ((positions[0].x - advances_so_far) * x_mult));
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਅੰਕ_glyphs; j++)
	  {
	    CGFloat advance;
	    ਜੇ (likely (j + 1 < ਅੰਕ_glyphs))
	      advance = positions[j + 1].x - positions[j].x;
	    ਨਹੀਂਤਾਂ /* last glyph */
	      advance = ਲੜੀਵਾਰ_advance - (positions[j].x - positions[0].x);
	    /* Signed cast necessary to pass through negative values. */
	    info->ਨਕਾਬ = (hb_ਨਕਾਬ_t) hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (advance * x_mult));
	    info->var1.i32 = x_ਵਿੱਥ_2;
	    info->var2.i32 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (positions[j].y * y_mult));
	    info++;
	  }
	}
	ਨਹੀਂਤਾਂ
	{
	  hb_position_t y_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (
	      round ((positions[0].y - advances_so_far) * y_mult));
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ਅੰਕ_glyphs; j++)
	  {
	    CGFloat advance;
	    ਜੇ (likely (j + 1 < ਅੰਕ_glyphs))
	      advance = positions[j + 1].y - positions[j].y;
	    ਨਹੀਂਤਾਂ /* last glyph */
	      advance = ਲੜੀਵਾਰ_advance - (positions[j].y - positions[0].y);
	    /* Signed cast necessary to pass through negative values. */
	    info->ਨਕਾਬ = (hb_ਨਕਾਬ_t) hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (advance * y_mult));
	    info->var1.i32 = hb_clamp_ਮੰਜ਼ਿਲ<int32_t> (round (positions[j].x * x_mult));
	    info->var2.i32 = y_ਵਿੱਥ_2;
	    info++;
	  }
	}
	SCRATCH_RESTORE();
	advances_so_far += ਲੜੀਵਾਰ_advance;
      }
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCRATCH_RESTORE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCRATCH_SAVE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ USE_PTR
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ALLOCATE_ARRAY

      ਬਫਰ->len += ਅੰਕ_glyphs;
    }

    ਬਫਰ->clear_positions ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
    hb_glyph_info_t *info = ਬਫਰ->info;
    hb_glyph_position_t *pos = ਬਫਰ->pos;
    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	pos->x_advance = info->ਨਕਾਬ;
	pos->x_ਵਿੱਥ_2 = info->var1.i32;
	pos->y_ਵਿੱਥ_2 = info->var2.i32;

	info++; pos++;
      }
    ਨਹੀਂਤਾਂ
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	pos->y_advance = info->ਨਕਾਬ;
	pos->x_ਵਿੱਥ_2 = info->var1.i32;
	pos->y_ਵਿੱਥ_2 = info->var2.i32;

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
    ਜੇ (ਗਿਣਤੀ_2 > 1 && (ਸਥਿਤੀ_or & k_CTRun_ਸਥਿਤੀ_Non_Monotonic) &&
	HB_ਬਫਰ_CLUSTER_LEVEL_IS_MONOTONE (ਬਫਰ->cluster_level))
    {
      hb_glyph_info_t *info = ਬਫਰ->info;
      ਜੇ (HB_DIRECTION_IS_FORWARD (ਬਫਰ->props.direction))
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[ਗਿਣਤੀ_2 - 1].cluster;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = ਗਿਣਤੀ_2 - 1; i > 0; i--)
	{
	  cluster = hb_min (cluster, info[i - 1].cluster);
	  info[i - 1].cluster = cluster;
	}
      }
      ਨਹੀਂਤਾਂ
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cluster = info[0].cluster;
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
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

  ਬਫਰ->clear_glyph_flags ();
  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break ();

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ FAIL

fail:
  ਜੇ (string_ref)
    CFRelease (string_ref);
  ਜੇ (ਸਤਰ)
    CFRelease (ਸਤਰ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < range_ਰਿਕਾਰਡਾਂ.ਲੰਮਾਈ; i++)
    ਜੇ (range_ਰਿਕਾਰਡਾਂ[i].font)
      CFRelease (range_ਰਿਕਾਰਡਾਂ[i].font);

  ਵਾਪਸ ret;
}


#ਜੇ_ਅੰਤ
