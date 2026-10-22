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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_CORETEXT

#అన్తర్భావ "hb-shaper-impl.hh"

#అన్తర్భావ "hb-coretext.hh"


/**
 * SECTION:hb-coretext
 * @title: hb-coretext
 * @short_description: CoreText integration
 * @include: hb-coretext.h
 *
 * Functions for using HarfBuzz with the CoreText fonts.
 **/

స్థిర శూన్య
release_పట్టిక_దత్తాంశం (శూన్య *user_దత్తాంశం)
{
  CFDataRef cf_దత్తాంశం = reinterpret_cast<CFDataRef> (user_దత్తాంశం);
  CFRelease(cf_దత్తాంశం);
}

స్థిర hb_blob_t *
hb_cg_reference_పట్టిక (hb_face_t *face HB_వాడని, hb_ట్యాగ్_t ట్యాగ్_2, శూన్య *user_దత్తాంశం)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_దత్తాంశం);
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి శూన్యసూచిక;

  hb_cf_ptr_t<CFDataRef> cf_దత్తాంశం (CGFont_Copy_పట్టిక_For_ట్యాగ్ (cg_font, ట్యాగ్_2));
  ఒకవేళ (unlikely (!cf_దత్తాంశం))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి అక్షరం *దత్తాంశం = reinterpret_cast<స్థిరరాశి అక్షరం*> (CFDataGetBytePtr (cf_దత్తాంశం));
  స్థిరరాశి size_t నిడివి = CFData_Get_నిడివి (cf_దత్తాంశం);
  ఒకవేళ (!దత్తాంశం || !నిడివి)
    తిరిగి శూన్యసూచిక;

  తిరిగి hb_blob_create (దత్తాంశం, నిడివి, HB_జ్ఞాపకం_బహుళకం_READONLY,
			 reinterpret_cast<శూన్య *> (const_cast<__CFData *> (cf_దత్తాంశం.release ())),
			 release_పట్టిక_దత్తాంశం);
}

స్థిర గుర్తులేని
hb_cg_get_పట్టిక_ట్యాగులు (స్థిరరాశి hb_face_t *face HB_వాడని,
		       గుర్తులేని పూర్ణాంకం ప్రారంభం_స్థానభ్రంశం,
		       గుర్తులేని పూర్ణాంకం *పట్టిక_లెక్క_2,
		       hb_ట్యాగ్_t *పట్టిక_ట్యాగులు,
		       శూన్య *user_దత్తాంశం)
{
  CGFontRef cg_font = reinterpret_cast<CGFontRef> (user_దత్తాంశం);
  ఒకవేళ (unlikely (!cg_font))
  {
    ఒకవేళ (పట్టిక_లెక్క_2)
      *పట్టిక_లెక్క_2 = 0;
    తిరిగి 0;
  }

  hb_cf_ptr_t<CTFontRef> ct_font (create_ct_font (cg_font, (CGFloat) HB_CORETEXT_DEFAULT_FONT_కొలత));
  ఒకవేళ (unlikely (!ct_font))
  {
    ఒకవేళ (పట్టిక_లెక్క_2)
      *పట్టిక_లెక్క_2 = 0;
    తిరిగి 0;
  }

  hb_cf_ptr_t<CFArrayRef> arr (CTFont_Copy_అందుబాటు_Tables (ct_font, k_CTFont_పట్టిక_Option_No_Options));
  ఒకవేళ (unlikely (!arr))
  {
    ఒకవేళ (పట్టిక_లెక్క_2)
      *పట్టిక_లెక్క_2 = 0;
    తిరిగి 0;
  }

  గుర్తులేని population = (గుర్తులేని) CFArray_Get_లెక్క (arr);

  ఒకవేళ (!పట్టిక_లెక్క_2)
    తిరిగి population;

  ఒకవేళ (unlikely (ప్రారంభం_స్థానభ్రంశం >= population))
  {
    *పట్టిక_లెక్క_2 = 0;
    తిరిగి population;
  }

  గుర్తులేని చివర_స్థానభ్రంశం = ప్రారంభం_స్థానభ్రంశం + *పట్టిక_లెక్క_2;
  ఒకవేళ (unlikely (చివర_స్థానభ్రంశం < ప్రారంభం_స్థానభ్రంశం))
  {
    *పట్టిక_లెక్క_2 = 0;
    తిరిగి population;
  }
  చివర_స్థానభ్రంశం = hb_min (చివర_స్థానభ్రంశం, (గుర్తులేని) population);

  *పట్టిక_లెక్క_2 = చివర_స్థానభ్రంశం - ప్రారంభం_స్థానభ్రంశం;
  ఒకవేళ (పట్టిక_ట్యాగులు)
    కొరకు (గుర్తులేని i = ప్రారంభం_స్థానభ్రంశం; i < చివర_స్థానభ్రంశం; i++)
    {
      CTFont_పట్టిక_ట్యాగ్ ట్యాగ్_2 = (CTFont_పట్టిక_ట్యాగ్)(uintptr_t) CFArray_Get_విలువ_At_సూచీ (arr, i);
      పట్టిక_ట్యాగులు[i - ప్రారంభం_స్థానభ్రంశం] = ట్యాగ్_2;
    }

  తిరిగి population;
}

స్థిర శూన్య
_hb_cg_font_release (శూన్య *దత్తాంశం)
{
  ఒకవేళ (దత్తాంశం)
    CGFontRelease ((CGFontRef) దత్తాంశం);
}


స్థిర CTFontDescriptorRef
get_చివరి_resort_font_desc ()
{
  hb_cf_ptr_t<CTFontDescriptorRef> చివరి_resort (CTFont_Descriptor_Create_With_Name_And_కొలత (CFSTR("LastResort"), 0));
  ఒకవేళ (unlikely (!చివరి_resort))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి శూన్య *cascade_elements[] = { చివరి_resort.get () };
  hb_cf_ptr_t<CFArrayRef> cascade_list (CFArrayCreate (kCFAllocatorDefault,
						       cascade_elements,
						       1,
						       &kCFTypeArrayCallBacks));
  ఒకవేళ (unlikely (!cascade_list))
    తిరిగి శూన్యసూచిక;

  స్థిరరాశి శూన్య *కీలకాలు[] = { kCTFontCascadeListAttribute };
  స్థిరరాశి శూన్య *values[] = { cascade_list.get () };
  hb_cf_ptr_t<CFDictionaryRef> లక్షణాలు (CFDictionaryCreate (kCFAllocatorDefault,
							       కీలకాలు,
							       values,
							       1,
							       &k_CFType_Dictionary_కీలకం_Call_Backs,
							       &k_CFType_Dictionary_విలువ_Call_Backs));
  ఒకవేళ (unlikely (!లక్షణాలు))
    తిరిగి శూన్యసూచిక;

  తిరిగి CTFont_Descriptor_Create_With_లక్షణాలు (లక్షణాలు);
}

స్థిర శూన్య
release_దత్తాంశం (శూన్య *info, స్థిరరాశి శూన్య *దత్తాంశం, size_t కొలత)
{
  ప్రతిజ్ఞా (hb_blob_get_నిడివి ((hb_blob_t *) info) == కొలత &&
	  hb_blob_get_దత్తాంశం ((hb_blob_t *) info, శూన్యసూచిక) == దత్తాంశం);

  hb_blob_destroy ((hb_blob_t *) info);
}

CGFontRef
create_cg_font (CFArrayRef ct_font_desc_array, గుర్తులేని పూర్ణాంకం named_instance_సూచీ)
{
  hb_cf_ptr_t<CFArrayRef> desc_array (ct_font_desc_array);
  ఒకవేళ (unlikely (!desc_array))
    తిరిగి శూన్యసూచిక;

  ఒకవేళ (named_instance_సూచీ == 0)
  {
    // Default instance. We don't know which one is it. Return the first one.
    // We will set the correct variations on it later.
  }
  లేకపోతే
    named_instance_సూచీ--;
  స్వయంచాలక ct_font_desc = (CFArray_Get_లెక్క (desc_array) > (CFIndex) named_instance_సూచీ) ?
		      (CTFontDescriptorRef) CFArray_Get_విలువ_At_సూచీ (desc_array, (CFIndex) named_instance_సూచీ) : శూన్యసూచిక;
  ఒకవేళ (unlikely (!ct_font_desc))
    తిరిగి శూన్యసూచిక;

  hb_cf_ptr_t<CTFontRef> ct_font (CTFontCreateWithFontDescriptor (ct_font_desc, 0, శూన్యసూచిక));
  ఒకవేళ (unlikely (!ct_font))
    తిరిగి శూన్యసూచిక;

  తిరిగి CTFontCopyGraphicsFont (ct_font, శూన్యసూచిక);
}

CGFontRef
create_cg_font (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం సూచీ_2)
{
  hb_blob_make_immutable (blob);
  గుర్తులేని పూర్ణాంకం blob_నిడివి;
  స్థిరరాశి అక్షరం *blob_దత్తాంశం = hb_blob_get_దత్తాంశం (blob, &blob_నిడివి);
  ఒకవేళ (unlikely (!blob_నిడివి))
    DEBUG_సందేశం (CORETEXT, blob, "Empty blob");

  గుర్తులేని ttc_సూచీ = సూచీ_2 & 0xFFFF;
  గుర్తులేని named_instance_సూచీ = సూచీ_2 >> 16;

  ఒకవేళ (ttc_సూచీ != 0)
  {
    DEBUG_సందేశం (CORETEXT, blob, "TTC index %u not supported", ttc_సూచీ);
    తిరిగి శూన్యసూచిక; // CoreText does not support TTCs
  }

  ఒకవేళ (unlikely (named_instance_సూచీ != 0))
  {
    // https://github.com/harfbuzz/harfbuzz/issues/5300
    // https://github.com/harfbuzz/harfbuzz/issues/5354
#ఒకవేళ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500) || \
    (defined(__TV_OS_VERSION_MIN_REQUIRED) && __TV_OS_VERSION_MIN_REQUIRED >= 110000) || \
    (defined(__WATCH_OS_VERSION_MIN_REQUIRED) && __WATCH_OS_VERSION_MIN_REQUIRED >= 40000) || \
    (defined(__MACCATALYST_VERSION_MIN_REQUIRED) && __MACCATALYST_VERSION_MIN_REQUIRED >= 130100) || \
    (defined(__VISION_OS_VERSION_MIN_REQUIRED) && __VISION_OS_VERSION_MIN_REQUIRED >= 10000)
    hb_cf_ptr_t<CFDataRef> దత్తాంశం (CFDataCreate (kCFAllocatorDefault, (స్థిరరాశి UInt8 *) blob_దత్తాంశం, blob_నిడివి));
    ఒకవేళ (దత్తాంశం)
    {
      స్వయంచాలక ct_font_desc_array = CTFont_Manager_Create_Font_Descriptors_From_దత్తాంశం (దత్తాంశం);
      ఒకవేళ (likely (ct_font_desc_array))
	తిరిగి create_cg_font (ct_font_desc_array, named_instance_సూచీ);
    }
#అయితే_ముగింపు
    తిరిగి శూన్యసూచిక;
  }

  hb_blob_reference (blob);
  hb_cf_ptr_t<CGDataProviderRef> provider (CGData_Provider_Create_With_దత్తాంశం (blob, blob_దత్తాంశం, blob_నిడివి, &release_దత్తాంశం));
  ఒకవేళ (unlikely (!provider))
  {
    hb_blob_destroy (blob);
    తిరిగి శూన్యసూచిక;
  }
  CGFontRef cg_font = CGFont_Create_With_దత్తాంశం_Provider (provider);
  ఒకవేళ (unlikely (!cg_font))
    DEBUG_సందేశం (CORETEXT, blob, "CGFontCreateWithDataProvider() failed");
  తిరిగి cg_font;
}

CGFontRef
create_cg_font (hb_face_t *face)
{
  CGFontRef cg_font = శూన్యసూచిక;
  ఒకవేళ (unlikely (!face))
    తిరిగి శూన్యసూచిక;
  ఒకవేళ (face->destroy == _hb_cg_font_release)
    cg_font = face->user_దత్తాంశం ? CGFontRetain ((CGFontRef) face->user_దత్తాంశం) : శూన్యసూచిక;
  లేకపోతే
  {
    hb_blob_t *blob = hb_face_reference_blob (face);
    cg_font = create_cg_font (blob, face->సూచీ_2);
    hb_blob_destroy (blob);
  }
  తిరిగి cg_font;
}

CTFontRef
create_ct_font (CGFontRef cg_font, CGFloat font_కొలత)
{
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి శూన్యసూచిక;

  hb_cf_ptr_t<CTFontRef> ct_font;

  /* CoreText does not enable trak table usage / tracking when creating a CTFont
   * using CTFontCreateWithGraphicsFont. The only way of enabling tracking seems
   * to be through the CTFontCreateUIFontForLanguage call. */
  hb_cf_ptr_t<CFStringRef> cg_postscript_name (CGFontCopyPostScriptName (cg_font));
  ఒకవేళ (cg_postscript_name)
  {
    ఒకవేళ (CFString_Has_ఉపసర్గ (cg_postscript_name, CFSTR (".SFNSText")) ||
	CFString_Has_ఉపసర్గ (cg_postscript_name, CFSTR (".SFNSDisplay")))
    {
#ఒకవేళ !(defined(లక్ష్యం_OS_IPHONE) && లక్ష్యం_OS_IPHONE) && MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 1080
# define kCTFontUIFontSystem kCTFontSystemFontType
# define kCTFontUIFontEmphasizedSystem kCTFontEmphasizedSystemFontType
#అయితే_ముగింపు
      CTFontUIFontType font_type = kCTFontUIFontSystem;
      ఒకవేళ (CFStringHasSuffix (cg_postscript_name, CFSTR ("-Bold")))
	font_type = kCTFontUIFontEmphasizedSystem;

      ct_font.reset (CTFontCreateUIFontForLanguage (font_type, font_కొలత, శూన్యసూచిక));
      ఒకవేళ (ct_font)
      {
	hb_cf_ptr_t<CFStringRef> ct_ఫలితం_name (CTFontCopyPostScriptName (ct_font));
	ఒకవేళ (!ct_ఫలితం_name || CFStringCompare (ct_ఫలితం_name, cg_postscript_name, 0) != k_CFCompare_Equal_గమ్యం)
	  ct_font.reset ();
      }
    }
  }

  ఒకవేళ (!ct_font)
    ct_font.reset (CTFontCreateWithGraphicsFont (cg_font, font_కొలత, శూన్యసూచిక, శూన్యసూచిక));

  ఒకవేళ (unlikely (!ct_font)) {
    DEBUG_సందేశం (CORETEXT, cg_font, "Font CTFontCreateWithGraphicsFont() failed");
    తిరిగి శూన్యసూచిక;
  }

  /* crbug.com/576941 and crbug.com/625902 and the investigation in the latter
   * bug indicate that the cascade list reconfiguration occasionally causes
   * crashes in CoreText on OS X 10.9, thus let's skip this step on older
   * operating system versions. Except for the emoji font, where _not_
   * reconfiguring the cascade list causes CoreText crashes. For details, see
   * crbug.com/549610 */
  // 0x00070000 stands for "kCTVersionNumber10_10", see CoreText.h
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wdeprecated-declarations"
  ఒకవేళ (&CTGet_Core_పాఠ్యం_సంస్కరణ != శూన్యసూచిక && CTGet_Core_పాఠ్యం_సంస్కరణ() < 0x00070000) {
#సూచన GCC diagnostic తీయి
    hb_cf_ptr_t<CFStringRef> fontName (CTFontCopyPostScriptName (ct_font));
    తార్కిక isEmojiFont = fontName && CFStringCompare (fontName, CFSTR("AppleColorEmoji"), 0) == k_CFCompare_Equal_గమ్యం;
    ఒకవేళ (!isEmojiFont)
      తిరిగి ct_font.release ();
  }

  hb_cf_ptr_t<CFURLRef> original_url;
#ఒకవేళ !(defined(లక్ష్యం_OS_IPHONE) && లక్ష్యం_OS_IPHONE) && MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 1060
  ATSFontRef atsFont;
  FSRef fsref;
  OSStatus స్థితి_2;
  atsFont = CTFontGetPlatformFont (ct_font, ఖాళీ);
  స్థితి_2 = ATSFontGetFileReference (atsFont, &fsref);
  ఒకవేళ (స్థితి_2 == noErr)
    original_url.reset (CFURLCreateFromFSRef (ఖాళీ, &fsref));
#లేకపోతే
  original_url.reset ((CFURLRef) CTFontCopyAttribute (ct_font, kCTFontURLAttribute));
#అయితే_ముగింపు

  /* Create font copy with cascade list that has LastResort first; this speeds up CoreText
   * font fallback which we don't need anyway. */
  {
    hb_cf_ptr_t<CTFontDescriptorRef> చివరి_resort_font_desc (get_చివరి_resort_font_desc ());
    ఒకవేళ (చివరి_resort_font_desc)
    {
      hb_cf_ptr_t<CTFontRef> new_ct_font (CTFont_Create_Copy_With_లక్షణాలు (ct_font, 0.0, శూన్యసూచిక, చివరి_resort_font_desc));
      ఒకవేళ (new_ct_font)
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
#ఒకవేళ !(defined(లక్ష్యం_OS_IPHONE) && లక్ష్యం_OS_IPHONE) && MAC_OS_X_సంస్కరణ_MIN_REQUIRED < 1060
	atsFont = CTFontGetPlatformFont (new_ct_font, ఖాళీ);
	స్థితి_2 = ATSFontGetFileReference (atsFont, &fsref);
	ఒకవేళ (స్థితి_2 == noErr)
	  new_url.reset (CFURLCreateFromFSRef (ఖాళీ, &fsref));
#లేకపోతే
	new_url.reset ((CFURLRef) CTFontCopyAttribute (new_ct_font, kCTFontURLAttribute));
#అయితే_ముగింపు
	// Keep reconfigured font if URL cannot be retrieved (seems to be the case
	// on Mac OS 10.12 Sierra), speculative fix for crbug.com/625606
	ఒకవేళ (!original_url || !new_url || CFEqual (original_url, new_url)) {
	  ct_font = std::move (new_ct_font);
	} లేకపోతే {
	  DEBUG_సందేశం (CORETEXT, ct_font, "Discarding reconfigured CTFont, location changed.");
	}
      }
      లేకపోతే
	DEBUG_సందేశం (CORETEXT, ct_font, "Font copy with empty cascade list failed");
    }
    లేకపోతే
      DEBUG_సందేశం (CORETEXT, ct_font, "Font copy with empty cascade list failed");
  }

  తిరిగి ct_font.release ();
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
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి hb_face_get_ఖాళీలు ();

  hb_face_t *face = hb_face_create_for_tables (hb_cg_reference_పట్టిక, CGFontRetain (cg_font), _hb_cg_font_release);
  hb_face_సమితి_get_పట్టిక_ట్యాగులు_func (face, hb_cg_get_పట్టిక_ట్యాగులు, cg_font, శూన్యసూచిక);
  తిరిగి face;
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
hb_coretext_face_create_from_file_or_fail (స్థిరరాశి అక్షరం   *file_name,
					   గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  గుర్తులేని ttc_సూచీ = సూచీ_2 & 0xFFFF;
  గుర్తులేని named_instance_సూచీ = సూచీ_2 >> 16;

  ఒకవేళ (ttc_సూచీ != 0)
  {
    DEBUG_సందేశం (CORETEXT, శూన్యసూచిక, "TTC index %u not supported", ttc_సూచీ);
    తిరిగి శూన్యసూచిక; // CoreText does not support TTCs
  }

  hb_cf_ptr_t<CFURLRef> url (CFURLCreateFromFileSystemRepresentation (శూన్యసూచిక,
								      (స్థిరరాశి UInt8 *) file_name,
								      అక్షరశ్రేణి_పొడవు (file_name),
								      అబద్ధం));
  ఒకవేళ (unlikely (!url))
    తిరిగి శూన్యసూచిక;

  స్వయంచాలక ct_font_desc_array = CTFontManagerCreateFontDescriptorsFromURL (url);
  ఒకవేళ (unlikely (!ct_font_desc_array))
    తిరిగి శూన్యసూచిక;

  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (ct_font_desc_array, named_instance_సూచీ));
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి శూన్యసూచిక;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ఒకవేళ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    తిరిగి శూన్యసూచిక;
  }

  hb_face_సమితి_సూచీ (face, సూచీ_2);

  తిరిగి face;
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
					   గుర్తులేని పూర్ణాంకం  సూచీ_2)
{
  hb_cf_ptr_t<CGFontRef> cg_font (create_cg_font (blob, సూచీ_2));
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి శూన్యసూచిక;

  hb_face_t *face = hb_coretext_face_create (cg_font);
  ఒకవేళ (unlikely (hb_face_is_immutable (face)))
  {
    hb_face_destroy (face);
    తిరిగి శూన్యసూచిక;
  }

  hb_face_సమితి_సూచీ (face, సూచీ_2);

  తిరిగి face;
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
  తిరిగి face ? (CGFontRef) (స్థిరరాశి శూన్య *) face->దత్తాంశం.coretext : శూన్యసూచిక;
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
  ఒకవేళ (unlikely (!ct_font))
    తిరిగి hb_font_get_ఖాళీలు ();

  hb_cf_ptr_t<CGFontRef> cg_font (CTFontCopyGraphicsFont (ct_font, శూన్యసూచిక));
  ఒకవేళ (unlikely (!cg_font))
    తిరిగి hb_font_get_ఖాళీలు ();

  hb_face_t *face = hb_coretext_face_create (cg_font);
  cg_font.reset ();
  hb_font_t *font = hb_font_create (face);
  hb_face_destroy (face);

  ఒకవేళ (unlikely (hb_object_is_immutable (font)))
    తిరిగి font;

  hb_font_సమితి_ptem (font, CTFont_Get_కొలత (ct_font));

  /* Copy font variations */
  hb_cf_ptr_t<CFDictionaryRef> variations (CTFontCopyVariation (ct_font));
  ఒకవేళ (variations)
  {

    hb_సదిశ_t<hb_variation_t> vars;
    hb_సదిశ_t<CFTypeRef> కీలకాలు;
    hb_సదిశ_t<CFTypeRef> values;

    CFIndex లెక్క = CFDictionary_Get_లెక్క (variations);
    ఒకవేళ (likely (vars.alloc_exact (లెక్క) && కీలకాలు.resize_exact (లెక్క) && values.resize_exact (లెక్క)))
    {
      // Fetch them one by one and collect in a vector of our own.
      CFDictionary_Get_కీలకాలు_And_విలువలు (variations, కీలకాలు.arrayZ, values.arrayZ);
      కొరకు (CFIndex i = 0; i < లెక్క; i++)
      {
	పూర్ణాంకం ట్యాగ్_2;
	దశాంశం value;
	CFNumber_Get_విలువ ((CFNumberRef) కీలకాలు.arrayZ[i], kCFNumberIntType, &ట్యాగ్_2);
	CFNumber_Get_విలువ ((CFNumberRef) values.arrayZ[i], kCFNumberFloatType, &value);

	hb_variation_t var = {(hb_ట్యాగ్_t) ట్యాగ్_2, value};
	vars.push (var);
      }
      hb_font_సమితి_variations (font, vars.arrayZ, vars.నిడివి);
    }
  }

  /* Let there be dragons here... */
  font->దత్తాంశం.coretext.cmpexch (శూన్యసూచిక, (hb_coretext_font_దత్తాంశం_t *) CFRetain (ct_font));

  // https://github.com/harfbuzz/harfbuzz/pull/4895#issuecomment-2408471254
  //hb_coretext_font_set_funcs (font);

  తిరిగి font;
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
  తిరిగి font ? (CTFontRef) (స్థిరరాశి శూన్య *) font->దత్తాంశం.coretext : శూన్యసూచిక;
}


#అయితే_ముగింపు
