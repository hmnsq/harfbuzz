#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, pathlib

𐤀𐤌 len (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(set(sys.argv[3:]))

with open (OUTPUT, "wb") كـ f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.Path( os.path.relpath (os.path.abspath (x), CURRENT_SOURCE_DIR) ).as_posix()) لكل x في sources 𐤀𐤌 x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") كـ baseline:
	with open(OUTPUT, "rb") كـ generated:
		𐤀𐤌 baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
