#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

واردکن os.path, sys

واردکن packTab

اگر len(sys.argv) > 1 و sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
وگرنه:
	language = packTab.languages["c"]

اگر len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') برای x در sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
وقتی files[0].readline ().find ('##################') < 0:
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
	برای سطر در f:

		j = سطر.find ('#')
		اگر j >= 0:
			سطر = سطر[:j]

		fields = [x.strip () برای x در سطر.split (';')]
		اگر len (fields) == 1:
			ادامه

		uu = fields[0].split ('..')
		start = صحیح (uu[0], 16)
		اگر len (uu) == 1:
			end = start
		وگرنه:
			end = صحیح (uu[1], 16)

		t = fields[1]

		برای u در range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	مقادیر = {}
	برای سطر در f:

		اگر سطر[0] == '#':
			ادامه

		fields = [x.strip () برای x در سطر.split (';')]
		اگر len (fields) == 1:
			ادامه

		u = صحیح (fields[0], 16)

		اگر fields[3] در ["ALAPH", "DALATH RISH"]:
			مقدار_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		وگرنه:
			مقدار_2 = "JOINING_TYPE_" + fields[2]
		مقادیر[u] = مقدار_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] برای u, v در مقادیر.اقلام ()}
	sol = packTab.pack_table (data, پیشفرض=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	اگر language.name == "c":
		sol.genCode (code, 'joining_type', language=language, خصوصی=درست)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, خصوصی=درست)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	وگرنهاگر language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, خصوصی=نادرست)

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
		code.print_code (file=sys.stdout, language=language, خصوصی=نادرست)
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
	وگرنه:
		أكد نادرست, "Unknown language: %s" % language.name

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
	سطرها = f.readlines()
	سطرها += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	برای سطر در سطرها:

		fields = [x.strip () برای x در سطر.split (';')]
		اگر fields[5][0:1] != '<':
			ادامه

		اقلام = fields[5].split (' ')
		shape, اقلام = اقلام[0][1:-1], tuple (صحیح (x, 16) برای x در اقلام[1:])
		c = صحیح (fields[0], 16)

		اگر نه shape در ['initial', 'medial', 'isolated', 'final']:
			ادامه

		اگر len (اقلام) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			اگر اقلام[0] == 0x0020:
				اقلام = اقلام[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			اگر c نه در LIGATURES:
				ادامه

			# Save ligature
			names[c] = fields[1]
			اگر اقلام نه در ligatures:
				ligatures[اقلام] = {}
			ligatures[اقلام][shape] = c
		وگرنه:
			# Save shape
			اگر اقلام[0] نه در names:
				names[اقلام[0]] = fields[1]
			وگرنه:
				names[اقلام[0]] = os.path.commonprefix ([names[اقلام[0]], fields[1]]).strip ()
			اگر اقلام[0] نه در shapes:
				shapes[اقلام[0]] = {}
			shapes[اقلام[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	برای کلید_2 در ligatures.کلیدها ():
		برای shape در ligatures[کلید_2]:
			c = ligatures[کلید_2][shape]
			اگر len(کلید_2) == 3:
				اگر shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وگرنهاگر shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['final'])
				وگرنهاگر shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'], shapes[کلید_2[2]]['medial'])
				وگرنه:
					بالاببر Exception ("Unexpected shape", shape)
				اگر liga[0] نه در ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			وگرنهاگر len(کلید_2) == 2:
				اگر shape is None:
					liga = کلید_2
					اگر liga[0] نه در ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					ادامه
				وگرنهاگر shape == 'isolated':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['final'])
				وگرنهاگر shape == 'final':
					liga = (shapes[کلید_2[0]]['medial'], shapes[کلید_2[1]]['final'])
				وگرنهاگر shape == 'initial':
					liga = (shapes[کلید_2[0]]['initial'], shapes[کلید_2[1]]['medial'])
				وگرنه:
					بالاببر Exception ("Unexpected shape", shape)
				اگر liga[0] نه در ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			وگرنه:
				بالاببر Exception ("Unexpected number of ligature components", کلید_2)

	بازگشت shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	کلیدها = shapes.کلیدها ()
	min_u, max_u = min (کلیدها), max (کلیدها)
	برای u در range (min_u, max_u + 1):
		s = [shapes[u][shape] اگر u در shapes و shape در shapes[u] وگرنه 0
		     برای shape در  ['initial', 'medial', 'final', 'isolated']]
		مقدار_2 = ', '.join ("0x%04Xu" % c برای c در s)
		print ("  {%s}, /* U+%04X %s */" % (مقدار_2, u, names[u] اگر u در names وگرنه ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) برای l در ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	برای first در sorted (ligas_2.کلیدها ()):

		print ("  { 0x%04Xu, {" % (first))
		برای liga در ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) برای l در ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	برای first در sorted (ligas_mark_2.کلیدها ()):

		print ("  { 0x%04Xu, {" % (first))
		برای liga در ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) برای l در ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	برای first در sorted (ligas_3.کلیدها ()):

		print ("  { 0x%04Xu, {" % (first))
		برای liga در ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.کلیدها ()), max (shapes.کلیدها ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	برای u در range (min_u, max_u + 1):
		s = [shapes[u][shape] اگر u در shapes و shape در shapes[u] وگرنه 0
		     برای shape در ['initial', 'medial', 'final', 'isolated']]
		اگر any (s):
			مقدار_2 = ", ".join ("0x%04X" % c برای c در s)
			print ("    (0x%04X, [%s])," % (u, مقدار_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	برای first در sorted (ligas_3.کلیدها ()):
		برای liga در ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	برای first در sorted (ligas_2.کلیدها ()):
		برای liga در ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	برای first در sorted (ligas_mark_2.کلیدها ()):
		برای liga در ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " اگر language.name == "rust" وگرنه ""))
print (" *")
print (" * on files with these headers:")
print (" *")
برای h در headers:
	برای l در h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
اگر language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
وگرنهاگر language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
وگرنه:
	أكد نادرست, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
