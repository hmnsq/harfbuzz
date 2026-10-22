#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

استورد os.path, sys

استورد packTab

إذا len(sys.argv) > 1 و sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
وإلا:
	language = packTab.languages["c"]

إذا len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') لكل x في sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
طالما files[0].readline ().find ('##################') < 0:
	pass

blocks = {}
# Keep this in sync with hb_arabic_joiner_type_t in hb-ot-shaper-arabic.cc.
JOINING_CODE = {
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

def read_blocks(f):
	عام blocks
	لكل هێڵ في f:

		j = هێڵ.find ('#')
		إذا j >= 0:
			هێڵ = هێڵ[:j]

		fields = [x.strip () لكل x في هێڵ.split (';')]
		إذا len (fields) == 1:
			استمر

		uu = fields[0].split ('..')
		start = int (uu[0], 16)
		إذا len (uu) == 1:
			end = start
		وإلا:
			end = int (uu[1], 16)

		t = fields[1]

		لكل u في range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	بەهاکان = {}
	لكل هێڵ في f:

		إذا هێڵ[0] == '#':
			استمر

		fields = [x.strip () لكل x في هێڵ.split (';')]
		إذا len (fields) == 1:
			استمر

		u = int (fields[0], 16)

		إذا fields[3] في ["ALAPH", "DALATH RISH"]:
			بەها_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		وإلا:
			بەها_2 = "JOINING_TYPE_" + fields[2]
		بەهاکان[u] = بەها_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] لكل u, v في بەهاکان.کاڵاکان ()}
	sol = packTab.pack_table (data, افتراضي=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	إذا language.name == "c":
		sol.genCode (code, 'joining_type', language=language, خاص=صحيح)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, خاص=صحيح)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	وإلاإذا language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, خاص=خطأ)

		print ("#![allow(unused_parens)]")
		print ("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
		print ()
		print ("use crate::unicode::Codepoint;")
		print ()
		print (
			"use super::ot_shaper_arabic::hb_arabic_joining_type_t::{"
			"self, D, GroupAlaph, GroupDalathRish, L, R, T, U, X};"
		)
		print ()
		code.print_code (file=sys.stdout, language=language, خاص=خطأ)
		print ()
		print ("#[inline]")
		print ("pub(crate) fn joining_type (u: Codepoint) -> hb_arabic_joining_type_t")
		print ("{")
		print ("  match _hb_arabic_joining_joining_type_u8 (u as usize) {")
		print ("    0 => U,")
		print ("    1 => L,")
		print ("    2 => R,")
		print ("    3 => D,")
		print ("    4 => GroupAlaph,")
		print ("    5 => GroupDalathRish,")
		print ("    6 => T,")
		print ("    7 => X,")
		print ("    _ => unreachable! (),")
		print ("  }")
		print ("}")
		print ()
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

def read_shaping_table(f):

	shapes = {}
	ligatures = {}
	names = {}
	هێڵەکان = f.readlines()
	هێڵەکان += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	لكل هێڵ في هێڵەکان:

		fields = [x.strip () لكل x في هێڵ.split (';')]
		إذا fields[5][0:1] != '<':
			استمر

		کاڵاکان = fields[5].split (' ')
		shape, کاڵاکان = کاڵاکان[0][1:-1], tuple (int (x, 16) لكل x في کاڵاکان[1:])
		c = int (fields[0], 16)

		إذا ليس shape في ['initial', 'medial', 'isolated', 'final']:
			استمر

		إذا len (کاڵاکان) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			إذا کاڵاکان[0] == 0x0020:
				کاڵاکان = کاڵاکان[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			إذا c ليس في LIGATURES:
				استمر

			# Save ligature
			names[c] = fields[1]
			إذا کاڵاکان ليس في ligatures:
				ligatures[کاڵاکان] = {}
			ligatures[کاڵاکان][shape] = c
		وإلا:
			# Save shape
			إذا کاڵاکان[0] ليس في names:
				names[کاڵاکان[0]] = fields[1]
			وإلا:
				names[کاڵاکان[0]] = os.path.commonprefix ([names[کاڵاکان[0]], fields[1]]).strip ()
			إذا کاڵاکان[0] ليس في shapes:
				shapes[کاڵاکان[0]] = {}
			shapes[کاڵاکان[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	لكل کلیل_2 في ligatures.کلیلەکان ():
		لكل shape في ligatures[کلیل_2]:
			c = ligatures[کلیل_2][shape]
			إذا len(کلیل_2) == 3:
				إذا shape == 'isolated':
					liga = (shapes[کلیل_2[0]]['initial'], shapes[کلیل_2[1]]['medial'], shapes[کلیل_2[2]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[کلیل_2[0]]['medial'], shapes[کلیل_2[1]]['medial'], shapes[کلیل_2[2]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[کلیل_2[0]]['initial'], shapes[کلیل_2[1]]['medial'], shapes[کلیل_2[2]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			وإلاإذا len(کلیل_2) == 2:
				إذا shape is None:
					liga = کلیل_2
					إذا liga[0] ليس في ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					استمر
				وإلاإذا shape == 'isolated':
					liga = (shapes[کلیل_2[0]]['initial'], shapes[کلیل_2[1]]['final'])
				وإلاإذا shape == 'final':
					liga = (shapes[کلیل_2[0]]['medial'], shapes[کلیل_2[1]]['final'])
				وإلاإذا shape == 'initial':
					liga = (shapes[کلیل_2[0]]['initial'], shapes[کلیل_2[1]]['medial'])
				وإلا:
					ارفع Exception ("Unexpected shape", shape)
				إذا liga[0] ليس في ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			وإلا:
				ارفع Exception ("Unexpected number of ligature components", کلیل_2)

	أرجع shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	کلیلەکان = shapes.کلیلەکان ()
	min_u, max_u = min (کلیلەکان), max (کلیلەکان)
	لكل u في range (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في  ['initial', 'medial', 'final', 'isolated']]
		بەها_2 = ', '.join ("0x%04Xu" % c لكل c في s)
		print ("  {%s}, /* U+%04X %s */" % (بەها_2, u, names[u] إذا u في names وإلا ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) لكل l في ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	لكل first في sorted (ligas_2.کلیلەکان ()):

		print ("  { 0x%04Xu, {" % (first))
		لكل liga في ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) لكل l في ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	لكل first في sorted (ligas_mark_2.کلیلەکان ()):

		print ("  { 0x%04Xu, {" % (first))
		لكل liga في ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) لكل l في ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	لكل first في sorted (ligas_3.کلیلەکان ()):

		print ("  { 0x%04Xu, {" % (first))
		لكل liga في ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.کلیلەکان ()), max (shapes.کلیلەکان ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	لكل u في range (min_u, max_u + 1):
		s = [shapes[u][shape] إذا u في shapes و shape في shapes[u] وإلا 0
		     لكل shape في ['initial', 'medial', 'final', 'isolated']]
		إذا any (s):
			بەها_2 = ", ".join ("0x%04X" % c لكل c في s)
			print ("    (0x%04X, [%s])," % (u, بەها_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	لكل first في sorted (ligas_3.کلیلەکان ()):
		لكل liga في ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	لكل first في sorted (ligas_2.کلیلەکان ()):
		لكل liga في ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	لكل first في sorted (ligas_mark_2.کلیلەکان ()):
		لكل liga في ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " إذا language.name == "rust" وإلا ""))
print (" *")
print (" * on files with these headers:")
print (" *")
لكل h في headers:
	لكل l في h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
إذا language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وإلاإذا language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
وإلا:
	أكد خطأ, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
