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

ഇറക്കുമതി os
ഇറക്കുമതി packTab

ഇറക്കുമതി sys
ഇറക്കുമതി logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

എങ്കിൽ len(sys.argv) > 1 ഒപ്പം sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
അല്ലെങ്കിൽ:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

ഇറക്കുമതി sys

എങ്കിൽ len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') വേണ്ടി x ഇൽ sys.argv[1:]]

headers = [[f.readline () വേണ്ടി i ഇൽ range (2)] വേണ്ടി j,f ഇൽ enumerate(files) എങ്കിൽ j != 4]
വേണ്ടി j ഇൽ range(7, 9):
	വേണ്ടി വരി ഇൽ files[j]:
		വരി = വരി.rstrip()
		എങ്കിൽ അല്ല വരി:
			നിർത്തുക
		headers[j - 1].append(വരി)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} വേണ്ടി _ ഇൽ files]
മൂല്യങ്ങൾ = [{} വേണ്ടി _ ഇൽ files]
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

		t = fields[1 എങ്കിൽ i അല്ല ഇൽ [2, 4] അല്ലെങ്കിൽ 2]

		എങ്കിൽ i == 2:
			t = 'jt_' + t
		അല്ലെങ്കിലെങ്കിൽ i == 3 ഒപ്പം t != 'Default_Ignorable_Code_Point':
			തുടരുക
		അല്ലെങ്കിലെങ്കിൽ i == 7 ഒപ്പം t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		അല്ലെങ്കിലെങ്കിൽ i == 8 ഒപ്പം t == 'NA':
			t = 'Not_Applicable'

		i0 = i എങ്കിൽ i < 7 അല്ലെങ്കിൽ i - 7
		വേണ്ടി u ഇൽ range (start, end + 1):
			unicode_data[i0][u] = t
		മൂല്യങ്ങൾ[i0][t] = മൂല്യങ്ങൾ[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
വേണ്ടി i,v ഇൽ enumerate (defaults):
	മൂല്യങ്ങൾ[i][v] = മൂല്യങ്ങൾ[i].get (v, 0) + 1
combined = {}
വേണ്ടി i,d ഇൽ enumerate (unicode_data):
	വേണ്ടി u,v ഇൽ d.ഇനങ്ങൾ ():
		എങ്കിൽ അല്ല u ഇൽ combined:
			എങ്കിൽ i >= 4:
				തുടരുക
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v വേണ്ടി k, v ഇൽ combined.ഇനങ്ങൾ() എങ്കിൽ v[6] അല്ല ഇൽ DISABLED_SCRIPTS}


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

വർഗ്ഗം Property_മൂല്യം(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		തിരികെ self.name
	def __eq__(self, other):
		തിരികെ self.name == (other എങ്കിൽ isinstance(other, str) അല്ലെങ്കിൽ other.name)
	def __ne__(self, other):
		തിരികെ അല്ല (self == other)
	def __hash__(self):
		തിരികെ hash(str(self))

property_മൂല്യങ്ങൾ = {}

വേണ്ടി name ഇൽ property_names:
	മൂല്യം_2 = Property_മൂല്യം(name)
	പ്രതിജ്ഞാ മൂല്യം_2 അല്ല ഇൽ property_മൂല്യങ്ങൾ
	പ്രതിജ്ഞാ മൂല്യം_2 അല്ല ഇൽ globals()
	property_മൂല്യങ്ങൾ[name] = മൂല്യം_2
globals().update(property_മൂല്യങ്ങൾ)


def is_BASE(U, UISC, UDI, UGC, AJT):
	തിരികെ (UISC ഇൽ [സംഖ്യ, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] അഥവാ
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT ഇൽ [jt_C, jt_D, jt_L, jt_R] ഒപ്പം UISC != Joiner അഥവാ
		(UGC == Lo ഒപ്പം UISC ഇൽ [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_അക്കം(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Brahmi_Joining_സംഖ്യ
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	എങ്കിൽ UISC == Consonant_Placeholder: തിരികെ സത്യം
	തിരികെ U ഇൽ [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	തിരികെ UISC == Joiner അഥവാ UDI ഒപ്പം UGC ഇൽ [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	തിരികെ ((UISC == Consonant_Final ഒപ്പം UGC != Lo) അഥവാ
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	തിരികെ (UISC == Consonant_Medial ഒപ്പം UGC != Lo അഥവാ
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC ഇൽ [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Consonant_Subjoined ഒപ്പം UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Virama ഒപ്പം അല്ല is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	തിരികെ U == 0x0DCA
def is_HALANT_അക്കം(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == സംഖ്യ_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC ഇൽ [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC ഇൽ [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	തിരികെ (UISC == Invisible_Stacker
		ഒപ്പം അല്ല is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	തിരികെ ((UGC == Po അഥവാ UISC ഇൽ [Consonant_Dead, Joiner, Modifying_Letter, Other])
		ഒപ്പം അല്ല is_BASE(U, UISC, UDI, UGC, AJT)
		ഒപ്പം അല്ല is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		ഒപ്പം അല്ല is_CGJ(U, UISC, UDI, UGC, AJT)
		ഒപ്പം അല്ല is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		ഒപ്പം അല്ല is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC ഇൽ [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	തിരികെ U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	തിരികെ UISC == ചിഹ്നം_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	തിരികെ (UISC == Pure_Killer അഥവാ
		UGC != Lo ഒപ്പം UISC ഇൽ [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	തിരികെ (UISC ഇൽ [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] അഥവാ
		UGC != Lo ഒപ്പം UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	തിരികെ (UDI ഒപ്പം U അല്ല ഇൽ [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		ഒപ്പം UISC == Other
		ഒപ്പം അല്ല is_CGJ(U, UISC, UDI, UGC, AJT)
	) അഥവാ UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_അക്കം,
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
	'HN':	is_HALANT_അക്കം,
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
		'Abv': [മുകൾ],
		'Blw': [Bottom],
		'Pst': [വലത്_2],
	},
	'M': {
		'Abv': [മുകൾ],
		'Blw': [Bottom, Bottom_And_ഇടത്, Bottom_And_വലത്],
		'Pst': [വലത്_2],
		'Pre': [ഇടത്, മുകൾ_And_Bottom_And_ഇടത്],
	},
	'CM': {
		'Abv': [മുകൾ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [മുകൾ, മുകൾ_And_Bottom, മുകൾ_And_Bottom_And_വലത്, മുകൾ_And_വലത്],
		'Blw': [Bottom, Overstruck, Bottom_And_വലത്],
		'Pst': [വലത്_2],
		'Pre': [ഇടത്, മുകൾ_And_ഇടത്, മുകൾ_And_ഇടത്_And_വലത്, ഇടത്_And_വലത്],
	},
	'VM': {
		'Abv': [മുകൾ],
		'Blw': [Bottom, Overstruck],
		'Pst': [വലത്_2],
		'Pre': [ഇടത്],
	},
	'SM': {
		'Abv': [മുകൾ],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [മുകൾ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	ഇനങ്ങൾ = use_mapping.ഇനങ്ങൾ()
	വേണ്ടി U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) ഇൽ data.ഇനങ്ങൾ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		എങ്കിൽ 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		എങ്കിൽ 0x0F18 <= U <= 0x0F19 അഥവാ 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		എങ്കിൽ U == 0x1CED: UISC = Tone_Mark

		മൂല്യങ്ങൾ = [k വേണ്ടി k,v ഇൽ ഇനങ്ങൾ എങ്കിൽ v(U, UISC, UDI, UGC, AJT)]
		പ്രതിജ്ഞാ len(മൂല്യങ്ങൾ) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, മൂല്യങ്ങൾ)
		USE = മൂല്യങ്ങൾ[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		എങ്കിൽ U ഇൽ [0x11302, 0x11303, 0x114C1]: UIPC = മുകൾ

		പ്രതിജ്ഞാ (UIPC ഇൽ [Not_Applicable, Visual_Order_ഇടത്] അഥവാ
			U == 0x0F7F അഥവാ
			USE ഇൽ use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		എങ്കിൽ pos_mapping:
			മൂല്യങ്ങൾ = [k വേണ്ടി k,v ഇൽ pos_mapping.ഇനങ്ങൾ() എങ്കിൽ v ഒപ്പം UIPC ഇൽ v]
			പ്രതിജ്ഞാ len(മൂല്യങ്ങൾ) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, മൂല്യങ്ങൾ)
			USE = USE + മൂല്യങ്ങൾ[0]

		out[U] = (USE, UBlock)
	തിരികെ out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
വേണ്ടി h ഇൽ headers:
	വേണ്ടി l ഇൽ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
എങ്കിൽ language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    വേണ്ടി k,v ഇൽ sorted(use_mapping.ഇനങ്ങൾ()):
        എങ്കിൽ k ഇൽ use_positions ഒപ്പം use_positions[k]: തുടരുക
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    വേണ്ടി k,v ഇൽ sorted(use_positions.ഇനങ്ങൾ()):
        എങ്കിൽ അല്ല v: തുടരുക
        വേണ്ടി suf ഇൽ v.താക്കോലുകൾ():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
അല്ലെങ്കിൽ:
    പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name

uu = sorted (use_data.താക്കോലുകൾ ())

data = {u:v[0] വേണ്ടി u,v ഇൽ use_data.ഇനങ്ങൾ()}

എങ്കിൽ language.name == "c":
    സ്വകാര്യ = സത്യം
അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
    സ്വകാര്യ = അസത്യം
    language.public_function_linkage = "pub(crate)"
അല്ലെങ്കിൽ:
    പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
എങ്കിൽ language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
അല്ലെങ്കിൽ:
    modes[DEFAULT] = ""

വേണ്ടി step, text ഇൽ modes.ഇനങ്ങൾ():
    print()
    എങ്കിൽ text:
        print(text)
        print()
    എങ്കിൽ step is None:
        തുടരുക

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, സ്ഥിരസ്ഥിതി='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, സ്വകാര്യ=സ്വകാര്യ)
    code.print_code(language=language, സ്വകാര്യ=സ്വകാര്യ)
    print ()

എങ്കിൽ language.name == "c":
    print ()
    വേണ്ടി k ഇൽ sorted(use_mapping.താക്കോലുകൾ()):
        എങ്കിൽ k ഇൽ use_positions ഒപ്പം use_positions[k]: തുടരുക
        print ("#undef %s" % k)
    വേണ്ടി k,v ഇൽ sorted(use_positions.ഇനങ്ങൾ()):
        എങ്കിൽ അല്ല v: തുടരുക
        വേണ്ടി suf ഇൽ v.താക്കോലുകൾ():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
    pass
അല്ലെങ്കിൽ:
    പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
