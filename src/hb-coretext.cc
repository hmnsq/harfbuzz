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

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_CORETEXT

#அந்தர்பாவ "hb-shaper-impl.hh"

#அந்தர்பாவ "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

நிலையான வெறுமை
release_table_data (வெறுமை *user_data)
{
  CFDataRef cf_data = reinterpret_cast<CFDataRef> (user_data);
  CFRelease(cf_data);
}

நிலையான hb_blob_t *
_hb_cg_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, வெறுமை *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  எனில் (unlikely (!cg_font))
    திரும்பு nullptr;

  hb_cf_ptr_t<CFDataRef> cf_data (CGFontCopyTableForTag (cg_font, tag));
  எனில் (unlikely (!cf_data))
    திரும்பு nullptr;

  மாறிலி எழுத்து *data = reinterpret_cast<மாறிலி எழுத்து*> (CFDataGetBytePtr (cf_data));
  மாறிலி size_t length = CFDataGetLength (cf_data);
  எனில் (!data || !length)
    திரும்பு nullptr;

  திரும்பு hb_blob_create (data, length, HB_MEMORY_MODE_READONLY,
			 reinterpret_cast<வெறுமை *> (const_cast<__CFData *> (cf_data.release ())),
			 release_table_data);
}

நிலையான unsigned
_hb_cg_get_table_tags (மாறிலி hb_face_t *face HB_UNUSED,
		       unsigned முழு start_offset,
		       unsigned முழு *table_மொத்தம்_2,
		       hb_tag_t *table_tags,
		       வெறுமை *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  எனில் (unlikely (!cg_font))
  {
    எனில் (table_மொத்தம்_2)
      *table_மொத்தம்_2 = 0;
    திரும்பு 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_SIZE));
  எனில் (unlikely (!ct_font))
  {
    எனில் (table_மொத்தம்_2)
      *table_மொத்தம்_2 = 0;
    திரும்பு 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFontCopyAvailableTables (ct_font, kCTFontTableOptionNoOptions));
  எனில் (unlikely (!arr))
  {
    எனில் (table_மொத்தம்_2)
      *table_மொத்தம்_2 = 0;
    திரும்பு 0;
  }

  unsigned population = (unsigned) CFArray_Get_மொத்தம் (arr);

  எனில் (!table_மொத்தம்_2)
    திரும்பு population;

  எனில் (unlikely (start_offset >= population))
  {
    *table_மொத்தம்_2 = 0;
    திரும்பு population;
  }

  unsigned end_offset = start_offset + *table_மொத்தம்_2;
  எனில் (unlikely (end_offset < start_offset))
  {
    *table_மொத்தம்_2 = 0;
    திரும்பு population;
  }
  end_offset = hb_min (end_offset, (unsigned) population);

  *table_மொத்தம்_2 = end_offset - start_offset;
  எனில் (table_tags)
    க்கு (unsigned i = start_offset; i < end_offset; i++)
    {
      CTFontTableTag tag = (CTFontTableTag)(uintptr_t) CFArray_Get_மதிப்பு_At_Index (arr, i);
      table_tags[i - start_offset] = tag;
    }

  திரும்பு population;
}

நிலையான வெறுமை
_hb_cg_font_release (வெறுமை *data)
{
  எனில் (data)
    CGFontRelease ((CGFontRef) data);
}


நிலையான CTFontDescriptorRef
get_last_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> last_resort (CTFontDescriptorCreateWithNameAndSize (CFSTR("LastResort"), 0));
  எனில் (unlikely (!last_resort))
    திரும்பு nullptr;

  மாறிலி வெறுமை *cascade_elements[] = { last_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  எனில் (unlikely (!cascade_list))
    திரும்பு nullptr;

  மாறிலி வெறுமை *சாவிகள்[] = { kCTFontCascadeListAttribute };
  மாறிலி வெறுமை *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> attributes (CFDictionaryCreate (kCFAllocatorDefault,
							       சாவிகள்,
							       values,
							       1,
							       &k_CFType_Dictionary_சாவி_Call_Backs,
							       &k_CFType_Dictionary_மதிப்பு_Call_Backs));
  எனில் (unlikely (!attributes))
    திரும்பு nullptr;

  திரும்பு CTFontDescriptorCreateWithAttributes (attributes);
}

நிலையான வெறுமை
release_data (வெறுமை *info, மாறிலி வெறுமை *data, size_t size)
{
  ப்ரதிஜ்ஞா (hb_blob_get_length ((hb_blob_t *) info) == size &&
	  hb_blob_get_data ((hb_blob_t *) info, nullptr) == data);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, unsigned முழு named_instance_index)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  எனில் (unlikely (!desc_array))
    திரும்பு nullptr;

  எனில் (named_instance_index == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  இல்லையெனில்
    named_instance_index--;
  auto ct_font_desc = (CFArray_Get_மொத்தம் (desc_array) > (CFIndex) named_instance_index) ?
		      (CTFontDescriptorRef) CFArray_Get_மதிப்பு_At_Index (desc_array, (CFIndex) named_instance_index) : nullptr;
  எனில் (unlikely (!ct_font_desc))
    திரும்பு nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, nullptr));
  எனில் (unlikely (!ct_font))
    திரும்பு nullptr;

  திரும்பு CTFontCopyGraphicsFont (ct_font, nullptr);
}

CGFontRef
create_cg_font (hb_blob_t *blob, unsigned முழு index)
{
  hb_blob_make_immutable (blob);
  unsigned முழு blob_length;
  மாறிலி எழுத்து *blob_data = hb_blob_get_data (blob, &blob_length);
  எனில் (unlikely (!blob_length))
    DEBUG_செய்தி (CORETEXT, blob, "Empty blob");

  unsigned ttc_index = index & 0xFFFF;
  unsigned named_instance_index = index >> 16;

  எனில் (ttc_index != 0)
  {
    DEBUG_செய்தி (CORETEXT, blob, "TTC index %u not supported", ttc_index);
    திரும்பு nullptr; // CoreText does not support TTCs
  }

  எனில் (unlikely (named_instance_index != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#எனில் (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> data (CFDataCreate (kCFAllocatorDefault, (மாறிலி UInt8 *) blob_data, blob_length));
    எனில் (data)
    {
      auto ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromData (data);
      எனில் (likely (ct_font_desc_array))
	திரும்பு create_cg_font (ct_font_desc_array, named_instance_index);
    }
#endif
    திரும்பு nullptr;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGDataProviderCreateWithData (blob, blob_data, blob_length, &release_data));
  எனில் (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    திரும்பு nullptr;
  }
  CGFontRef cg_font = CGFontCreateWithDataProvider (provider);
  எனில் (unlikely (!cg_font))
    DEBUG_செய்தி (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  திரும்பு cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = nullptr;
  எனில் (unlikely (!face))
    திரும்பு nullptr;
  எனில் (face->destroy == _hb_cg_font_release)
    cg_font = face->user_data ? CGFontRetain ((CGFontRef) face->user_data) : nullptr;
  இல்லையெனில்
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->index);
    hb_blob_destroy (blob);
  }
  திரும்பு cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_size)
{
  எனில் (unlikely (!cg_font))
    திரும்பு nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  எனில் (cg_postscript_name)
  {
    எனில் (CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#எனில் !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#endif
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      எனில் (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_size, nullptr));
      எனில் (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_result_name (CTFontCopyPostScriptName (ct_font));
	எனில் (!ct_result_name || CFStringCompare (ct_result_name, cg_postscript_name, 0) != kCFCompareEqualTo)
	  ct_font.reset ();
      }
    }
  }

  எனில் (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_size, nullptr, nullptr));

  எனில் (unlikely (!ct_font)) {
    DEBUG_செய்தி (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    திரும்பு nullptr;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  எனில் (&CTGetCoreTextVersion != nullptr && CTGetCoreTextVersion() < 0x00070000) {
#pragma GCC diagnostic எடு
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    தருக்க isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == kCFCompareEqualTo;
    எனில் (!isEmojiFont)
      திரும்பு ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#எனில் !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus status;
  atsFont = CTFontGetPlatformFont (ct_font, வெற்று);
  status = ATSFontGetFileReference (atsFont, &fsref);
  எனில் (status == noErr)
    original_url.reset (CFURLCreateFromFSRef (வெற்று, &fsref));
#இல்லையெனில்
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#endif

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> last_resort_font_desc (get_last_resort_font_desc ());
    எனில் (last_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, last_resort_font_desc));
      எனில் (new_ct_font)
      {
	/* The CTFontCreateCopyWithAttributes call fails to stay on the same font
	 * when reconfiguring the cascade list and may switch to a different font
	 * when there are fonts that go by the same name, since the descriptor is
	 * just name and size.
	 *
	 * Avoid reconfiguring the cascade lists if the new font is outside the
	 * system locations that we cannot access from the sandboxed renderer
	 * process in Blink. This can be detected by the new file URL location
	 * that the newly found font points to. */
	hb_cf_ptr_t<CFURLRef> new_url;
#எனில் !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, வெற்று);
	status = ATSFontGetFileReference (atsFont, &fsref);
	எனில் (status == noErr)
	  new_url.reset (CFURLCreateFromFSRef (வெற்று, &fsref));
#இல்லையெனில்
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#endif
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	எனில் (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} இல்லையெனில் {
	  DEBUG_செய்தி (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      இல்லையெனில்
	DEBUG_செய்தி (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    இல்லையெனில்
      DEBUG_செய்தி (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  திரும்பு ct_font.release ();
}

/**
 * hb_coretext_face_create:
 * @cg_font: The CGFontRef to work upon
 *
 * Creates an #hb_face_t face object from the specified
 * CGFontRef.
 *
 * Return value: (transfer full): The new face object
 *
 * Since: 0.9.10
 */
hb_face_t *
hb_coretext_face_create (CGFontRef cg_font)
{
  எனில் (unlikely (!cg_font))
    திரும்பு hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_cg_reference_table, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_set_get_table_tags_func (face, _hb_cg_get_table_tags, cg_font, nullptr);
  திரும்பு face;
}

/**
 * hb_coretext_face_create_from_file_or_fail:
 * @file_name: A font filename
 * @index: The index of the face within the file
 *
 * Creates an #hb_face_t face object from the specified
 * font file and face index.
 *
 * This is similar in functionality to hb_face_create_from_file_or_fail(),
 * but uses the CoreText library for loading the font file.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the file cannot be read.
 *
 * Since: 10.1.0
 */
hb_face_t *
hb_coretext_face_create_from_file_or_fail (மாறிலி எழுத்து   *file_name,
					   unsigned முழு  index)
{
  unsigned ttc_index = index & 0xFFFF;
  unsigned named_instance_index = index >> 16;

  எனில் (ttc_index != 0)
  {
    DEBUG_செய்தி (CORETEXT, nullptr, "TTC index %u not supported", ttc_index);
    திரும்பு nullptr; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (nullptr,
								      (மாறிலி UInt8 *) file_name,
								      strlen (file_name),
								      பொய்));
  எனில் (unlikely (!url))
    திரும்பு nullptr;

  auto ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  எனில் (unlikely (!ct_font_desc_array))
    திரும்பு nullptr;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_index));
  எனில் (unlikely (!cg_font))
    திரும்பு nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  எனில் (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    திரும்பு nullptr;
  }

  hb_face_set_index (face, index);

  திரும்பு face;
}

/**
 * hb_coretext_face_create_from_blob_or_fail:
 * @blob: A blob containing the font data
 * @index: The index of the face within the blob
 *
 * Creates an #hb_face_t face object from the specified
 * blob and face index.
 *
 * This is similar in functionality to hb_face_create_or_fail(),
 * but uses the CoreText library for loading the font data.
 *
 * Return value: (transfer full): The new face object, or `NULL` if
 * no face is found at the specified index or the blob cannot be read.
 *
 * Since: 11.0.0
 */
hb_face_t *
hb_coretext_face_create_from_blob_or_fail (hb_blob_t    *blob,
					   unsigned முழு  index)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, index));
  எனில் (unlikely (!cg_font))
    திரும்பு nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  எனில் (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    திரும்பு nullptr;
  }

  hb_face_set_index (face, index);

  திரும்பு face;
}

/**
 * hb_coretext_face_get_cg_font:
 * @face: The #hb_face_t to work upon
 *
 * Fetches the CGFontRef associated with an #hb_face_t
 * face object
 *
 * Return value: the CGFontRef found
 *
 * Since: 0.9.10
 */
CGFontRef
hb_coretext_face_get_cg_font (hb_face_t *face)
{
  திரும்பு face ? (CGFontRef) (மாறிலி வெறுமை *) face->data.coretext : nullptr;
}

/**
 * hb_coretext_font_create:
 * @ct_font: The CTFontRef to work upon
 *
 * Creates an #hb_font_t font object from the specified
 * CTFontRef.
 *
 * The created font uses the default font functions implemented
 * natively by HarfBuzz. If you want to use the CoreText font functions
 * instead (rarely needed), you can do so by calling
 * by hb_coretext_font_set_funcs().
 *
 * Return value: (transfer full): The new font object
 *
 * Since: 1.7.2
 **/
hb_font_t *
hb_coretext_font_create (CTFontRef ct_font)
{
  எனில் (unlikely (!ct_font))
    திரும்பு hb_font_get_empty ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, nullptr));
  எனில் (unlikely (!cg_font))
    திரும்பு hb_font_get_empty ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  எனில் (unlikely (hb_object_is_immutable (font)))
    திரும்பு font;

  hb_font_set_ptem (font, CTFontGetSize (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  எனில் (variations)
  {

    hb_vector_t<hb_variation_t> vars;
    hb_vector_t<CFTypeRef> சாவிகள்;
    hb_vector_t<CFTypeRef> values;

    CFIndex மொத்தம் = CFDictionary_Get_மொத்தம் (variations);
    எனில் (likely (vars.alloc_exact (மொத்தம்) && சாவிகள்.resize_exact (மொத்தம்) && values.resize_exact (மொத்தம்)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_சாவிகள்_And_மதிப்புகள் (variations, சாவிகள்.arrayZ, values.arrayZ);
      க்கு (CFIndex i = 0; i < மொத்தம்; i++)
      {
	முழு tag;
	தசம value;
	CFNumber_Get_மதிப்பு ((CFNumberRef) சாவிகள்.arrayZ[i], kCFNumberIntType, &tag);
	CFNumber_Get_மதிப்பு ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_tag_t) tag, value};
	vars.push (var);
      }
      hb_font_set_variations (font, vars.arrayZ, vars.length);
    }
  }

  /* Let there be dragons here... */
  font->data.coretext.cmpexch (nullptr, (hb_coretext_font_data_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  திரும்பு font;
}

/**
 * hb_coretext_font_get_ct_font:
 * @font: #hb_font_t to work upon
 *
 * Fetches the CTFontRef associated with the specified
 * #hb_font_t font object.
 *
 * Return value: the CTFontRef found
 *
 * Since: 0.9.10
 */
CTFontRef
hb_coretext_font_get_ct_font (hb_font_t *font)
{
  திரும்பு font ? (CTFontRef) (மாறிலி வெறுமை *) font->data.coretext : nullptr;
}


#endif
