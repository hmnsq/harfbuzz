#!/usr/bin/env python3

"""usage: ./gen-indic-table.py [--rust] IndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

దిగుమతి sys

దిగుమతి packTab

ఒకవేళ పొడవు(sys.argv) > 1 మరియు sys.argv[1] == "--rust":
	తీసివేయి sys.argv[1]
	language = packTab.languages["rust"]
లేకపోతే:
	language = packTab.languages["c"]

ఒకవేళ పొడవు (sys.argv) != 4:
	sys.exit (__doc__)

ALLOWED_SINGLES = [0x00A0, 0x25CC]
ALLOWED_ఖండాలు = [
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

files = [open (x, encoding='utf-8') కొరకు x లో sys.argv[1:]]

శీర్షికలు = [[f.readline () కొరకు i లో శ్రేణి (2)] కొరకు f లో files]

unicode_దత్తాంశం = [{} కొరకు _ లో files]
కొరకు i, f లో enumerate (files):
	కొరకు పంక్తి లో f:

		j = పంక్తి.కనుగొను ('#')
		ఒకవేళ j >= 0:
			పంక్తి = పంక్తి[:j]

		fields = [x.strip () కొరకు x లో పంక్తి.విభజించు (';')]
		ఒకవేళ పొడవు (fields) == 1:
			కొనసాగు

		uu = fields[0].విభజించు ('..')
		ప్రారంభం = పూర్ణాంకం (uu[0], 16)
		ఒకవేళ పొడవు (uu) == 1:
			చివర = ప్రారంభం
		లేకపోతే:
			చివర = పూర్ణాంకం (uu[1], 16)

		t = fields[1]

		కొరకు u లో శ్రేణి (ప్రారంభం, చివర + 1):
			unicode_దత్తాంశం[i][u] = t

# Merge data into one dict:
defaults = ('Other', 'Not_Applicable', 'No_Block')
combined = {}
కొరకు i,d లో enumerate (unicode_దత్తాంశం):
	కొరకు u,v లో d.వస్తువులు ():
		ఒకవేళ i == 2 మరియు కాదు u లో combined:
			కొనసాగు
		ఒకవేళ కాదు u లో combined:
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k:v కొరకు k,v లో combined.వస్తువులు() ఒకవేళ k లో ALLOWED_SINGLES లేదా v[2] లో ALLOWED_ఖండాలు}


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

నిర్వచనం matra_pos_ఎడమ(u, ఖండం):
  తిరిగి "PRE_M"
నిర్వచనం matra_pos_కుడి(u, ఖండం):
  ఒకవేళ ఖండం == 'Devanagari':	తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Bengali':	తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Gurmukhi':	తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Gujarati':	తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Oriya':		తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Tamil':		తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Telugu':		తిరిగి  'BEFORE_SUB' ఒకవేళ u <= 0x0C42 లేకపోతే 'AFTER_SUB'
  ఒకవేళ ఖండం == 'Kannada':	తిరిగి  'BEFORE_SUB' ఒకవేళ u < 0x0CC3 లేదా u > 0x0CD6 లేకపోతే 'AFTER_SUB'
  ఒకవేళ ఖండం == 'Malayalam':	తిరిగి  'AFTER_POST'
  తిరిగి 'AFTER_SUB'
నిర్వచనం matra_pos_శిఖరం(u, ఖండం):
  # BENG and MLYM don't have top matras.
  ఒకవేళ ఖండం == 'Devanagari':	తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Gurmukhi':	తిరిగి  'AFTER_POST' # Deviate from spec
  ఒకవేళ ఖండం == 'Gujarati':	తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Oriya':		తిరిగి  'AFTER_MAIN'
  ఒకవేళ ఖండం == 'Tamil':		తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Telugu':		తిరిగి  'BEFORE_SUB'
  ఒకవేళ ఖండం == 'Kannada':	తిరిగి  'BEFORE_SUB'
  తిరిగి 'AFTER_SUB'
నిర్వచనం matra_pos_bottom(u, ఖండం):
  ఒకవేళ ఖండం == 'Devanagari':	తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Bengali':	తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Gurmukhi':	తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Gujarati':	తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Oriya':		తిరిగి  'AFTER_SUB'
  ఒకవేళ ఖండం == 'Tamil':		తిరిగి  'AFTER_POST'
  ఒకవేళ ఖండం == 'Telugu':		తిరిగి  'BEFORE_SUB'
  ఒకవేళ ఖండం == 'Kannada':	తిరిగి  'BEFORE_SUB'
  ఒకవేళ ఖండం == 'Malayalam':	తిరిగి  'AFTER_POST'
  తిరిగి "AFTER_SUB"
నిర్వచనం indic_matra_position(u, pos, ఖండం): # Reposition matra
  ఒకవేళ pos == 'PRE_C':	తిరిగి matra_pos_ఎడమ(u, ఖండం)
  ఒకవేళ pos == 'POST_C':	తిరిగి matra_pos_కుడి(u, ఖండం)
  ఒకవేళ pos == 'ABOVE_C':	తిరిగి matra_pos_శిఖరం(u, ఖండం)
  ఒకవేళ pos == 'BELOW_C':	తిరిగి matra_pos_bottom(u, ఖండం)
  ప్రతిజ్ఞా (అబద్ధం)

నిర్వచనం position_గమ్యం_category(pos):
  ఒకవేళ pos == 'PRE_C':	తిరిగి 'VPre'
  ఒకవేళ pos == 'ABOVE_C':	తిరిగి 'VAbv'
  ఒకవేళ pos == 'BELOW_C':	తిరిగి 'VBlw'
  ఒకవేళ pos == 'POST_C':	తిరిగి 'VPst'
  ప్రతిజ్ఞా(అబద్ధం)


defaults = (category_map[defaults[0]], position_map[defaults[1]], defaults[2])

indic_దత్తాంశం = {}
కొరకు k, (cat, pos, ఖండం) లో combined.వస్తువులు():
  cat = category_map[cat]
  ఒకవేళ cat == 'SM' మరియు pos == 'Not_Applicable':
    cat = 'SMPst'
  pos = position_map[pos]
  indic_దత్తాంశం[k] = (cat, pos, ఖండం)

కొరకు k,new_cat లో category_overrides.వస్తువులు():
  (cat, pos, _) = indic_దత్తాంశం.get(k, defaults)
  indic_దత్తాంశం[k] = (new_cat, pos, unicode_దత్తాంశం[2][k])

# We only expect position for certain types
positioned_categories = ('CM', 'SM', 'RS', 'H', 'M', 'MPst')
కొరకు k, (cat, pos, ఖండం) లో indic_దత్తాంశం.వస్తువులు():
  ఒకవేళ cat కాదు లో positioned_categories:
    pos = 'END'
    indic_దత్తాంశం[k] = (cat, pos, ఖండం)

# Position overrides are more complicated

# Keep in sync with CONSONANT_FLAGS in the shaper
consonant_categories = ('C', 'CS', 'Ra','CM', 'V', 'PLACEHOLDER', 'DOTTEDCIRCLE')
matra_categories = ('M', 'MPst')
smvd_categories = ('SM', 'SMPst', 'VD', 'A', 'Symbol')
కొరకు k, (cat, pos, ఖండం) లో indic_దత్తాంశం.వస్తువులు():
  ఒకవేళ cat లో consonant_categories:
    pos = 'BASE_C'
  లేదావేళ cat లో matra_categories:
    ఒకవేళ ఖండం.startswith('Khmer') లేదా ఖండం.startswith('Myanmar'):
      cat = position_గమ్యం_category(pos)
    లేకపోతే:
      pos = indic_matra_position(k, pos, ఖండం)
  లేదావేళ cat లో smvd_categories:
    pos = 'SMVD';
  indic_దత్తాంశం[k] = (cat, pos, ఖండం)

కొరకు k,new_pos లో position_overrides.వస్తువులు():
  (cat, pos, _) = indic_దత్తాంశం.get(k, defaults)
  indic_దత్తాంశం[k] = (cat, new_pos, unicode_దత్తాంశం[2][k])


విలువలు = [{_: 1} కొరకు _ లో defaults]
కొరకు vv లో indic_దత్తాంశం.విలువలు():
  కొరకు i,v లో enumerate(vv):
    విలువలు[i][v] = విలువలు[i].get (v, 0) + 1




# Move the outliers NO-BREAK SPACE and DOTTED CIRCLE out
singles = {}
కొరకు u లో ALLOWED_SINGLES:
	singles[u] = indic_దత్తాంశం[u]
	తీసివేయి indic_దత్తాంశం[u]

ముద్రించు ("/* == Start of generated table == */")
ముద్రించు ("/*")
ముద్రించు (" * The following table is generated by running:")
ముద్రించు (" *")
ముద్రించు (" *   ./gen-indic-table.py %sIndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt" %
		("--rust " ఒకవేళ language.name == "rust" లేకపోతే ""))
ముద్రించు (" *")
ముద్రించు (" * on files with these headers:")
ముద్రించు (" *")
కొరకు h లో శీర్షికలు:
	కొరకు l లో h:
		ముద్రించు (" * %s" % (l.strip()))
ముద్రించు (" */")
ముద్రించు ()
ఒకవేళ language.name == "c":
	ముద్రించు ('#include "hb.hh"')
	ముద్రించు ()
	ముద్రించు ('#ifndef HB_NO_OT_SHAPE')
	ముద్రించు ()
	ముద్రించు ('#include "hb-ot-shaper-indic.hh"')
	ముద్రించు ()
	ముద్రించు ('#pragma GCC diagnostic push')
	ముద్రించు ('#pragma GCC diagnostic ignored "-Wunused-macros"')
	ముద్రించు ()

	కొరకు shaper లో categories:
	  ముద్రించు ('#include "hb-ot-shaper-%s-machine.hh"' % shaper)
	ముద్రించు ()
	పూర్తి = {}
	కొరకు shaper, shaper_cats లో categories.వస్తువులు():
	  ముద్రించు ('/* %s */' % shaper)
	  కొరకు cat లో shaper_cats:
	    v = shaper[0].పై()
	    ఒకవేళ cat కాదు లో పూర్తి:
	      ముద్రించు ("#define OT_%s %s_Cat(%s)" % (cat, v, cat))
	      పూర్తి[cat] = v
	    లేకపోతే:
	      ముద్రించు ('static_assert (OT_%s == %s_Cat(%s), "");' % (cat, v, cat))
	ముద్రించు ()
లేదావేళ language.name == "rust":
	ముద్రించు ("#![allow(unused_parens)]")
	ముద్రించు ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
	ముద్రించు ("#![allow(non_camel_case_types)]")
	ముద్రించు ("#![allow(unused_imports)]")
	ముద్రించు ()
	ముద్రించు ('use super::ot_shaper_indic::ot_category_t::*;')
	ముద్రించు ('use super::ot_shaper_indic::ot_position_t::*;')
	ముద్రించు ()
లేకపోతే:
	ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name

# Shorten values
పొట్టి = [{
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

కొరకు i లో శ్రేణి (2):
	కొరకు v,s లో పొట్టి[i].వస్తువులు ():
		all_shorts[i][s] = v

what = ["OT", "POS"]
what_short = ["_OT", "_POS"]
cat_defs = []
కొరకు i లో శ్రేణి (2):
	vv = sorted (విలువలు[i].కీలకాలు ())
	కొరకు v లో vv:
		v_no_and = v.replace ('_And_', '_')
		ఒకవేళ v లో పొట్టి[i]:
			s = పొట్టి[i][v]
		లేకపోతే:
			s = ''.join ([c కొరకు c లో v_no_and ఒకవేళ ord ('A') <= ord (c) <= ord ('Z')])
			ఒకవేళ s లో all_shorts[i]:
				లేపు Exception ("Duplicate short value alias", v, all_shorts[i][s])
			all_shorts[i][s] = v
			పొట్టి[i][v] = s
		cat_defs.append ((what_short[i] + '_' + s, what[i] + '_' + (v.పై () ఒకవేళ i లేకపోతే v), str (విలువలు[i][v]), v))

maxlen_s = max ([పొడవు (c[0]) కొరకు c లో cat_defs])
maxlen_l = max ([పొడవు (c[1]) కొరకు c లో cat_defs])
maxlen_n = max ([పొడవు (c[2]) కొరకు c లో cat_defs])
కొరకు s లో what_short:
	ముద్రించు ()
	కొరకు c లో [c కొరకు c లో cat_defs ఒకవేళ s లో c[0]]:
		ఒకవేళ language.name == "c":
			ముద్రించు ("#define %s %s /* %s chars; %s */" %
				(c[0].ljust (maxlen_s), c[1].ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
		లేకపోతే:
			ముద్రించు ("use %s as %s /* %s chars; %s */" %
				(c[1].ljust (maxlen_s), (c[0] + ";").ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
ముద్రించు ()
ఒకవేళ language.name == "c":
	ముద్రించు ('#pragma GCC diagnostic pop')
	ముద్రించు ()
	ముద్రించు ("#define INDIC_COMBINE_CATEGORIES(S,M) ((S) | ((M) << 8))")
	ముద్రించు ()
	ముద్రించు ("#define _(S,M) INDIC_COMBINE_CATEGORIES (%s_##S, %s_##M)" % tuple(what_short))
	ముద్రించు ()
	ముద్రించు ()
లేకపోతే:
	ముద్రించు ("const fn indic_combine_categories (s: u8, m: u8) -> u16")
	ముద్రించు ("{")
	ముద్రించు ("  (s as u16) | ((m as u16) << 8)")
	ముద్రించు ("}")
	ముద్రించు ()
	ముద్రించు ()

ఒకవేళ language.name == "c":
	default_విలువ = "_(%s,%s)" % (పొట్టి[0][defaults[0]], పొట్టి[1][defaults[1]])
	pack_దత్తాంశం = {
		u: "_(%s,%s)" % (పొట్టి[0][d[0]], పొట్టి[1][d[1]])
		కొరకు u, d లో indic_దత్తాంశం.వస్తువులు ()
	}
	pack_దత్తాంశం.update ({
		u: "_(%s,%s)" % (పొట్టి[0][d[0]], పొట్టి[1][d[1]])
		కొరకు u, d లో singles.వస్తువులు ()
	})
లేకపోతే:
	default_విలువ = "indic_combine_categories(_OT_%s, _POS_%s)" % (పొట్టి[0][defaults[0]], పొట్టి[1][defaults[1]])
	pack_దత్తాంశం = {
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (పొట్టి[0][d[0]], పొట్టి[1][d[1]])
		కొరకు u, d లో indic_దత్తాంశం.వస్తువులు ()
	}
	pack_దత్తాంశం.update ({
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (పొట్టి[0][d[0]], పొట్టి[1][d[1]])
		కొరకు u, d లో singles.వస్తువులు ()
	})

unique_విలువలు = sorted (సమితి (pack_దత్తాంశం.విలువలు ()) | {default_విలువ})
mapping = {v: i కొరకు i, v లో enumerate (unique_విలువలు)}

సంకేతం_2 = packTab.సంకేతం_3 ('_hb_indic')
విలువ_type = 'uint16_t' ఒకవేళ language.name == "c" లేకపోతే language.type_name ("u16")
indic_విలువలు, _ = సంకేతం_2.కూడు_Array (విలువ_type, 'values', unique_విలువలు)
sol = packTab.pack_పట్టిక (pack_దత్తాంశం, అప్రమేయ=default_విలువ, mapping=mapping, compression=9)
sol.gen_సంకేతం (సంకేతం_2, 'get_categories_index', language=language, అంతరంగ=language.name == "c")
సంకేతం_2.print_సంకేతం (language=language, అంతరంగ=language.name == "c")
ముద్రించు ()
ఒకవేళ language.name == "c":
	ముద్రించు ("uint16_t")
	ముద్రించు ("hb_indic_get_categories (hb_codepoint_t u)")
	ముద్రించు ("{")
	ముద్రించు ("  return %s[_hb_indic_get_categories_index (u)];" % indic_విలువలు)
	ముద్రించు ("}")
	ముద్రించు ()
	ముద్రించు ("#undef _")
	ముద్రించు ("#undef INDIC_COMBINE_CATEGORIES")
	కొరకు i లో శ్రేణి (2):
		ముద్రించు ()
		vv = sorted (విలువలు[i].కీలకాలు ())
		కొరకు v లో vv:
			ముద్రించు ("#undef %s_%s" %
				(what_short[i], పొట్టి[i][v]))
	ముద్రించు ()
	ముద్రించు ('#endif')
లేకపోతే:
	ముద్రించు ("#[inline]")
	ముద్రించు ("pub(crate) fn get_categories (u: u32) -> (u8, u8)")
	ముద్రించు ("{")
	ముద్రించు ("  let v = %s[_hb_indic_get_categories_index (u as usize) as usize];" % indic_విలువలు)
	ముద్రించు ("  ((v & 0xFF) as u8, (v >> 8) as u8)")
	ముద్రించు ("}")
ముద్రించు ()
ముద్రించు ("/* == End of generated table == */")
