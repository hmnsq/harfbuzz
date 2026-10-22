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

దిగుమతి os
దిగుమతి packTab

దిగుమతి sys
దిగుమతి logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

ఒకవేళ పొడవు(sys.argv) > 1 మరియు sys.argv[1] == "--rust":
    తీసివేయి sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
లేకపోతే:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

దిగుమతి sys

ఒకవేళ పొడవు (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') కొరకు x లో sys.argv[1:]]

శీర్షికలు = [[f.readline () కొరకు i లో శ్రేణి (2)] కొరకు j,f లో enumerate(files) ఒకవేళ j != 4]
కొరకు j లో శ్రేణి(7, 9):
	కొరకు పంక్తి లో files[j]:
		పంక్తి = పంక్తి.rstrip()
		ఒకవేళ కాదు పంక్తి:
			ఆపు
		శీర్షికలు[j - 1].append(పంక్తి)
శీర్షికలు.append (["UnicodeData.txt does not have a header."])

unicode_దత్తాంశం = [{} కొరకు _ లో files]
విలువలు = [{} కొరకు _ లో files]
కొరకు i, f లో enumerate (files):
	కొరకు పంక్తి లో f:

		j = పంక్తి.కనుగొను ('#')
		ఒకవేళ j >= 0:
			పంక్తి = పంక్తి[:j]

		fields = [x.strip () కొరకు x లో పంక్తి.విభజించు (';')]
		ఒకవేళ పొడవు (fields) == 1:
			కొనసాగు

		uu = fields[0].విభజించు ('..')
		ప్రారంభం = పూర్ణాంకం (uu[0], 16)
		ఒకవేళ పొడవు (uu) == 1:
			చివర = ప్రారంభం
		లేకపోతే:
			చివర = పూర్ణాంకం (uu[1], 16)

		t = fields[1 ఒకవేళ i కాదు లో [2, 4] లేకపోతే 2]

		ఒకవేళ i == 2:
			t = 'jt_' + t
		లేదావేళ i == 3 మరియు t != 'Default_Ignorable_Code_Point':
			కొనసాగు
		లేదావేళ i == 7 మరియు t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		లేదావేళ i == 8 మరియు t == 'NA':
			t = 'Not_Applicable'

		i0 = i ఒకవేళ i < 7 లేకపోతే i - 7
		కొరకు u లో శ్రేణి (ప్రారంభం, చివర + 1):
			unicode_దత్తాంశం[i0][u] = t
		విలువలు[i0][t] = విలువలు[i0].get (t, 0) + చివర - ప్రారంభం + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
కొరకు i,v లో enumerate (defaults):
	విలువలు[i][v] = విలువలు[i].get (v, 0) + 1
combined = {}
కొరకు i,d లో enumerate (unicode_దత్తాంశం):
	కొరకు u,v లో d.వస్తువులు ():
		ఒకవేళ కాదు u లో combined:
			ఒకవేళ i >= 4:
				కొనసాగు
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v కొరకు k, v లో combined.వస్తువులు() ఒకవేళ v[6] కాదు లో DISABLED_SCRIPTS}


property_పేర్లు = [
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

తరగతి Property_విలువ(object):
	నిర్వచనం __init__(self, name_):
		self.name = name_
	నిర్వచనం __str__(self):
		తిరిగి self.name
	నిర్వచనం __eq__(self, other):
		తిరిగి self.name == (other ఒకవేళ isinstance(other, str) లేకపోతే other.name)
	నిర్వచనం __ne__(self, other):
		తిరిగి కాదు (self == other)
	నిర్వచనం __hash__(self):
		తిరిగి hash(str(self))

property_విలువలు = {}

కొరకు name లో property_పేర్లు:
	విలువ_2 = Property_విలువ(name)
	ప్రతిజ్ఞా విలువ_2 కాదు లో property_విలువలు
	ప్రతిజ్ఞా విలువ_2 కాదు లో globals()
	property_విలువలు[name] = విలువ_2
globals().update(property_విలువలు)


నిర్వచనం is_పునాది(U, UISC, UDI, UGC, AJT):
	తిరిగి (UISC లో [సంఖ్య, Consonant, Consonant_తల_Letter,
			Tone_Letter,
			Vowel_Independent,
			] లేదా
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT లో [jt_C, jt_D, jt_L, jt_R] మరియు UISC != Joiner లేదా
		(UGC == Lo మరియు UISC లో [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
నిర్వచనం is_పునాది_అంకె(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Brahmi_Joining_సంఖ్య
నిర్వచనం is_పునాది_OTHER(U, UISC, UDI, UGC, AJT):
	ఒకవేళ UISC == Consonant_Placeholder: తిరిగి నిజం
	తిరిగి U లో [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
నిర్వచనం is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	తిరిగి UISC == Joiner లేదా UDI మరియు UGC లో [Mc, Me, Mn]
నిర్వచనం is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	తిరిగి ((UISC == Consonant_Final మరియు UGC != Lo) లేదా
		UISC == Consonant_Succeeding_Repha)
నిర్వచనం is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Syllable_Modifier
నిర్వచనం is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	తిరిగి (UISC == Consonant_Medial మరియు UGC != Lo లేదా
		UISC == Consonant_Initial_Postfixed)
నిర్వచనం is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC లో [Nukta, Gemination_Mark, Consonant_Killer]
నిర్వచనం is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Consonant_Subjoined మరియు UGC != Lo
నిర్వచనం is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Consonant_With_Stacker
నిర్వచనం is_HALANT(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Virama మరియు కాదు is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
నిర్వచనం is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	తిరిగి U == 0x0DCA
నిర్వచనం is_HALANT_అంకె(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == సంఖ్య_Joiner
నిర్వచనం is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Hieroglyph
నిర్వచనం is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Hieroglyph_Joiner
నిర్వచనం is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Hieroglyph_Mirror
నిర్వచనం is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Hieroglyph_Modifier
నిర్వచనం is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC లో [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
నిర్వచనం is_HIEROGLYPH_SEGMENT_చివర(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC లో [Hieroglyph_Mark_చివర, Hieroglyph_Segment_చివర]
నిర్వచనం is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	తిరిగి (UISC == Invisible_Stacker
		మరియు కాదు is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
నిర్వచనం is_ZWNJ(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Non_Joiner
నిర్వచనం is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	తిరిగి ((UGC == Po లేదా UISC లో [Consonant_Dead, Joiner, Modifying_Letter, Other])
		మరియు కాదు is_పునాది(U, UISC, UDI, UGC, AJT)
		మరియు కాదు is_పునాది_OTHER(U, UISC, UDI, UGC, AJT)
		మరియు కాదు is_CGJ(U, UISC, UDI, UGC, AJT)
		మరియు కాదు is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		మరియు కాదు is_మాట_Joiner(U, UISC, UDI, UGC, AJT)
	)
నిర్వచనం is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == Reordering_Killer
నిర్వచనం is_REPHA(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC లో [Consonant_Preceding_Repha, Consonant_Prefixed]
నిర్వచనం is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	తిరిగి U == 0x1A60
నిర్వచనం is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	తిరిగి UISC == చిహ్నం_Modifier
నిర్వచనం is_VOWEL(U, UISC, UDI, UGC, AJT):
	తిరిగి (UISC == శుద్ధ_Killer లేదా
		UGC != Lo మరియు UISC లో [Vowel, Vowel_Dependent])
నిర్వచనం is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	తిరిగి (UISC లో [Tone_Mark, Cantillation_Mark, రిజిస్టరు_Shifter, Visarga] లేదా
		UGC != Lo మరియు UISC == Bindu)
నిర్వచనం is_మాట_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	తిరిగి (UDI మరియు U కాదు లో [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		మరియు UISC == Other
		మరియు కాదు is_CGJ(U, UISC, UDI, UGC, AJT)
	) లేదా UGC == Cn

use_mapping = {
	'B':	is_పునాది,
	'N':	is_పునాది_అంకె,
	'GB':	is_పునాది_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_అంకె,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_చివర,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_మాట_Joiner,
}

use_positions = {
	'F': {
		'Abv': [శిఖరం],
		'Blw': [Bottom],
		'Pst': [కుడి_2],
	},
	'M': {
		'Abv': [శిఖరం],
		'Blw': [Bottom, Bottom_And_ఎడమ, Bottom_And_కుడి],
		'Pst': [కుడి_2],
		'Pre': [ఎడమ_2, శిఖరం_And_Bottom_And_ఎడమ],
	},
	'CM': {
		'Abv': [శిఖరం],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [శిఖరం, శిఖరం_And_Bottom, శిఖరం_And_Bottom_And_కుడి, శిఖరం_And_కుడి],
		'Blw': [Bottom, Overstruck, Bottom_And_కుడి],
		'Pst': [కుడి_2],
		'Pre': [ఎడమ_2, శిఖరం_And_ఎడమ, శిఖరం_And_ఎడమ_And_కుడి, ఎడమ_And_కుడి],
	},
	'VM': {
		'Abv': [శిఖరం],
		'Blw': [Bottom, Overstruck],
		'Pst': [కుడి_2],
		'Pre': [ఎడమ_2],
	},
	'SM': {
		'Abv': [శిఖరం],
		'Blw': [Bottom],
	},
	'H': ఏమీలేదు,
	'HM': ఏమీలేదు,
	'HR': ఏమీలేదు,
	'HVM': ఏమీలేదు,
	'IS': ఏమీలేదు,
	'B': ఏమీలేదు,
	'FM': {
		'Abv': [శిఖరం],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': ఏమీలేదు,
	'RK': ఏమీలేదు,
	'SUB': ఏమీలేదు,
}

నిర్వచనం map_గమ్యం_use(దత్తాంశం):
	బహిర్గతం = {}
	వస్తువులు = use_mapping.వస్తువులు()
	కొరకు U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) లో దత్తాంశం.వస్తువులు():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ఒకవేళ 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		ఒకవేళ 0x0F18 <= U <= 0x0F19 లేదా 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		ఒకవేళ U == 0x1CED: UISC = Tone_Mark

		విలువలు = [k కొరకు k,v లో వస్తువులు ఒకవేళ v(U, UISC, UDI, UGC, AJT)]
		ప్రతిజ్ఞా పొడవు(విలువలు) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, విలువలు)
		USE = విలువలు[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		ఒకవేళ U లో [0x11302, 0x11303, 0x114C1]: UIPC = శిఖరం

		ప్రతిజ్ఞా (UIPC లో [Not_Applicable, Visual_క్రమం_ఎడమ] లేదా
			U == 0x0F7F లేదా
			USE లో use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, ఏమీలేదు)
		ఒకవేళ pos_mapping:
			విలువలు = [k కొరకు k,v లో pos_mapping.వస్తువులు() ఒకవేళ v మరియు UIPC లో v]
			ప్రతిజ్ఞా పొడవు(విలువలు) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, విలువలు)
			USE = USE + విలువలు[0]

		బహిర్గతం[U] = (USE, UBlock)
	తిరిగి బహిర్గతం

use_దత్తాంశం = map_గమ్యం_use(combined)

ముద్రించు ("/* == Start of generated table == */")
ముద్రించు ("/*")
ముద్రించు (" * The following table is generated by running:")
ముద్రించు (" *")
ముద్రించు (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.మార్గం.basename (sys.argv[0])))
ముద్రించు (" *")
ముద్రించు (" * on files with these headers:")
ముద్రించు (" *")
కొరకు h లో శీర్షికలు:
	కొరకు l లో h:
		ముద్రించు (" * %s" % (l.strip()))
ముద్రించు (" */")
ముద్రించు ()
ఒకవేళ language.name == "c":
    ముద్రించు ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    ముద్రించు ("#define HB_OT_SHAPER_USE_TABLE_HH")
    ముద్రించు ()
    ముద్రించు ('#include "hb.hh"')
    ముద్రించు ()
    ముద్రించు ('#include "hb-ot-shaper-use-machine.hh"')
    ముద్రించు ()

    ముద్రించు ('#pragma GCC diagnostic push')
    ముద్రించు ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    కొరకు k,v లో sorted(use_mapping.వస్తువులు()):
        ఒకవేళ k లో use_positions మరియు use_positions[k]: కొనసాగు
        ముద్రించు ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    కొరకు k,v లో sorted(use_positions.వస్తువులు()):
        ఒకవేళ కాదు v: కొనసాగు
        కొరకు suf లో v.కీలకాలు():
            ట్యాగ్_2 = k + suf
            ముద్రించు ("#define %s	USE(%s)" % (ట్యాగ్_2, ట్యాగ్_2))
    ముద్రించు ('#pragma GCC diagnostic pop')
    ముద్రించు ("")

లేదావేళ language.name == "rust":
    ముద్రించు()
    ముద్రించు("#![allow(unused_parens)]")
    ముద్రించు("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    ముద్రించు()
    ముద్రించు("use super::ot_shaper_use::category::*;")
    ముద్రించు()
లేకపోతే:
    ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name

uu = sorted (use_దత్తాంశం.కీలకాలు ())

దత్తాంశం = {u:v[0] కొరకు u,v లో use_దత్తాంశం.వస్తువులు()}

ఒకవేళ language.name == "c":
    అంతరంగ = నిజం
లేదావేళ language.name == "rust":
    అంతరంగ = అబద్ధం
    language.ప్రజా_function_linkage = "pub(crate)"
లేకపోతే:
    ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
ఒకవేళ language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[ఏమీలేదు] = "#endif"
లేకపోతే:
    modes[DEFAULT] = ""

కొరకు అడుగు, పాఠ్యం లో modes.వస్తువులు():
    ముద్రించు()
    ఒకవేళ పాఠ్యం:
        ముద్రించు(పాఠ్యం)
        ముద్రించు()
    ఒకవేళ అడుగు అయిన ఏమీలేదు:
        కొనసాగు

    compression = compression_level[అడుగు]
    logging.info("  Compression=%d:" % compression)

    సంకేతం_2 = packTab.సంకేతం_3('hb_use')
    sol = packTab.pack_పట్టిక(దత్తాంశం, compression=compression, అప్రమేయ='O')
    logging.info('      FullCost=%d' % (sol.నిండిన_ఖర్చు))
    sol.gen_సంకేతం(సంకేతం_2, f'get_category', language=language, అంతరంగ=అంతరంగ)
    సంకేతం_2.print_సంకేతం(language=language, అంతరంగ=అంతరంగ)
    ముద్రించు ()

ఒకవేళ language.name == "c":
    ముద్రించు ()
    కొరకు k లో sorted(use_mapping.కీలకాలు()):
        ఒకవేళ k లో use_positions మరియు use_positions[k]: కొనసాగు
        ముద్రించు ("#undef %s" % k)
    కొరకు k,v లో sorted(use_positions.వస్తువులు()):
        ఒకవేళ కాదు v: కొనసాగు
        కొరకు suf లో v.కీలకాలు():
            ట్యాగ్_2 = k + suf
            ముద్రించు ("#undef %s" % ట్యాగ్_2)
    ముద్రించు ()
    ముద్రించు ()
    ముద్రించు ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
లేదావేళ language.name == "rust":
    దాటు
లేకపోతే:
    ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name
ముద్రించు ("/* == End of generated table == */")
