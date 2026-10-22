/*
 * Copyright © 2017  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 * Google Author(s): Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-ankr-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-bsln-table.hh" // Just so we compile it; unused otherwise.
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-feat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-just-table.hh" // Just so we compile it; unused otherwise.
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-kerx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-morx-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-layout-trak-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-aat-ltag-table.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gsub-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-gdef-table.hh"


/*
 * hb_aat_apply_context_t
 */

/* Note: This context is used for kerning, even without AAT, hence the condition. */
#ਜੇ !defined(HB_NO_AAT) || !defined(HB_NO_OT_KERN)

AAT::hb_aat_apply_context_t::hb_aat_apply_context_t (ਅਚਲ hb_ot_shape_plan_t *plan_,
						     hb_font_t *font_,
						     hb_ਬਫਰ_t *ਬਫਰ_2,
						     hb_blob_t *blob) :
						       plan (plan_),
						       font (font_),
						       face (font->face),
						       ਬਫਰ (ਬਫਰ_2),
						       sanitizer (),
						       ankr_ਸਾਰਣੀ (&Null (AAT::ankr)),
						       gdef (
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_LAYOUT
							 *face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ
#ਨਹੀਂਤਾਂ
							 Null (GDEF)
#ਜੇ_ਅੰਤ
						       ),
						       has_glyph_classes (gdef.has_glyph_classes ()),
						       lookup_ਸੂਚਕਾਂਕ (0)
{
  sanitizer.init (blob);
  sanitizer.ਸੈੱਟ_ਅੰਕ_glyphs (face->get_ਅੰਕ_glyphs ());
  sanitizer.ਸ਼ੁਰੂ_processing ();
  sanitizer.ਸੈੱਟ_max_ops (HB_SANITIZE_MAX_OPS_MAX);
}

AAT::hb_aat_apply_context_t::~hb_aat_apply_context_t ()
{ sanitizer.ਸਿਰਾ_processing (); }

ਖਾਲੀ
AAT::hb_aat_apply_context_t::ਸੈੱਟ_ankr_ਸਾਰਣੀ (ਅਚਲ AAT::ankr *ankr_ਸਾਰਣੀ_2)
{ ankr_ਸਾਰਣੀ = ankr_ਸਾਰਣੀ_2; }

#ਜੇ_ਅੰਤ


/**
 * SECTION:hb-aat-layout
 * @title: hb-aat-layout
 * @short_description: Apple Advanced Typography Layout
 * @include: hb-aat.h
 *
 * Functions for querying AAT Layout features in the font face.
 *
 * HarfBuzz supports all of the AAT tables used to implement shaping. Other
 * AAT tables and their associated features are not supported.
 **/


#ਜੇ !defined(HB_NO_AAT) || defined(HAVE_CORETEXT)

/* Mapping from OpenType feature tags to AAT feature names and selectors.
 *
 * Table data courtesy of Apple.  Converted from mnemonics to integers
 * when moving to this file. */
ਸਥਿਰ ਅਚਲ hb_aat_feature_mapping_t feature_mappings[] =
{
  {HB_ਟੈਗ ('a','f','r','c'), HB_AAT_LAYOUT_FEATURE_TYPE_FRACTIONS,               HB_AAT_LAYOUT_FEATURE_SELECTOR_VERTICAL_FRACTIONS,             HB_AAT_LAYOUT_FEATURE_SELECTOR_NO_FRACTIONS},
  {HB_ਟੈਗ ('c','2','p','c'), HB_AAT_LAYOUT_FEATURE_TYPE_ਉੱਤਲਾ_CASE,              HB_AAT_LAYOUT_FEATURE_SELECTOR_ਉੱਤਲਾ_CASE_PETITE_CAPS,         HB_AAT_LAYOUT_FEATURE_SELECTOR_DEFAULT_ਉੱਤਲਾ_CASE},
  {HB_ਟੈਗ ('c','2','s','c'), HB_AAT_LAYOUT_FEATURE_TYPE_ਉੱਤਲਾ_CASE,              HB_AAT_LAYOUT_FEATURE_SELECTOR_ਉੱਤਲਾ_CASE_SMALL_CAPS,          HB_AAT_LAYOUT_FEATURE_SELECTOR_DEFAULT_ਉੱਤਲਾ_CASE},
  {HB_ਟੈਗ ('c','a','l','t'), HB_AAT_LAYOUT_FEATURE_TYPE_CONTEXTUAL_ALTERNATIVES, HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_ALTERNATES_ON,       HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_ALTERNATES_OFF},
  {HB_ਟੈਗ ('c','a','s','e'), HB_AAT_LAYOUT_FEATURE_TYPE_CASE_SENSITIVE_LAYOUT,   HB_AAT_LAYOUT_FEATURE_SELECTOR_CASE_SENSITIVE_LAYOUT_ON,       HB_AAT_LAYOUT_FEATURE_SELECTOR_CASE_SENSITIVE_LAYOUT_OFF},
  {HB_ਟੈਗ ('c','l','i','g'), HB_AAT_LAYOUT_FEATURE_TYPE_LIGATURES,               HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_LIGATURES_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_LIGATURES_OFF},
  {HB_ਟੈਗ ('c','p','s','p'), HB_AAT_LAYOUT_FEATURE_TYPE_CASE_SENSITIVE_LAYOUT,   HB_AAT_LAYOUT_FEATURE_SELECTOR_CASE_SENSITIVE_SPACING_ON,      HB_AAT_LAYOUT_FEATURE_SELECTOR_CASE_SENSITIVE_SPACING_OFF},
  {HB_ਟੈਗ ('c','s','w','h'), HB_AAT_LAYOUT_FEATURE_TYPE_CONTEXTUAL_ALTERNATIVES, HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_SWASH_ALTERNATES_ON, HB_AAT_LAYOUT_FEATURE_SELECTOR_CONTEXTUAL_SWASH_ALTERNATES_OFF},
  {HB_ਟੈਗ ('d','l','i','g'), HB_AAT_LAYOUT_FEATURE_TYPE_LIGATURES,               HB_AAT_LAYOUT_FEATURE_SELECTOR_RARE_LIGATURES_ON,              HB_AAT_LAYOUT_FEATURE_SELECTOR_RARE_LIGATURES_OFF},
  {HB_ਟੈਗ ('e','x','p','t'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_EXPERT_CHARACTERS,              (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('f','r','a','c'), HB_AAT_LAYOUT_FEATURE_TYPE_FRACTIONS,               HB_AAT_LAYOUT_FEATURE_SELECTOR_DIAGONAL_FRACTIONS,             HB_AAT_LAYOUT_FEATURE_SELECTOR_NO_FRACTIONS},
  {HB_ਟੈਗ ('f','w','i','d'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_MONOSPACED_ਪਾਠ,                (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('h','a','l','t'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ALT_HALF_ਚੌੜਾਈ_ਪਾਠ,            (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('h','i','s','t'), (hb_aat_layout_feature_type_t) 40,                  (hb_aat_layout_feature_selector_t) 0,                          (hb_aat_layout_feature_selector_t) 1},
  {HB_ਟੈਗ ('h','k','n','a'), HB_AAT_LAYOUT_FEATURE_TYPE_ALTERNATE_KANA,          HB_AAT_LAYOUT_FEATURE_SELECTOR_ALTERNATE_HORIZ_KANA_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_ALTERNATE_HORIZ_KANA_OFF},
  {HB_ਟੈਗ ('h','l','i','g'), HB_AAT_LAYOUT_FEATURE_TYPE_LIGATURES,               HB_AAT_LAYOUT_FEATURE_SELECTOR_HISTORICAL_LIGATURES_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_HISTORICAL_LIGATURES_OFF},
  {HB_ਟੈਗ ('h','n','g','l'), HB_AAT_LAYOUT_FEATURE_TYPE_TRANSLITERATION,         HB_AAT_LAYOUT_FEATURE_SELECTOR_HANJA_ਮੰਜ਼ਿਲ_HANGUL,                HB_AAT_LAYOUT_FEATURE_SELECTOR_NO_TRANSLITERATION},
  {HB_ਟੈਗ ('h','o','j','o'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_HOJO_CHARACTERS,                (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('h','w','i','d'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_HALF_ਚੌੜਾਈ_ਪਾਠ,                (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('i','t','a','l'), HB_AAT_LAYOUT_FEATURE_TYPE_ITALIC_CJK_ROMAN,        HB_AAT_LAYOUT_FEATURE_SELECTOR_CJK_ITALIC_ROMAN_ON,            HB_AAT_LAYOUT_FEATURE_SELECTOR_CJK_ITALIC_ROMAN_OFF},
  {HB_ਟੈਗ ('j','p','0','4'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_JIS2004_CHARACTERS,             (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('j','p','7','8'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_JIS1978_CHARACTERS,             (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('j','p','8','3'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_JIS1983_CHARACTERS,             (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('j','p','9','0'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_JIS1990_CHARACTERS,             (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('l','i','g','a'), HB_AAT_LAYOUT_FEATURE_TYPE_LIGATURES,               HB_AAT_LAYOUT_FEATURE_SELECTOR_COMMON_LIGATURES_ON,            HB_AAT_LAYOUT_FEATURE_SELECTOR_COMMON_LIGATURES_OFF},
  {HB_ਟੈਗ ('l','n','u','m'), HB_AAT_LAYOUT_FEATURE_TYPE_ਸੰਖਿਆ_CASE,             HB_AAT_LAYOUT_FEATURE_SELECTOR_ਉੱਤਲਾ_CASE_NUMBERS,             (hb_aat_layout_feature_selector_t) 2},
  {HB_ਟੈਗ ('m','g','r','k'), HB_AAT_LAYOUT_FEATURE_TYPE_MATHEMATICAL_EXTRAS,     HB_AAT_LAYOUT_FEATURE_SELECTOR_MATHEMATICAL_GREEK_ON,          HB_AAT_LAYOUT_FEATURE_SELECTOR_MATHEMATICAL_GREEK_OFF},
  {HB_ਟੈਗ ('n','l','c','k'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_NLCCHARACTERS,                  (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('o','n','u','m'), HB_AAT_LAYOUT_FEATURE_TYPE_ਸੰਖਿਆ_CASE,             HB_AAT_LAYOUT_FEATURE_SELECTOR_ਹੇਠਲਾ_CASE_NUMBERS,             (hb_aat_layout_feature_selector_t) 2},
  {HB_ਟੈਗ ('o','r','d','n'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_POSITION,       HB_AAT_LAYOUT_FEATURE_SELECTOR_ORDINALS,                       HB_AAT_LAYOUT_FEATURE_SELECTOR_ਸਾਧਾਰਨ_POSITION},
  {HB_ਟੈਗ ('p','a','l','t'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ALT_PROPORTIONAL_ਪਾਠ,          (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('p','c','a','p'), HB_AAT_LAYOUT_FEATURE_TYPE_ਹੇਠਲਾ_CASE,              HB_AAT_LAYOUT_FEATURE_SELECTOR_ਹੇਠਲਾ_CASE_PETITE_CAPS,         HB_AAT_LAYOUT_FEATURE_SELECTOR_DEFAULT_ਹੇਠਲਾ_CASE},
  {HB_ਟੈਗ ('p','k','n','a'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_PROPORTIONAL_ਪਾਠ,              (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('p','n','u','m'), HB_AAT_LAYOUT_FEATURE_TYPE_ਸੰਖਿਆ_SPACING,          HB_AAT_LAYOUT_FEATURE_SELECTOR_PROPORTIONAL_NUMBERS,           (hb_aat_layout_feature_selector_t) 4},
  {HB_ਟੈਗ ('p','w','i','d'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_PROPORTIONAL_ਪਾਠ,              (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('q','w','i','d'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_QUARTER_ਚੌੜਾਈ_ਪਾਠ,             (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('r','l','i','g'), HB_AAT_LAYOUT_FEATURE_TYPE_LIGATURES,               HB_AAT_LAYOUT_FEATURE_SELECTOR_REQUIRED_LIGATURES_ON,          HB_AAT_LAYOUT_FEATURE_SELECTOR_REQUIRED_LIGATURES_OFF},
  {HB_ਟੈਗ ('r','u','b','y'), HB_AAT_LAYOUT_FEATURE_TYPE_RUBY_KANA,               HB_AAT_LAYOUT_FEATURE_SELECTOR_RUBY_KANA_ON,                   HB_AAT_LAYOUT_FEATURE_SELECTOR_RUBY_KANA_OFF},
  {HB_ਟੈਗ ('s','i','n','f'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_POSITION,       HB_AAT_LAYOUT_FEATURE_SELECTOR_SCIENTIFIC_INFERIORS,           HB_AAT_LAYOUT_FEATURE_SELECTOR_ਸਾਧਾਰਨ_POSITION},
  {HB_ਟੈਗ ('s','m','c','p'), HB_AAT_LAYOUT_FEATURE_TYPE_ਹੇਠਲਾ_CASE,              HB_AAT_LAYOUT_FEATURE_SELECTOR_ਹੇਠਲਾ_CASE_SMALL_CAPS,          HB_AAT_LAYOUT_FEATURE_SELECTOR_DEFAULT_ਹੇਠਲਾ_CASE},
  {HB_ਟੈਗ ('s','m','p','l'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_SIMPLIFIED_CHARACTERS,          (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('s','s','0','1'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_ONE_ON,           HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_ONE_OFF},
  {HB_ਟੈਗ ('s','s','0','2'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWO_ON,           HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWO_OFF},
  {HB_ਟੈਗ ('s','s','0','3'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_THREE_ON,         HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_THREE_OFF},
  {HB_ਟੈਗ ('s','s','0','4'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FOUR_ON,          HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FOUR_OFF},
  {HB_ਟੈਗ ('s','s','0','5'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FIVE_ON,          HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FIVE_OFF},
  {HB_ਟੈਗ ('s','s','0','6'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SIX_ON,           HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SIX_OFF},
  {HB_ਟੈਗ ('s','s','0','7'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SEVEN_ON,         HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SEVEN_OFF},
  {HB_ਟੈਗ ('s','s','0','8'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_EIGHT_ON,         HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_EIGHT_OFF},
  {HB_ਟੈਗ ('s','s','0','9'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_NINE_ON,          HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_NINE_OFF},
  {HB_ਟੈਗ ('s','s','1','0'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TEN_ON,           HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TEN_OFF},
  {HB_ਟੈਗ ('s','s','1','1'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_ELEVEN_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_ELEVEN_OFF},
  {HB_ਟੈਗ ('s','s','1','2'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWELVE_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWELVE_OFF},
  {HB_ਟੈਗ ('s','s','1','3'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_THIRTEEN_ON,      HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_THIRTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','4'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FOURTEEN_ON,      HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FOURTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','5'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FIFTEEN_ON,       HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_FIFTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','6'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SIXTEEN_ON,       HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SIXTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','7'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SEVENTEEN_ON,     HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_SEVENTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','8'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_EIGHTEEN_ON,      HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_EIGHTEEN_OFF},
  {HB_ਟੈਗ ('s','s','1','9'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_NINETEEN_ON,      HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_NINETEEN_OFF},
  {HB_ਟੈਗ ('s','s','2','0'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLISTIC_ALTERNATIVES,  HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWENTY_ON,        HB_AAT_LAYOUT_FEATURE_SELECTOR_STYLISTIC_ALT_TWENTY_OFF},
  {HB_ਟੈਗ ('s','u','b','s'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_POSITION,       HB_AAT_LAYOUT_FEATURE_SELECTOR_INFERIORS,                      HB_AAT_LAYOUT_FEATURE_SELECTOR_ਸਾਧਾਰਨ_POSITION},
  {HB_ਟੈਗ ('s','u','p','s'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_POSITION,       HB_AAT_LAYOUT_FEATURE_SELECTOR_SUPERIORS,                      HB_AAT_LAYOUT_FEATURE_SELECTOR_ਸਾਧਾਰਨ_POSITION},
  {HB_ਟੈਗ ('s','w','s','h'), HB_AAT_LAYOUT_FEATURE_TYPE_CONTEXTUAL_ALTERNATIVES, HB_AAT_LAYOUT_FEATURE_SELECTOR_SWASH_ALTERNATES_ON,            HB_AAT_LAYOUT_FEATURE_SELECTOR_SWASH_ALTERNATES_OFF},
  {HB_ਟੈਗ ('t','i','t','l'), HB_AAT_LAYOUT_FEATURE_TYPE_STYLE_OPTIONS,           HB_AAT_LAYOUT_FEATURE_SELECTOR_TITLING_CAPS,                   HB_AAT_LAYOUT_FEATURE_SELECTOR_NO_STYLE_OPTIONS},
  {HB_ਟੈਗ ('t','n','a','m'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_TRADITIONAL_ਨਾਂ_CHARACTERS,   (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('t','n','u','m'), HB_AAT_LAYOUT_FEATURE_TYPE_ਸੰਖਿਆ_SPACING,          HB_AAT_LAYOUT_FEATURE_SELECTOR_MONOSPACED_NUMBERS,             (hb_aat_layout_feature_selector_t) 4},
  {HB_ਟੈਗ ('t','r','a','d'), HB_AAT_LAYOUT_FEATURE_TYPE_CHARACTER_SHAPE,         HB_AAT_LAYOUT_FEATURE_SELECTOR_TRADITIONAL_CHARACTERS,         (hb_aat_layout_feature_selector_t) 16},
  {HB_ਟੈਗ ('t','w','i','d'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ਤੀਜਾ_ਚੌੜਾਈ_ਪਾਠ,               (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('u','n','i','c'), HB_AAT_LAYOUT_FEATURE_TYPE_LETTER_CASE,             (hb_aat_layout_feature_selector_t) 14,                 (hb_aat_layout_feature_selector_t) 15},
  {HB_ਟੈਗ ('v','a','l','t'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ALT_PROPORTIONAL_ਪਾਠ,          (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('v','e','r','t'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_SUBSTITUTION,   HB_AAT_LAYOUT_FEATURE_SELECTOR_SUBSTITUTE_VERTICAL_FORMS_ON,   HB_AAT_LAYOUT_FEATURE_SELECTOR_SUBSTITUTE_VERTICAL_FORMS_OFF},
  {HB_ਟੈਗ ('v','h','a','l'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ALT_HALF_ਚੌੜਾਈ_ਪਾਠ,            (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('v','k','n','a'), HB_AAT_LAYOUT_FEATURE_TYPE_ALTERNATE_KANA,          HB_AAT_LAYOUT_FEATURE_SELECTOR_ALTERNATE_VERT_KANA_ON,         HB_AAT_LAYOUT_FEATURE_SELECTOR_ALTERNATE_VERT_KANA_OFF},
  {HB_ਟੈਗ ('v','p','a','l'), HB_AAT_LAYOUT_FEATURE_TYPE_ਪਾਠ_SPACING,            HB_AAT_LAYOUT_FEATURE_SELECTOR_ALT_PROPORTIONAL_ਪਾਠ,          (hb_aat_layout_feature_selector_t) 7},
  {HB_ਟੈਗ ('v','r','t','2'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_SUBSTITUTION,   HB_AAT_LAYOUT_FEATURE_SELECTOR_SUBSTITUTE_VERTICAL_FORMS_ON,   HB_AAT_LAYOUT_FEATURE_SELECTOR_SUBSTITUTE_VERTICAL_FORMS_OFF},
  {HB_ਟੈਗ ('v','r','t','r'), HB_AAT_LAYOUT_FEATURE_TYPE_VERTICAL_SUBSTITUTION,   (hb_aat_layout_feature_selector_t) 2,                          (hb_aat_layout_feature_selector_t) 3},
  {HB_ਟੈਗ ('z','e','r','o'), HB_AAT_LAYOUT_FEATURE_TYPE_TYPOGRAPHIC_EXTRAS,      HB_AAT_LAYOUT_FEATURE_SELECTOR_SLASHED_ZERO_ON,                HB_AAT_LAYOUT_FEATURE_SELECTOR_SLASHED_ZERO_OFF},
};

/**
 * hb_aat_layout_find_feature_mapping:
 * @tag: The requested #hb_tag_t feature tag
 *
 * Fetches the AAT feature-and-selector combination that corresponds
 * to a given OpenType feature tag.
 *
 * Return value: the AAT features and selectors corresponding to the
 * OpenType feature tag queried
 *
 **/
ਅਚਲ hb_aat_feature_mapping_t *
hb_aat_layout_ਲੱਭੋ_feature_mapping (hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਵਾਪਸ hb_sorted_array (feature_mappings).bsearch (ਟੈਗ_2);
}
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_SHAPE

/*
 * mort/morx/kerx/trak
 */


ਤਾਰਕਿਕ
AAT::morx::is_blocklisted (hb_blob_t *blob,
                           hb_face_t *face) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_LAYOUT_BLOCKLIST
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ

  ਚੋਣ HB_CODEPOINT_ENCODE3 (blob->ਲੰਮਾਈ,
                               face->ਸਾਰਣੀ.GSUB->ਸਾਰਣੀ.get_ਲੰਮਾਈ (),
                               face->ਸਾਰਣੀ.GDEF->ਸਾਰਣੀ.get_ਲੰਮਾਈ ())
  {
    /* https://github.com/harfbuzz/harfbuzz/issues/4108
       sha1sum:a71ca6813b7e56a772cffff7c24a5166b087197c  AALMAGHRIBI.ttf */
    ਹਾਲਤ HB_CODEPOINT_ENCODE3 (19892, 2794, 340):
      ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਤਾਰਕਿਕ
AAT::mort::is_blocklisted (hb_blob_t *blob,
                           hb_face_t *face) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_AAT_LAYOUT_BLOCKLIST
  ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}

ਖਾਲੀ
hb_aat_layout_compile_map (ਅਚਲ hb_aat_map_builder_t *mapper,
			   hb_aat_map_t *map)
{
  ਅਚਲ AAT::morx& morx = *mapper->face->ਸਾਰਣੀ.morx->ਸਾਰਣੀ;
  ਜੇ (morx.has_ਅੰਕੜੇ ())
  {
    morx.compile_flags (mapper, map);
    ਵਾਪਸ;
  }

  ਅਚਲ AAT::mort& mort = *mapper->face->ਸਾਰਣੀ.mort->ਸਾਰਣੀ;
  ਜੇ (mort.has_ਅੰਕੜੇ ())
  {
    mort.compile_flags (mapper, map);
    ਵਾਪਸ;
  }
}


/**
 * hb_aat_layout_has_substitution:
 * @face: #hb_face_t to work upon
 *
 * Tests whether the specified face includes any substitutions in the
 * `morx` or `mort` tables.
 *
 * <note>Note: does not examine the `GSUB` table.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.3.0
 */
hb_bool_t
hb_aat_layout_has_substitution (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.morx->ਸਾਰਣੀ->has_ਅੰਕੜੇ () ||
	 face->ਸਾਰਣੀ.mort->ਸਾਰਣੀ->has_ਅੰਕੜੇ ();
}

ਖਾਲੀ
hb_aat_layout_substitute (ਅਚਲ hb_ot_shape_plan_t *plan,
			  hb_font_t *font,
			  hb_ਬਫਰ_t *ਬਫਰ,
			  ਅਚਲ hb_feature_t *features,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_features)
{
  hb_aat_map_t map;
  ਜੇ (ਅੰਕ_features)
  {
    hb_aat_map_builder_t builder (font->face, plan->props);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_features; i++)
      builder.ਜੋੜੋ_feature (features[i]);
    builder.compile (map);
  }

  {
    ਸਵੈਚਾਲਿਤ &accel = *font->face->ਸਾਰਣੀ.morx;
    ਅਚਲ AAT::morx& morx = *accel.ਸਾਰਣੀ;
    ਜੇ (morx.has_ਅੰਕੜੇ ())
    {
      AAT::hb_aat_apply_context_t c (plan, font, ਬਫਰ, accel.get_blob ());
      ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start table morx")) ਵਾਪਸ;
      c.ਬਫਰ_glyph_ਸੈੱਟ = accel.scratch.create_ਬਫਰ_glyph_ਸੈੱਟ ();
      morx.apply (&c, ਅੰਕ_features ? map : plan->aat_map, accel);
      accel.scratch.destroy_ਬਫਰ_glyph_ਸੈੱਟ (c.ਬਫਰ_glyph_ਸੈੱਟ);
      c.ਬਫਰ_glyph_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
      (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table morx");
      ਵਾਪਸ;
    }
  }

  {
    ਸਵੈਚਾਲਿਤ &accel = *font->face->ਸਾਰਣੀ.mort;
    ਅਚਲ AAT::mort& mort = *accel.ਸਾਰਣੀ;
    ਜੇ (mort.has_ਅੰਕੜੇ ())
    {
      AAT::hb_aat_apply_context_t c (plan, font, ਬਫਰ, accel.get_blob ());
      ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start table mort")) ਵਾਪਸ;
      mort.apply (&c, ਅੰਕ_features ? map : plan->aat_map, accel);
      (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table mort");
      ਵਾਪਸ;
    }
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
is_deleted_glyph (ਅਚਲ hb_glyph_info_t *info)
{
  ਵਾਪਸ _hb_glyph_info_is_aat_deleted (info);
}

ਖਾਲੀ
hb_aat_layout_remove_deleted_glyphs (hb_ਬਫਰ_t *ਬਫਰ)
{
  ਜੇ (ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_AAT_HAS_DELETED)
    ਬਫਰ->delete_glyphs_inplace (is_deleted_glyph);
}

/**
 * hb_aat_layout_has_positioning:
 * @face: #hb_face_t to work upon
 *
 * Tests whether the specified face includes any positioning information
 * in the `kerx` table.
 *
 * <note>Note: does not examine the `GPOS` table.</note>
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.3.0
 */
hb_bool_t
hb_aat_layout_has_positioning (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.kerx->ਸਾਰਣੀ->has_ਅੰਕੜੇ ();
}

ਖਾਲੀ
hb_aat_layout_position (ਅਚਲ hb_ot_shape_plan_t *plan,
			hb_font_t *font,
			hb_ਬਫਰ_t *ਬਫਰ)
{
  ਸਵੈਚਾਲਿਤ &accel = *font->face->ਸਾਰਣੀ.kerx;

  AAT::hb_aat_apply_context_t c (plan, font, ਬਫਰ, accel.get_blob ());
  ਜੇ (!ਬਫਰ->ਸੁਨੇਹਾ (font, "start table kerx")) ਵਾਪਸ;
  c.ਬਫਰ_glyph_ਸੈੱਟ = accel.scratch.create_ਬਫਰ_glyph_ਸੈੱਟ ();
  c.ਸੈੱਟ_ankr_ਸਾਰਣੀ (font->face->ਸਾਰਣੀ.ankr.get ());
  accel.apply (&c);
  accel.scratch.destroy_ਬਫਰ_glyph_ਸੈੱਟ (c.ਬਫਰ_glyph_ਸੈੱਟ);
  c.ਬਫਰ_glyph_ਸੈੱਟ = ਖਾਲੀ_ਸੂਚਕ;
  (ਖਾਲੀ) ਬਫਰ->ਸੁਨੇਹਾ (font, "end table kerx");
}


/**
 * hb_aat_layout_has_tracking:
 * @face:: #hb_face_t to work upon
 *
 * Tests whether the specified face includes any tracking information
 * in the `trak` table.
 *
 * Return value: `true` if data found, `false` otherwise
 *
 * Since: 2.3.0
 */
hb_bool_t
hb_aat_layout_has_tracking (hb_face_t *face)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.trak->has_ਅੰਕੜੇ ();
}

ਖਾਲੀ
hb_aat_layout_track (ਅਚਲ hb_ot_shape_plan_t *plan,
		     hb_font_t *font,
		     hb_ਬਫਰ_t *ਬਫਰ)
{
  ਅਚਲ AAT::trak& trak = *font->face->ਸਾਰਣੀ.trak;

  AAT::hb_aat_apply_context_t c (plan, font, ਬਫਰ);
  trak.apply (&c);
}

/**
 * hb_aat_layout_get_feature_types:
 * @face: #hb_face_t to work upon
 * @start_offset: offset of the first feature type to retrieve
 * @feature_count: (inout) (optional): Input = the maximum number of feature types to return;
 *                 Output = the actual number of feature types returned (may be zero)
 * @features: (out caller-allocates) (array length=feature_count) (nullable): Array of feature types found
 *
 * Fetches a list of the AAT feature types included in the specified face.
 *
 * Return value: Number of all available feature types.
 *
 * Since: 2.2.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_aat_layout_get_feature_types (hb_face_t                    *face,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                  ਸ਼ੁਰੂ_ਵਿੱਥ,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                 *feature_ਗਿਣਤੀ_2, /* IN/OUT.  May be NULL. */
				 hb_aat_layout_feature_type_t *features       /* OUT.     May be NULL. */)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.feat->get_feature_types (ਸ਼ੁਰੂ_ਵਿੱਥ, feature_ਗਿਣਤੀ_2, features);
}

/**
 * hb_aat_layout_feature_type_get_name_id:
 * @face: #hb_face_t to work upon
 * @feature_type: The #hb_aat_layout_feature_type_t of the requested feature type
 *
 * Fetches the name identifier of the specified feature type in the face's `name` table.
 *
 * Return value: Name identifier of the requested feature type
 *
 * Since: 2.2.0
 */
hb_ot_name_id_t
hb_aat_layout_feature_type_get_name_id (hb_face_t                    *face,
					hb_aat_layout_feature_type_t  feature_type)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.feat->get_feature_name_id (feature_type);
}

/**
 * hb_aat_layout_feature_type_get_selector_infos:
 * @face: #hb_face_t to work upon
 * @feature_type: The #hb_aat_layout_feature_type_t of the requested feature type
 * @start_offset: offset of the first feature type to retrieve
 * @selector_count: (inout) (optional): Input = the maximum number of selectors to return;
 *                  Output = the actual number of selectors returned (may be zero)
 * @selectors: (out caller-allocates) (array length=selector_count) (optional):
 *             A buffer pointer. The selectors available for the feature type queries.
 * @default_index: (out) (optional): The index of the feature's default selector, if any
 *
 * Fetches a list of the selectors available for the specified feature in the given face.
 *
 * If upon return, @default_index is set to #HB_AAT_LAYOUT_NO_SELECTOR_INDEX, then
 * the feature type is non-exclusive.  Otherwise, @default_index is the index of
 * the selector that is selected by default.
 *
 * Return value: Number of all available feature selectors
 *
 * Since: 2.2.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
hb_aat_layout_feature_type_get_selector_infos (hb_face_t                             *face,
					       hb_aat_layout_feature_type_t           feature_type,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                           ਸ਼ੁਰੂ_ਵਿੱਥ,
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *selector_ਗਿਣਤੀ, /* IN/OUT.  May be NULL. */
					       hb_aat_layout_feature_selector_info_t *selectors,      /* OUT.     May be NULL. */
					       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ                          *default_ਸੂਚਕਾਂਕ   /* OUT.     May be NULL. */)
{
  ਵਾਪਸ face->ਸਾਰਣੀ.feat->get_selector_infos (feature_type, ਸ਼ੁਰੂ_ਵਿੱਥ, selector_ਗਿਣਤੀ, selectors, default_ਸੂਚਕਾਂਕ);
}


#ਜੇ_ਅੰਤ
