#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

आयात os.path, sys

आयात packTab

अगर len(sys.argv) > 1 and sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
अन्यथा:
	language = packTab.languages["c"]

अगर len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') क्रम x in sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
जबतक files[0].readline ().find ('##################') < 0:
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
	global blocks
	क्रम पंक्ति in f:

		j = पंक्ति.find ('#')
		अगर j >= 0:
			पंक्ति = पंक्ति[:j]

		fields = [x.strip () क्रम x in पंक्ति.split (';')]
		अगर len (fields) == 1:
			जारी

		uu = fields[0].split ('..')
		शुरू = पूर्णांक (uu[0], 16)
		अगर len (uu) == 1:
			end = शुरू
		अन्यथा:
			end = पूर्णांक (uu[1], 16)

		t = fields[1]

		क्रम u in range (शुरू, end + 1):
			blocks[u] = t

def print_joining_table(f):

	मूल्यावली = {}
	क्रम पंक्ति in f:

		अगर पंक्ति[0] == '#':
			जारी

		fields = [x.strip () क्रम x in पंक्ति.split (';')]
		अगर len (fields) == 1:
			जारी

		u = पूर्णांक (fields[0], 16)

		अगर fields[3] in ["ALAPH", "DALATH RISH"]:
			मूल्य_2 = "JOINING_GROUP_" + fields[3].प्रतिस्थापन(' ', '_')
		अन्यथा:
			मूल्य_2 = "JOINING_TYPE_" + fields[2]
		मूल्यावली[u] = मूल्य_2

	code = packTab.Code ('_hb_arabic_joining')
	तथ्य = {u: JOINING_CODE[v] क्रम u, v in मूल्यावली.वस्तुएँ ()}
	sol = packTab.pack_table (तथ्य, शेष=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	अगर language.name == "c":
		sol.genCode (code, 'joining_type', language=language, निजी=True)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, निजी=True)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	elif language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, निजी=False)

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
		code.print_code (file=sys.stdout, language=language, निजी=False)
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
	अन्यथा:
		assert False, "Unknown language: %s" % language.name

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
	पंक्तियाँ = f.readlines()
	पंक्तियाँ += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	क्रम पंक्ति in पंक्तियाँ:

		fields = [x.strip () क्रम x in पंक्ति.split (';')]
		अगर fields[5][0:1] != '<':
			जारी

		वस्तुएँ = fields[5].split (' ')
		shape, वस्तुएँ = वस्तुएँ[0][1:-1], tuple (पूर्णांक (x, 16) क्रम x in वस्तुएँ[1:])
		c = पूर्णांक (fields[0], 16)

		अगर not shape in ['initial', 'medial', 'isolated', 'final']:
			जारी

		अगर len (वस्तुएँ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			अगर वस्तुएँ[0] == 0x0020:
				वस्तुएँ = वस्तुएँ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			अगर c not in LIGATURES:
				जारी

			# Save ligature
			names[c] = fields[1]
			अगर वस्तुएँ not in ligatures:
				ligatures[वस्तुएँ] = {}
			ligatures[वस्तुएँ][shape] = c
		अन्यथा:
			# Save shape
			अगर वस्तुएँ[0] not in names:
				names[वस्तुएँ[0]] = fields[1]
			अन्यथा:
				names[वस्तुएँ[0]] = os.path.commonprefix ([names[वस्तुएँ[0]], fields[1]]).strip ()
			अगर वस्तुएँ[0] not in shapes:
				shapes[वस्तुएँ[0]] = {}
			shapes[वस्तुएँ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	क्रम कुंजी_2 in ligatures.कुंजियाँ ():
		क्रम shape in ligatures[कुंजी_2]:
			c = ligatures[कुंजी_2][shape]
			अगर len(कुंजी_2) == 3:
				अगर shape == 'isolated':
					liga = (shapes[कुंजी_2[0]]['initial'], shapes[कुंजी_2[1]]['medial'], shapes[कुंजी_2[2]]['final'])
				elif shape == 'final':
					liga = (shapes[कुंजी_2[0]]['medial'], shapes[कुंजी_2[1]]['medial'], shapes[कुंजी_2[2]]['final'])
				elif shape == 'initial':
					liga = (shapes[कुंजी_2[0]]['initial'], shapes[कुंजी_2[1]]['medial'], shapes[कुंजी_2[2]]['medial'])
				अन्यथा:
					raise Exception ("Unexpected shape", shape)
				अगर liga[0] not in ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			elif len(कुंजी_2) == 2:
				अगर shape is None:
					liga = कुंजी_2
					अगर liga[0] not in ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					जारी
				elif shape == 'isolated':
					liga = (shapes[कुंजी_2[0]]['initial'], shapes[कुंजी_2[1]]['final'])
				elif shape == 'final':
					liga = (shapes[कुंजी_2[0]]['medial'], shapes[कुंजी_2[1]]['final'])
				elif shape == 'initial':
					liga = (shapes[कुंजी_2[0]]['initial'], shapes[कुंजी_2[1]]['medial'])
				अन्यथा:
					raise Exception ("Unexpected shape", shape)
				अगर liga[0] not in ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			अन्यथा:
				raise Exception ("Unexpected number of ligature components", कुंजी_2)

	वापस shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	कुंजियाँ = shapes.कुंजियाँ ()
	min_u, max_u = min (कुंजियाँ), max (कुंजियाँ)
	क्रम u in range (min_u, max_u + 1):
		s = [shapes[u][shape] अगर u in shapes and shape in shapes[u] अन्यथा 0
		     क्रम shape in  ['initial', 'medial', 'final', 'isolated']]
		मूल्य_2 = ', '.मेल ("0x%04Xu" % c क्रम c in s)
		print ("  {%s}, /* U+%04X %s */" % (मूल्य_2, u, names[u] अगर u in names अन्यथा ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) क्रम l in ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	क्रम first in sorted (ligas_2.कुंजियाँ ()):

		print ("  { 0x%04Xu, {" % (first))
		क्रम liga in ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) क्रम l in ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	क्रम first in sorted (ligas_mark_2.कुंजियाँ ()):

		print ("  { 0x%04Xu, {" % (first))
		क्रम liga in ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) क्रम l in ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	क्रम first in sorted (ligas_3.कुंजियाँ ()):

		print ("  { 0x%04Xu, {" % (first))
		क्रम liga in ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.कुंजियाँ ()), max (shapes.कुंजियाँ ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	क्रम u in range (min_u, max_u + 1):
		s = [shapes[u][shape] अगर u in shapes and shape in shapes[u] अन्यथा 0
		     क्रम shape in ['initial', 'medial', 'final', 'isolated']]
		अगर any (s):
			मूल्य_2 = ", ".मेल ("0x%04X" % c क्रम c in s)
			print ("    (0x%04X, [%s])," % (u, मूल्य_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	क्रम first in sorted (ligas_3.कुंजियाँ ()):
		क्रम liga in ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	क्रम first in sorted (ligas_2.कुंजियाँ ()):
		क्रम liga in ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	क्रम first in sorted (ligas_mark_2.कुंजियाँ ()):
		क्रम liga in ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " अगर language.name == "rust" अन्यथा ""))
print (" *")
print (" * on files with these headers:")
print (" *")
क्रम h in headers:
	क्रम l in h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
अगर language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
elif language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
अन्यथा:
	assert False, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
