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

إذا لمبائی(sys.argv) > 1 و sys.argv[1] == "--rust":
    ہٹاؤ sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وإلا:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

استورد sys

إذا لمبائی (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرخیاں = [[f.readline () لكل i في سلسلہ (2)] لكل j,f في enumerate(files) إذا j != 4]
لكل j في سلسلہ(7, 9):
	لكل سطر في files[j]:
		سطر = سطر.rstrip()
		إذا ليس سطر:
			اكسر
		سرخیاں[j - 1].append(سطر)
سرخیاں.append (["UnicodeData.txt does not have a header."])

unicode_انکڑے = [{} لكل _ في files]
اقدار = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل سطر في f:

		j = سطر.لبھو ('#')
		إذا j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () لكل x في سطر.ونڈو (';')]
		إذا لمبائی (fields) == 1:
			استمر

		uu = fields[0].ونڈو ('..')
		شروع = صحيح (uu[0], 16)
		إذا لمبائی (uu) == 1:
			سرا = شروع
		وإلا:
			سرا = صحيح (uu[1], 16)

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
		لكل u في سلسلہ (شروع, سرا + 1):
			unicode_انکڑے[i0][u] = t
		اقدار[i0][t] = اقدار[i0].get (t, 0) + سرا - شروع + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
لكل i,v في enumerate (defaults):
	اقدار[i][v] = اقدار[i].get (v, 0) + 1
combined = {}
لكل i,d في enumerate (unicode_انکڑے):
	لكل u,v في d.اشیاء ():
		إذا ليس u في combined:
			إذا i >= 4:
				استمر
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v لكل k, v في combined.اشیاء() إذا v[6] ليس في DISABLED_SCRIPTS}


property_ناں = [
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

صنف Property_قدر(object):
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

property_اقدار = {}

لكل name في property_ناں:
	قدر_2 = Property_قدر(name)
	أكد قدر_2 ليس في property_اقدار
	أكد قدر_2 ليس في globals()
	property_اقدار[name] = قدر_2
globals().update(property_اقدار)


تعریف is_ادھار(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [نمبر, Consonant, Consonant_سر_Letter,
			Tone_Letter,
			Vowel_Independent,
			] أو
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT في [jt_C, jt_D, jt_L, jt_R] و UISC != Joiner أو
		(UGC == Lo و UISC في [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
تعریف is_ادھار_عدد(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Brahmi_Joining_نمبر
تعریف is_ادھار_OTHER(U, UISC, UDI, UGC, AJT):
	إذا UISC == Consonant_Placeholder: أرجع سچ
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
	أرجع UISC == نمبر_Joiner
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
تعریف is_HIEROGLYPH_SEGMENT_سرا(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_سرا, Hieroglyph_Segment_سرا]
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
		و ليس is_ادھار(U, UISC, UDI, UGC, AJT)
		و ليس is_ادھار_OTHER(U, UISC, UDI, UGC, AJT)
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
		و ليس is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		و ليس is_لفظ_Joiner(U, UISC, UDI, UGC, AJT)
	)
تعریف is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Reordering_Killer
تعریف is_REPHA(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Consonant_Preceding_Repha, Consonant_Prefixed]
تعریف is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x1A60
تعریف is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == علامت_Modifier
تعریف is_VOWEL(U, UISC, UDI, UGC, AJT):
	أرجع (UISC == خالص_Killer أو
		UGC != Lo و UISC في [Vowel, Vowel_Dependent])
تعریف is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Tone_Mark, Cantillation_Mark, رجسٹر_Shifter, Visarga] أو
		UGC != Lo و UISC == Bindu)
تعریف is_لفظ_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	أرجع (UDI و U ليس في [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		و UISC == Other
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
	) أو UGC == Cn

use_mapping = {
	'B':	is_ادھار,
	'N':	is_ادھار_عدد,
	'GB':	is_ادھار_OTHER,
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
	'SE':	is_HIEROGLYPH_SEGMENT_سرا,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_لفظ_Joiner,
}

use_positions = {
	'F': {
		'Abv': [چوٹی],
		'Blw': [Bottom],
		'Pst': [دایاں],
	},
	'M': {
		'Abv': [چوٹی],
		'Blw': [Bottom, Bottom_And_بایاں, Bottom_And_دایاں],
		'Pst': [دایاں],
		'Pre': [بایاں, چوٹی_And_Bottom_And_بایاں],
	},
	'CM': {
		'Abv': [چوٹی],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [چوٹی, چوٹی_And_Bottom, چوٹی_And_Bottom_And_دایاں, چوٹی_And_دایاں],
		'Blw': [Bottom, Overstruck, Bottom_And_دایاں],
		'Pst': [دایاں],
		'Pre': [بایاں, چوٹی_And_بایاں, چوٹی_And_بایاں_And_دایاں, بایاں_And_دایاں],
	},
	'VM': {
		'Abv': [چوٹی],
		'Blw': [Bottom, Overstruck],
		'Pst': [دایاں],
		'Pre': [بایاں],
	},
	'SM': {
		'Abv': [چوٹی],
		'Blw': [Bottom],
	},
	'H': کجھ_نئیں,
	'HM': کجھ_نئیں,
	'HR': کجھ_نئیں,
	'HVM': کجھ_نئیں,
	'IS': کجھ_نئیں,
	'B': کجھ_نئیں,
	'FM': {
		'Abv': [چوٹی],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': کجھ_نئیں,
	'RK': کجھ_نئیں,
	'SUB': کجھ_نئیں,
}

تعریف map_منزل_use(انکڑے):
	باہرلا = {}
	اشیاء = use_mapping.اشیاء()
	لكل U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) في انکڑے.اشیاء():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x0F18 <= U <= 0x0F19 أو 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		إذا U == 0x1CED: UISC = Tone_Mark

		اقدار = [k لكل k,v في اشیاء إذا v(U, UISC, UDI, UGC, AJT)]
		أكد لمبائی(اقدار) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, اقدار)
		USE = اقدار[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		إذا U في [0x11302, 0x11303, 0x114C1]: UIPC = چوٹی

		أكد (UIPC في [Not_Applicable, Visual_ترتیب_بایاں] أو
			U == 0x0F7F أو
			USE في use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, کجھ_نئیں)
		إذا pos_mapping:
			اقدار = [k لكل k,v في pos_mapping.اشیاء() إذا v و UIPC في v]
			أكد لمبائی(اقدار) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, اقدار)
			USE = USE + اقدار[0]

		باہرلا[U] = (USE, UBlock)
	أرجع باہرلا

use_انکڑے = map_منزل_use(combined)

چھاپو ("/* == Start of generated table == */")
چھاپو ("/*")
چھاپو (" * The following table is generated by running:")
چھاپو (" *")
چھاپو (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.رستہ.basename (sys.argv[0])))
چھاپو (" *")
چھاپو (" * on files with these headers:")
چھاپو (" *")
لكل h في سرخیاں:
	لكل l في h:
		چھاپو (" * %s" % (l.strip()))
چھاپو (" */")
چھاپو ()
إذا language.name == "c":
    چھاپو ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    چھاپو ("#define HB_OT_SHAPER_USE_TABLE_HH")
    چھاپو ()
    چھاپو ('#include "hb.hh"')
    چھاپو ()
    چھاپو ('#include "hb-ot-shaper-use-machine.hh"')
    چھاپو ()

    چھاپو ('#pragma GCC diagnostic push')
    چھاپو ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    لكل k,v في sorted(use_mapping.اشیاء()):
        إذا k في use_positions و use_positions[k]: استمر
        چھاپو ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    لكل k,v في sorted(use_positions.اشیاء()):
        إذا ليس v: استمر
        لكل suf في v.کلیدیں():
            ٹیگ_2 = k + suf
            چھاپو ("#define %s	USE(%s)" % (ٹیگ_2, ٹیگ_2))
    چھاپو ('#pragma GCC diagnostic pop')
    چھاپو ("")

وإلاإذا language.name == "rust":
    چھاپو()
    چھاپو("#![allow(unused_parens)]")
    چھاپو("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    چھاپو()
    چھاپو("use super::ot_shaper_use::category::*;")
    چھاپو()
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name

uu = sorted (use_انکڑے.کلیدیں ())

انکڑے = {u:v[0] لكل u,v في use_انکڑے.اشیاء()}

إذا language.name == "c":
    خاص = سچ
وإلاإذا language.name == "rust":
    خاص = خطأ
    language.عوامی_function_linkage = "pub(crate)"
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
    modes[کجھ_نئیں] = "#endif"
وإلا:
    modes[DEFAULT] = ""

لكل قدم, متن في modes.اشیاء():
    چھاپو()
    إذا متن:
        چھاپو(متن)
        چھاپو()
    إذا قدم اے کجھ_نئیں:
        استمر

    compression = compression_level[قدم]
    logging.info("  Compression=%d:" % compression)

    کوڈ = packTab.کوڈ_2('hb_use')
    sol = packTab.pack_جدول(انکڑے, compression=compression, افتراضي='O')
    logging.info('      FullCost=%d' % (sol.بھریا_لاگت))
    sol.gen_کوڈ(کوڈ, f'get_category', language=language, خاص=خاص)
    کوڈ.print_کوڈ(language=language, خاص=خاص)
    چھاپو ()

إذا language.name == "c":
    چھاپو ()
    لكل k في sorted(use_mapping.کلیدیں()):
        إذا k في use_positions و use_positions[k]: استمر
        چھاپو ("#undef %s" % k)
    لكل k,v في sorted(use_positions.اشیاء()):
        إذا ليس v: استمر
        لكل suf في v.کلیدیں():
            ٹیگ_2 = k + suf
            چھاپو ("#undef %s" % ٹیگ_2)
    چھاپو ()
    چھاپو ()
    چھاپو ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وإلاإذا language.name == "rust":
    لنگھو
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name
چھاپو ("/* == End of generated table == */")
