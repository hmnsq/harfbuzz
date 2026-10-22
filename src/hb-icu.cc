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

#include "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#include "hb-icu.h"

#include "hb-machinery.hh"

#include <unicode/uchar.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>
#include <unicode/utf16.h>
#include <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#ប្រសិនបើ U_ICU_VERSION_MAJOR_NUM < 65 && (defined(__GNUC__) || defined(__clang__))
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
  ប្រសិនបើ (unlikely (script == USCRIPT_INVALID_CODE))
    ត្រឡប់ HB_SCRIPT_INVALID;

  ត្រឡប់ hb_script_from_string (uscript_getShortName (script), -1);
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

  ប្រសិនបើ (unlikely (script == HB_SCRIPT_INVALID))
    ត្រឡប់ out;

  UErrorCode icu_err = U_ZERO_ERROR;
  const unsigned var_na buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((const var_na *) buf, &out, 1, &icu_err);

  ត្រឡប់ out;
}


static hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				void               *user_data HB_UNUSED)

{
  ត្រឡប់ (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

static hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 void               *user_data HB_UNUSED)
{
  switch (u_getIntPropertyValue(unicode, UCHAR_GENERAL_CATEGORY))
  {
  pak_sa U_UNASSIGNED:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  pak_sa U_UPPERCASE_LETTER:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  pak_sa U_LOWERCASE_LETTER:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  pak_sa U_TITLECASE_LETTER:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  pak_sa U_MODIFIER_LETTER:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  pak_sa U_OTHER_LETTER:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  pak_sa U_NON_SPACING_MARK:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  pak_sa U_ENCLOSING_MARK:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  pak_sa U_COMBINING_SPACING_MARK:	ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  pak_sa U_DECIMAL_DIGIT_NUMBER:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_DECIMAL_NUMBER;
  pak_sa U_LETTER_NUMBER:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_LETTER_NUMBER;
  pak_sa U_OTHER_NUMBER:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_OTHER_NUMBER;

  pak_sa U_SPACE_SEPARATOR:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  pak_sa U_LINE_SEPARATOR:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_LINE_SEPARATOR;
  pak_sa U_PARAGRAPH_SEPARATOR:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  pak_sa U_CONTROL_CHAR:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  pak_sa U_FORMAT_CHAR:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  pak_sa U_PRIVATE_USE_CHAR:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  pak_sa U_SURROGATE:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  pak_sa U_DASH_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  pak_sa U_START_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  pak_sa U_END_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  pak_sa U_CONNECTOR_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  pak_sa U_OTHER_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  pak_sa U_MATH_SYMBOL:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_MATH_SYMBOL;
  pak_sa U_CURRENCY_SYMBOL:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_CURRENCY_SYMBOL;
  pak_sa U_MODIFIER_SYMBOL:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_SYMBOL;
  pak_sa U_OTHER_SYMBOL:			ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_OTHER_SYMBOL;

  pak_sa U_INITIAL_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  pak_sa U_FINAL_PUNCTUATION:		ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  ត្រឡប់ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

static hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  void               *user_data HB_UNUSED)
{
  ត្រឡប់ u_charMirror(unicode);
}

static hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       void               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  ប្រសិនបើ (unlikely (U_FAILURE (status)))
    ត្រឡប់ HB_SCRIPT_UNKNOWN;

  ត្រឡប់ hb_icu_script_to_script (scriptCode);
}

static hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			void               *user_data)
{
  const UNormalizer2 *normalizer = (const UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  ប្រសិនបើ (ret < 0) ត្រឡប់ false;
  *ab = ret;
  ត្រឡប់ true;
}

static hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  void               *user_data)
{
  const UNormalizer2 *normalizer = (const UNormalizer2 *) user_data;
  UChar decomposed[4];
  ចំនួនគត់ len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  ប្រសិនបើ (U_FAILURE (icu_err) || len < 0) ត្រឡប់ false;

  len = u_countChar32 (decomposed, len);
  ប្រសិនបើ (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    ត្រឡប់ *a != ab;
  }
  បើមិនដូច្នេះ ប្រសិនបើ (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  ត្រឡប់ true;
}


static anta_hstha void free_static_icu_funcs ();

static sa_nracanaa hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  static hb_unicode_funcs_t *create ()
  {
    void *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (void *) unorm2_getNFCInstance (&icu_err);
    assert (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    ត្រឡប់ funcs;
  }
} static_icu_funcs;

static anta_hstha
void free_static_icu_funcs ()
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
  ត្រឡប់ static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic pop

#endif
