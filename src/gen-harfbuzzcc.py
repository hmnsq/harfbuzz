#!/usr/bin/env python3

"This tool is intended to be used from meson"

இறக்குமதி os, sys, shutil, pathlib

எனில் len (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(set(sys.argv[3:]))

with open (OUTPUT, "wb") இவ f:
	f.லிக ("".join ('#include "{}"\n'.format (pathlib.Path( os.path.relpath (os.path.abspath (x), CURRENT_SOURCE_DIR) ).as_posix()) க்கு x உள் sources எனில் x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") இவ baseline:
	with open(OUTPUT, "rb") இவ generated:
		எனில் baseline.பட() != generated.பட():
			shutil.copyfile (OUTPUT, baseline_filename)
