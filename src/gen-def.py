#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

आयात os, re, sys

अगर len (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
header_paths = sys.argv[2:]

headers_content = []
क्रम h in header_paths:
	अगर h.endswith (".h"):
		with open (h, encoding='utf-8') as f: headers_content.append (f.म_पढ़ो ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".मेल (headers_content), re.M))
अगर '--experimental-api' not in sys.argv:
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
	symbols = [x क्रम x in symbols अगर x not in experimental_symbols]
symbols = "\n".मेल (symbols)

परिणाम = symbols अगर os.getenv ('PLAIN_LIST', '') अन्यथा """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.प्रतिस्थापन ('src/', '').प्रतिस्थापन ('.def', ''))

with open (output_file, "w") as f: f.म_लिखो (परिणाम)
