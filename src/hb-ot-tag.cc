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

#اگر_تعریف_نشده HB_NO_OT_برچسب


/* hb_script_t */

ساكن hb_برچسب_t
hb_ot_old_برچسب_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  بدل ((hb_برچسب_t) script)
  {
    حالة HB_SCRIPT_INVALID:		أرجع HB_OT_برچسب_DEFAULT_SCRIPT;
    حالة HB_SCRIPT_MATH:		أرجع HB_OT_برچسب_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالة HB_SCRIPT_HIRAGANA:		أرجع HB_برچسب('k','a','n','a');
    حالة HB_برچسب('H','r','k','t'):
					أرجع HB_برچسب('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالة HB_SCRIPT_LAO:			أرجع HB_برچسب('l','a','o',' ');
    حالة HB_SCRIPT_YI:			أرجع HB_برچسب('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_NKO:			أرجع HB_برچسب('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_VAI:			أرجع HB_برچسب('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  أرجع ((hb_برچسب_t) script) | 0x20000000u;
}

ساكن hb_script_t
hb_ot_old_برچسب_مقصد_script (hb_برچسب_t برچسب_2)
{
  إذا (unlikely (برچسب_2 == HB_OT_برچسب_DEFAULT_SCRIPT))
    أرجع HB_SCRIPT_INVALID;
  إذا (unlikely (برچسب_2 == HB_OT_برچسب_MATH_SCRIPT))
    أرجع HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  إذا (unlikely ((برچسب_2 & 0x0000FF00u) == 0x00002000u))
    برچسب_2 |= (برچسب_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  إذا (unlikely ((برچسب_2 & 0x000000FFu) == 0x00000020u))
    برچسب_2 |= (برچسب_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  أرجع (hb_script_t) (برچسب_2 & ~0x20000000u);
}

ساكن hb_برچسب_t
hb_ot_new_برچسب_from_script (hb_script_t script)
{
  بدل ((hb_برچسب_t) script) {
    حالة HB_SCRIPT_BENGALI:		أرجع HB_برچسب('b','n','g','2');
    حالة HB_SCRIPT_DEVANAGARI:		أرجع HB_برچسب('d','e','v','2');
    حالة HB_SCRIPT_GUJARATI:		أرجع HB_برچسب('g','j','r','2');
    حالة HB_SCRIPT_GURMUKHI:		أرجع HB_برچسب('g','u','r','2');
    حالة HB_SCRIPT_KANNADA:		أرجع HB_برچسب('k','n','d','2');
    حالة HB_SCRIPT_MALAYALAM:		أرجع HB_برچسب('m','l','m','2');
    حالة HB_SCRIPT_ORIYA:		أرجع HB_برچسب('o','r','y','2');
    حالة HB_SCRIPT_TAMIL:		أرجع HB_برچسب('t','m','l','2');
    حالة HB_SCRIPT_TELUGU:		أرجع HB_برچسب('t','e','l','2');
    حالة HB_SCRIPT_MYANMAR:		أرجع HB_برچسب('m','y','m','2');
  }

  أرجع HB_OT_برچسب_DEFAULT_SCRIPT;
}

ساكن hb_script_t
hb_ot_new_برچسب_مقصد_script (hb_برچسب_t برچسب_2)
{
  بدل (برچسب_2) {
    حالة HB_برچسب('b','n','g','2'):	أرجع HB_SCRIPT_BENGALI;
    حالة HB_برچسب('d','e','v','2'):	أرجع HB_SCRIPT_DEVANAGARI;
    حالة HB_برچسب('g','j','r','2'):	أرجع HB_SCRIPT_GUJARATI;
    حالة HB_برچسب('g','u','r','2'):	أرجع HB_SCRIPT_GURMUKHI;
    حالة HB_برچسب('k','n','d','2'):	أرجع HB_SCRIPT_KANNADA;
    حالة HB_برچسب('m','l','m','2'):	أرجع HB_SCRIPT_MALAYALAM;
    حالة HB_برچسب('o','r','y','2'):	أرجع HB_SCRIPT_ORIYA;
    حالة HB_برچسب('t','m','l','2'):	أرجع HB_SCRIPT_TAMIL;
    حالة HB_برچسب('t','e','l','2'):	أرجع HB_SCRIPT_TELUGU;
    حالة HB_برچسب('m','y','m','2'):	أرجع HB_SCRIPT_MYANMAR;
  }

  أرجع HB_SCRIPT_UNKNOWN;
}

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
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
hb_ot_برچسبها_from_script (hb_script_t  script,
			hb_برچسب_t    *script_برچسب_1,
			hb_برچسب_t    *script_برچسب_2)
{
  بیعلامت صحيح شمار = 2;
  hb_برچسب_t برچسبها[2];
  hb_ot_برچسبها_from_script_and_language (script, HB_LANGUAGE_INVALID, &شمار, برچسبها, اشاره_تهی, اشاره_تهی);
  *script_برچسب_1 = شمار > 0 ? برچسبها[0] : HB_OT_برچسب_DEFAULT_SCRIPT;
  *script_برچسب_2 = شمار > 1 ? برچسبها[1] : HB_OT_برچسب_DEFAULT_SCRIPT;
}
#پایان_اگر

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساكن فراغ
hb_ot_all_برچسبها_from_script (hb_script_t   script,
			    بیعلامت صحيح *شمار /* IN/OUT */,
			    hb_برچسب_t     *برچسبها /*بیرون*/)
{
  بیعلامت صحيح i = 0;

  hb_برچسب_t new_برچسب = hb_ot_new_برچسب_from_script (script);
  إذا (unlikely (new_برچسب != HB_OT_برچسب_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    إذا (new_برچسب != HB_برچسب('m','y','m','2'))
      برچسبها[i++] = new_برچسب | '3';
    إذا (*شمار > i)
      برچسبها[i++] = new_برچسب;
  }

  إذا (*شمار > i)
  {
    hb_برچسب_t old_برچسب = hb_ot_old_برچسب_from_script (script);
    إذا (old_برچسب != HB_OT_برچسب_DEFAULT_SCRIPT)
      برچسبها[i++] = old_برچسب;
  }

  *شمار = i;
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
hb_ot_برچسب_مقصد_script (hb_برچسب_t برچسب_2)
{
  بیعلامت حرف digit = برچسب_2 & 0x000000FFu;
  إذا (unlikely (digit == '2' || digit == '3'))
    أرجع hb_ot_new_برچسب_مقصد_script (برچسب_2 & 0xFFFFFF32);

  أرجع hb_ot_old_برچسب_مقصد_script (برچسب_2);
}


/* hb_language_t */

ساكن مضمن منطقي
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *حد,
		ثابت حرف *subtag,
		بیعلامت    subtag_len)
{
  إذا (likely ((بیعلامت) (حد - lang_str) < subtag_len))
    أرجع خطأ;

  افعل {
    ثابت حرف *s = جستجوی_رشته (lang_str, subtag);
    إذا (!s || s >= حد)
      أرجع خطأ;
    إذا (!ISALNUM (s[subtag_len]))
      أرجع درست;
    lang_str = s + subtag_len;
  } طالما (درست);
}

ساكن منطقي
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *حد,
	      ثابت حرف *spec,
	      بیعلامت    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  إذا (likely ((بیعلامت) (حد - lang_str) < spec_len))
    أرجع خطأ;

  أرجع مقایسه_رشته (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساكن منطقي
bfind_برچسب (ثابت hb_برچسب_t *array,
	   بیعلامت        len,
	   hb_برچسب_t        key)
{
  بیعلامت min = 0;
  بیعلامت max = len;

  طالما (min < max)
  {
    بیعلامت mid = min + (max - min) / 2;
    hb_برچسب_t val = array[mid];
    إذا (key < val)
      max = mid;
    وإلا إذا (key > val)
      min = mid + 1;
    وإلا
      أرجع درست;
  }

  أرجع خطأ;
}

بنية Lang_برچسب
{
  hb_برچسب_t language;
  hb_برچسب_t برچسب_2;

  صحيح cmp (hb_برچسب_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_برچسب *that) ثابت
  { أرجع cmp (that->language); }
};

بنية Lang_برچسب_Range
{
  hb_برچسب_t language;
  uint16_t جابه_جایی;
  uint8_t شمار;

  صحيح cmp (hb_برچسب_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_برچسب_Range *that) ثابت
  { أرجع cmp (that->language); }
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

#اگر_تعریف_نشده HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_برچسب_t
hb_ot_برچسب_from_language (hb_language_t language)
{
  بیعلامت صحيح شمار = 1;
  hb_برچسب_t برچسبها[1];
  hb_ot_برچسبها_from_script_and_language (HB_SCRIPT_UNKNOWN, language, اشاره_تهی, اشاره_تهی, &شمار, برچسبها);
  أرجع شمار > 0 ? برچسبها[0] : HB_OT_برچسب_DEFAULT_LANGUAGE;
}
#پایان_اگر

ساكن فراغ
hb_ot_برچسبها_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *حد,
			  بیعلامت صحيح *شمار,
			  hb_برچسب_t     *برچسبها)
{

#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  إذا (hb_ot_برچسبها_from_complex_language (lang_str, حد, شمار, برچسبها))
    أرجع;
#پایان_اگر

  /* Find a language matching in the first component. */
#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#پایان_اگر
  {
#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
    إذا (s && حد - lang_str >= 6)
    {
      ثابت حرف *extlang_انتها = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      إذا (3 == (extlang_انتها ? extlang_انتها - s - 1 : طول_رشته (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#پایان_اگر
    ثابت حرف *dash = strchr (lang_str, '-');
    بیعلامت اولی_len = dash ? dash - lang_str : حد - lang_str;
    hb_برچسب_t lang_برچسب = hb_برچسب_from_string (lang_str, اولی_len);

    إذا (اولی_len == 2)
    {
      ساكن hb_atomic_t<بیعلامت> آخرین_برچسب_idx_2 {0}; /* Poor man's cache. */
      بیعلامت برچسب_idx = آخرین_برچسب_idx_2;

      إذا (likely (برچسب_idx < ARRAY_درازی (ot_languages2) &&
		  ot_languages2[برچسب_idx].language == lang_برچسب) ||
	  hb_sorted_array (ot_languages2).bfind (lang_برچسب, &برچسب_idx))
      {
	آخرین_برچسب_idx_2 = برچسب_idx;
	بیعلامت صحيح i;
	طالما (برچسب_idx != 0 &&
	       ot_languages2[برچسب_idx].language == ot_languages2[برچسب_idx - 1].language)
	  برچسب_idx--;
	لكل (i = 0;
	     i < *شمار &&
	     برچسب_idx + i < ARRAY_درازی (ot_languages2) &&
	     ot_languages2[برچسب_idx + i].برچسب_2 != HB_برچسب_NONE &&
	     ot_languages2[برچسب_idx + i].language == ot_languages2[برچسب_idx].language;
	     i++)
	  برچسبها[i] = ot_languages2[برچسب_idx + i].برچسب_2;
	*شمار = i;
	أرجع;
      }
    }
#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
    وإلا إذا (اولی_len == 3)
    {
      ساكن hb_atomic_t<بیعلامت> آخرین_برچسب_idx_3 {0}; /* Poor man's cache. */
      بیعلامت برچسب_idx = آخرین_برچسب_idx_3;

      إذا (likely (برچسب_idx < ARRAY_درازی (ot_languages3) &&
		  ot_languages3[برچسب_idx].language == lang_برچسب) ||
	  hb_sorted_array (ot_languages3).bfind (lang_برچسب, &برچسب_idx))
      {
	آخرین_برچسب_idx_3 = برچسب_idx;
	إذا (*شمار)
	{
	  برچسبها[0] = ot_languages3[برچسب_idx].برچسب_2;
	  *شمار = 1;
	}
	وإلا
	  *شمار = 0;
	أرجع;
      }

      ساكن hb_atomic_t<بیعلامت> آخرین_برچسب_idx_3_multi {0}; /* Poor man's cache. */
      بیعلامت multi_برچسب_idx = آخرین_برچسب_idx_3_multi;

      إذا (likely (multi_برچسب_idx < ARRAY_درازی (ot_languages3_multi) &&
		  ot_languages3_multi[multi_برچسب_idx].language == lang_برچسب) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_برچسب, &multi_برچسب_idx))
      {
	آخرین_برچسب_idx_3_multi = multi_برچسب_idx;
	ثابت Lang_برچسب_Range &range = ot_languages3_multi[multi_برچسب_idx];
	بیعلامت صحيح i;
	لكل (i = 0; i < *شمار && i < range.شمار; i++)
	  برچسبها[i] = ot_languages3_multi_مقادیر[range.جابه_جایی + i];
	*شمار = i;
	أرجع;
      }

      إذا (bfind_برچسب (ot_languages3_blocked, ARRAY_درازی (ot_languages3_blocked), lang_برچسب))
      {
	*شمار = 0;
	أرجع;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      برچسبها[0] = lang_برچسب & ~0x20202000u;
      *شمار = 1;
      أرجع;
    }
#پایان_اگر
  }

#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  إذا (!s)
    s = lang_str + طول_رشته (lang_str);
  إذا (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    برچسبها[0] = hb_برچسب_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *شمار = 1;
    أرجع;
  }
#پایان_اگر

  *شمار = 0;
}

ساكن منطقي
parse_خصوصی_use_subtag (ثابت حرف     *خصوصی_use_subtag,
			  بیعلامت صحيح   *شمار,
			  hb_برچسب_t       *برچسبها,
			  ثابت حرف     *پیشوند,
			  بیعلامت حرف (*normalize) (بیعلامت حرف))
{
#اگر_تعریف_شده HB_NO_LANGUAGE_خصوصی_SUBTAG
  أرجع خطأ;
#پایان_اگر

  إذا (!(خصوصی_use_subtag && شمار && برچسبها && *شمار)) أرجع خطأ;

  ثابت حرف *s = جستجوی_رشته (خصوصی_use_subtag, پیشوند);
  إذا (!s) أرجع خطأ;

  حرف برچسب_2[4];
  صحيح i;
  s += طول_رشته (پیشوند);
  إذا (s[0] == '-') {
    s += 1;
    حرف c;
    لكل (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      إذا (i % 2 == 0)
	برچسب_2[i / 2] = c << 4;
      وإلا
	برچسب_2[i / 2] += c;
    }
    إذا (i != 8) أرجع خطأ;
  } وإلا {
    لكل (i = 0; i < 4 && ISALNUM (s[i]); i++)
      برچسب_2[i] = normalize (s[i]);
    إذا (!i) أرجع خطأ;

    لكل (; i < 4; i++)
      برچسب_2[i] = ' ';
  }
  برچسبها[0] = HB_برچسب (برچسب_2[0], برچسب_2[1], برچسب_2[2], برچسب_2[3]);
  إذا ((برچسبها[0] & 0xDFDFDFDF) == HB_OT_برچسب_DEFAULT_SCRIPT)
    برچسبها[0] ^= ~0xDFDFDFDF;
  *شمار = 1;
  أرجع درست;
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
hb_ot_برچسبها_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بیعلامت صحيح *script_شمار /* IN/OUT */,
				     hb_برچسب_t     *script_برچسبها /*بیرون*/,
				     بیعلامت صحيح *language_شمار /* IN/OUT */,
				     hb_برچسب_t     *language_برچسبها /*بیرون*/)
{
  منطقي needs_script = درست;

  إذا (language == HB_LANGUAGE_INVALID)
  {
    إذا (language_شمار && language_برچسبها && *language_شمار)
      *language_شمار = 0;
  }
  وإلا
  {
    ثابت حرف *lang_str, *s, *حد, *خصوصی_use_subtag;
    منطقي needs_language;

    lang_str = hb_language_مقصد_string (language);
    حد = اشاره_تهی;
    خصوصی_use_subtag = اشاره_تهی;
    إذا (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      خصوصی_use_subtag = lang_str;
    } وإلا {
      لكل (s = lang_str + 1; *s; s++)
      {
	إذا (s[-1] == '-' && s[1] == '-')
	{
	  إذا (s[0] == 'x')
	  {
	    خصوصی_use_subtag = s;
	    إذا (!حد)
	      حد = s - 1;
	    اكسر;
	  } وإلا إذا (!حد)
	  {
	    حد = s - 1;
	  }
	}
      }
      إذا (!حد)
	حد = s;
    }

    needs_script = !parse_خصوصی_use_subtag (خصوصی_use_subtag, script_شمار, script_برچسبها, "-hbsc", TOLOWER);
    needs_language = !parse_خصوصی_use_subtag (خصوصی_use_subtag, language_شمار, language_برچسبها, "-hbot", TOUPPER);

    إذا (needs_language && language_شمار && language_برچسبها && *language_شمار)
      hb_ot_برچسبها_from_language (lang_str, حد, language_شمار, language_برچسبها);
  }

  إذا (needs_script && script_شمار && script_برچسبها && *script_شمار)
    hb_ot_all_برچسبها_from_script (script, script_شمار, script_برچسبها);
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
hb_ot_برچسب_مقصد_language (hb_برچسب_t برچسب_2)
{
  بیعلامت صحيح i;

  إذا (برچسب_2 == HB_OT_برچسب_DEFAULT_LANGUAGE)
    أرجع اشاره_تهی;

#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_برچسب = hb_ot_ambiguous_برچسب_مقصد_language (برچسب_2);
    إذا (disambiguated_برچسب != HB_LANGUAGE_INVALID)
      أرجع disambiguated_برچسب;
  }
#پایان_اگر

  حرف buf[4];
  لكل (i = 0; i < ARRAY_درازی (ot_languages2); i++)
    إذا (ot_languages2[i].برچسب_2 == برچسب_2)
    {
      hb_برچسب_مقصد_string (ot_languages2[i].language, buf);
      أرجع hb_language_from_string (buf, 2);
    }
#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  لكل (i = 0; i < ARRAY_درازی (ot_languages3); i++)
    إذا (ot_languages3[i].برچسب_2 == برچسب_2)
    {
      hb_برچسب_مقصد_string (ot_languages3[i].language, buf);
      أرجع hb_language_from_string (buf, 3);
    }
  لكل (i = 0; i < ARRAY_درازی (ot_languages3_multi); i++)
    لكل (بیعلامت صحيح j = 0; j < ot_languages3_multi[i].شمار; j++)
      إذا (ot_languages3_multi_مقادیر[ot_languages3_multi[i].جابه_جایی + j] == برچسب_2)
      {
	hb_برچسب_مقصد_string (ot_languages3_multi[i].language, buf);
	أرجع hb_language_from_string (buf, 3);
      }
#پایان_اگر

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    إذا (ISALPHA (برچسب_2 >> 24)
	&& ISALPHA ((برچسب_2 >> 16) & 0xFF)
	&& ISALPHA ((برچسب_2 >> 8) & 0xFF)
	&& (برچسب_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (برچسب_2 >> 24);
      buf[1] = TOLOWER ((برچسب_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((برچسب_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, برچسب_2);
    أرجع hb_language_from_string (&*buf, -1);
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
hb_ot_برچسبها_مقصد_script_and_language (hb_برچسب_t       script_برچسب,
				   hb_برچسب_t       language_برچسب,
				   hb_script_t   *script /*بیرون*/,
				   hb_language_t *language /*بیرون*/)
{
  hb_script_t script_بیرون = hb_ot_برچسب_مقصد_script (script_برچسب);
  إذا (script)
    *script = script_بیرون;
  إذا (language)
  {
    بیعلامت صحيح script_شمار = 1;
    hb_برچسب_t primary_script_برچسب[1];
    hb_ot_برچسبها_from_script_and_language (script_بیرون,
					 HB_LANGUAGE_INVALID,
					 &script_شمار,
					 primary_script_برچسب,
					 اشاره_تهی, اشاره_تهی);
    *language = hb_ot_برچسب_مقصد_language (language_برچسب);
    ثابت حرف *lang_str = *language ? hb_language_مقصد_string (*language) : اشاره_تهی;
    إذا (lang_str && (script_شمار == 0 || primary_script_برچسب[0] != script_برچسب))
    {
      بیعلامت حرف *buf;
      size_t len = طول_رشته (lang_str);
      buf = (بیعلامت حرف *) hb_malloc (len + 16);
      إذا (unlikely (!buf))
      {
	*language = اشاره_تهی;
      }
      وإلا
      {
	صحيح لغزش;
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
	لكل (لغزش = 28; لغزش >= 0; لغزش -= 4)
	  buf[len++] = TOHEX (script_برچسب >> لغزش);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#اگر_تعریف_شده MAIN
ساكن مضمن فراغ
test_langs_sorted ()
{
  لكل (بیعلامت صحيح i = 1; i < ARRAY_درازی (ot_languages2); i++)
  {
    صحيح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#اگر_تعریف_نشده HB_NO_LANGUAGE_LONG
  لكل (بیعلامت صحيح i = 1; i < ARRAY_درازی (ot_languages3); i++)
  {
    صحيح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  لكل (بیعلامت صحيح i = 1; i < ARRAY_درازی (ot_languages3_multi); i++)
  {
    صحيح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  لكل (بیعلامت صحيح i = 1; i < ARRAY_درازی (ot_languages3_blocked); i++)
    إذا (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#پایان_اگر
}

صحيح
اصلی ()
{
  test_langs_sorted ();
  أرجع 0;
}

#پایان_اگر


#پایان_اگر
