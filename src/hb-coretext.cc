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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_CORETEXT

#अन्तर्भाव "hb-shaper-impl.hh"

#अन्तर्भाव "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

स्थिर शून्य
release_table_data (शून्य *user_data)
{
  CFDataRef cf_data = reinterpret_cast<CFDataRef> (user_data);
  CFRelease(cf_data);
}

स्थिर hb_blob_t *
_hb_cg_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, शून्य *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  जर (unlikely (!cg_font))
    परत nullptr;

  hb_cf_ptr_t<CFDataRef> cf_data (CGFontCopyTableForTag (cg_font, tag));
  जर (unlikely (!cf_data))
    परत nullptr;

  स्थिरांक अक्षर *data = reinterpret_cast<स्थिरांक अक्षर*> (CFDataGetBytePtr (cf_data));
  स्थिरांक माप_प्रकार length = CFDataGetLength (cf_data);
  जर (!data || !length)
    परत nullptr;

  परत hb_blob_create (data, length, HB_MEMORY_MODE_READONLY,
			 reinterpret_cast<शून्य *> (const_cast<__CFData *> (cf_data.release ())),
			 release_table_data);
}

स्थिर अचिन्हित
_hb_cg_get_table_tags (स्थिरांक hb_face_t *face HB_UNUSED,
		       अचिन्हित पूर्णांक start_offset,
		       अचिन्हित पूर्णांक *table_मोजणी_2,
		       hb_tag_t *table_tags,
		       शून्य *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  जर (unlikely (!cg_font))
  {
    जर (table_मोजणी_2)
      *table_मोजणी_2 = 0;
    परत 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_SIZE));
  जर (unlikely (!ct_font))
  {
    जर (table_मोजणी_2)
      *table_मोजणी_2 = 0;
    परत 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFontCopyAvailableTables (ct_font, kCTFontTableOptionNoOptions));
  जर (unlikely (!arr))
  {
    जर (table_मोजणी_2)
      *table_मोजणी_2 = 0;
    परत 0;
  }

  अचिन्हित population = (अचिन्हित) CFArray_Get_मोजणी (arr);

  जर (!table_मोजणी_2)
    परत population;

  जर (unlikely (start_offset >= population))
  {
    *table_मोजणी_2 = 0;
    परत population;
  }

  अचिन्हित end_offset = start_offset + *table_मोजणी_2;
  जर (unlikely (end_offset < start_offset))
  {
    *table_मोजणी_2 = 0;
    परत population;
  }
  end_offset = hb_min (end_offset, (अचिन्हित) population);

  *table_मोजणी_2 = end_offset - start_offset;
  जर (table_tags)
    साठी (अचिन्हित i = start_offset; i < end_offset; i++)
    {
      CTFontTableTag tag = (CTFontTableTag)(uintptr_t) CFArray_Get_मूल्य_At_Index (arr, i);
      table_tags[i - start_offset] = tag;
    }

  परत population;
}

स्थिर शून्य
_hb_cg_font_release (शून्य *data)
{
  जर (data)
    CGFontRelease ((CGFontRef) data);
}


स्थिर CTFontDescriptorRef
get_last_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> last_resort (CTFontDescriptorCreateWithNameAndSize (CFSTR("LastResort"), 0));
  जर (unlikely (!last_resort))
    परत nullptr;

  स्थिरांक शून्य *cascade_elements[] = { last_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  जर (unlikely (!cascade_list))
    परत nullptr;

  स्थिरांक शून्य *किल्ल्या[] = { kCTFontCascadeListAttribute };
  स्थिरांक शून्य *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> attributes (CFDictionaryCreate (kCFAllocatorDefault,
							       किल्ल्या,
							       values,
							       1,
							       &k_CFType_Dictionary_किल्ली_Call_Backs,
							       &k_CFType_Dictionary_मूल्य_Call_Backs));
  जर (unlikely (!attributes))
    परत nullptr;

  परत CTFontDescriptorCreateWithAttributes (attributes);
}

स्थिर शून्य
release_data (शून्य *info, स्थिरांक शून्य *data, माप_प्रकार size)
{
  प्रतिज्ञा (hb_blob_get_length ((hb_blob_t *) info) == size &&
	  hb_blob_get_data ((hb_blob_t *) info, nullptr) == data);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, अचिन्हित पूर्णांक named_instance_index)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  जर (unlikely (!desc_array))
    परत nullptr;

  जर (named_instance_index == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  अन्यथा
    named_instance_index--;
  स्वतः ct_font_desc = (CFArray_Get_मोजणी (desc_array) > (CFIndex) named_instance_index) ?
		      (CTFontDescriptorRef) CFArray_Get_मूल्य_At_Index (desc_array, (CFIndex) named_instance_index) : nullptr;
  जर (unlikely (!ct_font_desc))
    परत nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, nullptr));
  जर (unlikely (!ct_font))
    परत nullptr;

  परत CTFontCopyGraphicsFont (ct_font, nullptr);
}

CGFontRef
create_cg_font (hb_blob_t *blob, अचिन्हित पूर्णांक index)
{
  hb_blob_make_immutable (blob);
  अचिन्हित पूर्णांक blob_length;
  स्थिरांक अक्षर *blob_data = hb_blob_get_data (blob, &blob_length);
  जर (unlikely (!blob_length))
    DEBUG_संदेश (CORETEXT, blob, "Empty blob");

  अचिन्हित ttc_index = index & 0xFFFF;
  अचिन्हित named_instance_index = index >> 16;

  जर (ttc_index != 0)
  {
    DEBUG_संदेश (CORETEXT, blob, "TTC index %u not supported", ttc_index);
    परत nullptr; // CoreText does not support TTCs
  }

  जर (unlikely (named_instance_index != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#जर (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> data (CFDataCreate (kCFAllocatorDefault, (स्थिरांक UInt8 *) blob_data, blob_length));
    जर (data)
    {
      स्वतः ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromData (data);
      जर (likely (ct_font_desc_array))
	परत create_cg_font (ct_font_desc_array, named_instance_index);
    }
#endif
    परत nullptr;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGDataProviderCreateWithData (blob, blob_data, blob_length, &release_data));
  जर (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    परत nullptr;
  }
  CGFontRef cg_font = CGFontCreateWithDataProvider (provider);
  जर (unlikely (!cg_font))
    DEBUG_संदेश (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  परत cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = nullptr;
  जर (unlikely (!face))
    परत nullptr;
  जर (face->destroy == _hb_cg_font_release)
    cg_font = face->user_data ? CGFontRetain ((CGFontRef) face->user_data) : nullptr;
  अन्यथा
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->index);
    hb_blob_destroy (blob);
  }
  परत cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_size)
{
  जर (unlikely (!cg_font))
    परत nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  जर (cg_postscript_name)
  {
    जर (CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#जर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#endif
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      जर (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_size, nullptr));
      जर (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_result_name (CTFontCopyPostScriptName (ct_font));
	जर (!ct_result_name || CFStringCompare (ct_result_name, cg_postscript_name, 0) != kCFCompareEqualTo)
	  ct_font.reset ();
      }
    }
  }

  जर (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_size, nullptr, nullptr));

  जर (unlikely (!ct_font)) {
    DEBUG_संदेश (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    परत nullptr;
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
  जर (&CTGetCoreTextVersion != nullptr && CTGetCoreTextVersion() < 0x00070000) {
#pragma GCC diagnostic काढा
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    तार्किक isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == kCFCompareEqualTo;
    जर (!isEmojiFont)
      परत ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#जर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus status;
  atsFont = CTFontGetPlatformFont (ct_font, रिक्त);
  status = ATSFontGetFileReference (atsFont, &fsref);
  जर (status == noErr)
    original_url.reset (CFURLCreateFromFSRef (रिक्त, &fsref));
#अन्यथा
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#endif

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> last_resort_font_desc (get_last_resort_font_desc ());
    जर (last_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, last_resort_font_desc));
      जर (new_ct_font)
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
#जर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, रिक्त);
	status = ATSFontGetFileReference (atsFont, &fsref);
	जर (status == noErr)
	  new_url.reset (CFURLCreateFromFSRef (रिक्त, &fsref));
#अन्यथा
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#endif
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	जर (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} अन्यथा {
	  DEBUG_संदेश (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      अन्यथा
	DEBUG_संदेश (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    अन्यथा
      DEBUG_संदेश (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  परत ct_font.release ();
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
  जर (unlikely (!cg_font))
    परत hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_cg_reference_table, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_set_get_table_tags_func (face, _hb_cg_get_table_tags, cg_font, nullptr);
  परत face;
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
hb_coretext_face_create_from_file_or_fail (स्थिरांक अक्षर   *file_name,
					   अचिन्हित पूर्णांक  index)
{
  अचिन्हित ttc_index = index & 0xFFFF;
  अचिन्हित named_instance_index = index >> 16;

  जर (ttc_index != 0)
  {
    DEBUG_संदेश (CORETEXT, nullptr, "TTC index %u not supported", ttc_index);
    परत nullptr; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (nullptr,
								      (स्थिरांक UInt8 *) file_name,
								      म_माप (file_name),
								      असत्य));
  जर (unlikely (!url))
    परत nullptr;

  स्वतः ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  जर (unlikely (!ct_font_desc_array))
    परत nullptr;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_index));
  जर (unlikely (!cg_font))
    परत nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  जर (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    परत nullptr;
  }

  hb_face_set_index (face, index);

  परत face;
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
					   अचिन्हित पूर्णांक  index)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, index));
  जर (unlikely (!cg_font))
    परत nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  जर (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    परत nullptr;
  }

  hb_face_set_index (face, index);

  परत face;
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
  परत face ? (CGFontRef) (स्थिरांक शून्य *) face->data.coretext : nullptr;
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
  जर (unlikely (!ct_font))
    परत hb_font_get_empty ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, nullptr));
  जर (unlikely (!cg_font))
    परत hb_font_get_empty ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  जर (unlikely (hb_object_is_immutable (font)))
    परत font;

  hb_font_set_ptem (font, CTFontGetSize (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  जर (variations)
  {

    hb_vector_t<hb_variation_t> vars;
    hb_vector_t<CFTypeRef> किल्ल्या;
    hb_vector_t<CFTypeRef> values;

    CFIndex मोजणी = CFDictionary_Get_मोजणी (variations);
    जर (likely (vars.alloc_exact (मोजणी) && किल्ल्या.resize_exact (मोजणी) && values.resize_exact (मोजणी)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_किल्ल्या_And_मूल्ये (variations, किल्ल्या.arrayZ, values.arrayZ);
      साठी (CFIndex i = 0; i < मोजणी; i++)
      {
	पूर्णांक tag;
	दशांश value;
	CFNumber_Get_मूल्य ((CFNumberRef) किल्ल्या.arrayZ[i], kCFNumberIntType, &tag);
	CFNumber_Get_मूल्य ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

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

  परत font;
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
  परत font ? (CTFontRef) (स्थिरांक शून्य *) font->data.coretext : nullptr;
}


#endif
