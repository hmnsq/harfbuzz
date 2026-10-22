/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2009  Keith Stribley
 * Copyright © 2011  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#அந்தர்பாவ "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#அந்தர்பாவ "hb-icu.h"

#அந்தர்பாவ "hb-machinery.hh"

#அந்தர்பாவ <unicode/uchar.h>
#அந்தர்பாவ <unicode/unorm2.h>
#அந்தர்பாவ <unicode/ustring.h>
#அந்தர்பாவ <unicode/utf16.h>
#அந்தர்பாவ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#எனில் U_ICU_VERSION_MAJOR_இலக்கம் < 65 && (defined(__GNUC__) || defined(__clang__))
#define HB_ICU_EXTRA_SEMI_IGNORED
#pragma GCC diagnostic ignored "-Wextra-semi-stmt"
#endif

/**
 * SECTION:hb-icu
 * @title: hb-icu
 * @short_description: ICU integration
 * @include: hb-icu.h
 *
 * Functions for using HarfBuzz with the International Components for Unicode
 * (ICU) library. HarfBuzz supports using ICU to provide Unicode data, by attaching
 * ICU functions to the virtual methods in a #hb_unicode_funcs_t function
 * structure.
 **/

/**
 * hb_icu_script_to_script:
 * @script: The UScriptCode identifier to query
 *
 * Fetches the #hb_script_t script that corresponds to the
 * specified UScriptCode identifier.
 *
 * Return value: the #hb_script_t script found
 *
 **/

hb_script_t
hb_icu_script_to_script (UScriptCode script)
{
  எனில் (unlikely (script == USCRIPT_INVALID_CODE))
    திரும்பு HB_SCRIPT_INVALID;

  திரும்பு hb_script_from_string (uscript_getShortName (script), -1);
}

/**
 * hb_icu_script_from_script:
 * @script: The #hb_script_t script to query
 *
 * Fetches the UScriptCode identifier that corresponds to the
 * specified #hb_script_t script.
 *
 * Return value: the UScriptCode identifier found
 *
 **/
UScriptCode
hb_icu_script_from_script (hb_script_t script)
{
  UScriptCode out = USCRIPT_INVALID_CODE;

  எனில் (unlikely (script == HB_SCRIPT_INVALID))
    திரும்பு out;

  UErrorCode icu_err = U_ZERO_ERROR;
  மாறிலி unsigned எழுத்து buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((மாறிலி எழுத்து *) buf, &out, 1, &icu_err);

  திரும்பு out;
}


நிலையான hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				வெறுமை               *user_data HB_UNUSED)

{
  திரும்பு (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

நிலையான hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 வெறுமை               *user_data HB_UNUSED)
{
  தேர்வு (u_get_Int_Property_மதிப்பு(unicode, UCHAR_GENERAL_CATEGORY))
  {
  நிலை U_UNASSIGNED:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  நிலை U_UPPERCASE_LETTER:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  நிலை U_LOWERCASE_LETTER:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  நிலை U_TITLECASE_LETTER:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  நிலை U_MODIFIER_LETTER:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  நிலை U_OTHER_LETTER:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  நிலை U_NON_SPACING_MARK:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  நிலை U_ENCLOSING_MARK:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  நிலை U_COMBINING_SPACING_MARK:	திரும்பு HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  நிலை U_DECIMAL_DIGIT_எண்:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_DECIMAL_எண்;
  நிலை U_LETTER_எண்:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_LETTER_எண்;
  நிலை U_OTHER_எண்:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_OTHER_எண்;

  நிலை U_SPACE_SEPARATOR:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  நிலை U_வரி_SEPARATOR:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_வரி_SEPARATOR;
  நிலை U_PARAGRAPH_SEPARATOR:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  நிலை U_CONTROL_CHAR:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  நிலை U_FORMAT_CHAR:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  நிலை U_PRIVATE_USE_CHAR:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  நிலை U_SURROGATE:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  நிலை U_DASH_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  நிலை U_START_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  நிலை U_END_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  நிலை U_CONNECTOR_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  நிலை U_OTHER_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  நிலை U_MATH_குறியீடு:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_MATH_குறியீடு;
  நிலை U_CURRENCY_குறியீடு:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_CURRENCY_குறியீடு;
  நிலை U_MODIFIER_குறியீடு:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_MODIFIER_குறியீடு;
  நிலை U_OTHER_குறியீடு:			திரும்பு HB_UNICODE_GENERAL_CATEGORY_OTHER_குறியீடு;

  நிலை U_INITIAL_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  நிலை U_FINAL_PUNCTUATION:		திரும்பு HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  திரும்பு HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

நிலையான hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  வெறுமை               *user_data HB_UNUSED)
{
  திரும்பு u_charMirror(unicode);
}

நிலையான hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       வெறுமை               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  எனில் (unlikely (U_FAILURE (status)))
    திரும்பு HB_SCRIPT_UNKNOWN;

  திரும்பு hb_icu_script_to_script (scriptCode);
}

நிலையான hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			வெறுமை               *user_data)
{
  மாறிலி UNormalizer2 *normalizer = (மாறிலி UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  எனில் (ret < 0) திரும்பு பொய்;
  *ab = ret;
  திரும்பு உண்மை;
}

நிலையான hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  வெறுமை               *user_data)
{
  மாறிலி UNormalizer2 *normalizer = (மாறிலி UNormalizer2 *) user_data;
  UChar decomposed[4];
  முழு len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  எனில் (U_FAILURE (icu_err) || len < 0) திரும்பு பொய்;

  len = u_மொத்தம்_Char32 (decomposed, len);
  எனில் (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    திரும்பு *a != ab;
  }
  இல்லையெனில் எனில் (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  திரும்பு உண்மை;
}


நிலையான உள்வரி வெறுமை free_static_icu_funcs ();

நிலையான கட்டமைப்பு hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  நிலையான hb_unicode_funcs_t *create ()
  {
    வெறுமை *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (வெறுமை *) unorm2_getNFCInstance (&icu_err);
    ப்ரதிஜ்ஞா (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    திரும்பு funcs;
  }
} static_icu_funcs;

நிலையான உள்வரி
வெறுமை free_static_icu_funcs ()
{
  static_icu_funcs.free_instance ();
}

/**
 * hb_icu_get_unicode_funcs:
 *
 * Fetches a Unicode-functions structure that is populated
 * with the appropriate ICU function for each method.
 *
 * Return value: (transfer none): a pointer to the #hb_unicode_funcs_t Unicode-functions structure
 *
 * Since: 0.9.38
 **/
hb_unicode_funcs_t *
hb_icu_get_unicode_funcs ()
{
  திரும்பு static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic எடு

#endif
