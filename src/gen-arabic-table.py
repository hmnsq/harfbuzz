#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

આયાત os.path, sys

આયાત packTab

જો len(sys.argv) > 1 અને sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
નહીંતર:
	language = packTab.languages["c"]

જો len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') માટે x માં sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
જ્યારે files[0].readline ().find ('##################') < 0:
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
	વૈશ્વિક blocks
	માટે લીટી માં f:

		j = લીટી.find ('#')
		જો j >= 0:
			લીટી = લીટી[:j]

		fields = [x.strip () માટે x માં લીટી.split (';')]
		જો len (fields) == 1:
			ચાલુ

		uu = fields[0].split ('..')
		start = પૂર્ણાંક (uu[0], 16)
		જો len (uu) == 1:
			end = start
		નહીંતર:
			end = પૂર્ણાંક (uu[1], 16)

		t = fields[1]

		માટે u માં range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	કિંમતો = {}
	માટે લીટી માં f:

		જો લીટી[0] == '#':
			ચાલુ

		fields = [x.strip () માટે x માં લીટી.split (';')]
		જો len (fields) == 1:
			ચાલુ

		u = પૂર્ણાંક (fields[0], 16)

		જો fields[3] માં ["ALAPH", "DALATH RISH"]:
			કિંમત_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		નહીંતર:
			કિંમત_2 = "JOINING_TYPE_" + fields[2]
		કિંમતો[u] = કિંમત_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] માટે u, v માં કિંમતો.વસ્તુઓ ()}
	sol = packTab.pack_table (data, મૂળભૂત=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	જો language.name == "c":
		sol.genCode (code, 'joining_type', language=language, ખાનગી=સાચું)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, ખાનગી=સાચું)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	અથવાજો language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, ખાનગી=ખોટું)

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
		code.print_code (file=sys.stdout, language=language, ખાનગી=ખોટું)
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
	નહીંતર:
		પ્રતિજ્ઞા ખોટું, "Unknown language: %s" % language.name

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
	લીટીઓ = f.readlines()
	લીટીઓ += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	માટે લીટી માં લીટીઓ:

		fields = [x.strip () માટે x માં લીટી.split (';')]
		જો fields[5][0:1] != '<':
			ચાલુ

		વસ્તુઓ = fields[5].split (' ')
		shape, વસ્તુઓ = વસ્તુઓ[0][1:-1], tuple (પૂર્ણાંક (x, 16) માટે x માં વસ્તુઓ[1:])
		c = પૂર્ણાંક (fields[0], 16)

		જો નહીં shape માં ['initial', 'medial', 'isolated', 'final']:
			ચાલુ

		જો len (વસ્તુઓ) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			જો વસ્તુઓ[0] == 0x0020:
				વસ્તુઓ = વસ્તુઓ[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			જો c નહીં માં LIGATURES:
				ચાલુ

			# Save ligature
			names[c] = fields[1]
			જો વસ્તુઓ નહીં માં ligatures:
				ligatures[વસ્તુઓ] = {}
			ligatures[વસ્તુઓ][shape] = c
		નહીંતર:
			# Save shape
			જો વસ્તુઓ[0] નહીં માં names:
				names[વસ્તુઓ[0]] = fields[1]
			નહીંતર:
				names[વસ્તુઓ[0]] = os.path.commonprefix ([names[વસ્તુઓ[0]], fields[1]]).strip ()
			જો વસ્તુઓ[0] નહીં માં shapes:
				shapes[વસ્તુઓ[0]] = {}
			shapes[વસ્તુઓ[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	માટે ચાવી_2 માં ligatures.ચાવીઓ ():
		માટે shape માં ligatures[ચાવી_2]:
			c = ligatures[ચાવી_2][shape]
			જો len(ચાવી_2) == 3:
				જો shape == 'isolated':
					liga = (shapes[ચાવી_2[0]]['initial'], shapes[ચાવી_2[1]]['medial'], shapes[ચાવી_2[2]]['final'])
				અથવાજો shape == 'final':
					liga = (shapes[ચાવી_2[0]]['medial'], shapes[ચાવી_2[1]]['medial'], shapes[ચાવી_2[2]]['final'])
				અથવાજો shape == 'initial':
					liga = (shapes[ચાવી_2[0]]['initial'], shapes[ચાવી_2[1]]['medial'], shapes[ચાવી_2[2]]['medial'])
				નહીંતર:
					ઉઠાવો Exception ("Unexpected shape", shape)
				જો liga[0] નહીં માં ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			અથવાજો len(ચાવી_2) == 2:
				જો shape is None:
					liga = ચાવી_2
					જો liga[0] નહીં માં ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					ચાલુ
				અથવાજો shape == 'isolated':
					liga = (shapes[ચાવી_2[0]]['initial'], shapes[ચાવી_2[1]]['final'])
				અથવાજો shape == 'final':
					liga = (shapes[ચાવી_2[0]]['medial'], shapes[ચાવી_2[1]]['final'])
				અથવાજો shape == 'initial':
					liga = (shapes[ચાવી_2[0]]['initial'], shapes[ચાવી_2[1]]['medial'])
				નહીંતર:
					ઉઠાવો Exception ("Unexpected shape", shape)
				જો liga[0] નહીં માં ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			નહીંતર:
				ઉઠાવો Exception ("Unexpected number of ligature components", ચાવી_2)

	પાછા shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	ચાવીઓ = shapes.ચાવીઓ ()
	min_u, max_u = min (ચાવીઓ), max (ચાવીઓ)
	માટે u માં range (min_u, max_u + 1):
		s = [shapes[u][shape] જો u માં shapes અને shape માં shapes[u] નહીંતર 0
		     માટે shape માં  ['initial', 'medial', 'final', 'isolated']]
		કિંમત_2 = ', '.join ("0x%04Xu" % c માટે c માં s)
		print ("  {%s}, /* U+%04X %s */" % (કિંમત_2, u, names[u] જો u માં names નહીંતર ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) માટે l માં ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	માટે first માં sorted (ligas_2.ચાવીઓ ()):

		print ("  { 0x%04Xu, {" % (first))
		માટે liga માં ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) માટે l માં ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	માટે first માં sorted (ligas_mark_2.ચાવીઓ ()):

		print ("  { 0x%04Xu, {" % (first))
		માટે liga માં ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) માટે l માં ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	માટે first માં sorted (ligas_3.ચાવીઓ ()):

		print ("  { 0x%04Xu, {" % (first))
		માટે liga માં ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.ચાવીઓ ()), max (shapes.ચાવીઓ ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	માટે u માં range (min_u, max_u + 1):
		s = [shapes[u][shape] જો u માં shapes અને shape માં shapes[u] નહીંતર 0
		     માટે shape માં ['initial', 'medial', 'final', 'isolated']]
		જો any (s):
			કિંમત_2 = ", ".join ("0x%04X" % c માટે c માં s)
			print ("    (0x%04X, [%s])," % (u, કિંમત_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	માટે first માં sorted (ligas_3.ચાવીઓ ()):
		માટે liga માં ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	માટે first માં sorted (ligas_2.ચાવીઓ ()):
		માટે liga માં ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	માટે first માં sorted (ligas_mark_2.ચાવીઓ ()):
		માટે liga માં ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " જો language.name == "rust" નહીંતર ""))
print (" *")
print (" * on files with these headers:")
print (" *")
માટે h માં headers:
	માટે l માં h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
જો language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
અથવાજો language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
નહીંતર:
	પ્રતિજ્ઞા ખોટું, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
