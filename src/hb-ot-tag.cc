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

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_OT_لیبل


/* hb_script_t */

ساکن hb_لیبل_t
hb_ot_old_لیبل_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  انتخاب ((hb_لیبل_t) script)
  {
    حالت HB_SCRIPT_INVALID:		واپس HB_OT_لیبل_DEFAULT_SCRIPT;
    حالت HB_SCRIPT_MATH:		واپس HB_OT_لیبل_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالت HB_SCRIPT_HIRAGANA:		واپس HB_لیبل('k','a','n','a');
    حالت HB_لیبل('H','r','k','t'):
					واپس HB_لیبل('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالت HB_SCRIPT_LAO:			واپس HB_لیبل('l','a','o',' ');
    حالت HB_SCRIPT_YI:			واپس HB_لیبل('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالت HB_SCRIPT_NKO:			واپس HB_لیبل('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالت HB_SCRIPT_VAI:			واپس HB_لیبل('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  واپس ((hb_لیبل_t) script) | 0x20000000u;
}

ساکن hb_script_t
hb_ot_old_لیبل_مقصد_script (hb_لیبل_t لیبل_2)
{
  اگر (unlikely (لیبل_2 == HB_OT_لیبل_DEFAULT_SCRIPT))
    واپس HB_SCRIPT_INVALID;
  اگر (unlikely (لیبل_2 == HB_OT_لیبل_MATH_SCRIPT))
    واپس HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  اگر (unlikely ((لیبل_2 & 0x0000FF00u) == 0x00002000u))
    لیبل_2 |= (لیبل_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  اگر (unlikely ((لیبل_2 & 0x000000FFu) == 0x00000020u))
    لیبل_2 |= (لیبل_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  واپس (hb_script_t) (لیبل_2 & ~0x20000000u);
}

ساکن hb_لیبل_t
hb_ot_new_لیبل_from_script (hb_script_t script)
{
  انتخاب ((hb_لیبل_t) script) {
    حالت HB_SCRIPT_BENGALI:		واپس HB_لیبل('b','n','g','2');
    حالت HB_SCRIPT_DEVANAGARI:		واپس HB_لیبل('d','e','v','2');
    حالت HB_SCRIPT_GUJARATI:		واپس HB_لیبل('g','j','r','2');
    حالت HB_SCRIPT_GURMUKHI:		واپس HB_لیبل('g','u','r','2');
    حالت HB_SCRIPT_KANNADA:		واپس HB_لیبل('k','n','d','2');
    حالت HB_SCRIPT_MALAYALAM:		واپس HB_لیبل('m','l','m','2');
    حالت HB_SCRIPT_ORIYA:		واپس HB_لیبل('o','r','y','2');
    حالت HB_SCRIPT_TAMIL:		واپس HB_لیبل('t','m','l','2');
    حالت HB_SCRIPT_TELUGU:		واپس HB_لیبل('t','e','l','2');
    حالت HB_SCRIPT_MYANMAR:		واپس HB_لیبل('m','y','m','2');
  }

  واپس HB_OT_لیبل_DEFAULT_SCRIPT;
}

ساکن hb_script_t
hb_ot_new_لیبل_مقصد_script (hb_لیبل_t لیبل_2)
{
  انتخاب (لیبل_2) {
    حالت HB_لیبل('b','n','g','2'):	واپس HB_SCRIPT_BENGALI;
    حالت HB_لیبل('d','e','v','2'):	واپس HB_SCRIPT_DEVANAGARI;
    حالت HB_لیبل('g','j','r','2'):	واپس HB_SCRIPT_GUJARATI;
    حالت HB_لیبل('g','u','r','2'):	واپس HB_SCRIPT_GURMUKHI;
    حالت HB_لیبل('k','n','d','2'):	واپس HB_SCRIPT_KANNADA;
    حالت HB_لیبل('m','l','m','2'):	واپس HB_SCRIPT_MALAYALAM;
    حالت HB_لیبل('o','r','y','2'):	واپس HB_SCRIPT_ORIYA;
    حالت HB_لیبل('t','m','l','2'):	واپس HB_SCRIPT_TAMIL;
    حالت HB_لیبل('t','e','l','2'):	واپس HB_SCRIPT_TELUGU;
    حالت HB_لیبل('m','y','m','2'):	واپس HB_SCRIPT_MYANMAR;
  }

  واپس HB_SCRIPT_UNKNOWN;
}

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
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
خالی
hb_ot_لیبل_فہرست_from_script (hb_script_t  script,
			hb_لیبل_t    *script_لیبل_1,
			hb_لیبل_t    *script_لیبل_2)
{
  غیرعلامتی صحیح گنتی = 2;
  hb_لیبل_t لیبل_فہرست[2];
  hb_ot_لیبل_فہرست_from_script_and_language (script, HB_LANGUAGE_INVALID, &گنتی, لیبل_فہرست, خالی_اشاریہ, خالی_اشاریہ);
  *script_لیبل_1 = گنتی > 0 ? لیبل_فہرست[0] : HB_OT_لیبل_DEFAULT_SCRIPT;
  *script_لیبل_2 = گنتی > 1 ? لیبل_فہرست[1] : HB_OT_لیبل_DEFAULT_SCRIPT;
}
#اگر_ختم

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساکن خالی
hb_ot_all_لیبل_فہرست_from_script (hb_script_t   script,
			    غیرعلامتی صحیح *گنتی /* IN/OUT */,
			    hb_لیبل_t     *لیبل_فہرست /*برآمد*/)
{
  غیرعلامتی صحیح i = 0;

  hb_لیبل_t new_لیبل = hb_ot_new_لیبل_from_script (script);
  اگر (unlikely (new_لیبل != HB_OT_لیبل_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    اگر (new_لیبل != HB_لیبل('m','y','m','2'))
      لیبل_فہرست[i++] = new_لیبل | '3';
    اگر (*گنتی > i)
      لیبل_فہرست[i++] = new_لیبل;
  }

  اگر (*گنتی > i)
  {
    hb_لیبل_t old_لیبل = hb_ot_old_لیبل_from_script (script);
    اگر (old_لیبل != HB_OT_لیبل_DEFAULT_SCRIPT)
      لیبل_فہرست[i++] = old_لیبل;
  }

  *گنتی = i;
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
hb_ot_لیبل_مقصد_script (hb_لیبل_t لیبل_2)
{
  غیرعلامتی حرف digit = لیبل_2 & 0x000000FFu;
  اگر (unlikely (digit == '2' || digit == '3'))
    واپس hb_ot_new_لیبل_مقصد_script (لیبل_2 & 0xFFFFFF32);

  واپس hb_ot_old_لیبل_مقصد_script (لیبل_2);
}


/* hb_language_t */

ساکن اندرونی منطقی
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *حد_بندی,
		ثابت حرف *subtag,
		غیرعلامتی    subtag_len)
{
  اگر (likely ((غیرعلامتی) (حد_بندی - lang_str) < subtag_len))
    واپس جھوٹ;

  کرو {
    ثابت حرف *s = لڑی_تلاش (lang_str, subtag);
    اگر (!s || s >= حد_بندی)
      واپس جھوٹ;
    اگر (!ISALNUM (s[subtag_len]))
      واپس سچ;
    lang_str = s + subtag_len;
  } جب (سچ);
}

ساکن منطقی
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *حد_بندی,
	      ثابت حرف *spec,
	      غیرعلامتی    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  اگر (likely ((غیرعلامتی) (حد_بندی - lang_str) < spec_len))
    واپس جھوٹ;

  واپس لڑی_موازنہ (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساکن منطقی
bfind_لیبل (ثابت hb_لیبل_t *array,
	   غیرعلامتی        len,
	   hb_لیبل_t        key)
{
  غیرعلامتی min = 0;
  غیرعلامتی max = len;

  جب (min < max)
  {
    غیرعلامتی mid = min + (max - min) / 2;
    hb_لیبل_t val = array[mid];
    اگر (key < val)
      max = mid;
    ورنہ اگر (key > val)
      min = mid + 1;
    ورنہ
      واپس سچ;
  }

  واپس جھوٹ;
}

ساخت Lang_لیبل
{
  hb_لیبل_t language;
  hb_لیبل_t لیبل_2;

  صحیح cmp (hb_لیبل_t a) ثابت
  {
    واپس a < یہ->language ? -1 : a > یہ->language ? +1 : 0;
  }
  صحیح cmp (ثابت Lang_لیبل *that) ثابت
  { واپس cmp (that->language); }
};

ساخت Lang_لیبل_Range
{
  hb_لیبل_t language;
  uint16_t انحراف;
  uint8_t گنتی;

  صحیح cmp (hb_لیبل_t a) ثابت
  {
    واپس a < یہ->language ? -1 : a > یہ->language ? +1 : 0;
  }
  صحیح cmp (ثابت Lang_لیبل_Range *that) ثابت
  { واپس cmp (that->language); }
};

#شامل "hb-ot-tag-table.hh"

/* The corresponding languages IDs for the following IDs are unclear,
 * overlap, or are architecturally weird. Needs more research. */

/*{"??",	{HB_TAG('B','C','R',' ')}},*/	/* Bible Cree */
/*{"zh?",	{HB_TAG('C','H','N',' ')}},*/	/* Chinese (seen in Microsoft fonts) */
/*{"ar-Syrc?",	{HB_TAG('G','A','R',' ')}},*/	/* Garshuni */
/*{"??",	{HB_TAG('N','G','R',' ')}},*/	/* Nagari */
/*{"??",	{HB_TAG('Y','I','C',' ')}},*/	/* Yi Classic */
/*{"zh?",	{HB_TAG('Z','H','P',' ')}},*/	/* Chinese Phonetic */

#اگر_غیر_تعریف_شدہ HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_لیبل_t
hb_ot_لیبل_from_language (hb_language_t language)
{
  غیرعلامتی صحیح گنتی = 1;
  hb_لیبل_t لیبل_فہرست[1];
  hb_ot_لیبل_فہرست_from_script_and_language (HB_SCRIPT_UNKNOWN, language, خالی_اشاریہ, خالی_اشاریہ, &گنتی, لیبل_فہرست);
  واپس گنتی > 0 ? لیبل_فہرست[0] : HB_OT_لیبل_DEFAULT_LANGUAGE;
}
#اگر_ختم

ساکن خالی
hb_ot_لیبل_فہرست_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *حد_بندی,
			  غیرعلامتی صحیح *گنتی,
			  hb_لیبل_t     *لیبل_فہرست)
{

#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  اگر (hb_ot_لیبل_فہرست_from_complex_language (lang_str, حد_بندی, گنتی, لیبل_فہرست))
    واپس;
#اگر_ختم

  /* Find a language matching in the first component. */
#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#اگر_ختم
  {
#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
    اگر (s && حد_بندی - lang_str >= 6)
    {
      ثابت حرف *extlang_انتہا = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      اگر (3 == (extlang_انتہا ? extlang_انتہا - s - 1 : لڑی_لمبائی (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#اگر_ختم
    ثابت حرف *dash = strchr (lang_str, '-');
    غیرعلامتی پہلا_len = dash ? dash - lang_str : حد_بندی - lang_str;
    hb_لیبل_t lang_لیبل = hb_لیبل_from_string (lang_str, پہلا_len);

    اگر (پہلا_len == 2)
    {
      ساکن hb_atomic_t<غیرعلامتی> آخری_لیبل_idx_2 {0}; /* Poor man's cache. */
      غیرعلامتی لیبل_idx = آخری_لیبل_idx_2;

      اگر (likely (لیبل_idx < ARRAY_لمبائی (ot_languages2) &&
		  ot_languages2[لیبل_idx].language == lang_لیبل) ||
	  hb_sorted_array (ot_languages2).bfind (lang_لیبل, &لیبل_idx))
      {
	آخری_لیبل_idx_2 = لیبل_idx;
	غیرعلامتی صحیح i;
	جب (لیبل_idx != 0 &&
	       ot_languages2[لیبل_idx].language == ot_languages2[لیبل_idx - 1].language)
	  لیبل_idx--;
	برائے (i = 0;
	     i < *گنتی &&
	     لیبل_idx + i < ARRAY_لمبائی (ot_languages2) &&
	     ot_languages2[لیبل_idx + i].لیبل_2 != HB_لیبل_NONE &&
	     ot_languages2[لیبل_idx + i].language == ot_languages2[لیبل_idx].language;
	     i++)
	  لیبل_فہرست[i] = ot_languages2[لیبل_idx + i].لیبل_2;
	*گنتی = i;
	واپس;
      }
    }
#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
    ورنہ اگر (پہلا_len == 3)
    {
      ساکن hb_atomic_t<غیرعلامتی> آخری_لیبل_idx_3 {0}; /* Poor man's cache. */
      غیرعلامتی لیبل_idx = آخری_لیبل_idx_3;

      اگر (likely (لیبل_idx < ARRAY_لمبائی (ot_languages3) &&
		  ot_languages3[لیبل_idx].language == lang_لیبل) ||
	  hb_sorted_array (ot_languages3).bfind (lang_لیبل, &لیبل_idx))
      {
	آخری_لیبل_idx_3 = لیبل_idx;
	اگر (*گنتی)
	{
	  لیبل_فہرست[0] = ot_languages3[لیبل_idx].لیبل_2;
	  *گنتی = 1;
	}
	ورنہ
	  *گنتی = 0;
	واپس;
      }

      ساکن hb_atomic_t<غیرعلامتی> آخری_لیبل_idx_3_multi {0}; /* Poor man's cache. */
      غیرعلامتی multi_لیبل_idx = آخری_لیبل_idx_3_multi;

      اگر (likely (multi_لیبل_idx < ARRAY_لمبائی (ot_languages3_multi) &&
		  ot_languages3_multi[multi_لیبل_idx].language == lang_لیبل) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_لیبل, &multi_لیبل_idx))
      {
	آخری_لیبل_idx_3_multi = multi_لیبل_idx;
	ثابت Lang_لیبل_Range &range = ot_languages3_multi[multi_لیبل_idx];
	غیرعلامتی صحیح i;
	برائے (i = 0; i < *گنتی && i < range.گنتی; i++)
	  لیبل_فہرست[i] = ot_languages3_multi_اقدار[range.انحراف + i];
	*گنتی = i;
	واپس;
      }

      اگر (bfind_لیبل (ot_languages3_blocked, ARRAY_لمبائی (ot_languages3_blocked), lang_لیبل))
      {
	*گنتی = 0;
	واپس;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      لیبل_فہرست[0] = lang_لیبل & ~0x20202000u;
      *گنتی = 1;
      واپس;
    }
#اگر_ختم
  }

#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  اگر (!s)
    s = lang_str + لڑی_لمبائی (lang_str);
  اگر (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    لیبل_فہرست[0] = hb_لیبل_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *گنتی = 1;
    واپس;
  }
#اگر_ختم

  *گنتی = 0;
}

ساکن منطقی
parse_نجی_use_subtag (ثابت حرف     *نجی_use_subtag,
			  غیرعلامتی صحیح   *گنتی,
			  hb_لیبل_t       *لیبل_فہرست,
			  ثابت حرف     *سابقہ,
			  غیرعلامتی حرف (*normalize) (غیرعلامتی حرف))
{
#اگر_تعریف_شدہ HB_NO_LANGUAGE_نجی_SUBTAG
  واپس جھوٹ;
#اگر_ختم

  اگر (!(نجی_use_subtag && گنتی && لیبل_فہرست && *گنتی)) واپس جھوٹ;

  ثابت حرف *s = لڑی_تلاش (نجی_use_subtag, سابقہ);
  اگر (!s) واپس جھوٹ;

  حرف لیبل_2[4];
  صحیح i;
  s += لڑی_لمبائی (سابقہ);
  اگر (s[0] == '-') {
    s += 1;
    حرف c;
    برائے (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      اگر (i % 2 == 0)
	لیبل_2[i / 2] = c << 4;
      ورنہ
	لیبل_2[i / 2] += c;
    }
    اگر (i != 8) واپس جھوٹ;
  } ورنہ {
    برائے (i = 0; i < 4 && ISALNUM (s[i]); i++)
      لیبل_2[i] = normalize (s[i]);
    اگر (!i) واپس جھوٹ;

    برائے (; i < 4; i++)
      لیبل_2[i] = ' ';
  }
  لیبل_فہرست[0] = HB_لیبل (لیبل_2[0], لیبل_2[1], لیبل_2[2], لیبل_2[3]);
  اگر ((لیبل_فہرست[0] & 0xDFDFDFDF) == HB_OT_لیبل_DEFAULT_SCRIPT)
    لیبل_فہرست[0] ^= ~0xDFDFDFDF;
  *گنتی = 1;
  واپس سچ;
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
خالی
hb_ot_لیبل_فہرست_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				     hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/,
				     غیرعلامتی صحیح *language_گنتی /* IN/OUT */,
				     hb_لیبل_t     *language_لیبل_فہرست /*برآمد*/)
{
  منطقی needs_script = سچ;

  اگر (language == HB_LANGUAGE_INVALID)
  {
    اگر (language_گنتی && language_لیبل_فہرست && *language_گنتی)
      *language_گنتی = 0;
  }
  ورنہ
  {
    ثابت حرف *lang_str, *s, *حد_بندی, *نجی_use_subtag;
    منطقی needs_language;

    lang_str = hb_language_مقصد_string (language);
    حد_بندی = خالی_اشاریہ;
    نجی_use_subtag = خالی_اشاریہ;
    اگر (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      نجی_use_subtag = lang_str;
    } ورنہ {
      برائے (s = lang_str + 1; *s; s++)
      {
	اگر (s[-1] == '-' && s[1] == '-')
	{
	  اگر (s[0] == 'x')
	  {
	    نجی_use_subtag = s;
	    اگر (!حد_بندی)
	      حد_بندی = s - 1;
	    توڑو;
	  } ورنہ اگر (!حد_بندی)
	  {
	    حد_بندی = s - 1;
	  }
	}
      }
      اگر (!حد_بندی)
	حد_بندی = s;
    }

    needs_script = !parse_نجی_use_subtag (نجی_use_subtag, script_گنتی, script_لیبل_فہرست, "-hbsc", TOLOWER);
    needs_language = !parse_نجی_use_subtag (نجی_use_subtag, language_گنتی, language_لیبل_فہرست, "-hbot", TOUPPER);

    اگر (needs_language && language_گنتی && language_لیبل_فہرست && *language_گنتی)
      hb_ot_لیبل_فہرست_from_language (lang_str, حد_بندی, language_گنتی, language_لیبل_فہرست);
  }

  اگر (needs_script && script_گنتی && script_لیبل_فہرست && *script_گنتی)
    hb_ot_all_لیبل_فہرست_from_script (script, script_گنتی, script_لیبل_فہرست);
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
hb_ot_لیبل_مقصد_language (hb_لیبل_t لیبل_2)
{
  غیرعلامتی صحیح i;

  اگر (لیبل_2 == HB_OT_لیبل_DEFAULT_LANGUAGE)
    واپس خالی_اشاریہ;

#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_لیبل = hb_ot_ambiguous_لیبل_مقصد_language (لیبل_2);
    اگر (disambiguated_لیبل != HB_LANGUAGE_INVALID)
      واپس disambiguated_لیبل;
  }
#اگر_ختم

  حرف buf[4];
  برائے (i = 0; i < ARRAY_لمبائی (ot_languages2); i++)
    اگر (ot_languages2[i].لیبل_2 == لیبل_2)
    {
      hb_لیبل_مقصد_string (ot_languages2[i].language, buf);
      واپس hb_language_from_string (buf, 2);
    }
#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  برائے (i = 0; i < ARRAY_لمبائی (ot_languages3); i++)
    اگر (ot_languages3[i].لیبل_2 == لیبل_2)
    {
      hb_لیبل_مقصد_string (ot_languages3[i].language, buf);
      واپس hb_language_from_string (buf, 3);
    }
  برائے (i = 0; i < ARRAY_لمبائی (ot_languages3_multi); i++)
    برائے (غیرعلامتی صحیح j = 0; j < ot_languages3_multi[i].گنتی; j++)
      اگر (ot_languages3_multi_اقدار[ot_languages3_multi[i].انحراف + j] == لیبل_2)
      {
	hb_لیبل_مقصد_string (ot_languages3_multi[i].language, buf);
	واپس hb_language_from_string (buf, 3);
      }
#اگر_ختم

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    اگر (ISALPHA (لیبل_2 >> 24)
	&& ISALPHA ((لیبل_2 >> 16) & 0xFF)
	&& ISALPHA ((لیبل_2 >> 8) & 0xFF)
	&& (لیبل_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (لیبل_2 >> 24);
      buf[1] = TOLOWER ((لیبل_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((لیبل_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, لیبل_2);
    واپس hb_language_from_string (&*buf, -1);
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
خالی
hb_ot_لیبل_فہرست_مقصد_script_and_language (hb_لیبل_t       script_لیبل,
				   hb_لیبل_t       language_لیبل,
				   hb_script_t   *script /*برآمد*/,
				   hb_language_t *language /*برآمد*/)
{
  hb_script_t script_برآمد = hb_ot_لیبل_مقصد_script (script_لیبل);
  اگر (script)
    *script = script_برآمد;
  اگر (language)
  {
    غیرعلامتی صحیح script_گنتی = 1;
    hb_لیبل_t primary_script_لیبل[1];
    hb_ot_لیبل_فہرست_from_script_and_language (script_برآمد,
					 HB_LANGUAGE_INVALID,
					 &script_گنتی,
					 primary_script_لیبل,
					 خالی_اشاریہ, خالی_اشاریہ);
    *language = hb_ot_لیبل_مقصد_language (language_لیبل);
    ثابت حرف *lang_str = *language ? hb_language_مقصد_string (*language) : خالی_اشاریہ;
    اگر (lang_str && (script_گنتی == 0 || primary_script_لیبل[0] != script_لیبل))
    {
      غیرعلامتی حرف *buf;
      size_t len = لڑی_لمبائی (lang_str);
      buf = (غیرعلامتی حرف *) hb_malloc (len + 16);
      اگر (unlikely (!buf))
      {
	*language = خالی_اشاریہ;
      }
      ورنہ
      {
	صحیح سرکاؤ;
	hb_memcpy (buf, lang_str, len);
	اگر (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	برائے (سرکاؤ = 28; سرکاؤ >= 0; سرکاؤ -= 4)
	  buf[len++] = TOHEX (script_لیبل >> سرکاؤ);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#اگر_تعریف_شدہ MAIN
ساکن اندرونی خالی
test_langs_sorted ()
{
  برائے (غیرعلامتی صحیح i = 1; i < ARRAY_لمبائی (ot_languages2); i++)
  {
    صحیح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    اگر (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#اگر_غیر_تعریف_شدہ HB_NO_LANGUAGE_LONG
  برائے (غیرعلامتی صحیح i = 1; i < ARRAY_لمبائی (ot_languages3); i++)
  {
    صحیح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    اگر (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  برائے (غیرعلامتی صحیح i = 1; i < ARRAY_لمبائی (ot_languages3_multi); i++)
  {
    صحیح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    اگر (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  برائے (غیرعلامتی صحیح i = 1; i < ARRAY_لمبائی (ot_languages3_blocked); i++)
    اگر (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#اگر_ختم
}

صحیح
مین ()
{
  test_langs_sorted ();
  واپس 0;
}

#اگر_ختم


#اگر_ختم
