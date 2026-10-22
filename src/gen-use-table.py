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

استورد os
استورد packTab

استورد sys
استورد logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

𐡄𐡍 len(sys.argv) > 1 𐡅 sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وإلا:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

استورد sys

𐡄𐡍 len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

headers = [[f.readline () لكل i في range (2)] لكل j,f في enumerate(files) 𐡄𐡍 j != 4]
لكل j في range(7, 9):
	لكل line في files[j]:
		line = line.rstrip()
		𐡄𐡍 𐡋𐡀 line:
			اكسر
		headers[j - 1].append(line)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} لكل _ في files]
values = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل line في f:

		j = line.find ('#')
		𐡄𐡍 j >= 0:
			line = line[:j]

		fields = [x.strip () لكل x في line.split (';')]
		𐡄𐡍 len (fields) == 1:
			استمر

		uu = fields[0].split ('..')
		start = صحيح (uu[0], 16)
		𐡄𐡍 len (uu) == 1:
			end = start
		وإلا:
			end = صحيح (uu[1], 16)

		t = fields[1 𐡄𐡍 i 𐡋𐡀 في [2, 4] وإلا 2]

		𐡄𐡍 i == 2:
			t = 'jt_' + t
		وإلاإذا i == 3 𐡅 t != 'Default_Ignorable_Code_Point':
			استمر
		وإلاإذا i == 7 𐡅 t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		وإلاإذا i == 8 𐡅 t == 'NA':
			t = 'Not_Applicable'

		i0 = i 𐡄𐡍 i < 7 وإلا i - 7
		لكل u في range (start, end + 1):
			unicode_data[i0][u] = t
		values[i0][t] = values[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
لكل i,v في enumerate (defaults):
	values[i][v] = values[i].get (v, 0) + 1
combined = {}
لكل i,d في enumerate (unicode_data):
	لكل u,v في d.items ():
		𐡄𐡍 𐡋𐡀 u في combined:
			𐡄𐡍 i >= 4:
				استمر
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v لكل k, v في combined.items() 𐡄𐡍 v[6] 𐡋𐡀 في DISABLED_SCRIPTS}


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

صنف PropertyValue(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		أرجع self.name
	def __eq__(self, other):
		أرجع self.name == (other 𐡄𐡍 isinstance(other, str) وإلا other.name)
	def __ne__(self, other):
		أرجع 𐡋𐡀 (self == other)
	def __hash__(self):
		أرجع hash(str(self))

property_values = {}

لكل name في property_names:
	value = PropertyValue(name)
	أكد value 𐡋𐡀 في property_values
	أكد value 𐡋𐡀 في globals()
	property_values[name] = value
globals().update(property_values)


def is_BASE(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Number, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] 𐡀𐡅
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT في [jt_C, jt_D, jt_L, jt_R] 𐡅 UISC != Joiner 𐡀𐡅
		(UGC == Lo 𐡅 UISC في [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_NUM(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Brahmi_Joining_Number
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	𐡄𐡍 UISC == Consonant_Placeholder: أرجع 𐡒𐡔𐡈
	أرجع U في [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	أرجع UISC == Joiner 𐡀𐡅 UDI 𐡅 UGC في [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	أرجع ((UISC == Consonant_Final 𐡅 UGC != Lo) 𐡀𐡅
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	أرجع (UISC == Consonant_Medial 𐡅 UGC != Lo 𐡀𐡅
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_Subjoined 𐡅 UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Virama 𐡅 𐡋𐡀 is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x0DCA
def is_HALANT_NUM(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Number_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع (UISC == Invisible_Stacker
		𐡅 𐡋𐡀 is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	أرجع ((UGC == Po 𐡀𐡅 UISC في [Consonant_Dead, Joiner, Modifying_Letter, Other])
		𐡅 𐡋𐡀 is_BASE(U, UISC, UDI, UGC, AJT)
		𐡅 𐡋𐡀 is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		𐡅 𐡋𐡀 is_CGJ(U, UISC, UDI, UGC, AJT)
		𐡅 𐡋𐡀 is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		𐡅 𐡋𐡀 is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Symbol_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	أرجع (UISC == Pure_Killer 𐡀𐡅
		UGC != Lo 𐡅 UISC في [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] 𐡀𐡅
		UGC != Lo 𐡅 UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	أرجع (UDI 𐡅 U 𐡋𐡀 في [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		𐡅 UISC == Other
		𐡅 𐡋𐡀 is_CGJ(U, UISC, UDI, UGC, AJT)
	) 𐡀𐡅 UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_NUM,
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
	'HN':	is_HALANT_NUM,
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
		'Abv': [Top],
		'Blw': [Bottom],
		'Pst': [Right],
	},
	'M': {
		'Abv': [Top],
		'Blw': [Bottom, Bottom_And_Left, Bottom_And_Right],
		'Pst': [Right],
		'Pre': [Left, Top_And_Bottom_And_Left],
	},
	'CM': {
		'Abv': [Top],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [Top, Top_And_Bottom, Top_And_Bottom_And_Right, Top_And_Right],
		'Blw': [Bottom, Overstruck, Bottom_And_Right],
		'Pst': [Right],
		'Pre': [Left, Top_And_Left, Top_And_Left_And_Right, Left_And_Right],
	},
	'VM': {
		'Abv': [Top],
		'Blw': [Bottom, Overstruck],
		'Pst': [Right],
		'Pre': [Left],
	},
	'SM': {
		'Abv': [Top],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [Top],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	items = use_mapping.items()
	لكل U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) في data.items():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		𐡄𐡍 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		𐡄𐡍 0x0F18 <= U <= 0x0F19 𐡀𐡅 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		𐡄𐡍 U == 0x1CED: UISC = Tone_Mark

		values = [k لكل k,v في items 𐡄𐡍 v(U, UISC, UDI, UGC, AJT)]
		أكد len(values) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, values)
		USE = values[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		𐡄𐡍 U في [0x11302, 0x11303, 0x114C1]: UIPC = Top

		أكد (UIPC في [Not_Applicable, Visual_Order_Left] 𐡀𐡅
			U == 0x0F7F 𐡀𐡅
			USE في use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		𐡄𐡍 pos_mapping:
			values = [k لكل k,v في pos_mapping.items() 𐡄𐡍 v 𐡅 UIPC في v]
			أكد len(values) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, values)
			USE = USE + values[0]

		out[U] = (USE, UBlock)
	أرجع out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
لكل h في headers:
	لكل l في h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
𐡄𐡍 language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    لكل k,v في sorted(use_mapping.items()):
        𐡄𐡍 k في use_positions 𐡅 use_positions[k]: استمر
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    لكل k,v في sorted(use_positions.items()):
        𐡄𐡍 𐡋𐡀 v: استمر
        لكل suf في v.keys():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

وإلاإذا language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name

uu = sorted (use_data.keys ())

data = {u:v[0] لكل u,v في use_data.items()}

𐡄𐡍 language.name == "c":
    خاص = 𐡒𐡔𐡈
وإلاإذا language.name == "rust":
    خاص = خطأ
    language.public_function_linkage = "pub(crate)"
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
𐡄𐡍 language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
وإلا:
    modes[DEFAULT] = ""

لكل step, text في modes.items():
    print()
    𐡄𐡍 text:
        print(text)
        print()
    𐡄𐡍 step is None:
        استمر

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, افتراضي='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, خاص=خاص)
    code.print_code(language=language, خاص=خاص)
    print ()

𐡄𐡍 language.name == "c":
    print ()
    لكل k في sorted(use_mapping.keys()):
        𐡄𐡍 k في use_positions 𐡅 use_positions[k]: استمر
        print ("#undef %s" % k)
    لكل k,v في sorted(use_positions.items()):
        𐡄𐡍 𐡋𐡀 v: استمر
        لكل suf في v.keys():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وإلاإذا language.name == "rust":
    pass
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
