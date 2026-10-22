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

আনযন os
আনযন packTab

আনযন sys
আনযন logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

যদি len(sys.argv) > 1 আৰু sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
অন্যথা:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

আনযন sys

যদি len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') বাবে x অন্তঃ sys.argv[1:]]

headers = [[f.readline () বাবে i অন্তঃ range (2)] বাবে j,f অন্তঃ enumerate(files) যদি j != 4]
বাবে j অন্তঃ range(7, 9):
	বাবে সাৰি অন্তঃ files[j]:
		সাৰি = সাৰি.rstrip()
		যদি নহয় সাৰি:
			ভঙ্গ
		headers[j - 1].append(সাৰি)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} বাবে _ অন্তঃ files]
মানসমূহ = [{} বাবে _ অন্তঃ files]
বাবে i, f অন্তঃ enumerate (files):
	বাবে সাৰি অন্তঃ f:

		j = সাৰি.find ('#')
		যদি j >= 0:
			সাৰি = সাৰি[:j]

		fields = [x.strip () বাবে x অন্তঃ সাৰি.split (';')]
		যদি len (fields) == 1:
			অনুৱর্ত

		uu = fields[0].split ('..')
		start = পূর্ণাঙ্ক (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণাঙ্ক (uu[1], 16)

		t = fields[1 যদি i নহয় অন্তঃ [2, 4] অন্যথা 2]

		যদি i == 2:
			t = 'jt_' + t
		নোচেদ্যদি i == 3 আৰু t != 'Default_Ignorable_Code_Point':
			অনুৱর্ত
		নোচেদ্যদি i == 7 আৰু t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		নোচেদ্যদি i == 8 আৰু t == 'NA':
			t = 'Not_Applicable'

		i0 = i যদি i < 7 অন্যথা i - 7
		বাবে u অন্তঃ range (start, end + 1):
			unicode_data[i0][u] = t
		মানসমূহ[i0][t] = মানসমূহ[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
বাবে i,v অন্তঃ enumerate (defaults):
	মানসমূহ[i][v] = মানসমূহ[i].get (v, 0) + 1
combined = {}
বাবে i,d অন্তঃ enumerate (unicode_data):
	বাবে u,v অন্তঃ d.বস্তুসমূহ ():
		যদি নহয় u অন্তঃ combined:
			যদি i >= 4:
				অনুৱর্ত
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v বাবে k, v অন্তঃ combined.বস্তুসমূহ() যদি v[6] নহয় অন্তঃ DISABLED_SCRIPTS}


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

শ্ৰেণী Property_মান(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		ঘূৰাই self.name
	def __eq__(self, other):
		ঘূৰাই self.name == (other যদি isinstance(other, str) অন্যথা other.name)
	def __ne__(self, other):
		ঘূৰাই নহয় (self == other)
	def __hash__(self):
		ঘূৰাই hash(str(self))

property_মানসমূহ = {}

বাবে name অন্তঃ property_names:
	মান_2 = Property_মান(name)
	প্রতিজ্ঞা মান_2 নহয় অন্তঃ property_মানসমূহ
	প্রতিজ্ঞা মান_2 নহয় অন্তঃ globals()
	property_মানসমূহ[name] = মান_2
globals().update(property_মানসমূহ)


def is_BASE(U, UISC, UDI, UGC, AJT):
	ঘূৰাই (UISC অন্তঃ [সংখ্যা, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] বা
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT অন্তঃ [jt_C, jt_D, jt_L, jt_R] আৰু UISC != Joiner বা
		(UGC == Lo আৰু UISC অন্তঃ [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_অঙ্ক(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Brahmi_Joining_সংখ্যা
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	যদি UISC == Consonant_Placeholder: ঘূৰাই সঁচা
	ঘূৰাই U অন্তঃ [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ঘূৰাই UISC == Joiner বা UDI আৰু UGC অন্তঃ [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ঘূৰাই ((UISC == Consonant_Final আৰু UGC != Lo) বা
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ঘূৰাই (UISC == Consonant_Medial আৰু UGC != Lo বা
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC অন্তঃ [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Consonant_Subjoined আৰু UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Virama আৰু নহয় is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ঘূৰাই U == 0x0DCA
def is_HALANT_অঙ্ক(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == সংখ্যা_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC অন্তঃ [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC অন্তঃ [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ঘূৰাই (UISC == Invisible_Stacker
		আৰু নহয় is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ঘূৰাই ((UGC == Po বা UISC অন্তঃ [Consonant_Dead, Joiner, Modifying_Letter, Other])
		আৰু নহয় is_BASE(U, UISC, UDI, UGC, AJT)
		আৰু নহয় is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		আৰু নহয় is_CGJ(U, UISC, UDI, UGC, AJT)
		আৰু নহয় is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		আৰু নহয় is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC অন্তঃ [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ঘূৰাই U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ঘূৰাই UISC == প্ৰতীক_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	ঘূৰাই (UISC == Pure_Killer বা
		UGC != Lo আৰু UISC অন্তঃ [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ঘূৰাই (UISC অন্তঃ [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] বা
		UGC != Lo আৰু UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ঘূৰাই (UDI আৰু U নহয় অন্তঃ [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		আৰু UISC == Other
		আৰু নহয় is_CGJ(U, UISC, UDI, UGC, AJT)
	) বা UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_অঙ্ক,
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
	'HN':	is_HALANT_অঙ্ক,
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
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom],
		'Pst': [ডান_2],
	},
	'M': {
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom, Bottom_And_বাম, Bottom_And_ডান],
		'Pst': [ডান_2],
		'Pre': [বাম, শীৰ্ষ_And_Bottom_And_বাম],
	},
	'CM': {
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [শীৰ্ষ, শীৰ্ষ_And_Bottom, শীৰ্ষ_And_Bottom_And_ডান, শীৰ্ষ_And_ডান],
		'Blw': [Bottom, Overstruck, Bottom_And_ডান],
		'Pst': [ডান_2],
		'Pre': [বাম, শীৰ্ষ_And_বাম, শীৰ্ষ_And_বাম_And_ডান, বাম_And_ডান],
	},
	'VM': {
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom, Overstruck],
		'Pst': [ডান_2],
		'Pre': [বাম],
	},
	'SM': {
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [শীৰ্ষ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	বস্তুসমূহ = use_mapping.বস্তুসমূহ()
	বাবে U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) অন্তঃ data.বস্তুসমূহ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		যদি 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		যদি 0x0F18 <= U <= 0x0F19 বা 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		যদি U == 0x1CED: UISC = Tone_Mark

		মানসমূহ = [k বাবে k,v অন্তঃ বস্তুসমূহ যদি v(U, UISC, UDI, UGC, AJT)]
		প্রতিজ্ঞা len(মানসমূহ) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, মানসমূহ)
		USE = মানসমূহ[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		যদি U অন্তঃ [0x11302, 0x11303, 0x114C1]: UIPC = শীৰ্ষ

		প্রতিজ্ঞা (UIPC অন্তঃ [Not_Applicable, Visual_Order_বাম] বা
			U == 0x0F7F বা
			USE অন্তঃ use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		যদি pos_mapping:
			মানসমূহ = [k বাবে k,v অন্তঃ pos_mapping.বস্তুসমূহ() যদি v আৰু UIPC অন্তঃ v]
			প্রতিজ্ঞা len(মানসমূহ) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, মানসমূহ)
			USE = USE + মানসমূহ[0]

		out[U] = (USE, UBlock)
	ঘূৰাই out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
বাবে h অন্তঃ headers:
	বাবে l অন্তঃ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
যদি language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    বাবে k,v অন্তঃ sorted(use_mapping.বস্তুসমূহ()):
        যদি k অন্তঃ use_positions আৰু use_positions[k]: অনুৱর্ত
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    বাবে k,v অন্তঃ sorted(use_positions.বস্তুসমূহ()):
        যদি নহয় v: অনুৱর্ত
        বাবে suf অন্তঃ v.চাবিগুলি():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

নোচেদ্যদি language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
অন্যথা:
    প্রতিজ্ঞা মিছা, "Unknown language: %s" % language.name

uu = sorted (use_data.চাবিগুলি ())

data = {u:v[0] বাবে u,v অন্তঃ use_data.বস্তুসমূহ()}

যদি language.name == "c":
    গুপ্ত = সঁচা
নোচেদ্যদি language.name == "rust":
    গুপ্ত = মিছা
    language.public_function_linkage = "pub(crate)"
অন্যথা:
    প্রতিজ্ঞা মিছা, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
যদি language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
অন্যথা:
    modes[DEFAULT] = ""

বাবে step, text অন্তঃ modes.বস্তুসমূহ():
    print()
    যদি text:
        print(text)
        print()
    যদি step is None:
        অনুৱর্ত

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, স্ৱভাৱ='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, গুপ্ত=গুপ্ত)
    code.print_code(language=language, গুপ্ত=গুপ্ত)
    print ()

যদি language.name == "c":
    print ()
    বাবে k অন্তঃ sorted(use_mapping.চাবিগুলি()):
        যদি k অন্তঃ use_positions আৰু use_positions[k]: অনুৱর্ত
        print ("#undef %s" % k)
    বাবে k,v অন্তঃ sorted(use_positions.বস্তুসমূহ()):
        যদি নহয় v: অনুৱর্ত
        বাবে suf অন্তঃ v.চাবিগুলি():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
নোচেদ্যদি language.name == "rust":
    pass
অন্যথা:
    প্রতিজ্ঞা মিছা, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
