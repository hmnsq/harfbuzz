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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_UNICODE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ ਅਚਲ uint8_t _hb_modified_combining_class[256];

/*
 * hb_unicode_funcs_t
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS \
  HB_UNICODE_FUNC_IMPLEMENT (combining_class) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (eastasian_ਚੌੜਾਈ)) \
  HB_UNICODE_FUNC_IMPLEMENT (general_category) \
  HB_UNICODE_FUNC_IMPLEMENT (mirroring) \
  HB_UNICODE_FUNC_IMPLEMENT (script) \
  HB_UNICODE_FUNC_IMPLEMENT (compose) \
  HB_UNICODE_FUNC_IMPLEMENT (decompose) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (decompose_compatibility)) \
  /* ^--- Add new callbacks here */

/* Simple callbacks are those taking a hb_codepoint_t and returning a hb_codepoint_t */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS_SIMPLE \
  HB_UNICODE_FUNC_IMPLEMENT (hb_unicode_combining_class_t, combining_class) \
  HB_IF_NOT_DEPRECATED (HB_UNICODE_FUNC_IMPLEMENT (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, eastasian_ਚੌੜਾਈ)) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_unicode_general_category_t, general_category) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_codepoint_t, mirroring) \
  HB_UNICODE_FUNC_IMPLEMENT (hb_script_t, script) \
  /* ^--- Add new simple callbacks here */

ਬਣਤਰ hb_unicode_funcs_t
{
  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  hb_unicode_funcs_t *ਮਾਪਾ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNC_IMPLEMENT(return_type, name) \
  return_type name (hb_codepoint_t unicode) { ਵਾਪਸ func.name (ਇਹ, unicode, user_ਅੰਕੜੇ.name); }
HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS_SIMPLE
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_UNICODE_FUNC_IMPLEMENT

  hb_bool_t compose (hb_codepoint_t a, hb_codepoint_t b,
		     hb_codepoint_t *ab)
  {
    *ab = 0;
    ਜੇ (unlikely (!a || !b)) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ func.compose (ਇਹ, a, b, ab, user_ਅੰਕੜੇ.compose);
  }

  hb_bool_t decompose (hb_codepoint_t ab,
		       hb_codepoint_t *a, hb_codepoint_t *b)
  {
    *a = ab; *b = 0;
    ਵਾਪਸ func.decompose (ਇਹ, ab, a, b, user_ਅੰਕੜੇ.decompose);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ decompose_compatibility (hb_codepoint_t  u,
					hb_codepoint_t *decomposed)
  {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_DISABLE_DEPRECATED
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret  = 0;
#ਨਹੀਂਤਾਂ
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ret = func.decompose_compatibility (ਇਹ, u, decomposed, user_ਅੰਕੜੇ.decompose_compatibility);
#ਜੇ_ਅੰਤ
    ਜੇ (ret == 1 && u == decomposed[0]) {
      decomposed[0] = 0;
      ਵਾਪਸ 0;
    }
    decomposed[ret] = 0;
    ਵਾਪਸ ret;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
  modified_combining_class (hb_codepoint_t u)
  {
    /* Reorder SAKOT to ensure it comes after any tone marks. */
    ਜੇ (unlikely (u == 0x1A60u)) ਵਾਪਸ 254;
    /* Reorder PADMA to ensure it comes after any vowel marks. */
    ਜੇ (unlikely (u == 0x0FC6u)) ਵਾਪਸ 254;
    /* Reorder TSA -PHRU to reorder before U+0F74 */
    ਜੇ (unlikely (u == 0x0F39u)) ਵਾਪਸ 127;

    ਵਾਪਸ _hb_modified_combining_class[combining_class (u)];
  }

  ਸਥਿਰ hb_bool_t
  is_variation_selector (hb_codepoint_t unicode)
  {
    /* U+180B..180D, U+180F MONGOLIAN FREE VARIATION SELECTORs are handled in the
     * Arabic shaper.  No need to match them here. */
    ਵਾਪਸ unlikely (hb_ਵਿੱਚ_ranges<hb_codepoint_t> (unicode,
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
  ਸਥਿਰ hb_bool_t
  is_default_ignorable (hb_codepoint_t ch)
  {
    hb_codepoint_t plane = ch >> 16;
    ਜੇ (likely (plane == 0))
    {
      /* BMP */
      hb_codepoint_t ਪੰਨਾ = ch >> 8;
      ਚੋਣ (ਪੰਨਾ) {
	ਹਾਲਤ 0x00: ਵਾਪਸ unlikely (ch == 0x00ADu);
	ਹਾਲਤ 0x03: ਵਾਪਸ unlikely (ch == 0x034Fu);
	ਹਾਲਤ 0x06: ਵਾਪਸ unlikely (ch == 0x061Cu);
	ਹਾਲਤ 0x17: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0x17B4u, 0x17B5u);
	ਹਾਲਤ 0x18: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0x180Bu, 0x180Fu);
	ਹਾਲਤ 0x20: ਵਾਪਸ hb_ਵਿੱਚ_ranges<hb_codepoint_t> (ch, 0x200Bu, 0x200Fu,
					    0x202Au, 0x202Eu,
					    0x2060u, 0x206Fu);
	ਹਾਲਤ 0xFE: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0xFE00u, 0xFE0Fu) || ch == 0xFEFFu;
	ਹਾਲਤ 0xFF: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0xFFF0u, 0xFFF8u);
	ਮੂਲ: ਵਾਪਸ ਝੂਠ;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      /* Other planes */
      ਚੋਣ (plane) {
	ਹਾਲਤ 0x01: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0x1D173u, 0x1D17Au);
	ਹਾਲਤ 0x0E: ਵਾਪਸ hb_ਵਿੱਚ_range<hb_codepoint_t> (ch, 0xE0000u, 0xE0FFFu);
	ਮੂਲ: ਵਾਪਸ ਝੂਠ;
      }
    }
  }

  /* Space estimates based on:
   * https://unicode.org/charts/PDF/U2000.pdf
   * https://docs.microsoft.com/en-us/typography/develop/character-design-standards/whitespace
   */
  ਗਿਣਤੀ space_t {
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
  ਸਥਿਰ space_t
  space_fallback_type (hb_codepoint_t u)
  {
    ਚੋਣ (u)
    {
      /* All GC=Zs chars that can use a fallback. */
      ਮੂਲ:	    ਵਾਪਸ NOT_SPACE;	/* U+1680 OGHAM SPACE MARK */
      ਹਾਲਤ 0x0020u: ਵਾਪਸ SPACE;	/* U+0020 SPACE */
      ਹਾਲਤ 0x00A0u: ਵਾਪਸ SPACE;	/* U+00A0 NO-BREAK SPACE */
      ਹਾਲਤ 0x2000u: ਵਾਪਸ SPACE_EM_2;	/* U+2000 EN QUAD */
      ਹਾਲਤ 0x2001u: ਵਾਪਸ SPACE_EM;	/* U+2001 EM QUAD */
      ਹਾਲਤ 0x2002u: ਵਾਪਸ SPACE_EM_2;	/* U+2002 EN SPACE */
      ਹਾਲਤ 0x2003u: ਵਾਪਸ SPACE_EM;	/* U+2003 EM SPACE */
      ਹਾਲਤ 0x2004u: ਵਾਪਸ SPACE_EM_3;	/* U+2004 THREE-PER-EM SPACE */
      ਹਾਲਤ 0x2005u: ਵਾਪਸ SPACE_EM_4;	/* U+2005 FOUR-PER-EM SPACE */
      ਹਾਲਤ 0x2006u: ਵਾਪਸ SPACE_EM_6;	/* U+2006 SIX-PER-EM SPACE */
      ਹਾਲਤ 0x2007u: ਵਾਪਸ SPACE_FIGURE;	/* U+2007 FIGURE SPACE */
      ਹਾਲਤ 0x2008u: ਵਾਪਸ SPACE_PUNCTUATION;	/* U+2008 PUNCTUATION SPACE */
      ਹਾਲਤ 0x2009u: ਵਾਪਸ SPACE_EM_5;		/* U+2009 THIN SPACE */
      ਹਾਲਤ 0x200Au: ਵਾਪਸ SPACE_EM_16;		/* U+200A HAIR SPACE */
      ਹਾਲਤ 0x202Fu: ਵਾਪਸ SPACE_NARROW;	/* U+202F NARROW NO-BREAK SPACE */
      ਹਾਲਤ 0x205Fu: ਵਾਪਸ SPACE_4_EM_18;	/* U+205F MEDIUM MATHEMATICAL SPACE */
      ਹਾਲਤ 0x3000u: ਵਾਪਸ SPACE_EM;		/* U+3000 IDEOGRAPHIC SPACE */
    }
  }

  ਸਥਿਰ hb_codepoint_t
  vertical_char_for (hb_codepoint_t u)
  {
    ਚੋਣ (u >> 8)
    {
      ਹਾਲਤ 0x20: ਚੋਣ (u) {
	ਹਾਲਤ 0x2013u: ਵਾਪਸ 0xfe32u; // EN DASH
	ਹਾਲਤ 0x2014u: ਵਾਪਸ 0xfe31u; // EM DASH
	ਹਾਲਤ 0x2025u: ਵਾਪਸ 0xfe30u; // TWO DOT LEADER
	ਹਾਲਤ 0x2026u: ਵਾਪਸ 0xfe19u; // HORIZONTAL ELLIPSIS
      } ਰੋਕੋ;
      ਹਾਲਤ 0x30: ਚੋਣ (u) {
	ਹਾਲਤ 0x3001u: ਵਾਪਸ 0xfe11u; // IDEOGRAPHIC COMMA
	ਹਾਲਤ 0x3002u: ਵਾਪਸ 0xfe12u; // IDEOGRAPHIC FULL STOP
	ਹਾਲਤ 0x3008u: ਵਾਪਸ 0xfe3fu; // LEFT ANGLE BRACKET
	ਹਾਲਤ 0x3009u: ਵਾਪਸ 0xfe40u; // RIGHT ANGLE BRACKET
	ਹਾਲਤ 0x300au: ਵਾਪਸ 0xfe3du; // LEFT DOUBLE ANGLE BRACKET
	ਹਾਲਤ 0x300bu: ਵਾਪਸ 0xfe3eu; // RIGHT DOUBLE ANGLE BRACKET
	ਹਾਲਤ 0x300cu: ਵਾਪਸ 0xfe41u; // LEFT CORNER BRACKET
	ਹਾਲਤ 0x300du: ਵਾਪਸ 0xfe42u; // RIGHT CORNER BRACKET
	ਹਾਲਤ 0x300eu: ਵਾਪਸ 0xfe43u; // LEFT WHITE CORNER BRACKET
	ਹਾਲਤ 0x300fu: ਵਾਪਸ 0xfe44u; // RIGHT WHITE CORNER BRACKET
	ਹਾਲਤ 0x3010u: ਵਾਪਸ 0xfe3bu; // LEFT BLACK LENTICULAR BRACKET
	ਹਾਲਤ 0x3011u: ਵਾਪਸ 0xfe3cu; // RIGHT BLACK LENTICULAR BRACKET
	ਹਾਲਤ 0x3014u: ਵਾਪਸ 0xfe39u; // LEFT TORTOISE SHELL BRACKET
	ਹਾਲਤ 0x3015u: ਵਾਪਸ 0xfe3au; // RIGHT TORTOISE SHELL BRACKET
	ਹਾਲਤ 0x3016u: ਵਾਪਸ 0xfe17u; // LEFT WHITE LENTICULAR BRACKET
	ਹਾਲਤ 0x3017u: ਵਾਪਸ 0xfe18u; // RIGHT WHITE LENTICULAR BRACKET
      } ਰੋਕੋ;
      ਹਾਲਤ 0xfe: ਚੋਣ (u) {
	ਹਾਲਤ 0xfe4fu: ਵਾਪਸ 0xfe34u; // WAVY LOW LINE
      } ਰੋਕੋ;
      ਹਾਲਤ 0xff: ਚੋਣ (u) {
	ਹਾਲਤ 0xff01u: ਵਾਪਸ 0xfe15u; // FULLWIDTH EXCLAMATION MARK
	ਹਾਲਤ 0xff08u: ਵਾਪਸ 0xfe35u; // FULLWIDTH LEFT PARENTHESIS
	ਹਾਲਤ 0xff09u: ਵਾਪਸ 0xfe36u; // FULLWIDTH RIGHT PARENTHESIS
	ਹਾਲਤ 0xff0cu: ਵਾਪਸ 0xfe10u; // FULLWIDTH COMMA
	ਹਾਲਤ 0xff1au: ਵਾਪਸ 0xfe13u; // FULLWIDTH COLON
	ਹਾਲਤ 0xff1bu: ਵਾਪਸ 0xfe14u; // FULLWIDTH SEMICOLON
	ਹਾਲਤ 0xff1fu: ਵਾਪਸ 0xfe16u; // FULLWIDTH QUESTION MARK
	ਹਾਲਤ 0xff3bu: ਵਾਪਸ 0xfe47u; // FULLWIDTH LEFT SQUARE BRACKET
	ਹਾਲਤ 0xff3du: ਵਾਪਸ 0xfe48u; // FULLWIDTH RIGHT SQUARE BRACKET
	ਹਾਲਤ 0xff3fu: ਵਾਪਸ 0xfe33u; // FULLWIDTH LOW LINE
	ਹਾਲਤ 0xff5bu: ਵਾਪਸ 0xfe37u; // FULLWIDTH LEFT CURLY BRACKET
	ਹਾਲਤ 0xff5du: ਵਾਪਸ 0xfe38u; // FULLWIDTH RIGHT CURLY BRACKET
      } ਰੋਕੋ;
    }

    ਵਾਪਸ u;
  }

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNC_IMPLEMENT(name) hb_unicode_##name##_func_t name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_UNICODE_FUNC_IMPLEMENT
  } func;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNC_IMPLEMENT(name) ਖਾਲੀ *name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_UNICODE_FUNC_IMPLEMENT
  } user_ਅੰਕੜੇ;

  ਬਣਤਰ {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_FUNC_IMPLEMENT(name) hb_destroy_func_t name;
    HB_UNICODE_FUNCS_IMPLEMENT_CALLBACKS
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_UNICODE_FUNC_IMPLEMENT
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC10 22 /* sheva */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC11 15 /* hataf segol */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC12 16 /* hataf patah */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC13 17 /* hataf qamats */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC14 23 /* hiriq */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC15 18 /* tsere */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC16 19 /* segol */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC17 20 /* patah */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC18 21 /* qamats & qamats qatan */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC19 14 /* holam & holam haser for vav*/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC20 24 /* qubuts */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC21 12 /* dagesh */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC22 25 /* meteg */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC23 13 /* rafe */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC24 10 /* shin dot */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC25 11 /* sin dot */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC26 26 /* point varika */

/*
 * Arabic
 *
 * Modify to move Shadda (ccc=33) before other marks.  See:
 * https://unicode.org/faq/normalization.html#8
 * https://unicode.org/faq/normalization.html#9
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC27 28 /* fathatan */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC28 29 /* dammatan */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC29 30 /* kasratan */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC30 31 /* fatha */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC31 32 /* damma */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC32 33 /* kasra */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC33 27 /* shadda */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC34 34 /* sukun */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC35 35 /* superscript alef */

/* Syriac */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC36 36 /* superscript alaph */

/* Telugu
 *
 * Modify Telugu length marks (ccc=84, ccc=91).
 * These are the only matras in the main Indic scripts range that have
 * a non-zero ccc.  That makes them reorder with the Halant (ccc=9).
 * Assign 4 and 5, which are otherwise unassigned.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC84 4 /* length mark */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC91 5 /* ai length mark */

/* Thai
 *
 * Modify U+0E38 and U+0E39 (ccc=103) to be reordered before U+0E3A (ccc=9).
 * Assign 3, which is unassigned otherwise.
 * Uniscribe does this reordering too.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC103 3 /* sara u / sara uu */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC107 107 /* mai * */

/* Lao */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC118 118 /* sign u / sign uu */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC122 122 /* mai * */

/* Tibetan
 *
 * In case of multiple vowel-signs, use u first (but after achung)
 * this allows Dzongkha multi-vowel shortcuts to render correctly
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC129 129 /* sign aa */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC130 132 /* sign i */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_MODIFIED_COMBINING_CLASS_CCC132 131 /* sign u */

/* Misc */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_GENERAL_CATEGORY_IS_MARK(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_SPACING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_ENCLOSING_MARK) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_UNICODE_GENERAL_CATEGORY_IS_LETTER(gen_cat) \
	(FLAG_UNSAFE (gen_cat) & \
	 (FLAG (HB_UNICODE_GENERAL_CATEGORY_LOWERCASE_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_MODIFIER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_TITLECASE_LETTER) | \
	  FLAG (HB_UNICODE_GENERAL_CATEGORY_UPPERCASE_LETTER)))

/*
 * Ranges, used for bsearch tables.
 */

ਬਣਤਰ hb_unicode_range_t
{
  ਸਥਿਰ ਪੂਰਨ
  cmp (ਅਚਲ ਖਾਲੀ *ਕੁੰਜੀ_3, ਅਚਲ ਖਾਲੀ *ਵਸਤੂ_2)
  {
    hb_codepoint_t cp = *((hb_codepoint_t *) ਕੁੰਜੀ_3);
    ਅਚਲ hb_unicode_range_t *range = (hb_unicode_range_t *) ਵਸਤੂ_2;

    ਜੇ (cp < range->ਸ਼ੁਰੂ)
      ਵਾਪਸ -1;
    ਨਹੀਂਤਾਂ ਜੇ (cp <= range->ਸਿਰਾ)
      ਵਾਪਸ 0;
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ +1;
  }

  hb_codepoint_t ਸ਼ੁਰੂ;
  hb_codepoint_t ਸਿਰਾ;
};

/*
 * Emoji.
 */

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ
_hb_unicode_is_emoji_Extended_Pictographic (hb_codepoint_t cp);


ਬਾਹਰੀ "C" HB_ਅੰਦਰੂਨੀ hb_unicode_funcs_t *hb_ucd_get_unicode_funcs ();


#ਜੇ_ਅੰਤ /* HB_UNICODE_HH */
