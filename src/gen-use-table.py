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

درآمد os
درآمد packTab

درآمد sys
درآمد logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

اگر لمبائی(sys.argv) > 1 اور sys.argv[1] == "--rust":
    مٹاؤ sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
ورنہ:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

درآمد sys

اگر لمبائی (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') برائے x میں sys.argv[1:]]

سرخیاں = [[f.readline () برائے i میں سلسلہ (2)] برائے j,f میں enumerate(files) اگر j != 4]
برائے j میں سلسلہ(7, 9):
	برائے سطر میں files[j]:
		سطر = سطر.rstrip()
		اگر نہیں سطر:
			توڑو
		سرخیاں[j - 1].append(سطر)
سرخیاں.append (["UnicodeData.txt does not have a header."])

unicode_ڈیٹا = [{} برائے _ میں files]
اقدار = [{} برائے _ میں files]
برائے i, f میں enumerate (files):
	برائے سطر میں f:

		j = سطر.ڈھونڈو ('#')
		اگر j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () برائے x میں سطر.بانٹو (';')]
		اگر لمبائی (fields) == 1:
			جاری

		uu = fields[0].بانٹو ('..')
		آغاز = صحیح (uu[0], 16)
		اگر لمبائی (uu) == 1:
			انتہا = آغاز
		ورنہ:
			انتہا = صحیح (uu[1], 16)

		t = fields[1 اگر i نہیں میں [2, 4] ورنہ 2]

		اگر i == 2:
			t = 'jt_' + t
		وگرنہ_اگر i == 3 اور t != 'Default_Ignorable_Code_Point':
			جاری
		وگرنہ_اگر i == 7 اور t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		وگرنہ_اگر i == 8 اور t == 'NA':
			t = 'Not_Applicable'

		i0 = i اگر i < 7 ورنہ i - 7
		برائے u میں سلسلہ (آغاز, انتہا + 1):
			unicode_ڈیٹا[i0][u] = t
		اقدار[i0][t] = اقدار[i0].get (t, 0) + انتہا - آغاز + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
برائے i,v میں enumerate (defaults):
	اقدار[i][v] = اقدار[i].get (v, 0) + 1
combined = {}
برائے i,d میں enumerate (unicode_ڈیٹا):
	برائے u,v میں d.اشیاء ():
		اگر نہیں u میں combined:
			اگر i >= 4:
				جاری
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v برائے k, v میں combined.اشیاء() اگر v[6] نہیں میں DISABLED_SCRIPTS}


property_نام = [
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

جماعت Property_قدر(object):
	تعریف __init__(self, name_):
		self.name = name_
	تعریف __str__(self):
		واپس self.name
	تعریف __eq__(self, other):
		واپس self.name == (other اگر isinstance(other, str) ورنہ other.name)
	تعریف __ne__(self, other):
		واپس نہیں (self == other)
	تعریف __hash__(self):
		واپس hash(str(self))

property_اقدار = {}

برائے name میں property_نام:
	قدر_2 = Property_قدر(name)
	أكد قدر_2 نہیں میں property_اقدار
	أكد قدر_2 نہیں میں globals()
	property_اقدار[name] = قدر_2
globals().update(property_اقدار)


تعریف is_بنیاد(U, UISC, UDI, UGC, AJT):
	واپس (UISC میں [نمبر, Consonant, Consonant_سر_Letter,
			Tone_Letter,
			Vowel_Independent,
			] یا
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT میں [jt_C, jt_D, jt_L, jt_R] اور UISC != Joiner یا
		(UGC == Lo اور UISC میں [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
تعریف is_بنیاد_عدد(U, UISC, UDI, UGC, AJT):
	واپس UISC == Brahmi_Joining_نمبر
تعریف is_بنیاد_OTHER(U, UISC, UDI, UGC, AJT):
	اگر UISC == Consonant_Placeholder: واپس سچ
	واپس U میں [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
تعریف is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	واپس UISC == Joiner یا UDI اور UGC میں [Mc, Me, Mn]
تعریف is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	واپس ((UISC == Consonant_Final اور UGC != Lo) یا
		UISC == Consonant_Succeeding_Repha)
تعریف is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	واپس UISC == Syllable_Modifier
تعریف is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	واپس (UISC == Consonant_Medial اور UGC != Lo یا
		UISC == Consonant_Initial_Postfixed)
تعریف is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	واپس UISC میں [Nukta, Gemination_Mark, Consonant_Killer]
تعریف is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	واپس UISC == Consonant_Subjoined اور UGC != Lo
تعریف is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	واپس UISC == Consonant_With_Stacker
تعریف is_HALANT(U, UISC, UDI, UGC, AJT):
	واپس UISC == Virama اور نہیں is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
تعریف is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	واپس U == 0x0DCA
تعریف is_HALANT_عدد(U, UISC, UDI, UGC, AJT):
	واپس UISC == نمبر_Joiner
تعریف is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	واپس UISC == Hieroglyph
تعریف is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	واپس UISC == Hieroglyph_Joiner
تعریف is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	واپس UISC == Hieroglyph_Mirror
تعریف is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	واپس UISC == Hieroglyph_Modifier
تعریف is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	واپس UISC میں [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
تعریف is_HIEROGLYPH_SEGMENT_انتہا(U, UISC, UDI, UGC, AJT):
	واپس UISC میں [Hieroglyph_Mark_انتہا, Hieroglyph_Segment_انتہا]
تعریف is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	واپس (UISC == Invisible_Stacker
		اور نہیں is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
تعریف is_ZWNJ(U, UISC, UDI, UGC, AJT):
	واپس UISC == Non_Joiner
تعریف is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	واپس ((UGC == Po یا UISC میں [Consonant_Dead, Joiner, Modifying_Letter, Other])
		اور نہیں is_بنیاد(U, UISC, UDI, UGC, AJT)
		اور نہیں is_بنیاد_OTHER(U, UISC, UDI, UGC, AJT)
		اور نہیں is_CGJ(U, UISC, UDI, UGC, AJT)
		اور نہیں is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		اور نہیں is_لفظ_Joiner(U, UISC, UDI, UGC, AJT)
	)
تعریف is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	واپس UISC == Reordering_Killer
تعریف is_REPHA(U, UISC, UDI, UGC, AJT):
	واپس UISC میں [Consonant_Preceding_Repha, Consonant_Prefixed]
تعریف is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	واپس U == 0x1A60
تعریف is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	واپس UISC == علامت_Modifier
تعریف is_VOWEL(U, UISC, UDI, UGC, AJT):
	واپس (UISC == خالص_Killer یا
		UGC != Lo اور UISC میں [Vowel, Vowel_Dependent])
تعریف is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	واپس (UISC میں [Tone_Mark, Cantillation_Mark, رجسٹر_Shifter, Visarga] یا
		UGC != Lo اور UISC == Bindu)
تعریف is_لفظ_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	واپس (UDI اور U نہیں میں [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		اور UISC == Other
		اور نہیں is_CGJ(U, UISC, UDI, UGC, AJT)
	) یا UGC == Cn

use_mapping = {
	'B':	is_بنیاد,
	'N':	is_بنیاد_عدد,
	'GB':	is_بنیاد_OTHER,
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
	'SE':	is_HIEROGLYPH_SEGMENT_انتہا,
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
	'H': کچھ_نہیں,
	'HM': کچھ_نہیں,
	'HR': کچھ_نہیں,
	'HVM': کچھ_نہیں,
	'IS': کچھ_نہیں,
	'B': کچھ_نہیں,
	'FM': {
		'Abv': [چوٹی],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': کچھ_نہیں,
	'RK': کچھ_نہیں,
	'SUB': کچھ_نہیں,
}

تعریف map_مقصد_use(ڈیٹا):
	برآمد = {}
	اشیاء = use_mapping.اشیاء()
	برائے U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) میں ڈیٹا.اشیاء():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		اگر 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		اگر 0x0F18 <= U <= 0x0F19 یا 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		اگر U == 0x1CED: UISC = Tone_Mark

		اقدار = [k برائے k,v میں اشیاء اگر v(U, UISC, UDI, UGC, AJT)]
		أكد لمبائی(اقدار) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, اقدار)
		USE = اقدار[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		اگر U میں [0x11302, 0x11303, 0x114C1]: UIPC = چوٹی

		أكد (UIPC میں [Not_Applicable, Visual_ترتیب_بایاں] یا
			U == 0x0F7F یا
			USE میں use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, کچھ_نہیں)
		اگر pos_mapping:
			اقدار = [k برائے k,v میں pos_mapping.اشیاء() اگر v اور UIPC میں v]
			أكد لمبائی(اقدار) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, اقدار)
			USE = USE + اقدار[0]

		برآمد[U] = (USE, UBlock)
	واپس برآمد

use_ڈیٹا = map_مقصد_use(combined)

چھاپو ("/* == Start of generated table == */")
چھاپو ("/*")
چھاپو (" * The following table is generated by running:")
چھاپو (" *")
چھاپو (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.راستہ.basename (sys.argv[0])))
چھاپو (" *")
چھاپو (" * on files with these headers:")
چھاپو (" *")
برائے h میں سرخیاں:
	برائے l میں h:
		چھاپو (" * %s" % (l.strip()))
چھاپو (" */")
چھاپو ()
اگر language.name == "c":
    چھاپو ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    چھاپو ("#define HB_OT_SHAPER_USE_TABLE_HH")
    چھاپو ()
    چھاپو ('#include "hb.hh"')
    چھاپو ()
    چھاپو ('#include "hb-ot-shaper-use-machine.hh"')
    چھاپو ()

    چھاپو ('#pragma GCC diagnostic push')
    چھاپو ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    برائے k,v میں sorted(use_mapping.اشیاء()):
        اگر k میں use_positions اور use_positions[k]: جاری
        چھاپو ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    برائے k,v میں sorted(use_positions.اشیاء()):
        اگر نہیں v: جاری
        برائے suf میں v.کلیدیں():
            لیبل_2 = k + suf
            چھاپو ("#define %s	USE(%s)" % (لیبل_2, لیبل_2))
    چھاپو ('#pragma GCC diagnostic pop')
    چھاپو ("")

وگرنہ_اگر language.name == "rust":
    چھاپو()
    چھاپو("#![allow(unused_parens)]")
    چھاپو("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    چھاپو()
    چھاپو("use super::ot_shaper_use::category::*;")
    چھاپو()
ورنہ:
    أكد جھوٹ, "Unknown language: %s" % language.name

uu = sorted (use_ڈیٹا.کلیدیں ())

ڈیٹا = {u:v[0] برائے u,v میں use_ڈیٹا.اشیاء()}

اگر language.name == "c":
    نجی = سچ
وگرنہ_اگر language.name == "rust":
    نجی = جھوٹ
    language.عوامی_function_linkage = "pub(crate)"
ورنہ:
    أكد جھوٹ, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
اگر language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[کچھ_نہیں] = "#endif"
ورنہ:
    modes[DEFAULT] = ""

برائے قدم, متن میں modes.اشیاء():
    چھاپو()
    اگر متن:
        چھاپو(متن)
        چھاپو()
    اگر قدم ہے کچھ_نہیں:
        جاری

    compression = compression_level[قدم]
    logging.info("  Compression=%d:" % compression)

    رمز = packTab.رمز_2('hb_use')
    sol = packTab.pack_جدول(ڈیٹا, compression=compression, طےشدہ='O')
    logging.info('      FullCost=%d' % (sol.بھرا_لاگت))
    sol.gen_رمز(رمز, f'get_category', language=language, نجی=نجی)
    رمز.print_رمز(language=language, نجی=نجی)
    چھاپو ()

اگر language.name == "c":
    چھاپو ()
    برائے k میں sorted(use_mapping.کلیدیں()):
        اگر k میں use_positions اور use_positions[k]: جاری
        چھاپو ("#undef %s" % k)
    برائے k,v میں sorted(use_positions.اشیاء()):
        اگر نہیں v: جاری
        برائے suf میں v.کلیدیں():
            لیبل_2 = k + suf
            چھاپو ("#undef %s" % لیبل_2)
    چھاپو ()
    چھاپو ()
    چھاپو ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وگرنہ_اگر language.name == "rust":
    گزرو
ورنہ:
    أكد جھوٹ, "Unknown language: %s" % language.name
چھاپو ("/* == End of generated table == */")
