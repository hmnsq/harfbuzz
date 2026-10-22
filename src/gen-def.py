#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

దిగుమతి os, re, sys

ఒకవేళ పొడవు (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
శీర్షిక_paths = sys.argv[2:]

శీర్షికలు_content = []
కొరకు h లో శీర్షిక_paths:
	ఒకవేళ h.endswith (".h"):
		తోడుగా open (h, encoding='utf-8') ఇవ f: శీర్షికలు_content.append (f.పఠ ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (శీర్షికలు_content), re.M))
ఒకవేళ '--experimental-api' కాదు లో sys.argv:
	# Move these to harfbuzz-sections.txt when got stable
	experimental_symbols = \
"""hb_shape_justify
hb_subset_input_override_name_table
hb_subset_input_to_string_or_fail
hb_subset_cff_get_charstring_data
hb_subset_cff_get_charstrings_index
hb_subset_cff2_get_charstring_data
hb_subset_cff2_get_charstrings_index
hb_subset_depend_from_face_or_fail
hb_subset_depend_lookup_glyph
hb_subset_depend_lookup_set
hb_subset_depend_destroy
""".splitlines ()
	symbols = [x కొరకు x లో symbols ఒకవేళ x కాదు లో experimental_symbols]
symbols = "\n".join (symbols)

ఫలితం = symbols ఒకవేళ os.getenv ('PLAIN_LIST', '') లేకపోతే """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

తోడుగా open (output_file, "w") ఇవ f: f.లిఖ (ఫలితం)
