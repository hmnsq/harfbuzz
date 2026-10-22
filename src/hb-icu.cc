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

#অন্তর্ভাব "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#অন্তর্ভাব "hb-icu.h"

#অন্তর্ভাব "hb-machinery.hh"

#অন্তর্ভাব <unicode/uchar.h>
#অন্তর্ভাব <unicode/unorm2.h>
#অন্তর্ভাব <unicode/ustring.h>
#অন্তর্ভাব <unicode/utf16.h>
#অন্তর্ভাব <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#যদি U_ICU_VERSION_MAJOR_অঙ্ক < 65 && (defined(__GNUC__) || defined(__clang__))
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
  যদি (unlikely (script == USCRIPT_INVALID_CODE))
    ফেরত HB_SCRIPT_INVALID;

  ফেরত hb_script_from_string (uscript_getShortName (script), -1);
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

  যদি (unlikely (script == HB_SCRIPT_INVALID))
    ফেরত out;

  UErrorCode icu_err = U_ZERO_ERROR;
  ধ্রুবক unsigned অক্ষর buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((ধ্রুবক অক্ষর *) buf, &out, 1, &icu_err);

  ফেরত out;
}


স্থির hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				শূন্য               *user_তথ্য HB_UNUSED)

{
  ফেরত (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

স্থির hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 শূন্য               *user_তথ্য HB_UNUSED)
{
  নির্বাচন (u_get_Int_Property_মান(unicode, UCHAR_GENERAL_CATEGORY))
  {
  ক্ষেত্র U_UNASSIGNED:			ফেরত HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  ক্ষেত্র U_UPPERCASE_LETTER:		ফেরত HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  ক্ষেত্র U_LOWERCASE_LETTER:		ফেরত HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  ক্ষেত্র U_TITLECASE_LETTER:		ফেরত HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  ক্ষেত্র U_MODIFIER_LETTER:		ফেরত HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  ক্ষেত্র U_OTHER_LETTER:			ফেরত HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  ক্ষেত্র U_NON_SPACING_MARK:		ফেরত HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  ক্ষেত্র U_ENCLOSING_MARK:		ফেরত HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  ক্ষেত্র U_COMBINING_SPACING_MARK:	ফেরত HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  ক্ষেত্র U_DECIMAL_DIGIT_সংখ্যা:		ফেরত HB_UNICODE_GENERAL_CATEGORY_DECIMAL_সংখ্যা;
  ক্ষেত্র U_LETTER_সংখ্যা:			ফেরত HB_UNICODE_GENERAL_CATEGORY_LETTER_সংখ্যা;
  ক্ষেত্র U_OTHER_সংখ্যা:			ফেরত HB_UNICODE_GENERAL_CATEGORY_OTHER_সংখ্যা;

  ক্ষেত্র U_SPACE_SEPARATOR:		ফেরত HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  ক্ষেত্র U_সারি_SEPARATOR:		ফেরত HB_UNICODE_GENERAL_CATEGORY_সারি_SEPARATOR;
  ক্ষেত্র U_PARAGRAPH_SEPARATOR:		ফেরত HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  ক্ষেত্র U_CONTROL_CHAR:			ফেরত HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  ক্ষেত্র U_FORMAT_CHAR:			ফেরত HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  ক্ষেত্র U_PRIVATE_USE_CHAR:		ফেরত HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  ক্ষেত্র U_SURROGATE:			ফেরত HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  ক্ষেত্র U_DASH_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  ক্ষেত্র U_START_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  ক্ষেত্র U_END_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  ক্ষেত্র U_CONNECTOR_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  ক্ষেত্র U_OTHER_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  ক্ষেত্র U_MATH_প্রতীক:			ফেরত HB_UNICODE_GENERAL_CATEGORY_MATH_প্রতীক;
  ক্ষেত্র U_CURRENCY_প্রতীক:		ফেরত HB_UNICODE_GENERAL_CATEGORY_CURRENCY_প্রতীক;
  ক্ষেত্র U_MODIFIER_প্রতীক:		ফেরত HB_UNICODE_GENERAL_CATEGORY_MODIFIER_প্রতীক;
  ক্ষেত্র U_OTHER_প্রতীক:			ফেরত HB_UNICODE_GENERAL_CATEGORY_OTHER_প্রতীক;

  ক্ষেত্র U_INITIAL_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  ক্ষেত্র U_FINAL_PUNCTUATION:		ফেরত HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  ফেরত HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

স্থির hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  শূন্য               *user_তথ্য HB_UNUSED)
{
  ফেরত u_charMirror(unicode);
}

স্থির hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       শূন্য               *user_তথ্য HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  যদি (unlikely (U_FAILURE (status)))
    ফেরত HB_SCRIPT_UNKNOWN;

  ফেরত hb_icu_script_to_script (scriptCode);
}

স্থির hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			শূন্য               *user_তথ্য)
{
  ধ্রুবক UNormalizer2 *normalizer = (ধ্রুবক UNormalizer2 *) user_তথ্য;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  যদি (ret < 0) ফেরত মিথ্যা;
  *ab = ret;
  ফেরত সত্য;
}

স্থির hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  শূন্য               *user_তথ্য)
{
  ধ্রুবক UNormalizer2 *normalizer = (ধ্রুবক UNormalizer2 *) user_তথ্য;
  UChar decomposed[4];
  পূর্ণসংখ্যা len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_দৈর্ঘ্য (decomposed), &icu_err);
  যদি (U_FAILURE (icu_err) || len < 0) ফেরত মিথ্যা;

  len = u_গুনতি_Char32 (decomposed, len);
  যদি (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    ফেরত *a != ab;
  }
  অন্যথা যদি (len == 2)
  {
    len = 0;
    U16_পরবর্তী_UNSAFE (decomposed, len, *a);
    U16_পরবর্তী_UNSAFE (decomposed, len, *b);
  }
  ফেরত সত্য;
}


স্থির অন্তঃরেখ শূন্য free_static_icu_funcs ();

স্থির কাঠামো hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  স্থির hb_unicode_funcs_t *create ()
  {
    শূন্য *user_তথ্য = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_তথ্য = (শূন্য *) unorm2_getNFCInstance (&icu_err);
    প্রতিজ্ঞা (user_তথ্য);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_তথ্য, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_তথ্য, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    ফেরত funcs;
  }
} static_icu_funcs;

স্থির অন্তঃরেখ
শূন্য free_static_icu_funcs ()
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
  ফেরত static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic বের_করো

#endif
