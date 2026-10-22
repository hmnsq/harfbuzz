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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#ಅನ್ತರ್ಭಾವ "hb-icu.h"

#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"

#ಅನ್ತರ್ಭಾವ <unicode/uchar.h>
#ಅನ್ತರ್ಭಾವ <unicode/unorm2.h>
#ಅನ್ತರ್ಭಾವ <unicode/ustring.h>
#ಅನ್ತರ್ಭಾವ <unicode/utf16.h>
#ಅನ್ತರ್ಭಾವ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#ಯದಿ U_ICU_VERSION_MAJOR_ಅಂಕೆ < 65 && (defined(__GNUC__) || defined(__clang__))
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
  ಯದಿ (unlikely (script == USCRIPT_INVALID_CODE))
    ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_INVALID;

  ಪ್ರತ್ಯಾಗಮ hb_script_from_string (uscript_getShortName (script), -1);
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

  ಯದಿ (unlikely (script == HB_SCRIPT_INVALID))
    ಪ್ರತ್ಯಾಗಮ out;

  UErrorCode icu_err = U_ZERO_ERROR;
  ಧ್ರುವ unsigned ವರ್ಣ buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((ಧ್ರುವ ವರ್ಣ *) buf, &out, 1, &icu_err);

  ಪ್ರತ್ಯಾಗಮ out;
}


ಸ್ಥಿರ hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				ಶೂನ್ಯ               *user_data HB_UNUSED)

{
  ಪ್ರತ್ಯಾಗಮ (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ಸ್ಥಿರ hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 ಶೂನ್ಯ               *user_data HB_UNUSED)
{
  ವಿಕಲ್ಪ (u_get_Int_Property_ಮೌಲ್ಯ(unicode, UCHAR_GENERAL_CATEGORY))
  {
  ಪಕ್ಷ U_UNASSIGNED:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  ಪಕ್ಷ U_UPPERCASE_LETTER:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  ಪಕ್ಷ U_LOWERCASE_LETTER:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  ಪಕ್ಷ U_TITLECASE_LETTER:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  ಪಕ್ಷ U_MODIFIER_LETTER:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  ಪಕ್ಷ U_OTHER_LETTER:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  ಪಕ್ಷ U_NON_SPACING_MARK:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  ಪಕ್ಷ U_ENCLOSING_MARK:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  ಪಕ್ಷ U_COMBINING_SPACING_MARK:	ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  ಪಕ್ಷ U_DECIMAL_DIGIT_ಸಂಖ್ಯೆ:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ಸಂಖ್ಯೆ;
  ಪಕ್ಷ U_LETTER_ಸಂಖ್ಯೆ:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_LETTER_ಸಂಖ್ಯೆ;
  ಪಕ್ಷ U_OTHER_ಸಂಖ್ಯೆ:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_OTHER_ಸಂಖ್ಯೆ;

  ಪಕ್ಷ U_SPACE_SEPARATOR:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  ಪಕ್ಷ U_ಸಾಲು_SEPARATOR:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_ಸಾಲು_SEPARATOR;
  ಪಕ್ಷ U_PARAGRAPH_SEPARATOR:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  ಪಕ್ಷ U_CONTROL_CHAR:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  ಪಕ್ಷ U_FORMAT_CHAR:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  ಪಕ್ಷ U_PRIVATE_USE_CHAR:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  ಪಕ್ಷ U_SURROGATE:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  ಪಕ್ಷ U_DASH_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  ಪಕ್ಷ U_START_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  ಪಕ್ಷ U_END_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  ಪಕ್ಷ U_CONNECTOR_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  ಪಕ್ಷ U_OTHER_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  ಪಕ್ಷ U_MATH_ಚಿಹ್ನೆ:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_MATH_ಚಿಹ್ನೆ;
  ಪಕ್ಷ U_CURRENCY_ಚಿಹ್ನೆ:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_CURRENCY_ಚಿಹ್ನೆ;
  ಪಕ್ಷ U_MODIFIER_ಚಿಹ್ನೆ:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ಚಿಹ್ನೆ;
  ಪಕ್ಷ U_OTHER_ಚಿಹ್ನೆ:			ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_OTHER_ಚಿಹ್ನೆ;

  ಪಕ್ಷ U_INITIAL_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  ಪಕ್ಷ U_FINAL_PUNCTUATION:		ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  ಪ್ರತ್ಯಾಗಮ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ಸ್ಥಿರ hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  ಶೂನ್ಯ               *user_data HB_UNUSED)
{
  ಪ್ರತ್ಯಾಗಮ u_charMirror(unicode);
}

ಸ್ಥಿರ hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       ಶೂನ್ಯ               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  ಯದಿ (unlikely (U_FAILURE (status)))
    ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_UNKNOWN;

  ಪ್ರತ್ಯಾಗಮ hb_icu_script_to_script (scriptCode);
}

ಸ್ಥಿರ hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			ಶೂನ್ಯ               *user_data)
{
  ಧ್ರುವ UNormalizer2 *normalizer = (ಧ್ರುವ UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  ಯದಿ (ret < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  *ab = ret;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  ಶೂನ್ಯ               *user_data)
{
  ಧ್ರುವ UNormalizer2 *normalizer = (ಧ್ರುವ UNormalizer2 *) user_data;
  UChar decomposed[4];
  ಪೂರ್ಣಾಙ್ಕ len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  ಯದಿ (U_FAILURE (icu_err) || len < 0) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  len = u_ಎಣಿಕೆ_Char32 (decomposed, len);
  ಯದಿ (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    ಪ್ರತ್ಯಾಗಮ *a != ab;
  }
  ಅನ್ಯಥಾ ಯದಿ (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_icu_funcs ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ಸ್ಥಿರ hb_unicode_funcs_t *create ()
  {
    ಶೂನ್ಯ *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (ಶೂನ್ಯ *) unorm2_getNFCInstance (&icu_err);
    ಪ್ರತಿಜ್ಞಾ (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    ಪ್ರತ್ಯಾಗಮ funcs;
  }
} static_icu_funcs;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_icu_funcs ()
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
  ಪ್ರತ್ಯಾಗಮ static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic ತೆಗೆ

#endif
