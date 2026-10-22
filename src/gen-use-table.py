#!/usr/bin/env python3
# flake8: noqa: F821

"""usage: ./gen-use-table.py IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/DerivedCoreProperties.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
* https://unicode.org/Public/UCD/latest/ucd/Scripts.txt
* ms-use/IndicSyllabicCategory-Additional.txt
* ms-use/IndicPositionalCategory-Additional.txt
"""

ਆਯਾਤ os
ਆਯਾਤ packTab

ਆਯਾਤ sys
ਆਯਾਤ logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

ਜੇ ਲੰਬਾਈ(sys.argv) > 1 ਅਤੇ sys.argv[1] == "--rust":
    ਹਟਾਓ sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
ਨਹੀਂਤਾਂ:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

ਆਯਾਤ sys

ਜੇ ਲੰਬਾਈ (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') ਲਈ x ਵਿੱਚ sys.argv[1:]]

ਸਿਰਲੇਖਾਂ = [[f.readline () ਲਈ i ਵਿੱਚ ਸੀਮਾ (2)] ਲਈ j,f ਵਿੱਚ enumerate(files) ਜੇ j != 4]
ਲਈ j ਵਿੱਚ ਸੀਮਾ(7, 9):
	ਲਈ ਸਤਰ ਵਿੱਚ files[j]:
		ਸਤਰ = ਸਤਰ.rstrip()
		ਜੇ ਨਹੀਂ ਸਤਰ:
			ਰੋਕੋ
		ਸਿਰਲੇਖਾਂ[j - 1].append(ਸਤਰ)
ਸਿਰਲੇਖਾਂ.append (["UnicodeData.txt does not have a header."])

unicode_ਅੰਕੜੇ = [{} ਲਈ _ ਵਿੱਚ files]
ਮੁੱਲਾਂ = [{} ਲਈ _ ਵਿੱਚ files]
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

		t = fields[1 ਜੇ i ਨਹੀਂ ਵਿੱਚ [2, 4] ਨਹੀਂਤਾਂ 2]

		ਜੇ i == 2:
			t = 'jt_' + t
		ਜਾਂਜੇ i == 3 ਅਤੇ t != 'Default_Ignorable_Code_Point':
			ਜਾਰੀ
		ਜਾਂਜੇ i == 7 ਅਤੇ t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		ਜਾਂਜੇ i == 8 ਅਤੇ t == 'NA':
			t = 'Not_Applicable'

		i0 = i ਜੇ i < 7 ਨਹੀਂਤਾਂ i - 7
		ਲਈ u ਵਿੱਚ ਸੀਮਾ (ਸ਼ੁਰੂ, ਸਿਰਾ + 1):
			unicode_ਅੰਕੜੇ[i0][u] = t
		ਮੁੱਲਾਂ[i0][t] = ਮੁੱਲਾਂ[i0].get (t, 0) + ਸਿਰਾ - ਸ਼ੁਰੂ + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
ਲਈ i,v ਵਿੱਚ enumerate (defaults):
	ਮੁੱਲਾਂ[i][v] = ਮੁੱਲਾਂ[i].get (v, 0) + 1
combined = {}
ਲਈ i,d ਵਿੱਚ enumerate (unicode_ਅੰਕੜੇ):
	ਲਈ u,v ਵਿੱਚ d.ਚੀਜ਼ਾਂ ():
		ਜੇ ਨਹੀਂ u ਵਿੱਚ combined:
			ਜੇ i >= 4:
				ਜਾਰੀ
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v ਲਈ k, v ਵਿੱਚ combined.ਚੀਜ਼ਾਂ() ਜੇ v[6] ਨਹੀਂ ਵਿੱਚ DISABLED_SCRIPTS}


property_ਨਾਂ = [
	# General_Category
	'Cc', 'Cf', 'Cn', 'Co', 'Cs', 'Ll', 'Lm', 'Lo', 'Lt', 'Lu', 'Mc',
	'Me', 'Mn', 'Nd', 'Nl', 'No', 'Pc', 'Pd', 'Pe', 'Pf', 'Pi', 'Po',
	'Ps', 'Sc', 'Sk', 'Sm', 'So', 'Zl', 'Zp', 'Zs',
	# Indic_Syllabic_Category
	'Other',
	'Bindu',
	'Visarga',
	'Avagraha',
	'Nukta',
	'Virama',
	'Pure_Killer',
	'Reordering_Killer',
	'Invisible_Stacker',
	'Vowel_Independent',
	'Vowel_Dependent',
	'Vowel',
	'Consonant_Placeholder',
	'Consonant',
	'Consonant_Dead',
	'Consonant_With_Stacker',
	'Consonant_Prefixed',
	'Consonant_Preceding_Repha',
	'Consonant_Succeeding_Repha',
	'Consonant_Subjoined',
	'Consonant_Medial',
	'Consonant_Final',
	'Consonant_Head_Letter',
	'Consonant_Initial_Postfixed',
	'Modifying_Letter',
	'Tone_Letter',
	'Tone_Mark',
	'Gemination_Mark',
	'Cantillation_Mark',
	'Register_Shifter',
	'Syllable_Modifier',
	'Consonant_Killer',
	'Non_Joiner',
	'Joiner',
	'Number_Joiner',
	'Number',
	'Brahmi_Joining_Number',
	'Symbol_Modifier',
	'Hieroglyph',
	'Hieroglyph_Joiner',
	'Hieroglyph_Mark_Begin',
	'Hieroglyph_Mark_End',
	'Hieroglyph_Mirror',
	'Hieroglyph_Modifier',
	'Hieroglyph_Segment_Begin',
	'Hieroglyph_Segment_End',
	# Indic_Positional_Category
	'Not_Applicable',
	'Right',
	'Left',
	'Visual_Order_Left',
	'Left_And_Right',
	'Top',
	'Bottom',
	'Top_And_Bottom',
	'Top_And_Bottom_And_Left',
	'Top_And_Right',
	'Top_And_Left',
	'Top_And_Left_And_Right',
	'Bottom_And_Left',
	'Bottom_And_Right',
	'Top_And_Bottom_And_Right',
	'Overstruck',
	# Joining_Type
	'jt_C',
	'jt_D',
	'jt_L',
	'jt_R',
	'jt_T',
	'jt_U',
	'jt_X',
]

ਵਰਗ Property_ਮੁੱਲ(object):
	ਪਰਿਭਾਸ਼ਾ __init__(self, name_):
		self.name = name_
	ਪਰਿਭਾਸ਼ਾ __str__(self):
		ਵਾਪਸ self.name
	ਪਰਿਭਾਸ਼ਾ __eq__(self, other):
		ਵਾਪਸ self.name == (other ਜੇ isinstance(other, str) ਨਹੀਂਤਾਂ other.name)
	ਪਰਿਭਾਸ਼ਾ __ne__(self, other):
		ਵਾਪਸ ਨਹੀਂ (self == other)
	ਪਰਿਭਾਸ਼ਾ __hash__(self):
		ਵਾਪਸ hash(str(self))

property_ਮੁੱਲਾਂ = {}

ਲਈ name ਵਿੱਚ property_ਨਾਂ:
	ਮੁੱਲ_2 = Property_ਮੁੱਲ(name)
	ਪ੍ਰਤਿਜ੍ਞਾ ਮੁੱਲ_2 ਨਹੀਂ ਵਿੱਚ property_ਮੁੱਲਾਂ
	ਪ੍ਰਤਿਜ੍ਞਾ ਮੁੱਲ_2 ਨਹੀਂ ਵਿੱਚ globals()
	property_ਮੁੱਲਾਂ[name] = ਮੁੱਲ_2
globals().update(property_ਮੁੱਲਾਂ)


ਪਰਿਭਾਸ਼ਾ is_ਅਧਾਰ(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ (UISC ਵਿੱਚ [ਸੰਖਿਆ, Consonant, Consonant_ਸਿਰ_Letter,
			Tone_Letter,
			Vowel_Independent,
			] ਜਾਂ
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT ਵਿੱਚ [jt_C, jt_D, jt_L, jt_R] ਅਤੇ UISC != Joiner ਜਾਂ
		(UGC == Lo ਅਤੇ UISC ਵਿੱਚ [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
ਪਰਿਭਾਸ਼ਾ is_ਅਧਾਰ_ਅੰਕ(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Brahmi_Joining_ਸੰਖਿਆ
ਪਰਿਭਾਸ਼ਾ is_ਅਧਾਰ_OTHER(U, UISC, UDI, UGC, AJT):
	ਜੇ UISC == Consonant_Placeholder: ਵਾਪਸ ਸੱਚ
	ਵਾਪਸ U ਵਿੱਚ [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
ਪਰਿਭਾਸ਼ਾ is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ਵਾਪਸ UISC == Joiner ਜਾਂ UDI ਅਤੇ UGC ਵਿੱਚ [Mc, Me, Mn]
ਪਰਿਭਾਸ਼ਾ is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ ((UISC == Consonant_Final ਅਤੇ UGC != Lo) ਜਾਂ
		UISC == Consonant_Succeeding_Repha)
ਪਰਿਭਾਸ਼ਾ is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Syllable_Modifier
ਪਰਿਭਾਸ਼ਾ is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ਵਾਪਸ (UISC == Consonant_Medial ਅਤੇ UGC != Lo ਜਾਂ
		UISC == Consonant_Initial_Postfixed)
ਪਰਿਭਾਸ਼ਾ is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC ਵਿੱਚ [Nukta, Gemination_Mark, Consonant_Killer]
ਪਰਿਭਾਸ਼ਾ is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Consonant_Subjoined ਅਤੇ UGC != Lo
ਪਰਿਭਾਸ਼ਾ is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Consonant_With_Stacker
ਪਰਿਭਾਸ਼ਾ is_HALANT(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Virama ਅਤੇ ਨਹੀਂ is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
ਪਰਿਭਾਸ਼ਾ is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ਵਾਪਸ U == 0x0DCA
ਪਰਿਭਾਸ਼ਾ is_HALANT_ਅੰਕ(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == ਸੰਖਿਆ_Joiner
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Hieroglyph
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Hieroglyph_Joiner
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Hieroglyph_Mirror
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Hieroglyph_Modifier
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC ਵਿੱਚ [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
ਪਰਿਭਾਸ਼ਾ is_HIEROGLYPH_SEGMENT_ਸਿਰਾ(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC ਵਿੱਚ [Hieroglyph_Mark_ਸਿਰਾ, Hieroglyph_Segment_ਸਿਰਾ]
ਪਰਿਭਾਸ਼ਾ is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ਵਾਪਸ (UISC == Invisible_Stacker
		ਅਤੇ ਨਹੀਂ is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
ਪਰਿਭਾਸ਼ਾ is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Non_Joiner
ਪਰਿਭਾਸ਼ਾ is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ਵਾਪਸ ((UGC == Po ਜਾਂ UISC ਵਿੱਚ [Consonant_Dead, Joiner, Modifying_Letter, Other])
		ਅਤੇ ਨਹੀਂ is_ਅਧਾਰ(U, UISC, UDI, UGC, AJT)
		ਅਤੇ ਨਹੀਂ is_ਅਧਾਰ_OTHER(U, UISC, UDI, UGC, AJT)
		ਅਤੇ ਨਹੀਂ is_CGJ(U, UISC, UDI, UGC, AJT)
		ਅਤੇ ਨਹੀਂ is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		ਅਤੇ ਨਹੀਂ is_ਸ਼ਬਦ_Joiner(U, UISC, UDI, UGC, AJT)
	)
ਪਰਿਭਾਸ਼ਾ is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == Reordering_Killer
ਪਰਿਭਾਸ਼ਾ is_REPHA(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC ਵਿੱਚ [Consonant_Preceding_Repha, Consonant_Prefixed]
ਪਰਿਭਾਸ਼ਾ is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ਵਾਪਸ U == 0x1A60
ਪਰਿਭਾਸ਼ਾ is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ UISC == ਚਿੰਨ੍ਹ_Modifier
ਪਰਿਭਾਸ਼ਾ is_VOWEL(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ (UISC == ਸ਼ੁੱਧ_Killer ਜਾਂ
		UGC != Lo ਅਤੇ UISC ਵਿੱਚ [Vowel, Vowel_Dependent])
ਪਰਿਭਾਸ਼ਾ is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ਵਾਪਸ (UISC ਵਿੱਚ [Tone_Mark, Cantillation_Mark, ਰਜਿਸਟਰ_Shifter, Visarga] ਜਾਂ
		UGC != Lo ਅਤੇ UISC == Bindu)
ਪਰਿਭਾਸ਼ਾ is_ਸ਼ਬਦ_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ਵਾਪਸ (UDI ਅਤੇ U ਨਹੀਂ ਵਿੱਚ [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		ਅਤੇ UISC == Other
		ਅਤੇ ਨਹੀਂ is_CGJ(U, UISC, UDI, UGC, AJT)
	) ਜਾਂ UGC == Cn

use_mapping = {
	'B':	is_ਅਧਾਰ,
	'N':	is_ਅਧਾਰ_ਅੰਕ,
	'GB':	is_ਅਧਾਰ_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_ਅੰਕ,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_ਸਿਰਾ,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_ਸ਼ਬਦ_Joiner,
}

use_positions = {
	'F': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom],
		'Pst': [ਸੱਜਾ],
	},
	'M': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom, Bottom_And_ਖੱਬਾ, Bottom_And_ਸੱਜਾ],
		'Pst': [ਸੱਜਾ],
		'Pre': [ਖੱਬਾ, ਸਿਖਰ_And_Bottom_And_ਖੱਬਾ],
	},
	'CM': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [ਸਿਖਰ, ਸਿਖਰ_And_Bottom, ਸਿਖਰ_And_Bottom_And_ਸੱਜਾ, ਸਿਖਰ_And_ਸੱਜਾ],
		'Blw': [Bottom, Overstruck, Bottom_And_ਸੱਜਾ],
		'Pst': [ਸੱਜਾ],
		'Pre': [ਖੱਬਾ, ਸਿਖਰ_And_ਖੱਬਾ, ਸਿਖਰ_And_ਖੱਬਾ_And_ਸੱਜਾ, ਖੱਬਾ_And_ਸੱਜਾ],
	},
	'VM': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom, Overstruck],
		'Pst': [ਸੱਜਾ],
		'Pre': [ਖੱਬਾ],
	},
	'SM': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom],
	},
	'H': ਕੁਝ_ਨਹੀਂ,
	'HM': ਕੁਝ_ਨਹੀਂ,
	'HR': ਕੁਝ_ਨਹੀਂ,
	'HVM': ਕੁਝ_ਨਹੀਂ,
	'IS': ਕੁਝ_ਨਹੀਂ,
	'B': ਕੁਝ_ਨਹੀਂ,
	'FM': {
		'Abv': [ਸਿਖਰ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': ਕੁਝ_ਨਹੀਂ,
	'RK': ਕੁਝ_ਨਹੀਂ,
	'SUB': ਕੁਝ_ਨਹੀਂ,
}

ਪਰਿਭਾਸ਼ਾ map_ਮੰਜ਼ਿਲ_use(ਅੰਕੜੇ):
	ਬਾਹਰਲਾ = {}
	ਚੀਜ਼ਾਂ = use_mapping.ਚੀਜ਼ਾਂ()
	ਲਈ U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) ਵਿੱਚ ਅੰਕੜੇ.ਚੀਜ਼ਾਂ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ਜੇ 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ਜੇ 0x0F18 <= U <= 0x0F19 ਜਾਂ 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		ਜੇ U == 0x1CED: UISC = Tone_Mark

		ਮੁੱਲਾਂ = [k ਲਈ k,v ਵਿੱਚ ਚੀਜ਼ਾਂ ਜੇ v(U, UISC, UDI, UGC, AJT)]
		ਪ੍ਰਤਿਜ੍ਞਾ ਲੰਬਾਈ(ਮੁੱਲਾਂ) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, ਮੁੱਲਾਂ)
		USE = ਮੁੱਲਾਂ[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		ਜੇ U ਵਿੱਚ [0x11302, 0x11303, 0x114C1]: UIPC = ਸਿਖਰ

		ਪ੍ਰਤਿਜ੍ਞਾ (UIPC ਵਿੱਚ [Not_Applicable, Visual_ਕ੍ਰਮ_ਖੱਬਾ] ਜਾਂ
			U == 0x0F7F ਜਾਂ
			USE ਵਿੱਚ use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, ਕੁਝ_ਨਹੀਂ)
		ਜੇ pos_mapping:
			ਮੁੱਲਾਂ = [k ਲਈ k,v ਵਿੱਚ pos_mapping.ਚੀਜ਼ਾਂ() ਜੇ v ਅਤੇ UIPC ਵਿੱਚ v]
			ਪ੍ਰਤਿਜ੍ਞਾ ਲੰਬਾਈ(ਮੁੱਲਾਂ) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, ਮੁੱਲਾਂ)
			USE = USE + ਮੁੱਲਾਂ[0]

		ਬਾਹਰਲਾ[U] = (USE, UBlock)
	ਵਾਪਸ ਬਾਹਰਲਾ

use_ਅੰਕੜੇ = map_ਮੰਜ਼ਿਲ_use(combined)

ਛਾਪੋ ("/* == Start of generated table == */")
ਛਾਪੋ ("/*")
ਛਾਪੋ (" * The following table is generated by running:")
ਛਾਪੋ (" *")
ਛਾਪੋ (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.ਰਸਤਾ.basename (sys.argv[0])))
ਛਾਪੋ (" *")
ਛਾਪੋ (" * on files with these headers:")
ਛਾਪੋ (" *")
ਲਈ h ਵਿੱਚ ਸਿਰਲੇਖਾਂ:
	ਲਈ l ਵਿੱਚ h:
		ਛਾਪੋ (" * %s" % (l.strip()))
ਛਾਪੋ (" */")
ਛਾਪੋ ()
ਜੇ language.name == "c":
    ਛਾਪੋ ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    ਛਾਪੋ ("#define HB_OT_SHAPER_USE_TABLE_HH")
    ਛਾਪੋ ()
    ਛਾਪੋ ('#include "hb.hh"')
    ਛਾਪੋ ()
    ਛਾਪੋ ('#include "hb-ot-shaper-use-machine.hh"')
    ਛਾਪੋ ()

    ਛਾਪੋ ('#pragma GCC diagnostic push')
    ਛਾਪੋ ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    ਲਈ k,v ਵਿੱਚ sorted(use_mapping.ਚੀਜ਼ਾਂ()):
        ਜੇ k ਵਿੱਚ use_positions ਅਤੇ use_positions[k]: ਜਾਰੀ
        ਛਾਪੋ ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    ਲਈ k,v ਵਿੱਚ sorted(use_positions.ਚੀਜ਼ਾਂ()):
        ਜੇ ਨਹੀਂ v: ਜਾਰੀ
        ਲਈ suf ਵਿੱਚ v.ਕੁੰਜੀਆਂ():
            ਟੈਗ_2 = k + suf
            ਛਾਪੋ ("#define %s	USE(%s)" % (ਟੈਗ_2, ਟੈਗ_2))
    ਛਾਪੋ ('#pragma GCC diagnostic pop')
    ਛਾਪੋ ("")

ਜਾਂਜੇ language.name == "rust":
    ਛਾਪੋ()
    ਛਾਪੋ("#![allow(unused_parens)]")
    ਛਾਪੋ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    ਛਾਪੋ()
    ਛਾਪੋ("use super::ot_shaper_use::category::*;")
    ਛਾਪੋ()
ਨਹੀਂਤਾਂ:
    ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name

uu = sorted (use_ਅੰਕੜੇ.ਕੁੰਜੀਆਂ ())

ਅੰਕੜੇ = {u:v[0] ਲਈ u,v ਵਿੱਚ use_ਅੰਕੜੇ.ਚੀਜ਼ਾਂ()}

ਜੇ language.name == "c":
    ਨਿਜੀ = ਸੱਚ
ਜਾਂਜੇ language.name == "rust":
    ਨਿਜੀ = ਝੂਠ
    language.ਜਨਤਕ_function_linkage = "pub(crate)"
ਨਹੀਂਤਾਂ:
    ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
ਜੇ language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[ਕੁਝ_ਨਹੀਂ] = "#endif"
ਨਹੀਂਤਾਂ:
    modes[DEFAULT] = ""

ਲਈ ਕਦਮ, ਪਾਠ ਵਿੱਚ modes.ਚੀਜ਼ਾਂ():
    ਛਾਪੋ()
    ਜੇ ਪਾਠ:
        ਛਾਪੋ(ਪਾਠ)
        ਛਾਪੋ()
    ਜੇ ਕਦਮ ਹੈ ਕੁਝ_ਨਹੀਂ:
        ਜਾਰੀ

    compression = compression_level[ਕਦਮ]
    logging.info("  Compression=%d:" % compression)

    ਕੋਡ = packTab.ਕੋਡ_2('hb_use')
    sol = packTab.pack_ਸਾਰਣੀ(ਅੰਕੜੇ, compression=compression, ਮੂਲ='O')
    logging.info('      FullCost=%d' % (sol.ਭਰਿਆ_ਲਾਗਤ))
    sol.gen_ਕੋਡ(ਕੋਡ, f'get_category', language=language, ਨਿਜੀ=ਨਿਜੀ)
    ਕੋਡ.print_ਕੋਡ(language=language, ਨਿਜੀ=ਨਿਜੀ)
    ਛਾਪੋ ()

ਜੇ language.name == "c":
    ਛਾਪੋ ()
    ਲਈ k ਵਿੱਚ sorted(use_mapping.ਕੁੰਜੀਆਂ()):
        ਜੇ k ਵਿੱਚ use_positions ਅਤੇ use_positions[k]: ਜਾਰੀ
        ਛਾਪੋ ("#undef %s" % k)
    ਲਈ k,v ਵਿੱਚ sorted(use_positions.ਚੀਜ਼ਾਂ()):
        ਜੇ ਨਹੀਂ v: ਜਾਰੀ
        ਲਈ suf ਵਿੱਚ v.ਕੁੰਜੀਆਂ():
            ਟੈਗ_2 = k + suf
            ਛਾਪੋ ("#undef %s" % ਟੈਗ_2)
    ਛਾਪੋ ()
    ਛਾਪੋ ()
    ਛਾਪੋ ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
ਜਾਂਜੇ language.name == "rust":
    ਲੰਘੋ
ਨਹੀਂਤਾਂ:
    ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name
ਛਾਪੋ ("/* == End of generated table == */")
