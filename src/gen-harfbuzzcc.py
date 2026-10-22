#!/usr/bin/env python3

"This tool is intended to be used from meson"

આયાત os, sys, shutil, pathlib

જો len (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(set(sys.argv[3:]))

with open (OUTPUT, "wb") ઇવ f:
	f.લિખ ("".join ('#include "{}"\n'.format (pathlib.Path( os.path.relpath (os.path.abspath (x), CURRENT_SOURCE_DIR) ).as_posix()) માટે x માં sources જો x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ઇવ baseline:
	with open(OUTPUT, "rb") ઇવ generated:
		જો baseline.પઠ() != generated.પઠ():
			shutil.copyfile (OUTPUT, baseline_filename)
