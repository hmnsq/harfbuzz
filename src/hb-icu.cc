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

#समावेश "hb.hh"

#अगर_घोषित HAVE_ICU

#आशय GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#आशय GCC diagnostic ignored "-Wredundant-decls"

#समावेश "hb-icu.h"

#समावेश "hb-machinery.hh"

#समावेश <unicode/uchar.h>
#समावेश <unicode/unorm2.h>
#समावेश <unicode/ustring.h>
#समावेश <unicode/utf16.h>
#समावेश <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#अगर U_ICU_VERSION_MAJOR_अंक < 65 && (defined(__GNUC__) || defined(__clang__))
#घोषणा HB_ICU_EXTRA_SEMI_IGNORED
#आशय GCC diagnostic ignored "-Wextra-semi-stmt"
#पूर्ण_अगर

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
  अगर (unlikely (script == USCRIPT_INVALID_CODE))
    वापस HB_SCRIPT_INVALID;

  वापस hb_script_from_string (uscript_getShortName (script), -1);
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

  अगर (unlikely (script == HB_SCRIPT_INVALID))
    वापस out;

  UErrorCode icu_err = U_ZERO_ERROR;
  स्थिर अचिन्हित अक्षर buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((स्थिर अक्षर *) buf, &out, 1, &icu_err);

  वापस out;
}


अटल hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				व्योम               *user_तथ्य HB_UNUSED)

{
  वापस (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

अटल hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 व्योम               *user_तथ्य HB_UNUSED)
{
  चयन (u_get_Int_Property_मूल्य(unicode, UCHAR_GENERAL_CATEGORY))
  {
  हाल U_UNASSIGNED:			वापस HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  हाल U_UPPERCASE_LETTER:		वापस HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  हाल U_LOWERCASE_LETTER:		वापस HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  हाल U_TITLECASE_LETTER:		वापस HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  हाल U_MODIFIER_LETTER:		वापस HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  हाल U_OTHER_LETTER:			वापस HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  हाल U_NON_SPACING_MARK:		वापस HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  हाल U_ENCLOSING_MARK:		वापस HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  हाल U_COMBINING_SPACING_MARK:	वापस HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  हाल U_DECIMAL_DIGIT_संख्या:		वापस HB_UNICODE_GENERAL_CATEGORY_DECIMAL_संख्या;
  हाल U_LETTER_संख्या:			वापस HB_UNICODE_GENERAL_CATEGORY_LETTER_संख्या;
  हाल U_OTHER_संख्या:			वापस HB_UNICODE_GENERAL_CATEGORY_OTHER_संख्या;

  हाल U_SPACE_SEPARATOR:		वापस HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  हाल U_पंक्ति_SEPARATOR:		वापस HB_UNICODE_GENERAL_CATEGORY_पंक्ति_SEPARATOR;
  हाल U_PARAGRAPH_SEPARATOR:		वापस HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  हाल U_CONTROL_CHAR:			वापस HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  हाल U_FORMAT_CHAR:			वापस HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  हाल U_PRIVATE_USE_CHAR:		वापस HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  हाल U_SURROGATE:			वापस HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  हाल U_DASH_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  हाल U_START_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  हाल U_END_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  हाल U_CONNECTOR_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  हाल U_OTHER_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  हाल U_MATH_प्रतीक:			वापस HB_UNICODE_GENERAL_CATEGORY_MATH_प्रतीक;
  हाल U_CURRENCY_प्रतीक:		वापस HB_UNICODE_GENERAL_CATEGORY_CURRENCY_प्रतीक;
  हाल U_MODIFIER_प्रतीक:		वापस HB_UNICODE_GENERAL_CATEGORY_MODIFIER_प्रतीक;
  हाल U_OTHER_प्रतीक:			वापस HB_UNICODE_GENERAL_CATEGORY_OTHER_प्रतीक;

  हाल U_INITIAL_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  हाल U_FINAL_PUNCTUATION:		वापस HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  वापस HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

अटल hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  व्योम               *user_तथ्य HB_UNUSED)
{
  वापस u_charMirror(unicode);
}

अटल hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       व्योम               *user_तथ्य HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  अगर (unlikely (U_FAILURE (status)))
    वापस HB_SCRIPT_UNKNOWN;

  वापस hb_icu_script_to_script (scriptCode);
}

अटल hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			व्योम               *user_तथ्य)
{
  स्थिर UNormalizer2 *normalizer = (स्थिर UNormalizer2 *) user_तथ्य;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  अगर (ret < 0) वापस false;
  *ab = ret;
  वापस true;
}

अटल hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  व्योम               *user_तथ्य)
{
  स्थिर UNormalizer2 *normalizer = (स्थिर UNormalizer2 *) user_तथ्य;
  UChar decomposed[4];
  पूर्णांक len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_लंबाई (decomposed), &icu_err);
  अगर (U_FAILURE (icu_err) || len < 0) वापस false;

  len = u_गिनती_Char32 (decomposed, len);
  अगर (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    वापस *a != ab;
  }
  अन्यथा अगर (len == 2)
  {
    len = 0;
    U16_अग्र_UNSAFE (decomposed, len, *a);
    U16_अग्र_UNSAFE (decomposed, len, *b);
  }
  वापस true;
}


अटल अंतरभूत व्योम free_static_icu_funcs ();

अटल काष्ठा hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  अटल hb_unicode_funcs_t *create ()
  {
    व्योम *user_तथ्य = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_तथ्य = (व्योम *) unorm2_getNFCInstance (&icu_err);
    निश्चित (user_तथ्य);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_तथ्य, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_तथ्य, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    वापस funcs;
  }
} static_icu_funcs;

अटल अंतरभूत
व्योम free_static_icu_funcs ()
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
  वापस static_icu_funcs.get_unconst ();
}

#आशय GCC diagnostic निकालो

#पूर्ण_अगर
