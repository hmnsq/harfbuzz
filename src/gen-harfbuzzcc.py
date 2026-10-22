#!/usr/bin/env python3

"This tool is intended to be used from meson"

درآمد os, sys, shutil, pathlib

اگر لمبائی (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_ماخذ_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(سیٹ(sys.argv[3:]))

ساتھ open (OUTPUT, "wb") بطور f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.راستہ_2( os.راستہ.relpath (os.راستہ.abspath (x), CURRENT_ماخذ_DIR) ).as_posix()) برائے x میں sources اگر x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.راستہ.join (CURRENT_ماخذ_DIR, os.راستہ.basename (OUTPUT))
ساتھ open(baseline_filename, "rb") بطور baseline:
	ساتھ open(OUTPUT, "rb") بطور generated:
		اگر baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
