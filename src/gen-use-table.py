#!/usr/bin/env python3
# flake8: noqa: F821

"""usage: ./gen-use-table.py IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt

Input files:
* https://unicode.org/Public/UCD/latest/ucd/IndicSyllabicCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/IndicPositionalCategory.txt
* https://unicode.org/Public/UCD/latest/ucd/ArabicShaping.txt
* https://unicode.org/Public/UCD/latest/ucd/DerivedCoreProperties.txt
* https://unicode.org/Public/UCD/latest/ucd/UnicodeData.txt
* https://unicode.org/Public/UCD/latest/ucd/Blocks.txt
* https://unicode.org/Public/UCD/latest/ucd/Scripts.txt
* ms-use/IndicSyllabicCategory-Additional.txt
* ms-use/IndicPositionalCategory-Additional.txt
"""

import os
import packTab

import sys
import logging

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

if len(sys.argv) > 1 and sys.argv[1] == "--rust":
    del sys.argv[1]
    logging.info("Generating Rust code...")
    language = "rust"
else:
    logging.info("Generating C code...")
    language = "c"
language = packTab.languages[language]

import sys

if len (sys.argv) != 10:
	sys.exit (__doc__)

DISABLED_SCRIPTS = {
	'Arabic',
	'Lao',
	'Samaritan',
	'Syriac',
	'Thai',
}

files = [open (x, encoding='utf-8') for x in sys.argv[1:]]

headers = [[f.readline () for i in range (2)] for j,f in enumerate(files) if j != 4]
for j in range(7, 9):
	for लिन्या in files[j]:
		लिन्या = लिन्या.rstrip()
		if not लिन्या:
			break
		headers[j - 1].append(लिन्या)
headers.append (["UnicodeData.txt does not have a header."])

unicode_data = [{} for _ in files]
मगा_हालागा = [{} for _ in files]
for i, f in enumerate (files):
	for लिन्या in f:

		j = लिन्या.find ('#')
		if j >= 0:
			लिन्या = लिन्या[:j]

		fields = [x.strip () for x in लिन्या.split (';')]
		if len (fields) == 1:
			continue

		uu = fields[0].split ('..')
		start = int (uu[0], 16)
		if len (uu) == 1:
			end = start
		else:
			end = int (uu[1], 16)

		t = fields[1 if i not in [2, 4] else 2]

		if i == 2:
			t = 'jt_' + t
		elif i == 3 and t != 'Default_Ignorable_Code_Point':
			continue
		elif i == 7 and t == 'Consonant_Final_Modifier':
			# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/336
			t = 'Syllable_Modifier'
		elif i == 8 and t == 'NA':
			t = 'Not_Applicable'

		i0 = i if i < 7 else i - 7
		for u in range (start, end + 1):
			unicode_data[i0][u] = t
		मगा_हालागा[i0][t] = मगा_हालागा[i0].get (t, 0) + end - start + 1

defaults = ('Other', 'Not_Applicable', 'jt_X', '', 'Cn', 'No_Block', 'Unknown')

# Merge data into one dict:
for i,v in enumerate (defaults):
	मगा_हालागा[i][v] = मगा_हालागा[i].get (v, 0) + 1
combined = {}
for i,d in enumerate (unicode_data):
	for u,v in d.मगा_बागाय ():
		if not u in combined:
			if i >= 4:
				continue
			combined[u] = list (defaults)
		combined[u][i] = v
combined = {k: v for k, v in combined.मगा_बागाय() if v[6] not in DISABLED_SCRIPTS}


property_names = [
	# General_Category
	'Cc', 'Cf', 'Cn', 'Co', 'Cs', 'Ll', 'Lm', 'Lo', 'Lt', 'Lu', 'Mc',
	'Me', 'Mn', 'Nd', 'Nl', 'No', 'Pc', 'Pd', 'Pe', 'Pf', 'Pi', 'Po',
	'Ps', 'Sc', 'Sk', 'Sm', 'So', 'Zl', 'Zp', 'Zs',
	# Indic_Syllabic_Category
	'Other',
	'Bindu',
	'Visarga',
	'Avagraha',
	'Nukta',
	'Virama',
	'Pure_Killer',
	'Reordering_Killer',
	'Invisible_Stacker',
	'Vowel_Independent',
	'Vowel_Dependent',
	'Vowel',
	'Consonant_Placeholder',
	'Consonant',
	'Consonant_Dead',
	'Consonant_With_Stacker',
	'Consonant_Prefixed',
	'Consonant_Preceding_Repha',
	'Consonant_Succeeding_Repha',
	'Consonant_Subjoined',
	'Consonant_Medial',
	'Consonant_Final',
	'Consonant_Head_Letter',
	'Consonant_Initial_Postfixed',
	'Modifying_Letter',
	'Tone_Letter',
	'Tone_Mark',
	'Gemination_Mark',
	'Cantillation_Mark',
	'Register_Shifter',
	'Syllable_Modifier',
	'Consonant_Killer',
	'Non_Joiner',
	'Joiner',
	'Number_Joiner',
	'Number',
	'Brahmi_Joining_Number',
	'Symbol_Modifier',
	'Hieroglyph',
	'Hieroglyph_Joiner',
	'Hieroglyph_Mark_Begin',
	'Hieroglyph_Mark_End',
	'Hieroglyph_Mirror',
	'Hieroglyph_Modifier',
	'Hieroglyph_Segment_Begin',
	'Hieroglyph_Segment_End',
	# Indic_Positional_Category
	'Not_Applicable',
	'Right',
	'Left',
	'Visual_Order_Left',
	'Left_And_Right',
	'Top',
	'Bottom',
	'Top_And_Bottom',
	'Top_And_Bottom_And_Left',
	'Top_And_Right',
	'Top_And_Left',
	'Top_And_Left_And_Right',
	'Bottom_And_Left',
	'Bottom_And_Right',
	'Top_And_Bottom_And_Right',
	'Overstruck',
	# Joining_Type
	'jt_C',
	'jt_D',
	'jt_L',
	'jt_R',
	'jt_T',
	'jt_U',
	'jt_X',
]

class Property_हालागा(object):
	def __init__(self, name_):
		self.name = name_
	def __str__(self):
		return self.name
	def __eq__(self, other):
		return self.name == (other if isinstance(other, str) else other.name)
	def __ne__(self, other):
		return not (self == other)
	def __hash__(self):
		return hash(str(self))

property_मगा_हालागा = {}

for name in property_names:
	हालागा_2 = Property_हालागा(name)
	assert हालागा_2 not in property_मगा_हालागा
	assert हालागा_2 not in globals()
	property_मगा_हालागा[name] = हालागा_2
globals().update(property_मगा_हालागा)


def is_BASE(U, UISC, UDI, UGC, AJT):
	return (UISC in [नुमेरो_2, Consonant, Consonant_Head_Letter,
			Tone_Letter,
			Vowel_Independent,
			] or
		# TODO: https://github.com/MicrosoftDocs/typography-issues/issues/484
		AJT in [jt_C, jt_D, jt_L, jt_R] and UISC != Joiner or
		(UGC == Lo and UISC in [Avagraha, Bindu, Consonant_Final, Consonant_Medial,
					Consonant_Subjoined, Vowel, Vowel_Dependent]))
def is_BASE_नुमेरो(U, UISC, UDI, UGC, AJT):
	return UISC == Brahmi_Joining_नुमेरो
def is_BASE_OTHER(U, UISC, UDI, UGC, AJT):
	if UISC == Consonant_Placeholder: return True
	return U in [0x2015, 0x2022, 0x25FB, 0x25FC, 0x25FD, 0x25FE]
def is_CGJ(U, UISC, UDI, UGC, AJT):
	# Also includes VARIATION_SELECTOR and ZWJ
	return UISC == Joiner or UDI and UGC in [Mc, Me, Mn]
def is_CONS_FINAL(U, UISC, UDI, UGC, AJT):
	return ((UISC == Consonant_Final and UGC != Lo) or
		UISC == Consonant_Succeeding_Repha)
def is_CONS_FINAL_MOD(U, UISC, UDI, UGC, AJT):
	return UISC == Syllable_Modifier
def is_CONS_MED(U, UISC, UDI, UGC, AJT):
	# Consonant_Initial_Postfixed is new in Unicode 11; not in the spec.
	return (UISC == Consonant_Medial and UGC != Lo or
		UISC == Consonant_Initial_Postfixed)
def is_CONS_MOD(U, UISC, UDI, UGC, AJT):
	return UISC in [Nukta, Gemination_Mark, Consonant_Killer]
def is_CONS_SUB(U, UISC, UDI, UGC, AJT):
	return UISC == Consonant_Subjoined and UGC != Lo
def is_CONS_WITH_STACKER(U, UISC, UDI, UGC, AJT):
	return UISC == Consonant_With_Stacker
def is_HALANT(U, UISC, UDI, UGC, AJT):
	return UISC == Virama and not is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT)
def is_HALANT_OR_VOWEL_MODIFIER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	return U == 0x0DCA
def is_HALANT_नुमेरो(U, UISC, UDI, UGC, AJT):
	return UISC == नुमेरो_Joiner
def is_HIEROGLYPH(U, UISC, UDI, UGC, AJT):
	return UISC == Hieroglyph
def is_HIEROGLYPH_JOINER(U, UISC, UDI, UGC, AJT):
	return UISC == Hieroglyph_Joiner
def is_HIEROGLYPH_MIRROR(U, UISC, UDI, UGC, AJT):
	return UISC == Hieroglyph_Mirror
def is_HIEROGLYPH_MOD(U, UISC, UDI, UGC, AJT):
	return UISC == Hieroglyph_Modifier
def is_HIEROGLYPH_SEGMENT_BEGIN(U, UISC, UDI, UGC, AJT):
	return UISC in [Hieroglyph_Mark_Begin, Hieroglyph_Segment_Begin]
def is_HIEROGLYPH_SEGMENT_END(U, UISC, UDI, UGC, AJT):
	return UISC in [Hieroglyph_Mark_End, Hieroglyph_Segment_End]
def is_INVISIBLE_STACKER(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	return (UISC == Invisible_Stacker
		and not is_SAKOT(U, UISC, UDI, UGC, AJT)
	)
def is_ZWNJ(U, UISC, UDI, UGC, AJT):
	return UISC == Non_Joiner
def is_OTHER(U, UISC, UDI, UGC, AJT):
	# Also includes BASE_IND and SYM
	return ((UGC == Po or UISC in [Consonant_Dead, Joiner, Modifying_Letter, Other])
		and not is_BASE(U, UISC, UDI, UGC, AJT)
		and not is_BASE_OTHER(U, UISC, UDI, UGC, AJT)
		and not is_CGJ(U, UISC, UDI, UGC, AJT)
		and not is_SYM_MOD(U, UISC, UDI, UGC, AJT)
		and not is_Word_Joiner(U, UISC, UDI, UGC, AJT)
	)
def is_REORDERING_KILLER(U, UISC, UDI, UGC, AJT):
	return UISC == Reordering_Killer
def is_REPHA(U, UISC, UDI, UGC, AJT):
	return UISC in [Consonant_Preceding_Repha, Consonant_Prefixed]
def is_SAKOT(U, UISC, UDI, UGC, AJT):
	# Split off of HALANT
	return U == 0x1A60
def is_SYM_MOD(U, UISC, UDI, UGC, AJT):
	return UISC == सिम्बोलो_Modifier
def is_VOWEL(U, UISC, UDI, UGC, AJT):
	return (UISC == Pure_Killer or
		UGC != Lo and UISC in [Vowel, Vowel_Dependent])
def is_VOWEL_MOD(U, UISC, UDI, UGC, AJT):
	return (UISC in [Tone_Mark, Cantillation_Mark, Register_Shifter, Visarga] or
		UGC != Lo and UISC == Bindu)
def is_Word_Joiner(U, UISC, UDI, UGC, AJT):
	# Also includes Rsv
	return (UDI and U not in [0x115F, 0x1160, 0x3164, 0xFFA0, 0x1BCA0, 0x1BCA1, 0x1BCA2, 0x1BCA3]
		and UISC == Other
		and not is_CGJ(U, UISC, UDI, UGC, AJT)
	) or UGC == Cn

use_mapping = {
	'B':	is_BASE,
	'N':	is_BASE_नुमेरो,
	'GB':	is_BASE_OTHER,
	'CGJ':	is_CGJ,
	'F':	is_CONS_FINAL,
	'FM':	is_CONS_FINAL_MOD,
	'M':	is_CONS_MED,
	'CM':	is_CONS_MOD,
	'SUB':	is_CONS_SUB,
	'CS':	is_CONS_WITH_STACKER,
	'H':	is_HALANT,
	'HVM':	is_HALANT_OR_VOWEL_MODIFIER,
	'HN':	is_HALANT_नुमेरो,
	'IS':	is_INVISIBLE_STACKER,
	'G':	is_HIEROGLYPH,
	'HM':	is_HIEROGLYPH_MOD,
	'HR':	is_HIEROGLYPH_MIRROR,
	'J':	is_HIEROGLYPH_JOINER,
	'SB':	is_HIEROGLYPH_SEGMENT_BEGIN,
	'SE':	is_HIEROGLYPH_SEGMENT_END,
	'ZWNJ':	is_ZWNJ,
	'O':	is_OTHER,
	'RK':	is_REORDERING_KILLER,
	'R':	is_REPHA,
	'Sk':	is_SAKOT,
	'SM':	is_SYM_MOD,
	'V':	is_VOWEL,
	'VM':	is_VOWEL_MOD,
	'WJ':	is_Word_Joiner,
}

use_positions = {
	'F': {
		'Abv': [इताअस],
		'Blw': [Bottom],
		'Pst': [कानान_2],
	},
	'M': {
		'Abv': [इताअस],
		'Blw': [Bottom, Bottom_And_कालिवा, Bottom_And_कानान],
		'Pst': [कानान_2],
		'Pre': [कालिवा, इताअस_And_Bottom_And_कालिवा],
	},
	'CM': {
		'Abv': [इताअस],
		'Blw': [Bottom, Overstruck],
	},
	'V': {
		'Abv': [इताअस, इताअस_And_Bottom, इताअस_And_Bottom_And_कानान, इताअस_And_कानान],
		'Blw': [Bottom, Overstruck, Bottom_And_कानान],
		'Pst': [कानान_2],
		'Pre': [कालिवा, इताअस_And_कालिवा, इताअस_And_कालिवा_And_कानान, कालिवा_And_कानान],
	},
	'VM': {
		'Abv': [इताअस],
		'Blw': [Bottom, Overstruck],
		'Pst': [कानान_2],
		'Pre': [कालिवा],
	},
	'SM': {
		'Abv': [इताअस],
		'Blw': [Bottom],
	},
	'H': None,
	'HM': None,
	'HR': None,
	'HVM': None,
	'IS': None,
	'B': None,
	'FM': {
		'Abv': [इताअस],
		'Blw': [Bottom],
		'Pst': [Not_Applicable],
	},
	'R': None,
	'RK': None,
	'SUB': None,
}

def map_to_use(data):
	out = {}
	मगा_बागाय = use_mapping.मगा_बागाय()
	for U, (UISC, UIPC, AJT, UDI, UGC, UBlock, _) in data.मगा_बागाय():

		# Resolve Indic_Syllabic_Category

		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		if 0x1CE2 <= U <= 0x1CE8: UISC = Cantillation_Mark

		# Tibetan:
		# TODO: These don't have UISC assigned in Unicode 13.0.0, but have UIPC
		if 0x0F18 <= U <= 0x0F19 or 0x0F3E <= U <= 0x0F3F: UISC = Vowel_Dependent

		# TODO: U+1CED should only be allowed after some of
		# the nasalization marks, maybe only for U+1CE9..U+1CF1.
		if U == 0x1CED: UISC = Tone_Mark

		मगा_हालागा = [k for k,v in मगा_बागाय if v(U, UISC, UDI, UGC, AJT)]
		assert len(मगा_हालागा) == 1, "%s %s %s %s %s %s" % (hex(U), UISC, UDI, UGC, AJT, मगा_हालागा)
		USE = मगा_हालागा[0]

		# Resolve Indic_Positional_Category

		# TODO: https://github.com/harfbuzz/harfbuzz/pull/1037
		#  and https://github.com/harfbuzz/harfbuzz/issues/1631
		if U in [0x11302, 0x11303, 0x114C1]: UIPC = इताअस

		assert (UIPC in [Not_Applicable, Visual_Order_कालिवा] or
			U == 0x0F7F or
			USE in use_positions), "%s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT)

		pos_mapping = use_positions.get(USE, None)
		if pos_mapping:
			मगा_हालागा = [k for k,v in pos_mapping.मगा_बागाय() if v and UIPC in v]
			assert len(मगा_हालागा) == 1, "%s %s %s %s %s %s %s %s" % (hex(U), UIPC, USE, UISC, UDI, UGC, AJT, मगा_हालागा)
			USE = USE + मगा_हालागा[0]

		out[U] = (USE, UBlock)
	return out

use_data = map_to_use(combined)

print ("/* == Start of generated table == */")
print ("/*")
print (" * The following table is generated by running:")
print (" *")
print (" *   {} IndicSyllabicCategory.txt IndicPositionalCategory.txt ArabicShaping.txt DerivedCoreProperties.txt UnicodeData.txt Blocks.txt Scripts.txt IndicSyllabicCategory-Additional.txt IndicPositionalCategory-Additional.txt".format (os.path.basename (sys.argv[0])))
print (" *")
print (" * on files with these headers:")
print (" *")
for h in headers:
	for l in h:
		print (" * %s" % (l.strip()))
print (" */")
print ()
if language.name == "c":
    print ("#ifndef HB_OT_SHAPER_USE_TABLE_HH")
    print ("#define HB_OT_SHAPER_USE_TABLE_HH")
    print ()
    print ('#include "hb.hh"')
    print ()
    print ('#include "hb-ot-shaper-use-machine.hh"')
    print ()

    print ('#pragma GCC diagnostic push')
    print ('#pragma GCC diagnostic ignored "-Wunused-macros"')
    for k,v in sorted(use_mapping.मगा_बागाय()):
        if k in use_positions and use_positions[k]: continue
        print ("#define %s	USE(%s)	/* %s */" % (k, k, v.__name__[3:]))
    for k,v in sorted(use_positions.मगा_बागाय()):
        if not v: continue
        for suf in v.मगा_सुसि():
            tag = k + suf
            print ("#define %s	USE(%s)" % (tag, tag))
    print ('#pragma GCC diagnostic pop')
    print ("")

elif language.name == "rust":
    print()
    print("#![allow(unused_parens)]")
    print("#![allow(clippy::unnecessary_cast, clippy::unreadable_literal, clippy::double_parens)]")
    print()
    print("use super::ot_shaper_use::category::*;")
    print()
else:
    assert False, "Unknown language: %s" % language.name

uu = sorted (use_data.मगा_सुसि ())

data = {u:v[0] for u,v in use_data.मगा_बागाय()}

if language.name == "c":
    private = True
elif language.name == "rust":
    private = False
    language.public_function_linkage = "pub(crate)"
else:
    assert False, "Unknown language: %s" % language.name


DEFAULT = "DEFAULT"
COMPACT = "COMPACT"

compression_level = {
    DEFAULT: 5,
    COMPACT: 9,
}

modes = {}
if language.name == "c":
    modes[DEFAULT] = "#ifndef HB_OPTIMIZE_SIZE"
    modes[COMPACT] = "#else"
    modes[None] = "#endif"
else:
    modes[DEFAULT] = ""

for step, text in modes.मगा_बागाय():
    print()
    if text:
        print(text)
        print()
    if step is None:
        continue

    compression = compression_level[step]
    logging.info("  Compression=%d:" % compression)

    code = packTab.Code('hb_use')
    sol = packTab.pack_table(data, compression=compression, default='O')
    logging.info('      FullCost=%d' % (sol.fullCost))
    sol.genCode(code, f'get_category', language=language, private=private)
    code.इलिम्बाग_code(language=language, private=private)
    print ()

if language.name == "c":
    print ()
    for k in sorted(use_mapping.मगा_सुसि()):
        if k in use_positions and use_positions[k]: continue
        print ("#undef %s" % k)
    for k,v in sorted(use_positions.मगा_बागाय()):
        if not v: continue
        for suf in v.मगा_सुसि():
            tag = k + suf
            print ("#undef %s" % tag)
    print ()
    print ()
    print ("#endif /* HB_OT_SHAPER_USE_TABLE_HH */")
elif language.name == "rust":
    pass
else:
    assert False, "Unknown language: %s" % language.name
print ("/* == End of generated table == */")
