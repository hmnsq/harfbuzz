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

ଆମଦାନୀ os
ଆମଦାନୀ packTab

ଆମଦାନୀ sys
ଆମଦାନୀ logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

ଯଦି len(sys.argv) > 1 ଏବଂ sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
ଅନ୍ୟଥା:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

ଆମଦାନୀ sys

ଯଦି len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') ପାଇଁ x ଭିତରେ sys.argv[1:]]

headers = [[f.readline () ପାଇଁ i ଭିତରେ range (2)] ପାଇଁ j,f ଭିତରେ enumerate(files) ଯଦି j != 4]
ପାଇଁ j ଭିତରେ range(7, 9):
	ପାଇଁ ଧାଡ଼ି ଭିତରେ files[j]:
		ଧାଡ଼ି = ଧାଡ଼ି.rstrip()
		ଯଦି ନୁହେଁ ଧାଡ଼ି:
			ଭାଙ୍ଗ
		headers[j - 1].append(ଧାଡ଼ି)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} ପାଇଁ _ ଭିତରେ files]
ମୂଲ୍ୟଗୁଡ଼ିକ = [{} ପାଇଁ _ ଭିତରେ files]
ପାଇଁ i, f ଭିତରେ enumerate (files):
	ପାଇଁ ଧାଡ଼ି ଭିତରେ f:

		j = ଧାଡ଼ି.find ('#')
		ଯଦି j >= 0:
			ଧାଡ଼ି = ଧାଡ଼ି[:j]

		fields = [x.strip () ପାଇଁ x ଭିତରେ ଧାଡ଼ି.split (';')]
		ଯଦି len (fields) == 1:
			ଜାରି

		uu = fields[0].split ('..')
		start = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[0], 16)
		ଯଦି len (uu) == 1:
			end = start
		ଅନ୍ୟଥା:
			end = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[1], 16)

		t = fields[1 ଯଦି i ନୁହେଁ ଭିତରେ [2, 4] ଅନ୍ୟଥା 2]

		ଯଦି i == 2:
			t = 'jt_' + t
		ନଚେତଯଦି i == 3 ଏବଂ t != 'Default_Ignorable_Code_Point':
			ଜାରି
		ନଚେତଯଦି i == 7 ଏବଂ t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		ନଚେତଯଦି i == 8 ଏବଂ t == 'NA':
			t = 'Not_Applicable'

		i0 = i ଯଦି i < 7 ଅନ୍ୟଥା i - 7
		ପାଇଁ u ଭିତରେ range (start, end + 1):
			unicode_data[i0][u] = t
		ମୂଲ୍ୟଗୁଡ଼ିକ[i0][t] = ମୂଲ୍ୟଗୁଡ଼ିକ[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
ପାଇଁ i,v ଭିତରେ enumerate (defaults):
	ମୂଲ୍ୟଗୁଡ଼ିକ[i][v] = ମୂଲ୍ୟଗୁଡ଼ିକ[i].get (v, 0) + 1
combined = {}
ପାଇଁ i,d ଭିତରେ enumerate (unicode_data):
	ପାଇଁ u,v ଭିତରେ d.ବସ୍ତୁଗୁଡ଼ିକ ():
		ଯଦି ନୁହେଁ u ଭିତରେ combined:
			ଯଦି i >= 4:
				ଜାରି
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v ପାଇଁ k, v ଭିତରେ combined.ବସ୍ତୁଗୁଡ଼ିକ() ଯଦି v[6] ନୁହେଁ ଭିତରେ DISABLED_SCRIPTS}


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

ଶ୍ରେଣୀ Property_ମୂଲ୍ୟ(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		ଫେରନ୍ତୁ self.name
	def __eq__(self, other):
		ଫେରନ୍ତୁ self.name == (other ଯଦି isinstance(other, str) ଅନ୍ୟଥା other.name)
	def __ne__(self, other):
		ଫେରନ୍ତୁ ନୁହେଁ (self == other)
	def __hash__(self):
		ଫେରନ୍ତୁ hash(str(self))

property_ମୂଲ୍ୟଗୁଡ଼ିକ = {}

ପାଇଁ name ଭିତରେ property_names:
	ମୂଲ୍ୟ_2 = Property_ମୂଲ୍ୟ(name)
	ପ୍ରତିଜ୍ଞା ମୂଲ୍ୟ_2 ନୁହେଁ ଭିତରେ property_ମୂଲ୍ୟଗୁଡ଼ିକ
	ପ୍ରତିଜ୍ଞା ମୂଲ୍ୟ_2 ନୁହେଁ ଭିତରେ globals()
	property_ମୂଲ୍ୟଗୁଡ଼ିକ[name] = ମୂଲ୍ୟ_2
globals().update(property_ମୂଲ୍ୟଗୁଡ଼ିକ)


def is_BASE(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ (UISC ଭିତରେ [ସଂଖ୍ୟା, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] କିମ୍ବା
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT ଭିତରେ [jt_C, jt_D, jt_L, jt_R] ଏବଂ UISC != Joiner କିମ୍ବା
		(UGC == Lo ଏବଂ UISC ଭିତରେ [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_ଅଙ୍କ(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Brahmi_Joining_ସଂଖ୍ୟା
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	ଯଦି UISC == Consonant_Placeholder: ଫେରନ୍ତୁ ସତ୍ୟ
	ଫେରନ୍ତୁ U ଭିତରେ [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ଫେରନ୍ତୁ UISC == Joiner କିମ୍ବା UDI ଏବଂ UGC ଭିତରେ [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ ((UISC == Consonant_Final ଏବଂ UGC != Lo) କିମ୍ବା
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ଫେରନ୍ତୁ (UISC == Consonant_Medial ଏବଂ UGC != Lo କିମ୍ବା
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC ଭିତରେ [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Consonant_Subjoined ଏବଂ UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Virama ଏବଂ ନୁହେଁ is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ଫେରନ୍ତୁ U == 0x0DCA
def is_HALANT_ଅଙ୍କ(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == ସଂଖ୍ୟା_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC ଭିତରେ [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC ଭିତରେ [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ଫେରନ୍ତୁ (UISC == Invisible_Stacker
		ଏବଂ ନୁହେଁ is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ଫେରନ୍ତୁ ((UGC == Po କିମ୍ବା UISC ଭିତରେ [Consonant_Dead, Joiner, Modifying_Letter, Other])
		ଏବଂ ନୁହେଁ is_BASE(U, UISC, UDI, UGC, AJT)
		ଏବଂ ନୁହେଁ is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		ଏବଂ ନୁହେଁ is_CGJ(U, UISC, UDI, UGC, AJT)
		ଏବଂ ନୁହେଁ is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		ଏବଂ ନୁହେଁ is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC ଭିତରେ [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ଫେରନ୍ତୁ U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ UISC == ପ୍ରତୀକ_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ (UISC == Pure_Killer କିମ୍ବା
		UGC != Lo ଏବଂ UISC ଭିତରେ [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ଫେରନ୍ତୁ (UISC ଭିତରେ [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] କିମ୍ବା
		UGC != Lo ଏବଂ UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ଫେରନ୍ତୁ (UDI ଏବଂ U ନୁହେଁ ଭିତରେ [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		ଏବଂ UISC == Other
		ଏବଂ ନୁହେଁ is_CGJ(U, UISC, UDI, UGC, AJT)
	) କିମ୍ବା UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_ଅଙ୍କ,
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
	'HN':	is_HALANT_ଅଙ୍କ,
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
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom],
		'Pst': [ଡାହାଣ_2],
	},
	'M': {
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom, Bottom_And_ବାମ, Bottom_And_ଡାହାଣ],
		'Pst': [ଡାହାଣ_2],
		'Pre': [ବାମ, ଶୀର୍ଷ_And_Bottom_And_ବାମ],
	},
	'CM': {
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [ଶୀର୍ଷ, ଶୀର୍ଷ_And_Bottom, ଶୀର୍ଷ_And_Bottom_And_ଡାହାଣ, ଶୀର୍ଷ_And_ଡାହାଣ],
		'Blw': [Bottom, Overstruck, Bottom_And_ଡାହାଣ],
		'Pst': [ଡାହାଣ_2],
		'Pre': [ବାମ, ଶୀର୍ଷ_And_ବାମ, ଶୀର୍ଷ_And_ବାମ_And_ଡାହାଣ, ବାମ_And_ଡାହାଣ],
	},
	'VM': {
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom, Overstruck],
		'Pst': [ଡାହାଣ_2],
		'Pre': [ବାମ],
	},
	'SM': {
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [ଶୀର୍ଷ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	ବସ୍ତୁଗୁଡ଼ିକ = use_mapping.ବସ୍ତୁଗୁଡ଼ିକ()
	ପାଇଁ U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) ଭିତରେ data.ବସ୍ତୁଗୁଡ଼ିକ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ଯଦି 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ଯଦି 0x0F18 <= U <= 0x0F19 କିମ୍ବା 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		ଯଦି U == 0x1CED: UISC = Tone_Mark

		ମୂଲ୍ୟଗୁଡ଼ିକ = [k ପାଇଁ k,v ଭିତରେ ବସ୍ତୁଗୁଡ଼ିକ ଯଦି v(U, UISC, UDI, UGC, AJT)]
		ପ୍ରତିଜ୍ଞା len(ମୂଲ୍ୟଗୁଡ଼ିକ) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, ମୂଲ୍ୟଗୁଡ଼ିକ)
		USE = ମୂଲ୍ୟଗୁଡ଼ିକ[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		ଯଦି U ଭିତରେ [0x11302, 0x11303, 0x114C1]: UIPC = ଶୀର୍ଷ

		ପ୍ରତିଜ୍ଞା (UIPC ଭିତରେ [Not_Applicable, Visual_Order_ବାମ] କିମ୍ବା
			U == 0x0F7F କିମ୍ବା
			USE ଭିତରେ use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		ଯଦି pos_mapping:
			ମୂଲ୍ୟଗୁଡ଼ିକ = [k ପାଇଁ k,v ଭିତରେ pos_mapping.ବସ୍ତୁଗୁଡ଼ିକ() ଯଦି v ଏବଂ UIPC ଭିତରେ v]
			ପ୍ରତିଜ୍ଞା len(ମୂଲ୍ୟଗୁଡ଼ିକ) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, ମୂଲ୍ୟଗୁଡ଼ିକ)
			USE = USE + ମୂଲ୍ୟଗୁଡ଼ିକ[0]

		out[U] = (USE, UBlock)
	ଫେରନ୍ତୁ out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
ପାଇଁ h ଭିତରେ headers:
	ପାଇଁ l ଭିତରେ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
ଯଦି language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    ପାଇଁ k,v ଭିତରେ sorted(use_mapping.ବସ୍ତୁଗୁଡ଼ିକ()):
        ଯଦି k ଭିତରେ use_positions ଏବଂ use_positions[k]: ଜାରି
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    ପାଇଁ k,v ଭିତରେ sorted(use_positions.ବସ୍ତୁଗୁଡ଼ିକ()):
        ଯଦି ନୁହେଁ v: ଜାରି
        ପାଇଁ suf ଭିତରେ v.ଚାବିଗୁଡ଼ିକ():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

ନଚେତଯଦି language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
ଅନ୍ୟଥା:
    ପ୍ରତିଜ୍ଞା ମିଥ୍ୟା, "Unknown language: %s" % language.name

uu = sorted (use_data.ଚାବିଗୁଡ଼ିକ ())

data = {u:v[0] ପାଇଁ u,v ଭିତରେ use_data.ବସ୍ତୁଗୁଡ଼ିକ()}

ଯଦି language.name == "c":
    ବ୍ୟକ୍ତିଗତ = ସତ୍ୟ
ନଚେତଯଦି language.name == "rust":
    ବ୍ୟକ୍ତିଗତ = ମିଥ୍ୟା
    language.public_function_linkage = "pub(crate)"
ଅନ୍ୟଥା:
    ପ୍ରତିଜ୍ଞା ମିଥ୍ୟା, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
ଯଦି language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
ଅନ୍ୟଥା:
    modes[DEFAULT] = ""

ପାଇଁ step, text ଭିତରେ modes.ବସ୍ତୁଗୁଡ଼ିକ():
    print()
    ଯଦି text:
        print(text)
        print()
    ଯଦି step is None:
        ଜାରି

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, ପୂର୍ବନିର୍ଦ୍ଧାରିତ='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, ବ୍ୟକ୍ତିଗତ=ବ୍ୟକ୍ତିଗତ)
    code.print_code(language=language, ବ୍ୟକ୍ତିଗତ=ବ୍ୟକ୍ତିଗତ)
    print ()

ଯଦି language.name == "c":
    print ()
    ପାଇଁ k ଭିତରେ sorted(use_mapping.ଚାବିଗୁଡ଼ିକ()):
        ଯଦି k ଭିତରେ use_positions ଏବଂ use_positions[k]: ଜାରି
        print ("#undef %s" % k)
    ପାଇଁ k,v ଭିତରେ sorted(use_positions.ବସ୍ତୁଗୁଡ଼ିକ()):
        ଯଦି ନୁହେଁ v: ଜାରି
        ପାଇଁ suf ଭିତରେ v.ଚାବିଗୁଡ଼ିକ():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
ନଚେତଯଦି language.name == "rust":
    pass
ଅନ୍ୟଥା:
    ପ୍ରତିଜ୍ଞା ମିଥ୍ୟା, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
