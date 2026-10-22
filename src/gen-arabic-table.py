#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

ಆನಯನ os.path, sys

ಆನಯನ packTab

ಯದಿ len(sys.argv) > 1 ಚ sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
ಅನ್ಯಥಾ:
	language = packTab.languages["c"]

ಯದಿ len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') ಪ್ರತಿ x ಅನ್ತಃ sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
ಯಾವತ್ files[0].readline ().find ('##################') < 0:
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
	ವೈಶ್ವಿಕ blocks
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ f:

		j = ಸಾಲು.find ('#')
		ಯದಿ j >= 0:
			ಸಾಲು = ಸಾಲು[:j]

		fields = [x.strip () ಪ್ರತಿ x ಅನ್ತಃ ಸಾಲು.split (';')]
		ಯದಿ len (fields) == 1:
			ಅನುವರ್ತ

		uu = fields[0].split ('..')
		start = ಪೂರ್ಣಾಙ್ಕ (uu[0], 16)
		ಯದಿ len (uu) == 1:
			end = start
		ಅನ್ಯಥಾ:
			end = ಪೂರ್ಣಾಙ್ಕ (uu[1], 16)

		t = fields[1]

		ಪ್ರತಿ u ಅನ್ತಃ range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	ಮೌಲ್ಯಗಳು = {}
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ f:

		ಯದಿ ಸಾಲು[0] == '#':
			ಅನುವರ್ತ

		fields = [x.strip () ಪ್ರತಿ x ಅನ್ತಃ ಸಾಲು.split (';')]
		ಯದಿ len (fields) == 1:
			ಅನುವರ್ತ

		u = ಪೂರ್ಣಾಙ್ಕ (fields[0], 16)

		ಯದಿ fields[3] ಅನ್ತಃ ["ALAPH", "DALATH RISH"]:
			ಮೌಲ್ಯ_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		ಅನ್ಯಥಾ:
			ಮೌಲ್ಯ_2 = "JOINING_TYPE_" + fields[2]
		ಮೌಲ್ಯಗಳು[u] = ಮೌಲ್ಯ_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] ಪ್ರತಿ u, v ಅನ್ತಃ ಮೌಲ್ಯಗಳು.ವಸ್ತುಗಳು ()}
	sol = packTab.pack_table (data, ಸ್ವಭಾವ=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	ಯದಿ language.name == "c":
		sol.genCode (code, 'joining_type', language=language, ಗುಪ್ತ=ಸತ್ಯ)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, ಗುಪ್ತ=ಸತ್ಯ)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	ನೋಚೇದ್ಯದಿ language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, ಗುಪ್ತ=ಅಸತ್ಯ)

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
		code.print_code (file=sys.stdout, language=language, ಗುಪ್ತ=ಅಸತ್ಯ)
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
	ಅನ್ಯಥಾ:
		ಪ್ರತಿಜ್ಞಾ ಅಸತ್ಯ, "Unknown language: %s" % language.name

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
	ಸಾಲುಗಳು = f.readlines()
	ಸಾಲುಗಳು += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	ಪ್ರತಿ ಸಾಲು ಅನ್ತಃ ಸಾಲುಗಳು:

		fields = [x.strip () ಪ್ರತಿ x ಅನ್ತಃ ಸಾಲು.split (';')]
		ಯದಿ fields[5][0:1] != '<':
			ಅನುವರ್ತ

		ವಸ್ತುಗಳು = fields[5].split (' ')
		shape, ವಸ್ತುಗಳು = ವಸ್ತುಗಳು[0][1:-1], tuple (ಪೂರ್ಣಾಙ್ಕ (x, 16) ಪ್ರತಿ x ಅನ್ತಃ ವಸ್ತುಗಳು[1:])
		c = ಪೂರ್ಣಾಙ್ಕ (fields[0], 16)

		ಯದಿ ನ shape ಅನ್ತಃ ['initial', 'medial', 'isolated', 'final']:
			ಅನುವರ್ತ

		ಯದಿ len (ವಸ್ತುಗಳು) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			ಯದಿ ವಸ್ತುಗಳು[0] == 0x0020:
				ವಸ್ತುಗಳು = ವಸ್ತುಗಳು[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			ಯದಿ c ನ ಅನ್ತಃ LIGATURES:
				ಅನುವರ್ತ

			# Save ligature
			names[c] = fields[1]
			ಯದಿ ವಸ್ತುಗಳು ನ ಅನ್ತಃ ligatures:
				ligatures[ವಸ್ತುಗಳು] = {}
			ligatures[ವಸ್ತುಗಳು][shape] = c
		ಅನ್ಯಥಾ:
			# Save shape
			ಯದಿ ವಸ್ತುಗಳು[0] ನ ಅನ್ತಃ names:
				names[ವಸ್ತುಗಳು[0]] = fields[1]
			ಅನ್ಯಥಾ:
				names[ವಸ್ತುಗಳು[0]] = os.path.commonprefix ([names[ವಸ್ತುಗಳು[0]], fields[1]]).strip ()
			ಯದಿ ವಸ್ತುಗಳು[0] ನ ಅನ್ತಃ shapes:
				shapes[ವಸ್ತುಗಳು[0]] = {}
			shapes[ವಸ್ತುಗಳು[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	ಪ್ರತಿ ಕೀಲಿ_2 ಅನ್ತಃ ligatures.ಕೀಲಿಗಳು ():
		ಪ್ರತಿ shape ಅನ್ತಃ ligatures[ಕೀಲಿ_2]:
			c = ligatures[ಕೀಲಿ_2][shape]
			ಯದಿ len(ಕೀಲಿ_2) == 3:
				ಯದಿ shape == 'isolated':
					liga = (shapes[ಕೀಲಿ_2[0]]['initial'], shapes[ಕೀಲಿ_2[1]]['medial'], shapes[ಕೀಲಿ_2[2]]['final'])
				ನೋಚೇದ್ಯದಿ shape == 'final':
					liga = (shapes[ಕೀಲಿ_2[0]]['medial'], shapes[ಕೀಲಿ_2[1]]['medial'], shapes[ಕೀಲಿ_2[2]]['final'])
				ನೋಚೇದ್ಯದಿ shape == 'initial':
					liga = (shapes[ಕೀಲಿ_2[0]]['initial'], shapes[ಕೀಲಿ_2[1]]['medial'], shapes[ಕೀಲಿ_2[2]]['medial'])
				ಅನ್ಯಥಾ:
					ಉತ್ಥಾಪಯ Exception ("Unexpected shape", shape)
				ಯದಿ liga[0] ನ ಅನ್ತಃ ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			ನೋಚೇದ್ಯದಿ len(ಕೀಲಿ_2) == 2:
				ಯದಿ shape is None:
					liga = ಕೀಲಿ_2
					ಯದಿ liga[0] ನ ಅನ್ತಃ ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					ಅನುವರ್ತ
				ನೋಚೇದ್ಯದಿ shape == 'isolated':
					liga = (shapes[ಕೀಲಿ_2[0]]['initial'], shapes[ಕೀಲಿ_2[1]]['final'])
				ನೋಚೇದ್ಯದಿ shape == 'final':
					liga = (shapes[ಕೀಲಿ_2[0]]['medial'], shapes[ಕೀಲಿ_2[1]]['final'])
				ನೋಚೇದ್ಯದಿ shape == 'initial':
					liga = (shapes[ಕೀಲಿ_2[0]]['initial'], shapes[ಕೀಲಿ_2[1]]['medial'])
				ಅನ್ಯಥಾ:
					ಉತ್ಥಾಪಯ Exception ("Unexpected shape", shape)
				ಯದಿ liga[0] ನ ಅನ್ತಃ ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			ಅನ್ಯಥಾ:
				ಉತ್ಥಾಪಯ Exception ("Unexpected number of ligature components", ಕೀಲಿ_2)

	ಪ್ರತ್ಯಾಗಮ shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	ಕೀಲಿಗಳು = shapes.ಕೀಲಿಗಳು ()
	min_u, max_u = min (ಕೀಲಿಗಳು), max (ಕೀಲಿಗಳು)
	ಪ್ರತಿ u ಅನ್ತಃ range (min_u, max_u + 1):
		s = [shapes[u][shape] ಯದಿ u ಅನ್ತಃ shapes ಚ shape ಅನ್ತಃ shapes[u] ಅನ್ಯಥಾ 0
		     ಪ್ರತಿ shape ಅನ್ತಃ  ['initial', 'medial', 'final', 'isolated']]
		ಮೌಲ್ಯ_2 = ', '.join ("0x%04Xu" % c ಪ್ರತಿ c ಅನ್ತಃ s)
		print ("  {%s}, /* U+%04X %s */" % (ಮೌಲ್ಯ_2, u, names[u] ಯದಿ u ಅನ್ತಃ names ಅನ್ಯಥಾ ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) ಪ್ರತಿ l ಅನ್ತಃ ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_2.ಕೀಲಿಗಳು ()):

		print ("  { 0x%04Xu, {" % (first))
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) ಪ್ರತಿ l ಅನ್ತಃ ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_mark_2.ಕೀಲಿಗಳು ()):

		print ("  { 0x%04Xu, {" % (first))
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) ಪ್ರತಿ l ಅನ್ತಃ ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_3.ಕೀಲಿಗಳು ()):

		print ("  { 0x%04Xu, {" % (first))
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.ಕೀಲಿಗಳು ()), max (shapes.ಕೀಲಿಗಳು ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	ಪ್ರತಿ u ಅನ್ತಃ range (min_u, max_u + 1):
		s = [shapes[u][shape] ಯದಿ u ಅನ್ತಃ shapes ಚ shape ಅನ್ತಃ shapes[u] ಅನ್ಯಥಾ 0
		     ಪ್ರತಿ shape ಅನ್ತಃ ['initial', 'medial', 'final', 'isolated']]
		ಯದಿ any (s):
			ಮೌಲ್ಯ_2 = ", ".join ("0x%04X" % c ಪ್ರತಿ c ಅನ್ತಃ s)
			print ("    (0x%04X, [%s])," % (u, ಮೌಲ್ಯ_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_3.ಕೀಲಿಗಳು ()):
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_2.ಕೀಲಿಗಳು ()):
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	ಪ್ರತಿ first ಅನ್ತಃ sorted (ligas_mark_2.ಕೀಲಿಗಳು ()):
		ಪ್ರತಿ liga ಅನ್ತಃ ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " ಯದಿ language.name == "rust" ಅನ್ಯಥಾ ""))
print (" *")
print (" * on files with these headers:")
print (" *")
ಪ್ರತಿ h ಅನ್ತಃ headers:
	ಪ್ರತಿ l ಅನ್ತಃ h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
ಯದಿ language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
ನೋಚೇದ್ಯದಿ language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
ಅನ್ಯಥಾ:
	ಪ್ರತಿಜ್ಞಾ ಅಸತ್ಯ, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
