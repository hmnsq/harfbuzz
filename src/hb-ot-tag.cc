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

#جے_بنا_تعریف HB_NO_OT_ٹیگ


/* hb_script_t */

ساكن hb_ٹیگ_t
hb_ot_old_ٹیگ_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  بدل ((hb_ٹیگ_t) script)
  {
    حالة HB_SCRIPT_INVALID:		أرجع HB_OT_ٹیگ_DEFAULT_SCRIPT;
    حالة HB_SCRIPT_MATH:		أرجع HB_OT_ٹیگ_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالة HB_SCRIPT_HIRAGANA:		أرجع HB_ٹیگ('k','a','n','a');
    حالة HB_ٹیگ('H','r','k','t'):
					أرجع HB_ٹیگ('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالة HB_SCRIPT_LAO:			أرجع HB_ٹیگ('l','a','o',' ');
    حالة HB_SCRIPT_YI:			أرجع HB_ٹیگ('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_NKO:			أرجع HB_ٹیگ('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_VAI:			أرجع HB_ٹیگ('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  أرجع ((hb_ٹیگ_t) script) | 0x20000000u;
}

ساكن hb_script_t
hb_ot_old_ٹیگ_منزل_script (hb_ٹیگ_t ٹیگ_2)
{
  إذا (unlikely (ٹیگ_2 == HB_OT_ٹیگ_DEFAULT_SCRIPT))
    أرجع HB_SCRIPT_INVALID;
  إذا (unlikely (ٹیگ_2 == HB_OT_ٹیگ_MATH_SCRIPT))
    أرجع HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  إذا (unlikely ((ٹیگ_2 & 0x0000FF00u) == 0x00002000u))
    ٹیگ_2 |= (ٹیگ_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  إذا (unlikely ((ٹیگ_2 & 0x000000FFu) == 0x00000020u))
    ٹیگ_2 |= (ٹیگ_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  أرجع (hb_script_t) (ٹیگ_2 & ~0x20000000u);
}

ساكن hb_ٹیگ_t
hb_ot_new_ٹیگ_from_script (hb_script_t script)
{
  بدل ((hb_ٹیگ_t) script) {
    حالة HB_SCRIPT_BENGALI:		أرجع HB_ٹیگ('b','n','g','2');
    حالة HB_SCRIPT_DEVANAGARI:		أرجع HB_ٹیگ('d','e','v','2');
    حالة HB_SCRIPT_GUJARATI:		أرجع HB_ٹیگ('g','j','r','2');
    حالة HB_SCRIPT_GURMUKHI:		أرجع HB_ٹیگ('g','u','r','2');
    حالة HB_SCRIPT_KANNADA:		أرجع HB_ٹیگ('k','n','d','2');
    حالة HB_SCRIPT_MALAYALAM:		أرجع HB_ٹیگ('m','l','m','2');
    حالة HB_SCRIPT_ORIYA:		أرجع HB_ٹیگ('o','r','y','2');
    حالة HB_SCRIPT_TAMIL:		أرجع HB_ٹیگ('t','m','l','2');
    حالة HB_SCRIPT_TELUGU:		أرجع HB_ٹیگ('t','e','l','2');
    حالة HB_SCRIPT_MYANMAR:		أرجع HB_ٹیگ('m','y','m','2');
  }

  أرجع HB_OT_ٹیگ_DEFAULT_SCRIPT;
}

ساكن hb_script_t
hb_ot_new_ٹیگ_منزل_script (hb_ٹیگ_t ٹیگ_2)
{
  بدل (ٹیگ_2) {
    حالة HB_ٹیگ('b','n','g','2'):	أرجع HB_SCRIPT_BENGALI;
    حالة HB_ٹیگ('d','e','v','2'):	أرجع HB_SCRIPT_DEVANAGARI;
    حالة HB_ٹیگ('g','j','r','2'):	أرجع HB_SCRIPT_GUJARATI;
    حالة HB_ٹیگ('g','u','r','2'):	أرجع HB_SCRIPT_GURMUKHI;
    حالة HB_ٹیگ('k','n','d','2'):	أرجع HB_SCRIPT_KANNADA;
    حالة HB_ٹیگ('m','l','m','2'):	أرجع HB_SCRIPT_MALAYALAM;
    حالة HB_ٹیگ('o','r','y','2'):	أرجع HB_SCRIPT_ORIYA;
    حالة HB_ٹیگ('t','m','l','2'):	أرجع HB_SCRIPT_TAMIL;
    حالة HB_ٹیگ('t','e','l','2'):	أرجع HB_SCRIPT_TELUGU;
    حالة HB_ٹیگ('m','y','m','2'):	أرجع HB_SCRIPT_MYANMAR;
  }

  أرجع HB_SCRIPT_UNKNOWN;
}

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
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
hb_ot_ٹیگ_سوچی_from_script (hb_script_t  script,
			hb_ٹیگ_t    *script_ٹیگ_1,
			hb_ٹیگ_t    *script_ٹیگ_2)
{
  بنانشانی صحيح گنتی = 2;
  hb_ٹیگ_t ٹیگ_سوچی[2];
  hb_ot_ٹیگ_سوچی_from_script_and_language (script, HB_LANGUAGE_INVALID, &گنتی, ٹیگ_سوچی, خالی_سوچک, خالی_سوچک);
  *script_ٹیگ_1 = گنتی > 0 ? ٹیگ_سوچی[0] : HB_OT_ٹیگ_DEFAULT_SCRIPT;
  *script_ٹیگ_2 = گنتی > 1 ? ٹیگ_سوچی[1] : HB_OT_ٹیگ_DEFAULT_SCRIPT;
}
#جے_انت

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساكن فراغ
hb_ot_all_ٹیگ_سوچی_from_script (hb_script_t   script,
			    بنانشانی صحيح *گنتی /* IN/OUT */,
			    hb_ٹیگ_t     *ٹیگ_سوچی /*باہرلا*/)
{
  بنانشانی صحيح i = 0;

  hb_ٹیگ_t new_ٹیگ = hb_ot_new_ٹیگ_from_script (script);
  إذا (unlikely (new_ٹیگ != HB_OT_ٹیگ_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    إذا (new_ٹیگ != HB_ٹیگ('m','y','m','2'))
      ٹیگ_سوچی[i++] = new_ٹیگ | '3';
    إذا (*گنتی > i)
      ٹیگ_سوچی[i++] = new_ٹیگ;
  }

  إذا (*گنتی > i)
  {
    hb_ٹیگ_t old_ٹیگ = hb_ot_old_ٹیگ_from_script (script);
    إذا (old_ٹیگ != HB_OT_ٹیگ_DEFAULT_SCRIPT)
      ٹیگ_سوچی[i++] = old_ٹیگ;
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
hb_ot_ٹیگ_منزل_script (hb_ٹیگ_t ٹیگ_2)
{
  بنانشانی حرف digit = ٹیگ_2 & 0x000000FFu;
  إذا (unlikely (digit == '2' || digit == '3'))
    أرجع hb_ot_new_ٹیگ_منزل_script (ٹیگ_2 & 0xFFFFFF32);

  أرجع hb_ot_old_ٹیگ_منزل_script (ٹیگ_2);
}


/* hb_language_t */

ساكن مضمن منطقي
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *حد_بندی,
		ثابت حرف *subtag,
		بنانشانی    subtag_len)
{
  إذا (likely ((بنانشانی) (حد_بندی - lang_str) < subtag_len))
    أرجع خطأ;

  افعل {
    ثابت حرف *s = لڑی_کھوج (lang_str, subtag);
    إذا (!s || s >= حد_بندی)
      أرجع خطأ;
    إذا (!ISALNUM (s[subtag_len]))
      أرجع سچ;
    lang_str = s + subtag_len;
  } طالما (سچ);
}

ساكن منطقي
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *حد_بندی,
	      ثابت حرف *spec,
	      بنانشانی    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  إذا (likely ((بنانشانی) (حد_بندی - lang_str) < spec_len))
    أرجع خطأ;

  أرجع لڑی_مقابلہ (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساكن منطقي
bfind_ٹیگ (ثابت hb_ٹیگ_t *array,
	   بنانشانی        len,
	   hb_ٹیگ_t        key)
{
  بنانشانی min = 0;
  بنانشانی max = len;

  طالما (min < max)
  {
    بنانشانی mid = min + (max - min) / 2;
    hb_ٹیگ_t val = array[mid];
    إذا (key < val)
      max = mid;
    وإلا إذا (key > val)
      min = mid + 1;
    وإلا
      أرجع سچ;
  }

  أرجع خطأ;
}

بنية Lang_ٹیگ
{
  hb_ٹیگ_t language;
  hb_ٹیگ_t ٹیگ_2;

  صحيح cmp (hb_ٹیگ_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_ٹیگ *that) ثابت
  { أرجع cmp (that->language); }
};

بنية Lang_ٹیگ_Range
{
  hb_ٹیگ_t language;
  uint16_t وتھ;
  uint8_t گنتی;

  صحيح cmp (hb_ٹیگ_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_ٹیگ_Range *that) ثابت
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

#جے_بنا_تعریف HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_ٹیگ_t
hb_ot_ٹیگ_from_language (hb_language_t language)
{
  بنانشانی صحيح گنتی = 1;
  hb_ٹیگ_t ٹیگ_سوچی[1];
  hb_ot_ٹیگ_سوچی_from_script_and_language (HB_SCRIPT_UNKNOWN, language, خالی_سوچک, خالی_سوچک, &گنتی, ٹیگ_سوچی);
  أرجع گنتی > 0 ? ٹیگ_سوچی[0] : HB_OT_ٹیگ_DEFAULT_LANGUAGE;
}
#جے_انت

ساكن فراغ
hb_ot_ٹیگ_سوچی_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *حد_بندی,
			  بنانشانی صحيح *گنتی,
			  hb_ٹیگ_t     *ٹیگ_سوچی)
{

#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  إذا (hb_ot_ٹیگ_سوچی_from_complex_language (lang_str, حد_بندی, گنتی, ٹیگ_سوچی))
    أرجع;
#جے_انت

  /* Find a language matching in the first component. */
#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#جے_انت
  {
#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
    إذا (s && حد_بندی - lang_str >= 6)
    {
      ثابت حرف *extlang_سرا = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      إذا (3 == (extlang_سرا ? extlang_سرا - s - 1 : لڑی_لمبائی (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#جے_انت
    ثابت حرف *dash = strchr (lang_str, '-');
    بنانشانی پہلا_len = dash ? dash - lang_str : حد_بندی - lang_str;
    hb_ٹیگ_t lang_ٹیگ = hb_ٹیگ_from_string (lang_str, پہلا_len);

    إذا (پہلا_len == 2)
    {
      ساكن hb_atomic_t<بنانشانی> آخری_ٹیگ_idx_2 {0}; /* Poor man's cache. */
      بنانشانی ٹیگ_idx = آخری_ٹیگ_idx_2;

      إذا (likely (ٹیگ_idx < ARRAY_لمائی (ot_languages2) &&
		  ot_languages2[ٹیگ_idx].language == lang_ٹیگ) ||
	  hb_sorted_array (ot_languages2).bfind (lang_ٹیگ, &ٹیگ_idx))
      {
	آخری_ٹیگ_idx_2 = ٹیگ_idx;
	بنانشانی صحيح i;
	طالما (ٹیگ_idx != 0 &&
	       ot_languages2[ٹیگ_idx].language == ot_languages2[ٹیگ_idx - 1].language)
	  ٹیگ_idx--;
	لكل (i = 0;
	     i < *گنتی &&
	     ٹیگ_idx + i < ARRAY_لمائی (ot_languages2) &&
	     ot_languages2[ٹیگ_idx + i].ٹیگ_2 != HB_ٹیگ_NONE &&
	     ot_languages2[ٹیگ_idx + i].language == ot_languages2[ٹیگ_idx].language;
	     i++)
	  ٹیگ_سوچی[i] = ot_languages2[ٹیگ_idx + i].ٹیگ_2;
	*گنتی = i;
	أرجع;
      }
    }
#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
    وإلا إذا (پہلا_len == 3)
    {
      ساكن hb_atomic_t<بنانشانی> آخری_ٹیگ_idx_3 {0}; /* Poor man's cache. */
      بنانشانی ٹیگ_idx = آخری_ٹیگ_idx_3;

      إذا (likely (ٹیگ_idx < ARRAY_لمائی (ot_languages3) &&
		  ot_languages3[ٹیگ_idx].language == lang_ٹیگ) ||
	  hb_sorted_array (ot_languages3).bfind (lang_ٹیگ, &ٹیگ_idx))
      {
	آخری_ٹیگ_idx_3 = ٹیگ_idx;
	إذا (*گنتی)
	{
	  ٹیگ_سوچی[0] = ot_languages3[ٹیگ_idx].ٹیگ_2;
	  *گنتی = 1;
	}
	وإلا
	  *گنتی = 0;
	أرجع;
      }

      ساكن hb_atomic_t<بنانشانی> آخری_ٹیگ_idx_3_multi {0}; /* Poor man's cache. */
      بنانشانی multi_ٹیگ_idx = آخری_ٹیگ_idx_3_multi;

      إذا (likely (multi_ٹیگ_idx < ARRAY_لمائی (ot_languages3_multi) &&
		  ot_languages3_multi[multi_ٹیگ_idx].language == lang_ٹیگ) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_ٹیگ, &multi_ٹیگ_idx))
      {
	آخری_ٹیگ_idx_3_multi = multi_ٹیگ_idx;
	ثابت Lang_ٹیگ_Range &range = ot_languages3_multi[multi_ٹیگ_idx];
	بنانشانی صحيح i;
	لكل (i = 0; i < *گنتی && i < range.گنتی; i++)
	  ٹیگ_سوچی[i] = ot_languages3_multi_اقدار[range.وتھ + i];
	*گنتی = i;
	أرجع;
      }

      إذا (bfind_ٹیگ (ot_languages3_blocked, ARRAY_لمائی (ot_languages3_blocked), lang_ٹیگ))
      {
	*گنتی = 0;
	أرجع;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      ٹیگ_سوچی[0] = lang_ٹیگ & ~0x20202000u;
      *گنتی = 1;
      أرجع;
    }
#جے_انت
  }

#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  إذا (!s)
    s = lang_str + لڑی_لمبائی (lang_str);
  إذا (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    ٹیگ_سوچی[0] = hb_ٹیگ_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *گنتی = 1;
    أرجع;
  }
#جے_انت

  *گنتی = 0;
}

ساكن منطقي
parse_نجی_use_subtag (ثابت حرف     *نجی_use_subtag,
			  بنانشانی صحيح   *گنتی,
			  hb_ٹیگ_t       *ٹیگ_سوچی,
			  ثابت حرف     *اگیتر,
			  بنانشانی حرف (*normalize) (بنانشانی حرف))
{
#جے_تعریف_شدہ HB_NO_LANGUAGE_نجی_SUBTAG
  أرجع خطأ;
#جے_انت

  إذا (!(نجی_use_subtag && گنتی && ٹیگ_سوچی && *گنتی)) أرجع خطأ;

  ثابت حرف *s = لڑی_کھوج (نجی_use_subtag, اگیتر);
  إذا (!s) أرجع خطأ;

  حرف ٹیگ_2[4];
  صحيح i;
  s += لڑی_لمبائی (اگیتر);
  إذا (s[0] == '-') {
    s += 1;
    حرف c;
    لكل (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      إذا (i % 2 == 0)
	ٹیگ_2[i / 2] = c << 4;
      وإلا
	ٹیگ_2[i / 2] += c;
    }
    إذا (i != 8) أرجع خطأ;
  } وإلا {
    لكل (i = 0; i < 4 && ISALNUM (s[i]); i++)
      ٹیگ_2[i] = normalize (s[i]);
    إذا (!i) أرجع خطأ;

    لكل (; i < 4; i++)
      ٹیگ_2[i] = ' ';
  }
  ٹیگ_سوچی[0] = HB_ٹیگ (ٹیگ_2[0], ٹیگ_2[1], ٹیگ_2[2], ٹیگ_2[3]);
  إذا ((ٹیگ_سوچی[0] & 0xDFDFDFDF) == HB_OT_ٹیگ_DEFAULT_SCRIPT)
    ٹیگ_سوچی[0] ^= ~0xDFDFDFDF;
  *گنتی = 1;
  أرجع سچ;
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
hb_ot_ٹیگ_سوچی_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بنانشانی صحيح *script_گنتی /* IN/OUT */,
				     hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/,
				     بنانشانی صحيح *language_گنتی /* IN/OUT */,
				     hb_ٹیگ_t     *language_ٹیگ_سوچی /*باہرلا*/)
{
  منطقي needs_script = سچ;

  إذا (language == HB_LANGUAGE_INVALID)
  {
    إذا (language_گنتی && language_ٹیگ_سوچی && *language_گنتی)
      *language_گنتی = 0;
  }
  وإلا
  {
    ثابت حرف *lang_str, *s, *حد_بندی, *نجی_use_subtag;
    منطقي needs_language;

    lang_str = hb_language_منزل_string (language);
    حد_بندی = خالی_سوچک;
    نجی_use_subtag = خالی_سوچک;
    إذا (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      نجی_use_subtag = lang_str;
    } وإلا {
      لكل (s = lang_str + 1; *s; s++)
      {
	إذا (s[-1] == '-' && s[1] == '-')
	{
	  إذا (s[0] == 'x')
	  {
	    نجی_use_subtag = s;
	    إذا (!حد_بندی)
	      حد_بندی = s - 1;
	    اكسر;
	  } وإلا إذا (!حد_بندی)
	  {
	    حد_بندی = s - 1;
	  }
	}
      }
      إذا (!حد_بندی)
	حد_بندی = s;
    }

    needs_script = !parse_نجی_use_subtag (نجی_use_subtag, script_گنتی, script_ٹیگ_سوچی, "-hbsc", TOLOWER);
    needs_language = !parse_نجی_use_subtag (نجی_use_subtag, language_گنتی, language_ٹیگ_سوچی, "-hbot", TOUPPER);

    إذا (needs_language && language_گنتی && language_ٹیگ_سوچی && *language_گنتی)
      hb_ot_ٹیگ_سوچی_from_language (lang_str, حد_بندی, language_گنتی, language_ٹیگ_سوچی);
  }

  إذا (needs_script && script_گنتی && script_ٹیگ_سوچی && *script_گنتی)
    hb_ot_all_ٹیگ_سوچی_from_script (script, script_گنتی, script_ٹیگ_سوچی);
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
hb_ot_ٹیگ_منزل_language (hb_ٹیگ_t ٹیگ_2)
{
  بنانشانی صحيح i;

  إذا (ٹیگ_2 == HB_OT_ٹیگ_DEFAULT_LANGUAGE)
    أرجع خالی_سوچک;

#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_ٹیگ = hb_ot_ambiguous_ٹیگ_منزل_language (ٹیگ_2);
    إذا (disambiguated_ٹیگ != HB_LANGUAGE_INVALID)
      أرجع disambiguated_ٹیگ;
  }
#جے_انت

  حرف buf[4];
  لكل (i = 0; i < ARRAY_لمائی (ot_languages2); i++)
    إذا (ot_languages2[i].ٹیگ_2 == ٹیگ_2)
    {
      hb_ٹیگ_منزل_string (ot_languages2[i].language, buf);
      أرجع hb_language_from_string (buf, 2);
    }
#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  لكل (i = 0; i < ARRAY_لمائی (ot_languages3); i++)
    إذا (ot_languages3[i].ٹیگ_2 == ٹیگ_2)
    {
      hb_ٹیگ_منزل_string (ot_languages3[i].language, buf);
      أرجع hb_language_from_string (buf, 3);
    }
  لكل (i = 0; i < ARRAY_لمائی (ot_languages3_multi); i++)
    لكل (بنانشانی صحيح j = 0; j < ot_languages3_multi[i].گنتی; j++)
      إذا (ot_languages3_multi_اقدار[ot_languages3_multi[i].وتھ + j] == ٹیگ_2)
      {
	hb_ٹیگ_منزل_string (ot_languages3_multi[i].language, buf);
	أرجع hb_language_from_string (buf, 3);
      }
#جے_انت

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    إذا (ISALPHA (ٹیگ_2 >> 24)
	&& ISALPHA ((ٹیگ_2 >> 16) & 0xFF)
	&& ISALPHA ((ٹیگ_2 >> 8) & 0xFF)
	&& (ٹیگ_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (ٹیگ_2 >> 24);
      buf[1] = TOLOWER ((ٹیگ_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((ٹیگ_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, ٹیگ_2);
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
hb_ot_ٹیگ_سوچی_منزل_script_and_language (hb_ٹیگ_t       script_ٹیگ,
				   hb_ٹیگ_t       language_ٹیگ,
				   hb_script_t   *script /*باہرلا*/,
				   hb_language_t *language /*باہرلا*/)
{
  hb_script_t script_باہرلا = hb_ot_ٹیگ_منزل_script (script_ٹیگ);
  إذا (script)
    *script = script_باہرلا;
  إذا (language)
  {
    بنانشانی صحيح script_گنتی = 1;
    hb_ٹیگ_t primary_script_ٹیگ[1];
    hb_ot_ٹیگ_سوچی_from_script_and_language (script_باہرلا,
					 HB_LANGUAGE_INVALID,
					 &script_گنتی,
					 primary_script_ٹیگ,
					 خالی_سوچک, خالی_سوچک);
    *language = hb_ot_ٹیگ_منزل_language (language_ٹیگ);
    ثابت حرف *lang_str = *language ? hb_language_منزل_string (*language) : خالی_سوچک;
    إذا (lang_str && (script_گنتی == 0 || primary_script_ٹیگ[0] != script_ٹیگ))
    {
      بنانشانی حرف *buf;
      size_t len = لڑی_لمبائی (lang_str);
      buf = (بنانشانی حرف *) hb_malloc (len + 16);
      إذا (unlikely (!buf))
      {
	*language = خالی_سوچک;
      }
      وإلا
      {
	صحيح کھسکاؤ;
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
	لكل (کھسکاؤ = 28; کھسکاؤ >= 0; کھسکاؤ -= 4)
	  buf[len++] = TOHEX (script_ٹیگ >> کھسکاؤ);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#جے_تعریف_شدہ MAIN
ساكن مضمن فراغ
test_langs_sorted ()
{
  لكل (بنانشانی صحيح i = 1; i < ARRAY_لمائی (ot_languages2); i++)
  {
    صحيح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#جے_بنا_تعریف HB_NO_LANGUAGE_LONG
  لكل (بنانشانی صحيح i = 1; i < ARRAY_لمائی (ot_languages3); i++)
  {
    صحيح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  لكل (بنانشانی صحيح i = 1; i < ARRAY_لمائی (ot_languages3_multi); i++)
  {
    صحيح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  لكل (بنانشانی صحيح i = 1; i < ARRAY_لمائی (ot_languages3_blocked); i++)
    إذا (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#جے_انت
}

صحيح
مکھ ()
{
  test_langs_sorted ();
  أرجع 0;
}

#جے_انت


#جے_انت
