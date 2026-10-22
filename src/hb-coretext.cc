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


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

ساكن فراغ
release_جدول_معلومات (فراغ *user_معلومات)
{
  CFDataRef cf_معلومات = reinterpret_cast<CFDataRef> (user_معلومات);
  CFRelease(cf_معلومات);
}

ساكن hb_blob_t *
hb_cg_reference_جدول (hb_face_t *face HB_ناکارول_شوی, hb_نښان_t نښان_2, فراغ *user_معلومات)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_معلومات);
  إذا (unlikely (!cg_font))
    أرجع تش_اشاره;

  hb_cf_ptr_t<CFDataRef> cf_معلومات (CGFont_Copy_جدول_For_نښان (cg_font, نښان_2));
  إذا (unlikely (!cf_معلومات))
    أرجع تش_اشاره;

  ثابت حرف *معلومات = reinterpret_cast<ثابت حرف*> (CFDataGetBytePtr (cf_معلومات));
  ثابت size_t اوږدتیا = CFData_Get_اوږدتیا (cf_معلومات);
  إذا (!معلومات || !اوږدتیا)
    أرجع تش_اشاره;

  أرجع hb_blob_create (معلومات, اوږدتیا, HB_حافظه_ډېرپېښ_READONLY,
			 reinterpret_cast<فراغ *> (const_cast<__CFData *> (cf_معلومات.release ())),
			 release_جدول_معلومات);
}

ساكن بینښه
hb_cg_get_جدول_نښانونه (ثابت hb_face_t *face HB_ناکارول_شوی,
		       بینښه صحيح پیل_دننه_واټن,
		       بینښه صحيح *جدول_شمېر_2,
		       hb_نښان_t *جدول_نښانونه,
		       فراغ *user_معلومات)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_معلومات);
  إذا (unlikely (!cg_font))
  {
    إذا (جدول_شمېر_2)
      *جدول_شمېر_2 = 0;
    أرجع 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_اندازه));
  إذا (unlikely (!ct_font))
  {
    إذا (جدول_شمېر_2)
      *جدول_شمېر_2 = 0;
    أرجع 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFont_Copy_شته_Tables (ct_font, k_CTFont_جدول_Option_No_Options));
  إذا (unlikely (!arr))
  {
    إذا (جدول_شمېر_2)
      *جدول_شمېر_2 = 0;
    أرجع 0;
  }

  بینښه population = (بینښه) CFArray_Get_شمېر (arr);

  إذا (!جدول_شمېر_2)
    أرجع population;

  إذا (unlikely (پیل_دننه_واټن >= population))
  {
    *جدول_شمېر_2 = 0;
    أرجع population;
  }

  بینښه پای_ټکی_دننه_واټن = پیل_دننه_واټن + *جدول_شمېر_2;
  إذا (unlikely (پای_ټکی_دننه_واټن < پیل_دننه_واټن))
  {
    *جدول_شمېر_2 = 0;
    أرجع population;
  }
  پای_ټکی_دننه_واټن = hb_min (پای_ټکی_دننه_واټن, (بینښه) population);

  *جدول_شمېر_2 = پای_ټکی_دننه_واټن - پیل_دننه_واټن;
  إذا (جدول_نښانونه)
    لكل (بینښه i = پیل_دننه_واټن; i < پای_ټکی_دننه_واټن; i++)
    {
      CTFont_جدول_نښان نښان_2 = (CTFont_جدول_نښان)(uintptr_t) CFArray_Get_ارزښت_At_شاخص (arr, i);
      جدول_نښانونه[i - پیل_دننه_واټن] = نښان_2;
    }

  أرجع population;
}

ساكن فراغ
_hb_cg_font_release (فراغ *معلومات)
{
  إذا (معلومات)
    CGFontRelease ((CGFontRef) معلومات);
}


ساكن CTFontDescriptorRef
get_وروستی_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> وروستی_resort (CTFont_Descriptor_Create_With_Name_And_اندازه (CFSTR("LastResort"), 0));
  إذا (unlikely (!وروستی_resort))
    أرجع تش_اشاره;

  ثابت فراغ *cascade_elements[] = { وروستی_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  إذا (unlikely (!cascade_list))
    أرجع تش_اشاره;

  ثابت فراغ *کلیګانې[] = { kCTFontCascadeListAttribute };
  ثابت فراغ *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> ځانګړتیاوې (CFDictionaryCreate (kCFAllocatorDefault,
							       کلیګانې,
							       values,
							       1,
							       &k_CFType_Dictionary_کلی_Call_Backs,
							       &k_CFType_Dictionary_ارزښت_Call_Backs));
  إذا (unlikely (!ځانګړتیاوې))
    أرجع تش_اشاره;

  أرجع CTFont_Descriptor_Create_With_ځانګړتیاوې (ځانګړتیاوې);
}

ساكن فراغ
release_معلومات (فراغ *info, ثابت فراغ *معلومات, size_t اندازه)
{
  أكد (hb_blob_get_اوږدتیا ((hb_blob_t *) info) == اندازه &&
	  hb_blob_get_معلومات ((hb_blob_t *) info, تش_اشاره) == معلومات);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, بینښه صحيح named_instance_شاخص)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  إذا (unlikely (!desc_array))
    أرجع تش_اشاره;

  إذا (named_instance_شاخص == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  وإلا
    named_instance_شاخص--;
  خپلکاری ct_font_desc = (CFArray_Get_شمېر (desc_array) > (CFIndex) named_instance_شاخص) ?
		      (CTFontDescriptorRef) CFArray_Get_ارزښت_At_شاخص (desc_array, (CFIndex) named_instance_شاخص) : تش_اشاره;
  إذا (unlikely (!ct_font_desc))
    أرجع تش_اشاره;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, تش_اشاره));
  إذا (unlikely (!ct_font))
    أرجع تش_اشاره;

  أرجع CTFontCopyGraphicsFont (ct_font, تش_اشاره);
}

CGFontRef
create_cg_font (hb_blob_t *blob, بینښه صحيح شاخص_2)
{
  hb_blob_make_immutable (blob);
  بینښه صحيح blob_اوږدتیا;
  ثابت حرف *blob_معلومات = hb_blob_get_معلومات (blob, &blob_اوږدتیا);
  إذا (unlikely (!blob_اوږدتیا))
    DEBUG_پیغام (CORETEXT, blob, "Empty blob");

  بینښه ttc_شاخص = شاخص_2 & 0xFFFF;
  بینښه named_instance_شاخص = شاخص_2 >> 16;

  إذا (ttc_شاخص != 0)
  {
    DEBUG_پیغام (CORETEXT, blob, "TTC index %u not supported", ttc_شاخص);
    أرجع تش_اشاره; // CoreText does not support TTCs
  }

  إذا (unlikely (named_instance_شاخص != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#إذا (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> معلومات (CFDataCreate (kCFAllocatorDefault, (ثابت UInt8 *) blob_معلومات, blob_اوږدتیا));
    إذا (معلومات)
    {
      خپلکاری ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_معلومات (معلومات);
      إذا (likely (ct_font_desc_array))
	أرجع create_cg_font (ct_font_desc_array, named_instance_شاخص);
    }
#د_که_پای
    أرجع تش_اشاره;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_معلومات (blob, blob_معلومات, blob_اوږدتیا, &release_معلومات));
  إذا (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    أرجع تش_اشاره;
  }
  CGFontRef cg_font = CGFont_Create_With_معلومات_Provider (provider);
  إذا (unlikely (!cg_font))
    DEBUG_پیغام (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  أرجع cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = تش_اشاره;
  إذا (unlikely (!face))
    أرجع تش_اشاره;
  إذا (face->destroy == _hb_cg_font_release)
    cg_font = face->user_معلومات ? CGFontRetain ((CGFontRef) face->user_معلومات) : تش_اشاره;
  وإلا
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->شاخص_2);
    hb_blob_destroy (blob);
  }
  أرجع cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_اندازه)
{
  إذا (unlikely (!cg_font))
    أرجع تش_اشاره;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  إذا (cg_postscript_name)
  {
    إذا (CFString_Has_مختاړی (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFString_Has_مختاړی (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#إذا !(defined(موخه_OS_IPHONE) && موخه_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#د_که_پای
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      إذا (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_اندازه, تش_اشاره));
      إذا (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_پایله_name (CTFontCopyPostScriptName (ct_font));
	إذا (!ct_پایله_name || CFStringCompare (ct_پایله_name, cg_postscript_name, 0) != k_CFCompare_Equal_منزل)
	  ct_font.reset ();
      }
    }
  }

  إذا (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_اندازه, تش_اشاره, تش_اشاره));

  إذا (unlikely (!ct_font)) {
    DEBUG_پیغام (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    أرجع تش_اشاره;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#لارښوونه GCC diagnostic push
#لارښوونه GCC diagnostic ignored "-Wdeprecated-declarations"
  إذا (&CTGet_Core_متن_نسخه != تش_اشاره && CTGet_Core_متن_نسخه() < 0x00070000) {
#لارښوونه GCC diagnostic راوباسه
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    منطقي isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == k_CFCompare_Equal_منزل;
    إذا (!isEmojiFont)
      أرجع ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#إذا !(defined(موخه_OS_IPHONE) && موخه_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus حالت;
  atsFont = CTFontGetPlatformFont (ct_font, عدم);
  حالت = ATSFontGetFileReference (atsFont, &fsref);
  إذا (حالت == noErr)
    original_url.reset (CFURLCreateFromFSRef (عدم, &fsref));
#وإلا
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#د_که_پای

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> وروستی_resort_font_desc (get_وروستی_resort_font_desc ());
    إذا (وروستی_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFont_Create_Copy_With_ځانګړتیاوې (ct_font, 0.0, تش_اشاره, وروستی_resort_font_desc));
      إذا (new_ct_font)
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
#إذا !(defined(موخه_OS_IPHONE) && موخه_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, عدم);
	حالت = ATSFontGetFileReference (atsFont, &fsref);
	إذا (حالت == noErr)
	  new_url.reset (CFURLCreateFromFSRef (عدم, &fsref));
#وإلا
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#د_که_پای
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	إذا (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} وإلا {
	  DEBUG_پیغام (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      وإلا
	DEBUG_پیغام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    وإلا
      DEBUG_پیغام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  أرجع ct_font.release ();
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
  إذا (unlikely (!cg_font))
    أرجع hb_face_get_تشه ();

  hb_face_t *face = hb_face_create_for_tables (hb_cg_reference_جدول, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_سېټ_get_جدول_نښانونه_func (face, hb_cg_get_جدول_نښانونه, cg_font, تش_اشاره);
  أرجع face;
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
hb_coretext_face_create_from_file_or_fail (ثابت حرف   *file_name,
					   بینښه صحيح  شاخص_2)
{
  بینښه ttc_شاخص = شاخص_2 & 0xFFFF;
  بینښه named_instance_شاخص = شاخص_2 >> 16;

  إذا (ttc_شاخص != 0)
  {
    DEBUG_پیغام (CORETEXT, تش_اشاره, "TTC index %u not supported", ttc_شاخص);
    أرجع تش_اشاره; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (تش_اشاره,
								      (ثابت UInt8 *) file_name,
								      مزي_اوږدوالی (file_name),
								      خطأ));
  إذا (unlikely (!url))
    أرجع تش_اشاره;

  خپلکاری ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  إذا (unlikely (!ct_font_desc_array))
    أرجع تش_اشاره;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_شاخص));
  إذا (unlikely (!cg_font))
    أرجع تش_اشاره;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  إذا (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    أرجع تش_اشاره;
  }

  hb_face_سېټ_شاخص (face, شاخص_2);

  أرجع face;
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
					   بینښه صحيح  شاخص_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, شاخص_2));
  إذا (unlikely (!cg_font))
    أرجع تش_اشاره;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  إذا (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    أرجع تش_اشاره;
  }

  hb_face_سېټ_شاخص (face, شاخص_2);

  أرجع face;
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
  أرجع face ? (CGFontRef) (ثابت فراغ *) face->معلومات.coretext : تش_اشاره;
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
  إذا (unlikely (!ct_font))
    أرجع hb_font_get_تشه ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, تش_اشاره));
  إذا (unlikely (!cg_font))
    أرجع hb_font_get_تشه ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع font;

  hb_font_سېټ_ptem (font, CTFont_Get_اندازه (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  إذا (variations)
  {

    hb_ویکټور_t<hb_variation_t> vars;
    hb_ویکټور_t<CFTypeRef> کلیګانې;
    hb_ویکټور_t<CFTypeRef> values;

    CFIndex شمېر = CFDictionary_Get_شمېر (variations);
    إذا (likely (vars.alloc_exact (شمېر) && کلیګانې.resize_exact (شمېر) && values.resize_exact (شمېر)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_کلیګانې_And_ارزښتونه (variations, کلیګانې.arrayZ, values.arrayZ);
      لكل (CFIndex i = 0; i < شمېر; i++)
      {
	صحيح نښان_2;
	عائم value;
	CFNumber_Get_ارزښت ((CFNumberRef) کلیګانې.arrayZ[i], kCFNumberIntType, &نښان_2);
	CFNumber_Get_ارزښت ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_نښان_t) نښان_2, value};
	vars.push (var);
      }
      hb_font_سېټ_variations (font, vars.arrayZ, vars.اوږدتیا);
    }
  }

  /* Let there be dragons here... */
  font->معلومات.coretext.cmpexch (تش_اشاره, (hb_coretext_font_معلومات_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  أرجع font;
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
  أرجع font ? (CTFontRef) (ثابت فراغ *) font->معلومات.coretext : تش_اشاره;
}


#د_که_پای
