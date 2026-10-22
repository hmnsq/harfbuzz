#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ഇറക്കുമതി os.path, sys

ഇറക്കുമതി packTab

എങ്കിൽ len(sys.argv) > 1 ഒപ്പം sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
അല്ലെങ്കിൽ:
	language = packTab.languages["c"]

എങ്കിൽ len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') വേണ്ടി x ഇൽ sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
ആകുമ്പോൾ files[0].readline ().find ('##################') < 0:
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
	വൈശ്വിക blocks
	വേണ്ടി വരി ഇൽ f:

		j = വരി.find ('#')
		എങ്കിൽ j >= 0:
			വരി = വരി[:j]

		fields = [x.strip () വേണ്ടി x ഇൽ വരി.split (';')]
		എങ്കിൽ len (fields) == 1:
			തുടരുക

		uu = fields[0].split ('..')
		start = പൂർണ്ണസംഖ്യ (uu[0], 16)
		എങ്കിൽ len (uu) == 1:
			end = start
		അല്ലെങ്കിൽ:
			end = പൂർണ്ണസംഖ്യ (uu[1], 16)

		t = fields[1]

		വേണ്ടി u ഇൽ range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	മൂല്യങ്ങൾ = {}
	വേണ്ടി വരി ഇൽ f:

		എങ്കിൽ വരി[0] == '#':
			തുടരുക

		fields = [x.strip () വേണ്ടി x ഇൽ വരി.split (';')]
		എങ്കിൽ len (fields) == 1:
			തുടരുക

		u = പൂർണ്ണസംഖ്യ (fields[0], 16)

		എങ്കിൽ fields[3] ഇൽ ["ALAPH", "DALATH RISH"]:
			മൂല്യം_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		അല്ലെങ്കിൽ:
			മൂല്യം_2 = "JOINING_TYPE_" + fields[2]
		മൂല്യങ്ങൾ[u] = മൂല്യം_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] വേണ്ടി u, v ഇൽ മൂല്യങ്ങൾ.ഇനങ്ങൾ ()}
	sol = packTab.pack_table (data, സ്ഥിരസ്ഥിതി=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	എങ്കിൽ language.name == "c":
		sol.genCode (code, 'joining_type', language=language, സ്വകാര്യ=സത്യം)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, സ്വകാര്യ=സത്യം)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, സ്വകാര്യ=അസത്യം)

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
		code.print_code (file=sys.stdout, language=language, സ്വകാര്യ=അസത്യം)
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
	അല്ലെങ്കിൽ:
		പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name

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
	വരികൾ = f.readlines()
	വരികൾ += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	വേണ്ടി വരി ഇൽ വരികൾ:

		fields = [x.strip () വേണ്ടി x ഇൽ വരി.split (';')]
		എങ്കിൽ fields[5][0:1] != '<':
			തുടരുക

		ഇനങ്ങൾ = fields[5].split (' ')
		shape, ഇനങ്ങൾ = ഇനങ്ങൾ[0][1:-1], tuple (പൂർണ്ണസംഖ്യ (x, 16) വേണ്ടി x ഇൽ ഇനങ്ങൾ[1:])
		c = പൂർണ്ണസംഖ്യ (fields[0], 16)

		എങ്കിൽ അല്ല shape ഇൽ ['initial', 'medial', 'isolated', 'final']:
			തുടരുക

		എങ്കിൽ len (ഇനങ്ങൾ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			എങ്കിൽ ഇനങ്ങൾ[0] == 0x0020:
				ഇനങ്ങൾ = ഇനങ്ങൾ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			എങ്കിൽ c അല്ല ഇൽ LIGATURES:
				തുടരുക

			# Save ligature
			names[c] = fields[1]
			എങ്കിൽ ഇനങ്ങൾ അല്ല ഇൽ ligatures:
				ligatures[ഇനങ്ങൾ] = {}
			ligatures[ഇനങ്ങൾ][shape] = c
		അല്ലെങ്കിൽ:
			# Save shape
			എങ്കിൽ ഇനങ്ങൾ[0] അല്ല ഇൽ names:
				names[ഇനങ്ങൾ[0]] = fields[1]
			അല്ലെങ്കിൽ:
				names[ഇനങ്ങൾ[0]] = os.path.commonprefix ([names[ഇനങ്ങൾ[0]], fields[1]]).strip ()
			എങ്കിൽ ഇനങ്ങൾ[0] അല്ല ഇൽ shapes:
				shapes[ഇനങ്ങൾ[0]] = {}
			shapes[ഇനങ്ങൾ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	വേണ്ടി താക്കോൽ_2 ഇൽ ligatures.താക്കോലുകൾ ():
		വേണ്ടി shape ഇൽ ligatures[താക്കോൽ_2]:
			c = ligatures[താക്കോൽ_2][shape]
			എങ്കിൽ len(താക്കോൽ_2) == 3:
				എങ്കിൽ shape == 'isolated':
					liga = (shapes[താക്കോൽ_2[0]]['initial'], shapes[താക്കോൽ_2[1]]['medial'], shapes[താക്കോൽ_2[2]]['final'])
				അല്ലെങ്കിലെങ്കിൽ shape == 'final':
					liga = (shapes[താക്കോൽ_2[0]]['medial'], shapes[താക്കോൽ_2[1]]['medial'], shapes[താക്കോൽ_2[2]]['final'])
				അല്ലെങ്കിലെങ്കിൽ shape == 'initial':
					liga = (shapes[താക്കോൽ_2[0]]['initial'], shapes[താക്കോൽ_2[1]]['medial'], shapes[താക്കോൽ_2[2]]['medial'])
				അല്ലെങ്കിൽ:
					ഉയർത്തുക Exception ("Unexpected shape", shape)
				എങ്കിൽ liga[0] അല്ല ഇൽ ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			അല്ലെങ്കിലെങ്കിൽ len(താക്കോൽ_2) == 2:
				എങ്കിൽ shape is None:
					liga = താക്കോൽ_2
					എങ്കിൽ liga[0] അല്ല ഇൽ ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					തുടരുക
				അല്ലെങ്കിലെങ്കിൽ shape == 'isolated':
					liga = (shapes[താക്കോൽ_2[0]]['initial'], shapes[താക്കോൽ_2[1]]['final'])
				അല്ലെങ്കിലെങ്കിൽ shape == 'final':
					liga = (shapes[താക്കോൽ_2[0]]['medial'], shapes[താക്കോൽ_2[1]]['final'])
				അല്ലെങ്കിലെങ്കിൽ shape == 'initial':
					liga = (shapes[താക്കോൽ_2[0]]['initial'], shapes[താക്കോൽ_2[1]]['medial'])
				അല്ലെങ്കിൽ:
					ഉയർത്തുക Exception ("Unexpected shape", shape)
				എങ്കിൽ liga[0] അല്ല ഇൽ ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			അല്ലെങ്കിൽ:
				ഉയർത്തുക Exception ("Unexpected number of ligature components", താക്കോൽ_2)

	തിരികെ shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	താക്കോലുകൾ = shapes.താക്കോലുകൾ ()
	min_u, max_u = min (താക്കോലുകൾ), max (താക്കോലുകൾ)
	വേണ്ടി u ഇൽ range (min_u, max_u + 1):
		s = [shapes[u][shape] എങ്കിൽ u ഇൽ shapes ഒപ്പം shape ഇൽ shapes[u] അല്ലെങ്കിൽ 0
		     വേണ്ടി shape ഇൽ  ['initial', 'medial', 'final', 'isolated']]
		മൂല്യം_2 = ', '.join ("0x%04Xu" % c വേണ്ടി c ഇൽ s)
		print ("  {%s}, /* U+%04X %s */" % (മൂല്യം_2, u, names[u] എങ്കിൽ u ഇൽ names അല്ലെങ്കിൽ ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) വേണ്ടി l ഇൽ ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	വേണ്ടി first ഇൽ sorted (ligas_2.താക്കോലുകൾ ()):

		print ("  { 0x%04Xu, {" % (first))
		വേണ്ടി liga ഇൽ ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) വേണ്ടി l ഇൽ ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	വേണ്ടി first ഇൽ sorted (ligas_mark_2.താക്കോലുകൾ ()):

		print ("  { 0x%04Xu, {" % (first))
		വേണ്ടി liga ഇൽ ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) വേണ്ടി l ഇൽ ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	വേണ്ടി first ഇൽ sorted (ligas_3.താക്കോലുകൾ ()):

		print ("  { 0x%04Xu, {" % (first))
		വേണ്ടി liga ഇൽ ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.താക്കോലുകൾ ()), max (shapes.താക്കോലുകൾ ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	വേണ്ടി u ഇൽ range (min_u, max_u + 1):
		s = [shapes[u][shape] എങ്കിൽ u ഇൽ shapes ഒപ്പം shape ഇൽ shapes[u] അല്ലെങ്കിൽ 0
		     വേണ്ടി shape ഇൽ ['initial', 'medial', 'final', 'isolated']]
		എങ്കിൽ any (s):
			മൂല്യം_2 = ", ".join ("0x%04X" % c വേണ്ടി c ഇൽ s)
			print ("    (0x%04X, [%s])," % (u, മൂല്യം_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	വേണ്ടി first ഇൽ sorted (ligas_3.താക്കോലുകൾ ()):
		വേണ്ടി liga ഇൽ ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	വേണ്ടി first ഇൽ sorted (ligas_2.താക്കോലുകൾ ()):
		വേണ്ടി liga ഇൽ ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	വേണ്ടി first ഇൽ sorted (ligas_mark_2.താക്കോലുകൾ ()):
		വേണ്ടി liga ഇൽ ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " എങ്കിൽ language.name == "rust" അല്ലെങ്കിൽ ""))
print (" *")
print (" * on files with these headers:")
print (" *")
വേണ്ടി h ഇൽ headers:
	വേണ്ടി l ഇൽ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
എങ്കിൽ language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
അല്ലെങ്കിലെങ്കിൽ language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
അല്ലെങ്കിൽ:
	പ്രതിജ്ഞാ അസത്യം, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
