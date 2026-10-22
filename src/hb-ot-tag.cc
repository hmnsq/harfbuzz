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

#که_نه_تعریف_شوی HB_NO_OT_نښان


/* hb_script_t */

ساكن hb_نښان_t
hb_ot_old_نښان_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  بدل ((hb_نښان_t) script)
  {
    حالة HB_SCRIPT_INVALID:		أرجع HB_OT_نښان_DEFAULT_SCRIPT;
    حالة HB_SCRIPT_MATH:		أرجع HB_OT_نښان_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالة HB_SCRIPT_HIRAGANA:		أرجع HB_نښان('k','a','n','a');
    حالة HB_نښان('H','r','k','t'):
					أرجع HB_نښان('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالة HB_SCRIPT_LAO:			أرجع HB_نښان('l','a','o',' ');
    حالة HB_SCRIPT_YI:			أرجع HB_نښان('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_NKO:			أرجع HB_نښان('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_VAI:			أرجع HB_نښان('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  أرجع ((hb_نښان_t) script) | 0x20000000u;
}

ساكن hb_script_t
hb_ot_old_نښان_منزل_script (hb_نښان_t نښان_2)
{
  إذا (unlikely (نښان_2 == HB_OT_نښان_DEFAULT_SCRIPT))
    أرجع HB_SCRIPT_INVALID;
  إذا (unlikely (نښان_2 == HB_OT_نښان_MATH_SCRIPT))
    أرجع HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  إذا (unlikely ((نښان_2 & 0x0000FF00u) == 0x00002000u))
    نښان_2 |= (نښان_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  إذا (unlikely ((نښان_2 & 0x000000FFu) == 0x00000020u))
    نښان_2 |= (نښان_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  أرجع (hb_script_t) (نښان_2 & ~0x20000000u);
}

ساكن hb_نښان_t
hb_ot_new_نښان_from_script (hb_script_t script)
{
  بدل ((hb_نښان_t) script) {
    حالة HB_SCRIPT_BENGALI:		أرجع HB_نښان('b','n','g','2');
    حالة HB_SCRIPT_DEVANAGARI:		أرجع HB_نښان('d','e','v','2');
    حالة HB_SCRIPT_GUJARATI:		أرجع HB_نښان('g','j','r','2');
    حالة HB_SCRIPT_GURMUKHI:		أرجع HB_نښان('g','u','r','2');
    حالة HB_SCRIPT_KANNADA:		أرجع HB_نښان('k','n','d','2');
    حالة HB_SCRIPT_MALAYALAM:		أرجع HB_نښان('m','l','m','2');
    حالة HB_SCRIPT_ORIYA:		أرجع HB_نښان('o','r','y','2');
    حالة HB_SCRIPT_TAMIL:		أرجع HB_نښان('t','m','l','2');
    حالة HB_SCRIPT_TELUGU:		أرجع HB_نښان('t','e','l','2');
    حالة HB_SCRIPT_MYANMAR:		أرجع HB_نښان('m','y','m','2');
  }

  أرجع HB_OT_نښان_DEFAULT_SCRIPT;
}

ساكن hb_script_t
hb_ot_new_نښان_منزل_script (hb_نښان_t نښان_2)
{
  بدل (نښان_2) {
    حالة HB_نښان('b','n','g','2'):	أرجع HB_SCRIPT_BENGALI;
    حالة HB_نښان('d','e','v','2'):	أرجع HB_SCRIPT_DEVANAGARI;
    حالة HB_نښان('g','j','r','2'):	أرجع HB_SCRIPT_GUJARATI;
    حالة HB_نښان('g','u','r','2'):	أرجع HB_SCRIPT_GURMUKHI;
    حالة HB_نښان('k','n','d','2'):	أرجع HB_SCRIPT_KANNADA;
    حالة HB_نښان('m','l','m','2'):	أرجع HB_SCRIPT_MALAYALAM;
    حالة HB_نښان('o','r','y','2'):	أرجع HB_SCRIPT_ORIYA;
    حالة HB_نښان('t','m','l','2'):	أرجع HB_SCRIPT_TAMIL;
    حالة HB_نښان('t','e','l','2'):	أرجع HB_SCRIPT_TELUGU;
    حالة HB_نښان('m','y','m','2'):	أرجع HB_SCRIPT_MYANMAR;
  }

  أرجع HB_SCRIPT_UNKNOWN;
}

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
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
hb_ot_نښانونه_from_script (hb_script_t  script,
			hb_نښان_t    *script_نښان_1,
			hb_نښان_t    *script_نښان_2)
{
  بینښه صحيح شمېر = 2;
  hb_نښان_t نښانونه[2];
  hb_ot_نښانونه_from_script_and_language (script, HB_LANGUAGE_INVALID, &شمېر, نښانونه, تش_اشاره, تش_اشاره);
  *script_نښان_1 = شمېر > 0 ? نښانونه[0] : HB_OT_نښان_DEFAULT_SCRIPT;
  *script_نښان_2 = شمېر > 1 ? نښانونه[1] : HB_OT_نښان_DEFAULT_SCRIPT;
}
#د_که_پای

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساكن فراغ
hb_ot_all_نښانونه_from_script (hb_script_t   script,
			    بینښه صحيح *شمېر /* IN/OUT */,
			    hb_نښان_t     *نښانونه /*بهر*/)
{
  بینښه صحيح i = 0;

  hb_نښان_t new_نښان = hb_ot_new_نښان_from_script (script);
  إذا (unlikely (new_نښان != HB_OT_نښان_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    إذا (new_نښان != HB_نښان('m','y','m','2'))
      نښانونه[i++] = new_نښان | '3';
    إذا (*شمېر > i)
      نښانونه[i++] = new_نښان;
  }

  إذا (*شمېر > i)
  {
    hb_نښان_t old_نښان = hb_ot_old_نښان_from_script (script);
    إذا (old_نښان != HB_OT_نښان_DEFAULT_SCRIPT)
      نښانونه[i++] = old_نښان;
  }

  *شمېر = i;
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
hb_ot_نښان_منزل_script (hb_نښان_t نښان_2)
{
  بینښه حرف digit = نښان_2 & 0x000000FFu;
  إذا (unlikely (digit == '2' || digit == '3'))
    أرجع hb_ot_new_نښان_منزل_script (نښان_2 & 0xFFFFFF32);

  أرجع hb_ot_old_نښان_منزل_script (نښان_2);
}


/* hb_language_t */

ساكن مضمن منطقي
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *حد,
		ثابت حرف *subtag,
		بینښه    subtag_len)
{
  إذا (likely ((بینښه) (حد - lang_str) < subtag_len))
    أرجع خطأ;

  افعل {
    ثابت حرف *s = مزي_پلټنه (lang_str, subtag);
    إذا (!s || s >= حد)
      أرجع خطأ;
    إذا (!ISALNUM (s[subtag_len]))
      أرجع ریښتیا;
    lang_str = s + subtag_len;
  } طالما (ریښتیا);
}

ساكن منطقي
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *حد,
	      ثابت حرف *spec,
	      بینښه    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  إذا (likely ((بینښه) (حد - lang_str) < spec_len))
    أرجع خطأ;

  أرجع مزي_پرتله (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساكن منطقي
bfind_نښان (ثابت hb_نښان_t *array,
	   بینښه        len,
	   hb_نښان_t        key)
{
  بینښه min = 0;
  بینښه max = len;

  طالما (min < max)
  {
    بینښه mid = min + (max - min) / 2;
    hb_نښان_t val = array[mid];
    إذا (key < val)
      max = mid;
    وإلا إذا (key > val)
      min = mid + 1;
    وإلا
      أرجع ریښتیا;
  }

  أرجع خطأ;
}

بنية Lang_نښان
{
  hb_نښان_t language;
  hb_نښان_t نښان_2;

  صحيح cmp (hb_نښان_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_نښان *that) ثابت
  { أرجع cmp (that->language); }
};

بنية Lang_نښان_Range
{
  hb_نښان_t language;
  uint16_t دننه_واټن;
  uint8_t شمېر;

  صحيح cmp (hb_نښان_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_نښان_Range *that) ثابت
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

#که_نه_تعریف_شوی HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_نښان_t
hb_ot_نښان_from_language (hb_language_t language)
{
  بینښه صحيح شمېر = 1;
  hb_نښان_t نښانونه[1];
  hb_ot_نښانونه_from_script_and_language (HB_SCRIPT_UNKNOWN, language, تش_اشاره, تش_اشاره, &شمېر, نښانونه);
  أرجع شمېر > 0 ? نښانونه[0] : HB_OT_نښان_DEFAULT_LANGUAGE;
}
#د_که_پای

ساكن فراغ
hb_ot_نښانونه_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *حد,
			  بینښه صحيح *شمېر,
			  hb_نښان_t     *نښانونه)
{

#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  إذا (hb_ot_نښانونه_from_complex_language (lang_str, حد, شمېر, نښانونه))
    أرجع;
#د_که_پای

  /* Find a language matching in the first component. */
#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#د_که_پای
  {
#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
    إذا (s && حد - lang_str >= 6)
    {
      ثابت حرف *extlang_پای_ټکی = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      إذا (3 == (extlang_پای_ټکی ? extlang_پای_ټکی - s - 1 : مزي_اوږدوالی (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#د_که_پای
    ثابت حرف *dash = strchr (lang_str, '-');
    بینښه لومړی_len = dash ? dash - lang_str : حد - lang_str;
    hb_نښان_t lang_نښان = hb_نښان_from_string (lang_str, لومړی_len);

    إذا (لومړی_len == 2)
    {
      ساكن hb_atomic_t<بینښه> وروستی_نښان_idx_2 {0}; /* Poor man's cache. */
      بینښه نښان_idx = وروستی_نښان_idx_2;

      إذا (likely (نښان_idx < ARRAY_اوږدتیا (ot_languages2) &&
		  ot_languages2[نښان_idx].language == lang_نښان) ||
	  hb_sorted_array (ot_languages2).bfind (lang_نښان, &نښان_idx))
      {
	وروستی_نښان_idx_2 = نښان_idx;
	بینښه صحيح i;
	طالما (نښان_idx != 0 &&
	       ot_languages2[نښان_idx].language == ot_languages2[نښان_idx - 1].language)
	  نښان_idx--;
	لكل (i = 0;
	     i < *شمېر &&
	     نښان_idx + i < ARRAY_اوږدتیا (ot_languages2) &&
	     ot_languages2[نښان_idx + i].نښان_2 != HB_نښان_NONE &&
	     ot_languages2[نښان_idx + i].language == ot_languages2[نښان_idx].language;
	     i++)
	  نښانونه[i] = ot_languages2[نښان_idx + i].نښان_2;
	*شمېر = i;
	أرجع;
      }
    }
#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
    وإلا إذا (لومړی_len == 3)
    {
      ساكن hb_atomic_t<بینښه> وروستی_نښان_idx_3 {0}; /* Poor man's cache. */
      بینښه نښان_idx = وروستی_نښان_idx_3;

      إذا (likely (نښان_idx < ARRAY_اوږدتیا (ot_languages3) &&
		  ot_languages3[نښان_idx].language == lang_نښان) ||
	  hb_sorted_array (ot_languages3).bfind (lang_نښان, &نښان_idx))
      {
	وروستی_نښان_idx_3 = نښان_idx;
	إذا (*شمېر)
	{
	  نښانونه[0] = ot_languages3[نښان_idx].نښان_2;
	  *شمېر = 1;
	}
	وإلا
	  *شمېر = 0;
	أرجع;
      }

      ساكن hb_atomic_t<بینښه> وروستی_نښان_idx_3_multi {0}; /* Poor man's cache. */
      بینښه multi_نښان_idx = وروستی_نښان_idx_3_multi;

      إذا (likely (multi_نښان_idx < ARRAY_اوږدتیا (ot_languages3_multi) &&
		  ot_languages3_multi[multi_نښان_idx].language == lang_نښان) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_نښان, &multi_نښان_idx))
      {
	وروستی_نښان_idx_3_multi = multi_نښان_idx;
	ثابت Lang_نښان_Range &range = ot_languages3_multi[multi_نښان_idx];
	بینښه صحيح i;
	لكل (i = 0; i < *شمېر && i < range.شمېر; i++)
	  نښانونه[i] = ot_languages3_multi_ارزښتونه[range.دننه_واټن + i];
	*شمېر = i;
	أرجع;
      }

      إذا (bfind_نښان (ot_languages3_blocked, ARRAY_اوږدتیا (ot_languages3_blocked), lang_نښان))
      {
	*شمېر = 0;
	أرجع;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      نښانونه[0] = lang_نښان & ~0x20202000u;
      *شمېر = 1;
      أرجع;
    }
#د_که_پای
  }

#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  إذا (!s)
    s = lang_str + مزي_اوږدوالی (lang_str);
  إذا (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    نښانونه[0] = hb_نښان_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *شمېر = 1;
    أرجع;
  }
#د_که_پای

  *شمېر = 0;
}

ساكن منطقي
parse_شخصي_use_subtag (ثابت حرف     *شخصي_use_subtag,
			  بینښه صحيح   *شمېر,
			  hb_نښان_t       *نښانونه,
			  ثابت حرف     *مختاړی,
			  بینښه حرف (*normalize) (بینښه حرف))
{
#که_تعریف_شوی HB_NO_LANGUAGE_شخصي_SUBTAG
  أرجع خطأ;
#د_که_پای

  إذا (!(شخصي_use_subtag && شمېر && نښانونه && *شمېر)) أرجع خطأ;

  ثابت حرف *s = مزي_پلټنه (شخصي_use_subtag, مختاړی);
  إذا (!s) أرجع خطأ;

  حرف نښان_2[4];
  صحيح i;
  s += مزي_اوږدوالی (مختاړی);
  إذا (s[0] == '-') {
    s += 1;
    حرف c;
    لكل (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      إذا (i % 2 == 0)
	نښان_2[i / 2] = c << 4;
      وإلا
	نښان_2[i / 2] += c;
    }
    إذا (i != 8) أرجع خطأ;
  } وإلا {
    لكل (i = 0; i < 4 && ISALNUM (s[i]); i++)
      نښان_2[i] = normalize (s[i]);
    إذا (!i) أرجع خطأ;

    لكل (; i < 4; i++)
      نښان_2[i] = ' ';
  }
  نښانونه[0] = HB_نښان (نښان_2[0], نښان_2[1], نښان_2[2], نښان_2[3]);
  إذا ((نښانونه[0] & 0xDFDFDFDF) == HB_OT_نښان_DEFAULT_SCRIPT)
    نښانونه[0] ^= ~0xDFDFDFDF;
  *شمېر = 1;
  أرجع ریښتیا;
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
hb_ot_نښانونه_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بینښه صحيح *script_شمېر /* IN/OUT */,
				     hb_نښان_t     *script_نښانونه /*بهر*/,
				     بینښه صحيح *language_شمېر /* IN/OUT */,
				     hb_نښان_t     *language_نښانونه /*بهر*/)
{
  منطقي needs_script = ریښتیا;

  إذا (language == HB_LANGUAGE_INVALID)
  {
    إذا (language_شمېر && language_نښانونه && *language_شمېر)
      *language_شمېر = 0;
  }
  وإلا
  {
    ثابت حرف *lang_str, *s, *حد, *شخصي_use_subtag;
    منطقي needs_language;

    lang_str = hb_language_منزل_string (language);
    حد = تش_اشاره;
    شخصي_use_subtag = تش_اشاره;
    إذا (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      شخصي_use_subtag = lang_str;
    } وإلا {
      لكل (s = lang_str + 1; *s; s++)
      {
	إذا (s[-1] == '-' && s[1] == '-')
	{
	  إذا (s[0] == 'x')
	  {
	    شخصي_use_subtag = s;
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

    needs_script = !parse_شخصي_use_subtag (شخصي_use_subtag, script_شمېر, script_نښانونه, "-hbsc", TOLOWER);
    needs_language = !parse_شخصي_use_subtag (شخصي_use_subtag, language_شمېر, language_نښانونه, "-hbot", TOUPPER);

    إذا (needs_language && language_شمېر && language_نښانونه && *language_شمېر)
      hb_ot_نښانونه_from_language (lang_str, حد, language_شمېر, language_نښانونه);
  }

  إذا (needs_script && script_شمېر && script_نښانونه && *script_شمېر)
    hb_ot_all_نښانونه_from_script (script, script_شمېر, script_نښانونه);
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
hb_ot_نښان_منزل_language (hb_نښان_t نښان_2)
{
  بینښه صحيح i;

  إذا (نښان_2 == HB_OT_نښان_DEFAULT_LANGUAGE)
    أرجع تش_اشاره;

#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_نښان = hb_ot_ambiguous_نښان_منزل_language (نښان_2);
    إذا (disambiguated_نښان != HB_LANGUAGE_INVALID)
      أرجع disambiguated_نښان;
  }
#د_که_پای

  حرف buf[4];
  لكل (i = 0; i < ARRAY_اوږدتیا (ot_languages2); i++)
    إذا (ot_languages2[i].نښان_2 == نښان_2)
    {
      hb_نښان_منزل_string (ot_languages2[i].language, buf);
      أرجع hb_language_from_string (buf, 2);
    }
#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  لكل (i = 0; i < ARRAY_اوږدتیا (ot_languages3); i++)
    إذا (ot_languages3[i].نښان_2 == نښان_2)
    {
      hb_نښان_منزل_string (ot_languages3[i].language, buf);
      أرجع hb_language_from_string (buf, 3);
    }
  لكل (i = 0; i < ARRAY_اوږدتیا (ot_languages3_multi); i++)
    لكل (بینښه صحيح j = 0; j < ot_languages3_multi[i].شمېر; j++)
      إذا (ot_languages3_multi_ارزښتونه[ot_languages3_multi[i].دننه_واټن + j] == نښان_2)
      {
	hb_نښان_منزل_string (ot_languages3_multi[i].language, buf);
	أرجع hb_language_from_string (buf, 3);
      }
#د_که_پای

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    إذا (ISALPHA (نښان_2 >> 24)
	&& ISALPHA ((نښان_2 >> 16) & 0xFF)
	&& ISALPHA ((نښان_2 >> 8) & 0xFF)
	&& (نښان_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (نښان_2 >> 24);
      buf[1] = TOLOWER ((نښان_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((نښان_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, نښان_2);
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
hb_ot_نښانونه_منزل_script_and_language (hb_نښان_t       script_نښان,
				   hb_نښان_t       language_نښان,
				   hb_script_t   *script /*بهر*/,
				   hb_language_t *language /*بهر*/)
{
  hb_script_t script_بهر = hb_ot_نښان_منزل_script (script_نښان);
  إذا (script)
    *script = script_بهر;
  إذا (language)
  {
    بینښه صحيح script_شمېر = 1;
    hb_نښان_t primary_script_نښان[1];
    hb_ot_نښانونه_from_script_and_language (script_بهر,
					 HB_LANGUAGE_INVALID,
					 &script_شمېر,
					 primary_script_نښان,
					 تش_اشاره, تش_اشاره);
    *language = hb_ot_نښان_منزل_language (language_نښان);
    ثابت حرف *lang_str = *language ? hb_language_منزل_string (*language) : تش_اشاره;
    إذا (lang_str && (script_شمېر == 0 || primary_script_نښان[0] != script_نښان))
    {
      بینښه حرف *buf;
      size_t len = مزي_اوږدوالی (lang_str);
      buf = (بینښه حرف *) hb_malloc (len + 16);
      إذا (unlikely (!buf))
      {
	*language = تش_اشاره;
      }
      وإلا
      {
	صحيح ښويدنه;
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
	لكل (ښويدنه = 28; ښويدنه >= 0; ښويدنه -= 4)
	  buf[len++] = TOHEX (script_نښان >> ښويدنه);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#که_تعریف_شوی MAIN
ساكن مضمن فراغ
test_langs_sorted ()
{
  لكل (بینښه صحيح i = 1; i < ARRAY_اوږدتیا (ot_languages2); i++)
  {
    صحيح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#که_نه_تعریف_شوی HB_NO_LANGUAGE_LONG
  لكل (بینښه صحيح i = 1; i < ARRAY_اوږدتیا (ot_languages3); i++)
  {
    صحيح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  لكل (بینښه صحيح i = 1; i < ARRAY_اوږدتیا (ot_languages3_multi); i++)
  {
    صحيح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  لكل (بینښه صحيح i = 1; i < ARRAY_اوږدتیا (ot_languages3_blocked); i++)
    إذا (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#د_که_پای
}

صحيح
اصلي ()
{
  test_langs_sorted ();
  أرجع 0;
}

#د_که_پای


#د_که_پای
