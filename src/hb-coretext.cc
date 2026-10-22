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

#include "hb.hh"

#ifdef HAVE_CORETEXT

#include "hb-shaper-impl.hh"

#include "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

static void
release_table_data (void *user_data)
{
  CFDataRef cf_data = reinterpret_cast<CFDataRef> (user_data);
  CFRelease(cf_data);
}

static hb_blob_t *
_hb_cg_reference_table (hb_face_t *face HB_UNUSED, hb_tag_t tag, void *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ nullptr;

  hb_cf_ptr_t<CFDataRef> cf_data (CGFontCopyTableForTag (cg_font, tag));
  ᬬᬾᬦ᭄ (unlikely (!cf_data))
    ᬯᬮᬶ nullptr;

  const var_na *data = reinterpret_cast<const var_na*> (CFDataGetBytePtr (cf_data));
  const size_t length = CFDataGetLength (cf_data);
  ᬬᬾᬦ᭄ (!data || !length)
    ᬯᬮᬶ nullptr;

  ᬯᬮᬶ hb_blob_create (data, length, HB_MEMORY_MODE_READONLY,
			 reinterpret_cast<void *> (const_cast<__CFData *> (cf_data.release ())),
			 release_table_data);
}

static unsigned
_hb_cg_get_table_tags (const hb_face_t *face HB_UNUSED,
		       unsigned puur_naa_nka start_offset,
		       unsigned puur_naa_nka *table_count,
		       hb_tag_t *table_tags,
		       void *user_data)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_data);
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
  {
    ᬬᬾᬦ᭄ (table_count)
      *table_count = 0;
    ᬯᬮᬶ 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_SIZE));
  ᬬᬾᬦ᭄ (unlikely (!ct_font))
  {
    ᬬᬾᬦ᭄ (table_count)
      *table_count = 0;
    ᬯᬮᬶ 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFontCopyAvailableTables (ct_font, kCTFontTableOptionNoOptions));
  ᬬᬾᬦ᭄ (unlikely (!arr))
  {
    ᬬᬾᬦ᭄ (table_count)
      *table_count = 0;
    ᬯᬮᬶ 0;
  }

  unsigned population = (unsigned) CFArrayGetCount (arr);

  ᬬᬾᬦ᭄ (!table_count)
    ᬯᬮᬶ population;

  ᬬᬾᬦ᭄ (unlikely (start_offset >= population))
  {
    *table_count = 0;
    ᬯᬮᬶ population;
  }

  unsigned end_offset = start_offset + *table_count;
  ᬬᬾᬦ᭄ (unlikely (end_offset < start_offset))
  {
    *table_count = 0;
    ᬯᬮᬶ population;
  }
  end_offset = hb_min (end_offset, (unsigned) population);

  *table_count = end_offset - start_offset;
  ᬬᬾᬦ᭄ (table_tags)
    for (unsigned i = start_offset; i < end_offset; i++)
    {
      CTFontTableTag tag = (CTFontTableTag)(uintptr_t) CFArrayGetValueAtIndex (arr, i);
      table_tags[i - start_offset] = tag;
    }

  ᬯᬮᬶ population;
}

static void
_hb_cg_font_release (void *data)
{
  ᬬᬾᬦ᭄ (data)
    CGFontRelease ((CGFontRef) data);
}


static CTFontDescriptorRef
get_last_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> last_resort (CTFontDescriptorCreateWithNameAndSize (CFSTR("LastResort"), 0));
  ᬬᬾᬦ᭄ (unlikely (!last_resort))
    ᬯᬮᬶ nullptr;

  const void *cascade_elements[] = { last_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  ᬬᬾᬦ᭄ (unlikely (!cascade_list))
    ᬯᬮᬶ nullptr;

  const void *keys[] = { kCTFontCascadeListAttribute };
  const void *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> attributes (CFDictionaryCreate (kCFAllocatorDefault,
							       keys,
							       values,
							       1,
							       &kCFTypeDictionaryKeyCallBacks,
							       &kCFTypeDictionaryValueCallBacks));
  ᬬᬾᬦ᭄ (unlikely (!attributes))
    ᬯᬮᬶ nullptr;

  ᬯᬮᬶ CTFontDescriptorCreateWithAttributes (attributes);
}

static void
release_data (void *info, const void *data, size_t size)
{
  assert (hb_blob_get_length ((hb_blob_t *) info) == size &&
	  hb_blob_get_data ((hb_blob_t *) info, nullptr) == data);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, unsigned puur_naa_nka named_instance_index)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  ᬬᬾᬦ᭄ (unlikely (!desc_array))
    ᬯᬮᬶ nullptr;

  ᬬᬾᬦ᭄ (named_instance_index == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  ᬮᬶᬬ
    named_instance_index--;
  auto ct_font_desc = (CFArrayGetCount (desc_array) > (CFIndex) named_instance_index) ?
		      (CTFontDescriptorRef) CFArrayGetValueAtIndex (desc_array, (CFIndex) named_instance_index) : nullptr;
  ᬬᬾᬦ᭄ (unlikely (!ct_font_desc))
    ᬯᬮᬶ nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, nullptr));
  ᬬᬾᬦ᭄ (unlikely (!ct_font))
    ᬯᬮᬶ nullptr;

  ᬯᬮᬶ CTFontCopyGraphicsFont (ct_font, nullptr);
}

CGFontRef
create_cg_font (hb_blob_t *blob, unsigned puur_naa_nka index)
{
  hb_blob_make_immutable (blob);
  unsigned puur_naa_nka blob_length;
  const var_na *blob_data = hb_blob_get_data (blob, &blob_length);
  ᬬᬾᬦ᭄ (unlikely (!blob_length))
    DEBUG_MSG (CORETEXT, blob, "Empty blob");

  unsigned ttc_index = index & 0xFFFF;
  unsigned named_instance_index = index >> 16;

  ᬬᬾᬦ᭄ (ttc_index != 0)
  {
    DEBUG_MSG (CORETEXT, blob, "TTC index %u not supported", ttc_index);
    ᬯᬮᬶ nullptr; // CoreText does not support TTCs
  }

  ᬬᬾᬦ᭄ (unlikely (named_instance_index != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#ᬬᬾᬦ᭄ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> data (CFDataCreate (kCFAllocatorDefault, (const UInt8 *) blob_data, blob_length));
    ᬬᬾᬦ᭄ (data)
    {
      auto ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromData (data);
      ᬬᬾᬦ᭄ (likely (ct_font_desc_array))
	ᬯᬮᬶ create_cg_font (ct_font_desc_array, named_instance_index);
    }
#endif
    ᬯᬮᬶ nullptr;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGDataProviderCreateWithData (blob, blob_data, blob_length, &release_data));
  ᬬᬾᬦ᭄ (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    ᬯᬮᬶ nullptr;
  }
  CGFontRef cg_font = CGFontCreateWithDataProvider (provider);
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    DEBUG_MSG (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  ᬯᬮᬶ cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = nullptr;
  ᬬᬾᬦ᭄ (unlikely (!face))
    ᬯᬮᬶ nullptr;
  ᬬᬾᬦ᭄ (face->destroy == _hb_cg_font_release)
    cg_font = face->user_data ? CGFontRetain ((CGFontRef) face->user_data) : nullptr;
  ᬮᬶᬬ
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->index);
    hb_blob_destroy (blob);
  }
  ᬯᬮᬶ cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_size)
{
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ nullptr;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  ᬬᬾᬦ᭄ (cg_postscript_name)
  {
    ᬬᬾᬦ᭄ (CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFStringHasPrefix (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#ᬬᬾᬦ᭄ !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#endif
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      ᬬᬾᬦ᭄ (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_size, nullptr));
      ᬬᬾᬦ᭄ (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_result_name (CTFontCopyPostScriptName (ct_font));
	ᬬᬾᬦ᭄ (!ct_result_name || CFStringCompare (ct_result_name, cg_postscript_name, 0) != kCFCompareEqualTo)
	  ct_font.reset ();
      }
    }
  }

  ᬬᬾᬦ᭄ (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_size, nullptr, nullptr));

  ᬬᬾᬦ᭄ (unlikely (!ct_font)) {
    DEBUG_MSG (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    ᬯᬮᬶ nullptr;
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
  ᬬᬾᬦ᭄ (&CTGetCoreTextVersion != nullptr && CTGetCoreTextVersion() < 0x00070000) {
#pragma GCC diagnostic pop
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    bool isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == kCFCompareEqualTo;
    ᬬᬾᬦ᭄ (!isEmojiFont)
      ᬯᬮᬶ ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#ᬬᬾᬦ᭄ !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus status;
  atsFont = CTFontGetPlatformFont (ct_font, NULL);
  status = ATSFontGetFileReference (atsFont, &fsref);
  ᬬᬾᬦ᭄ (status == noErr)
    original_url.reset (CFURLCreateFromFSRef (NULL, &fsref));
#ᬮᬶᬬ
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#endif

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> last_resort_font_desc (get_last_resort_font_desc ());
    ᬬᬾᬦ᭄ (last_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFontCreateCopyWithAttributes (ct_font, 0.0, nullptr, last_resort_font_desc));
      ᬬᬾᬦ᭄ (new_ct_font)
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
#ᬬᬾᬦ᭄ !(defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) && MAC_OS_X_VERSION_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, NULL);
	status = ATSFontGetFileReference (atsFont, &fsref);
	ᬬᬾᬦ᭄ (status == noErr)
	  new_url.reset (CFURLCreateFromFSRef (NULL, &fsref));
#ᬮᬶᬬ
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#endif
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	ᬬᬾᬦ᭄ (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} ᬮᬶᬬ {
	  DEBUG_MSG (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      ᬮᬶᬬ
	DEBUG_MSG (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    ᬮᬶᬬ
      DEBUG_MSG (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  ᬯᬮᬶ ct_font.release ();
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
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ hb_face_get_empty ();

  hb_face_t *face = hb_face_create_for_tables (_hb_cg_reference_table, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_set_get_table_tags_func (face, _hb_cg_get_table_tags, cg_font, nullptr);
  ᬯᬮᬶ face;
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
hb_coretext_face_create_from_file_or_fail (const var_na   *file_name,
					   unsigned puur_naa_nka  index)
{
  unsigned ttc_index = index & 0xFFFF;
  unsigned named_instance_index = index >> 16;

  ᬬᬾᬦ᭄ (ttc_index != 0)
  {
    DEBUG_MSG (CORETEXT, nullptr, "TTC index %u not supported", ttc_index);
    ᬯᬮᬶ nullptr; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (nullptr,
								      (const UInt8 *) file_name,
								      strlen (file_name),
								      false));
  ᬬᬾᬦ᭄ (unlikely (!url))
    ᬯᬮᬶ nullptr;

  auto ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  ᬬᬾᬦ᭄ (unlikely (!ct_font_desc_array))
    ᬯᬮᬶ nullptr;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_index));
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ᬬᬾᬦ᭄ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    ᬯᬮᬶ nullptr;
  }

  hb_face_set_index (face, index);

  ᬯᬮᬶ face;
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
					   unsigned puur_naa_nka  index)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, index));
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ nullptr;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ᬬᬾᬦ᭄ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    ᬯᬮᬶ nullptr;
  }

  hb_face_set_index (face, index);

  ᬯᬮᬶ face;
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
  ᬯᬮᬶ face ? (CGFontRef) (const void *) face->data.coretext : nullptr;
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
  ᬬᬾᬦ᭄ (unlikely (!ct_font))
    ᬯᬮᬶ hb_font_get_empty ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, nullptr));
  ᬬᬾᬦ᭄ (unlikely (!cg_font))
    ᬯᬮᬶ hb_font_get_empty ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  ᬬᬾᬦ᭄ (unlikely (hb_object_is_immutable (font)))
    ᬯᬮᬶ font;

  hb_font_set_ptem (font, CTFontGetSize (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  ᬬᬾᬦ᭄ (variations)
  {

    hb_vector_t<hb_variation_t> vars;
    hb_vector_t<CFTypeRef> keys;
    hb_vector_t<CFTypeRef> values;

    CFIndex count = CFDictionaryGetCount (variations);
    ᬬᬾᬦ᭄ (likely (vars.alloc_exact (count) && keys.resize_exact (count) && values.resize_exact (count)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionaryGetKeysAndValues (variations, keys.arrayZ, values.arrayZ);
      for (CFIndex i = 0; i < count; i++)
      {
	puur_naa_nka tag;
	float value;
	CFNumberGetValue ((CFNumberRef) keys.arrayZ[i], kCFNumberIntType, &tag);
	CFNumberGetValue ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

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

  ᬯᬮᬶ font;
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
  ᬯᬮᬶ font ? (CTFontRef) (const void *) font->data.coretext : nullptr;
}


#endif
