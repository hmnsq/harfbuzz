#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

आयात os.path, sys

आयात packTab

जर len(sys.argv) > 1 आणि sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
अन्यथा:
	language = packTab.languages["c"]

जर len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') साठी x मध्ये sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
पर्यंत files[0].readline ().find ('##################') < 0:
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
	साठी ओळ मध्ये f:

		j = ओळ.find ('#')
		जर j >= 0:
			ओळ = ओळ[:j]

		fields = [x.strip () साठी x मध्ये ओळ.split (';')]
		जर len (fields) == 1:
			सुरूठेवा

		uu = fields[0].split ('..')
		start = पूर्णांक (uu[0], 16)
		जर len (uu) == 1:
			end = start
		अन्यथा:
			end = पूर्णांक (uu[1], 16)

		t = fields[1]

		साठी u मध्ये range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	मूल्ये = {}
	साठी ओळ मध्ये f:

		जर ओळ[0] == '#':
			सुरूठेवा

		fields = [x.strip () साठी x मध्ये ओळ.split (';')]
		जर len (fields) == 1:
			सुरूठेवा

		u = पूर्णांक (fields[0], 16)

		जर fields[3] मध्ये ["ALAPH", "DALATH RISH"]:
			मूल्य_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		अन्यथा:
			मूल्य_2 = "JOINING_TYPE_" + fields[2]
		मूल्ये[u] = मूल्य_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] साठी u, v मध्ये मूल्ये.वस्तुसंच ()}
	sol = packTab.pack_table (data, पूर्वनिर्धारित=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	जर language.name == "c":
		sol.genCode (code, 'joining_type', language=language, खाजगी=सत्य)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, खाजगी=सत्य)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	किंवाजर language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, खाजगी=असत्य)

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
		code.print_code (file=sys.stdout, language=language, खाजगी=असत्य)
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
		प्रतिज्ञा असत्य, "Unknown language: %s" % language.name

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
	ओळी = f.readlines()
	ओळी += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	साठी ओळ मध्ये ओळी:

		fields = [x.strip () साठी x मध्ये ओळ.split (';')]
		जर fields[5][0:1] != '<':
			सुरूठेवा

		वस्तुसंच = fields[5].split (' ')
		shape, वस्तुसंच = वस्तुसंच[0][1:-1], tuple (पूर्णांक (x, 16) साठी x मध्ये वस्तुसंच[1:])
		c = पूर्णांक (fields[0], 16)

		जर नाही shape मध्ये ['initial', 'medial', 'isolated', 'final']:
			सुरूठेवा

		जर len (वस्तुसंच) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			जर वस्तुसंच[0] == 0x0020:
				वस्तुसंच = वस्तुसंच[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			जर c नाही मध्ये LIGATURES:
				सुरूठेवा

			# Save ligature
			names[c] = fields[1]
			जर वस्तुसंच नाही मध्ये ligatures:
				ligatures[वस्तुसंच] = {}
			ligatures[वस्तुसंच][shape] = c
		अन्यथा:
			# Save shape
			जर वस्तुसंच[0] नाही मध्ये names:
				names[वस्तुसंच[0]] = fields[1]
			अन्यथा:
				names[वस्तुसंच[0]] = os.path.commonprefix ([names[वस्तुसंच[0]], fields[1]]).strip ()
			जर वस्तुसंच[0] नाही मध्ये shapes:
				shapes[वस्तुसंच[0]] = {}
			shapes[वस्तुसंच[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	साठी किल्ली_2 मध्ये ligatures.किल्ल्या ():
		साठी shape मध्ये ligatures[किल्ली_2]:
			c = ligatures[किल्ली_2][shape]
			जर len(किल्ली_2) == 3:
				जर shape == 'isolated':
					liga = (shapes[किल्ली_2[0]]['initial'], shapes[किल्ली_2[1]]['medial'], shapes[किल्ली_2[2]]['final'])
				किंवाजर shape == 'final':
					liga = (shapes[किल्ली_2[0]]['medial'], shapes[किल्ली_2[1]]['medial'], shapes[किल्ली_2[2]]['final'])
				किंवाजर shape == 'initial':
					liga = (shapes[किल्ली_2[0]]['initial'], shapes[किल्ली_2[1]]['medial'], shapes[किल्ली_2[2]]['medial'])
				अन्यथा:
					उठवा Exception ("Unexpected shape", shape)
				जर liga[0] नाही मध्ये ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			किंवाजर len(किल्ली_2) == 2:
				जर shape is None:
					liga = किल्ली_2
					जर liga[0] नाही मध्ये ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					सुरूठेवा
				किंवाजर shape == 'isolated':
					liga = (shapes[किल्ली_2[0]]['initial'], shapes[किल्ली_2[1]]['final'])
				किंवाजर shape == 'final':
					liga = (shapes[किल्ली_2[0]]['medial'], shapes[किल्ली_2[1]]['final'])
				किंवाजर shape == 'initial':
					liga = (shapes[किल्ली_2[0]]['initial'], shapes[किल्ली_2[1]]['medial'])
				अन्यथा:
					उठवा Exception ("Unexpected shape", shape)
				जर liga[0] नाही मध्ये ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			अन्यथा:
				उठवा Exception ("Unexpected number of ligature components", किल्ली_2)

	परत shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	किल्ल्या = shapes.किल्ल्या ()
	min_u, max_u = min (किल्ल्या), max (किल्ल्या)
	साठी u मध्ये range (min_u, max_u + 1):
		s = [shapes[u][shape] जर u मध्ये shapes आणि shape मध्ये shapes[u] अन्यथा 0
		     साठी shape मध्ये  ['initial', 'medial', 'final', 'isolated']]
		मूल्य_2 = ', '.join ("0x%04Xu" % c साठी c मध्ये s)
		print ("  {%s}, /* U+%04X %s */" % (मूल्य_2, u, names[u] जर u मध्ये names अन्यथा ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) साठी l मध्ये ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	साठी first मध्ये sorted (ligas_2.किल्ल्या ()):

		print ("  { 0x%04Xu, {" % (first))
		साठी liga मध्ये ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) साठी l मध्ये ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	साठी first मध्ये sorted (ligas_mark_2.किल्ल्या ()):

		print ("  { 0x%04Xu, {" % (first))
		साठी liga मध्ये ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) साठी l मध्ये ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	साठी first मध्ये sorted (ligas_3.किल्ल्या ()):

		print ("  { 0x%04Xu, {" % (first))
		साठी liga मध्ये ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.किल्ल्या ()), max (shapes.किल्ल्या ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	साठी u मध्ये range (min_u, max_u + 1):
		s = [shapes[u][shape] जर u मध्ये shapes आणि shape मध्ये shapes[u] अन्यथा 0
		     साठी shape मध्ये ['initial', 'medial', 'final', 'isolated']]
		जर any (s):
			मूल्य_2 = ", ".join ("0x%04X" % c साठी c मध्ये s)
			print ("    (0x%04X, [%s])," % (u, मूल्य_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	साठी first मध्ये sorted (ligas_3.किल्ल्या ()):
		साठी liga मध्ये ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	साठी first मध्ये sorted (ligas_2.किल्ल्या ()):
		साठी liga मध्ये ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	साठी first मध्ये sorted (ligas_mark_2.किल्ल्या ()):
		साठी liga मध्ये ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " जर language.name == "rust" अन्यथा ""))
print (" *")
print (" * on files with these headers:")
print (" *")
साठी h मध्ये headers:
	साठी l मध्ये h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
जर language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
किंवाजर language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
अन्यथा:
	प्रतिज्ञा असत्य, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
