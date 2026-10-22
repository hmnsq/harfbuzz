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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


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
hb_ਟੈਗ_t
hb_ਟੈਗ_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len)
{
  ਅੱਖਰ ਟੈਗ_2[4];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

  ਜੇ (!str || !len || !*str)
    ਵਾਪਸ HB_ਟੈਗ_NONE;

  ਜੇ (len < 0 || len > 4)
    len = 4;
  ਲਈ (i = 0; i < (ਬਿਨਾਨਿਸ਼ਾਨੀ) len && str[i]; i++)
    ਟੈਗ_2[i] = str[i];
  ਲਈ (; i < 4; i++)
    ਟੈਗ_2[i] = ' ';

  ਵਾਪਸ HB_ਟੈਗ (ਟੈਗ_2[0], ਟੈਗ_2[1], ਟੈਗ_2[2], ਟੈਗ_2[3]);
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
ਖਾਲੀ
hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (hb_ਟੈਗ_t ਟੈਗ_2, ਅੱਖਰ *buf)
{
  buf[0] = (ਅੱਖਰ) (uint8_t) (ਟੈਗ_2 >> 24);
  buf[1] = (ਅੱਖਰ) (uint8_t) (ਟੈਗ_2 >> 16);
  buf[2] = (ਅੱਖਰ) (uint8_t) (ਟੈਗ_2 >>  8);
  buf[3] = (ਅੱਖਰ) (uint8_t) (ਟੈਗ_2 >>  0);
}


/* hb_direction_t */

ਸਥਿਰ ਅਚਲ ਅੱਖਰ direction_ਲੜੀਆਂ[][4] = {
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
hb_direction_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len)
{
  ਜੇ (unlikely (!str || !len || !*str))
    ਵਾਪਸ HB_DIRECTION_INVALID;

  /* Lets match loosely: just match the first letter, such that
   * all of "ltr", "left-to-right", etc work!
   */
  ਅੱਖਰ c = TOLOWER (str[0]);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਲੰਮਾਈ (direction_ਲੜੀਆਂ); i++)
    ਜੇ (c == direction_ਲੜੀਆਂ[i][0])
      ਵਾਪਸ (hb_direction_t) (HB_DIRECTION_LTR + i);

  ਵਾਪਸ HB_DIRECTION_INVALID;
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
ਅਚਲ ਅੱਖਰ *
hb_direction_ਮੰਜ਼ਿਲ_string (hb_direction_t direction)
{
  ਜੇ (likely ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (direction - HB_DIRECTION_LTR)
	      < ARRAY_ਲੰਮਾਈ (direction_ਲੜੀਆਂ)))
    ਵਾਪਸ direction_ਲੜੀਆਂ[direction - HB_DIRECTION_LTR];

  ਵਾਪਸ "invalid";
}


/* hb_language_t */

ਬਣਤਰ hb_language_impl_t {
  ਅਚਲ ਅੱਖਰ s[1];
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ canon_map[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  '-',  0,   0,
  '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9',  0,   0,   0,   0,   0,   0,
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,  '-',
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,   0
};

ਸਥਿਰ ਤਾਰਕਿਕ
lang_equal (hb_language_t  v1,
	    ਅਚਲ ਖਾਲੀ    *v2)
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p1 = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) v1;
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p2 = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) v2;

  ਜਦੋਂ (*p1 && *p1 == canon_map[*p2]) {
    p1++;
    p2++;
  }

  ਵਾਪਸ *p1 == canon_map[*p2];
}

#ਜੇ 0
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
lang_hash (ਅਚਲ ਖਾਲੀ *key)
{
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = key;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ h = 0;
  ਜਦੋਂ (canon_map[*p])
    {
      h = (h << 5) - h + canon_map[*p];
      p++;
    }

  ਵਾਪਸ h;
}
#ਜੇ_ਅੰਤ


ਬਣਤਰ hb_language_ਵਸਤੂ_t {

  ਬਣਤਰ hb_language_ਵਸਤੂ_t *next;
  hb_language_t lang;

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ ਅੱਖਰ *s) ਅਚਲ
  { ਵਾਪਸ lang_equal (lang, s); }

  hb_language_ਵਸਤੂ_t & ਸੰਚਾਲਕ = (ਅਚਲ ਅੱਖਰ *s)
  {
    /* We can't call strdup(), because we allow custom allocators. */
    size_t len = ਲੜੀ_ਲੰਬਾਈ(s) + 1;
    lang = (hb_language_t) hb_malloc(len);
    ਜੇ (likely (lang))
    {
      hb_memcpy((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) lang, s, len);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *p = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) lang; *p; p++)
	*p = canon_map[*p];
    }

    ਵਾਪਸ *ਇਹ;
  }

  ਖਾਲੀ fini () { hb_free ((ਖਾਲੀ *) lang); }
};


/* Thread-safe lockfree language list */

ਸਥਿਰ hb_atomic_t<hb_language_ਵਸਤੂ_t *> langs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_langs ()
{
retry:
  hb_language_ਵਸਤੂ_t *ਪਹਿਲਾ_lang = langs;
  ਜੇ (unlikely (!langs.cmpexch (ਪਹਿਲਾ_lang, ਖਾਲੀ_ਸੂਚਕ)))
    ਜਾਓ retry;

  ਜਦੋਂ (ਪਹਿਲਾ_lang) {
    hb_language_ਵਸਤੂ_t *next = ਪਹਿਲਾ_lang->next;
    ਪਹਿਲਾ_lang->fini ();
    hb_free (ਪਹਿਲਾ_lang);
    ਪਹਿਲਾ_lang = next;
  }
}

ਸਥਿਰ hb_language_ਵਸਤੂ_t *
lang_ਲੱਭੋ_or_ਪਾਓ (ਅਚਲ ਅੱਖਰ *key)
{
retry:
  hb_language_ਵਸਤੂ_t *ਪਹਿਲਾ_lang = langs;

  ਲਈ (hb_language_ਵਸਤੂ_t *lang = ਪਹਿਲਾ_lang; lang; lang = lang->next)
    ਜੇ (*lang == key)
      ਵਾਪਸ lang;

  /* Not found; allocate one. */
  hb_language_ਵਸਤੂ_t *lang = (hb_language_ਵਸਤੂ_t *) hb_calloc (1, ਆਕਾਰ (hb_language_ਵਸਤੂ_t));
  ਜੇ (unlikely (!lang))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  lang->next = ਪਹਿਲਾ_lang;
  *lang = key;
  ਜੇ (unlikely (!lang->lang))
  {
    hb_free (lang);
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਜੇ (unlikely (!langs.cmpexch (ਪਹਿਲਾ_lang, lang)))
  {
    lang->fini ();
    hb_free (lang);
    ਜਾਓ retry;
  }

  ਜੇ (!ਪਹਿਲਾ_lang)
    hb_atexit (free_langs); /* First person registers atexit() callback. */

  ਵਾਪਸ lang;
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
hb_language_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len)
{
  ਜੇ (!str || !len || !*str)
    ਵਾਪਸ HB_LANGUAGE_INVALID;

  hb_language_ਵਸਤੂ_t *ਵਸਤੂ = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (len >= 0)
  {
    /* NUL-terminate it. */
    ਅੱਖਰ strbuf[64];
    len = hb_min (len, (ਪੂਰਨ) ਆਕਾਰ (strbuf) - 1);
    hb_memcpy (strbuf, str, len);
    strbuf[len] = '\0';
    ਵਸਤੂ = lang_ਲੱਭੋ_or_ਪਾਓ (strbuf);
  }
  ਨਹੀਂਤਾਂ
    ਵਸਤੂ = lang_ਲੱਭੋ_or_ਪਾਓ (str);

  ਵਾਪਸ likely (ਵਸਤੂ) ? ਵਸਤੂ->lang : HB_LANGUAGE_INVALID;
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
ਅਚਲ ਅੱਖਰ *
hb_language_ਮੰਜ਼ਿਲ_string (hb_language_t language)
{
  ਜੇ (unlikely (!language)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਵਾਪਸ language->s;
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
  ਸਥਿਰ hb_atomic_t<hb_language_t> default_language;

  hb_language_t language = default_language;
  ਜੇ (unlikely (language == HB_LANGUAGE_INVALID))
  {
    language = hb_language_from_string (hb_setlocale (LC_CTYPE, ਖਾਲੀ_ਸੂਚਕ), -1);
    (ਖਾਲੀ) default_language.cmpexch (HB_LANGUAGE_INVALID, language);
  }

  ਵਾਪਸ language;
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
  ਜੇ (language == specific) ਵਾਪਸ ਸੱਚ;
  ਜੇ (!language || !specific) ਵਾਪਸ ਝੂਠ;

  ਅਚਲ ਅੱਖਰ *l = language->s;
  ਅਚਲ ਅੱਖਰ *s = specific->s;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ll = ਲੜੀ_ਲੰਬਾਈ (l);
  ਬਿਨਾਨਿਸ਼ਾਨੀ sl = ਲੜੀ_ਲੰਬਾਈ (s);

  ਜੇ (ll > sl)
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ ਲੜੀ_ਤੁਲਨਾ (l, s, ll) == 0 &&
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
hb_script_from_iso15924_ਟੈਗ (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਜੇ (unlikely (ਟੈਗ_2 == HB_ਟੈਗ_NONE))
    ਵਾਪਸ HB_SCRIPT_INVALID;

  /* Be lenient, adjust case (one capital letter followed by three small letters) */
  ਟੈਗ_2 = (ਟੈਗ_2 & 0xDFDFDFDFu) | 0x00202020u;

  ਚੋਣ (ਟੈਗ_2)
  {
    /* Script variants from https://unicode.org/iso15924/ */
    ਹਾਲਤ HB_ਟੈਗ('A','r','a','n'): ਵਾਪਸ HB_SCRIPT_ARABIC;
    ਹਾਲਤ HB_ਟੈਗ('C','y','r','s'): ਵਾਪਸ HB_SCRIPT_CYRILLIC;
    ਹਾਲਤ HB_ਟੈਗ('G','e','o','k'): ਵਾਪਸ HB_SCRIPT_GEORGIAN;
    ਹਾਲਤ HB_ਟੈਗ('H','a','n','s'): ਵਾਪਸ HB_SCRIPT_HAN;
    ਹਾਲਤ HB_ਟੈਗ('H','a','n','t'): ਵਾਪਸ HB_SCRIPT_HAN;
    ਹਾਲਤ HB_ਟੈਗ('J','a','m','o'): ਵਾਪਸ HB_SCRIPT_HANGUL;
    ਹਾਲਤ HB_ਟੈਗ('L','a','t','f'): ਵਾਪਸ HB_SCRIPT_LATIN;
    ਹਾਲਤ HB_ਟੈਗ('L','a','t','g'): ਵਾਪਸ HB_SCRIPT_LATIN;
    ਹਾਲਤ HB_ਟੈਗ('S','y','r','e'): ਵਾਪਸ HB_SCRIPT_SYRIAC;
    ਹਾਲਤ HB_ਟੈਗ('S','y','r','j'): ਵਾਪਸ HB_SCRIPT_SYRIAC;
    ਹਾਲਤ HB_ਟੈਗ('S','y','r','n'): ਵਾਪਸ HB_SCRIPT_SYRIAC;
  }

  /* If it looks right, just use the tag as a script */
  ਜੇ (((uint32_t) ਟੈਗ_2 & 0xE0E0E0E0u) == 0x40606060u)
    ਵਾਪਸ (hb_script_t) ਟੈਗ_2;

  /* Otherwise, return unknown */
  ਵਾਪਸ HB_SCRIPT_UNKNOWN;
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
hb_script_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len)
{
  ਵਾਪਸ hb_script_from_iso15924_ਟੈਗ (hb_ਟੈਗ_from_string (str, len));
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
hb_ਟੈਗ_t
hb_script_ਮੰਜ਼ਿਲ_iso15924_ਟੈਗ (hb_script_t script)
{
  ਵਾਪਸ (hb_ਟੈਗ_t) script;
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
  ਚੋਣ ((hb_ਟੈਗ_t) script)
  {
    /* Unicode-1.1 additions */
    ਹਾਲਤ HB_SCRIPT_ARABIC:
    ਹਾਲਤ HB_SCRIPT_HEBREW:

    /* Unicode-3.0 additions */
    ਹਾਲਤ HB_SCRIPT_SYRIAC:
    ਹਾਲਤ HB_SCRIPT_THAANA:

    /* Unicode-4.0 additions */
    ਹਾਲਤ HB_SCRIPT_CYPRIOT:

    /* Unicode-4.1 additions */
    ਹਾਲਤ HB_SCRIPT_KHAROSHTHI:

    /* Unicode-5.0 additions */
    ਹਾਲਤ HB_SCRIPT_PHOENICIAN:
    ਹਾਲਤ HB_SCRIPT_NKO:

    /* Unicode-5.1 additions */
    ਹਾਲਤ HB_SCRIPT_LYDIAN:

    /* Unicode-5.2 additions */
    ਹਾਲਤ HB_SCRIPT_AVESTAN:
    ਹਾਲਤ HB_SCRIPT_IMPERIAL_ARAMAIC:
    ਹਾਲਤ HB_SCRIPT_INSCRIPTIONAL_PAHLAVI:
    ਹਾਲਤ HB_SCRIPT_INSCRIPTIONAL_PARTHIAN:
    ਹਾਲਤ HB_SCRIPT_OLD_SOUTH_ARABIAN:
    ਹਾਲਤ HB_SCRIPT_OLD_TURKIC:
    ਹਾਲਤ HB_SCRIPT_SAMARITAN:

    /* Unicode-6.0 additions */
    ਹਾਲਤ HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    ਹਾਲਤ HB_SCRIPT_MEROITIC_CURSIVE:
    ਹਾਲਤ HB_SCRIPT_MEROITIC_HIEROGLYPHS:

    /* Unicode-7.0 additions */
    ਹਾਲਤ HB_SCRIPT_MANICHAEAN:
    ਹਾਲਤ HB_SCRIPT_MENDE_KIKAKUI:
    ਹਾਲਤ HB_SCRIPT_NABATAEAN:
    ਹਾਲਤ HB_SCRIPT_OLD_NORTH_ARABIAN:
    ਹਾਲਤ HB_SCRIPT_PALMYRENE:
    ਹਾਲਤ HB_SCRIPT_PSALTER_PAHLAVI:

    /* Unicode-8.0 additions */
    ਹਾਲਤ HB_SCRIPT_HATRAN:

    /* Unicode-9.0 additions */
    ਹਾਲਤ HB_SCRIPT_ADLAM:

    /* Unicode-11.0 additions */
    ਹਾਲਤ HB_SCRIPT_HANIFI_ROHINGYA:
    ਹਾਲਤ HB_SCRIPT_OLD_SOGDIAN:
    ਹਾਲਤ HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    ਹਾਲਤ HB_SCRIPT_ELYMAIC:

    /* Unicode-13.0 additions */
    ਹਾਲਤ HB_SCRIPT_CHORASMIAN:
    ਹਾਲਤ HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    ਹਾਲਤ HB_SCRIPT_OLD_UYGHUR:

    /* Unicode-16.0 additions */
    ਹਾਲਤ HB_SCRIPT_GARAY:

    /* Unicode-17.0 additions */
    ਹਾਲਤ HB_SCRIPT_SIDETIC:

      ਵਾਪਸ HB_DIRECTION_RTL;


    /* https://github.com/harfbuzz/harfbuzz/issues/1000 */
    ਹਾਲਤ HB_SCRIPT_OLD_HUNGARIAN:
    ਹਾਲਤ HB_SCRIPT_OLD_ITALIC:
    ਹਾਲਤ HB_SCRIPT_RUNIC:
    ਹਾਲਤ HB_SCRIPT_TIFINAGH:

      ਵਾਪਸ HB_DIRECTION_INVALID;
  }

  ਵਾਪਸ HB_DIRECTION_LTR;
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
ਖਾਲੀ
hb_ਸੰਸਕਰਣ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *major,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *minor,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *micro)
{
  *major = HB_ਸੰਸਕਰਣ_MAJOR;
  *minor = HB_ਸੰਸਕਰਣ_MINOR;
  *micro = HB_ਸੰਸਕਰਣ_MICRO;
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
ਅਚਲ ਅੱਖਰ *
hb_ਸੰਸਕਰਣ_string ()
{
  ਵਾਪਸ HB_ਸੰਸਕਰਣ_STRING;
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
hb_ਸੰਸਕਰਣ_atleast (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ major,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ minor,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ micro)
{
  ਵਾਪਸ HB_ਸੰਸਕਰਣ_ATLEAST (major, minor, micro);
}



/* hb_feature_t and hb_variation_t */

ਸਥਿਰ ਤਾਰਕਿਕ
parse_space (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ)
{
  ਜਦੋਂ (*pp < ਸਿਰਾ && ISSPACE (**pp))
    (*pp)++;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_char (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, ਅੱਖਰ c)
{
  parse_space (pp, ਸਿਰਾ);

  ਜੇ (*pp == ਸਿਰਾ || **pp != c)
    ਵਾਪਸ ਝੂਠ;

  (*pp)++;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_uint (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  ਪੂਰਨ v;
  ਜੇ (unlikely (!hb_parse_int (pp, ਸਿਰਾ, &v))) ਵਾਪਸ ਝੂਠ;

  *pv = v;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_uint32 (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, uint32_t *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  ਪੂਰਨ v;
  ਜੇ (unlikely (!hb_parse_int (pp, ਸਿਰਾ, &v))) ਵਾਪਸ ਝੂਠ;

  *pv = v;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_bool (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, uint32_t *pv)
{
  parse_space (pp, ਸਿਰਾ);

  ਅਚਲ ਅੱਖਰ *p = *pp;
  ਜਦੋਂ (*pp < ਸਿਰਾ && ISALPHA(**pp))
    (*pp)++;

  /* CSS allows on/off as aliases 1/0. */
  ਜੇ (*pp - p == 2
      && TOLOWER (p[0]) == 'o'
      && TOLOWER (p[1]) == 'n')
    *pv = 1;
  ਨਹੀਂਤਾਂ ਜੇ (*pp - p == 3
	   && TOLOWER (p[0]) == 'o'
	   && TOLOWER (p[1]) == 'f'
	   && TOLOWER (p[2]) == 'f')
    *pv = 0;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ ਸੱਚ;
}

/* hb_feature_t */

ਸਥਿਰ ਤਾਰਕਿਕ
parse_feature_ਮੁੱਲ_ਅਗੇਤਰ (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_feature_t *feature)
{
  ਜੇ (parse_char (pp, ਸਿਰਾ, '-'))
    feature->value = 0;
  ਨਹੀਂਤਾਂ {
    parse_char (pp, ਸਿਰਾ, '+');
    feature->value = 1;
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_ਟੈਗ (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_ਟੈਗ_t *ਟੈਗ_2)
{
  parse_space (pp, ਸਿਰਾ);

  ਅੱਖਰ quote = 0;

  ਜੇ (*pp < ਸਿਰਾ && (**pp == '\'' || **pp == '"'))
  {
    quote = **pp;
    (*pp)++;
  }

  ਅਚਲ ਅੱਖਰ *p = *pp;
  ਜਦੋਂ (*pp < ਸਿਰਾ && (**pp != ' ' && **pp != '=' && **pp != '[' && **pp != quote))
    (*pp)++;

  ਜੇ (p == *pp || *pp - p > 4)
    ਵਾਪਸ ਝੂਠ;

  *ਟੈਗ_2 = hb_ਟੈਗ_from_string (p, *pp - p);

  ਜੇ (quote)
  {
    /* CSS expects exactly four bytes.  And we only allow quotations for
     * CSS compatibility.  So, enforce the length. */
     ਜੇ (*pp - p != 4)
       ਵਾਪਸ ਝੂਠ;
    ਜੇ (*pp == ਸਿਰਾ || **pp != quote)
      ਵਾਪਸ ਝੂਠ;
    (*pp)++;
  }

  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_feature_indices (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_feature_t *feature)
{
  parse_space (pp, ਸਿਰਾ);

  ਤਾਰਕਿਕ has_ਸ਼ੁਰੂ;

  feature->ਸ਼ੁਰੂ = HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ;
  feature->ਸਿਰਾ = HB_FEATURE_GLOBAL_ਸਿਰਾ;

  ਜੇ (!parse_char (pp, ਸਿਰਾ, '['))
    ਵਾਪਸ ਸੱਚ;

  has_ਸ਼ੁਰੂ = parse_uint (pp, ਸਿਰਾ, &feature->ਸ਼ੁਰੂ);

  ਜੇ (parse_char (pp, ਸਿਰਾ, ':') || parse_char (pp, ਸਿਰਾ, ';')) {
    parse_uint (pp, ਸਿਰਾ, &feature->ਸਿਰਾ);
  } ਨਹੀਂਤਾਂ {
    ਜੇ (has_ਸ਼ੁਰੂ)
      feature->ਸਿਰਾ = feature->ਸ਼ੁਰੂ + 1;
  }

  ਵਾਪਸ parse_char (pp, ਸਿਰਾ, ']');
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_feature_ਮੁੱਲ_postfix (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_feature_t *feature)
{
  ਤਾਰਕਿਕ had_equal = parse_char (pp, ਸਿਰਾ, '=');
  ਤਾਰਕਿਕ had_ਮੁੱਲ = parse_uint32 (pp, ਸਿਰਾ, &feature->value) ||
		   parse_bool (pp, ਸਿਰਾ, &feature->value);
  /* CSS doesn't use equal-sign between tag and value.
   * If there was an equal-sign, then there *must* be a value.
   * A value without an equal-sign is ok, but not required. */
  ਵਾਪਸ !had_equal || had_ਮੁੱਲ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_one_feature (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_feature_t *feature)
{
  ਵਾਪਸ parse_feature_ਮੁੱਲ_ਅਗੇਤਰ (pp, ਸਿਰਾ, feature) &&
	 parse_ਟੈਗ (pp, ਸਿਰਾ, &feature->ਟੈਗ_2) &&
	 parse_feature_indices (pp, ਸਿਰਾ, feature) &&
	 parse_feature_ਮੁੱਲ_postfix (pp, ਸਿਰਾ, feature) &&
	 parse_space (pp, ਸਿਰਾ) &&
	 *pp == ਸਿਰਾ;
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
hb_feature_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len,
			hb_feature_t *feature)
{
  hb_feature_t feat;

  ਜੇ (len < 0)
    len = ਲੜੀ_ਲੰਬਾਈ (str);

  ਜੇ (likely (parse_one_feature (&str, str + len, &feat)))
  {
    ਜੇ (feature)
      *feature = feat;
    ਵਾਪਸ ਸੱਚ;
  }

  ਜੇ (feature)
    hb_memset (feature, 0, ਆਕਾਰ (*feature));
  ਵਾਪਸ ਝੂਠ;
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
ਖਾਲੀ
hb_feature_ਮੰਜ਼ਿਲ_string (hb_feature_t *feature,
		      ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
  ਜੇ (unlikely (!ਮਾਪ)) ਵਾਪਸ;

  ਅੱਖਰ s[128];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = 0;
  ਜੇ (feature->value == 0)
    s[len++] = '-';
  hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (feature->ਟੈਗ_2, s + len);
  len += 4;
  ਜਦੋਂ (len && s[len - 1] == ' ')
    len--;
  ਜੇ (feature->ਸ਼ੁਰੂ != HB_FEATURE_GLOBAL_ਸ਼ੁਰੂ || feature->ਸਿਰਾ != HB_FEATURE_GLOBAL_ਸਿਰਾ)
  {
    s[len++] = '[';
    ਜੇ (feature->ਸ਼ੁਰੂ)
      len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%u", feature->ਸ਼ੁਰੂ));
    ਜੇ (feature->ਸਿਰਾ != feature->ਸ਼ੁਰੂ + 1) {
      s[len++] = ':';
      ਜੇ (feature->ਸਿਰਾ != HB_FEATURE_GLOBAL_ਸਿਰਾ)
	len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%u", feature->ਸਿਰਾ));
    }
    s[len++] = ']';
  }
  ਜੇ (feature->value > 1)
  {
    s[len++] = '=';
    len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%" PRIu32, feature->value));
  }
  ਪ੍ਰਤਿਜ੍ਞਾ (len < ARRAY_ਲੰਮਾਈ (s));
  len = hb_min (len, ਮਾਪ - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/* hb_variation_t */

ਸਥਿਰ ਤਾਰਕਿਕ
parse_variation_ਮੁੱਲ (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_variation_t *variation)
{
  parse_char (pp, ਸਿਰਾ, '='); /* Optional. */
  ਦੁੱਗਣਾ v;
  ਜੇ (unlikely (!hb_parse_double (pp, ਸਿਰਾ, &v))) ਵਾਪਸ ਝੂਠ;

  variation->value = v;
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_one_variation (ਅਚਲ ਅੱਖਰ **pp, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ, hb_variation_t *variation)
{
  ਵਾਪਸ parse_ਟੈਗ (pp, ਸਿਰਾ, &variation->ਟੈਗ_2) &&
	 parse_variation_ਮੁੱਲ (pp, ਸਿਰਾ, variation) &&
	 parse_space (pp, ਸਿਰਾ) &&
	 *pp == ਸਿਰਾ;
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
hb_variation_from_string (ਅਚਲ ਅੱਖਰ *str, ਪੂਰਨ len,
			  hb_variation_t *variation)
{
  hb_variation_t var;

  ਜੇ (len < 0)
    len = ਲੜੀ_ਲੰਬਾਈ (str);

  ਜੇ (likely (parse_one_variation (&str, str + len, &var)))
  {
    ਜੇ (variation)
      *variation = var;
    ਵਾਪਸ ਸੱਚ;
  }

  ਜੇ (variation)
    hb_memset (variation, 0, ਆਕਾਰ (*variation));
  ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SETLOCALE

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_C_locale ();

ਸਥਿਰ ਬਣਤਰ hb_C_locale_lazy_loader_t : hb_lazy_loader_t<hb_remove_ਸੂਚਕ<hb_locale_t>,
							   hb_C_locale_lazy_loader_t>
{
  ਸਥਿਰ hb_locale_t create ()
  {
    hb_locale_t l = newlocale (LC_ALL_MASK, "C", ਨਿਰਾ);
    ਜੇ (!l)
      ਵਾਪਸ l;

    hb_atexit (free_static_C_locale);

    ਵਾਪਸ l;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_locale_t l)
  {
    freelocale (l);
  }
  ਸਥਿਰ hb_locale_t get_null ()
  {
    ਵਾਪਸ (hb_locale_t) 0;
  }
} static_C_locale;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_C_locale ()
{
  static_C_locale.free_instance ();
}

ਸਥਿਰ hb_locale_t
get_C_locale ()
{
  ਵਾਪਸ static_C_locale.get_unconst ();
}

#ਜੇ_ਅੰਤ

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
ਖਾਲੀ
hb_variation_ਮੰਜ਼ਿਲ_string (hb_variation_t *variation,
			ਅੱਖਰ *buf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
{
  ਜੇ (unlikely (!ਮਾਪ)) ਵਾਪਸ;

  ਅੱਖਰ s[128];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len = 0;
  hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (variation->ਟੈਗ_2, s + len);
  len += 4;
  ਜਦੋਂ (len && s[len - 1] == ' ')
    len--;
  s[len++] = '=';

  hb_locale_t oldlocale HB_ਅਣਵਰਤਿਆ;
  oldlocale = hb_uselocale (get_C_locale ());
  len += hb_max (0, snprintf (s + len, ARRAY_ਲੰਮਾਈ (s) - len, "%g", (ਦੁੱਗਣਾ) variation->value));
  (ਖਾਲੀ) hb_uselocale (oldlocale);

  ਪ੍ਰਤਿਜ੍ਞਾ (len < ARRAY_ਲੰਮਾਈ (s));
  len = hb_min (len, ਮਾਪ - 1);
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
  ਵਾਪਸ hb_color_get_alpha (color);
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
  ਵਾਪਸ hb_color_get_red (color);
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
  ਵਾਪਸ hb_color_get_green (color);
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
  ਵਾਪਸ hb_color_get_blue (color);
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
ਖਾਲੀ* hb_malloc(size_t ਮਾਪ) { ਵਾਪਸ hb_malloc_impl (ਮਾਪ); }

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
ਖਾਲੀ* hb_calloc(size_t nmemb, size_t ਮਾਪ) { ਵਾਪਸ hb_calloc_impl (nmemb, ਮਾਪ); }

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
ਖਾਲੀ* hb_realloc(ਖਾਲੀ *ptr, size_t ਮਾਪ) { ਵਾਪਸ hb_realloc_impl (ptr, ਮਾਪ); }

/**
 * hb_free:
 * @ptr: The pointer to the memory to free.
 *
 * Frees the memory pointed to by @ptr, using the allocator set at
 * compile-time. Typically just free().
 *
 * Since: 11.0.0
 **/
ਖਾਲੀ  hb_free(ਖਾਲੀ *ptr) { hb_free_impl (ptr); }


/* If there is no visibility control, then hb-static.cc will NOT
 * define anything.  Instead, we get it to define one set in here
 * only, so only libharfbuzz.so defines them, not other libs. */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_VISIBILITY
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_NO_VISIBILITY
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-static.cc"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_NO_VISIBILITY 1
#ਜੇ_ਅੰਤ
