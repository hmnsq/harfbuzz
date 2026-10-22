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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

ସ୍ଥିର hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  ବାଛ ((hb_tag_t) script)
  {
    ସ୍ଥିତି HB_SCRIPT_INVALID:		ଫେରନ୍ତୁ HB_OT_TAG_DEFAULT_SCRIPT;
    ସ୍ଥିତି HB_SCRIPT_MATH:		ଫେରନ୍ତୁ HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    ସ୍ଥିତି HB_SCRIPT_HIRAGANA:		ଫେରନ୍ତୁ HB_TAG('k','a','n','a');
    ସ୍ଥିତି HB_TAG('H','r','k','t'):
					ଫେରନ୍ତୁ HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    ସ୍ଥିତି HB_SCRIPT_LAO:			ଫେରନ୍ତୁ HB_TAG('l','a','o',' ');
    ସ୍ଥିତି HB_SCRIPT_YI:			ଫେରନ୍ତୁ HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    ସ୍ଥିତି HB_SCRIPT_NKO:			ଫେରନ୍ତୁ HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    ସ୍ଥିତି HB_SCRIPT_VAI:			ଫେରନ୍ତୁ HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  ଫେରନ୍ତୁ ((hb_tag_t) script) | 0x20000000u;
}

ସ୍ଥିର hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  ଯଦି (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    ଫେରନ୍ତୁ HB_SCRIPT_INVALID;
  ଯଦି (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    ଫେରନ୍ତୁ HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  ଯଦି (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  ଯଦି (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  ଫେରନ୍ତୁ (hb_script_t) (tag & ~0x20000000u);
}

ସ୍ଥିର hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  ବାଛ ((hb_tag_t) script) {
    ସ୍ଥିତି HB_SCRIPT_BENGALI:		ଫେରନ୍ତୁ HB_TAG('b','n','g','2');
    ସ୍ଥିତି HB_SCRIPT_DEVANAGARI:		ଫେରନ୍ତୁ HB_TAG('d','e','v','2');
    ସ୍ଥିତି HB_SCRIPT_GUJARATI:		ଫେରନ୍ତୁ HB_TAG('g','j','r','2');
    ସ୍ଥିତି HB_SCRIPT_GURMUKHI:		ଫେରନ୍ତୁ HB_TAG('g','u','r','2');
    ସ୍ଥିତି HB_SCRIPT_KANNADA:		ଫେରନ୍ତୁ HB_TAG('k','n','d','2');
    ସ୍ଥିତି HB_SCRIPT_MALAYALAM:		ଫେରନ୍ତୁ HB_TAG('m','l','m','2');
    ସ୍ଥିତି HB_SCRIPT_ORIYA:		ଫେରନ୍ତୁ HB_TAG('o','r','y','2');
    ସ୍ଥିତି HB_SCRIPT_TAMIL:		ଫେରନ୍ତୁ HB_TAG('t','m','l','2');
    ସ୍ଥିତି HB_SCRIPT_TELUGU:		ଫେରନ୍ତୁ HB_TAG('t','e','l','2');
    ସ୍ଥିତି HB_SCRIPT_MYANMAR:		ଫେରନ୍ତୁ HB_TAG('m','y','m','2');
  }

  ଫେରନ୍ତୁ HB_OT_TAG_DEFAULT_SCRIPT;
}

ସ୍ଥିର hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  ବାଛ (tag) {
    ସ୍ଥିତି HB_TAG('b','n','g','2'):	ଫେରନ୍ତୁ HB_SCRIPT_BENGALI;
    ସ୍ଥିତି HB_TAG('d','e','v','2'):	ଫେରନ୍ତୁ HB_SCRIPT_DEVANAGARI;
    ସ୍ଥିତି HB_TAG('g','j','r','2'):	ଫେରନ୍ତୁ HB_SCRIPT_GUJARATI;
    ସ୍ଥିତି HB_TAG('g','u','r','2'):	ଫେରନ୍ତୁ HB_SCRIPT_GURMUKHI;
    ସ୍ଥିତି HB_TAG('k','n','d','2'):	ଫେରନ୍ତୁ HB_SCRIPT_KANNADA;
    ସ୍ଥିତି HB_TAG('m','l','m','2'):	ଫେରନ୍ତୁ HB_SCRIPT_MALAYALAM;
    ସ୍ଥିତି HB_TAG('o','r','y','2'):	ଫେରନ୍ତୁ HB_SCRIPT_ORIYA;
    ସ୍ଥିତି HB_TAG('t','m','l','2'):	ଫେରନ୍ତୁ HB_SCRIPT_TAMIL;
    ସ୍ଥିତି HB_TAG('t','e','l','2'):	ଫେରନ୍ତୁ HB_SCRIPT_TELUGU;
    ସ୍ଥିତି HB_TAG('m','y','m','2'):	ଫେରନ୍ତୁ HB_SCRIPT_MYANMAR;
  }

  ଫେରନ୍ତୁ HB_SCRIPT_UNKNOWN;
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
ଶୂନ୍ୟ
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &ଗଣନା_2, tags, nullptr, nullptr);
  *script_tag_1 = ଗଣନା_2 > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = ଗଣନା_2 > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ସ୍ଥିର ଶୂନ୍ୟ
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2 /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  ଯଦି (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    ଯଦି (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    ଯଦି (*ଗଣନା_2 > i)
      tags[i++] = new_tag;
  }

  ଯଦି (*ଗଣନା_2 > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    ଯଦି (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *ଗଣନା_2 = i;
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
  unsigned ଅକ୍ଷର digit = tag & 0x000000FFu;
  ଯଦି (unlikely (digit == '2' || digit == '3'))
    ଫେରନ୍ତୁ hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  ଫେରନ୍ତୁ hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
subtag_matches (ଅଚଳ ଅକ୍ଷର *lang_str,
		ଅଚଳ ଅକ୍ଷର *limit,
		ଅଚଳ ଅକ୍ଷର *subtag,
		unsigned    subtag_len)
{
  ଯଦି (likely ((unsigned) (limit - lang_str) < subtag_len))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  କର {
    ଅଚଳ ଅକ୍ଷର *s = strstr (lang_str, subtag);
    ଯଦି (!s || s >= limit)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!ISALNUM (s[subtag_len]))
      ଫେରନ୍ତୁ ସତ୍ୟ;
    lang_str = s + subtag_len;
  } ଯେତେବେଳେ (ସତ୍ୟ);
}

ସ୍ଥିର ତାର୍କିକ
lang_matches (ଅଚଳ ଅକ୍ଷର *lang_str,
	      ଅଚଳ ଅକ୍ଷର *limit,
	      ଅଚଳ ଅକ୍ଷର *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  ଯଦି (likely ((unsigned) (limit - lang_str) < spec_len))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଫେରନ୍ତୁ strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ସ୍ଥିର ତାର୍କିକ
bfind_tag (ଅଚଳ hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  ଯେତେବେଳେ (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    ଯଦି (key < val)
      max = mid;
    ଅନ୍ୟଥା ଯଦି (key > val)
      min = mid + 1;
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

ସଂରଚନା LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t a) ଅଚଳ
  {
    ଫେରନ୍ତୁ a < ଏହି->language ? -1 : a > ଏହି->language ? +1 : 0;
  }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ LangTag *that) ଅଚଳ
  { ଫେରନ୍ତୁ cmp (that->language); }
};

ସଂରଚନା LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t ଗଣନା_2;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_tag_t a) ଅଚଳ
  {
    ଫେରନ୍ତୁ a < ଏହି->language ? -1 : a > ଏହି->language ? +1 : 0;
  }
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ LangTagRange *that) ଅଚଳ
  { ଫେରନ୍ତୁ cmp (that->language); }
};

#ଅନ୍ତର୍ଭାଵ "hb-ot-tag-table.hh"

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
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &ଗଣନା_2, tags);
  ଫେରନ୍ତୁ ଗଣନା_2 > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ
hb_ot_tags_from_language (ଅଚଳ ଅକ୍ଷର   *lang_str,
			  ଅଚଳ ଅକ୍ଷର   *limit,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଗଣନା_2,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  ଯଦି (hb_ot_tags_from_complex_language (lang_str, limit, ଗଣନା_2, tags))
    ଫେରନ୍ତୁ;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  ଅଚଳ ଅକ୍ଷର *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    ଯଦି (s && limit - lang_str >= 6)
    {
      ଅଚଳ ଅକ୍ଷର *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      ଯଦି (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    ଅଚଳ ଅକ୍ଷର *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    ଯଦି (first_len == 2)
    {
      ସ୍ଥିର hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      ଯଦି (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଯେତେବେଳେ (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	ପାଇଁ (i = 0;
	     i < *ଗଣନା_2 &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*ଗଣନା_2 = i;
	ଫେରନ୍ତୁ;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    ଅନ୍ୟଥା ଯଦି (first_len == 3)
    {
      ସ୍ଥିର hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      ଯଦି (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	ଯଦି (*ଗଣନା_2)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *ଗଣନା_2 = 1;
	}
	ଅନ୍ୟଥା
	  *ଗଣନା_2 = 0;
	ଫେରନ୍ତୁ;
      }

      ସ୍ଥିର hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      ଯଦି (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	ଅଚଳ LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < *ଗଣନା_2 && i < range.ଗଣନା_2; i++)
	  tags[i] = ot_languages3_multi_ମୂଲ୍ୟଗୁଡ଼ିକ[range.offset + i];
	*ଗଣନା_2 = i;
	ଫେରନ୍ତୁ;
      }

      ଯଦି (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*ଗଣନା_2 = 0;
	ଫେରନ୍ତୁ;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *ଗଣନା_2 = 1;
      ଫେରନ୍ତୁ;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  ଯଦି (!s)
    s = lang_str + strlen (lang_str);
  ଯଦି (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *ଗଣନା_2 = 1;
    ଫେରନ୍ତୁ;
  }
#endif

  *ଗଣନା_2 = 0;
}

ସ୍ଥିର ତାର୍କିକ
parse_private_use_subtag (ଅଚଳ ଅକ୍ଷର     *private_use_subtag,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା   *ଗଣନା_2,
			  hb_tag_t       *tags,
			  ଅଚଳ ଅକ୍ଷର     *prefix,
			  unsigned ଅକ୍ଷର (*normalize) (unsigned ଅକ୍ଷର))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

  ଯଦି (!(private_use_subtag && ଗଣନା_2 && tags && *ଗଣନା_2)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅଚଳ ଅକ୍ଷର *s = strstr (private_use_subtag, prefix);
  ଯଦି (!s) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଅକ୍ଷର tag[4];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  s += strlen (prefix);
  ଯଦି (s[0] == '-') {
    s += 1;
    ଅକ୍ଷର c;
    ପାଇଁ (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      ଯଦି (i % 2 == 0)
	tag[i / 2] = c << 4;
      ଅନ୍ୟଥା
	tag[i / 2] += c;
    }
    ଯଦି (i != 8) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  } ଅନ୍ୟଥା {
    ପାଇଁ (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    ଯଦି (!i) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  ଯଦି ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *ଗଣନା_2 = 1;
  ଫେରନ୍ତୁ ସତ୍ୟ;
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
ଶୂନ୍ୟ
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *script_ଗଣନା /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *language_ଗଣନା /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  ତାର୍କିକ needs_script = ସତ୍ୟ;

  ଯଦି (language == HB_LANGUAGE_INVALID)
  {
    ଯଦି (language_ଗଣନା && language_tags && *language_ଗଣନା)
      *language_ଗଣନା = 0;
  }
  ଅନ୍ୟଥା
  {
    ଅଚଳ ଅକ୍ଷର *lang_str, *s, *limit, *private_use_subtag;
    ତାର୍କିକ needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    ଯଦି (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } ଅନ୍ୟଥା {
      ପାଇଁ (s = lang_str + 1; *s; s++)
      {
	ଯଦି (s[-1] == '-' && s[1] == '-')
	{
	  ଯଦି (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    ଯଦି (!limit)
	      limit = s - 1;
	    ଭାଙ୍ଗ;
	  } ଅନ୍ୟଥା ଯଦି (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      ଯଦି (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_ଗଣନା, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_ଗଣନା, language_tags, "-hbot", TOUPPER);

    ଯଦି (needs_language && language_ଗଣନା && language_tags && *language_ଗଣନା)
      hb_ot_tags_from_language (lang_str, limit, language_ଗଣନା, language_tags);
  }

  ଯଦି (needs_script && script_ଗଣନା && script_tags && *script_ଗଣନା)
    hb_ot_all_tags_from_script (script, script_ଗଣନା, script_tags);
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
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ଯଦି (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    ଫେରନ୍ତୁ nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    ଯଦି (disambiguated_tag != HB_LANGUAGE_INVALID)
      ଫେରନ୍ତୁ disambiguated_tag;
  }
#endif

  ଅକ୍ଷର buf[4];
  ପାଇଁ (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    ଯଦି (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      ଫେରନ୍ତୁ hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  ପାଇଁ (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    ଯଦି (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      ଫେରନ୍ତୁ hb_language_from_string (buf, 3);
    }
  ପାଇଁ (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ot_languages3_multi[i].ଗଣନା_2; j++)
      ଯଦି (ot_languages3_multi_ମୂଲ୍ୟଗୁଡ଼ିକ[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	ଫେରନ୍ତୁ hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    ଅକ୍ଷର buf[20];
    ଅକ୍ଷର *str = buf;
    ଯଦି (ISALPHA (tag >> 24)
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
    ଫେରନ୍ତୁ hb_language_from_string (&*buf, -1);
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
ଶୂନ୍ୟ
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  ଯଦି (script)
    *script = script_out;
  ଯଦି (language)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା script_ଗଣନା = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_ଗଣନା,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    ଅଚଳ ଅକ୍ଷର *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    ଯଦି (lang_str && (script_ଗଣନା == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned ଅକ୍ଷର *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned ଅକ୍ଷର *) hb_malloc (len + 16);
      ଯଦି (unlikely (!buf))
      {
	*language = nullptr;
      }
      ଅନ୍ୟଥା
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift;
	hb_memcpy (buf, lang_str, len);
	ଯଦି (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	ପାଇଁ (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((ଅକ୍ଷର *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
test_langs_sorted ()
{
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    ଯଦି (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    ଯଦି (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    ଯଦି (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    ଯଦି (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ ()
{
  test_langs_sorted ();
  ଫେରନ୍ତୁ 0;
}

#endif


#endif
