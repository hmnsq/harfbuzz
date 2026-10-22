#!/usr/bin/env python3

"This tool is intended to be used from meson"

आयात os, sys, shutil, pathlib

अगर len (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_SOURCE_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(set(sys.argv[3:]))

with open (OUTPUT, "wb") as f:
	f.म_लिखो ("".मेल ('#include "{}"\n'.format (pathlib.Path( os.path.relpath (os.path.abspath (x), CURRENT_SOURCE_DIR) ).as_posix()) क्रम x in sources अगर x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.मेल (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") as baseline:
	with open(OUTPUT, "rb") as generated:
		अगर baseline.म_पढ़ो() != generated.म_पढ़ो():
			shutil.copyfile (OUTPUT, baseline_filename)
