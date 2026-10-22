#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ਆਯਾਤ os.ਰਸਤਾ, sys

ਆਯਾਤ packTab

ਜੇ ਲੰਬਾਈ(sys.argv) > 1 ਅਤੇ sys.argv[1] == "--rust":
	ਹਟਾਓ sys.argv[1]
	language = packTab.languages["rust"]
ਨਹੀਂਤਾਂ:
	language = packTab.languages["c"]

ਜੇ ਲੰਬਾਈ (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') ਲਈ x ਵਿੱਚ sys.argv[1:]]

ਸਿਰਲੇਖਾਂ = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
ਸਿਰਲੇਖਾਂ.append (["UnicodeData.txt does not have a header."])
ਜਦੋਂ files[0].readline ().ਲੱਭੋ ('##################') < 0:
	ਲੰਘੋ

ਖੰਡਾਂ = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_ਕੋਡ = {
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

ਪਰਿਭਾਸ਼ਾ read_ਖੰਡਾਂ(f):
	ਵੈਸ਼੍ਵਿਕ ਖੰਡਾਂ
	ਲਈ ਸਤਰ ਵਿੱਚ f:

		j = ਸਤਰ.ਲੱਭੋ ('#')
		ਜੇ j >= 0:
			ਸਤਰ = ਸਤਰ[:j]

		fields = [x.strip () ਲਈ x ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')]
		ਜੇ ਲੰਬਾਈ (fields) == 1:
			ਜਾਰੀ

		uu = fields[0].ਵੰਡੋ ('..')
		ਸ਼ੁਰੂ = ਪੂਰਨ (uu[0], 16)
		ਜੇ ਲੰਬਾਈ (uu) == 1:
			ਸਿਰਾ = ਸ਼ੁਰੂ
		ਨਹੀਂਤਾਂ:
			ਸਿਰਾ = ਪੂਰਨ (uu[1], 16)

		t = fields[1]

		ਲਈ u ਵਿੱਚ ਸੀਮਾ (ਸ਼ੁਰੂ, ਸਿਰਾ + 1):
			ਖੰਡਾਂ[u] = t

ਪਰਿਭਾਸ਼ਾ print_joining_ਸਾਰਣੀ(f):

	ਮੁੱਲਾਂ = {}
	ਲਈ ਸਤਰ ਵਿੱਚ f:

		ਜੇ ਸਤਰ[0] == '#':
			ਜਾਰੀ

		fields = [x.strip () ਲਈ x ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')]
		ਜੇ ਲੰਬਾਈ (fields) == 1:
			ਜਾਰੀ

		u = ਪੂਰਨ (fields[0], 16)

		ਜੇ fields[3] ਵਿੱਚ ["ALAPH", "DALATH RISH"]:
			ਮੁੱਲ_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		ਨਹੀਂਤਾਂ:
			ਮੁੱਲ_2 = "JOINING_TYPE_" + fields[2]
		ਮੁੱਲਾਂ[u] = ਮੁੱਲ_2

	ਕੋਡ = packTab.ਕੋਡ_2 ('_hb_arabic_joining')
	ਅੰਕੜੇ = {u: JOINING_ਕੋਡ[v] ਲਈ u, v ਵਿੱਚ ਮੁੱਲਾਂ.ਚੀਜ਼ਾਂ ()}
	sol = packTab.pack_ਸਾਰਣੀ (ਅੰਕੜੇ, ਮੂਲ=JOINING_ਕੋਡ['JOINING_TYPE_X'], compression=9)
	ਜੇ language.name == "c":
		sol.gen_ਕੋਡ (ਕੋਡ, 'joining_type', language=language, ਨਿਜੀ=ਸੱਚ)

		ਛਾਪੋ ("#include \"hb.hh\"")
		ਛਾਪੋ ()
		ਕੋਡ.print_ਕੋਡ (file=sys.stdout, language=language, ਨਿਜੀ=ਸੱਚ)
		ਛਾਪੋ ()
		ਛਾਪੋ ("static unsigned int")
		ਛਾਪੋ ("joining_type (hb_codepoint_t u)")
		ਛਾਪੋ ("{")
		ਛਾਪੋ ("  return _hb_arabic_joining_joining_type (u);")
		ਛਾਪੋ ("}")
		ਛਾਪੋ ()
	ਜਾਂਜੇ language.name == "rust":
		sol.gen_ਕੋਡ (ਕੋਡ, 'joining_type_u8', language=language, ਨਿਜੀ=ਝੂਠ)

		ਛਾਪੋ ("#![allow(unused_parens)]")
		ਛਾਪੋ ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		ਛਾਪੋ ()
		ਛਾਪੋ ("use crate::unicode::Codepoint;")
		ਛਾਪੋ ()
		ਛਾਪੋ (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		ਛਾਪੋ ()
		ਕੋਡ.print_ਕੋਡ (file=sys.stdout, language=language, ਨਿਜੀ=ਝੂਠ)
		ਛਾਪੋ ()
		ਛਾਪੋ ("#[inline]")
		ਛਾਪੋ ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		ਛਾਪੋ ("{")
		ਛਾਪੋ ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		ਛਾਪੋ ("    0 => U,")
		ਛਾਪੋ ("    1 => L,")
		ਛਾਪੋ ("    2 => R,")
		ਛਾਪੋ ("    3 => D,")
		ਛਾਪੋ ("    4 => GroupAlaph,")
		ਛਾਪੋ ("    5 => GroupDalathRish,")
		ਛਾਪੋ ("    6 => T,")
		ਛਾਪੋ ("    7 => X,")
		ਛਾਪੋ ("    _ => unreachable! (),")
		ਛਾਪੋ ("  }")
		ਛਾਪੋ ("}")
		ਛਾਪੋ ()
	ਨਹੀਂਤਾਂ:
		ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name

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

ਪਰਿਭਾਸ਼ਾ read_shaping_ਸਾਰਣੀ(f):

	shapes = {}
	ligatures = {}
	ਨਾਂ = {}
	ਸਤਰਾਂ = f.readlines()
	ਸਤਰਾਂ += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	ਲਈ ਸਤਰ ਵਿੱਚ ਸਤਰਾਂ:

		fields = [x.strip () ਲਈ x ਵਿੱਚ ਸਤਰ.ਵੰਡੋ (';')]
		ਜੇ fields[5][0:1] != '<':
			ਜਾਰੀ

		ਚੀਜ਼ਾਂ = fields[5].ਵੰਡੋ (' ')
		shape, ਚੀਜ਼ਾਂ = ਚੀਜ਼ਾਂ[0][1:-1], tuple (ਪੂਰਨ (x, 16) ਲਈ x ਵਿੱਚ ਚੀਜ਼ਾਂ[1:])
		c = ਪੂਰਨ (fields[0], 16)

		ਜੇ ਨਹੀਂ shape ਵਿੱਚ ['initial', 'medial', 'isolated', 'final']:
			ਜਾਰੀ

		ਜੇ ਲੰਬਾਈ (ਚੀਜ਼ਾਂ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			ਜੇ ਚੀਜ਼ਾਂ[0] == 0x0020:
				ਚੀਜ਼ਾਂ = ਚੀਜ਼ਾਂ[:0:-1]
				shape = ਕੁਝ_ਨਹੀਂ
			# We only care about a subset of ligatures
			ਜੇ c ਨਹੀਂ ਵਿੱਚ LIGATURES:
				ਜਾਰੀ

			# Save ligature
			ਨਾਂ[c] = fields[1]
			ਜੇ ਚੀਜ਼ਾਂ ਨਹੀਂ ਵਿੱਚ ligatures:
				ligatures[ਚੀਜ਼ਾਂ] = {}
			ligatures[ਚੀਜ਼ਾਂ][shape] = c
		ਨਹੀਂਤਾਂ:
			# Save shape
			ਜੇ ਚੀਜ਼ਾਂ[0] ਨਹੀਂ ਵਿੱਚ ਨਾਂ:
				ਨਾਂ[ਚੀਜ਼ਾਂ[0]] = fields[1]
			ਨਹੀਂਤਾਂ:
				ਨਾਂ[ਚੀਜ਼ਾਂ[0]] = os.ਰਸਤਾ.commonprefix ([ਨਾਂ[ਚੀਜ਼ਾਂ[0]], fields[1]]).strip ()
			ਜੇ ਚੀਜ਼ਾਂ[0] ਨਹੀਂ ਵਿੱਚ shapes:
				shapes[ਚੀਜ਼ਾਂ[0]] = {}
			shapes[ਚੀਜ਼ਾਂ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	ਲਈ ਕੁੰਜੀ_2 ਵਿੱਚ ligatures.ਕੁੰਜੀਆਂ ():
		ਲਈ shape ਵਿੱਚ ligatures[ਕੁੰਜੀ_2]:
			c = ligatures[ਕੁੰਜੀ_2][shape]
			ਜੇ ਲੰਬਾਈ(ਕੁੰਜੀ_2) == 3:
				ਜੇ shape == 'isolated':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['initial'], shapes[ਕੁੰਜੀ_2[1]]['medial'], shapes[ਕੁੰਜੀ_2[2]]['final'])
				ਜਾਂਜੇ shape == 'final':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['medial'], shapes[ਕੁੰਜੀ_2[1]]['medial'], shapes[ਕੁੰਜੀ_2[2]]['final'])
				ਜਾਂਜੇ shape == 'initial':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['initial'], shapes[ਕੁੰਜੀ_2[1]]['medial'], shapes[ਕੁੰਜੀ_2[2]]['medial'])
				ਨਹੀਂਤਾਂ:
					ਉਠਾਓ Exception ("Unexpected shape", shape)
				ਜੇ liga[0] ਨਹੀਂ ਵਿੱਚ ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			ਜਾਂਜੇ ਲੰਬਾਈ(ਕੁੰਜੀ_2) == 2:
				ਜੇ shape ਹੈ ਕੁਝ_ਨਹੀਂ:
					liga = ਕੁੰਜੀ_2
					ਜੇ liga[0] ਨਹੀਂ ਵਿੱਚ ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					ਜਾਰੀ
				ਜਾਂਜੇ shape == 'isolated':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['initial'], shapes[ਕੁੰਜੀ_2[1]]['final'])
				ਜਾਂਜੇ shape == 'final':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['medial'], shapes[ਕੁੰਜੀ_2[1]]['final'])
				ਜਾਂਜੇ shape == 'initial':
					liga = (shapes[ਕੁੰਜੀ_2[0]]['initial'], shapes[ਕੁੰਜੀ_2[1]]['medial'])
				ਨਹੀਂਤਾਂ:
					ਉਠਾਓ Exception ("Unexpected shape", shape)
				ਜੇ liga[0] ਨਹੀਂ ਵਿੱਚ ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			ਨਹੀਂਤਾਂ:
				ਉਠਾਓ Exception ("Unexpected number of ligature components", ਕੁੰਜੀ_2)

	ਵਾਪਸ shapes, ਨਾਂ, ligas_2, ligas_mark_2, ligas_3


ਪਰਿਭਾਸ਼ਾ print_shaping_ਸਾਰਣੀ(f):

	shapes, ਨਾਂ, ligas_2, ligas_mark_2, ligas_3 = read_shaping_ਸਾਰਣੀ (f)

	ਛਾਪੋ ()
	ਛਾਪੋ ("static const uint16_t shaping_table[][4] =")
	ਛਾਪੋ ("{")

	ਕੁੰਜੀਆਂ = shapes.ਕੁੰਜੀਆਂ ()
	min_u, max_u = min (ਕੁੰਜੀਆਂ), max (ਕੁੰਜੀਆਂ)
	ਲਈ u ਵਿੱਚ ਸੀਮਾ (min_u, max_u + 1):
		s = [shapes[u][shape] ਜੇ u ਵਿੱਚ shapes ਅਤੇ shape ਵਿੱਚ shapes[u] ਨਹੀਂਤਾਂ 0
		     ਲਈ shape ਵਿੱਚ  ['initial', 'medial', 'final', 'isolated']]
		ਮੁੱਲ_2 = ', '.join ("0x%04Xu" % c ਲਈ c ਵਿੱਚ s)
		ਛਾਪੋ ("  {%s}, /* U+%04X %s */" % (ਮੁੱਲ_2, u, ਨਾਂ[u] ਜੇ u ਵਿੱਚ ਨਾਂ ਨਹੀਂਤਾਂ ""))

	ਛਾਪੋ ("};")
	ਛਾਪੋ ()
	ਛਾਪੋ ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	ਛਾਪੋ ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	ਛਾਪੋ ()

	max_i = max (ਲੰਬਾਈ (ligas_2[l]) ਲਈ l ਵਿੱਚ ligas_2)
	ਛਾਪੋ ()
	ਛਾਪੋ ("static const struct ligature_set_t {")
	ਛਾਪੋ (" uint16_t first;")
	ਛਾਪੋ (" struct ligature_pairs_t {")
	ਛਾਪੋ ("   uint16_t components[1];")
	ਛਾਪੋ ("   uint16_t ligature;")
	ਛਾਪੋ (" } ligatures[%d];" % max_i)
	ਛਾਪੋ ("} ligature_table[] =")
	ਛਾਪੋ ("{")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_2.ਕੁੰਜੀਆਂ ()):

		ਛਾਪੋ ("  { 0x%04Xu, {" % (ਪਹਿਲਾ))
		ਲਈ liga ਵਿੱਚ ligas_2[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], ਨਾਂ[liga[1]]))
		ਛਾਪੋ ("  }},")

	ਛਾਪੋ ("};")
	ਛਾਪੋ ()

	max_i = max (ਲੰਬਾਈ (ligas_mark_2[l]) ਲਈ l ਵਿੱਚ ligas_mark_2)
	ਛਾਪੋ ()
	ਛਾਪੋ ("static const struct ligature_mark_set_t {")
	ਛਾਪੋ (" uint16_t first;")
	ਛਾਪੋ (" struct ligature_pairs_t {")
	ਛਾਪੋ ("   uint16_t components[1];")
	ਛਾਪੋ ("   uint16_t ligature;")
	ਛਾਪੋ (" } ligatures[%d];" % max_i)
	ਛਾਪੋ ("} ligature_mark_table[] =")
	ਛਾਪੋ ("{")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_mark_2.ਕੁੰਜੀਆਂ ()):

		ਛਾਪੋ ("  { 0x%04Xu, {" % (ਪਹਿਲਾ))
		ਲਈ liga ਵਿੱਚ ligas_mark_2[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], ਨਾਂ[liga[1]]))
		ਛਾਪੋ ("  }},")

	ਛਾਪੋ ("};")
	ਛਾਪੋ ()

	max_i = max (ਲੰਬਾਈ (ligas_3[l]) ਲਈ l ਵਿੱਚ ligas_3)
	ਛਾਪੋ ()
	ਛਾਪੋ ("static const struct ligature_3_set_t {")
	ਛਾਪੋ (" uint16_t first;")
	ਛਾਪੋ (" struct ligature_triplets_t {")
	ਛਾਪੋ ("   uint16_t components[2];")
	ਛਾਪੋ ("   uint16_t ligature;")
	ਛਾਪੋ (" } ligatures[%d];" % max_i)
	ਛਾਪੋ ("} ligature_3_table[] =")
	ਛਾਪੋ ("{")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_3.ਕੁੰਜੀਆਂ ()):

		ਛਾਪੋ ("  { 0x%04Xu, {" % (ਪਹਿਲਾ))
		ਲਈ liga ਵਿੱਚ ligas_3[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], ਨਾਂ[liga[2]]))
		ਛਾਪੋ ("  }},")

	ਛਾਪੋ ("};")
	ਛਾਪੋ ()


ਪਰਿਭਾਸ਼ਾ print_shaping_ਸਾਰਣੀ_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_ਸਾਰਣੀ (f)
	min_u, max_u = min (shapes.ਕੁੰਜੀਆਂ ()), max (shapes.ਕੁੰਜੀਆਂ ())

	ਛਾਪੋ ()
	ਛਾਪੋ ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	ਲਈ u ਵਿੱਚ ਸੀਮਾ (min_u, max_u + 1):
		s = [shapes[u][shape] ਜੇ u ਵਿੱਚ shapes ਅਤੇ shape ਵਿੱਚ shapes[u] ਨਹੀਂਤਾਂ 0
		     ਲਈ shape ਵਿੱਚ ['initial', 'medial', 'final', 'isolated']]
		ਜੇ any (s):
			ਮੁੱਲ_2 = ", ".join ("0x%04X" % c ਲਈ c ਵਿੱਚ s)
			ਛਾਪੋ ("    (0x%04X, [%s])," % (u, ਮੁੱਲ_2))
	ਛਾਪੋ ("];")

	ਛਾਪੋ ()
	ਛਾਪੋ ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_3.ਕੁੰਜੀਆਂ ()):
		ਲਈ liga ਵਿੱਚ ligas_3[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (ਪਹਿਲਾ, liga[0], liga[1], liga[2]))
	ਛਾਪੋ ("];")

	ਛਾਪੋ ()
	ਛਾਪੋ ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_2.ਕੁੰਜੀਆਂ ()):
		ਲਈ liga ਵਿੱਚ ligas_2[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (ਪਹਿਲਾ, liga[0], liga[1]))
	ਛਾਪੋ ("];")

	ਛਾਪੋ ()
	ਛਾਪੋ ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	ਲਈ ਪਹਿਲਾ ਵਿੱਚ sorted (ligas_mark_2.ਕੁੰਜੀਆਂ ()):
		ਲਈ liga ਵਿੱਚ ligas_mark_2[ਪਹਿਲਾ]:
			ਛਾਪੋ ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (ਪਹਿਲਾ, liga[0], liga[1]))
	ਛਾਪੋ ("];")



ਛਾਪੋ ("/* == Start of generated table == */")
ਛਾਪੋ ("/*")
ਛਾਪੋ (" * The following table is generated by running:")
ਛਾਪੋ (" *")
ਛਾਪੋ (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " ਜੇ language.name == "rust" ਨਹੀਂਤਾਂ ""))
ਛਾਪੋ (" *")
ਛਾਪੋ (" * on files with these headers:")
ਛਾਪੋ (" *")
ਲਈ h ਵਿੱਚ ਸਿਰਲੇਖਾਂ:
	ਲਈ l ਵਿੱਚ h:
		ਛਾਪੋ (" * %s" % (l.strip()))
ਛਾਪੋ (" */")
ਛਾਪੋ ()
read_ਖੰਡਾਂ (files[2])
ਜੇ language.name == "c":
	ਛਾਪੋ ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	ਛਾਪੋ ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	ਛਾਪੋ ()

	print_joining_ਸਾਰਣੀ (files[0])
	print_shaping_ਸਾਰਣੀ (files[1])

	ਛਾਪੋ ()
	ਛਾਪੋ ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
ਜਾਂਜੇ language.name == "rust":
	print_joining_ਸਾਰਣੀ (files[0])
	print_shaping_ਸਾਰਣੀ_rust (files[1])
ਨਹੀਂਤਾਂ:
	ਪ੍ਰਤਿਜ੍ਞਾ ਝੂਠ, "Unknown language: %s" % language.name
ਛਾਪੋ ()
ਛਾਪੋ ("/* == End of generated table == */")
