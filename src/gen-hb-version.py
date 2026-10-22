#!/usr/bin/env python3

"This tool is intended to be used from meson"

આયાત os, sys, shutil, re

જો len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
આગત_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(આગત_2)

પ્રયાસ:
	with open (OUTPUT, "r", encoding='utf-8') ઇવ old_output:
		માટે લીટી માં old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", લીટી)
			જો old_version અને old_version[1] == version:
				sys.exit ()
અપવાદ IOError:
	pass

with open (આગત_2, "r", encoding='utf-8') ઇવ template:
	with open (OUTPUT, "wb") ઇવ output:
		output.લિખ (template.પઠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ઇવ baseline:
	with open(OUTPUT, "rb") ઇવ generated:
		જો baseline.પઠ() != generated.પઠ():
			shutil.copyfile (OUTPUT, baseline_filename)
