#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

استورد os.مسیر, sys

استورد packTab

إذا طول(sys.argv) > 1 و sys.argv[1] == "--rust":
	پاک sys.argv[1]
	language = packTab.languages["rust"]
وإلا:
	language = packTab.languages["c"]

إذا طول (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرآیندها = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
سرآیندها.append (["UnicodeData.txt does not have a header."])
طالما files[0].readline ().پیدا_کن ('##################') < 0:
	بگذر

بلوکها = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_کد = {
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

تعریف read_بلوکها(f):
	عام بلوکها
	لكل سطر في f:

		j = سطر.پیدا_کن ('#')
		إذا j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () لكل x في سطر.تقسیم (';')]
		إذا طول (fields) == 1:
			استمر

		uu = fields[0].تقسیم ('..')
		شروع = صحيح (uu[0], 16)
		إذا طول (uu) == 1:
			انتها = شروع
		وإلا:
			انتها = صحيح (uu[1], 16)

		t = fields[1]

		لكل u في دامنه (شروع, انتها + 1):
			بلوکها[u] = t

تعریف print_joining_جدول(f):

	مقادیر = {}
	لكل سطر في f:

		إذا سطر[0] == '#':
			استمر

		fields = [x.strip () لكل x في سطر.تقسیم (';')]
		إذا طول (fields) == 1:
			استمر

		u = صحيح (fields[0], 16)

		إذا fields[3] في ["ALAPH", "DALATH RISH"]:
			مقدار_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		وإلا:
			مقدار_2 = "JOINING_TYPE_" + fields[2]
		مقادیر[u] = مقدار_2

	کد = packTab.کد_2 ('_hb_arabic_joining')
	داده = {u: JOINING_کد[v] لكل u, v في مقادیر.اقلام ()}
	sol = packTab.pack_جدول (داده, افتراضي=JOINING_کد['JOINING_TYPE_X'], compression=9)
	إذا language.name == "c":
		sol.gen_کد (کد, 'joining_type', language=language, خاص=درست)

		چاپ ("#include \"hb.hh\"")
		چاپ ()
		کد.print_کد (file=sys.stdout, language=language, خاص=درست)
		چاپ ()
		چاپ ("static unsigned int")
		چاپ ("joining_type (hb_codepoint_t u)")
		چاپ ("{")
		چاپ ("  return _hb_arabic_joining_joining_type (u);")
		چاپ ("}")
		چاپ ()
	وإلاإذا language.name == "rust":
		sol.gen_کد (کد, 'joining_type_u8', language=language, خاص=خطأ)

		چاپ ("#![allow(unused_parens)]")
		چاپ ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		چاپ ()
		چاپ ("use crate::unicode::Codepoint;")
		چاپ ()
		چاپ (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		چاپ ()
		کد.print_کد (file=sys.stdout, language=language, خاص=خطأ)
		چاپ ()
		چاپ ("#[inline]")
		چاپ ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		چاپ ("{")
		چاپ ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		چاپ ("    0 => U,")
		چاپ ("    1 => L,")
		چاپ ("    2 => R,")
		چاپ ("    3 => D,")
		چاپ ("    4 => GroupAlaph,")
		چاپ ("    5 => GroupDalathRish,")
		چاپ ("    6 => T,")
		چاپ ("    7 => X,")
		چاپ ("    _ => unreachable! (),")
		چاپ ("  }")
		چاپ ("}")
		چاپ ()
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
	نامها = {}
	سطرها = f.readlines()
	سطرها += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	لكل سطر في سطرها:

		fields = [x.strip () لكل x في سطر.تقسیم (';')]
		إذا fields[5][0:1] != '<':
			استمر

		اقلام = fields[5].تقسیم (' ')
		shape, اقلام = اقلام[0][1:-1], tuple (صحيح (x, 16) لكل x في اقلام[1:])
		c = صحيح (fields[0], 16)

		إذا ليس shape في ['initial', 'medial', 'isolated', 'final']:
			استمر

		إذا طول (اقلام) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			إذا اقلام[0] == 0x0020:
				اقلام = اقلام[:0:-1]
				shape = هیچ
			# We only care about a subset of ligatures
			إذا c ليس في LIGATURES:
				استمر

			# Save ligature
			نامها[c] = fields[1]
			إذا اقلام ليس في ligatures:
				ligatures[اقلام] = {}
			ligatures[اقلام][shape] = c
		وإلا:
			# Save shape
			إذا اقلام[0] ليس في نامها:
				نامها[اقلام[0]] = fields[1]
			وإلا:
				نامها[اقلام[0]] = os.مسیر.commonprefix ([نامها[اقلام[0]], fields[1]]).strip ()
			إذا اقلام[0] ليس في shapes:
				shapes[اقلام[0]] = {}
			shapes[اقلام[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	لكل کلید_2 في ligatures.کلیدها ():
		لكل shape في ligatures[کلید_2]:
			c = ligatures[کلید_2][shape]
			إذا طول(کلید_2) == 3:
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
			وإلاإذا طول(کلید_2) == 2:
				إذا shape است هیچ:
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

	أرجع shapes, نامها, ligas_2, ligas_mark_2, ligas_3


تعریف print_shaping_جدول(f):

	shapes, نامها, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)

	چاپ ()
	چاپ ("static const uint16_t shaping_table[][4] =")
	چاپ ("{")

	کلیدها = shapes.کلیدها ()
	min_u, max_u = min (کلیدها), max (کلیدها)
	لكل u في دامنه (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في  ['initial', 'medial', 'final', 'isolated']]
		مقدار_2 = ', '.join ("0x%04Xu" % c لكل c في s)
		چاپ ("  {%s}, /* U+%04X %s */" % (مقدار_2, u, نامها[u] إذا u في نامها وإلا ""))

	چاپ ("};")
	چاپ ()
	چاپ ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	چاپ ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	چاپ ()

	max_i = max (طول (ligas_2[l]) لكل l في ligas_2)
	چاپ ()
	چاپ ("static const struct ligature_set_t {")
	چاپ (" uint16_t first;")
	چاپ (" struct ligature_pairs_t {")
	چاپ ("   uint16_t components[1];")
	چاپ ("   uint16_t ligature;")
	چاپ (" } ligatures[%d];" % max_i)
	چاپ ("} ligature_table[] =")
	چاپ ("{")
	لكل اولی في sorted (ligas_2.کلیدها ()):

		چاپ ("  { 0x%04Xu, {" % (اولی))
		لكل liga في ligas_2[اولی]:
			چاپ ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نامها[liga[1]]))
		چاپ ("  }},")

	چاپ ("};")
	چاپ ()

	max_i = max (طول (ligas_mark_2[l]) لكل l في ligas_mark_2)
	چاپ ()
	چاپ ("static const struct ligature_mark_set_t {")
	چاپ (" uint16_t first;")
	چاپ (" struct ligature_pairs_t {")
	چاپ ("   uint16_t components[1];")
	چاپ ("   uint16_t ligature;")
	چاپ (" } ligatures[%d];" % max_i)
	چاپ ("} ligature_mark_table[] =")
	چاپ ("{")
	لكل اولی في sorted (ligas_mark_2.کلیدها ()):

		چاپ ("  { 0x%04Xu, {" % (اولی))
		لكل liga في ligas_mark_2[اولی]:
			چاپ ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نامها[liga[1]]))
		چاپ ("  }},")

	چاپ ("};")
	چاپ ()

	max_i = max (طول (ligas_3[l]) لكل l في ligas_3)
	چاپ ()
	چاپ ("static const struct ligature_3_set_t {")
	چاپ (" uint16_t first;")
	چاپ (" struct ligature_triplets_t {")
	چاپ ("   uint16_t components[2];")
	چاپ ("   uint16_t ligature;")
	چاپ (" } ligatures[%d];" % max_i)
	چاپ ("} ligature_3_table[] =")
	چاپ ("{")
	لكل اولی في sorted (ligas_3.کلیدها ()):

		چاپ ("  { 0x%04Xu, {" % (اولی))
		لكل liga في ligas_3[اولی]:
			چاپ ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], نامها[liga[2]]))
		چاپ ("  }},")

	چاپ ("};")
	چاپ ()


تعریف print_shaping_جدول_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)
	min_u, max_u = min (shapes.کلیدها ()), max (shapes.کلیدها ())

	چاپ ()
	چاپ ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	لكل u في دامنه (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في ['initial', 'medial', 'final', 'isolated']]
		إذا any (s):
			مقدار_2 = ", ".join ("0x%04X" % c لكل c في s)
			چاپ ("    (0x%04X, [%s])," % (u, مقدار_2))
	چاپ ("];")

	چاپ ()
	چاپ ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	لكل اولی في sorted (ligas_3.کلیدها ()):
		لكل liga في ligas_3[اولی]:
			چاپ ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (اولی, liga[0], liga[1], liga[2]))
	چاپ ("];")

	چاپ ()
	چاپ ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	لكل اولی في sorted (ligas_2.کلیدها ()):
		لكل liga في ligas_2[اولی]:
			چاپ ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (اولی, liga[0], liga[1]))
	چاپ ("];")

	چاپ ()
	چاپ ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	لكل اولی في sorted (ligas_mark_2.کلیدها ()):
		لكل liga في ligas_mark_2[اولی]:
			چاپ ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (اولی, liga[0], liga[1]))
	چاپ ("];")



چاپ ("/* == Start of generated table == */")
چاپ ("/*")
چاپ (" * The following table is generated by running:")
چاپ (" *")
چاپ (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " إذا language.name == "rust" وإلا ""))
چاپ (" *")
چاپ (" * on files with these headers:")
چاپ (" *")
لكل h في سرآیندها:
	لكل l في h:
		چاپ (" * %s" % (l.strip()))
چاپ (" */")
چاپ ()
read_بلوکها (files[2])
إذا language.name == "c":
	چاپ ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	چاپ ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	چاپ ()

	print_joining_جدول (files[0])
	print_shaping_جدول (files[1])

	چاپ ()
	چاپ ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وإلاإذا language.name == "rust":
	print_joining_جدول (files[0])
	print_shaping_جدول_rust (files[1])
وإلا:
	أكد خطأ, "Unknown language: %s" % language.name
چاپ ()
چاپ ("/* == End of generated table == */")
