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

આયાત os
આયાત packTab

આયાત sys
આયાત logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

જો len(sys.argv) > 1 અને sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
નહીંતર:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

આયાત sys

જો len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') માટે x માં sys.argv[1:]]

headers = [[f.readline () માટે i માં range (2)] માટે j,f માં enumerate(files) જો j != 4]
માટે j માં range(7, 9):
	માટે લીટી માં files[j]:
		લીટી = લીટી.rstrip()
		જો નહીં લીટી:
			તોડો
		headers[j - 1].append(લીટી)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} માટે _ માં files]
કિંમતો = [{} માટે _ માં files]
માટે i, f માં enumerate (files):
	માટે લીટી માં f:

		j = લીટી.find ('#')
		જો j >= 0:
			લીટી = લીટી[:j]

		fields = [x.strip () માટે x માં લીટી.split (';')]
		જો len (fields) == 1:
			ચાલુ

		uu = fields[0].split ('..')
		start = પૂર્ણાંક (uu[0], 16)
		જો len (uu) == 1:
			end = start
		નહીંતર:
			end = પૂર્ણાંક (uu[1], 16)

		t = fields[1 જો i નહીં માં [2, 4] નહીંતર 2]

		જો i == 2:
			t = 'jt_' + t
		અથવાજો i == 3 અને t != 'Default_Ignorable_Code_Point':
			ચાલુ
		અથવાજો i == 7 અને t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		અથવાજો i == 8 અને t == 'NA':
			t = 'Not_Applicable'

		i0 = i જો i < 7 નહીંતર i - 7
		માટે u માં range (start, end + 1):
			unicode_data[i0][u] = t
		કિંમતો[i0][t] = કિંમતો[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
માટે i,v માં enumerate (defaults):
	કિંમતો[i][v] = કિંમતો[i].get (v, 0) + 1
combined = {}
માટે i,d માં enumerate (unicode_data):
	માટે u,v માં d.વસ્તુઓ ():
		જો નહીં u માં combined:
			જો i >= 4:
				ચાલુ
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v માટે k, v માં combined.વસ્તુઓ() જો v[6] નહીં માં DISABLED_SCRIPTS}


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

વર્ગ Property_કિંમત(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		પાછા self.name
	def __eq__(self, other):
		પાછા self.name == (other જો isinstance(other, str) નહીંતર other.name)
	def __ne__(self, other):
		પાછા નહીં (self == other)
	def __hash__(self):
		પાછા hash(str(self))

property_કિંમતો = {}

માટે name માં property_names:
	કિંમત_2 = Property_કિંમત(name)
	પ્રતિજ્ઞા કિંમત_2 નહીં માં property_કિંમતો
	પ્રતિજ્ઞા કિંમત_2 નહીં માં globals()
	property_કિંમતો[name] = કિંમત_2
globals().update(property_કિંમતો)


def is_BASE(U, UISC, UDI, UGC, AJT):
	પાછા (UISC માં [સંખ્યા, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] અથવા
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT માં [jt_C, jt_D, jt_L, jt_R] અને UISC != Joiner અથવા
		(UGC == Lo અને UISC માં [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_અંક(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Brahmi_Joining_સંખ્યા
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	જો UISC == Consonant_Placeholder: પાછા સાચું
	પાછા U માં [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	પાછા UISC == Joiner અથવા UDI અને UGC માં [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	પાછા ((UISC == Consonant_Final અને UGC != Lo) અથવા
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	પાછા (UISC == Consonant_Medial અને UGC != Lo અથવા
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	પાછા UISC માં [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Consonant_Subjoined અને UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Virama અને નહીં is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	પાછા U == 0x0DCA
def is_HALANT_અંક(U, UISC, UDI, UGC, AJT):
	પાછા UISC == સંખ્યા_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	પાછા UISC માં [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	પાછા UISC માં [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	પાછા (UISC == Invisible_Stacker
		અને નહીં is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	પાછા ((UGC == Po અથવા UISC માં [Consonant_Dead, Joiner, Modifying_Letter, Other])
		અને નહીં is_BASE(U, UISC, UDI, UGC, AJT)
		અને નહીં is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		અને નહીં is_CGJ(U, UISC, UDI, UGC, AJT)
		અને નહીં is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		અને નહીં is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	પાછા UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	પાછા UISC માં [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	પાછા U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	પાછા UISC == પ્રતીક_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	પાછા (UISC == Pure_Killer અથવા
		UGC != Lo અને UISC માં [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	પાછા (UISC માં [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] અથવા
		UGC != Lo અને UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	પાછા (UDI અને U નહીં માં [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		અને UISC == Other
		અને નહીં is_CGJ(U, UISC, UDI, UGC, AJT)
	) અથવા UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_અંક,
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
	'HN':	is_HALANT_અંક,
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
		'Abv': [ટોચ],
		'Blw': [Bottom],
		'Pst': [જમણું_2],
	},
	'M': {
		'Abv': [ટોચ],
		'Blw': [Bottom, Bottom_And_ડાબું, Bottom_And_જમણું],
		'Pst': [જમણું_2],
		'Pre': [ડાબું, ટોચ_And_Bottom_And_ડાબું],
	},
	'CM': {
		'Abv': [ટોચ],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [ટોચ, ટોચ_And_Bottom, ટોચ_And_Bottom_And_જમણું, ટોચ_And_જમણું],
		'Blw': [Bottom, Overstruck, Bottom_And_જમણું],
		'Pst': [જમણું_2],
		'Pre': [ડાબું, ટોચ_And_ડાબું, ટોચ_And_ડાબું_And_જમણું, ડાબું_And_જમણું],
	},
	'VM': {
		'Abv': [ટોચ],
		'Blw': [Bottom, Overstruck],
		'Pst': [જમણું_2],
		'Pre': [ડાબું],
	},
	'SM': {
		'Abv': [ટોચ],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [ટોચ],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	વસ્તુઓ = use_mapping.વસ્તુઓ()
	માટે U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) માં data.વસ્તુઓ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		જો 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		જો 0x0F18 <= U <= 0x0F19 અથવા 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		જો U == 0x1CED: UISC = Tone_Mark

		કિંમતો = [k માટે k,v માં વસ્તુઓ જો v(U, UISC, UDI, UGC, AJT)]
		પ્રતિજ્ઞા len(કિંમતો) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, કિંમતો)
		USE = કિંમતો[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		જો U માં [0x11302, 0x11303, 0x114C1]: UIPC = ટોચ

		પ્રતિજ્ઞા (UIPC માં [Not_Applicable, Visual_Order_ડાબું] અથવા
			U == 0x0F7F અથવા
			USE માં use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		જો pos_mapping:
			કિંમતો = [k માટે k,v માં pos_mapping.વસ્તુઓ() જો v અને UIPC માં v]
			પ્રતિજ્ઞા len(કિંમતો) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, કિંમતો)
			USE = USE + કિંમતો[0]

		out[U] = (USE, UBlock)
	પાછા out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
માટે h માં headers:
	માટે l માં h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
જો language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    માટે k,v માં sorted(use_mapping.વસ્તુઓ()):
        જો k માં use_positions અને use_positions[k]: ચાલુ
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    માટે k,v માં sorted(use_positions.વસ્તુઓ()):
        જો નહીં v: ચાલુ
        માટે suf માં v.ચાવીઓ():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

અથવાજો language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
નહીંતર:
    પ્રતિજ્ઞા ખોટું, "Unknown language: %s" % language.name

uu = sorted (use_data.ચાવીઓ ())

data = {u:v[0] માટે u,v માં use_data.વસ્તુઓ()}

જો language.name == "c":
    ખાનગી = સાચું
અથવાજો language.name == "rust":
    ખાનગી = ખોટું
    language.public_function_linkage = "pub(crate)"
નહીંતર:
    પ્રતિજ્ઞા ખોટું, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
જો language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
નહીંતર:
    modes[DEFAULT] = ""

માટે step, text માં modes.વસ્તુઓ():
    print()
    જો text:
        print(text)
        print()
    જો step is None:
        ચાલુ

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, મૂળભૂત='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, ખાનગી=ખાનગી)
    code.print_code(language=language, ખાનગી=ખાનગી)
    print ()

જો language.name == "c":
    print ()
    માટે k માં sorted(use_mapping.ચાવીઓ()):
        જો k માં use_positions અને use_positions[k]: ચાલુ
        print ("#undef %s" % k)
    માટે k,v માં sorted(use_positions.વસ્તુઓ()):
        જો નહીં v: ચાલુ
        માટે suf માં v.ચાવીઓ():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
અથવાજો language.name == "rust":
    pass
નહીંતર:
    પ્રતિજ્ઞા ખોટું, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
