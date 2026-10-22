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

#समावेश "hb.hh"

#अगर_घोषित HAVE_CORETEXT

#समावेश "hb-shaper-impl.hh"

#समावेश "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

अटल व्योम
release_table_तथ्य (व्योम *user_तथ्य)
{
  CFDataRef cf_तथ्य = reinterpret_cast<CFDataRef> (user_तथ्य);
  CFRelease(cf_तथ्य);
}

अटल hb_blob_t *
_hb_cg_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, व्योम *user_तथ्य)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_तथ्य);
  अगर (unlikely (!cg_font))
    वापस nullptr;

  hb_cf_ptr_t<CFDataRef> cf_तथ्य (CGFontCopyTableForTag (cg_font, tag));
  अगर (unlikely (!cf_तथ्य))
    वापस nullptr;

  स्थिर अक्षर *तथ्य = reinterpret_cast<स्थिर अक्षर*> (CFDataGetBytePtr (cf_तथ्य));
  स्थिर माप_प्रकार लंबाई = CFData_Get_लंबाई (cf_तथ्य);
  अगर (!तथ्य || !लंबाई)
    वापस nullptr;

  वापस hb_blob_create (तथ्य, लंबाई, HB_MEMORY_MODE_READONLY,
			 reinterpret_cast<व्योम *> (const_cast<__CFData *> (cf_तथ्य.release ())),
			 release_table_तथ्य);
}

अटल अचिन्हित
_hb_cg_get_table_tags (स्थिर hb_face_t *face HB_UNUSED,
		       अचिन्हित पूर्णांक start_offset,
		       अचिन्हित पूर्णांक *table_गिनती_2,
		       hb_tag_t *table_tags,
		       व्योम *user_तथ्य)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_तथ्य);
  अगर (unlikely (!cg_font))
  {
    अगर (table_गिनती_2)
      *table_गिनती_2 = 0;
    वापस 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_आकार));
  अगर (unlikely (!ct_font))
  {
    अगर (table_गिनती_2)
      *table_गिनती_2 = 0;
    वापस 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFontCopyAvailableTables (ct_font, kCTFontTableOptionNoOptions));
  अगर (unlikely (!arr))
  {
    अगर (table_गिनती_2)
      *table_गिनती_2 = 0;
    वापस 0;
  }

  अचिन्हित population = (अचिन्हित) CFArray_Get_गिनती (arr);

  अगर (!table_गिनती_2)
    वापस population;

  अगर (unlikely (start_offset >= population))
  {
    *table_गिनती_2 = 0;
    वापस population;
  }

  अचिन्हित end_offset = start_offset + *table_गिनती_2;
  अगर (unlikely (end_offset < start_offset))
  {
    *table_गिनती_2 = 0;
    वापस population;
  }
  end_offset = hb_min (end_offset, (अचिन्हित) population);

  *table_गिनती_2 = end_offset - start_offset;
  अगर (table_tags)
    क्रम (अचिन्हित i = start_offset; i < end_offset; i++)
    {
      CTFontTableTag tag = (CTFontTableTag)(uintptr_t) CFArray_Get_मूल्य_At_सूचकांक (arr, i);
      table_tags[i - start_offset] = tag;
    }

  वापस population;
}

अटल व्योम
_hb_cg_font_release (व्योम *तथ्य)
{
  अगर (तथ्य)
    CGFontRelease ((CGFontRef) तथ्य);
}


अटल CTFontDescriptorRef
get_last_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> last_resort (CTFont_Descriptor_Create_With_Name_And_आकार (CFSTR("LastResort"), 0));
  अगर (unlikely (!last_resort))
    वापस nullptr;

  स्थिर व्योम *cascade_elements[] = { last_resort.लो () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  अगर (unlikely (!cascade_list))
    वापस nullptr;

  स्थिर व्योम *कुंजियाँ[] = { kCTFontCascadeListAttribute };
  स्थिर व्योम *values[] = { cascade_list.लो () };
  hb_cf_ptr_t<CFDictionaryRef> attributes (CFDictionaryCreate (kCFAllocatorDefault,
							       कुंजियाँ,
							       values,
							       1,
							       &k_CFType_Dictionary_कुंजी_Call_Backs,
							       &k_CFType_Dictionary_मूल्य_Call_Backs));
  अगर (unlikely (!attributes))
    वापस nullptr;

  वापस CTFontDescriptorCreateWithAttributes (attributes);
}

अटल व्योम
release_तथ्य (व्योम *info, स्थिर व्योम *तथ्य, माप_प्रकार आकार)
{
  निश्चित (hb_blob_get_लंबाई ((hb_blob_t *) info) == आकार &&
	  hb_blob_get_तथ्य ((hb_blob_t *) info, nullptr) == तथ्य);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, अचिन्हित पूर्णांक named_instance_सूचकांक)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  अगर (unlikely (!desc_array))
    वापस nullptr;

  अगर (named_instance_सूचकांक == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  अन्यथा
    named_instance_सूचकांक--;
  स्वतः ct_font_desc = (CFArray_Get_गिनती (desc_array) > (CFIndex) named_instance_सूचकांक) ?
		      (CTFontDescriptorRef) CFArray_Get_मूल्य_At_सूचकांक (desc_array, (CFIndex) named_instance_सूचकांक) : nullptr;
  अगर (unlikely (!ct_font_desc))
    वापस nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, nullptr));
  अगर (unlikely (!ct_font))
    वापस nullptr;

  वापस CTFontCopyGraphicsFont (ct_font, nullptr);
}

CGFontRef
create_cg_font (hb_blob_t *blob, अचिन्हित पूर्णांक सूचकांक_2)
{
  hb_blob_make_immutable (blob);
  अचिन्हित पूर्णांक blob_लंबाई;
  स्थिर अक्षर *blob_तथ्य = hb_blob_get_तथ्य (blob, &blob_लंबाई);
  अगर (unlikely (!blob_लंबाई))
    DEBUG_संदेश (CORETEXT, blob, "Empty blob");

  अचिन्हित ttc_सूचकांक = सूचकांक_2 & 0xFFFF;
  अचिन्हित named_instance_सूचकांक = सूचकांक_2 >> 16;

  अगर (ttc_सूचकांक != 0)
  {
    DEBUG_संदेश (CORETEXT, blob, "TTC index %u not supported", ttc_सूचकांक);
    वापस nullptr; // CoreText does not support TTCs
  }

  अगर (unlikely (named_instance_सूचकांक != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#अगर (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> तथ्य (CFDataCreate (kCFAllocatorDefault, (स्थिर UInt8 *) blob_तथ्य, blob_लंबाई));
    अगर (तथ्य)
    {
      स्वतः ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_तथ्य (तथ्य);
      अगर (likely (ct_font_desc_array))
	वापस create_cg_font (ct_font_desc_array, named_instance_सूचकांक);
    }
#पूर्ण_अगर
    वापस nullptr;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_तथ्य (blob, blob_तथ्य, blob_लंबाई, &release_तथ्य));
  अगर (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    वापस nullptr;
  }
  CGFontRef cg_font = CGFont_Create_With_तथ्य_Provider (provider);
  अगर (unlikely (!cg_font))
    DEBUG_संदेश (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  वापस cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = nullptr;
  अगर (unlikely (!face))
    वापस nullptr;
  अगर (face->destroy == _hb_cg_font_release)
    cg_font = face->user_तथ्य ? CGFontRetain ((CGFontRef) face->user_तथ्य) : nullptr;
  अन्यथा
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->सूचकांक_2);
    hb_blob_destroy (blob);
  }
  वापस cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_आकार)
{
  अगर (unlikely (!cg_font))
    वापस nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  अगर (cg_postscript_name)
  {
    अगर (CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#अगर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#पूर्ण_अगर
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      अगर (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_आकार, nullptr));
      अगर (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_परिणाम_name (CTFontCopyPostScriptName (ct_font));
	अगर (!ct_परिणाम_name || CFStringCompare (ct_परिणाम_name, cg_postscript_name, 0) != kCFCompareEqualTo)
	  ct_font.reset ();
      }
    }
  }

  अगर (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_आकार, nullptr, nullptr));

  अगर (unlikely (!ct_font)) {
    DEBUG_संदेश (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    वापस nullptr;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wdeprecated-declarations"
  अगर (&CTGetCoreTextVersion != nullptr && CTGetCoreTextVersion() < 0x00070000) {
#आशय GCC diagnostic निकालो
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    bool isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == kCFCompareEqualTo;
    अगर (!isEmojiFont)
      वापस ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#अगर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus status;
  atsFont = CTFontGetPlatformFont (ct_font, शून्य);
  status = ATSFontGetFileReference (atsFont, &fsref);
  अगर (status == noErr)
    original_url.reset (CFURLCreateFromFSRef (शून्य, &fsref));
#अन्यथा
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#पूर्ण_अगर

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> last_resort_font_desc (get_last_resort_font_desc ());
    अगर (last_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, last_resort_font_desc));
      अगर (new_ct_font)
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
#अगर !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, शून्य);
	status = ATSFontGetFileReference (atsFont, &fsref);
	अगर (status == noErr)
	  new_url.reset (CFURLCreateFromFSRef (शून्य, &fsref));
#अन्यथा
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#पूर्ण_अगर
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	अगर (!original_url || !new_url || CFEqual (original_url, new_url)) {
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

  वापस ct_font.release ();
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
  अगर (unlikely (!cg_font))
    वापस hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_cg_reference_table, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_set_get_table_tags_func (face, _hb_cg_get_table_tags, cg_font, nullptr);
  वापस face;
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
hb_coretext_face_create_from_file_or_fail (स्थिर अक्षर   *file_name,
					   अचिन्हित पूर्णांक  सूचकांक_2)
{
  अचिन्हित ttc_सूचकांक = सूचकांक_2 & 0xFFFF;
  अचिन्हित named_instance_सूचकांक = सूचकांक_2 >> 16;

  अगर (ttc_सूचकांक != 0)
  {
    DEBUG_संदेश (CORETEXT, nullptr, "TTC index %u not supported", ttc_सूचकांक);
    वापस nullptr; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (nullptr,
								      (स्थिर UInt8 *) file_name,
								      म_माप (file_name),
								      false));
  अगर (unlikely (!url))
    वापस nullptr;

  स्वतः ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  अगर (unlikely (!ct_font_desc_array))
    वापस nullptr;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_सूचकांक));
  अगर (unlikely (!cg_font))
    वापस nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  अगर (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    वापस nullptr;
  }

  hb_face_set_सूचकांक (face, सूचकांक_2);

  वापस face;
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
					   अचिन्हित पूर्णांक  सूचकांक_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, सूचकांक_2));
  अगर (unlikely (!cg_font))
    वापस nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  अगर (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    वापस nullptr;
  }

  hb_face_set_सूचकांक (face, सूचकांक_2);

  वापस face;
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
  वापस face ? (CGFontRef) (स्थिर व्योम *) face->तथ्य.coretext : nullptr;
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
  अगर (unlikely (!ct_font))
    वापस hb_font_get_empty ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, nullptr));
  अगर (unlikely (!cg_font))
    वापस hb_font_get_empty ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  अगर (unlikely (hb_object_is_immutable (font)))
    वापस font;

  hb_font_set_ptem (font, CTFont_Get_आकार (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  अगर (variations)
  {

    hb_vector_t<hb_variation_t> vars;
    hb_vector_t<CFTypeRef> कुंजियाँ;
    hb_vector_t<CFTypeRef> values;

    CFIndex गिनती = CFDictionary_Get_गिनती (variations);
    अगर (likely (vars.alloc_exact (गिनती) && कुंजियाँ.resize_exact (गिनती) && values.resize_exact (गिनती)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_कुंजियाँ_And_मूल्यावली (variations, कुंजियाँ.arrayZ, values.arrayZ);
      क्रम (CFIndex i = 0; i < गिनती; i++)
      {
	पूर्णांक tag;
	भग्न value;
	CFNumber_Get_मूल्य ((CFNumberRef) कुंजियाँ.arrayZ[i], kCFNumberIntType, &tag);
	CFNumber_Get_मूल्य ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_tag_t) tag, value};
	vars.push (var);
      }
      hb_font_set_variations (font, vars.arrayZ, vars.लंबाई);
    }
  }

  /* Let there be dragons here... */
  font->तथ्य.coretext.cmpexch (nullptr, (hb_coretext_font_तथ्य_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  वापस font;
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
  वापस font ? (CTFontRef) (स्थिर व्योम *) font->तथ्य.coretext : nullptr;
}


#पूर्ण_अगर
