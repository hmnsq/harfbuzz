#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

درآمد os.راستہ, sys

درآمد packTab

اگر لمبائی(sys.argv) > 1 اور sys.argv[1] == "--rust":
	مٹاؤ sys.argv[1]
	language = packTab.languages["rust"]
ورنہ:
	language = packTab.languages["c"]

اگر لمبائی (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') برائے x میں sys.argv[1:]]

سرخیاں = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
سرخیاں.append (["UnicodeData.txt does not have a header."])
جب files[0].readline ().ڈھونڈو ('##################') < 0:
	گزرو

بلاکیں = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_رمز = {
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

تعریف read_بلاکیں(f):
	عام بلاکیں
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

		t = fields[1]

		برائے u میں سلسلہ (آغاز, انتہا + 1):
			بلاکیں[u] = t

تعریف print_joining_جدول(f):

	اقدار = {}
	برائے سطر میں f:

		اگر سطر[0] == '#':
			جاری

		fields = [x.strip () برائے x میں سطر.بانٹو (';')]
		اگر لمبائی (fields) == 1:
			جاری

		u = صحیح (fields[0], 16)

		اگر fields[3] میں ["ALAPH", "DALATH RISH"]:
			قدر_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		ورنہ:
			قدر_2 = "JOINING_TYPE_" + fields[2]
		اقدار[u] = قدر_2

	رمز = packTab.رمز_2 ('_hb_arabic_joining')
	ڈیٹا = {u: JOINING_رمز[v] برائے u, v میں اقدار.اشیاء ()}
	sol = packTab.pack_جدول (ڈیٹا, طےشدہ=JOINING_رمز['JOINING_TYPE_X'], compression=9)
	اگر language.name == "c":
		sol.gen_رمز (رمز, 'joining_type', language=language, نجی=سچ)

		چھاپو ("#include \"hb.hh\"")
		چھاپو ()
		رمز.print_رمز (file=sys.stdout, language=language, نجی=سچ)
		چھاپو ()
		چھاپو ("static unsigned int")
		چھاپو ("joining_type (hb_codepoint_t u)")
		چھاپو ("{")
		چھاپو ("  return _hb_arabic_joining_joining_type (u);")
		چھاپو ("}")
		چھاپو ()
	وگرنہ_اگر language.name == "rust":
		sol.gen_رمز (رمز, 'joining_type_u8', language=language, نجی=جھوٹ)

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
		رمز.print_رمز (file=sys.stdout, language=language, نجی=جھوٹ)
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
	ورنہ:
		أكد جھوٹ, "Unknown language: %s" % language.name

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
	نام = {}
	سطریں = f.readlines()
	سطریں += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	برائے سطر میں سطریں:

		fields = [x.strip () برائے x میں سطر.بانٹو (';')]
		اگر fields[5][0:1] != '<':
			جاری

		اشیاء = fields[5].بانٹو (' ')
		shape, اشیاء = اشیاء[0][1:-1], tuple (صحیح (x, 16) برائے x میں اشیاء[1:])
		c = صحیح (fields[0], 16)

		اگر نہیں shape میں ['initial', 'medial', 'isolated', 'final']:
			جاری

		اگر لمبائی (اشیاء) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			اگر اشیاء[0] == 0x0020:
				اشیاء = اشیاء[:0:-1]
				shape = کچھ_نہیں
			# We only care about a subset of ligatures
			اگر c نہیں میں LIGATURES:
				جاری

			# Save ligature
			نام[c] = fields[1]
			اگر اشیاء نہیں میں ligatures:
				ligatures[اشیاء] = {}
			ligatures[اشیاء][shape] = c
		ورنہ:
			# Save shape
			اگر اشیاء[0] نہیں میں نام:
				نام[اشیاء[0]] = fields[1]
			ورنہ:
				نام[اشیاء[0]] = os.راستہ.commonprefix ([نام[اشیاء[0]], fields[1]]).strip ()
			اگر اشیاء[0] نہیں میں shapes:
				shapes[اشیاء[0]] = {}
			shapes[اشیاء[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	برائے کلید_2 میں ligatures.کلیدیں ():
		برائے shape میں ligatures[کلید_2]:
			c = ligatures[کلید_2][shape]
			اگر لمبائی(کلید_2) == 3:
				اگر shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وگرنہ_اگر shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وگرنہ_اگر shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['medial'])
				ورنہ:
					اٹھاؤ Exception ("Unexpected shape", shape)
				اگر liga[0] نہیں میں ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			وگرنہ_اگر لمبائی(کلید_2) == 2:
				اگر shape ہے کچھ_نہیں:
					liga = کلید_2
					اگر liga[0] نہیں میں ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					جاری
				وگرنہ_اگر shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['final'])
				وگرنہ_اگر shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['final'])
				وگرنہ_اگر shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'])
				ورنہ:
					اٹھاؤ Exception ("Unexpected shape", shape)
				اگر liga[0] نہیں میں ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			ورنہ:
				اٹھاؤ Exception ("Unexpected number of ligature components", کلید_2)

	واپس shapes, نام, ligas_2, ligas_mark_2, ligas_3


تعریف print_shaping_جدول(f):

	shapes, نام, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)

	چھاپو ()
	چھاپو ("static const uint16_t shaping_table[][4] =")
	چھاپو ("{")

	کلیدیں = shapes.کلیدیں ()
	min_u, max_u = min (کلیدیں), max (کلیدیں)
	برائے u میں سلسلہ (min_u, max_u + 1):
		s = [shapes[u][shape] اگر u میں shapes اور shape میں shapes[u] ورنہ 0
		     برائے shape میں  ['initial', 'medial', 'final', 'isolated']]
		قدر_2 = ', '.join ("0x%04Xu" % c برائے c میں s)
		چھاپو ("  {%s}, /* U+%04X %s */" % (قدر_2, u, نام[u] اگر u میں نام ورنہ ""))

	چھاپو ("};")
	چھاپو ()
	چھاپو ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	چھاپو ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	چھاپو ()

	max_i = max (لمبائی (ligas_2[l]) برائے l میں ligas_2)
	چھاپو ()
	چھاپو ("static const struct ligature_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_pairs_t {")
	چھاپو ("   uint16_t components[1];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_table[] =")
	چھاپو ("{")
	برائے پہلا میں sorted (ligas_2.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		برائے liga میں ligas_2[پہلا]:
			چھاپو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نام[liga[1]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()

	max_i = max (لمبائی (ligas_mark_2[l]) برائے l میں ligas_mark_2)
	چھاپو ()
	چھاپو ("static const struct ligature_mark_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_pairs_t {")
	چھاپو ("   uint16_t components[1];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_mark_table[] =")
	چھاپو ("{")
	برائے پہلا میں sorted (ligas_mark_2.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		برائے liga میں ligas_mark_2[پہلا]:
			چھاپو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نام[liga[1]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()

	max_i = max (لمبائی (ligas_3[l]) برائے l میں ligas_3)
	چھاپو ()
	چھاپو ("static const struct ligature_3_set_t {")
	چھاپو (" uint16_t first;")
	چھاپو (" struct ligature_triplets_t {")
	چھاپو ("   uint16_t components[2];")
	چھاپو ("   uint16_t ligature;")
	چھاپو (" } ligatures[%d];" % max_i)
	چھاپو ("} ligature_3_table[] =")
	چھاپو ("{")
	برائے پہلا میں sorted (ligas_3.کلیدیں ()):

		چھاپو ("  { 0x%04Xu, {" % (پہلا))
		برائے liga میں ligas_3[پہلا]:
			چھاپو ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], نام[liga[2]]))
		چھاپو ("  }},")

	چھاپو ("};")
	چھاپو ()


تعریف print_shaping_جدول_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)
	min_u, max_u = min (shapes.کلیدیں ()), max (shapes.کلیدیں ())

	چھاپو ()
	چھاپو ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	برائے u میں سلسلہ (min_u, max_u + 1):
		s = [shapes[u][shape] اگر u میں shapes اور shape میں shapes[u] ورنہ 0
		     برائے shape میں ['initial', 'medial', 'final', 'isolated']]
		اگر any (s):
			قدر_2 = ", ".join ("0x%04X" % c برائے c میں s)
			چھاپو ("    (0x%04X, [%s])," % (u, قدر_2))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	برائے پہلا میں sorted (ligas_3.کلیدیں ()):
		برائے liga میں ligas_3[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1], liga[2]))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	برائے پہلا میں sorted (ligas_2.کلیدیں ()):
		برائے liga میں ligas_2[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1]))
	چھاپو ("];")

	چھاپو ()
	چھاپو ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	برائے پہلا میں sorted (ligas_mark_2.کلیدیں ()):
		برائے liga میں ligas_mark_2[پہلا]:
			چھاپو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پہلا, liga[0], liga[1]))
	چھاپو ("];")



چھاپو ("/* == Start of generated table == */")
چھاپو ("/*")
چھاپو (" * The following table is generated by running:")
چھاپو (" *")
چھاپو (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " اگر language.name == "rust" ورنہ ""))
چھاپو (" *")
چھاپو (" * on files with these headers:")
چھاپو (" *")
برائے h میں سرخیاں:
	برائے l میں h:
		چھاپو (" * %s" % (l.strip()))
چھاپو (" */")
چھاپو ()
read_بلاکیں (files[2])
اگر language.name == "c":
	چھاپو ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	چھاپو ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	چھاپو ()

	print_joining_جدول (files[0])
	print_shaping_جدول (files[1])

	چھاپو ()
	چھاپو ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وگرنہ_اگر language.name == "rust":
	print_joining_جدول (files[0])
	print_shaping_جدول_rust (files[1])
ورنہ:
	أكد جھوٹ, "Unknown language: %s" % language.name
چھاپو ()
چھاپو ("/* == End of generated table == */")
