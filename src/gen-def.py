#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

استورد os, re, sys

إذا اوږدوالی (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
سرلیک_paths = sys.argv[2:]

سرلیکونه_content = []
لكل h في سرلیک_paths:
	إذا h.endswith (".h"):
		سره open (h, encoding='utf-8') كـ f: سرلیکونه_content.append (f.اقرأ ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (سرلیکونه_content), re.M))
إذا '--experimental-api' ليس في sys.argv:
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
	symbols = [x لكل x في symbols إذا x ليس في experimental_symbols]
symbols = "\n".join (symbols)

پایله = symbols إذا os.getenv ('PLAIN_LIST', '') وإلا """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

سره open (output_file, "w") كـ f: f.اكتب (پایله)
