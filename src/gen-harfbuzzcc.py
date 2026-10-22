#!/usr/bin/env python3

"This tool is intended to be used from meson"

ਆਯਾਤ os, sys, shutil, pathlib

ਜੇ ਲੰਬਾਈ (sys.argv) < 3:
	sys.exit (__doc__)

OUTPUT = sys.argv[1]
CURRENT_ਸਰੋਤ_DIR = sys.argv[2]

# make sure input files are unique
sources = sorted(ਸੈੱਟ(sys.argv[3:]))

ਨਾਲ open (OUTPUT, "wb") ਇਵ f:
	f.ਲਿਖ ("".join ('#include "{}"\n'.format (pathlib.ਰਸਤਾ_2( os.ਰਸਤਾ.relpath (os.ਰਸਤਾ.abspath (x), CURRENT_ਸਰੋਤ_DIR) ).as_posix()) ਲਈ x ਵਿੱਚ sources ਜੇ x.endswith (".cc")).encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.ਰਸਤਾ.join (CURRENT_ਸਰੋਤ_DIR, os.ਰਸਤਾ.basename (OUTPUT))
ਨਾਲ open(baseline_filename, "rb") ਇਵ baseline:
	ਨਾਲ open(OUTPUT, "rb") ਇਵ generated:
		ਜੇ baseline.ਪਠ() != generated.ਪਠ():
			shutil.copyfile (OUTPUT, baseline_filename)
