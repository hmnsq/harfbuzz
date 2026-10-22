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

#ضمن "hb.hh"

#ifndef HB_NO_OT_TAG


/* hb_script_t */

ساكن hb_tag_t
hb_ot_old_tag_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  بدل ((hb_tag_t) script)
  {
    حالة HB_SCRIPT_INVALID:		إرجاع HB_OT_TAG_DEFAULT_SCRIPT;
    حالة HB_SCRIPT_MATH:		إرجاع HB_OT_TAG_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالة HB_SCRIPT_HIRAGANA:		إرجاع HB_TAG('k','a','n','a');
    حالة HB_TAG('H','r','k','t'):
					إرجاع HB_TAG('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالة HB_SCRIPT_LAO:			إرجاع HB_TAG('l','a','o',' ');
    حالة HB_SCRIPT_YI:			إرجاع HB_TAG('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_NKO:			إرجاع HB_TAG('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_VAI:			إرجاع HB_TAG('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  إرجاع ((hb_tag_t) script) | 0x20000000u;
}

ساكن hb_script_t
hb_ot_old_tag_to_script (hb_tag_t tag)
{
  إذا (unlikely (tag == HB_OT_TAG_DEFAULT_SCRIPT))
    إرجاع HB_SCRIPT_INVALID;
  إذا (unlikely (tag == HB_OT_TAG_MATH_SCRIPT))
    إرجاع HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  إذا (unlikely ((tag & 0x0000FF00u) == 0x00002000u))
    tag |= (tag >> 8) & 0x0000FF00u; /* Copy second letter to third */
  إذا (unlikely ((tag & 0x000000FFu) == 0x00000020u))
    tag |= (tag >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  إرجاع (hb_script_t) (tag & ~0x20000000u);
}

ساكن hb_tag_t
hb_ot_new_tag_from_script (hb_script_t script)
{
  بدل ((hb_tag_t) script) {
    حالة HB_SCRIPT_BENGALI:		إرجاع HB_TAG('b','n','g','2');
    حالة HB_SCRIPT_DEVANAGARI:		إرجاع HB_TAG('d','e','v','2');
    حالة HB_SCRIPT_GUJARATI:		إرجاع HB_TAG('g','j','r','2');
    حالة HB_SCRIPT_GURMUKHI:		إرجاع HB_TAG('g','u','r','2');
    حالة HB_SCRIPT_KANNADA:		إرجاع HB_TAG('k','n','d','2');
    حالة HB_SCRIPT_MALAYALAM:		إرجاع HB_TAG('m','l','m','2');
    حالة HB_SCRIPT_ORIYA:		إرجاع HB_TAG('o','r','y','2');
    حالة HB_SCRIPT_TAMIL:		إرجاع HB_TAG('t','m','l','2');
    حالة HB_SCRIPT_TELUGU:		إرجاع HB_TAG('t','e','l','2');
    حالة HB_SCRIPT_MYANMAR:		إرجاع HB_TAG('m','y','m','2');
  }

  إرجاع HB_OT_TAG_DEFAULT_SCRIPT;
}

ساكن hb_script_t
hb_ot_new_tag_to_script (hb_tag_t tag)
{
  بدل (tag) {
    حالة HB_TAG('b','n','g','2'):	إرجاع HB_SCRIPT_BENGALI;
    حالة HB_TAG('d','e','v','2'):	إرجاع HB_SCRIPT_DEVANAGARI;
    حالة HB_TAG('g','j','r','2'):	إرجاع HB_SCRIPT_GUJARATI;
    حالة HB_TAG('g','u','r','2'):	إرجاع HB_SCRIPT_GURMUKHI;
    حالة HB_TAG('k','n','d','2'):	إرجاع HB_SCRIPT_KANNADA;
    حالة HB_TAG('m','l','m','2'):	إرجاع HB_SCRIPT_MALAYALAM;
    حالة HB_TAG('o','r','y','2'):	إرجاع HB_SCRIPT_ORIYA;
    حالة HB_TAG('t','m','l','2'):	إرجاع HB_SCRIPT_TAMIL;
    حالة HB_TAG('t','e','l','2'):	إرجاع HB_SCRIPT_TELUGU;
    حالة HB_TAG('m','y','m','2'):	إرجاع HB_SCRIPT_MYANMAR;
  }

  إرجاع HB_SCRIPT_UNKNOWN;
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
فراغ
hb_ot_tags_from_script (hb_script_t  script,
			hb_tag_t    *script_tag_1,
			hb_tag_t    *script_tag_2)
{
  unsigned صحيح تعداد_2 = 2;
  hb_tag_t tags[2];
  hb_ot_tags_from_script_and_language (script, HB_LANGUAGE_INVALID, &تعداد_2, tags, nullptr, nullptr);
  *script_tag_1 = تعداد_2 > 0 ? tags[0] : HB_OT_TAG_DEFAULT_SCRIPT;
  *script_tag_2 = تعداد_2 > 1 ? tags[1] : HB_OT_TAG_DEFAULT_SCRIPT;
}
#endif

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساكن فراغ
hb_ot_all_tags_from_script (hb_script_t   script,
			    unsigned صحيح *تعداد_2 /* IN/OUT */,
			    hb_tag_t     *tags /* OUT */)
{
  unsigned صحيح i = 0;

  hb_tag_t new_tag = hb_ot_new_tag_from_script (script);
  إذا (unlikely (new_tag != HB_OT_TAG_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    إذا (new_tag != HB_TAG('m','y','m','2'))
      tags[i++] = new_tag | '3';
    إذا (*تعداد_2 > i)
      tags[i++] = new_tag;
  }

  إذا (*تعداد_2 > i)
  {
    hb_tag_t old_tag = hb_ot_old_tag_from_script (script);
    إذا (old_tag != HB_OT_TAG_DEFAULT_SCRIPT)
      tags[i++] = old_tag;
  }

  *تعداد_2 = i;
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
  unsigned حرف digit = tag & 0x000000FFu;
  إذا (unlikely (digit == '2' || digit == '3'))
    إرجاع hb_ot_new_tag_to_script (tag & 0xFFFFFF32);

  إرجاع hb_ot_old_tag_to_script (tag);
}


/* hb_language_t */

ساكن مضمن منطقي
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *limit,
		ثابت حرف *subtag,
		unsigned    subtag_len)
{
  إذا (likely ((unsigned) (limit - lang_str) < subtag_len))
    إرجاع خطأ;

  افعل {
    ثابت حرف *s = strstr (lang_str, subtag);
    إذا (!s || s >= limit)
      إرجاع خطأ;
    إذا (!ISALNUM (s[subtag_len]))
      إرجاع true;
    lang_str = s + subtag_len;
  } بينما (true);
}

ساكن منطقي
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *limit,
	      ثابت حرف *spec,
	      unsigned    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  إذا (likely ((unsigned) (limit - lang_str) < spec_len))
    إرجاع خطأ;

  إرجاع strncmp (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساكن منطقي
bfind_tag (ثابت hb_tag_t *array,
	   unsigned        len,
	   hb_tag_t        key)
{
  unsigned min = 0;
  unsigned max = len;

  بينما (min < max)
  {
    unsigned mid = min + (max - min) / 2;
    hb_tag_t val = array[mid];
    إذا (key < val)
      max = mid;
    وإلا إذا (key > val)
      min = mid + 1;
    وإلا
      إرجاع true;
  }

  إرجاع خطأ;
}

بنية LangTag
{
  hb_tag_t language;
  hb_tag_t tag;

  صحيح cmp (hb_tag_t a) ثابت
  {
    إرجاع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت LangTag *that) ثابت
  { إرجاع cmp (that->language); }
};

بنية LangTagRange
{
  hb_tag_t language;
  uint16_t offset;
  uint8_t تعداد_2;

  صحيح cmp (hb_tag_t a) ثابت
  {
    إرجاع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت LangTagRange *that) ثابت
  { إرجاع cmp (that->language); }
};

#ضمن "hb-ot-tag-table.hh"

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
  unsigned صحيح تعداد_2 = 1;
  hb_tag_t tags[1];
  hb_ot_tags_from_script_and_language (HB_SCRIPT_UNKNOWN, language, nullptr, nullptr, &تعداد_2, tags);
  إرجاع تعداد_2 > 0 ? tags[0] : HB_OT_TAG_DEFAULT_LANGUAGE;
}
#endif

ساكن فراغ
hb_ot_tags_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *limit,
			  unsigned صحيح *تعداد_2,
			  hb_tag_t     *tags)
{

#ifndef HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  إذا (hb_ot_tags_from_complex_language (lang_str, limit, تعداد_2, tags))
    إرجاع;
#endif

  /* Find a language matching in the first component. */
#ifndef HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#endif
  {
#ifndef HB_NO_LANGUAGE_LONG
    إذا (s && limit - lang_str >= 6)
    {
      ثابت حرف *extlang_end = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      إذا (3 == (extlang_end ? extlang_end - s - 1 : strlen (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#endif
    ثابت حرف *dash = strchr (lang_str, '-');
    unsigned first_len = dash ? dash - lang_str : limit - lang_str;
    hb_tag_t lang_tag = hb_tag_from_string (lang_str, first_len);

    إذا (first_len == 2)
    {
      ساكن hb_atomic_t<unsigned> last_tag_idx_2 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_2;

      إذا (likely (tag_idx < ARRAY_LENGTH (ot_languages2) &&
		  ot_languages2[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages2).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_2 = tag_idx;
	unsigned صحيح i;
	بينما (tag_idx != 0 &&
	       ot_languages2[tag_idx].language == ot_languages2[tag_idx - 1].language)
	  tag_idx--;
	لكل (i = 0;
	     i < *تعداد_2 &&
	     tag_idx + i < ARRAY_LENGTH (ot_languages2) &&
	     ot_languages2[tag_idx + i].tag != HB_TAG_NONE &&
	     ot_languages2[tag_idx + i].language == ot_languages2[tag_idx].language;
	     i++)
	  tags[i] = ot_languages2[tag_idx + i].tag;
	*تعداد_2 = i;
	إرجاع;
      }
    }
#ifndef HB_NO_LANGUAGE_LONG
    وإلا إذا (first_len == 3)
    {
      ساكن hb_atomic_t<unsigned> last_tag_idx_3 {0}; /* Poor man's cache. */
      unsigned tag_idx = last_tag_idx_3;

      إذا (likely (tag_idx < ARRAY_LENGTH (ot_languages3) &&
		  ot_languages3[tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3).bfind (lang_tag, &tag_idx))
      {
	last_tag_idx_3 = tag_idx;
	إذا (*تعداد_2)
	{
	  tags[0] = ot_languages3[tag_idx].tag;
	  *تعداد_2 = 1;
	}
	وإلا
	  *تعداد_2 = 0;
	إرجاع;
      }

      ساكن hb_atomic_t<unsigned> last_tag_idx_3_multi {0}; /* Poor man's cache. */
      unsigned multi_tag_idx = last_tag_idx_3_multi;

      إذا (likely (multi_tag_idx < ARRAY_LENGTH (ot_languages3_multi) &&
		  ot_languages3_multi[multi_tag_idx].language == lang_tag) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_tag, &multi_tag_idx))
      {
	last_tag_idx_3_multi = multi_tag_idx;
	ثابت LangTagRange &range = ot_languages3_multi[multi_tag_idx];
	unsigned صحيح i;
	لكل (i = 0; i < *تعداد_2 && i < range.تعداد_2; i++)
	  tags[i] = ot_languages3_multi_قيم[range.offset + i];
	*تعداد_2 = i;
	إرجاع;
      }

      إذا (bfind_tag (ot_languages3_blocked, ARRAY_LENGTH (ot_languages3_blocked), lang_tag))
      {
	*تعداد_2 = 0;
	إرجاع;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      tags[0] = lang_tag & ~0x20202000u;
      *تعداد_2 = 1;
      إرجاع;
    }
#endif
  }

#ifndef HB_NO_LANGUAGE_LONG
  إذا (!s)
    s = lang_str + strlen (lang_str);
  إذا (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    tags[0] = hb_tag_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *تعداد_2 = 1;
    إرجاع;
  }
#endif

  *تعداد_2 = 0;
}

ساكن منطقي
parse_private_use_subtag (ثابت حرف     *private_use_subtag,
			  unsigned صحيح   *تعداد_2,
			  hb_tag_t       *tags,
			  ثابت حرف     *prefix,
			  unsigned حرف (*normalize) (unsigned حرف))
{
#ifdef HB_NO_LANGUAGE_PRIVATE_SUBTAG
  إرجاع خطأ;
#endif

  إذا (!(private_use_subtag && تعداد_2 && tags && *تعداد_2)) إرجاع خطأ;

  ثابت حرف *s = strstr (private_use_subtag, prefix);
  إذا (!s) إرجاع خطأ;

  حرف tag[4];
  صحيح i;
  s += strlen (prefix);
  إذا (s[0] == '-') {
    s += 1;
    حرف c;
    لكل (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      إذا (i % 2 == 0)
	tag[i / 2] = c << 4;
      وإلا
	tag[i / 2] += c;
    }
    إذا (i != 8) إرجاع خطأ;
  } وإلا {
    لكل (i = 0; i < 4 && ISALNUM (s[i]); i++)
      tag[i] = normalize (s[i]);
    إذا (!i) إرجاع خطأ;

    لكل (; i < 4; i++)
      tag[i] = ' ';
  }
  tags[0] = HB_TAG (tag[0], tag[1], tag[2], tag[3]);
  إذا ((tags[0] & 0xDFDFDFDF) == HB_OT_TAG_DEFAULT_SCRIPT)
    tags[0] ^= ~0xDFDFDFDF;
  *تعداد_2 = 1;
  إرجاع true;
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
فراغ
hb_ot_tags_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     unsigned صحيح *script_تعداد /* IN/OUT */,
				     hb_tag_t     *script_tags /* OUT */,
				     unsigned صحيح *language_تعداد /* IN/OUT */,
				     hb_tag_t     *language_tags /* OUT */)
{
  منطقي needs_script = true;

  إذا (language == HB_LANGUAGE_INVALID)
  {
    إذا (language_تعداد && language_tags && *language_تعداد)
      *language_تعداد = 0;
  }
  وإلا
  {
    ثابت حرف *lang_str, *s, *limit, *private_use_subtag;
    منطقي needs_language;

    lang_str = hb_language_to_string (language);
    limit = nullptr;
    private_use_subtag = nullptr;
    إذا (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      private_use_subtag = lang_str;
    } وإلا {
      لكل (s = lang_str + 1; *s; s++)
      {
	إذا (s[-1] == '-' && s[1] == '-')
	{
	  إذا (s[0] == 'x')
	  {
	    private_use_subtag = s;
	    إذا (!limit)
	      limit = s - 1;
	    اكسر;
	  } وإلا إذا (!limit)
	  {
	    limit = s - 1;
	  }
	}
      }
      إذا (!limit)
	limit = s;
    }

    needs_script = !parse_private_use_subtag (private_use_subtag, script_تعداد, script_tags, "-hbsc", TOLOWER);
    needs_language = !parse_private_use_subtag (private_use_subtag, language_تعداد, language_tags, "-hbot", TOUPPER);

    إذا (needs_language && language_تعداد && language_tags && *language_تعداد)
      hb_ot_tags_from_language (lang_str, limit, language_تعداد, language_tags);
  }

  إذا (needs_script && script_تعداد && script_tags && *script_تعداد)
    hb_ot_all_tags_from_script (script, script_تعداد, script_tags);
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
  unsigned صحيح i;

  إذا (tag == HB_OT_TAG_DEFAULT_LANGUAGE)
    إرجاع nullptr;

#ifndef HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_tag = hb_ot_ambiguous_tag_to_language (tag);
    إذا (disambiguated_tag != HB_LANGUAGE_INVALID)
      إرجاع disambiguated_tag;
  }
#endif

  حرف buf[4];
  لكل (i = 0; i < ARRAY_LENGTH (ot_languages2); i++)
    إذا (ot_languages2[i].tag == tag)
    {
      hb_tag_to_string (ot_languages2[i].language, buf);
      إرجاع hb_language_from_string (buf, 2);
    }
#ifndef HB_NO_LANGUAGE_LONG
  لكل (i = 0; i < ARRAY_LENGTH (ot_languages3); i++)
    إذا (ot_languages3[i].tag == tag)
    {
      hb_tag_to_string (ot_languages3[i].language, buf);
      إرجاع hb_language_from_string (buf, 3);
    }
  لكل (i = 0; i < ARRAY_LENGTH (ot_languages3_multi); i++)
    لكل (unsigned صحيح j = 0; j < ot_languages3_multi[i].تعداد_2; j++)
      إذا (ot_languages3_multi_قيم[ot_languages3_multi[i].offset + j] == tag)
      {
	hb_tag_to_string (ot_languages3_multi[i].language, buf);
	إرجاع hb_language_from_string (buf, 3);
      }
#endif

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    إذا (ISALPHA (tag >> 24)
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
    إرجاع hb_language_from_string (&*buf, -1);
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
فراغ
hb_ot_tags_to_script_and_language (hb_tag_t       script_tag,
				   hb_tag_t       language_tag,
				   hb_script_t   *script /* OUT */,
				   hb_language_t *language /* OUT */)
{
  hb_script_t script_out = hb_ot_tag_to_script (script_tag);
  إذا (script)
    *script = script_out;
  إذا (language)
  {
    unsigned صحيح script_تعداد = 1;
    hb_tag_t primary_script_tag[1];
    hb_ot_tags_from_script_and_language (script_out,
					 HB_LANGUAGE_INVALID,
					 &script_تعداد,
					 primary_script_tag,
					 nullptr, nullptr);
    *language = hb_ot_tag_to_language (language_tag);
    ثابت حرف *lang_str = *language ? hb_language_to_string (*language) : nullptr;
    إذا (lang_str && (script_تعداد == 0 || primary_script_tag[0] != script_tag))
    {
      unsigned حرف *buf;
      size_t len = strlen (lang_str);
      buf = (unsigned حرف *) hb_malloc (len + 16);
      إذا (unlikely (!buf))
      {
	*language = nullptr;
      }
      وإلا
      {
	صحيح shift;
	hb_memcpy (buf, lang_str, len);
	إذا (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	لكل (shift = 28; shift >= 0; shift -= 4)
	  buf[len++] = TOHEX (script_tag >> shift);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ifdef MAIN
ساكن مضمن فراغ
test_langs_sorted ()
{
  لكل (unsigned صحيح i = 1; i < ARRAY_LENGTH (ot_languages2); i++)
  {
    صحيح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ifndef HB_NO_LANGUAGE_LONG
  لكل (unsigned صحيح i = 1; i < ARRAY_LENGTH (ot_languages3); i++)
  {
    صحيح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  لكل (unsigned صحيح i = 1; i < ARRAY_LENGTH (ot_languages3_multi); i++)
  {
    صحيح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  لكل (unsigned صحيح i = 1; i < ARRAY_LENGTH (ot_languages3_blocked); i++)
    إذا (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#endif
}

صحيح
الرئيسية ()
{
  test_langs_sorted ();
  إرجاع 0;
}

#endif


#endif
