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

#अन्तर्भाव "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#अन्तर्भाव "hb-icu.h"

#अन्तर्भाव "hb-machinery.hh"

#अन्तर्भाव <unicode/uchar.h>
#अन्तर्भाव <unicode/unorm2.h>
#अन्तर्भाव <unicode/ustring.h>
#अन्तर्भाव <unicode/utf16.h>
#अन्तर्भाव <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#जर U_ICU_VERSION_MAJOR_अंक < 65 && (defined(__GNUC__) || defined(__clang__))
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
  जर (unlikely (script == USCRIPT_INVALID_CODE))
    परत HB_SCRIPT_INVALID;

  परत hb_script_from_string (uscript_getShortName (script), -1);
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

  जर (unlikely (script == HB_SCRIPT_INVALID))
    परत out;

  UErrorCode icu_err = U_ZERO_ERROR;
  स्थिरांक अचिन्हित अक्षर buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((स्थिरांक अक्षर *) buf, &out, 1, &icu_err);

  परत out;
}


स्थिर hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				शून्य               *user_data HB_UNUSED)

{
  परत (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

स्थिर hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 शून्य               *user_data HB_UNUSED)
{
  निवड (u_get_Int_Property_मूल्य(unicode, UCHAR_GENERAL_CATEGORY))
  {
  स्थिती U_UNASSIGNED:			परत HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  स्थिती U_UPPERCASE_LETTER:		परत HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  स्थिती U_LOWERCASE_LETTER:		परत HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  स्थिती U_TITLECASE_LETTER:		परत HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  स्थिती U_MODIFIER_LETTER:		परत HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  स्थिती U_OTHER_LETTER:			परत HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  स्थिती U_NON_SPACING_MARK:		परत HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  स्थिती U_ENCLOSING_MARK:		परत HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  स्थिती U_COMBINING_SPACING_MARK:	परत HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  स्थिती U_DECIMAL_DIGIT_संख्या:		परत HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या;
  स्थिती U_LETTER_संख्या:			परत HB_UNICODE_GENERAL_CATEGORY_LETTER_संख्या;
  स्थिती U_OTHER_संख्या:			परत HB_UNICODE_GENERAL_CATEGORY_OTHER_संख्या;

  स्थिती U_SPACE_SEPARATOR:		परत HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  स्थिती U_ओळ_SEPARATOR:		परत HB_UNICODE_GENERAL_CATEGORY_ओळ_SEPARATOR;
  स्थिती U_PARAGRAPH_SEPARATOR:		परत HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  स्थिती U_CONTROL_CHAR:			परत HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  स्थिती U_FORMAT_CHAR:			परत HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  स्थिती U_PRIVATE_USE_CHAR:		परत HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  स्थिती U_SURROGATE:			परत HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  स्थिती U_DASH_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  स्थिती U_START_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  स्थिती U_END_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  स्थिती U_CONNECTOR_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  स्थिती U_OTHER_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  स्थिती U_MATH_चिन्ह:			परत HB_UNICODE_GENERAL_CATEGORY_MATH_चिन्ह;
  स्थिती U_CURRENCY_चिन्ह:		परत HB_UNICODE_GENERAL_CATEGORY_CURRENCY_चिन्ह;
  स्थिती U_MODIFIER_चिन्ह:		परत HB_UNICODE_GENERAL_CATEGORY_MODIFIER_चिन्ह;
  स्थिती U_OTHER_चिन्ह:			परत HB_UNICODE_GENERAL_CATEGORY_OTHER_चिन्ह;

  स्थिती U_INITIAL_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  स्थिती U_FINAL_PUNCTUATION:		परत HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  परत HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

स्थिर hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  शून्य               *user_data HB_UNUSED)
{
  परत u_charMirror(unicode);
}

स्थिर hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       शून्य               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  जर (unlikely (U_FAILURE (status)))
    परत HB_SCRIPT_UNKNOWN;

  परत hb_icu_script_to_script (scriptCode);
}

स्थिर hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			शून्य               *user_data)
{
  स्थिरांक UNormalizer2 *normalizer = (स्थिरांक UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  जर (ret < 0) परत असत्य;
  *ab = ret;
  परत सत्य;
}

स्थिर hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  शून्य               *user_data)
{
  स्थिरांक UNormalizer2 *normalizer = (स्थिरांक UNormalizer2 *) user_data;
  UChar decomposed[4];
  पूर्णांक len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  जर (U_FAILURE (icu_err) || len < 0) परत असत्य;

  len = u_मोजणी_Char32 (decomposed, len);
  जर (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    परत *a != ab;
  }
  अन्यथा जर (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  परत सत्य;
}


स्थिर ओळीत शून्य free_static_icu_funcs ();

स्थिर संरचना hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  स्थिर hb_unicode_funcs_t *create ()
  {
    शून्य *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (शून्य *) unorm2_getNFCInstance (&icu_err);
    प्रतिज्ञा (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    परत funcs;
  }
} static_icu_funcs;

स्थिर ओळीत
शून्य free_static_icu_funcs ()
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
  परत static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic काढा

#endif
