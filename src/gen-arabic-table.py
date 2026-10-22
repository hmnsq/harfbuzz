#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

आनयन os.path, sys

आनयन packTab

जइ len(sys.argv) > 1 य sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
अण्णहा:
	language = packTab.languages["c"]

जइ len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') प्रति x अन्तः sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
जाव files[0].readline ().find ('##################') < 0:
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
	वैश्विक blocks
	प्रति पङ्क्तिः अन्तः f:

		j = पङ्क्तिः.find ('#')
		जइ j >= 0:
			पङ्क्तिः = पङ्क्तिः[:j]

		fields = [x.strip () प्रति x अन्तः पङ्क्तिः.split (';')]
		जइ len (fields) == 1:
			अनुवर्त

		uu = fields[0].split ('..')
		start = पूर्णाङ्क (uu[0], 16)
		जइ len (uu) == 1:
			end = start
		अण्णहा:
			end = पूर्णाङ्क (uu[1], 16)

		t = fields[1]

		प्रति u अन्तः range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	मानानि = {}
	प्रति पङ्क्तिः अन्तः f:

		जइ पङ्क्तिः[0] == '#':
			अनुवर्त

		fields = [x.strip () प्रति x अन्तः पङ्क्तिः.split (';')]
		जइ len (fields) == 1:
			अनुवर्त

		u = पूर्णाङ्क (fields[0], 16)

		जइ fields[3] अन्तः ["ALAPH", "DALATH RISH"]:
			मानम्_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		अण्णहा:
			मानम्_2 = "JOINING_TYPE_" + fields[2]
		मानानि[u] = मानम्_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] प्रति u, v अन्तः मानानि.वस्तूनि ()}
	sol = packTab.pack_table (data, स्वभाव=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	जइ language.name == "c":
		sol.genCode (code, 'joining_type', language=language, गुप्त=सच्च)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, गुप्त=सच्च)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	नोचेद्यदि language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, गुप्त=मिच्छ)

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
		code.print_code (file=sys.stdout, language=language, गुप्त=मिच्छ)
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
	अण्णहा:
		प्रतिज्ञा मिच्छ, "Unknown language: %s" % language.name

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
	पङ्क्तयः = f.readlines()
	पङ्क्तयः += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	प्रति पङ्क्तिः अन्तः पङ्क्तयः:

		fields = [x.strip () प्रति x अन्तः पङ्क्तिः.split (';')]
		जइ fields[5][0:1] != '<':
			अनुवर्त

		वस्तूनि = fields[5].split (' ')
		shape, वस्तूनि = वस्तूनि[0][1:-1], tuple (पूर्णाङ्क (x, 16) प्रति x अन्तः वस्तूनि[1:])
		c = पूर्णाङ्क (fields[0], 16)

		जइ ण shape अन्तः ['initial', 'medial', 'isolated', 'final']:
			अनुवर्त

		जइ len (वस्तूनि) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			जइ वस्तूनि[0] == 0x0020:
				वस्तूनि = वस्तूनि[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			जइ c ण अन्तः LIGATURES:
				अनुवर्त

			# Save ligature
			names[c] = fields[1]
			जइ वस्तूनि ण अन्तः ligatures:
				ligatures[वस्तूनि] = {}
			ligatures[वस्तूनि][shape] = c
		अण्णहा:
			# Save shape
			जइ वस्तूनि[0] ण अन्तः names:
				names[वस्तूनि[0]] = fields[1]
			अण्णहा:
				names[वस्तूनि[0]] = os.path.commonprefix ([names[वस्तूनि[0]], fields[1]]).strip ()
			जइ वस्तूनि[0] ण अन्तः shapes:
				shapes[वस्तूनि[0]] = {}
			shapes[वस्तूनि[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	प्रति कुञ्चिका_2 अन्तः ligatures.कुञ्चिकाः ():
		प्रति shape अन्तः ligatures[कुञ्चिका_2]:
			c = ligatures[कुञ्चिका_2][shape]
			जइ len(कुञ्चिका_2) == 3:
				जइ shape == 'isolated':
					liga = (shapes[कुञ्चिका_2[0]]['initial'], shapes[कुञ्चिका_2[1]]['medial'], shapes[कुञ्चिका_2[2]]['final'])
				नोचेद्यदि shape == 'final':
					liga = (shapes[कुञ्चिका_2[0]]['medial'], shapes[कुञ्चिका_2[1]]['medial'], shapes[कुञ्चिका_2[2]]['final'])
				नोचेद्यदि shape == 'initial':
					liga = (shapes[कुञ्चिका_2[0]]['initial'], shapes[कुञ्चिका_2[1]]['medial'], shapes[कुञ्चिका_2[2]]['medial'])
				अण्णहा:
					उत्थापय Exception ("Unexpected shape", shape)
				जइ liga[0] ण अन्तः ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			नोचेद्यदि len(कुञ्चिका_2) == 2:
				जइ shape is None:
					liga = कुञ्चिका_2
					जइ liga[0] ण अन्तः ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					अनुवर्त
				नोचेद्यदि shape == 'isolated':
					liga = (shapes[कुञ्चिका_2[0]]['initial'], shapes[कुञ्चिका_2[1]]['final'])
				नोचेद्यदि shape == 'final':
					liga = (shapes[कुञ्चिका_2[0]]['medial'], shapes[कुञ्चिका_2[1]]['final'])
				नोचेद्यदि shape == 'initial':
					liga = (shapes[कुञ्चिका_2[0]]['initial'], shapes[कुञ्चिका_2[1]]['medial'])
				अण्णहा:
					उत्थापय Exception ("Unexpected shape", shape)
				जइ liga[0] ण अन्तः ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			अण्णहा:
				उत्थापय Exception ("Unexpected number of ligature components", कुञ्चिका_2)

	प्रत्यागम shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	कुञ्चिकाः = shapes.कुञ्चिकाः ()
	min_u, max_u = min (कुञ्चिकाः), max (कुञ्चिकाः)
	प्रति u अन्तः range (min_u, max_u + 1):
		s = [shapes[u][shape] जइ u अन्तः shapes य shape अन्तः shapes[u] अण्णहा 0
		     प्रति shape अन्तः  ['initial', 'medial', 'final', 'isolated']]
		मानम्_2 = ', '.join ("0x%04Xu" % c प्रति c अन्तः s)
		print ("  {%s}, /* U+%04X %s */" % (मानम्_2, u, names[u] जइ u अन्तः names अण्णहा ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) प्रति l अन्तः ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	प्रति first अन्तः sorted (ligas_2.कुञ्चिकाः ()):

		print ("  { 0x%04Xu, {" % (first))
		प्रति liga अन्तः ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) प्रति l अन्तः ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	प्रति first अन्तः sorted (ligas_mark_2.कुञ्चिकाः ()):

		print ("  { 0x%04Xu, {" % (first))
		प्रति liga अन्तः ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) प्रति l अन्तः ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	प्रति first अन्तः sorted (ligas_3.कुञ्चिकाः ()):

		print ("  { 0x%04Xu, {" % (first))
		प्रति liga अन्तः ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.कुञ्चिकाः ()), max (shapes.कुञ्चिकाः ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	प्रति u अन्तः range (min_u, max_u + 1):
		s = [shapes[u][shape] जइ u अन्तः shapes य shape अन्तः shapes[u] अण्णहा 0
		     प्रति shape अन्तः ['initial', 'medial', 'final', 'isolated']]
		जइ any (s):
			मानम्_2 = ", ".join ("0x%04X" % c प्रति c अन्तः s)
			print ("    (0x%04X, [%s])," % (u, मानम्_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	प्रति first अन्तः sorted (ligas_3.कुञ्चिकाः ()):
		प्रति liga अन्तः ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	प्रति first अन्तः sorted (ligas_2.कुञ्चिकाः ()):
		प्रति liga अन्तः ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	प्रति first अन्तः sorted (ligas_mark_2.कुञ्चिकाः ()):
		प्रति liga अन्तः ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " जइ language.name == "rust" अण्णहा ""))
print (" *")
print (" * on files with these headers:")
print (" *")
प्रति h अन्तः headers:
	प्रति l अन्तः h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
जइ language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
नोचेद्यदि language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
अण्णहा:
	प्रतिज्ञा मिच्छ, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
