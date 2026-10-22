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


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

ਸਥਿਰ ਖਾਲੀ
release_ਸਾਰਣੀ_ਅੰਕੜੇ (ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  CFDataRef cf_ਅੰਕੜੇ = reinterpret_cast<CFDataRef> (user_ਅੰਕੜੇ);
  CFRelease(cf_ਅੰਕੜੇ);
}

ਸਥਿਰ hb_blob_t *
hb_cg_reference_ਸਾਰਣੀ (hb_face_t *face HB_ਅਣਵਰਤਿਆ, hb_ਟੈਗ_t ਟੈਗ_2, ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_ਅੰਕੜੇ);
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_cf_ptr_t<CFDataRef> cf_ਅੰਕੜੇ (CGFont_Copy_ਸਾਰਣੀ_For_ਟੈਗ (cg_font, ਟੈਗ_2));
  ਜੇ (unlikely (!cf_ਅੰਕੜੇ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ = reinterpret_cast<ਅਚਲ ਅੱਖਰ*> (CFDataGetBytePtr (cf_ਅੰਕੜੇ));
  ਅਚਲ size_t ਲੰਮਾਈ = CFData_Get_ਲੰਮਾਈ (cf_ਅੰਕੜੇ);
  ਜੇ (!ਅੰਕੜੇ || !ਲੰਮਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ hb_blob_create (ਅੰਕੜੇ, ਲੰਮਾਈ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
			 reinterpret_cast<ਖਾਲੀ *> (const_cast<__CFData *> (cf_ਅੰਕੜੇ.release ())),
			 release_ਸਾਰਣੀ_ਅੰਕੜੇ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_cg_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਅਚਲ hb_face_t *face HB_ਅਣਵਰਤਿਆ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2,
		       hb_ਟੈਗ_t *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ,
		       ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_ਅੰਕੜੇ);
  ਜੇ (unlikely (!cg_font))
  {
    ਜੇ (ਸਾਰਣੀ_ਗਿਣਤੀ_2)
      *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_ਮਾਪ));
  ਜੇ (unlikely (!ct_font))
  {
    ਜੇ (ਸਾਰਣੀ_ਗਿਣਤੀ_2)
      *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFont_Copy_ਉਪਲਬਧ_Tables (ct_font, k_CTFont_ਸਾਰਣੀ_Option_No_Options));
  ਜੇ (unlikely (!arr))
  {
    ਜੇ (ਸਾਰਣੀ_ਗਿਣਤੀ_2)
      *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ 0;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ population = (ਬਿਨਾਨਿਸ਼ਾਨੀ) CFArray_Get_ਗਿਣਤੀ (arr);

  ਜੇ (!ਸਾਰਣੀ_ਗਿਣਤੀ_2)
    ਵਾਪਸ population;

  ਜੇ (unlikely (ਸ਼ੁਰੂ_ਵਿੱਥ >= population))
  {
    *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ population;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ_ਵਿੱਥ = ਸ਼ੁਰੂ_ਵਿੱਥ + *ਸਾਰਣੀ_ਗਿਣਤੀ_2;
  ਜੇ (unlikely (ਸਿਰਾ_ਵਿੱਥ < ਸ਼ੁਰੂ_ਵਿੱਥ))
  {
    *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = 0;
    ਵਾਪਸ population;
  }
  ਸਿਰਾ_ਵਿੱਥ = hb_min (ਸਿਰਾ_ਵਿੱਥ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) population);

  *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = ਸਿਰਾ_ਵਿੱਥ - ਸ਼ੁਰੂ_ਵਿੱਥ;
  ਜੇ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਸ਼ੁਰੂ_ਵਿੱਥ; i < ਸਿਰਾ_ਵਿੱਥ; i++)
    {
      CTFont_ਸਾਰਣੀ_ਟੈਗ ਟੈਗ_2 = (CTFont_ਸਾਰਣੀ_ਟੈਗ)(uintptr_t) CFArray_Get_ਮੁੱਲ_At_ਸੂਚਕਾਂਕ (arr, i);
      ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[i - ਸ਼ੁਰੂ_ਵਿੱਥ] = ਟੈਗ_2;
    }

  ਵਾਪਸ population;
}

ਸਥਿਰ ਖਾਲੀ
_hb_cg_font_release (ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਜੇ (ਅੰਕੜੇ)
    CGFontRelease ((CGFontRef) ਅੰਕੜੇ);
}


ਸਥਿਰ CTFontDescriptorRef
get_ਆਖਰੀ_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> ਆਖਰੀ_resort (CTFont_Descriptor_Create_With_Name_And_ਮਾਪ (CFSTR("LastResort"), 0));
  ਜੇ (unlikely (!ਆਖਰੀ_resort))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ ਖਾਲੀ *cascade_elements[] = { ਆਖਰੀ_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  ਜੇ (unlikely (!cascade_list))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀਆਂ[] = { kCTFontCascadeListAttribute };
  ਅਚਲ ਖਾਲੀ *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> ਗੁਣ (CFDictionaryCreate (kCFAllocatorDefault,
							       ਕੁੰਜੀਆਂ,
							       values,
							       1,
							       &k_CFType_Dictionary_ਕੁੰਜੀ_Call_Backs,
							       &k_CFType_Dictionary_ਮੁੱਲ_Call_Backs));
  ਜੇ (unlikely (!ਗੁਣ))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ CTFont_Descriptor_Create_With_ਗੁਣ (ਗੁਣ);
}

ਸਥਿਰ ਖਾਲੀ
release_ਅੰਕੜੇ (ਖਾਲੀ *info, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ, size_t ਮਾਪ)
{
  ਪ੍ਰਤਿਜ੍ਞਾ (hb_blob_get_ਲੰਮਾਈ ((hb_blob_t *) info) == ਮਾਪ &&
	  hb_blob_get_ਅੰਕੜੇ ((hb_blob_t *) info, ਖਾਲੀ_ਸੂਚਕ) == ਅੰਕੜੇ);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ named_instance_ਸੂਚਕਾਂਕ)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  ਜੇ (unlikely (!desc_array))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (named_instance_ਸੂਚਕਾਂਕ == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  ਨਹੀਂਤਾਂ
    named_instance_ਸੂਚਕਾਂਕ--;
  ਸਵੈਚਾਲਿਤ ct_font_desc = (CFArray_Get_ਗਿਣਤੀ (desc_array) > (CFIndex) named_instance_ਸੂਚਕਾਂਕ) ?
		      (CTFontDescriptorRef) CFArray_Get_ਮੁੱਲ_At_ਸੂਚਕਾਂਕ (desc_array, (CFIndex) named_instance_ਸੂਚਕਾਂਕ) : ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (unlikely (!ct_font_desc))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, ਖਾਲੀ_ਸੂਚਕ));
  ਜੇ (unlikely (!ct_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ CTFontCopyGraphicsFont (ct_font, ਖਾਲੀ_ਸੂਚਕ);
}

CGFontRef
create_cg_font (hb_blob_t *blob, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2)
{
  hb_blob_make_immutable (blob);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blob_ਲੰਮਾਈ;
  ਅਚਲ ਅੱਖਰ *blob_ਅੰਕੜੇ = hb_blob_get_ਅੰਕੜੇ (blob, &blob_ਲੰਮਾਈ);
  ਜੇ (unlikely (!blob_ਲੰਮਾਈ))
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, blob, "Empty blob");

  ਬਿਨਾਨਿਸ਼ਾਨੀ ttc_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2 & 0xFFFF;
  ਬਿਨਾਨਿਸ਼ਾਨੀ named_instance_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2 >> 16;

  ਜੇ (ttc_ਸੂਚਕਾਂਕ != 0)
  {
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, blob, "TTC index %u not supported", ttc_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; // CoreText does not support TTCs
  }

  ਜੇ (unlikely (named_instance_ਸੂਚਕਾਂਕ != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#ਜੇ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> ਅੰਕੜੇ (CFDataCreate (kCFAllocatorDefault, (ਅਚਲ UInt8 *) blob_ਅੰਕੜੇ, blob_ਲੰਮਾਈ));
    ਜੇ (ਅੰਕੜੇ)
    {
      ਸਵੈਚਾਲਿਤ ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_ਅੰਕੜੇ (ਅੰਕੜੇ);
      ਜੇ (likely (ct_font_desc_array))
	ਵਾਪਸ create_cg_font (ct_font_desc_array, named_instance_ਸੂਚਕਾਂਕ);
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_ਅੰਕੜੇ (blob, blob_ਅੰਕੜੇ, blob_ਲੰਮਾਈ, &release_ਅੰਕੜੇ));
  ਜੇ (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  CGFontRef cg_font = CGFont_Create_With_ਅੰਕੜੇ_Provider (provider);
  ਜੇ (unlikely (!cg_font))
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  ਵਾਪਸ cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (unlikely (!face))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (face->destroy == _hb_cg_font_release)
    cg_font = face->user_ਅੰਕੜੇ ? CGFontRetain ((CGFontRef) face->user_ਅੰਕੜੇ) : ਖਾਲੀ_ਸੂਚਕ;
  ਨਹੀਂਤਾਂ
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->ਸੂਚਕਾਂਕ_2);
    hb_blob_destroy (blob);
  }
  ਵਾਪਸ cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_ਮਾਪ)
{
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  ਜੇ (cg_postscript_name)
  {
    ਜੇ (CFString_Has_ਅਗੇਤਰ (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFString_Has_ਅਗੇਤਰ (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#ਜੇ !(defined(ਨਿਸ਼ਾਨਾ_OS_IPHONE) && ਨਿਸ਼ਾਨਾ_OS_IPHONE) && MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#ਜੇ_ਅੰਤ
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      ਜੇ (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_ਮਾਪ, ਖਾਲੀ_ਸੂਚਕ));
      ਜੇ (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_ਨਤੀਜਾ_name (CTFontCopyPostScriptName (ct_font));
	ਜੇ (!ct_ਨਤੀਜਾ_name || CFStringCompare (ct_ਨਤੀਜਾ_name, cg_postscript_name, 0) != k_CFCompare_Equal_ਮੰਜ਼ਿਲ)
	  ct_font.reset ();
      }
    }
  }

  ਜੇ (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_ਮਾਪ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ));

  ਜੇ (unlikely (!ct_font)) {
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#ਹਦਾਇਤ GCC diagnostic push
#ਹਦਾਇਤ GCC diagnostic ignored "-Wdeprecated-declarations"
  ਜੇ (&CTGet_Core_ਪਾਠ_ਸੰਸਕਰਣ != ਖਾਲੀ_ਸੂਚਕ && CTGet_Core_ਪਾਠ_ਸੰਸਕਰਣ() < 0x00070000) {
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    ਤਾਰਕਿਕ isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == k_CFCompare_Equal_ਮੰਜ਼ਿਲ;
    ਜੇ (!isEmojiFont)
      ਵਾਪਸ ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#ਜੇ !(defined(ਨਿਸ਼ਾਨਾ_OS_IPHONE) && ਨਿਸ਼ਾਨਾ_OS_IPHONE) && MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus ਸਥਿਤੀ;
  atsFont = CTFontGetPlatformFont (ct_font, ਨਿਰਾ);
  ਸਥਿਤੀ = ATSFontGetFileReference (atsFont, &fsref);
  ਜੇ (ਸਥਿਤੀ == noErr)
    original_url.reset (CFURLCreateFromFSRef (ਨਿਰਾ, &fsref));
#ਨਹੀਂਤਾਂ
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#ਜੇ_ਅੰਤ

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> ਆਖਰੀ_resort_font_desc (get_ਆਖਰੀ_resort_font_desc ());
    ਜੇ (ਆਖਰੀ_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFont_Create_Copy_With_ਗੁਣ (ct_font, 0.0, ਖਾਲੀ_ਸੂਚਕ, ਆਖਰੀ_resort_font_desc));
      ਜੇ (new_ct_font)
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
#ਜੇ !(defined(ਨਿਸ਼ਾਨਾ_OS_IPHONE) && ਨਿਸ਼ਾਨਾ_OS_IPHONE) && MAC_OS_X_ਸੰਸਕਰਣ_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, ਨਿਰਾ);
	ਸਥਿਤੀ = ATSFontGetFileReference (atsFont, &fsref);
	ਜੇ (ਸਥਿਤੀ == noErr)
	  new_url.reset (CFURLCreateFromFSRef (ਨਿਰਾ, &fsref));
#ਨਹੀਂਤਾਂ
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#ਜੇ_ਅੰਤ
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	ਜੇ (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} ਨਹੀਂਤਾਂ {
	  DEBUG_ਸੁਨੇਹਾ (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      ਨਹੀਂਤਾਂ
	DEBUG_ਸੁਨੇਹਾ (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    ਨਹੀਂਤਾਂ
      DEBUG_ਸੁਨੇਹਾ (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  ਵਾਪਸ ct_font.release ();
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
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ hb_face_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_face_t *face = hb_face_create_for_tables (hb_cg_reference_ਸਾਰਣੀ, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_ਸੈੱਟ_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_func (face, hb_cg_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ, cg_font, ਖਾਲੀ_ਸੂਚਕ);
  ਵਾਪਸ face;
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
hb_coretext_face_create_from_file_or_fail (ਅਚਲ ਅੱਖਰ   *file_name,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ttc_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2 & 0xFFFF;
  ਬਿਨਾਨਿਸ਼ਾਨੀ named_instance_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ_2 >> 16;

  ਜੇ (ttc_ਸੂਚਕਾਂਕ != 0)
  {
    DEBUG_ਸੁਨੇਹਾ (CORETEXT, ਖਾਲੀ_ਸੂਚਕ, "TTC index %u not supported", ttc_ਸੂਚਕਾਂਕ);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (ਖਾਲੀ_ਸੂਚਕ,
								      (ਅਚਲ UInt8 *) file_name,
								      ਲੜੀ_ਲੰਬਾਈ (file_name),
								      ਝੂਠ));
  ਜੇ (unlikely (!url))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਸਵੈਚਾਲਿਤ ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  ਜੇ (unlikely (!ct_font_desc_array))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_ਸੂਚਕਾਂਕ));
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ਜੇ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (face, ਸੂਚਕਾਂਕ_2);

  ਵਾਪਸ face;
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
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸੂਚਕਾਂਕ_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, ਸੂਚਕਾਂਕ_2));
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ਜੇ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_face_ਸੈੱਟ_ਸੂਚਕਾਂਕ (face, ਸੂਚਕਾਂਕ_2);

  ਵਾਪਸ face;
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
  ਵਾਪਸ face ? (CGFontRef) (ਅਚਲ ਖਾਲੀ *) face->ਅੰਕੜੇ.coretext : ਖਾਲੀ_ਸੂਚਕ;
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
  ਜੇ (unlikely (!ct_font))
    ਵਾਪਸ hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, ਖਾਲੀ_ਸੂਚਕ));
  ਜੇ (unlikely (!cg_font))
    ਵਾਪਸ hb_font_get_ਖਾਲੀ_ਥਾਵਾਂ ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  ਜੇ (unlikely (hb_object_is_immutable (font)))
    ਵਾਪਸ font;

  hb_font_ਸੈੱਟ_ptem (font, CTFont_Get_ਮਾਪ (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  ਜੇ (variations)
  {

    hb_ਸਦਿਸ਼_t<hb_variation_t> vars;
    hb_ਸਦਿਸ਼_t<CFTypeRef> ਕੁੰਜੀਆਂ;
    hb_ਸਦਿਸ਼_t<CFTypeRef> values;

    CFIndex ਗਿਣਤੀ_2 = CFDictionary_Get_ਗਿਣਤੀ (variations);
    ਜੇ (likely (vars.alloc_exact (ਗਿਣਤੀ_2) && ਕੁੰਜੀਆਂ.resize_exact (ਗਿਣਤੀ_2) && values.resize_exact (ਗਿਣਤੀ_2)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_ਕੁੰਜੀਆਂ_And_ਮੁੱਲਾਂ (variations, ਕੁੰਜੀਆਂ.arrayZ, values.arrayZ);
      ਲਈ (CFIndex i = 0; i < ਗਿਣਤੀ_2; i++)
      {
	ਪੂਰਨ ਟੈਗ_2;
	ਦਸ਼ਮਲਵ value;
	CFNumber_Get_ਮੁੱਲ ((CFNumberRef) ਕੁੰਜੀਆਂ.arrayZ[i], kCFNumberIntType, &ਟੈਗ_2);
	CFNumber_Get_ਮੁੱਲ ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_ਟੈਗ_t) ਟੈਗ_2, value};
	vars.push (var);
      }
      hb_font_ਸੈੱਟ_variations (font, vars.arrayZ, vars.ਲੰਮਾਈ);
    }
  }

  /* Let there be dragons here... */
  font->ਅੰਕੜੇ.coretext.cmpexch (ਖਾਲੀ_ਸੂਚਕ, (hb_coretext_font_ਅੰਕੜੇ_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  ਵਾਪਸ font;
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
  ਵਾਪਸ font ? (CTFontRef) (ਅਚਲ ਖਾਲੀ *) font->ਅੰਕੜੇ.coretext : ਖਾਲੀ_ਸੂਚਕ;
}


#ਜੇ_ਅੰਤ
