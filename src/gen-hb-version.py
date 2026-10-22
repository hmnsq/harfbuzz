#!/usr/bin/env python3

"This tool is intended to be used from meson"

ଆମଦାନୀ os, sys, shutil, re

ଯଦି len (sys.argv) < 4:
	sys.exit(__doc__)

version = sys.argv[1]
major, minor, micro = version.split (".")

OUTPUT = sys.argv[2]
ନିବେଶ_2 = sys.argv[3]
CURRENT_SOURCE_DIR = os.path.dirname(ନିବେଶ_2)

ଚେଷ୍ଟା:
	with open (OUTPUT, "r", encoding='utf-8') ଇଵ old_output:
		ପାଇଁ ଧାଡ଼ି ଭିତରେ old_output:
			old_version = re.match (r"#define HB_VERSION_STRING \"(\d.\d.\d)\"", ଧାଡ଼ି)
			ଯଦି old_version ଏବଂ old_version[1] == version:
				sys.exit ()
ବ୍ୟତିକ୍ରମ IOError:
	pass

with open (ନିବେଶ_2, "r", encoding='utf-8') ଇଵ template:
	with open (OUTPUT, "wb") ଇଵ output:
		output.ଲିଖ (template.ପଠ ()
			.replace ("@HB_VERSION_MAJOR@", major)
			.replace ("@HB_VERSION_MINOR@", minor)
			.replace ("@HB_VERSION_MICRO@", micro)
			.replace ("@HB_VERSION@", version)
			.encode ())

# copy it also to the source tree, but only if it has changed
baseline_filename = os.path.join (CURRENT_SOURCE_DIR, os.path.basename (OUTPUT))
with open(baseline_filename, "rb") ଇଵ baseline:
	with open(OUTPUT, "rb") ଇଵ generated:
		ଯଦି baseline.ପଠ() != generated.ପଠ():
			shutil.copyfile (OUTPUT, baseline_filename)
