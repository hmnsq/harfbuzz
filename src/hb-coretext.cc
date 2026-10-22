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

#اگر_تعریف_شده HAVE_CORETEXT

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
release_جدول_داده (فراغ *user_داده)
{
  CFDataRef cf_داده = reinterpret_cast<CFDataRef> (user_داده);
  CFRelease(cf_داده);
}

ساكن hb_blob_t *
hb_cg_reference_جدول (hb_face_t *face HB_استفادهنشده, hb_برچسب_t برچسب_2, فراغ *user_داده)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_داده);
  إذا (unlikely (!cg_font))
    أرجع اشاره_تهی;

  hb_cf_ptr_t<CFDataRef> cf_داده (CGFont_Copy_جدول_For_برچسب (cg_font, برچسب_2));
  إذا (unlikely (!cf_داده))
    أرجع اشاره_تهی;

  ثابت حرف *داده = reinterpret_cast<ثابت حرف*> (CFDataGetBytePtr (cf_داده));
  ثابت size_t درازی = CFData_Get_درازی (cf_داده);
  إذا (!داده || !درازی)
    أرجع اشاره_تهی;

  أرجع hb_blob_create (داده, درازی, HB_حافظه_نما_READONLY,
			 reinterpret_cast<فراغ *> (const_cast<__CFData *> (cf_داده.release ())),
			 release_جدول_داده);
}

ساكن بیعلامت
hb_cg_get_جدول_برچسبها (ثابت hb_face_t *face HB_استفادهنشده,
		       بیعلامت صحيح شروع_جابه_جایی,
		       بیعلامت صحيح *جدول_شمار_2,
		       hb_برچسب_t *جدول_برچسبها,
		       فراغ *user_داده)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_داده);
  إذا (unlikely (!cg_font))
  {
    إذا (جدول_شمار_2)
      *جدول_شمار_2 = 0;
    أرجع 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_اندازه));
  إذا (unlikely (!ct_font))
  {
    إذا (جدول_شمار_2)
      *جدول_شمار_2 = 0;
    أرجع 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFont_Copy_موجود_Tables (ct_font, k_CTFont_جدول_Option_No_Options));
  إذا (unlikely (!arr))
  {
    إذا (جدول_شمار_2)
      *جدول_شمار_2 = 0;
    أرجع 0;
  }

  بیعلامت population = (بیعلامت) CFArray_Get_شمار (arr);

  إذا (!جدول_شمار_2)
    أرجع population;

  إذا (unlikely (شروع_جابه_جایی >= population))
  {
    *جدول_شمار_2 = 0;
    أرجع population;
  }

  بیعلامت انتها_جابه_جایی = شروع_جابه_جایی + *جدول_شمار_2;
  إذا (unlikely (انتها_جابه_جایی < شروع_جابه_جایی))
  {
    *جدول_شمار_2 = 0;
    أرجع population;
  }
  انتها_جابه_جایی = hb_min (انتها_جابه_جایی, (بیعلامت) population);

  *جدول_شمار_2 = انتها_جابه_جایی - شروع_جابه_جایی;
  إذا (جدول_برچسبها)
    لكل (بیعلامت i = شروع_جابه_جایی; i < انتها_جابه_جایی; i++)
    {
      CTFont_جدول_برچسب برچسب_2 = (CTFont_جدول_برچسب)(uintptr_t) CFArray_Get_مقدار_At_شاخص (arr, i);
      جدول_برچسبها[i - شروع_جابه_جایی] = برچسب_2;
    }

  أرجع population;
}

ساكن فراغ
_hb_cg_font_release (فراغ *داده)
{
  إذا (داده)
    CGFontRelease ((CGFontRef) داده);
}


ساكن CTFontDescriptorRef
get_آخرین_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> آخرین_resort (CTFont_Descriptor_Create_With_Name_And_اندازه (CFSTR("LastResort"), 0));
  إذا (unlikely (!آخرین_resort))
    أرجع اشاره_تهی;

  ثابت فراغ *cascade_elements[] = { آخرین_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  إذا (unlikely (!cascade_list))
    أرجع اشاره_تهی;

  ثابت فراغ *کلیدها[] = { kCTFontCascadeListAttribute };
  ثابت فراغ *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> صفات (CFDictionaryCreate (kCFAllocatorDefault,
							       کلیدها,
							       values,
							       1,
							       &k_CFType_Dictionary_کلید_Call_Backs,
							       &k_CFType_Dictionary_مقدار_Call_Backs));
  إذا (unlikely (!صفات))
    أرجع اشاره_تهی;

  أرجع CTFont_Descriptor_Create_With_صفات (صفات);
}

ساكن فراغ
release_داده (فراغ *info, ثابت فراغ *داده, size_t اندازه)
{
  أكد (hb_blob_get_درازی ((hb_blob_t *) info) == اندازه &&
	  hb_blob_get_داده ((hb_blob_t *) info, اشاره_تهی) == داده);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, بیعلامت صحيح named_instance_شاخص)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  إذا (unlikely (!desc_array))
    أرجع اشاره_تهی;

  إذا (named_instance_شاخص == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  وإلا
    named_instance_شاخص--;
  خودکار ct_font_desc = (CFArray_Get_شمار (desc_array) > (CFIndex) named_instance_شاخص) ?
		      (CTFontDescriptorRef) CFArray_Get_مقدار_At_شاخص (desc_array, (CFIndex) named_instance_شاخص) : اشاره_تهی;
  إذا (unlikely (!ct_font_desc))
    أرجع اشاره_تهی;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, اشاره_تهی));
  إذا (unlikely (!ct_font))
    أرجع اشاره_تهی;

  أرجع CTFontCopyGraphicsFont (ct_font, اشاره_تهی);
}

CGFontRef
create_cg_font (hb_blob_t *blob, بیعلامت صحيح شاخص_2)
{
  hb_blob_make_immutable (blob);
  بیعلامت صحيح blob_درازی;
  ثابت حرف *blob_داده = hb_blob_get_داده (blob, &blob_درازی);
  إذا (unlikely (!blob_درازی))
    DEBUG_پیام (CORETEXT, blob, "Empty blob");

  بیعلامت ttc_شاخص = شاخص_2 & 0xFFFF;
  بیعلامت named_instance_شاخص = شاخص_2 >> 16;

  إذا (ttc_شاخص != 0)
  {
    DEBUG_پیام (CORETEXT, blob, "TTC index %u not supported", ttc_شاخص);
    أرجع اشاره_تهی; // CoreText does not support TTCs
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
    hb_cf_ptr_t<CFDataRef> داده (CFDataCreate (kCFAllocatorDefault, (ثابت UInt8 *) blob_داده, blob_درازی));
    إذا (داده)
    {
      خودکار ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_داده (داده);
      إذا (likely (ct_font_desc_array))
	أرجع create_cg_font (ct_font_desc_array, named_instance_شاخص);
    }
#پایان_اگر
    أرجع اشاره_تهی;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_داده (blob, blob_داده, blob_درازی, &release_داده));
  إذا (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    أرجع اشاره_تهی;
  }
  CGFontRef cg_font = CGFont_Create_With_داده_Provider (provider);
  إذا (unlikely (!cg_font))
    DEBUG_پیام (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  أرجع cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = اشاره_تهی;
  إذا (unlikely (!face))
    أرجع اشاره_تهی;
  إذا (face->destroy == _hb_cg_font_release)
    cg_font = face->user_داده ? CGFontRetain ((CGFontRef) face->user_داده) : اشاره_تهی;
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
    أرجع اشاره_تهی;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  إذا (cg_postscript_name)
  {
    إذا (CFString_Has_پیشوند (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFString_Has_پیشوند (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#پایان_اگر
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      إذا (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_اندازه, اشاره_تهی));
      إذا (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_نتیجه_name (CTFontCopyPostScriptName (ct_font));
	إذا (!ct_نتیجه_name || CFStringCompare (ct_نتیجه_name, cg_postscript_name, 0) != k_CFCompare_Equal_مقصد)
	  ct_font.reset ();
      }
    }
  }

  إذا (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_اندازه, اشاره_تهی, اشاره_تهی));

  إذا (unlikely (!ct_font)) {
    DEBUG_پیام (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    أرجع اشاره_تهی;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#رهنمود GCC diagnostic push
#رهنمود GCC diagnostic ignored "-Wdeprecated-declarations"
  إذا (&CTGet_Core_متن_نسخه != اشاره_تهی && CTGet_Core_متن_نسخه() < 0x00070000) {
#رهنمود GCC diagnostic برداشت
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    منطقي isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == k_CFCompare_Equal_مقصد;
    إذا (!isEmojiFont)
      أرجع ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus وضعیت;
  atsFont = CTFontGetPlatformFont (ct_font, عدم);
  وضعیت = ATSFontGetFileReference (atsFont, &fsref);
  إذا (وضعیت == noErr)
    original_url.reset (CFURLCreateFromFSRef (عدم, &fsref));
#وإلا
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#پایان_اگر

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> آخرین_resort_font_desc (get_آخرین_resort_font_desc ());
    إذا (آخرین_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFont_Create_Copy_With_صفات (ct_font, 0.0, اشاره_تهی, آخرین_resort_font_desc));
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
#إذا !(defined(هدف_OS_IPHONE) && هدف_OS_IPHONE) && MAC_OS_X_نسخه_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, عدم);
	وضعیت = ATSFontGetFileReference (atsFont, &fsref);
	إذا (وضعیت == noErr)
	  new_url.reset (CFURLCreateFromFSRef (عدم, &fsref));
#وإلا
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#پایان_اگر
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	إذا (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} وإلا {
	  DEBUG_پیام (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      وإلا
	DEBUG_پیام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    وإلا
      DEBUG_پیام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
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
    أرجع hb_face_get_خالی ();

  hb_face_t *face = hb_face_create_for_tables (hb_cg_reference_جدول, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_مجموعه_get_جدول_برچسبها_func (face, hb_cg_get_جدول_برچسبها, cg_font, اشاره_تهی);
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
					   بیعلامت صحيح  شاخص_2)
{
  بیعلامت ttc_شاخص = شاخص_2 & 0xFFFF;
  بیعلامت named_instance_شاخص = شاخص_2 >> 16;

  إذا (ttc_شاخص != 0)
  {
    DEBUG_پیام (CORETEXT, اشاره_تهی, "TTC index %u not supported", ttc_شاخص);
    أرجع اشاره_تهی; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (اشاره_تهی,
								      (ثابت UInt8 *) file_name,
								      طول_رشته (file_name),
								      خطأ));
  إذا (unlikely (!url))
    أرجع اشاره_تهی;

  خودکار ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  إذا (unlikely (!ct_font_desc_array))
    أرجع اشاره_تهی;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_شاخص));
  إذا (unlikely (!cg_font))
    أرجع اشاره_تهی;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  إذا (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    أرجع اشاره_تهی;
  }

  hb_face_مجموعه_شاخص (face, شاخص_2);

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
					   بیعلامت صحيح  شاخص_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, شاخص_2));
  إذا (unlikely (!cg_font))
    أرجع اشاره_تهی;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  إذا (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    أرجع اشاره_تهی;
  }

  hb_face_مجموعه_شاخص (face, شاخص_2);

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
  أرجع face ? (CGFontRef) (ثابت فراغ *) face->داده.coretext : اشاره_تهی;
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
    أرجع hb_font_get_خالی ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, اشاره_تهی));
  إذا (unlikely (!cg_font))
    أرجع hb_font_get_خالی ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  إذا (unlikely (hb_object_is_immutable (font)))
    أرجع font;

  hb_font_مجموعه_ptem (font, CTFont_Get_اندازه (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  إذا (variations)
  {

    hb_وکتور_t<hb_variation_t> vars;
    hb_وکتور_t<CFTypeRef> کلیدها;
    hb_وکتور_t<CFTypeRef> values;

    CFIndex شمار = CFDictionary_Get_شمار (variations);
    إذا (likely (vars.alloc_exact (شمار) && کلیدها.resize_exact (شمار) && values.resize_exact (شمار)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_کلیدها_And_مقادیر (variations, کلیدها.arrayZ, values.arrayZ);
      لكل (CFIndex i = 0; i < شمار; i++)
      {
	صحيح برچسب_2;
	عائم value;
	CFNumber_Get_مقدار ((CFNumberRef) کلیدها.arrayZ[i], kCFNumberIntType, &برچسب_2);
	CFNumber_Get_مقدار ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_برچسب_t) برچسب_2, value};
	vars.push (var);
      }
      hb_font_مجموعه_variations (font, vars.arrayZ, vars.درازی);
    }
  }

  /* Let there be dragons here... */
  font->داده.coretext.cmpexch (اشاره_تهی, (hb_coretext_font_داده_t *) CFRetain (ct_font));

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
  أرجع font ? (CTFontRef) (ثابت فراغ *) font->داده.coretext : اشاره_تهی;
}


#پایان_اگر
