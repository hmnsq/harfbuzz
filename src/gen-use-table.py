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

आयात os
आयात packTab

आयात sys
आयात logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

जर len(sys.argv) > 1 आणि sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
अन्यथा:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

आयात sys

जर len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') साठी x मध्ये sys.argv[1:]]

headers = [[f.readline () साठी i मध्ये range (2)] साठी j,f मध्ये enumerate(files) जर j != 4]
साठी j मध्ये range(7, 9):
	साठी ओळ मध्ये files[j]:
		ओळ = ओळ.rstrip()
		जर नाही ओळ:
			थांबा
		headers[j - 1].append(ओळ)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} साठी _ मध्ये files]
मूल्ये = [{} साठी _ मध्ये files]
साठी i, f मध्ये enumerate (files):
	साठी ओळ मध्ये f:

		j = ओळ.find ('#')
		जर j >= 0:
			ओळ = ओळ[:j]

		fields = [x.strip () साठी x मध्ये ओळ.split (';')]
		जर len (fields) == 1:
			सुरूठेवा

		uu = fields[0].split ('..')
		start = पूर्णांक (uu[0], 16)
		जर len (uu) == 1:
			end = start
		अन्यथा:
			end = पूर्णांक (uu[1], 16)

		t = fields[1 जर i नाही मध्ये [2, 4] अन्यथा 2]

		जर i == 2:
			t = 'jt_' + t
		किंवाजर i == 3 आणि t != 'Default_Ignorable_Code_Point':
			सुरूठेवा
		किंवाजर i == 7 आणि t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		किंवाजर i == 8 आणि t == 'NA':
			t = 'Not_Applicable'

		i0 = i जर i < 7 अन्यथा i - 7
		साठी u मध्ये range (start, end + 1):
			unicode_data[i0][u] = t
		मूल्ये[i0][t] = मूल्ये[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
साठी i,v मध्ये enumerate (defaults):
	मूल्ये[i][v] = मूल्ये[i].get (v, 0) + 1
combined = {}
साठी i,d मध्ये enumerate (unicode_data):
	साठी u,v मध्ये d.वस्तुसंच ():
		जर नाही u मध्ये combined:
			जर i >= 4:
				सुरूठेवा
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v साठी k, v मध्ये combined.वस्तुसंच() जर v[6] नाही मध्ये DISABLED_SCRIPTS}


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
		परत self.name
	def __eq__(self, other):
		परत self.name == (other जर isinstance(other, str) अन्यथा other.name)
	def __ne__(self, other):
		परत नाही (self == other)
	def __hash__(self):
		परत hash(str(self))

property_मूल्ये = {}

साठी name मध्ये property_names:
	मूल्य_2 = Property_मूल्य(name)
	प्रतिज्ञा मूल्य_2 नाही मध्ये property_मूल्ये
	प्रतिज्ञा मूल्य_2 नाही मध्ये globals()
	property_मूल्ये[name] = मूल्य_2
globals().update(property_मूल्ये)


def is_BASE(U, UISC, UDI, UGC, AJT):
	परत (UISC मध्ये [संख्या, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] किंवा
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT मध्ये [jt_C, jt_D, jt_L, jt_R] आणि UISC != Joiner किंवा
		(UGC == Lo आणि UISC मध्ये [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_अंक(U, UISC, UDI, UGC, AJT):
	परत UISC == Brahmi_Joining_संख्या
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	जर UISC == Consonant_Placeholder: परत सत्य
	परत U मध्ये [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	परत UISC == Joiner किंवा UDI आणि UGC मध्ये [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	परत ((UISC == Consonant_Final आणि UGC != Lo) किंवा
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	परत UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	परत (UISC == Consonant_Medial आणि UGC != Lo किंवा
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	परत UISC मध्ये [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	परत UISC == Consonant_Subjoined आणि UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	परत UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	परत UISC == Virama आणि नाही is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	परत U == 0x0DCA
def is_HALANT_अंक(U, UISC, UDI, UGC, AJT):
	परत UISC == संख्या_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	परत UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	परत UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	परत UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	परत UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	परत UISC मध्ये [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	परत UISC मध्ये [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	परत (UISC == Invisible_Stacker
		आणि नाही is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	परत UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	परत ((UGC == Po किंवा UISC मध्ये [Consonant_Dead, Joiner, Modifying_Letter, Other])
		आणि नाही is_BASE(U, UISC, UDI, UGC, AJT)
		आणि नाही is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		आणि नाही is_CGJ(U, UISC, UDI, UGC, AJT)
		आणि नाही is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		आणि नाही is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	परत UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	परत UISC मध्ये [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	परत U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	परत UISC == चिन्ह_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	परत (UISC == Pure_Killer किंवा
		UGC != Lo आणि UISC मध्ये [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	परत (UISC मध्ये [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] किंवा
		UGC != Lo आणि UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	परत (UDI आणि U नाही मध्ये [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		आणि UISC == Other
		आणि नाही is_CGJ(U, UISC, UDI, UGC, AJT)
	) किंवा UGC == Cn

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
	साठी U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) मध्ये data.वस्तुसंच():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		जर 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		जर 0x0F18 <= U <= 0x0F19 किंवा 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		जर U == 0x1CED: UISC = Tone_Mark

		मूल्ये = [k साठी k,v मध्ये वस्तुसंच जर v(U, UISC, UDI, UGC, AJT)]
		प्रतिज्ञा len(मूल्ये) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, मूल्ये)
		USE = मूल्ये[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		जर U मध्ये [0x11302, 0x11303, 0x114C1]: UIPC = शीर्ष

		प्रतिज्ञा (UIPC मध्ये [Not_Applicable, Visual_Order_डावा] किंवा
			U == 0x0F7F किंवा
			USE मध्ये use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		जर pos_mapping:
			मूल्ये = [k साठी k,v मध्ये pos_mapping.वस्तुसंच() जर v आणि UIPC मध्ये v]
			प्रतिज्ञा len(मूल्ये) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, मूल्ये)
			USE = USE + मूल्ये[0]

		out[U] = (USE, UBlock)
	परत out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
साठी h मध्ये headers:
	साठी l मध्ये h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
जर language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    साठी k,v मध्ये sorted(use_mapping.वस्तुसंच()):
        जर k मध्ये use_positions आणि use_positions[k]: सुरूठेवा
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    साठी k,v मध्ये sorted(use_positions.वस्तुसंच()):
        जर नाही v: सुरूठेवा
        साठी suf मध्ये v.किल्ल्या():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

किंवाजर language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
अन्यथा:
    प्रतिज्ञा असत्य, "Unknown language: %s" % language.name

uu = sorted (use_data.किल्ल्या ())

data = {u:v[0] साठी u,v मध्ये use_data.वस्तुसंच()}

जर language.name == "c":
    खाजगी = सत्य
किंवाजर language.name == "rust":
    खाजगी = असत्य
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
जर language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
अन्यथा:
    modes[DEFAULT] = ""

साठी step, text मध्ये modes.वस्तुसंच():
    print()
    जर text:
        print(text)
        print()
    जर step is None:
        सुरूठेवा

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, पूर्वनिर्धारित='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, खाजगी=खाजगी)
    code.print_code(language=language, खाजगी=खाजगी)
    print ()

जर language.name == "c":
    print ()
    साठी k मध्ये sorted(use_mapping.किल्ल्या()):
        जर k मध्ये use_positions आणि use_positions[k]: सुरूठेवा
        print ("#undef %s" % k)
    साठी k,v मध्ये sorted(use_positions.वस्तुसंच()):
        जर नाही v: सुरूठेवा
        साठी suf मध्ये v.किल्ल्या():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
किंवाजर language.name == "rust":
    pass
अन्यथा:
    प्रतिज्ञा असत्य, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
