#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

درآمد os, re, sys

اگر لمبائی (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
سرخی_paths = sys.argv[2:]

سرخیاں_content = []
برائے h میں سرخی_paths:
	اگر h.endswith (".h"):
		ساتھ open (h, encoding='utf-8') بطور f: سرخیاں_content.append (f.اقرأ ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (سرخیاں_content), re.M))
اگر '--experimental-api' نہیں میں sys.argv:
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
	symbols = [x برائے x میں symbols اگر x نہیں میں experimental_symbols]
symbols = "\n".join (symbols)

نتیجہ = symbols اگر os.getenv ('PLAIN_LIST', '') ورنہ """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

ساتھ open (output_file, "w") بطور f: f.اكتب (نتیجہ)
