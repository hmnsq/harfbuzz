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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ICU

#ਹਦਾਇਤ GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#ਹਦਾਇਤ GCC diagnostic ignored "-Wredundant-decls"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-icu.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <unicode/uchar.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unicode/unorm2.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unicode/ustring.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unicode/utf16.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#ਜੇ U_ICU_ਸੰਸਕਰਣ_MAJOR_ਅੰਕ < 65 && (defined(__GNUC__) || defined(__clang__))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ICU_EXTRA_SEMI_IGNORED
#ਹਦਾਇਤ GCC diagnostic ignored "-Wextra-semi-stmt"
#ਜੇ_ਅੰਤ

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
hb_icu_script_ਮੰਜ਼ਿਲ_script (UScript_ਕੋਡ script)
{
  ਜੇ (unlikely (script == USCRIPT_INVALID_ਕੋਡ))
    ਵਾਪਸ HB_SCRIPT_INVALID;

  ਵਾਪਸ hb_script_from_string (uscript_getShortName (script), -1);
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
UScript_ਕੋਡ
hb_icu_script_from_script (hb_script_t script)
{
  UScript_ਕੋਡ ਬਾਹਰਲਾ = USCRIPT_INVALID_ਕੋਡ;

  ਜੇ (unlikely (script == HB_SCRIPT_INVALID))
    ਵਾਪਸ ਬਾਹਰਲਾ;

  UError_ਕੋਡ icu_err = U_ZERO_ERROR;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ buf[5] = {HB_UNTAG (script), 0};
  uscript_get_ਕੋਡ ((ਅਚਲ ਅੱਖਰ *) buf, &ਬਾਹਰਲਾ, 1, &icu_err);

  ਵਾਪਸ ਬਾਹਰਲਾ;
}


ਸਥਿਰ hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
				hb_codepoint_t      unicode,
				ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)

{
  ਵਾਪਸ (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ਸਥਿਰ hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t      unicode,
				 ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਚੋਣ (u_get_Int_Property_ਮੁੱਲ(unicode, UCHAR_GENERAL_CATEGORY))
  {
  ਹਾਲਤ U_UNASSIGNED:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  ਹਾਲਤ U_UPPERCASE_LETTER:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  ਹਾਲਤ U_LOWERCASE_LETTER:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  ਹਾਲਤ U_TITLECASE_LETTER:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  ਹਾਲਤ U_MODIFIER_LETTER:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  ਹਾਲਤ U_OTHER_LETTER:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  ਹਾਲਤ U_NON_SPACING_MARK:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  ਹਾਲਤ U_ENCLOSING_MARK:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  ਹਾਲਤ U_COMBINING_SPACING_MARK:	ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  ਹਾਲਤ U_DECIMAL_DIGIT_ਸੰਖਿਆ:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_DECIMAL_ਸੰਖਿਆ;
  ਹਾਲਤ U_LETTER_ਸੰਖਿਆ:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_LETTER_ਸੰਖਿਆ;
  ਹਾਲਤ U_OTHER_ਸੰਖਿਆ:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_OTHER_ਸੰਖਿਆ;

  ਹਾਲਤ U_SPACE_SEPARATOR:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  ਹਾਲਤ U_ਸਤਰ_SEPARATOR:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_ਸਤਰ_SEPARATOR;
  ਹਾਲਤ U_PARAGRAPH_SEPARATOR:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  ਹਾਲਤ U_CONTROL_CHAR:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  ਹਾਲਤ U_FORMAT_CHAR:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  ਹਾਲਤ U_ਨਿੱਜੀ_USE_CHAR:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_ਨਿੱਜੀ_USE;
  ਹਾਲਤ U_SURROGATE:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  ਹਾਲਤ U_DASH_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  ਹਾਲਤ U_ਸ਼ੁਰੂ_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  ਹਾਲਤ U_ਸਿਰਾ_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  ਹਾਲਤ U_CONNECTOR_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  ਹਾਲਤ U_OTHER_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  ਹਾਲਤ U_MATH_ਚਿੰਨ੍ਹ:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_MATH_ਚਿੰਨ੍ਹ;
  ਹਾਲਤ U_CURRENCY_ਚਿੰਨ੍ਹ:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_CURRENCY_ਚਿੰਨ੍ਹ;
  ਹਾਲਤ U_MODIFIER_ਚਿੰਨ੍ਹ:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_MODIFIER_ਚਿੰਨ੍ਹ;
  ਹਾਲਤ U_OTHER_ਚਿੰਨ੍ਹ:			ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_OTHER_ਚਿੰਨ੍ਹ;

  ਹਾਲਤ U_INITIAL_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  ਹਾਲਤ U_FINAL_PUNCTUATION:		ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  ਵਾਪਸ HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ਸਥਿਰ hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			  hb_codepoint_t      unicode,
			  ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ u_charMirror(unicode);
}

ਸਥਿਰ hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
		       hb_codepoint_t      unicode,
		       ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  UError_ਕੋਡ ਸਥਿਤੀ = U_ZERO_ERROR;
  UScript_ਕੋਡ script_ਕੋਡ = uscript_getScript(unicode, &ਸਥਿਤੀ);

  ਜੇ (unlikely (U_FAILURE (ਸਥਿਤੀ)))
    ਵਾਪਸ HB_SCRIPT_UNKNOWN;

  ਵਾਪਸ hb_icu_script_ਮੰਜ਼ਿਲ_script (script_ਕੋਡ);
}

ਸਥਿਰ hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			ਖਾਲੀ               *user_ਅੰਕੜੇ)
{
  ਅਚਲ UNormalizer2 *normalizer = (ਅਚਲ UNormalizer2 *) user_ਅੰਕੜੇ;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  ਜੇ (ret < 0) ਵਾਪਸ ਝੂਠ;
  *ab = ret;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  ਖਾਲੀ               *user_ਅੰਕੜੇ)
{
  ਅਚਲ UNormalizer2 *normalizer = (ਅਚਲ UNormalizer2 *) user_ਅੰਕੜੇ;
  UChar decomposed[4];
  ਪੂਰਨ len;
  UError_ਕੋਡ icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_ਲੰਮਾਈ (decomposed), &icu_err);
  ਜੇ (U_FAILURE (icu_err) || len < 0) ਵਾਪਸ ਝੂਠ;

  len = u_ਗਿਣਤੀ_Char32 (decomposed, len);
  ਜੇ (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    ਵਾਪਸ *a != ab;
  }
  ਨਹੀਂਤਾਂ ਜੇ (len == 2)
  {
    len = 0;
    U16_ਅਗਲਾ_UNSAFE (decomposed, len, *a);
    U16_ਅਗਲਾ_UNSAFE (decomposed, len, *b);
  }
  ਵਾਪਸ ਸੱਚ;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_icu_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_unicode_funcs_t *create ()
  {
    ਖਾਲੀ *user_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    UError_ਕੋਡ icu_err = U_ZERO_ERROR;
    user_ਅੰਕੜੇ = (ਖਾਲੀ *) unorm2_getNFCInstance (&icu_err);
    ਪ੍ਰਤਿਜ੍ਞਾ (user_ਅੰਕੜੇ);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_ਸੈੱਟ_combining_class_func (funcs, hb_icu_unicode_combining_class, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_general_category_func (funcs, hb_icu_unicode_general_category, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_mirroring_func (funcs, hb_icu_unicode_mirroring, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_script_func (funcs, hb_icu_unicode_script, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_compose_func (funcs, hb_icu_unicode_compose, user_ਅੰਕੜੇ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_decompose_func (funcs, hb_icu_unicode_decompose, user_ਅੰਕੜੇ, ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    ਵਾਪਸ funcs;
  }
} static_icu_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_icu_funcs ()
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
  ਵਾਪਸ static_icu_funcs.get_unconst ();
}

#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ

#ਜੇ_ਅੰਤ
