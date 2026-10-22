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

আমদানি os
আমদানি packTab

আমদানি sys
আমদানি logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

যদি len(sys.argv) > 1 এবং sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
অন্যথা:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

আমদানি sys

যদি len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') জন্য x মধ্যে sys.argv[1:]]

headers = [[f.readline () জন্য i মধ্যে range (2)] জন্য j,f মধ্যে enumerate(files) যদি j != 4]
জন্য j মধ্যে range(7, 9):
	জন্য সারি মধ্যে files[j]:
		সারি = সারি.rstrip()
		যদি নয় সারি:
			বিরতি
		headers[j - 1].append(সারি)
headers.append (["UnicodeData.txt does not have a header."])

unicode_তথ্য = [{} জন্য _ মধ্যে files]
মানসমূহ = [{} জন্য _ মধ্যে files]
জন্য i, f মধ্যে enumerate (files):
	জন্য সারি মধ্যে f:

		j = সারি.find ('#')
		যদি j >= 0:
			সারি = সারি[:j]

		fields = [x.strip () জন্য x মধ্যে সারি.split (';')]
		যদি len (fields) == 1:
			চালু

		uu = fields[0].split ('..')
		start = পূর্ণসংখ্যা (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণসংখ্যা (uu[1], 16)

		t = fields[1 যদি i নয় মধ্যে [2, 4] অন্যথা 2]

		যদি i == 2:
			t = 'jt_' + t
		নতুবা_যদি i == 3 এবং t != 'Default_Ignorable_Code_Point':
			চালু
		নতুবা_যদি i == 7 এবং t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		নতুবা_যদি i == 8 এবং t == 'NA':
			t = 'Not_Applicable'

		i0 = i যদি i < 7 অন্যথা i - 7
		জন্য u মধ্যে range (start, end + 1):
			unicode_তথ্য[i0][u] = t
		মানসমূহ[i0][t] = মানসমূহ[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
জন্য i,v মধ্যে enumerate (defaults):
	মানসমূহ[i][v] = মানসমূহ[i].get (v, 0) + 1
combined = {}
জন্য i,d মধ্যে enumerate (unicode_তথ্য):
	জন্য u,v মধ্যে d.বস্তুসমূহ ():
		যদি নয় u মধ্যে combined:
			যদি i >= 4:
				চালু
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v জন্য k, v মধ্যে combined.বস্তুসমূহ() যদি v[6] নয় মধ্যে DISABLED_SCRIPTS}


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

শ্রেণী Property_মান(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		ফেরত self.name
	def __eq__(self, other):
		ফেরত self.name == (other যদি isinstance(other, str) অন্যথা other.name)
	def __ne__(self, other):
		ফেরত নয় (self == other)
	def __hash__(self):
		ফেরত hash(str(self))

property_মানসমূহ = {}

জন্য name মধ্যে property_names:
	মান_2 = Property_মান(name)
	প্রতিজ্ঞা মান_2 নয় মধ্যে property_মানসমূহ
	প্রতিজ্ঞা মান_2 নয় মধ্যে globals()
	property_মানসমূহ[name] = মান_2
globals().update(property_মানসমূহ)


def is_BASE(U, UISC, UDI, UGC, AJT):
	ফেরত (UISC মধ্যে [সংখ্যা, Consonant, Consonant_শীর্ষ_Letter,
			Tone_Letter,
			Vowel_Independent,
			] অথবা
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT মধ্যে [jt_C, jt_D, jt_L, jt_R] এবং UISC != Joiner অথবা
		(UGC == Lo এবং UISC মধ্যে [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_অঙ্ক(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Brahmi_Joining_সংখ্যা
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	যদি UISC == Consonant_Placeholder: ফেরত সত্য
	ফেরত U মধ্যে [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ফেরত UISC == Joiner অথবা UDI এবং UGC মধ্যে [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ফেরত ((UISC == Consonant_Final এবং UGC != Lo) অথবা
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ফেরত (UISC == Consonant_Medial এবং UGC != Lo অথবা
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ফেরত UISC মধ্যে [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Consonant_Subjoined এবং UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Virama এবং নয় is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ফেরত U == 0x0DCA
def is_HALANT_অঙ্ক(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == সংখ্যা_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ফেরত UISC মধ্যে [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	ফেরত UISC মধ্যে [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ফেরত (UISC == Invisible_Stacker
		এবং নয় is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ফেরত ((UGC == Po অথবা UISC মধ্যে [Consonant_Dead, Joiner, Modifying_Letter, Other])
		এবং নয় is_BASE(U, UISC, UDI, UGC, AJT)
		এবং নয় is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		এবং নয় is_CGJ(U, UISC, UDI, UGC, AJT)
		এবং নয় is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		এবং নয় is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	ফেরত UISC মধ্যে [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ফেরত U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ফেরত UISC == প্রতীক_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	ফেরত (UISC == Pure_Killer অথবা
		UGC != Lo এবং UISC মধ্যে [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ফেরত (UISC মধ্যে [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] অথবা
		UGC != Lo এবং UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ফেরত (UDI এবং U নয় মধ্যে [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		এবং UISC == Other
		এবং নয় is_CGJ(U, UISC, UDI, UGC, AJT)
	) অথবা UGC == Cn

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
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom],
		'Pst': [ডান_2],
	},
	'M': {
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom, Bottom_And_বাম, Bottom_And_ডান],
		'Pst': [ডান_2],
		'Pre': [বাম, শীর্ষ_And_Bottom_And_বাম],
	},
	'CM': {
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [শীর্ষ_3, শীর্ষ_And_Bottom, শীর্ষ_And_Bottom_And_ডান, শীর্ষ_And_ডান],
		'Blw': [Bottom, Overstruck, Bottom_And_ডান],
		'Pst': [ডান_2],
		'Pre': [বাম, শীর্ষ_And_বাম, শীর্ষ_And_বাম_And_ডান, বাম_And_ডান],
	},
	'VM': {
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom, Overstruck],
		'Pst': [ডান_2],
		'Pre': [বাম],
	},
	'SM': {
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [শীর্ষ_3],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(তথ্য):
	out = {}
	বস্তুসমূহ = use_mapping.বস্তুসমূহ()
	জন্য U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) মধ্যে তথ্য.বস্তুসমূহ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		যদি 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		যদি 0x0F18 <= U <= 0x0F19 অথবা 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		যদি U == 0x1CED: UISC = Tone_Mark

		মানসমূহ = [k জন্য k,v মধ্যে বস্তুসমূহ যদি v(U, UISC, UDI, UGC, AJT)]
		প্রতিজ্ঞা len(মানসমূহ) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, মানসমূহ)
		USE = মানসমূহ[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		যদি U মধ্যে [0x11302, 0x11303, 0x114C1]: UIPC = শীর্ষ_3

		প্রতিজ্ঞা (UIPC মধ্যে [Not_Applicable, Visual_Order_বাম] অথবা
			U == 0x0F7F অথবা
			USE মধ্যে use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		যদি pos_mapping:
			মানসমূহ = [k জন্য k,v মধ্যে pos_mapping.বস্তুসমূহ() যদি v এবং UIPC মধ্যে v]
			প্রতিজ্ঞা len(মানসমূহ) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, মানসমূহ)
			USE = USE + মানসমূহ[0]

		out[U] = (USE, UBlock)
	ফেরত out

use_তথ্য = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
জন্য h মধ্যে headers:
	জন্য l মধ্যে h:
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
    জন্য k,v মধ্যে sorted(use_mapping.বস্তুসমূহ()):
        যদি k মধ্যে use_positions এবং use_positions[k]: চালু
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    জন্য k,v মধ্যে sorted(use_positions.বস্তুসমূহ()):
        যদি নয় v: চালু
        জন্য suf মধ্যে v.চাবিগুলি():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

নতুবা_যদি language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
অন্যথা:
    প্রতিজ্ঞা মিথ্যা, "Unknown language: %s" % language.name

uu = sorted (use_তথ্য.চাবিগুলি ())

তথ্য = {u:v[0] জন্য u,v মধ্যে use_তথ্য.বস্তুসমূহ()}

যদি language.name == "c":
    ব্যক্তিগত = সত্য
নতুবা_যদি language.name == "rust":
    ব্যক্তিগত = মিথ্যা
    language.public_function_linkage = "pub(crate)"
অন্যথা:
    প্রতিজ্ঞা মিথ্যা, "Unknown language: %s" % language.name


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

জন্য step, text মধ্যে modes.বস্তুসমূহ():
    print()
    যদি text:
        print(text)
        print()
    যদি step is None:
        চালু

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(তথ্য, compression=compression, পূর্বনির্ধারিত='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, ব্যক্তিগত=ব্যক্তিগত)
    code.print_code(language=language, ব্যক্তিগত=ব্যক্তিগত)
    print ()

যদি language.name == "c":
    print ()
    জন্য k মধ্যে sorted(use_mapping.চাবিগুলি()):
        যদি k মধ্যে use_positions এবং use_positions[k]: চালু
        print ("#undef %s" % k)
    জন্য k,v মধ্যে sorted(use_positions.বস্তুসমূহ()):
        যদি নয় v: চালু
        জন্য suf মধ্যে v.চাবিগুলি():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
নতুবা_যদি language.name == "rust":
    pass
অন্যথা:
    প্রতিজ্ঞা মিথ্যা, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
