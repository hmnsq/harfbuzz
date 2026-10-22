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

إذا اوږدوالی(sys.argv) > 1 و sys.argv[1] == "--rust":
    لرېکړه sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وإلا:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

استورد sys

إذا اوږدوالی (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرلیکونه = [[f.readline () لكل i في لړ (2)] لكل j,f في enumerate(files) إذا j != 4]
لكل j في لړ(7, 9):
	لكل کرښه في files[j]:
		کرښه = کرښه.rstrip()
		إذا ليس کرښه:
			اكسر
		سرلیکونه[j - 1].append(کرښه)
سرلیکونه.append (["UnicodeData.txt does not have a header."])

unicode_معلومات = [{} لكل _ في files]
ارزښتونه = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل کرښه في f:

		j = کرښه.پیدا_کړه ('#')
		إذا j >= 0:
			کرښه = کرښه[:j]

		fields = [x.strip () لكل x في کرښه.وېشل (';')]
		إذا اوږدوالی (fields) == 1:
			استمر

		uu = fields[0].وېشل ('..')
		پیل = صحيح (uu[0], 16)
		إذا اوږدوالی (uu) == 1:
			پای_ټکی = پیل
		وإلا:
			پای_ټکی = صحيح (uu[1], 16)

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
		لكل u في لړ (پیل, پای_ټکی + 1):
			unicode_معلومات[i0][u] = t
		ارزښتونه[i0][t] = ارزښتونه[i0].get (t, 0) + پای_ټکی - پیل + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
لكل i,v في enumerate (defaults):
	ارزښتونه[i][v] = ارزښتونه[i].get (v, 0) + 1
combined = {}
لكل i,d في enumerate (unicode_معلومات):
	لكل u,v في d.توکي ():
		إذا ليس u في combined:
			إذا i >= 4:
				استمر
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v لكل k, v في combined.توکي() إذا v[6] ليس في DISABLED_SCRIPTS}


property_نومونه = [
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

صنف Property_ارزښت(object):
	تعریف __init__(self, name_):
		self.name = name_
	تعریف __str__(self):
		أرجع self.name
	تعریف __eq__(self, other):
		أرجع self.name == (other إذا isinstance(other, str) وإلا other.name)
	تعریف __ne__(self, other):
		أرجع ليس (self == other)
	تعریف __hash__(self):
		أرجع hash(str(self))

property_ارزښتونه = {}

لكل name في property_نومونه:
	ارزښت_2 = Property_ارزښت(name)
	أكد ارزښت_2 ليس في property_ارزښتونه
	أكد ارزښت_2 ليس في globals()
	property_ارزښتونه[name] = ارزښت_2
globals().update(property_ارزښتونه)


تعریف is_بنسټ(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [شمېره, Consonant, Consonant_سر_Letter,
			Tone_Letter,
			Vowel_Independent,
			] أو
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT في [jt_C, jt_D, jt_L, jt_R] و UISC != Joiner أو
		(UGC == Lo و UISC في [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
تعریف is_بنسټ_عدد(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Brahmi_Joining_شمېره
تعریف is_بنسټ_OTHER(U, UISC, UDI, UGC, AJT):
	إذا UISC == Consonant_Placeholder: أرجع ریښتیا
	أرجع U في [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
تعریف is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	أرجع UISC == Joiner أو UDI و UGC في [Mc, Me, Mn]
تعریف is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	أرجع ((UISC == Consonant_Final و UGC != Lo) أو
		UISC == Consonant_Succeeding_Repha)
تعریف is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Syllable_Modifier
تعریف is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	أرجع (UISC == Consonant_Medial و UGC != Lo أو
		UISC == Consonant_Initial_Postfixed)
تعریف is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Nukta, Gemination_Mark, Consonant_Killer]
تعریف is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_Subjoined و UGC != Lo
تعریف is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_With_Stacker
تعریف is_HALANT(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Virama و ليس is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
تعریف is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x0DCA
تعریف is_HALANT_عدد(U, UISC, UDI, UGC, AJT):
	أرجع UISC == شمېره_Joiner
تعریف is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph
تعریف is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Joiner
تعریف is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Mirror
تعریف is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Modifier
تعریف is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
تعریف is_HIEROGLYPH_SEGMENT_پای_ټکی(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_پای_ټکی, Hieroglyph_Segment_پای_ټکی]
تعریف is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع (UISC == Invisible_Stacker
		و ليس is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
تعریف is_ZWNJ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Non_Joiner
تعریف is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	أرجع ((UGC == Po أو UISC في [Consonant_Dead, Joiner, Modifying_Letter, Other])
		و ليس is_بنسټ(U, UISC, UDI, UGC, AJT)
		و ليس is_بنسټ_OTHER(U, UISC, UDI, UGC, AJT)
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
		و ليس is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		و ليس is_کلمه_Joiner(U, UISC, UDI, UGC, AJT)
	)
تعریف is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Reordering_Killer
تعریف is_REPHA(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Consonant_Preceding_Repha, Consonant_Prefixed]
تعریف is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x1A60
تعریف is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == نښه_Modifier
تعریف is_VOWEL(U, UISC, UDI, UGC, AJT):
	أرجع (UISC == خالص_Killer أو
		UGC != Lo و UISC في [Vowel, Vowel_Dependent])
تعریف is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Tone_Mark, Cantillation_Mark, ثبتوونکی_Shifter, Visarga] أو
		UGC != Lo و UISC == Bindu)
تعریف is_کلمه_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	أرجع (UDI و U ليس في [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		و UISC == Other
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
	) أو UGC == Cn

use_mapping = {
	'B':	is_بنسټ,
	'N':	is_بنسټ_عدد,
	'GB':	is_بنسټ_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_عدد,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_پای_ټکی,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_کلمه_Joiner,
}

use_positions = {
	'F': {
		'Abv': [سر_3],
		'Blw': [Bottom],
		'Pst': [ښی_2],
	},
	'M': {
		'Abv': [سر_3],
		'Blw': [Bottom, Bottom_And_کیڼ, Bottom_And_ښی],
		'Pst': [ښی_2],
		'Pre': [کیڼ_2, سر_And_Bottom_And_کیڼ],
	},
	'CM': {
		'Abv': [سر_3],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [سر_3, سر_And_Bottom, سر_And_Bottom_And_ښی, سر_And_ښی],
		'Blw': [Bottom, Overstruck, Bottom_And_ښی],
		'Pst': [ښی_2],
		'Pre': [کیڼ_2, سر_And_کیڼ, سر_And_کیڼ_And_ښی, کیڼ_And_ښی],
	},
	'VM': {
		'Abv': [سر_3],
		'Blw': [Bottom, Overstruck],
		'Pst': [ښی_2],
		'Pre': [کیڼ_2],
	},
	'SM': {
		'Abv': [سر_3],
		'Blw': [Bottom],
	},
	'H': هیڅ,
	'HM': هیڅ,
	'HR': هیڅ,
	'HVM': هیڅ,
	'IS': هیڅ,
	'B': هیڅ,
	'FM': {
		'Abv': [سر_3],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': هیڅ,
	'RK': هیڅ,
	'SUB': هیڅ,
}

تعریف map_منزل_use(معلومات):
	بهر = {}
	توکي = use_mapping.توکي()
	لكل U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) في معلومات.توکي():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x0F18 <= U <= 0x0F19 أو 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		إذا U == 0x1CED: UISC = Tone_Mark

		ارزښتونه = [k لكل k,v في توکي إذا v(U, UISC, UDI, UGC, AJT)]
		أكد اوږدوالی(ارزښتونه) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, ارزښتونه)
		USE = ارزښتونه[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		إذا U في [0x11302, 0x11303, 0x114C1]: UIPC = سر_3

		أكد (UIPC في [Not_Applicable, Visual_ترتیب_کیڼ] أو
			U == 0x0F7F أو
			USE في use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, هیڅ)
		إذا pos_mapping:
			ارزښتونه = [k لكل k,v في pos_mapping.توکي() إذا v و UIPC في v]
			أكد اوږدوالی(ارزښتونه) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, ارزښتونه)
			USE = USE + ارزښتونه[0]

		بهر[U] = (USE, UBlock)
	أرجع بهر

use_معلومات = map_منزل_use(combined)

چاپ ("/* == Start of generated table == */")
چاپ ("/*")
چاپ (" * The following table is generated by running:")
چاپ (" *")
چاپ (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.لار.basename (sys.argv[0])))
چاپ (" *")
چاپ (" * on files with these headers:")
چاپ (" *")
لكل h في سرلیکونه:
	لكل l في h:
		چاپ (" * %s" % (l.strip()))
چاپ (" */")
چاپ ()
إذا language.name == "c":
    چاپ ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    چاپ ("#define HB_OT_SHAPER_USE_TABLE_HH")
    چاپ ()
    چاپ ('#include "hb.hh"')
    چاپ ()
    چاپ ('#include "hb-ot-shaper-use-machine.hh"')
    چاپ ()

    چاپ ('#pragma GCC diagnostic push')
    چاپ ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    لكل k,v في sorted(use_mapping.توکي()):
        إذا k في use_positions و use_positions[k]: استمر
        چاپ ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    لكل k,v في sorted(use_positions.توکي()):
        إذا ليس v: استمر
        لكل suf في v.کلیګانې():
            نښان_2 = k + suf
            چاپ ("#define %s	USE(%s)" % (نښان_2, نښان_2))
    چاپ ('#pragma GCC diagnostic pop')
    چاپ ("")

وإلاإذا language.name == "rust":
    چاپ()
    چاپ("#![allow(unused_parens)]")
    چاپ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    چاپ()
    چاپ("use super::ot_shaper_use::category::*;")
    چاپ()
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name

uu = sorted (use_معلومات.کلیګانې ())

معلومات = {u:v[0] لكل u,v في use_معلومات.توکي()}

إذا language.name == "c":
    خاص = ریښتیا
وإلاإذا language.name == "rust":
    خاص = خطأ
    language.عامه_function_linkage = "pub(crate)"
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
    modes[هیڅ] = "#endif"
وإلا:
    modes[DEFAULT] = ""

لكل ګام, متن في modes.توکي():
    چاپ()
    إذا متن:
        چاپ(متن)
        چاپ()
    إذا ګام دی هیڅ:
        استمر

    compression = compression_level[ګام]
    logging.info("  Compression=%d:" % compression)

    کوډ = packTab.کوډ_2('hb_use')
    sol = packTab.pack_جدول(معلومات, compression=compression, افتراضي='O')
    logging.info('      FullCost=%d' % (sol.ډک_لګښت))
    sol.gen_کوډ(کوډ, f'get_category', language=language, خاص=خاص)
    کوډ.print_کوډ(language=language, خاص=خاص)
    چاپ ()

إذا language.name == "c":
    چاپ ()
    لكل k في sorted(use_mapping.کلیګانې()):
        إذا k في use_positions و use_positions[k]: استمر
        چاپ ("#undef %s" % k)
    لكل k,v في sorted(use_positions.توکي()):
        إذا ليس v: استمر
        لكل suf في v.کلیګانې():
            نښان_2 = k + suf
            چاپ ("#undef %s" % نښان_2)
    چاپ ()
    چاپ ()
    چاپ ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وإلاإذا language.name == "rust":
    تیرشه
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name
چاپ ("/* == End of generated table == */")
