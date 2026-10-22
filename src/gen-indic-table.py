#!/usr/bin/env python3

"""usage: ./gen-indic-table.py [--rust] IndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ഇറക്കുമതി sys

ഇറക്കുമതി packTab

എങ്കിൽ len(sys.argv) > 1 ഒപ്പം sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
അല്ലെങ്കിൽ:
	language = packTab.languages["c"]

എങ്കിൽ len (sys.argv) != 4:
	sys.exit (__doc__)

ALLOWED_SINGLES = [0x00A0, 0x25CC]
ALLOWED_BLOCKS = [
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

files = [open (x, encoding='utf-8') വേണ്ടി x ഇൽ sys.argv[1:]]

headers = [[f.readline () വേണ്ടി i ഇൽ range (2)] വേണ്ടി f ഇൽ files]

unicode_data = [{} വേണ്ടി _ ഇൽ files]
വേണ്ടി i, f ഇൽ enumerate (files):
	വേണ്ടി വരി ഇൽ f:

		j = വരി.find ('#')
		എങ്കിൽ j >= 0:
			വരി = വരി[:j]

		fields = [x.strip () വേണ്ടി x ഇൽ വരി.split (';')]
		എങ്കിൽ len (fields) == 1:
			തുടരുക

		uu = fields[0].split ('..')
		start = പൂർണ്ണസംഖ്യ (uu[0], 16)
		എങ്കിൽ len (uu) == 1:
			end = start
		അല്ലെങ്കിൽ:
			end = പൂർണ്ണസംഖ്യ (uu[1], 16)

		t = fields[1]

		വേണ്ടി u ഇൽ range (start, end + 1):
			unicode_data[i][u] = t

# Merge data into one dict:
defaults = ('Other', 'Not_Applicable', 'No_Block')
combined = {}
വേണ്ടി i,d ഇൽ enumerate (unicode_data):
	വേണ്ടി u,v ഇൽ d.ഇനങ്ങൾ ():
		എങ്കിൽ i == 2 ഒപ്പം അല്ല u ഇൽ combined:
			തുടരുക
		എങ്കിൽ അല്ല u ഇൽ combined:
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k:v വേണ്ടി k,v ഇൽ combined.ഇനങ്ങൾ() എങ്കിൽ k ഇൽ ALLOWED_SINGLES അഥവാ v[2] ഇൽ ALLOWED_BLOCKS}


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

def matra_pos_ഇടത്(u, block):
  തിരികെ "PRE_M"
def matra_pos_വലത്(u, block):
  എങ്കിൽ block == 'Devanagari':	തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Bengali':	തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Gurmukhi':	തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Gujarati':	തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Oriya':		തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Tamil':		തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Telugu':		തിരികെ  'BEFORE_SUB' എങ്കിൽ u <= 0x0C42 അല്ലെങ്കിൽ 'AFTER_SUB'
  എങ്കിൽ block == 'Kannada':	തിരികെ  'BEFORE_SUB' എങ്കിൽ u < 0x0CC3 അഥവാ u > 0x0CD6 അല്ലെങ്കിൽ 'AFTER_SUB'
  എങ്കിൽ block == 'Malayalam':	തിരികെ  'AFTER_POST'
  തിരികെ 'AFTER_SUB'
def matra_pos_മുകൾ(u, block):
  # BENG and MLYM don't have top matras.
  എങ്കിൽ block == 'Devanagari':	തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Gurmukhi':	തിരികെ  'AFTER_POST' # Deviate from spec
  എങ്കിൽ block == 'Gujarati':	തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Oriya':		തിരികെ  'AFTER_MAIN'
  എങ്കിൽ block == 'Tamil':		തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Telugu':		തിരികെ  'BEFORE_SUB'
  എങ്കിൽ block == 'Kannada':	തിരികെ  'BEFORE_SUB'
  തിരികെ 'AFTER_SUB'
def matra_pos_bottom(u, block):
  എങ്കിൽ block == 'Devanagari':	തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Bengali':	തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Gurmukhi':	തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Gujarati':	തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Oriya':		തിരികെ  'AFTER_SUB'
  എങ്കിൽ block == 'Tamil':		തിരികെ  'AFTER_POST'
  എങ്കിൽ block == 'Telugu':		തിരികെ  'BEFORE_SUB'
  എങ്കിൽ block == 'Kannada':	തിരികെ  'BEFORE_SUB'
  എങ്കിൽ block == 'Malayalam':	തിരികെ  'AFTER_POST'
  തിരികെ "AFTER_SUB"
def indic_matra_position(u, pos, block): # Reposition matra
  എങ്കിൽ pos == 'PRE_C':	തിരികെ matra_pos_ഇടത്(u, block)
  എങ്കിൽ pos == 'POST_C':	തിരികെ matra_pos_വലത്(u, block)
  എങ്കിൽ pos == 'ABOVE_C':	തിരികെ matra_pos_മുകൾ(u, block)
  എങ്കിൽ pos == 'BELOW_C':	തിരികെ matra_pos_bottom(u, block)
  പ്രതിജ്ഞാ (അസത്യം)

def position_to_category(pos):
  എങ്കിൽ pos == 'PRE_C':	തിരികെ 'VPre'
  എങ്കിൽ pos == 'ABOVE_C':	തിരികെ 'VAbv'
  എങ്കിൽ pos == 'BELOW_C':	തിരികെ 'VBlw'
  എങ്കിൽ pos == 'POST_C':	തിരികെ 'VPst'
  പ്രതിജ്ഞാ(അസത്യം)


defaults = (category_map[defaults[0]], position_map[defaults[1]], defaults[2])

indic_data = {}
വേണ്ടി k, (cat, pos, block) ഇൽ combined.ഇനങ്ങൾ():
  cat = category_map[cat]
  എങ്കിൽ cat == 'SM' ഒപ്പം pos == 'Not_Applicable':
    cat = 'SMPst'
  pos = position_map[pos]
  indic_data[k] = (cat, pos, block)

വേണ്ടി k,new_cat ഇൽ category_overrides.ഇനങ്ങൾ():
  (cat, pos, _) = indic_data.get(k, defaults)
  indic_data[k] = (new_cat, pos, unicode_data[2][k])

# We only expect position for certain types
positioned_categories = ('CM', 'SM', 'RS', 'H', 'M', 'MPst')
വേണ്ടി k, (cat, pos, block) ഇൽ indic_data.ഇനങ്ങൾ():
  എങ്കിൽ cat അല്ല ഇൽ positioned_categories:
    pos = 'END'
    indic_data[k] = (cat, pos, block)

# Position overrides are more complicated

# Keep in sync with CONSONANT_FLAGS in the shaper
consonant_categories = ('C', 'CS', 'Ra','CM', 'V', 'PLACEHOLDER', 'DOTTEDCIRCLE')
matra_categories = ('M', 'MPst')
smvd_categories = ('SM', 'SMPst', 'VD', 'A', 'Symbol')
വേണ്ടി k, (cat, pos, block) ഇൽ indic_data.ഇനങ്ങൾ():
  എങ്കിൽ cat ഇൽ consonant_categories:
    pos = 'BASE_C'
  അല്ലെങ്കിലെങ്കിൽ cat ഇൽ matra_categories:
    എങ്കിൽ block.startswith('Khmer') അഥവാ block.startswith('Myanmar'):
      cat = position_to_category(pos)
    അല്ലെങ്കിൽ:
      pos = indic_matra_position(k, pos, block)
  അല്ലെങ്കിലെങ്കിൽ cat ഇൽ smvd_categories:
    pos = 'SMVD';
  indic_data[k] = (cat, pos, block)

വേണ്ടി k,new_pos ഇൽ position_overrides.ഇനങ്ങൾ():
  (cat, pos, _) = indic_data.get(k, defaults)
  indic_data[k] = (cat, new_pos, unicode_data[2][k])


മൂല്യങ്ങൾ = [{_: 1} വേണ്ടി _ ഇൽ defaults]
വേണ്ടി vv ഇൽ indic_data.മൂല്യങ്ങൾ():
  വേണ്ടി i,v ഇൽ enumerate(vv):
    മൂല്യങ്ങൾ[i][v] = മൂല്യങ്ങൾ[i].get (v, 0) + 1




# Move the outliers NO-BREAK SPACE and DOTTED CIRCLE out
singles = {}
വേണ്ടി u ഇൽ ALLOWED_SINGLES:
	singles[u] = indic_data[u]
	del indic_data[u]

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-indic-table.py %sIndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt" %
		("--rust " എങ്കിൽ language.name == "rust" അല്ലെങ്കിൽ ""))
print (" *")
print (" * on files with these headers:")
print (" *")
വേണ്ടി h ഇൽ headers:
	വേണ്ടി l ഇൽ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
എങ്കിൽ language.name == "c":
	print ('#include "hb.hh"')
	print ()
	print ('#ifndef HB_NO_OT_SHAPE')
	print ()
	print ('#include "hb-ot-shaper-indic.hh"')
	print ()
	print ('#pragma GCC diagnostic push')
	print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
	print ()

	വേണ്ടി shaper ഇൽ categories:
	  print ('#include "hb-ot-shaper-%s-machine.hh"' % shaper)
	print ()
	done = {}
	വേണ്ടി shaper, shaper_cats ഇൽ categories.ഇനങ്ങൾ():
	  print ('/* %s */' % shaper)
	  വേണ്ടി cat ഇൽ shaper_cats:
	    v = shaper[0].upper()
	    എങ്കിൽ cat അല്ല ഇൽ done:
	      print ("#define OT_%s %s_Cat(%s)" % (cat, v, cat))
	      done[cat] = v
	    അല്ലെങ്കിൽ:
	      print ('static_assert (OT_%s == %s_Cat(%s), "");' % (cat, v, cat))
	print ()
അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
	print ("#![allow(unused_parens)]")
	print ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
	print ("#![allow(non_camel_case_types)]")
	print ("#![allow(unused_imports)]")
	print ()
	print ('use super::ot_shaper_indic::ot_category_t::*;')
	print ('use super::ot_shaper_indic::ot_position_t::*;')
	print ()
അല്ലെങ്കിൽ:
	പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name

# Shorten values
short = [{
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

വേണ്ടി i ഇൽ range (2):
	വേണ്ടി v,s ഇൽ short[i].ഇനങ്ങൾ ():
		all_shorts[i][s] = v

what = ["OT", "POS"]
what_short = ["_OT", "_POS"]
cat_defs = []
വേണ്ടി i ഇൽ range (2):
	vv = sorted (മൂല്യങ്ങൾ[i].താക്കോലുകൾ ())
	വേണ്ടി v ഇൽ vv:
		v_no_and = v.replace ('_And_', '_')
		എങ്കിൽ v ഇൽ short[i]:
			s = short[i][v]
		അല്ലെങ്കിൽ:
			s = ''.join ([c വേണ്ടി c ഇൽ v_no_and എങ്കിൽ ord ('A') <= ord (c) <= ord ('Z')])
			എങ്കിൽ s ഇൽ all_shorts[i]:
				ഉയർത്തുക Exception ("Duplicate short value alias", v, all_shorts[i][s])
			all_shorts[i][s] = v
			short[i][v] = s
		cat_defs.append ((what_short[i] + '_' + s, what[i] + '_' + (v.upper () എങ്കിൽ i അല്ലെങ്കിൽ v), str (മൂല്യങ്ങൾ[i][v]), v))

maxlen_s = max ([len (c[0]) വേണ്ടി c ഇൽ cat_defs])
maxlen_l = max ([len (c[1]) വേണ്ടി c ഇൽ cat_defs])
maxlen_n = max ([len (c[2]) വേണ്ടി c ഇൽ cat_defs])
വേണ്ടി s ഇൽ what_short:
	print ()
	വേണ്ടി c ഇൽ [c വേണ്ടി c ഇൽ cat_defs എങ്കിൽ s ഇൽ c[0]]:
		എങ്കിൽ language.name == "c":
			print ("#define %s %s /* %s chars; %s */" %
				(c[0].ljust (maxlen_s), c[1].ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
		അല്ലെങ്കിൽ:
			print ("use %s as %s /* %s chars; %s */" %
				(c[1].ljust (maxlen_s), (c[0] + ";").ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
print ()
എങ്കിൽ language.name == "c":
	print ('#pragma GCC diagnostic pop')
	print ()
	print ("#define INDIC_COMBINE_CATEGORIES(S,M) ((S) | ((M) << 8))")
	print ()
	print ("#define _(S,M) INDIC_COMBINE_CATEGORIES (%s_##S, %s_##M)" % tuple(what_short))
	print ()
	print ()
അല്ലെങ്കിൽ:
	print ("const fn indic_combine_categories (s: u8, m: u8) -> u16")
	print ("{")
	print ("  (s as u16) | ((m as u16) << 8)")
	print ("}")
	print ()
	print ()

എങ്കിൽ language.name == "c":
	default_മൂല്യം = "_(%s,%s)" % (short[0][defaults[0]], short[1][defaults[1]])
	pack_data = {
		u: "_(%s,%s)" % (short[0][d[0]], short[1][d[1]])
		വേണ്ടി u, d ഇൽ indic_data.ഇനങ്ങൾ ()
	}
	pack_data.update ({
		u: "_(%s,%s)" % (short[0][d[0]], short[1][d[1]])
		വേണ്ടി u, d ഇൽ singles.ഇനങ്ങൾ ()
	})
അല്ലെങ്കിൽ:
	default_മൂല്യം = "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][defaults[0]], short[1][defaults[1]])
	pack_data = {
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][d[0]], short[1][d[1]])
		വേണ്ടി u, d ഇൽ indic_data.ഇനങ്ങൾ ()
	}
	pack_data.update ({
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][d[0]], short[1][d[1]])
		വേണ്ടി u, d ഇൽ singles.ഇനങ്ങൾ ()
	})

unique_മൂല്യങ്ങൾ = sorted (set (pack_data.മൂല്യങ്ങൾ ()) | {default_മൂല്യം})
mapping = {v: i വേണ്ടി i, v ഇൽ enumerate (unique_മൂല്യങ്ങൾ)}

code = packTab.Code ('_hb_indic')
മൂല്യം_type = 'uint16_t' എങ്കിൽ language.name == "c" അല്ലെങ്കിൽ language.type_name ("u16")
indic_മൂല്യങ്ങൾ, _ = code.addArray (മൂല്യം_type, 'values', unique_മൂല്യങ്ങൾ)
sol = packTab.pack_table (pack_data, സ്ഥിരസ്ഥിതി=default_മൂല്യം, mapping=mapping, compression=9)
sol.genCode (code, 'get_categories_index', language=language, സ്വകാര്യ=language.name == "c")
code.print_code (language=language, സ്വകാര്യ=language.name == "c")
print ()
എങ്കിൽ language.name == "c":
	print ("uint16_t")
	print ("hb_indic_get_categories (hb_codepoint_t u)")
	print ("{")
	print ("  return %s[_hb_indic_get_categories_index (u)];" % indic_മൂല്യങ്ങൾ)
	print ("}")
	print ()
	print ("#undef _")
	print ("#undef INDIC_COMBINE_CATEGORIES")
	വേണ്ടി i ഇൽ range (2):
		print ()
		vv = sorted (മൂല്യങ്ങൾ[i].താക്കോലുകൾ ())
		വേണ്ടി v ഇൽ vv:
			print ("#undef %s_%s" %
				(what_short[i], short[i][v]))
	print ()
	print ('#endif')
അല്ലെങ്കിൽ:
	print ("#[inline]")
	print ("pub(crate) fn get_categories (u: u32) -> (u8, u8)")
	print ("{")
	print ("  let v = %s[_hb_indic_get_categories_index (u as usize) as usize];" % indic_മൂല്യങ്ങൾ)
	print ("  ((v & 0xFF) as u8, (v >> 8) as u8)")
	print ("}")
print ()
print ("/* == End of generated table == */")
