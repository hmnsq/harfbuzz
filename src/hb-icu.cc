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

#اگر_تعریف_شده HAVE_ICU

#رهنمود GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#رهنمود GCC diagnostic ignored "-Wredundant-decls"

#ضمن "hb-icu.h"

#ضمن "hb-machinery.hh"

#ضمن <unicode/uchar.h>
#ضمن <unicode/unorm2.h>
#ضمن <unicode/ustring.h>
#ضمن <unicode/utf16.h>
#ضمن <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#إذا U_ICU_نسخه_MAJOR_رقم < 65 && (defined(__GNUC__) || defined(__clang__))
#تعریف_کن HB_ICU_EXTRA_SEMI_IGNORED
#رهنمود GCC diagnostic ignored "-Wextra-semi-stmt"
#پایان_اگر

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
hb_icu_script_مقصد_script (UScript_کد script)
{
  إذا (unlikely (script == USCRIPT_INVALID_کد))
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
UScript_کد
hb_icu_script_from_script (hb_script_t script)
{
  UScript_کد بیرون = USCRIPT_INVALID_کد;

  إذا (unlikely (script == HB_SCRIPT_INVALID))
    أرجع بیرون;

  UError_کد icu_err = U_ZERO_ERROR;
  ثابت بیعلامت حرف buf[5] = {HB_UNTAG (script), 0};
  uscript_get_کد ((ثابت حرف *) buf, &بیرون, 1, &icu_err);

  أرجع بیرون;
}


ساكن hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
				hb_codepoint_t      unicode,
				فراغ               *user_داده HB_استفادهنشده)

{
  أرجع (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

ساكن hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
				 hb_codepoint_t      unicode,
				 فراغ               *user_داده HB_استفادهنشده)
{
  بدل (u_get_Int_Property_مقدار(unicode, UCHAR_GENERAL_CATEGORY))
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

  حالة U_DECIMAL_DIGIT_عدد:		أرجع HB_UNICODE_GENERAL_CATEGORY_DECIMAL_عدد;
  حالة U_LETTER_عدد:			أرجع HB_UNICODE_GENERAL_CATEGORY_LETTER_عدد;
  حالة U_OTHER_عدد:			أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_عدد;

  حالة U_SPACE_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  حالة U_سطر_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_سطر_SEPARATOR;
  حالة U_PARAGRAPH_SEPARATOR:		أرجع HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  حالة U_CONTROL_CHAR:			أرجع HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  حالة U_FORMAT_CHAR:			أرجع HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  حالة U_خصوصی_USE_CHAR:		أرجع HB_UNICODE_GENERAL_CATEGORY_خصوصی_USE;
  حالة U_SURROGATE:			أرجع HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  حالة U_DASH_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  حالة U_شروع_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  حالة U_انتها_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  حالة U_CONNECTOR_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  حالة U_OTHER_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  حالة U_MATH_نماد:			أرجع HB_UNICODE_GENERAL_CATEGORY_MATH_نماد;
  حالة U_CURRENCY_نماد:		أرجع HB_UNICODE_GENERAL_CATEGORY_CURRENCY_نماد;
  حالة U_MODIFIER_نماد:		أرجع HB_UNICODE_GENERAL_CATEGORY_MODIFIER_نماد;
  حالة U_OTHER_نماد:			أرجع HB_UNICODE_GENERAL_CATEGORY_OTHER_نماد;

  حالة U_INITIAL_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  حالة U_FINAL_PUNCTUATION:		أرجع HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  أرجع HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

ساكن hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
			  hb_codepoint_t      unicode,
			  فراغ               *user_داده HB_استفادهنشده)
{
  أرجع u_charMirror(unicode);
}

ساكن hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
		       hb_codepoint_t      unicode,
		       فراغ               *user_داده HB_استفادهنشده)
{
  UError_کد وضعیت = U_ZERO_ERROR;
  UScript_کد script_کد = uscript_getScript(unicode, &وضعیت);

  إذا (unlikely (U_FAILURE (وضعیت)))
    أرجع HB_SCRIPT_UNKNOWN;

  أرجع hb_icu_script_مقصد_script (script_کد);
}

ساكن hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			فراغ               *user_داده)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_داده;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  إذا (ret < 0) أرجع خطأ;
  *ab = ret;
  أرجع درست;
}

ساكن hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_استفادهنشده,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  فراغ               *user_داده)
{
  ثابت UNormalizer2 *normalizer = (ثابت UNormalizer2 *) user_داده;
  UChar decomposed[4];
  صحيح len;
  UError_کد icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_درازی (decomposed), &icu_err);
  إذا (U_FAILURE (icu_err) || len < 0) أرجع خطأ;

  len = u_شمار_Char32 (decomposed, len);
  إذا (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    أرجع *a != ab;
  }
  وإلا إذا (len == 2)
  {
    len = 0;
    U16_بعدی_UNSAFE (decomposed, len, *a);
    U16_بعدی_UNSAFE (decomposed, len, *b);
  }
  أرجع درست;
}


ساكن مضمن فراغ free_static_icu_funcs ();

ساكن بنية hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  ساكن hb_unicode_funcs_t *create ()
  {
    فراغ *user_داده = اشاره_تهی;
    UError_کد icu_err = U_ZERO_ERROR;
    user_داده = (فراغ *) unorm2_getNFCInstance (&icu_err);
    أكد (user_داده);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (اشاره_تهی);

    hb_unicode_funcs_مجموعه_combining_class_func (funcs, hb_icu_unicode_combining_class, اشاره_تهی, اشاره_تهی);
    hb_unicode_funcs_مجموعه_general_category_func (funcs, hb_icu_unicode_general_category, اشاره_تهی, اشاره_تهی);
    hb_unicode_funcs_مجموعه_mirroring_func (funcs, hb_icu_unicode_mirroring, اشاره_تهی, اشاره_تهی);
    hb_unicode_funcs_مجموعه_script_func (funcs, hb_icu_unicode_script, اشاره_تهی, اشاره_تهی);
    hb_unicode_funcs_مجموعه_compose_func (funcs, hb_icu_unicode_compose, user_داده, اشاره_تهی);
    hb_unicode_funcs_مجموعه_decompose_func (funcs, hb_icu_unicode_decompose, user_داده, اشاره_تهی);

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

#رهنمود GCC diagnostic برداشت

#پایان_اگر
