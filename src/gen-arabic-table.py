#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

আনযন os.path, sys

আনযন packTab

যদি len(sys.argv) > 1 আৰু sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
অন্যথা:
	language = packTab.languages["c"]

যদি len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') বাবে x অন্তঃ sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
যেতিয়ালৈ files[0].readline ().find ('##################') < 0:
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
	ৱৈশ্ৱিক blocks
	বাবে সাৰি অন্তঃ f:

		j = সাৰি.find ('#')
		যদি j >= 0:
			সাৰি = সাৰি[:j]

		fields = [x.strip () বাবে x অন্তঃ সাৰি.split (';')]
		যদি len (fields) == 1:
			অনুৱর্ত

		uu = fields[0].split ('..')
		start = পূর্ণাঙ্ক (uu[0], 16)
		যদি len (uu) == 1:
			end = start
		অন্যথা:
			end = পূর্ণাঙ্ক (uu[1], 16)

		t = fields[1]

		বাবে u অন্তঃ range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	মানসমূহ = {}
	বাবে সাৰি অন্তঃ f:

		যদি সাৰি[0] == '#':
			অনুৱর্ত

		fields = [x.strip () বাবে x অন্তঃ সাৰি.split (';')]
		যদি len (fields) == 1:
			অনুৱর্ত

		u = পূর্ণাঙ্ক (fields[0], 16)

		যদি fields[3] অন্তঃ ["ALAPH", "DALATH RISH"]:
			মান_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		অন্যথা:
			মান_2 = "JOINING_TYPE_" + fields[2]
		মানসমূহ[u] = মান_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] বাবে u, v অন্তঃ মানসমূহ.বস্তুসমূহ ()}
	sol = packTab.pack_table (data, স্ৱভাৱ=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	যদি language.name == "c":
		sol.genCode (code, 'joining_type', language=language, গুপ্ত=সঁচা)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, গুপ্ত=সঁচা)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	নোচেদ্যদি language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, গুপ্ত=মিছা)

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
		code.print_code (file=sys.stdout, language=language, গুপ্ত=মিছা)
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
		প্রতিজ্ঞা মিছা, "Unknown language: %s" % language.name

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
	সাৰিগুলি = f.readlines()
	সাৰিগুলি += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	বাবে সাৰি অন্তঃ সাৰিগুলি:

		fields = [x.strip () বাবে x অন্তঃ সাৰি.split (';')]
		যদি fields[5][0:1] != '<':
			অনুৱর্ত

		বস্তুসমূহ = fields[5].split (' ')
		shape, বস্তুসমূহ = বস্তুসমূহ[0][1:-1], tuple (পূর্ণাঙ্ক (x, 16) বাবে x অন্তঃ বস্তুসমূহ[1:])
		c = পূর্ণাঙ্ক (fields[0], 16)

		যদি নহয় shape অন্তঃ ['initial', 'medial', 'isolated', 'final']:
			অনুৱর্ত

		যদি len (বস্তুসমূহ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			যদি বস্তুসমূহ[0] == 0x0020:
				বস্তুসমূহ = বস্তুসমূহ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			যদি c নহয় অন্তঃ LIGATURES:
				অনুৱর্ত

			# Save ligature
			names[c] = fields[1]
			যদি বস্তুসমূহ নহয় অন্তঃ ligatures:
				ligatures[বস্তুসমূহ] = {}
			ligatures[বস্তুসমূহ][shape] = c
		অন্যথা:
			# Save shape
			যদি বস্তুসমূহ[0] নহয় অন্তঃ names:
				names[বস্তুসমূহ[0]] = fields[1]
			অন্যথা:
				names[বস্তুসমূহ[0]] = os.path.commonprefix ([names[বস্তুসমূহ[0]], fields[1]]).strip ()
			যদি বস্তুসমূহ[0] নহয় অন্তঃ shapes:
				shapes[বস্তুসমূহ[0]] = {}
			shapes[বস্তুসমূহ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	বাবে চাবি_2 অন্তঃ ligatures.চাবিগুলি ():
		বাবে shape অন্তঃ ligatures[চাবি_2]:
			c = ligatures[চাবি_2][shape]
			যদি len(চাবি_2) == 3:
				যদি shape == 'isolated':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['final'])
				নোচেদ্যদি shape == 'final':
					liga = (shapes[চাবি_2[0]]['medial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['final'])
				নোচেদ্যদি shape == 'initial':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'], shapes[চাবি_2[2]]['medial'])
				অন্যথা:
					উত্থাপয Exception ("Unexpected shape", shape)
				যদি liga[0] নহয় অন্তঃ ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			নোচেদ্যদি len(চাবি_2) == 2:
				যদি shape is None:
					liga = চাবি_2
					যদি liga[0] নহয় অন্তঃ ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					অনুৱর্ত
				নোচেদ্যদি shape == 'isolated':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['final'])
				নোচেদ্যদি shape == 'final':
					liga = (shapes[চাবি_2[0]]['medial'], shapes[চাবি_2[1]]['final'])
				নোচেদ্যদি shape == 'initial':
					liga = (shapes[চাবি_2[0]]['initial'], shapes[চাবি_2[1]]['medial'])
				অন্যথা:
					উত্থাপয Exception ("Unexpected shape", shape)
				যদি liga[0] নহয় অন্তঃ ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			অন্যথা:
				উত্থাপয Exception ("Unexpected number of ligature components", চাবি_2)

	ঘূৰাই shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	চাবিগুলি = shapes.চাবিগুলি ()
	min_u, max_u = min (চাবিগুলি), max (চাবিগুলি)
	বাবে u অন্তঃ range (min_u, max_u + 1):
		s = [shapes[u][shape] যদি u অন্তঃ shapes আৰু shape অন্তঃ shapes[u] অন্যথা 0
		     বাবে shape অন্তঃ  ['initial', 'medial', 'final', 'isolated']]
		মান_2 = ', '.join ("0x%04Xu" % c বাবে c অন্তঃ s)
		print ("  {%s}, /* U+%04X %s */" % (মান_2, u, names[u] যদি u অন্তঃ names অন্যথা ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) বাবে l অন্তঃ ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	বাবে first অন্তঃ sorted (ligas_2.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		বাবে liga অন্তঃ ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) বাবে l অন্তঃ ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	বাবে first অন্তঃ sorted (ligas_mark_2.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		বাবে liga অন্তঃ ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) বাবে l অন্তঃ ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	বাবে first অন্তঃ sorted (ligas_3.চাবিগুলি ()):

		print ("  { 0x%04Xu, {" % (first))
		বাবে liga অন্তঃ ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.চাবিগুলি ()), max (shapes.চাবিগুলি ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	বাবে u অন্তঃ range (min_u, max_u + 1):
		s = [shapes[u][shape] যদি u অন্তঃ shapes আৰু shape অন্তঃ shapes[u] অন্যথা 0
		     বাবে shape অন্তঃ ['initial', 'medial', 'final', 'isolated']]
		যদি any (s):
			মান_2 = ", ".join ("0x%04X" % c বাবে c অন্তঃ s)
			print ("    (0x%04X, [%s])," % (u, মান_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	বাবে first অন্তঃ sorted (ligas_3.চাবিগুলি ()):
		বাবে liga অন্তঃ ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	বাবে first অন্তঃ sorted (ligas_2.চাবিগুলি ()):
		বাবে liga অন্তঃ ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	বাবে first অন্তঃ sorted (ligas_mark_2.চাবিগুলি ()):
		বাবে liga অন্তঃ ligas_mark_2[first]:
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
বাবে h অন্তঃ headers:
	বাবে l অন্তঃ h:
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
নোচেদ্যদি language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
অন্যথা:
	প্রতিজ্ঞা মিছা, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
