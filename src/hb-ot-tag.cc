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

#অন্তর্ভাব "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

স্থির hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  নির্বাচন ((hb_tag_t) script)
  {
    ক্ষেত্র HB_SCRIPT_INVALID:		ফেরত HB_OT_TAG_DEFAULT_SCRIPT;
    ক্ষেত্র HB_SCRIPT_MATH:		ফেরত HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    ক্ষেত্র HB_SCRIPT_HIRAGANA:		ফেরত HB_TAG('k','a','n','a');
    ক্ষেত্র HB_TAG('H','r','k','t'):
					ফেরত HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    ক্ষেত্র HB_SCRIPT_LAO:			ফেরত HB_TAG('l','a','o',' ');
    ক্ষেত্র HB_SCRIPT_YI:			ফেরত HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    ক্ষেত্র HB_SCRIPT_NKO:			ফেরত HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    ক্ষেত্র HB_SCRIPT_VAI:			ফেরত HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  ফেরত ((hb_tag_t) script) | 0x20000000u;
}

স্থির hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  যদি (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    ফেরত HB_SCRIPT_INVALID;
  যদি (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    ফেরত HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  যদি (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  যদি (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  ফেরত (hb_script_t) (tag & ~0x20000000u);
}

স্থির hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  নির্বাচন ((hb_tag_t) script) {
    ক্ষেত্র HB_SCRIPT_BENGALI:		ফেরত HB_TAG('b','n','g','2');
    ক্ষেত্র HB_SCRIPT_DEVANAGARI:		ফেরত HB_TAG('d','e','v','2');
    ক্ষেত্র HB_SCRIPT_GUJARATI:		ফেরত HB_TAG('g','j','r','2');
    ক্ষেত্র HB_SCRIPT_GURMUKHI:		ফেরত HB_TAG('g','u','r','2');
    ক্ষেত্র HB_SCRIPT_KANNADA:		ফেরত HB_TAG('k','n','d','2');
    ক্ষেত্র HB_SCRIPT_MALAYALAM:		ফেরত HB_TAG('m','l','m','2');
    ক্ষেত্র HB_SCRIPT_ORIYA:		ফেরত HB_TAG('o','r','y','2');
    ক্ষেত্র HB_SCRIPT_TAMIL:		ফেরত HB_TAG('t','m','l','2');
    ক্ষেত্র HB_SCRIPT_TELUGU:		ফেরত HB_TAG('t','e','l','2');
    ক্ষেত্র HB_SCRIPT_MYANMAR:		ফেরত HB_TAG('m','y','m','2');
  }

  ফেরত HB_OT_TAG_DEFAULT_SCRIPT;
}

স্থির hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  নির্বাচন (tag) {
    ক্ষেত্র HB_TAG('b','n','g','2'):	ফেরত HB_SCRIPT_BENGALI;
    ক্ষেত্র HB_TAG('d','e','v','2'):	ফেরত HB_SCRIPT_DEVANAGARI;
    ক্ষেত্র HB_TAG('g','j','r','2'):	ফেরত HB_SCRIPT_GUJARATI;
    ক্ষেত্র HB_TAG('g','u','r','2'):	ফেরত HB_SCRIPT_GURMUKHI;
    ক্ষেত্র HB_TAG('k','n','d','2'):	ফেরত HB_SCRIPT_KANNADA;
    ক্ষেত্র HB_TAG('m','l','m','2'):	ফেরত HB_SCRIPT_MALAYALAM;
    ক্ষেত্র HB_TAG('o','r','y','2'):	ফেরত HB_SCRIPT_ORIYA;
    ক্ষেত্র HB_TAG('t','m','l','2'):	ফেরত HB_SCRIPT_TAMIL;
    ক্ষেত্র HB_TAG('t','e','l','2'):	ফেরত HB_SCRIPT_TELUGU;
    ক্ষেত্র HB_TAG('m','y','m','2'):	ফেরত HB_SCRIPT_MYANMAR;
  }

  ফেরত HB_SCRIPT_UNKNOWN;
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
শূন্য
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned পূর্ণসংখ্যা গুনতি = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &গুনতি, tags, nullptr, nullptr);
  *script_tag_1 = গুনতি > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = গুনতি > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

স্থির শূন্য
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned পূর্ণসংখ্যা *গুনতি /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned পূর্ণসংখ্যা i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  যদি (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    যদি (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    যদি (*গুনতি > i)
      tags[i++] = new_tag;
  }

  যদি (*গুনতি > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    যদি (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *গুনতি = i;
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
  unsigned অক্ষর digit = tag & 0x000000FFu;
  যদি (unlikely (digit == '2' || digit == '3'))
    ফেরত hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  ফেরত hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

স্থির অন্তঃরেখ যৌক্তিক
subtag_matches (ধ্রুবক অক্ষর *lang_str,
		ধ্রুবক অক্ষর *limit,
		ধ্রুবক অক্ষর *subtag,
		unsigned    subtag_len)
{
  যদি (likely ((unsigned) (limit - lang_str) < subtag_len))
    ফেরত মিথ্যা;

  করো {
    ধ্রুবক অক্ষর *s = strstr (lang_str, subtag);
    যদি (!s || s >= limit)
      ফেরত মিথ্যা;
    যদি (!ISALNUM (s[subtag_len]))
      ফেরত সত্য;
    lang_str = s + subtag_len;
  } যখন (সত্য);
}

স্থির যৌক্তিক
lang_matches (ধ্রুবক অক্ষর *lang_str,
	      ধ্রুবক অক্ষর *limit,
	      ধ্রুবক অক্ষর *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  যদি (likely ((unsigned) (limit - lang_str) < spec_len))
    ফেরত মিথ্যা;

  ফেরত strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

স্থির যৌক্তিক
bfind_tag (ধ্রুবক hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  যখন (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    যদি (key < val)
      max = mid;
    অন্যথা যদি (key > val)
      min = mid + 1;
    অন্যথা
      ফেরত সত্য;
  }

  ফেরত মিথ্যা;
}

কাঠামো LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  পূর্ণসংখ্যা cmp (hb_tag_t a) ধ্রুবক
  {
    ফেরত a < এই->language ? -1 : a > এই->language ? +1 : 0;
  }
  পূর্ণসংখ্যা cmp (ধ্রুবক LangTag *that) ধ্রুবক
  { ফেরত cmp (that->language); }
};

কাঠামো LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t গুনতি;

  পূর্ণসংখ্যা cmp (hb_tag_t a) ধ্রুবক
  {
    ফেরত a < এই->language ? -1 : a > এই->language ? +1 : 0;
  }
  পূর্ণসংখ্যা cmp (ধ্রুবক LangTagRange *that) ধ্রুবক
  { ফেরত cmp (that->language); }
};

#অন্তর্ভাব "hb-ot-tag-table.hh"

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
  unsigned পূর্ণসংখ্যা গুনতি = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &গুনতি, tags);
  ফেরত গুনতি > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

স্থির শূন্য
hb_ot_tags_from_language (ধ্রুবক অক্ষর   *lang_str,
			  ধ্রুবক অক্ষর   *limit,
			  unsigned পূর্ণসংখ্যা *গুনতি,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  যদি (hb_ot_tags_from_complex_language (lang_str, limit, গুনতি, tags))
    ফেরত;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  ধ্রুবক অক্ষর *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    যদি (s && limit - lang_str >= 6)
    {
      ধ্রুবক অক্ষর *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      যদি (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    ধ্রুবক অক্ষর *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    যদি (first_len == 2)
    {
      স্থির hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      যদি (likely (tag_idx < ARRAY_দৈর্ঘ্য (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned পূর্ণসংখ্যা i;
	যখন (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	জন্য (i = 0;
	     i < *গুনতি &&
	     tag_idx + i < ARRAY_দৈর্ঘ্য (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*গুনতি = i;
	ফেরত;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    অন্যথা যদি (first_len == 3)
    {
      স্থির hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      যদি (likely (tag_idx < ARRAY_দৈর্ঘ্য (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	যদি (*গুনতি)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *গুনতি = 1;
	}
	অন্যথা
	  *গুনতি = 0;
	ফেরত;
      }

      স্থির hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      যদি (likely (multi_tag_idx < ARRAY_দৈর্ঘ্য (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	ধ্রুবক LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned পূর্ণসংখ্যা i;
	জন্য (i = 0; i < *গুনতি && i < range.গুনতি; i++)
	  tags[i] = ot_languages3_multi_মানসমূহ[range.offset + i];
	*গুনতি = i;
	ফেরত;
      }

      যদি (bfind_tag (ot_languages3_blocked, ARRAY_দৈর্ঘ্য (ot_languages3_blocked), lang_tag))
      {
	*গুনতি = 0;
	ফেরত;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *গুনতি = 1;
      ফেরত;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  যদি (!s)
    s = lang_str + strlen (lang_str);
  যদি (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *গুনতি = 1;
    ফেরত;
  }
#endif

  *গুনতি = 0;
}

স্থির যৌক্তিক
parse_private_use_subtag (ধ্রুবক অক্ষর     *private_use_subtag,
			  unsigned পূর্ণসংখ্যা   *গুনতি,
			  hb_tag_t       *tags,
			  ধ্রুবক অক্ষর     *prefix,
			  unsigned অক্ষর (*normalize) (unsigned অক্ষর))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  ফেরত মিথ্যা;
#endif

  যদি (!(private_use_subtag && গুনতি && tags && *গুনতি)) ফেরত মিথ্যা;

  ধ্রুবক অক্ষর *s = strstr (private_use_subtag, prefix);
  যদি (!s) ফেরত মিথ্যা;

  অক্ষর tag[4];
  পূর্ণসংখ্যা i;
  s += strlen (prefix);
  যদি (s[0] == '-') {
    s += 1;
    অক্ষর c;
    জন্য (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      যদি (i % 2 == 0)
	tag[i / 2] = c << 4;
      অন্যথা
	tag[i / 2] += c;
    }
    যদি (i != 8) ফেরত মিথ্যা;
  } অন্যথা {
    জন্য (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    যদি (!i) ফেরত মিথ্যা;

    জন্য (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  যদি ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *গুনতি = 1;
  ফেরত সত্য;
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
শূন্য
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned পূর্ণসংখ্যা *script_গুনতি /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned পূর্ণসংখ্যা *language_গুনতি /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  যৌক্তিক needs_script = সত্য;

  যদি (language == HB_LANGUAGE_INVALID)
  {
    যদি (language_গুনতি && language_tags && *language_গুনতি)
      *language_গুনতি = 0;
  }
  অন্যথা
  {
    ধ্রুবক অক্ষর *lang_str, *s, *limit, *private_use_subtag;
    যৌক্তিক needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    যদি (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } অন্যথা {
      জন্য (s = lang_str + 1; *s; s++)
      {
	যদি (s[-1] == '-' && s[1] == '-')
	{
	  যদি (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    যদি (!limit)
	      limit = s - 1;
	    বিরতি;
	  } অন্যথা যদি (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      যদি (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_গুনতি, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_গুনতি, language_tags, "-hbot", TOUPPER);

    যদি (needs_language && language_গুনতি && language_tags && *language_গুনতি)
      hb_ot_tags_from_language (lang_str, limit, language_গুনতি, language_tags);
  }

  যদি (needs_script && script_গুনতি && script_tags && *script_গুনতি)
    hb_ot_all_tags_from_script (script, script_গুনতি, script_tags);
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
  unsigned পূর্ণসংখ্যা i;

  যদি (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    ফেরত nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    যদি (disambiguated_tag != HB_LANGUAGE_INVALID)
      ফেরত disambiguated_tag;
  }
#endif

  অক্ষর buf[4];
  জন্য (i = 0; i < ARRAY_দৈর্ঘ্য (ot_languages2); i++)
    যদি (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      ফেরত hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  জন্য (i = 0; i < ARRAY_দৈর্ঘ্য (ot_languages3); i++)
    যদি (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      ফেরত hb_language_from_string (buf, 3);
    }
  জন্য (i = 0; i < ARRAY_দৈর্ঘ্য (ot_languages3_multi); i++)
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < ot_languages3_multi[i].গুনতি; j++)
      যদি (ot_languages3_multi_মানসমূহ[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	ফেরত hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    অক্ষর buf[20];
    অক্ষর *str = buf;
    যদি (ISALPHA (tag >> 24)
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
    ফেরত hb_language_from_string (&*buf, -1);
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
শূন্য
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  যদি (script)
    *script = script_out;
  যদি (language)
  {
    unsigned পূর্ণসংখ্যা script_গুনতি = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_গুনতি,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    ধ্রুবক অক্ষর *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    যদি (lang_str && (script_গুনতি == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned অক্ষর *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned অক্ষর *) hb_malloc (len + 16);
      যদি (unlikely (!buf))
      {
	*language = nullptr;
      }
      অন্যথা
      {
	পূর্ণসংখ্যা shift;
	hb_memcpy (buf, lang_str, len);
	যদি (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	জন্য (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((অক্ষর *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
স্থির অন্তঃরেখ শূন্য
test_langs_sorted ()
{
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < ARRAY_দৈর্ঘ্য (ot_languages2); i++)
  {
    পূর্ণসংখ্যা c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    যদি (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < ARRAY_দৈর্ঘ্য (ot_languages3); i++)
  {
    পূর্ণসংখ্যা c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    যদি (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < ARRAY_দৈর্ঘ্য (ot_languages3_multi); i++)
  {
    পূর্ণসংখ্যা c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    যদি (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < ARRAY_দৈর্ঘ্য (ot_languages3_blocked); i++)
    যদি (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

পূর্ণসংখ্যা
প্রধান ()
{
  test_langs_sorted ();
  ফেরত 0;
}

#endif


#endif
