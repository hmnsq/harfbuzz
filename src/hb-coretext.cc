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

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_CORETEXT

#شامل "hb-shaper-impl.hh"

#شامل "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

ساکن خالی
release_جدول_ڈیٹا (خالی *user_ڈیٹا)
{
  CFDataRef cf_ڈیٹا = reinterpret_cast<CFDataRef> (user_ڈیٹا);
  CFRelease(cf_ڈیٹا);
}

ساکن hb_blob_t *
hb_cg_reference_جدول (hb_face_t *face HB_غیر_مستعمل, hb_لیبل_t لیبل_2, خالی *user_ڈیٹا)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_ڈیٹا);
  اگر (unlikely (!cg_font))
    واپس خالی_اشاریہ;

  hb_cf_ptr_t<CFDataRef> cf_ڈیٹا (CGFont_Copy_جدول_For_لیبل (cg_font, لیبل_2));
  اگر (unlikely (!cf_ڈیٹا))
    واپس خالی_اشاریہ;

  ثابت حرف *ڈیٹا = reinterpret_cast<ثابت حرف*> (CFDataGetBytePtr (cf_ڈیٹا));
  ثابت size_t لمبائی = CFData_Get_لمبائی (cf_ڈیٹا);
  اگر (!ڈیٹا || !لمبائی)
    واپس خالی_اشاریہ;

  واپس hb_blob_create (ڈیٹا, لمبائی, HB_حافظہ_منوال_READONLY,
			 reinterpret_cast<خالی *> (const_cast<__CFData *> (cf_ڈیٹا.release ())),
			 release_جدول_ڈیٹا);
}

ساکن غیرعلامتی
hb_cg_get_جدول_لیبل_فہرست (ثابت hb_face_t *face HB_غیر_مستعمل,
		       غیرعلامتی صحیح آغاز_انحراف,
		       غیرعلامتی صحیح *جدول_گنتی_2,
		       hb_لیبل_t *جدول_لیبل_فہرست,
		       خالی *user_ڈیٹا)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_ڈیٹا);
  اگر (unlikely (!cg_font))
  {
    اگر (جدول_گنتی_2)
      *جدول_گنتی_2 = 0;
    واپس 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_سائز));
  اگر (unlikely (!ct_font))
  {
    اگر (جدول_گنتی_2)
      *جدول_گنتی_2 = 0;
    واپس 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFont_Copy_دستیاب_Tables (ct_font, k_CTFont_جدول_Option_No_Options));
  اگر (unlikely (!arr))
  {
    اگر (جدول_گنتی_2)
      *جدول_گنتی_2 = 0;
    واپس 0;
  }

  غیرعلامتی population = (غیرعلامتی) CFArray_Get_گنتی (arr);

  اگر (!جدول_گنتی_2)
    واپس population;

  اگر (unlikely (آغاز_انحراف >= population))
  {
    *جدول_گنتی_2 = 0;
    واپس population;
  }

  غیرعلامتی انتہا_انحراف = آغاز_انحراف + *جدول_گنتی_2;
  اگر (unlikely (انتہا_انحراف < آغاز_انحراف))
  {
    *جدول_گنتی_2 = 0;
    واپس population;
  }
  انتہا_انحراف = hb_min (انتہا_انحراف, (غیرعلامتی) population);

  *جدول_گنتی_2 = انتہا_انحراف - آغاز_انحراف;
  اگر (جدول_لیبل_فہرست)
    برائے (غیرعلامتی i = آغاز_انحراف; i < انتہا_انحراف; i++)
    {
      CTFont_جدول_لیبل لیبل_2 = (CTFont_جدول_لیبل)(uintptr_t) CFArray_Get_قدر_At_اشاریہ (arr, i);
      جدول_لیبل_فہرست[i - آغاز_انحراف] = لیبل_2;
    }

  واپس population;
}

ساکن خالی
_hb_cg_font_release (خالی *ڈیٹا)
{
  اگر (ڈیٹا)
    CGFontRelease ((CGFontRef) ڈیٹا);
}


ساکن CTFontDescriptorRef
get_آخری_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> آخری_resort (CTFont_Descriptor_Create_With_Name_And_سائز (CFSTR("LastResort"), 0));
  اگر (unlikely (!آخری_resort))
    واپس خالی_اشاریہ;

  ثابت خالی *cascade_elements[] = { آخری_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  اگر (unlikely (!cascade_list))
    واپس خالی_اشاریہ;

  ثابت خالی *کلیدیں[] = { kCTFontCascadeListAttribute };
  ثابت خالی *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> اوصاف (CFDictionaryCreate (kCFAllocatorDefault,
							       کلیدیں,
							       values,
							       1,
							       &k_CFType_Dictionary_کلید_Call_Backs,
							       &k_CFType_Dictionary_قدر_Call_Backs));
  اگر (unlikely (!اوصاف))
    واپس خالی_اشاریہ;

  واپس CTFont_Descriptor_Create_With_اوصاف (اوصاف);
}

ساکن خالی
release_ڈیٹا (خالی *info, ثابت خالی *ڈیٹا, size_t سائز)
{
  أكد (hb_blob_get_لمبائی ((hb_blob_t *) info) == سائز &&
	  hb_blob_get_ڈیٹا ((hb_blob_t *) info, خالی_اشاریہ) == ڈیٹا);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, غیرعلامتی صحیح named_instance_اشاریہ)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  اگر (unlikely (!desc_array))
    واپس خالی_اشاریہ;

  اگر (named_instance_اشاریہ == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  ورنہ
    named_instance_اشاریہ--;
  خودکار ct_font_desc = (CFArray_Get_گنتی (desc_array) > (CFIndex) named_instance_اشاریہ) ?
		      (CTFontDescriptorRef) CFArray_Get_قدر_At_اشاریہ (desc_array, (CFIndex) named_instance_اشاریہ) : خالی_اشاریہ;
  اگر (unlikely (!ct_font_desc))
    واپس خالی_اشاریہ;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, خالی_اشاریہ));
  اگر (unlikely (!ct_font))
    واپس خالی_اشاریہ;

  واپس CTFontCopyGraphicsFont (ct_font, خالی_اشاریہ);
}

CGFontRef
create_cg_font (hb_blob_t *blob, غیرعلامتی صحیح اشاریہ_2)
{
  hb_blob_make_immutable (blob);
  غیرعلامتی صحیح blob_لمبائی;
  ثابت حرف *blob_ڈیٹا = hb_blob_get_ڈیٹا (blob, &blob_لمبائی);
  اگر (unlikely (!blob_لمبائی))
    DEBUG_پیغام (CORETEXT, blob, "Empty blob");

  غیرعلامتی ttc_اشاریہ = اشاریہ_2 & 0xFFFF;
  غیرعلامتی named_instance_اشاریہ = اشاریہ_2 >> 16;

  اگر (ttc_اشاریہ != 0)
  {
    DEBUG_پیغام (CORETEXT, blob, "TTC index %u not supported", ttc_اشاریہ);
    واپس خالی_اشاریہ; // CoreText does not support TTCs
  }

  اگر (unlikely (named_instance_اشاریہ != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#اگر (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> ڈیٹا (CFDataCreate (kCFAllocatorDefault, (ثابت UInt8 *) blob_ڈیٹا, blob_لمبائی));
    اگر (ڈیٹا)
    {
      خودکار ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_ڈیٹا (ڈیٹا);
      اگر (likely (ct_font_desc_array))
	واپس create_cg_font (ct_font_desc_array, named_instance_اشاریہ);
    }
#اگر_ختم
    واپس خالی_اشاریہ;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_ڈیٹا (blob, blob_ڈیٹا, blob_لمبائی, &release_ڈیٹا));
  اگر (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    واپس خالی_اشاریہ;
  }
  CGFontRef cg_font = CGFont_Create_With_ڈیٹا_Provider (provider);
  اگر (unlikely (!cg_font))
    DEBUG_پیغام (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  واپس cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = خالی_اشاریہ;
  اگر (unlikely (!face))
    واپس خالی_اشاریہ;
  اگر (face->destroy == _hb_cg_font_release)
    cg_font = face->user_ڈیٹا ? CGFontRetain ((CGFontRef) face->user_ڈیٹا) : خالی_اشاریہ;
  ورنہ
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->اشاریہ_2);
    hb_blob_destroy (blob);
  }
  واپس cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_سائز)
{
  اگر (unlikely (!cg_font))
    واپس خالی_اشاریہ;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  اگر (cg_postscript_name)
  {
    اگر (CFString_Has_سابقہ (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFString_Has_سابقہ (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#اگر !(defined(ہدف_OS_IPHONE) && ہدف_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#اگر_ختم
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      اگر (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_سائز, خالی_اشاریہ));
      اگر (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_نتیجہ_name (CTFontCopyPostScriptName (ct_font));
	اگر (!ct_نتیجہ_name || CFStringCompare (ct_نتیجہ_name, cg_postscript_name, 0) != k_CFCompare_Equal_مقصد)
	  ct_font.reset ();
      }
    }
  }

  اگر (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_سائز, خالی_اشاریہ, خالی_اشاریہ));

  اگر (unlikely (!ct_font)) {
    DEBUG_پیغام (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    واپس خالی_اشاریہ;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#ہدایت GCC diagnostic push
#ہدایت GCC diagnostic ignored "-Wdeprecated-declarations"
  اگر (&CTGet_Core_متن_نسخہ != خالی_اشاریہ && CTGet_Core_متن_نسخہ() < 0x00070000) {
#ہدایت GCC diagnostic نکالو
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    منطقی isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == k_CFCompare_Equal_مقصد;
    اگر (!isEmojiFont)
      واپس ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#اگر !(defined(ہدف_OS_IPHONE) && ہدف_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus حالت_2;
  atsFont = CTFontGetPlatformFont (ct_font, تہی);
  حالت_2 = ATSFontGetFileReference (atsFont, &fsref);
  اگر (حالت_2 == noErr)
    original_url.reset (CFURLCreateFromFSRef (تہی, &fsref));
#ورنہ
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#اگر_ختم

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> آخری_resort_font_desc (get_آخری_resort_font_desc ());
    اگر (آخری_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFont_Create_Copy_With_اوصاف (ct_font, 0.0, خالی_اشاریہ, آخری_resort_font_desc));
      اگر (new_ct_font)
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
#اگر !(defined(ہدف_OS_IPHONE) && ہدف_OS_IPHONE) && MAC_OS_X_نسخہ_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, تہی);
	حالت_2 = ATSFontGetFileReference (atsFont, &fsref);
	اگر (حالت_2 == noErr)
	  new_url.reset (CFURLCreateFromFSRef (تہی, &fsref));
#ورنہ
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#اگر_ختم
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	اگر (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} ورنہ {
	  DEBUG_پیغام (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      ورنہ
	DEBUG_پیغام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    ورنہ
      DEBUG_پیغام (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  واپس ct_font.release ();
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
  اگر (unlikely (!cg_font))
    واپس hb_face_get_خالی_خانے ();

  hb_face_t *face = hb_face_create_for_tables (hb_cg_reference_جدول, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_سیٹ_get_جدول_لیبل_فہرست_func (face, hb_cg_get_جدول_لیبل_فہرست, cg_font, خالی_اشاریہ);
  واپس face;
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
					   غیرعلامتی صحیح  اشاریہ_2)
{
  غیرعلامتی ttc_اشاریہ = اشاریہ_2 & 0xFFFF;
  غیرعلامتی named_instance_اشاریہ = اشاریہ_2 >> 16;

  اگر (ttc_اشاریہ != 0)
  {
    DEBUG_پیغام (CORETEXT, خالی_اشاریہ, "TTC index %u not supported", ttc_اشاریہ);
    واپس خالی_اشاریہ; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (خالی_اشاریہ,
								      (ثابت UInt8 *) file_name,
								      لڑی_لمبائی (file_name),
								      جھوٹ));
  اگر (unlikely (!url))
    واپس خالی_اشاریہ;

  خودکار ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  اگر (unlikely (!ct_font_desc_array))
    واپس خالی_اشاریہ;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_اشاریہ));
  اگر (unlikely (!cg_font))
    واپس خالی_اشاریہ;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  اگر (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    واپس خالی_اشاریہ;
  }

  hb_face_سیٹ_اشاریہ (face, اشاریہ_2);

  واپس face;
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
					   غیرعلامتی صحیح  اشاریہ_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, اشاریہ_2));
  اگر (unlikely (!cg_font))
    واپس خالی_اشاریہ;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  اگر (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    واپس خالی_اشاریہ;
  }

  hb_face_سیٹ_اشاریہ (face, اشاریہ_2);

  واپس face;
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
  واپس face ? (CGFontRef) (ثابت خالی *) face->ڈیٹا.coretext : خالی_اشاریہ;
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
  اگر (unlikely (!ct_font))
    واپس hb_font_get_خالی_خانے ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, خالی_اشاریہ));
  اگر (unlikely (!cg_font))
    واپس hb_font_get_خالی_خانے ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  اگر (unlikely (hb_object_is_immutable (font)))
    واپس font;

  hb_font_سیٹ_ptem (font, CTFont_Get_سائز (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  اگر (variations)
  {

    hb_سمتیہ_t<hb_variation_t> vars;
    hb_سمتیہ_t<CFTypeRef> کلیدیں;
    hb_سمتیہ_t<CFTypeRef> values;

    CFIndex گنتی = CFDictionary_Get_گنتی (variations);
    اگر (likely (vars.alloc_exact (گنتی) && کلیدیں.resize_exact (گنتی) && values.resize_exact (گنتی)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_کلیدیں_And_اقدار (variations, کلیدیں.arrayZ, values.arrayZ);
      برائے (CFIndex i = 0; i < گنتی; i++)
      {
	صحیح لیبل_2;
	اعشاری value;
	CFNumber_Get_قدر ((CFNumberRef) کلیدیں.arrayZ[i], kCFNumberIntType, &لیبل_2);
	CFNumber_Get_قدر ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_لیبل_t) لیبل_2, value};
	vars.push (var);
      }
      hb_font_سیٹ_variations (font, vars.arrayZ, vars.لمبائی);
    }
  }

  /* Let there be dragons here... */
  font->ڈیٹا.coretext.cmpexch (خالی_اشاریہ, (hb_coretext_font_ڈیٹا_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  واپس font;
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
  واپس font ? (CTFontRef) (ثابت خالی *) font->ڈیٹا.coretext : خالی_اشاریہ;
}


#اگر_ختم
