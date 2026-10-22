#!/usr/bin/env python3

"""usage: ./gen-arabic-table.py [--rust] ArabicShaping.txt UnicodeData.txt Blocks.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
"""

இறக்குமதி os.path, sys

இறக்குமதி packTab

எனில் len(sys.argv) > 1 மற்றும் sys.argv[1] == "--rust":
	del sys.argv[1]
	language = packTab.languages["rust"]
இல்லையெனில்:
	language = packTab.languages["c"]

எனில் len (sys.argv) != 4:
	sys.exit (__doc__)

files = [open (x, encoding='utf-8') க்கு x உள் sys.argv[1:]]

headers = [[files[0].readline (), files[0].readline ()], [files[2].readline (), files[2].readline ()]]
headers.append (["UnicodeData.txt does not have a header."])
போது files[0].readline ().find ('##################') < 0:
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
	வைஶ்விக blocks
	க்கு வரி உள் f:

		j = வரி.find ('#')
		எனில் j >= 0:
			வரி = வரி[:j]

		fields = [x.strip () க்கு x உள் வரி.split (';')]
		எனில் len (fields) == 1:
			தொடர்

		uu = fields[0].split ('..')
		start = முழு (uu[0], 16)
		எனில் len (uu) == 1:
			end = start
		இல்லையெனில்:
			end = முழு (uu[1], 16)

		t = fields[1]

		க்கு u உள் range (start, end + 1):
			blocks[u] = t

def print_joining_table(f):

	மதிப்புகள் = {}
	க்கு வரி உள் f:

		எனில் வரி[0] == '#':
			தொடர்

		fields = [x.strip () க்கு x உள் வரி.split (';')]
		எனில் len (fields) == 1:
			தொடர்

		u = முழு (fields[0], 16)

		எனில் fields[3] உள் ["ALAPH", "DALATH RISH"]:
			மதிப்பு_2 = "JOINING_GROUP_" + fields[3].replace(' ', '_')
		இல்லையெனில்:
			மதிப்பு_2 = "JOINING_TYPE_" + fields[2]
		மதிப்புகள்[u] = மதிப்பு_2

	code = packTab.Code ('_hb_arabic_joining')
	data = {u: JOINING_CODE[v] க்கு u, v உள் மதிப்புகள்.பொருட்கள் ()}
	sol = packTab.pack_table (data, இயல்பு=JOINING_CODE['JOINING_TYPE_X'], compression=9)
	எனில் language.name == "c":
		sol.genCode (code, 'joining_type', language=language, தனிப்பட்ட=உண்மை)

		print ("#include \"hb.hh\"")
		print ()
		code.print_code (file=sys.stdout, language=language, தனிப்பட்ட=உண்மை)
		print ()
		print ("static unsigned int")
		print ("joining_type (hb_codepoint_t u)")
		print ("{")
		print ("  return _hb_arabic_joining_joining_type (u);")
		print ("}")
		print ()
	அல்லதெனில் language.name == "rust":
		sol.genCode (code, 'joining_type_u8', language=language, தனிப்பட்ட=பொய்)

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
		code.print_code (file=sys.stdout, language=language, தனிப்பட்ட=பொய்)
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
	இல்லையெனில்:
		ப்ரதிஜ்ஞா பொய், "Unknown language: %s" % language.name

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
	வரிகள் = f.readlines()
	வரிகள் += [
		"F201;PUA ARABIC LIGATURE LELLAH ISOLATED FORM;Lo;0;AL;<isolated> 0644 0644 0647;;;;N;;;;;",
		"F211;PUA ARABIC LIGATURE LAM WITH MEEM WITH JEEM INITIAL FORM;Lo;0;AL;<initial> 0644 0645 062C;;;;N;;;;;",
		"F2EE;PUA ARABIC LIGATURE SHADDA WITH FATHATAN ISOLATED FORM;Lo;0;AL;<isolated> 0020 064B 0651;;;;N;;;;;",
	]
	க்கு வரி உள் வரிகள்:

		fields = [x.strip () க்கு x உள் வரி.split (';')]
		எனில் fields[5][0:1] != '<':
			தொடர்

		பொருட்கள் = fields[5].split (' ')
		shape, பொருட்கள் = பொருட்கள்[0][1:-1], tuple (முழு (x, 16) க்கு x உள் பொருட்கள்[1:])
		c = முழு (fields[0], 16)

		எனில் இல்லை shape உள் ['initial', 'medial', 'isolated', 'final']:
			தொடர்

		எனில் len (பொருட்கள்) != 1:
			# Mark ligatures start with space and are in visual order, so we
			# remove the space and reverse the items.
			எனில் பொருட்கள்[0] == 0x0020:
				பொருட்கள் = பொருட்கள்[:0:-1]
				shape = None
			# We only care about a subset of ligatures
			எனில் c இல்லை உள் LIGATURES:
				தொடர்

			# Save ligature
			names[c] = fields[1]
			எனில் பொருட்கள் இல்லை உள் ligatures:
				ligatures[பொருட்கள்] = {}
			ligatures[பொருட்கள்][shape] = c
		இல்லையெனில்:
			# Save shape
			எனில் பொருட்கள்[0] இல்லை உள் names:
				names[பொருட்கள்[0]] = fields[1]
			இல்லையெனில்:
				names[பொருட்கள்[0]] = os.path.commonprefix ([names[பொருட்கள்[0]], fields[1]]).strip ()
			எனில் பொருட்கள்[0] இல்லை உள் shapes:
				shapes[பொருட்கள்[0]] = {}
			shapes[பொருட்கள்[0]][shape] = c

	ligas_2 = {}
	ligas_3 = {}
	ligas_mark_2 = {}
	க்கு சாவி_2 உள் ligatures.சாவிகள் ():
		க்கு shape உள் ligatures[சாவி_2]:
			c = ligatures[சாவி_2][shape]
			எனில் len(சாவி_2) == 3:
				எனில் shape == 'isolated':
					liga = (shapes[சாவி_2[0]]['initial'], shapes[சாவி_2[1]]['medial'], shapes[சாவி_2[2]]['final'])
				அல்லதெனில் shape == 'final':
					liga = (shapes[சாவி_2[0]]['medial'], shapes[சாவி_2[1]]['medial'], shapes[சாவி_2[2]]['final'])
				அல்லதெனில் shape == 'initial':
					liga = (shapes[சாவி_2[0]]['initial'], shapes[சாவி_2[1]]['medial'], shapes[சாவி_2[2]]['medial'])
				இல்லையெனில்:
					எழுப்பு Exception ("Unexpected shape", shape)
				எனில் liga[0] இல்லை உள் ligas_3:
					ligas_3[liga[0]] = []
				ligas_3[liga[0]].append ((liga[1], liga[2], c))
			அல்லதெனில் len(சாவி_2) == 2:
				எனில் shape is None:
					liga = சாவி_2
					எனில் liga[0] இல்லை உள் ligas_mark_2:
						ligas_mark_2[liga[0]] = []
					ligas_mark_2[liga[0]].append ((liga[1], c))
					தொடர்
				அல்லதெனில் shape == 'isolated':
					liga = (shapes[சாவி_2[0]]['initial'], shapes[சாவி_2[1]]['final'])
				அல்லதெனில் shape == 'final':
					liga = (shapes[சாவி_2[0]]['medial'], shapes[சாவி_2[1]]['final'])
				அல்லதெனில் shape == 'initial':
					liga = (shapes[சாவி_2[0]]['initial'], shapes[சாவி_2[1]]['medial'])
				இல்லையெனில்:
					எழுப்பு Exception ("Unexpected shape", shape)
				எனில் liga[0] இல்லை உள் ligas_2:
					ligas_2[liga[0]] = []
				ligas_2[liga[0]].append ((liga[1], c))
			இல்லையெனில்:
				எழுப்பு Exception ("Unexpected number of ligature components", சாவி_2)

	திரும்பு shapes, names, ligas_2, ligas_mark_2, ligas_3


def print_shaping_table(f):

	shapes, names, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)

	print ()
	print ("static const uint16_t shaping_table[][4] =")
	print ("{")

	சாவிகள் = shapes.சாவிகள் ()
	min_u, max_u = min (சாவிகள்), max (சாவிகள்)
	க்கு u உள் range (min_u, max_u + 1):
		s = [shapes[u][shape] எனில் u உள் shapes மற்றும் shape உள் shapes[u] இல்லையெனில் 0
		     க்கு shape உள்  ['initial', 'medial', 'final', 'isolated']]
		மதிப்பு_2 = ', '.join ("0x%04Xu" % c க்கு c உள் s)
		print ("  {%s}, /* U+%04X %s */" % (மதிப்பு_2, u, names[u] எனில் u உள் names இல்லையெனில் ""))

	print ("};")
	print ()
	print ("#define SHAPING_TABLE_FIRST	0x%04Xu" % min_u)
	print ("#define SHAPING_TABLE_LAST	0x%04Xu" % max_u)
	print ()

	max_i = max (len (ligas_2[l]) க்கு l உள் ligas_2)
	print ()
	print ("static const struct ligature_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_table[] =")
	print ("{")
	க்கு first உள் sorted (ligas_2.சாவிகள் ()):

		print ("  { 0x%04Xu, {" % (first))
		க்கு liga உள் ligas_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_mark_2[l]) க்கு l உள் ligas_mark_2)
	print ()
	print ("static const struct ligature_mark_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_pairs_t {")
	print ("   uint16_t components[1];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_mark_table[] =")
	print ("{")
	க்கு first உள் sorted (ligas_mark_2.சாவிகள் ()):

		print ("  { 0x%04Xu, {" % (first))
		க்கு liga உள் ligas_mark_2[first]:
			print ("    { {0x%04Xu}, 0x%04Xu }, /* %s */" % (liga[0], liga[1], names[liga[1]]))
		print ("  }},")

	print ("};")
	print ()

	max_i = max (len (ligas_3[l]) க்கு l உள் ligas_3)
	print ()
	print ("static const struct ligature_3_set_t {")
	print (" uint16_t first;")
	print (" struct ligature_triplets_t {")
	print ("   uint16_t components[2];")
	print ("   uint16_t ligature;")
	print (" } ligatures[%d];" % max_i)
	print ("} ligature_3_table[] =")
	print ("{")
	க்கு first உள் sorted (ligas_3.சாவிகள் ()):

		print ("  { 0x%04Xu, {" % (first))
		க்கு liga உள் ligas_3[first]:
			print ("    { {0x%04Xu, 0x%04Xu}, 0x%04Xu}, /* %s */" % (liga[0], liga[1], liga[2], names[liga[2]]))
		print ("  }},")

	print ("};")
	print ()


def print_shaping_table_rust(f):

	shapes, _, ligas_2, ligas_mark_2, ligas_3 = read_shaping_table (f)
	min_u, max_u = min (shapes.சாவிகள் ()), max (shapes.சாவிகள் ())

	print ()
	print ("pub(crate) const SHAPING_TABLE: &[(u16, [u16; 4])] = &[")
	க்கு u உள் range (min_u, max_u + 1):
		s = [shapes[u][shape] எனில் u உள் shapes மற்றும் shape உள் shapes[u] இல்லையெனில் 0
		     க்கு shape உள் ['initial', 'medial', 'final', 'isolated']]
		எனில் any (s):
			மதிப்பு_2 = ", ".join ("0x%04X" % c க்கு c உள் s)
			print ("    (0x%04X, [%s])," % (u, மதிப்பு_2))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_3_TABLE: &[([u16; 3], u16)] = &[")
	க்கு first உள் sorted (ligas_3.சாவிகள் ()):
		க்கு liga உள் ligas_3[first]:
			print ("    ([0x%04X, 0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1], liga[2]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_TABLE: &[([u16; 2], u16)] = &[")
	க்கு first உள் sorted (ligas_2.சாவிகள் ()):
		க்கு liga உள் ligas_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")

	print ()
	print ("pub(crate) const LIGATURE_MARK_TABLE: &[([u16; 2], u16)] = &[")
	க்கு first உள் sorted (ligas_mark_2.சாவிகள் ()):
		க்கு liga உள் ligas_mark_2[first]:
			print ("    ([0x%04X, 0x%04X], 0x%04X)," %
			       (first, liga[0], liga[1]))
	print ("];")



print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   ./gen-arabic-table.py %sArabicShaping.txt UnicodeData.txt Blocks.txt" %
		("--rust " எனில் language.name == "rust" இல்லையெனில் ""))
print (" *")
print (" * on files with these headers:")
print (" *")
க்கு h உள் headers:
	க்கு l உள் h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
read_blocks (files[2])
எனில் language.name == "c":
	print ("#ifndef HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ("#define HB_OT_SHAPER_ARABIC_TABLE_HH")
	print ()

	print_joining_table (files[0])
	print_shaping_table (files[1])

	print ()
	print ("#endif /* HB_OT_SHAPER_ARABIC_TABLE_HH */")
அல்லதெனில் language.name == "rust":
	print_joining_table (files[0])
	print_shaping_table_rust (files[1])
இல்லையெனில்:
	ப்ரதிஜ்ஞா பொய், "Unknown language: %s" % language.name
print ()
print ("/* == End of generated table == */")
