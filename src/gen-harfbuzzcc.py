#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, pathlib

إذا لمبائی (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_سروت_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(سیٹ(sys.argv[3:]))

نال open (OUTPUT, "wb") كـ f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.رستہ_2( os.رستہ.relpath (os.رستہ.abspath (x), CURRENT_سروت_DIR) ).as_posix()) لكل x في sources إذا x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.رستہ.join (CURRENT_سروت_DIR, os.رستہ.basename (OUTPUT))
نال open(baseline_filename, "rb") كـ baseline:
	نال open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
