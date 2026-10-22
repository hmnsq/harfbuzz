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

#شامل "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#شامل "hb-icu.h"

#شامل "hb-machinery.hh"

#شامل <unicode/uchar.h>
#شامل <unicode/unorm2.h>
#شامل <unicode/ustring.h>
#شامل <unicode/utf16.h>
#شامل <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#اگر U_ICU_VERSION_MAJOR_رقم < 65 && (defined(__GNUC__) || defined(__clang__))
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
  اگر (unlikely (script == USCRIPT_INVALID_CODE))
    بازگشت HB_SCRIPT_INVALID;

  بازگشت hb_script_from_string (uscript_getShortName (script), -1);
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

  اگر (unlikely (script == HB_SCRIPT_INVALID))
    بازگشت out;

  UErrorCode icu_err = U_ZERO_ERROR;
  ثابت unsigned نویسه buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((ثابت نویسه *) buf, &out, 1, &icu_err);

  بازگشت out;
}


ایستا hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				تهی               *user_data HB_UNUSED)

{
  بازگشت (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ایستا hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 تهی               *user_data HB_UNUSED)
{
  انتخاب (u_get_Int_Property_مقدار(unicode, UCHAR_GENERAL_CATEGORY))
  {
  حالت U_UNASSIGNED:			بازگشت HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  حالت U_UPPERCASE_LETTER:		بازگشت HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  حالت U_LOWERCASE_LETTER:		بازگشت HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  حالت U_TITLECASE_LETTER:		بازگشت HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  حالت U_MODIFIER_LETTER:		بازگشت HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  حالت U_OTHER_LETTER:			بازگشت HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  حالت U_NON_SPACING_MARK:		بازگشت HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  حالت U_ENCLOSING_MARK:		بازگشت HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  حالت U_COMBINING_SPACING_MARK:	بازگشت HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  حالت U_DECIMAL_DIGIT_عدد:		بازگشت HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد;
  حالت U_LETTER_عدد:			بازگشت HB_UNICODE_GENERAL_CATEGORY_LETTER_عدد;
  حالت U_OTHER_عدد:			بازگشت HB_UNICODE_GENERAL_CATEGORY_OTHER_عدد;

  حالت U_SPACE_SEPARATOR:		بازگشت HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  حالت U_سطر_SEPARATOR:		بازگشت HB_UNICODE_GENERAL_CATEGORY_سطر_SEPARATOR;
  حالت U_PARAGRAPH_SEPARATOR:		بازگشت HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  حالت U_CONTROL_CHAR:			بازگشت HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  حالت U_FORMAT_CHAR:			بازگشت HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  حالت U_PRIVATE_USE_CHAR:		بازگشت HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  حالت U_SURROGATE:			بازگشت HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  حالت U_DASH_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  حالت U_START_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  حالت U_END_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  حالت U_CONNECTOR_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  حالت U_OTHER_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  حالت U_MATH_نماد:			بازگشت HB_UNICODE_GENERAL_CATEGORY_MATH_نماد;
  حالت U_CURRENCY_نماد:		بازگشت HB_UNICODE_GENERAL_CATEGORY_CURRENCY_نماد;
  حالت U_MODIFIER_نماد:		بازگشت HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نماد;
  حالت U_OTHER_نماد:			بازگشت HB_UNICODE_GENERAL_CATEGORY_OTHER_نماد;

  حالت U_INITIAL_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  حالت U_FINAL_PUNCTUATION:		بازگشت HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  بازگشت HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ایستا hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  تهی               *user_data HB_UNUSED)
{
  بازگشت u_charMirror(unicode);
}

ایستا hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       تهی               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  اگر (unlikely (U_FAILURE (status)))
    بازگشت HB_SCRIPT_UNKNOWN;

  بازگشت hb_icu_script_to_script (scriptCode);
}

ایستا hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			تهی               *user_data)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  اگر (ret < 0) بازگشت نادرست;
  *ab = ret;
  بازگشت درست;
}

ایستا hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  تهی               *user_data)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_data;
  UChar decomposed[4];
  صحیح len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  اگر (U_FAILURE (icu_err) || len < 0) بازگشت نادرست;

  len = u_شمار_Char32 (decomposed, len);
  اگر (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    بازگشت *a != ab;
  }
  وگرنه اگر (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  بازگشت درست;
}


ایستا درونخطی تهی free_static_icu_funcs ();

ایستا ساختار hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ایستا hb_unicode_funcs_t *create ()
  {
    تهی *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (تهی *) unorm2_getNFCInstance (&icu_err);
    أكد (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    بازگشت funcs;
  }
} static_icu_funcs;

ایستا درونخطی
تهی free_static_icu_funcs ()
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
  بازگشت static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic برداشت

#endif
