#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, pathlib

إذا اوږدوالی (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_سرچینه_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(سېټ(sys.argv[3:]))

سره open (OUTPUT, "wb") كـ f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.لار_2( os.لار.relpath (os.لار.abspath (x), CURRENT_سرچینه_DIR) ).as_posix()) لكل x في sources إذا x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.لار.join (CURRENT_سرچینه_DIR, os.لار.basename (OUTPUT))
سره open(baseline_filename, "rb") كـ baseline:
	سره open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
