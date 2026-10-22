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

आनयन os
आनयन packTab

आनयन sys
आनयन logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

यदि len(sys.argv) > 1 च sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
अन्यथा:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

आनयन sys

यदि len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') प्रति x अन्तः sys.argv[1:]]

headers = [[f.readline () प्रति i अन्तः range (2)] प्रति j,f अन्तः enumerate(files) यदि j != 4]
प्रति j अन्तः range(7, 9):
	प्रति ओळ अन्तः files[j]:
		ओळ = ओळ.rstrip()
		यदि न ओळ:
			भङ्ग
		headers[j - 1].append(ओळ)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} प्रति _ अन्तः files]
मूल्ये = [{} प्रति _ अन्तः files]
प्रति i, f अन्तः enumerate (files):
	प्रति ओळ अन्तः f:

		j = ओळ.find ('#')
		यदि j >= 0:
			ओळ = ओळ[:j]

		fields = [x.strip () प्रति x अन्तः ओळ.split (';')]
		यदि len (fields) == 1:
			अनुवर्त

		uu = fields[0].split ('..')
		start = पूर्णाङ्क (uu[0], 16)
		यदि len (uu) == 1:
			end = start
		अन्यथा:
			end = पूर्णाङ्क (uu[1], 16)

		t = fields[1 यदि i न अन्तः [2, 4] अन्यथा 2]

		यदि i == 2:
			t = 'jt_' + t
		नोचेद्यदि i == 3 च t != 'Default_Ignorable_Code_Point':
			अनुवर्त
		नोचेद्यदि i == 7 च t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		नोचेद्यदि i == 8 च t == 'NA':
			t = 'Not_Applicable'

		i0 = i यदि i < 7 अन्यथा i - 7
		प्रति u अन्तः range (start, end + 1):
			unicode_data[i0][u] = t
		मूल्ये[i0][t] = मूल्ये[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
प्रति i,v अन्तः enumerate (defaults):
	मूल्ये[i][v] = मूल्ये[i].get (v, 0) + 1
combined = {}
प्रति i,d अन्तः enumerate (unicode_data):
	प्रति u,v अन्तः d.वस्तुसंच ():
		यदि न u अन्तः combined:
			यदि i >= 4:
				अनुवर्त
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v प्रति k, v अन्तः combined.वस्तुसंच() यदि v[6] न अन्तः DISABLED_SCRIPTS}


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

वर्ग Property_मूल्य(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		प्रत्यागम self.name
	def __eq__(self, other):
		प्रत्यागम self.name == (other यदि isinstance(other, str) अन्यथा other.name)
	def __ne__(self, other):
		प्रत्यागम न (self == other)
	def __hash__(self):
		प्रत्यागम hash(str(self))

property_मूल्ये = {}

प्रति name अन्तः property_names:
	मूल्य_2 = Property_मूल्य(name)
	प्रतिज्ञा मूल्य_2 न अन्तः property_मूल्ये
	प्रतिज्ञा मूल्य_2 न अन्तः globals()
	property_मूल्ये[name] = मूल्य_2
globals().update(property_मूल्ये)


def is_BASE(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC अन्तः [संख्या, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] वा
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT अन्तः [jt_C, jt_D, jt_L, jt_R] च UISC != Joiner वा
		(UGC == Lo च UISC अन्तः [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_अंक(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Brahmi_Joining_संख्या
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	यदि UISC == Consonant_Placeholder: प्रत्यागम सत्य
	प्रत्यागम U अन्तः [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	प्रत्यागम UISC == Joiner वा UDI च UGC अन्तः [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	प्रत्यागम ((UISC == Consonant_Final च UGC != Lo) वा
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	प्रत्यागम (UISC == Consonant_Medial च UGC != Lo वा
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Consonant_Subjoined च UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Virama च न is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	प्रत्यागम U == 0x0DCA
def is_HALANT_अंक(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == संख्या_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	प्रत्यागम (UISC == Invisible_Stacker
		च न is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	प्रत्यागम ((UGC == Po वा UISC अन्तः [Consonant_Dead, Joiner, Modifying_Letter, Other])
		च न is_BASE(U, UISC, UDI, UGC, AJT)
		च न is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		च न is_CGJ(U, UISC, UDI, UGC, AJT)
		च न is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		च न is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	प्रत्यागम U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == चिन्ह_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC == Pure_Killer वा
		UGC != Lo च UISC अन्तः [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC अन्तः [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] वा
		UGC != Lo च UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	प्रत्यागम (UDI च U न अन्तः [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		च UISC == Other
		च न is_CGJ(U, UISC, UDI, UGC, AJT)
	) वा UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_अंक,
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
	'HN':	is_HALANT_अंक,
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
		'Abv': [शीर्ष],
		'Blw': [Bottom],
		'Pst': [उजवा_2],
	},
	'M': {
		'Abv': [शीर्ष],
		'Blw': [Bottom, Bottom_And_डावा, Bottom_And_उजवा],
		'Pst': [उजवा_2],
		'Pre': [डावा, शीर्ष_And_Bottom_And_डावा],
	},
	'CM': {
		'Abv': [शीर्ष],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [शीर्ष, शीर्ष_And_Bottom, शीर्ष_And_Bottom_And_उजवा, शीर्ष_And_उजवा],
		'Blw': [Bottom, Overstruck, Bottom_And_उजवा],
		'Pst': [उजवा_2],
		'Pre': [डावा, शीर्ष_And_डावा, शीर्ष_And_डावा_And_उजवा, डावा_And_उजवा],
	},
	'VM': {
		'Abv': [शीर्ष],
		'Blw': [Bottom, Overstruck],
		'Pst': [उजवा_2],
		'Pre': [डावा],
	},
	'SM': {
		'Abv': [शीर्ष],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [शीर्ष],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	वस्तुसंच = use_mapping.वस्तुसंच()
	प्रति U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) अन्तः data.वस्तुसंच():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		यदि 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		यदि 0x0F18 <= U <= 0x0F19 वा 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		यदि U == 0x1CED: UISC = Tone_Mark

		मूल्ये = [k प्रति k,v अन्तः वस्तुसंच यदि v(U, UISC, UDI, UGC, AJT)]
		प्रतिज्ञा len(मूल्ये) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, मूल्ये)
		USE = मूल्ये[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		यदि U अन्तः [0x11302, 0x11303, 0x114C1]: UIPC = शीर्ष

		प्रतिज्ञा (UIPC अन्तः [Not_Applicable, Visual_Order_डावा] वा
			U == 0x0F7F वा
			USE अन्तः use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		यदि pos_mapping:
			मूल्ये = [k प्रति k,v अन्तः pos_mapping.वस्तुसंच() यदि v च UIPC अन्तः v]
			प्रतिज्ञा len(मूल्ये) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, मूल्ये)
			USE = USE + मूल्ये[0]

		out[U] = (USE, UBlock)
	प्रत्यागम out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
प्रति h अन्तः headers:
	प्रति l अन्तः h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
यदि language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    प्रति k,v अन्तः sorted(use_mapping.वस्तुसंच()):
        यदि k अन्तः use_positions च use_positions[k]: अनुवर्त
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    प्रति k,v अन्तः sorted(use_positions.वस्तुसंच()):
        यदि न v: अनुवर्त
        प्रति suf अन्तः v.किल्ल्या():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

नोचेद्यदि language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
अन्यथा:
    प्रतिज्ञा असत्य, "Unknown language: %s" % language.name

uu = sorted (use_data.किल्ल्या ())

data = {u:v[0] प्रति u,v अन्तः use_data.वस्तुसंच()}

यदि language.name == "c":
    गुप्त = सत्य
नोचेद्यदि language.name == "rust":
    गुप्त = असत्य
    language.public_function_linkage = "pub(crate)"
अन्यथा:
    प्रतिज्ञा असत्य, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
यदि language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
अन्यथा:
    modes[DEFAULT] = ""

प्रति step, text अन्तः modes.वस्तुसंच():
    print()
    यदि text:
        print(text)
        print()
    यदि step is None:
        अनुवर्त

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, स्वभाव='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, गुप्त=गुप्त)
    code.print_code(language=language, गुप्त=गुप्त)
    print ()

यदि language.name == "c":
    print ()
    प्रति k अन्तः sorted(use_mapping.किल्ल्या()):
        यदि k अन्तः use_positions च use_positions[k]: अनुवर्त
        print ("#undef %s" % k)
    प्रति k,v अन्तः sorted(use_positions.वस्तुसंच()):
        यदि न v: अनुवर्त
        प्रति suf अन्तः v.किल्ल्या():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
नोचेद्यदि language.name == "rust":
    pass
अन्यथा:
    प्रतिज्ञा असत्य, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
