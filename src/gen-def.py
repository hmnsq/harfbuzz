#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

இறக்குமதி os, re, sys

எனில் len (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
header_paths = sys.argv[2:]

headers_content = []
க்கு h உள் header_paths:
	எனில் h.endswith (".h"):
		with open (h, encoding='utf-8') இவ f: headers_content.append (f.பட ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (headers_content), re.M))
எனில் '--experimental-api' இல்லை உள் sys.argv:
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
	symbols = [x க்கு x உள் symbols எனில் x இல்லை உள் experimental_symbols]
symbols = "\n".join (symbols)

result = symbols எனில் os.getenv ('PLAIN_LIST', '') இல்லையெனில் """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

with open (output_file, "w") இவ f: f.லிக (result)
