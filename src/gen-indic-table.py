#!/usr/bin/env python3

"""usage: ./gen-indic-table.py [--rust] IndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ਆਯਾਤ sys

ਆਯਾਤ packTab

ਜੇ ਲੰਬਾਈ(sys.argv) > 1 ਅਤੇ sys.argv[1] == "--rust":
	ਹਟਾਓ sys.argv[1]
	language = packTab.languages["rust"]
ਨਹੀਂਤਾਂ:
	language = packTab.languages["c"]

ਜੇ ਲੰਬਾਈ (sys.argv) != 4:
	sys.exit (__doc__)

ALLOWED_SINGLES = [0x00A0, 0x25CC]
ALLOWED_ਖੰਡਾਂ = [
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

files = [open (x, encoding='utf-8') ਲਈ x ਵਿੱਚ sys.argv[1:]]

ਸਿਰਲੇਖਾਂ = [[f.readline () ਲਈ i ਵਿੱਚ ਸੀਮਾ (2)] ਲਈ f ਵਿੱਚ files]

unicode_ਅੰਕੜੇ = [{} ਲਈ _ ਵਿੱਚ files]
ਲਈ i, f ਵਿੱਚ enumerate (files):
	ਲਈ ਸਤਰ ਵਿੱਚ f:

		j = ਸਤਰ.ਲੱਭੋ ('#')
		ਜੇ j >= 0:
			ਸਤਰ = ਸਤਰ[:j]

		fields = [x.strip () ਲਈ x ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')]
		ਜੇ ਲੰਬਾਈ (fields) == 1:
			ਜਾਰੀ

		uu = fields[0].ਵੰਡੋ ('..')
		ਸ਼ੁਰੂ = ਪੂਰਨ (uu[0], 16)
		ਜੇ ਲੰਬਾਈ (uu) == 1:
			ਸਿਰਾ = ਸ਼ੁਰੂ
		ਨਹੀਂਤਾਂ:
			ਸਿਰਾ = ਪੂਰਨ (uu[1], 16)

		t = fields[1]

		ਲਈ u ਵਿੱਚ ਸੀਮਾ (ਸ਼ੁਰੂ, ਸਿਰਾ + 1):
			unicode_ਅੰਕੜੇ[i][u] = t

# Merge data into one dict:
defaults = ('Other', 'Not_Applicable', 'No_Block')
combined = {}
ਲਈ i,d ਵਿੱਚ enumerate (unicode_ਅੰਕੜੇ):
	ਲਈ u,v ਵਿੱਚ d.ਚੀਜ਼ਾਂ ():
		ਜੇ i == 2 ਅਤੇ ਨਹੀਂ u ਵਿੱਚ combined:
			ਜਾਰੀ
		ਜੇ ਨਹੀਂ u ਵਿੱਚ combined:
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k:v ਲਈ k,v ਵਿੱਚ combined.ਚੀਜ਼ਾਂ() ਜੇ k ਵਿੱਚ ALLOWED_SINGLES ਜਾਂ v[2] ਵਿੱਚ ALLOWED_ਖੰਡਾਂ}


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

ਪਰਿਭਾਸ਼ਾ matra_pos_ਖੱਬਾ(u, ਖੰਡ):
  ਵਾਪਸ "PRE_M"
ਪਰਿਭਾਸ਼ਾ matra_pos_ਸੱਜਾ(u, ਖੰਡ):
  ਜੇ ਖੰਡ == 'Devanagari':	ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Bengali':	ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Gurmukhi':	ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Gujarati':	ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Oriya':		ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Tamil':		ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Telugu':		ਵਾਪਸ  'BEFORE_SUB' ਜੇ u <= 0x0C42 ਨਹੀਂਤਾਂ 'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Kannada':	ਵਾਪਸ  'BEFORE_SUB' ਜੇ u < 0x0CC3 ਜਾਂ u > 0x0CD6 ਨਹੀਂਤਾਂ 'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Malayalam':	ਵਾਪਸ  'AFTER_POST'
  ਵਾਪਸ 'AFTER_SUB'
ਪਰਿਭਾਸ਼ਾ matra_pos_ਸਿਖਰ(u, ਖੰਡ):
  # BENG and MLYM don't have top matras.
  ਜੇ ਖੰਡ == 'Devanagari':	ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Gurmukhi':	ਵਾਪਸ  'AFTER_POST' # Deviate from spec
  ਜੇ ਖੰਡ == 'Gujarati':	ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Oriya':		ਵਾਪਸ  'AFTER_MAIN'
  ਜੇ ਖੰਡ == 'Tamil':		ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Telugu':		ਵਾਪਸ  'BEFORE_SUB'
  ਜੇ ਖੰਡ == 'Kannada':	ਵਾਪਸ  'BEFORE_SUB'
  ਵਾਪਸ 'AFTER_SUB'
ਪਰਿਭਾਸ਼ਾ matra_pos_bottom(u, ਖੰਡ):
  ਜੇ ਖੰਡ == 'Devanagari':	ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Bengali':	ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Gurmukhi':	ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Gujarati':	ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Oriya':		ਵਾਪਸ  'AFTER_SUB'
  ਜੇ ਖੰਡ == 'Tamil':		ਵਾਪਸ  'AFTER_POST'
  ਜੇ ਖੰਡ == 'Telugu':		ਵਾਪਸ  'BEFORE_SUB'
  ਜੇ ਖੰਡ == 'Kannada':	ਵਾਪਸ  'BEFORE_SUB'
  ਜੇ ਖੰਡ == 'Malayalam':	ਵਾਪਸ  'AFTER_POST'
  ਵਾਪਸ "AFTER_SUB"
ਪਰਿਭਾਸ਼ਾ indic_matra_position(u, pos, ਖੰਡ): # Reposition matra
  ਜੇ pos == 'PRE_C':	ਵਾਪਸ matra_pos_ਖੱਬਾ(u, ਖੰਡ)
  ਜੇ pos == 'POST_C':	ਵਾਪਸ matra_pos_ਸੱਜਾ(u, ਖੰਡ)
  ਜੇ pos == 'ABOVE_C':	ਵਾਪਸ matra_pos_ਸਿਖਰ(u, ਖੰਡ)
  ਜੇ pos == 'BELOW_C':	ਵਾਪਸ matra_pos_bottom(u, ਖੰਡ)
  ਪ੍ਰਤਿਜ੍ਞਾ (ਝੂਠ)

ਪਰਿਭਾਸ਼ਾ position_ਮੰਜ਼ਿਲ_category(pos):
  ਜੇ pos == 'PRE_C':	ਵਾਪਸ 'VPre'
  ਜੇ pos == 'ABOVE_C':	ਵਾਪਸ 'VAbv'
  ਜੇ pos == 'BELOW_C':	ਵਾਪਸ 'VBlw'
  ਜੇ pos == 'POST_C':	ਵਾਪਸ 'VPst'
  ਪ੍ਰਤਿਜ੍ਞਾ(ਝੂਠ)


defaults = (category_map[defaults[0]], position_map[defaults[1]], defaults[2])

indic_ਅੰਕੜੇ = {}
ਲਈ k, (cat, pos, ਖੰਡ) ਵਿੱਚ combined.ਚੀਜ਼ਾਂ():
  cat = category_map[cat]
  ਜੇ cat == 'SM' ਅਤੇ pos == 'Not_Applicable':
    cat = 'SMPst'
  pos = position_map[pos]
  indic_ਅੰਕੜੇ[k] = (cat, pos, ਖੰਡ)

ਲਈ k,new_cat ਵਿੱਚ category_overrides.ਚੀਜ਼ਾਂ():
  (cat, pos, _) = indic_ਅੰਕੜੇ.get(k, defaults)
  indic_ਅੰਕੜੇ[k] = (new_cat, pos, unicode_ਅੰਕੜੇ[2][k])

# We only expect position for certain types
positioned_categories = ('CM', 'SM', 'RS', 'H', 'M', 'MPst')
ਲਈ k, (cat, pos, ਖੰਡ) ਵਿੱਚ indic_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ():
  ਜੇ cat ਨਹੀਂ ਵਿੱਚ positioned_categories:
    pos = 'END'
    indic_ਅੰਕੜੇ[k] = (cat, pos, ਖੰਡ)

# Position overrides are more complicated

# Keep in sync with CONSONANT_FLAGS in the shaper
consonant_categories = ('C', 'CS', 'Ra','CM', 'V', 'PLACEHOLDER', 'DOTTEDCIRCLE')
matra_categories = ('M', 'MPst')
smvd_categories = ('SM', 'SMPst', 'VD', 'A', 'Symbol')
ਲਈ k, (cat, pos, ਖੰਡ) ਵਿੱਚ indic_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ():
  ਜੇ cat ਵਿੱਚ consonant_categories:
    pos = 'BASE_C'
  ਜਾਂਜੇ cat ਵਿੱਚ matra_categories:
    ਜੇ ਖੰਡ.startswith('Khmer') ਜਾਂ ਖੰਡ.startswith('Myanmar'):
      cat = position_ਮੰਜ਼ਿਲ_category(pos)
    ਨਹੀਂਤਾਂ:
      pos = indic_matra_position(k, pos, ਖੰਡ)
  ਜਾਂਜੇ cat ਵਿੱਚ smvd_categories:
    pos = 'SMVD';
  indic_ਅੰਕੜੇ[k] = (cat, pos, ਖੰਡ)

ਲਈ k,new_pos ਵਿੱਚ position_overrides.ਚੀਜ਼ਾਂ():
  (cat, pos, _) = indic_ਅੰਕੜੇ.get(k, defaults)
  indic_ਅੰਕੜੇ[k] = (cat, new_pos, unicode_ਅੰਕੜੇ[2][k])


ਮੁੱਲਾਂ = [{_: 1} ਲਈ _ ਵਿੱਚ defaults]
ਲਈ vv ਵਿੱਚ indic_ਅੰਕੜੇ.ਮੁੱਲਾਂ():
  ਲਈ i,v ਵਿੱਚ enumerate(vv):
    ਮੁੱਲਾਂ[i][v] = ਮੁੱਲਾਂ[i].get (v, 0) + 1




# Move the outliers NO-BREAK SPACE and DOTTED CIRCLE out
singles = {}
ਲਈ u ਵਿੱਚ ALLOWED_SINGLES:
	singles[u] = indic_ਅੰਕੜੇ[u]
	ਹਟਾਓ indic_ਅੰਕੜੇ[u]

ਛਾਪੋ ("/* == Start of generated table == */")
ਛਾਪੋ ("/*")
ਛਾਪੋ (" * The following table is generated by running:")
ਛਾਪੋ (" *")
ਛਾਪੋ (" *   ./gen-indic-table.py %sIndicSyllabicCategory.txt IndicPositionalCategory.txt Blocks.txt" %
		("--rust " ਜੇ language.name == "rust" ਨਹੀਂਤਾਂ ""))
ਛਾਪੋ (" *")
ਛਾਪੋ (" * on files with these headers:")
ਛਾਪੋ (" *")
ਲਈ h ਵਿੱਚ ਸਿਰਲੇਖਾਂ:
	ਲਈ l ਵਿੱਚ h:
		ਛਾਪੋ (" * %s" % (l.strip()))
ਛਾਪੋ (" */")
ਛਾਪੋ ()
ਜੇ language.name == "c":
	ਛਾਪੋ ('#include "hb.hh"')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#ifndef HB_NO_OT_SHAPE')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#include "hb-ot-shaper-indic.hh"')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#pragma GCC diagnostic push')
	ਛਾਪੋ ('#pragma GCC diagnostic ignored "-Wunused-macros"')
	ਛਾਪੋ ()

	ਲਈ shaper ਵਿੱਚ categories:
	  ਛਾਪੋ ('#include "hb-ot-shaper-%s-machine.hh"' % shaper)
	ਛਾਪੋ ()
	ਪੂਰਾ = {}
	ਲਈ shaper, shaper_cats ਵਿੱਚ categories.ਚੀਜ਼ਾਂ():
	  ਛਾਪੋ ('/* %s */' % shaper)
	  ਲਈ cat ਵਿੱਚ shaper_cats:
	    v = shaper[0].ਉੱਤਲਾ()
	    ਜੇ cat ਨਹੀਂ ਵਿੱਚ ਪੂਰਾ:
	      ਛਾਪੋ ("#define OT_%s %s_Cat(%s)" % (cat, v, cat))
	      ਪੂਰਾ[cat] = v
	    ਨਹੀਂਤਾਂ:
	      ਛਾਪੋ ('static_assert (OT_%s == %s_Cat(%s), "");' % (cat, v, cat))
	ਛਾਪੋ ()
ਜਾਂਜੇ language.name == "rust":
	ਛਾਪੋ ("#![allow(unused_parens)]")
	ਛਾਪੋ ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
	ਛਾਪੋ ("#![allow(non_camel_case_types)]")
	ਛਾਪੋ ("#![allow(unused_imports)]")
	ਛਾਪੋ ()
	ਛਾਪੋ ('use super::ot_shaper_indic::ot_category_t::*;')
	ਛਾਪੋ ('use super::ot_shaper_indic::ot_position_t::*;')
	ਛਾਪੋ ()
ਨਹੀਂਤਾਂ:
	ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name

# Shorten values
ਛੋਟਾ = [{
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

ਲਈ i ਵਿੱਚ ਸੀਮਾ (2):
	ਲਈ v,s ਵਿੱਚ ਛੋਟਾ[i].ਚੀਜ਼ਾਂ ():
		all_shorts[i][s] = v

what = ["OT", "POS"]
what_short = ["_OT", "_POS"]
cat_defs = []
ਲਈ i ਵਿੱਚ ਸੀਮਾ (2):
	vv = sorted (ਮੁੱਲਾਂ[i].ਕੁੰਜੀਆਂ ())
	ਲਈ v ਵਿੱਚ vv:
		v_no_and = v.replace ('_And_', '_')
		ਜੇ v ਵਿੱਚ ਛੋਟਾ[i]:
			s = ਛੋਟਾ[i][v]
		ਨਹੀਂਤਾਂ:
			s = ''.join ([c ਲਈ c ਵਿੱਚ v_no_and ਜੇ ord ('A') <= ord (c) <= ord ('Z')])
			ਜੇ s ਵਿੱਚ all_shorts[i]:
				ਉਠਾਓ Exception ("Duplicate short value alias", v, all_shorts[i][s])
			all_shorts[i][s] = v
			ਛੋਟਾ[i][v] = s
		cat_defs.append ((what_short[i] + '_' + s, what[i] + '_' + (v.ਉੱਤਲਾ () ਜੇ i ਨਹੀਂਤਾਂ v), str (ਮੁੱਲਾਂ[i][v]), v))

maxlen_s = max ([ਲੰਬਾਈ (c[0]) ਲਈ c ਵਿੱਚ cat_defs])
maxlen_l = max ([ਲੰਬਾਈ (c[1]) ਲਈ c ਵਿੱਚ cat_defs])
maxlen_n = max ([ਲੰਬਾਈ (c[2]) ਲਈ c ਵਿੱਚ cat_defs])
ਲਈ s ਵਿੱਚ what_short:
	ਛਾਪੋ ()
	ਲਈ c ਵਿੱਚ [c ਲਈ c ਵਿੱਚ cat_defs ਜੇ s ਵਿੱਚ c[0]]:
		ਜੇ language.name == "c":
			ਛਾਪੋ ("#define %s %s /* %s chars; %s */" %
				(c[0].ljust (maxlen_s), c[1].ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
		ਨਹੀਂਤਾਂ:
			ਛਾਪੋ ("use %s as %s /* %s chars; %s */" %
				(c[1].ljust (maxlen_s), (c[0] + ";").ljust (maxlen_l), c[2].rjust (maxlen_n), c[3]))
ਛਾਪੋ ()
ਜੇ language.name == "c":
	ਛਾਪੋ ('#pragma GCC diagnostic pop')
	ਛਾਪੋ ()
	ਛਾਪੋ ("#define INDIC_COMBINE_CATEGORIES(S,M) ((S) | ((M) << 8))")
	ਛਾਪੋ ()
	ਛਾਪੋ ("#define _(S,M) INDIC_COMBINE_CATEGORIES (%s_##S, %s_##M)" % tuple(what_short))
	ਛਾਪੋ ()
	ਛਾਪੋ ()
ਨਹੀਂਤਾਂ:
	ਛਾਪੋ ("const fn indic_combine_categories (s: u8, m: u8) -> u16")
	ਛਾਪੋ ("{")
	ਛਾਪੋ ("  (s as u16) | ((m as u16) << 8)")
	ਛਾਪੋ ("}")
	ਛਾਪੋ ()
	ਛਾਪੋ ()

ਜੇ language.name == "c":
	default_ਮੁੱਲ = "_(%s,%s)" % (ਛੋਟਾ[0][defaults[0]], ਛੋਟਾ[1][defaults[1]])
	pack_ਅੰਕੜੇ = {
		u: "_(%s,%s)" % (ਛੋਟਾ[0][d[0]], ਛੋਟਾ[1][d[1]])
		ਲਈ u, d ਵਿੱਚ indic_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ ()
	}
	pack_ਅੰਕੜੇ.update ({
		u: "_(%s,%s)" % (ਛੋਟਾ[0][d[0]], ਛੋਟਾ[1][d[1]])
		ਲਈ u, d ਵਿੱਚ singles.ਚੀਜ਼ਾਂ ()
	})
ਨਹੀਂਤਾਂ:
	default_ਮੁੱਲ = "indic_combine_categories(_OT_%s, _POS_%s)" % (ਛੋਟਾ[0][defaults[0]], ਛੋਟਾ[1][defaults[1]])
	pack_ਅੰਕੜੇ = {
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (ਛੋਟਾ[0][d[0]], ਛੋਟਾ[1][d[1]])
		ਲਈ u, d ਵਿੱਚ indic_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ ()
	}
	pack_ਅੰਕੜੇ.update ({
		u: "indic_combine_categories(_OT_%s, _POS_%s)" % (ਛੋਟਾ[0][d[0]], ਛੋਟਾ[1][d[1]])
		ਲਈ u, d ਵਿੱਚ singles.ਚੀਜ਼ਾਂ ()
	})

unique_ਮੁੱਲਾਂ = sorted (ਸੈੱਟ (pack_ਅੰਕੜੇ.ਮੁੱਲਾਂ ()) | {default_ਮੁੱਲ})
mapping = {v: i ਲਈ i, v ਵਿੱਚ enumerate (unique_ਮੁੱਲਾਂ)}

ਕੋਡ = packTab.ਕੋਡ_2 ('_hb_indic')
ਮੁੱਲ_type = 'uint16_t' ਜੇ language.name == "c" ਨਹੀਂਤਾਂ language.type_name ("u16")
indic_ਮੁੱਲਾਂ, _ = ਕੋਡ.ਜੋੜੋ_Array (ਮੁੱਲ_type, 'values', unique_ਮੁੱਲਾਂ)
sol = packTab.pack_ਸਾਰਣੀ (pack_ਅੰਕੜੇ, ਮੂਲ=default_ਮੁੱਲ, mapping=mapping, compression=9)
sol.gen_ਕੋਡ (ਕੋਡ, 'get_categories_index', language=language, ਨਿਜੀ=language.name == "c")
ਕੋਡ.print_ਕੋਡ (language=language, ਨਿਜੀ=language.name == "c")
ਛਾਪੋ ()
ਜੇ language.name == "c":
	ਛਾਪੋ ("uint16_t")
	ਛਾਪੋ ("hb_indic_get_categories (hb_codepoint_t u)")
	ਛਾਪੋ ("{")
	ਛਾਪੋ ("  return %s[_hb_indic_get_categories_index (u)];" % indic_ਮੁੱਲਾਂ)
	ਛਾਪੋ ("}")
	ਛਾਪੋ ()
	ਛਾਪੋ ("#undef _")
	ਛਾਪੋ ("#undef INDIC_COMBINE_CATEGORIES")
	ਲਈ i ਵਿੱਚ ਸੀਮਾ (2):
		ਛਾਪੋ ()
		vv = sorted (ਮੁੱਲਾਂ[i].ਕੁੰਜੀਆਂ ())
		ਲਈ v ਵਿੱਚ vv:
			ਛਾਪੋ ("#undef %s_%s" %
				(what_short[i], ਛੋਟਾ[i][v]))
	ਛਾਪੋ ()
	ਛਾਪੋ ('#endif')
ਨਹੀਂਤਾਂ:
	ਛਾਪੋ ("#[inline]")
	ਛਾਪੋ ("pub(crate) fn get_categories (u: u32) -> (u8, u8)")
	ਛਾਪੋ ("{")
	ਛਾਪੋ ("  let v = %s[_hb_indic_get_categories_index (u as usize) as usize];" % indic_ਮੁੱਲਾਂ)
	ਛਾਪੋ ("  ((v & 0xFF) as u8, (v >> 8) as u8)")
	ਛਾਪੋ ("}")
ਛਾਪੋ ()
ਛਾਪੋ ("/* == End of generated table == */")
