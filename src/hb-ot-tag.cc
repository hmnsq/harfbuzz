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

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OT_ట్యాగ్


/* hb_script_t */

స్థిర hb_ట్యాగ్_t
hb_ot_old_ట్యాగ్_from_script (hb_script_t script)
{
  /* This seems to be accurate as of end of 2012. */

  ఎంపిక ((hb_ట్యాగ్_t) script)
  {
    స్థితి HB_SCRIPT_INVALID:		తిరిగి HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
    స్థితి HB_SCRIPT_MATH:		తిరిగి HB_OT_ట్యాగ్_MATH_SCRIPT;

    /* Hiragana, Katakana, and their collective ISO 15924 script all map to 'kana'. */
    స్థితి HB_SCRIPT_HIRAGANA:		తిరిగి HB_ట్యాగ్('k','a','n','a');
    స్థితి HB_ట్యాగ్('H','r','k','t'):
					తిరిగి HB_ట్యాగ్('k','a','n','a');

    /* Spaces at the end are preserved, unlike ISO 15924 */
    స్థితి HB_SCRIPT_LAO:			తిరిగి HB_ట్యాగ్('l','a','o',' ');
    స్థితి HB_SCRIPT_YI:			తిరిగి HB_ట్యాగ్('y','i',' ',' ');
    /* Unicode-5.0 additions */
    స్థితి HB_SCRIPT_NKO:			తిరిగి HB_ట్యాగ్('n','k','o',' ');
    /* Unicode-5.1 additions */
    స్థితి HB_SCRIPT_VAI:			తిరిగి HB_ట్యాగ్('v','a','i',' ');
  }

  /* Else, just change first char to lowercase and return */
  తిరిగి ((hb_ట్యాగ్_t) script) | 0x20000000u;
}

స్థిర hb_script_t
hb_ot_old_ట్యాగ్_గమ్యం_script (hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఒకవేళ (unlikely (ట్యాగ్_2 == HB_OT_ట్యాగ్_DEFAULT_SCRIPT))
    తిరిగి HB_SCRIPT_INVALID;
  ఒకవేళ (unlikely (ట్యాగ్_2 == HB_OT_ట్యాగ్_MATH_SCRIPT))
    తిరిగి HB_SCRIPT_MATH;

  /* This side of the conversion is fully algorithmic. */

  /* Any spaces at the end of the tag are replaced by repeating the last
   * letter.  Eg 'nko ' -> 'Nkoo' */
  ఒకవేళ (unlikely ((ట్యాగ్_2 & 0x0000FF00u) == 0x00002000u))
    ట్యాగ్_2 |= (ట్యాగ్_2 >> 8) & 0x0000FF00u; /* Copy second letter to third */
  ఒకవేళ (unlikely ((ట్యాగ్_2 & 0x000000FFu) == 0x00000020u))
    ట్యాగ్_2 |= (ట్యాగ్_2 >> 8) & 0x000000FFu; /* Copy third letter to fourth */

  /* Change first char to uppercase and return */
  తిరిగి (hb_script_t) (ట్యాగ్_2 & ~0x20000000u);
}

స్థిర hb_ట్యాగ్_t
hb_ot_new_ట్యాగ్_from_script (hb_script_t script)
{
  ఎంపిక ((hb_ట్యాగ్_t) script) {
    స్థితి HB_SCRIPT_BENGALI:		తిరిగి HB_ట్యాగ్('b','n','g','2');
    స్థితి HB_SCRIPT_DEVANAGARI:		తిరిగి HB_ట్యాగ్('d','e','v','2');
    స్థితి HB_SCRIPT_GUJARATI:		తిరిగి HB_ట్యాగ్('g','j','r','2');
    స్థితి HB_SCRIPT_GURMUKHI:		తిరిగి HB_ట్యాగ్('g','u','r','2');
    స్థితి HB_SCRIPT_KANNADA:		తిరిగి HB_ట్యాగ్('k','n','d','2');
    స్థితి HB_SCRIPT_MALAYALAM:		తిరిగి HB_ట్యాగ్('m','l','m','2');
    స్థితి HB_SCRIPT_ORIYA:		తిరిగి HB_ట్యాగ్('o','r','y','2');
    స్థితి HB_SCRIPT_TAMIL:		తిరిగి HB_ట్యాగ్('t','m','l','2');
    స్థితి HB_SCRIPT_TELUGU:		తిరిగి HB_ట్యాగ్('t','e','l','2');
    స్థితి HB_SCRIPT_MYANMAR:		తిరిగి HB_ట్యాగ్('m','y','m','2');
  }

  తిరిగి HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
}

స్థిర hb_script_t
hb_ot_new_ట్యాగ్_గమ్యం_script (hb_ట్యాగ్_t ట్యాగ్_2)
{
  ఎంపిక (ట్యాగ్_2) {
    స్థితి HB_ట్యాగ్('b','n','g','2'):	తిరిగి HB_SCRIPT_BENGALI;
    స్థితి HB_ట్యాగ్('d','e','v','2'):	తిరిగి HB_SCRIPT_DEVANAGARI;
    స్థితి HB_ట్యాగ్('g','j','r','2'):	తిరిగి HB_SCRIPT_GUJARATI;
    స్థితి HB_ట్యాగ్('g','u','r','2'):	తిరిగి HB_SCRIPT_GURMUKHI;
    స్థితి HB_ట్యాగ్('k','n','d','2'):	తిరిగి HB_SCRIPT_KANNADA;
    స్థితి HB_ట్యాగ్('m','l','m','2'):	తిరిగి HB_SCRIPT_MALAYALAM;
    స్థితి HB_ట్యాగ్('o','r','y','2'):	తిరిగి HB_SCRIPT_ORIYA;
    స్థితి HB_ట్యాగ్('t','m','l','2'):	తిరిగి HB_SCRIPT_TAMIL;
    స్థితి HB_ట్యాగ్('t','e','l','2'):	తిరిగి HB_SCRIPT_TELUGU;
    స్థితి HB_ట్యాగ్('m','y','m','2'):	తిరిగి HB_SCRIPT_MYANMAR;
  }

  తిరిగి HB_SCRIPT_UNKNOWN;
}

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
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
శూన్య
hb_ot_ట్యాగులు_from_script (hb_script_t  script,
			hb_ట్యాగ్_t    *script_ట్యాగ్_1,
			hb_ట్యాగ్_t    *script_ట్యాగ్_2)
{
  గుర్తులేని పూర్ణాంకం లెక్క = 2;
  hb_ట్యాగ్_t ట్యాగులు[2];
  hb_ot_ట్యాగులు_from_script_and_language (script, HB_LANGUAGE_INVALID, &లెక్క, ట్యాగులు, శూన్యసూచిక, శూన్యసూచిక);
  *script_ట్యాగ్_1 = లెక్క > 0 ? ట్యాగులు[0] : HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
  *script_ట్యాగ్_2 = లెక్క > 1 ? ట్యాగులు[1] : HB_OT_ట్యాగ్_DEFAULT_SCRIPT;
}
#అయితే_ముగింపు

/*
 * Complete list at:
 * https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
 *
 * Most of the script tags are the same as the ISO 15924 tag but lowercased.
 * So we just do that, and handle the exceptional cases in a switch.
 */

స్థిర శూన్య
hb_ot_all_ట్యాగులు_from_script (hb_script_t   script,
			    గుర్తులేని పూర్ణాంకం *లెక్క /* IN/OUT */,
			    hb_ట్యాగ్_t     *ట్యాగులు /*బహిర్గతం*/)
{
  గుర్తులేని పూర్ణాంకం i = 0;

  hb_ట్యాగ్_t new_ట్యాగ్ = hb_ot_new_ట్యాగ్_from_script (script);
  ఒకవేళ (unlikely (new_ట్యాగ్ != HB_OT_ట్యాగ్_DEFAULT_SCRIPT))
  {
    /* HB_SCRIPT_MYANMAR maps to 'mym2', but there is no 'mym3'. */
    ఒకవేళ (new_ట్యాగ్ != HB_ట్యాగ్('m','y','m','2'))
      ట్యాగులు[i++] = new_ట్యాగ్ | '3';
    ఒకవేళ (*లెక్క > i)
      ట్యాగులు[i++] = new_ట్యాగ్;
  }

  ఒకవేళ (*లెక్క > i)
  {
    hb_ట్యాగ్_t old_ట్యాగ్ = hb_ot_old_ట్యాగ్_from_script (script);
    ఒకవేళ (old_ట్యాగ్ != HB_OT_ట్యాగ్_DEFAULT_SCRIPT)
      ట్యాగులు[i++] = old_ట్యాగ్;
  }

  *లెక్క = i;
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
hb_ot_ట్యాగ్_గమ్యం_script (hb_ట్యాగ్_t ట్యాగ్_2)
{
  గుర్తులేని అక్షరం digit = ట్యాగ్_2 & 0x000000FFu;
  ఒకవేళ (unlikely (digit == '2' || digit == '3'))
    తిరిగి hb_ot_new_ట్యాగ్_గమ్యం_script (ట్యాగ్_2 & 0xFFFFFF32);

  తిరిగి hb_ot_old_ట్యాగ్_గమ్యం_script (ట్యాగ్_2);
}


/* hb_language_t */

స్థిర అంతఃవరుస తార్కిక
subtag_matches (స్థిరరాశి అక్షరం *lang_str,
		స్థిరరాశి అక్షరం *హద్దు,
		స్థిరరాశి అక్షరం *subtag,
		గుర్తులేని    subtag_len)
{
  ఒకవేళ (likely ((గుర్తులేని) (హద్దు - lang_str) < subtag_len))
    తిరిగి అబద్ధం;

  చేయి {
    స్థిరరాశి అక్షరం *s = అక్షరశ్రేణి_శోధన (lang_str, subtag);
    ఒకవేళ (!s || s >= హద్దు)
      తిరిగి అబద్ధం;
    ఒకవేళ (!ISALNUM (s[subtag_len]))
      తిరిగి నిజం;
    lang_str = s + subtag_len;
  } అయితే (నిజం);
}

స్థిర తార్కిక
lang_matches (స్థిరరాశి అక్షరం *lang_str,
	      స్థిరరాశి అక్షరం *హద్దు,
	      స్థిరరాశి అక్షరం *spec,
	      గుర్తులేని    spec_len)
{
  /* Same as hb_language_matches(); duplicated. */

  ఒకవేళ (likely ((గుర్తులేని) (హద్దు - lang_str) < spec_len))
    తిరిగి అబద్ధం;

  తిరిగి అక్షరశ్రేణి_పోలిక (lang_str, spec, spec_len) == 0 &&
	 (lang_str[spec_len] == '\0' || lang_str[spec_len] == '-');
}

స్థిర తార్కిక
bfind_ట్యాగ్ (స్థిరరాశి hb_ట్యాగ్_t *array,
	   గుర్తులేని        len,
	   hb_ట్యాగ్_t        key)
{
  గుర్తులేని min = 0;
  గుర్తులేని max = len;

  అయితే (min < max)
  {
    గుర్తులేని mid = min + (max - min) / 2;
    hb_ట్యాగ్_t val = array[mid];
    ఒకవేళ (key < val)
      max = mid;
    లేకపోతే ఒకవేళ (key > val)
      min = mid + 1;
    లేకపోతే
      తిరిగి నిజం;
  }

  తిరిగి అబద్ధం;
}

నిర్మాణం Lang_ట్యాగ్
{
  hb_ట్యాగ్_t language;
  hb_ట్యాగ్_t ట్యాగ్_2;

  పూర్ణాంకం cmp (hb_ట్యాగ్_t a) స్థిరరాశి
  {
    తిరిగి a < ఇది->language ? -1 : a > ఇది->language ? +1 : 0;
  }
  పూర్ణాంకం cmp (స్థిరరాశి Lang_ట్యాగ్ *that) స్థిరరాశి
  { తిరిగి cmp (that->language); }
};

నిర్మాణం Lang_ట్యాగ్_Range
{
  hb_ట్యాగ్_t language;
  uint16_t స్థానభ్రంశం;
  uint8_t లెక్క;

  పూర్ణాంకం cmp (hb_ట్యాగ్_t a) స్థిరరాశి
  {
    తిరిగి a < ఇది->language ? -1 : a > ఇది->language ? +1 : 0;
  }
  పూర్ణాంకం cmp (స్థిరరాశి Lang_ట్యాగ్_Range *that) స్థిరరాశి
  { తిరిగి cmp (that->language); }
};

#అన్తర్భావ "hb-ot-tag-table.hh"

/* The corresponding languages IDs for the following IDs are unclear,
 * overlap, or are architecturally weird. Needs more research. */

/*{"??",	{HB_TAG('B','C','R',' ')}},*/	/* Bible Cree */
/*{"zh?",	{HB_TAG('C','H','N',' ')}},*/	/* Chinese (seen in Microsoft fonts) */
/*{"ar-Syrc?",	{HB_TAG('G','A','R',' ')}},*/	/* Garshuni */
/*{"??",	{HB_TAG('N','G','R',' ')}},*/	/* Nagari */
/*{"??",	{HB_TAG('Y','I','C',' ')}},*/	/* Yi Classic */
/*{"zh?",	{HB_TAG('Z','H','P',' ')}},*/	/* Chinese Phonetic */

#నిర్వచితం_కాకపోతే HB_DISABLE_DEPRECATED
/**
 * hb_ot_tag_from_language:
 * @language: an #hb_language_t to convert.
 *
 * Converts an #hb_language_t to an #hb_tag_t.
 *
 * Since: 0.6.0
 * Deprecated: 2.0.0: use hb_ot_tags_from_script_and_language() instead
 **/
hb_ట్యాగ్_t
hb_ot_ట్యాగ్_from_language (hb_language_t language)
{
  గుర్తులేని పూర్ణాంకం లెక్క = 1;
  hb_ట్యాగ్_t ట్యాగులు[1];
  hb_ot_ట్యాగులు_from_script_and_language (HB_SCRIPT_UNKNOWN, language, శూన్యసూచిక, శూన్యసూచిక, &లెక్క, ట్యాగులు);
  తిరిగి లెక్క > 0 ? ట్యాగులు[0] : HB_OT_ట్యాగ్_DEFAULT_LANGUAGE;
}
#అయితే_ముగింపు

స్థిర శూన్య
hb_ot_ట్యాగులు_from_language (స్థిరరాశి అక్షరం   *lang_str,
			  స్థిరరాశి అక్షరం   *హద్దు,
			  గుర్తులేని పూర్ణాంకం *లెక్క,
			  hb_ట్యాగ్_t     *ట్యాగులు)
{

#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  /* Check for matches of multiple subtags. */
  ఒకవేళ (hb_ot_ట్యాగులు_from_complex_language (lang_str, హద్దు, లెక్క, ట్యాగులు))
    తిరిగి;
#అయితే_ముగింపు

  /* Find a language matching in the first component. */
#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  స్థిరరాశి అక్షరం *s; s = strchr (lang_str, '-');
#అయితే_ముగింపు
  {
#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
    ఒకవేళ (s && హద్దు - lang_str >= 6)
    {
      స్థిరరాశి అక్షరం *extlang_చివర = strchr (s + 1, '-');
      /* If there is an extended language tag, use it. */
      ఒకవేళ (3 == (extlang_చివర ? extlang_చివర - s - 1 : అక్షరశ్రేణి_పొడవు (s + 1)) &&
	  ISALPHA (s[1]))
	lang_str = s + 1;
    }
#అయితే_ముగింపు
    స్థిరరాశి అక్షరం *dash = strchr (lang_str, '-');
    గుర్తులేని మొదటి_len = dash ? dash - lang_str : హద్దు - lang_str;
    hb_ట్యాగ్_t lang_ట్యాగ్ = hb_ట్యాగ్_from_string (lang_str, మొదటి_len);

    ఒకవేళ (మొదటి_len == 2)
    {
      స్థిర hb_atomic_t<గుర్తులేని> చివరి_ట్యాగ్_idx_2 {0}; /* Poor man's cache. */
      గుర్తులేని ట్యాగ్_idx = చివరి_ట్యాగ్_idx_2;

      ఒకవేళ (likely (ట్యాగ్_idx < ARRAY_నిడివి (ot_languages2) &&
		  ot_languages2[ట్యాగ్_idx].language == lang_ట్యాగ్) ||
	  hb_sorted_array (ot_languages2).bfind (lang_ట్యాగ్, &ట్యాగ్_idx))
      {
	చివరి_ట్యాగ్_idx_2 = ట్యాగ్_idx;
	గుర్తులేని పూర్ణాంకం i;
	అయితే (ట్యాగ్_idx != 0 &&
	       ot_languages2[ట్యాగ్_idx].language == ot_languages2[ట్యాగ్_idx - 1].language)
	  ట్యాగ్_idx--;
	కొరకు (i = 0;
	     i < *లెక్క &&
	     ట్యాగ్_idx + i < ARRAY_నిడివి (ot_languages2) &&
	     ot_languages2[ట్యాగ్_idx + i].ట్యాగ్_2 != HB_ట్యాగ్_NONE &&
	     ot_languages2[ట్యాగ్_idx + i].language == ot_languages2[ట్యాగ్_idx].language;
	     i++)
	  ట్యాగులు[i] = ot_languages2[ట్యాగ్_idx + i].ట్యాగ్_2;
	*లెక్క = i;
	తిరిగి;
      }
    }
#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
    లేకపోతే ఒకవేళ (మొదటి_len == 3)
    {
      స్థిర hb_atomic_t<గుర్తులేని> చివరి_ట్యాగ్_idx_3 {0}; /* Poor man's cache. */
      గుర్తులేని ట్యాగ్_idx = చివరి_ట్యాగ్_idx_3;

      ఒకవేళ (likely (ట్యాగ్_idx < ARRAY_నిడివి (ot_languages3) &&
		  ot_languages3[ట్యాగ్_idx].language == lang_ట్యాగ్) ||
	  hb_sorted_array (ot_languages3).bfind (lang_ట్యాగ్, &ట్యాగ్_idx))
      {
	చివరి_ట్యాగ్_idx_3 = ట్యాగ్_idx;
	ఒకవేళ (*లెక్క)
	{
	  ట్యాగులు[0] = ot_languages3[ట్యాగ్_idx].ట్యాగ్_2;
	  *లెక్క = 1;
	}
	లేకపోతే
	  *లెక్క = 0;
	తిరిగి;
      }

      స్థిర hb_atomic_t<గుర్తులేని> చివరి_ట్యాగ్_idx_3_multi {0}; /* Poor man's cache. */
      గుర్తులేని multi_ట్యాగ్_idx = చివరి_ట్యాగ్_idx_3_multi;

      ఒకవేళ (likely (multi_ట్యాగ్_idx < ARRAY_నిడివి (ot_languages3_multi) &&
		  ot_languages3_multi[multi_ట్యాగ్_idx].language == lang_ట్యాగ్) ||
	  hb_sorted_array (ot_languages3_multi).bfind (lang_ట్యాగ్, &multi_ట్యాగ్_idx))
      {
	చివరి_ట్యాగ్_idx_3_multi = multi_ట్యాగ్_idx;
	స్థిరరాశి Lang_ట్యాగ్_Range &range = ot_languages3_multi[multi_ట్యాగ్_idx];
	గుర్తులేని పూర్ణాంకం i;
	కొరకు (i = 0; i < *లెక్క && i < range.లెక్క; i++)
	  ట్యాగులు[i] = ot_languages3_multi_విలువలు[range.స్థానభ్రంశం + i];
	*లెక్క = i;
	తిరిగి;
      }

      ఒకవేళ (bfind_ట్యాగ్ (ot_languages3_blocked, ARRAY_నిడివి (ot_languages3_blocked), lang_ట్యాగ్))
      {
	*లెక్క = 0;
	తిరిగి;
      }

      /* Assume it's ISO-639-3 and upper-case and use it. */
      ట్యాగులు[0] = lang_ట్యాగ్ & ~0x20202000u;
      *లెక్క = 1;
      తిరిగి;
    }
#అయితే_ముగింపు
  }

#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  ఒకవేళ (!s)
    s = lang_str + అక్షరశ్రేణి_పొడవు (lang_str);
  ఒకవేళ (s - lang_str == 3) {
    /* Assume it's ISO-639-3 and upper-case and use it. */
    ట్యాగులు[0] = hb_ట్యాగ్_from_string (lang_str, s - lang_str) & ~0x20202000u;
    *లెక్క = 1;
    తిరిగి;
  }
#అయితే_ముగింపు

  *లెక్క = 0;
}

స్థిర తార్కిక
parse_వ్యక్తిగత_use_subtag (స్థిరరాశి అక్షరం     *వ్యక్తిగత_use_subtag,
			  గుర్తులేని పూర్ణాంకం   *లెక్క,
			  hb_ట్యాగ్_t       *ట్యాగులు,
			  స్థిరరాశి అక్షరం     *ఉపసర్గ,
			  గుర్తులేని అక్షరం (*normalize) (గుర్తులేని అక్షరం))
{
#నిర్వచితమైతే HB_NO_LANGUAGE_వ్యక్తిగత_SUBTAG
  తిరిగి అబద్ధం;
#అయితే_ముగింపు

  ఒకవేళ (!(వ్యక్తిగత_use_subtag && లెక్క && ట్యాగులు && *లెక్క)) తిరిగి అబద్ధం;

  స్థిరరాశి అక్షరం *s = అక్షరశ్రేణి_శోధన (వ్యక్తిగత_use_subtag, ఉపసర్గ);
  ఒకవేళ (!s) తిరిగి అబద్ధం;

  అక్షరం ట్యాగ్_2[4];
  పూర్ణాంకం i;
  s += అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
  ఒకవేళ (s[0] == '-') {
    s += 1;
    అక్షరం c;
    కొరకు (i = 0; i < 8 && ISHEX (s[i]); i++)
    {
      c = FROMHEX (s[i]);
      ఒకవేళ (i % 2 == 0)
	ట్యాగ్_2[i / 2] = c << 4;
      లేకపోతే
	ట్యాగ్_2[i / 2] += c;
    }
    ఒకవేళ (i != 8) తిరిగి అబద్ధం;
  } లేకపోతే {
    కొరకు (i = 0; i < 4 && ISALNUM (s[i]); i++)
      ట్యాగ్_2[i] = normalize (s[i]);
    ఒకవేళ (!i) తిరిగి అబద్ధం;

    కొరకు (; i < 4; i++)
      ట్యాగ్_2[i] = ' ';
  }
  ట్యాగులు[0] = HB_ట్యాగ్ (ట్యాగ్_2[0], ట్యాగ్_2[1], ట్యాగ్_2[2], ట్యాగ్_2[3]);
  ఒకవేళ ((ట్యాగులు[0] & 0xDFDFDFDF) == HB_OT_ట్యాగ్_DEFAULT_SCRIPT)
    ట్యాగులు[0] ^= ~0xDFDFDFDF;
  *లెక్క = 1;
  తిరిగి నిజం;
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
శూన్య
hb_ot_ట్యాగులు_from_script_and_language (hb_script_t   script,
				     hb_language_t language,
				     గుర్తులేని పూర్ణాంకం *script_లెక్క /* IN/OUT */,
				     hb_ట్యాగ్_t     *script_ట్యాగులు /*బహిర్గతం*/,
				     గుర్తులేని పూర్ణాంకం *language_లెక్క /* IN/OUT */,
				     hb_ట్యాగ్_t     *language_ట్యాగులు /*బహిర్గతం*/)
{
  తార్కిక needs_script = నిజం;

  ఒకవేళ (language == HB_LANGUAGE_INVALID)
  {
    ఒకవేళ (language_లెక్క && language_ట్యాగులు && *language_లెక్క)
      *language_లెక్క = 0;
  }
  లేకపోతే
  {
    స్థిరరాశి అక్షరం *lang_str, *s, *హద్దు, *వ్యక్తిగత_use_subtag;
    తార్కిక needs_language;

    lang_str = hb_language_గమ్యం_string (language);
    హద్దు = శూన్యసూచిక;
    వ్యక్తిగత_use_subtag = శూన్యసూచిక;
    ఒకవేళ (lang_str[0] == 'x' && lang_str[1] == '-')
    {
      వ్యక్తిగత_use_subtag = lang_str;
    } లేకపోతే {
      కొరకు (s = lang_str + 1; *s; s++)
      {
	ఒకవేళ (s[-1] == '-' && s[1] == '-')
	{
	  ఒకవేళ (s[0] == 'x')
	  {
	    వ్యక్తిగత_use_subtag = s;
	    ఒకవేళ (!హద్దు)
	      హద్దు = s - 1;
	    ఆపు;
	  } లేకపోతే ఒకవేళ (!హద్దు)
	  {
	    హద్దు = s - 1;
	  }
	}
      }
      ఒకవేళ (!హద్దు)
	హద్దు = s;
    }

    needs_script = !parse_వ్యక్తిగత_use_subtag (వ్యక్తిగత_use_subtag, script_లెక్క, script_ట్యాగులు, "-hbsc", TOLOWER);
    needs_language = !parse_వ్యక్తిగత_use_subtag (వ్యక్తిగత_use_subtag, language_లెక్క, language_ట్యాగులు, "-hbot", TOUPPER);

    ఒకవేళ (needs_language && language_లెక్క && language_ట్యాగులు && *language_లెక్క)
      hb_ot_ట్యాగులు_from_language (lang_str, హద్దు, language_లెక్క, language_ట్యాగులు);
  }

  ఒకవేళ (needs_script && script_లెక్క && script_ట్యాగులు && *script_లెక్క)
    hb_ot_all_ట్యాగులు_from_script (script, script_లెక్క, script_ట్యాగులు);
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
hb_ot_ట్యాగ్_గమ్యం_language (hb_ట్యాగ్_t ట్యాగ్_2)
{
  గుర్తులేని పూర్ణాంకం i;

  ఒకవేళ (ట్యాగ్_2 == HB_OT_ట్యాగ్_DEFAULT_LANGUAGE)
    తిరిగి శూన్యసూచిక;

#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  {
    hb_language_t disambiguated_ట్యాగ్ = hb_ot_ambiguous_ట్యాగ్_గమ్యం_language (ట్యాగ్_2);
    ఒకవేళ (disambiguated_ట్యాగ్ != HB_LANGUAGE_INVALID)
      తిరిగి disambiguated_ట్యాగ్;
  }
#అయితే_ముగింపు

  అక్షరం buf[4];
  కొరకు (i = 0; i < ARRAY_నిడివి (ot_languages2); i++)
    ఒకవేళ (ot_languages2[i].ట్యాగ్_2 == ట్యాగ్_2)
    {
      hb_ట్యాగ్_గమ్యం_string (ot_languages2[i].language, buf);
      తిరిగి hb_language_from_string (buf, 2);
    }
#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  కొరకు (i = 0; i < ARRAY_నిడివి (ot_languages3); i++)
    ఒకవేళ (ot_languages3[i].ట్యాగ్_2 == ట్యాగ్_2)
    {
      hb_ట్యాగ్_గమ్యం_string (ot_languages3[i].language, buf);
      తిరిగి hb_language_from_string (buf, 3);
    }
  కొరకు (i = 0; i < ARRAY_నిడివి (ot_languages3_multi); i++)
    కొరకు (గుర్తులేని పూర్ణాంకం j = 0; j < ot_languages3_multi[i].లెక్క; j++)
      ఒకవేళ (ot_languages3_multi_విలువలు[ot_languages3_multi[i].స్థానభ్రంశం + j] == ట్యాగ్_2)
      {
	hb_ట్యాగ్_గమ్యం_string (ot_languages3_multi[i].language, buf);
	తిరిగి hb_language_from_string (buf, 3);
      }
#అయితే_ముగింపు

  /* Return a custom language in the form of "x-hbot-AABBCCDD".
   * If it's three letters long, also guess it's ISO 639-3 and lower-case and
   * prepend it (if it's not a registered tag, the private use subtags will
   * ensure that calling hb_ot_tag_from_language on the result will still return
   * the same tag as the original tag).
   */
  {
    అక్షరం buf[20];
    అక్షరం *str = buf;
    ఒకవేళ (ISALPHA (ట్యాగ్_2 >> 24)
	&& ISALPHA ((ట్యాగ్_2 >> 16) & 0xFF)
	&& ISALPHA ((ట్యాగ్_2 >> 8) & 0xFF)
	&& (ట్యాగ్_2 & 0xFF) == ' ')
    {
      buf[0] = TOLOWER (ట్యాగ్_2 >> 24);
      buf[1] = TOLOWER ((ట్యాగ్_2 >> 16) & 0xFF);
      buf[2] = TOLOWER ((ట్యాగ్_2 >> 8) & 0xFF);
      buf[3] = '-';
      str += 4;
    }
    snprintf (str, 16, "x-hbot-%08" PRIx32, ట్యాగ్_2);
    తిరిగి hb_language_from_string (&*buf, -1);
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
శూన్య
hb_ot_ట్యాగులు_గమ్యం_script_and_language (hb_ట్యాగ్_t       script_ట్యాగ్,
				   hb_ట్యాగ్_t       language_ట్యాగ్,
				   hb_script_t   *script /*బహిర్గతం*/,
				   hb_language_t *language /*బహిర్గతం*/)
{
  hb_script_t script_బహిర్గతం = hb_ot_ట్యాగ్_గమ్యం_script (script_ట్యాగ్);
  ఒకవేళ (script)
    *script = script_బహిర్గతం;
  ఒకవేళ (language)
  {
    గుర్తులేని పూర్ణాంకం script_లెక్క = 1;
    hb_ట్యాగ్_t primary_script_ట్యాగ్[1];
    hb_ot_ట్యాగులు_from_script_and_language (script_బహిర్గతం,
					 HB_LANGUAGE_INVALID,
					 &script_లెక్క,
					 primary_script_ట్యాగ్,
					 శూన్యసూచిక, శూన్యసూచిక);
    *language = hb_ot_ట్యాగ్_గమ్యం_language (language_ట్యాగ్);
    స్థిరరాశి అక్షరం *lang_str = *language ? hb_language_గమ్యం_string (*language) : శూన్యసూచిక;
    ఒకవేళ (lang_str && (script_లెక్క == 0 || primary_script_ట్యాగ్[0] != script_ట్యాగ్))
    {
      గుర్తులేని అక్షరం *buf;
      size_t len = అక్షరశ్రేణి_పొడవు (lang_str);
      buf = (గుర్తులేని అక్షరం *) hb_malloc (len + 16);
      ఒకవేళ (unlikely (!buf))
      {
	*language = శూన్యసూచిక;
      }
      లేకపోతే
      {
	పూర్ణాంకం జరుపు;
	hb_memcpy (buf, lang_str, len);
	ఒకవేళ (lang_str[0] != 'x' || lang_str[1] != '-') {
	  buf[len++] = '-';
	  buf[len++] = 'x';
	}
	buf[len++] = '-';
	buf[len++] = 'h';
	buf[len++] = 'b';
	buf[len++] = 's';
	buf[len++] = 'c';
	buf[len++] = '-';
	కొరకు (జరుపు = 28; జరుపు >= 0; జరుపు -= 4)
	  buf[len++] = TOHEX (script_ట్యాగ్ >> జరుపు);
	*language = hb_language_from_string ((అక్షరం *) buf, len);
	hb_free (buf);
      }
    }
  }
}

#నిర్వచితమైతే MAIN
స్థిర అంతఃవరుస శూన్య
test_langs_sorted ()
{
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < ARRAY_నిడివి (ot_languages2); i++)
  {
    పూర్ణాంకం c = ot_languages2[i].cmp (&ot_languages2[i - 1]);
    ఒకవేళ (c > 0)
    {
      fprintf (stderr, "ot_languages2 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages2[i-1].language, c, ot_languages2[i].language);
      abort();
    }
  }
#నిర్వచితం_కాకపోతే HB_NO_LANGUAGE_LONG
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < ARRAY_నిడివి (ot_languages3); i++)
  {
    పూర్ణాంకం c = ot_languages3[i].cmp (&ot_languages3[i - 1]);
    ఒకవేళ (c > 0)
    {
      fprintf (stderr, "ot_languages3 not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3[i-1].language, c, ot_languages3[i].language);
      abort();
    }
  }
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < ARRAY_నిడివి (ot_languages3_multi); i++)
  {
    పూర్ణాంకం c = ot_languages3_multi[i].cmp (&ot_languages3_multi[i - 1]);
    ఒకవేళ (c > 0)
    {
      fprintf (stderr, "ot_languages3_multi not sorted at index %u: %08x %d %08x\n",
	       i, ot_languages3_multi[i-1].language, c, ot_languages3_multi[i].language);
      abort();
    }
  }
  కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < ARRAY_నిడివి (ot_languages3_blocked); i++)
    ఒకవేళ (ot_languages3_blocked[i] < ot_languages3_blocked[i - 1])
    {
      fprintf (stderr, "ot_languages3_blocked not sorted at index %u: %08x < %08x\n",
	       i, ot_languages3_blocked[i], ot_languages3_blocked[i - 1]);
      abort();
    }
#అయితే_ముగింపు
}

పూర్ణాంకం
ప్రధాన ()
{
  test_langs_sorted ();
  తిరిగి 0;
}

#అయితే_ముగింపు


#అయితే_ముగింపు
