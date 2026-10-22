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

#اگر_تعریف_شدہ HAVE_ICU

#ہدایت GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#ہدایت GCC diagnostic ignored "-Wredundant-decls"

#شامل "hb-icu.h"

#شامل "hb-machinery.hh"

#شامل <unicode/uchar.h>
#شامل <unicode/unorm2.h>
#شامل <unicode/ustring.h>
#شامل <unicode/utf16.h>
#شامل <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#اگر U_ICU_نسخہ_MAJOR_عدد < 65 && (defined(__GNUC__) || defined(__clang__))
#تعریف_کرو HB_ICU_EXTRA_SEMI_IGNORED
#ہدایت GCC diagnostic ignored "-Wextra-semi-stmt"
#اگر_ختم

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
hb_icu_script_مقصد_script (UScript_رمز script)
{
  اگر (unlikely (script == USCRIPT_INVALID_رمز))
    واپس HB_SCRIPT_INVALID;

  واپس hb_script_from_string (uscript_getShortName (script), -1);
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
UScript_رمز
hb_icu_script_from_script (hb_script_t script)
{
  UScript_رمز برآمد = USCRIPT_INVALID_رمز;

  اگر (unlikely (script == HB_SCRIPT_INVALID))
    واپس برآمد;

  UError_رمز icu_err = U_ZERO_ERROR;
  ثابت غیرعلامتی حرف buf[5] = {HB_UNTAG (script), 0};
  uscript_get_رمز ((ثابت حرف *) buf, &برآمد, 1, &icu_err);

  واپس برآمد;
}


ساکن hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
				hb_codepoint_t      unicode,
				خالی               *user_ڈیٹا HB_غیر_مستعمل)

{
  واپس (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ساکن hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
				 hb_codepoint_t      unicode,
				 خالی               *user_ڈیٹا HB_غیر_مستعمل)
{
  انتخاب (u_get_Int_Property_قدر(unicode, UCHAR_GENERAL_CATEGORY))
  {
  حالت U_UNASSIGNED:			واپس HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  حالت U_UPPERCASE_LETTER:		واپس HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  حالت U_LOWERCASE_LETTER:		واپس HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  حالت U_TITLECASE_LETTER:		واپس HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  حالت U_MODIFIER_LETTER:		واپس HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  حالت U_OTHER_LETTER:			واپس HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  حالت U_NON_SPACING_MARK:		واپس HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  حالت U_ENCLOSING_MARK:		واپس HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  حالت U_COMBINING_SPACING_MARK:	واپس HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  حالت U_DECIMAL_DIGIT_نمبر:		واپس HB_UNICODE_GENERAL_CATEGORY_DECIMAL_نمبر;
  حالت U_LETTER_نمبر:			واپس HB_UNICODE_GENERAL_CATEGORY_LETTER_نمبر;
  حالت U_OTHER_نمبر:			واپس HB_UNICODE_GENERAL_CATEGORY_OTHER_نمبر;

  حالت U_SPACE_SEPARATOR:		واپس HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  حالت U_سطر_SEPARATOR:		واپس HB_UNICODE_GENERAL_CATEGORY_سطر_SEPARATOR;
  حالت U_PARAGRAPH_SEPARATOR:		واپس HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  حالت U_CONTROL_CHAR:			واپس HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  حالت U_FORMAT_CHAR:			واپس HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  حالت U_نجی_USE_CHAR:		واپس HB_UNICODE_GENERAL_CATEGORY_نجی_USE;
  حالت U_SURROGATE:			واپس HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  حالت U_DASH_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  حالت U_آغاز_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  حالت U_انتہا_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  حالت U_CONNECTOR_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  حالت U_OTHER_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  حالت U_MATH_علامت:			واپس HB_UNICODE_GENERAL_CATEGORY_MATH_علامت;
  حالت U_CURRENCY_علامت:		واپس HB_UNICODE_GENERAL_CATEGORY_CURRENCY_علامت;
  حالت U_MODIFIER_علامت:		واپس HB_UNICODE_GENERAL_CATEGORY_MODIFIER_علامت;
  حالت U_OTHER_علامت:			واپس HB_UNICODE_GENERAL_CATEGORY_OTHER_علامت;

  حالت U_INITIAL_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  حالت U_FINAL_PUNCTUATION:		واپس HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  واپس HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ساکن hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
			  hb_codepoint_t      unicode,
			  خالی               *user_ڈیٹا HB_غیر_مستعمل)
{
  واپس u_charMirror(unicode);
}

ساکن hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
		       hb_codepoint_t      unicode,
		       خالی               *user_ڈیٹا HB_غیر_مستعمل)
{
  UError_رمز حالت_2 = U_ZERO_ERROR;
  UScript_رمز script_رمز = uscript_getScript(unicode, &حالت_2);

  اگر (unlikely (U_FAILURE (حالت_2)))
    واپس HB_SCRIPT_UNKNOWN;

  واپس hb_icu_script_مقصد_script (script_رمز);
}

ساکن hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			خالی               *user_ڈیٹا)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_ڈیٹا;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  اگر (ret < 0) واپس جھوٹ;
  *ab = ret;
  واپس سچ;
}

ساکن hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_غیر_مستعمل,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  خالی               *user_ڈیٹا)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_ڈیٹا;
  UChar decomposed[4];
  صحیح len;
  UError_رمز icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_لمبائی (decomposed), &icu_err);
  اگر (U_FAILURE (icu_err) || len < 0) واپس جھوٹ;

  len = u_گنتی_Char32 (decomposed, len);
  اگر (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    واپس *a != ab;
  }
  ورنہ اگر (len == 2)
  {
    len = 0;
    U16_اگلا_UNSAFE (decomposed, len, *a);
    U16_اگلا_UNSAFE (decomposed, len, *b);
  }
  واپس سچ;
}


ساکن اندرونی خالی free_static_icu_funcs ();

ساکن ساخت hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ساکن hb_unicode_funcs_t *create ()
  {
    خالی *user_ڈیٹا = خالی_اشاریہ;
    UError_رمز icu_err = U_ZERO_ERROR;
    user_ڈیٹا = (خالی *) unorm2_getNFCInstance (&icu_err);
    أكد (user_ڈیٹا);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (خالی_اشاریہ);

    hb_unicode_funcs_سیٹ_combining_class_func (funcs, hb_icu_unicode_combining_class, خالی_اشاریہ, خالی_اشاریہ);
    hb_unicode_funcs_سیٹ_general_category_func (funcs, hb_icu_unicode_general_category, خالی_اشاریہ, خالی_اشاریہ);
    hb_unicode_funcs_سیٹ_mirroring_func (funcs, hb_icu_unicode_mirroring, خالی_اشاریہ, خالی_اشاریہ);
    hb_unicode_funcs_سیٹ_script_func (funcs, hb_icu_unicode_script, خالی_اشاریہ, خالی_اشاریہ);
    hb_unicode_funcs_سیٹ_compose_func (funcs, hb_icu_unicode_compose, user_ڈیٹا, خالی_اشاریہ);
    hb_unicode_funcs_سیٹ_decompose_func (funcs, hb_icu_unicode_decompose, user_ڈیٹا, خالی_اشاریہ);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    واپس funcs;
  }
} static_icu_funcs;

ساکن اندرونی
خالی free_static_icu_funcs ()
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
  واپس static_icu_funcs.get_unconst ();
}

#ہدایت GCC diagnostic نکالو

#اگر_ختم
