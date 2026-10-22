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

இறக்குமதி os
இறக்குமதி packTab

இறக்குமதி sys
இறக்குமதி logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

எனில் len(sys.argv) > 1 மற்றும் sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
இல்லையெனில்:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

இறக்குமதி sys

எனில் len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') க்கு x உள் sys.argv[1:]]

headers = [[f.readline () க்கு i உள் range (2)] க்கு j,f உள் enumerate(files) எனில் j != 4]
க்கு j உள் range(7, 9):
	க்கு வரி உள் files[j]:
		வரி = வரி.rstrip()
		எனில் இல்லை வரி:
			நிறுத்து
		headers[j - 1].append(வரி)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} க்கு _ உள் files]
மதிப்புகள் = [{} க்கு _ உள் files]
க்கு i, f உள் enumerate (files):
	க்கு வரி உள் f:

		j = வரி.find ('#')
		எனில் j >= 0:
			வரி = வரி[:j]

		fields = [x.strip () க்கு x உள் வரி.split (';')]
		எனில் len (fields) == 1:
			தொடர்

		uu = fields[0].split ('..')
		start = முழு (uu[0], 16)
		எனில் len (uu) == 1:
			end = start
		இல்லையெனில்:
			end = முழு (uu[1], 16)

		t = fields[1 எனில் i இல்லை உள் [2, 4] இல்லையெனில் 2]

		எனில் i == 2:
			t = 'jt_' + t
		அல்லதெனில் i == 3 மற்றும் t != 'Default_Ignorable_Code_Point':
			தொடர்
		அல்லதெனில் i == 7 மற்றும் t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		அல்லதெனில் i == 8 மற்றும் t == 'NA':
			t = 'Not_Applicable'

		i0 = i எனில் i < 7 இல்லையெனில் i - 7
		க்கு u உள் range (start, end + 1):
			unicode_data[i0][u] = t
		மதிப்புகள்[i0][t] = மதிப்புகள்[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
க்கு i,v உள் enumerate (defaults):
	மதிப்புகள்[i][v] = மதிப்புகள்[i].get (v, 0) + 1
combined = {}
க்கு i,d உள் enumerate (unicode_data):
	க்கு u,v உள் d.பொருட்கள் ():
		எனில் இல்லை u உள் combined:
			எனில் i >= 4:
				தொடர்
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v க்கு k, v உள் combined.பொருட்கள்() எனில் v[6] இல்லை உள் DISABLED_SCRIPTS}


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

வகுப்பு Property_மதிப்பு(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		திரும்பு self.name
	def __eq__(self, other):
		திரும்பு self.name == (other எனில் isinstance(other, str) இல்லையெனில் other.name)
	def __ne__(self, other):
		திரும்பு இல்லை (self == other)
	def __hash__(self):
		திரும்பு hash(str(self))

property_மதிப்புகள் = {}

க்கு name உள் property_names:
	மதிப்பு_2 = Property_மதிப்பு(name)
	ப்ரதிஜ்ஞா மதிப்பு_2 இல்லை உள் property_மதிப்புகள்
	ப்ரதிஜ்ஞா மதிப்பு_2 இல்லை உள் globals()
	property_மதிப்புகள்[name] = மதிப்பு_2
globals().update(property_மதிப்புகள்)


def is_BASE(U, UISC, UDI, UGC, AJT):
	திரும்பு (UISC உள் [எண், Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] அல்லது
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT உள் [jt_C, jt_D, jt_L, jt_R] மற்றும் UISC != Joiner அல்லது
		(UGC == Lo மற்றும் UISC உள் [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_இலக்கம்(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Brahmi_Joining_எண்
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	எனில் UISC == Consonant_Placeholder: திரும்பு உண்மை
	திரும்பு U உள் [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	திரும்பு UISC == Joiner அல்லது UDI மற்றும் UGC உள் [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	திரும்பு ((UISC == Consonant_Final மற்றும் UGC != Lo) அல்லது
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	திரும்பு (UISC == Consonant_Medial மற்றும் UGC != Lo அல்லது
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC உள் [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Consonant_Subjoined மற்றும் UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Virama மற்றும் இல்லை is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	திரும்பு U == 0x0DCA
def is_HALANT_இலக்கம்(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == எண்_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC உள் [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC உள் [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	திரும்பு (UISC == Invisible_Stacker
		மற்றும் இல்லை is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	திரும்பு ((UGC == Po அல்லது UISC உள் [Consonant_Dead, Joiner, Modifying_Letter, Other])
		மற்றும் இல்லை is_BASE(U, UISC, UDI, UGC, AJT)
		மற்றும் இல்லை is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		மற்றும் இல்லை is_CGJ(U, UISC, UDI, UGC, AJT)
		மற்றும் இல்லை is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		மற்றும் இல்லை is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC உள் [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	திரும்பு U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	திரும்பு UISC == குறியீடு_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	திரும்பு (UISC == Pure_Killer அல்லது
		UGC != Lo மற்றும் UISC உள் [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	திரும்பு (UISC உள் [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] அல்லது
		UGC != Lo மற்றும் UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	திரும்பு (UDI மற்றும் U இல்லை உள் [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		மற்றும் UISC == Other
		மற்றும் இல்லை is_CGJ(U, UISC, UDI, UGC, AJT)
	) அல்லது UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_இலக்கம்,
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
	'HN':	is_HALANT_இலக்கம்,
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
		'Abv': [உச்சி],
		'Blw': [Bottom],
		'Pst': [வலது_2],
	},
	'M': {
		'Abv': [உச்சி],
		'Blw': [Bottom, Bottom_And_இடது, Bottom_And_வலது],
		'Pst': [வலது_2],
		'Pre': [இடது, உச்சி_And_Bottom_And_இடது],
	},
	'CM': {
		'Abv': [உச்சி],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [உச்சி, உச்சி_And_Bottom, உச்சி_And_Bottom_And_வலது, உச்சி_And_வலது],
		'Blw': [Bottom, Overstruck, Bottom_And_வலது],
		'Pst': [வலது_2],
		'Pre': [இடது, உச்சி_And_இடது, உச்சி_And_இடது_And_வலது, இடது_And_வலது],
	},
	'VM': {
		'Abv': [உச்சி],
		'Blw': [Bottom, Overstruck],
		'Pst': [வலது_2],
		'Pre': [இடது],
	},
	'SM': {
		'Abv': [உச்சி],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [உச்சி],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	பொருட்கள் = use_mapping.பொருட்கள்()
	க்கு U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) உள் data.பொருட்கள்():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		எனில் 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		எனில் 0x0F18 <= U <= 0x0F19 அல்லது 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		எனில் U == 0x1CED: UISC = Tone_Mark

		மதிப்புகள் = [k க்கு k,v உள் பொருட்கள் எனில் v(U, UISC, UDI, UGC, AJT)]
		ப்ரதிஜ்ஞா len(மதிப்புகள்) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, மதிப்புகள்)
		USE = மதிப்புகள்[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		எனில் U உள் [0x11302, 0x11303, 0x114C1]: UIPC = உச்சி

		ப்ரதிஜ்ஞா (UIPC உள் [Not_Applicable, Visual_Order_இடது] அல்லது
			U == 0x0F7F அல்லது
			USE உள் use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		எனில் pos_mapping:
			மதிப்புகள் = [k க்கு k,v உள் pos_mapping.பொருட்கள்() எனில் v மற்றும் UIPC உள் v]
			ப்ரதிஜ்ஞா len(மதிப்புகள்) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, மதிப்புகள்)
			USE = USE + மதிப்புகள்[0]

		out[U] = (USE, UBlock)
	திரும்பு out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
க்கு h உள் headers:
	க்கு l உள் h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
எனில் language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    க்கு k,v உள் sorted(use_mapping.பொருட்கள்()):
        எனில் k உள் use_positions மற்றும் use_positions[k]: தொடர்
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    க்கு k,v உள் sorted(use_positions.பொருட்கள்()):
        எனில் இல்லை v: தொடர்
        க்கு suf உள் v.சாவிகள்():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

அல்லதெனில் language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
இல்லையெனில்:
    ப்ரதிஜ்ஞா பொய், "Unknown language: %s" % language.name

uu = sorted (use_data.சாவிகள் ())

data = {u:v[0] க்கு u,v உள் use_data.பொருட்கள்()}

எனில் language.name == "c":
    தனிப்பட்ட = உண்மை
அல்லதெனில் language.name == "rust":
    தனிப்பட்ட = பொய்
    language.public_function_linkage = "pub(crate)"
இல்லையெனில்:
    ப்ரதிஜ்ஞா பொய், "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
எனில் language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
இல்லையெனில்:
    modes[DEFAULT] = ""

க்கு step, text உள் modes.பொருட்கள்():
    print()
    எனில் text:
        print(text)
        print()
    எனில் step is None:
        தொடர்

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, இயல்பு='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, தனிப்பட்ட=தனிப்பட்ட)
    code.print_code(language=language, தனிப்பட்ட=தனிப்பட்ட)
    print ()

எனில் language.name == "c":
    print ()
    க்கு k உள் sorted(use_mapping.சாவிகள்()):
        எனில் k உள் use_positions மற்றும் use_positions[k]: தொடர்
        print ("#undef %s" % k)
    க்கு k,v உள் sorted(use_positions.பொருட்கள்()):
        எனில் இல்லை v: தொடர்
        க்கு suf உள் v.சாவிகள்():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
அல்லதெனில் language.name == "rust":
    pass
இல்லையெனில்:
    ப்ரதிஜ்ஞா பொய், "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
