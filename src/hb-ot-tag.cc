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
    पक्ष HB_SCRIPT_INVALID:		फर्काऊ HB_OT_TAG_DEFAULT_SCRIPT;
    पक्ष HB_SCRIPT_MATH:		फर्काऊ HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    पक्ष HB_SCRIPT_HIRAGANA:		फर्काऊ HB_TAG('k','a','n','a');
    पक्ष HB_TAG('H','r','k','t'):
					फर्काऊ HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    पक्ष HB_SCRIPT_LAO:			फर्काऊ HB_TAG('l','a','o',' ');
    पक्ष HB_SCRIPT_YI:			फर्काऊ HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    पक्ष HB_SCRIPT_NKO:			फर्काऊ HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    पक्ष HB_SCRIPT_VAI:			फर्काऊ HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  फर्काऊ ((hb_tag_t) script) | 0x20000000u;
}

स्थिर hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  यदि (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    फर्काऊ HB_SCRIPT_INVALID;
  यदि (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    फर्काऊ HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  यदि (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  यदि (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  फर्काऊ (hb_script_t) (tag & ~0x20000000u);
}

स्थिर hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  विकल्प ((hb_tag_t) script) {
    पक्ष HB_SCRIPT_BENGALI:		फर्काऊ HB_TAG('b','n','g','2');
    पक्ष HB_SCRIPT_DEVANAGARI:		फर्काऊ HB_TAG('d','e','v','2');
    पक्ष HB_SCRIPT_GUJARATI:		फर्काऊ HB_TAG('g','j','r','2');
    पक्ष HB_SCRIPT_GURMUKHI:		फर्काऊ HB_TAG('g','u','r','2');
    पक्ष HB_SCRIPT_KANNADA:		फर्काऊ HB_TAG('k','n','d','2');
    पक्ष HB_SCRIPT_MALAYALAM:		फर्काऊ HB_TAG('m','l','m','2');
    पक्ष HB_SCRIPT_ORIYA:		फर्काऊ HB_TAG('o','r','y','2');
    पक्ष HB_SCRIPT_TAMIL:		फर्काऊ HB_TAG('t','m','l','2');
    पक्ष HB_SCRIPT_TELUGU:		फर्काऊ HB_TAG('t','e','l','2');
    पक्ष HB_SCRIPT_MYANMAR:		फर्काऊ HB_TAG('m','y','m','2');
  }

  फर्काऊ HB_OT_TAG_DEFAULT_SCRIPT;
}

स्थिर hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  विकल्प (tag) {
    पक्ष HB_TAG('b','n','g','2'):	फर्काऊ HB_SCRIPT_BENGALI;
    पक्ष HB_TAG('d','e','v','2'):	फर्काऊ HB_SCRIPT_DEVANAGARI;
    पक्ष HB_TAG('g','j','r','2'):	फर्काऊ HB_SCRIPT_GUJARATI;
    पक्ष HB_TAG('g','u','r','2'):	फर्काऊ HB_SCRIPT_GURMUKHI;
    पक्ष HB_TAG('k','n','d','2'):	फर्काऊ HB_SCRIPT_KANNADA;
    पक्ष HB_TAG('m','l','m','2'):	फर्काऊ HB_SCRIPT_MALAYALAM;
    पक्ष HB_TAG('o','r','y','2'):	फर्काऊ HB_SCRIPT_ORIYA;
    पक्ष HB_TAG('t','m','l','2'):	फर्काऊ HB_SCRIPT_TAMIL;
    पक्ष HB_TAG('t','e','l','2'):	फर्काऊ HB_SCRIPT_TELUGU;
    पक्ष HB_TAG('m','y','m','2'):	फर्काऊ HB_SCRIPT_MYANMAR;
  }

  फर्काऊ HB_SCRIPT_UNKNOWN;
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
  अचिन्हित पूर्णाङ्क गन्ती = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &गन्ती, tags, nullptr, nullptr);
  *script_tag_1 = गन्ती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = गन्ती > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
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
			    अचिन्हित पूर्णाङ्क *गन्ती /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  अचिन्हित पूर्णाङ्क i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  यदि (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    यदि (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    यदि (*गन्ती > i)
      tags[i++] = new_tag;
  }

  यदि (*गन्ती > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    यदि (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *गन्ती = i;
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
  अचिन्हित वर्ण digit = tag & 0x000000FFu;
  यदि (unlikely (digit == '2' || digit == '3'))
    फर्काऊ hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  फर्काऊ hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

स्थिर अन्तःस्थ तार्किक
subtag_matches (अचर वर्ण *lang_str,
		अचर वर्ण *limit,
		अचर वर्ण *subtag,
		अचिन्हित    subtag_len)
{
  यदि (likely ((अचिन्हित) (limit - lang_str) < subtag_len))
    फर्काऊ असत्य;

  गर {
    अचर वर्ण *s = म_माला (lang_str, subtag);
    यदि (!s || s >= limit)
      फर्काऊ असत्य;
    यदि (!ISALNUM (s[subtag_len]))
      फर्काऊ सत्य;
    lang_str = s + subtag_len;
  } जबसम्म (सत्य);
}

स्थिर तार्किक
lang_matches (अचर वर्ण *lang_str,
	      अचर वर्ण *limit,
	      अचर वर्ण *spec,
	      अचिन्हित    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  यदि (likely ((अचिन्हित) (limit - lang_str) < spec_len))
    फर्काऊ असत्य;

  फर्काऊ म_भेदन (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

स्थिर तार्किक
bfind_tag (अचर hb_tag_t *array,
	   अचिन्हित        len,
	   hb_tag_t        key)
{
  अचिन्हित min = 0;
  अचिन्हित max = len;

  जबसम्म (min < max)
  {
    अचिन्हित mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    यदि (key < val)
      max = mid;
    अन्यथा यदि (key > val)
      min = mid + 1;
    अन्यथा
      फर्काऊ सत्य;
  }

  फर्काऊ असत्य;
}

संरचना LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  पूर्णाङ्क cmp (hb_tag_t a) अचर
  {
    फर्काऊ a < एतत्->language ? -1 : a > एतत्->language ? +1 : 0;
  }
  पूर्णाङ्क cmp (अचर LangTag *that) अचर
  { फर्काऊ cmp (that->language); }
};

संरचना LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t गन्ती;

  पूर्णाङ्क cmp (hb_tag_t a) अचर
  {
    फर्काऊ a < एतत्->language ? -1 : a > एतत्->language ? +1 : 0;
  }
  पूर्णाङ्क cmp (अचर LangTagRange *that) अचर
  { फर्काऊ cmp (that->language); }
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
  अचिन्हित पूर्णाङ्क गन्ती = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &गन्ती, tags);
  फर्काऊ गन्ती > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

स्थिर शून्य
hb_ot_tags_from_language (अचर वर्ण   *lang_str,
			  अचर वर्ण   *limit,
			  अचिन्हित पूर्णाङ्क *गन्ती,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  यदि (hb_ot_tags_from_complex_language (lang_str, limit, गन्ती, tags))
    फर्काऊ;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  अचर वर्ण *s; s = म_अक्षर (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    यदि (s && limit - lang_str >= 6)
    {
      अचर वर्ण *extlang_end = म_अक्षर (s + 1, '-');
      /* If there is an extended language tag, use it. */
      यदि (3 == (extlang_end ? extlang_end - s - 1 : म_माप (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    अचर वर्ण *dash = म_अक्षर (lang_str, '-');
    अचिन्हित first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    यदि (first_len == 2)
    {
      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_2 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_2;

      यदि (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	अचिन्हित पूर्णाङ्क i;
	जबसम्म (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	लागि (i = 0;
	     i < *गन्ती &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*गन्ती = i;
	फर्काऊ;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    अन्यथा यदि (first_len == 3)
    {
      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_3 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_3;

      यदि (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	यदि (*गन्ती)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *गन्ती = 1;
	}
	अन्यथा
	  *गन्ती = 0;
	फर्काऊ;
      }

      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      अचिन्हित multi_tag_idx = last_tag_idx_3_multi;

      यदि (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	अचर LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	अचिन्हित पूर्णाङ्क i;
	लागि (i = 0; i < *गन्ती && i < range.गन्ती; i++)
	  tags[i] = ot_languages3_multi_मानहरू[range.offset + i];
	*गन्ती = i;
	फर्काऊ;
      }

      यदि (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*गन्ती = 0;
	फर्काऊ;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *गन्ती = 1;
      फर्काऊ;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  यदि (!s)
    s = lang_str + म_माप (lang_str);
  यदि (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *गन्ती = 1;
    फर्काऊ;
  }
#endif

  *गन्ती = 0;
}

स्थिर तार्किक
parse_private_use_subtag (अचर वर्ण     *private_use_subtag,
			  अचिन्हित पूर्णाङ्क   *गन्ती,
			  hb_tag_t       *tags,
			  अचर वर्ण     *prefix,
			  अचिन्हित वर्ण (*normalize) (अचिन्हित वर्ण))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  फर्काऊ असत्य;
#endif

  यदि (!(private_use_subtag && गन्ती && tags && *गन्ती)) फर्काऊ असत्य;

  अचर वर्ण *s = म_माला (private_use_subtag, prefix);
  यदि (!s) फर्काऊ असत्य;

  वर्ण tag[4];
  पूर्णाङ्क i;
  s += म_माप (prefix);
  यदि (s[0] == '-') {
    s += 1;
    वर्ण c;
    लागि (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      यदि (i % 2 == 0)
	tag[i / 2] = c << 4;
      अन्यथा
	tag[i / 2] += c;
    }
    यदि (i != 8) फर्काऊ असत्य;
  } अन्यथा {
    लागि (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    यदि (!i) फर्काऊ असत्य;

    लागि (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  यदि ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *गन्ती = 1;
  फर्काऊ सत्य;
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
				     अचिन्हित पूर्णाङ्क *script_गन्ती /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     अचिन्हित पूर्णाङ्क *language_गन्ती /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  तार्किक needs_script = सत्य;

  यदि (language == HB_LANGUAGE_INVALID)
  {
    यदि (language_गन्ती && language_tags && *language_गन्ती)
      *language_गन्ती = 0;
  }
  अन्यथा
  {
    अचर वर्ण *lang_str, *s, *limit, *private_use_subtag;
    तार्किक needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    यदि (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } अन्यथा {
      लागि (s = lang_str + 1; *s; s++)
      {
	यदि (s[-1] == '-' && s[1] == '-')
	{
	  यदि (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    यदि (!limit)
	      limit = s - 1;
	    रोक;
	  } अन्यथा यदि (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      यदि (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_गन्ती, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_गन्ती, language_tags, "-hbot", TOUPPER);

    यदि (needs_language && language_गन्ती && language_tags && *language_गन्ती)
      hb_ot_tags_from_language (lang_str, limit, language_गन्ती, language_tags);
  }

  यदि (needs_script && script_गन्ती && script_tags && *script_गन्ती)
    hb_ot_all_tags_from_script (script, script_गन्ती, script_tags);
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
  अचिन्हित पूर्णाङ्क i;

  यदि (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    फर्काऊ nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    यदि (disambiguated_tag != HB_LANGUAGE_INVALID)
      फर्काऊ disambiguated_tag;
  }
#endif

  वर्ण buf[4];
  लागि (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    यदि (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      फर्काऊ hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  लागि (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    यदि (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      फर्काऊ hb_language_from_string (buf, 3);
    }
  लागि (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    लागि (अचिन्हित पूर्णाङ्क j = 0; j < ot_languages3_multi[i].गन्ती; j++)
      यदि (ot_languages3_multi_मानहरू[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	फर्काऊ hb_language_from_string (buf, 3);
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
    फर्काऊ hb_language_from_string (&*buf, -1);
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
    अचिन्हित पूर्णाङ्क script_गन्ती = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_गन्ती,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    अचर वर्ण *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    यदि (lang_str && (script_गन्ती == 0 || primary_script_tag[0] != script_tag))
    {
      अचिन्हित वर्ण *buf;
      माप_प्रकार len = म_माप (lang_str);
      buf = (अचिन्हित वर्ण *) hb_malloc (len + 16);
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
	लागि (shift = 28; shift >= 0; shift -= 4)
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
  लागि (अचिन्हित पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    पूर्णाङ्क c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    यदि (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      पात();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  लागि (अचिन्हित पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    पूर्णाङ्क c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    यदि (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      पात();
    }
  }
  लागि (अचिन्हित पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    पूर्णाङ्क c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    यदि (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      पात();
    }
  }
  लागि (अचिन्हित पूर्णाङ्क i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    यदि (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      पात();
    }
#endif
}

पूर्णाङ्क
main ()
{
  test_langs_sorted ();
  फर्काऊ 0;
}

#endif


#endif
