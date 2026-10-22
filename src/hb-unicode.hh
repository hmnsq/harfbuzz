/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Codethink Limited
 * Copyright © 2010,2011,2012  Google, Inc.
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
 * Codethink Author(s): Ryan Lortie
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_UNICODE_HH
#define HB_UNICODE_HH

#شامل "hb.hh"


بیرونی HB_INTERNAL ثابت uint8_t _hb_modified_combining_class[256];

/*
 * hb_unicode_funcs_t
 */

#define HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS \
  HB_UNICODE_FUNC_IMPLEMENT (combining_class) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (eastasian_width)) \
  HB_UNICODE_FUNC_IMPLEMENT (general_category) \
  HB_UNICODE_FUNC_IMPLEMENT (mirroring) \
  HB_UNICODE_FUNC_IMPLEMENT (script) \
  HB_UNICODE_FUNC_IMPLEMENT (compose) \
  HB_UNICODE_FUNC_IMPLEMENT (decompose) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (decompose_compatibility)) \
  /* ^--- Add new callbacks here */

/* Simple callbacks are those taking a hb_codepoint_t and returning a hb_codepoint_t */
#define HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS_SIMPLE \
  HB_UNICODE_FUNC_IMPLEMENT (hb_unicode_combining_class_t, combining_class) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (unsigned صحیح, eastasian_width)) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_unicode_general_category_t, general_category) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_codepoint_t, mirroring) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_script_t, script) \
  /* ^--- Add new simple callbacks here */

ساختار hb_unicode_funcs_t
{
  hb_object_header_t header;

  hb_unicode_funcs_t *parent;

#define HB_UNICODE_FUNC_IMPLEMENT(return_type, name) \
  return_type name (hb_codepoint_t unicode) { بازگشت func.name (این, unicode, user_data.name); }
HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS_SIMPLE
#undef HB_UNICODE_FUNC_IMPLEMENT

  hb_bool_t compose (hb_codepoint_t a, hb_codepoint_t b,
		     hb_codepoint_t *ab)
  {
    *ab = 0;
    اگر (unlikely (!a || !b)) بازگشت نادرست;
    بازگشت func.compose (این, a, b, ab, user_data.compose);
  }

  hb_bool_t decompose (hb_codepoint_t ab,
		       hb_codepoint_t *a, hb_codepoint_t *b)
  {
    *a = ab; *b = 0;
    بازگشت func.decompose (این, ab, a, b, user_data.decompose);
  }

  unsigned صحیح decompose_compatibility (hb_codepoint_t  u,
					hb_codepoint_t *decomposed)
  {
#ifdef HB_DISABLE_DEPRECATED
    unsigned صحیح ret  = 0;
#وگرنه
    unsigned صحیح ret = func.decompose_compatibility (این, u, decomposed, user_data.decompose_compatibility);
#endif
    اگر (ret == 1 && u == decomposed[0]) {
      decomposed[0] = 0;
      بازگشت 0;
    }
    decomposed[ret] = 0;
    بازگشت ret;
  }

  unsigned صحیح
  modified_combining_class (hb_codepoint_t u)
  {
    /* Reorder SAKOT to ensure it comes after any tone marks. */
    اگر (unlikely (u == 0x1A60u)) بازگشت 254;
    /* Reorder PADMA to ensure it comes after any vowel marks. */
    اگر (unlikely (u == 0x0FC6u)) بازگشت 254;
    /* Reorder TSA -PHRU to reorder before U+0F74 */
    اگر (unlikely (u == 0x0F39u)) بازگشت 127;

    بازگشت _hb_modified_combining_class[combining_class (u)];
  }

  ایستا hb_bool_t
  is_variation_selector (hb_codepoint_t unicode)
  {
    /* U+180B..180D, U+180F MONGOLIAN FREE VARIATION SELECTORs are handled in the
     * Arabic shaper.  No need to match them here. */
    بازگشت unlikely (hb_in_ranges<hb_codepoint_t> (unicode,
						   0xFE00u, 0xFE0Fu, /* VARIATION SELECTOR-1..16 */
						   0xE0100u, 0xE01EFu));  /* VARIATION SELECTOR-17..256 */
  }

  /* Default_Ignorable codepoints:
   *
   * Note: While U+115F, U+1160, U+3164 and U+FFA0 are Default_Ignorable,
   * we do NOT want to hide them, as the way Uniscribe has implemented them
   * is with regular spacing glyphs, and that's the way fonts are made to work.
   * As such, we make exceptions for those four.
   * Also ignoring U+1BCA0..1BCA3. https://github.com/harfbuzz/harfbuzz/issues/503
   *
   * Unicode 14.0:
   * $ grep '; Default_Ignorable_Code_Point ' DerivedCoreProperties.txt | sed 's/;.*#/#/'
   * 00AD          # Cf       SOFT HYPHEN
   * 034F          # Mn       COMBINING GRAPHEME JOINER
   * 061C          # Cf       ARABIC LETTER MARK
   * 115F..1160    # Lo   [2] HANGUL CHOSEONG FILLER..HANGUL JUNGSEONG FILLER
   * 17B4..17B5    # Mn   [2] KHMER VOWEL INHERENT AQ..KHMER VOWEL INHERENT AA
   * 180B..180D    # Mn   [3] MONGOLIAN FREE VARIATION SELECTOR ONE..MONGOLIAN FREE VARIATION SELECTOR THREE
   * 180E          # Cf       MONGOLIAN VOWEL SEPARATOR
   * 180F          # Mn       MONGOLIAN FREE VARIATION SELECTOR FOUR
   * 200B..200F    # Cf   [5] ZERO WIDTH SPACE..RIGHT-TO-LEFT MARK
   * 202A..202E    # Cf   [5] LEFT-TO-RIGHT EMBEDDING..RIGHT-TO-LEFT OVERRIDE
   * 2060..2064    # Cf   [5] WORD JOINER..INVISIBLE PLUS
   * 2065          # Cn       <reserved-2065>
   * 2066..206F    # Cf  [10] LEFT-TO-RIGHT ISOLATE..NOMINAL DIGIT SHAPES
   * 3164          # Lo       HANGUL FILLER
   * FE00..FE0F    # Mn  [16] VARIATION SELECTOR-1..VARIATION SELECTOR-16
   * FEFF          # Cf       ZERO WIDTH NO-BREAK SPACE
   * FFA0          # Lo       HALFWIDTH HANGUL FILLER
   * FFF0..FFF8    # Cn   [9] <reserved-FFF0>..<reserved-FFF8>
   * 1BCA0..1BCA3  # Cf   [4] SHORTHAND FORMAT LETTER OVERLAP..SHORTHAND FORMAT UP STEP
   * 1D173..1D17A  # Cf   [8] MUSICAL SYMBOL BEGIN BEAM..MUSICAL SYMBOL END PHRASE
   * E0000         # Cn       <reserved-E0000>
   * E0001         # Cf       LANGUAGE TAG
   * E0002..E001F  # Cn  [30] <reserved-E0002>..<reserved-E001F>
   * E0020..E007F  # Cf  [96] TAG SPACE..CANCEL TAG
   * E0080..E00FF  # Cn [128] <reserved-E0080>..<reserved-E00FF>
   * E0100..E01EF  # Mn [240] VARIATION SELECTOR-17..VARIATION SELECTOR-256
   * E01F0..E0FFF  # Cn [3600] <reserved-E01F0>..<reserved-E0FFF>
   */
  ایستا hb_bool_t
  is_default_ignorable (hb_codepoint_t ch)
  {
    hb_codepoint_t plane = ch >> 16;
    اگر (likely (plane == 0))
    {
      /* BMP */
      hb_codepoint_t page = ch >> 8;
      انتخاب (page) {
	حالت 0x00: بازگشت unlikely (ch == 0x00ADu);
	حالت 0x03: بازگشت unlikely (ch == 0x034Fu);
	حالت 0x06: بازگشت unlikely (ch == 0x061Cu);
	حالت 0x17: بازگشت hb_in_range<hb_codepoint_t> (ch, 0x17B4u, 0x17B5u);
	حالت 0x18: بازگشت hb_in_range<hb_codepoint_t> (ch, 0x180Bu, 0x180Fu);
	حالت 0x20: بازگشت hb_in_ranges<hb_codepoint_t> (ch, 0x200Bu, 0x200Fu,
					    0x202Au, 0x202Eu,
					    0x2060u, 0x206Fu);
	حالت 0xFE: بازگشت hb_in_range<hb_codepoint_t> (ch, 0xFE00u, 0xFE0Fu) || ch == 0xFEFFu;
	حالت 0xFF: بازگشت hb_in_range<hb_codepoint_t> (ch, 0xFFF0u, 0xFFF8u);
	پیشفرض: بازگشت نادرست;
      }
    }
    وگرنه
    {
      /* Other planes */
      انتخاب (plane) {
	حالت 0x01: بازگشت hb_in_range<hb_codepoint_t> (ch, 0x1D173u, 0x1D17Au);
	حالت 0x0E: بازگشت hb_in_range<hb_codepoint_t> (ch, 0xE0000u, 0xE0FFFu);
	پیشفرض: بازگشت نادرست;
      }
    }
  }

  /* Space estimates based on:
   * https://unicode.org/charts/PDF/U2000.pdf
   * https://docs.microsoft.com/en-us/typography/develop/character-design-standards/whitespace
   */
  شمارش space_t {
    NOT_SPACE = 0,
    SPACE_EM   = 1,
    SPACE_EM_2 = 2,
    SPACE_EM_3 = 3,
    SPACE_EM_4 = 4,
    SPACE_EM_5 = 5,
    SPACE_EM_6 = 6,
    SPACE_EM_16 = 16,
    SPACE_4_EM_18,	/* 4/18th of an EM! */
    SPACE,
    SPACE_FIGURE,
    SPACE_PUNCTUATION,
    SPACE_NARROW,
  };
  ایستا space_t
  space_fallback_type (hb_codepoint_t u)
  {
    انتخاب (u)
    {
      /* All GC=Zs chars that can use a fallback. */
      پیشفرض:	    بازگشت NOT_SPACE;	/* U+1680 OGHAM SPACE MARK */
      حالت 0x0020u: بازگشت SPACE;	/* U+0020 SPACE */
      حالت 0x00A0u: بازگشت SPACE;	/* U+00A0 NO-BREAK SPACE */
      حالت 0x2000u: بازگشت SPACE_EM_2;	/* U+2000 EN QUAD */
      حالت 0x2001u: بازگشت SPACE_EM;	/* U+2001 EM QUAD */
      حالت 0x2002u: بازگشت SPACE_EM_2;	/* U+2002 EN SPACE */
      حالت 0x2003u: بازگشت SPACE_EM;	/* U+2003 EM SPACE */
      حالت 0x2004u: بازگشت SPACE_EM_3;	/* U+2004 THREE-PER-EM SPACE */
      حالت 0x2005u: بازگشت SPACE_EM_4;	/* U+2005 FOUR-PER-EM SPACE */
      حالت 0x2006u: بازگشت SPACE_EM_6;	/* U+2006 SIX-PER-EM SPACE */
      حالت 0x2007u: بازگشت SPACE_FIGURE;	/* U+2007 FIGURE SPACE */
      حالت 0x2008u: بازگشت SPACE_PUNCTUATION;	/* U+2008 PUNCTUATION SPACE */
      حالت 0x2009u: بازگشت SPACE_EM_5;		/* U+2009 THIN SPACE */
      حالت 0x200Au: بازگشت SPACE_EM_16;		/* U+200A HAIR SPACE */
      حالت 0x202Fu: بازگشت SPACE_NARROW;	/* U+202F NARROW NO-BREAK SPACE */
      حالت 0x205Fu: بازگشت SPACE_4_EM_18;	/* U+205F MEDIUM MATHEMATICAL SPACE */
      حالت 0x3000u: بازگشت SPACE_EM;		/* U+3000 IDEOGRAPHIC SPACE */
    }
  }

  ایستا hb_codepoint_t
  vertical_char_for (hb_codepoint_t u)
  {
    انتخاب (u >> 8)
    {
      حالت 0x20: انتخاب (u) {
	حالت 0x2013u: بازگشت 0xfe32u; // EN DASH
	حالت 0x2014u: بازگشت 0xfe31u; // EM DASH
	حالت 0x2025u: بازگشت 0xfe30u; // TWO DOT LEADER
	حالت 0x2026u: بازگشت 0xfe19u; // HORIZONTAL ELLIPSIS
      } بشکن;
      حالت 0x30: انتخاب (u) {
	حالت 0x3001u: بازگشت 0xfe11u; // IDEOGRAPHIC COMMA
	حالت 0x3002u: بازگشت 0xfe12u; // IDEOGRAPHIC FULL STOP
	حالت 0x3008u: بازگشت 0xfe3fu; // LEFT ANGLE BRACKET
	حالت 0x3009u: بازگشت 0xfe40u; // RIGHT ANGLE BRACKET
	حالت 0x300au: بازگشت 0xfe3du; // LEFT DOUBLE ANGLE BRACKET
	حالت 0x300bu: بازگشت 0xfe3eu; // RIGHT DOUBLE ANGLE BRACKET
	حالت 0x300cu: بازگشت 0xfe41u; // LEFT CORNER BRACKET
	حالت 0x300du: بازگشت 0xfe42u; // RIGHT CORNER BRACKET
	حالت 0x300eu: بازگشت 0xfe43u; // LEFT WHITE CORNER BRACKET
	حالت 0x300fu: بازگشت 0xfe44u; // RIGHT WHITE CORNER BRACKET
	حالت 0x3010u: بازگشت 0xfe3bu; // LEFT BLACK LENTICULAR BRACKET
	حالت 0x3011u: بازگشت 0xfe3cu; // RIGHT BLACK LENTICULAR BRACKET
	حالت 0x3014u: بازگشت 0xfe39u; // LEFT TORTOISE SHELL BRACKET
	حالت 0x3015u: بازگشت 0xfe3au; // RIGHT TORTOISE SHELL BRACKET
	حالت 0x3016u: بازگشت 0xfe17u; // LEFT WHITE LENTICULAR BRACKET
	حالت 0x3017u: بازگشت 0xfe18u; // RIGHT WHITE LENTICULAR BRACKET
      } بشکن;
      حالت 0xfe: انتخاب (u) {
	حالت 0xfe4fu: بازگشت 0xfe34u; // WAVY LOW LINE
      } بشکن;
      حالت 0xff: انتخاب (u) {
	حالت 0xff01u: بازگشت 0xfe15u; // FULLWIDTH EXCLAMATION MARK
	حالت 0xff08u: بازگشت 0xfe35u; // FULLWIDTH LEFT PARENTHESIS
	حالت 0xff09u: بازگشت 0xfe36u; // FULLWIDTH RIGHT PARENTHESIS
	حالت 0xff0cu: بازگشت 0xfe10u; // FULLWIDTH COMMA
	حالت 0xff1au: بازگشت 0xfe13u; // FULLWIDTH COLON
	حالت 0xff1bu: بازگشت 0xfe14u; // FULLWIDTH SEMICOLON
	حالت 0xff1fu: بازگشت 0xfe16u; // FULLWIDTH QUESTION MARK
	حالت 0xff3bu: بازگشت 0xfe47u; // FULLWIDTH LEFT SQUARE BRACKET
	حالت 0xff3du: بازگشت 0xfe48u; // FULLWIDTH RIGHT SQUARE BRACKET
	حالت 0xff3fu: بازگشت 0xfe33u; // FULLWIDTH LOW LINE
	حالت 0xff5bu: بازگشت 0xfe37u; // FULLWIDTH LEFT CURLY BRACKET
	حالت 0xff5du: بازگشت 0xfe38u; // FULLWIDTH RIGHT CURLY BRACKET
      } بشکن;
    }

    بازگشت u;
  }

  ساختار {
#define HB_UNICODE_FUNC_IMPLEMENT(name) hb_unicode_##name##_func_t name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_UNICODE_FUNC_IMPLEMENT
  } func;

  ساختار {
#define HB_UNICODE_FUNC_IMPLEMENT(name) تهی *name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_UNICODE_FUNC_IMPLEMENT
  } user_data;

  ساختار {
#define HB_UNICODE_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#undef HB_UNICODE_FUNC_IMPLEMENT
  } destroy;
};
DECLARE_NULL_INSTANCE (hb_unicode_funcs_t);


/*
 * Modified combining marks
 */

/* Hebrew
 *
 * We permute the "fixed-position" classes 10-26 into the order
 * described in the SBL Hebrew manual:
 *
 * https://www.sbl-site.org/Fonts/SBLHebrewUserManual1.5x.pdf
 *
 * (as recommended by:
 *  https://forum.fontlab.com/archive-old-microsoft-volt-group/vista-and-diacritic-ordering/msg22823/)
 *
 * More details here:
 * https://bugzilla.mozilla.org/show_bug.cgi?id=662055
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC10 22 /* sheva */
#define HB_MODIFIED_COMBINING_CLASS_CCC11 15 /* hataf segol */
#define HB_MODIFIED_COMBINING_CLASS_CCC12 16 /* hataf patah */
#define HB_MODIFIED_COMBINING_CLASS_CCC13 17 /* hataf qamats */
#define HB_MODIFIED_COMBINING_CLASS_CCC14 23 /* hiriq */
#define HB_MODIFIED_COMBINING_CLASS_CCC15 18 /* tsere */
#define HB_MODIFIED_COMBINING_CLASS_CCC16 19 /* segol */
#define HB_MODIFIED_COMBINING_CLASS_CCC17 20 /* patah */
#define HB_MODIFIED_COMBINING_CLASS_CCC18 21 /* qamats & qamats qatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC19 14 /* holam & holam haser for vav*/
#define HB_MODIFIED_COMBINING_CLASS_CCC20 24 /* qubuts */
#define HB_MODIFIED_COMBINING_CLASS_CCC21 12 /* dagesh */
#define HB_MODIFIED_COMBINING_CLASS_CCC22 25 /* meteg */
#define HB_MODIFIED_COMBINING_CLASS_CCC23 13 /* rafe */
#define HB_MODIFIED_COMBINING_CLASS_CCC24 10 /* shin dot */
#define HB_MODIFIED_COMBINING_CLASS_CCC25 11 /* sin dot */
#define HB_MODIFIED_COMBINING_CLASS_CCC26 26 /* point varika */

/*
 * Arabic
 *
 * Modify to move Shadda (ccc=33) before other marks.  See:
 * https://unicode.org/faq/normalization.html#8
 * https://unicode.org/faq/normalization.html#9
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC27 28 /* fathatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC28 29 /* dammatan */
#define HB_MODIFIED_COMBINING_CLASS_CCC29 30 /* kasratan */
#define HB_MODIFIED_COMBINING_CLASS_CCC30 31 /* fatha */
#define HB_MODIFIED_COMBINING_CLASS_CCC31 32 /* damma */
#define HB_MODIFIED_COMBINING_CLASS_CCC32 33 /* kasra */
#define HB_MODIFIED_COMBINING_CLASS_CCC33 27 /* shadda */
#define HB_MODIFIED_COMBINING_CLASS_CCC34 34 /* sukun */
#define HB_MODIFIED_COMBINING_CLASS_CCC35 35 /* superscript alef */

/* Syriac */
#define HB_MODIFIED_COMBINING_CLASS_CCC36 36 /* superscript alaph */

/* Telugu
 *
 * Modify Telugu length marks (ccc=84, ccc=91).
 * These are the only matras in the main Indic scripts range that have
 * a non-zero ccc.  That makes them reorder with the Halant (ccc=9).
 * Assign 4 and 5, which are otherwise unassigned.
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC84 4 /* length mark */
#define HB_MODIFIED_COMBINING_CLASS_CCC91 5 /* ai length mark */

/* Thai
 *
 * Modify U+0E38 and U+0E39 (ccc=103) to be reordered before U+0E3A (ccc=9).
 * Assign 3, which is unassigned otherwise.
 * Uniscribe does this reordering too.
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC103 3 /* sara u / sara uu */
#define HB_MODIFIED_COMBINING_CLASS_CCC107 107 /* mai * */

/* Lao */
#define HB_MODIFIED_COMBINING_CLASS_CCC118 118 /* sign u / sign uu */
#define HB_MODIFIED_COMBINING_CLASS_CCC122 122 /* mai * */

/* Tibetan
 *
 * In case of multiple vowel-signs, use u first (but after achung)
 * this allows Dzongkha multi-vowel shortcuts to render correctly
 */
#define HB_MODIFIED_COMBINING_CLASS_CCC129 129 /* sign aa */
#define HB_MODIFIED_COMBINING_CLASS_CCC130 132 /* sign i */
#define HB_MODIFIED_COMBINING_CLASS_CCC132 131 /* sign u */

/* Misc */

#define HB_UNICODE_GENERAL_CATEGORY_IS_MARK(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)))

#define HB_UNICODE_GENERAL_CATEGORY_IS_LETTER(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER)))

/*
 * Ranges, used for bsearch tables.
 */

ساختار hb_unicode_range_t
{
  ایستا صحیح
  cmp (ثابت تهی *کلید_3, ثابت تهی *_item)
  {
    hb_codepoint_t cp = *((hb_codepoint_t *) کلید_3);
    ثابت hb_unicode_range_t *range = (hb_unicode_range_t *) _item;

    اگر (cp < range->start)
      بازگشت -1;
    وگرنه اگر (cp <= range->end)
      بازگشت 0;
    وگرنه
      بازگشت +1;
  }

  hb_codepoint_t start;
  hb_codepoint_t end;
};

/*
 * Emoji.
 */

HB_INTERNAL منطقی
_hb_unicode_is_emoji_Extended_Pictographic (hb_codepoint_t cp);


بیرونی "C" HB_INTERNAL hb_unicode_funcs_t *hb_ucd_get_unicode_funcs ();


#endif /* HB_UNICODE_HH */
