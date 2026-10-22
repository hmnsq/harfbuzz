#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, pathlib

إذا طول (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_منبع_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(مجموعه(sys.argv[3:]))

با open (OUTPUT, "wb") كـ f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.مسیر_2( os.مسیر.relpath (os.مسیر.abspath (x), CURRENT_منبع_DIR) ).as_posix()) لكل x في sources إذا x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.مسیر.join (CURRENT_منبع_DIR, os.مسیر.basename (OUTPUT))
با open(baseline_filename, "rb") كـ baseline:
	با open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
