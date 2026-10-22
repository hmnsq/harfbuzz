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

अगर len(sys.argv) > 1 and sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
अन्यथा:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

आयात sys

अगर len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') क्रम x in sys.argv[1:]]

headers = [[f.readline () क्रम i in range (2)] क्रम j,f in enumerate(files) अगर j != 4]
क्रम j in range(7, 9):
	क्रम पंक्ति in files[j]:
		पंक्ति = पंक्ति.rstrip()
		अगर not पंक्ति:
			अवरोध
		headers[j - 1].append(पंक्ति)
headers.append (["UnicodeData.txt does not have a header."])

unicode_तथ्य = [{} क्रम _ in files]
मूल्यावली = [{} क्रम _ in files]
क्रम i, f in enumerate (files):
	क्रम पंक्ति in f:

		j = पंक्ति.find ('#')
		अगर j >= 0:
			पंक्ति = पंक्ति[:j]

		fields = [x.strip () क्रम x in पंक्ति.split (';')]
		अगर len (fields) == 1:
			जारी

		uu = fields[0].split ('..')
		शुरू = पूर्णांक (uu[0], 16)
		अगर len (uu) == 1:
			end = शुरू
		अन्यथा:
			end = पूर्णांक (uu[1], 16)

		t = fields[1 अगर i not in [2, 4] अन्यथा 2]

		अगर i == 2:
			t = 'jt_' + t
		elif i == 3 and t != 'Default_Ignorable_Code_Point':
			जारी
		elif i == 7 and t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		elif i == 8 and t == 'NA':
			t = 'Not_Applicable'

		i0 = i अगर i < 7 अन्यथा i - 7
		क्रम u in range (शुरू, end + 1):
			unicode_तथ्य[i0][u] = t
		मूल्यावली[i0][t] = मूल्यावली[i0].get (t, 0) + end - शुरू + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
क्रम i,v in enumerate (defaults):
	मूल्यावली[i][v] = मूल्यावली[i].get (v, 0) + 1
combined = {}
क्रम i,d in enumerate (unicode_तथ्य):
	क्रम u,v in d.वस्तुएँ ():
		अगर not u in combined:
			अगर i >= 4:
				जारी
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v क्रम k, v in combined.वस्तुएँ() अगर v[6] not in DISABLED_SCRIPTS}


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

श्रेणी Property_मूल्य(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		वापस self.name
	def __eq__(self, other):
		वापस self.name == (other अगर isinstance(other, str) अन्यथा other.name)
	def __ne__(self, other):
		वापस not (self == other)
	def __hash__(self):
		वापस hash(str(self))

property_मूल्यावली = {}

क्रम name in property_names:
	मूल्य_2 = Property_मूल्य(name)
	assert मूल्य_2 not in property_मूल्यावली
	assert मूल्य_2 not in globals()
	property_मूल्यावली[name] = मूल्य_2
globals().नया_हाल(property_मूल्यावली)


def is_BASE(U, UISC, UDI, UGC, AJT):
	वापस (UISC in [संख्या, Consonant, Consonant_शीर्ष_Letter,
			Tone_Letter,
			Vowel_Independent,
			] or
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT in [jt_C, jt_D, jt_L, jt_R] and UISC != Joiner or
		(UGC == Lo and UISC in [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_अंक(U, UISC, UDI, UGC, AJT):
	वापस UISC == Brahmi_Joining_संख्या
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	अगर UISC == Consonant_Placeholder: वापस True
	वापस U in [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	वापस UISC == Joiner or UDI and UGC in [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	वापस ((UISC == Consonant_Final and UGC != Lo) or
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	वापस UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	वापस (UISC == Consonant_Medial and UGC != Lo or
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	वापस UISC in [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	वापस UISC == Consonant_Subjoined and UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	वापस UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	वापस UISC == Virama and not is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	वापस U == 0x0DCA
def is_HALANT_अंक(U, UISC, UDI, UGC, AJT):
	वापस UISC == संख्या_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	वापस UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	वापस UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	वापस UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	वापस UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	वापस UISC in [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	वापस UISC in [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	वापस (UISC == Invisible_Stacker
		and not is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	वापस UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	वापस ((UGC == Po or UISC in [Consonant_Dead, Joiner, Modifying_Letter, Other])
		and not is_BASE(U, UISC, UDI, UGC, AJT)
		and not is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		and not is_CGJ(U, UISC, UDI, UGC, AJT)
		and not is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		and not is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	वापस UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	वापस UISC in [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	वापस U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	वापस UISC == प्रतीक_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	वापस (UISC == Pure_Killer or
		UGC != Lo and UISC in [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	वापस (UISC in [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] or
		UGC != Lo and UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	वापस (UDI and U not in [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		and UISC == Other
		and not is_CGJ(U, UISC, UDI, UGC, AJT)
	) or UGC == Cn

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
		'Abv': [शीर्ष_3],
		'Blw': [Bottom],
		'Pst': [दायाँ],
	},
	'M': {
		'Abv': [शीर्ष_3],
		'Blw': [Bottom, Bottom_And_बायाँ, Bottom_And_दायाँ],
		'Pst': [दायाँ],
		'Pre': [बायाँ, शीर्ष_And_Bottom_And_बायाँ],
	},
	'CM': {
		'Abv': [शीर्ष_3],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [शीर्ष_3, शीर्ष_And_Bottom, शीर्ष_And_Bottom_And_दायाँ, शीर्ष_And_दायाँ],
		'Blw': [Bottom, Overstruck, Bottom_And_दायाँ],
		'Pst': [दायाँ],
		'Pre': [बायाँ, शीर्ष_And_बायाँ, शीर्ष_And_बायाँ_And_दायाँ, बायाँ_And_दायाँ],
	},
	'VM': {
		'Abv': [शीर्ष_3],
		'Blw': [Bottom, Overstruck],
		'Pst': [दायाँ],
		'Pre': [बायाँ],
	},
	'SM': {
		'Abv': [शीर्ष_3],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [शीर्ष_3],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(तथ्य):
	बाहर = {}
	वस्तुएँ = use_mapping.वस्तुएँ()
	क्रम U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) in तथ्य.वस्तुएँ():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		अगर 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		अगर 0x0F18 <= U <= 0x0F19 or 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		अगर U == 0x1CED: UISC = Tone_Mark

		मूल्यावली = [k क्रम k,v in वस्तुएँ अगर v(U, UISC, UDI, UGC, AJT)]
		assert len(मूल्यावली) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, मूल्यावली)
		USE = मूल्यावली[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		अगर U in [0x11302, 0x11303, 0x114C1]: UIPC = शीर्ष_3

		assert (UIPC in [Not_Applicable, Visual_Order_बायाँ] or
			U == 0x0F7F or
			USE in use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		अगर pos_mapping:
			मूल्यावली = [k क्रम k,v in pos_mapping.वस्तुएँ() अगर v and UIPC in v]
			assert len(मूल्यावली) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, मूल्यावली)
			USE = USE + मूल्यावली[0]

		बाहर[U] = (USE, UBlock)
	वापस बाहर

use_तथ्य = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
क्रम h in headers:
	क्रम l in h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
अगर language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    क्रम k,v in sorted(use_mapping.वस्तुएँ()):
        अगर k in use_positions and use_positions[k]: जारी
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    क्रम k,v in sorted(use_positions.वस्तुएँ()):
        अगर not v: जारी
        क्रम suf in v.कुंजियाँ():
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
अन्यथा:
    assert False, "Unknown language: %s" % language.name

uu = sorted (use_तथ्य.कुंजियाँ ())

तथ्य = {u:v[0] क्रम u,v in use_तथ्य.वस्तुएँ()}

अगर language.name == "c":
    निजी = True
elif language.name == "rust":
    निजी = False
    language.public_function_linkage = "pub(crate)"
अन्यथा:
    assert False, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
अगर language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
अन्यथा:
    modes[DEFAULT] = ""

क्रम step, text in modes.वस्तुएँ():
    print()
    अगर text:
        print(text)
        print()
    अगर step is None:
        जारी

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(तथ्य, compression=compression, शेष='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, निजी=निजी)
    code.print_code(language=language, निजी=निजी)
    print ()

अगर language.name == "c":
    print ()
    क्रम k in sorted(use_mapping.कुंजियाँ()):
        अगर k in use_positions and use_positions[k]: जारी
        print ("#undef %s" % k)
    क्रम k,v in sorted(use_positions.वस्तुएँ()):
        अगर not v: जारी
        क्रम suf in v.कुंजियाँ():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
elif language.name == "rust":
    pass
अन्यथा:
    assert False, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
