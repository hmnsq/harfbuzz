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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-machinery.hh"


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
hb_tag_from_string (अचर वर्ण *str, पूर्णाङ्क len)
{
  वर्ण tag[4];
  अचिन्हित पूर्णाङ्क i;

  यदि (!str || !len || !*str)
    फर्काऊ HB_TAG_NONE;

  यदि (len < 0 || len > 4)
    len = 4;
  लागि (i = 0; i < (अचिन्हित) len && str[i]; i++)
    tag[i] = str[i];
  लागि (; i < 4; i++)
    tag[i] = ' ';

  फर्काऊ HB_TAG (tag[0], tag[1], tag[2], tag[3]);
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
शून्य
hb_tag_to_string (hb_tag_t tag, वर्ण *buf)
{
  buf[0] = (वर्ण) (uint8_t) (tag >> 24);
  buf[1] = (वर्ण) (uint8_t) (tag >> 16);
  buf[2] = (वर्ण) (uint8_t) (tag >>  8);
  buf[3] = (वर्ण) (uint8_t) (tag >>  0);
}


/* hb_direction_t */

स्थिर अचर वर्ण direction_strings[][4] = {
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
hb_direction_from_string (अचर वर्ण *str, पूर्णाङ्क len)
{
  यदि (unlikely (!str || !len || !*str))
    फर्काऊ HB_DIRECTION_INVALID;

  /* Lets match loosely: just match the first letter, such that
   * all of "ltr", "left-to-right", etc work!
   */
  वर्ण c = TOLOWER (str[0]);
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < ARRAY_LENGTH (direction_strings); i++)
    यदि (c == direction_strings[i][0])
      फर्काऊ (hb_direction_t) (HB_DIRECTION_LTR + i);

  फर्काऊ HB_DIRECTION_INVALID;
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
अचर वर्ण *
hb_direction_to_string (hb_direction_t direction)
{
  यदि (likely ((अचिन्हित पूर्णाङ्क) (direction - HB_DIRECTION_LTR)
	      < ARRAY_LENGTH (direction_strings)))
    फर्काऊ direction_strings[direction - HB_DIRECTION_LTR];

  फर्काऊ "invalid";
}


/* hb_language_t */

संरचना hb_language_impl_t {
  अचर वर्ण s[1];
};

स्थिर अचर वर्ण canon_map[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  '-',  0,   0,
  '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9',  0,   0,   0,   0,   0,   0,
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,  '-',
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,   0
};

स्थिर तार्किक
lang_equal (hb_language_t  v1,
	    अचर शून्य    *v2)
{
  अचर अचिन्हित वर्ण *p1 = (अचर अचिन्हित वर्ण *) v1;
  अचर अचिन्हित वर्ण *p2 = (अचर अचिन्हित वर्ण *) v2;

  जबसम्म (*p1 && *p1 == canon_map[*p2]) {
    p1++;
    p2++;
  }

  फर्काऊ *p1 == canon_map[*p2];
}

#यदि 0
स्थिर अचिन्हित पूर्णाङ्क
lang_hash (अचर शून्य *key)
{
  अचर अचिन्हित वर्ण *p = key;
  अचिन्हित पूर्णाङ्क h = 0;
  जबसम्म (canon_map[*p])
    {
      h = (h << 5) - h + canon_map[*p];
      p++;
    }

  फर्काऊ h;
}
#endif


संरचना hb_language_item_t {

  संरचना hb_language_item_t *next;
  hb_language_t lang;

  तार्किक चालक == (अचर वर्ण *s) अचर
  { फर्काऊ lang_equal (lang, s); }

  hb_language_item_t & चालक = (अचर वर्ण *s)
  {
    /* We can't call strdup(), because we allow custom allocators. */
    माप_प्रकार len = म_माप(s) + 1;
    lang = (hb_language_t) hb_malloc(len);
    यदि (likely (lang))
    {
      hb_memcpy((अचिन्हित वर्ण *) lang, s, len);
      लागि (अचिन्हित वर्ण *p = (अचिन्हित वर्ण *) lang; *p; p++)
	*p = canon_map[*p];
    }

    फर्काऊ *एतत्;
  }

  शून्य fini () { hb_free ((शून्य *) lang); }
};


/* Thread-safe lockfree language list */

स्थिर hb_atomic_t<hb_language_item_t *> langs;

स्थिर अन्तःस्थ शून्य
free_langs ()
{
retry:
  hb_language_item_t *first_lang = langs;
  यदि (unlikely (!langs.cmpexch (first_lang, nullptr)))
    गच्छ retry;

  जबसम्म (first_lang) {
    hb_language_item_t *next = first_lang->next;
    first_lang->fini ();
    hb_free (first_lang);
    first_lang = next;
  }
}

स्थिर hb_language_item_t *
lang_find_or_घुसाउ (अचर वर्ण *key)
{
retry:
  hb_language_item_t *first_lang = langs;

  लागि (hb_language_item_t *lang = first_lang; lang; lang = lang->next)
    यदि (*lang == key)
      फर्काऊ lang;

  /* Not found; allocate one. */
  hb_language_item_t *lang = (hb_language_item_t *) hb_calloc (1, परिमाण (hb_language_item_t));
  यदि (unlikely (!lang))
    फर्काऊ nullptr;
  lang->next = first_lang;
  *lang = key;
  यदि (unlikely (!lang->lang))
  {
    hb_free (lang);
    फर्काऊ nullptr;
  }

  यदि (unlikely (!langs.cmpexch (first_lang, lang)))
  {
    lang->fini ();
    hb_free (lang);
    गच्छ retry;
  }

  यदि (!first_lang)
    hb_atexit (free_langs); /* First person registers atexit() callback. */

  फर्काऊ lang;
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
hb_language_from_string (अचर वर्ण *str, पूर्णाङ्क len)
{
  यदि (!str || !len || !*str)
    फर्काऊ HB_LANGUAGE_INVALID;

  hb_language_item_t *item = nullptr;
  यदि (len >= 0)
  {
    /* NUL-terminate it. */
    वर्ण strbuf[64];
    len = hb_min (len, (पूर्णाङ्क) परिमाण (strbuf) - 1);
    hb_memcpy (strbuf, str, len);
    strbuf[len] = '\0';
    item = lang_find_or_घुसाउ (strbuf);
  }
  अन्यथा
    item = lang_find_or_घुसाउ (str);

  फर्काऊ likely (item) ? item->lang : HB_LANGUAGE_INVALID;
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
अचर वर्ण *
hb_language_to_string (hb_language_t language)
{
  यदि (unlikely (!language)) फर्काऊ nullptr;

  फर्काऊ language->s;
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
  स्थिर hb_atomic_t<hb_language_t> default_language;

  hb_language_t language = default_language;
  यदि (unlikely (language == HB_LANGUAGE_INVALID))
  {
    language = hb_language_from_string (hb_setlocale (LC_CTYPE, nullptr), -1);
    (शून्य) default_language.cmpexch (HB_LANGUAGE_INVALID, language);
  }

  फर्काऊ language;
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
  यदि (language == specific) फर्काऊ सत्य;
  यदि (!language || !specific) फर्काऊ असत्य;

  अचर वर्ण *l = language->s;
  अचर वर्ण *s = specific->s;
  अचिन्हित ll = म_माप (l);
  अचिन्हित sl = म_माप (s);

  यदि (ll > sl)
    फर्काऊ असत्य;

  फर्काऊ म_भेदन (l, s, ll) == 0 &&
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
  यदि (unlikely (tag == HB_TAG_NONE))
    फर्काऊ HB_SCRIPT_INVALID;

  /* Be lenient, adjust case (one capital letter followed by three small letters) */
  tag = (tag & 0xDFDFDFDFu) | 0x00202020u;

  विकल्प (tag)
  {
    /* Script variants from https://unicode.org/iso15924/ */
    पक्ष HB_TAG('A','r','a','n'): फर्काऊ HB_SCRIPT_ARABIC;
    पक्ष HB_TAG('C','y','r','s'): फर्काऊ HB_SCRIPT_CYRILLIC;
    पक्ष HB_TAG('G','e','o','k'): फर्काऊ HB_SCRIPT_GEORGIAN;
    पक्ष HB_TAG('H','a','n','s'): फर्काऊ HB_SCRIPT_HAN;
    पक्ष HB_TAG('H','a','n','t'): फर्काऊ HB_SCRIPT_HAN;
    पक्ष HB_TAG('J','a','m','o'): फर्काऊ HB_SCRIPT_HANGUL;
    पक्ष HB_TAG('L','a','t','f'): फर्काऊ HB_SCRIPT_LATIN;
    पक्ष HB_TAG('L','a','t','g'): फर्काऊ HB_SCRIPT_LATIN;
    पक्ष HB_TAG('S','y','r','e'): फर्काऊ HB_SCRIPT_SYRIAC;
    पक्ष HB_TAG('S','y','r','j'): फर्काऊ HB_SCRIPT_SYRIAC;
    पक्ष HB_TAG('S','y','r','n'): फर्काऊ HB_SCRIPT_SYRIAC;
  }

  /* If it looks right, just use the tag as a script */
  यदि (((uint32_t) tag & 0xE0E0E0E0u) == 0x40606060u)
    फर्काऊ (hb_script_t) tag;

  /* Otherwise, return unknown */
  फर्काऊ HB_SCRIPT_UNKNOWN;
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
hb_script_from_string (अचर वर्ण *str, पूर्णाङ्क len)
{
  फर्काऊ hb_script_from_iso15924_tag (hb_tag_from_string (str, len));
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
  फर्काऊ (hb_tag_t) script;
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
  विकल्प ((hb_tag_t) script)
  {
    /* Unicode-1.1 additions */
    पक्ष HB_SCRIPT_ARABIC:
    पक्ष HB_SCRIPT_HEBREW:

    /* Unicode-3.0 additions */
    पक्ष HB_SCRIPT_SYRIAC:
    पक्ष HB_SCRIPT_THAANA:

    /* Unicode-4.0 additions */
    पक्ष HB_SCRIPT_CYPRIOT:

    /* Unicode-4.1 additions */
    पक्ष HB_SCRIPT_KHAROSHTHI:

    /* Unicode-5.0 additions */
    पक्ष HB_SCRIPT_PHOENICIAN:
    पक्ष HB_SCRIPT_NKO:

    /* Unicode-5.1 additions */
    पक्ष HB_SCRIPT_LYDIAN:

    /* Unicode-5.2 additions */
    पक्ष HB_SCRIPT_AVESTAN:
    पक्ष HB_SCRIPT_IMPERIAL_ARAMAIC:
    पक्ष HB_SCRIPT_INSCRIPTIONAL_PAHLAVI:
    पक्ष HB_SCRIPT_INSCRIPTIONAL_PARTHIAN:
    पक्ष HB_SCRIPT_OLD_SOUTH_ARABIAN:
    पक्ष HB_SCRIPT_OLD_TURKIC:
    पक्ष HB_SCRIPT_SAMARITAN:

    /* Unicode-6.0 additions */
    पक्ष HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    पक्ष HB_SCRIPT_MEROITIC_CURSIVE:
    पक्ष HB_SCRIPT_MEROITIC_HIEROGLYPHS:

    /* Unicode-7.0 additions */
    पक्ष HB_SCRIPT_MANICHAEAN:
    पक्ष HB_SCRIPT_MENDE_KIKAKUI:
    पक्ष HB_SCRIPT_NABATAEAN:
    पक्ष HB_SCRIPT_OLD_NORTH_ARABIAN:
    पक्ष HB_SCRIPT_PALMYRENE:
    पक्ष HB_SCRIPT_PSALTER_PAHLAVI:

    /* Unicode-8.0 additions */
    पक्ष HB_SCRIPT_HATRAN:

    /* Unicode-9.0 additions */
    पक्ष HB_SCRIPT_ADLAM:

    /* Unicode-11.0 additions */
    पक्ष HB_SCRIPT_HANIFI_ROHINGYA:
    पक्ष HB_SCRIPT_OLD_SOGDIAN:
    पक्ष HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    पक्ष HB_SCRIPT_ELYMAIC:

    /* Unicode-13.0 additions */
    पक्ष HB_SCRIPT_CHORASMIAN:
    पक्ष HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    पक्ष HB_SCRIPT_OLD_UYGHUR:

    /* Unicode-16.0 additions */
    पक्ष HB_SCRIPT_GARAY:

    /* Unicode-17.0 additions */
    पक्ष HB_SCRIPT_SIDETIC:

      फर्काऊ HB_DIRECTION_RTL;


    /* https://github.com/harfbuzz/harfbuzz/issues/1000 */
    पक्ष HB_SCRIPT_OLD_HUNGARIAN:
    पक्ष HB_SCRIPT_OLD_ITALIC:
    पक्ष HB_SCRIPT_RUNIC:
    पक्ष HB_SCRIPT_TIFINAGH:

      फर्काऊ HB_DIRECTION_INVALID;
  }

  फर्काऊ HB_DIRECTION_LTR;
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
शून्य
hb_version (अचिन्हित पूर्णाङ्क *major,
	    अचिन्हित पूर्णाङ्क *minor,
	    अचिन्हित पूर्णाङ्क *micro)
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
अचर वर्ण *
hb_version_string ()
{
  फर्काऊ HB_VERSION_STRING;
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
hb_version_atleast (अचिन्हित पूर्णाङ्क major,
		    अचिन्हित पूर्णाङ्क minor,
		    अचिन्हित पूर्णाङ्क micro)
{
  फर्काऊ HB_VERSION_ATLEAST (major, minor, micro);
}



/* hb_feature_t and hb_variation_t */

स्थिर तार्किक
parse_space (अचर वर्ण **pp, अचर वर्ण *end)
{
  जबसम्म (*pp < end && ISSPACE (**pp))
    (*pp)++;
  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_char (अचर वर्ण **pp, अचर वर्ण *end, वर्ण c)
{
  parse_space (pp, end);

  यदि (*pp == end || **pp != c)
    फर्काऊ असत्य;

  (*pp)++;
  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_uint (अचर वर्ण **pp, अचर वर्ण *end, अचिन्हित पूर्णाङ्क *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  पूर्णाङ्क v;
  यदि (unlikely (!hb_parse_int (pp, end, &v))) फर्काऊ असत्य;

  *pv = v;
  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_uint32 (अचर वर्ण **pp, अचर वर्ण *end, uint32_t *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  पूर्णाङ्क v;
  यदि (unlikely (!hb_parse_int (pp, end, &v))) फर्काऊ असत्य;

  *pv = v;
  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_bool (अचर वर्ण **pp, अचर वर्ण *end, uint32_t *pv)
{
  parse_space (pp, end);

  अचर वर्ण *p = *pp;
  जबसम्म (*pp < end && ISALPHA(**pp))
    (*pp)++;

  /* CSS allows on/off as aliases 1/0. */
  यदि (*pp - p == 2
      && TOLOWER (p[0]) == 'o'
      && TOLOWER (p[1]) == 'n')
    *pv = 1;
  अन्यथा यदि (*pp - p == 3
	   && TOLOWER (p[0]) == 'o'
	   && TOLOWER (p[1]) == 'f'
	   && TOLOWER (p[2]) == 'f')
    *pv = 0;
  अन्यथा
    फर्काऊ असत्य;

  फर्काऊ सत्य;
}

/* hb_feature_t */

स्थिर तार्किक
parse_feature_मान_prefix (अचर वर्ण **pp, अचर वर्ण *end, hb_feature_t *feature)
{
  यदि (parse_char (pp, end, '-'))
    feature->value = 0;
  अन्यथा {
    parse_char (pp, end, '+');
    feature->value = 1;
  }

  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_tag (अचर वर्ण **pp, अचर वर्ण *end, hb_tag_t *tag)
{
  parse_space (pp, end);

  वर्ण quote = 0;

  यदि (*pp < end && (**pp == '\'' || **pp == '"'))
  {
    quote = **pp;
    (*pp)++;
  }

  अचर वर्ण *p = *pp;
  जबसम्म (*pp < end && (**pp != ' ' && **pp != '=' && **pp != '[' && **pp != quote))
    (*pp)++;

  यदि (p == *pp || *pp - p > 4)
    फर्काऊ असत्य;

  *tag = hb_tag_from_string (p, *pp - p);

  यदि (quote)
  {
    /* CSS expects exactly four bytes.  And we only allow quotations for
     * CSS compatibility.  So, enforce the length. */
     यदि (*pp - p != 4)
       फर्काऊ असत्य;
    यदि (*pp == end || **pp != quote)
      फर्काऊ असत्य;
    (*pp)++;
  }

  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_feature_indices (अचर वर्ण **pp, अचर वर्ण *end, hb_feature_t *feature)
{
  parse_space (pp, end);

  तार्किक has_start;

  feature->start = HB_FEATURE_GLOBAL_START;
  feature->end = HB_FEATURE_GLOBAL_END;

  यदि (!parse_char (pp, end, '['))
    फर्काऊ सत्य;

  has_start = parse_uint (pp, end, &feature->start);

  यदि (parse_char (pp, end, ':') || parse_char (pp, end, ';')) {
    parse_uint (pp, end, &feature->end);
  } अन्यथा {
    यदि (has_start)
      feature->end = feature->start + 1;
  }

  फर्काऊ parse_char (pp, end, ']');
}

स्थिर तार्किक
parse_feature_मान_postfix (अचर वर्ण **pp, अचर वर्ण *end, hb_feature_t *feature)
{
  तार्किक had_equal = parse_char (pp, end, '=');
  तार्किक had_मान = parse_uint32 (pp, end, &feature->value) ||
		   parse_bool (pp, end, &feature->value);
  /* CSS doesn't use equal-sign between tag and value.
   * If there was an equal-sign, then there *must* be a value.
   * A value without an equal-sign is ok, but not required. */
  फर्काऊ !had_equal || had_मान;
}

स्थिर तार्किक
parse_one_feature (अचर वर्ण **pp, अचर वर्ण *end, hb_feature_t *feature)
{
  फर्काऊ parse_feature_मान_prefix (pp, end, feature) &&
	 parse_tag (pp, end, &feature->tag) &&
	 parse_feature_indices (pp, end, feature) &&
	 parse_feature_मान_postfix (pp, end, feature) &&
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
hb_feature_from_string (अचर वर्ण *str, पूर्णाङ्क len,
			hb_feature_t *feature)
{
  hb_feature_t feat;

  यदि (len < 0)
    len = म_माप (str);

  यदि (likely (parse_one_feature (&str, str + len, &feat)))
  {
    यदि (feature)
      *feature = feat;
    फर्काऊ सत्य;
  }

  यदि (feature)
    hb_memset (feature, 0, परिमाण (*feature));
  फर्काऊ असत्य;
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
शून्य
hb_feature_to_string (hb_feature_t *feature,
		      वर्ण *buf, अचिन्हित पूर्णाङ्क size)
{
  यदि (unlikely (!size)) फर्काऊ;

  वर्ण s[128];
  अचिन्हित पूर्णाङ्क len = 0;
  यदि (feature->value == 0)
    s[len++] = '-';
  hb_tag_to_string (feature->tag, s + len);
  len += 4;
  जबसम्म (len && s[len - 1] == ' ')
    len--;
  यदि (feature->start != HB_FEATURE_GLOBAL_START || feature->end != HB_FEATURE_GLOBAL_END)
  {
    s[len++] = '[';
    यदि (feature->start)
      len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%u", feature->start));
    यदि (feature->end != feature->start + 1) {
      s[len++] = ':';
      यदि (feature->end != HB_FEATURE_GLOBAL_END)
	len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%u", feature->end));
    }
    s[len++] = ']';
  }
  यदि (feature->value > 1)
  {
    s[len++] = '=';
    len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%" PRIu32, feature->value));
  }
  प्रतिज्ञा (len < ARRAY_LENGTH (s));
  len = hb_min (len, size - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/* hb_variation_t */

स्थिर तार्किक
parse_variation_मान (अचर वर्ण **pp, अचर वर्ण *end, hb_variation_t *variation)
{
  parse_char (pp, end, '='); /* Optional. */
  द्विगुण v;
  यदि (unlikely (!hb_parse_double (pp, end, &v))) फर्काऊ असत्य;

  variation->value = v;
  फर्काऊ सत्य;
}

स्थिर तार्किक
parse_one_variation (अचर वर्ण **pp, अचर वर्ण *end, hb_variation_t *variation)
{
  फर्काऊ parse_tag (pp, end, &variation->tag) &&
	 parse_variation_मान (pp, end, variation) &&
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
hb_variation_from_string (अचर वर्ण *str, पूर्णाङ्क len,
			  hb_variation_t *variation)
{
  hb_variation_t var;

  यदि (len < 0)
    len = म_माप (str);

  यदि (likely (parse_one_variation (&str, str + len, &var)))
  {
    यदि (variation)
      *variation = var;
    फर्काऊ सत्य;
  }

  यदि (variation)
    hb_memset (variation, 0, परिमाण (*variation));
  फर्काऊ असत्य;
}

#ifndef HB_NO_SETLOCALE

स्थिर अन्तःस्थ शून्य free_static_C_locale ();

स्थिर संरचना hb_C_locale_lazy_loader_t : hb_lazy_loader_t<hb_remove_pointer<hb_locale_t>,
							   hb_C_locale_lazy_loader_t>
{
  स्थिर hb_locale_t create ()
  {
    hb_locale_t l = newlocale (LC_ALL_MASK, "C", रिक्त);
    यदि (!l)
      फर्काऊ l;

    hb_atexit (free_static_C_locale);

    फर्काऊ l;
  }
  स्थिर शून्य destroy (hb_locale_t l)
  {
    freelocale (l);
  }
  स्थिर hb_locale_t get_null ()
  {
    फर्काऊ (hb_locale_t) 0;
  }
} static_C_locale;

स्थिर अन्तःस्थ
शून्य free_static_C_locale ()
{
  static_C_locale.free_instance ();
}

स्थिर hb_locale_t
get_C_locale ()
{
  फर्काऊ static_C_locale.get_unconst ();
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
शून्य
hb_variation_to_string (hb_variation_t *variation,
			वर्ण *buf, अचिन्हित पूर्णाङ्क size)
{
  यदि (unlikely (!size)) फर्काऊ;

  वर्ण s[128];
  अचिन्हित पूर्णाङ्क len = 0;
  hb_tag_to_string (variation->tag, s + len);
  len += 4;
  जबसम्म (len && s[len - 1] == ' ')
    len--;
  s[len++] = '=';

  hb_locale_t oldlocale HB_UNUSED;
  oldlocale = hb_uselocale (get_C_locale ());
  len += hb_max (0, snprintf (s + len, ARRAY_LENGTH (s) - len, "%g", (द्विगुण) variation->value));
  (शून्य) hb_uselocale (oldlocale);

  प्रतिज्ञा (len < ARRAY_LENGTH (s));
  len = hb_min (len, size - 1);
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
  फर्काऊ hb_color_get_alpha (color);
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
  फर्काऊ hb_color_get_red (color);
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
  फर्काऊ hb_color_get_green (color);
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
  फर्काऊ hb_color_get_blue (color);
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
शून्य* hb_malloc(माप_प्रकार size) { फर्काऊ hb_malloc_impl (size); }

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
शून्य* hb_calloc(माप_प्रकार nmemb, माप_प्रकार size) { फर्काऊ hb_calloc_impl (nmemb, size); }

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
शून्य* hb_realloc(शून्य *ptr, माप_प्रकार size) { फर्काऊ hb_realloc_impl (ptr, size); }

/**
 * hb_free:
 * @ptr: The pointer to the memory to free.
 *
 * Frees the memory pointed to by @ptr, using the allocator set at
 * compile-time. Typically just free().
 *
 * Since: 11.0.0
 **/
शून्य  hb_free(शून्य *ptr) { hb_free_impl (ptr); }


/* If there is no visibility control, then hb-static.cc will NOT
 * define anything.  Instead, we get it to define one set in here
 * only, so only libharfbuzz.so defines them, not other libs. */
#ifdef HB_NO_VISIBILITY
#undef HB_NO_VISIBILITY
#अन्तर्भाव "hb-static.cc"
#define HB_NO_VISIBILITY 1
#endif
