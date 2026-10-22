#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

استورد os.رستو, sys

استورد packTab

إذا ڊيگهه(sys.argv) > 1 و sys.argv[1] == "--rust":
	هٽايو sys.argv[1]
	language = packTab.languages["rust"]
وإلا:
	language = packTab.languages["c"]

إذا ڊيگهه (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

سرا = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
سرا.append (["UnicodeData.txt does not have a header."])
طالما files[0].readline ().ڳوليو ('##################') < 0:
	لنگهه

بلاڪون = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_ڪوڊ = {
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

وصف read_بلاڪون(f):
	عام بلاڪون
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

		t = fields[1]

		لكل u في سلسلو (شروعات, پڇاڙي + 1):
			بلاڪون[u] = t

وصف print_joining_جدول(f):

	قدرون = {}
	لكل سٽ في f:

		إذا سٽ[0] == '#':
			استمر

		fields = [x.strip () لكل x في سٽ.ورهايو (';')]
		إذا ڊيگهه (fields) == 1:
			استمر

		u = صحيح (fields[0], 16)

		إذا fields[3] في ["ALAPH", "DALATH RISH"]:
			قدر_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		وإلا:
			قدر_2 = "JOINING_TYPE_" + fields[2]
		قدرون[u] = قدر_2

	ڪوڊ = packTab.ڪوڊ_2 ('_hb_arabic_joining')
	معلومات = {u: JOINING_ڪوڊ[v] لكل u, v في قدرون.شيون ()}
	sol = packTab.pack_جدول (معلومات, افتراضي=JOINING_ڪوڊ['JOINING_TYPE_X'], compression=9)
	إذا language.name == "c":
		sol.gen_ڪوڊ (ڪوڊ, 'joining_type', language=language, خاص=سچ)

		ڇاپيو ("#include \"hb.hh\"")
		ڇاپيو ()
		ڪوڊ.print_ڪوڊ (file=sys.stdout, language=language, خاص=سچ)
		ڇاپيو ()
		ڇاپيو ("static unsigned int")
		ڇاپيو ("joining_type (hb_codepoint_t u)")
		ڇاپيو ("{")
		ڇاپيو ("  return _hb_arabic_joining_joining_type (u);")
		ڇاپيو ("}")
		ڇاپيو ()
	وإلاإذا language.name == "rust":
		sol.gen_ڪوڊ (ڪوڊ, 'joining_type_u8', language=language, خاص=خطأ)

		ڇاپيو ("#![allow(unused_parens)]")
		ڇاپيو ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		ڇاپيو ()
		ڇاپيو ("use crate::unicode::Codepoint;")
		ڇاپيو ()
		ڇاپيو (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		ڇاپيو ()
		ڪوڊ.print_ڪوڊ (file=sys.stdout, language=language, خاص=خطأ)
		ڇاپيو ()
		ڇاپيو ("#[inline]")
		ڇاپيو ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		ڇاپيو ("{")
		ڇاپيو ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		ڇاپيو ("    0 => U,")
		ڇاپيو ("    1 => L,")
		ڇاپيو ("    2 => R,")
		ڇاپيو ("    3 => D,")
		ڇاپيو ("    4 => GroupAlaph,")
		ڇاپيو ("    5 => GroupDalathRish,")
		ڇاپيو ("    6 => T,")
		ڇاپيو ("    7 => X,")
		ڇاپيو ("    _ => unreachable! (),")
		ڇاپيو ("  }")
		ڇاپيو ("}")
		ڇاپيو ()
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

وصف read_shaping_جدول(f):

	shapes = {}
	ligatures = {}
	نالا = {}
	سٽون = f.readlines()
	سٽون += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	لكل سٽ في سٽون:

		fields = [x.strip () لكل x في سٽ.ورهايو (';')]
		إذا fields[5][0:1] != '<':
			استمر

		شيون = fields[5].ورهايو (' ')
		shape, شيون = شيون[0][1:-1], tuple (صحيح (x, 16) لكل x في شيون[1:])
		c = صحيح (fields[0], 16)

		إذا ليس shape في ['initial', 'medial', 'isolated', 'final']:
			استمر

		إذا ڊيگهه (شيون) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			إذا شيون[0] == 0x0020:
				شيون = شيون[:0:-1]
				shape = ڪجهه_نه
			# We only care about a subset of ligatures
			إذا c ليس في LIGATURES:
				استمر

			# Save ligature
			نالا[c] = fields[1]
			إذا شيون ليس في ligatures:
				ligatures[شيون] = {}
			ligatures[شيون][shape] = c
		وإلا:
			# Save shape
			إذا شيون[0] ليس في نالا:
				نالا[شيون[0]] = fields[1]
			وإلا:
				نالا[شيون[0]] = os.رستو.commonprefix ([نالا[شيون[0]], fields[1]]).strip ()
			إذا شيون[0] ليس في shapes:
				shapes[شيون[0]] = {}
			shapes[شيون[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	لكل ڪنجي_2 في ligatures.ڪنجيون ():
		لكل shape في ligatures[ڪنجي_2]:
			c = ligatures[ڪنجي_2][shape]
			إذا ڊيگهه(ڪنجي_2) == 3:
				إذا shape == 'isolated':
					liga = (shapes[ڪنجي_2[0]]['initial'], shapes[ڪنجي_2[1]]['medial'], shapes[ڪنجي_2[2]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[ڪنجي_2[0]]['medial'], shapes[ڪنجي_2[1]]['medial'], shapes[ڪنجي_2[2]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[ڪنجي_2[0]]['initial'], shapes[ڪنجي_2[1]]['medial'], shapes[ڪنجي_2[2]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			وإلاإذا ڊيگهه(ڪنجي_2) == 2:
				إذا shape آهي ڪجهه_نه:
					liga = ڪنجي_2
					إذا liga[0] ليس في ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					استمر
				وإلاإذا shape == 'isolated':
					liga = (shapes[ڪنجي_2[0]]['initial'], shapes[ڪنجي_2[1]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[ڪنجي_2[0]]['medial'], shapes[ڪنجي_2[1]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[ڪنجي_2[0]]['initial'], shapes[ڪنجي_2[1]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			وإلا:
				ارفع Exception ("Unexpected number of ligature components", ڪنجي_2)

	أرجع shapes, نالا, ligas_2, ligas_mark_2, ligas_3


وصف print_shaping_جدول(f):

	shapes, نالا, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)

	ڇاپيو ()
	ڇاپيو ("static const uint16_t shaping_table[][4] =")
	ڇاپيو ("{")

	ڪنجيون = shapes.ڪنجيون ()
	min_u, max_u = min (ڪنجيون), max (ڪنجيون)
	لكل u في سلسلو (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في  ['initial', 'medial', 'final', 'isolated']]
		قدر_2 = ', '.join ("0x%04Xu" % c لكل c في s)
		ڇاپيو ("  {%s}, /* U+%04X %s */" % (قدر_2, u, نالا[u] إذا u في نالا وإلا ""))

	ڇاپيو ("};")
	ڇاپيو ()
	ڇاپيو ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	ڇاپيو ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	ڇاپيو ()

	max_i = max (ڊيگهه (ligas_2[l]) لكل l في ligas_2)
	ڇاپيو ()
	ڇاپيو ("static const struct ligature_set_t {")
	ڇاپيو (" uint16_t first;")
	ڇاپيو (" struct ligature_pairs_t {")
	ڇاپيو ("   uint16_t components[1];")
	ڇاپيو ("   uint16_t ligature;")
	ڇاپيو (" } ligatures[%d];" % max_i)
	ڇاپيو ("} ligature_table[] =")
	ڇاپيو ("{")
	لكل پهريون في sorted (ligas_2.ڪنجيون ()):

		ڇاپيو ("  { 0x%04Xu, {" % (پهريون))
		لكل liga في ligas_2[پهريون]:
			ڇاپيو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نالا[liga[1]]))
		ڇاپيو ("  }},")

	ڇاپيو ("};")
	ڇاپيو ()

	max_i = max (ڊيگهه (ligas_mark_2[l]) لكل l في ligas_mark_2)
	ڇاپيو ()
	ڇاپيو ("static const struct ligature_mark_set_t {")
	ڇاپيو (" uint16_t first;")
	ڇاپيو (" struct ligature_pairs_t {")
	ڇاپيو ("   uint16_t components[1];")
	ڇاپيو ("   uint16_t ligature;")
	ڇاپيو (" } ligatures[%d];" % max_i)
	ڇاپيو ("} ligature_mark_table[] =")
	ڇاپيو ("{")
	لكل پهريون في sorted (ligas_mark_2.ڪنجيون ()):

		ڇاپيو ("  { 0x%04Xu, {" % (پهريون))
		لكل liga في ligas_mark_2[پهريون]:
			ڇاپيو ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], نالا[liga[1]]))
		ڇاپيو ("  }},")

	ڇاپيو ("};")
	ڇاپيو ()

	max_i = max (ڊيگهه (ligas_3[l]) لكل l في ligas_3)
	ڇاپيو ()
	ڇاپيو ("static const struct ligature_3_set_t {")
	ڇاپيو (" uint16_t first;")
	ڇاپيو (" struct ligature_triplets_t {")
	ڇاپيو ("   uint16_t components[2];")
	ڇاپيو ("   uint16_t ligature;")
	ڇاپيو (" } ligatures[%d];" % max_i)
	ڇاپيو ("} ligature_3_table[] =")
	ڇاپيو ("{")
	لكل پهريون في sorted (ligas_3.ڪنجيون ()):

		ڇاپيو ("  { 0x%04Xu, {" % (پهريون))
		لكل liga في ligas_3[پهريون]:
			ڇاپيو ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], نالا[liga[2]]))
		ڇاپيو ("  }},")

	ڇاپيو ("};")
	ڇاپيو ()


وصف print_shaping_جدول_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_جدول (f)
	min_u, max_u = min (shapes.ڪنجيون ()), max (shapes.ڪنجيون ())

	ڇاپيو ()
	ڇاپيو ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	لكل u في سلسلو (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في ['initial', 'medial', 'final', 'isolated']]
		إذا any (s):
			قدر_2 = ", ".join ("0x%04X" % c لكل c في s)
			ڇاپيو ("    (0x%04X, [%s])," % (u, قدر_2))
	ڇاپيو ("];")

	ڇاپيو ()
	ڇاپيو ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	لكل پهريون في sorted (ligas_3.ڪنجيون ()):
		لكل liga في ligas_3[پهريون]:
			ڇاپيو ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (پهريون, liga[0], liga[1], liga[2]))
	ڇاپيو ("];")

	ڇاپيو ()
	ڇاپيو ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	لكل پهريون في sorted (ligas_2.ڪنجيون ()):
		لكل liga في ligas_2[پهريون]:
			ڇاپيو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پهريون, liga[0], liga[1]))
	ڇاپيو ("];")

	ڇاپيو ()
	ڇاپيو ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	لكل پهريون في sorted (ligas_mark_2.ڪنجيون ()):
		لكل liga في ligas_mark_2[پهريون]:
			ڇاپيو ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (پهريون, liga[0], liga[1]))
	ڇاپيو ("];")



ڇاپيو ("/* == Start of generated table == */")
ڇاپيو ("/*")
ڇاپيو (" * The following table is generated by running:")
ڇاپيو (" *")
ڇاپيو (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " إذا language.name == "rust" وإلا ""))
ڇاپيو (" *")
ڇاپيو (" * on files with these headers:")
ڇاپيو (" *")
لكل h في سرا:
	لكل l في h:
		ڇاپيو (" * %s" % (l.strip()))
ڇاپيو (" */")
ڇاپيو ()
read_بلاڪون (files[2])
إذا language.name == "c":
	ڇاپيو ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	ڇاپيو ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	ڇاپيو ()

	print_joining_جدول (files[0])
	print_shaping_جدول (files[1])

	ڇاپيو ()
	ڇاپيو ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وإلاإذا language.name == "rust":
	print_joining_جدول (files[0])
	print_shaping_جدول_rust (files[1])
وإلا:
	أكد خطأ, "Unknown language: %s" % language.name
ڇاپيو ()
ڇاپيو ("/* == End of generated table == */")
