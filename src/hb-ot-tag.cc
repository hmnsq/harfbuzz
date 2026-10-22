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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_ਟੈਗ


/* hb_script_t */

ਸਥਿਰ hb_ਟੈਗ_t
hb_ot_old_ਟੈਗ_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  ਚੋਣ ((hb_ਟੈਗ_t) script)
  {
    ਹਾਲਤ HB_SCRIPT_INVALID:		ਵਾਪਸ HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
    ਹਾਲਤ HB_SCRIPT_MATH:		ਵਾਪਸ HB_OT_ਟੈਗ_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    ਹਾਲਤ HB_SCRIPT_HIRAGANA:		ਵਾਪਸ HB_ਟੈਗ('k','a','n','a');
    ਹਾਲਤ HB_ਟੈਗ('H','r','k','t'):
					ਵਾਪਸ HB_ਟੈਗ('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    ਹਾਲਤ HB_SCRIPT_LAO:			ਵਾਪਸ HB_ਟੈਗ('l','a','o',' ');
    ਹਾਲਤ HB_SCRIPT_YI:			ਵਾਪਸ HB_ਟੈਗ('y','i',' ',' ');
    /* Unicode-5.0 additions */
    ਹਾਲਤ HB_SCRIPT_NKO:			ਵਾਪਸ HB_ਟੈਗ('n','k','o',' ');
    /* Unicode-5.1 additions */
    ਹਾਲਤ HB_SCRIPT_VAI:			ਵਾਪਸ HB_ਟੈਗ('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  ਵਾਪਸ ((hb_ਟੈਗ_t) script) | 0x20000000u;
}

ਸਥਿਰ hb_script_t
hb_ot_old_ਟੈਗ_ਮੰਜ਼ਿਲ_script (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਜੇ (unlikely (ਟੈਗ_2 == HB_OT_ਟੈਗ_DEFAULT_SCRIPT))
    ਵਾਪਸ HB_SCRIPT_INVALID;
  ਜੇ (unlikely (ਟੈਗ_2 == HB_OT_ਟੈਗ_MATH_SCRIPT))
    ਵਾਪਸ HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  ਜੇ (unlikely ((ਟੈਗ_2 & 0x0000FF00u) == 0x00002000u))
    ਟੈਗ_2 |= (ਟੈਗ_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  ਜੇ (unlikely ((ਟੈਗ_2 & 0x000000FFu) == 0x00000020u))
    ਟੈਗ_2 |= (ਟੈਗ_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  ਵਾਪਸ (hb_script_t) (ਟੈਗ_2 & ~0x20000000u);
}

ਸਥਿਰ hb_ਟੈਗ_t
hb_ot_new_ਟੈਗ_from_script (hb_script_t script)
{
  ਚੋਣ ((hb_ਟੈਗ_t) script) {
    ਹਾਲਤ HB_SCRIPT_BENGALI:		ਵਾਪਸ HB_ਟੈਗ('b','n','g','2');
    ਹਾਲਤ HB_SCRIPT_DEVANAGARI:		ਵਾਪਸ HB_ਟੈਗ('d','e','v','2');
    ਹਾਲਤ HB_SCRIPT_GUJARATI:		ਵਾਪਸ HB_ਟੈਗ('g','j','r','2');
    ਹਾਲਤ HB_SCRIPT_GURMUKHI:		ਵਾਪਸ HB_ਟੈਗ('g','u','r','2');
    ਹਾਲਤ HB_SCRIPT_KANNADA:		ਵਾਪਸ HB_ਟੈਗ('k','n','d','2');
    ਹਾਲਤ HB_SCRIPT_MALAYALAM:		ਵਾਪਸ HB_ਟੈਗ('m','l','m','2');
    ਹਾਲਤ HB_SCRIPT_ORIYA:		ਵਾਪਸ HB_ਟੈਗ('o','r','y','2');
    ਹਾਲਤ HB_SCRIPT_TAMIL:		ਵਾਪਸ HB_ਟੈਗ('t','m','l','2');
    ਹਾਲਤ HB_SCRIPT_TELUGU:		ਵਾਪਸ HB_ਟੈਗ('t','e','l','2');
    ਹਾਲਤ HB_SCRIPT_MYANMAR:		ਵਾਪਸ HB_ਟੈਗ('m','y','m','2');
  }

  ਵਾਪਸ HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
}

ਸਥਿਰ hb_script_t
hb_ot_new_ਟੈਗ_ਮੰਜ਼ਿਲ_script (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਚੋਣ (ਟੈਗ_2) {
    ਹਾਲਤ HB_ਟੈਗ('b','n','g','2'):	ਵਾਪਸ HB_SCRIPT_BENGALI;
    ਹਾਲਤ HB_ਟੈਗ('d','e','v','2'):	ਵਾਪਸ HB_SCRIPT_DEVANAGARI;
    ਹਾਲਤ HB_ਟੈਗ('g','j','r','2'):	ਵਾਪਸ HB_SCRIPT_GUJARATI;
    ਹਾਲਤ HB_ਟੈਗ('g','u','r','2'):	ਵਾਪਸ HB_SCRIPT_GURMUKHI;
    ਹਾਲਤ HB_ਟੈਗ('k','n','d','2'):	ਵਾਪਸ HB_SCRIPT_KANNADA;
    ਹਾਲਤ HB_ਟੈਗ('m','l','m','2'):	ਵਾਪਸ HB_SCRIPT_MALAYALAM;
    ਹਾਲਤ HB_ਟੈਗ('o','r','y','2'):	ਵਾਪਸ HB_SCRIPT_ORIYA;
    ਹਾਲਤ HB_ਟੈਗ('t','m','l','2'):	ਵਾਪਸ HB_SCRIPT_TAMIL;
    ਹਾਲਤ HB_ਟੈਗ('t','e','l','2'):	ਵਾਪਸ HB_SCRIPT_TELUGU;
    ਹਾਲਤ HB_ਟੈਗ('m','y','m','2'):	ਵਾਪਸ HB_SCRIPT_MYANMAR;
  }

  ਵਾਪਸ HB_SCRIPT_UNKNOWN;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
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
ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_from_script (hb_script_t  script,
			hb_ਟੈਗ_t    *script_ਟੈਗ_1,
			hb_ਟੈਗ_t    *script_ਟੈਗ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 2;
  hb_ਟੈਗ_t ਟੈਗ_ਸੂਚੀ[2];
  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (script, HB_LANGUAGE_INVALID, &ਗਿਣਤੀ_2, ਟੈਗ_ਸੂਚੀ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  *script_ਟੈਗ_1 = ਗਿਣਤੀ_2 > 0 ? ਟੈਗ_ਸੂਚੀ[0] : HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
  *script_ਟੈਗ_2 = ਗਿਣਤੀ_2 > 1 ? ਟੈਗ_ਸੂਚੀ[1] : HB_OT_ਟੈਗ_DEFAULT_SCRIPT;
}
#ਜੇ_ਅੰਤ

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ਸਥਿਰ ਖਾਲੀ
hb_ot_all_ਟੈਗ_ਸੂਚੀ_from_script (hb_script_t   script,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2 /* IN/OUT */,
			    hb_ਟੈਗ_t     *ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;

  hb_ਟੈਗ_t new_ਟੈਗ = hb_ot_new_ਟੈਗ_from_script (script);
  ਜੇ (unlikely (new_ਟੈਗ != HB_OT_ਟੈਗ_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    ਜੇ (new_ਟੈਗ != HB_ਟੈਗ('m','y','m','2'))
      ਟੈਗ_ਸੂਚੀ[i++] = new_ਟੈਗ | '3';
    ਜੇ (*ਗਿਣਤੀ_2 > i)
      ਟੈਗ_ਸੂਚੀ[i++] = new_ਟੈਗ;
  }

  ਜੇ (*ਗਿਣਤੀ_2 > i)
  {
    hb_ਟੈਗ_t old_ਟੈਗ = hb_ot_old_ਟੈਗ_from_script (script);
    ਜੇ (old_ਟੈਗ != HB_OT_ਟੈਗ_DEFAULT_SCRIPT)
      ਟੈਗ_ਸੂਚੀ[i++] = old_ਟੈਗ;
  }

  *ਗਿਣਤੀ_2 = i;
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
hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_script (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ digit = ਟੈਗ_2 & 0x000000FFu;
  ਜੇ (unlikely (digit == '2' || digit == '3'))
    ਵਾਪਸ hb_ot_new_ਟੈਗ_ਮੰਜ਼ਿਲ_script (ਟੈਗ_2 & 0xFFFFFF32);

  ਵਾਪਸ hb_ot_old_ਟੈਗ_ਮੰਜ਼ਿਲ_script (ਟੈਗ_2);
}


/* hb_language_t */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
subtag_matches (ਅਚਲ ਅੱਖਰ *lang_str,
		ਅਚਲ ਅੱਖਰ *ਹੱਦਬੰਦੀ,
		ਅਚਲ ਅੱਖਰ *subtag,
		ਬਿਨਾਨਿਸ਼ਾਨੀ    subtag_len)
{
  ਜੇ (likely ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਹੱਦਬੰਦੀ - lang_str) < subtag_len))
    ਵਾਪਸ ਝੂਠ;

  ਕਰੋ {
    ਅਚਲ ਅੱਖਰ *s = ਲੜੀ_ਖੋਜ (lang_str, subtag);
    ਜੇ (!s || s >= ਹੱਦਬੰਦੀ)
      ਵਾਪਸ ਝੂਠ;
    ਜੇ (!ISALNUM (s[subtag_len]))
      ਵਾਪਸ ਸੱਚ;
    lang_str = s + subtag_len;
  } ਜਦੋਂ (ਸੱਚ);
}

ਸਥਿਰ ਤਾਰਕਿਕ
lang_matches (ਅਚਲ ਅੱਖਰ *lang_str,
	      ਅਚਲ ਅੱਖਰ *ਹੱਦਬੰਦੀ,
	      ਅਚਲ ਅੱਖਰ *spec,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  ਜੇ (likely ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਹੱਦਬੰਦੀ - lang_str) < spec_len))
    ਵਾਪਸ ਝੂਠ;

  ਵਾਪਸ ਲੜੀ_ਤੁਲਨਾ (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ਸਥਿਰ ਤਾਰਕਿਕ
bfind_ਟੈਗ (ਅਚਲ hb_ਟੈਗ_t *array,
	   ਬਿਨਾਨਿਸ਼ਾਨੀ        len,
	   hb_ਟੈਗ_t        key)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ min = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ max = len;

  ਜਦੋਂ (min < max)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ mid = min + (max - min) / 2;
    hb_ਟੈਗ_t val = array[mid];
    ਜੇ (key < val)
      max = mid;
    ਨਹੀਂਤਾਂ ਜੇ (key > val)
      min = mid + 1;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ ਸੱਚ;
  }

  ਵਾਪਸ ਝੂਠ;
}

ਬਣਤਰ Lang_ਟੈਗ
{
  hb_ਟੈਗ_t language;
  hb_ਟੈਗ_t ਟੈਗ_2;

  ਪੂਰਨ cmp (hb_ਟੈਗ_t a) ਅਚਲ
  {
    ਵਾਪਸ a < ਇਹ->language ? -1 : a > ਇਹ->language ? +1 : 0;
  }
  ਪੂਰਨ cmp (ਅਚਲ Lang_ਟੈਗ *that) ਅਚਲ
  { ਵਾਪਸ cmp (that->language); }
};

ਬਣਤਰ Lang_ਟੈਗ_Range
{
  hb_ਟੈਗ_t language;
  uint16_t ਵਿੱਥ;
  uint8_t ਗਿਣਤੀ_2;

  ਪੂਰਨ cmp (hb_ਟੈਗ_t a) ਅਚਲ
  {
    ਵਾਪਸ a < ਇਹ->language ? -1 : a > ਇਹ->language ? +1 : 0;
  }
  ਪੂਰਨ cmp (ਅਚਲ Lang_ਟੈਗ_Range *that) ਅਚਲ
  { ਵਾਪਸ cmp (that->language); }
};

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-tag-table.hh"

/* The corresponding languages IDs for the following IDs are unclear,
 * overlap, or are architecturally weird. Needs more research. */

/*{"??",	{HB_TAG('B','C','R',' ')}},*/	/* Bible Cree */
/*{"zh?",	{HB_TAG('C','H','N',' ')}},*/	/* Chinese (seen in Microsoft fonts) */
/*{"ar-Syrc?",	{HB_TAG('G','A','R',' ')}},*/	/* Garshuni */
/*{"??",	{HB_TAG('N','G','R',' ')}},*/	/* Nagari */
/*{"??",	{HB_TAG('Y','I','C',' ')}},*/	/* Yi Classic */
/*{"zh?",	{HB_TAG('Z','H','P',' ')}},*/	/* Chinese Phonetic */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_ਟੈਗ_t
hb_ot_ਟੈਗ_from_language (hb_language_t language)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 1;
  hb_ਟੈਗ_t ਟੈਗ_ਸੂਚੀ[1];
  hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (HB_SCRIPT_UNKNOWN, language, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ, &ਗਿਣਤੀ_2, ਟੈਗ_ਸੂਚੀ);
  ਵਾਪਸ ਗਿਣਤੀ_2 > 0 ? ਟੈਗ_ਸੂਚੀ[0] : HB_OT_ਟੈਗ_DEFAULT_LANGUAGE;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_from_language (ਅਚਲ ਅੱਖਰ   *lang_str,
			  ਅਚਲ ਅੱਖਰ   *ਹੱਦਬੰਦੀ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਗਿਣਤੀ_2,
			  hb_ਟੈਗ_t     *ਟੈਗ_ਸੂਚੀ)
{

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  ਜੇ (hb_ot_ਟੈਗ_ਸੂਚੀ_from_complex_language (lang_str, ਹੱਦਬੰਦੀ, ਗਿਣਤੀ_2, ਟੈਗ_ਸੂਚੀ))
    ਵਾਪਸ;
#ਜੇ_ਅੰਤ

  /* Find a language matching in the first component. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  ਅਚਲ ਅੱਖਰ *s; s = strchr (lang_str, '-');
#ਜੇ_ਅੰਤ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
    ਜੇ (s && ਹੱਦਬੰਦੀ - lang_str >= 6)
    {
      ਅਚਲ ਅੱਖਰ *extlang_ਸਿਰਾ = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      ਜੇ (3 == (extlang_ਸਿਰਾ ? extlang_ਸਿਰਾ - s - 1 : ਲੜੀ_ਲੰਬਾਈ (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#ਜੇ_ਅੰਤ
    ਅਚਲ ਅੱਖਰ *dash = strchr (lang_str, '-');
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪਹਿਲਾ_len = dash ? dash - lang_str : ਹੱਦਬੰਦੀ - lang_str;
    hb_ਟੈਗ_t lang_ਟੈਗ = hb_ਟੈਗ_from_string (lang_str, ਪਹਿਲਾ_len);

    ਜੇ (ਪਹਿਲਾ_len == 2)
    {
      ਸਥਿਰ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਆਖਰੀ_ਟੈਗ_idx_2 {0}; /* Poor man's cache. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਟੈਗ_idx = ਆਖਰੀ_ਟੈਗ_idx_2;

      ਜੇ (likely (ਟੈਗ_idx < ARRAY_ਲੰਮਾਈ (ot_languages2) &&
		  ot_languages2[ਟੈਗ_idx].language == lang_ਟੈਗ) ||
	  hb_sorted_array (ot_languages2).bfind (lang_ਟੈਗ, &ਟੈਗ_idx))
      {
	ਆਖਰੀ_ਟੈਗ_idx_2 = ਟੈਗ_idx;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਜਦੋਂ (ਟੈਗ_idx != 0 &&
	       ot_languages2[ਟੈਗ_idx].language == ot_languages2[ਟੈਗ_idx - 1].language)
	  ਟੈਗ_idx--;
	ਲਈ (i = 0;
	     i < *ਗਿਣਤੀ_2 &&
	     ਟੈਗ_idx + i < ARRAY_ਲੰਮਾਈ (ot_languages2) &&
	     ot_languages2[ਟੈਗ_idx + i].ਟੈਗ_2 != HB_ਟੈਗ_NONE &&
	     ot_languages2[ਟੈਗ_idx + i].language == ot_languages2[ਟੈਗ_idx].language;
	     i++)
	  ਟੈਗ_ਸੂਚੀ[i] = ot_languages2[ਟੈਗ_idx + i].ਟੈਗ_2;
	*ਗਿਣਤੀ_2 = i;
	ਵਾਪਸ;
      }
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
    ਨਹੀਂਤਾਂ ਜੇ (ਪਹਿਲਾ_len == 3)
    {
      ਸਥਿਰ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਆਖਰੀ_ਟੈਗ_idx_3 {0}; /* Poor man's cache. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਟੈਗ_idx = ਆਖਰੀ_ਟੈਗ_idx_3;

      ਜੇ (likely (ਟੈਗ_idx < ARRAY_ਲੰਮਾਈ (ot_languages3) &&
		  ot_languages3[ਟੈਗ_idx].language == lang_ਟੈਗ) ||
	  hb_sorted_array (ot_languages3).bfind (lang_ਟੈਗ, &ਟੈਗ_idx))
      {
	ਆਖਰੀ_ਟੈਗ_idx_3 = ਟੈਗ_idx;
	ਜੇ (*ਗਿਣਤੀ_2)
	{
	  ਟੈਗ_ਸੂਚੀ[0] = ot_languages3[ਟੈਗ_idx].ਟੈਗ_2;
	  *ਗਿਣਤੀ_2 = 1;
	}
	ਨਹੀਂਤਾਂ
	  *ਗਿਣਤੀ_2 = 0;
	ਵਾਪਸ;
      }

      ਸਥਿਰ hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਆਖਰੀ_ਟੈਗ_idx_3_multi {0}; /* Poor man's cache. */
      ਬਿਨਾਨਿਸ਼ਾਨੀ multi_ਟੈਗ_idx = ਆਖਰੀ_ਟੈਗ_idx_3_multi;

      ਜੇ (likely (multi_ਟੈਗ_idx < ARRAY_ਲੰਮਾਈ (ot_languages3_multi) &&
		  ot_languages3_multi[multi_ਟੈਗ_idx].language == lang_ਟੈਗ) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_ਟੈਗ, &multi_ਟੈਗ_idx))
      {
	ਆਖਰੀ_ਟੈਗ_idx_3_multi = multi_ਟੈਗ_idx;
	ਅਚਲ Lang_ਟੈਗ_Range &range = ot_languages3_multi[multi_ਟੈਗ_idx];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਲਈ (i = 0; i < *ਗਿਣਤੀ_2 && i < range.ਗਿਣਤੀ_2; i++)
	  ਟੈਗ_ਸੂਚੀ[i] = ot_languages3_multi_ਮੁੱਲਾਂ[range.ਵਿੱਥ + i];
	*ਗਿਣਤੀ_2 = i;
	ਵਾਪਸ;
      }

      ਜੇ (bfind_ਟੈਗ (ot_languages3_blocked, ARRAY_ਲੰਮਾਈ (ot_languages3_blocked), lang_ਟੈਗ))
      {
	*ਗਿਣਤੀ_2 = 0;
	ਵਾਪਸ;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      ਟੈਗ_ਸੂਚੀ[0] = lang_ਟੈਗ & ~0x20202000u;
      *ਗਿਣਤੀ_2 = 1;
      ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  ਜੇ (!s)
    s = lang_str + ਲੜੀ_ਲੰਬਾਈ (lang_str);
  ਜੇ (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    ਟੈਗ_ਸੂਚੀ[0] = hb_ਟੈਗ_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *ਗਿਣਤੀ_2 = 1;
    ਵਾਪਸ;
  }
#ਜੇ_ਅੰਤ

  *ਗਿਣਤੀ_2 = 0;
}

ਸਥਿਰ ਤਾਰਕਿਕ
parse_ਨਿੱਜੀ_use_subtag (ਅਚਲ ਅੱਖਰ     *ਨਿੱਜੀ_use_subtag,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਗਿਣਤੀ_2,
			  hb_ਟੈਗ_t       *ਟੈਗ_ਸੂਚੀ,
			  ਅਚਲ ਅੱਖਰ     *ਅਗੇਤਰ,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ (*normalize) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ))
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_ਨਿੱਜੀ_SUBTAG
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

  ਜੇ (!(ਨਿੱਜੀ_use_subtag && ਗਿਣਤੀ_2 && ਟੈਗ_ਸੂਚੀ && *ਗਿਣਤੀ_2)) ਵਾਪਸ ਝੂਠ;

  ਅਚਲ ਅੱਖਰ *s = ਲੜੀ_ਖੋਜ (ਨਿੱਜੀ_use_subtag, ਅਗੇਤਰ);
  ਜੇ (!s) ਵਾਪਸ ਝੂਠ;

  ਅੱਖਰ ਟੈਗ_2[4];
  ਪੂਰਨ i;
  s += ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
  ਜੇ (s[0] == '-') {
    s += 1;
    ਅੱਖਰ c;
    ਲਈ (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      ਜੇ (i % 2 == 0)
	ਟੈਗ_2[i / 2] = c << 4;
      ਨਹੀਂਤਾਂ
	ਟੈਗ_2[i / 2] += c;
    }
    ਜੇ (i != 8) ਵਾਪਸ ਝੂਠ;
  } ਨਹੀਂਤਾਂ {
    ਲਈ (i = 0; i < 4 && ISALNUM (s[i]); i++)
      ਟੈਗ_2[i] = normalize (s[i]);
    ਜੇ (!i) ਵਾਪਸ ਝੂਠ;

    ਲਈ (; i < 4; i++)
      ਟੈਗ_2[i] = ' ';
  }
  ਟੈਗ_ਸੂਚੀ[0] = HB_ਟੈਗ (ਟੈਗ_2[0], ਟੈਗ_2[1], ਟੈਗ_2[2], ਟੈਗ_2[3]);
  ਜੇ ((ਟੈਗ_ਸੂਚੀ[0] & 0xDFDFDFDF) == HB_OT_ਟੈਗ_DEFAULT_SCRIPT)
    ਟੈਗ_ਸੂਚੀ[0] ^= ~0xDFDFDFDF;
  *ਗਿਣਤੀ_2 = 1;
  ਵਾਪਸ ਸੱਚ;
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
ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *script_ਗਿਣਤੀ /* IN/OUT */,
				     hb_ਟੈਗ_t     *script_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *language_ਗਿਣਤੀ /* IN/OUT */,
				     hb_ਟੈਗ_t     *language_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/)
{
  ਤਾਰਕਿਕ needs_script = ਸੱਚ;

  ਜੇ (language == HB_LANGUAGE_INVALID)
  {
    ਜੇ (language_ਗਿਣਤੀ && language_ਟੈਗ_ਸੂਚੀ && *language_ਗਿਣਤੀ)
      *language_ਗਿਣਤੀ = 0;
  }
  ਨਹੀਂਤਾਂ
  {
    ਅਚਲ ਅੱਖਰ *lang_str, *s, *ਹੱਦਬੰਦੀ, *ਨਿੱਜੀ_use_subtag;
    ਤਾਰਕਿਕ needs_language;

    lang_str = hb_language_ਮੰਜ਼ਿਲ_string (language);
    ਹੱਦਬੰਦੀ = ਖਾਲੀ_ਸੂਚਕ;
    ਨਿੱਜੀ_use_subtag = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      ਨਿੱਜੀ_use_subtag = lang_str;
    } ਨਹੀਂਤਾਂ {
      ਲਈ (s = lang_str + 1; *s; s++)
      {
	ਜੇ (s[-1] == '-' && s[1] == '-')
	{
	  ਜੇ (s[0] == 'x')
	  {
	    ਨਿੱਜੀ_use_subtag = s;
	    ਜੇ (!ਹੱਦਬੰਦੀ)
	      ਹੱਦਬੰਦੀ = s - 1;
	    ਰੋਕੋ;
	  } ਨਹੀਂਤਾਂ ਜੇ (!ਹੱਦਬੰਦੀ)
	  {
	    ਹੱਦਬੰਦੀ = s - 1;
	  }
	}
      }
      ਜੇ (!ਹੱਦਬੰਦੀ)
	ਹੱਦਬੰਦੀ = s;
    }

    needs_script = !parse_ਨਿੱਜੀ_use_subtag (ਨਿੱਜੀ_use_subtag, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ, "-hbsc", TOLOWER);
    needs_language = !parse_ਨਿੱਜੀ_use_subtag (ਨਿੱਜੀ_use_subtag, language_ਗਿਣਤੀ, language_ਟੈਗ_ਸੂਚੀ, "-hbot", TOUPPER);

    ਜੇ (needs_language && language_ਗਿਣਤੀ && language_ਟੈਗ_ਸੂਚੀ && *language_ਗਿਣਤੀ)
      hb_ot_ਟੈਗ_ਸੂਚੀ_from_language (lang_str, ਹੱਦਬੰਦੀ, language_ਗਿਣਤੀ, language_ਟੈਗ_ਸੂਚੀ);
  }

  ਜੇ (needs_script && script_ਗਿਣਤੀ && script_ਟੈਗ_ਸੂਚੀ && *script_ਗਿਣਤੀ)
    hb_ot_all_ਟੈਗ_ਸੂਚੀ_from_script (script, script_ਗਿਣਤੀ, script_ਟੈਗ_ਸੂਚੀ);
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
hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_language (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

  ਜੇ (ਟੈਗ_2 == HB_OT_ਟੈਗ_DEFAULT_LANGUAGE)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_ਟੈਗ = hb_ot_ambiguous_ਟੈਗ_ਮੰਜ਼ਿਲ_language (ਟੈਗ_2);
    ਜੇ (disambiguated_ਟੈਗ != HB_LANGUAGE_INVALID)
      ਵਾਪਸ disambiguated_ਟੈਗ;
  }
#ਜੇ_ਅੰਤ

  ਅੱਖਰ buf[4];
  ਲਈ (i = 0; i < ARRAY_ਲੰਮਾਈ (ot_languages2); i++)
    ਜੇ (ot_languages2[i].ਟੈਗ_2 == ਟੈਗ_2)
    {
      hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (ot_languages2[i].language, buf);
      ਵਾਪਸ hb_language_from_string (buf, 2);
    }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  ਲਈ (i = 0; i < ARRAY_ਲੰਮਾਈ (ot_languages3); i++)
    ਜੇ (ot_languages3[i].ਟੈਗ_2 == ਟੈਗ_2)
    {
      hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (ot_languages3[i].language, buf);
      ਵਾਪਸ hb_language_from_string (buf, 3);
    }
  ਲਈ (i = 0; i < ARRAY_ਲੰਮਾਈ (ot_languages3_multi); i++)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; j < ot_languages3_multi[i].ਗਿਣਤੀ_2; j++)
      ਜੇ (ot_languages3_multi_ਮੁੱਲਾਂ[ot_languages3_multi[i].ਵਿੱਥ + j] == ਟੈਗ_2)
      {
	hb_ਟੈਗ_ਮੰਜ਼ਿਲ_string (ot_languages3_multi[i].language, buf);
	ਵਾਪਸ hb_language_from_string (buf, 3);
      }
#ਜੇ_ਅੰਤ

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    ਅੱਖਰ buf[20];
    ਅੱਖਰ *str = buf;
    ਜੇ (ISALPHA (ਟੈਗ_2 >> 24)
	&& ISALPHA ((ਟੈਗ_2 >> 16) & 0xFF)
	&& ISALPHA ((ਟੈਗ_2 >> 8) & 0xFF)
	&& (ਟੈਗ_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (ਟੈਗ_2 >> 24);
      buf[1] = TOLOWER ((ਟੈਗ_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((ਟੈਗ_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, ਟੈਗ_2);
    ਵਾਪਸ hb_language_from_string (&*buf, -1);
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
ਖਾਲੀ
hb_ot_ਟੈਗ_ਸੂਚੀ_ਮੰਜ਼ਿਲ_script_and_language (hb_ਟੈਗ_t       script_ਟੈਗ,
				   hb_ਟੈਗ_t       language_ਟੈਗ,
				   hb_script_t   *script /*ਬਾਹਰਲਾ*/,
				   hb_language_t *language /*ਬਾਹਰਲਾ*/)
{
  hb_script_t script_ਬਾਹਰਲਾ = hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_script (script_ਟੈਗ);
  ਜੇ (script)
    *script = script_ਬਾਹਰਲਾ;
  ਜੇ (language)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ script_ਗਿਣਤੀ = 1;
    hb_ਟੈਗ_t primary_script_ਟੈਗ[1];
    hb_ot_ਟੈਗ_ਸੂਚੀ_from_script_and_language (script_ਬਾਹਰਲਾ,
					 HB_LANGUAGE_INVALID,
					 &script_ਗਿਣਤੀ,
					 primary_script_ਟੈਗ,
					 ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    *language = hb_ot_ਟੈਗ_ਮੰਜ਼ਿਲ_language (language_ਟੈਗ);
    ਅਚਲ ਅੱਖਰ *lang_str = *language ? hb_language_ਮੰਜ਼ਿਲ_string (*language) : ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (lang_str && (script_ਗਿਣਤੀ == 0 || primary_script_ਟੈਗ[0] != script_ਟੈਗ))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf;
      size_t len = ਲੜੀ_ਲੰਬਾਈ (lang_str);
      buf = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) hb_malloc (len + 16);
      ਜੇ (unlikely (!buf))
      {
	*language = ਖਾਲੀ_ਸੂਚਕ;
      }
      ਨਹੀਂਤਾਂ
      {
	ਪੂਰਨ ਖਿਸਕਾਓ;
	hb_memcpy (buf, lang_str, len);
	ਜੇ (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	ਲਈ (ਖਿਸਕਾਓ = 28; ਖਿਸਕਾਓ >= 0; ਖਿਸਕਾਓ -= 4)
	  buf[len++] = TOHEX (script_ਟੈਗ >> ਖਿਸਕਾਓ);
	*language = hb_language_from_string ((ਅੱਖਰ *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ MAIN
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
test_langs_sorted ()
{
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ARRAY_ਲੰਮਾਈ (ot_languages2); i++)
  {
    ਪੂਰਨ c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    ਜੇ (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_LANGUAGE_LONG
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ARRAY_ਲੰਮਾਈ (ot_languages3); i++)
  {
    ਪੂਰਨ c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    ਜੇ (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ARRAY_ਲੰਮਾਈ (ot_languages3_multi); i++)
  {
    ਪੂਰਨ c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    ਜੇ (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ARRAY_ਲੰਮਾਈ (ot_languages3_blocked); i++)
    ਜੇ (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ
ਮੁੱਖ ()
{
  test_langs_sorted ();
  ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ
