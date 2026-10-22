#!/usr/bin/env python3

"usage: gen-def.py harfbuzz.def hb.h [hb-blob.h hb-buffer.h ...]"

ਆਯਾਤ os, re, sys

ਜੇ ਲੰਬਾਈ (sys.argv) < 3:
	sys.exit(__doc__)

output_file = sys.argv[1]
ਸਿਰਲੇਖ_paths = sys.argv[2:]

ਸਿਰਲੇਖਾਂ_content = []
ਲਈ h ਵਿੱਚ ਸਿਰਲੇਖ_paths:
	ਜੇ h.endswith (".h"):
		ਨਾਲ open (h, encoding='utf-8') ਇਵ f: ਸਿਰਲੇਖਾਂ_content.append (f.ਪਠ ())

symbols = sorted (re.findall (r"^hb_\w+(?= \()", "\n".join (ਸਿਰਲੇਖਾਂ_content), re.M))
ਜੇ '--experimental-api' ਨਹੀਂ ਵਿੱਚ sys.argv:
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
	symbols = [x ਲਈ x ਵਿੱਚ symbols ਜੇ x ਨਹੀਂ ਵਿੱਚ experimental_symbols]
symbols = "\n".join (symbols)

ਨਤੀਜਾ = symbols ਜੇ os.getenv ('PLAIN_LIST', '') ਨਹੀਂਤਾਂ """EXPORTS
%s
LIBRARY lib%s-0.dll""" % (symbols, output_file.replace ('src/', '').replace ('.def', ''))

ਨਾਲ open (output_file, "w") ਇਵ f: f.ਲਿਖ (ਨਤੀਜਾ)
