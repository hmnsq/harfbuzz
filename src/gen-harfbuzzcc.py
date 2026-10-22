#!/usr/bin/env python3

"This tool is intended to be used from meson"

استورد os, sys, shutil, pathlib

إذا ڊيگهه (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_ذريعو_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(سيٽ(sys.argv[3:]))

سان open (OUTPUT, "wb") كـ f:
	f.اكتب ("".join ('#include "{}"\n'.format (pathlib.رستو_2( os.رستو.relpath (os.رستو.abspath (x), CURRENT_ذريعو_DIR) ).as_posix()) لكل x في sources إذا x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.رستو.join (CURRENT_ذريعو_DIR, os.رستو.basename (OUTPUT))
سان open(baseline_filename, "rb") كـ baseline:
	سان open(OUTPUT, "rb") كـ generated:
		إذا baseline.اقرأ() != generated.اقرأ():
			shutil.copyfile (OUTPUT, baseline_filename)
