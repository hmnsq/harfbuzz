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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#ଅନ୍ତର୍ଭାଵ "hb-icu.h"

#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"

#ଅନ୍ତର୍ଭାଵ <unicode/uchar.h>
#ଅନ୍ତର୍ଭାଵ <unicode/unorm2.h>
#ଅନ୍ତର୍ଭାଵ <unicode/ustring.h>
#ଅନ୍ତର୍ଭାଵ <unicode/utf16.h>
#ଅନ୍ତର୍ଭାଵ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#ଯଦି U_ICU_VERSION_MAJOR_ଅଙ୍କ < 65 && (defined(__GNUC__) || defined(__clang__))
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
  ଯଦି (unlikely (script == USCRIPT_INVALID_CODE))
    ଫେରନ୍ତୁ HB_SCRIPT_INVALID;

  ଫେରନ୍ତୁ hb_script_from_string (uscript_getShortName (script), -1);
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

  ଯଦି (unlikely (script == HB_SCRIPT_INVALID))
    ଫେରନ୍ତୁ out;

  UErrorCode icu_err = U_ZERO_ERROR;
  ଅଚଳ unsigned ଅକ୍ଷର buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((ଅଚଳ ଅକ୍ଷର *) buf, &out, 1, &icu_err);

  ଫେରନ୍ତୁ out;
}


ସ୍ଥିର hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				ଶୂନ୍ୟ               *user_data HB_UNUSED)

{
  ଫେରନ୍ତୁ (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ସ୍ଥିର hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 ଶୂନ୍ୟ               *user_data HB_UNUSED)
{
  ବାଛ (u_get_Int_Property_ମୂଲ୍ୟ(unicode, UCHAR_GENERAL_CATEGORY))
  {
  ସ୍ଥିତି U_UNASSIGNED:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  ସ୍ଥିତି U_UPPERCASE_LETTER:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  ସ୍ଥିତି U_LOWERCASE_LETTER:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  ସ୍ଥିତି U_TITLECASE_LETTER:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  ସ୍ଥିତି U_MODIFIER_LETTER:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  ସ୍ଥିତି U_OTHER_LETTER:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  ସ୍ଥିତି U_NON_SPACING_MARK:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  ସ୍ଥିତି U_ENCLOSING_MARK:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  ସ୍ଥିତି U_COMBINING_SPACING_MARK:	ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  ସ୍ଥିତି U_DECIMAL_DIGIT_ସଂଖ୍ୟା:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ସଂଖ୍ୟା;
  ସ୍ଥିତି U_LETTER_ସଂଖ୍ୟା:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_LETTER_ସଂଖ୍ୟା;
  ସ୍ଥିତି U_OTHER_ସଂଖ୍ୟା:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_OTHER_ସଂଖ୍ୟା;

  ସ୍ଥିତି U_SPACE_SEPARATOR:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  ସ୍ଥିତି U_ଧାଡ଼ି_SEPARATOR:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_ଧାଡ଼ି_SEPARATOR;
  ସ୍ଥିତି U_PARAGRAPH_SEPARATOR:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  ସ୍ଥିତି U_CONTROL_CHAR:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  ସ୍ଥିତି U_FORMAT_CHAR:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  ସ୍ଥିତି U_PRIVATE_USE_CHAR:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  ସ୍ଥିତି U_SURROGATE:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  ସ୍ଥିତି U_DASH_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  ସ୍ଥିତି U_START_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  ସ୍ଥିତି U_END_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  ସ୍ଥିତି U_CONNECTOR_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  ସ୍ଥିତି U_OTHER_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  ସ୍ଥିତି U_MATH_ପ୍ରତୀକ:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_MATH_ପ୍ରତୀକ;
  ସ୍ଥିତି U_CURRENCY_ପ୍ରତୀକ:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_CURRENCY_ପ୍ରତୀକ;
  ସ୍ଥିତି U_MODIFIER_ପ୍ରତୀକ:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ପ୍ରତୀକ;
  ସ୍ଥିତି U_OTHER_ପ୍ରତୀକ:			ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_OTHER_ପ୍ରତୀକ;

  ସ୍ଥିତି U_INITIAL_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  ସ୍ଥିତି U_FINAL_PUNCTUATION:		ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  ଫେରନ୍ତୁ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ସ୍ଥିର hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  ଶୂନ୍ୟ               *user_data HB_UNUSED)
{
  ଫେରନ୍ତୁ u_charMirror(unicode);
}

ସ୍ଥିର hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       ଶୂନ୍ୟ               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  ଯଦି (unlikely (U_FAILURE (status)))
    ଫେରନ୍ତୁ HB_SCRIPT_UNKNOWN;

  ଫେରନ୍ତୁ hb_icu_script_to_script (scriptCode);
}

ସ୍ଥିର hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			ଶୂନ୍ୟ               *user_data)
{
  ଅଚଳ UNormalizer2 *normalizer = (ଅଚଳ UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  ଯଦି (ret < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  *ab = ret;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  ଶୂନ୍ୟ               *user_data)
{
  ଅଚଳ UNormalizer2 *normalizer = (ଅଚଳ UNormalizer2 *) user_data;
  UChar decomposed[4];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  ଯଦି (U_FAILURE (icu_err) || len < 0) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  len = u_ଗଣନା_Char32 (decomposed, len);
  ଯଦି (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    ଫେରନ୍ତୁ *a != ab;
  }
  ଅନ୍ୟଥା ଯଦି (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_icu_funcs ();

ସ୍ଥିର ସଂରଚନା hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ସ୍ଥିର hb_unicode_funcs_t *create ()
  {
    ଶୂନ୍ୟ *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (ଶୂନ୍ୟ *) unorm2_getNFCInstance (&icu_err);
    ପ୍ରତିଜ୍ଞା (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    ଫେରନ୍ତୁ funcs;
  }
} static_icu_funcs;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_icu_funcs ()
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
  ଫେରନ୍ତୁ static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic କାଢ଼

#endif
