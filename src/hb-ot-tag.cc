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

  निवड ((hb_tag_t) script)
  {
    स्थिती HB_SCRIPT_INVALID:		परत HB_OT_TAG_DEFAULT_SCRIPT;
    स्थिती HB_SCRIPT_MATH:		परत HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    स्थिती HB_SCRIPT_HIRAGANA:		परत HB_TAG('k','a','n','a');
    स्थिती HB_TAG('H','r','k','t'):
					परत HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    स्थिती HB_SCRIPT_LAO:			परत HB_TAG('l','a','o',' ');
    स्थिती HB_SCRIPT_YI:			परत HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    स्थिती HB_SCRIPT_NKO:			परत HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    स्थिती HB_SCRIPT_VAI:			परत HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  परत ((hb_tag_t) script) | 0x20000000u;
}

स्थिर hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  जर (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    परत HB_SCRIPT_INVALID;
  जर (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    परत HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  जर (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  जर (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  परत (hb_script_t) (tag & ~0x20000000u);
}

स्थिर hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  निवड ((hb_tag_t) script) {
    स्थिती HB_SCRIPT_BENGALI:		परत HB_TAG('b','n','g','2');
    स्थिती HB_SCRIPT_DEVANAGARI:		परत HB_TAG('d','e','v','2');
    स्थिती HB_SCRIPT_GUJARATI:		परत HB_TAG('g','j','r','2');
    स्थिती HB_SCRIPT_GURMUKHI:		परत HB_TAG('g','u','r','2');
    स्थिती HB_SCRIPT_KANNADA:		परत HB_TAG('k','n','d','2');
    स्थिती HB_SCRIPT_MALAYALAM:		परत HB_TAG('m','l','m','2');
    स्थिती HB_SCRIPT_ORIYA:		परत HB_TAG('o','r','y','2');
    स्थिती HB_SCRIPT_TAMIL:		परत HB_TAG('t','m','l','2');
    स्थिती HB_SCRIPT_TELUGU:		परत HB_TAG('t','e','l','2');
    स्थिती HB_SCRIPT_MYANMAR:		परत HB_TAG('m','y','m','2');
  }

  परत HB_OT_TAG_DEFAULT_SCRIPT;
}

स्थिर hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  निवड (tag) {
    स्थिती HB_TAG('b','n','g','2'):	परत HB_SCRIPT_BENGALI;
    स्थिती HB_TAG('d','e','v','2'):	परत HB_SCRIPT_DEVANAGARI;
    स्थिती HB_TAG('g','j','r','2'):	परत HB_SCRIPT_GUJARATI;
    स्थिती HB_TAG('g','u','r','2'):	परत HB_SCRIPT_GURMUKHI;
    स्थिती HB_TAG('k','n','d','2'):	परत HB_SCRIPT_KANNADA;
    स्थिती HB_TAG('m','l','m','2'):	परत HB_SCRIPT_MALAYALAM;
    स्थिती HB_TAG('o','r','y','2'):	परत HB_SCRIPT_ORIYA;
    स्थिती HB_TAG('t','m','l','2'):	परत HB_SCRIPT_TAMIL;
    स्थिती HB_TAG('t','e','l','2'):	परत HB_SCRIPT_TELUGU;
    स्थिती HB_TAG('m','y','m','2'):	परत HB_SCRIPT_MYANMAR;
  }

  परत HB_SCRIPT_UNKNOWN;
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
  अचिन्हित पूर्णांक मोजणी = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &मोजणी, tags, nullptr, nullptr);
  *script_tag_1 = मोजणी > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = मोजणी > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
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
			    अचिन्हित पूर्णांक *मोजणी /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  अचिन्हित पूर्णांक i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  जर (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    जर (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    जर (*मोजणी > i)
      tags[i++] = new_tag;
  }

  जर (*मोजणी > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    जर (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *मोजणी = i;
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
  जर (unlikely (digit == '2' || digit == '3'))
    परत hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  परत hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

स्थिर ओळीत तार्किक
subtag_matches (स्थिरांक अक्षर *lang_str,
		स्थिरांक अक्षर *limit,
		स्थिरांक अक्षर *subtag,
		अचिन्हित    subtag_len)
{
  जर (likely ((अचिन्हित) (limit - lang_str) < subtag_len))
    परत असत्य;

  करा {
    स्थिरांक अक्षर *s = म_माला (lang_str, subtag);
    जर (!s || s >= limit)
      परत असत्य;
    जर (!ISALNUM (s[subtag_len]))
      परत सत्य;
    lang_str = s + subtag_len;
  } पर्यंत (सत्य);
}

स्थिर तार्किक
lang_matches (स्थिरांक अक्षर *lang_str,
	      स्थिरांक अक्षर *limit,
	      स्थिरांक अक्षर *spec,
	      अचिन्हित    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  जर (likely ((अचिन्हित) (limit - lang_str) < spec_len))
    परत असत्य;

  परत म_भेदन (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

स्थिर तार्किक
bfind_tag (स्थिरांक hb_tag_t *array,
	   अचिन्हित        len,
	   hb_tag_t        key)
{
  अचिन्हित min = 0;
  अचिन्हित max = len;

  पर्यंत (min < max)
  {
    अचिन्हित mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    जर (key < val)
      max = mid;
    अन्यथा जर (key > val)
      min = mid + 1;
    अन्यथा
      परत सत्य;
  }

  परत असत्य;
}

संरचना LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  पूर्णांक cmp (hb_tag_t a) स्थिरांक
  {
    परत a < हे->language ? -1 : a > हे->language ? +1 : 0;
  }
  पूर्णांक cmp (स्थिरांक LangTag *that) स्थिरांक
  { परत cmp (that->language); }
};

संरचना LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t मोजणी;

  पूर्णांक cmp (hb_tag_t a) स्थिरांक
  {
    परत a < हे->language ? -1 : a > हे->language ? +1 : 0;
  }
  पूर्णांक cmp (स्थिरांक LangTagRange *that) स्थिरांक
  { परत cmp (that->language); }
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
  अचिन्हित पूर्णांक मोजणी = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &मोजणी, tags);
  परत मोजणी > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

स्थिर शून्य
hb_ot_tags_from_language (स्थिरांक अक्षर   *lang_str,
			  स्थिरांक अक्षर   *limit,
			  अचिन्हित पूर्णांक *मोजणी,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  जर (hb_ot_tags_from_complex_language (lang_str, limit, मोजणी, tags))
    परत;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  स्थिरांक अक्षर *s; s = म_अक्षर (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    जर (s && limit - lang_str >= 6)
    {
      स्थिरांक अक्षर *extlang_end = म_अक्षर (s + 1, '-');
      /* If there is an extended language tag, use it. */
      जर (3 == (extlang_end ? extlang_end - s - 1 : म_माप (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    स्थिरांक अक्षर *dash = म_अक्षर (lang_str, '-');
    अचिन्हित first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    जर (first_len == 2)
    {
      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_2 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_2;

      जर (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	अचिन्हित पूर्णांक i;
	पर्यंत (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	साठी (i = 0;
	     i < *मोजणी &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*मोजणी = i;
	परत;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    अन्यथा जर (first_len == 3)
    {
      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_3 {0}; /* Poor man's cache. */
      अचिन्हित tag_idx = last_tag_idx_3;

      जर (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	जर (*मोजणी)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *मोजणी = 1;
	}
	अन्यथा
	  *मोजणी = 0;
	परत;
      }

      स्थिर hb_atomic_t<अचिन्हित> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      अचिन्हित multi_tag_idx = last_tag_idx_3_multi;

      जर (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	स्थिरांक LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	अचिन्हित पूर्णांक i;
	साठी (i = 0; i < *मोजणी && i < range.मोजणी; i++)
	  tags[i] = ot_languages3_multi_मूल्ये[range.offset + i];
	*मोजणी = i;
	परत;
      }

      जर (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*मोजणी = 0;
	परत;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *मोजणी = 1;
      परत;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  जर (!s)
    s = lang_str + म_माप (lang_str);
  जर (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *मोजणी = 1;
    परत;
  }
#endif

  *मोजणी = 0;
}

स्थिर तार्किक
parse_private_use_subtag (स्थिरांक अक्षर     *private_use_subtag,
			  अचिन्हित पूर्णांक   *मोजणी,
			  hb_tag_t       *tags,
			  स्थिरांक अक्षर     *prefix,
			  अचिन्हित अक्षर (*normalize) (अचिन्हित अक्षर))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  परत असत्य;
#endif

  जर (!(private_use_subtag && मोजणी && tags && *मोजणी)) परत असत्य;

  स्थिरांक अक्षर *s = म_माला (private_use_subtag, prefix);
  जर (!s) परत असत्य;

  अक्षर tag[4];
  पूर्णांक i;
  s += म_माप (prefix);
  जर (s[0] == '-') {
    s += 1;
    अक्षर c;
    साठी (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      जर (i % 2 == 0)
	tag[i / 2] = c << 4;
      अन्यथा
	tag[i / 2] += c;
    }
    जर (i != 8) परत असत्य;
  } अन्यथा {
    साठी (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    जर (!i) परत असत्य;

    साठी (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  जर ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *मोजणी = 1;
  परत सत्य;
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
				     अचिन्हित पूर्णांक *script_मोजणी /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     अचिन्हित पूर्णांक *language_मोजणी /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  तार्किक needs_script = सत्य;

  जर (language == HB_LANGUAGE_INVALID)
  {
    जर (language_मोजणी && language_tags && *language_मोजणी)
      *language_मोजणी = 0;
  }
  अन्यथा
  {
    स्थिरांक अक्षर *lang_str, *s, *limit, *private_use_subtag;
    तार्किक needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    जर (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } अन्यथा {
      साठी (s = lang_str + 1; *s; s++)
      {
	जर (s[-1] == '-' && s[1] == '-')
	{
	  जर (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    जर (!limit)
	      limit = s - 1;
	    थांबा;
	  } अन्यथा जर (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      जर (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_मोजणी, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_मोजणी, language_tags, "-hbot", TOUPPER);

    जर (needs_language && language_मोजणी && language_tags && *language_मोजणी)
      hb_ot_tags_from_language (lang_str, limit, language_मोजणी, language_tags);
  }

  जर (needs_script && script_मोजणी && script_tags && *script_मोजणी)
    hb_ot_all_tags_from_script (script, script_मोजणी, script_tags);
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

  जर (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    परत nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    जर (disambiguated_tag != HB_LANGUAGE_INVALID)
      परत disambiguated_tag;
  }
#endif

  अक्षर buf[4];
  साठी (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    जर (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      परत hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  साठी (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    जर (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      परत hb_language_from_string (buf, 3);
    }
  साठी (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    साठी (अचिन्हित पूर्णांक j = 0; j < ot_languages3_multi[i].मोजणी; j++)
      जर (ot_languages3_multi_मूल्ये[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	परत hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    अक्षर buf[20];
    अक्षर *str = buf;
    जर (ISALPHA (tag >> 24)
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
    परत hb_language_from_string (&*buf, -1);
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
  जर (script)
    *script = script_out;
  जर (language)
  {
    अचिन्हित पूर्णांक script_मोजणी = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_मोजणी,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    स्थिरांक अक्षर *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    जर (lang_str && (script_मोजणी == 0 || primary_script_tag[0] != script_tag))
    {
      अचिन्हित अक्षर *buf;
      माप_प्रकार len = म_माप (lang_str);
      buf = (अचिन्हित अक्षर *) hb_malloc (len + 16);
      जर (unlikely (!buf))
      {
	*language = nullptr;
      }
      अन्यथा
      {
	पूर्णांक shift;
	hb_memcpy (buf, lang_str, len);
	जर (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	साठी (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((अक्षर *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
स्थिर ओळीत शून्य
test_langs_sorted ()
{
  साठी (अचिन्हित पूर्णांक i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    पूर्णांक c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    जर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      पात();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  साठी (अचिन्हित पूर्णांक i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    पूर्णांक c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    जर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      पात();
    }
  }
  साठी (अचिन्हित पूर्णांक i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    पूर्णांक c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    जर (c > 0)
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      पात();
    }
  }
  साठी (अचिन्हित पूर्णांक i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    जर (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      ख_लिखो (मानक_त्रुटि, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      पात();
    }
#endif
}

पूर्णांक
main ()
{
  test_langs_sorted ();
  परत 0;
}

#endif


#endif
