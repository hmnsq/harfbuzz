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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-machinery.hh"


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
hb_ట్యాగ్_t
hb_ట్యాగ్_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len)
{
  అక్షరం ట్యాగ్_2[4];
  గుర్తులేని పూర్ణాంకం i;

  ఒకవేళ (!str || !len || !*str)
    తిరిగి HB_ట్యాగ్_NONE;

  ఒకవేళ (len < 0 || len > 4)
    len = 4;
  కొరకు (i = 0; i < (గుర్తులేని) len && str[i]; i++)
    ట్యాగ్_2[i] = str[i];
  కొరకు (; i < 4; i++)
    ట్యాగ్_2[i] = ' ';

  తిరిగి HB_ట్యాగ్ (ట్యాగ్_2[0], ట్యాగ్_2[1], ట్యాగ్_2[2], ట్యాగ్_2[3]);
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
శూన్య
hb_ట్యాగ్_గమ్యం_string (hb_ట్యాగ్_t ట్యాగ్_2, అక్షరం *buf)
{
  buf[0] = (అక్షరం) (uint8_t) (ట్యాగ్_2 >> 24);
  buf[1] = (అక్షరం) (uint8_t) (ట్యాగ్_2 >> 16);
  buf[2] = (అక్షరం) (uint8_t) (ట్యాగ్_2 >>  8);
  buf[3] = (అక్షరం) (uint8_t) (ట్యాగ్_2 >>  0);
}


/* hb_direction_t */

స్థిర స్థిరరాశి అక్షరం direction_అక్షరశ్రేణులు[][4] = {
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
hb_direction_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len)
{
  ఒకవేళ (unlikely (!str || !len || !*str))
    తిరిగి HB_DIRECTION_INVALID;

  /* Lets match loosely: just match the first letter, such that
   * all of "ltr", "left-to-right", etc work!
   */
  అక్షరం c = TOLOWER (str[0]);
  కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (direction_అక్షరశ్రేణులు); i++)
    ఒకవేళ (c == direction_అక్షరశ్రేణులు[i][0])
      తిరిగి (hb_direction_t) (HB_DIRECTION_LTR + i);

  తిరిగి HB_DIRECTION_INVALID;
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
స్థిరరాశి అక్షరం *
hb_direction_గమ్యం_string (hb_direction_t direction)
{
  ఒకవేళ (likely ((గుర్తులేని పూర్ణాంకం) (direction - HB_DIRECTION_LTR)
	      < ARRAY_నిడివి (direction_అక్షరశ్రేణులు)))
    తిరిగి direction_అక్షరశ్రేణులు[direction - HB_DIRECTION_LTR];

  తిరిగి "invalid";
}


/* hb_language_t */

నిర్మాణం hb_language_impl_t {
  స్థిరరాశి అక్షరం s[1];
};

స్థిర స్థిరరాశి అక్షరం canon_map[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,   0,   0,  '-',  0,   0,
  '0', '1', '2', '3', '4', '5', '6', '7',  '8', '9',  0,   0,   0,   0,   0,   0,
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,  '-',
   0,  'a', 'b', 'c', 'd', 'e', 'f', 'g',  'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w',  'x', 'y', 'z',  0,   0,   0,   0,   0
};

స్థిర తార్కిక
lang_equal (hb_language_t  v1,
	    స్థిరరాశి శూన్య    *v2)
{
  స్థిరరాశి గుర్తులేని అక్షరం *p1 = (స్థిరరాశి గుర్తులేని అక్షరం *) v1;
  స్థిరరాశి గుర్తులేని అక్షరం *p2 = (స్థిరరాశి గుర్తులేని అక్షరం *) v2;

  అయితే (*p1 && *p1 == canon_map[*p2]) {
    p1++;
    p2++;
  }

  తిరిగి *p1 == canon_map[*p2];
}

#ఒకవేళ 0
స్థిర గుర్తులేని పూర్ణాంకం
lang_hash (స్థిరరాశి శూన్య *key)
{
  స్థిరరాశి గుర్తులేని అక్షరం *p = key;
  గుర్తులేని పూర్ణాంకం h = 0;
  అయితే (canon_map[*p])
    {
      h = (h << 5) - h + canon_map[*p];
      p++;
    }

  తిరిగి h;
}
#అయితే_ముగింపు


నిర్మాణం hb_language_అంశం_t {

  నిర్మాణం hb_language_అంశం_t *next;
  hb_language_t lang;

  తార్కిక క్రియాకారకం == (స్థిరరాశి అక్షరం *s) స్థిరరాశి
  { తిరిగి lang_equal (lang, s); }

  hb_language_అంశం_t & క్రియాకారకం = (స్థిరరాశి అక్షరం *s)
  {
    /* We can't call strdup(), because we allow custom allocators. */
    size_t len = అక్షరశ్రేణి_పొడవు(s) + 1;
    lang = (hb_language_t) hb_malloc(len);
    ఒకవేళ (likely (lang))
    {
      hb_memcpy((గుర్తులేని అక్షరం *) lang, s, len);
      కొరకు (గుర్తులేని అక్షరం *p = (గుర్తులేని అక్షరం *) lang; *p; p++)
	*p = canon_map[*p];
    }

    తిరిగి *ఇది;
  }

  శూన్య fini () { hb_free ((శూన్య *) lang); }
};


/* Thread-safe lockfree language list */

స్థిర hb_atomic_t<hb_language_అంశం_t *> langs;

స్థిర అంతఃవరుస శూన్య
free_langs ()
{
retry:
  hb_language_అంశం_t *మొదటి_lang = langs;
  ఒకవేళ (unlikely (!langs.cmpexch (మొదటి_lang, శూన్యసూచిక)))
    వెళ్ళు retry;

  అయితే (మొదటి_lang) {
    hb_language_అంశం_t *next = మొదటి_lang->next;
    మొదటి_lang->fini ();
    hb_free (మొదటి_lang);
    మొదటి_lang = next;
  }
}

స్థిర hb_language_అంశం_t *
lang_కనుగొను_or_చొప్పించు (స్థిరరాశి అక్షరం *key)
{
retry:
  hb_language_అంశం_t *మొదటి_lang = langs;

  కొరకు (hb_language_అంశం_t *lang = మొదటి_lang; lang; lang = lang->next)
    ఒకవేళ (*lang == key)
      తిరిగి lang;

  /* Not found; allocate one. */
  hb_language_అంశం_t *lang = (hb_language_అంశం_t *) hb_calloc (1, పరిమాణం (hb_language_అంశం_t));
  ఒకవేళ (unlikely (!lang))
    తిరిగి శూన్యసూచిక;
  lang->next = మొదటి_lang;
  *lang = key;
  ఒకవేళ (unlikely (!lang->lang))
  {
    hb_free (lang);
    తిరిగి శూన్యసూచిక;
  }

  ఒకవేళ (unlikely (!langs.cmpexch (మొదటి_lang, lang)))
  {
    lang->fini ();
    hb_free (lang);
    వెళ్ళు retry;
  }

  ఒకవేళ (!మొదటి_lang)
    hb_atexit (free_langs); /* First person registers atexit() callback. */

  తిరిగి lang;
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
hb_language_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len)
{
  ఒకవేళ (!str || !len || !*str)
    తిరిగి HB_LANGUAGE_INVALID;

  hb_language_అంశం_t *అంశం = శూన్యసూచిక;
  ఒకవేళ (len >= 0)
  {
    /* NUL-terminate it. */
    అక్షరం strbuf[64];
    len = hb_min (len, (పూర్ణాంకం) పరిమాణం (strbuf) - 1);
    hb_memcpy (strbuf, str, len);
    strbuf[len] = '\0';
    అంశం = lang_కనుగొను_or_చొప్పించు (strbuf);
  }
  లేకపోతే
    అంశం = lang_కనుగొను_or_చొప్పించు (str);

  తిరిగి likely (అంశం) ? అంశం->lang : HB_LANGUAGE_INVALID;
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
స్థిరరాశి అక్షరం *
hb_language_గమ్యం_string (hb_language_t language)
{
  ఒకవేళ (unlikely (!language)) తిరిగి శూన్యసూచిక;

  తిరిగి language->s;
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
  స్థిర hb_atomic_t<hb_language_t> default_language;

  hb_language_t language = default_language;
  ఒకవేళ (unlikely (language == HB_LANGUAGE_INVALID))
  {
    language = hb_language_from_string (hb_setlocale (LC_CTYPE, శూన్యసూచిక), -1);
    (శూన్య) default_language.cmpexch (HB_LANGUAGE_INVALID, language);
  }

  తిరిగి language;
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
  ఒకవేళ (language == specific) తిరిగి నిజం;
  ఒకవేళ (!language || !specific) తిరిగి అబద్ధం;

  స్థిరరాశి అక్షరం *l = language->s;
  స్థిరరాశి అక్షరం *s = specific->s;
  గుర్తులేని ll = అక్షరశ్రేణి_పొడవు (l);
  గుర్తులేని sl = అక్షరశ్రేణి_పొడవు (s);

  ఒకవేళ (ll > sl)
    తిరిగి అబద్ధం;

  తిరిగి అక్షరశ్రేణి_పోలిక (l, s, ll) == 0 &&
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
hb_script_from_iso15924_ట్యాగ్ (hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఒకవేళ (unlikely (ట్యాగ్_2 == HB_ట్యాగ్_NONE))
    తిరిగి HB_SCRIPT_INVALID;

  /* Be lenient, adjust case (one capital letter followed by three small letters) */
  ట్యాగ్_2 = (ట్యాగ్_2 & 0xDFDFDFDFu) | 0x00202020u;

  ఎంపిక (ట్యాగ్_2)
  {
    /* Script variants from https://unicode.org/iso15924/ */
    స్థితి HB_ట్యాగ్('A','r','a','n'): తిరిగి HB_SCRIPT_ARABIC;
    స్థితి HB_ట్యాగ్('C','y','r','s'): తిరిగి HB_SCRIPT_CYRILLIC;
    స్థితి HB_ట్యాగ్('G','e','o','k'): తిరిగి HB_SCRIPT_GEORGIAN;
    స్థితి HB_ట్యాగ్('H','a','n','s'): తిరిగి HB_SCRIPT_HAN;
    స్థితి HB_ట్యాగ్('H','a','n','t'): తిరిగి HB_SCRIPT_HAN;
    స్థితి HB_ట్యాగ్('J','a','m','o'): తిరిగి HB_SCRIPT_HANGUL;
    స్థితి HB_ట్యాగ్('L','a','t','f'): తిరిగి HB_SCRIPT_LATIN;
    స్థితి HB_ట్యాగ్('L','a','t','g'): తిరిగి HB_SCRIPT_LATIN;
    స్థితి HB_ట్యాగ్('S','y','r','e'): తిరిగి HB_SCRIPT_SYRIAC;
    స్థితి HB_ట్యాగ్('S','y','r','j'): తిరిగి HB_SCRIPT_SYRIAC;
    స్థితి HB_ట్యాగ్('S','y','r','n'): తిరిగి HB_SCRIPT_SYRIAC;
  }

  /* If it looks right, just use the tag as a script */
  ఒకవేళ (((uint32_t) ట్యాగ్_2 & 0xE0E0E0E0u) == 0x40606060u)
    తిరిగి (hb_script_t) ట్యాగ్_2;

  /* Otherwise, return unknown */
  తిరిగి HB_SCRIPT_UNKNOWN;
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
hb_script_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len)
{
  తిరిగి hb_script_from_iso15924_ట్యాగ్ (hb_ట్యాగ్_from_string (str, len));
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
hb_ట్యాగ్_t
hb_script_గమ్యం_iso15924_ట్యాగ్ (hb_script_t script)
{
  తిరిగి (hb_ట్యాగ్_t) script;
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
  ఎంపిక ((hb_ట్యాగ్_t) script)
  {
    /* Unicode-1.1 additions */
    స్థితి HB_SCRIPT_ARABIC:
    స్థితి HB_SCRIPT_HEBREW:

    /* Unicode-3.0 additions */
    స్థితి HB_SCRIPT_SYRIAC:
    స్థితి HB_SCRIPT_THAANA:

    /* Unicode-4.0 additions */
    స్థితి HB_SCRIPT_CYPRIOT:

    /* Unicode-4.1 additions */
    స్థితి HB_SCRIPT_KHAROSHTHI:

    /* Unicode-5.0 additions */
    స్థితి HB_SCRIPT_PHOENICIAN:
    స్థితి HB_SCRIPT_NKO:

    /* Unicode-5.1 additions */
    స్థితి HB_SCRIPT_LYDIAN:

    /* Unicode-5.2 additions */
    స్థితి HB_SCRIPT_AVESTAN:
    స్థితి HB_SCRIPT_IMPERIAL_ARAMAIC:
    స్థితి HB_SCRIPT_INSCRIPTIONAL_PAHLAVI:
    స్థితి HB_SCRIPT_INSCRIPTIONAL_PARTHIAN:
    స్థితి HB_SCRIPT_OLD_SOUTH_ARABIAN:
    స్థితి HB_SCRIPT_OLD_TURKIC:
    స్థితి HB_SCRIPT_SAMARITAN:

    /* Unicode-6.0 additions */
    స్థితి HB_SCRIPT_MANDAIC:

    /* Unicode-6.1 additions */
    స్థితి HB_SCRIPT_MEROITIC_CURSIVE:
    స్థితి HB_SCRIPT_MEROITIC_HIEROGLYPHS:

    /* Unicode-7.0 additions */
    స్థితి HB_SCRIPT_MANICHAEAN:
    స్థితి HB_SCRIPT_MENDE_KIKAKUI:
    స్థితి HB_SCRIPT_NABATAEAN:
    స్థితి HB_SCRIPT_OLD_NORTH_ARABIAN:
    స్థితి HB_SCRIPT_PALMYRENE:
    స్థితి HB_SCRIPT_PSALTER_PAHLAVI:

    /* Unicode-8.0 additions */
    స్థితి HB_SCRIPT_HATRAN:

    /* Unicode-9.0 additions */
    స్థితి HB_SCRIPT_ADLAM:

    /* Unicode-11.0 additions */
    స్థితి HB_SCRIPT_HANIFI_ROHINGYA:
    స్థితి HB_SCRIPT_OLD_SOGDIAN:
    స్థితి HB_SCRIPT_SOGDIAN:

    /* Unicode-12.0 additions */
    స్థితి HB_SCRIPT_ELYMAIC:

    /* Unicode-13.0 additions */
    స్థితి HB_SCRIPT_CHORASMIAN:
    స్థితి HB_SCRIPT_YEZIDI:

    /* Unicode-14.0 additions */
    స్థితి HB_SCRIPT_OLD_UYGHUR:

    /* Unicode-16.0 additions */
    స్థితి HB_SCRIPT_GARAY:

    /* Unicode-17.0 additions */
    స్థితి HB_SCRIPT_SIDETIC:

      తిరిగి HB_DIRECTION_RTL;


    /* https://github.com/harfbuzz/harfbuzz/issues/1000 */
    స్థితి HB_SCRIPT_OLD_HUNGARIAN:
    స్థితి HB_SCRIPT_OLD_ITALIC:
    స్థితి HB_SCRIPT_RUNIC:
    స్థితి HB_SCRIPT_TIFINAGH:

      తిరిగి HB_DIRECTION_INVALID;
  }

  తిరిగి HB_DIRECTION_LTR;
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
శూన్య
hb_సంస్కరణ (గుర్తులేని పూర్ణాంకం *major,
	    గుర్తులేని పూర్ణాంకం *minor,
	    గుర్తులేని పూర్ణాంకం *micro)
{
  *major = HB_సంస్కరణ_MAJOR;
  *minor = HB_సంస్కరణ_MINOR;
  *micro = HB_సంస్కరణ_MICRO;
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
స్థిరరాశి అక్షరం *
hb_సంస్కరణ_string ()
{
  తిరిగి HB_సంస్కరణ_STRING;
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
hb_సంస్కరణ_atleast (గుర్తులేని పూర్ణాంకం major,
		    గుర్తులేని పూర్ణాంకం minor,
		    గుర్తులేని పూర్ణాంకం micro)
{
  తిరిగి HB_సంస్కరణ_ATLEAST (major, minor, micro);
}



/* hb_feature_t and hb_variation_t */

స్థిర తార్కిక
parse_space (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర)
{
  అయితే (*pp < చివర && ISSPACE (**pp))
    (*pp)++;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_char (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, అక్షరం c)
{
  parse_space (pp, చివర);

  ఒకవేళ (*pp == చివర || **pp != c)
    తిరిగి అబద్ధం;

  (*pp)++;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_uint (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, గుర్తులేని పూర్ణాంకం *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  పూర్ణాంకం v;
  ఒకవేళ (unlikely (!hb_parse_int (pp, చివర, &v))) తిరిగి అబద్ధం;

  *pv = v;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_uint32 (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, uint32_t *pv)
{
  /* Intentionally use hb_parse_int inside instead of hb_parse_uint,
   * such that -1 turns into "big number"... */
  పూర్ణాంకం v;
  ఒకవేళ (unlikely (!hb_parse_int (pp, చివర, &v))) తిరిగి అబద్ధం;

  *pv = v;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_bool (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, uint32_t *pv)
{
  parse_space (pp, చివర);

  స్థిరరాశి అక్షరం *p = *pp;
  అయితే (*pp < చివర && ISALPHA(**pp))
    (*pp)++;

  /* CSS allows on/off as aliases 1/0. */
  ఒకవేళ (*pp - p == 2
      && TOLOWER (p[0]) == 'o'
      && TOLOWER (p[1]) == 'n')
    *pv = 1;
  లేకపోతే ఒకవేళ (*pp - p == 3
	   && TOLOWER (p[0]) == 'o'
	   && TOLOWER (p[1]) == 'f'
	   && TOLOWER (p[2]) == 'f')
    *pv = 0;
  లేకపోతే
    తిరిగి అబద్ధం;

  తిరిగి నిజం;
}

/* hb_feature_t */

స్థిర తార్కిక
parse_feature_విలువ_ఉపసర్గ (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_feature_t *feature)
{
  ఒకవేళ (parse_char (pp, చివర, '-'))
    feature->value = 0;
  లేకపోతే {
    parse_char (pp, చివర, '+');
    feature->value = 1;
  }

  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_ట్యాగ్ (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_ట్యాగ్_t *ట్యాగ్_2)
{
  parse_space (pp, చివర);

  అక్షరం quote = 0;

  ఒకవేళ (*pp < చివర && (**pp == '\'' || **pp == '"'))
  {
    quote = **pp;
    (*pp)++;
  }

  స్థిరరాశి అక్షరం *p = *pp;
  అయితే (*pp < చివర && (**pp != ' ' && **pp != '=' && **pp != '[' && **pp != quote))
    (*pp)++;

  ఒకవేళ (p == *pp || *pp - p > 4)
    తిరిగి అబద్ధం;

  *ట్యాగ్_2 = hb_ట్యాగ్_from_string (p, *pp - p);

  ఒకవేళ (quote)
  {
    /* CSS expects exactly four bytes.  And we only allow quotations for
     * CSS compatibility.  So, enforce the length. */
     ఒకవేళ (*pp - p != 4)
       తిరిగి అబద్ధం;
    ఒకవేళ (*pp == చివర || **pp != quote)
      తిరిగి అబద్ధం;
    (*pp)++;
  }

  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_feature_indices (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_feature_t *feature)
{
  parse_space (pp, చివర);

  తార్కిక has_ప్రారంభం;

  feature->ప్రారంభం = HB_FEATURE_GLOBAL_ప్రారంభం;
  feature->చివర = HB_FEATURE_GLOBAL_చివర;

  ఒకవేళ (!parse_char (pp, చివర, '['))
    తిరిగి నిజం;

  has_ప్రారంభం = parse_uint (pp, చివర, &feature->ప్రారంభం);

  ఒకవేళ (parse_char (pp, చివర, ':') || parse_char (pp, చివర, ';')) {
    parse_uint (pp, చివర, &feature->చివర);
  } లేకపోతే {
    ఒకవేళ (has_ప్రారంభం)
      feature->చివర = feature->ప్రారంభం + 1;
  }

  తిరిగి parse_char (pp, చివర, ']');
}

స్థిర తార్కిక
parse_feature_విలువ_postfix (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_feature_t *feature)
{
  తార్కిక had_equal = parse_char (pp, చివర, '=');
  తార్కిక had_విలువ = parse_uint32 (pp, చివర, &feature->value) ||
		   parse_bool (pp, చివర, &feature->value);
  /* CSS doesn't use equal-sign between tag and value.
   * If there was an equal-sign, then there *must* be a value.
   * A value without an equal-sign is ok, but not required. */
  తిరిగి !had_equal || had_విలువ;
}

స్థిర తార్కిక
parse_one_feature (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_feature_t *feature)
{
  తిరిగి parse_feature_విలువ_ఉపసర్గ (pp, చివర, feature) &&
	 parse_ట్యాగ్ (pp, చివర, &feature->ట్యాగ్_2) &&
	 parse_feature_indices (pp, చివర, feature) &&
	 parse_feature_విలువ_postfix (pp, చివర, feature) &&
	 parse_space (pp, చివర) &&
	 *pp == చివర;
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
hb_feature_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len,
			hb_feature_t *feature)
{
  hb_feature_t feat;

  ఒకవేళ (len < 0)
    len = అక్షరశ్రేణి_పొడవు (str);

  ఒకవేళ (likely (parse_one_feature (&str, str + len, &feat)))
  {
    ఒకవేళ (feature)
      *feature = feat;
    తిరిగి నిజం;
  }

  ఒకవేళ (feature)
    hb_memset (feature, 0, పరిమాణం (*feature));
  తిరిగి అబద్ధం;
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
శూన్య
hb_feature_గమ్యం_string (hb_feature_t *feature,
		      అక్షరం *buf, గుర్తులేని పూర్ణాంకం కొలత)
{
  ఒకవేళ (unlikely (!కొలత)) తిరిగి;

  అక్షరం s[128];
  గుర్తులేని పూర్ణాంకం len = 0;
  ఒకవేళ (feature->value == 0)
    s[len++] = '-';
  hb_ట్యాగ్_గమ్యం_string (feature->ట్యాగ్_2, s + len);
  len += 4;
  అయితే (len && s[len - 1] == ' ')
    len--;
  ఒకవేళ (feature->ప్రారంభం != HB_FEATURE_GLOBAL_ప్రారంభం || feature->చివర != HB_FEATURE_GLOBAL_చివర)
  {
    s[len++] = '[';
    ఒకవేళ (feature->ప్రారంభం)
      len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%u", feature->ప్రారంభం));
    ఒకవేళ (feature->చివర != feature->ప్రారంభం + 1) {
      s[len++] = ':';
      ఒకవేళ (feature->చివర != HB_FEATURE_GLOBAL_చివర)
	len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%u", feature->చివర));
    }
    s[len++] = ']';
  }
  ఒకవేళ (feature->value > 1)
  {
    s[len++] = '=';
    len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%" PRIu32, feature->value));
  }
  ప్రతిజ్ఞా (len < ARRAY_నిడివి (s));
  len = hb_min (len, కొలత - 1);
  hb_memcpy (buf, s, len);
  buf[len] = '\0';
}

/* hb_variation_t */

స్థిర తార్కిక
parse_variation_విలువ (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_variation_t *variation)
{
  parse_char (pp, చివర, '='); /* Optional. */
  ద్విగుణ v;
  ఒకవేళ (unlikely (!hb_parse_double (pp, చివర, &v))) తిరిగి అబద్ధం;

  variation->value = v;
  తిరిగి నిజం;
}

స్థిర తార్కిక
parse_one_variation (స్థిరరాశి అక్షరం **pp, స్థిరరాశి అక్షరం *చివర, hb_variation_t *variation)
{
  తిరిగి parse_ట్యాగ్ (pp, చివర, &variation->ట్యాగ్_2) &&
	 parse_variation_విలువ (pp, చివర, variation) &&
	 parse_space (pp, చివర) &&
	 *pp == చివర;
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
hb_variation_from_string (స్థిరరాశి అక్షరం *str, పూర్ణాంకం len,
			  hb_variation_t *variation)
{
  hb_variation_t var;

  ఒకవేళ (len < 0)
    len = అక్షరశ్రేణి_పొడవు (str);

  ఒకవేళ (likely (parse_one_variation (&str, str + len, &var)))
  {
    ఒకవేళ (variation)
      *variation = var;
    తిరిగి నిజం;
  }

  ఒకవేళ (variation)
    hb_memset (variation, 0, పరిమాణం (*variation));
  తిరిగి అబద్ధం;
}

#నిర్వచితం_కాకపోతే HB_NO_SETLOCALE

స్థిర అంతఃవరుస శూన్య free_static_C_locale ();

స్థిర నిర్మాణం hb_C_locale_lazy_loader_t : hb_lazy_loader_t<hb_remove_సూచిక<hb_locale_t>,
							   hb_C_locale_lazy_loader_t>
{
  స్థిర hb_locale_t create ()
  {
    hb_locale_t l = newlocale (LC_ALL_MASK, "C", ఖాళీ);
    ఒకవేళ (!l)
      తిరిగి l;

    hb_atexit (free_static_C_locale);

    తిరిగి l;
  }
  స్థిర శూన్య destroy (hb_locale_t l)
  {
    freelocale (l);
  }
  స్థిర hb_locale_t get_null ()
  {
    తిరిగి (hb_locale_t) 0;
  }
} static_C_locale;

స్థిర అంతఃవరుస
శూన్య free_static_C_locale ()
{
  static_C_locale.free_instance ();
}

స్థిర hb_locale_t
get_C_locale ()
{
  తిరిగి static_C_locale.get_unconst ();
}

#అయితే_ముగింపు

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
శూన్య
hb_variation_గమ్యం_string (hb_variation_t *variation,
			అక్షరం *buf, గుర్తులేని పూర్ణాంకం కొలత)
{
  ఒకవేళ (unlikely (!కొలత)) తిరిగి;

  అక్షరం s[128];
  గుర్తులేని పూర్ణాంకం len = 0;
  hb_ట్యాగ్_గమ్యం_string (variation->ట్యాగ్_2, s + len);
  len += 4;
  అయితే (len && s[len - 1] == ' ')
    len--;
  s[len++] = '=';

  hb_locale_t oldlocale HB_వాడని;
  oldlocale = hb_uselocale (get_C_locale ());
  len += hb_max (0, snprintf (s + len, ARRAY_నిడివి (s) - len, "%g", (ద్విగుణ) variation->value));
  (శూన్య) hb_uselocale (oldlocale);

  ప్రతిజ్ఞా (len < ARRAY_నిడివి (s));
  len = hb_min (len, కొలత - 1);
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
  తిరిగి hb_color_get_alpha (color);
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
  తిరిగి hb_color_get_red (color);
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
  తిరిగి hb_color_get_green (color);
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
  తిరిగి hb_color_get_blue (color);
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
శూన్య* hb_malloc(size_t కొలత) { తిరిగి hb_malloc_impl (కొలత); }

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
శూన్య* hb_calloc(size_t nmemb, size_t కొలత) { తిరిగి hb_calloc_impl (nmemb, కొలత); }

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
శూన్య* hb_realloc(శూన్య *ptr, size_t కొలత) { తిరిగి hb_realloc_impl (ptr, కొలత); }

/**
 * hb_free:
 * @ptr: The pointer to the memory to free.
 *
 * Frees the memory pointed to by @ptr, using the allocator set at
 * compile-time. Typically just free().
 *
 * Since: 11.0.0
 **/
శూన్య  hb_free(శూన్య *ptr) { hb_free_impl (ptr); }


/* If there is no visibility control, then hb-static.cc will NOT
 * define anything.  Instead, we get it to define one set in here
 * only, so only libharfbuzz.so defines them, not other libs. */
#నిర్వచితమైతే HB_NO_VISIBILITY
#నిర్వచనం_రద్దు HB_NO_VISIBILITY
#అన్తర్భావ "hb-static.cc"
#నిర్వచించు HB_NO_VISIBILITY 1
#అయితే_ముగింపు
