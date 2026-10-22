#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

ഇറക്കുമതി os, re, sys

എങ്കിൽ len (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
header_paths = sys.argv[2:]

headers_content = []
വേണ്ടി h ഇൽ header_paths:
	എങ്കിൽ h.endswith (".h"):
		with open (h, encoding='utf-8') ഇവ f: headers_content.append (f.പഠ ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (headers_content), re.M))
എങ്കിൽ '--experimental-api' അല്ല ഇൽ sys.argv:
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
	symbols = [x വേണ്ടി x ഇൽ symbols എങ്കിൽ x അല്ല ഇൽ experimental_symbols]
symbols = "\n".join (symbols)

result = symbols എങ്കിൽ os.getenv ('PLAIN_LIST', '') അല്ലെങ്കിൽ """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

with open (output_file, "w") ഇവ f: f.ലിഖ (result)
