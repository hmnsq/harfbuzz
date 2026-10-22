#!/usr/bin/env python3

"""usage: ./gen-indic-table.py [--rust] IndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

আনযন sys

আনযন packTab

যদি len(sys.argv) > 1 চ sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
অন্যথা:
	language = packTab.languages["c"]

যদি len (sys.argv) != 4:
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

files = [open (x, encoding='utf-8') প্রতি x অন্তঃ sys.argv[1:]]

headers = [[f.readline () প্রতি i অন্তঃ range (2)] প্রতি f অন্তঃ files]

unicode_data = [{} প্রতি _ অন্তঃ files]
প্রতি i, f অন্তঃ enumerate (files):
	প্রতি সারি অন্তঃ f:

		j = সারি.find ('#')
		যদি j >= 0:
			সারি = সারি[:j]

		fields = [x.strip () প্রতি x অন্তঃ সারি.split (';')]
		যদি len (fields) == 1:
			অনুবর্ত

		uu = fields[0].split ('..')
		start = পূর্ণাঙ্ক (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণাঙ্ক (uu[1], 16)

		t = fields[1]

		প্রতি u অন্তঃ range (start, end + 1):
			unicode_data[i][u] = t

# Merge data into one dict:
defaults = ('Other', 'Not_Applicable', 'No_Block')
combined = {}
প্রতি i,d অন্তঃ enumerate (unicode_data):
	প্রতি u,v অন্তঃ d.বস্তুসমূহ ():
		যদি i == 2 চ ন u অন্তঃ combined:
			অনুবর্ত
		যদি ন u অন্তঃ combined:
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k:v প্রতি k,v অন্তঃ combined.বস্তুসমূহ() যদি k অন্তঃ ALLOWED_SINGLES বা v[2] অন্তঃ ALLOWED_BLOCKS}


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

def matra_pos_বাম(u, block):
  প্রত্যাগম "PRE_M"
def matra_pos_ডান(u, block):
  যদি block == 'Devanagari':	প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Bengali':	প্রত্যাগম  'AFTER_POST'
  যদি block == 'Gurmukhi':	প্রত্যাগম  'AFTER_POST'
  যদি block == 'Gujarati':	প্রত্যাগম  'AFTER_POST'
  যদি block == 'Oriya':		প্রত্যাগম  'AFTER_POST'
  যদি block == 'Tamil':		প্রত্যাগম  'AFTER_POST'
  যদি block == 'Telugu':		প্রত্যাগম  'BEFORE_SUB' যদি u <= 0x0C42 অন্যথা 'AFTER_SUB'
  যদি block == 'Kannada':	প্রত্যাগম  'BEFORE_SUB' যদি u < 0x0CC3 বা u > 0x0CD6 অন্যথা 'AFTER_SUB'
  যদি block == 'Malayalam':	প্রত্যাগম  'AFTER_POST'
  প্রত্যাগম 'AFTER_SUB'
def matra_pos_শীর্ষ(u, block):
  # BENG and MLYM don't have top matras.
  যদি block == 'Devanagari':	প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Gurmukhi':	প্রত্যাগম  'AFTER_POST' # Deviate from spec
  যদি block == 'Gujarati':	প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Oriya':		প্রত্যাগম  'AFTER_MAIN'
  যদি block == 'Tamil':		প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Telugu':		প্রত্যাগম  'BEFORE_SUB'
  যদি block == 'Kannada':	প্রত্যাগম  'BEFORE_SUB'
  প্রত্যাগম 'AFTER_SUB'
def matra_pos_bottom(u, block):
  যদি block == 'Devanagari':	প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Bengali':	প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Gurmukhi':	প্রত্যাগম  'AFTER_POST'
  যদি block == 'Gujarati':	প্রত্যাগম  'AFTER_POST'
  যদি block == 'Oriya':		প্রত্যাগম  'AFTER_SUB'
  যদি block == 'Tamil':		প্রত্যাগম  'AFTER_POST'
  যদি block == 'Telugu':		প্রত্যাগম  'BEFORE_SUB'
  যদি block == 'Kannada':	প্রত্যাগম  'BEFORE_SUB'
  যদি block == 'Malayalam':	প্রত্যাগম  'AFTER_POST'
  প্রত্যাগম "AFTER_SUB"
def indic_matra_position(u, pos, block): # Reposition matra
  যদি pos == 'PRE_C':	প্রত্যাগম matra_pos_বাম(u, block)
  যদি pos == 'POST_C':	প্রত্যাগম matra_pos_ডান(u, block)
  যদি pos == 'ABOVE_C':	প্রত্যাগম matra_pos_শীর্ষ(u, block)
  যদি pos == 'BELOW_C':	প্রত্যাগম matra_pos_bottom(u, block)
  প্রতিজ্ঞা (অসত্য)

def position_to_category(pos):
  যদি pos == 'PRE_C':	প্রত্যাগম 'VPre'
  যদি pos == 'ABOVE_C':	প্রত্যাগম 'VAbv'
  যদি pos == 'BELOW_C':	প্রত্যাগম 'VBlw'
  যদি pos == 'POST_C':	প্রত্যাগম 'VPst'
  প্রতিজ্ঞা(অসত্য)


defaults = (category_map[defaults[0]], position_map[defaults[1]], defaults[2])

indic_data = {}
প্রতি k, (cat, pos, block) অন্তঃ combined.বস্তুসমূহ():
  cat = category_map[cat]
  যদি cat == 'SM' চ pos == 'Not_Applicable':
    cat = 'SMPst'
  pos = position_map[pos]
  indic_data[k] = (cat, pos, block)

প্রতি k,new_cat অন্তঃ category_overrides.বস্তুসমূহ():
  (cat, pos, _) = indic_data.get(k, defaults)
  indic_data[k] = (new_cat, pos, unicode_data[2][k])

# We only expect position for certain types
positioned_categories = ('CM', 'SM', 'RS', 'H', 'M', 'MPst')
প্রতি k, (cat, pos, block) অন্তঃ indic_data.বস্তুসমূহ():
  যদি cat ন অন্তঃ positioned_categories:
    pos = 'END'
    indic_data[k] = (cat, pos, block)

# Position overrides are more complicated

# Keep in sync with CONSONANT_FLAGS in the shaper
consonant_categories = ('C', 'CS', 'Ra','CM', 'V', 'PLACEHOLDER', 'DOTTEDCIRCLE')
matra_categories = ('M', 'MPst')
smvd_categories = ('SM', 'SMPst', 'VD', 'A', 'Symbol')
প্রতি k, (cat, pos, block) অন্তঃ indic_data.বস্তুসমূহ():
  যদি cat অন্তঃ consonant_categories:
    pos = 'BASE_C'
  নোচেদ্যদি cat অন্তঃ matra_categories:
    যদি block.startswith('Khmer') বা block.startswith('Myanmar'):
      cat = position_to_category(pos)
    অন্যথা:
      pos = indic_matra_position(k, pos, block)
  নোচেদ্যদি cat অন্তঃ smvd_categories:
    pos = 'SMVD';
  indic_data[k] = (cat, pos, block)

প্রতি k,new_pos অন্তঃ position_overrides.বস্তুসমূহ():
  (cat, pos, _) = indic_data.get(k, defaults)
  indic_data[k] = (cat, new_pos, unicode_data[2][k])


মানসমূহ = [{_: 1} প্রতি _ অন্তঃ defaults]
প্রতি vv অন্তঃ indic_data.মানসমূহ():
  প্রতি i,v অন্তঃ enumerate(vv):
    মানসমূহ[i][v] = মানসমূহ[i].get (v, 0) + 1




# Move the outliers NO-BREAK SPACE and DOTTED CIRCLE out
singles = {}
প্রতি u অন্তঃ ALLOWED_SINGLES:
	singles[u] = indic_data[u]
	del indic_data[u]

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-indic-table.py %sIndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt" %
		("--rust " যদি language.name == "rust" অন্যথা ""))
print (" *")
print (" * on files with these headers:")
print (" *")
প্রতি h অন্তঃ headers:
	প্রতি l অন্তঃ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
যদি language.name == "c":
	print ('#include "hb.hh"')
	print ()
	print ('#ifndef HB_NO_OT_SHAPE')
	print ()
	print ('#include "hb-ot-shaper-indic.hh"')
	print ()
	print ('#pragma GCC diagnostic push')
	print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
	print ()

	প্রতি shaper অন্তঃ categories:
	  print ('#include "hb-ot-shaper-%s-machine.hh"' % shaper)
	print ()
	done = {}
	প্রতি shaper, shaper_cats অন্তঃ categories.বস্তুসমূহ():
	  print ('/* %s */' % shaper)
	  প্রতি cat অন্তঃ shaper_cats:
	    v = shaper[0].upper()
	    যদি cat ন অন্তঃ done:
	      print ("#define OT_%s %s_Cat(%s)" % (cat, v, cat))
	      done[cat] = v
	    অন্যথা:
	      print ('static_assert (OT_%s == %s_Cat(%s), "");' % (cat, v, cat))
	print ()
নোচেদ্যদি language.name == "rust":
	print ("#![allow(unused_parens)]")
	print ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
	print ("#![allow(non_camel_case_types)]")
	print ("#![allow(unused_imports)]")
	print ()
	print ('use super::ot_shaper_indic::ot_category_t::*;')
	print ('use super::ot_shaper_indic::ot_position_t::*;')
	print ()
অন্যথা:
	প্রতিজ্ঞা অসত্য, "Unknown language: %s" % language.name

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

প্রতি i অন্তঃ range (2):
	প্রতি v,s অন্তঃ short[i].বস্তুসমূহ ():
		all_shorts[i][s] = v

what = ["OT", "POS"]
what_short = ["_OT", "_POS"]
cat_defs = []
প্রতি i অন্তঃ range (2):
	vv = sorted (মানসমূহ[i].চাবিগুলি ())
	প্রতি v অন্তঃ vv:
		v_no_and = v.replace ('_And_', '_')
		যদি v অন্তঃ short[i]:
			s = short[i][v]
		অন্যথা:
			s = ''.join ([c প্রতি c অন্তঃ v_no_and যদি ord ('A') <= ord (c) <= ord ('Z')])
			যদি s অন্তঃ all_shorts[i]:
				উত্থাপয Exception ("Duplicate short value alias", v, all_shorts[i][s])
			all_shorts[i][s] = v
			short[i][v] = s
		cat_defs.append ((what_short[i] + '_' + s, what[i] + '_' + (v.upper () যদি i অন্যথা v), str (মানসমূহ[i][v]), v))

maxlen_s = max ([len (c[0]) প্রতি c অন্তঃ cat_defs])
maxlen_l = max ([len (c[1]) প্রতি c অন্তঃ cat_defs])
maxlen_n = max ([len (c[2]) প্রতি c অন্তঃ cat_defs])
প্রতি s অন্তঃ what_short:
	print ()
	প্রতি c অন্তঃ [c প্রতি c অন্তঃ cat_defs যদি s অন্তঃ c[0]]:
		যদি language.name == "c":
			print ("#define %s %s /* %s chars; %s */" %
				(c[0].ljust (maxlen_s), c[1].ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
		অন্যথা:
			print ("use %s as %s /* %s chars; %s */" %
				(c[1].ljust (maxlen_s), (c[0] + ";").ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
print ()
যদি language.name == "c":
	print ('#pragma GCC diagnostic pop')
	print ()
	print ("#define INDIC_COMBINE_CATEGORIES(S,M) ((S) | ((M) << 8))")
	print ()
	print ("#define _(S,M) INDIC_COMBINE_CATEGORIES (%s_##S, %s_##M)" % tuple(what_short))
	print ()
	print ()
অন্যথা:
	print ("const fn indic_combine_categories (s: u8, m: u8) -> u16")
	print ("{")
	print ("  (s as u16) | ((m as u16) << 8)")
	print ("}")
	print ()
	print ()

যদি language.name == "c":
	default_মান = "_(%s,%s)" % (short[0][defaults[0]], short[1][defaults[1]])
	pack_data = {
		u: "_(%s,%s)" % (short[0][d[0]], short[1][d[1]])
		প্রতি u, d অন্তঃ indic_data.বস্তুসমূহ ()
	}
	pack_data.update ({
		u: "_(%s,%s)" % (short[0][d[0]], short[1][d[1]])
		প্রতি u, d অন্তঃ singles.বস্তুসমূহ ()
	})
অন্যথা:
	default_মান = "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][defaults[0]], short[1][defaults[1]])
	pack_data = {
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][d[0]], short[1][d[1]])
		প্রতি u, d অন্তঃ indic_data.বস্তুসমূহ ()
	}
	pack_data.update ({
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (short[0][d[0]], short[1][d[1]])
		প্রতি u, d অন্তঃ singles.বস্তুসমূহ ()
	})

unique_মানসমূহ = sorted (set (pack_data.মানসমূহ ()) | {default_মান})
mapping = {v: i প্রতি i, v অন্তঃ enumerate (unique_মানসমূহ)}

code = packTab.Code ('_hb_indic')
মান_type = 'uint16_t' যদি language.name == "c" অন্যথা language.type_name ("u16")
indic_মানসমূহ, _ = code.addArray (মান_type, 'values', unique_মানসমূহ)
sol = packTab.pack_table (pack_data, স্বভাব=default_মান, mapping=mapping, compression=9)
sol.genCode (code, 'get_categories_index', language=language, গুপ্ত=language.name == "c")
code.print_code (language=language, গুপ্ত=language.name == "c")
print ()
যদি language.name == "c":
	print ("uint16_t")
	print ("hb_indic_get_categories (hb_codepoint_t u)")
	print ("{")
	print ("  return %s[_hb_indic_get_categories_index (u)];" % indic_মানসমূহ)
	print ("}")
	print ()
	print ("#undef _")
	print ("#undef INDIC_COMBINE_CATEGORIES")
	প্রতি i অন্তঃ range (2):
		print ()
		vv = sorted (মানসমূহ[i].চাবিগুলি ())
		প্রতি v অন্তঃ vv:
			print ("#undef %s_%s" %
				(what_short[i], short[i][v]))
	print ()
	print ('#endif')
অন্যথা:
	print ("#[inline]")
	print ("pub(crate) fn get_categories (u: u32) -> (u8, u8)")
	print ("{")
	print ("  let v = %s[_hb_indic_get_categories_index (u as usize) as usize];" % indic_মানসমূহ)
	print ("  ((v & 0xFF) as u8, (v >> 8) as u8)")
	print ("}")
print ()
print ("/* == End of generated table == */")
