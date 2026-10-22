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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-machinery.hh"


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
hb_tag_t
hb_tag_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len)
{
  অক্ষর tag[4];
  unsigned পূর্ণসংখ্যা i;

  যদি (!str || !len || !*str)
    ফেরত HB_TAG_NONE;

  যদি (len < 0 || len > 4)
    len = 4;
  জন্য (i = 0; i < (unsigned) len && str[i]; i++)
    tag[i] = str[i];
  জন্য (; i < 4; i++)
    tag[i] = ' ';

  ফেরত HB_TAG (tag[0], tag[1], tag[2], tag[3]);
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
শূন্য
hb_tag_to_string (hb_tag_t tag, অক্ষর *buf)
{
  buf[0] = (অক্ষর) (uint8_t) (tag >> 24);
  buf[1] = (অক্ষর) (uint8_t) (tag >> 16);
  buf[2] = (অক্ষর) (uint8_t) (tag >>  8);
  buf[3] = (অক্ষর) (uint8_t) (tag >>  0);
}


/* hb_direction_t */

স্থির ধ্রুবক অক্ষর direction_strings[][4] = {
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
hb_direction_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len)
{
  যদি (unlikely (!str || !len || !*str))
    ফেরত HB_DIRECTION_INVALID;

  /* Lets match loosely: just match the first letter, such that
   * all of "ltr", "left-to-right", etc work!
   */
  অক্ষর c = TOLOWER (str[0]);
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_দৈর্ঘ্য (direction_strings); i++)
    যদি (c == direction_strings[i][0])
      ফেরত (hb_direction_t) (HB_DIRECTION_LTR + i);

  ফেরত HB_DIRECTION_INVALID;
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
ধ্রুবক অক্ষর *
hb_direction_to_string (hb_direction_t direction)
{
  যদি (likely ((unsigned পূর্ণসংখ্যা) (direction - HB_DIRECTION_LTR)
	      < ARRAY_দৈর্ঘ্য (direction_strings)))
    ফেরত direction_strings[direction - HB_DIRECTION_LTR];

  ফেরত "invalid";
}


/* hb_language_t */

কাঠামো hb_language_impl_t {
  ধ্রুবক অক্ষর s[1];
};

স্থির ধ্রুবক অক্ষর canon_map[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  '-',  0,   0,
  '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9',  0,   0,   0,   0,   0,   0,
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,  '-',
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,   0
};

স্থির যৌক্তিক
lang_equal (hb_language_t  v1,
	    ধ্রুবক শূন্য    *v2)
{
  ধ্রুবক unsigned অক্ষর *p1 = (ধ্রুবক unsigned অক্ষর *) v1;
  ধ্রুবক unsigned অক্ষর *p2 = (ধ্রুবক unsigned অক্ষর *) v2;

  যখন (*p1 && *p1 == canon_map[*p2]) {
    p1++;
    p2++;
  }

  ফেরত *p1 == canon_map[*p2];
}

#যদি 0
স্থির unsigned পূর্ণসংখ্যা
lang_hash (ধ্রুবক শূন্য *key)
{
  ধ্রুবক unsigned অক্ষর *p = key;
  unsigned পূর্ণসংখ্যা h = 0;
  যখন (canon_map[*p])
    {
      h = (h << 5) - h + canon_map[*p];
      p++;
    }

  ফেরত h;
}
#endif


কাঠামো hb_language_item_t {

  কাঠামো hb_language_item_t *next;
  hb_language_t lang;

  যৌক্তিক operator == (ধ্রুবক অক্ষর *s) ধ্রুবক
  { ফেরত lang_equal (lang, s); }

  hb_language_item_t & operator = (ধ্রুবক অক্ষর *s)
  {
    /* We can't call strdup(), because we allow custom allocators. */
    size_t len = strlen(s) + 1;
    lang = (hb_language_t) hb_malloc(len);
    যদি (likely (lang))
    {
      hb_memcpy((unsigned অক্ষর *) lang, s, len);
      জন্য (unsigned অক্ষর *p = (unsigned অক্ষর *) lang; *p; p++)
	*p = canon_map[*p];
    }

    ফেরত *এই;
  }

  শূন্য fini () { hb_free ((শূন্য *) lang); }
};


/* Thread-safe lockfree language list */

স্থির hb_atomic_t<hb_language_item_t *> langs;

স্থির অন্তঃরেখ শূন্য
free_langs ()
{
retry:
  hb_language_item_t *first_lang = langs;
  যদি (unlikely (!langs.cmpexch (first_lang, nullptr)))
    যাও retry;

  যখন (first_lang) {
    hb_language_item_t *next = first_lang->next;
    first_lang->fini ();
    hb_free (first_lang);
    first_lang = next;
  }
}

স্থির hb_language_item_t *
lang_find_or_ঢোকাও (ধ্রুবক অক্ষর *key)
{
retry:
  hb_language_item_t *first_lang = langs;

  জন্য (hb_language_item_t *lang = first_lang; lang; lang = lang->next)
    যদি (*lang == key)
      ফেরত lang;

  /* Not found; allocate one. */
  hb_language_item_t *lang = (hb_language_item_t *) hb_calloc (1, আকার (hb_language_item_t));
  যদি (unlikely (!lang))
    ফেরত nullptr;
  lang->next = first_lang;
  *lang = key;
  যদি (unlikely (!lang->lang))
  {
    hb_free (lang);
    ফেরত nullptr;
  }

  যদি (unlikely (!langs.cmpexch (first_lang, lang)))
  {
    lang->fini ();
    hb_free (lang);
    যাও retry;
  }

  যদি (!first_lang)
    hb_atexit (free_langs); /* First person registers atexit() callback. */

  ফেরত lang;
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
hb_language_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len)
{
  যদি (!str || !len || !*str)
    ফেরত HB_LANGUAGE_INVALID;

  hb_language_item_t *item = nullptr;
  যদি (len >= 0)
  {
    /* NUL-terminate it. */
    অক্ষর strbuf[64];
    len = hb_min (len, (পূর্ণসংখ্যা) আকার (strbuf) - 1);
    hb_memcpy (strbuf, str, len);
    strbuf[len] = '\0';
    item = lang_find_or_ঢোকাও (strbuf);
  }
  অন্যথা
    item = lang_find_or_ঢোকাও (str);

  ফেরত likely (item) ? item->lang : HB_LANGUAGE_INVALID;
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
ধ্রুবক অক্ষর *
hb_language_to_string (hb_language_t language)
{
  যদি (unlikely (!language)) ফেরত nullptr;

  ফেরত language->s;
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
  স্থির hb_atomic_t<hb_language_t> default_language;

  hb_language_t language = default_language;
  যদি (unlikely (language == HB_LANGUAGE_INVALID))
  {
    language = hb_language_from_string (hb_setlocale (LC_CTYPE, nullptr), -1);
    (শূন্য) default_language.cmpexch (HB_LANGUAGE_INVALID, language);
  }

  ফেরত language;
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
  যদি (language == specific) ফেরত সত্য;
  যদি (!language || !specific) ফেরত মিথ্যা;

  ধ্রুবক অক্ষর *l = language->s;
  ধ্রুবক অক্ষর *s = specific->s;
  unsigned ll = strlen (l);
  unsigned sl = strlen (s);

  যদি (ll > sl)
    ফেরত মিথ্যা;

  ফেরত strncmp (l, s, ll) == 0 &&
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
hb_script_from_iso15924_tag (hb_tag_t tag)
{
  যদি (unlikely (tag == HB_TAG_NONE))
    ফেরত HB_SCRIPT_INVALID;

  /* Be lenient, adjust case (one capital letter followed by three small letters) */
  tag = (tag & 0xDFDFDFDFu) | 0x00202020u;

  নির্বাচন (tag)
  {
    /* Script variants from https://unicode.org/iso15924/ */
    ক্ষেত্র HB_TAG('A','r','a','n'): ফেরত HB_SCRIPT_ARABIC;
    ক্ষেত্র HB_TAG('C','y','r','s'): ফেরত HB_SCRIPT_CYRILLIC;
    ক্ষেত্র HB_TAG('G','e','o','k'): ফেরত HB_SCRIPT_GEORGIAN;
    ক্ষেত্র HB_TAG('H','a','n','s'): ফেরত HB_SCRIPT_HAN;
    ক্ষেত্র HB_TAG('H','a','n','t'): ফেরত HB_SCRIPT_HAN;
    ক্ষেত্র HB_TAG('J','a','m','o'): ফেরত HB_SCRIPT_HANGUL;
    ক্ষেত্র HB_TAG('L','a','t','f'): ফেরত HB_SCRIPT_LATIN;
    ক্ষেত্র HB_TAG('L','a','t','g'): ফেরত HB_SCRIPT_LATIN;
    ক্ষেত্র HB_TAG('S','y','r','e'): ফেরত HB_SCRIPT_SYRIAC;
    ক্ষেত্র HB_TAG('S','y','r','j'): ফেরত HB_SCRIPT_SYRIAC;
    ক্ষেত্র HB_TAG('S','y','r','n'): ফেরত HB_SCRIPT_SYRIAC;
  }

  /* If it looks right, just use the tag as a script */
  যদি (((uint32_t) tag & 0xE0E0E0E0u) == 0x40606060u)
    ফেরত (hb_script_t) tag;

  /* Otherwise, return unknown */
  ফেরত HB_SCRIPT_UNKNOWN;
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
hb_script_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len)
{
  ফেরত hb_script_from_iso15924_tag (hb_tag_from_string (str, len));
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
hb_tag_t
hb_script_to_iso15924_tag (hb_script_t script)
{
  ফেরত (hb_tag_t) script;
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
  নির্বাচন ((hb_tag_t) script)
  {
    /* Unicode-1.1 additions */
    ক্ষেত্র HB_SCRIPT_ARABIC:
    ক্ষেত্র HB_SCRIPT_HEBREW:

    /* Unicode-3.0 additions */
    ক্ষেত্র HB_SCRIPT_SYRIAC:
    ক্ষেত্র HB_SCRIPT_THAANA:

    /* Unicode-4.0 additions */
    ক্ষেত্র HB_SCRIPT_CYPRIOT:

    /* Unicode-4.1 additions */
    ক্ষেত্র HB_SCRIPT_KHAROSHTHI:

    /* Unicode-5.0 additions */
    ক্ষেত্র HB_SCRIPT_PHOENICIAN:
    ক্ষেত্র HB_SCRIPT_NKO:

    /* Unicode-5.1 additions */
    ক্ষেত্র HB_SCRIPT_LYDIAN:

    /* Unicode-5.2 additions */
    ক্ষেত্র HB_SCRIPT_AVESTAN:
    ক্ষেত্র HB_SCRIPT_IMPERIAL_ARAMAIC:
    ক্ষেত্র HB_SCRIPT_INSCRIPTIONAL_PAHLAVI:
    ক্ষেত্র HB_SCRIPT_INSCRIPTIONAL_PARTHIAN:
    ক্ষেত্র HB_SCRIPT_OLD_SOUTH_ARABIAN:
    ক্ষেত্র HB_SCRIPT_OLD_TURKIC:
    ক্ষেত্র HB_SCRIPT_SAMARITAN:

    /* Unicode-6.0 additions */
    ক্ষেত্র HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    ক্ষেত্র HB_SCRIPT_MEROITIC_CURSIVE:
    ক্ষেত্র HB_SCRIPT_MEROITIC_HIEROGLYPHS:

    /* Unicode-7.0 additions */
    ক্ষেত্র HB_SCRIPT_MANICHAEAN:
    ক্ষেত্র HB_SCRIPT_MENDE_KIKAKUI:
    ক্ষেত্র HB_SCRIPT_NABATAEAN:
    ক্ষেত্র HB_SCRIPT_OLD_NORTH_ARABIAN:
    ক্ষেত্র HB_SCRIPT_PALMYRENE:
    ক্ষেত্র HB_SCRIPT_PSALTER_PAHLAVI:

    /* Unicode-8.0 additions */
    ক্ষেত্র HB_SCRIPT_HATRAN:

    /* Unicode-9.0 additions */
    ক্ষেত্র HB_SCRIPT_ADLAM:

    /* Unicode-11.0 additions */
    ক্ষেত্র HB_SCRIPT_HANIFI_ROHINGYA:
    ক্ষেত্র HB_SCRIPT_OLD_SOGDIAN:
    ক্ষেত্র HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    ক্ষেত্র HB_SCRIPT_ELYMAIC:

    /* Unicode-13.0 additions */
    ক্ষেত্র HB_SCRIPT_CHORASMIAN:
    ক্ষেত্র HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    ক্ষেত্র HB_SCRIPT_OLD_UYGHUR:

    /* Unicode-16.0 additions */
    ক্ষেত্র HB_SCRIPT_GARAY:

    /* Unicode-17.0 additions */
    ক্ষেত্র HB_SCRIPT_SIDETIC:

      ফেরত HB_DIRECTION_RTL;


    /* https://github.com/harfbuzz/harfbuzz/issues/1000 */
    ক্ষেত্র HB_SCRIPT_OLD_HUNGARIAN:
    ক্ষেত্র HB_SCRIPT_OLD_ITALIC:
    ক্ষেত্র HB_SCRIPT_RUNIC:
    ক্ষেত্র HB_SCRIPT_TIFINAGH:

      ফেরত HB_DIRECTION_INVALID;
  }

  ফেরত HB_DIRECTION_LTR;
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
শূন্য
hb_version (unsigned পূর্ণসংখ্যা *major,
	    unsigned পূর্ণসংখ্যা *minor,
	    unsigned পূর্ণসংখ্যা *micro)
{
  *major = HB_VERSION_MAJOR;
  *minor = HB_VERSION_MINOR;
  *micro = HB_VERSION_MICRO;
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
ধ্রুবক অক্ষর *
hb_version_string ()
{
  ফেরত HB_VERSION_STRING;
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
hb_version_atleast (unsigned পূর্ণসংখ্যা major,
		    unsigned পূর্ণসংখ্যা minor,
		    unsigned পূর্ণসংখ্যা micro)
{
  ফেরত HB_VERSION_ATLEAST (major, minor, micro);
}



/* hb_feature_t and hb_variation_t */

স্থির যৌক্তিক
parse_space (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end)
{
  যখন (*pp < end && ISSPACE (**pp))
    (*pp)++;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_char (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, অক্ষর c)
{
  parse_space (pp, end);

  যদি (*pp == end || **pp != c)
    ফেরত মিথ্যা;

  (*pp)++;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_uint (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, unsigned পূর্ণসংখ্যা *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  পূর্ণসংখ্যা v;
  যদি (unlikely (!hb_parse_int (pp, end, &v))) ফেরত মিথ্যা;

  *pv = v;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_uint32 (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, uint32_t *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  পূর্ণসংখ্যা v;
  যদি (unlikely (!hb_parse_int (pp, end, &v))) ফেরত মিথ্যা;

  *pv = v;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_bool (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, uint32_t *pv)
{
  parse_space (pp, end);

  ধ্রুবক অক্ষর *p = *pp;
  যখন (*pp < end && ISALPHA(**pp))
    (*pp)++;

  /* CSS allows on/off as aliases 1/0. */
  যদি (*pp - p == 2
      && TOLOWER (p[0]) == 'o'
      && TOLOWER (p[1]) == 'n')
    *pv = 1;
  অন্যথা যদি (*pp - p == 3
	   && TOLOWER (p[0]) == 'o'
	   && TOLOWER (p[1]) == 'f'
	   && TOLOWER (p[2]) == 'f')
    *pv = 0;
  অন্যথা
    ফেরত মিথ্যা;

  ফেরত সত্য;
}

/* hb_feature_t */

স্থির যৌক্তিক
parse_feature_মান_prefix (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_feature_t *feature)
{
  যদি (parse_char (pp, end, '-'))
    feature->value = 0;
  অন্যথা {
    parse_char (pp, end, '+');
    feature->value = 1;
  }

  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_tag (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_tag_t *tag)
{
  parse_space (pp, end);

  অক্ষর quote = 0;

  যদি (*pp < end && (**pp == '\'' || **pp == '"'))
  {
    quote = **pp;
    (*pp)++;
  }

  ধ্রুবক অক্ষর *p = *pp;
  যখন (*pp < end && (**pp != ' ' && **pp != '=' && **pp != '[' && **pp != quote))
    (*pp)++;

  যদি (p == *pp || *pp - p > 4)
    ফেরত মিথ্যা;

  *tag = hb_tag_from_string (p, *pp - p);

  যদি (quote)
  {
    /* CSS expects exactly four bytes.  And we only allow quotations for
     * CSS compatibility.  So, enforce the length. */
     যদি (*pp - p != 4)
       ফেরত মিথ্যা;
    যদি (*pp == end || **pp != quote)
      ফেরত মিথ্যা;
    (*pp)++;
  }

  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_feature_indices (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_feature_t *feature)
{
  parse_space (pp, end);

  যৌক্তিক has_start;

  feature->start = HB_FEATURE_GLOBAL_START;
  feature->end = HB_FEATURE_GLOBAL_END;

  যদি (!parse_char (pp, end, '['))
    ফেরত সত্য;

  has_start = parse_uint (pp, end, &feature->start);

  যদি (parse_char (pp, end, ':') || parse_char (pp, end, ';')) {
    parse_uint (pp, end, &feature->end);
  } অন্যথা {
    যদি (has_start)
      feature->end = feature->start + 1;
  }

  ফেরত parse_char (pp, end, ']');
}

স্থির যৌক্তিক
parse_feature_মান_postfix (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_feature_t *feature)
{
  যৌক্তিক had_equal = parse_char (pp, end, '=');
  যৌক্তিক had_মান = parse_uint32 (pp, end, &feature->value) ||
		   parse_bool (pp, end, &feature->value);
  /* CSS doesn't use equal-sign between tag and value.
   * If there was an equal-sign, then there *must* be a value.
   * A value without an equal-sign is ok, but not required. */
  ফেরত !had_equal || had_মান;
}

স্থির যৌক্তিক
parse_one_feature (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_feature_t *feature)
{
  ফেরত parse_feature_মান_prefix (pp, end, feature) &&
	 parse_tag (pp, end, &feature->tag) &&
	 parse_feature_indices (pp, end, feature) &&
	 parse_feature_মান_postfix (pp, end, feature) &&
	 parse_space (pp, end) &&
	 *pp == end;
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
hb_feature_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len,
			hb_feature_t *feature)
{
  hb_feature_t feat;

  যদি (len < 0)
    len = strlen (str);

  যদি (likely (parse_one_feature (&str, str + len, &feat)))
  {
    যদি (feature)
      *feature = feat;
    ফেরত সত্য;
  }

  যদি (feature)
    hb_memset (feature, 0, আকার (*feature));
  ফেরত মিথ্যা;
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
শূন্য
hb_feature_to_string (hb_feature_t *feature,
		      অক্ষর *buf, unsigned পূর্ণসংখ্যা আকার_2)
{
  যদি (unlikely (!আকার_2)) ফেরত;

  অক্ষর s[128];
  unsigned পূর্ণসংখ্যা len = 0;
  যদি (feature->value == 0)
    s[len++] = '-';
  hb_tag_to_string (feature->tag, s + len);
  len += 4;
  যখন (len && s[len - 1] == ' ')
    len--;
  যদি (feature->start != HB_FEATURE_GLOBAL_START || feature->end != HB_FEATURE_GLOBAL_END)
  {
    s[len++] = '[';
    যদি (feature->start)
      len += hb_max (0, snprintf (s + len, ARRAY_দৈর্ঘ্য (s) - len, "%u", feature->start));
    যদি (feature->end != feature->start + 1) {
      s[len++] = ':';
      যদি (feature->end != HB_FEATURE_GLOBAL_END)
	len += hb_max (0, snprintf (s + len, ARRAY_দৈর্ঘ্য (s) - len, "%u", feature->end));
    }
    s[len++] = ']';
  }
  যদি (feature->value > 1)
  {
    s[len++] = '=';
    len += hb_max (0, snprintf (s + len, ARRAY_দৈর্ঘ্য (s) - len, "%" PRIu32, feature->value));
  }
  প্রতিজ্ঞা (len < ARRAY_দৈর্ঘ্য (s));
  len = hb_min (len, আকার_2 - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/* hb_variation_t */

স্থির যৌক্তিক
parse_variation_মান (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_variation_t *variation)
{
  parse_char (pp, end, '='); /* Optional. */
  দ্বিগুণ v;
  যদি (unlikely (!hb_parse_double (pp, end, &v))) ফেরত মিথ্যা;

  variation->value = v;
  ফেরত সত্য;
}

স্থির যৌক্তিক
parse_one_variation (ধ্রুবক অক্ষর **pp, ধ্রুবক অক্ষর *end, hb_variation_t *variation)
{
  ফেরত parse_tag (pp, end, &variation->tag) &&
	 parse_variation_মান (pp, end, variation) &&
	 parse_space (pp, end) &&
	 *pp == end;
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
hb_variation_from_string (ধ্রুবক অক্ষর *str, পূর্ণসংখ্যা len,
			  hb_variation_t *variation)
{
  hb_variation_t var;

  যদি (len < 0)
    len = strlen (str);

  যদি (likely (parse_one_variation (&str, str + len, &var)))
  {
    যদি (variation)
      *variation = var;
    ফেরত সত্য;
  }

  যদি (variation)
    hb_memset (variation, 0, আকার (*variation));
  ফেরত মিথ্যা;
}

#ifndef HB_NO_SETLOCALE

স্থির অন্তঃরেখ শূন্য free_static_C_locale ();

স্থির কাঠামো hb_C_locale_lazy_loader_t : hb_lazy_loader_t<hb_remove_pointer<hb_locale_t>,
							   hb_C_locale_lazy_loader_t>
{
  স্থির hb_locale_t create ()
  {
    hb_locale_t l = newlocale (LC_ALL_MASK, "C", ফাঁকা);
    যদি (!l)
      ফেরত l;

    hb_atexit (free_static_C_locale);

    ফেরত l;
  }
  স্থির শূন্য destroy (hb_locale_t l)
  {
    freelocale (l);
  }
  স্থির hb_locale_t get_null ()
  {
    ফেরত (hb_locale_t) 0;
  }
} static_C_locale;

স্থির অন্তঃরেখ
শূন্য free_static_C_locale ()
{
  static_C_locale.free_instance ();
}

স্থির hb_locale_t
get_C_locale ()
{
  ফেরত static_C_locale.get_unconst ();
}

#endif

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
শূন্য
hb_variation_to_string (hb_variation_t *variation,
			অক্ষর *buf, unsigned পূর্ণসংখ্যা আকার_2)
{
  যদি (unlikely (!আকার_2)) ফেরত;

  অক্ষর s[128];
  unsigned পূর্ণসংখ্যা len = 0;
  hb_tag_to_string (variation->tag, s + len);
  len += 4;
  যখন (len && s[len - 1] == ' ')
    len--;
  s[len++] = '=';

  hb_locale_t oldlocale HB_UNUSED;
  oldlocale = hb_uselocale (get_C_locale ());
  len += hb_max (0, snprintf (s + len, ARRAY_দৈর্ঘ্য (s) - len, "%g", (দ্বিগুণ) variation->value));
  (শূন্য) hb_uselocale (oldlocale);

  প্রতিজ্ঞা (len < ARRAY_দৈর্ঘ্য (s));
  len = hb_min (len, আকার_2 - 1);
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
  ফেরত hb_color_get_alpha (color);
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
  ফেরত hb_color_get_red (color);
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
  ফেরত hb_color_get_green (color);
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
  ফেরত hb_color_get_blue (color);
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
শূন্য* hb_malloc(size_t আকার_2) { ফেরত hb_malloc_impl (আকার_2); }

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
শূন্য* hb_calloc(size_t nmemb, size_t আকার_2) { ফেরত hb_calloc_impl (nmemb, আকার_2); }

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
শূন্য* hb_realloc(শূন্য *ptr, size_t আকার_2) { ফেরত hb_realloc_impl (ptr, আকার_2); }

/**
 * hb_free:
 * @ptr: The pointer to the memory to free.
 *
 * Frees the memory pointed to by @ptr, using the allocator set at
 * compile-time. Typically just free().
 *
 * Since: 11.0.0
 **/
শূন্য  hb_free(শূন্য *ptr) { hb_free_impl (ptr); }


/* If there is no visibility control, then hb-static.cc will NOT
 * define anything.  Instead, we get it to define one set in here
 * only, so only libharfbuzz.so defines them, not other libs. */
#ifdef HB_NO_VISIBILITY
#undef HB_NO_VISIBILITY
#অন্তর্ভাব "hb-static.cc"
#define HB_NO_VISIBILITY 1
#endif
