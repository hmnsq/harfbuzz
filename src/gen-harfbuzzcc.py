#!/usr/bin/env python3

"This tool is intended to be used from meson"

దిగుమతి os, sys, shutil, pathlib

ఒకవేళ పొడవు (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_ఆధారం_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(సమితి(sys.argv[3:]))

తోడుగా open (OUTPUT, "wb") ఇవ f:
	f.లిఖ ("".join ('#include "{}"\n'.format (pathlib.మార్గం_2( os.మార్గం.relpath (os.మార్గం.abspath (x), CURRENT_ఆధారం_DIR) ).as_posix()) కొరకు x లో sources ఒకవేళ x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.మార్గం.join (CURRENT_ఆధారం_DIR, os.మార్గం.basename (OUTPUT))
తోడుగా open(baseline_filename, "rb") ఇవ baseline:
	తోడుగా open(OUTPUT, "rb") ఇవ generated:
		ఒకవేళ baseline.పఠ() != generated.పఠ():
			shutil.copyfile (OUTPUT, baseline_filename)
