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

#ضمن "hb.hh"

#ifdef HAVE_ICU

#pragma GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#pragma GCC diagnostic ignored "-Wredundant-decls"

#ضمن "hb-icu.h"

#ضمن "hb-machinery.hh"

#ضمن <unicode/uchar.h>
#ضمن <unicode/unorm2.h>
#ضمن <unicode/ustring.h>
#ضمن <unicode/utf16.h>
#ضمن <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#إذا U_ICU_VERSION_MAJOR_لمب < 65 && (defined(__GNUC__) || defined(__clang__))
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
  إذا (unlikely (script == USCRIPT_INVALID_CODE))
    أرجع HB_SCRIPT_INVALID;

  أرجع hb_script_from_string (uscript_getShortName (script), -1);
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

  إذا (unlikely (script == HB_SCRIPT_INVALID))
    أرجع out;

  UErrorCode icu_err = U_ZERO_ERROR;
  ثابت unsigned حرف buf[5] = {HB_UNTAG (script), 0};
  uscript_getCode ((ثابت حرف *) buf, &out, 1, &icu_err);

  أرجع out;
}


ساكن hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				hb_codepoint_t      unicode,
				فراغ               *user_data HB_UNUSED)

{
  أرجع (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ساكن hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 فراغ               *user_data HB_UNUSED)
{
  بدل (u_get_Int_Property_درج(unicode, UCHAR_GENERAL_CATEGORY))
  {
  حالة U_UNASSIGNED:			أرجع HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  حالة U_UPPERCASE_LETTER:		أرجع HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  حالة U_LOWERCASE_LETTER:		أرجع HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  حالة U_TITLECASE_LETTER:		أرجع HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  حالة U_MODIFIER_LETTER:		أرجع HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  حالة U_OTHER_LETTER:			أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  حالة U_NON_SPACING_MARK:		أرجع HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  حالة U_ENCLOSING_MARK:		أرجع HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  حالة U_COMBINING_SPACING_MARK:	أرجع HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  حالة U_DECIMAL_DIGIT_لمب:		أرجع HB_UNICODE_GENERAL_CATEGORY_DECIMAL_لمب;
  حالة U_LETTER_لمب:			أرجع HB_UNICODE_GENERAL_CATEGORY_LETTER_لمب;
  حالة U_OTHER_لمب:			أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_لمب;

  حالة U_SPACE_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  حالة U_لي_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_لي_SEPARATOR;
  حالة U_PARAGRAPH_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  حالة U_CONTROL_CHAR:			أرجع HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  حالة U_FORMAT_CHAR:			أرجع HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  حالة U_PRIVATE_USE_CHAR:		أرجع HB_UNICODE_GENERAL_CATEGORY_PRIVATE_USE;
  حالة U_SURROGATE:			أرجع HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  حالة U_DASH_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  حالة U_START_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  حالة U_END_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  حالة U_CONNECTOR_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  حالة U_OTHER_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  حالة U_MATH_علام:			أرجع HB_UNICODE_GENERAL_CATEGORY_MATH_علام;
  حالة U_CURRENCY_علام:		أرجع HB_UNICODE_GENERAL_CATEGORY_CURRENCY_علام;
  حالة U_MODIFIER_علام:		أرجع HB_UNICODE_GENERAL_CATEGORY_MODIFIER_علام;
  حالة U_OTHER_علام:			أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_علام;

  حالة U_INITIAL_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  حالة U_FINAL_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  أرجع HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ساكن hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      unicode,
			  فراغ               *user_data HB_UNUSED)
{
  أرجع u_charMirror(unicode);
}

ساكن hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
		       hb_codepoint_t      unicode,
		       فراغ               *user_data HB_UNUSED)
{
  UErrorCode status = U_ZERO_ERROR;
  UScriptCode scriptCode = uscript_getScript(unicode, &status);

  إذا (unlikely (U_FAILURE (status)))
    أرجع HB_SCRIPT_UNKNOWN;

  أرجع hb_icu_script_to_script (scriptCode);
}

ساكن hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			فراغ               *user_data)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_data;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  إذا (ret < 0) أرجع خطأ;
  *ab = ret;
  أرجع true;
}

ساكن hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  فراغ               *user_data)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_data;
  UChar decomposed[4];
  صحيح len;
  UErrorCode icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_LENGTH (decomposed), &icu_err);
  إذا (U_FAILURE (icu_err) || len < 0) أرجع خطأ;

  len = u_كدي_Char32 (decomposed, len);
  إذا (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    أرجع *a != ab;
  }
  وإلا إذا (len == 2)
  {
    len = 0;
    U16_NEXT_UNSAFE (decomposed, len, *a);
    U16_NEXT_UNSAFE (decomposed, len, *b);
  }
  أرجع true;
}


ساكن مضمن فراغ free_static_icu_funcs ();

ساكن بنية hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ساكن hb_unicode_funcs_t *create ()
  {
    فراغ *user_data = nullptr;
    UErrorCode icu_err = U_ZERO_ERROR;
    user_data = (فراغ *) unorm2_getNFCInstance (&icu_err);
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

    أرجع funcs;
  }
} static_icu_funcs;

ساكن مضمن
فراغ free_static_icu_funcs ()
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
  أرجع static_icu_funcs.get_unconst ();
}

#pragma GCC diagnostic چرو

#endif
