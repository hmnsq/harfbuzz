#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ଆମଦାନୀ os.path, sys

ଆମଦାନୀ packTab

ଯଦି len(sys.argv) > 1 ଏବଂ sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
ଅନ୍ୟଥା:
	language = packTab.languages["c"]

ଯଦି len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') ପାଇଁ x ଭିତରେ sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
ଯେତେବେଳେ files[0].readline ().find ('##################') < 0:
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
	ଵୈଶ୍ଵିକ blocks
	ପାଇଁ ଧାଡ଼ି ଭିତରେ f:

		j = ଧାଡ଼ି.find ('#')
		ଯଦି j >= 0:
			ଧାଡ଼ି = ଧାଡ଼ି[:j]

		fields = [x.strip () ପାଇଁ x ଭିତରେ ଧାଡ଼ି.split (';')]
		ଯଦି len (fields) == 1:
			ଜାରି

		uu = fields[0].split ('..')
		start = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[0], 16)
		ଯଦି len (uu) == 1:
			end = start
		ଅନ୍ୟଥା:
			end = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (uu[1], 16)

		t = fields[1]

		ପାଇଁ u ଭିତରେ range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	ମୂଲ୍ୟଗୁଡ଼ିକ = {}
	ପାଇଁ ଧାଡ଼ି ଭିତରେ f:

		ଯଦି ଧାଡ଼ି[0] == '#':
			ଜାରି

		fields = [x.strip () ପାଇଁ x ଭିତରେ ଧାଡ଼ି.split (';')]
		ଯଦି len (fields) == 1:
			ଜାରି

		u = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (fields[0], 16)

		ଯଦି fields[3] ଭିତରେ ["ALAPH", "DALATH RISH"]:
			ମୂଲ୍ୟ_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		ଅନ୍ୟଥା:
			ମୂଲ୍ୟ_2 = "JOINING_TYPE_" + fields[2]
		ମୂଲ୍ୟଗୁଡ଼ିକ[u] = ମୂଲ୍ୟ_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] ପାଇଁ u, v ଭିତରେ ମୂଲ୍ୟଗୁଡ଼ିକ.ବସ୍ତୁଗୁଡ଼ିକ ()}
	sol = packTab.pack_table (data, ପୂର୍ବନିର୍ଦ୍ଧାରିତ=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	ଯଦି language.name == "c":
		sol.genCode (code, 'joining_type', language=language, ବ୍ୟକ୍ତିଗତ=ସତ୍ୟ)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, ବ୍ୟକ୍ତିଗତ=ସତ୍ୟ)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	ନଚେତଯଦି language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, ବ୍ୟକ୍ତିଗତ=ମିଥ୍ୟା)

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
		code.print_code (file=sys.stdout, language=language, ବ୍ୟକ୍ତିଗତ=ମିଥ୍ୟା)
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
	ଅନ୍ୟଥା:
		ପ୍ରତିଜ୍ଞା ମିଥ୍ୟା, "Unknown language: %s" % language.name

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
	ଧାଡ଼ିଗୁଡ଼ିକ = f.readlines()
	ଧାଡ଼ିଗୁଡ଼ିକ += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	ପାଇଁ ଧାଡ଼ି ଭିତରେ ଧାଡ଼ିଗୁଡ଼ିକ:

		fields = [x.strip () ପାଇଁ x ଭିତରେ ଧାଡ଼ି.split (';')]
		ଯଦି fields[5][0:1] != '<':
			ଜାରି

		ବସ୍ତୁଗୁଡ଼ିକ = fields[5].split (' ')
		shape, ବସ୍ତୁଗୁଡ଼ିକ = ବସ୍ତୁଗୁଡ଼ିକ[0][1:-1], tuple (ପୂର୍ଣ୍ଣସଂଖ୍ୟା (x, 16) ପାଇଁ x ଭିତରେ ବସ୍ତୁଗୁଡ଼ିକ[1:])
		c = ପୂର୍ଣ୍ଣସଂଖ୍ୟା (fields[0], 16)

		ଯଦି ନୁହେଁ shape ଭିତରେ ['initial', 'medial', 'isolated', 'final']:
			ଜାରି

		ଯଦି len (ବସ୍ତୁଗୁଡ଼ିକ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			ଯଦି ବସ୍ତୁଗୁଡ଼ିକ[0] == 0x0020:
				ବସ୍ତୁଗୁଡ଼ିକ = ବସ୍ତୁଗୁଡ଼ିକ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			ଯଦି c ନୁହେଁ ଭିତରେ LIGATURES:
				ଜାରି

			# Save ligature
			names[c] = fields[1]
			ଯଦି ବସ୍ତୁଗୁଡ଼ିକ ନୁହେଁ ଭିତରେ ligatures:
				ligatures[ବସ୍ତୁଗୁଡ଼ିକ] = {}
			ligatures[ବସ୍ତୁଗୁଡ଼ିକ][shape] = c
		ଅନ୍ୟଥା:
			# Save shape
			ଯଦି ବସ୍ତୁଗୁଡ଼ିକ[0] ନୁହେଁ ଭିତରେ names:
				names[ବସ୍ତୁଗୁଡ଼ିକ[0]] = fields[1]
			ଅନ୍ୟଥା:
				names[ବସ୍ତୁଗୁଡ଼ିକ[0]] = os.path.commonprefix ([names[ବସ୍ତୁଗୁଡ଼ିକ[0]], fields[1]]).strip ()
			ଯଦି ବସ୍ତୁଗୁଡ଼ିକ[0] ନୁହେଁ ଭିତରେ shapes:
				shapes[ବସ୍ତୁଗୁଡ଼ିକ[0]] = {}
			shapes[ବସ୍ତୁଗୁଡ଼ିକ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	ପାଇଁ ଚାବି_2 ଭିତରେ ligatures.ଚାବିଗୁଡ଼ିକ ():
		ପାଇଁ shape ଭିତରେ ligatures[ଚାବି_2]:
			c = ligatures[ଚାବି_2][shape]
			ଯଦି len(ଚାବି_2) == 3:
				ଯଦି shape == 'isolated':
					liga = (shapes[ଚାବି_2[0]]['initial'], shapes[ଚାବି_2[1]]['medial'], shapes[ଚାବି_2[2]]['final'])
				ନଚେତଯଦି shape == 'final':
					liga = (shapes[ଚାବି_2[0]]['medial'], shapes[ଚାବି_2[1]]['medial'], shapes[ଚାବି_2[2]]['final'])
				ନଚେତଯଦି shape == 'initial':
					liga = (shapes[ଚାବି_2[0]]['initial'], shapes[ଚାବି_2[1]]['medial'], shapes[ଚାବି_2[2]]['medial'])
				ଅନ୍ୟଥା:
					ଉଠାଅ Exception ("Unexpected shape", shape)
				ଯଦି liga[0] ନୁହେଁ ଭିତରେ ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			ନଚେତଯଦି len(ଚାବି_2) == 2:
				ଯଦି shape is None:
					liga = ଚାବି_2
					ଯଦି liga[0] ନୁହେଁ ଭିତରେ ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					ଜାରି
				ନଚେତଯଦି shape == 'isolated':
					liga = (shapes[ଚାବି_2[0]]['initial'], shapes[ଚାବି_2[1]]['final'])
				ନଚେତଯଦି shape == 'final':
					liga = (shapes[ଚାବି_2[0]]['medial'], shapes[ଚାବି_2[1]]['final'])
				ନଚେତଯଦି shape == 'initial':
					liga = (shapes[ଚାବି_2[0]]['initial'], shapes[ଚାବି_2[1]]['medial'])
				ଅନ୍ୟଥା:
					ଉଠାଅ Exception ("Unexpected shape", shape)
				ଯଦି liga[0] ନୁହେଁ ଭିତରେ ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			ଅନ୍ୟଥା:
				ଉଠାଅ Exception ("Unexpected number of ligature components", ଚାବି_2)

	ଫେରନ୍ତୁ shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	ଚାବିଗୁଡ଼ିକ = shapes.ଚାବିଗୁଡ଼ିକ ()
	min_u, max_u = min (ଚାବିଗୁଡ଼ିକ), max (ଚାବିଗୁଡ଼ିକ)
	ପାଇଁ u ଭିତରେ range (min_u, max_u + 1):
		s = [shapes[u][shape] ଯଦି u ଭିତରେ shapes ଏବଂ shape ଭିତରେ shapes[u] ଅନ୍ୟଥା 0
		     ପାଇଁ shape ଭିତରେ  ['initial', 'medial', 'final', 'isolated']]
		ମୂଲ୍ୟ_2 = ', '.join ("0x%04Xu" % c ପାଇଁ c ଭିତରେ s)
		print ("  {%s}, /* U+%04X %s */" % (ମୂଲ୍ୟ_2, u, names[u] ଯଦି u ଭିତରେ names ଅନ୍ୟଥା ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) ପାଇଁ l ଭିତରେ ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	ପାଇଁ first ଭିତରେ sorted (ligas_2.ଚାବିଗୁଡ଼ିକ ()):

		print ("  { 0x%04Xu, {" % (first))
		ପାଇଁ liga ଭିତରେ ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) ପାଇଁ l ଭିତରେ ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	ପାଇଁ first ଭିତରେ sorted (ligas_mark_2.ଚାବିଗୁଡ଼ିକ ()):

		print ("  { 0x%04Xu, {" % (first))
		ପାଇଁ liga ଭିତରେ ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) ପାଇଁ l ଭିତରେ ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	ପାଇଁ first ଭିତରେ sorted (ligas_3.ଚାବିଗୁଡ଼ିକ ()):

		print ("  { 0x%04Xu, {" % (first))
		ପାଇଁ liga ଭିତରେ ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.ଚାବିଗୁଡ଼ିକ ()), max (shapes.ଚାବିଗୁଡ଼ିକ ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	ପାଇଁ u ଭିତରେ range (min_u, max_u + 1):
		s = [shapes[u][shape] ଯଦି u ଭିତରେ shapes ଏବଂ shape ଭିତରେ shapes[u] ଅନ୍ୟଥା 0
		     ପାଇଁ shape ଭିତରେ ['initial', 'medial', 'final', 'isolated']]
		ଯଦି any (s):
			ମୂଲ୍ୟ_2 = ", ".join ("0x%04X" % c ପାଇଁ c ଭିତରେ s)
			print ("    (0x%04X, [%s])," % (u, ମୂଲ୍ୟ_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	ପାଇଁ first ଭିତରେ sorted (ligas_3.ଚାବିଗୁଡ଼ିକ ()):
		ପାଇଁ liga ଭିତରେ ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	ପାଇଁ first ଭିତରେ sorted (ligas_2.ଚାବିଗୁଡ଼ିକ ()):
		ପାଇଁ liga ଭିତରେ ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	ପାଇଁ first ଭିତରେ sorted (ligas_mark_2.ଚାବିଗୁଡ଼ିକ ()):
		ପାଇଁ liga ଭିତରେ ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " ଯଦି language.name == "rust" ଅନ୍ୟଥା ""))
print (" *")
print (" * on files with these headers:")
print (" *")
ପାଇଁ h ଭିତରେ headers:
	ପାଇଁ l ଭିତରେ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
ଯଦି language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
ନଚେତଯଦି language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
ଅନ୍ୟଥା:
	ପ୍ରତିଜ୍ଞା ମିଥ୍ୟା, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
