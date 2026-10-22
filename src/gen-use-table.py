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

import os
import packTab

import sys
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

གལ་སྲིད len(sys.argv) > 1 ca sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
དེ་མིན:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

import sys

གལ་སྲིད len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') དོན་ལུ x anta_h sys.argv[1:]]

headers = [[f.readline () དོན་ལུ i anta_h range (2)] དོན་ལུ j,f anta_h enumerate(files) གལ་སྲིད j != 4]
དོན་ལུ j anta_h range(7, 9):
	དོན་ལུ line anta_h files[j]:
		line = line.rstrip()
		གལ་སྲིད na line:
			bha_nga
		headers[j - 1].append(line)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} དོན་ལུ _ anta_h files]
values = [{} དོན་ལུ _ anta_h files]
དོན་ལུ i, f anta_h enumerate (files):
	དོན་ལུ line anta_h f:

		j = line.find ('#')
		གལ་སྲིད j >= 0:
			line = line[:j]

		fields = [x.strip () དོན་ལུ x anta_h line.split (';')]
		གལ་སྲིད len (fields) == 1:
			continue

		uu = fields[0].split ('..')
		start = ཧྲིལ (uu[0], 16)
		གལ་སྲིད len (uu) == 1:
			end = start
		དེ་མིན:
			end = ཧྲིལ (uu[1], 16)

		t = fields[1 གལ་སྲིད i na anta_h [2, 4] དེ་མིན 2]

		གལ་སྲིད i == 2:
			t = 'jt_' + t
		elif i == 3 ca t != 'Default_Ignorable_Code_Point':
			continue
		elif i == 7 ca t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		elif i == 8 ca t == 'NA':
			t = 'Not_Applicable'

		i0 = i གལ་སྲིད i < 7 དེ་མིན i - 7
		དོན་ལུ u anta_h range (start, end + 1):
			unicode_data[i0][u] = t
		values[i0][t] = values[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
དོན་ལུ i,v anta_h enumerate (defaults):
	values[i][v] = values[i].get (v, 0) + 1
combined = {}
དོན་ལུ i,d anta_h enumerate (unicode_data):
	དོན་ལུ u,v anta_h d.items ():
		གལ་སྲིད na u anta_h combined:
			གལ་སྲིད i >= 4:
				continue
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v དོན་ལུ k, v anta_h combined.items() གལ་སྲིད v[6] na anta_h DISABLED_SCRIPTS}


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

class PropertyValue(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		ལོག self.name
	def __eq__(self, other):
		ལོག self.name == (other གལ་སྲིད isinstance(other, str) དེ་མིན other.name)
	def __ne__(self, other):
		ལོག na (self == other)
	def __hash__(self):
		ལོག hash(str(self))

property_values = {}

དོན་ལུ name anta_h property_names:
	value = PropertyValue(name)
	assert value na anta_h property_values
	assert value na anta_h globals()
	property_values[name] = value
globals().update(property_values)


def is_BASE(U, UISC, UDI, UGC, AJT):
	ལོག (UISC anta_h [Number, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] or
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT anta_h [jt_C, jt_D, jt_L, jt_R] ca UISC != Joiner or
		(UGC == Lo ca UISC anta_h [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_NUM(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Brahmi_Joining_Number
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	གལ་སྲིད UISC == Consonant_Placeholder: ལོག True
	ལོག U anta_h [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	ལོག UISC == Joiner or UDI ca UGC anta_h [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	ལོག ((UISC == Consonant_Final ca UGC != Lo) or
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	ལོག (UISC == Consonant_Medial ca UGC != Lo or
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	ལོག UISC anta_h [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Consonant_Subjoined ca UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Virama ca na is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ལོག U == 0x0DCA
def is_HALANT_NUM(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Number_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	ལོག UISC anta_h [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	ལོག UISC anta_h [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ལོག (UISC == Invisible_Stacker
		ca na is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	ལོག ((UGC == Po or UISC anta_h [Consonant_Dead, Joiner, Modifying_Letter, Other])
		ca na is_BASE(U, UISC, UDI, UGC, AJT)
		ca na is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		ca na is_CGJ(U, UISC, UDI, UGC, AJT)
		ca na is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		ca na is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	ལོག UISC anta_h [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	ལོག U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	ལོག UISC == Symbol_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	ལོག (UISC == Pure_Killer or
		UGC != Lo ca UISC anta_h [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	ལོག (UISC anta_h [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] or
		UGC != Lo ca UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	ལོག (UDI ca U na anta_h [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		ca UISC == Other
		ca na is_CGJ(U, UISC, UDI, UGC, AJT)
	) or UGC == Cn

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
	དོན་ལུ U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) anta_h data.items():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		གལ་སྲིད 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		གལ་སྲིད 0x0F18 <= U <= 0x0F19 or 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		གལ་སྲིད U == 0x1CED: UISC = Tone_Mark

		values = [k དོན་ལུ k,v anta_h items གལ་སྲིད v(U, UISC, UDI, UGC, AJT)]
		assert len(values) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, values)
		USE = values[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		གལ་སྲིད U anta_h [0x11302, 0x11303, 0x114C1]: UIPC = Top

		assert (UIPC anta_h [Not_Applicable, Visual_Order_Left] or
			U == 0x0F7F or
			USE anta_h use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		གལ་སྲིད pos_mapping:
			values = [k དོན་ལུ k,v anta_h pos_mapping.items() གལ་སྲིད v ca UIPC anta_h v]
			assert len(values) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, values)
			USE = USE + values[0]

		out[U] = (USE, UBlock)
	ལོག out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
དོན་ལུ h anta_h headers:
	དོན་ལུ l anta_h h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
གལ་སྲིད language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    དོན་ལུ k,v anta_h sorted(use_mapping.items()):
        གལ་སྲིད k anta_h use_positions ca use_positions[k]: continue
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    དོན་ལུ k,v anta_h sorted(use_positions.items()):
        གལ་སྲིད na v: continue
        དོན་ལུ suf anta_h v.keys():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

elif language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
དེ་མིན:
    assert False, "Unknown language: %s" % language.name

uu = sorted (use_data.keys ())

data = {u:v[0] དོན་ལུ u,v anta_h use_data.items()}

གལ་སྲིད language.name == "c":
    private = True
elif language.name == "rust":
    private = False
    language.public_function_linkage = "pub(crate)"
དེ་མིན:
    assert False, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
གལ་སྲིད language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
དེ་མིན:
    modes[DEFAULT] = ""

དོན་ལུ step, text anta_h modes.items():
    print()
    གལ་སྲིད text:
        print(text)
        print()
    གལ་སྲིད step is None:
        continue

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, default='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, private=private)
    code.print_code(language=language, private=private)
    print ()

གལ་སྲིད language.name == "c":
    print ()
    དོན་ལུ k anta_h sorted(use_mapping.keys()):
        གལ་སྲིད k anta_h use_positions ca use_positions[k]: continue
        print ("#undef %s" % k)
    དོན་ལུ k,v anta_h sorted(use_positions.items()):
        གལ་སྲིད na v: continue
        དོན་ལུ suf anta_h v.keys():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
elif language.name == "rust":
    pass
དེ་མིན:
    assert False, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
