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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_ICU

#సూచన GCC diagnostic push

// https://github.com/harfbuzz/harfbuzz/issues/4915
#సూచన GCC diagnostic ignored "-Wredundant-decls"

#అన్తర్భావ "hb-icu.h"

#అన్తర్భావ "hb-machinery.hh"

#అన్తర్భావ <unicode/uchar.h>
#అన్తర్భావ <unicode/unorm2.h>
#అన్తర్భావ <unicode/ustring.h>
#అన్తర్భావ <unicode/utf16.h>
#అన్తర్భావ <unicode/uversion.h>

/* ICU extra semicolon, fixed since 65, https://github.com/unicode-org/icu/commit/480bec3 */
#ఒకవేళ U_ICU_సంస్కరణ_MAJOR_అంకె < 65 && (defined(__GNUC__) || defined(__clang__))
#నిర్వచించు HB_ICU_EXTRA_SEMI_IGNORED
#సూచన GCC diagnostic ignored "-Wextra-semi-stmt"
#అయితే_ముగింపు

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
hb_icu_script_గమ్యం_script (UScript_సంకేతం script)
{
  ఒకవేళ (unlikely (script == USCRIPT_INVALID_సంకేతం))
    తిరిగి HB_SCRIPT_INVALID;

  తిరిగి hb_script_from_string (uscript_getShortName (script), -1);
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
UScript_సంకేతం
hb_icu_script_from_script (hb_script_t script)
{
  UScript_సంకేతం బహిర్గతం = USCRIPT_INVALID_సంకేతం;

  ఒకవేళ (unlikely (script == HB_SCRIPT_INVALID))
    తిరిగి బహిర్గతం;

  UError_సంకేతం icu_err = U_ZERO_ERROR;
  స్థిరరాశి గుర్తులేని అక్షరం buf[5] = {HB_UNTAG (script), 0};
  uscript_get_సంకేతం ((స్థిరరాశి అక్షరం *) buf, &బహిర్గతం, 1, &icu_err);

  తిరిగి బహిర్గతం;
}


స్థిర hb_unicode_combining_class_t
hb_icu_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_వాడని,
				hb_codepoint_t      unicode,
				శూన్య               *user_దత్తాంశం HB_వాడని)

{
  తిరిగి (hb_unicode_combining_class_t) u_getCombiningClass (unicode);
}

స్థిర hb_unicode_general_category_t
hb_icu_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_వాడని,
				 hb_codepoint_t      unicode,
				 శూన్య               *user_దత్తాంశం HB_వాడని)
{
  ఎంపిక (u_get_Int_Property_విలువ(unicode, UCHAR_GENERAL_CATEGORY))
  {
  స్థితి U_UNASSIGNED:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;

  స్థితి U_UPPERCASE_LETTER:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER;
  స్థితి U_LOWERCASE_LETTER:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER;
  స్థితి U_TITLECASE_LETTER:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER;
  స్థితి U_MODIFIER_LETTER:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER;
  స్థితి U_OTHER_LETTER:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER;

  స్థితి U_NON_SPACING_MARK:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK;
  స్థితి U_ENCLOSING_MARK:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK;
  స్థితి U_COMBINING_SPACING_MARK:	తిరిగి HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK;

  స్థితి U_DECIMAL_DIGIT_సంఖ్య:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_DECIMAL_సంఖ్య;
  స్థితి U_LETTER_సంఖ్య:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_LETTER_సంఖ్య;
  స్థితి U_OTHER_సంఖ్య:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_OTHER_సంఖ్య;

  స్థితి U_SPACE_SEPARATOR:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_SPACE_SEPARATOR;
  స్థితి U_పంక్తి_SEPARATOR:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_పంక్తి_SEPARATOR;
  స్థితి U_PARAGRAPH_SEPARATOR:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_PARAGRAPH_SEPARATOR;

  స్థితి U_CONTROL_CHAR:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_CONTROL;
  స్థితి U_FORMAT_CHAR:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_FORMAT;
  స్థితి U_వ్యక్తిగత_USE_CHAR:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_వ్యక్తిగత_USE;
  స్థితి U_SURROGATE:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_SURROGATE;


  స్థితి U_DASH_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_DASH_PUNCTUATION;
  స్థితి U_ప్రారంభం_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_OPEN_PUNCTUATION;
  స్థితి U_చివర_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_CLOSE_PUNCTUATION;
  స్థితి U_CONNECTOR_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_CONNECT_PUNCTUATION;
  స్థితి U_OTHER_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_OTHER_PUNCTUATION;

  స్థితి U_MATH_చిహ్నం:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_MATH_చిహ్నం;
  స్థితి U_CURRENCY_చిహ్నం:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_CURRENCY_చిహ్నం;
  స్థితి U_MODIFIER_చిహ్నం:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_MODIFIER_చిహ్నం;
  స్థితి U_OTHER_చిహ్నం:			తిరిగి HB_UNICODE_GENERAL_CATEGORY_OTHER_చిహ్నం;

  స్థితి U_INITIAL_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_INITIAL_PUNCTUATION;
  స్థితి U_FINAL_PUNCTUATION:		తిరిగి HB_UNICODE_GENERAL_CATEGORY_FINAL_PUNCTUATION;
  }

  తిరిగి HB_UNICODE_GENERAL_CATEGORY_UNASSIGNED;
}

స్థిర hb_codepoint_t
hb_icu_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_వాడని,
			  hb_codepoint_t      unicode,
			  శూన్య               *user_దత్తాంశం HB_వాడని)
{
  తిరిగి u_charMirror(unicode);
}

స్థిర hb_script_t
hb_icu_unicode_script (hb_unicode_funcs_t *ufuncs HB_వాడని,
		       hb_codepoint_t      unicode,
		       శూన్య               *user_దత్తాంశం HB_వాడని)
{
  UError_సంకేతం స్థితి_2 = U_ZERO_ERROR;
  UScript_సంకేతం script_సంకేతం = uscript_getScript(unicode, &స్థితి_2);

  ఒకవేళ (unlikely (U_FAILURE (స్థితి_2)))
    తిరిగి HB_SCRIPT_UNKNOWN;

  తిరిగి hb_icu_script_గమ్యం_script (script_సంకేతం);
}

స్థిర hb_bool_t
hb_icu_unicode_compose (hb_unicode_funcs_t *ufuncs HB_వాడని,
			hb_codepoint_t      a,
			hb_codepoint_t      b,
			hb_codepoint_t     *ab,
			శూన్య               *user_దత్తాంశం)
{
  స్థిరరాశి UNormalizer2 *normalizer = (స్థిరరాశి UNormalizer2 *) user_దత్తాంశం;
  UChar32 ret = unorm2_composePair (normalizer, a, b);
  ఒకవేళ (ret < 0) తిరిగి అబద్ధం;
  *ab = ret;
  తిరిగి నిజం;
}

స్థిర hb_bool_t
hb_icu_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_వాడని,
			  hb_codepoint_t      ab,
			  hb_codepoint_t     *a,
			  hb_codepoint_t     *b,
			  శూన్య               *user_దత్తాంశం)
{
  స్థిరరాశి UNormalizer2 *normalizer = (స్థిరరాశి UNormalizer2 *) user_దత్తాంశం;
  UChar decomposed[4];
  పూర్ణాంకం len;
  UError_సంకేతం icu_err = U_ZERO_ERROR;
  len = unorm2_getRawDecomposition (normalizer, ab, decomposed,
				    ARRAY_నిడివి (decomposed), &icu_err);
  ఒకవేళ (U_FAILURE (icu_err) || len < 0) తిరిగి అబద్ధం;

  len = u_లెక్క_Char32 (decomposed, len);
  ఒకవేళ (len == 1)
  {
    U16_GET_UNSAFE (decomposed, 0, *a);
    *b = 0;
    తిరిగి *a != ab;
  }
  లేకపోతే ఒకవేళ (len == 2)
  {
    len = 0;
    U16_తదుపరి_UNSAFE (decomposed, len, *a);
    U16_తదుపరి_UNSAFE (decomposed, len, *b);
  }
  తిరిగి నిజం;
}


స్థిర అంతఃవరుస శూన్య free_static_icu_funcs ();

స్థిర నిర్మాణం hb_icu_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_icu_unicode_funcs_lazy_loader_t>
{
  స్థిర hb_unicode_funcs_t *create ()
  {
    శూన్య *user_దత్తాంశం = శూన్యసూచిక;
    UError_సంకేతం icu_err = U_ZERO_ERROR;
    user_దత్తాంశం = (శూన్య *) unorm2_getNFCInstance (&icu_err);
    ప్రతిజ్ఞా (user_దత్తాంశం);

    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (శూన్యసూచిక);

    hb_unicode_funcs_సమితి_combining_class_func (funcs, hb_icu_unicode_combining_class, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_general_category_func (funcs, hb_icu_unicode_general_category, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_mirroring_func (funcs, hb_icu_unicode_mirroring, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_script_func (funcs, hb_icu_unicode_script, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_compose_func (funcs, hb_icu_unicode_compose, user_దత్తాంశం, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_decompose_func (funcs, hb_icu_unicode_decompose, user_దత్తాంశం, శూన్యసూచిక);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_icu_funcs);

    తిరిగి funcs;
  }
} static_icu_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_icu_funcs ()
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
  తిరిగి static_icu_funcs.get_unconst ();
}

#సూచన GCC diagnostic తీయి

#అయితే_ముగింపు
