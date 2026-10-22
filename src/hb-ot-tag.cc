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

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_ٽيگ


/* hb_script_t */

ساكن hb_ٽيگ_t
hb_ot_old_ٽيگ_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  بدل ((hb_ٽيگ_t) script)
  {
    حالة HB_SCRIPT_INVALID:		أرجع HB_OT_ٽيگ_DEFAULT_SCRIPT;
    حالة HB_SCRIPT_MATH:		أرجع HB_OT_ٽيگ_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    حالة HB_SCRIPT_HIRAGANA:		أرجع HB_ٽيگ('k','a','n','a');
    حالة HB_ٽيگ('H','r','k','t'):
					أرجع HB_ٽيگ('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    حالة HB_SCRIPT_LAO:			أرجع HB_ٽيگ('l','a','o',' ');
    حالة HB_SCRIPT_YI:			أرجع HB_ٽيگ('y','i',' ',' ');
    /* Unicode-5.0 additions */
    حالة HB_SCRIPT_NKO:			أرجع HB_ٽيگ('n','k','o',' ');
    /* Unicode-5.1 additions */
    حالة HB_SCRIPT_VAI:			أرجع HB_ٽيگ('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  أرجع ((hb_ٽيگ_t) script) | 0x20000000u;
}

ساكن hb_script_t
hb_ot_old_ٽيگ_منزل_script (hb_ٽيگ_t ٽيگ_2)
{
  إذا (unlikely (ٽيگ_2 == HB_OT_ٽيگ_DEFAULT_SCRIPT))
    أرجع HB_SCRIPT_INVALID;
  إذا (unlikely (ٽيگ_2 == HB_OT_ٽيگ_MATH_SCRIPT))
    أرجع HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  إذا (unlikely ((ٽيگ_2 & 0x0000FF00u) == 0x00002000u))
    ٽيگ_2 |= (ٽيگ_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  إذا (unlikely ((ٽيگ_2 & 0x000000FFu) == 0x00000020u))
    ٽيگ_2 |= (ٽيگ_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  أرجع (hb_script_t) (ٽيگ_2 & ~0x20000000u);
}

ساكن hb_ٽيگ_t
hb_ot_new_ٽيگ_from_script (hb_script_t script)
{
  بدل ((hb_ٽيگ_t) script) {
    حالة HB_SCRIPT_BENGALI:		أرجع HB_ٽيگ('b','n','g','2');
    حالة HB_SCRIPT_DEVANAGARI:		أرجع HB_ٽيگ('d','e','v','2');
    حالة HB_SCRIPT_GUJARATI:		أرجع HB_ٽيگ('g','j','r','2');
    حالة HB_SCRIPT_GURMUKHI:		أرجع HB_ٽيگ('g','u','r','2');
    حالة HB_SCRIPT_KANNADA:		أرجع HB_ٽيگ('k','n','d','2');
    حالة HB_SCRIPT_MALAYALAM:		أرجع HB_ٽيگ('m','l','m','2');
    حالة HB_SCRIPT_ORIYA:		أرجع HB_ٽيگ('o','r','y','2');
    حالة HB_SCRIPT_TAMIL:		أرجع HB_ٽيگ('t','m','l','2');
    حالة HB_SCRIPT_TELUGU:		أرجع HB_ٽيگ('t','e','l','2');
    حالة HB_SCRIPT_MYANMAR:		أرجع HB_ٽيگ('m','y','m','2');
  }

  أرجع HB_OT_ٽيگ_DEFAULT_SCRIPT;
}

ساكن hb_script_t
hb_ot_new_ٽيگ_منزل_script (hb_ٽيگ_t ٽيگ_2)
{
  بدل (ٽيگ_2) {
    حالة HB_ٽيگ('b','n','g','2'):	أرجع HB_SCRIPT_BENGALI;
    حالة HB_ٽيگ('d','e','v','2'):	أرجع HB_SCRIPT_DEVANAGARI;
    حالة HB_ٽيگ('g','j','r','2'):	أرجع HB_SCRIPT_GUJARATI;
    حالة HB_ٽيگ('g','u','r','2'):	أرجع HB_SCRIPT_GURMUKHI;
    حالة HB_ٽيگ('k','n','d','2'):	أرجع HB_SCRIPT_KANNADA;
    حالة HB_ٽيگ('m','l','m','2'):	أرجع HB_SCRIPT_MALAYALAM;
    حالة HB_ٽيگ('o','r','y','2'):	أرجع HB_SCRIPT_ORIYA;
    حالة HB_ٽيگ('t','m','l','2'):	أرجع HB_SCRIPT_TAMIL;
    حالة HB_ٽيگ('t','e','l','2'):	أرجع HB_SCRIPT_TELUGU;
    حالة HB_ٽيگ('m','y','m','2'):	أرجع HB_SCRIPT_MYANMAR;
  }

  أرجع HB_SCRIPT_UNKNOWN;
}

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
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
hb_ot_ٽيگ_فهرست_from_script (hb_script_t  script,
			hb_ٽيگ_t    *script_ٽيگ_1,
			hb_ٽيگ_t    *script_ٽيگ_2)
{
  بنانشان صحيح ڳڻپ = 2;
  hb_ٽيگ_t ٽيگ_فهرست[2];
  hb_ot_ٽيگ_فهرست_from_script_and_language (script, HB_LANGUAGE_INVALID, &ڳڻپ, ٽيگ_فهرست, خالي_اشارو, خالي_اشارو);
  *script_ٽيگ_1 = ڳڻپ > 0 ? ٽيگ_فهرست[0] : HB_OT_ٽيگ_DEFAULT_SCRIPT;
  *script_ٽيگ_2 = ڳڻپ > 1 ? ٽيگ_فهرست[1] : HB_OT_ٽيگ_DEFAULT_SCRIPT;
}
#جيڪڏهن_پورو

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

ساكن فراغ
hb_ot_all_ٽيگ_فهرست_from_script (hb_script_t   script,
			    بنانشان صحيح *ڳڻپ /* IN/OUT */,
			    hb_ٽيگ_t     *ٽيگ_فهرست /*ٻاهريون*/)
{
  بنانشان صحيح i = 0;

  hb_ٽيگ_t new_ٽيگ = hb_ot_new_ٽيگ_from_script (script);
  إذا (unlikely (new_ٽيگ != HB_OT_ٽيگ_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    إذا (new_ٽيگ != HB_ٽيگ('m','y','m','2'))
      ٽيگ_فهرست[i++] = new_ٽيگ | '3';
    إذا (*ڳڻپ > i)
      ٽيگ_فهرست[i++] = new_ٽيگ;
  }

  إذا (*ڳڻپ > i)
  {
    hb_ٽيگ_t old_ٽيگ = hb_ot_old_ٽيگ_from_script (script);
    إذا (old_ٽيگ != HB_OT_ٽيگ_DEFAULT_SCRIPT)
      ٽيگ_فهرست[i++] = old_ٽيگ;
  }

  *ڳڻپ = i;
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
hb_ot_ٽيگ_منزل_script (hb_ٽيگ_t ٽيگ_2)
{
  بنانشان حرف digit = ٽيگ_2 & 0x000000FFu;
  إذا (unlikely (digit == '2' || digit == '3'))
    أرجع hb_ot_new_ٽيگ_منزل_script (ٽيگ_2 & 0xFFFFFF32);

  أرجع hb_ot_old_ٽيگ_منزل_script (ٽيگ_2);
}


/* hb_language_t */

ساكن مضمن منطقي
subtag_matches (ثابت حرف *lang_str,
		ثابت حرف *حد_بندي,
		ثابت حرف *subtag,
		بنانشان    subtag_len)
{
  إذا (likely ((بنانشان) (حد_بندي - lang_str) < subtag_len))
    أرجع خطأ;

  افعل {
    ثابت حرف *s = لڙي_ڳولا (lang_str, subtag);
    إذا (!s || s >= حد_بندي)
      أرجع خطأ;
    إذا (!ISALNUM (s[subtag_len]))
      أرجع سچ;
    lang_str = s + subtag_len;
  } طالما (سچ);
}

ساكن منطقي
lang_matches (ثابت حرف *lang_str,
	      ثابت حرف *حد_بندي,
	      ثابت حرف *spec,
	      بنانشان    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  إذا (likely ((بنانشان) (حد_بندي - lang_str) < spec_len))
    أرجع خطأ;

  أرجع لڙي_ڀيٽ (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

ساكن منطقي
bfind_ٽيگ (ثابت hb_ٽيگ_t *array,
	   بنانشان        len,
	   hb_ٽيگ_t        key)
{
  بنانشان min = 0;
  بنانشان max = len;

  طالما (min < max)
  {
    بنانشان mid = min + (max - min) / 2;
    hb_ٽيگ_t val = array[mid];
    إذا (key < val)
      max = mid;
    وإلا إذا (key > val)
      min = mid + 1;
    وإلا
      أرجع سچ;
  }

  أرجع خطأ;
}

بنية Lang_ٽيگ
{
  hb_ٽيگ_t language;
  hb_ٽيگ_t ٽيگ_2;

  صحيح cmp (hb_ٽيگ_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_ٽيگ *that) ثابت
  { أرجع cmp (that->language); }
};

بنية Lang_ٽيگ_Range
{
  hb_ٽيگ_t language;
  uint16_t وٿي;
  uint8_t ڳڻپ;

  صحيح cmp (hb_ٽيگ_t a) ثابت
  {
    أرجع a < هذا->language ? -1 : a > هذا->language ? +1 : 0;
  }
  صحيح cmp (ثابت Lang_ٽيگ_Range *that) ثابت
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

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_ٽيگ_t
hb_ot_ٽيگ_from_language (hb_language_t language)
{
  بنانشان صحيح ڳڻپ = 1;
  hb_ٽيگ_t ٽيگ_فهرست[1];
  hb_ot_ٽيگ_فهرست_from_script_and_language (HB_SCRIPT_UNKNOWN, language, خالي_اشارو, خالي_اشارو, &ڳڻپ, ٽيگ_فهرست);
  أرجع ڳڻپ > 0 ? ٽيگ_فهرست[0] : HB_OT_ٽيگ_DEFAULT_LANGUAGE;
}
#جيڪڏهن_پورو

ساكن فراغ
hb_ot_ٽيگ_فهرست_from_language (ثابت حرف   *lang_str,
			  ثابت حرف   *حد_بندي,
			  بنانشان صحيح *ڳڻپ,
			  hb_ٽيگ_t     *ٽيگ_فهرست)
{

#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  إذا (hb_ot_ٽيگ_فهرست_from_complex_language (lang_str, حد_بندي, ڳڻپ, ٽيگ_فهرست))
    أرجع;
#جيڪڏهن_پورو

  /* Find a language matching in the first component. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  ثابت حرف *s; s = strchr (lang_str, '-');
#جيڪڏهن_پورو
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
    إذا (s && حد_بندي - lang_str >= 6)
    {
      ثابت حرف *extlang_پڇاڙي = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      إذا (3 == (extlang_پڇاڙي ? extlang_پڇاڙي - s - 1 : لڙي_ڊيگهه (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#جيڪڏهن_پورو
    ثابت حرف *dash = strchr (lang_str, '-');
    بنانشان پهريون_len = dash ? dash - lang_str : حد_بندي - lang_str;
    hb_ٽيگ_t lang_ٽيگ = hb_ٽيگ_from_string (lang_str, پهريون_len);

    إذا (پهريون_len == 2)
    {
      ساكن hb_atomic_t<بنانشان> آخري_ٽيگ_idx_2 {0}; /* Poor man's cache. */
      بنانشان ٽيگ_idx = آخري_ٽيگ_idx_2;

      إذا (likely (ٽيگ_idx < ARRAY_ڊيگهائي (ot_languages2) &&
		  ot_languages2[ٽيگ_idx].language == lang_ٽيگ) ||
	  hb_sorted_array (ot_languages2).bfind (lang_ٽيگ, &ٽيگ_idx))
      {
	آخري_ٽيگ_idx_2 = ٽيگ_idx;
	بنانشان صحيح i;
	طالما (ٽيگ_idx != 0 &&
	       ot_languages2[ٽيگ_idx].language == ot_languages2[ٽيگ_idx - 1].language)
	  ٽيگ_idx--;
	لكل (i = 0;
	     i < *ڳڻپ &&
	     ٽيگ_idx + i < ARRAY_ڊيگهائي (ot_languages2) &&
	     ot_languages2[ٽيگ_idx + i].ٽيگ_2 != HB_ٽيگ_NONE &&
	     ot_languages2[ٽيگ_idx + i].language == ot_languages2[ٽيگ_idx].language;
	     i++)
	  ٽيگ_فهرست[i] = ot_languages2[ٽيگ_idx + i].ٽيگ_2;
	*ڳڻپ = i;
	أرجع;
      }
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
    وإلا إذا (پهريون_len == 3)
    {
      ساكن hb_atomic_t<بنانشان> آخري_ٽيگ_idx_3 {0}; /* Poor man's cache. */
      بنانشان ٽيگ_idx = آخري_ٽيگ_idx_3;

      إذا (likely (ٽيگ_idx < ARRAY_ڊيگهائي (ot_languages3) &&
		  ot_languages3[ٽيگ_idx].language == lang_ٽيگ) ||
	  hb_sorted_array (ot_languages3).bfind (lang_ٽيگ, &ٽيگ_idx))
      {
	آخري_ٽيگ_idx_3 = ٽيگ_idx;
	إذا (*ڳڻپ)
	{
	  ٽيگ_فهرست[0] = ot_languages3[ٽيگ_idx].ٽيگ_2;
	  *ڳڻپ = 1;
	}
	وإلا
	  *ڳڻپ = 0;
	أرجع;
      }

      ساكن hb_atomic_t<بنانشان> آخري_ٽيگ_idx_3_multi {0}; /* Poor man's cache. */
      بنانشان multi_ٽيگ_idx = آخري_ٽيگ_idx_3_multi;

      إذا (likely (multi_ٽيگ_idx < ARRAY_ڊيگهائي (ot_languages3_multi) &&
		  ot_languages3_multi[multi_ٽيگ_idx].language == lang_ٽيگ) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_ٽيگ, &multi_ٽيگ_idx))
      {
	آخري_ٽيگ_idx_3_multi = multi_ٽيگ_idx;
	ثابت Lang_ٽيگ_Range &range = ot_languages3_multi[multi_ٽيگ_idx];
	بنانشان صحيح i;
	لكل (i = 0; i < *ڳڻپ && i < range.ڳڻپ; i++)
	  ٽيگ_فهرست[i] = ot_languages3_multi_قدرون[range.وٿي + i];
	*ڳڻپ = i;
	أرجع;
      }

      إذا (bfind_ٽيگ (ot_languages3_blocked, ARRAY_ڊيگهائي (ot_languages3_blocked), lang_ٽيگ))
      {
	*ڳڻپ = 0;
	أرجع;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      ٽيگ_فهرست[0] = lang_ٽيگ & ~0x20202000u;
      *ڳڻپ = 1;
      أرجع;
    }
#جيڪڏهن_پورو
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  إذا (!s)
    s = lang_str + لڙي_ڊيگهه (lang_str);
  إذا (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    ٽيگ_فهرست[0] = hb_ٽيگ_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *ڳڻپ = 1;
    أرجع;
  }
#جيڪڏهن_پورو

  *ڳڻپ = 0;
}

ساكن منطقي
parse_ذاتي_use_subtag (ثابت حرف     *ذاتي_use_subtag,
			  بنانشان صحيح   *ڳڻپ,
			  hb_ٽيگ_t       *ٽيگ_فهرست,
			  ثابت حرف     *اڳياڙي,
			  بنانشان حرف (*normalize) (بنانشان حرف))
{
#جيڪڏهن_وصف_ٿيل HB_NO_LANGUAGE_ذاتي_SUBTAG
  أرجع خطأ;
#جيڪڏهن_پورو

  إذا (!(ذاتي_use_subtag && ڳڻپ && ٽيگ_فهرست && *ڳڻپ)) أرجع خطأ;

  ثابت حرف *s = لڙي_ڳولا (ذاتي_use_subtag, اڳياڙي);
  إذا (!s) أرجع خطأ;

  حرف ٽيگ_2[4];
  صحيح i;
  s += لڙي_ڊيگهه (اڳياڙي);
  إذا (s[0] == '-') {
    s += 1;
    حرف c;
    لكل (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      إذا (i % 2 == 0)
	ٽيگ_2[i / 2] = c << 4;
      وإلا
	ٽيگ_2[i / 2] += c;
    }
    إذا (i != 8) أرجع خطأ;
  } وإلا {
    لكل (i = 0; i < 4 && ISALNUM (s[i]); i++)
      ٽيگ_2[i] = normalize (s[i]);
    إذا (!i) أرجع خطأ;

    لكل (; i < 4; i++)
      ٽيگ_2[i] = ' ';
  }
  ٽيگ_فهرست[0] = HB_ٽيگ (ٽيگ_2[0], ٽيگ_2[1], ٽيگ_2[2], ٽيگ_2[3]);
  إذا ((ٽيگ_فهرست[0] & 0xDFDFDFDF) == HB_OT_ٽيگ_DEFAULT_SCRIPT)
    ٽيگ_فهرست[0] ^= ~0xDFDFDFDF;
  *ڳڻپ = 1;
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
hb_ot_ٽيگ_فهرست_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				     hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/,
				     بنانشان صحيح *language_ڳڻپ /* IN/OUT */,
				     hb_ٽيگ_t     *language_ٽيگ_فهرست /*ٻاهريون*/)
{
  منطقي needs_script = سچ;

  إذا (language == HB_LANGUAGE_INVALID)
  {
    إذا (language_ڳڻپ && language_ٽيگ_فهرست && *language_ڳڻپ)
      *language_ڳڻپ = 0;
  }
  وإلا
  {
    ثابت حرف *lang_str, *s, *حد_بندي, *ذاتي_use_subtag;
    منطقي needs_language;

    lang_str = hb_language_منزل_string (language);
    حد_بندي = خالي_اشارو;
    ذاتي_use_subtag = خالي_اشارو;
    إذا (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      ذاتي_use_subtag = lang_str;
    } وإلا {
      لكل (s = lang_str + 1; *s; s++)
      {
	إذا (s[-1] == '-' && s[1] == '-')
	{
	  إذا (s[0] == 'x')
	  {
	    ذاتي_use_subtag = s;
	    إذا (!حد_بندي)
	      حد_بندي = s - 1;
	    اكسر;
	  } وإلا إذا (!حد_بندي)
	  {
	    حد_بندي = s - 1;
	  }
	}
      }
      إذا (!حد_بندي)
	حد_بندي = s;
    }

    needs_script = !parse_ذاتي_use_subtag (ذاتي_use_subtag, script_ڳڻپ, script_ٽيگ_فهرست, "-hbsc", TOLOWER);
    needs_language = !parse_ذاتي_use_subtag (ذاتي_use_subtag, language_ڳڻپ, language_ٽيگ_فهرست, "-hbot", TOUPPER);

    إذا (needs_language && language_ڳڻپ && language_ٽيگ_فهرست && *language_ڳڻپ)
      hb_ot_ٽيگ_فهرست_from_language (lang_str, حد_بندي, language_ڳڻپ, language_ٽيگ_فهرست);
  }

  إذا (needs_script && script_ڳڻپ && script_ٽيگ_فهرست && *script_ڳڻپ)
    hb_ot_all_ٽيگ_فهرست_from_script (script, script_ڳڻپ, script_ٽيگ_فهرست);
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
hb_ot_ٽيگ_منزل_language (hb_ٽيگ_t ٽيگ_2)
{
  بنانشان صحيح i;

  إذا (ٽيگ_2 == HB_OT_ٽيگ_DEFAULT_LANGUAGE)
    أرجع خالي_اشارو;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_ٽيگ = hb_ot_ambiguous_ٽيگ_منزل_language (ٽيگ_2);
    إذا (disambiguated_ٽيگ != HB_LANGUAGE_INVALID)
      أرجع disambiguated_ٽيگ;
  }
#جيڪڏهن_پورو

  حرف buf[4];
  لكل (i = 0; i < ARRAY_ڊيگهائي (ot_languages2); i++)
    إذا (ot_languages2[i].ٽيگ_2 == ٽيگ_2)
    {
      hb_ٽيگ_منزل_string (ot_languages2[i].language, buf);
      أرجع hb_language_from_string (buf, 2);
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  لكل (i = 0; i < ARRAY_ڊيگهائي (ot_languages3); i++)
    إذا (ot_languages3[i].ٽيگ_2 == ٽيگ_2)
    {
      hb_ٽيگ_منزل_string (ot_languages3[i].language, buf);
      أرجع hb_language_from_string (buf, 3);
    }
  لكل (i = 0; i < ARRAY_ڊيگهائي (ot_languages3_multi); i++)
    لكل (بنانشان صحيح j = 0; j < ot_languages3_multi[i].ڳڻپ; j++)
      إذا (ot_languages3_multi_قدرون[ot_languages3_multi[i].وٿي + j] == ٽيگ_2)
      {
	hb_ٽيگ_منزل_string (ot_languages3_multi[i].language, buf);
	أرجع hb_language_from_string (buf, 3);
      }
#جيڪڏهن_پورو

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    حرف buf[20];
    حرف *str = buf;
    إذا (ISALPHA (ٽيگ_2 >> 24)
	&& ISALPHA ((ٽيگ_2 >> 16) & 0xFF)
	&& ISALPHA ((ٽيگ_2 >> 8) & 0xFF)
	&& (ٽيگ_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (ٽيگ_2 >> 24);
      buf[1] = TOLOWER ((ٽيگ_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((ٽيگ_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, ٽيگ_2);
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
hb_ot_ٽيگ_فهرست_منزل_script_and_language (hb_ٽيگ_t       script_ٽيگ,
				   hb_ٽيگ_t       language_ٽيگ,
				   hb_script_t   *script /*ٻاهريون*/,
				   hb_language_t *language /*ٻاهريون*/)
{
  hb_script_t script_ٻاهريون = hb_ot_ٽيگ_منزل_script (script_ٽيگ);
  إذا (script)
    *script = script_ٻاهريون;
  إذا (language)
  {
    بنانشان صحيح script_ڳڻپ = 1;
    hb_ٽيگ_t primary_script_ٽيگ[1];
    hb_ot_ٽيگ_فهرست_from_script_and_language (script_ٻاهريون,
					 HB_LANGUAGE_INVALID,
					 &script_ڳڻپ,
					 primary_script_ٽيگ,
					 خالي_اشارو, خالي_اشارو);
    *language = hb_ot_ٽيگ_منزل_language (language_ٽيگ);
    ثابت حرف *lang_str = *language ? hb_language_منزل_string (*language) : خالي_اشارو;
    إذا (lang_str && (script_ڳڻپ == 0 || primary_script_ٽيگ[0] != script_ٽيگ))
    {
      بنانشان حرف *buf;
      size_t len = لڙي_ڊيگهه (lang_str);
      buf = (بنانشان حرف *) hb_malloc (len + 16);
      إذا (unlikely (!buf))
      {
	*language = خالي_اشارو;
      }
      وإلا
      {
	صحيح سرڪايو;
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
	لكل (سرڪايو = 28; سرڪايو >= 0; سرڪايو -= 4)
	  buf[len++] = TOHEX (script_ٽيگ >> سرڪايو);
	*language = hb_language_from_string ((حرف *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#جيڪڏهن_وصف_ٿيل MAIN
ساكن مضمن فراغ
test_langs_sorted ()
{
  لكل (بنانشان صحيح i = 1; i < ARRAY_ڊيگهائي (ot_languages2); i++)
  {
    صحيح c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_LANGUAGE_LONG
  لكل (بنانشان صحيح i = 1; i < ARRAY_ڊيگهائي (ot_languages3); i++)
  {
    صحيح c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  لكل (بنانشان صحيح i = 1; i < ARRAY_ڊيگهائي (ot_languages3_multi); i++)
  {
    صحيح c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    إذا (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  لكل (بنانشان صحيح i = 1; i < ARRAY_ڊيگهائي (ot_languages3_blocked); i++)
    إذا (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#جيڪڏهن_پورو
}

صحيح
مکيه ()
{
  test_langs_sorted ();
  أرجع 0;
}

#جيڪڏهن_پورو


#جيڪڏهن_پورو
