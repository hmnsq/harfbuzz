#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

import os.path, sys

import packTab

གལ་ཏེ len(sys.argv) > 1 ca sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
དེ་མིན:
	language = packTab.languages["c"]

གལ་ཏེ len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') ཆེད་དུ x anta_h sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
སྐབས files[0].readline ().find ('##################') < 0:
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
	ཆེད་དུ line anta_h f:

		j = line.find ('#')
		གལ་ཏེ j >= 0:
			line = line[:j]

		fields = [x.strip () ཆེད་དུ x anta_h line.split (';')]
		གལ་ཏེ len (fields) == 1:
			continue

		uu = fields[0].split ('..')
		start = ཧྲིལ (uu[0], 16)
		གལ་ཏེ len (uu) == 1:
			end = start
		དེ་མིན:
			end = ཧྲིལ (uu[1], 16)

		t = fields[1]

		ཆེད་དུ u anta_h range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	values = {}
	ཆེད་དུ line anta_h f:

		གལ་ཏེ line[0] == '#':
			continue

		fields = [x.strip () ཆེད་དུ x anta_h line.split (';')]
		གལ་ཏེ len (fields) == 1:
			continue

		u = ཧྲིལ (fields[0], 16)

		གལ་ཏེ fields[3] anta_h ["ALAPH", "DALATH RISH"]:
			value = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		དེ་མིན:
			value = "JOINING_TYPE_" + fields[2]
		values[u] = value

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] ཆེད་དུ u, v anta_h values.items ()}
	sol = packTab.pack_table (data, default=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	གལ་ཏེ language.name == "c":
		sol.genCode (code, 'joining_type', language=language, private=True)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, private=True)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	elif language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, private=False)

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
		code.print_code (file=sys.stdout, language=language, private=False)
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
	དེ་མིན:
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
	lines = f.readlines()
	lines += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	ཆེད་དུ line anta_h lines:

		fields = [x.strip () ཆེད་དུ x anta_h line.split (';')]
		གལ་ཏེ fields[5][0:1] != '<':
			continue

		items = fields[5].split (' ')
		shape, items = items[0][1:-1], tuple (ཧྲིལ (x, 16) ཆེད་དུ x anta_h items[1:])
		c = ཧྲིལ (fields[0], 16)

		གལ་ཏེ na shape anta_h ['initial', 'medial', 'isolated', 'final']:
			continue

		གལ་ཏེ len (items) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			གལ་ཏེ items[0] == 0x0020:
				items = items[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			གལ་ཏེ c na anta_h LIGATURES:
				continue

			# Save ligature
			names[c] = fields[1]
			གལ་ཏེ items na anta_h ligatures:
				ligatures[items] = {}
			ligatures[items][shape] = c
		དེ་མིན:
			# Save shape
			གལ་ཏེ items[0] na anta_h names:
				names[items[0]] = fields[1]
			དེ་མིན:
				names[items[0]] = os.path.commonprefix ([names[items[0]], fields[1]]).strip ()
			གལ་ཏེ items[0] na anta_h shapes:
				shapes[items[0]] = {}
			shapes[items[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	ཆེད་དུ key anta_h ligatures.keys ():
		ཆེད་དུ shape anta_h ligatures[key]:
			c = ligatures[key][shape]
			གལ་ཏེ len(key) == 3:
				གལ་ཏེ shape == 'isolated':
					liga = (shapes[key[0]]['initial'], shapes[key[1]]['medial'], shapes[key[2]]['final'])
				elif shape == 'final':
					liga = (shapes[key[0]]['medial'], shapes[key[1]]['medial'], shapes[key[2]]['final'])
				elif shape == 'initial':
					liga = (shapes[key[0]]['initial'], shapes[key[1]]['medial'], shapes[key[2]]['medial'])
				དེ་མིན:
					raise Exception ("Unexpected shape", shape)
				གལ་ཏེ liga[0] na anta_h ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			elif len(key) == 2:
				གལ་ཏེ shape is None:
					liga = key
					གལ་ཏེ liga[0] na anta_h ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					continue
				elif shape == 'isolated':
					liga = (shapes[key[0]]['initial'], shapes[key[1]]['final'])
				elif shape == 'final':
					liga = (shapes[key[0]]['medial'], shapes[key[1]]['final'])
				elif shape == 'initial':
					liga = (shapes[key[0]]['initial'], shapes[key[1]]['medial'])
				དེ་མིན:
					raise Exception ("Unexpected shape", shape)
				གལ་ཏེ liga[0] na anta_h ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			དེ་མིན:
				raise Exception ("Unexpected number of ligature components", key)

	ཕྱིར shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	keys = shapes.keys ()
	min_u, max_u = min (keys), max (keys)
	ཆེད་དུ u anta_h range (min_u, max_u + 1):
		s = [shapes[u][shape] གལ་ཏེ u anta_h shapes ca shape anta_h shapes[u] དེ་མིན 0
		     ཆེད་དུ shape anta_h  ['initial', 'medial', 'final', 'isolated']]
		value = ', '.join ("0x%04Xu" % c ཆེད་དུ c anta_h s)
		print ("  {%s}, /* U+%04X %s */" % (value, u, names[u] གལ་ཏེ u anta_h names དེ་མིན ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) ཆེད་དུ l anta_h ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	ཆེད་དུ first anta_h sorted (ligas_2.keys ()):

		print ("  { 0x%04Xu, {" % (first))
		ཆེད་དུ liga anta_h ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) ཆེད་དུ l anta_h ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	ཆེད་དུ first anta_h sorted (ligas_mark_2.keys ()):

		print ("  { 0x%04Xu, {" % (first))
		ཆེད་དུ liga anta_h ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) ཆེད་དུ l anta_h ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	ཆེད་དུ first anta_h sorted (ligas_3.keys ()):

		print ("  { 0x%04Xu, {" % (first))
		ཆེད་དུ liga anta_h ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.keys ()), max (shapes.keys ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	ཆེད་དུ u anta_h range (min_u, max_u + 1):
		s = [shapes[u][shape] གལ་ཏེ u anta_h shapes ca shape anta_h shapes[u] དེ་མིན 0
		     ཆེད་དུ shape anta_h ['initial', 'medial', 'final', 'isolated']]
		གལ་ཏེ any (s):
			value = ", ".join ("0x%04X" % c ཆེད་དུ c anta_h s)
			print ("    (0x%04X, [%s])," % (u, value))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	ཆེད་དུ first anta_h sorted (ligas_3.keys ()):
		ཆེད་དུ liga anta_h ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	ཆེད་དུ first anta_h sorted (ligas_2.keys ()):
		ཆེད་དུ liga anta_h ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	ཆེད་དུ first anta_h sorted (ligas_mark_2.keys ()):
		ཆེད་དུ liga anta_h ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " གལ་ཏེ language.name == "rust" དེ་མིན ""))
print (" *")
print (" * on files with these headers:")
print (" *")
ཆེད་དུ h anta_h headers:
	ཆེད་དུ l anta_h h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
གལ་ཏེ language.name == "c":
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
དེ་མིན:
	assert False, "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
