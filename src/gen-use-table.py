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

واردکن os
واردکن packTab

واردکن sys
واردکن logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

اگر len(sys.argv) > 1 و sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وگرنه:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

واردکن sys

اگر len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') برای x در sys.argv[1:]]

headers = [[f.readline () برای i در range (2)] برای j,f در enumerate(files) اگر j != 4]
برای j در range(7, 9):
	برای سطر در files[j]:
		سطر = سطر.rstrip()
		اگر نه سطر:
			بشکن
		headers[j - 1].append(سطر)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} برای _ در files]
مقادیر = [{} برای _ در files]
برای i, f در enumerate (files):
	برای سطر در f:

		j = سطر.find ('#')
		اگر j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () برای x در سطر.split (';')]
		اگر len (fields) == 1:
			ادامه

		uu = fields[0].split ('..')
		start = صحیح (uu[0], 16)
		اگر len (uu) == 1:
			end = start
		وگرنه:
			end = صحیح (uu[1], 16)

		t = fields[1 اگر i نه در [2, 4] وگرنه 2]

		اگر i == 2:
			t = 'jt_' + t
		وگرنهاگر i == 3 و t != 'Default_Ignorable_Code_Point':
			ادامه
		وگرنهاگر i == 7 و t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		وگرنهاگر i == 8 و t == 'NA':
			t = 'Not_Applicable'

		i0 = i اگر i < 7 وگرنه i - 7
		برای u در range (start, end + 1):
			unicode_data[i0][u] = t
		مقادیر[i0][t] = مقادیر[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
برای i,v در enumerate (defaults):
	مقادیر[i][v] = مقادیر[i].get (v, 0) + 1
combined = {}
برای i,d در enumerate (unicode_data):
	برای u,v در d.اقلام ():
		اگر نه u در combined:
			اگر i >= 4:
				ادامه
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v برای k, v در combined.اقلام() اگر v[6] نه در DISABLED_SCRIPTS}


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

رده Property_مقدار(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		بازگشت self.name
	def __eq__(self, other):
		بازگشت self.name == (other اگر isinstance(other, str) وگرنه other.name)
	def __ne__(self, other):
		بازگشت نه (self == other)
	def __hash__(self):
		بازگشت hash(str(self))

property_مقادیر = {}

برای name در property_names:
	مقدار_2 = Property_مقدار(name)
	أكد مقدار_2 نه در property_مقادیر
	أكد مقدار_2 نه در globals()
	property_مقادیر[name] = مقدار_2
globals().update(property_مقادیر)


def is_BASE(U, UISC, UDI, UGC, AJT):
	بازگشت (UISC در [عدد, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] یا
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT در [jt_C, jt_D, jt_L, jt_R] و UISC != Joiner یا
		(UGC == Lo و UISC در [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_رقم(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Brahmi_Joining_عدد
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	اگر UISC == Consonant_Placeholder: بازگشت درست
	بازگشت U در [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	بازگشت UISC == Joiner یا UDI و UGC در [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	بازگشت ((UISC == Consonant_Final و UGC != Lo) یا
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	بازگشت (UISC == Consonant_Medial و UGC != Lo یا
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	بازگشت UISC در [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Consonant_Subjoined و UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Virama و نه is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	بازگشت U == 0x0DCA
def is_HALANT_رقم(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == عدد_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	بازگشت UISC در [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	بازگشت UISC در [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	بازگشت (UISC == Invisible_Stacker
		و نه is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	بازگشت ((UGC == Po یا UISC در [Consonant_Dead, Joiner, Modifying_Letter, Other])
		و نه is_BASE(U, UISC, UDI, UGC, AJT)
		و نه is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		و نه is_CGJ(U, UISC, UDI, UGC, AJT)
		و نه is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		و نه is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	بازگشت UISC در [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	بازگشت U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	بازگشت UISC == نماد_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	بازگشت (UISC == Pure_Killer یا
		UGC != Lo و UISC در [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	بازگشت (UISC در [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] یا
		UGC != Lo و UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	بازگشت (UDI و U نه در [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		و UISC == Other
		و نه is_CGJ(U, UISC, UDI, UGC, AJT)
	) یا UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_رقم,
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
	'HN':	is_HALANT_رقم,
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
		'Abv': [بالاترین],
		'Blw': [Bottom],
		'Pst': [راست_2],
	},
	'M': {
		'Abv': [بالاترین],
		'Blw': [Bottom, Bottom_And_چپ, Bottom_And_راست],
		'Pst': [راست_2],
		'Pre': [چپ, بالاترین_And_Bottom_And_چپ],
	},
	'CM': {
		'Abv': [بالاترین],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [بالاترین, بالاترین_And_Bottom, بالاترین_And_Bottom_And_راست, بالاترین_And_راست],
		'Blw': [Bottom, Overstruck, Bottom_And_راست],
		'Pst': [راست_2],
		'Pre': [چپ, بالاترین_And_چپ, بالاترین_And_چپ_And_راست, چپ_And_راست],
	},
	'VM': {
		'Abv': [بالاترین],
		'Blw': [Bottom, Overstruck],
		'Pst': [راست_2],
		'Pre': [چپ],
	},
	'SM': {
		'Abv': [بالاترین],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [بالاترین],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	اقلام = use_mapping.اقلام()
	برای U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) در data.اقلام():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		اگر 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		اگر 0x0F18 <= U <= 0x0F19 یا 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		اگر U == 0x1CED: UISC = Tone_Mark

		مقادیر = [k برای k,v در اقلام اگر v(U, UISC, UDI, UGC, AJT)]
		أكد len(مقادیر) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, مقادیر)
		USE = مقادیر[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		اگر U در [0x11302, 0x11303, 0x114C1]: UIPC = بالاترین

		أكد (UIPC در [Not_Applicable, Visual_Order_چپ] یا
			U == 0x0F7F یا
			USE در use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		اگر pos_mapping:
			مقادیر = [k برای k,v در pos_mapping.اقلام() اگر v و UIPC در v]
			أكد len(مقادیر) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, مقادیر)
			USE = USE + مقادیر[0]

		out[U] = (USE, UBlock)
	بازگشت out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
برای h در headers:
	برای l در h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
اگر language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    برای k,v در sorted(use_mapping.اقلام()):
        اگر k در use_positions و use_positions[k]: ادامه
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    برای k,v در sorted(use_positions.اقلام()):
        اگر نه v: ادامه
        برای suf در v.کلیدها():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

وگرنهاگر language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
وگرنه:
    أكد نادرست, "Unknown language: %s" % language.name

uu = sorted (use_data.کلیدها ())

data = {u:v[0] برای u,v در use_data.اقلام()}

اگر language.name == "c":
    خصوصی = درست
وگرنهاگر language.name == "rust":
    خصوصی = نادرست
    language.public_function_linkage = "pub(crate)"
وگرنه:
    أكد نادرست, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
اگر language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
وگرنه:
    modes[DEFAULT] = ""

برای step, text در modes.اقلام():
    print()
    اگر text:
        print(text)
        print()
    اگر step is None:
        ادامه

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, پیشفرض='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, خصوصی=خصوصی)
    code.print_code(language=language, خصوصی=خصوصی)
    print ()

اگر language.name == "c":
    print ()
    برای k در sorted(use_mapping.کلیدها()):
        اگر k در use_positions و use_positions[k]: ادامه
        print ("#undef %s" % k)
    برای k,v در sorted(use_positions.اقلام()):
        اگر نه v: ادامه
        برای suf در v.کلیدها():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وگرنهاگر language.name == "rust":
    pass
وگرنه:
    أكد نادرست, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
