#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

దిగుమతి os.మార్గం, sys

దిగుమతి packTab

ఒకవేళ పొడవు(sys.argv) > 1 మరియు sys.argv[1] == "--rust":
	తీసివేయి sys.argv[1]
	language = packTab.languages["rust"]
లేకపోతే:
	language = packTab.languages["c"]

ఒకవేళ పొడవు (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') కొరకు x లో sys.argv[1:]]

శీర్షికలు = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
శీర్షికలు.append (["UnicodeData.txt does not have a header."])
అయితే files[0].readline ().కనుగొను ('##################') < 0:
	దాటు

ఖండాలు = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_సంకేతం = {
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

నిర్వచనం read_ఖండాలు(f):
	వైశ్విక ఖండాలు
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

		t = fields[1]

		కొరకు u లో శ్రేణి (ప్రారంభం, చివర + 1):
			ఖండాలు[u] = t

నిర్వచనం print_joining_పట్టిక(f):

	విలువలు = {}
	కొరకు పంక్తి లో f:

		ఒకవేళ పంక్తి[0] == '#':
			కొనసాగు

		fields = [x.strip () కొరకు x లో పంక్తి.విభజించు (';')]
		ఒకవేళ పొడవు (fields) == 1:
			కొనసాగు

		u = పూర్ణాంకం (fields[0], 16)

		ఒకవేళ fields[3] లో ["ALAPH", "DALATH RISH"]:
			విలువ_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		లేకపోతే:
			విలువ_2 = "JOINING_TYPE_" + fields[2]
		విలువలు[u] = విలువ_2

	సంకేతం_2 = packTab.సంకేతం_3 ('_hb_arabic_joining')
	దత్తాంశం = {u: JOINING_సంకేతం[v] కొరకు u, v లో విలువలు.వస్తువులు ()}
	sol = packTab.pack_పట్టిక (దత్తాంశం, అప్రమేయ=JOINING_సంకేతం['JOINING_TYPE_X'], compression=9)
	ఒకవేళ language.name == "c":
		sol.gen_సంకేతం (సంకేతం_2, 'joining_type', language=language, అంతరంగ=నిజం)

		ముద్రించు ("#include \"hb.hh\"")
		ముద్రించు ()
		సంకేతం_2.print_సంకేతం (file=sys.stdout, language=language, అంతరంగ=నిజం)
		ముద్రించు ()
		ముద్రించు ("static unsigned int")
		ముద్రించు ("joining_type (hb_codepoint_t u)")
		ముద్రించు ("{")
		ముద్రించు ("  return _hb_arabic_joining_joining_type (u);")
		ముద్రించు ("}")
		ముద్రించు ()
	లేదావేళ language.name == "rust":
		sol.gen_సంకేతం (సంకేతం_2, 'joining_type_u8', language=language, అంతరంగ=అబద్ధం)

		ముద్రించు ("#![allow(unused_parens)]")
		ముద్రించు ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		ముద్రించు ()
		ముద్రించు ("use crate::unicode::Codepoint;")
		ముద్రించు ()
		ముద్రించు (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		ముద్రించు ()
		సంకేతం_2.print_సంకేతం (file=sys.stdout, language=language, అంతరంగ=అబద్ధం)
		ముద్రించు ()
		ముద్రించు ("#[inline]")
		ముద్రించు ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		ముద్రించు ("{")
		ముద్రించు ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		ముద్రించు ("    0 => U,")
		ముద్రించు ("    1 => L,")
		ముద్రించు ("    2 => R,")
		ముద్రించు ("    3 => D,")
		ముద్రించు ("    4 => GroupAlaph,")
		ముద్రించు ("    5 => GroupDalathRish,")
		ముద్రించు ("    6 => T,")
		ముద్రించు ("    7 => X,")
		ముద్రించు ("    _ => unreachable! (),")
		ముద్రించు ("  }")
		ముద్రించు ("}")
		ముద్రించు ()
	లేకపోతే:
		ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name

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

నిర్వచనం read_shaping_పట్టిక(f):

	shapes = {}
	ligatures = {}
	పేర్లు = {}
	పంక్తులు = f.readlines()
	పంక్తులు += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	కొరకు పంక్తి లో పంక్తులు:

		fields = [x.strip () కొరకు x లో పంక్తి.విభజించు (';')]
		ఒకవేళ fields[5][0:1] != '<':
			కొనసాగు

		వస్తువులు = fields[5].విభజించు (' ')
		shape, వస్తువులు = వస్తువులు[0][1:-1], tuple (పూర్ణాంకం (x, 16) కొరకు x లో వస్తువులు[1:])
		c = పూర్ణాంకం (fields[0], 16)

		ఒకవేళ కాదు shape లో ['initial', 'medial', 'isolated', 'final']:
			కొనసాగు

		ఒకవేళ పొడవు (వస్తువులు) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			ఒకవేళ వస్తువులు[0] == 0x0020:
				వస్తువులు = వస్తువులు[:0:-1]
				shape = ఏమీలేదు
			# We only care about a subset of ligatures
			ఒకవేళ c కాదు లో LIGATURES:
				కొనసాగు

			# Save ligature
			పేర్లు[c] = fields[1]
			ఒకవేళ వస్తువులు కాదు లో ligatures:
				ligatures[వస్తువులు] = {}
			ligatures[వస్తువులు][shape] = c
		లేకపోతే:
			# Save shape
			ఒకవేళ వస్తువులు[0] కాదు లో పేర్లు:
				పేర్లు[వస్తువులు[0]] = fields[1]
			లేకపోతే:
				పేర్లు[వస్తువులు[0]] = os.మార్గం.commonprefix ([పేర్లు[వస్తువులు[0]], fields[1]]).strip ()
			ఒకవేళ వస్తువులు[0] కాదు లో shapes:
				shapes[వస్తువులు[0]] = {}
			shapes[వస్తువులు[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	కొరకు కీలకం_2 లో ligatures.కీలకాలు ():
		కొరకు shape లో ligatures[కీలకం_2]:
			c = ligatures[కీలకం_2][shape]
			ఒకవేళ పొడవు(కీలకం_2) == 3:
				ఒకవేళ shape == 'isolated':
					liga = (shapes[కీలకం_2[0]]['initial'], shapes[కీలకం_2[1]]['medial'], shapes[కీలకం_2[2]]['final'])
				లేదావేళ shape == 'final':
					liga = (shapes[కీలకం_2[0]]['medial'], shapes[కీలకం_2[1]]['medial'], shapes[కీలకం_2[2]]['final'])
				లేదావేళ shape == 'initial':
					liga = (shapes[కీలకం_2[0]]['initial'], shapes[కీలకం_2[1]]['medial'], shapes[కీలకం_2[2]]['medial'])
				లేకపోతే:
					లేపు Exception ("Unexpected shape", shape)
				ఒకవేళ liga[0] కాదు లో ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			లేదావేళ పొడవు(కీలకం_2) == 2:
				ఒకవేళ shape అయిన ఏమీలేదు:
					liga = కీలకం_2
					ఒకవేళ liga[0] కాదు లో ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					కొనసాగు
				లేదావేళ shape == 'isolated':
					liga = (shapes[కీలకం_2[0]]['initial'], shapes[కీలకం_2[1]]['final'])
				లేదావేళ shape == 'final':
					liga = (shapes[కీలకం_2[0]]['medial'], shapes[కీలకం_2[1]]['final'])
				లేదావేళ shape == 'initial':
					liga = (shapes[కీలకం_2[0]]['initial'], shapes[కీలకం_2[1]]['medial'])
				లేకపోతే:
					లేపు Exception ("Unexpected shape", shape)
				ఒకవేళ liga[0] కాదు లో ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			లేకపోతే:
				లేపు Exception ("Unexpected number of ligature components", కీలకం_2)

	తిరిగి shapes, పేర్లు, ligas_2, ligas_mark_2, ligas_3


నిర్వచనం print_shaping_పట్టిక(f):

	shapes, పేర్లు, ligas_2, ligas_mark_2, ligas_3 = read_shaping_పట్టిక (f)

	ముద్రించు ()
	ముద్రించు ("static const uint16_t shaping_table[][4] =")
	ముద్రించు ("{")

	కీలకాలు = shapes.కీలకాలు ()
	min_u, max_u = min (కీలకాలు), max (కీలకాలు)
	కొరకు u లో శ్రేణి (min_u, max_u + 1):
		s = [shapes[u][shape] ఒకవేళ u లో shapes మరియు shape లో shapes[u] లేకపోతే 0
		     కొరకు shape లో  ['initial', 'medial', 'final', 'isolated']]
		విలువ_2 = ', '.join ("0x%04Xu" % c కొరకు c లో s)
		ముద్రించు ("  {%s}, /* U+%04X %s */" % (విలువ_2, u, పేర్లు[u] ఒకవేళ u లో పేర్లు లేకపోతే ""))

	ముద్రించు ("};")
	ముద్రించు ()
	ముద్రించు ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	ముద్రించు ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	ముద్రించు ()

	max_i = max (పొడవు (ligas_2[l]) కొరకు l లో ligas_2)
	ముద్రించు ()
	ముద్రించు ("static const struct ligature_set_t {")
	ముద్రించు (" uint16_t first;")
	ముద్రించు (" struct ligature_pairs_t {")
	ముద్రించు ("   uint16_t components[1];")
	ముద్రించు ("   uint16_t ligature;")
	ముద్రించు (" } ligatures[%d];" % max_i)
	ముద్రించు ("} ligature_table[] =")
	ముద్రించు ("{")
	కొరకు మొదటి లో sorted (ligas_2.కీలకాలు ()):

		ముద్రించు ("  { 0x%04Xu, {" % (మొదటి))
		కొరకు liga లో ligas_2[మొదటి]:
			ముద్రించు ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], పేర్లు[liga[1]]))
		ముద్రించు ("  }},")

	ముద్రించు ("};")
	ముద్రించు ()

	max_i = max (పొడవు (ligas_mark_2[l]) కొరకు l లో ligas_mark_2)
	ముద్రించు ()
	ముద్రించు ("static const struct ligature_mark_set_t {")
	ముద్రించు (" uint16_t first;")
	ముద్రించు (" struct ligature_pairs_t {")
	ముద్రించు ("   uint16_t components[1];")
	ముద్రించు ("   uint16_t ligature;")
	ముద్రించు (" } ligatures[%d];" % max_i)
	ముద్రించు ("} ligature_mark_table[] =")
	ముద్రించు ("{")
	కొరకు మొదటి లో sorted (ligas_mark_2.కీలకాలు ()):

		ముద్రించు ("  { 0x%04Xu, {" % (మొదటి))
		కొరకు liga లో ligas_mark_2[మొదటి]:
			ముద్రించు ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], పేర్లు[liga[1]]))
		ముద్రించు ("  }},")

	ముద్రించు ("};")
	ముద్రించు ()

	max_i = max (పొడవు (ligas_3[l]) కొరకు l లో ligas_3)
	ముద్రించు ()
	ముద్రించు ("static const struct ligature_3_set_t {")
	ముద్రించు (" uint16_t first;")
	ముద్రించు (" struct ligature_triplets_t {")
	ముద్రించు ("   uint16_t components[2];")
	ముద్రించు ("   uint16_t ligature;")
	ముద్రించు (" } ligatures[%d];" % max_i)
	ముద్రించు ("} ligature_3_table[] =")
	ముద్రించు ("{")
	కొరకు మొదటి లో sorted (ligas_3.కీలకాలు ()):

		ముద్రించు ("  { 0x%04Xu, {" % (మొదటి))
		కొరకు liga లో ligas_3[మొదటి]:
			ముద్రించు ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], పేర్లు[liga[2]]))
		ముద్రించు ("  }},")

	ముద్రించు ("};")
	ముద్రించు ()


నిర్వచనం print_shaping_పట్టిక_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_పట్టిక (f)
	min_u, max_u = min (shapes.కీలకాలు ()), max (shapes.కీలకాలు ())

	ముద్రించు ()
	ముద్రించు ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	కొరకు u లో శ్రేణి (min_u, max_u + 1):
		s = [shapes[u][shape] ఒకవేళ u లో shapes మరియు shape లో shapes[u] లేకపోతే 0
		     కొరకు shape లో ['initial', 'medial', 'final', 'isolated']]
		ఒకవేళ any (s):
			విలువ_2 = ", ".join ("0x%04X" % c కొరకు c లో s)
			ముద్రించు ("    (0x%04X, [%s])," % (u, విలువ_2))
	ముద్రించు ("];")

	ముద్రించు ()
	ముద్రించు ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	కొరకు మొదటి లో sorted (ligas_3.కీలకాలు ()):
		కొరకు liga లో ligas_3[మొదటి]:
			ముద్రించు ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (మొదటి, liga[0], liga[1], liga[2]))
	ముద్రించు ("];")

	ముద్రించు ()
	ముద్రించు ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	కొరకు మొదటి లో sorted (ligas_2.కీలకాలు ()):
		కొరకు liga లో ligas_2[మొదటి]:
			ముద్రించు ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (మొదటి, liga[0], liga[1]))
	ముద్రించు ("];")

	ముద్రించు ()
	ముద్రించు ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	కొరకు మొదటి లో sorted (ligas_mark_2.కీలకాలు ()):
		కొరకు liga లో ligas_mark_2[మొదటి]:
			ముద్రించు ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (మొదటి, liga[0], liga[1]))
	ముద్రించు ("];")



ముద్రించు ("/* == Start of generated table == */")
ముద్రించు ("/*")
ముద్రించు (" * The following table is generated by running:")
ముద్రించు (" *")
ముద్రించు (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " ఒకవేళ language.name == "rust" లేకపోతే ""))
ముద్రించు (" *")
ముద్రించు (" * on files with these headers:")
ముద్రించు (" *")
కొరకు h లో శీర్షికలు:
	కొరకు l లో h:
		ముద్రించు (" * %s" % (l.strip()))
ముద్రించు (" */")
ముద్రించు ()
read_ఖండాలు (files[2])
ఒకవేళ language.name == "c":
	ముద్రించు ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	ముద్రించు ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	ముద్రించు ()

	print_joining_పట్టిక (files[0])
	print_shaping_పట్టిక (files[1])

	ముద్రించు ()
	ముద్రించు ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
లేదావేళ language.name == "rust":
	print_joining_పట్టిక (files[0])
	print_shaping_పట్టిక_rust (files[1])
లేకపోతే:
	ప్రతిజ్ఞా అబద్ధం, "Unknown language: %s" % language.name
ముద్రించు ()
ముద్రించు ("/* == End of generated table == */")
