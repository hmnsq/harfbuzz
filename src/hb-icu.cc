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

#അന്തര്ഭാവ "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#അന്തര്ഭാവ "hb-icu.h"

#അന്തര്ഭാവ "hb-machinery.hh"

#അന്തര്ഭാവ <unicode/uchar.h>
#അന്തര്ഭാവ <unicode/unorm2.h>
#അന്തര്ഭാവ <unicode/ustring.h>
#അന്തര്ഭാവ <unicode/utf16.h>
#അന്തര്ഭാവ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#എങ്കിൽ U_ICU_VERSION_MAJOR_അക്കം < 65 && (defined(__GNUC__) || defined(__clang__))
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
  എങ്കിൽ (unlikely (script == USCRIPT_INVALID_CODE))
    തിരികെ HB_SCRIPT_INVALID;

  തിരികെ hb_script_from_string (uscript_getShortName (script), -1);
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

  എങ്കിൽ (unlikely (script == HB_SCRIPT_INVALID))
    തിരികെ out;

  UErrorCode icu_err = U_ZERO_ERROR;
  സ്ഥിരം unsigned അക്ഷരം buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((സ്ഥിരം അക്ഷരം *) buf, &out, 1, &icu_err);

  തിരികെ out;
}


സ്ഥിതം hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				ശൂന്യം               *user_data HB_UNUSED)

{
  തിരികെ (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

സ്ഥിതം hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 ശൂന്യം               *user_data HB_UNUSED)
{
  തിരഞ്ഞെടുപ്പ് (u_get_Int_Property_മൂല്യം(unicode, UCHAR_GENERAL_CATEGORY))
  {
  അവസ്ഥ U_UNASSIGNED:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  അവസ്ഥ U_UPPERCASE_LETTER:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  അവസ്ഥ U_LOWERCASE_LETTER:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  അവസ്ഥ U_TITLECASE_LETTER:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  അവസ്ഥ U_MODIFIER_LETTER:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  അവസ്ഥ U_OTHER_LETTER:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  അവസ്ഥ U_NON_SPACING_MARK:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  അവസ്ഥ U_ENCLOSING_MARK:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  അവസ്ഥ U_COMBINING_SPACING_MARK:	തിരികെ HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  അവസ്ഥ U_DECIMAL_DIGIT_സംഖ്യ:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_DECIMAL_സംഖ്യ;
  അവസ്ഥ U_LETTER_സംഖ്യ:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_LETTER_സംഖ്യ;
  അവസ്ഥ U_OTHER_സംഖ്യ:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_OTHER_സംഖ്യ;

  അവസ്ഥ U_SPACE_SEPARATOR:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  അവസ്ഥ U_വരി_SEPARATOR:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_വരി_SEPARATOR;
  അവസ്ഥ U_PARAGRAPH_SEPARATOR:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  അവസ്ഥ U_CONTROL_CHAR:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  അവസ്ഥ U_FORMAT_CHAR:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  അവസ്ഥ U_PRIVATE_USE_CHAR:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  അവസ്ഥ U_SURROGATE:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  അവസ്ഥ U_DASH_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  അവസ്ഥ U_START_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  അവസ്ഥ U_END_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  അവസ്ഥ U_CONNECTOR_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  അവസ്ഥ U_OTHER_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  അവസ്ഥ U_MATH_ചിഹ്നം:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_MATH_ചിഹ്നം;
  അവസ്ഥ U_CURRENCY_ചിഹ്നം:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_CURRENCY_ചിഹ്നം;
  അവസ്ഥ U_MODIFIER_ചിഹ്നം:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ചിഹ്നം;
  അവസ്ഥ U_OTHER_ചിഹ്നം:			തിരികെ HB_UNICODE_GENERAL_CATEGORY_OTHER_ചിഹ്നം;

  അവസ്ഥ U_INITIAL_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  അവസ്ഥ U_FINAL_PUNCTUATION:		തിരികെ HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  തിരികെ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

സ്ഥിതം hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  ശൂന്യം               *user_data HB_UNUSED)
{
  തിരികെ u_charMirror(unicode);
}

സ്ഥിതം hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       ശൂന്യം               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  എങ്കിൽ (unlikely (U_FAILURE (status)))
    തിരികെ HB_SCRIPT_UNKNOWN;

  തിരികെ hb_icu_script_to_script (scriptCode);
}

സ്ഥിതം hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			ശൂന്യം               *user_data)
{
  സ്ഥിരം UNormalizer2 *normalizer = (സ്ഥിരം UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  എങ്കിൽ (ret < 0) തിരികെ അസത്യം;
  *ab = ret;
  തിരികെ സത്യം;
}

സ്ഥിതം hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  ശൂന്യം               *user_data)
{
  സ്ഥിരം UNormalizer2 *normalizer = (സ്ഥിരം UNormalizer2 *) user_data;
  UChar decomposed[4];
  പൂർണ്ണസംഖ്യ len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  എങ്കിൽ (U_FAILURE (icu_err) || len < 0) തിരികെ അസത്യം;

  len = u_എണ്ണം_Char32 (decomposed, len);
  എങ്കിൽ (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    തിരികെ *a != ab;
  }
  അല്ലെങ്കിൽ എങ്കിൽ (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  തിരികെ സത്യം;
}


സ്ഥിതം വരിയിൽ ശൂന്യം free_static_icu_funcs ();

സ്ഥിതം ഘടന hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_unicode_funcs_t *create ()
  {
    ശൂന്യം *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (ശൂന്യം *) unorm2_getNFCInstance (&icu_err);
    പ്രതിജ്ഞാ (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    തിരികെ funcs;
  }
} static_icu_funcs;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_icu_funcs ()
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
  തിരികെ static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic എടുക്കുക

#endif
