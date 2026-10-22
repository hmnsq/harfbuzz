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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

ಸ್ಥಿರ hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  ವಿಕಲ್ಪ ((hb_tag_t) script)
  {
    ಪಕ್ಷ HB_SCRIPT_INVALID:		ಪ್ರತ್ಯಾಗಮ HB_OT_TAG_DEFAULT_SCRIPT;
    ಪಕ್ಷ HB_SCRIPT_MATH:		ಪ್ರತ್ಯಾಗಮ HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    ಪಕ್ಷ HB_SCRIPT_HIRAGANA:		ಪ್ರತ್ಯಾಗಮ HB_TAG('k','a','n','a');
    ಪಕ್ಷ HB_TAG('H','r','k','t'):
					ಪ್ರತ್ಯಾಗಮ HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    ಪಕ್ಷ HB_SCRIPT_LAO:			ಪ್ರತ್ಯಾಗಮ HB_TAG('l','a','o',' ');
    ಪಕ್ಷ HB_SCRIPT_YI:			ಪ್ರತ್ಯಾಗಮ HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    ಪಕ್ಷ HB_SCRIPT_NKO:			ಪ್ರತ್ಯಾಗಮ HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    ಪಕ್ಷ HB_SCRIPT_VAI:			ಪ್ರತ್ಯಾಗಮ HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  ಪ್ರತ್ಯಾಗಮ ((hb_tag_t) script) | 0x20000000u;
}

ಸ್ಥಿರ hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  ಯದಿ (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_INVALID;
  ಯದಿ (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  ಯದಿ (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  ಯದಿ (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  ಪ್ರತ್ಯಾಗಮ (hb_script_t) (tag & ~0x20000000u);
}

ಸ್ಥಿರ hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  ವಿಕಲ್ಪ ((hb_tag_t) script) {
    ಪಕ್ಷ HB_SCRIPT_BENGALI:		ಪ್ರತ್ಯಾಗಮ HB_TAG('b','n','g','2');
    ಪಕ್ಷ HB_SCRIPT_DEVANAGARI:		ಪ್ರತ್ಯಾಗಮ HB_TAG('d','e','v','2');
    ಪಕ್ಷ HB_SCRIPT_GUJARATI:		ಪ್ರತ್ಯಾಗಮ HB_TAG('g','j','r','2');
    ಪಕ್ಷ HB_SCRIPT_GURMUKHI:		ಪ್ರತ್ಯಾಗಮ HB_TAG('g','u','r','2');
    ಪಕ್ಷ HB_SCRIPT_KANNADA:		ಪ್ರತ್ಯಾಗಮ HB_TAG('k','n','d','2');
    ಪಕ್ಷ HB_SCRIPT_MALAYALAM:		ಪ್ರತ್ಯಾಗಮ HB_TAG('m','l','m','2');
    ಪಕ್ಷ HB_SCRIPT_ORIYA:		ಪ್ರತ್ಯಾಗಮ HB_TAG('o','r','y','2');
    ಪಕ್ಷ HB_SCRIPT_TAMIL:		ಪ್ರತ್ಯಾಗಮ HB_TAG('t','m','l','2');
    ಪಕ್ಷ HB_SCRIPT_TELUGU:		ಪ್ರತ್ಯಾಗಮ HB_TAG('t','e','l','2');
    ಪಕ್ಷ HB_SCRIPT_MYANMAR:		ಪ್ರತ್ಯಾಗಮ HB_TAG('m','y','m','2');
  }

  ಪ್ರತ್ಯಾಗಮ HB_OT_TAG_DEFAULT_SCRIPT;
}

ಸ್ಥಿರ hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  ವಿಕಲ್ಪ (tag) {
    ಪಕ್ಷ HB_TAG('b','n','g','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_BENGALI;
    ಪಕ್ಷ HB_TAG('d','e','v','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_DEVANAGARI;
    ಪಕ್ಷ HB_TAG('g','j','r','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_GUJARATI;
    ಪಕ್ಷ HB_TAG('g','u','r','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_GURMUKHI;
    ಪಕ್ಷ HB_TAG('k','n','d','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_KANNADA;
    ಪಕ್ಷ HB_TAG('m','l','m','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_MALAYALAM;
    ಪಕ್ಷ HB_TAG('o','r','y','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_ORIYA;
    ಪಕ್ಷ HB_TAG('t','m','l','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_TAMIL;
    ಪಕ್ಷ HB_TAG('t','e','l','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_TELUGU;
    ಪಕ್ಷ HB_TAG('m','y','m','2'):	ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_MYANMAR;
  }

  ಪ್ರತ್ಯಾಗಮ HB_SCRIPT_UNKNOWN;
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
ಶೂನ್ಯ
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &ಎಣಿಕೆ, tags, nullptr, nullptr);
  *script_tag_1 = ಎಣಿಕೆ > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = ಎಣಿಕೆ > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ಸ್ಥಿರ ಶೂನ್ಯ
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  ಯದಿ (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    ಯದಿ (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    ಯದಿ (*ಎಣಿಕೆ > i)
      tags[i++] = new_tag;
  }

  ಯದಿ (*ಎಣಿಕೆ > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    ಯದಿ (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *ಎಣಿಕೆ = i;
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
  unsigned ವರ್ಣ digit = tag & 0x000000FFu;
  ಯದಿ (unlikely (digit == '2' || digit == '3'))
    ಪ್ರತ್ಯಾಗಮ hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  ಪ್ರತ್ಯಾಗಮ hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
subtag_matches (ಧ್ರುವ ವರ್ಣ *lang_str,
		ಧ್ರುವ ವರ್ಣ *limit,
		ಧ್ರುವ ವರ್ಣ *subtag,
		unsigned    subtag_len)
{
  ಯದಿ (likely ((unsigned) (limit - lang_str) < subtag_len))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಕುರು {
    ಧ್ರುವ ವರ್ಣ *s = strstr (lang_str, subtag);
    ಯದಿ (!s || s >= limit)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!ISALNUM (s[subtag_len]))
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    lang_str = s + subtag_len;
  } ಯಾವತ್ (ಸತ್ಯ);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
lang_matches (ಧ್ರುವ ವರ್ಣ *lang_str,
	      ಧ್ರುವ ವರ್ಣ *limit,
	      ಧ್ರುವ ವರ್ಣ *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  ಯದಿ (likely ((unsigned) (limit - lang_str) < spec_len))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತ್ಯಾಗಮ strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
bfind_tag (ಧ್ರುವ hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  ಯಾವತ್ (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    ಯದಿ (key < val)
      max = mid;
    ಅನ್ಯಥಾ ಯದಿ (key > val)
      min = mid + 1;
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸಂರಚನಾ LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t a) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ a < ಏತತ್->language ? -1 : a > ಏತತ್->language ? +1 : 0;
  }
  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ LangTag *that) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ cmp (that->language); }
};

ಸಂರಚನಾ LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t ಎಣಿಕೆ;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_tag_t a) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ a < ಏತತ್->language ? -1 : a > ಏತತ್->language ? +1 : 0;
  }
  ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ LangTagRange *that) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ cmp (that->language); }
};

#ಅನ್ತರ್ಭಾವ "hb-ot-tag-table.hh"

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
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &ಎಣಿಕೆ, tags);
  ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ
hb_ot_tags_from_language (ಧ್ರುವ ವರ್ಣ   *lang_str,
			  ಧ್ರುವ ವರ್ಣ   *limit,
			  unsigned ಪೂರ್ಣಾಙ್ಕ *ಎಣಿಕೆ,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  ಯದಿ (hb_ot_tags_from_complex_language (lang_str, limit, ಎಣಿಕೆ, tags))
    ಪ್ರತ್ಯಾಗಮ;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  ಧ್ರುವ ವರ್ಣ *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    ಯದಿ (s && limit - lang_str >= 6)
    {
      ಧ್ರುವ ವರ್ಣ *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      ಯದಿ (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    ಧ್ರುವ ವರ್ಣ *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    ಯದಿ (first_len == 2)
    {
      ಸ್ಥಿರ hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      ಯದಿ (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಯಾವತ್ (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	ಪ್ರತಿ (i = 0;
	     i < *ಎಣಿಕೆ &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*ಎಣಿಕೆ = i;
	ಪ್ರತ್ಯಾಗಮ;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    ಅನ್ಯಥಾ ಯದಿ (first_len == 3)
    {
      ಸ್ಥಿರ hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      ಯದಿ (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	ಯದಿ (*ಎಣಿಕೆ)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *ಎಣಿಕೆ = 1;
	}
	ಅನ್ಯಥಾ
	  *ಎಣಿಕೆ = 0;
	ಪ್ರತ್ಯಾಗಮ;
      }

      ಸ್ಥಿರ hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      ಯದಿ (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	ಧ್ರುವ LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	ಪ್ರತಿ (i = 0; i < *ಎಣಿಕೆ && i < range.ಎಣಿಕೆ; i++)
	  tags[i] = ot_languages3_multi_ಮೌಲ್ಯಗಳು[range.offset + i];
	*ಎಣಿಕೆ = i;
	ಪ್ರತ್ಯಾಗಮ;
      }

      ಯದಿ (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*ಎಣಿಕೆ = 0;
	ಪ್ರತ್ಯಾಗಮ;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *ಎಣಿಕೆ = 1;
      ಪ್ರತ್ಯಾಗಮ;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  ಯದಿ (!s)
    s = lang_str + strlen (lang_str);
  ಯದಿ (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *ಎಣಿಕೆ = 1;
    ಪ್ರತ್ಯಾಗಮ;
  }
#endif

  *ಎಣಿಕೆ = 0;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
parse_private_use_subtag (ಧ್ರುವ ವರ್ಣ     *private_use_subtag,
			  unsigned ಪೂರ್ಣಾಙ್ಕ   *ಎಣಿಕೆ,
			  hb_tag_t       *tags,
			  ಧ್ರುವ ವರ್ಣ     *prefix,
			  unsigned ವರ್ಣ (*normalize) (unsigned ವರ್ಣ))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif

  ಯದಿ (!(private_use_subtag && ಎಣಿಕೆ && tags && *ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಧ್ರುವ ವರ್ಣ *s = strstr (private_use_subtag, prefix);
  ಯದಿ (!s) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ವರ್ಣ tag[4];
  ಪೂರ್ಣಾಙ್ಕ i;
  s += strlen (prefix);
  ಯದಿ (s[0] == '-') {
    s += 1;
    ವರ್ಣ c;
    ಪ್ರತಿ (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      ಯದಿ (i % 2 == 0)
	tag[i / 2] = c << 4;
      ಅನ್ಯಥಾ
	tag[i / 2] += c;
    }
    ಯದಿ (i != 8) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  } ಅನ್ಯಥಾ {
    ಪ್ರತಿ (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    ಯದಿ (!i) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  ಯದಿ ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *ಎಣಿಕೆ = 1;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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
ಶೂನ್ಯ
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned ಪೂರ್ಣಾಙ್ಕ *script_ಎಣಿಕೆ /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned ಪೂರ್ಣಾಙ್ಕ *language_ಎಣಿಕೆ /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  ತಾರ್ಕಿಕ needs_script = ಸತ್ಯ;

  ಯದಿ (language == HB_LANGUAGE_INVALID)
  {
    ಯದಿ (language_ಎಣಿಕೆ && language_tags && *language_ಎಣಿಕೆ)
      *language_ಎಣಿಕೆ = 0;
  }
  ಅನ್ಯಥಾ
  {
    ಧ್ರುವ ವರ್ಣ *lang_str, *s, *limit, *private_use_subtag;
    ತಾರ್ಕಿಕ needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    ಯದಿ (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } ಅನ್ಯಥಾ {
      ಪ್ರತಿ (s = lang_str + 1; *s; s++)
      {
	ಯದಿ (s[-1] == '-' && s[1] == '-')
	{
	  ಯದಿ (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    ಯದಿ (!limit)
	      limit = s - 1;
	    ಭಙ್ಗ;
	  } ಅನ್ಯಥಾ ಯದಿ (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      ಯದಿ (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_ಎಣಿಕೆ, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_ಎಣಿಕೆ, language_tags, "-hbot", TOUPPER);

    ಯದಿ (needs_language && language_ಎಣಿಕೆ && language_tags && *language_ಎಣಿಕೆ)
      hb_ot_tags_from_language (lang_str, limit, language_ಎಣಿಕೆ, language_tags);
  }

  ಯದಿ (needs_script && script_ಎಣಿಕೆ && script_tags && *script_ಎಣಿಕೆ)
    hb_ot_all_tags_from_script (script, script_ಎಣಿಕೆ, script_tags);
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
  unsigned ಪೂರ್ಣಾಙ್ಕ i;

  ಯದಿ (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    ಪ್ರತ್ಯಾಗಮ nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    ಯದಿ (disambiguated_tag != HB_LANGUAGE_INVALID)
      ಪ್ರತ್ಯಾಗಮ disambiguated_tag;
  }
#endif

  ವರ್ಣ buf[4];
  ಪ್ರತಿ (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    ಯದಿ (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      ಪ್ರತ್ಯಾಗಮ hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  ಪ್ರತಿ (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    ಯದಿ (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      ಪ್ರತ್ಯಾಗಮ hb_language_from_string (buf, 3);
    }
  ಪ್ರತಿ (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < ot_languages3_multi[i].ಎಣಿಕೆ; j++)
      ಯದಿ (ot_languages3_multi_ಮೌಲ್ಯಗಳು[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	ಪ್ರತ್ಯಾಗಮ hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    ವರ್ಣ buf[20];
    ವರ್ಣ *str = buf;
    ಯದಿ (ISALPHA (tag >> 24)
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
    ಪ್ರತ್ಯಾಗಮ hb_language_from_string (&*buf, -1);
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
ಶೂನ್ಯ
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  ಯದಿ (script)
    *script = script_out;
  ಯದಿ (language)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ script_ಎಣಿಕೆ = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_ಎಣಿಕೆ,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    ಧ್ರುವ ವರ್ಣ *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    ಯದಿ (lang_str && (script_ಎಣಿಕೆ == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned ವರ್ಣ *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned ವರ್ಣ *) hb_malloc (len + 16);
      ಯದಿ (unlikely (!buf))
      {
	*language = nullptr;
      }
      ಅನ್ಯಥಾ
      {
	ಪೂರ್ಣಾಙ್ಕ shift;
	hb_memcpy (buf, lang_str, len);
	ಯದಿ (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	ಪ್ರತಿ (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((ವರ್ಣ *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
test_langs_sorted ()
{
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    ಪೂರ್ಣಾಙ್ಕ c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    ಯದಿ (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    ಪೂರ್ಣಾಙ್ಕ c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    ಯದಿ (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    ಪೂರ್ಣಾಙ್ಕ c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    ಯದಿ (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    ಯದಿ (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

ಪೂರ್ಣಾಙ್ಕ
main ()
{
  test_langs_sorted ();
  ಪ್ರತ್ಯಾಗಮ 0;
}

#endif


#endif
