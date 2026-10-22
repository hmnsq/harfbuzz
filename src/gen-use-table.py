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

जइ len(sys.argv) > 1 य sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
अण्णहा:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

आनयन sys

जइ len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') प्रति x अन्तः sys.argv[1:]]

headers = [[f.readline () प्रति i अन्तः range (2)] प्रति j,f अन्तः enumerate(files) जइ j != 4]
प्रति j अन्तः range(7, 9):
	प्रति पङ्क्तिः अन्तः files[j]:
		पङ्क्तिः = पङ्क्तिः.rstrip()
		जइ ण पङ्क्तिः:
			भङ्ग
		headers[j - 1].append(पङ्क्तिः)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} प्रति _ अन्तः files]
मानानि = [{} प्रति _ अन्तः files]
प्रति i, f अन्तः enumerate (files):
	प्रति पङ्क्तिः अन्तः f:

		j = पङ्क्तिः.find ('#')
		जइ j >= 0:
			पङ्क्तिः = पङ्क्तिः[:j]

		fields = [x.strip () प्रति x अन्तः पङ्क्तिः.split (';')]
		जइ len (fields) == 1:
			अनुवर्त

		uu = fields[0].split ('..')
		start = पूर्णाङ्क (uu[0], 16)
		जइ len (uu) == 1:
			end = start
		अण्णहा:
			end = पूर्णाङ्क (uu[1], 16)

		t = fields[1 जइ i ण अन्तः [2, 4] अण्णहा 2]

		जइ i == 2:
			t = 'jt_' + t
		नोचेद्यदि i == 3 य t != 'Default_Ignorable_Code_Point':
			अनुवर्त
		नोचेद्यदि i == 7 य t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		नोचेद्यदि i == 8 य t == 'NA':
			t = 'Not_Applicable'

		i0 = i जइ i < 7 अण्णहा i - 7
		प्रति u अन्तः range (start, end + 1):
			unicode_data[i0][u] = t
		मानानि[i0][t] = मानानि[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
प्रति i,v अन्तः enumerate (defaults):
	मानानि[i][v] = मानानि[i].get (v, 0) + 1
combined = {}
प्रति i,d अन्तः enumerate (unicode_data):
	प्रति u,v अन्तः d.वस्तूनि ():
		जइ ण u अन्तः combined:
			जइ i >= 4:
				अनुवर्त
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v प्रति k, v अन्तः combined.वस्तूनि() जइ v[6] ण अन्तः DISABLED_SCRIPTS}


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

वर्ग Property_मानम्(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		प्रत्यागम self.name
	def __eq__(self, other):
		प्रत्यागम self.name == (other जइ isinstance(other, str) अण्णहा other.name)
	def __ne__(self, other):
		प्रत्यागम ण (self == other)
	def __hash__(self):
		प्रत्यागम hash(str(self))

property_मानानि = {}

प्रति name अन्तः property_names:
	मानम्_2 = Property_मानम्(name)
	प्रतिज्ञा मानम्_2 ण अन्तः property_मानानि
	प्रतिज्ञा मानम्_2 ण अन्तः globals()
	property_मानानि[name] = मानम्_2
globals().update(property_मानानि)


def is_BASE(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC अन्तः [सङ्ख्या, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] वा
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT अन्तः [jt_C, jt_D, jt_L, jt_R] य UISC != Joiner वा
		(UGC == Lo य UISC अन्तः [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_अङ्कः(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Brahmi_Joining_सङ्ख्या
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	जइ UISC == Consonant_Placeholder: प्रत्यागम सच्च
	प्रत्यागम U अन्तः [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	प्रत्यागम UISC == Joiner वा UDI य UGC अन्तः [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	प्रत्यागम ((UISC == Consonant_Final य UGC != Lo) वा
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	प्रत्यागम (UISC == Consonant_Medial य UGC != Lo वा
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Consonant_Subjoined य UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Virama य ण is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	प्रत्यागम U == 0x0DCA
def is_HALANT_अङ्कः(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == सङ्ख्या_Joiner
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
		य ण is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	प्रत्यागम ((UGC == Po वा UISC अन्तः [Consonant_Dead, Joiner, Modifying_Letter, Other])
		य ण is_BASE(U, UISC, UDI, UGC, AJT)
		य ण is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		य ण is_CGJ(U, UISC, UDI, UGC, AJT)
		य ण is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		य ण is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC अन्तः [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	प्रत्यागम U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम UISC == चिह्नम्_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC == Pure_Killer वा
		UGC != Lo य UISC अन्तः [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	प्रत्यागम (UISC अन्तः [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] वा
		UGC != Lo य UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	प्रत्यागम (UDI य U ण अन्तः [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		य UISC == Other
		य ण is_CGJ(U, UISC, UDI, UGC, AJT)
	) वा UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_अङ्कः,
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
	'HN':	is_HALANT_अङ्कः,
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
		'Abv': [शिखरम्],
		'Blw': [Bottom],
		'Pst': [दक्षिणः_2],
	},
	'M': {
		'Abv': [शिखरम्],
		'Blw': [Bottom, Bottom_And_वामः, Bottom_And_दक्षिणः],
		'Pst': [दक्षिणः_2],
		'Pre': [वामः, शिखरम्_And_Bottom_And_वामः],
	},
	'CM': {
		'Abv': [शिखरम्],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [शिखरम्, शिखरम्_And_Bottom, शिखरम्_And_Bottom_And_दक्षिणः, शिखरम्_And_दक्षिणः],
		'Blw': [Bottom, Overstruck, Bottom_And_दक्षिणः],
		'Pst': [दक्षिणः_2],
		'Pre': [वामः, शिखरम्_And_वामः, शिखरम्_And_वामः_And_दक्षिणः, वामः_And_दक्षिणः],
	},
	'VM': {
		'Abv': [शिखरम्],
		'Blw': [Bottom, Overstruck],
		'Pst': [दक्षिणः_2],
		'Pre': [वामः],
	},
	'SM': {
		'Abv': [शिखरम्],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [शिखरम्],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	वस्तूनि = use_mapping.वस्तूनि()
	प्रति U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) अन्तः data.वस्तूनि():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		जइ 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		जइ 0x0F18 <= U <= 0x0F19 वा 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		जइ U == 0x1CED: UISC = Tone_Mark

		मानानि = [k प्रति k,v अन्तः वस्तूनि जइ v(U, UISC, UDI, UGC, AJT)]
		प्रतिज्ञा len(मानानि) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, मानानि)
		USE = मानानि[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		जइ U अन्तः [0x11302, 0x11303, 0x114C1]: UIPC = शिखरम्

		प्रतिज्ञा (UIPC अन्तः [Not_Applicable, Visual_Order_वामः] वा
			U == 0x0F7F वा
			USE अन्तः use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		जइ pos_mapping:
			मानानि = [k प्रति k,v अन्तः pos_mapping.वस्तूनि() जइ v य UIPC अन्तः v]
			प्रतिज्ञा len(मानानि) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, मानानि)
			USE = USE + मानानि[0]

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
जइ language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    प्रति k,v अन्तः sorted(use_mapping.वस्तूनि()):
        जइ k अन्तः use_positions य use_positions[k]: अनुवर्त
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    प्रति k,v अन्तः sorted(use_positions.वस्तूनि()):
        जइ ण v: अनुवर्त
        प्रति suf अन्तः v.कुञ्चिकाः():
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
अण्णहा:
    प्रतिज्ञा मिच्छ, "Unknown language: %s" % language.name

uu = sorted (use_data.कुञ्चिकाः ())

data = {u:v[0] प्रति u,v अन्तः use_data.वस्तूनि()}

जइ language.name == "c":
    गुप्त = सच्च
नोचेद्यदि language.name == "rust":
    गुप्त = मिच्छ
    language.public_function_linkage = "pub(crate)"
अण्णहा:
    प्रतिज्ञा मिच्छ, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
जइ language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
अण्णहा:
    modes[DEFAULT] = ""

प्रति step, text अन्तः modes.वस्तूनि():
    print()
    जइ text:
        print(text)
        print()
    जइ step is None:
        अनुवर्त

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, स्वभाव='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, गुप्त=गुप्त)
    code.print_code(language=language, गुप्त=गुप्त)
    print ()

जइ language.name == "c":
    print ()
    प्रति k अन्तः sorted(use_mapping.कुञ्चिकाः()):
        जइ k अन्तः use_positions य use_positions[k]: अनुवर्त
        print ("#undef %s" % k)
    प्रति k,v अन्तः sorted(use_positions.वस्तूनि()):
        जइ ण v: अनुवर्त
        प्रति suf अन्तः v.कुञ्चिकाः():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
नोचेद्यदि language.name == "rust":
    pass
अण्णहा:
    प्रतिज्ञा मिच्छ, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
