/*
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
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
#شامل "hb-machinery.hh"


/**
 * SECTION:hb-common
 * @title: hb-common
 * @short_description: Common data types
 * @include: hb.h
 *
 * Common data types used across HarfBuzz are defined here.
 **/


/* hb_tag_t */

/**
 * hb_tag_from_string:
 * @str: (array length=len) (element-type uint8_t): String to convert
 * @len: Length of @str, or -1 if it is `NULL`-terminated
 *
 * Converts a string into an #hb_tag_t. Valid tags
 * are four characters. Shorter input strings will be
 * padded with spaces. Longer input strings will be
 * truncated.
 *
 * Return value: The #hb_tag_t corresponding to @str
 *
 * Since: 0.9.2
 **/
hb_لیبل_t
hb_لیبل_from_string (ثابت حرف *str, صحیح len)
{
  حرف لیبل_2[4];
  غیرعلامتی صحیح i;

  اگر (!str || !len || !*str)
    واپس HB_لیبل_NONE;

  اگر (len < 0 || len > 4)
    len = 4;
  برائے (i = 0; i < (غیرعلامتی) len && str[i]; i++)
    لیبل_2[i] = str[i];
  برائے (; i < 4; i++)
    لیبل_2[i] = ' ';

  واپس HB_لیبل (لیبل_2[0], لیبل_2[1], لیبل_2[2], لیبل_2[3]);
}

/**
 * hb_tag_to_string:
 * @tag: #hb_tag_t to convert
 * @buf: (out caller-allocates) (array fixed-size=4) (element-type uint8_t): Converted string
 *
 * Converts an #hb_tag_t to a string and returns it in @buf.
 * Strings will be four characters long.
 *
 * Since: 0.9.5
 **/
خالی
hb_لیبل_مقصد_string (hb_لیبل_t لیبل_2, حرف *buf)
{
  buf[0] = (حرف) (uint8_t) (لیبل_2 >> 24);
  buf[1] = (حرف) (uint8_t) (لیبل_2 >> 16);
  buf[2] = (حرف) (uint8_t) (لیبل_2 >>  8);
  buf[3] = (حرف) (uint8_t) (لیبل_2 >>  0);
}


/* hb_direction_t */

ساکن ثابت حرف direction_لڑیاں[][4] = {
  "ltr",
  "rtl",
  "ttb",
  "btt"
};

/**
 * hb_direction_from_string:
 * @str: (array length=len) (element-type uint8_t): String to convert
 * @len: Length of @str, or -1 if it is `NULL`-terminated
 *
 * Converts a string to an #hb_direction_t.
 *
 * Matching is loose and applies only to the first letter. For
 * examples, "LTR" and "left-to-right" will both return #HB_DIRECTION_LTR.
 *
 * Unmatched strings will return #HB_DIRECTION_INVALID.
 *
 * Return value: The #hb_direction_t matching @str
 *
 * Since: 0.9.2
 **/
hb_direction_t
hb_direction_from_string (ثابت حرف *str, صحیح len)
{
  اگر (unlikely (!str || !len || !*str))
    واپس HB_DIRECTION_INVALID;

  /* Lets match loosely: just match the first letter, such that
   * all of "ltr", "left-to-right", etc work!
   */
  حرف c = TOLOWER (str[0]);
  برائے (غیرعلامتی صحیح i = 0; i < ARRAY_لمبائی (direction_لڑیاں); i++)
    اگر (c == direction_لڑیاں[i][0])
      واپس (hb_direction_t) (HB_DIRECTION_LTR + i);

  واپس HB_DIRECTION_INVALID;
}

/**
 * hb_direction_to_string:
 * @direction: The #hb_direction_t to convert
 *
 * Converts an #hb_direction_t to a string.
 *
 * Return value: (transfer none): The string corresponding to @direction
 *
 * Since: 0.9.2
 **/
ثابت حرف *
hb_direction_مقصد_string (hb_direction_t direction)
{
  اگر (likely ((غیرعلامتی صحیح) (direction - HB_DIRECTION_LTR)
	      < ARRAY_لمبائی (direction_لڑیاں)))
    واپس direction_لڑیاں[direction - HB_DIRECTION_LTR];

  واپس "invalid";
}


/* hb_language_t */

ساخت hb_language_impl_t {
  ثابت حرف s[1];
};

ساکن ثابت حرف canon_map[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  '-',  0,   0,
  '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9',  0,   0,   0,   0,   0,   0,
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,  '-',
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,   0
};

ساکن منطقی
lang_equal (hb_language_t  v1,
	    ثابت خالی    *v2)
{
  ثابت غیرعلامتی حرف *p1 = (ثابت غیرعلامتی حرف *) v1;
  ثابت غیرعلامتی حرف *p2 = (ثابت غیرعلامتی حرف *) v2;

  جب (*p1 && *p1 == canon_map[*p2]) {
    p1++;
    p2++;
  }

  واپس *p1 == canon_map[*p2];
}

#اگر 0
ساکن غیرعلامتی صحیح
lang_hash (ثابت خالی *key)
{
  ثابت غیرعلامتی حرف *p = key;
  غیرعلامتی صحیح h = 0;
  جب (canon_map[*p])
    {
      h = (h << 5) - h + canon_map[*p];
      p++;
    }

  واپس h;
}
#اگر_ختم


ساخت hb_language_شے_t {

  ساخت hb_language_شے_t *next;
  hb_language_t lang;

  منطقی عامل == (ثابت حرف *s) ثابت
  { واپس lang_equal (lang, s); }

  hb_language_شے_t & عامل = (ثابت حرف *s)
  {
    /* We can't call strdup(), because we allow custom allocators. */
    size_t len = لڑی_لمبائی(s) + 1;
    lang = (hb_language_t) hb_malloc(len);
    اگر (likely (lang))
    {
      hb_memcpy((غیرعلامتی حرف *) lang, s, len);
      برائے (غیرعلامتی حرف *p = (غیرعلامتی حرف *) lang; *p; p++)
	*p = canon_map[*p];
    }

    واپس *یہ;
  }

  خالی fini () { hb_free ((خالی *) lang); }
};


/* Thread-safe lockfree language list */

ساکن hb_atomic_t<hb_language_شے_t *> langs;

ساکن اندرونی خالی
free_langs ()
{
retry:
  hb_language_شے_t *پہلا_lang = langs;
  اگر (unlikely (!langs.cmpexch (پہلا_lang, خالی_اشاریہ)))
    جاؤ retry;

  جب (پہلا_lang) {
    hb_language_شے_t *next = پہلا_lang->next;
    پہلا_lang->fini ();
    hb_free (پہلا_lang);
    پہلا_lang = next;
  }
}

ساکن hb_language_شے_t *
lang_ڈھونڈو_or_داخل_کرو (ثابت حرف *key)
{
retry:
  hb_language_شے_t *پہلا_lang = langs;

  برائے (hb_language_شے_t *lang = پہلا_lang; lang; lang = lang->next)
    اگر (*lang == key)
      واپس lang;

  /* Not found; allocate one. */
  hb_language_شے_t *lang = (hb_language_شے_t *) hb_calloc (1, حجم (hb_language_شے_t));
  اگر (unlikely (!lang))
    واپس خالی_اشاریہ;
  lang->next = پہلا_lang;
  *lang = key;
  اگر (unlikely (!lang->lang))
  {
    hb_free (lang);
    واپس خالی_اشاریہ;
  }

  اگر (unlikely (!langs.cmpexch (پہلا_lang, lang)))
  {
    lang->fini ();
    hb_free (lang);
    جاؤ retry;
  }

  اگر (!پہلا_lang)
    hb_atexit (free_langs); /* First person registers atexit() callback. */

  واپس lang;
}


/**
 * hb_language_from_string:
 * @str: (array length=len) (element-type uint8_t): a string representing
 *       a BCP 47 language tag
 * @len: length of the @str, or -1 if it is `NULL`-terminated.
 *
 * Converts @str representing a BCP 47 language tag to the corresponding
 * #hb_language_t.
 *
 * Return value: (transfer none):
 * The #hb_language_t corresponding to the BCP 47 language tag.
 *
 * Since: 0.9.2
 **/
hb_language_t
hb_language_from_string (ثابت حرف *str, صحیح len)
{
  اگر (!str || !len || !*str)
    واپس HB_LANGUAGE_INVALID;

  hb_language_شے_t *شے = خالی_اشاریہ;
  اگر (len >= 0)
  {
    /* NUL-terminate it. */
    حرف strbuf[64];
    len = hb_min (len, (صحیح) حجم (strbuf) - 1);
    hb_memcpy (strbuf, str, len);
    strbuf[len] = '\0';
    شے = lang_ڈھونڈو_or_داخل_کرو (strbuf);
  }
  ورنہ
    شے = lang_ڈھونڈو_or_داخل_کرو (str);

  واپس likely (شے) ? شے->lang : HB_LANGUAGE_INVALID;
}

/**
 * hb_language_to_string:
 * @language: The #hb_language_t to convert
 *
 * Converts an #hb_language_t to a string.
 *
 * Return value: (transfer none):
 * A `NULL`-terminated string representing the @language. Must not be freed by
 * the caller.
 *
 * Since: 0.9.2
 **/
ثابت حرف *
hb_language_مقصد_string (hb_language_t language)
{
  اگر (unlikely (!language)) واپس خالی_اشاریہ;

  واپس language->s;
}

/**
 * hb_language_get_default:
 *
 * Fetch the default language from current locale.
 *
 * <note>Note that the first time this function is called, it calls
 * "setlocale (LC_CTYPE, nullptr)" to fetch current locale.  The underlying
 * setlocale function is, in many implementations, NOT threadsafe.  To avoid
 * problems, call this function once before multiple threads can call it.
 * This function is only used from hb_buffer_guess_segment_properties() by
 * HarfBuzz itself.</note>
 *
 * Return value: (transfer none): The default language of the locale as
 * an #hb_language_t
 *
 * Since: 0.9.2
 **/
hb_language_t
hb_language_get_default ()
{
  ساکن hb_atomic_t<hb_language_t> default_language;

  hb_language_t language = default_language;
  اگر (unlikely (language == HB_LANGUAGE_INVALID))
  {
    language = hb_language_from_string (hb_setlocale (LC_CTYPE, خالی_اشاریہ), -1);
    (خالی) default_language.cmpexch (HB_LANGUAGE_INVALID, language);
  }

  واپس language;
}

/**
 * hb_language_matches:
 * @language: The #hb_language_t to work on
 * @specific: Another #hb_language_t
 *
 * Check whether a second language tag is the same or a more
 * specific version of the provided language tag.  For example,
 * "fa_IR.utf8" is a more specific tag for "fa" or for "fa_IR".
 *
 * Return value: `true` if languages match, `false` otherwise.
 *
 * Since: 5.0.0
 **/
hb_bool_t
hb_language_matches (hb_language_t language,
		     hb_language_t specific)
{
  اگر (language == specific) واپس سچ;
  اگر (!language || !specific) واپس جھوٹ;

  ثابت حرف *l = language->s;
  ثابت حرف *s = specific->s;
  غیرعلامتی ll = لڑی_لمبائی (l);
  غیرعلامتی sl = لڑی_لمبائی (s);

  اگر (ll > sl)
    واپس جھوٹ;

  واپس لڑی_موازنہ (l, s, ll) == 0 &&
	 (s[ll] == '\0' || s[ll] == '-');
}


/* hb_script_t */

/**
 * hb_script_from_iso15924_tag:
 * @tag: an #hb_tag_t representing an ISO 15924 tag.
 *
 * Converts an ISO 15924 script tag to a corresponding #hb_script_t.
 *
 * Return value:
 * An #hb_script_t corresponding to the ISO 15924 tag.
 *
 * Since: 0.9.2
 **/
hb_script_t
hb_script_from_iso15924_لیبل (hb_لیبل_t لیبل_2)
{
  اگر (unlikely (لیبل_2 == HB_لیبل_NONE))
    واپس HB_SCRIPT_INVALID;

  /* Be lenient, adjust case (one capital letter followed by three small letters) */
  لیبل_2 = (لیبل_2 & 0xDFDFDFDFu) | 0x00202020u;

  انتخاب (لیبل_2)
  {
    /* Script variants from https://unicode.org/iso15924/ */
    حالت HB_لیبل('A','r','a','n'): واپس HB_SCRIPT_ARABIC;
    حالت HB_لیبل('C','y','r','s'): واپس HB_SCRIPT_CYRILLIC;
    حالت HB_لیبل('G','e','o','k'): واپس HB_SCRIPT_GEORGIAN;
    حالت HB_لیبل('H','a','n','s'): واپس HB_SCRIPT_HAN;
    حالت HB_لیبل('H','a','n','t'): واپس HB_SCRIPT_HAN;
    حالت HB_لیبل('J','a','m','o'): واپس HB_SCRIPT_HANGUL;
    حالت HB_لیبل('L','a','t','f'): واپس HB_SCRIPT_LATIN;
    حالت HB_لیبل('L','a','t','g'): واپس HB_SCRIPT_LATIN;
    حالت HB_لیبل('S','y','r','e'): واپس HB_SCRIPT_SYRIAC;
    حالت HB_لیبل('S','y','r','j'): واپس HB_SCRIPT_SYRIAC;
    حالت HB_لیبل('S','y','r','n'): واپس HB_SCRIPT_SYRIAC;
  }

  /* If it looks right, just use the tag as a script */
  اگر (((uint32_t) لیبل_2 & 0xE0E0E0E0u) == 0x40606060u)
    واپس (hb_script_t) لیبل_2;

  /* Otherwise, return unknown */
  واپس HB_SCRIPT_UNKNOWN;
}

/**
 * hb_script_from_string:
 * @str: (array length=len) (element-type uint8_t): a string representing an
 *       ISO 15924 tag.
 * @len: length of the @str, or -1 if it is `NULL`-terminated.
 *
 * Converts a string @str representing an ISO 15924 script tag to a
 * corresponding #hb_script_t. Shorthand for hb_tag_from_string() then
 * hb_script_from_iso15924_tag().
 *
 * Return value:
 * An #hb_script_t corresponding to the ISO 15924 tag.
 *
 * Since: 0.9.2
 **/
hb_script_t
hb_script_from_string (ثابت حرف *str, صحیح len)
{
  واپس hb_script_from_iso15924_لیبل (hb_لیبل_from_string (str, len));
}

/**
 * hb_script_to_iso15924_tag:
 * @script: an #hb_script_t to convert.
 *
 * Converts an #hb_script_t to a corresponding ISO 15924 script tag.
 *
 * Return value:
 * An #hb_tag_t representing an ISO 15924 script tag.
 *
 * Since: 0.9.2
 **/
hb_لیبل_t
hb_script_مقصد_iso15924_لیبل (hb_script_t script)
{
  واپس (hb_لیبل_t) script;
}

/**
 * hb_script_get_horizontal_direction:
 * @script: The #hb_script_t to query
 *
 * Fetches the #hb_direction_t of a script when it is
 * set horizontally. All right-to-left scripts will return
 * #HB_DIRECTION_RTL. All left-to-right scripts will return
 * #HB_DIRECTION_LTR.
 *
 * Scripts that can be written either right-to-left or
 * left-to-right will return #HB_DIRECTION_INVALID.
 *
 * Unknown scripts will return #HB_DIRECTION_LTR.
 *
 * Return value: The horizontal #hb_direction_t of @script
 *
 * Since: 0.9.2
 **/
hb_direction_t
hb_script_get_horizontal_direction (hb_script_t script)
{
  /* https://docs.google.com/spreadsheets/d/1Y90M0Ie3MUJ6UVCRDOypOtijlMDLNNyyLk36T6iMu0o */
  انتخاب ((hb_لیبل_t) script)
  {
    /* Unicode-1.1 additions */
    حالت HB_SCRIPT_ARABIC:
    حالت HB_SCRIPT_HEBREW:

    /* Unicode-3.0 additions */
    حالت HB_SCRIPT_SYRIAC:
    حالت HB_SCRIPT_THAANA:

    /* Unicode-4.0 additions */
    حالت HB_SCRIPT_CYPRIOT:

    /* Unicode-4.1 additions */
    حالت HB_SCRIPT_KHAROSHTHI:

    /* Unicode-5.0 additions */
    حالت HB_SCRIPT_PHOENICIAN:
    حالت HB_SCRIPT_NKO:

    /* Unicode-5.1 additions */
    حالت HB_SCRIPT_LYDIAN:

    /* Unicode-5.2 additions */
    حالت HB_SCRIPT_AVESTAN:
    حالت HB_SCRIPT_IMPERIAL_ARAMAIC:
    حالت HB_SCRIPT_INSCRIPTIONAL_PAHLAVI:
    حالت HB_SCRIPT_INSCRIPTIONAL_PARTHIAN:
    حالت HB_SCRIPT_OLD_SOUTH_ARABIAN:
    حالت HB_SCRIPT_OLD_TURKIC:
    حالت HB_SCRIPT_SAMARITAN:

    /* Unicode-6.0 additions */
    حالت HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    حالت HB_SCRIPT_MEROITIC_CURSIVE:
    حالت HB_SCRIPT_MEROITIC_HIEROGLYPHS:

    /* Unicode-7.0 additions */
    حالت HB_SCRIPT_MANICHAEAN:
    حالت HB_SCRIPT_MENDE_KIKAKUI:
    حالت HB_SCRIPT_NABATAEAN:
    حالت HB_SCRIPT_OLD_NORTH_ARABIAN:
    حالت HB_SCRIPT_PALMYRENE:
    حالت HB_SCRIPT_PSALTER_PAHLAVI:

    /* Unicode-8.0 additions */
    حالت HB_SCRIPT_HATRAN:

    /* Unicode-9.0 additions */
    حالت HB_SCRIPT_ADLAM:

    /* Unicode-11.0 additions */
    حالت HB_SCRIPT_HANIFI_ROHINGYA:
    حالت HB_SCRIPT_OLD_SOGDIAN:
    حالت HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    حالت HB_SCRIPT_ELYMAIC:

    /* Unicode-13.0 additions */
    حالت HB_SCRIPT_CHORASMIAN:
    حالت HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    حالت HB_SCRIPT_OLD_UYGHUR:

    /* Unicode-16.0 additions */
    حالت HB_SCRIPT_GARAY:

    /* Unicode-17.0 additions */
    حالت HB_SCRIPT_SIDETIC:

      واپس HB_DIRECTION_RTL;


    /* https://github.com/harfbuzz/harfbuzz/issues/1000 */
    حالت HB_SCRIPT_OLD_HUNGARIAN:
    حالت HB_SCRIPT_OLD_ITALIC:
    حالت HB_SCRIPT_RUNIC:
    حالت HB_SCRIPT_TIFINAGH:

      واپس HB_DIRECTION_INVALID;
  }

  واپس HB_DIRECTION_LTR;
}


/* hb_version */


/**
 * SECTION:hb-version
 * @title: hb-version
 * @short_description: Information about the version of HarfBuzz in use
 * @include: hb.h
 *
 * These functions and macros allow accessing version of the HarfBuzz
 * library used at compile- as well as run-time, and to direct code
 * conditionally based on those versions, again, at compile- or run-time.
 **/


/**
 * hb_version:
 * @major: (out): Library major version component
 * @minor: (out): Library minor version component
 * @micro: (out): Library micro version component
 *
 * Returns library version as three integer components.
 *
 * Since: 0.9.2
 **/
خالی
hb_نسخہ (غیرعلامتی صحیح *major,
	    غیرعلامتی صحیح *minor,
	    غیرعلامتی صحیح *micro)
{
  *major = HB_نسخہ_MAJOR;
  *minor = HB_نسخہ_MINOR;
  *micro = HB_نسخہ_MICRO;
}

/**
 * hb_version_string:
 *
 * Returns library version as a string with three components.
 *
 * Return value: Library version string
 *
 * Since: 0.9.2
 **/
ثابت حرف *
hb_نسخہ_string ()
{
  واپس HB_نسخہ_STRING;
}

/**
 * hb_version_atleast:
 * @major: Library major version component
 * @minor: Library minor version component
 * @micro: Library micro version component
 *
 * Tests the library version against a minimum value,
 * as three integer components.
 *
 * Return value: `true` if the library is equal to or greater than
 * the test value, `false` otherwise
 *
 * Since: 0.9.30
 **/
hb_bool_t
hb_نسخہ_atleast (غیرعلامتی صحیح major,
		    غیرعلامتی صحیح minor,
		    غیرعلامتی صحیح micro)
{
  واپس HB_نسخہ_ATLEAST (major, minor, micro);
}



/* hb_feature_t and hb_variation_t */

ساکن منطقی
parse_space (ثابت حرف **pp, ثابت حرف *انتہا)
{
  جب (*pp < انتہا && ISSPACE (**pp))
    (*pp)++;
  واپس سچ;
}

ساکن منطقی
parse_char (ثابت حرف **pp, ثابت حرف *انتہا, حرف c)
{
  parse_space (pp, انتہا);

  اگر (*pp == انتہا || **pp != c)
    واپس جھوٹ;

  (*pp)++;
  واپس سچ;
}

ساکن منطقی
parse_uint (ثابت حرف **pp, ثابت حرف *انتہا, غیرعلامتی صحیح *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  صحیح v;
  اگر (unlikely (!hb_parse_int (pp, انتہا, &v))) واپس جھوٹ;

  *pv = v;
  واپس سچ;
}

ساکن منطقی
parse_uint32 (ثابت حرف **pp, ثابت حرف *انتہا, uint32_t *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  صحیح v;
  اگر (unlikely (!hb_parse_int (pp, انتہا, &v))) واپس جھوٹ;

  *pv = v;
  واپس سچ;
}

ساکن منطقی
parse_bool (ثابت حرف **pp, ثابت حرف *انتہا, uint32_t *pv)
{
  parse_space (pp, انتہا);

  ثابت حرف *p = *pp;
  جب (*pp < انتہا && ISALPHA(**pp))
    (*pp)++;

  /* CSS allows on/off as aliases 1/0. */
  اگر (*pp - p == 2
      && TOLOWER (p[0]) == 'o'
      && TOLOWER (p[1]) == 'n')
    *pv = 1;
  ورنہ اگر (*pp - p == 3
	   && TOLOWER (p[0]) == 'o'
	   && TOLOWER (p[1]) == 'f'
	   && TOLOWER (p[2]) == 'f')
    *pv = 0;
  ورنہ
    واپس جھوٹ;

  واپس سچ;
}

/* hb_feature_t */

ساکن منطقی
parse_feature_قدر_سابقہ (ثابت حرف **pp, ثابت حرف *انتہا, hb_feature_t *feature)
{
  اگر (parse_char (pp, انتہا, '-'))
    feature->value = 0;
  ورنہ {
    parse_char (pp, انتہا, '+');
    feature->value = 1;
  }

  واپس سچ;
}

ساکن منطقی
parse_لیبل (ثابت حرف **pp, ثابت حرف *انتہا, hb_لیبل_t *لیبل_2)
{
  parse_space (pp, انتہا);

  حرف quote = 0;

  اگر (*pp < انتہا && (**pp == '\'' || **pp == '"'))
  {
    quote = **pp;
    (*pp)++;
  }

  ثابت حرف *p = *pp;
  جب (*pp < انتہا && (**pp != ' ' && **pp != '=' && **pp != '[' && **pp != quote))
    (*pp)++;

  اگر (p == *pp || *pp - p > 4)
    واپس جھوٹ;

  *لیبل_2 = hb_لیبل_from_string (p, *pp - p);

  اگر (quote)
  {
    /* CSS expects exactly four bytes.  And we only allow quotations for
     * CSS compatibility.  So, enforce the length. */
     اگر (*pp - p != 4)
       واپس جھوٹ;
    اگر (*pp == انتہا || **pp != quote)
      واپس جھوٹ;
    (*pp)++;
  }

  واپس سچ;
}

ساکن منطقی
parse_feature_indices (ثابت حرف **pp, ثابت حرف *انتہا, hb_feature_t *feature)
{
  parse_space (pp, انتہا);

  منطقی has_آغاز;

  feature->آغاز = HB_FEATURE_GLOBAL_آغاز;
  feature->انتہا = HB_FEATURE_GLOBAL_انتہا;

  اگر (!parse_char (pp, انتہا, '['))
    واپس سچ;

  has_آغاز = parse_uint (pp, انتہا, &feature->آغاز);

  اگر (parse_char (pp, انتہا, ':') || parse_char (pp, انتہا, ';')) {
    parse_uint (pp, انتہا, &feature->انتہا);
  } ورنہ {
    اگر (has_آغاز)
      feature->انتہا = feature->آغاز + 1;
  }

  واپس parse_char (pp, انتہا, ']');
}

ساکن منطقی
parse_feature_قدر_postfix (ثابت حرف **pp, ثابت حرف *انتہا, hb_feature_t *feature)
{
  منطقی had_equal = parse_char (pp, انتہا, '=');
  منطقی had_قدر = parse_uint32 (pp, انتہا, &feature->value) ||
		   parse_bool (pp, انتہا, &feature->value);
  /* CSS doesn't use equal-sign between tag and value.
   * If there was an equal-sign, then there *must* be a value.
   * A value without an equal-sign is ok, but not required. */
  واپس !had_equal || had_قدر;
}

ساکن منطقی
parse_one_feature (ثابت حرف **pp, ثابت حرف *انتہا, hb_feature_t *feature)
{
  واپس parse_feature_قدر_سابقہ (pp, انتہا, feature) &&
	 parse_لیبل (pp, انتہا, &feature->لیبل_2) &&
	 parse_feature_indices (pp, انتہا, feature) &&
	 parse_feature_قدر_postfix (pp, انتہا, feature) &&
	 parse_space (pp, انتہا) &&
	 *pp == انتہا;
}

/**
 * hb_feature_from_string:
 * @str: (array length=len) (element-type uint8_t): a string to parse
 * @len: length of @str, or -1 if string is `NULL` terminated
 * @feature: (out): the #hb_feature_t to initialize with the parsed values
 *
 * Parses a string into a #hb_feature_t.
 *
 * The format for specifying feature strings follows. All valid CSS
 * font-feature-settings values other than 'normal' and the global values are
 * also accepted, though not documented below. CSS string escapes are not
 * supported.
 *
 * The range indices refer to the positions between Unicode characters. The
 * position before the first character is always 0.
 *
 * The format is Python-esque.  Here is how it all works:
 *
 * <informaltable pgwide='1' align='left' frame='none'>
 * <tgroup cols='5'>
 * <thead>
 * <row><entry>Syntax</entry>    <entry>Value</entry> <entry>Start</entry> <entry>End</entry></row>
 * </thead>
 * <tbody>
 * <row><entry>Setting value:</entry></row>
 * <row><entry>kern</entry>      <entry>1</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature on</entry></row>
 * <row><entry>+kern</entry>     <entry>1</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature on</entry></row>
 * <row><entry>-kern</entry>     <entry>0</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature off</entry></row>
 * <row><entry>kern=0</entry>    <entry>0</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature off</entry></row>
 * <row><entry>kern=1</entry>    <entry>1</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature on</entry></row>
 * <row><entry>aalt=2</entry>    <entry>2</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Choose 2nd alternate</entry></row>
 * <row><entry>Setting index:</entry></row>
 * <row><entry>kern[]</entry>    <entry>1</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature on</entry></row>
 * <row><entry>kern[:]</entry>   <entry>1</entry>     <entry>0</entry>      <entry>∞</entry>   <entry>Turn feature on</entry></row>
 * <row><entry>kern[5:]</entry>  <entry>1</entry>     <entry>5</entry>      <entry>∞</entry>   <entry>Turn feature on, partial</entry></row>
 * <row><entry>kern[:5]</entry>  <entry>1</entry>     <entry>0</entry>      <entry>5</entry>   <entry>Turn feature on, partial</entry></row>
 * <row><entry>kern[3:5]</entry> <entry>1</entry>     <entry>3</entry>      <entry>5</entry>   <entry>Turn feature on, range</entry></row>
 * <row><entry>kern[3]</entry>   <entry>1</entry>     <entry>3</entry>      <entry>3+1</entry> <entry>Turn feature on, single char</entry></row>
 * <row><entry>Mixing it all:</entry></row>
 * <row><entry>aalt[3:5]=2</entry> <entry>2</entry>   <entry>3</entry>      <entry>5</entry>   <entry>Turn 2nd alternate on for range</entry></row>
 * </tbody>
 * </tgroup>
 * </informaltable>
 *
 * Return value:
 * `true` if @str is successfully parsed, `false` otherwise
 *
 * Since: 0.9.5
 **/
hb_bool_t
hb_feature_from_string (ثابت حرف *str, صحیح len,
			hb_feature_t *feature)
{
  hb_feature_t feat;

  اگر (len < 0)
    len = لڑی_لمبائی (str);

  اگر (likely (parse_one_feature (&str, str + len, &feat)))
  {
    اگر (feature)
      *feature = feat;
    واپس سچ;
  }

  اگر (feature)
    hb_memset (feature, 0, حجم (*feature));
  واپس جھوٹ;
}

/**
 * hb_feature_to_string:
 * @feature: an #hb_feature_t to convert
 * @buf: (array length=size) (out): output string
 * @size: the allocated size of @buf
 *
 * Converts a #hb_feature_t into a `NULL`-terminated string in the format
 * understood by hb_feature_from_string(). The client in responsible for
 * allocating big enough size for @buf, 128 bytes is more than enough.
 *
 * Note that the feature value will be omitted if it is '1', but the
 * string won't include any whitespace.
 *
 * Since: 0.9.5
 **/
خالی
hb_feature_مقصد_string (hb_feature_t *feature,
		      حرف *buf, غیرعلامتی صحیح سائز)
{
  اگر (unlikely (!سائز)) واپس;

  حرف s[128];
  غیرعلامتی صحیح len = 0;
  اگر (feature->value == 0)
    s[len++] = '-';
  hb_لیبل_مقصد_string (feature->لیبل_2, s + len);
  len += 4;
  جب (len && s[len - 1] == ' ')
    len--;
  اگر (feature->آغاز != HB_FEATURE_GLOBAL_آغاز || feature->انتہا != HB_FEATURE_GLOBAL_انتہا)
  {
    s[len++] = '[';
    اگر (feature->آغاز)
      len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%u", feature->آغاز));
    اگر (feature->انتہا != feature->آغاز + 1) {
      s[len++] = ':';
      اگر (feature->انتہا != HB_FEATURE_GLOBAL_انتہا)
	len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%u", feature->انتہا));
    }
    s[len++] = ']';
  }
  اگر (feature->value > 1)
  {
    s[len++] = '=';
    len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%" PRIu32, feature->value));
  }
  أكد (len < ARRAY_لمبائی (s));
  len = hb_min (len, سائز - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/* hb_variation_t */

ساکن منطقی
parse_variation_قدر (ثابت حرف **pp, ثابت حرف *انتہا, hb_variation_t *variation)
{
  parse_char (pp, انتہا, '='); /* Optional. */
  دوہرا v;
  اگر (unlikely (!hb_parse_double (pp, انتہا, &v))) واپس جھوٹ;

  variation->value = v;
  واپس سچ;
}

ساکن منطقی
parse_one_variation (ثابت حرف **pp, ثابت حرف *انتہا, hb_variation_t *variation)
{
  واپس parse_لیبل (pp, انتہا, &variation->لیبل_2) &&
	 parse_variation_قدر (pp, انتہا, variation) &&
	 parse_space (pp, انتہا) &&
	 *pp == انتہا;
}

/**
 * hb_variation_from_string:
 * @str: (array length=len) (element-type uint8_t): a string to parse
 * @len: length of @str, or -1 if string is `NULL` terminated
 * @variation: (out): the #hb_variation_t to initialize with the parsed values
 *
 * Parses a string into a #hb_variation_t.
 *
 * The format for specifying variation settings follows. All valid CSS
 * font-variation-settings values other than 'normal' and 'inherited' are also
 * accepted, though, not documented below.
 *
 * The format is a tag, optionally followed by an equals sign, followed by a
 * number. For example `wght=500`, or `slnt=-7.5`.
 *
 * Return value:
 * `true` if @str is successfully parsed, `false` otherwise
 *
 * Since: 1.4.2
 */
hb_bool_t
hb_variation_from_string (ثابت حرف *str, صحیح len,
			  hb_variation_t *variation)
{
  hb_variation_t var;

  اگر (len < 0)
    len = لڑی_لمبائی (str);

  اگر (likely (parse_one_variation (&str, str + len, &var)))
  {
    اگر (variation)
      *variation = var;
    واپس سچ;
  }

  اگر (variation)
    hb_memset (variation, 0, حجم (*variation));
  واپس جھوٹ;
}

#اگر_غیر_تعریف_شدہ HB_NO_SETLOCALE

ساکن اندرونی خالی free_static_C_locale ();

ساکن ساخت hb_C_locale_lazy_loader_t : hb_lazy_loader_t<hb_remove_اشارہ_گر<hb_locale_t>,
							   hb_C_locale_lazy_loader_t>
{
  ساکن hb_locale_t create ()
  {
    hb_locale_t l = newlocale (LC_ALL_MASK, "C", تہی);
    اگر (!l)
      واپس l;

    hb_atexit (free_static_C_locale);

    واپس l;
  }
  ساکن خالی destroy (hb_locale_t l)
  {
    freelocale (l);
  }
  ساکن hb_locale_t get_null ()
  {
    واپس (hb_locale_t) 0;
  }
} static_C_locale;

ساکن اندرونی
خالی free_static_C_locale ()
{
  static_C_locale.free_instance ();
}

ساکن hb_locale_t
get_C_locale ()
{
  واپس static_C_locale.get_unconst ();
}

#اگر_ختم

/**
 * hb_variation_to_string:
 * @variation: an #hb_variation_t to convert
 * @buf: (array length=size) (out caller-allocates): output string
 * @size: the allocated size of @buf
 *
 * Converts an #hb_variation_t into a `NULL`-terminated string in the format
 * understood by hb_variation_from_string(). The client in responsible for
 * allocating big enough size for @buf, 128 bytes is more than enough.
 *
 * Note that the string won't include any whitespace.
 *
 * Since: 1.4.2
 */
خالی
hb_variation_مقصد_string (hb_variation_t *variation,
			حرف *buf, غیرعلامتی صحیح سائز)
{
  اگر (unlikely (!سائز)) واپس;

  حرف s[128];
  غیرعلامتی صحیح len = 0;
  hb_لیبل_مقصد_string (variation->لیبل_2, s + len);
  len += 4;
  جب (len && s[len - 1] == ' ')
    len--;
  s[len++] = '=';

  hb_locale_t oldlocale HB_غیر_مستعمل;
  oldlocale = hb_uselocale (get_C_locale ());
  len += hb_max (0, snprintf (s + len, ARRAY_لمبائی (s) - len, "%g", (دوہرا) variation->value));
  (خالی) hb_uselocale (oldlocale);

  أكد (len < ARRAY_لمبائی (s));
  len = hb_min (len, سائز - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/**
 * hb_color_get_alpha:
 * @color: an #hb_color_t we are interested in its channels.
 *
 * Fetches the alpha channel of the given @color.
 *
 * Return value: Alpha channel value
 *
 * Since: 2.1.0
 */
uint8_t
(hb_color_get_alpha) (hb_color_t color)
{
  واپس hb_color_get_alpha (color);
}

/**
 * hb_color_get_red:
 * @color: an #hb_color_t we are interested in its channels.
 *
 * Fetches the red channel of the given @color.
 *
 * Return value: Red channel value
 *
 * Since: 2.1.0
 */
uint8_t
(hb_color_get_red) (hb_color_t color)
{
  واپس hb_color_get_red (color);
}

/**
 * hb_color_get_green:
 * @color: an #hb_color_t we are interested in its channels.
 *
 * Fetches the green channel of the given @color.
 *
 * Return value: Green channel value
 *
 * Since: 2.1.0
 */
uint8_t
(hb_color_get_green) (hb_color_t color)
{
  واپس hb_color_get_green (color);
}

/**
 * hb_color_get_blue:
 * @color: an #hb_color_t we are interested in its channels.
 *
 * Fetches the blue channel of the given @color.
 *
 * Return value: Blue channel value
 *
 * Since: 2.1.0
 */
uint8_t
(hb_color_get_blue) (hb_color_t color)
{
  واپس hb_color_get_blue (color);
}

/**
 * hb_malloc:
 * @size: The size of the memory to allocate.
 *
 * Allocates @size bytes of memory, using the allocator set at
 * compile-time. Typically just malloc().
 *
 * Return value: A pointer to the allocated memory.
 *
 * Since: 11.0.0
 **/
خالی* hb_malloc(size_t سائز) { واپس hb_malloc_impl (سائز); }

/**
 * hb_calloc:
 * @nmemb: The number of elements to allocate.
 * @size: The size of each element.
 *
 * Allocates @nmemb elements of @size bytes each, initialized to zero,
 * using the allocator set at compile-time. Typically just calloc().
 *
 * Return value: A pointer to the allocated memory.
 *
 * Since: 11.0.0
 **/
خالی* hb_calloc(size_t nmemb, size_t سائز) { واپس hb_calloc_impl (nmemb, سائز); }

/**
 * hb_realloc:
 * @ptr: The pointer to the memory to reallocate.
 * @size: The new size of the memory.
 *
 * Reallocates the memory pointed to by @ptr to @size bytes, using the
 * allocator set at compile-time. Typically just realloc().
 *
 * Return value: A pointer to the reallocated memory.
 *
 * Since: 11.0.0
 **/
خالی* hb_realloc(خالی *ptr, size_t سائز) { واپس hb_realloc_impl (ptr, سائز); }

/**
 * hb_free:
 * @ptr: The pointer to the memory to free.
 *
 * Frees the memory pointed to by @ptr, using the allocator set at
 * compile-time. Typically just free().
 *
 * Since: 11.0.0
 **/
خالی  hb_free(خالی *ptr) { hb_free_impl (ptr); }


/* If there is no visibility control, then hb-static.cc will NOT
 * define anything.  Instead, we get it to define one set in here
 * only, so only libharfbuzz.so defines them, not other libs. */
#اگر_تعریف_شدہ HB_NO_VISIBILITY
#تعریف_منسوخ HB_NO_VISIBILITY
#شامل "hb-static.cc"
#تعریف_کرو HB_NO_VISIBILITY 1
#اگر_ختم
