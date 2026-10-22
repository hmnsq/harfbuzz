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

إذا len(sys.argv) > 1 و sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وإلا:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

استورد sys

إذا len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

headers = [[f.readline () لكل i في range (2)] لكل j,f في enumerate(files) إذا j != 4]
لكل j في range(7, 9):
	لكل سطر في files[j]:
		سطر = سطر.rstrip()
		إذا ليس سطر:
			اكسر
		headers[j - 1].append(سطر)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} لكل _ في files]
قيم = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل سطر في f:

		j = سطر.find ('#')
		إذا j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () لكل x في سطر.split (';')]
		إذا len (fields) == 1:
			استمر

		uu = fields[0].split ('..')
		start = int (uu[0], 16)
		إذا len (uu) == 1:
			end = start
		وإلا:
			end = int (uu[1], 16)

		t = fields[1 إذا i ليس في [2, 4] وإلا 2]

		إذا i == 2:
			t = 'jt_' + t
		وإلاإذا i == 3 و t != 'Default_Ignorable_Code_Point':
			استمر
		وإلاإذا i == 7 و t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		وإلاإذا i == 8 و t == 'NA':
			t = 'Not_Applicable'

		i0 = i إذا i < 7 وإلا i - 7
		لكل u في range (start, end + 1):
			unicode_data[i0][u] = t
		قيم[i0][t] = قيم[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
لكل i,v في enumerate (defaults):
	قيم[i][v] = قيم[i].get (v, 0) + 1
combined = {}
لكل i,d في enumerate (unicode_data):
	لكل u,v في d.عناصر ():
		إذا ليس u في combined:
			إذا i >= 4:
				استمر
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v لكل k, v في combined.عناصر() إذا v[6] ليس في DISABLED_SCRIPTS}


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

صنف Property_قيمة(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		أرجع self.name
	def __eq__(self, other):
		أرجع self.name == (other إذا isinstance(other, str) وإلا other.name)
	def __ne__(self, other):
		أرجع ليس (self == other)
	def __hash__(self):
		أرجع hash(str(self))

property_قيم = {}

لكل name في property_names:
	قيمة_2 = Property_قيمة(name)
	أكد قيمة_2 ليس في property_قيم
	أكد قيمة_2 ليس في globals()
	property_قيم[name] = قيمة_2
globals().update(property_قيم)


def is_BASE(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [رقم_2, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] أو
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT في [jt_C, jt_D, jt_L, jt_R] و UISC != Joiner أو
		(UGC == Lo و UISC في [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_رقم(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Brahmi_Joining_رقم
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	إذا UISC == Consonant_Placeholder: أرجع صحيح
	أرجع U في [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	أرجع UISC == Joiner أو UDI و UGC في [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	أرجع ((UISC == Consonant_Final و UGC != Lo) أو
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	أرجع (UISC == Consonant_Medial و UGC != Lo أو
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_Subjoined و UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Virama و ليس is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x0DCA
def is_HALANT_رقم(U, UISC, UDI, UGC, AJT):
	أرجع UISC == رقم_Joiner
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
		و ليس is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	أرجع ((UGC == Po أو UISC في [Consonant_Dead, Joiner, Modifying_Letter, Other])
		و ليس is_BASE(U, UISC, UDI, UGC, AJT)
		و ليس is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
		و ليس is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		و ليس is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == رمز_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	أرجع (UISC == Pure_Killer أو
		UGC != Lo و UISC في [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] أو
		UGC != Lo و UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	أرجع (UDI و U ليس في [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		و UISC == Other
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
	) أو UGC == Cn

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
		'Abv': [قمة],
		'Blw': [Bottom],
		'Pst': [يمين_2],
	},
	'M': {
		'Abv': [قمة],
		'Blw': [Bottom, Bottom_And_يسار, Bottom_And_يمين],
		'Pst': [يمين_2],
		'Pre': [يسار, قمة_And_Bottom_And_يسار],
	},
	'CM': {
		'Abv': [قمة],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [قمة, قمة_And_Bottom, قمة_And_Bottom_And_يمين, قمة_And_يمين],
		'Blw': [Bottom, Overstruck, Bottom_And_يمين],
		'Pst': [يمين_2],
		'Pre': [يسار, قمة_And_يسار, قمة_And_يسار_And_يمين, يسار_And_يمين],
	},
	'VM': {
		'Abv': [قمة],
		'Blw': [Bottom, Overstruck],
		'Pst': [يمين_2],
		'Pre': [يسار],
	},
	'SM': {
		'Abv': [قمة],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [قمة],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	عناصر = use_mapping.عناصر()
	لكل U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) في data.عناصر():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x0F18 <= U <= 0x0F19 أو 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		إذا U == 0x1CED: UISC = Tone_Mark

		قيم = [k لكل k,v في عناصر إذا v(U, UISC, UDI, UGC, AJT)]
		أكد len(قيم) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, قيم)
		USE = قيم[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		إذا U في [0x11302, 0x11303, 0x114C1]: UIPC = قمة

		أكد (UIPC في [Not_Applicable, Visual_Order_يسار] أو
			U == 0x0F7F أو
			USE في use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		إذا pos_mapping:
			قيم = [k لكل k,v في pos_mapping.عناصر() إذا v و UIPC في v]
			أكد len(قيم) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, قيم)
			USE = USE + قيم[0]

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
إذا language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    لكل k,v في sorted(use_mapping.عناصر()):
        إذا k في use_positions و use_positions[k]: استمر
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    لكل k,v في sorted(use_positions.عناصر()):
        إذا ليس v: استمر
        لكل suf في v.مفاتيح():
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

uu = sorted (use_data.مفاتيح ())

data = {u:v[0] لكل u,v في use_data.عناصر()}

إذا language.name == "c":
    خاص = صحيح
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
إذا language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
وإلا:
    modes[DEFAULT] = ""

لكل step, text في modes.عناصر():
    print()
    إذا text:
        print(text)
        print()
    إذا step is None:
        استمر

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, افتراضي='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, خاص=خاص)
    code.اطبع_code(language=language, خاص=خاص)
    print ()

إذا language.name == "c":
    print ()
    لكل k في sorted(use_mapping.مفاتيح()):
        إذا k في use_positions و use_positions[k]: استمر
        print ("#undef %s" % k)
    لكل k,v في sorted(use_positions.عناصر()):
        إذا ليس v: استمر
        لكل suf في v.مفاتيح():
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
