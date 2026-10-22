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

#अन्तर्भाव "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

स्थिर hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  विकल्प ((hb_tag_t) script)
  {
    पक्ष HB_SCRIPT_INVALID:		प्रत्यागम HB_OT_TAG_DEFAULT_SCRIPT;
    पक्ष HB_SCRIPT_MATH:		प्रत्यागम HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    पक्ष HB_SCRIPT_HIRAGANA:		प्रत्यागम HB_TAG('k','a','n','a');
    पक्ष HB_TAG('H','r','k','t'):
					प्रत्यागम HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    पक्ष HB_SCRIPT_LAO:			प्रत्यागम HB_TAG('l','a','o',' ');
    पक्ष HB_SCRIPT_YI:			प्रत्यागम HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    पक्ष HB_SCRIPT_NKO:			प्रत्यागम HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    पक्ष HB_SCRIPT_VAI:			प्रत्यागम HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  प्रत्यागम ((hb_tag_t) script) | 0x20000000u;
}

स्थिर hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  यदि (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    प्रत्यागम HB_SCRIPT_INVALID;
  यदि (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    प्रत्यागम HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  यदि (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  यदि (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  प्रत्यागम (hb_script_t) (tag & ~0x20000000u);
}

स्थिर hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  विकल्प ((hb_tag_t) script) {
    पक्ष HB_SCRIPT_BENGALI:		प्रत्यागम HB_TAG('b','n','g','2');
    पक्ष HB_SCRIPT_DEVANAGARI:		प्रत्यागम HB_TAG('d','e','v','2');
    पक्ष HB_SCRIPT_GUJARATI:		प्रत्यागम HB_TAG('g','j','r','2');
    पक्ष HB_SCRIPT_GURMUKHI:		प्रत्यागम HB_TAG('g','u','r','2');
    पक्ष HB_SCRIPT_KANNADA:		प्रत्यागम HB_TAG('k','n','d','2');
    पक्ष HB_SCRIPT_MALAYALAM:		प्रत्यागम HB_TAG('m','l','m','2');
    पक्ष HB_SCRIPT_ORIYA:		प्रत्यागम HB_TAG('o','r','y','2');
    पक्ष HB_SCRIPT_TAMIL:		प्रत्यागम HB_TAG('t','m','l','2');
    पक्ष HB_SCRIPT_TELUGU:		प्रत्यागम HB_TAG('t','e','l','2');
    पक्ष HB_SCRIPT_MYANMAR:		प्रत्यागम HB_TAG('m','y','m','2');
  }

  प्रत्यागम HB_OT_TAG_DEFAULT_SCRIPT;
}

स्थिर hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  विकल्प (tag) {
    पक्ष HB_TAG('b','n','g','2'):	प्रत्यागम HB_SCRIPT_BENGALI;
    पक्ष HB_TAG('d','e','v','2'):	प्रत्यागम HB_SCRIPT_DEVANAGARI;
    पक्ष HB_TAG('g','j','r','2'):	प्रत्यागम HB_SCRIPT_GUJARATI;
    पक्ष HB_TAG('g','u','r','2'):	प्रत्यागम HB_SCRIPT_GURMUKHI;
    पक्ष HB_TAG('k','n','d','2'):	प्रत्यागम HB_SCRIPT_KANNADA;
    पक्ष HB_TAG('m','l','m','2'):	प्रत्यागम HB_SCRIPT_MALAYALAM;
    पक्ष HB_TAG('o','r','y','2'):	प्रत्यागम HB_SCRIPT_ORIYA;
    पक्ष HB_TAG('t','m','l','2'):	प्रत्यागम HB_SCRIPT_TAMIL;
    पक्ष HB_TAG('t','e','l','2'):	प्रत्यागम HB_SCRIPT_TELUGU;
    पक्ष HB_TAG('m','y','m','2'):	प्रत्यागम HB_SCRIPT_MYANMAR;
  }

  प्रत्यागम HB_SCRIPT_UNKNOWN;
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
शून्य
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned पूर्णाङ्क गिनती = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &गिनती, tags, nullptr, nullptr);
  *script_tag_1 = गिनती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = गिनती > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

स्थिर शून्य
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned पूर्णाङ्क *गिनती /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned पूर्णाङ्क i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  यदि (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    यदि (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    यदि (*गिनती > i)
      tags[i++] = new_tag;
  }

  यदि (*गिनती > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    यदि (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *गिनती = i;
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
  unsigned वर्ण digit = tag & 0x000000FFu;
  यदि (unlikely (digit == '2' || digit == '3'))
    प्रत्यागम hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  प्रत्यागम hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

स्थिर अन्तःस्थ तार्किक
subtag_matches (ध्रुव वर्ण *lang_str,
		ध्रुव वर्ण *limit,
		ध्रुव वर्ण *subtag,
		unsigned    subtag_len)
{
  यदि (likely ((unsigned) (limit - lang_str) < subtag_len))
    प्रत्यागम असत्य;

  कुरु {
    ध्रुव वर्ण *s = strstr (lang_str, subtag);
    यदि (!s || s >= limit)
      प्रत्यागम असत्य;
    यदि (!ISALNUM (s[subtag_len]))
      प्रत्यागम सत्य;
    lang_str = s + subtag_len;
  } यावत् (सत्य);
}

स्थिर तार्किक
lang_matches (ध्रुव वर्ण *lang_str,
	      ध्रुव वर्ण *limit,
	      ध्रुव वर्ण *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  यदि (likely ((unsigned) (limit - lang_str) < spec_len))
    प्रत्यागम असत्य;

  प्रत्यागम strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

स्थिर तार्किक
bfind_tag (ध्रुव hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  यावत् (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    यदि (key < val)
      max = mid;
    अन्यथा यदि (key > val)
      min = mid + 1;
    अन्यथा
      प्रत्यागम सत्य;
  }

  प्रत्यागम असत्य;
}

संरचना LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  पूर्णाङ्क cmp (hb_tag_t a) ध्रुव
  {
    प्रत्यागम a < एतत्->language ? -1 : a > एतत्->language ? +1 : 0;
  }
  पूर्णाङ्क cmp (ध्रुव LangTag *that) ध्रुव
  { प्रत्यागम cmp (that->language); }
};

संरचना LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t गिनती;

  पूर्णाङ्क cmp (hb_tag_t a) ध्रुव
  {
    प्रत्यागम a < एतत्->language ? -1 : a > एतत्->language ? +1 : 0;
  }
  पूर्णाङ्क cmp (ध्रुव LangTagRange *that) ध्रुव
  { प्रत्यागम cmp (that->language); }
};

#अन्तर्भाव "hb-ot-tag-table.hh"

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
  unsigned पूर्णाङ्क गिनती = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &गिनती, tags);
  प्रत्यागम गिनती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

स्थिर शून्य
hb_ot_tags_from_language (ध्रुव वर्ण   *lang_str,
			  ध्रुव वर्ण   *limit,
			  unsigned पूर्णाङ्क *गिनती,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  यदि (hb_ot_tags_from_complex_language (lang_str, limit, गिनती, tags))
    प्रत्यागम;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  ध्रुव वर्ण *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    यदि (s && limit - lang_str >= 6)
    {
      ध्रुव वर्ण *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      यदि (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    ध्रुव वर्ण *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    यदि (first_len == 2)
    {
      स्थिर hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      यदि (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned पूर्णाङ्क i;
	यावत् (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	प्रति (i = 0;
	     i < *गिनती &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*गिनती = i;
	प्रत्यागम;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    अन्यथा यदि (first_len == 3)
    {
      स्थिर hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      यदि (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	यदि (*गिनती)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *गिनती = 1;
	}
	अन्यथा
	  *गिनती = 0;
	प्रत्यागम;
      }

      स्थिर hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      यदि (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	ध्रुव LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned पूर्णाङ्क i;
	प्रति (i = 0; i < *गिनती && i < range.गिनती; i++)
	  tags[i] = ot_languages3_multi_मूल्यावली[range.offset + i];
	*गिनती = i;
	प्रत्यागम;
      }

      यदि (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*गिनती = 0;
	प्रत्यागम;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *गिनती = 1;
      प्रत्यागम;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  यदि (!s)
    s = lang_str + strlen (lang_str);
  यदि (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *गिनती = 1;
    प्रत्यागम;
  }
#endif

  *गिनती = 0;
}

स्थिर तार्किक
parse_private_use_subtag (ध्रुव वर्ण     *private_use_subtag,
			  unsigned पूर्णाङ्क   *गिनती,
			  hb_tag_t       *tags,
			  ध्रुव वर्ण     *prefix,
			  unsigned वर्ण (*normalize) (unsigned वर्ण))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  प्रत्यागम असत्य;
#endif

  यदि (!(private_use_subtag && गिनती && tags && *गिनती)) प्रत्यागम असत्य;

  ध्रुव वर्ण *s = strstr (private_use_subtag, prefix);
  यदि (!s) प्रत्यागम असत्य;

  वर्ण tag[4];
  पूर्णाङ्क i;
  s += strlen (prefix);
  यदि (s[0] == '-') {
    s += 1;
    वर्ण c;
    प्रति (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      यदि (i % 2 == 0)
	tag[i / 2] = c << 4;
      अन्यथा
	tag[i / 2] += c;
    }
    यदि (i != 8) प्रत्यागम असत्य;
  } अन्यथा {
    प्रति (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    यदि (!i) प्रत्यागम असत्य;

    प्रति (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  यदि ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *गिनती = 1;
  प्रत्यागम सत्य;
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
शून्य
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned पूर्णाङ्क *script_गिनती /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned पूर्णाङ्क *language_गिनती /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  तार्किक needs_script = सत्य;

  यदि (language == HB_LANGUAGE_INVALID)
  {
    यदि (language_गिनती && language_tags && *language_गिनती)
      *language_गिनती = 0;
  }
  अन्यथा
  {
    ध्रुव वर्ण *lang_str, *s, *limit, *private_use_subtag;
    तार्किक needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    यदि (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } अन्यथा {
      प्रति (s = lang_str + 1; *s; s++)
      {
	यदि (s[-1] == '-' && s[1] == '-')
	{
	  यदि (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    यदि (!limit)
	      limit = s - 1;
	    भङ्ग;
	  } अन्यथा यदि (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      यदि (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_गिनती, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_गिनती, language_tags, "-hbot", TOUPPER);

    यदि (needs_language && language_गिनती && language_tags && *language_गिनती)
      hb_ot_tags_from_language (lang_str, limit, language_गिनती, language_tags);
  }

  यदि (needs_script && script_गिनती && script_tags && *script_गिनती)
    hb_ot_all_tags_from_script (script, script_गिनती, script_tags);
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
  unsigned पूर्णाङ्क i;

  यदि (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    प्रत्यागम nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    यदि (disambiguated_tag != HB_LANGUAGE_INVALID)
      प्रत्यागम disambiguated_tag;
  }
#endif

  वर्ण buf[4];
  प्रति (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    यदि (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      प्रत्यागम hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  प्रति (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    यदि (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      प्रत्यागम hb_language_from_string (buf, 3);
    }
  प्रति (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    प्रति (unsigned पूर्णाङ्क j = 0; j < ot_languages3_multi[i].गिनती; j++)
      यदि (ot_languages3_multi_मूल्यावली[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	प्रत्यागम hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    वर्ण buf[20];
    वर्ण *str = buf;
    यदि (ISALPHA (tag >> 24)
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
    प्रत्यागम hb_language_from_string (&*buf, -1);
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
शून्य
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  यदि (script)
    *script = script_out;
  यदि (language)
  {
    unsigned पूर्णाङ्क script_गिनती = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_गिनती,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    ध्रुव वर्ण *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    यदि (lang_str && (script_गिनती == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned वर्ण *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned वर्ण *) hb_malloc (len + 16);
      यदि (unlikely (!buf))
      {
	*language = nullptr;
      }
      अन्यथा
      {
	पूर्णाङ्क shift;
	hb_memcpy (buf, lang_str, len);
	यदि (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	प्रति (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((वर्ण *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
स्थिर अन्तःस्थ शून्य
test_langs_sorted ()
{
  प्रति (unsigned पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    पूर्णाङ्क c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    यदि (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  प्रति (unsigned पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    पूर्णाङ्क c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    यदि (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  प्रति (unsigned पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    पूर्णाङ्क c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    यदि (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  प्रति (unsigned पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    यदि (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

पूर्णाङ्क
main ()
{
  test_langs_sorted ();
  प्रत्यागम 0;
}

#endif


#endif
