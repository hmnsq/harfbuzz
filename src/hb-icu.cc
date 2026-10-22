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

#અન્તર્ભાવ "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#અન્તર્ભાવ "hb-icu.h"

#અન્તર્ભાવ "hb-machinery.hh"

#અન્તર્ભાવ <unicode/uchar.h>
#અન્તર્ભાવ <unicode/unorm2.h>
#અન્તર્ભાવ <unicode/ustring.h>
#અન્તર્ભાવ <unicode/utf16.h>
#અન્તર્ભાવ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#જો U_ICU_VERSION_MAJOR_અંક < 65 && (defined(__GNUC__) || defined(__clang__))
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
  જો (unlikely (script == USCRIPT_INVALID_CODE))
    પાછા HB_SCRIPT_INVALID;

  પાછા hb_script_from_string (uscript_getShortName (script), -1);
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

  જો (unlikely (script == HB_SCRIPT_INVALID))
    પાછા out;

  UErrorCode icu_err = U_ZERO_ERROR;
  અચલ unsigned અક્ષર buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((અચલ અક્ષર *) buf, &out, 1, &icu_err);

  પાછા out;
}


સ્થિર hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				શૂન્ય               *user_data HB_UNUSED)

{
  પાછા (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

સ્થિર hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 શૂન્ય               *user_data HB_UNUSED)
{
  પસંદગી (u_get_Int_Property_કિંમત(unicode, UCHAR_GENERAL_CATEGORY))
  {
  સ્થિતિ U_UNASSIGNED:			પાછા HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  સ્થિતિ U_UPPERCASE_LETTER:		પાછા HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  સ્થિતિ U_LOWERCASE_LETTER:		પાછા HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  સ્થિતિ U_TITLECASE_LETTER:		પાછા HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  સ્થિતિ U_MODIFIER_LETTER:		પાછા HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  સ્થિતિ U_OTHER_LETTER:			પાછા HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  સ્થિતિ U_NON_SPACING_MARK:		પાછા HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  સ્થિતિ U_ENCLOSING_MARK:		પાછા HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  સ્થિતિ U_COMBINING_SPACING_MARK:	પાછા HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  સ્થિતિ U_DECIMAL_DIGIT_સંખ્યા:		પાછા HB_UNICODE_GENERAL_CATEGORY_DECIMAL_સંખ્યા;
  સ્થિતિ U_LETTER_સંખ્યા:			પાછા HB_UNICODE_GENERAL_CATEGORY_LETTER_સંખ્યા;
  સ્થિતિ U_OTHER_સંખ્યા:			પાછા HB_UNICODE_GENERAL_CATEGORY_OTHER_સંખ્યા;

  સ્થિતિ U_SPACE_SEPARATOR:		પાછા HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  સ્થિતિ U_લીટી_SEPARATOR:		પાછા HB_UNICODE_GENERAL_CATEGORY_લીટી_SEPARATOR;
  સ્થિતિ U_PARAGRAPH_SEPARATOR:		પાછા HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  સ્થિતિ U_CONTROL_CHAR:			પાછા HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  સ્થિતિ U_FORMAT_CHAR:			પાછા HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  સ્થિતિ U_PRIVATE_USE_CHAR:		પાછા HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  સ્થિતિ U_SURROGATE:			પાછા HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  સ્થિતિ U_DASH_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  સ્થિતિ U_START_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  સ્થિતિ U_END_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  સ્થિતિ U_CONNECTOR_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  સ્થિતિ U_OTHER_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  સ્થિતિ U_MATH_પ્રતીક:			પાછા HB_UNICODE_GENERAL_CATEGORY_MATH_પ્રતીક;
  સ્થિતિ U_CURRENCY_પ્રતીક:		પાછા HB_UNICODE_GENERAL_CATEGORY_CURRENCY_પ્રતીક;
  સ્થિતિ U_MODIFIER_પ્રતીક:		પાછા HB_UNICODE_GENERAL_CATEGORY_MODIFIER_પ્રતીક;
  સ્થિતિ U_OTHER_પ્રતીક:			પાછા HB_UNICODE_GENERAL_CATEGORY_OTHER_પ્રતીક;

  સ્થિતિ U_INITIAL_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  સ્થિતિ U_FINAL_PUNCTUATION:		પાછા HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  પાછા HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

સ્થિર hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  શૂન્ય               *user_data HB_UNUSED)
{
  પાછા u_charMirror(unicode);
}

સ્થિર hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       શૂન્ય               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  જો (unlikely (U_FAILURE (status)))
    પાછા HB_SCRIPT_UNKNOWN;

  પાછા hb_icu_script_to_script (scriptCode);
}

સ્થિર hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			શૂન્ય               *user_data)
{
  અચલ UNormalizer2 *normalizer = (અચલ UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  જો (ret < 0) પાછા ખોટું;
  *ab = ret;
  પાછા સાચું;
}

સ્થિર hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  શૂન્ય               *user_data)
{
  અચલ UNormalizer2 *normalizer = (અચલ UNormalizer2 *) user_data;
  UChar decomposed[4];
  પૂર્ણાંક len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  જો (U_FAILURE (icu_err) || len < 0) પાછા ખોટું;

  len = u_ગણતરી_Char32 (decomposed, len);
  જો (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    પાછા *a != ab;
  }
  નહીંતર જો (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  પાછા સાચું;
}


સ્થિર અંતઃરેખ શૂન્ય free_static_icu_funcs ();

સ્થિર સંરચના hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  સ્થિર hb_unicode_funcs_t *create ()
  {
    શૂન્ય *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (શૂન્ય *) unorm2_getNFCInstance (&icu_err);
    પ્રતિજ્ઞા (user_data);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_icu_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_icu_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_icu_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_icu_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_icu_unicode_compose, user_data, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_icu_unicode_decompose, user_data, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    પાછા funcs;
  }
} static_icu_funcs;

સ્થિર અંતઃરેખ
શૂન્ય free_static_icu_funcs ()
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
  પાછા static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic કાઢો

#endif
