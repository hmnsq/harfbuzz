#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

আমদানি os.path, sys

আমদানি packTab

যদি len(sys.argv) > 1 এবং sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
অন্যথা:
	language = packTab.languages["c"]

যদি len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') জন্য x মধ্যে sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
যখন files[0].readline ().find ('##################') < 0:
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
	বৈশ্বিক blocks
	জন্য সারি মধ্যে f:

		j = সারি.find ('#')
		যদি j >= 0:
			সারি = সারি[:j]

		fields = [x.strip () জন্য x মধ্যে সারি.split (';')]
		যদি len (fields) == 1:
			চালু

		uu = fields[0].split ('..')
		start = পূর্ণসংখ্যা (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণসংখ্যা (uu[1], 16)

		t = fields[1]

		জন্য u মধ্যে range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	মানসমূহ = {}
	জন্য সারি মধ্যে f:

		যদি সারি[0] == '#':
			চালু

		fields = [x.strip () জন্য x মধ্যে সারি.split (';')]
		যদি len (fields) == 1:
			চালু

		u = পূর্ণসংখ্যা (fields[0], 16)

		যদি fields[3] মধ্যে ["ALAPH", "DALATH RISH"]:
			মান_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		অন্যথা:
			মান_2 = "JOINING_TYPE_" + fields[2]
		মানসমূহ[u] = মান_2

	code = packTab.Code ('_hb_arabic_joining')
	তথ্য = {u: JOINING_CODE[v] জন্য u, v মধ্যে মানসমূহ.বস্তুসমূহ ()}
	sol = packTab.pack_table (তথ্য, পূর্বনির্ধারিত=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	যদি language.name == "c":
		sol.genCode (code, 'joining_type', language=language, ব্যক্তিগত=সত্য)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, ব্যক্তিগত=সত্য)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	নতুবা_যদি language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, ব্যক্তিগত=মিথ্যা)

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
		code.print_code (file=sys.stdout, language=language, ব্যক্তিগত=মিথ্যা)
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
	অন্যথা:
		প্রতিজ্ঞা মিথ্যা, "Unknown language: %s" % language.name

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
	সারিগুলি = f.readlines()
	সারিগুলি += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	জন্য সারি মধ্যে সারিগুলি:

		fields = [x.strip () জন্য x মধ্যে সারি.split (';')]
		যদি fields[5][0:1] != '<':
			চালু

		বস্তুসমূহ = fields[5].split (' ')
		shape, বস্তুসমূহ = বস্তুসমূহ[0][1:-1], tuple (পূর্ণসংখ্যা (x, 16) জন্য x মধ্যে বস্তুসমূহ[1:])
		c = পূর্ণসংখ্যা (fields[0], 16)

		যদি নয় shape মধ্যে ['initial', 'medial', 'isolated', 'final']:
			চালু

		যদি len (বস্তুসমূহ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			যদি বস্তুসমূহ[0] == 0x0020:
				বস্তুসমূহ = বস্তুসমূহ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			যদি c নয় মধ্যে LIGATURES:
				চালু

			# Save ligature
			names[c] = fields[1]
			যদি বস্তুসমূহ নয় মধ্যে ligatures:
				ligatures[বস্তুসমূহ] = {}
			ligatures[বস্তুসমূহ][shape] = c
		অন্যথা:
			# Save shape
			যদি বস্তুসমূহ[0] নয় মধ্যে names:
				names[বস্তুসমূহ[0]] = fields[1]
			অন্যথা:
				names[বস্তুসমূহ[0]] = os.path.commonprefix ([names[বস্তুসমূহ[0]], fields[1]]).strip ()
			যদি বস্তুসমূহ[0] নয় মধ্যে shapes:
				shapes[বস্তুসমূহ[0]] = {}
			shapes[বস্তুসমূহ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	জন্য চাবি_2 মধ্যে ligatures.চাবিগুলি ():
		জন্য shape মধ্যে ligatures[চাবি_2]:
			c = ligatures[চাবি_2][shape]
			যদি len(চাবি_2) == 3:
				যদি shape == 'isolated':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['final'])
				নতুবা_যদি shape == 'final':
					liga = (shapes[চাবি_2[0]]['medial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['final'])
				নতুবা_যদি shape == 'initial':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['medial'])
				অন্যথা:
					তোলো Exception ("Unexpected shape", shape)
				যদি liga[0] নয় মধ্যে ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			নতুবা_যদি len(চাবি_2) == 2:
				যদি shape is None:
					liga = চাবি_2
					যদি liga[0] নয় মধ্যে ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					চালু
				নতুবা_যদি shape == 'isolated':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['final'])
				নতুবা_যদি shape == 'final':
					liga = (shapes[চাবি_2[0]]['medial'], shapes[চাবি_2[1]]['final'])
				নতুবা_যদি shape == 'initial':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'])
				অন্যথা:
					তোলো Exception ("Unexpected shape", shape)
				যদি liga[0] নয় মধ্যে ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			অন্যথা:
				তোলো Exception ("Unexpected number of ligature components", চাবি_2)

	ফেরত shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	চাবিগুলি = shapes.চাবিগুলি ()
	min_u, max_u = min (চাবিগুলি), max (চাবিগুলি)
	জন্য u মধ্যে range (min_u, max_u + 1):
		s = [shapes[u][shape] যদি u মধ্যে shapes এবং shape মধ্যে shapes[u] অন্যথা 0
		     জন্য shape মধ্যে  ['initial', 'medial', 'final', 'isolated']]
		মান_2 = ', '.join ("0x%04Xu" % c জন্য c মধ্যে s)
		print ("  {%s}, /* U+%04X %s */" % (মান_2, u, names[u] যদি u মধ্যে names অন্যথা ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) জন্য l মধ্যে ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	জন্য first মধ্যে sorted (ligas_2.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		জন্য liga মধ্যে ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) জন্য l মধ্যে ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	জন্য first মধ্যে sorted (ligas_mark_2.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		জন্য liga মধ্যে ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) জন্য l মধ্যে ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	জন্য first মধ্যে sorted (ligas_3.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		জন্য liga মধ্যে ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.চাবিগুলি ()), max (shapes.চাবিগুলি ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	জন্য u মধ্যে range (min_u, max_u + 1):
		s = [shapes[u][shape] যদি u মধ্যে shapes এবং shape মধ্যে shapes[u] অন্যথা 0
		     জন্য shape মধ্যে ['initial', 'medial', 'final', 'isolated']]
		যদি any (s):
			মান_2 = ", ".join ("0x%04X" % c জন্য c মধ্যে s)
			print ("    (0x%04X, [%s])," % (u, মান_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	জন্য first মধ্যে sorted (ligas_3.চাবিগুলি ()):
		জন্য liga মধ্যে ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	জন্য first মধ্যে sorted (ligas_2.চাবিগুলি ()):
		জন্য liga মধ্যে ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	জন্য first মধ্যে sorted (ligas_mark_2.চাবিগুলি ()):
		জন্য liga মধ্যে ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " যদি language.name == "rust" অন্যথা ""))
print (" *")
print (" * on files with these headers:")
print (" *")
জন্য h মধ্যে headers:
	জন্য l মধ্যে h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
যদি language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
নতুবা_যদি language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
অন্যথা:
	প্রতিজ্ঞা মিথ্যা, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
