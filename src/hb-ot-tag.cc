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

#அந்தர்பாவ "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

நிலையான hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  தேர்வு ((hb_tag_t) script)
  {
    நிலை HB_SCRIPT_INVALID:		திரும்பு HB_OT_TAG_DEFAULT_SCRIPT;
    நிலை HB_SCRIPT_MATH:		திரும்பு HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    நிலை HB_SCRIPT_HIRAGANA:		திரும்பு HB_TAG('k','a','n','a');
    நிலை HB_TAG('H','r','k','t'):
					திரும்பு HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    நிலை HB_SCRIPT_LAO:			திரும்பு HB_TAG('l','a','o',' ');
    நிலை HB_SCRIPT_YI:			திரும்பு HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    நிலை HB_SCRIPT_NKO:			திரும்பு HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    நிலை HB_SCRIPT_VAI:			திரும்பு HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  திரும்பு ((hb_tag_t) script) | 0x20000000u;
}

நிலையான hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  எனில் (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    திரும்பு HB_SCRIPT_INVALID;
  எனில் (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    திரும்பு HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  எனில் (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  எனில் (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  திரும்பு (hb_script_t) (tag & ~0x20000000u);
}

நிலையான hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  தேர்வு ((hb_tag_t) script) {
    நிலை HB_SCRIPT_BENGALI:		திரும்பு HB_TAG('b','n','g','2');
    நிலை HB_SCRIPT_DEVANAGARI:		திரும்பு HB_TAG('d','e','v','2');
    நிலை HB_SCRIPT_GUJARATI:		திரும்பு HB_TAG('g','j','r','2');
    நிலை HB_SCRIPT_GURMUKHI:		திரும்பு HB_TAG('g','u','r','2');
    நிலை HB_SCRIPT_KANNADA:		திரும்பு HB_TAG('k','n','d','2');
    நிலை HB_SCRIPT_MALAYALAM:		திரும்பு HB_TAG('m','l','m','2');
    நிலை HB_SCRIPT_ORIYA:		திரும்பு HB_TAG('o','r','y','2');
    நிலை HB_SCRIPT_TAMIL:		திரும்பு HB_TAG('t','m','l','2');
    நிலை HB_SCRIPT_TELUGU:		திரும்பு HB_TAG('t','e','l','2');
    நிலை HB_SCRIPT_MYANMAR:		திரும்பு HB_TAG('m','y','m','2');
  }

  திரும்பு HB_OT_TAG_DEFAULT_SCRIPT;
}

நிலையான hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  தேர்வு (tag) {
    நிலை HB_TAG('b','n','g','2'):	திரும்பு HB_SCRIPT_BENGALI;
    நிலை HB_TAG('d','e','v','2'):	திரும்பு HB_SCRIPT_DEVANAGARI;
    நிலை HB_TAG('g','j','r','2'):	திரும்பு HB_SCRIPT_GUJARATI;
    நிலை HB_TAG('g','u','r','2'):	திரும்பு HB_SCRIPT_GURMUKHI;
    நிலை HB_TAG('k','n','d','2'):	திரும்பு HB_SCRIPT_KANNADA;
    நிலை HB_TAG('m','l','m','2'):	திரும்பு HB_SCRIPT_MALAYALAM;
    நிலை HB_TAG('o','r','y','2'):	திரும்பு HB_SCRIPT_ORIYA;
    நிலை HB_TAG('t','m','l','2'):	திரும்பு HB_SCRIPT_TAMIL;
    நிலை HB_TAG('t','e','l','2'):	திரும்பு HB_SCRIPT_TELUGU;
    நிலை HB_TAG('m','y','m','2'):	திரும்பு HB_SCRIPT_MYANMAR;
  }

  திரும்பு HB_SCRIPT_UNKNOWN;
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
வெறுமை
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned முழு மொத்தம் = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &மொத்தம், tags, nullptr, nullptr);
  *script_tag_1 = மொத்தம் > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = மொத்தம் > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

நிலையான வெறுமை
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned முழு *மொத்தம் /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned முழு i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  எனில் (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    எனில் (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    எனில் (*மொத்தம் > i)
      tags[i++] = new_tag;
  }

  எனில் (*மொத்தம் > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    எனில் (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *மொத்தம் = i;
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
  unsigned எழுத்து digit = tag & 0x000000FFu;
  எனில் (unlikely (digit == '2' || digit == '3'))
    திரும்பு hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  திரும்பு hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

நிலையான உள்வரி தருக்க
subtag_matches (மாறிலி எழுத்து *lang_str,
		மாறிலி எழுத்து *limit,
		மாறிலி எழுத்து *subtag,
		unsigned    subtag_len)
{
  எனில் (likely ((unsigned) (limit - lang_str) < subtag_len))
    திரும்பு பொய்;

  செய் {
    மாறிலி எழுத்து *s = strstr (lang_str, subtag);
    எனில் (!s || s >= limit)
      திரும்பு பொய்;
    எனில் (!ISALNUM (s[subtag_len]))
      திரும்பு உண்மை;
    lang_str = s + subtag_len;
  } போது (உண்மை);
}

நிலையான தருக்க
lang_matches (மாறிலி எழுத்து *lang_str,
	      மாறிலி எழுத்து *limit,
	      மாறிலி எழுத்து *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  எனில் (likely ((unsigned) (limit - lang_str) < spec_len))
    திரும்பு பொய்;

  திரும்பு strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

நிலையான தருக்க
bfind_tag (மாறிலி hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  போது (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    எனில் (key < val)
      max = mid;
    இல்லையெனில் எனில் (key > val)
      min = mid + 1;
    இல்லையெனில்
      திரும்பு உண்மை;
  }

  திரும்பு பொய்;
}

கட்டமைப்பு LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  முழு cmp (hb_tag_t a) மாறிலி
  {
    திரும்பு a < இது->language ? -1 : a > இது->language ? +1 : 0;
  }
  முழு cmp (மாறிலி LangTag *that) மாறிலி
  { திரும்பு cmp (that->language); }
};

கட்டமைப்பு LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t மொத்தம்;

  முழு cmp (hb_tag_t a) மாறிலி
  {
    திரும்பு a < இது->language ? -1 : a > இது->language ? +1 : 0;
  }
  முழு cmp (மாறிலி LangTagRange *that) மாறிலி
  { திரும்பு cmp (that->language); }
};

#அந்தர்பாவ "hb-ot-tag-table.hh"

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
  unsigned முழு மொத்தம் = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &மொத்தம், tags);
  திரும்பு மொத்தம் > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

நிலையான வெறுமை
hb_ot_tags_from_language (மாறிலி எழுத்து   *lang_str,
			  மாறிலி எழுத்து   *limit,
			  unsigned முழு *மொத்தம்,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  எனில் (hb_ot_tags_from_complex_language (lang_str, limit, மொத்தம், tags))
    திரும்பு;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  மாறிலி எழுத்து *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    எனில் (s && limit - lang_str >= 6)
    {
      மாறிலி எழுத்து *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      எனில் (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    மாறிலி எழுத்து *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    எனில் (first_len == 2)
    {
      நிலையான hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      எனில் (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned முழு i;
	போது (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	க்கு (i = 0;
	     i < *மொத்தம் &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*மொத்தம் = i;
	திரும்பு;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    இல்லையெனில் எனில் (first_len == 3)
    {
      நிலையான hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      எனில் (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	எனில் (*மொத்தம்)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *மொத்தம் = 1;
	}
	இல்லையெனில்
	  *மொத்தம் = 0;
	திரும்பு;
      }

      நிலையான hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      எனில் (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	மாறிலி LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned முழு i;
	க்கு (i = 0; i < *மொத்தம் && i < range.மொத்தம்; i++)
	  tags[i] = ot_languages3_multi_மதிப்புகள்[range.offset + i];
	*மொத்தம் = i;
	திரும்பு;
      }

      எனில் (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*மொத்தம் = 0;
	திரும்பு;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *மொத்தம் = 1;
      திரும்பு;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  எனில் (!s)
    s = lang_str + strlen (lang_str);
  எனில் (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *மொத்தம் = 1;
    திரும்பு;
  }
#endif

  *மொத்தம் = 0;
}

நிலையான தருக்க
parse_private_use_subtag (மாறிலி எழுத்து     *private_use_subtag,
			  unsigned முழு   *மொத்தம்,
			  hb_tag_t       *tags,
			  மாறிலி எழுத்து     *prefix,
			  unsigned எழுத்து (*normalize) (unsigned எழுத்து))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  திரும்பு பொய்;
#endif

  எனில் (!(private_use_subtag && மொத்தம் && tags && *மொத்தம்)) திரும்பு பொய்;

  மாறிலி எழுத்து *s = strstr (private_use_subtag, prefix);
  எனில் (!s) திரும்பு பொய்;

  எழுத்து tag[4];
  முழு i;
  s += strlen (prefix);
  எனில் (s[0] == '-') {
    s += 1;
    எழுத்து c;
    க்கு (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      எனில் (i % 2 == 0)
	tag[i / 2] = c << 4;
      இல்லையெனில்
	tag[i / 2] += c;
    }
    எனில் (i != 8) திரும்பு பொய்;
  } இல்லையெனில் {
    க்கு (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    எனில் (!i) திரும்பு பொய்;

    க்கு (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  எனில் ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *மொத்தம் = 1;
  திரும்பு உண்மை;
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
வெறுமை
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned முழு *script_மொத்தம் /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned முழு *language_மொத்தம் /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  தருக்க needs_script = உண்மை;

  எனில் (language == HB_LANGUAGE_INVALID)
  {
    எனில் (language_மொத்தம் && language_tags && *language_மொத்தம்)
      *language_மொத்தம் = 0;
  }
  இல்லையெனில்
  {
    மாறிலி எழுத்து *lang_str, *s, *limit, *private_use_subtag;
    தருக்க needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    எனில் (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } இல்லையெனில் {
      க்கு (s = lang_str + 1; *s; s++)
      {
	எனில் (s[-1] == '-' && s[1] == '-')
	{
	  எனில் (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    எனில் (!limit)
	      limit = s - 1;
	    நிறுத்து;
	  } இல்லையெனில் எனில் (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      எனில் (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_மொத்தம், script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_மொத்தம், language_tags, "-hbot", TOUPPER);

    எனில் (needs_language && language_மொத்தம் && language_tags && *language_மொத்தம்)
      hb_ot_tags_from_language (lang_str, limit, language_மொத்தம், language_tags);
  }

  எனில் (needs_script && script_மொத்தம் && script_tags && *script_மொத்தம்)
    hb_ot_all_tags_from_script (script, script_மொத்தம், script_tags);
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
  unsigned முழு i;

  எனில் (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    திரும்பு nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    எனில் (disambiguated_tag != HB_LANGUAGE_INVALID)
      திரும்பு disambiguated_tag;
  }
#endif

  எழுத்து buf[4];
  க்கு (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    எனில் (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      திரும்பு hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  க்கு (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    எனில் (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      திரும்பு hb_language_from_string (buf, 3);
    }
  க்கு (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    க்கு (unsigned முழு j = 0; j < ot_languages3_multi[i].மொத்தம்; j++)
      எனில் (ot_languages3_multi_மதிப்புகள்[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	திரும்பு hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    எழுத்து buf[20];
    எழுத்து *str = buf;
    எனில் (ISALPHA (tag >> 24)
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
    திரும்பு hb_language_from_string (&*buf, -1);
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
வெறுமை
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  எனில் (script)
    *script = script_out;
  எனில் (language)
  {
    unsigned முழு script_மொத்தம் = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_மொத்தம்,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    மாறிலி எழுத்து *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    எனில் (lang_str && (script_மொத்தம் == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned எழுத்து *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned எழுத்து *) hb_malloc (len + 16);
      எனில் (unlikely (!buf))
      {
	*language = nullptr;
      }
      இல்லையெனில்
      {
	முழு shift;
	hb_memcpy (buf, lang_str, len);
	எனில் (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	க்கு (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((எழுத்து *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
நிலையான உள்வரி வெறுமை
test_langs_sorted ()
{
  க்கு (unsigned முழு i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    முழு c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    எனில் (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  க்கு (unsigned முழு i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    முழு c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    எனில் (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  க்கு (unsigned முழு i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    முழு c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    எனில் (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  க்கு (unsigned முழு i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    எனில் (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

முழு
முதன்மை ()
{
  test_langs_sorted ();
  திரும்பு 0;
}

#endif


#endif
