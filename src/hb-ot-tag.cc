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

#समावेश "hb.hh"

#अगर_अघोषित HB_NO_OT_TAG


/* hb_script_t */

अटल hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  चयन ((hb_tag_t) script)
  {
    हाल HB_SCRIPT_INVALID:		वापस HB_OT_TAG_DEFAULT_SCRIPT;
    हाल HB_SCRIPT_MATH:		वापस HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    हाल HB_SCRIPT_HIRAGANA:		वापस HB_TAG('k','a','n','a');
    हाल HB_TAG('H','r','k','t'):
					वापस HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    हाल HB_SCRIPT_LAO:			वापस HB_TAG('l','a','o',' ');
    हाल HB_SCRIPT_YI:			वापस HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    हाल HB_SCRIPT_NKO:			वापस HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    हाल HB_SCRIPT_VAI:			वापस HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  वापस ((hb_tag_t) script) | 0x20000000u;
}

अटल hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  अगर (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    वापस HB_SCRIPT_INVALID;
  अगर (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    वापस HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  अगर (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  अगर (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  वापस (hb_script_t) (tag & ~0x20000000u);
}

अटल hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  चयन ((hb_tag_t) script) {
    हाल HB_SCRIPT_BENGALI:		वापस HB_TAG('b','n','g','2');
    हाल HB_SCRIPT_DEVANAGARI:		वापस HB_TAG('d','e','v','2');
    हाल HB_SCRIPT_GUJARATI:		वापस HB_TAG('g','j','r','2');
    हाल HB_SCRIPT_GURMUKHI:		वापस HB_TAG('g','u','r','2');
    हाल HB_SCRIPT_KANNADA:		वापस HB_TAG('k','n','d','2');
    हाल HB_SCRIPT_MALAYALAM:		वापस HB_TAG('m','l','m','2');
    हाल HB_SCRIPT_ORIYA:		वापस HB_TAG('o','r','y','2');
    हाल HB_SCRIPT_TAMIL:		वापस HB_TAG('t','m','l','2');
    हाल HB_SCRIPT_TELUGU:		वापस HB_TAG('t','e','l','2');
    हाल HB_SCRIPT_MYANMAR:		वापस HB_TAG('m','y','m','2');
  }

  वापस HB_OT_TAG_DEFAULT_SCRIPT;
}

अटल hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  चयन (tag) {
    हाल HB_TAG('b','n','g','2'):	वापस HB_SCRIPT_BENGALI;
    हाल HB_TAG('d','e','v','2'):	वापस HB_SCRIPT_DEVANAGARI;
    हाल HB_TAG('g','j','r','2'):	वापस HB_SCRIPT_GUJARATI;
    हाल HB_TAG('g','u','r','2'):	वापस HB_SCRIPT_GURMUKHI;
    हाल HB_TAG('k','n','d','2'):	वापस HB_SCRIPT_KANNADA;
    हाल HB_TAG('m','l','m','2'):	वापस HB_SCRIPT_MALAYALAM;
    हाल HB_TAG('o','r','y','2'):	वापस HB_SCRIPT_ORIYA;
    हाल HB_TAG('t','m','l','2'):	वापस HB_SCRIPT_TAMIL;
    हाल HB_TAG('t','e','l','2'):	वापस HB_SCRIPT_TELUGU;
    हाल HB_TAG('m','y','m','2'):	वापस HB_SCRIPT_MYANMAR;
  }

  वापस HB_SCRIPT_UNKNOWN;
}

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
व्योम
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  अचिन्हित पूर्णांक गिनती = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &गिनती, tags, nullptr, nullptr);
  *script_tag_1 = गिनती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = गिनती > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#पूर्ण_अगर

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

अटल व्योम
hb_ot_all_tags_from_script (hb_script_t   script,
			    अचिन्हित पूर्णांक *गिनती /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  अचिन्हित पूर्णांक i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  अगर (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    अगर (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    अगर (*गिनती > i)
      tags[i++] = new_tag;
  }

  अगर (*गिनती > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    अगर (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
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
  अचिन्हित अक्षर digit = tag & 0x000000FFu;
  अगर (unlikely (digit == '2' || digit == '3'))
    वापस hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  वापस hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

अटल अंतरभूत bool
subtag_matches (स्थिर अक्षर *lang_str,
		स्थिर अक्षर *limit,
		स्थिर अक्षर *subtag,
		अचिन्हित    subtag_len)
{
  अगर (likely ((अचिन्हित) (limit - lang_str) < subtag_len))
    वापस false;

  करो {
    स्थिर अक्षर *s = म_माला (lang_str, subtag);
    अगर (!s || s >= limit)
      वापस false;
    अगर (!ISALNUM (s[subtag_len]))
      वापस true;
    lang_str = s + subtag_len;
  } जबतक (true);
}

अटल bool
lang_matches (स्थिर अक्षर *lang_str,
	      स्थिर अक्षर *limit,
	      स्थिर अक्षर *spec,
	      अचिन्हित    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  अगर (likely ((अचिन्हित) (limit - lang_str) < spec_len))
    वापस false;

  वापस म_भेदन (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

अटल bool
bfind_tag (स्थिर hb_tag_t *array,
	   अचिन्हित        len,
	   hb_tag_t        key)
{
  अचिन्हित min = 0;
  अचिन्हित max = len;

  जबतक (min < max)
  {
    अचिन्हित mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    अगर (key < val)
      max = mid;
    अन्यथा अगर (key > val)
      min = mid + 1;
    अन्यथा
      वापस true;
  }

  वापस false;
}

काष्ठा LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  पूर्णांक cmp (hb_tag_t a) स्थिर
  {
    वापस a < यह->language ? -1 : a > यह->language ? +1 : 0;
  }
  पूर्णांक cmp (स्थिर LangTag *that) स्थिर
  { वापस cmp (that->language); }
};

काष्ठा LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t गिनती;

  पूर्णांक cmp (hb_tag_t a) स्थिर
  {
    वापस a < यह->language ? -1 : a > यह->language ? +1 : 0;
  }
  पूर्णांक cmp (स्थिर LangTagRange *that) स्थिर
  { वापस cmp (that->language); }
};

#समावेश "hb-ot-tag-table.hh"

/* The corresponding languages IDs for the following IDs are unclear,
 * overlap, or are architecturally weird. Needs more research. */

/*{"??",	{HB_TAG('B','C','R',' ')}},*/	/* Bible Cree */
/*{"zh?",	{HB_TAG('C','H','N',' ')}},*/	/* Chinese (seen in Microsoft fonts) */
/*{"ar-Syrc?",	{HB_TAG('G','A','R',' ')}},*/	/* Garshuni */
/*{"??",	{HB_TAG('N','G','R',' ')}},*/	/* Nagari */
/*{"??",	{HB_TAG('Y','I','C',' ')}},*/	/* Yi Classic */
/*{"zh?",	{HB_TAG('Z','H','P',' ')}},*/	/* Chinese Phonetic */

#अगर_अघोषित HB_DISABLE_DEPRECATED
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
  अचिन्हित पूर्णांक गिनती = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &गिनती, tags);
  वापस गिनती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#पूर्ण_अगर

अटल व्योम
hb_ot_tags_from_language (स्थिर अक्षर   *lang_str,
			  स्थिर अक्षर   *limit,
			  अचिन्हित पूर्णांक *गिनती,
			  hb_tag_t     *tags)
{

#अगर_अघोषित HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  अगर (hb_ot_tags_from_complex_language (lang_str, limit, गिनती, tags))
    वापस;
#पूर्ण_अगर

  /* Find a language matching in the first component. */
#अगर_अघोषित HB_NO_LANGUAGE_LONG
  स्थिर अक्षर *s; s = म_अक्षर (lang_str, '-');
#पूर्ण_अगर
  {
#अगर_अघोषित HB_NO_LANGUAGE_LONG
    अगर (s && limit - lang_str >= 6)
    {
      स्थिर अक्षर *extlang_end = म_अक्षर (s + 1, '-');
      /* If there is an extended language tag, use it. */
      अगर (3 == (extlang_end ? extlang_end - s - 1 : म_माप (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#पूर्ण_अगर
    स्थिर अक्षर *dash = म_अक्षर (lang_str, '-');
    अचिन्हित first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    अगर (first_len == 2)
    {
      अटल hb_atomic_t<अचिन्हित> last_tag_idx_2 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_2;

      अगर (likely (tag_idx < ARRAY_लंबाई (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	अचिन्हित पूर्णांक i;
	जबतक (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	क्रम (i = 0;
	     i < *गिनती &&
	     tag_idx + i < ARRAY_लंबाई (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*गिनती = i;
	वापस;
      }
    }
#अगर_अघोषित HB_NO_LANGUAGE_LONG
    अन्यथा अगर (first_len == 3)
    {
      अटल hb_atomic_t<अचिन्हित> last_tag_idx_3 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_3;

      अगर (likely (tag_idx < ARRAY_लंबाई (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	अगर (*गिनती)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *गिनती = 1;
	}
	अन्यथा
	  *गिनती = 0;
	वापस;
      }

      अटल hb_atomic_t<अचिन्हित> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      अचिन्हित multi_tag_idx = last_tag_idx_3_multi;

      अगर (likely (multi_tag_idx < ARRAY_लंबाई (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	स्थिर LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	अचिन्हित पूर्णांक i;
	क्रम (i = 0; i < *गिनती && i < range.गिनती; i++)
	  tags[i] = ot_languages3_multi_मूल्यावली[range.offset + i];
	*गिनती = i;
	वापस;
      }

      अगर (bfind_tag (ot_languages3_blocked, ARRAY_लंबाई (ot_languages3_blocked), lang_tag))
      {
	*गिनती = 0;
	वापस;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *गिनती = 1;
      वापस;
    }
#पूर्ण_अगर
  }

#अगर_अघोषित HB_NO_LANGUAGE_LONG
  अगर (!s)
    s = lang_str + म_माप (lang_str);
  अगर (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *गिनती = 1;
    वापस;
  }
#पूर्ण_अगर

  *गिनती = 0;
}

अटल bool
parse_private_use_subtag (स्थिर अक्षर     *private_use_subtag,
			  अचिन्हित पूर्णांक   *गिनती,
			  hb_tag_t       *tags,
			  स्थिर अक्षर     *prefix,
			  अचिन्हित अक्षर (*normalize) (अचिन्हित अक्षर))
{
#अगर_घोषित HB_NO_LANGUAGE_PRIVATE_SUBTAG
  वापस false;
#पूर्ण_अगर

  अगर (!(private_use_subtag && गिनती && tags && *गिनती)) वापस false;

  स्थिर अक्षर *s = म_माला (private_use_subtag, prefix);
  अगर (!s) वापस false;

  अक्षर tag[4];
  पूर्णांक i;
  s += म_माप (prefix);
  अगर (s[0] == '-') {
    s += 1;
    अक्षर c;
    क्रम (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      अगर (i % 2 == 0)
	tag[i / 2] = c << 4;
      अन्यथा
	tag[i / 2] += c;
    }
    अगर (i != 8) वापस false;
  } अन्यथा {
    क्रम (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    अगर (!i) वापस false;

    क्रम (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  अगर ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *गिनती = 1;
  वापस true;
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
व्योम
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     अचिन्हित पूर्णांक *script_गिनती /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     अचिन्हित पूर्णांक *language_गिनती /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  bool needs_script = true;

  अगर (language == HB_LANGUAGE_INVALID)
  {
    अगर (language_गिनती && language_tags && *language_गिनती)
      *language_गिनती = 0;
  }
  अन्यथा
  {
    स्थिर अक्षर *lang_str, *s, *limit, *private_use_subtag;
    bool needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    अगर (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } अन्यथा {
      क्रम (s = lang_str + 1; *s; s++)
      {
	अगर (s[-1] == '-' && s[1] == '-')
	{
	  अगर (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    अगर (!limit)
	      limit = s - 1;
	    अवरोध;
	  } अन्यथा अगर (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      अगर (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_गिनती, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_गिनती, language_tags, "-hbot", TOUPPER);

    अगर (needs_language && language_गिनती && language_tags && *language_गिनती)
      hb_ot_tags_from_language (lang_str, limit, language_गिनती, language_tags);
  }

  अगर (needs_script && script_गिनती && script_tags && *script_गिनती)
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
  अचिन्हित पूर्णांक i;

  अगर (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    वापस nullptr;

#अगर_अघोषित HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    अगर (disambiguated_tag != HB_LANGUAGE_INVALID)
      वापस disambiguated_tag;
  }
#पूर्ण_अगर

  अक्षर buf[4];
  क्रम (i = 0; i < ARRAY_लंबाई (ot_languages2); i++)
    अगर (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      वापस hb_language_from_string (buf, 2);
    }
#अगर_अघोषित HB_NO_LANGUAGE_LONG
  क्रम (i = 0; i < ARRAY_लंबाई (ot_languages3); i++)
    अगर (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      वापस hb_language_from_string (buf, 3);
    }
  क्रम (i = 0; i < ARRAY_लंबाई (ot_languages3_multi); i++)
    क्रम (अचिन्हित पूर्णांक j = 0; j < ot_languages3_multi[i].गिनती; j++)
      अगर (ot_languages3_multi_मूल्यावली[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	वापस hb_language_from_string (buf, 3);
      }
#पूर्ण_अगर

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    अक्षर buf[20];
    अक्षर *str = buf;
    अगर (ISALPHA (tag >> 24)
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
    वापस hb_language_from_string (&*buf, -1);
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
व्योम
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  अगर (script)
    *script = script_out;
  अगर (language)
  {
    अचिन्हित पूर्णांक script_गिनती = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_गिनती,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    स्थिर अक्षर *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    अगर (lang_str && (script_गिनती == 0 || primary_script_tag[0] != script_tag))
    {
      अचिन्हित अक्षर *buf;
      माप_प्रकार len = म_माप (lang_str);
      buf = (अचिन्हित अक्षर *) hb_malloc (len + 16);
      अगर (unlikely (!buf))
      {
	*language = nullptr;
      }
      अन्यथा
      {
	पूर्णांक shift;
	hb_memcpy (buf, lang_str, len);
	अगर (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	क्रम (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((अक्षर *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#अगर_घोषित MAIN
अटल अंतरभूत व्योम
test_langs_sorted ()
{
  क्रम (अचिन्हित पूर्णांक i = 1; i < ARRAY_लंबाई (ot_languages2); i++)
  {
    पूर्णांक c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    अगर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      पात();
    }
  }
#अगर_अघोषित HB_NO_LANGUAGE_LONG
  क्रम (अचिन्हित पूर्णांक i = 1; i < ARRAY_लंबाई (ot_languages3); i++)
  {
    पूर्णांक c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    अगर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      पात();
    }
  }
  क्रम (अचिन्हित पूर्णांक i = 1; i < ARRAY_लंबाई (ot_languages3_multi); i++)
  {
    पूर्णांक c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    अगर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      पात();
    }
  }
  क्रम (अचिन्हित पूर्णांक i = 1; i < ARRAY_लंबाई (ot_languages3_blocked); i++)
    अगर (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      पात();
    }
#पूर्ण_अगर
}

पूर्णांक
मुख्य ()
{
  test_langs_sorted ();
  वापस 0;
}

#पूर्ण_अगर


#पूर्ण_अगर
