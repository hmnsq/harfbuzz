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

ಆನಯನ os
ಆನಯನ packTab

ಆನಯನ sys
ಆನಯನ logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

ಯದಿ len(sys.argv) > 1 ಚ sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
ಅನ್ಯಥಾ:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

ಆನಯನ sys

ಯದಿ len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') ಪ್ರತಿ x ಅನ್ತಃ sys.argv[1:]]

headers = [[f.readline () ಪ್ರತಿ i ಅನ್ತಃ range (2)] ಪ್ರತಿ j,f ಅನ್ತಃ enumerate(files) ಯದಿ j != 4]
ಪ್ರತಿ j ಅನ್ತಃ range(7, 9):
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ files[j]:
		ಸಾಲು = ಸಾಲು.rstrip()
		ಯದಿ ನ ಸಾಲು:
			ಭಙ್ಗ
		headers[j - 1].append(ಸಾಲು)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} ಪ್ರತಿ _ ಅನ್ತಃ files]
ಮೌಲ್ಯಗಳು = [{} ಪ್ರತಿ _ ಅನ್ತಃ files]
ಪ್ರತಿ i, f ಅನ್ತಃ enumerate (files):
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ f:

		j = ಸಾಲು.find ('#')
		ಯದಿ j >= 0:
			ಸಾಲು = ಸಾಲು[:j]

		fields = [x.strip () ಪ್ರತಿ x ಅನ್ತಃ ಸಾಲು.split (';')]
		ಯದಿ len (fields) == 1:
			ಅನುವರ್ತ

		uu = fields[0].split ('..')
		start = ಪೂರ್ಣಾಙ್ಕ (uu[0], 16)
		ಯದಿ len (uu) == 1:
			end = start
		ಅನ್ಯಥಾ:
			end = ಪೂರ್ಣಾಙ್ಕ (uu[1], 16)

		t = fields[1 ಯದಿ i ನ ಅನ್ತಃ [2, 4] ಅನ್ಯಥಾ 2]

		ಯದಿ i == 2:
			t = 'jt_' + t
		ನೋಚೇದ್ಯದಿ i == 3 ಚ t != 'Default_Ignorable_Code_Point':
			ಅನುವರ್ತ
		ನೋಚೇದ್ಯದಿ i == 7 ಚ t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		ನೋಚೇದ್ಯದಿ i == 8 ಚ t == 'NA':
			t = 'Not_Applicable'

		i0 = i ಯದಿ i < 7 ಅನ್ಯಥಾ i - 7
		ಪ್ರತಿ u ಅನ್ತಃ range (start, end + 1):
			unicode_data[i0][u] = t
		ಮೌಲ್ಯಗಳು[i0][t] = ಮೌಲ್ಯಗಳು[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
ಪ್ರತಿ i,v ಅನ್ತಃ enumerate (defaults):
	ಮೌಲ್ಯಗಳು[i][v] = ಮೌಲ್ಯಗಳು[i].get (v, 0) + 1
combined = {}
ಪ್ರತಿ i,d ಅನ್ತಃ enumerate (unicode_data):
	ಪ್ರತಿ u,v ಅನ್ತಃ d.ವಸ್ತುಗಳು ():
		ಯದಿ ನ u ಅನ್ತಃ combined:
			ಯದಿ i >= 4:
				ಅನುವರ್ತ
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v ಪ್ರತಿ k, v ಅನ್ತಃ combined.ವಸ್ತುಗಳು() ಯದಿ v[6] ನ ಅನ್ತಃ DISABLED_SCRIPTS}


property_names = [
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

ವರ್ಗ Property_ಮೌಲ್ಯ(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		ಪ್ರತ್ಯಾಗಮ self.name
	def __eq__(self, other):
		ಪ್ರತ್ಯಾಗಮ self.name == (other ಯದಿ isinstance(other, str) ಅನ್ಯಥಾ other.name)
	def __ne__(self, other):
		ಪ್ರತ್ಯಾಗಮ ನ (self == other)
	def __hash__(self):
		ಪ್ರತ್ಯಾಗಮ hash(str(self))

property_ಮೌಲ್ಯಗಳು = {}

ಪ್ರತಿ name ಅನ್ತಃ property_names:
	ಮೌಲ್ಯ_2 = Property_ಮೌಲ್ಯ(name)
	ಪ್ರತಿಜ್ಞಾ ಮೌಲ್ಯ_2 ನ ಅನ್ತಃ property_ಮೌಲ್ಯಗಳು
	ಪ್ರತಿಜ್ಞಾ ಮೌಲ್ಯ_2 ನ ಅನ್ತಃ globals()
	property_ಮೌಲ್ಯಗಳು[name] = ಮೌಲ್ಯ_2
globals().update(property_ಮೌಲ್ಯಗಳು)


def is_BASE(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ (UISC ಅನ್ತಃ [ಸಂಖ್ಯೆ, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] ವಾ
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT ಅನ್ತಃ [jt_C, jt_D, jt_L, jt_R] ಚ UISC != Joiner ವಾ
		(UGC == Lo ಚ UISC ಅನ್ತಃ [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_ಅಂಕೆ(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Brahmi_Joining_ಸಂಖ್ಯೆ
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	ಯದಿ UISC == Consonant_Placeholder: ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ
	ಪ್ರತ್ಯಾಗಮ U ಅನ್ತಃ [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ಪ್ರತ್ಯಾಗಮ UISC == Joiner ವಾ UDI ಚ UGC ಅನ್ತಃ [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ ((UISC == Consonant_Final ಚ UGC != Lo) ವಾ
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ಪ್ರತ್ಯಾಗಮ (UISC == Consonant_Medial ಚ UGC != Lo ವಾ
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC ಅನ್ತಃ [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Consonant_Subjoined ಚ UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Virama ಚ ನ is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ಪ್ರತ್ಯಾಗಮ U == 0x0DCA
def is_HALANT_ಅಂಕೆ(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == ಸಂಖ್ಯೆ_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC ಅನ್ತಃ [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC ಅನ್ತಃ [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ಪ್ರತ್ಯಾಗಮ (UISC == Invisible_Stacker
		ಚ ನ is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ಪ್ರತ್ಯಾಗಮ ((UGC == Po ವಾ UISC ಅನ್ತಃ [Consonant_Dead, Joiner, Modifying_Letter, Other])
		ಚ ನ is_BASE(U, UISC, UDI, UGC, AJT)
		ಚ ನ is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		ಚ ನ is_CGJ(U, UISC, UDI, UGC, AJT)
		ಚ ನ is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		ಚ ನ is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC ಅನ್ತಃ [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ಪ್ರತ್ಯಾಗಮ U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ UISC == ಚಿಹ್ನೆ_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ (UISC == Pure_Killer ವಾ
		UGC != Lo ಚ UISC ಅನ್ತಃ [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ಪ್ರತ್ಯಾಗಮ (UISC ಅನ್ತಃ [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] ವಾ
		UGC != Lo ಚ UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ಪ್ರತ್ಯಾಗಮ (UDI ಚ U ನ ಅನ್ತಃ [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		ಚ UISC == Other
		ಚ ನ is_CGJ(U, UISC, UDI, UGC, AJT)
	) ವಾ UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_ಅಂಕೆ,
	'GB':	is_BASE_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_ಅಂಕೆ,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_END,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_Word_Joiner,
}

use_positions = {
	'F': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom],
		'Pst': [ಬಲ_2],
	},
	'M': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom, Bottom_And_ಎಡ, Bottom_And_ಬಲ],
		'Pst': [ಬಲ_2],
		'Pre': [ಎಡ, ತುದಿ_And_Bottom_And_ಎಡ],
	},
	'CM': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [ತುದಿ, ತುದಿ_And_Bottom, ತುದಿ_And_Bottom_And_ಬಲ, ತುದಿ_And_ಬಲ],
		'Blw': [Bottom, Overstruck, Bottom_And_ಬಲ],
		'Pst': [ಬಲ_2],
		'Pre': [ಎಡ, ತುದಿ_And_ಎಡ, ತುದಿ_And_ಎಡ_And_ಬಲ, ಎಡ_And_ಬಲ],
	},
	'VM': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom, Overstruck],
		'Pst': [ಬಲ_2],
		'Pre': [ಎಡ],
	},
	'SM': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [ತುದಿ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	ವಸ್ತುಗಳು = use_mapping.ವಸ್ತುಗಳು()
	ಪ್ರತಿ U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) ಅನ್ತಃ data.ವಸ್ತುಗಳು():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ಯದಿ 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ಯದಿ 0x0F18 <= U <= 0x0F19 ವಾ 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		ಯದಿ U == 0x1CED: UISC = Tone_Mark

		ಮೌಲ್ಯಗಳು = [k ಪ್ರತಿ k,v ಅನ್ತಃ ವಸ್ತುಗಳು ಯದಿ v(U, UISC, UDI, UGC, AJT)]
		ಪ್ರತಿಜ್ಞಾ len(ಮೌಲ್ಯಗಳು) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, ಮೌಲ್ಯಗಳು)
		USE = ಮೌಲ್ಯಗಳು[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		ಯದಿ U ಅನ್ತಃ [0x11302, 0x11303, 0x114C1]: UIPC = ತುದಿ

		ಪ್ರತಿಜ್ಞಾ (UIPC ಅನ್ತಃ [Not_Applicable, Visual_Order_ಎಡ] ವಾ
			U == 0x0F7F ವಾ
			USE ಅನ್ತಃ use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		ಯದಿ pos_mapping:
			ಮೌಲ್ಯಗಳು = [k ಪ್ರತಿ k,v ಅನ್ತಃ pos_mapping.ವಸ್ತುಗಳು() ಯದಿ v ಚ UIPC ಅನ್ತಃ v]
			ಪ್ರತಿಜ್ಞಾ len(ಮೌಲ್ಯಗಳು) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, ಮೌಲ್ಯಗಳು)
			USE = USE + ಮೌಲ್ಯಗಳು[0]

		out[U] = (USE, UBlock)
	ಪ್ರತ್ಯಾಗಮ out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
ಪ್ರತಿ h ಅನ್ತಃ headers:
	ಪ್ರತಿ l ಅನ್ತಃ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
ಯದಿ language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    ಪ್ರತಿ k,v ಅನ್ತಃ sorted(use_mapping.ವಸ್ತುಗಳು()):
        ಯದಿ k ಅನ್ತಃ use_positions ಚ use_positions[k]: ಅನುವರ್ತ
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    ಪ್ರತಿ k,v ಅನ್ತಃ sorted(use_positions.ವಸ್ತುಗಳು()):
        ಯದಿ ನ v: ಅನುವರ್ತ
        ಪ್ರತಿ suf ಅನ್ತಃ v.ಕೀಲಿಗಳು():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

ನೋಚೇದ್ಯದಿ language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
ಅನ್ಯಥಾ:
    ಪ್ರತಿಜ್ಞಾ ಅಸತ್ಯ, "Unknown language: %s" % language.name

uu = sorted (use_data.ಕೀಲಿಗಳು ())

data = {u:v[0] ಪ್ರತಿ u,v ಅನ್ತಃ use_data.ವಸ್ತುಗಳು()}

ಯದಿ language.name == "c":
    ಗುಪ್ತ = ಸತ್ಯ
ನೋಚೇದ್ಯದಿ language.name == "rust":
    ಗುಪ್ತ = ಅಸತ್ಯ
    language.public_function_linkage = "pub(crate)"
ಅನ್ಯಥಾ:
    ಪ್ರತಿಜ್ಞಾ ಅಸತ್ಯ, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
ಯದಿ language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
ಅನ್ಯಥಾ:
    modes[DEFAULT] = ""

ಪ್ರತಿ step, text ಅನ್ತಃ modes.ವಸ್ತುಗಳು():
    print()
    ಯದಿ text:
        print(text)
        print()
    ಯದಿ step is None:
        ಅನುವರ್ತ

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, ಸ್ವಭಾವ='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, ಗುಪ್ತ=ಗುಪ್ತ)
    code.print_code(language=language, ಗುಪ್ತ=ಗುಪ್ತ)
    print ()

ಯದಿ language.name == "c":
    print ()
    ಪ್ರತಿ k ಅನ್ತಃ sorted(use_mapping.ಕೀಲಿಗಳು()):
        ಯದಿ k ಅನ್ತಃ use_positions ಚ use_positions[k]: ಅನುವರ್ತ
        print ("#undef %s" % k)
    ಪ್ರತಿ k,v ಅನ್ತಃ sorted(use_positions.ವಸ್ತುಗಳು()):
        ಯದಿ ನ v: ಅನುವರ್ತ
        ಪ್ರತಿ suf ಅನ್ತಃ v.ಕೀಲಿಗಳು():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
ನೋಚೇದ್ಯದಿ language.name == "rust":
    pass
ಅನ್ಯಥಾ:
    ಪ್ರತಿಜ್ಞಾ ಅಸತ್ಯ, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
