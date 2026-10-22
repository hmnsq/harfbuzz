/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Google Author(s): Behdad Esfahbod, Roozbeh Pournader
 */

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

સ્થિર hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  પસંદગી ((hb_tag_t) script)
  {
    સ્થિતિ HB_SCRIPT_INVALID:		પાછા HB_OT_TAG_DEFAULT_SCRIPT;
    સ્થિતિ HB_SCRIPT_MATH:		પાછા HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    સ્થિતિ HB_SCRIPT_HIRAGANA:		પાછા HB_TAG('k','a','n','a');
    સ્થિતિ HB_TAG('H','r','k','t'):
					પાછા HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    સ્થિતિ HB_SCRIPT_LAO:			પાછા HB_TAG('l','a','o',' ');
    સ્થિતિ HB_SCRIPT_YI:			પાછા HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    સ્થિતિ HB_SCRIPT_NKO:			પાછા HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    સ્થિતિ HB_SCRIPT_VAI:			પાછા HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  પાછા ((hb_tag_t) script) | 0x20000000u;
}

સ્થિર hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  જો (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    પાછા HB_SCRIPT_INVALID;
  જો (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    પાછા HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  જો (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  જો (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  પાછા (hb_script_t) (tag & ~0x20000000u);
}

સ્થિર hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  પસંદગી ((hb_tag_t) script) {
    સ્થિતિ HB_SCRIPT_BENGALI:		પાછા HB_TAG('b','n','g','2');
    સ્થિતિ HB_SCRIPT_DEVANAGARI:		પાછા HB_TAG('d','e','v','2');
    સ્થિતિ HB_SCRIPT_GUJARATI:		પાછા HB_TAG('g','j','r','2');
    સ્થિતિ HB_SCRIPT_GURMUKHI:		પાછા HB_TAG('g','u','r','2');
    સ્થિતિ HB_SCRIPT_KANNADA:		પાછા HB_TAG('k','n','d','2');
    સ્થિતિ HB_SCRIPT_MALAYALAM:		પાછા HB_TAG('m','l','m','2');
    સ્થિતિ HB_SCRIPT_ORIYA:		પાછા HB_TAG('o','r','y','2');
    સ્થિતિ HB_SCRIPT_TAMIL:		પાછા HB_TAG('t','m','l','2');
    સ્થિતિ HB_SCRIPT_TELUGU:		પાછા HB_TAG('t','e','l','2');
    સ્થિતિ HB_SCRIPT_MYANMAR:		પાછા HB_TAG('m','y','m','2');
  }

  પાછા HB_OT_TAG_DEFAULT_SCRIPT;
}

સ્થિર hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  પસંદગી (tag) {
    સ્થિતિ HB_TAG('b','n','g','2'):	પાછા HB_SCRIPT_BENGALI;
    સ્થિતિ HB_TAG('d','e','v','2'):	પાછા HB_SCRIPT_DEVANAGARI;
    સ્થિતિ HB_TAG('g','j','r','2'):	પાછા HB_SCRIPT_GUJARATI;
    સ્થિતિ HB_TAG('g','u','r','2'):	પાછા HB_SCRIPT_GURMUKHI;
    સ્થિતિ HB_TAG('k','n','d','2'):	પાછા HB_SCRIPT_KANNADA;
    સ્થિતિ HB_TAG('m','l','m','2'):	પાછા HB_SCRIPT_MALAYALAM;
    સ્થિતિ HB_TAG('o','r','y','2'):	પાછા HB_SCRIPT_ORIYA;
    સ્થિતિ HB_TAG('t','m','l','2'):	પાછા HB_SCRIPT_TAMIL;
    સ્થિતિ HB_TAG('t','e','l','2'):	પાછા HB_SCRIPT_TELUGU;
    સ્થિતિ HB_TAG('m','y','m','2'):	પાછા HB_SCRIPT_MYANMAR;
  }

  પાછા HB_SCRIPT_UNKNOWN;
}

#ifndef HB_DISABLE_DEPRECATED
/**
 * hb_ot_tags_from_script:
 * @script: an #hb_script_t to convert.
 * @script_tag_1: (out): output #hb_tag_t.
 * @script_tag_2: (out): output #hb_tag_t.
 *
 * Converts an #hb_script_t to script tags.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
શૂન્ય
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned પૂર્ણાંક ગણતરી = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &ગણતરી, tags, nullptr, nullptr);
  *script_tag_1 = ગણતરી > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = ગણતરી > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

સ્થિર શૂન્ય
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned પૂર્ણાંક *ગણતરી /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned પૂર્ણાંક i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  જો (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    જો (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    જો (*ગણતરી > i)
      tags[i++] = new_tag;
  }

  જો (*ગણતરી > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    જો (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *ગણતરી = i;
}

/**
 * hb_ot_tag_to_script:
 * @tag: a script tag
 *
 * Converts a script tag to an #hb_script_t.
 *
 * Return value: The #hb_script_t corresponding to @tag.
 *
 **/
hb_script_t
hb_ot_tag_to_script (hb_tag_t tag)
{
  unsigned અક્ષર digit = tag & 0x000000FFu;
  જો (unlikely (digit == '2' || digit == '3'))
    પાછા hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  પાછા hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

સ્થિર અંતઃરેખ તાર્કિક
subtag_matches (અચલ અક્ષર *lang_str,
		અચલ અક્ષર *limit,
		અચલ અક્ષર *subtag,
		unsigned    subtag_len)
{
  જો (likely ((unsigned) (limit - lang_str) < subtag_len))
    પાછા ખોટું;

  કરો {
    અચલ અક્ષર *s = strstr (lang_str, subtag);
    જો (!s || s >= limit)
      પાછા ખોટું;
    જો (!ISALNUM (s[subtag_len]))
      પાછા સાચું;
    lang_str = s + subtag_len;
  } જ્યારે (સાચું);
}

સ્થિર તાર્કિક
lang_matches (અચલ અક્ષર *lang_str,
	      અચલ અક્ષર *limit,
	      અચલ અક્ષર *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  જો (likely ((unsigned) (limit - lang_str) < spec_len))
    પાછા ખોટું;

  પાછા strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

સ્થિર તાર્કિક
bfind_tag (અચલ hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  જ્યારે (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    જો (key < val)
      max = mid;
    નહીંતર જો (key > val)
      min = mid + 1;
    નહીંતર
      પાછા સાચું;
  }

  પાછા ખોટું;
}

સંરચના LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  પૂર્ણાંક cmp (hb_tag_t a) અચલ
  {
    પાછા a < આ->language ? -1 : a > આ->language ? +1 : 0;
  }
  પૂર્ણાંક cmp (અચલ LangTag *that) અચલ
  { પાછા cmp (that->language); }
};

સંરચના LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t ગણતરી;

  પૂર્ણાંક cmp (hb_tag_t a) અચલ
  {
    પાછા a < આ->language ? -1 : a > આ->language ? +1 : 0;
  }
  પૂર્ણાંક cmp (અચલ LangTagRange *that) અચલ
  { પાછા cmp (that->language); }
};

#અન્તર્ભાવ "hb-ot-tag-table.hh"

/* The corresponding languages IDs for the following IDs are unclear,
 * overlap, or are architecturally weird. Needs more research. */

/*{"??",	{HB_TAG('B','C','R',' ')}},*/	/* Bible Cree */
/*{"zh?",	{HB_TAG('C','H','N',' ')}},*/	/* Chinese (seen in Microsoft fonts) */
/*{"ar-Syrc?",	{HB_TAG('G','A','R',' ')}},*/	/* Garshuni */
/*{"??",	{HB_TAG('N','G','R',' ')}},*/	/* Nagari */
/*{"??",	{HB_TAG('Y','I','C',' ')}},*/	/* Yi Classic */
/*{"zh?",	{HB_TAG('Z','H','P',' ')}},*/	/* Chinese Phonetic */

#ifndef HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_tag_t
hb_ot_tag_from_language (hb_language_t language)
{
  unsigned પૂર્ણાંક ગણતરી = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &ગણતરી, tags);
  પાછા ગણતરી > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

સ્થિર શૂન્ય
hb_ot_tags_from_language (અચલ અક્ષર   *lang_str,
			  અચલ અક્ષર   *limit,
			  unsigned પૂર્ણાંક *ગણતરી,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  જો (hb_ot_tags_from_complex_language (lang_str, limit, ગણતરી, tags))
    પાછા;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  અચલ અક્ષર *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    જો (s && limit - lang_str >= 6)
    {
      અચલ અક્ષર *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      જો (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    અચલ અક્ષર *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    જો (first_len == 2)
    {
      સ્થિર hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      જો (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned પૂર્ણાંક i;
	જ્યારે (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	માટે (i = 0;
	     i < *ગણતરી &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*ગણતરી = i;
	પાછા;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    નહીંતર જો (first_len == 3)
    {
      સ્થિર hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      જો (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	જો (*ગણતરી)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *ગણતરી = 1;
	}
	નહીંતર
	  *ગણતરી = 0;
	પાછા;
      }

      સ્થિર hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      જો (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	અચલ LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned પૂર્ણાંક i;
	માટે (i = 0; i < *ગણતરી && i < range.ગણતરી; i++)
	  tags[i] = ot_languages3_multi_કિંમતો[range.offset + i];
	*ગણતરી = i;
	પાછા;
      }

      જો (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*ગણતરી = 0;
	પાછા;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *ગણતરી = 1;
      પાછા;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  જો (!s)
    s = lang_str + strlen (lang_str);
  જો (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *ગણતરી = 1;
    પાછા;
  }
#endif

  *ગણતરી = 0;
}

સ્થિર તાર્કિક
parse_private_use_subtag (અચલ અક્ષર     *private_use_subtag,
			  unsigned પૂર્ણાંક   *ગણતરી,
			  hb_tag_t       *tags,
			  અચલ અક્ષર     *prefix,
			  unsigned અક્ષર (*normalize) (unsigned અક્ષર))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  પાછા ખોટું;
#endif

  જો (!(private_use_subtag && ગણતરી && tags && *ગણતરી)) પાછા ખોટું;

  અચલ અક્ષર *s = strstr (private_use_subtag, prefix);
  જો (!s) પાછા ખોટું;

  અક્ષર tag[4];
  પૂર્ણાંક i;
  s += strlen (prefix);
  જો (s[0] == '-') {
    s += 1;
    અક્ષર c;
    માટે (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      જો (i % 2 == 0)
	tag[i / 2] = c << 4;
      નહીંતર
	tag[i / 2] += c;
    }
    જો (i != 8) પાછા ખોટું;
  } નહીંતર {
    માટે (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    જો (!i) પાછા ખોટું;

    માટે (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  જો ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *ગણતરી = 1;
  પાછા સાચું;
}

/**
 * hb_ot_tags_from_script_and_language:
 * @script: an #hb_script_t to convert.
 * @language: (nullable): an #hb_language_t to convert.
 * @script_count: (inout) (optional): maximum number of script tags to retrieve (IN)
 * and actual number of script tags retrieved (OUT)
 * @script_tags: (out) (optional): array of size at least @script_count to store the
 * script tag results
 * @language_count: (inout) (optional): maximum number of language tags to retrieve
 * (IN) and actual number of language tags retrieved (OUT)
 * @language_tags: (out) (optional): array of size at least @language_count to store
 * the language tag results
 *
 * Converts an #hb_script_t and an #hb_language_t to script and language tags.
 *
 * Since: 2.0.0
 **/
શૂન્ય
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned પૂર્ણાંક *script_ગણતરી /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned પૂર્ણાંક *language_ગણતરી /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  તાર્કિક needs_script = સાચું;

  જો (language == HB_LANGUAGE_INVALID)
  {
    જો (language_ગણતરી && language_tags && *language_ગણતરી)
      *language_ગણતરી = 0;
  }
  નહીંતર
  {
    અચલ અક્ષર *lang_str, *s, *limit, *private_use_subtag;
    તાર્કિક needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    જો (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } નહીંતર {
      માટે (s = lang_str + 1; *s; s++)
      {
	જો (s[-1] == '-' && s[1] == '-')
	{
	  જો (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    જો (!limit)
	      limit = s - 1;
	    તોડો;
	  } નહીંતર જો (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      જો (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_ગણતરી, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_ગણતરી, language_tags, "-hbot", TOUPPER);

    જો (needs_language && language_ગણતરી && language_tags && *language_ગણતરી)
      hb_ot_tags_from_language (lang_str, limit, language_ગણતરી, language_tags);
  }

  જો (needs_script && script_ગણતરી && script_tags && *script_ગણતરી)
    hb_ot_all_tags_from_script (script, script_ગણતરી, script_tags);
}

/**
 * hb_ot_tag_to_language:
 * @tag: an language tag
 *
 * Converts a language tag to an #hb_language_t.
 *
 * Return value: (transfer none) (nullable):
 * The #hb_language_t corresponding to @tag.
 *
 * Since: 0.9.2
 **/
hb_language_t
hb_ot_tag_to_language (hb_tag_t tag)
{
  unsigned પૂર્ણાંક i;

  જો (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    પાછા nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    જો (disambiguated_tag != HB_LANGUAGE_INVALID)
      પાછા disambiguated_tag;
  }
#endif

  અક્ષર buf[4];
  માટે (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    જો (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      પાછા hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  માટે (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    જો (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      પાછા hb_language_from_string (buf, 3);
    }
  માટે (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    માટે (unsigned પૂર્ણાંક j = 0; j < ot_languages3_multi[i].ગણતરી; j++)
      જો (ot_languages3_multi_કિંમતો[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	પાછા hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    અક્ષર buf[20];
    અક્ષર *str = buf;
    જો (ISALPHA (tag >> 24)
	&& ISALPHA ((tag >> 16) & 0xFF)
	&& ISALPHA ((tag >> 8) & 0xFF)
	&& (tag & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (tag >> 24);
      buf[1] = TOLOWER ((tag >> 16) & 0xFF);
      buf[2] = TOLOWER ((tag >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, tag);
    પાછા hb_language_from_string (&*buf, -1);
  }
}

/**
 * hb_ot_tags_to_script_and_language:
 * @script_tag: a script tag
 * @language_tag: a language tag
 * @script: (out) (optional): the #hb_script_t corresponding to @script_tag.
 * @language: (out) (optional): the #hb_language_t corresponding to @script_tag and
 * @language_tag.
 *
 * Converts a script tag and a language tag to an #hb_script_t and an
 * #hb_language_t.
 *
 * Since: 2.0.0
 **/
શૂન્ય
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  જો (script)
    *script = script_out;
  જો (language)
  {
    unsigned પૂર્ણાંક script_ગણતરી = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_ગણતરી,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    અચલ અક્ષર *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    જો (lang_str && (script_ગણતરી == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned અક્ષર *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned અક્ષર *) hb_malloc (len + 16);
      જો (unlikely (!buf))
      {
	*language = nullptr;
      }
      નહીંતર
      {
	પૂર્ણાંક shift;
	hb_memcpy (buf, lang_str, len);
	જો (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	માટે (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((અક્ષર *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
સ્થિર અંતઃરેખ શૂન્ય
test_langs_sorted ()
{
  માટે (unsigned પૂર્ણાંક i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    પૂર્ણાંક c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    જો (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  માટે (unsigned પૂર્ણાંક i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    પૂર્ણાંક c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    જો (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  માટે (unsigned પૂર્ણાંક i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    પૂર્ણાંક c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    જો (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  માટે (unsigned પૂર્ણાંક i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    જો (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

પૂર્ણાંક
મુખ્ય ()
{
  test_langs_sorted ();
  પાછા 0;
}

#endif


#endif
