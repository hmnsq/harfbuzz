#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

استورد os.رستہ, sys

استورد packTab

إذا لمبائی(sys.argv) > 1 و sys.argv[1] == "--rust":
	ہٹاؤ sys.argv[1]
	language = packTab.languages["rust"]
وإلا:
	language = packTab.languages["c"]

إذا لمبائی (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرخیاں = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
سرخیاں.append (["UnicodeData.txt does not have a header."])
طالما files[0].readline ().لبھو ('##################') < 0:
	لنگھو

بلاکاں = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_کوڈ = {
	"JOINING_TYPE_U": 0,
	"JOINING_TYPE_L": 1,
	"JOINING_TYPE_R": 2,
	"JOINING_TYPE_D": 3,
	"JOINING_TYPE_C": 3,
	"JOINING_GROUP_ALAPH": 4,
	"JOINING_GROUP_DALATH_RISH": 5,
	"JOINING_TYPE_T": 6,
	"JOINING_TYPE_X": 7,
}

تعریف read_بلاکاں(f):
	عام بلاکاں
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

		t = fields[1]

		لكل u في سلسلہ (شروع, سرا + 1):
			بلاکاں[u] = t

تعریف print_joining_جدول(f):

	اقدار = {}
	لكل سطر في f:

		إذا سطر[0] == '#':
			استمر

		fields = [x.strip () لكل x في سطر.ونڈو (';')]
		إذا لمبائی (fields) == 1:
			استمر

		u = صحيح (fields[0], 16)

		إذا fields[3] في ["ALAPH", "DALATH RISH"]:
			قدر_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		وإلا:
			قدر_2 = "JOINING_TYPE_" + fields[2]
		اقدار[u] = قدر_2

	کوڈ = packTab.کوڈ_2 ('_hb_arabic_joining')
	انکڑے = {u: JOINING_کوڈ[v] لكل u, v في اقدار.اشیاء ()}
	sol = packTab.pack_جدول (انکڑے, افتراضي=JOINING_کوڈ['JOINING_TYPE_X'], compression=9)
	إذا language.name == "c":
		sol.gen_کوڈ (کوڈ, 'joining_type', language=language, خاص=سچ)

		چھاپو ("#include \"hb.hh\"")
		چھاپو ()
		کوڈ.print_کوڈ (file=sys.stdout, language=language, خاص=سچ)
		چھاپو ()
		چھاپو ("static unsigned int")
		چھاپو ("joining_type (hb_codepoint_t u)")
		چھاپو ("{")
		چھاپو ("  return _hb_arabic_joining_joining_type (u);")
		چھاپو ("}")
		چھاپو ()
	وإلاإذا language.name == "rust":
		sol.gen_کوڈ (کوڈ, 'joining_type_u8', language=language, خاص=خطأ)

		چھاپو ("#![allow(unused_parens)]")
		چھاپو ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		چھاپو ()
		چھاپو ("use crate::unicode::Codepoint;")
		چھاپو ()
		چھاپو (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		چھاپو ()
		کوڈ.print_کوڈ (file=sys.stdout, language=language, خاص=خطأ)
		چھاپو ()
		چھاپو ("#[inline]")
		چھاپو ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		چھاپو ("{")
		چھاپو ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		چھاپو ("    0 => U,")
		چھاپو ("    1 => L,")
		چھاپو ("    2 => R,")
		چھاپو ("    3 => D,")
		چھاپو ("    4 => GroupAlaph,")
		چھاپو ("    5 => GroupDalathRish,")
		چھاپو ("    6 => T,")
		چھاپو ("    7 => X,")
		چھاپو ("    _ => unreachable! (),")
		چھاپو ("  }")
		چھاپو ("}")
		چھاپو ()
	وإلا:
		أكد خطأ, "Unknown language: %s" % language.name

LIGATURES = (
	0xF2EE, 0xFC08, 0xFC0E, 0xFC12, 0xFC32, 0xFC3F, 0xFC40, 0xFC41, 0xFC42,
	0xFC43, 0xFC44, 0xFC4E, 0xFC5E, 0xFC60, 0xFC61, 0xFC62, 0xFC6A, 0xFC6D,
	0xFC6F, 0xFC70, 0xFC73, 0xFC75, 0xFC86, 0xFC8F, 0xFC91, 0xFC94, 0xFC9C,
	0xFC9D, 0xFC9E, 0xFC9F, 0xFCA1, 0xFCA2, 0xFCA3, 0xFCA4, 0xFCA8, 0xFCAA,
	0xFCAC, 0xFCB0, 0xFCC9, 0xFCCA, 0xFCCB, 0xFCCC, 0xFCCD, 0xFCCE, 0xFCCF,
	0xFCD0, 0xFCD1, 0xFCD2, 0xFCD3, 0xFCD5, 0xFCDA, 0xFCDB, 0xFCDC, 0xFCDD,
	0xFD30, 0xFD88, 0xFEF5, 0xFEF6, 0xFEF7, 0xFEF8, 0xFEF9, 0xFEFA, 0xFEFB,
	0xFEFC, 0xF201, 0xF211, 0xF2EE,
)

تعریف read_shaping_جدول(f):

	shapes = {}
	ligatures = {}
	ناں = {}
	سطریں = f.readlines()
	سطریں += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	لكل سطر في سطریں:

		fields = [x.strip () لكل x في سطر.ونڈو (';')]
		إذا fields[5][0:1] != '<':
			استمر

		اشیاء = fields[5].ونڈو (' ')
		shape, اشیاء = اشیاء[0][1:-1], tuple (صحيح (x, 16) لكل x في اشیاء[1:])
		c = صحيح (fields[0], 16)

		إذا ليس shape في ['initial', 'medial', 'isolated', 'final']:
			استمر

		إذا لمبائی (اشیاء) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			إذا اشیاء[0] == 0x0020:
				اشیاء = اشیاء[:0:-1]
				shape = کجھ_نئیں
			# We only care about a subset of ligatures
			إذا c ليس في LIGATURES:
				استمر

			# Save ligature
			ناں[c] = fields[1]
			إذا اشیاء ليس في ligatures:
				ligatures[اشیاء] = {}
			ligatures[اشیاء][shape] = c
		وإلا:
			# Save shape
			إذا اشیاء[0] ليس في ناں:
				ناں[اشیاء[0]] = fields[1]
			وإلا:
				ناں[اشیاء[0]] = os.رستہ.commonprefix ([ناں[اشیاء[0]], fields[1]]).strip ()
			إذا اشیاء[0] ليس في shapes:
				shapes[اشیاء[0]] = {}
			shapes[اشیاء[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	لكل کلید_2 في ligatures.کلیدیں ():
		لكل shape في ligatures[کلید_2]:
			c = ligatures[کلید_2][shape]
			إذا لمبائی(کلید_2) == 3:
				إذا shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			وإلاإذا لمبائی(کلید_2) == 2:
				إذا shape اے کجھ_نئیں:
					liga = کلید_2
					إذا liga[0] ليس في ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					استمر
				وإلاإذا shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			وإلا:
				ارفع Exception ("Unexpected number of ligature components", کلید_2)

	أرجع shapes, ناں, ligas_2, ligas_mark_2, ligas_3


تعریف print_shaping_جدول(f):

	shapes, ناں, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)

	چھاپو ()
	چھاپو ("static const uint16_t shaping_table[][4] =")
	چھاپو ("{")

	کلیدیں = shapes.کلیدیں ()
	min_u, max_u = min (کلیدیں), max (کلیدیں)
	لكل u في سلسلہ (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في  ['initial', 'medial', 'final', 'isolated']]
		قدر_2 = ', '.join ("0x%04Xu" % c لكل c في s)
		چھاپو ("  {%s}, /* U+%04X %s */" % (قدر_2, u, ناں[u] إذا u في ناں وإلا ""))

	چھاپو ("};")
	چھاپو ()
	چھاپو ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	چھاپو ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	چھاپو ()

	max_i = max (لمبائی (ligas_2[l]) لكل l في ligas_2)
	چھاپو ()
	چھاپو ("static const struct ligature_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_pairs_t {")
	چھاپو ("   uint16_t components[1];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_table[] =")
	چھاپو ("{")
	لكل پہلا في sorted (ligas_2.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		لكل liga في ligas_2[پہلا]:
			چھاپو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], ناں[liga[1]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()

	max_i = max (لمبائی (ligas_mark_2[l]) لكل l في ligas_mark_2)
	چھاپو ()
	چھاپو ("static const struct ligature_mark_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_pairs_t {")
	چھاپو ("   uint16_t components[1];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_mark_table[] =")
	چھاپو ("{")
	لكل پہلا في sorted (ligas_mark_2.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		لكل liga في ligas_mark_2[پہلا]:
			چھاپو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], ناں[liga[1]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()

	max_i = max (لمبائی (ligas_3[l]) لكل l في ligas_3)
	چھاپو ()
	چھاپو ("static const struct ligature_3_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_triplets_t {")
	چھاپو ("   uint16_t components[2];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_3_table[] =")
	چھاپو ("{")
	لكل پہلا في sorted (ligas_3.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		لكل liga في ligas_3[پہلا]:
			چھاپو ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], ناں[liga[2]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()


تعریف print_shaping_جدول_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)
	min_u, max_u = min (shapes.کلیدیں ()), max (shapes.کلیدیں ())

	چھاپو ()
	چھاپو ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	لكل u في سلسلہ (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في ['initial', 'medial', 'final', 'isolated']]
		إذا any (s):
			قدر_2 = ", ".join ("0x%04X" % c لكل c في s)
			چھاپو ("    (0x%04X, [%s])," % (u, قدر_2))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	لكل پہلا في sorted (ligas_3.کلیدیں ()):
		لكل liga في ligas_3[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1], liga[2]))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	لكل پہلا في sorted (ligas_2.کلیدیں ()):
		لكل liga في ligas_2[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1]))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	لكل پہلا في sorted (ligas_mark_2.کلیدیں ()):
		لكل liga في ligas_mark_2[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1]))
	چھاپو ("];")



چھاپو ("/* == Start of generated table == */")
چھاپو ("/*")
چھاپو (" * The following table is generated by running:")
چھاپو (" *")
چھاپو (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " إذا language.name == "rust" وإلا ""))
چھاپو (" *")
چھاپو (" * on files with these headers:")
چھاپو (" *")
لكل h في سرخیاں:
	لكل l في h:
		چھاپو (" * %s" % (l.strip()))
چھاپو (" */")
چھاپو ()
read_بلاکاں (files[2])
إذا language.name == "c":
	چھاپو ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	چھاپو ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	چھاپو ()

	print_joining_جدول (files[0])
	print_shaping_جدول (files[1])

	چھاپو ()
	چھاپو ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وإلاإذا language.name == "rust":
	print_joining_جدول (files[0])
	print_shaping_جدول_rust (files[1])
وإلا:
	أكد خطأ, "Unknown language: %s" % language.name
چھاپو ()
چھاپو ("/* == End of generated table == */")
