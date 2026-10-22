#!/usr/bin/env python3

"This tool is intended to be used from meson"

ଆମଦାନୀ os, sys, shutil, pathlib

ଯଦି len (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(set(sys.argv[3:]))

with open (OUTPUT, "wb") ଇଵ f:
	f.ଲିଖ ("".join ('#include "{}"\n'.format (pathlib.Path( os.path.relpath (os.path.abspath (x), CURRENT_SOURCE_DIR) ).as_posix()) ପାଇଁ x ଭିତରେ sources ଯଦି x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ଇଵ baseline:
	with open(OUTPUT, "rb") ଇଵ generated:
		ଯଦି baseline.ପଠ() != generated.ପଠ():
			shutil.copyfile (OUTPUT, baseline_filename)
