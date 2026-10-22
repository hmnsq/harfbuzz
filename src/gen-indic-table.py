#!/usr/bin/env python3

"""usage: ./gen-indic-table.py [--rust] IndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

استورد sys

استورد packTab

إذا لمبائی(sys.argv) > 1 و sys.argv[1] == "--rust":
	ہٹاؤ sys.argv[1]
	language = packTab.languages["rust"]
وإلا:
	language = packTab.languages["c"]

إذا لمبائی (sys.argv) != 4:
	sys.exit (__doc__)

ALLOWED_SINGLES = [0x00A0, 0x25CC]
ALLOWED_بلاکاں = [
	'Basic Latin',
	'Latin-1 Supplement',
	'Devanagari',
	'Bengali',
	'Gurmukhi',
	'Gujarati',
	'Oriya',
	'Tamil',
	'Telugu',
	'Kannada',
	'Malayalam',
	'Myanmar',
	'Khmer',
	'Vedic Extensions',
	'General Punctuation',
	'Superscripts and Subscripts',
	'Devanagari Extended',
	'Myanmar Extended-B',
	'Myanmar Extended-A',
	'Myanmar Extended-C',
]

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرخیاں = [[f.readline () لكل i في سلسلہ (2)] لكل f في files]

unicode_انکڑے = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل سطر في f:

		j = سطر.لبھو ('#')
		إذا j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () لكل x في سطر.ونڈو (';')]
		إذا لمبائی (fields) == 1:
			استمر

		uu = fields[0].ونڈو ('..')
		شروع = صحيح (uu[0], 16)
		إذا لمبائی (uu) == 1:
			سرا = شروع
		وإلا:
			سرا = صحيح (uu[1], 16)

		t = fields[1]

		لكل u في سلسلہ (شروع, سرا + 1):
			unicode_انکڑے[i][u] = t

# Merge data into one dict:
defaults = ('Other', 'Not_Applicable', 'No_Block')
combined = {}
لكل i,d في enumerate (unicode_انکڑے):
	لكل u,v في d.اشیاء ():
		إذا i == 2 و ليس u في combined:
			استمر
		إذا ليس u في combined:
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k:v لكل k,v في combined.اشیاء() إذا k في ALLOWED_SINGLES أو v[2] في ALLOWED_بلاکاں}


# Convert categories & positions types

categories = {
  'indic' : [
    'X',
    'C',
    'V',
    'N',
    'H',
    'ZWNJ',
    'ZWJ',
    'M',
    'SM',
    'A',
    'VD',
    'PLACEHOLDER',
    'DOTTEDCIRCLE',
    'RS',
    'MPst',
    'Repha',
    'Ra',
    'CM',
    'Symbol',
    'CS',
    'SMPst',
  ],
  'khmer' : [
    'VAbv',
    'VBlw',
    'VPre',
    'VPst',

    'Robatic',
    'Xgroup',
    'Ygroup',
  ],
  'myanmar' : [
    'VAbv',
    'VBlw',
    'VPre',
    'VPst',

    'IV',
    'As',
    'DB',
    'GB',
    'MH',
    'MR',
    'MW',
    'MY',
    'PT',
    'VS',
    'ML',
  ],
}

category_map = {
  'Other'			: 'X',
  'Avagraha'			: 'Symbol',
  'Bindu'			: 'SM',
  'Brahmi_Joining_Number'	: 'PLACEHOLDER', # Don't care.
  'Cantillation_Mark'		: 'A',
  'Consonant'			: 'C',
  'Consonant_Dead'		: 'C',
  'Consonant_Final'		: 'CM',
  'Consonant_Head_Letter'	: 'C',
  'Consonant_Initial_Postfixed'	: 'C', # TODO
  'Consonant_Killer'		: 'M', # U+17CD only.
  'Consonant_Medial'		: 'CM',
  'Consonant_Placeholder'	: 'PLACEHOLDER',
  'Consonant_Preceding_Repha'	: 'Repha',
  'Consonant_Prefixed'		: 'X', # Don't care.
  'Consonant_Subjoined'		: 'CM',
  'Consonant_Succeeding_Repha'	: 'CM',
  'Consonant_With_Stacker'	: 'CS',
  'Gemination_Mark'		: 'SM', # https://github.com/harfbuzz/harfbuzz/issues/552
  'Invisible_Stacker'		: 'H',
  'Joiner'			: 'ZWJ',
  'Modifying_Letter'		: 'X',
  'Non_Joiner'			: 'ZWNJ',
  'Nukta'			: 'N',
  'Number'			: 'PLACEHOLDER',
  'Number_Joiner'		: 'PLACEHOLDER', # Don't care.
  'Pure_Killer'			: 'M', # Is like a vowel matra.
  'Register_Shifter'		: 'RS',
  'Syllable_Modifier'		: 'SM',
  'Tone_Letter'			: 'X',
  'Tone_Mark'			: 'N',
  'Virama'			: 'H',
  'Visarga'			: 'SM',
  'Vowel'			: 'V',
  'Vowel_Dependent'		: 'M',
  'Vowel_Independent'		: 'V',
}
position_map = {
  'Not_Applicable'		: 'END',

  'Left'			: 'PRE_C',
  'Top'				: 'ABOVE_C',
  'Bottom'			: 'BELOW_C',
  'Right'			: 'POST_C',

  # These should resolve to the position of the last part of the split sequence.
  'Bottom_And_Right'		: 'POST_C',
  'Left_And_Right'		: 'POST_C',
  'Top_And_Bottom'		: 'BELOW_C',
  'Top_And_Bottom_And_Left'	: 'BELOW_C',
  'Top_And_Bottom_And_Right'	: 'POST_C',
  'Top_And_Left'		: 'ABOVE_C',
  'Top_And_Left_And_Right'	: 'POST_C',
  'Top_And_Right'		: 'POST_C',

  'Overstruck'			: 'AFTER_MAIN',
  'Visual_order_left'		: 'PRE_M',
}

category_overrides = {

  # These are the variation-selectors. They only appear in the Myanmar grammar
  # but are not Myanmar-specific
  0xFE00: 'VS',
  0xFE01: 'VS',
  0xFE02: 'VS',
  0xFE03: 'VS',
  0xFE04: 'VS',
  0xFE05: 'VS',
  0xFE06: 'VS',
  0xFE07: 'VS',
  0xFE08: 'VS',
  0xFE09: 'VS',
  0xFE0A: 'VS',
  0xFE0B: 'VS',
  0xFE0C: 'VS',
  0xFE0D: 'VS',
  0xFE0E: 'VS',
  0xFE0F: 'VS',

  # These appear in the OT Myanmar spec, but are not Myanmar-specific
  0x2015: 'PLACEHOLDER',
  0x2022: 'PLACEHOLDER',
  0x25FB: 'PLACEHOLDER',
  0x25FC: 'PLACEHOLDER',
  0x25FD: 'PLACEHOLDER',
  0x25FE: 'PLACEHOLDER',


  # Indic

  0x0930: 'Ra', # Devanagari
  0x09B0: 'Ra', # Bengali
  0x09F0: 'Ra', # Bengali
  0x0A30: 'Ra', # Gurmukhi 	No Reph
  0x0AB0: 'Ra', # Gujarati
  0x0B30: 'Ra', # Oriya
  0x0BB0: 'Ra', # Tamil 	No Reph
  0x0C30: 'Ra', # Telugu 	Reph formed only with ZWJ
  0x0CB0: 'Ra', # Kannada
  0x0D30: 'Ra', # Malayalam 	No Reph, Logical Repha

  # The following act more like the Bindus.
  0x0953: 'SM',
  0x0954: 'SM',

  # U+0A40 GURMUKHI VOWEL SIGN II may be preceded by U+0A02 GURMUKHI SIGN BINDI.
  0x0A40: 'MPst',

  # The following act like consonants.
  0x0A72: 'C',
  0x0A73: 'C',

  # TODO: The following should only be allowed after a Visarga.
  # For now, just treat them like regular tone marks.
  0x1CE2: 'A',
  0x1CE3: 'A',
  0x1CE4: 'A',
  0x1CE5: 'A',
  0x1CE6: 'A',
  0x1CE7: 'A',
  0x1CE8: 'A',

  # TODO: The following should only be allowed after some of
  # the nasalization marks, maybe only for U+1CE9..U+1CF1.
  # For now, just treat them like tone marks.
  0x1CED: 'A',

  # The following take marks in standalone clusters, similar to Avagraha.
  0xA8F2: 'Symbol',
  0xA8F3: 'Symbol',
  0xA8F4: 'Symbol',
  0xA8F5: 'Symbol',
  0xA8F6: 'Symbol',
  0xA8F7: 'Symbol',
  0x1CE9: 'Symbol',
  0x1CEA: 'Symbol',
  0x1CEB: 'Symbol',
  0x1CEC: 'Symbol',
  0x1CEE: 'Symbol',
  0x1CEF: 'Symbol',
  0x1CF0: 'Symbol',
  0x1CF1: 'Symbol',

  0x0A51: 'M', # https://github.com/harfbuzz/harfbuzz/issues/524

  # According to ScriptExtensions.txt, these Grantha marks may also be used in Tamil,
  # so the Indic shaper needs to know their categories.
  0x11301: 'SM',
  0x11302: 'SM',
  0x11303: 'SM',
  0x1133B: 'N',
  0x1133C: 'N',

  0x0AFB: 'N', # https://github.com/harfbuzz/harfbuzz/issues/552
  0x0B55: 'N', # https://github.com/harfbuzz/harfbuzz/issues/2849

  0x09FC: 'PLACEHOLDER', # https://github.com/harfbuzz/harfbuzz/pull/1613
  0x0C80: 'PLACEHOLDER', # https://github.com/harfbuzz/harfbuzz/pull/623
  0x0D04: 'PLACEHOLDER', # https://github.com/harfbuzz/harfbuzz/pull/3511

  0x25CC: 'DOTTEDCIRCLE',


  # Khmer

  0x179A: 'Ra',

  0x17CC: 'Robatic',
  0x17C9: 'Robatic',
  0x17CA: 'Robatic',

  0x17C6: 'Xgroup',
  0x17CB: 'Xgroup',
  0x17CD: 'Xgroup',
  0x17CE: 'Xgroup',
  0x17CF: 'Xgroup',
  0x17D0: 'Xgroup',
  0x17D1: 'Xgroup',

  0x17C7: 'Ygroup',
  0x17C8: 'Ygroup',
  0x17DD: 'Ygroup',
  0x17D3: 'Ygroup', # Just guessing. Uniscribe doesn't categorize it.

  0x17D9: 'PLACEHOLDER', # https://github.com/harfbuzz/harfbuzz/issues/2384


  # Myanmar

  # https://docs.microsoft.com/en-us/typography/script-development/myanmar#analyze

  0x104E: 'C', # The spec says C, IndicSyllableCategory says Consonant_Placeholder

  0x1004: 'Ra',
  0x101B: 'Ra',
  0x105A: 'Ra',

  0x1032: 'A',
  0x1036: 'A',

  0x103A: 'As',

  #0x1040: 'D0', # XXX The spec says D0, but Uniscribe doesn't seem to do.

  0x103E: 'MH',
  0x1060: 'ML',
  0x103C: 'MR',
  0x103D: 'MW',
  0x1082: 'MW',
  0x103B: 'MY',
  0x105E: 'MY',
  0x105F: 'MY',

  0x1063: 'PT',
  0x1064: 'PT',
  0x1069: 'PT',
  0x106A: 'PT',
  0x106B: 'PT',
  0x106C: 'PT',
  0x106D: 'PT',
  0xAA7B: 'PT',

  0x1038: 'SM',
  0x1087: 'SM',
  0x1088: 'SM',
  0x1089: 'SM',
  0x108A: 'SM',
  0x108B: 'SM',
  0x108C: 'SM',
  0x108D: 'SM',
  0x108F: 'SM',
  0x109A: 'SM',
  0x109B: 'SM',
  0x109C: 'SM',

  0x104A: 'PLACEHOLDER',
}
position_overrides = {

  0x0A51: 'BELOW_C', # https://github.com/harfbuzz/harfbuzz/issues/524

  0x0B01: 'BEFORE_SUB', # Oriya Bindu is BeforeSub in the spec.
}

تعریف matra_pos_بایاں(u, بلاک):
  أرجع "PRE_M"
تعریف matra_pos_دایاں(u, بلاک):
  إذا بلاک == 'Devanagari':	أرجع  'AFTER_SUB'
  إذا بلاک == 'Bengali':	أرجع  'AFTER_POST'
  إذا بلاک == 'Gurmukhi':	أرجع  'AFTER_POST'
  إذا بلاک == 'Gujarati':	أرجع  'AFTER_POST'
  إذا بلاک == 'Oriya':		أرجع  'AFTER_POST'
  إذا بلاک == 'Tamil':		أرجع  'AFTER_POST'
  إذا بلاک == 'Telugu':		أرجع  'BEFORE_SUB' إذا u <= 0x0C42 وإلا 'AFTER_SUB'
  إذا بلاک == 'Kannada':	أرجع  'BEFORE_SUB' إذا u < 0x0CC3 أو u > 0x0CD6 وإلا 'AFTER_SUB'
  إذا بلاک == 'Malayalam':	أرجع  'AFTER_POST'
  أرجع 'AFTER_SUB'
تعریف matra_pos_چوٹی(u, بلاک):
  # BENG and MLYM don't have top matras.
  إذا بلاک == 'Devanagari':	أرجع  'AFTER_SUB'
  إذا بلاک == 'Gurmukhi':	أرجع  'AFTER_POST' # Deviate from spec
  إذا بلاک == 'Gujarati':	أرجع  'AFTER_SUB'
  إذا بلاک == 'Oriya':		أرجع  'AFTER_MAIN'
  إذا بلاک == 'Tamil':		أرجع  'AFTER_SUB'
  إذا بلاک == 'Telugu':		أرجع  'BEFORE_SUB'
  إذا بلاک == 'Kannada':	أرجع  'BEFORE_SUB'
  أرجع 'AFTER_SUB'
تعریف matra_pos_bottom(u, بلاک):
  إذا بلاک == 'Devanagari':	أرجع  'AFTER_SUB'
  إذا بلاک == 'Bengali':	أرجع  'AFTER_SUB'
  إذا بلاک == 'Gurmukhi':	أرجع  'AFTER_POST'
  إذا بلاک == 'Gujarati':	أرجع  'AFTER_POST'
  إذا بلاک == 'Oriya':		أرجع  'AFTER_SUB'
  إذا بلاک == 'Tamil':		أرجع  'AFTER_POST'
  إذا بلاک == 'Telugu':		أرجع  'BEFORE_SUB'
  إذا بلاک == 'Kannada':	أرجع  'BEFORE_SUB'
  إذا بلاک == 'Malayalam':	أرجع  'AFTER_POST'
  أرجع "AFTER_SUB"
تعریف indic_matra_position(u, pos, بلاک): # Reposition matra
  إذا pos == 'PRE_C':	أرجع matra_pos_بایاں(u, بلاک)
  إذا pos == 'POST_C':	أرجع matra_pos_دایاں(u, بلاک)
  إذا pos == 'ABOVE_C':	أرجع matra_pos_چوٹی(u, بلاک)
  إذا pos == 'BELOW_C':	أرجع matra_pos_bottom(u, بلاک)
  أكد (خطأ)

تعریف position_منزل_category(pos):
  إذا pos == 'PRE_C':	أرجع 'VPre'
  إذا pos == 'ABOVE_C':	أرجع 'VAbv'
  إذا pos == 'BELOW_C':	أرجع 'VBlw'
  إذا pos == 'POST_C':	أرجع 'VPst'
  أكد(خطأ)


defaults = (category_map[defaults[0]], position_map[defaults[1]], defaults[2])

indic_انکڑے = {}
لكل k, (cat, pos, بلاک) في combined.اشیاء():
  cat = category_map[cat]
  إذا cat == 'SM' و pos == 'Not_Applicable':
    cat = 'SMPst'
  pos = position_map[pos]
  indic_انکڑے[k] = (cat, pos, بلاک)

لكل k,new_cat في category_overrides.اشیاء():
  (cat, pos, _) = indic_انکڑے.get(k, defaults)
  indic_انکڑے[k] = (new_cat, pos, unicode_انکڑے[2][k])

# We only expect position for certain types
positioned_categories = ('CM', 'SM', 'RS', 'H', 'M', 'MPst')
لكل k, (cat, pos, بلاک) في indic_انکڑے.اشیاء():
  إذا cat ليس في positioned_categories:
    pos = 'END'
    indic_انکڑے[k] = (cat, pos, بلاک)

# Position overrides are more complicated

# Keep in sync with CONSONANT_FLAGS in the shaper
consonant_categories = ('C', 'CS', 'Ra','CM', 'V', 'PLACEHOLDER', 'DOTTEDCIRCLE')
matra_categories = ('M', 'MPst')
smvd_categories = ('SM', 'SMPst', 'VD', 'A', 'Symbol')
لكل k, (cat, pos, بلاک) في indic_انکڑے.اشیاء():
  إذا cat في consonant_categories:
    pos = 'BASE_C'
  وإلاإذا cat في matra_categories:
    إذا بلاک.startswith('Khmer') أو بلاک.startswith('Myanmar'):
      cat = position_منزل_category(pos)
    وإلا:
      pos = indic_matra_position(k, pos, بلاک)
  وإلاإذا cat في smvd_categories:
    pos = 'SMVD';
  indic_انکڑے[k] = (cat, pos, بلاک)

لكل k,new_pos في position_overrides.اشیاء():
  (cat, pos, _) = indic_انکڑے.get(k, defaults)
  indic_انکڑے[k] = (cat, new_pos, unicode_انکڑے[2][k])


اقدار = [{_: 1} لكل _ في defaults]
لكل vv في indic_انکڑے.اقدار():
  لكل i,v في enumerate(vv):
    اقدار[i][v] = اقدار[i].get (v, 0) + 1




# Move the outliers NO-BREAK SPACE and DOTTED CIRCLE out
singles = {}
لكل u في ALLOWED_SINGLES:
	singles[u] = indic_انکڑے[u]
	ہٹاؤ indic_انکڑے[u]

چھاپو ("/* == Start of generated table == */")
چھاپو ("/*")
چھاپو (" * The following table is generated by running:")
چھاپو (" *")
چھاپو (" *   ./gen-indic-table.py %sIndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt" %
		("--rust " إذا language.name == "rust" وإلا ""))
چھاپو (" *")
چھاپو (" * on files with these headers:")
چھاپو (" *")
لكل h في سرخیاں:
	لكل l في h:
		چھاپو (" * %s" % (l.strip()))
چھاپو (" */")
چھاپو ()
إذا language.name == "c":
	چھاپو ('#include "hb.hh"')
	چھاپو ()
	چھاپو ('#ifndef HB_NO_OT_SHAPE')
	چھاپو ()
	چھاپو ('#include "hb-ot-shaper-indic.hh"')
	چھاپو ()
	چھاپو ('#pragma GCC diagnostic push')
	چھاپو ('#pragma GCC diagnostic ignored "-Wunused-macros"')
	چھاپو ()

	لكل shaper في categories:
	  چھاپو ('#include "hb-ot-shaper-%s-machine.hh"' % shaper)
	چھاپو ()
	پورا = {}
	لكل shaper, shaper_cats في categories.اشیاء():
	  چھاپو ('/* %s */' % shaper)
	  لكل cat في shaper_cats:
	    v = shaper[0].اتلا()
	    إذا cat ليس في پورا:
	      چھاپو ("#define OT_%s %s_Cat(%s)" % (cat, v, cat))
	      پورا[cat] = v
	    وإلا:
	      چھاپو ('static_assert (OT_%s == %s_Cat(%s), "");' % (cat, v, cat))
	چھاپو ()
وإلاإذا language.name == "rust":
	چھاپو ("#![allow(unused_parens)]")
	چھاپو ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
	چھاپو ("#![allow(non_camel_case_types)]")
	چھاپو ("#![allow(unused_imports)]")
	چھاپو ()
	چھاپو ('use super::ot_shaper_indic::ot_category_t::*;')
	چھاپو ('use super::ot_shaper_indic::ot_position_t::*;')
	چھاپو ()
وإلا:
	أكد خطأ, "Unknown language: %s" % language.name

# Shorten values
چھوٹا = [{
	"Repha":		'Rf',
	"PLACEHOLDER":		'GB',
	"DOTTEDCIRCLE":		'DC',
	"SMPst":		'SP',
	"VPst":			'VR',
	"VPre":			'VL',
	"Robatic":		'Rt',
	"Xgroup":		'Xg',
	"Ygroup":		'Yg',
	"As":			'As',
},{
	"END":			'X',
	"BASE_C":		'C',
	"ABOVE_C":		'T',
	"BELOW_C":		'B',
	"POST_C":		'R',
	"PRE_C":		'L',
	"PRE_M":		'LM',
	"AFTER_MAIN":		'A',
	"AFTER_SUB":		'AS',
	"BEFORE_SUB":		'BS',
	"AFTER_POST":		'AP',
	"SMVD":			'SM',
}]
all_shorts = [{},{}]

# Add some of the values, to make them more readable, and to avoid duplicates

لكل i في سلسلہ (2):
	لكل v,s في چھوٹا[i].اشیاء ():
		all_shorts[i][s] = v

what = ["OT", "POS"]
what_short = ["_OT", "_POS"]
cat_defs = []
لكل i في سلسلہ (2):
	vv = sorted (اقدار[i].کلیدیں ())
	لكل v في vv:
		v_no_and = v.replace ('_And_', '_')
		إذا v في چھوٹا[i]:
			s = چھوٹا[i][v]
		وإلا:
			s = ''.join ([c لكل c في v_no_and إذا ord ('A') <= ord (c) <= ord ('Z')])
			إذا s في all_shorts[i]:
				ارفع Exception ("Duplicate short value alias", v, all_shorts[i][s])
			all_shorts[i][s] = v
			چھوٹا[i][v] = s
		cat_defs.append ((what_short[i] + '_' + s, what[i] + '_' + (v.اتلا () إذا i وإلا v), str (اقدار[i][v]), v))

maxlen_s = max ([لمبائی (c[0]) لكل c في cat_defs])
maxlen_l = max ([لمبائی (c[1]) لكل c في cat_defs])
maxlen_n = max ([لمبائی (c[2]) لكل c في cat_defs])
لكل s في what_short:
	چھاپو ()
	لكل c في [c لكل c في cat_defs إذا s في c[0]]:
		إذا language.name == "c":
			چھاپو ("#define %s %s /* %s chars; %s */" %
				(c[0].ljust (maxlen_s), c[1].ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
		وإلا:
			چھاپو ("use %s as %s /* %s chars; %s */" %
				(c[1].ljust (maxlen_s), (c[0] + ";").ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
چھاپو ()
إذا language.name == "c":
	چھاپو ('#pragma GCC diagnostic pop')
	چھاپو ()
	چھاپو ("#define INDIC_COMBINE_CATEGORIES(S,M) ((S) | ((M) << 8))")
	چھاپو ()
	چھاپو ("#define _(S,M) INDIC_COMBINE_CATEGORIES (%s_##S, %s_##M)" % tuple(what_short))
	چھاپو ()
	چھاپو ()
وإلا:
	چھاپو ("const fn indic_combine_categories (s: u8, m: u8) -> u16")
	چھاپو ("{")
	چھاپو ("  (s as u16) | ((m as u16) << 8)")
	چھاپو ("}")
	چھاپو ()
	چھاپو ()

إذا language.name == "c":
	default_قدر = "_(%s,%s)" % (چھوٹا[0][defaults[0]], چھوٹا[1][defaults[1]])
	pack_انکڑے = {
		u: "_(%s,%s)" % (چھوٹا[0][d[0]], چھوٹا[1][d[1]])
		لكل u, d في indic_انکڑے.اشیاء ()
	}
	pack_انکڑے.update ({
		u: "_(%s,%s)" % (چھوٹا[0][d[0]], چھوٹا[1][d[1]])
		لكل u, d في singles.اشیاء ()
	})
وإلا:
	default_قدر = "indic_combine_categories(_OT_%s, _POS_%s)" % (چھوٹا[0][defaults[0]], چھوٹا[1][defaults[1]])
	pack_انکڑے = {
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (چھوٹا[0][d[0]], چھوٹا[1][d[1]])
		لكل u, d في indic_انکڑے.اشیاء ()
	}
	pack_انکڑے.update ({
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (چھوٹا[0][d[0]], چھوٹا[1][d[1]])
		لكل u, d في singles.اشیاء ()
	})

unique_اقدار = sorted (سیٹ (pack_انکڑے.اقدار ()) | {default_قدر})
mapping = {v: i لكل i, v في enumerate (unique_اقدار)}

کوڈ = packTab.کوڈ_2 ('_hb_indic')
قدر_type = 'uint16_t' إذا language.name == "c" وإلا language.type_name ("u16")
indic_اقدار, _ = کوڈ.جوڑو_Array (قدر_type, 'values', unique_اقدار)
sol = packTab.pack_جدول (pack_انکڑے, افتراضي=default_قدر, mapping=mapping, compression=9)
sol.gen_کوڈ (کوڈ, 'get_categories_index', language=language, خاص=language.name == "c")
کوڈ.print_کوڈ (language=language, خاص=language.name == "c")
چھاپو ()
إذا language.name == "c":
	چھاپو ("uint16_t")
	چھاپو ("hb_indic_get_categories (hb_codepoint_t u)")
	چھاپو ("{")
	چھاپو ("  return %s[_hb_indic_get_categories_index (u)];" % indic_اقدار)
	چھاپو ("}")
	چھاپو ()
	چھاپو ("#undef _")
	چھاپو ("#undef INDIC_COMBINE_CATEGORIES")
	لكل i في سلسلہ (2):
		چھاپو ()
		vv = sorted (اقدار[i].کلیدیں ())
		لكل v في vv:
			چھاپو ("#undef %s_%s" %
				(what_short[i], چھوٹا[i][v]))
	چھاپو ()
	چھاپو ('#endif')
وإلا:
	چھاپو ("#[inline]")
	چھاپو ("pub(crate) fn get_categories (u: u32) -> (u8, u8)")
	چھاپو ("{")
	چھاپو ("  let v = %s[_hb_indic_get_categories_index (u as usize) as usize];" % indic_اقدار)
	چھاپو ("  ((v & 0xFF) as u8, (v >> 8) as u8)")
	چھاپو ("}")
چھاپو ()
چھاپو ("/* == End of generated table == */")
