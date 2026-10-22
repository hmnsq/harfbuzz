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

إذا ڊيگهه(sys.argv) > 1 و sys.argv[1] == "--rust":
    هٽايو sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
وإلا:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

استورد sys

إذا ڊيگهه (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرا = [[f.readline () لكل i في سلسلو (2)] لكل j,f في enumerate(files) إذا j != 4]
لكل j في سلسلو(7, 9):
	لكل سٽ في files[j]:
		سٽ = سٽ.rstrip()
		إذا ليس سٽ:
			اكسر
		سرا[j - 1].append(سٽ)
سرا.append (["UnicodeData.txt does not have a header."])

unicode_معلومات = [{} لكل _ في files]
قدرون = [{} لكل _ في files]
لكل i, f في enumerate (files):
	لكل سٽ في f:

		j = سٽ.ڳوليو ('#')
		إذا j >= 0:
			سٽ = سٽ[:j]

		fields = [x.strip () لكل x في سٽ.ورهايو (';')]
		إذا ڊيگهه (fields) == 1:
			استمر

		uu = fields[0].ورهايو ('..')
		شروعات = صحيح (uu[0], 16)
		إذا ڊيگهه (uu) == 1:
			پڇاڙي = شروعات
		وإلا:
			پڇاڙي = صحيح (uu[1], 16)

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
		لكل u في سلسلو (شروعات, پڇاڙي + 1):
			unicode_معلومات[i0][u] = t
		قدرون[i0][t] = قدرون[i0].get (t, 0) + پڇاڙي - شروعات + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
لكل i,v في enumerate (defaults):
	قدرون[i][v] = قدرون[i].get (v, 0) + 1
combined = {}
لكل i,d في enumerate (unicode_معلومات):
	لكل u,v في d.شيون ():
		إذا ليس u في combined:
			إذا i >= 4:
				استمر
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v لكل k, v في combined.شيون() إذا v[6] ليس في DISABLED_SCRIPTS}


property_نالا = [
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
	وصف __init__(self, name_):
		self.name = name_
	وصف __str__(self):
		أرجع self.name
	وصف __eq__(self, other):
		أرجع self.name == (other إذا isinstance(other, str) وإلا other.name)
	وصف __ne__(self, other):
		أرجع ليس (self == other)
	وصف __hash__(self):
		أرجع hash(str(self))

property_قدرون = {}

لكل name في property_نالا:
	قدر_2 = Property_قدر(name)
	أكد قدر_2 ليس في property_قدرون
	أكد قدر_2 ليس في globals()
	property_قدرون[name] = قدر_2
globals().update(property_قدرون)


وصف is_بنياد(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [عدد, Consonant, Consonant_سر_Letter,
			Tone_Letter,
			Vowel_Independent,
			] أو
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT في [jt_C, jt_D, jt_L, jt_R] و UISC != Joiner أو
		(UGC == Lo و UISC في [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
وصف is_بنياد_انگ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Brahmi_Joining_عدد
وصف is_بنياد_OTHER(U, UISC, UDI, UGC, AJT):
	إذا UISC == Consonant_Placeholder: أرجع سچ
	أرجع U في [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
وصف is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	أرجع UISC == Joiner أو UDI و UGC في [Mc, Me, Mn]
وصف is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	أرجع ((UISC == Consonant_Final و UGC != Lo) أو
		UISC == Consonant_Succeeding_Repha)
وصف is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Syllable_Modifier
وصف is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	أرجع (UISC == Consonant_Medial و UGC != Lo أو
		UISC == Consonant_Initial_Postfixed)
وصف is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Nukta, Gemination_Mark, Consonant_Killer]
وصف is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_Subjoined و UGC != Lo
وصف is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Consonant_With_Stacker
وصف is_HALANT(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Virama و ليس is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
وصف is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x0DCA
وصف is_HALANT_انگ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == عدد_Joiner
وصف is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph
وصف is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Joiner
وصف is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Mirror
وصف is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Hieroglyph_Modifier
وصف is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
وصف is_HIEROGLYPH_SEGMENT_پڇاڙي(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Hieroglyph_Mark_پڇاڙي, Hieroglyph_Segment_پڇاڙي]
وصف is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع (UISC == Invisible_Stacker
		و ليس is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
وصف is_ZWNJ(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Non_Joiner
وصف is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	أرجع ((UGC == Po أو UISC في [Consonant_Dead, Joiner, Modifying_Letter, Other])
		و ليس is_بنياد(U, UISC, UDI, UGC, AJT)
		و ليس is_بنياد_OTHER(U, UISC, UDI, UGC, AJT)
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
		و ليس is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		و ليس is_لفظ_Joiner(U, UISC, UDI, UGC, AJT)
	)
وصف is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	أرجع UISC == Reordering_Killer
وصف is_REPHA(U, UISC, UDI, UGC, AJT):
	أرجع UISC في [Consonant_Preceding_Repha, Consonant_Prefixed]
وصف is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	أرجع U == 0x1A60
وصف is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	أرجع UISC == نشان_Modifier
وصف is_VOWEL(U, UISC, UDI, UGC, AJT):
	أرجع (UISC == خالص_Killer أو
		UGC != Lo و UISC في [Vowel, Vowel_Dependent])
وصف is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	أرجع (UISC في [Tone_Mark, Cantillation_Mark, رجسٽر_Shifter, Visarga] أو
		UGC != Lo و UISC == Bindu)
وصف is_لفظ_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	أرجع (UDI و U ليس في [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		و UISC == Other
		و ليس is_CGJ(U, UISC, UDI, UGC, AJT)
	) أو UGC == Cn

use_mapping = {
	'B':	is_بنياد,
	'N':	is_بنياد_انگ,
	'GB':	is_بنياد_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_انگ,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_پڇاڙي,
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
		'Abv': [چوٽي],
		'Blw': [Bottom],
		'Pst': [ساڄو],
	},
	'M': {
		'Abv': [چوٽي],
		'Blw': [Bottom, Bottom_And_کاٻو, Bottom_And_ساڄو],
		'Pst': [ساڄو],
		'Pre': [کاٻو, چوٽي_And_Bottom_And_کاٻو],
	},
	'CM': {
		'Abv': [چوٽي],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [چوٽي, چوٽي_And_Bottom, چوٽي_And_Bottom_And_ساڄو, چوٽي_And_ساڄو],
		'Blw': [Bottom, Overstruck, Bottom_And_ساڄو],
		'Pst': [ساڄو],
		'Pre': [کاٻو, چوٽي_And_کاٻو, چوٽي_And_کاٻو_And_ساڄو, کاٻو_And_ساڄو],
	},
	'VM': {
		'Abv': [چوٽي],
		'Blw': [Bottom, Overstruck],
		'Pst': [ساڄو],
		'Pre': [کاٻو],
	},
	'SM': {
		'Abv': [چوٽي],
		'Blw': [Bottom],
	},
	'H': ڪجهه_نه,
	'HM': ڪجهه_نه,
	'HR': ڪجهه_نه,
	'HVM': ڪجهه_نه,
	'IS': ڪجهه_نه,
	'B': ڪجهه_نه,
	'FM': {
		'Abv': [چوٽي],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': ڪجهه_نه,
	'RK': ڪجهه_نه,
	'SUB': ڪجهه_نه,
}

وصف map_منزل_use(معلومات):
	ٻاهريون = {}
	شيون = use_mapping.شيون()
	لكل U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) في معلومات.شيون():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		إذا 0x0F18 <= U <= 0x0F19 أو 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		إذا U == 0x1CED: UISC = Tone_Mark

		قدرون = [k لكل k,v في شيون إذا v(U, UISC, UDI, UGC, AJT)]
		أكد ڊيگهه(قدرون) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, قدرون)
		USE = قدرون[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		إذا U في [0x11302, 0x11303, 0x114C1]: UIPC = چوٽي

		أكد (UIPC في [Not_Applicable, Visual_ترتيب_کاٻو] أو
			U == 0x0F7F أو
			USE في use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, ڪجهه_نه)
		إذا pos_mapping:
			قدرون = [k لكل k,v في pos_mapping.شيون() إذا v و UIPC في v]
			أكد ڊيگهه(قدرون) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, قدرون)
			USE = USE + قدرون[0]

		ٻاهريون[U] = (USE, UBlock)
	أرجع ٻاهريون

use_معلومات = map_منزل_use(combined)

ڇاپيو ("/* == Start of generated table == */")
ڇاپيو ("/*")
ڇاپيو (" * The following table is generated by running:")
ڇاپيو (" *")
ڇاپيو (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.رستو.basename (sys.argv[0])))
ڇاپيو (" *")
ڇاپيو (" * on files with these headers:")
ڇاپيو (" *")
لكل h في سرا:
	لكل l في h:
		ڇاپيو (" * %s" % (l.strip()))
ڇاپيو (" */")
ڇاپيو ()
إذا language.name == "c":
    ڇاپيو ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    ڇاپيو ("#define HB_OT_SHAPER_USE_TABLE_HH")
    ڇاپيو ()
    ڇاپيو ('#include "hb.hh"')
    ڇاپيو ()
    ڇاپيو ('#include "hb-ot-shaper-use-machine.hh"')
    ڇاپيو ()

    ڇاپيو ('#pragma GCC diagnostic push')
    ڇاپيو ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    لكل k,v في sorted(use_mapping.شيون()):
        إذا k في use_positions و use_positions[k]: استمر
        ڇاپيو ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    لكل k,v في sorted(use_positions.شيون()):
        إذا ليس v: استمر
        لكل suf في v.ڪنجيون():
            ٽيگ_2 = k + suf
            ڇاپيو ("#define %s	USE(%s)" % (ٽيگ_2, ٽيگ_2))
    ڇاپيو ('#pragma GCC diagnostic pop')
    ڇاپيو ("")

وإلاإذا language.name == "rust":
    ڇاپيو()
    ڇاپيو("#![allow(unused_parens)]")
    ڇاپيو("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    ڇاپيو()
    ڇاپيو("use super::ot_shaper_use::category::*;")
    ڇاپيو()
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name

uu = sorted (use_معلومات.ڪنجيون ())

معلومات = {u:v[0] لكل u,v في use_معلومات.شيون()}

إذا language.name == "c":
    خاص = سچ
وإلاإذا language.name == "rust":
    خاص = خطأ
    language.عوامي_function_linkage = "pub(crate)"
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
    modes[ڪجهه_نه] = "#endif"
وإلا:
    modes[DEFAULT] = ""

لكل قدم, متن في modes.شيون():
    ڇاپيو()
    إذا متن:
        ڇاپيو(متن)
        ڇاپيو()
    إذا قدم آهي ڪجهه_نه:
        استمر

    compression = compression_level[قدم]
    logging.info("  Compression=%d:" % compression)

    ڪوڊ = packTab.ڪوڊ_2('hb_use')
    sol = packTab.pack_جدول(معلومات, compression=compression, افتراضي='O')
    logging.info('      FullCost=%d' % (sol.ڀريل_لاڳت))
    sol.gen_ڪوڊ(ڪوڊ, f'get_category', language=language, خاص=خاص)
    ڪوڊ.print_ڪوڊ(language=language, خاص=خاص)
    ڇاپيو ()

إذا language.name == "c":
    ڇاپيو ()
    لكل k في sorted(use_mapping.ڪنجيون()):
        إذا k في use_positions و use_positions[k]: استمر
        ڇاپيو ("#undef %s" % k)
    لكل k,v في sorted(use_positions.شيون()):
        إذا ليس v: استمر
        لكل suf في v.ڪنجيون():
            ٽيگ_2 = k + suf
            ڇاپيو ("#undef %s" % ٽيگ_2)
    ڇاپيو ()
    ڇاپيو ()
    ڇاپيو ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
وإلاإذا language.name == "rust":
    لنگهه
وإلا:
    أكد خطأ, "Unknown language: %s" % language.name
ڇاپيو ("/* == End of generated table == */")
