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

#include "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

static hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  switch ((hb_tag_t) script)
  {
    pak_sa HB_SCRIPT_INVALID:		ᤀᤡᤴ HB_OT_TAG_DEFAULT_SCRIPT;
    pak_sa HB_SCRIPT_MATH:		ᤀᤡᤴ HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    pak_sa HB_SCRIPT_HIRAGANA:		ᤀᤡᤴ HB_TAG('k','a','n','a');
    pak_sa HB_TAG('H','r','k','t'):
					ᤀᤡᤴ HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    pak_sa HB_SCRIPT_LAO:			ᤀᤡᤴ HB_TAG('l','a','o',' ');
    pak_sa HB_SCRIPT_YI:			ᤀᤡᤴ HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    pak_sa HB_SCRIPT_NKO:			ᤀᤡᤴ HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    pak_sa HB_SCRIPT_VAI:			ᤀᤡᤴ HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  ᤀᤡᤴ ((hb_tag_t) script) | 0x20000000u;
}

static hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  ᤀᤡ᤺ (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    ᤀᤡᤴ HB_SCRIPT_INVALID;
  ᤀᤡ᤺ (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    ᤀᤡᤴ HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  ᤀᤡ᤺ (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  ᤀᤡ᤺ (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  ᤀᤡᤴ (hb_script_t) (tag & ~0x20000000u);
}

static hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  switch ((hb_tag_t) script) {
    pak_sa HB_SCRIPT_BENGALI:		ᤀᤡᤴ HB_TAG('b','n','g','2');
    pak_sa HB_SCRIPT_DEVANAGARI:		ᤀᤡᤴ HB_TAG('d','e','v','2');
    pak_sa HB_SCRIPT_GUJARATI:		ᤀᤡᤴ HB_TAG('g','j','r','2');
    pak_sa HB_SCRIPT_GURMUKHI:		ᤀᤡᤴ HB_TAG('g','u','r','2');
    pak_sa HB_SCRIPT_KANNADA:		ᤀᤡᤴ HB_TAG('k','n','d','2');
    pak_sa HB_SCRIPT_MALAYALAM:		ᤀᤡᤴ HB_TAG('m','l','m','2');
    pak_sa HB_SCRIPT_ORIYA:		ᤀᤡᤴ HB_TAG('o','r','y','2');
    pak_sa HB_SCRIPT_TAMIL:		ᤀᤡᤴ HB_TAG('t','m','l','2');
    pak_sa HB_SCRIPT_TELUGU:		ᤀᤡᤴ HB_TAG('t','e','l','2');
    pak_sa HB_SCRIPT_MYANMAR:		ᤀᤡᤴ HB_TAG('m','y','m','2');
  }

  ᤀᤡᤴ HB_OT_TAG_DEFAULT_SCRIPT;
}

static hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  switch (tag) {
    pak_sa HB_TAG('b','n','g','2'):	ᤀᤡᤴ HB_SCRIPT_BENGALI;
    pak_sa HB_TAG('d','e','v','2'):	ᤀᤡᤴ HB_SCRIPT_DEVANAGARI;
    pak_sa HB_TAG('g','j','r','2'):	ᤀᤡᤴ HB_SCRIPT_GUJARATI;
    pak_sa HB_TAG('g','u','r','2'):	ᤀᤡᤴ HB_SCRIPT_GURMUKHI;
    pak_sa HB_TAG('k','n','d','2'):	ᤀᤡᤴ HB_SCRIPT_KANNADA;
    pak_sa HB_TAG('m','l','m','2'):	ᤀᤡᤴ HB_SCRIPT_MALAYALAM;
    pak_sa HB_TAG('o','r','y','2'):	ᤀᤡᤴ HB_SCRIPT_ORIYA;
    pak_sa HB_TAG('t','m','l','2'):	ᤀᤡᤴ HB_SCRIPT_TAMIL;
    pak_sa HB_TAG('t','e','l','2'):	ᤀᤡᤴ HB_SCRIPT_TELUGU;
    pak_sa HB_TAG('m','y','m','2'):	ᤀᤡᤴ HB_SCRIPT_MYANMAR;
  }

  ᤀᤡᤴ HB_SCRIPT_UNKNOWN;
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
void
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned puur_naa_nka count = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &count, tags, nullptr, nullptr);
  *script_tag_1 = count > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = count > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

static void
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned puur_naa_nka *count /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned puur_naa_nka i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  ᤀᤡ᤺ (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    ᤀᤡ᤺ (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    ᤀᤡ᤺ (*count > i)
      tags[i++] = new_tag;
  }

  ᤀᤡ᤺ (*count > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    ᤀᤡ᤺ (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *count = i;
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
  unsigned var_na digit = tag & 0x000000FFu;
  ᤀᤡ᤺ (unlikely (digit == '2' || digit == '3'))
    ᤀᤡᤴ hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  ᤀᤡᤴ hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

static anta_hstha bool
subtag_matches (const var_na *lang_str,
		const var_na *limit,
		const var_na *subtag,
		unsigned    subtag_len)
{
  ᤀᤡ᤺ (likely ((unsigned) (limit - lang_str) < subtag_len))
    ᤀᤡᤴ false;

  do {
    const var_na *s = strstr (lang_str, subtag);
    ᤀᤡ᤺ (!s || s >= limit)
      ᤀᤡᤴ false;
    ᤀᤡ᤺ (!ISALNUM (s[subtag_len]))
      ᤀᤡᤴ true;
    lang_str = s + subtag_len;
  } ᤀᤠ᤺ (true);
}

static bool
lang_matches (const var_na *lang_str,
	      const var_na *limit,
	      const var_na *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  ᤀᤡ᤺ (likely ((unsigned) (limit - lang_str) < spec_len))
    ᤀᤡᤴ false;

  ᤀᤡᤴ strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

static bool
bfind_tag (const hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  ᤀᤠ᤺ (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    ᤀᤡ᤺ (key < val)
      max = mid;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (key > val)
      min = mid + 1;
    ᤀᤡ᤺ᤆᤡ
      ᤀᤡᤴ true;
  }

  ᤀᤡᤴ false;
}

sa_nracanaa LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  puur_naa_nka cmp (hb_tag_t a) const
  {
    ᤀᤡᤴ a < this->language ? -1 : a > this->language ? +1 : 0;
  }
  puur_naa_nka cmp (const LangTag *that) const
  { ᤀᤡᤴ cmp (that->language); }
};

sa_nracanaa LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t count;

  puur_naa_nka cmp (hb_tag_t a) const
  {
    ᤀᤡᤴ a < this->language ? -1 : a > this->language ? +1 : 0;
  }
  puur_naa_nka cmp (const LangTagRange *that) const
  { ᤀᤡᤴ cmp (that->language); }
};

#include "hb-ot-tag-table.hh"

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
  unsigned puur_naa_nka count = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &count, tags);
  ᤀᤡᤴ count > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

static void
hb_ot_tags_from_language (const var_na   *lang_str,
			  const var_na   *limit,
			  unsigned puur_naa_nka *count,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  ᤀᤡ᤺ (hb_ot_tags_from_complex_language (lang_str, limit, count, tags))
    ᤀᤡᤴ;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  const var_na *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    ᤀᤡ᤺ (s && limit - lang_str >= 6)
    {
      const var_na *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      ᤀᤡ᤺ (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    const var_na *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    ᤀᤡ᤺ (first_len == 2)
    {
      static hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      ᤀᤡ᤺ (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned puur_naa_nka i;
	ᤀᤠ᤺ (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	for (i = 0;
	     i < *count &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*count = i;
	ᤀᤡᤴ;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (first_len == 3)
    {
      static hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      ᤀᤡ᤺ (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	ᤀᤡ᤺ (*count)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *count = 1;
	}
	ᤀᤡ᤺ᤆᤡ
	  *count = 0;
	ᤀᤡᤴ;
      }

      static hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      ᤀᤡ᤺ (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	const LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned puur_naa_nka i;
	for (i = 0; i < *count && i < range.count; i++)
	  tags[i] = ot_languages3_multi_values[range.offset + i];
	*count = i;
	ᤀᤡᤴ;
      }

      ᤀᤡ᤺ (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*count = 0;
	ᤀᤡᤴ;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *count = 1;
      ᤀᤡᤴ;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  ᤀᤡ᤺ (!s)
    s = lang_str + strlen (lang_str);
  ᤀᤡ᤺ (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *count = 1;
    ᤀᤡᤴ;
  }
#endif

  *count = 0;
}

static bool
parse_private_use_subtag (const var_na     *private_use_subtag,
			  unsigned puur_naa_nka   *count,
			  hb_tag_t       *tags,
			  const var_na     *prefix,
			  unsigned var_na (*normalize) (unsigned var_na))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  ᤀᤡᤴ false;
#endif

  ᤀᤡ᤺ (!(private_use_subtag && count && tags && *count)) ᤀᤡᤴ false;

  const var_na *s = strstr (private_use_subtag, prefix);
  ᤀᤡ᤺ (!s) ᤀᤡᤴ false;

  var_na tag[4];
  puur_naa_nka i;
  s += strlen (prefix);
  ᤀᤡ᤺ (s[0] == '-') {
    s += 1;
    var_na c;
    for (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      ᤀᤡ᤺ (i % 2 == 0)
	tag[i / 2] = c << 4;
      ᤀᤡ᤺ᤆᤡ
	tag[i / 2] += c;
    }
    ᤀᤡ᤺ (i != 8) ᤀᤡᤴ false;
  } ᤀᤡ᤺ᤆᤡ {
    for (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    ᤀᤡ᤺ (!i) ᤀᤡᤴ false;

    for (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  ᤀᤡ᤺ ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *count = 1;
  ᤀᤡᤴ true;
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
void
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned puur_naa_nka *script_count /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned puur_naa_nka *language_count /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  bool needs_script = true;

  ᤀᤡ᤺ (language == HB_LANGUAGE_INVALID)
  {
    ᤀᤡ᤺ (language_count && language_tags && *language_count)
      *language_count = 0;
  }
  ᤀᤡ᤺ᤆᤡ
  {
    const var_na *lang_str, *s, *limit, *private_use_subtag;
    bool needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    ᤀᤡ᤺ (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } ᤀᤡ᤺ᤆᤡ {
      for (s = lang_str + 1; *s; s++)
      {
	ᤀᤡ᤺ (s[-1] == '-' && s[1] == '-')
	{
	  ᤀᤡ᤺ (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    ᤀᤡ᤺ (!limit)
	      limit = s - 1;
	    bha_nga;
	  } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      ᤀᤡ᤺ (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_count, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_count, language_tags, "-hbot", TOUPPER);

    ᤀᤡ᤺ (needs_language && language_count && language_tags && *language_count)
      hb_ot_tags_from_language (lang_str, limit, language_count, language_tags);
  }

  ᤀᤡ᤺ (needs_script && script_count && script_tags && *script_count)
    hb_ot_all_tags_from_script (script, script_count, script_tags);
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
  unsigned puur_naa_nka i;

  ᤀᤡ᤺ (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    ᤀᤡᤴ nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    ᤀᤡ᤺ (disambiguated_tag != HB_LANGUAGE_INVALID)
      ᤀᤡᤴ disambiguated_tag;
  }
#endif

  var_na buf[4];
  for (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    ᤀᤡ᤺ (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      ᤀᤡᤴ hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  for (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    ᤀᤡ᤺ (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      ᤀᤡᤴ hb_language_from_string (buf, 3);
    }
  for (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    for (unsigned puur_naa_nka j = 0; j < ot_languages3_multi[i].count; j++)
      ᤀᤡ᤺ (ot_languages3_multi_values[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	ᤀᤡᤴ hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    var_na buf[20];
    var_na *str = buf;
    ᤀᤡ᤺ (ISALPHA (tag >> 24)
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
    ᤀᤡᤴ hb_language_from_string (&*buf, -1);
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
void
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  ᤀᤡ᤺ (script)
    *script = script_out;
  ᤀᤡ᤺ (language)
  {
    unsigned puur_naa_nka script_count = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_count,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    const var_na *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    ᤀᤡ᤺ (lang_str && (script_count == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned var_na *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned var_na *) hb_malloc (len + 16);
      ᤀᤡ᤺ (unlikely (!buf))
      {
	*language = nullptr;
      }
      ᤀᤡ᤺ᤆᤡ
      {
	puur_naa_nka shift;
	hb_memcpy (buf, lang_str, len);
	ᤀᤡ᤺ (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	for (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((var_na *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
static anta_hstha void
test_langs_sorted ()
{
  for (unsigned puur_naa_nka i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    puur_naa_nka c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    ᤀᤡ᤺ (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  for (unsigned puur_naa_nka i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    puur_naa_nka c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    ᤀᤡ᤺ (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  for (unsigned puur_naa_nka i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    puur_naa_nka c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    ᤀᤡ᤺ (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  for (unsigned puur_naa_nka i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    ᤀᤡ᤺ (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

puur_naa_nka
main ()
{
  test_langs_sorted ();
  ᤀᤡᤴ 0;
}

#endif


#endif
